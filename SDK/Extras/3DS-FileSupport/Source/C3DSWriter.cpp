/*  NAME:
        C3DSWriter.cp

    DESCRIPTION:
       Main class implementing the Writer.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
	  This program is  free  software;  you can redistribute it and/or modify it
	  under the terms of the  GNU Lesser General Public License  as published by 
	  the  Free Software Foundation;  either version 2.1 of the License,  or (at 
	  your option) any later version.
	 
	  This  program  is  distributed in  the  hope that it will  be useful,  but
	  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
	  or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public  
	  License for more details.
	 
	  You should  have received  a copy of the GNU Lesser General Public License
	  along with  this program;  if not, write to the  Free Software Foundation,
	  Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
  
    ___________________________________________________________________________
*/
#include "C3DSWriter.h"
#include "3DS-FileSupport_Prefix.pch"

#if __MACH__
	#include <Quesa/Quesa.h>
	#include <Quesa/CQ3ObjectRef.h>
	#include <Quesa/QuesaExtension.h>
	#include <Quesa/QuesaStorage.h>
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/QuesaMath.h>
#else
	#include <Quesa.h>
	#include <CQ3ObjectRef.h>
	#include <QuesaExtension.h>
	#include <QuesaStorage.h>
	#include <QuesaGroup.h>
	#include <QuesaMath.h>
#endif

#include <cstring>
#include <string>
#include <sstream>
#include <map>
#include <vector>


#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
#include <lib3ds/node.h>
#include <lib3ds/material.h>
#include <lib3ds/io.h>

namespace
{	
	typedef		std::map< std::string, CQ3ObjectRef >	NameToTexture;
	typedef		std::map< std::string, CQ3ObjectRef >	NameToTexture;
}

struct stateStackItem{
	TQ3Matrix4x4	matrix;
	TQ3ColorRGB		diffuseColor;
	TQ3ColorRGB		specularColor;
};

#pragma mark struct X3DSWriterImp;
struct X3DSWriterImp
{
							X3DSWriterImp(
									C3DSWriter* inSelf,
									TQ3FFormatBaseData* inData );

	
	TQ3Status				StartFile(
									TQ3ViewObject				theView,
									TQ3FFormatBaseData			*fileFormatPrivate,
									TQ3DrawContextObject		theDrawContext);
	
	TQ3ViewStatus			EndPass(
									TQ3ViewObject				theView,
									TQ3FFormatBaseData		*fileFormatPrivate);
	
	TQ3Status				Close( TQ3Boolean inAbort );
	
	void					Write_TriMesh( TQ3TriMeshData *geomData );
	
	void					Write_Triangle(TQ3TriangleData *geomData);
	
	void					UpdateAttributes( TQ3AttributeSet theAttributes );
	void					UpdateMatrix( TQ3Matrix4x4* theMatrix );
	void					PushGroupLevel();
	void					PopGroupLevel();

	const char*				GetMaterial();
	Lib3dsMesh*				CreateLib3dsMesh(TQ3TriMeshData *meshData);
	Lib3dsMesh*				CreateLib3dsTriangle(TQ3TriangleData *triData);
	void					InsertMeshAndCreateNode( Lib3dsMesh* mesh );

	void					QuesaToLib3dsPoints(TQ3Point3D *src, Lib3dsVector *dest, int numPoints);
	void					QuesaToLib3dsPoint(TQ3Point3D src, Lib3dsVector *dest);
	void					QuesaToLib3dsFaces(TQ3TriMeshData* facesQuesa, Lib3dsMesh *meshData);


	C3DSWriter*						mSelf;
	Lib3dsFile*						m3DSfileRef;
	TQ3FFormatBaseData*				mBaseData;
	
	std::vector<stateStackItem>		mStateStack;
	stateStackItem					mCurrentState;
	
	std::auto_ptr<std::ostringstream>	mDebugStream;
};

#pragma mark lib3DS Compatibility functions;

static Lib3dsBool
Q3DSWriter_fileio_error_func(void *self)
{
  X3DSWriterImp* writer = (X3DSWriterImp*)self;
  return(false);
}


static long
Q3DSWriter_fileio_seek_func(void *self, long offset, Lib3dsIoSeek origin)
{
  X3DSWriterImp* writer = (X3DSWriterImp*)self;
  TQ3FFormatBaseData *instanceData = (TQ3FFormatBaseData *) writer->mBaseData;

  switch (origin) {
    case LIB3DS_SEEK_SET:
      instanceData->currentStoragePosition = offset;
      break;
    case LIB3DS_SEEK_CUR:
      instanceData->currentStoragePosition += offset;
      break;
    default:
//      Q3_ASSERT(0);
      return(-1);
  }
  return (0);
}


static long
Q3DSWriter_fileio_tell_func(void *self)
{
	X3DSWriterImp* writer = (X3DSWriterImp*)self;
	TQ3FFormatBaseData *instanceData = (TQ3FFormatBaseData *) writer->mBaseData;
	return(instanceData->currentStoragePosition);
}


static int
Q3DSWriter_fileio_read_func(void *self, Lib3dsByte *buffer, int size)
{
	X3DSWriterImp* writer = (X3DSWriterImp*)self;
	TQ3Uns32 sizeRead = 0;
	TQ3FFormatBaseData *instanceData = (TQ3FFormatBaseData *) writer->mBaseData;

	Q3Storage_GetData(instanceData->storage,
							instanceData->currentStoragePosition,
							size, (TQ3Uns8*)buffer, &sizeRead);

//	Q3_ASSERT(sizeRead == size);
	instanceData->currentStoragePosition += sizeRead;

	return sizeRead;							 
}


static int
Q3DSWriter_fileio_write_func(void *self, const Lib3dsByte *buffer, int size)
{
	X3DSWriterImp* writer = (X3DSWriterImp*)self;
	TQ3Uns32 sizeWrite = 0;
	TQ3FFormatBaseData *instanceData = (TQ3FFormatBaseData *) writer->mBaseData;

	Q3Storage_SetData(instanceData->storage,
							instanceData->currentStoragePosition,
							size, (TQ3Uns8*)buffer, &sizeWrite);

	//Q3_ASSERT(sizeWrite == length);
	instanceData->currentStoragePosition += sizeWrite;

	return sizeWrite;							 
}

#pragma mark X3DSWriter Implementation;


X3DSWriterImp::X3DSWriterImp (C3DSWriter* inSelf,
							TQ3FFormatBaseData* inData )
	: mSelf( inSelf )
	, mBaseData( inData )
{
	mBaseData->noMoreObjects = kQ3False;
	mBaseData->fileVersion = 0;	// should be reset in ReadHeader
	
	m3DSfileRef = NULL;
	
	memset(&mCurrentState, 0 , sizeof(mCurrentState));
	Q3Matrix4x4_SetIdentity(&mCurrentState.matrix);
}




/*!
	@function			StartFile
	@abstract			.
*/
TQ3Status	X3DSWriterImp::StartFile( TQ3ViewObject				theView,
									TQ3FFormatBaseData			*fileFormatPrivate,
									TQ3DrawContextObject		theDrawContext )
{
	m3DSfileRef = lib3ds_file_new();
	return (m3DSfileRef != NULL) ? kQ3Success : kQ3Failure;
}


/*!
	@function			EndPass
	@abstract			.
*/
TQ3ViewStatus	X3DSWriterImp::EndPass( TQ3ViewObject			theView,
									TQ3FFormatBaseData		*fileFormatPrivate)
{
	return kQ3ViewStatusDone;
}


/*!
	@function			Close
	@abstract			The file is being closed.  If there is a debug
						stream, it is finished and returned to the client
						now.
*/
TQ3Status	X3DSWriterImp::Close(TQ3Boolean inAbort)
{
	TQ3Status status = kQ3Success;
	if(m3DSfileRef){
		if (inAbort == kQ3False){ // do the real saving
			status = kQ3Failure;
			
			Lib3dsIo *io = lib3ds_io_new(	// prepare the file handling function pointers used by the 3dslib 
										this, 
										Q3DSWriter_fileio_error_func,
										Q3DSWriter_fileio_seek_func,
										Q3DSWriter_fileio_tell_func,
										Q3DSWriter_fileio_read_func,
										Q3DSWriter_fileio_write_func
									  );

			if (io) {
				status = lib3ds_file_write(m3DSfileRef, io) ?kQ3Success : kQ3Failure ;
				lib3ds_io_free(io);
				}

			}
		lib3ds_file_free(m3DSfileRef);
		m3DSfileRef = NULL;
	}
	
	if (mDebugStream.get() != NULL)
	{
		if (inAbort)
		{
			*mDebugStream << std::endl << "Import file closing abnormally." <<
				std::endl;
		}
		else
		{
			*mDebugStream << std::endl << "Import file closing normally." <<
				std::endl;
		}
		*mDebugStream << "===========" << std::endl;
		std::string		debugText( mDebugStream->str() );
		Q3Object_SetProperty( mBaseData->storage, kDebugTextProperty, debugText.size(),
			debugText.data() );
	}
	
	return kQ3Success;
}





void	X3DSWriterImp::InsertMeshAndCreateNode( Lib3dsMesh* mesh )
{
	if(mesh){
		Lib3dsQuatKey* key_q;
		Lib3dsLin3Key* key_l;

		lib3ds_file_insert_mesh(m3DSfileRef,mesh);

		// create a node
		Lib3dsNode	*node;
		node = lib3ds_node_new_object();
		strncpy(node->name, mesh->name, sizeof(node->name));
		node->parent_id = LIB3DS_NO_PARENT;
		static int node_id = 0;
		node->node_id = node_id;
		node_id++;

		node->data.object.rot_track.keyL = 0;
		key_q = lib3ds_quat_key_new();
		key_q->q[0] = 0;
		key_q->q[1] = 0;
		key_q->q[2] = 0;
		key_q->q[3] = 1;
		lib3ds_quat_track_insert(&node->data.object.rot_track, key_q);
		lib3ds_quat_track_setup(&node->data.object.rot_track);

		node->data.object.pos_track.keyL = 0;
		key_l = lib3ds_lin3_key_new();
		lib3ds_lin3_track_insert(&node->data.object.pos_track, key_l);
		lib3ds_lin3_track_setup(&node->data.object.pos_track);

		node->data.object.scl_track.keyL = 0;
		key_l = lib3ds_lin3_key_new();
		key_l->value[0] = 1;
		key_l->value[1] = 1;
		key_l->value[2] = 1;
		lib3ds_lin3_track_insert(&node->data.object.scl_track, key_l);
		lib3ds_lin3_track_setup(&node->data.object.scl_track);
		lib3ds_file_insert_node(m3DSfileRef,node);

		}
}




void	X3DSWriterImp::Write_TriMesh( TQ3TriMeshData *geomData )
{
	Lib3dsMesh* mesh = CreateLib3dsMesh(geomData);
	if(mesh){
		InsertMeshAndCreateNode(mesh);
		}

}






void	X3DSWriterImp::Write_Triangle(TQ3TriangleData *geomData )
{
	Lib3dsMesh* mesh = CreateLib3dsTriangle(geomData);
	if(mesh){
		InsertMeshAndCreateNode(mesh);
		}
}



void	X3DSWriterImp::UpdateAttributes( TQ3AttributeSet theAttributes )
{
	TQ3ColorRGB diffuseColor;
	TQ3Status qd3dstatus = Q3AttributeSet_Get (theAttributes, kQ3AttributeTypeDiffuseColor, &diffuseColor);
	if(qd3dstatus == kQ3Success){
		mCurrentState.diffuseColor = diffuseColor;
		}
		
	TQ3ColorRGB specularColor;
	qd3dstatus = Q3AttributeSet_Get (theAttributes, kQ3AttributeTypeSpecularColor, &specularColor);
	if(qd3dstatus == kQ3Success){
		mCurrentState.specularColor = specularColor;
		}
}



void	X3DSWriterImp::UpdateMatrix( TQ3Matrix4x4* theMatrix )
{
	Q3Matrix4x4_Multiply(theMatrix,&mCurrentState.matrix,&mCurrentState.matrix);
}



void	X3DSWriterImp::PushGroupLevel( )
{
	mStateStack.push_back(mCurrentState);
}


void	X3DSWriterImp::PopGroupLevel( )
{
	mCurrentState = mStateStack.back();
	mStateStack.pop_back();
}


const char*		X3DSWriterImp::GetMaterial()
{
	Lib3dsMaterial* foundMaterial = m3DSfileRef->materials;
	
	//find an Equivalent Material;
	while (foundMaterial != NULL){
		if(
			foundMaterial->diffuse[0] == mCurrentState.diffuseColor.r &&
			foundMaterial->diffuse[1] == mCurrentState.diffuseColor.g &&
			foundMaterial->diffuse[2] == mCurrentState.diffuseColor.b &&
			
			foundMaterial->specular[0] == mCurrentState.specularColor.r &&
			foundMaterial->specular[1] == mCurrentState.specularColor.g &&
			foundMaterial->specular[2] == mCurrentState.specularColor.b 
			)
			break;
		else
			foundMaterial = foundMaterial->next;
		}
	
	if (foundMaterial == NULL){
		foundMaterial =lib3ds_material_new();
		
		sprintf(foundMaterial->name,"Ma%X",(long)foundMaterial);
		foundMaterial->name[10] = 0; // max len 10
		
//		foundMaterial->ambient[0] = foundMaterial->ambient[1] = foundMaterial->ambient[2] = 0.0f;
//		foundMaterial->ambient[0] = 1.0f;
		foundMaterial->diffuse[0] = mCurrentState.diffuseColor.r;
		foundMaterial->diffuse[1] = mCurrentState.diffuseColor.g;
		foundMaterial->diffuse[2] = mCurrentState.diffuseColor.b;
		
		foundMaterial->specular[0] = mCurrentState.specularColor.r;
		foundMaterial->specular[1] = mCurrentState.specularColor.g;
		foundMaterial->specular[2] = mCurrentState.specularColor.b;

		lib3ds_file_insert_material(m3DSfileRef,foundMaterial);
	}
	
	return foundMaterial->name;
}


Lib3dsMesh*		X3DSWriterImp::CreateLib3dsMesh(TQ3TriMeshData *meshData)
{
	if(meshData == NULL) return NULL;
	
	
	if(meshData->triMeshAttributeSet){ // push the geometry attribute set
		PushGroupLevel( );
		UpdateAttributes( meshData->triMeshAttributeSet );
		}
	
	Lib3dsMesh* mesh;
	char s[1024];

	sprintf(s,"Me%X",(long)meshData);
	
	s[10] = 0; // max len 10
	
	mesh=lib3ds_mesh_new(s);
	
	if( ! mesh)
		return NULL;
	
	if( ! lib3ds_mesh_new_point_list(mesh,meshData->numPoints)){
		lib3ds_mesh_free(mesh);
		return NULL;
		}
		
	if( ! lib3ds_mesh_new_face_list(mesh,meshData->numTriangles)){
		lib3ds_mesh_free(mesh);
		return NULL;
		}
		
	QuesaToLib3dsPoints(meshData->points, (Lib3dsVector*)mesh->pointL, meshData->numPoints);
	QuesaToLib3dsFaces (meshData, mesh);
	
	if(meshData->triMeshAttributeSet){ // pop the geometry attribute set
		PopGroupLevel( );
		}
	
	return mesh;
}


Lib3dsMesh*		X3DSWriterImp::CreateLib3dsTriangle(TQ3TriangleData *triData)
{
	if(triData == NULL) return NULL;
	
	if(triData->triangleAttributeSet){ // push the geometry attribute set
		PushGroupLevel( );
		UpdateAttributes( triData->triangleAttributeSet );
		}
	
	Lib3dsMesh* mesh;
	char s[1024];

	sprintf(s,"Tr%X",(long)triData);
	s[10] = 0; // max len 10
	
	mesh=lib3ds_mesh_new(s);
	
	if( ! mesh)
		return NULL;
	
	if( ! lib3ds_mesh_new_point_list(mesh,3)){
		lib3ds_mesh_free(mesh);
		return NULL;
		}
		
	if( ! lib3ds_mesh_new_face_list(mesh,1)){
		lib3ds_mesh_free(mesh);
		return NULL;
		}
		
	for(int i=0; i<3; i++){
		QuesaToLib3dsPoint(triData->vertices[i].point, &mesh->pointL[i].pos);
		}
		
	memset(&mesh->faceL[0],0,sizeof(mesh->faceL[0]));
	strcpy(mesh->faceL[0].material,GetMaterial());
	mesh->faceL[0].points[0] = 0;
	mesh->faceL[0].points[1] = 2;
	mesh->faceL[0].points[2] = 1;
	
	if(triData->triangleAttributeSet){ // pop the geometry attribute set
		PopGroupLevel( );
		}
	
	return mesh;
}



void	X3DSWriterImp::QuesaToLib3dsPoints(TQ3Point3D *src, Lib3dsVector *dest, int numPoints)
{
	for(int i=0; i<numPoints; i++){
			
		QuesaToLib3dsPoint(*src, dest);
		
		src++;
		dest++;
		}
}


void	X3DSWriterImp::QuesaToLib3dsPoint(TQ3Point3D src, Lib3dsVector *dest)
{
		Q3Point3D_Transform(&src,&mCurrentState.matrix,&src);
		(*dest)[0] = src.x;
		(*dest)[2] = src.y;
		(*dest)[1] = -src.z;
		
}


void	X3DSWriterImp::QuesaToLib3dsFaces(TQ3TriMeshData* meshData, Lib3dsMesh *mesh)
{
	if(meshData == NULL) return;
	if(mesh == NULL) return;
	static int smooth = 1; // needed a  way to find smoothed surfaces
	
	
	for(int i=0; i<mesh->faces; i++){
		memset(&mesh->faceL[i],0,sizeof(mesh->faceL[i]));
		strcpy(mesh->faceL[i].material,GetMaterial());
		mesh->faceL[i].points[0] = meshData->triangles[i].pointIndices[0];
		mesh->faceL[i].points[1] = meshData->triangles[i].pointIndices[2];
		mesh->faceL[i].points[2] = meshData->triangles[i].pointIndices[1];
		mesh->faceL[i].smoothing = smooth;
		}
	smooth++;
}





#pragma mark -

C3DSWriter::C3DSWriter( TQ3FFormatBaseData* inData )
	: mImp( new X3DSWriterImp( this, inData ) )
{
}

C3DSWriter::~C3DSWriter()
{
}

/*!
	@function			FromFileFormat
	@abstract			Retrieve a C3DSWriter pointer given a file format
						object.	(Static method)
*/
C3DSWriter*	C3DSWriter::FromFileFormat( TQ3FileFormatObject inFormat )
{
	TQ3XObjectClass theFormatClass = Q3XObject_GetClass( inFormat );
	TQ3FFormatBaseData*	baseData = static_cast<TQ3FFormatBaseData*>(
		Q3XObjectClass_GetPrivate( theFormatClass, inFormat ) );
	C3DSWriter*	writer = reinterpret_cast<C3DSWriter*>(
		baseData->reserved1 );
	return writer;
}

/*!
	@function			FromFile
	@abstract			Retrieve a C3DSWriter pointer given a file
						object.	(Static method)
*/
C3DSWriter*	C3DSWriter::FromFile( TQ3FileObject inFile )
{
	TQ3FileFormatObject format = Q3File_GetFileFormat( inFile );
	C3DSWriter*	writer = C3DSWriter::FromFileFormat( format );
	return writer;
}


/*!
	@function			StartFile
	@abstract			.
	@discussion			.
	@result				.
*/
TQ3Status	C3DSWriter::StartFile(
								TQ3ViewObject				theView,
								TQ3FFormatBaseData			*fileFormatPrivate,
								TQ3DrawContextObject		theDrawContext)
{
	return mImp->StartFile( theView, fileFormatPrivate, theDrawContext );
}

/*!
	@function			EndPass
	@abstract			.
	@result				.
*/
TQ3ViewStatus	C3DSWriter::EndPass(
								TQ3ViewObject				theView,
								TQ3FFormatBaseData		*fileFormatPrivate)
{
	return mImp->EndPass( theView, fileFormatPrivate );
}
	


/*!
	@function			Close
	@abstract			The file is being closed.  If there is a debug
						stream, it is finished and returned to the client
						now.
*/
TQ3Status	C3DSWriter::Close( TQ3Boolean inAbort )
{
	return mImp->Close( inAbort );
}



/*!
	@function			Write_TriMesh
	@abstract			Trimesh Handler.
*/
void	C3DSWriter::Write_TriMesh( TQ3TriMeshData *geomData )
{
	mImp->Write_TriMesh( geomData );
}




/*!
	@function			Write_Triangle
	@abstract			Triangle Handler.
*/
void	C3DSWriter::Write_Triangle(TQ3TriangleData *geomData )
{
	mImp->Write_Triangle( geomData );
}




/*!
	@function			UpdateAttributes
	@abstract			An Attribute set is submitted, so let's update our material state.
*/
void	C3DSWriter::UpdateAttributes( TQ3AttributeSet theAttributes )
{
	mImp->UpdateAttributes( theAttributes );
}



/*!
	@function			UpdateMatrix
	@abstract			A new Matrix is submitted, so let's update our matrix state.
*/
void	C3DSWriter::UpdateMatrix( TQ3Matrix4x4* theMatrix )
{
	mImp->UpdateMatrix( theMatrix );
}



/*!
	@function			PushGroupLevel
	@abstract			Add a frame to our matrix stack.
*/
void	C3DSWriter::PushGroupLevel( )
{
	mImp->PushGroupLevel( );
}



/*!
	@function			PopGroupLevel
	@abstract			Pop a frame from our matrix stack .
*/
void	C3DSWriter::PopGroupLevel( )
{
	mImp->PopGroupLevel( );
}

/*!
	@function			GetDebugStream
	@abstract			Accessor for a debug text stream to be used by
						node handlers.
	@result				An output stream pointer, or NULL.
*/
std::ostream*	C3DSWriter::GetDebugStream()
{
	return mImp->mDebugStream.get();
}


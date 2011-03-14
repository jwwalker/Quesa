/*  NAME:
        C3DSReader.cp

    DESCRIPTION:
       Main class implementing the reader.

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
#include "C3DSReader.h"
#include "3DS-FileSupport_Prefix.pch"


#if __MACH__
	#include <Quesa/Quesa.h>
	#include <Quesa/CQ3ObjectRef.h>
	#include <Quesa/QuesaExtension.h>
	#include <Quesa/QuesaStorage.h>
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaMath.h>
#else
	#include <Quesa.h>
	#include <CQ3ObjectRef.h>
	#include <QuesaExtension.h>
	#include <QuesaStorage.h>
	#include <QuesaGroup.h>
	#include <QuesaGeometry.h>
	#include <QuesaMath.h>
#endif

#include <cstring>
#include <string>
#include <sstream>
#include <map>

#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
#include <lib3ds/io.h>

namespace
{	
	typedef		std::map< std::string, CQ3ObjectRef >	NameToTexture;
	typedef		std::map< std::string, CQ3ObjectRef >	NameToTexture;
}

#pragma mark struct X3DSReaderImp;
struct X3DSReaderImp
{
							X3DSReaderImp(
									C3DSReader* inSelf,
									TQ3FFormatBaseData* inData );

	CQ3ObjectRef			NodeToObject();
	bool					ReadHeader();
	TQ3Object				ReadObject();
	
	void					CreateQuesaObjects();
	TQ3Object				CreateQuesaMesh(Lib3dsMesh *meshData);

	void					Lib3dsToQuesaPoint(Lib3dsVector*src, TQ3Point3D *dest, int numPoints);
	void					Lib3dsToQuesaFaces(Lib3dsMesh *meshData, TQ3TriMeshTriangleData* facesQuesa);
	
	
	void					Close( TQ3Boolean inAbort );

	void					CacheExternalTexture( const char* inURL,
												CQ3ObjectRef& inTexture );
	bool					GetCachedExternalTexture( const char* inURL,
												CQ3ObjectRef& outTexture ) const;

	C3DSReader*				mSelf;
	Lib3dsFile*				m3DSfileRef;
	TQ3FFormatBaseData*		mBaseData;
	std::auto_ptr<std::ostringstream>	mDebugStream;
	NameToTexture			mTextureCache;
	TQ3Object				mModel;
};

#pragma mark lib3DS Compatibility functions;

static Lib3dsBool
Q3DSReader_fileio_error_func(void *self)
{
  X3DSReaderImp* reader = (X3DSReaderImp*)self;
  return(false);
}


static long
Q3DSReader_fileio_seek_func(void *self, long offset, Lib3dsIoSeek origin)
{
  X3DSReaderImp* reader = (X3DSReaderImp*)self;
  TQ3FFormatBaseData *instanceData = (TQ3FFormatBaseData *) reader->mBaseData;

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
Q3DSReader_fileio_tell_func(void *self)
{
	X3DSReaderImp* reader = (X3DSReaderImp*)self;
	TQ3FFormatBaseData *instanceData = (TQ3FFormatBaseData *) reader->mBaseData;
	return(instanceData->currentStoragePosition);
}


static int
Q3DSReader_fileio_read_func(void *self, Lib3dsByte *buffer, int size)
{
	X3DSReaderImp* reader = (X3DSReaderImp*)self;
	TQ3Uns32 sizeRead = 0;
	TQ3FFormatBaseData *instanceData = (TQ3FFormatBaseData *) reader->mBaseData;

	Q3Storage_GetData(instanceData->storage,
							instanceData->currentStoragePosition,
							size, (TQ3Uns8*)buffer, &sizeRead);

//	Q3_ASSERT(sizeRead == size);
	instanceData->currentStoragePosition += sizeRead;

	return sizeRead;							 
}


static int
Q3DSReader_fileio_write_func(void *self, const Lib3dsByte *buffer, int size)
{
	X3DSReaderImp* reader = (X3DSReaderImp*)self;
	TQ3Uns32 sizeWrite = 0;
	TQ3FFormatBaseData *instanceData = (TQ3FFormatBaseData *) reader->mBaseData;

	Q3Storage_SetData(instanceData->storage,
							instanceData->currentStoragePosition,
							size, (TQ3Uns8*)buffer, &sizeWrite);

	//Q3_ASSERT(sizeWrite == length);
	instanceData->currentStoragePosition += sizeWrite;

	return sizeWrite;							 
}

#pragma mark X3DSReader Implementation;


X3DSReaderImp::X3DSReaderImp (C3DSReader* inSelf,
							TQ3FFormatBaseData* inData )
	: mSelf( inSelf )
	, mBaseData( inData )
{
	mBaseData->noMoreObjects = kQ3False;
	mBaseData->fileVersion = 0;	// should be reset in ReadHeader
	
	m3DSfileRef = NULL;
	
	
}

static bool StringStartsWithx( const char* inMain, const char* inPrefix )
{
	using namespace std;
	int	preLen = strlen( inPrefix );
	return 0 == memcmp( inMain, inPrefix, preLen );
}



/*!
	@function			ReadHeader
	@abstract			Begin reading 3DS data.
	@discussion			Although this implements the Quesa "read header"
						method, it actually reads the entire file and
						performs the first phase of parsing it.
	@result				True if all data was parsed successfully.
*/
bool	X3DSReaderImp::ReadHeader()
{
	bool	didRead = false;
	
	// If the 'Debg' property exists, start a debug stream.
	TQ3Status	propStat = Q3Object_GetProperty( mBaseData->storage,
		kDebugTextProperty, 0, NULL, NULL );
	if (propStat == kQ3Success)
	{
		mDebugStream.reset( new std::ostringstream );
		*mDebugStream << std::endl << "3DS ReadHeader starting." <<
			std::endl << "===========" << std::endl;
	}
	
	m3DSfileRef = lib3ds_file_new();
	if (!m3DSfileRef) {
		return didRead;
	}
	
	Lib3dsIo *io = lib3ds_io_new(	// prepare the file handling function pointers used by the 3dslib 
								this, 
								Q3DSReader_fileio_error_func,
								Q3DSReader_fileio_seek_func,
								Q3DSReader_fileio_tell_func,
								Q3DSReader_fileio_read_func,
								Q3DSReader_fileio_write_func
							  );

	if (!io) {
		lib3ds_file_free(m3DSfileRef);
		m3DSfileRef = NULL;
		return(didRead);
		}

	if (!lib3ds_file_read(m3DSfileRef, io)) { // read the data into a Lib3dsFile structure
		lib3ds_file_free(m3DSfileRef);
		m3DSfileRef = NULL;
		}
	else{
		didRead = true;
		}
		
	lib3ds_io_free(io);
	
	CreateQuesaObjects(); // convert the Lib3dsFile structure to a Quesa Group
		
	return didRead;
}

/*!
	@function			ReadObject
	@abstract			Fetch the next object.
	@result				A new Quesa object reference, or NULL if there
						are no more objects.
*/
TQ3Object		X3DSReaderImp::ReadObject()
{
	TQ3Object	theObject = NULL;
	
	mBaseData->noMoreObjects = kQ3True; // we return all the objects at once
	
	return mModel;
}



/*!
	@function			Close
	@abstract			The file is being closed.  If there is a debug
						stream, it is finished and returned to the client
						now.
*/
void	X3DSReaderImp::Close( TQ3Boolean inAbort )
{
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
}


void		X3DSReaderImp::CacheExternalTexture( const char* inURL,
												CQ3ObjectRef& inTexture )
{
	mTextureCache.insert( NameToTexture::value_type( inURL, inTexture ) );
}

bool	X3DSReaderImp::GetCachedExternalTexture( const char* inURL,
												CQ3ObjectRef& outTexture ) const
{
	bool	didFind = false;
	outTexture = CQ3ObjectRef();
	
	NameToTexture::const_iterator	found = mTextureCache.find( inURL );
	
	if (found != mTextureCache.end())
	{
		outTexture = found->second;
		didFind = true;
	}
	
	return didFind;
}


/*!
	@function			CreateQuesaObjects
	@abstract			converts the raw data read from the .3DS file to Quesa Objects inside a Display group
*/
void		X3DSReaderImp::CreateQuesaObjects()
{
	mModel = Q3DisplayGroup_New ();
	
	if (! m3DSfileRef) return;
	
	Lib3dsMesh *p;

	for (p=m3DSfileRef->meshes; p!=0; p=p->next) {
		
		CQ3ObjectRef obj(CreateQuesaMesh(p));
		
		if(obj.isvalid()){
			Q3Group_AddObject(mModel,obj.get());
			}
	}

}




/*!
	@function			Lib3dsToQuesaPoint
	@abstract			converts the raw data read from the .3DS file to Quesa Objects inside a Display group
*/
void		X3DSReaderImp::Lib3dsToQuesaPoint(Lib3dsVector*src, TQ3Point3D *dest, int numPoints)
{
	for(int i=0; i<numPoints; i++){
		dest->x = (*src)[0];
		dest->y = (*src)[2];
		dest->z = -(*src)[1];
		src++;
		dest++;
		}
}




/*!
	@function			Lib3dsToQuesaFaces
	@abstract			converts the raw data read from the .3DS file to Quesa Objects inside a Display group
*/
void		X3DSReaderImp::Lib3dsToQuesaFaces(Lib3dsMesh *meshData, TQ3TriMeshTriangleData* facesQuesa)
{
	Lib3dsFace* face = meshData->faceL;
	Lib3dsDword maxPointIndex = meshData->points - 1;
	volatile int mask = 0x0F0;
	
	for(int i=0; i<meshData->faces; i++){
		facesQuesa->pointIndices[0] = std::min<int>(face->points[0],maxPointIndex);
		if(face->flags & mask){
			facesQuesa->pointIndices[1] = std::min<int>(face->points[1],maxPointIndex);
			facesQuesa->pointIndices[2] = std::min<int>(face->points[2],maxPointIndex);
			}
		else{
			facesQuesa->pointIndices[1] = std::min<int>(face->points[2],maxPointIndex);
			facesQuesa->pointIndices[2] = std::min<int>(face->points[1],maxPointIndex);
			}
		face++;
		facesQuesa++;
	}
}



/*!
	@function			CreateQuesaMesh
	@abstract			converts a mesh structure created by the lib3ds to a trimesh Quesa object
*/
TQ3Object		X3DSReaderImp::CreateQuesaMesh(Lib3dsMesh *meshData)
{
	TQ3TriMeshData triMeshData;
	TQ3GeometryObject triMesh = NULL;
	
	// Set up the data
	// Calculate the Vertex Normals for smoothing
	int numNormals = meshData->faces * 3;
	Lib3dsVector *normalL = (Lib3dsVector*) malloc(sizeof(Lib3dsVector) * numNormals);
	if(!normalL)
		return NULL;
	
	TQ3Point3D *normalQuesa = (TQ3Point3D*) malloc(sizeof(TQ3Point3D) * numNormals);
	if(!normalQuesa){
		free(normalL);
		return NULL;
		}

	lib3ds_mesh_calculate_normals(meshData, normalL);
	Lib3dsToQuesaPoint(normalL,normalQuesa,numNormals);
	free(normalL);
	
	TQ3TriMeshTriangleData * facesQuesa = NULL;
	
	// Convert the coordinate system from lib3DS to Quesa
	TQ3Point3D *pointsQuesa = (TQ3Point3D*) malloc(sizeof(TQ3Point3D) * meshData->points);
	if(! pointsQuesa)
		goto freeNormalQuesa;
		
	Lib3dsToQuesaPoint((Lib3dsVector*)meshData->pointL,pointsQuesa,meshData->points);

	// Build the Faces 
	facesQuesa = (TQ3TriMeshTriangleData*) malloc(sizeof(TQ3Point3D) * meshData->faces);
	if(! facesQuesa)
		goto freePointsQuesa;
	Lib3dsToQuesaFaces(meshData,facesQuesa);


	// Build the Trimesh
	triMeshData.triMeshAttributeSet       = NULL;
	triMeshData.numTriangles              = meshData->faces;
	triMeshData.triangles                 = facesQuesa;
	triMeshData.numPoints                 = meshData->points;
	triMeshData.points                    = pointsQuesa;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = 0;
	triMeshData.edges                     = NULL;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 0;
	triMeshData.vertexAttributeTypes      = NULL;

	Q3BoundingBox_SetFromPoints3D (&triMeshData.bBox, triMeshData.points, triMeshData.numPoints, sizeof(TQ3Point3D));

	triMesh = Q3TriMesh_New (&triMeshData);
	
	free(facesQuesa);
freePointsQuesa:
	free(pointsQuesa);
freeNormalQuesa:
	free(normalQuesa);
	
	return triMesh;

}



#pragma mark -

C3DSReader::C3DSReader( TQ3FFormatBaseData* inData )
	: mImp( new X3DSReaderImp( this, inData ) )
{
}

C3DSReader::~C3DSReader()
{
}

/*!
	@function			FromFileFormat
	@abstract			Retrieve a C3DSReader pointer given a file format
						object.	(Static method)
*/
C3DSReader*	C3DSReader::FromFileFormat( TQ3FileFormatObject inFormat )
{
	TQ3XObjectClass theFormatClass = Q3XObject_GetClass( inFormat );
	TQ3FFormatBaseData*	baseData = static_cast<TQ3FFormatBaseData*>(
		Q3XObjectClass_GetPrivate( theFormatClass, inFormat ) );
	C3DSReader*	reader = reinterpret_cast<C3DSReader*>(
		baseData->reserved1 );
	return reader;
}

/*!
	@function			FromFile
	@abstract			Retrieve a C3DSReader pointer given a file
						object.	(Static method)
*/
C3DSReader*	C3DSReader::FromFile( TQ3FileObject inFile )
{
	TQ3FileFormatObject format = Q3File_GetFileFormat( inFile );
	C3DSReader*	reader = C3DSReader::FromFileFormat( format );
	return reader;
}


/*!
	@function			ReadHeader
	@abstract			Begin reading 3DS data.
	@discussion			Although this implements the Quesa "read header"
						method, it actually reads the entire file and
						performs the first phase of parsing it.
	@result				True if all data was parsed successfully.
*/
bool	C3DSReader::ReadHeader()
{
	bool	didRead = mImp->ReadHeader();

	return didRead;
}

/*!
	@function			ReadObject
	@abstract			Fetch the next object.
	@result				A new Quesa object reference, or NULL if there
						are no more objects.
*/
TQ3Object		C3DSReader::ReadObject()
{
	TQ3Object	theObject = mImp->ReadObject();
	
	return theObject;
}

/*!
	@function			Close
	@abstract			The file is being closed.  If there is a debug
						stream, it is finished and returned to the client
						now.
*/
void	C3DSReader::Close( TQ3Boolean inAbort )
{
	mImp->Close( inAbort );
}

/*!
	@function			GetDebugStream
	@abstract			Accessor for a debug text stream to be used by
						node handlers.
	@result				An output stream pointer, or NULL.
*/
std::ostream*	C3DSReader::GetDebugStream()
{
	return mImp->mDebugStream.get();
}


/*!
	@function			CacheExternalTexture
	@abstract			Cache an external texture so that it need not be loaded again.
	@discussion			When the client is unable to provide the texture, we want to
						put NULL in the cache so that we do not keep pestering it.
	@param				inURL		The URL or file name given in the ImageTexure or
									Texture2 node.
	@param				inTexture	The Quesa texture object to cache, or NULL.
*/
void	C3DSReader::CacheExternalTexture( const char* inURL, CQ3ObjectRef& inTexture )
{
	mImp->CacheExternalTexture( inURL, inTexture );
}

/*!
	@function			GetCachedExternalTexture
	@abstract			Retrieve a Quesa texture object from the cache.
	@param				inURL		The URL or file name given in the ImageTexure or
									Texture2 node.
	@param				outTexture	A texture object or NULL.
	@result				True if we found the URL in the cache, regardless of whether
						the associated texture was present or NULL.
*/
bool	C3DSReader::GetCachedExternalTexture( const char* inURL,
												CQ3ObjectRef& outTexture ) const
{
	return mImp->GetCachedExternalTexture( inURL, outTexture );
}

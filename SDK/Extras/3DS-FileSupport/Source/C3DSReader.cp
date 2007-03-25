/*  NAME:
        C3DSReader.cp

    DESCRIPTION:
       Main class implementing the reader.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
#include "C3DSReader.h"

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
	Lib3dsWord maxPointIndex = meshData->points - 1;
	volatile int mask = 0x0F0;
	
	for(int i=0; i<meshData->faces; i++){
		facesQuesa->pointIndices[0] = std::min(face->points[0],maxPointIndex);
		if(face->flags & mask){
			facesQuesa->pointIndices[1] = std::min(face->points[1],maxPointIndex);
			facesQuesa->pointIndices[2] = std::min(face->points[2],maxPointIndex);
			}
		else{
			facesQuesa->pointIndices[1] = std::min(face->points[2],maxPointIndex);
			facesQuesa->pointIndices[2] = std::min(face->points[1],maxPointIndex);
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
	
	// Convert the coordinate system from lib3DS to Quesa
	TQ3Point3D *pointsQuesa = (TQ3Point3D*) malloc(sizeof(TQ3Point3D) * meshData->points);
	if(! pointsQuesa)
		goto freeNormalQuesa;
		
	Lib3dsToQuesaPoint((Lib3dsVector*)meshData->pointL,pointsQuesa,meshData->points);

	// Build the Faces 
	TQ3TriMeshTriangleData * facesQuesa = (TQ3TriMeshTriangleData*) malloc(sizeof(TQ3Point3D) * meshData->faces);
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
	@function			GetStorage
	@abstract			Accessor for storage object from which VRML data
						is being read.  Do not dispose it.
	@result				Quesa storage object.
*/
TQ3StorageObject	C3DSReader::GetStorage()
{
	return mImp->mBaseData->storage;
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

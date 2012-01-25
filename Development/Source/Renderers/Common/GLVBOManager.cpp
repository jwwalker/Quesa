/*  NAME:
        GLVBOManager.c

    DESCRIPTION:
        Quesa OpenGL vertex buffer object caching.
       
    COPYRIGHT:
        Copyright (c) 2007-2012, Quesa Developers. All rights reserved.

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

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "GLVBOManager.h"
#include "GLGPUSharing.h"
#include "CQ3ObjectRef.h"
#include "GLUtils.h"
#include "E3Main.h"

#include <vector>
#include <algorithm>
#include <cstddef>


//=============================================================================
//		Internal constants
//-----------------------------------------------------------------------------
namespace
{
	const TQ3Uns32	kVBOCacheKey	= Q3_FOUR_CHARACTER_CONSTANT('v', 'b', 'o', 'k');
	
	const TQ3Uns32	kAbsentBuffer	= 0xFFFFFFFFU;
}

#ifndef GL_ARRAY_BUFFER
	#define GL_ARRAY_BUFFER					0x8892
	#define GL_ELEMENT_ARRAY_BUFFER			0x8893
	#define GL_STATIC_DRAW                  0x88E4
#endif


//=============================================================================
//		Internal types
//-----------------------------------------------------------------------------

#ifndef APIENTRY
	#if QUESA_OS_WIN32
		#define APIENTRY	__stdcall
	#else
		#define APIENTRY
	#endif
#endif

#ifndef GL_ARB_vertex_buffer_object
    typedef std::ptrdiff_t GLintptrARB;
    typedef std::ptrdiff_t GLsizeiptrARB;
#endif

typedef void (APIENTRY * BindBufferARBProcPtr) (GLenum target,
												GLuint buffer);
typedef void (APIENTRY * DeleteBuffersARBProcPtr) (GLsizei n,
												const GLuint *buffers);
typedef void (APIENTRY * GenBuffersARBProcPtr) (GLsizei n,
												GLuint *buffers);
typedef GLboolean (APIENTRY * IsBufferARBProcPtr) (GLuint buffer);
typedef void (APIENTRY * BufferDataARBProcPtr) (GLenum target,
												GLsizeiptrARB size,
												const GLvoid *data,
												GLenum usage);
typedef void (APIENTRY * BufferSubDataARBProcPtr) (GLenum target,
												GLintptrARB offset,
												GLsizeiptrARB size,
												const GLvoid *data);

namespace
{
#pragma mark struct CachedVBO
	struct CachedVBO
	{
						CachedVBO();
						CachedVBO( TQ3GeometryObject inGeom, GLenum inMode );
						~CachedVBO() {}
		
						
		bool			IsStale() const;
	
		CQ3ObjectRef	mGeomObject;
		TQ3Uns32		mEditIndex;
		GLenum			mGLMode;			// e.g., GL_TRIANGLES
		TQ3Uns32		mNumIndices;
		GLuint			mGLBufferNames[2];	// 0 is array, 1 is index
		TQ3Uns32		mVertexBufferOffset;
		TQ3Uns32		mNormalBufferOffset;
		TQ3Uns32		mColorBufferOffset;
		TQ3Uns32		mTextureUVBufferOffset;
		TQ3Uns32		mModeCount;
		TQ3Uns32		mBufferBytes;
		CachedVBO*		mPrev;
		CachedVBO*		mNext;
	
	private:
		// Unimplemented; declared just to make sure that they don't get used
		// accidentally
						CachedVBO( const CachedVBO& inOther );
		CachedVBO&		operator=( const CachedVBO& inOther );
	};
	
	typedef	std::vector< CachedVBO* >	CachedVBOVec;

	class VBOCache : public CQ3GPSharedCache
	{
	public:
						VBOCache();
						~VBOCache();
		
		void			InitProcPtrs();
		CachedVBO*		FindVBO( TQ3GeometryObject inGeom, GLenum inMode );
		void			RenderVBO( const CachedVBO* inCachedVBO );
		void			AddVBO( CachedVBO* inVBO );
		void			UpdateModeCount( TQ3GeometryObject inGeom );
		void			FlushUnreferenced();
		void			DeleteVBO( CachedVBO* inCachedVBO );
		TQ3Uns32		CountVBOs( TQ3GeometryObject inGeom );
		void			PurgeDownToSize( long long inTargetSize );
		void			SetMaxBufferSize( long long inBufferSize );
		void			MakeRoom( TQ3Uns32 inBytesNeeded );
		
		void			AddToUsageList( CachedVBO* ioVBO );
		void			DeleteFromUsageList( CachedVBO* ioVBO );
		void			RenewInUsageList( CachedVBO* ioVBO );

		GenBuffersARBProcPtr		glGenBuffersARBProc;
		BindBufferARBProcPtr		glBindBufferARBProc;
		DeleteBuffersARBProcPtr		glDeleteBuffersARBProc;
		IsBufferARBProcPtr			glIsBufferARBProc;
		BufferDataARBProcPtr		glBufferDataARBProc;
		BufferSubDataARBProcPtr		glBufferSubDataARBProc;
		
	private:
		// The vector of VBO records will be maintained in order
		// of geometry reference, so that search can be faster.
		CachedVBOVec	mCachedVBOs;
		
		CachedVBO					mListOldEnd;
		CachedVBO					mListNewEnd;
		long long					mTotalBytes;
		long long					mMaxBufferBytes;
	};

	// Comparator for ordering cached VBOs by their mode and geometries
	struct VBOLess
	{
		bool	operator()( const CachedVBO* inOne, const CachedVBO* inTwo ) const
				{
					return (inOne->mGLMode < inTwo->mGLMode) ||
						(
							(inOne->mGLMode == inTwo->mGLMode) &&
							(inOne->mGeomObject.get() < inTwo->mGeomObject.get())
						);
				}
	};
	
	// Predicate for use with std::partition and CachedVBOVec.
	struct IsReferenced
	{
		bool			operator()( const CachedVBO* inCachedVBO ) const
								{
									return Q3Shared_GetReferenceCount(
										inCachedVBO->mGeomObject.get() ) >
										inCachedVBO->mModeCount;
								}
	};
}


//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------

static inline GLvoid* BufferObPtr( GLuint offset )
{
	return (GLvoid*)( ((char*)NULL) + offset );
}


static VBOCache* GetVBOCache( TQ3GLContext glContext )
{
	VBOCache*	theCache = static_cast<VBOCache*>( GLGPUSharing_GetCache(
		glContext, kVBOCacheKey ) );
	
	if (theCache == NULL)
	{
		theCache = new(std::nothrow) VBOCache;
		
		if (theCache != NULL)
		{
			GLGPUSharing_AddCache( glContext, kVBOCacheKey, theCache );
		}
	}
	
	return theCache;
}

#pragma mark -

CachedVBO::CachedVBO( TQ3GeometryObject inGeom, GLenum inMode )
	: mGeomObject( Q3Shared_GetReference( inGeom ) )
	, mEditIndex( Q3Shared_GetEditIndex( inGeom ) )
	, mGLMode( inMode )
	, mModeCount( 1 )
	, mBufferBytes( 0 )
	, mPrev( NULL )
	, mNext( NULL )
{
	// Leave other fields uninitialized for now
}

CachedVBO::CachedVBO()
	: mGeomObject()
	, mBufferBytes( 0 )
	, mPrev( NULL )
	, mNext( NULL )
{
}

bool	CachedVBO::IsStale() const
{
	return static_cast<E3Shared*>( mGeomObject.get() )->GetEditIndex() != mEditIndex;
}

#pragma mark -

VBOCache::VBOCache()
	: glGenBuffersARBProc( NULL )
	, glBindBufferARBProc( NULL )
	, glDeleteBuffersARBProc( NULL )
	, glIsBufferARBProc( NULL )
	, glBufferDataARBProc( NULL )
	, glBufferSubDataARBProc( NULL )
	, mTotalBytes( 0 )
	, mMaxBufferBytes( 0 )
{
	InitProcPtrs();

	mListOldEnd.mNext = &mListNewEnd;
	mListNewEnd.mPrev = &mListOldEnd;
}

VBOCache::~VBOCache()
{
	for (CachedVBOVec::iterator i = mCachedVBOs.begin(); i != mCachedVBOs.end(); ++i)
	{
		delete *i;
	}
}

void		VBOCache::InitProcPtrs()
{
	if (glGenBuffersARBProc == NULL)
	{
		GLGetProcAddress( glGenBuffersARBProc, "glGenBuffers", "glGenBuffersARB" );
		GLGetProcAddress( glBindBufferARBProc, "glBindBuffer", "glBindBufferARB" );
		GLGetProcAddress( glDeleteBuffersARBProc, "glDeleteBuffers", "glDeleteBuffersARB" );
		GLGetProcAddress( glIsBufferARBProc, "glIsBuffer", "glIsBufferARB" );
		GLGetProcAddress( glBufferDataARBProc, "glBufferData", "glBufferDataARB" );
		GLGetProcAddress( glBufferSubDataARBProc, "glBufferSubData", "glBufferSubDataARB" );

		Q3_ASSERT( (glGenBuffersARBProc != NULL) &&
			(glBindBufferARBProc != NULL) &&
			(glDeleteBuffersARBProc != NULL) &&
			(glIsBufferARBProc != NULL) &&
			(glBufferDataARBProc != NULL) &&
			(glBufferSubDataARBProc != NULL) );
	}
}


CachedVBO*		VBOCache::FindVBO( TQ3GeometryObject inGeom, GLenum inMode )
{
	CachedVBO*	theCachedVBO = NULL;
	
	CachedVBO	searchDummy( inGeom, inMode );
	
	CachedVBOVec::iterator	foundIt = std::lower_bound( mCachedVBOs.begin(),
		mCachedVBOs.end(), &searchDummy, VBOLess() );
	
	if ( (foundIt != mCachedVBOs.end()) &&
		((*foundIt)->mGeomObject.get() == inGeom) &&
		((*foundIt)->mGLMode == inMode) )
	{
		theCachedVBO = *foundIt;
		
		if ( (theCachedVBO != NULL) && theCachedVBO->IsStale() )
		{
			mCachedVBOs.erase( foundIt );
			
			DeleteVBO( theCachedVBO );
			
			theCachedVBO = NULL;
		}
	}
	
	return theCachedVBO;
}

TQ3Uns32		VBOCache::CountVBOs( TQ3GeometryObject inGeom )
{
	TQ3Uns32		refCount = 0;
	
	CachedVBO*	aVBO = FindVBO( inGeom, GL_TRIANGLE_STRIP );
	if (aVBO == NULL)
	{
		aVBO = FindVBO( inGeom, GL_TRIANGLES );
	}
	if (aVBO == NULL)
	{
		aVBO = FindVBO( inGeom, GL_LINES );
	}
	
	if (aVBO != NULL)
	{
		refCount = aVBO->mModeCount;
	}
	
	return refCount;
}

void	VBOCache::AddToUsageList( CachedVBO* ioVBO )
{
	// Insert the new record at the "new" end of a doubly-linked list.
	ioVBO->mNext = &mListNewEnd;
	ioVBO->mPrev = mListNewEnd.mPrev;
	ioVBO->mPrev->mNext = ioVBO;
	mListNewEnd.mPrev = ioVBO;
}

void	VBOCache::RenewInUsageList( CachedVBO* ioVBO )
{
	// Move the item to the "new" end of the list, so that it will not be
	// purged soon.
	DeleteFromUsageList( ioVBO );
	AddToUsageList( ioVBO );
}

void	VBOCache::AddVBO( CachedVBO* inVBO )
{
	CachedVBOVec::iterator	placeIt = std::lower_bound( mCachedVBOs.begin(),
		mCachedVBOs.end(), inVBO, VBOLess() );

	mCachedVBOs.insert( placeIt, inVBO );
	
	UpdateModeCount( inVBO->mGeomObject.get() );

	AddToUsageList( inVBO );

	mTotalBytes += inVBO->mBufferBytes;
	
	//Q3_MESSAGE_FMT("VBO cache size: %lld", mTotalBytes );
}

/*
	UpdateModeCount
	
	A particular geometry may be referenced more than once by the VBO cache.
	We need to keep track of the number of such references, so that we will be
	able to quickly tell whether the geometry is still referenced by something
	other than the VBO cache.
*/
void	VBOCache::UpdateModeCount( TQ3GeometryObject inGeom )
{
	CachedVBO*	stripVBO = FindVBO( inGeom, GL_TRIANGLE_STRIP );
	CachedVBO*	triVBO = FindVBO( inGeom, GL_TRIANGLES );
	CachedVBO*	lineVBO = FindVBO( inGeom, GL_LINES );
	
	TQ3Uns32	modeCount = (stripVBO? 1 : 0) + (triVBO? 1 : 0) + (lineVBO? 1 : 0);
	
	if (modeCount > 0)
	{
		if (stripVBO)
		{
			stripVBO->mModeCount = modeCount;
		}
		if (triVBO)
		{
			triVBO->mModeCount = modeCount;
		}
		if (lineVBO)
		{
			lineVBO->mModeCount = modeCount;
		}
	}
}

void VBOCache::RenderVBO( const CachedVBO* inCachedVBO )
{
	(*glBindBufferARBProc)( GL_ARRAY_BUFFER, inCachedVBO->mGLBufferNames[0] );
	glVertexPointer( 3, GL_FLOAT, 0,
		BufferObPtr( inCachedVBO->mVertexBufferOffset ) );
	
	if (inCachedVBO->mNormalBufferOffset != kAbsentBuffer)
	{
		glNormalPointer( GL_FLOAT, 0,
			BufferObPtr( inCachedVBO->mNormalBufferOffset ) );
	}
	
	if (inCachedVBO->mTextureUVBufferOffset != kAbsentBuffer)
	{
		glTexCoordPointer( 2, GL_FLOAT, 0,
			BufferObPtr( inCachedVBO->mTextureUVBufferOffset ) );
	}
	
	if (inCachedVBO->mColorBufferOffset != kAbsentBuffer)
	{
		glColorPointer( 3, GL_FLOAT, 0,
			BufferObPtr( inCachedVBO->mColorBufferOffset ) );
	}
	
	(*glBindBufferARBProc)( GL_ELEMENT_ARRAY_BUFFER,
		inCachedVBO->mGLBufferNames[1] );
	glDrawElements( inCachedVBO->mGLMode, inCachedVBO->mNumIndices,
		GL_UNSIGNED_INT, BufferObPtr( 0 ) );
		
	(*glBindBufferARBProc)( GL_ARRAY_BUFFER, 0 );
	(*glBindBufferARBProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void	VBOCache::DeleteFromUsageList( CachedVBO* ioVBO )
{
	ioVBO->mPrev->mNext = ioVBO->mNext;
	ioVBO->mNext->mPrev = ioVBO->mPrev;
}


void	VBOCache::DeleteVBO( CachedVBO* inCachedVBO )
{
	(*glDeleteBuffersARBProc)( 2, inCachedVBO->mGLBufferNames );
	
	UpdateModeCount( inCachedVBO->mGeomObject.get() );

	DeleteFromUsageList( inCachedVBO );
	
	mTotalBytes -= inCachedVBO->mBufferBytes;
	
	delete inCachedVBO;
}

void	VBOCache::FlushUnreferenced()
{
	// Move unreferenced VBOs to end of list
	CachedVBOVec::iterator startUnused = std::stable_partition(
		mCachedVBOs.begin(), mCachedVBOs.end(), IsReferenced() );
	
	// Make a copy of the records that will go away
	CachedVBOVec	doomedVBOs( startUnused, mCachedVBOs.end() );
	
	// Remove them from the normal array
	mCachedVBOs.erase( startUnused, mCachedVBOs.end() );

	// Delete the buffers for the VBO records that are going away
	for (CachedVBOVec::iterator i = doomedVBOs.begin(); i != doomedVBOs.end(); ++i)
	{
		DeleteVBO( *i );
	}
}

void	VBOCache::PurgeDownToSize( long long inTargetSize )
{
	while (mTotalBytes > inTargetSize)
	{
		// Find the least recently used VBO from the doubly-linked list
		CachedVBO* oldestVBO = mListOldEnd.mNext;
		
		// Remove it from the sorted vector
		CachedVBOVec::iterator	foundIt = std::lower_bound( mCachedVBOs.begin(),
			mCachedVBOs.end(), oldestVBO, VBOLess() );
		if ( (foundIt != mCachedVBOs.end()) &&
			((*foundIt)->mGeomObject.get() == oldestVBO->mGeomObject.get()) )
		{
			mCachedVBOs.erase( foundIt );
		}
		
		// Remove the VBO, remove the record from the doubly-linked list, and
		// dispose the record.
		DeleteVBO( oldestVBO );
	}
}

void	VBOCache::SetMaxBufferSize( long long inBufferSize )
{
	if (inBufferSize < mMaxBufferBytes)
	{
		PurgeDownToSize( inBufferSize );
	}
	
	mMaxBufferBytes = inBufferSize;
}

void	VBOCache::MakeRoom( TQ3Uns32 inBytesNeeded )
{
	if ( (inBytesNeeded < mMaxBufferBytes) &&
		(mTotalBytes + inBytesNeeded > mMaxBufferBytes) )
	{
		long long targetSize = mMaxBufferBytes - inBytesNeeded;
		
		PurgeDownToSize( targetSize );
	}
}


#pragma mark -
//=============================================================================
//		Public functions
//-----------------------------------------------------------------------------

/*!
	@function		UpdateVBOCacheLimit
	@abstract		Update the limit on memory that can be used in this cache.
	@param			glContext		An OpenGL context.
	@param			inMaxMemK		New memory limit in K-bytes.
*/
void				UpdateVBOCacheLimit(
									TQ3GLContext glContext,
									TQ3Uns32 inMaxMemK )
{
	VBOCache*	theCache = GetVBOCache( glContext );
	
	if (theCache != NULL)
	{
		theCache->SetMaxBufferSize( inMaxMemK * 1024LL );
	}
}


/*!
	@function		RenderCachedVBO
	@abstract		Look for a cached VBO for the given geometry and OpenGL
					context.  If we find one, render it.
	@discussion		If we find the object in the cache, but the cached object
					is stale, we delete it from the cache and return false.
	@param			glContext		An OpenGL context.
	@param			inGeom			A geometry object.
	@param			inMode			OpenGL mode, e.g., GL_TRIANGLES.
	@result			True if the object was found and rendered.
*/
TQ3Boolean			RenderCachedVBO(
									TQ3GLContext glContext,
									TQ3GeometryObject inGeom,
									GLenum inMode )
{
	TQ3Boolean	didRender = kQ3False;
	VBOCache*	theCache = GetVBOCache( glContext );
	
	if (theCache != NULL)
	{
		CachedVBO*	theVBO = theCache->FindVBO( inGeom, inMode );
		
		if ( (theVBO == NULL) && (inMode == GL_TRIANGLE_STRIP) )
		{
			theVBO = theCache->FindVBO( inGeom, GL_TRIANGLES );
		}
		
		if (theVBO != NULL)
		{
			theCache->RenderVBO( theVBO );
			theCache->RenewInUsageList( theVBO );
			didRender = kQ3True;
		}
	}
	
	return didRender;
}

/*!
	@function		AddVBOToCache
	@abstract		Add VBO data to the cache.  Do not call this unless
					RenderCachedVBO has just returned false.
	@param			glContext		An OpenGL context.
	@param			inGeom			A geometry object.
	@param			inNumPoints		Number of points (vertices).
	@param			inPoints		Array of point locations.
	@param			inNormals		Array of normal vectors (or NULL).
	@param			inColors		Array of vertex colors (or NULL).
	@param			inUVs			Array of vertex UV coordinates (or NULL).
	@param			inMode			OpenGL mode, e.g., GL_TRIANGLES.
	@param			inNumIndices	Number of vertex indices to follow.
	@param			inIndices		Array of vertex indices.
*/
void				AddVBOToCache(
									TQ3GLContext glContext,
									TQ3GeometryObject inGeom,
									TQ3Uns32 inNumPoints,
									const TQ3Point3D* inPoints,
									const TQ3Vector3D* inNormals,
									const TQ3ColorRGB* inColors,
									const TQ3Param2D* inUVs,
									GLenum inMode,
									TQ3Uns32 inNumIndices,
									const TQ3Uns32* inIndices )
{
	VBOCache*	theCache = GetVBOCache( glContext );
	
	if (theCache != NULL)
	{
		CachedVBO*	newVBO = new CachedVBO( inGeom, inMode );
		newVBO->mNumIndices = inNumIndices;
		(*theCache->glGenBuffersARBProc)( 2, newVBO->mGLBufferNames );
		
		TQ3Uns32	vertexDataSize = inNumPoints * sizeof(TQ3Point3D);
		TQ3Uns32	normalDataSize = (inNormals == NULL)? 0 :
			inNumPoints * sizeof(TQ3Vector3D);
		TQ3Uns32	colorDataSize = (inColors == NULL)? 0 :
			inNumPoints * sizeof(TQ3ColorRGB);
		TQ3Uns32	uvDataSize = (inUVs == NULL)? 0 :
			inNumPoints * sizeof(TQ3Param2D);
		TQ3Uns32	totalDataSize = vertexDataSize + normalDataSize +
			colorDataSize + uvDataSize;
		
		newVBO->mBufferBytes = inNumIndices * sizeof(TQ3Uns32) + totalDataSize;
		theCache->MakeRoom( newVBO->mBufferBytes );
			
		newVBO->mVertexBufferOffset = 0;
		newVBO->mNormalBufferOffset = (inNormals == NULL)? kAbsentBuffer :
			vertexDataSize;
		newVBO->mColorBufferOffset = (inColors == NULL)? kAbsentBuffer :
			vertexDataSize + normalDataSize;
		newVBO->mTextureUVBufferOffset = (inUVs == NULL)? kAbsentBuffer :
			vertexDataSize + normalDataSize + colorDataSize;
		
		theCache->AddVBO( newVBO );
		
		const GLvoid *	dataAddr;
		
		// First define an uninitialized buffer of the right size
		(*theCache->glBindBufferARBProc)( GL_ARRAY_BUFFER,
			newVBO->mGLBufferNames[0] );
		(*theCache->glBufferDataARBProc)( GL_ARRAY_BUFFER, totalDataSize,
			NULL, GL_STATIC_DRAW );
		
		// Then set sub-buffer data
		dataAddr = inPoints;
		(*theCache->glBufferSubDataARBProc)( GL_ARRAY_BUFFER, 0,
			vertexDataSize, dataAddr );
		if (inNormals != NULL)
		{
			dataAddr = inNormals;
			(*theCache->glBufferSubDataARBProc)( GL_ARRAY_BUFFER,
				newVBO->mNormalBufferOffset, normalDataSize, dataAddr );
		}
		if (inColors != NULL)
		{
			dataAddr = inColors;
			(*theCache->glBufferSubDataARBProc)( GL_ARRAY_BUFFER,
				newVBO->mColorBufferOffset, colorDataSize, dataAddr );
		}
		if (inUVs != NULL)
		{
			dataAddr = inUVs;
			(*theCache->glBufferSubDataARBProc)( GL_ARRAY_BUFFER,
				newVBO->mTextureUVBufferOffset, uvDataSize, dataAddr );
		}
		(*theCache->glBindBufferARBProc)( GL_ARRAY_BUFFER, 0 );
		
		// Now for the index data.
		(*theCache->glBindBufferARBProc)( GL_ELEMENT_ARRAY_BUFFER,
			newVBO->mGLBufferNames[1] );
		dataAddr = inIndices;
		(*theCache->glBufferDataARBProc)( GL_ELEMENT_ARRAY_BUFFER,
			inNumIndices * sizeof(TQ3Uns32), dataAddr, GL_STATIC_DRAW );
		(*theCache->glBindBufferARBProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
}


/*!
	@function		FlushVBOCache
	@abstract		Delete any cached VBOs for geometries that are no longer
					referenced elsewhere.
	@param			glContext		An OpenGL context.
*/
void				FlushVBOCache(
									TQ3GLContext glContext )
{
	VBOCache*	theCache = GetVBOCache( glContext );
	
	if (theCache != NULL)
	{
		theCache->FlushUnreferenced();
	}
}


/*!
	@function		CountVBOs
	@abstract		Count how many references the VBO manager holds for a given
					geometry, counting all GL contexts and all modes.
	@param			inGeom			A geometry object.
	@result			Number of VBOs referencing the geometry.
*/
TQ3Uns32			CountVBOs( TQ3GeometryObject inGeom )
{
	TQ3Uns32 refCount = 0;
	TQ3GLContext sharingBase = NULL;
	
	while ( (sharingBase = GLGPUSharing_GetNextSharingBase( sharingBase )) != NULL )
	{
		VBOCache*	theCache = GetVBOCache( sharingBase );
		
		if (theCache != NULL)
		{
			refCount += theCache->CountVBOs( inGeom );
		}
	}
	
	return refCount;
}

/*  NAME:
        GLVBOManager.c

    DESCRIPTION:
        Quesa OpenGL vertex buffer object caching.
       
    COPYRIGHT:
        Copyright (c) 2007-2014, Quesa Developers. All rights reserved.

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
#include "GLShadowVolumeManager.h"
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
		
		CachedVBO*		FindVBO( TQ3GeometryObject inGeom, GLenum inMode, const GLBufferFuncs& inFuncs );
		void			RenderVBO( const GLBufferFuncs& inFuncs, const CachedVBO* inCachedVBO );
		void			AddVBO( CachedVBO* inVBO );
		void			FlushUnreferenced( const GLBufferFuncs& inFuncs );
		void			DeleteVBO( CachedVBO* inCachedVBO, const GLBufferFuncs& inFuncs );
		void			DeleteCachedVBOs( CachedVBOVec& inVBOs, const GLBufferFuncs& inFuncs );
		TQ3Uns32		CountVBOs( TQ3GeometryObject inGeom );
		void			PurgeDownToSize( long long inTargetSize, const GLBufferFuncs& inFuncs );
		void			SetMaxBufferSize( long long inBufferSize, const GLBufferFuncs& inFuncs );
		void			MakeRoom( TQ3Uns32 inBytesNeeded, const GLBufferFuncs& inFuncs );
		
		void			AddToUsageList( CachedVBO* ioVBO );
		void			DeleteFromUsageList( CachedVBO* ioVBO );
		void			RenewInUsageList( CachedVBO* ioVBO );

	private:
		CachedVBOVec*	GetVBOVecForMode( GLenum inMode );
		CachedVBO*		FindVBOInVec( TQ3GeometryObject inGeom, CachedVBOVec& inVBOs );
		void			FlushUnreferencedInVec( CachedVBOVec& ioVBOs, const GLBufferFuncs& inFuncs );

		// VBO records will be stored in 3 vectors, one for each of
		// the modes GL_TRIANGLE_STRIP, GL_TRIANGLES, GL_LINES.
		// Each vector will be maintained in order of the
		// geometry reference, so we can use binary search.
		CachedVBOVec				mCachedVBOs_strips;
		CachedVBOVec				mCachedVBOs_triangles;
		CachedVBOVec				mCachedVBOs_lines;
		
		CachedVBO					mListOldEnd;
		CachedVBO					mListNewEnd;
		long long					mTotalBytes;
		long long					mMaxBufferBytes;
	};

	// Comparator for ordering cached VBOs by geometries
	struct VBOLess
	{
		bool	operator()( const CachedVBO* inOne, const CachedVBO* inTwo ) const
				{
					return (inOne->mGeomObject.get() < inTwo->mGeomObject.get());
				}
	};
	
	// Predicate for use with std::partition and CachedVBOVec.
	struct IsReferenced
	{
		bool			operator()( const CachedVBO* inCachedVBO ) const
								{
									TQ3GeometryObject geom =
										inCachedVBO->mGeomObject.get();
									TQ3Uns32 allRefs = Q3Shared_GetReferenceCount( geom );
									TQ3Uns32 vboRefs = CountVBOs( geom );
									TQ3Uns32 shadowRefs = ShadowVolMgr::CountVBOs( geom );
									return allRefs > vboRefs + shadowRefs;
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


/*!
	@function	FindVBOByGeom
	@abstract	Look for a VBO record in a sorted vector, matching a geometry.
	@discussion	One can do this using std::lower_bound, but I wanted to avoid
				creating and destroying a temporary CachedVBO and some other
				STL overhead.
	@param		inVBOs		A vector of CachedVBO*, sorted by geometry.
	@param		inGeom		A geometry to look for.
	@result		Iterator pointing to the matching record, or inVBOs.end() if
				there is no matching record.
*/
static CachedVBOVec::iterator FindVBOByGeom( CachedVBOVec& inVBOs, TQ3GeometryObject inGeom )
{
	CachedVBOVec::iterator foundIt = inVBOs.end();
	
	const CachedVBOVec::size_type kRecordCount = inVBOs.size();
	if (kRecordCount > 0)
	{
		CachedVBO** vboArray = &inVBOs[0];
		CachedVBOVec::size_type lowIndex = 0;
		CachedVBOVec::size_type highIndex = lowIndex + kRecordCount - 1;
		while (lowIndex < highIndex)
		{
			CachedVBOVec::size_type mid = (lowIndex + highIndex) / 2;
			
			if (vboArray[ mid ]->mGeomObject.get() < inGeom)
			{
				lowIndex = mid + 1;
			}
			else
			{
				highIndex = mid;
			}
		}
		
		if ( (lowIndex == highIndex) && (vboArray[ lowIndex ]->mGeomObject.get() == inGeom) )
		{
			foundIt = inVBOs.begin() + lowIndex;
		}
	}
	
	return foundIt;
}

GLBufferFuncs::GLBufferFuncs()
	: glGenBuffersProc( NULL )
	, glBindBufferProc( NULL )
	, glDeleteBuffersProc( NULL )
	, glIsBufferProc( NULL )
	, glBufferDataProc( NULL )
	, glBufferSubDataProc( NULL )
	, glClientActiveTextureProc( NULL )
	, glMultiTexCoord1fProc( NULL )
	, glGetBufferParameterivProc( NULL )
{
}

void	GLBufferFuncs::Initialize( const TQ3GLExtensions& inExts )
{
	if (inExts.vertexBufferObjects == kQ3True)
	{
		GLGetProcAddress( glGenBuffersProc, "glGenBuffers", "glGenBuffersARB" );
		GLGetProcAddress( glBindBufferProc, "glBindBuffer", "glBindBufferARB" );
		GLGetProcAddress( glDeleteBuffersProc, "glDeleteBuffers", "glDeleteBuffersARB" );
		GLGetProcAddress( glIsBufferProc, "glIsBuffer", "glIsBufferARB" );
		GLGetProcAddress( glBufferDataProc, "glBufferData", "glBufferDataARB" );
		GLGetProcAddress( glBufferSubDataProc, "glBufferSubData", "glBufferSubDataARB" );
		GLGetProcAddress( glClientActiveTextureProc, "glClientActiveTexture", "glClientActiveTextureARB" );
		GLGetProcAddress( glMultiTexCoord1fProc, "glMultiTexCoord1f", "glMultiTexCoord1fARB" );
		GLGetProcAddress( glGetBufferParameterivProc, "glGetBufferParameteriv",
			"glGetBufferParameterivARB" );

		Q3_ASSERT( (glGenBuffersProc != NULL) &&
			(glBindBufferProc != NULL) &&
			(glDeleteBuffersProc != NULL) &&
			(glIsBufferProc != NULL) &&
			(glBufferDataProc != NULL) &&
			(glBufferSubDataProc != NULL) &&
			(glClientActiveTextureProc != NULL) &&
			(glMultiTexCoord1fProc != NULL) &&
			(glGetBufferParameterivProc != NULL) );
	}
}

void	GLBufferFuncs::InitializeForDelete()
{
	GLGetProcAddress( glDeleteBuffersProc, "glDeleteBuffers", "glDeleteBuffersARB" );
	GLGetProcAddress( glIsBufferProc, "glIsBuffer", "glIsBufferARB" );
}

#pragma mark -

CachedVBO::CachedVBO( TQ3GeometryObject inGeom, GLenum inMode )
	: mGeomObject( Q3Shared_GetReference( inGeom ) )
	, mEditIndex( Q3Shared_GetEditIndex( inGeom ) )
	, mGLMode( inMode )
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
	: mTotalBytes( 0 )
	, mMaxBufferBytes( 0 )
{
	mListOldEnd.mNext = &mListNewEnd;
	mListNewEnd.mPrev = &mListOldEnd;
}

VBOCache::~VBOCache()
{
	GLBufferFuncs funcs;
	funcs.InitializeForDelete();
	DeleteCachedVBOs( mCachedVBOs_strips, funcs );
	DeleteCachedVBOs( mCachedVBOs_triangles, funcs );
	DeleteCachedVBOs( mCachedVBOs_lines, funcs );
}


CachedVBO*		VBOCache::FindVBOInVec( TQ3GeometryObject inGeom, CachedVBOVec& inVBOs )
{
	CachedVBO*	theCachedVBO = NULL;
	
	CachedVBOVec::iterator foundIt = FindVBOByGeom( inVBOs, inGeom );
	
	if ( foundIt != inVBOs.end() )
	{
		theCachedVBO = *foundIt;
		
		// Note: We cannot delete stale VBOs here, because this function may
		// be called by CountVBOs at a time when the OpenGL context that is
		// in effect does not go with this cache.
	}
	
	return theCachedVBO;
}

CachedVBOVec*	VBOCache::GetVBOVecForMode( GLenum inMode )
{
	CachedVBOVec* whichVec = NULL;
	
	switch (inMode)
	{
		case GL_TRIANGLE_STRIP:
			whichVec = &mCachedVBOs_strips;
			break;
			
		case GL_TRIANGLES:
			whichVec = &mCachedVBOs_triangles;
			break;
			
		case GL_LINES:
			whichVec = &mCachedVBOs_lines;
			break;
	}
	
	return whichVec;
}

CachedVBO*		VBOCache::FindVBO( TQ3GeometryObject inGeom, GLenum inMode,
									const GLBufferFuncs& inFuncs )
{
	CachedVBO*	theCachedVBO = NULL;
	
	CachedVBOVec* whichVec = GetVBOVecForMode( inMode );

	if (whichVec != NULL)
	{
		theCachedVBO = FindVBOInVec( inGeom, *whichVec );
		
		if ( (theCachedVBO != NULL) && theCachedVBO->IsStale() )
		{
			CachedVBOVec::iterator foundIt = FindVBOByGeom( *whichVec, inGeom );
			whichVec->erase( foundIt );
			
			DeleteVBO( theCachedVBO, inFuncs );
			
			theCachedVBO = NULL;
		}
	}
	
	return theCachedVBO;
}

TQ3Uns32		VBOCache::CountVBOs( TQ3GeometryObject inGeom )
{
	TQ3Uns32		refCount = 0;
	
	CachedVBO*	stripVBO = FindVBOInVec( inGeom, mCachedVBOs_strips );
	CachedVBO*	triVBO = FindVBOInVec( inGeom, mCachedVBOs_triangles );
	CachedVBO*	lineVBO = FindVBOInVec( inGeom, mCachedVBOs_lines );
	
	refCount = (stripVBO? 1 : 0) + (triVBO? 1 : 0) + (lineVBO? 1 : 0);
	
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
	CachedVBOVec* whichVec = GetVBOVecForMode( inVBO->mGLMode );
	
	if (whichVec != NULL)
	{
		CachedVBOVec::iterator	placeIt = std::lower_bound( whichVec->begin(),
			whichVec->end(), inVBO, VBOLess() );

		whichVec->insert( placeIt, inVBO );
		
		AddToUsageList( inVBO );

		mTotalBytes += inVBO->mBufferBytes;
		
		//Q3_MESSAGE_FMT("VBO cache size: %lld", mTotalBytes );
	}
}


void VBOCache::RenderVBO( const GLBufferFuncs& inFuncs, const CachedVBO* inCachedVBO )
{
	(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, inCachedVBO->mGLBufferNames[0] );
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
	
	(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER,
		inCachedVBO->mGLBufferNames[1] );
	glDrawElements( inCachedVBO->mGLMode, inCachedVBO->mNumIndices,
		GL_UNSIGNED_INT, BufferObPtr( 0 ) );
		
	(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
	(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void	VBOCache::DeleteFromUsageList( CachedVBO* ioVBO )
{
	ioVBO->mPrev->mNext = ioVBO->mNext;
	ioVBO->mNext->mPrev = ioVBO->mPrev;
}


void	VBOCache::DeleteVBO( CachedVBO* inCachedVBO, const GLBufferFuncs& inFuncs )
{
	(*inFuncs.glDeleteBuffersProc)( 2, inCachedVBO->mGLBufferNames );
	
	DeleteFromUsageList( inCachedVBO );
	
	mTotalBytes -= inCachedVBO->mBufferBytes;
	
	delete inCachedVBO;
}

void	VBOCache::DeleteCachedVBOs( CachedVBOVec& inVBOs, const GLBufferFuncs& inFuncs )
{
	CachedVBOVec::iterator endIt = inVBOs.end();
	for (CachedVBOVec::iterator i = inVBOs.begin(); i != endIt; ++i)
	{
		DeleteVBO( *i, inFuncs );
	}
}

void	VBOCache::FlushUnreferencedInVec( CachedVBOVec& ioVBOs, const GLBufferFuncs& inFuncs )
{
	// Move unreferenced VBOs to end of list
	CachedVBOVec::iterator startUnused = std::stable_partition(
		ioVBOs.begin(), ioVBOs.end(), IsReferenced() );
	
	if (startUnused != ioVBOs.end())
	{
		// Make a copy of the records that will go away
		CachedVBOVec	doomedVBOs( startUnused, ioVBOs.end() );
		
		// Remove them from the normal array
		ioVBOs.erase( startUnused, ioVBOs.end() );

		// Delete the buffers for the VBO records that are going away
		for (CachedVBOVec::iterator i = doomedVBOs.begin(); i != doomedVBOs.end(); ++i)
		{
			DeleteVBO( *i, inFuncs );
		}
	}
}

void	VBOCache::FlushUnreferenced( const GLBufferFuncs& inFuncs )
{
	FlushUnreferencedInVec( mCachedVBOs_strips, inFuncs );
	FlushUnreferencedInVec( mCachedVBOs_triangles, inFuncs );
	FlushUnreferencedInVec( mCachedVBOs_lines, inFuncs );
}

void	VBOCache::PurgeDownToSize( long long inTargetSize, const GLBufferFuncs& inFuncs )
{
	while (mTotalBytes > inTargetSize)
	{
		// Find the least recently used VBO from the doubly-linked list
		CachedVBO* oldestVBO = mListOldEnd.mNext;
		
		// Remove it from the sorted vector
		CachedVBOVec* whichVec = GetVBOVecForMode( oldestVBO->mGLMode );
		
		CachedVBOVec::iterator	foundIt = std::lower_bound( whichVec->begin(),
			whichVec->end(), oldestVBO, VBOLess() );
		if ( (foundIt != whichVec->end()) &&
			((*foundIt)->mGeomObject.get() == oldestVBO->mGeomObject.get()) )
		{
			whichVec->erase( foundIt );
		}
		
		// Remove the VBO, remove the record from the doubly-linked list, and
		// dispose the record.
		DeleteVBO( oldestVBO, inFuncs );
	}
}

void	VBOCache::SetMaxBufferSize( long long inBufferSize, const GLBufferFuncs& inFuncs )
{
	if (inBufferSize < mMaxBufferBytes)
	{
		PurgeDownToSize( inBufferSize, inFuncs );
	}
	
	mMaxBufferBytes = inBufferSize;
}

void	VBOCache::MakeRoom( TQ3Uns32 inBytesNeeded, const GLBufferFuncs& inFuncs )
{
	if ( (inBytesNeeded < mMaxBufferBytes) &&
		(mTotalBytes + inBytesNeeded > mMaxBufferBytes) )
	{
		long long targetSize = mMaxBufferBytes - inBytesNeeded;
		
		PurgeDownToSize( targetSize, inFuncs );
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
	@param			inFuncs			OpenGL buffer function pointers.
	@param			inMaxMemK		New memory limit in K-bytes.
*/
void				UpdateVBOCacheLimit(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3Uns32 inMaxMemK )
{
	VBOCache*	theCache = GetVBOCache( glContext );
	
	if (theCache != NULL)
	{
		theCache->SetMaxBufferSize( inMaxMemK * 1024LL, inFuncs );
	}
}


/*!
	@function		RenderCachedVBO
	@abstract		Look for a cached VBO for the given geometry and OpenGL
					context.  If we find one, render it.
	@discussion		If we find the object in the cache, but the cached object
					is stale, we delete it from the cache and return false.
	@param			glContext		An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
	@param			inGeom			A geometry object.
	@param			inMode			OpenGL mode, e.g., GL_TRIANGLES.
	@result			True if the object was found and rendered.
*/
TQ3Boolean			RenderCachedVBO(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3GeometryObject inGeom,
									GLenum inMode )
{
	TQ3Boolean	didRender = kQ3False;
	VBOCache*	theCache = GetVBOCache( glContext );
	
	if (theCache != NULL)
	{
		CachedVBO*	theVBO = theCache->FindVBO( inGeom, inMode, inFuncs );
		
		if ( (theVBO == NULL) && (inMode == GL_TRIANGLE_STRIP) )
		{
			theVBO = theCache->FindVBO( inGeom, GL_TRIANGLES, inFuncs );
		}
		
		if (theVBO != NULL)
		{
			theCache->RenderVBO( inFuncs, theVBO );
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
	@param			inFuncs			OpenGL buffer function pointers.
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
									const GLBufferFuncs& inFuncs,
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
		(*inFuncs.glGenBuffersProc)( 2, newVBO->mGLBufferNames );
		
		TQ3Uns32	vertexDataSize = static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3Point3D));
		TQ3Uns32	normalDataSize = (inNormals == NULL)? 0 :
			static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3Vector3D));
		TQ3Uns32	colorDataSize = (inColors == NULL)? 0 :
			static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3ColorRGB));
		TQ3Uns32	uvDataSize = (inUVs == NULL)? 0 :
			static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3Param2D));
		TQ3Uns32	totalDataSize = vertexDataSize + normalDataSize +
			colorDataSize + uvDataSize;
		
		newVBO->mBufferBytes = static_cast<TQ3Uns32>(inNumIndices * sizeof(TQ3Uns32) + totalDataSize);
		theCache->MakeRoom( newVBO->mBufferBytes, inFuncs );
			
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
		(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER,
			newVBO->mGLBufferNames[0] );
		(*inFuncs.glBufferDataProc)( GL_ARRAY_BUFFER, totalDataSize,
			NULL, GL_STATIC_DRAW );
		
		// Then set sub-buffer data
		dataAddr = inPoints;
		(*inFuncs.glBufferSubDataProc)( GL_ARRAY_BUFFER, 0,
			vertexDataSize, dataAddr );
		if (inNormals != NULL)
		{
			dataAddr = inNormals;
			(*inFuncs.glBufferSubDataProc)( GL_ARRAY_BUFFER,
				newVBO->mNormalBufferOffset, normalDataSize, dataAddr );
		}
		if (inColors != NULL)
		{
			dataAddr = inColors;
			(*inFuncs.glBufferSubDataProc)( GL_ARRAY_BUFFER,
				newVBO->mColorBufferOffset, colorDataSize, dataAddr );
		}
		if (inUVs != NULL)
		{
			dataAddr = inUVs;
			(*inFuncs.glBufferSubDataProc)( GL_ARRAY_BUFFER,
				newVBO->mTextureUVBufferOffset, uvDataSize, dataAddr );
		}
		(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
		
		// Now for the index data.
		(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER,
			newVBO->mGLBufferNames[1] );
		dataAddr = inIndices;
		(*inFuncs.glBufferDataProc)( GL_ELEMENT_ARRAY_BUFFER,
			inNumIndices * sizeof(TQ3Uns32), dataAddr, GL_STATIC_DRAW );
		(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
}



/*!
	@function		FlushVBOCache
	@abstract		Delete any cached VBOs for geometries that are no longer
					referenced elsewhere.
	@param			glContext		An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
*/
void				FlushVBOCache(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs )
{
	VBOCache*	theCache = GetVBOCache( glContext );
	
	if (theCache != NULL)
	{
		theCache->FlushUnreferenced( inFuncs );
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

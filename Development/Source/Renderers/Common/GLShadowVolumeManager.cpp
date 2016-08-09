/*  NAME:
        GLShadowVolumeManager.cpp

    DESCRIPTION:
        Quesa OpenGL shadow volume caching.
       
    COPYRIGHT:
        Copyright (c) 2011-2016, Quesa Developers. All rights reserved.

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
#include "GLShadowVolumeManager.h"
#include "GLGPUSharing.h"
#include "CQ3WeakObjectRef.h"
#include "GLUtils.h"
#include "E3Main.h"
#include "GLVBOManager.h"

#include <vector>
#include <algorithm>
#include <cstddef>
#include <map>
#include <utility>

//=============================================================================
//		Internal constants
//-----------------------------------------------------------------------------
namespace
{
	const TQ3Uns32	kShadowCacheKey	= Q3_FOUR_CHARACTER_CONSTANT('s', 'v', 'c', 'k');
	
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


namespace
{
	typedef std::pair< TQ3GeometryObject, TQ3LightObject >	GeomAndLight;

	typedef std::vector< class ShadowVBO* >					ShadowVec;
	typedef std::vector< GeomAndLight >						GeomLightVec;

	typedef std::map< GeomAndLight, ShadowVec >				GeomLightToShadows;
	
	const TQ3RationalPoint4D	kDummyPoint = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	const float					kMaxLightDistSq	= 7.0e-6f;

	/*!
		@class		ShadowVBO
		@abstract	Cached shadow volume VBO.
	*/
	class ShadowVBO
	{
	public:
							ShadowVBO(); // used for linked list sentinels
							ShadowVBO( TQ3GeometryObject inGeom,
										TQ3LightObject inLight,
										const TQ3RationalPoint4D& inLocalLightPos );
							~ShadowVBO();

		bool				IsStale() const;
							
		CQ3WeakObjectRef	mGeomObject;
		TQ3Uns32			mGeomEditIndex;

		CQ3WeakObjectRef	mLightObject;
		TQ3RationalPoint4D	mLocalLightPosition;	// w is 0 or 1

		GeomAndLight		mMapKey;

		TQ3Uns32			mNumTriIndices;
		TQ3Uns32			mNumQuadIndices;
		GLuint				mGLBufferNames[2];	// 0 is array, 1 is index
		TQ3Uns32			mBufferBytes;
	
	private:
		ShadowVBO*			mPrev;
		ShadowVBO*			mNext;
		
		// Unimplemented; declared privately just to make sure that they don't
		// get used accidentally.
							ShadowVBO( const ShadowVBO& inOther );
		ShadowVBO&			operator=( const ShadowVBO& inOther );
		friend class ShadowVolCache;
	};
	
	
	/*!
		@class		ShadowVolCache
		@abstract	Cache of shadow volume VBOs for a sharing set of OpenGL
					contexts.
	*/
	class ShadowVolCache : public CQ3GPSharedCache
	{
	public:
							ShadowVolCache();
							~ShadowVolCache();

		void				DeleteVBO( ShadowVBO* inCachedVBO, const GLBufferFuncs& inFuncs );
		ShadowVBO*			FindVBO( TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos,
									const GLBufferFuncs& inFuncs );
		void				RenderVBO( ShadowVBO* inCachedVBO, const GLBufferFuncs& inFuncs );
		void				AddVBO( ShadowVBO* inVBO );
		void				FlushStaleShadows( const GLBufferFuncs& inFuncs );
		void				MakeRoom( TQ3Uns32 inBytesNeeded, const GLBufferFuncs& inFuncs );
		void				AddBytes( TQ3Uns32 inBytes );
		void				PurgeDownToSize( long long inTargetSize, const GLBufferFuncs& inFuncs );
		void				SetMaxBufferSize( long long inBufferSize, const GLBufferFuncs& inFuncs );
	
	private:
		
		GeomLightToShadows			mGeomLightToShadows;
		ShadowVBO					mListOldEnd;
		ShadowVBO					mListNewEnd;
		long long					mTotalBytes;
		long long					mMaxBufferBytes;
	};
}



//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------

static inline GLvoid* BufferObPtr( GLuint offset )
{
	return (GLvoid*)( ((char*)NULL) + offset );
}


static ShadowVolCache* GetShadowCache( TQ3GLContext glContext )
{
	ShadowVolCache*	theCache = static_cast<ShadowVolCache*>( GLGPUSharing_GetCache(
		glContext, kShadowCacheKey ) );
	
	if (theCache == NULL)
	{
		theCache = new(std::nothrow) ShadowVolCache;
		
		if (theCache != NULL)
		{
			GLGPUSharing_AddCache( glContext, kShadowCacheKey, theCache );
		}
	}
	
	return theCache;
}

static bool IsSameLightPosition( const TQ3RationalPoint4D& a,
								const TQ3RationalPoint4D& b )
{
	TQ3RationalPoint4D d =
	{
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
		a.w - b.w
	};
	float distSq = d.x * d.x + d.y * d.y + d.z * d.z + d.w * d.w;
	return distSq < kMaxLightDistSq;
}


#pragma mark -

ShadowVBO::ShadowVBO()
	: mGeomObject()
	, mLightObject()
	, mBufferBytes( 0 )
	, mPrev( NULL )
	, mNext( NULL )
{
}

ShadowVBO::ShadowVBO(					TQ3GeometryObject inGeom,
										TQ3LightObject inLight,
										const TQ3RationalPoint4D& inLocalLightPos )
	: mGeomObject( inGeom )
	, mGeomEditIndex( Q3Shared_GetEditIndex( inGeom ) )
	, mLightObject( inLight )
	, mLocalLightPosition( inLocalLightPos )
	, mMapKey( inGeom, inLight )
	, mBufferBytes( 0 )
	, mPrev( NULL )
	, mNext( NULL )
{
	// Leave other fields uninitialized for now
}

ShadowVBO::~ShadowVBO()
{
}

bool	ShadowVBO::IsStale() const
{
	bool stale = true;
	
	if (mGeomObject.isvalid() && mLightObject.isvalid())
	{
		TQ3Uns32 curEditIndex = static_cast<E3Shared*>( mGeomObject.get() )->GetEditIndex();
		if (curEditIndex == mGeomEditIndex)
		{
			stale = false;
		}
	}

	return stale;
}

#pragma mark -

ShadowVolCache::ShadowVolCache()
	: mTotalBytes( 0 )
	, mMaxBufferBytes( 0 )
{
	mListOldEnd.mNext = &mListNewEnd;
	mListNewEnd.mPrev = &mListOldEnd;
}

ShadowVolCache::~ShadowVolCache()
{
	// Note:  This object will only be destroyed after the last OpenGL context
	// of its sharing group has gone away.  Hopefully, this will automatically
	// clear the VBOs from VRAM.  Hence, we do not need or want to call DeleteVBO,
	// but we do want to delete the ShadowVBO objects.
	ShadowVBO* listItem = mListOldEnd.mNext;
	
	while (listItem != &mListNewEnd)
	{
		listItem->mPrev->mNext = listItem->mNext;
		listItem->mNext->mPrev = listItem->mPrev;
		delete listItem;
		listItem = mListOldEnd.mNext;
	}
}


void	ShadowVolCache::DeleteVBO( ShadowVBO* inCachedVBO, const GLBufferFuncs& inFuncs )
{
	// Remove the buffers from VRAM.
	if ( inCachedVBO->mNumTriIndices + inCachedVBO->mNumQuadIndices > 0)
	{
		(*inFuncs.glDeleteBuffersProc)( 2, inCachedVBO->mGLBufferNames );
	}
	
	// Remove the record from the doubly-linked list.
	inCachedVBO->mPrev->mNext = inCachedVBO->mNext;
	inCachedVBO->mNext->mPrev = inCachedVBO->mPrev;
	
	mTotalBytes -= inCachedVBO->mBufferBytes;
	
	delete inCachedVBO;
}


void	ShadowVolCache::AddVBO( ShadowVBO* inVBO )
{
	// Add the new record to our map
	GeomAndLight key( inVBO->mGeomObject.get(), inVBO->mLightObject.get() );
	
	GeomLightToShadows::iterator found = mGeomLightToShadows.find( key );
	
	if (found == mGeomLightToShadows.end())
	{
		mGeomLightToShadows[ key ] = ShadowVec();
		found = mGeomLightToShadows.find( key );
	}
	
	found->second.push_back( inVBO );

	// Insert the new record at the new end of a doubly-linked list.
	inVBO->mNext = &mListNewEnd;
	inVBO->mPrev = mListNewEnd.mPrev;
	inVBO->mPrev->mNext = inVBO;
	mListNewEnd.mPrev = inVBO;
	
	mTotalBytes += inVBO->mBufferBytes;
}


ShadowVBO*	ShadowVolCache::FindVBO( TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos,
									const GLBufferFuncs& inFuncs )
{
	ShadowVBO* cachedShadow = NULL;
	
	GeomAndLight key( inGeom, inLight );
	
	GeomLightToShadows::iterator found = mGeomLightToShadows.find( key );
	
	if (found != mGeomLightToShadows.end())
	{
		ShadowVec& theShadows( found->second );
		
		// Linear search for a light position that is close enough.
		ShadowVec::iterator endIt = theShadows.end();
		for (ShadowVec::iterator i = theShadows.begin(); i != endIt; ++i)
		{
			if (IsSameLightPosition( (*i)->mLocalLightPosition, inLocalLightPos ))
			{
				cachedShadow = *i;
				
				if (cachedShadow->IsStale())
				{
					DeleteVBO( cachedShadow, inFuncs );
					theShadows.erase( i );
					cachedShadow = NULL;
					break;
				}
			}
		}
	}
	
	return cachedShadow;
}

void	ShadowVolCache::RenderVBO( ShadowVBO* inCachedVBO, const GLBufferFuncs& inFuncs )
{
	if ( inCachedVBO->mNumTriIndices + inCachedVBO->mNumQuadIndices > 0)
	{
		(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, inCachedVBO->mGLBufferNames[0] );
		glVertexPointer( 4, GL_FLOAT, 0, BufferObPtr( 0 ) );

		(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER,
			inCachedVBO->mGLBufferNames[1] );
		
		if (inCachedVBO->mNumTriIndices > 0)
		{
			glDrawElements( GL_TRIANGLES, inCachedVBO->mNumTriIndices,
				GL_UNSIGNED_INT, BufferObPtr( 0 ) );
		}

		if (inCachedVBO->mNumQuadIndices > 0)
		{
			glDrawElements( GL_QUADS, inCachedVBO->mNumQuadIndices,
				GL_UNSIGNED_INT,
				BufferObPtr( inCachedVBO->mNumTriIndices * sizeof(GLuint) ) );
		}

		(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
		(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
	
	// Remove the record from the doubly-linked list.
	inCachedVBO->mPrev->mNext = inCachedVBO->mNext;
	inCachedVBO->mNext->mPrev = inCachedVBO->mPrev;

	// Move it to the new end of the list
	inCachedVBO->mNext = &mListNewEnd;
	inCachedVBO->mPrev = mListNewEnd.mPrev;
	inCachedVBO->mPrev->mNext = inCachedVBO;
	mListNewEnd.mPrev = inCachedVBO;
}

void	ShadowVolCache::FlushStaleShadows( const GLBufferFuncs& inFuncs )
{
	GeomLightVec toFlush;
	ShadowVec staleShadows;
	
	GeomLightToShadows::iterator endMap = mGeomLightToShadows.end();
	for (GeomLightToShadows::iterator i = mGeomLightToShadows.begin();
		i != endMap; ++i)
	{
		ShadowVec& theShadows( i->second );
		// Find the first stale shadow
		ShadowVec::iterator endVec = theShadows.end();
		ShadowVec::iterator first;
		for (first = theShadows.begin(); first != endVec; ++first)
		{
			if ((*first)->IsStale())
			{
				break;
			}
		}
		if (first != endVec)
		{
			// As we go, delete stale ones and move the good ones to the left.
			// The algorithm is similar to std::remove_if.
			DeleteVBO( *first, inFuncs );
			
			ShadowVec::iterator j;
			
			for (j = first, ++j; j != endVec; ++j)
			{
				if ((*j)->IsStale())
				{
					DeleteVBO( *j, inFuncs );
				}
				else
				{
					*first = *j;
					++first;
				}
			}
			theShadows.erase( first, endVec );
		}
	}
}


void	ShadowVolCache::MakeRoom( TQ3Uns32 inBytesNeeded, const GLBufferFuncs& inFuncs )
{
	if ( (inBytesNeeded < mMaxBufferBytes) &&
		(mTotalBytes + inBytesNeeded > mMaxBufferBytes) )
	{
		long long targetSize = mMaxBufferBytes - inBytesNeeded;
		
		PurgeDownToSize( targetSize, inFuncs );
	}
}

void	ShadowVolCache::PurgeDownToSize( long long inTargetSize, const GLBufferFuncs& inFuncs )
{
	while ( (mTotalBytes > inTargetSize) && (mListOldEnd.mNext != &mListNewEnd) )
	{
		// Find the least recently used VBO from the doubly-linked list
		ShadowVBO* oldestVBO = mListOldEnd.mNext;
		
		// Remove it from the map index
		GeomLightToShadows::iterator found = mGeomLightToShadows.find( oldestVBO->mMapKey );
		if (found != mGeomLightToShadows.end())
		{
			ShadowVec& theShadows( found->second );
			ShadowVec::iterator foundShadow = std::find( theShadows.begin(),
				theShadows.end(), oldestVBO );
			if (foundShadow != theShadows.end())
			{
				theShadows.erase( foundShadow );
				if (theShadows.empty())
				{
					mGeomLightToShadows.erase( found );
				}
			}
		}
		
		// Remove it from the doubly linked list, remove the VBOs, update
		// mTotalBytes, and delete the ShadowVBO record.
		DeleteVBO( oldestVBO, inFuncs );
	}
}

void	ShadowVolCache::SetMaxBufferSize( long long inBufferSize, const GLBufferFuncs& inFuncs )
{
	if (inBufferSize < mMaxBufferBytes)
	{
		PurgeDownToSize( inBufferSize, inFuncs );
	}
	
	mMaxBufferBytes = inBufferSize;
}

#pragma mark -

/*!
	@function	StartFrame
	@abstract	Update the limit on memory that can be used in this cache.
	@param			glContext		An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
	@param			memLimitK		New memory limit in K-bytes.
*/
void	ShadowVolMgr::StartFrame(	TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3Uns32 memLimitK )
{
	ShadowVolCache*	theCache = GetShadowCache( glContext );
	
	if (theCache != NULL)
	{
		theCache->SetMaxBufferSize( memLimitK * 1024LL, inFuncs );
	}
}

/*!
	@function	RenderShadowVolume
	@abstract	Look for a cached shadow volume for a given geometry, OpenGL
				context, and shadow-casting light.  If we find one, render it.
	@discussion		If we find the object in the cache, but the cached object
					is stale, we delete it from the cache and return false.
	@param			glContext		An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
	@param			inGeom			A geometry object.
	@param			inLight			A light object.
	@param			inLocalLightPos	The position of the light in local coordinates.
	@result			True if the object was found and rendered.
*/
TQ3Boolean		ShadowVolMgr::RenderShadowVolume(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos )
{
	TQ3Boolean	didRender = kQ3False;
	ShadowVolCache*	theCache = GetShadowCache( glContext );
	
	if (theCache != NULL)
	{
		ShadowVBO* theVBO = theCache->FindVBO( inGeom, inLight, inLocalLightPos, inFuncs );
		
		if (theVBO != NULL)
		{
			theCache->RenderVBO( theVBO, inFuncs );
			didRender = kQ3True;
		}
	}
	
	return didRender;
}

/*!
	@function	AddShadowVolume
	@abstract	Add a shadow volume mesh to the cache.  Do not call this unless
				RenderCachedShadowVolume has just returned false.
	@param			glContext			An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
	@param			inGeom				A geometry object.
	@param			inLight				A light object.
	@param			inLocalLightPos		The position of the light in local coordinates.
	@param			inNumPoints			Number of points (vertices).
	@param			inPoints			Array of point locations.
	@param			inNumTriIndices		Number of triangle vertex indices to follow.
	@param			inNumQuadIndices	Number of quad vertex indices to follow.
	@param			inVertIndices		Array of vertex indices (triangle, then quad).
*/
void	ShadowVolMgr::AddShadowVolume(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos,
									TQ3Uns32 inNumPoints,
									const TQ3RationalPoint4D* inPoints,
									TQ3Uns32 inNumTriIndices,
									TQ3Uns32 inNumQuadIndices,
									const GLuint* inVertIndices )
{
	ShadowVolCache*	theCache = GetShadowCache( glContext );
	
	if (theCache != NULL)
	{
		ShadowVBO* newVBO = new ShadowVBO( inGeom, inLight, inLocalLightPos );
		
		newVBO->mNumTriIndices = inNumTriIndices;
		newVBO->mNumQuadIndices = inNumQuadIndices;
		
		if (inNumTriIndices + inNumQuadIndices > 0)
		{
			newVBO->mBufferBytes = inNumPoints * sizeof(TQ3RationalPoint4D) +
				(inNumTriIndices + inNumQuadIndices) * sizeof(GLuint);
			theCache->MakeRoom( newVBO->mBufferBytes, inFuncs );

			// Get buffer names
			(*inFuncs.glGenBuffersProc)( 2, newVBO->mGLBufferNames );
			
			// Copy point data into array buffer.
			(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER,
				newVBO->mGLBufferNames[0] );
			(*inFuncs.glBufferDataProc)( GL_ARRAY_BUFFER,
				inNumPoints * sizeof(TQ3RationalPoint4D),
				inPoints, GL_STATIC_DRAW );
			(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
			
			// Index data into elements buffer.
			(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER,
				newVBO->mGLBufferNames[1] );
			(*inFuncs.glBufferDataProc)( GL_ELEMENT_ARRAY_BUFFER,
				(inNumTriIndices + inNumQuadIndices) * sizeof(GLuint),
				inVertIndices, GL_STATIC_DRAW );
			(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
		}
		
		theCache->AddVBO( newVBO );
	}
}


/*!
	@function		Flush
	@abstract		Delete any cached VBOs for geometries that are no longer
					referenced elsewhere, or lights that are no longer
					referenced elsewhere.
	@param			glContext		An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
	@param			inRenderer		A Quesa renderer.
*/
void	ShadowVolMgr::Flush(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3RendererObject inRenderer )
{
	ShadowVolCache*	theCache = GetShadowCache( glContext );
	
	if (theCache != NULL)
	{
		theCache->FlushStaleShadows( inFuncs );
	}
}


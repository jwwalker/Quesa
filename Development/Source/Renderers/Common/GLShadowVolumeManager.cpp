/*  NAME:
        GLShadowVolumeManager.cpp

    DESCRIPTION:
        Quesa OpenGL shadow volume caching.
       
    COPYRIGHT:
        Copyright (c) 2011-2019, Quesa Developers. All rights reserved.

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
#include "GLVBOManager.h"
#include "CQ3WeakObjectRef.h"
#include "GLUtils.h"
#include "E3Main.h"
#include "E3CustomElements.h"
#include "QORenderer.h"

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

#ifndef GL_ARB_multitexture
	#define GL_TEXTURE0_ARB                   0x84C0
#endif

//#define VALIDATE_COUNTS		1


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
	
	typedef std::map< GeomAndLight, class ShadowVBO* >		GeomLightToShadow;
		
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
		GLuint				mGLBufferNames[2];	// 0 is array, 1 is index
		TQ3Uns32			mBufferBytes;
		TQ3Uns32			mLayerShiftOffset;

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

		void				DeleteVBO( ShadowVBO* inCachedVBO, const QORenderer::GLFuncs& inFuncs );
		ShadowVBO*			FindVBO( TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos,
									const QORenderer::GLFuncs& inFuncs );
		void				RenderVBO( ShadowVBO* inCachedVBO, const QORenderer::Renderer& inRenderer );
		void				AddVBO( ShadowVBO* inVBO );
		void				FlushStaleShadows( const QORenderer::GLFuncs& inFuncs );
		void				MakeRoom( TQ3Uns32 inBytesNeeded, const QORenderer::GLFuncs& inFuncs );
		void				PurgeDownToSize( long long inTargetSize, const QORenderer::GLFuncs& inFuncs );
		void				SetMaxBufferSize( long long inBufferSize, const QORenderer::GLFuncs& inFuncs );
	
	#if VALIDATE_COUNTS
		void				ValidateByteCount() const;
		void				ValidateShadowCount() const;
	#endif
	
	private:
		
		GeomLightToShadow			mGeomLightToShadows;
		ShadowVBO					mListOldEnd;
		ShadowVBO					mListNewEnd;
		long long					mTotalBytes;
		long long					mMaxBufferBytes;
	};
}

#if VALIDATE_COUNTS
	#define VALIDATE_COUNT(cache)	do {\
										cache->ValidateByteCount();		\
										cache->ValidateShadowCount();	\
									} while (false)
#else
	#define VALIDATE_COUNT(cache)
#endif

//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------

static ShadowVolCache* GetShadowCache( TQ3GLContext glContext )
{
	ShadowVolCache*	theCache = static_cast<ShadowVolCache*>( GLGPUSharing_GetCache(
		glContext, kShadowCacheKey ) );
	
	if (theCache == nullptr)
	{
		theCache = new(std::nothrow) ShadowVolCache;
		
		if (theCache != nullptr)
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
	, mLayerShiftOffset( kAbsentBuffer )
	, mPrev( nullptr )
	, mNext( nullptr )
{
}

#if 0//Q3_DEBUG
static long sShadowVBOCount = 0;
#endif

ShadowVBO::ShadowVBO(					TQ3GeometryObject inGeom,
										TQ3LightObject inLight,
										const TQ3RationalPoint4D& inLocalLightPos )
	: mGeomObject( inGeom )
	, mGeomEditIndex( Q3Shared_GetEditIndex( inGeom ) )
	, mLightObject( inLight )
	, mLocalLightPosition( inLocalLightPos )
	, mMapKey( inGeom, inLight )
	, mBufferBytes( 0 )
	, mLayerShiftOffset( kAbsentBuffer )
	, mPrev( nullptr )
	, mNext( nullptr )
{
	// Leave other fields uninitialized for now
	//Q3_MESSAGE_FMT("ShadowVBO created, %ld", ++sShadowVBOCount );
}

ShadowVBO::~ShadowVBO()
{
	//Q3_MESSAGE_FMT("ShadowVBO destroyed, %ld", --sShadowVBOCount);
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
	VALIDATE_COUNT(this);
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
		ShadowVBO* toDelete = listItem;
		listItem = listItem->mNext;
		delete toDelete;
	}
}

#if VALIDATE_COUNTS
void	ShadowVolCache::ValidateByteCount() const
{
	long long trueTotal = 0;
	ShadowVBO* listPtr;
	for (listPtr = mListOldEnd.mNext; listPtr != &mListNewEnd; listPtr = listPtr->mNext)
	{
		trueTotal += listPtr->mBufferBytes;
	}
	Q3_ASSERT_FMT( trueTotal == mTotalBytes, "Actual shadow VBO byte count %lld != nominal %lld",
		 trueTotal, mTotalBytes );
}

void	ShadowVolCache::ValidateShadowCount() const
{
	size_t listCount = 0;
	ShadowVBO* listPtr;
	for (listPtr = mListOldEnd.mNext; listPtr != &mListNewEnd; listPtr = listPtr->mNext)
	{
		++listCount;
	}
	
	size_t mapCount = mGeomLightToShadows.size();
	Q3_ASSERT_FMT( mapCount == listCount, "Shadow cache list has %d shadows, "
		"map has %d shadows", (int)listCount, (int)mapCount );
}
#endif

void	ShadowVolCache::DeleteVBO( ShadowVBO* inCachedVBO, const QORenderer::GLFuncs& inFuncs )
{
	// Remove the buffers from VRAM.
	if ( inCachedVBO->mNumTriIndices > 0)
	{
		(*inFuncs.glDeleteBuffersProc)( 2, inCachedVBO->mGLBufferNames );
		ForgetVBO( inCachedVBO->mGLBufferNames[0] );
		ForgetVBO( inCachedVBO->mGLBufferNames[1] );
	}
	
	// Remove the record from the doubly-linked list.
	inCachedVBO->mPrev->mNext = inCachedVBO->mNext;
	inCachedVBO->mNext->mPrev = inCachedVBO->mPrev;
	
	mTotalBytes -= inCachedVBO->mBufferBytes;
	
	delete inCachedVBO;
}


void	ShadowVolCache::AddVBO( ShadowVBO* inVBO )
{
	VALIDATE_COUNT(this);
	
	// Add the new record to our map
	GeomAndLight key( inVBO->mGeomObject.get(), inVBO->mLightObject.get() );
	
	GeomLightToShadow::iterator found = mGeomLightToShadows.find( key );
	
	Q3_ASSERT_FMT( found == mGeomLightToShadows.end(), "Adding shadow VBO without deleting a stale one" );
	
	mGeomLightToShadows[ key ] = inVBO;

	// Insert the new record at the new end of a doubly-linked list.
	inVBO->mNext = &mListNewEnd;
	inVBO->mPrev = mListNewEnd.mPrev;
	inVBO->mPrev->mNext = inVBO;
	mListNewEnd.mPrev = inVBO;
	
	mTotalBytes += inVBO->mBufferBytes;
	
	VALIDATE_COUNT(this);
	
	//Q3_MESSAGE_FMT("Shadow cache %lld bytes", mTotalBytes );
}


ShadowVBO*	ShadowVolCache::FindVBO( TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos,
									const QORenderer::GLFuncs& inFuncs )
{
	ShadowVBO* cachedShadow = nullptr;
	VALIDATE_COUNT(this);
	
	GeomAndLight key( inGeom, inLight );
	
	GeomLightToShadow::iterator found = mGeomLightToShadows.find( key );
	
	if (found != mGeomLightToShadows.end())
	{
		cachedShadow = found->second;
		
		if ( cachedShadow->IsStale() ||
			(! IsSameLightPosition( cachedShadow->mLocalLightPosition, inLocalLightPos )) )
		{
			DeleteVBO( cachedShadow, inFuncs );
			mGeomLightToShadows.erase( found );
			cachedShadow = nullptr;
		}
	}
	VALIDATE_COUNT(this);
	
	return cachedShadow;
}

void	ShadowVolCache::RenderVBO( ShadowVBO* inCachedVBO, const QORenderer::Renderer& inRenderer )
{
	if ( inCachedVBO->mNumTriIndices > 0)
	{
		(*inRenderer.Funcs().glBindBufferProc)( GL_ARRAY_BUFFER, inCachedVBO->mGLBufferNames[0] );
		inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mVertexAttribLoc,
			4, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( 0U ) );
		
		if (inCachedVBO->mLayerShiftOffset != kAbsentBuffer)
		{
			inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mLayerShiftAttribLoc,
				1, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( inCachedVBO->mLayerShiftOffset ) );
		}

		(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER,
			inCachedVBO->mGLBufferNames[1] );
		
		if (inCachedVBO->mNumTriIndices > 0)
		{
			glDrawElements( GL_TRIANGLES, inCachedVBO->mNumTriIndices,
				GL_UNSIGNED_INT, GLBufferObPtr( 0U ) );
		}

		(*inRenderer.Funcs().glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
		(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
	
	// Remove the record from the doubly-linked list.
	inCachedVBO->mPrev->mNext = inCachedVBO->mNext;
	inCachedVBO->mNext->mPrev = inCachedVBO->mPrev;

	// Move it to the new end of the list
	inCachedVBO->mNext = &mListNewEnd;
	inCachedVBO->mPrev = mListNewEnd.mPrev;
	inCachedVBO->mPrev->mNext = inCachedVBO;
	mListNewEnd.mPrev = inCachedVBO;
	VALIDATE_COUNT(this);
}

void	ShadowVolCache::FlushStaleShadows( const QORenderer::GLFuncs& inFuncs )
{
	VALIDATE_COUNT(this);
	GeomLightToShadow::iterator endMap = mGeomLightToShadows.end();
	GeomLightToShadow::iterator i = mGeomLightToShadows.begin();
	
	while (i != endMap)
	{
		// Get next iterator now, in case we invalidate the current one.
		GeomLightToShadow::iterator next = i;
		++next;
		
		ShadowVBO* theShadow = i->second;
		
		if (theShadow->IsStale())
		{
			DeleteVBO( theShadow, inFuncs );
			mGeomLightToShadows.erase( i );
		}
		
		i = next;
	}

	VALIDATE_COUNT(this);
}


void	ShadowVolCache::MakeRoom( TQ3Uns32 inBytesNeeded, const QORenderer::GLFuncs& inFuncs )
{
	if ( (inBytesNeeded < mMaxBufferBytes) &&
		(mTotalBytes + inBytesNeeded > mMaxBufferBytes) )
	{
		long long targetSize = mMaxBufferBytes - inBytesNeeded;
		
		PurgeDownToSize( targetSize, inFuncs );
	}
}

void	ShadowVolCache::PurgeDownToSize( long long inTargetSize, const QORenderer::GLFuncs& inFuncs )
{
	VALIDATE_COUNT(this);
	while ( (mTotalBytes > inTargetSize) && (mListOldEnd.mNext != &mListNewEnd) )
	{
		// Find the least recently used VBO from the doubly-linked list
		ShadowVBO* oldestVBO = mListOldEnd.mNext;
		
		// Remove it from the map index
		mGeomLightToShadows.erase( oldestVBO->mMapKey );
		
		// Remove it from the doubly linked list, remove the VBOs, update
		// mTotalBytes, and delete the ShadowVBO record.
		DeleteVBO( oldestVBO, inFuncs );
	}
	VALIDATE_COUNT(this);
}

void	ShadowVolCache::SetMaxBufferSize( long long inBufferSize, const QORenderer::GLFuncs& inFuncs )
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
	@param			inRenderer		An OpenGL renderer.
	@param			memLimitK		New memory limit in K-bytes.
*/
void	ShadowVolMgr::StartFrame(	const QORenderer::Renderer& inRenderer,
									TQ3Uns32 memLimitK )
{
	ShadowVolCache*	theCache = GetShadowCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		theCache->SetMaxBufferSize( memLimitK * 1024LL, inRenderer.Funcs() );
	}
}

/*!
	@function	RenderShadowVolume
	@abstract	Look for a cached shadow volume for a given geometry, OpenGL
				context, and shadow-casting light.  If we find one, render it.
	@discussion		If we find the object in the cache, but the cached object
					is stale, we delete it from the cache and return false.
	@param			inRenderer		An OpenGL renderer.
	@param			inGeom			A geometry object.
	@param			inLight			A light object.
	@param			inLocalLightPos	The position of the light in local coordinates.
	@result			True if the object was found and rendered.
*/
TQ3Boolean		ShadowVolMgr::RenderShadowVolume(
									const QORenderer::Renderer& inRenderer,
									TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos )
{
	TQ3Boolean	didRender = kQ3False;
	ShadowVolCache*	theCache = GetShadowCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		ShadowVBO* theVBO = theCache->FindVBO( inGeom, inLight, inLocalLightPos, inRenderer.Funcs() );
		
		if (theVBO != nullptr)
		{
			theCache->RenderVBO( theVBO, inRenderer );
			didRender = kQ3True;
		}
	}
	
	return didRender;
}

/*!
	@function	AddShadowVolume
	@abstract	Add a shadow volume mesh to the cache.  Do not call this unless
				RenderCachedShadowVolume has just returned false.
	@param			inRenderer			An OpenGL renderer.
	@param			inGeom				A geometry object.
	@param			inLight				A light object.
	@param			inLocalLightPos		The position of the light in local coordinates.
	@param			inNumPoints			Number of points (vertices).
	@param			inPoints			Array of point locations.
	@param			inNumTriIndices		Number of triangle vertex indices to follow.
	@param			inVertIndices		Array of vertex indices (triangle, then quad).
*/
void	ShadowVolMgr::AddShadowVolume(
									const QORenderer::Renderer& inRenderer,
									TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos,
									TQ3Uns32 inNumPoints,
									const TQ3RationalPoint4D* inPoints,
									TQ3Uns32 inNumTriIndices,
									const GLuint* inVertIndices )
{
	ShadowVolCache*	theCache = GetShadowCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		VALIDATE_COUNT(theCache);
		ShadowVBO* newVBO = new ShadowVBO( inGeom, inLight, inLocalLightPos );
		
		newVBO->mNumTriIndices = inNumTriIndices;
		
		if (inNumTriIndices > 0)
		{
			newVBO->mBufferBytes = inNumPoints * sizeof(TQ3RationalPoint4D) +
				inNumTriIndices * sizeof(GLuint);
			theCache->MakeRoom( newVBO->mBufferBytes, inRenderer.Funcs() );

			// Get buffer names
			(*inRenderer.Funcs().glGenBuffersProc)( 2, newVBO->mGLBufferNames );
			
			// Look for optional layer shift data
			TQ3Uns32 layerShiftSize;
			TQ3LayerShifts* layerShifts = nullptr;
			std::vector<TQ3Uns8>	layerShiftBuffer;
			if ( (kQ3Success == Q3Object_GetProperty( inGeom,
				kQ3GeometryPropertyLayerShifts, 0, &layerShiftSize, nullptr )) &&
				((layerShiftSize % 4) == 0) )
			{
				layerShiftBuffer.resize( layerShiftSize );
				Q3Object_GetProperty( inGeom, kQ3GeometryPropertyLayerShifts,
					layerShiftSize, nullptr, &layerShiftBuffer[0] );
				layerShifts = reinterpret_cast<TQ3LayerShifts*>( &layerShiftBuffer[0] );
				if (layerShifts->numPoints >= inNumPoints)
				{
					layerShifts = nullptr;	// bogus data, ignore it
				}
			}
			
			// Compute sizes of array data sub-buffers
			TQ3Uns32 pointDataSize = inNumPoints * sizeof(TQ3RationalPoint4D);
			TQ3Uns32 layerShiftDataSize = (layerShifts == nullptr)? 0 :
				inNumPoints * sizeof(float);
			TQ3Uns32 totalArrayDataSize = pointDataSize + layerShiftDataSize;
			newVBO->mLayerShiftOffset = (layerShifts == nullptr)? kAbsentBuffer : pointDataSize;
			
			// Create the array buffer
			(*inRenderer.Funcs().glBindBufferProc)( GL_ARRAY_BUFFER,
				newVBO->mGLBufferNames[0] );
			(*inRenderer.Funcs().glBufferDataProc)( GL_ARRAY_BUFFER,
				totalArrayDataSize, nullptr, GL_STATIC_DRAW );
			GLenum error = glGetError();
			if (error == GL_OUT_OF_MEMORY)
			{
				DumpVBOs();
				Q3_ASSERT_FMT( false, "Failed to allocate shadow VBO");
			}
			
			// Copy the vertex data into the buffer
			inRenderer.Funcs().glBufferSubDataProc( GL_ARRAY_BUFFER, 0,
				pointDataSize, inPoints );
			
			// If there's layer data, copy that into the buffer.
			if (layerShifts != nullptr)
			{
				inRenderer.Funcs().glBufferSubDataProc( GL_ARRAY_BUFFER, pointDataSize,
					layerShifts->numPoints * sizeof(float), layerShifts->coords );
				
				// fill the rest with zero
				std::vector<float> zeros( inNumPoints - layerShifts->numPoints, 0.0f );
				inRenderer.Funcs().glBufferSubDataProc( GL_ARRAY_BUFFER,
					pointDataSize + layerShifts->numPoints * sizeof(float),
					zeros.size() * sizeof(float), &zeros[0] );
			}
			
			(*inRenderer.Funcs().glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
			RecordVBO( newVBO->mGLBufferNames[0], inGeom, inNumPoints * sizeof(TQ3RationalPoint4D), 2 );
			
			// Index data into elements buffer.
			(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER,
				newVBO->mGLBufferNames[1] );
			(*inRenderer.Funcs().glBufferDataProc)( GL_ELEMENT_ARRAY_BUFFER,
				inNumTriIndices * sizeof(GLuint),
				inVertIndices, GL_STATIC_DRAW );
			(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
			RecordVBO( newVBO->mGLBufferNames[1], inGeom, (inNumTriIndices + inNumQuadIndices) * sizeof(GLuint), 2 );
		}
		
		theCache->AddVBO( newVBO );
		VALIDATE_COUNT(theCache);
	}
}


/*!
	@function		Flush
	@abstract		Delete any cached VBOs for geometries that are no longer
					referenced elsewhere, or lights that are no longer
					referenced elsewhere.
	@param			inRenderer			An OpenGL renderer.
	@param			inQuesaRenderer		A Quesa renderer.
*/
void	ShadowVolMgr::Flush(
									const QORenderer::Renderer& inRenderer,
									TQ3RendererObject inQuesaRenderer )
{
	ShadowVolCache*	theCache = GetShadowCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		theCache->FlushStaleShadows( inRenderer.Funcs() );
	}
}


/*  NAME:
        GLShadowVolumeManager.cpp

    DESCRIPTION:
        Quesa OpenGL shadow volume caching.
       
    COPYRIGHT:
        Copyright (c) 2011, Quesa Developers. All rights reserved.

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
#include "CQ3ObjectRef.h"
#include "GLUtils.h"
#include "E3Main.h"
#include "GLVBOManager.h"

#include <vector>
#include <algorithm>
#include <cstddef>
#include <map>

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
	typedef	std::map< TQ3LightObject, class ShadowVBO* >	LightToShadow;
	typedef	std::map< TQ3GeometryObject, LightToShadow >	GeomToLightToShadow;

	typedef	std::map< TQ3GeometryObject, class ShadowVBO* > GeomToShadow;
	typedef	std::map< TQ3LightObject, GeomToShadow >		LightToGeomToShadow;

	typedef std::vector< CQ3ObjectRef >						ObVec;
	
	const TQ3RationalPoint4D	kDummyPoint = { 0.0f, 0.0f, 0.0f, 0.0f };

	/*!
		@class		ShadowVBO
		@abstract	Cached shadow volume VBO.
	*/
	class ShadowVBO
	{
	public:
							ShadowVBO();
							ShadowVBO( TQ3GeometryObject inGeom,
										TQ3LightObject inLight,
										const TQ3RationalPoint4D& inLocalLightPos );
							~ShadowVBO();

		bool				IsStale( const TQ3RationalPoint4D& inLocalLightPos ) const;
							
		CQ3ObjectRef		mGeomObject;
		TQ3Uns32			mGeomEditIndex;

		CQ3ObjectRef		mLightObject;
		TQ3RationalPoint4D	mLocalLightPosition;	// w is 0 or 1

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

		void				InitProcPtrs();
		
		void				DeleteVBO( ShadowVBO* inCachedVBO );
		void				DeleteFromLightToGeomMap( ShadowVBO* inCachedVBO );
		void				DeleteFromGeomToLightMap( ShadowVBO* inCachedVBO );
		ShadowVBO*			FindVBO( TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos );
		void				RenderVBO( ShadowVBO* inCachedVBO );
		void				AddVBO( ShadowVBO* inVBO );
		void				FlushUnreferencedGeometries();
		void				FlushUnreferencedLights();
		void				MakeRoom( TQ3Uns32 inBytesNeeded );
		void				PurgeDownToSize( long long inTargetSize );
		void				SetMaxBufferSize( long long inBufferSize );
	
		GenBuffersARBProcPtr		glGenBuffersARBProc;
		BindBufferARBProcPtr		glBindBufferARBProc;
		DeleteBuffersARBProcPtr		glDeleteBuffersARBProc;
		IsBufferARBProcPtr			glIsBufferARBProc;
		BufferDataARBProcPtr		glBufferDataARBProc;
		BufferSubDataARBProcPtr		glBufferSubDataARBProc;

	private:
		GeomToLightToShadow			mGeomToLightToShadow;
		LightToGeomToShadow			mLightToGeomToShadow;
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
	: mGeomObject( Q3Shared_GetReference( inGeom ) )
	, mGeomEditIndex( Q3Shared_GetEditIndex( inGeom ) )
	, mLightObject( Q3Shared_GetReference( inLight ) )
	, mLocalLightPosition( inLocalLightPos )
	, mBufferBytes( 0 )
	, mPrev( NULL )
	, mNext( NULL )
{
	// Leave other fields uninitialized for now
}

ShadowVBO::~ShadowVBO()
{
}

bool	ShadowVBO::IsStale( const TQ3RationalPoint4D& inLocalLightPos ) const
{
	bool stale = static_cast<E3Shared*>( mGeomObject.get() )->GetEditIndex() !=
		mGeomEditIndex;
	if (! stale)
	{
		TQ3RationalPoint4D posDelta =
		{
			inLocalLightPos.x - mLocalLightPosition.x,
			inLocalLightPos.y - mLocalLightPosition.y,
			inLocalLightPos.z - mLocalLightPosition.z,
			inLocalLightPos.w - mLocalLightPosition.w
		};
		float distSq = posDelta.x * posDelta.x + posDelta.y * posDelta.y +
			posDelta.z * posDelta.z + posDelta.w * posDelta.w;
		stale = (distSq > 7.0e-6f);
		if (stale)
		{
			Q3_MESSAGE_FMT( "Stale shadow due to light position error %f", distSq );
		}
	}
	else
	{
		Q3_MESSAGE_FMT( "Stale shadow due to geom edit index" );
	}
	return stale;
}

#pragma mark -

ShadowVolCache::ShadowVolCache()
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

ShadowVolCache::~ShadowVolCache()
{
	// Note:  This object will only be destroyed after the last OpenGL context
	// of its sharing group has gone away.  Hopefully, this will automatically
	// clear the VBOs from VRAM.  Hence, we do not need or want to call DeleteVBO,
	// but we do want to delete the ShadowVBO objects.
	for (GeomToLightToShadow::iterator i = mGeomToLightToShadow.begin();
		i != mGeomToLightToShadow.end(); ++i)
	{
		LightToShadow& lMap( i->second );
		for (LightToShadow::iterator j = lMap.begin(); j != lMap.end(); ++j)
		{
			delete j->second;
		}
	}
}

void		ShadowVolCache::InitProcPtrs()
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


void	ShadowVolCache::DeleteVBO( ShadowVBO* inCachedVBO )
{
	// Remove the buffers from VRAM.
	if ( inCachedVBO->mNumTriIndices + inCachedVBO->mNumQuadIndices > 0)
	{
		(*glDeleteBuffersARBProc)( 2, inCachedVBO->mGLBufferNames );
	}
	
	// Remove the record from the doubly-linked list.
	inCachedVBO->mPrev->mNext = inCachedVBO->mNext;
	inCachedVBO->mNext->mPrev = inCachedVBO->mPrev;
	
	mTotalBytes -= inCachedVBO->mBufferBytes;
	
	delete inCachedVBO;
}

void	ShadowVolCache::DeleteFromLightToGeomMap( ShadowVBO* inCachedVBO )
{
	LightToGeomToShadow::iterator foundLight = mLightToGeomToShadow.find(
		inCachedVBO->mLightObject.get() );
	if (foundLight != mLightToGeomToShadow.end())
	{
		GeomToShadow& geomToShadowMap( foundLight->second );
		GeomToShadow::iterator foundGeom = geomToShadowMap.find(
			inCachedVBO->mGeomObject.get() );
		if (foundGeom != geomToShadowMap.end())
		{
			geomToShadowMap.erase( foundGeom );
			
			if (geomToShadowMap.empty())
			{
				mLightToGeomToShadow.erase( foundLight );
			}
		}
	}
}

void	ShadowVolCache::DeleteFromGeomToLightMap( ShadowVBO* inCachedVBO )
{
	GeomToLightToShadow::iterator foundGeom = mGeomToLightToShadow.find(
		inCachedVBO->mGeomObject.get() );
	if (foundGeom != mGeomToLightToShadow.end())
	{
		LightToShadow& lightToShadowMap( foundGeom->second );
		LightToShadow::iterator foundLight = lightToShadowMap.find(
			inCachedVBO->mLightObject.get() );
		if (foundLight != lightToShadowMap.end())
		{
			lightToShadowMap.erase( foundLight );
			
			if (lightToShadowMap.empty())
			{
				mGeomToLightToShadow.erase( foundGeom );
			}
		}
	}
}

void	ShadowVolCache::AddVBO( ShadowVBO* inVBO )
{
	mGeomToLightToShadow[ inVBO->mGeomObject.get() ][ inVBO->mLightObject.get() ] =
		inVBO;
	
	mLightToGeomToShadow[ inVBO->mLightObject.get() ][ inVBO->mGeomObject.get() ] =
		inVBO;

	// Insert the new record at the new end of a doubly-linked list.
	inVBO->mNext = &mListNewEnd;
	inVBO->mPrev = mListNewEnd.mPrev;
	inVBO->mPrev->mNext = inVBO;
	mListNewEnd.mPrev = inVBO;
	
	mTotalBytes += inVBO->mBufferBytes;
	
	Q3_MESSAGE_FMT("Shadow cache %lld bytes", mTotalBytes );
}


ShadowVBO*	ShadowVolCache::FindVBO( TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos )
{
	ShadowVBO* cachedShadow = NULL;
	
	GeomToLightToShadow::iterator foundGeom = mGeomToLightToShadow.find( inGeom );
	
	if (foundGeom != mGeomToLightToShadow.end())
	{
		LightToShadow& lightToShadowMap( foundGeom->second );
		LightToShadow::iterator foundLight = lightToShadowMap.find( inLight );
		
		if (foundLight != lightToShadowMap.end())
		{
			cachedShadow = foundLight->second;
			
			if (cachedShadow->IsStale( inLocalLightPos ))
			{
				lightToShadowMap.erase( foundLight );
				if (lightToShadowMap.empty())
				{
					mGeomToLightToShadow.erase( foundGeom );
				}
				
				DeleteFromLightToGeomMap( cachedShadow );
				DeleteVBO( cachedShadow );
				cachedShadow = NULL;
			}
		}
	}
	
	return cachedShadow;
}

void	ShadowVolCache::RenderVBO( ShadowVBO* inCachedVBO )
{
	if ( inCachedVBO->mNumTriIndices + inCachedVBO->mNumQuadIndices > 0)
	{
		(*glBindBufferARBProc)( GL_ARRAY_BUFFER, inCachedVBO->mGLBufferNames[0] );
		glVertexPointer( 4, GL_FLOAT, 0, BufferObPtr( 0 ) );

		(*glBindBufferARBProc)( GL_ELEMENT_ARRAY_BUFFER,
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

		(*glBindBufferARBProc)( GL_ARRAY_BUFFER, 0 );
		(*glBindBufferARBProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
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

void	ShadowVolCache::FlushUnreferencedGeometries()
{
	ObVec	geomsToFlush;
	TQ3GeometryObject theGeom;
	
	for (GeomToLightToShadow::const_iterator geomIt = mGeomToLightToShadow.begin();
		geomIt != mGeomToLightToShadow.end(); ++geomIt)
	{
		theGeom = geomIt->first;
		TQ3Uns32 vboRefs = CountVBOs( theGeom );
		TQ3Uns32 shadowRefs = geomIt->second.size();
		TQ3Uns32 totalRefs = Q3Shared_GetReferenceCount( theGeom );
		if (vboRefs + shadowRefs == totalRefs)
		{
			const LightToShadow& ltos( geomIt->second );
			const ShadowVBO* aVBO = ltos.begin()->second;
			geomsToFlush.push_back( aVBO->mGeomObject );
		}
	}
	
	for (ObVec::const_iterator j = geomsToFlush.begin(); j != geomsToFlush.end(); ++j)
	{
		theGeom = j->get();
		
		GeomToLightToShadow::iterator foundGeom = mGeomToLightToShadow.find( theGeom );
		if (foundGeom != mGeomToLightToShadow.end())
		{
			LightToShadow& lightMap( foundGeom->second );
			for (LightToShadow::iterator k = lightMap.begin(); k != lightMap.end(); ++k)
			{
				DeleteFromLightToGeomMap( k->second );
				DeleteVBO( k->second );
			}
			mGeomToLightToShadow.erase( foundGeom );
		}
	}
}

void	ShadowVolCache::FlushUnreferencedLights()
{
	ObVec	lightsToFlush;
	TQ3LightObject theLight;
	
	for (LightToGeomToShadow::const_iterator lightIt = mLightToGeomToShadow.begin();
		lightIt != mLightToGeomToShadow.end(); ++lightIt)
	{
		theLight = lightIt->first;
		TQ3Uns32 shadowRefs = lightIt->second.size();
		TQ3Uns32 totalRefs = Q3Shared_GetReferenceCount( theLight );
		if (shadowRefs == totalRefs)
		{
			const GeomToShadow& gtos( lightIt->second );
			const ShadowVBO* aVBO = gtos.begin()->second;
			lightsToFlush.push_back( aVBO->mLightObject );
		}
	}
	
	for (ObVec::const_iterator j = lightsToFlush.begin(); j != lightsToFlush.end(); ++j)
	{
		theLight = j->get();
		
		GeomToLightToShadow::iterator foundLight = mLightToGeomToShadow.find( theLight );
		if (foundLight != mLightToGeomToShadow.end())
		{
			GeomToShadow& geomMap( foundLight->second );
			for (GeomToShadow::iterator k = geomMap.begin(); k != geomMap.end(); ++k)
			{
				DeleteFromGeomToLightMap( k->second );
				DeleteVBO( k->second );
			}
			mLightToGeomToShadow.erase( foundLight );
		}
	}
	
	if (not lightsToFlush.empty())
	{
		Q3_MESSAGE_FMT( "Flushed %d lights", (int)lightsToFlush.size() );
	}
}

void	ShadowVolCache::MakeRoom( TQ3Uns32 inBytesNeeded )
{
	if ( (inBytesNeeded < mMaxBufferBytes) &&
		(mTotalBytes + inBytesNeeded > mMaxBufferBytes) )
	{
		long long targetSize = mMaxBufferBytes - inBytesNeeded;
		
		PurgeDownToSize( targetSize );
	}
}

void	ShadowVolCache::PurgeDownToSize( long long inTargetSize )
{
	while (mTotalBytes > inTargetSize)
	{
		// Find the least recently used VBO from the doubly-linked list
		ShadowVBO* oldestVBO = mListOldEnd.mNext;
		
		// Remove it from the map indexes
		DeleteFromLightToGeomMap( oldestVBO );
		DeleteFromGeomToLightMap( oldestVBO );
		
		// Remove it from the doubly linked list, remove the VBOs, update
		// mTotalBytes, and delete the ShadowVBO record.
		DeleteVBO( oldestVBO );
	}
}

void	ShadowVolCache::SetMaxBufferSize( long long inBufferSize )
{
	if (inBufferSize < mMaxBufferBytes)
	{
		PurgeDownToSize( inBufferSize );
	}
	
	mMaxBufferBytes = inBufferSize;
}

#pragma mark -

/*!
	@function	StartFrame
	@abstract	Update the limit on memory that can be used in this cache.
	@param			glContext		An OpenGL context.
	@param			memLimitK		New memory limit in K-bytes.
*/
void	ShadowVolMgr::StartFrame(	TQ3GLContext glContext,
									TQ3Uns32 memLimitK )
{
	ShadowVolCache*	theCache = GetShadowCache( glContext );
	
	if (theCache != NULL)
	{
		theCache->SetMaxBufferSize( memLimitK * 1024LL );
	}
}

/*!
	@function	RenderShadowVolume
	@abstract	Look for a cached shadow volume for a given geometry, OpenGL
				context, and shadow-casting light.  If we find one, render it.
	@discussion		If we find the object in the cache, but the cached object
					is stale, we delete it from the cache and return false.
	@param			glContext		An OpenGL context.
	@param			inGeom			A geometry object.
	@param			inLight			A light object.
	@param			inLocalLightPos	The position of the light in local coordinates.
	@result			True if the object was found and rendered.
*/
TQ3Boolean		ShadowVolMgr::RenderShadowVolume(
									TQ3GLContext glContext,
									TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos )
{
	TQ3Boolean	didRender = kQ3False;
	ShadowVolCache*	theCache = GetShadowCache( glContext );
	
	if (theCache != NULL)
	{
		ShadowVBO* theVBO = theCache->FindVBO( inGeom, inLight, inLocalLightPos );
		
		if (theVBO != NULL)
		{
			theCache->RenderVBO( theVBO );
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
			theCache->MakeRoom( newVBO->mBufferBytes );

			// Get buffer names
			(*theCache->glGenBuffersARBProc)( 2, newVBO->mGLBufferNames );
			
			// Copy point data into array buffer.
			(*theCache->glBindBufferARBProc)( GL_ARRAY_BUFFER,
				newVBO->mGLBufferNames[0] );
			(*theCache->glBufferDataARBProc)( GL_ARRAY_BUFFER,
				inNumPoints * sizeof(TQ3RationalPoint4D),
				inPoints, GL_STATIC_DRAW );
			(*theCache->glBindBufferARBProc)( GL_ARRAY_BUFFER, 0 );
			
			// Index data into elements buffer.
			(*theCache->glBindBufferARBProc)( GL_ELEMENT_ARRAY_BUFFER,
				newVBO->mGLBufferNames[1] );
			(*theCache->glBufferDataARBProc)( GL_ELEMENT_ARRAY_BUFFER,
				(inNumTriIndices + inNumQuadIndices) * sizeof(GLuint),
				inVertIndices, GL_STATIC_DRAW );
			(*theCache->glBindBufferARBProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
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
	@param			inRenderer		A Quesa renderer.
*/
void	ShadowVolMgr::Flush(
									TQ3GLContext glContext,
									TQ3RendererObject inRenderer )
{
	ShadowVolCache*	theCache = GetShadowCache( glContext );
	
	if (theCache != NULL)
	{
		theCache->FlushUnreferencedGeometries();
		theCache->FlushUnreferencedLights();
	}
}

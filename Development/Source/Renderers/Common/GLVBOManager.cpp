/*  NAME:
        GLVBOManager.c

    DESCRIPTION:
        Quesa OpenGL vertex buffer object caching.
       
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

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "GLVBOManager.h"
#include "GLGPUSharing.h"
#include "CQ3ObjectRef.h"
#include "GLUtils.h"

#include <vector>
#include <algorithm>


//=============================================================================
//		Internal constants
//-----------------------------------------------------------------------------
namespace
{
	const TQ3Uns32	kVBOCacheKey	= Q3_FOUR_CHARACTER_CONSTANT('v', 'b', 'o', 'k');
	
	const TQ3Uns32	kAbsentBuffer	= 0xFFFFFFFFUL;
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

typedef long GLintptrARB;
typedef long GLsizeiptrARB;

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
	struct CachedVBO
	{
						CachedVBO() {}
						CachedVBO( TQ3GeometryObject inGeom );
						CachedVBO( const CachedVBO& inOther );
						~CachedVBO() {}
		
		CachedVBO&		operator=( const CachedVBO& inOther );
						
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
	};
	
	typedef	std::vector< CachedVBO >	CachedVBOVec;

	class VBOCache : public CQ3GPSharedCache
	{
	public:
						VBOCache();
		
		void			InitProcPtrs();
		CachedVBO*		FindVBO( TQ3GeometryObject inGeom );
		void			DeleteVBO( CachedVBO* inVBO );
		void			RenderVBO( const CachedVBO* inCachedVBO );
		void			AddVBO( const CachedVBO& inVBO );
		void			Flush();
		void			ForgetTriangleStrips();

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
	};

	// Comparator for ordering cached VBOs by their geometries
	struct GeomLess
	{
		bool	operator()( const CachedVBO& inOne, const CachedVBO& inTwo ) const
				{
					return inOne.mGeomObject.get() < inTwo.mGeomObject.get();
				}
	};
	
	// Predicate for use with std::partition and CachedVBOVec.
	struct IsReferenced
	{
		bool			operator()( const CachedVBO& inCachedVBO ) const
								{
									return Q3Shared_IsReferenced(
										inCachedVBO.mGeomObject.get() ) ==
										kQ3True;
								}
	};
	
	// Predicate for use with std::partition and CachedVBOVec.
	struct IsNotStrip
	{
		bool			operator()( const CachedVBO& inCachedVBO ) const
								{
									return inCachedVBO.mGLMode !=
										GL_TRIANGLE_STRIP;
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

bool	CachedVBO::IsStale() const
{
	return Q3Shared_GetEditIndex( mGeomObject.get() ) != mEditIndex;
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

CachedVBO::CachedVBO( TQ3GeometryObject inGeom )
	: mGeomObject( Q3Shared_GetReference( inGeom ) )
	, mEditIndex( Q3Shared_GetEditIndex( inGeom ) )
{
	// Leave other fields uninitialized for now
}

CachedVBO::CachedVBO( const CachedVBO& inOther )
	: mGeomObject( inOther.mGeomObject )
	, mEditIndex( inOther.mEditIndex )
	, mGLMode( inOther.mGLMode )
	, mNumIndices( inOther.mNumIndices )
	, mVertexBufferOffset( inOther.mVertexBufferOffset )
	, mNormalBufferOffset( inOther.mNormalBufferOffset )
	, mColorBufferOffset( inOther.mColorBufferOffset )
	, mTextureUVBufferOffset( inOther.mTextureUVBufferOffset )
{
	mGLBufferNames[0] = inOther.mGLBufferNames[0];
	mGLBufferNames[1] = inOther.mGLBufferNames[1];
}

CachedVBO&		CachedVBO::operator=( const CachedVBO& inOther )
{
	mGeomObject = inOther.mGeomObject;
	mEditIndex = inOther.mEditIndex;
	mGLMode = inOther.mGLMode;
	mNumIndices = inOther.mNumIndices;
	
	mGLBufferNames[0] = inOther.mGLBufferNames[0];
	mGLBufferNames[1] = inOther.mGLBufferNames[1];
	
	mVertexBufferOffset = inOther.mVertexBufferOffset;
	mNormalBufferOffset = inOther.mNormalBufferOffset;
	mColorBufferOffset = inOther.mColorBufferOffset;
	mTextureUVBufferOffset = inOther.mTextureUVBufferOffset;
	
	return *this;
}


VBOCache::VBOCache()
	: glGenBuffersARBProc( NULL )
	, glBindBufferARBProc( NULL )
	, glDeleteBuffersARBProc( NULL )
	, glIsBufferARBProc( NULL )
	, glBufferDataARBProc( NULL )
	, glBufferSubDataARBProc( NULL )
{
	InitProcPtrs();
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


CachedVBO*		VBOCache::FindVBO( TQ3GeometryObject inGeom )
{
	CachedVBO*	theCachedVBO = NULL;
	
	CachedVBO	searchDummy( inGeom );
	
	CachedVBOVec::iterator	foundIt = std::lower_bound( mCachedVBOs.begin(),
		mCachedVBOs.end(), searchDummy, GeomLess() );
	
	if ( (foundIt != mCachedVBOs.end()) &&
		(foundIt->mGeomObject.get() == inGeom) )
	{
		theCachedVBO = &*foundIt;
	}
	
	return theCachedVBO;
}

void	VBOCache::DeleteVBO( CachedVBO* inVBO )
{
	(*glDeleteBuffersARBProc)( 2, inVBO->mGLBufferNames );

	TQ3Uns32	arrayIndex = inVBO - &mCachedVBOs[0];
	mCachedVBOs.erase( mCachedVBOs.begin() + arrayIndex );
}

void	VBOCache::AddVBO( const CachedVBO& inVBO )
{
	CachedVBOVec::iterator	placeIt = std::lower_bound( mCachedVBOs.begin(),
		mCachedVBOs.end(), inVBO, GeomLess() );

	mCachedVBOs.insert( placeIt, inVBO );
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


void	VBOCache::ForgetTriangleStrips()
{
	// Move VBOs holding triangle strips to end of list
	CachedVBOVec::iterator startStrips = std::stable_partition(
		mCachedVBOs.begin(), mCachedVBOs.end(), IsNotStrip() );
	
	// Delete the buffers for the VBO records that are going away
	for (CachedVBOVec::iterator i = startStrips; i != mCachedVBOs.end(); ++i)
	{
		(*glDeleteBuffersARBProc)( 2, i->mGLBufferNames );
	}
	
	mCachedVBOs.erase( startStrips, mCachedVBOs.end() );
}


void	VBOCache::Flush()
{
	// Move unreferenced VBOs to end of list
	CachedVBOVec::iterator startUnused = std::stable_partition(
		mCachedVBOs.begin(), mCachedVBOs.end(), IsReferenced() );
	
	// Delete the buffers for the VBO records that are going away
	for (CachedVBOVec::iterator i = startUnused; i != mCachedVBOs.end(); ++i)
	{
		(*glDeleteBuffersARBProc)( 2, i->mGLBufferNames );
	}
	
	mCachedVBOs.erase( startUnused, mCachedVBOs.end() );
}

#pragma mark -
//=============================================================================
//		Public functions
//-----------------------------------------------------------------------------


/*!
	@function		RenderCachedVBO
	@abstract		Look for a cached VBO for the given geometry and OpenGL
					context.  If we find one, render it.
	@discussion		If we find the object in the cache, but the cached object
					is stale, we delete it from the cache and return false.
	@param			glContext		An OpenGL context.
	@param			inGeom			A geometry object.
	@result			True if the object was found and rendered.
*/
TQ3Boolean			RenderCachedVBO(
									TQ3GLContext glContext,
									TQ3GeometryObject inGeom )
{
	TQ3Boolean	didRender = kQ3False;
	VBOCache*	theCache = GetVBOCache( glContext );
	
	if (theCache != NULL)
	{
		CachedVBO*	theVBO = theCache->FindVBO( inGeom );
		
		if (theVBO != NULL)
		{
			if ( theVBO->IsStale() )
			{
				theCache->DeleteVBO( theVBO );
			}
			else
			{
				theCache->RenderVBO( theVBO );
				didRender = kQ3True;
			}
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
		CachedVBO	newVBO( inGeom );
		newVBO.mGLMode = inMode;
		newVBO.mNumIndices = inNumIndices;
		(*theCache->glGenBuffersARBProc)( 2, newVBO.mGLBufferNames );
		
		TQ3Uns32	vertexDataSize = inNumPoints * sizeof(TQ3Point3D);
		TQ3Uns32	normalDataSize = (inNormals == NULL)? 0 :
			inNumPoints * sizeof(TQ3Vector3D);
		TQ3Uns32	colorDataSize = (inColors == NULL)? 0 :
			inNumPoints * sizeof(TQ3ColorRGB);
		TQ3Uns32	uvDataSize = (inUVs == NULL)? 0 :
			inNumPoints * sizeof(TQ3Param2D);
		TQ3Uns32	totalDataSize = vertexDataSize + normalDataSize +
			colorDataSize + uvDataSize;
		
		newVBO.mVertexBufferOffset = 0;
		newVBO.mNormalBufferOffset = (inNormals == NULL)? kAbsentBuffer :
			vertexDataSize;
		newVBO.mColorBufferOffset = (inColors == NULL)? kAbsentBuffer :
			vertexDataSize + normalDataSize;
		newVBO.mTextureUVBufferOffset = (inUVs == NULL)? kAbsentBuffer :
			vertexDataSize + normalDataSize + colorDataSize;
		
		theCache->AddVBO( newVBO );
		
		const GLvoid *	dataAddr;
		
		// First define an uninitialized buffer of the right size
		(*theCache->glBindBufferARBProc)( GL_ARRAY_BUFFER,
			newVBO.mGLBufferNames[0] );
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
				newVBO.mNormalBufferOffset, normalDataSize, dataAddr );
		}
		if (inColors != NULL)
		{
			dataAddr = inColors;
			(*theCache->glBufferSubDataARBProc)( GL_ARRAY_BUFFER,
				newVBO.mColorBufferOffset, colorDataSize, dataAddr );
		}
		if (inUVs != NULL)
		{
			dataAddr = inUVs;
			(*theCache->glBufferSubDataARBProc)( GL_ARRAY_BUFFER,
				newVBO.mTextureUVBufferOffset, uvDataSize, dataAddr );
		}
		(*theCache->glBindBufferARBProc)( GL_ARRAY_BUFFER, 0 );
		
		// Now for the index data.
		(*theCache->glBindBufferARBProc)( GL_ELEMENT_ARRAY_BUFFER,
			newVBO.mGLBufferNames[1] );
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
		theCache->Flush();
	}
}

/*!
	@function		ForgetTriangleStripVBOs
	@abstract		Delete any cached VBOs for triangle strips.
	@param			glContext		An OpenGL context.
*/
void				ForgetTriangleStripVBOs(
									TQ3GLContext glContext )
{
	VBOCache*	theCache = GetVBOCache( glContext );
	
	if (theCache != NULL)
	{
		theCache->ForgetTriangleStrips();
	}
}

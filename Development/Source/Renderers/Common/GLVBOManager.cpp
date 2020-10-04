/*  NAME:
        GLVBOManager.cpp

    DESCRIPTION:
        Quesa OpenGL vertex buffer object caching.
       
    COPYRIGHT:
        Copyright (c) 2007-2020, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#include "CQ3WeakObjectRef.h"
#include "GLUtils.h"
#include "E3Main.h"
#include "QORenderer.h"

#include <vector>
#include <algorithm>
#include <cstddef>

// In lieu of glext.h
#ifndef GL_ARB_multitexture
	#define GL_TEXTURE0_ARB                   0x84C0
#endif

#ifndef GL_BUFFER_SIZE
#define GL_BUFFER_SIZE                                 0x8764
#endif

#if Q3_DEBUG
	#define Q3_DEBUG_VBO	0
#endif

#if Q3_DEBUG_VBO
	#include <map>
#endif

static int sVBOCount = 0;

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

#if Q3_DEBUG
	//#define VALIDATE_COUNTS		1
#endif

#if VALIDATE_COUNTS
	#define VALIDATE_COUNT(cache)	cache->ValidateVBOCount()
#else
	#define VALIDATE_COUNT(cache)
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
#pragma mark struct CachedVBO
	struct CachedVBO
	{
						CachedVBO(); // only for linked list sentinels
						CachedVBO( TQ3GeometryObject inGeom, GLenum inMode );
						~CachedVBO() {}
		
						
		bool			IsStale() const;
	
		CQ3WeakObjectRef	mGeomObject;
		TQ3Object		mSortKey;
		TQ3Uns32		mEditIndex;
		GLenum			mGLMode;			// e.g., GL_TRIANGLES
		TQ3Uns32		mNumIndices;
	#if Q3_DEBUG
		TQ3Uns32		mNumPoints;
	#endif
		GLuint			mGLBufferNames[2];	// 0 is array, 1 is index
		TQ3Uns32		mVertexBufferOffset;
		TQ3Uns32		mNormalBufferOffset;
		TQ3Uns32		mColorBufferOffset;
		TQ3Uns32		mTextureUVBufferOffset;
		TQ3Uns32		mLayerShiftBufferOffset;
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
		
		CachedVBO*		FindVBO( TQ3GeometryObject inGeom, GLenum inMode, const QORenderer::GLFuncs& inFuncs );
		void			RenderVBO( const QORenderer::Renderer& inRenderer, const CachedVBO* inCachedVBO );
		void			AddVBO( CachedVBO* inVBO );
		void			FlushUnreferenced( const QORenderer::GLFuncs& inFuncs );
		void			DeleteVBO( CachedVBO* inCachedVBO, const QORenderer::GLFuncs& inFuncs );
		void			DeleteCachedVBOs( CachedVBOVec& inVBOs, const QORenderer::GLFuncs& inFuncs );
		void			PurgeDownToSize( long long inTargetSize, const QORenderer::GLFuncs& inFuncs );
		void			SetMaxBufferSize( long long inBufferSize, const QORenderer::GLFuncs& inFuncs );
		void			MakeRoom( TQ3Uns32 inBytesNeeded, const QORenderer::GLFuncs& inFuncs );
		
		void			AddToUsageList( CachedVBO* ioVBO );
		void			DeleteFromUsageList( CachedVBO* ioVBO );
		void			RenewInUsageList( CachedVBO* ioVBO );

	private:
		CachedVBOVec*	GetVBOVecForMode( GLenum inMode );
		CachedVBO*		FindVBOInVec( TQ3GeometryObject inGeom, CachedVBOVec& inVBOs );
		void			FlushUnreferencedInVec( CachedVBOVec& ioVBOs, const QORenderer::GLFuncs& inFuncs );

	#if VALIDATE_COUNTS
		void			ValidateVBOCount() const;
	#endif
	
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
					return (inOne->mSortKey < inTwo->mSortKey);
				}
	};
	
	// Predicate for use with std::partition and CachedVBOVec.
	struct IsReferenced
	{
		bool			operator()( const CachedVBO* inCachedVBO ) const
								{
									return inCachedVBO->mGeomObject.isvalid();
								}
	};
	
#if 0//Q3_DEBUG
	struct VBODebugRec
	{
		TQ3Uns32			byteCount;
		TQ3GeometryObject	geom;
		int					kind;
		
							VBODebugRec()
								: byteCount( 0 )
								, geom( nullptr )
								, kind( -1 )
								{}
							VBODebugRec( TQ3Uns32 inBytes,
										TQ3GeometryObject inGeom,
										int inKind )
								: byteCount( inBytes )
								, geom( inGeom )
								, kind( inKind )
								{}
							VBODebugRec( const VBODebugRec& inOther )
								: byteCount( inOther.byteCount )
								, geom( inOther.geom )
								, kind( inOther.kind )
								{
								}
	};
#endif
}

#if 0//Q3_DEBUG
typedef std::map< GLuint, VBODebugRec >					VBOMap;

static VBOMap* sVBORecords = nullptr;

/*!
	@function		RecordVBO
	@abstract		For debugging, record a VBO allocation.
	@param			inBufferName		The ID number of the buffer.
	@param			inGeom				The geometry owning the VBO.
	@param			inBytes				Size of the buffer in bytes.
	@param			inKind				Normal = 0, normal secondary = 1,
										shadow = 2, shadow secondary = 3
*/
void				RecordVBO(
									GLuint inBufferName,
									TQ3GeometryObject inGeom,
									TQ3Uns32 inBytes,
									int inKind )
{
	if (sVBORecords == nullptr)
	{
		sVBORecords = new VBOMap;
	}
	
	VBOMap::iterator foundIt = sVBORecords->find( inBufferName );
	Q3_ASSERT_FMT( foundIt == sVBORecords->end(), "VBO buffer name again??" );
	(*sVBORecords)[ inBufferName ] = VBODebugRec( inBytes, inGeom, inKind );
}

/*!
	@function		ForgetVBO
	@abstract		Remove a VBO previously passed to RecordVBO.
	@param			inBufferName		The ID number of the buffer.
*/
void				ForgetVBO(		GLuint inBufferName )
{
	Q3_ASSERT_FMT( sVBORecords != nullptr, "Debug log of VBOs was not started");
	VBOMap::iterator foundIt = sVBORecords->find( inBufferName );
	Q3_ASSERT_FMT( foundIt != sVBORecords->end(), "VBO buffer being forgotten when not known??" );
	sVBORecords->erase( foundIt );
}

/*!
	@function		DumpVBOs
	@abstract		List the contents of the VBO recording to the debug
					log.
*/
void				DumpVBOs( void )
{
	Q3_ASSERT_FMT( sVBORecords != nullptr, "Debug log of VBOs was not started");
	Q3_MESSAGE_FMT("==== Dump of VBO log ====");
	for (auto vboPair: *sVBORecords)
	{
		Q3_MESSAGE_FMT("ID %d, size %lu, geom %p, kind %d", (int) i->first,
			(unsigned long) vboPair.second.byteCount, vboPair.second.geom, vboPair.second.kind );
	}
	Q3_MESSAGE_FMT("==== End VBO log ====");
}
#else
	#define RecordVBO(...)
	#define ForgetVBO(...)
	#define DumpVBOs(...)
#endif

#if Q3_DEBUG_VBO
	typedef std::map< const CachedVBO*, bool >	GoodVBOMap;
	static GoodVBOMap*	sGoodVBO = nullptr;

	static void ALLOCATED_VBO( const CachedVBO* inNewVBO )
	{
		if (sGoodVBO == nullptr)
		{
			sGoodVBO = new GoodVBOMap;
		}
		GoodVBOMap::iterator foundIt = sGoodVBO->find( inNewVBO );
		Q3_ASSERT_FMT( (foundIt == sGoodVBO->end()) or (not foundIt->second),
			"Supposedly new VBO already known and valid!" );
		(*sGoodVBO)[ inNewVBO ] = true;
	}

	static void FREED_VBO( const CachedVBO* inOldVBO )
	{
		if (sGoodVBO == nullptr)
		{
			sGoodVBO = new GoodVBOMap;
		}
		GoodVBOMap::iterator foundIt = sGoodVBO->find( inOldVBO );
		Q3_ASSERT_FMT( foundIt != sGoodVBO->end(), "VBO being freed is not known!" );
		Q3_ASSERT_FMT( foundIt->second, "VBO being freed a second time!");
		(*sGoodVBO)[ inOldVBO ] = false;
	}

	static void CHECK_VBO( const CachedVBO* inOldVBO )
	{
		if (sGoodVBO == nullptr)
		{
			sGoodVBO = new GoodVBOMap;
		}
		GoodVBOMap::iterator foundIt = sGoodVBO->find( inOldVBO );
		Q3_ASSERT_FMT( foundIt != sGoodVBO->end(), "VBO being accessed is not known!" );
		Q3_ASSERT_FMT( foundIt->second, "VBO being accessed after being freed!");
	}

	static void CHECK_VBO_OR_NULL( const CachedVBO* inOldVBO )
	{
		if (inOldVBO != nullptr)
		{
			CHECK_VBO( inOldVBO );
		}
	}
#else
	#define		ALLOCATED_VBO( x )
	#define		FREED_VBO( x )
	#define		CHECK_VBO( x )
	#define		CHECK_VBO_OR_NULL( x )
#endif

//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------


static VBOCache* GetVBOCache( TQ3GLContext glContext )
{
	VBOCache*	theCache = static_cast<VBOCache*>( GLGPUSharing_GetCache(
		glContext, kVBOCacheKey ) );
	
	if (theCache == nullptr)
	{
		theCache = new(std::nothrow) VBOCache;
		
		if (theCache != nullptr)
		{
			//Q3_MESSAGE_FMT("Created VBOCache %p for GL context %p", theCache, glContext);
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
			
			CHECK_VBO( vboArray[ mid ] );
			if (vboArray[ mid ]->mSortKey < inGeom)
			{
				lowIndex = mid + 1;
			}
			else
			{
				highIndex = mid;
			}
		}
		
		if ( (lowIndex == highIndex) &&
			(vboArray[ lowIndex ]->mGeomObject.get() == inGeom) )
		{
			CHECK_VBO( vboArray[ lowIndex ] );
			foundIt = inVBOs.begin() + lowIndex;
		}
	}
	
	return foundIt;
}


#pragma mark -

CachedVBO::CachedVBO( TQ3GeometryObject inGeom, GLenum inMode )
	: mGeomObject( inGeom )
	, mSortKey( inGeom )
	, mEditIndex( Q3Shared_GetEditIndex( inGeom ) )
	, mGLMode( inMode )
	, mBufferBytes( 0 )
	, mPrev( nullptr )
	, mNext( nullptr )
{
	// Leave other fields uninitialized for now
}

CachedVBO::CachedVBO()
	: mGeomObject()
	, mSortKey( nullptr )
	, mBufferBytes( 0 )
	, mPrev( nullptr )
	, mNext( nullptr )
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
	Q3_MESSAGE_FMT("Deleting VBOCache %p", this );
	QORenderer::GLFuncs funcs;
	funcs.InitializeForBufferDelete();
	DeleteCachedVBOs( mCachedVBOs_strips, funcs );
	DeleteCachedVBOs( mCachedVBOs_triangles, funcs );
	DeleteCachedVBOs( mCachedVBOs_lines, funcs );
}


CachedVBO*		VBOCache::FindVBOInVec( TQ3GeometryObject inGeom, CachedVBOVec& inVBOs )
{
	CachedVBO*	theCachedVBO = nullptr;
	
	CachedVBOVec::iterator foundIt = FindVBOByGeom( inVBOs, inGeom );
	
	if ( foundIt != inVBOs.end() )
	{
		theCachedVBO = *foundIt;
		CHECK_VBO( theCachedVBO );
		
		// Note: We cannot delete stale VBOs here, because this function may
		// be called by CountVBOs at a time when the OpenGL context that is
		// in effect does not go with this cache.
	}
	
	return theCachedVBO;
}


#if VALIDATE_COUNTS
void	VBOCache::ValidateVBOCount() const
{
	// Check for corrupt objects
	(void) E3Memory_CountRecords();

	size_t listCount = 0;
	CachedVBO* listPtr;
	for (listPtr = mListOldEnd.mNext; listPtr != &mListNewEnd; listPtr = listPtr->mNext)
	{
		++listCount;
	}
	
	size_t vecCount = mCachedVBOs_strips.size() + mCachedVBOs_triangles.size() +
		mCachedVBOs_lines.size();
	
	Q3_ASSERT_FMT( vecCount == listCount, "VBOCache list %d, vec %d",
		(int)listCount, (int)vecCount );
}
#endif


CachedVBOVec*	VBOCache::GetVBOVecForMode( GLenum inMode )
{
	CachedVBOVec* whichVec = nullptr;
	
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
									const QORenderer::GLFuncs& inFuncs )
{
	VALIDATE_COUNT( this );
	CachedVBO*	theCachedVBO = nullptr;
	
	CachedVBOVec* whichVec = GetVBOVecForMode( inMode );

	if (whichVec != nullptr)
	{
		theCachedVBO = FindVBOInVec( inGeom, *whichVec );
		CHECK_VBO_OR_NULL( theCachedVBO );
		
		if ( (theCachedVBO != nullptr) && theCachedVBO->IsStale() )
		{
			CachedVBOVec::iterator foundIt = FindVBOByGeom( *whichVec, inGeom );
			whichVec->erase( foundIt );
			
			DeleteVBO( theCachedVBO, inFuncs );
			
			theCachedVBO = nullptr;
		}
	}
	VALIDATE_COUNT( this );
	
	return theCachedVBO;
}

void	VBOCache::AddToUsageList( CachedVBO* ioVBO )
{
	CHECK_VBO( ioVBO );
	// Insert the new record at the "new" end of a doubly-linked list.
	ioVBO->mNext = &mListNewEnd;
	ioVBO->mPrev = mListNewEnd.mPrev;
	ioVBO->mPrev->mNext = ioVBO;
	mListNewEnd.mPrev = ioVBO;
}

void	VBOCache::RenewInUsageList( CachedVBO* ioVBO )
{
	CHECK_VBO( ioVBO );
	// Move the item to the "new" end of the list, so that it will not be
	// purged soon.
	DeleteFromUsageList( ioVBO );
	AddToUsageList( ioVBO );
	VALIDATE_COUNT( this );
}

void	VBOCache::AddVBO( CachedVBO* inVBO )
{
	CHECK_VBO( inVBO );
	CachedVBOVec* whichVec = GetVBOVecForMode( inVBO->mGLMode );
	
	if (whichVec != nullptr)
	{
		CachedVBOVec::iterator	placeIt = std::lower_bound( whichVec->begin(),
			whichVec->end(), inVBO, VBOLess() );

		whichVec->insert( placeIt, inVBO );
		
		AddToUsageList( inVBO );

		mTotalBytes += inVBO->mBufferBytes;
		
		//Q3_MESSAGE_FMT("VBO cache size after add: %lld", mTotalBytes );
	}
}


void VBOCache::RenderVBO( const QORenderer::Renderer& inRenderer, const CachedVBO* inCachedVBO )
{
	CHECK_VBO( inCachedVBO );
	Q3_ASSERT( (*inRenderer.Funcs().glIsBufferProc)( inCachedVBO->mGLBufferNames[0] ) );
	Q3_ASSERT( (*inRenderer.Funcs().glIsBufferProc)( inCachedVBO->mGLBufferNames[1] ) );

	(*inRenderer.Funcs().glBindBufferProc)( GL_ARRAY_BUFFER, inCachedVBO->mGLBufferNames[0] );

#if Q3_DEBUG
	GLint elementDataSize, mainDataSize, tcSumSize = 0;
	(*inRenderer.Funcs().glGetBufferParameterivProc)( GL_ARRAY_BUFFER, GL_BUFFER_SIZE,
		&mainDataSize );
#endif
	
	inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mVertexAttribLoc,
		3, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( inCachedVBO->mVertexBufferOffset ) );
	
	if (inCachedVBO->mNormalBufferOffset != kAbsentBuffer)
	{
		inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mNormalAttribLoc,
			3, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( inCachedVBO->mNormalBufferOffset ) );
	}
	
	if (inCachedVBO->mTextureUVBufferOffset != kAbsentBuffer)
	{
		inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mTexCoordAttribLoc,
			2, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( inCachedVBO->mTextureUVBufferOffset ) );
	}
	
	if (inCachedVBO->mColorBufferOffset != kAbsentBuffer)
	{
		inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mColorAttribLoc,
			3, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( inCachedVBO->mColorBufferOffset ) );
	}
	
	if (inCachedVBO->mLayerShiftBufferOffset != kAbsentBuffer)
	{
		inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mLayerShiftAttribLoc,
			1, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( inCachedVBO->mLayerShiftBufferOffset ) );
	}
	
	(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER,
		inCachedVBO->mGLBufferNames[1] );
	CHECK_GL_ERROR;

#if Q3_DEBUG
	(*inRenderer.Funcs().glGetBufferParameterivProc)( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE,
		&elementDataSize );
	CHECK_GL_ERROR;
	Q3_ASSERT( (TQ3Uns32)elementDataSize == inCachedVBO->mNumIndices * sizeof(TQ3Uns32) );
	Q3_ASSERT( (GLint)inCachedVBO->mBufferBytes == elementDataSize + mainDataSize + tcSumSize );
#endif

	//Q3_MESSAGE_FMT("glDrawElements in RenderVBO (count %d, mode %d)", (int)inCachedVBO->mNumIndices,
	//	(int)inCachedVBO->mGLMode );
	glDrawElements( inCachedVBO->mGLMode, inCachedVBO->mNumIndices,
		GL_UNSIGNED_INT, GLBufferObPtr( 0U ) );
	CHECK_GL_ERROR;
		
	(*inRenderer.Funcs().glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
	(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void	VBOCache::DeleteFromUsageList( CachedVBO* ioVBO )
{
	CHECK_VBO( ioVBO );
	ioVBO->mPrev->mNext = ioVBO->mNext;
	ioVBO->mNext->mPrev = ioVBO->mPrev;
}


void	VBOCache::DeleteVBO( CachedVBO* inCachedVBO, const QORenderer::GLFuncs& inFuncs )
{
	CHECK_VBO( inCachedVBO );
	Q3_ASSERT( (*inFuncs.glIsBufferProc)( inCachedVBO->mGLBufferNames[0] ) );
	Q3_ASSERT( (*inFuncs.glIsBufferProc)( inCachedVBO->mGLBufferNames[1] ) );
	(*inFuncs.glDeleteBuffersProc)( 2, inCachedVBO->mGLBufferNames );
	sVBOCount -= 2;
	ForgetVBO( inCachedVBO->mGLBufferNames[0] );
	ForgetVBO( inCachedVBO->mGLBufferNames[1] );
	//Q3_MESSAGE_FMT("Deleted VBOs %d, %d, now there are %d",
	//	inCachedVBO->mGLBufferNames[0], inCachedVBO->mGLBufferNames[1],
	//	sVBOCount );
	
	DeleteFromUsageList( inCachedVBO );
	
	mTotalBytes -= inCachedVBO->mBufferBytes;
	//Q3_MESSAGE_FMT("VBO cache size after delete: %lld", mTotalBytes );
	
	FREED_VBO( inCachedVBO );
	// It would be more logical to call FREED_VBO after delete,
	// but that causes the Clang static analyzer to issue a
	// "Use of memory after it is freed" warning.
	delete inCachedVBO;
}

void	VBOCache::DeleteCachedVBOs( CachedVBOVec& inVBOs, const QORenderer::GLFuncs& inFuncs )
{
	for (auto& vboPtr : inVBOs)
	{
		DeleteVBO( vboPtr, inFuncs );
	}
}

void	VBOCache::FlushUnreferencedInVec( CachedVBOVec& ioVBOs, const QORenderer::GLFuncs& inFuncs )
{
	// Move unreferenced VBOs to end of list
	auto startUnused = std::stable_partition(
		ioVBOs.begin(), ioVBOs.end(), IsReferenced() );
	
	if (startUnused != ioVBOs.end())
	{
		// Delete the buffers for the VBO records that are going away
		for (auto i = startUnused; i != ioVBOs.end(); ++i)
		{
			DeleteVBO( *i, inFuncs );
		}
		
		// Remove them from the array
		ioVBOs.erase( startUnused, ioVBOs.end() );
	}
}

void	VBOCache::FlushUnreferenced( const QORenderer::GLFuncs& inFuncs )
{
	VALIDATE_COUNT( this );
	FlushUnreferencedInVec( mCachedVBOs_strips, inFuncs );
	FlushUnreferencedInVec( mCachedVBOs_triangles, inFuncs );
	FlushUnreferencedInVec( mCachedVBOs_lines, inFuncs );
	VALIDATE_COUNT( this );
}

void	VBOCache::PurgeDownToSize( long long inTargetSize, const QORenderer::GLFuncs& inFuncs )
{
	while (mTotalBytes > inTargetSize)
	{
		// Find the least recently used VBO from the doubly-linked list
		CachedVBO* oldestVBO = mListOldEnd.mNext;
		CHECK_VBO( oldestVBO );
		
		// Remove it from the sorted vector
		CachedVBOVec* whichVec = GetVBOVecForMode( oldestVBO->mGLMode );
		
		CachedVBOVec::iterator	foundIt = std::lower_bound( whichVec->begin(),
			whichVec->end(), oldestVBO, VBOLess() );
		if ( (foundIt != whichVec->end()) &&
			((*foundIt)->mSortKey == oldestVBO->mSortKey) )
		{
			whichVec->erase( foundIt );
		}
		
		// Remove the VBO, remove the record from the doubly-linked list, and
		// dispose the record.
		DeleteVBO( oldestVBO, inFuncs );
	}
}

void	VBOCache::SetMaxBufferSize( long long inBufferSize, const QORenderer::GLFuncs& inFuncs )
{
	VALIDATE_COUNT( this );
	if (inBufferSize < mMaxBufferBytes)
	{
		PurgeDownToSize( inBufferSize, inFuncs );
	}
	
	mMaxBufferBytes = inBufferSize;
	VALIDATE_COUNT( this );
}

void	VBOCache::MakeRoom( TQ3Uns32 inBytesNeeded, const QORenderer::GLFuncs& inFuncs )
{
	if ( (inBytesNeeded < mMaxBufferBytes) &&
		(mTotalBytes + inBytesNeeded > mMaxBufferBytes) )
	{
		VALIDATE_COUNT( this );
		long long targetSize = mMaxBufferBytes - inBytesNeeded;
		
		PurgeDownToSize( targetSize, inFuncs );
		VALIDATE_COUNT( this );
	}
}


#pragma mark -
//=============================================================================
//		Public functions
//-----------------------------------------------------------------------------

/*!
	@function		UpdateVBOCacheLimit
	@abstract		Update the limit on memory that can be used in this cache.
	@param			inRenderer		An OpenGL renderer.
	@param			inMaxMemK		New memory limit in K-bytes.
*/
void				UpdateVBOCacheLimit(
									const QORenderer::Renderer& inRenderer,
									TQ3Uns32 inMaxMemK )
{
	VBOCache*	theCache = GetVBOCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		theCache->SetMaxBufferSize( inMaxMemK * 1024LL, inRenderer.Funcs() );
	}
}


/*!
	@function		RenderCachedVBO
	@abstract		Look for a cached VBO for the given geometry and OpenGL
					context.  If we find one, render it.
	@discussion		If we find the object in the cache, but the cached object
					is stale, we delete it from the cache and return false.
	@param			inRenderer		An OpenGL renderer.
	@param			inGeom			A geometry object.
	@param			inMode			OpenGL mode, e.g., GL_TRIANGLES.
	@result			True if the object was found and rendered.
*/
TQ3Boolean			RenderCachedVBO(
									const QORenderer::Renderer& inRenderer,
									TQ3GeometryObject inGeom,
									GLenum inMode )
{
	TQ3Boolean	didRender = kQ3False;
	VBOCache*	theCache = GetVBOCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		CachedVBO*	theVBO = theCache->FindVBO( inGeom, inMode, inRenderer.Funcs() );
		
		if ( (theVBO == nullptr) && (inMode == GL_TRIANGLE_STRIP) )
		{
			theVBO = theCache->FindVBO( inGeom, GL_TRIANGLES, inRenderer.Funcs() );
		}
		
		if (theVBO != nullptr)
		{
			CHECK_VBO( theVBO );
			theCache->RenderVBO( inRenderer, theVBO );
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
	@param			inRenderer		An OpenGL renderer.
	@param			inGeom			A geometry object.
	@param			inNumPoints		Number of points (vertices).
	@param			inPoints		Array of point locations.
	@param			inNormals		Array of normal vectors (or nullptr).
	@param			inColors		Array of vertex colors (or nullptr).
	@param			inUVs			Array of vertex UV coordinates (or nullptr).
	@param			inMode			OpenGL mode, e.g., GL_TRIANGLES.
	@param			inNumIndices	Number of vertex indices to follow.
	@param			inIndices		Array of vertex indices.
*/
void				AddVBOToCache(
									const QORenderer::Renderer& inRenderer,
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
	VBOCache*	theCache = GetVBOCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		CachedVBO*	newVBO = new CachedVBO( inGeom, inMode );
		ALLOCATED_VBO( newVBO );
		newVBO->mNumIndices = inNumIndices;
	#if Q3_DEBUG
		newVBO->mNumPoints = inNumPoints;
	#endif
		(*inRenderer.Funcs().glGenBuffersProc)( 2, newVBO->mGLBufferNames );
		sVBOCount += 2;
		//Q3_MESSAGE_FMT("Created VBOs %d, %d, now there are %d",
		//	newVBO->mGLBufferNames[0], newVBO->mGLBufferNames[1], sVBOCount );
		
		// Check for layer shift data
		TQ3Uns32 bufSize;
		TQ3LayerShifts* layerShifts = nullptr;
		std::vector<TQ3Uns8>	layerShiftBuffer;
		if ( (kQ3Success == Q3Object_GetProperty( inGeom,
			kQ3GeometryPropertyLayerShifts, 0, &bufSize, nullptr )) &&
			((bufSize % 4) == 0) &&
			bufSize == sizeof(TQ3LayerShifts) + inNumPoints * sizeof(float) )
		{
			layerShiftBuffer.resize( bufSize );
			Q3Object_GetProperty( inGeom, kQ3GeometryPropertyLayerShifts,
				bufSize, nullptr, &layerShiftBuffer[0] );
			layerShifts = reinterpret_cast<TQ3LayerShifts*>( &layerShiftBuffer[0] );
			if (layerShifts->numPoints != inNumPoints)
			{
				layerShifts = nullptr;	// bogus data, ignore it
			}
		}
		
		// Compute sizes of each kind of vertex data
		TQ3Uns32	vertexDataSize = static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3Point3D));
		TQ3Uns32	normalDataSize = (inNormals == nullptr)? 0 :
			static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3Vector3D));
		TQ3Uns32	colorDataSize = (inColors == nullptr)? 0 :
			static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3ColorRGB));
		TQ3Uns32	uvDataSize = (inUVs == nullptr)? 0 :
			static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3Param2D));
		TQ3Uns32	layerShiftDataSize = (layerShifts == nullptr)? 0 :
			static_cast<TQ3Uns32>(inNumPoints * sizeof(float));
		TQ3Uns32	totalDataSize = vertexDataSize + normalDataSize +
			colorDataSize + uvDataSize + layerShiftDataSize;
		
		newVBO->mBufferBytes = static_cast<TQ3Uns32>(inNumIndices * sizeof(TQ3Uns32) + totalDataSize);
		theCache->MakeRoom( newVBO->mBufferBytes, inRenderer.Funcs() );
			
		newVBO->mVertexBufferOffset = 0;
		newVBO->mNormalBufferOffset = (inNormals == nullptr)? kAbsentBuffer :
			vertexDataSize;
		newVBO->mColorBufferOffset = (inColors == nullptr)? kAbsentBuffer :
			vertexDataSize + normalDataSize;
		newVBO->mTextureUVBufferOffset = (inUVs == nullptr)? kAbsentBuffer :
			vertexDataSize + normalDataSize + colorDataSize;
		newVBO->mLayerShiftBufferOffset = (layerShifts == nullptr)? kAbsentBuffer :
			vertexDataSize + normalDataSize + colorDataSize + uvDataSize;
		
		theCache->AddVBO( newVBO );
		
		const GLvoid *	dataAddr;
		
		// First define an uninitialized buffer of the right size
		(*inRenderer.Funcs().glBindBufferProc)( GL_ARRAY_BUFFER,
			newVBO->mGLBufferNames[0] );
		(*inRenderer.Funcs().glBufferDataProc)( GL_ARRAY_BUFFER, totalDataSize,
			nullptr, GL_STATIC_DRAW );
		GLenum error = glGetError();
		if (error == GL_OUT_OF_MEMORY)
		{
			DumpVBOs();
			Q3_ASSERT_FMT( false, "Failed to allocate VBO memory!");
		}
		RecordVBO( newVBO->mGLBufferNames[0], inGeom, totalDataSize, 0 );
		
		// Then set sub-buffer data
		dataAddr = inPoints;
		(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER, 0,
			vertexDataSize, dataAddr );
		if (inNormals != nullptr)
		{
			dataAddr = inNormals;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER,
				newVBO->mNormalBufferOffset, normalDataSize, dataAddr );
		}
		if (inColors != nullptr)
		{
			dataAddr = inColors;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER,
				newVBO->mColorBufferOffset, colorDataSize, dataAddr );
		}
		if (inUVs != nullptr)
		{
			dataAddr = inUVs;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER,
				newVBO->mTextureUVBufferOffset, uvDataSize, dataAddr );
		}
		if (layerShifts != nullptr)
		{
			dataAddr = layerShifts->coords;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER,
				newVBO->mLayerShiftBufferOffset, layerShiftDataSize, dataAddr );
		}
		(*inRenderer.Funcs().glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
		
		// Now for the index data.
		(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER,
			newVBO->mGLBufferNames[1] );
		dataAddr = inIndices;
		(*inRenderer.Funcs().glBufferDataProc)( GL_ELEMENT_ARRAY_BUFFER,
			inNumIndices * sizeof(TQ3Uns32), dataAddr, GL_STATIC_DRAW );
		(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
		RecordVBO( newVBO->mGLBufferNames[1], inGeom, inNumIndices * sizeof(TQ3Uns32), 0 );
	}
}



/*!
	@function		FlushVBOCache
	@abstract		Delete any cached VBOs for geometries that are no longer
					referenced elsewhere.
	@param			inRenderer		An OpenGL renderer.
*/
void				FlushVBOCache(
									const QORenderer::Renderer& inRenderer )
{
	VBOCache*	theCache = GetVBOCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		theCache->FlushUnreferenced( inRenderer.Funcs() );
	}
}



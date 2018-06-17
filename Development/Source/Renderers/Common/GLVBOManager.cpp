/*  NAME:
        GLVBOManager.c

    DESCRIPTION:
        Quesa OpenGL vertex buffer object caching.
       
    COPYRIGHT:
        Copyright (c) 2007-2018, Quesa Developers. All rights reserved.

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
#include "CQ3WeakObjectRef.h"
#include "GLUtils.h"
#include "E3Main.h"

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
	#define Q3_DEBUG_VBO	1
#endif

#if Q3_DEBUG_VBO
	#include <map>
#endif

static GLenum sGLError = 0;

#if Q3_DEBUG_GL_ERRORS
	#define		CHECK_GL_ERROR	do {	\
									sGLError = glGetError();	\
									if (sGLError != GL_NO_ERROR)	\
									{	\
										char	xmsg[200];	\
										snprintf( xmsg, sizeof(xmsg),	\
											"glGetError() is 0x%04X", \
											(unsigned int)sGLError );	\
										E3Assert(__FILE__, __LINE__, xmsg);	\
									} \
								} while (false)
#else
	#define		CHECK_GL_ERROR
#endif

static int sVBOCount = 0;

//=============================================================================
//		Internal constants
//-----------------------------------------------------------------------------
namespace
{
	const TQ3Uns32	kVBOCacheKey	= Q3_FOUR_CHARACTER_CONSTANT('v', 'b', 'o', 'k');
	
	const TQ3Uns32	kAbsentBuffer	= 0xFFFFFFFFU;
	
	const TQ3Uns32	kMaxSecondaryTexCoords = 16;
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
typedef void (APIENTRY * ClientActiveTextureProcPtr)( GLenum unit );
typedef void (APIENTRY * MultiTexCoord1fProcPtr)( GLenum unit, GLfloat s );
typedef void (APIENTRY * GetBufferParameterivProcPtr)(GLenum target, GLenum value, GLint * data);

namespace
{
	struct SecondaryTexCoord
	{
		TQ3Uns32		mGLBufferName;
		GLint			mCoordsPerVertex;
		GLenum			mTextureUnit;
	};

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
		TQ3Uns32		mBufferBytes;
		TQ3Uns32		mNumSecondaryTexCoords;
		SecondaryTexCoord	mSecondaryTexCoord[kMaxSecondaryTexCoords];
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
		void			PurgeDownToSize( long long inTargetSize, const GLBufferFuncs& inFuncs );
		void			SetMaxBufferSize( long long inBufferSize, const GLBufferFuncs& inFuncs );
		void			MakeRoom( TQ3Uns32 inBytesNeeded, const GLBufferFuncs& inFuncs );
		void			AddBytes( TQ3Uns32 inBytes );
		
		void			AddToUsageList( CachedVBO* ioVBO );
		void			DeleteFromUsageList( CachedVBO* ioVBO );
		void			RenewInUsageList( CachedVBO* ioVBO );

	private:
		CachedVBOVec*	GetVBOVecForMode( GLenum inMode );
		CachedVBO*		FindVBOInVec( TQ3GeometryObject inGeom, CachedVBOVec& inVBOs );
		void			FlushUnreferencedInVec( CachedVBOVec& ioVBOs, const GLBufferFuncs& inFuncs );

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
	VBOMap::iterator endRecs = sVBORecords->end();
	for (VBOMap::iterator i = sVBORecords->begin(); i != endRecs; ++i)
	{
		Q3_MESSAGE_FMT("ID %d, size %lu, geom %p, kind %d", (int) i->first,
			(unsigned long) i->second.byteCount, i->second.geom, i->second.kind );
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

static inline GLvoid* BufferObPtr( GLuint offset )
{
	return (GLvoid*)( ((char*)nullptr) + offset );
}


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

GLBufferFuncs::GLBufferFuncs()
	: glGenBuffersProc( nullptr )
	, glBindBufferProc( nullptr )
	, glDeleteBuffersProc( nullptr )
	, glIsBufferProc( nullptr )
	, glBufferDataProc( nullptr )
	, glBufferSubDataProc( nullptr )
	, glClientActiveTextureProc( nullptr )
	, glMultiTexCoord1fProc( nullptr )
	, glGetBufferParameterivProc( nullptr )
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

		Q3_ASSERT( (glGenBuffersProc != nullptr) &&
			(glBindBufferProc != nullptr) &&
			(glDeleteBuffersProc != nullptr) &&
			(glIsBufferProc != nullptr) &&
			(glBufferDataProc != nullptr) &&
			(glBufferSubDataProc != nullptr) &&
			(glClientActiveTextureProc != nullptr) &&
			(glMultiTexCoord1fProc != nullptr) &&
			(glGetBufferParameterivProc != nullptr) );
	}
}

void	GLBufferFuncs::InitializeForDelete()
{
	GLGetProcAddress( glDeleteBuffersProc, "glDeleteBuffers", "glDeleteBuffersARB" );
	GLGetProcAddress( glIsBufferProc, "glIsBuffer", "glIsBufferARB" );
}

#pragma mark -

CachedVBO::CachedVBO( TQ3GeometryObject inGeom, GLenum inMode )
	: mGeomObject( inGeom )
	, mSortKey( inGeom )
	, mEditIndex( Q3Shared_GetEditIndex( inGeom ) )
	, mGLMode( inMode )
	, mBufferBytes( 0 )
	, mNumSecondaryTexCoords( 0 )
	, mPrev( nullptr )
	, mNext( nullptr )
{
	// Leave other fields uninitialized for now
}

CachedVBO::CachedVBO()
	: mGeomObject()
	, mSortKey( nullptr )
	, mBufferBytes( 0 )
	, mNumSecondaryTexCoords( 0 )
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
	GLBufferFuncs funcs;
	funcs.InitializeForDelete();
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
									const GLBufferFuncs& inFuncs )
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


void VBOCache::RenderVBO( const GLBufferFuncs& inFuncs, const CachedVBO* inCachedVBO )
{
	CHECK_VBO( inCachedVBO );
	Q3_ASSERT( (*inFuncs.glIsBufferProc)( inCachedVBO->mGLBufferNames[0] ) );
	Q3_ASSERT( (*inFuncs.glIsBufferProc)( inCachedVBO->mGLBufferNames[1] ) );

	(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, inCachedVBO->mGLBufferNames[0] );

#if Q3_DEBUG
	GLint elementDataSize, mainDataSize, tcSumSize = 0;
	(*inFuncs.glGetBufferParameterivProc)( GL_ARRAY_BUFFER, GL_BUFFER_SIZE,
		&mainDataSize );
#endif
	
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
	
	TQ3Uns32 i;
	if (inCachedVBO->mNumSecondaryTexCoords > 0)
	{
		for (i = 0; i < inCachedVBO->mNumSecondaryTexCoords; ++i)
		{
			Q3_ASSERT( (*inFuncs.glIsBufferProc)( inCachedVBO->mSecondaryTexCoord[i].mGLBufferName ) );
			(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, inCachedVBO->mSecondaryTexCoord[i].mGLBufferName );
		
		#if Q3_DEBUG
			GLint tcDataSize;
			(*inFuncs.glGetBufferParameterivProc)( GL_ARRAY_BUFFER, GL_BUFFER_SIZE,
				&tcDataSize );
			Q3_ASSERT( (TQ3Uns32)tcDataSize == inCachedVBO->mNumPoints * sizeof(float) *
				inCachedVBO->mSecondaryTexCoord[i].mCoordsPerVertex );
			tcSumSize += tcDataSize;
		#endif
		
			(*inFuncs.glClientActiveTextureProc)( inCachedVBO->mSecondaryTexCoord[i].mTextureUnit );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			glTexCoordPointer( inCachedVBO->mSecondaryTexCoord[i].mCoordsPerVertex, GL_FLOAT, 0,
				BufferObPtr( 0 ) );
		}
		(*inFuncs.glClientActiveTextureProc)( GL_TEXTURE0_ARB );
	}
	
	(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER,
		inCachedVBO->mGLBufferNames[1] );
	CHECK_GL_ERROR;

#if Q3_DEBUG
	(*inFuncs.glGetBufferParameterivProc)( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE,
		&elementDataSize );
	CHECK_GL_ERROR;
	Q3_ASSERT( (TQ3Uns32)elementDataSize == inCachedVBO->mNumIndices * sizeof(TQ3Uns32) );
	Q3_ASSERT( (GLint)inCachedVBO->mBufferBytes == elementDataSize + mainDataSize + tcSumSize );
#endif

	//Q3_MESSAGE_FMT("glDrawElements in RenderVBO (count %d, mode %d)", (int)inCachedVBO->mNumIndices,
	//	(int)inCachedVBO->mGLMode );
	glDrawElements( inCachedVBO->mGLMode, inCachedVBO->mNumIndices,
		GL_UNSIGNED_INT, BufferObPtr( 0 ) );
	CHECK_GL_ERROR;
		
	(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
	(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
	
	if (inCachedVBO->mNumSecondaryTexCoords > 0)
	{
		for (i = 0; i < inCachedVBO->mNumSecondaryTexCoords; ++i)
		{
			(*inFuncs.glClientActiveTextureProc)( inCachedVBO->mSecondaryTexCoord[i].mTextureUnit );
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
			(*inFuncs.glMultiTexCoord1fProc)( inCachedVBO->mSecondaryTexCoord[i].mTextureUnit, 0.0f );
		}
		(*inFuncs.glClientActiveTextureProc)( GL_TEXTURE0_ARB );
	}
}

void	VBOCache::DeleteFromUsageList( CachedVBO* ioVBO )
{
	CHECK_VBO( ioVBO );
	ioVBO->mPrev->mNext = ioVBO->mNext;
	ioVBO->mNext->mPrev = ioVBO->mPrev;
}


void	VBOCache::DeleteVBO( CachedVBO* inCachedVBO, const GLBufferFuncs& inFuncs )
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
	
	for (TQ3Uns32 i = 0; i < inCachedVBO->mNumSecondaryTexCoords; ++i)
	{
		(*inFuncs.glDeleteBuffersProc)( 1, &inCachedVBO->mSecondaryTexCoord[i].mGLBufferName );
		ForgetVBO( inCachedVBO->mSecondaryTexCoord[i].mGLBufferName );
	}
	
	DeleteFromUsageList( inCachedVBO );
	
	mTotalBytes -= inCachedVBO->mBufferBytes;
	//Q3_MESSAGE_FMT("VBO cache size after delete: %lld", mTotalBytes );
	
	FREED_VBO( inCachedVBO );
	// It would be more logical to call FREED_VBO after delete,
	// but that causes the Clang static analyzer to issue a
	// "Use of memory after it is freed" warning.
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
	VALIDATE_COUNT( this );
	FlushUnreferencedInVec( mCachedVBOs_strips, inFuncs );
	FlushUnreferencedInVec( mCachedVBOs_triangles, inFuncs );
	FlushUnreferencedInVec( mCachedVBOs_lines, inFuncs );
	VALIDATE_COUNT( this );
}

void	VBOCache::PurgeDownToSize( long long inTargetSize, const GLBufferFuncs& inFuncs )
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

void	VBOCache::SetMaxBufferSize( long long inBufferSize, const GLBufferFuncs& inFuncs )
{
	VALIDATE_COUNT( this );
	if (inBufferSize < mMaxBufferBytes)
	{
		PurgeDownToSize( inBufferSize, inFuncs );
	}
	
	mMaxBufferBytes = inBufferSize;
	VALIDATE_COUNT( this );
}

void	VBOCache::MakeRoom( TQ3Uns32 inBytesNeeded, const GLBufferFuncs& inFuncs )
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

void	VBOCache::AddBytes( TQ3Uns32 inBytes )
{
	mTotalBytes += inBytes;
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
	
	if (theCache != nullptr)
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
	
	if (theCache != nullptr)
	{
		CachedVBO*	theVBO = theCache->FindVBO( inGeom, inMode, inFuncs );
		
		if ( (theVBO == nullptr) && (inMode == GL_TRIANGLE_STRIP) )
		{
			theVBO = theCache->FindVBO( inGeom, GL_TRIANGLES, inFuncs );
		}
		
		if (theVBO != nullptr)
		{
			CHECK_VBO( theVBO );
			theCache->RenderVBO( inFuncs, theVBO );
			theCache->RenewInUsageList( theVBO );
			didRender = kQ3True;
		}
	}
	
	return didRender;
}



/*!
	@function		AddSecondaryTextureCoordsToVBO
	@abstract		Add an array of secondary texture coordinates to an
					already-cached VBO, as an optional follow-up to
					AddVBOToCache.
	@param			inCache				A VBO cache.
	@param			ioVBO				A cached VBO object.
	@param			inTextureUnit		A texture unit to which we should supply
										the array, e.g., GL_TEXTURE1_ARB.
	@param			inNumPoints			Number of points (vertices).
	@param			inNumCoordsPerPoint	Number of floating-point values for each
										vertex.  Must be 1, 2, 3, or 4.
	@param			inData				Array of data.  The number of floating-point
										values should be inNumPoints * inNumCoordsPerPoint.
*/
static void			AddSecondaryTextureCoordsToVBO(
									const GLBufferFuncs& inFuncs,
									VBOCache* inCache,
									CachedVBO* ioVBO,
									GLenum inTextureUnit,
									TQ3Uns32 inNumPoints,
									TQ3Uns32 inNumCoordsPerPoint,
									const float* inData )
{
	if ( (ioVBO != nullptr) && (ioVBO->mNumSecondaryTexCoords < kMaxSecondaryTexCoords) )
	{
		TQ3Uns32	coordIndex = ioVBO->mNumSecondaryTexCoords;
		ioVBO->mNumSecondaryTexCoords += 1;
		SecondaryTexCoord& theCoord( ioVBO->mSecondaryTexCoord[ coordIndex ] );
		(*inFuncs.glGenBuffersProc)( 1, &theCoord.mGLBufferName );
		theCoord.mCoordsPerVertex = (GLint) inNumCoordsPerPoint;
		theCoord.mTextureUnit = inTextureUnit;
		
		TQ3Uns32 dataSize = inNumPoints * inNumCoordsPerPoint * sizeof(float);
		ioVBO->mBufferBytes += dataSize;
		inCache->MakeRoom( dataSize, inFuncs );
		inCache->AddBytes( dataSize );
		
		(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, theCoord.mGLBufferName );
		(*inFuncs.glBufferDataProc)( GL_ARRAY_BUFFER, dataSize,
			inData, GL_STATIC_DRAW );
		(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
		RecordVBO( theCoord.mGLBufferName, ioVBO->mSortKey, dataSize, 1 );
	}
}



/*!
	@function		HandleSecondaryTextureCoords
	@abstract		Check for secondary texture coordinates attached to the geometry.
	@param			inGeom				A geometry object.
	@param			inCache				A VBO cache.
	@param			ioVBO				A cached VBO object.
	@param			inNumPoints			Number of points (vertices).
*/
static void HandleSecondaryTextureCoords(
									const GLBufferFuncs& inFuncs,
									TQ3GeometryObject inGeom,
									VBOCache* inCache,
									CachedVBO* ioVBO,
									TQ3Uns32 inNumPoints )
{
	TQ3Uns32 bufSize;
	if ( (kQ3Success == Q3Object_GetProperty( inGeom,
		kQ3GeometryPropertyCustomTextureCoordinates, 0, &bufSize, nullptr )) &&
		((bufSize % 4) == 0) &&
		bufSize >= sizeof(TQ3CustomTextureCoordinates) + inNumPoints * sizeof(float) )
	{
		std::vector<TQ3Uns8>	buffer( bufSize );
		Q3Object_GetProperty( inGeom, kQ3GeometryPropertyCustomTextureCoordinates,
			bufSize, nullptr, &buffer[0] );
		TQ3CustomTextureCoordinates* tcRec(
			reinterpret_cast<TQ3CustomTextureCoordinates*>( &buffer[0] ) );
		Q3_ASSERT( tcRec->numPoints == inNumPoints );
		if (tcRec->numPoints == inNumPoints)
		{
			GLenum textureUnit = GL_TEXTURE0_ARB + tcRec->textureUnit;
			AddSecondaryTextureCoordsToVBO( inFuncs, inCache, ioVBO, textureUnit,
				inNumPoints, tcRec->coordsPerPoint, tcRec->coords );
		}
	}
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
	@param			inNormals		Array of normal vectors (or nullptr).
	@param			inColors		Array of vertex colors (or nullptr).
	@param			inUVs			Array of vertex UV coordinates (or nullptr).
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
	
	if (theCache != nullptr)
	{
		CachedVBO*	newVBO = new CachedVBO( inGeom, inMode );
		ALLOCATED_VBO( newVBO );
		newVBO->mNumIndices = inNumIndices;
	#if Q3_DEBUG
		newVBO->mNumPoints = inNumPoints;
	#endif
		(*inFuncs.glGenBuffersProc)( 2, newVBO->mGLBufferNames );
		sVBOCount += 2;
		//Q3_MESSAGE_FMT("Created VBOs %d, %d, now there are %d",
		//	newVBO->mGLBufferNames[0], newVBO->mGLBufferNames[1], sVBOCount );
		
		TQ3Uns32	vertexDataSize = static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3Point3D));
		TQ3Uns32	normalDataSize = (inNormals == nullptr)? 0 :
			static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3Vector3D));
		TQ3Uns32	colorDataSize = (inColors == nullptr)? 0 :
			static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3ColorRGB));
		TQ3Uns32	uvDataSize = (inUVs == nullptr)? 0 :
			static_cast<TQ3Uns32>(inNumPoints * sizeof(TQ3Param2D));
		TQ3Uns32	totalDataSize = vertexDataSize + normalDataSize +
			colorDataSize + uvDataSize;
		
		newVBO->mBufferBytes = static_cast<TQ3Uns32>(inNumIndices * sizeof(TQ3Uns32) + totalDataSize);
		theCache->MakeRoom( newVBO->mBufferBytes, inFuncs );
			
		newVBO->mVertexBufferOffset = 0;
		newVBO->mNormalBufferOffset = (inNormals == nullptr)? kAbsentBuffer :
			vertexDataSize;
		newVBO->mColorBufferOffset = (inColors == nullptr)? kAbsentBuffer :
			vertexDataSize + normalDataSize;
		newVBO->mTextureUVBufferOffset = (inUVs == nullptr)? kAbsentBuffer :
			vertexDataSize + normalDataSize + colorDataSize;
		
		theCache->AddVBO( newVBO );
		
		const GLvoid *	dataAddr;
		
		// First define an uninitialized buffer of the right size
		(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER,
			newVBO->mGLBufferNames[0] );
		(*inFuncs.glBufferDataProc)( GL_ARRAY_BUFFER, totalDataSize,
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
		(*inFuncs.glBufferSubDataProc)( GL_ARRAY_BUFFER, 0,
			vertexDataSize, dataAddr );
		if (inNormals != nullptr)
		{
			dataAddr = inNormals;
			(*inFuncs.glBufferSubDataProc)( GL_ARRAY_BUFFER,
				newVBO->mNormalBufferOffset, normalDataSize, dataAddr );
		}
		if (inColors != nullptr)
		{
			dataAddr = inColors;
			(*inFuncs.glBufferSubDataProc)( GL_ARRAY_BUFFER,
				newVBO->mColorBufferOffset, colorDataSize, dataAddr );
		}
		if (inUVs != nullptr)
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
		RecordVBO( newVBO->mGLBufferNames[1], inGeom, inNumIndices * sizeof(TQ3Uns32), 0 );
		
		HandleSecondaryTextureCoords( inFuncs, inGeom, theCache, newVBO, inNumPoints );
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
	
	if (theCache != nullptr)
	{
		theCache->FlushUnreferenced( inFuncs );
	}
}



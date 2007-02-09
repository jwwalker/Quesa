/*  NAME:
        GLDisplayListManager.cpp

    DESCRIPTION:
        OpenGL display list caching.
       
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

#include "GLDisplayListManager.h"
#include "GLGPUSharing.h"
#include "CQ3ObjectRef.h"

#include <vector>
#include <algorithm>


//=============================================================================
//		Internal constants
//-----------------------------------------------------------------------------

namespace
{
	const TQ3Uns32	kDisplayListCacheKey	=
		Q3_FOUR_CHARACTER_CONSTANT('d', 'l', 'c', 'k');
	
}


//=============================================================================
//		Internal types
//-----------------------------------------------------------------------------

namespace
{
	struct CachedDisplayList
	{
						CachedDisplayList() {}
						CachedDisplayList( TQ3GeometryObject inGeom );
						CachedDisplayList( const CachedDisplayList& inOther );
						~CachedDisplayList() {}
						
		CachedDisplayList&	operator=( const CachedDisplayList& inOther );
						
		bool			IsStale() const;
	
		CQ3ObjectRef	mGeomObject;
		TQ3Uns32		mEditIndex;
		GLuint			mDisplayList;
	};
	
	typedef	std::vector< CachedDisplayList >	DisplayListVec;
	
	class DisplayListCache : public CQ3GPSharedCache
	{
	public:
		CachedDisplayList*	FindDisplayList( TQ3GeometryObject inGeom );
		void				DeleteList( CachedDisplayList* inList );
		void				RenderList( const CachedDisplayList* inList );
		void				AddList( const CachedDisplayList& inList );
		void				Flush();
		
	private:
		// The vector of display list records will be maintained in order
		// of geometry reference, so that search can be faster.
		DisplayListVec		mDisplayLists;
	};

	// Comparator for ordering cached display lists by their geometries
	struct GeomLess
	{
		bool	operator()( const CachedDisplayList& inOne,
							const CachedDisplayList& inTwo ) const
				{
					return inOne.mGeomObject.get() < inTwo.mGeomObject.get();
				}
	};
	
	// Predicate for use with std::partition and DisplayListVec.
	struct IsReferenced
	{
		bool		operator()( const CachedDisplayList& inCachedDL ) const
								{
									return Q3Shared_IsReferenced(
										inCachedDL.mGeomObject.get() ) ==
										kQ3True;
								}
	};
}


//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------

CachedDisplayList::CachedDisplayList( TQ3GeometryObject inGeom )
	: mGeomObject( Q3Shared_GetReference( inGeom ) )
	, mEditIndex( Q3Shared_GetEditIndex( inGeom ) )
{
	// Leave other fields uninitialized for now
}

CachedDisplayList::CachedDisplayList( const CachedDisplayList& inOther )
	: mGeomObject( inOther.mGeomObject )
	, mEditIndex( inOther.mEditIndex )
	, mDisplayList( inOther.mDisplayList )
{
}

CachedDisplayList&	CachedDisplayList::operator=( const CachedDisplayList& inOther )
{
	mGeomObject = inOther.mGeomObject;
	mEditIndex = inOther.mEditIndex;
	mDisplayList = inOther.mDisplayList;
	return *this;
}

bool	CachedDisplayList::IsStale() const
{
	return Q3Shared_GetEditIndex( mGeomObject.get() ) != mEditIndex;
}

static DisplayListCache* GetDisplayListCache( TQ3GLContext glContext )
{
	DisplayListCache*	theCache = static_cast<DisplayListCache*>(
		GLGPUSharing_GetCache( glContext, kDisplayListCacheKey ) );
	
	if (theCache == NULL)
	{
		theCache = new(std::nothrow) DisplayListCache;
		
		if (theCache != NULL)
		{
			GLGPUSharing_AddCache( glContext, kDisplayListCacheKey, theCache );
		}
	}
	
	return theCache;
}

CachedDisplayList*	DisplayListCache::FindDisplayList( TQ3GeometryObject inGeom )
{
	CachedDisplayList*	theCachedList = NULL;
	CachedDisplayList	searchDummy( inGeom );
	
	DisplayListVec::iterator	foundIt = std::lower_bound( mDisplayLists.begin(),
		mDisplayLists.end(), searchDummy, GeomLess() );
	
	if ( (foundIt != mDisplayLists.end()) &&
		(foundIt->mGeomObject.get() == inGeom) )
	{
		theCachedList = &*foundIt;
	}
	
	return theCachedList;
}

void	DisplayListCache::DeleteList( CachedDisplayList* inList )
{
	glDeleteLists( inList->mDisplayList, 1 );

	TQ3Uns32	arrayIndex = inList - &mDisplayLists[0];
	mDisplayLists.erase( mDisplayLists.begin() + arrayIndex );
}

void	DisplayListCache::RenderList( const CachedDisplayList* inList )
{
	glCallList( inList->mDisplayList );
}

void	DisplayListCache::AddList( const CachedDisplayList& inList )
{
	try
	{
		DisplayListVec::iterator	placeIt = std::lower_bound(
			mDisplayLists.begin(), mDisplayLists.end(), inList, GeomLess() );

		mDisplayLists.insert( placeIt, inList );
	}
	catch (...)
	{
	}
}

void	DisplayListCache::Flush()
{
	// Move unreferenced display lists to end of list
	DisplayListVec::iterator startUnused = std::stable_partition(
		mDisplayLists.begin(), mDisplayLists.end(), IsReferenced() );
	
	// Delete the lists for the records that are going away
	for (DisplayListVec::iterator i = startUnused; i != mDisplayLists.end(); ++i)
	{
		glDeleteLists( i->mDisplayList, 1 );
	}
	
	mDisplayLists.erase( startUnused, mDisplayLists.end() );
}


#pragma mark -
//=============================================================================
//		Public functions
//-----------------------------------------------------------------------------

/*!
	@function		RenderCachedDisplayList
	@abstract		Look for a cached display list for the given geometry and
					OpenGL context.  If we find one, render it.
	@discussion		If we find the object in the cache, but the cached object
					is stale, we delete it from the cache and return false.
					
					The caller should have activated the GL context.
	@param			glContext		An OpenGL context.
	@param			inGeom			A geometry object.
	@result			True if the object was found and rendered.
*/
TQ3Boolean			RenderCachedDisplayList(
									TQ3GLContext glContext,
									TQ3GeometryObject inGeom )
{
	TQ3Boolean	didRender = kQ3False;
	DisplayListCache*	theCache = GetDisplayListCache( glContext );
	
	if (theCache != NULL)
	{
		CachedDisplayList*	theList = theCache->FindDisplayList( inGeom );
		
		if (theList != NULL)
		{
			if ( theList->IsStale() )
			{
				theCache->DeleteList( theList );
			}
			else
			{
				theCache->RenderList( theList );
				didRender = kQ3True;
			}
		}
	}
	
	return didRender;
}

/*!
	@function		CacheDisplayList
	@abstract		Add a display list to the cache.
	@discussion		If RenderCachedDisplayList returns kQ3False, the caller
					should create a new display list as follows:
					
					<blockquote><pre><code>
					GLuint	displayListID = glGenLists( 1 );
					glNewList( displayListID, GL_COMPILE );
					
					IssueSomeRenderingCommands();
					
					glEndList();
					CacheDisplayList( displayListID, glContext, geom );
					</code></pre></blockquote>
					
					and finally call RenderCachedDisplayList again.
*/
void				CacheDisplayList(
									GLuint inDisplayList,
									TQ3GLContext glContext,
									TQ3GeometryObject inGeom )
{
	DisplayListCache*	theCache = GetDisplayListCache( glContext );
	
	if (theCache != NULL)
	{
		CachedDisplayList	newRec( inGeom );
		newRec.mDisplayList = inDisplayList;
		
		theCache->AddList( newRec );
	}
}

/*!
	@function		FlushDisplayListCache
	@abstract		Delete any cached display lists for geometries that are no
					longer referenced elsewhere.
	@param			glContext		An OpenGL context.
*/
void				FlushDisplayListCache(
									TQ3GLContext glContext )
{
	DisplayListCache*	theCache = GetDisplayListCache( glContext );
	
	if (theCache != NULL)
	{
		theCache->Flush();
	}
}

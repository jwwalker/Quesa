/*  NAME:
        GLGPUSharing.cpp

    DESCRIPTION:
        Source for GPU shared data cache manager.

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
#include "GLGPUSharing.h"


#include <algorithm>
#include <vector>
#include <map>
#include <list>


//=============================================================================
//		Internal types
//-----------------------------------------------------------------------------

namespace
{
	typedef std::vector< TQ3GLContext >					GLContextVec;
	typedef std::map< TQ3Uns32, CQ3GPSharedCache* >		CacheMap;
	
	/*!
		@class		GPUSharingGroup
		@abstract	A sharing group, which records GL contexts that share data
					and caches that hold that data.
	*/
	struct GPUSharingGroup
	{
							GPUSharingGroup();
							GPUSharingGroup(
								const GPUSharingGroup& inOther );
							~GPUSharingGroup();
		
		GPUSharingGroup&	operator=( const GPUSharingGroup& inOther );
	
		GLContextVec	mGLContexts;
		CacheMap		mCacheMap;
	};
	
	typedef std::list< GPUSharingGroup >		GroupList;
}


//=============================================================================
//		Static variables
//-----------------------------------------------------------------------------

static GroupList*	sGroupList = NULL;



//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------


/*!
	@function	InitGroups
	@abstract	Create the vector of sharing groups if it has not already been
				created.
*/
static void InitGroups()
{
	if (sGroupList == NULL)
	{
		sGroupList = new(std::nothrow) GroupList;
		
		if (sGroupList == NULL)
		{
			E3ErrorManager_PostError( kQ3ErrorOutOfMemory, kQ3True );
		}
	}
}


/*!
	@function	FindGroupFromContext
	@abstract	Find the sharing group containing a specific CL context.
	@param		inContext			A GL context.
	@param		outGroupIterator	Receives an iterator into sGroupList,
									possibly sGroupList->end().
	@param		outWhichContext		Receives an iterator to the context.
									May be omitted if you do not need this.
	@result		True if we found a group containing the context.
*/
static bool FindGroupFromContext( TQ3GLContext inContext,
								GroupList::iterator& outGroupIterator,
								GLContextVec::iterator* outWhichContext = NULL )
{
	bool	didFind = false;
	InitGroups();
	
	if (sGroupList != NULL)
	{
		GroupList::iterator	endList = sGroupList->end();
		outGroupIterator = endList;
		
		for (GroupList::iterator i = sGroupList->begin(); i != endList; ++i)
		{
			GLContextVec::iterator	foundContextIt = std::find(
				i->mGLContexts.begin(), i->mGLContexts.end(), inContext );
			
			if (foundContextIt != i->mGLContexts.end())
			{
				if (outWhichContext != NULL)
					*outWhichContext = foundContextIt;
				outGroupIterator = i;
				didFind = true;
				break;
			}
		}
	}
	
	
	return didFind;
}

GPUSharingGroup::GPUSharingGroup()
{
}

GPUSharingGroup::GPUSharingGroup(
	const GPUSharingGroup& inOther )
	: mGLContexts( inOther.mGLContexts )
	, mCacheMap( inOther.mCacheMap )
{
}

GPUSharingGroup::~GPUSharingGroup()
{
	for (CacheMap::iterator i = mCacheMap.begin(); i != mCacheMap.end(); ++i)
	{
		delete i->second;
	}
}

GPUSharingGroup&	GPUSharingGroup::operator=(
									const GPUSharingGroup& inOther )
{
	GPUSharingGroup	temp( inOther );
	mGLContexts.swap( temp.mGLContexts );
	mCacheMap.swap( temp.mCacheMap );
	return *this;
}



//=============================================================================
//		Public functions
//-----------------------------------------------------------------------------


/*!
	@function		GLGPUSharing_GetNextSharingBase
	@abstract		Get the next GL sharing group base.
	@discussion		When a GL context is being created, we usually want to
					make it share data with previously created GL contexts.
					The texture manager maintains a list of existing GL contexts
					grouped by sharing.  This function can be used to find one
					context in each sharing group.
	@param			glBase			NULL to begin iteration, or a value returned
									by a previous call to this function.
	@result			Next GL sharing context, or NULL when there are no more.
*/

TQ3GLContext		GLGPUSharing_GetNextSharingBase( TQ3GLContext glBase )
{
	TQ3GLContext	nextContext = NULL;
	
	try
	{
		InitGroups();
		
		if (sGroupList != NULL)
		{
			if (glBase == NULL)
			{
				if (! sGroupList->empty())
				{
					Q3_ASSERT( ! sGroupList->front().mGLContexts.empty() );
					nextContext = sGroupList->front().mGLContexts[ 0 ];
				}
			}
			else
			{
				GroupList::iterator	theGroupIt;
				
				if (FindGroupFromContext( glBase, theGroupIt ))
				{
					++theGroupIt;
					if (theGroupIt != sGroupList->end())
					{
						nextContext = theGroupIt->mGLContexts[ 0 ];
					}
				}
			}
		}
	}
	catch (...)
	{
		E3ErrorManager_PostError( kQ3ErrorOutOfMemory, kQ3True );
	}
	
	return nextContext;
}


/*!
	@function		GLGPUSharing_AddContext
	@abstract		After successfully creating a new context, call this function
					to inform the GPU sharing manager.
	@param			newGLContext		The newly created context.
	@param			sharingBase			The existing context with which the new one
										shares data, or NULL.
*/
void				GLGPUSharing_AddContext( TQ3GLContext newGLContext,
											TQ3GLContext sharingBase )
{
	InitGroups();
	
	if (sGroupList != NULL)
	{
		try
		{
			GPUSharingGroup*	theGroup = NULL;
			
			// If there is a sharing base, look for an existing group containing it.
			if (sharingBase != NULL)
			{
				GroupList::iterator	groupIt;
				
				if (FindGroupFromContext( sharingBase, groupIt ))
				{
					theGroup = &*groupIt;
				}
			}
			
			// If we do not have an existing group to use, make a new one.
			if (theGroup == NULL)
			{
				sGroupList->push_back( GPUSharingGroup() );
				theGroup = &sGroupList->back();
			}
			
			if (theGroup != NULL)
			{
				theGroup->mGLContexts.push_back( newGLContext );
			}
		}
		catch (...)
		{
			E3ErrorManager_PostError( kQ3ErrorOutOfMemory, kQ3True );
		}
	}
}

/*!
	@function		GLGPUSharing_RemoveContext
	@abstract		Inform the GPU sharing manager that a GL context has been destroyed.
	@discussion		The given GL context will be forgotten from its sharing group.
					If there are no more contexts associated with the sharing group,
					then the sharing group will be disposed.
	@param			deadGLContext		A former GL context.
*/
void				GLGPUSharing_RemoveContext( TQ3GLContext deadGLContext )
{
	GroupList::iterator	groupIt;
	GLContextVec::iterator	contextIt;
	
	if (FindGroupFromContext( deadGLContext, groupIt, &contextIt ))
	{
		groupIt->mGLContexts.erase( contextIt );
		
		if (groupIt->mGLContexts.empty())
		{
			sGroupList->erase( groupIt );
		}
	}
}

/*!
	@function		GLGPUSharing_IsContextKnown
	@abstract		Test whether the sharing manager knows about an OpenGL context.
	@param			inGLContext			A GL context.
	@param			True if we find a sharing group for the context.
*/
bool				GLGPUSharing_IsContextKnown( TQ3GLContext inGLContext )
{
	GroupList::iterator	groupIt;
	
	return FindGroupFromContext( inGLContext, groupIt );
}



/*!
	@function		GLGPUSharing_GetCache
	@abstract		Locate a cache within a sharing group.
	@param			glContext			A GL context.
	@param			inCacheType			key code for type of cache.
	@result			A cache pointer, or NULL.
*/
CQ3GPSharedCache*	GLGPUSharing_GetCache( TQ3GLContext glContext,
											TQ3Uns32 inCacheType )
{
	CQ3GPSharedCache*	theCache = NULL;
	GroupList::iterator	groupIt;
	
	if (FindGroupFromContext( glContext, groupIt ))
	{
		CacheMap::iterator	foundIt = groupIt->mCacheMap.find( inCacheType );
		if (foundIt != groupIt->mCacheMap.end())
		{
			theCache = foundIt->second;
		}
	}
	else
	{
		Q3_ASSERT( !"unknown GL context" );
	}
	
	return theCache;
}

/*!
	@function		GLGPUSharing_AddCache
	@abstract		Add a cache to a sharing group.
	@discussion		This should only be called if GLGPUSharing_GetCache has
					returned NULL.  After this call, the sharing group takes
					ownership of the cache, so do not delete the cache. it will
					be deleted when the sharing group is deleted.
	@param			glContext			A GL context.
	@param			inCacheType			key code for type of cache.
	@param			inCache				A cache.
*/
void				GLGPUSharing_AddCache( TQ3GLContext glContext,
											TQ3Uns32 inCacheType,
											CQ3GPSharedCache* inCache )
{
	Q3_ASSERT( GLGPUSharing_GetCache( glContext, inCacheType ) == NULL );
	GroupList::iterator	groupIt;
	
	if (FindGroupFromContext( glContext, groupIt ))
	{
		try
		{
			groupIt->mCacheMap[ inCacheType ] = inCache;
		}
		catch (...)
		{
			E3ErrorManager_PostError( kQ3ErrorOutOfMemory, kQ3True );
		}
	}
}

#if Q3_DEBUG
/*!
	@function		GLGPUSharing_IsCacheValid
	@abstract		Test whether a cache exists, for debugging.
	@param			inCacheType			key code for type of cache.
	@param			inCache				A cache.
	@result			True if the cache is found.
*/
bool				GLGPUSharing_IsCacheValid(
										CQ3GPSharedCache* inCache,
										TQ3Uns32 inCacheType )
{
	bool	didFind = false;
	
	if (sGroupList != NULL)
	{
		for (GroupList::iterator i = sGroupList->begin();
			i != sGroupList->end(); ++i)
		{
			CacheMap::iterator	foundIt = i->mCacheMap.find( inCacheType );
			if ( (foundIt != i->mCacheMap.end()) &&
				(foundIt->second == inCache) )
			{
				didFind = true;
				break;
			}
		}
	}
	
	return didFind;
}
#endif


/*  NAME:
        GLTextureManager.c

    DESCRIPTION:
        Quesa OpenGL texture caching.
       
    REMARKS:
    	The TQ3InteractiveData structure contains the current GL context
    	pointer and the current texture cache pointer.  It would have been
    	possible to hold only the GL context pointer in TQ3InteractiveData,
    	and let the texture manager look up the texture cache at need.
    	Keeping the texture cache pointer in TQ3InteractiveData was a
    	performance optimization.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
#include "GLTextureManager.h"
#include "IRPrefix.h"

#ifndef __cplusplus
	#error This file must be compiled as C++.
#endif

#include <algorithm>
#include <list>
#include <vector>



//=============================================================================
//		Internal types
//-----------------------------------------------------------------------------
namespace
{
	// We use a list instead of a vector to hold texture caches because we use
	// the pointer to the TQ3CachedTexture as the OpenGl "name" of the texture.
	typedef std::list<TQ3CachedTexture>		CachedTextureList;
	typedef std::vector<TQ3GLContext>		GLContextVec;
}

// TQ3TextureCache: object holding cached textures for a number of GL contexts
// that share texture memory.  This declaration cannot be in an unnamed namespace,
// because it needs to match the struct TQ3TextureCache* member of the TQ3InteractiveData
// structure.
typedef struct TQ3TextureCache
{
	CachedTextureList		cachedTextures;
	GLContextVec			glContexts;
} TQ3TextureCache;

namespace
{
	// The primary reason for using a list, rather than a vector, here is that
	// TQ3InteractiveData holds a pointer to its associated texture cache.
	// If we used a vector, then such a pointer would be invalidated by adding
	// or deleting other texture caches.
	typedef std::list< TQ3TextureCache >	TextureCacheList;


	class MatchCachePtr	// function object for use with find_if and TextureCacheList
	{
	public:
				MatchCachePtr( TQ3TextureCachePtr inCache )
					: mCachePtrToMatch( inCache ) {}
				
		bool	operator()( const TQ3TextureCache& inCache ) const
						{ return &inCache == mCachePtrToMatch; }

	private:
		TQ3TextureCachePtr	mCachePtrToMatch;
	};

	class MatchTexture	// function object for use with find_if and CachedTextureList
	{
	public:
							MatchTexture( TQ3TextureObject inTexture )
									: mTextureToMatch( inTexture ) {}
							MatchTexture( const MatchTexture& inOther )
									: mTextureToMatch( inOther.mTextureToMatch ) {}

		bool				operator()( const TQ3CachedTexture& inCachedTexture ) const
									{
										return (inCachedTexture.cachedTextureObject.get() ==
											mTextureToMatch);
									}
	private:
		TQ3TextureObject	mTextureToMatch;
	};

}



//=============================================================================
//		Static variables
//-----------------------------------------------------------------------------
static TextureCacheList*			sTextureCacheList = NULL;



//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------
//		gltexturemgr_InitList : Lazy initialization of the cache list.
//-----------------------------------------------------------------------------
static void
gltexturemgr_InitList()
{
	if (sTextureCacheList == NULL)
	{
		sTextureCacheList = new(std::nothrow) TextureCacheList;
		
		if (sTextureCacheList == NULL)
		{
			E3ErrorManager_PostError( kQ3ErrorOutOfMemory, kQ3True );
		}
	}
}





//=============================================================================
//		gltexturemgr_FindContextInCacheList : Find a GL context in our cache.
//-----------------------------------------------------------------------------
static TQ3Status
gltexturemgr_FindContextInCacheList( TQ3GLContext context,
								TextureCacheList::iterator* outWhichTextureCache,
								GLContextVec::iterator* outWhichContext = NULL )
{
	TQ3Status	status = kQ3Failure;
	
	gltexturemgr_InitList();
	
	if (sTextureCacheList != NULL)
	{
		TextureCacheList::iterator	endList =  sTextureCacheList->end();
		*outWhichTextureCache = endList;
		
		for (TextureCacheList::iterator	i = sTextureCacheList->begin(); i != endList; ++i)
		{
			GLContextVec::iterator	foundContextIt = std::find( i->glContexts.begin(),
				i->glContexts.end(), context );
			
			if (foundContextIt != i->glContexts.end())
			{
				if (outWhichContext != NULL)
					*outWhichContext = foundContextIt;
				*outWhichTextureCache = i;
				status = kQ3Success;
				break;
			}
		}
	}
	
	return status;
}





#pragma mark -
//=============================================================================
//		Public functions
//-----------------------------------------------------------------------------
/*!
	@function		GLTextureMgr_GetNextSharingBase
	@abstract		Get the next GL sharing context base.
	@discussion		When a GL context is being created, we usually want to
					make it share textures with previously created GL contexts.
					The texture manager maintains a list of existing GL contexts
					grouped by sharing.  This function can be used to find one
					context in each sharing group.
	@param			glBase			NULL to begin iteration, or a value returned
									by a previous call to this function.
	@result			Next GL sharing context, or NULL when there are no more.
*/
TQ3GLContext		GLTextureMgr_GetNextSharingBase( TQ3GLContext glBase )
{
	TQ3GLContext	nextContext = NULL;
	
	TRY
	{
		gltexturemgr_InitList();
		
		if (sTextureCacheList != NULL)
		{
			if (glBase == NULL)
			{
				if (! sTextureCacheList->empty())
				{
					Q3_ASSERT( ! sTextureCacheList->front().glContexts.empty() );
					nextContext = sTextureCacheList->front().glContexts[ 0 ];
				}
			}
			else
			{
				TextureCacheList::iterator	theCacheIt;
				
				if (kQ3Success == gltexturemgr_FindContextInCacheList( glBase, &theCacheIt ))
				{
					++theCacheIt;
					if (theCacheIt != sTextureCacheList->end())
					{
						nextContext = theCacheIt->glContexts[ 0 ];
					}
				}
			}
		}
	}
	CATCH_ALL
	
	return nextContext;
}





/*!
	@function		GLTextureMgr_AddContext
	@abstract		After successfully creating a new context, call this function
					to inform the texture manager.
	@discussion		If the new context shares with a previous context, the texture
					manager records the context and returns the texture cache.
					If there was no shared context, the texture manager creates
					a new texture cache.
	@param			newGLContext		The newly created context.
	@param			sharingBase			The existing context with which the new one
										shares textures, or NULL.
*/
void				GLTextureMgr_AddContext( TQ3GLContext newGLContext,
											TQ3GLContext sharingBase )
{
	TQ3TextureCachePtr	theCache = NULL;
	
	TRY
	{
		gltexturemgr_InitList();
		
		if (sTextureCacheList != NULL)
		{
			if (sharingBase != NULL)
			{
				TextureCacheList::iterator	cacheIt;
				
				if (kQ3Success == gltexturemgr_FindContextInCacheList( sharingBase, &cacheIt ))
				{
					theCache = &*cacheIt;
				}
			}
			
			if (theCache == NULL)
			{
				TQ3TextureCache	newCache;
				sTextureCacheList->push_back( newCache );
				theCache = &(sTextureCacheList->back());
			}
			
			if (theCache != NULL)
			{
				theCache->glContexts.push_back( newGLContext );
			}
		}
	}
	CATCH_ALL
}





/*!
	@function		GLTextureMgr_GetTextureCache
	@abstract		After a GL context has been recorded in the texture manager,
					this function can be used to look up the corresponding
					texture cache.
	@param			glContext			A GL context.
	@result			Pointer to a texture cache.
*/
TQ3TextureCachePtr	GLTextureMgr_GetTextureCache( TQ3GLContext glContext )
{
	TQ3TextureCachePtr	theCache = NULL;
	
	TRY
	{
		TextureCacheList::iterator	cacheIt;
		
		if (kQ3Success == gltexturemgr_FindContextInCacheList( glContext, &cacheIt ))
		{
			theCache = &*cacheIt;
		}
	}
	CATCH_ALL
	
	return theCache;
}





#if Q3_DEBUG
/*!
	@function		GLTextureMgr_IsValidTextureCache
	@abstract		Test whether a texture cache pointer is valid.
	@param			txCache			A texture cache.
	@result			Whether the texture cache pointer is valid.
*/
TQ3Boolean			GLTextureMgr_IsValidTextureCache( TQ3TextureCachePtr txCache )
{
	TQ3Boolean	isValid = kQ3False;
	
	TRY
	{
		gltexturemgr_InitList();
		
		if (sTextureCacheList != NULL)
		{
			MatchCachePtr	matcher( txCache );
			
			if (std::find_if( sTextureCacheList->begin(), sTextureCacheList->end(), matcher ) !=
				sTextureCacheList->end())
			{
				isValid = kQ3True;
			}
		}
	}
	CATCH_ALL

	return isValid;
}
#endif





/*!
	@function		GLTextureMgr_FindCachedTexture
	@abstract		Access a texture cache record by matching the texture object.
	@discussion		This is the texture manager function that is called most often,
					so speed is important.
	@param			txCache			A texture cache.
	@param			texture			Reference to a texture object.
	@result			Pointer to a cached texture record, or NULL if not found.
*/
TQ3CachedTexture*	GLTextureMgr_FindCachedTexture( TQ3TextureCachePtr txCache,
								TQ3TextureObject texture )
{
	TQ3CachedTexture*	theRecord = NULL;
	
	TRY
	{
		CachedTextureList::iterator	foundIt = std::find_if( txCache->cachedTextures.begin(),
			txCache->cachedTextures.end(), MatchTexture( texture ) );
		
		if (foundIt != txCache->cachedTextures.end())
		{
			theRecord = &*foundIt;
		}
	}
	CATCH_ALL
	
	return theRecord;
}





/*!
	@function		GLTextureMgr_GetCachedTextureByIndex
	@abstract		Access a texture cache record by index.
	@param			txCache			A texture cache.
	@param			memberIndex		Zero-based index of a cached texture.
	@result			Pointer to a cached texture record, or NULL if not found.
*/
TQ3CachedTexture*	GLTextureMgr_GetCachedTextureByIndex( TQ3TextureCachePtr txCache,
								TQ3Uns32 memberIndex )
{
	TQ3CachedTexture*	theRecord = NULL;
	
	TQ3Uns32		theIndex = 0;
	
	for (CachedTextureList::iterator i = txCache->cachedTextures.begin();
		i != txCache->cachedTextures.end(); ++i, ++theIndex)
	{
		if (theIndex == memberIndex)
		{
			theRecord = &*i;
			break;
		}
	}
	return theRecord;
}





/*!
	@function		GLTextureMgr_RemoveContext
	@abstract		Inform the texture manager that a GL context has been destroyed.
	@discussion		The given GL context will be forgotten from its texture cache.
					If there are no more contexts associated with the texture cache,
					then the texture cache will be disposed.
	@param			deadGLContext		A former GL context.
*/
void				GLTextureMgr_RemoveContext( TQ3GLContext deadGLContext )
{
	TRY
	{
		GLContextVec::iterator		contextIter;
		TextureCacheList::iterator	theCache;
		
		if (kQ3Success == gltexturemgr_FindContextInCacheList( deadGLContext, &theCache,
			&contextIter ))
		{
			theCache->glContexts.erase( contextIter );
			
			if (theCache->glContexts.empty())
			{
				sTextureCacheList->erase( theCache );
			}
		}
	}
	CATCH_ALL
}





/*!
	@function		GLTextureMgr_AddCachedTexture
	@abstract		Append a texture record to the texture cache.
	@param			txCache			A texture cache.
	@param			textureRec		A new texture record to cache.
	@result			Success or failure of the operation.
*/
TQ3Status			GLTextureMgr_AddCachedTexture( TQ3TextureCachePtr txCache,
								TQ3CachedTexture* textureRec )
{
	TQ3Status	theStatus = kQ3Failure;
	TRY
	{
		txCache->cachedTextures.push_back( *textureRec );
		theStatus = kQ3Success;
	}
	CATCH_ALL

	return theStatus;
}





/*!
	@function		GLTextureMgr_RemoveCachedTexture
	@abstract		Remove a texture record from a texture cache.
	@param			txCache			A texture cache.
	@param			textureRec		Texture cache record to remove.
*/
void				GLTextureMgr_RemoveCachedTexture( TQ3TextureCachePtr txCache,
								TQ3CachedTexture* textureRec )
{
	TRY
	{
		CachedTextureList::iterator	iter;
		
		for (iter = txCache->cachedTextures.begin(); iter != txCache->cachedTextures.end();
			++iter)
		{
			if (&*iter == textureRec)
			{
				txCache->cachedTextures.erase( iter );
				break;
			}
		}
	}
	CATCH_ALL
}

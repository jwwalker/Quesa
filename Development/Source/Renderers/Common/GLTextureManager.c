/*  NAME:
        GLTextureManager.c

    DESCRIPTION:
        Quesa OpenGL texture caching.

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




//=============================================================================
//		Internal types
//-----------------------------------------------------------------------------
typedef struct TQ3TextureCache
{
	TQ3Uns32				cachedTextureCount;
	TQ3CachedTexture		*cachedTextures;
	TQ3Uns32				glContextCount;
	TQ3GLContext			*glContexts;
} TQ3TextureCache;





//=============================================================================
//		Static variables
//-----------------------------------------------------------------------------
static TQ3TextureCache*		sTextureCaches;
static TQ3Uns32				sTextureCacheCount = 0;




//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------
//		gltexturemgr_AppendToDynamicArray : Add an item to a growable array.
//-----------------------------------------------------------------------------
static TQ3Status
gltexturemgr_AppendToDynamicArray( void* newRecord, TQ3Uns32 recordSize,
								void** ioArray, TQ3Uns32* ioArraySize )
{
	TQ3Status	status;
	
	status = Q3Memory_Reallocate( ioArray, (*ioArraySize + 1) * recordSize);
	
	if (status == kQ3Success)
	{
		char*	arrayData = (char*) *ioArray;
		
		Q3Memory_Copy( newRecord, arrayData + *ioArraySize * recordSize, recordSize );
		*ioArraySize += 1;
	}
	return status;
}





//=============================================================================
//		gltexturemgr_DeleteFromDynamicArray : Delete an item from a growable array.
//-----------------------------------------------------------------------------
static void
gltexturemgr_DeleteFromDynamicArray( TQ3Uns32 index, TQ3Uns32 recordSize,
									void** ioArray, TQ3Uns32* ioArraySize )
{
	Q3_ASSERT( index < *ioArraySize );
	
	if (index < *ioArraySize)
	{
		if (*ioArraySize == 1)	// we are deleting the only record
		{
			Q3Memory_Free( ioArray );
		}
		else
		{
			// Just move down later items, do not bother resizing the storage.
			if (index < *ioArraySize - 1)
			{
				char*	theData = (char*) *ioArray;
				
				memmove( theData + index * recordSize,
						theData + (index+1) * recordSize,
						(*ioArraySize - index - 1) * recordSize );
			}
		}
		*ioArraySize -= 1;
	}
}





//=============================================================================
//		gltexturemgr_FindContextInCache : Find a GL context in our cache.
//-----------------------------------------------------------------------------
static TQ3Status
gltexturemgr_FindContextInCache( TQ3GLContext context,
											TQ3Uns32* outWhichTextureCache,
										 	TQ3Uns32* outWhichContext )
{
	TQ3Status	status = kQ3Failure;
	TQ3Uns32	whichCache, whichContext;
	
	for (whichCache = 0; (whichCache < sTextureCacheCount) && (status == kQ3Failure);
		++whichCache)
	{
		TQ3Uns32	kNumContexts = sTextureCaches[whichCache].glContextCount;
		
		for (whichContext = 0; whichContext < kNumContexts; ++whichContext)
		{
			if (sTextureCaches[whichCache].glContexts[whichContext] == context)
			{
				*outWhichTextureCache = whichCache;
				*outWhichContext = whichContext;
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
	
	if (glBase == NULL)
	{
		if (sTextureCacheCount > 0)
		{
			Q3_ASSERT( sTextureCaches[ 0 ].glContextCount > 0 );
			nextContext = sTextureCaches[ 0 ].glContexts[ 0 ];
		}
	}
	else
	{
		TQ3Uns32	i;
		
		for (i = 0; i < sTextureCacheCount; ++i)
		{
			if (sTextureCaches[ i ].glContexts[ 0 ] == glBase)
			{
				break;
			}
		}
		
		if (i + 1 < sTextureCacheCount)
		{
			nextContext = sTextureCaches[ i + 1 ].glContexts[ 0 ];
		}
	}
	
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
	TQ3Uns32	i;
	
	
	if (sharingBase != NULL)
	{
		for (i = 0; i < sTextureCacheCount; ++i)
		{
			if (sTextureCaches[ i ].glContexts[ 0 ] == sharingBase)
			{
				theCache = &sTextureCaches[ i ];
				break;
			}
		}
	}
	
	if (theCache == NULL)
	{
		TQ3TextureCache	newCache;
		newCache.cachedTextureCount = 0;
		newCache.cachedTextures = NULL;
		newCache.glContextCount = 0;
		newCache.glContexts = NULL;
		
		if (kQ3Success == gltexturemgr_AppendToDynamicArray( &newCache, sizeof(newCache),
			(void**)&sTextureCaches, &sTextureCacheCount ))
		{
			theCache = &sTextureCaches[ sTextureCacheCount - 1 ];
		}
	}
	
	if (theCache != NULL)
	{
		gltexturemgr_AppendToDynamicArray( &newGLContext, sizeof(newGLContext),
			(void**)&theCache->glContexts, &theCache->glContextCount );
	}
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
	TQ3Uns32	whichCache, whichContext;
	TQ3TextureCachePtr	theCache = NULL;
	
	if (kQ3Success == gltexturemgr_FindContextInCache( glContext, &whichCache,
		&whichContext ))
	{
		theCache =  &sTextureCaches[ whichCache ];
	}
	Q3_ASSERT( theCache );
	
	return theCache;
}





/*!
	@function		GLTextureMgr_GetCachedTextureByIndex
	@abstract		Access a texture cache record by index.
	@param			txCache			A texture cache.
	@param			memberIndex		Zero-based index of a cached texture.
	@result			Pointer to a cached texture record, or NULL if not found.
*/
struct TQ3CachedTexture*
					GLTextureMgr_GetCachedTextureByIndex( TQ3TextureCachePtr txCache,
								TQ3Uns32 memberIndex )
{
	TQ3CachedTexture*	theRecord = NULL;
	
	if (memberIndex < txCache->cachedTextureCount)
	{
		theRecord = &txCache->cachedTextures[ memberIndex ];
	}
	return theRecord;
}





/*!
	@function		GLTextureMgr_FindCachedTextureIndex
	@abstract		Access a texture cache record by matching the texture object.
	@param			txCache			A texture cache.
	@param			texture			Reference to a texture object.
	@result			Index of a cached texture record, or -1 if not found.
*/
TQ3Int32			GLTextureMgr_FindCachedTextureIndex( TQ3TextureCachePtr txCache,
								TQ3TextureObject texture )
{
	TQ3Int32	theIndex = -1;
	TQ3Uns32	n;
	
	for (n = 0; n < txCache->cachedTextureCount; ++n)
	{
		if (txCache->cachedTextures[ n ].cachedTextureObject == texture)
		{
			theIndex = n;
			break;
		}
	}
	
	return theIndex;
}





/*!
	@function		GLTextureMgr_FindCachedTexture
	@abstract		Access a texture cache record by matching the texture object.
	@param			txCache			A texture cache.
	@param			texture			Reference to a texture object.
	@result			Pointer to a cached texture record, or NULL if not found.
*/
TQ3CachedTexture*	GLTextureMgr_FindCachedTexture( TQ3TextureCachePtr txCache,
								TQ3TextureObject texture )
{
	TQ3CachedTexture*	theRecord = NULL;
	TQ3Uns32	n;
	TQ3Uns32	txCount = txCache->cachedTextureCount;
	
	for (n = 0; n < txCount; ++n)
	{
		if (txCache->cachedTextures[ n ].cachedTextureObject == texture)
		{
			theRecord = &txCache->cachedTextures[ n ];
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
	TQ3Uns32	cacheIndex, contextIndex;
	
	if (kQ3Success == gltexturemgr_FindContextInCache( deadGLContext, &cacheIndex,
		&contextIndex ))
	{
		TQ3TextureCachePtr	theCache = &sTextureCaches[ cacheIndex ];
		
		gltexturemgr_DeleteFromDynamicArray( contextIndex, sizeof(TQ3GLContext),
			(void**)&theCache->glContexts, &theCache->glContextCount );
		
		if (theCache->glContextCount == 0)
		{
			TQ3Uns32	i;
			
			if (theCache->cachedTextureCount > 0)
			{
				for (i = 0; i < theCache->cachedTextureCount; ++i)
				{
					Q3Object_Dispose( theCache->cachedTextures[i].cachedTextureObject );
				}
				Q3Memory_Free( &theCache->cachedTextures );
			}
			
			gltexturemgr_DeleteFromDynamicArray( cacheIndex, sizeof(TQ3TextureCache),
				(void**)&sTextureCaches, &sTextureCacheCount );
		}
	}
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
	return gltexturemgr_AppendToDynamicArray( textureRec, sizeof(TQ3CachedTexture),
		(void**)&txCache->cachedTextures, &txCache->cachedTextureCount );
}





/*!
	@function		GLTextureMgr_RemoveCachedTexture
	@abstract		Remove a texture record from a texture cache.
	@param			txCache			A texture cache.
	@param			memberIndex		Zero-based index of a cached texture.
*/
void				GLTextureMgr_RemoveCachedTexture( TQ3TextureCachePtr txCache,
								TQ3Uns32 memberIndex )
{
	gltexturemgr_DeleteFromDynamicArray( memberIndex, sizeof(TQ3CachedTexture),
				(void**)&txCache->cachedTextures, &txCache->cachedTextureCount );
}

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
        Copyright (c) 1999-2009, Quesa Developers. All rights reserved.

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
#include "GLGPUSharing.h"

#ifndef __cplusplus
	#error This file must be compiled as C++.
#endif

#include <algorithm>
#include <set>
#include <vector>



//=============================================================================
//		Internal types
//-----------------------------------------------------------------------------

// Cached texture data
struct TQ3CachedTexture
{
	CQ3ObjectRef			cachedTextureObject;
	TQ3Uns32				editIndexTexture;
	TQ3Uns32				editIndexStorage;
	GLuint					glTextureName;
};

namespace
{
	struct CompByTexOb
	{
		bool operator()( TQ3CachedTexturePtr inOne, TQ3CachedTexturePtr inTwo ) const
		{
			return inOne->cachedTextureObject.get() < inTwo->cachedTextureObject.get();
		}
	};

	typedef std::set< TQ3CachedTexturePtr, CompByTexOb >		CachedTextureList;
	
}


// TQ3TextureCache: object holding cached textures for a number of GL contexts
// that share texture memory.  This declaration cannot be in an unnamed namespace,
// because it needs to match the struct TQ3TextureCache* member of the TQ3InteractiveData
// structure.
struct TQ3TextureCache : public CQ3GPSharedCache
{
	virtual		~TQ3TextureCache();

	CachedTextureList		cachedTextures;
};


//=============================================================================
//		Internal constants
//-----------------------------------------------------------------------------

namespace
{
	const TQ3Uns32	kTextureCacheKey	= Q3_FOUR_CHARACTER_CONSTANT('t', 'x', 'c', 'k');
}

//=============================================================================
//		Static variables
//-----------------------------------------------------------------------------



//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------

TQ3TextureCache::~TQ3TextureCache()
{
	for (CachedTextureList::iterator i = cachedTextures.begin();
		i != cachedTextures.end(); ++i)
	{
		delete *i;
	}
}

/*!
	@function	GetPixmapTextureStorage
	@abstract	Get the data from a pixmap texture object.
	@param		inTexture		A pixmap texture object.
	@result		A reference to image storage, or NULL on failure.
*/
static TQ3Object GetPixmapTextureStorage( TQ3TextureObject inTexture )
{
	TQ3Object	imageStorage = NULL;
	TQ3StoragePixmap	dataRec;
	if (kQ3Success == Q3PixmapTexture_GetPixmap( inTexture, &dataRec ))
	{
		imageStorage = dataRec.image;
	}
	return imageStorage;
}

/*!
	@function	GetMipmapTextureStorage
	@abstract	Get the data from a mipmap texture object.
	@param		inTexture		A mipmap texture object.
	@param		outData			Receives mipmap data.
	@result		Holds a reference to image storage, or NULL on failure.
*/
static TQ3Object GetMipmapTextureStorage( TQ3TextureObject inTexture )
{
	TQ3Object	imageStorage = NULL;
	TQ3Mipmap		dataRec;
	
	if (kQ3Success == Q3MipmapTexture_GetMipmap( inTexture, &dataRec ))
	{
		imageStorage = dataRec.image;
	}
	return imageStorage;
}

/*!
	@function	GetTextureStorage
	@abstract	Get the image storage from a texture object.
	@param		inTexture		A texture object.
	@result		A reference to image storage, or NULL on failure.
*/
static TQ3Object	GetTextureStorage( TQ3TextureObject inTexture )
{
	TQ3Object	imageStorage = NULL;
	
	switch (Q3Texture_GetType( inTexture ))
	{
		case kQ3TextureTypePixmap:
			imageStorage = GetPixmapTextureStorage( inTexture );
			break;
		
		case kQ3TextureTypeMipmap:
			imageStorage = GetMipmapTextureStorage( inTexture );
			break;
	}
	
	return imageStorage;
}

/*!
	@function	GetStorageEditIndex
	@abstract	Get the edit index of the storage object within a texture object.
	@param		inTexture		A texture object.
	@result		The edit index.
*/
static TQ3Uns32 GetStorageEditIndex( TQ3TextureObject inTexture )
{
	TQ3Uns32	editIndex = 0;
	CQ3ObjectRef	imageStorage( GetTextureStorage( inTexture ) );
	if (imageStorage.isvalid())
	{
		editIndex = Q3Shared_GetEditIndex( imageStorage.get() );
	}
	return editIndex;
}





/*!
	@function		RemoveCachedTexture
	@abstract		Remove a texture record from a texture cache.
	@param			txCache			A texture cache.
	@param			textureRec		Texture cache record to remove.
*/
static void			RemoveCachedTexture( TQ3TextureCachePtr txCache,
								CachedTextureList::iterator toRemove )
{
	TRY
	{
		TQ3CachedTexturePtr theRec = *toRemove;
		
		GLuint	textureName = GLTextureMgr_GetOpenGLTexture( theRec );
		Q3_ASSERT( glIsTexture( textureName ) );
		glDeleteTextures( 1, &textureName );
		Q3_ASSERT( !glIsTexture( textureName ) );
		
		txCache->cachedTextures.erase( toRemove );
		
		delete theRec;
	}
	CATCH_ALL
}


#if Q3_DEBUG
/*!
	@function		IsValidTextureCache
	@abstract		Test whether a texture cache pointer is valid.
	@param			txCache			A texture cache.
	@result			Whether the texture cache pointer is valid.
*/
static TQ3Boolean			IsValidTextureCache( TQ3TextureCachePtr txCache )
{
	TQ3Boolean	isValid = kQ3False;
	
	if (GLGPUSharing_IsCacheValid( txCache, kTextureCacheKey ))
	{
		isValid = kQ3True;
	}
	
	return isValid;
}
#endif



#pragma mark -
//=============================================================================
//		Public functions
//-----------------------------------------------------------------------------

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
	TQ3TextureCachePtr	theCache = static_cast<TQ3TextureCachePtr>(
		GLGPUSharing_GetCache( glContext, kTextureCacheKey ) );
	
	if (theCache == NULL)
	{
		TQ3TextureCache*	newCache = new(std::nothrow) TQ3TextureCache;
		
		if (newCache != NULL)
		{
			GLGPUSharing_AddCache( glContext, kTextureCacheKey, newCache );
			
			theCache = newCache;
		}
	}
	
	Q3_ASSERT( IsValidTextureCache( theCache ) );
	
	return theCache;
}





/*!
	@function		GLTextureMgr_FindCachedTexture
	@abstract		Access a texture cache record by matching the texture object.
	@discussion		This is the texture manager function that is called most often,
					so speed is important.
	@param			txCache			A texture cache.
	@param			texture			Reference to a texture object.
	@result			Pointer to a cached texture record, or NULL if not found.
*/
TQ3CachedTexturePtr	GLTextureMgr_FindCachedTexture( TQ3TextureCachePtr txCache,
								TQ3TextureObject texture )
{
	TQ3CachedTexturePtr	theRecord = NULL;
	
	TRY
	{
		TQ3CachedTexture	toFind;
		toFind.cachedTextureObject = CQ3ObjectRef( Q3Shared_GetReference( texture ) );
		
		CachedTextureList::iterator	foundIt = txCache->cachedTextures.find( &toFind );
		
		if (foundIt != txCache->cachedTextures.end())
		{
			theRecord = *foundIt;
		}

		// if we found a record, but it is stale, delete it.
		if (theRecord != NULL)
		{
			if ( (Q3Shared_GetEditIndex( texture ) != theRecord->editIndexTexture)
			||
				(GetStorageEditIndex( texture ) != theRecord->editIndexStorage) )
			{
				RemoveCachedTexture( txCache, foundIt );
				theRecord = NULL;
			}
		}
	}
	CATCH_ALL
	
	return theRecord;
}





/*!
	@function		GLTextureMgr_CacheTexture
	@abstract		Add a texture to the cache.
	@param			txCache			A texture cache.
	@param			inTexture		A Quesa texture object.
	@param			inGLTextureName	An OpenGL texture object name.
	@result			Pointer to a new cached texture record.
*/
TQ3CachedTexturePtr		GLTextureMgr_CacheTexture(
								TQ3TextureCachePtr txCache,
								TQ3TextureObject inTexture,
								GLuint inGLTextureName )
{
	TQ3CachedTexturePtr theResult = NULL;
	
	TRY
	{
		TQ3CachedTexture* newRec = new TQ3CachedTexture;
		
		newRec->cachedTextureObject = CQ3ObjectRef( Q3Shared_GetReference(
			inTexture ) );
		newRec->editIndexTexture = Q3Shared_GetEditIndex( inTexture );
		newRec->editIndexStorage = GetStorageEditIndex( inTexture );
		newRec->glTextureName = inGLTextureName;
		theResult = newRec;
		
		txCache->cachedTextures.insert( theResult );
	}
	CATCH_ALL
	
	return theResult;
}



/*!
	@function		GetOpenGLTexture
	@abstract		Access the OpenGL texture name from a cached texture.
	@param			inCachedTexture		A cached texture record.
	@result			An OpenGL texture name.
*/
GLuint	GLTextureMgr_GetOpenGLTexture( TQ3CachedTexturePtr inCachedTexture )
{
	return inCachedTexture->glTextureName;
}




/*!
	@function		GLTextureMgr_FlushUnreferencedTextures
	@abstract		Remove from the cache any textures that are not referenced
					except by the texture cache itself.
	@param			txCache			A texture cache.
*/
void				GLTextureMgr_FlushUnreferencedTextures(
								TQ3TextureCachePtr txCache )
{
	TQ3Uns32	textureIndex = 0;
	CachedTextureList::iterator iter = txCache->cachedTextures.begin();
	while (iter != txCache->cachedTextures.end())
	{
		CachedTextureList::iterator nextIter = iter;
		++nextIter;
		
		TQ3CachedTexturePtr cachedTexture = *iter;
		
		// If we hold the last reference to this texture, release it
		TQ3TextureObject	theTexture = cachedTexture->cachedTextureObject.get();
		
		if ( (theTexture != NULL) && ! Q3Shared_IsReferenced( theTexture ) )
		{
			RemoveCachedTexture( txCache, iter );
		}

		iter = nextIter;
	}
}

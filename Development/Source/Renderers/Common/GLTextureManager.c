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
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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

// Cached texture data
struct TQ3CachedTexture
{
	CQ3ObjectRef			cachedTextureObject;
	TQ3Uns32				editIndexTexture;
	TQ3Uns32				editIndexStorage;
	GLuint					glTextureName;
};


// TQ3TextureCache: object holding cached textures for a number of GL contexts
// that share texture memory.  This declaration cannot be in an unnamed namespace,
// because it needs to match the struct TQ3TextureCache* member of the TQ3InteractiveData
// structure.
struct TQ3TextureCache : public CQ3GPSharedCache
{
	CachedTextureList		cachedTextures;
};

namespace
{


	class MatchTexture	// function object for use with find_if and CachedTextureList
	{
	public:
							MatchTexture( TQ3TextureObject inTexture )
									: mTextureToMatch( inTexture ) {}
							MatchTexture( const MatchTexture& inOther )
									: mTextureToMatch( inOther.mTextureToMatch ) {}

		bool				operator()( const TQ3CachedTexture& inCachedTexture ) const
									{
										return (GLTextureMgr_GetQuesaTexture(
											&inCachedTexture ) ==
											mTextureToMatch);
									}
	private:
		TQ3TextureObject	mTextureToMatch;
	};

}

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

/*!
	@function	GetPixmapTextureData
	@abstract	Get the data from a pixmap texture object.
	@param		inTexture		A pixmap texture object.
	@param		outData			Receives pixmap data.
	@result		Holds a reference to outData.image, or NULL on failure.
*/
static CQ3ObjectRef GetPixmapTextureData( TQ3TextureObject inTexture,
											TQ3StoragePixmap& outData )
{
	CQ3ObjectRef	imageHolder;
	if (kQ3Success == Q3PixmapTexture_GetPixmap( inTexture, &outData ))
	{
		imageHolder = CQ3ObjectRef( outData.image );
	}
	return imageHolder;
}

/*!
	@function	GetMipmapTextureData
	@abstract	Get the data from a mipmap texture object.
	@param		inTexture		A mipmap texture object.
	@param		outData			Receives mipmap data.
	@result		Holds a reference to outData.image, or NULL on failure.
*/
static CQ3ObjectRef GetMipmapTextureData( TQ3TextureObject inTexture,
											TQ3Mipmap& outData )
{
	CQ3ObjectRef	imageHolder;
	if (kQ3Success == Q3MipmapTexture_GetMipmap( inTexture, &outData ))
	{
		imageHolder = CQ3ObjectRef( outData.image );
	}
	return imageHolder;
}

/*!
	@function	GetTextureStorage
	@abstract	Get the image storage from a texture object.
	@param		inTexture		A texture object.
	@result		A reference to image storage, or NULL on failure.
*/
static CQ3ObjectRef	GetTextureStorage( TQ3TextureObject inTexture )
{
	CQ3ObjectRef	imageHolder;
	
	switch (Q3Texture_GetType( inTexture ))
	{
		case kQ3TextureTypePixmap:
			{
				TQ3StoragePixmap	thePixmap;
				imageHolder = GetPixmapTextureData( inTexture, thePixmap );
			}
			break;
		
		case kQ3TextureTypeMipmap:
			{
				TQ3Mipmap	theMipmap;
				imageHolder = GetMipmapTextureData( inTexture, theMipmap );
			}
			break;
	}
	
	return imageHolder;
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
	
	if (GLGPUSharing_IsCacheValid( txCache, kTextureCacheKey ))
	{
		isValid = kQ3True;
	}
	
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
TQ3CachedTexturePtr	GLTextureMgr_FindCachedTexture( TQ3TextureCachePtr txCache,
								TQ3TextureObject texture )
{
	TQ3CachedTexturePtr	theRecord = NULL;
	
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
TQ3CachedTexturePtr	GLTextureMgr_GetCachedTextureByIndex( TQ3TextureCachePtr txCache,
								TQ3Uns32 memberIndex )
{
	TQ3CachedTexturePtr	theRecord = NULL;
	
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
	@function		GLTextureMgr_CacheTexture
	@abstract		Add a texture to the cache.
	@param			txCache			A texture cache.
	@param			inTexture		A Quesa texture object.
	@param			inGLTextureName	An OpenGL texture object name.
*/
void				GLTextureMgr_CacheTexture(
								TQ3TextureCachePtr txCache,
								TQ3TextureObject inTexture,
								GLuint inGLTextureName )
{
	TQ3CachedTexture	localRec;
	localRec.cachedTextureObject = CQ3ObjectRef( Q3Shared_GetReference(
		inTexture ) );
	localRec.editIndexTexture = Q3Shared_GetEditIndex( inTexture );
	localRec.editIndexStorage = GetStorageEditIndex( inTexture );
	localRec.glTextureName = inGLTextureName;
	
	TRY
	{
		txCache->cachedTextures.push_back( localRec );
	}
	CATCH_ALL
}


/*!
	@function		GLTextureMgr_RemoveCachedTexture
	@abstract		Remove a texture record from a texture cache.
	@param			txCache			A texture cache.
	@param			textureRec		Texture cache record to remove.
*/
void				GLTextureMgr_RemoveCachedTexture( TQ3TextureCachePtr txCache,
								TQ3CachedTexturePtr textureRec )
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


/*!
	@function		GLTextureMgr_IsCachedTextureStale
	@abstract		Determine whether the texture cache contains this texture,
					but the texture has been modified since it was cached.
	@param			txCache			A texture cache.
	@param			inTexture		A texture object.
	@result			True if the texture is in the cache but stale.
*/
TQ3Boolean				GLTextureMgr_IsCachedTextureStale(
								TQ3TextureCachePtr txCache,
								TQ3TextureObject inTexture )
{
	TQ3Boolean	isStale = kQ3False;
	
	TQ3CachedTexturePtr	cacheRec = GLTextureMgr_FindCachedTexture(
		txCache, inTexture );
	
	if (cacheRec != NULL)
	{
		if ( (Q3Shared_GetEditIndex( inTexture ) != cacheRec->editIndexTexture)
		||
			(GetStorageEditIndex( inTexture ) != cacheRec->editIndexStorage) )
		{
			isStale = kQ3True;
		}
	}
	
	return isStale;
}

/*!
	@function		GetQuesaTexture
	@abstract		Access the Quesa texture object from a cached texture.
	@param			inCachedTexture		A cached texture record.
	@result			A texture object.
*/
TQ3TextureObject	GLTextureMgr_GetQuesaTexture(
								TQ3CachedTexturePtr inCachedTexture )
{
	return inCachedTexture->cachedTextureObject.get();
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


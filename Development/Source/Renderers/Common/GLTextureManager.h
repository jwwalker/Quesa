/*  NAME:
        GLTextureManager.h

    DESCRIPTION:
        Header file for GLTextureManager.c.

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
#ifndef GLTEXTUREMANAGER_HDR
#define GLTEXTUREMANAGER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "GLPrefix.h"
#include "CQ3ObjectRef.h"



//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif



//=============================================================================
//      Types
//-----------------------------------------------------------------------------


// Cached texture record opaque pointer
typedef const struct TQ3CachedTexture*	TQ3CachedTexturePtr;


// Texture cache opaque pointer
typedef struct TQ3TextureCache*	TQ3TextureCachePtr;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------


/*!
	@function		GLTextureMgr_GetTextureCache
	@abstract		After a GL context has been recorded in the texture manager,
					this function can be used to look up the corresponding
					texture cache.
	@param			glContext			A GL context.
	@result			Pointer to a texture cache.
*/
TQ3TextureCachePtr	GLTextureMgr_GetTextureCache( TQ3GLContext glContext );

/*!
	@function		GLTextureMgr_FindCachedTexture
	@abstract		Access a texture cache record by matching the texture object.
	@param			txCache			A texture cache.
	@param			texture			Reference to a texture object.
	@result			Pointer to a cached texture record, or NULL if not found.
*/
TQ3CachedTexturePtr	GLTextureMgr_FindCachedTexture(
								TQ3TextureCachePtr txCache,
								TQ3TextureObject texture );

/*!
	@function		GLTextureMgr_GetCachedTextureByIndex
	@abstract		Access a texture cache record by index.
	@param			txCache			A texture cache.
	@param			memberIndex		Zero-based index of a cached texture.
	@result			Pointer to a cached texture record, or NULL if not found.
*/
TQ3CachedTexturePtr	GLTextureMgr_GetCachedTextureByIndex(
								TQ3TextureCachePtr txCache,
								TQ3Uns32 memberIndex );

/*!
	@function		GLTextureMgr_RemoveCachedTexture
	@abstract		Remove a texture record from a texture cache.
	@param			txCache			A texture cache.
	@param			textureRec		Texture cache record to remove.
*/
void				GLTextureMgr_RemoveCachedTexture(
								TQ3TextureCachePtr txCache,
								TQ3CachedTexturePtr textureRec );


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
								GLuint inGLTextureName );

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
								TQ3TextureObject inTexture );

#if Q3_DEBUG
/*!
	@function		GLTextureMgr_IsValidTextureCache
	@abstract		Test whether a texture cache pointer is valid.
	@param			txCache			A texture cache.
	@result			Whether the texture cache pointer is valid.
*/
TQ3Boolean			GLTextureMgr_IsValidTextureCache( TQ3TextureCachePtr txCache );
#endif


/*!
	@function		GetQuesaTexture
	@abstract		Access the Quesa texture object from a cached texture.
	@param			inCachedTexture		A cached texture record.
	@result			A texture object.
*/
TQ3TextureObject	GLTextureMgr_GetQuesaTexture( TQ3CachedTexturePtr inCachedTexture );
	
/*!
	@function		GetOpenGLTexture
	@abstract		Access the OpenGL texture name from a cached texture.
	@param			inCachedTexture		A cached texture record.
	@result			An OpenGL texture name.
*/
GLuint		GLTextureMgr_GetOpenGLTexture( TQ3CachedTexturePtr inCachedTexture );



//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif


#endif

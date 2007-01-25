/*  NAME:
        GLTextureManager.h

    DESCRIPTION:
        Header file for GLTextureManager.c.

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

// Cached texture data
struct TQ3CachedTexture
{
	CQ3ObjectRef			cachedTextureObject;
	TQ3Uns32				editIndexTexture;
	TQ3Uns32				editIndexStorage;
	GLuint					glTextureName;
};


// Texture cache opaque pointer
typedef struct TQ3TextureCache*	TQ3TextureCachePtr;


// Opaque pointer to a GL context.  What it points to is platform-dependent.
typedef	void*				TQ3GLContext;





//=============================================================================
//      Function prototypes
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
TQ3GLContext		GLTextureMgr_GetNextSharingBase( TQ3GLContext glBase );


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
								TQ3GLContext sharingBase );


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
	@function		GLTextureMgr_RemoveContext
	@abstract		Inform the texture manager that a GL context has been destroyed.
	@discussion		The given GL context will be forgotten from its texture cache.
					If there are no more contexts associated with the texture cache,
					then the texture cache will be disposed.
	@param			deadGLContext		A former GL context.
*/
void				GLTextureMgr_RemoveContext( TQ3GLContext deadGLContext );


/*!
	@function		GLTextureMgr_FindCachedTexture
	@abstract		Access a texture cache record by matching the texture object.
	@param			txCache			A texture cache.
	@param			texture			Reference to a texture object.
	@result			Pointer to a cached texture record, or NULL if not found.
*/
TQ3CachedTexture* GLTextureMgr_FindCachedTexture( TQ3TextureCachePtr txCache,
								TQ3TextureObject texture );

/*!
	@function		GLTextureMgr_GetCachedTextureByIndex
	@abstract		Access a texture cache record by index.
	@param			txCache			A texture cache.
	@param			memberIndex		Zero-based index of a cached texture.
	@result			Pointer to a cached texture record, or NULL if not found.
*/
TQ3CachedTexture*	GLTextureMgr_GetCachedTextureByIndex( TQ3TextureCachePtr txCache,
								TQ3Uns32 memberIndex );

/*!
	@function		GLTextureMgr_RemoveCachedTexture
	@abstract		Remove a texture record from a texture cache.
	@param			txCache			A texture cache.
	@param			textureRec		Texture cache record to remove.
*/
void				GLTextureMgr_RemoveCachedTexture( TQ3TextureCachePtr txCache,
								TQ3CachedTexture* textureRec );

/*!
	@function		GLTextureMgr_AddCachedTexture
	@abstract		Append a texture record to the texture cache.
	@param			txCache			A texture cache.
	@param			textureRec		A new texture record to cache.
	@result			Success or failure of the operation.
*/
TQ3Status			GLTextureMgr_AddCachedTexture( TQ3TextureCachePtr txCache,
								struct TQ3CachedTexture* textureRec );


#if Q3_DEBUG
/*!
	@function		GLTextureMgr_IsValidTextureCache
	@abstract		Test whether a texture cache pointer is valid.
	@param			txCache			A texture cache.
	@result			Whether the texture cache pointer is valid.
*/
TQ3Boolean			GLTextureMgr_IsValidTextureCache( TQ3TextureCachePtr txCache );
#endif


//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif



#endif

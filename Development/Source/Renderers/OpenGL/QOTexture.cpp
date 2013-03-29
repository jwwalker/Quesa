/*  NAME:
       QOTexture.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2013, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://quesa.sourceforge.net/>
        
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

#include "QOTexture.h"
#include "GLDrawContext.h"
#include "GLTextureLoader.h"
#include "GLUtils.h"
#include "E3Shader.h"
#include "QuesaCustomElements.h"
#include "QOGLShadingLanguage.h"

#include <algorithm>

using namespace QORenderer;

// In lieu of glext.h
#ifndef GL_ARB_multitexture
	#define GL_TEXTURE0_ARB                   0x84C0
	#define GL_TEXTURE1_ARB                   0x84C1
#endif



//=============================================================================
//      Local functions
//-----------------------------------------------------------------------------


static bool GetPixmapTextureData( TQ3TextureObject inTexture,
								TQ3StoragePixmap& outData,
								CQ3ObjectRef& outStorageHolder )
{
	TQ3Status	theStatus = Q3PixmapTexture_GetPixmap( inTexture, &outData );
	if (theStatus == kQ3Success)
	{
		outStorageHolder = CQ3ObjectRef( outData.image );
	}
	return (theStatus == kQ3Success);
}

static bool GetMipmapTextureData( TQ3TextureObject inTexture,
								TQ3Mipmap& outData,
								CQ3ObjectRef& outStorageHolder )
{
	TQ3Status	theStatus = Q3MipmapTexture_GetMipmap( inTexture, &outData );
	if (theStatus == kQ3Success)
	{
		outStorageHolder = CQ3ObjectRef( outData.image );
	}
	return (theStatus == kQ3Success);
}


static TQ3PixelType	GetTexturePixelType( TQ3TextureObject inTexture )
{
	TQ3PixelType	thePixelType = kQ3PixelTypeUnknown;
	CQ3ObjectRef	storageHolder;
	
	switch (Q3Texture_GetType( inTexture ))
	{
		case kQ3TextureTypePixmap:
			{
				TQ3StoragePixmap	thePixmap;
				if (GetPixmapTextureData( inTexture, thePixmap,
					storageHolder ))
				{
					thePixelType = thePixmap.pixelType;
				}
			}
			break;
		
		case kQ3TextureTypeMipmap:
			{
				TQ3Mipmap			theMipmap;
				if (GetMipmapTextureData( inTexture, theMipmap,
					storageHolder ))
				{
					thePixelType = theMipmap.pixelType;
				}
			}
			break;
		
		default:
			thePixelType = kQ3PixelTypeUnknown;
			break;
	}
	
	return thePixelType;
}

/*!
	@function		IsTextureMipmapped
	
	@abstract		Test whether a texture will use mipmapping.
	
	@discussion		In Quesa, pixmap textures automatically use mipmapping.
					However, a mipmap texture can either provide a full set
					of mipmap textures, or can provide one and say that
					mipmapping is not to be used.
*/
static bool IsTextureMipmapped( TQ3TextureObject inTexture )
{
	bool	isMipmapped = false;
	
	switch (Q3Texture_GetType( inTexture ))
	{
		case kQ3TextureTypePixmap:
			isMipmapped = true;	// automatic
			break;
			
		case kQ3TextureTypeMipmap:
			{
				TQ3Mipmap			theMipmap;
				CQ3ObjectRef	storageHolder;
				if (GetMipmapTextureData( inTexture, theMipmap,
					storageHolder ))
				{
					isMipmapped = (theMipmap.useMipmapping == kQ3True);
				}
			}
			break;
	}
	
	return isMipmapped;
}


#pragma mark -

//=============================================================================
//       Method implementations
//-----------------------------------------------------------------------------

Texture::Texture(
									TQ3RendererObject inRenderer,
									const TQ3GLContext& inGLContext,
									const TQ3GLExtensions& inExtensions,
									PerPixelLighting& ioPPLighting )
	: mRenderer( inRenderer )
	, mGLContext( inGLContext )
	, mGLExtensions( inExtensions )
	, mPPLighting( ioPPLighting )
	, mTextureCache( NULL )
	, mPendingTextureRemoval( true )
	, mGLActiveTexture( NULL )
{
	mState.Reset();
}

Texture::~Texture()
{
	FlushCache();
}

void	Texture::FlushCache()
{
	if (mGLContext != NULL)
	{
		GLDrawContext_SetCurrent( mGLContext, kQ3False );
		
		GLTextureMgr_FlushUnreferencedTextures( mTextureCache );
	}
}




void	Texture::SetOpenGLTextureFiltering(
								bool isMipmapped )
{
	TQ3TextureFilter	raveFilter;
	Q3InteractiveRenderer_GetRAVETextureFilter( mRenderer, &raveFilter );
	
	GLuint	magFilter, minFilter;
	
	switch (raveFilter)
	{
		case kQATextureFilter_Fast:
			magFilter = GL_NEAREST;
			minFilter = GL_NEAREST;
			break;
			
		case kQATextureFilter_Best:
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR_MIPMAP_LINEAR;
			break;
		
		default:// kQATextureFilter_Mid
			magFilter = GL_LINEAR;
			minFilter = GL_LINEAR_MIPMAP_NEAREST;
			break;
	}
	
	glTexParameteri( GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, magFilter );
	
	if (isMipmapped)
	{
		glTexParameteri( GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, minFilter );
	}
	else
	{
		glTexParameteri( GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, magFilter );
	}
}

void	Texture::GetShaderParams(
								TQ3ShaderObject inShader,
								TQ3ShaderUVBoundary& outShaderUBoundary,
								TQ3ShaderUVBoundary& outShaderVBoundary,
								TQ3Matrix3x3& outShaderUVTransform,
								bool& outUseAlphaTest,
								TQ3Float32& outAlphaTestThreshold )
{
	E3Shader* theShader = (E3Shader*) inShader;
	theShader->GetUBoundary( &outShaderUBoundary );
	theShader->GetVBoundary( &outShaderVBoundary );
	theShader->GetUVTransform( &outShaderUVTransform );
	
	TQ3Status hasElement = Q3Object_GetElement( inShader,
		kQ3ElementTypeTextureShaderAlphaTest, &outAlphaTestThreshold );
	outUseAlphaTest = (hasElement == kQ3Success);
}

void	Texture::SetOpenGLTexturingParameters()
{
	SetOpenGLTextureFiltering( mState.mIsTextureMipmapped );
	
	// boundary behavior
	GLint		glBoundsU, glBoundsV;
	GLUtils_ConvertUVBoundary( mState.mShaderUBoundary, &glBoundsU,
		mGLExtensions.clampToEdge );
	GLUtils_ConvertUVBoundary( mState.mShaderVBoundary, &glBoundsV,
		mGLExtensions.clampToEdge );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glBoundsU );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glBoundsV );
	
	
	// UV transform
	GLUtils_LoadShaderUVTransform( &mState.mUVTransform );
	
	
	// Alpha test
	if (mState.mIsTextureAlphaTest)
	{
		glEnable( GL_ALPHA_TEST );
		glAlphaFunc( GL_GREATER, mState.mAlphaTestThreshold );
	}
	else
	{
		glDisable( GL_ALPHA_TEST );
	}
}

void Texture::TextureState::Reset()
{
	mIsTextureActive = false;
	mIsTextureTransparent = false;
	mIsTextureMipmapped = false;
	mIsTextureAlphaTest = false;
	mAlphaTestThreshold = 0.5f;
	mGLTextureObject = 0;
}

/*!
	@function			UpdateTextureCache
	@abstract			Called by QORenderer when the GL context is
						(re)built.
*/
void	Texture::UpdateTextureCache()
{
	mTextureCache = GLTextureMgr_GetTextureCache( mGLContext );
}


/*!
	@function			GetTextureState
	@abstract			Get the current texture state.  May be used for
						handling transparent triangles.
*/
const Texture::TextureState&		Texture::GetTextureState() const
{
	return mState;
}


/*!
	@function			StartPass
	@abstract			Called by QORenderer at start of a pass for
						initialization.
*/
void	Texture::StartPass()
{
	mState.Reset();
	mPendingTextureRemoval = true;
	GLGetProcAddress( mGLActiveTexture, "glActiveTexture", "glActiveTextureARB" );
}


/*!
	@function			EndPass
	@abstract			Called by QORenderer at end of a pass for cleanup.
*/
void	Texture::EndPass()
{
	FlushCache();
}


/*!
	@function	CacheTexture
	@abstract	Add a texture to the cache.
*/
TQ3CachedTexturePtr		Texture::CacheTexture( TQ3TextureObject inTexture )
{
	TQ3CachedTexturePtr	cacheRec = NULL;
	TQ3Boolean	convertAlpha = kQ3False;
	
	Q3Object_GetProperty( mRenderer, kQ3RendererPropertyConvertToPremultipliedAlpha,
		sizeof(convertAlpha), NULL, &convertAlpha );
	
	GLuint	textureName = GLTextureLoader( inTexture, convertAlpha );
	
	if (textureName != 0)
	{
		cacheRec = GLTextureMgr_CacheTexture( mTextureCache, inTexture, textureName );
	}
	
	return cacheRec;
}

/*!
	@function			HandlePendingTextureRemoval
	@abstract			If there should be no active texture, tell OpenGL
						about it, just in time for rendering.
*/
void	Texture::HandlePendingTextureRemoval()
{
	if (mPendingTextureRemoval)
	{
		GLDrawContext_SetCurrent( mGLContext, kQ3False );
		
		glDisable( GL_TEXTURE_2D );
		glDisable( GL_ALPHA_TEST );
		glBindTexture( GL_TEXTURE_2D, 0 );
		glMatrixMode( GL_TEXTURE );
		glLoadIdentity();
		
		(*mGLActiveTexture)( GL_TEXTURE1_ARB );
		glBindTexture( GL_TEXTURE_2D, 0 );
		(*mGLActiveTexture)( GL_TEXTURE0_ARB );
		mPPLighting.UpdateSpecularMapping( false );
		
		mPendingTextureRemoval = false;
	}
}

/*!
	@function			SetCurrentTexture
	@abstract			Activate a texture.
*/
void	Texture::SetCurrentTexture(
								TQ3TextureObject inTexture,
								TQ3ShaderObject inShader )
{
	if (inTexture == NULL)	// disable texturing
	{
		mState.mIsTextureActive = false;
		mPendingTextureRemoval = true;
	}
	else	// enable texturing
	{
		// Activate our context
		GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	
		// Put it in the cache if need be
		TQ3CachedTexturePtr	cachedTexture = GLTextureMgr_FindCachedTexture(
			mTextureCache, inTexture );
		if (cachedTexture == NULL)
		{
			cachedTexture = CacheTexture( inTexture );
		}
		
		if (cachedTexture != NULL)
		{
			mState.mIsTextureActive = true;
			
			GetShaderParams( inShader, mState.mShaderUBoundary,
				mState.mShaderVBoundary, mState.mUVTransform,
				mState.mIsTextureAlphaTest, mState.mAlphaTestThreshold );
			
			mState.mGLTextureObject = GLTextureMgr_GetOpenGLTexture(
				cachedTexture );
			TQ3PixelType	pixelType = GetTexturePixelType( inTexture );
			mState.mIsTextureTransparent = (! mState.mIsTextureAlphaTest) &&
				((pixelType == kQ3PixelTypeARGB32) ||
				(pixelType == kQ3PixelTypeARGB16));
			mState.mIsTextureMipmapped = IsTextureMipmapped( inTexture );
			
			glEnable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, mState.mGLTextureObject );
			
			SetOpenGLTexturingParameters();
			mPendingTextureRemoval = false;
			SetSpecularMap( inShader );
		}
	}
}



/*!
	@function	SetSpecularMap
	@abstract	If the shader has a Specular map attached to it, set it up
				as a texture on the second texture unit.
*/
void	Texture::SetSpecularMap( TQ3ShaderObject inShader )
{
	if (inShader != NULL)
	{
		CQ3ObjectRef shininessTexture( CESpecularMapElement_Copy( inShader ) );
		if (shininessTexture.isvalid())
		{
			TQ3CachedTexturePtr	cachedTexture = GLTextureMgr_FindCachedTexture(
				mTextureCache, shininessTexture.get() );
			if (cachedTexture == NULL)
			{
				(*mGLActiveTexture)( GL_TEXTURE1_ARB );
				cachedTexture = CacheTexture( shininessTexture.get() );
			}
			if (cachedTexture != NULL)
			{
				GLuint textureName = GLTextureMgr_GetOpenGLTexture( cachedTexture );
				(*mGLActiveTexture)( GL_TEXTURE1_ARB );
				// Note: a fragment shader can ignore the enabled state of a
				// texture unit.  We disable this one in case per-pixel lighting
				// is not running, so as not to disrupt the fixed-function pipeline.
				glDisable( GL_TEXTURE_2D );
				glBindTexture( GL_TEXTURE_2D, textureName );
				SetOpenGLTexturingParameters();
				(*mGLActiveTexture)( GL_TEXTURE0_ARB );
				mPPLighting.UpdateSpecularMapping( true );
			}
		}
	}
}


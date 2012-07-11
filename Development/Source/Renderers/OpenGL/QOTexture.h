/*  NAME:
       QOTexture.h

    DESCRIPTION:
        Header for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2012, Quesa Developers. All rights reserved.

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

#include "E3Prefix.h"
#include "GLPrefix.h"
#include "GLTextureManager.h"

#include <vector>


//=============================================================================
//      Class declaration
//-----------------------------------------------------------------------------


namespace QORenderer
{

/*!
	@class		Texture
	
	@abstract	Texture handling functions for QORenderer.
*/
class Texture
{
public:

	struct TextureState
	{
		void				Reset();
		
		bool				mIsTextureActive;
		bool				mIsTextureTransparent;
		bool				mIsTextureMipmapped;
		bool				mIsTextureAlphaTest;
		TQ3Float32			mAlphaTestThreshold;
		GLuint				mGLTextureObject;
		TQ3ShaderUVBoundary	mShaderUBoundary;
		TQ3ShaderUVBoundary	mShaderVBoundary;
		TQ3Matrix3x3		mUVTransform;
	};
	
							Texture(
									TQ3RendererObject inRenderer,
									const TQ3GLContext& inGLContext,
									const TQ3GLExtensions& inExtensions );
							
							~Texture();
	
	/*!
		@function			UpdateTextureCache
		@abstract			Called by QORenderer when the GL context is
							(re)built.
	*/
	void					UpdateTextureCache();
	
	/*!
		@function			SetCurrentTexture
		@abstract			Activate a texture.
	*/
	void					SetCurrentTexture(
									TQ3TextureObject inTexture,
									TQ3ShaderObject inShader );
	
	/*!
		@function			IsTextureActive
		@abstract			Tell whether a texture is active.
	*/
	inline
	bool					IsTextureActive() const
							{
								return mIsTopActive;
							}

	/*!
		@function			IsTextureTransparent
		@abstract			Tell whether a transparent texture is active.
	*/
	inline
	bool					IsTextureTransparent() const
							{
								return mIsTopTransparent && mIsTopActive;
							}
	
	/*!
		@function			GetTextureState
		@abstract			Get the current texture state.  May be used for
							handling transparent triangles.
	*/
	const TextureState&		GetTextureState() const;
	
	/*!
		@function			StartPass
		@abstract			Called by QORenderer at start of a pass for
							initialization.
	*/
	void					StartPass();
	
	/*!
		@function			EndPass
		@abstract			Called by QORenderer at end of a pass for cleanup.
	*/
	void					EndPass();

	
	
private:
	TQ3CachedTexturePtr		CacheTexture( TQ3TextureObject inTexture );
	void					SetOpenGLTexturingParameters();
	void					SetOpenGLTextureFiltering(
									bool isMipmapped );
	void					GetShaderParams(
									TQ3ShaderObject inShader,
									TQ3ShaderUVBoundary& outShaderUBoundary,
									TQ3ShaderUVBoundary& outShaderVBoundary,
									TQ3Matrix3x3& outShaderUVTransform,
									bool& outUseAlphaTest,
									TQ3Float32& outAlphaTestThreshold );
	void					FlushCache();

	TQ3RendererObject		mRenderer;
	const TQ3GLContext&		mGLContext;
	const TQ3GLExtensions&	mGLExtensions;
	TQ3TextureCachePtr		mTextureCache;
	TextureState			mState;
	std::vector<TQ3Uns8>	mSrcImageData;
	std::vector<GLubyte>	mGLFormatWork;
	bool					mIsTopActive;
	bool					mIsTopTransparent;
};

}

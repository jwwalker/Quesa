/*  NAME:
       QORenderer.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2018, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#include "QORenderer.h"

#include "GLDrawContext.h"
#include "GLUtils.h"
#include "E3Math_Intersect.h"



//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
namespace
{
	const TQ3ColorRGB	kDefaultDiffuseColor = { 1.0f, 1.0f, 1.0f };
	const TQ3ColorRGB	kDefaultSpecularColor = { 0.5f, 0.5f, 0.5f };
	const float			kDefaultSpecularControl = 4.0f;
	const float			kDefaultAlpha = 1.0f;
	const TQ3ColorRGB	kDefaultEmissiveColor = { 0.0f, 0.0f, 0.0f };
	
	const TQ3Uns32		kDefaultDepthBits	= 24;
}


//=============================================================================
//     Subsidiary Class Implementations
//-----------------------------------------------------------------------------
void	QORenderer::ColorState::Reset()
{
	diffuseColor = &kDefaultDiffuseColor;
	specularColor = &kDefaultSpecularColor;
	specularControl = kDefaultSpecularControl;
	emissiveColor = &kDefaultEmissiveColor;
	alpha = kDefaultAlpha;
	highlightState = kQ3Off;
}

QORenderer::GLContextCleanup::~GLContextCleanup()
{
	if (mContext != nullptr)
	{
		GLDrawContext_Destroy( &mContext );
	}
}

QORenderer::GLFuncs::GLFuncs()
	: glStencilFuncSeparate( nullptr )
	, glStencilOpSeparate( nullptr )
	, glStencilMaskSeparate( nullptr )
	, glGenBuffersProc( nullptr )
	, glBindBufferProc( nullptr )
	, glDeleteBuffersProc( nullptr )
	, glIsBufferProc( nullptr )
	, glBufferDataProc( nullptr )
	, glBufferSubDataProc( nullptr )
	, glGetBufferParameterivProc( nullptr )
	, glGenerateMipmapProc( nullptr )
	, glActiveTexture( nullptr )
{
}

/*!
	@function	Initialize
	@abstract	Get the function pointers.  This should be called just
				after the OpenGL context is created.
*/
void	QORenderer::GLFuncs::Initialize( const TQ3GLExtensions& inExts )
{
	// Stencil
	GLGetProcAddress( glStencilFuncSeparate, "glStencilFuncSeparate" );
	GLGetProcAddress( glStencilOpSeparate, "glStencilOpSeparate" );
	GLGetProcAddress( glStencilMaskSeparate, "glStencilMaskSeparate" );

	// VBO
	GLGetProcAddress( glGenBuffersProc, "glGenBuffers", "glGenBuffersARB" );
	GLGetProcAddress( glBindBufferProc, "glBindBuffer", "glBindBufferARB" );
	GLGetProcAddress( glDeleteBuffersProc, "glDeleteBuffers", "glDeleteBuffersARB" );
	GLGetProcAddress( glIsBufferProc, "glIsBuffer", "glIsBufferARB" );
	GLGetProcAddress( glBufferDataProc, "glBufferData", "glBufferDataARB" );
	GLGetProcAddress( glBufferSubDataProc, "glBufferSubData", "glBufferSubDataARB" );
	GLGetProcAddress( glGetBufferParameterivProc, "glGetBufferParameteriv",
		"glGetBufferParameterivARB" );
	
	// Misc
	GLGetProcAddress( glGenerateMipmapProc, "glGenerateMipmap" );
	GLGetProcAddress( glActiveTexture, "glActiveTexture", "glActiveTextureARB" );
	GLGetProcAddress( glGenVertexArrays, "glGenVertexArrays" );
	GLGetProcAddress( glBindVertexArray, "glBindVertexArray" );
	
	// Fatal assertion failure if we fail to get any function
	Q3_ASSERT( glStencilFuncSeparate != nullptr );
	Q3_ASSERT( glStencilOpSeparate != nullptr );
	Q3_ASSERT( glGenBuffersProc != nullptr );
	
	Q3_ASSERT( glBindBufferProc != nullptr );
	Q3_ASSERT( glDeleteBuffersProc != nullptr );
	Q3_ASSERT( glIsBufferProc != nullptr );
	Q3_ASSERT( glBufferDataProc != nullptr );
	Q3_ASSERT( glBufferSubDataProc != nullptr );
	Q3_ASSERT( glGetBufferParameterivProc != nullptr );
	Q3_ASSERT( glStencilFuncSeparate != nullptr );
	
	Q3_ASSERT( glGenerateMipmapProc != nullptr );
	Q3_ASSERT( glActiveTexture != nullptr );
	Q3_ASSERT( glGenVertexArrays != nullptr );
	Q3_ASSERT( glBindVertexArray != nullptr );
}

/*!
	@function	InitializeForBufferDelete
	@abstract	Get just the functions needed to delete VBOs.
*/
void	QORenderer::GLFuncs::InitializeForBufferDelete()
{
	GLGetProcAddress( glDeleteBuffersProc, "glDeleteBuffers", "glDeleteBuffersARB" );
	GLGetProcAddress( glIsBufferProc, "glIsBuffer", "glIsBufferARB" );
}



#pragma mark -

#ifdef _MSC_VER
	// Visual Studio complains about passing 'this' to the mTriBuffer and
	// mTriBuffer objects before the renderer object is fully constructed.
	// But since those objects don't immediately do anything with the
	// reference to the renderer other than save it, it's safe.
	#pragma warning(disable: 4355)
#endif

//=============================================================================
//     Main Class Constructor/destructor Implementations
//-----------------------------------------------------------------------------

QORenderer::Renderer::Renderer( TQ3RendererObject inRenderer )
	: mRendererObject( inRenderer )
	, mDrawContextObject( nullptr )
	, mGLContext( nullptr )
	, mCleanup( mGLContext )
	, mSLFuncs()
	, mFuncs()
	, mPPLighting( *this, mSLFuncs, mRendererObject, mGLContext, mGLExtensions )
	, mRendererEditIndex( Q3Shared_GetEditIndex( inRenderer ) )
	, mDrawContextEditIndex( 0 )
	, mGLClearFlags( 0 )
	, mPassIndex( 0 )
	, mNumPasses( 1 )
	, mAllowLineSmooth( true )
	, mIsCachingShadows( false )
	, mNumPrimitivesRenderedInFrame( 0 )
	, mLineWidth( 1.0f )
	, mAttributesMask( kQ3XAttributeMaskAll )
	, mUpdateShader( true )
	, mGLClientStates( mSLFuncs, mPPLighting )
	, mLights( *this )
	, mTriBuffer( *this )
	, mTransBuffer( *this, mPPLighting )
	, mTextures( *this )
{
	Q3Object_AddElement( mRendererObject, kQ3ElementTypeDepthBits,
		&kDefaultDepthBits );
	
	Q3InteractiveRenderer_SetRAVETextureFilter( mRendererObject,
		kQATextureFilter_Mid );
}

QORenderer::Renderer::~Renderer()
{
	if (mGLContext != nullptr)
	{
		GLDrawContext_SetCurrent( mGLContext, kQ3False );
	}
}

bool	QORenderer::Renderer::IsBoundingBoxVisible(
								TQ3ViewObject inView,
								const TQ3BoundingBox& inBounds )
{
	bool	isVisible = false;
	
	if ( (kQ3False == inBounds.isEmpty) && mLights.IsLit( inBounds ) )
	{
		if (mLights.IsShadowMarkingPass())
		{
			// Find the position of the light (world space).
			TQ3RationalPoint4D lightPos( mLights.GetShadowingLightPosition() );
			
			isVisible = E3BoundingBox_ShadowIntersectsViewFrustum( inView, inBounds, lightPos );
		}
		else // lighting pass
		{
			isVisible = E3BoundingBox_IntersectViewFrustum( inView, inBounds );
		}
	}
	
	return isVisible;
}

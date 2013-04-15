/*  NAME:
       QORenderer.cpp

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
	if (mContext != NULL)
	{
		GLDrawContext_Destroy( &mContext );
	}
}

QORenderer::GLStencilFuncs::GLStencilFuncs()
{
	SetNULL();
}

void	QORenderer::GLStencilFuncs::SetNULL()
{
	glActiveStencilFace = NULL;
	glStencilFuncSeparate = NULL;
	glStencilOpSeparate = NULL;
	glStencilMaskSeparate = NULL;
}

/*!
	@function	Initialize
	@abstract	Get the function pointers.  This should be called just
				after the OpenGL context is created.
*/
void	QORenderer::GLStencilFuncs::Initialize( const TQ3GLExtensions& inExts )
{
	SetNULL();
	
	if (inExts.stencilTwoSide)
	{
		GLGetProcAddress( glActiveStencilFace, "glActiveStencilFaceEXT" );
	}
	
	if (inExts.separateStencil)
	{
		GLGetProcAddress( glStencilFuncSeparate, "glStencilFuncSeparate" );
		GLGetProcAddress( glStencilOpSeparate, "glStencilOpSeparate" );
		GLGetProcAddress( glStencilMaskSeparate, "glStencilMaskSeparate" );
	}
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
	, mDrawContextObject( NULL )
	, mGLContext( NULL )
	, mCleanup( mGLContext )
	, mSLFuncs()
	, mStencilFuncs()
	, mPPLighting( mSLFuncs, mRendererObject, mGLExtensions )
	, mRendererEditIndex( Q3Shared_GetEditIndex( inRenderer ) )
	, mDrawContextEditIndex( 0 )
	, mGLClearFlags( 0 )
	, mGLBlendEqProc( NULL )
	, mPassIndex( 0 )
	, mNumPasses( 1 )
	, mAllowLineSmooth( true )
	, mIsCachingShadows( false )
	, mNumPrimitivesRenderedInFrame( 0 )
	, mLineWidth( 1.0f )
	, mAttributesMask( kQ3XAttributeMaskAll )
	, mUpdateShader( true )
	, mLights( mGLExtensions, mStencilFuncs, mMatrixState, mStyleState,
		mPPLighting, mGLContext, mIsCachingShadows )
	, mTriBuffer( *this )
	, mTransBuffer( *this, mPPLighting )
	, mTextures( mRendererObject, mGLContext, mGLExtensions, mPPLighting )
{
	Q3Object_AddElement( mRendererObject, kQ3ElementTypeDepthBits,
		&kDefaultDepthBits );
	
	Q3InteractiveRenderer_SetRAVETextureFilter( mRendererObject,
		kQATextureFilter_Mid );
}

QORenderer::Renderer::~Renderer()
{
	if (mGLContext != NULL)
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

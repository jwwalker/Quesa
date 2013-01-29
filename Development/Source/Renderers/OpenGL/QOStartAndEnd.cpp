/*  NAME:
       QOStartAndEnd.cpp

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
#include "GLVBOManager.h"
#include "GLDisplayListManager.h"
#include "CQ3ObjectRef_Gets.h"
#include "GLShadowVolumeManager.h"

#ifndef STENCIL_TEST_TWO_SIDE_EXT
	#define	STENCIL_TEST_TWO_SIDE_EXT	0x8910
#endif

//=============================================================================
//      Local Functions
//-----------------------------------------------------------------------------

/*!
	@function	AdjustStencilAndDepthForShadows
	@abstract	Adjust stencil and depth resolution for stencil shadows.
	@discussion	To do stencil shadows, we request 8 bits of stencil buffer.
				In many cases, stencil and depth share space, so we cannot
				request a 32-bit depth buffer.
*/
static bool
AdjustStencilAndDepthForShadows( TQ3RendererObject inRenderer,
								TQ3DrawContextObject inDrawContext )
{
	bool	changedPref = false;
	TQ3Uns32	stencilBits = 0;
	Q3Object_GetProperty( inDrawContext,
		kQ3DrawContextPropertyGLStencilBufferDepth,
		sizeof(stencilBits), NULL, &stencilBits );
	
	TQ3Uns32	depthBits = 32;
	Q3Object_GetElement( inRenderer, kQ3ElementTypeDepthBits, &depthBits );
	
	if (stencilBits < 8)
	{
		stencilBits = 8;
		
		Q3Object_SetProperty( inDrawContext,
			kQ3DrawContextPropertyGLStencilBufferDepth,
			sizeof(stencilBits), &stencilBits );
		
		changedPref = true;
	}
	
	if (stencilBits + depthBits > 32)
	{
		depthBits = 32 - stencilBits;
		
		Q3Object_AddElement( inRenderer, kQ3ElementTypeDepthBits, &depthBits );
		
		changedPref = true;
	}
	
	return changedPref;
}



//=============================================================================
//      Method Implementations
//-----------------------------------------------------------------------------

TQ3Status	QORenderer::Renderer::StartFrame(
								TQ3ViewObject inView,
								TQ3DrawContextObject inDrawContext )
{
	// Save draw context for access from StartPass
	mDrawContextObject = inDrawContext;
	
	// Update draw context validation flags
	TQ3XDrawContextValidation		drawContextFlags;
	Q3XDrawContext_GetValidationFlags( inDrawContext, &drawContextFlags );
	TQ3Uns32 rendererEditIndex = Q3Shared_GetEditIndex( mRendererObject );
	TQ3Uns32 drawContextEditIndex = Q3Shared_GetEditIndex( inDrawContext );
	if (rendererEditIndex != mRendererEditIndex)
	{
		mRendererEditIndex = rendererEditIndex;
		drawContextFlags |= kQ3XDrawContextValidationClearFunction;
	}
	if (drawContextEditIndex != mDrawContextEditIndex)
	{
		mDrawContextEditIndex = drawContextEditIndex;
		drawContextFlags |= kQ3XDrawContextValidationClearFunction |
			kQ3XDrawContextValidationDepthState;
	}

	// Check whether shadows are requested
	TQ3Boolean	isShadowingRequested = kQ3False;
	if (mNumPasses == 1){ // disable shadows because shadow and multipass is not well implemented
		Q3Object_GetProperty( mRendererObject,
			kQ3RendererPropertyShadows, sizeof(isShadowingRequested), NULL,
			&isShadowingRequested );
		}
	
	// Check whether line smoothing is allowed
	mAllowLineSmooth = true;
	TQ3Boolean	allowSmooth;
	if ( (kQ3Success == Q3Object_GetProperty( mRendererObject,
		kQ3RendererPropertyAllowLineSmooth, sizeof(allowSmooth), NULL,
			&allowSmooth )) && (allowSmooth == kQ3False) )
	{
		mAllowLineSmooth = false;
	}
	
	if (isShadowingRequested)
	{
		if (AdjustStencilAndDepthForShadows( mRendererObject, inDrawContext ))
		{
			drawContextFlags = kQ3XDrawContextValidationAll;
		}
	}

#if QUESA_OS_MACINTOSH
	// On the Mac, rendering with OpenGL, we do not care about changes of device or
	// window clipping.
	drawContextFlags &= ~kQ3XDrawContextValidationDevice;
	drawContextFlags &= ~kQ3XDrawContextValidationWindowClip;
#endif
	
	if (drawContextFlags != 0)
	{
		// If we don't have a GL context, rebuild everything
		if (mGLContext == NULL)
		{
			drawContextFlags = kQ3XDrawContextValidationAll;
		}
		// Otherwise, make sure it's active
		else
		{
			 GLDrawContext_SetCurrent( mGLContext, kQ3True );
		}

		// Handle some common easy cases
		if (drawContextFlags != kQ3XDrawContextValidationAll)
		{
			if (drawContextFlags & kQ3XDrawContextValidationClearFunction)
			{
				GLDrawContext_SetClearFlags( inDrawContext, &mGLClearFlags );
				drawContextFlags &= ~kQ3XDrawContextValidationClearFunction;
			}
			
			if (drawContextFlags & kQ3XDrawContextValidationDepthState)
			{
				GLDrawContext_SetDepthState( inDrawContext );
				drawContextFlags &= ~kQ3XDrawContextValidationDepthState;
			}

			if (drawContextFlags & kQ3XDrawContextValidationBackgroundShader)
			{
				GLDrawContext_SetBackgroundColour( inDrawContext );
				drawContextFlags &= ~kQ3XDrawContextValidationBackgroundShader;
			}

			if (drawContextFlags & kQ3XDrawContextValidationWindowPosition)
			{
				if (GLDrawContext_UpdateWindowPosition( mGLContext ))
					drawContextFlags &= ~kQ3XDrawContextValidationWindowPosition;
			}

			if (drawContextFlags & (kQ3XDrawContextValidationWindowSize |
				kQ3XDrawContextValidationPane))
			{
				if (GLDrawContext_UpdateSize( inDrawContext, mGLContext ) == kQ3Success)
				{
					drawContextFlags &= ~kQ3XDrawContextValidationWindowSize;
					drawContextFlags &= ~kQ3XDrawContextValidationPane;
				}
			}
		}

		// Handle more complex cases by doing a rebuild
		if (drawContextFlags != kQ3XDrawContextValidationClearFlags)
		{
			// Dispose of the old GL context
			if (mGLContext != NULL)
			{
				mPPLighting.Cleanup();
				GLDrawContext_Destroy( &mGLContext );
			}


			// And try to build a new one
			mGLContext = GLDrawContext_New( inView, inDrawContext,
				&mGLClearFlags );
			if (mGLContext == NULL)
				return(kQ3Failure);
			
			mTextures.UpdateTextureCache();


			GLUtils_CheckExtensions( &mGLExtensions );
			TQ3Boolean	allowVBO = kQ3True;
			Q3Object_GetProperty( mRendererObject,
				kQ3RendererPropertyAllowVBOs, sizeof(allowVBO), NULL,
				&allowVBO );
			if (allowVBO == kQ3False)
			{
				mGLExtensions.vertexBufferObjects = kQ3False;
			}
			mSLFuncs.Initialize( mGLExtensions );
			mStencilFuncs.Initialize( mGLExtensions );
			
			
			GLGetProcAddress( mGLBlendEqProc, "glBlendEquation",
				"glBlendEquationEXT" );
		}



		// Clear the draw context flags
		Q3XDrawContext_ClearValidationFlags( inDrawContext );
	}

	// Activate our context (forcing it to be set at least once per frame)
	GLDrawContext_SetCurrent( mGLContext, kQ3True );
	GLDrawContext_StartFrame( mGLContext );

	
	// If shadows are desired, check whether we got a stencil buffer,
	// and whether we have some form of 2-sided stencil.
	bool	isShadowing = false;
	if ( isShadowingRequested &&
		(mGLExtensions.stencilTwoSide || mGLExtensions.separateStencil) &&
		(mGLExtensions.stencilBits >= 8) )
	{
		isShadowing = true;
	}
	glDisable( GL_STENCIL_TEST );
	
	
	// Update the main VBO cache memory limit
	{
		TQ3Uns32 vboCacheK = 51200;	// 50 megabytes
		Q3Object_GetProperty( mRendererObject,
				kQ3RendererPropertyVBOLimit, sizeof(vboCacheK), NULL,
				&vboCacheK );
		UpdateVBOCacheLimit( mGLContext, vboCacheK );
	}
	
	
	// Update the shadow VBO cache memory limit
	TQ3Uns32	shadowCacheMemK = 0;
	if ( isShadowing && (mGLExtensions.vertexBufferObjects == kQ3True) )
	{
		Q3Object_GetProperty( mRendererObject,
				kQ3RendererPropertyShadowVBOLimit, sizeof(shadowCacheMemK), NULL,
				&shadowCacheMemK );
		ShadowVolMgr::StartFrame( mGLContext, shadowCacheMemK );
	}
	mIsCachingShadows = (shadowCacheMemK > 0);
	
	
	// Multi-pass lighting may have turned off depth writing, which would
	// prevent clearing the depth buffer if we do not reset it.
	GLDrawContext_SetDepthState( inDrawContext );

	
	// Tell light manager that a frame is starting
	mLights.StartFrame( inView, isShadowing );
	mPPLighting.StartFrame();


	// Clear the context
	if (mGLClearFlags != 0)
		glClear( mGLClearFlags );


	// Reset pass counter
	mPassIndex = 0;
	mUpdateShader = true;
	mAttributesMask = kQ3XAttributeMaskAll;
	
	return kQ3Success;
}

TQ3Status	QORenderer::Renderer::EndFrame(
								TQ3ViewObject inView )
{
	// Note: This method is only called when Q3View_Sync is called,
	// meaning usually never.

	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// Swap buffers
	GLDrawContext_SwapBuffers( mGLContext );

	// Let the view know that we're done
	TQ3Status qd3dStatus = Q3XView_EndFrame( inView );

	return(qd3dStatus);
}

void		QORenderer::Renderer::StartPass(
								TQ3CameraObject inCamera,
								TQ3GroupObject inLights )
{
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	mMatrixState.Reset();
	
	mViewState.Reset();
	mGeomState.Reset();
	mViewIllumination = kQ3ObjectTypeInvalid;
	
	// Initialize specularity and emission
	mCurrentSpecularColor[0] = mCurrentSpecularColor[1] =
		mCurrentSpecularColor[2] = 0.0f;
	mCurrentSpecularColor[3] = 1.0f;
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,
				mCurrentSpecularColor );
	mCurrentSpecularControl = 0.0f;
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 0.0f );
	mCurrentEmissiveColor.r = mCurrentEmissiveColor.g =
		mCurrentEmissiveColor.b = 0.0f;
	SetEmissiveMaterial( mCurrentEmissiveColor );

	// Initialize style states
	mStyleState.mInterpolation = kQ3InterpolationStyleVertex;
	mStyleState.mBackfacing = kQ3BackfacingStyleBoth;
	mStyleState.mFill = kQ3FillStyleFilled;
	mStyleState.mOrientation = kQ3OrientationStyleCounterClockwise;
	mStyleState.mHilite = CQ3ObjectRef();	// i.e., NULL
	mStyleState.mIsCastingShadows = true;
	mStyleState.mExplicitEdges = false;
	mLineWidth = 1.0f;
		
	// Turn fog off.
	mStyleState.mFogStyles.clear();
	TQ3FogStyleData	noFog = { kQ3Off };
	mStyleState.mFogStyles.push_back( noFog );
	mStyleState.mCurFogStyleIndex = 0;
	
	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

	mGLClientStates.StartPass();
	
	glDisable( GL_ALPHA_TEST );
	glDisable( GL_BLEND );
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);
	glDisable(GL_POLYGON_OFFSET_POINT);
	glFrontFace( GL_CCW );	// orientation style
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
	glShadeModel( GL_SMOOTH );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );	// fill style
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	GLDrawContext_SetDepthState( mDrawContextObject );
	glLineWidth( mLineWidth );
	
	mLights.StartPass( inCamera, mRendererObject );
	mTextures.StartPass();
	if (! mLights.IsShadowMarkingPass())
	{
		mPPLighting.StartPass();
	}
}

static bool IsSwapWanted( TQ3ViewObject inView )
{
	CQ3ObjectRef	theDrawContext( CQ3View_GetDrawContext( inView ) );
	TQ3Boolean		swapFlag;
	return (kQ3Failure == Q3Object_GetProperty( theDrawContext.get(),
		kQ3DrawContextPropertySwapBufferInEndPass, sizeof(swapFlag), NULL,
		&swapFlag )) ||
		(swapFlag == kQ3True);
}

/*!
	@function	RenderTransparent
	@abstract	Draw transparent stuff in the case where multiple lighting
				passes were needed, using additional mini-passes.
*/
void	QORenderer::Renderer::RenderTransparent( TQ3ViewObject inView )
{
	CQ3ObjectRef	theCamera( CQ3View_GetCamera( inView ) );
	CQ3ObjectRef	theLightGroup( CQ3View_GetLightGroup( inView ) );
	bool isMoreNeeded;
	GLenum	dstFactor = GL_ONE_MINUS_SRC_ALPHA;
	
	// Ignore any mask set for shadowing
	glDisable( GL_STENCIL_TEST );
	
	// If kQ3RendererPropertyDepthAlphaThreshold has been set, let the mostly-
	// opaque parts affect the depth buffer, so that other parts will not be
	// drawn behind them.
	mTransBuffer.DrawDepth( inView );

	mLights.StartFrame( inView, false );
	
	do
	{
		mLights.StartPass( theCamera.get(), mRendererObject );
		mTextures.StartPass();
		mPPLighting.StartPass();
		
		mTransBuffer.DrawTransparency( inView, GL_ONE, dstFactor );
		dstFactor = GL_ONE;	// for next mini-pass
		
		mTextures.EndPass();
		mPPLighting.EndPass();
		isMoreNeeded = mLights.EndPass();
	} while (isMoreNeeded);
	
	mTransBuffer.Cleanup();
}

TQ3ViewStatus		QORenderer::Renderer::EndPass(
								TQ3ViewObject inView )
{
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// Flush any remaining triangles
	mTriBuffer.Flush();
	
	// Transparency is drawn at the end of the last lighting pass.
	// If there was only one lighting pass, we can do it now.
	bool isFirstLightingPass = mLights.IsFirstPass();
	if (isFirstLightingPass && mLights.IsLastLightingPass() && mTransBuffer.HasContent())
	{
		mTransBuffer.DrawDepth( inView );
		
		// When drawing the depth first, normally we want to use GL_LEQUAL, but
		// for special purposes the client might reverse it.
		TQ3Uns32 compareFunc = GL_LEQUAL;
		if ( (kQ3Success == Q3Object_GetProperty( mDrawContextObject,
			kQ3DrawContextPropertyGLDepthFunc,
			sizeof(compareFunc), NULL, &compareFunc )) &&
			(compareFunc == GL_GREATER) )
		{
			compareFunc = GL_GEQUAL;
		}
		
		glDepthFunc( compareFunc );
		mTransBuffer.DrawTransparency( inView, GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
		mTransBuffer.Cleanup();
	}
	
	// don't increment the pass count if we have to make another pass because of lighting
	if ( ! mLights.EndPass())
		mPassIndex += 1;
		
	TQ3ViewStatus	allDone = (mPassIndex < mNumPasses)? kQ3ViewStatusRetraverse: kQ3ViewStatusDone;

	// reset some state to defaults

	mTextures.EndPass();
	mPPLighting.EndPass();
	if (allDone == kQ3ViewStatusDone)
	{
		mLights.EndFrame( inView );
	}
	
	// If this is the end of several lighting passes, handling transparency is
	// trickier.
	if (mLights.IsLastLightingPass() && (! isFirstLightingPass) && mTransBuffer.HasContent())
	{
		RenderTransparent( inView );
	}

	// Swap the back buffer, unless asked not to
	if ( (allDone == kQ3ViewStatusDone) && IsSwapWanted( inView ) )
	{
		GLDrawContext_SwapBuffers( mGLContext );
	}
	
	if (mGLExtensions.vertexBufferObjects == kQ3True)
	{
		FlushVBOCache( mGLContext );
		ShadowVolMgr::Flush( mGLContext, mRendererObject );
	}
	FlushDisplayListCache( mGLContext );

	// Copy the primitive render count to a property where the user can get it
	Q3Object_SetProperty( mRendererObject, kQ3RendererPropertyPrimitivesRenderedCount,
		sizeof(TQ3Uns64), &mNumPrimitivesRenderedInFrame );
	
	return allDone;
}

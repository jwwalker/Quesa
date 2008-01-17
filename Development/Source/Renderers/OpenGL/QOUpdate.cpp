/*  NAME:
       QOUpdate.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2008, Quesa Developers. All rights reserved.

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
#include "CQ3ObjectRef_Gets.h"
#include "GLUtils.h"
#include "GLVBOManager.h"
#include "GLDisplayListManager.h"

#include <algorithm>


//=============================================================================
//     Constants
//-----------------------------------------------------------------------------

#ifndef	GL_SAMPLE_BUFFERS_ARB
	#define	GL_SAMPLE_BUFFERS_ARB			0x80A8
	#define	GL_MULTISAMPLE_ARB				0x809D
#endif


//=============================================================================
//      operator== : Operator to compare fog style datas.
//-----------------------------------------------------------------------------
static	bool operator==( const TQ3FogStyleData& inOne, const TQ3FogStyleData& inTwo )
{
	bool	isSameFog = (inOne.state == inTwo.state);
	
	if (isSameFog && (inOne.state == kQ3On))
	{
		isSameFog = (inOne.mode == inTwo.mode) &&
			(inOne.color.r == inTwo.color.r) &&
			(inOne.color.g == inTwo.color.g) &&
			(inOne.color.b == inTwo.color.b);
			// Note, alpha is not relevant
			
		if (isSameFog)
		{
			switch (inOne.mode)
			{
				case kQ3FogModeExponential:
				case kQ3FogModeExponentialSquared:
					isSameFog = (inOne.density == inTwo.density);
					break;
				
				default:	// linear fog or anything else
					isSameFog = (inOne.fogStart == inTwo.fogStart) &&
						(inOne.fogEnd == inTwo.fogEnd);
					break;
			}
		}
	}
	return isSameFog;
}

//=============================================================================
//      Method implementations
//-----------------------------------------------------------------------------


void	QORenderer::Renderer::UpdateDiffuseColor(
								const TQ3ColorRGB* inAttColor )
{
	if ( (mAttributesMask & kQ3XAttributeMaskDiffuseColor) != 0 )
		mViewState.diffuseColor = inAttColor;
}

void	QORenderer::Renderer::UpdateSpecularColor(
								const TQ3ColorRGB* inAttColor )
{
	mViewState.specularColor = inAttColor;
}

void	QORenderer::Renderer::UpdateTransparencyColor(
								const TQ3ColorRGB* inAttColor )
{
	mViewState.alpha = (inAttColor->r + inAttColor->g + inAttColor->b) *
		0.3333333f;
}

void	QORenderer::Renderer::UpdateEmissiveColor(
								const TQ3ColorRGB* inAttColor )
{
	mViewState.emissiveColor = inAttColor;
}

void	QORenderer::Renderer::UpdateSpecularControl(
								const float* inAttValue )
{
	mViewState.specularControl = *inAttValue;
}

void	QORenderer::Renderer::UpdateHiliteState(
								const TQ3Switch* inAttState )
{
	mViewState.highlightState = *inAttState;
}

void	QORenderer::Renderer::UpdateSurfaceShader(
								TQ3ShaderObject inShader )
{
	if (mLights.IsShadowMarkingPass())
	{
		return;
	}

	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	
	mTriBuffer.Flush();
	
	
	// If this is a texture shader, get the texture from the shader
	CQ3ObjectRef	theTexture;
	if (inShader != NULL && ( (mAttributesMask & kQ3XAttributeMaskDiffuseColor) != 0))
	{
		if (Q3SurfaceShader_GetType( inShader ) == kQ3SurfaceShaderTypeTexture)
		{
			theTexture = CQ3TextureShader_GetTexture( inShader );
		}
	}
	
	
	mTextures.SetCurrentTexture( theTexture.get(), inShader );
	mPPLighting.UpdateTexture();
}


void	QORenderer::Renderer::UpdateIlluminationShader(
								TQ3ShaderObject inShader )
{
	if (mLights.IsShadowMarkingPass())
	{
		return;
	}
	
	if (!mUpdateShader)
	{
		return;
	}
	
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	
	mTriBuffer.Flush();
	
	
	// Update our state
	if (inShader != NULL)
	{
		mViewIllumination = Q3IlluminationShader_GetType(inShader);
	}
	else
	{
		mViewIllumination = kQ3ObjectTypeInvalid;
	}
	mPPLighting.UpdateIllumination( mViewIllumination );
	
	
	// Update OpenGL specular state
	GLfloat	dullSpecular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat	dullSpecularControl = 0.0f;
	
	// Texturing note:  The legacy behavior is that unless the illumination is
	// NULL, the texture completely overrides the underlying color.
	
	switch (mViewIllumination)
	{
		default:
		case kQ3IlluminationTypeNULL:
			glDisable( GL_LIGHTING );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  dullSpecular );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, dullSpecularControl );
			break;
		
		case kQ3IlluminationTypeLambert:
			glEnable( GL_LIGHTING );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  dullSpecular );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, dullSpecularControl );
			break;
		
		case kQ3IlluminationTypePhong:
			glEnable( GL_LIGHTING );
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,
				mCurrentSpecularColor );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS,
				GLUtils_SpecularControlToGLShininess( mCurrentSpecularControl ) );
			break;
	}
}

void	QORenderer::Renderer::UpdateInterpolationStyle(
								const TQ3InterpolationStyle* inStyleData )
{
	if (mLights.IsShadowMarkingPass())
	{
		return;
	}
	
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	
	mTriBuffer.Flush();
	
	
	mStyleState.mInterpolation = *inStyleData;
	
	
	switch (mStyleState.mInterpolation)
	{
		case kQ3InterpolationStyleNone:
			glShadeModel( GL_FLAT );
			break;
		
		default:
		case kQ3InterpolationStyleVertex:
		case kQ3InterpolationStylePixel:
			glShadeModel( GL_SMOOTH );
			break;
	}
}

void	QORenderer::Renderer::UpdateBackfacingStyle(
								const TQ3BackfacingStyle* inStyleData )
{
	if (mLights.IsShadowMarkingPass())
	{
		mStyleState.mBackfacing = *inStyleData;
		return;
	}
	
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	
	mTriBuffer.Flush();
	
	
	mStyleState.mBackfacing = *inStyleData;
	
	
	switch (mStyleState.mBackfacing)
	{
		case kQ3BackfacingStyleRemove:
			glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
			glCullFace( GL_BACK );
			glEnable( GL_CULL_FACE );
			break;
		
		default:
		case kQ3BackfacingStyleBoth:
		case kQ3BackfacingStyleFlip:
			glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
			glDisable( GL_CULL_FACE );
			break;
	}
}

void	QORenderer::Renderer::UpdateFillStyle(
								const TQ3FillStyle* inStyleData )
{
	if (mLights.IsShadowMarkingPass())
	{
		mStyleState.mFill = *inStyleData;
		return;
	}

	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	
	mTriBuffer.Flush();
	
	
	mStyleState.mFill = *inStyleData;
	
	
	switch (mStyleState.mFill)
	{
		case kQ3FillStylePoints:
			glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
			break;

		case kQ3FillStyleEdges:
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			break;

		case kQ3FillStyleFilled:
		default:
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			break;
	}

}

void	QORenderer::Renderer::UpdateOrientationStyle(
								const TQ3OrientationStyle* inStyleData )
{
	if (mLights.IsShadowMarkingPass())
	{
		return;
	}
	
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	
	mTriBuffer.Flush();
	
	
	mStyleState.mOrientation = *inStyleData;
	
	
	switch (mStyleState.mOrientation)
	{
		case kQ3OrientationStyleClockwise:
			glFrontFace( GL_CW );
			break;

		case kQ3OrientationStyleCounterClockwise:
		default:
			glFrontFace( GL_CCW );
			break;
	}
}

void	QORenderer::Renderer::UpdateHighlightStyle(
								TQ3AttributeSet* inStyleData )
{
	if (mLights.IsShadowMarkingPass())
	{
		return;
	}
	
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	
	mTriBuffer.Flush();
	
	
	if (*inStyleData == NULL)
	{
		mStyleState.mHilite = CQ3ObjectRef();
	}
	else
	{
		mStyleState.mHilite = CQ3ObjectRef( Q3Shared_GetReference( *inStyleData ) );
	}
	
}

void	QORenderer::Renderer::UpdateAntiAliasStyle(
								TQ3AntiAliasStyleData* inStyleData )
{
	if (mLights.IsShadowMarkingPass())
	{
		return;
	}
	
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	
	mTriBuffer.Flush();
	
	
	// Currently there is no way to vary point and line size.
	glPointSize( 1.0f );
	glLineWidth( mLineWidth );


	// Turn everything off
	glDisable( GL_POINT_SMOOTH );
	glDisable( GL_LINE_SMOOTH );
	glDisable( GL_POLYGON_SMOOTH );
	GLint	sampleBuffers = 0;
	glGetIntegerv( GL_SAMPLE_BUFFERS_ARB, &sampleBuffers );
	if (sampleBuffers > 0)
	{
		glDisable( GL_MULTISAMPLE_ARB );
	}


	// Maybe turn some things on.
	if (inStyleData->state == kQ3On)
	{
		if ( (inStyleData->mode & kQ3AntiAliasModeMaskFullScreen) != 0 )
		{
			if (sampleBuffers > 0)
			{
				glEnable( GL_MULTISAMPLE_ARB );
			}
		}
		else
		{
			glEnable(GL_POINT_SMOOTH);
			
			if ( ((inStyleData->mode & kQ3AntiAliasModeMaskEdges) != 0) &&
				mAllowLineSmooth )
			{
				glEnable( GL_LINE_SMOOTH );
			}
			
			if ( (inStyleData->mode & kQ3AntiAliasModeMaskFilled) != 0 )
			{
				glEnable( GL_POLYGON_SMOOTH );
			}
		}
	}
}

void	QORenderer::Renderer::UpdateFogStyle(
								const TQ3FogStyleData* inStyleData )
{
	if (mLights.IsShadowMarkingPass())
	{
		return;
	}
	
	
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	
	mTriBuffer.Flush();
	
	
	if (inStyleData->state == kQ3On)
	{
		glEnable(GL_FOG);
		
		GLfloat	fogColor[4] = {
			inStyleData->color.r,
			inStyleData->color.g,
			inStyleData->color.b,
			inStyleData->color.a
		};
		glFogfv( GL_FOG_COLOR, fogColor );
		
		switch (inStyleData->mode)
		{
			case kQ3FogModeExponential:
				glFogi( GL_FOG_MODE, GL_EXP);
				glFogf( GL_FOG_DENSITY, inStyleData->density );
				break;
				
			case kQ3FogModeExponentialSquared:
				glFogi( GL_FOG_MODE, GL_EXP2);
				glFogf( GL_FOG_DENSITY, inStyleData->density );
				break;
				
			case kQ3FogModeLinear:
			default:
				glFogi( GL_FOG_MODE, GL_LINEAR);
				glFogf( GL_FOG_START, inStyleData->fogStart );
				glFogf( GL_FOG_END, inStyleData->fogEnd );
				break;
		}
	}
	else
	{
		glDisable( GL_FOG );
	}
	
	
	// Update fog state in my instance data.  This is needed for buffered
	// transparent triangles.
	// Note that the find operation uses a custom operator== for fog data.
	std::vector<TQ3FogStyleData>::iterator foundFog =
		std::find( mStyleState.mFogStyles.begin(), mStyleState.mFogStyles.end(),
		*inStyleData );
	if (foundFog == mStyleState.mFogStyles.end())
	{
		try
		{
			mStyleState.mFogStyles.push_back( *inStyleData );
			mStyleState.mCurFogStyleIndex = mStyleState.mFogStyles.size() - 1;
		}
		catch (...)
		{
		}
	}
	else
	{
		mStyleState.mCurFogStyleIndex = foundFog - mStyleState.mFogStyles.begin();
	}
}

void	QORenderer::Renderer::UpdateCastShadowsStyle(
									TQ3Boolean inStyleData )
{
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	
	mTriBuffer.Flush();
	
	
	mStyleState.mIsCastingShadows = (inStyleData == kQ3True);
}

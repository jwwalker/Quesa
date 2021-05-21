/*  NAME:
       QOLights.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2021, Quesa Developers. All rights reserved.

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
#include "QOLights.h"
#include "QORenderer.h"
#include "Q3GroupIterator.h"
#include "E3ErrorManager.h"
#include "E3Math.h"
#include "E3Math_Intersect.h"
#include "E3View.h"
#include "CQ3ObjectRef_Gets.h"
#include "GLCamera.h"
#include "GLUtils.h"
#include "QuesaMathOperators.hpp"
#include <cmath>
#include <limits>
using namespace std;

#if Q3_DEBUG && QUESA_OS_MACINTOSH && QUESA_UH_IN_FRAMEWORKS && QUESA_TRACE_GL
	// This code allows one to see the values passed to certain functions in a
	// trace produced by Apple's OpenGL Profiler.
	#include <OpenGL/CGLProfiler.h>
	
	#define TRACE_MSG(...)	do {	\
								char msg_[1024];	\
								std::snprintf( msg_, sizeof(msg_), __VA_ARGS__ );	\
								CGLSetOption( kCGLGOComment, (long) msg_ );	\
							} while (0)
	
	static void TraceGLMatrix( const GLfloat* inMatrix )
	{
		TRACE_MSG( "  %8.4f %8.4f %8.4f %8.4f", inMatrix[0], inMatrix[1], inMatrix[2], inMatrix[3] );
		TRACE_MSG( "  %8.4f %8.4f %8.4f %8.4f", inMatrix[4], inMatrix[5], inMatrix[6], inMatrix[7] );
		TRACE_MSG( "  %8.4f %8.4f %8.4f %8.4f", inMatrix[8], inMatrix[9], inMatrix[10], inMatrix[11] );
		TRACE_MSG( "  %8.4f %8.4f %8.4f %8.4f", inMatrix[12], inMatrix[13], inMatrix[14], inMatrix[15] );
	}
#else
	#define		TraceGLMatrix(x)
#endif


//=============================================================================
//     Local constants
//-----------------------------------------------------------------------------


// These constants are defined in the Mac framework version of gl.h,
// but if we are building a CFM binary, we may be using an older header.
// Such a situation could also occur on other platforms.

#ifndef GL_DEPTH_CLAMP_NV
	#define	GL_DEPTH_CLAMP_NV					0x864F
#endif

#ifndef GL_INCR_WRAP_EXT
	#define	GL_INCR_WRAP_EXT					0x8507
	#define	GL_DECR_WRAP_EXT					0x8508
#endif

// GL_EXT_stencil_two_side
#ifndef GL_STENCIL_TEST_TWO_SIDE_EXT
	#define	GL_STENCIL_TEST_TWO_SIDE_EXT		0x8910
#endif

//=============================================================================
//      Local Functions
//-----------------------------------------------------------------------------

/*!
	@function	CalcLightColor
	
	@abstract	Combine Quesa light color and brightness to make an OpenGL
				light color.
*/
static void CalcLightColor( const TQ3LightData& inLightData,
							GLfloat* outLightColor )
{
	outLightColor[0] = (inLightData.color.r * inLightData.brightness);
	outLightColor[1] = (inLightData.color.g * inLightData.brightness);
	outLightColor[2] = (inLightData.color.b * inLightData.brightness);
	outLightColor[3] = 1.0f;
}

/*!
	@function	ConvertAmbientLight
	
	@abstract	Turn a Quesa ambient light object into OpenGL ambient light.
*/
static void ConvertAmbientLight( TQ3LightObject inLight, TQ3ColorRGB& ioAmbientColor )
{
	// Get light data
	TQ3LightData		lightData;
	Q3Light_GetData( inLight, &lightData );
	
	// Find the light color
	GLfloat		lightColor[4];
	CalcLightColor( lightData, lightColor );
	
	// Add this light into the ambient illumination
	ioAmbientColor.r += lightColor[0];
	ioAmbientColor.g += lightColor[1];
	ioAmbientColor.b += lightColor[2];
}

/*!
	@function	IsShadowCaster
	
	@abstract	Test whether a light casts shadows.
*/
static bool IsShadowCaster( TQ3LightObject inLight, TQ3ObjectType inLightType )
{
	TQ3Boolean	castsShadows = kQ3False;
	
	switch (inLightType)
	{
		case kQ3LightTypeDirectional:
			Q3DirectionalLight_GetCastShadowsState( inLight, &castsShadows );
			break;
			
		case kQ3LightTypeSpot:
			Q3SpotLight_GetCastShadowsState( inLight, &castsShadows );
			break;
			
		case kQ3LightTypePoint:
			Q3PointLight_GetCastShadowsState( inLight, &castsShadows );
			break;
	}
	
	return castsShadows == kQ3True;
}

/*!
	@function	SetAttenuationParams
	
	@abstract	Convert a Quesa attenuation type to OpenGL parameters.
*/
static void SetAttenuationParams( TQ3AttenuationType inType,
								GLfloat& outConstantAtt,
								GLfloat& outLinearAtt,
								GLfloat& outQuadraticAtt )
{
	switch (inType)
	{
		case kQ3AttenuationTypeNone:
			outConstantAtt  = 1.0f;
			outLinearAtt    = 0.0f;
			outQuadraticAtt = 0.0f;
			break;

		case kQ3AttenuationTypeInverseDistance:
			outConstantAtt  = 0.0f;
			outLinearAtt    = 1.0f;
			outQuadraticAtt = 0.0f;
			break;

		case kQ3AttenuationTypeInverseDistanceSquared:
			outConstantAtt  = 0.0f;
			outLinearAtt    = 0.0f;
			outQuadraticAtt = 1.0f;
			break;
			
		default:
			Q3_ASSERT(!"Unknown attenuation");
			E3ErrorManager_PostError( kQ3ErrorInvalidParameter, kQ3False );
			outConstantAtt  = 0.0f;
			outLinearAtt    = 0.0f;
			outQuadraticAtt = 0.0f;
			break;
	}
}


static bool IsIgnorablePositionalLight( TQ3LightObject inLight,
										TQ3ObjectType inLightType,
										TQ3ViewObject inView,
										float inBrightness )
{
	bool ignorable = false;
	if ( (inLightType == kQ3LightTypeSpot) || (inLightType == kQ3LightTypePoint) )
	{
		CQ3ObjectRef theCamera( CQ3View_GetCamera( inView ) );
		TQ3AttenuationType attType;
		TQ3Point3D lightLocation;
		TQ3SpotLightData spotData;
		if (inLightType == kQ3LightTypeSpot)
		{
			Q3SpotLight_GetData( inLight, &spotData );
			attType = spotData.attenuation;
			lightLocation = spotData.location;
		}
		else // kQ3LightTypePoint
		{
			TQ3PointLightData	ptData;
			Q3PointLight_GetData( inLight, &ptData );
			attType = ptData.attenuation;
			lightLocation = ptData.location;
		}
		
		if (attType == kQ3AttenuationTypeInverseDistanceSquared)
		{
			float distance = E3Math_DistanceFromPointToViewFrustum( lightLocation, theCamera.get() );
			//Q3_MESSAGE_FMT("Distance to frustum=%f, brightness=%f", distance, inBrightness );
			if (distance > 10.0f * sqrtf( inBrightness ))
			{
				ignorable = true;
				// This means that the effective brightness in the view frustum is <= 0.01.
				//Q3_MESSAGE_FMT("Light %p is ignorable", inLight );
			}
		}
		
		if ( (! ignorable) && (inLightType == kQ3LightTypeSpot) )
		{
			// If the outer cone of light does not intersect the view frustum, then the light
			// can have no effect on visible things.
			TQ3Ray3D coneAxis;
			coneAxis.origin = spotData.location;
			coneAxis.direction = Q3Normalize3D( spotData.direction );
			float outerCos = cosf( spotData.outerAngle );
			if ( ! E3Cone_IntersectViewFrustum( coneAxis, outerCos, theCamera.get() ))
			{
				ignorable = true;
				//Q3_MESSAGE_FMT("Spot Light %p is ignorable", inLight );
			}
		}
	}
	
	return ignorable;
}


//=============================================================================
//      Class Implementations
//-----------------------------------------------------------------------------
QORenderer::Lights::Lights( QORenderer::Renderer& inRenderer )
	: mRenderer( inRenderer )
	, mLightCount( 0 )
	, mIsShadowFrame( false )
	, mIsFirstPass( false )
	, mIsShadowPhase( false )
	, mIsNextPassShadowPhase( false )
	, mIsAnotherPassNeeded( false )
	, mIsShadowMarkingPass( false )
	, mStartingLightIndexForPass( 0 )
	, mMaxGLLights( 8 )
	, mSavedYon( std::numeric_limits<float>::infinity() )
	, mShadowMarker( inRenderer, inRenderer.GetMatrixState(), inRenderer.GetStyleState(), inRenderer.Shader(),
		mGLLightPosition, inRenderer.GLContext(), inRenderer.GLExtensions(),
		inRenderer.Funcs(), inRenderer.IsCachingShadows() )
	, mIsLowDMode( false )
{
	
}

/*!
	@function	AddDirectionalLight
	
	@abstract	Turn a Quesa directional light object into OpenGL directional
				light.
*/
void QORenderer::Lights::AddDirectionalLight( TQ3LightObject inLight,
								const TQ3Matrix4x4& inWorldToView )
{
	// Get light data
	TQ3DirectionalLightData		lightData;
	Q3DirectionalLight_GetData( inLight, &lightData );
	mLightBrightness = lightData.lightData.brightness;
	
	// Transform the direction to view coordinates
	lightData.direction = Q3Normalize3D( lightData.direction * inWorldToView );
	
	// Set up the light direction as OpenGL likes it
	mGLLightPosition[0] = -lightData.direction.x;
	mGLLightPosition[1] = -lightData.direction.y;
	mGLLightPosition[2] = -lightData.direction.z;
	mGLLightPosition[3] = 0.0f;
}

/*!
	@function	AddPointLight
	
	@abstract	Turn a Quesa point light object into OpenGL point
				light.
*/
void QORenderer::Lights::AddPointLight( TQ3LightObject inLight,
								const TQ3Matrix4x4& inWorldToView )
{
	// Get light data
	TQ3PointLightData	lightData;
	Q3PointLight_GetData( inLight, &lightData );
	mLightBrightness = lightData.lightData.brightness;

	// Transform the location to view coordinates
	lightData.location = lightData.location * inWorldToView;
	
	// and put the location in OpenGL form
	mGLLightPosition[0] = lightData.location.x;
	mGLLightPosition[1] = lightData.location.y;
	mGLLightPosition[2] = lightData.location.z;
	mGLLightPosition[3] = 1.0f;
}

/*!
	@function	AddSpotLight
	
	@abstract	Turn a Quesa spot light object into OpenGL spot
				light.
*/
void QORenderer::Lights::AddSpotLight( TQ3LightObject inLight,
							const TQ3Matrix4x4& inWorldToView )
{
	// Get light data
	TQ3SpotLightData	lightData;
	Q3SpotLight_GetData( inLight, &lightData );
	mLightBrightness = lightData.lightData.brightness;

	// Transform the direction and location to view coordinates
	lightData.location = lightData.location * inWorldToView;
	mSpotLightDirection = Q3Normalize3D( lightData.direction * inWorldToView );
		
	// Put location in OpenGL form
	mGLLightPosition[0] = lightData.location.x;
	mGLLightPosition[1] = lightData.location.y;
	mGLLightPosition[2] = lightData.location.z;
	mGLLightPosition[3] = 1.0f;
}

/*!
	@function	AddLight
	
	@abstract	Turn a Quesa light object into OpenGL light.
*/
void QORenderer::Lights::AddLight( TQ3LightObject inLight,
						const TQ3Matrix4x4& inWorldToView )
{
	mLightType = Q3Light_GetType( inLight );
	bool isRecognizedLightType = false;
	
	// If this is a shadow pass, then there is one light and
	// it needs to be submitted to OpenGL so that the shadow
	// renderer can find it.
	if (mLightCount == 0)
	{
		switch (mLightType)
		{
			case kQ3LightTypeDirectional:
				AddDirectionalLight( inLight, inWorldToView );
				isRecognizedLightType = true;
				break;
				
			case kQ3LightTypePoint:
				AddPointLight( inLight, inWorldToView );
				isRecognizedLightType = true;
				break;
				
			case kQ3LightTypeSpot:
				AddSpotLight( inLight, inWorldToView );
				isRecognizedLightType = true;
				break;
		}
	}
	else
	{
		// Otherwise, there is no need to bother with traditional
		// glLight calls, and doing so could be trouble if there
		// are more than 8 lights in a pass.
		switch (mLightType)
		{
			case kQ3LightTypeDirectional:
			case kQ3LightTypePoint:
			case kQ3LightTypeSpot:
				isRecognizedLightType = true;
				break;
		}
	}
	
	if (isRecognizedLightType)
	{
		mRenderer.Shader().AddLight( inLight );
	}
	
	if (isRecognizedLightType)
	{
		++mLightCount;
	}
}

/*!
	@function	Reset
	
	@abstract	Initialize the lights.
*/
void	QORenderer::Lights::Reset( bool inEnableLighting )
{
	mLightCount = 0;
	
	if (inEnableLighting)
	{
		TQ3ColorRGB noLight = {
			0.0f, 0.0f, 0.0f
		};
		mRenderer.Shader().SetAmbientLight( noLight );
	}
	else
	{
		// This gets turned on in the shadow marking phase.  If we leave it
		// on, it can cause blending artifacts in shadow lighting phases.
		glDisable( GL_DEPTH_CLAMP_NV );
	}
}

/*!
	@function	ClassifyLights
	@abstract	At the start of a frame, classify the lights as ambient,
				shadowing, or non-shadowing.
*/
void	QORenderer::Lights::ClassifyLights( TQ3ViewObject inView )
{
	mGlAmbientLight.r = mGlAmbientLight.g = mGlAmbientLight.b = 0.0f;
	
	mNonShadowingLights.clear();
	mShadowingLights.clear();

	CQ3ObjectRef	theLightGroup( CQ3View_GetLightGroup( inView ) );
	Q3GroupIterator		iter( theLightGroup.get(), kQ3ShapeTypeLight );
	CQ3ObjectRef	theLight;
	
	while ( (theLight = iter.NextObject()).isvalid() )
	{
		TQ3Boolean	isOn;
		Q3Light_GetState( theLight.get(), &isOn );
		float	brightness;
		Q3Light_GetBrightness( theLight.get(), &brightness );
		
		if ( (isOn == kQ3True) && (brightness > kQ3RealZero) )
		{
			TQ3ObjectType	lightType = Q3Light_GetType( theLight.get() );
			
			if (lightType == kQ3LightTypeAmbient)
			{
				ConvertAmbientLight( theLight.get(), mGlAmbientLight );
			}
			else if ( ! IsIgnorablePositionalLight( theLight.get(), lightType, inView, brightness ) )
			{
				if ( IsShadowFrame() && IsShadowCaster( theLight.get(), lightType ) )
				{
					mShadowingLights.push_back( theLight );
				}
				else
				{
					mNonShadowingLights.push_back( theLight );
				}
			}
		}
	}
}


/*!
	@function	UseInfiniteYon
	@abstract	Set the far distance to infinity.
*/
void	QORenderer::Lights::UseInfiniteYon( TQ3ViewObject inView )
{
	CQ3ObjectRef	theCamera( CQ3View_GetCamera( inView ) );
	if (Q3Camera_GetType( theCamera.get() ) == kQ3CameraTypeViewAngleAspect)
	{
		TQ3CameraRange	theRange;
		Q3Camera_GetRange( theCamera.get(), &theRange );
		if (isfinite( theRange.yon ))
		{
			mSavedYon = theRange.yon;
			theRange.yon = std::numeric_limits<float>::infinity();
			Q3Camera_SetRange( theCamera.get(), &theRange );
		}
	}
}


/*!
	@function	EndFrame
	
	@abstract	Restore original yon.
*/
void	QORenderer::Lights::EndFrame(
								TQ3ViewObject inView )
{
	if (isfinite( mSavedYon ))
	{
		CQ3ObjectRef	theCamera( CQ3View_GetCamera( inView ) );
		TQ3CameraRange	theRange;
		Q3Camera_GetRange( theCamera.get(), &theRange );
		theRange.yon = mSavedYon;
		Q3Camera_SetRange( theCamera.get(), &theRange );
	}
}


/*!
	@function	SetUpShadowMarkingPass
	@abstract	Perform initialization for the start of a shadow marking pass.
*/
void	QORenderer::Lights::SetUpShadowMarkingPass( TQ3ViewObject inView,
						const TQ3Matrix4x4& inWorldToView )
{
	// Although we do not use lighting until the shadow lighting pass,
	// we do need the light position, so we may as well set up the
	// light now.
	AddLight( mShadowingLights[ mStartingLightIndexForPass ].get(),
		inWorldToView );
	mLightCount = 0;
	mRenderer.Shader().UpdateIllumination( kQ3IlluminationTypeNULL );
	
	// This should help prevent problems if shadow-casting geometry is in
	// front of the near plane.
	glEnable( GL_DEPTH_CLAMP_NV );

	glDisable( GL_CULL_FACE );
	
	// This helps prevent z-fighting artifacts.
	// It is unclear what are the best values to pass.
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 1.0f, 1.0f );
	
	// do not write to color buffer or depth buffer, only stencil
	E3View_State_SetStyleWriteSwitch( inView, 0 );
	
	// Do write to stencil buffer
	glStencilMask( ~0U );
	
	// Clear stencil buffer
	glClearStencil( 128 );
	glClear( GL_STENCIL_BUFFER_BIT );
	
	glEnable( GL_STENCIL_TEST );
	
	GLenum	decrEnum = GL_DECR_WRAP_EXT;
	GLenum	incrEnum = GL_INCR_WRAP_EXT;
	
	mRenderer.Funcs().glStencilOpSeparate(
		GL_BACK,
		GL_KEEP,			// stencil test fail
		incrEnum, 			// depth test fail
		GL_KEEP );			// depth test pass
	mRenderer.Funcs().glStencilOpSeparate(
		GL_FRONT,
		GL_KEEP,			// stencil test fail
		decrEnum,			// depth test fail
		GL_KEEP );			// depth test pass
	glStencilMask( ~0U );
	glStencilFunc( GL_ALWAYS, 0, ~0U );

	glDepthFunc( GL_LESS );
	glDisable( GL_BLEND );

	mIsAnotherPassNeeded = true;
}

/*!
	@function	SetUpShadowLightingPass
	@abstract	Perform initialization for the start of a shadow lighting pass.
*/
void	QORenderer::Lights::SetUpShadowLightingPass(TQ3ViewObject inView)
{
	mLightCount = 1;
	mGlAmbientLight.r = mGlAmbientLight.g = mGlAmbientLight.b = 0.0f;
	
	mStartingLightIndexForPass += 1;
	if (mStartingLightIndexForPass < mShadowingLights.size())
	{
		mIsAnotherPassNeeded = true;
	}

	// do write to color buffer, not to depth
	E3View_State_SetStyleWriteSwitch( inView, kQ3WriteSwitchMaskColor );
	
	glStencilFunc( GL_EQUAL, 128, ~0U );
	glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
	glStencilMask( 0 );

	glDepthFunc( GL_LEQUAL );	// pass depth test on equal
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE );
}


/*!
	@function	SetUpNonShadowLightingPass
	@abstract	Perform initialization for the start of a non-shadow lighting pass.
*/
void	QORenderer::Lights::SetUpNonShadowLightingPass( const TQ3Matrix4x4& inWorldToView )
{
	if (mIsFirstPass)
	{
		// A previous Reset call turned off ambient light.
		mRenderer.Shader().SetAmbientLight( mGlAmbientLight );
	}
	
	TQ3Uns32 endIndex = E3Num_Min<TQ3Uns32>( static_cast<TQ3Uns32>(mNonShadowingLights.size()),
		mStartingLightIndexForPass + mMaxGLLights );
	TQ3Uns32	lightIndex;
	TQ3Uns32	handledNonAmbientLights = 0;
	
	for (lightIndex = mStartingLightIndexForPass;
		lightIndex < endIndex;
		++lightIndex)
	{
		AddLight( mNonShadowingLights[ lightIndex ].get(), inWorldToView );
		++handledNonAmbientLights;
	}
	
	if (lightIndex == mNonShadowingLights.size())
	{
		// Done with non-shadowing lights.  Any shadowing lights?
		if (! mShadowingLights.empty())
		{
			mIsAnotherPassNeeded = true;
			mIsNextPassShadowPhase = true;
			mIsShadowMarkingPass = false;	// will toggle at next start
			mStartingLightIndexForPass = 0;
		}
	}
	else	// more non-shadowing lights to handle
	{
		mIsAnotherPassNeeded = true;
		mStartingLightIndexForPass += handledNonAmbientLights;
	}

	if (! mIsFirstPass)
	{
		glDepthMask( GL_FALSE );	// no writes to depth buffer
		glDepthFunc( GL_LEQUAL );	// pass depth test on equal
		glEnable( GL_BLEND );
		glBlendFunc( GL_ONE, GL_ONE );
		
		// I was seeing a problem where I had 9 identically configured
		// spot lights, but the one rendered in the 2nd pass produced a
		// fainter spot.  This seems to fix it.
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( -1.0f, -1.0f );
	}
}


/*!
	@function	StartFrame
	
	@abstract	Initialize lights for the start of a frame.
*/
void	QORenderer::Lights::StartFrame( TQ3ViewObject inView,
										bool inIsShadowing )
{
	mIsShadowFrame = inIsShadowing;
	mIsFirstPass = true;
	mIsShadowPhase = false;
	mIsNextPassShadowPhase = false;
	mIsShadowMarkingPass = false;
	mStartingLightIndexForPass = 0;
	mSavedYon = std::numeric_limits<float>::infinity();

	// How many OpenGL non-ambient lights can we have?
	mRenderer.Shader().StartFrame( inView );
	mMaxGLLights = mRenderer.Shader().GetMaxLightsPerPass();
	
	ClassifyLights( inView );
	
	// If we are going to do shadow volumes, we need infinite yon.
	if (! mShadowingLights.empty())
	{
		UseInfiniteYon( inView );
		
		CQ3ObjectRef	theRenderer( CQ3View_GetRenderer( inView ) );
		
		if (kQ3Failure == Q3Object_GetProperty( theRenderer.get(),
			kQ3RendererPropertyAttenuationThreshold,
			sizeof(mAttenuatedLightThreshold), nullptr,
			&mAttenuatedLightThreshold ))
		{
			// We need to know the minimum color that is distinguishable from
			// black, which depends on the bit depth of the color buffer.
			GLint	colorBits = 8;
			
			// We used to be able to query the number of color bits using
			//   glGetIntegerv( GL_GREEN_BITS, &colorBits );
			// but that is no longer allowed.
			// In some GL version, but apparently not 3.2, one can use
			// glGetFramebufferAttachmentParameteriv to query
			// GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE.  There does not seem to
			// be a way to do it in the default framebuffer in core profile 3.2.
			// So I'll just assume I'm using 24-bit color.
			
			mAttenuatedLightThreshold = 1.0f / (1L << colorBits);
		}
	}
}

/*!
	@function	StartPass
	
	@abstract	Set up the lights for the start of a pass.
*/
void	QORenderer::Lights::StartPass(
								TQ3ViewObject inView,
								TQ3CameraObject inCamera,
								TQ3RendererObject inRenderer )
{
	mIsLowDMode = false;
	mIsAnotherPassNeeded = false;
	
	Reset( true );
	
	// Get the Quesa world->view transform, and reset the OpenGL model->view
	// transform to the identity. Since lights are translated before geometry,
	// we need to transform their direction/positions ourselves in order to
	// put them into the OpenGL camera's coordinate system.
	TQ3Matrix4x4	savedModelViewMatrix = mRenderer.GetMatrixState().GetLocalToCamera();
	TQ3Matrix4x4	worldToView, ident;
	Q3Camera_GetWorldToView( inCamera, &worldToView );
	Q3Matrix4x4_SetIdentity( &ident );
	GLCamera_SetModelView( &ident, mRenderer.Shader() );
	
	if (mIsNextPassShadowPhase)
	{
		mIsShadowPhase = true;
		mIsNextPassShadowPhase = false;
	}
	
	
	TQ3RendererPassInfo	passInfo;
	

	if (mIsShadowPhase)
	{
		mIsShadowMarkingPass = ! mIsShadowMarkingPass;
		
		passInfo.light = mShadowingLights[ mStartingLightIndexForPass ].get();
		
		if (mIsShadowMarkingPass)
		{
			//Q3_MESSAGE_FMT("Shadow marking pass");
			passInfo.passType = kQ3RendererPassShadowMarking;
			
			mRenderer.Shader().ClearLights();
			SetUpShadowMarkingPass( inView, worldToView );
		}
		else	// shadow lighting pass
		{
			//Q3_MESSAGE_FMT("Shadow lighting pass");
			passInfo.passType = kQ3RendererPassShadowLighting;
			
			SetUpShadowLightingPass( inView );
		}
	}
	else	// non-shadowing phase
	{
		//Q3_MESSAGE_FMT("Non-shadow pass");
		passInfo.light = nullptr;
		passInfo.passType = kQ3RendererPassNonShadow;
		
		mRenderer.Shader().ClearLights();
		SetUpNonShadowLightingPass( worldToView );
	}
	
	GLCamera_SetModelView( &savedModelViewMatrix, mRenderer.Shader() );
	
	Q3Object_SetProperty( inRenderer, kQ3RendererPropertyPassType,
		sizeof(passInfo), &passInfo );
}


/*!
	@function	EndPass
	
	@abstract	Turn off the lights at the end of a pass.
	
	@result		True if we are done with all lights.
*/
bool	QORenderer::Lights::EndPass()
{
	Reset( false );
		
	mIsFirstPass = ! mIsAnotherPassNeeded; // mIsFirstPass becames false only in the second pass DUE to the lights
	
	return mIsAnotherPassNeeded;
}


/*!
	@function	IsShadowMarkingPass
	@abstract	Test whether this is a pass that is marking shadows in the
				stencil buffer.
*/
bool	QORenderer::Lights::IsShadowMarkingPass() const
{
	return mIsShadowPhase && mIsShadowMarkingPass;
}


/*!
	@function	IsShadowPhase
	@abstract	Test whether this is a shadow marking or lighting pass.
*/
bool	QORenderer::Lights::IsShadowPhase() const
{
	return mIsShadowPhase;
}


/*!
	@function	SetLowDimensionalMode
	
	@abstract	Set whether we use special illumination for 0-D or 1-D geometry.
	
	@discussion	It's not clear whether it makes sense for geometries of
				dimension 0 or 1 (Point, Line, PolyLine, Ellipse, NURBCurve) to
				be affected by the direction of the light and by normal
				vectors.  Therefore, when we are about to render geometries of
				dimension 0 or 1, we switch to nondirectional illumination.
				Note that this is not the same as using NULL illumination,
				where brightness is ignored.
				Of course, when we are ready to render 2-dimensional geometry,
				we must make sure that normal illumination is restored.
*/
void	QORenderer::Lights::SetLowDimensionalMode( bool inLowD, TQ3ObjectType inIlluminationType )
{
	if (mIsLowDMode != inLowD)
	{
		mIsLowDMode = inLowD;
		
		if (inLowD)
		{
			if (inIlluminationType != kQ3IlluminationTypeNULL)
			{
				mRenderer.Shader().UpdateIllumination( kQ3IlluminationTypeNondirectional );
			}
			else
			{
				mRenderer.Shader().UpdateIllumination( inIlluminationType );
			}
		}
	}
}


/*!
	@function	IsEmissionUsed
	@abstract	Are we using emissive light in this pass?
*/
bool	QORenderer::Lights::IsEmissionUsed() const
{
	return mIsFirstPass;
}

void	QORenderer::Lights::MarkShadowOfTriMesh(
								TQ3GeometryObject inTMObject,
								const TQ3TriMeshData& inTMData,
								const TQ3Vector3D* inFaceNormals,
								TQ3ViewObject inView )
{
	mShadowMarker.MarkShadowOfTriMesh( inTMObject, inTMData, inFaceNormals,
		mShadowingLights[ mStartingLightIndexForPass ].get(), inView );
}

void	QORenderer::Lights::MarkShadowOfTriangle(
								const Vertex* inVertices )
{
	mShadowMarker.MarkShadowOfTriangle( inVertices );
}


/*!
	@function	IsLit
	@abstract	Test whether a nonzero amount of the current light may be
				shining on a certain region.  Used for culling during the
				shadow phase.
*/
bool	QORenderer::Lights::IsLit( const TQ3BoundingBox& inBounds ) const
{
	bool	isLit = true;
	
	// Only do culling in shadow phase for positional lights
	
	if ( mIsShadowPhase && (mLightType != kQ3LightTypeDirectional) )
	{
		// Find the bounds in eye (camera) coordinates.
		TQ3BoundingBox		cameraBounds;
		E3BoundingBox_Transform( &inBounds, &mRenderer.GetMatrixState().GetLocalToCamera(),
			&cameraBounds );
		
		// Find the distance from the light to the object
		TQ3Point3D	lightPos = {
			mGLLightPosition[0], mGLLightPosition[1], mGLLightPosition[2]
		};
		float	theDistanceSq = E3Point3D_BoundingBox_DistanceSquared(
			&lightPos, &cameraBounds, nullptr );
		
		if (theDistanceSq > kQ3RealZero)
		{
			// Find the brightness as attenuated by distance
			float	attenuatedLight = mLightBrightness;
			switch (mLightAttenuation)
			{
				case kQ3AttenuationTypeInverseDistance:
					attenuatedLight /= sqrt( theDistanceSq );
					break;
					
				case kQ3AttenuationTypeInverseDistanceSquared:
					attenuatedLight /= theDistanceSq;
					break;
				
				default:
					break;
			}
			
			if (attenuatedLight < mAttenuatedLightThreshold)
			{
				isLit = false;
			}
			
			if ( isLit && (mLightType == kQ3LightTypeSpot) )
			{
				// Cull objects outside the light cone
				TQ3Ray3D	spotAxis;
				spotAxis.origin = lightPos;
				spotAxis.direction = mSpotLightDirection;
				if (! E3Cone_IntersectBoundingBox( spotAxis, mSpotAngleCosine,
					cameraBounds ))
				{
					isLit = false;
				}
			}
		}
	}
	
	return isLit;
}



/*!
	@function			GetShadowingLightPosition
	@abstract			During a shadow marking or lighting pass (in which
						exactly one light is active), get the position of 
						the light.
	@discussion			If the light is a point or spot light, we return the
						position as a finite point, with w component 1.  If
						the light is directional, we return an infinite point,
						with w = 0 and x, y, z being the direction vector.
*/
TQ3RationalPoint4D	QORenderer::Lights::GetShadowingLightPosition() const
{
	TQ3RationalPoint4D resultPlace = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	if (mIsShadowPhase)
	{
		CQ3ObjectRef theLight( mShadowingLights[ mStartingLightIndexForPass ] );
		
		switch (mLightType)
		{
			case kQ3LightTypeDirectional:
				{
					TQ3Vector3D dir;
					Q3DirectionalLight_GetDirection( theLight.get(), &dir );
					resultPlace.x = dir.x;
					resultPlace.y = dir.y;
					resultPlace.z = dir.z;
					resultPlace.w = 0.0f;
				}
				break;
			
			case kQ3LightTypeSpot:
				{
					TQ3Point3D spotLoc;
					Q3SpotLight_GetLocation( theLight.get(), &spotLoc );
					resultPlace.x = spotLoc.x;
					resultPlace.y = spotLoc.y;
					resultPlace.z = spotLoc.z;
					resultPlace.w = 1.0f;
				}
				break;
			
			case kQ3LightTypePoint:
				{
					TQ3Point3D pointLoc;
					Q3PointLight_GetLocation( theLight.get(), &pointLoc );
					resultPlace.x = pointLoc.x;
					resultPlace.y = pointLoc.y;
					resultPlace.z = pointLoc.z;
					resultPlace.w = 1.0f;
				}
				break;
		}
	}
	
	return resultPlace;
}


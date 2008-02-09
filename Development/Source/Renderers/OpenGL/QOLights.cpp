/*  NAME:
       QOLights.cpp

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
#include "QOLights.h"
#include "QORenderer.h"
#include "Q3GroupIterator.h"
#include "E3ErrorManager.h"
#include "E3Math.h"
#include "E3Math_Intersect.h"
#include "CQ3ObjectRef_Gets.h"
#include <cmath>
#include <limits>
using namespace std;



//=============================================================================
//     Local constants
//-----------------------------------------------------------------------------


// These constants are defined in the Mac framework version of gl.h,
// but if we are building a CFM binary, we may be using an older header.
// Such a situation could also occur on other platforms.
// The numbers are defined in the EXT_separate_specular_color specification.
#ifndef GL_LIGHT_MODEL_COLOR_CONTROL
	#define GL_LIGHT_MODEL_COLOR_CONTROL      0x81F8
	#define GL_SINGLE_COLOR                   0x81F9
	#define GL_SEPARATE_SPECULAR_COLOR        0x81FA
#endif

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
static void ConvertAmbientLight( TQ3LightObject inLight, GLfloat* ioAmbientColor )
{
	// Get light data
	TQ3LightData		lightData;
	Q3Light_GetData( inLight, &lightData );
	
	// Find the light color
	GLfloat		lightColor[4];
	CalcLightColor( lightData, lightColor );
	
	// Add this light into the ambient illumination
	for (int i = 0; i < 3; ++i)	// deliberately not touching component 3
	{
		ioAmbientColor[i] += lightColor[i];
	}
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

//=============================================================================
//      Class Implementations
//-----------------------------------------------------------------------------

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
	Q3Vector3D_Transform( &lightData.direction, &inWorldToView,
		&lightData.direction );
	
	// Normalize the direction, to help per-pixel lighting.
	Q3FastVector3D_Normalize( &lightData.direction, &lightData.direction );
	
	// Set up the light direction as OpenGL likes it
	mGLLightPosition[0] = -lightData.direction.x;
	mGLLightPosition[1] = -lightData.direction.y;
	mGLLightPosition[2] = -lightData.direction.z;
	mGLLightPosition[3] = 0.0f;

	// Find the light color
	GLfloat		lightColor[4];
	CalcLightColor( lightData.lightData, lightColor );

	// Set up the light
	GLenum	lightIndex = GL_LIGHT0 + mLightCount;
	glEnable( lightIndex );
	glLighti( lightIndex, GL_SPOT_CUTOFF, 180 );
	glLightfv( lightIndex, GL_DIFFUSE,  lightColor );
	glLightfv( lightIndex, GL_SPECULAR, lightColor );
	glLightfv( lightIndex, GL_POSITION, mGLLightPosition );
	glLightf( lightIndex, GL_CONSTANT_ATTENUATION, 1.0f );
	glLightf( lightIndex, GL_LINEAR_ATTENUATION, 0.0f );
	glLightf( lightIndex, GL_QUADRATIC_ATTENUATION, 0.0f );
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
	Q3Point3D_Transform( &lightData.location, &inWorldToView,
		&lightData.location );
	
	// and put the location in OpenGL form
	mGLLightPosition[0] = lightData.location.x;
	mGLLightPosition[1] = lightData.location.y;
	mGLLightPosition[2] = lightData.location.z;
	mGLLightPosition[3] = 1.0f;

	// Find the light color
	GLfloat		lightColor[4];
	CalcLightColor( lightData.lightData, lightColor );
	
	// Find attenuation values
	GLfloat		attConstant, attLinear, attQuadratic;
	mLightAttenuation = lightData.attenuation;
	SetAttenuationParams( lightData.attenuation,
		attConstant, attLinear, attQuadratic );

	// Set up the light
	GLenum	lightIndex = GL_LIGHT0 + mLightCount;
	glEnable( lightIndex );
	glLighti( lightIndex, GL_SPOT_CUTOFF, 180 );
	glLightfv( lightIndex, GL_DIFFUSE, lightColor );
	glLightfv( lightIndex, GL_SPECULAR, lightColor );
	glLightfv( lightIndex, GL_POSITION, mGLLightPosition );
	glLightf( lightIndex, GL_CONSTANT_ATTENUATION, attConstant );
	glLightf( lightIndex, GL_LINEAR_ATTENUATION, attLinear );
	glLightf( lightIndex, GL_QUADRATIC_ATTENUATION, attQuadratic );
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
	Q3Point3D_Transform( &lightData.location, &inWorldToView,
		&lightData.location );
	Q3Vector3D_Transform( &lightData.direction, &inWorldToView,
		&mSpotLightDirection );
	
	// Normalize the spot direction... this is probably not needed for the
	// fixed-function pipeline, but saves a little work for per-pixel lighting.
	Q3FastVector3D_Normalize( &mSpotLightDirection, &mSpotLightDirection );
		
	// Put direction and location in OpenGL form
	GLfloat		lightDirection[3];
	lightDirection[0] = mSpotLightDirection.x;
	lightDirection[1] = mSpotLightDirection.y;
	lightDirection[2] = mSpotLightDirection.z;

	mGLLightPosition[0] = lightData.location.x;
	mGLLightPosition[1] = lightData.location.y;
	mGLLightPosition[2] = lightData.location.z;
	mGLLightPosition[3] = 1.0f;
	
	// Find the light color
	GLfloat		lightColor[4];
	CalcLightColor( lightData.lightData, lightColor );
	
	// Find attenuation values
	GLfloat		attConstant, attLinear, attQuadratic;
	mLightAttenuation = lightData.attenuation;
	SetAttenuationParams( lightData.attenuation,
		attConstant, attLinear, attQuadratic );
	
	// Find the cutoff angle
	GLfloat		lightOuterAngle = Q3Math_RadiansToDegrees( lightData.outerAngle );
	mSpotAngleCosine = cos( lightData.outerAngle );
	
	
	// Set up the light
	GLenum	lightIndex = GL_LIGHT0 + mLightCount;
	glEnable( lightIndex );
	glLightfv( lightIndex, GL_DIFFUSE, lightColor );
	glLightfv( lightIndex, GL_SPECULAR, lightColor );
	glLightfv( lightIndex, GL_POSITION, mGLLightPosition );
	glLightfv( lightIndex, GL_SPOT_DIRECTION, lightDirection );
	glLightf( lightIndex, GL_SPOT_CUTOFF, lightOuterAngle );
	glLightf( lightIndex, GL_CONSTANT_ATTENUATION, attConstant );
	glLightf( lightIndex, GL_LINEAR_ATTENUATION, attLinear );
	glLightf( lightIndex, GL_QUADRATIC_ATTENUATION, attQuadratic );
}

/*!
	@function	AddLight
	
	@abstract	Turn a Quesa light object into OpenGL light.
*/
void QORenderer::Lights::AddLight( TQ3LightObject inLight,
						const TQ3Matrix4x4& inWorldToView )
{
	mLightType = Q3Light_GetType( inLight );
	
	switch (mLightType)
	{
		case kQ3LightTypeDirectional:
			AddDirectionalLight( inLight, inWorldToView );
			++mLightCount;
			break;
			
		case kQ3LightTypePoint:
			AddPointLight( inLight, inWorldToView );
			++mLightCount;
			break;
			
		case kQ3LightTypeSpot:
			AddSpotLight( inLight, inWorldToView );
			++mLightCount;
			break;
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
		glEnable( GL_LIGHTING );
		glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );
		
		GLfloat	noAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, noAmbient );
		
		if (mGLExtensions.separateSpecularColor == kQ3True)
		{
			glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL,
				GL_SEPARATE_SPECULAR_COLOR );
		}
	}
	else
	{
		glDisable( GL_LIGHTING );
	}
}

/*!
	@function	ClassifyLights
	@abstract	At the start of a frame, classify the lights as ambient,
				shadowing, or non-shadowing.
*/
void	QORenderer::Lights::ClassifyLights( TQ3ViewObject inView )
{
	mGlAmbientLight[0] = mGlAmbientLight[1] = mGlAmbientLight[2] = 0.0f;
	mGlAmbientLight[3] = 1.0f;
	
	mNonShadowingLights.clear();
	mShadowingLights.clear();

	CQ3ObjectRef	theLightGroup( CQ3View_GetLightGroup( inView ) );
	Q3GroupIterator		iter( theLightGroup.get(), kQ3ShapeTypeLight );
	CQ3ObjectRef	theLight;
	
	while ( (theLight = iter.NextObject()).isvalid() )
	{
		TQ3Boolean	isOn;
		Q3Light_GetState( theLight.get(), &isOn );
		
		if (isOn == kQ3True)
		{
			TQ3ObjectType	lightType = Q3Light_GetType( theLight.get() );
			
			if (lightType == kQ3LightTypeAmbient)
			{
				ConvertAmbientLight( theLight.get(), mGlAmbientLight );
			}
			else
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
void	QORenderer::Lights::SetUpShadowMarkingPass( const TQ3Matrix4x4& inWorldToView )
{
	// Although we do not use lighting until the shadow lighting pass,
	// we do need the light position, so we may as well set up the
	// light now.
	AddLight( mShadowingLights[ mStartingLightIndexForPass ].get(),
		inWorldToView );
	glDisable( GL_LIGHT0 );
	glDisable( GL_LIGHTING );
	
	if (mGLExtensions.depthClamp)
	{
		// This should help prevent problems if shadow-casting geometry is in
		// front of the near plane.
		glEnable( GL_DEPTH_CLAMP_NV );
	}

	glDisable( GL_CULL_FACE );
	
	// This helps prevent z-fighting artifacts.
	// It is unclear what are the best values to pass.
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 1.0f, 1.0f );
	
	// do not write to color buffer
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	
	// Do write to stencil buffer
	glStencilMask( ~0 );
	
	// Clear stencil buffer
	glClearStencil( 128 );
	glClear( GL_STENCIL_BUFFER_BIT );
	
	glEnable( GL_STENCIL_TEST );
	
	GLenum	decrEnum = mGLExtensions.stencilWrap? GL_DECR_WRAP_EXT : GL_DECR;
	GLenum	incrEnum = mGLExtensions.stencilWrap? GL_INCR_WRAP_EXT : GL_INCR;
	
	if (mGLExtensions.separateStencil)
	{
		mGLStencilFuncs.glStencilOpSeparate(
			GL_BACK,
			GL_KEEP,			// stencil test fail
			incrEnum, 			// depth test fail
			GL_KEEP );			// depth test pass
		mGLStencilFuncs.glStencilOpSeparate(
			GL_FRONT,
			GL_KEEP,			// stencil test fail
			decrEnum,			// depth test fail
			GL_KEEP );			// depth test pass
		glStencilMask( ~0 );
		glStencilFunc( GL_ALWAYS, 0, ~0 );
	}
	else if (mGLExtensions.stencilTwoSide)
	{
		glEnable( GL_STENCIL_TEST_TWO_SIDE_EXT );
		
		mGLStencilFuncs.glActiveStencilFace( GL_BACK );
		glStencilOp( GL_KEEP,	// stencil test fail
			incrEnum, 			// depth test fail
			GL_KEEP );			// depth test pass
		glStencilMask( ~0 );
		glStencilFunc( GL_ALWAYS, 0, ~0 );

		mGLStencilFuncs.glActiveStencilFace( GL_FRONT );
		glStencilOp( GL_KEEP,	// stencil test fail
			decrEnum,			// depth test fail
			GL_KEEP );			// depth test pass
		glStencilMask( ~0 );
		glStencilFunc( GL_ALWAYS, 0, ~0 );
	}

	glDepthMask( GL_FALSE );	// no writes to depth buffer
	glDepthFunc( GL_LESS );
	glDisable( GL_BLEND );

	mIsAnotherPassNeeded = true;
}

/*!
	@function	SetUpShadowLightingPass
	@abstract	Perform initialization for the start of a shadow lighting pass.
*/
void	QORenderer::Lights::SetUpShadowLightingPass()
{
	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHTING );
	mLightCount = 1;
	mGlAmbientLight[0] = mGlAmbientLight[1] = mGlAmbientLight[2] = 0.0f;
	
	mStartingLightIndexForPass += 1;
	if (mStartingLightIndexForPass < mShadowingLights.size())
	{
		mIsAnotherPassNeeded = true;
	}

	// do write to color buffer
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	
	if (mGLExtensions.separateStencil)
	{
		glStencilFunc( GL_EQUAL, 128, ~0 );
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		glStencilMask( 0 );
	}
	else if (mGLExtensions.stencilTwoSide)
	{
		mGLStencilFuncs.glActiveStencilFace( GL_BACK );
		glStencilFunc( GL_EQUAL, 128, ~0 );
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		glStencilMask( 0 );
		
		mGLStencilFuncs.glActiveStencilFace( GL_FRONT );
		glStencilFunc( GL_EQUAL, 128, ~0 );
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		glStencilMask( 0 );
	}

	glDepthMask( GL_FALSE );	// no writes to depth buffer
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
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, mGlAmbientLight );
	}
	
	TQ3Uns32 endIndex = E3Num_Min<unsigned>( mNonShadowingLights.size(),
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
	mStartingLightIndexForPass = 0;
	mSavedYon = std::numeric_limits<float>::infinity();

	// How many OpenGL non-ambient lights can we have?
	glGetIntegerv( GL_MAX_LIGHTS, &mMaxGLLights );
	
	ClassifyLights( inView );
	
	// If we are going to do shadow volumes, we need infinite yon.
	if (! mShadowingLights.empty())
	{
		UseInfiniteYon( inView );
		
		CQ3ObjectRef	theRenderer( CQ3View_GetRenderer( inView ) );
		
		if (kQ3Failure == Q3Object_GetProperty( theRenderer.get(),
			kQ3RendererPropertyAttenuationThreshold,
			sizeof(mAttenuatedLightThreshold), NULL,
			&mAttenuatedLightThreshold ))
		{
			// We need to know the minimum color that is distinguishable from
			// black, which depends on the bit depth of the color buffer.
			GLint	colorBits;
			glGetIntegerv( GL_GREEN_BITS, &colorBits );
			mAttenuatedLightThreshold = 1.0f / (1L << colorBits);
		}
	}
}

/*!
	@function	StartPass
	
	@abstract	Set up the lights for the start of a pass.
*/
void	QORenderer::Lights::StartPass(
								TQ3CameraObject inCamera,
								TQ3GroupObject inLights )
{
#pragma unused( inLights )
	mIsOnlyAmbient = false;
	mIsAnotherPassNeeded = false;
	
	Reset( true );
	
	// Get the Quesa world->view transform, and reset the OpenGL model->view
	// transform to the identity. Since lights are translated before geometry,
	// we need to transform their direction/positions ourselves in order to
	// put them into the OpenGL camera's coordinate system.
	GLfloat				savedModelViewMatrix[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, savedModelViewMatrix );
	TQ3Matrix4x4	worldToView;
	Q3Camera_GetWorldToView( inCamera, &worldToView );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	

	if (mIsNextPassShadowPhase)
	{
		mIsShadowPhase = true;
		mIsNextPassShadowPhase = false;
	}
	
	if (mIsShadowPhase)
	{
		mIsShadowMarkingPass = ! mIsShadowMarkingPass;
		
		if (mIsShadowMarkingPass)
		{
			SetUpShadowMarkingPass( worldToView );
		}
		else	// shadow lighting pass
		{
			SetUpShadowLightingPass();
		}
	}
	else	// non-shadowing phase
	{
		SetUpNonShadowLightingPass( worldToView );
	}
	
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( savedModelViewMatrix );
}


/*!
	@function	EndPass
	
	@abstract	Turn off the lights at the end of a pass.
	
	@result		True if we are done with all lights.
*/
bool	QORenderer::Lights::EndPass()
{
	for (int i = 0; i < mLightCount; ++i)
	{
		glDisable( GL_LIGHT0 + i );
	}
	
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
	@function	SetOnlyAmbient
	
	@abstract	Set whether or not all light is ambient.
	
	@discussion	It's not clear whether it makes sense for geometries of
				dimension 0 or 1 (Point, Line, PolyLine, Ellipse, NURBCurve) to
				be affected by the direction of the light and by normal
				vectors.  Therefore, when we are about to render geometries of
				dimension 0 or 1, we replace each non-ambient light by ambient
				light of the same brightness.  Note that this is not the same
				as using NULL illumination, where brightness is ignored.
				Of course, when we are ready to render 2-dimensional geometry,
				we must make sure that the non-ambient lights are restored.
*/
void	QORenderer::Lights::SetOnlyAmbient( bool inOnlyAmbient )
{
	int i, j;
	
	if (mIsOnlyAmbient != inOnlyAmbient)
	{
		mIsOnlyAmbient = inOnlyAmbient;
		
		if (inOnlyAmbient)
		{
			for (i = 0; i < 4; ++i)
			{
				mOnlyAmbient[i] = mGlAmbientLight[i];
			}
		
			for (i = 0; i < mLightCount; ++i)
			{
				GLfloat	lightColor[4];
				glGetLightfv( GL_LIGHT0 + i, GL_DIFFUSE, lightColor );
				glDisable( GL_LIGHT0 + i );
				
				for (j = 0; j < 3; ++j)
				{
					mOnlyAmbient[j] += lightColor[j];
				}
			}
			
			glLightModelfv( GL_LIGHT_MODEL_AMBIENT, mOnlyAmbient );
		}
		else	// restore normal state
		{
			for (i = 0; i < mLightCount; ++i)
			{
				glEnable( GL_LIGHT0 + i );
			}
			
			glLightModelfv( GL_LIGHT_MODEL_AMBIENT, mGlAmbientLight );
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
								const TQ3Vector3D* inFaceNormals )
{
	mShadowMarker.MarkShadowOfTriMesh( inTMObject, inTMData, inFaceNormals );
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
		E3BoundingBox_Transform( &inBounds, &mMatrixState.GetLocalToCamera(),
			&cameraBounds );
		
		// Find the distance from the light to the object
		TQ3Point3D	lightPos = {
			mGLLightPosition[0], mGLLightPosition[1], mGLLightPosition[2]
		};
		float	theDistanceSq = E3Point3D_BoundingBox_DistanceSquared(
			&lightPos, &cameraBounds, NULL );
		
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
			}
			
			if (attenuatedLight < mAttenuatedLightThreshold)
			{
				isLit = false;
			}
			
			if ( isLit && (mLightType == kQ3LightTypeSpot) )
			{
				// Cull objects outside the light cone
				TQ3Ray3D	spotAxis = {
					lightPos, mSpotLightDirection
				};
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

/*  NAME:
       QOLights.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

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
#include "Q3GroupIterator.h"
#include "E3ErrorManager.h"



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

	// Update the OpenGL state
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ioAmbientColor );
}

/*!
	@function	AddDirectionalLight
	
	@abstract	Turn a Quesa directional light object into OpenGL directional
				light.
*/
static void AddDirectionalLight( TQ3LightObject inLight,
								const TQ3Matrix4x4& inWorldToView,
								TQ3Uns32 inLightCount )
{
	// Get light data
	TQ3DirectionalLightData		lightData;
	Q3DirectionalLight_GetData( inLight, &lightData );
	
	// Transform the direction to view coordinates
	Q3Vector3D_Transform( &lightData.direction, &inWorldToView,
		&lightData.direction );
	
	// Set up the light direction as OpenGL likes it
	GLfloat		lightDirection[4];
	lightDirection[0] = -lightData.direction.x;
	lightDirection[1] = -lightData.direction.y;
	lightDirection[2] = -lightData.direction.z;
	lightDirection[3] = 0;

	// Find the light color
	GLfloat		lightColor[4];
	CalcLightColor( lightData.lightData, lightColor );

	// Set up the light
	GLenum	lightIndex = GL_LIGHT0 + inLightCount;
	glEnable( lightIndex );
	glLighti( lightIndex, GL_SPOT_CUTOFF, 180 );
	glLightfv( lightIndex, GL_DIFFUSE,  lightColor );
	glLightfv( lightIndex, GL_SPECULAR, lightColor );
	glLightfv( lightIndex, GL_POSITION, lightDirection );
	glLightf( lightIndex, GL_CONSTANT_ATTENUATION, 1.0f );
	glLightf( lightIndex, GL_LINEAR_ATTENUATION, 0.0f );
	glLightf( lightIndex, GL_QUADRATIC_ATTENUATION, 0.0f );
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

/*!
	@function	AddPointLight
	
	@abstract	Turn a Quesa point light object into OpenGL point
				light.
*/
static void AddPointLight( TQ3LightObject inLight,
								const TQ3Matrix4x4& inWorldToView,
								TQ3Uns32 inLightCount )
{
	// Get light data
	TQ3PointLightData	lightData;
	Q3PointLight_GetData( inLight, &lightData );

	// Transform the location to view coordinates
	Q3Point3D_Transform( &lightData.location, &inWorldToView,
		&lightData.location );
	
	// and put the location in OpenGL form
	GLfloat	lightPosition[4];
	lightPosition[0] = lightData.location.x;
	lightPosition[1] = lightData.location.y;
	lightPosition[2] = lightData.location.z;
	lightPosition[3] = 1.0f;

	// Find the light color
	GLfloat		lightColor[4];
	CalcLightColor( lightData.lightData, lightColor );
	
	// Find attenuation values
	GLfloat		attConstant, attLinear, attQuadratic;
	SetAttenuationParams( lightData.attenuation,
		attConstant, attLinear, attQuadratic );

	// Set up the light
	GLenum	lightIndex = GL_LIGHT0 + inLightCount;
	glEnable( lightIndex );
	glLighti( lightIndex, GL_SPOT_CUTOFF, 180 );
	glLightfv( lightIndex, GL_DIFFUSE, lightColor );
	glLightfv( lightIndex, GL_SPECULAR, lightColor );
	glLightfv( lightIndex, GL_POSITION, lightPosition );
	glLightf( lightIndex, GL_CONSTANT_ATTENUATION, attConstant );
	glLightf( lightIndex, GL_LINEAR_ATTENUATION, attLinear );
	glLightf( lightIndex, GL_QUADRATIC_ATTENUATION, attQuadratic );
}

/*!
	@function	AddSpotLight
	
	@abstract	Turn a Quesa point spot object into OpenGL spot
				light.
*/
static void AddSpotLight( TQ3LightObject inLight,
							const TQ3Matrix4x4& inWorldToView,
							TQ3Uns32 inLightCount )
{
	// Get light data
	TQ3SpotLightData	lightData;
	Q3SpotLight_GetData( inLight, &lightData );

	// Transform the direction and location to view coordinates
	Q3Point3D_Transform( &lightData.location, &inWorldToView,
		&lightData.location );
	Q3Vector3D_Transform( &lightData.direction, &inWorldToView,
		&lightData.direction );
		
	// Put direction and location in OpenGL form
	GLfloat		lightDirection[3];
	lightDirection[0] = lightData.direction.x;
	lightDirection[1] = lightData.direction.y;
	lightDirection[2] = lightData.direction.z;
	GLfloat		lightLocation[4];
	lightLocation[0] = lightData.location.x;
	lightLocation[1] = lightData.location.y;
	lightLocation[2] = lightData.location.z;
	lightLocation[3] = 1.0f;
	
	// Find the light color
	GLfloat		lightColor[4];
	CalcLightColor( lightData.lightData, lightColor );
	
	// Find attenuation values
	GLfloat		attConstant, attLinear, attQuadratic;
	SetAttenuationParams( lightData.attenuation,
		attConstant, attLinear, attQuadratic );
	
	// Find the cutoff angle
	GLfloat		lightOuterAngle = Q3Math_RadiansToDegrees( lightData.outerAngle );
	
	
	// Set up the light
	GLenum	lightIndex = GL_LIGHT0 + inLightCount;
	glEnable( lightIndex );
	glLightfv( lightIndex, GL_DIFFUSE, lightColor );
	glLightfv( lightIndex, GL_SPECULAR, lightColor );
	glLightfv( lightIndex, GL_POSITION, lightLocation );
	glLightfv( lightIndex, GL_SPOT_DIRECTION, lightDirection );
	glLightf( lightIndex, GL_SPOT_CUTOFF, lightOuterAngle );
	glLightf( lightIndex, GL_CONSTANT_ATTENUATION, attConstant );
	glLightf( lightIndex, GL_LINEAR_ATTENUATION, attLinear );
	glLightf( lightIndex, GL_QUADRATIC_ATTENUATION, attQuadratic );
}


//=============================================================================
//      Class Implementations
//-----------------------------------------------------------------------------

/*!
	@function	Reset
	
	@abstract	Initialize the lights.
*/
void	QORenderer::Lights::Reset( TQ3Uns32 inNumQ3Lights )
{
	mLightCount = 0;
	
	mGlAmbientLight[0] = mGlAmbientLight[1] = mGlAmbientLight[2] = 0.0f;
	mGlAmbientLight[3] = 1.0f;
	
	if (inNumQ3Lights > 0)
	{
		glEnable( GL_LIGHTING );
		glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, mGlAmbientLight );
		
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
	@function	StartFrame
	
	@abstract	Initialize lights for the start of a frame.
*/
void	QORenderer::Lights::StartFrame()
{
	mIsFirstPass = true;
	mStartingLightIndexForPass = 0;
}

/*!
	@function	StartPass
	
	@abstract	Set up the lights for the start of a pass.
*/
void	QORenderer::Lights::StartPass(
								TQ3CameraObject inCamera,
								TQ3GroupObject inLights )
{
	mIsOnlyAmbient = false;
	mIsAnotherPassNeeded = false;
	
	// How many Quesa lights do we have?
	TQ3Uns32	numQuesaLights;
	Q3Group_CountObjects( inLights, &numQuesaLights );
	
	// Reset lighting state.  If there are no lights, we are done.
	Reset( numQuesaLights );
	if (numQuesaLights == 0)
	{
		return;
	}
	
	// How many OpenGL non-ambient lights can we have?
	GLint		maxGLLights;
	glGetIntegerv( GL_MAX_LIGHTS, &maxGLLights );
	
	
	// Get the Quesa world->view transform, and reset the OpenGL model->view
	// transform to the identity. Since lights are translated before geometry,
	// we need to transform their direction/positions ourselves in order to
	// put them into the OpenGL camera's coordinate system.
	GLfloat				glMatrix[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, glMatrix );
	TQ3Matrix4x4	worldToView;
	Q3Camera_GetWorldToView( inCamera, &worldToView );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	
	// Process each enabled light
	TQ3Uns32	lightIndex = 0;
	TQ3Uns32	handledNonAmbientLights = 0;
	Q3GroupIterator		iter( inLights, kQ3ShapeTypeLight );
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
				if (mIsFirstPass)
				{
					ConvertAmbientLight( theLight.get(), mGlAmbientLight );
				}
			}
			else
			{
				if (lightIndex >= mStartingLightIndexForPass)
				{
					if (mLightCount < maxGLLights)
					{
						switch (lightType)
						{
							case kQ3LightTypeDirectional:
								AddDirectionalLight( theLight.get(), worldToView,
									mLightCount );
								++mLightCount;
								break;
								
							case kQ3LightTypePoint:
								AddPointLight( theLight.get(), worldToView,
									mLightCount );
								++mLightCount;
								break;
								
							case kQ3LightTypeSpot:
								AddSpotLight( theLight.get(), worldToView,
									mLightCount );
								++mLightCount;
								break;
						}
						++handledNonAmbientLights;
					}
					else
					{
						mIsAnotherPassNeeded = true;
					}
				}
				lightIndex += 1;
			}
		}
	}
	
	mStartingLightIndexForPass += handledNonAmbientLights;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glMatrix);
	
	if (! mIsFirstPass)
	{
		glDepthMask( GL_FALSE );	// no writes to depth buffer
		glDepthFunc( GL_LEQUAL );	// pass depth test on equal
		glEnable( GL_BLEND );
		glBlendFunc( GL_ONE, GL_ONE );
	}
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
	
	Reset( 0 );
		
	mIsFirstPass = ! mIsAnotherPassNeeded; // mIsFirstPass becames false only in the second pass DUE to the lights
	
	return mIsAnotherPassNeeded;
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

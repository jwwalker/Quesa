/*  NAME:
        IRLights.c

    DESCRIPTION:
        Quesa interactive renderer lighting.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "IRPrefix.h"
#include "IRLights.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kLightFudge										0.000001f





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      ir_light_calculate_diffuse : Calculate the diffuse colour of a light.
//-----------------------------------------------------------------------------
//		Note :	We take the diffuse colour, adjust it by the brightness, and
//				return four floats suitable for passing to glLightModelfv.
//-----------------------------------------------------------------------------
static void
ir_light_calculate_diffuse(const TQ3LightData *lightData, GLfloat *lightColour)
{


	// Calculate the light colour
	lightColour[0] = (lightData->color.r * lightData->brightness);
	lightColour[1] = (lightData->color.g * lightData->brightness);
	lightColour[2] = (lightData->color.b * lightData->brightness);
	lightColour[3] = 1.0f;
}





//=============================================================================
//      ir_light_convert_ambient : Convert an ambient light.
//-----------------------------------------------------------------------------
//		Note :	Rather than use a new light object, we accumulate the ambient
//				term into the global ambient value.
//-----------------------------------------------------------------------------
static void
ir_light_convert_ambient(TQ3InteractiveData *instanceData, TQ3LightObject theLight)
{	GLfloat				lightColour[4];
	TQ3LightData		lightData;
	TQ3Uns32			n;



	// Get the light data
	Q3Light_GetData(theLight, &lightData);



	// Calculate the light colour
	ir_light_calculate_diffuse(&lightData, lightColour);



	// Add this light into the ambient illumination
	for (n = 0; n < 4; n++)
		instanceData->glAmbientLight[n] += lightColour[n];



	// Update the OpenGL state
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, instanceData->glAmbientLight);
}





//=============================================================================
//      ir_light_convert_directional : Convert a directional light.
//-----------------------------------------------------------------------------
static void
ir_light_convert_directional(TQ3InteractiveData		*instanceData,
								const TQ3Matrix4x4	*worldToView,
								TQ3LightObject		theLight)
{	GLfloat						lightDirection[4];
	GLfloat						lightColour[4];
	TQ3Uns32					lightIndex;
	TQ3DirectionalLightData		lightData;



	// Get the light data
	lightIndex = GL_LIGHT0 + instanceData->lightCount;
	Q3DirectionalLight_GetData(theLight, &lightData);

	Q3Vector3D_Transform(&lightData.direction, worldToView, &lightData.direction);



	// Calculate the light values
	ir_light_calculate_diffuse(&lightData.lightData, lightColour);

	lightDirection[0] = -lightData.direction.x;
	lightDirection[1] = -lightData.direction.y;
	lightDirection[2] = -lightData.direction.z;
	lightDirection[3] = 0.0f;



	// Set up the light
	glLightfv(lightIndex, GL_DIFFUSE,  lightColour);
	glLightfv(lightIndex, GL_SPECULAR, lightColour);
	glLightfv(lightIndex, GL_POSITION, lightDirection);
	glEnable (lightIndex);
}





//=============================================================================
//      ir_light_convert_point : Convert a point light.
//-----------------------------------------------------------------------------
static void
ir_light_convert_point(TQ3InteractiveData	*instanceData,
						const TQ3Matrix4x4	*worldToView,
						TQ3LightObject		theLight)
{	GLfloat					attConstant, attLinear, attQuadratic;
	GLfloat					lightPosition[4];
	GLfloat					lightColour[4];
	TQ3Uns32				lightIndex;
	TQ3PointLightData		lightData;



	// Get the light data
	lightIndex = GL_LIGHT0 + instanceData->lightCount;
	Q3PointLight_GetData(theLight, &lightData);

	Q3Point3D_Transform(&lightData.location, worldToView, &lightData.location);
	


	// Calculate the light values
	ir_light_calculate_diffuse(&lightData.lightData, lightColour);

	lightPosition[0] = lightData.location.x;
	lightPosition[1] = lightData.location.y;
	lightPosition[2] = lightData.location.z;
	lightPosition[3] = 1.0f;

	switch (lightData.attenuation) {
		case kQ3AttenuationTypeNone:
			attConstant  = 1.0f;
			attLinear    = 0.0f;
			attQuadratic = 0.0f;
			break;

		case kQ3AttenuationTypeInverseDistance:
			attConstant  = 0.0f;
			attLinear    = 1.0f;
			attQuadratic = 0.0f;
			break;

		case kQ3AttenuationTypeInverseDistanceSquared:
			attConstant  = 0.0f;
			attLinear    = 0.0f;
			attQuadratic = 1.0f;
			break;
		}



	// Set up the light
	glLightfv(lightIndex, GL_DIFFUSE,               lightColour);
	glLightfv(lightIndex, GL_SPECULAR,              lightColour);
	glLightfv(lightIndex, GL_POSITION,              lightPosition);
	glLightf (lightIndex, GL_CONSTANT_ATTENUATION,  attConstant);
	glLightf (lightIndex, GL_LINEAR_ATTENUATION,    attLinear);
	glLightf (lightIndex, GL_QUADRATIC_ATTENUATION, attQuadratic);
	glEnable (lightIndex);
}





//=============================================================================
//      ir_light_convert_spot : Convert a spot light.
//-----------------------------------------------------------------------------
//		Note :	We do not currently support the hotAngle or fallOff parameters
//				for QD3D spot lights.
//
//				It may be possible to emulate some of the fallOff values by
//				using GL_SPOT_EXPONENT, but there doesn't seem to be any
//				equivalent to hotAngle.
//-----------------------------------------------------------------------------
static void
ir_light_convert_spot(TQ3InteractiveData	*instanceData,
					  const TQ3Matrix4x4	*worldToView,
					  TQ3LightObject		theLight)
{	GLfloat					attConstant, attLinear, attQuadratic;
	GLfloat					lightDirection[3];
	GLfloat					lightPosition[4];
	GLfloat					lightColour[4];
	GLfloat					lightOuter;
	TQ3Uns32				lightIndex;
	TQ3SpotLightData		lightData;



	// Get the light data
	lightIndex = GL_LIGHT0 + instanceData->lightCount;
	Q3SpotLight_GetData(theLight, &lightData);

	Q3Point3D_Transform(&lightData.location, worldToView, &lightData.location);



	// Calculate the light values
	ir_light_calculate_diffuse(&lightData.lightData, lightColour);

	lightPosition[0] = lightData.location.x;
	lightPosition[1] = lightData.location.y;
	lightPosition[2] = lightData.location.z;
	lightPosition[3] = 1.0f;

	lightDirection[0] = lightData.direction.x;
	lightDirection[1] = lightData.direction.y;
	lightDirection[2] = lightData.direction.z;

	switch (lightData.attenuation) {
		case kQ3AttenuationTypeNone:
			attConstant  = 1.0f;
			attLinear    = 0.0f;
			attQuadratic = 0.0f;
			break;

		case kQ3AttenuationTypeInverseDistance:
			attConstant  = 0.0f;
			attLinear    = 1.0f;
			attQuadratic = 0.0f;
			break;

		case kQ3AttenuationTypeInverseDistanceSquared:
			attConstant  = 0.0f;
			attLinear    = 0.0f;
			attQuadratic = 1.0f;
			break;
		}

	lightOuter = Q3Math_RadiansToDegrees(lightData.hotAngle);



	// Set up the light
	glLightfv(lightIndex, GL_DIFFUSE,               lightColour);
	glLightfv(lightIndex, GL_SPECULAR,              lightColour);
	glLightfv(lightIndex, GL_POSITION,              lightPosition);
	glLightfv(lightIndex, GL_SPOT_DIRECTION,        lightDirection);
	glLightf (lightIndex, GL_SPOT_CUTOFF,           lightOuter);
	glLightf (lightIndex, GL_CONSTANT_ATTENUATION,  attConstant);
	glLightf (lightIndex, GL_LINEAR_ATTENUATION,    attLinear);
	glLightf (lightIndex, GL_QUADRATIC_ATTENUATION, attQuadratic);
	glEnable (lightIndex);
}





//=============================================================================
//      ir_light_reset : Reset the light state to the defaults.
//-----------------------------------------------------------------------------
static void
ir_light_reset(TQ3InteractiveData *instanceData, TQ3Uns32 numLights)
{	TQ3Uns32		n;



	// Reset our state
	instanceData->lightCount = 0;
	
	for (n = 0; n < 4; n++)
		instanceData->glAmbientLight[n] = 0.0f;



	// Reset the OpenGL state
	if (numLights != 0)
		{
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
 		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT,     instanceData->glAmbientLight);
		}
	else
		{
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		}
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRRenderer_Lights_StartPass : Start a frame.
//-----------------------------------------------------------------------------
#pragma mark -
void
IRRenderer_Lights_StartPass(TQ3InteractiveData		*instanceData,
							 TQ3CameraObject		theCamera,
							 TQ3GroupObject			theLights)
{	TQ3Uns32				numLightsQD3D, numLightsGL;
	TQ3Matrix4x4			worldToView;
	TQ3Status               qd3dStatus;
	TQ3GroupPosition        lightPos;
    TQ3LightObject          theLight;
	TQ3Boolean				isOn;



	// Find out how many lights we need, and how many we can have
	Q3Group_CountObjects(theLights,        &numLightsQD3D);
	glGetIntegerv(GL_MAX_LIGHTS, (GLint *) &numLightsGL);



	// Reset the lighting state. If we don't have any lights, we're done
	ir_light_reset(instanceData, numLightsQD3D);
	if (numLightsQD3D == 0)
		return;



	// Get the QD3D world->view transform, and reset the OpenGL model->view
	// transform to the identity. Since lights are translated before geometry,
	// we need to transform their direction/positions ourselves in order to
	// put them into the OpenGL camera's coordinate system.
	Q3Camera_GetWorldToView(theCamera, &worldToView);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();



	// Translate the enabled lights
	lightPos   = NULL;
	qd3dStatus = Q3Group_GetFirstPosition(theLights, &lightPos);
    while (qd3dStatus == kQ3Success && lightPos != NULL)
        {
        // Get the light
        qd3dStatus = Q3Group_GetPositionObject(theLights, lightPos, &theLight);
        qd3dStatus = Q3Light_GetState(theLight, &isOn);



		// If it's enabled, and we've not run out of lights, pass it to OpenGL
        if (isOn && instanceData->lightCount < numLightsGL)
            {
            switch(Q3Light_GetType(theLight)) {
                case kQ3LightTypeAmbient:
                    ir_light_convert_ambient(instanceData, theLight);
                    break;
                
                case kQ3LightTypeDirectional:
                    ir_light_convert_directional(instanceData, &worldToView, theLight);
		            instanceData->lightCount++;
                    break;

                case kQ3LightTypePoint:
                    ir_light_convert_point(instanceData, &worldToView, theLight);
		            instanceData->lightCount++;
                    break;

                case kQ3LightTypeSpot:
                    ir_light_convert_spot(instanceData, &worldToView, theLight);
		            instanceData->lightCount++;
                    break;
                }
            }



		// Get the next light
        Q3Object_Dispose(theLight);
        qd3dStatus = Q3Group_GetNextPosition(theLights, &lightPos);
        }
}





//=============================================================================
//      IRRenderer_Lights_EndPass : Finish a frame.
//-----------------------------------------------------------------------------
void
IRRenderer_Lights_EndPass(TQ3InteractiveData *instanceData)
{	TQ3Uns32		n;



	// Turn off the lights and reset their state
	for (n = 0; n < instanceData->lightCount; n++)
		glDisable(GL_LIGHT0 + n);



	// Reset the lighting state
	ir_light_reset(instanceData, 0);
}

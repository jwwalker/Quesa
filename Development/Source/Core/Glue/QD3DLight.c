/*  NAME:
        QD3DLight.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
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
#include "E3Light.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3Light_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
Q3Light_GetType(TQ3LightObject light)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Light_GetType(light));
}





//=============================================================================
//      Q3Light_GetState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Light_GetState(TQ3LightObject light, TQ3Boolean *isOn)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(isOn), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on isOn
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Light_GetState(light, isOn));
}





//=============================================================================
//      Q3Light_GetBrightness : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Light_GetBrightness(TQ3LightObject light, float *brightness)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(brightness), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on brightness
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Light_GetBrightness(light, brightness));
}





//=============================================================================
//      Q3Light_GetColor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Light_GetColor(TQ3LightObject light, TQ3ColorRGB *color)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on color
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Light_GetColor(light, color));
}





//=============================================================================
//      Q3Light_SetState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Light_SetState(TQ3LightObject light, TQ3Boolean isOn)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on isOn
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Light_SetState(light, isOn));
}





//=============================================================================
//      Q3Light_SetBrightness : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Light_SetBrightness(TQ3LightObject light, float brightness)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on brightness
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Light_SetBrightness(light, brightness));
}





//=============================================================================
//      Q3Light_SetColor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Light_SetColor(TQ3LightObject light, const TQ3ColorRGB *color)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on color
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Light_SetColor(light, color));
}





//=============================================================================
//      Q3Light_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Light_GetData(TQ3LightObject light, TQ3LightData *lightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lightData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on lightData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Light_GetData(light, lightData));
}





//=============================================================================
//      Q3Light_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Light_SetData(TQ3LightObject light, const TQ3LightData *lightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lightData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on lightData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Light_SetData(light, lightData));
}



#pragma mark -

//=============================================================================
//      Q3AmbientLight_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3LightObject
Q3AmbientLight_New(const TQ3LightData *lightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lightData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on lightData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3AmbientLight_New(lightData));
}





//=============================================================================
//      Q3AmbientLight_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AmbientLight_GetData(TQ3LightObject light, TQ3LightData *lightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lightData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on lightData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3AmbientLight_GetData(light, lightData));
}





//=============================================================================
//      Q3AmbientLight_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AmbientLight_SetData(TQ3LightObject light, const TQ3LightData *lightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lightData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on lightData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3AmbientLight_SetData(light, lightData));
}



#pragma mark -

//=============================================================================
//      Q3DirectionalLight_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3LightObject
Q3DirectionalLight_New(const TQ3DirectionalLightData *directionalLightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(directionalLightData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on directionalLightData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DirectionalLight_New(directionalLightData));
}





//=============================================================================
//      Q3DirectionalLight_GetCastShadowsState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DirectionalLight_GetCastShadowsState(TQ3LightObject light, TQ3Boolean *castsShadows)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(castsShadows), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on castsShadows
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DirectionalLight_GetCastShadowsState(light, castsShadows));
}





//=============================================================================
//      Q3DirectionalLight_GetDirection : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DirectionalLight_GetDirection(TQ3LightObject light, TQ3Vector3D *direction)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(direction), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on direction
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DirectionalLight_GetDirection(light, direction));
}





//=============================================================================
//      Q3DirectionalLight_SetCastShadowsState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DirectionalLight_SetCastShadowsState(TQ3LightObject light, TQ3Boolean castsShadows)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on castsShadows
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DirectionalLight_SetCastShadowsState(light, castsShadows));
}





//=============================================================================
//      Q3DirectionalLight_SetDirection : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DirectionalLight_SetDirection(TQ3LightObject light, const TQ3Vector3D *direction)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(direction), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on direction
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DirectionalLight_SetDirection(light, direction));
}





//=============================================================================
//      Q3DirectionalLight_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DirectionalLight_GetData(TQ3LightObject light, TQ3DirectionalLightData *directionalLightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(directionalLightData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on directionalLightData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DirectionalLight_GetData(light, directionalLightData));
}





//=============================================================================
//      Q3DirectionalLight_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DirectionalLight_SetData(TQ3LightObject light, const TQ3DirectionalLightData *directionalLightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(directionalLightData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on directionalLightData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DirectionalLight_SetData(light, directionalLightData));
}



#pragma mark -

//=============================================================================
//      Q3PointLight_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3LightObject
Q3PointLight_New(const TQ3PointLightData *pointLightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pointLightData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pointLightData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PointLight_New(pointLightData));
}





//=============================================================================
//      Q3PointLight_GetCastShadowsState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PointLight_GetCastShadowsState(TQ3LightObject light, TQ3Boolean *castsShadows)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(castsShadows), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on castsShadows
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PointLight_GetCastShadowsState(light, castsShadows));
}





//=============================================================================
//      Q3PointLight_GetAttenuation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PointLight_GetAttenuation(TQ3LightObject light, TQ3AttenuationType *attenuation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attenuation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on attenuation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PointLight_GetAttenuation(light, attenuation));
}





//=============================================================================
//      Q3PointLight_GetLocation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PointLight_GetLocation(TQ3LightObject light, TQ3Point3D *location)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(location), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on location
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PointLight_GetLocation(light, location));
}





//=============================================================================
//      Q3PointLight_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PointLight_GetData(TQ3LightObject light, TQ3PointLightData *pointLightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pointLightData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on pointLightData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PointLight_GetData(light, pointLightData));
}





//=============================================================================
//      Q3PointLight_SetCastShadowsState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PointLight_SetCastShadowsState(TQ3LightObject light, TQ3Boolean castsShadows)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on castsShadows
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PointLight_SetCastShadowsState(light, castsShadows));
}





//=============================================================================
//      Q3PointLight_SetAttenuation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PointLight_SetAttenuation(TQ3LightObject light, TQ3AttenuationType attenuation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on attenuation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PointLight_SetAttenuation(light, attenuation));
}





//=============================================================================
//      Q3PointLight_SetLocation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PointLight_SetLocation(TQ3LightObject light, const TQ3Point3D *location)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(location), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on location
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PointLight_SetLocation(light, location));
}





//=============================================================================
//      Q3PointLight_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PointLight_SetData(TQ3LightObject light, const TQ3PointLightData *pointLightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pointLightData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on pointLightData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PointLight_SetData(light, pointLightData));
}



#pragma mark -

//=============================================================================
//      Q3SpotLight_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3LightObject
Q3SpotLight_New(const TQ3SpotLightData *spotLightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(spotLightData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on spotLightData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_New(spotLightData));
}





//=============================================================================
//      Q3SpotLight_GetCastShadowsState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_GetCastShadowsState(TQ3LightObject light, TQ3Boolean *castsShadows)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(castsShadows), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on castsShadows
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_GetCastShadowsState(light, castsShadows));
}





//=============================================================================
//      Q3SpotLight_GetAttenuation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_GetAttenuation(TQ3LightObject light, TQ3AttenuationType *attenuation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attenuation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on attenuation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_GetAttenuation(light, attenuation));
}





//=============================================================================
//      Q3SpotLight_GetLocation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_GetLocation(TQ3LightObject light, TQ3Point3D *location)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(location), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on location
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_GetLocation(light, location));
}





//=============================================================================
//      Q3SpotLight_GetDirection : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_GetDirection(TQ3LightObject light, TQ3Vector3D *direction)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(direction), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on direction
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_GetDirection(light, direction));
}





//=============================================================================
//      Q3SpotLight_GetHotAngle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_GetHotAngle(TQ3LightObject light, float *hotAngle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(hotAngle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on hotAngle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_GetHotAngle(light, hotAngle));
}





//=============================================================================
//      Q3SpotLight_GetOuterAngle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_GetOuterAngle(TQ3LightObject light, float *outerAngle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outerAngle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on outerAngle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_GetOuterAngle(light, outerAngle));
}





//=============================================================================
//      Q3SpotLight_GetFallOff : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_GetFallOff(TQ3LightObject light, TQ3FallOffType *fallOff)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fallOff), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on fallOff
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_GetFallOff(light, fallOff));
}





//=============================================================================
//      Q3SpotLight_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_GetData(TQ3LightObject light, TQ3SpotLightData *spotLightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(spotLightData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on spotLightData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_GetData(light, spotLightData));
}





//=============================================================================
//      Q3SpotLight_SetCastShadowsState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_SetCastShadowsState(TQ3LightObject light, TQ3Boolean castsShadows)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on castsShadows
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_SetCastShadowsState(light, castsShadows));
}





//=============================================================================
//      Q3SpotLight_SetAttenuation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_SetAttenuation(TQ3LightObject light, TQ3AttenuationType attenuation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on attenuation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_SetAttenuation(light, attenuation));
}





//=============================================================================
//      Q3SpotLight_SetLocation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_SetLocation(TQ3LightObject light, const TQ3Point3D *location)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(location), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on location
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_SetLocation(light, location));
}





//=============================================================================
//      Q3SpotLight_SetDirection : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_SetDirection(TQ3LightObject light, const TQ3Vector3D *direction)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(direction), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on direction
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_SetDirection(light, direction));
}





//=============================================================================
//      Q3SpotLight_SetHotAngle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_SetHotAngle(TQ3LightObject light, float hotAngle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on hotAngle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_SetHotAngle(light, hotAngle));
}





//=============================================================================
//      Q3SpotLight_SetOuterAngle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_SetOuterAngle(TQ3LightObject light, float outerAngle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on outerAngle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_SetOuterAngle(light, outerAngle));
}





//=============================================================================
//      Q3SpotLight_SetFallOff : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_SetFallOff(TQ3LightObject light, TQ3FallOffType fallOff)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on fallOff
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_SetFallOff(light, fallOff));
}





//=============================================================================
//      Q3SpotLight_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SpotLight_SetData(TQ3LightObject light, const TQ3SpotLightData *spotLightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Light_IsOfMyClass ( light ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(spotLightData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on light
		return(kQ3Failure);

	if (0) // Further checks on spotLightData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SpotLight_SetData(light, spotLightData));
}






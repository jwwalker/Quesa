/*  NAME:
        E3Light.h

    DESCRIPTION:
        Header file for E3Light.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#ifndef E3LIGHT_HDR
#define E3LIGHT_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3Light_RegisterClass(void);
TQ3Status			E3Light_UnregisterClass(void);

TQ3ObjectType		E3Light_GetType(TQ3LightObject light);
TQ3Status			E3Light_GetState(TQ3LightObject light, TQ3Boolean *isOn);
TQ3Status			E3Light_GetBrightness(TQ3LightObject light, float *brightness);
TQ3Status			E3Light_GetColor(TQ3LightObject light, TQ3ColorRGB *color);
TQ3Status			E3Light_SetState(TQ3LightObject light, TQ3Boolean isOn);
TQ3Status			E3Light_SetBrightness(TQ3LightObject light, float brightness);
TQ3Status			E3Light_SetColor(TQ3LightObject light, const TQ3ColorRGB *color);
TQ3Status			E3Light_GetData(TQ3LightObject light, TQ3LightData *lightData);
TQ3Status			E3Light_SetData(TQ3LightObject light, const TQ3LightData *lightData);

TQ3LightObject		E3AmbientLight_New(const TQ3LightData *lightData);
TQ3Status			E3AmbientLight_GetData(TQ3LightObject light, TQ3LightData *lightData);
TQ3Status			E3AmbientLight_SetData(TQ3LightObject light, const TQ3LightData *lightData);

TQ3LightObject		E3DirectionalLight_New(const TQ3DirectionalLightData *directionalLightData);
TQ3Status			E3DirectionalLight_GetCastShadowsState(TQ3LightObject light, TQ3Boolean *castsShadows);
TQ3Status			E3DirectionalLight_GetDirection(TQ3LightObject light, TQ3Vector3D *direction);
TQ3Status			E3DirectionalLight_SetCastShadowsState(TQ3LightObject light, TQ3Boolean castsShadows);
TQ3Status			E3DirectionalLight_SetDirection(TQ3LightObject light, const TQ3Vector3D *direction);
TQ3Status			E3DirectionalLight_GetData(TQ3LightObject light, TQ3DirectionalLightData *directionalLightData);
TQ3Status			E3DirectionalLight_SetData(TQ3LightObject light, const TQ3DirectionalLightData *directionalLightData);

TQ3LightObject		E3PointLight_New(const TQ3PointLightData *pointLightData);
TQ3Status			E3PointLight_GetCastShadowsState(TQ3LightObject light, TQ3Boolean *castsShadows);
TQ3Status			E3PointLight_GetAttenuation(TQ3LightObject light, TQ3AttenuationType *attenuation);
TQ3Status			E3PointLight_GetLocation(TQ3LightObject light, TQ3Point3D *location);
TQ3Status			E3PointLight_GetData(TQ3LightObject light, TQ3PointLightData *pointLightData);
TQ3Status			E3PointLight_SetCastShadowsState(TQ3LightObject light, TQ3Boolean castsShadows);
TQ3Status			E3PointLight_SetAttenuation(TQ3LightObject light, TQ3AttenuationType attenuation);
TQ3Status			E3PointLight_SetLocation(TQ3LightObject light, const TQ3Point3D *location);
TQ3Status			E3PointLight_SetData(TQ3LightObject light, const TQ3PointLightData *pointLightData);

TQ3LightObject		E3SpotLight_New(const TQ3SpotLightData *spotLightData);
TQ3Status			E3SpotLight_GetCastShadowsState(TQ3LightObject light, TQ3Boolean *castsShadows);
TQ3Status			E3SpotLight_GetAttenuation(TQ3LightObject light, TQ3AttenuationType *attenuation);
TQ3Status			E3SpotLight_GetLocation(TQ3LightObject light, TQ3Point3D *location);
TQ3Status			E3SpotLight_GetDirection(TQ3LightObject light, TQ3Vector3D *direction);
TQ3Status			E3SpotLight_GetHotAngle(TQ3LightObject light, float *hotAngle);
TQ3Status			E3SpotLight_GetOuterAngle(TQ3LightObject light, float *outerAngle);
TQ3Status			E3SpotLight_GetFallOff(TQ3LightObject light, TQ3FallOffType *fallOff);
TQ3Status			E3SpotLight_GetData(TQ3LightObject light, TQ3SpotLightData *spotLightData);
TQ3Status			E3SpotLight_SetCastShadowsState(TQ3LightObject light, TQ3Boolean castsShadows);
TQ3Status			E3SpotLight_SetAttenuation(TQ3LightObject light, TQ3AttenuationType attenuation);
TQ3Status			E3SpotLight_SetLocation(TQ3LightObject light, const TQ3Point3D *location);
TQ3Status			E3SpotLight_SetDirection(TQ3LightObject light, const TQ3Vector3D *direction);
TQ3Status			E3SpotLight_SetHotAngle(TQ3LightObject light, float hotAngle);
TQ3Status			E3SpotLight_SetOuterAngle(TQ3LightObject light, float outerAngle);
TQ3Status			E3SpotLight_SetFallOff(TQ3LightObject light, TQ3FallOffType fallOff);
TQ3Status			E3SpotLight_SetData(TQ3LightObject light, const TQ3SpotLightData *spotLightData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


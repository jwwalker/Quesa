/*  NAME:
        E3Light.h

    DESCRIPTION:
        Header file for E3Light.c.

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

TQ3Boolean			E3Light_IsOfMyClass ( TQ3Object object ) ;

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


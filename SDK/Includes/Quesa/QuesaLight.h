/*! @header QuesaLight.h
        Declares the Quesa light objects.
 */
/*  NAME:
        QuesaLight.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_LIGHT_HDR
#define QUESA_LIGHT_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DLIGHT__)
#error
#endif

#define __QD3DLIGHT__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Attenuation type
typedef enum {
    kQ3AttenuationTypeNone                      = 0,
    kQ3AttenuationTypeInverseDistance           = 1,
    kQ3AttenuationTypeInverseDistanceSquared    = 2
} TQ3AttenuationType;


// Fall-off type
typedef enum {
    kQ3FallOffTypeNone                          = 0,
    kQ3FallOffTypeLinear                        = 1,
    kQ3FallOffTypeExponential                   = 2,
    kQ3FallOffTypeCosine                        = 3
} TQ3FallOffType;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Light data
typedef struct {
    TQ3Boolean                                  isOn;
    float                                       brightness;
    TQ3ColorRGB                                 color;
} TQ3LightData;


// Directional light data
typedef struct {
    TQ3LightData                                lightData;
    TQ3Boolean                                  castsShadows;
    TQ3Vector3D                                 direction;
} TQ3DirectionalLightData;


// Point light data
typedef struct {
    TQ3LightData                                lightData;
    TQ3Boolean                                  castsShadows;
    TQ3AttenuationType                          attenuation;
    TQ3Point3D                                  location;
} TQ3PointLightData;


// Spot light data
typedef struct {
    TQ3LightData                                lightData;
    TQ3Boolean                                  castsShadows;
    TQ3AttenuationType                          attenuation;
    TQ3Point3D                                  location;
    TQ3Vector3D                                 direction;
    float                                       hotAngle;
    float                                       outerAngle;
    TQ3FallOffType                              fallOff;
} TQ3SpotLightData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Light_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Light_GetType (
    TQ3LightObject                light
);



/*!
 *  @function
 *      Q3Light_GetState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param isOn             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_GetState (
    TQ3LightObject                light,
    TQ3Boolean                    *isOn
);



/*!
 *  @function
 *      Q3Light_GetBrightness
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param brightness       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_GetBrightness (
    TQ3LightObject                light,
    float                         *brightness
);



/*!
 *  @function
 *      Q3Light_GetColor
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param color            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_GetColor (
    TQ3LightObject                light,
    TQ3ColorRGB                   *color
);



/*!
 *  @function
 *      Q3Light_SetState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param isOn             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_SetState (
    TQ3LightObject                light,
    TQ3Boolean                    isOn
);



/*!
 *  @function
 *      Q3Light_SetBrightness
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param brightness       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_SetBrightness (
    TQ3LightObject                light,
    float                         brightness
);



/*!
 *  @function
 *      Q3Light_SetColor
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param color            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_SetColor (
    TQ3LightObject                light,
    const TQ3ColorRGB             *color
);



/*!
 *  @function
 *      Q3Light_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param lightData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_GetData (
    TQ3LightObject                light,
    TQ3LightData                  *lightData
);



/*!
 *  @function
 *      Q3Light_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param lightData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_SetData (
    TQ3LightObject                light,
    const TQ3LightData            *lightData
);



/*!
 *  @function
 *      Q3AmbientLight_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param lightData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3LightObject  )
Q3AmbientLight_New (
    const TQ3LightData            *lightData
);



/*!
 *  @function
 *      Q3AmbientLight_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param lightData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AmbientLight_GetData (
    TQ3LightObject                light,
    TQ3LightData                  *lightData
);



/*!
 *  @function
 *      Q3AmbientLight_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param lightData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AmbientLight_SetData (
    TQ3LightObject                light,
    const TQ3LightData            *lightData
);



/*!
 *  @function
 *      Q3DirectionalLight_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param directionalLightData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3LightObject  )
Q3DirectionalLight_New (
    const TQ3DirectionalLightData *directionalLightData
);



/*!
 *  @function
 *      Q3DirectionalLight_GetCastShadowsState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param castsShadows     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_GetCastShadowsState (
    TQ3LightObject                light,
    TQ3Boolean                    *castsShadows
);



/*!
 *  @function
 *      Q3DirectionalLight_GetDirection
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param direction        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_GetDirection (
    TQ3LightObject                light,
    TQ3Vector3D                   *direction
);



/*!
 *  @function
 *      Q3DirectionalLight_SetCastShadowsState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param castsShadows     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_SetCastShadowsState (
    TQ3LightObject                light,
    TQ3Boolean                    castsShadows
);



/*!
 *  @function
 *      Q3DirectionalLight_SetDirection
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param direction        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_SetDirection (
    TQ3LightObject                light,
    const TQ3Vector3D             *direction
);



/*!
 *  @function
 *      Q3DirectionalLight_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param directionalLightData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_GetData (
    TQ3LightObject                light,
    TQ3DirectionalLightData       *directionalLightData
);



/*!
 *  @function
 *      Q3DirectionalLight_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param directionalLightData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_SetData (
    TQ3LightObject                light,
    const TQ3DirectionalLightData *directionalLightData
);



/*!
 *  @function
 *      Q3PointLight_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pointLightData   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3LightObject  )
Q3PointLight_New (
    const TQ3PointLightData       *pointLightData
);



/*!
 *  @function
 *      Q3PointLight_GetCastShadowsState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param castsShadows     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_GetCastShadowsState (
    TQ3LightObject                light,
    TQ3Boolean                    *castsShadows
);



/*!
 *  @function
 *      Q3PointLight_GetAttenuation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param attenuation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_GetAttenuation (
    TQ3LightObject                light,
    TQ3AttenuationType            *attenuation
);



/*!
 *  @function
 *      Q3PointLight_GetLocation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param location         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_GetLocation (
    TQ3LightObject                light,
    TQ3Point3D                    *location
);



/*!
 *  @function
 *      Q3PointLight_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param pointLightData   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_GetData (
    TQ3LightObject                light,
    TQ3PointLightData             *pointLightData
);



/*!
 *  @function
 *      Q3PointLight_SetCastShadowsState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param castsShadows     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_SetCastShadowsState (
    TQ3LightObject                light,
    TQ3Boolean                    castsShadows
);



/*!
 *  @function
 *      Q3PointLight_SetAttenuation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param attenuation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_SetAttenuation (
    TQ3LightObject                light,
    TQ3AttenuationType            attenuation
);



/*!
 *  @function
 *      Q3PointLight_SetLocation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param location         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_SetLocation (
    TQ3LightObject                light,
    const TQ3Point3D              *location
);



/*!
 *  @function
 *      Q3PointLight_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param pointLightData   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_SetData (
    TQ3LightObject                light,
    const TQ3PointLightData       *pointLightData
);



/*!
 *  @function
 *      Q3SpotLight_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param spotLightData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3LightObject  )
Q3SpotLight_New (
    const TQ3SpotLightData        *spotLightData
);



/*!
 *  @function
 *      Q3SpotLight_GetCastShadowsState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param castsShadows     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetCastShadowsState (
    TQ3LightObject                light,
    TQ3Boolean                    *castsShadows
);



/*!
 *  @function
 *      Q3SpotLight_GetAttenuation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param attenuation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetAttenuation (
    TQ3LightObject                light,
    TQ3AttenuationType            *attenuation
);



/*!
 *  @function
 *      Q3SpotLight_GetLocation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param location         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetLocation (
    TQ3LightObject                light,
    TQ3Point3D                    *location
);



/*!
 *  @function
 *      Q3SpotLight_GetDirection
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param direction        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetDirection (
    TQ3LightObject                light,
    TQ3Vector3D                   *direction
);



/*!
 *  @function
 *      Q3SpotLight_GetHotAngle
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param hotAngle         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetHotAngle (
    TQ3LightObject                light,
    float                         *hotAngle
);



/*!
 *  @function
 *      Q3SpotLight_GetOuterAngle
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param outerAngle       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetOuterAngle (
    TQ3LightObject                light,
    float                         *outerAngle
);



/*!
 *  @function
 *      Q3SpotLight_GetFallOff
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param fallOff          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetFallOff (
    TQ3LightObject                light,
    TQ3FallOffType                *fallOff
);



/*!
 *  @function
 *      Q3SpotLight_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param spotLightData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetData (
    TQ3LightObject                light,
    TQ3SpotLightData              *spotLightData
);



/*!
 *  @function
 *      Q3SpotLight_SetCastShadowsState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param castsShadows     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetCastShadowsState (
    TQ3LightObject                light,
    TQ3Boolean                    castsShadows
);



/*!
 *  @function
 *      Q3SpotLight_SetAttenuation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param attenuation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetAttenuation (
    TQ3LightObject                light,
    TQ3AttenuationType            attenuation
);



/*!
 *  @function
 *      Q3SpotLight_SetLocation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param location         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetLocation (
    TQ3LightObject                light,
    const TQ3Point3D              *location
);



/*!
 *  @function
 *      Q3SpotLight_SetDirection
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param direction        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetDirection (
    TQ3LightObject                light,
    const TQ3Vector3D             *direction
);



/*!
 *  @function
 *      Q3SpotLight_SetHotAngle
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param hotAngle         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetHotAngle (
    TQ3LightObject                light,
    float                         hotAngle
);



/*!
 *  @function
 *      Q3SpotLight_SetOuterAngle
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param outerAngle       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetOuterAngle (
    TQ3LightObject                light,
    float                         outerAngle
);



/*!
 *  @function
 *      Q3SpotLight_SetFallOff
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param fallOff          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetFallOff (
    TQ3LightObject                light,
    TQ3FallOffType                fallOff
);



/*!
 *  @function
 *      Q3SpotLight_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param light            Description of the parameter.
 *  @param spotLightData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetData (
    TQ3LightObject                light,
    const TQ3SpotLightData        *spotLightData
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



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
#pragma enumsalwaysint on

#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
 *  @enum
 *      TQ3AttenuationType
 *  @discussion
 *      Light attenuation methods.
 *
 *      The attenuation of a light controls how the intensity of the light
 *      is diminished over distance. In the real world, the itensity of a
 *      light source is attenuated as 1/(d*d).
 *
 *  @constant kQ3AttenuationTypeNone                      Intensity is not attenuated.
 *  @constant kQ3AttenuationTypeInverseDistance           Intensity is attenuated as 1/d.
 *  @constant kQ3AttenuationTypeInverseDistanceSquared    Intensity is attenuated as 1/(d*d).
 */
typedef enum {
    kQ3AttenuationTypeNone                      = 0,
    kQ3AttenuationTypeInverseDistance           = 1,
    kQ3AttenuationTypeInverseDistanceSquared    = 2
} TQ3AttenuationType;


/*!
 *  @enum
 *      TQ3FallOffType
 *  @discussion
 *      Light fall-off methods.
 *
 *      The fall-off value of a light controls how the intensity of the light
 *      varies from the edge of the hot angle (where the light is at full intensity)
 *      to the outer angle (where the light intensity falls to zero).
 *
 *  @constant kQ3FallOffTypeNone           Intensity does not fall off.
 *  @constant kQ3FallOffTypeLinear         Intensity falls off linearly.
 *  @constant kQ3FallOffTypeExponential    Intensity falls off exponentially.
 *  @constant kQ3FallOffTypeCosine         Intensity falls off as the cosine of the angle.
 */
typedef enum {
    kQ3FallOffTypeNone                          = 0,
    kQ3FallOffTypeLinear                        = 1,
    kQ3FallOffTypeExponential                   = 2,
    kQ3FallOffTypeCosine                        = 3
} TQ3FallOffType;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
/*!
 *  @struct
 *      TQ3LightData
 *  @discussion
 *      Describes the common state for a light.
 *
 *      The common light state includes the on/off state, brightness, and color.
 *      Brightness and color values should be between 0.0 and 1.0 inclusive.
 *
 *  @field isOn             Controls if the light is active or not.
 *  @field brightness       The brightness of the light.
 *  @field color            The colour of the light.
 */
typedef struct TQ3LightData {
    TQ3Boolean                                  isOn;
    float                                       brightness;
    TQ3ColorRGB                                 color;
} TQ3LightData;


/*!
 *  @struct
 *      TQ3DirectionalLightData
 *  @discussion
 *      Describes the state for a directional light.
 *
 *      A directional light is defined by a vector, which indicates the
 *      world-space direction away from the light source.
 *
 *  @field lightData        The common state for the light.
 *  @field castsShadows     Indicates if the light casts shadows.
 *  @field direction        The direction of the light.
 */
typedef struct TQ3DirectionalLightData {
    TQ3LightData                                lightData;
    TQ3Boolean                                  castsShadows;
    TQ3Vector3D                                 direction;
} TQ3DirectionalLightData;


/*!
 *  @struct
 *      TQ3PointLightData
 *  @discussion
 *      Describes the state for a point light.
 *
 *      A point light is defined by a coordinate in world space, and an
 *      attenuation value which controls how the light diminishes with
 *      distance.
 *
 *  @field lightData        The common state for the light.
 *  @field castsShadows     Indicates if the light casts shadows.
 *  @field attenuation      The attenuation style of the light.
 *  @field location         The location of the light.
 */
typedef struct TQ3PointLightData {
    TQ3LightData                                lightData;
    TQ3Boolean                                  castsShadows;
    TQ3AttenuationType                          attenuation;
    TQ3Point3D                                  location;
} TQ3PointLightData;


/*!
 *  @struct
 *      TQ3SpotLightData
 *  @discussion
 *      Describes the state for a spot light.
 *
 *      A spot light is defined by a coordinate in world space, a vector
 *      away from that coordinate, and an attenuation value which controls
 *      how the light diminishes with distance.
 *
 *      A spot light casts a cone of light, where the cone is defined by
 *      two angles. The hot angle is the angle from the center of the light
 *      cone to the point where the light intensity starts to drop, and the
 *      outerAngle is the angle from the center of the light to the point
 *      where the light intensity has fallen to zero.
 *
 *      The way the light intensity diminishes between the hotAngle and
 *      outerAngle values is controlled by the light fallOff value.
 *
 *      Both hotAngle and outerAngle are half-angles from the center of the
 *      light cone, and are specified in radians. They both range from 0.0
 *      to kQ3Pi/2.0 (inclusive), and the outerAngle must be equal than or
 *      greater to the hotAngle.
 *
 *  @field lightData        The common state for the light.
 *  @field castsShadows     Indicates if the light casts shadows.
 *  @field attenuation      The attenuation style of the light.
 *  @field location         The location of the light.
 *  @field direction        The direction of the light.
 *  @field hotAngle         The half-angle where the light intensity starts to drop.
 *  @field outerAngle       The half-angle where the light intensity reaches zero.
 *  @field fallOff          The fall off between the hotAngle and the outerAngle.
 */
typedef struct TQ3SpotLightData {
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
 *      Get the type of a light.
 *
 *      Returns kQ3ObjectTypeInvalid if the light type is unknown.
 *
 *  @param light            The light to query.
 *  @result                 The type of the light object.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Light_GetType (
    TQ3LightObject                light
);



/*!
 *  @function
 *      Q3Light_GetState
 *  @discussion
 *      Get the on/off state of a light.
 *
 *  @param light            The light to query.
 *  @param isOn             Receives true/false as the light is on.
 *  @result                 Success or failure of the operation.
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
 *      Get the brightness of a light.
 *
 *  @param light            The light to query.
 *  @param brightness       Receives the brightness of the light.
 *  @result                 Success or failure of the operation.
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
 *      Get the color of a light.
 *
 *  @param light            The light to query.
 *  @param color            Receives the color of the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the on/off state for a light.
 *
 *  @param light            The light to update.
 *  @param isOn             True or false as the light is on.
 *  @result                 Success or failure of the operation.
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
 *      Set the brightness of a light.
 *
 *  @param light            The light to update.
 *  @param brightness       The new brightness for the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the color of a light.
 *
 *  @param light            The light to update.
 *  @param color            The new color for the light.
 *  @result                 Success or failure of the operation.
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
 *      Get the common state of a light.
 *
 *  @param light            The light to query.
 *  @param lightData        Receives the common state of the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the common state for a light.
 *
 *  @param light            The light to update.
 *  @param lightData        The new common state for the light.
 *  @result                 Success or failure of the operation.
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
 *      Create a new ambient light object.
 *
 *  @param lightData        The data for the light object.
 *  @result                 The new light object.
 */
EXTERN_API_C ( TQ3LightObject  )
Q3AmbientLight_New (
    const TQ3LightData            *lightData
);



/*!
 *  @function
 *      Q3AmbientLight_GetData
 *  @discussion
 *      Get the data for an ambient light.
 *
 *  @param light            The light to query.
 *  @param lightData        Receives the data of the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the data for an ambient light.
 *
 *  @param light            The light to update.
 *  @param lightData        The new data for the light.
 *  @result                 Success or failure of the operation.
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
 *      Create a new directional light object.
 *
 *  @param directionalLightData    The data for the light object.
 *  @result                        The new light object.
 */
EXTERN_API_C ( TQ3LightObject  )
Q3DirectionalLight_New (
    const TQ3DirectionalLightData *directionalLightData
);



/*!
 *  @function
 *      Q3DirectionalLight_GetCastShadowsState
 *  @discussion
 *      Get the cast-shadow state of a directional light.
 *
 *  @param light            The light to query.
 *  @param castsShadows     Receives true or false as the light casts shadows.
 *  @result                 Success or failure of the operation.
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
 *      Get the direction of a directional light.
 *
 *  @param light            The light to query.
 *  @param direction        Receives the direction of the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the cast-shadow state of a directional light.
 *
 *  @param light            The light to update.
 *  @param castsShadows     True or false as the light casts shadows.
 *  @result                 Success or failure of the operation.
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
 *      Set the direction of a directional light.
 *
 *  @param light            The light to update.
 *  @param direction        The new direction for the light.
 *  @result                 Success or failure of the operation.
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
 *      Get the data for a directional light.
 *
 *  @param light                   The light to query.
 *  @param directionalLightData    Receives the data of the light.
 *  @result                        Success or failure of the operation.
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
 *      Set the data for a directional light.
 *
 *  @param light                   The light to update.
 *  @param directionalLightData    The new data for the light.
 *  @result                        Success or failure of the operation.
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
 *      Create a new point light object.
 *
 *  @param pointLightData   The data for the light object.
 *  @result                 The new light object.
 */
EXTERN_API_C ( TQ3LightObject  )
Q3PointLight_New (
    const TQ3PointLightData       *pointLightData
);



/*!
 *  @function
 *      Q3PointLight_GetCastShadowsState
 *  @discussion
 *      Get the cast-shadow state of a point light.
 *
 *  @param light            The light to query.
 *  @param castsShadows     Receives true or false as the light casts shadows.
 *  @result                 Success or failure of the operation.
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
 *      Get the attenuation of a point light.
 *
 *  @param light            The light to query.
 *  @param attenuation      Receives the attenuation of the light.
 *  @result                 Success or failure of the operation.
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
 *      Get the location of a point light.
 *
 *  @param light            The light to query.
 *  @param location         Receives the location of the light.
 *  @result                 Success or failure of the operation.
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
 *      Get the data for a point light.
 *
 *  @param light            The light to query.
 *  @param pointLightData   Receives the data of the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the cast-shadow state of a point light.
 *
 *  @param light            The light to update.
 *  @param castsShadows     True or false as the light casts shadows.
 *  @result                 Success or failure of the operation.
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
 *      Set the attenuation of a point light.
 *
 *  @param light            The light to update.
 *  @param attenuation      The new attenuation for the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the location of a point light.
 *
 *  @param light            The light to update.
 *  @param location         The new location for the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the data for a point light.
 *
 *  @param light            The light to update.
 *  @param pointLightData   The new data for the light.
 *  @result                 Success or failure of the operation.
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
 *      Create a new spot light object.
 *
 *  @param spotLightData    The data for the light object.
 *  @result                 The new light object.
 */
EXTERN_API_C ( TQ3LightObject  )
Q3SpotLight_New (
    const TQ3SpotLightData        *spotLightData
);



/*!
 *  @function
 *      Q3SpotLight_GetCastShadowsState
 *  @discussion
 *      Get the cast-shadow state of a spot light.
 *
 *  @param light            The light to query.
 *  @param castsShadows     Receives true or false as the light casts shadows.
 *  @result                 Success or failure of the operation.
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
 *      Get the attenuation of a spot light.
 *
 *  @param light            The light to query.
 *  @param attenuation      Receives the attenuation of the light.
 *  @result                 Success or failure of the operation.
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
 *      Get the location of a spot light.
 *
 *  @param light            The light to query.
 *  @param location         Receives the location of the light.
 *  @result                 Success or failure of the operation.
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
 *      Get the direction of a spot light.
 *
 *  @param light            The light to query.
 *  @param direction        Receives the direction of the light.
 *  @result                 Success or failure of the operation.
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
 *      Get the hot angle of a spot light.
 *
 *  @param light            The light to query.
 *  @param hotAngle         Receives the hot angle of the light.
 *  @result                 Success or failure of the operation.
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
 *      Get the outer angle of a spot light.
 *
 *  @param light            The light to query.
 *  @param outerAngle       Receives the outer angle of the light.
 *  @result                 Success or failure of the operation.
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
 *      Get the fall off of a spot light.
 *
 *  @param light            The light to query.
 *  @param fallOff          Receives the fall off value of the light.
 *  @result                 Success or failure of the operation.
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
 *      Get the data for a spot light.
 *
 *  @param light            The light to query.
 *  @param spotLightData    Receives the data of the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the cast-shadow state of a spot light.
 *
 *  @param light            The light to update.
 *  @param castsShadows     True or false as the light casts shadows.
 *  @result                 Success or failure of the operation.
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
 *      Set the attenuation for a spot light.
 *
 *  @param light            The light to update.
 *  @param attenuation      The new attenuation for the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the location for a spot light.
 *
 *  @param light            The light to update.
 *  @param location         The new location for the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the direction of a spot light.
 *
 *  @param light            The light to update.
 *  @param direction        The new direction for the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the hot angle of a spot light.
 *
 *  @param light            The light to update.
 *  @param hotAngle         The new hot angle for the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the outer angle of a spot light.
 *
 *  @param light            The light to update.
 *  @param outerAngle       The new outer angle for the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the fall off of a spot light.
 *
 *  @param light            The light to update.
 *  @param fallOff          The new fall off value for the light.
 *  @result                 Success or failure of the operation.
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
 *      Set the data for a spot light.
 *
 *  @param light            The light to update.
 *  @param spotLightData    The new data for the light.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetData (
    TQ3LightObject                light,
    const TQ3SpotLightData        *spotLightData
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif



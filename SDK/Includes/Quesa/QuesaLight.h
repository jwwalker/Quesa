/*  NAME:
        QuesaLight.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#ifndef __QUESA_LIGHT__
#define __QUESA_LIGHT__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Constants go here





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Types go here





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Macros go here





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
#if defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON

/*
 *	Q3Light_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Light_GetType (
	TQ3LightObject                light
);



/*
 *	Q3Light_GetState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_GetState (
	TQ3LightObject                light,
	TQ3Boolean                    *isOn
);



/*
 *	Q3Light_GetBrightness
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_GetBrightness (
	TQ3LightObject                light,
	float                         *brightness
);



/*
 *	Q3Light_GetColor
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_GetColor (
	TQ3LightObject                light,
	TQ3ColorRGB                   *color
);



/*
 *	Q3Light_SetState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_SetState (
	TQ3LightObject                light,
	TQ3Boolean                    isOn
);



/*
 *	Q3Light_SetBrightness
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_SetBrightness (
	TQ3LightObject                light,
	float                         brightness
);



/*
 *	Q3Light_SetColor
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_SetColor (
	TQ3LightObject                light,
	const TQ3ColorRGB             *color
);



/*
 *	Q3Light_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_GetData (
	TQ3LightObject                light,
	TQ3LightData                  *lightData
);



/*
 *	Q3Light_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Light_SetData (
	TQ3LightObject                light,
	const TQ3LightData            *lightData
);



/*
 *	Q3AmbientLight_New
 *		Description of function
 */
EXTERN_API_C ( TQ3LightObject  )
Q3AmbientLight_New (
	const TQ3LightData            *lightData
);



/*
 *	Q3AmbientLight_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AmbientLight_GetData (
	TQ3LightObject                light,
	TQ3LightData                  *lightData
);



/*
 *	Q3AmbientLight_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AmbientLight_SetData (
	TQ3LightObject                light,
	const TQ3LightData            *lightData
);



/*
 *	Q3DirectionalLight_New
 *		Description of function
 */
EXTERN_API_C ( TQ3LightObject  )
Q3DirectionalLight_New (
	const TQ3DirectionalLightData *directionalLightData
);



/*
 *	Q3DirectionalLight_GetCastShadowsState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_GetCastShadowsState (
	TQ3LightObject                light,
	TQ3Boolean                    *castsShadows
);



/*
 *	Q3DirectionalLight_GetDirection
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_GetDirection (
	TQ3LightObject                light,
	TQ3Vector3D                   *direction
);



/*
 *	Q3DirectionalLight_SetCastShadowsState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_SetCastShadowsState (
	TQ3LightObject                light,
	TQ3Boolean                    castsShadows
);



/*
 *	Q3DirectionalLight_SetDirection
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_SetDirection (
	TQ3LightObject                light,
	const TQ3Vector3D             *direction
);



/*
 *	Q3DirectionalLight_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_GetData (
	TQ3LightObject                light,
	TQ3DirectionalLightData       *directionalLightData
);



/*
 *	Q3DirectionalLight_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DirectionalLight_SetData (
	TQ3LightObject                light,
	const TQ3DirectionalLightData *directionalLightData
);



/*
 *	Q3PointLight_New
 *		Description of function
 */
EXTERN_API_C ( TQ3LightObject  )
Q3PointLight_New (
	const TQ3PointLightData       *pointLightData
);



/*
 *	Q3PointLight_GetCastShadowsState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_GetCastShadowsState (
	TQ3LightObject                light,
	TQ3Boolean                    *castsShadows
);



/*
 *	Q3PointLight_GetAttenuation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_GetAttenuation (
	TQ3LightObject                light,
	TQ3AttenuationType            *attenuation
);



/*
 *	Q3PointLight_GetLocation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_GetLocation (
	TQ3LightObject                light,
	TQ3Point3D                    *location
);



/*
 *	Q3PointLight_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_GetData (
	TQ3LightObject                light,
	TQ3PointLightData             *pointLightData
);



/*
 *	Q3PointLight_SetCastShadowsState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_SetCastShadowsState (
	TQ3LightObject                light,
	TQ3Boolean                    castsShadows
);



/*
 *	Q3PointLight_SetAttenuation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_SetAttenuation (
	TQ3LightObject                light,
	TQ3AttenuationType            attenuation
);



/*
 *	Q3PointLight_SetLocation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_SetLocation (
	TQ3LightObject                light,
	const TQ3Point3D              *location
);



/*
 *	Q3PointLight_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PointLight_SetData (
	TQ3LightObject                light,
	const TQ3PointLightData       *pointLightData
);



/*
 *	Q3SpotLight_New
 *		Description of function
 */
EXTERN_API_C ( TQ3LightObject  )
Q3SpotLight_New (
	const TQ3SpotLightData        *spotLightData
);



/*
 *	Q3SpotLight_GetCastShadowsState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetCastShadowsState (
	TQ3LightObject                light,
	TQ3Boolean                    *castsShadows
);



/*
 *	Q3SpotLight_GetAttenuation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetAttenuation (
	TQ3LightObject                light,
	TQ3AttenuationType            *attenuation
);



/*
 *	Q3SpotLight_GetLocation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetLocation (
	TQ3LightObject                light,
	TQ3Point3D                    *location
);



/*
 *	Q3SpotLight_GetDirection
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetDirection (
	TQ3LightObject                light,
	TQ3Vector3D                   *direction
);



/*
 *	Q3SpotLight_GetHotAngle
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetHotAngle (
	TQ3LightObject                light,
	float                         *hotAngle
);



/*
 *	Q3SpotLight_GetOuterAngle
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetOuterAngle (
	TQ3LightObject                light,
	float                         *outerAngle
);



/*
 *	Q3SpotLight_GetFallOff
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetFallOff (
	TQ3LightObject                light,
	TQ3FallOffType                *fallOff
);



/*
 *	Q3SpotLight_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_GetData (
	TQ3LightObject                light,
	TQ3SpotLightData              *spotLightData
);



/*
 *	Q3SpotLight_SetCastShadowsState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetCastShadowsState (
	TQ3LightObject                light,
	TQ3Boolean                    castsShadows
);



/*
 *	Q3SpotLight_SetAttenuation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetAttenuation (
	TQ3LightObject                light,
	TQ3AttenuationType            attenuation
);



/*
 *	Q3SpotLight_SetLocation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetLocation (
	TQ3LightObject                light,
	const TQ3Point3D              *location
);



/*
 *	Q3SpotLight_SetDirection
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetDirection (
	TQ3LightObject                light,
	const TQ3Vector3D             *direction
);



/*
 *	Q3SpotLight_SetHotAngle
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetHotAngle (
	TQ3LightObject                light,
	float                         hotAngle
);



/*
 *	Q3SpotLight_SetOuterAngle
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetOuterAngle (
	TQ3LightObject                light,
	float                         outerAngle
);



/*
 *	Q3SpotLight_SetFallOff
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetFallOff (
	TQ3LightObject                light,
	TQ3FallOffType                fallOff
);



/*
 *	Q3SpotLight_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SpotLight_SetData (
	TQ3LightObject                light,
	const TQ3SpotLightData        *spotLightData
);

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


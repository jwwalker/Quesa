/*  NAME:
        RS_Light.c

    DESCRIPTION:
        A procedural api for RayShade.

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
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------

#include "RT_Light.h"

#include "liblight/light.h"
#include "liblight/infinite.h"
#include "liblight/point.h"
#include "liblight/spot.h"

#include "libshade/options.h"

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
	#include <QuesaMath.h>
#else
	#include <QD3D.h>
	#include <QD3DMath.h>
#endif
/*===========================================================================*\
 *
 *	Routine:	rt_Color_Set_Q3ColorRGBIntensity()
 *
 *	Comments:	Sets the rayshade color to an RGB color multiplied by
 *				intensity
 *
\*===========================================================================*/
static void
rt_Color_Set_Q3ColorRGBIntensity(
        Color                   *dst,
        const TQ3ColorRGB       *srccolor,
        float                   srcintensity)
{
    dst->r = srccolor->r*srcintensity;
    dst->g = srccolor->g*srcintensity;
    dst->b = srccolor->b*srcintensity;
}

/*===========================================================================*\
 *
 *	Routine:	RT_AddAmbientLight()
 *
 *	Comments:	Adds an ambient light to the scene
 *
\*===========================================================================*/
TQ3Status
RT_AddAmbientLight(
		TRTDrawContext			 * /*inContext*/,
        const TQ3ColorRGB        *inColor,
        float                    inBrightness)
{
	Options.ambient.r += inColor->r*inBrightness;
	Options.ambient.g += inColor->g*inBrightness;
	Options.ambient.b += inColor->b*inBrightness;
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	RT_AddDirectionalLight()
 *
 *	Comments:	Adds a directional light to the scene
 *
\*===========================================================================*/
TQ3Status
RT_AddDirectionalLight(
			TRTDrawContext			 * /*inContext*/,
            TQ3Vector3D              *inDirection,
            TQ3Boolean               inCastShadows,
            const TQ3ColorRGB        *inColor,
            float                    inBrightness)
{
    Color   theColor;
    Vector  theVector;
    Light   *theLight;
    
    rt_Color_Set_Q3ColorRGBIntensity(&theColor,inColor,inBrightness);
    
    theVector.x = -inDirection->x;
    theVector.y = -inDirection->y;
    theVector.z = -inDirection->z;
    
    
    theLight = LightInfiniteCreate(&theColor,&theVector);
    if (theLight)
    {
        theLight->shadow = inCastShadows;
        LightAddToDefined(theLight);
        return kQ3Success;
    }
    return kQ3Failure;
}
/*===========================================================================*\
 *
 *	Routine:	RT_AddPointLight()
 *
 *	Comments:	Adds a point light to the scene
 *
\*===========================================================================*/
TQ3Status
RT_AddPointLight(
			TRTDrawContext			 * /*inContext*/,
            TQ3Point3D               *inLocation,
            TQ3Boolean               inCastShadows,
            const TQ3ColorRGB        *inColor,
            float                    inBrightness)
{
    Color   theColor;
    Vector  thePos;
    Light*  theLight;
    
    rt_Color_Set_Q3ColorRGBIntensity(&theColor,inColor,inBrightness);
    
    thePos.x = inLocation->x;
    thePos.y = inLocation->y;
    thePos.z = inLocation->z;
    
    
    theLight = LightPointCreate(&theColor,
                                &thePos);
    if (theLight)
    {
       theLight->shadow = inCastShadows; 
       LightAddToDefined(theLight);
       return kQ3Success;     
    }
    return kQ3Failure;
}
/*===========================================================================*\
 *
 *	Routine:	RT_AddSpotLight()
 *
 *	Comments:	Adds a spot light to the scene
 *
\*===========================================================================*/
TQ3Status
RT_AddSpotLight(
			TRTDrawContext			 * /*inContext*/,
            TQ3Point3D               *inLocation,
            TQ3Vector3D              *inDirection,
            float                    hotAngle,
            float                    outerAngle,
            TQ3FallOffType           inFallOffType,
            TQ3Boolean               inCastShadows,
            const TQ3ColorRGB        *inColor,
            float                    inBrightness)
{
    Color   theColor;
    Vector  thePos;
    Vector  theVector;
    float   theCoeff;
    Light*  theLight;
    
    rt_Color_Set_Q3ColorRGBIntensity(&theColor,inColor,inBrightness);
    
    thePos.x = inLocation->x;
    thePos.y = inLocation->y;
    thePos.z = inLocation->z;
    
    theVector.x = -inDirection->x;
    theVector.y = -inDirection->y;
    theVector.z = -inDirection->z;
    
    switch(inFallOffType)
    {
        case kQ3FallOffTypeNone:
            theCoeff = 0.0f;
            break;
        case kQ3FallOffTypeLinear:
            theCoeff = 1.0f;
            break;
        case kQ3FallOffTypeExponential:
            theCoeff = 0.8f;
            break;
        case kQ3FallOffTypeCosine:
            theCoeff = 2.0f;
            break;
    }
    theLight = LightSpotCreate(&theColor,
                  &thePos,
                  &theVector,
                  theCoeff,
                  Q3Math_RadiansToDegrees(hotAngle),
                  Q3Math_RadiansToDegrees(outerAngle));
    if (theLight)
    {
       theLight->shadow = inCastShadows;  
       LightAddToDefined(theLight);   
       return kQ3Success; 
    }
    return kQ3Failure;
}

/*===========================================================================*\
 *
 *	Routine:	RT_ResetLights()
 *
 *	Comments:	Deletes the defined lights
 *
\*===========================================================================*/
TQ3Status
RT_ResetLights(
			TRTDrawContext			 * /*inContext*/)
{
	LightDeleteDefinedLights();
	Options.ambient.r = 0.0;
	Options.ambient.g = 0.0;
	Options.ambient.b = 0.0;
	return kQ3Success;
}
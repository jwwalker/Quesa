/*  NAME:
        RS_Light.c

    DESCRIPTION:
        A procedural api for RayShade.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
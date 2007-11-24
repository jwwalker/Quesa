/*  NAME:
        RS_Surface.c

    DESCRIPTION:
        RayShade surface handling

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "RSPrefix.h"

#if __MACH__
	#include <Quesa/QuesaMath.h>
#else
	#include <QuesaMath.h>
#endif

#include "RT_Surface.h"
#include "RT_DrawContext.h"

#include "libcommon/common.h"
#include "libcommon/color.h"
#include "libsurf/surface.h"

/*===========================================================================*\
 *
 *	Routine:	rt_Q3ColorRGB_Compare()
 *
 *	Comments:	Altough comparing floats in that way isn't too clever,
 *			 	it's usefull for us.
 *
\*===========================================================================*/
static TQ3Boolean  
rt_Q3ColorRGB_Compare(
        const TQ3ColorRGB      *from,
        const TQ3ColorRGB      *to)
{
    if (from->r != to->r    ||
        from->g != to->g    ||
        from->b != to->b)
        return kQ3False;
        
    return kQ3True;
}
/*===========================================================================*\
 *
 *	Routine:	rt_Color_SetQ3ColorRGB()
 *
 *	Comments:	Sets the rayshade color to QD3D's.
 *
\*===========================================================================*/
static void
rt_Color_SetQ3ColorRGB(
        Color                   *dst,
        const TQ3ColorRGB       *src)
{
    dst->r = src->r;
    dst->g = src->g;
    dst->b = src->b;
}
/*===========================================================================*\
 *
 *	Routine:	rt_Color_SetIntensity()
 *
 *	Comments:	Sets the rayshade color to intensity.
 *
\*===========================================================================*/
static void
rt_Color_SetIntensity(
        Color                   *dst,
        float                   src)
{
    dst->r = src;
    dst->g = src;
    dst->b = src;
}
/*===========================================================================*\
 *
 *	Routine:	rt_Color_SetColor_Intensity()
 *
 *	Comments:	Sets the rayshade color to intensity.
 *
\*===========================================================================*/
static void
rt_Color_SetColor_Intensity(
        Color                   *dst,
        const Color             *srccolor,
        float                   srcintensity)
{
    dst->r = srccolor->r*srcintensity;
    dst->g = srccolor->g*srcintensity;
    dst->b = srccolor->b*srcintensity;
}

/*===========================================================================*\
 *
 *	Routine:	rt_Color_SetColor_Intensity()
 *
 *	Comments:	Sets the rayshade color to intensity.
 *
\*===========================================================================*/
static void
rt_Color_SetQ3ColorRGB_Intensity(
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
 *	Routine:	rt_Q3ColorRGB_Compare()
 *
 *	Comments:	Initializes the surface data to the default state.
 *
\*===========================================================================*/
void  
RTSurfaceData_Init(
        TRTSurfaceData      *data)
{
    data->diffuseColor.r = 0;
    data->diffuseColor.g = 0;
    data->diffuseColor.b = 0;

    data->specularColor.r = 0;
    data->specularColor.g = 0;
    data->specularColor.b = 0;    

    data->transparencyColor.r = 1.0;
    data->transparencyColor.g = 1.0;
    data->transparencyColor.b = 1.0; 
    
    data->hilightState  = kQ3On;
    data->specularControl = 0.0;
    
    data->ambientCoefficient = 0.0;
}
/*===========================================================================*\
 *
 *	Routine:	RTSurfaceData_Compare()
 *
 *	Comments:	Compares if the two surface data describes the same surface.
 *
\*===========================================================================*/
TQ3Boolean  
RTSurfaceData_Compare(
        const TRTSurfaceData   *from,
        const TRTSurfaceData   *to)
{
    if (from && to &&
    	rt_Q3ColorRGB_Compare(&from->diffuseColor,&to->diffuseColor) &&
        rt_Q3ColorRGB_Compare(&from->specularColor,&to->specularColor) &&
        rt_Q3ColorRGB_Compare(&from->transparencyColor,&to->transparencyColor) &&
        from->specularControl == to->specularControl &&
        from->hilightState == to->hilightState &&
        from->ambientCoefficient == to->ambientCoefficient)
        return kQ3True;
    else
        return kQ3False;
}
/*===========================================================================*\
 *
 *	Routine:	rt_SurfaceAddToDefined()
 *
 *	Comments:	Adds a surface to the list of defined
 *
\*===========================================================================*/
static TQ3Status
rt_SurfaceAddToDefined(
			TRTDrawContext			*drawContext,
			Surface					*inSurface)
{
	if (inSurface == NULL)
		return kQ3Failure;
	inSurface->next = drawContext->definedSurfaces;
	drawContext->definedSurfaces = inSurface;
	
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	rt_NewSurfaceFromData()
 *
 *	Comments:	Creates a new RayShade surface, also adds to the list of 
 *				defined surfaces.
 *
\*===========================================================================*/
static Surface*
rt_NewSurfaceFromData(
		TRTDrawContext			*drawContext,
		TRTSurfaceData			*data)
{
	TQ3Status 	theStatus;
    Surface*    result;
    
    
    result = SurfaceCreate();
    if (result == NULL)
        return NULL;
    
    rt_Color_SetQ3ColorRGB(&result->diff,&data->diffuseColor);
    /* 
     * Is it correct ???(ToDo)
     */
    result->diff.r *= data->transparencyColor.r;
    result->diff.g *= data->transparencyColor.g;
    result->diff.b *= data->transparencyColor.b;
    
    rt_Color_SetIntensity(&result->amb,data->ambientCoefficient);
    result->amb.r *= result->diff.r;
    result->amb.g *= result->diff.g;
    result->amb.b *= result->diff.b;
   

    
    /*
     * Specular control
     */
    rt_Color_SetQ3ColorRGB(&result->spec,&data->specularColor);
    result->srexp = data->specularControl;
    
    // adapted from GLUtils_SpecularControlToGLShininess
    
    result->reflect = (128.0f - (100.0f * 128.0f)/(data->specularControl + 100.0f))*0.008;
    if(result->reflect > 1.0)
    	result->reflect = 1.0;
    if(result->reflect < 0.0)
    	result->reflect = 0.0;
    
    /*
     * Transparency Color: (1,1,1) Opaque, (0,0,0) Transparent
     */
	if (data->transparencyColor.r != 1.0 ||
        data->transparencyColor.g != 1.0 ||
        data->transparencyColor.b != 1.0)
    {
    	result->body.r = 1.0-data->transparencyColor.r;
    	result->body.g = 1.0-data->transparencyColor.g;
    	result->body.b = 1.0-data->transparencyColor.b;
    	
        result->transp = 1.0;
    }    

    theStatus = rt_SurfaceAddToDefined(drawContext,result);
    if (theStatus != kQ3Success)
    	goto cleanup;
    
    return result;  
cleanup:
	if (result)
		SurfaceDeleteOne(result);
	return NULL;  	
}
/*===========================================================================*\
 *
 *	Routine:	RT_SetCurrentSurface()
 *
 *	Comments:	Sets the surace to the current surface.
 *
\*===========================================================================*/
TQ3Status
RT_SetCurrentSurface(
		TRTDrawContext			*drawContext,
		TRTSurfaceData			*data)
{
	Surface*    theSurface;
	
	theSurface = rt_NewSurfaceFromData(drawContext,data);
	if (theSurface == NULL)
		return kQ3Failure;
		
	drawContext->currentSurface = theSurface;
	return kQ3Success;
}

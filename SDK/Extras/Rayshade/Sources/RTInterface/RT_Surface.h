/*  NAME:
        RS_APISurface.h

    DESCRIPTION:
        A procedural api for RayShade.

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
*/
#ifndef _RT_SURFACE_H_
#define _RT_SURFACE_H_

#include "RSPrefix.h"

#include "RT.h"

#if USE_QUESA_INCLUDES
	#include <QuesaMath.h>
	#include <QuesaCamera.h>
	#include <QuesaLight.h>
#else
	#include <QD3DMath.h>
	#include <QD3DCamera.h>
	#include <QD3DLight.h>
#endif
/******************************************************************************
 **																			 **
 **								Data Structure							     **
 **																			 **
 *****************************************************************************/
typedef
struct TRTSurfaceData{
    TQ3ColorRGB         diffuseColor;
    TQ3ColorRGB         specularColor;
    float               ambientCoefficient;
    float               specularControl;
    TQ3ColorRGB         transparencyColor;
    TQ3Switch           hilightState;
} TRTSurfaceData;

/******************************************************************************
 **																			 **
 **								Functions								     **
 **																			 **
 *****************************************************************************/
void  
RTSurfaceData_Init(        
		TRTSurfaceData          *data);
TQ3Boolean  
RTSurfaceData_Compare(
        const TRTSurfaceData   	*from,
        const TRTSurfaceData   	*to); 
TQ3Status
RT_SetCurrentSurface(
		TRTDrawContext			*drawContext,
		TRTSurfaceData			*data);


 
		
#endif /* _RT_SURFACE_H_ */ 
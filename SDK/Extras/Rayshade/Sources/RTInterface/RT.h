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

#ifndef _RT_H_
#define _RT_H_

#include "RSPrefix.h"

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
#else
	#include <QD3D.h>
#endif

/******************************************************************************
 **																			 **
 **								Data Structure							     **
 **																			 **
 *****************************************************************************/
typedef struct TRTDrawContext TRTDrawContext;

/******************************************************************************
 **																			 **
 **								Functions								     **
 **																			 **
 *****************************************************************************/

extern TRTDrawContext *RT_Create();

extern TQ3Status RT_EndScene(TRTDrawContext *inContext);

extern TQ3Status RT_BeginScene(TRTDrawContext *inContext);

extern TQ3Status RT_Reset(TRTDrawContext *inContext);
extern void	RT_Delete(TRTDrawContext *inContext);
/******************************************************************************
 **																			 **
 **						Camera Functions								     **
 **																			 **
 *****************************************************************************/
extern TQ3Status RT_SetCamera(TRTDrawContext *inContext, 
			TQ3Point3D *inCameraLocation,TQ3Point3D *inPointOfInterests,
			TQ3Vector3D *inUpVector,double minfov,
			double  aspectRationXToY);

/******************************************************************************
 **																			 **
 **						Atmosphere Functions							     **
 **																			 **
 *****************************************************************************/
extern TQ3Status RT_SetAtmosphereColor(
						TRTDrawContext			*inContext,
						const TQ3ColorRGB		*inColor);
#endif	/* _RT_H_ */ 
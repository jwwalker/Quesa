/*  NAME:
        RS_APISurface.h

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
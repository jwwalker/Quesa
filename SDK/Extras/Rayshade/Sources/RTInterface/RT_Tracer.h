/*  NAME:
        RS_Tracer.h

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
#ifndef _RT_TRACER_H_
#define _RT_TRACER_H_

#include "RT.h"

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
	#include <QuesaIO.h>
#else
	#include <QD3D.h>
	#include <QD3DIO.h>
#endif

/******************************************************************************
 **																			 **
 **								Data Structure							     **
 **																			 **
 *****************************************************************************/
typedef struct TRTRayTracer		TRTRayTracer;
/******************************************************************************
 **																			 **
 **								Functions								     **
 **																			 **
 *****************************************************************************/
extern TRTRayTracer*
RTRayTracer_Create(
					TRTDrawContext	*inDrawContext,
					int				width,
					int				height);

extern TQ3Status
RTRayTracer_ScanNextLine(
					TRTRayTracer 	*inTracer,
					int 			inCurrentLine,
					TQ3Uns8			outBuffer[][3],
					int				inBufferSize);
extern void 
RTRayTracer_Delete(TRTRayTracer		*theTracer);


#endif _RT_TRACER_H_

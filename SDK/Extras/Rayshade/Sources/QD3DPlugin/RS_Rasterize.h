/*  NAME:
        RS_Rasterize.h

    DESCRIPTION:
        General rasterizer functions

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
 typedef struct TRSRasterizer TRSRasterizer;
/******************************************************************************
 **																			 **
 **						Functions										     **
 **																			 **
 *****************************************************************************/
TRSRasterizer	*RSRasterizer_Create(
 					TQ3DrawContextObject		theDrawContext);
TQ3Status		RSRasterizer_GetSize(
					TRSRasterizer				*inRasterizer,
					int							*outWidth,
					int							*outHeight);
void			RSRasterizer_Delete(
					TRSRasterizer				*inRasterizer);

TQ3Status		RSRasterizer_Lock(
					TRSRasterizer				*inRasterizer);
void			RSResterizer_Unlock(
					TRSRasterizer				*inRasterizer);					

TQ3Status		RSRasterizer_Rasterize_RGB_Span(
					TRSRasterizer				*inRasterizer,
					const int					x,
					const int					y,
					const int					width,
					const unsigned char			rgbPixels[][3]);
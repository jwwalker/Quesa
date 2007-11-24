/*  NAME:
        RS_Rasterize.h

    DESCRIPTION:
        General rasterizer functions

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
#include "RSPrefix.h"

#if __MACH__
	#include <Quesa/Quesa.h>
#else
	#include <Quesa.h>
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
					int							*outHeight,
					int							*outMinX,
					int							*outMinY);
void			RSRasterizer_Delete(
					TRSRasterizer				*inRasterizer);

TQ3Status		RSRasterizer_Lock(
					TRSRasterizer				*inRasterizer);
void			RSRasterizer_Unlock(
					TRSRasterizer				*inRasterizer);

void			RSRasterizer_Start(
					TRSRasterizer				*inRasterizer);
void			RSRasterizer_Finish(
					TRSRasterizer				*inRasterizer);


TQ3Status		RSRasterizer_Rasterize_RGB_Span(
					TRSRasterizer				*inRasterizer,
					const int					x,
					const int					y,
					const int					width,
					const TQ3Float32			rgbaPixels[][4]);




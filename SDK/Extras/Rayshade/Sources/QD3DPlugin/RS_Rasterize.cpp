/*  NAME:
        RSPlugin.h

    DESCRIPTION:
        General rasterizer functions

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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

#include "RT.h"

#include "libshade/rayshade.h"
#include "libshade/viewing.h"


//#include "RSPrefix.h"

#include <stdlib.h>

#if __MACH__
	#include <Quesa/QuesaDrawContext.h>
#else
	#include <QuesaDrawContext.h>
#endif

#include "RS_Rasterize.h"

#if QUESA_OS_MACINTOSH
	#if ((QUESA_UH_IN_FRAMEWORKS) || (QUESA_OS_COCOA))
		#include <Carbon/Carbon.h>
	#else
	#include <QuickDraw.h>
	#include <MacWindows.h>
	#endif
#elif defined(WIN32)
	
#else
	#error "Only MacOS + Win32 API-s are currently supported!"
#endif
#include "libshade/viewing.h"

/******************************************************************************
 **																			 **
 **								Data Structure							     **
 **																			 **
 *****************************************************************************/
typedef TQ3Status (*TRGBSpanRasterizerFunction)(
							TRSRasterizer 				*inRasterizer,
							const int					x,
							const int					y,
							const int					width,
							const TQ3Float32			rgbaPixels[][4]);
				

typedef struct TRSRasterizer {
	bool						isLocked;
	
	int							width, height;
	int							left, top;
#if defined(macintosh)
	CGrafPtr					grafPort;
#elif defined(WIN32)
	HDC							hdc;
#endif
	TQ3Pixmap					pixmap;
	
	TQ3ObjectType				type;
	TRGBSpanRasterizerFunction	rgbSpanRasterize;
	
} TRSRasterizer;

/******************************************************************************
 **																			 **
 **								Prototypes								     **
 **																			 **
 *****************************************************************************/
static
TQ3Status		RSRasterizer_Rasterize_RGB_Span_Nop(
					TRSRasterizer				*inRasterizer,
					const int					x,
					const int					y,
					const int					width,
					const TQ3Float32			rgbaPixels[][4]);
#if defined(macintosh)
static
TQ3Status		RSRasterizer_Rasterize_RGB_Span_QD(
					TRSRasterizer				*inRasterizer,
					const int					x,
					const int					y,
					const int					width,
					const TQ3Float32			rgbaPixels[][4]);
#endif
#if defined(WIN32)
static
TQ3Status		RSRasterizer_Rasterize_RGB_Span_Win32DC(
					TRSRasterizer				*inRasterizer,
					const int					x,
					const int					y,
					const int					width,
					const TQ3Float32			rgbaPixels[][4]);
#endif
static
TQ3Status		RSRasterizer_Rasterize_RGB_Span_PM(
					TRSRasterizer				*inRasterizer,
					const int					x,
					const int					y,
					const int					width,
					const TQ3Float32			rgbaPixels[][4]);
					
					
/******************************************************************************
 **																			 **
 **								Functions								     **
 **																			 **
 *****************************************************************************/					

TRSRasterizer	*RSRasterizer_Create(
 					TQ3DrawContextObject		theDrawContext)
{
	TQ3Status 			theStatus;
	TQ3DrawContextData	theData;
	TQ3ObjectType		theDrawContextType;
	TQ3Pixmap			thePixmap;
	TQ3Area				theArea;

	TRSRasterizer		*result = (TRSRasterizer*)malloc(sizeof(TRSRasterizer));


	if (result == NULL)
		goto cleanup;
	result->isLocked = false;

	
	theStatus = Q3DrawContext_GetData ( theDrawContext , &theData );
	if (theStatus != kQ3Success)
		goto cleanup;	
	
	/*
	 * Get the type specific draw context data
	 */ 
	theDrawContextType = Q3DrawContext_GetType(theDrawContext);

	if(theData.clearImageMethod == kQ3ClearMethodWithColor){
		Screen.background.r = theData.clearImageColor.r;
		Screen.background.g = theData.clearImageColor.g;
		Screen.background.b = theData.clearImageColor.b;
	}
	switch (theDrawContextType)
	{
#if defined(macintosh)
		case kQ3DrawContextTypeMacintosh:
		{
			CWindowPtr theWindow;
			Rect thePortRect;
			theStatus = Q3MacDrawContext_GetWindow(theDrawContext,&theWindow);
			if ((theStatus != kQ3Success) || (theWindow == NULL))
				goto cleanup;
			result->grafPort = GetWindowPort((WindowPtr)theWindow);
			result->type = kQ3DrawContextTypeMacintosh;
			GetPortBounds (result->grafPort, &thePortRect);
			theArea.min.x = thePortRect.left;
			theArea.min.y = thePortRect.top;
			theArea.max.x = thePortRect.right;
			theArea.max.y = thePortRect.bottom;
			break;
		}
#endif
#if defined(WIN32)
		case kQ3DrawContextTypeWin32DC:
		{
			HDC		theHDC;
			HWND	theWindow;
			RECT	windowRect;

			theStatus = Q3Win32DCDrawContext_GetDC (theDrawContext,&theHDC);
			if ((theStatus != kQ3Success) || (theHDC == NULL))
				goto cleanup;
			result->hdc = theHDC;
			result->type = kQ3DrawContextTypeWin32DC;
			theWindow = WindowFromDC( theHDC );
			GetClientRect( theWindow, &windowRect );
			theArea.min.x = windowRect.left;
			theArea.min.y = windowRect.top;
			theArea.max.x = windowRect.right;
			theArea.max.y = windowRect.bottom;
			break;
		}
#endif
		case kQ3DrawContextTypePixmap:
			theStatus = Q3PixmapDrawContext_GetPixmap(theDrawContext, &thePixmap);
			if (theStatus != kQ3Success || thePixmap.image == NULL)
				goto cleanup;
			result->pixmap = thePixmap;
			result->type = kQ3DrawContextTypePixmap;
			theArea.min.x = 0;
			theArea.min.y = 0;
			theArea.max.x = (float)thePixmap.width;
			theArea.max.y = (float)thePixmap.height;
			break;


		default:
			goto cleanup;
			break;
	}
	
	if (!theData.paneState)
	{
		theData.pane = theArea;
	}
	
	result->width 	= (int)(theData.pane.max.x-theData.pane.min.x);
    result->height 	= (int)(theData.pane.max.y-theData.pane.min.y);
    result->left = (int)theData.pane.min.x;
    result->top = (int)theData.pane.min.y;
    
	result->rgbSpanRasterize = RSRasterizer_Rasterize_RGB_Span_Nop;
	return result;
cleanup:
	if (result)	free (result);
	return NULL;
}

void			RSRasterizer_Delete(
					TRSRasterizer				*inRasterizer)
{
	if (inRasterizer == NULL)
		return;
	
	if (inRasterizer->isLocked)
		RSRasterizer_Unlock(inRasterizer);
	
	free(inRasterizer);
}
TQ3Status		RSRasterizer_GetSize(
							TRSRasterizer	*inRasterizer,
							int				*outWidth,
							int				*outHeight,
							int				*outMinX,
							int				*outMinY)
{
	if (inRasterizer == NULL)
		return kQ3Failure;
	*outWidth = inRasterizer->width;
	*outHeight = inRasterizer->height;
	*outMinX = inRasterizer->left;
	*outMinY = inRasterizer->top;
	
	return kQ3Success;
}
TQ3Status		RSRasterizer_Lock(
					TRSRasterizer				*inRasterizer)
{
	if (inRasterizer == NULL)
		return kQ3Failure;
	inRasterizer->isLocked = true;
	switch (inRasterizer->type) {
#if defined(macintosh)
	case kQ3DrawContextTypeMacintosh:
		{	
			SetPort((GrafPtr)inRasterizer->grafPort);
			inRasterizer->rgbSpanRasterize = RSRasterizer_Rasterize_RGB_Span_QD;
			break;
		}
#endif
#if defined(WIN32)
	case kQ3DrawContextTypeWin32DC:
		{	
			inRasterizer->rgbSpanRasterize = RSRasterizer_Rasterize_RGB_Span_Win32DC;
			break;
		}
#endif
	case kQ3DrawContextTypePixmap:
		{
			inRasterizer->rgbSpanRasterize = RSRasterizer_Rasterize_RGB_Span_PM;
			break;
		}
	default:
		{
			Q3_ASSERT(false);
			return kQ3Failure;
		}
	}

	return kQ3Success;
}
void			RSRasterizer_Unlock(
					TRSRasterizer				*inRasterizer)
{
	if (inRasterizer == NULL)
		return;
	
	
	inRasterizer->rgbSpanRasterize = RSRasterizer_Rasterize_RGB_Span_Nop;
	inRasterizer->isLocked = false;
}

void			RSRasterizer_Start(
					TRSRasterizer				*inRasterizer)
{
#ifdef macintosh
 	if (inRasterizer->type == kQ3DrawContextTypeMacintosh)
 	{
 		LockPortBits( (GrafPtr)inRasterizer->grafPort );
 		
		Rect	dirtyRect = {
			inRasterizer->top,
			inRasterizer->left,
			inRasterizer->top + inRasterizer->height,
			inRasterizer->left + inRasterizer->width
		};
		QDAddRectToDirtyRegion( (GrafPtr)inRasterizer->grafPort, &dirtyRect );
 	}
#else
	#pragma unused( inRasterizer )
#endif
}

void			RSRasterizer_Finish(
					TRSRasterizer				*inRasterizer)
{
#ifdef macintosh
 	if (inRasterizer->type == kQ3DrawContextTypeMacintosh)
 	{
 		UnlockPortBits( (GrafPtr)inRasterizer->grafPort );
 	}
#else
	#pragma unused( inRasterizer )
#endif
}

/*===========================================================================*/

static unsigned char
correct(Float x)
{
	/*
	 * Truncate values < 0 or > 1.
	 */
	if (x < 0)
		return 0;
	if (x > 1.)
		return 255;
	return (unsigned char)(x * 255.);
}

/*===========================================================================*/
static
TQ3Status		RSRasterizer_Rasterize_RGB_Span_Nop(
					TRSRasterizer				*inRasterizer,
					const int					/*x*/,
					const int					/*y*/,
					const int					/*width*/,
					const TQ3Float32			/*rgbPixels*/[][4])
{
	if (inRasterizer == NULL)
		return kQ3Failure;
	
	
	return kQ3Success;
}

static
TQ3Status		RSRasterizer_Rasterize_RGB_Span_PM(
					TRSRasterizer				*inRasterizer,
					const int					x,
					const int					y,
					const int					width,
					const TQ3Float32			rgbaPixels[][4])
{
	unsigned char* 	rowAddr;
	int			   	pixelSize = (int)inRasterizer->pixmap.pixelSize / 8;	/* This is pixel size in bytes */
	if (inRasterizer == NULL)
		return kQ3Failure;
	
	rowAddr = (unsigned char*)inRasterizer->pixmap.image+x*pixelSize+y*inRasterizer->pixmap.rowBytes;
	for (int i = 0; i < width; i++)
	{
		switch (inRasterizer->pixmap.pixelType)
		{
			case kQ3PixelTypeRGB32:
			case kQ3PixelTypeARGB32:
				{
					unsigned int pixel;
#if QUESA_HOST_IS_BIG_ENDIAN
					pixel = 	( correct ( rgbaPixels[i][3] ) << 24) |  // Alpha
								( correct ( rgbaPixels[i][0] ) << 16) | 
								( correct ( rgbaPixels[i][1] ) << 8)  |
								( correct ( rgbaPixels[i][2] ) << 0 ) ;
					*((unsigned int *)rowAddr) = pixel;
#else
					pixel = 	( correct ( rgbaPixels[i][3] ) << 0 ) |   // Alpha 
								( correct ( rgbaPixels[i][0] ) << 8)  | 
								( correct ( rgbaPixels[i][1] ) << 16) |
								( correct ( rgbaPixels[i][2] ) << 24 ) ;
					*((unsigned int *)rowAddr) = pixel;
#endif
				}
				break;

			case kQ3PixelTypeRGB24:
#if QUESA_HOST_IS_BIG_ENDIAN
				{
					rowAddr[0] = correct ( rgbaPixels[i][0] ) ;
					rowAddr[1] = correct ( rgbaPixels[i][1] ) ;
					rowAddr[2] = correct ( rgbaPixels[i][2] ) ;
				}
#else
				{
					rowAddr[0] = correct ( rgbaPixels[i][2] ) ;
					rowAddr[1] = correct ( rgbaPixels[i][1] ) ;
					rowAddr[2] = correct ( rgbaPixels[i][0] ) ;
				}
#endif
				break;
			case kQ3PixelTypeRGB16:
			case kQ3PixelTypeARGB16:
				{
					unsigned short pixel;
				
					pixel = (0x8000) | // Could check alpha > 0.5 here andd save in the single bit if desired
							(( correct ( rgbaPixels[i][0] ) << 7) & 0x7C00) |
							(( correct ( rgbaPixels[i][1] ) << 2) & 0x03E0) |
							(( correct ( rgbaPixels[i][2] ) >> 3) & 0x001F) ;
					*((unsigned short*)rowAddr) = pixel;
				}
				break;
			case kQ3PixelTypeRGB16_565:
				{
					unsigned short pixel;
				
					pixel = (( correct ( rgbaPixels[i][0] ) << 8) & 0xF800) |
							(( correct ( rgbaPixels[i][1] ) << 2) & 0x07E0) |
							(( correct ( rgbaPixels[i][2] ) >> 3) & 0x001F) ;
					*((unsigned short*)rowAddr) = pixel;
				}
				break;				
			default:
				{
					Q3_ASSERT(false);
					return kQ3Failure;
				}
		}
		rowAddr = (unsigned char*)rowAddr+pixelSize;
	}
	
	return kQ3Success;
}
#if defined (macintosh)
static
TQ3Status		RSRasterizer_Rasterize_RGB_Span_QD(
					TRSRasterizer				*inRasterizer,
					const int					x,
					const int					y,
					const int					width,
					const TQ3Float32			rgbaPixels[][4])
{
	if (inRasterizer == NULL)
		return kQ3Failure;
	
	for (int i = 0; i < width; i++)
	{
		RGBColor	theColor;
		theColor.red = correct ( rgbaPixels[i][0] ) << 8;
		theColor.green = correct ( rgbaPixels[i][1] ) << 8;
		theColor.blue = correct ( rgbaPixels[i][2] ) << 8;
		
		RGBForeColor(&theColor);
		MoveTo(x+i,y);
		LineTo(x+i,y);
	}
	
	return kQ3Success;
}
#endif
#if defined (WIN32)
static
TQ3Status		RSRasterizer_Rasterize_RGB_Span_Win32DC(
					TRSRasterizer				*inRasterizer,
					const int					x,
					const int					y,
					const int					width,
					const TQ3Float32			rgbaPixels[][4])
{
	if (inRasterizer == NULL)
		return kQ3Failure;
	
	for (int i = 0; i < width; i++)
	{
		RECT theRect;
		COLORREF theColor = ( correct ( rgbaPixels[i][2] ) << 16 ) | ( correct ( rgbaPixels[i][1] ) << 8 ) |
							( correct ( rgbaPixels[i][0] ) ) ; // Alpha is available in [3] but can Windows use it?

		SetRect(&theRect,x+i,y,x+i+1,y+1);
		HBRUSH theBrush = CreateSolidBrush(theColor); 
		FillRect(inRasterizer->hdc,&theRect,theBrush);
		DeleteObject (theBrush);
	}
	
	return kQ3Success;
}
#endif
TQ3Status		RSRasterizer_Rasterize_RGB_Span(
					TRSRasterizer				*inRasterizer,
					const int					x,
					const int					y,
					const int					width,
					const TQ3Float32			rgbaPixels[][4])
{
	return (inRasterizer->rgbSpanRasterize)(inRasterizer,x,y,width,rgbaPixels);
}


#if 0
template <class tTargetPixelType, bool tClip>
TQ3Status SRScanline_Rasterize(
					const TRSRendererPrivate 	*rsPrivate, 
					const int					x,
					const int					y,
					const int					width, 
					const TQ3Uns8				*data[][3])
{
	tTargetPixel		*cBuffer;

	long				clipX, clipY;
	TQ3Bitmap			*clipMask = NULL;
	unsigned long		*clipMaskPtr, clipBits, clipXMask;
	unsigned long		clipMaskRowBytes;
	float				upperLeftX, upperLeftY;	
	
	long				longX, longY;
	unsigned long		leftOver;
	unsigned char		leftOverPickIndex = 0x80;
	unsigned char		*cRowPtr;
	
	const TQ3XDrawRegionDescriptor	*descriptor;
	void							*image;
	
	descriptor = srPrivate->descriptor;
	image = srPrivate->image;
	rowBytes = descriptor->rowBytes;	

	if (tClip)
	{
		if (Q3XDrawRegion_GetClipMask(srPrivate->drawRegion, &clipMask) == kQ3Failure) {
			return (kQ3Failure);
		}
		if (clipMask == NULL) {
			return (kQ3Failure);
		}
	}
	
	longX = (long)x;
	longY = (long)y;
	
	cBuffer = (tTargetPixelType *)((unsigned char *) image +
				(((long)longY * rowBytes) + 
					(long)longX * sizeof(tTargetPixelType)));		
	
	if (tClip)
	{
	clipMaskRowBytes = clipMask->rowBytes;
	Q3XDrawRegion_GetDeviceOffsetX(srPrivate->drawRegion, &upperLeftX);
	Q3XDrawRegion_GetDeviceOffsetY(srPrivate->drawRegion, &upperLeftY);

	clipX = longX - upperLeftX;
	clipY = longY - upperLeftY;
	clipMaskPtr = (unsigned long *)((unsigned char *)clipMask->image +
									(clipY * clipMaskRowBytes) + ((clipX >> 5) << 2));
	clipBits = *clipMaskPtr;
	clipXMask = (unsigned long)(1 << (0x1F - (clipX & 0x1F)));
		
	}
}

#endif

/*  NAME:
        RSPlugin.h

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
#include "QD3D.h"
#include "QD3DStorage.h"
 
#include "RSPlugin.h"


#undef SR_PIXELTYPE

#ifdef SR_RASTERIZE_8BITS
	#define	SR_PIXELTYPE		unsigned char
	#define	SR_SIZEOF_PIXEL		1
#endif  /*  SR_RASTERIZE_8BITS  */


#ifdef SR_RASTERIZE_32BITS
	#define	SR_PIXELTYPE		unsigned long
	#define	SR_SIZEOF_PIXEL		4
#endif  /*  SR_RASTERIZE_32BITS  */


/*===========================================================================*\
 *
 *	Routine:	RSScanline_Rasterize_8
 *				RSScanline_Rasterize_8_WClip
 *				RSScanline_Rasterize_32
 *				RSScanline_Rasterize_32_WClip
 *
 *	Comments:	
 *
\*===========================================================================*/

#ifdef SR_RASTERIZE_32BITS
	#ifdef SR_WIN_CLIP
		TQ3Status SRScanline_Rasterize_32_WClip(
	#else
		TQ3Status RSScanline_Rasterize_32(
	#endif  /*  SR_WIN_CLIP  		*/
#endif  /*  SR_RASTERIZE_32BITS  */

#ifdef SR_RASTERIZE_8BITS
	#ifdef SR_WIN_CLIP
		TQ3Status RSScanline_Rasterize_8_WClip(
	#else
		TQ3Status RSScanline_Rasterize_8(
	#endif  /*  SR_WIN_CLIP  		*/
#endif  /*  SR_RASTERIZE_8BITS	*/

	const TSRPrivate 			*srPrivate, 
	const int					x,
	const int					y,
	const int					width, 
	const unsigned char			*colors[][3])
{
	long				rowBytes;

	SR_PIXELTYPE		*cBuffer;
	
#ifdef SR_WIN_CLIP
	long				clipX, clipY;
	TQ3Bitmap			*clipMask = NULL;
	unsigned long		*clipMaskPtr, clipBits, clipXMask;
	unsigned long		clipMaskRowBytes;
	float				upperLeftX, upperLeftY;
#endif  /*  SR_WIN_CLIP  */

#ifdef SR_RASTERIZE_8BITS	 
	SR_PIXELTYPE		pointColor;
#endif  /*  SR_RASTERIZE_8BITS  */

	long				longX, longY;
	unsigned long		leftOver;
	unsigned char		leftOverPickIndex = 0x80;
	unsigned char		*cRowPtr;
	unsigned char		*markerRowPtr;
	
	unsigned long		*marker32ARGB;
	unsigned short		*marker16ARGB;

	unsigned long		highlightMask;
	
	TQ3PixelType		markerPixelType;
	
#ifdef SR_RASTERIZE_8BITS	 
	unsigned long		color;
	unsigned char		r8, g8, b8;
#endif  /*  SR_RASTERIZE_8BITS  */

	const TQ3XDrawRegionDescriptor	*descriptor;
	void							*image;

	descriptor = srPrivate->descriptor;
	image = srPrivate->image;
	rowBytes = descriptor->rowBytes;
	
	markerPixelType = pixmapMarkerRaster->pixmap->pixelType;
	
	

#ifdef SR_WIN_CLIP
	if (Q3XDrawRegion_GetClipMask(srPrivate->drawRegion, &clipMask) == kQ3Failure) {
		return (kQ3Failure);
	}
	if (clipMask == NULL) {
		return (kQ3Failure);
	}
	
#endif  /*  SR_WIN_CLIP  */

	longX = (long)x;
	longY = (long)y;
			   
	cBuffer = (SR_PIXELTYPE *)((unsigned char *) image +
				(((long)longY * rowBytes) + 
					(long)longX * SR_SIZEOF_PIXEL));


#ifdef SR_WIN_CLIP

	clipMaskRowBytes = clipMask->rowBytes;
	Q3XDrawRegion_GetDeviceOffsetX(srPrivate->drawRegion, &upperLeftX);
	Q3XDrawRegion_GetDeviceOffsetY(srPrivate->drawRegion, &upperLeftY);

	clipX = longX - upperLeftX;
	clipY = longY - upperLeftY;
	clipMaskPtr = (unsigned long *)((unsigned char *)clipMask->image +
									(clipY * clipMaskRowBytes) + ((clipX >> 5) << 2));
	clipBits = *clipMaskPtr;
	clipXMask = (unsigned long)(1 << (0x1F - (clipX & 0x1F)));
	 
#endif  /*  SR_WIN_CLIP  */

	cRowPtr = (unsigned char *)cBuffer;
	
	{
		/*
		 *  Get image from pixmap storage 
		 */
		unsigned long size;
		
		if (pixmapMarkerRaster->pixmap->image != NULL) {
			Q3MemoryStorage_GetBuffer(
				pixmapMarkerRaster->pixmap->image, 
				&markerRowPtr, 
				&size,
				NULL);
		} else {
			return (kQ3Failure);
		}
	}
	
	leftOver = width & 0x00000007;
	width   -= leftOver;

	/* 
	 *  This is true if the marker is clipped on the top 
	 */
	if (pixmapMarkerRaster->startLineSkip) {
		markerRowPtr += pixmapMarkerRaster->startLineSkip * markerRowBytes;
	}
	
	/*
	 *  This is true if the marker is clipped on the left 
	 */
	if (pixmapMarkerRaster->startRowSkip) {
		width -= pixmapMarkerRaster->startRowSkip;
		
		if (markerPixelType == kQ3PixelTypeRGB32 ||
			markerPixelType == kQ3PixelTypeARGB32) {
			markerRowPtr += (pixmapMarkerRaster->startRowSkip << 2);
		} else if (markerPixelType == kQ3PixelTypeRGB16 ||
				   markerPixelType == kQ3PixelTypeARGB16) {
			markerRowPtr += (pixmapMarkerRaster->startRowSkip << 1);
		}

		if (width < 0) {
			leftOver += width;
			leftOverPickIndex >>= -width;
		}
	}

	do {
		long			scanWidth, scanLeftOver;
		unsigned char 	pickIndex = 0x80 >> (pixmapMarkerRaster->startRowSkip & 0x7);

		cBuffer = (SR_PIXELTYPE *)cRowPtr;
		
		if (markerPixelType == kQ3PixelTypeRGB32 ||
			markerPixelType == kQ3PixelTypeARGB32) {
			marker32ARGB = (unsigned long *)markerRowPtr;
		}
		if (markerPixelType == kQ3PixelTypeRGB16 ||
			markerPixelType == kQ3PixelTypeARGB16) {
			marker16ARGB = (unsigned short *)markerRowPtr;
		}

		scanWidth = width;
		
		if (scanWidth > 0) {
			do {
				do {
					if ((1)
#ifdef SR_WIN_CLIP
						&& (clipBits & clipXMask)
#endif  /*  SR_WIN_CLIP  */
					   ) {

#ifdef SR_RASTERIZE_32BITS
					
						if (markerPixelType == kQ3PixelTypeARGB32) {
							if ((*marker32ARGB & 0xFF000000) == 0) {
								/* 
								 *  If alpha is not set don't turn on the pixel 
								 */
							} else {
								if (highlightColor == NULL) {
									*cBuffer = *marker32ARGB;
								} else {
									*cBuffer = highlightMask | (*marker32ARGB & 0xFF000000);
								}
							}
								
							marker32ARGB++;
						} else if (markerPixelType == kQ3PixelTypeRGB32) {
							if (highlightColor == NULL) {
								*cBuffer = *marker32ARGB | 0xFF000000;
							} else {
								*cBuffer = highlightMask | 0xFF000000;
							}
								
							marker32ARGB++;
						} else if (markerPixelType ==  kQ3PixelTypeARGB16) {
							if ((*marker16ARGB & 0x8000) == 0)  {
								/* 
								 *  If alpha is not set don't turn on the pixel 
								 */
							} else {
								if (highlightColor == NULL) {
									/* scale 16 to 32 */
									unsigned short r, g, b;
									
									r = ((*marker16ARGB >> 10) & 0x1F) << 3;
									g = ((*marker16ARGB >>  5) & 0x1F) << 3;
									b = ((*marker16ARGB >>  0) & 0x1F) << 3;
									
									/*
									 *  Set the r, g, and b
									 */
									*cBuffer = ((r << 16) | (g << 8) | b) | 0xFF000000;
								} else {
									*cBuffer = highlightMask | 0xFF000000;
								} 
							}	
							marker16ARGB++;
						} else if (markerPixelType == kQ3PixelTypeRGB16) {
							if (highlightColor == NULL) {
								/* scale 16 to 32 */
								unsigned short r, g, b;
								
								r = ((*marker16ARGB >> 10) & 0x1F) << 3;
								g = ((*marker16ARGB >>  5) & 0x1F) << 3;
								b = ((*marker16ARGB >>  0) & 0x1F) << 3;
								
								*cBuffer = ((r << 16) | (g << 8) | b) | 0xFF000000;
							} else {
								*cBuffer = highlightMask | 0xFF000000;
							} 
							marker16ARGB++;
						}

#endif  /*  SR_RASTERIZE_32BITS  */


#ifdef SR_RASTERIZE_8BITS				
						pointColor = *cBuffer;

						if (markerPixelType == kQ3PixelTypeRGB32) {
							if (highlightColor == NULL) {
								color = *marker32ARGB;
							} else {
								color = highlightMask;
							}
								
							r8 = (color >> 16) & 0xFF;
							g8 = (color >>  8) & 0xFF;
							b8 = (color >>  0) & 0xFF;
		
							COLOR_8_TO_INDEX(
								pointColor,
								srPrivate->drawRegion,
								descriptor->pixelType,
								r8,
								g8, 
								b8);
								
							marker32ARGB++;
						} else if (markerPixelType == kQ3PixelTypeARGB32) {
							if ((*marker32ARGB & 0xFF000000) == 0) {
								/* 
								 *  If alpha is not set don't turn on the pixel 
								 */
							} else {
								if (highlightColor == NULL) {
									color = *marker32ARGB;
								} else {
									color = highlightMask;
								}
									
								r8 = (color >> 16) & 0xFF;
								g8 = (color >>  8) & 0xFF;
								b8 = (color >>  0) & 0xFF;
			
								COLOR_8_TO_INDEX(
									pointColor,
									srPrivate->drawRegion,
									descriptor->pixelType,
									r8,
									g8, 
									b8);
							}
							marker32ARGB++;
						} else if (markerPixelType == kQ3PixelTypeRGB16) {
							if (highlightColor == NULL) {
								color = *marker16ARGB;
								
								r8 = (color >> 10) & 0x1F;
								g8 = (color >>  5) & 0x1F;
								b8 = (color >>  0) & 0x1F;
							} else {
								color = highlightMask;
								
								r8 = (color >> 16) & 0xFF;
								g8 = (color >>  8) & 0xFF;
								b8 = (color >>  0) & 0xFF;
							}
								
							COLOR_5_TO_INDEX(
								pointColor,
								srPrivate->drawRegion,
								descriptor->pixelType,
								r8,
								g8, 
								b8);
							
							marker16ARGB++;
						} else if (markerPixelType == kQ3PixelTypeARGB16) {
							if ((*marker16ARGB & 0x8000) == 0) {
								/* 
								 *  If alpha is not set don't turn on the pixel 
								 */
							} else {	
								if (highlightColor == NULL) {
									color = *marker16ARGB;
									
									r8 = (color >> 10) & 0x1F;
									g8 = (color >>  5) & 0x1F;
									b8 = (color >>  0) & 0x1F;
								} else {
									color = highlightMask;
									
									r8 = (color >> 16) & 0xFF;
									g8 = (color >>  8) & 0xFF;
									b8 = (color >>  0) & 0xFF;
								}
									
								COLOR_5_TO_INDEX(
									pointColor,
									srPrivate->drawRegion,
									descriptor->pixelType,
									r8,
									g8, 
									b8);
							}
							
							marker16ARGB++;
						}
						
						*cBuffer = pointColor;
						
#endif  /*  SR_RASTERIZE_8BITS  */
						cBuffer++;
					}

#ifdef SR_WIN_CLIP
					clipXMask >>= 1;
					if (clipXMask == 0) {
						clipMaskPtr++;
						clipXMask = (unsigned long)(1 << 0x1F);
						clipBits = *clipMaskPtr;
					}
#endif  /*  SR_WIN_CLIP  */

				} while( pickIndex >>= 1);
				
				pickIndex = 0x80;
			} while ((scanWidth -= 8) > 0);
		}
		
		scanLeftOver 	= leftOver;
		pickIndex 		= leftOverPickIndex;
		
		if (scanLeftOver > 0) {
			do {
				if ((1) 
#ifdef SR_WIN_CLIP
					&& (clipBits & clipXMask)
#endif  /*  SR_WIN_CLIP  */
					){

#ifdef SR_RASTERIZE_32BITS
				
					if (markerPixelType == kQ3PixelTypeARGB32) {
						if ((*marker32ARGB & 0xFF000000) == 0) {
							/* 
							 *  If alpha is not set don't turn on the pixel 
							 */
						} else {
							if (highlightColor == NULL) {
								*cBuffer = *marker32ARGB;
							} else {
								*cBuffer = highlightMask | (*marker32ARGB & 0xFF000000);
							}
						}
							
						marker32ARGB++;
					} else if (markerPixelType == kQ3PixelTypeRGB32) {
						if (highlightColor == NULL) {
							*cBuffer = *marker32ARGB | 0xFF000000;
						} else {
							*cBuffer = highlightMask | 0xFF000000;
						}
							
						marker32ARGB++;
					} else if (markerPixelType ==  kQ3PixelTypeARGB16) {
						if ((*marker16ARGB & 0x8000) == 0)  {
							/* 
							 *  If alpha is not set don't turn on the pixel 
							 */
						} else {
							if (highlightColor == NULL) {
								/* scale 16 to 32 */
								unsigned short r, g, b;
								
								r = ((*marker16ARGB >> 10) & 0x1F) << 3;
								g = ((*marker16ARGB >>  5) & 0x1F) << 3;
								b = ((*marker16ARGB >>  0) & 0x1F) << 3;
								
								/*
								 *  Set the r, g, and b
								 */
								*cBuffer = ((r << 16) | (g << 8) | b) | 0xFF000000;
							} else {
								*cBuffer = highlightMask | 0xFF000000;
							} 
						}	
						marker16ARGB++;
					} else if (markerPixelType == kQ3PixelTypeRGB16) {
						if (highlightColor == NULL) {
							/* scale 16 to 32 */
							unsigned short r, g, b;
							
							r = ((*marker16ARGB >> 10) & 0x1F) << 3;
							g = ((*marker16ARGB >>  5) & 0x1F) << 3;
							b = ((*marker16ARGB >>  0) & 0x1F) << 3;
							
							*cBuffer = ((r << 16) | (g << 8) | b) | 0xFF000000;
						} else {
							*cBuffer = highlightMask | 0xFF000000;
						} 
						marker16ARGB++;
					}

#endif  /*  SR_RASTERIZE_32BITS  */


#ifdef SR_RASTERIZE_8BITS				
					pointColor = *cBuffer;

					if (markerPixelType == kQ3PixelTypeRGB32) {
						if (highlightColor == NULL) {
							color = *marker32ARGB;
						} else {
							color = highlightMask;
						}
							
						r8 = (color >> 16) & 0xFF;
						g8 = (color >>  8) & 0xFF;
						b8 = (color >>  0) & 0xFF;
	
						COLOR_8_TO_INDEX(
							pointColor,
							srPrivate->drawRegion,
							descriptor->pixelType,
							r8,
							g8, 
							b8);
							
						marker32ARGB++;
					} else if (markerPixelType == kQ3PixelTypeARGB32) {
						if ((*marker32ARGB & 0xFF000000) == 0) {
							/* 
							 *  If alpha is not set don't turn on the pixel 
							 */
						} else {
							if (highlightColor == NULL) {
								color = *marker32ARGB;
							} else {
								color = highlightMask;
							}
								
							r8 = (color >> 16) & 0xFF;
							g8 = (color >>  8) & 0xFF;
							b8 = (color >>  0) & 0xFF;
		
							COLOR_8_TO_INDEX(
								pointColor,
								srPrivate->drawRegion,
								descriptor->pixelType,
								r8,
								g8, 
								b8);
						}
						marker32ARGB++;
					} else if (markerPixelType == kQ3PixelTypeRGB16) {
						if (highlightColor == NULL) {
							color = *marker16ARGB;
							
							r8 = (color >> 10) & 0x1F;
							g8 = (color >>  5) & 0x1F;
							b8 = (color >>  0) & 0x1F;
						} else {
							color = highlightMask;
							
							r8 = (color >> 16) & 0xFF;
							g8 = (color >>  8) & 0xFF;
							b8 = (color >>  0) & 0xFF;
						}
	
						COLOR_5_TO_INDEX(
							pointColor,
							srPrivate->drawRegion,
							descriptor->pixelType,
							r8,
							g8, 
							b8);
						
						marker16ARGB++;
					} else if (markerPixelType == kQ3PixelTypeARGB16) {
						if ((*marker16ARGB & 0x8000) == 0) {
							/* 
							 *  If alpha is not set don't turn on the pixel 
							 */
						} else {	
						
							if (highlightColor == NULL) {
								color = *marker16ARGB;
								
								r8 = (color >> 10) & 0x1F;
								g8 = (color >>  5) & 0x1F;
								b8 = (color >>  0) & 0x1F;
							} else {
								color = highlightMask;
								
								r8 = (color >> 16) & 0xFF;
								g8 = (color >>  8) & 0xFF;
								b8 = (color >>  0) & 0xFF;
							}
		
							COLOR_5_TO_INDEX(
								pointColor,
								srPrivate->drawRegion,
								descriptor->pixelType,
								r8,
								g8, 
								b8);
						}
						
						marker16ARGB++;
					}
					
					*cBuffer = pointColor;
						
#endif  /*  SR_RASTERIZE_8BITS  */

					cBuffer++;
				}
				
#ifdef SR_WIN_CLIP
				clipXMask >>= 1;
				if (clipXMask == 0) {
					clipMaskPtr++;
					clipXMask = (unsigned long)(1 << 0x1F);
					clipBits = *clipMaskPtr;
				}
#endif  /*  SR_WIN_CLIP  */

				pickIndex >>= 1;
			} while (--scanLeftOver && pickIndex);
		}

		markerRowPtr += markerRowBytes;
		cRowPtr 	 += rowBytes;
		
	} while (--height);
	
	return (kQ3Success);
}

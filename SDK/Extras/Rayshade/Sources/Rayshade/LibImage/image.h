/*
 * image.h
 *
 * Copyright (C) 1989, 1991, Rod G. Bogart, Craig E. Kolb
 * All rights reserved.
 *
 * This software may be freely copied, modified, and redistributed
 * provided that this copyright notice is preserved on all copies.
 *
 * You may not distribute this software, in whole or in part, as part of
 * any commercial product without the express consent of the authors.
 *
 * There is no warranty or other guarantee of fitness of this software
 * for any purpose.  It is provided solely "as is".
 *
 * $Id: image.h,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:33:39  kolb
 * Initial version.
 * 
 */
#ifndef IMAGE_H
#define IMAGE_H

/*
 * Generic image object for texture map storage.
 */
typedef struct Image {
	int	width, height,		/* Image size */
		chan, has_alpha,	/* # of channels, has alpha info? */
		totalchan,		/* # channels + any alpha channel */
		chansize;		/* width * height */
	unsigned char *data;		/* Image */
	char	*filename;		/* Filename (identifier) */
	struct Image *next;		/* Next image in list. */
} Image;


typedef enum TRSPixelType {
	kRSPixelTypeRGB32	 		= 0,	/* Alpha:8 (ignored), R:8, G:8, B:8	*/
	kRSPixelTypeRGB24			= 1		/* R: 8,G: 8, B:8 */
} TRSPixelType;

Image			*ImageCreateFromMemory(
					unsigned char 	*pixels,
					int 			rowBytes,
					int				width,
					int				height,
					TRSPixelType	inPixelType);
		
Image			*ImageCreateFromFile(char *filename); 
Image			*ImageFind(char *name);
Image			*ImageRead(char *filename);
void			ImageIndex(Image *img, int ix,int iy,Float fx,Float fy,int smooth,Float outval[4]);
void			ImageDelete(Image *img);

#endif /* IMAGE_H */

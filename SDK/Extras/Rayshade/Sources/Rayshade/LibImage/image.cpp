/*
 * image.c
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
 * $Id: image.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:33:29  kolb
 * Initial version.
 * 
 */
#include <stdio.h>
#include "libcommon/common.h"
#include "image.h"
#ifdef URT
#include "rle.h"
#endif

#include <string.h>

Image *image_list = NULL;		/* Linked list of images */


const int kPixelType_Unknown = -1;
static int getPixelSize(TRSPixelType inPixelType)
{
	switch(inPixelType)
	{
		case kRSPixelTypeRGB32: 	return 4;
		case kRSPixelTypeRGB24: 	return 3;
		default:					return kPixelType_Unknown;
	}
}
static int getPixelChannels(TRSPixelType inPixelType)
{
	switch(inPixelType)
	{
		case kRSPixelTypeRGB32: 	return 3;
		case kRSPixelTypeRGB24: 	return 3;
		default:					return kPixelType_Unknown;
	}
}

Image *
ImageCreateFromMemory(
			unsigned char	*pixels,
			int				rowBytes,
			int				width,
			int				height,
			TRSPixelType	inPixelType)
{
	Image *result;
	int thePixelSize;
	int thePixelChannels;
	int x,y;
	unsigned char *rbuf,*gbuf,*bbuf;
	
	result = NULL;
	thePixelSize = getPixelSize(inPixelType);
	if (thePixelSize == kPixelType_Unknown)
		goto cleanup;
	thePixelChannels = getPixelChannels(inPixelType);
	if (thePixelChannels == kPixelType_Unknown)
		goto cleanup;
	
	result = (Image*)Calloc(1,sizeof(Image));
	if (!result)
		goto cleanup;
	
	result->width = width;
	result->height = height;
	result->chan = thePixelChannels;
	result->has_alpha= 0;
	result->totalchan = thePixelChannels;
	result->chansize =width*height;
	result->data = NULL;
	result->next = NULL;
	result->filename = NULL;
	
	result->data= (unsigned char*)Malloc(
					result->chansize * result->totalchan * sizeof(unsigned char));
	
	if (result->data == NULL)
		goto cleanup;
	
	rbuf = result->data;
	gbuf = &result->data[result->chansize];
	bbuf = &result->data[result->chansize+result->chansize];
	
	for (y = 0; y < height; y++)
	{
		unsigned char *currentRow = pixels+rowBytes*y;
		for (x = 0; x < width; x++)
		{
			switch (inPixelType)
			{
				case kRSPixelTypeRGB32:
					*(rbuf++) = currentRow[1]; 
					*(gbuf++) = currentRow[2];
					*(bbuf++) = currentRow[3];
					break;
				case kRSPixelTypeRGB24:
					*(rbuf++) = currentRow[0]; 
					*(gbuf++) = currentRow[1];
					*(bbuf++) = currentRow[2];
					break;
				default:
					goto cleanup;
			}
			
			currentRow+=thePixelSize;
		} 
	}
	return result;
cleanup:
	if (result)	
	{
		if (result->data) Free(result->data);
		result->data = NULL;
		
		Free(result);
	}
	result =NULL;
	return NULL;	
}

void
ImageDelete(Image *inImage)
{
	if (inImage->data)
		Free(inImage->data);
	Free(inImage);
}

Image *
ImageCreateFromFile(char *filename)
{
	Image *thenew;

	thenew = (Image *)Malloc(sizeof(Image));
	thenew->filename = strsave(filename);
	thenew->width = 0;
	thenew->height = 0;
	thenew->chan = 0;
	thenew->data = NULL;
	thenew->next = image_list;
	image_list = thenew;
	return thenew;
}

Image *
ImageFind(char *name)
{
	Image *im;

	for (im = image_list; im; im = im->next) {
		if (strcmp(im->filename, name) == 0)
			return im;
	}

	return (Image *)NULL;
}

#ifdef URT
Image *
ImageRead(char *filename)
{
	FILE *fp;
	int i, y, chan;
	rle_hdr in_hdr;
	Image *image;
	rle_pixel **inrows;

	/*
	 * See if image has been read before.
	 */
	image = ImageFind(filename);
	if (image)
		return image;

	fp = fopen(filename, "r");
	if (fp == (FILE *)NULL) {
		RLerror(RL_ABORT, "Cannot open RLE file %s.\n",filename);
		return (Image *)NULL;
	}

	in_hdr.rle_file = fp;
   
	/* Try to open the RLE file */
	if (rle_get_setup(&in_hdr) < 0) {
		RLerror(RL_ABORT, "Error reading header of %s\n", filename);
		return (Image *)NULL;
	}

	/*
	 * Read new image
	 */
	image = ImageCreate(filename);

	in_hdr.xmax -= in_hdr.xmin;
	in_hdr.xmin = 0;
	image->width = in_hdr.xmax + 1;
	image->height = in_hdr.ymax - in_hdr.ymin + 1;
	image->chan = in_hdr.ncolors;
	image->has_alpha = in_hdr.alpha ? 1 : 0;
	image->totalchan = image->chan + image->has_alpha;
	image->chansize = image->width * image->height;

	image->data = (unsigned char *) Malloc(
		image->chansize * image->totalchan * sizeof(unsigned char));

	/*
	 * Allocate array of pointers to pass to rle_getrow.
	 */
	inrows = (rle_pixel **)Malloc(image->totalchan *
			sizeof(rle_pixel *));
	/*
	 * Set inrows to point to appropriate initial location in image.
	 */
	inrows[0] = (rle_pixel *)image->data;
	for (i = 1; i < image->totalchan; i++)
		inrows[i] = inrows[i-1] + image->chansize;
	if (image->has_alpha)
		/* Alpha channel lives in channel -1 */
		inrows++;

	/* Read the image */
	for ( y = 0; y < image->height; y++ ) {
		rle_getrow( &in_hdr, inrows );
		/*
		 * Update inrows to point to next scanline for
		 * each channel.
		 */
		for (i = 0; i < image->chan; i++)
			inrows[i] += image->width;
		if (image->has_alpha)
			inrows[-1] += image->width;
	}

	(void)fclose(fp);
	return image;
}

#else /* !URT */

Image *
ImageRead(char *filename)
{
	FILE *fp;
	char buf[80];
	Image *image;
	int y, x;
	unsigned char *rbuf, *gbuf, *bbuf;

	image = ImageFind(filename);
	if (image)
		return image;

	fp = fopen(filename, "r");
	if (fp == (FILE *)NULL) {
		RLerror(RL_ABORT, "Cannot open image file %s.\n",filename);
		return (Image *)NULL;
	}

	image = ImageCreateFromFile(filename);
	/*
	 * Read image header.
	 */
	if (fgets(buf, 100, fp) == (char *)NULL ||
	    sscanf(buf, "%d %d\n", &image->width, &image->height) != 2) {
		RLerror(RL_ABORT, "Cannot read header of image file %s.\n",
			filename);
		fclose(fp);
		return (Image *)NULL;
	}
	/*
	 * Generic image files always have 3 channels, no alpha.
	 */
	image->chan = image->totalchan = 3;
	image->has_alpha = 0;
	image->chansize = image->width * image->height;

	image->data = (unsigned char *) Malloc(
		image->chansize * image->totalchan * sizeof(unsigned char));

	rbuf = image->data;
	gbuf = &image->data[image->chansize];
	bbuf = &image->data[image->chansize+image->chansize];
	for (y = 0; y < image->height; y++ ) {
		for (x = 0; x < image->width; x++) {
			*(rbuf++) = getc(fp);
			*(gbuf++) = getc(fp);
			*(bbuf++) = getc(fp);
			if (feof(fp)) {
				RLerror(RL_ABORT,
				"Error reading image %s\n",filename);
				fclose(fp);
				return (Image *)NULL;
			}
		}
	}

	(void)fclose(fp);
	return image;
}
#endif

void
ImageIndex(Image *img, int ix,int iy,Float fx,Float fy,int smooth,Float outval[4])
{
	int xplus, yplus, chan, offset;
	Float x0y0, x1y0, x0y1, x1y1;
	unsigned char *data;

	if (smooth) {
		/*
		 * bi-linear interp of four pixels.  Note this blends
		 * the top with the bottom, and the left with the right.
		 */
		if (ix == img->width - 1)
			xplus = 1 - img->width;
		else 
			xplus = 1;
		if (iy == img->height - 1)
			yplus = (1 - img->height) * img->width;
		else
			yplus = img->width;
		data = img->data;
		/* compute offset into first channel */
		offset = ix + iy * img->width;
		for (chan = 0; chan < img->totalchan; chan++) {
			x0y0 = (Float)data[offset] / 255.0;
			x1y0 = (Float)data[offset+xplus] / 255.0;
			x0y1 = (Float)data[offset+yplus] / 255.0;
			x1y1 = (Float)data[offset+xplus+yplus]/255.0;
			outval[chan] = (x0y0*(1.0-fx)*(1.0-fy) +
					x1y0*(fx)*(1.0-fy) +
					x0y1*(1.0-fx)*(fy) +  x1y1*(fx)*(fy));
			/* Make offset point to next channel */
			offset += img->chansize;
		}
	} else {
		/*
		 * Hard edged image pixels (rectangles)
		 * Compute offset into first channel
		 */
		offset = ix + iy * img->width;
		for (chan = 0; chan < img->totalchan; chan++) {
			outval[chan] = (Float)img->data[offset]/255.0;
			/* Make offset point to next channel */
			offset += img->chansize;
		}
	}
}

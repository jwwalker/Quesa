/*
 * picture.h
 *
 * Copyright (C) 1989, 1991, Craig E. Kolb
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
 * $Id: picture.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:47:07  kolb
 * Initial version.
 * 
 */
#ifndef PICTURE_H
#define PICTURE_H

#ifdef URT
#include "rle.h"
#include "rle_raw.h"
#endif

#define CORRECT(x)		(correct(GAMMACORRECT(x)))
#define GAMMACORRECT(x)		(Options.gamma == 1. ? (x) \
				   : (pow(x, Options.gamma)))
/*
 * Pixel
 */
typedef struct Pixel {
	Float r, g, b, alpha;		/* Red, green, blue, alpha channel */
} Pixel;

extern void	PictureStart(), PictureEnd(), PictureWriteLine();

#ifdef URT
extern void	PictureSetWindow();
#endif

#endif /* PICTURE_H */

/*
 * viewing.h
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
 * $Id: viewing.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:48:26  kolb
 * Initial version.
 * 
 */
#ifndef VIEWING_H
#define VIEWING_H

#include "picture.h"

/*
 * Screen
 */
typedef struct RSScreen {
	int	xres, yres,		/* Resolution of entire screen */
		xsize, ysize,		/* Resolution of window */
		minx, miny, maxx, maxy; /* Window to be rendered */
	Vector	scrnx, scrny,		/* Horizontal & vertical screen axes */
		scrni, scrnj,		/* Normalized versions of the above */
		firstray;		/* Direction from eye to screen UL  */
	Color	background;		/* Background color */
} RSScreen;

/*
 * Camera
 */
typedef struct {
	Vector	pos,			/* Eye position */
		lookp,			/* Look position */
		dir,			/* Look direction */
		up;			/* 'Up' vector */
	Float	hfov, vfov,		/* Horizontal/vertical field of view */
		lookdist,		/* Eye pos/look pos distance */
		aperture,		/* Aperture width (0 == pinhole) */
		focaldist;		/* Distance from eye to focal plane */
} RSCamera;

extern RSScreen Screen;
extern RSCamera Camera;

extern void ViewingSetup(void);
extern void RSViewing(void);
void SampleScreen(Float x, Float y, Ray *ray, Pixel *color,int sample);

#endif /* VIEWING_H */

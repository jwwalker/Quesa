/*
 * windy.c
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
 * $Id: windy.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:44:25  kolb
 * Initial version.
 * 
 */
#include "windy.h"
#include "textureutils.h"

/*
 * Create and return a reference to a "windy" texture.
 */
WindyText *
WindyCreate(
    Float       scale,
    Float       wscale,
    Float       cscale,
    Float       bscale,
    int         octaves,
    Float       tscale,
    Float       hscale,
    Float       offset)
{
	WindyText *windy;

	windy = (WindyText *)Malloc(sizeof(WindyText));
	windy->scale = scale;
	windy->windscale = wscale;
	windy->chaoscale = cscale;
	windy->bumpscale = bscale;
	windy->tscale = tscale;
	windy->hscale = hscale;
	windy->offset = offset;
	windy->octaves = octaves;
	return windy;
}

/*
 * Apply a "windy" texture.
 */
void
WindyApply(
    WindyText           *windy,
    Geom                * /*prim*/,
    Ray                 * /*ray*/,
    Vector              *pos,
    Vector              *norm,
    Vector              * /*gnorm*/,
    Surface             * /*surf*/)
{
	Vector bump;

	Windy(pos, windy->windscale, windy->chaoscale, windy->bumpscale,
	      windy->octaves, windy->tscale, windy->hscale, windy->offset,
	      &bump);

	norm->x += windy->scale * bump.x;
	norm->y += windy->scale * bump.y;
	norm->z += windy->scale * bump.z;
	VecNormalize(norm);
}

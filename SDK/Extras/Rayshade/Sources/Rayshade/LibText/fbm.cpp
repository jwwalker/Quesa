/*
 * fbm.c
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
 * $Id: fbm.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:42:06  kolb
 * Initial version.
 * 
 */
#include "fbm.h"
#include "textureutils.h"

FBm *
FBmCreate(
        Float           offset,
        Float           scale, 
        Float           h,
        Float           lambda,
        int             octaves,
        Float           thresh,
        char*           mapname)

{
	FBm *fbm;

	fbm = (FBm *)Malloc(sizeof(FBm));

	fbm->beta = 1. + 2*h;
	fbm->omega = pow(lambda, -0.5*fbm->beta);
	fbm->lambda = lambda;
	fbm->scale = scale;
	fbm->offset = offset;
	fbm->thresh = thresh;
	fbm->octaves = octaves;
	if (mapname != (char *)NULL)
		fbm->colormap = ColormapRead(mapname);
	else
		fbm->colormap = (Color *)NULL;
	return fbm;
}

static void
FBmApply(
    FBm             *fbm, 
    Geom            * /*prim*/,
    Ray             * /*ray*/,
    Vector          *pos,
    Vector          * /*norm*/,
    Vector          * /*gnorm*/,
    Surface         *surf)
{
	Float val;
	int index;

	val = fBm(pos, fbm->omega, fbm->lambda, fbm->octaves);
	if (val < fbm->thresh)
		val = fbm->offset;
	else
		val = fbm->offset + fbm->scale*(val - fbm->thresh);
	if (fbm->colormap) {
		index = (int)(255. * val);
		if (index > 255) index = 255;
		if (index < 0) index = 0;
		surf->diff.r *= fbm->colormap[index].r;
		surf->diff.g *= fbm->colormap[index].g;
		surf->diff.b *= fbm->colormap[index].b;
		surf->amb.r *= fbm->colormap[index].r;
		surf->amb.g *= fbm->colormap[index].g;
		surf->amb.b *= fbm->colormap[index].b;
	} else {
		ColorScale(val, surf->diff, &surf->diff);
		ColorScale(val, surf->amb, &surf->amb);
	}
}

/*
 * sky.c
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
 * $Id: sky.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:43:43  kolb
 * Initial version.
 * 
 */
#include "sky.h"
#include "libobj/geom.h"
#include "textureutils.h"

Sky *
SkyCreate(
        Float           scale,
        Float           h,
        Float           lambda,
        int             octaves,
        Float           cthresh,
        Float           lthresh)
{
	Sky *sky;

	sky = (Sky *)Malloc(sizeof(Sky));
	sky->beta = 1. + 2 * h;
	sky->omega = pow(lambda, -0.5 * sky->beta);
	sky->lambda = lambda;
	sky->scale = scale;
	sky->cthresh = cthresh;
	sky->lthresh = lthresh;
	sky->octaves = octaves;
	return sky;
}

void
SkyApply(
    Sky             *sky,
    Geom            * /*prim*/,
    Ray             * /*ray*/,
    Vector          *pos,
    Vector          * /*norm*/,
    Vector          * /*gnorm*/,
    Surface         *surf)
{
	Float It, maxval;

	It = fBm(pos, sky->omega, sky->lambda, sky->octaves);
	maxval = 1. / (1. - sky->omega);
	/*
	 * Map from [-maxval,maxval] to [0,1]
	 */
	It = (maxval +  It) * 0.5/maxval;

	It = (It - sky->lthresh) / (sky->cthresh - sky->lthresh);
	if (It < 0.)
		It = 0;
	else if (It > 1.)
		It = 1;

	if (sky->scale != 0.)
		It = pow(It, sky->scale);

	surf->transp = 1. - It;

	ColorScale(It, surf->diff, &surf->diff);
	ColorScale(It, surf->amb, &surf->amb);
}

/*
 * mist.c
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
 * $Id: mist.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:40:42  kolb
 * Initial version.
 * 
 */
#include "atmosphere.h"
#include "mist.h"

Mist *
MistCreate(Color *color,Color *trans,Float zero,Float scale)
{
	Mist *mist;

	mist = (Mist *)Malloc(sizeof(Mist));
	mist->color = *color;
	mist->trans = *trans;
	mist->zero = zero;
	mist->scale = 1. / scale;
	return mist;
}

/*
 * Add low-altitude mist to the given color.
 */
void
MistApply(
    Mist            *mist, 
    Ray             *ray, 
    Vector          *pos, 
    Float           dist, 
    Color           *color)

{
	Float deltaZ, d, atten;

	deltaZ = mist->scale * (pos->z - ray->pos.z);
	if (fabs(deltaZ) > EPSILON)
		d = (exp(-ray->pos.z*mist->scale + mist->zero) -
			    exp(-pos->z*mist->scale + mist->zero)) / deltaZ;
	else
		d = exp(-pos->z*mist->scale + mist->zero);

	dist *= d;

	atten = ExpAtten(dist, mist->trans.r);
	color->r = atten*color->r + (1. - atten)*mist->color.r;

	atten = ExpAtten(dist, mist->trans.g);
	color->g = atten*color->g + (1. - atten)*mist->color.g;

	atten = ExpAtten(dist, mist->trans.b);
	color->b = atten*color->b + (1. - atten)*mist->color.b;
}

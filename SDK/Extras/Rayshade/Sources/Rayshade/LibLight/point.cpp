/*
 * point.c
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
 * $Id: point.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:35:20  kolb
 * Initial version.
 * 
 */
#include "light.h"
#include "point.h"

static LightMethods *iPointMethods = NULL;

static int
PointIntens(LightRef lr, Color *lcolor,ShadowCache* cache,Ray* ray,Float dist,int noshadow,Color* color);
void
PointDirection(LightRef lr, Vector *pos,Vector* dir, Float* dist);

Pointlight *
PointCreate(Vector *pos)
{
	Pointlight *p;

	p = (Pointlight *)share_malloc(sizeof(Pointlight));
	p->pos = *pos;
	return p;
}

LightMethods *
PointMethods()
{
	if (iPointMethods == (LightMethods *)NULL) {
		iPointMethods = LightMethodsCreate();
		iPointMethods->intens = PointIntens;
		iPointMethods->dir = PointDirection;
	}
	return iPointMethods;
}

static int
PointIntens(LightRef lr, Color *lcolor,ShadowCache* cache,Ray* ray,Float dist,int noshadow,Color* color)
{
    Pointlight *lp = (Pointlight*)lr;
	return !Shadowed(color, lcolor, cache, ray, dist, noshadow);
}

void
PointDirection(LightRef lr, Vector *pos,Vector* dir, Float* dist)
{
    Pointlight *lp= (Pointlight*)lr;
	/*
	 * Calculate dir from position to center of
	 * light source.
	 */
	VecSub(lp->pos, *pos, dir);
	*dist = VecNormalize(dir);
}
static void
PointMethodRegister(UserMethodType meth)
{
	if (iPointMethods)
		iPointMethods->user = meth;
}

/*
 * infinite.c
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
 * $Id: infinite.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:34:28  kolb
 * Initial version.
 * 
 */
#include "light.h"
#include "infinite.h"

static LightMethods *iInfMethods = NULL;

static int InfiniteIntens(
                        LightRef        inf, 
                        Color           *lcolor, 
                        ShadowCache     *cache, 
                        Ray             *ray, 
                        Float           dist, 
                        int             noshadow,
                        Color           *color);
static void        InfiniteDirection(
                        LightRef        lr, 
                        Vector          *pos,
                        Vector          *dir,
                        Float           *dist);

Infinite *
InfiniteCreate(Vector *dir)
{
	Infinite *inf;

	inf = (Infinite *)share_malloc(sizeof(Infinite));
	inf->dir = *dir;
	if (VecNormalize(&inf->dir) == 0.) {
		RLerror(RL_ABORT, "Invalid directional light.\n");
		return (Infinite *)NULL;
	}
	return inf;
}

LightMethods *
InfiniteMethods(void)
{
	if (iInfMethods == (LightMethods *)NULL) {
		iInfMethods = LightMethodsCreate();
		iInfMethods->intens = InfiniteIntens;
		iInfMethods->dir = InfiniteDirection;
	}
	return iInfMethods;
}
static
int
InfiniteIntens(
        LightRef        /*inf*/, 
        Color           *lcolor, 
        ShadowCache     *cache, 
        Ray             *ray, 
        Float           dist, 
        int             noshadow,
        Color           *color)

{
	return !Shadowed(color, lcolor, cache, ray, dist, noshadow);
}
static 
void
InfiniteDirection(
    LightRef        lr, 
    Vector          * /*pos*/,
    Vector          *dir,
    Float           *dist)
{
    Infinite *lp = (Infinite*)lr;
	*dir = lp->dir;
	*dist = FAR_AWAY;
}

static void
InfiniteMethodRegister(UserMethodType meth)
{
	if (iInfMethods)
		iInfMethods->user = meth;
}

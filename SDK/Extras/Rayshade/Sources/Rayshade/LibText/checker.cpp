/*
 * checker.c
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
 * $Id: checker.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:41:48  kolb
 * Initial version.
 * 
 */
#include "checker.h"

/*
 * Create and return a reference to a "checker" texture.
 */
Checker *
CheckerCreate(Surface *surf)
{
	Checker *checker;
	checker = (Checker *)Malloc(sizeof(checker));
	checker->surf = surf;
	return checker;
}

/*
 * Apply a "checker" texture.
 */
static void
CheckerApply(
        Checker         *checker, 
        Geom            * /*prim*/, 
        Ray             * /*ray*/, 
        Vector          *pos,
        Vector          * /*norm*/,
        Vector          * /*gnorm*/,
        Surface         *surf)
{
	int xp, yp, zp;

	xp = (int)(pos->x > 0. ? pos->x : 1. - pos->x);
	yp = (int)(pos->y > 0. ? pos->y : 1. - pos->y);
	zp = (int)(pos->z > 0. ? pos->z : 1. - pos->z);

	if ((xp + yp + zp) % 2)
		*surf = *checker->surf;
	/* else surface stays the same. */
}

/*
 * bump.c
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
 * $Id: bump.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:41:39  kolb
 * Initial version.
 * 
 */
#include "bump.h"
#include "textureutils.h"

/*
 * Create and return a reference to a "bump" texture.
 */
Bump *
BumpCreate(Float size)
{
	Bump *bump;

	bump = (Bump *)Malloc(sizeof(Bump));
	bump->size = size;
	return bump;
}

/*
 * Apply a "bump" texture.
 */
void
BumpApply(
        Bump            *bump, 
        Geom            * /*prim*/, 
        Ray             * /*ray*/, 
        Vector          *pos,
        Vector          *norm,
        Vector          * /*gnorm*/,
        Surface         * /*surf*/)
{
	Vector disp;

	DNoise3(pos, &disp);
	norm->x += disp.x * bump->size;
	norm->y += disp.y * bump->size;
	norm->z += disp.z * bump->size;
	(void)VecNormalize(norm);
}

/*
 * fbmbump.c
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
 * $Id: fbmbump.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:42:21  kolb
 * Initial version.
 * 
 */
#include "fbm.h"
#include "fbmbump.h"
#include "textureutils.h"
#include "libobj/geom.h"

FBm *
FBmBumpCreate(
        Float       offset,
        Float       scale,
        Float       h,
        Float       lambda,
        int         octaves)

{
	FBm *fbm;
	fbm = FBmCreate(offset, scale, h, lambda, octaves, 0., (char *)NULL);
	return fbm;
}

/*ARGSUSED*/
static void
FBmBumpApply(
        FBm         *fbm,
        Geom        * /*prim*/,
        Ray         * /*ray*/,
        Vector      *pos,
        Vector      *norm,
        Vector      * /*gnorm*/, 
        Surface     * /*surf*/)
{
	Vector disp;

	VfBm(pos, fbm->omega, fbm->lambda, fbm->octaves, &disp);
	norm->x += fbm->offset + disp.x * fbm->scale;
	norm->y += fbm->offset + disp.y * fbm->scale;
	norm->z += fbm->offset + disp.z * fbm->scale;
}

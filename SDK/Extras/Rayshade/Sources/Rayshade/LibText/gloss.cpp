/*
 * gloss.c
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
 * $Id: gloss.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:42:30  kolb
 * Initial version.
 * 
 */
#include "gloss.h"
#include "libcommon/sampling.h"
#include "libobj/geom.h"

Gloss *
GlossCreate(Float glossiness)
{
	Gloss *gloss;

	gloss = (Gloss *)Malloc(sizeof(Gloss));
	gloss->glossy = 1. - glossiness;
	return gloss;
}

static void
GlossApply(
    Gloss       *gloss, 
    Geom        * /*prim*/, 
    Ray         *ray, 
    Vector      * /*pos*/,
    Vector      *norm,
    Vector      * /*gnorm*/,
    Surface     * /*surf*/)

{
	Vector uaxis, vaxis, point, norminc;

	/*
	 * Find coordinate system with norm as the Z axis.
	 */
	VecCoordSys(norm, &uaxis, &vaxis);
	/*
	 * Find point on unit circle based on sample #.
	 */
	UnitCirclePoint(&point, ray->sample);
	/*
	 * Perturb normal appropriately.
	 */
	VecComb(gloss->glossy * point.x, uaxis,
		gloss->glossy * point.y, vaxis,
		&norminc);
	VecAdd(*norm, norminc, norm);
	/*
	 * Renormalize.
	 */
	(void)VecNormalize(norm);
}

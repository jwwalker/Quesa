/*
 * blotch.c
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
 * $Id: blotch.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:41:31  kolb
 * Initial version.
 * 
 */
#include "blotch.h"
#include "textureutils.h"
/*
 * Create and return a reference to a "blotch" texture.
 */
Blotch *
BlotchCreate(Float mix, Surface *surf)
{
	Blotch *blotch;

	blotch = (Blotch *)Malloc(sizeof(Blotch));
	blotch->mix = mix;
	blotch->surf = surf;
	return blotch;
}

/*
 * Apply "blotch" texture.
 */
/*ARGSUSED*/
void
BlotchApply(
    Blotch      *blotch, 
    Geom        * /*prim*/, 
    Ray         * /*ray*/, 
    Vector      *pos,
    Vector      * /*norm*/,
    Vector      * /*gnorm*/,
    Surface     *surf)
{
	Float val;

	/*
	 * "mix" represents the 'average' noise value at a point.
	 */
	val = Noise3(pos);
	if (val > blotch->mix) {
		val = (val - blotch->mix) / (1. - blotch->mix);
		SurfaceBlend(surf, blotch->surf, 1. - val, val);
	}
}

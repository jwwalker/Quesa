/*
 * marble.c
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
 * $Id: marble.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:43:06  kolb
 * Initial version.
 * 
 */
#include "marble.h"
#include "textureutils.h"

MarbleText *
MarbleCreate(char *mapname)
{
	MarbleText *marble;

	marble = (MarbleText *)Malloc(sizeof(MarbleText));
	if (mapname)
		marble->colormap = ColormapRead(mapname);
	else
		marble->colormap = (Color *)NULL;
	return marble;
}

void
MarbleApply(
        MarbleText          *marble, 
        Geom                * /*prim*/, 
        Ray                 * /*ray*/, 
        Vector              *pos,
        Vector              * /*norm*/,
        Vector              * /*gnorm*/,
        Surface             *surf)

{
	Float val;
	int index;

	val = Marble(pos);
	if (marble->colormap) {
		index = (int)(255. * val);
		surf->diff.r *= marble->colormap[index].r;
		surf->diff.g *= marble->colormap[index].g;
		surf->diff.b *= marble->colormap[index].b;
		surf->amb.r *= marble->colormap[index].r;
		surf->amb.g *= marble->colormap[index].g;
		surf->amb.b *= marble->colormap[index].b;
	} else {
		ColorScale(val, surf->amb, &surf->amb);
		ColorScale(val, surf->diff, &surf->diff);
	}
}

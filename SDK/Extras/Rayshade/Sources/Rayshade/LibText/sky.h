/*
 * sky.h
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
 * $Id: sky.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:43:46  kolb
 * Initial version.
 * 
 */
#ifndef SKY_H

#include "libcommon/common.h"
#include "libobj/geom.h"
#include "libsurf/surface.h"

#define TextSkyCreate(s,h,l,n,c,t) TextCreate((TextRef)SkyCreate(s,h,l,n,c,t),\
					SkyApply)
typedef struct {
	Float	beta,
		omega,
		lambda,
		scale,
		cthresh,
		lthresh;
	int	octaves;
} Sky;

extern Sky *
SkyCreate(
        Float           scale,
        Float           h,
        Float           lambda,
        int             octaves,
        Float           cthresh,
        Float           lthresh);
extern void
SkyApply(
    Sky             *sky,
    Geom            *prim,
    Ray             *ray,
    Vector          *pos,
    Vector          *norm,
    Vector          *gnorm,
    Surface         *surf);

#endif /* SKY_H */

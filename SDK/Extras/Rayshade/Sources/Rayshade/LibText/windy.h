/*
 * windy.h
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
 * $Id: windy.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:44:28  kolb
 * Initial version.
 * 
 */
#ifndef WINDY_H

#include "libcommon/common.h"
#include "libobj/geom.h"

#define TextWindyCreate(s,w,c,b,o,t,h,i) TextCreate( \
		(TextRef)WindyCreate(s,w,c,b,o,t,h,i), WindyApply)

typedef struct {
	Float	scale,
		windscale,
		chaoscale,
		bumpscale,
		tscale,
		hscale,
		offset;
	int	octaves;
} WindyText;

extern void WindyApply();

extern WindyText *      WindyCreate(
                            Float               scale,
                            Float               wscale,
                            Float               cscale,
                            Float               bscale,
                            int                 octaves,
                            Float               tscale,
                            Float               hscale,
                            Float               offset);
extern void             WindyApply(
                            WindyText           *windy,
                            Geom                *prim,
                            Ray                 *ray,
                            Vector              *pos,
                            Vector              *norm,
                            Vector              *gnorm,
                            Surface             *surf);


#endif /* WINDY_H */

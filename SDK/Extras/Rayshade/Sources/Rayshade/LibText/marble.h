/*
 * marble.h
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
 * $Id: marble.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:43:10  kolb
 * Initial version.
 * 
 */
#ifndef MARBLE_H

#include "libcommon/common.h"
#include "libobj/geom.h"
#include "libsurf/surface.h"

#define TextMarbleCreate(m)	TextCreate((TextRef) MarbleCreate(m), \
					MarbleApply)
typedef struct {
	Color *colormap;	/* colormap */
} MarbleText;

extern MarbleText *MarbleCreate(char *mapname);
extern void MarbleApply(MarbleText          *marble, 
        				Geom                *prim, 
        				Ray                 *ray, 
        				Vector              *pos,
        				Vector              *norm,
        				Vector              *gnorm,
        				Surface             *surf);

#endif /* MARBLE_H */

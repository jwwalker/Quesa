/*
 * disc.h
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
 * $Id: disc.h,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:37:31  kolb
 * Initial version.
 * 
 */
#ifndef DISC_H
#define DISC_H

#define GeomDiscCreate(r,p,n)	GeomCreate((GeomRef)DiscCreate(r,p,n), \
					DiscMethods())

typedef struct disc {
	Float radius, d;		/* Radius squared, plane constant */
	Vector norm, pos;		/* Normal, center */
} Disc;

extern Disc *
DiscCreate(
        Float           r, 
        Vector          *pos,
        Vector          *norm);
extern Methods	*DiscMethods();

#endif /* DISC_H */

/*
 * cone.h
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
 * $Id: cone.h,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:36:49  kolb
 * Initial version.
 * 
 */
#ifndef CONE_H
#define CONE_H

#define GeomConeCreate(c,a,r1,r2) GeomCreate((GeomRef)ConeCreate(c,a,r1,r2), \
					ConeMethods())

/*
 * Cone
 */
typedef struct cone {
	Float start_dist;
	Trans trans;
} Cone;

extern Cone *
ConeCreate(Float br, Vector *bot, Float ar, Vector *apex);
extern Methods	*ConeMethods();

#endif /* CONE_H */

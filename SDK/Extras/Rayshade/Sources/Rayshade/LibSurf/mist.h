/*
 * mist.h
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
 * $Id: mist.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:40:46  kolb
 * Initial version.
 * 
 */
#ifndef MIST_H

#define AtmosMistCreate(c,t,z,s) AtmosCreate((AtmosRef)MistCreate(c,t,z,s), \
						MistApply)
typedef struct Mist {
	Color color;		/* Mist color */
	Color trans;		/* R, G, B transmissivity */
	Float scale, zero;	/* Height scale, start Z */
} Mist;

extern Mist *MistCreate(Color *color,Color *trans,Float zero,Float scale);
extern void MistApply(Mist *mist, Ray *ray, Vector *pos, Float dist, Color *color);

#endif /* MIST_H */


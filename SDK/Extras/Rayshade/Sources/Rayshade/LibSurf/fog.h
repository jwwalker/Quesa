/*
 * fog.h
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
 * $Id: fog.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:40:22  kolb
 * Initial version.
 * 
 */
#ifndef FOG_H

#define AtmosFogCreate(c,t)	AtmosCreate((AtmosRef) FogCreate(c,t), \
						FogApply)	
typedef struct Fog {
	Color	color,		/* Fog color */
		trans;		/* Fog transmissivity */
} Fog;

extern Fog *FogCreate(Color  *color, Color *trans);
extern void FogApply(Fog *fog, Ray *ray, Vector *pos, Float dist, Color *color);

#endif /* FOG_H */

/*
 * mapping.h
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
 * $Id: mapping.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:43:00  kolb
 * Initial version.
 * 
 */
#ifndef MAPPING_H
#define MAPPING_H

#define PRIMSPACE	01	/* Mapping uses data in prim space */
#define OBJSPACE	02	/* Mapping uses data in object space */

typedef struct Mapping Mapping;
typedef void	(*TMappingMethod)   (Mapping             *map,
                                     Geom                *obj,
                                     Vector              *pos,
                                     Vector              *norm,
                                     Vec2d               *uv,
                                     Vector              *dpdu,
                                     Vector              *dpdv);

typedef struct Mapping {
	int flags;		/* mapping flags */
	Vector center, uaxis, vaxis, norm;
	Float dist;
	RSMatrix m;
	TMappingMethod  method;
} Mapping;
extern Mapping *UVMappingCreate();
extern Mapping *SphereMappingCreate(Vector *center, Vector *norm, Vector *uaxis);
extern Mapping *CylMappingCreate(Vector *center,Vector* norm,Vector* uaxis);
extern Mapping *LinearMappingCreate(Vector *center, Vector *vaxis, Vector *uaxis);
#endif /* MAPPING_H */

/*
 * csg.h
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
 * $Id: csg.h,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:37:06  kolb
 * Initial version.
 * 
 */
#ifndef CSG_H
#define CSG_H

#define GeomCsgCreate(t)	GeomCreate((GeomRef)CsgCreate(t), CsgMethods())

/*
 * CSG (Constructive Solid Geometry)
 */
#define CSG_UNION	0
#define CSG_INTERSECT	1
#define CSG_DIFFERENCE	2


typedef int (*TCsg_IntersetionMethod)(Ray *ray,HitList *hit1p,HitList *hit2p,Float dist1,Float dist2,
                                      HitList **hitclose, Float *distclose);
typedef struct Csg {
	char csgoperator;			/* Union, Intersect, or Difference */
	struct Geom *obj1, *obj2;
	TCsg_IntersetionMethod intmeth;		/* Intersection method. */
	Float bounds[2][3];
} Csg;

extern char	*CsgName();
extern GeomRef  CsgCreate(int op);
extern int      CsgConvert(GeomRef csg, Geom *list);
extern int      CsgIntersect(GeomRef gref, Ray *ray, HitList *hitlist,Float mindist,Float* maxdist);

extern int	    FirstCSGGeom(HitList *hitlist);
extern Methods	*CsgMethods();

#endif /* CSG_H */

/*
 * poly.h
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
 * $Id: poly.h,v 1.2 2004-11-30 01:10:50 jwwalker Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.1  2002/12/18 18:36:42  pepe
 * First upload
 *
 * Revision 4.0  91/07/17  14:39:04  kolb
 * Initial version.
 * 
 */
#ifndef POLY_H
#define POLY_H

#define GeomPolygonCreate(r,p,f) GeomCreate((GeomRef)PolygonCreate(r,p,f), \
					PolygonMethods())

namespace RayShade	// avoid name conflict in Windows
{
/*
 * Polygon
 */
typedef struct {
	Vector norm;		/* Normal to polygon */
	Float d;		/* Plane constant */
	char index;		/* Which normal coord is "dominant"? */
	Vector *points;		/* Array of vertices */
	int npoints;		/* Number of vertices */
} Polygon;

}

extern RayShade::Polygon *PolygonCreate(PointList *plist,int npoints,int flipflag);
extern Methods *PolygonMethods();

#endif /* POLY_H */

/*
 * poly.c
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
 * $Id: poly.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0.1.1  91/11/26  21:25:34  cek
 * patch3: Additional check for degenerate polygon.
 * 
 * Revision 4.0  91/07/17  14:39:00  kolb
 * Initial version.
 * 
 */
#include "geom.h"
#include "poly.h"

static Methods *iPolygonMethods = NULL;
static char polyName[] = "polygon";

unsigned long PolyTests, PolyHits;

/*
 * Create a reference to a polygon with vertices equal to those
 * on the linked-list "plist."
 */
Polygon *
PolygonCreate(PointList *plist,int npoints,int flipflag)
{
	Polygon *poly;
	Float indexval;
	Vector *prev, *cur, anorm;
	PointList *curp, *pltmp;
	int i;

	if (npoints < 3) {
		RLerror(RL_WARN, "Degenerate polygon.\n");
		return (Polygon *)NULL;
	}
	
	poly = (Polygon *)share_malloc(sizeof(Polygon));
	/*
	 * Allocate space for the vertices.
	 */
	poly->points = (Vector *)Malloc((unsigned)(npoints*sizeof(Vector)));
	poly->npoints = npoints;

	/*
	 * Copy the vertices from the linked list to the array, freeing
	 * the linked list as we go so that the caller doesn't have
	 * to worry about doing so.
	 */
	i = npoints -1;
	for(curp = plist; curp != (PointList *)0; curp = pltmp) {
		poly->points[i--] = curp->vec;
		pltmp = curp->next;
		free((voidstar)curp);
	}

	/*
	 * Find normal to polygon.
	 */
	poly->norm.x = poly->norm.y = poly->norm.z = 0.;
	prev = &poly->points[poly->npoints -1];
	for(i = 0,cur = poly->points;i < poly->npoints;i++, prev = cur, cur++) {
		poly->norm.x += (prev->y - cur->y) * (prev->z + cur->z);
		poly->norm.y += (prev->z - cur->z) * (prev->x + cur->x);
		poly->norm.z += (prev->x - cur->x) * (prev->y + cur->y);
	}

	if (VecNormalize(&poly->norm) == 0.) {
		/*
		 * Degenerate normal --> degenerate polygon
		 */
		RLerror(RL_WARN, "Degenerate polygon.\n");
		free((voidstar)poly->points);
		return (Polygon *)NULL;
	}

	/*
	 * If filflag is true, flip the normal.
	 */
	if (flipflag)
		VecScale(-1, poly->norm, &poly->norm);

	/*
	 * Compute and store the plane constant.
	 */
	poly->d = dotp(&poly->norm, &poly->points[0]);

	/*
	 * Find the "dominant" part of the normal vector.  This
	 * is used to turn the point-in-polygon test into a 2D problem.
	 */
	anorm.x = fabs(poly->norm.x);
	anorm.y = fabs(poly->norm.y);
	anorm.z = fabs(poly->norm.z);
	indexval = max(anorm.y, anorm.z);
	indexval = max(anorm.x, indexval);

	if (indexval == anorm.x)
		poly->index = XNORMAL;
	else if (indexval == anorm.y)
		poly->index = YNORMAL;
	else
		poly->index = ZNORMAL;

	return poly;
}



/*
 * Quadrants are defined as:
 *        |
 *   1    |   0
 *        |
 * -------c--------
 *        |
 *   2    |   3
 *        |
 */
#define quadrant(p, c) ((p.u<c.u) ? ((p.v<c.v) ? 2 : 1) : ((p.v<c.v) ? 3 : 0))

/*
 * Perform ray-polygon intersection test.
 */
static int
PolygonIntersect(GeomRef gref,Ray* ray,Float mindist,Float* maxdist)
{
	Polygon *poly = (Polygon*)gref;

	register int winding, i;
	Vector dir, pos;
	int quad, lastquad;
	Float dist, left, right;
	Vec2d center, cur, last;

	PolyTests++;
	pos = ray->pos;
	dir = ray->dir;
	/*
	 * First, find where ray hits polygon plane, projecting
	 * along the polygon's dominant normal component.
	 */

	dist = dotp(&poly->norm, &dir);
	if(fabs(dist) < EPSILON)
		/*
	 	 * No intersection with polygon plane.
		 */
		return FALSE;

	dist = (poly->d - dotp(&poly->norm, &pos)) / dist;
	if(dist < mindist || dist > *maxdist)
		/*
		 * Intersection point behind origin or too far.
		 */
		return FALSE;

	/*
	 * Compute the point of intersection, projected appropriately.
	 */
	if(poly->index == XNORMAL) {
		center.u = pos.y + dist * dir.y;
		center.v = pos.z + dist * dir.z;
	} else if(poly->index == YNORMAL) {
		center.v = pos.z + dist * dir.z;
		center.u = pos.x + dist * dir.x;
	} else  {
		center.u = pos.x + dist * dir.x;
		center.v = pos.y + dist * dir.y;
	}	

	/*
	 * Is the point inside the polygon?
	 *
	 * Compute the winding number by finding the quadrant each
	 * polygon point lies in with respect to the the point in
	 * question, and computing a "delta" (winding number).  If we
	 * end up going around in a complete circle around
	 * the point (winding number is non-zero at the end), then
	 * we're inside.  Otherwise, the point is outside.
	 *
	 * Note that we can turn this into a 2D problem by projecting
	 * all the points along the axis defined by poly->index,
	 * the "dominant" part of the polygon's normal vector.
	 */
	winding = 0;
	VecProject(last, poly->points[poly->npoints -1], poly->index);
	lastquad = quadrant(last, center);
	for(i = 0; i < poly->npoints; i++, last = cur) {
		VecProject(cur, poly->points[i], poly->index);
		quad = quadrant(cur, center);
		if (quad == lastquad)
			continue;
		if(((lastquad + 1) & 3) == quad)
			winding++;
		else if(((quad + 1) & 3) == lastquad)
			winding--;
		else {
			/*
			 * Find where edge crosses
			 * center's X axis.
			 */
			right = last.u - cur.u;
			left = (last.v - cur.v) * (center.u - last.u);
			if(left + last.v * right > right * center.v)
				winding += 2;
			else
				winding -= 2;
		}
		lastquad = quad;
	}

	if (winding != 0) {
		*maxdist = dist;
		PolyHits++;
		return TRUE;
	}
	return FALSE;
}

/*
 * Return the normal to the polygon surface.
 */
/*ARGSUSED*/
static int
PolygonNormal(GeomRef gref,Vector* /*pos*/,Vector* nrm,Vector* gnrm)
{
	Polygon *poly = (Polygon*)gref;
	*gnrm = *nrm = poly->norm;
	return FALSE;
}

/*ARGSUSED*/
static void
PolygonUV(GeomRef gref,Vector* pos,Vector* /*norm*/,Vec2d* uv,Vector* dpdu,Vector* dpdv)
{
	Polygon *poly = (Polygon*)gref;
	/*
	 * Since there's no nice way to do this, we wimp out and
	 * do the following...
	 *
	 * Of course, we could force the user to specify U and V
	 * axes, but forcing them to use X and Y as U and V is
	 * just as arbitrary and much simpler to deal with.
	 */
	uv->u = pos->x;
	uv->v = pos->y;
	if (dpdu) {
		dpdu->x = 1.;
		dpdu->y = dpdu->z = 0.;
		dpdv->x = dpdv->z = 0.;
		dpdv->y = 1.;
	}
}

/*
 * Compute the extent of a polygon
 */
static void
PolygonBounds(GeomRef gref, Float bounds[2][3])
{
	Polygon *poly = (Polygon*)gref;
	register int i;

	bounds[LOW][X] = bounds[HIGH][X] = poly->points[0].x;
	bounds[LOW][Y] = bounds[HIGH][Y] = poly->points[0].y;
	bounds[LOW][Z] = bounds[HIGH][Z] = poly->points[0].z;

	for (i = 1 ;i < poly->npoints; i++) {
		if (poly->points[i].x < bounds[LOW][X])
			bounds[LOW][X] = poly->points[i].x;
		if (poly->points[i].x > bounds[HIGH][X])
			bounds[HIGH][X] = poly->points[i].x;
		if (poly->points[i].y < bounds[LOW][Y])
			bounds[LOW][Y] = poly->points[i].y;
		if (poly->points[i].y > bounds[HIGH][Y])
			bounds[HIGH][Y] = poly->points[i].y;
		if (poly->points[i].z < bounds[LOW][Z])
			bounds[LOW][Z] = poly->points[i].z;
		if (poly->points[i].z > bounds[HIGH][Z])
			bounds[HIGH][Z] = poly->points[i].z;
	}
}

static char *
PolygonName()
{
	return polyName;
}

static void
PolygonStats(unsigned long *tests,unsigned long * hits)
{
	*tests = PolyTests;
	*hits = PolyHits;
}

Methods *
PolygonMethods()
{
	if (iPolygonMethods == (Methods *)NULL) {
		iPolygonMethods = MethodsCreate();
		iPolygonMethods->create = (TGeomMethod_Create)PolygonCreate;
		iPolygonMethods->methods = PolygonMethods;
		iPolygonMethods->name = PolygonName;
		iPolygonMethods->intersect.normal = PolygonIntersect;
		iPolygonMethods->normal = PolygonNormal;
		iPolygonMethods->uv = PolygonUV;
		iPolygonMethods->bounds = PolygonBounds;
		iPolygonMethods->stats = PolygonStats;
		iPolygonMethods->checkbounds = TRUE;
		iPolygonMethods->closed = FALSE;
	}
	return iPolygonMethods;
}

static void
PolygonMethodRegister(UserMethodType meth)
{
	if (iPolygonMethods)
		iPolygonMethods->user = meth;
}



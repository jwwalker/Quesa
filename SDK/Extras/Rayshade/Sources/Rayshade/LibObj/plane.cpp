/*
 * plane.c
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
 * $Id: plane.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:38:51  kolb
 * Initial version.
 * 
 */
#include "geom.h"
#include "plane.h"

static Methods *iPlaneMethods = NULL;
static char planeName[] = "plane";

unsigned long PlaneTests, PlaneHits;

/*
 * create plane primitive
 */
Plane *
PlaneCreate(Vector *pos,Vector *norm)
{
	Plane *plane;
	Vector tmpnrm;

	tmpnrm = *norm;
	if (VecNormalize(&tmpnrm) == 0.) {
		RLerror(RL_WARN, "Degenerate plane normal.\n");
		return (Plane *)NULL;
	}
	plane = (Plane *)share_malloc(sizeof(Plane));
	plane->norm = tmpnrm;
	plane->pos = *pos;
	plane->d = dotp(&plane->norm, pos);

	return plane;
}



static int
PlaneIntersect(GeomRef gref, Ray *ray, Float mindist,Float* maxdist)
{
	Plane *plane = (Plane*)gref;

	Float d;

	PlaneTests++;

	d = dotp(&plane->norm, &ray->dir);
	if (fabs(d) < EPSILON)
		return FALSE;
	d = (plane->d - dotp(&plane->norm, &ray->pos)) / d;

	if (d > mindist && d < *maxdist) {
		*maxdist = d;
		PlaneHits++;
		return TRUE;
	}
	return FALSE;
}

/*ARGSUSED*/
static int
PlaneNormal(GeomRef gref, Vector * /*pos*/,Vector * nrm,Vector * gnrm)
{
	Plane *plane = (Plane*)gref;

	*gnrm = *nrm = plane->norm;
	return FALSE;
}

static void
PlaneUV(GeomRef gref, Vector *pos,Vector *norm, Vec2d *uv,Vector *dpdu,Vector *dpdv)
{
	Plane *plane = (Plane*)gref;

	Vector vec, du, dv;

	VecCoordSys(norm, &du, &dv);
	VecSub(*pos, plane->pos, &vec);

	uv->u = dotp(&vec, &du);
	uv->v = dotp(&vec, &dv);

	if (dpdu)
		*dpdu = du;
	if (dpdv)
		*dpdv = dv;
}
	
/*ARGSUSED*/
static void
PlaneBounds(GeomRef gref, Float bounds[2][3])
{
	Plane *plane = (Plane*)gref;
	/*
	 * Planes are unbounded by nature.  minx > maxx signifies
	 * this.
	 */
	bounds[LOW][X] = 1.0;
	bounds[HIGH][X] = -1.0;
}

static char *
PlaneName()
{
	return planeName;
}

static void
PlaneStats(unsigned long *tests,unsigned long * hits)
{
	*tests = PlaneTests;
	*hits = PlaneHits;
}

Methods *
PlaneMethods()
{
	if (iPlaneMethods == (Methods *)NULL) {
		iPlaneMethods = MethodsCreate();
		iPlaneMethods->name = PlaneName;
		iPlaneMethods->create = (TGeomMethod_Create)PlaneCreate;
		iPlaneMethods->methods = PlaneMethods;
		iPlaneMethods->intersect.normal = PlaneIntersect;
		iPlaneMethods->normal = PlaneNormal;
		iPlaneMethods->uv = PlaneUV;
		iPlaneMethods->bounds = PlaneBounds;
		iPlaneMethods->stats = PlaneStats;
		iPlaneMethods->checkbounds = FALSE;
		iPlaneMethods->closed = FALSE;
	}
	return iPlaneMethods;
}

static void
PlaneMethodRegister(UserMethodType meth)
{
	if (iPlaneMethods)
		iPlaneMethods->user = meth;
}

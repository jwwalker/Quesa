/*
 * box.c
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
 * $Id: box.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:36:32  kolb
 * Initial version.
 * 
 */
#include "geom.h"
#include "box.h"

static Methods *iBoxMethods = NULL;
static char boxName[] = "box";

unsigned long BoxTests, BoxHits;

Box *
BoxCreate(Vector *v1,Vector* v2)
{
	Box *box;
	Vector size;

	VecSub(*v1, *v2, &size);

	if (equal(size.x, 0.) || equal(size.y, 0.) || equal(size.z, 0.)) {
		RLerror(RL_WARN, "Degenerate box.\n");
		return (Box *)NULL;
	}

	box = (Box *)share_malloc(sizeof(Box));
	box->bounds[LOW][X] = min(v1->x, v2->x);
	box->bounds[HIGH][X] = max(v1->x, v2->x);
	box->bounds[LOW][Y] = min(v1->y, v2->y);
	box->bounds[HIGH][Y] = max(v1->y, v2->y);
	box->bounds[LOW][Z] = min(v1->z, v2->z);
	box->bounds[HIGH][Z] = max(v1->z, v2->z);
	return box;
}


static int
BoxIntersect(GeomRef gref, Ray *ray,Float mindist,Float* maxdist)

{
	Box *box = (Box*)gref;

	BoxTests++;
	if (BoundsIntersect(ray, box->bounds, mindist, maxdist)) {
		BoxHits++;
		return TRUE;
	}
	return FALSE;
}

static int
BoxNormal(GeomRef gref,Vector* pos,Vector* nrm,Vector* gnrm)
{
	Box *box = (Box*)gref;
	nrm->x = nrm->y = nrm->z = 0.;

	if (equal(pos->x, box->bounds[HIGH][X]))
		nrm->x = 1.;
	else if (equal(pos->x, box->bounds[LOW][X]))
		nrm->x = -1.;
	else if (equal(pos->y, box->bounds[HIGH][Y]))
		nrm->y = 1.;
	else if (equal(pos->y, box->bounds[LOW][Y]))
		nrm->y = -1.;
	else if (equal(pos->z, box->bounds[HIGH][Z]))
		nrm->z = 1.;
	else if (equal(pos->z, box->bounds[LOW][Z]))
		nrm->z = -1.;
	else
		RLerror(RL_WARN, "Confusion in nrmbox!\n");
	*gnrm = *nrm;
	return FALSE;
}

/*
 * Determine if ray enters (TRUE) or leaves (FALSE) box at pos
 */
static int
BoxEnter(GeomRef gref, Ray *ray,Float mind,Float /*hitd*/)
{
	Box *box = (Box*)gref;
	Vector pos;

	VecAddScaled(ray->pos, mind, ray->dir, &pos);
	return OutOfBounds(&pos, box->bounds);
}

static void
BoxBounds(GeomRef gref,Float bounds[2][3])
{
	Box *box = (Box*)gref;
	BoundsCopy(box->bounds, bounds);
}

static char *
BoxName()
{
	return boxName;
}

static void
BoxStats(unsigned long *tests,unsigned long * hits)
{
	*tests = BoxTests;
	*hits = BoxHits;
}
Methods *
BoxMethods()
{
	if (iBoxMethods == (Methods *)NULL) {
		iBoxMethods = MethodsCreate();
		iBoxMethods->create = (GeomCreateFunc *)BoxCreate;
		iBoxMethods->methods = BoxMethods;
		iBoxMethods->name = BoxName;
		iBoxMethods->intersect.normal = BoxIntersect;
		iBoxMethods->normal = BoxNormal;
		iBoxMethods->enter = BoxEnter;
		iBoxMethods->bounds = BoxBounds;
		iBoxMethods->stats = BoxStats;
		iBoxMethods->checkbounds = FALSE;
		iBoxMethods->closed = TRUE;
	}
	return iBoxMethods;
}
static void
BoxMethodRegister(UserMethodType meth)

{
	if (iBoxMethods)
		iBoxMethods->user = meth;
}

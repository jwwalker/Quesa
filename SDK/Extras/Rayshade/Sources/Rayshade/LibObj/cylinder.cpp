/*
 * cylinder.c
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
 * $Id: cylinder.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:37:12  kolb
 * Initial version.
 * 
 */
#include "geom.h"
#include "cylinder.h"

static Methods *iCylinderMethods = NULL;
static char cylName[] = "cylinder";

unsigned long CylTests, CylHits;

Cylinder *
CylinderCreate(Float r, Vector *bot,Vector* top)
{
	Cylinder *cyl;
	Float len;
	Vector axis;

	if (r <= 0.) {
		RLerror(RL_WARN, "Invalid cylinder radius.\n");
		return (Cylinder*)NULL;
	}

	VecSub(*top, *bot, &axis);

	len = VecNormalize(&axis);

	if (len < EPSILON) {
		RLerror(RL_WARN, "Degenerate cylinder.\n");
		return (Cylinder *)NULL;
	}

	cyl = (Cylinder *)share_malloc(sizeof(Cylinder));
	CoordSysTransform(bot, &axis, r, len, &cyl->trans);
	return cyl;
}



/*
 * Ray-cylinder intersection test.
 */
static int
CylinderIntersect(GeomRef gref,Ray* ray,Float mindist,Float* maxdist)
{
	Cylinder *cyl = (Cylinder*)gref;

	Float t1, t2, a, b, c, zpos1, zpos2, disc;
	Float distfact;
	Ray newray;
	Vector nray, npos;
	Float nmin;

	CylTests++;

	/*
	 * Transform ray into canonical cylinder space.
	 */
	newray = *ray;
	distfact = RayTransform(&newray, &cyl->trans.itrans);
	nray = newray.dir;
	npos = newray.pos;
	nmin = mindist * distfact;

	a = nray.x * nray.x + nray.y * nray.y;
	if (a < EPSILON*EPSILON)
		/* |nray.z| == 1. */
		return FALSE;

	b = nray.x * npos.x + nray.y * npos.y;
	c = npos.x*npos.x + npos.y*npos.y - 1;
	disc = b*b - a*c;
	if(disc < 0.)
		return FALSE;
	disc = sqrt(disc);
	t1 = (-b + disc) / a;
	t2 = (-b - disc) / a;
	if (t1 < nmin && t2 < nmin)
		return FALSE;
	zpos1 = npos.z + t1 * nray.z;
	zpos2 = npos.z + t2 * nray.z;

	if (t1 < nmin || zpos1 < 0. || zpos1 > 1.) {
		if (t2 < nmin || zpos2 < 0. || zpos2 > 1.)
			return FALSE;
		else
			t1 = t2 / distfact;

	} else {
		if (t2 < nmin || zpos2 < 0. || zpos2 > 1.)
			t1 /= distfact;
		else {
			t1 = min(t1, t2) / distfact;
		}
	}
	
	if (t1 < *maxdist) {
		*maxdist = t1;
		CylHits++;
		return TRUE;
	}
	return FALSE;
}

static int
CylinderNormal(GeomRef gref,Vector* pos,Vector* nrm,Vector* gnrm)
{
	Cylinder *cyl = (Cylinder*)gref;

	/*
	 * Transform position into cylinder space.
	 */
	*nrm = *pos;
	PointTransform(nrm, &cyl->trans.itrans);
	/*
	 * The normal is equal to the point of intersection in cylinder
	 * space, but with Z = 0.;
	 */
	nrm->z = 0.;

	/*
	 * Tranform normal back to world space.
	 */
	NormalTransform(nrm, &cyl->trans.itrans);
	*gnrm = *nrm;
	return FALSE;
}

static void
CylinderUV(GeomRef gref, Vector *pos,Vector * /*norm*/, Vec2d *uv,Vector *dpdu,Vector *dpdv)
{
	Cylinder *cyl = (Cylinder*)gref;

	Vector npos;

	npos = *pos;
	PointTransform(&npos, &cyl->trans.itrans);

	uv->v = npos.z;
	/*
	 * Due to roundoff error, |npos.x| may be > 1.
	 */
	if (npos.x > 1.)
		uv->u = 0.;
	else if (npos.x < -1.)
		uv->u = 0.5;
	else
		uv->u = acos(npos.x) / TWOPI;
	if (npos.y < 0.)
		uv->u = 1. - uv->u;

	if (dpdu) {
		dpdv->x = dpdv->y = 0.;
		dpdv->z = 1.;
		dpdu->x = -npos.y;
		dpdu->y = npos.x;
		dpdu->z = 0.;
		VecTransform(dpdu, &cyl->trans.trans);
		VecTransform(dpdv, &cyl->trans.trans);
		(void)VecNormalize(dpdu);
		(void)VecNormalize(dpdv);
	}
}

static void
CylinderBounds(GeomRef gref,Float bounds[2][3])

{
	Cylinder *cyl = (Cylinder*)gref;

	bounds[LOW][X] = bounds[LOW][Y] = -1;
	bounds[HIGH][X] = bounds[HIGH][Y] = 1;
	bounds[LOW][Z] = 0.;
	bounds[HIGH][Z] = 1;
	/*
	 * Transform bounding box to world space.
	 */
	BoundsTransform(&cyl->trans.trans, bounds);
}

static char *
CylinderName()
{
	return cylName;
}

static void
CylinderStats(unsigned long *tests,unsigned long * hits)
{
	*tests = CylTests;
	*hits = CylHits;
}
Methods *
CylinderMethods()
{
	if (iCylinderMethods == (Methods *)NULL) {
		iCylinderMethods = MethodsCreate();
		iCylinderMethods->name = CylinderName;
		iCylinderMethods->create = (GeomCreateFunc *)CylinderCreate;
		iCylinderMethods->methods = CylinderMethods;
		iCylinderMethods->intersect.normal = CylinderIntersect;
		iCylinderMethods->normal = CylinderNormal;
		iCylinderMethods->uv = CylinderUV;
		iCylinderMethods->bounds = CylinderBounds;
		iCylinderMethods->stats = CylinderStats;
		iCylinderMethods->checkbounds = TRUE;
		iCylinderMethods->closed = FALSE;
	}
	return iCylinderMethods;
}
static void
CylinderMethodRegister(UserMethodType meth)
{
	if (iCylinderMethods)
		iCylinderMethods->user = meth;
}

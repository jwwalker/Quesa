/*
 * csg.c
 *
 * Copyright (C) 1991, Rod G. Bogart, Craig E. Kolb
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
 * $Id: csg.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:37:00  kolb
 * Initial version.
 * 
 */
#include "geom.h"
#include "csg.h"

#define csg_set_enter(l, f) ((l)->data[0].enter = (f) + 1)

static Methods *iCsgMethods = NULL;
static char csgName[] = "csg";

static int
CsgUnionInt(Ray *ray, HitList *hit1p,HitList* hit2p,Float dist1,Float dist2,HitList** hitclose,Float* distclose);

static int
CsgDifferenceInt(Ray *ray, HitList *hit1p,HitList* hit2p,Float dist1,Float dist2,HitList** hitclose,Float* distclose);

static int
CsgIntersectInt(Ray *ray, HitList *hit1p,HitList* hit2p,Float dist1,Float dist2,HitList** hitclose,Float* distclose);

static void
CsgSetBounds(Csg *csg, Float bounds[2][3]);

static void
CsgHitlistCopy(HitList *from,HitList* to);

static void  	
CsgBounds(GeomRef gref, Float bounds[2][3]);

GeomRef 
CsgCreate(int op)
{
	Csg *csg;

	csg = (Csg *)share_malloc(sizeof(Csg));
	csg->csgoperator = op;
	csg->obj1 = csg->obj2 = (Geom *)NULL;


	switch(op) {
		case CSG_UNION:
			csg->intmeth = CsgUnionInt;
			break;
		case CSG_INTERSECT:
			csg->intmeth = CsgIntersectInt;
			break;
		case CSG_DIFFERENCE:
			csg->intmeth = CsgDifferenceInt;
			break;
		default:
			RLerror(RL_ABORT, "Unknown csg op type %d?\n",op);
	}
	return (GeomRef)csg;
}

Methods *
CsgMethods()
{
	if (iCsgMethods== (Methods *)NULL) {
		iCsgMethods = MethodsCreate();
		iCsgMethods->create = (TGeomMethod_Create)CsgCreate;
		iCsgMethods->convert = CsgConvert;
		iCsgMethods->methods = CsgMethods;
		iCsgMethods->name = CsgName;
		iCsgMethods->intersect.aggregate = CsgIntersect;
		iCsgMethods->bounds = CsgBounds;
		iCsgMethods->checkbounds = FALSE;
		iCsgMethods->closed = TRUE;
	}
	return iCsgMethods;
}

char *
CsgName()
{
	return csgName;
}

static int
csg_intersect_objs(Csg *csg, Ray *ray, HitList *hit1,HitList* hit2,Float mindist,Float* dist1,Float* dist2)
{
	int csgoperator;

	hit1->nodes = 0;
	hit2->nodes = 0;
	*dist1 = FAR_AWAY;
	*dist2 = FAR_AWAY;
	csgoperator = csg->csgoperator;

	if (!intersect(csg->obj1, ray, hit1, mindist, dist1) &&
	    ((csgoperator == CSG_INTERSECT) || (csgoperator == CSG_DIFFERENCE))) {
		/*
		 * Intersection and Difference cases: if you miss the first
		 * object, you missed the whole thing.
		 */
		return FALSE;
	}

	if (!intersect(csg->obj2, ray, hit2, mindist, dist2) &&
	    ((csgoperator == CSG_INTERSECT) ||
	     (hit1->nodes == 0) && (csgoperator == CSG_UNION))) {
		/*
		 * Intersect case:  if you miss either object, you miss whole
		 * Union case: if you miss both object, you miss whole
		 */
		return FALSE;
	}

	return TRUE;
}

static int
csg_enter_obj(HitList *hitp)
{
	if (hitp->data[0].enter)
		return hitp->data[0].enter - 1;

	return PrimEnter(hitp->data[0].obj, &hitp->data[0].ray,
			hitp->data[0].mindist, hitp->data[0].dist);
}

static int
CsgUnionInt(Ray *ray, HitList *hit1p,HitList* hit2p,Float dist1,Float dist2,HitList** hitclose,Float* distclose)
{
	Float distnext;
	HitList hitnext, *hittmp;

	while (TRUE) {
		if (hit2p->nodes == 0 ||
		    csg_enter_obj(hit2p)) {
			/* return hit1 */
			*hitclose = hit1p;
			*distclose = dist1;
			csg_set_enter(hit1p, csg_enter_obj(hit1p));
			return TRUE;
		} else {
			distnext = FAR_AWAY;
			hitnext.nodes = 0;
			if (!intersect(hit1p->data[hit1p->nodes-1].obj,
			    ray, &hitnext, dist2+EPSILON, &distnext)) {
				/*
				 * None of obj1 beyond, return hit2 (leaving)
				 */
				*hitclose = hit2p;
				*distclose = dist2;
				csg_set_enter(hit2p, FALSE);
				return TRUE;
			} else {
				/*
				 * Since hit1 is supposed to be the close one,
				 * swap them and copy hitnext into hit2.
	     			 */
				hittmp = hit1p;
				hit1p = hit2p;
				hit2p = hittmp;
				dist1 = dist2;
				CsgHitlistCopy(&hitnext, hit2p);
				dist2 = distnext;
				/* and continue */
			}
		}
	}
}

static int
CsgIntersectInt(Ray *ray, HitList *hit1p,HitList* hit2p,Float dist1,Float dist2,HitList** hitclose,Float* distclose)
{
	HitList *hittmp, hitnext;
	Float distnext;

	while (TRUE) {
		if (!csg_enter_obj(hit2p)) {
			/* Ray is leaving obj2 */
			/* Return hit1 info */
			*hitclose = hit1p;
			*distclose = dist1;
			csg_set_enter(hit1p, csg_enter_obj(hit1p));
			return TRUE;
		} else {
			distnext = FAR_AWAY;
			hitnext.nodes = 0;
			if (!intersect(hit1p->data[hit1p->nodes-1].obj,
			    ray, &hitnext, dist2+EPSILON, &distnext)) {
				/*
				 * None of obj1 beyond, so return miss
				 */
				return FALSE;
			} else {
				/*
				 * Since hit1 is supposed to be the
				 * close one, swap them and copy
				 * hitnext into hit2.
				 */
				hittmp = hit1p;
				hit1p = hit2p;
				hit2p = hittmp;
				dist1 = dist2;
				CsgHitlistCopy(&hitnext, hit2p);
				dist2 = distnext;
				/* and continue */
			}
		}
	}
}

static int
CsgDifferenceInt(Ray *ray, HitList *hit1p,HitList* hit2p,Float dist1,Float dist2,HitList** hitclose,Float* distclose)
{
	Float distnext;
	HitList hitnext;

	while (TRUE) {
		if (dist1 < dist2) {
			if (hit2p->nodes == 0 ||
			    csg_enter_obj(hit2p)) {
				/* return hit1 */
				*hitclose = hit1p;
				*distclose = dist1;
				csg_set_enter(hit1p, csg_enter_obj(hit1p));
				return TRUE;
			} else {
				distnext = FAR_AWAY;
				hitnext.nodes = 0;
				if (!intersect(hit1p->data[hit1p->nodes-1].obj,
				    ray, &hitnext, dist2+EPSILON, &distnext)) {
					/*
					 * None of obj1 beyond, so
					 * return miss
					 */
					return FALSE;
				} else {
					dist1 = distnext;
					CsgHitlistCopy(&hitnext, hit1p);
					/* and continue */
				}
			}
		} else { /* dist1 <= dist2 */
			if (hit1p->nodes == 0) {
				/* return a miss */
				return FALSE;
			}
			if (!csg_enter_obj(hit1p)) {
				/*
				 * return hit2, but invert hit2
				 * Enter/Leave flag
				 */
				*hitclose = hit2p;
				*distclose = dist2;
				csg_set_enter(hit2p, !csg_enter_obj(hit2p));
				return TRUE;
			} else {
				distnext = FAR_AWAY;
				hitnext.nodes = 0;
				if (!intersect(hit2p->data[hit2p->nodes-1].obj,
				    ray, &hitnext, dist1+EPSILON, &distnext)) {
					/*
					 * None of obj2 beyond, so
					 * return hit1
					 */
					*hitclose = hit1p;
					*distclose = dist1;
					/* we know we're entering obj1 */
					csg_set_enter(hit1p, TRUE);
					return TRUE;
				} else {
					dist2 = distnext;
					CsgHitlistCopy(&hitnext, hit2p);
					/* and continue */
				}
			}
		}
	}
}

int
CsgIntersect(GeomRef gref, Ray *ray, HitList *hitlist,Float mindist,Float* maxdist)
{
    Csg *csg = (Csg*)gref;
	Float dist1, dist2, disttmp, distclose;
	HitList hit1, hit2, *hit1p, *hit2p, *hitclose;

	hit1p = &hit1;
	hit2p = &hit2;
	if (!csg_intersect_objs(csg, ray, hit1p, hit2p, mindist,
	    &dist1, &dist2)) {
		/* missed the csg object */
		return FALSE;
	}

	if ((dist1 > dist2) &&
	    (csg->csgoperator == CSG_UNION || csg->csgoperator == CSG_INTERSECT)) {
		/* swap so 1 is closest (except in difference case) */
		disttmp = dist2;  
		dist2 = dist1;  
		dist1 = disttmp;
		hit1p = &hit2;  
		hit2p = &hit1;
	}

	/*
	 * Call appropriate intersection method.  If FALSE is return,
	 * no hit of any kind was found.
	 */
	if (!(*csg->intmeth)(ray, hit1p, hit2p, dist1, dist2,
	    &hitclose, &distclose))
		return FALSE;

	/*
	 * At this time, the closest hit is in hitclose and
	 * distclose.
	 */
	if (distclose < mindist || distclose > *maxdist)
		return FALSE;

	CsgHitlistCopy(hitclose, hitlist);
	*maxdist = distclose;
	return TRUE;
}

static void
CsgHitlistCopy(HitList *from,HitList* to)
{
	int i;

	to->nodes = from->nodes;
	for (i = 0; i < from->nodes; i++)
		to->data[i] = from->data[i];
}

static void
CsgSetBounds(Csg *csg, Float bounds[2][3])
{
	GeomComputeBounds(csg->obj1);
	GeomComputeBounds(csg->obj2);

	switch (csg->csgoperator) {
	case CSG_UNION:
		bounds[LOW][X] = min(csg->obj1->bounds[LOW][X], csg->obj2->bounds[LOW][X]);
		bounds[HIGH][X] = max(csg->obj1->bounds[HIGH][X], csg->obj2->bounds[HIGH][X]);
		bounds[LOW][Y] = min(csg->obj1->bounds[LOW][Y], csg->obj2->bounds[LOW][Y]);
		bounds[HIGH][Y] = max(csg->obj1->bounds[HIGH][Y], csg->obj2->bounds[HIGH][Y]);
		bounds[LOW][Z] = min(csg->obj1->bounds[LOW][Z], csg->obj2->bounds[LOW][Z]);
		bounds[HIGH][Z] = max(csg->obj1->bounds[HIGH][Z], csg->obj2->bounds[HIGH][Z]);
		break;
	case CSG_INTERSECT:
		bounds[LOW][X] = max(csg->obj1->bounds[LOW][X], csg->obj2->bounds[LOW][X]);
		bounds[HIGH][X] = min(csg->obj1->bounds[HIGH][X], csg->obj2->bounds[HIGH][X]);
		bounds[LOW][Y] = max(csg->obj1->bounds[LOW][Y], csg->obj2->bounds[LOW][Y]);
		bounds[HIGH][Y] = min(csg->obj1->bounds[HIGH][Y], csg->obj2->bounds[HIGH][Y]);
		bounds[LOW][Z] = max(csg->obj1->bounds[LOW][Z], csg->obj2->bounds[LOW][Z]);
		bounds[HIGH][Z] = min(csg->obj1->bounds[HIGH][Z], csg->obj2->bounds[HIGH][Z]);
		break;
	case CSG_DIFFERENCE:
		bounds[LOW][X] = csg->obj1->bounds[LOW][X];
		bounds[HIGH][X] = csg->obj1->bounds[HIGH][X];
		bounds[LOW][Y] = csg->obj1->bounds[LOW][Y];
		bounds[HIGH][Y] = csg->obj1->bounds[HIGH][Y];
		bounds[LOW][Z] = csg->obj1->bounds[LOW][Z];
		bounds[HIGH][Z] = csg->obj1->bounds[HIGH][Z];
		break;
	default:
		RLerror(RL_ABORT, "Unknown csg operator type %d?\n",
			csg->csgoperator);
	}
}

/*
 * Return index of hitlist node closest to the leaf and not below any
 * CSG object.
 */
int
FirstCSGGeom(HitList *hitlist)
{
	int i;

	/*
	 * UUUUGLY -- detect if obj is a CsgGeom by comparing
	 * methods with iCsgMethods.
	 */
	for (i = hitlist->nodes -1; i; i--)
		if (hitlist->data[i].obj->methods == iCsgMethods)
			return i;
	return 0;
}

void
CsgBounds(GeomRef gref, Float bounds[2][3])
{   
    Csg *csg =(Csg*)gref;
	CsgSetBounds(csg, csg->bounds);
	BoundsCopy(csg->bounds, bounds);
}

int
CsgConvert(GeomRef gref, Geom *list)
{
    Csg *csg = (Csg*)gref;
	static int OpenAdvised = FALSE;
	/*
	 * Currently, this only handles two objects.
	 * Will be fixed in the future.
	 * No really we promise.
	 */
	if (!list || !list->next) {
		RLerror(RL_WARN, "CSG needs at least two objects.\n");
		return 0;
	}
	if (list->next->next) {
		RLerror(RL_WARN, "Currently, CSG only handles two objects.\n");
		return 0;
	}
	/*
	 * Things are put into lists backwards....
	 */
	csg->obj2 = list;
	csg->obj1 = list->next;
	if ((!csg->obj1->methods->closed || !csg->obj2->methods->closed) &&
	    !OpenAdvised) {
		RLerror(RL_ADVISE,
			"Performing CSG with non-closed object(s).\n");
		OpenAdvised = TRUE;
	}
	return csg->obj1->prims + csg->obj2->prims;
}

static void
CsgMethodRegister(UserMethodType meth)
{
	if (iCsgMethods)
		iCsgMethods->user = meth;
}

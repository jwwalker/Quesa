/*
 * list.c
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
 * $Id: list.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:38:42  kolb
 * Initial version.
 * 
 */
#include "geom.h"
#include "list.h"

static Methods *iListMethods = NULL;
static char listName[] = "list";

void ListBounds(GeomRef gref, Float bounds[2][3]);

GeomRef
ListCreate(void)
{
	return (GeomRef)share_calloc(1, sizeof(List));
}
/*
 * Destroys the list and the items in it.
 */
static void
ListDelete(GeomRef	gref)
{
	List *list = (List*)gref;
	
	/* 
	 * We should delete both bounded, and unbounded lists:
	 */
	GeomDeleteEvery(list->list);
	GeomDeleteEvery(list->unbounded);
	
	share_free(list);
}

static char *
ListName()
{
	return listName;
}

/*
 * Take a list whose DATA field points to a linked list of objects and
 * turn it into a List.
 */
static int
ListConvert(GeomRef gref, Geom *objlist)
{
	List *list = (List*)gref;
	int num;

	/*
	 * Find the unbounded objects on the list as well as the
	 * bounding box of the list.
	 */
	list->list = objlist;
	for (num = 0; objlist; objlist = objlist->next)
		num += objlist->prims;
	return num;
}


/*
 * Intersect ray & list of objects.
 */
static int
ListIntersect(GeomRef gref, Ray *ray, HitList *hitlist,Float mindist,Float* maxdist)
{
	List *list= (List *)gref;
	Geom *objlist;
	Vector vtmp;
	Float s;
	int hit;

	hit = FALSE;
	/*
	 * Intersect with unbounded objects.
	 */
	for (objlist = list->unbounded; objlist ; objlist = objlist->next) {
		if (intersect(objlist, ray, hitlist, mindist, maxdist))
			hit = TRUE;
	}

	/*
	 * Check for intersection with bounding box.
	 */
	s = *maxdist;	/* So maxdist won't be reset. */
	VecAddScaled(ray->pos, mindist, ray->dir, &vtmp);
	if (OutOfBounds(&vtmp, list->bounds) &&
	    !BoundsIntersect(ray, list->bounds, mindist, &s))
		/*
		 * Ray never hit list.
		 */
		return hit;
	/*
	 * Else the ray enters list-space before it hits an
	 * unbounded object. Intersect with objects on list.
	 */
	for (objlist = list->list; objlist ; objlist = objlist->next) {
		if (intersect(objlist, ray, hitlist, mindist, maxdist))
			hit = TRUE;
	}

	return hit;
}



Methods *
ListMethods()
{
	if (iListMethods == (Methods *)NULL) {
		iListMethods = MethodsCreate();
		iListMethods->methods = ListMethods;
		iListMethods->create = (TGeomMethod_Create)ListCreate;
		iListMethods->name = ListName;
		iListMethods->intersect.aggregate = ListIntersect;
		iListMethods->bounds = ListBounds;
		iListMethods->convert = ListConvert;
		iListMethods->checkbounds = FALSE;
		iListMethods->closed = TRUE;
		iListMethods->deleteobj = ListDelete;
	}
	return iListMethods;
}

void
ListBounds(GeomRef gref, Float bounds[2][3])
{
	List *list = (List *)gref;

	BoundsInit(list->bounds);
	/*
	 * For each object on the list,
	 * compute its bounds...
	 */
	list->unbounded  = GeomComputeAggregateBounds(&list->list, 
				list->unbounded, list->bounds);
	BoundsCopy(list->bounds, bounds);
}

static void
ListMethodRegister(UserMethodType meth)
{
	if (iListMethods)
		iListMethods->user = meth;
}

/*
 * instance.c
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
 * $Id: instance.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:38:26  kolb
 * Initial version.
 * 
 */
#include "geom.h"
#include "instance.h"

static int
InstanceIntersect(GeomRef gref, Ray *ray, HitList *hitlist, Float mindist,Float* maxdist);

static void
InstanceBounds(GeomRef gref, Float bounds[2][3]);

static Methods *iInstanceMethods = NULL;
static char instanceName[] = "instance";

GeomRef 
InstanceCreate(
        Geom        *obj)
{
	Instance *inst;

	if (obj == (Geom *)NULL) {
		RLerror(RL_WARN, "Instance of NULL?\n");
		return (GeomRef)NULL;
	}
	inst = (Instance *)share_malloc(sizeof(Instance));
	inst->obj = obj;
	BoundsCopy(obj->bounds, inst->bounds);
	return (GeomRef)inst;
}

char *
InstanceName()
{
	return instanceName;
}


/*
 * Intersect ray & an instance by calling intersect.
 */
static int
InstanceIntersect(GeomRef gref, Ray *ray, HitList *hitlist, Float mindist,Float* maxdist)
{
	Instance* inst=(Instance*)gref;
	return intersect(inst->obj, ray, hitlist, mindist, maxdist);
}

Methods *
InstanceMethods()
{
	/*
	 * Instances are special in that there is no
	 * "convert" method -- when created, they are passed
	 * a pointer to the object being instantiated.
	 * This means that you will need to set an instance's
	 * 'prims' field by hand (e.g., inst->prims = object->prims).
	 */
	if (iInstanceMethods == (Methods *)NULL) {
		iInstanceMethods = MethodsCreate();
		iInstanceMethods->methods = InstanceMethods;
		iInstanceMethods->create = InstanceCreate;
		iInstanceMethods->name = InstanceName;
		iInstanceMethods->intersect.aggregate = InstanceIntersect;
		iInstanceMethods->bounds = InstanceBounds;
		iInstanceMethods->convert = NULL;
		iInstanceMethods->checkbounds = FALSE;
		iInstanceMethods->closed = TRUE;
	}
	return iInstanceMethods;
}

void
InstanceBounds(GeomRef gref, Float bounds[2][3])
{
	Instance *inst = (Instance*)gref;
	GeomComputeBounds(inst->obj);
	BoundsCopy(inst->obj->bounds, inst->bounds);
	BoundsCopy(inst->bounds, bounds);
}

void
InstanceMethodRegister(UserMethodType meth)
{
	if (iInstanceMethods)
		iInstanceMethods->user = meth;
}

/*
 * objdef.c
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
 * $Id: objdef.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:46:38  kolb
 * Initial version.
 * 
 */

#include "rayshade.h"
#include "options.h"
#include "stats.h"

static Geom *Objects = NULL;		/* named objects */
Geom 		*World;					/* top-level object */


/*
 * Return pointer to named object, NULL if no such object has been defined.
 */
static Geom *
GeomGetNamed(char *name)
{
	Geom *otmp;
	for (otmp = Objects; otmp; otmp = otmp->next)
		if (strcmp(name, otmp->name) == 0)
			return otmp;
	return (Geom *)NULL;
}

/*
 * Add object to list of defined objects.  At this point, the object has
 * been converted to the correct type, and obj->next is either NULL or
 * garbage.
 */
static void
GeomAddToDefined(Geom *obj)
{
	obj->next = Objects;
	Objects = obj;
	if (Options.verbose)
		AggregatePrintInfo(obj, Stats.fstats);
	else
		AggregatePrintInfo(obj, (FILE *)NULL);
}

/*
 * Return a copy of the named object.
 */
Geom *
GeomCopyNamed(char *name)
{
	Geom *child;

	child = GeomGetNamed(name);
	if (child == (Geom *)NULL)
		RLerror(RL_PANIC, "There is no object named \"%s\".", name);
	child = GeomCopy(child);
	return child;
}

/*
 * Main ray-spwaning routine required by libray
 */
int
TraceRay(Ray *ray, HitList *hitlist, Float mindist,Float * maxdist)
{
	return intersect(World, ray, hitlist, mindist, maxdist);
}

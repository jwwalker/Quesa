/*
 * light.c
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
 * $Id: light.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:35:01  kolb
 * Initial version.
 * 
 */
#include "light.h"

Light *
LightCreate(LightRef light, LightMethods *meth, Color *color)
{
	Light *ltmp;

	if (light == (LightRef)NULL || meth == (LightMethods *)NULL)
		return (Light *)NULL;

	ltmp = (Light *)share_malloc(sizeof(Light));
	ltmp->light = light;
	ltmp->methods = meth;
	ltmp->color = *color;
	ltmp->next = (Light *)NULL;
	ltmp->cache = (ShadowCache *)NULL;
	ltmp->shadow = TRUE;
	return ltmp;
}

void
LightDelete(Light*  inLight)
{
    if (inLight->methods->deletelight)
        inLight->methods->deletelight(inLight->light);
    else
        share_free(inLight->light);
    if (inLight->cache)
        Free(inLight->cache);
    share_free(inLight);
}

LightMethods *
LightMethodsCreate(void)
{
	return (LightMethods *)share_calloc(1, sizeof(LightMethods));
}

/*
 * Compute light color.  Returns FALSE if in full shadow, TRUE otherwise.
 * Computed light color is stored in 'color'.
 */
int
LightIntens(
            Light           *lp, 
            Ray             *ray, 
            Float           dist, 
            int             noshadow, 
            Color           *color)
{
	if (lp->methods->intens)
		return (*lp->methods->intens)(lp->light, &lp->color,
			lp->cache, ray, dist, noshadow || !lp->shadow, color);
	RLerror(RL_ABORT, "Cannot compute light intensity!\n");
	return FALSE;
}

/*
 * Calculate ray and distance from position to light.
 */
int
LightDirection(
        Light           *lp,
        Vector          *objpos,
        Vector          *lray,
        Float           *dist)
{
	if (lp->methods->dir) {
		(*lp->methods->dir)(lp->light, objpos, lray, dist);
		return TRUE;
	} else {
		RLerror(RL_ABORT, "Cannot compute light direction!\n");
		return FALSE;
	}
}

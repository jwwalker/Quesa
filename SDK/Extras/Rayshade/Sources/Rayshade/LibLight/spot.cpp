/*
 * spot.c
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
 * $Id: spot.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:35:42  kolb
 * Initial version.
 * 
 */
#include "light.h"
#include "spot.h"

static LightMethods *iSpotMethods = NULL;

static int  SpotIntens(LightRef lr, Color *lcolor,ShadowCache *cache, 
                Ray *ray, Float dist,int noshadow, Color *color);
static void SpotDirection(LightRef lr, Vector *pos,Vector* dir,Float* dist);
static Float rampup(Float left,Float right,Float at);

static Float SpotAtten(Spotlight *lp, Vector *dir);
Spotlight *
SpotCreate(
        Vector          *from,
        Vector          *to, 
        Float           coef,
        Float           in,
        Float           out)
{
	Spotlight *spot;

	spot = (Spotlight *)share_malloc(sizeof(Spotlight));
	spot->pos = *from;
	VecSub(*to, *from, &spot->dir);
	if (VecNormalize(&spot->dir) == 0. || in > out) {
		RLerror(RL_ABORT,"Invalid spotlight specification.\n");
		return (Spotlight *)NULL;
	}
	spot->coef = coef;
	spot->radius = cos(deg2rad(in));
	spot->falloff = cos(deg2rad(out));

	return spot;
}

LightMethods *
SpotMethods(void)
{
	if (iSpotMethods == (LightMethods *)NULL) {
		iSpotMethods = LightMethodsCreate();
		iSpotMethods->intens = SpotIntens;
		iSpotMethods->dir = SpotDirection;
	}
	return iSpotMethods;
}

/*
 * Calculate intensity ('color') of light reaching 'pos' from light 'lp'.
 * The spotlight is 'dist' units from 'pos' along 'dir'.
 *
 * Returns TRUE if non-zero illumination, FALSE otherwise.
 */
int
SpotIntens(
        LightRef        lr, 
        Color           *lcolor, 
        ShadowCache     *cache, 
        Ray             *ray, 
        Float           dist, 
        int             noshadow,
        Color           *color)

{
    Spotlight       *spot = (Spotlight*)lr;
	Float atten;
	extern Float SpotAtten(Spotlight*,Vector*);

	/*
	 * Compute spotlight color
	 */
	atten = SpotAtten(spot, &ray->dir);
	/*
	 * If outside of spot, return FALSE.
	 */
	if (atten == 0.)
		return FALSE;
	if (Shadowed(color, lcolor, cache, ray, dist, noshadow))
		return FALSE;
	ColorScale(atten, *color, color);
	return TRUE;
}

/*
 * Compute intensity of spotlight along 'dir'.
 */
static Float
SpotAtten(
        Spotlight       *lp, 
        Vector          *dir)
{
	Float costheta, atten;

	costheta = -dotp(dir, &lp->dir);
	/*
	 * Behind spotlight.
	 */
	if (costheta <= 0.)
		return 0.;
	/*
	 * Intensity is the product of costheta raised to lp->coef and
	 * a function that smoothly interpolates from 0 at
	 * costheta=lp->falloff to 1 at costheta=lp->radius.
	 */
	atten = pow(costheta, lp->coef);
	if (lp->radius > 0.)
		atten *= rampup(lp->falloff, lp->radius, costheta);
	return atten;
}

/*
 * Cubic interpolation between 0 at left and 1 at right, sampled at 'at'
 * It is assumed that right >= left.
 */
Float
rampup(Float left,Float right,Float at)
{
	if (at < left)
		return 0.;
	else if (at > right)
		return 1.;

	if (right == left)
		return 0.;

	at = (at - left) / (right - left);
	return (3 - 2*at)*at*at;
}

static void
SpotDirection(LightRef lr, Vector *pos,Vector* dir,Float* dist)
{
    Spotlight *lp = (Spotlight*)lr;
	/*
	 * Calculate dir from position to center of light source.
	 */
	VecSub(lp->pos, *pos, dir);
	*dist = VecNormalize(dir);
}

static void
SpotMethodRegister(UserMethodType meth)
{
	if (iSpotMethods)
		iSpotMethods->user = meth;
}

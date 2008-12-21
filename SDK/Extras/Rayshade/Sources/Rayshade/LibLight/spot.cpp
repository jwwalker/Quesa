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
 * $Id: spot.cpp,v 1.2 2008-12-21 02:04:26 jwwalker Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.1  2002/12/18 18:36:42  pepe
 * First upload
 *
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

static Float SpotAtten(Spotlight *lp, Vector *dir);
Spotlight *
SpotCreate(
        Vector          *from,
        Vector          *dir,
        Float           hotAngle,
        Float           outerAngle ,
		int				attenuation ,
		int				fallOff )
{
	Spotlight *spot;

	spot = (Spotlight *)share_malloc(sizeof(Spotlight));
	spot->pos = *from;
	spot->dir.x = dir->x ;
	spot->dir.y = dir->y ;
	spot->dir.z = dir->z ;
	if (VecNormalize(&spot->dir) == 0. || hotAngle > outerAngle ) {
		RLerror(RL_ABORT,"Invalid spotlight specification.\n");
		return (Spotlight *)NULL;
	}
	spot->hotAngle = hotAngle ;
	spot->outerAngle = outerAngle ;
	spot->cosHotAngle = cos ( hotAngle ) ;
	spot->cosOuterAngle = cos ( outerAngle ) ;
	spot->attenuation = attenuation ;
	spot->fallOff = fallOff ;

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

#define kQ3PiOver2                     ((Float)  (3.1415926535898 / 2.0))
static const float eMinus1 = exp ( 1.0f ) - 1.0f ;

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

	if ( costheta < lp->cosOuterAngle )
		return 0.0 ;

	atten = costheta ;
	if (lp->cosHotAngle > 0.) // What is this all about ?
		{
		Float fallOffMultiplier = 1.0 ;
		if ( lp->fallOff == 1 /*kQ3FallOffTypeLinear*/ )
			{
			if ( costheta < lp->cosHotAngle )
				{
				Float angle = acos ( costheta ) ;
				fallOffMultiplier = ( lp->outerAngle - angle ) /
					( lp->outerAngle - lp->hotAngle ) ;
				}
			}
		else
		if ( lp->fallOff == 2/*kQ3FallOffTypeExponential*/ )
			{
			if ( costheta < lp->cosHotAngle )
				{
				Float angle = acos ( costheta ) ;
				fallOffMultiplier = ( exp ( ( lp->outerAngle - angle ) /
					( lp->outerAngle - lp->hotAngle ) ) - 1 ) / eMinus1 ;
				}
			}
		else
		if ( lp->fallOff == 3/*kQ3FallOffTypeCosine*/ )
			{
			if ( costheta < lp->cosHotAngle )
				{
				Float angle = acos ( costheta ) ;
				fallOffMultiplier = cos ( ( angle - lp->hotAngle ) * kQ3PiOver2 /
					( lp->outerAngle - lp->hotAngle ) ) ;
				}
			}

				
/*
		This may be required some time but presently neither the attenuation  
nor the world pixel position are available. Maybe it should go  
somewhere else in the calculations
		if ( lp->attenuation != kQ3AttenuationTypeNone )
			{
			switch ( lp->attenuation )
				{
	    		case kQ3AttenuationTypeInverseDistance :
	    			{
	    			fallOffMultiplier *= 1.0f / Distance ( pixelWorldPos - lp- 
pos ) ;
	    			break ;
	   				}
				case kQ3AttenuationTypeInverseDistanceSquared :
	    			{
	    			fallOffMultiplier *= 1.0f / DistanceSquared ( pixelWorldPos -  
lp->pos ) ;
	    			break ;
	   				}
				}
			}
*/		
		
		atten *= fallOffMultiplier ;
		}
	return atten;
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

/*
 * shade.c
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
 * $Id: shade.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:47:36  kolb
 * Initial version.
 * 
 */
#include "shade.h"
#include "rayshade.h"
#include "libsurf/surface.h"
#include "liblight/light.h"
#include "libsurf/atmosphere.h"
#include "options.h"
#include "stats.h"
#include "libtext/CTexture.h"

Medium	TopMedium;
Atmosphere *AtmosEffects;

static void
shade(
        Vector      *pos,       /* hit pos */
        Ray         *ray, 
        Vector      *nrm,       /* shade normal */
        Vector      *gnrm,      /* geo normal */
        int         smooth,     /* true if shading norm and geo norm differ */
        int         enter,      /* TRUE if entering surface */
        Surface     *surf,      /* indicent ray */
        Color       *back,      /* properties of hit surface */
        Color       *color,     /* background color, computed color */
        Color       *contrib);   /* contribution to final pixel value */		


static int TransmitRay(Ray *ray,Vector *pos,Vector *norm,Float k,Float index,
    Float statten,int enter,Color *back,Color *contrib,Color *intens,Color *color);
static void LightRay(Light *lp,Vector *pos,Vector *norm, Vector *gnorm,int smooth,Vector *reflect,Surface *surf,int depth,      /* ray depth, */ 
    int samp,Float time, Color *color); 
static void ReflectRay(Ray *ray,Vector *pos,Vector *dir,Color *back,
        Color *intens,Color *contrib,Color *color);
static void Lighting(Float costheta,Float cosalpha,Color *lcolor,Color *diff,
    Color *spec,Float coef,Color *color);
/*
 * Calculate color of ray.
 */
void
ShadeRay(
    HitList         *hitlist,   /* Information about point of intersection. */
    Ray             *ray,       /* Direction and origin of ray. */
    Float           dist,       /* Distance from origin of intersection. */
    Color           *back,      /* "Background" color */
    Color           *color,     /* Color to assign current ray. */
    Color           *contrib)   /* Contribution of this ray to final color */
{
	Vector norm, gnorm, pos; /* surface normal, point of intersection */
	Surface surf, *stmp;	/* surface properties */
	int enter, smooth;	/* entering ?, gnorm != snorm ?*/

	if (hitlist->nodes == 0) {
		/*
		 * No valid intersection.  Set distance for atmospheric
		 * effects and set color of ray to background.
		 */
		*color = *back;
		VecAddScaled(ray->pos, FAR_AWAY, ray->dir, &pos);
		if (!ray->media && AtmosEffects)
			Atmospherics(AtmosEffects, ray, FAR_AWAY, &pos, color);
		return;
	}

	/*
	 * Compute normal, surface properties, etc.
	 */
	stmp = GetShadingSurf(hitlist);
	surf = *stmp;
	enter = ComputeSurfProps(hitlist, ray, &pos, &norm, &gnorm, &surf,
			&smooth);
	Stats.HitRays++;

	/*
	 * Calculate ray color.
	 */
	shade(&pos, ray, &norm, &gnorm, smooth, enter, &surf, back, color,
			contrib);
	if (!ray->media && AtmosEffects)
		Atmospherics(AtmosEffects, ray, dist, &pos, color);
}

/*
 * Perform lighting calculations based on surface normal & other properties,
 * incident ray direction and position, and light source properties.
 * Spawn any necessary reflected and transmitted rays.
 */
static void
shade(
        Vector      *pos,       /* hit pos */
        Ray         *ray, 
        Vector      *nrm,       /* shade normal */
        Vector      *gnrm,      /* geo normal */
        int         smooth,     /* true if shading norm and geo norm differ */
        int         enter,      /* TRUE if entering surface */
        Surface     *surf,      /* indicent ray */
        Color       *back,      /* properties of hit surface */
        Color       *color,     /* background color, computed color */
        Color       *contrib)   /* contribution to final pixel value */		
{
	Float	k;		/* -ray . normal */
	Color	newcontrib;
	Vector	refl;		/* reflected direction */
	Color	reflectivity,	/* effective surface reflectivity */
		intens;		/* reflected/transmitted intensity */
	Light *lp;		/* current light source */
	extern Light *Lights;	/* list of defined sources */

	/*
	 * Ambient color is always included.
	 */
	ColorMultiply(surf->amb, Options.ambient, color);

	/*
	 * Calculate direction of reflected ray.
	 */
	k = -dotp(&ray->dir, nrm);
	VecAddScaled(ray->dir, 2.*k, *nrm, &refl);

	/*
	 * Calculate intensity contributed by each light source.
	 */
	for (lp = Lights; lp; lp = lp->next)
		LightRay(lp, pos, nrm, gnrm, smooth, &refl, surf,
				ray->depth, ray->sample, ray->time, color);

	if (ray->depth >= Options.maxdepth)
		/*
		 * Don't spawn any transmitted/reflected rays.
		 */
		return;
	/*
	 * Specular transmission (refraction).
	 */
	ColorScale(surf->reflect, surf->spec, &reflectivity);

	if (surf->transp > EPSILON) {
		ColorScale(surf->transp, surf->body, &intens);
		ColorMultiply(intens, *contrib, &newcontrib);
		if (newcontrib.r > Options.cutoff.r ||
		    newcontrib.g > Options.cutoff.g ||
		    newcontrib.b > Options.cutoff.b)
			/*
			 * Transmit ray.  If TIR occurs, add transmitted
			 * component to reflected component.  Kinda strange, but...
			 */
			if (TransmitRay(ray, pos, nrm, k, surf->index,
			    surf->statten, enter, back, &newcontrib, &intens, color))
				ColorAdd(reflectivity, intens, &reflectivity);
	}

	if (reflectivity.r > EPSILON ||
	    reflectivity.g > EPSILON ||
	    reflectivity.b > EPSILON) {
		ColorMultiply(reflectivity, *contrib, &newcontrib);
		if (newcontrib.r > Options.cutoff.r ||
		    newcontrib.g > Options.cutoff.g ||
		    newcontrib.b > Options.cutoff.b)
			ReflectRay(ray, pos, &refl, back, &reflectivity,
				&newcontrib, color);
	}
}

/*
 * Lighting calculations
 */
static void
LightRay(
    Light       *lp,        /* Light source */
    Vector      *pos,       /* hit pos */ 
    Vector      *norm,      /* shade norm */ 
    Vector      *gnorm,     /* geo norm */
    int         smooth,     /* true if shade and geo norm differ */
    Vector      *reflect,   /* reflection direction */
    Surface     *surf,      /* surface characteristics */
    int         depth,      /* ray depth, */ 
    int         samp,       /* sample # */
    Float       time,
    Color       *color)     /* resulting color */		
{
	Color lcolor;
	Ray newray;
	Float costheta, cosalpha, dist;

	newray.pos = *pos;
	newray.depth = depth;
	newray.sample = samp;
	newray.time = time; 
	newray.media = (Medium *)NULL;	

	LightDirection(lp, pos, &newray.dir, &dist);

	costheta = dotp(&newray.dir, norm);

	if (smooth) {
		cosalpha = dotp(&newray.dir, gnorm); 
		/*
		 * If shading normal indicates self-shadowing
		 * and geom normal indicates no self-shadowing,
		 * trust the geom normal.
		 */
		if (costheta <= 0. && cosalpha > 0.)
			costheta = cosalpha;
		/*
		 * If geom normal indicates self-shadowing and
		 * geom normal doesn't, then have to do something
		 * clever ala Snyder & Barr.
		 */
	}

	if (costheta <= 0.) {
		/*
		 * Light source is on opposite side of surface,
		 * hence light must be transmitted through...
		 */
		if (surf->translucency < EPSILON)
			return;
		if (!LightIntens(lp, &newray, dist,
			(int)surf->noshadow, &lcolor))
			return;
		cosalpha = -dotp(reflect, &newray.dir);
		Lighting(-costheta, cosalpha, &lcolor, &surf->translu,
				&surf->body, surf->stexp, color);
		ColorScale(surf->translucency, *color, color);
	} else {
		if (!LightIntens(lp, &newray, dist,
			(int)surf->noshadow, &lcolor))
			return;  /* prim is in shadow w.r.t light source */

		cosalpha = dotp(reflect, &newray.dir);
		Lighting(costheta, cosalpha, &lcolor, &surf->diff,
				&surf->spec, surf->srexp, color);
	}
}

/*
 * Compute shading function (diffuse reflection and specular highlight)
 *
 * This function *adds* the computed color to "color".
 */
static void
Lighting(
    Float       costheta,
    Float       cosalpha,
    Color       *lcolor,
    Color       *diff,
    Color       *spec,
    Float       coef,
    Color       *color)
{
	Float intens;

	/*
	 * Diffuse reflection.
	 * Falls off as the cosine of the angle between
	 * the normal and the ray to the light (costheta).
	 */
	color->r += diff->r * costheta * lcolor->r;
	color->g += diff->g * costheta * lcolor->g;
	color->b += diff->b * costheta * lcolor->b;
	/*
	 * Specularly reflected highlights.
	 * Fall off as the cosine of the angle
	 * between the reflected ray and the ray to the light source.
	 */
	if (coef < EPSILON || cosalpha <= 0.)
		return;
	/*
	 * Specular highlight = cosine of the angle raised to the
	 * appropriate power.
	 */
	intens = pow(cosalpha, coef);
	color->r += spec->r * intens * lcolor->r;
	color->g += spec->g * intens * lcolor->g;
	color->b += spec->b * intens * lcolor->b;
}

/*
 * Spawn a transmitted ray.  Returns TRUE if total internal reflection
 * occurs, FALSE otherwise.
 */
static int
TransmitRay(
    Ray         *ray, 
    Vector      *pos,
    Vector      *norm,
    Float       k, 
    Float       index,
    Float       statten,
    int         enter,
    Color       *back,
    Color       *contrib,
    Color       *intens,
    Color       *color)
{
	int total_int_refl = FALSE;
	Ray NewRay;
	Float dist;
	Color newcol;
	HitList hittmp;		/* Geom intersection record */

	NewRay.pos = *pos;		/* Origin == hit point */
	NewRay.media = ray->media;	/* Media == old media */
	NewRay.sample = ray->sample;
	NewRay.time = ray->time;
	NewRay.depth = ray->depth + 1;

	if (enter) {
		/*
		 * Entering surface.
		 */
		if (Refract(&NewRay.dir,
		    NewRay.media ? NewRay.media->index :
		    TopMedium.index, index, &ray->dir, norm, k)) {
			total_int_refl = TRUE;
		} else {
			/*
			 * Push information for new medium.
			 */
			NewRay.media = MediumPush(index, statten, NewRay.media);
		}
	} else {
		/*
		 * Exiting surface
		 * Pop medium from stack.
		 */
		if (NewRay.media != (Medium *)0)
			NewRay.media = NewRay.media->next;
		if (Refract(&NewRay.dir, index,
		    NewRay.media ? NewRay.media->index :
		    TopMedium.index, &ray->dir, norm, k)) {
			total_int_refl = TRUE;
		}
	}

	/*
	 * At this point, NewRay.media is the medium into which
	 * the new ray is entering.
	 */

	if (!total_int_refl) {
		Stats.RefractRays++;
		hittmp.nodes = 0;
		dist = FAR_AWAY;
		TraceRay(&NewRay, &hittmp, EPSILON, &dist);
		ShadeRay(&hittmp, &NewRay, dist, back, &newcol, contrib);
		ColorMultiply(newcol, *intens, &newcol);
		/*
		 * Attenuate transmitted color.  Note that
		 * if the transmitted ray hit nothing, we still
		 * perform this computation, as it's possible
		 * that 'air' has a non-unit statten.
		 */
		statten = NewRay.media ? NewRay.media->statten :
			TopMedium.statten;
		if (statten != 1.0) {
			statten = pow(statten, dist);
			ColorScale(statten, newcol, &newcol);
		}
		ColorAdd(*color, newcol, color);
		/* Free pushed medium */
		if (enter)
			free((voidstar)NewRay.media);
	}

	return total_int_refl;
}

static void
ReflectRay(
        Ray         *ray,
        Vector      *pos,
        Vector      *dir, 
        Color       *back,
        Color       *intens,
        Color       *contrib,
        Color       *color)
{
	Ray NewRay;
	HitList hittmp;		/* Geom intersection record */
	Color newcol;
	Float dist;

	NewRay.pos = *pos;		/* Origin == hit point */
	NewRay.dir = *dir;		/* Direction == reflection */
	NewRay.media = ray->media;	/* Medium == old medium */
	NewRay.sample = ray->sample;
	NewRay.time = ray->time;
	NewRay.depth = ray->depth + 1;
	Stats.ReflectRays++;
	hittmp.nodes = 0;
	dist = FAR_AWAY;
	(void)TraceRay(&NewRay, &hittmp, EPSILON, &dist);
	ShadeRay(&hittmp, &NewRay, dist, back, &newcol, contrib);
	ColorMultiply(newcol, *intens, &newcol);
	ColorAdd(*color, newcol, color);
}

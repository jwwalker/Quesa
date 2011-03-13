/*
 * surface.h
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
 * $Id: surface.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:41:02  kolb
 * Initial version.
 * 
 */
#ifndef SURFACE_H
#define SURFACE_H

#define DEFAULT_INDEX		1.0	/* Default index of refraction */
#define DEFAULT_PHONGPOW	15.0	/* Default specular highlight exp */

#include "libobj/geom.h"
/*
 * Surface definition.
 */
typedef struct Surface {
	char	*name;			/* Name */
	struct Color	amb,		/* Ambient 'curve' */
			diff,		/* Diffuse reflection 'curve' */
			spec,		/* Specular reflection 'curve' */
			translu,	/* Diffuse transmission 'curve' */
			body;		/* Specular transmission 'curve' */
	Float	srexp,			/* Specular reflection exponent */
		stexp,			/* Specular transmission exponent */
		statten,		/* Specular transmission attenuation */
		index,			/* Index of refraction */
		reflect,		/* Specular reflectivity */
		transp,			/* Specular transmittance */
		translucency;		/* Diffuse transmittance */ 
	char	noshadow;		/* No shadowing? */
	struct Surface *next;		/* Next surface in list (if any) */
} Surface;

/*
 * Linked list of (linked list of) surfaces.
 */
typedef struct SurfList {
	Surface *surf;
	struct SurfList *next;
} SurfList;

extern Surface	*SurfaceCreate();	/* application-provided */
		

extern SurfList *SurfPop(SurfList *list);

extern SurfList *SurfPush(
                    Surface         *surf, 
                    SurfList        *list);
extern void	SurfaceBlend(    
                    Surface         *surf1,
                    Surface         *surf2, 
                    Float           p,
                    Float           q);
                    
extern void ColorBlend(
                    Color           *color1,
                    Color           *color2,
                    Float           p,
                    Float           q);

extern int  ComputeSurfProps(
                    HitList         *hitlist,   /* Hit information (path through DAG) */
                    Ray             *ray,       /* Ray in world space */
                    Vector          *pos,       /* Intersection point */
                    Vector          *norm,      /* shading normal, geometric normal (return values) */
                    Vector          *gnorm,
                    Surface         *surf,      /* Copy of surface to use, texture-modified */
                    int             *smooth);

extern Surface      *SurfaceCopy(
                            Surface         *surf);
                            
extern void         SurfaceAddToDefined(
                            Surface         *surf);
                            
extern void         SurfaceDeleteDefinedSurfaces(void);

extern Surface      *SurfaceGetNamed(
                            char            *name);
                            
extern void         SurfaceDeleteOne(Surface    *surf);
extern void         SurfaceDeleteEvery(Surface  *surf);
extern Surface *    GetShadingSurf(HitList *hitlist);
extern Surface *    SurfaceFetchNamed(const char *name);
#endif /* SURFACE_H */

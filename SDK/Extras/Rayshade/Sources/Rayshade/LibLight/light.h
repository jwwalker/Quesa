/*
 * light.h
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
 * $Id: light.h,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:35:10  kolb
 * Initial version.
 * 
 */
#ifndef LIGHT_H
#define LIGHT_H

#include "libobj/geom.h"

#define SHADOW_NONE	001
#define SHADOW_TRANSP	002
#define SHADOW_CSG	004
#define SHADOW_CACHE	010
#define SHADOW_BLUR	020

#define NOSHADOWS(f)	((f) & SHADOW_NONE)
#define SHADOWTRANSP(f)	((f) & SHADOW_TRANSP)
#define SHADOWCSG(f)	((f) & SHADOW_CSG)
#define SHADOWCACHE(f)	((f) & SHADOW_CACHE)
#define SHADOWBLUR(f)	((f) & SHADOW_BLUR)

#define SHADOW_EPSILON	(4. * EPSILON)

typedef char * LightRef;

typedef struct {
	struct Geom 	*obj;			/* Pointer to cached object */
	RSMatrix 		trans;			/* World-to-object transformation */
	char 			dotrans;		/* TRUE if above trans is non-identity */
} ShadowCache;

typedef int    (*TLightMethod_Intens)(LightRef inLight,Color *lcolor,ShadowCache *cache, 
                          Ray *ray, Float dist,int noshadow,Color *color);
typedef void   (*TLightMethod_Delete)(LightRef inLight);
typedef void   (*TLightMethod_Dir)(LightRef inLight,Vector *objpos,Vector *lray,Float *dist);
                        
typedef struct {
	TLightMethod_Intens intens;	       /* intensity method */                      
	TLightMethod_Dir    dir;           /* direction method */
	void        	    (*user)();	   /* user-defined method */
    TLightMethod_Delete deletelight;   /* delete the light's private data */
} LightMethods;

class CRSLight;

typedef class CRSLight {
public:
	virtual  		~CRSLight();
	
	virtual int		CalculateIntensity(Color* lcolor,ShadowCache *cache,Ray *ray,
						   Float* dist,int noshadow,Color *color);
						   
	virtual void	Direction(Vector* objpos,Vector *lray,Float *dist);
	

	Color			color;		/* Light source color & intensity */
	bool			shadow;		/* Does light source cast shadows? */
	ShadowCache		*cache;
	CRSLight		*next;
	
	/* ToDo: Remove */
	LightRef light;		    /* Pointer to light information */
	LightMethods *methods;	/* Light source methods */
	
} CRSLight;

typedef struct Light {
	Color color;		    
	int shadow;		        
	LightRef light;		    /* Pointer to light information */
	LightMethods *methods;	/* Light source methods */
	ShadowCache *cache;	    /* Shadow cache, if any */
	struct Light *next;	    /* Next light in list */
} Light;

extern LightMethods	*LightMethodsCreate();

extern Light	*LightCreate(
            LightRef        light, 
            LightMethods    *meth, 
            Color           *color);
extern void  LightDelete(Light*  inLight);

extern void	LightAllocateCache();
extern void LightAddToDefined(Light *light);

extern void LightDeleteDefinedLights();

extern int	LightIntens(
            Light           *lp, 
            Ray             *ray, 
            Float           dist, 
            int             noshadow, 
            Color           *color);
            
extern int LightDirection(
            Light           *lp,
            Vector          *objpos,
            Vector          *lray,
            Float           *dist);
extern void LightSetup(void);
            
extern void ShadowStats(unsigned long *shadowrays,unsigned long * shadowhit,unsigned long* cachehit,unsigned long* cachemiss);

extern void ShadowSetOptions(long options);

extern int Shadowed(Color           *result, /* resultant intensity */
                    Color           *color,  /* light color */
                    ShadowCache     *cache,  /* shadow cache for light */
                    Ray             *ray,    /* ray, origin on surface, dir towards light */
                    Float           dist,    /* distance from pos to light source */
                    int             noshadow);/* If TRUE, no shadow ray is cast. */	



#endif /* LIGHT_H */

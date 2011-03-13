/*
 * geom.h
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
 * $Id: geom.h,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0.1.1  92/02/07  13:10:58  cek
 * patch6: Decreased MAXMODELDEPTH to keep from beating on the stack.
 * 
 * Revision 4.0  91/07/17  14:37:52  kolb
 * Initial version.
 * 
 */
#ifndef OBJECT_H
#define OBJECT_H

#include "libcommon/common.h"
#include "libcommon/transform.h"
#include "bounds.h"

/*
 * Constants for enter flag in HitNode.
 */
#define EXITING		1
#define ENTERING	2

#define MAXMODELDEPTH	32		/* Maximum height of DAG. */

typedef struct Geom Geom;
typedef struct HitList HitList;

typedef char * GeomRef;
typedef GeomRef GeomCreateFunc(Geom *obj);

/*
 * If the object has a normal method, it's a primitive
 * otherwise it's an aggregate (or an instance)
 */
#define IsAggregate(o)		((o)->methods->normal == NULL)



typedef GeomRef  (*TGeomMethod_Create) (Geom *obj);
typedef int  (*TGeomMethod_Convert) (GeomRef obj,Geom *objlist);
typedef void (*TGeomMethod_Bounds)  (GeomRef obj,Float bounds[2][3]);
typedef int  (*TGeomMethod_Normal)  (GeomRef obj,Vector *pos,Vector* norm,Vector* gnorm);
typedef void  (*TGeomMethod_UV)      (GeomRef obj,Vector *pos,Vector *norm,Vec2d *uv,Vector *dpdu,Vector *dpdv);
typedef void  (*TGeomMethod_Stats)   (unsigned long *tests,unsigned long *hits);
typedef int  (*TGeomMethod_Enter)   (GeomRef obj,Ray *ray, Float mind,Float hitd);
typedef int  (*TGeomMethod_IntersectAggregate) (GeomRef obj,Ray *ray, HitList *list,Float mindist,Float *maxdist);
typedef int  (*TGeomMethod_IntersectNormal) (GeomRef obj,Ray *ray, Float mindist,Float *maxdist);

typedef union TGeomMethod_Intersect {
	TGeomMethod_IntersectAggregate		aggregate;
	TGeomMethod_IntersectNormal			normal;
}  TGeomMethod_Intersect;
/*
 * Geom methods.
 * (p) means applies only to primitive objects
 * (a) means applies only to aggregate objects
 */
typedef struct Methods {
	char		        *(*name)();		    /* Geom name */
	TGeomMethod_Create		create;			/* Create and return ref */
	TGeomMethod_Intersect intersect;		/* Ray/obj intersection */
	TGeomMethod_Normal	 normal;		    /* Geom normal (p) */
	TGeomMethod_Enter	 enter;		        /* Ray enter or exit? (p) */
	TGeomMethod_Convert	 convert;		    /* Convert from list (a) */
	TGeomMethod_UV		 uv;		        /* 2D mapping (p) */
	TGeomMethod_Stats	 stats;		        /* Statistics */
	TGeomMethod_Bounds   bounds;		    /* Bounding volume */
	void		         (*user)();		    /* User-defined method */
	void		         (*deleteobj)(GeomRef inObj);
	struct Methods      *(*methods)();		/* object methods func. */
	char		         checkbounds;		/* check bbox before int.? */
	char		         closed;			/* properly closed? */
} Methods;

typedef void (*UserMethodType)();


/*
 * Geom definition
 */
typedef struct Geom {
	char 			*name;			/* Geom name, if any. */
	GeomRef 		obj;			/* Pointer to object info. */
	Methods 		*methods;
	unsigned long 	prims;			/* sum of # primitive objects */
	Float 			bounds[2][3];	/* Bounding box */
	Float 			timenow;		/* Geom's idea of what time it is */
	short int 		animtrans;		/* transformation is animated */
	short int 		frame;			/* frame for which obj is inited */
	struct Surface 	*surf;			/* surface, if any */
	struct Trans 	*trans;			/* Transformation information */
	struct Trans 	*transtail;		/* Double linked list end */
	class  CTexture *texture;		/* Texture mapping info. */
#ifdef SHAREDMEM
	unsigned long *counter;			/* Geoms are shared, counters aren't */
#else
	unsigned long counter;			/* "mailbox" for grid intersection */
#endif
	struct Geom *next;				/* Next object. */
} Geom;

/*
 * Linked list of pointers to objects.
 */
typedef struct GeomList {
	Geom *obj;
	struct GeomList *next;
} GeomList;

/*
 * Array of hit information.  Stores a path through an object DAG,
 * as well as the ray in 'model' (object) space and the distance from
 * the ray origin to the point of intersection.
 */
typedef struct HitNode {
	Geom    *obj;			/* Geom hit */
	Ray	    ray;			/* Ray */
	Float	mindist;		/* Amount of ray to ignore */
	Float	dist;			/* Distance from ray origin to hit */
	short	enter,			/* Enter (TRUE) or Leave (FALSE) obj */
		dotrans;		/* transformations non-identity? */
	Trans	trans;			/* parent-->obj and inverse trans */
} HitNode;

/*
 * Structure holding a list of HitNodes.  A maximum of MAXMODELDEPTH
 * nodes can be referenced.
 */
typedef struct HitList {
	int nodes;
	HitNode data[MAXMODELDEPTH];
} HitList;

extern const char     *GeomName(Geom *obj);

extern Geom	*GeomCreate(GeomRef objptr, Methods *methods);
extern Geom *GeomCopy(Geom *obj);
extern Geom *GeomCopyNamed(char *name);
extern Geom *GeomComputeAggregateBounds(Geom **bounded,Geom* unbounded, Float bounds[2][3]);

/* Frees up the memory used by object inGeom */
extern void GeomDeleteOne(
                Geom*           inGeom);
/* Frees up the memory used by onject inGeom, and all of it's children */
extern void GeomDeleteEvery(
                Geom*           inGeom);

extern GeomList *
        GeomStackPush(Geom *obj, GeomList *list);
extern GeomList *GeomStackPop(GeomList *list);
extern void      GeomComputeBounds(Geom *obj);
extern void 	 AggregatePrintInfo(Geom *obj,FILE *fp);
extern void      IntersectStats();
extern void         PrimUV(
                        Geom                *prim,
                        Vector              *pos,
                        Vector              *norm,
                        Vec2d               *uv,
                        Vector              *dpdu,
                        Vector              *dpdv);
extern int      AggregateConvert(Geom *obj,Geom *objlist);
extern int      intersect(
                            Geom        *obj,       /* Geom to be tested. */  
                            Ray         *ray,       /* Ray origin, direction. */
                            HitList     *hitlist,   /* Intersection path */
                            Float       mindist, 
                            Float       *maxdist);
extern int      TraceRay(
                    Ray             *ray, 
                    HitList         *hitlist, 
                    Float           mindist,
                    Float           *maxdist);	/* application-provided */
extern int      PrimNormal(Geom *prim, Vector *pos,Vector* norm,Vector* gnorm);
extern Methods	*MethodsCreate();

extern void     WorldSetup(void);

extern int      PrimEnter(Geom *obj, Ray *ray, Float mind,Float hitd);
extern int		FirstAnimatedGeom(HitList *hitlist);
extern void		GeomStats(Geom *obj, unsigned long *tests,unsigned long *hits);
extern void		IntersectStats(unsigned long *bvtests);
#endif /* OBJECT_H */

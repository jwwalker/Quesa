/*
 * surfshade.c
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
 * $Id: surfshade.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0.1.2  92/01/10  14:08:28  cek
 * patch3: Fixed additional composition problems.
 * 
 * Revision 4.0.1.1  91/12/13  11:54:51  cek
 * patch3: Fixed ordering of world/model xform composition.
 * patch3: Geometric normal now transformed correctly.
 * 
 * Revision 4.0  91/07/17  14:41:15  kolb
 * Initial version.
 * 
 */
#include "libobj/geom.h"
#include "surface.h"
#include "libtext/CTexture.h"

/*
 * Compute surface properties from given hitlist
 * Returns TRUE if ray is entering object, FALSE otherwise.
 */
int
ComputeSurfProps(
        HitList         *hitlist,   /* Hit information (path through DAG) */
        Ray             *ray,       /* Ray in world space */
        Vector          *pos,       /* Intersection point */
        Vector          *norm,      /* shading normal, geometric normal (return values) */
        Vector          *gnorm,
        Surface         *surf,      /* Copy of surface to use, texture-modified */
        int             *smooth)
{
	HitNode *hp;
	int i;
	Ray rtmp;
	Geom *prim, *obj;
	Float k, kp;
	int texturing, transforming, entering;
	Trans prim2model, world2model;

	hp = hitlist->data;
	prim = hp->obj;

	/*
	 * Compute point of intersection in "primitive space".
	 */
	VecAddScaled(hp->ray.pos, hp->dist, hp->ray.dir, pos);

	/*
	 * Find normal to primitive at point of intersection.
	 */
	*smooth = PrimNormal(prim, pos, norm, gnorm);

	texturing = transforming = FALSE;

	/*
	 * Walk down hit list, constructing world<-->primitive transformation
	 * and determining if we need to perform texture mapping.
	 * The last node is the World node, which cannot be textured or
	 * transformed, so we skip it.
	 */
	for (i = 0, hp = hitlist->data; i < hitlist->nodes -1; hp++, i++) {
		obj = hp->obj;
		texturing = texturing || obj->texture;
		if (hp->dotrans) {
			/*
			 * Here we're actually computing prim2world.
			 * When finished, we invert it.
			 */
			if (transforming) {
				TransCompose(&world2model, &hp->trans,
					&world2model);
			} else {
				TransCopy(&hp->trans, &world2model);
				transforming = TRUE;
			}
		}
	}

	/*
	 * Determine if we're entering or exiting the surface,
	 * flipping surface normals if necessary.
	 */
	k = dotp(&hitlist->data[0].ray.dir, norm);
	if (*smooth) {
		/*
		 * If gnorm and shading norm differ and
		 * their dot products with the ray have
		 * different signs, use the geometric normal
		 * instead, ala Snyder & Barr's paper.
		 */
		kp = dotp(&hitlist->data[0].ray.dir, gnorm);
		if (k <= 0. && kp > 0. || k >= 0. && kp < 0.)
			k = kp;
	}

	if (k > 0.) {
		/* flip normals */
		VecScale(-1., *gnorm, gnorm);
		VecScale(-1., *norm, norm);
		/*
		 * Normal indicates that we're exiting.
		 * Only set entering to TRUE if csg has indicated
		 * that the ray is, indeed, entering.
		 */
		entering = (hitlist->data[0].enter == ENTERING);
	} else {
		/*
		 * Normal indicates that we're entering.
		 * Set entering flag as such unless csg has
		 * told us that we're exiting.
		 */
		entering = !(hitlist->data[0].enter == EXITING);
	}
		
	/*
	 * If there are no transformations, then world2model is identity.
	 */
	if (!transforming)
		TransInit(&world2model);
	/*
	 * If we're not performing texturing, we simply need to compute
	 * the normal and point of intersection to world space.
	 */
	if (!texturing) {
		/*
	 	 * At this point 'world2model' is really 'prim2world'.
		 */
		if (transforming) {
			NormalTransform(norm, &world2model.itrans);
			NormalTransform(gnorm, &world2model.itrans);
			VecAddScaled(ray->pos,
				     hitlist->data[hitlist->nodes -1].dist,
				     ray->dir, pos);
		}
		return entering;
	}
	/*
	 * world2model currently transforms from primitive to world space.
	 * Invert it to get transformation from world to primitive space.
	 */
	TransInvert(&world2model, &world2model);
	TransInit(&prim2model);
	rtmp = hitlist->data[0].ray;
	/*
	 * Walk down hitlist (from primitive up to World object),
	 * transforming hit point and shading normal and applying textures.
	 * Note that the texturing routines want gnorm in object space,
	 * so we don't transform the geometric normal until texturing
	 * is complete.
	 */
	for (hp = hitlist->data, i = 0; i < hitlist->nodes -1; i++, hp++) {
		obj = hp->obj;
		if (hp->dotrans) {
			NormalTransform(norm, &hp->trans.itrans);
			if (texturing) {
				/*
				 * Compose prim<-->model and world<-->model
				 * with current transformation.
				 */
				TransCompose(&prim2model, &hp->trans,
					&prim2model);
				TransCompose(&world2model, &hp->trans,
					&world2model);
				/*
				 * Transform point and ray to model space.
				 */
				PointTransform(pos, &hp->trans.trans);
				(void)RayTransform(&rtmp, &hp->trans.trans);
			}
		}
		/*
		 * Apply textures
		 */
		if (obj->texture)
			obj->texture->TextApply(prim, &rtmp, pos, norm,
				gnorm, surf, &prim2model, &world2model);
	}
	/* Transform geometric normal from object to world space. */
	NormalTransform(gnorm, &world2model.trans);
	return entering;
}

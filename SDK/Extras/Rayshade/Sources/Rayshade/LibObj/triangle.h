/*
 * triangle.h
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
 * $Id: triangle.h,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:39:43  kolb
 * Initial version.
 * 
 */
#ifndef TRIANGLE_H
#define TRIANGLE_H

#define FLATTRI		0
#define PHONGTRI	1

#define GeomTriangleCreate(t,a,b,c,d,e,f,g,h,i,s)  GeomCreate( \
		(GeomRef)TriangleCreate(t,a,b,c,d,e,f,g,h,i,s), TriangleMethods())



/*
 * Triangle
 */
typedef struct {
	Vector	nrm,		/* triangle normal */
		p[3],		/* vertices */
		e[3],		/* "edge" vectors (scaled) */
		*vnorm,		/* Array of vertex normals */
		*dpdu, *dpdv;	/* U and V direction vectors */
	Float	d,		/* plane constant  */
		b[3];		/* Array of barycentric coordinates */
	Vec2d	*uv;		/* Array of UV coordinates of vertices */
	char	index,		/* Flag used for shading/intersection test. */
		type;		/* type (to detect if phong or flat) */
} Triangle;

extern Triangle	*TriangleCreate(
                    int	        type, 
                    Vector	    *p1, 
                    Vector      *p2,
                    Vector      *p3,
                    Vector      *n1,
                    Vector      *n2, 
                    Vector      *n3, 
                    Vec2d       *u1, 
                    Vec2d       *u2,
                    Vec2d       *u3, 
                    int         flipflag);
                    

extern Methods	*TriangleMethods();

#endif /* TRIANGLE_H */

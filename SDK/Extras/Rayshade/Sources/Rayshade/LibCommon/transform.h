/*
 * transform.h
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
 * $Id: transform.h,v 1.1 2002-12-18 18:36:41 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:32:33  kolb
 * Initial version.
 * 
 */
#ifndef TRANSFORM_H
#define TRANSFORM_H

typedef voidstar TransRef;
typedef TransRef TransCreateFunc();

/*
 * Transformation 'matrix'.
 */
typedef struct RSMatrix {
	Float matrix[3][3];		/* Rotation matrix */
	Vector translate;		/* Translation */
} RSMatrix;

typedef void (*TTransMethod_Propagate)(TransRef tr,RSMatrix *trans,RSMatrix *itrans);

typedef struct {
	TransRef	(*create)();		            /* Create it... */
	TTransMethod_Propagate		propagate;		/* Propagate changes to parameters */
} TransMethods;

/* 
 * Transformation structure
 */
typedef struct Trans {
	TransRef tr;			/* transform data */
	TransMethods *methods;		/* transform methods */
	ExprAssoc *assoc;		/* animated parameters */
	short	animated;		/* is the transformation animated? */
	RSMatrix trans,		/* object space --> world space */
		 itrans;	/* worldspace --> object space */
	struct Trans *next, *prev;
} Trans;

extern void MatrixInit(RSMatrix *trans);

extern void     NormalTransform(
                        Vector          *norm, 
                        RSMatrix        *it);
extern void     MatrixMult(
                        RSMatrix        *t1,
                        RSMatrix        *t2,
                        RSMatrix        *res);	
extern void     MatrixCopy(
                        RSMatrix        *from,
                        RSMatrix        *into);
extern void     VecTransform(
                        Vector          *vec, 
                        RSMatrix        *trans);
extern void     PointTransform(
                        Vector          *vec, 
                        RSMatrix        *trans);
         
extern Trans *TransCreate(
    					TransRef            tr, 
    					TransMethods        *meth);
extern void     TransInit(
                        Trans           *trans);
extern void     TransInvert(
                        Trans           *from,
                        Trans           *into);
extern void     TransCopy(
                        Trans           *from,
                        Trans           *into);
extern void     TransCompose(
                        Trans           *t1,
                        Trans           *t2,
                        Trans           *res);

extern RSMatrix	*MatrixCreate();

void		RotationMatrix();
void    		ScaleMatrix();
void CoordSysTransform(Vector*base, Vector*axis, Float ar, Float tlen, Trans *res);
extern void     TranslationMatrix(
                        Float           x,
                        Float           y,
                        Float           z,
                        RSMatrix        *mat);
extern void     ArbitraryMatrix(
                        Float               c1x,
                        Float               c2x,
                        Float               c3x,
                        Float               c1y,
                        Float               c2y,
                        Float               c3y,
                        Float               c1z,
                        Float               c2z,
                        Float               c3z,
                        Float               tx,
                        Float               ty,
                        Float               tz,
                        RSMatrix            *trans);
extern  Float   RayTransform(
                        Ray                 *ray, 
                        RSMatrix            *trans);
extern void     MatrixInvert(
                        RSMatrix            *trans, 
                        RSMatrix            *inverse);
extern void     TransResolveAssoc(
                        Trans               *trans);
extern void     TransComposeList(
                        Trans               *list,
                        Trans               *result);

#endif /* TRANSFORM_H */

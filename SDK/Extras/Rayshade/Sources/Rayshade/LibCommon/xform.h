/*
 * xform.h
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
 * $Id: xform.h,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0.1.1  91/10/04  15:51:33  cek
 * patch1: Initial revision.
 * 
 * Revision 4.0  1991/09/29  15:33:56  cek
 * Initial version.
 *
 */
#ifndef XFORM_H
#define XFORM_H

#define TransXformCreate()	TransCreate((TransRef)XformCreate(), XformMethods())

#define TransXformSetX0(t, v)	TransAssoc(t, &t->trans.matrix[0][0], v)
#define TransXformSetY0(t, v)	TransAssoc(t, &t->trans.matrix[0][1], v)
#define TransXformSetZ0(t, v)	TransAssoc(t, &t->trans.matrix[0][2], v)
#define TransXformSetX1(t, v)	TransAssoc(t, &t->trans.matrix[1][0], v)
#define TransXformSetY1(t, v)	TransAssoc(t, &t->trans.matrix[1][1], v)
#define TransXformSetZ1(t, v)	TransAssoc(t, &t->trans.matrix[1][2], v)
#define TransXformSetX2(t, v)	TransAssoc(t, &t->trans.matrix[2][0], v)
#define TransXformSetY2(t, v)	TransAssoc(t, &t->trans.matrix[2][1], v)
#define TransXformSetZ2(t, v)	TransAssoc(t, &t->trans.matrix[2][2], v)
#define TransXformSetXt(t, v)	TransAssoc(t, &t->trans.translate.x, v)
#define TransXformSetYt(t, v)	TransAssoc(t, &t->trans.translate.y, v)
#define TransXformSetZt(t, v)	TransAssoc(t, &t->trans.translate.z, v)

typedef char Xform;	/* Dummy; Xform has no private data. */

extern Xform *XformCreate();
extern TransMethods *XformMethods();
extern void XformPropagate(TransRef tr, RSMatrix *trans,RSMatrix* itrans);

#endif /* XFORM_H */

/*
 * xform.c
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
 * $Id: xform.cpp,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0.1.1  91/10/04  15:51:53  cek
 * patch1: Initial revision.
 * 
 * Revision 4.0  1991/09/29  15:34:31  cek
 * Initial version.
 *
 */
#include "common.h"
#include "xform.h"

TransMethods *iXformMethods;

/*
 * Create and return reference to an Xform structure.
 * In this case, we return a NULL pointer, as the Xform
 * structure does not hold any data.
 */
Xform *
XformCreate()
{
	return (Xform *)NULL;
}

/*
 * Return a pointer to collection of methods for the
 * Xform transformation.
 */
TransMethods *
XformMethods()
{
	if (iXformMethods == (TransMethods *)NULL) {
		iXformMethods = (TransMethods *)Malloc(sizeof(TransMethods));
		iXformMethods->create = (TransCreateFunc *)XformCreate;
		iXformMethods->propagate = XformPropagate;
	}
	return iXformMethods;	
}

/*
 * Given an Xform structure and forward and inverse transformations,
 * propagate the information in the Xform structure to the
 * transformations.
 * In this case, the information "in" the Xform structure is
 * actually stored in the forward transformation; the Xform
 * points to NULL.
 */
void
XformPropagate(TransRef tr, RSMatrix *trans,RSMatrix* itrans)
{
    Xform *xform = (Xform*)tr;
	/*
	 * The Xform methods change the forward trans
	 * directly, so it's already all set.
	 * Build the inverse...
	 */
	MatrixInvert(trans, itrans);
}

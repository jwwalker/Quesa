/*
 * translate.c
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
 * $Id: translate.cpp,v 1.1 2002-12-18 18:36:41 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:32:42  kolb
 * Initial version.
 * 
 */
#include "common.h"
#include "translate.h"

TransMethods *iTranslateMethods;
void TranslationMatrix();

Translate *
TranslateCreate()
{
	Translate *res;

	res = (Translate *)Malloc(sizeof(Translate));
	res->x = res->y = res->z = 0.;
	return res;
}

TransMethods *
TranslateMethods()
{
	if (iTranslateMethods == (TransMethods *)NULL) {
		iTranslateMethods = (TransMethods *)Malloc(sizeof(TransMethods));
		iTranslateMethods->create = (TransCreateFunc *)TranslateCreate;
		iTranslateMethods->propagate = TranslatePropagate;
	}
	return iTranslateMethods;	
}

void
TranslatePropagate(TransRef tr, RSMatrix *trans,RSMatrix* itrans)
{
    Translate *translate = (Translate*)tr;
	TranslationMatrix(translate->x, translate->y, translate->z, trans);
	/*
	 * Build the inverse...
	 */
	MatrixInit(itrans);
	itrans->translate.x = -translate->x;
	itrans->translate.y = -translate->y;
	itrans->translate.z = -translate->z;
}

void
TranslationMatrix(
    Float           x,
    Float           y,
    Float           z,
    RSMatrix        *mat)
{
	MatrixInit(mat);
	mat->translate.x = x;
	mat->translate.y = y;
	mat->translate.z = z;
}

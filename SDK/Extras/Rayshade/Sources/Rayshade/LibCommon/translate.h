/*
 * translate.h
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
 * $Id: translate.h,v 1.1 2002-12-18 18:36:41 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:32:49  kolb
 * Initial version.
 * 
 */
#ifndef TRANSLATE_H
#define TRANSLATE_H

#define TransTranslateCreate()		TransCreate((TransRef)TranslateCreate(), TranslateMethods())

#define TransTranslateSetX(t, v)		TransAssoc(t, &((Translate *)t->tr)->x, v)
#define TransTranslateSetY(t, v)		TransAssoc(t, &((Translate *)t->tr)->y, v)
#define TransTranslateSetZ(t, v)		TransAssoc(t, &((Translate *)t->tr)->z, v)

typedef Vector Translate;

extern Translate *TranslateCreate();
extern TransMethods *TranslateMethods();
extern void         TranslatePropagate(TransRef tr, RSMatrix *trans,RSMatrix* itrans);

#endif /* TRANSLATE_H */

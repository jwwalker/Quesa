/*
 * gloss.h
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
 * $Id: gloss.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:42:33  kolb
 * Initial version.
 * 
 */
#ifndef GLOSS_H

#include "libcommon/common.h"

#define TextGlossCreate(g)	TextCreate((TextRef) GlossCreate(g), \
					GlossApply)
typedef struct {
	Float glossy;		/* glossiness */
} Gloss;

extern Gloss *GlossCreate(Float glossiness);
extern void GlossApply();

#endif /* GLOSS_H */

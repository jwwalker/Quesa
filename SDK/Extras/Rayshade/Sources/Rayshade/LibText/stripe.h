/*
 * stripe.h
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
 * $Id: stripe.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:43:55  kolb
 * Initial version.
 * 
 */
#ifndef STRIPE_H

#include "libcommon/common.h"
#include "libsurf/surface.h"
#include "mapping.h"

#define TextStripeCreate(s,w,b,m) TextCreate((TextRef)StripeCreate(s,w,b,m), \
					StripeApply)
typedef struct {
	Surface	*surf;
	Mapping	*mapping;
	Float	width,
		bump;
} Stripe;

extern Stripe *
StripeCreate(
    Surface         *surf,
    Float           width,
    Float           bump, 
    Mapping         *mapping);
extern void
StripeApply(
    Stripe          *stripe, 
    Geom            *prim,
    Vector          *ray,
    Vector          *pos,
    Vector          *norm,
    Vector          *gnorm,
    Surface         *surf);

#endif /* STRIPE_H */

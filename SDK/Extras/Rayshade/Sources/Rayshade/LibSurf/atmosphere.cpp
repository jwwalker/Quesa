/*
 * atmosphere.c
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
 * $Id: atmosphere.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:40:02  kolb
 * Initial version.
 * 
 */
#include "atmosphere.h"

Atmosphere *
AtmosCreate(char *data, TAtmosphereMethod   method)
{
	Atmosphere *ef;

	ef = (Atmosphere *)Malloc(sizeof(Atmosphere));
	ef->data = data;
	ef->method = method;
	ef->next = (Atmosphere *)0;
	return ef;
}

Atmosphere *
AtmosphereCopy(
        Atmosphere      *atmos)
{
	Atmosphere *res;

	if (atmos == (Atmosphere *)NULL)
		return (Atmosphere *)NULL;
	res = AtmosCreate(atmos->data, atmos->method);
	res->next = AtmosphereCopy(atmos->next);
	return res;
}

Medium *
MediumPush(
    Float               index,
    Float               statten,
    Medium              *media)
{
	Medium *anew;

	anew = (Medium *)Malloc(sizeof(Medium));
	anew->index = index;
	anew->statten = statten;
	anew->next = media;

	return anew;
}

void
Atmospherics(
        Atmosphere          *effects, 
        Ray                 *ray, 
        Float               dist,
        Vector              *pos, 
        Color               *color)
{
	Atmosphere *etmp;

	for (etmp = effects; etmp; etmp = etmp->next)
		(*etmp->method)(etmp->data, ray, pos, dist, color);
}

Float
ExpAtten(
        Float       dist, 
        Float       trans)
{
	Float atten;

	if (trans < EPSILON)
		return 0.;
	atten = LNHALF * dist / trans;
	return (atten < -10. ? 0. : exp(atten));
}

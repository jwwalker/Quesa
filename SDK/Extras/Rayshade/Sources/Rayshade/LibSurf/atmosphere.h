/*
 * atmosphere.h
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
 * $Id: atmosphere.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:40:06  kolb
 * Initial version.
 * 
 */
#ifndef ATMOSPHERE_H
#define ATMOSPHERE_H

#include "libcommon/common.h"

typedef char * AtmosRef;

typedef struct Medium {
	Float	index,			/* Index of refraction */
		statten;		/* specular transmission attenuation */	
	struct Medium *next;
} Medium;

typedef void (*TAtmosphereMethod)(
                    AtmosRef            data,
                    Ray                 *ray,
                    Vector              *pos,
                    Float               dist,
                    Color               *color);   

typedef struct Atmosphere {
	AtmosRef            data;	/* Effect info */
	TAtmosphereMethod   method;	/* Atmosphere method */
	struct Atmosphere   *next;	/* Next effect */
} Atmosphere;


extern Atmosphere   *AtmosCreate(
                            char                *data, 
                            TAtmosphereMethod   inMethod);
extern Atmosphere   *AtmosphereCopy(
                            Atmosphere          *atmos);
extern Medium *     MediumPush(
                            Float               index,
                            Float               statten,
                            Medium              *media);
extern void         Atmospherics(
                            Atmosphere          *effects, 
                            Ray                 *ray, 
                            Float               dist,
                            Vector              *pos, 
                            Color               *color);
extern Float        ExpAtten(
                            Float               dist, 
                            Float               trans);

#endif /* ATMOSPHERE_H */

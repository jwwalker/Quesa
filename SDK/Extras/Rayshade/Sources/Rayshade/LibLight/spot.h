/*
 * spot.h
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
 * $Id: spot.h,v 1.2 2008-12-21 02:04:29 jwwalker Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 1.1  2002/12/18 18:36:42  pepe
 * First upload
 *
 * Revision 4.0  91/07/17  14:35:50  kolb
 * Initial version.
 * 
 */
#ifndef SPOT_H
#define SPOT_H

#define LightSpotCreate(c,f,t,i,o,at,fo) LightCreate( \
			(LightRef)SpotCreate(f,t,i,o,at,fo), SpotMethods(), c)
typedef struct {
	Vector pos, dir;
	Float hotAngle ;
	Float outerAngle ;
	Float cosHotAngle ;
	Float cosOuterAngle ;
	int	attenuation ;
	int	fallOff ;
} Spotlight;

extern Spotlight *
SpotCreate ( Vector* from , Vector* dir , Float hotAngle , Float  
			outerAngle , int attenuation , int fallOff ) ;
extern LightMethods *SpotMethods(void);
                

#endif /* SPOT_H */

/* 
 * cylinder.h
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
 * $Id: cylinder.h,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:37:18  kolb
 * Initial version.
 * 
 */
#ifndef CYLINDER_H
#define CYLINDER_H

#define GeomCylinderCreate(c,a,r) GeomCreate((GeomRef)CylinderCreate(c,a,r), \
					 CylinderMethods())

/*
 * Cylinder
 */
typedef struct cylinder	{
	Trans trans;
} Cylinder;

extern Cylinder *
CylinderCreate(Float r, Vector *bot,Vector* top);
extern Methods	*CylinderMethods();

#endif /* CYLINDER_H */

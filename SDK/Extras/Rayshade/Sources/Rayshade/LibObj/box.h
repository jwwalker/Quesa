/*
 * box.h
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
 * $Id: box.h,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:36:38  kolb
 * Initial version.
 * 
 */
#ifndef BOX_H
#define BOX_H

#define GeomBoxCreate(a,b) GeomCreate((GeomRef)BoxCreate(a,b), BoxMethods())

/*
 * Box
 */
typedef struct box {
	Float bounds[2][3];
} Box;

extern Box *BoxCreate(Vector *v1,Vector* v2);
extern Methods	*BoxMethods();

#endif /* BOX_H */

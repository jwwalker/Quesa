/*
 * color.h
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
 * $Id: color.h,v 1.1 2002-12-18 18:36:41 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0.1.1  91/09/29  15:32:09  cek
 * patch1: Fixed #endif typo.
 * 
 * Revision 4.0  91/07/17  14:30:08  kolb
 * Initial version.
 * 
 */
#ifndef COLOR_H
#define COLOR_H
/*
 * Color
 */
typedef struct Color {
	Float r, g, b;			/* Red, green, blue. */
} Color;

#define ColorScale(s,c,a)		(a)->r = (s)*(c).r, \
					(a)->g = (s)*(c).g, \
					(a)->b = (s)*(c).b

#define ColorAddScaled(x, s, y, c)	(c)->r = (x).r + (s)*(y).r, \
					(c)->g = (x).g + (s)*(y).g, \
					(c)->b = (x).b + (s)*(y).b

#define ColorMultiply(x,y,a)		(a)->r = (x).r*(y).r, \
					(a)->g = (x).g*(y).g, \
					(a)->b = (x).b*(y).b

#define ColorAdd(x,y,a)			(a)->r = (x).r+(y).r, \
					(a)->g = (x).g+(y).g, \
					(a)->b = (x).b+(y).b
#endif /* COLOR_H */

/*
 * builtin.c
 *
 * Copyright (C) 1989, 1991, Craig E. Kolb, Rod G. Bogart
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
 * $Id: builtin.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:45:00  kolb
 * Initial version.
 * 
 */

#include "rayshade.h"
#include "builtin.h"

Float
SumExpr(Float a,Float b)
{
	return a + b;
}
Float
DiffExpr(Float a,Float b)
{
	return a - b;
}

Float
MultExpr(Float a,Float b)
{
	return a * b;
}
Float
DivideExpr(Float a,Float b)
{
	return a / b;
}

Float
ModExpr(Float a,Float b)
{
	return (Float)((int)a % (int)b);
}

Float
NegateExpr(Float a)
{
	return -a;
}

Float
LinearTime(Float starttime,Float startval,Float endtime,Float endval)
{
	if (TimeExpr->value < starttime)
		return startval;
	if (TimeExpr->value > endtime)
		return endval;
	if (equal(endtime, starttime))
		return startval;
	return startval + (endval - startval) * 
		(TimeExpr->value - starttime) / (endtime - starttime);
}

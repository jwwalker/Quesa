/*
 * builtin.h
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
 * $Id: builtin.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:45:09  kolb
 * Initial version.
 * 
 */
#ifndef BUILTIN_H
#define BUILTIN_H
extern Float SumExpr(Float a,Float b);
extern Float DiffExpr(Float a,Float b);
extern Float MultExpr(Float a,Float b);
extern Float DivideExpr(Float a,Float b);
extern Float ModExpr(Float a,Float b);
extern Float NegateExpr(Float a);
extern Float LinearTime(Float starttime,Float startval,Float endtime,Float endval);

#endif /* BUILTIN_H */

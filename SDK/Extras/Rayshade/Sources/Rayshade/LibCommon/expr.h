/*
 * expr.h
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
 * $Id: expr.h,v 1.1 2002-12-18 18:36:41 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:30:47  kolb
 * Initial version.
 * 
 */
#ifndef EXPR_H
#define EXPR_H

#define FLOAT_EXPR		0
#define BUILTIN_EXPR		1

typedef Float (*TExprFunction_5)(Float a,Float b,Float c,Float d,Float e);
typedef Float (*TExprFunction_4)(Float a,Float b,Float c,Float d);
typedef Float (*TExprFunction_3)(Float a,Float b,Float c);
typedef Float (*TExprFunction_2)(Float a,Float b);
typedef Float (*TExprFunction_1)(Float a);

typedef struct Expr {
	short	type,			         /* Expr type (float/builtin/time) */
		timevary,		            /* does the expr vary over time? */
		symtab,			            /* is the expr in the symtab? */
		nparams;		            /* # of params, if builtin */
	    Float	        value;	    /* float val/last eval if timeexpr */
		Float           timenow;    /* time of last evaluation */
		TExprFunction_2 function;
	    struct Expr     **params;   /* parameters, if a builtin */
} Expr;

typedef struct ExprAssoc {
	Float *lhs;			        /* left-hand side */
	Expr *expr;			        /* right-hand side */
	struct ExprAssoc *next;		/* Next in list */
} ExprAssoc;
	
extern Float ExprEval(Expr *expr);

extern Expr *ExprResolve1(Expr *a, TExprFunction_2   fp, int timevary);
extern Expr *ExprResolve2(Expr *a,Expr *b, TExprFunction_2 fp, int timevary);
extern Expr *ExprResolve3(Expr *a,Expr* b,Expr* c,TExprFunction_2 fp,int timevary);
extern Expr *ExprResolve4(Expr *a,Expr *b,Expr *c,Expr *d,TExprFunction_2 fp, int timevary);
extern Expr *ExprResolve5(Expr *a,Expr *b,Expr *c,Expr *d,Expr* e,TExprFunction_2 fp, int timevary);
extern Expr *ExprFloatCreate(Float val, int timevary);
extern Expr	*TimeExpr, *FrameExpr;
extern Expr *ExprReuseFloatCreate(Float val);
extern ExprAssoc *          AssocCreate(
                                    Float               *lhs,
                                    Expr                *expr,
                                    ExprAssoc           *next);
extern void                 TimeSet(Float time);
extern void                 FrameSet(Float frame);


extern int SolveQuartic(double c[ 5 ], double s[ 4 ]);

#endif /* EXPR_H */

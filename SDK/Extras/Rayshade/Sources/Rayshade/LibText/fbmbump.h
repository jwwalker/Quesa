/*
 * fbmbump.h
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
 * $Id: fbmbump.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:42:24  kolb
 * Initial version.
 * 
 */
#ifndef FBMBUMP_H

#define TextFBmBumpCreate(o,s,h,l,n) TextCreate( \
		(TextRef)FBmBumpCreate(o, s,h,l,n),FBmBumpApply)
extern FBm *        FBmBumpCreate(
                        Float       offset,
                        Float       scale,
                        Float       h,
                        Float       lambda,
                        int         octaves);
extern void FBmBumpApply();

#endif /* FBMBUMP_H */

/*
 * instance.h
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
 * $Id: instance.h,v 1.1 2002-12-18 18:36:42 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:38:30  kolb
 * Initial version.
 * 
 */
#ifndef INSTANCE_H
#define INSTANCE_H

#define GeomInstanceCreate(o)  GeomCreate((GeomRef)InstanceCreate(o), \
					InstanceMethods())

/*
 * Instance object
 */
typedef struct {
	struct Geom *obj;	/* Geom being instantiated */
	Float bounds[2][3];	/* Bounding box of object */
} Instance;

extern char	*InstanceName();
extern int	InstanceConvert();
extern void	InstanceBounds();
extern Instance	*InstanceCreate();
extern Methods	*InstanceMethods();

#endif /* INSTANCE_H */

/*
 * funcdefs.h
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
 * $Id: funcdefs.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0.1.1  91/09/29  15:50:07  cek
 * patch1: Don't declare free or exit void if stdlib.h is used.
 * 
 * Revision 4.0  91/07/17  14:46:11  kolb
 * Initial version.
 * 
 */
#ifndef FUNCDEFS_H
#define FUNCDEFS_H

#ifdef I_STRING
#include <string.h>
#else
#include <strings.h>
#endif

/*
 * Misc. routines.
 */
#ifndef I_STDLIB
extern void	free(), exit();
#endif

extern void get_cpu_time(), read_input_file();

extern void     RSReadInputFile();

#endif /* FUNCDEFS_H */

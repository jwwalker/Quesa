/*
 * memory.c
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
 * $Id: memory.cpp,v 1.1 2002-12-18 18:36:41 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:30:57  kolb
 * Initial version.
 * 
 */
#ifdef SYSV
#include <memory.h>
#endif
#include "common.h"
#include <stdlib.h>
#include <string.h>

unsigned long TotalAllocated;

voidstar
Malloc(unsigned int bytes)
{
	voidstar res;

	TotalAllocated += bytes;

	res = (voidstar)malloc(bytes);
	if (res == (voidstar)NULL)
		RLerror(RL_PANIC,
			"Out of memory trying to allocate %d bytes.\n",bytes);
	return res;
}

void
Free(voidstar  ptr)
{
    free(ptr);    
}

voidstar
Calloc(unsigned int nelem,unsigned int  elen)
{
	voidstar res;

	res = Malloc(nelem*elen);
	bzero(res, (int)nelem*elen);
	return res;
}

void
PrintMemoryStats(FILE *fp)
{
	fprintf(fp,"Total memory allocated:\t\t%lu bytes\n",
			TotalAllocated);
}

/*
 * Allocate space for a string, copy string into space.
 */
char *
strsave(char *s)
{
	char *tmp;

	if (s == (char *)NULL)
		return (char *)NULL;

	tmp = (char *)Malloc((unsigned)strlen(s) + 1);
	(void)strcpy(tmp, s);
	return tmp;
}

#ifdef MULTIMAX

char *
share_calloc(num, siz)
int num;
unsigned int siz;
{
	char *res;

	res = share_malloc(num*siz);
	bzero(res, num*siz);
	return res;
}
#endif

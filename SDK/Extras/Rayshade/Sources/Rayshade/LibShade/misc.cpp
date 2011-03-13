/*
 * misc.c
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
 * $Id: misc.cpp,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0.1.1  92/01/14  18:29:05  cek
 * patch3: Added support for switching cpp on/off.
 * 
 * Revision 4.0  91/07/17  14:46:31  kolb
 * Initial version.
 * 
 */
#include "rayshade.h"
#ifdef RUSAGE
#include <sys/time.h>
#include <sys/resource.h>
#else
#ifdef TIMES
#include <sys/types.h>
#include <sys/times.h>
#include <sys/param.h>
#endif
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "options.h"
#include "stats.h"


Float RSabstmp;	/* Temporary value used by fabs macro.  Ugly. */
static void RSmessage(char *str,char *pat,...);


void
OpenStatsFile()
{
	if (Options.statsname == (char *)NULL || Stats.fstats != stderr)
		return;		/* Not specified or already opened. */

	Stats.fstats = fopen(Options.statsname, "w");
	if (Stats.fstats == (FILE *)NULL) {
		RLerror(RL_PANIC,
			"Cannot open stats file %s.\n", Options.statsname);
	}
}
static void
RLmessagev(const char *type, const char *pat,va_list arg_ptr)
{
#if 0
	extern FILE *yyin;
	extern int yylineno;
	extern char yyfilename[];

	if (yyin) {
		/*
		 * cleanup() hasn't nulled yyin, so line #
		 * info is valid.
		 */
		fprintf(stderr,"%s: %s: %s, line %d: ",
			Options.progname, type,
			yyfilename == (char *)NULL ? "stdin" :
				yyfilename, yylineno);
	} else {
		fprintf(stderr,"%s: %s: ", Options.progname, type);
	}
	vfprintf(stderr, pat, arg_ptr);	
#else
	fprintf(stderr,"%s: %s: ", Options.progname, type);
	vfprintf(stderr, pat, arg_ptr);	
#endif
}
void
RLerror(int level, const char *pat, ...)
{
	va_list arg_ptr=NULL;
	
	va_start(arg_ptr,pat);
	
	switch (level) {
		case RL_ADVISE:
			if (!Options.quiet)
				RLmessagev("Warning", pat, arg_ptr);
			break;
		case RL_WARN:
			RLmessagev("Warning", pat, arg_ptr);
			break;
		case RL_ABORT:
			RLmessagev("Error", pat, arg_ptr);
			exit(1);
			break;
		case RL_PANIC:
			RLmessagev("Fatal error", pat, arg_ptr);
			exit(2);
			break;
		default:
			RLmessagev("Unknown error", pat, arg_ptr);
			exit(3);
	}
}

static void
RSmessage(char *type,char * pat,...)
{
	va_list arg_ptr = NULL;
	
	va_start(arg_ptr,pat);
	RLmessagev(type,pat,arg_ptr);
}
		
#ifdef RUSAGE
void
RSGetCpuTime(usertime, systime)
Float *usertime, *systime;
{
	struct rusage usage;

	getrusage(RUSAGE_SELF, &usage);

	*usertime = (Float)usage.ru_utime.tv_sec +
			(Float)usage.ru_utime.tv_usec / 1000000.;
	*systime = (Float)usage.ru_stime.tv_sec +
			(Float)usage.ru_stime.tv_usec / 1000000.;
}

#else
#ifdef TIMES

void
RSGetCpuTime(usertime, systime)
Float *usertime, *systime;
{
	extern CLOCKTYPE times();
	struct tms time;

	(void)times(&time);
	*usertime = (Float)time.tms_utime / (Float)HZ;
	*systime = (Float)time.tms_stime / (Float)HZ;
}

#else /* !RUSAGE && !TIMES */

void
RSGetCpuTime(Float *usertime,Float *systime)
{
	*usertime = *systime = 0.;
}

#endif /* TIMES */
#endif /* RUSAGE */

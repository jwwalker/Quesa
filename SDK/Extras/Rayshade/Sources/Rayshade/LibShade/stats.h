/*
 * stats.h
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
 * $Id: stats.h,v 1.1 2002-12-18 18:36:43 pepe Exp $
 *
 * $Log: not supported by cvs2svn $
 * Revision 4.0  91/07/17  14:47:46  kolb
 * Initial version.
 * 
 */
#ifndef STATS_H
#define STATS_H

/*
 * Statistics
 */
typedef struct RSStats {
	unsigned long	EyeRays,	/* # of eye rays spawned */
			ShadowRays,	/* # of shadow rays spawned */
			ReflectRays,	/* # of reflected rays */
			RefractRays,	/* # of refracted rays */
			HitRays,	/* # of rays that hit something. */
			BVTests,	/* # of bounding volume tests. */
			SuperSampled,	/* # of supersampled pixels. */
			ShadowHits,	/* # of shadow ray hits */
			CacheHits,	/* # of shadow cache hits */
			CacheMisses;	/* # of shadow cache misses */
	Float		Utime,		/* User time */
			Stime;		/* System time */
	FILE		*fstats;	/* Stats/info file pointer. */
} RSStats;

extern RSStats Stats;
extern void StatsPrint(), StatsAddRep();

extern void VersionPrint();
extern void OpenStatsFile();
extern void RSGetCpuTime(Float *usertime,Float *systime);
extern void ShadowStats(unsigned long *shadowrays,unsigned long * shadowhit,unsigned long* cachehit,unsigned long* cachemiss);
extern void IntersectStats(unsigned long *bvtests);
extern void PrintMemoryStats(FILE *fp);
extern void GeomStats(Geom *obj, unsigned long *tests,unsigned long *hits);
#endif /* STATS_H */

#include "RSPrefix.h"

#include <stdlib.h>
/* config.h
 * This file was produced by running the config.h.SH script, which
 * gets its values from config.sh, which is generally produced by
 * running Configure.
 *
 * Feel free to modify any of this as the need arises.  Note, however,
 * that running config.h.SH again will wipe out any changes you've made.
 * For a more permanent change edit config.sh and rerun config.h.SH.
 */

#define BSD		/**/

#if !__MACH__

/* bzero:
 *	This symbol is maped to memset if the  bzero() routine is not
 *	available to set memory to 0.
 */
#define	bzero(s,l)	memset((s),0,(l))	;	/* mapped to memset */

/* index:
 *	This preprocessor symbol is defined, along with rindex, if the system
 *	uses the strchr and strrchr routines instead.
 */
/* rindex:
 *	This preprocessor symbol is defined, along with index, if the system
 *	uses the strchr and strrchr routines instead.
 */
#define	index strchr	/* cultural */
#define	rindex strrchr	/*  differences? */

#endif

/* MEMSET:
 *	This symbol, if defined, indicates that the memset routine is available
 *	to set blocks of memory. You should always use bzero() instead of
 *	memset() because bzero is remaped to memset if necessary. This means
 *	that a memset() routine must be provided in case MEMSET is not defined
 *	and no bzero() is found.
 */
#define	MEMSET		/**/

/* CLOCKTYPE:
 *	This symbol holds the type returned by times(). It can be long,
 *	or clock_t on BSD sites (in which case <sys/types.h> should be
 *	included). Moreover, the Clock_t symbol is defined in common.h
 *	and should be used for easy clean reference.
 */
/* #define TIMES */		/**/
#define CLOCKTYPE long		/**/

/* I_STRING:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <string.h> (USG systems) instead of <strings.h> (BSD systems).
 */
#define I_STRING		/**/

/* I_SYSRESOURCE:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <sys/resource.h>.
 */
#define	I_SYSRESOURCE		/**/

/* I_SYSTYPES:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <sys/types.h>.
 */
#define	I_SYSTYPES		/**/

/* I_TIME:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <time.h>.
 */
/* I_SYSTIME:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <sys/time.h>.
 */
/* I_SYSTIMEKERNEL:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <sys/time.h> with KERNEL defined.
 */
/*#undef I_TIME		/**/
#define I_SYSTIME		/**/
/*#undef I_SYSTIMEKERNEL		/**/

/* nrand:
 *	This macro is to be used to generate uniformly distributed
 *	random numbers over the range [0., 1.].
 */
/* seednrand:
 *	This symbol defines the macro to be used in seeding the
 *	random number generator (see nrand).
 */
#define nrand()		((double)rand() / (double) 32767)

#define seednrand(x)	srand(x)

/* VOIDFLAGS:
 *	This symbol indicates how much support of the void type is given by this
 *	compiler.  What various bits mean:
 *
 *	    1 = supports declaration of void
 *	    2 = supports arrays of pointers to functions returning void
 *	    4 = supports comparisons between pointers to void functions and
 *		    addresses of void functions
 *	    8 = suports declaration of generic void pointers
 *
 *	The package designer should define VOIDUSED to indicate the requirements
 *	of the package.  This can be done either by #defining VOIDUSED before
 *	including config.h, or by defining defvoidused in Myinit.U.  If the
 *	latter approach is taken, only those flags will be tested.  If the
 *	level of void support necessary is not present, defines void to int.
 */
#define VOIDFLAGS 15
/*
 * ifndef VOIDUSED
 * define VOIDUSED 9
 * endif
 * define VOIDFLAGS 15
 * if (VOIDFLAGS & VOIDUSED) != VOIDUSED
 * define void int		/ is void to be avoided? /
 * define M_VOID		/ Xenix strikes again /
 * endif
 */


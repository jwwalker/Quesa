/* Copyright © Biit Inc 1996-2000 All Rights Reserved */
/*
	Minor changes were made by James W. Walker for the Quesa project:
	
	* To make this compile in plain C mode, reference parameters were
	  changed to pointer parameters.
	
	* When asked for a name further up the stack than was available,
	  GetCallerName was returning the last name that was available.
	  Now it returns a length of zero.
	  
	* For some reason, all the names returned by GetStackCrawl were
	  prefixed with a period, which I have removed.
*/
#include <ConditionalMacros.h>
#include <Memory.h>
#include "StackWalker.h"
//#include "benchmark.h"

#ifdef PROFILING_ON
	#pragma profile on
#endif

#if TARGET_CPU_PPC	/* JWWalker: was GENERATINGPOWERPC, which is obsolete */

typedef struct TracebackTable
	{
	unsigned long   zero;     /*  indicates start of traceback table  */
	unsigned char   version;    /*  version number  */
	unsigned char   lang;     /*  source language (see below) */
	unsigned char   globallink:1,
	        is_eprol:1,
	        has_tboff:1,  /*  1 -> offset from start of proc follows  */
	        int_proc:1,
	        has_ctl:1,
	        tocless:1,
	        fp_present:1,
	        log_abort:1;
	unsigned char   int_hndl:1,
	        name_present:1, /*  1 -> namelen and name of proc follows */
	        uses_alloca:1,  /*  1 -> alloca is used (alloca reg follows)  */
	        cl_dis_inv:3,
	        saves_cr:1,   /*  1 -> prologue saves CR  */
	        saves_lr:1;   /*  1 -> prologue saves LR  */
	unsigned char   stores_bc:1,  /*  1 -> prologue saves SP  */
	        spare2:1,
	        fpr_saved:6;  /*  # of FPRs saved in prologue */
	unsigned char   spare3:2,
	        gpr_saved:6;  /*  # of GPRs saved in prologue */
	unsigned char   fixedparms:8;
	unsigned char   floatparms:7,
	        parmsonstk:1;
	unsigned long   tb_offset;    /*  offset from start of proc to traceback table  */
	short       namelen;    /*  # characters in name  */
	char        name[256];  /*  name string (not nul-terminated!) */
	} TracebackTable;


static asm long* GetSP();

static asm long* GetSP()
	{
	lwz r3, 0(SP)
	blr
	}

void GetCallerName( const char** outName, short* outLength, short steps )
	{
	short tryCount = 1; 
	long* callerStackPtr = GetSP();
	
	while( steps-- && callerStackPtr != 0 && (long*)*callerStackPtr != 0 )
		callerStackPtr = (long*)*callerStackPtr;
	
	if (steps >= 0)	// JWWalker 5/29/2002
	{
		*outLength = 0;
		return;
	}
	
	if ( callerStackPtr != 0 )
		{
		long* returnAddress;
		long *p;
		long i;
FIND_NAME:
		returnAddress = (long*)callerStackPtr[2];
		p = returnAddress;
		for (i=0; i<2000; i++, p++ )
			if ( *p == 0 )
				{
				const TracebackTable*t = (TracebackTable*)p;
				*outLength = t->namelen;
				*outName = t->name;
				break;
				}
		if ( tryCount--==0 && *outLength == 0 )	// no name, try stepping back one more
			{
			callerStackPtr = (long*)*callerStackPtr;
			if ( callerStackPtr )
				goto FIND_NAME;
			}
		}
	}

#else

static asm long GetFP();
static asm long GetFP()
	{
	move.l a6,d0
	rts
	}

inline long GetLongAtAddress(long address, long offset ) 	{ return *(long*)(((char*)address)+offset); }
inline short GetShortAtAddress(long address, long offset ) 	{ return *(short*)(((char*)address)+offset); }
inline unsigned char GetCharAtAddress(long address, long offset ) 	{ return *(unsigned char*)(((char*)address)+offset); }
inline const char* GetAddress(long address, long offset ) 	{ return (const char*)(((char*)address)+offset); }

void GetCallerName( const char** outName, short* outLength, short steps )
	{
	long FP = GetFP();

	while( steps-- )
		FP = GetLongAtAddress( FP, 0 );
	
	long returnAddr3 = GetLongAtAddress( FP, 4 );	// caller
	
	for ( long L=0; L<2000; L+=2 )
		{
		short v = GetShortAtAddress( returnAddr3, L );
		if ( v == 0x4E5E )
			{
			v = GetShortAtAddress( returnAddr3, L+2 );
			if ( v == 0x4E75 )
				{
				unsigned short c = (unsigned short)GetCharAtAddress( returnAddr3, L+4 );	// if this is unsigned char
				if ( c & 0x80 )																// compiler makes bad code here
					{
					if ( c == 0x80 )														// and here
						{
						*outLength = GetCharAtAddress( returnAddr3, L+5 );
						*outName = GetAddress( returnAddr3, L+6 );
						}
					else
						{
						*outLength = c&0x7F;
						*outName = GetAddress( returnAddr3, L+5 );
						}
					break;
					}
				}
			}
		}
	}

#endif

void GetStackCrawl( short depth, char* buffer, long* ioBufferSize )
	{
	long current = 0;
	short i;
	
	for ( i=1; i<=depth; i++ )
		{
		const char* name;
		short length;
		
		GetCallerName( &name, &length, i+1 );
		if ( current+length+1 > *ioBufferSize )
			break;
		if (length <= 0)	// JWWalker 5/30/2002
			break;
		if (*name == '.')	// JWWalker 5/29/2002
		{
			name += 1;
			length -= 1;
		}
		BlockMoveData( name, buffer+current, length );
		buffer[ current+length ] = 13;
		current += length+1;
		}
	*ioBufferSize = current;
	}

#ifdef PROFILING_ON
	#pragma profile reset
#endif

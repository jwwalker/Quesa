/*  NAME:
        E3MacCFMStackCrawl.c

    DESCRIPTION:
        Stack crawl utilities.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
		
		--------------------------------------------------------------------
		
		The implementation is based on code from HeapManager,
		Copyright © Biit Inc 1996-2000 All Rights Reserved.
		
		Permission to use, copy, modify, and distribute this software, in both
		binary and source forms, and together with its documentation, for any
		purpose and without fee is hereby granted, provided that the above
		copyright notice appear in all copies and that both that copyright notice
		and this permission notice appear in supporting documentation, and that
		the name of Biit not be used in advertising or publicity pertaining to
		distribution of this software without specific, written permission.

		BIIT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL
		IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL BIIT
		BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
		DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
		IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
		OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3StackCrawl.h"
#include "QuesaMemory.h"
#include <cstring>
#include <LowMem.h>




//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
//      A stack crawl record is a variable-sized chunk of memory.
//		The nameData field consists of a certain number of NUL-terminated
//		strings.  This is intended to minimize memory usage, at some
//		expense in convenience.
//-----------------------------------------------------------------------------
typedef struct TQ3StackCrawlRec
{
	TQ3Uns16	numberOfNames;
	char		nameData[2];		// actually variable size
} TQ3StackCrawlRec;





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
const TQ3Uns32	kMaxCrawlSteps	= 50;
const TQ3Uns32	kMaxCrawlBuffer	= 2048;





//=============================================================================
//      Internal functions from HeapManager
//-----------------------------------------------------------------------------
//		Notes on changes from the original HeapManager code:
//		
//		*	68K code omitted.
//
//		*	To make this compile in plain C mode, reference parameters were
//	 		changed to pointer parameters.
//	
//		*	When asked for a name further up the stack than was available,
//	  		GetCallerName was returning the last name that was available.
//	  		Now it returns a length of zero.
//	  
//		*	For some reason, all the names returned by GetStackCrawl were
//	 		prefixed with a period, which I have removed.
//
//		*	Further changes by dair, to allow compilation on Windows/Unix.
//
//		*	In GetCallerName, added code to bail out if we walk off the
//			stack into the "weeds".  This is to fix a crash when running
//			in a cooperative thread.
//-----------------------------------------------------------------------------

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

//=============================================================================
//      GetCallerName:	Get one name from the call stack.
//-----------------------------------------------------------------------------
static void GetCallerName( const char** outName, short* outLength, short steps )
	{
	short tryCount = 1; 
	long* callerStackPtr = GetSP();
	
	long*	origStack = callerStackPtr;
	long*	stackBase = (long*)LMGetCurStackBase();
	
	*outLength = 0;	// In case we bail out early
	
	while ( steps-- &&
		callerStackPtr != 0 &&
		(callerStackPtr >= origStack) &&
		(callerStackPtr <= stackBase) &&
		(long*)*callerStackPtr != 0 )
	{
		callerStackPtr = (long*)*callerStackPtr;
	}
	
	if (steps >= 0)
	{
		return;
	}
	if ( (callerStackPtr < origStack) || (callerStackPtr > stackBase) )
	{
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

//=============================================================================
//      GetStackCrawl:	Get names from the call stack.
//-----------------------------------------------------------------------------
static void GetStackCrawl( short depth, char* buffer, long* ioBufferSize )
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
		memcpy( buffer+current, name, length );
		buffer[ current+length ] = 13;
		current += length+1;
		}
	*ioBufferSize = current;
	}


//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3StackCrawl_New:	Create a stack crawl record for the caller of this.
//-----------------------------------------------------------------------------
TQ3StackCrawl
E3StackCrawl_New()
{
	TQ3StackCrawl	theCrawl = NULL;
	
	char*	rawCrawl = (char*) Q3Memory_Allocate( kMaxCrawlBuffer );
	if (rawCrawl != NULL)
	{
		long	rawCrawlSize = kMaxCrawlBuffer;
		char*	crawlStart;
		
		GetStackCrawl( kMaxCrawlSteps, rawCrawl, &rawCrawlSize );
		
		// Allocate the crawl record.
		theCrawl = (TQ3StackCrawl) Q3Memory_Allocate( sizeof(TQ3Uns16) + rawCrawlSize );
		
		if (theCrawl != NULL)
		{
			crawlStart = rawCrawl;
		
			memcpy( theCrawl->nameData, crawlStart, rawCrawlSize );
			
			// Change each 0x0D to 0x00, counting the names as we go.
			crawlStart = theCrawl->nameData;
			theCrawl->numberOfNames = 0;
			while (rawCrawlSize > 0)
			{
				if (*crawlStart == 0x0D)
				{
					*crawlStart = '\0';
					theCrawl->numberOfNames += 1;
				}
				crawlStart += 1;
				rawCrawlSize -= 1;
			}
		}
		
		Q3Memory_Free( &rawCrawl );
	}
	
	return theCrawl;
}





//=============================================================================
//      E3StackCrawl_Count:	Return the number of names in a stack crawl.
//-----------------------------------------------------------------------------
TQ3Uns32
E3StackCrawl_Count( TQ3StackCrawl inCrawl )
{
	TQ3Uns32	theCount = 0;
	
	if (inCrawl != NULL)
	{
		theCount = inCrawl->numberOfNames;
	}
	
	return theCount;
}





//=============================================================================
//      E3StackCrawl_Get:	Return a names in a stack crawl.
//							The index should be at least 0, and less than the
//							result of E3StackCrawl_Count.
//-----------------------------------------------------------------------------
const char*
E3StackCrawl_Get( TQ3StackCrawl inCrawl, TQ3Uns32 inIndex )
{
	const char*	theName = NULL;
	
	if ( inIndex < E3StackCrawl_Count( inCrawl ) )
	{
		const char*	names = inCrawl->nameData;
		TQ3Uns32	index = 0;
		while (index < inIndex)
		{
			names += strlen( names ) + 1;
			index += 1;
		}
		theName = names;
	}
	
	return theName;
}





//=============================================================================
//      E3StackCrawl_Dispose:	Free the memory of a stack crawl.
//-----------------------------------------------------------------------------
void
E3StackCrawl_Dispose( TQ3StackCrawl inCrawl )
{
	Q3Memory_Free( &inCrawl );
}

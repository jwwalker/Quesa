/*  NAME:
        E3StackCrawl.c

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
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3StackCrawl.h"
#include "QuesaMemory.h"
#include <cstring>

#if macintosh
	#include "StackWalker.h"
	#include <ConditionalMacros.h>
#endif

//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct TQ3StackCrawlRec
{
	TQ3Uns16	numberOfNames;
	char		nameData[2];		// actually variable size
} TQ3StackCrawlRec;
//-----------------------------------------------------------------------------
//      A stack crawl record is a variable-sized chunk of memory.
//		The nameData field consists of a certain number of NUL-terminated
//		strings.  This is intended to minimize memory usage, at some
//		expense in convenience.
//-----------------------------------------------------------------------------



//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
const TQ3Uns32	kMaxCrawlSteps	= 50;
const TQ3Uns32	kMaxCrawlBuffer	= 2048;



//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------

//=============================================================================
//      E3StackCrawl_New:	Create a stack crawl record for the caller of this.
//-----------------------------------------------------------------------------
TQ3StackCrawl
E3StackCrawl_New()
{
	TQ3StackCrawl	theCrawl = NULL;
	
#if TARGET_RT_MAC_CFM
	char*	rawCrawl = (char*) Q3Memory_Allocate( kMaxCrawlBuffer );
	if (rawCrawl != NULL)
	{
		long	rawCrawlSize = kMaxCrawlBuffer;
		char*	crawlStart;
		
		GetStackCrawl( kMaxCrawlSteps, rawCrawl, &rawCrawlSize );
		
		// The first name on the crawl should be E3StackCrawl_New, which I want
		// to skip.
		crawlStart = rawCrawl;
		while ( (rawCrawlSize > 0) && (*crawlStart != 0x0D) )
		{
			crawlStart += 1;
			rawCrawlSize -= 1;
		}
		if (*crawlStart == 0x0D)
		{
			crawlStart += 1;
			rawCrawlSize -= 1;
		}
		
		// Allocate the crawl record.
		theCrawl = (TQ3StackCrawl) Q3Memory_Allocate( sizeof(TQ3Uns16) + rawCrawlSize );
		
		if (theCrawl != NULL)
		{
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
#endif
	
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

/*  NAME:
        E3CocoaStackCrawl.c

    DESCRIPTION:
        Stack crawl utilities for leak debugging.
		
		Contrary to the file name, this is not just for Cocoa, but for any Mac
		code.

    COPYRIGHT:
        Copyright (c) 1999-2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
		
    ___________________________________________________________________________
*/
#if Q3_DEBUG
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3StackCrawl.h"
#include "QuesaMemory.h"

#include <execinfo.h>
#include <stdlib.h>




//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#define kMaxCrawlDepth		100		// limit how far up we'll crawl...
#define	kMaxTextLength		500





//=============================================================================
//      Types
//-----------------------------------------------------------------------------

struct TQ3StackCrawlRec
{
	int			numFrames;
	void*		frames[ kMaxCrawlDepth ];
	char**		symbols;
};




//=============================================================================
//      Private functions
//-----------------------------------------------------------------------------



#pragma mark -
//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3StackCrawl_New:	Create a stack crawl record for the caller of this.
//-----------------------------------------------------------------------------
TQ3StackCrawl
E3StackCrawl_New()
{
#ifdef QUESA_NO_STACK_CRAWL
	return nullptr;
#else
	TQ3StackCrawl	theCrawl = nullptr;
	
	try
	{
		theCrawl = new TQ3StackCrawlRec;
		theCrawl->numFrames = backtrace( theCrawl->frames, kMaxCrawlDepth );
		theCrawl->symbols = nullptr;
	}
	catch (...)
	{
	}
	
	return theCrawl;
#endif
}





//=============================================================================
//      E3StackCrawl_Count:	Return the number of names in a stack crawl.
//-----------------------------------------------------------------------------
TQ3Uns32
E3StackCrawl_Count( TQ3StackCrawl inCrawl )
{
	if (inCrawl != nullptr)
	{
		return inCrawl->numFrames;
	}
	else
	{
		return 0;
	}
}





//=============================================================================
//      E3StackCrawl_Get:	Return a name from a stack crawl.
//							The index should be at least 0, and less than the
//							result of E3StackCrawl_Count.
//-----------------------------------------------------------------------------
const char*
E3StackCrawl_Get( TQ3StackCrawl inCrawl, TQ3Uns32 inIndex )
{
	const char*	theName = nullptr;
	
	if ( (inCrawl != nullptr) &&
		E3Num_SafeLess( inIndex, inCrawl->numFrames ) )
	{
		// symbolicate lazily
		if (inCrawl->symbols == nullptr)
			{
			inCrawl->symbols = backtrace_symbols( inCrawl->frames, inCrawl->numFrames );
			}
			
		if (inCrawl->symbols != nullptr)
		{
			theName = inCrawl->symbols[ inIndex ];
		}
	}
	
	return theName;
}





//=============================================================================
//      E3StackCrawl_Dispose:	Free the memory of a stack crawl.
//-----------------------------------------------------------------------------
void
E3StackCrawl_Dispose( TQ3StackCrawl inCrawl )
{
	if (inCrawl != nullptr)
	{
		free( inCrawl->symbols );
	delete inCrawl;
}
}

#endif	// Q3_DEBUG

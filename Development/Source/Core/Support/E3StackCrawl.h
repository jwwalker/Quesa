/*  NAME:
        E3StackCrawl.h

    DESCRIPTION:
        Header file for E3StackCrawl.c.
        
        The leak-checking feature is more useful if it can give a stack crawl,
        showing exactly who allocated the objects that leaked.  The implementation
        of a stack crawl will be very platform-dependent, but should have a
        public API as defined here.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
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
#ifndef E3STACKCRAWL_HDR
#define E3STACKCRAWL_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
/*!
	@typedef	TQ3StackCrawl
	
	@discussion	This is an opaque pointer type holding a list of function names
				of a stack crawl.  Some time in the future, we might want to
				record additional information such as addresses.
*/
typedef struct TQ3StackCrawlRec*	TQ3StackCrawl;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
	@function	E3StackCrawl_New
	@abstract	Create a new stack crawl for the caller of this routine.
	@result		A stack crawl pointer, or NULL if there is an error or if
				stack crawls are not implemented on the current platform.
*/
TQ3StackCrawl	E3StackCrawl_New(void);


/*!
	@function	E3StackCrawl_Count
	@abstract	Count the names recorded in a stack crawl.
	@param		inCrawl		A stack crawl created by <code>E3StackCrawl_New</code>.
	@result		Number of routine names recorded in the stack crawl.
				If inCrawl is NULL, the return value should be 0.
*/
TQ3Uns32		E3StackCrawl_Count( TQ3StackCrawl inCrawl );


/*!
	@function	E3StackCrawl_Get
	@abstract	Get a function name out of a stack crawl.
	@param		inCrawl		A stack crawl created by <code>E3StackCrawl_New</code>.
	@param		inIndex		An index, at least 0 and less than the result of
							<code>E3StackCrawl_Count</code>.
							Index 0 should be the name of the function that called
							<code>E3StackCrawl_New</code>, which in the case of leak
							detection will always be <code>e3root_new</code>.
	@result		A NUL-terminated string, or NULL.
*/
const char*		E3StackCrawl_Get( TQ3StackCrawl inCrawl, TQ3Uns32 inIndex );


/*!
	@function	E3StackCrawl_Dispose
	@abstract	Dispose a stack crawl.
	@param		inCrawl		A stack crawl created by <code>E3StackCrawl_New</code>.
*/
void			E3StackCrawl_Dispose( TQ3StackCrawl inCrawl );





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

/*  NAME:
        E3StackCrawl.h

    DESCRIPTION:
        Header file for E3StackCrawl.c.
        
        The leak-checking feature is more useful if it can give a stack crawl,
        showing exactly who allocated the objects that leaked.  The implementation
        of a stack crawl will be very platform-dependent, but should have a
        public API as defined here.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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

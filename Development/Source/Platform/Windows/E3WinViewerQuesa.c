/*  NAME:
        E3WinViewerQuesa.c

    DESCRIPTION:
        Quesa snippets which are needed by the viewer. It also
		opens the door to integrate the viewer back to Quesa
		(which is perhaps not needed because the small functionality
		of this file is the proof of independence)

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#include "E3Prefix.h"


//=============================================================================
//      E3System_Bottleneck : System bottlebeck point.
//-----------------------------------------------------------------------------
//		Note :	This function is called on virtually every API call, in order
//				to perform any bookkeeping that's required.
//
//				At the present, this simply involves clearing the global error
//				state. For speed, we check the state from outside to see if
//				something has been posted.
//
//				Please profile before making any changes to this routine, as
//				this will be called for every API entry point (so must have
//				minimal overhead).
//-----------------------------------------------------------------------------
void
E3System_Bottleneck(void)
{	
}


//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Memory_Allocate : Allocate an uninitialised block of memory.
//-----------------------------------------------------------------------------
void *
E3Memory_Allocate(TQ3Uns32 theSize)
{	void	*thePtr;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(theSize != 0, NULL);



	// Allocate the memory and a header to hold the size
	thePtr = malloc(theSize);

	return(thePtr);
}


//=============================================================================
//      E3Memory_Free_ : Free a block of memory.
//-----------------------------------------------------------------------------
//		Note :	The trailing underscore is a workaround for over-zealous
//				compilers, which force us to use a macro to add a cast.
//-----------------------------------------------------------------------------
void
E3Memory_Free_(void **thePtr)
{	void		*realPtr;

	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(thePtr));

	// Fetch the pointer, and release it
	realPtr = *thePtr;
	if (realPtr != NULL)
		{
		// Check it looks OK
		Q3_ASSERT_VALID_PTR(realPtr);

		// Free the pointer
		free(realPtr);
		*thePtr = NULL;
		}
}










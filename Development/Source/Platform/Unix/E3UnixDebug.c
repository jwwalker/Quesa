/*  NAME:
        E3UnixDebug.c

    DESCRIPTION:
        Unix debug implementation.

    COPYRIGHT:
        Quesa Copyright � 1999-2002, Quesa Developers.
        
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
#include "E3Debug.h"

#include <stdio.h>





//=============================================================================
//      E3Assert : Report failed assertions.
//-----------------------------------------------------------------------------
void
E3Assert(const char *srcFile, TQ3Uns32 lineNum, const char *theAssertion)
{ 


    // Check our parameters
    if (srcFile == NULL)
    	srcFile = "Unknown";

    if (theAssertion == NULL)
    	theAssertion = "Unknown";



	// Report the assertion failure
	fprintf(stderr, "Quesa Assertion: '%s' failed on line %ld of %s", theAssertion, lineNum, srcFile);
}





//=============================================================================
//      E3IsValidPtr : Check a pointer for validity.
//-----------------------------------------------------------------------------
TQ3Boolean
E3IsValidPtr(void *thePtr)
{


	// Check the pointer
	if (thePtr == NULL)
		return(kQ3False);



	// If we're still here, it looks OK	
	return(kQ3True);
}




/*  NAME:
        E3UnixDebug.c

    DESCRIPTION:
        Unix debug implementation.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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




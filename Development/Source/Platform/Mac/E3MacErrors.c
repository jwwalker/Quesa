/*  NAME:
        E3MacErrors.c

    DESCRIPTION:
        Mac specific Error calls.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#include "E3Extension.h"
#include "E3Errors.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3MacintoshError_Get : Get the oldest and latest Mac error.
//-----------------------------------------------------------------------------
OSErr
E3MacintoshError_Get(OSErr *firstErr)
{	OSErr		theErr;



	// Get the error
	theErr = Q3Error_PlatformGet((TQ3Uns32 *) firstErr);
	
	return(theErr);
}





//=============================================================================
//      E3XMacintoshError_Post : Post an Mac error.
//-----------------------------------------------------------------------------
void
E3XMacintoshError_Post(OSErr theErr)
{


	// Post the error
	Q3Error_PlatformPost((TQ3Uns32) theErr);
}



/*  NAME:
        E3Globals.c

    DESCRIPTION:
        Quesa global state.

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
#include "E3Prefix.h"
#include "E3Globals.h"





//=============================================================================
//      Internal variables
//-----------------------------------------------------------------------------
static TQ3Boolean		gInitialisedGlobals = kQ3False;
static E3Globals		gE3Globals;





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Globals_Get : Get access to the Quesa global state.
//-----------------------------------------------------------------------------
//		Note : The global state is currently stored in a static variable, and
//				its contents are not shared between instance of Quesa.
//-----------------------------------------------------------------------------
E3GlobalsPtr
E3Globals_Get(void)
{


	// If we've not initialised the globals, do so now
	if (!gInitialisedGlobals)
		{
		Q3Memory_Clear(&gE3Globals, sizeof(gE3Globals));
		gInitialisedGlobals = kQ3True;
		}



	// Return the globals
	return(&gE3Globals);
}

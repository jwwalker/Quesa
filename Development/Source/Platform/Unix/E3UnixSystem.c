/*  NAME:
        E3UnixSystem.c

    DESCRIPTION:
        Unix specific routines.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3System.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3UnixSystem_Initialise : Initialise the system.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3UnixSystem_Initialise(void)
{


	// Initialise the system
	return(kQ3Success);
}





//=============================================================================
//      E3UnixSystem_Terminate : Terminate the system.
//-----------------------------------------------------------------------------
void
E3UnixSystem_Terminate(void)
{


	// Terminate the system
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3UnixSystem_LoadPlugins : Scan for and load plug-ins.
//-----------------------------------------------------------------------------
void
E3UnixSystem_LoadPlugins(void)
{
}




//=============================================================================
//      E3UnixSystem_UnloadPlugins : Unload plug-ins.
//-----------------------------------------------------------------------------
void
E3UnixSystem_UnloadPlugins(void)
{
}

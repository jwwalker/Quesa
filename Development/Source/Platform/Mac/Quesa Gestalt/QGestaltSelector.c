/*  NAME:
        QGestaltVersion.c

    DESCRIPTION:
        'gest' resource for Quesa - Version Gestalt selector.

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
#include "E3Version.h"

#include <Gestalt.h>





//=============================================================================
//      Code resource entry point
//-----------------------------------------------------------------------------
pascal OSErr main(OSType theSelector, SInt32 *theResponse)
{


	// Handle the QD3D selectors
	switch (theSelector) {
		case gestaltQD3DVersion:
			*theResponse = (SInt32) kQ3PackedVersion;
			break;

		case gestaltQD3DViewer:
			*theResponse = (1 << gestaltQD3DViewerPresent);
			break;

		case gestaltQD3D:
			*theResponse = (1 << gestaltQD3DPresent);
			break;

		default:
			*theResponse = 0;
			break;
		}
	
	return(noErr);
}

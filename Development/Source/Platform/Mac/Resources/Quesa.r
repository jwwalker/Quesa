/*  NAME:
        Quesa.r

    DESCRIPTION:
        Mac Rez file for Quesa.

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
#ifndef QUESA_REZ
#define QUESA_REZ
//=============================================================================
//		Include files
//-----------------------------------------------------------------------------
#define SystemSevenOrBetter 1			// We want the extended types
#define	SystemSevenOrLater	1			// Types.r uses this variable

#include <Types.r>
#include <SysTypes.r>
#include "E3Version.h"





//=============================================================================
//		Version resources
//-----------------------------------------------------------------------------
// Finder version 1 resource
resource 'vers' (1) {
	kQ3MajorVersion,
	kQ3MinorVersion,
	kQ3ReleaseType,
	kQ3PrereleaseVersion,
	verUS,
	kQ3StringVersion,
	kQ3StringVersion ", Copyright © 1999-2002\nQuesa Developers"
};


// Finder version 2 resource
resource 'vers' (2) {
	kQ3MajorVersion,
	kQ3MinorVersion,
	kQ3ReleaseType,
	kQ3PrereleaseVersion,
	verUS,
	kQ3StringVersion,
	"3D Graphics Library"
};

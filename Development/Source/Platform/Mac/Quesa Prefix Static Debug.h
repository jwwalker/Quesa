/*  NAME:
        Quesa Prefix Static Debug.h

    DESCRIPTION:
        Prefix file for Mac static debug builds.

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
#ifndef QUESA_PREFIX_STATIC_DEBUG_HDR
#define QUESA_PREFIX_STATIC_DEBUG_HDR
//=============================================================================
//      Quesa build constants
//-----------------------------------------------------------------------------
#define QUESA_BUILD_AS_STATIC_LIBRARY	1
#define QUESA_OS_MACINTOSH				1
#define Q3_DEBUG						1
#define Q3_PROFILE						0





//=============================================================================
//      Mac OS build constants
//-----------------------------------------------------------------------------
#define TARGET_API_MAC_CARBON			1



#endif

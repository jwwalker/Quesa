/*  NAME:
        E3WindowsPrefix.h

    DESCRIPTION:
        Windows specific prefix file.

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
#ifndef E3WINDOWSPREFIX_HDR
#define E3WINDOWSPREFIX_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------





//=============================================================================
//      Build constants
//-----------------------------------------------------------------------------
#define QUESA_BUILD_AS_BINARY_COMPATIBLE					1


// tell the sistem if you have to initialize your DIBs used by Quesa as 
//			"top-up" (default in QuickTime, Quesa and QD3D) or
//			"bottom-up" aka "normal" (default in OS/2 and hence in Windows 
//			and in the Win32 implementation of OpenGL)
#ifndef QUESA_USES_NORMAL_DIBs
	#define QUESA_USES_NORMAL_DIBs							0
#endif


#endif

/*  NAME:
        E3WindowsPrefix.h

    DESCRIPTION:
        Windows specific prefix file.

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
#ifndef E3WINDOWSPREFIX_HDR
#define E3WINDOWSPREFIX_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include <Windows.h>





//=============================================================================
//      Universal Headers compatibility
//-----------------------------------------------------------------------------
// If building a DLL, we need to use our own EXTERN_API_C
#ifdef WIN32_EXPORTING
    #include <ConditionalMacros.h>

    #undef  EXTERN_API_C
    #define EXTERN_API_C(_type) __declspec(dllexport) _type __cdecl	 
#endif





//=============================================================================
//      Build constants
//-----------------------------------------------------------------------------
// Assume x86 is always little endian
#define QUESA_HOST_IS_BIG_ENDIAN				0


// We don't support Apple's QD3D renderers under Windows
#define QUESA_REPLACE_QD3D_RENDERERS			1


// Assume we're not to require QuickTime
#define QUESA_SUPPORT_QUICKTIME					0



#endif

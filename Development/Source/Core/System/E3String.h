/*  NAME:
        E3String.h

    DESCRIPTION:
        Header file for E3String.c.

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
#ifndef E3STRING_HDR
#define E3STRING_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3String_RegisterClass(void);
TQ3Status			E3String_UnregisterClass(void);

TQ3ObjectType		E3String_GetType(TQ3StringObject stringObj);
TQ3StringObject		E3CString_New(const char *str);
TQ3Status			E3CString_GetLength(TQ3StringObject stringObj, TQ3Uns32 *length);
TQ3Status			E3CString_SetString(TQ3StringObject stringObj, const char *str);
TQ3Status			E3CString_GetString(TQ3StringObject stringObj, char **str);
TQ3Status			E3CString_EmptyData(char **str);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


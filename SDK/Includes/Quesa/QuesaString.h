/*  NAME:
        QuesaString.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef __QUESA_STRING__
#define __QUESA_STRING__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Constants go here





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Types go here





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Macros go here





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
#if defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON

/*
 *	Q3String_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3String_GetType (
	TQ3StringObject               stringObj
);



/*
 *	Q3CString_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StringObject  )
Q3CString_New (
	const char                    *str
);



/*
 *	Q3CString_GetLength
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CString_GetLength (
	TQ3StringObject               stringObj,
	TQ3Uns32                      *length
);



/*
 *	Q3CString_SetString
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CString_SetString (
	TQ3StringObject               stringObj,
	const char                    *str
);



/*
 *	Q3CString_GetString
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CString_GetString (
	TQ3StringObject               stringObj,
	char                          **str
);



/*
 *	Q3CString_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CString_EmptyData (
	char                          **str
);

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


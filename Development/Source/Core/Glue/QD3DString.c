/*  NAME:
        QD3DString.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
#include "E3String.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3String_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
Q3String_GetType(TQ3StringObject stringObj)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(stringObj->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(stringObj, kQ3SharedTypeString), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on stringObj
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3String_GetType(stringObj));
}





//=============================================================================
//      Q3CString_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StringObject
Q3CString_New(const char *str)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(str), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on str
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CString_New(str));
}





//=============================================================================
//      Q3CString_GetLength : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CString_GetLength(TQ3StringObject stringObj, TQ3Uns32 *length)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(stringObj->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(stringObj, kQ3SharedTypeString), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(length), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on stringObj
		return(kQ3Failure);

	if (0) // Further checks on length
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CString_GetLength(stringObj, length));
}





//=============================================================================
//      Q3CString_SetString : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CString_SetString(TQ3StringObject stringObj, const char *str)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(stringObj->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(stringObj, kQ3SharedTypeString), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(str), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on stringObj
		return(kQ3Failure);

	if (0) // Further checks on str
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CString_SetString(stringObj, str));
}





//=============================================================================
//      Q3CString_GetString : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CString_GetString(TQ3StringObject stringObj, char **str)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(stringObj->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(stringObj, kQ3SharedTypeString), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(str), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on stringObj
		return(kQ3Failure);

	if (0) // Further checks on str
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CString_GetString(stringObj, str));
}





//=============================================================================
//      Q3CString_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CString_EmptyData(char **str)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(str), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on str
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CString_EmptyData(str));
}






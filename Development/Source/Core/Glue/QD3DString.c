/*  NAME:
        QD3DString.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(stringObj, (kQ3SharedTypeString)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(stringObj, (kQ3SharedTypeString)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(stringObj, (kQ3SharedTypeString)), kQ3Failure);
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






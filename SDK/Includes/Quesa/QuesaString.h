/*! @header QuesaString.h
        Declares the Quesa string objects.
 */
/*  NAME:
        QuesaString.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_STRING_HDR
#define QUESA_STRING_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#ifdef __QD3DSTRING__
#error
#endif

#define __QD3DSTRING__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3String_GetType
 *  @discussion
 *      Returns the type of a string object.
 *
 *      Returns kQ3StringTypeCString, or kQ3ObjectTypeInvalid if the
 *		object type is unknown.
 *
 *  @param stringObj        The object to test.
 *  @result                 The type of the object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3String_GetType (
    TQ3StringObject               stringObj
);



/*!
 *  @function
 *      Q3CString_New
 *  @discussion
 *      Create a new C string object.
 *
 *      Creates a string object based on a NULL terminated string. The
 *		string data is copied, and so str can be disposed of after
 *		creating the object.
 *
 *  @param str              The C string to copy.
 *  @result                 The new string object.
 */
Q3_EXTERN_API_C ( TQ3StringObject  )
Q3CString_New (
    const char                    *str
);



/*!
 *  @function
 *      Q3CString_GetLength
 *  @discussion
 *      Get the length of a string object.
 *
 *      Returns the number of bytes required to store the character
 *		data in the string object. The length returned does not
 *		include the terminating NULL byte.
 *
 *  @param stringObj        The object to test.
 *  @param length           The number of bytes needed for character data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CString_GetLength (
    TQ3StringObject               stringObj,
    TQ3Uns32                      *length
);



/*!
 *  @function
 *      Q3CString_SetString
 *  @discussion
 *      Set the character data of a string object.
 *
 *      Assigns a C string to the string object. The string data is copied
 *		and so str can be disposed of after this call.
 *
 *  @param stringObj        The object to update.
 *  @param str              The string to assign to the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CString_SetString (
    TQ3StringObject               stringObj,
    const char                    *str
);



/*!
 *  @function
 *      Q3CString_GetString
 *  @discussion
 *      Return the character data of a string object.
 *
 *      The data returned must be released with a subsequent call to
 *		Q3CString_EmptyData. The data returned will be NULL terminated.
 *
 *		The str parameter is overwritten, and so must not point to an
 *		existing string or a memory leak will occur. If the value of
 *		the str parameter is not NULL, a warning will be posted.
 *
 *  @param stringObj        The object to query.
 *  @param str              Receives a pointer to the character data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CString_GetString (
    TQ3StringObject               stringObj,
    char                          **str
);



/*!
 *  @function
 *      Q3CString_EmptyData
 *  @discussion
 *      Releases the memory allocated by a previous call to
 *		Q3CString_GetString.
 *
 *		After the string data has been freed, the str parameter
 *		will be reset to NULL.
 *
 *  @param str              The string data to release.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CString_EmptyData (
    char                          **str
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



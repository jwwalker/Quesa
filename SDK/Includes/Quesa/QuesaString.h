/*! @header QuesaString.h
        Declares the Quesa string objects.
 */
/*  NAME:
        QuesaString.h

    DESCRIPTION:
        Quesa public header.

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
EXTERN_API_C ( TQ3ObjectType  )
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
EXTERN_API_C ( TQ3StringObject  )
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
EXTERN_API_C ( TQ3Status  )
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
EXTERN_API_C ( TQ3Status  )
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
EXTERN_API_C ( TQ3Status  )
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
EXTERN_API_C ( TQ3Status  )
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



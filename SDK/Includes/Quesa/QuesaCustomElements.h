/*! @header QuesaCustomElements.h
        Declares the Quesa custom elements inherited from QD3D.
 */
/*  NAME:
        QuesaCustomElements.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_CUSTOMELEMENTS_HDR
#define QUESA_CUSTOMELEMENTS_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DCUSTOMELEMENTS__)
#error
#endif

#define __QD3DCUSTOMELEMENTS__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint on

#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Custom element names (as defined by QD3D)
#define CEcNameElementName                      "Apple Computer, Inc.:NameElement"
#define CEcUrlElementName                       "Apple Computer, Inc.:URLElement"
#define CEcWireElementName                      "Apple Computer, Inc.:WireElement"


/*!
 *  @enum
 *      TCEUrlOptions
 *  @discussion
 *      URL options.
 *
 *  @constant kCEUrlOptionNone          No options set.
 *  @constant kCEUrlOptionUseMap        Map option set (meaning unknown).
 */
typedef enum {
    kCEUrlOptionNone                            = 0,
    kCEUrlOptionUseMap                          = 1
} TCEUrlOptions;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
/*!
 *  @struct
 *      TCEUrlData
 *  @discussion
 *      Describes the data for a URL.
 
 *  @field url              C string containing the full (scheme + path) URL.
 *  @field description      Human readable description of the URL.
 *  @field options          Options for the URL.
 */
typedef struct TCEUrlData {
    char                                        *url;
    char                                        *description;
    TCEUrlOptions                               options;
} TCEUrlData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      CENameElement_SetData  
 *  @discussion
 *      Set the name element for an object.
 *
 *      The name parameter should point to a C string, and will be copied
 *      by CENameElement_SetData.
 *
 *  @param object           The object to assign the name to.
 *  @param name             The C string to associate with the object.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
CENameElement_SetData (
    TQ3Object                     object,
    const char                    *name
);



/*!
 *  @function
 *      CENameElement_GetData  
 *  @discussion
 *      Get the name element for an object.
 *
 *      The name parameter will receive a C string, or NULL if no name has
 *      been associated with the object. If a non-NULL pointer is returned,
 *      it must be disposed of with CENameElement_EmptyData.
 *
 *  @param object           The object to query.
 *  @param name             Receives the name associated with the object.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
CENameElement_GetData (
    TQ3Object                     object,
    char                          **name
);



/*!
 *  @function
 *      CENameElement_EmptyData  
 *  @discussion
 *      Release the data returned by CENameElement_GetData.
 *
 *  @param name             A pointer previously returned by CENameElement_GetData.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
CENameElement_EmptyData (
    char                          **name
);



/*!
 *  @function
 *      CEUrlElement_SetData  
 *  @discussion
 *      Set the URL element for an object.
 *
 *      The URL element data will be copied by CEUrlElement_SetData.
 *
 *  @param object           The object to assign the URL element to.
 *  @param urlData          The URL data to associate with the object.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
CEUrlElement_SetData (
    TQ3Object                     object,
    TCEUrlData                    *urlData
);



/*!
 *  @function
 *      CEUrlElement_GetData  
 *  @discussion
 *      Get the URL element for an object.
 *
 *      The urlData parameter will receive a pointer to the URL data, or NULL
 *      if no URL element has been associated with the object. If a non-NULL
 *      pointer is returned, it must be disposed of with CEUrlElement_EmptyData.
 *
 *  @param object           Description of the parameter.
 *  @param urlData          Description of the parameter.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
CEUrlElement_GetData (
    TQ3Object                     object,
    TCEUrlData                    **urlData
);



/*!
 *  @function
 *      CEUrlElement_EmptyData  
 *  @discussion
 *      Release the data returned by CEUrlElement_GetData.
 *
 *  @param urlData          A pointer previously returned by CEUrlElement_GetData.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
CEUrlElement_EmptyData (
    TCEUrlData                    **urlData
);



#if QUESA_SUPPORT_QUICKTIME

/*!
 *  @function
 *      CEWireElement_SetData  
 *  @discussion
 *      Set the QuickTime wired element for an object.
 *
 *  @param object           The object to assign the element to.
 *  @param wireData         The QuickTime wired element to associate with the object.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
CEWireElement_SetData (
    TQ3Object                     object,
    QTAtomContainer               wireData
);



/*!
 *  @function
 *      CEWireElement_GetData  
 *  @discussion
 *      Get the QuickTime wired element for an object.
 *
 *      The wireData parameter will receive a QTAtomContainer, or NULL if no
 *      wired element has been associated with the object. If a non-NULL pointer
 *      is returned, it must be disposed of with CEWireElement_EmptyData.
 *
 *  @param object           The object to query.
 *  @param wireData         Receives the QuickTime wired element associated with the object.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
CEWireElement_GetData (
    TQ3Object                     object,
    QTAtomContainer               *wireData
);



/*!
 *  @function
 *      CEWireElement_EmptyData  
 *  @discussion
 *      Release the data returned by CEWireElement_GetData.
 *
 *  @param wireData         A pointer previously returned by CEWireElement_GetData.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
CEWireElement_EmptyData (
    QTAtomContainer               *wireData
);

#endif // QUESA_SUPPORT_QUICKTIME





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif



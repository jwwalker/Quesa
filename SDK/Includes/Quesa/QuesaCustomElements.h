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
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Custom element names
#define CEcNameElementName                      "Apple Computer, Inc.:NameElement"
#define CEcUrlElementName                       "Apple Computer, Inc.:URLElement"
#define CEcWireElementName                      "Apple Computer, Inc.:WireElement"


// URL options
typedef enum {
    kCEUrlOptionNone                            = 0,
    kCEUrlOptionUseMap                          = 1
} TCEUrlOptions;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// URL type
typedef struct {
    char                                        *url;
    char                                        *description;
    TCEUrlOptions                               options;
} TCEUrlData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      TQ3Status  
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param object           Description of the parameter.
 *  @param name             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
CENameElement_SetData (
    TQ3Object                     object,
    const char                    *name
);



/*!
 *  @function
 *      TQ3Status  
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param object           Description of the parameter.
 *  @param name             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
CENameElement_GetData (
    TQ3Object                     object,
    char                          **name
);



/*!
 *  @function
 *      TQ3Status  
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param name             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
CENameElement_EmptyData (
    char                          **name
);



/*!
 *  @function
 *      TQ3Status  
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param object           Description of the parameter.
 *  @param urlData          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
CEUrlElement_SetData (
    TQ3Object                     object,
    TCEUrlData                    *urlData
);



/*!
 *  @function
 *      TQ3Status  
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param object           Description of the parameter.
 *  @param urlData          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
CEUrlElement_GetData (
    TQ3Object                     object,
    TCEUrlData                    **urlData
);



/*!
 *  @function
 *      TQ3Status  
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param urlData          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
CEUrlElement_EmptyData (
    TCEUrlData                    **urlData
);



#if QUESA_SUPPORT_QUICKTIME

/*!
 *  @function
 *      TQ3Status  
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param object           Description of the parameter.
 *  @param wireData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
CEWireElement_SetData (
    TQ3Object                     object,
    QTAtomContainer               wireData
);



/*!
 *  @function
 *      TQ3Status  
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param object           Description of the parameter.
 *  @param wireData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
CEWireElement_GetData (
    TQ3Object                     object,
    QTAtomContainer               *wireData
);



/*!
 *  @function
 *      TQ3Status  
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param wireData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
CEWireElement_EmptyData (
    QTAtomContainer               *wireData
);

#endif // QUESA_SUPPORT_QUICKTIME





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



/*! @header QuesaSet.h
        Declares the Quesa set objects.
 */
/*  NAME:
        QuesaSet.h

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
#ifndef QUESA_SET_HDR
#define QUESA_SET_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DSET__)
#error
#endif

#define __QD3DSET__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Attribute types
typedef enum {
    kQ3AttributeTypeNone                        = 0,            // n/a
    kQ3AttributeTypeSurfaceUV                   = 1,            // TQ3Param2D
    kQ3AttributeTypeShadingUV                   = 2,            // TQ3Param2D
    kQ3AttributeTypeNormal                      = 3,            // TQ3Vector3D
    kQ3AttributeTypeAmbientCoefficient          = 4,            // float
    kQ3AttributeTypeDiffuseColor                = 5,            // TQ3ColorRGB
    kQ3AttributeTypeSpecularColor               = 6,            // TQ3ColorRGB
    kQ3AttributeTypeSpecularControl             = 7,            // float
    kQ3AttributeTypeTransparencyColor           = 8,            // TQ3ColorRGB
    kQ3AttributeTypeSurfaceTangent              = 9,            // TQ3Tangent2D
    kQ3AttributeTypeHighlightState              = 10,           // TQ3Switch
    kQ3AttributeTypeSurfaceShader               = 11,           // TQ3SurfaceShaderObject
    kQ3AttributeTypeNumTypes                    = 12            // n/a
} TQ3AttributeTypes;


// Element method types
enum {
    kQ3XMethodTypeElementCopyAdd                = Q3_METHOD_TYPE('e', 'c', 'p', 'a'),
    kQ3XMethodTypeElementCopyReplace            = Q3_METHOD_TYPE('e', 'c', 'p', 'r'),
    kQ3XMethodTypeElementCopyGet                = Q3_METHOD_TYPE('e', 'c', 'p', 'g'),
    kQ3XMethodTypeElementCopyDuplicate          = Q3_METHOD_TYPE('e', 'c', 'p', 'd'),
    kQ3XMethodTypeElementDelete                 = Q3_METHOD_TYPE('e', 'd', 'e', 'l')
};


// Attribute method types
enum {
    kQ3XMethodTypeAttributeInherit              = Q3_METHOD_TYPE('i', 'n', 'h', 't'),
    kQ3XMethodTypeAttributeCopyInherit          = Q3_METHOD_TYPE('a', 'c', 'p', 'i'),
    kQ3XMethodTypeAttributeDefault              = Q3_METHOD_TYPE('a', 's', 'd', 'f'),
    kQ3XMethodTypeAttributeIsDefault            = Q3_METHOD_TYPE('a', 'i', 'd', 'f')
};





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Attribute type
typedef TQ3ElementType                          TQ3AttributeType;


// Element methods
typedef CALLBACK_API_C(TQ3Status,           TQ3XElementCopyAddMethod)(
                            const void          *fromAPIElement,
                            void                *toInternalElement);
                            
typedef CALLBACK_API_C(TQ3Status,           TQ3XElementCopyReplaceMethod)(
                            const void          *fromAPIElement,
                            void                *toInternalElement);
                            
typedef CALLBACK_API_C(TQ3Status,           TQ3XElementCopyGetMethod)(
                            const void          *fromAPIElement,
                            void                *toInternalElement);
                            
typedef CALLBACK_API_C(TQ3Status,           TQ3XElementCopyDuplicateMethod)(
                            const void          *fromAPIElement,
                            void                *toInternalElement);
                            
typedef CALLBACK_API_C(TQ3Status,           TQ3XElementDeleteMethod)(
                            void                *internalElement);


// Attribute methods
typedef TQ3Boolean                          TQ3XAttributeInheritMethod;

typedef CALLBACK_API_C(TQ3Status,           TQ3XAttributeCopyInheritMethod)(
                            const void          *fromInternalAttribute,
                            void                *toInternalAttribute);

typedef CALLBACK_API_C(TQ3Status,           TQ3XAttributeDefaultMethod)(
                            void                *internalAttribute);

typedef CALLBACK_API_C(TQ3Boolean,          TQ3XAttributeIsDefaultMethod)(
                            void                *internalAttribute);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Set_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3SetObject  )
Q3Set_New (
    void
);



/*!
 *  @function
 *      Q3Set_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theSet           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Set_GetType (
    TQ3SetObject                  theSet
);



/*!
 *  @function
 *      Q3Set_Add
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theSet           Description of the parameter.
 *  @param theType          Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Set_Add (
    TQ3SetObject                  theSet,
    TQ3ElementType                theType,
    const void                    *data
);



/*!
 *  @function
 *      Q3Set_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theSet           Description of the parameter.
 *  @param theType          Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Set_Get (
    TQ3SetObject                  theSet,
    TQ3ElementType                theType,
    void                          *data
);



/*!
 *  @function
 *      Q3Set_Contains
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theSet           Description of the parameter.
 *  @param theType          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Set_Contains (
    TQ3SetObject                  theSet,
    TQ3ElementType                theType
);



/*!
 *  @function
 *      Q3Set_Clear
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theSet           Description of the parameter.
 *  @param theType          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Set_Clear (
    TQ3SetObject                  theSet,
    TQ3ElementType                theType
);



/*!
 *  @function
 *      Q3Set_Empty
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param target           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Set_Empty (
    TQ3SetObject                  target
);



/*!
 *  @function
 *      Q3Set_GetNextElementType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theSet           Description of the parameter.
 *  @param theType          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Set_GetNextElementType (
    TQ3SetObject                  theSet,
    TQ3ElementType                *theType
);



/*!
 *  @function
 *      Q3Attribute_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param attributeType    Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Attribute_Submit (
    TQ3AttributeType              attributeType,
    const void                    *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3AttributeSet_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3AttributeSet  )
Q3AttributeSet_New (
    void
);



/*!
 *  @function
 *      Q3AttributeSet_Add
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param attributeSet     Description of the parameter.
 *  @param theType          Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Add (
    TQ3AttributeSet               attributeSet,
    TQ3AttributeType              theType,
    const void                    *data
);



/*!
 *  @function
 *      Q3AttributeSet_Contains
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param attributeSet     Description of the parameter.
 *  @param attributeType    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Boolean  )
Q3AttributeSet_Contains (
    TQ3AttributeSet               attributeSet,
    TQ3AttributeType              attributeType
);



/*!
 *  @function
 *      Q3AttributeSet_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param attributeSet     Description of the parameter.
 *  @param theType          Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Get (
    TQ3AttributeSet               attributeSet,
    TQ3AttributeType              theType,
    void                          *data
);



/*!
 *  @function
 *      Q3AttributeSet_Clear
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param attributeSet     Description of the parameter.
 *  @param theType          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Clear (
    TQ3AttributeSet               attributeSet,
    TQ3AttributeType              theType
);



/*!
 *  @function
 *      Q3AttributeSet_Empty
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param target           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Empty (
    TQ3AttributeSet               target
);



/*!
 *  @function
 *      Q3AttributeSet_GetNextAttributeType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param source           Description of the parameter.
 *  @param theType          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_GetNextAttributeType (
    TQ3AttributeSet               source,
    TQ3AttributeType              *theType
);



/*!
 *  @function
 *      Q3AttributeSet_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param attributeSet     Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Submit (
    TQ3AttributeSet               attributeSet,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3AttributeSet_Inherit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param parent           Description of the parameter.
 *  @param child            Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Inherit (
    TQ3AttributeSet               parent,
    TQ3AttributeSet               child,
    TQ3AttributeSet               result
);



/*!
 *  @function
 *      Q3XElementClass_Register
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param elementType      Description of the parameter.
 *  @param name             Description of the parameter.
 *  @param sizeOfElement    Description of the parameter.
 *  @param metaHandler      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3XObjectClass  )
Q3XElementClass_Register (
    TQ3ElementType                *elementType,
    const char                    *name,
    TQ3Uns32                      sizeOfElement,
    TQ3XMetaHandler               metaHandler
);



/*!
 *  @function
 *      Q3XElementType_GetElementSize
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param elementType      Description of the parameter.
 *  @param sizeOfElement    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XElementType_GetElementSize (
    TQ3ElementType                elementType,
    TQ3Uns32                      *sizeOfElement
);



/*!
 *  @function
 *      Q3XAttributeClass_Register
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param attributeType    Description of the parameter.
 *  @param creatorName      Description of the parameter.
 *  @param sizeOfElement    Description of the parameter.
 *  @param metaHandler      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3XObjectClass  )
Q3XAttributeClass_Register (
    TQ3AttributeType              *attributeType,
    const char                    *creatorName,
    TQ3Uns32                      sizeOfElement,
    TQ3XMetaHandler               metaHandler
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



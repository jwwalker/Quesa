/*! @header QuesaSet.h
        Declarations for Quesa sets, elements, and attributes.
 */
/*  NAME:
        QuesaSet.h

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
/*!
 *	@enum
 *		TQ3AttributeTypes
 *	@discussion
 *		Type numbers for standard attributes. See QuesaView.h for default values of some
 *		of these attributes.  For instance you will find that if you do not specify
 *		any attributes, an object will be white, opaque, and somewhat shiny.
 *
 *	@constant	kQ3AttributeTypeNone				No type.  See
 *													<code>Q3AttributeSet_GetNextAttributeType</code>.
 *	@constant	kQ3AttributeTypeSurfaceUV			Surface UV coordinates (<code>TQ3Param2D</code>)
 *													for texture mapping.
 *	@constant	kQ3AttributeTypeShadingUV			Shading UV coordinates (<code>TQ3Param2D</code>)
 *													(originally intended for procedural shaders, but
 *													effectively interchangable with surface UV).
 *	@constant	kQ3AttributeTypeNormal				Normal vector (<code>TQ3Vector3D</code>)
 *	@constant	kQ3AttributeTypeAmbientCoefficient	Ambient coefficient (<code>float</code>)
 *	@constant	kQ3AttributeTypeDiffuseColor		Diffuse color (<code>TQ3ColorRGB</code>)
 *	@constant	kQ3AttributeTypeSpecularColor		Specular color (<code>TQ3ColorRGB</code>)
 *	@constant	kQ3AttributeTypeSpecularControl		Specular control (<code>float</code>).
 *													Larger values make smaller specular highlights.
 *	@constant	kQ3AttributeTypeTransparencyColor	Transparency color (<code>TQ3ColorRGB</code>).
 *													{ 0, 0, 0 } is completely transparent, and
 *													{ 1, 1, 1 } is completely opaque.
 *	@constant	kQ3AttributeTypeSurfaceTangent		Surface tangent (<code>TQ3Tangent2D</code>)
 *	@constant	kQ3AttributeTypeHighlightState		Highlight state (<code>TQ3Switch</code>)
 *	@constant	kQ3AttributeTypeSurfaceShader		Surface shader (<code>TQ3SurfaceShaderObject</code>)
 *	@constant	kQ3AttributeTypeNumTypes			Number of standard attribute types.
*/
typedef enum TQ3AttributeTypes {
    kQ3AttributeTypeNone                        = 0,            // N/A
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
    kQ3AttributeTypeNumTypes                    = 12,           // N/A
    kQ3AttributeTypeSize32                      = 0xFFFFFFFF
} TQ3AttributeTypes;


/*!
 *  @enum
 *      TQ3XAttributeMask
 *  @discussion
 *      Attribute mask flags.
 *
 *  @constant kQ3XAttributeMaskNone                 No attributes.
 *  @constant kQ3XAttributeMaskSurfaceUV            Surface UV attribute mask.
 *  @constant kQ3XAttributeMaskShadingUV            Shading UV attribute mask.
 *  @constant kQ3XAttributeMaskNormal               Normal attribute mask.
 *  @constant kQ3XAttributeMaskAmbientCoefficient   Ambient coefficent attribute mask.
 *  @constant kQ3XAttributeMaskDiffuseColor         Diffuse color attribute mask.
 *  @constant kQ3XAttributeMaskSpecularColor        Specular color attribute mask.
 *  @constant kQ3XAttributeMaskSpecularControl      Specular control attribute mask.
 *  @constant kQ3XAttributeMaskTransparencyColor    Transparency color attribute mask.
 *  @constant kQ3XAttributeMaskSurfaceTangent       Surface tangent attribute mask.
 *  @constant kQ3XAttributeMaskHighlightState       Highlight state attribute mask.
 *  @constant kQ3XAttributeMaskSurfaceShader        Surface shader attribute mask.
 *  @constant kQ3XAttributeMaskCustomAttribute      Custom attribute mask.
 *  @constant kQ3XAttributeMaskAll                  All attributes.
 *  @constant kQ3XAttributeMaskInherited            Inherited attributes mask.
 *  @constant kQ3XAttributeMaskInterpolated         Interpolated attributes mask.
 */
typedef enum TQ3XAttributeMask {
    kQ3XAttributeMaskNone                       = 0,
    kQ3XAttributeMaskSurfaceUV                  = (1 << (kQ3AttributeTypeSurfaceUV          - 1)),
    kQ3XAttributeMaskShadingUV                  = (1 << (kQ3AttributeTypeShadingUV          - 1)),
    kQ3XAttributeMaskNormal                     = (1 << (kQ3AttributeTypeNormal             - 1)),
    kQ3XAttributeMaskAmbientCoefficient         = (1 << (kQ3AttributeTypeAmbientCoefficient - 1)),
    kQ3XAttributeMaskDiffuseColor               = (1 << (kQ3AttributeTypeDiffuseColor       - 1)),
    kQ3XAttributeMaskSpecularColor              = (1 << (kQ3AttributeTypeSpecularColor      - 1)),
    kQ3XAttributeMaskSpecularControl            = (1 << (kQ3AttributeTypeSpecularControl    - 1)),
    kQ3XAttributeMaskTransparencyColor          = (1 << (kQ3AttributeTypeTransparencyColor  - 1)),
    kQ3XAttributeMaskSurfaceTangent             = (1 << (kQ3AttributeTypeSurfaceTangent     - 1)),
    kQ3XAttributeMaskHighlightState             = (1 << (kQ3AttributeTypeHighlightState     - 1)),
    kQ3XAttributeMaskSurfaceShader              = (1 << (kQ3AttributeTypeSurfaceShader      - 1)),
    kQ3XAttributeMaskCustomAttribute            = 0x80000000,
    kQ3XAttributeMaskAll                        = 0x800007FF,
    kQ3XAttributeMaskInherited                  = 0x000003FF,
    kQ3XAttributeMaskInterpolated               = kQ3XAttributeMaskSurfaceUV          |
                                                  kQ3XAttributeMaskShadingUV          |
                                                  kQ3XAttributeMaskNormal             |
                                                  kQ3XAttributeMaskAmbientCoefficient |
                                                  kQ3XAttributeMaskDiffuseColor       |
                                                  kQ3XAttributeMaskSpecularControl    |
                                                  kQ3XAttributeMaskTransparencyColor  |
                                                  kQ3XAttributeMaskSurfaceTangent,
    kQ3XAttributeMaskSize32                     = 0xFFFFFFFF
} TQ3XAttributeMask;


/*!
 *	@enum
 *		Element&nbsp;method&nbsp;types
 *	@discussion
 *		These are method types that apply particularly to custom elements. Note that
 *		a custom element may also need to provide more general custom class methods,
 *		such as <code>kQ3XMethodTypeObjectTraverse</code>.
 *
 *	@constant	kQ3XMethodTypeElementCopyAdd		See <code>TQ3XElementCopyAddMethod</code>.
 *	@constant	kQ3XMethodTypeElementCopyReplace	See <code>TQ3XElementCopyReplaceMethod</code>.
 *	@constant	kQ3XMethodTypeElementCopyGet		See <code>TQ3XElementCopyGetMethod</code>.
 *	@constant	kQ3XMethodTypeElementCopyDuplicate	See <code>TQ3XElementCopyDuplicateMethod</code>.
 *	@constant	kQ3XMethodTypeElementDelete			See <code>TQ3XElementDeleteMethod</code>.
*/
enum {
    kQ3XMethodTypeElementCopyAdd                = Q3_METHOD_TYPE('e', 'c', 'p', 'a'),
    kQ3XMethodTypeElementCopyReplace            = Q3_METHOD_TYPE('e', 'c', 'p', 'r'),
    kQ3XMethodTypeElementCopyGet                = Q3_METHOD_TYPE('e', 'c', 'p', 'g'),
    kQ3XMethodTypeElementCopyDuplicate          = Q3_METHOD_TYPE('e', 'c', 'p', 'd'),
    kQ3XMethodTypeElementDelete                 = Q3_METHOD_TYPE('e', 'd', 'e', 'l')
};


/*!
 *	@enum
 *		Attribute&nbsp;method&nbsp;types
 *	@discussion
 *		These are method types that apply particularly to custom attributes. Note that
 *		a custom element may also need to provide more general custom element or custom class methods,
 *		such as <code>kQ3XMethodTypeObjectTraverse</code>.
 *
 *	@constant	kQ3XMethodTypeAttributeInherit		See <code>TQ3XAttributeInheritMethod</code>.
 *	@constant	kQ3XMethodTypeAttributeCopyInherit	See <code>TQ3XAttributeCopyInheritMethod</code>.
 *	@constant	kQ3XMethodTypeAttributeDefault		See <code>TQ3XAttributeDefaultMethod</code>.
 *	@constant	kQ3XMethodTypeAttributeIsDefault	See <code>TQ3XAttributeIsDefaultMethod</code>.
*/
enum {
    kQ3XMethodTypeAttributeInherit              = Q3_METHOD_TYPE('i', 'n', 'h', 't'),
    kQ3XMethodTypeAttributeCopyInherit          = Q3_METHOD_TYPE('a', 'c', 'p', 'i'),
    kQ3XMethodTypeAttributeDefault              = Q3_METHOD_TYPE('a', 's', 'd', 'f'),
    kQ3XMethodTypeAttributeIsDefault            = Q3_METHOD_TYPE('a', 'i', 'd', 'f')
};





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
/*!
 *	@typedef	TQ3AttributeType
 *	@discussion	This is a 32-bit value identifying a particular attribute.
*/
typedef TQ3ElementType                          TQ3AttributeType;


/*!
 *	@typedef	TQ3XElementCopyAddMethod
 *	@discussion
 *		This method is called to add a new element to a set, as by <code>Q3Set_Add</code> or
 *		<code>Q3Shape_AddElement</code>.  On entry, <code>toInternalElement</code> points to an uninitialized
 *		block of <code>sizeOfElement</code> bytes, where <code>sizeOfElement</code>
 *		is the size you provided when registering the class.
 *
 *	@param	fromAPIElement		Data to be copied, in its external form.
 *	@param	toInternalElement	Destination of the copy, in internal form.
 *	@result						Success or failure of the method.
*/
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XElementCopyAddMethod)(
                            const void          *fromAPIElement,
                            void                *toInternalElement);


/*!
 *	@typedef	TQ3XElementCopyReplaceMethod
 *	@discussion
 *		This method is called to replace an existing element in a set, as by <code>Q3Set_Add</code> or
 *		<code>Q3Shape_AddElement</code>.  On entry, <code>toInternalElement</code> points to existing element data,
 *		which you may need to dispose before replacing by new data.
 *
 *	@param	fromAPIElement		Data to be copied, in its external form.
 *	@param	toInternalElement	Destination of the copy, in internal form.
 *	@result						Success or failure of the method.
*/
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XElementCopyReplaceMethod)(
                            const void          *fromAPIElement,
                            void                *toInternalElement);


/*!
 *	@typedef	TQ3XElementCopyGetMethod
 *	@discussion
 *		This method is called to copy element data out of a set, as by <code>Q3Set_Get</code> or
 *		<code>Q3Shape_GetElement</code>.  If you do not provide this method, the default action is
 *		a memory copy of <code>sizeOfElement</code> bytes.
 *
 *	@param	fromInternalElement	Data to be copied, in its internal form.
 *	@param	toAPIElement		Destination of the copy, in external form.
 *	@result						Success or failure of the method.
*/
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XElementCopyGetMethod)(
                            const void          *fromInternalElement,
                            void                *toAPIElement);


/*!
 *	@typedef	TQ3XElementCopyDuplicateMethod
 *	@discussion
 *		This method is called when <code>Q3Object_Duplicate</code> is used on a set or attribute set. 
 *
 *	@param	fromInternalElement	Data to be copied, in its internal form.
 *	@param	toInternalElement	Destination of the copy, in internal form.
 *	@result						Success or failure of the method.
*/
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XElementCopyDuplicateMethod)(
                            const void          *fromInternalElement,
                            void                *toInternalElement);


/*!
 *	@typedef	TQ3XElementDeleteMethod
 *	@discussion
 *		This method is called to delete element data from a set or attribute set. 
 *
 *	@param	internalElement		Data to be deleted, in its internal form.
 *	@result						Success or failure of the method.
*/
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XElementDeleteMethod)(
                            void                *internalElement);


/*!
 *	@typedef	TQ3XAttributeInheritMethod
 *	@discussion
 *		This is not literally a method.  Your metahandler returns kQ3True or kQ3False to
 *		report whether a custom attribute supports inheritance.
*/
typedef TQ3Boolean                          TQ3XAttributeInheritMethod;


/*!
 *	@typedef	TQ3XAttributeCopyInheritMethod
 *	@discussion	If your custom attribute supports inheritance, this method is called to
 *				copy attribute data from a parent set to a child set.
 *
 *	@param	fromInternalAttribute	Attribute data to copy, in internal form.
 *	@param	toInternalAttribute		Destination of the copy, in internal form.
 *	@result							Success or failure of the method.
*/
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XAttributeCopyInheritMethod)(
                            const void          *fromInternalAttribute,
                            void                *toInternalAttribute);

/*!
 *	@typedef	TQ3XAttributeDefaultMethod
 *	@discussion	This method does not appear to be mentioned in the QuickDraw 3D documentation,
 *				and is not currently used in Quesa.
*/
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XAttributeDefaultMethod)(
                            void                *internalAttribute);


/*!
 *	@typedef	TQ3XAttributeIsDefaultMethod
 *	@discussion	This method does not appear to be mentioned in the QuickDraw 3D documentation,
 *				and is not currently used in Quesa.
*/
typedef Q3_CALLBACK_API_C(TQ3Boolean,          TQ3XAttributeIsDefaultMethod)(
                            void                *internalAttribute);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Set_New
 *  @discussion
 *      Create a new set, initially empty.
 *
 *      Plain sets are less commonly used than attribute sets.
 *
 *  @result                 The new set object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3SetObject  )
Q3Set_New (
    void
);



/*!
 *  @function
 *      Q3Set_GetType
 *  @discussion
 *      Get the type of a set object.
 *
 *      Returns kQ3SetTypeAttribute or kQ3ObjectTypeInvalid.
 *
 *  @param theSet           The set object.
 *  @result                 The subtype of the set.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Set_GetType (
    TQ3SetObject                  theSet
);



/*!
 *  @function
 *      Q3Set_Add
 *  @discussion
 *      Add an element to a set.
 *
 *      This function copies an element into an existing set.  If the set
 *      already contains an element of the given type, it is replaced.
 *		In the case of a custom element, the type is the type that was
 *		returned by <code>Q3XElementClass_Register</code>.
 *
 *  @param theSet           The set object.
 *  @param theType          Type of the element.
 *  @param data             Pointer to the element data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Set_Add (
    TQ3SetObject                  theSet,
    TQ3ElementType                theType,
    const void                    *data
);



/*!
 *  @function
 *      Q3Set_Get
 *  @discussion
 *      Copy the data of an element into a buffer you provide.
 *
 *  @param theSet           The set object.
 *  @param theType          The element type.
 *  @param data             Pointer to a buffer large enough to receive the element data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Set_Get (
    TQ3SetObject                  theSet,
    TQ3ElementType                theType,
    void                          *data
);



/*!
 *  @function
 *      Q3Set_Contains
 *  @discussion
 *      Determine whether a set contains an element of a given type.
 *
 *  @param theSet           The set object.
 *  @param theType          The element type.
 *  @result                 kQ3True if the element exists in the set.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Set_Contains (
    TQ3SetObject                  theSet,
    TQ3ElementType                theType
);



/*!
 *  @function
 *      Q3Set_Clear
 *  @discussion
 *      Remove an element of a given type from a set.
 *
 *      If the set did not contain an element of the specified type,
 *      the function returns kQ3Failure.
 *
 *  @param theSet           The set object.
 *  @param theType          The element type.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Set_Clear (
    TQ3SetObject                  theSet,
    TQ3ElementType                theType
);



/*!
 *  @function
 *      Q3Set_Empty
 *  @discussion
 *      Remove all elements from a set.
 *
 *  @param target          The set object.
 *  @result                Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Set_Empty (
    TQ3SetObject                  target
);



/*!
 *  @function
 *      Q3Set_GetNextElementType
 *  @discussion
 *      Find the next element type in a set.
 *
 *      If you pass <code>kQ3ElementTypeNone</code>, it will return the first element type.
 *		After reaching the last element type, it returns <code>kQ3ElementTypeNone</code>.
 *      Hence, you could iterate through all the element types as follows:
 *
 *		<blockquote><pre><code>
 *		TQ3ElementType	theType = kQ3ElementTypeNone;
 *		while ( Q3Set_GetNextElementType( set, &amp;theType ) &amp;&amp;
 *		&nbsp;	(theType != kQ3ElementTypeNone) )
 *		{
 *		&nbsp;	DoSomething( theType );
 *		}
 *		</code></pre></blockquote>
 *
 *      Search results are undefined if the set is edited while a search is in progress.
 *      Q3Set_GetNextElementType will always return a valid element type or kQ3ElementTypeNone,
 *      however some elements may be skipped if the set is modified before the final
 *      kQ3ElementTypeNone is returned.
 *
 *  @param theSet           The set object.
 *  @param theType          Given an element type, receives the next element type.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Set_GetNextElementType (
    TQ3SetObject                  theSet,
    TQ3ElementType                *theType
);



/*!
 *  @function
 *      Q3Set_CopyElement
 *  @discussion
 *      Copies an element from one set to another.
 *
 *      If the destination set already contains an element of the specified
 *      type, the element will be replaced.
 *
 *		<em>This function is not available in QD3D.</em>
 *
 *  @param sourceSet   		The set containing the element to be copied.
 *  @param theType          Type of the element.
 *  @param destSet          Set to receive the copied element.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS
Q3_EXTERN_API_C ( TQ3Status  )
Q3Set_CopyElement(
	TQ3SetObject				sourceSet,
	TQ3ElementType				theType,
	TQ3SetObject				destSet
);
#endif



/*!
 *  @function
 *      Q3Attribute_Submit
 *  @discussion
 *      Submit an attribute in immediate mode.
 *
 *      This should only be used within a submitting loop.
 *
 *  @param attributeType    The attribute type.
 *  @param data             Pointer to attribute data.
 *  @param view             The view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Attribute_Submit (
    TQ3AttributeType              attributeType,
    const void                    *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3AttributeSet_New
 *  @discussion
 *      Create an attribute set object.
 *
 *  @result                 A new empty attribute set, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3AttributeSet  )
Q3AttributeSet_New (
    void
);



/*!
 *  @function
 *      Q3AttributeSet_Add
 *  @discussion
 *      Add an attribute to an attribute set.
 *
 *      Copy attribute data into an attribute set.  If the set already contains
 *      the specified type of attribute, it is replaced.
 *
 *  @param attributeSet     The attribute set.
 *  @param theType          The attribute type.
 *  @param data             Pointer to the attribute data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Add (
    TQ3AttributeSet               attributeSet,
    TQ3AttributeType              theType,
    const void                    *data
);



/*!
 *  @function
 *      Q3AttributeSet_Contains
 *  @discussion
 *      Determine whether an attribute set contains a specified attribute.
 *
 *  @param attributeSet     The attribute set.
 *  @param attributeType    The attribute type.
 *  @result                 kQ3True if the attribute type exists in the set.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3AttributeSet_Contains (
    TQ3AttributeSet               attributeSet,
    TQ3AttributeType              attributeType
);



/*!
 *  @function
 *      Q3AttributeSet_Get
 *  @discussion
 *      Get attribute data from an attribute set.
 *
 *      Copies attribute data into a user-provided buffer which should be
 *      large enough for the given kind of attribute.
 *
 *  @param attributeSet     The attribute set.
 *  @param theType          The attribute type.
 *  @param data             Pointer to a buffer to receive the attribute data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Get (
    TQ3AttributeSet               attributeSet,
    TQ3AttributeType              theType,
    void                          *data
);



/*!
 *  @function
 *      Q3AttributeSet_Clear
 *  @discussion
 *      Remove an attribute from an attribute set.
 *
 *  @param attributeSet     The attribute set.
 *  @param theType          The attribute type.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Clear (
    TQ3AttributeSet               attributeSet,
    TQ3AttributeType              theType
);



/*!
 *  @function
 *      Q3AttributeSet_Empty
 *  @discussion
 *      Remove all attributes from an attribute set.
 *
 *  @param target           The attribute set.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Empty (
    TQ3AttributeSet               target
);



/*!
 *  @function
 *      Q3AttributeSet_GetNextAttributeType
 *  @discussion
 *      Find the next attribute type in an attribute set.
 *
 *      If you pass <code>kQ3AttributeTypeNone</code>, it will return the first attribute type.
 *      After reaching the last element type, it returns <code>kQ3AttributeTypeNone</code>.
 *		Hence, you could iterate through all the attribute types as follows:
 *
 *		<blockquote><pre><code>
 *		TQ3AttributeType	theType = kQ3AttributeTypeNone;
 *		while ( Q3AttributeSet_GetNextAttributeType( set, &amp;theType ) &amp;&amp;
 *		&nbsp;	(theType != kQ3AttributeTypeNone) )
 *		{
 *		&nbsp;	DoSomething( theType );
 *		}
 *		</code></pre></blockquote>
 *
 *  @param source           The attribute set object.
 *  @param theType          Given an attribute type, receives the next attribute type.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_GetNextAttributeType (
    TQ3AttributeSet               source,
    TQ3AttributeType              *theType
);



/*!
 *  @function
 *      Q3AttributeSet_Submit
 *  @discussion
 *      Submit an attribute set.
 *
 *      This should only be used within a submitting loop.
 *
 *  @param attributeSet     The attribute set object.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Submit (
    TQ3AttributeSet               attributeSet,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3AttributeSet_Inherit
 *  @discussion
 *      Configures an attribute set by copying all attributes from one set
 *		(the child) and then copying any attributes that were not in the
 *		child from another attribute set (the parent).
 *
 *      The set you pass to receive the result must be an existing attribute set,
 *      but any existing attributes in it will be removed.
 *
 *  @param parent           An attribute set.
 *  @param child            An attribute set.
 *  @param result           An attribute set to be redefined.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Inherit (
    TQ3AttributeSet               parent,
    TQ3AttributeSet               child,
    TQ3AttributeSet               result
);



/*!
 *  @function
 *      Q3XElementClass_Register
 *  @discussion
 *      Register a custom element class.
 *
 *		The custom class naming convention is to begin with your company name,
 *		then a colon, then a specific class name.
 *
 *		If your element's data contains indirect data, such as pointers to other
 *		data or references to Quesa objects, your metahandler should provide methods
 *		of the types <code>kQ3XMethodTypeElementCopyAdd</code>, <code>kQ3XMethodTypeElementCopyGet</code>,
 *		<code>kQ3XMethodTypeElementCopyDuplicate</code>, <code>kQ3XMethodTypeElementCopyReplace</code>, and
 *		<code>kQ3XMethodTypeElementDelete</code>.  If you want to be able to write your element
 *		to a file and read it back, your metahandler should define methods of the
 *		types <code>kQ3XMethodTypeObjectTraverse</code>, <code>kQ3XMethodTypeObjectWrite</code>, and
 *		<code>kQ3XMethodTypeObjectReadData</code>.
 *
 *      See QuesaExtension.h for other operations on a custom class, such
 *		as <code>Q3XObjectHierarchy_UnregisterClass</code>.
 *
 *  @param elementType      Receives the element type assigned by Quesa.
 *  @param name             The unique name of the new class.
 *  @param sizeOfElement    Size of the data for the element.
 *  @param metaHandler      Your metahandler function that returns element methods.
 *							You may return NULL for some methods.
 *  @result                 Opaque pointer to the new object class.
 */
Q3_EXTERN_API_C ( TQ3XObjectClass  )
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
 *      Return the size in bytes of an element type.
 *
 *      In the case of a custom element, this would be the same as the size
 *		you provided when you called <code>Q3XElementClass_Register</code>.
 *
 *  @param elementType      An element type.
 *  @param sizeOfElement    Receives the size in bytes of the element.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XElementType_GetElementSize (
    TQ3ElementType                elementType,
    TQ3Uns32                      *sizeOfElement
);



/*!
 *  @function
 *      Q3XAttributeClass_Register
 *  @discussion
 *      Register a custom attribute class.
 *
 *  @param attributeType    Receives the new attribute type.
 *  @param className      	Unique name for the new attribute.
 *  @param sizeOfElement    Size in bytes of the attribute data.
 *  @param metaHandler      Your metahandler, providing appropriate methods.
 *							It may return NULL in some cases.
 *  @result                 Pointer to the new class.
 */
Q3_EXTERN_API_C ( TQ3XObjectClass  )
Q3XAttributeClass_Register (
    TQ3AttributeType              *attributeType,
    const char                    *className,
    TQ3Uns32                      sizeOfElement,
    TQ3XMetaHandler               metaHandler
);



/*!
 *  @function
 *      Q3XAttributeSet_GetPointer
 *  @discussion
 *      Get a pointer to the internal data structure for an attribute.
 *
 *      For attributes of type kQ3AttributeTypeXXX, the internal attribute data
 *      is currently identical to the data structured passed to Q3AttributeSet_Add.
 *
 *      This function should only be called from renderer plug-ins.
 *
 *  @param attributeSet     The attribute set to query.
 *  @param attributeType    The attribute type to locate.
 *  @result                 A pointer to the internal attribute data if present, or NULL.
 */
Q3_EXTERN_API_C ( void * )
Q3XAttributeSet_GetPointer (
    TQ3AttributeSet               attributeSet,
    TQ3AttributeType              attributeType
);



/*!
 *  @function
 *      Q3XAttributeSet_GetMask
 *  @discussion
 *      Get a mask of the attributes contained in an attribute set.
 *
 *      This function should only be called from renderer plug-ins.
 *
 *  @param attributeSet     The attribute set to query.
 *  @result                 A mask indicating the attributes present in attributeSet.
 */
Q3_EXTERN_API_C ( TQ3XAttributeMask  )
Q3XAttributeSet_GetMask (
    TQ3AttributeSet               attributeSet
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



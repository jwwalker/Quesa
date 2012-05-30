/*  NAME:
        E3Set.h

    DESCRIPTION:
        Header file for E3Set.c.

    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

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
#ifndef E3SET_HDR
#define E3SET_HDR
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
//      Internal types
//-----------------------------------------------------------------------------



class E3ElementInfo : public E3Root
	{
	const TQ3XElementCopyAddMethod			elementCopyAddMethod ;
	const TQ3XElementCopyReplaceMethod		elementCopyReplaceMethod ;
	const TQ3XElementCopyGetMethod			elementCopyGetMethod ;
	const TQ3XElementCopyDuplicateMethod	elementCopyDuplicateMethod ;	
	const TQ3XElementDeleteMethod			elementDeleteMethod ;
		
public :

									E3ElementInfo	(
													TQ3XMetaHandler	newClassMetaHandler,
													E3ClassInfo*	newParent
					 								) ; // constructor	
	friend class E3Set ;
	friend class OpaqueTQ3Object ;
	} ;




class E3Element : public OpaqueTQ3Object
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeElement, E3Element, OpaqueTQ3Object )

	// There is no extra data for this class

public :
	} ;
	


class E3Attribute : public E3Element
	{
Q3_CLASS_ENUMS ( kQ3ElementTypeAttribute, E3Attribute, E3Element )

	// There is no extra data for this class
	
	public :
	} ;
	


// Set attribute data
typedef struct TQ3SetAttributes {
	TQ3Vector3D			normal;
	TQ3Switch			highlightState;
	TQ3Tangent2D		surfaceTangent;
	TQ3Param2D 			surfaceUV;
	TQ3ColorRGB			diffuseColor;
	float				ambientCoeficient;
	TQ3ColorRGB			specularColor;
	float				specularControl;
	TQ3Param2D 			shadingUV;
	TQ3SurfaceShaderObject surfaceShader;
	TQ3ColorRGB			transparencyColor;
	TQ3ColorRGB			emissiveColor;
} TQ3SetAttributes;



// Set instance data
typedef struct TQ3SetData {
	TQ3SetAttributes	attributes;			// Data for built-in attributes
	E3HashTablePtr		theTable;			// Elements in set, keyed by type
	TQ3Uns32			scanEditIndex;		// Set edit index while scanning
	TQ3Uns32			scanCount;			// Size of scanResults
	TQ3Uns32			scanIndex;			// Current index into scanResults
	TQ3ElementType		*scanResults;		// Scan results
	TQ3XAttributeMask	theMask;			// Attribute mask
} TQ3SetData;



class E3Set : public E3Shared
	{
Q3_CLASS_ENUMS ( kQ3SharedTypeSet, E3Set, E3Shared )

	TQ3SetData				setData ;

public :

	static TQ3Boolean		IsOfMyClass ( TQ3Object object ) ;

	TQ3ElementObject		FindElement ( TQ3ElementType theType ) ;
	TQ3ObjectType			GetType ( void ) ;
	TQ3Status				Add ( TQ3ElementType theType, const void *data ) ;
	TQ3Status				Get ( TQ3ElementType theType, void *data ) ;
	TQ3Boolean				Contains ( TQ3ElementType theType ) ;
	TQ3Status				Clear ( TQ3ElementType theType ) ;
	TQ3Status				Empty ( void ) ;
	TQ3Status				GetNextElementType ( TQ3ElementType *theType ) ;
	TQ3Status				CopyElement ( TQ3ElementType theType, TQ3SetObject destSet ) ;
	TQ3Status				SubmitElements ( TQ3ViewObject inView ) ;


	friend TQ3Status E3AttributeSet_GetNextAttributeType ( TQ3AttributeSet theSet, TQ3AttributeType *theType ) ;
	friend TQ3Status E3AttributeSet_Inherit ( TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet result ) ;
	friend void * E3XAttributeSet_GetPointer ( TQ3AttributeSet attributeSet, TQ3AttributeType attributeType ) ;
	friend TQ3XAttributeMask E3XAttributeSet_GetMask ( TQ3AttributeSet attributeSet ) ;
	} ;


//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------

TQ3Status			E3Set_RegisterClass(void);
TQ3Status			E3Set_UnregisterClass(void);

TQ3SetObject		E3Set_New(void);

TQ3Status			E3AttributeSet_GetNextAttributeType(TQ3AttributeSet theSet, TQ3AttributeType *theType);
TQ3AttributeType	E3Attribute_ClassToAttributeType(TQ3ObjectType theType);
TQ3ObjectType		E3Attribute_AttributeToClassType(TQ3AttributeType theType);
TQ3Status			E3Attribute_Submit(TQ3AttributeType attributeType, const void *attributeData, TQ3ViewObject theView);
TQ3Status			E3AttributeSet_Submit(TQ3AttributeSet theSet, TQ3ViewObject theView);
TQ3AttributeSet		E3AttributeSet_New(void);
TQ3Status			E3AttributeSet_Inherit(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet result);

TQ3XObjectClass		E3XElementClass_Register(TQ3ElementType *elementType, const char *name, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler);
TQ3Status			E3XElementType_GetElementSize(TQ3ElementType elementType, TQ3Uns32 *sizeOfElement);
TQ3XObjectClass		E3XAttributeClass_Register(TQ3AttributeType *attributeType, const char *creatorName, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler);
void				*E3XAttributeSet_GetPointer(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType);
TQ3XAttributeMask	E3XAttributeSet_GetMask(TQ3AttributeSet attributeSet);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


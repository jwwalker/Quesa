/*  NAME:
        QD3DSet.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Copyright (c) 1999-2014, Quesa Developers. All rights reserved.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Set.h"
#include "E3View.h"





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
//      Q3Set_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3SetObject
Q3Set_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_New());
}





//=============================================================================
//      Q3Set_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3Set_GetType(TQ3SetObject theSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Set::IsOfMyClass ( theSet ), kQ3ObjectTypeInvalid);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) theSet )->GetType () ;
}





//=============================================================================
//      Q3Set_Add : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Set_Add(TQ3SetObject theSet, TQ3ElementType theType, const void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Set::IsOfMyClass ( theSet ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3ClassTree::GetClass ( E3Attribute_AttributeToClassType ( theType ) )->IsType (
		kQ3ObjectTypeElement ), kQ3Failure ) ;



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) theSet )->Add ( theType, data ) ;
}





//=============================================================================
//      Q3Set_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Set_Get(TQ3SetObject theSet, TQ3ElementType theType, void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Set::IsOfMyClass ( theSet ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) theSet )->Get ( theType, data ) ;
}





//=============================================================================
//      Q3Set_Contains : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Set_Contains(TQ3SetObject theSet, TQ3ElementType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Set::IsOfMyClass ( theSet ), kQ3False);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) theSet )->Contains ( theType ) ;
}





//=============================================================================
//      Q3Set_Clear : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Set_Clear(TQ3SetObject theSet, TQ3ElementType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Set::IsOfMyClass ( theSet ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) theSet )->Clear ( theType ) ;
}





//=============================================================================
//      Q3Set_Empty : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Set_Empty(TQ3SetObject target)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Set::IsOfMyClass ( target ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) target )->Empty () ;
}





//=============================================================================
//      Q3Set_GetNextElementType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Set_GetNextElementType(TQ3SetObject theSet, TQ3ElementType *theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Set::IsOfMyClass ( theSet ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theType), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) theSet )->GetNextElementType ( theType ) ;
}





//=============================================================================
//      Q3Set_CopyElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Set_CopyElement(
	TQ3SetObject				sourceSet,
	TQ3ElementType				theType,
	TQ3SetObject				destSet
)
{

	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Set::IsOfMyClass ( sourceSet ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3Set::IsOfMyClass ( destSet ), kQ3Failure);



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) sourceSet )->CopyElement ( theType, destSet ) ;
}



#pragma mark -

//=============================================================================
//      Q3Attribute_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Attribute_Submit(TQ3AttributeType attributeType, const void *data, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Attribute_Submit(attributeType, data, view));
}





//=============================================================================
//      Q3AttributeSet_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3AttributeSet
Q3AttributeSet_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3AttributeSet_New());
}





//=============================================================================
//      Q3AttributeSet_Add : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_Add(TQ3AttributeSet attributeSet, TQ3AttributeType theType, const void *data)
	{
	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( Q3Object_IsType( attributeSet, kQ3SetTypeAttribute ), kQ3Failure );
	TQ3ObjectType classType = E3Attribute_AttributeToClassType ( theType ) ;
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( classType ) ;
	Q3_REQUIRE_OR_RESULT( theClass->IsType ( kQ3ElementTypeAttribute ), kQ3Failure ) ;


	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) attributeSet )->Add ( theType, data ) ;
}





//=============================================================================
//      Q3AttributeSet_Contains : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3AttributeSet_Contains(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( Q3Object_IsType( attributeSet, kQ3SetTypeAttribute ), kQ3False );



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) attributeSet )->Contains ( attributeType ) ;
}





//=============================================================================
//      Q3AttributeSet_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_Get(TQ3AttributeSet attributeSet, TQ3AttributeType theType, void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( Q3Object_IsType( attributeSet, kQ3SetTypeAttribute ), kQ3Failure );



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) attributeSet )->Get ( theType, data ) ;
}





//=============================================================================
//      Q3AttributeSet_Clear : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_Clear(TQ3AttributeSet attributeSet, TQ3AttributeType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( Q3Object_IsType( attributeSet, kQ3SetTypeAttribute ), kQ3Failure );



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) attributeSet )->Clear ( theType ) ;
}





//=============================================================================
//      Q3AttributeSet_Empty : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_Empty(TQ3AttributeSet target)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( Q3Object_IsType( target, kQ3SetTypeAttribute ), kQ3Failure );



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Set*) target )->Empty () ;
}





//=============================================================================
//      Q3AttributeSet_GetNextAttributeType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_GetNextAttributeType(TQ3AttributeSet source, TQ3AttributeType *theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theType), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( Q3Object_IsType( source, kQ3SetTypeAttribute ), kQ3Failure );



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3AttributeSet_GetNextAttributeType(source, theType));
}





//=============================================================================
//      Q3AttributeSet_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_Submit(TQ3AttributeSet attributeSet, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( Q3Object_IsType( attributeSet, kQ3SetTypeAttribute ), kQ3Failure );
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3AttributeSet_Submit(attributeSet, view));
}





//=============================================================================
//      Q3AttributeSet_Inherit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_Inherit(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( Q3Object_IsType( parent, kQ3SetTypeAttribute ), kQ3Failure );
	Q3_REQUIRE_OR_RESULT( Q3Object_IsType( child, kQ3SetTypeAttribute ), kQ3Failure );
	Q3_REQUIRE_OR_RESULT( Q3Object_IsType( result, kQ3SetTypeAttribute ), kQ3Failure );



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3AttributeSet_Inherit(parent, child, result));
}



#pragma mark -

//=============================================================================
//      Q3XElementClass_Register : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3XObjectClass
Q3XElementClass_Register(TQ3ElementType *elementType, const char *name, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(elementType), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(name), NULL);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XElementClass_Register(elementType, name, sizeOfElement, metaHandler));
}





//=============================================================================
//      Q3XElementType_GetElementSize : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XElementType_GetElementSize(TQ3ElementType elementType, TQ3Uns32 *sizeOfElement)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(sizeOfElement), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XElementType_GetElementSize(elementType, sizeOfElement));
}






//=============================================================================
//      Q3XAttributeClass_Register : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3XObjectClass
Q3XAttributeClass_Register(TQ3AttributeType *attributeType, const char *creatorName, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeType), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(creatorName), NULL);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XAttributeClass_Register(attributeType, creatorName, sizeOfElement, metaHandler));
}





//=============================================================================
//      Q3XAttributeSet_GetPointer : Quesa API entry point.
//-----------------------------------------------------------------------------
void *
Q3XAttributeSet_GetPointer(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType)
{


	// Release build checks



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XAttributeSet_GetPointer(attributeSet, attributeType));
}





//=============================================================================
//      Q3XAttributeSet_GetMask : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3XAttributeMask
Q3XAttributeSet_GetMask(TQ3AttributeSet attributeSet)
{


	// Release build checks
	if(attributeSet == NULL)
		return(kQ3XAttributeMaskNone);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XAttributeSet_GetMask(attributeSet));
}


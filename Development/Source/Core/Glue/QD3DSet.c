/*  NAME:
        QD3DSet.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#include "E3Set.h"





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
	Q3_REQUIRE_OR_RESULT(theSet->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theSet, kQ3SharedTypeSet), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSet
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_GetType(theSet));
}





//=============================================================================
//      Q3Set_Add : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Set_Add(TQ3SetObject theSet, TQ3ElementType theType, const void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(theSet->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theSet, kQ3SharedTypeSet), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3ClassTree_IsType( E3ClassTree_GetClassByType( theType ),
		kQ3ObjectTypeElement ), kQ3Failure );



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSet
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_Add(theSet, theType, data));
}





//=============================================================================
//      Q3Set_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Set_Get(TQ3SetObject theSet, TQ3ElementType theType, void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(theSet->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theSet, kQ3SharedTypeSet), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSet
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_Get(theSet, theType, data));
}





//=============================================================================
//      Q3Set_Contains : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Set_Contains(TQ3SetObject theSet, TQ3ElementType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(theSet->quesaTag == kQ3ObjectTypeQuesa, kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theSet, kQ3SharedTypeSet), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSet
		return(kQ3False);

	if (0) // Further checks on theType
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_Contains(theSet, theType));
}





//=============================================================================
//      Q3Set_Clear : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Set_Clear(TQ3SetObject theSet, TQ3ElementType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(theSet->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theSet, kQ3SharedTypeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSet
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_Clear(theSet, theType));
}





//=============================================================================
//      Q3Set_Empty : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Set_Empty(TQ3SetObject target)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(target->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(target, kQ3SharedTypeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on target
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_Empty(target));
}





//=============================================================================
//      Q3Set_GetNextElementType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Set_GetNextElementType(TQ3SetObject theSet, TQ3ElementType *theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(theSet->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theSet, kQ3SharedTypeSet), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theType), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSet
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_GetNextElementType(theSet, theType));
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
	Q3_REQUIRE_OR_RESULT(sourceSet->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(sourceSet, kQ3SharedTypeSet), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(destSet->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(destSet, kQ3SharedTypeSet), kQ3Failure);



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_CopyElement(sourceSet, theType, destSet));
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
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on attributeType
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



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
	TQ3ObjectType	classType;
	E3ClassInfoPtr	theClass;


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	classType = E3Attribute_AttributeToClassType( theType );
	theClass = E3ClassTree_GetClassByType( classType );
	Q3_REQUIRE_OR_RESULT( E3ClassTree_IsType( theClass, kQ3ElementTypeAttribute ),
		kQ3Failure );


	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on attributeSet
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_Add(attributeSet, theType, data));
}





//=============================================================================
//      Q3AttributeSet_Contains : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3AttributeSet_Contains(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on attributeSet
		return(kQ3False);

	if (0) // Further checks on attributeType
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_Contains(attributeSet, attributeType));
}





//=============================================================================
//      Q3AttributeSet_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_Get(TQ3AttributeSet attributeSet, TQ3AttributeType theType, void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on attributeSet
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_Get(attributeSet, theType, data));
}





//=============================================================================
//      Q3AttributeSet_Clear : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_Clear(TQ3AttributeSet attributeSet, TQ3AttributeType theType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on attributeSet
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_Clear(attributeSet, theType));
}





//=============================================================================
//      Q3AttributeSet_Empty : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_Empty(TQ3AttributeSet target)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on target
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_Empty(target));
}





//=============================================================================
//      Q3AttributeSet_GetNextAttributeType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_GetNextAttributeType(TQ3AttributeSet source, TQ3AttributeType *theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theType), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on source
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_GetNextElementType(source, theType));
}





//=============================================================================
//      Q3AttributeSet_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_Submit(TQ3AttributeSet attributeSet, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on attributeSet
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Set_SubmitElements(attributeSet, view));
}





//=============================================================================
//      Q3AttributeSet_Inherit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3AttributeSet_Inherit(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet result)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on parent
		return(kQ3Failure);

	if (0) // Further checks on child
		return(kQ3Failure);

	if (0) // Further checks on result
		return(kQ3Failure);
#endif



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
#if Q3_DEBUG
	if (0) // Further checks on elementType
		return(NULL);

	if (0) // Further checks on name
		return(NULL);

	if (0) // Further checks on sizeOfElement
		return(NULL);

	if (0) // Further checks on metaHandler
		return(NULL);
#endif



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
#if Q3_DEBUG
	if (0) // Further checks on elementType
		return(kQ3Failure);

	if (0) // Further checks on sizeOfElement
		return(kQ3Failure);
#endif



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
#if Q3_DEBUG
	if (0) // Further checks on attributeType
		return(NULL);

	if (0) // Further checks on creatorName
		return(NULL);

	if (0) // Further checks on sizeOfElement
		return(NULL);

	if (0) // Further checks on metaHandler
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XAttributeClass_Register(attributeType, creatorName, sizeOfElement, metaHandler));
}





//=============================================================================
//      Q3XAttributeSet_GetPointer : Quesa API entry point.
//-----------------------------------------------------------------------------
/*void *
Q3XAttributeSet_GetPointer(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on attributeSet
		return(NULL);

	if (0) // Further checks on attributeType
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XAttributeSet_GetPointer(attributeSet, attributeType));
}

*/



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
#if Q3_DEBUG
	if (0) // Further checks on attributeSet
		return(0);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XAttributeSet_GetMask(attributeSet));
}


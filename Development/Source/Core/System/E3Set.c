/*  NAME:
        E3Set.c

    DESCRIPTION:
        Set and attribute set implementation.
        
        Sets are implemented as an array of TQ3ElementObjects. Elements are
        registered as classes in their own right, so the data for each element
        is just the instance data for the TQ3ElementObjects in the set.
        
        This might need to be revised to support custom attributes, but for
        now this is sufficient to implement the pre-defined attribute types.
        
        Attribute sets contain an additional mask, which caches the attributes
        contained in the set. This mask is currently only updated by the
        Q3Attribute_foo routines, and so using a Q3Set_foo routine on an
        attribute set may produce unexpected results (since the mask might get
        out of sync with the set contents).
        
        The QD3D docs are somewhat vague as to what should happen here, but
        I read them as meaning that you shouldn't actually be calling Q3Set_foo
        routines on an attribute set - so this implementation would be OK.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
#include "E3IOFileFormat.h"
#include "E3Main.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
const TQ3Uns32 kSetTableSize									= 8;





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
#define e3attribute_type_to_mask(theType)												\
			((theType > kQ3AttributeTypeNone && theType < kQ3AttributeTypeNumTypes) ?	\
				(1 << (theType - 1)) : kQ3XAttributeMaskCustomAttribute)





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Set iterator
typedef TQ3Status (*TQ3SetIterator)(TQ3SetData *instanceData, TQ3ObjectType theType, TQ3ElementObject theElement, void *userData);


// Set iterator param info
typedef struct TQ3SetIteratorParamInfo {
	TQ3SetData			*setInstanceData;	// Set instance data
	TQ3SetIterator		theIterator;		// Set iterator
	void				*iteratorData;		// User data for the set iterator
} TQ3SetIteratorParamInfo;



// Attribute set inherit iterator param info
typedef struct TQ3AttributeSetInheritParamInfo {
	TQ3AttributeSet		theResult;			// Output set
	TQ3Boolean			isChild;			// Iterating over child
} TQ3AttributeSetInheritParamInfo;





class E3AttributeSet : public E3Set // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3SetTypeAttribute, E3AttributeSet, E3Set )
public :

	// There is no extra data for this class
	} ;
	


class E3SetElement : public E3Element // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeSetElement, E3SetElement, E3Element )
public :

	TQ3SetObject			instanceData ;
	} ;
	


class E3SurfaceUVAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeSurfaceUV, E3SurfaceUVAttribute, E3Attribute )
public :

	TQ3Param2D				instanceData ; // This appears to be redundant but I will not risk removing it yet.
	} ;
	


class E3ShadingUVAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeShadingUV, E3ShadingUVAttribute, E3Attribute )
public :

	TQ3Param2D				instanceData ; // This appears to be redundant but I will not risk removing it yet.
	} ;
	


class E3NormalAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeNormal, E3NormalAttribute, E3Attribute )
public :

	TQ3Vector3D				instanceData ; // This appears to be redundant but I will not risk removing it yet.
	} ;
	


class E3AmbientCoefficientAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeAmbientCoefficient, E3AmbientCoefficientAttribute, E3Attribute )
public :

	float					instanceData ; // This appears to be redundant but I will not risk removing it yet.
	} ;
	


class E3DiffuseColorAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeDiffuseColor, E3DiffuseColorAttribute, E3Attribute )
public :

	TQ3ColorRGB				instanceData ; // This appears to be redundant but I will not risk removing it yet.
	} ;
	


class E3SpecularColorAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeSpecularColor, E3SpecularColorAttribute, E3Attribute )
public :

	TQ3ColorRGB				instanceData ; // This appears to be redundant but I will not risk removing it yet.
	} ;
	


class E3SpecularControlAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeSpecularControl, E3SpecularControlAttribute, E3Attribute )
public :

	float					instanceData ; // This appears to be redundant but I will not risk removing it yet.
	} ;
	


class E3TransparencyColorAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeTransparencyColor, E3TransparencyColorAttribute, E3Attribute )
public :

	TQ3ColorRGB				instanceData ; // This appears to be redundant but I will not risk removing it yet.
	} ;
	


class E3SurfaceTangentAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeSurfaceTangent, E3SurfaceTangentAttribute, E3Attribute )
public :

	TQ3Tangent2D			instanceData ; // This appears to be redundant but I will not risk removing it yet.
	} ;
	


class E3HighlightStateAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeHighlightState, E3HighlightStateAttribute, E3Attribute )
public :

	TQ3Switch				instanceData ; // This appears to be redundant but I will not risk removing it yet.
	} ;
	


class E3SurfaceShaderAttribute : public E3Attribute // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeSurfaceShader, E3SurfaceShaderAttribute, E3Attribute )
public :

	TQ3SurfaceShaderObject	instanceData ; // This appears to be redundant but I will not risk removing it yet.
	} ;
	




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      E3ElementInfo::E3ElementInfo : Constructor for class info of the class.
//-----------------------------------------------------------------------------

E3ElementInfo::E3ElementInfo	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
		: E3Root ( newClassMetaHandler, newParent ) ,
		elementCopyAddMethod		( (TQ3XElementCopyAddMethod)		Find_Method ( kQ3XMethodTypeElementCopyAdd , kQ3False ) ) ,	// Not inherited  
		elementCopyReplaceMethod	( (TQ3XElementCopyReplaceMethod)	Find_Method ( kQ3XMethodTypeElementCopyReplace ) ) ,	 
		elementCopyGetMethod		( (TQ3XElementCopyGetMethod)		Find_Method ( kQ3XMethodTypeElementCopyGet ) ) ,	 
		elementCopyDuplicateMethod	( (TQ3XElementCopyDuplicateMethod)	Find_Method ( kQ3XMethodTypeElementCopyDuplicate , kQ3False ) ) , // Not inherited 
		elementDeleteMethod			( (TQ3XElementDeleteMethod)			Find_Method ( kQ3XMethodTypeElementDelete , kQ3False ) ) // Not inherited 
	{

	} ;



//=============================================================================
//      e3element_new_class_info : Method to construct a class info record.
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3element_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent
			 	)
	{
	return new ( std::nothrow ) E3ElementInfo ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3set_add_element : Add an element to a set.
//-----------------------------------------------------------------------------
static TQ3Status
e3set_add_element(TQ3SetData *instanceData, TQ3ElementType theType, TQ3ElementObject theElement)
{	TQ3Status	qd3dStatus;



	// Create our table if required
	if (instanceData->theTable == NULL)
		instanceData->theTable = E3HashTable_Create(kSetTableSize);



	// Make sure we have a table
	if (instanceData->theTable == NULL)
		return(kQ3Failure);



	// Add the element to the table
	qd3dStatus = E3HashTable_Add(instanceData->theTable, theType, theElement);

	return(qd3dStatus);
}





//=============================================================================
//      e3set_remove_element : Remove an element to a set.
//-----------------------------------------------------------------------------
static TQ3ElementObject
e3set_remove_element(TQ3SetData *instanceData, TQ3ElementType theType)
{	TQ3ElementObject	theElement;



	// Make sure we have a table
	if (instanceData->theTable == NULL)
		return(NULL);



	// Find the appropriate element, and remove it
	theElement = (TQ3ElementObject) E3HashTable_Find(instanceData->theTable, theType);
	if (theElement != NULL)
		E3HashTable_Remove(instanceData->theTable, theType);

	return(theElement);
}





//=============================================================================
//      e3set_clear_elements : Clear the elements associated with a set.
//-----------------------------------------------------------------------------
//		Note : We assume our caller has already released the elements.
//-----------------------------------------------------------------------------
static void
e3set_clear_elements(TQ3SetData *instanceData)
{


	// Recreate the table
	if (instanceData->theTable != NULL)
		{
		E3HashTable_Destroy(&instanceData->theTable);
		instanceData->theTable = NULL;
		}

}





//=============================================================================
//      e3set_find_element : Find an element in a set.
//-----------------------------------------------------------------------------
static TQ3ElementObject
e3set_find_element(const TQ3SetData *instanceData, TQ3ElementType theType)
{	TQ3ElementObject	theElement;



	// Make sure we have a table
	if (instanceData->theTable == NULL)
		return(NULL);



	// Find the element
	theElement = (TQ3ElementObject) E3HashTable_Find(instanceData->theTable, theType);
	
	return(theElement);
}





//=============================================================================
//      e3set_iterator : Set iterator glue.
//-----------------------------------------------------------------------------
static TQ3Status
e3set_iterator(E3HashTablePtr theTable, TQ3ObjectType theKey, void *theItem, void *userData)
{	TQ3SetData					*setInstanceData;
	void						*iteratorData;
	TQ3SetIterator				theIterator;
	TQ3SetIteratorParamInfo		*paramInfo;
	TQ3Status					qd3dStatus;



	// Get our param info
	paramInfo        = (TQ3SetIteratorParamInfo *) userData;
	setInstanceData  = paramInfo->setInstanceData;
	theIterator      = paramInfo->theIterator;
	iteratorData     = paramInfo->iteratorData;



	// Invoke the iterator
	qd3dStatus = theIterator(setInstanceData, theKey, (TQ3ElementObject) theItem, iteratorData);

	return(qd3dStatus);
}





//=============================================================================
//      e3set_iterate_elements : Iterate over the elements in a set.
//-----------------------------------------------------------------------------
static TQ3Status
e3set_iterate_elements(TQ3SetData *instanceData, TQ3SetIterator theIterator, void *userData)
{	TQ3Status					qd3dStatus;
	TQ3SetIteratorParamInfo		paramInfo;



	// Make sure we have a table
	if (instanceData->theTable == NULL)
		return(kQ3Success); // it's OK to have an empty set, just do nothing



	// Iterate over the table	
	paramInfo.setInstanceData = instanceData;
	paramInfo.theIterator     = theIterator;
	paramInfo.iteratorData    = userData;
	
	qd3dStatus = E3HashTable_Iterate(instanceData->theTable, e3set_iterator, &paramInfo);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3set_iterator_delete : Set iterator to delete elements.
//-----------------------------------------------------------------------------
static TQ3Status
e3set_iterator_delete(TQ3SetData *instanceData, TQ3ObjectType theType, TQ3ElementObject theElement, void *userData)
{


	// Delete the element
	Q3Object_Dispose(theElement);
	
	return(kQ3Success);
}





//=============================================================================
//      e3set_iterator_duplicate : Set iterator to duplicate elements.
//-----------------------------------------------------------------------------
static TQ3Status
e3set_iterator_duplicate(TQ3SetData *instanceData, TQ3ObjectType theType, TQ3ElementObject theElement, void *userData)
{	TQ3SetData			*toInstanceData;
	TQ3Status			qd3dStatus;
	TQ3ElementObject	toElement;



	// Duplicate the element into the destination attribute set
	toInstanceData = (TQ3SetData *) userData;
	toElement      = Q3Object_Duplicate(theElement);

	if (toElement != NULL)
		qd3dStatus = e3set_add_element(toInstanceData, theType, toElement);
	else
		qd3dStatus = kQ3Failure;
	
	return(qd3dStatus);
}





//=============================================================================
//      e3set_iterator_submit : Set iterator to submit elements.
//-----------------------------------------------------------------------------
static TQ3Status
e3set_iterator_submit(TQ3SetData *instanceData, TQ3ObjectType theType, TQ3ElementObject theElement, void *userData)
{	TQ3Status					qd3dStatus;
	TQ3ViewObject				*theView;



	// Get our param info
	theView = (TQ3ViewObject *) userData;



	// Submit the element
	qd3dStatus = E3View_SubmitImmediate(*theView, theType, theElement->FindLeafInstanceData () ) ;

	return(qd3dStatus);
}





//=============================================================================
//      e3set_iterator_scan_types : Set iterator to build the scan list.
//-----------------------------------------------------------------------------
static TQ3Status
e3set_iterator_scan_types(TQ3SetData *instanceData, TQ3ObjectType theType, TQ3ElementObject theElement, void *userData)
{	TQ3Status	qd3dStatus;



	// Append the type to the scan results table
	qd3dStatus = Q3Memory_Reallocate(&instanceData->scanResults, (instanceData->scanCount + 1) * sizeof (TQ3ObjectType));
	if (qd3dStatus == kQ3Success)
		{
		instanceData->scanResults[instanceData->scanCount] = theType;
		instanceData->scanCount++;
		}
	
	return(qd3dStatus);
}





//=============================================================================
//      e3set_delete : Set class delete method.
//-----------------------------------------------------------------------------
static void
e3set_delete ( E3Set* theObject, void *privateData )
	{
	TQ3SetData		*instanceData = (TQ3SetData *) privateData;



	// Empty the set
	theObject->Empty () ;



	// Dispose of our instance data
	if (instanceData->theTable != NULL)
		E3HashTable_Destroy(&instanceData->theTable);

	Q3Memory_Free(&instanceData->scanResults);
	}





//=============================================================================
//      e3set_duplicate : Set class duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3set_duplicate(TQ3Object fromObject, const void *fromPrivateData,
				 E3Set* toObject,        void *toPrivateData)
{	const TQ3SetData	*fromInstanceData = (const TQ3SetData *) fromPrivateData;
	TQ3SetData			*toInstanceData   = (TQ3SetData *)       toPrivateData;
	TQ3Status			qd3dStatus;
#pragma unused(fromObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	toInstanceData->theTable = NULL;
	toInstanceData->theMask = fromInstanceData->theMask;
	toInstanceData->attributes = fromInstanceData->attributes;
	if(toInstanceData->attributes.surfaceShader != NULL)
		toInstanceData->attributes.surfaceShader = Q3Object_Duplicate(toInstanceData->attributes.surfaceShader);
	
	


	// If there are any elements to copy, duplicate them
	if (fromInstanceData->theTable != NULL)
		{
		qd3dStatus = e3set_iterate_elements((TQ3SetData *) fromInstanceData, e3set_iterator_duplicate, toInstanceData);
		if (qd3dStatus != kQ3Success)
			{
			toObject->Empty () ;

			if (toInstanceData->theTable != NULL)
				E3HashTable_Destroy(&toInstanceData->theTable);

			return(kQ3Failure);
			}
		}

	return(kQ3Success);
}





//=============================================================================
//      e3set_metahandler : Set class metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3set_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3set_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3set_duplicate;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attributeset_iterator_inherit : Attribute set inheritance iterator.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attributeset_iterator_inherit(TQ3SetData *instanceData, TQ3ObjectType theType, TQ3ElementObject theElement, void *userData)
	{
	// Get our param info
	TQ3AttributeSetInheritParamInfo* paramInfo = (TQ3AttributeSetInheritParamInfo *) userData;
	TQ3AttributeSet theResult  = paramInfo->theResult;
	TQ3Boolean isChild    = paramInfo->isChild;



	// Get the attribute type
	theType = E3Attribute_ClassToAttributeType(theType);



	// Decide if we need to add the element to the result or not
	TQ3Status qd3dStatus = kQ3Success;
	TQ3Boolean addElement = (TQ3Boolean) (isChild || ! ( (E3Set*) theResult ) ->Contains ( theType ) ) ;

	if (addElement)
		{
		// Handle built in attributes
		if ((theType > kQ3AttributeTypeNone) && (theType < kQ3AttributeTypeNumTypes))
			qd3dStatus = ( (E3Set*) theResult )->Add ( theType, theElement->FindLeafInstanceData () ) ;


		// Handle custom attributes
		else
			{
			// See if we need to inherit
			TQ3XAttributeInheritMethod inheritMethod = (TQ3XAttributeInheritMethod)
							( NULL != theElement->GetMethod ( kQ3XMethodTypeAttributeInherit ) ) ;
			if (inheritMethod == kQ3True)
				{
				// Use the copy inherit method to copy the attribute
				TQ3XAttributeCopyInheritMethod copyInheritMethod = (TQ3XAttributeCopyInheritMethod) theElement->GetMethod ( kQ3XMethodTypeAttributeCopyInherit ) ;
				if (copyInheritMethod != NULL)
					{
					qd3dStatus    = kQ3Failure;
					void* attributeData = Q3Memory_AllocateClear ( theElement->GetClass ()->GetInstanceSize () ) ;
	
					if (attributeData != NULL)
						qd3dStatus = copyInheritMethod( theElement->FindLeafInstanceData (), attributeData); 
	
					if (qd3dStatus == kQ3Success)
						qd3dStatus = ( (E3Set*) theResult )->Add ( theType, attributeData ) ;
	
					Q3Memory_Free(&attributeData);
					}
	
	
				// Or just copy it directly		
				else
					qd3dStatus = ( (E3Set*) theResult )->Add ( theType, theElement->FindLeafInstanceData () ) ;
				}
			}



		// Handle failure
		if (qd3dStatus != kQ3Success)
			( (E3Set*) theResult )->Empty () ;
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3attributeset_submit : Attribute set class submit method.
//-----------------------------------------------------------------------------
//		Note :	See the comments in E3Set_SubmitElements for an explanation
//				as to why we don't perform the actual submit here.
//-----------------------------------------------------------------------------
static TQ3Status
e3attributeset_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
	{	
#pragma unused(objectType)
#pragma unused(objectData)



	// Submit the attribute set
	return ( (E3Set*) theObject )->SubmitElements ( theView ) ;
	}





//=============================================================================
//      e3attributeset_metahandler : Attribute set class metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attributeset_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attributeset_submit;
			break;
			
		case kQ3XMethodTypeObjectIsDrawable:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_surfaceuv_submit : Surface UV submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_surfaceuv_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the attribute
	E3View_State_SetAttributeSurfaceUV(theView, (const TQ3Param2D *) objectData);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_surfaceuv_metahandler : Surface UV metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_surfaceuv_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attribute_surfaceuv_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_shadinguv_submit : Shading UV submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_shadinguv_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the attribute
	E3View_State_SetAttributeShadingUV(theView, (const TQ3Param2D *) objectData);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_shadinguv_metahandler : Shading UV metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_shadinguv_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attribute_shadinguv_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_normal_submit : Normal submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_normal_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the attribute
	E3View_State_SetAttributeNormal(theView, (const TQ3Vector3D *) objectData);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_normal_metahandler : Normal metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_normal_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attribute_normal_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_ambientcoefficient_submit : Ambient coeff submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_ambientcoefficient_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the attribute
	E3View_State_SetAttributeAmbientCoefficient(theView, (const float *) objectData);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_ambientcoefficient_metahandler : Ambient coeff metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_ambientcoefficient_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attribute_ambientcoefficient_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_diffusecolor_submit : Diffuse colour submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_diffusecolor_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the attribute
	E3View_State_SetAttributeDiffuseColor(theView, (const TQ3ColorRGB *) objectData);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_diffusecolor_metahandler : Diffuse colour metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_diffusecolor_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attribute_diffusecolor_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_specularcolor_submit : Specular colour submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_specularcolor_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the attribute
	E3View_State_SetAttributeSpecularColor(theView, (const TQ3ColorRGB *) objectData);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_specularcolor_metahandler : Specular colour metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_specularcolor_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attribute_specularcolor_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_specularcontrol_submit : Specular control submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_specularcontrol_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the attribute
	E3View_State_SetAttributeSpecularControl(theView, (const float *) objectData);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_specularcontrol_metahandler : Specular control metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_specularcontrol_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attribute_specularcontrol_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_transparencycolor_submit : Transparency submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_transparencycolor_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the attribute
	E3View_State_SetAttributeTransparencyColor(theView, (const TQ3ColorRGB *) objectData);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_transparencycolor_metahandler : Transparency metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_transparencycolor_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attribute_transparencycolor_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_surfacetangent_submit : Surface tangent submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_surfacetangent_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the attribute
	E3View_State_SetAttributeSurfaceTangent(theView, (const TQ3Tangent2D *) objectData);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_surfacetangent_metahandler : Surface tangent metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_surfacetangent_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attribute_surfacetangent_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_highlightstate_submit : Highlight state submit method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_highlightstate_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the attribute
	E3View_State_SetAttributeHighlightState(theView, (const TQ3Switch *) objectData);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_highlightstate_metahandler : Highlight state metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_highlightstate_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attribute_highlightstate_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_surfaceshader_new : Surface shader new method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_surfaceshader_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3SurfaceShaderObject			*instanceData = (TQ3SurfaceShaderObject *) privateData;
	const TQ3SurfaceShaderObject	*theShader    = (const TQ3SurfaceShaderObject *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	*instanceData = Q3Shared_GetReference(*theShader);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_surfaceshader_delete : Surface shader delete method.
//-----------------------------------------------------------------------------
static void
e3attribute_surfaceshader_delete(TQ3Object theObject, void *privateData)
{	TQ3SurfaceShaderObject	*instanceData = (TQ3SurfaceShaderObject *) privateData;
#pragma unused(privateData)
#pragma unused(theObject)



	// Dispose of our instance data
	Q3Object_CleanDispose(instanceData);
}





//=============================================================================
//      e3attribute_surfaceshader_duplicate : Surface shader duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3attribute_surfaceshader_duplicate(TQ3Object fromObject, const void *fromPrivateData,
								    TQ3Object toObject,   void       *toPrivateData)
{	const TQ3SurfaceShaderObject	*fromInstanceData = (const TQ3SurfaceShaderObject *) fromPrivateData;
	TQ3SurfaceShaderObject			*toInstanceData   = (TQ3SurfaceShaderObject *)       toPrivateData;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	if (*fromInstanceData != NULL)
		{
		*toInstanceData = Q3Object_Duplicate(*fromInstanceData);
		if (*toInstanceData == NULL)
			return(kQ3Failure);
		}

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_surfaceshader_submit : Surface shader submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3attribute_surfaceshader_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)



	// Submit the attribute
	E3View_State_SetAttributeSurfaceShader(theView, (const TQ3SurfaceShaderObject *) objectData);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_surfaceshader_copyreplace : Surface shader copy method.
//-----------------------------------------------------------------------------
static TQ3Status
e3attribute_surfaceshader_copyreplace(const void *fromAPIElement, void *ontoInternalElement)
{	const TQ3SurfaceShaderObject	*fromShader;
	TQ3SurfaceShaderObject			*toShader;



	// Retrieve our data
	fromShader = (const TQ3SurfaceShaderObject *) fromAPIElement;
	toShader   = (      TQ3SurfaceShaderObject *) ontoInternalElement;



	// Replace to current toShader with the new fromShader
	E3Shared_Replace(toShader, *fromShader);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_surfaceshader_copyget : Surface shader copy method.
//-----------------------------------------------------------------------------
static TQ3Status
e3attribute_surfaceshader_copyget(const void *fromInternalElement, void *toAPIElement)
{	const TQ3SurfaceShaderObject	*fromShader;
	TQ3SurfaceShaderObject			*toShader;



	// Retrieve our data
	fromShader = (const TQ3SurfaceShaderObject *) fromInternalElement;
	toShader   = (      TQ3SurfaceShaderObject *) toAPIElement;



	// Return a new reference to our shader
	E3Shared_Acquire(toShader, *fromShader);

	return(kQ3Success);
}





//=============================================================================
//      e3attribute_surfaceshader_metahandler : Surface shader metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_surfaceshader_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3attribute_surfaceshader_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3attribute_surfaceshader_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3attribute_surfaceshader_duplicate;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3attribute_surfaceshader_submit;
			break;

		case kQ3XMethodTypeElementCopyReplace:
			theMethod = (TQ3XFunctionPointer) e3attribute_surfaceshader_copyreplace;
			break;

		case kQ3XMethodTypeElementCopyGet:
			theMethod = (TQ3XFunctionPointer) e3attribute_surfaceshader_copyget;
			break;
		}
	
	return(theMethod);
}






#pragma mark -
//=============================================================================
//      e3element_write : Element write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3element_write(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status		qd3dStatus;



	// Submit the object
	qd3dStatus = E3FileFormat_Method_SubmitObject (theView, theObject, objectType, objectData);

	return(qd3dStatus);
}





//=============================================================================
//      e3element_metahandler : Element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3element_metahandler(TQ3XMethodType methodType)
	{
	switch ( methodType )
		{
		case kQ3XMethodTypeNewObjectClass :
			return (TQ3XFunctionPointer) e3element_new_class_info ;

		case kQ3XMethodTypeObjectSubmitWrite:
			return (TQ3XFunctionPointer) e3element_write ;
		}
	
	return NULL ;
	}





//=============================================================================
//      e3attribute_metahandler : Element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3attribute_metahandler(TQ3XMethodType methodType)
	{
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectSubmitWrite:
			return (TQ3XFunctionPointer) e3element_write ; // Why not just inherit?
		}
	
	return NULL ;
	}





#pragma mark -
//=============================================================================
//      e3setelement_copyadd : Copy add/get method.
//-----------------------------------------------------------------------------
static TQ3Status
e3setelement_copyadd (TQ3SetObject *source, TQ3SetObject *dest)
{
	*dest = Q3Shared_GetReference(*source);

	return (*dest != NULL) ? kQ3Success : kQ3Failure;
}





//=============================================================================
//      e3setelement_copyduplicate : Copy duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3setelement_copyduplicate( TQ3SetObject *source, TQ3SetObject *dest)
{
	*dest = Q3Object_Duplicate(*source);

	return (*dest != NULL) ? kQ3Success : kQ3Failure;
}





//=============================================================================
//      e3setelement_copyreplace : Copy replace method.
//-----------------------------------------------------------------------------
static TQ3Status
e3setelement_copyreplace (TQ3SetObject *source, TQ3SetObject *dest)
{
	TQ3SetObject	temp;
	
	temp = Q3Shared_GetReference(*source);
	if (temp == NULL) 
		return kQ3Failure;

	if (*dest)
		Q3Object_Dispose(*dest);
	
	*dest = temp;

	return kQ3Success;
}





//=============================================================================
//      e3setelement_delete : Delete method.
//-----------------------------------------------------------------------------
static TQ3Status
e3setelement_delete (TQ3SetObject *set)
{
	return Q3Object_CleanDispose( set );
}





//=============================================================================
//      e3setelement_metahandler : Set element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3setelement_metahandler(TQ3XMethodType methodType)
{
	TQ3XFunctionPointer		theMethod = NULL;
	
	switch (methodType)
	{
		case kQ3XMethodTypeElementCopyAdd:
		case kQ3XMethodTypeElementCopyGet:
			theMethod = (TQ3XFunctionPointer) e3setelement_copyadd;
			break;

		case kQ3XMethodTypeElementCopyDuplicate:
			theMethod = (TQ3XFunctionPointer) e3setelement_copyduplicate;
			break;

		case kQ3XMethodTypeElementCopyReplace:
			theMethod = (TQ3XFunctionPointer) e3setelement_copyreplace;
			break;

		case kQ3XMethodTypeElementDelete:
			theMethod = (TQ3XFunctionPointer) e3setelement_delete;
			break;
	}

	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Set_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ElementObject
E3Set::FindElement ( TQ3ElementType theType )
	{
	return e3set_find_element ( & setData , theType ) ;
	}





//=============================================================================
//      E3Set_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the set classes
	qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameSet,
										e3set_metahandler,
										E3Set ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameSetAttribute,
											e3attributeset_metahandler,
											E3AttributeSet ) ;


	// Register the element classes
	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameElement,
											e3element_metahandler,
											E3Element ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttribute,
											e3attribute_metahandler,
											E3Attribute ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameSetElement,
											e3setelement_metahandler,
											E3SetElement ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeSurfaceUV,
											e3attribute_surfaceuv_metahandler,
											E3SurfaceUVAttribute ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeShadingUV,
											e3attribute_shadinguv_metahandler,
											E3ShadingUVAttribute ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeNormal,
											e3attribute_normal_metahandler,
											E3NormalAttribute ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeAmbientCoefficient,
											e3attribute_ambientcoefficient_metahandler,
											E3AmbientCoefficientAttribute ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeDiffuseColor,
											e3attribute_diffusecolor_metahandler,
											E3DiffuseColorAttribute ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeSpecularColor,
											e3attribute_specularcolor_metahandler,
											E3SpecularColorAttribute ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeSpecularControl,
											e3attribute_specularcontrol_metahandler,
											E3SpecularControlAttribute ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeTransparencyColor,
											e3attribute_transparencycolor_metahandler,
											E3TransparencyColorAttribute ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeSurfaceTangent,
											e3attribute_surfacetangent_metahandler,
											E3SurfaceTangentAttribute ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeHighlightState,
											e3attribute_highlightstate_metahandler,
											E3HighlightStateAttribute ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameAttributeSurfaceShader,
											e3attribute_surfaceshader_metahandler,
											E3SurfaceShaderAttribute ) ;

	return(qd3dStatus);
}





//=============================================================================
//      E3Set_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeSurfaceShader,      kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeHighlightState,     kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeSurfaceTangent,     kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeTransparencyColor,  kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeSpecularControl,    kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeSpecularColor,      kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeDiffuseColor,       kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeAmbientCoefficient, kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeNormal,             kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeShadingUV,          kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeSurfaceUV,          kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeElement,                     kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3SharedTypeSet,                         kQ3True);

	return(qd3dStatus);
}







//=============================================================================
//      E3Set::IsOfMyClass : Check if object pointer is valid and of type set
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3SharedTypeSet )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3Set::IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3Set ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3Set_New : Create a set object.
//-----------------------------------------------------------------------------
TQ3SetObject
E3Set_New(void)
	{
	// Create the object
	return E3ClassTree::CreateInstance ( kQ3SharedTypeSet, kQ3False, NULL ) ;
	}





//=============================================================================
//      E3Set_GetType : Get the type of a set.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Set::GetType ( void )
	{
	// Return the type
	return GetObjectType ( kQ3SharedTypeSet ) ;
	}





//=============================================================================
//      E3Set_Add : Add an element to a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set::Add ( TQ3ElementType theType, const void *data )
	{
	TQ3Status qd3dStatus = kQ3Success ;

	if ( ( theType  < kQ3AttributeTypeNone ) || ( theType > kQ3AttributeTypeNumTypes ) )
		theType = E3Attribute_ClassToAttributeType ( theType ) ;

	switch ( theType )
		{
		case kQ3AttributeTypeSurfaceUV:
			setData.attributes.surfaceUV = * ( (TQ3Param2D*) data ) ;
			break;
		case kQ3AttributeTypeShadingUV:
			setData.attributes.shadingUV = * ( (TQ3Param2D*) data ) ;
			break;
		case kQ3AttributeTypeNormal:
			setData.attributes.normal = * ( (TQ3Vector3D*) data ) ;
			break;
		case kQ3AttributeTypeAmbientCoefficient:
			setData.attributes.ambientCoeficient = * ( (float*) data ) ;
			break;
		case kQ3AttributeTypeDiffuseColor:
			setData.attributes.diffuseColor = * ( (TQ3ColorRGB*) data ) ;
			break;
		case kQ3AttributeTypeSpecularColor:
			setData.attributes.specularColor = * ( (TQ3ColorRGB*) data ) ;
			break;
		case kQ3AttributeTypeSpecularControl:
			setData.attributes.specularControl = * ( (float*) data ) ;
			break;
		case kQ3AttributeTypeTransparencyColor:
			setData.attributes.trasparencyColor = * ( (TQ3ColorRGB*) data ) ;
			break;
		case kQ3AttributeTypeSurfaceTangent:
			setData.attributes.surfaceTangent = * ( (TQ3Tangent2D*) data ) ;
			break;
		case kQ3AttributeTypeHighlightState:
			setData.attributes.highlightState = * ( (TQ3Switch*) data ) ;
			break;
			
		case kQ3AttributeTypeSurfaceShader:
			if ( setData.attributes.surfaceShader != NULL )
				Q3Object_Dispose ( setData.attributes.surfaceShader ) ;
				
			setData.attributes.surfaceShader = * ( (TQ3SurfaceShaderObject*) data ) ;
			Q3Shared_GetReference ( setData.attributes.surfaceShader ) ;
			Q3_ASSERT( Q3Object_IsType ( setData.attributes.surfaceShader,
				kQ3ShaderTypeSurface ) ) ;
			break;
			
		default:
			// Find the element, and replace its data if it exists
			TQ3ElementObject theElement = e3set_find_element ( & setData , theType ) ;
			if ( theElement != NULL )
				{
				E3ElementInfo* theClass = (E3ElementInfo*) theElement->GetClass () ;
				
				if ( theClass->elementCopyReplaceMethod != NULL )
					qd3dStatus = theClass->elementCopyReplaceMethod ( data, theElement->FindLeafInstanceData () ) ;
				else
					{
					TQ3Uns32 dataSize = theClass->GetInstanceSize () ;
					if ( dataSize > 0 )
						Q3Memory_Copy ( data, theElement->FindLeafInstanceData () , dataSize ) ;
					qd3dStatus = kQ3Success ;
					}

				}
			else
				{
				// We don't have an existing element, so instantiate a new one
				theElement = E3ClassTree::CreateInstance ( theType, kQ3False, data ) ;
				if ( theElement == NULL )
					return kQ3Failure ;

				// And add it to the set
				qd3dStatus = e3set_add_element ( & setData , theType, theElement ) ;
				}
			break;
		}


	if ( qd3dStatus != kQ3Failure )
		{
		setData.theMask |= e3attribute_type_to_mask ( theType ) ;
		Q3Shared_Edited ( this ) ;
		}

	return qd3dStatus ;
	}





//=============================================================================
//      E3Set_Get : Get the data for an element in a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set::Get ( TQ3ElementType theType, void *data )
	{
	TQ3Status qd3dStatus = kQ3Success ;

	if ( ( theType < kQ3AttributeTypeNone ) || ( theType > kQ3AttributeTypeNumTypes ) )
		theType = E3Attribute_ClassToAttributeType ( theType ) ;

	if ( ( theType > kQ3AttributeTypeNone ) && ( theType < kQ3AttributeTypeNumTypes ) )
		if ( ( setData.theMask & e3attribute_type_to_mask(theType) ) == 0 )
			return kQ3Failure ;
			
	switch ( theType )
		{
		case kQ3AttributeTypeSurfaceUV:
			* ( (TQ3Param2D*) data ) = setData.attributes.surfaceUV ;
			break;
		case kQ3AttributeTypeShadingUV:
			* ( (TQ3Param2D*) data ) = setData.attributes.shadingUV ;
			break;
		case kQ3AttributeTypeNormal:
			* ( (TQ3Vector3D*) data ) = setData.attributes.normal ;
			break;
		case kQ3AttributeTypeAmbientCoefficient:
			* ( (float*) data ) = setData.attributes.ambientCoeficient ;
			break;
		case kQ3AttributeTypeDiffuseColor:
			* ( (TQ3ColorRGB*) data ) = setData.attributes.diffuseColor ;
			break;
		case kQ3AttributeTypeSpecularColor:
			* ( (TQ3ColorRGB*) data ) = setData.attributes.specularColor ;
			break;
		case kQ3AttributeTypeSpecularControl:
			* ( (float*) data ) = setData.attributes.specularControl ;
			break;
		case kQ3AttributeTypeTransparencyColor:
			* ( (TQ3ColorRGB*) data ) = setData.attributes.trasparencyColor ;
			break;
		case kQ3AttributeTypeSurfaceTangent:
			* ( (TQ3Tangent2D*) data ) = setData.attributes.surfaceTangent ;
			break;
		case kQ3AttributeTypeHighlightState:
			* ( (TQ3Switch*) data ) = setData.attributes.highlightState ;
			break;
		case kQ3AttributeTypeSurfaceShader:
			* ( (TQ3SurfaceShaderObject*) data ) = Q3Shared_GetReference( setData.attributes.surfaceShader) ;
			break;
		default:

			// Get the size and pointer for the data for the element
			TQ3ElementObject theElement = e3set_find_element ( & setData , theType ) ;
			if ( theElement == NULL )
				return kQ3Failure ;
				
			E3ElementInfo* theClass = (E3ElementInfo*) theElement->GetClass () ;

			if ( theClass == NULL )
				return kQ3Failure ;
				
			TQ3Uns32 dataSize = theClass->GetInstanceSize () ;


			// If there's nothing to copy, bail. It is OK for dataSize to be 0, as the
			// mere presence of an attribute can itself carry information.
			if ( dataSize == 0 )
				return kQ3Success ;



			// Copy the element data
			if ( theClass->elementCopyGetMethod != NULL )
				qd3dStatus = theClass->elementCopyGetMethod ( theElement->FindLeafInstanceData () , (void*) data ) ;
			else
				{
				Q3Memory_Copy ( theElement->FindLeafInstanceData () , data, dataSize ) ;
				qd3dStatus = kQ3Success ;
				}
		}
	
	return qd3dStatus ;
	}





//=============================================================================
//      E3Set_CopyElement : Copy an element from one set to another
//-----------------------------------------------------------------------------
TQ3Status
E3Set::CopyElement ( TQ3ElementType theType, TQ3SetObject destSet )
	{
	TQ3Status qd3dStatus = kQ3Success ;

	// Get the instance data for each set
	E3Set* srcSet = (E3Set*) this ;
	E3Set* dstSet = (E3Set*) destSet ;

	if ( ( theType  < kQ3AttributeTypeNone ) || ( theType > kQ3AttributeTypeNumTypes ) )
		theType = E3Attribute_ClassToAttributeType ( theType ) ;

	if ( ( theType  > kQ3AttributeTypeNone ) && ( theType < kQ3AttributeTypeNumTypes ) )
		if ( ( srcSet->setData.theMask & e3attribute_type_to_mask(theType) ) == 0 )
			return kQ3Failure ;
			
	switch ( theType )
		{
		case kQ3AttributeTypeSurfaceUV:
			dstSet->setData.attributes.surfaceUV = srcSet->setData.attributes.surfaceUV ;
			break;
		case kQ3AttributeTypeShadingUV:
			dstSet->setData.attributes.shadingUV = srcSet->setData.attributes.shadingUV ;
			break;
		case kQ3AttributeTypeNormal:
			dstSet->setData.attributes.normal = srcSet->setData.attributes.normal ;
			break;
		case kQ3AttributeTypeAmbientCoefficient:
			dstSet->setData.attributes.ambientCoeficient = srcSet->setData.attributes.ambientCoeficient ;
			break;
		case kQ3AttributeTypeDiffuseColor:
			dstSet->setData.attributes.diffuseColor = srcSet->setData.attributes.diffuseColor ;
			break;
		case kQ3AttributeTypeSpecularColor:
			dstSet->setData.attributes.specularColor = srcSet->setData.attributes.specularColor ;
			break;
		case kQ3AttributeTypeSpecularControl:
			dstSet->setData.attributes.specularControl = srcSet->setData.attributes.specularControl ;
			break;
		case kQ3AttributeTypeTransparencyColor:
			dstSet->setData.attributes.trasparencyColor = srcSet->setData.attributes.trasparencyColor ;
			break;
		case kQ3AttributeTypeSurfaceTangent:
			dstSet->setData.attributes.surfaceTangent = srcSet->setData.attributes.surfaceTangent ;
			break;
		case kQ3AttributeTypeHighlightState:
			dstSet->setData.attributes.highlightState = srcSet->setData.attributes.highlightState ;
			break;
		case kQ3AttributeTypeSurfaceShader:
			if ( dstSet->setData.attributes.surfaceShader != NULL )
				Q3Object_Dispose ( dstSet->setData.attributes.surfaceShader ) ;
			dstSet->setData.attributes.surfaceShader = Q3Shared_GetReference ( srcSet->setData.attributes.surfaceShader ) ;
			break;
		default:

			// Find the element to copy
			TQ3ElementObject srcElement = e3set_find_element ( & srcSet->setData, theType ) ;
			if ( srcElement == NULL )
				return kQ3Failure ;



			// If the destination has an element of this type, remove it
			( (E3Set*) destSet )->Clear ( theType ) ;



			// Duplicate the element, and add it to the set
			TQ3ElementObject dstElement = Q3Object_Duplicate ( srcElement ) ;
			if ( dstElement != NULL )
				qd3dStatus = e3set_add_element ( & dstSet->setData, theType, dstElement ) ;
			else
				qd3dStatus = kQ3Failure ;
			}
			
	return qd3dStatus ;
	}





//=============================================================================
//      E3Set_Contains : Does a set contain an element?
//-----------------------------------------------------------------------------
TQ3Boolean
E3Set::Contains ( TQ3ElementType theType )
	{
	if ( ( theType < kQ3AttributeTypeNone ) || ( theType > kQ3AttributeTypeNumTypes ) )
		theType = E3Attribute_ClassToAttributeType ( theType ) ;
	// Built-in attributes
	if ( ( theType > kQ3AttributeTypeNone ) && ( theType < kQ3AttributeTypeNumTypes ) )
		{
		// Test the mask
		TQ3XAttributeMask mask = ( e3attribute_type_to_mask(theType) ) ;
		mask = ( ( setData.theMask & e3attribute_type_to_mask(theType) ) != 0 ) ;
		return (TQ3Boolean) ( ( setData.theMask & e3attribute_type_to_mask(theType) ) != 0 ) ;
		}
	
	// Custom attributes
	TQ3ElementObject theElement = e3set_find_element ( & setData , theType ) ;
	return (TQ3Boolean) ( theElement != NULL ) ;
	}





//=============================================================================
//      E3Set_Clear : Remove an element from a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set::Clear ( TQ3ElementType theType )
	{
	if ( ( theType < kQ3AttributeTypeNone ) || (theType > kQ3AttributeTypeNumTypes ) )
		theType = E3Attribute_ClassToAttributeType ( theType ) ;

	if ( ( theType  > kQ3AttributeTypeNone ) && (theType < kQ3AttributeTypeNumTypes ) )
		{
		if ( ( setData.theMask & e3attribute_type_to_mask(theType) ) == 0 )
			return kQ3Failure ;
			
		if ( theType == kQ3AttributeTypeSurfaceShader )
			Q3Object_CleanDispose ( & setData.attributes.surfaceShader ) ;

		Q3Shared_Edited ( this ) ;
		setData.theMask &= ~e3attribute_type_to_mask(theType) ;
		return kQ3Success ;
		}

	// Remove the element
	TQ3ElementObject theElement = e3set_remove_element ( & setData, theType ) ;
	if ( theElement != NULL )
		{
		Q3Object_Dispose ( theElement ) ;
		Q3Shared_Edited ( this ) ;
		return kQ3Success ;
		}
	
	return kQ3Failure ;
	}





//=============================================================================
//      E3Set_Empty : Remove everything from a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set::Empty ( void )
	{
	if ( setData.attributes.surfaceShader != NULL )
		{
		Q3Object_Dispose ( setData.attributes.surfaceShader ) ;
		setData.attributes.surfaceShader = NULL ;
		}

	// Remove the elements from the set
	if ( setData.theTable != NULL )
		{
		e3set_iterate_elements ( & setData, e3set_iterator_delete, NULL ) ;
		e3set_clear_elements ( & setData ) ;
		}
	
	Q3Shared_Edited ( this ) ;

	setData.theMask = kQ3XAttributeMaskNone ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Set_GetNextElementType : Get the next element type in a set.
//-----------------------------------------------------------------------------
//		See notes in E3AttributeSet_GetNextAttributeType
//-----------------------------------------------------------------------------
TQ3Status
E3Set::GetNextElementType ( TQ3ElementType *theType )
	{
	return E3AttributeSet_GetNextAttributeType ( this, theType ) ;
	}





//=============================================================================
//      E3Set_SubmitElements : Submit the elements of a set.
//-----------------------------------------------------------------------------
//		Note :	Relies on the fact that an attribute set stores its attributes
//				as elements in the parent set.
//
//				We can therefore submit our attributes by walking through the
//				elements in the parent set and submitting them.
//
//
//				Note that since an attribute set does not store the data to be
//				submitted in its instance data, we can't call the normal
//				E3View_SubmitImmediate routine used for immediate mode submits.
//
//				Instead the immediate mode routine (this one) handles the
//				actual submit, and the retained mode routine (our render
//				method) calls through to us to submit retained mode objects.
//
//				This is the reverse of the normal scheme for immediate/retained
//				submits, so it's worth explaining.
//-----------------------------------------------------------------------------
TQ3Status
E3Set::SubmitElements ( TQ3ViewObject inView )
	{
	TQ3Status qd3dStatus = kQ3Success ;

	// Submit the elements in the set

	if ( setData.theMask != kQ3XAttributeMaskNone )
		{
		TQ3XAttributeMask mask = setData.theMask ;
		if ( ( ( mask & kQ3XAttributeMaskSurfaceUV ) != 0 ) && ( qd3dStatus == kQ3Success ) )
			qd3dStatus = E3View_SubmitImmediate ( inView, kQ3ObjectTypeAttributeSurfaceUV, &setData.attributes.surfaceUV ) ;

		if ( ( ( mask & kQ3XAttributeMaskShadingUV ) != 0 ) && ( qd3dStatus == kQ3Success ) )
			qd3dStatus = E3View_SubmitImmediate ( inView, kQ3ObjectTypeAttributeShadingUV, &setData.attributes.shadingUV ) ;

		if ( ( ( mask & kQ3XAttributeMaskNormal ) != 0 ) && ( qd3dStatus == kQ3Success ) )
			qd3dStatus = E3View_SubmitImmediate ( inView, kQ3ObjectTypeAttributeNormal, &setData.attributes.normal ) ;

		if ( ( ( mask & kQ3XAttributeMaskAmbientCoefficient ) != 0 ) && ( qd3dStatus == kQ3Success ) )
			qd3dStatus = E3View_SubmitImmediate ( inView, kQ3ObjectTypeAttributeAmbientCoefficient, &setData.attributes.ambientCoeficient ) ;

		if ( ( ( mask & kQ3XAttributeMaskDiffuseColor ) != 0 ) && ( qd3dStatus == kQ3Success ) )
			qd3dStatus = E3View_SubmitImmediate ( inView, kQ3ObjectTypeAttributeDiffuseColor, &setData.attributes.diffuseColor ) ;

		if ( ( ( mask & kQ3XAttributeMaskSpecularColor ) != 0 ) && ( qd3dStatus == kQ3Success ) )
			qd3dStatus = E3View_SubmitImmediate ( inView, kQ3ObjectTypeAttributeSpecularColor, &setData.attributes.specularColor ) ;

		if ( ( ( mask & kQ3XAttributeMaskSpecularControl ) != 0 ) && ( qd3dStatus == kQ3Success ) )
			qd3dStatus = E3View_SubmitImmediate ( inView, kQ3ObjectTypeAttributeSpecularControl, &setData.attributes.specularControl ) ;

		if ( ( ( mask & kQ3XAttributeMaskTransparencyColor ) != 0 ) && ( qd3dStatus == kQ3Success ) )
			qd3dStatus = E3View_SubmitImmediate ( inView, kQ3ObjectTypeAttributeTransparencyColor, &setData.attributes.trasparencyColor ) ;

		if ( ( ( mask & kQ3XAttributeMaskSurfaceTangent ) != 0 ) && ( qd3dStatus == kQ3Success ) )
			qd3dStatus = E3View_SubmitImmediate ( inView, kQ3ObjectTypeAttributeSurfaceTangent, &setData.attributes.surfaceTangent ) ;

		if ( ( ( mask & kQ3XAttributeMaskHighlightState ) != 0 ) && ( qd3dStatus == kQ3Success ) )
			qd3dStatus = E3View_SubmitImmediate ( inView, kQ3ObjectTypeAttributeHighlightState, &setData.attributes.highlightState ) ;

		if ( ( ( mask & kQ3XAttributeMaskSurfaceShader ) != 0 ) && ( qd3dStatus == kQ3Success ) )
			qd3dStatus = E3View_SubmitImmediate ( inView, kQ3ObjectTypeAttributeSurfaceShader, &setData.attributes.surfaceShader ) ;

		}

	if ( ( setData.theTable != NULL ) && ( qd3dStatus == kQ3Success ) )
		qd3dStatus = e3set_iterate_elements ( & setData, e3set_iterator_submit, &inView ) ;
	
	return qd3dStatus ;
	}




//=============================================================================
//      E3Attribute_ClassToAttributeType : Get the attribute type for a class.
//-----------------------------------------------------------------------------
//		Note :	We convert from an attribute class type to the public QD3D
//				attribute type for that attribute class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3AttributeType
E3Attribute_ClassToAttributeType(TQ3ObjectType theType)
{


	// Convert the type
	switch (theType) {
		case kQ3ObjectTypeAttributeSurfaceUV:
			theType = kQ3AttributeTypeSurfaceUV;
			break;
		case kQ3ObjectTypeAttributeShadingUV:
			theType = kQ3AttributeTypeShadingUV;
			break;
		case kQ3ObjectTypeAttributeNormal:
			theType = kQ3AttributeTypeNormal;
			break;
		case kQ3ObjectTypeAttributeAmbientCoefficient:
			theType = kQ3AttributeTypeAmbientCoefficient;
			break;
		case kQ3ObjectTypeAttributeDiffuseColor:
			theType = kQ3AttributeTypeDiffuseColor;
			break;
		case kQ3ObjectTypeAttributeSpecularColor:
			theType = kQ3AttributeTypeSpecularColor;
			break;
		case kQ3ObjectTypeAttributeSpecularControl:
			theType = kQ3AttributeTypeSpecularControl;
			break;
		case kQ3ObjectTypeAttributeTransparencyColor:
			theType = kQ3AttributeTypeTransparencyColor;
			break;
		case kQ3ObjectTypeAttributeSurfaceTangent:
			theType = kQ3AttributeTypeSurfaceTangent;
			break;
		case kQ3ObjectTypeAttributeHighlightState:
			theType = kQ3AttributeTypeHighlightState;
			break;
		case kQ3ObjectTypeAttributeSurfaceShader:
			theType = kQ3AttributeTypeSurfaceShader;
			break;
		default:
			break;
		}
	
	return(theType);
}





//=============================================================================
//      E3Attribute_AttributeToClassType : Get the class type for an attribute.
//-----------------------------------------------------------------------------
//		Note :	We convert from a public QD3D attribute type to the class type
//				that the associated attribute class was registered with.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Attribute_AttributeToClassType(TQ3AttributeType theType)
{	static const TQ3ObjectType attributeTypes[kQ3AttributeTypeNumTypes] =
									{ kQ3ObjectTypeInvalid,
									  kQ3ObjectTypeAttributeSurfaceUV,
									  kQ3ObjectTypeAttributeShadingUV,
									  kQ3ObjectTypeAttributeNormal,
									  kQ3ObjectTypeAttributeAmbientCoefficient,
									  kQ3ObjectTypeAttributeDiffuseColor,
									  kQ3ObjectTypeAttributeSpecularColor,
									  kQ3ObjectTypeAttributeSpecularControl,
									  kQ3ObjectTypeAttributeTransparencyColor,
									  kQ3ObjectTypeAttributeSurfaceTangent,
									  kQ3ObjectTypeAttributeHighlightState,
									  kQ3ObjectTypeAttributeSurfaceShader };



	// If the type is in range, return it. In debug builds,
	// we check that types can be translated both ways.
	if ( (theType >= kQ3AttributeTypeNone) && (theType < kQ3AttributeTypeNumTypes))
		{
		Q3_ASSERT(theType == E3Attribute_ClassToAttributeType(attributeTypes[theType]));
		theType = attributeTypes[theType];
		}

	return(theType);
}





//=============================================================================
//      E3AttributeSet_GetNextAttributeType : Get the next element type in a set.
//-----------------------------------------------------------------------------
//		Note :	Assumes the set will not be changed while scanning. If the set
//				is changed, we will detect this and stop the iteration.
//
//				To avoid depending on the details of how sets are implemented,
//				we use an iterator to build up a flat list of the types of the
//				elements in the set.
//
//				When the search concludes, this list will be freed: if a scan
//				is stopped early, the list will persist until the next scan or
//				until the set is disposed of.
//-----------------------------------------------------------------------------
TQ3Status
E3AttributeSet_GetNextAttributeType(TQ3AttributeSet theSet, TQ3AttributeType *theType)
	{
	E3Set* set = (E3Set*) theSet ;

	// Get the edit index for the set
	TQ3Uns32 editIndex = Q3Shared_GetEditIndex ( theSet ) ;



	// Prepare to start a new scan
	if ( *theType == kQ3ElementTypeNone )
		{
		// Reset our state from any previous scan 
		set->setData.scanEditIndex = editIndex ;
		set->setData.scanCount     = 0 ;
		set->setData.scanIndex     = 0 ;
		Q3Memory_Free(& set->setData.scanResults) ;
		
		// put in the built-in attributes
		if ( set->setData.theMask != kQ3XAttributeMaskNone )
			{
			TQ3XAttributeMask mask = set->setData.theMask ;
			if ( ( mask & kQ3XAttributeMaskSurfaceUV ) != 0 )
				e3set_iterator_scan_types ( & set->setData , kQ3AttributeTypeSurfaceUV, NULL, NULL ) ;

			if ( ( mask & kQ3XAttributeMaskShadingUV ) != 0 )
				e3set_iterator_scan_types ( & set->setData , kQ3AttributeTypeShadingUV, NULL, NULL ) ;

			if ( ( mask & kQ3XAttributeMaskNormal ) != 0 )
				e3set_iterator_scan_types ( & set->setData , kQ3AttributeTypeNormal, NULL, NULL ) ;

			if ( ( mask & kQ3XAttributeMaskAmbientCoefficient ) != 0 )
				e3set_iterator_scan_types ( & set->setData , kQ3AttributeTypeAmbientCoefficient, NULL, NULL ) ;

			if ( ( mask & kQ3XAttributeMaskDiffuseColor ) != 0 )
				e3set_iterator_scan_types ( & set->setData , kQ3AttributeTypeDiffuseColor, NULL, NULL ) ;

			if ( ( mask & kQ3XAttributeMaskSpecularColor ) != 0 )
				e3set_iterator_scan_types ( & set->setData , kQ3AttributeTypeSpecularColor, NULL, NULL ) ;

			if ( ( mask & kQ3XAttributeMaskSpecularControl ) != 0 )
				e3set_iterator_scan_types ( & set->setData , kQ3AttributeTypeSpecularControl, NULL, NULL ) ;

			if ( ( mask & kQ3XAttributeMaskTransparencyColor ) != 0 )
				e3set_iterator_scan_types ( & set->setData , kQ3AttributeTypeTransparencyColor, NULL, NULL ) ;

			if ( ( mask & kQ3XAttributeMaskSurfaceTangent ) != 0 )
				e3set_iterator_scan_types ( & set->setData , kQ3AttributeTypeSurfaceTangent, NULL, NULL ) ;

			if ( ( mask & kQ3XAttributeMaskHighlightState ) != 0 )
				e3set_iterator_scan_types ( & set->setData , kQ3AttributeTypeHighlightState, NULL, NULL ) ;

			if ( ( mask & kQ3XAttributeMaskSurfaceShader ) != 0 )
				e3set_iterator_scan_types ( & set->setData , kQ3AttributeTypeSurfaceShader, NULL, NULL ) ;

			}

		// Build the array of types in the set
		if ( set->setData.theTable != NULL )
			e3set_iterate_elements ( & set->setData, e3set_iterator_scan_types, NULL ) ;
		}
	
	
	// Continue a previous scan
	else
		{
		// If we've been edited, stop the scan
		if ( editIndex != set->setData.scanEditIndex )
			{
			set->setData.scanCount     = 0 ;
			set->setData.scanIndex     = 0 ;
			Q3Memory_Free(&set->setData.scanResults) ;

			*theType = kQ3ElementTypeNone ;
			return kQ3Success ;
			}
		}



	// Return the next type in the set
	if ( set->setData.scanIndex < set->setData.scanCount )
		{
		Q3_ASSERT_VALID_PTR(set->setData.scanResults) ;
		
		*theType = set->setData.scanResults [ set->setData.scanIndex ] ;
		set->setData.scanIndex++ ;
		}
	else
		{
		*theType = kQ3ElementTypeNone ;
		}



	// If that was the last type, clean up
	if ( set->setData.scanIndex == set->setData.scanCount )
		{
		set->setData.scanCount     = 0 ;
		set->setData.scanIndex     = 0 ;
		Q3Memory_Free(&set->setData.scanResults) ;
		}
	
	return kQ3Success ;
	}




//=============================================================================
//      E3Attribute_Submit : Submit an attribute to the view.
//-----------------------------------------------------------------------------
TQ3Status
E3Attribute_Submit(TQ3AttributeType attributeType, const void *attributeData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the attribute
	attributeType = E3Attribute_AttributeToClassType(attributeType);
	qd3dStatus    = E3View_SubmitImmediate(theView, attributeType, attributeData);

	return(qd3dStatus);
}





//=============================================================================
//      E3AttributeSet_Submit : Submit a display group.
//-----------------------------------------------------------------------------
TQ3Status
E3AttributeSet_Submit(TQ3AttributeSet theSet, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the set to the view
	qd3dStatus = E3View_SubmitRetained(theView, theSet);

	return(qd3dStatus);
}





//=============================================================================
//      E3AttributeSet_New : Create an attribute set.
//-----------------------------------------------------------------------------
TQ3AttributeSet
E3AttributeSet_New(void)
	{
	// Create the object
	return E3ClassTree::CreateInstance ( kQ3SetTypeAttribute, kQ3False, NULL ) ;
	}





//=============================================================================
//      E3AttributeSet_Inherit : Merge two attribute sets.
//-----------------------------------------------------------------------------
//		Note :	Should really use the kQ3MethodTypeAttributeCopyInherit method
//				for custom attributes.
//-----------------------------------------------------------------------------
TQ3Status
E3AttributeSet_Inherit(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet result)
	{
	TQ3AttributeSetInheritParamInfo		paramInfo ;



	// Empty the final attribute set
	TQ3Status qd3dStatus = ( (E3Set*) result )->Empty () ;
	if ( qd3dStatus == kQ3Failure )
		return qd3dStatus ;



	// Find the instance data
	E3Set* resultSet = (E3Set*) result ;
	E3Set* parentSet = (E3Set*) parent ;
	E3Set* childSet  = (E3Set*) child ;

	// Process the child
	if ( qd3dStatus == kQ3Success )
		{
		// Copy the mask and attributes directly
		resultSet->setData.theMask    = childSet->setData.theMask ;
		resultSet->setData.attributes = childSet->setData.attributes ;
		if ( resultSet->setData.attributes.surfaceShader != NULL )
			resultSet->setData.attributes.surfaceShader = Q3Shared_GetReference ( childSet->setData.attributes.surfaceShader ) ;



		// Iterate over any additional elements
		if ( childSet->setData.theTable != NULL )
			{
			paramInfo.theResult = result ;
			paramInfo.isChild   = kQ3True ;
			qd3dStatus = e3set_iterate_elements ( & childSet->setData, e3attributeset_iterator_inherit, &paramInfo ) ;
			}
		}



	// Process the parent
	if ( qd3dStatus == kQ3Success )
		{
		// Copy any attributes which were not defined by the child
		if ( parentSet->setData.theMask != kQ3XAttributeMaskNone )
			{
			// Set theMask to the attributes unique to the parent
			TQ3XAttributeMask theMask = ~childSet->setData.theMask ;
			theMask &= parentSet->setData.theMask ;



			// Copy those attributes to the result
			if ( E3Bit_IsSet(theMask, kQ3XAttributeMaskSurfaceUV) )
				resultSet->setData.attributes.surfaceUV = parentSet->setData.attributes.surfaceUV ;

			if  (E3Bit_IsSet(theMask, kQ3XAttributeMaskShadingUV) )
				resultSet->setData.attributes.shadingUV = parentSet->setData.attributes.shadingUV ;

			if ( E3Bit_IsSet(theMask, kQ3XAttributeMaskNormal) )
				resultSet->setData.attributes.normal = parentSet->setData.attributes.normal ;

			if ( E3Bit_IsSet(theMask, kQ3XAttributeMaskAmbientCoefficient) )
				resultSet->setData.attributes.ambientCoeficient = parentSet->setData.attributes.ambientCoeficient ;

			if ( E3Bit_IsSet(theMask, kQ3XAttributeMaskDiffuseColor) )
				resultSet->setData.attributes.diffuseColor = parentSet->setData.attributes.diffuseColor ;

			if ( E3Bit_IsSet(theMask, kQ3XAttributeMaskSpecularColor) )
				resultSet->setData.attributes.specularColor = parentSet->setData.attributes.specularColor ;

			if ( E3Bit_IsSet(theMask, kQ3XAttributeMaskSpecularControl) )
				resultSet->setData.attributes.specularControl = parentSet->setData.attributes.specularControl ;

			if ( E3Bit_IsSet(theMask, kQ3XAttributeMaskTransparencyColor) )
				resultSet->setData.attributes.trasparencyColor = parentSet->setData.attributes.trasparencyColor ;

			if ( E3Bit_IsSet(theMask, kQ3XAttributeMaskSurfaceTangent) )
				resultSet->setData.attributes.surfaceTangent = parentSet->setData.attributes.surfaceTangent ;

			if ( E3Bit_IsSet(theMask, kQ3XAttributeMaskHighlightState) )
				resultSet->setData.attributes.highlightState = parentSet->setData.attributes.highlightState ;

			if ( E3Bit_IsSet(theMask, kQ3XAttributeMaskSurfaceShader) )
				resultSet->setData.attributes.surfaceShader = Q3Shared_GetReference(parentSet->setData.attributes.surfaceShader ) ;


			// Update the mask in the result
			resultSet->setData.theMask |= theMask ;
			}



		// Iterate over any additional elements
		if  (parentSet->setData.theTable != NULL )
			{
			paramInfo.theResult = result ;
			paramInfo.isChild   = kQ3False ;
			qd3dStatus          = e3set_iterate_elements ( & parentSet->setData, e3attributeset_iterator_inherit, &paramInfo ) ;
			}
		}
	
	return qd3dStatus ;
	}





//=============================================================================
//      E3XElementClass_Register : Register an element class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3XObjectClass
E3XElementClass_Register(TQ3ElementType *elementType, const char *name, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler)
	{
	// Allocate a unique type for this class
	*elementType = E3ClassTree::GetNextClassType () ;



	// Register the class
	TQ3Status qd3dStatus = E3ClassTree::RegisterClass ( kQ3ObjectTypeElement ,
											*elementType ,
											name ,
											metaHandler ,
											sizeOfElement + sizeof ( E3Element ) ); 
	if ( qd3dStatus == kQ3Failure )
		return NULL ;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( *elementType ) ;

	return (TQ3XObjectClass) theClass ;
	}





//=============================================================================
//      E3XElementType_GetElementSize : Get the size of an element.
//-----------------------------------------------------------------------------
TQ3Status
E3XElementType_GetElementSize(TQ3ElementType elementType, TQ3Uns32 *sizeOfElement)
	{
	// Assign a return value
	*sizeOfElement = 0 ;



	// Although this is a Q3X method (which are normally intended for external
	// elements registered by applications), QD3D's version of this routine did
	// accept the built-in attribute types for the elementType parameter.
	//
	// To be consistent with QD3D, we provide the same behaviour.
	if ( ( elementType > kQ3AttributeTypeNone ) && ( elementType < kQ3AttributeTypeNumTypes ) )
		elementType = E3Attribute_AttributeToClassType ( elementType ) ;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( elementType ) ;
	if ( theClass == NULL )
		return kQ3Failure ;



	// Get the size of the element	
	*sizeOfElement = theClass->GetInstanceSize () ;

	return kQ3Success ;
	}






//=============================================================================
//      E3XAttributeClass_Register : Register an attribute class.
//-----------------------------------------------------------------------------
TQ3XObjectClass
E3XAttributeClass_Register(TQ3AttributeType *attributeType, const char *creatorName, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler)
	{
	// Allocate a unique type for this class
	*attributeType = E3ClassTree::GetNextClassType () ;



	// Register the class
	TQ3Status qd3dStatus = E3ClassTree::RegisterClass ( kQ3ElementTypeAttribute ,
											*attributeType ,
											creatorName ,
											metaHandler ,
											sizeOfElement + sizeof ( E3Attribute ) ) ;
	if ( qd3dStatus == kQ3Failure )
		return NULL ;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( *attributeType ) ;

	return (TQ3XObjectClass) theClass ;
	}





//=============================================================================
//      E3XAttributeSet_GetPointer : Get a pointer to an attribute's data.
//-----------------------------------------------------------------------------
// Note:	E3XAttributeSet_GetPointer handles only built-in attributes,
// 			but this is OK since only a pointer is returned,
// 			so you have to know what data had been returned,
//			and this is only true with built-in attributes,
//			if you want custom attributes you have to call Q3Set_Get
//
//			We should remove this restriction in the future, and allow direct
//			access to the internal data of custom attributes as well: renderers
//			may well want to register their own custom attributes for apps and
//			still get fast access to them during a rendering loop.
//-----------------------------------------------------------------------------
void *
E3XAttributeSet_GetPointer(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType)
	{
	E3Set* set = (E3Set*) attributeSet ;
	
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet),   NULL);

	switch (attributeType)
		{
		case kQ3AttributeTypeSurfaceUV:
			return ( set->setData.theMask & kQ3XAttributeMaskSurfaceUV ) == 0 ?
					NULL : &set->setData.attributes.surfaceUV ;
			break;
		case kQ3AttributeTypeShadingUV:
			return ( set->setData.theMask & kQ3XAttributeMaskShadingUV ) == 0 ?
					NULL : &set->setData.attributes.shadingUV ;
			break;
		case kQ3AttributeTypeNormal:
			return ( set->setData.theMask & kQ3XAttributeMaskNormal ) == 0 ?
					NULL : &set->setData.attributes.normal ;
			break;
		case kQ3AttributeTypeAmbientCoefficient:
			return ( set->setData.theMask & kQ3XAttributeMaskAmbientCoefficient ) == 0 ?
					NULL : &set->setData.attributes.ambientCoeficient ;
			break;
		case kQ3AttributeTypeDiffuseColor:
			return ( set->setData.theMask & kQ3XAttributeMaskDiffuseColor ) == 0 ?
					NULL : &set->setData.attributes.diffuseColor ;
			break;
		case kQ3AttributeTypeSpecularColor:
			return ( set->setData.theMask & kQ3XAttributeMaskSpecularColor ) == 0 ?
					NULL : &set->setData.attributes.specularColor ;
			break;
		case kQ3AttributeTypeSpecularControl:
			return ( set->setData.theMask & kQ3XAttributeMaskSpecularControl ) == 0 ?
					NULL : &set->setData.attributes.specularControl ;
			break;
		case kQ3AttributeTypeTransparencyColor:
			return ( set->setData.theMask & kQ3XAttributeMaskTransparencyColor ) == 0 ?
					NULL : &set->setData.attributes.trasparencyColor ;
			break;
		case kQ3AttributeTypeSurfaceTangent:
			return ( set->setData.theMask & kQ3XAttributeMaskSurfaceTangent ) == 0 ?
					NULL : &set->setData.attributes.surfaceTangent ;
			break;
		case kQ3AttributeTypeHighlightState:
			return ( set->setData.theMask & kQ3XAttributeMaskHighlightState ) == 0 ?
					NULL : &set->setData.attributes.highlightState;
			break;
		case kQ3AttributeTypeSurfaceShader:
			return ( set->setData.theMask & kQ3XAttributeMaskSurfaceShader ) == 0 ?
					NULL : &set->setData.attributes.surfaceShader  ;
			break;
		}
		
	return NULL ;
	}





//=============================================================================
//      E3XAttributeSet_GetMask : Get the mask of attributes in a set.
//-----------------------------------------------------------------------------
TQ3XAttributeMask
E3XAttributeSet_GetMask(TQ3AttributeSet attributeSet)
	{
	return ( (E3Set*) attributeSet )->setData.theMask ;
	}



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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Set.h"
#include "E3View.h"
#include "E3IOFileFormat.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
const TQ3Uns32 kSetTableSize									= 6;





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
#define e3attribute_type_to_mask(theType)												\
			((theType > kQ3AttributeTypeNone && theType < kQ3AttributeTypeNumTypes) ?	\
				(1 << (theType - 1)) : kQ3XAttributeMaskCustomAttribute)





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
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
	TQ3ColorRGB			trasparencyColor;
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





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
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
	theElement = E3HashTable_Find(instanceData->theTable, theType);
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
	theElement = E3HashTable_Find(instanceData->theTable, theType);
	
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
	qd3dStatus = E3View_SubmitImmediate(*theView, theType, theElement->instanceData);

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
e3set_delete(TQ3Object theObject, void *privateData)
{	TQ3SetData		*instanceData = (TQ3SetData *) privateData;



	// Empty the set
	E3Set_Empty(theObject);



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
				 TQ3Object toObject,        void *toPrivateData)
{	const TQ3SetData	*fromInstanceData = (const TQ3SetData *) fromPrivateData;
	TQ3SetData			*toInstanceData   = (TQ3SetData *)       toPrivateData;
	TQ3Status			qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



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
			E3Set_Empty(toObject);

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
{	TQ3Boolean							addElement, isChild;
	TQ3XAttributeCopyInheritMethod 		copyInheritMethod;
	void 								*attributeData;
	TQ3XAttributeInheritMethod 			inheritMethod;
	TQ3Status							qd3dStatus;
	TQ3AttributeSetInheritParamInfo		*paramInfo;
	TQ3AttributeSet						theResult;



	// Get our param info
	paramInfo = (TQ3AttributeSetInheritParamInfo *) userData;
	theResult  = paramInfo->theResult;
	isChild    = paramInfo->isChild;



	// Get the attribute type
	theType = E3Attribute_ClassToAttributeType(theType);



	// Decide if we need to add the element to the result or not
	qd3dStatus = kQ3Success;
	addElement = isChild || !E3Set_Contains(theResult, theType);

	if (addElement)
		{
		// Handle built in attributes
		if ((theType > kQ3AttributeTypeNone) && (theType < kQ3AttributeTypeNumTypes))
			qd3dStatus = E3Set_Add(theResult, theType, theElement->instanceData);


		// Handle custom attributes
		else
			{
			// See if we need to inherit
			inheritMethod = (TQ3XAttributeInheritMethod) E3ClassTree_GetMethod(
																theElement->theClass,
																kQ3XMethodTypeAttributeInherit);
			if (inheritMethod == kQ3True)
				{
				// Use the copy inherit method to copy the attribute
				copyInheritMethod = (TQ3XAttributeCopyInheritMethod) E3ClassTree_GetMethod(
																		theElement->theClass,
																		kQ3XMethodTypeAttributeCopyInherit);
				if (copyInheritMethod != NULL)
					{
					qd3dStatus    = kQ3Failure;
					attributeData = Q3Memory_AllocateClear(E3ClassTree_GetInstanceSize(theElement->theClass));
	
					if (attributeData != NULL)
						qd3dStatus = copyInheritMethod(theElement->instanceData, attributeData); 
	
					if (qd3dStatus == kQ3Success)
						qd3dStatus = E3Set_Add(theResult, theType, attributeData);
	
					Q3Memory_Free(&attributeData);
					}
	
	
				// Or just copy it directly		
				else
					qd3dStatus = E3Set_Add(theResult, theType, theElement->instanceData);
				}
			}



		// Handle failure
		if (qd3dStatus != kQ3Success)
			E3Set_Empty(theResult);
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
{	TQ3Status		qd3dStatus;
#pragma unused(objectType)
#pragma unused(objectData)



	// Submit the attribute set
	qd3dStatus = E3Set_SubmitElements(theObject, theView);

	return(qd3dStatus);
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
{	TQ3XFunctionPointer		theMethod = NULL;

	
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitWrite:
			theMethod = (TQ3XFunctionPointer) e3element_write;
			break;
		}
	
	return(theMethod);
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
E3Set_FindElement(TQ3SetObject theSet, TQ3ElementType theType)
{	TQ3SetData			*instanceData;


	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(theSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return (NULL);

	return (e3set_find_element(instanceData, theType));
}





//=============================================================================
//      E3Set_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the set classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
											kQ3SharedTypeSet,
											kQ3ClassNameSet,
											e3set_metahandler,
											sizeof(TQ3SetData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeSet,
												kQ3SetTypeAttribute,
												kQ3ClassNameSetAttribute,
												e3attributeset_metahandler,
												0);



	// Register the element classes
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
												kQ3ObjectTypeElement,
												kQ3ClassNameElement,
												e3element_metahandler,
												0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ElementTypeAttribute,
												kQ3ClassNameAttribute,
												e3element_metahandler,
												0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ObjectTypeSetElement,
												kQ3ClassNameSetElement,
												e3setelement_metahandler,
												sizeof(TQ3SetObject));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
												kQ3ObjectTypeAttributeSurfaceUV,
												kQ3ClassNameAttributeSurfaceUV,
												e3attribute_surfaceuv_metahandler,
												sizeof(TQ3Param2D));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
												kQ3ObjectTypeAttributeShadingUV,
												kQ3ClassNameAttributeShadingUV,
												e3attribute_shadinguv_metahandler,
												sizeof(TQ3Param2D));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
												kQ3ObjectTypeAttributeNormal,
												kQ3ClassNameAttributeNormal,
												e3attribute_normal_metahandler,
												sizeof(TQ3Vector3D));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
												kQ3ObjectTypeAttributeAmbientCoefficient,
												kQ3ClassNameAttributeAmbientCoefficient,
												e3attribute_ambientcoefficient_metahandler,
												sizeof(float));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
												kQ3ObjectTypeAttributeDiffuseColor,
												kQ3ClassNameAttributeDiffuseColor,
												e3attribute_diffusecolor_metahandler,
												sizeof(TQ3ColorRGB));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
												kQ3ObjectTypeAttributeSpecularColor,
												kQ3ClassNameAttributeSpecularColor,
												e3attribute_specularcolor_metahandler,
												sizeof(TQ3ColorRGB));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
												kQ3ObjectTypeAttributeSpecularControl,
												kQ3ClassNameAttributeSpecularControl,
												e3attribute_specularcontrol_metahandler,
												sizeof(float));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
												kQ3ObjectTypeAttributeTransparencyColor,
												kQ3ClassNameAttributeTransparencyColor,
												e3attribute_transparencycolor_metahandler,
												sizeof(TQ3ColorRGB));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
												kQ3ObjectTypeAttributeSurfaceTangent,
												kQ3ClassNameAttributeSurfaceTangent,
												e3attribute_surfacetangent_metahandler,
												sizeof(TQ3Tangent2D));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
												kQ3ObjectTypeAttributeHighlightState,
												kQ3ClassNameAttributeHighlightState,
												e3attribute_highlightstate_metahandler,
												sizeof(TQ3Switch));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
												kQ3ObjectTypeAttributeSurfaceShader,
												kQ3ClassNameAttributeSurfaceShader,
												e3attribute_surfaceshader_metahandler,
												sizeof(TQ3SurfaceShaderObject));

	return(qd3dStatus);
}





//=============================================================================
//      E3Set_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeSurfaceShader,      kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeHighlightState,     kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeSurfaceTangent,     kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeTransparencyColor,  kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeSpecularControl,    kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeSpecularColor,      kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeDiffuseColor,       kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeAmbientCoefficient, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeNormal,             kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeShadingUV,          kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeSurfaceUV,          kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeElement,                     kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3SharedTypeSet,                         kQ3True);

	return(qd3dStatus);
}







//=============================================================================
//      E3Set_New : Create a set object.
//-----------------------------------------------------------------------------
TQ3SetObject
E3Set_New(void)
{	TQ3SetObject	theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3SharedTypeSet, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3Set_GetType : Get the type of a set.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Set_GetType(TQ3SetObject theSet)
{


	// Return the type
	return(E3ClassTree_GetObjectType(theSet, kQ3SharedTypeSet));
}





//=============================================================================
//      E3Set_Add : Add an element to a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_Add(TQ3SetObject theSet, TQ3ElementType theType, const void *data)
{	TQ3XElementCopyReplaceMethod		copyReplaceMethod;
	TQ3SetData							*instanceData;
	TQ3Status							qd3dStatus = kQ3Success;
	TQ3ElementObject					theElement;
	TQ3Uns32							dataSize;



	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(theSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(kQ3Failure);


	if ((theType  < kQ3AttributeTypeNone ) || (theType > kQ3AttributeTypeNumTypes))
		theType = E3Attribute_ClassToAttributeType(theType);

	switch (theType) {
		case kQ3AttributeTypeSurfaceUV:
			instanceData->attributes.surfaceUV = *((TQ3Param2D*)data);
			break;
		case kQ3AttributeTypeShadingUV:
			instanceData->attributes.shadingUV = *((TQ3Param2D*)data);
			break;
		case kQ3AttributeTypeNormal:
			instanceData->attributes.normal = *((TQ3Vector3D*)data);
			break;
		case kQ3AttributeTypeAmbientCoefficient:
			instanceData->attributes.ambientCoeficient = *((float*)data);
			break;
		case kQ3AttributeTypeDiffuseColor:
			instanceData->attributes.diffuseColor = *((TQ3ColorRGB*)data);
			break;
		case kQ3AttributeTypeSpecularColor:
			instanceData->attributes.specularColor = *((TQ3ColorRGB*)data);
			break;
		case kQ3AttributeTypeSpecularControl:
			instanceData->attributes.specularControl = *((float*)data);
			break;
		case kQ3AttributeTypeTransparencyColor:
			instanceData->attributes.trasparencyColor = *((TQ3ColorRGB*)data);
			break;
		case kQ3AttributeTypeSurfaceTangent:
			instanceData->attributes.surfaceTangent = *((TQ3Tangent2D*)data);
			break;
		case kQ3AttributeTypeHighlightState:
			instanceData->attributes.highlightState = *((TQ3Switch*)data);
			break;
		case kQ3AttributeTypeSurfaceShader:
		
			if(instanceData->attributes.surfaceShader != NULL)
				Q3Object_Dispose(instanceData->attributes.surfaceShader);
				
			instanceData->attributes.surfaceShader = *((TQ3SurfaceShaderObject*)data);
			Q3Shared_GetReference(instanceData->attributes.surfaceShader);
			Q3_ASSERT( Q3Object_IsType( instanceData->attributes.surfaceShader,
				kQ3ShaderTypeSurface ) );
			break;
		default:
			// Find the element, and replace its data if it exists
			theElement = e3set_find_element(instanceData, theType);
			if (theElement != NULL)
				{
				copyReplaceMethod = (TQ3XElementCopyReplaceMethod) E3ClassTree_GetMethod(theElement->theClass,
																   kQ3XMethodTypeElementCopyReplace);
				if (copyReplaceMethod != NULL)
					qd3dStatus = copyReplaceMethod(data, theElement->instanceData);
				else
					{
					dataSize = E3ClassTree_GetInstanceSize(theElement->theClass);
					if (dataSize > 0)
						Q3Memory_Copy(data, theElement->instanceData, dataSize);
					qd3dStatus = kQ3Success;
					}
				
				}
			else
				{
				// We don't have an existing element, so instantiate a new one
				theElement = E3ClassTree_CreateInstance(theType, kQ3False, data);
				if (theElement == NULL)
					return(kQ3Failure);

				// And add it to the set
				qd3dStatus = e3set_add_element(instanceData, theType, theElement);
				}
			break;
		}


	if (qd3dStatus == kQ3Success){
		instanceData->theMask |= e3attribute_type_to_mask(theType);
		Q3Shared_Edited(theSet);
		}

	return(qd3dStatus);
}





//=============================================================================
//      E3Set_Get : Get the data for an element in a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_Get(TQ3SetObject theSet, TQ3ElementType theType, void *data)
{	TQ3XElementCopyGetMethod	copyGetMethod;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3SetData							*instanceData;
	TQ3ElementObject			theElement;
	TQ3Uns32					dataSize;

	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(theSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(kQ3Failure);


	if ((theType  < kQ3AttributeTypeNone ) || (theType > kQ3AttributeTypeNumTypes))
		theType = E3Attribute_ClassToAttributeType(theType);

	if ((theType  > kQ3AttributeTypeNone ) && (theType < kQ3AttributeTypeNumTypes))
		if ((instanceData->theMask & e3attribute_type_to_mask(theType)) == 0)
			return (kQ3Failure);
			
	switch (theType) {
		case kQ3AttributeTypeSurfaceUV:
			*((TQ3Param2D*)data) = instanceData->attributes.surfaceUV;
			break;
		case kQ3AttributeTypeShadingUV:
			*((TQ3Param2D*)data) = instanceData->attributes.shadingUV;
			break;
		case kQ3AttributeTypeNormal:
			*((TQ3Vector3D*)data) = instanceData->attributes.normal;
			break;
		case kQ3AttributeTypeAmbientCoefficient:
			*((float*)data) = instanceData->attributes.ambientCoeficient;
			break;
		case kQ3AttributeTypeDiffuseColor:
			*((TQ3ColorRGB*)data) = instanceData->attributes.diffuseColor;
			break;
		case kQ3AttributeTypeSpecularColor:
			*((TQ3ColorRGB*)data) = instanceData->attributes.specularColor;
			break;
		case kQ3AttributeTypeSpecularControl:
			*((float*)data) = instanceData->attributes.specularControl;
			break;
		case kQ3AttributeTypeTransparencyColor:
			*((TQ3ColorRGB*)data) = instanceData->attributes.trasparencyColor;
			break;
		case kQ3AttributeTypeSurfaceTangent:
			*((TQ3Tangent2D*)data) = instanceData->attributes.surfaceTangent;
			break;
		case kQ3AttributeTypeHighlightState:
			*((TQ3Switch*)data) = instanceData->attributes.highlightState;
			break;
		case kQ3AttributeTypeSurfaceShader:
			*((TQ3SurfaceShaderObject*)data) = Q3Shared_GetReference(instanceData->attributes.surfaceShader);
			break;
		default:

			// Get the size and pointer for the data for the element
			theElement = e3set_find_element(instanceData, theType);
			if (theElement == NULL)
				return(kQ3Failure);

			if (theElement->theClass == NULL)
				return(kQ3Failure);
				
			dataSize = E3ClassTree_GetInstanceSize(theElement->theClass);


			// If there's nothing to copy, bail. It is OK for dataSize to be 0, as the
			// mere presence of an attribute can itself carry information.
			if (dataSize == 0)
				return(kQ3Success);



			// Copy the element data
			copyGetMethod = (TQ3XElementCopyGetMethod) E3ClassTree_GetMethod(theElement->theClass,
																  kQ3XMethodTypeElementCopyGet);
			if (copyGetMethod != NULL)
				qd3dStatus = copyGetMethod((void *) theElement->instanceData, (void *) data);
			else
				{
				Q3Memory_Copy(theElement->instanceData, data, dataSize);
				qd3dStatus = kQ3Success;
				}
			}
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Set_CopyElement : Copy an element from one set to another
//-----------------------------------------------------------------------------
TQ3Status
E3Set_CopyElement( TQ3SetObject sourceSet, TQ3ElementType theType, TQ3SetObject destSet )
{	TQ3ElementObject	srcElement, dstElement;
	TQ3SetData			*srcInstanceData;
	TQ3SetData			*dstInstanceData;
	TQ3Status			qd3dStatus;



	// Get the instance data for each set
	srcInstanceData = (TQ3SetData *) E3ClassTree_FindInstanceData( sourceSet,
		kQ3SharedTypeSet );
	if (srcInstanceData == NULL)
		return(kQ3Failure);
	
	dstInstanceData = (TQ3SetData *) E3ClassTree_FindInstanceData( destSet,
		kQ3SharedTypeSet );
	if (dstInstanceData == NULL)
		return(kQ3Failure);



	if ((theType  < kQ3AttributeTypeNone ) || (theType > kQ3AttributeTypeNumTypes))
		theType = E3Attribute_ClassToAttributeType(theType);

	if ((theType  > kQ3AttributeTypeNone ) && (theType < kQ3AttributeTypeNumTypes))
		if ((srcInstanceData->theMask & e3attribute_type_to_mask(theType)) == 0)
			return (kQ3Failure);
			
	switch (theType) {
		case kQ3AttributeTypeSurfaceUV:
			dstInstanceData->attributes.surfaceUV = srcInstanceData->attributes.surfaceUV;
			break;
		case kQ3AttributeTypeShadingUV:
			dstInstanceData->attributes.shadingUV = srcInstanceData->attributes.shadingUV;
			break;
		case kQ3AttributeTypeNormal:
			dstInstanceData->attributes.normal = srcInstanceData->attributes.normal;
			break;
		case kQ3AttributeTypeAmbientCoefficient:
			dstInstanceData->attributes.ambientCoeficient = srcInstanceData->attributes.ambientCoeficient;
			break;
		case kQ3AttributeTypeDiffuseColor:
			dstInstanceData->attributes.diffuseColor = srcInstanceData->attributes.diffuseColor;
			break;
		case kQ3AttributeTypeSpecularColor:
			dstInstanceData->attributes.specularColor = srcInstanceData->attributes.specularColor;
			break;
		case kQ3AttributeTypeSpecularControl:
			dstInstanceData->attributes.specularControl = srcInstanceData->attributes.specularControl;
			break;
		case kQ3AttributeTypeTransparencyColor:
			dstInstanceData->attributes.trasparencyColor = srcInstanceData->attributes.trasparencyColor;
			break;
		case kQ3AttributeTypeSurfaceTangent:
			dstInstanceData->attributes.surfaceTangent = srcInstanceData->attributes.surfaceTangent;
			break;
		case kQ3AttributeTypeHighlightState:
			dstInstanceData->attributes.highlightState = srcInstanceData->attributes.highlightState;
			break;
		case kQ3AttributeTypeSurfaceShader:
			if(dstInstanceData->attributes.surfaceShader != NULL)
				Q3Object_Dispose(dstInstanceData->attributes.surfaceShader);
			dstInstanceData->attributes.surfaceShader = Q3Shared_GetReference(srcInstanceData->attributes.surfaceShader);
			break;
		default:

			// Find the element to copy
			srcElement = e3set_find_element(srcInstanceData, theType);
			if (srcElement == NULL)
				return(kQ3Failure);



			// If the destination has an element of this type, remove it
			E3Set_Clear(destSet, theType);



			// Duplicate the element, and add it to the set
			dstElement = Q3Object_Duplicate(srcElement);
			if (dstElement != NULL)
				qd3dStatus = e3set_add_element(dstInstanceData, theType, dstElement);
			else
				qd3dStatus = kQ3Failure;
			}
			
	return(qd3dStatus);
}





//=============================================================================
//      E3Set_Contains : Does a set contain an element?
//-----------------------------------------------------------------------------
TQ3Boolean
E3Set_Contains(TQ3SetObject theSet, TQ3ElementType theType)
{	TQ3SetData			*instanceData;
	TQ3ElementObject	theElement;
	TQ3Boolean				inSet;
	TQ3XAttributeMask	mask;


	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(theSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(kQ3False);
		
	if ((theType  < kQ3AttributeTypeNone ) || (theType > kQ3AttributeTypeNumTypes))
		theType =	E3Attribute_ClassToAttributeType(theType);
	// Built-in attributes
	if ((theType  > kQ3AttributeTypeNone ) && (theType < kQ3AttributeTypeNumTypes))
		{
		// Test the mask
		mask = ( e3attribute_type_to_mask(theType));
		mask = ((instanceData->theMask & e3attribute_type_to_mask(theType)) != 0);
		inSet = (TQ3Boolean) ((instanceData->theMask & e3attribute_type_to_mask(theType)) != 0);
		}
	
	// Custom attributes
	else
		{
		theElement = e3set_find_element(instanceData, theType);
		inSet = (TQ3Boolean) (theElement != NULL);
		}
		
	return(inSet);
}





//=============================================================================
//      E3Set_Clear : Remove an element from a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_Clear(TQ3SetObject theSet, TQ3ElementType theType)
{	TQ3SetData			*instanceData;
	TQ3ElementObject	theElement;



	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(theSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(kQ3Failure);


	if ((theType  < kQ3AttributeTypeNone ) || (theType > kQ3AttributeTypeNumTypes))
		theType = E3Attribute_ClassToAttributeType(theType);

	if ((theType  > kQ3AttributeTypeNone ) && (theType < kQ3AttributeTypeNumTypes))
		{
		if ((instanceData->theMask & e3attribute_type_to_mask(theType)) == 0)
			return (kQ3Failure);
			
		if(theType == kQ3AttributeTypeSurfaceShader)
			Q3Object_CleanDispose(&instanceData->attributes.surfaceShader);

		Q3Shared_Edited(theSet);
		instanceData->theMask &= ~e3attribute_type_to_mask(theType);
		return(kQ3Success);
		}
	else
		{

		// Remove the element
		theElement = e3set_remove_element(instanceData, theType);
		if (theElement != NULL)
			{
			Q3Object_Dispose(theElement);
			Q3Shared_Edited(theSet);
			return(kQ3Success);
			}
		else
			{
			return(kQ3Failure);
			}
		}

}





//=============================================================================
//      E3Set_Empty : Remove everything from a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_Empty(TQ3SetObject theSet)
{	TQ3SetData *instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(theSet, kQ3SharedTypeSet);

	if (instanceData == NULL)
		return(kQ3Failure);

	if(instanceData->attributes.surfaceShader != NULL){
		Q3Object_Dispose(instanceData->attributes.surfaceShader);
		instanceData->attributes.surfaceShader = NULL;
		}

	// Remove the elements from the set
	if (instanceData->theTable != NULL){
		e3set_iterate_elements(instanceData, e3set_iterator_delete, NULL);
		e3set_clear_elements(instanceData);
		}
	
	Q3Shared_Edited(theSet);

	instanceData->theMask = kQ3XAttributeMaskNone;

	return(kQ3Success);
}





//=============================================================================
//      E3Set_GetNextElementType : Get the next element type in a set.
//-----------------------------------------------------------------------------
//		See notes in E3AttributeSet_GetNextAttributeType
//-----------------------------------------------------------------------------
TQ3Status
E3Set_GetNextElementType(TQ3SetObject theSet, TQ3ElementType *theType)
{
	TQ3Status qd3dStatus = E3AttributeSet_GetNextAttributeType(theSet, theType);
	//*theType = E3Attribute_AttributeToClassType(*theType);

	return(qd3dStatus);

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
E3Set_SubmitElements( TQ3SetObject inSet, TQ3ViewObject inView )
{	TQ3SetData					*instanceData;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3XAttributeMask	mask;


	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(inSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Submit the elements in the set

		if(instanceData->theMask != kQ3XAttributeMaskNone){
			mask = instanceData->theMask;
			if(((mask & kQ3XAttributeMaskSurfaceUV) != 0) && (qd3dStatus == kQ3Success))
				qd3dStatus = E3View_SubmitImmediate(inView, kQ3ObjectTypeAttributeSurfaceUV, &instanceData->attributes.surfaceUV);

			if(((mask & kQ3XAttributeMaskShadingUV) != 0) && (qd3dStatus == kQ3Success))
				qd3dStatus = E3View_SubmitImmediate(inView, kQ3ObjectTypeAttributeShadingUV, &instanceData->attributes.shadingUV);

			if(((mask & kQ3XAttributeMaskNormal) != 0) && (qd3dStatus == kQ3Success))
				qd3dStatus = E3View_SubmitImmediate(inView, kQ3ObjectTypeAttributeNormal, &instanceData->attributes.normal);

			if(((mask & kQ3XAttributeMaskAmbientCoefficient) != 0) && (qd3dStatus == kQ3Success))
				qd3dStatus = E3View_SubmitImmediate(inView, kQ3ObjectTypeAttributeAmbientCoefficient, &instanceData->attributes.ambientCoeficient);

			if(((mask & kQ3XAttributeMaskDiffuseColor) != 0) && (qd3dStatus == kQ3Success))
				qd3dStatus = E3View_SubmitImmediate(inView, kQ3ObjectTypeAttributeDiffuseColor, &instanceData->attributes.diffuseColor);

			if(((mask & kQ3XAttributeMaskSpecularColor) != 0) && (qd3dStatus == kQ3Success))
				qd3dStatus = E3View_SubmitImmediate(inView, kQ3ObjectTypeAttributeSpecularColor, &instanceData->attributes.specularColor);

			if(((mask & kQ3XAttributeMaskSpecularControl) != 0) && (qd3dStatus == kQ3Success))
				qd3dStatus = E3View_SubmitImmediate(inView, kQ3ObjectTypeAttributeSpecularControl, &instanceData->attributes.specularControl);

			if(((mask & kQ3XAttributeMaskTransparencyColor) != 0) && (qd3dStatus == kQ3Success))
				qd3dStatus = E3View_SubmitImmediate(inView, kQ3ObjectTypeAttributeTransparencyColor, &instanceData->attributes.trasparencyColor);

			if(((mask & kQ3XAttributeMaskSurfaceTangent) != 0) && (qd3dStatus == kQ3Success))
				qd3dStatus = E3View_SubmitImmediate(inView, kQ3ObjectTypeAttributeSurfaceTangent, &instanceData->attributes.surfaceTangent);

			if(((mask & kQ3XAttributeMaskHighlightState) != 0) && (qd3dStatus == kQ3Success))
				qd3dStatus = E3View_SubmitImmediate(inView, kQ3ObjectTypeAttributeHighlightState, &instanceData->attributes.highlightState);

			if(((mask & kQ3XAttributeMaskSurfaceShader) != 0) && (qd3dStatus == kQ3Success))
				qd3dStatus = E3View_SubmitImmediate(inView, kQ3ObjectTypeAttributeSurfaceShader, &instanceData->attributes.surfaceShader);

			}

		if ((instanceData->theTable != NULL) && (qd3dStatus == kQ3Success))
			qd3dStatus = e3set_iterate_elements(instanceData, e3set_iterator_submit, &inView);
	
	return(qd3dStatus);
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
{	TQ3SetData			*instanceData;
	TQ3Uns32			editIndex;
	TQ3XAttributeMask	mask;



	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(theSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Get the edit index for the set
	editIndex = Q3Shared_GetEditIndex(theSet);



	// Prepare to start a new scan
	if (*theType == kQ3ElementTypeNone)
		{
		// Reset our state from any previous scan 
		instanceData->scanEditIndex = editIndex;
		instanceData->scanCount     = 0;
		instanceData->scanIndex     = 0;
		Q3Memory_Free(&instanceData->scanResults);
		
		// put in the built-in attributes
		if(instanceData->theMask != kQ3XAttributeMaskNone){
			mask = instanceData->theMask;
			if((mask & kQ3XAttributeMaskSurfaceUV) != 0)
				e3set_iterator_scan_types(instanceData, kQ3AttributeTypeSurfaceUV, NULL, NULL);

			if((mask & kQ3XAttributeMaskShadingUV) != 0)
				e3set_iterator_scan_types(instanceData, kQ3AttributeTypeShadingUV, NULL, NULL);

			if((mask & kQ3XAttributeMaskNormal) != 0)
				e3set_iterator_scan_types(instanceData, kQ3AttributeTypeNormal, NULL, NULL);

			if((mask & kQ3XAttributeMaskAmbientCoefficient) != 0)
				e3set_iterator_scan_types(instanceData, kQ3AttributeTypeAmbientCoefficient, NULL, NULL);

			if((mask & kQ3XAttributeMaskDiffuseColor) != 0)
				e3set_iterator_scan_types(instanceData, kQ3AttributeTypeDiffuseColor, NULL, NULL);

			if((mask & kQ3XAttributeMaskSpecularColor) != 0)
				e3set_iterator_scan_types(instanceData, kQ3AttributeTypeSpecularColor, NULL, NULL);

			if((mask & kQ3XAttributeMaskSpecularControl) != 0)
				e3set_iterator_scan_types(instanceData, kQ3AttributeTypeSpecularControl, NULL, NULL);

			if((mask & kQ3XAttributeMaskTransparencyColor) != 0)
				e3set_iterator_scan_types(instanceData, kQ3AttributeTypeTransparencyColor, NULL, NULL);

			if((mask & kQ3XAttributeMaskSurfaceTangent) != 0)
				e3set_iterator_scan_types(instanceData, kQ3AttributeTypeSurfaceTangent, NULL, NULL);

			if((mask & kQ3XAttributeMaskHighlightState) != 0)
				e3set_iterator_scan_types(instanceData, kQ3AttributeTypeHighlightState, NULL, NULL);

			if((mask & kQ3XAttributeMaskSurfaceShader) != 0)
				e3set_iterator_scan_types(instanceData, kQ3AttributeTypeSurfaceShader, NULL, NULL);

			}

		// Build the array of types in the set
		if (instanceData->theTable != NULL)
			e3set_iterate_elements(instanceData, e3set_iterator_scan_types, NULL);
		}
	
	
	// Continue a previous scan
	else
		{
		// If we've been edited, stop the scan
		if (editIndex != instanceData->scanEditIndex)
			{
			instanceData->scanCount     = 0;
			instanceData->scanIndex     = 0;
			Q3Memory_Free(&instanceData->scanResults);

			*theType = kQ3ElementTypeNone;
			return(kQ3Success);
			}
		}



	// Return the next type in the set
	if (instanceData->scanIndex < instanceData->scanCount)
		{
		Q3_ASSERT_VALID_PTR(instanceData->scanResults);
		
		*theType = instanceData->scanResults[instanceData->scanIndex];
		instanceData->scanIndex++;
		}
	else
		{
		*theType = kQ3ElementTypeNone;
		}



	// If that was the last type, clean up
	if (instanceData->scanIndex == instanceData->scanCount)
		{
		instanceData->scanCount     = 0;
		instanceData->scanIndex     = 0;
		Q3Memory_Free(&instanceData->scanResults);
		}
	
	return(kQ3Success);
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
{	TQ3SetObject	theObject;


	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3SetTypeAttribute, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3AttributeSet_Inherit : Merge two attribute sets.
//-----------------------------------------------------------------------------
//		Note :	Should really use the kQ3MethodTypeAttributeCopyInherit method
//				for custom attributes.
//-----------------------------------------------------------------------------
TQ3Status
E3AttributeSet_Inherit(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet result)
{	TQ3SetData							*parentInstanceData, *childInstanceData, *resultInstanceData;
	TQ3Status							qd3dStatus;
	TQ3AttributeSetInheritParamInfo		paramInfo;
	TQ3XAttributeMask					theMask;



	// Empty the final attribute set
	qd3dStatus = E3Set_Empty(result);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Find the instance data
	resultInstanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(result, kQ3SharedTypeSet);
	parentInstanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(parent, kQ3SharedTypeSet);
	childInstanceData  = (TQ3SetData *) E3ClassTree_FindInstanceData(child,  kQ3SharedTypeSet);

	if (parentInstanceData == NULL || resultInstanceData == NULL || childInstanceData == NULL)
		return(kQ3Failure);



	// Process the child
	if (qd3dStatus == kQ3Success)
		{
		// Copy the mask and attributes directly
		resultInstanceData->theMask    = childInstanceData->theMask;
		resultInstanceData->attributes = childInstanceData->attributes;
		if(resultInstanceData->attributes.surfaceShader != NULL)
			resultInstanceData->attributes.surfaceShader = Q3Shared_GetReference(childInstanceData->attributes.surfaceShader);



		// Iterate over any additional elements
		if (childInstanceData->theTable != NULL)
			{
			paramInfo.theResult = result;
			paramInfo.isChild   = kQ3True;
			qd3dStatus = e3set_iterate_elements(childInstanceData, e3attributeset_iterator_inherit, &paramInfo);
			}
		}



	// Process the parent
	if (qd3dStatus == kQ3Success)
		{
		// Copy any attributes which were not defined by the child
		if (parentInstanceData->theMask != kQ3XAttributeMaskNone)
			{
			// Set theMask to the attributes unique to the parent
			theMask = ~childInstanceData->theMask;
			theMask &= parentInstanceData->theMask;



			// Copy those attributes to the result
			if (E3Bit_IsSet(theMask, kQ3XAttributeMaskSurfaceUV))
				resultInstanceData->attributes.surfaceUV = parentInstanceData->attributes.surfaceUV;

			if (E3Bit_IsSet(theMask, kQ3XAttributeMaskShadingUV))
				resultInstanceData->attributes.shadingUV = parentInstanceData->attributes.shadingUV;

			if (E3Bit_IsSet(theMask, kQ3XAttributeMaskNormal))
				resultInstanceData->attributes.normal = parentInstanceData->attributes.normal;

			if (E3Bit_IsSet(theMask, kQ3XAttributeMaskAmbientCoefficient))
				resultInstanceData->attributes.ambientCoeficient = parentInstanceData->attributes.ambientCoeficient;

			if (E3Bit_IsSet(theMask, kQ3XAttributeMaskDiffuseColor))
				resultInstanceData->attributes.diffuseColor = parentInstanceData->attributes.diffuseColor;

			if (E3Bit_IsSet(theMask, kQ3XAttributeMaskSpecularColor))
				resultInstanceData->attributes.specularColor = parentInstanceData->attributes.specularColor;

			if (E3Bit_IsSet(theMask, kQ3XAttributeMaskSpecularControl))
				resultInstanceData->attributes.specularControl = parentInstanceData->attributes.specularControl;

			if (E3Bit_IsSet(theMask, kQ3XAttributeMaskTransparencyColor))
				resultInstanceData->attributes.trasparencyColor = parentInstanceData->attributes.trasparencyColor;

			if (E3Bit_IsSet(theMask, kQ3XAttributeMaskSurfaceTangent))
				resultInstanceData->attributes.surfaceTangent = parentInstanceData->attributes.surfaceTangent;

			if (E3Bit_IsSet(theMask, kQ3XAttributeMaskHighlightState))
				resultInstanceData->attributes.highlightState = parentInstanceData->attributes.highlightState;

			if (E3Bit_IsSet(theMask, kQ3XAttributeMaskSurfaceShader))
				resultInstanceData->attributes.surfaceShader = Q3Shared_GetReference(parentInstanceData->attributes.surfaceShader);


			// Update the mask in the result
			resultInstanceData->theMask |= theMask;
			}



		// Iterate over any additional elements
		if (parentInstanceData->theTable != NULL)
			{
			paramInfo.theResult = result;
			paramInfo.isChild   = kQ3False;
			qd3dStatus          = e3set_iterate_elements(parentInstanceData, e3attributeset_iterator_inherit, &paramInfo);
			}
		}
	
	return(qd3dStatus);
}





//=============================================================================
//      E3XElementClass_Register : Register an element class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3XObjectClass
E3XElementClass_Register(TQ3ElementType *elementType, const char *name, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler)
{	TQ3Status			qd3dStatus;
	E3ClassInfoPtr		theClass;



	// Allocate a unique type for this class
	*elementType = E3ClassTree_GetNextClassType();



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
											*elementType,
											name,
											metaHandler,
											sizeOfElement);
	if (qd3dStatus != kQ3Success)
		return(NULL);



	// Find the class
	theClass = E3ClassTree_GetClassByType(*elementType);

	return((TQ3XObjectClass) theClass);
}





//=============================================================================
//      E3XElementType_GetElementSize : Get the size of an element.
//-----------------------------------------------------------------------------
TQ3Status
E3XElementType_GetElementSize(TQ3ElementType elementType, TQ3Uns32 *sizeOfElement)
{	E3ClassInfoPtr		theClass;



	// Assign a return value
	*sizeOfElement = 0;



	// Although this is a Q3X method (which are normally intended for external
	// elements registered by applications), QD3D's version of this routine did
	// accept the built-in attribute types for the elementType parameter.
	//
	// To be consistent with QD3D, we provide the same behaviour.
	if ((elementType > kQ3AttributeTypeNone) && (elementType < kQ3AttributeTypeNumTypes))
		elementType = E3Attribute_AttributeToClassType(elementType);



	// Find the class
	theClass = E3ClassTree_GetClassByType(elementType);
	if (theClass == NULL)
		return(kQ3Failure);



	// Get the size of the element	
	*sizeOfElement = E3ClassTree_GetInstanceSize(theClass);

	return(kQ3Success);
}






//=============================================================================
//      E3XAttributeClass_Register : Register an attribute class.
//-----------------------------------------------------------------------------
TQ3XObjectClass
E3XAttributeClass_Register(TQ3AttributeType *attributeType, const char *creatorName, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler)
{	TQ3Status			qd3dStatus;
	E3ClassInfoPtr		theClass;



	// Allocate a unique type for this class
	*attributeType = E3ClassTree_GetNextClassType();



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ElementTypeAttribute,
											*attributeType,
											creatorName,
											metaHandler,
											sizeOfElement);
	if (qd3dStatus != kQ3Success)
		return(NULL);



	// Find the class
	theClass = E3ClassTree_GetClassByType(*attributeType);

	return((TQ3XObjectClass) theClass);
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
{	TQ3SetData			*instanceData;	

	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet),   NULL);
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(attributeSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(NULL);


	switch (attributeType) {
		case kQ3AttributeTypeSurfaceUV:
			return (instanceData->theMask & kQ3XAttributeMaskSurfaceUV) == 0?
					NULL : &instanceData->attributes.surfaceUV;
			break;
		case kQ3AttributeTypeShadingUV:
			return (instanceData->theMask & kQ3XAttributeMaskShadingUV) == 0?
					NULL : &instanceData->attributes.shadingUV;
			break;
		case kQ3AttributeTypeNormal:
			return (instanceData->theMask & kQ3XAttributeMaskNormal) == 0?
					NULL : &instanceData->attributes.normal;
			break;
		case kQ3AttributeTypeAmbientCoefficient:
			return (instanceData->theMask & kQ3XAttributeMaskAmbientCoefficient) == 0?
					NULL : &instanceData->attributes.ambientCoeficient;
			break;
		case kQ3AttributeTypeDiffuseColor:
			return (instanceData->theMask & kQ3XAttributeMaskDiffuseColor) == 0?
					NULL : &instanceData->attributes.diffuseColor;
			break;
		case kQ3AttributeTypeSpecularColor:
			return (instanceData->theMask & kQ3XAttributeMaskSpecularColor) == 0?
					NULL : &instanceData->attributes.specularColor;
			break;
		case kQ3AttributeTypeSpecularControl:
			return (instanceData->theMask & kQ3XAttributeMaskSpecularControl) == 0?
					NULL : &instanceData->attributes.specularControl;
			break;
		case kQ3AttributeTypeTransparencyColor:
			return (instanceData->theMask & kQ3XAttributeMaskTransparencyColor) == 0?
					NULL : &instanceData->attributes.trasparencyColor;
			break;
		case kQ3AttributeTypeSurfaceTangent:
			return (instanceData->theMask & kQ3XAttributeMaskSurfaceTangent) == 0?
					NULL : &instanceData->attributes.surfaceTangent;
			break;
		case kQ3AttributeTypeHighlightState:
			return (instanceData->theMask & kQ3XAttributeMaskHighlightState) == 0?
					NULL : &instanceData->attributes.highlightState;
			break;
		case kQ3AttributeTypeSurfaceShader:
			return (instanceData->theMask & kQ3XAttributeMaskSurfaceShader) == 0?
					NULL : &instanceData->attributes.surfaceShader;
			break;
		}
		
		return NULL;
}





//=============================================================================
//      E3XAttributeSet_GetMask : Get the mask of attributes in a set.
//-----------------------------------------------------------------------------
TQ3XAttributeMask
E3XAttributeSet_GetMask(TQ3AttributeSet attributeSet)
{	TQ3SetData *instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(attributeSet, kQ3SharedTypeSet);



	// Return the mask
	if (instanceData == NULL)
		return(kQ3XAttributeMaskNone);

	return(instanceData->theMask);
}



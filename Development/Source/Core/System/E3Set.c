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
//      Internal types
//-----------------------------------------------------------------------------
// Set instance data
typedef struct TQ3SetData {
	E3HashTablePtr		theTable;			// Elements in set, keyed by type
	TQ3Uns32			scanEditIndex;		// Set edit index while scanning
	TQ3Uns32			scanCount;			// Size of scanResults
	TQ3Uns32			scanIndex;			// Current index into scanResults
	TQ3ElementType		*scanResults;		// Scan results
} TQ3SetData;


// Attribute set instance data
typedef struct TQ3AttributeSetData {
	TQ3XAttributeMask	theMask;			// Attribute mask
} TQ3AttributeSetData;


// Set iterator
typedef TQ3Status (*TQ3SetIterator)(TQ3SetData *instanceData, TQ3ObjectType theType, TQ3ElementObject theElement, void *userData);


// Set iterator param info
typedef struct TQ3SetIteratorParamInfo {
	TQ3SetData			*setInstanceData;	// Set instance data
	TQ3SetIterator		theIterator;		// Set iterator
	void				*iteratorData;		// User data for the set iterator
} TQ3SetIteratorParamInfo;


// Set submit iterator param info
typedef struct TQ3SetSubmitParamInfo {
	TQ3Boolean			isAttributeSet;		// Are we an attribute set?
	TQ3ViewObject		theView;			// View to submit to
} TQ3SetSubmitParamInfo;


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
		instanceData->theTable = E3HashTable_Create(kSetTableSize);
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
		return(kQ3Failure);



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
{	TQ3Boolean					isAttributeSet;
	TQ3Status					qd3dStatus;
	TQ3SetSubmitParamInfo		*paramInfo;
	TQ3ViewObject				theView;



	// Get our param info
	paramInfo      = (TQ3SetSubmitParamInfo *) userData;
	isAttributeSet = paramInfo->isAttributeSet;
	theView        = paramInfo->theView;



	// Submit the element in the appropriate way
	if (isAttributeSet)
		{
		theType    = E3Attribute_ClassToAttributeType(theType);
		qd3dStatus = Q3Attribute_Submit(theType, theElement->instanceData, theView);
		}
	else
		qd3dStatus = E3View_SubmitImmediate(theView, theType, theElement->instanceData);

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
	Q3Set_Empty(theObject);



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
	


	// If there are any elements to copy, duplicate them
	if (fromInstanceData->theTable != NULL)
		{
		qd3dStatus = e3set_iterate_elements((TQ3SetData *) fromInstanceData, e3set_iterator_duplicate, toInstanceData);
		if (qd3dStatus != kQ3Success)
			{
			Q3Set_Empty(toObject);

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
	addElement = isChild || !E3AttributeSet_Contains(theResult, theType);

	if (addElement)
		{
		// Handle built in attributes
		if (theType < kQ3AttributeTypeNumTypes)
			qd3dStatus = Q3AttributeSet_Add(theResult, theType, theElement->instanceData);


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
						qd3dStatus = Q3AttributeSet_Add(theResult, theType, attributeData);
	
					Q3Memory_Free(&attributeData);
					}
	
	
				// Or just copy it directly		
				else
					qd3dStatus = Q3AttributeSet_Add(theResult, theType, theElement->instanceData);
				}
			}



		// Handle failure
		if (qd3dStatus != kQ3Success)
			Q3AttributeSet_Empty(theResult);
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3attributeset_submit : Attribute set class submit method.
//-----------------------------------------------------------------------------
//		Note :	See the comments in E3AttributeSet_Submit for an explanation
//				as to why we don't perform the actual submit here.
//-----------------------------------------------------------------------------
static TQ3Status
e3attributeset_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status		qd3dStatus;
#pragma unused(objectType)
#pragma unused(objectData)



	// Submit the attribute set
	qd3dStatus = Q3AttributeSet_Submit(theObject, theView);

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
	E3Object_DisposeAndForget(*instanceData);
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





//=============================================================================
//      e3attribute_type_to_mask : Convert an attribute type to a mask.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3XAttributeMask
e3attribute_type_to_mask(TQ3AttributeType theType)
{	TQ3XAttributeMask	theMask;



	// Validate our parameters
	Q3_ASSERT(theType != kQ3AttributeTypeNone);



	// Convert the type to the appropriate mask
	theMask = kQ3XAttributeMaskNone;
	switch (theType) {
		case kQ3ObjectTypeAttributeSurfaceUV:
			theMask |= kQ3XAttributeMaskSurfaceUV;
			break;
		case kQ3ObjectTypeAttributeShadingUV:
			theMask |= kQ3XAttributeMaskShadingUV;
			break;
		case kQ3ObjectTypeAttributeNormal:
			theMask |= kQ3XAttributeMaskNormal;
			break;
		case kQ3ObjectTypeAttributeAmbientCoefficient:
			theMask |= kQ3XAttributeMaskAmbientCoefficient;
			break;
		case kQ3ObjectTypeAttributeDiffuseColor:
			theMask |= kQ3XAttributeMaskDiffuseColor;
			break;
		case kQ3ObjectTypeAttributeSpecularColor:
			theMask |= kQ3XAttributeMaskSpecularColor;
			break;
		case kQ3ObjectTypeAttributeSpecularControl:
			theMask |= kQ3XAttributeMaskSpecularControl;
			break;
		case kQ3ObjectTypeAttributeTransparencyColor:
			theMask |= kQ3XAttributeMaskTransparencyColor;
			break;
		case kQ3ObjectTypeAttributeSurfaceTangent:
			theMask |= kQ3XAttributeMaskSurfaceTangent;
			break;
		case kQ3ObjectTypeAttributeHighlightState:
			theMask |= kQ3XAttributeMaskHighlightState;
			break;
		case kQ3ObjectTypeAttributeSurfaceShader:
			theMask |= kQ3XAttributeMaskSurfaceShader;
			break;
		default:
			theMask |= kQ3XAttributeMaskCustomAttribute;
			break;
		}

	return(theMask);
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





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Set_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
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
												sizeof(TQ3AttributeSetData));



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
												kQ3ClassNameElement,
												e3element_metahandler,
												0);

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
//      E3Set_AccessElementData : Access the data for a set element.
//-----------------------------------------------------------------------------
//		Note :	Used by Q3XAttributeSet_GetPointer and Q3Set_Get to get a
//				pointer to the data, and the size of the data, for an element
//				in a set.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ElementObject
E3Set_AccessElementData(TQ3SetObject theSet, TQ3ElementType theType, TQ3Uns32 *dataSize, void **data)
{	TQ3SetData			*instanceData;
	TQ3ElementObject	theElement;
	E3ClassInfoPtr		theClass;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theSet),   NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dataSize), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data),     NULL);



	// Set up some return values
	*dataSize = 0;
	*data     = NULL;



	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(theSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(NULL);



	// Find the element
	theElement = e3set_find_element(instanceData, theType);
	if (theElement != NULL)
		{
		// Find the class for the element
		theClass = theElement->theClass;
		if (theClass == NULL)
			return(NULL);


		// Return the size and pointer for the data for the element
		*dataSize = E3ClassTree_GetInstanceSize(theClass);
		*data     = theElement->instanceData;
		}
	
	return(theElement);
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
	TQ3Status							qd3dStatus;
	TQ3ElementObject					theElement;
	TQ3Uns32							dataSize;



	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(theSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(kQ3Failure);



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
			Q3Memory_Copy(data, theElement->instanceData, dataSize);
			qd3dStatus = kQ3Success;
			}
		
		Q3Shared_Edited(theSet);
		return(qd3dStatus);
		}



	// We don't have an existing element, so instantiate a new one
	theElement = E3ClassTree_CreateInstance(theType, kQ3False, data);
	if (theElement == NULL)
		return(kQ3Failure);



	// And add it to the set
	qd3dStatus = e3set_add_element(instanceData, theType, theElement);
	Q3Shared_Edited(theSet);

	return(qd3dStatus);
}





//=============================================================================
//      E3Set_Get : Get the data for an element in a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_Get(TQ3SetObject theSet, TQ3ElementType theType, void *data)
{	TQ3XElementCopyGetMethod	copyGetMethod;
	void						*elementData;
	TQ3Status					qd3dStatus;
	TQ3ElementObject			theElement;
	TQ3Uns32					dataSize;



	// Get the size and pointer for the data for the element
	theElement = E3Set_AccessElementData(theSet, theType, &dataSize, &elementData);
	if (theElement == NULL)
		return(kQ3Failure);



	// If there's nothing to copy, bail. It is OK for dataSize to be 0, as the
	// mere presence of an attribute can itself carry information.
	if (dataSize == 0)
		return(kQ3Success);



	// Copy the element data
	copyGetMethod = (TQ3XElementCopyGetMethod) E3ClassTree_GetMethod(theElement->theClass,
														  kQ3XMethodTypeElementCopyGet);
	if (copyGetMethod != NULL)
		qd3dStatus = copyGetMethod((void *) elementData, (void *) data);
	else
		Q3Memory_Copy(elementData, data, dataSize);
	
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



	// Find the element to copy
	srcElement = e3set_find_element(srcInstanceData, theType);
	if (srcElement == NULL)
		return(kQ3Failure);



	// If the destination has an element of this type, remove it
	Q3Set_Clear(destSet, theType);



	// Duplicate the element, and add it to the set
	dstElement = Q3Object_Duplicate(srcElement);
	if (dstElement != NULL)
		qd3dStatus = e3set_add_element(dstInstanceData, theType, dstElement);
	else
		qd3dStatus = kQ3Failure;
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Set_Contains : Does a set contain an element?
//-----------------------------------------------------------------------------
TQ3Boolean
E3Set_Contains(TQ3SetObject theSet, TQ3ElementType theType)
{	TQ3SetData			*instanceData;
	TQ3ElementObject	theElement;



	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(theSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(kQ3False);



	// Find the element
	theElement = e3set_find_element(instanceData, theType);
	return((TQ3Boolean) (theElement != NULL));
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



	// Remove the element
	theElement = e3set_remove_element(instanceData, theType);
	if (theElement != NULL)
		{
		Q3Object_Dispose(theElement);
		Q3Shared_Edited(theSet);
		}

	return(theElement != NULL ? kQ3Success : kQ3Failure);
}





//=============================================================================
//      E3Set_Empty : Remove everything from a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_Empty(TQ3SetObject theSet)
{	TQ3SetData		*instanceData;



	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(theSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Remove the elements from the set
	e3set_iterate_elements(instanceData, e3set_iterator_delete, NULL);
	e3set_clear_elements(instanceData);
	
	Q3Shared_Edited(theSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Set_GetNextElementType : Get the next element type in a set.
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
E3Set_GetNextElementType(TQ3SetObject theSet, TQ3ElementType *theType)
{	TQ3SetData			*instanceData;
	TQ3Uns32			editIndex;



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


		// Build the array of types in the set
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
//      E3Set_SubmitElements : Submit the elements of a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_SubmitElements( TQ3SetObject inSet, TQ3ViewObject inView )
{	TQ3SetData					*instanceData;
	TQ3Status					qd3dStatus;
	TQ3SetSubmitParamInfo		paramInfo;



	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(inSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Submit the elements in the set
	paramInfo.isAttributeSet = kQ3False;
	paramInfo.theView        = inView;

	qd3dStatus = e3set_iterate_elements(instanceData, e3set_iterator_submit, &paramInfo);
	
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
	if ( (theType >= 0) && (theType < kQ3AttributeTypeNumTypes))
		{
		Q3_ASSERT(theType == E3Attribute_ClassToAttributeType(attributeTypes[theType]));
		theType = attributeTypes[theType];
		}

	return(theType);
}





//=============================================================================
//      E3AttributeSet_AccessMask : Access the mask of an attribute set.
//-----------------------------------------------------------------------------
TQ3XAttributeMask
E3AttributeSet_AccessMask(TQ3AttributeSet attributeSet)
{	TQ3AttributeSetData		*instanceData = (TQ3AttributeSetData *) attributeSet->instanceData;



	// Return the mask
	return(instanceData->theMask);
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
//      E3AttributeSet_Add : Add an attribute to an attribute set.
//-----------------------------------------------------------------------------
TQ3Status
E3AttributeSet_Add(TQ3AttributeSet attributeSet, TQ3AttributeType theType, const void *data)
{	TQ3AttributeSetData		*instanceData = (TQ3AttributeSetData *) attributeSet->instanceData;
	TQ3Status				qd3dStatus;



	// Add the attribute and update our mask
	theType    = E3Attribute_AttributeToClassType(theType);
	qd3dStatus = Q3Set_Add(attributeSet, theType, data);

	if (qd3dStatus == kQ3Success)
		instanceData->theMask |= e3attribute_type_to_mask(theType);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3AttributeSet_Contains : Does a set contain an attribute?
//-----------------------------------------------------------------------------
TQ3Boolean
E3AttributeSet_Contains(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType)
{	TQ3AttributeSetData		*instanceData = (TQ3AttributeSetData *) attributeSet->instanceData;
	TQ3Boolean				inSet;



	// Built-in attributes
	if (attributeType < kQ3AttributeTypeNumTypes)
		{
		// Test the mask
		attributeType = E3Attribute_AttributeToClassType(attributeType);
		inSet         = (TQ3Boolean) ((instanceData->theMask & e3attribute_type_to_mask(attributeType)) != 0);


		// In debug builds, double-check the underlying set
		Q3_ASSERT(inSet == Q3Set_Contains(attributeSet, attributeType));
		}
	
	// Custom attributes
	else
		inSet = Q3Set_Contains(attributeSet, attributeType);
	
	return(inSet);
}





//=============================================================================
//      E3AttributeSet_Get : Get the data for an attribute in a set.
//-----------------------------------------------------------------------------
TQ3Status
E3AttributeSet_Get(TQ3AttributeSet attributeSet, TQ3AttributeType theType, void *data)
{	TQ3Status		qd3dStatus;



	// Get the data
	theType    = E3Attribute_AttributeToClassType(theType);
	qd3dStatus = Q3Set_Get(attributeSet, theType, data);

	return(qd3dStatus);
}





//=============================================================================
//      E3AttributeSet_Clear : Remove an attribute from a set.
//-----------------------------------------------------------------------------
TQ3Status
E3AttributeSet_Clear(TQ3AttributeSet attributeSet, TQ3AttributeType theType)
{	TQ3AttributeSetData		*instanceData = (TQ3AttributeSetData *) attributeSet->instanceData;
	TQ3Status				qd3dStatus;



	// Remove the attribute and update our mask
	theType    = E3Attribute_AttributeToClassType(theType);
	qd3dStatus = Q3Set_Clear(attributeSet, theType);

	if (qd3dStatus == kQ3Success)
		instanceData->theMask &= ~e3attribute_type_to_mask(theType);

	return(qd3dStatus);
}





//=============================================================================
//      E3AttributeSet_Empty : Remove everything from a set.
//-----------------------------------------------------------------------------
TQ3Status
E3AttributeSet_Empty(TQ3AttributeSet attributeSet)
{	TQ3AttributeSetData		*instanceData = (TQ3AttributeSetData *) attributeSet->instanceData;
	TQ3Status				qd3dStatus;



	// Empty the set and update our mask
	qd3dStatus = Q3Set_Empty(attributeSet);
	if (qd3dStatus == kQ3Success)
		instanceData->theMask = kQ3XAttributeMaskNone;

	return(qd3dStatus);
}





//=============================================================================
//      E3AttributeSet_GetNextAttributeType : Get the next attribute type.
//-----------------------------------------------------------------------------
TQ3Status
E3AttributeSet_GetNextAttributeType(TQ3AttributeSet source, TQ3AttributeType *theType)
{	TQ3Status		qd3dStatus;



	// Get the next attribute type
	*theType   = E3Attribute_AttributeToClassType(*theType);
	qd3dStatus = Q3Set_GetNextElementType(source, theType);
	*theType   = E3Attribute_ClassToAttributeType(*theType);

	return(qd3dStatus);
}





//=============================================================================
//      E3AttributeSet_Submit : Submit an attribute set.
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
E3AttributeSet_Submit(TQ3AttributeSet attributeSet, TQ3ViewObject view)
{	TQ3SetData					*instanceData;
	TQ3Status					qd3dStatus;
	TQ3SetSubmitParamInfo		paramInfo;



	// Find the instance data
	instanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(attributeSet, kQ3SharedTypeSet);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Submit the elements in the set
	paramInfo.isAttributeSet = kQ3True;
	paramInfo.theView        = view;

	qd3dStatus = e3set_iterate_elements(instanceData, e3set_iterator_submit, &paramInfo);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3AttributeSet_Inherit : Merge two attribute sets.
//-----------------------------------------------------------------------------
//		Note :	Should really use the kQ3MethodTypeAttributeCopyInherit method
//				for custom attributes.
//-----------------------------------------------------------------------------
TQ3Status
E3AttributeSet_Inherit(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet result)
{	TQ3SetData							*parentInstanceData, *childInstanceData;
	TQ3Status							qd3dStatus;
	TQ3AttributeSetInheritParamInfo		paramInfo;



	// Empty the final attribute set
	qd3dStatus = Q3AttributeSet_Empty(result);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Find the instance data
	parentInstanceData = (TQ3SetData *) E3ClassTree_FindInstanceData(parent, kQ3SharedTypeSet);
	childInstanceData  = (TQ3SetData *) E3ClassTree_FindInstanceData(child,  kQ3SharedTypeSet);

	if (parentInstanceData == NULL || childInstanceData == NULL)
		return(kQ3Failure);



	// Iterate over the child
	if (qd3dStatus == kQ3Success)
		{
		paramInfo.theResult = result;
		paramInfo.isChild   = kQ3True;
		qd3dStatus          = e3set_iterate_elements(childInstanceData, e3attributeset_iterator_inherit, &paramInfo);
		}



	// Iterate over the parent
	if (qd3dStatus == kQ3Success)
		{
		paramInfo.theResult = result;
		paramInfo.isChild   = kQ3False;
		qd3dStatus          = e3set_iterate_elements(parentInstanceData, e3attributeset_iterator_inherit, &paramInfo);
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


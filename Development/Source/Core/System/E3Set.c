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
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Set data
typedef struct {
	TQ3Uns32			numElements;		// Element count
	TQ3ElementObject	*theElements;		// Element array (may contain empty slots)
} TQ3SetData;


// Attribute set data
typedef struct {
	TQ3XAttributeMask	theMask;			// Attribute mask
} TQ3AttributeSetData;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3set_findinstance : Get the set data for a set object.
//-----------------------------------------------------------------------------
//		Note : Given a set object, we locate the set instance data.
//-----------------------------------------------------------------------------
static TQ3SetData *
e3set_findinstance(TQ3SetObject theSet)
{	TQ3SetData		*instanceData;
	TQ3SetObject	theObject;



	// Find the set object for this object. This may in fact be
	// theSet, but we need to walk upwards in case it isn't.
	theObject = E3ClassTree_FindParentInstance(theSet, kQ3SharedTypeSet);
	if (theObject == NULL)
		return(NULL);



	// Get the instance data for the set object		
	instanceData = (TQ3SetData *) theObject->instanceData;
	Q3_ASSERT_VALID_PTR(instanceData);
	Q3_ASSERT_VALID_PTR(E3ClassTree_GetInstanceSize(theObject->theClass)
						== sizeof(TQ3SetData));

	return(instanceData);
}





//=============================================================================
//      e3set_delete : Set class delete method.
//-----------------------------------------------------------------------------
static void
e3set_delete(TQ3Object theObject, void *privateData)
{	TQ3SetData		*instanceData = (TQ3SetData *) privateData;
	TQ3Uns32		n;
#pragma unused(theObject)



	// Dispose of our instance data
	for (n = 0; n < instanceData->numElements; n++)
		{
		if (instanceData->theElements[n] != NULL)
			Q3Object_Dispose(instanceData->theElements[n]);
		}
}





//=============================================================================
//      e3set_duplicate : Set class duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3set_duplicate(TQ3Object fromObject,    const void *fromPrivateData,
					 TQ3Object toObject, void       *toPrivateData)
{	const TQ3SetData	*fromInstanceData = (const TQ3SetData *) fromPrivateData;
	TQ3SetData			*toInstanceData   = (TQ3SetData *)       toPrivateData;
	TQ3Status			qd3dStatus;
	TQ3Uns32			n;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	toInstanceData->numElements = fromInstanceData->numElements;
	toInstanceData->theElements = NULL;



	// If there are any elements to copy, duplicate them
	if (toInstanceData->numElements != 0)
		{
		// Allocate the memory (cleared, since the set might have unused slots)
		toInstanceData->theElements = (TQ3ElementObject *) E3Memory_AllocateClear(sizeof(TQ3ElementObject) * toInstanceData->numElements);
		if (toInstanceData->theElements == NULL)
			return(kQ3Failure);



		// Duplicate the elements
		qd3dStatus = kQ3Success;
		for (n = 0; n < toInstanceData->numElements && qd3dStatus == kQ3Success; n++)
			{
			if (fromInstanceData->theElements[n] != NULL)
				{
				toInstanceData->theElements[n] = Q3Object_Duplicate(fromInstanceData->theElements[n]);
				if (toInstanceData->theElements[n] == NULL)
					qd3dStatus = kQ3Failure;				
				}
			}



		// Handle failure
		if (qd3dStatus != kQ3Success)
			{
			// Dispose of any elements we managed to duplicate
			for (n = 0; n < toInstanceData->numElements; n++)
				{
				if (toInstanceData->theElements[n] != NULL)
					Q3Object_Dispose(toInstanceData->theElements[n]);
				}
			
			
			// Dispose of the set data
			E3Memory_Free(&toInstanceData->theElements);
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
//      e3attributeset_render : Attribute set class render method.
//-----------------------------------------------------------------------------
//		Note :	See the comments in E3AttributeSet_Submit for an explanation
//				as to why we don't perform the actual submit here.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attributeset_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
		case kQ3XMethodTypeObjectSubmitBounds:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitRender:
			theMethod = (TQ3XFunctionPointer) e3attributeset_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_surfaceuv_render : Surface UV render method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_surfaceuv_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3attribute_surfaceuv_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_shadinguv_render : Shading UV render method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_shadinguv_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3attribute_shadinguv_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_normal_render : Normal render method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_normal_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3attribute_normal_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_ambientcoefficient_render : Ambient coeff render method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_ambientcoefficient_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3attribute_ambientcoefficient_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_diffusecolor_render : Diffuse colour render method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_diffusecolor_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3attribute_diffusecolor_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_specularcolor_render : Specular colour render method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_specularcolor_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3attribute_specularcolor_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_specularcontrol_render : Specular control render method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_specularcontrol_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3attribute_specularcontrol_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_transparencycolor_render : Transparency  render method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_transparencycolor_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3attribute_transparencycolor_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_surfacetangent_render : Surface tangent render method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_surfacetangent_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3attribute_surfacetangent_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3attribute_highlightstate_render : Highlight state render method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3attribute_highlightstate_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3attribute_highlightstate_render;
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
//      e3attribute_surfaceshader_render : Surface shader render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3attribute_surfaceshader_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3attribute_surfaceshader_render;
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
												NULL,
												0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ObjectTypeAttributeSurfaceUV,
												kQ3ClassNameAttributeSurfaceUV,
												e3attribute_surfaceuv_metahandler,
												sizeof(TQ3Param2D));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ObjectTypeAttributeShadingUV,
												kQ3ClassNameAttributeShadingUV,
												e3attribute_shadinguv_metahandler,
												sizeof(TQ3Param2D));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ObjectTypeAttributeNormal,
												kQ3ClassNameAttributeNormal,
												e3attribute_normal_metahandler,
												sizeof(TQ3Vector3D));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ObjectTypeAttributeAmbientCoefficient,
												kQ3ClassNameAttributeAmbientCoefficient,
												e3attribute_ambientcoefficient_metahandler,
												sizeof(float));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ObjectTypeAttributeDiffuseColor,
												kQ3ClassNameAttributeDiffuseColor,
												e3attribute_diffusecolor_metahandler,
												sizeof(TQ3ColorRGB));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ObjectTypeAttributeSpecularColor,
												kQ3ClassNameAttributeSpecularColor,
												e3attribute_specularcolor_metahandler,
												sizeof(TQ3ColorRGB));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ObjectTypeAttributeSpecularControl,
												kQ3ClassNameAttributeSpecularControl,
												e3attribute_specularcontrol_metahandler,
												sizeof(float));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ObjectTypeAttributeTransparencyColor,
												kQ3ClassNameAttributeTransparencyColor,
												e3attribute_transparencycolor_metahandler,
												sizeof(TQ3ColorRGB));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ObjectTypeAttributeSurfaceTangent,
												kQ3ClassNameAttributeSurfaceTangent,
												e3attribute_surfacetangent_metahandler,
												sizeof(TQ3Tangent2D));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
												kQ3ObjectTypeAttributeHighlightState,
												kQ3ClassNameAttributeHighlightState,
												e3attribute_highlightstate_metahandler,
												sizeof(TQ3Switch));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
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
TQ3Status
E3Set_AccessElementData(TQ3SetObject theSet, TQ3ElementType theType, TQ3Uns32 *dataSize, void **data)
{	TQ3SetData			*instanceData;
	TQ3ElementObject	theElement;
	E3ClassInfoPtr		theClass;
	TQ3Uns32			n;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theSet),  kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dataSize), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data),     kQ3Failure);



	// Set up some return values
	*dataSize = 0;
	*data     = NULL;



	// Find the instance data
	instanceData = e3set_findinstance(theSet);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Find the element
	theElement = NULL;
	for (n = 0; n < instanceData->numElements; n++)
		{
		if (instanceData->theElements[n] != NULL)
			{
			if (Q3Object_IsType(instanceData->theElements[n], theType))
				{
				theElement = instanceData->theElements[n];
				break;
				}
			}
		}

	if (theElement == NULL)
		return(kQ3Failure);



	// Find the class for the element
	theClass = E3ClassTree_GetClassByType(theType);
	if (theClass == NULL)
		return(kQ3Failure);



	// Return the size and pointer for the data for the element
	*dataSize = E3ClassTree_GetInstanceSize(theClass);
	*data     = theElement->instanceData;

	return(kQ3Success);
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
	TQ3Uns32							n, dataSize;
	TQ3Status							qd3dStatus;
	TQ3ElementObject					theElement;
	E3ClassInfoPtr						theClass;



	// Find the instance data
	instanceData = e3set_findinstance(theSet);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Find the class
	theClass = E3ClassTree_GetClassByType(theType);
	if (theClass == NULL)
		return(kQ3Failure);



	// If the element exists, replace it
	for (n = 0; n < instanceData->numElements; n++)
		{
		if (instanceData->theElements[n] != NULL)
			{
			if (Q3Object_IsType(instanceData->theElements[n], theType))
				{
				copyReplaceMethod = (TQ3XElementCopyReplaceMethod) E3ClassTree_GetMethod(theClass,
																	  kQ3XMethodTypeElementCopyReplace);
				if (copyReplaceMethod != NULL)
					{
					qd3dStatus = copyReplaceMethod(data, instanceData->theElements[n]->instanceData);
					return(qd3dStatus);
					}
				else
					{
					dataSize = E3ClassTree_GetInstanceSize(theClass);
					memcpy(instanceData->theElements[n]->instanceData, data, dataSize);
					}
				
				Q3Shared_Edited(theSet);
				return(kQ3Success);
				}
			}
		}



	// If we're still here, the element doesn't exist. So, try and
	// find an empty slot that we can use to store it into.
	for (n = 0; n < instanceData->numElements; n++)
		{
		if (instanceData->theElements[n] == NULL)
			break;
		}



	// If we didn't find an empty slot, create one
	if (n == instanceData->numElements)
		{
		// Grow the array
		qd3dStatus = E3Memory_Reallocate(&instanceData->theElements,
										 sizeof(TQ3ElementObject) * (instanceData->numElements+1));
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);
		
		
		// Increment the count - n now refers to the last slot
		instanceData->theElements[n] = NULL;
		instanceData->numElements++;
		}



	// Create a new element
	theElement = E3ClassTree_CreateInstance(theType, kQ3False, data);
	if (theElement == NULL)
		return(kQ3Failure);



	// Save the element in the empty slot	
	instanceData->theElements[n] = theElement;
	Q3Shared_Edited(theSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Set_Get : Get the data for an element in a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_Get(TQ3SetObject theSet, TQ3ElementType theType, void *data)
{	TQ3XElementCopyGetMethod	copyGetMethod;
	void						*elementData;
	TQ3Status					qd3dStatus;
	TQ3Uns32					dataSize;
	E3ClassInfoPtr				theClass;



	// Find the class
	theClass = E3ClassTree_GetClassByType(theType);
	if (theClass == NULL)
		return(kQ3Failure);



	// Get the size and pointer for the data for the element
	qd3dStatus = E3Set_AccessElementData(theSet, theType, &dataSize, &elementData);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// If there's nothing to copy, bail
	if (dataSize == 0)
		return(kQ3Failure);



	// Copy the element data
	copyGetMethod = (TQ3XElementCopyGetMethod) E3ClassTree_GetMethod(theClass,
														  kQ3XMethodTypeElementCopyGet);
	if (copyGetMethod != NULL)
		qd3dStatus = copyGetMethod((void *) elementData, (void *) data);
	else
		memcpy(data, elementData, dataSize);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Set_Contains : Does a set contain an element?
//-----------------------------------------------------------------------------
TQ3Boolean
E3Set_Contains(TQ3SetObject theSet, TQ3ElementType theType)
{	TQ3SetData		*instanceData;
	TQ3Uns32		n;



	// Find the instance data
	instanceData = e3set_findinstance(theSet);
	if (instanceData == NULL)
		return(kQ3False);



	// Look for the element
	for (n = 0; n < instanceData->numElements; n++)
		{
		if (instanceData->theElements[n] != NULL)
			{
			if (Q3Object_IsType(instanceData->theElements[n], theType))
				return(kQ3True);
			}
		}

	return(kQ3False);
}





//=============================================================================
//      E3Set_Clear : Remove an element from a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_Clear(TQ3SetObject theSet, TQ3ElementType theType)
{	TQ3SetData		*instanceData;
	TQ3Uns32		n;



	// Find the instance data
	instanceData = e3set_findinstance(theSet);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Remove the element
	for (n = 0; n < instanceData->numElements; n++)
		{
		if (instanceData->theElements[n] != NULL)
			{
			if (Q3Object_IsType(instanceData->theElements[n], theType))
				{
				Q3Object_Dispose(instanceData->theElements[n]);
				instanceData->theElements[n] = NULL;

				Q3Shared_Edited(theSet);
				return(kQ3Success);
				}
			}
		}

	return(kQ3Failure);
}





//=============================================================================
//      E3Set_Empty : Remove everything from a set.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_Empty(TQ3SetObject theSet)
{	TQ3SetData		*instanceData;
	TQ3Uns32		n;



	// Find the instance data
	instanceData = e3set_findinstance(theSet);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Remove the elements
	for (n = 0; n < instanceData->numElements; n++)
		{
		if (instanceData->theElements[n] != NULL)
			{
			Q3Object_Dispose(instanceData->theElements[n]);
			instanceData->theElements[n] = NULL;
			}
		}



	// Remove the element array from the set
	instanceData->numElements = 0;
	E3Memory_Free(&instanceData->theElements);



	// Bump the edit index
	Q3Shared_Edited(theSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Set_GetNextElementType : Get the next element type in a set.
//-----------------------------------------------------------------------------
//		Note :	Currently performs a linear search from the start of the set
//				on each invokation. This could be improved by caching the
//				index of the previous search, but we'd also need to reset the
//				index if the set was modified during a search.
//-----------------------------------------------------------------------------
TQ3Status
E3Set_GetNextElementType(TQ3SetObject theSet, TQ3ElementType *theType)
{	TQ3SetData		*instanceData;
	TQ3Uns32		n;



	// Find the instance data
	instanceData = e3set_findinstance(theSet);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Check for empty sets
	if (instanceData->numElements == 0)
		{
		*theType = kQ3ElementTypeNone;
		return(kQ3Success);
		}



	// Find the 'current' element. This should probably be cached
	// so that we don't start from the beginning each time.
	if (*theType == kQ3ElementTypeNone)
		n = 0;
	else
		{
		for (n = 0; n < instanceData->numElements; n++)
			{
			if (instanceData->theElements[n] != NULL)
				{
				if (Q3Object_IsType(instanceData->theElements[n], *theType))
					break;
				}
			}
		n++;
		}



	// Skip any empty slots
	while (n < instanceData->numElements && instanceData->theElements[n] == NULL)
		n++;



	// Return the next element, if any
	if (n >= instanceData->numElements)
		*theType = kQ3ElementTypeNone;
	else
		{
		Q3_ASSERT_VALID_PTR(instanceData->theElements[n]);
		*theType = Q3Object_GetLeafType(instanceData->theElements[n]);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3Attribute_ClassToAttributeType : Get the attribut type for a class.
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
	if (theType < kQ3AttributeTypeNumTypes)
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
{	TQ3SetData			*instanceData;
	TQ3ObjectType		theType;
	TQ3Uns32			n;



	// Find the instance data for the set
	instanceData = e3set_findinstance(attributeSet);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Submit the attributes
	for (n = 0; n < instanceData->numElements; n++)
		{
		if (instanceData->theElements[n] != NULL)
			{
			theType = Q3Object_GetLeafType(instanceData->theElements[n]);
			theType = E3Attribute_ClassToAttributeType(theType);
			Q3Attribute_Submit(theType, instanceData->theElements[n]->instanceData, view);
			}
		}

	return(kQ3Success);
}





//=============================================================================
//      E3AttributeSet_Inherit : Merge two attribute sets.
//-----------------------------------------------------------------------------
//		Note :	Should really use the kQ3MethodTypeAttributeCopyInherit method
//				for custom attributes.
//-----------------------------------------------------------------------------
TQ3Status
E3AttributeSet_Inherit(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet result)
{	TQ3SetData						*parentInstanceData, *childInstanceData;
	TQ3XAttributeCopyInheritMethod 	copyInheritMethod;
	void 							*attributeData;
	TQ3XAttributeInheritMethod 		inheritMethod;
	TQ3Status						qd3dStatus;
	TQ3ObjectType					theType;
	TQ3Uns32						n;



	// Empty the final attribute set
	qd3dStatus = Q3AttributeSet_Empty(result);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Find the instance data
	parentInstanceData = e3set_findinstance(parent);
	childInstanceData  = e3set_findinstance(child);

	if (parentInstanceData == NULL || childInstanceData == NULL)
		return(kQ3Failure);



	// For each attribute in child, copy it into result
	for (n = 0; n < childInstanceData->numElements; n++)
		{
		if (childInstanceData->theElements[n] != NULL)
			{
			theType = Q3Object_GetLeafType(childInstanceData->theElements[n]);
			theType = E3Attribute_ClassToAttributeType(theType);


			// Built in attributes
			if (theType < kQ3AttributeTypeNumTypes)
				qd3dStatus = Q3AttributeSet_Add(result, theType, childInstanceData->theElements[n]->instanceData);


			// Custom attributes
			else
				{
				inheritMethod = (TQ3XAttributeInheritMethod) E3ClassTree_GetMethod(
																	childInstanceData->theElements[n]->theClass,
																	kQ3XMethodTypeAttributeInherit);
				if (inheritMethod == kQ3True) // We have to inherit.
					{
					copyInheritMethod = (TQ3XAttributeCopyInheritMethod) E3ClassTree_GetMethod(
																childInstanceData->theElements[n]->theClass,
																kQ3XMethodTypeAttributeCopyInherit);
					if (copyInheritMethod != NULL) // Use copy inherit method to copy the attribute.
						{
						qd3dStatus    = kQ3Failure;
						attributeData = E3Memory_AllocateClear(E3ClassTree_GetInstanceSize(childInstanceData->theElements[n]->theClass));

						if (attributeData != NULL)
							qd3dStatus = copyInheritMethod(childInstanceData->theElements[n]->instanceData, attributeData); 

						if (qd3dStatus == kQ3Success)
							qd3dStatus = Q3AttributeSet_Add(result, theType, attributeData);

						E3Memory_Free(&attributeData);
						}
					
					else // There's no copy inherit method, so copy it directly into result.
						qd3dStatus = Q3AttributeSet_Add(result, theType, childInstanceData->theElements[n]->instanceData);
						
					}
				}
				
			
			// Clean up
			if (qd3dStatus != kQ3Success)
				{
				Q3AttributeSet_Empty(result);
				return(qd3dStatus);
				}
			}
		}



	// For each attribute in parent, copy it in if it's not already there
	for (n = 0; n < parentInstanceData->numElements; n++)
		{
		if (parentInstanceData->theElements[n] != NULL)
			{
			theType = Q3Object_GetLeafType(parentInstanceData->theElements[n]);
			theType = E3Attribute_ClassToAttributeType(theType);
			if (!E3AttributeSet_Contains(result, theType))
				{
				// Built in attributes
				if (theType < kQ3AttributeTypeNumTypes)
					qd3dStatus = Q3AttributeSet_Add(result, theType, parentInstanceData->theElements[n]->instanceData);


				// Custom attributes
				else
					{
					inheritMethod = (TQ3XAttributeInheritMethod) E3ClassTree_GetMethod(
																			parentInstanceData->theElements[n]->theClass,
																		  	kQ3XMethodTypeAttributeInherit);
					if (inheritMethod == kQ3True) // We have to inherit.
						{
						copyInheritMethod = (TQ3XAttributeCopyInheritMethod) E3ClassTree_GetMethod(
																				parentInstanceData->theElements[n]->theClass,
																			 	 kQ3XMethodTypeAttributeCopyInherit);
						if (copyInheritMethod != NULL) // Use copy inherit method to copy the attribute.
							{
							qd3dStatus    = kQ3Failure;
							attributeData = E3Memory_AllocateClear(E3ClassTree_GetInstanceSize(childInstanceData->theElements[n]->theClass));

							if (attributeData != NULL)
								qd3dStatus = copyInheritMethod(parentInstanceData->theElements[n]->instanceData, attributeData); 

							if (qd3dStatus == kQ3Success)
								qd3dStatus = Q3AttributeSet_Add(result, theType, attributeData);

							E3Memory_Free(&attributeData);
							}
						
						else // There's no copy inherit method, so copy it directly into result.
							qd3dStatus = Q3AttributeSet_Add(result, theType, parentInstanceData->theElements[n]->instanceData);
							
						}
					}
				
				
				// Clean up
				if (qd3dStatus != kQ3Success)
					{
					Q3AttributeSet_Empty(result);
					return(qd3dStatus);
					}
				}
			}
		}

	return(kQ3Success);
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
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeElement,
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






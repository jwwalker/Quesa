/*  NAME:
        E3View.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
#include "E3Geometry.h"
#include "E3Renderer.h"
#include "E3DrawContext.h"
#include "E3Transform.h"
#include "E3IOFileFormat.h"
#include "E3Pick.h"
#include "E3View.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Misc
#define kApproxBoundsThreshold								12


// View stack
enum TQ3ViewStackState {
	kQ3ViewStateMatrixLocalToWorld			= 1 <<  0,		// Local to world changed
	kQ3ViewStateShaderIllumination			= 1 <<	1,		// Illumination shader changed
	kQ3ViewStateShaderSurface				= 1 <<	2,		// Surface shader changed
	kQ3ViewStateStyleBackfacing				= 1 <<  3,		// Backfacing style changed
	kQ3ViewStateStyleInterpolation			= 1 <<  4,		// Interpolation style changed
	kQ3ViewStateStyleFill					= 1 <<  5,		// Fill style changed
	kQ3ViewStateStyleHighlight				= 1 <<  6,		// Highlight style changed
	kQ3ViewStateStyleSubdivision			= 1 <<  7,		// Subdivision style changed
	kQ3ViewStateStyleOrientation			= 1 <<  8,		// Orientation style changed
	kQ3ViewStateStyleReceiveShadows			= 1 <<  9,		// ReceiveShadows style changed
	kQ3ViewStateStylePickID					= 1 << 10,		// Pick ID style changed
	kQ3ViewStateStylePickParts				= 1 << 11,		// Pick Parts style changed
	kQ3ViewStateStyleAntiAlias				= 1 << 12,		// Anti-alias style changed
	kQ3ViewStateStyleFog					= 1 << 13,		// Fog style changed
	kQ3ViewStateAttributeSurfaceUV			= 1 << 14,		// Surface UV attribute changed
	kQ3ViewStateAttributeShadingUV			= 1 << 15,		// Shading UV attribute changed
	kQ3ViewStateAttributeNormal				= 1 << 16,		// Normal attribute changed
	kQ3ViewStateAttributeAmbientCoefficient	= 1 << 17,		// Ambient coefficient attribute changed
	kQ3ViewStateAttributeDiffuseColour		= 1 << 18,		// Diffuse colour attribute changed
	kQ3ViewStateAttributeSpecularColour		= 1 << 19,		// Specular colour attribute changed
	kQ3ViewStateAttributeSpecularControl	= 1 << 20,		// Specular control attribute changed
	kQ3ViewStateAttributeTransparencyColour = 1 << 21,		// Transparency colour attribute changed
	kQ3ViewStateAttributeSurfaceTangent		= 1 << 22,		// Surface tangent attribute changed
	kQ3ViewStateAttributeHighlightState		= 1 << 23,		// Highlight switch attribute changed
	kQ3ViewStateAttributeSurfaceShader		= 1 << 24,		// Surface shader attribute changed
	kQ3ViewStateNone						= 0,			// Nothing changed
	kQ3ViewStateAll							= 0xFFFFFFFF	// Everything changed
};
typedef enum TQ3ViewStackState TQ3ViewStackState;





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Stack data
typedef struct TQ3ViewStackItem {
	// Next stack item
	struct TQ3ViewStackItem*	next;
	
	
	// Stack item state
	TQ3ViewStackState			stackState;
	TQ3AttributeSet				attributeSet;
	TQ3Matrix4x4				matrixLocalToWorld;
	TQ3ShaderObject				shaderIllumination;
	TQ3ShaderObject				shaderSurface;
	TQ3BackfacingStyle			styleBackfacing;
	TQ3InterpolationStyle		styleInterpolation;
	TQ3FillStyle				styleFill;
	TQ3AttributeSet				styleHighlight;
	TQ3SubdivisionStyleData		styleSubdivision;
	TQ3OrientationStyle			styleOrientation;
	TQ3Boolean					styleReceiveShadows;
	TQ3Uns32					stylePickID;
	TQ3PickParts				stylePickParts;
	TQ3AntiAliasStyleData		styleAntiAlias;
	TQ3FogStyleData				styleFog;
	TQ3Param2D					attributeSurfaceUV;
	TQ3Param2D					attributeShadingUV;
	TQ3Vector3D					attributeNormal;
	float						attributeAmbientCoefficient;
	TQ3ColorRGB					attributeDiffuseColor;
	TQ3ColorRGB					attributeSpecularColor;
	float						attributeSpecularControl;
	TQ3ColorRGB					attributeTransparencyColor;
	TQ3Tangent2D				attributeSurfaceTangent;
	TQ3Switch					attributeHighlightState;
	TQ3SurfaceShaderObject		attributeSurfaceShader;
} TQ3ViewStackItem;


// View data
typedef struct TQ3ViewData {
	// View state
	TQ3ViewMode					viewMode;
	TQ3ViewState				viewState;
	TQ3Uns32					viewPass;
	TQ3AttributeSet				viewAttributes;
	TQ3Boolean					allowGroupCulling;


	// View stack
	TQ3ViewStackItem			*stackState;


	// Bounds state
	TQ3BoundingMethod			boundingMethod;
	TQ3BoundingBox				boundingBox;
	TQ3BoundingSphere			boundingSphere;
	
	
	// Pick state
	TQ3PickObject				thePick;
	TQ3HitPath					pickedPath;
	TQ3Object					pickedObject;
	TQ3Ray3D					rayThroughPick;
	TQ3Uns32					pickDecomposeCount;


	// Write state
	TQ3FileObject				theFile;
	

	// Rendering resources
	TQ3RendererObject			theRenderer;
	TQ3CameraObject				theCamera;
	TQ3GroupObject				theLights;
	TQ3DrawContextObject		theDrawContext;
	TQ3AttributeSet				defaultAttributeSet;	
	TQ3Boolean					rendererFinishedFrame;
	

	// View callbacks
	TQ3ViewIdleMethod			idleMethod;
	TQ3ViewIdleProgressMethod	idleProgressMethod;
	TQ3ViewEndFrameMethod		endFrameMethod;
	const void					*idleData;
	const void					*idleProgressData;
	const void					*endFrameData;
} TQ3ViewData;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3view_stack_initialise : Initialise a view state stack item.
//-----------------------------------------------------------------------------
static void
e3view_stack_initialise(TQ3ViewStackItem *theItem)
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theItem));



	// Initialise the item
	Q3Matrix4x4_SetIdentity(&theItem->matrixLocalToWorld);

	theItem->next					 = NULL;
	theItem->stackState				 = kQ3ViewStateAll;
	theItem->attributeSet            = Q3AttributeSet_New();
	theItem->shaderIllumination		 = Q3NULLIllumination_New();
	theItem->shaderSurface			 = NULL;
	theItem->styleBackfacing         = kQ3BackfacingStyleBoth;
	theItem->styleInterpolation      = kQ3InterpolationStyleVertex;
	theItem->styleFill               = kQ3FillStyleFilled;
	theItem->styleHighlight          = NULL;
	theItem->styleSubdivision.method = kQ3SubdivisionMethodScreenSpace;
	theItem->styleSubdivision.c1     = kQ3ViewDefaultSubdivisionC1;
	theItem->styleSubdivision.c2     = kQ3ViewDefaultSubdivisionC2;
	theItem->styleOrientation        = kQ3OrientationStyleCounterClockwise;
	theItem->styleReceiveShadows     = kQ3True;
	theItem->stylePickID             = 0;
	theItem->stylePickParts          = kQ3PickPartsObject;
	theItem->styleAntiAlias.state    = kQ3Off;
	theItem->styleAntiAlias.mode     = kQ3AntiAliasModeMaskEdges;
	theItem->styleAntiAlias.quality  = 1.0f;
	theItem->styleFog.state          = kQ3Off;
	theItem->styleFog.mode           = kQ3FogModeLinear;
	theItem->styleFog.fogStart       = 0.0f;
	theItem->styleFog.fogEnd         = 1.0f;
	theItem->styleFog.density        = 0.5f;
	Q3ColorARGB_Set(&theItem->styleFog.color, 1.0f, 1.0f, 1.0f, 1.0f);

	theItem->attributeAmbientCoefficient = kQ3ViewDefaultAmbientCoefficient;
	theItem->attributeSpecularControl    = kQ3ViewDefaultSpecularControl;
	theItem->attributeHighlightState     = kQ3ViewDefaultHighlightState;
	theItem->attributeSurfaceShader      = NULL;
	Q3Param2D_Set(&theItem->attributeSurfaceUV, 0.0f, 0.0f);
	Q3Param2D_Set(&theItem->attributeShadingUV, 0.0f, 0.0f);
	Q3Vector3D_Set(&theItem->attributeNormal,   0.0f, 1.0f, 0.0f);
	Q3ColorRGB_Set(&theItem->attributeDiffuseColor,      kQ3ViewDefaultDiffuseColor);
	Q3ColorRGB_Set(&theItem->attributeSpecularColor,     kQ3ViewDefaultSpecularColor);
	Q3ColorRGB_Set(&theItem->attributeTransparencyColor, kQ3ViewDefaultTransparency);
	Q3Vector3D_Set(&theItem->attributeSurfaceTangent.uTangent, 1.0f, 1.0f, 1.0f);
	Q3Vector3D_Set(&theItem->attributeSurfaceTangent.vTangent, 1.0f, 1.0f, 1.0f);
}





//=============================================================================
//      e3view_stack_update_attribute : Update the renderer's attribute state.
//-----------------------------------------------------------------------------
//		Note :	We add the attribute data to the topmost attribute set on the
//				view stat stack, and inform the renderer of the change.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_stack_update_attribute(TQ3ViewObject				theView,
								TQ3ViewStackItem		*topItem,
								TQ3AttributeType		attributeType,
								const void				*paramData)
{	TQ3Status		qd3dStatus;


	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theView);
	Q3_ASSERT_VALID_PTR(topItem);



	// Update the current attribute set.
	//
	// We need to special case surface shaders, since these are real TQ3Objects
	// rather than flat data (and so we can't just add a NULL value - if we need
	// to get rid of it, we have to clear it by hand).
	if (attributeType == kQ3AttributeTypeSurfaceShader)
		{
		qd3dStatus = kQ3Success;

		if (Q3AttributeSet_Contains(topItem->attributeSet, attributeType))
			qd3dStatus = Q3AttributeSet_Clear(topItem->attributeSet, attributeType);
		
		if (*((const TQ3ShaderObject *) paramData) != NULL)
			qd3dStatus = Q3AttributeSet_Add(topItem->attributeSet, attributeType, paramData);
		}
	else
		qd3dStatus = Q3AttributeSet_Add(topItem->attributeSet, attributeType, paramData);
	
	
	
	// Update the renderer
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3Renderer_Method_UpdateAttribute(theView, attributeType, paramData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3view_stack_update : Update the renderer state.
//-----------------------------------------------------------------------------
//		Note :	We take the topmost item from the view's state stack, and the
//				mask of which fields have changed, and inform the renderer of
//				the change.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_stack_update(TQ3ViewObject theView, TQ3ViewStackState stateChange)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status			qd3dStatus    = kQ3Success;
	TQ3ViewStackItem	*theItem;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theView);



	// If the stack is empty, we're done
	if (instanceData->stackState == NULL)
		return(kQ3Success);



	// Find the item at the top of the stack
	Q3_ASSERT_VALID_PTR(instanceData->stackState);
	theItem = instanceData->stackState;



	// Update the renderer if we're currently drawing
	if (instanceData->viewMode == kQ3ViewModeDrawing)
		{
		if ((stateChange & kQ3ViewStateMatrixLocalToWorld) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateMatrixLocalToWorld(theView, &theItem->matrixLocalToWorld);

		if ((stateChange & kQ3ViewStateShaderIllumination) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateShader(theView, kQ3ShaderTypeIllumination, &theItem->shaderIllumination);
		
		if ((stateChange & kQ3ViewStateShaderSurface) && qd3dStatus == kQ3Success)
			{
			// QD3D only submits textures when in kQ3FillStyleFilled mode, so we do the same
			if (theItem->styleFill == kQ3FillStyleFilled)
				qd3dStatus = E3Renderer_Method_UpdateShader(theView, kQ3ShaderTypeSurface, &theItem->shaderSurface);
			}
	
		if ((stateChange & kQ3ViewStateStyleBackfacing) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateStyle(theView, kQ3StyleTypeBackfacing, &theItem->styleBackfacing);

		if ((stateChange & kQ3ViewStateStyleInterpolation) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateStyle(theView, kQ3StyleTypeInterpolation, &theItem->styleInterpolation);

		if ((stateChange & kQ3ViewStateStyleFill) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateStyle(theView, kQ3StyleTypeFill, &theItem->styleFill);

		if ((stateChange & kQ3ViewStateStyleHighlight) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateStyle(theView, kQ3StyleTypeHighlight, &theItem->styleHighlight);

		if ((stateChange & kQ3ViewStateStyleSubdivision) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateStyle(theView, kQ3StyleTypeSubdivision, &theItem->styleSubdivision);

		if ((stateChange & kQ3ViewStateStyleOrientation) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateStyle(theView, kQ3StyleTypeOrientation, &theItem->styleOrientation);

		if ((stateChange & kQ3ViewStateStyleReceiveShadows) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateStyle(theView, kQ3StyleTypeReceiveShadows, &theItem->styleReceiveShadows);

		if ((stateChange & kQ3ViewStateStylePickID) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateStyle(theView, kQ3StyleTypePickID, &theItem->stylePickID);

		if ((stateChange & kQ3ViewStateStylePickParts) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateStyle(theView, kQ3StyleTypePickParts, &theItem->stylePickParts);

		if ((stateChange & kQ3ViewStateStyleAntiAlias) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateStyle(theView, kQ3StyleTypeAntiAlias, &theItem->styleAntiAlias);

		if ((stateChange & kQ3ViewStateStyleFog) && qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateStyle(theView, kQ3StyleTypeFog, &theItem->styleFog);

		if ((stateChange & kQ3ViewStateAttributeSurfaceUV) && qd3dStatus == kQ3Success)
			qd3dStatus = e3view_stack_update_attribute(theView, theItem, kQ3AttributeTypeSurfaceUV, &theItem->attributeSurfaceUV);

		if ((stateChange & kQ3ViewStateAttributeShadingUV) && qd3dStatus == kQ3Success)
			qd3dStatus = e3view_stack_update_attribute(theView, theItem, kQ3AttributeTypeShadingUV, &theItem->attributeShadingUV);

		if ((stateChange & kQ3ViewStateAttributeNormal) && qd3dStatus == kQ3Success)
			qd3dStatus = e3view_stack_update_attribute(theView, theItem, kQ3AttributeTypeNormal, &theItem->attributeNormal);

		if ((stateChange & kQ3ViewStateAttributeAmbientCoefficient) && qd3dStatus == kQ3Success)
			qd3dStatus = e3view_stack_update_attribute(theView, theItem, kQ3AttributeTypeAmbientCoefficient, &theItem->attributeAmbientCoefficient);

		if ((stateChange & kQ3ViewStateAttributeDiffuseColour) && qd3dStatus == kQ3Success)
			qd3dStatus = e3view_stack_update_attribute(theView, theItem, kQ3AttributeTypeDiffuseColor, &theItem->attributeDiffuseColor);

		if ((stateChange & kQ3ViewStateAttributeSpecularColour) && qd3dStatus == kQ3Success)
			qd3dStatus = e3view_stack_update_attribute(theView, theItem, kQ3AttributeTypeSpecularColor, &theItem->attributeSpecularColor);

		if ((stateChange & kQ3ViewStateAttributeSpecularControl) && qd3dStatus == kQ3Success)
			qd3dStatus = e3view_stack_update_attribute(theView, theItem, kQ3AttributeTypeSpecularControl, &theItem->attributeSpecularControl);

		if ((stateChange & kQ3ViewStateAttributeTransparencyColour) && qd3dStatus == kQ3Success)
			qd3dStatus = e3view_stack_update_attribute(theView, theItem, kQ3AttributeTypeTransparencyColor, &theItem->attributeTransparencyColor);

		if ((stateChange & kQ3ViewStateAttributeSurfaceTangent) && qd3dStatus == kQ3Success)
			qd3dStatus = e3view_stack_update_attribute(theView, theItem, kQ3AttributeTypeSurfaceTangent, &theItem->attributeSurfaceTangent);

		if ((stateChange & kQ3ViewStateAttributeHighlightState) && qd3dStatus == kQ3Success)
			qd3dStatus = e3view_stack_update_attribute(theView, theItem, kQ3AttributeTypeHighlightState, &theItem->attributeHighlightState);

		if ((stateChange & kQ3ViewStateAttributeSurfaceShader) && qd3dStatus == kQ3Success)
			qd3dStatus = e3view_stack_update_attribute(theView, theItem, kQ3AttributeTypeSurfaceShader, &theItem->attributeSurfaceShader);
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3view_stack_push : Push the view state stack.
//-----------------------------------------------------------------------------
//		Note :	The first item is initialise to default values, and further
//				items are cloned from the previously topmost item on the stack.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_stack_push(TQ3ViewObject theView)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3ViewStackItem	*newTop, *oldTop;


	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theView);



	// Grow the view stack to the hold the new item
	newTop = Q3Memory_Allocate( sizeof(TQ3ViewStackItem) );
	if (newTop == NULL)
		{
		return kQ3Failure;
		}
	oldTop = instanceData->stackState;
	instanceData->stackState = newTop;
	

	// If this is the first item, initialise it
	if (oldTop == NULL)
		{
		e3view_stack_initialise(newTop);
		newTop->next = oldTop;
		}
	
	// Otherwise, clone what was on the top to the new top
	else
		{
		// Take a copy of the state
		Q3Memory_Copy(oldTop, newTop, sizeof(TQ3ViewStackItem));
		newTop->next = oldTop;



		// Adjust the reference counts of the shared objects. The Q3Memory_Copy will have
		// have copied them without adjusting the reference counts, which is incorrect.
		//
		// Note that for the attributeSet, we need to duplicate the object rather than
		// simply increment the reference count. The other shared objects can be replaced
		// with E3Shared_Replace when new values are submitted, but the attributeSet is
		// special.
		//
		// This item is updated as new attributes are submitted, which means that the
		// set on each level of the stack must be a distinct object. This is because an
		// attribute set can only hold one value of each type, so if we just incremented
		// the reference count we would overwrite the previously top set's value whenever
		// we changed the value for the new top set's value.
		if (oldTop->attributeSet != NULL)
			newTop->attributeSet = Q3Object_Duplicate(oldTop->attributeSet);
		
		E3Shared_Acquire(&newTop->shaderIllumination,     oldTop->shaderIllumination);
		E3Shared_Acquire(&newTop->shaderSurface,          oldTop->shaderSurface);
		E3Shared_Acquire(&newTop->styleHighlight,         oldTop->styleHighlight);
		E3Shared_Acquire(&newTop->attributeSurfaceShader, oldTop->attributeSurfaceShader);
		}



	return(kQ3Success);
}





//=============================================================================
//      e3view_stack_pop : Pop the view state stack.
//-----------------------------------------------------------------------------
static void
e3view_stack_pop(TQ3ViewObject theView)
{	TQ3ViewData				*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3ViewStackState		theState;
	TQ3Status				qd3dStatus;
	TQ3ViewStackItem*		theItem;



	// Validate our parameters and state
	Q3_ASSERT_VALID_PTR(theView);
	Q3_REQUIRE(Q3_VALID_PTR(instanceData->stackState));



	// Save the state mask for the topmost item
	theState = instanceData->stackState->stackState;



	// Dispose of the shared objects in the topmost item
	Q3Object_CleanDispose(&instanceData->stackState->attributeSet);
	Q3Object_CleanDispose(&instanceData->stackState->shaderIllumination);
	Q3Object_CleanDispose(&instanceData->stackState->shaderSurface);
	Q3Object_CleanDispose(&instanceData->stackState->styleHighlight);
	Q3Object_CleanDispose(&instanceData->stackState->attributeSurfaceShader);



	// Shrink the stack to get rid of the top item
	theItem = instanceData->stackState;
	instanceData->stackState = instanceData->stackState->next;
	Q3Memory_Free( &theItem );



	// Update the renderer, using the state of the previously top
	// item as the mask indicating what's changed.
	qd3dStatus = e3view_stack_update(theView, theState);
}





//=============================================================================
//      e3view_stack_pop_clean : Pop the view stack clean.
//-----------------------------------------------------------------------------
static void
e3view_stack_pop_clean(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;


	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theView);



	// Pop the stack clean
	while (instanceData->stackState != NULL)
		e3view_stack_pop(theView);
}





//=============================================================================
//      e3view_bounds_box_exact : Update our bounds.
//-----------------------------------------------------------------------------
//		Note :	We transform the vertices to world coordinates, then union them
//				with the view bounding box.
//-----------------------------------------------------------------------------
static void
e3view_bounds_box_exact(TQ3ViewObject theView, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	const TQ3Matrix4x4	*localToWorld;
	TQ3Point3D			worldPoint;
	const TQ3Uns8		*rawPoint;
	TQ3Uns32			i;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theView);
	Q3_ASSERT(numPoints != 0);



	// Get the local to world matrix
	localToWorld = &instanceData->stackState->matrixLocalToWorld;
	Q3_ASSERT_VALID_PTR(localToWorld);



	// Transform the points, and accumulate them into the bounding box
	rawPoint = (const TQ3Uns8 *) thePoints;
	for (i = 0; i < numPoints; ++i, rawPoint += pointStride)
		{
		// Transform the point
		Q3Point3D_Transform((const TQ3Point3D *) rawPoint, localToWorld, &worldPoint);
		
		// Union it into the bounding box
		Q3BoundingBox_UnionPoint3D(&instanceData->boundingBox, &worldPoint, &instanceData->boundingBox);
		}
}





//=============================================================================
//      e3view_bounds_box_approx : Update our bounds.
//-----------------------------------------------------------------------------
//		Note :	We calculate a bounding box in local coordinates, transform the
//				bounding box to world coordinates, then union this with the
//				view bounding box.
//-----------------------------------------------------------------------------
static void
e3view_bounds_box_approx(TQ3ViewObject theView, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3BoundingBox		transformedBounds;
	const TQ3Matrix4x4	*localToWorld;
	TQ3BoundingBox		localBounds;
	TQ3Point3D			tempPoint;



	// Validate our parameters and state
	Q3_ASSERT(numPoints                    != 0);
	Q3_ASSERT(instanceData->boundingMethod == kQ3BoxBoundsApprox);



	// Get the local to world matrix
	localToWorld = &instanceData->stackState->matrixLocalToWorld;
	Q3_ASSERT_VALID_PTR(localToWorld);



	// Calculate a bounding box in local coordinates
	Q3BoundingBox_SetFromPoints3D(&localBounds, thePoints, numPoints, pointStride);



	// Transform the bounding box to world coordinates
	Q3Point3D_Transform(&localBounds.min, localToWorld, &tempPoint);
	Q3BoundingBox_Set(&transformedBounds,&tempPoint,&tempPoint,kQ3False);
		
	Q3Point3D_Set (&tempPoint, localBounds.min.x,localBounds.min.y,localBounds.max.z);
	Q3Point3D_Transform(&tempPoint, localToWorld, &tempPoint);
	Q3BoundingBox_UnionPoint3D(&transformedBounds,&tempPoint,&transformedBounds);
	
	Q3Point3D_Set (&tempPoint, localBounds.max.x,localBounds.min.y,localBounds.max.z);
	Q3Point3D_Transform(&tempPoint, localToWorld, &tempPoint);
	Q3BoundingBox_UnionPoint3D(&transformedBounds,&tempPoint,&transformedBounds);
	
	Q3Point3D_Set (&tempPoint, localBounds.max.x,localBounds.min.y,localBounds.min.z);
	Q3Point3D_Transform(&tempPoint, localToWorld, &tempPoint);
	Q3BoundingBox_UnionPoint3D(&transformedBounds,&tempPoint,&transformedBounds);
	
	Q3Point3D_Transform(&localBounds.max, localToWorld, &tempPoint);
	Q3BoundingBox_UnionPoint3D(&transformedBounds,&tempPoint,&transformedBounds);
	
	Q3Point3D_Set (&tempPoint, localBounds.max.x,localBounds.max.y,localBounds.min.z);
	Q3Point3D_Transform(&tempPoint, localToWorld, &tempPoint);
	Q3BoundingBox_UnionPoint3D(&transformedBounds,&tempPoint,&transformedBounds);
	
	Q3Point3D_Set (&tempPoint, localBounds.min.x,localBounds.max.y,localBounds.min.z);
	Q3Point3D_Transform(&tempPoint, localToWorld, &tempPoint);
	Q3BoundingBox_UnionPoint3D(&transformedBounds,&tempPoint,&transformedBounds);
	
	Q3Point3D_Set (&tempPoint, localBounds.min.x,localBounds.max.y,localBounds.max.z);
	Q3Point3D_Transform(&tempPoint, localToWorld, &tempPoint);
	Q3BoundingBox_UnionPoint3D(&transformedBounds,&tempPoint,&transformedBounds);
	
	
	

	// Accumulate the bounding box
	Q3BoundingBox_Union(&transformedBounds, &instanceData->boundingBox, &instanceData->boundingBox);
}





//=============================================================================
//      e3view_bounds_sphere_exact : Update our bounds.
//-----------------------------------------------------------------------------
//		Note :	We transform the vertices to world coordinates, then union them
//				with the view bounding sphere.
//-----------------------------------------------------------------------------
static void
e3view_bounds_sphere_exact(TQ3ViewObject theView, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints)
{	TQ3ViewData				*instanceData = (TQ3ViewData *) theView->instanceData;
	const TQ3Matrix4x4		*localToWorld;
	TQ3Point3D				*worldPoints;
	TQ3BoundingSphere		localBounds;



	// Validate our parameters and state
	Q3_ASSERT(numPoints                    != 0);
	Q3_ASSERT(instanceData->boundingMethod == kQ3SphereBoundsExact);



	// Get the local to world matrix
	localToWorld = &instanceData->stackState->matrixLocalToWorld;
	Q3_ASSERT_VALID_PTR(localToWorld);



	// Transform the points to world coordinates
	worldPoints = (TQ3Point3D *) Q3Memory_Allocate(numPoints * sizeof(TQ3Point3D));
	if (worldPoints == NULL)
		return;

	Q3Point3D_To3DTransformArray(thePoints, localToWorld, worldPoints,
								  numPoints, pointStride, sizeof(TQ3Point3D));



	// Calculate their bounding sphere and accumulate it
	Q3BoundingSphere_SetFromPoints3D(&localBounds, worldPoints, numPoints, sizeof(TQ3Point3D));
	Q3BoundingSphere_Union(&localBounds, &instanceData->boundingSphere, &instanceData->boundingSphere);



	// Clean up
	Q3Memory_Free(&worldPoints);
}





//=============================================================================
//      e3view_bounds_sphere_approx : Update our bounds.
//-----------------------------------------------------------------------------
//		Note :	We calculate a bounding sphere in local coordinates, transform
//				the bounding sphere to world coordinates, then union this with
//				the view bounding sphere.
//-----------------------------------------------------------------------------
static void
e3view_bounds_sphere_approx(TQ3ViewObject theView, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints)
{	TQ3ViewData				*instanceData = (TQ3ViewData *) theView->instanceData;
	const TQ3Matrix4x4		*localToWorld;
	TQ3SphericalPoint		radSpherical;
	TQ3BoundingSphere		localBounds;
	TQ3Point3D				radPoint;



	// Validate our parameters and state
	Q3_ASSERT(numPoints                    != 0);
	Q3_ASSERT(instanceData->boundingMethod == kQ3SphereBoundsApprox);



	// Get the local to world matrix
	localToWorld = &instanceData->stackState->matrixLocalToWorld;
	Q3_ASSERT_VALID_PTR(localToWorld);



	// Calculate a bounding sphere in local coordinates
	Q3BoundingSphere_SetFromPoints3D(&localBounds, thePoints, numPoints, pointStride);



	// Transform the bounding sphere origin to world coordinates
	Q3Point3D_Transform(&localBounds.origin, localToWorld, &localBounds.origin);



	// Transform the bounding sphere radius to world coordinate
	Q3SphericalPoint_Set(&radSpherical, localBounds.radius, 0.0f, 0.0f);

	Q3SphericalPoint_ToPoint3D(&radSpherical, &radPoint);
	Q3Point3D_Transform(&radPoint, localToWorld, &radPoint);
	Q3Point3D_ToSpherical(&radPoint, &radSpherical);

	localBounds.radius = radSpherical.rho;



	// Accumulate the bounding sphere
	Q3BoundingSphere_Union(&localBounds, &instanceData->boundingSphere, &instanceData->boundingSphere);
}





//=============================================================================
//      e3view_submit_begin : Begin a submitting loop.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_begin(TQ3ViewObject theView, TQ3ViewMode viewMode)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus    = kQ3Success;



	// Set up the view state
	if (instanceData->viewState == kQ3ViewStateInactive)
		{
		// Initialise the state
		instanceData->viewMode  = viewMode;
		instanceData->viewState = kQ3ViewStateSubmitting;
		instanceData->viewPass  = 1;
		}

	else if (instanceData->viewState == kQ3ViewStateSubmitting)
		{
		// Make sure our state is valid
		Q3_ASSERT(instanceData->viewMode != kQ3ViewModeInactive);
		Q3_ASSERT(instanceData->viewPass > 1);
		}

	else
		qd3dStatus = kQ3Failure;



	// Push the first entry on the view stack
	if (qd3dStatus == kQ3Success)
		qd3dStatus = e3view_stack_push(theView);



	// Handle failure
	if (qd3dStatus != kQ3Success)
		{
		instanceData->viewMode  = kQ3ViewModeInactive;
		instanceData->viewState = kQ3ViewStateInactive;
		instanceData->viewPass  = 0;
		e3view_stack_pop_clean(theView);
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3view_submit_initial_state : Submit the initial state to a view.
//-----------------------------------------------------------------------------
//		Note :	We submit the initial contents of the state stack, and the view
//				attribute set, to the view.
//
//				Must be done separately from e3view_submit_begin, since the
//				Interactive Renderer expects its BeginPass method to be called
//				before the first state is submitted.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_initial_state(TQ3ViewObject theView)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status			qd3dStatus;



	// Submit the initial state stack
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAll);



	// Submit the view attribute state, if any
	if (instanceData->viewAttributes != NULL && qd3dStatus == kQ3Success)
		qd3dStatus = Q3AttributeSet_Submit(instanceData->viewAttributes, theView);

	return(qd3dStatus);
}





//=============================================================================
//      e3view_submit_end : End a submit loop.
//-----------------------------------------------------------------------------
//		Note :	If the current view state is within a submitting a loop, the
//				submitStatus value is used as the result of the submit loop.
//-----------------------------------------------------------------------------
static TQ3ViewStatus
e3view_submit_end(TQ3ViewObject theView, TQ3ViewStatus submitStatus)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3ViewStatus		viewStatus;
	TQ3Status			qd3dStatus;



	// End the pass
	if (instanceData->viewState == kQ3ViewStateCancelled)
		viewStatus = kQ3ViewStatusCancelled;

	else if (instanceData->viewState == kQ3ViewStateSubmitting)
		viewStatus = submitStatus;

	else
		viewStatus = kQ3ViewStatusDone;



	// Pop the view stack
	e3view_stack_pop_clean(theView);



	// Handle re-traversal
	if (viewStatus == kQ3ViewStatusRetraverse)
		{
		// Restart the submit
		instanceData->viewState = kQ3ViewStateSubmitting;
		instanceData->viewPass++;
		qd3dStatus = e3view_submit_begin(theView, instanceData->viewMode);


		// Invoke the start pass method		
		if (qd3dStatus == kQ3Success)
			{
			instanceData->rendererFinishedFrame = kQ3False;

			if (instanceData->viewMode == kQ3ViewModeDrawing)
				qd3dStatus = E3Renderer_Method_StartPass(theView, instanceData->theCamera, instanceData->theLights);

			else if (instanceData->viewMode == kQ3ViewModeWriting)
				qd3dStatus = E3FileFormat_Method_StartPass(theView);
			}


		// Submit the initial state
		if (qd3dStatus == kQ3Success)
			qd3dStatus = e3view_submit_initial_state(theView);


		// If something went wrong, stop
		if (qd3dStatus != kQ3Success)
			viewStatus = kQ3ViewStatusError;
		}
	else
		{
		instanceData->viewState = kQ3ViewStateInactive;
		instanceData->viewPass  = 0;
		}



	// Return the view status
	return(viewStatus);
}





//=============================================================================
//      e3view_submit_object : Submit an object to a view.
//-----------------------------------------------------------------------------
//		Note :	We dispatch to the appropriate submit routine for the object,
//				given the current submitting loop.
//
//				The leaf type must be explicitly passed in through the
//				objectType parameter, to allow 'objects' to be submitted in
//				immediate mode (in which case theObject will be NULL).
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_object(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3ViewData					*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3XObjectSubmitMethod		submitMethod;
	TQ3Status					qd3dStatus;
	E3ClassInfoPtr				theClass;



	// Find the object class
	theClass = E3ClassTree_GetClassByType(objectType);
	if (theClass == NULL)
		{
		E3ErrorManager_PostError(kQ3ErrorInvalidObjectClass, kQ3False);
		return(kQ3Failure);
		}



	// Find the appropriate submit method
	submitMethod = NULL;
	switch (instanceData->viewMode) {
		case kQ3ViewModeDrawing:
			submitMethod = (TQ3XObjectSubmitMethod)
							E3ClassTree_GetMethod(theClass, kQ3XMethodTypeObjectSubmitRender);
			break;

		case kQ3ViewModePicking:
			submitMethod = (TQ3XObjectSubmitMethod)
							E3ClassTree_GetMethod(theClass, kQ3XMethodTypeObjectSubmitPick);
			break;

		case kQ3ViewModeWriting:
			submitMethod = (TQ3XObjectSubmitMethod)
							E3ClassTree_GetMethod(theClass, kQ3XMethodTypeObjectSubmitWrite);
			break;

		case kQ3ViewModeCalcBounds:
			submitMethod = (TQ3XObjectSubmitMethod)
							E3ClassTree_GetMethod(theClass, kQ3XMethodTypeObjectSubmitBounds);
			break;

		default:
			Q3_ASSERT(!"Unrecognised view mode");
			E3ErrorManager_PostError(kQ3ErrorUnsupportedFunctionality, kQ3False);
			return(kQ3Failure);
			break;
		}



	// If we're picking, update the current hit target. We only do this if we are not
	// within a decomposed object, as we want to track the object submitted by the
	// application and not any sub-objects which are submitted to calculate the pick
	// for that top-level object.
	if (instanceData->viewMode == kQ3ViewModePicking && instanceData->pickDecomposeCount == 0)
		E3View_PickStack_SaveObject(theView, theObject);



	// Call the method
	if (submitMethod != NULL)
		qd3dStatus = submitMethod(theView, objectType, theObject, objectData);
	else
		qd3dStatus = kQ3Success;



	// If we're picking, reset the current hit target. Not strictly necessary (since we
	// will release our reference on the next object or at the end of the picking loop)
	// but this helps keep our internal state easily debuggable (the tracked object
	// should always be NULL except when a pick submit method is invoked).
	if (instanceData->viewMode == kQ3ViewModePicking && instanceData->pickDecomposeCount == 0)
		E3View_PickStack_SaveObject(theView, NULL);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3view_pick_begin : Prepare to pick.
//-----------------------------------------------------------------------------
//		Note :	Saves a reference to the current pick object, so it can't be
//				released during the pick.
//
//				If the current pick is a window point pick, recalculates the
//				ray from the camera location through the picked pixel.
//-----------------------------------------------------------------------------
static void
e3view_pick_begin(TQ3ViewObject theView, TQ3PickObject thePick)
{	TQ3ViewData				*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Matrix4x4			worldToFrustum, frustumToWindow, worldToWindow, windowToWorld;
	TQ3Point3D				worldPoint, windowPoint3D;
	TQ3CameraPlacement		thePlacement;
	TQ3Point2D				windowPoint;
	TQ3Vector3D				theVector;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode == kQ3ViewModePicking);



	// Save a reference to the pick object
	Q3_ASSERT(instanceData->thePick == NULL);
	instanceData->thePick = thePick;



	// Initialise our state
	Q3Memory_Clear(&instanceData->pickedPath, sizeof(instanceData->pickedPath)); 
	instanceData->pickedObject       = NULL;
	instanceData->pickDecomposeCount = 0;



	// If this is a window-point pick, recalculate the pick ray
	Q3Memory_Clear(&instanceData->rayThroughPick, sizeof(instanceData->rayThroughPick));
	if (Q3Pick_GetType(thePick) == kQ3PickTypeWindowPoint)
		{
		// Get the point that was picked, in window coordinates
		Q3WindowPointPick_GetPoint(thePick, &windowPoint);

		windowPoint3D.x = windowPoint.x;
		windowPoint3D.y = windowPoint.y;
		windowPoint3D.z = 0.0f;
		


		// Transform this point from window to world coordinates
		Q3Camera_GetWorldToFrustum(instanceData->theCamera, &worldToFrustum);
		E3View_GetFrustumToWindowMatrixState( theView, &frustumToWindow );
		Q3Matrix4x4_Multiply( &worldToFrustum, &frustumToWindow, &worldToWindow );
		Q3Matrix4x4_Invert(&worldToWindow, &windowToWorld);
		Q3Point3D_Transform(&windowPoint3D,  &windowToWorld, &worldPoint);



		switch( Q3Camera_GetType( instanceData->theCamera )	)	
			{
		
			case kQ3CameraTypeViewPlane:
			case kQ3CameraTypeViewAngleAspect:

				// Initialise the ray origin - the camera location
				Q3Camera_GetPlacement(instanceData->theCamera, &thePlacement);
				instanceData->rayThroughPick.origin = thePlacement.cameraLocation;



				// Initialise the ray - from the pixel back to the camera
				Q3Point3D_Subtract(&worldPoint,  &instanceData->rayThroughPick.origin, &theVector);
				Q3Vector3D_Normalize(&theVector, &instanceData->rayThroughPick.direction);
				break;
				
			case kQ3CameraTypeOrthographic:
			
				// Set origin of pick to the pick loc in world coordinates
				Q3Camera_GetPlacement(instanceData->theCamera, &thePlacement);
				instanceData->rayThroughPick.origin = worldPoint;



				// Initialise the ray - from the pixel back to the camera
				Q3Point3D_Subtract(&thePlacement.pointOfInterest, &thePlacement.cameraLocation, &theVector);
				Q3Vector3D_Normalize(&theVector, &instanceData->rayThroughPick.direction);
				break;
			}

		}

}





//=============================================================================
//      e3view_pick_end : End a picking loop.
//-----------------------------------------------------------------------------
static void
e3view_pick_end(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode           == kQ3ViewModePicking);
	Q3_ASSERT(instanceData->pickDecomposeCount == 0);



	// Forget our reference to the pick object (don't dispose of it, as picks aren't shared)
	instanceData->thePick = NULL;



	// Empty the pick path
	Q3HitPath_EmptyData(&instanceData->pickedPath);



	// Dispose of our reference to the picked object
	Q3Object_CleanDispose(&instanceData->pickedObject);
}





//=============================================================================
//		e3view_default_lights : Create the default lights for a view.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_default_lights(TQ3ViewObject theView)
{	TQ3Vector3D					sunDirection = { -1.0f, 0.0f, -1.0f };
	TQ3ColorRGB					lightColour  = {  1.0f, 1.0f,  1.0f };
	TQ3DirectionalLightData		directionalLight;
	TQ3LightData				ambientLight;
	TQ3Status					qd3dStatus;



	// Add the ambient light
	ambientLight.isOn       = kQ3True;
	ambientLight.color      = lightColour;
	ambientLight.brightness = 0.1f;

	qd3dStatus = Q3View_AddLight(theView, kQ3LightTypeAmbient, &ambientLight);



	// Add the directional light
	directionalLight.lightData.isOn       = kQ3True;
	directionalLight.lightData.color      = lightColour;
	directionalLight.lightData.brightness = 1.0f;
	directionalLight.castsShadows         = kQ3True;
	directionalLight.direction            = sunDirection;

	qd3dStatus = Q3View_AddLight(theView, kQ3LightTypeDirectional, &directionalLight);

	return(qd3dStatus);
}





//=============================================================================
//		e3view_default_camera : Create the default camera for a view.
//-----------------------------------------------------------------------------
static TQ3CameraObject
e3view_default_camera(TQ3DrawContextObject theDrawContext)
{	float							theWidth, theHeight;
	TQ3Status						qd3dStatus;
	TQ3ViewAngleAspectCameraData	cameraData;
	TQ3CameraObject					theCamera;
	TQ3Area							theArea;



	// Get the size of the image we're rendering
	qd3dStatus = Q3DrawContext_GetPane(theDrawContext, &theArea);
	theWidth  = theArea.max.x - theArea.min.x;
	theHeight = theArea.max.y - theArea.min.y;



	// Fill in the camera data
	Q3Point3D_Set(&cameraData.cameraData.placement.cameraLocation,  0.0f, 0.0f, 5.0f);
	Q3Point3D_Set(&cameraData.cameraData.placement.pointOfInterest, 0.0f, 0.0f, 0.0f);
	Q3Vector3D_Set(&cameraData.cameraData.placement.upVector,       0.0f, 1.0f, 0.0f);

	cameraData.cameraData.range.hither		= 0.1f;
	cameraData.cameraData.range.yon			= 60.0f;
	cameraData.cameraData.viewPort.origin.x	= -1.0f;
	cameraData.cameraData.viewPort.origin.y	=  1.0f;
	cameraData.cameraData.viewPort.width	=  2.0f;
	cameraData.cameraData.viewPort.height	=  2.0f;
	cameraData.fov							= Q3Math_DegreesToRadians(50.0f);
	cameraData.aspectRatioXToY              = (theWidth / theHeight);



	// Create the camera
	theCamera = Q3ViewAngleAspectCamera_New(&cameraData);
	return(theCamera);
}





//=============================================================================
//      e3view_new : View class new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3ViewData			*instanceData  = (TQ3ViewData *) privateData;
	TQ3ColorRGB			defaultTransparencyColour = { kQ3ViewDefaultTransparency  };
	TQ3ColorRGB			defaultSpecularColour     = { kQ3ViewDefaultSpecularColor };
	TQ3ColorRGB			defaultDiffuseColour      = { kQ3ViewDefaultDiffuseColor  };
	float				defaultAmbientCoefficient =   kQ3ViewDefaultAmbientCoefficient;
	float				defaultSpecularControl    =   kQ3ViewDefaultSpecularControl;
	TQ3Switch			defaultHighlightState     =   kQ3ViewDefaultHighlightState;
#pragma unused(theObject)
#pragma unused(paramData)



	// Initialise our instance data
	instanceData->viewAttributes = Q3AttributeSet_New();
	if (instanceData->viewAttributes != NULL)
		{
		// Add the attributes
		Q3AttributeSet_Add(instanceData->viewAttributes,
							kQ3AttributeTypeAmbientCoefficient,
							&defaultAmbientCoefficient);
		
		Q3AttributeSet_Add(instanceData->viewAttributes,
							kQ3AttributeTypeHighlightState,
							&defaultHighlightState);
		
		Q3AttributeSet_Add(instanceData->viewAttributes,
							kQ3AttributeTypeSpecularControl,
							&defaultSpecularControl);

		Q3AttributeSet_Add(instanceData->viewAttributes,
							kQ3AttributeTypeDiffuseColor,
							&defaultDiffuseColour);
		
		Q3AttributeSet_Add(instanceData->viewAttributes,
							kQ3AttributeTypeSpecularColor,
							&defaultSpecularColour);
		
		Q3AttributeSet_Add(instanceData->viewAttributes,
							kQ3AttributeTypeTransparencyColor,
							&defaultTransparencyColour);
		}

	return(kQ3Success);
}





//=============================================================================
//      e3view_delete : View class delete method.
//-----------------------------------------------------------------------------
static void
e3view_delete(TQ3Object theObject, void *privateData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	Q3Object_CleanDispose(&instanceData->viewAttributes);
	Q3Object_CleanDispose(&instanceData->theRenderer);
	Q3Object_CleanDispose(&instanceData->theCamera);
	Q3Object_CleanDispose(&instanceData->theLights);
	Q3Object_CleanDispose(&instanceData->theDrawContext);
	Q3Object_CleanDispose(&instanceData->defaultAttributeSet);

	e3view_stack_pop_clean(theObject);
}





//=============================================================================
//      e3view_metahandler : View class metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3view_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3view_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3view_delete;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3push_submit : Push submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3push_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status		qd3dStatus;
#pragma unused(objectType)
#pragma unused(theObject)
#pragma unused(objectData)



	// Push the view state
	qd3dStatus = e3view_stack_push(theView);

	return(qd3dStatus);
}





//=============================================================================
//      e3push_metahandler : Push object metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3push_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3push_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3pop_submit : Pop submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3pop_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
#pragma unused(objectType)
#pragma unused(theObject)
#pragma unused(objectData)



	// If the stack is empty, we can't pop
	if (instanceData->stackState == NULL)
		return(kQ3Failure);



	// Pop the stack
	e3view_stack_pop(theView);

	return(kQ3Success);
}





//=============================================================================
//      e3pop_metahandler : Pop object metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3pop_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3pop_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3View_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3View_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3ObjectTypeView,
											kQ3ClassNameView,
											e3view_metahandler,
											sizeof(TQ3ViewData));
	
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeShape,
												kQ3ShapeTypeStateOperator,
												kQ3ClassNameStateOperator,
												NULL,
												0);
	
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStateOperator,
												kQ3StateOperatorTypePush,
												kQ3ClassNameStateOperatorPush,
												e3push_metahandler,
												0);
	
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeStateOperator,
												kQ3StateOperatorTypePop,
												kQ3ClassNameStateOperatorPop,
												e3pop_metahandler,
												0);

	return(qd3dStatus);
}





//=============================================================================
//      E3View_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3View_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StateOperatorTypePop,   kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StateOperatorTypePush,  kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShapeTypeStateOperator, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeView,         kQ3True);
	return(qd3dStatus);
}





//=============================================================================
//      E3View_SubmitRetained : Submit an object to a view.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3View_SubmitRetained(TQ3ViewObject theView, TQ3Object theObject)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status			qd3dStatus;
	TQ3Error			theError;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		{
		switch (instanceData->viewMode) {
			case kQ3ViewModeDrawing:
				theError = kQ3ErrorRenderingNotStarted;
				break;
			case kQ3ViewModePicking:
				theError = kQ3ErrorPickingNotStarted;
				break;
			case kQ3ViewModeWriting:
				theError = kQ3ErrorWriteStateInactive;
				break;
			case kQ3ViewModeCalcBounds:
				theError = kQ3ErrorBoundsNotStarted;
				break;
			default:
				theError = kQ3ErrorViewNotStarted;
				break;
			}

		E3ErrorManager_PostError(theError, kQ3False);
		return(kQ3Failure);
		}



	// Submit the object
	qd3dStatus = e3view_submit_object(theView,   E3ClassTree_GetType(theObject->theClass),
									  theObject, theObject->instanceData);

	return(qd3dStatus);
}





//=============================================================================
//      E3View_SubmitImmediate : Submit an immediate mode object to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SubmitImmediate(TQ3ViewObject theView, TQ3ObjectType objectType, const void *objectData)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status			qd3dStatus;
	TQ3Error			theError;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		{
		switch (instanceData->viewMode) {
			case kQ3ViewModeDrawing:
				theError = kQ3ErrorRenderingNotStarted;
				break;
			case kQ3ViewModePicking:
				theError = kQ3ErrorPickingNotStarted;
				break;
			case kQ3ViewModeWriting:
				theError = kQ3ErrorWriteStateInactive;
				break;
			case kQ3ViewModeCalcBounds:
				theError = kQ3ErrorBoundsNotStarted;
				break;
			default:
				theError = kQ3ErrorViewNotStarted;
				break;
			}

		E3ErrorManager_PostError(theError, kQ3False);
		return(kQ3Failure);
		}



	// Submit the object
	qd3dStatus = e3view_submit_object(theView, objectType, NULL, objectData);

	return(qd3dStatus);
}





//=============================================================================
//      E3View_CallIdleMethod : Call the idle method for a view.
//-----------------------------------------------------------------------------
//		Note :	Calls the view's idle progress method, falling back to the
//				view's idle method if no progress method has been supplied.
//-----------------------------------------------------------------------------
TQ3Status
E3View_CallIdleMethod(TQ3ViewObject theView, TQ3Uns32 current, TQ3Uns32 completed)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;
	


	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// If we have an idle progress method, call it
	if (instanceData->idleProgressMethod != NULL)
		qd3dStatus = instanceData->idleProgressMethod(theView, 
													  instanceData->idleProgressData,
													  current,
													  completed);


	// Otherwise, fall back to the progress method
	else if (instanceData->idleMethod != NULL)
		qd3dStatus = instanceData->idleMethod(theView, instanceData->idleData);


	// Otherwise, do nothing
	else
		qd3dStatus = kQ3Success;

	return(qd3dStatus);
}





//=============================================================================
//      E3View_AccessPick : Access our pick without ref-counting.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access a view's pick.
//-----------------------------------------------------------------------------
TQ3PickObject
E3View_AccessPick(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Return the pick
	return(instanceData->thePick);
}





//=============================================================================
//      E3View_AccessRenderer : Access our renderer without ref-counting.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access a view's renderer.
//-----------------------------------------------------------------------------
TQ3RendererObject
E3View_AccessRenderer(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Return the renderer
	return(instanceData->theRenderer);
}





//=============================================================================
//      E3View_AccessFile : Access the current file.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access a view's current file.
//-----------------------------------------------------------------------------
TQ3FileObject
E3View_AccessFile(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Return the file
	return (instanceData->theFile);
}





//=============================================================================
//      E3View_AccessFileFormat : Get the format of the current file.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access the current file's format.
//-----------------------------------------------------------------------------
TQ3FileFormatObject
E3View_AccessFileFormat(TQ3ViewObject theView)
{	TQ3ViewData				*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3FileFormatObject		theFormat     = NULL;



	// Return the file Format
	if (instanceData->theFile != NULL)
		theFormat = Q3File_GetFileFormat(instanceData->theFile);

	return(theFormat);
}





//=============================================================================
//      E3View_AccessCamera : Access our camera without ref-counting.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access a view's camera.
//-----------------------------------------------------------------------------
TQ3CameraObject
E3View_AccessCamera(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Return the camera
	return(instanceData->theCamera);
}





//=============================================================================
//      E3View_EndFrame : An async renderer has finished.
//-----------------------------------------------------------------------------
//		Note : Called by E3XView_EndFrame when an async renderer has finished.
//-----------------------------------------------------------------------------
void
E3View_EndFrame(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Set our flag
	instanceData->rendererFinishedFrame = kQ3True;
}





//=============================================================================
//      E3View_GetViewMode : Return the current mode of a view.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access a view's mode.
//-----------------------------------------------------------------------------
TQ3ViewMode
E3View_GetViewMode(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(instanceData->viewState == kQ3ViewStateSubmitting);



	// Return the current mode
	return(instanceData->viewMode);
}





//=============================================================================
//      E3View_GetViewState : Return the current state of a view.
//-----------------------------------------------------------------------------
//		Note :	Used internally by Quesa to access a view's state. May only be
//				called from within a submitting loop.
//-----------------------------------------------------------------------------
TQ3ViewState
E3View_GetViewState(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(instanceData->viewState == kQ3ViewStateSubmitting);



	// Return the current state
	return(instanceData->viewState);
}





//=============================================================================
//      E3View_GetBoundingMethod : Return the current bounding method.
//-----------------------------------------------------------------------------
//		Note :	Used internally by Quesa to access a view's bounding method.
//				May only be called from within a bounding loop.
//-----------------------------------------------------------------------------
TQ3BoundingMethod
E3View_GetBoundingMethod(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode == kQ3ViewModeCalcBounds);



	// Return the current state
	return(instanceData->boundingMethod);
}





//=============================================================================
//      E3View_GetRayThroughPickPoint : Return the pick point ray.
//-----------------------------------------------------------------------------
//		Note :	Used internally by Quesa to access the current pick point ray,
//				which runs from the camera through the picked pixel.
//
//				May only be called from within a bounding loop, and only if
//				the current pick object is a window point pick.
//-----------------------------------------------------------------------------
void
E3View_GetRayThroughPickPoint(TQ3ViewObject theView, TQ3Ray3D *theRay)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode                == kQ3ViewModePicking);
	Q3_ASSERT(Q3Pick_GetType(instanceData->thePick) == kQ3PickTypeWindowPoint);



	// Return the pick ray
	*theRay = instanceData->rayThroughPick;
}





//=============================================================================
//      E3View_UpdateBounds : Incorporate vertices into the bounds.
//-----------------------------------------------------------------------------
//		Note :	Used by geometry classes to accumulate their vertices into the
//				current bounding box/sphere.
//-----------------------------------------------------------------------------
void
E3View_UpdateBounds(TQ3ViewObject theView, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our parameters and state
	Q3_ASSERT(numPoints               != 0);
	Q3_ASSERT(instanceData->viewState == kQ3ViewStateSubmitting);



	// Update the bounds
	switch (instanceData->boundingMethod) {
		case kQ3BoxBoundsExact:
			e3view_bounds_box_exact(theView, numPoints, pointStride, thePoints);
			break;

		case kQ3BoxBoundsApprox:
			if (numPoints > kApproxBoundsThreshold)
				e3view_bounds_box_approx(theView, numPoints, pointStride, thePoints);
			else
				e3view_bounds_box_exact(theView, numPoints, pointStride, thePoints);
			break;

		case kQ3SphereBoundsExact:
			e3view_bounds_sphere_exact(theView, numPoints, pointStride, thePoints);
			break;

		case kQ3SphereBoundsApprox:
			e3view_bounds_sphere_approx(theView, numPoints, pointStride, thePoints);
			break;

		default:
			Q3_ASSERT(!"Unknown bounding method");
			break;
		}
}





//=============================================================================
//      E3View_PickStack_PushGroup : Push a group onto the pick path.
//-----------------------------------------------------------------------------
TQ3Status
E3View_PickStack_PushGroup(TQ3ViewObject theView, TQ3GroupObject theGroup)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3HitPath		*pickedPath   = &instanceData->pickedPath;
	TQ3Status		qd3dStatus;
	TQ3Uns32		theSize;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode == kQ3ViewModePicking);



	// If we don't have a root group yet, this is a top-level group.
	// So save this group as the root group in the pick path.
	if (pickedPath->rootGroup == NULL)
		{
		// Validate our state - the path should be empty
		Q3_ASSERT(pickedPath->rootGroup == NULL);
		Q3_ASSERT(pickedPath->depth     == 0);
		
		
		// Save the group as the current root
		pickedPath->rootGroup = Q3Shared_GetReference(theGroup);
		}



	// Allocate another slot in the positions array for any objects within this group.
	// As each object is submitted the group will call us back to save the current
	// position, so that any objects that are intersected by the current pick will
	// be able to retrieve their submit path.
	//
	// Validate our state - we should have a root
	Q3_ASSERT(pickedPath->rootGroup != NULL);



	// Expand the positions array
	theSize    = sizeof(TQ3GroupPosition) * (pickedPath->depth + 1);
	qd3dStatus = Q3Memory_Reallocate(&pickedPath->positions, theSize);
	if (qd3dStatus == kQ3Success)
		pickedPath->depth++;



	// Save a NULL position to initialise the slot
	pickedPath->positions[pickedPath->depth - 1] = NULL;
	
	return(qd3dStatus);
}





//=============================================================================
//      E3View_PickStack_GetPickedPath : Return the path to the picked object.
//-----------------------------------------------------------------------------
TQ3HitPath *
E3View_PickStack_GetPickedPath(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode == kQ3ViewModePicking);



	// Return the current pick path
	return(&instanceData->pickedPath);
}





//=============================================================================
//      E3View_PickStack_GetPickedObject : Return the picked object.
//-----------------------------------------------------------------------------
TQ3Object
E3View_PickStack_GetPickedObject(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode == kQ3ViewModePicking);



	// Return a new reference to the picked object
	if (instanceData->pickedObject != NULL)
		return(Q3Shared_GetReference(instanceData->pickedObject));
	
	return(NULL);
}





//=============================================================================
//      E3View_PickStack_SavePosition : Save the current pick position.
//-----------------------------------------------------------------------------
void
E3View_PickStack_SavePosition(TQ3ViewObject theView, TQ3GroupPosition thePosition)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3HitPath		*pickedPath   = &instanceData->pickedPath;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode == kQ3ViewModePicking);



	// Make sure that there's at least one slot defined
	Q3_ASSERT(pickedPath->depth     != 0);
	Q3_ASSERT(pickedPath->positions != NULL);



	// Save the position in the last slot in the positions array. We can
	// override any previous value, since group positions are not objects.
	pickedPath->positions[pickedPath->depth - 1] = thePosition;
}





//=============================================================================
//      E3View_PickStack_SaveObject : Save the current pick object.
//-----------------------------------------------------------------------------
void
E3View_PickStack_SaveObject(TQ3ViewObject theView, TQ3Object theObject)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode == kQ3ViewModePicking);



	// Save a new reference to the picked object
	E3Shared_Replace(&instanceData->pickedObject, theObject);
}





//=============================================================================
//      E3View_PickStack_BeginDecomposedObject : Begin a decomposed object.
//-----------------------------------------------------------------------------
//		Note :	When picking, we need to distinguish between objects submitted
//				by the app, and objects submitted due to the decomposition of
//				those objects.
//
//				Whenever a decomposed object is submitted, we increment a count
//				so we know that further objects can be ignored until the count
//				falls to 0 again.
//-----------------------------------------------------------------------------
void
E3View_PickStack_BeginDecomposedObject(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode == kQ3ViewModePicking);



	// Increment the count
	instanceData->pickDecomposeCount++;
	Q3_ASSERT(instanceData->pickDecomposeCount != 0);
}





//=============================================================================
//      E3View_PickStack_EndDecomposedObject : End a decomposed object.
//-----------------------------------------------------------------------------
//		Note :	When the count falls to 0, we have finished the object being
//				decomposed and we can resume tracking the current object as the
//				target for successful hits.
//-----------------------------------------------------------------------------
void
E3View_PickStack_EndDecomposedObject(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode == kQ3ViewModePicking);



	// Increment the count
	Q3_ASSERT(instanceData->pickDecomposeCount > 0);
	instanceData->pickDecomposeCount--;
}





//=============================================================================
//      E3View_PickStack_PopGroup : Pop a group off the pick path.
//-----------------------------------------------------------------------------
void
E3View_PickStack_PopGroup(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3HitPath		*pickedPath   = &instanceData->pickedPath;



	// Validate our state
	Q3_ASSERT(instanceData->viewMode == kQ3ViewModePicking);



	// Make sure that there's at least one slot defined
	Q3_ASSERT(pickedPath->depth     != 0);
	Q3_ASSERT(pickedPath->positions != NULL);



	// Shrink the positions array. Note that we just decrement the count
	// rather than shrinking the array, since future groups would cause
	// the array to be resized - which realloc will skip if the array
	// has already been extended.
	pickedPath->depth--;



	// If this was the last group, remove the root object
	if (pickedPath->depth == 0)
		Q3Object_CleanDispose(&pickedPath->rootGroup);



	// Remove our reference to the picked object
	E3Shared_Replace(&instanceData->pickedObject, NULL);
}





//=============================================================================
//      E3View_State_AddMatrixLocalToWorld : Accumulate a matrix.
//-----------------------------------------------------------------------------
TQ3Status
E3View_State_AddMatrixLocalToWorld(TQ3ViewObject theView, const TQ3Matrix4x4 *theMatrix)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status			qd3dStatus;
	TQ3Matrix4x4*		matrixState;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// No-op if this is an identity matrix
	if (E3Matrix4x4_IsIdentity(theMatrix))
		return(kQ3Success);



	// Get the matrix we need
	matrixState = &instanceData->stackState->matrixLocalToWorld;



	// Multiply in the matrix
	Q3Matrix4x4_Multiply(theMatrix,
						 matrixState,
						 matrixState);



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateMatrixLocalToWorld);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3View_State_GetLocalToWorld : Get the local to world matrix state.
//-----------------------------------------------------------------------------
const TQ3Matrix4x4 *
E3View_State_GetLocalToWorld(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Return the state
	return(&instanceData->stackState->matrixLocalToWorld);
}





//=============================================================================
//      E3View_State_GetStyleSubdivision : Get the subdivision style state.
//-----------------------------------------------------------------------------
//		Note :	The subdivision style pushed onto the stack is normalised, so
//				callers to this routine can assume that unused fields are 0.
//-----------------------------------------------------------------------------
const TQ3SubdivisionStyleData *
E3View_State_GetStyleSubdivision(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Return the state
	return(&instanceData->stackState->styleSubdivision);
}





//=============================================================================
//      E3View_State_GetStyleOrientation : Get the orientation style state.
//-----------------------------------------------------------------------------
TQ3OrientationStyle
E3View_State_GetStyleOrientation(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Return the state
	return(instanceData->stackState->styleOrientation);
}





//=============================================================================
//      E3View_State_SetShaderIllumination : Set the illumination shader state
//-----------------------------------------------------------------------------
void
E3View_State_SetShaderIllumination(TQ3ViewObject theView, const TQ3IlluminationShaderObject theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	E3Shared_Replace(&instanceData->stackState->shaderIllumination, theData);



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateShaderIllumination);
}





//=============================================================================
//      E3View_State_SetShaderSurface : Set the surface shader state
//-----------------------------------------------------------------------------
void
E3View_State_SetShaderSurface(	TQ3ViewObject theView, const TQ3SurfaceShaderObject theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	E3Shared_Replace(&instanceData->stackState->shaderSurface, theData);



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateShaderSurface);
}





//=============================================================================
//      E3View_State_SetStyleSubdivision : Set the subdivision state.
//-----------------------------------------------------------------------------
//		Note :	We normalise the value pushed on the stack, so that unused
//				fields will be 0.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleSubdivision(TQ3ViewObject theView, const TQ3SubdivisionStyleData *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->styleSubdivision = *theData;



	// Normalise it
	if (theData->method != kQ3SubdivisionMethodConstant)
		instanceData->stackState->styleSubdivision.c2 = 0.0f;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateStyleSubdivision);
}





//=============================================================================
//      E3View_State_SetStylePickID : Set the pick ID state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStylePickID(TQ3ViewObject theView, TQ3Uns32 pickID)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->stylePickID = pickID;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateStylePickID);
}





//=============================================================================
//      E3View_State_SetStylePickParts : Set the pick parts state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStylePickParts(TQ3ViewObject theView, TQ3PickParts pickParts)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->stylePickParts = pickParts;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateStylePickParts);
}





//=============================================================================
//      E3View_State_SetStyleReceiveShadows : Set the receive shadows state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleReceiveShadows(TQ3ViewObject theView, TQ3Boolean receiveShadows)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->styleReceiveShadows = receiveShadows;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateStyleReceiveShadows);
}





//=============================================================================
//      E3View_State_SetStyleFill : Set the fill state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleFill(TQ3ViewObject theView, TQ3FillStyle fillStyle)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->styleFill = fillStyle;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateStyleFill);
}





//=============================================================================
//      E3View_State_SetStyleBackfacing : Set the backfacing state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleBackfacing(TQ3ViewObject theView, TQ3BackfacingStyle backfacingStyle)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->styleBackfacing = backfacingStyle;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateStyleBackfacing);
}





//=============================================================================
//      E3View_State_SetStyleInterpolation : Set the interpolation state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleInterpolation(TQ3ViewObject theView, TQ3InterpolationStyle interpolationStyle)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->styleInterpolation = interpolationStyle;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateStyleInterpolation);
}





//=============================================================================
//      E3View_State_SetStyleHilight : Set the orientation state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleHighlight(TQ3ViewObject theView, TQ3AttributeSet highlightAttribute)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	E3Shared_Replace(&instanceData->stackState->styleHighlight, highlightAttribute);



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateStyleHighlight);
}





//=============================================================================
//      E3View_State_SetStyleOrientation : Set the orientation state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleOrientation(TQ3ViewObject theView, TQ3OrientationStyle frontFacingDirection)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->styleOrientation = frontFacingDirection;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateStyleOrientation);
}





//=============================================================================
//      E3View_State_SetStyleAntiAlias : Set the anti-alias state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleAntiAlias(TQ3ViewObject theView, const TQ3AntiAliasStyleData *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->styleAntiAlias = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateStyleAntiAlias);
}





//=============================================================================
//      E3View_State_SetStyleFog : Set the fog state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleFog(TQ3ViewObject theView, const TQ3FogStyleData *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->styleFog = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateStyleFog);
}





//=============================================================================
//      E3View_State_SetAttributeSurfaceUV : Set the surface UV state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeSurfaceUV(TQ3ViewObject theView, const TQ3Param2D *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->attributeSurfaceUV = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAttributeSurfaceUV);
}





//=============================================================================
//      E3View_State_SetAttributeShadingUV : Set the shading UV state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeShadingUV(TQ3ViewObject theView, const TQ3Param2D *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->attributeShadingUV = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAttributeShadingUV);
}





//=============================================================================
//      E3View_State_SetAttributeNormal : Set the normal state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeNormal(TQ3ViewObject theView, const TQ3Vector3D *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->attributeNormal = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAttributeNormal);
}





//=============================================================================
//      E3View_State_SetAttributeAmbientCoefficient : Set the ambient coeff.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeAmbientCoefficient(TQ3ViewObject theView, const float *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->attributeAmbientCoefficient = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAttributeAmbientCoefficient);
}





//=============================================================================
//      E3View_State_SetAttributeDiffuseColor : Set the diffuse colour state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeDiffuseColor(TQ3ViewObject theView, const TQ3ColorRGB *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->attributeDiffuseColor = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAttributeDiffuseColour);
}





//=============================================================================
//      E3View_State_SetAttributeSpecularColor : Set the specular colour state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeSpecularColor(TQ3ViewObject theView, const TQ3ColorRGB *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->attributeSpecularColor = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAttributeSpecularColour);
}





//=============================================================================
//      E3View_State_SetAttributeSpecularControl : Set the specular control.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeSpecularControl(TQ3ViewObject theView, const float *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->attributeSpecularControl = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAttributeSpecularControl);
}





//=============================================================================
//      E3View_State_SetAttributeTransparencyColor : Set the transparency.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeTransparencyColor(TQ3ViewObject theView, const TQ3ColorRGB *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->attributeTransparencyColor = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAttributeTransparencyColour);
}





//=============================================================================
//      E3View_State_SetAttributeSurfaceTangent : Set the surface tangent.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeSurfaceTangent(TQ3ViewObject theView, const TQ3Tangent2D *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->attributeSurfaceTangent = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAttributeSurfaceTangent);
}





//=============================================================================
//      E3View_State_SetAttributeHighlightState : Set the highlight state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeHighlightState(TQ3ViewObject theView, const TQ3Switch *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	instanceData->stackState->attributeHighlightState = *theData;



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAttributeHighlightState);
}





//=============================================================================
//      E3View_State_SetAttributeSurfaceShader : Set the surface shader.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeSurfaceShader(TQ3ViewObject theView, const TQ3SurfaceShaderObject *theData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Set the value
	E3Shared_Replace(&instanceData->stackState->attributeSurfaceShader, *theData);



	// Update the renderer
	qd3dStatus = e3view_stack_update(theView, kQ3ViewStateAttributeSurfaceShader);
}





//=============================================================================
//      E3View_New : Create a TQ3ViewObject.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ViewObject
E3View_New(void)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3ObjectTypeView, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3View_NewWithDefaults : Create a TQ3ViewObject with some defaults.
//-----------------------------------------------------------------------------
TQ3ViewObject
E3View_NewWithDefaults(TQ3ObjectType drawContextType, void *drawContextTarget)
{	TQ3DrawContextObject	theDrawContext;
	TQ3RendererObject		theRenderer;
	TQ3Status				qd3dStatus;
	TQ3CameraObject			theCamera;
	TQ3ViewObject			theView;



	// Create the view object
	theView = Q3View_New();
	if (theView == NULL)
		return(NULL);



	// Create the objects we need for the view
	theDrawContext = E3DrawContext_New(drawContextType, drawContextTarget);
	theCamera      = e3view_default_camera(theDrawContext);
	theRenderer    = Q3Renderer_NewFromType(kQ3RendererTypeInteractive);

	if (theDrawContext == NULL || theCamera == NULL || theRenderer == NULL)
		{
		Q3Object_Dispose(theDrawContext);
		Q3Object_Dispose(theCamera);
		Q3Object_Dispose(theRenderer);
		Q3Object_Dispose(theView);
		return(NULL);
		}



	// Configure the view
	qd3dStatus = e3view_default_lights(theView);
	qd3dStatus = Q3View_SetDrawContext(theView, theDrawContext);
	qd3dStatus = Q3View_SetRenderer(theView,    theRenderer);
	qd3dStatus = Q3View_SetCamera(theView,      theCamera);

	Q3Object_Dispose(theDrawContext);
	Q3Object_Dispose(theRenderer);
	Q3Object_Dispose(theCamera);

	return(theView);
}





//=============================================================================
//      E3View_Cancel : Cancel the current view operation.
//-----------------------------------------------------------------------------
TQ3Status
E3View_Cancel(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;
	
	
	
	// Cancel the view
	if (instanceData->viewState == kQ3ViewStateInactive)
		qd3dStatus = kQ3Failure;

	else
		{
		instanceData->viewState = kQ3ViewStateCancelled;
		qd3dStatus = kQ3Success;
		}

	return(qd3dStatus);
}





//=============================================================================
//      E3View_SetRendererByType : Select a renderer by type.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetRendererByType(TQ3ViewObject theView, TQ3ObjectType theType)
{	TQ3RendererObject		theRenderer;
	TQ3Status				qd3dStatus;



	// Create the new renderer
	theRenderer = Q3Renderer_NewFromType(theType);
	if (theRenderer == NULL)
		return(kQ3Failure);



	// Replace the existing renderer
	qd3dStatus = Q3View_SetRenderer(theView, theRenderer);
	Q3Object_Dispose(theRenderer);

	return(qd3dStatus);
}





//=============================================================================
//      E3View_SetRenderer : Assign a new renderer to a view.
//-----------------------------------------------------------------------------
//		Note :	We also reset the draw context state, so that the new renderer
//				will not end up using the state flags cleared by the previous
//				renderer.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetRenderer(TQ3ViewObject theView, TQ3RendererObject theRenderer)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;



	// Replace the existing renderer reference
	E3Shared_Replace(&instanceData->theRenderer, theRenderer);



	// Reset the draw context state
	if (instanceData->theDrawContext != NULL)
		E3DrawContext_ResetState(instanceData->theDrawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetRenderer : Return a reference to our renderer.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetRenderer(TQ3ViewObject theView, TQ3RendererObject *theRenderer)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Assign a return value
	*theRenderer = NULL;



	// Make sure we have a renderer
	if (instanceData->theRenderer == NULL)
		return(kQ3Failure);



	// Create a new reference to our renderer
	*theRenderer = Q3Shared_GetReference(instanceData->theRenderer);

	return(kQ3Success);
}





//=============================================================================
//      E3View_StartRendering : Start a rendering loop.
//-----------------------------------------------------------------------------
//		Note :	The renderer's StartFrame method is only called the first time
//				through the rendering loop - subsequent passes only call the
//				StartPass method.
//-----------------------------------------------------------------------------
TQ3Status
E3View_StartRendering(TQ3ViewObject theView)
{	TQ3ViewData				*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3DrawContextData		drawContextData;
	TQ3Matrix4x4			worldToFrustum;
	TQ3Status				qd3dStatus;



	// Make sure we have the objects we need
	if (instanceData->theDrawContext == NULL ||
		instanceData->theRenderer    == NULL || 
		instanceData->theCamera      == NULL)
		{
		if (instanceData->theDrawContext == NULL)
			E3ErrorManager_PostError(kQ3ErrorDrawContextNotSet, kQ3False);
			
		if (instanceData->theRenderer == NULL)
			E3ErrorManager_PostError(kQ3ErrorRendererNotSet, kQ3False);
			
		if (instanceData->theCamera == NULL)
			E3ErrorManager_PostError(kQ3ErrorCameraNotSet, kQ3False);
			
		return(kQ3Failure);
		}



	// Make sure we have a valid draw context pane - reject empty or negative bounds
	Q3DrawContext_GetData(instanceData->theDrawContext, &drawContextData);
	if (drawContextData.paneState)
		{
		if (drawContextData.pane.min.x >= drawContextData.pane.max.x ||
			drawContextData.pane.min.y >= drawContextData.pane.max.y)
			{
			E3ErrorManager_PostError(kQ3ErrorBadDrawContext, kQ3False);
			return(kQ3Failure);
			}
		}



	// Start the submit loop
	qd3dStatus = e3view_submit_begin(theView, kQ3ViewModeDrawing);



	// If this is the first pass then update the draw context and start the frame
	if (instanceData->viewPass == 1 && qd3dStatus == kQ3Success)
		{
		qd3dStatus = E3DrawContext_Update(instanceData->theDrawContext);

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_StartFrame(theView, instanceData->theDrawContext);
		
		if (qd3dStatus == kQ3Success)
			qd3dStatus = Q3Camera_GetWorldToFrustum(instanceData->theCamera, &worldToFrustum);

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_Method_UpdateMatrixWorldToFrustum(theView, &worldToFrustum);
		}



	// Start the pass
	if (qd3dStatus == kQ3Success)
		{
		instanceData->rendererFinishedFrame = kQ3False;
		qd3dStatus = E3Renderer_Method_StartPass(theView, instanceData->theCamera, instanceData->theLights);
		}


	// Submit the initial state
	if (qd3dStatus == kQ3Success)
		qd3dStatus = e3view_submit_initial_state(theView);



	// Handle failure
	if (qd3dStatus != kQ3Success)
		(void) e3view_submit_end(theView, kQ3ViewStatusError);

	return(qd3dStatus);
}





//=============================================================================
//      E3View_EndRendering : End a rendering loop.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3View_EndRendering(TQ3ViewObject theView)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3ViewStatus		viewStatus    = kQ3ViewStatusDone;



	// If we're still in the submit loop, end the pass
	if (instanceData->viewState == kQ3ViewStateSubmitting)
		viewStatus = E3Renderer_Method_EndPass(theView);



	// End the submit loop
	viewStatus = e3view_submit_end(theView, viewStatus);

	return(viewStatus);
}





//=============================================================================
//      E3View_Flush : Ask the renderer to flush output (non-blocking).
//-----------------------------------------------------------------------------
//		Note : Can only be called from within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_Flush(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Ask the renderer to flush output
	qd3dStatus = E3Renderer_Method_FlushFrame(theView, instanceData->theDrawContext);
	return(qd3dStatus);
}





//=============================================================================
//      E3View_Sync : Ask the renderer to flush output (blocking).
//-----------------------------------------------------------------------------
//		Note : Can only be called from outside a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_Sync(TQ3ViewObject theView)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateInactive)
		return(kQ3Failure);



	// Ask the renderer to flush output
	qd3dStatus = E3Renderer_Method_EndFrame(theView, instanceData->theDrawContext);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// If the renderer is async, block until its done. We might want to have
	// a hook in here for a platform specific yield routine, since we're just
	// wasting CPU time otherwise.
	//
	// The kQ3XMethodTypeRendererEndFrame method is only implemented by async
	// renderers, so if this method is implemented we know we need to block.
	if (E3ClassTree_GetMethod(instanceData->theRenderer->theClass,
							  kQ3XMethodTypeRendererEndFrame) != NULL)
		{
		// Skanky hack - the QD3D Interactive Renderer doesn't appear to call
		// Q3XView_EndFrame (although it should, since it implements the
		// kQ3XMethodTypeRendererEndFrame method).
		//
		// So for now, we just return immediately - this may cause flicker
		// if hardware acceleration is present.
		//
		// -dair
#if 0
		// Wait for E3View_EndFrame to set the flag
		while (!instanceData->rendererFinishedFrame)
			;
#endif
		}

	return(kQ3Success);
}





//=============================================================================
//      E3View_StartBoundingBox : Start a bounding box loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_StartBoundingBox(TQ3ViewObject theView, TQ3ComputeBounds computeBounds)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Start the submit loop
	qd3dStatus = e3view_submit_begin(theView, kQ3ViewModeCalcBounds);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// If this is the first pass, initialise the bounding box
	if (instanceData->viewPass == 1)
		{
		if (computeBounds == kQ3ComputeBoundsExact)
			instanceData->boundingMethod = kQ3BoxBoundsExact;
		else
			instanceData->boundingMethod = kQ3BoxBoundsApprox;
		
		instanceData->boundingBox.min.x   = 0.0f;
		instanceData->boundingBox.min.y   = 0.0f;
		instanceData->boundingBox.min.z   = 0.0f;
		instanceData->boundingBox.max.x   = 0.0f;
		instanceData->boundingBox.max.y   = 0.0f;
		instanceData->boundingBox.max.z   = 0.0f;
		instanceData->boundingBox.isEmpty = kQ3True;
		}



	// Submit the initial state
	if (qd3dStatus == kQ3Success)
		qd3dStatus = e3view_submit_initial_state(theView);

	return(qd3dStatus);
}





//=============================================================================
//      E3View_EndBoundingBox : End a bounding box loop.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3View_EndBoundingBox(TQ3ViewObject theView, TQ3BoundingBox *result)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3ViewStatus		viewStatus    = kQ3ViewStatusDone;



	// If we're still in the submit loop, return the results
	if (instanceData->viewState == kQ3ViewStateSubmitting)
		*result = instanceData->boundingBox;



	// End the submit loop
	viewStatus = e3view_submit_end(theView, viewStatus);

	return(viewStatus);
}





//=============================================================================
//      E3View_StartBoundingSphere : Start a bounding sphere loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_StartBoundingSphere(TQ3ViewObject theView, TQ3ComputeBounds computeBounds)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Start the submit loop
	qd3dStatus = e3view_submit_begin(theView, kQ3ViewModeCalcBounds);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// If this is the first pass, initialise the bounding sphere
	if (instanceData->viewPass == 1)
		{
		if (computeBounds == kQ3ComputeBoundsExact)
			instanceData->boundingMethod = kQ3SphereBoundsExact;
		else
			instanceData->boundingMethod = kQ3SphereBoundsApprox;
		
		instanceData->boundingSphere.origin.x = 0.0f;
		instanceData->boundingSphere.origin.y = 0.0f;
		instanceData->boundingSphere.origin.z = 0.0f;
		instanceData->boundingSphere.radius   = 0.0f;
		instanceData->boundingSphere.isEmpty  = kQ3True;
		}



	// Submit the initial state
	if (qd3dStatus == kQ3Success)
		qd3dStatus = e3view_submit_initial_state(theView);

	return(qd3dStatus);
}





//=============================================================================
//      E3View_EndBoundingSphere : End a bounding sphere loop.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3View_EndBoundingSphere(TQ3ViewObject theView, TQ3BoundingSphere *result)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3ViewStatus		viewStatus    = kQ3ViewStatusDone;



	// If we're still in the submit loop, return the results
	if (instanceData->viewState == kQ3ViewStateSubmitting)
		*result = instanceData->boundingSphere;



	// End the submit loop
	viewStatus = e3view_submit_end(theView, viewStatus);

	return(viewStatus);
}





//=============================================================================
//      E3View_StartPicking : Start a picking loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_StartPicking(TQ3ViewObject theView, TQ3PickObject pick)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Start the submit loop
	qd3dStatus = e3view_submit_begin(theView, kQ3ViewModePicking);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// If this is the first pass, start picking
	if (instanceData->viewPass == 1)
		e3view_pick_begin(theView, pick);



	// Submit the initial state
	if (qd3dStatus == kQ3Success)
		qd3dStatus = e3view_submit_initial_state(theView);

	return(qd3dStatus);
}





//=============================================================================
//      E3View_EndPicking : End a picking loop.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3View_EndPicking(TQ3ViewObject theView)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3ViewStatus		viewStatus    = kQ3ViewStatusDone;



	// If we're still in the submit loop, end the pass
	if (instanceData->viewState == kQ3ViewStateSubmitting)
		viewStatus = kQ3ViewStatusDone;
		


	// End the submit loop
	viewStatus = e3view_submit_end(theView, viewStatus);
	if (viewStatus != kQ3ViewStatusRetraverse)
		e3view_pick_end(theView);
	
	return(viewStatus);
}





//=============================================================================
//      E3View_StartWriting : Start a writing loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_StartWriting(TQ3ViewObject theView, TQ3FileObject theFile)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;



	// Start the submit loop
	qd3dStatus = e3view_submit_begin(theView, kQ3ViewModeWriting);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Replace the existing file reference
	if (qd3dStatus == kQ3Success)
		E3Shared_Replace(&instanceData->theFile, theFile);



	// If this is the first pass then prepare the file for writing
	if (instanceData->viewPass == 1 && qd3dStatus == kQ3Success)
		qd3dStatus = E3FileFormat_Method_StartFile(theView);



	// Start the pass
	if (qd3dStatus == kQ3Success)
		{
		instanceData->rendererFinishedFrame = kQ3False;
		qd3dStatus = E3FileFormat_Method_StartPass(theView);
		}

	return(qd3dStatus);
}





//=============================================================================
//      E3View_EndWriting : End a writing loop.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3View_EndWriting(TQ3ViewObject theView)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3ViewStatus		viewStatus    = kQ3ViewStatusDone;



	// If we're still in the submit loop, end the pass
	if (instanceData->viewState == kQ3ViewStateSubmitting)
		viewStatus = E3FileFormat_Method_EndPass(theView);



	// End the submit loop
	viewStatus = e3view_submit_end(theView, viewStatus);



	// If we don't need to retraverse, drop the file reference
	if (viewStatus != kQ3ViewStatusRetraverse)
		{
		E3FileFormat_Method_EndFile(theView);
		E3Shared_Replace(&instanceData->theFile, NULL);
		}

	return(viewStatus);
}





//=============================================================================
//      E3View_GetCamera : Return a reference to our camera.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetCamera(TQ3ViewObject theView, TQ3CameraObject *theCamera)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Assign a return value
	*theCamera = NULL;



	// Make sure we have a camera
	if (instanceData->theCamera == NULL)
		return(kQ3Failure);



	// Create a new reference to our camera
	*theCamera = Q3Shared_GetReference(instanceData->theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3View_SetCamera : Assign a new camera to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetCamera(TQ3ViewObject theView, TQ3CameraObject theCamera)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Replace the existing camera reference
	E3Shared_Replace(&instanceData->theCamera, theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3View_SetLightGroup : Assign a new light group to a view.
//-----------------------------------------------------------------------------
//		Note : Lights are removed if lightGroup is NULL.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetLightGroup(TQ3ViewObject theView, TQ3GroupObject lightGroup)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Replace the existing light group reference
	E3Shared_Replace(&instanceData->theLights, lightGroup);

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetLightGroup : Return a reference to our light group.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetLightGroup(TQ3ViewObject theView, TQ3GroupObject *lightGroup)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Assign a return value
	*lightGroup = NULL;



	// If we have a light group, create a new reference to it
	if (instanceData->theLights != NULL)
		*lightGroup = Q3Shared_GetReference(instanceData->theLights);

	return(kQ3Success);
}





//=============================================================================
//      E3View_AddLight : Add a light to a view's light group.
//-----------------------------------------------------------------------------
TQ3Status
E3View_AddLight(TQ3ViewObject theView, TQ3ObjectType lightType, void *lightData)
{	TQ3GroupObject		lightGroup;
	TQ3Status			qd3dStatus;
	TQ3LightObject		theLight;



	// Get the light group for the view
	qd3dStatus = Q3View_GetLightGroup(theView, &lightGroup);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// If we don't have a light group yet, create one
	if (lightGroup == NULL)
		{
		lightGroup = Q3LightGroup_New();
		if (lightGroup == NULL)
			return(kQ3Failure);
		
		Q3View_SetLightGroup(theView, lightGroup);
		}



	// Create the light object
	//
	// If we've been passed an existing light object we need to increment
	// the reference count, so that the caller's reference remains valid.
	switch (lightType) {
		case kQ3LightTypeAmbient:
			theLight = Q3AmbientLight_New((TQ3LightData *) lightData);
			break;

		case kQ3LightTypeDirectional:
			theLight = Q3DirectionalLight_New((TQ3DirectionalLightData *) lightData);
			break;

		case kQ3LightTypePoint:
			theLight = Q3PointLight_New((TQ3PointLightData *) lightData);
			break;

		case kQ3LightTypeSpot:
			theLight = Q3SpotLight_New((TQ3SpotLightData *) lightData);
			break;
		
		case kQ3ShapeTypeLight:
			theLight = *((TQ3LightObject *) lightData);
			if (theLight != NULL)
				Q3Shared_GetReference(theLight);
			break;

		default:
			theLight = NULL;
			break;
		}



	// Add the light to the light group
	Q3Group_AddObjectAndDispose(lightGroup, &theLight);
	Q3Object_Dispose(lightGroup);
	
	return(kQ3Success);
}





//=============================================================================
//      E3View_SetIdleMethod : Set the idle method.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetIdleMethod(TQ3ViewObject theView, TQ3ViewIdleMethod idleMethod, const void *idleData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Update the object
	instanceData->idleMethod = idleMethod;
	instanceData->idleData   = idleData;
	
	return(kQ3Success);
}





//=============================================================================
//      E3View_SetIdleProgressMethod : Set the idle progress method.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetIdleProgressMethod(TQ3ViewObject theView, TQ3ViewIdleProgressMethod idleMethod, const void *idleData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Update the object
	instanceData->idleProgressMethod = idleMethod;
	instanceData->idleProgressData   = idleData;
	
	return(kQ3Success);
}





//=============================================================================
//      E3View_SetEndFrameMethod : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetEndFrameMethod(TQ3ViewObject theView, TQ3ViewEndFrameMethod endFrame, void *endFrameData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Update the object
	instanceData->endFrameMethod = endFrame;
	instanceData->endFrameData   = endFrameData;
	
	return(kQ3Success);
}





//=============================================================================
//      E3View_IsBoundingBoxVisible : See if a bounding box is visible.
//-----------------------------------------------------------------------------
TQ3Boolean
E3View_IsBoundingBoxVisible(TQ3ViewObject theView, const TQ3BoundingBox *theBBox)
{	TQ3Boolean		isVisible;



	// Ask the renderer if the bounding box is visible
	isVisible = E3Renderer_Method_IsBBoxVisible(theView, theBBox);

	return(isVisible);
}





//=============================================================================
//      E3View_AllowAllGroupCulling : Set group culling behaviour.
//-----------------------------------------------------------------------------
TQ3Status
E3View_AllowAllGroupCulling(TQ3ViewObject theView, TQ3Boolean allowCulling)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Update our state
	instanceData->allowGroupCulling = allowCulling;

	return(kQ3Success);
}





//=============================================================================
//      E3View_TransformLocalToWorld : Transform a point from local->world.
//-----------------------------------------------------------------------------
TQ3Status
E3View_TransformLocalToWorld(TQ3ViewObject theView, const TQ3Point3D *localPoint, TQ3Point3D *worldPoint)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	const TQ3Matrix4x4	*localToWorld;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Get the local to world matrix
	localToWorld = &instanceData->stackState->matrixLocalToWorld;
	Q3_ASSERT_VALID_PTR(localToWorld);



	// Transform the point
	Q3Point3D_Transform(localPoint, localToWorld, worldPoint);
	return(kQ3Success);
}





//=============================================================================
//      E3View_TransformLocalToWindow : Transform a point from local->window.
//-----------------------------------------------------------------------------
TQ3Status
E3View_TransformLocalToWindow(TQ3ViewObject theView, const TQ3Point3D *localPoint, TQ3Point2D *windowPoint)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Matrix4x4		theMatrix, worldToFrustum, frustumToWindow;
	TQ3Point3D			thePoint;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Get the matrices we ned
	Q3View_GetWorldToFrustumMatrixState(theView,  &worldToFrustum);
	Q3View_GetFrustumToWindowMatrixState(theView, &frustumToWindow);

	Q3Matrix4x4_Multiply(E3View_State_GetLocalToWorld(theView), &worldToFrustum, &theMatrix);
	Q3Matrix4x4_Multiply(&theMatrix, &frustumToWindow, &theMatrix);



	// Transform the point
	Q3Point3D_Transform(localPoint, &theMatrix, &thePoint);
	windowPoint->x = thePoint.x;
	windowPoint->y = thePoint.y;

	return(kQ3Success);
}





//=============================================================================
//      E3View_TransformWorldToWindow : Transform a point from world->window.
//-----------------------------------------------------------------------------
TQ3Status
E3View_TransformWorldToWindow(TQ3ViewObject theView, const TQ3Point3D *worldPoint, TQ3Point2D *windowPoint)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Matrix4x4		theMatrix, worldToFrustum, frustumToWindow;
	TQ3Point3D			thePoint;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Get the matrices we ned
	Q3View_GetWorldToFrustumMatrixState(theView,  &worldToFrustum);
	Q3View_GetFrustumToWindowMatrixState(theView, &frustumToWindow);
	Q3Matrix4x4_Multiply(&worldToFrustum, &frustumToWindow, &theMatrix);



	// Transform the point
	Q3Point3D_Transform(worldPoint, &theMatrix, &thePoint);
	windowPoint->x = thePoint.x;
	windowPoint->y = thePoint.y;

	return(kQ3Success);
}





//=============================================================================
//      E3View_SetDrawContext : Assign a new draw context to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetDrawContext(TQ3ViewObject theView, TQ3DrawContextObject drawContext)
{	TQ3ViewData			*instanceData = (TQ3ViewData *) theView->instanceData;



	// Replace the existing draw context reference, and reset the state
	E3Shared_Replace(&instanceData->theDrawContext, drawContext);

	if (instanceData->theDrawContext != NULL)
		E3DrawContext_ResetState(instanceData->theDrawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetDrawContext : Return a reference to our draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetDrawContext(TQ3ViewObject theView, TQ3DrawContextObject *drawContext)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Assign a return value
	*drawContext = NULL;



	// Make sure we have a draw context
	if (instanceData->theDrawContext == NULL)
		return(kQ3Failure);



	// Create a new reference to our camera
	*drawContext = Q3Shared_GetReference(instanceData->theDrawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetLocalToWorldMatrixState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetLocalToWorldMatrixState(TQ3ViewObject theView, TQ3Matrix4x4 *theMatrix)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*theMatrix = instanceData->stackState->matrixLocalToWorld;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetWorldToFrustumMatrixState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetWorldToFrustumMatrixState(TQ3ViewObject theView, TQ3Matrix4x4 *theMatrix)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;
	


	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Get the matrix
	qd3dStatus = Q3Camera_GetWorldToFrustum(instanceData->theCamera, theMatrix);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3View_GetFrustumToWindowMatrixState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetFrustumToWindowMatrixState(TQ3ViewObject theView, TQ3Matrix4x4 *theMatrix)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;
	TQ3Status		qd3dStatus;
	TQ3Area			thePane;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Get the size of our draw context
	qd3dStatus = Q3DrawContext_GetPane(instanceData->theDrawContext, &thePane);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Return the frustum to window matrix
	Q3Matrix4x4_SetIdentity(theMatrix);

	theMatrix->value[0][0] = (thePane.max.x - thePane.min.x) * 0.5f;
	theMatrix->value[1][1] = -(thePane.max.y - thePane.min.y) * 0.5f;
	theMatrix->value[3][0] = (thePane.max.x + thePane.min.x) * 0.5f;
	theMatrix->value[3][1] = (thePane.max.y + thePane.min.y) * 0.5f;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetBackfacingStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetBackfacingStyleState(TQ3ViewObject theView, TQ3BackfacingStyle *backfacingStyle)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*backfacingStyle = instanceData->stackState->styleBackfacing;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetInterpolationStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetInterpolationStyleState(TQ3ViewObject theView, TQ3InterpolationStyle *interpolationType)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*interpolationType = instanceData->stackState->styleInterpolation;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetFillStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetFillStyleState(TQ3ViewObject theView, TQ3FillStyle *fillStyle)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*fillStyle = instanceData->stackState->styleFill;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetHighlightStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetHighlightStyleState(TQ3ViewObject theView, TQ3AttributeSet *highlightStyle)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*highlightStyle = NULL;
	if (instanceData->stackState->styleHighlight != NULL)
		*highlightStyle = Q3Shared_GetReference(instanceData->stackState->styleHighlight);

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetSubdivisionStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetSubdivisionStyleState(TQ3ViewObject theView, TQ3SubdivisionStyleData *subdivisionStyle)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*subdivisionStyle = instanceData->stackState->styleSubdivision;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetOrientationStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetOrientationStyleState(TQ3ViewObject theView, TQ3OrientationStyle *frontFacingDirectionStyle)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*frontFacingDirectionStyle = instanceData->stackState->styleOrientation;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetReceiveShadowsStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetReceiveShadowsStyleState(TQ3ViewObject theView, TQ3Boolean *receives)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*receives = instanceData->stackState->styleReceiveShadows;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetPickIDStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetPickIDStyleState(TQ3ViewObject theView, TQ3Uns32 *pickIDStyle)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*pickIDStyle = instanceData->stackState->stylePickID;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetPickPartsStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetPickPartsStyleState(TQ3ViewObject theView, TQ3PickParts *pickPartsStyle)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*pickPartsStyle = instanceData->stackState->stylePickParts;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetAntiAliasStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetAntiAliasStyleState(TQ3ViewObject theView, TQ3AntiAliasStyleData *antiAliasData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*antiAliasData = instanceData->stackState->styleAntiAlias;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetFogStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetFogStyleState(TQ3ViewObject theView, TQ3FogStyleData *fogData)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Make sure we're in the correct state
	if (instanceData->viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));



	// Get the value
	*fogData = instanceData->stackState->styleFog;

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetDefaultAttributeSet : Return the view default attribute set.
//-----------------------------------------------------------------------------
//		Note :	We return a new reference to the attribute set - not clear from
//				the QD3D docs if this is the correct behaviour, but for now we
//				assume it is.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetDefaultAttributeSet(TQ3ViewObject theView, TQ3AttributeSet *attributeSet)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Return a new reference to the attribute set
	if (instanceData->viewAttributes != NULL)
		*attributeSet = Q3Shared_GetReference(instanceData->viewAttributes);
	else
		*attributeSet = NULL;

	return(kQ3Success);
}





//=============================================================================
//      E3View_SetDefaultAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetDefaultAttributeSet(TQ3ViewObject theView, TQ3AttributeSet attributeSet)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Replace the existing attribute set reference
	E3Shared_Replace(&instanceData->viewAttributes, attributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetAttributeSetState : Get the current attribute set for a view.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetAttributeSetState(TQ3ViewObject theView, TQ3AttributeSet *attributeSet)
{	TQ3ViewData		*instanceData = (TQ3ViewData *) theView->instanceData;



	// Set up a return value
	*attributeSet = NULL;



	// If there's anything on the stack, return the current attribute set
	if (instanceData->stackState != NULL)
		{
		Q3_ASSERT(Q3_VALID_PTR(instanceData->stackState));
		if(instanceData->stackState->attributeSet != NULL)
			*attributeSet = Q3Shared_GetReference(instanceData->stackState->attributeSet);
		else
			Q3View_GetDefaultAttributeSet (theView, attributeSet);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3View_GetAttributeState : Get the current state of an attribute.
//-----------------------------------------------------------------------------
//		Note :	The QD3D docs describe this function as returning a pointer to
//				a pointer, however the prototype is for a function that only
//				takes a pointer (not a pointer to a pointer).
//
//				For now, we assume the docs are right: this may need to be
//				changed if it causes a problem.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetAttributeState(TQ3ViewObject theView, TQ3AttributeType attributeType, void *data)
{	TQ3AttributeSet		attributeSet = NULL;



	// Get the attribute set
	Q3View_GetAttributeSetState(theView, &attributeSet);



	// If we didn't get it, we're done
	if (attributeSet == NULL)
		{
		*((void **) data) = NULL;
		return(kQ3Success);
		}



	// Get a pointer to the data for the attribute
	*((void **) data) = Q3XAttributeSet_GetPointer(attributeSet, attributeType);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Push_Submit : Push the view state.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Push_Submit(TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StateOperatorTypePush, NULL);

	return(qd3dStatus);
}





//=============================================================================
//      E3Pop_Submit : Pop the view state.
//-----------------------------------------------------------------------------
TQ3Status
E3Pop_Submit(TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StateOperatorTypePop, NULL);

	return(qd3dStatus);
}





//=============================================================================
//      E3Push_New : Create a new push operator object.
//-----------------------------------------------------------------------------
TQ3StateOperatorObject
E3Push_New(void)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StateOperatorTypePush, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3Pop_New : Create a new pop operator object.
//-----------------------------------------------------------------------------
TQ3StateOperatorObject
E3Pop_New(void)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StateOperatorTypePop, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3StateOperator_Submit : Submit a push or pop state operator object.
//-----------------------------------------------------------------------------
TQ3Status
E3StateOperator_Submit(TQ3StateOperatorObject stateOperator, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitRetained(theView, stateOperator);

	return(qd3dStatus);
}



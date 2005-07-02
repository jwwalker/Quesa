/*  NAME:
        E3View.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
enum {
	kQ3ViewStateMatrixLocalToWorld			= 1 <<  0,		// Local-to-world changed
	kQ3ViewStateMatrixWorldToCamera			= 1 <<  1,		// World-to-camera changed
	kQ3ViewStateMatrixCameraToFrustum		= 1 <<  2,		// Camera-to-frustum changed
	kQ3ViewStateShaderIllumination			= 1 <<	3,		// Illumination shader changed
	kQ3ViewStateShaderSurface				= 1 <<	4,		// Surface shader changed
	kQ3ViewStateStyleBackfacing				= 1 <<  5,		// Backfacing style changed
	kQ3ViewStateStyleInterpolation			= 1 <<  6,		// Interpolation style changed
	kQ3ViewStateStyleFill					= 1 <<  7,		// Fill style changed
	kQ3ViewStateStyleHighlight				= 1 <<  8,		// Highlight style changed
	kQ3ViewStateStyleSubdivision			= 1 <<  9,		// Subdivision style changed
	kQ3ViewStateStyleOrientation			= 1 << 10,		// Orientation style changed
	kQ3ViewStateStyleCastShadows			= 1 << 11,		// Cast Shadows style changed
	kQ3ViewStateStyleReceiveShadows			= 1 << 12,		// Receive Shadows style changed
	kQ3ViewStateStylePickID					= 1 << 13,		// Pick ID style changed
	kQ3ViewStateStylePickParts				= 1 << 14,		// Pick Parts style changed
	kQ3ViewStateStyleAntiAlias				= 1 << 15,		// Anti-alias style changed
	kQ3ViewStateStyleFog					= 1 << 16,		// Fog style changed
	kQ3ViewStateAttributeSurfaceUV			= 1 << 17,		// Surface UV attribute changed
	kQ3ViewStateAttributeShadingUV			= 1 << 18,		// Shading UV attribute changed
	kQ3ViewStateAttributeNormal				= 1 << 19,		// Normal attribute changed
	kQ3ViewStateAttributeAmbientCoefficient	= 1 << 20,		// Ambient coefficient attribute changed
	kQ3ViewStateAttributeDiffuseColour		= 1 << 21,		// Diffuse colour attribute changed
	kQ3ViewStateAttributeSpecularColour		= 1 << 22,		// Specular colour attribute changed
	kQ3ViewStateAttributeSpecularControl	= 1 << 23,		// Specular control attribute changed
	kQ3ViewStateAttributeTransparencyColour = 1 << 24,		// Transparency colour attribute changed
	kQ3ViewStateAttributeSurfaceTangent		= 1 << 25,		// Surface tangent attribute changed
	kQ3ViewStateAttributeHighlightState		= 1 << 26,		// Highlight switch attribute changed
	kQ3ViewStateAttributeSurfaceShader		= 1 << 27,		// Surface shader attribute changed
	kQ3ViewStateNone						= 0,			// Nothing changed
	kQ3ViewStateAll							= 0xFFFFFFFF,	// Everything changed
	kQ3ViewStateMatrixAny					= kQ3ViewStateMatrixLocalToWorld  |	// Any matrix changed
											  kQ3ViewStateMatrixWorldToCamera |
											  kQ3ViewStateMatrixCameraToFrustum
};

typedef TQ3Uns32 TQ3ViewStackState;





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
class E3View ;




// Stack data
typedef struct TQ3ViewStackItem {
	// Next stack item
	struct TQ3ViewStackItem*	next;
	
	
	// Stack item state
	TQ3ViewStackState			stackState;
	TQ3AttributeSet				attributeSet;
	TQ3Matrix4x4				matrixLocalToWorld;
	TQ3Matrix4x4				matrixWorldToCamera;
	TQ3Matrix4x4				matrixCameraToFrustum;
	TQ3ShaderObject				shaderIllumination;
	TQ3ShaderObject				shaderSurface;
	TQ3BackfacingStyle			styleBackfacing;
	TQ3InterpolationStyle		styleInterpolation;
	TQ3FillStyle				styleFill;
	TQ3AttributeSet				styleHighlight;
	TQ3SubdivisionStyleData		styleSubdivision;
	TQ3OrientationStyle			styleOrientation;
	TQ3Boolean					styleCastShadows;
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
	TQ3XViewSubmitRetainedMethod		submitRetainedMethod;
	TQ3XViewSubmitImmediateMethod		submitImmediateMethod;
	TQ3AttributeSet				viewAttributes;
	TQ3Boolean					allowGroupCulling;


	// View stack
	TQ3ViewStackItem			*viewStack;


	// Bounds state
	TQ3BoundingMethod			boundingMethod;
	TQ3BoundingBox				boundingBox;
	TQ3SlabObject				boundingPointsSlab;
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





class E3View : public OpaqueTQ3Object // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeView, E3View, OpaqueTQ3Object )
public :

	TQ3ViewData					instanceData ;
	} ;
	


class E3StateOperator : public E3Shape // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
	{
Q3_CLASS_ENUMS ( kQ3ShapeTypeStateOperator, E3StateOperator, E3Shape )
public :

	// There is no extra data for this class
	} ;
	


class E3Push : public E3StateOperator  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3StateOperatorTypePush, E3Push, E3StateOperator )
public :

	// There is no extra data for this class
	} ;
	


class E3Pop : public E3StateOperator  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3StateOperatorTypePop, E3Pop, E3StateOperator )
public :

	// There is no extra data for this class
	} ;
	


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
	Q3Matrix4x4_SetIdentity(&theItem->matrixWorldToCamera);
	Q3Matrix4x4_SetIdentity(&theItem->matrixCameraToFrustum);

	theItem->next					 = NULL;
	theItem->stackState				 = kQ3ViewStateNone;
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
	theItem->styleCastShadows        = kQ3True;
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
	if (qd3dStatus != kQ3Failure)
		qd3dStatus = E3Renderer_Method_UpdateAttribute(theView, attributeType, paramData);
	
	return qd3dStatus ;
}





//=============================================================================
//      e3view_stack_update : Update the renderer state.
//-----------------------------------------------------------------------------
//		Note :	We take the topmost item from the view's state stack, and the
//				mask of which fields have changed, and inform the renderer of
//				the change.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_stack_update ( E3View* view, TQ3ViewStackState stateChange )
	{
	TQ3Status qd3dStatus = kQ3Success ;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(view);



	// If the stack is empty, we're done
	if ( view->instanceData.viewStack == NULL )
		return kQ3Success ;



	// Find the item at the top of the stack
	Q3_ASSERT_VALID_PTR( view->instanceData.viewStack);
	TQ3ViewStackItem* theItem = view->instanceData.viewStack ;



	// Update the renderer if we're currently drawing
	if ( view->instanceData.viewMode == kQ3ViewModeDrawing )
		{
		if ( ( stateChange & kQ3ViewStateMatrixAny ) && qd3dStatus != kQ3Failure )
			{
			// Identify the matrices which have changed
			TQ3MatrixState matrixState = kQ3MatrixStateNone ;
			if ( stateChange & kQ3ViewStateMatrixLocalToWorld )    matrixState |= kQ3MatrixStateLocalToWorld ;
			if ( stateChange & kQ3ViewStateMatrixWorldToCamera )   matrixState |= kQ3MatrixStateWorldToCamera ;
			if ( stateChange & kQ3ViewStateMatrixCameraToFrustum ) matrixState |= kQ3MatrixStateCameraToFrustum ;


			// And update them
			qd3dStatus = E3Renderer_Method_UpdateMatrix ( view, matrixState,
														&theItem->matrixLocalToWorld,
														&theItem->matrixWorldToCamera,
														&theItem->matrixCameraToFrustum ) ;
			}

		if ( ( stateChange & kQ3ViewStateShaderIllumination ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateShader ( view, kQ3ShaderTypeIllumination, &theItem->shaderIllumination ) ;
		
		if ( ( stateChange & kQ3ViewStateShaderSurface ) && qd3dStatus != kQ3Failure )
			{
			// QD3D only submits textures when in kQ3FillStyleFilled mode, so we do the same
			if ( theItem->styleFill == kQ3FillStyleFilled )
				qd3dStatus = E3Renderer_Method_UpdateShader ( view, kQ3ShaderTypeSurface, &theItem->shaderSurface ) ;
			}
	
		if ( ( stateChange & kQ3ViewStateStyleBackfacing ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypeBackfacing, &theItem->styleBackfacing ) ;

		if ( ( stateChange & kQ3ViewStateStyleInterpolation ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypeInterpolation, &theItem->styleInterpolation ) ;

		if ( ( stateChange & kQ3ViewStateStyleFill ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypeFill, &theItem->styleFill ) ;

		if ( ( stateChange & kQ3ViewStateStyleHighlight ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypeHighlight, &theItem->styleHighlight ) ;

		if ( ( stateChange & kQ3ViewStateStyleSubdivision ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypeSubdivision, &theItem->styleSubdivision ) ;

		if ( ( stateChange & kQ3ViewStateStyleOrientation ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypeOrientation, &theItem->styleOrientation ) ;

		if ( ( stateChange & kQ3ViewStateStyleCastShadows ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypeCastShadows, &theItem->styleCastShadows ) ;

		if ( ( stateChange & kQ3ViewStateStyleReceiveShadows ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypeReceiveShadows, &theItem->styleReceiveShadows ) ;

		if ( ( stateChange & kQ3ViewStateStylePickID ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypePickID, &theItem->stylePickID ) ;

		if ( ( stateChange & kQ3ViewStateStylePickParts ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypePickParts, &theItem->stylePickParts ) ;

		if ( ( stateChange & kQ3ViewStateStyleAntiAlias ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypeAntiAlias, &theItem->styleAntiAlias ) ;

		if ( ( stateChange & kQ3ViewStateStyleFog ) && qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_UpdateStyle ( view, kQ3StyleTypeFog, &theItem->styleFog ) ;

		if ( ( stateChange & kQ3ViewStateAttributeSurfaceUV ) && qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_stack_update_attribute ( view, theItem, kQ3AttributeTypeSurfaceUV, &theItem->attributeSurfaceUV ) ;

		if ( ( stateChange & kQ3ViewStateAttributeShadingUV ) && qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_stack_update_attribute ( view, theItem, kQ3AttributeTypeShadingUV, &theItem->attributeShadingUV ) ;

		if ( ( stateChange & kQ3ViewStateAttributeNormal ) && qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_stack_update_attribute ( view, theItem, kQ3AttributeTypeNormal, &theItem->attributeNormal ) ;

		if ( ( stateChange & kQ3ViewStateAttributeAmbientCoefficient ) && qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_stack_update_attribute ( view, theItem, kQ3AttributeTypeAmbientCoefficient, &theItem->attributeAmbientCoefficient ) ;

		if ( ( stateChange & kQ3ViewStateAttributeDiffuseColour ) && qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_stack_update_attribute ( view, theItem, kQ3AttributeTypeDiffuseColor, &theItem->attributeDiffuseColor ) ;

		if ( ( stateChange & kQ3ViewStateAttributeSpecularColour ) && qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_stack_update_attribute ( view, theItem, kQ3AttributeTypeSpecularColor, &theItem->attributeSpecularColor ) ;

		if ( ( stateChange & kQ3ViewStateAttributeSpecularControl ) && qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_stack_update_attribute ( view, theItem, kQ3AttributeTypeSpecularControl, &theItem->attributeSpecularControl ) ;

		if ( ( stateChange & kQ3ViewStateAttributeTransparencyColour ) && qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_stack_update_attribute ( view, theItem, kQ3AttributeTypeTransparencyColor, &theItem->attributeTransparencyColor ) ;

		if ( ( stateChange & kQ3ViewStateAttributeSurfaceTangent ) && qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_stack_update_attribute ( view, theItem, kQ3AttributeTypeSurfaceTangent, &theItem->attributeSurfaceTangent ) ;

		if ( ( stateChange & kQ3ViewStateAttributeHighlightState ) && qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_stack_update_attribute ( view, theItem, kQ3AttributeTypeHighlightState, &theItem->attributeHighlightState ) ;

		if ( ( stateChange & kQ3ViewStateAttributeSurfaceShader ) && qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_stack_update_attribute ( view, theItem, kQ3AttributeTypeSurfaceShader, &theItem->attributeSurfaceShader ) ;
		}
	
	

	// Update what has changed since the last push.
	theItem->stackState |= stateChange ;



	return qd3dStatus ;
	}





//=============================================================================
//      e3view_stack_push : Push the view state stack.
//-----------------------------------------------------------------------------
//		Note :	The first item is initialise to default values, and further
//				items are cloned from the previously topmost item on the stack.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_stack_push ( E3View* view )
	{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(view);



	// Grow the view stack to the hold the new item
	TQ3ViewStackItem* newTop = (TQ3ViewStackItem*) Q3Memory_Allocate ( sizeof ( TQ3ViewStackItem ) ) ;
	if ( newTop == NULL )
		{
		return kQ3Failure ;
		}
	TQ3ViewStackItem* oldTop = view->instanceData.viewStack ;
	view->instanceData.viewStack = newTop ;
	

	// If this is the first item, initialise it
	if ( oldTop == NULL )
		{
		e3view_stack_initialise ( newTop ) ;
		newTop->next = oldTop ;
		}
	
	// Otherwise, clone what was on the top to the new top
	else
		{
		// Take a copy of the state
		Q3Memory_Copy ( oldTop, newTop, sizeof ( TQ3ViewStackItem ) ) ;
		newTop->next = oldTop ;



		// The stack state represents renderer state that has been changed since the push.
		newTop->stackState = kQ3ViewStateNone ;



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
		if ( oldTop->attributeSet != NULL )
			newTop->attributeSet = Q3Object_Duplicate ( oldTop->attributeSet ) ;
		
		E3Shared_Acquire ( &newTop->shaderIllumination,     oldTop->shaderIllumination ) ;
		E3Shared_Acquire ( &newTop->shaderSurface,          oldTop->shaderSurface ) ;
		E3Shared_Acquire ( &newTop->styleHighlight,         oldTop->styleHighlight ) ;
		E3Shared_Acquire ( &newTop->attributeSurfaceShader, oldTop->attributeSurfaceShader ) ;
		}



	return kQ3Success ;
	}





//=============================================================================
//      e3view_stack_pop : Pop the view state stack.
//-----------------------------------------------------------------------------
static void
e3view_stack_pop ( E3View* view )
	{
	// Validate our parameters and state
	Q3_ASSERT_VALID_PTR(view);
	Q3_REQUIRE(Q3_VALID_PTR(view->instanceData.viewStack));



	// Save the state mask for the topmost item
	TQ3ViewStackState theStateToUpdate = view->instanceData.viewStack->stackState ;



	// Dispose of the shared objects in the topmost item
	Q3Object_CleanDispose ( & view->instanceData.viewStack->attributeSet ) ;
	Q3Object_CleanDispose ( & view->instanceData.viewStack->shaderIllumination ) ;
	Q3Object_CleanDispose ( & view->instanceData.viewStack->shaderSurface ) ;
	Q3Object_CleanDispose ( & view->instanceData.viewStack->styleHighlight ) ;
	Q3Object_CleanDispose ( & view->instanceData.viewStack->attributeSurfaceShader ) ;



	// Shrink the stack to get rid of the top item
	TQ3ViewStackItem* theItem = view->instanceData.viewStack ;
	view->instanceData.viewStack = view->instanceData.viewStack->next ;
	Q3Memory_Free( &theItem ) ;



	// Update the renderer, using the state of the previously top
	// item as the mask indicating what's changed.
	
	// In so doing, the mask of changes to view state after the last push
	// will be ORed with the mask of changes before the push.
	
	// One might think that the mask could be kept the same as before the
	// push, since we are undoing those state changes.  However, with the
	// current design, it is not quite possible, because the renderer state
	// keeps pointers into the top of the view stack.
	// Suppose, for instance, you push twice, then set a diffuse color, then
	// pop twice.  On the first (inner) pop, the renderer state points at the color
	// in the TQ3ViewStackItem created by the first (outer) push.  The second
	// pop deletes that storage, so it had better update the renderer.
	e3view_stack_update ( view, theStateToUpdate ) ;
	}





//=============================================================================
//      e3view_stack_pop_clean : Pop the view stack clean.
//-----------------------------------------------------------------------------
static void
e3view_stack_pop_clean ( E3View* view )
	{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(view);



	// Pop the stack clean
	while ( view->instanceData.viewStack != NULL )
		e3view_stack_pop ( view ) ;
	}





//=============================================================================
//      e3view_bounds_box_exact : Update our bounds.
//-----------------------------------------------------------------------------
//		Note :	We transform the vertices to world coordinates, then union them
//				with the view bounding box.
//-----------------------------------------------------------------------------
static void
e3view_bounds_box_exact ( E3View* view, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints )
	{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(view) ;
	Q3_ASSERT(numPoints != 0) ;



	// Get the local to world matrix
	const TQ3Matrix4x4* localToWorld = & view->instanceData.viewStack->matrixLocalToWorld ;
	Q3_ASSERT_VALID_PTR(localToWorld);



	// Transform the points, and accumulate them into the bounding box
	const TQ3Uns8* rawPoint = (const TQ3Uns8 *) thePoints ;
	for ( TQ3Uns32 i = 0 ; i < numPoints ; ++i, rawPoint += pointStride )
		{
		// Transform the point
		TQ3Point3D worldPoint ;
		Q3Point3D_Transform ( (const TQ3Point3D *) rawPoint, localToWorld, &worldPoint ) ;
		
		// Union it into the bounding box
		Q3BoundingBox_UnionPoint3D ( & view->instanceData.boundingBox, &worldPoint, & view->instanceData.boundingBox ) ;
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
e3view_bounds_box_approx ( E3View* view, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints )
	{
	TQ3BoundingBox		transformedBounds ;
	TQ3BoundingBox		localBounds ;
	TQ3Point3D			tempPoint ;



	// Validate our parameters and state
	Q3_ASSERT(numPoints                    != 0);
	Q3_ASSERT(view->instanceData.boundingMethod == kQ3BoxBoundsApprox);



	// Get the local to world matrix
	const TQ3Matrix4x4* localToWorld = & view->instanceData.viewStack->matrixLocalToWorld ;
	Q3_ASSERT_VALID_PTR(localToWorld);



	// Calculate a bounding box in local coordinates
	Q3BoundingBox_SetFromPoints3D ( & localBounds, thePoints, numPoints, pointStride ) ;



	// Transform the bounding box to world coordinates
	Q3Point3D_Transform ( & localBounds.min, localToWorld, &tempPoint ) ;
	Q3BoundingBox_Set ( & transformedBounds,&tempPoint,&tempPoint,kQ3False ) ;
		
	Q3Point3D_Set ( & tempPoint, localBounds.min.x,localBounds.min.y,localBounds.max.z ) ;
	Q3Point3D_Transform ( & tempPoint, localToWorld, &tempPoint ) ;
	Q3BoundingBox_UnionPoint3D ( & transformedBounds,&tempPoint,&transformedBounds ) ;
	
	Q3Point3D_Set ( & tempPoint, localBounds.max.x,localBounds.min.y,localBounds.max.z ) ;
	Q3Point3D_Transform ( & tempPoint, localToWorld, &tempPoint ) ;
	Q3BoundingBox_UnionPoint3D ( & transformedBounds,&tempPoint,&transformedBounds ) ;
	
	Q3Point3D_Set ( & tempPoint, localBounds.max.x,localBounds.min.y,localBounds.min.z ) ;
	Q3Point3D_Transform ( & tempPoint, localToWorld, &tempPoint ) ;
	Q3BoundingBox_UnionPoint3D ( & transformedBounds,&tempPoint,&transformedBounds ) ;
	
	Q3Point3D_Transform ( & localBounds.max, localToWorld, &tempPoint ) ;
	Q3BoundingBox_UnionPoint3D ( & transformedBounds,&tempPoint,&transformedBounds ) ;
	
	Q3Point3D_Set ( & tempPoint, localBounds.max.x,localBounds.max.y,localBounds.min.z ) ;
	Q3Point3D_Transform ( & tempPoint, localToWorld, &tempPoint ) ;
	Q3BoundingBox_UnionPoint3D ( & transformedBounds,&tempPoint,&transformedBounds ) ;
	
	Q3Point3D_Set ( & tempPoint, localBounds.min.x,localBounds.max.y,localBounds.min.z ) ;
	Q3Point3D_Transform ( & tempPoint, localToWorld, &tempPoint ) ;
	Q3BoundingBox_UnionPoint3D ( & transformedBounds,&tempPoint,&transformedBounds ) ;
	
	Q3Point3D_Set ( & tempPoint, localBounds.min.x,localBounds.max.y,localBounds.max.z ) ;
	Q3Point3D_Transform ( & tempPoint, localToWorld, &tempPoint ) ;
	Q3BoundingBox_UnionPoint3D ( & transformedBounds,&tempPoint,&transformedBounds ) ;
	
	
	

	// Accumulate the bounding box
	Q3BoundingBox_Union ( & transformedBounds, & view->instanceData.boundingBox, & view->instanceData.boundingBox ) ;
	}





//=============================================================================
//      e3view_bounds_sphere_exact : Update our bounds.
//-----------------------------------------------------------------------------
//		Note :	We transform the vertices to world coordinates, then union them
//				with the view bounding sphere.
//-----------------------------------------------------------------------------
static void
e3view_bounds_sphere_exact ( E3View* view, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints )
	{
	// Validate our parameters and state
	Q3_ASSERT(numPoints                    != 0);
//	Q3_ASSERT(view->instanceData.boundingMethod == kQ3SphereBoundsExact);



	// Get the local to world matrix
	const TQ3Matrix4x4* localToWorld = & view->instanceData.viewStack->matrixLocalToWorld ;
	Q3_ASSERT_VALID_PTR(localToWorld);

	if ( view->instanceData.boundingPointsSlab != NULL )
		{	

		// Transform the points to world coordinates, and store them in the slab
		TQ3Point3D* worldPoints = (TQ3Point3D *) Q3SlabMemory_AppendData ( view->instanceData.boundingPointsSlab, numPoints, NULL ) ;
		if ( worldPoints == NULL )
			return ;

		Q3Point3D_To3DTransformArray ( thePoints, localToWorld, worldPoints,
								  numPoints, pointStride, sizeof ( TQ3Point3D ) ) ;
		}

	}





//=============================================================================
//      e3view_bounds_sphere_approx : Update our bounds.
//-----------------------------------------------------------------------------
//		Note :	We calculate a bounding sphere in local coordinates, transform
//				the bounding sphere to world coordinates, then union this with
//				the view bounding sphere.
//-----------------------------------------------------------------------------
static void
e3view_bounds_sphere_approx ( E3View* view, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints )
	{
	TQ3SphericalPoint		radSpherical;
	TQ3BoundingSphere		localBounds;
	TQ3Point3D				radPoint;



	// Validate our parameters and state
	Q3_ASSERT(numPoints                    != 0);
	Q3_ASSERT(view->instanceData.boundingMethod == kQ3SphereBoundsApprox);



	// Get the local to world matrix
	const TQ3Matrix4x4* localToWorld = & view->instanceData.viewStack->matrixLocalToWorld ;
	Q3_ASSERT_VALID_PTR(localToWorld);



	// Calculate a bounding sphere in local coordinates
	Q3BoundingSphere_SetFromPoints3D ( & localBounds, thePoints, numPoints, pointStride ) ;



	// Transform the bounding sphere origin to world coordinates
	Q3Point3D_Transform ( & localBounds.origin, localToWorld, & localBounds.origin ) ;



	// Transform the bounding sphere radius to world coordinate
	Q3SphericalPoint_Set ( & radSpherical, localBounds.radius, 0.0f, 0.0f ) ;

	Q3SphericalPoint_ToPoint3D ( & radSpherical, & radPoint ) ;
	Q3Point3D_Transform ( & radPoint, localToWorld, & radPoint ) ;
	Q3Point3D_ToSpherical ( & radPoint, & radSpherical ) ;

	localBounds.radius = radSpherical.rho ;



	// Accumulate the bounding sphere
	Q3BoundingSphere_Union ( & localBounds, & view->instanceData.boundingSphere, & view->instanceData.boundingSphere ) ;
	}





//=============================================================================
//      e3view_submit_retained_error : viewState != kQ3ViewStateSubmitting.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_retained_error ( E3View* view, TQ3Object theObject )
	{
	TQ3Error theError ;
	
 	// We get called is app writer tries to submit when viewState != kQ3ViewStateSubmitting
 	// Error, decide which error to post
	switch ( view->instanceData.viewMode )
		{
		case kQ3ViewModeDrawing:
			theError = kQ3ErrorRenderingNotStarted ;
			break;
		case kQ3ViewModePicking:
			theError = kQ3ErrorPickingNotStarted ;
			break;
		case kQ3ViewModeWriting:
			theError = kQ3ErrorWriteStateInactive ;
			break;
		case kQ3ViewModeCalcBounds:
			theError = kQ3ErrorBoundsNotStarted ;
			break;
		default:
			theError = kQ3ErrorViewNotStarted ;
			Q3_ASSERT(!"Unrecognised view mode");
			E3ErrorManager_PostError ( kQ3ErrorUnsupportedFunctionality, kQ3False ) ;
			return kQ3Failure ;
			break;
		}

	E3ErrorManager_PostError ( theError, kQ3False ) ;
	return kQ3Failure ;
	}





//=============================================================================
//      e3view_submit_retained_pick : viewMode == kQ3ViewModePicking.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_retained_pick ( E3View* view, TQ3Object theObject )
	{
	E3Root* theClass = (E3Root*) theObject->GetClass () ;

	// Call a presubmit callback, if appropriate
	TQ3ObjectEventCallback eventCallback ;
	if ( Q3Object_GetElement ( view, kQ3CallbackElementTypeBeforePick, &eventCallback ) != kQ3Failure )
		if ( eventCallback ( theObject, kQ3CallbackElementTypeBeforePick, view ) == kQ3Failure )
			return kQ3Failure ;

	// Update the current hit target. We only do this if we are not
	// within a decomposed object, as we want to track the object submitted by the
	// application and not any sub-objects which are submitted to calculate the pick
	// for that top-level object.
	if ( view->instanceData.pickDecomposeCount == 0 )
		E3View_PickStack_SaveObject ( view, theObject ) ;
		
	// Call the method
	TQ3Status qd3dStatus = kQ3Success ;
	if ( theClass->submitPickMethod != NULL )
		qd3dStatus = theClass->submitPickMethod ( view, theClass->GetType (), theObject, theObject->FindLeafInstanceData () ) ;


	// Reset the current hit target. Not strictly necessary (since we
	// will release our reference on the next object or at the end of the picking loop)
	// but this helps keep our internal state easily debuggable (the tracked object
	// should always be NULL except when a pick submit method is invoked).
	if ( view->instanceData.pickDecomposeCount == 0 )
		E3View_PickStack_SaveObject ( view, NULL ) ;


	// Call a postsubmit callback, if appropriate
	if ( ( qd3dStatus != kQ3Failure )
	&&	( Q3Object_GetElement ( view, kQ3CallbackElementTypeAfterPick, &eventCallback ) != kQ3Failure ) )
		{
		(void) eventCallback ( theObject, kQ3CallbackElementTypeAfterPick, view ) ;
		}
	
	return qd3dStatus ;
	}





//=============================================================================
//      e3view_submit_retained_write : viewMode == kQ3ViewModeWriting.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_retained_write ( E3View* theView, TQ3Object theObject)
	{
	E3Root* theClass = (E3Root*) theObject->GetClass () ;



	// Call the method
	if ( theClass->submitWriteMethod == NULL )
		return kQ3Success ;
		
	return theClass->submitWriteMethod ( theView, theClass->GetType (), theObject, theObject->FindLeafInstanceData () ) ;
	}





//=============================================================================
//      e3view_submit_retained_bounds : viewMode == kQ3ViewModeCalcBounds.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_retained_bounds ( E3View* theView, TQ3Object theObject )
	{
	E3Root* theClass = (E3Root*) theObject->GetClass () ;



	// Call the method
	if ( theClass->submitBoundsMethod == NULL )
		return kQ3Success ;
	
	return theClass->submitBoundsMethod ( theView, theClass->GetType (), theObject, theObject->FindLeafInstanceData () ) ;
	}





//=============================================================================
//      e3view_submit_retained_bad_mode : viewMode is none of the above.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_retained_bad_mode ( E3View* theView, TQ3Object theObject )
{
	Q3_ASSERT(!"Unrecognised view mode");
	E3ErrorManager_PostError(kQ3ErrorUnsupportedFunctionality, kQ3False);
	return kQ3Failure ;
}





//=============================================================================
//      e3view_submit_retained_render : viewMode == kQ3ViewModeDrawing.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_retained_render ( E3View* theView, TQ3Object theObject)
	{
	TQ3Status qd3dStatus = kQ3Success ;
	E3Root* theClass = (E3Root*) theObject->GetClass () ;



	// Invoke the pre-render callback
	TQ3ObjectEventCallback	eventCallback ;
	if ( Q3Object_GetElement ( theView, kQ3CallbackElementTypeBeforeRender, &eventCallback) != kQ3Failure )
		qd3dStatus = eventCallback ( theObject, kQ3CallbackElementTypeBeforeRender, theView ) ;



	// Submit the object
	if (qd3dStatus != kQ3Failure && theClass->submitRenderMethod != NULL)
		qd3dStatus = theClass->submitRenderMethod ( theView, theClass->GetType (), theObject, theObject->FindLeafInstanceData () ) ;



	// Invoke the post-render callback
	if (qd3dStatus != kQ3Failure && Q3Object_GetElement ( theView, kQ3CallbackElementTypeAfterRender, &eventCallback ) != kQ3Failure )
		qd3dStatus = eventCallback(theObject, kQ3CallbackElementTypeAfterRender, theView ) ;

	return qd3dStatus ;
	}








//=============================================================================
//      e3view_submit_immediate_error : viewState != kQ3ViewStateSubmitting.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_immediate_error ( E3View* view , TQ3ObjectType objectType , const void* objectData )
	{
	TQ3Error theError ;
	
 	// We get called is app writer tries to submit when viewState != kQ3ViewStateSubmitting
 	// Error, decide which error to post
	switch ( view->instanceData.viewMode )
		{
		case kQ3ViewModeDrawing:
			theError = kQ3ErrorRenderingNotStarted ;
			break;
		case kQ3ViewModePicking:
			theError = kQ3ErrorPickingNotStarted ;
			break;
		case kQ3ViewModeWriting:
			theError = kQ3ErrorWriteStateInactive ;
			break;
		case kQ3ViewModeCalcBounds:
			theError = kQ3ErrorBoundsNotStarted ;
			break;
		default:
			theError = kQ3ErrorViewNotStarted ;
			break;
		}

	E3ErrorManager_PostError ( theError, kQ3False ) ;
	return kQ3Failure ;
	}





//=============================================================================
//      e3view_submit_immediate_render : viewMode == kQ3ViewModeDrawing.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_immediate_render ( E3View* theView , TQ3ObjectType objectType , const void* objectData )
	{
	// Find the object class
	E3Root* theClass = (E3Root*) E3ClassTree::GetClass ( objectType ) ;
	if ( theClass == NULL )
		{
		E3ErrorManager_PostError ( kQ3ErrorInvalidObjectClass, kQ3False ) ;
		return kQ3Failure ;
		}


	// Call the rendering submit method unless it is NULL
	if ( theClass->submitRenderMethod == NULL )
		return kQ3Success ;
		
	return theClass->submitRenderMethod ( theView, objectType, NULL, objectData ) ;
	}





//=============================================================================
//      e3view_submit_immediate_pick : viewMode == kQ3ViewModePicking.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_immediate_pick ( E3View* view , TQ3ObjectType objectType , const void* objectData )
	{
	// Find the object class
	E3Root* theClass = (E3Root*) E3ClassTree::GetClass ( objectType ) ;
	if ( theClass == NULL )
		{
		E3ErrorManager_PostError ( kQ3ErrorInvalidObjectClass, kQ3False ) ;
		return kQ3Failure ;
		}



	// Update the current hit target. We only do this if we are not
	// within a decomposed object, as we want to track the object submitted by the
	// application and not any sub-objects which are submitted to calculate the pick
	// for that top-level object.
	if ( view->instanceData.pickDecomposeCount == 0 )
		E3View_PickStack_SaveObject ( view, NULL ) ;

	// Call the method
	TQ3Status qd3dStatus ;
	if ( theClass->submitPickMethod != NULL )
		qd3dStatus = theClass->submitPickMethod ( view, objectType, NULL, objectData ) ;
	else
		qd3dStatus = kQ3Success ;
		
	// Reset the current hit target. Not strictly necessary (since we
	// will release our reference on the next object or at the end of the picking loop)
	// but this helps keep our internal state easily debuggable (the tracked object
	// should always be NULL except when a pick submit method is invoked).
	if ( view->instanceData.pickDecomposeCount == 0 )
		E3View_PickStack_SaveObject ( view, NULL ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      e3view_submit_immediate_write : viewMode == kQ3ViewModeWriting.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_immediate_write ( E3View* theView , TQ3ObjectType objectType , const void* objectData )
	{
	// Find the object class
	E3Root* theClass = (E3Root*) E3ClassTree::GetClass ( objectType ) ;
	if ( theClass == NULL )
		{
		E3ErrorManager_PostError ( kQ3ErrorInvalidObjectClass, kQ3False ) ;
		return kQ3Failure ;
		}


	// Call the method
	if ( theClass->submitWriteMethod == NULL )
		return kQ3Success ;
	
	return theClass->submitWriteMethod ( theView, objectType, NULL, objectData ) ;
	}





//=============================================================================
//      e3view_submit_immediate_bounds : viewMode == kQ3ViewModeCalcBounds.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_immediate_bounds ( E3View* theView , TQ3ObjectType objectType , const void* objectData )
	{
	// Find the object class
	E3Root* theClass = (E3Root*) E3ClassTree::GetClass ( objectType ) ;
	if ( theClass == NULL )
		{
		E3ErrorManager_PostError ( kQ3ErrorInvalidObjectClass, kQ3False ) ;
		return kQ3Failure ;
		}



	// Call the method
	if ( theClass->submitBoundsMethod == NULL )
		return kQ3Success ;
	
	return theClass->submitBoundsMethod ( theView, objectType, NULL, objectData ) ;
	}





//=============================================================================
//      e3view_submit_immediate_bad_mode : viewMode is none of the above.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_immediate_bad_mode ( E3View* theView , TQ3ObjectType objectType , const void* objectData )
{
	Q3_ASSERT(!"Unrecognised view mode");
	E3ErrorManager_PostError(kQ3ErrorUnsupportedFunctionality, kQ3False);
	return kQ3Failure ;
}





//=============================================================================
//      e3view_submit_begin : Begin a submitting loop.
//-----------------------------------------------------------------------------
static TQ3Status
e3view_submit_begin ( E3View* view, TQ3ViewMode viewMode )
	{
	TQ3Status qd3dStatus = kQ3Success ;



	// Set up the view state
	if ( view->instanceData.viewState == kQ3ViewStateInactive )
		{
		// Initialise the state
		view->instanceData.viewMode  = viewMode ;
		view->instanceData.viewState = kQ3ViewStateSubmitting ;
		view->instanceData.viewPass  = 1 ;
		
		// Find the appropriate submit method
		switch ( viewMode )
			{
			case kQ3ViewModeDrawing:
				view->instanceData.submitRetainedMethod  = (TQ3XViewSubmitRetainedMethod)
					view->GetMethod( kQ3XMethodTypeViewSubmitRetainedRender );
				view->instanceData.submitImmediateMethod = (TQ3XViewSubmitImmediateMethod)
					view->GetMethod( kQ3XMethodTypeViewSubmitImmediateRender );
				break;

			case kQ3ViewModePicking:
				view->instanceData.submitRetainedMethod  = (TQ3XViewSubmitRetainedMethod)
					view->GetMethod( kQ3XMethodTypeViewSubmitRetainedPick );
				view->instanceData.submitImmediateMethod = (TQ3XViewSubmitImmediateMethod)
					view->GetMethod( kQ3XMethodTypeViewSubmitImmediatePick );
				break;

			case kQ3ViewModeWriting:
				view->instanceData.submitRetainedMethod  = (TQ3XViewSubmitRetainedMethod)
					view->GetMethod( kQ3XMethodTypeViewSubmitRetainedWrite );
				view->instanceData.submitImmediateMethod = (TQ3XViewSubmitImmediateMethod)
					view->GetMethod( kQ3XMethodTypeViewSubmitImmediateWrite );
				break;

			case kQ3ViewModeCalcBounds:
				view->instanceData.submitRetainedMethod  = (TQ3XViewSubmitRetainedMethod)
					view->GetMethod( kQ3XMethodTypeViewSubmitRetainedBound );
				view->instanceData.submitImmediateMethod = (TQ3XViewSubmitImmediateMethod)
					view->GetMethod( kQ3XMethodTypeViewSubmitImmediateBound );
				break;

			default:
				view->instanceData.submitRetainedMethod  = (TQ3XViewSubmitRetainedMethod)
					e3view_submit_retained_bad_mode ;
				view->instanceData.submitImmediateMethod = (TQ3XViewSubmitImmediateMethod)
					e3view_submit_immediate_bad_mode ;
				break;
			}

		}
	else
	if ( view->instanceData.viewState == kQ3ViewStateSubmitting )
		{
		// Make sure our state is valid
		Q3_ASSERT(view->instanceData.viewMode != kQ3ViewModeInactive);
		Q3_ASSERT(view->instanceData.viewPass > 1);
		}
	else
		qd3dStatus = kQ3Failure ;



	// Push the first entry on the view stack
	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = e3view_stack_push ( view ) ;



	// Handle failure
	if ( qd3dStatus == kQ3Failure )
		{
		view->instanceData.viewMode              = kQ3ViewModeInactive ;
		view->instanceData.viewState             = kQ3ViewStateInactive ;
		view->instanceData.viewPass              = 0 ;
		view->instanceData.submitRetainedMethod  = (TQ3XViewSubmitRetainedMethod) e3view_submit_retained_error ;
		view->instanceData.submitImmediateMethod = (TQ3XViewSubmitImmediateMethod) e3view_submit_immediate_error ;
		e3view_stack_pop_clean ( view ) ;
		}

	return qd3dStatus ;
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
e3view_submit_initial_state ( E3View* view)
	{
	// Submit the initial state stack
	TQ3Status qd3dStatus = e3view_stack_update ( view, kQ3ViewStateAll ) ;



	// Submit the view attribute state, if any
	if ( view->instanceData.viewAttributes != NULL && qd3dStatus != kQ3Failure )
		qd3dStatus = Q3AttributeSet_Submit ( view->instanceData.viewAttributes, view ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      e3view_submit_end : End a submit loop.
//-----------------------------------------------------------------------------
//		Note :	If the current view state is within a submitting a loop, the
//				submitStatus value is used as the result of the submit loop.
//-----------------------------------------------------------------------------
static TQ3ViewStatus
e3view_submit_end ( E3View* view, TQ3ViewStatus submitStatus )
	{
	TQ3ViewStatus		viewStatus;


	// End the pass
	if ( view->instanceData.viewState == kQ3ViewStateCancelled )
		viewStatus = kQ3ViewStatusCancelled ;
	else
	if ( view->instanceData.viewState == kQ3ViewStateSubmitting )
		viewStatus = submitStatus ;
	else
		viewStatus = kQ3ViewStatusDone ;



	// Pop the view stack
	e3view_stack_pop_clean ( view ) ;



	// Handle re-traversal
	if ( viewStatus == kQ3ViewStatusRetraverse )
		{
		// Restart the submit
		view->instanceData.viewState = kQ3ViewStateSubmitting ; 
		view->instanceData.viewPass++ ;
		TQ3Status qd3dStatus = e3view_submit_begin ( view, view->instanceData.viewMode )  ;


		// Invoke the start pass method		
		if ( qd3dStatus != kQ3Failure )
			{
			view->instanceData.rendererFinishedFrame = kQ3False ;

			if ( view->instanceData.viewMode == kQ3ViewModeDrawing)
				qd3dStatus = E3Renderer_Method_StartPass ( view, view->instanceData.theCamera, view->instanceData.theLights ) ;
			else
			if ( view->instanceData.viewMode == kQ3ViewModeWriting )
				qd3dStatus = E3FileFormat_Method_StartPass ( view ) ;
			}


		// Submit the initial state
		if ( qd3dStatus != kQ3Failure )
			qd3dStatus = e3view_submit_initial_state ( view )  ;


		// If something went wrong, stop
		if ( qd3dStatus == kQ3Failure )
			viewStatus = kQ3ViewStatusError ;
		}
	else
		{
		view->instanceData.viewState             = kQ3ViewStateInactive ;
		view->instanceData.viewPass              = 0 ;
		view->instanceData.submitRetainedMethod  = (TQ3XViewSubmitRetainedMethod) e3view_submit_retained_error ;
		view->instanceData.submitImmediateMethod = (TQ3XViewSubmitImmediateMethod) e3view_submit_immediate_error ;
		}



	// Return the view status
	return viewStatus ;
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
e3view_pick_begin ( E3View* view, TQ3PickObject thePick )
	{
	TQ3Matrix4x4			worldToFrustum, frustumToWindow, worldToWindow, windowToWorld;
	TQ3Point3D				worldPoint, windowPoint3D;
	TQ3CameraPlacement		thePlacement;
	TQ3Point2D				windowPoint;
	TQ3Vector3D				theVector;



	// Validate our state
	Q3_ASSERT( view->instanceData.viewMode == kQ3ViewModePicking);



	// Save a reference to the pick object
	Q3_ASSERT( view->instanceData.thePick == NULL);
	view->instanceData.thePick = thePick ;



	// Initialise our state
	Q3Memory_Clear(& view->instanceData.pickedPath, sizeof( view->instanceData.pickedPath)); 
	view->instanceData.pickedObject       = NULL;
	view->instanceData.pickDecomposeCount = 0;



	// If this is a window-point pick, recalculate the pick ray
	Q3Memory_Clear(& view->instanceData.rayThroughPick, sizeof( view->instanceData.rayThroughPick));
	if (Q3Pick_GetType(thePick) == kQ3PickTypeWindowPoint)
		{
		// Get the point that was picked, in window coordinates
		Q3WindowPointPick_GetPoint(thePick, &windowPoint);

		windowPoint3D.x = windowPoint.x;
		windowPoint3D.y = windowPoint.y;
		windowPoint3D.z = 0.0f;
		


		// Transform this point from window to world coordinates
		Q3Camera_GetWorldToFrustum( view->instanceData.theCamera, &worldToFrustum);
		E3View_GetFrustumToWindowMatrixState ( view, &frustumToWindow ) ;
		Q3Matrix4x4_Multiply( &worldToFrustum, &frustumToWindow, &worldToWindow );
		Q3Matrix4x4_Invert(&worldToWindow, &windowToWorld);
		Q3Point3D_Transform(&windowPoint3D,  &windowToWorld, &worldPoint);



		switch( Q3Camera_GetType( view->instanceData.theCamera )	)	
			{
		
			case kQ3CameraTypeViewPlane:
			case kQ3CameraTypeViewAngleAspect:

				// Initialise the ray origin - the camera location
				Q3Camera_GetPlacement( view->instanceData.theCamera, &thePlacement);
				view->instanceData.rayThroughPick.origin = thePlacement.cameraLocation;



				// Initialise the ray - from the pixel back to the camera
				Q3Point3D_Subtract(&worldPoint,  & view->instanceData.rayThroughPick.origin, &theVector);
				Q3Vector3D_Normalize(&theVector, & view->instanceData.rayThroughPick.direction);
				break;
				
			case kQ3CameraTypeOrthographic:
			
				// Set origin of pick to the pick loc in world coordinates
				Q3Camera_GetPlacement( view->instanceData.theCamera, &thePlacement);
				view->instanceData.rayThroughPick.origin = worldPoint;



				// Initialise the ray - from the pixel back to the camera
				Q3Point3D_Subtract(&thePlacement.pointOfInterest, &thePlacement.cameraLocation, &theVector);
				Q3Vector3D_Normalize(&theVector, & view->instanceData.rayThroughPick.direction);
				break;
			}
		}
	}





//=============================================================================
//      e3view_pick_end : End a picking loop.
//-----------------------------------------------------------------------------
static void
e3view_pick_end ( E3View* view )
	{
	// Validate our state
	Q3_ASSERT( view->instanceData.viewMode           == kQ3ViewModePicking ) ;
	Q3_ASSERT( view->instanceData.pickDecomposeCount == 0 ) ;



	// Forget our reference to the pick object (don't dispose of it, as picks aren't shared)
	view->instanceData.thePick = NULL ;



	// Empty the pick path
	Q3HitPath_EmptyData ( & view->instanceData.pickedPath ) ;



	// Dispose of our reference to the picked object
	Q3Object_CleanDispose ( & view->instanceData.pickedObject ) ;
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

	return qd3dStatus ;
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
	instanceData->submitRetainedMethod  = (TQ3XViewSubmitRetainedMethod) e3view_submit_retained_error;
	instanceData->submitImmediateMethod = (TQ3XViewSubmitImmediateMethod) e3view_submit_immediate_error;
	
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

	return kQ3Success ;
}





//=============================================================================
//      e3view_delete : View class delete method.
//-----------------------------------------------------------------------------
static void
e3view_delete ( E3View* view, void *privateData )
	{
	TQ3ViewData		*instanceData = (TQ3ViewData *) privateData ;


	// Dispose of our instance data
	Q3Object_CleanDispose(&instanceData->viewAttributes);
	Q3Object_CleanDispose(&instanceData->theRenderer);
	Q3Object_CleanDispose(&instanceData->theCamera);
	Q3Object_CleanDispose(&instanceData->theLights);
	Q3Object_CleanDispose(&instanceData->theDrawContext);
	Q3Object_CleanDispose(&instanceData->defaultAttributeSet);
	Q3Object_CleanDispose(&instanceData->boundingPointsSlab);

	e3view_stack_pop_clean ( view ) ;
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
		
		case kQ3XMethodTypeViewSubmitRetainedRender:
			theMethod = (TQ3XFunctionPointer) e3view_submit_retained_render;
			break;
		
		case kQ3XMethodTypeViewSubmitRetainedPick:
			theMethod = (TQ3XFunctionPointer) e3view_submit_retained_pick;
			break;
		
		case kQ3XMethodTypeViewSubmitRetainedBound:
			theMethod = (TQ3XFunctionPointer) e3view_submit_retained_bounds;
			break;
		
		case kQ3XMethodTypeViewSubmitRetainedWrite:
			theMethod = (TQ3XFunctionPointer) e3view_submit_retained_write;
			break;
		
		case kQ3XMethodTypeViewSubmitImmediateRender:
			theMethod = (TQ3XFunctionPointer) e3view_submit_immediate_render;
			break;
		
		case kQ3XMethodTypeViewSubmitImmediatePick:
			theMethod = (TQ3XFunctionPointer) e3view_submit_immediate_pick;
			break;
		
		case kQ3XMethodTypeViewSubmitImmediateBound:
			theMethod = (TQ3XFunctionPointer) e3view_submit_immediate_bounds;
			break;
		
		case kQ3XMethodTypeViewSubmitImmediateWrite:
			theMethod = (TQ3XFunctionPointer) e3view_submit_immediate_write;
			break;
		
		}
	
	return(theMethod);
}





//=============================================================================
//      e3push_submit : Push submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3push_submit ( E3View* view, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData )
	{
#pragma unused(objectType)
#pragma unused(theObject)
#pragma unused(objectData)



	// Push the view state
	return e3view_stack_push ( view ) ;
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
e3pop_submit ( E3View* view, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData )
	{
#pragma unused(objectType)
#pragma unused(theObject)
#pragma unused(objectData)



	// If the stack is empty, we can't pop
	if ( view->instanceData.viewStack == NULL )
		return kQ3Failure ;



	// Pop the stack
	e3view_stack_pop ( view ) ;

	return kQ3Success ;
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
	qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameView,
										e3view_metahandler,
										E3View ) ;
	
	if (qd3dStatus != kQ3Failure)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameStateOperator,
											NULL,
											E3StateOperator ) ;
	
	if (qd3dStatus != kQ3Failure)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameStateOperatorPush,
											e3push_metahandler,
											E3Push ) ;
	
	if (qd3dStatus != kQ3Failure)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameStateOperatorPop,
											e3pop_metahandler,
											E3Pop ) ;

	return qd3dStatus ;
}





//=============================================================================
//      E3View_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3View_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3StateOperatorTypePop,   kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3StateOperatorTypePush,  kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ShapeTypeStateOperator, kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeView,         kQ3True);
	return qd3dStatus ;
}





//=============================================================================
//      E3View_SubmitRetained : Submit an object to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SubmitRetained ( TQ3ViewObject theView , TQ3Object theObject )
	{
	return ( (E3View*) theView )->instanceData.submitRetainedMethod ( (E3View*) theView , theObject ) ;
	}





//=============================================================================
//      E3View_SubmitImmediate : Submit an immediate mode object to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SubmitImmediate ( TQ3ViewObject theView , TQ3ObjectType objectType , const void* objectData ) 
	{
	return ( (E3View*) theView )->instanceData.submitImmediateMethod ( (E3View*) theView , objectType , objectData ) ;
	}





//=============================================================================
//      E3View_CallIdleMethod : Call the idle method for a view.
//-----------------------------------------------------------------------------
//		Note :	Calls the view's idle progress method, falling back to the
//				view's idle method if no progress method has been supplied.
//-----------------------------------------------------------------------------
TQ3Status
E3View_CallIdleMethod(TQ3ViewObject theView, TQ3Uns32 current, TQ3Uns32 completed)
	{
	E3View* view = (E3View*) theView ;	


	// Make sure we're in the correct state
	if ( view->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// If we have an idle progress method, call it
	if ( view->instanceData.idleProgressMethod != NULL )
		return view->instanceData.idleProgressMethod ( theView, 
													  view->instanceData.idleProgressData,
													  current,
													  completed ) ;


	// Otherwise, fall back to the progress method
	if ( view->instanceData.idleMethod != NULL )
		return view->instanceData.idleMethod ( theView, view->instanceData.idleData ) ;


	// Otherwise, do nothing
	return kQ3Success;
	}





//=============================================================================
//      E3View_AccessPick : Access our pick without ref-counting.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access a view's pick.
//-----------------------------------------------------------------------------
TQ3PickObject
E3View_AccessPick(TQ3ViewObject theView)
	{
	// Return the pick
	return ( (E3View*) theView )->instanceData.thePick ;
	}





//=============================================================================
//      E3View_AccessRenderer : Access our renderer without ref-counting.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access a view's renderer.
//-----------------------------------------------------------------------------
TQ3RendererObject
E3View_AccessRenderer(TQ3ViewObject theView)
	{
	// Return the renderer
	return ( (E3View*) theView )->instanceData.theRenderer ;
	}





//=============================================================================
//      E3View_AccessDrawContext : Access our drawcontext without ref-counting.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access a view's draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3View_AccessDrawContext(TQ3ViewObject theView)
	{
	// Return the draw context
	return ( (E3View*) theView )->instanceData.theDrawContext ;
	}





//=============================================================================
//      E3View_AccessFile : Access the current file.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access a view's current file.
//-----------------------------------------------------------------------------
TQ3FileObject
E3View_AccessFile(TQ3ViewObject theView)
	{
	// Return the file
	return ( (E3View*) theView )->instanceData.theFile ;
	}





//=============================================================================
//      E3View_AccessFileFormat : Get the format of the current file.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access the current file's format.
//-----------------------------------------------------------------------------
TQ3FileFormatObject
E3View_AccessFileFormat(TQ3ViewObject theView)
	{
	// Return the file Format
	if ( ( (E3View*) theView )->instanceData.theFile != NULL )
		return Q3File_GetFileFormat ( ( (E3View*) theView )->instanceData.theFile ) ;

	return NULL ;
	}





//=============================================================================
//      E3View_AccessCamera : Access our camera without ref-counting.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access a view's camera.
//-----------------------------------------------------------------------------
TQ3CameraObject
E3View_AccessCamera(TQ3ViewObject theView)
	{
	// Return the camera
	return ( (E3View*) theView )->instanceData.theCamera ;
	}





//=============================================================================
//      E3View_EndFrame : An async renderer has finished.
//-----------------------------------------------------------------------------
//		Note : Called by E3XView_EndFrame when an async renderer has finished.
//-----------------------------------------------------------------------------
void
E3View_EndFrame(TQ3ViewObject theView)
	{
	// Set our flag
	( (E3View*) theView )->instanceData.rendererFinishedFrame = kQ3True ;
	}





//=============================================================================
//      E3View_GetViewMode : Return the current mode of a view.
//-----------------------------------------------------------------------------
//		Note : Used internally by Quesa to access a view's mode.
//-----------------------------------------------------------------------------
TQ3ViewMode
E3View_GetViewMode(TQ3ViewObject theView)
	{
	// Validate our state
	Q3_ASSERT( ( (E3View*) theView )->instanceData.viewState == kQ3ViewStateSubmitting ) ;



	// Return the current mode
	return ( (E3View*) theView )->instanceData.viewMode ;
	}





//=============================================================================
//      E3View_GetViewState : Return the current state of a view.
//-----------------------------------------------------------------------------
//		Note :	Used internally by Quesa to access a view's state. May only be
//				called from within a submitting loop.
//-----------------------------------------------------------------------------
TQ3ViewState
E3View_GetViewState(TQ3ViewObject theView)
	{
	// Validate our state
	Q3_ASSERT( ( (E3View*) theView )->instanceData.viewState == kQ3ViewStateSubmitting ) ;



	// Return the current state
	return ( (E3View*) theView )->instanceData.viewState ;
	}





//=============================================================================
//      E3View_GetBoundingMethod : Return the current bounding method.
//-----------------------------------------------------------------------------
//		Note :	Used internally by Quesa to access a view's bounding method.
//				May only be called from within a bounding loop.
//-----------------------------------------------------------------------------
TQ3BoundingMethod
E3View_GetBoundingMethod(TQ3ViewObject theView)
	{
	// Validate our state
	Q3_ASSERT( ( (E3View*) theView )->instanceData.viewMode == kQ3ViewModeCalcBounds ); 



	// Return the current state
	return( (E3View*) theView )->instanceData.boundingMethod ;
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
	{
	// Validate our state
	Q3_ASSERT( ( (E3View*) theView )->instanceData.viewMode                == kQ3ViewModePicking ) ;
	Q3_ASSERT( Q3Pick_GetType ( ( (E3View*) theView )->instanceData.thePick ) == kQ3PickTypeWindowPoint ) ;



	// Return the pick ray
	*theRay = ( (E3View*) theView )->instanceData.rayThroughPick ;
	}





//=============================================================================
//      E3View_UpdateBounds : Incorporate vertices into the bounds.
//-----------------------------------------------------------------------------
//		Note :	Used by geometry classes to accumulate their vertices into the
//				current bounding box/sphere.
//-----------------------------------------------------------------------------
void
E3View_UpdateBounds(TQ3ViewObject theView, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints)
	{
	// Validate our parameters and state
	Q3_ASSERT(numPoints               != 0);
	Q3_ASSERT( ( (E3View*) theView )->instanceData.viewState == kQ3ViewStateSubmitting ) ;



	// Update the bounds
	switch ( ( (E3View*) theView )->instanceData.boundingMethod )
		{
		case kQ3BoxBoundsExact:
			e3view_bounds_box_exact( (E3View*) theView, numPoints, pointStride, thePoints);
			break;

		case kQ3BoxBoundsApprox:
			if (numPoints > kApproxBoundsThreshold)
				e3view_bounds_box_approx ( (E3View*) theView, numPoints, pointStride, thePoints ) ;
			else
				e3view_bounds_box_exact ( (E3View*) theView, numPoints, pointStride, thePoints ) ;
			break;

		case kQ3SphereBoundsExact:
			e3view_bounds_sphere_exact ( (E3View*) theView, numPoints, pointStride, thePoints ) ;
			break;

		case kQ3SphereBoundsApprox:
			e3view_bounds_sphere_exact ( (E3View*) theView, numPoints, pointStride, thePoints ) ;
//			e3view_bounds_sphere_approx(theView, numPoints, pointStride, thePoints);
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
	{
	TQ3HitPath* pickedPath = & ( (E3View*) theView )->instanceData.pickedPath ;



	// Validate our state
	Q3_ASSERT( ( (E3View*) theView )->instanceData.viewMode == kQ3ViewModePicking ) ;



	// If we are inside a decomposed object, never mind
	if ( ( (E3View*) theView )->instanceData.pickDecomposeCount > 0 )
		return kQ3Success ;



	// If we don't have a root group yet, this is a top-level group.
	// So save this group as the root group in the pick path.
	if ( pickedPath->rootGroup == NULL )
		{
		// Validate our state - the path should be empty
		Q3_ASSERT ( pickedPath->rootGroup == NULL ) ;
		Q3_ASSERT ( pickedPath->depth     == 0 ) ;
		
		
		// Save the group as the current root
		pickedPath->rootGroup = Q3Shared_GetReference ( theGroup ) ;
		}



	// Allocate another slot in the positions array for any objects within this group.
	// As each object is submitted the group will call us back to save the current
	// position, so that any objects that are intersected by the current pick will
	// be able to retrieve their submit path.
	//
	// Validate our state - we should have a root
	Q3_ASSERT ( pickedPath->rootGroup != NULL ) ;



	// Expand the positions array
	TQ3Uns32 theSize = sizeof ( TQ3GroupPosition ) * (pickedPath->depth + 1 ) ;
	TQ3Status qd3dStatus = Q3Memory_Reallocate(&pickedPath->positions, theSize ) ;
	if ( qd3dStatus != kQ3Failure )
		pickedPath->depth++ ;



	// Save a NULL position to initialise the slot
	pickedPath->positions [ pickedPath->depth - 1 ] = NULL ;
	
	return qd3dStatus ;
	}





//=============================================================================
//      E3View_PickStack_GetPickedPath : Return the path to the picked object.
//-----------------------------------------------------------------------------
TQ3HitPath *
E3View_PickStack_GetPickedPath(TQ3ViewObject theView)
	{
	// Validate our state
	Q3_ASSERT(( (E3View*) theView )->instanceData.viewMode == kQ3ViewModePicking ) ;



	// Return the current pick path
	return & ( (E3View*) theView )->instanceData.pickedPath ;
	}





//=============================================================================
//      E3View_PickStack_GetPickedObject : Return the picked object.
//-----------------------------------------------------------------------------
TQ3Object
E3View_PickStack_GetPickedObject(TQ3ViewObject theView)
	{
	// Validate our state
	Q3_ASSERT ( ( (E3View*) theView )->instanceData.viewMode == kQ3ViewModePicking ) ;



	// Return a new reference to the picked object
	if ( ( (E3View*) theView )->instanceData.pickedObject != NULL )
		return Q3Shared_GetReference ( ( (E3View*) theView )->instanceData.pickedObject ) ;
	
	return NULL ;
	}





//=============================================================================
//      E3View_PickStack_SavePosition : Save the current pick position.
//-----------------------------------------------------------------------------
void
E3View_PickStack_SavePosition(TQ3ViewObject theView, TQ3GroupPosition thePosition)
	{
	TQ3HitPath* pickedPath   = & ( (E3View*) theView )->instanceData.pickedPath ;



	// Validate our state
	Q3_ASSERT ( ( (E3View*) theView )->instanceData.viewMode == kQ3ViewModePicking ) ;



	// If we are inside a decomposed object, never mind
	if ( ( (E3View*) theView )->instanceData.pickDecomposeCount > 0 )
		return ;



	// Make sure that there's at least one slot defined
	Q3_ASSERT ( pickedPath->depth     != 0 ) ;
	Q3_ASSERT ( pickedPath->positions != NULL ) ;



	// Save the position in the last slot in the positions array. We can
	// override any previous value, since group positions are not objects.
	pickedPath->positions [ pickedPath->depth - 1 ] = thePosition ;
	}





//=============================================================================
//      E3View_PickStack_SaveObject : Save the current pick object.
//-----------------------------------------------------------------------------
void
E3View_PickStack_SaveObject(TQ3ViewObject theView, TQ3Object theObject)
	{
	// Validate our state
	Q3_ASSERT ( ( (E3View*) theView )->instanceData.viewMode == kQ3ViewModePicking ) ;
	Q3_ASSERT ( ( (E3View*) theView )->instanceData.pickDecomposeCount == 0 ) ;



	// Save a new reference to the picked object
	E3Shared_Replace ( & ( (E3View*) theView )->instanceData.pickedObject, theObject ) ;
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
	{
	// Validate our state
	Q3_ASSERT( ( (E3View*) theView )->instanceData.viewMode == kQ3ViewModePicking);



	// Increment the count
	( (E3View*) theView )->instanceData.pickDecomposeCount++ ;
	Q3_ASSERT(( (E3View*) theView )->instanceData.pickDecomposeCount != 0);
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
	{
	// Validate our state
	Q3_ASSERT ( ( (E3View*) theView )->instanceData.viewMode == kQ3ViewModePicking ) ;



	// Increment the count
	Q3_ASSERT ( ( (E3View*) theView )->instanceData.pickDecomposeCount > 0 ) ;
	( (E3View*) theView )->instanceData.pickDecomposeCount-- ;
	}





//=============================================================================
//      E3View_PickStack_PopGroup : Pop a group off the pick path.
//-----------------------------------------------------------------------------
void
E3View_PickStack_PopGroup(TQ3ViewObject theView)
	{
	TQ3HitPath* pickedPath = &( (E3View*) theView )->instanceData.pickedPath ;



	// Validate our state
	Q3_ASSERT ( ( (E3View*) theView )->instanceData.viewMode == kQ3ViewModePicking ) ;



	// If we are inside a decomposed object, never mind
	if ( ( (E3View*) theView )->instanceData.pickDecomposeCount > 0 )
		return;



	// Make sure that there's at least one slot defined
	Q3_ASSERT ( pickedPath->depth     != 0 ) ;
	Q3_ASSERT ( pickedPath->positions != NULL ) ;



	// Shrink the positions array. Note that we just decrement the count
	// rather than shrinking the array, since future groups would cause
	// the array to be resized - which realloc will skip if the array
	// has already been extended.
	pickedPath->depth-- ;



	// If this was the last group, remove the root object
	if ( pickedPath->depth == 0 )
		Q3Object_CleanDispose ( & pickedPath->rootGroup ) ;



	// Remove our reference to the picked object
	E3Shared_Replace ( & ( (E3View*) theView )->instanceData.pickedObject, NULL ) ;
	}





//=============================================================================
//      E3View_State_AddMatrixLocalToWorld : Add to the local-to-world matrix.
//-----------------------------------------------------------------------------
TQ3Status
E3View_State_AddMatrixLocalToWorld(TQ3ViewObject theView, const TQ3Matrix4x4 *theMatrix)
	{
	TQ3Matrix4x4	tmpMatrix;



	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR( ( (E3View*) theView )->instanceData.viewStack));



	// No-op if this is an identity matrix
	if ( E3Matrix4x4_IsIdentity ( theMatrix ) )
		return kQ3Success ;



	// Accumulate the local to world transform
	Q3Matrix4x4_Multiply ( theMatrix, E3View_State_GetMatrixLocalToWorld(theView), &tmpMatrix ) ;

	return E3View_State_SetMatrix ( theView, kQ3MatrixStateLocalToWorld, &tmpMatrix, NULL, NULL ) ;
	}





//=============================================================================
//      E3View_State_GetMatrixLocalToWorld : Get the local-to-world matrix.
//-----------------------------------------------------------------------------
const TQ3Matrix4x4 *
E3View_State_GetMatrixLocalToWorld(TQ3ViewObject theView)
	{
	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Return the state
	return & ( (E3View*) theView )->instanceData.viewStack->matrixLocalToWorld ;
	}





//=============================================================================
//      E3View_State_GetStyleSubdivision : Get the subdivision style state.
//-----------------------------------------------------------------------------
//		Note :	The subdivision style pushed onto the stack is normalised, so
//				callers to this routine can assume that unused fields are 0.
//-----------------------------------------------------------------------------
const TQ3SubdivisionStyleData *
E3View_State_GetStyleSubdivision(TQ3ViewObject theView)
	{
	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(( (E3View*) theView )->instanceData.viewStack));



	// Return the state
	return & ( (E3View*) theView )->instanceData.viewStack->styleSubdivision ;
	}





//=============================================================================
//      E3View_State_GetStyleOrientation : Get the orientation style state.
//-----------------------------------------------------------------------------
TQ3OrientationStyle
E3View_State_GetStyleOrientation(TQ3ViewObject theView)
	{
	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(( (E3View*) theView )->instanceData.viewStack));



	// Return the state
	return ( (E3View*) theView )->instanceData.viewStack->styleOrientation ;
	}





//=============================================================================
//      E3View_State_SetMatrix : Set the camera matrices.
//-----------------------------------------------------------------------------
TQ3Status
E3View_State_SetMatrix(TQ3ViewObject			theView,
							TQ3MatrixState		theState,
							const TQ3Matrix4x4	*localToWorld,
							const TQ3Matrix4x4	*worldToCamera,
							const TQ3Matrix4x4	*cameraToFrustum)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the matrices which have changed
	TQ3ViewStackState stateChange = kQ3ViewStateNone;
	
	if (theState & kQ3MatrixStateLocalToWorld)
		{
		Q3_ASSERT(Q3_VALID_PTR(localToWorld));
		stateChange                                 |= kQ3ViewStateMatrixLocalToWorld;
		( (E3View*) theView )->instanceData.viewStack->matrixLocalToWorld = *localToWorld;
		}
	
	if (theState & kQ3MatrixStateWorldToCamera)
		{
		Q3_ASSERT(Q3_VALID_PTR(worldToCamera));
		stateChange                                  |= kQ3ViewStateMatrixWorldToCamera;
		Q3_ASSERT( isfinite( worldToCamera->value[0][0] ) );
		( (E3View*) theView )->instanceData.viewStack->matrixWorldToCamera = *worldToCamera;
		}
	
	if (theState & kQ3MatrixStateCameraToFrustum)
		{
		Q3_ASSERT(Q3_VALID_PTR(cameraToFrustum));
		stateChange                                    |= kQ3ViewStateMatrixCameraToFrustum;
		( (E3View*) theView )->instanceData.viewStack->matrixCameraToFrustum = *cameraToFrustum;
		}



	// Update the renderer
	Q3_ASSERT(stateChange != kQ3ViewStateNone);
	return e3view_stack_update ( (E3View*) theView, stateChange) ;
	}





//=============================================================================
//      E3View_State_SetShaderIllumination : Set the illumination shader state
//-----------------------------------------------------------------------------
void
E3View_State_SetShaderIllumination(TQ3ViewObject theView, const TQ3IlluminationShaderObject theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;
	
	
	
	// Get type of old and new illumination
	TQ3ObjectType oldType = ( ( (E3View*) theView )->instanceData.viewStack->shaderIllumination == NULL ) ? kQ3ObjectTypeInvalid :
		Q3IlluminationShader_GetType ( ( (E3View*) theView )->instanceData.viewStack->shaderIllumination ) ;
	TQ3ObjectType newType = ( theData == NULL ) ? kQ3ObjectTypeInvalid : Q3IlluminationShader_GetType ( theData ) ;



	// Set the value
	E3Shared_Replace ( & ( (E3View*) theView )->instanceData.viewStack->shaderIllumination, theData ) ;



	// Update the renderer if the type has changed
	if ( newType != oldType )
		e3view_stack_update ( (E3View*) theView, kQ3ViewStateShaderIllumination ) ;
	}





//=============================================================================
//      E3View_State_SetShaderSurface : Set the surface shader state
//-----------------------------------------------------------------------------
void
E3View_State_SetShaderSurface(	TQ3ViewObject theView, const TQ3SurfaceShaderObject theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	if ( ( (E3View*) theView )->instanceData.viewStack->shaderSurface != theData )
		{
		// Set the value
		E3Shared_Replace ( & ( (E3View*) theView )->instanceData.viewStack->shaderSurface, theData ) ;



		// Update the renderer
		e3view_stack_update ( (E3View*) theView, kQ3ViewStateShaderSurface ) ;
		}
	}





//=============================================================================
//      E3View_State_SetStyleSubdivision : Set the subdivision state.
//-----------------------------------------------------------------------------
//		Note :	We normalise the value pushed on the stack, so that unused
//				fields will be 0.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleSubdivision(TQ3ViewObject theView, const TQ3SubdivisionStyleData *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->styleSubdivision = *theData ;



	// Normalise it
	if ( theData->method != kQ3SubdivisionMethodConstant )
		( (E3View*) theView )->instanceData.viewStack->styleSubdivision.c2 = 0.0f ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateStyleSubdivision ) ;
	}





//=============================================================================
//      E3View_State_SetStylePickID : Set the pick ID state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStylePickID(TQ3ViewObject theView, TQ3Uns32 pickID)
	{
	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR(( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->stylePickID = pickID ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateStylePickID ) ;
	}





//=============================================================================
//      E3View_State_SetStylePickParts : Set the pick parts state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStylePickParts(TQ3ViewObject theView, TQ3PickParts pickParts)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->stylePickParts = pickParts ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateStylePickParts ) ;
	}





//=============================================================================
//      E3View_State_SetStyleCastShadows : Set the cast shadows state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleCastShadows(TQ3ViewObject theView, TQ3Boolean castShadows)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->styleCastShadows = castShadows ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateStyleCastShadows ) ;
	}





//=============================================================================
//      E3View_State_SetStyleReceiveShadows : Set the receive shadows state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleReceiveShadows(TQ3ViewObject theView, TQ3Boolean receiveShadows)
	{
	// Validate our state
	Q3_ASSERT(Q3_VALID_PTR( ( (E3View*) theView )->instanceData.viewStack));



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->styleReceiveShadows = receiveShadows;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateStyleReceiveShadows);
	}





//=============================================================================
//      E3View_State_SetStyleFill : Set the fill state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleFill(TQ3ViewObject theView, TQ3FillStyle fillStyle)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	if ( ( (E3View*) theView )->instanceData.viewStack->styleFill != fillStyle )
		{
		// Set the value
		( (E3View*) theView )->instanceData.viewStack->styleFill = fillStyle ;



		// Update the renderer
		e3view_stack_update ( (E3View*) theView, kQ3ViewStateStyleFill ) ;
		}
	}





//=============================================================================
//      E3View_State_SetStyleBackfacing : Set the backfacing state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleBackfacing(TQ3ViewObject theView, TQ3BackfacingStyle backfacingStyle)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	if ( ( (E3View*) theView )->instanceData.viewStack->styleBackfacing != backfacingStyle )
		{
		// Set the value
		( (E3View*) theView )->instanceData.viewStack->styleBackfacing = backfacingStyle ;



		// Update the renderer
		e3view_stack_update ( (E3View*) theView, kQ3ViewStateStyleBackfacing ) ;
		}
	}





//=============================================================================
//      E3View_State_SetStyleInterpolation : Set the interpolation state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleInterpolation(TQ3ViewObject theView, TQ3InterpolationStyle interpolationStyle)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	if ( ( (E3View*) theView )->instanceData.viewStack->styleInterpolation != interpolationStyle )
		{
		// Set the value
		( (E3View*) theView )->instanceData.viewStack->styleInterpolation = interpolationStyle ;



		// Update the renderer
		e3view_stack_update ( (E3View*) theView, kQ3ViewStateStyleInterpolation ) ;
		}
	}





//=============================================================================
//      E3View_State_SetStyleHilight : Set the orientation state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleHighlight(TQ3ViewObject theView, TQ3AttributeSet highlightAttribute)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	E3Shared_Replace ( & ( (E3View*) theView )->instanceData.viewStack->styleHighlight, highlightAttribute ) ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateStyleHighlight ) ;
	}





//=============================================================================
//      E3View_State_SetStyleOrientation : Set the orientation state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleOrientation(TQ3ViewObject theView, TQ3OrientationStyle frontFacingDirection)
	{	
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	if ( ( (E3View*) theView )->instanceData.viewStack->styleOrientation != frontFacingDirection )
		{
		// Set the value
		( (E3View*) theView )->instanceData.viewStack->styleOrientation = frontFacingDirection ;



		// Update the renderer
		e3view_stack_update ( (E3View*) theView, kQ3ViewStateStyleOrientation ) ;
		}
	}





//=============================================================================
//      E3View_State_SetStyleAntiAlias : Set the anti-alias state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleAntiAlias(TQ3ViewObject theView, const TQ3AntiAliasStyleData *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the state
	//
	// Multiple submits of a style within a group simply override each other, and
	// so we can avoid updating the renderer if the style state does not change.
	if ( memcmp ( & ( (E3View*) theView )->instanceData.viewStack->styleAntiAlias, theData, sizeof ( TQ3AntiAliasStyleData ) ) != 0 )
		{
		( (E3View*) theView )->instanceData.viewStack->styleAntiAlias = *theData ;
		e3view_stack_update ( (E3View*) theView, kQ3ViewStateStyleAntiAlias ) ;
		}
	}





//=============================================================================
//      E3View_State_SetStyleFog : Set the fog state.
//-----------------------------------------------------------------------------
void
E3View_State_SetStyleFog(TQ3ViewObject theView, const TQ3FogStyleData *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the state
	//
	// Multiple submits of a style within a group simply override each other, and
	// so we can avoid updating the renderer if the style state does not change.
	if ( memcmp ( & ( (E3View*) theView )->instanceData.viewStack->styleFog, theData, sizeof ( TQ3FogStyleData ) ) != 0 )
		{
		( (E3View*) theView )->instanceData.viewStack->styleFog = *theData ;
		e3view_stack_update ( (E3View*) theView, kQ3ViewStateStyleFog ) ;
		}
	}





//=============================================================================
//      E3View_State_SetAttributeSurfaceUV : Set the surface UV state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeSurfaceUV(TQ3ViewObject theView, const TQ3Param2D *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->attributeSurfaceUV = *theData ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateAttributeSurfaceUV ) ;
	}





//=============================================================================
//      E3View_State_SetAttributeShadingUV : Set the shading UV state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeShadingUV(TQ3ViewObject theView, const TQ3Param2D *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->attributeShadingUV = *theData ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateAttributeShadingUV ) ;
	}





//=============================================================================
//      E3View_State_SetAttributeNormal : Set the normal state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeNormal(TQ3ViewObject theView, const TQ3Vector3D *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->attributeNormal = *theData ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateAttributeNormal ) ;
	}





//=============================================================================
//      E3View_State_SetAttributeAmbientCoefficient : Set the ambient coeff.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeAmbientCoefficient(TQ3ViewObject theView, const float *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->attributeAmbientCoefficient = *theData ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateAttributeAmbientCoefficient ) ;
	}





//=============================================================================
//      E3View_State_SetAttributeDiffuseColor : Set the diffuse colour state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeDiffuseColor(TQ3ViewObject theView, const TQ3ColorRGB *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->attributeDiffuseColor = *theData ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateAttributeDiffuseColour ) ;
	}





//=============================================================================
//      E3View_State_SetAttributeSpecularColor : Set the specular colour state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeSpecularColor(TQ3ViewObject theView, const TQ3ColorRGB *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->attributeSpecularColor = *theData ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateAttributeSpecularColour ) ;
	}





//=============================================================================
//      E3View_State_SetAttributeSpecularControl : Set the specular control.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeSpecularControl(TQ3ViewObject theView, const float *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->attributeSpecularControl = *theData ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateAttributeSpecularControl ) ;
	}





//=============================================================================
//      E3View_State_SetAttributeTransparencyColor : Set the transparency.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeTransparencyColor(TQ3ViewObject theView, const TQ3ColorRGB *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->attributeTransparencyColor = *theData ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateAttributeTransparencyColour ) ;
	}





//=============================================================================
//      E3View_State_SetAttributeSurfaceTangent : Set the surface tangent.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeSurfaceTangent(TQ3ViewObject theView, const TQ3Tangent2D *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->attributeSurfaceTangent = *theData ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateAttributeSurfaceTangent ) ;
	}	





//=============================================================================
//      E3View_State_SetAttributeHighlightState : Set the highlight state.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeHighlightState(TQ3ViewObject theView, const TQ3Switch *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	( (E3View*) theView )->instanceData.viewStack->attributeHighlightState = *theData ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateAttributeHighlightState ) ;
	}





//=============================================================================
//      E3View_State_SetAttributeSurfaceShader : Set the surface shader.
//-----------------------------------------------------------------------------
void
E3View_State_SetAttributeSurfaceShader(TQ3ViewObject theView, const TQ3SurfaceShaderObject *theData)
	{
	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Set the value
	E3Shared_Replace ( & ( (E3View*) theView )->instanceData.viewStack->attributeSurfaceShader, *theData ) ;



	// Update the renderer
	e3view_stack_update ( (E3View*) theView, kQ3ViewStateAttributeSurfaceShader ) ;
	}





//=============================================================================
//      E3View_New : Create a TQ3ViewObject.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ViewObject
E3View_New(void)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeView, kQ3False, NULL);
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
	{	
	// Cancel the view
	if ( ( (E3View*) theView )->instanceData.viewState == kQ3ViewStateInactive )
		return kQ3Failure ;

	( (E3View*) theView )->instanceData.viewState             = kQ3ViewStateCancelled ;
	( (E3View*) theView )->instanceData.submitRetainedMethod  = (TQ3XViewSubmitRetainedMethod)
		e3view_submit_retained_error ;
	( (E3View*) theView )->instanceData.submitImmediateMethod = (TQ3XViewSubmitImmediateMethod)
		e3view_submit_immediate_error ;
	return kQ3Success ;
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
		return kQ3Failure ;



	// Replace the existing renderer
	qd3dStatus = Q3View_SetRenderer(theView, theRenderer);
	Q3Object_Dispose(theRenderer);

	return qd3dStatus ;
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
	{
	// Replace the existing renderer reference
	E3Shared_Replace ( & ( (E3View*) theView )->instanceData.theRenderer, theRenderer ) ;



	// Reset the draw context state
	if ( ( (E3View*) theView )->instanceData.theDrawContext != NULL )
		E3DrawContext_ResetState ( ( (E3View*) theView )->instanceData.theDrawContext ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetRenderer : Return a reference to our renderer.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetRenderer(TQ3ViewObject theView, TQ3RendererObject *theRenderer)
	{
	// Assign a return value
	*theRenderer = NULL ;



	// Make sure we have a renderer
	if ( ( (E3View*) theView )->instanceData.theRenderer == NULL )
		return kQ3Failure ;



	// Create a new reference to our renderer
	*theRenderer = Q3Shared_GetReference ( ( (E3View*) theView )->instanceData.theRenderer ) ;

	return kQ3Success ;
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
	{
	TQ3Matrix4x4			worldToCamera, cameraToFrustum;
	TQ3DrawContextData		drawContextData;



	// Make sure we have the objects we need
	if ( ( (E3View*) theView )->instanceData.theDrawContext == NULL
	||	 ( (E3View*) theView )->instanceData.theRenderer    == NULL
	||	 ( (E3View*) theView )->instanceData.theCamera      == NULL )
		{
		if ( ( (E3View*) theView )->instanceData.theDrawContext == NULL )
			E3ErrorManager_PostError ( kQ3ErrorDrawContextNotSet, kQ3False ) ;
			
		if ( ( (E3View*) theView )->instanceData.theRenderer == NULL )
			E3ErrorManager_PostError ( kQ3ErrorRendererNotSet, kQ3False ) ;
			
		if ( ( (E3View*) theView )->instanceData.theCamera == NULL )
			E3ErrorManager_PostError ( kQ3ErrorCameraNotSet, kQ3False ) ;
			
		return kQ3Failure ;
		}



	// Make sure we have a valid draw context pane - reject empty or negative bounds
	Q3DrawContext_GetData ( ( (E3View*) theView )->instanceData.theDrawContext, &drawContextData ) ;
	if ( drawContextData.paneState )
		{
		if ( drawContextData.pane.min.x >= drawContextData.pane.max.x
		||	 drawContextData.pane.min.y >= drawContextData.pane.max.y )
			{
			E3ErrorManager_PostError ( kQ3ErrorBadDrawContext, kQ3False ) ;
			return kQ3Failure ;
			}
		}



	// Start the submit loop
	TQ3Status qd3dStatus = e3view_submit_begin ( (E3View*) theView, kQ3ViewModeDrawing ) ;



	// If this is the first pass then update the draw context and start the frame
	if ( ( (E3View*) theView )->instanceData.viewPass == 1 && qd3dStatus != kQ3Failure )
		{
		qd3dStatus = ( (E3DrawContext*) ( (E3View*) theView )->instanceData.theDrawContext )->Update () ;

		if ( qd3dStatus != kQ3Failure )
			qd3dStatus = E3Renderer_Method_StartFrame ( theView, ( (E3View*) theView )->instanceData.theDrawContext ) ;
		
		if ( qd3dStatus != kQ3Failure )
			qd3dStatus = Q3Camera_GetWorldToView ( ( (E3View*) theView )->instanceData.theCamera,   &worldToCamera ) ;
		
		if ( qd3dStatus != kQ3Failure )
			qd3dStatus = Q3Camera_GetViewToFrustum ( ( (E3View*) theView )->instanceData.theCamera, &cameraToFrustum ) ;

		if ( qd3dStatus != kQ3Failure )
			E3View_State_SetMatrix ( theView, (TQ3MatrixState) ( kQ3MatrixStateWorldToCamera | kQ3MatrixStateCameraToFrustum ),
											NULL, &worldToCamera, &cameraToFrustum ) ;
		}



	// Start the pass
	if ( qd3dStatus != kQ3Failure )
		{
		( (E3View*) theView )->instanceData.rendererFinishedFrame = kQ3False ;
		qd3dStatus = E3Renderer_Method_StartPass ( theView, ( (E3View*) theView )->instanceData.theCamera, ( (E3View*) theView )->instanceData.theLights) ;
		}



	// Submit the initial state
	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = e3view_submit_initial_state( (E3View*) theView ) ;



	// Handle failure
	if ( qd3dStatus == kQ3Failure )
		(void) e3view_submit_end ( (E3View*) theView, kQ3ViewStatusError ) ;

	return qd3dStatus ;
	}	





//=============================================================================
//      E3View_EndRendering : End a rendering loop.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3View_EndRendering(TQ3ViewObject theView)
	{
	TQ3ViewStatus viewStatus = kQ3ViewStatusDone ;



	// If we're still in the submit loop, end the pass
	if ( ( (E3View*) theView )->instanceData.viewState == kQ3ViewStateSubmitting )
		viewStatus = E3Renderer_Method_EndPass ( theView ) ;



	// End the submit loop
	return e3view_submit_end ( (E3View*) theView, viewStatus ) ;
	}





//=============================================================================
//      E3View_Flush : Ask the renderer to flush output (non-blocking).
//-----------------------------------------------------------------------------
//		Note : Can only be called from within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_Flush(TQ3ViewObject theView)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Ask the renderer to flush output
	return E3Renderer_Method_FlushFrame ( theView, ( (E3View*) theView )->instanceData.theDrawContext ) ;
	}





//=============================================================================
//      E3View_Sync : Ask the renderer to flush output (blocking).
//-----------------------------------------------------------------------------
//		Note : Can only be called from outside a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_Sync(TQ3ViewObject theView)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateInactive )
		return kQ3Failure ;



	// Ask the renderer to flush output
	TQ3Status qd3dStatus = E3Renderer_Method_EndFrame ( theView, ( (E3View*) theView )->instanceData.theDrawContext ) ;
	if ( qd3dStatus == kQ3Failure )
		return qd3dStatus ;



	// If the renderer is async, block until its done. We might want to have
	// a hook in here for a platform specific yield routine, since we're just
	// wasting CPU time otherwise.
	//
	// The kQ3XMethodTypeRendererEndFrame method is only implemented by async
	// renderers, so if this method is implemented we know we need to block.
	if ( ( (E3View*) theView )->instanceData.theRenderer->GetMethod ( kQ3XMethodTypeRendererEndFrame ) != NULL )
		{
		// Note - the QD3D Interactive Renderer doesn't appear to call Q3XView_EndFrame even
		// though it should, since it implements the kQ3XMethodTypeRendererEndFrame method.
		//
		// For now, we just return immediately - this may cause flicker on some cards. This
		// could be reinstated if blocking was ever required (and perhaps only make it conditional
		// when rendering with the QD3D IR).
		#if 0
		// Wait for E3View_EndFrame to set the flag
		while ( ! ( (E3View*) theView )->instanceData.rendererFinishedFrame )
			;
		#endif
		}

	return kQ3Success ;
	}





//=============================================================================
//      E3View_StartBoundingBox : Start a bounding box loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_StartBoundingBox(TQ3ViewObject theView, TQ3ComputeBounds computeBounds)
	{
	// Start the submit loop
	TQ3Status qd3dStatus = e3view_submit_begin ( (E3View*) theView, kQ3ViewModeCalcBounds ) ;
	if ( qd3dStatus == kQ3Failure )
		return qd3dStatus ;



	// If this is the first pass, initialise the bounding box
	if ( ( (E3View*) theView )->instanceData.viewPass == 1 )
		{
		if ( computeBounds == kQ3ComputeBoundsExact )
			( (E3View*) theView )->instanceData.boundingMethod = kQ3BoxBoundsExact ;
		else
			( (E3View*) theView )->instanceData.boundingMethod = kQ3BoxBoundsApprox ;
		
		( (E3View*) theView )->instanceData.boundingBox.min.x   = 0.0f ;
		( (E3View*) theView )->instanceData.boundingBox.min.y   = 0.0f ;
		( (E3View*) theView )->instanceData.boundingBox.min.z   = 0.0f ;
		( (E3View*) theView )->instanceData.boundingBox.max.x   = 0.0f ;
		( (E3View*) theView )->instanceData.boundingBox.max.y   = 0.0f ;
		( (E3View*) theView )->instanceData.boundingBox.max.z   = 0.0f ;
		( (E3View*) theView )->instanceData.boundingBox.isEmpty = kQ3True ;
		}



	// Submit the initial state
	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = e3view_submit_initial_state ( (E3View*) theView ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3View_EndBoundingBox : End a bounding box loop.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3View_EndBoundingBox(TQ3ViewObject theView, TQ3BoundingBox *result)
	{
	TQ3ViewStatus viewStatus = kQ3ViewStatusDone ;



	// If we're still in the submit loop, return the results
	if ( ( (E3View*) theView )->instanceData.viewState == kQ3ViewStateSubmitting )
		*result = ( (E3View*) theView )->instanceData.boundingBox ;



	// End the submit loop
	return e3view_submit_end ( (E3View*) theView, viewStatus ) ;
	}





//=============================================================================
//      E3View_StartBoundingSphere : Start a bounding sphere loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_StartBoundingSphere(TQ3ViewObject theView, TQ3ComputeBounds computeBounds)
	{
	// Start the submit loop
	TQ3Status qd3dStatus = e3view_submit_begin ( (E3View*) theView, kQ3ViewModeCalcBounds ) ;
	if ( qd3dStatus == kQ3Failure )
		return qd3dStatus ;



	// If this is the first pass, initialise the bounding sphere
	if ( ( (E3View*) theView )->instanceData.viewPass == 1 )
		{
		// clean previous points from an aborted operation...
		Q3Object_CleanDispose ( & ( (E3View*) theView )->instanceData.boundingPointsSlab ) ;
		
		// allocate new Slab to hold the points
		( (E3View*) theView )->instanceData.boundingPointsSlab = Q3SlabMemory_New ( sizeof ( TQ3Point3D ), 0, NULL ) ;
		if ( ( (E3View*) theView )->instanceData.boundingPointsSlab == NULL )
			return qd3dStatus ;
		
		
		if ( computeBounds == kQ3ComputeBoundsExact )
			( (E3View*) theView )->instanceData.boundingMethod = kQ3SphereBoundsExact ;
		else
			( (E3View*) theView )->instanceData.boundingMethod = kQ3SphereBoundsApprox ;
		
		( (E3View*) theView )->instanceData.boundingSphere.origin.x = 0.0f ;
		( (E3View*) theView )->instanceData.boundingSphere.origin.y = 0.0f ;
		( (E3View*) theView )->instanceData.boundingSphere.origin.z = 0.0f ;
		( (E3View*) theView )->instanceData.boundingSphere.radius   = 0.0f ;
		( (E3View*) theView )->instanceData.boundingSphere.isEmpty  = kQ3True ;
		}



	// Submit the initial state
	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = e3view_submit_initial_state ( (E3View*) theView ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3View_EndBoundingSphere : End a bounding sphere loop.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3View_EndBoundingSphere(TQ3ViewObject theView, TQ3BoundingSphere *result)
	{
	TQ3ViewStatus viewStatus = kQ3ViewStatusDone ;


	// If we're still in the submit loop, calcolate and return the results
	if ( ( (E3View*) theView )->instanceData.viewState == kQ3ViewStateSubmitting )
		{
		if ( ( (E3View*) theView )->instanceData.boundingPointsSlab != NULL && Q3SlabMemory_GetCount ( ( (E3View*) theView )->instanceData.boundingPointsSlab ) > 0 )
			{
			TQ3Point3D* points = (TQ3Point3D*) Q3SlabMemory_GetData ( ( (E3View*) theView )->instanceData.boundingPointsSlab,0 ) ;
			if ( points != NULL )
				Q3BoundingSphere_SetFromPoints3D ( & ( (E3View*) theView )->instanceData.boundingSphere,
								 points, Q3SlabMemory_GetCount ( ( (E3View*) theView )->instanceData.boundingPointsSlab ), sizeof ( TQ3Point3D ) ) ;
			}
		*result = ( (E3View*) theView )->instanceData.boundingSphere ;
		}



	// End the submit loop
	viewStatus = e3view_submit_end ( (E3View*) theView, viewStatus ) ;

	Q3Object_CleanDispose ( & ( (E3View*) theView )->instanceData.boundingPointsSlab ) ;

	return viewStatus ;
	}





//=============================================================================
//      E3View_StartPicking : Start a picking loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_StartPicking(TQ3ViewObject theView, TQ3PickObject pick)
	{
	// Start the submit loop
	TQ3Status qd3dStatus = e3view_submit_begin ( (E3View*) theView, kQ3ViewModePicking ) ;
	if ( qd3dStatus == kQ3Failure )
		return qd3dStatus ;



	// If this is the first pass, start picking
	if ( ( (E3View*) theView )->instanceData.viewPass == 1 )
		e3view_pick_begin ( (E3View*) theView, pick ) ;



	// Submit the initial state
	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = e3view_submit_initial_state ( (E3View*) theView ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3View_EndPicking : End a picking loop.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3View_EndPicking(TQ3ViewObject theView)
	{
	TQ3ViewStatus viewStatus = kQ3ViewStatusDone ;



	// If we're still in the submit loop, end the pass
	if ( ( (E3View*) theView )->instanceData.viewState == kQ3ViewStateSubmitting )
		viewStatus = kQ3ViewStatusDone ; // What is the point of this? It is already kQ3ViewStatusDone
		


	// End the submit loop
	viewStatus = e3view_submit_end ( (E3View*) theView, viewStatus ) ;
	if ( viewStatus != kQ3ViewStatusRetraverse )
		e3view_pick_end ( (E3View*) theView ) ;
	
	return viewStatus ;
	}





//=============================================================================
//      E3View_StartWriting : Start a writing loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_StartWriting(TQ3ViewObject theView, TQ3FileObject theFile)
	{
	// Start the submit loop
	TQ3Status qd3dStatus = e3view_submit_begin ( (E3View*) theView, kQ3ViewModeWriting ) ;
	if ( qd3dStatus == kQ3Failure )
		return qd3dStatus ;



	// Replace the existing file reference
	if ( qd3dStatus != kQ3Failure )
		E3Shared_Replace ( & ( (E3View*) theView )->instanceData.theFile, theFile ) ;



	// If this is the first pass then prepare the file for writing
	if ( ( (E3View*) theView )->instanceData.viewPass == 1 && qd3dStatus != kQ3Failure )
		qd3dStatus = E3FileFormat_Method_StartFile ( theView ) ;



	// Start the pass
	if ( qd3dStatus != kQ3Failure )
		{
		 ( (E3View*) theView )->instanceData.rendererFinishedFrame = kQ3False ;
		qd3dStatus = E3FileFormat_Method_StartPass ( theView ) ;
		}

	return qd3dStatus ;
	}





//=============================================================================
//      E3View_EndWriting : End a writing loop.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3View_EndWriting(TQ3ViewObject theView)
	{
	TQ3ViewStatus viewStatus = kQ3ViewStatusDone ;



	// If we're still in the submit loop, end the pass
	if ( ( (E3View*) theView )->instanceData.viewState == kQ3ViewStateSubmitting )
		viewStatus = E3FileFormat_Method_EndPass ( theView ) ;



	// End the submit loop
	viewStatus = e3view_submit_end ( (E3View*) theView, viewStatus ) ;



	// If we don't need to retraverse, drop the file reference
	if ( viewStatus != kQ3ViewStatusRetraverse )
		{
		E3FileFormat_Method_EndFile(theView);
		E3Shared_Replace ( & ( (E3View*) theView )->instanceData.theFile, NULL ) ;
		}

	return viewStatus ;
	}





//=============================================================================
//      E3View_GetCamera : Return a reference to our camera.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetCamera(TQ3ViewObject theView, TQ3CameraObject *theCamera)
	{
	// Assign a return value
	*theCamera = NULL ;



	// Make sure we have a camera
	if ( ( (E3View*) theView )->instanceData.theCamera == NULL )
		return kQ3Failure ;



	// Create a new reference to our camera
	*theCamera = Q3Shared_GetReference ( ( (E3View*) theView )->instanceData.theCamera ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_SetCamera : Assign a new camera to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetCamera(TQ3ViewObject theView, TQ3CameraObject theCamera)
	{
	// Replace the existing camera reference
	E3Shared_Replace ( & ( (E3View*) theView )->instanceData.theCamera, theCamera ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_SetLightGroup : Assign a new light group to a view.
//-----------------------------------------------------------------------------
//		Note : Lights are removed if lightGroup is NULL.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetLightGroup(TQ3ViewObject theView, TQ3GroupObject lightGroup)
	{
	// Replace the existing light group reference
	E3Shared_Replace ( & ( (E3View*) theView )->instanceData.theLights, lightGroup ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetLightGroup : Return a reference to our light group.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetLightGroup(TQ3ViewObject theView, TQ3GroupObject *lightGroup)
	{
	// Assign a return value
	*lightGroup = NULL ;



	// If we have a light group, create a new reference to it
	if ( ( (E3View*) theView )->instanceData.theLights != NULL )
		*lightGroup = Q3Shared_GetReference ( ( (E3View*) theView )->instanceData.theLights ) ;

	return kQ3Success ;
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
	if (qd3dStatus == kQ3Failure )
		return qd3dStatus ;



	// If we don't have a light group yet, create one
	if (lightGroup == NULL)
		{
		lightGroup = Q3LightGroup_New();
		if (lightGroup == NULL)
			return kQ3Failure ;
		
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
	
	return kQ3Success ;
}





//=============================================================================
//      E3View_SetIdleMethod : Set the idle method.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetIdleMethod(TQ3ViewObject theView, TQ3ViewIdleMethod idleMethod, const void *idleData)
	{
	// Update the object
	( (E3View*) theView )->instanceData.idleMethod = idleMethod ;
	( (E3View*) theView )->instanceData.idleData   = idleData ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3View_SetIdleProgressMethod : Set the idle progress method.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetIdleProgressMethod(TQ3ViewObject theView, TQ3ViewIdleProgressMethod idleMethod, const void *idleData)
	{
	// Update the object
	( (E3View*) theView )->instanceData.idleProgressMethod = idleMethod ;
	( (E3View*) theView )->instanceData.idleProgressData   = idleData ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3View_SetEndFrameMethod : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetEndFrameMethod(TQ3ViewObject theView, TQ3ViewEndFrameMethod endFrame, void *endFrameData)
	{
	// Update the object
	( (E3View*) theView )->instanceData.endFrameMethod = endFrame ;
	( (E3View*) theView )->instanceData.endFrameData   = endFrameData ;
	
	return kQ3Success ;
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
	{
	// Update our state
	( (E3View*) theView )->instanceData.allowGroupCulling = allowCulling ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_TransformLocalToWorld : Transform a point from local->world.
//-----------------------------------------------------------------------------
TQ3Status
E3View_TransformLocalToWorld(TQ3ViewObject theView, const TQ3Point3D *localPoint, TQ3Point3D *worldPoint)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Get the local to world matrix
	const TQ3Matrix4x4* localToWorld = & ( (E3View*) theView )->instanceData.viewStack->matrixLocalToWorld ;
	Q3_ASSERT_VALID_PTR ( localToWorld ) ;



	// Transform the point
	Q3Point3D_Transform ( localPoint, localToWorld, worldPoint ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3View_TransformLocalToWindow : Transform a point from local->window.
//-----------------------------------------------------------------------------
TQ3Status
E3View_TransformLocalToWindow(TQ3ViewObject theView, const TQ3Point3D *localPoint, TQ3Point2D *windowPoint)
	{
	TQ3Matrix4x4		theMatrix, worldToFrustum, frustumToWindow;
	TQ3Point3D			thePoint;



	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Get the matrices we ned
	Q3View_GetWorldToFrustumMatrixState ( theView,  &worldToFrustum ) ;
	Q3View_GetFrustumToWindowMatrixState ( theView, &frustumToWindow ) ;

	Q3Matrix4x4_Multiply ( E3View_State_GetMatrixLocalToWorld ( theView ), &worldToFrustum, &theMatrix ) ;
	Q3Matrix4x4_Multiply ( &theMatrix, &frustumToWindow, &theMatrix ) ;

 

	// Transform the point
	Q3Point3D_Transform ( localPoint, &theMatrix, &thePoint ) ;
	windowPoint->x = thePoint.x ;
	windowPoint->y = thePoint.y ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_TransformWorldToWindow : Transform a point from world->window.
//-----------------------------------------------------------------------------
TQ3Status
E3View_TransformWorldToWindow(TQ3ViewObject theView, const TQ3Point3D *worldPoint, TQ3Point2D *windowPoint)
	{
	TQ3Matrix4x4		theMatrix, worldToFrustum, frustumToWindow;
	TQ3Point3D			thePoint;



	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Get the matrices we ned
	Q3View_GetWorldToFrustumMatrixState ( theView,  &worldToFrustum ) ;
	Q3View_GetFrustumToWindowMatrixState ( theView, &frustumToWindow ) ;
	Q3Matrix4x4_Multiply ( &worldToFrustum, &frustumToWindow, &theMatrix ) ;



	// Transform the point
	Q3Point3D_Transform ( worldPoint, &theMatrix, &thePoint ) ;
	windowPoint->x = thePoint.x ;
	windowPoint->y = thePoint.y ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_SetDrawContext : Assign a new draw context to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetDrawContext(TQ3ViewObject theView, TQ3DrawContextObject drawContext)
	{
	// Replace the existing draw context reference, and reset the state
	E3Shared_Replace ( & ( (E3View*) theView )->instanceData.theDrawContext, drawContext ) ;

	if ( ( (E3View*) theView )->instanceData.theDrawContext != NULL )
		E3DrawContext_ResetState ( ( (E3View*) theView )->instanceData.theDrawContext ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetDrawContext : Return a reference to our draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetDrawContext(TQ3ViewObject theView, TQ3DrawContextObject *drawContext)
	{
	// Assign a return value
	*drawContext = NULL ;



	// Make sure we have a draw context
	if ( ( (E3View*) theView )->instanceData.theDrawContext == NULL )
		return kQ3Failure ;



	// Create a new reference to our camera
	*drawContext = Q3Shared_GetReference ( ( (E3View*) theView )->instanceData.theDrawContext ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetLocalToWorldMatrixState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetLocalToWorldMatrixState(TQ3ViewObject theView, TQ3Matrix4x4 *theMatrix)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*theMatrix = ( (E3View*) theView )->instanceData.viewStack->matrixLocalToWorld ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetWorldToFrustumMatrixState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetWorldToFrustumMatrixState(TQ3ViewObject theView, TQ3Matrix4x4 *theMatrix)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Get the matrix
	return Q3Camera_GetWorldToFrustum ( ( (E3View*) theView )->instanceData.theCamera, theMatrix ) ;
	}





//=============================================================================
//      E3View_GetFrustumToWindowMatrixState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetFrustumToWindowMatrixState(TQ3ViewObject theView, TQ3Matrix4x4 *theMatrix)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Get the size of our draw context
	TQ3Area	 thePane ;
	TQ3Status qd3dStatus = Q3DrawContext_GetPane ( ( (E3View*) theView )->instanceData.theDrawContext, &thePane ) ;
	if ( qd3dStatus == kQ3Failure )
		return qd3dStatus ;



	// Return the frustum to window matrix
	Q3Matrix4x4_SetIdentity ( theMatrix ) ;

	theMatrix->value [ 0 ] [ 0 ] =   ( thePane.max.x - thePane.min.x ) * 0.5f ;
	theMatrix->value [ 1 ] [ 1 ] = - ( thePane.max.y - thePane.min.y ) * 0.5f ;
	theMatrix->value [ 3 ] [ 0 ] =   ( thePane.max.x + thePane.min.x ) * 0.5f ;
	theMatrix->value [ 3 ] [ 1 ] =   ( thePane.max.y + thePane.min.y ) * 0.5f ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetBackfacingStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetBackfacingStyleState(TQ3ViewObject theView, TQ3BackfacingStyle *backfacingStyle)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*backfacingStyle = ( (E3View*) theView )->instanceData.viewStack->styleBackfacing ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetInterpolationStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetInterpolationStyleState(TQ3ViewObject theView, TQ3InterpolationStyle *interpolationType)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*interpolationType = ( (E3View*) theView )->instanceData.viewStack->styleInterpolation ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetFillStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetFillStyleState(TQ3ViewObject theView, TQ3FillStyle *fillStyle)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*fillStyle = ( (E3View*) theView )->instanceData.viewStack->styleFill ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetHighlightStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetHighlightStyleState(TQ3ViewObject theView, TQ3AttributeSet *highlightStyle)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*highlightStyle = NULL ;
	if ( ( (E3View*) theView )->instanceData.viewStack->styleHighlight != NULL )
		*highlightStyle = Q3Shared_GetReference ( ( (E3View*) theView )->instanceData.viewStack->styleHighlight ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetSubdivisionStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetSubdivisionStyleState(TQ3ViewObject theView, TQ3SubdivisionStyleData *subdivisionStyle)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*subdivisionStyle = ( (E3View*) theView )->instanceData.viewStack->styleSubdivision ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetOrientationStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetOrientationStyleState(TQ3ViewObject theView, TQ3OrientationStyle *frontFacingDirectionStyle)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*frontFacingDirectionStyle = ( (E3View*) theView )->instanceData.viewStack->styleOrientation ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetCastShadowsStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetCastShadowsStyleState(TQ3ViewObject theView, TQ3Boolean *castShadows)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*castShadows = ( (E3View*) theView )->instanceData.viewStack->styleCastShadows ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetReceiveShadowsStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetReceiveShadowsStyleState(TQ3ViewObject theView, TQ3Boolean *receiveShadows)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*receiveShadows = ( (E3View*) theView )->instanceData.viewStack->styleReceiveShadows ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetPickIDStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetPickIDStyleState(TQ3ViewObject theView, TQ3Uns32 *pickIDStyle)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*pickIDStyle = ( (E3View*) theView )->instanceData.viewStack->stylePickID ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetPickPartsStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetPickPartsStyleState(TQ3ViewObject theView, TQ3PickParts *pickPartsStyle)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*pickPartsStyle = ( (E3View*) theView )->instanceData.viewStack->stylePickParts ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetAntiAliasStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetAntiAliasStyleState(TQ3ViewObject theView, TQ3AntiAliasStyleData *antiAliasData)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*antiAliasData = ( (E3View*) theView )->instanceData.viewStack->styleAntiAlias ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetFogStyleState : Get the current state.
//-----------------------------------------------------------------------------
//		Note : Can only be called within a submitting loop.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetFogStyleState(TQ3ViewObject theView, TQ3FogStyleData *fogData)
	{
	// Make sure we're in the correct state
	if ( ( (E3View*) theView )->instanceData.viewState != kQ3ViewStateSubmitting )
		return kQ3Failure ;



	// Validate our state
	Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;



	// Get the value
	*fogData = ( (E3View*) theView )->instanceData.viewStack->styleFog ;

	return kQ3Success ;
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
	{
	// Return a new reference to the attribute set
	if ( ( (E3View*) theView )->instanceData.viewAttributes != NULL )
		*attributeSet = Q3Shared_GetReference ( ( (E3View*) theView )->instanceData.viewAttributes ) ;
	else
		*attributeSet = NULL;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_SetDefaultAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3View_SetDefaultAttributeSet(TQ3ViewObject theView, TQ3AttributeSet attributeSet)
	{
	// Replace the existing attribute set reference
	E3Shared_Replace ( & ( (E3View*) theView )->instanceData.viewAttributes, attributeSet ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3View_GetAttributeSetState : Get the current attribute set for a view.
//-----------------------------------------------------------------------------
TQ3Status
E3View_GetAttributeSetState(TQ3ViewObject theView, TQ3AttributeSet *attributeSet)
	{
	// Set up a return value
	*attributeSet = NULL ;



	// If there's anything on the stack, return the current attribute set
	if ( ( (E3View*) theView )->instanceData.viewStack != NULL )
		{
		Q3_ASSERT ( Q3_VALID_PTR ( ( (E3View*) theView )->instanceData.viewStack ) ) ;
		if ( ( (E3View*) theView )->instanceData.viewStack->attributeSet != NULL )
			*attributeSet = Q3Shared_GetReference ( ( (E3View*) theView )->instanceData.viewStack->attributeSet ) ;
		else
			Q3View_GetDefaultAttributeSet ( theView, attributeSet ) ;
		}

	return kQ3Success ;
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
		return kQ3Success ;
		}



	// Get a pointer to the data for the attribute
	*((void **) data) = Q3XAttributeSet_GetPointer(attributeSet, attributeType);
	
	return kQ3Success ;
}





//=============================================================================
//      E3View_IsMyClass : Check if object pointer is valid and of type view
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3ObjectTypeView )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3View_IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3View ) ;
		
	return kQ3False ;
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

	return qd3dStatus ;
}





//=============================================================================
//      E3Pop_Submit : Pop the view state.
//-----------------------------------------------------------------------------
TQ3Status
E3Pop_Submit(TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3StateOperatorTypePop, NULL);

	return qd3dStatus ;
}





//=============================================================================
//      E3Push_New : Create a new push operator object.
//-----------------------------------------------------------------------------
TQ3StateOperatorObject
E3Push_New(void)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3StateOperatorTypePush, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3Pop_New : Create a new pop operator object.
//-----------------------------------------------------------------------------
TQ3StateOperatorObject
E3Pop_New(void)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3StateOperatorTypePop, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3StateOperator_Submit : Submit a push or pop state operator object.
//-----------------------------------------------------------------------------
TQ3Status
E3StateOperator_Submit(TQ3StateOperatorObject stateOperator, TQ3ViewObject theView)
	{
	// Submit the object to the view
	
	return ( (E3View*) theView )->instanceData.submitRetainedMethod ( (E3View*) theView , stateOperator ) ;
	}



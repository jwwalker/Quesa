/*  NAME:
        QD3DView.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
//      Q3View_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ViewObject
Q3View_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_New());
}





//=============================================================================
//      Q3View_Cancel : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_Cancel(TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_Cancel(view));
}





//=============================================================================
//      Q3View_SetRendererByType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SetRendererByType(TQ3ViewObject view, TQ3ObjectType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_SetRendererByType(view, theType));
}





//=============================================================================
//      Q3View_SetRenderer : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SetRenderer(TQ3ViewObject view, TQ3RendererObject renderer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on renderer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_SetRenderer(view, renderer));
}





//=============================================================================
//      Q3View_GetRenderer : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetRenderer(TQ3ViewObject view, TQ3RendererObject *renderer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(renderer), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on renderer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetRenderer(view, renderer));
}





//=============================================================================
//      Q3View_StartRendering : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_StartRendering(TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_StartRendering(view));
}





//=============================================================================
//      Q3View_EndRendering : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewStatus
Q3View_EndRendering(TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3ViewStatusError);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3ViewStatusError);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3ViewStatusError);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_EndRendering(view));
}





//=============================================================================
//      Q3View_Flush : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_Flush(TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_Flush(view));
}





//=============================================================================
//      Q3View_Sync : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_Sync(TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_Sync(view));
}





//=============================================================================
//      Q3View_StartBoundingBox : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_StartBoundingBox(TQ3ViewObject view, TQ3ComputeBounds computeBounds)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on computeBounds
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_StartBoundingBox(view, computeBounds));
}





//=============================================================================
//      Q3View_EndBoundingBox : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewStatus
Q3View_EndBoundingBox(TQ3ViewObject view, TQ3BoundingBox *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3ViewStatusError);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3ViewStatusError);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), kQ3ViewStatusError);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3ViewStatusError);

	if (0) // Further checks on result
		return(kQ3ViewStatusError);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_EndBoundingBox(view, result));
}





//=============================================================================
//      Q3View_StartBoundingSphere : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_StartBoundingSphere(TQ3ViewObject view, TQ3ComputeBounds computeBounds)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on computeBounds
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_StartBoundingSphere(view, computeBounds));
}





//=============================================================================
//      Q3View_EndBoundingSphere : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewStatus
Q3View_EndBoundingSphere(TQ3ViewObject view, TQ3BoundingSphere *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3ViewStatusError);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3ViewStatusError);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), kQ3ViewStatusError);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3ViewStatusError);

	if (0) // Further checks on result
		return(kQ3ViewStatusError);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_EndBoundingSphere(view, result));
}





//=============================================================================
//      Q3View_StartPicking : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_StartPicking(TQ3ViewObject view, TQ3PickObject pick)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on pick
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_StartPicking(view, pick));
}





//=============================================================================
//      Q3View_EndPicking : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewStatus
Q3View_EndPicking(TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3ViewStatusError);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3ViewStatusError);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3ViewStatusError);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_EndPicking(view));
}





//=============================================================================
//      Q3View_GetCamera : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetCamera(TQ3ViewObject view, TQ3CameraObject *camera)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(camera), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on camera
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetCamera(view, camera));
}





//=============================================================================
//      Q3View_SetCamera : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SetCamera(TQ3ViewObject view, TQ3CameraObject camera)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on camera
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_SetCamera(view, camera));
}





//=============================================================================
//      Q3View_SetLightGroup : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SetLightGroup(TQ3ViewObject view, TQ3GroupObject lightGroup)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	
	if (lightGroup != NULL)
		{
		Q3_REQUIRE_OR_RESULT(lightGroup->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
		Q3_REQUIRE_OR_RESULT(Q3Object_IsType(lightGroup, kQ3ShapeTypeGroup), kQ3Failure);
		}



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on lightGroup (may be NULL)
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_SetLightGroup(view, lightGroup));
}





//=============================================================================
//      Q3View_GetLightGroup : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetLightGroup(TQ3ViewObject view, TQ3GroupObject *lightGroup)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lightGroup), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on lightGroup
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetLightGroup(view, lightGroup));
}





//=============================================================================
//      Q3View_SetIdleMethod : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SetIdleMethod(TQ3ViewObject view, TQ3ViewIdleMethod idleMethod, const void *idleData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on idleMethod
		return(kQ3Failure);

	if (0) // Further checks on idleData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_SetIdleMethod(view, idleMethod, idleData));
}





//=============================================================================
//      Q3View_SetIdleProgressMethod : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SetIdleProgressMethod(TQ3ViewObject view, TQ3ViewIdleProgressMethod idleMethod, const void *idleData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(idleData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on idleMethod
		return(kQ3Failure);

	if (0) // Further checks on idleData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_SetIdleProgressMethod(view, idleMethod, idleData));
}





//=============================================================================
//      Q3View_SetEndFrameMethod : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SetEndFrameMethod(TQ3ViewObject view, TQ3ViewEndFrameMethod endFrame, void *endFrameData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(endFrameData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on endFrame
		return(kQ3Failure);

	if (0) // Further checks on endFrameData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_SetEndFrameMethod(view, endFrame, endFrameData));
}



#pragma mark -

//=============================================================================
//      Q3Push_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Push_Submit(TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Push_Submit(view));
}





//=============================================================================
//      Q3Pop_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pop_Submit(TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pop_Submit(view));
}





//=============================================================================
//      Q3Push_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StateOperatorObject
Q3Push_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Push_New());
}





//=============================================================================
//      Q3Pop_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StateOperatorObject
Q3Pop_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pop_New());
}





//=============================================================================
//      Q3StateOperator_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3StateOperator_Submit(TQ3StateOperatorObject stateOperator, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(stateOperator->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(stateOperator, kQ3ShapeTypeStateOperator), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on stateOperator
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3StateOperator_Submit(stateOperator, view));
}


#pragma mark -


//=============================================================================
//      Q3View_IsBoundingBoxVisible : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3View_IsBoundingBoxVisible(TQ3ViewObject view, const TQ3BoundingBox *bbox)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bbox), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3False);

	if (0) // Further checks on bbox
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_IsBoundingBoxVisible(view, bbox));
}





//=============================================================================
//      Q3View_AllowAllGroupCulling : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_AllowAllGroupCulling(TQ3ViewObject view, TQ3Boolean allowCulling)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on allowCulling
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_AllowAllGroupCulling(view, allowCulling));
}





//=============================================================================
//      Q3View_SetDrawContext : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SetDrawContext(TQ3ViewObject view, TQ3DrawContextObject drawContext)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(drawContext->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(drawContext, kQ3SharedTypeDrawContext), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on drawContext
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_SetDrawContext(view, drawContext));
}





//=============================================================================
//      Q3View_GetDrawContext : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetDrawContext(TQ3ViewObject view, TQ3DrawContextObject *drawContext)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(drawContext), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on drawContext
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetDrawContext(view, drawContext));
}





//=============================================================================
//      Q3View_GetLocalToWorldMatrixState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetLocalToWorldMatrixState(TQ3ViewObject view, TQ3Matrix4x4 *matrix)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on matrix
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetLocalToWorldMatrixState(view, matrix));
}





//=============================================================================
//      Q3View_GetWorldToFrustumMatrixState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetWorldToFrustumMatrixState(TQ3ViewObject view, TQ3Matrix4x4 *matrix)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on matrix
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetWorldToFrustumMatrixState(view, matrix));
}





//=============================================================================
//      Q3View_GetFrustumToWindowMatrixState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetFrustumToWindowMatrixState(TQ3ViewObject view, TQ3Matrix4x4 *matrix)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on matrix
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetFrustumToWindowMatrixState(view, matrix));
}





//=============================================================================
//      Q3View_GetBackfacingStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetBackfacingStyleState(TQ3ViewObject view, TQ3BackfacingStyle *backfacingStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(backfacingStyle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on backfacingStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetBackfacingStyleState(view, backfacingStyle));
}





//=============================================================================
//      Q3View_GetInterpolationStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetInterpolationStyleState(TQ3ViewObject view, TQ3InterpolationStyle *interpolationType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(interpolationType), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on interpolationType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetInterpolationStyleState(view, interpolationType));
}





//=============================================================================
//      Q3View_GetFillStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetFillStyleState(TQ3ViewObject view, TQ3FillStyle *fillStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fillStyle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on fillStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetFillStyleState(view, fillStyle));
}





//=============================================================================
//      Q3View_GetHighlightStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetHighlightStyleState(TQ3ViewObject view, TQ3AttributeSet *highlightStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(highlightStyle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on highlightStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetHighlightStyleState(view, highlightStyle));
}





//=============================================================================
//      Q3View_GetSubdivisionStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetSubdivisionStyleState(TQ3ViewObject view, TQ3SubdivisionStyleData *subdivisionStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(subdivisionStyle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on subdivisionStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetSubdivisionStyleState(view, subdivisionStyle));
}





//=============================================================================
//      Q3View_GetOrientationStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetOrientationStyleState(TQ3ViewObject view, TQ3OrientationStyle *fontFacingDirectionStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fontFacingDirectionStyle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on fontFacingDirectionStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetOrientationStyleState(view, fontFacingDirectionStyle));
}





//=============================================================================
//      Q3View_GetReceiveShadowsStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetReceiveShadowsStyleState(TQ3ViewObject view, TQ3Boolean *receives)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(receives), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on receives
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetReceiveShadowsStyleState(view, receives));
}





//=============================================================================
//      Q3View_GetPickIDStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetPickIDStyleState(TQ3ViewObject view, TQ3Uns32 *pickIDStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pickIDStyle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on pickIDStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetPickIDStyleState(view, pickIDStyle));
}





//=============================================================================
//      Q3View_GetPickPartsStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetPickPartsStyleState(TQ3ViewObject view, TQ3PickParts *pickPartsStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pickPartsStyle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on pickPartsStyle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetPickPartsStyleState(view, pickPartsStyle));
}





//=============================================================================
//      Q3View_GetAntiAliasStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetAntiAliasStyleState(TQ3ViewObject view, TQ3AntiAliasStyleData *antiAliasData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(antiAliasData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on antiAliasData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetAntiAliasStyleState(view, antiAliasData));
}





//=============================================================================
//      Q3View_GetFogStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetFogStyleState(TQ3ViewObject view, TQ3FogStyleData *fogData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fogData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on fogData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetFogStyleState(view, fogData));
}





//=============================================================================
//      Q3View_GetDefaultAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetDefaultAttributeSet(TQ3ViewObject view, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetDefaultAttributeSet(view, attributeSet));
}





//=============================================================================
//      Q3View_SetDefaultAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SetDefaultAttributeSet(TQ3ViewObject view, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_SetDefaultAttributeSet(view, attributeSet));
}





//=============================================================================
//      Q3View_GetAttributeSetState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetAttributeSetState(TQ3ViewObject view, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetAttributeSetState(view, attributeSet));
}





//=============================================================================
//      Q3View_GetAttributeState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetAttributeState(TQ3ViewObject view, TQ3AttributeType attributeType, void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on attributeType
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetAttributeState(view, attributeType, data));
}






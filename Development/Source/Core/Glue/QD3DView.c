/*  NAME:
        QD3DView.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
//      Q3View_NewWithDefaults : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewObject
Q3View_NewWithDefaults(TQ3ObjectType drawContextType, void *drawContextTarget)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(drawContextType == kQ3DrawContextTypePixmap    ||
						 drawContextType == kQ3DrawContextTypeMacintosh ||
						 drawContextType == kQ3DrawContextTypeCocoa     ||
						 drawContextType == kQ3DrawContextTypeWin32DC, NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(drawContextTarget), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContextType
		return(NULL);

	if (0) // Further checks on drawContextTarget
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_NewWithDefaults(drawContextType, drawContextTarget));
}





//=============================================================================
//      Q3View_Cancel : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_Cancel(TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, (kQ3SharedTypeRenderer)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, (kQ3ObjectTypePick)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, (kQ3ShapeTypeCamera)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
	
	if (lightGroup != NULL)
		Q3_REQUIRE_OR_RESULT(Q3Object_IsType(lightGroup, kQ3ShapeTypeGroup), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
//      Q3View_AddLight : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_AddLight(TQ3ViewObject theView, TQ3ObjectType lightType, void *lightData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theView, (kQ3ObjectTypeView)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(lightType == kQ3ShapeTypeLight       ||
						 lightType == kQ3LightTypeAmbient     ||
						 lightType == kQ3LightTypeDirectional ||
						 lightType == kQ3LightTypePoint       ||
						 lightType == kQ3LightTypeSpot, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lightData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theView
		return(kQ3Failure);

	if (0) // Further checks on lightType
		return(kQ3Failure);

	if (0) // Further checks on lightData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_AddLight(theView, lightType, lightData));
}





//=============================================================================
//      Q3View_SetIdleMethod : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SetIdleMethod(TQ3ViewObject view, TQ3ViewIdleMethod idleMethod, const void *idleData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on idleMethod
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on idleMethod
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on endFrame
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(stateOperator, (kQ3ShapeTypeStateOperator)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
//      Q3View_TransformLocalToWorld : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_TransformLocalToWorld(TQ3ViewObject theView, const TQ3Point3D *localPoint, TQ3Point3D *worldPoint)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theView, (kQ3ObjectTypeView)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(localPoint), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(worldPoint), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theView
		return(kQ3Failure);

	if (0) // Further checks on localPoint
		return(kQ3Failure);

	if (0) // Further checks on worldPoint
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_TransformLocalToWorld(theView, localPoint, worldPoint));
}





//=============================================================================
//      Q3View_TransformLocalToWindow : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_TransformLocalToWindow(TQ3ViewObject theView, const TQ3Point3D *localPoint, TQ3Point2D *windowPoint)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theView, (kQ3ObjectTypeView)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(localPoint), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(windowPoint), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theView
		return(kQ3Failure);

	if (0) // Further checks on localPoint
		return(kQ3Failure);

	if (0) // Further checks on windowPoint
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_TransformLocalToWindow(theView, localPoint, windowPoint));
}





//=============================================================================
//      Q3View_TransformWorldToWindow : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_TransformWorldToWindow(TQ3ViewObject theView, const TQ3Point3D *worldPoint, TQ3Point2D *windowPoint)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theView, (kQ3ObjectTypeView)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(worldPoint), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(windowPoint), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theView
		return(kQ3Failure);

	if (0) // Further checks on worldPoint
		return(kQ3Failure);

	if (0) // Further checks on windowPoint
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_TransformWorldToWindow(theView, worldPoint, windowPoint));
}





//=============================================================================
//      Q3View_SetDrawContext : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SetDrawContext(TQ3ViewObject view, TQ3DrawContextObject drawContext)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(drawContext, (kQ3SharedTypeDrawContext)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
//      Q3View_GetCastShadowsStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetCastShadowsStyleState(TQ3ViewObject view, TQ3Boolean *castShadows)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(castShadows), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on castShadows
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetCastShadowsStyleState(view, castShadows));
}





//=============================================================================
//      Q3View_GetReceiveShadowsStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetReceiveShadowsStyleState(TQ3ViewObject view, TQ3Boolean *receiveShadows)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(receiveShadows), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on receiveShadows
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3View_GetReceiveShadowsStyleState(view, receiveShadows));
}





//=============================================================================
//      Q3View_GetPickIDStyleState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_GetPickIDStyleState(TQ3ViewObject view, TQ3Uns32 *pickIDStyle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);
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






/*  NAME:
        E3View.h

    DESCRIPTION:
        Header file for E3View.c.

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
#ifndef E3VIEW_HDR
#define E3VIEW_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here




//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// View mode (current behaviour)
typedef enum TQ3ViewMode {
	kQ3ViewModeInactive	             		= 0,		// View not doing anythig
	kQ3ViewModeDrawing		         		= 1,		// Performing drawing
	kQ3ViewModePicking		         		= 2,		// Performing picking
	kQ3ViewModeWriting		    			= 3,		// Performing I/O
	kQ3ViewModeCalcBounds			 		= 4			// Calculating bounds
} TQ3ViewMode;


// View state (activity within current behaviour)
typedef enum TQ3ViewState {
	kQ3ViewStateInactive   					= 0,		// View not doing anything
	kQ3ViewStateCancelled  					= 1,		// View cancelled
	kQ3ViewStateSubmitting 					= 2			// Inside submit loop
} TQ3ViewState;


// Bounding method (activity within current behaviour)
typedef enum TQ3BoundingMethod {
	kQ3BoxBoundsExact						= 0,		// Bounding box, slow
	kQ3BoxBoundsApprox						= 1,		// Bounding box, fast
	kQ3SphereBoundsExact					= 2,		// Bounding sphere, slow
	kQ3SphereBoundsApprox					= 3			// Bounding sphere, fast
} TQ3BoundingMethod;


// Matrix state
enum {
	kQ3MatrixStateNone						= 0,
	kQ3MatrixStateLocalToWorld				= (1 << 0),	// Local  to world   changed
	kQ3MatrixStateWorldToCamera				= (1 << 1),	// World  to camera  changed
	kQ3MatrixStateCameraToFrustum			= (1 << 2)	// Camera to frustum changed
};

typedef TQ3Uns32	TQ3MatrixState;




//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status				E3View_RegisterClass(void);
TQ3Status				E3View_UnregisterClass(void);
TQ3Status				E3View_SubmitRetained(TQ3ViewObject theView, TQ3Object theObject);
TQ3Status				E3View_SubmitImmediate(TQ3ViewObject theView, TQ3ObjectType objectType, const void *objectData);
TQ3Status				E3View_CallIdleMethod(TQ3ViewObject theView, TQ3Uns32 current, TQ3Uns32 completed);
TQ3PickObject			E3View_AccessPick(TQ3ViewObject theView);
TQ3RendererObject		E3View_AccessRenderer(TQ3ViewObject theView);
TQ3DrawContextObject	E3View_AccessDrawContext(TQ3ViewObject theView);
TQ3FileObject			E3View_AccessFile(TQ3ViewObject theView);
TQ3FileFormatObject		E3View_AccessFileFormat(TQ3ViewObject theView);
TQ3CameraObject			E3View_AccessCamera(TQ3ViewObject theView);
void					E3View_EndFrame(TQ3ViewObject theView);
TQ3ViewMode				E3View_GetViewMode(TQ3ViewObject theView);
TQ3ViewState			E3View_GetViewState(TQ3ViewObject theView);
TQ3BoundingMethod		E3View_GetBoundingMethod(TQ3ViewObject theView);
void					E3View_GetRayThroughPickPoint(TQ3ViewObject theView, TQ3Ray3D *theRay);
void					E3View_UpdateBounds(TQ3ViewObject theView, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints);
TQ3Status				E3View_PickStack_PushGroup(TQ3ViewObject theView, TQ3GroupObject theGroup);
TQ3HitPath				*E3View_PickStack_GetPickedPath(TQ3ViewObject theView);
TQ3Object				E3View_PickStack_GetPickedObject(TQ3ViewObject theView);
void					E3View_PickStack_SavePosition(TQ3ViewObject theView, TQ3GroupPosition thePosition);
void					E3View_PickStack_SaveObject(TQ3ViewObject theView, TQ3Object theObject);
void					E3View_PickStack_BeginDecomposedObject(TQ3ViewObject theView);
void					E3View_PickStack_EndDecomposedObject(TQ3ViewObject theView);
void					E3View_PickStack_PopGroup(TQ3ViewObject theView);

TQ3Status						E3View_State_AddMatrixLocalToWorld(TQ3ViewObject theView, const TQ3Matrix4x4 *theMatrix);
const TQ3Matrix4x4				*E3View_State_GetMatrixLocalToWorld(TQ3ViewObject theView);
const TQ3SubdivisionStyleData	*E3View_State_GetStyleSubdivision(TQ3ViewObject theView);
TQ3OrientationStyle				E3View_State_GetStyleOrientation(TQ3ViewObject theView);
TQ3Status						E3View_State_SetMatrix(TQ3ViewObject theView, TQ3MatrixState theState, const TQ3Matrix4x4 *localToWorld, const TQ3Matrix4x4 *worldToCamera, const TQ3Matrix4x4 *cameraToFrustum);
void							E3View_State_SetShaderIllumination(TQ3ViewObject theView, const TQ3IlluminationShaderObject theData);
void							E3View_State_SetShaderSurface(TQ3ViewObject theView, const TQ3SurfaceShaderObject theData);
void							E3View_State_SetStyleSubdivision(TQ3ViewObject theView, const TQ3SubdivisionStyleData *theData);
void							E3View_State_SetStylePickID(TQ3ViewObject theView, TQ3Uns32 pickID);
void							E3View_State_SetStylePickParts(TQ3ViewObject theView, TQ3PickParts pickParts);
void							E3View_State_SetStyleCastShadows(TQ3ViewObject theView, TQ3Boolean castShadows);
void							E3View_State_SetStyleReceiveShadows(TQ3ViewObject theView, TQ3Boolean receiveShadows);
void							E3View_State_SetStyleFill(TQ3ViewObject theView, TQ3FillStyle fillStyle);
void							E3View_State_SetStyleBackfacing(TQ3ViewObject theView, TQ3BackfacingStyle backfacingStyle);
void							E3View_State_SetStyleInterpolation(TQ3ViewObject theView, TQ3InterpolationStyle interpolationStyle);
void							E3View_State_SetStyleHighlight(TQ3ViewObject theView, TQ3AttributeSet interpolationStyle);
void							E3View_State_SetStyleOrientation(TQ3ViewObject theView, TQ3OrientationStyle frontFacingDirection);
void							E3View_State_SetStyleAntiAlias(TQ3ViewObject theView, const TQ3AntiAliasStyleData *theData);
void							E3View_State_SetStyleFog(TQ3ViewObject theView, const TQ3FogStyleData *theData);
void							E3View_State_SetAttributeSurfaceUV(TQ3ViewObject theView, const TQ3Param2D *theData);
void							E3View_State_SetAttributeShadingUV(TQ3ViewObject theView, const TQ3Param2D *theData);
void							E3View_State_SetAttributeNormal(TQ3ViewObject theView, const TQ3Vector3D *theData);
void							E3View_State_SetAttributeAmbientCoefficient(TQ3ViewObject theView, const float *theData);
void							E3View_State_SetAttributeDiffuseColor(TQ3ViewObject theView, const TQ3ColorRGB *theData);
void							E3View_State_SetAttributeSpecularColor(TQ3ViewObject theView, const TQ3ColorRGB *theData);
void							E3View_State_SetAttributeSpecularControl(TQ3ViewObject theView, const float *theData);
void							E3View_State_SetAttributeTransparencyColor(TQ3ViewObject theView, const TQ3ColorRGB *theData);
void							E3View_State_SetAttributeSurfaceTangent(TQ3ViewObject theView, const TQ3Tangent2D *theData);
void							E3View_State_SetAttributeHighlightState(TQ3ViewObject theView, const TQ3Switch *theData);
void							E3View_State_SetAttributeSurfaceShader(TQ3ViewObject theView, const TQ3SurfaceShaderObject *theData);

TQ3ViewObject			E3View_New(void);
TQ3ViewObject			E3View_NewWithDefaults(TQ3ObjectType drawContextType, void *drawContextTarget);
TQ3Status				E3View_Cancel(TQ3ViewObject theView);
TQ3Status				E3View_SetRendererByType(TQ3ViewObject theView, TQ3ObjectType theType);
TQ3Status				E3View_SetRenderer(TQ3ViewObject theView, TQ3RendererObject theRenderer);
TQ3Status				E3View_GetRenderer(TQ3ViewObject theView, TQ3RendererObject *theRenderer);
TQ3Status				E3View_StartRendering(TQ3ViewObject theView);
TQ3ViewStatus			E3View_EndRendering(TQ3ViewObject theView);
TQ3Status				E3View_Flush(TQ3ViewObject theView);
TQ3Status				E3View_Sync(TQ3ViewObject theView);
TQ3Status				E3View_StartBoundingBox(TQ3ViewObject theView, TQ3ComputeBounds computeBounds);
TQ3ViewStatus			E3View_EndBoundingBox(TQ3ViewObject theView, TQ3BoundingBox *result);
TQ3Status				E3View_StartBoundingSphere(TQ3ViewObject theView, TQ3ComputeBounds computeBounds);
TQ3ViewStatus			E3View_EndBoundingSphere(TQ3ViewObject theView, TQ3BoundingSphere *result);
TQ3Status				E3View_StartPicking(TQ3ViewObject theView, TQ3PickObject pick);
TQ3ViewStatus			E3View_EndPicking(TQ3ViewObject theView);
TQ3Status				E3View_StartWriting(TQ3ViewObject view, TQ3FileObject theFile);
TQ3ViewStatus			E3View_EndWriting(TQ3ViewObject view);
TQ3Status				E3View_GetCamera(TQ3ViewObject theView, TQ3CameraObject *theCamera);
TQ3Status				E3View_SetCamera(TQ3ViewObject theView, TQ3CameraObject theCamera);
TQ3Status				E3View_SetLightGroup(TQ3ViewObject theView, TQ3GroupObject lightGroup);
TQ3Status				E3View_GetLightGroup(TQ3ViewObject theView, TQ3GroupObject *lightGroup);
TQ3Status				E3View_AddLight(TQ3ViewObject theView, TQ3ObjectType lightType, void *lightData);
TQ3Status				E3View_SetIdleMethod(TQ3ViewObject theView, TQ3ViewIdleMethod idleMethod, const void *idleData);
TQ3Status				E3View_SetIdleProgressMethod(TQ3ViewObject theView, TQ3ViewIdleProgressMethod idleMethod, const void *idleData);
TQ3Status				E3View_SetEndFrameMethod(TQ3ViewObject theView, TQ3ViewEndFrameMethod endFrame, void *endFrameData);
TQ3Boolean				E3View_IsBoundingBoxVisible(TQ3ViewObject theView, const TQ3BoundingBox *theBBox);
TQ3Status				E3View_AllowAllGroupCulling(TQ3ViewObject theView, TQ3Boolean allowCulling);
TQ3Status				E3View_TransformLocalToWorld(TQ3ViewObject theView, const TQ3Point3D *localPoint, TQ3Point3D *worldPoint);
TQ3Status				E3View_TransformLocalToWindow(TQ3ViewObject theView, const TQ3Point3D *localPoint, TQ3Point2D *windowPoint);
TQ3Status				E3View_TransformWorldToWindow(TQ3ViewObject theView, const TQ3Point3D *worldPoint, TQ3Point2D *windowPoint);
TQ3Status				E3View_SetDrawContext(TQ3ViewObject theView, TQ3DrawContextObject drawContext);
TQ3Status				E3View_GetDrawContext(TQ3ViewObject theView, TQ3DrawContextObject *drawContext);
TQ3Status				E3View_GetLocalToWorldMatrixState(TQ3ViewObject theView, TQ3Matrix4x4 *theMatrix);
TQ3Status				E3View_GetWorldToFrustumMatrixState(TQ3ViewObject theView, TQ3Matrix4x4 *theMatrix);
TQ3Status				E3View_GetFrustumToWindowMatrixState(TQ3ViewObject theView, TQ3Matrix4x4 *theMatrix);
TQ3Status				E3View_GetBackfacingStyleState(TQ3ViewObject theView, TQ3BackfacingStyle *backfacingStyle);
TQ3Status				E3View_GetInterpolationStyleState(TQ3ViewObject theView, TQ3InterpolationStyle *interpolationType);
TQ3Status				E3View_GetFillStyleState(TQ3ViewObject theView, TQ3FillStyle *fillStyle);
TQ3Status				E3View_GetHighlightStyleState(TQ3ViewObject theView, TQ3AttributeSet *highlightStyle);
TQ3Status				E3View_GetSubdivisionStyleState(TQ3ViewObject theView, TQ3SubdivisionStyleData *subdivisionStyle);
TQ3Status				E3View_GetOrientationStyleState(TQ3ViewObject theView, TQ3OrientationStyle *frontFacingDirectionStyle);
TQ3Status				E3View_GetCastShadowsStyleState(TQ3ViewObject theView, TQ3Boolean *castShadows);
TQ3Status				E3View_GetReceiveShadowsStyleState(TQ3ViewObject theView, TQ3Boolean *receiveShadows);
TQ3Status				E3View_GetPickIDStyleState(TQ3ViewObject theView, TQ3Uns32 *pickIDStyle);
TQ3Status				E3View_GetPickPartsStyleState(TQ3ViewObject theView, TQ3PickParts *pickPartsStyle);
TQ3Status				E3View_GetAntiAliasStyleState(TQ3ViewObject theView, TQ3AntiAliasStyleData *antiAliasData);
TQ3Status				E3View_GetFogStyleState(TQ3ViewObject theView, TQ3FogStyleData *fogData);
TQ3Status				E3View_GetDefaultAttributeSet(TQ3ViewObject theView, TQ3AttributeSet *attributeSet);
TQ3Status				E3View_SetDefaultAttributeSet(TQ3ViewObject theView, TQ3AttributeSet attributeSet);
TQ3Status				E3View_GetAttributeSetState(TQ3ViewObject theView, TQ3AttributeSet *attributeSet);
TQ3Status				E3View_GetAttributeState(TQ3ViewObject theView, TQ3AttributeType attributeType, void *data);
TQ3Boolean				E3View_IsOfMyClass ( TQ3Object object ) ;

TQ3Status				E3Push_Submit(TQ3ViewObject theView);
TQ3Status				E3Pop_Submit(TQ3ViewObject theView);
TQ3StateOperatorObject	E3Push_New(void);
TQ3StateOperatorObject	E3Pop_New(void);
TQ3Status				E3StateOperator_Submit(TQ3StateOperatorObject stateOperator, TQ3ViewObject theView);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


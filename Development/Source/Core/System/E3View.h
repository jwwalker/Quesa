/*  NAME:
        E3View.h

    DESCRIPTION:
        Header file for E3View.c.

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
#ifndef __E3VIEW__
#define __E3VIEW__
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
enum TQ3ViewMode {
	kQ3ViewModeInactive	             		= 0,		// View not doing anythig
	kQ3ViewModeDrawing		         		= 1,		// Performing drawing
	kQ3ViewModePicking		         		= 2,		// Performing picking
	kQ3ViewModeWriting		    			= 3,		// Performing I/O
	kQ3ViewModeCalcBounds			 		= 4			// Calculating bounds
};
typedef enum TQ3ViewMode TQ3ViewMode;


// View state (activity within current behaviour)
enum TQ3ViewState {
	kQ3ViewStateInactive   					= 0,		// View not doing anything
	kQ3ViewStateCancelled  					= 1,		// View cancelled
	kQ3ViewStateSubmitting 					= 2			// Inside submit loop
};
typedef enum TQ3ViewState TQ3ViewState;

// Bounding state (activity within current behaviour)
enum TQ3BoundingMethod {
	kQ3BoxBoundsExact					= 0,			// Bounding box, slow
	kQ3BoxBoundsApprox					= 1,			// Bounding box, fast
	kQ3SphereBoundsExact				= 2,			// Bounding sphere, slow
	kQ3SphereBoundsApprox				= 3				// Bounding sphere, fast
};
typedef enum TQ3BoundingMethod TQ3BoundingMethod;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status				E3View_RegisterClass(void);
TQ3Status				E3View_UnregisterClass(void);
TQ3Status				E3View_SubmitRetained(TQ3ViewObject theView, TQ3Object theObject);
TQ3Status				E3View_SubmitImmediate(TQ3ViewObject theView, TQ3ObjectType objectType, const void *objectData);
TQ3Status				E3View_CallIdleMethod(TQ3ViewObject theView, TQ3Uns32 current, TQ3Uns32 completed);
TQ3RendererObject		E3View_AccessRenderer(TQ3ViewObject theView);
TQ3CameraObject			E3View_AccessCamera(TQ3ViewObject theView);
TQ3FileFormatObject		E3View_AccessFileFormat(TQ3ViewObject theView);
void					E3View_EndFrame(TQ3ViewObject theView);
TQ3ViewMode				E3View_GetViewMode(TQ3ViewObject theView);
TQ3ViewState			E3View_GetViewState(TQ3ViewObject theView);
TQ3BoundingMethod		E3View_GetBoundingMethod(TQ3ViewObject theView);
void					E3View_UpdateBounds(TQ3ViewObject theView, TQ3Uns32 numPoints, TQ3Uns32 pointStride, const TQ3Point3D *thePoints);

void					E3View_State_InitGroupMatrix(TQ3ViewObject theView);
TQ3Status				E3View_State_AddMatrixLocalToWorld(TQ3ViewObject theView, const TQ3Matrix4x4 *theMatrix);

const TQ3Matrix4x4				*E3View_State_GetLocalToWorld(TQ3ViewObject theView);
const TQ3SubdivisionStyleData	*E3View_State_GetStyleSubdivision(TQ3ViewObject theView);

void					E3View_State_SetShaderSurface(TQ3ViewObject theView, const TQ3SurfaceShaderObject theData);
void					E3View_State_SetShaderIllumination(TQ3ViewObject theView, const TQ3IlluminationShaderObject theData);
void					E3View_State_SetStyleSubdivision(TQ3ViewObject theView, const TQ3SubdivisionStyleData *theData);
void					E3View_State_SetStylePickID(TQ3ViewObject theView, TQ3Uns32 pickID);
void					E3View_State_SetStylePickParts(TQ3ViewObject theView, TQ3PickParts pickParts);
void					E3View_State_SetStyleReceiveShadows(TQ3ViewObject theView, TQ3Boolean receiveShadows);
void					E3View_State_SetStyleFill(TQ3ViewObject theView, TQ3FillStyle fillStyle);
void					E3View_State_SetStyleBackfacing(TQ3ViewObject theView, TQ3BackfacingStyle backfacingStyle);
void					E3View_State_SetStyleInterpolation(TQ3ViewObject theView, TQ3InterpolationStyle interpolationStyle);
void					E3View_State_SetStyleOrientation(TQ3ViewObject theView, TQ3OrientationStyle frontFacingDirection);
void					E3View_State_SetStyleAntiAlias(TQ3ViewObject theView, const TQ3AntiAliasStyleData *theData);
void					E3View_State_SetStyleFog(TQ3ViewObject theView, const TQ3FogStyleData *theData);
void					E3View_State_SetAttributeSurfaceUV(TQ3ViewObject theView, const TQ3Param2D *theData);
void					E3View_State_SetAttributeShadingUV(TQ3ViewObject theView, const TQ3Param2D *theData);
void					E3View_State_SetAttributeNormal(TQ3ViewObject theView, const TQ3Vector3D *theData);
void					E3View_State_SetAttributeAmbientCoefficient(TQ3ViewObject theView, const float *theData);
void					E3View_State_SetAttributeDiffuseColor(TQ3ViewObject theView, const TQ3ColorRGB *theData);
void					E3View_State_SetAttributeSpecularColor(TQ3ViewObject theView, const TQ3ColorRGB *theData);
void					E3View_State_SetAttributeSpecularControl(TQ3ViewObject theView, const float *theData);
void					E3View_State_SetAttributeTransparencyColor(TQ3ViewObject theView, const TQ3ColorRGB *theData);
void					E3View_State_SetAttributeSurfaceTangent(TQ3ViewObject theView, const TQ3Tangent2D *theData);
void					E3View_State_SetAttributeHighlightState(TQ3ViewObject theView, const TQ3Switch *theData);
void					E3View_State_SetAttributeSurfaceShader(TQ3ViewObject theView, const TQ3SurfaceShaderObject *theData);

TQ3ViewObject			E3View_New(void);
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
TQ3Status				E3View_GetCamera(TQ3ViewObject theView, TQ3CameraObject *theCamera);
TQ3Status				E3View_SetCamera(TQ3ViewObject theView, TQ3CameraObject theCamera);
TQ3Status				E3View_SetLightGroup(TQ3ViewObject theView, TQ3GroupObject lightGroup);
TQ3Status				E3View_GetLightGroup(TQ3ViewObject theView, TQ3GroupObject *lightGroup);
TQ3Status				E3View_SetIdleMethod(TQ3ViewObject theView, TQ3ViewIdleMethod idleMethod, const void *idleData);
TQ3Status				E3View_SetIdleProgressMethod(TQ3ViewObject theView, TQ3ViewIdleProgressMethod idleMethod, const void *idleData);
TQ3Status				E3View_SetEndFrameMethod(TQ3ViewObject theView, TQ3ViewEndFrameMethod endFrame, void *endFrameData);
TQ3Boolean				E3View_IsBoundingBoxVisible(TQ3ViewObject theView, const TQ3BoundingBox *theBBox);
TQ3Status				E3View_AllowAllGroupCulling(TQ3ViewObject theView, TQ3Boolean allowCulling);
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
TQ3Status				E3View_GetReceiveShadowsStyleState(TQ3ViewObject theView, TQ3Boolean *receives);
TQ3Status				E3View_GetPickIDStyleState(TQ3ViewObject theView, unsigned long *pickIDStyle);
TQ3Status				E3View_GetPickPartsStyleState(TQ3ViewObject theView, TQ3PickParts *pickPartsStyle);
TQ3Status				E3View_GetAntiAliasStyleState(TQ3ViewObject theView, TQ3AntiAliasStyleData *antiAliasData);
TQ3Status				E3View_GetDefaultAttributeSet(TQ3ViewObject theView, TQ3AttributeSet *attributeSet);
TQ3Status				E3View_SetDefaultAttributeSet(TQ3ViewObject theView, TQ3AttributeSet attributeSet);
TQ3Status				E3View_GetAttributeSetState(TQ3ViewObject theView, TQ3AttributeSet *attributeSet);
TQ3Status				E3View_GetAttributeState(TQ3ViewObject theView, TQ3AttributeType attributeType, void *data);

TQ3Status				E3Push_Submit(TQ3ViewObject theView);
TQ3Status				E3Pop_Submit(TQ3ViewObject theView);
TQ3StateOperatorObject	E3Push_New(void);
TQ3StateOperatorObject	E3Pop_New(void);
TQ3Status				E3StateOperator_Submit(TQ3StateOperatorObject stateOperator, TQ3ViewObject theView);


TQ3Status				E3View_StartWriting(TQ3ViewObject view, TQ3FileObject theFile);
TQ3ViewStatus			E3View_EndWriting(TQ3ViewObject view);



//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


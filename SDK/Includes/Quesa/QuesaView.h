/*! @header QuesaView.h
        Declares the Quesa view object.
 */
/*  NAME:
        QuesaView.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_VIEW_HDR
#define QUESA_VIEW_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

#include "QuesaStyle.h"
#include "QuesaSet.h"

// Disable QD3D header
#ifdef __QD3DVIEW__
#error
#endif

#define __QD3DVIEW__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint on

#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// View status
typedef enum {
    kQ3ViewStatusDone                           = 0,
    kQ3ViewStatusRetraverse                     = 1,
    kQ3ViewStatusError                          = 2,
    kQ3ViewStatusCancelled                      = 3
} TQ3ViewStatus;


// Default attribute values
#define kQ3ViewDefaultAmbientCoefficient        1.0f
#define kQ3ViewDefaultDiffuseColor              1.0f, 1.0f, 1.0f
#define kQ3ViewDefaultSpecularColor             0.5f, 0.5f, 0.5f
#define kQ3ViewDefaultSpecularControl           4.0f
#define kQ3ViewDefaultTransparency              1.0f, 1.0f, 1.0f
#define kQ3ViewDefaultHighlightState            kQ3Off
#define kQ3ViewDefaultHighlightColor            1.0f, 0.0f, 0.0f
#define kQ3ViewDefaultSubdivisionMethod         kQ3SubdivisionMethodScreenSpace
#define kQ3ViewDefaultSubdivisionC1             20.0f
#define kQ3ViewDefaultSubdivisionC2             20.0f





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// View callbacks
typedef CALLBACK_API_C(TQ3Status,           TQ3ViewIdleMethod)(
                            TQ3ViewObject       theView,
                            const void          *idlerData);
                            
typedef CALLBACK_API_C(TQ3Status,           TQ3ViewIdleProgressMethod)(
                            TQ3ViewObject       theView,
                            const void          *idlerData,
                            TQ3Uns32            progressCurrent,
                            TQ3Uns32            progressCompleted);
                            
typedef CALLBACK_API_C(void,                TQ3ViewEndFrameMethod)(
                            TQ3ViewObject       theView,
                            void                *endFrameData);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3View_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ViewObject  )
Q3View_New (
    void
);



/*!
 *  @function
 *      Q3View_Cancel
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_Cancel (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3View_SetRendererByType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param theType          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetRendererByType (
    TQ3ViewObject                 view,
    TQ3ObjectType                 theType
);



/*!
 *  @function
 *      Q3View_SetRenderer
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param renderer         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetRenderer (
    TQ3ViewObject                 view,
    TQ3RendererObject             renderer
);



/*!
 *  @function
 *      Q3View_GetRenderer
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param renderer         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetRenderer (
    TQ3ViewObject                 view,
    TQ3RendererObject             *renderer
);



/*!
 *  @function
 *      Q3View_StartRendering
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_StartRendering (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3View_EndRendering
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ViewStatus  )
Q3View_EndRendering (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3View_Flush
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_Flush (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3View_Sync
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_Sync (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3View_StartBoundingBox
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param computeBounds    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_StartBoundingBox (
    TQ3ViewObject                 view,
    TQ3ComputeBounds              computeBounds
);



/*!
 *  @function
 *      Q3View_EndBoundingBox
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ViewStatus  )
Q3View_EndBoundingBox (
    TQ3ViewObject                 view,
    TQ3BoundingBox                *result
);



/*!
 *  @function
 *      Q3View_StartBoundingSphere
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param computeBounds    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_StartBoundingSphere (
    TQ3ViewObject                 view,
    TQ3ComputeBounds              computeBounds
);



/*!
 *  @function
 *      Q3View_EndBoundingSphere
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ViewStatus  )
Q3View_EndBoundingSphere (
    TQ3ViewObject                 view,
    TQ3BoundingSphere             *result
);



/*!
 *  @function
 *      Q3View_StartPicking
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param pick             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_StartPicking (
    TQ3ViewObject                 view,
    TQ3PickObject                 pick
);



/*!
 *  @function
 *      Q3View_EndPicking
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ViewStatus  )
Q3View_EndPicking (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3View_GetCamera
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param camera           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetCamera (
    TQ3ViewObject                 view,
    TQ3CameraObject               *camera
);



/*!
 *  @function
 *      Q3View_SetCamera
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param camera           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetCamera (
    TQ3ViewObject                 view,
    TQ3CameraObject               camera
);



/*!
 *  @function
 *      Q3View_SetLightGroup
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param lightGroup       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetLightGroup (
    TQ3ViewObject                 view,
    TQ3GroupObject                lightGroup
);



/*!
 *  @function
 *      Q3View_GetLightGroup
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param lightGroup       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetLightGroup (
    TQ3ViewObject                 view,
    TQ3GroupObject                *lightGroup
);



/*!
 *  @function
 *      Q3View_SetIdleMethod
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param idleMethod       Description of the parameter.
 *  @param idleData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetIdleMethod (
    TQ3ViewObject                 view,
    TQ3ViewIdleMethod             idleMethod,
    const void                    *idleData
);



/*!
 *  @function
 *      Q3View_SetIdleProgressMethod
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param idleMethod       Description of the parameter.
 *  @param idleData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetIdleProgressMethod (
    TQ3ViewObject                 view,
    TQ3ViewIdleProgressMethod     idleMethod,
    const void                    *idleData
);



/*!
 *  @function
 *      Q3View_SetEndFrameMethod
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param endFrame         Description of the parameter.
 *  @param endFrameData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetEndFrameMethod (
    TQ3ViewObject                 view,
    TQ3ViewEndFrameMethod         endFrame,
    void                          *endFrameData
);



/*!
 *  @function
 *      Q3Push_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Push_Submit (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Pop_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Pop_Submit (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Push_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StateOperatorObject  )
Q3Push_New (
    void
);



/*!
 *  @function
 *      Q3Pop_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StateOperatorObject  )
Q3Pop_New (
    void
);



/*!
 *  @function
 *      Q3StateOperator_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param stateOperator    Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3StateOperator_Submit (
    TQ3StateOperatorObject        stateOperator,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3View_IsBoundingBoxVisible
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param bbox             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Boolean  )
Q3View_IsBoundingBoxVisible (
    TQ3ViewObject                 view,
    const TQ3BoundingBox          *bbox
);



/*!
 *  @function
 *      Q3View_AllowAllGroupCulling
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param allowCulling     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_AllowAllGroupCulling (
    TQ3ViewObject                 view,
    TQ3Boolean                    allowCulling
);



/*!
 *  @function
 *      Q3View_TransformLocalToWorld
 *  @discussion
 *      Transforms a point from local to world coordinates.
 *
 *      May only be called within a view submitting loop.
 *
 *      Not supported by QD3D.
 *
 *  @param theView          The view currently being submitted to.
 *  @param localPoint       The point to transform, in local coordinates.
 *  @param worldPoint       The transformed point, in world coordinates.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3View_TransformLocalToWorld (
    TQ3ViewObject                 theView,
    const TQ3Point3D              *localPoint,
    TQ3Point3D                    *worldPoint
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3View_TransformLocalToWindow
 *  @discussion
 *      Transforms a point from local to window (pixel) coordinates.
 *
 *      May only be called within a view submitting loop.
 *
 *      Not supported by QD3D.
 *
 *  @param theView          The view currently being submitted to.
 *  @param localPoint       The point to transform, in local coordinates.
 *  @param windowPoint      The transformed point, in window coordinates.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3View_TransformLocalToWindow (
    TQ3ViewObject                 theView,
    const TQ3Point3D              *localPoint,
    TQ3Point2D                    *windowPoint
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3View_TransformWorldToWindow
 *  @discussion
 *      Transforms a point from world to window (pixel) coordinates.
 *
 *      May only be called within a view submitting loop.
 *
 *      Not supported by QD3D.
 *
 *  @param theView          The view currently being submitted to.
 *  @param worldPoint       The point to transform, in world coordinates.
 *  @param windowPoint      The transformed point, in window coordinates.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3View_TransformWorldToWindow (
    TQ3ViewObject                 theView,
    const TQ3Point3D              *worldPoint,
    TQ3Point2D                    *windowPoint
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3View_SetDrawContext
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param drawContext      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetDrawContext (
    TQ3ViewObject                 view,
    TQ3DrawContextObject          drawContext
);



/*!
 *  @function
 *      Q3View_GetDrawContext
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param drawContext      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetDrawContext (
    TQ3ViewObject                 view,
    TQ3DrawContextObject          *drawContext
);



/*!
 *  @function
 *      Q3View_GetLocalToWorldMatrixState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param matrix           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetLocalToWorldMatrixState (
    TQ3ViewObject                 view,
    TQ3Matrix4x4                  *matrix
);



/*!
 *  @function
 *      Q3View_GetWorldToFrustumMatrixState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param matrix           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetWorldToFrustumMatrixState (
    TQ3ViewObject                 view,
    TQ3Matrix4x4                  *matrix
);



/*!
 *  @function
 *      Q3View_GetFrustumToWindowMatrixState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param matrix           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetFrustumToWindowMatrixState (
    TQ3ViewObject                 view,
    TQ3Matrix4x4                  *matrix
);



/*!
 *  @function
 *      Q3View_GetBackfacingStyleState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param backfacingStyle  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetBackfacingStyleState (
    TQ3ViewObject                 view,
    TQ3BackfacingStyle            *backfacingStyle
);



/*!
 *  @function
 *      Q3View_GetInterpolationStyleState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param interpolationType Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetInterpolationStyleState (
    TQ3ViewObject                 view,
    TQ3InterpolationStyle         *interpolationType
);



/*!
 *  @function
 *      Q3View_GetFillStyleState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param fillStyle        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetFillStyleState (
    TQ3ViewObject                 view,
    TQ3FillStyle                  *fillStyle
);



/*!
 *  @function
 *      Q3View_GetHighlightStyleState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param highlightStyle   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetHighlightStyleState (
    TQ3ViewObject                 view,
    TQ3AttributeSet               *highlightStyle
);



/*!
 *  @function
 *      Q3View_GetSubdivisionStyleState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param subdivisionStyle Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetSubdivisionStyleState (
    TQ3ViewObject                 view,
    TQ3SubdivisionStyleData       *subdivisionStyle
);



/*!
 *  @function
 *      Q3View_GetOrientationStyleState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param fontFacingDirectionStyle Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetOrientationStyleState (
    TQ3ViewObject                 view,
    TQ3OrientationStyle           *fontFacingDirectionStyle
);



/*!
 *  @function
 *      Q3View_GetReceiveShadowsStyleState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param receives         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetReceiveShadowsStyleState (
    TQ3ViewObject                 view,
    TQ3Boolean                    *receives
);



/*!
 *  @function
 *      Q3View_GetPickIDStyleState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param pickIDStyle      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetPickIDStyleState (
    TQ3ViewObject                 view,
    TQ3Uns32                      *pickIDStyle
);



/*!
 *  @function
 *      Q3View_GetPickPartsStyleState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param pickPartsStyle   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetPickPartsStyleState (
    TQ3ViewObject                 view,
    TQ3PickParts                  *pickPartsStyle
);



/*!
 *  @function
 *      Q3View_GetAntiAliasStyleState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param antiAliasData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetAntiAliasStyleState (
    TQ3ViewObject                 view,
    TQ3AntiAliasStyleData         *antiAliasData
);



/*!
 *  @function
 *      Q3View_GetFogStyleState
 *  @discussion
 *      Get the fog style associated with a view.
 *
 *      Not supported by QD3D.
 *
 *  @param theView          The view to query.
 *  @param fogData          Receives the current fog style for the viwe.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3View_GetFogStyleState (
    TQ3ViewObject                 view,
    TQ3FogStyleData               *fogData
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3View_GetDefaultAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetDefaultAttributeSet (
    TQ3ViewObject                 view,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3View_SetDefaultAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetDefaultAttributeSet (
    TQ3ViewObject                 view,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3View_GetAttributeSetState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetAttributeSetState (
    TQ3ViewObject                 view,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3View_GetAttributeState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param attributeType    Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetAttributeState (
    TQ3ViewObject                 view,
    TQ3AttributeType              attributeType,
    void                          *data
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif



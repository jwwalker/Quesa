/*! @header QuesaCamera.h
        Declares the Quesa camera objects.
 */
/*  NAME:
        QuesaCamera.h

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
#ifndef QUESA_CAMERA_HDR
#define QUESA_CAMERA_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DCAMERA__)
#error
#endif

#define __QD3DCAMERA__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint on

#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Camera placement
/*!
 *  @struct
 *      TQ3CameraPlacement
 *  @discussion
 *      Describes the location and orientation of a camera. All points are
 *      in world-space coordinates.

 *  @field cameraLocation   The location of the camera.
 *  @field pointOfInterest  The point at which the camera is aimed.
 *  @field upVector         The up vector for the camera. This vector must be
 *                          normalised and perpendicular to the viewing direction
 *                          of the camera. This vector is transformed to the y
 *                          axis of the viewing plane.
 */
typedef struct TQ3CameraPlacement {
    TQ3Point3D                                  cameraLocation;
    TQ3Point3D                                  pointOfInterest;
    TQ3Vector3D                                 upVector;
} TQ3CameraPlacement;


/*!
 *  @struct
 *      TQ3CameraRange
 *  @discussion
 *      Describes the hither and yon clipping planes of a camera.

 *  @field hither           The distance from the camera to the near clipping plane.
 *                          This value must always be more than 0.
 *  @field yon              The distance from the camera to the far clipping plane.
 *                          This value must always be more than the hither field,
 *                          although if it is too large then artifacts may be visible
 *                          during rendering.
 */
typedef struct TQ3CameraRange {
    float                                       hither;
    float                                       yon;
} TQ3CameraRange;


/*!
 *  @struct
 *      TQ3CameraViewPort
 *  @discussion
 *      Describes the viewport for a camera.
 *
 *      The camera viewport is the rectangular area of the view plane which is
 *      mapped to the rendered area of the current draw context. The default
 *      mapping is a square of size 2.0x2.0, with the top left corner anchored
 *      at {-1.0, 1.0}.
 *
 *      By adjusting the viewport, it is possible to control which area of the
 *      camera's view is rendered (e.g., to divide an image into a series of
 *      horizontal strips for printing).
 *
 *  @field origin           The origin for the viewport.
 *  @field width            The width of the viewport.
 *  @field height           The width of the viewport.
 */
typedef struct TQ3CameraViewPort {
    TQ3Point2D                                  origin;
    float                                       width;
    float                                       height;
} TQ3CameraViewPort;


/*!
 *  @struct
 *      TQ3CameraData
 *  @discussion
 *      Describes the common state for a camera.
 *
 *      The common camera state includes its position and orientation within
 *      the world (placement), the near and far clipping planes (range), and
 *      the current viewport.
 *
 *  @field placement        The position and orientation of the camera.
 *  @field range            The near and far clipping planes of the camera.
 *  @field viewPort         The viewport for the camera.
 */
typedef struct TQ3CameraData {
    TQ3CameraPlacement                          placement;
    TQ3CameraRange                              range;
    TQ3CameraViewPort                           viewPort;
} TQ3CameraData;


/*!
 *  @struct
 *      TQ3OrthographicCameraData
 *  @discussion
 *      Describes the state for an orthographic camera.
 *
 *      An orthographic camera is defined by four view planes, which form a
 *      box aligned with the camera view direction. These planes are defined
 *      by distances relative to the coordinate system formed by the camera
 *      location and its view direction.
 *
 *  @field cameraData       The common state for the camera.
 *  @field left             The left side of the view volume.
 *  @field top              The top side of the view volume.
 *  @field right            The right side of the view volume.
 *  @field bottom           The bottom side of the view volume.
 */
typedef struct TQ3OrthographicCameraData {
    TQ3CameraData                               cameraData;
    float                                       left;
    float                                       top;
    float                                       right;
    float                                       bottom;
} TQ3OrthographicCameraData;


/*!
 *  @struct
 *      TQ3ViewPlaneCameraData
 *  @discussion
 *      Describes the state for a view plane camera.
 *
 *      A view plane camera is a perspective camera which allows the specification
 *      of an off-center viewing frustum.
 *
 *      The frustum is formed by following the camera view direction for a given
 *      distance, then taking the specified square on that plane. The frustum
 *      extends from the camera position through the four edges of this square.
 *
 *  @field cameraData                The common state for the camera.
 *  @field viewPlane                 The distance from the camera to the view plane.
 *  @field halfWidthAtViewPlane      The half-width of the square on the view plane.
 *  @field halfHeightAtViewPlane     The half-height of the square on the view plane.
 *  @field centerXOnViewPlane        The x-center of the square on the view plane.
 *  @field centerYOnViewPlane        The y-center of the square on the view plane.
 */
typedef struct TQ3ViewPlaneCameraData {
    TQ3CameraData                               cameraData;
    float                                       viewPlane;
    float                                       halfWidthAtViewPlane;
    float                                       halfHeightAtViewPlane;
    float                                       centerXOnViewPlane;
    float                                       centerYOnViewPlane;
} TQ3ViewPlaneCameraData;


/*!
 *  @struct
 *      TQ3ViewAngleAspectCameraData
 *  @discussion
 *      Describes the state for a traditional perspective camera.
 *
 *      A view angle aspect camera is a perspective camera defined by a field of
 *      view angle and an aspect ratio.
 *
 *      The field of view angle must be a positive angle in radians. The aspect
 *      ratio should be greater than 1.0 if the field of view is vertical, and
 *      less than 1.0 if the field of view is horizontal.
 *
 *  @field cameraData       The common state for the camera.
 *  @field fov              The field of view of the camera, in radians.
 *  @field aspectRatioXToY  The horizontal-to-vertical aspect ratio of the camera.
 */
typedef struct TQ3ViewAngleAspectCameraData {
    TQ3CameraData                               cameraData;
    float                                       fov;
    float                                       aspectRatioXToY;
} TQ3ViewAngleAspectCameraData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Camera_GetType
 *  @discussion
 *      Returns the type of a camera.
 *
 *      Returns kQ3CameraTypeOrthographic, kQ3CameraTypeViewAngleAspect, or
 *      kQ3CameraTypeViewPlane. Returns kQ3ObjectTypeInvalid if the camera type
 *      is unknown.
 *
 *  @param camera           The camera to test.
 *  @result                 The type of the camera object.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Camera_GetType (
    TQ3CameraObject               camera
);



/*!
 *  @function
 *      Q3Camera_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param cameraData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_SetData (
    TQ3CameraObject               camera,
    const TQ3CameraData           *cameraData
);



/*!
 *  @function
 *      Q3Camera_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param cameraData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetData (
    TQ3CameraObject               camera,
    TQ3CameraData                 *cameraData
);



/*!
 *  @function
 *      Q3Camera_SetPlacement
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param placement        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_SetPlacement (
    TQ3CameraObject               camera,
    const TQ3CameraPlacement      *placement
);



/*!
 *  @function
 *      Q3Camera_GetPlacement
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param placement        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetPlacement (
    TQ3CameraObject               camera,
    TQ3CameraPlacement            *placement
);



/*!
 *  @function
 *      Q3Camera_SetRange
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param range            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_SetRange (
    TQ3CameraObject               camera,
    const TQ3CameraRange          *range
);



/*!
 *  @function
 *      Q3Camera_GetRange
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param range            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetRange (
    TQ3CameraObject               camera,
    TQ3CameraRange                *range
);



/*!
 *  @function
 *      Q3Camera_SetViewPort
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param viewPort         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_SetViewPort (
    TQ3CameraObject               camera,
    const TQ3CameraViewPort       *viewPort
);



/*!
 *  @function
 *      Q3Camera_GetViewPort
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param viewPort         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetViewPort (
    TQ3CameraObject               camera,
    TQ3CameraViewPort             *viewPort
);



/*!
 *  @function
 *      Q3Camera_GetWorldToView
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param worldToView      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetWorldToView (
    TQ3CameraObject               camera,
    TQ3Matrix4x4                  *worldToView
);



/*!
 *  @function
 *      Q3Camera_GetWorldToFrustum
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param worldToFrustum   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetWorldToFrustum (
    TQ3CameraObject               camera,
    TQ3Matrix4x4                  *worldToFrustum
);



/*!
 *  @function
 *      Q3Camera_GetViewToFrustum
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param viewToFrustum    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetViewToFrustum (
    TQ3CameraObject               camera,
    TQ3Matrix4x4                  *viewToFrustum
);



/*!
 *  @function
 *      Q3OrthographicCamera_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param orthographicData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3CameraObject  )
Q3OrthographicCamera_New (
    const TQ3OrthographicCameraData *orthographicData
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param cameraData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetData (
    TQ3CameraObject               camera,
    TQ3OrthographicCameraData     *cameraData
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param cameraData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetData (
    TQ3CameraObject               camera,
    const TQ3OrthographicCameraData *cameraData
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetLeft
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param left             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetLeft (
    TQ3CameraObject               camera,
    float                         left
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetLeft
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param left             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetLeft (
    TQ3CameraObject               camera,
    float                         *left
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetTop
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param top              Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetTop (
    TQ3CameraObject               camera,
    float                         top
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetTop
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param top              Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetTop (
    TQ3CameraObject               camera,
    float                         *top
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetRight
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param right            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetRight (
    TQ3CameraObject               camera,
    float                         right
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetRight
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param right            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetRight (
    TQ3CameraObject               camera,
    float                         *right
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetBottom
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param bottom           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetBottom (
    TQ3CameraObject               camera,
    float                         bottom
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetBottom
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param bottom           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetBottom (
    TQ3CameraObject               camera,
    float                         *bottom
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cameraData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3CameraObject  )
Q3ViewPlaneCamera_New (
    const TQ3ViewPlaneCameraData  *cameraData
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param cameraData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetData (
    TQ3CameraObject               camera,
    TQ3ViewPlaneCameraData        *cameraData
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param cameraData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetData (
    TQ3CameraObject               camera,
    const TQ3ViewPlaneCameraData  *cameraData
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetViewPlane
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param viewPlane        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetViewPlane (
    TQ3CameraObject               camera,
    float                         viewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetViewPlane
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param viewPlane        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetViewPlane (
    TQ3CameraObject               camera,
    float                         *viewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetHalfWidth
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param halfWidthAtViewPlane Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetHalfWidth (
    TQ3CameraObject               camera,
    float                         halfWidthAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetHalfWidth
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param halfWidthAtViewPlane Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetHalfWidth (
    TQ3CameraObject               camera,
    float                         *halfWidthAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetHalfHeight
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param halfHeightAtViewPlane Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetHalfHeight (
    TQ3CameraObject               camera,
    float                         halfHeightAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetHalfHeight
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param halfHeightAtViewPlane Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetHalfHeight (
    TQ3CameraObject               camera,
    float                         *halfHeightAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetCenterX
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param centerXOnViewPlane Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetCenterX (
    TQ3CameraObject               camera,
    float                         centerXOnViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetCenterX
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param centerXOnViewPlane Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetCenterX (
    TQ3CameraObject               camera,
    float                         *centerXOnViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetCenterY
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param centerYOnViewPlane Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetCenterY (
    TQ3CameraObject               camera,
    float                         centerYOnViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetCenterY
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param centerYOnViewPlane Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetCenterY (
    TQ3CameraObject               camera,
    float                         *centerYOnViewPlane
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cameraData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3CameraObject  )
Q3ViewAngleAspectCamera_New (
    const TQ3ViewAngleAspectCameraData *cameraData
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param cameraData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetData (
    TQ3CameraObject               camera,
    const TQ3ViewAngleAspectCameraData *cameraData
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param cameraData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_GetData (
    TQ3CameraObject               camera,
    TQ3ViewAngleAspectCameraData  *cameraData
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_SetFOV
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param fov              Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetFOV (
    TQ3CameraObject               camera,
    float                         fov
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_GetFOV
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param fov              Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_GetFOV (
    TQ3CameraObject               camera,
    float                         *fov
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_SetAspectRatio
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param aspectRatioXToY  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetAspectRatio (
    TQ3CameraObject               camera,
    float                         aspectRatioXToY
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_GetAspectRatio
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param camera           Description of the parameter.
 *  @param aspectRatioXToY  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_GetAspectRatio (
    TQ3CameraObject               camera,
    float                         *aspectRatioXToY
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif



/*! @header QuesaCamera.h
        Declares the Quesa camera objects.
 */
/*  NAME:
        QuesaCamera.h

    DESCRIPTION:
        Quesa public header.

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
 *      distance, then taking the specified rectangle on that plane. The frustum
 *      extends from the camera position through the four edges of this rectangle.
 *
 *  @field cameraData                The common state for the camera.
 *  @field viewPlane                 The distance from the camera to the view plane.
 *  @field halfWidthAtViewPlane      The half-width of the rectangle on the view plane.
 *  @field halfHeightAtViewPlane     The half-height of the rectangle on the view plane.
 *  @field centerXOnViewPlane        The x-center of the rectangle on the view plane.
 *  @field centerYOnViewPlane        The y-center of the rectangle on the view plane.
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
 *      Get the type of a camera.
 *
 *      Returns kQ3CameraTypeOrthographic, kQ3CameraTypeViewAngleAspect, or
 *      kQ3CameraTypeViewPlane. Returns kQ3ObjectTypeInvalid if the camera type
 *      is unknown.
 *
 *  @param camera           The camera to query.
 *  @result                 The type of the camera object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Camera_GetType (
    TQ3CameraObject               camera
);



/*!
 *  @function
 *      Q3Camera_SetData
 *  @discussion
 *      Set the common state for a camera.
 *
 *  @param camera           The camera to update.
 *  @param cameraData       The new common state for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_SetData (
    TQ3CameraObject               camera,
    const TQ3CameraData           *cameraData
);



/*!
 *  @function
 *      Q3Camera_GetData
 *  @discussion
 *      Get the common state of a camera.
 *
 *  @param camera           The camera to query.
 *  @param cameraData       Receives the common state of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetData (
    TQ3CameraObject               camera,
    TQ3CameraData                 *cameraData
);



/*!
 *  @function
 *      Q3Camera_SetPlacement
 *  @discussion
 *      Set the placement for a camera.
 *
 *  @param camera           The camera to update.
 *  @param placement        The new placement for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_SetPlacement (
    TQ3CameraObject               camera,
    const TQ3CameraPlacement      *placement
);



/*!
 *  @function
 *      Q3Camera_GetPlacement
 *  @discussion
 *      Get the placement of a camera.
 *
 *  @param camera           The camera to query.
 *  @param placement        Receives the placement of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetPlacement (
    TQ3CameraObject               camera,
    TQ3CameraPlacement            *placement
);



/*!
 *  @function
 *      Q3Camera_SetRange
 *  @discussion
 *      Set the range for a camera.
 *
 *  @param camera           The camera to update.
 *  @param range            The new range for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_SetRange (
    TQ3CameraObject               camera,
    const TQ3CameraRange          *range
);



/*!
 *  @function
 *      Q3Camera_GetRange
 *  @discussion
 *      Get the range of a camera.
 *
 *  @param camera           The camera to query.
 *  @param range            Receives the range of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetRange (
    TQ3CameraObject               camera,
    TQ3CameraRange                *range
);



/*!
 *  @function
 *      Q3Camera_SetViewPort
 *  @discussion
 *      Set the view port for a camera.
 *
 *  @param camera           The camera to update.
 *  @param viewPort         The new view port for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_SetViewPort (
    TQ3CameraObject               camera,
    const TQ3CameraViewPort       *viewPort
);



/*!
 *  @function
 *      Q3Camera_GetViewPort
 *  @discussion
 *      Get the view port of a camera.
 *
 *  @param camera           The camera to query.
 *  @param viewPort         Receives the view port of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetViewPort (
    TQ3CameraObject               camera,
    TQ3CameraViewPort             *viewPort
);



/*!
 *  @function
 *      Q3Camera_GetWorldToView
 *  @discussion
 *      Get the world-to-view matrix of a camera.
 *
 *      The world-to-view matrix transforms world coordinates to a coordinate
 *      system relative to the camera. The origin of this coordinate system is
 *      the camera location, with the camera view pointing down the -z axis
 *      and the camera up vector placed along the +y axis.
 *
 *  @param camera           The camera to query.
 *  @param worldToView      Receives the world-to-view matrix of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetWorldToView (
    TQ3CameraObject               camera,
    TQ3Matrix4x4                  *worldToView
);



/*!
 *  @function
 *      Q3Camera_GetWorldToFrustum
 *  @discussion
 *      Get the world-to-frustum matrix of a camera.
 *
 *      The world-to-frustum matrix transforms world coordinates to the viewing
 *      frustum coordinate system. It is equivalent to multiplying the matrices
 *      returned by Q3Camera_GetWorldToView and Q3Camera_GetViewToFrustum.
 *
 *  @param camera           The camera to query.
 *  @param worldToFrustum   Receives the world-to-frustum matrix of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetWorldToFrustum (
    TQ3CameraObject               camera,
    TQ3Matrix4x4                  *worldToFrustum
);



/*!
 *  @function
 *      Q3Camera_GetViewToFrustum
 *  @discussion
 *      Get the view-to-frustum matrix of a camera.
 *
 *      The view-to-frustum matrix transforms the camera coordinate system
 *      (as returned by Q3Camera_GetWorldToView) to the viewing frustum
 *      coordinate system.
 *
 *      The frustum coordinate system ranges from 0.0 to 1.0 in z, and from
 *      -1.0 to +1.0 in both x and y.
 *
 *  @param camera           The camera to query.
 *  @param viewToFrustum    Receives the view-to-frustum matrix of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetViewToFrustum (
    TQ3CameraObject               camera,
    TQ3Matrix4x4                  *viewToFrustum
);



/*!
 *  @function
 *      Q3OrthographicCamera_New
 *  @discussion
 *      Create a new orthographic camera object.
 *
 *  @param orthographicData The data for the camera object.
 *  @result                 The new camera object.
 */
Q3_EXTERN_API_C ( TQ3CameraObject  )
Q3OrthographicCamera_New (
    const TQ3OrthographicCameraData *orthographicData
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetData
 *  @discussion
 *      Get the data for an orthographic camera.
 *
 *  @param camera           The camera to query.
 *  @param cameraData       Receives the data of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetData (
    TQ3CameraObject               camera,
    TQ3OrthographicCameraData     *cameraData
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetData
 *  @discussion
 *      Set the data for an orthographic camera.
 *
 *  @param camera           The camera to update.
 *  @param cameraData       The new data for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetData (
    TQ3CameraObject                  camera,
    const TQ3OrthographicCameraData *cameraData
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetLeft
 *  @discussion
 *      Set the left side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to update.
 *  @param left             The new left side for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetLeft (
    TQ3CameraObject               camera,
    float                         left
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetLeft
 *  @discussion
 *      Get the left side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to query.
 *  @param left             Receives the left side of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetLeft (
    TQ3CameraObject               camera,
    float                         *left
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetTop
 *  @discussion
 *      Set the top side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to update.
 *  @param top              The new top side for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetTop (
    TQ3CameraObject               camera,
    float                         top
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetTop
 *  @discussion
 *      Get the top side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to query.
 *  @param top              Receives the top side of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetTop (
    TQ3CameraObject               camera,
    float                         *top
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetRight
 *  @discussion
 *      Set the right side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to update.
 *  @param right            The new right side for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetRight (
    TQ3CameraObject               camera,
    float                         right
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetRight
 *  @discussion
 *      Get the right side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to query.
 *  @param right            Receives the right side of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetRight (
    TQ3CameraObject               camera,
    float                         *right
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetBottom
 *  @discussion
 *      Set the bottom side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to update.
 *  @param bottom           The new bottom side for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetBottom (
    TQ3CameraObject               camera,
    float                         bottom
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetBottom
 *  @discussion
 *      Get the bottom side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to query.
 *  @param bottom           Receives the bottom side of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetBottom (
    TQ3CameraObject               camera,
    float                         *bottom
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_New
 *  @discussion
 *      Create a new view plane camera object.
 *
 *  @param cameraData       The data for the camera object.
 *  @result                 The new camera object.
 */
Q3_EXTERN_API_C ( TQ3CameraObject  )
Q3ViewPlaneCamera_New (
    const TQ3ViewPlaneCameraData  *cameraData
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetData
 *  @discussion
 *      Get the data for a view plane camera.
 *
 *  @param camera           The camera to query.
 *  @param cameraData       Receives the data of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetData (
    TQ3CameraObject               camera,
    TQ3ViewPlaneCameraData        *cameraData
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetData
 *  @discussion
 *      Set the data for a view plane camera.
 *
 *  @param camera           The camera to update.
 *  @param cameraData       The new data for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetData (
    TQ3CameraObject               camera,
    const TQ3ViewPlaneCameraData  *cameraData
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetViewPlane
 *  @discussion
 *      Set the view plane distance for a view plane camera.
 *
 *      The view plane distance is the distance along the camera view vector
 *      from the camera location.
 *
 *  @param camera           The camera to update.
 *  @param viewPlane        The new view plane distance for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetViewPlane (
    TQ3CameraObject               camera,
    float                         viewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetViewPlane
 *  @discussion
 *      Get the view plane distance of a view plane camera.
 *
 *      The view plane distance is the distance along the camera view vector
 *      from the camera location.
 *
 *  @param camera           The camera to query.
 *  @param viewPlane        Receives the view plane distance of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetViewPlane (
    TQ3CameraObject               camera,
    float                         *viewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetHalfWidth
 *  @discussion
 *      Set the half-width of the view rectangle of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle,
 *      whose width is twice the specified half-width.
 *
 *  @param camera                  The camera to update.
 *  @param halfWidthAtViewPlane    The new half-width of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetHalfWidth (
    TQ3CameraObject               camera,
    float                         halfWidthAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetHalfWidth
 *  @discussion
 *      Get the half-width of the view rectangle of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle,
 *      whose width is twice the returned half-width.
 *
 *  @param camera                  The camera to query.
 *  @param halfWidthAtViewPlane    Receives the half-width of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetHalfWidth (
    TQ3CameraObject               camera,
    float                         *halfWidthAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetHalfHeight
 *  @discussion
 *      Set the half-height of the view rectangle of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle,
 *      whose height is twice the specified half-height.
 *
 *  @param camera                  The camera to update.
 *  @param halfHeightAtViewPlane   The new half-height of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetHalfHeight (
    TQ3CameraObject               camera,
    float                         halfHeightAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetHalfHeight
 *  @discussion
 *      Get the half-height of the view rectangle of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle,
 *      whose height is twice the returned half-height.
 *
 *  @param camera                  The camera to query.
 *  @param halfHeightAtViewPlane   Receives the half-height of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetHalfHeight (
    TQ3CameraObject               camera,
    float                         *halfHeightAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetCenterX
 *  @discussion
 *      Set the x coordinate of the view rectangle center of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle, whose
 *      origin in x is at the specified coordinate.
 *
 *  @param camera                  The camera to update.
 *  @param centerXOnViewPlane      The new x coordinate for the center of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetCenterX (
    TQ3CameraObject               camera,
    float                         centerXOnViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetCenterX
 *  @discussion
 *      Get the x coordinate of the view rectangle center of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle, whose
 *      origin in x is at the returned coordinate.
 *
 *  @param camera                  The camera to query.
 *  @param centerXOnViewPlane      Receives the x coordinate of the center of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetCenterX (
    TQ3CameraObject               camera,
    float                         *centerXOnViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetCenterY
 *  @discussion
 *      Set the y coordinate of the view rectangle center of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle, whose
 *      origin in y is at the specified coordinate.
 *
 *  @param camera                  The camera to update.
 *  @param centerYOnViewPlane      The new y coordinate for the center of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetCenterY (
    TQ3CameraObject               camera,
    float                         centerYOnViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetCenterY
 *  @discussion
 *      Get the y coordinate of the view rectangle center of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle, whose
 *      origin in y is at the returned coordinate.
 *
 *  @param camera                  The camera to query.
 *  @param centerYOnViewPlane      Receives the y coordinate of the center of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetCenterY (
    TQ3CameraObject               camera,
    float                         *centerYOnViewPlane
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_New
 *  @discussion
 *      Create a new view angle aspect camera object.
 *
 *  @param cameraData       The data for the camera object.
 *  @result                 The new camera object.
 */
Q3_EXTERN_API_C ( TQ3CameraObject  )
Q3ViewAngleAspectCamera_New (
    const TQ3ViewAngleAspectCameraData *cameraData
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_SetData
 *  @discussion
 *      Set the data for a view angle aspect camera.
 *
 *  @param camera           The camera to update.
 *  @param cameraData       The new data for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetData (
    TQ3CameraObject                     camera,
    const TQ3ViewAngleAspectCameraData *cameraData
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_GetData
 *  @discussion
 *      Get the data for a view angle aspect camera.
 *
 *  @param camera           The camera to query.
 *  @param cameraData       Receives the data of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_GetData (
    TQ3CameraObject               camera,
    TQ3ViewAngleAspectCameraData  *cameraData
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_SetFOV
 *  @discussion
 *      Set the field of view for a view angle aspect camera.
 *
 *      The field of view is specified in radians.
 *
 *  @param camera           The camera to update.
 *  @param fov              The new field of view for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetFOV (
    TQ3CameraObject               camera,
    float                         fov
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_GetFOV
 *  @discussion
 *      Get the field of view of a view angle aspect camera.
 *
 *      The field of view is specified in radians.
 *
 *  @param camera           The camera to query.
 *  @param fov              Receives the field of view of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_GetFOV (
    TQ3CameraObject               camera,
    float                         *fov
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_SetAspectRatio
 *  @discussion
 *      Set the aspect ratio for a view angle aspect camera.
 *
 *      If the aspect ratio is greater than 1.0, the field of view of the
 *      camera is vertical. If it is less than 1.0, the field of view is
 *      horizontal.
 *
 *  @param camera           The camera to update.
 *  @param aspectRatioXToY  The new horizontal-to-vertical aspect ratio of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetAspectRatio (
    TQ3CameraObject               camera,
    float                         aspectRatioXToY
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_GetAspectRatio
 *  @discussion
 *      Get the aspect ratio of a view angle aspect camera.
 *
 *      If the aspect ratio is greater than 1.0, the field of view of the
 *      camera is vertical. If it is less than 1.0, the field of view is
 *      horizontal.
 *
 *  @param camera           The camera to query.
 *  @param aspectRatioXToY  Receives the horizontal-to-vertical aspect ratio of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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



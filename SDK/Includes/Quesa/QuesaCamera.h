/*! @header QuesaCamera.h
        Declares the Quesa camera objects.
 */
/*  NAME:
        QuesaCamera.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
typedef struct {
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
typedef struct {
    float                                       hither;
    float                                       yon;
} TQ3CameraRange;


// Camera viewport
typedef struct {
    TQ3Point2D                                  origin;
    float                                       width;
    float                                       height;
} TQ3CameraViewPort;


// Camera data
typedef struct {
    TQ3CameraPlacement                          placement;
    TQ3CameraRange                              range;
    TQ3CameraViewPort                           viewPort;
} TQ3CameraData;


// Orthographic camera data
typedef struct {
    TQ3CameraData                               cameraData;
    float                                       left;
    float                                       top;
    float                                       right;
    float                                       bottom;
} TQ3OrthographicCameraData;


// Perspective view plane camera data
typedef struct {
    TQ3CameraData                               cameraData;
    float                                       viewPlane;
    float                                       halfWidthAtViewPlane;
    float                                       halfHeightAtViewPlane;
    float                                       centerXOnViewPlane;
    float                                       centerYOnViewPlane;
} TQ3ViewPlaneCameraData;


// Perspective view angle camera data
typedef struct {
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
#ifdef __cplusplus
}
#endif

#endif



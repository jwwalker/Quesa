/*  NAME:
        QuesaCamera.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef __QUESA_CAMERA__
#define __QUESA_CAMERA__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Constants go here





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Types go here





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Macros go here





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
#if defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON

/*
 *	Q3Camera_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Camera_GetType (
	TQ3CameraObject               camera
);



/*
 *	Q3Camera_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_SetData (
	TQ3CameraObject               camera,
	const TQ3CameraData           *cameraData
);



/*
 *	Q3Camera_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetData (
	TQ3CameraObject               camera,
	TQ3CameraData                 *cameraData
);



/*
 *	Q3Camera_SetPlacement
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_SetPlacement (
	TQ3CameraObject               camera,
	const TQ3CameraPlacement      *placement
);



/*
 *	Q3Camera_GetPlacement
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetPlacement (
	TQ3CameraObject               camera,
	TQ3CameraPlacement            *placement
);



/*
 *	Q3Camera_SetRange
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_SetRange (
	TQ3CameraObject               camera,
	const TQ3CameraRange          *range
);



/*
 *	Q3Camera_GetRange
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetRange (
	TQ3CameraObject               camera,
	TQ3CameraRange                *range
);



/*
 *	Q3Camera_SetViewPort
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_SetViewPort (
	TQ3CameraObject               camera,
	const TQ3CameraViewPort       *viewPort
);



/*
 *	Q3Camera_GetViewPort
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetViewPort (
	TQ3CameraObject               camera,
	TQ3CameraViewPort             *viewPort
);



/*
 *	Q3Camera_GetWorldToView
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetWorldToView (
	TQ3CameraObject               camera,
	TQ3Matrix4x4                  *worldToView
);



/*
 *	Q3Camera_GetWorldToFrustum
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetWorldToFrustum (
	TQ3CameraObject               camera,
	TQ3Matrix4x4                  *worldToFrustum
);



/*
 *	Q3Camera_GetViewToFrustum
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Camera_GetViewToFrustum (
	TQ3CameraObject               camera,
	TQ3Matrix4x4                  *viewToFrustum
);



/*
 *	Q3OrthographicCamera_New
 *		Description of function
 */
EXTERN_API_C ( TQ3CameraObject  )
Q3OrthographicCamera_New (
	const TQ3OrthographicCameraData *orthographicData
);



/*
 *	Q3OrthographicCamera_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetData (
	TQ3CameraObject               camera,
	TQ3OrthographicCameraData     *cameraData
);



/*
 *	Q3OrthographicCamera_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetData (
	TQ3CameraObject               camera,
	const TQ3OrthographicCameraData *cameraData
);



/*
 *	Q3OrthographicCamera_SetLeft
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetLeft (
	TQ3CameraObject               camera,
	float                         left
);



/*
 *	Q3OrthographicCamera_GetLeft
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetLeft (
	TQ3CameraObject               camera,
	float                         *left
);



/*
 *	Q3OrthographicCamera_SetTop
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetTop (
	TQ3CameraObject               camera,
	float                         top
);



/*
 *	Q3OrthographicCamera_GetTop
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetTop (
	TQ3CameraObject               camera,
	float                         *top
);



/*
 *	Q3OrthographicCamera_SetRight
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetRight (
	TQ3CameraObject               camera,
	float                         right
);



/*
 *	Q3OrthographicCamera_GetRight
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetRight (
	TQ3CameraObject               camera,
	float                         *right
);



/*
 *	Q3OrthographicCamera_SetBottom
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetBottom (
	TQ3CameraObject               camera,
	float                         bottom
);



/*
 *	Q3OrthographicCamera_GetBottom
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetBottom (
	TQ3CameraObject               camera,
	float                         *bottom
);



/*
 *	Q3ViewPlaneCamera_New
 *		Description of function
 */
EXTERN_API_C ( TQ3CameraObject  )
Q3ViewPlaneCamera_New (
	const TQ3ViewPlaneCameraData  *cameraData
);



/*
 *	Q3ViewPlaneCamera_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetData (
	TQ3CameraObject               camera,
	TQ3ViewPlaneCameraData        *cameraData
);



/*
 *	Q3ViewPlaneCamera_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetData (
	TQ3CameraObject               camera,
	const TQ3ViewPlaneCameraData  *cameraData
);



/*
 *	Q3ViewPlaneCamera_SetViewPlane
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetViewPlane (
	TQ3CameraObject               camera,
	float                         viewPlane
);



/*
 *	Q3ViewPlaneCamera_GetViewPlane
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetViewPlane (
	TQ3CameraObject               camera,
	float                         *viewPlane
);



/*
 *	Q3ViewPlaneCamera_SetHalfWidth
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetHalfWidth (
	TQ3CameraObject               camera,
	float                         halfWidthAtViewPlane
);



/*
 *	Q3ViewPlaneCamera_GetHalfWidth
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetHalfWidth (
	TQ3CameraObject               camera,
	float                         *halfWidthAtViewPlane
);



/*
 *	Q3ViewPlaneCamera_SetHalfHeight
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetHalfHeight (
	TQ3CameraObject               camera,
	float                         halfHeightAtViewPlane
);



/*
 *	Q3ViewPlaneCamera_GetHalfHeight
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetHalfHeight (
	TQ3CameraObject               camera,
	float                         *halfHeightAtViewPlane
);



/*
 *	Q3ViewPlaneCamera_SetCenterX
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetCenterX (
	TQ3CameraObject               camera,
	float                         centerXOnViewPlane
);



/*
 *	Q3ViewPlaneCamera_GetCenterX
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetCenterX (
	TQ3CameraObject               camera,
	float                         *centerXOnViewPlane
);



/*
 *	Q3ViewPlaneCamera_SetCenterY
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetCenterY (
	TQ3CameraObject               camera,
	float                         centerYOnViewPlane
);



/*
 *	Q3ViewPlaneCamera_GetCenterY
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetCenterY (
	TQ3CameraObject               camera,
	float                         *centerYOnViewPlane
);



/*
 *	Q3ViewAngleAspectCamera_New
 *		Description of function
 */
EXTERN_API_C ( TQ3CameraObject  )
Q3ViewAngleAspectCamera_New (
	const TQ3ViewAngleAspectCameraData *cameraData
);



/*
 *	Q3ViewAngleAspectCamera_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetData (
	TQ3CameraObject               camera,
	const TQ3ViewAngleAspectCameraData *cameraData
);



/*
 *	Q3ViewAngleAspectCamera_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_GetData (
	TQ3CameraObject               camera,
	TQ3ViewAngleAspectCameraData  *cameraData
);



/*
 *	Q3ViewAngleAspectCamera_SetFOV
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetFOV (
	TQ3CameraObject               camera,
	float                         fov
);



/*
 *	Q3ViewAngleAspectCamera_GetFOV
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_GetFOV (
	TQ3CameraObject               camera,
	float                         *fov
);



/*
 *	Q3ViewAngleAspectCamera_SetAspectRatio
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetAspectRatio (
	TQ3CameraObject               camera,
	float                         aspectRatioXToY
);



/*
 *	Q3ViewAngleAspectCamera_GetAspectRatio
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_GetAspectRatio (
	TQ3CameraObject               camera,
	float                         *aspectRatioXToY
);

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        E3Camera.h

    DESCRIPTION:
        Header file for E3Camera.c.

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
#ifndef __E3CAMERA__
#define __E3CAMERA__
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
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3Camera_RegisterClass(void);
TQ3Status			E3Camera_UnregisterClass(void);

TQ3ObjectType		E3Camera_GetType(TQ3CameraObject theCamera);
TQ3Status			E3Camera_SetData(TQ3CameraObject theCamera, const TQ3CameraData *cameraData);
TQ3Status			E3Camera_GetData(TQ3CameraObject theCamera, TQ3CameraData *cameraData);
TQ3Status			E3Camera_SetPlacement(TQ3CameraObject theCamera, const TQ3CameraPlacement *placement);
TQ3Status			E3Camera_GetPlacement(TQ3CameraObject theCamera, TQ3CameraPlacement *placement);
TQ3Status			E3Camera_SetRange(TQ3CameraObject theCamera, const TQ3CameraRange *range);
TQ3Status			E3Camera_GetRange(TQ3CameraObject theCamera, TQ3CameraRange *range);
TQ3Status			E3Camera_SetViewPort(TQ3CameraObject theCamera, const TQ3CameraViewPort *viewPort);
TQ3Status			E3Camera_GetViewPort(TQ3CameraObject theCamera, TQ3CameraViewPort *viewPort);
TQ3Status			E3Camera_GetWorldToView(TQ3CameraObject theCamera, TQ3Matrix4x4 *worldToView);
TQ3Status			E3Camera_GetWorldToFrustum(TQ3CameraObject theCamera, TQ3Matrix4x4 *worldToFrustum);
TQ3Status			E3Camera_GetViewToFrustum(TQ3CameraObject theCamera, TQ3Matrix4x4 *viewToFrustum);
TQ3CameraObject		E3OrthographicCamera_New(const TQ3OrthographicCameraData *orthographicData);
TQ3Status			E3OrthographicCamera_GetData(TQ3CameraObject theCamera, TQ3OrthographicCameraData *cameraData);
TQ3Status			E3OrthographicCamera_SetData(TQ3CameraObject theCamera, const TQ3OrthographicCameraData *cameraData);
TQ3Status			E3OrthographicCamera_SetLeft(TQ3CameraObject theCamera, float left);
TQ3Status			E3OrthographicCamera_GetLeft(TQ3CameraObject theCamera, float *left);
TQ3Status			E3OrthographicCamera_SetTop(TQ3CameraObject theCamera, float top);
TQ3Status			E3OrthographicCamera_GetTop(TQ3CameraObject theCamera, float *top);
TQ3Status			E3OrthographicCamera_SetRight(TQ3CameraObject theCamera, float right);
TQ3Status			E3OrthographicCamera_GetRight(TQ3CameraObject theCamera, float *right);
TQ3Status			E3OrthographicCamera_SetBottom(TQ3CameraObject theCamera, float bottom);
TQ3Status			E3OrthographicCamera_GetBottom(TQ3CameraObject theCamera, float *bottom);
TQ3CameraObject		E3ViewPlaneCamera_New(const TQ3ViewPlaneCameraData *cameraData);
TQ3Status			E3ViewPlaneCamera_GetData(TQ3CameraObject theCamera, TQ3ViewPlaneCameraData *cameraData);
TQ3Status			E3ViewPlaneCamera_SetData(TQ3CameraObject theCamera, const TQ3ViewPlaneCameraData *cameraData);
TQ3Status			E3ViewPlaneCamera_SetViewPlane(TQ3CameraObject theCamera, float viewPlane);
TQ3Status			E3ViewPlaneCamera_GetViewPlane(TQ3CameraObject theCamera, float *viewPlane);
TQ3Status			E3ViewPlaneCamera_SetHalfWidth(TQ3CameraObject theCamera, float halfWidthAtViewPlane);
TQ3Status			E3ViewPlaneCamera_GetHalfWidth(TQ3CameraObject theCamera, float *halfWidthAtViewPlane);
TQ3Status			E3ViewPlaneCamera_SetHalfHeight(TQ3CameraObject theCamera, float halfHeightAtViewPlane);
TQ3Status			E3ViewPlaneCamera_GetHalfHeight(TQ3CameraObject theCamera, float *halfHeightAtViewPlane);
TQ3Status			E3ViewPlaneCamera_SetCenterX(TQ3CameraObject theCamera, float centerXOnViewPlane);
TQ3Status			E3ViewPlaneCamera_GetCenterX(TQ3CameraObject theCamera, float *centerXOnViewPlane);
TQ3Status			E3ViewPlaneCamera_SetCenterY(TQ3CameraObject theCamera, float centerYOnViewPlane);
TQ3Status			E3ViewPlaneCamera_GetCenterY(TQ3CameraObject theCamera, float *centerYOnViewPlane);
TQ3CameraObject		E3ViewAngleAspectCamera_New(const TQ3ViewAngleAspectCameraData *cameraData);
TQ3Status			E3ViewAngleAspectCamera_SetData(TQ3CameraObject theCamera, const TQ3ViewAngleAspectCameraData *cameraData);
TQ3Status			E3ViewAngleAspectCamera_GetData(TQ3CameraObject theCamera, TQ3ViewAngleAspectCameraData *cameraData);
TQ3Status			E3ViewAngleAspectCamera_SetFOV(TQ3CameraObject theCamera, float fov);
TQ3Status			E3ViewAngleAspectCamera_GetFOV(TQ3CameraObject theCamera, float *fov);
TQ3Status			E3ViewAngleAspectCamera_SetAspectRatio(TQ3CameraObject theCamera, float aspectRatioXToY);
TQ3Status			E3ViewAngleAspectCamera_GetAspectRatio(TQ3CameraObject theCamera, float *aspectRatioXToY);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


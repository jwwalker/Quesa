/*  NAME:
        E3Camera.h

    DESCRIPTION:
        Header file for E3Camera.c.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
#ifndef E3CAMERA_HDR
#define E3CAMERA_HDR
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


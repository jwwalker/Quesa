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
#ifndef E3CAMERA_HDR
#define E3CAMERA_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here


#include "E3Main.h"
#include "QuesaCamera.h"


//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------



class E3CameraInfo : public E3ShapeInfo
	{
	const TQ3XCameraFrustumMatrixMethod frustumMatrixMethod ;
	
public :

									E3CameraInfo	(
													TQ3XMetaHandler	newClassMetaHandler,
													E3ClassInfo*	newParent
					 								) ; // constructor	
	friend class E3Camera ;
	} ;




class E3Camera : public E3ShapeData
	{
Q3_CLASS_ENUMS ( kQ3ShapeTypeCamera, E3Camera, E3ShapeData )
protected :

    TQ3CameraData cameraData ;

public :
	static TQ3Status	RegisterClass ( void ) ;
	static TQ3Status	UnregisterClass ( void ) ;
	
	E3CameraInfo*		GetClass ( void ) { return (E3CameraInfo*) OpaqueTQ3Object::GetClass () ; }

	TQ3ObjectType		GetType ( void ) ;
	TQ3Status			SetData ( const TQ3CameraData *cameraData ) ;
	TQ3Status			GetData ( TQ3CameraData *CameraData ) ;
	TQ3Status			SetPlacement ( const TQ3CameraPlacement *Placement ) ;
	TQ3Status			GetPlacement ( TQ3CameraPlacement *placement ) ;
	TQ3Status			SetRange ( const TQ3CameraRange *range ) ;	
	TQ3Status			GetRange ( TQ3CameraRange *range ) ;
	TQ3Status			SetViewPort ( const TQ3CameraViewPort *viewPort ) ;
	TQ3Status			GetViewPort ( TQ3CameraViewPort *viewPort ) ;	
	TQ3Status			GetWorldToView ( TQ3Matrix4x4 *worldToView ) ;
	TQ3Status			GetWorldToFrustum ( TQ3Matrix4x4 *worldToFrustum ) ;
	TQ3Status			GetViewToFrustum ( TQ3Matrix4x4 *viewToFrustum ) ;	
	} ;
	


class E3OrthographicCamera : public E3Camera
	{
Q3_CLASS_ENUMS ( kQ3CameraTypeOrthographic, E3OrthographicCamera, E3Camera )
    float                                       left ;
    float                                       top ;
    float                                       right ;
    float                                       bottom ;
	
public :

	void				GetFrustumMatrix ( TQ3Matrix4x4 *theMatrix ) ;
	
	TQ3Status			GetData ( TQ3OrthographicCameraData *CameraData ) ;
	TQ3Status			SetData ( const TQ3OrthographicCameraData *CameraData ) ;
	TQ3Status			SetLeft ( float Left ) ;
	TQ3Status			GetLeft ( float *Left ) ;
	TQ3Status			SetTop ( float Top ) ;
	TQ3Status			GetTop ( float *Top ) ;
	TQ3Status			SetRight ( float Right ) ;
	TQ3Status			GetRight ( float *Right ) ;
	TQ3Status			SetBottom ( float Bottom ) ;
	TQ3Status			GetBottom ( float *Bottom ) ;
	
	
	friend TQ3Status e3camera_orthographic_new(TQ3Object theObject, void *privateData, const void *paramData) ;
	} ;
	


class E3ViewPlaneCamera : public E3Camera
	{
Q3_CLASS_ENUMS ( kQ3CameraTypeViewPlane, E3ViewPlaneCamera, E3Camera )
    float                                       viewPlane ;
    float                                       halfWidthAtViewPlane ;
    float                                       halfHeightAtViewPlane ;
    float                                       centerXOnViewPlane ;
    float                                       centerYOnViewPlane ;

public :

	TQ3Status			GetData ( TQ3ViewPlaneCameraData *CameraData ) ;
	TQ3Status			SetData ( const TQ3ViewPlaneCameraData *CameraData ) ;
	TQ3Status			SetViewPlane ( float ViewPlane ) ;
	TQ3Status			GetViewPlane ( float *ViewPlane ) ;
	TQ3Status			SetHalfWidth ( float HalfWidthAtViewPlane ) ;
	TQ3Status			GetHalfWidth ( float *HalfWidthAtViewPlane ) ;
	TQ3Status			SetHalfHeight ( float HalfHeightAtViewPlane ) ;
	TQ3Status			GetHalfHeight ( float *HalfHeightAtViewPlane ) ;
	TQ3Status			SetCenterX ( float CenterXOnViewPlane ) ;
	TQ3Status			GetCenterX ( float *CenterXOnViewPlane ) ;
	TQ3Status			SetCenterY ( float CenterYOnViewPlane ) ;
	TQ3Status			GetCenterY ( float *CenterYOnViewPlane ) ;

		
	friend void e3camera_viewplane_frustum_matrix(TQ3CameraObject theCamera, TQ3Matrix4x4 *theMatrix) ;
	friend TQ3Status e3camera_viewplane_new(TQ3Object theObject, void *privateData, const void *paramData) ;
	} ;
	


class E3ViewAngleAspectCamera : public E3Camera
	{
Q3_CLASS_ENUMS ( kQ3CameraTypeViewAngleAspect, E3ViewAngleAspectCamera, E3Camera )
    float                                       fov ;
    float                                       aspectRatioXToY ;

public :

	void				GetFrustumMatrix ( TQ3Matrix4x4 *theMatrix ) ;
	TQ3Status			SetData ( const TQ3ViewAngleAspectCameraData *CameraData ) ;
	TQ3Status			GetData ( TQ3ViewAngleAspectCameraData *CameraData ) ;
	TQ3Status			SetFOV ( float Fov ) ;
	TQ3Status			GetFOV ( float *Fov ) ;
	TQ3Status			SetAspectRatio ( float AspectRatioXToY ) ;
	TQ3Status			GetAspectRatio ( float *AspectRatioXToY ) ;
	
	friend TQ3Status e3camera_viewangle_new(TQ3Object theObject, void *privateData, const void *paramData) ;
	} ;




//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------

TQ3CameraObject		E3OrthographicCamera_New(const TQ3OrthographicCameraData *orthographicData);
TQ3CameraObject		E3ViewPlaneCamera_New(const TQ3ViewPlaneCameraData *cameraData);
TQ3CameraObject		E3ViewAngleAspectCamera_New(const TQ3ViewAngleAspectCameraData *cameraData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


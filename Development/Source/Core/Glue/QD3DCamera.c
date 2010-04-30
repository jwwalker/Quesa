/*  NAME:
        QD3DCamera.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Copyright (c) 1999-2010, Quesa Developers. All rights reserved.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Camera.h"
#include "E3Math_Intersect.h"




//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3Camera_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3Camera_GetType(TQ3CameraObject camera)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3ObjectTypeInvalid ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->GetType () ;
}





//=============================================================================
//      Q3Camera_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_SetData(TQ3CameraObject camera, const TQ3CameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cameraData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	// Further checks on cameraData
	if (fabs( Q3FastVector3D_Length( &cameraData->placement.upVector ) - 1.0f ) > kQ3RealZero)
	{
		E3ErrorManager_PostWarning( kQ3WarningVector3DNotUnitLength );
	}
	
	if (kQ3RealZero > Q3FastPoint3D_DistanceSquared(
		&cameraData->placement.cameraLocation,
		&cameraData->placement.pointOfInterest))
	{
		E3ErrorManager_PostError( kQ3ErrorVector3DZeroLength, kQ3False );
	}
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->SetData ( cameraData ) ;
}





//=============================================================================
//      Q3Camera_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetData(TQ3CameraObject camera, TQ3CameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cameraData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on cameraData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->GetData ( cameraData ) ;
}





//=============================================================================
//      Q3Camera_SetPlacement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_SetPlacement(TQ3CameraObject camera, const TQ3CameraPlacement *placement)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(placement), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	// Further checks on placement
	if (fabs( Q3FastVector3D_Length( &placement->upVector ) - 1.0f ) > kQ3RealZero)
	{
		E3ErrorManager_PostWarning( kQ3WarningVector3DNotUnitLength );
	}
	
	if (kQ3RealZero > Q3FastPoint3D_DistanceSquared( &placement->cameraLocation,
		&placement->pointOfInterest))
	{
		E3ErrorManager_PostError( kQ3ErrorVector3DZeroLength, kQ3False );
	}
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->SetPlacement ( placement ) ;
}





//=============================================================================
//      Q3Camera_GetPlacement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetPlacement(TQ3CameraObject camera, TQ3CameraPlacement *placement)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(placement), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on placement
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->GetPlacement ( placement ) ;
}





//=============================================================================
//      Q3Camera_SetRange : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_SetRange(TQ3CameraObject camera, const TQ3CameraRange *range)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(range), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on range
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->SetRange ( range ) ;
}





//=============================================================================
//      Q3Camera_GetRange : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetRange(TQ3CameraObject camera, TQ3CameraRange *range)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(range), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on range
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->GetRange ( range ) ;
}





//=============================================================================
//      Q3Camera_SetViewPort : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_SetViewPort(TQ3CameraObject camera, const TQ3CameraViewPort *viewPort)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(viewPort), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on viewPort
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->SetViewPort ( viewPort ) ;
}





//=============================================================================
//      Q3Camera_GetViewPort : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetViewPort(TQ3CameraObject camera, TQ3CameraViewPort *viewPort)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(viewPort), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on viewPort
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->GetViewPort ( viewPort ) ;
}





//=============================================================================
//      Q3Camera_GetWorldToView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetWorldToView(TQ3CameraObject camera, TQ3Matrix4x4 *worldToView)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(worldToView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on worldToView
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->GetWorldToView ( worldToView ) ;
}





//=============================================================================
//      Q3Camera_GetWorldToFrustum : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetWorldToFrustum(TQ3CameraObject camera, TQ3Matrix4x4 *worldToFrustum)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(worldToFrustum), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on worldToFrustum
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->GetWorldToFrustum ( worldToFrustum ) ;
}





//=============================================================================
//      Q3Camera_GetViewToFrustum : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetViewToFrustum(TQ3CameraObject camera, TQ3Matrix4x4 *viewToFrustum)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(viewToFrustum), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on viewToFrustum
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Camera*) camera )->GetViewToFrustum ( viewToFrustum ) ;
}





/*!
 *  @function
 *      Q3Camera_IsBoundingBoxVisible
 *  @abstract
 *      Test a bounding box for visibility.
 *
 *  @discussion
 *      The bounding box (assumed to be in world coordinates) is tested for
 *		intersection with the view frustum of the camera.
 *
 *  @param camera           The view to check the bounding box against.
 *  @param bbox             The world bounding box to test.
 *  @result                 True or false as the bounding box is visible.
 */
TQ3Boolean
Q3Camera_IsBoundingBoxVisible (
    TQ3CameraObject               camera,
    const TQ3BoundingBox          *bbox
)
{
	return (TQ3Boolean) E3BoundingBox_IntersectCameraFrustum( camera, *bbox );
}





//=============================================================================
//      Q3OrthographicCamera_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3CameraObject
Q3OrthographicCamera_New(const TQ3OrthographicCameraData *orthographicData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orthographicData), NULL);



	// Debug build checks
#if Q3_DEBUG
	// Further checks on orthographicData
	if (fabs( Q3FastVector3D_Length( &orthographicData->cameraData.placement.upVector ) -
		1.0f ) > kQ3RealZero)
	{
		E3ErrorManager_PostWarning( kQ3WarningVector3DNotUnitLength );
	}
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3OrthographicCamera_New(orthographicData));
}





//=============================================================================
//      Q3OrthographicCamera_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_GetData(TQ3CameraObject camera, TQ3OrthographicCameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cameraData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on cameraData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3OrthographicCamera*) camera )->GetData ( cameraData ) ;
}





//=============================================================================
//      Q3OrthographicCamera_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_SetData(TQ3CameraObject camera, const TQ3OrthographicCameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cameraData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	// Further checks on cameraData
	if (fabs( Q3FastVector3D_Length( &cameraData->cameraData.placement.upVector ) -
		1.0f ) > kQ3RealZero)
	{
		E3ErrorManager_PostWarning( kQ3WarningVector3DNotUnitLength );
	}
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3OrthographicCamera*) camera )->SetData ( cameraData ) ;
}





//=============================================================================
//      Q3OrthographicCamera_SetLeft : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_SetLeft(TQ3CameraObject camera, float left)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on left
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3OrthographicCamera*) camera )->SetLeft ( left ) ;
}





//=============================================================================
//      Q3OrthographicCamera_GetLeft : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_GetLeft(TQ3CameraObject camera, float *left)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(left), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on left
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3OrthographicCamera*) camera )->GetLeft ( left ) ;
}





//=============================================================================
//      Q3OrthographicCamera_SetTop : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_SetTop(TQ3CameraObject camera, float top)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on top
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3OrthographicCamera*) camera )->SetTop ( top ) ;
}





//=============================================================================
//      Q3OrthographicCamera_GetTop : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_GetTop(TQ3CameraObject camera, float *top)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(top), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on top
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3OrthographicCamera*) camera )->GetTop ( top ) ;
}





//=============================================================================
//      Q3OrthographicCamera_SetRight : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_SetRight(TQ3CameraObject camera, float right)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on right
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3OrthographicCamera*) camera )->SetRight ( right ) ;
}





//=============================================================================
//      Q3OrthographicCamera_GetRight : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_GetRight(TQ3CameraObject camera, float *right)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(right), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on right
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3OrthographicCamera*) camera )->GetRight ( right ) ;
}





//=============================================================================
//      Q3OrthographicCamera_SetBottom : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_SetBottom(TQ3CameraObject camera, float bottom)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on bottom
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3OrthographicCamera*) camera )->SetBottom ( bottom ) ;
}





//=============================================================================
//      Q3OrthographicCamera_GetBottom : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_GetBottom(TQ3CameraObject camera, float *bottom)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bottom), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on bottom
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3OrthographicCamera*) camera )->GetBottom ( bottom ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3CameraObject
Q3ViewPlaneCamera_New(const TQ3ViewPlaneCameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cameraData), NULL);



	// Debug build checks
#if Q3_DEBUG
	// Further checks on cameraData
	if (fabs( Q3FastVector3D_Length( &cameraData->cameraData.placement.upVector ) -
		1.0f ) > kQ3RealZero)
	{
		E3ErrorManager_PostWarning( kQ3WarningVector3DNotUnitLength );
	}
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewPlaneCamera_New(cameraData));
}





//=============================================================================
//      Q3ViewPlaneCamera_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_GetData(TQ3CameraObject camera, TQ3ViewPlaneCameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cameraData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on cameraData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->GetData ( cameraData ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetData(TQ3CameraObject camera, const TQ3ViewPlaneCameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cameraData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	// Further checks on cameraData
	if (fabs( Q3FastVector3D_Length( &cameraData->cameraData.placement.upVector ) -
		1.0f ) > kQ3RealZero)
	{
		E3ErrorManager_PostWarning( kQ3WarningVector3DNotUnitLength );
	}
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->SetData ( cameraData ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_SetViewPlane : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetViewPlane(TQ3CameraObject camera, float viewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on viewPlane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->SetViewPlane ( viewPlane ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_GetViewPlane : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_GetViewPlane(TQ3CameraObject camera, float *viewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(viewPlane), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on viewPlane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->GetViewPlane ( viewPlane ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_SetHalfWidth : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetHalfWidth(TQ3CameraObject camera, float halfWidthAtViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on halfWidthAtViewPlane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->SetHalfWidth ( halfWidthAtViewPlane ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_GetHalfWidth : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_GetHalfWidth(TQ3CameraObject camera, float *halfWidthAtViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(halfWidthAtViewPlane), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on halfWidthAtViewPlane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->GetHalfWidth ( halfWidthAtViewPlane ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_SetHalfHeight : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetHalfHeight(TQ3CameraObject camera, float halfHeightAtViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on halfHeightAtViewPlane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->SetHalfHeight ( halfHeightAtViewPlane ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_GetHalfHeight : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_GetHalfHeight(TQ3CameraObject camera, float *halfHeightAtViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(halfHeightAtViewPlane), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on halfHeightAtViewPlane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->GetHalfHeight ( halfHeightAtViewPlane ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_SetCenterX : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetCenterX(TQ3CameraObject camera, float centerXOnViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on centerXOnViewPlane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->SetCenterX ( centerXOnViewPlane ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_GetCenterX : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_GetCenterX(TQ3CameraObject camera, float *centerXOnViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(centerXOnViewPlane), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on centerXOnViewPlane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->GetCenterX ( centerXOnViewPlane ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_SetCenterY : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetCenterY(TQ3CameraObject camera, float centerYOnViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on centerYOnViewPlane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->SetCenterY ( centerYOnViewPlane ) ;
}





//=============================================================================
//      Q3ViewPlaneCamera_GetCenterY : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_GetCenterY(TQ3CameraObject camera, float *centerYOnViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(centerYOnViewPlane), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on centerYOnViewPlane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewPlaneCamera*) camera )->GetCenterY ( centerYOnViewPlane ) ;
}





//=============================================================================
//      Q3ViewAngleAspectCamera_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3CameraObject
Q3ViewAngleAspectCamera_New(const TQ3ViewAngleAspectCameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cameraData), NULL);



	// Debug build checks
#if Q3_DEBUG
	// Further checks on cameraData
	if (fabs( Q3FastVector3D_Length( &cameraData->cameraData.placement.upVector ) -
		1.0f ) > kQ3RealZero)
	{
		E3ErrorManager_PostWarning( kQ3WarningVector3DNotUnitLength );
	}
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewAngleAspectCamera_New(cameraData));
}





//=============================================================================
//      Q3ViewAngleAspectCamera_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewAngleAspectCamera_SetData(TQ3CameraObject camera, const TQ3ViewAngleAspectCameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cameraData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	// Further checks on cameraData
	if (fabs( Q3FastVector3D_Length( &cameraData->cameraData.placement.upVector ) -
		1.0f ) > kQ3RealZero)
	{
		E3ErrorManager_PostWarning( kQ3WarningVector3DNotUnitLength );
	}
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewAngleAspectCamera*) camera )->SetData ( cameraData ) ;
}





//=============================================================================
//      Q3ViewAngleAspectCamera_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewAngleAspectCamera_GetData(TQ3CameraObject camera, TQ3ViewAngleAspectCameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cameraData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on cameraData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewAngleAspectCamera*) camera )->GetData ( cameraData ) ;
}





//=============================================================================
//      Q3ViewAngleAspectCamera_SetFOV : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewAngleAspectCamera_SetFOV(TQ3CameraObject camera, float fov)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on fov
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewAngleAspectCamera*) camera )->SetFOV ( fov ) ;
}





//=============================================================================
//      Q3ViewAngleAspectCamera_GetFOV : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewAngleAspectCamera_GetFOV(TQ3CameraObject camera, float *fov)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fov), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on fov
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewAngleAspectCamera*) camera )->GetFOV ( fov ) ;
}





//=============================================================================
//      Q3ViewAngleAspectCamera_SetAspectRatio : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewAngleAspectCamera_SetAspectRatio(TQ3CameraObject camera, float aspectRatioXToY)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on aspectRatioXToY
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewAngleAspectCamera*) camera )->SetAspectRatio ( aspectRatioXToY ) ;
}





//=============================================================================
//      Q3ViewAngleAspectCamera_GetAspectRatio : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewAngleAspectCamera_GetAspectRatio(TQ3CameraObject camera, float *aspectRatioXToY)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Camera::IsOfMyClass ( camera ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(aspectRatioXToY), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3Failure);

	if (0) // Further checks on aspectRatioXToY
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3ViewAngleAspectCamera*) camera )->GetAspectRatio ( aspectRatioXToY ) ;
}






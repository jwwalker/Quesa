/*  NAME:
        QD3DCamera.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Camera.h"





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
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on camera
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Camera_GetType(camera));
}





//=============================================================================
//      Q3Camera_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_SetData(TQ3CameraObject camera, const TQ3CameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Camera_SetData(camera, cameraData));
}





//=============================================================================
//      Q3Camera_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetData(TQ3CameraObject camera, TQ3CameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3Camera_GetData(camera, cameraData));
}





//=============================================================================
//      Q3Camera_SetPlacement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_SetPlacement(TQ3CameraObject camera, const TQ3CameraPlacement *placement)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Camera_SetPlacement(camera, placement));
}





//=============================================================================
//      Q3Camera_GetPlacement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetPlacement(TQ3CameraObject camera, TQ3CameraPlacement *placement)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3Camera_GetPlacement(camera, placement));
}





//=============================================================================
//      Q3Camera_SetRange : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_SetRange(TQ3CameraObject camera, const TQ3CameraRange *range)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3Camera_SetRange(camera, range));
}





//=============================================================================
//      Q3Camera_GetRange : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetRange(TQ3CameraObject camera, TQ3CameraRange *range)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3Camera_GetRange(camera, range));
}





//=============================================================================
//      Q3Camera_SetViewPort : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_SetViewPort(TQ3CameraObject camera, const TQ3CameraViewPort *viewPort)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3Camera_SetViewPort(camera, viewPort));
}





//=============================================================================
//      Q3Camera_GetViewPort : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetViewPort(TQ3CameraObject camera, TQ3CameraViewPort *viewPort)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3Camera_GetViewPort(camera, viewPort));
}





//=============================================================================
//      Q3Camera_GetWorldToView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetWorldToView(TQ3CameraObject camera, TQ3Matrix4x4 *worldToView)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3Camera_GetWorldToView(camera, worldToView));
}





//=============================================================================
//      Q3Camera_GetWorldToFrustum : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetWorldToFrustum(TQ3CameraObject camera, TQ3Matrix4x4 *worldToFrustum)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3Camera_GetWorldToFrustum(camera, worldToFrustum));
}





//=============================================================================
//      Q3Camera_GetViewToFrustum : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Camera_GetViewToFrustum(TQ3CameraObject camera, TQ3Matrix4x4 *viewToFrustum)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3Camera_GetViewToFrustum(camera, viewToFrustum));
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
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3OrthographicCamera_GetData(camera, cameraData));
}





//=============================================================================
//      Q3OrthographicCamera_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_SetData(TQ3CameraObject camera, const TQ3OrthographicCameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3OrthographicCamera_SetData(camera, cameraData));
}





//=============================================================================
//      Q3OrthographicCamera_SetLeft : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_SetLeft(TQ3CameraObject camera, float left)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



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
	return(E3OrthographicCamera_SetLeft(camera, left));
}





//=============================================================================
//      Q3OrthographicCamera_GetLeft : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_GetLeft(TQ3CameraObject camera, float *left)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3OrthographicCamera_GetLeft(camera, left));
}





//=============================================================================
//      Q3OrthographicCamera_SetTop : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_SetTop(TQ3CameraObject camera, float top)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



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
	return(E3OrthographicCamera_SetTop(camera, top));
}





//=============================================================================
//      Q3OrthographicCamera_GetTop : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_GetTop(TQ3CameraObject camera, float *top)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3OrthographicCamera_GetTop(camera, top));
}





//=============================================================================
//      Q3OrthographicCamera_SetRight : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_SetRight(TQ3CameraObject camera, float right)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



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
	return(E3OrthographicCamera_SetRight(camera, right));
}





//=============================================================================
//      Q3OrthographicCamera_GetRight : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_GetRight(TQ3CameraObject camera, float *right)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3OrthographicCamera_GetRight(camera, right));
}





//=============================================================================
//      Q3OrthographicCamera_SetBottom : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_SetBottom(TQ3CameraObject camera, float bottom)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



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
	return(E3OrthographicCamera_SetBottom(camera, bottom));
}





//=============================================================================
//      Q3OrthographicCamera_GetBottom : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3OrthographicCamera_GetBottom(TQ3CameraObject camera, float *bottom)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3OrthographicCamera_GetBottom(camera, bottom));
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
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3ViewPlaneCamera_GetData(camera, cameraData));
}





//=============================================================================
//      Q3ViewPlaneCamera_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetData(TQ3CameraObject camera, const TQ3ViewPlaneCameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3ViewPlaneCamera_SetData(camera, cameraData));
}





//=============================================================================
//      Q3ViewPlaneCamera_SetViewPlane : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetViewPlane(TQ3CameraObject camera, float viewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



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
	return(E3ViewPlaneCamera_SetViewPlane(camera, viewPlane));
}





//=============================================================================
//      Q3ViewPlaneCamera_GetViewPlane : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_GetViewPlane(TQ3CameraObject camera, float *viewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3ViewPlaneCamera_GetViewPlane(camera, viewPlane));
}





//=============================================================================
//      Q3ViewPlaneCamera_SetHalfWidth : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetHalfWidth(TQ3CameraObject camera, float halfWidthAtViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



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
	return(E3ViewPlaneCamera_SetHalfWidth(camera, halfWidthAtViewPlane));
}





//=============================================================================
//      Q3ViewPlaneCamera_GetHalfWidth : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_GetHalfWidth(TQ3CameraObject camera, float *halfWidthAtViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3ViewPlaneCamera_GetHalfWidth(camera, halfWidthAtViewPlane));
}





//=============================================================================
//      Q3ViewPlaneCamera_SetHalfHeight : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetHalfHeight(TQ3CameraObject camera, float halfHeightAtViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



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
	return(E3ViewPlaneCamera_SetHalfHeight(camera, halfHeightAtViewPlane));
}





//=============================================================================
//      Q3ViewPlaneCamera_GetHalfHeight : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_GetHalfHeight(TQ3CameraObject camera, float *halfHeightAtViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3ViewPlaneCamera_GetHalfHeight(camera, halfHeightAtViewPlane));
}





//=============================================================================
//      Q3ViewPlaneCamera_SetCenterX : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetCenterX(TQ3CameraObject camera, float centerXOnViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



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
	return(E3ViewPlaneCamera_SetCenterX(camera, centerXOnViewPlane));
}





//=============================================================================
//      Q3ViewPlaneCamera_GetCenterX : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_GetCenterX(TQ3CameraObject camera, float *centerXOnViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3ViewPlaneCamera_GetCenterX(camera, centerXOnViewPlane));
}





//=============================================================================
//      Q3ViewPlaneCamera_SetCenterY : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_SetCenterY(TQ3CameraObject camera, float centerYOnViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



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
	return(E3ViewPlaneCamera_SetCenterY(camera, centerYOnViewPlane));
}





//=============================================================================
//      Q3ViewPlaneCamera_GetCenterY : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewPlaneCamera_GetCenterY(TQ3CameraObject camera, float *centerYOnViewPlane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3ViewPlaneCamera_GetCenterY(camera, centerYOnViewPlane));
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
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3ViewAngleAspectCamera_SetData(camera, cameraData));
}





//=============================================================================
//      Q3ViewAngleAspectCamera_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewAngleAspectCamera_GetData(TQ3CameraObject camera, TQ3ViewAngleAspectCameraData *cameraData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3ViewAngleAspectCamera_GetData(camera, cameraData));
}





//=============================================================================
//      Q3ViewAngleAspectCamera_SetFOV : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewAngleAspectCamera_SetFOV(TQ3CameraObject camera, float fov)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



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
	return(E3ViewAngleAspectCamera_SetFOV(camera, fov));
}





//=============================================================================
//      Q3ViewAngleAspectCamera_GetFOV : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewAngleAspectCamera_GetFOV(TQ3CameraObject camera, float *fov)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3ViewAngleAspectCamera_GetFOV(camera, fov));
}





//=============================================================================
//      Q3ViewAngleAspectCamera_SetAspectRatio : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewAngleAspectCamera_SetAspectRatio(TQ3CameraObject camera, float aspectRatioXToY)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);



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
	return(E3ViewAngleAspectCamera_SetAspectRatio(camera, aspectRatioXToY));
}





//=============================================================================
//      Q3ViewAngleAspectCamera_GetAspectRatio : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewAngleAspectCamera_GetAspectRatio(TQ3CameraObject camera, float *aspectRatioXToY)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(camera->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(camera, kQ3ShapeTypeCamera), kQ3Failure);
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
	return(E3ViewAngleAspectCamera_GetAspectRatio(camera, aspectRatioXToY));
}






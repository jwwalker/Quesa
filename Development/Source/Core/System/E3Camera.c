/*  NAME:
        E3Camera.c

    DESCRIPTION:
        Quesa camera object implementation.
        
        The implementation of the E3Camera_xxx routines _requires_ that a
        TQ3CameraData field be the first field in any camera specific
        instance data.
        
        This allows the generic routines to manipulate the generic fields
        in any camera object, without having to have accessors for every
        camera type.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Camera.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3camera_orthographic_frustum_matrix : Get the view to frustum matrix.
//-----------------------------------------------------------------------------
static void
e3camera_orthographic_frustum_matrix(TQ3CameraObject theCamera, TQ3Matrix4x4 *theMatrix)
{	TQ3OrthographicCameraData		*instanceData = (TQ3OrthographicCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeOrthographic);
	float							x, y, z, tx, ty;



	// Initialise ourselves
	x  = 2.0f / (instanceData->right - instanceData->left);
	y  = 2.0f / (instanceData->top   - instanceData->bottom);
	z  = 1.0f / (instanceData->cameraData.range.yon - instanceData->cameraData.range.hither);

	tx = -(instanceData->right + instanceData->left)   / (instanceData->right - instanceData->left);
	ty = -(instanceData->top   + instanceData->bottom) / (instanceData->top   - instanceData->bottom);



	// Set up the matrix
	Q3Matrix4x4_SetIdentity(theMatrix);
	theMatrix->value[0][0] = x;
	theMatrix->value[3][0] = tx;
	theMatrix->value[1][1] = y;
	theMatrix->value[3][1] = ty;
	theMatrix->value[2][2] = z;
	theMatrix->value[3][2] = instanceData->cameraData.range.hither * z;
}





//=============================================================================
//      e3camera_orthographic_new : Orthographic camera new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3camera_orthographic_new(TQ3Object /*theObject*/, void *privateData, const void *paramData)
{	TQ3OrthographicCameraData			*instanceData = (TQ3OrthographicCameraData *)       privateData;
	const TQ3OrthographicCameraData		*cameraData   = (const TQ3OrthographicCameraData *) paramData;



	// Initialise our instance data
	*instanceData = *cameraData;
	return(kQ3Success);
}





//=============================================================================
//      e3camera_orthographic_metahandler : Orthographic camera metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3camera_orthographic_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3camera_orthographic_new;
			break;

		case kQ3XMethodTypeCameraFrustumMatrix:
			theMethod = (TQ3XFunctionPointer) e3camera_orthographic_frustum_matrix;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3camera_viewplane_frustum_matrix : Get the view to frustum matrix.
//-----------------------------------------------------------------------------
static void
e3camera_viewplane_frustum_matrix(TQ3CameraObject theCamera, TQ3Matrix4x4 *theMatrix)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Calculate the view to frustum matrix for the camera
	// dair, to be implemented
	Q3Matrix4x4_SetIdentity(theMatrix);
}





//=============================================================================
//      e3camera_viewplane_new : View Plane camera new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3camera_viewplane_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3ViewPlaneCameraData			*instanceData = (TQ3ViewPlaneCameraData *)       privateData;
	const TQ3ViewPlaneCameraData	*cameraData   = (const TQ3ViewPlaneCameraData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	*instanceData = *cameraData;
	return(kQ3Success);
}





//=============================================================================
//      e3camera_viewplane_metahandler : View Plane camera metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3camera_viewplane_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3camera_viewplane_new;
			break;

		case kQ3XMethodTypeCameraFrustumMatrix:
			theMethod = (TQ3XFunctionPointer) e3camera_viewplane_frustum_matrix;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3camera_viewangle_frustum_matrix : Get the view to frustum matrix.
//-----------------------------------------------------------------------------
static void
e3camera_viewangle_frustum_matrix(TQ3CameraObject theCamera, TQ3Matrix4x4 *theMatrix)
{	TQ3ViewAngleAspectCameraData	*instanceData = (TQ3ViewAngleAspectCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewAngleAspect);
	float							zNear, zFar, oneOverZFar;
	float							w, h, q;
	float							a, c;



	// Initialise ourselves
	zNear       = instanceData->cameraData.range.hither;
	zFar        = instanceData->cameraData.range.yon;
	oneOverZFar = 1.0f / zFar;
	
	a = 1.0f / (1.0f - zNear * oneOverZFar);
	c = (-zNear * a) / (zNear * zFar);

	if (instanceData->aspectRatioXToY <= 1.0f)
		w = 1.0f / (float) tan(instanceData->fov * 0.5f);
	else
		w = 1.0f / (float) tan(instanceData->fov * 0.5f) / instanceData->aspectRatioXToY;

	h = w * instanceData->aspectRatioXToY;
	q = zFar / (zFar - zNear);



	// Set up the matrix
	Q3Matrix4x4_SetIdentity(theMatrix);
	theMatrix->value[0][0] = w * oneOverZFar;
	theMatrix->value[1][1] = h * oneOverZFar;
	theMatrix->value[2][2] = a * oneOverZFar;
	theMatrix->value[2][3] = c / q;
	theMatrix->value[3][2] = q * zNear * oneOverZFar;
	theMatrix->value[3][3] = 0.0f;
}





//=============================================================================
//      e3camera_viewangle_new : View Angle camera new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3camera_viewangle_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3ViewAngleAspectCameraData			*instanceData = (TQ3ViewAngleAspectCameraData *)       privateData;
	const TQ3ViewAngleAspectCameraData		*cameraData   = (const TQ3ViewAngleAspectCameraData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	*instanceData = *cameraData;
	return(kQ3Success);
}





//=============================================================================
//      e3camera_viewangle_metahandler : View Angle camera metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3camera_viewangle_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3camera_viewangle_new;
			break;

		case kQ3XMethodTypeCameraFrustumMatrix:
			theMethod = (TQ3XFunctionPointer) e3camera_viewangle_frustum_matrix;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Camera_RegisterClass : Register the camera classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Camera_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the camera classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeShape,
											kQ3ShapeTypeCamera,
											kQ3ClassNameCamera,
											NULL,
											0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeCamera,
												kQ3CameraTypeOrthographic,
												kQ3ClassNameCameraOrthographic,
												e3camera_orthographic_metahandler,
												sizeof(TQ3OrthographicCameraData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeCamera,
												kQ3CameraTypeViewPlane,
												kQ3ClassNameCameraViewPlane,
												e3camera_viewplane_metahandler,
												sizeof(TQ3ViewPlaneCameraData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeCamera,
												kQ3CameraTypeViewAngleAspect,
												kQ3ClassNameCameraViewAngle,
												e3camera_viewangle_metahandler,
												sizeof(TQ3ViewAngleAspectCameraData));

	return(qd3dStatus);
}





//=============================================================================
//      E3Camera_UnregisterClass : Unregister the camera classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the camera classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3CameraTypeViewAngleAspect, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3CameraTypeViewPlane,       kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3CameraTypeOrthographic,    kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShapeTypeCamera,           kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Camera_GetType : Return the type of a camera.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Camera_GetType(TQ3CameraObject theCamera)
{


	// Return the type
	return(E3ClassTree_GetObjectType(theCamera, kQ3ShapeTypeCamera));
}





//=============================================================================
//      E3Camera_SetData : Set the data for a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_SetData(TQ3CameraObject theCamera, const TQ3CameraData *cameraData)
{	TQ3CameraData		*instanceData = (TQ3CameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3ObjectTypeLeaf);



	// Set the camera data
	*instanceData = *cameraData;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3Camera_GetData : Return the data for a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_GetData(TQ3CameraObject theCamera, TQ3CameraData *cameraData)
{	TQ3CameraData		*instanceData = (TQ3CameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3ObjectTypeLeaf);



	// Return the camera data
	*cameraData = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3Camera_SetPlacement : Set the placement for a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_SetPlacement(TQ3CameraObject theCamera, const TQ3CameraPlacement *placement)
{	TQ3CameraData		*instanceData = (TQ3CameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3ObjectTypeLeaf);



	// Set the camera placement
	instanceData->placement = *placement;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3Camera_GetPlacement : Return the placement of a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_GetPlacement(TQ3CameraObject theCamera, TQ3CameraPlacement *placement)
{	TQ3CameraData		*instanceData = (TQ3CameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3ObjectTypeLeaf);



	// Return the camera placement
	*placement = instanceData->placement;

	return(kQ3Success);
}





//=============================================================================
//      E3Camera_SetRange : Set the range for a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_SetRange(TQ3CameraObject theCamera, const TQ3CameraRange *range)
{	TQ3CameraData		*instanceData = (TQ3CameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3ObjectTypeLeaf);



	// Set the camera range
	instanceData->range = *range;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3Camera_GetRange : Return the range of a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_GetRange(TQ3CameraObject theCamera, TQ3CameraRange *range)
{	TQ3CameraData		*instanceData = (TQ3CameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3ObjectTypeLeaf);



	// Return the camera range
	*range = instanceData->range;
	return(kQ3Success);
}





//=============================================================================
//      E3Camera_SetViewPort : Set the viewport for a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_SetViewPort(TQ3CameraObject theCamera, const TQ3CameraViewPort *viewPort)
{	TQ3CameraData		*instanceData = (TQ3CameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3ObjectTypeLeaf);



	// Set the camera viewport
	instanceData->viewPort = *viewPort;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3Camera_GetViewPort : Return the viewport of a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_GetViewPort(TQ3CameraObject theCamera, TQ3CameraViewPort *viewPort)
{	TQ3CameraData		*instanceData = (TQ3CameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3ObjectTypeLeaf);



	// Return the camera viewport
	*viewPort = instanceData->viewPort;
	return(kQ3Success);
}





//=============================================================================
//      E3Camera_GetWorldToView : Return a camera's world-to-view matrix.
//-----------------------------------------------------------------------------
//		Note :	The world-to-view space transform is defined only by the
//				placement of the camera; it establishes the camera location as
//				the origin of the view space, with the view vector (that is,
//				the vector from the camera's eye toward the point of interest)
//				placed along the -z axis and the up vector placed along the
//				y axis.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_GetWorldToView(TQ3CameraObject theCamera, TQ3Matrix4x4 *worldToView)
{	TQ3CameraData		*instanceData = (TQ3CameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3ObjectTypeLeaf);
	TQ3Matrix4x4		translateMatrix;
	TQ3Vector3D			n, v, u;



	// Work out our vectors
	Q3Point3D_Subtract(&instanceData->placement.cameraLocation,
					   &instanceData->placement.pointOfInterest,
					   &n);

	Q3Vector3D_Normalize(&n, &n);
	Q3Vector3D_Normalize(&instanceData->placement.upVector, &v);

	Q3Vector3D_Cross(&v, &n, &u);
	Q3Vector3D_Normalize(&u, &u);

	Q3Vector3D_Cross(&n, &u, &v);
	Q3Vector3D_Normalize(&v, &v);



	// Set up the rotation matrix
	Q3Matrix4x4_SetIdentity(worldToView);
	
	worldToView->value[0][0] = u.x;
	worldToView->value[1][0] = u.y;
	worldToView->value[2][0] = u.z;
	
	worldToView->value[0][1] = v.x;
	worldToView->value[1][1] = v.y;
	worldToView->value[2][1] = v.z;

	worldToView->value[0][2] = n.x;
	worldToView->value[1][2] = n.y;
	worldToView->value[2][2] = n.z;



	// Multiply in the translation matrix
	Q3Matrix4x4_SetTranslate(&translateMatrix,
								-instanceData->placement.cameraLocation.x,
								-instanceData->placement.cameraLocation.y,
								-instanceData->placement.cameraLocation.z);

	Q3Matrix4x4_Multiply(&translateMatrix, worldToView, worldToView);

	return(kQ3Success);
}





//=============================================================================
//      E3Camera_GetWorldToFrustum : Return a camera's world-to-frustum matrix.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_GetWorldToFrustum(TQ3CameraObject theCamera, TQ3Matrix4x4 *worldToFrustum)
{	TQ3Matrix4x4		worldToView, viewToFrustum;
	TQ3Status			qd3dStatus;
	


	// Get the two matrices we need, and multiply them together
	qd3dStatus = Q3Camera_GetWorldToView(theCamera,   &worldToView);
	qd3dStatus = Q3Camera_GetViewToFrustum(theCamera, &viewToFrustum);

	Q3Matrix4x4_Multiply(&worldToView, &viewToFrustum, worldToFrustum);

	return(kQ3Success);
}





//=============================================================================
//      E3Camera_GetViewToFrustum : Return a camera's view-to-frustum matrix.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera_GetViewToFrustum(TQ3CameraObject theCamera, TQ3Matrix4x4 *viewToFrustum)
{	float								scaleWidth, scaleHeight;
	float								translateX, translateY;
	TQ3XCameraFrustumMatrixMethod		frustumMatrixMethod;
	TQ3Matrix4x4						viewPortMatrix;
	TQ3CameraViewPort					viewPort;



	// Initialise a return value
	Q3Matrix4x4_SetIdentity(viewToFrustum);



	// Get the camera method
	frustumMatrixMethod = (TQ3XCameraFrustumMatrixMethod)
						  E3ClassTree_GetMethodByObject(theCamera, kQ3XMethodTypeCameraFrustumMatrix);
	if (frustumMatrixMethod == NULL)
		return(kQ3Failure);



	// Get the view to frustum matrix
	//
	// This matrix transforms the viewing coordinate system to the canonical
	// frustum. This ranges from -1 to +1 in x and y, and 0 to -1 in z (where
	// 0 is the near clip plane, and -1 is the  far clip plane).
	frustumMatrixMethod(theCamera, viewToFrustum);



	// Get the camera view port
	//
	// A portion of the viewing frustum can be selected with the camera's
	// view port.
	//
	// Since the frustum sides are parallel, this can be accomplished by
	// scaling and translating the viewing frustum in order to expand some
	// portion of the xy plane to the full -1/+1 range.
	//
	// The default view port corresponds to the full -1/+1 range, and so the
	// additional transforms will be identity transforms for that case.
	Q3Camera_GetViewPort(theCamera, &viewPort);



	// Translate the view port to the origin
	translateX = viewPort.origin.x + (viewPort.width  / 2.0f);
	translateY = viewPort.origin.y - (viewPort.height / 2.0f);

	Q3Matrix4x4_SetTranslate(&viewPortMatrix, -translateX, -translateY, 0.0f);
	Q3Matrix4x4_Multiply(viewToFrustum, &viewPortMatrix, viewToFrustum);



	// Scale it back up to -1 to +1 in x and y
	scaleWidth  = 2.0f / viewPort.width;
	scaleHeight = 2.0f / viewPort.height;

 	Q3Matrix4x4_SetScale(&viewPortMatrix, scaleWidth, scaleHeight, 1.0f);
	Q3Matrix4x4_Multiply(viewToFrustum, &viewPortMatrix, viewToFrustum);

	return(kQ3Success);
}





//=============================================================================
//      E3OrthographicCamera_New : Create an orthographic camera.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3CameraObject
E3OrthographicCamera_New(const TQ3OrthographicCameraData *orthographicData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3CameraTypeOrthographic, kQ3False, orthographicData);
	return(theObject);
}





//=============================================================================
//      E3OrthographicCamera_GetData : Return an orthographic camera's data.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera_GetData(TQ3CameraObject theCamera, TQ3OrthographicCameraData *cameraData)
{	TQ3OrthographicCameraData		*instanceData = (TQ3OrthographicCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeOrthographic);



	// Return the camera's data
	*cameraData = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3OrthographicCamera_SetData : Set the data for an orthographic camera.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera_SetData(TQ3CameraObject theCamera, const TQ3OrthographicCameraData *cameraData)
{	TQ3OrthographicCameraData		*instanceData = (TQ3OrthographicCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeOrthographic);



	// Return the camera's data
	*instanceData = *cameraData;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3OrthographicCamera_SetLeft : Set the left field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera_SetLeft(TQ3CameraObject theCamera, float left)
{	TQ3OrthographicCameraData		*instanceData = (TQ3OrthographicCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeOrthographic);



	// Set the field
	instanceData->left = left;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3OrthographicCamera_GetLeft : Return the left field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera_GetLeft(TQ3CameraObject theCamera, float *left)
{	TQ3OrthographicCameraData		*instanceData = (TQ3OrthographicCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeOrthographic);



	// Return the field
	*left = instanceData->left;
	return(kQ3Success);
}





//=============================================================================
//      E3OrthographicCamera_SetTop : Set the top field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera_SetTop(TQ3CameraObject theCamera, float top)
{	TQ3OrthographicCameraData		*instanceData = (TQ3OrthographicCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeOrthographic);



	// Set the field
	instanceData->top = top;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3OrthographicCamera_GetTop : Return the top field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera_GetTop(TQ3CameraObject theCamera, float *top)
{	TQ3OrthographicCameraData		*instanceData = (TQ3OrthographicCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeOrthographic);



	// Return the field
	*top = instanceData->top;
	return(kQ3Success);
}





//=============================================================================
//      E3OrthographicCamera_SetRight : Set the right field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera_SetRight(TQ3CameraObject theCamera, float right)
{	TQ3OrthographicCameraData		*instanceData = (TQ3OrthographicCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeOrthographic);



	// Set the field
	instanceData->right = right;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3OrthographicCamera_GetRight : Return the right field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera_GetRight(TQ3CameraObject theCamera, float *right)
{	TQ3OrthographicCameraData		*instanceData = (TQ3OrthographicCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeOrthographic);



	// Return the field
	*right = instanceData->right;
	return(kQ3Success);
}





//=============================================================================
//      E3OrthographicCamera_SetBottom : Set the bottom field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera_SetBottom(TQ3CameraObject theCamera, float bottom)
{	TQ3OrthographicCameraData		*instanceData = (TQ3OrthographicCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeOrthographic);



	// Set the field
	instanceData->bottom = bottom;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3OrthographicCamera_GetBottom : Return the bottom field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera_GetBottom(TQ3CameraObject theCamera, float *bottom)
{	TQ3OrthographicCameraData		*instanceData = (TQ3OrthographicCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeOrthographic);



	// Return the field
	*bottom = instanceData->bottom;
	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_New : Create a view plane camera.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3CameraObject
E3ViewPlaneCamera_New(const TQ3ViewPlaneCameraData *cameraData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3CameraTypeViewPlane, kQ3False, cameraData);
	return(theObject);
}





//=============================================================================
//      E3ViewPlaneCamera_GetData : Return a view plane camera's data.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_GetData(TQ3CameraObject theCamera, TQ3ViewPlaneCameraData *cameraData)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Return the camera's data
	*cameraData = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_SetData : Set the data for a view plane camera.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_SetData(TQ3CameraObject theCamera, const TQ3ViewPlaneCameraData *cameraData)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Set the camera's data
	*instanceData = *cameraData;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_SetViewPlane : Set the view plane field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_SetViewPlane(TQ3CameraObject theCamera, float viewPlane)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Set the field
	instanceData->viewPlane = viewPlane;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_GetViewPlane : Return the view plane field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_GetViewPlane(TQ3CameraObject theCamera, float *viewPlane)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Return the field
	*viewPlane = instanceData->viewPlane;
	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_SetHalfWidth : Set the half width field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_SetHalfWidth(TQ3CameraObject theCamera, float halfWidthAtViewPlane)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Set the field
	instanceData->halfWidthAtViewPlane = halfWidthAtViewPlane;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_GetHalfWidth : Return the half width field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_GetHalfWidth(TQ3CameraObject theCamera, float *halfWidthAtViewPlane)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Return the field
	*halfWidthAtViewPlane = instanceData->halfWidthAtViewPlane;
	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_SetHalfHeight : Set the half height field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_SetHalfHeight(TQ3CameraObject theCamera, float halfHeightAtViewPlane)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Set the field
	instanceData->halfHeightAtViewPlane = halfHeightAtViewPlane;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_GetHalfHeight : Return the half height field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_GetHalfHeight(TQ3CameraObject theCamera, float *halfHeightAtViewPlane)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Return the field
	*halfHeightAtViewPlane = instanceData->halfHeightAtViewPlane;
	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_SetCenterX : Set the center X field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_SetCenterX(TQ3CameraObject theCamera, float centerXOnViewPlane)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Set the field
	instanceData->centerXOnViewPlane = centerXOnViewPlane;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_GetCenterX : Return the center X field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_GetCenterX(TQ3CameraObject theCamera, float *centerXOnViewPlane)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Return the field
	*centerXOnViewPlane = instanceData->centerXOnViewPlane;
	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_SetCenterY : Set the center Y field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_SetCenterY(TQ3CameraObject theCamera, float centerYOnViewPlane)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Set the field
	instanceData->centerYOnViewPlane = centerYOnViewPlane;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3ViewPlaneCamera_GetCenterY : Return the center Y field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera_GetCenterY(TQ3CameraObject theCamera, float *centerYOnViewPlane)
{	TQ3ViewPlaneCameraData		*instanceData = (TQ3ViewPlaneCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewPlane);



	// Return the field
	*centerYOnViewPlane = instanceData->centerYOnViewPlane;
	return(kQ3Success);
}





//=============================================================================
//      E3ViewAngleAspectCamera_New : Create a view angle camera.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3CameraObject
E3ViewAngleAspectCamera_New(const TQ3ViewAngleAspectCameraData *cameraData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3CameraTypeViewAngleAspect, kQ3False, cameraData);
	return(theObject);
}





//=============================================================================
//      E3ViewAngleAspectCamera_SetData : Set the data for a view angle camera.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera_SetData(TQ3CameraObject theCamera, const TQ3ViewAngleAspectCameraData *cameraData)
{	TQ3ViewAngleAspectCameraData		*instanceData = (TQ3ViewAngleAspectCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewAngleAspect);



	// Set the camera's data
	*instanceData = *cameraData;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3ViewAngleAspectCamera_GetData : Return a view angle camera.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera_GetData(TQ3CameraObject theCamera, TQ3ViewAngleAspectCameraData *cameraData)
{	TQ3ViewAngleAspectCameraData		*instanceData = (TQ3ViewAngleAspectCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewAngleAspect);



	// Return the camera's data
	*cameraData = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3ViewAngleAspectCamera_SetFOV : Set the FOV field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera_SetFOV(TQ3CameraObject theCamera, float fov)
{	TQ3ViewAngleAspectCameraData		*instanceData = (TQ3ViewAngleAspectCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewAngleAspect);



	// Set the field
	instanceData->fov = fov;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3ViewAngleAspectCamera_GetFOV : Return the FOV field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera_GetFOV(TQ3CameraObject theCamera, float *fov)
{	TQ3ViewAngleAspectCameraData		*instanceData = (TQ3ViewAngleAspectCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewAngleAspect);



	// Return the field
	*fov = instanceData->fov;
	return(kQ3Success);
}





//=============================================================================
//      E3ViewAngleAspectCamera_SetAspectRatio : Set the aspect ratio field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera_SetAspectRatio(TQ3CameraObject theCamera, float aspectRatioXToY)
{	TQ3ViewAngleAspectCameraData		*instanceData = (TQ3ViewAngleAspectCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewAngleAspect);



	// Set the field
	instanceData->aspectRatioXToY = aspectRatioXToY;
	Q3Shared_Edited(theCamera);

	return(kQ3Success);
}





//=============================================================================
//      E3ViewAngleAspectCamera_GetAspectRatio : Return the aspect ratio field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera_GetAspectRatio(TQ3CameraObject theCamera, float *aspectRatioXToY)
{	TQ3ViewAngleAspectCameraData		*instanceData = (TQ3ViewAngleAspectCameraData *) E3ClassTree_FindInstanceData(theCamera, kQ3CameraTypeViewAngleAspect);



	// Return the field
	*aspectRatioXToY = instanceData->aspectRatioXToY;
	return(kQ3Success);
}






/*  NAME:
        E3Transform.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Transform.h"
#include "E3View.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3transform_matrix_matrix : Return the matrix representation.
//-----------------------------------------------------------------------------
static void
e3transform_matrix_matrix(void *transformData, TQ3Matrix4x4 *theMatrix)
{	TQ3Matrix4x4		*instanceData = (TQ3Matrix4x4 *) transformData;



	// Return the matrix representation
	*theMatrix = *instanceData;
}





//=============================================================================
//      e3transform_matrix_render : Matrix render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_matrix_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3Matrix4x4		*instanceData = (const TQ3Matrix4x4 *) objectData;
	TQ3Status				qd3dStatus;



	// Update the view
	qd3dStatus = E3View_State_AddMatrixLocalToWorld(theView, instanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3transform_matrix_metahandler : Matrix metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_matrix_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeTransformMatrix:
			theMethod = (TQ3XFunctionPointer) e3transform_matrix_matrix;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3transform_matrix_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3transform_rotate_matrix : Return the matrix representation.
//-----------------------------------------------------------------------------
static void
e3transform_rotate_matrix(const void *transformData, TQ3Matrix4x4 *theMatrix)
{	const TQ3RotateTransformData		*instanceData = (const TQ3RotateTransformData *) transformData;



	// Return the matrix representation
	switch (instanceData->axis) {
		case kQ3AxisX:
			Q3Matrix4x4_SetRotate_X(theMatrix, instanceData->radians);
			break;

		case kQ3AxisY:
			Q3Matrix4x4_SetRotate_Y(theMatrix, instanceData->radians);
			break;

		case kQ3AxisZ:
			Q3Matrix4x4_SetRotate_Z(theMatrix, instanceData->radians);
			break;
		
		default:
			Q3Matrix4x4_SetIdentity(theMatrix);
			break;
		}
}





//=============================================================================
//      e3transform_rotate_render : Rotate render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_rotate_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3Matrix4x4		theMatrix;
#pragma unused(objectType)
#pragma unused(theObject)



	// Convert the transform to a matrix, and update the view
	e3transform_rotate_matrix(objectData, &theMatrix);

	qd3dStatus = E3View_State_AddMatrixLocalToWorld(theView, &theMatrix);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3transform_rotate_metahandler : Rotate metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_rotate_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeTransformMatrix:
			theMethod = (TQ3XFunctionPointer) e3transform_rotate_matrix;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3transform_rotate_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3transform_rotateaboutpoint_matrix : Return the matrix.
//-----------------------------------------------------------------------------
static void
e3transform_rotateaboutpoint_matrix(const void *transformData, TQ3Matrix4x4 *theMatrix)
{
	const TQ3RotateAboutPointTransformData* instanceData = ( const TQ3RotateAboutPointTransformData* ) transformData ;
	float cosAngle ;
	float sinAngle ;


	// Return the matrix representation
	cosAngle = (float) cos(instanceData->radians);
	sinAngle = (float) sin(instanceData->radians);

	#define M(X,Y)			theMatrix->value[X][Y]
	#define Dx				instanceData->about.x
	#define Dy				instanceData->about.y
	#define Dz				instanceData->about.z

	Q3Memory_Clear(theMatrix, sizeof(TQ3Matrix4x4));

	switch (instanceData->axis)
		{
		case kQ3AxisX:
			{
			M(0,0) = 1.0f;
			M(1,1) = cosAngle;
			M(1,2) = sinAngle;
			M(2,1) = - sinAngle;
			M(2,2) = cosAngle;
			M(3,1) = Dy - Dy * cosAngle + Dz * sinAngle;
			M(3,2) = Dz - Dy * sinAngle - Dz * cosAngle;
			M(3,3) = 1.0f;
			break;
			}

		case kQ3AxisY:
			{
			M(0,0) = cosAngle;
			M(0,2) = -sinAngle;
			M(1,1) = 1.0f;
			M(2,0) = sinAngle;
			M(2,2) = cosAngle;
			M(3,0) = Dx - Dx * cosAngle - Dz * sinAngle;
			M(3,2) = Dz + Dx * sinAngle - Dz * cosAngle;
			M(3,3) = 1.0f;
			break;
			}
		case kQ3AxisZ:
			{
			M(0,0) = cosAngle;
			M(0,1) = sinAngle;
			M(1,0) = -sinAngle;
			M(1,1) = cosAngle;
			M(2,2) = 1.0f;
			M(3,0) = Dx - Dx * cosAngle + Dy * sinAngle;
			M(3,1) = Dy - Dx * sinAngle - Dy * cosAngle;
			M(3,3) = 1.0f;
			break;
			}
		}

	#undef M
	#undef Dx
	#undef Dy
	#undef Dz
}





//=============================================================================
//      e3transform_rotateaboutpoint_render : Rotate about point render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_rotateaboutpoint_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3Matrix4x4		theMatrix;
#pragma unused(objectType)
#pragma unused(theObject)



	// Convert the transform to a matrix, and update the view
	e3transform_rotateaboutpoint_matrix(objectData, &theMatrix);

	qd3dStatus = E3View_State_AddMatrixLocalToWorld(theView, &theMatrix);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3transform_rotateaboutpoint_metahandler : Rotate point metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_rotateaboutpoint_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeTransformMatrix:
			theMethod = (TQ3XFunctionPointer) e3transform_rotateaboutpoint_matrix;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3transform_rotateaboutpoint_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3transform_rotateaboutaxis_matrix : Return the matrix.
//-----------------------------------------------------------------------------
static void
e3transform_rotateaboutaxis_matrix(const void *transformData, TQ3Matrix4x4 *theMatrix)
{	const TQ3RotateAboutAxisTransformData		*instanceData = (const TQ3RotateAboutAxisTransformData *) transformData;
	TQ3Vector3D									normalisedOrientation;



	// Return the matrix representation
	Q3Vector3D_Normalize ( & ( instanceData->orientation ) , &normalisedOrientation ) ;
	Q3Matrix4x4_SetRotateAboutAxis ( theMatrix ,
									 & ( instanceData->origin ) ,
									 & normalisedOrientation ,
									 instanceData->radians ) ;
}





//=============================================================================
//      e3transform_rotateaboutaxis_render : Rotate about axis render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_rotateaboutaxis_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3Matrix4x4		theMatrix;
#pragma unused(objectType)
#pragma unused(theObject)



	// Convert the transform to a matrix, and update the view
	e3transform_rotateaboutaxis_matrix(objectData, &theMatrix);

	qd3dStatus = E3View_State_AddMatrixLocalToWorld(theView, &theMatrix);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3transform_rotateaboutaxis_metahandler : Rotate axis metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_rotateaboutaxis_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeTransformMatrix:
			theMethod = (TQ3XFunctionPointer) e3transform_rotateaboutaxis_matrix;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3transform_rotateaboutaxis_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3transform_scale_matrix : Return the matrix representation.
//-----------------------------------------------------------------------------
static void
e3transform_scale_matrix(const void *transformData, TQ3Matrix4x4 *theMatrix)
{	const TQ3Vector3D		*instanceData = (const TQ3Vector3D *) transformData;



	// Return the matrix representation
	Q3Matrix4x4_SetScale(theMatrix, instanceData->x, instanceData->y, instanceData->z);
}





//=============================================================================
//      e3transform_scale_render : Scale render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_scale_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3Matrix4x4		theMatrix;
#pragma unused(objectType)
#pragma unused(theObject)



	// Convert the transform to a matrix, and update the view
	e3transform_scale_matrix(objectData, &theMatrix);

	qd3dStatus = E3View_State_AddMatrixLocalToWorld(theView, &theMatrix);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3transform_scale_metahandler : Scale metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_scale_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeTransformMatrix:
			theMethod = (TQ3XFunctionPointer) e3transform_scale_matrix;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3transform_scale_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3transform_translate_matrix : Return the matrix representation.
//-----------------------------------------------------------------------------
static void
e3transform_translate_matrix(const void *transformData, TQ3Matrix4x4 *theMatrix)
{	const TQ3Vector3D		*instanceData = (const TQ3Vector3D *) transformData;



	// Return the matrix representation
	Q3Matrix4x4_SetTranslate(theMatrix, instanceData->x, instanceData->y, instanceData->z);
}





//=============================================================================
//      e3transform_translate_render : Translate render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_translate_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3Matrix4x4		theMatrix;
#pragma unused(objectType)
#pragma unused(theObject)



	// Convert the transform to a matrix, and update the view
	e3transform_translate_matrix(objectData, &theMatrix);

	qd3dStatus = E3View_State_AddMatrixLocalToWorld(theView, &theMatrix);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3transform_translate_metahandler : Translate metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_translate_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeTransformMatrix:
			theMethod = (TQ3XFunctionPointer) e3transform_translate_matrix;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3transform_translate_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3transform_quaternion_matrix : Return the matrix representation.
//-----------------------------------------------------------------------------
static void
e3transform_quaternion_matrix(const void *transformData, TQ3Matrix4x4 *theMatrix)
{	const TQ3Quaternion		*instanceData = (const TQ3Quaternion *) transformData;



	// Return the matrix representation
	Q3Matrix4x4_SetQuaternion(theMatrix, instanceData);
}





//=============================================================================
//      e3transform_quaternion_render : Quaternion render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_quaternion_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3Matrix4x4		theMatrix;
#pragma unused(objectType)
#pragma unused(theObject)



	// Convert the transform to a matrix, and update the view
	e3transform_quaternion_matrix(objectData, &theMatrix);

	qd3dStatus = E3View_State_AddMatrixLocalToWorld(theView, &theMatrix);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3transform_quaternion_metahandler : Quaternion metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_quaternion_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeTransformMatrix:
			theMethod = (TQ3XFunctionPointer) e3transform_quaternion_matrix;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3transform_quaternion_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3transform_reset_matrix : Return the matrix representation.
//-----------------------------------------------------------------------------
static void
e3transform_reset_matrix(const void *transformData, TQ3Matrix4x4 *theMatrix)
{
#pragma unused(transformData)



	// Return the matrix representation
	Q3Matrix4x4_SetIdentity(theMatrix);
}





//=============================================================================
//      e3transform_reset_render : Reset render method.
//-----------------------------------------------------------------------------
//		Note :	The reset transform is not cumulative - it resets the local to
//				world matrix of the view to the identity matrix.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_reset_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3Matrix4x4		theMatrix;
#pragma unused(objectType)
#pragma unused(theObject)



	// Reset the view's local to world matrix
	Q3Matrix4x4_Invert(E3View_State_GetLocalToWorld(theView), &theMatrix);

	qd3dStatus = E3View_State_AddMatrixLocalToWorld(theView, &theMatrix);

	return(kQ3Success);
}





//=============================================================================
//      e3transform_reset_metahandler : Reset metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_reset_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeTransformMatrix:
			theMethod = (TQ3XFunctionPointer) e3transform_reset_matrix;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3transform_reset_render;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3transform_metahandler : base metahandler for transforms.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectIsDrawable:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Transform_RegisterClass : Register the transform classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Transform_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the camera classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeShape,
											kQ3ShapeTypeTransform,
											kQ3ClassNameTransform,
											e3transform_metahandler,
											0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeTransform,
												kQ3TransformTypeMatrix,
												kQ3ClassNameTransformMatrix,
												e3transform_matrix_metahandler,
												sizeof(TQ3Matrix4x4));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeTransform,
												kQ3TransformTypeRotate,
												kQ3ClassNameTransformRotate,
												e3transform_rotate_metahandler,
												sizeof(TQ3RotateTransformData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeTransform,
												kQ3TransformTypeRotateAboutPoint,
												kQ3ClassNameTransformRotateAboutPoint,
												e3transform_rotateaboutpoint_metahandler,
												sizeof(TQ3RotateAboutPointTransformData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeTransform,
												kQ3TransformTypeRotateAboutAxis,
												kQ3ClassNameTransformRotateAboutAxis,
												e3transform_rotateaboutaxis_metahandler,
												sizeof(TQ3RotateAboutAxisTransformData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeTransform,
												kQ3TransformTypeScale,
												kQ3ClassNameTransformScale,
												e3transform_scale_metahandler,
												sizeof(TQ3Vector3D));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeTransform,
												kQ3TransformTypeTranslate,
												kQ3ClassNameTransformTranslate,
												e3transform_translate_metahandler,
												sizeof(TQ3Vector3D));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeTransform,
												kQ3TransformTypeQuaternion,
												kQ3ClassNameTransformQuaternion,
												e3transform_quaternion_metahandler,
												sizeof(TQ3Quaternion));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeTransform,
												kQ3TransformTypeReset,
												kQ3ClassNameTransformReset,
												e3transform_reset_metahandler,
												0);

	return(qd3dStatus);
}





//=============================================================================
//      E3Transform_UnregisterClass : Unregister the transform classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Transform_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the camera classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3TransformTypeReset,            kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3TransformTypeQuaternion,       kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3TransformTypeTranslate,        kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3TransformTypeScale,            kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3TransformTypeRotateAboutAxis,  kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3TransformTypeRotateAboutPoint, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3TransformTypeRotate,           kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3TransformTypeMatrix,           kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShapeTypeTransform,            kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Transform_GetType : Return the type of a transform.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Transform_GetType(TQ3TransformObject theTransform)
{


	// Get the type
	return(E3ClassTree_GetObjectType(theTransform, kQ3ShapeTypeTransform));
}





//=============================================================================
//      E3Transform_GetMatrix : Return the matrix for the transform.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Transform_GetMatrix(TQ3TransformObject theTransform, TQ3Matrix4x4 *theMatrix)
{	TQ3XTransformMatrixMethod		matrixMethod;



	// Get the matrix method for the transform
	matrixMethod = (TQ3XTransformMatrixMethod)
					E3ClassTree_GetMethod(theTransform->theClass, kQ3XMethodTypeTransformMatrix);



	// Call the method, or revert to the identity matrix on error
	if (matrixMethod != NULL)
		matrixMethod(theTransform->instanceData, theMatrix);
	else
		Q3Matrix4x4_SetIdentity(theMatrix);
	
	return(theMatrix);
}





//=============================================================================
//      E3Transform_Submit : Submit a transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3Transform_Submit(TQ3TransformObject theTransform, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitRetained(theView, theTransform);

	return(qd3dStatus);
}





//=============================================================================
//      E3MatrixTransform_New : Create a new matrix transform.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TransformObject
E3MatrixTransform_New(const TQ3Matrix4x4 *theMatrix)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3TransformTypeMatrix, kQ3False, theMatrix);

	return(theObject);
}





//=============================================================================
//      E3MatrixTransform_Submit : Submit the transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3MatrixTransform_Submit(const TQ3Matrix4x4 *theMatrix, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3TransformTypeMatrix, theMatrix);

	return(qd3dStatus);
}





//=============================================================================
//      E3MatrixTransform_Set : Set the matrix for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3MatrixTransform_Set(TQ3TransformObject theTransform, const TQ3Matrix4x4 *theMatrix)
{	TQ3Matrix4x4	*instanceData = (TQ3Matrix4x4 *) theTransform->instanceData;



	// Set the data
	*instanceData = *theMatrix;
	Q3Shared_Edited(theTransform);

	return(kQ3Success);
}





//=============================================================================
//      E3MatrixTransform_Get : Get the matrix for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3MatrixTransform_Get(TQ3TransformObject theTransform, TQ3Matrix4x4 *theMatrix)
{	TQ3Matrix4x4	*instanceData = (TQ3Matrix4x4 *) theTransform->instanceData;



	// Get the data
	*theMatrix = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3RotateTransform_New : Create a new rotate transform.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TransformObject
E3RotateTransform_New(const TQ3RotateTransformData *data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3TransformTypeRotate, kQ3False, data);

	return(theObject);
}





//=============================================================================
//      E3RotateTransform_Submit : Submit the transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_Submit(const TQ3RotateTransformData *data, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3TransformTypeRotate, data);

	return(qd3dStatus);
}





//=============================================================================
//      E3RotateTransform_SetData : Set the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_SetData(TQ3TransformObject theTransform, const TQ3RotateTransformData *data)
{	TQ3RotateTransformData	*instanceData = (TQ3RotateTransformData *) theTransform->instanceData;



	// Set the data
	*instanceData = *data;
	Q3Shared_Edited(theTransform);

	return(kQ3Success);
}





//=============================================================================
//      E3RotateTransform_GetData : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_GetData(TQ3TransformObject theTransform, TQ3RotateTransformData *data)
{	TQ3RotateTransformData	*instanceData = (TQ3RotateTransformData *) theTransform->instanceData;



	// Get the data
	*data = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3RotateTransform_SetAxis : Set the axis for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_SetAxis(TQ3TransformObject theTransform, TQ3Axis axis)
{	TQ3RotateTransformData	*instanceData = (TQ3RotateTransformData *) theTransform->instanceData;



	// Set the field
	instanceData->axis = axis;
	Q3Shared_Edited(theTransform);
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateTransform_GetAxis : Get the axis for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_GetAxis(TQ3TransformObject theTransform, TQ3Axis *axis)
{	TQ3RotateTransformData	*instanceData = (TQ3RotateTransformData *) theTransform->instanceData;



	// Get the field
	*axis = instanceData->axis;
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateTransform_SetAngle : Set the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_SetAngle(TQ3TransformObject theTransform, float radians)
{	TQ3RotateTransformData	*instanceData = (TQ3RotateTransformData *) theTransform->instanceData;



	// Set the field
	instanceData->radians = radians;
	Q3Shared_Edited(theTransform);
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateTransform_GetAngle : Get the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_GetAngle(TQ3TransformObject theTransform, float *radians)
{	TQ3RotateTransformData	*instanceData = (TQ3RotateTransformData *) theTransform->instanceData;



	// Get the field
	*radians = instanceData->radians;
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutPointTransform_New : Create a rotate-point transform.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TransformObject
E3RotateAboutPointTransform_New(const TQ3RotateAboutPointTransformData *data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3TransformTypeRotateAboutPoint, kQ3False, data);

	return(theObject);
}





//=============================================================================
//      E3RotateAboutPointTransform_Submit : Submit the transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_Submit(const TQ3RotateAboutPointTransformData *data, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3TransformTypeRotateAboutPoint, data);

	return(qd3dStatus);
}





//=============================================================================
//      E3RotateAboutPointTransform_SetData : Set the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_SetData(TQ3TransformObject theTransform, const TQ3RotateAboutPointTransformData *data)
{	TQ3RotateAboutPointTransformData	*instanceData = (TQ3RotateAboutPointTransformData *) theTransform->instanceData;



	// Set the data
	*instanceData = *data;
	Q3Shared_Edited(theTransform);

	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutPointTransform_GetData : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_GetData(TQ3TransformObject theTransform, TQ3RotateAboutPointTransformData *data)
{	TQ3RotateAboutPointTransformData	*instanceData = (TQ3RotateAboutPointTransformData *) theTransform->instanceData;



	// Get the data
	*data = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutPointTransform_SetAxis : Set the axis for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_SetAxis(TQ3TransformObject theTransform, TQ3Axis axis)
{	TQ3RotateAboutPointTransformData	*instanceData = (TQ3RotateAboutPointTransformData *) theTransform->instanceData;



	// Set the field
	instanceData->axis = axis;
	Q3Shared_Edited(theTransform);
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutPointTransform_GetAxis : Get the axis for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_GetAxis(TQ3TransformObject theTransform, TQ3Axis *axis)
{	TQ3RotateAboutPointTransformData	*instanceData = (TQ3RotateAboutPointTransformData *) theTransform->instanceData;



	// Get the field
	*axis = instanceData->axis;
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutPointTransform_SetAngle : Set the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_SetAngle(TQ3TransformObject theTransform, float radians)
{	TQ3RotateAboutPointTransformData	*instanceData = (TQ3RotateAboutPointTransformData *) theTransform->instanceData;



	// Set the field
	instanceData->radians = radians;
	Q3Shared_Edited(theTransform);
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutPointTransform_GetAngle : Get the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_GetAngle(TQ3TransformObject theTransform, float *radians)
{	TQ3RotateAboutPointTransformData	*instanceData = (TQ3RotateAboutPointTransformData *) theTransform->instanceData;



	// Get the field
	*radians = instanceData->radians;
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutPointTransform_SetAboutPoint : Set the rotation point.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_SetAboutPoint(TQ3TransformObject theTransform, const TQ3Point3D *about)
{	TQ3RotateAboutPointTransformData	*instanceData = (TQ3RotateAboutPointTransformData *) theTransform->instanceData;



	// Set the field
	instanceData->about = *about;
	Q3Shared_Edited(theTransform);
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutPointTransform_GetAboutPoint : Get the rotation point.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_GetAboutPoint(TQ3TransformObject theTransform, TQ3Point3D *about)
{	TQ3RotateAboutPointTransformData	*instanceData = (TQ3RotateAboutPointTransformData *) theTransform->instanceData;



	// Get the field
	*about = instanceData->about;
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutAxisTransform_New : Create a rotate-axis transform.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TransformObject
E3RotateAboutAxisTransform_New(const TQ3RotateAboutAxisTransformData *data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3TransformTypeRotateAboutAxis, kQ3False, data);

	return(theObject);
}





//=============================================================================
//      E3RotateAboutAxisTransform_Submit : Submit the transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_Submit(const TQ3RotateAboutAxisTransformData *data, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3TransformTypeRotateAboutAxis, data);

	return(qd3dStatus);
}





//=============================================================================
//      E3RotateAboutAxisTransform_SetData : Set the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_SetData(TQ3TransformObject theTransform, const TQ3RotateAboutAxisTransformData *data)
{	TQ3RotateAboutAxisTransformData	*instanceData = (TQ3RotateAboutAxisTransformData *) theTransform->instanceData;



	// Set the data
	*instanceData = *data;
	Q3Shared_Edited(theTransform);

	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutAxisTransform_GetData : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_GetData(TQ3TransformObject theTransform, TQ3RotateAboutAxisTransformData *data)
{	TQ3RotateAboutAxisTransformData	*instanceData = (TQ3RotateAboutAxisTransformData *) theTransform->instanceData;



	// Get the data
	*data = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutAxisTransform_SetOrientation : Set the orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_SetOrientation(TQ3TransformObject theTransform, const TQ3Vector3D *axis)
{	TQ3RotateAboutAxisTransformData	*instanceData = (TQ3RotateAboutAxisTransformData *) theTransform->instanceData;



	// Set the field
	instanceData->orientation = *axis;
	Q3Shared_Edited(theTransform);
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutAxisTransform_GetOrientation : Get the orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_GetOrientation(TQ3TransformObject theTransform, TQ3Vector3D *axis)
{	TQ3RotateAboutAxisTransformData	*instanceData = (TQ3RotateAboutAxisTransformData *) theTransform->instanceData;



	// Set the field
	*axis = instanceData->orientation;
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutAxisTransform_SetAngle : Set the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_SetAngle(TQ3TransformObject theTransform, float radians)
{	TQ3RotateAboutAxisTransformData	*instanceData = (TQ3RotateAboutAxisTransformData *) theTransform->instanceData;



	// Set the field
	instanceData->radians = radians;
	Q3Shared_Edited(theTransform);
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutAxisTransform_GetAngle : Get the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_GetAngle(TQ3TransformObject theTransform, float *radians)
{	TQ3RotateAboutAxisTransformData	*instanceData = (TQ3RotateAboutAxisTransformData *) theTransform->instanceData;



	// Get the field
	*radians = instanceData->radians;
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutAxisTransform_SetOrigin : Set the origin.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_SetOrigin(TQ3TransformObject theTransform, const TQ3Point3D *origin)
{	TQ3RotateAboutAxisTransformData	*instanceData = (TQ3RotateAboutAxisTransformData *) theTransform->instanceData;



	// Set the field
	instanceData->origin = *origin;
	Q3Shared_Edited(theTransform);
	
	return(kQ3Success);
}





//=============================================================================
//      E3RotateAboutAxisTransform_GetOrigin : Get the origin.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_GetOrigin(TQ3TransformObject theTransform, TQ3Point3D *origin)
{	TQ3RotateAboutAxisTransformData	*instanceData = (TQ3RotateAboutAxisTransformData *) theTransform->instanceData;



	// Get the field
	*origin = instanceData->origin;
	
	return(kQ3Success);
}





//=============================================================================
//      E3ScaleTransform_New : Create a scale transform.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TransformObject
E3ScaleTransform_New(const TQ3Vector3D *scale)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3TransformTypeScale, kQ3False, scale);

	return(theObject);
}





//=============================================================================
//      E3ScaleTransform_Submit : Submit the transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3ScaleTransform_Submit(const TQ3Vector3D *scale, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3TransformTypeScale, scale);

	return(qd3dStatus);
}





//=============================================================================
//      E3ScaleTransform_Set : Set the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3ScaleTransform_Set(TQ3TransformObject theTransform, const TQ3Vector3D *scale)
{	TQ3Vector3D	*instanceData = (TQ3Vector3D *) theTransform->instanceData;



	// Set the data
	*instanceData = *scale;
	Q3Shared_Edited(theTransform);

	return(kQ3Success);
}





//=============================================================================
//      E3ScaleTransform_Get : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3ScaleTransform_Get(TQ3TransformObject theTransform, TQ3Vector3D *scale)
{	TQ3Vector3D	*instanceData = (TQ3Vector3D *) theTransform->instanceData;



	// Get the data
	*scale = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3TranslateTransform_New : Create a translate transform.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TransformObject
E3TranslateTransform_New(const TQ3Vector3D *translate)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3TransformTypeTranslate, kQ3False, translate);

	return(theObject);
}





//=============================================================================
//      E3TranslateTransform_Submit : Submit the transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3TranslateTransform_Submit(const TQ3Vector3D *translate, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3TransformTypeTranslate, translate);

	return(qd3dStatus);
}





//=============================================================================
//      E3TranslateTransform_Set : Set the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3TranslateTransform_Set(TQ3TransformObject theTransform, const TQ3Vector3D *translate)
{	TQ3Vector3D	*instanceData = (TQ3Vector3D *) theTransform->instanceData;



	// Set the data
	*instanceData = *translate;
	Q3Shared_Edited(theTransform);

	return(kQ3Success);
}





//=============================================================================
//      E3TranslateTransform_Get : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3TranslateTransform_Get(TQ3TransformObject theTransform, TQ3Vector3D *translate)
{	TQ3Vector3D		*instanceData = (TQ3Vector3D *) theTransform->instanceData;



	// Get the data
	*translate = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3QuaternionTransform_New : Create a quaternion transform.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TransformObject
E3QuaternionTransform_New(const TQ3Quaternion *quaternion)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3TransformTypeQuaternion, kQ3False, quaternion);

	return(theObject);
}





//=============================================================================
//      E3QuaternionTransform_Submit : Submit the transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3QuaternionTransform_Submit(const TQ3Quaternion *quaternion, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3TransformTypeQuaternion, quaternion);

	return(qd3dStatus);
}





//=============================================================================
//      E3QuaternionTransform_Set : Set the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3QuaternionTransform_Set(TQ3TransformObject theTransform, const TQ3Quaternion *quaternion)
{	TQ3Quaternion	*instanceData = (TQ3Quaternion *) theTransform->instanceData;



	// Set the data
	*instanceData = *quaternion;
	Q3Shared_Edited(theTransform);

	return(kQ3Success);
}





//=============================================================================
//      E3QuaternionTransform_Get : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3QuaternionTransform_Get(TQ3TransformObject theTransform, TQ3Quaternion *quaternion)
{	TQ3Quaternion	*instanceData = (TQ3Quaternion *) theTransform->instanceData;



	// Get the data
	*quaternion = *instanceData;

	return(kQ3Success);
}





//=============================================================================
//      E3ResetTransform_New : Create a reset transform.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TransformObject
E3ResetTransform_New(void)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3TransformTypeReset, kQ3False, NULL);

	return(theObject);
}





//=============================================================================
//      E3ResetTransform_Submit : Submit the transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3ResetTransform_Submit(TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3TransformTypeReset, NULL);

	return(qd3dStatus);
}

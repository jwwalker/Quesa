/*  NAME:
        E3Transform.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

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
#include "E3Transform.h"
#include "E3View.h"
#include "E3Main.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------



class E3RotateTransform : public E3Transform  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3TransformTypeRotate, E3RotateTransform, E3Transform )
public :

	TQ3RotateTransformData				instanceData ;
	} ;
	


class E3RotateAboutPointTransform : public E3Transform  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3TransformTypeRotateAboutPoint, E3RotateAboutPointTransform, E3Transform )
public :

	TQ3RotateAboutPointTransformData	instanceData ;
	} ;
	


class E3RotateAboutAxisTransform : public E3Transform  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3TransformTypeRotateAboutAxis, E3RotateAboutAxisTransform, E3Transform )
public :

	TQ3RotateAboutAxisTransformData		instanceData ;
	} ;
	


class E3ScaleTransform : public E3Transform  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3TransformTypeScale, E3ScaleTransform, E3Transform )
public :

	TQ3Vector3D							instanceData ;
	} ;
	


class E3TranslateTransform : public E3Transform  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3TransformTypeTranslate, E3TranslateTransform, E3Transform )
public :

	TQ3Vector3D							instanceData ;
	} ;
	


class E3QuaternionTransform : public E3Transform  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3TransformTypeQuaternion, E3QuaternionTransform, E3Transform )
public :

	TQ3Quaternion						instanceData ;
	} ;
	


class E3ResetTransform : public E3Transform  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3TransformTypeReset, E3ResetTransform, E3Transform )
public :

	// There is no extra data for this class
	} ;
	


class E3CameraTransform : public E3Transform  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3TransformTypeCamera, E3CameraTransform, E3Transform )
public :

	TQ3CameraTransformData				instanceData ;
	} ;
	


class E3CameraRasterizeTransform : public E3Transform  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3TransformTypeCameraRasterize, E3CameraRasterizeTransform, E3Transform )
public :

	// There is no extra data for this class
	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      E3TransformInfo::E3TransformInfo : Constructor for class info of the class.
//-----------------------------------------------------------------------------

E3TransformInfo::E3TransformInfo	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
		: E3ShapeInfo ( newClassMetaHandler, newParent ) ,
		matrixMethod		( (TQ3XTransformMatrixMethod)		Find_Method ( kQ3XMethodTypeTransformMatrix ) )		 
	{
	if ( matrixMethod == NULL )
		SetAbstract () ;
	}


//=============================================================================
//      e3transform_new_class_info : Method to construct a class info record.
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3transform_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent
			 	)
	{
	return new ( std::nothrow ) E3TransformInfo ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3transform_default_matrix : Return the unit matrix.
//-----------------------------------------------------------------------------
static void
e3transform_default_matrix(void *transformData, TQ3Matrix4x4 *theMatrix)
	{
	// Return the unit matrix
	Q3Matrix4x4_SetIdentity ( theMatrix ) ;
	}





//=============================================================================
//      e3transform_matrix_matrix : Return the matrix representation.
//-----------------------------------------------------------------------------
static void
e3transform_matrix_matrix(void *transformData, TQ3Matrix4x4 *theMatrix)
{	TQ3Matrix4x4		*instanceData = (TQ3Matrix4x4 *) transformData;



	// Return the matrix representation
	*theMatrix = *instanceData;
}





//=============================================================================
//      e3transform_matrix_submit : Matrix submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_matrix_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3transform_matrix_submit;
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
//      e3transform_rotate_submit : Rotate submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_rotate_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3transform_rotate_submit;
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
			
		case kQ3AxisSize32:
		default:
			Q3_ASSERT(!"Unknown axis");
			break;
		}

	#undef M
	#undef Dx
	#undef Dy
	#undef Dz
}





//=============================================================================
//      e3transform_rotateaboutpoint_submit : Rotate about point submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_rotateaboutpoint_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3transform_rotateaboutpoint_submit;
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
//      e3transform_rotateaboutaxis_submit : Rotate about axis submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_rotateaboutaxis_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3transform_rotateaboutaxis_submit;
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
//      e3transform_scale_submit : Scale submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_scale_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3transform_scale_submit;
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
//      e3transform_translate_submit : Translate submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_translate_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3transform_translate_submit;
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
//      e3transform_quaternion_submit : Quaternion submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_quaternion_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
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
			theMethod = (TQ3XFunctionPointer) e3transform_quaternion_submit;
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
//      e3transform_reset_submit : Reset submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_reset_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3Matrix4x4		theMatrix;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the view
	//
	// Unlike other transforms, the reset transform is not cumulative: it
	// simply resets the local->world matrix of the view to the identity.
	Q3Matrix4x4_SetIdentity(&theMatrix);
	
	qd3dStatus = E3View_State_SetMatrix(theView, kQ3MatrixStateLocalToWorld, &theMatrix, NULL, NULL);

	return(qd3dStatus);
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
			theMethod = (TQ3XFunctionPointer) e3transform_reset_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3transform_camera_matrix : Return the matrix representation.
//-----------------------------------------------------------------------------
static void
e3transform_camera_matrix(const void *transformData, TQ3Matrix4x4 *theMatrix)
{


	// We can't return a matrix, so return an identity matrix
	Q3Matrix4x4_SetIdentity(theMatrix);
}





//=============================================================================
//      e3transform_camera_submit : Camera submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_camera_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3CameraTransformData	*instanceData = (const TQ3CameraTransformData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the view
	E3View_State_SetMatrix(theView, kQ3MatrixStateLocalToWorld  |
									kQ3MatrixStateWorldToCamera |
									kQ3MatrixStateCameraToFrustum,
									&instanceData->localToWorld,
									&instanceData->worldToCamera,
									&instanceData->cameraToFrustum);

	return(kQ3Success);
}





//=============================================================================
//      e3transform_camera_metahandler : Camera metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_camera_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeTransformMatrix:
			theMethod = (TQ3XFunctionPointer) e3transform_camera_matrix;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3transform_camera_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3transform_camera_rasterize_matrix : Return the matrix representation.
//-----------------------------------------------------------------------------
static void
e3transform_camera_rasterize_matrix(const void *transformData, TQ3Matrix4x4 *theMatrix)
{


	// We can't return a matrix, so return an identity matrix
	Q3Matrix4x4_SetIdentity(theMatrix);
}





//=============================================================================
//      e3transform_camera_rasterize_submit : Camera rasterize submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3transform_camera_rasterize_submit(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Matrix4x4				tmpMatrix, localToWorld, worldToCamera, cameraToFrustum;
	float						theWidth, theHeight, scaleWidth, scaleHeight;
	TQ3DrawContextObject		drawContext;
	TQ3Status					qd3dStatus;
	TQ3Area						drawContextArea;
#pragma unused(objectType)
#pragma unused(theObject)
#pragma unused(objectData)



	// Get the draw context size
	drawContext = E3View_AccessDrawContext(theView);
	qd3dStatus = Q3DrawContext_GetPane( drawContext, &drawContextArea );
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);

	theWidth  = drawContextArea.max.x - drawContextArea.min.x;
	theHeight = drawContextArea.max.y - drawContextArea.min.y;



	// Create our matrices
	//
	// A rasterize transform causes vertex x and y coordinates to be treated as
	// pixel coordinates within the draw context pane, and z to range from 0 to 1.
	// We treat this coordinate system as both local and world, so the local to
	// world matrix is the identity.
	Q3Matrix4x4_SetIdentity(&localToWorld);
	
	// The camera coordinate system is supposed to have visible stuff along the
	// negative z axis, so we need to negate z.  In order to make camera
	// coordinates still be a right-handed coordinate system, let us also flip
	// the y axis, so that the value at the bottom of the screen is -height
	// instead of height.
	Q3Matrix4x4_SetScale( &worldToCamera, 1.0f, -1.0f, -1.0f );
	
	// The QD3D frustum ranges from -1 to +1 in x and y, and 0 to -1 in z.
	// The z coordinate is fine as is, but x and y must be scaled and translated.
	scaleWidth  = 2.0f / theWidth;
	scaleHeight = 2.0f / theHeight;
	Q3Matrix4x4_SetScale( &cameraToFrustum, scaleWidth, scaleHeight, 1.0f );
	Q3Matrix4x4_SetTranslate( &tmpMatrix, -1.0f, 1.0f, 0.0f );
	Q3Matrix4x4_Multiply( &cameraToFrustum, &tmpMatrix, &cameraToFrustum );



	// Update the view
	E3View_State_SetMatrix(theView, kQ3MatrixStateLocalToWorld  |
												 kQ3MatrixStateWorldToCamera |
												 kQ3MatrixStateCameraToFrustum,
												 &localToWorld,
												 &worldToCamera,
												 &cameraToFrustum);

	return(kQ3Success);
}





//=============================================================================
//      e3transform_camera_rasterize_metahandler : Camera rasterize metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_camera_rasterize_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeTransformMatrix:
			theMethod = (TQ3XFunctionPointer) e3transform_camera_rasterize_matrix;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3transform_camera_rasterize_submit;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3transform_metahandler : base metahandler for transforms.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3transform_metahandler(TQ3XMethodType methodType)
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeNewObjectClass :
			return (TQ3XFunctionPointer) e3transform_new_class_info ;

		case kQ3XMethodTypeTransformMatrix :
			return (TQ3XFunctionPointer) e3transform_default_matrix ;

		case kQ3XMethodTypeObjectIsDrawable :
			return (TQ3XFunctionPointer) kQ3True ;
		}
	
	return NULL ;
	}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Transform_RegisterClass : Register the transform classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Transform_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the camera classes
	qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (	kQ3ClassNameTransform,
										e3transform_metahandler,
										E3Transform ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_WITH_DATA (	kQ3ClassNameTransformMatrix,
											e3transform_matrix_metahandler,
											E3MatrixTransform,
											sizeof(TQ3Matrix4x4) ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameTransformRotate,
											e3transform_rotate_metahandler,
											E3RotateTransform ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameTransformRotateAboutPoint,
											e3transform_rotateaboutpoint_metahandler,
											E3RotateAboutPointTransform ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameTransformRotateAboutAxis,
											e3transform_rotateaboutaxis_metahandler,
											E3RotateAboutAxisTransform ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameTransformScale,
											e3transform_scale_metahandler,
											E3ScaleTransform ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameTransformTranslate,
											e3transform_translate_metahandler,
											E3TranslateTransform ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameTransformQuaternion,
											e3transform_quaternion_metahandler,
											E3QuaternionTransform ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (	kQ3ClassNameTransformReset,
											e3transform_reset_metahandler,
											E3ResetTransform ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameTransformCamera,
											e3transform_camera_metahandler,
											E3CameraTransform ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (	kQ3ClassNameTransformCameraRasterize,
											e3transform_camera_rasterize_metahandler,
											E3CameraRasterizeTransform ) ;

	return(qd3dStatus);
}





//=============================================================================
//      E3Transform_UnregisterClass : Unregister the transform classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Transform_UnregisterClass(void)
{



	// Unregister the camera classes
	E3ClassTree::UnregisterClass(kQ3TransformTypeReset,            kQ3True);
	E3ClassTree::UnregisterClass(kQ3TransformTypeQuaternion,       kQ3True);
	E3ClassTree::UnregisterClass(kQ3TransformTypeTranslate,        kQ3True);
	E3ClassTree::UnregisterClass(kQ3TransformTypeScale,            kQ3True);
	E3ClassTree::UnregisterClass(kQ3TransformTypeRotateAboutAxis,  kQ3True);
	E3ClassTree::UnregisterClass(kQ3TransformTypeRotateAboutPoint, kQ3True);
	E3ClassTree::UnregisterClass(kQ3TransformTypeRotate,           kQ3True);
	E3ClassTree::UnregisterClass(kQ3TransformTypeMatrix,           kQ3True);
	E3ClassTree::UnregisterClass(kQ3TransformTypeCamera,           kQ3True);
	E3ClassTree::UnregisterClass(kQ3TransformTypeCameraRasterize,  kQ3True);
	E3ClassTree::UnregisterClass(kQ3ShapeTypeTransform,            kQ3True);

	return(kQ3Success);
}





//=============================================================================
//      E3Transform::IsOfMyClass : Check if object pointer is valid and of type Transform
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3ShapeTypeTransform )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3Transform::IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3Transform ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3Transform_GetType : Return the type of a transform.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Transform::GetType ( void )
	{
	// Get the type
	return GetObjectType ( kQ3ShapeTypeTransform ) ;
	}





//=============================================================================
//      E3Transform_GetMatrix : Return the matrix for the transform.
//-----------------------------------------------------------------------------
TQ3Matrix4x4*
E3Transform::GetMatrix ( TQ3Matrix4x4* theMatrix )
	{
	GetClass ()->matrixMethod ( FindLeafInstanceData (), theMatrix ) ;
	
	return theMatrix ;
	}





//=============================================================================
//      E3Transform_Submit : Submit a transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3Transform::Submit ( TQ3ViewObject theView )
	{

	// Submit the object to the view
	return E3View_SubmitRetained ( theView, this ) ;
	}





//=============================================================================
//      E3MatrixTransform_New : Create a new matrix transform.
//-----------------------------------------------------------------------------
TQ3TransformObject
E3MatrixTransform_New(const TQ3Matrix4x4 *theMatrix)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3TransformTypeMatrix, kQ3False, theMatrix);

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
//      E3MatrixTransform::IsOfMyClass : Check if object pointer is valid and of type MatrixTransform
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3TransformTypeMatrix )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3MatrixTransform::IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3MatrixTransform ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3MatrixTransform_Set : Set the matrix for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3MatrixTransform::Set ( const TQ3Matrix4x4* theMatrix )
	{
	// Set the data
	matrix = *theMatrix ;
	
	Edited () ;

	return kQ3Success ;
	}





//=============================================================================
//      E3MatrixTransform_Get : Get the matrix for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3MatrixTransform::Get ( TQ3Matrix4x4* theMatrix )
	{
	// Get the data
	*theMatrix = matrix ;

	return kQ3Success ;
	}





//=============================================================================
//      E3RotateTransform_New : Create a new rotate transform.
//-----------------------------------------------------------------------------
TQ3TransformObject
E3RotateTransform_New(const TQ3RotateTransformData *data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3TransformTypeRotate, kQ3False, data);

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
	{
	// Set the data
	( (E3RotateTransform*) theTransform )->instanceData = *data ;
	Q3Shared_Edited ( theTransform ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3RotateTransform_GetData : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_GetData(TQ3TransformObject theTransform, TQ3RotateTransformData *data)
	{
	// Get the data
	*data = ( (E3RotateTransform*) theTransform )->instanceData ;

	return kQ3Success ;
	}





//=============================================================================
//      E3RotateTransform_SetAxis : Set the axis for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_SetAxis(TQ3TransformObject theTransform, TQ3Axis axis)
	{
	// Set the field
	( (E3RotateTransform*) theTransform )->instanceData.axis = axis ;
	Q3Shared_Edited ( theTransform ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateTransform_GetAxis : Get the axis for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_GetAxis(TQ3TransformObject theTransform, TQ3Axis *axis)
	{
	// Get the field
	*axis = ( (E3RotateTransform*) theTransform )->instanceData.axis ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateTransform_SetAngle : Set the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_SetAngle(TQ3TransformObject theTransform, float radians)
	{
	// Set the field
	( (E3RotateTransform*) theTransform )->instanceData.radians = radians ;
	Q3Shared_Edited ( theTransform ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateTransform_GetAngle : Get the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateTransform_GetAngle(TQ3TransformObject theTransform, float *radians)
	{
	// Get the field
	*radians = ( (E3RotateTransform*) theTransform )->instanceData.radians ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutPointTransform_New : Create a rotate-point transform.
//-----------------------------------------------------------------------------
TQ3TransformObject
E3RotateAboutPointTransform_New(const TQ3RotateAboutPointTransformData *data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3TransformTypeRotateAboutPoint, kQ3False, data);

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
	{
	// Set the data
	( (E3RotateAboutPointTransform*) theTransform )->instanceData = *data ;
	Q3Shared_Edited ( theTransform ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutPointTransform_GetData : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_GetData(TQ3TransformObject theTransform, TQ3RotateAboutPointTransformData *data)
	{
	// Get the data
	*data = ( (E3RotateAboutPointTransform*) theTransform )->instanceData ;

	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutPointTransform_SetAxis : Set the axis for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_SetAxis(TQ3TransformObject theTransform, TQ3Axis axis)
	{
	// Set the field
	( (E3RotateAboutPointTransform*) theTransform )->instanceData.axis = axis ;
	Q3Shared_Edited ( theTransform ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutPointTransform_GetAxis : Get the axis for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_GetAxis(TQ3TransformObject theTransform, TQ3Axis *axis)
	{
	// Get the field
	*axis = ( (E3RotateAboutPointTransform*) theTransform )->instanceData.axis ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutPointTransform_SetAngle : Set the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_SetAngle(TQ3TransformObject theTransform, float radians)
	{
	// Set the field
	( (E3RotateAboutPointTransform*) theTransform )->instanceData.radians = radians ;
	Q3Shared_Edited ( theTransform ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutPointTransform_GetAngle : Get the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_GetAngle(TQ3TransformObject theTransform, float *radians)
	{
	// Get the field
	*radians = ( (E3RotateAboutPointTransform*) theTransform )->instanceData.radians ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutPointTransform_SetAboutPoint : Set the rotation point.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_SetAboutPoint(TQ3TransformObject theTransform, const TQ3Point3D *about)
	{
	// Set the field
	( (E3RotateAboutPointTransform*) theTransform )->instanceData.about = *about ;
	Q3Shared_Edited ( theTransform ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutPointTransform_GetAboutPoint : Get the rotation point.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutPointTransform_GetAboutPoint(TQ3TransformObject theTransform, TQ3Point3D *about)
	{
	// Get the field
	*about = ( (E3RotateAboutPointTransform*) theTransform )->instanceData.about ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutAxisTransform_New : Create a rotate-axis transform.
//-----------------------------------------------------------------------------
TQ3TransformObject
E3RotateAboutAxisTransform_New(const TQ3RotateAboutAxisTransformData *data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3TransformTypeRotateAboutAxis, kQ3False, data);

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
	{
	// Set the data
	( (E3RotateAboutAxisTransform*) theTransform )->instanceData = *data ;
	Q3Shared_Edited ( theTransform ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutAxisTransform_GetData : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_GetData(TQ3TransformObject theTransform, TQ3RotateAboutAxisTransformData *data)
	{
	// Get the data
	*data = ( (E3RotateAboutAxisTransform*) theTransform )->instanceData ;

	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutAxisTransform_SetOrientation : Set the orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_SetOrientation(TQ3TransformObject theTransform, const TQ3Vector3D *axis)
	{
	// Set the field
	( (E3RotateAboutAxisTransform*) theTransform )->instanceData.orientation = *axis ;
	Q3Shared_Edited ( theTransform ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutAxisTransform_GetOrientation : Get the orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_GetOrientation(TQ3TransformObject theTransform, TQ3Vector3D *axis)
	{
	// Set the field
	*axis = ( (E3RotateAboutAxisTransform*) theTransform )->instanceData.orientation ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutAxisTransform_SetAngle : Set the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_SetAngle(TQ3TransformObject theTransform, float radians)
	{
	// Set the field
	( (E3RotateAboutAxisTransform*) theTransform )->instanceData.radians = radians ;
	Q3Shared_Edited ( theTransform ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutAxisTransform_GetAngle : Get the angle for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_GetAngle(TQ3TransformObject theTransform, float *radians)
	{
	// Get the field
	*radians = ( (E3RotateAboutAxisTransform*) theTransform )->instanceData.radians;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutAxisTransform_SetOrigin : Set the origin.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_SetOrigin(TQ3TransformObject theTransform, const TQ3Point3D *origin)
	{
	// Set the field
	( (E3RotateAboutAxisTransform*) theTransform )->instanceData.origin = *origin;
	Q3Shared_Edited ( theTransform ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3RotateAboutAxisTransform_GetOrigin : Get the origin.
//-----------------------------------------------------------------------------
TQ3Status
E3RotateAboutAxisTransform_GetOrigin(TQ3TransformObject theTransform, TQ3Point3D *origin)
	{
	// Get the field
	*origin = ( (E3RotateAboutAxisTransform*) theTransform )->instanceData.origin ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3ScaleTransform_New : Create a scale transform.
//-----------------------------------------------------------------------------
TQ3TransformObject
E3ScaleTransform_New(const TQ3Vector3D *scale)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3TransformTypeScale, kQ3False, scale);

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
	{
	// Set the data
	( (E3ScaleTransform*) theTransform )->instanceData = *scale ;
	Q3Shared_Edited ( theTransform ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ScaleTransform_Get : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3ScaleTransform_Get(TQ3TransformObject theTransform, TQ3Vector3D *scale)
	{
	// Get the data
	*scale = ( (E3ScaleTransform*) theTransform )->instanceData ;

	return kQ3Success ;
	}





//=============================================================================
//      E3TranslateTransform_New : Create a translate transform.
//-----------------------------------------------------------------------------
TQ3TransformObject
E3TranslateTransform_New(const TQ3Vector3D *translate)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3TransformTypeTranslate, kQ3False, translate);

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
	{
	// Set the data
	( (E3TranslateTransform*) theTransform )->instanceData = *translate ;
	Q3Shared_Edited ( theTransform ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3TranslateTransform_Get : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3TranslateTransform_Get(TQ3TransformObject theTransform, TQ3Vector3D *translate)
	{
	// Get the data
	*translate = ( (E3TranslateTransform*) theTransform )->instanceData;

	return kQ3Success ;
	}





//=============================================================================
//      E3QuaternionTransform_New : Create a quaternion transform.
//-----------------------------------------------------------------------------
TQ3TransformObject
E3QuaternionTransform_New(const TQ3Quaternion *quaternion)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3TransformTypeQuaternion, kQ3False, quaternion);

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
	{
	// Set the data
	( (E3QuaternionTransform*) theTransform )->instanceData = *quaternion ;
	Q3Shared_Edited ( theTransform ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3QuaternionTransform_Get : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3QuaternionTransform_Get(TQ3TransformObject theTransform, TQ3Quaternion *quaternion)
	{
	// Get the data
	*quaternion = ( (E3QuaternionTransform*) theTransform )->instanceData ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ResetTransform_New : Create a reset transform.
//-----------------------------------------------------------------------------
TQ3TransformObject
E3ResetTransform_New(void)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3TransformTypeReset, kQ3False, NULL);

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





//=============================================================================
//      E3CameraTransform_New : Create a camera transform.
//-----------------------------------------------------------------------------
TQ3TransformObject
E3CameraTransform_New(const TQ3CameraTransformData *theData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3TransformTypeCamera, kQ3False, theData);

	return(theObject);
}





//=============================================================================
//      E3CameraTransform_Submit : Submit the transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3CameraTransform_Submit(const TQ3CameraTransformData *theData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3TransformTypeCamera, theData);

	return(qd3dStatus);
}





//=============================================================================
//      E3CameraTransform_Set : Set the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3CameraTransform_Set(TQ3TransformObject theTransform, const TQ3CameraTransformData *theData)
	{
	// Set the data
	( (E3CameraTransform*) theTransform )->instanceData = *theData ;
	Q3Shared_Edited ( theTransform ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3CameraTransform_Get : Get the data for the transform.
//-----------------------------------------------------------------------------
TQ3Status
E3CameraTransform_Get(TQ3TransformObject theTransform, TQ3CameraTransformData *theData)
	{
	// Get the data
	*theData = ( (E3CameraTransform*) theTransform )->instanceData ;

	return kQ3Success ;
	}





//=============================================================================
//      E3RasterizeCameraTransform_New : Create a RasterizeCamera transform.
//-----------------------------------------------------------------------------
TQ3TransformObject
E3RasterizeCameraTransform_New(void)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3TransformTypeCameraRasterize, kQ3False, NULL);

	return(theObject);
}





//=============================================================================
//      E3RasterizeCameraTransform_Submit : Submit the transform to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3RasterizeCameraTransform_Submit(TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3TransformTypeCameraRasterize, NULL);

	return(qd3dStatus);
}

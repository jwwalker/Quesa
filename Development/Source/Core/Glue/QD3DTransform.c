/*  NAME:
        QD3DTransform.c

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
#include "E3Transform.h"





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
//      Q3Transform_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
Q3Transform_GetType(TQ3TransformObject transform)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Transform_GetType(transform));
}





//=============================================================================
//      Q3Transform_GetMatrix : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Transform_GetMatrix(TQ3TransformObject transform, TQ3Matrix4x4 *matrix)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, NULL);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(NULL);

	if (0) // Further checks on matrix
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Transform_GetMatrix(transform, matrix));
}





//=============================================================================
//      Q3Transform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Transform_Submit(TQ3TransformObject transform, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Transform_Submit(transform, view));
}



#pragma mark -

//=============================================================================
//      Q3MatrixTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3MatrixTransform_New(const TQ3Matrix4x4 *matrix)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MatrixTransform_New(matrix));
}





//=============================================================================
//      Q3MatrixTransform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MatrixTransform_Submit(const TQ3Matrix4x4 *matrix, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MatrixTransform_Submit(matrix, view));
}





//=============================================================================
//      Q3MatrixTransform_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MatrixTransform_Set(TQ3TransformObject transform, const TQ3Matrix4x4 *matrix)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on matrix
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MatrixTransform_Set(transform, matrix));
}





//=============================================================================
//      Q3MatrixTransform_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MatrixTransform_Get(TQ3TransformObject transform, TQ3Matrix4x4 *matrix)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on matrix
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MatrixTransform_Get(transform, matrix));
}



#pragma mark -

//=============================================================================
//      Q3RotateTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3RotateTransform_New(const TQ3RotateTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateTransform_New(data));
}





//=============================================================================
//      Q3RotateTransform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateTransform_Submit(const TQ3RotateTransformData *data, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateTransform_Submit(data, view));
}





//=============================================================================
//      Q3RotateTransform_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateTransform_SetData(TQ3TransformObject transform, const TQ3RotateTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateTransform_SetData(transform, data));
}





//=============================================================================
//      Q3RotateTransform_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateTransform_GetData(TQ3TransformObject transform, TQ3RotateTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateTransform_GetData(transform, data));
}





//=============================================================================
//      Q3RotateTransform_SetAxis : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateTransform_SetAxis(TQ3TransformObject transform, TQ3Axis axis)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on axis
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateTransform_SetAxis(transform, axis));
}





//=============================================================================
//      Q3RotateTransform_SetAngle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateTransform_SetAngle(TQ3TransformObject transform, float radians)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on radians
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateTransform_SetAngle(transform, radians));
}





//=============================================================================
//      Q3RotateTransform_GetAxis : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateTransform_GetAxis(TQ3TransformObject renderable, TQ3Axis *axis)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(renderable->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderable, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(axis), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderable
		return(kQ3Failure);

	if (0) // Further checks on axis
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateTransform_GetAxis(renderable, axis));
}





//=============================================================================
//      Q3RotateTransform_GetAngle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateTransform_GetAngle(TQ3TransformObject transform, float *radians)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(radians), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on radians
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateTransform_GetAngle(transform, radians));
}



#pragma mark -

//=============================================================================
//      Q3RotateAboutPointTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3RotateAboutPointTransform_New(const TQ3RotateAboutPointTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutPointTransform_New(data));
}





//=============================================================================
//      Q3RotateAboutPointTransform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutPointTransform_Submit(const TQ3RotateAboutPointTransformData *data, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutPointTransform_Submit(data, view));
}





//=============================================================================
//      Q3RotateAboutPointTransform_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutPointTransform_SetData(TQ3TransformObject transform, const TQ3RotateAboutPointTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutPointTransform_SetData(transform, data));
}





//=============================================================================
//      Q3RotateAboutPointTransform_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutPointTransform_GetData(TQ3TransformObject transform, TQ3RotateAboutPointTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutPointTransform_GetData(transform, data));
}





//=============================================================================
//      Q3RotateAboutPointTransform_SetAxis : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutPointTransform_SetAxis(TQ3TransformObject transform, TQ3Axis axis)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on axis
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutPointTransform_SetAxis(transform, axis));
}





//=============================================================================
//      Q3RotateAboutPointTransform_GetAxis : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutPointTransform_GetAxis(TQ3TransformObject transform, TQ3Axis *axis)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(axis), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on axis
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutPointTransform_GetAxis(transform, axis));
}





//=============================================================================
//      Q3RotateAboutPointTransform_SetAngle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutPointTransform_SetAngle(TQ3TransformObject transform, float radians)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on radians
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutPointTransform_SetAngle(transform, radians));
}





//=============================================================================
//      Q3RotateAboutPointTransform_GetAngle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutPointTransform_GetAngle(TQ3TransformObject transform, float *radians)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(radians), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on radians
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutPointTransform_GetAngle(transform, radians));
}





//=============================================================================
//      Q3RotateAboutPointTransform_SetAboutPoint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutPointTransform_SetAboutPoint(TQ3TransformObject transform, const TQ3Point3D *about)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(about), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on about
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutPointTransform_SetAboutPoint(transform, about));
}





//=============================================================================
//      Q3RotateAboutPointTransform_GetAboutPoint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutPointTransform_GetAboutPoint(TQ3TransformObject transform, TQ3Point3D *about)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(about), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on about
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutPointTransform_GetAboutPoint(transform, about));
}



#pragma mark -

//=============================================================================
//      Q3RotateAboutAxisTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3RotateAboutAxisTransform_New(const TQ3RotateAboutAxisTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutAxisTransform_New(data));
}





//=============================================================================
//      Q3RotateAboutAxisTransform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutAxisTransform_Submit(const TQ3RotateAboutAxisTransformData *data, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutAxisTransform_Submit(data, view));
}





//=============================================================================
//      Q3RotateAboutAxisTransform_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutAxisTransform_SetData(TQ3TransformObject transform, const TQ3RotateAboutAxisTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutAxisTransform_SetData(transform, data));
}





//=============================================================================
//      Q3RotateAboutAxisTransform_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutAxisTransform_GetData(TQ3TransformObject transform, TQ3RotateAboutAxisTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutAxisTransform_GetData(transform, data));
}





//=============================================================================
//      Q3RotateAboutAxisTransform_SetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutAxisTransform_SetOrientation(TQ3TransformObject transform, const TQ3Vector3D *axis)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(axis), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on axis
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutAxisTransform_SetOrientation(transform, axis));
}





//=============================================================================
//      Q3RotateAboutAxisTransform_GetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutAxisTransform_GetOrientation(TQ3TransformObject transform, TQ3Vector3D *axis)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(axis), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on axis
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutAxisTransform_GetOrientation(transform, axis));
}





//=============================================================================
//      Q3RotateAboutAxisTransform_SetAngle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutAxisTransform_SetAngle(TQ3TransformObject transform, float radians)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on radians
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutAxisTransform_SetAngle(transform, radians));
}





//=============================================================================
//      Q3RotateAboutAxisTransform_GetAngle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutAxisTransform_GetAngle(TQ3TransformObject transform, float *radians)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(radians), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on radians
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutAxisTransform_GetAngle(transform, radians));
}





//=============================================================================
//      Q3RotateAboutAxisTransform_SetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutAxisTransform_SetOrigin(TQ3TransformObject transform, const TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutAxisTransform_SetOrigin(transform, origin));
}





//=============================================================================
//      Q3RotateAboutAxisTransform_GetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RotateAboutAxisTransform_GetOrigin(TQ3TransformObject transform, TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutAxisTransform_GetOrigin(transform, origin));
}



#pragma mark -

//=============================================================================
//      Q3ScaleTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3ScaleTransform_New(const TQ3Vector3D *scale)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(scale), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on scale
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ScaleTransform_New(scale));
}





//=============================================================================
//      Q3ScaleTransform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ScaleTransform_Submit(const TQ3Vector3D *scale, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(scale), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on scale
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ScaleTransform_Submit(scale, view));
}





//=============================================================================
//      Q3ScaleTransform_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ScaleTransform_Set(TQ3TransformObject transform, const TQ3Vector3D *scale)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(scale), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on scale
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ScaleTransform_Set(transform, scale));
}





//=============================================================================
//      Q3ScaleTransform_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ScaleTransform_Get(TQ3TransformObject transform, TQ3Vector3D *scale)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(scale), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on scale
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ScaleTransform_Get(transform, scale));
}


#pragma mark -


//=============================================================================
//      Q3TranslateTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3TranslateTransform_New(const TQ3Vector3D *translate)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(translate), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on translate
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TranslateTransform_New(translate));
}





//=============================================================================
//      Q3TranslateTransform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TranslateTransform_Submit(const TQ3Vector3D *translate, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(translate), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on translate
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TranslateTransform_Submit(translate, view));
}





//=============================================================================
//      Q3TranslateTransform_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TranslateTransform_Set(TQ3TransformObject transform, const TQ3Vector3D *translate)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(translate), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on translate
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TranslateTransform_Set(transform, translate));
}





//=============================================================================
//      Q3TranslateTransform_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TranslateTransform_Get(TQ3TransformObject transform, TQ3Vector3D *translate)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(translate), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on translate
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TranslateTransform_Get(transform, translate));
}



#pragma mark -

//=============================================================================
//      Q3QuaternionTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3QuaternionTransform_New(const TQ3Quaternion *quaternion)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3QuaternionTransform_New(quaternion));
}





//=============================================================================
//      Q3QuaternionTransform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3QuaternionTransform_Submit(const TQ3Quaternion *quaternion, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3QuaternionTransform_Submit(quaternion, view));
}





//=============================================================================
//      Q3QuaternionTransform_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3QuaternionTransform_Set(TQ3TransformObject transform, const TQ3Quaternion *quaternion)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on quaternion
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3QuaternionTransform_Set(transform, quaternion));
}





//=============================================================================
//      Q3QuaternionTransform_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3QuaternionTransform_Get(TQ3TransformObject transform, TQ3Quaternion *quaternion)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(transform->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, kQ3ShapeTypeTransform), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on transform
		return(kQ3Failure);

	if (0) // Further checks on quaternion
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3QuaternionTransform_Get(transform, quaternion));
}



#pragma mark -

//=============================================================================
//      Q3ResetTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3ResetTransform_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ResetTransform_New());
}





//=============================================================================
//      Q3ResetTransform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ResetTransform_Submit(TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ResetTransform_Submit(view));
}






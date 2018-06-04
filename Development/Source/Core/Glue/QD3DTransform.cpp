/*  NAME:
        QD3DTransform.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Copyright (c) 1999-2014, Quesa Developers. All rights reserved.

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
TQ3ObjectType
Q3Transform_GetType(TQ3TransformObject transform)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Transform::IsOfMyClass ( transform ), kQ3ObjectTypeInvalid);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Transform*) transform )->GetType () ;
}





//=============================================================================
//      Q3Transform_GetMatrix : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Transform_GetMatrix(TQ3TransformObject transform, TQ3Matrix4x4 *matrix)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Transform::IsOfMyClass ( transform ), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), NULL);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Transform*) transform )->GetMatrix ( matrix ) ;
}





//=============================================================================
//      Q3Transform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Transform_Submit(TQ3TransformObject transform, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Transform::IsOfMyClass  (transform ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Transform*) transform )->Submit ( view ) ;
}





//=============================================================================
//      Q3MatrixTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3MatrixTransform_New(const TQ3Matrix4x4 *matrix)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), NULL);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
 


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
	Q3_REQUIRE_OR_RESULT( E3MatrixTransform::IsOfMyClass ( transform ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3MatrixTransform*) transform )->Set ( matrix ) ;
}





//=============================================================================
//      Q3MatrixTransform_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MatrixTransform_Get(TQ3TransformObject transform, TQ3Matrix4x4 *matrix)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3MatrixTransform::IsOfMyClass ( transform ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3MatrixTransform*) transform )->Get ( matrix ) ;
}





//=============================================================================
//      Q3RotateTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3RotateTransform_New(const TQ3RotateTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotate)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotate)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotate)), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotate)), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderable, (kQ3TransformTypeRotate)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(axis), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotate)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(radians), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateTransform_GetAngle(transform, radians));
}





//=============================================================================
//      Q3RotateAboutPointTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3RotateAboutPointTransform_New(const TQ3RotateAboutPointTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutPoint)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutPoint)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutPoint)), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutPoint)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(axis), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutPoint)), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutPoint)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(radians), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutPoint)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(about), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutPoint)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(about), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutPointTransform_GetAboutPoint(transform, about));
}





//=============================================================================
//      Q3RotateAboutAxisTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3RotateAboutAxisTransform_New(const TQ3RotateAboutAxisTransformData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutAxis)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutAxis)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutAxis)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(axis), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutAxis)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(axis), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutAxis)), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutAxis)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(radians), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutAxis)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeRotateAboutAxis)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RotateAboutAxisTransform_GetOrigin(transform, origin));
}





//=============================================================================
//      Q3ScaleTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3ScaleTransform_New(const TQ3Vector3D *scale)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(scale), NULL);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeScale)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(scale), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeScale)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(scale), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ScaleTransform_Get(transform, scale));
}





//=============================================================================
//      Q3TranslateTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3TranslateTransform_New(const TQ3Vector3D *translate)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(translate), NULL);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeTranslate)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(translate), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeTranslate)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(translate), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TranslateTransform_Get(transform, translate));
}





//=============================================================================
//      Q3QuaternionTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3QuaternionTransform_New(const TQ3Quaternion *quaternion)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeQuaternion)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), kQ3Failure);



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(transform, (kQ3TransformTypeQuaternion)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3QuaternionTransform_Get(transform, quaternion));
}





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
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ResetTransform_Submit(view));
}





//=============================================================================
//      Q3CameraTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3CameraTransform_New(const TQ3CameraTransformData *theData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theData), NULL);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CameraTransform_New(theData));
}





//=============================================================================
//      Q3CameraTransform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CameraTransform_Submit(const TQ3CameraTransformData *theData, TQ3ViewObject theView)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( theView ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CameraTransform_Submit(theData, theView));
}





//=============================================================================
//      Q3CameraTransform_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CameraTransform_Set(TQ3TransformObject theTransform, const TQ3CameraTransformData *theData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theTransform, (kQ3TransformTypeCamera)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theData), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CameraTransform_Set(theTransform, theData));
}





//=============================================================================
//      Q3CameraTransform_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CameraTransform_Get(TQ3TransformObject theTransform, TQ3CameraTransformData *theData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theTransform, (kQ3TransformTypeCamera)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theData), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CameraTransform_Get(theTransform, theData));
}





//=============================================================================
//      Q3RasterizeCameraTransform_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TransformObject
Q3RasterizeCameraTransform_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RasterizeCameraTransform_New());
}





//=============================================================================
//      Q3RasterizeCameraTransform_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RasterizeCameraTransform_Submit(TQ3ViewObject theView)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( theView ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RasterizeCameraTransform_Submit(theView));
}




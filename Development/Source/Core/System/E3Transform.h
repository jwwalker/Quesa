/*  NAME:
        E3Transform.h

    DESCRIPTION:
        Header file for E3Transform.c.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
#ifndef E3TRANSFORM_HDR
#define E3TRANSFORM_HDR
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
//      Types
//-----------------------------------------------------------------------------



class E3TransformInfo : public E3ShapeInfo
	{
	const TQ3XTransformMatrixMethod matrixMethod ;
	
public :

									E3TransformInfo	(
													TQ3XMetaHandler	newClassMetaHandler,
													E3ClassInfo*	newParent
					 								) ; // constructor	
	friend class E3Transform ;
	} ;




class E3Transform : public E3ShapeData
	{
Q3_CLASS_ENUMS ( kQ3ShapeTypeTransform, E3Transform, E3ShapeData )
public :

	// There is no extra data for this class


	TQ3Matrix4x4*			GetMatrix ( TQ3Matrix4x4* theMatrix ) ;
	} ;
	


//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3Transform_RegisterClass(void);
TQ3Status			E3Transform_UnregisterClass(void);

TQ3ObjectType		E3Transform_GetType(TQ3TransformObject theTransform);
//TQ3Matrix4x4 *		E3Transform_GetMatrix(TQ3TransformObject theTransform, TQ3Matrix4x4 *theMatrix);
TQ3Status			E3Transform_Submit(TQ3TransformObject theTransform, TQ3ViewObject theView);

TQ3TransformObject	E3MatrixTransform_New(const TQ3Matrix4x4 *theMatrix);
TQ3Status			E3MatrixTransform_Submit(const TQ3Matrix4x4 *theMatrix, TQ3ViewObject theView);
TQ3Status			E3MatrixTransform_Set(TQ3TransformObject theTransform, const TQ3Matrix4x4 *theMatrix);
TQ3Status			E3MatrixTransform_Get(TQ3TransformObject theTransform, TQ3Matrix4x4 *theMatrix);

TQ3TransformObject	E3RotateTransform_New(const TQ3RotateTransformData *data);
TQ3Status			E3RotateTransform_Submit(const TQ3RotateTransformData *data, TQ3ViewObject theView);
TQ3Status			E3RotateTransform_SetData(TQ3TransformObject theTransform, const TQ3RotateTransformData *data);
TQ3Status			E3RotateTransform_GetData(TQ3TransformObject theTransform, TQ3RotateTransformData *data);
TQ3Status			E3RotateTransform_SetAxis(TQ3TransformObject theTransform, TQ3Axis axis);
TQ3Status			E3RotateTransform_GetAxis(TQ3TransformObject theTransform, TQ3Axis *axis);
TQ3Status			E3RotateTransform_SetAngle(TQ3TransformObject theTransform, float radians);
TQ3Status			E3RotateTransform_GetAngle(TQ3TransformObject theTransform, float *radians);

TQ3TransformObject	E3RotateAboutPointTransform_New(const TQ3RotateAboutPointTransformData *data);
TQ3Status			E3RotateAboutPointTransform_Submit(const TQ3RotateAboutPointTransformData *data, TQ3ViewObject theView);
TQ3Status			E3RotateAboutPointTransform_SetData(TQ3TransformObject theTransform, const TQ3RotateAboutPointTransformData *data);
TQ3Status			E3RotateAboutPointTransform_GetData(TQ3TransformObject theTransform, TQ3RotateAboutPointTransformData *data);
TQ3Status			E3RotateAboutPointTransform_SetAxis(TQ3TransformObject theTransform, TQ3Axis axis);
TQ3Status			E3RotateAboutPointTransform_GetAxis(TQ3TransformObject theTransform, TQ3Axis *axis);
TQ3Status			E3RotateAboutPointTransform_SetAngle(TQ3TransformObject theTransform, float radians);
TQ3Status			E3RotateAboutPointTransform_GetAngle(TQ3TransformObject theTransform, float *radians);
TQ3Status			E3RotateAboutPointTransform_SetAboutPoint(TQ3TransformObject theTransform, const TQ3Point3D *about);
TQ3Status			E3RotateAboutPointTransform_GetAboutPoint(TQ3TransformObject theTransform, TQ3Point3D *about);

TQ3TransformObject	E3RotateAboutAxisTransform_New(const TQ3RotateAboutAxisTransformData *data);
TQ3Status			E3RotateAboutAxisTransform_Submit(const TQ3RotateAboutAxisTransformData *data, TQ3ViewObject theView);
TQ3Status			E3RotateAboutAxisTransform_SetData(TQ3TransformObject theTransform, const TQ3RotateAboutAxisTransformData *data);
TQ3Status			E3RotateAboutAxisTransform_GetData(TQ3TransformObject theTransform, TQ3RotateAboutAxisTransformData *data);
TQ3Status			E3RotateAboutAxisTransform_SetOrientation(TQ3TransformObject theTransform, const TQ3Vector3D *axis);
TQ3Status			E3RotateAboutAxisTransform_GetOrientation(TQ3TransformObject theTransform, TQ3Vector3D *axis);
TQ3Status			E3RotateAboutAxisTransform_SetAngle(TQ3TransformObject theTransform, float radians);
TQ3Status			E3RotateAboutAxisTransform_GetAngle(TQ3TransformObject theTransform, float *radians);
TQ3Status			E3RotateAboutAxisTransform_SetOrigin(TQ3TransformObject theTransform, const TQ3Point3D *origin);
TQ3Status			E3RotateAboutAxisTransform_GetOrigin(TQ3TransformObject theTransform, TQ3Point3D *origin);

TQ3TransformObject	E3ScaleTransform_New(const TQ3Vector3D *scale);
TQ3Status			E3ScaleTransform_Submit(const TQ3Vector3D *scale, TQ3ViewObject theView);
TQ3Status			E3ScaleTransform_Set(TQ3TransformObject theTransform, const TQ3Vector3D *scale);
TQ3Status			E3ScaleTransform_Get(TQ3TransformObject theTransform, TQ3Vector3D *scale);

TQ3TransformObject	E3TranslateTransform_New(const TQ3Vector3D *translate);
TQ3Status			E3TranslateTransform_Submit(const TQ3Vector3D *translate, TQ3ViewObject theView);
TQ3Status			E3TranslateTransform_Set(TQ3TransformObject theTransform, const TQ3Vector3D *translate);
TQ3Status			E3TranslateTransform_Get(TQ3TransformObject theTransform, TQ3Vector3D *translate);

TQ3TransformObject	E3QuaternionTransform_New(const TQ3Quaternion *quaternion);
TQ3Status			E3QuaternionTransform_Submit(const TQ3Quaternion *quaternion, TQ3ViewObject theView);
TQ3Status			E3QuaternionTransform_Set(TQ3TransformObject theTransform, const TQ3Quaternion *quaternion);
TQ3Status			E3QuaternionTransform_Get(TQ3TransformObject theTransform, TQ3Quaternion *quaternion);

TQ3TransformObject	E3ResetTransform_New(void);
TQ3Status			E3ResetTransform_Submit(TQ3ViewObject theView);

TQ3TransformObject	E3CameraTransform_New(   const TQ3CameraTransformData *theData);
TQ3Status			E3CameraTransform_Submit(const TQ3CameraTransformData *theData, TQ3ViewObject theView);
TQ3Status			E3CameraTransform_Set(TQ3TransformObject theTransform, const TQ3CameraTransformData *theData);
TQ3Status			E3CameraTransform_Get(TQ3TransformObject theTransform,       TQ3CameraTransformData *theData);

TQ3TransformObject	E3RasterizeCameraTransform_New(void);
TQ3Status			E3RasterizeCameraTransform_Submit(TQ3ViewObject theView);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


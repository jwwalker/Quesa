/*  NAME:
        E3Transform.h

    DESCRIPTION:
        Header file for E3Transform.c.

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
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3Transform_RegisterClass(void);
TQ3Status			E3Transform_UnregisterClass(void);

TQ3ObjectType		E3Transform_GetType(TQ3TransformObject theTransform);
TQ3Matrix4x4 *		E3Transform_GetMatrix(TQ3TransformObject theTransform, TQ3Matrix4x4 *theMatrix);
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





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        E3Math.h

    DESCRIPTION:
        Header file for E3Math.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#ifndef E3MATH_HDR
#define E3MATH_HDR
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
//      Point and Vector creation
//-----------------------------------------------------------------------------
TQ3Vector2D *			E3Vector2D_Set(TQ3Vector2D *vector2D, float x, float y);
TQ3Vector3D *			E3Vector3D_Set(TQ3Vector3D *vector3D, float x, float y, float z);
TQ3Point2D *			E3Point2D_Set(TQ3Point2D *point2D, float x, float y);
TQ3Param2D *			E3Param2D_Set(TQ3Param2D *param2D, float u, float v);
TQ3RationalPoint3D *	E3RationalPoint3D_Set(TQ3RationalPoint3D *rationalPoint3D, float x, float y, float w);
TQ3Point3D *			E3Point3D_Set(TQ3Point3D *point3D, float x, float y, float z);
TQ3RationalPoint4D *	E3RationalPoint4D_Set(TQ3RationalPoint4D *rationalPoint4D, float x, float y, float z, float w);
TQ3PolarPoint *			E3PolarPoint_Set(TQ3PolarPoint *polarPoint, float r, float theta);
TQ3SphericalPoint *		E3SphericalPoint_Set(TQ3SphericalPoint *sphericalPoint, float rho, float theta, float phi);





//=============================================================================
//      Point and Vector dimension conversion
//-----------------------------------------------------------------------------
TQ3Vector3D *			E3Vector2D_To3D(const TQ3Vector2D *vector2D, TQ3Vector3D *result);
TQ3RationalPoint3D *	E3Vector2D_ToRationalPoint3D(const TQ3Vector2D *vector2D, TQ3RationalPoint3D *result);
TQ3Vector2D *			E3Vector3D_To2D(const TQ3Vector3D *vector3D, TQ3Vector2D *result);
TQ3Vector2D *			E3RationalPoint3D_ToVector2D(const TQ3RationalPoint3D *rationalPoint3D, TQ3Vector2D *result);
TQ3RationalPoint4D *	E3Vector3D_ToRationalPoint4D(const TQ3Vector3D *vector3D, TQ3RationalPoint4D *result);
TQ3Vector3D *			E3RationalPoint4D_ToVector3D(const TQ3RationalPoint4D *rationalPoint4D, TQ3Vector3D *result);
TQ3RationalPoint3D *	E3Point2D_To3D(const TQ3Point2D *point2D, TQ3RationalPoint3D *result);
TQ3Point2D *			E3RationalPoint3D_To2D(const TQ3RationalPoint3D *rationalPoint3D, TQ3Point2D *result);
TQ3RationalPoint4D *	E3Point3D_To4D(const TQ3Point3D *point3D, TQ3RationalPoint4D *result);
TQ3Point3D *			E3RationalPoint4D_To3D(const TQ3RationalPoint4D *rationalPoint4D, TQ3Point3D *result);





//=============================================================================
//      Point conversion from cartesian to polar/spherical
//-----------------------------------------------------------------------------
TQ3PolarPoint *			E3Point2D_ToPolar(const TQ3Point2D *point2D, TQ3PolarPoint *result);
TQ3Point2D *			E3PolarPoint_ToPoint2D(const TQ3PolarPoint *polarPoint, TQ3Point2D *result);
TQ3SphericalPoint *		E3Point3D_ToSpherical(const TQ3Point3D *point3D, TQ3SphericalPoint *result);
TQ3Point3D *			E3SphericalPoint_ToPoint3D(const TQ3SphericalPoint *sphericalPoint, TQ3Point3D *result);





//=============================================================================
//      Dot product
//-----------------------------------------------------------------------------
float					E3Vector2D_Dot(const TQ3Vector2D *v1, const TQ3Vector2D *v2);
float					E3Vector3D_Dot(const TQ3Vector3D *v1, const TQ3Vector3D *v2);
TQ3Status				E3Vector3D_DotArray(TQ3Uns32				numVectors,
											const TQ3Vector3D		*firstVectors,
											const TQ3Vector3D		*secondVectors,
											float					*dotProducts,
											TQ3Boolean				*dotLessThanZero);





//=============================================================================
//      Cross product
//-----------------------------------------------------------------------------
float					E3Vector2D_Cross(const TQ3Vector2D *v1, const TQ3Vector2D *v2);
float					E3Point2D_CrossProductTri(const TQ3Point2D *p1, const TQ3Point2D *p2, const TQ3Point2D *p3);
TQ3Vector3D *			E3Vector3D_Cross(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result);
TQ3Vector3D *			E3Point3D_CrossProductTri(const TQ3Point3D *p1, const TQ3Point3D *p2, const TQ3Point3D *p3, TQ3Vector3D *result);
TQ3Status				E3Triangle_CrossProductArray(TQ3Uns32				numTriangles,
														const TQ3Uns8		*usageFlags,
														const TQ3Uns32		*theIndices,
														const TQ3Point3D	*thePoints,
														TQ3Vector3D			*theNormals);





//=============================================================================
//      Vector length
//-----------------------------------------------------------------------------
float					E3Vector2D_Length(const TQ3Vector2D *vector2D);
float					E3Vector2D_LengthSquared(const TQ3Vector2D *vector2D);
float					E3Vector3D_Length(const TQ3Vector3D *vector3D);
float					E3Vector3D_LengthSquared(const TQ3Vector3D *vector3D);





//=============================================================================
//      Point distance
//-----------------------------------------------------------------------------
float					E3Point2D_Distance(const TQ3Point2D *p1, const TQ3Point2D *p2);
float					E3Point2D_DistanceSquared(const TQ3Point2D *p1, const TQ3Point2D *p2);
float					E3Param2D_Distance(const TQ3Param2D *p1, const TQ3Param2D *p2);
float					E3Param2D_DistanceSquared(const TQ3Param2D *p1, const TQ3Param2D *p2);
float					E3RationalPoint3D_Distance(const TQ3RationalPoint3D *p1, const TQ3RationalPoint3D *p2);
float					E3RationalPoint3D_DistanceSquared(const TQ3RationalPoint3D *p1, const TQ3RationalPoint3D *p2);
float					E3Point3D_Distance(const TQ3Point3D *p1, const TQ3Point3D *p2);
float					E3Point3D_DistanceSquared(const TQ3Point3D *p1, const TQ3Point3D *p2);
float					E3RationalPoint4D_Distance(const TQ3RationalPoint4D *p1, const TQ3RationalPoint4D *p2);
float					E3RationalPoint4D_DistanceSquared(const TQ3RationalPoint4D *p1, const TQ3RationalPoint4D *p2);





//=============================================================================
//      Vector negation
//-----------------------------------------------------------------------------
TQ3Vector2D *			E3Vector2D_Negate(const TQ3Vector2D *vector2D, TQ3Vector2D *result);
TQ3Vector3D *			E3Vector3D_Negate(const TQ3Vector3D *vector3D, TQ3Vector3D *result);





//=============================================================================
//      Vector scale
//-----------------------------------------------------------------------------
TQ3Vector2D *			E3Vector2D_Scale(const TQ3Vector2D *vector2D, float scalar, TQ3Vector2D *result);
TQ3Vector3D *			E3Vector3D_Scale(const TQ3Vector3D *vector3D, float scalar, TQ3Vector3D *result);





//=============================================================================
//      Vector normalize
//-----------------------------------------------------------------------------
TQ3Vector2D *			E3Vector2D_Normalize(const TQ3Vector2D *vector2D, TQ3Vector2D *result);
TQ3Vector3D *			E3Vector3D_Normalize(const TQ3Vector3D *vector3D, TQ3Vector3D *result);





//=============================================================================
//      Vector-vector addition/subtraction
//-----------------------------------------------------------------------------
TQ3Vector2D *			E3Vector2D_Add(const TQ3Vector2D *v1, const TQ3Vector2D *v2, TQ3Vector2D *result);
TQ3Vector3D *			E3Vector3D_Add(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result);
TQ3Vector2D *			E3Vector2D_Subtract(const TQ3Vector2D *v1, const TQ3Vector2D *v2, TQ3Vector2D *result);
TQ3Vector3D *			E3Vector3D_Subtract(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result);





//=============================================================================
//      Point and Vector addition/subtraction
//-----------------------------------------------------------------------------
TQ3Point2D *			E3Point2D_Vector2D_Add(const TQ3Point2D *point2D, const TQ3Vector2D *vector2D, TQ3Point2D *result);
TQ3Param2D *			E3Param2D_Vector2D_Add(const TQ3Param2D *param2D, const TQ3Vector2D *vector2D, TQ3Param2D *result);
TQ3Point3D *			E3Point3D_Vector3D_Add(const TQ3Point3D *point3D, const TQ3Vector3D *vector3D, TQ3Point3D *result);
TQ3Point2D *			E3Point2D_Vector2D_Subtract(const TQ3Point2D *point2D, const TQ3Vector2D *vector2D, TQ3Point2D *result);
TQ3Param2D *			E3Param2D_Vector2D_Subtract(const TQ3Param2D *param2D, const TQ3Vector2D *vector2D, TQ3Param2D *result);
TQ3Point3D *			E3Point3D_Vector3D_Subtract(const TQ3Point3D *point3D, const TQ3Vector3D *vector3D, TQ3Point3D *result);





//=============================================================================
//      Point subtraction
//-----------------------------------------------------------------------------
TQ3Vector2D *			E3Point2D_Subtract(const TQ3Point2D *p1, const TQ3Point2D *p2, TQ3Vector2D *result);
TQ3Vector2D *			E3Param2D_Subtract(const TQ3Param2D *p1, const TQ3Param2D *p2, TQ3Vector2D *result);
TQ3Vector3D *			E3Point3D_Subtract(const TQ3Point3D *p1, const TQ3Point3D *p2, TQ3Vector3D *result);





//=============================================================================
//      Point relative ratio
//-----------------------------------------------------------------------------
TQ3Point2D *			E3Point2D_RRatio(const TQ3Point2D *p1, const TQ3Point2D *p2, float r1, float r2, TQ3Point2D *result);
TQ3Param2D *			E3Param2D_RRatio(const TQ3Param2D *p1, const TQ3Param2D *p2, float r1, float r2, TQ3Param2D *result);
TQ3Point3D *			E3Point3D_RRatio(const TQ3Point3D *p1, const TQ3Point3D *p2, float r1, float r2, TQ3Point3D *result);
TQ3RationalPoint4D *	E3RationalPoint4D_RRatio(const TQ3RationalPoint4D *p1, const TQ3RationalPoint4D *p2, float r1, float r2, TQ3RationalPoint4D *result);





//=============================================================================
//      Point affine combinations
//-----------------------------------------------------------------------------
TQ3Point2D *			E3Point2D_AffineComb(const TQ3Point2D *points2D, const float *weights, TQ3Uns32 numPoints, TQ3Point2D *result);
TQ3Param2D *			E3Param2D_AffineComb(const TQ3Param2D *params2D, const float *weights, TQ3Uns32 numPoints, TQ3Param2D *result);
TQ3RationalPoint3D *	E3RationalPoint3D_AffineComb(const TQ3RationalPoint3D *rationalPoints3D, const float *weights, TQ3Uns32 numPoints, TQ3RationalPoint3D *result);
TQ3Point3D *			E3Point3D_AffineComb(const TQ3Point3D *points3D, const float *weights, TQ3Uns32 numPoints, TQ3Point3D *result);
TQ3RationalPoint4D *	E3RationalPoint4D_AffineComb(const TQ3RationalPoint4D *rationalPoints4D, const float *weights, TQ3Uns32 numPoints, TQ3RationalPoint4D *result);





//=============================================================================
//      Point and vector transformation
//-----------------------------------------------------------------------------
TQ3Vector2D *			E3Vector2D_Transform(const TQ3Vector2D *vector2D, const TQ3Matrix3x3 *matrix3x3, TQ3Vector2D *result);
TQ3Vector3D *			E3Vector3D_Transform(const TQ3Vector3D *vector3D, const TQ3Matrix4x4 *matrix4x4, TQ3Vector3D *result);
TQ3Point2D *			E3Point2D_Transform(const TQ3Point2D *point2D, const TQ3Matrix3x3 *matrix3x3, TQ3Point2D *result);
TQ3Param2D *			E3Param2D_Transform(const TQ3Param2D *param2D, const TQ3Matrix3x3 *matrix3x3, TQ3Param2D *result);
TQ3RationalPoint3D *	E3RationalPoint3D_Transform(const TQ3RationalPoint3D *rationalPoint3D, const TQ3Matrix3x3 *matrix3x3, TQ3RationalPoint3D *result);
TQ3Point3D *			E3Point3D_Transform(const TQ3Point3D *point3D, const TQ3Matrix4x4 *matrix4x4, TQ3Point3D *result);
TQ3RationalPoint4D *	E3RationalPoint4D_Transform(const TQ3RationalPoint4D *rationalPoint4D, const TQ3Matrix4x4 *matrix4x4, TQ3RationalPoint4D *result);
TQ3Status				E3Vector2D_To2DTransformArray(const TQ3Vector2D *inVectors2D, const TQ3Matrix3x3 *matrix3x3, TQ3Vector2D *outVectors2D, TQ3Uns32 numVectors, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize);
TQ3Status				E3Vector3D_To3DTransformArray(const TQ3Vector3D *inVectors3D, const TQ3Matrix4x4 *matrix4x4, TQ3Vector3D *outVectors3D, TQ3Uns32 numVectors, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize);
TQ3Status				E3Point2D_To2DTransformArray(const TQ3Point2D *inPoints2D, const TQ3Matrix3x3 *matrix3x3, TQ3Point2D *outPoints2D, TQ3Uns32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize);
TQ3Status				E3RationalPoint3D_To3DTransformArray(const TQ3RationalPoint3D *inRationalPoints3D, const TQ3Matrix3x3 *matrix3x3, TQ3RationalPoint3D *outRationalPoints3D, TQ3Uns32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize);
TQ3Status				E3Point3D_To3DTransformArray(const TQ3Point3D *inPoints3D, const TQ3Matrix4x4 *matrix4x4, TQ3Point3D *outPoints3D, TQ3Uns32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize);
TQ3Status				E3Point3D_To4DTransformArray(const TQ3Point3D *inPoints3D, const TQ3Matrix4x4 *matrix4x4, TQ3RationalPoint4D *outRationalPoints4D, TQ3Uns32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize);
TQ3Status				E3RationalPoint4D_To4DTransformArray(const TQ3RationalPoint4D *inRationalPoints4D, const TQ3Matrix4x4 *matrix4x4, TQ3RationalPoint4D *outRationalPoints4D, TQ3Uns32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize);





//=============================================================================
//      Matrix functions
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *			E3Matrix3x3_SetIdentity(TQ3Matrix3x3 *matrix3x3);
TQ3Matrix4x4 *			E3Matrix4x4_SetIdentity(TQ3Matrix4x4 *matrix4x4);
TQ3Matrix3x3 *			E3Matrix3x3_SetTranslate(TQ3Matrix3x3 *matrix3x3, float xTrans, float yTrans);
TQ3Matrix4x4 *			E3Matrix4x4_SetTranslate(TQ3Matrix4x4 *matrix4x4, float xTrans, float yTrans, float zTrans);
TQ3Matrix3x3 *			E3Matrix3x3_SetScale(TQ3Matrix3x3 *matrix3x3, float xScale, float yScale);
TQ3Matrix4x4 *			E3Matrix4x4_SetScale(TQ3Matrix4x4 *matrix4x4, float xScale, float yScale, float zScale);
TQ3Matrix3x3 *			E3Matrix3x3_SetRotate(TQ3Matrix3x3 *matrix3x3, float angle);
TQ3Matrix4x4 *			E3Matrix4x4_SetRotate_X(TQ3Matrix4x4 *matrix4x4, float angle);
TQ3Matrix4x4 *			E3Matrix4x4_SetRotate_Y(TQ3Matrix4x4 *matrix4x4, float angle);
TQ3Matrix4x4 *			E3Matrix4x4_SetRotate_Z(TQ3Matrix4x4 *matrix4x4, float angle);
TQ3Matrix4x4 *			E3Matrix4x4_SetRotate_XYZ(TQ3Matrix4x4 *matrix4x4, float xAngle, float yAngle, float zAngle);
TQ3Matrix3x3 *			E3Matrix3x3_SetRotateAboutPoint(TQ3Matrix3x3 *matrix3x3, const TQ3Point2D *origin, float angle);
TQ3Matrix4x4 *			E3Matrix4x4_SetRotateAboutPoint(TQ3Matrix4x4 *matrix4x4, const TQ3Point3D *origin, float xAngle, float yAngle, float zAngle);
TQ3Matrix4x4 *			E3Matrix4x4_SetRotateAboutAxis(TQ3Matrix4x4 *matrix4x4, const TQ3Point3D *origin, const TQ3Vector3D *axis, float angle);
TQ3Matrix4x4 *			E3Matrix4x4_SetRotateVectorToVector(TQ3Matrix4x4 *matrix4x4, const TQ3Vector3D *v1, const TQ3Vector3D *v2);
TQ3Matrix4x4 *			E3Matrix4x4_SetQuaternion(TQ3Matrix4x4 *matrix4x4, const TQ3Quaternion *quaternion);
TQ3Matrix3x3 *			E3Matrix3x3_Copy(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result);
TQ3Matrix4x4 *			E3Matrix4x4_Copy(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result);
TQ3Matrix3x3 *			E3Matrix3x3_Transpose(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result);
TQ3Matrix4x4 *			E3Matrix4x4_Transpose(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result);
float					E3Matrix3x3_Determinant(const TQ3Matrix3x3 *matrix3x3);
float					E3Matrix4x4_Determinant(const TQ3Matrix4x4 *matrix4x4);
TQ3Matrix3x3 *			E3Matrix3x3_Adjoint(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result);
TQ3Matrix3x3 *			E3Matrix3x3_Invert(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result);
TQ3Matrix4x4 *			E3Matrix4x4_Invert(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result);
TQ3Matrix3x3 *			E3Matrix3x3_Multiply(const TQ3Matrix3x3 *m1, const TQ3Matrix3x3 *m2, TQ3Matrix3x3 *result);
TQ3Matrix4x4 *			E3Matrix4x4_Multiply(const TQ3Matrix4x4 *m1, const TQ3Matrix4x4 *m2, TQ3Matrix4x4 *result);





//=============================================================================
//      Quaternion functions
//-----------------------------------------------------------------------------
TQ3Quaternion *			E3Quaternion_Set(TQ3Quaternion *quaternion, float w, float x, float y, float z);
TQ3Quaternion *			E3Quaternion_SetIdentity(TQ3Quaternion *quaternion);
TQ3Quaternion *			E3Quaternion_SetRotate_X(TQ3Quaternion *quaternion, float angle);
TQ3Quaternion *			E3Quaternion_SetRotate_Y(TQ3Quaternion *quaternion, float angle);
TQ3Quaternion *			E3Quaternion_SetRotate_Z(TQ3Quaternion *quaternion, float angle);
TQ3Quaternion *			E3Quaternion_SetRotate_XYZ(TQ3Quaternion *quaternion, float xAngle, float yAngle, float zAngle);
TQ3Quaternion *			E3Quaternion_SetRotateAboutAxis(TQ3Quaternion *quaternion, const TQ3Vector3D *axis, float angle);
TQ3Quaternion *			E3Quaternion_SetRotateVectorToVector(TQ3Quaternion *quaternion, const TQ3Vector3D *v1, const TQ3Vector3D *v2);
TQ3Quaternion *			E3Quaternion_SetMatrix(TQ3Quaternion *quaternion, const TQ3Matrix4x4 *matrix4x4);
TQ3Quaternion *			E3Quaternion_Copy(const TQ3Quaternion *quaternion, TQ3Quaternion *result);
TQ3Boolean				E3Quaternion_IsIdentity(const TQ3Quaternion *quaternion);
float					E3Quaternion_Dot(const TQ3Quaternion *q1, const TQ3Quaternion *q2);
TQ3Quaternion *			E3Quaternion_Normalize(const TQ3Quaternion *quaternion, TQ3Quaternion *result);
TQ3Quaternion *			E3Quaternion_Invert(const TQ3Quaternion *quaternion, TQ3Quaternion *result);
TQ3Quaternion *			E3Quaternion_Multiply(const TQ3Quaternion *q1, const TQ3Quaternion *q2, TQ3Quaternion *result);
TQ3Quaternion *			E3Quaternion_MatchReflection(const TQ3Quaternion *q1, const TQ3Quaternion *q2, TQ3Quaternion *result);
TQ3Quaternion *			E3Quaternion_InterpolateFast(const TQ3Quaternion *q1, const TQ3Quaternion *q2, float t, TQ3Quaternion *result);
TQ3Quaternion *			E3Quaternion_InterpolateLinear(const TQ3Quaternion *q1, const TQ3Quaternion *q2, float t, TQ3Quaternion *result);
TQ3Vector3D *			E3Quaternion_GetAxisAndAngle(const TQ3Quaternion *q, TQ3Vector3D *outAxis, float *outAngle);
TQ3Vector3D *			E3Vector3D_TransformQuaternion(const TQ3Vector3D *vector3D, const TQ3Quaternion *quaternion, TQ3Vector3D *result);
TQ3Point3D *			E3Point3D_TransformQuaternion(const TQ3Point3D *point3D, const TQ3Quaternion *quaternion, TQ3Point3D *result);





//=============================================================================
//      Bounding box functions
//-----------------------------------------------------------------------------
TQ3BoundingBox *		E3BoundingBox_Reset(TQ3BoundingBox *bBox);
TQ3BoundingBox *		E3BoundingBox_Set(TQ3BoundingBox *bBox, const TQ3Point3D *min, const TQ3Point3D *max, TQ3Boolean isEmpty);
TQ3BoundingBox *		E3BoundingBox_SetFromPoints3D(TQ3BoundingBox *bBox, const TQ3Point3D *points3D, TQ3Uns32 numPoints, TQ3Uns32 structSize);
TQ3BoundingBox *		E3BoundingBox_SetFromRationalPoints4D(TQ3BoundingBox *bBox, const TQ3RationalPoint4D *rationalPoints4D, TQ3Uns32 numPoints, TQ3Uns32 structSize);
TQ3BoundingBox *		E3BoundingBox_Copy(const TQ3BoundingBox *bBox, TQ3BoundingBox *result);
TQ3BoundingBox *		E3BoundingBox_Union(const TQ3BoundingBox *b1, const TQ3BoundingBox *b2, TQ3BoundingBox *result);
TQ3BoundingBox *		E3BoundingBox_UnionPoint3D(const TQ3BoundingBox *bBox, const TQ3Point3D *point3D, TQ3BoundingBox *result);
TQ3BoundingBox *		E3BoundingBox_UnionRationalPoint4D(const TQ3BoundingBox *bBox, const TQ3RationalPoint4D *rationalPoint4D, TQ3BoundingBox *result);





//=============================================================================
//      Bounding sphere functions
//-----------------------------------------------------------------------------
TQ3BoundingSphere *		E3BoundingSphere_Reset(TQ3BoundingSphere *bSphere);
TQ3BoundingSphere *		E3BoundingSphere_Set(TQ3BoundingSphere *bSphere, const TQ3Point3D *origin, float radius, TQ3Boolean isEmpty);
TQ3BoundingSphere *		E3BoundingSphere_SetFromPoints3D(TQ3BoundingSphere *bSphere, const TQ3Point3D *points3D, TQ3Uns32 numPoints, TQ3Uns32 structSize);
TQ3BoundingSphere *		E3BoundingSphere_SetFromRationalPoints4D(TQ3BoundingSphere *bSphere, const TQ3RationalPoint4D *rationalPoints4D, TQ3Uns32 numPoints, TQ3Uns32 structSize);
TQ3BoundingSphere *		E3BoundingSphere_Copy(const TQ3BoundingSphere *bSphere, TQ3BoundingSphere *result);
TQ3BoundingSphere *		E3BoundingSphere_Union(const TQ3BoundingSphere *s1, const TQ3BoundingSphere *s2, TQ3BoundingSphere *result);
TQ3BoundingSphere *		E3BoundingSphere_UnionPoint3D(const TQ3BoundingSphere *bSphere, const TQ3Point3D *point3D, TQ3BoundingSphere *result);
TQ3BoundingSphere *		E3BoundingSphere_UnionRationalPoint4D(const TQ3BoundingSphere *bSphere, const TQ3RationalPoint4D *rationalPoint4D, TQ3BoundingSphere *result);





//=============================================================================
//      Intersection functions
//-----------------------------------------------------------------------------
TQ3Boolean				E3Ray3D_IntersectSphere(const TQ3Ray3D *theRay, const TQ3Sphere *theSphere, TQ3Point3D *hitPoint);
TQ3Boolean				E3Ray3D_IntersectBoundingBox(const TQ3Ray3D *theRay, const TQ3BoundingBox *theBounds, TQ3Point3D *hitPoint);
TQ3Boolean				E3Ray3D_IntersectTriangle(const TQ3Ray3D		*theRay,
													const TQ3Point3D	*point1,
													const TQ3Point3D	*point2,
													const TQ3Point3D	*point3,
													TQ3Boolean			cullBackfacing,
													TQ3Param3D			*hitPoint);





//=============================================================================
//      General math functions
//-----------------------------------------------------------------------------
float					E3Math_SquareRoot(float x);
float					E3Math_InvSquareRoot(float x);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


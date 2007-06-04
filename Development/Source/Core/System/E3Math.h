/*  NAME:
        E3Math.h

    DESCRIPTION:
        Header file for E3Math.c.

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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
TQ3Status				E3Vector3D_DotArray(const TQ3Vector3D *inFirstVectors3D, const TQ3Vector3D *inSecondVectors3D, float *outDotProducts, TQ3Boolean *outDotLessThanZeros, TQ3Uns32 numVectors, TQ3Uns32 inStructSize, TQ3Uns32 outDotProductStructSize, TQ3Uns32 outDotLessThanZeroStructSize);





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
float					E3Point3D_BoundingBox_DistanceSquared( const TQ3Point3D *p, const TQ3BoundingBox* box,
								TQ3Point3D* outNearest );





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
TQ3Status				E3Point2D_To3DTransformArray(const TQ3Point2D *inPoints2D, const TQ3Matrix3x3 *matrix3x3, TQ3RationalPoint3D *outRationalPoints3D, TQ3Uns32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize);
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
void					E3BoundingBox_GetCorners( const TQ3BoundingBox *inBox, TQ3Point3D* out8Corners );
void					E3BoundingBox_Transform( const TQ3BoundingBox *inBox, const TQ3Matrix4x4* inMtx,
												TQ3BoundingBox* outBox );





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


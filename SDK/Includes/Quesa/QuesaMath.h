/*  NAME:
        QuesaMath.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#ifndef QUESA_MATH_HDR
#define QUESA_MATH_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#ifdef __QD3DMATH__
#error
#endif

#define __QD3DMATH__

#include <float.h>





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#ifdef FLT_EPSILON
	#define kQ3RealZero							(FLT_EPSILON)
#else
	#define kQ3RealZero							((TQ3Float32) 1.19209290e-07)
#endif

#ifdef FLT_MAX
	#define kQ3MaxFloat							(FLT_MAX)
#else
	#define kQ3MaxFloat							((TQ3Float32) 3.40282347e+38)
#endif

#define kQ3Pi									((TQ3Float32) 3.1415926535898)
#define kQ32Pi									((TQ3Float32) (2.0 * 3.1415926535898))
#define kQ3PiOver2								((TQ3Float32) (3.1415926535898 / 2.0))
#define kQ33PiOver2								((TQ3Float32) (3.0 * 3.1415926535898 / 2.0))





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
#define Q3Math_DegreesToRadians(_x)				((TQ3Float32) ((_x) *  kQ3Pi / 180.0f))
#define Q3Math_RadiansToDegrees(_x)				((TQ3Float32) ((_x) * 180.0f / kQ3Pi))
#define Q3Math_Min(_x,_y)						((_x) <= (_y) ? (_x) : (_y))
#define Q3Math_Max(_x,_y)						((_x) >= (_y) ? (_x) : (_y))





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*
 *	Q3Point2D_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_Set (
	TQ3Point2D                    *point2D,
	float                         x,
	float                         y
);



/*
 *	Q3Param2D_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_Set (
	TQ3Param2D                    *param2D,
	float                         u,
	float                         v
);



/*
 *	Q3Point3D_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_Set (
	TQ3Point3D                    *point3D,
	float                         x,
	float                         y,
	float                         z
);



/*
 *	Q3RationalPoint3D_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3RationalPoint3D * )
Q3RationalPoint3D_Set (
	TQ3RationalPoint3D            *point3D,
	float                         x,
	float                         y,
	float                         w
);



/*
 *	Q3RationalPoint4D_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3RationalPoint4D * )
Q3RationalPoint4D_Set (
	TQ3RationalPoint4D            *point4D,
	float                         x,
	float                         y,
	float                         z,
	float                         w
);



/*
 *	Q3Vector2D_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Set (
	TQ3Vector2D                   *vector2D,
	float                         x,
	float                         y
);



/*
 *	Q3Vector3D_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Set (
	TQ3Vector3D                   *vector3D,
	float                         x,
	float                         y,
	float                         z
);



/*
 *	Q3PolarPoint_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3PolarPoint * )
Q3PolarPoint_Set (
	TQ3PolarPoint                 *polarPoint,
	float                         r,
	float                         theta
);



/*
 *	Q3SphericalPoint_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3SphericalPoint * )
Q3SphericalPoint_Set (
	TQ3SphericalPoint             *sphericalPoint,
	float                         rho,
	float                         theta,
	float                         phi
);



/*
 *	Q3Point2D_To3D
 *		Description of function
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point2D_To3D (
	const TQ3Point2D              *point2D,
	TQ3Point3D                    *result
);



/*
 *	Q3RationalPoint3D_To2D
 *		Description of function
 */
EXTERN_API_C ( TQ3Point2D * )
Q3RationalPoint3D_To2D (
	const TQ3RationalPoint3D      *point3D,
	TQ3Point2D                    *result
);



/*
 *	Q3Point3D_To4D
 *		Description of function
 */
EXTERN_API_C ( TQ3RationalPoint4D * )
Q3Point3D_To4D (
	const TQ3Point3D              *point3D,
	TQ3RationalPoint4D            *result
);



/*
 *	Q3RationalPoint4D_To3D
 *		Description of function
 */
EXTERN_API_C ( TQ3Point3D * )
Q3RationalPoint4D_To3D (
	const TQ3RationalPoint4D      *point4D,
	TQ3Point3D                    *result
);



/*
 *	Q3Vector2D_To3D
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector2D_To3D (
	const TQ3Vector2D             *vector2D,
	TQ3Vector3D                   *result
);



/*
 *	Q3Vector3D_To2D
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector3D_To2D (
	const TQ3Vector3D             *vector3D,
	TQ3Vector2D                   *result
);



/*
 *	Q3Point2D_Subtract
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Point2D_Subtract (
	const TQ3Point2D              *p1,
	const TQ3Point2D              *p2,
	TQ3Vector2D                   *result
);



/*
 *	Q3Param2D_Subtract
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Param2D_Subtract (
	const TQ3Param2D              *p1,
	const TQ3Param2D              *p2,
	TQ3Vector2D                   *result
);



/*
 *	Q3Point3D_Subtract
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Point3D_Subtract (
	const TQ3Point3D              *p1,
	const TQ3Point3D              *p2,
	TQ3Vector3D                   *result
);



/*
 *	Q3Point2D_Distance
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Point2D_Distance (
	const TQ3Point2D              *p1,
	const TQ3Point2D              *p2
);



/*
 *	Q3Point2D_DistanceSquared
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Point2D_DistanceSquared (
	const TQ3Point2D              *p1,
	const TQ3Point2D              *p2
);



/*
 *	Q3Param2D_Distance
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Param2D_Distance (
	const TQ3Param2D              *p1,
	const TQ3Param2D              *p2
);



/*
 *	Q3Param2D_DistanceSquared
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Param2D_DistanceSquared (
	const TQ3Param2D              *p1,
	const TQ3Param2D              *p2
);



/*
 *	Q3RationalPoint3D_Distance
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3RationalPoint3D_Distance (
	const TQ3RationalPoint3D      *p1,
	const TQ3RationalPoint3D      *p2
);



/*
 *	Q3RationalPoint3D_DistanceSquared
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3RationalPoint3D_DistanceSquared (
	const TQ3RationalPoint3D      *p1,
	const TQ3RationalPoint3D      *p2
);



/*
 *	Q3Point3D_Distance
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Point3D_Distance (
	const TQ3Point3D              *p1,
	const TQ3Point3D              *p2
);



/*
 *	Q3Point3D_DistanceSquared
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Point3D_DistanceSquared (
	const TQ3Point3D              *p1,
	const TQ3Point3D              *p2
);



/*
 *	Q3RationalPoint4D_Distance
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3RationalPoint4D_Distance (
	const TQ3RationalPoint4D      *p1,
	const TQ3RationalPoint4D      *p2
);



/*
 *	Q3RationalPoint4D_DistanceSquared
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3RationalPoint4D_DistanceSquared (
	const TQ3RationalPoint4D      *p1,
	const TQ3RationalPoint4D      *p2
);



/*
 *	Q3Point2D_RRatio
 *		Description of function
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_RRatio (
	const TQ3Point2D              *p1,
	const TQ3Point2D              *p2,
	float                         r1,
	float                         r2,
	TQ3Point2D                    *result
);



/*
 *	Q3Param2D_RRatio
 *		Description of function
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_RRatio (
	const TQ3Param2D              *p1,
	const TQ3Param2D              *p2,
	float                         r1,
	float                         r2,
	TQ3Param2D                    *result
);



/*
 *	Q3Point3D_RRatio
 *		Description of function
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_RRatio (
	const TQ3Point3D              *p1,
	const TQ3Point3D              *p2,
	float                         r1,
	float                         r2,
	TQ3Point3D                    *result
);



/*
 *	Q3RationalPoint4D_RRatio
 *		Description of function
 */
EXTERN_API_C ( TQ3RationalPoint4D * )
Q3RationalPoint4D_RRatio (
	const TQ3RationalPoint4D      *p1,
	const TQ3RationalPoint4D      *p2,
	float                         r1,
	float                         r2,
	TQ3RationalPoint4D            *result
);



/*
 *	Q3Point2D_Vector2D_Add
 *		Description of function
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_Vector2D_Add (
	const TQ3Point2D              *point2D,
	const TQ3Vector2D             *vector2D,
	TQ3Point2D                    *result
);



/*
 *	Q3Param2D_Vector2D_Add
 *		Description of function
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_Vector2D_Add (
	const TQ3Param2D              *param2D,
	const TQ3Vector2D             *vector2D,
	TQ3Param2D                    *result
);



/*
 *	Q3Point3D_Vector3D_Add
 *		Description of function
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_Vector3D_Add (
	const TQ3Point3D              *point3D,
	const TQ3Vector3D             *vector3D,
	TQ3Point3D                    *result
);



/*
 *	Q3Point2D_Vector2D_Subtract
 *		Description of function
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_Vector2D_Subtract (
	const TQ3Point2D              *point2D,
	const TQ3Vector2D             *vector2D,
	TQ3Point2D                    *result
);



/*
 *	Q3Param2D_Vector2D_Subtract
 *		Description of function
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_Vector2D_Subtract (
	const TQ3Param2D              *param2D,
	const TQ3Vector2D             *vector2D,
	TQ3Param2D                    *result
);



/*
 *	Q3Point3D_Vector3D_Subtract
 *		Description of function
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_Vector3D_Subtract (
	const TQ3Point3D              *point3D,
	const TQ3Vector3D             *vector3D,
	TQ3Point3D                    *result
);



/*
 *	Q3Vector2D_Scale
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Scale (
	const TQ3Vector2D             *vector2D,
	float                         scalar,
	TQ3Vector2D                   *result
);



/*
 *	Q3Vector3D_Scale
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Scale (
	const TQ3Vector3D             *vector3D,
	float                         scalar,
	TQ3Vector3D                   *result
);



/*
 *	Q3Vector2D_Length
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Vector2D_Length (
	const TQ3Vector2D             *vector2D
);



/*
 *	Q3Vector3D_Length
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Vector3D_Length (
	const TQ3Vector3D             *vector3D
);



/*
 *	Q3Vector2D_Normalize
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Normalize (
	const TQ3Vector2D             *vector2D,
	TQ3Vector2D                   *result
);



/*
 *	Q3Vector3D_Normalize
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Normalize (
	const TQ3Vector3D             *vector3D,
	TQ3Vector3D                   *result
);



/*
 *	Q3Vector2D_Add
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Add (
	const TQ3Vector2D             *v1,
	const TQ3Vector2D             *v2,
	TQ3Vector2D                   *result
);



/*
 *	Q3Vector3D_Add
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Add (
	const TQ3Vector3D             *v1,
	const TQ3Vector3D             *v2,
	TQ3Vector3D                   *result
);



/*
 *	Q3Vector2D_Subtract
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Subtract (
	const TQ3Vector2D             *v1,
	const TQ3Vector2D             *v2,
	TQ3Vector2D                   *result
);



/*
 *	Q3Vector3D_Subtract
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Subtract (
	const TQ3Vector3D             *v1,
	const TQ3Vector3D             *v2,
	TQ3Vector3D                   *result
);



/*
 *	Q3Vector2D_Cross
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Vector2D_Cross (
	const TQ3Vector2D             *v1,
	const TQ3Vector2D             *v2
);



/*
 *	Q3Vector3D_Cross
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Cross (
	const TQ3Vector3D             *v1,
	const TQ3Vector3D             *v2,
	TQ3Vector3D                   *result
);



/*
 *	Q3Point3D_CrossProductTri
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Point3D_CrossProductTri (
	const TQ3Point3D              *point1,
	const TQ3Point3D              *point2,
	const TQ3Point3D              *point3,
	TQ3Vector3D                   *crossVector
);



/*
 *	Q3Vector2D_Dot
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Vector2D_Dot (
	const TQ3Vector2D             *v1,
	const TQ3Vector2D             *v2
);



/*
 *	Q3Vector3D_Dot
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Vector3D_Dot (
	const TQ3Vector3D             *v1,
	const TQ3Vector3D             *v2
);



/*
 *	Q3Vector2D_Transform
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Transform (
	const TQ3Vector2D             *vector2D,
	const TQ3Matrix3x3            *matrix3x3,
	TQ3Vector2D                   *result
);



/*
 *	Q3Vector3D_Transform
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Transform (
	const TQ3Vector3D             *vector3D,
	const TQ3Matrix4x4            *matrix4x4,
	TQ3Vector3D                   *result
);



/*
 *	Q3Point2D_Transform
 *		Description of function
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_Transform (
	const TQ3Point2D              *point2D,
	const TQ3Matrix3x3            *matrix3x3,
	TQ3Point2D                    *result
);



/*
 *	Q3Param2D_Transform
 *		Description of function
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_Transform (
	const TQ3Param2D              *param2D,
	const TQ3Matrix3x3            *matrix3x3,
	TQ3Param2D                    *result
);



/*
 *	Q3Point3D_Transform
 *		Description of function
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_Transform (
	const TQ3Point3D              *point3D,
	const TQ3Matrix4x4            *matrix4x4,
	TQ3Point3D                    *result
);



/*
 *	Q3RationalPoint4D_Transform
 *		Description of function
 */
EXTERN_API_C ( TQ3RationalPoint4D * )
Q3RationalPoint4D_Transform (
	const TQ3RationalPoint4D      *point4D,
	const TQ3Matrix4x4            *matrix4x4,
	TQ3RationalPoint4D            *result
);



/*
 *	Q3Point3D_To3DTransformArray
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point3D_To3DTransformArray (
	const TQ3Point3D              *inPoint3D,
	const TQ3Matrix4x4            *matrix,
	TQ3Point3D                    *outPoint3D,
	TQ3Int32                      numPoints,
	TQ3Uns32                      inStructSize,
	TQ3Uns32                      outStructSize
);



/*
 *	Q3Point3D_To4DTransformArray
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point3D_To4DTransformArray (
	const TQ3Point3D              *inPoint3D,
	const TQ3Matrix4x4            *matrix,
	TQ3RationalPoint4D            *outPoint4D,
	TQ3Int32                      numPoints,
	TQ3Uns32                      inStructSize,
	TQ3Uns32                      outStructSize
);



/*
 *	Q3RationalPoint4D_To4DTransformArray
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RationalPoint4D_To4DTransformArray (
	const TQ3RationalPoint4D      *inPoint4D,
	const TQ3Matrix4x4            *matrix,
	TQ3RationalPoint4D            *outPoint4D,
	TQ3Int32                      numPoints,
	TQ3Uns32                      inStructSize,
	TQ3Uns32                      outStructSize
);



/*
 *	Q3Vector2D_Negate
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Negate (
	const TQ3Vector2D             *vector2D,
	TQ3Vector2D                   *result
);



/*
 *	Q3Vector3D_Negate
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Negate (
	const TQ3Vector3D             *vector3D,
	TQ3Vector3D                   *result
);



/*
 *	Q3Point2D_ToPolar
 *		Description of function
 */
EXTERN_API_C ( TQ3PolarPoint * )
Q3Point2D_ToPolar (
	const TQ3Point2D              *point2D,
	TQ3PolarPoint                 *result
);



/*
 *	Q3PolarPoint_ToPoint2D
 *		Description of function
 */
EXTERN_API_C ( TQ3Point2D * )
Q3PolarPoint_ToPoint2D (
	const TQ3PolarPoint           *polarPoint,
	TQ3Point2D                    *result
);



/*
 *	Q3Point3D_ToSpherical
 *		Description of function
 */
EXTERN_API_C ( TQ3SphericalPoint * )
Q3Point3D_ToSpherical (
	const TQ3Point3D              *point3D,
	TQ3SphericalPoint             *result
);



/*
 *	Q3SphericalPoint_ToPoint3D
 *		Description of function
 */
EXTERN_API_C ( TQ3Point3D * )
Q3SphericalPoint_ToPoint3D (
	const TQ3SphericalPoint       *sphericalPoint,
	TQ3Point3D                    *result
);



/*
 *	Q3Point2D_AffineComb
 *		Description of function
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_AffineComb (
	const TQ3Point2D              *points2D,
	const float                   *weights,
	TQ3Uns32                      nPoints,
	TQ3Point2D                    *result
);



/*
 *	Q3Param2D_AffineComb
 *		Description of function
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_AffineComb (
	const TQ3Param2D              *params2D,
	const float                   *weights,
	TQ3Uns32                      nPoints,
	TQ3Param2D                    *result
);



/*
 *	Q3RationalPoint3D_AffineComb
 *		Description of function
 */
EXTERN_API_C ( TQ3RationalPoint3D * )
Q3RationalPoint3D_AffineComb (
	const TQ3RationalPoint3D      *points3D,
	const float                   *weights,
	TQ3Uns32                      numPoints,
	TQ3RationalPoint3D            *result
);



/*
 *	Q3Point3D_AffineComb
 *		Description of function
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_AffineComb (
	const TQ3Point3D              *points3D,
	const float                   *weights,
	TQ3Uns32                      numPoints,
	TQ3Point3D                    *result
);



/*
 *	Q3RationalPoint4D_AffineComb
 *		Description of function
 */
EXTERN_API_C ( TQ3RationalPoint4D * )
Q3RationalPoint4D_AffineComb (
	const TQ3RationalPoint4D      *points4D,
	const float                   *weights,
	TQ3Uns32                      numPoints,
	TQ3RationalPoint4D            *result
);



/*
 *	Q3Matrix3x3_Copy
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_Copy (
	const TQ3Matrix3x3            *matrix3x3,
	TQ3Matrix3x3                  *result
);



/*
 *	Q3Matrix4x4_Copy
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_Copy (
	const TQ3Matrix4x4            *matrix4x4,
	TQ3Matrix4x4                  *result
);



/*
 *	Q3Matrix3x3_SetIdentity
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_SetIdentity (
	TQ3Matrix3x3                  *matrix3x3
);



/*
 *	Q3Matrix4x4_SetIdentity
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetIdentity (
	TQ3Matrix4x4                  *matrix4x4
);



/*
 *	Q3Matrix3x3_Transpose
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_Transpose (
	const TQ3Matrix3x3            *matrix3x3,
	TQ3Matrix3x3                  *result
);



/*
 *	Q3Matrix4x4_Transpose
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_Transpose (
	const TQ3Matrix4x4            *matrix4x4,
	TQ3Matrix4x4                  *result
);



/*
 *	Q3Matrix3x3_Invert
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_Invert (
	const TQ3Matrix3x3            *matrix3x3,
	TQ3Matrix3x3                  *result
);



/*
 *	Q3Matrix4x4_Invert
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_Invert (
	const TQ3Matrix4x4            *matrix4x4,
	TQ3Matrix4x4                  *result
);



/*
 *	Q3Matrix3x3_Adjoint
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_Adjoint (
	const TQ3Matrix3x3            *matrix3x3,
	TQ3Matrix3x3                  *result
);



/*
 *	Q3Matrix3x3_Multiply
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_Multiply (
	const TQ3Matrix3x3            *matrixA,
	const TQ3Matrix3x3            *matrixB,
	TQ3Matrix3x3                  *result
);



/*
 *	Q3Matrix4x4_Multiply
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_Multiply (
	const TQ3Matrix4x4            *matrixA,
	const TQ3Matrix4x4            *matrixB,
	TQ3Matrix4x4                  *result
);



/*
 *	Q3Matrix3x3_SetTranslate
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_SetTranslate (
	TQ3Matrix3x3                  *matrix3x3,
	float                         xTrans,
	float                         yTrans
);



/*
 *	Q3Matrix3x3_SetScale
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_SetScale (
	TQ3Matrix3x3                  *matrix3x3,
	float                         xScale,
	float                         yScale
);



/*
 *	Q3Matrix3x3_SetRotateAboutPoint
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_SetRotateAboutPoint (
	TQ3Matrix3x3                  *matrix3x3,
	const TQ3Point2D              *origin,
	float                         angle
);



/*
 *	Q3Matrix4x4_SetTranslate
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetTranslate (
	TQ3Matrix4x4                  *matrix4x4,
	float                         xTrans,
	float                         yTrans,
	float                         zTrans
);



/*
 *	Q3Matrix4x4_SetScale
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetScale (
	TQ3Matrix4x4                  *matrix4x4,
	float                         xScale,
	float                         yScale,
	float                         zScale
);



/*
 *	Q3Matrix4x4_SetRotateAboutPoint
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotateAboutPoint (
	TQ3Matrix4x4                  *matrix4x4,
	const TQ3Point3D              *origin,
	float                         xAngle,
	float                         yAngle,
	float                         zAngle
);



/*
 *	Q3Matrix4x4_SetRotateAboutAxis
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotateAboutAxis (
	TQ3Matrix4x4                  *matrix4x4,
	const TQ3Point3D              *origin,
	const TQ3Vector3D             *orientation,
	float                         angle
);



/*
 *	Q3Matrix4x4_SetRotate_X
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotate_X (
	TQ3Matrix4x4                  *matrix4x4,
	float                         angle
);



/*
 *	Q3Matrix4x4_SetRotate_Y
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotate_Y (
	TQ3Matrix4x4                  *matrix4x4,
	float                         angle
);



/*
 *	Q3Matrix4x4_SetRotate_Z
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotate_Z (
	TQ3Matrix4x4                  *matrix4x4,
	float                         angle
);



/*
 *	Q3Matrix4x4_SetRotate_XYZ
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotate_XYZ (
	TQ3Matrix4x4                  *matrix4x4,
	float                         xAngle,
	float                         yAngle,
	float                         zAngle
);



/*
 *	Q3Matrix4x4_SetRotateVectorToVector
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotateVectorToVector (
	TQ3Matrix4x4                  *matrix4x4,
	const TQ3Vector3D             *v1,
	const TQ3Vector3D             *v2
);



/*
 *	Q3Matrix4x4_SetQuaternion
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetQuaternion (
	TQ3Matrix4x4                  *matrix,
	const TQ3Quaternion           *quaternion
);



/*
 *	Q3Matrix3x3_Determinant
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Matrix3x3_Determinant (
	const TQ3Matrix3x3            *matrix3x3
);



/*
 *	Q3Matrix4x4_Determinant
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Matrix4x4_Determinant (
	const TQ3Matrix4x4            *matrix4x4
);



/*
 *	Q3Quaternion_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_Set (
	TQ3Quaternion                 *quaternion,
	float                         w,
	float                         x,
	float                         y,
	float                         z
);



/*
 *	Q3Quaternion_SetIdentity
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetIdentity (
	TQ3Quaternion                 *quaternion
);



/*
 *	Q3Quaternion_Copy
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_Copy (
	const TQ3Quaternion           *quaternion,
	TQ3Quaternion                 *result
);



/*
 *	Q3Quaternion_IsIdentity
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Quaternion_IsIdentity (
	const TQ3Quaternion           *quaternion
);



/*
 *	Q3Quaternion_Invert
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_Invert (
	const TQ3Quaternion           *quaternion,
	TQ3Quaternion                 *result
);



/*
 *	Q3Quaternion_Normalize
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_Normalize (
	const TQ3Quaternion           *quaternion,
	TQ3Quaternion                 *result
);



/*
 *	Q3Quaternion_Dot
 *		Description of function
 */
EXTERN_API_C ( float  )
Q3Quaternion_Dot (
	const TQ3Quaternion           *q1,
	const TQ3Quaternion           *q2
);



/*
 *	Q3Quaternion_Multiply
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_Multiply (
	const TQ3Quaternion           *q1,
	const TQ3Quaternion           *q2,
	TQ3Quaternion                 *result
);



/*
 *	Q3Quaternion_SetRotateAboutAxis
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotateAboutAxis (
	TQ3Quaternion                 *quaternion,
	const TQ3Vector3D             *axis,
	float                         angle
);



/*
 *	Q3Quaternion_SetRotate_XYZ
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotate_XYZ (
	TQ3Quaternion                 *quaternion,
	float                         xAngle,
	float                         yAngle,
	float                         zAngle
);



/*
 *	Q3Quaternion_SetRotate_X
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotate_X (
	TQ3Quaternion                 *quaternion,
	float                         angle
);



/*
 *	Q3Quaternion_SetRotate_Y
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotate_Y (
	TQ3Quaternion                 *quaternion,
	float                         angle
);



/*
 *	Q3Quaternion_SetRotate_Z
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotate_Z (
	TQ3Quaternion                 *quaternion,
	float                         angle
);



/*
 *	Q3Quaternion_SetMatrix
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetMatrix (
	TQ3Quaternion                 *quaternion,
	const TQ3Matrix4x4            *matrix
);



/*
 *	Q3Quaternion_SetRotateVectorToVector
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotateVectorToVector (
	TQ3Quaternion                 *quaternion,
	const TQ3Vector3D             *v1,
	const TQ3Vector3D             *v2
);



/*
 *	Q3Quaternion_MatchReflection
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_MatchReflection (
	const TQ3Quaternion           *q1,
	const TQ3Quaternion           *q2,
	TQ3Quaternion                 *result
);



/*
 *	Q3Quaternion_InterpolateFast
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_InterpolateFast (
	const TQ3Quaternion           *q1,
	const TQ3Quaternion           *q2,
	float                         t,
	TQ3Quaternion                 *result
);



/*
 *	Q3Quaternion_InterpolateLinear
 *		Description of function
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_InterpolateLinear (
	const TQ3Quaternion           *q1,
	const TQ3Quaternion           *q2,
	float                         t,
	TQ3Quaternion                 *result
);



/*
 *	Q3Vector3D_TransformQuaternion
 *		Description of function
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_TransformQuaternion (
	const TQ3Vector3D             *vector3D,
	const TQ3Quaternion           *quaternion,
	TQ3Vector3D                   *result
);



/*
 *	Q3Point3D_TransformQuaternion
 *		Description of function
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_TransformQuaternion (
	const TQ3Point3D              *point3D,
	const TQ3Quaternion           *quaternion,
	TQ3Point3D                    *result
);



/*
 *	Q3BoundingBox_Copy
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_Copy (
	const TQ3BoundingBox          *src,
	TQ3BoundingBox                *dest
);



/*
 *	Q3BoundingBox_Union
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_Union (
	const TQ3BoundingBox          *v1,
	const TQ3BoundingBox          *v2,
	TQ3BoundingBox                *result
);



/*
 *	Q3BoundingBox_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_Set (
	TQ3BoundingBox                *bBox,
	const TQ3Point3D              *min,
	const TQ3Point3D              *max,
	TQ3Boolean                    isEmpty
);



/*
 *	Q3BoundingBox_UnionPoint3D
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_UnionPoint3D (
	const TQ3BoundingBox          *bBox,
	const TQ3Point3D              *point3D,
	TQ3BoundingBox                *result
);



/*
 *	Q3BoundingBox_UnionRationalPoint4D
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_UnionRationalPoint4D (
	const TQ3BoundingBox          *bBox,
	const TQ3RationalPoint4D      *point4D,
	TQ3BoundingBox                *result
);



/*
 *	Q3BoundingBox_SetFromPoints3D
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_SetFromPoints3D (
	TQ3BoundingBox                *bBox,
	const TQ3Point3D              *points3D,
	TQ3Uns32                      numPoints,
	TQ3Uns32                      structSize
);



/*
 *	Q3BoundingBox_SetFromRationalPoints4D
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_SetFromRationalPoints4D (
	TQ3BoundingBox                *bBox,
	const TQ3RationalPoint4D      *points4D,
	TQ3Uns32                      numPoints,
	TQ3Uns32                      structSize
);



/*
 *	Q3BoundingSphere_Copy
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_Copy (
	const TQ3BoundingSphere       *src,
	TQ3BoundingSphere             *dest
);



/*
 *	Q3BoundingSphere_Union
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_Union (
	const TQ3BoundingSphere       *s1,
	const TQ3BoundingSphere       *s2,
	TQ3BoundingSphere             *result
);



/*
 *	Q3BoundingSphere_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_Set (
	TQ3BoundingSphere             *bSphere,
	const TQ3Point3D              *origin,
	float                         radius,
	TQ3Boolean                    isEmpty
);



/*
 *	Q3BoundingSphere_UnionPoint3D
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_UnionPoint3D (
	const TQ3BoundingSphere       *bSphere,
	const TQ3Point3D              *point3D,
	TQ3BoundingSphere             *result
);



/*
 *	Q3BoundingSphere_UnionRationalPoint4D
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_UnionRationalPoint4D (
	const TQ3BoundingSphere       *bSphere,
	const TQ3RationalPoint4D      *point4D,
	TQ3BoundingSphere             *result
);



/*
 *	Q3BoundingSphere_SetFromPoints3D
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_SetFromPoints3D (
	TQ3BoundingSphere             *bSphere,
	const TQ3Point3D              *points3D,
	TQ3Uns32                      numPoints,
	TQ3Uns32                      structSize
);



/*
 *	Q3BoundingSphere_SetFromRationalPoints4D
 *		Description of function
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_SetFromRationalPoints4D (
	TQ3BoundingSphere             *bSphere,
	const TQ3RationalPoint4D      *points4D,
	TQ3Uns32                      numPoints,
	TQ3Uns32                      structSize
);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


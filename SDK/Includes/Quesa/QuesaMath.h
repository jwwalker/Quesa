/*! @header QuesaMath.h
        Declares the Quesa math utilities.
 */
/*  NAME:
        QuesaMath.h

    DESCRIPTION:
        Quesa public header.

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
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#ifdef FLT_EPSILON
    #define kQ3RealZero                         (FLT_EPSILON)
#else
    #define kQ3RealZero                         ((TQ3Float32) 1.19209290e-07)
#endif

#ifdef FLT_MAX
    #define kQ3MaxFloat                         (FLT_MAX)
#else
    #define kQ3MaxFloat                         ((TQ3Float32) 3.40282347e+38)
#endif

#define kQ3Pi                                   ((TQ3Float32) 3.1415926535898)
#define kQ32Pi                                  ((TQ3Float32) (2.0 * 3.1415926535898))
#define kQ3PiOver2                              ((TQ3Float32) (3.1415926535898 / 2.0))
#define kQ33PiOver2                             ((TQ3Float32) (3.0 * 3.1415926535898 / 2.0))





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
#define Q3Math_DegreesToRadians(_x)             ((TQ3Float32) ((_x) *  kQ3Pi / 180.0f))
#define Q3Math_RadiansToDegrees(_x)             ((TQ3Float32) ((_x) * 180.0f / kQ3Pi))
#define Q3Math_Min(_x,_y)                       ((_x) <= (_y) ? (_x) : (_y))
#define Q3Math_Max(_x,_y)                       ((_x) >= (_y) ? (_x) : (_y))





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
//      Point and Vector creation
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Vector2D_Set
 *  @discussion
 *      Set a 2D vector.
 *
 *  @param vector2D         Address of vector to set (may be NULL).
 *  @param x                X coordinate to set into vector2D.
 *  @param y                Y coordinate to set into vector2D.
 *  @result                 Convenience copy of vector2D parameter.
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Set (
    TQ3Vector2D                   *vector2D,
    float                         x,
    float                         y
);



/*!
 *  @function
 *      Q3Vector3D_Set
 *  @discussion
 *      Set a 3D vector.
 *
 *  @param vector3D         Address of vector to set (may be NULL).
 *  @param x                X coordinate to set into vector3D.
 *  @param y                Y coordinate to set into vector3D.
 *  @param z                Z coordinate to set into vector3D.
 *  @result                 Convenience copy of vector3D parameter.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Set (
    TQ3Vector3D                   *vector3D,
    float                         x,
    float                         y,
    float                         z
);



/*!
 *  @function
 *      Q3Point2D_Set
 *  @discussion
 *      Set a 2D point.
 *
 *  @param point2D          Address of point to set (may be NULL).
 *  @param x                X coordinate to set into vector2D.
 *  @param y                Y coordinate to set into vector2D.
 *  @result                 Convenience copy of point2D parameter.
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_Set (
    TQ3Point2D                    *point2D,
    float                         x,
    float                         y
);



/*!
 *  @function
 *      Q3Param2D_Set
 *  @discussion
 *      Set a 2D parameterization value (i.e., a UV coordinate).
 *
 *  @param param2D          Address of param2D to set (may be NULL).
 *  @param u                U coordinate to set into param2D.
 *  @param v                V coordinate to set into param2D.
 *  @result                 Convenience copy of param2D parameter.
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_Set (
    TQ3Param2D                    *param2D,
    float                         u,
    float                         v
);



/*!
 *  @function
 *      Q3RationalPoint3D_Set
 *  @discussion
 *      Set a 3D rational point (x,y,w).
 *
 *  @param rationalPoint3D  Address of rational point to set (may be NULL).
 *  @param x                X coordinate to set into rationalPoint3D.
 *  @param y                Y coordinate to set into rationalPoint3D.
 *  @param w                W coordinate to set into rationalPoint3D.
 *  @result                 Convenience copy of rationalPoint3D parameter.
 */
EXTERN_API_C ( TQ3RationalPoint3D * )
Q3RationalPoint3D_Set (
    TQ3RationalPoint3D            *rationalPoint3D,
    float                         x,
    float                         y,
    float                         w
);



/*!
 *  @function
 *      Q3Point3D_Set
 *  @discussion
 *      Set a 3D point.
 *
 *  @param point3D          Address of point to set (may be NULL).
 *  @param x                X coordinate to set into point3D.
 *  @param y                Y coordinate to set into point3D.
 *  @param z                Z coordinate to set into point3D.
 *  @result                 Convenience copy of point3D parameter.
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_Set (
    TQ3Point3D                    *point3D,
    float                         x,
    float                         y,
    float                         z
);



/*!
 *  @function
 *      Q3RationalPoint4D_Set
 *  @discussion
 *      Set a 4D rational point (x,y,z,w).
 *
 *  @param rationalPoint4D  Address of rational point to set.
 *  @param x                X coordinate to set into rationalPoint4D.
 *  @param y                Y coordinate to set into rationalPoint4D.
 *  @param z                Z coordinate to set into rationalPoint4D.
 *  @result                 Convenience copy of rationalPoint4D parameter.
 */
EXTERN_API_C ( TQ3RationalPoint4D * )
Q3RationalPoint4D_Set (
    TQ3RationalPoint4D            *rationalPoint4D,
    float                         x,
    float                         y,
    float                         z,
    float                         w
);



/*!
 *  @function
 *      Q3PolarPoint_Set
 *  @discussion
 *      Set a 2D polar-coordinates point.
 *
 *  @param polarPoint       Address of point to set (may be NULL).
 *  @param r                Radius coordinate to set into polarPoint.
 *  @param theta            Angle coordinate (in radians) to set into polarPoint.
 *  @result                 Convenience copy of polarPoint parameter.
 */
EXTERN_API_C ( TQ3PolarPoint * )
Q3PolarPoint_Set (
    TQ3PolarPoint                 *polarPoint,
    float                         r,
    float                         theta
);



/*!
 *  @function
 *      Q3SphericalPoint_Set
 *  @discussion
 *      Set a 3D spherical-coordinates point.
 *
 *  @param sphericalPoint   Address of point to set (may be NULL).
 *  @param rho              Rho coordinate to set into sphericalPoint.
 *  @param theta            Theta coordinate to set into sphericalPoint.
 *  @param phi              Phi coordinate to set into sphericalPoint.
 *  @result                 Convenience copy of polarPoint parameter.
 */
EXTERN_API_C ( TQ3SphericalPoint * )
Q3SphericalPoint_Set (
    TQ3SphericalPoint             *sphericalPoint,
    float                         rho,
    float                         theta,
    float                         phi
);





//=============================================================================
//      Point and Vector dimension conversion
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Vector2D_To3D
 *  @discussion
 *      Convert 2D vector to 3D, by setting z to 1.
 *
 *		Note: this operation makes no sense mathematically, but is included
 *		for backward-compatibility with QD3D.  Perhaps the QD3D
 *		implementation was really intended to convert a 2D vector into a 3D
 *		rational point -- see QDPoint2D_To3D, which does exactly that.
 *
 *  @param vector2D         Address of 2D vector to convert.
 *  @param result           Address of 3D vector to set.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector2D_To3D (
    const TQ3Vector2D             *vector2D,
    TQ3Vector3D                   *result
);



/*!
 *  @function
 *      Q3Vector2D_ToRationalPoint3D
 *  @discussion
 *		Convert 2D vector to 3D rational point, setting w to 0.
 *
 *      Not supported by QD3D.
 *
 *  @param vector2D         Address of 2D vector to convert.
 *  @param result           Address of 3D rational point to set.
 *  @result                 Convenience copy of result parameter.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3RationalPoint3D * )
Q3Vector2D_ToRationalPoint3D (
    const TQ3Vector2D             *vector2D,
    TQ3RationalPoint3D            *result
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Vector3D_To2D
 *  @discussion
 *      Convert 3D vector to 2D, dividing by z.
 *
 *		Note: this operation makes no sense mathematically, but is included
 *		for backward-compatibility with QD3D.  It's possible that the QD3D
 *		function was really intended to convert a 3D rational point to a
 *		2D vector -- see E3RationalPoint3D_To2D, which does the same thing
 *		for a 2D point.
 *
 *  @param vector3D         Address of 3D vector to convert.
 *  @param result           Address of 2D vector to set.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector3D_To2D (
    const TQ3Vector3D             *vector3D,
    TQ3Vector2D                   *result
);



/*!
 *  @function
 *      Q3RationalPoint3D_ToVector2D
 *  @discussion
 *		Convert 3D rational point to 2D vector, discarding w.
 *
 *      Not supported by QD3D.
 *
 *  @param rationalPoint3D  Address of 3D rational point to convert.
 *  @param result           Address of 2D vector to set.
 *  @result                 Convenience copy of result parameter.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Vector2D * )
Q3RationalPoint3D_ToVector2D (
    const TQ3RationalPoint3D      *rationalPoint3D,
    TQ3Vector2D                   *result
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Vector3D_ToRationalPoint4D
 *  @discussion
 *		Convert 3D vector to 4D rational point, setting w to 0.
 *
 *      Not supported by QD3D.
 *
 *  @param vector3D         Address of 3D vector to convert.
 *  @param result           Address of 4D rational point to set.
 *  @result                 Convenience copy of result parameter.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3RationalPoint4D * )
Q3Vector3D_ToRationalPoint4D (
    const TQ3Vector3D             *vector3D,
    TQ3RationalPoint4D            *result
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3RationalPoint4D_ToVector3D
 *  @discussion
 *		Convert 4D rational point to 3D vector, discarding w.
 *
 *      Not supported by QD3D.
 *
 *  @param rationalPoint4D  Address of 4D rational point to convert.
 *  @param result           Address of 3D vector to set.
 *  @result                 Convenience copy of result parameter.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Vector3D * )
Q3RationalPoint4D_ToVector3D (
    const TQ3RationalPoint4D      *rationalPoint4D,
    TQ3Vector3D                   *result
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Point2D_To3D
 *  @discussion
 *      Convert 2D point to rational 3D, setting w to 1.
 *
 *      The Apple version incorrectly declares the type of 'result' to
 *      be TQ3Point3D rather than TQ3RationalPoint3D. At a binary level
 *      there is no difference, but at the source code level the Apple
 *      version forces the use of the incorrect type or type casting.
 *
 *  @param point2D          Address of 2D point to convert.
 *  @param result           Address of 3D rational point to set.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3RationalPoint3D * )
Q3Point2D_To3D (
    const TQ3Point2D              *point2D,
    TQ3RationalPoint3D            *result
);



/*!
 *  @function
 *      Q3RationalPoint3D_To2D
 *  @discussion
 *      Convert rational 3D point to 2D, dividing by w.
 *
 *  @param rationalPoint3D  Address of rational 3D point to convert.
 *  @param result           Address of 2D point to set.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point2D * )
Q3RationalPoint3D_To2D (
    const TQ3RationalPoint3D      *rationalPoint3D,
    TQ3Point2D                    *result
);



/*!
 *  @function
 *      Q3Point3D_To4D
 *  @discussion
 *      Convert 3D point to rational 4D, setting w to 1.
 *
 *  @param point3D          Address of 3D point to convert.
 *  @param result           Address of rational 4D point to set.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3RationalPoint4D * )
Q3Point3D_To4D (
    const TQ3Point3D              *point3D,
    TQ3RationalPoint4D            *result
);



/*!
 *  @function
 *      Q3RationalPoint4D_To3D
 *  @discussion
 *      Convert rational 4D point to 3D, dividing by w.
 *
 *  @param rationalPoint4D  Address of rational 4D point to convert.
 *  @param result           Address of 3D point to set.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point3D * )
Q3RationalPoint4D_To3D (
    const TQ3RationalPoint4D      *rationalPoint4D,
    TQ3Point3D                    *result
);





//=============================================================================
//      Point conversion from cartesian to polar/spherical
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Point2D_ToPolar
 *  @discussion
 *      Convert 2D cartesian point to polar coordinates.
 *
 *      The angle (theta) here is measured counter-clockwise from the +x axis.
 *
 *  @param point2D          Address of 2D point to convert.
 *  @param result           Address of polar point to set.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3PolarPoint * )
Q3Point2D_ToPolar (
    const TQ3Point2D              *point2D,
    TQ3PolarPoint                 *result
);



/*!
 *  @function
 *      Q3PolarPoint_ToPoint2D
 *  @discussion
 *      Convert 2D polar point to cartesian coordinates.
 *
 *      The angle (theta) here is measured counter-clockwise from the +x axis.
 *
 *  @param polarPoint       Address of polar point to convert.
 *  @param result           Address of 2D cartesian point to set.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point2D * )
Q3PolarPoint_ToPoint2D (
    const TQ3PolarPoint           *polarPoint,
    TQ3Point2D                    *result
);



/*!
 *  @function
 *      Q3Point3D_ToSpherical
 *  @discussion
 *      Convert 3D cartesian point to spherical coordinates.
 *
 *  @param point3D          Address of 3D cartesian point to convert.
 *  @param result           Address of spherical-coordinates point to set.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3SphericalPoint * )
Q3Point3D_ToSpherical (
    const TQ3Point3D              *point3D,
    TQ3SphericalPoint             *result
);



/*!
 *  @function
 *      Q3SphericalPoint_ToPoint3D
 *  @discussion
 *      Convert 3D spherical point to cartesian coordinates.
 *
 *  @param sphericalPoint   Address of spherical-coordinates point to convert.
 *  @param result           Address of 3D cartesian point to set.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point3D * )
Q3SphericalPoint_ToPoint3D (
    const TQ3SphericalPoint       *sphericalPoint,
    TQ3Point3D                    *result
);





//=============================================================================
//      Dot product
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Vector2D_Dot
 *  @discussion
 *      Return the dot product of two 2D vectors.
 *
 *  @param v1               Address of first vector.
 *  @param v2               Address of second vector.
 *  @result                 Dot product of the two vectors.
 */
EXTERN_API_C ( float  )
Q3Vector2D_Dot (
    const TQ3Vector2D             *v1,
    const TQ3Vector2D             *v2
);



/*!
 *  @function
 *      Q3Vector3D_Dot
 *  @discussion
 *      Return the dot product of two 3D vectors.
 *
 *  @param v1               Address of first vector.
 *  @param v2               Address of second vector.
 *  @result                 Dot product of the two vectors.
 */
EXTERN_API_C ( float  )
Q3Vector3D_Dot (
    const TQ3Vector3D             *v1,
    const TQ3Vector3D             *v2
);





//=============================================================================
//      Cross product
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Vector2D_Cross
 *  @discussion
 *      Return the length of the cross product of two 2D vectors.
 *
 *		Equivalently, we assume that the 2D vectors are really 3D vectors with
 *		z=0, then return the z coordinate of the cross product (0,0,z).
 *
 *  @param v1               Address of first vector.
 *  @param v2               Address of second vector.
 *  @result                 Length of the 2D cross product.
 */
EXTERN_API_C ( float  )
Q3Vector2D_Cross (
    const TQ3Vector2D             *v1,
    const TQ3Vector2D             *v2
);



/*!
 *  @function
 *      Q3Point2D_CrossProductTri
 *  @discussion
 *		Return the length of the cross product of a triangle specified by
 *		three 2D points, that is, of the vectors p2-p1 and p3-p2.
 *
 *      Not supported by QD3D.
 *
 *  @param p1               Address of one point in the triangle.
 *  @param p2               Address of a second point in the triangle.
 *  @param p3               Address of a third point in the triangle.
 *  @result                 Length of (p2-p1) x (p3-p2).
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( float )
Q3Point2D_CrossProductTri (
    const TQ3Point2D              *p1,
    const TQ3Point2D              *p2,
    const TQ3Point2D              *p3
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Vector3D_Cross
 *  @discussion
 *      Return 3D cross product of two 3D vectors.
 *
 *  @param v1               Address of first vector.
 *  @param v2               Address of second vector.
 *  @param result           Address of vector to set with the result;
 *							may be the same address as v1 and/or v2.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Cross (
    const TQ3Vector3D             *v1,
    const TQ3Vector3D             *v2,
    TQ3Vector3D                   *result
);



/*!
 *  @function
 *      Q3Point3D_CrossProductTri
 *  @discussion
 *      Return the cross product of triangle triangle defined by three
 *		3D points, that is, of the vectors p2-p1 and p3-p2.
 *
 *  @param p1               Address of one point in the triangle.
 *  @param p2               Address of a second point in the triangle.
 *  @param p3               Address of a third point in the triangle.
 *  @param result           Address of vector to set with cross product (p2-p1) x (p3-p2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Point3D_CrossProductTri (
    const TQ3Point3D              *p1,
    const TQ3Point3D              *p2,
    const TQ3Point3D              *p3,
    TQ3Vector3D                   *result
);





//=============================================================================
//      Vector length
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Vector2D_Length
 *  @discussion
 *      Return length of 2D vector.
 *
 *  @param vector2D         Address of vector to get length of.
 *  @result                 Length of the given vector.
 */
EXTERN_API_C ( float  )
Q3Vector2D_Length (
    const TQ3Vector2D             *vector2D
);



/*!
 *  @function
 *      Q3Vector2D_LengthSquared  
 *  @discussion
 *		Return squared length of 2D vector.
 *
 *		For many operations, knowing the squared length of a vector is just
 *		as good as knowing the actual length (e.g., when sorting a set of
 *		vectors by length, or comparing a vector to a cut-off length).  But
 *		finding the squared length is much faster, since it avoids a costly
 *		square root computation.
 *
 *      Not supported by QD3D.
 *
 *  @param vector2D         Address of vector to get length of.
 *  @result                 Squared length of the given vector.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( float  )
Q3Vector2D_LengthSquared (
    const TQ3Vector2D             *vector2D
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Vector3D_Length
 *  @discussion
 *      Return length of 2D vector.
 *
 *  @param vector3D         Address of vector to get length of.
 *  @result                 Length of the given vector.
 */
EXTERN_API_C ( float  )
Q3Vector3D_Length (
    const TQ3Vector3D             *vector3D
);



/*!
 *  @function
 *      Q3Vector3D_LengthSquared 
 *  @discussion
 *		Return squared length of 2D vector.
 *
 *		For many operations, knowing the squared length of a vector is just
 *		as good as knowing the actual length (e.g., when sorting a set of
 *		vectors by length, or comparing a vector to a cut-off length).  But
 *		finding the squared length is much faster, since it avoids a costly
 *		square root computation.
 *
 *      Not supported by QD3D.
 *
 *  @param vector3D         Address of vector to get length of.
 *  @result                 Squared length of the given vector.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( float  )
Q3Vector3D_LengthSquared (
    const TQ3Vector3D             *vector3D
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      Point distance
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Point2D_Distance
 *  @discussion
 *      Return Euclidean distance between two 2D points.
 *
 *  @param p1               Address of first point of interest.
 *  @param p2               Address of second point of interest.
 *  @result                 Distance between the given points.
 */
EXTERN_API_C ( float  )
Q3Point2D_Distance (
    const TQ3Point2D              *p1,
    const TQ3Point2D              *p2
);



/*!
 *  @function
 *      Q3Point2D_DistanceSquared
 *  @discussion
 *      Return the squared Euclidean distance between two 2D points.
 *
 *  @param p1               Address of first point of interest.
 *  @param p2               Address of second point of interest.
 *  @result                 Square of the distance between the given points.
 */
EXTERN_API_C ( float  )
Q3Point2D_DistanceSquared (
    const TQ3Point2D              *p1,
    const TQ3Point2D              *p2
);



/*!
 *  @function
 *      Q3Param2D_Distance
 *  @discussion
 *      Return Euclidean distance between two UV parameter points.
 *
 *  @param p1               Address of first point of interest.
 *  @param p2               Address of second point of interest.
 *  @result                 Distance between the given points.
 */
EXTERN_API_C ( float  )
Q3Param2D_Distance (
    const TQ3Param2D              *p1,
    const TQ3Param2D              *p2
);



/*!
 *  @function
 *      Q3Param2D_DistanceSquared
 *  @discussion
 *      Return the squared Euclidean distance between two UV parameter points.
 *
 *  @param p1               Address of first point of interest.
 *  @param p2               Address of second point of interest.
 *  @result                 Square of the distance between the given points.
 */
EXTERN_API_C ( float  )
Q3Param2D_DistanceSquared (
    const TQ3Param2D              *p1,
    const TQ3Param2D              *p2
);



/*!
 *  @function
 *      Q3RationalPoint3D_Distance 
 *  @discussion
 *		Returns the Euclidian distance between two rational 3D points.
 *
 *      This operation makes no sense mathematically, but is included
 *		for backwards compatibility with QD3D.
 *
 *  @param p1               Address of first point of interest.
 *  @param p2               Address of second point of interest.
 *  @result                 Distance between the given points, treating
 *							w as a spatial coordinate.
 */
EXTERN_API_C ( float  )
Q3RationalPoint3D_Distance (
    const TQ3RationalPoint3D      *p1,
    const TQ3RationalPoint3D      *p2
);



/*!
 *  @function
 *      Q3RationalPoint3D_DistanceSquared
 *  @discussion
 *		Returns the squared Euclidian distance between two rational 3D points.
 *
 *      This operation makes no sense mathematically, but is included
 *		for backwards compatibility with QD3D.
 *
 *  @param p1               Address of first point of interest.
 *  @param p2               Address of second point of interest.
 *  @result                 Square of the distance between the given points, 
 *							treating w as a spatial coordinate.
 */
EXTERN_API_C ( float  )
Q3RationalPoint3D_DistanceSquared (
    const TQ3RationalPoint3D      *p1,
    const TQ3RationalPoint3D      *p2
);



/*!
 *  @function
 *      Q3Point3D_Distance
 *  @discussion
 *      Return Euclidean distance between two 3D points.
 *
 *  @param p1               Address of first point of interest.
 *  @param p2               Address of second point of interest.
 *  @result                 Distance between the given points.
 */
EXTERN_API_C ( float  )
Q3Point3D_Distance (
    const TQ3Point3D              *p1,
    const TQ3Point3D              *p2
);



/*!
 *  @function
 *      Q3Point3D_DistanceSquared
 *  @discussion
 *      Return the squared Euclidean distance between two 3D points.
 *
 *  @param p1               Address of first point of interest.
 *  @param p2               Address of second point of interest.
 *  @result                 Square of the distance between the given points.
 */
EXTERN_API_C ( float  )
Q3Point3D_DistanceSquared (
    const TQ3Point3D              *p1,
    const TQ3Point3D              *p2
);



/*!
 *  @function
 *      Q3RationalPoint4D_Distance
 *  @discussion
 *		Returns the Euclidian distance between two rational 4D points.
 *
 *      This operation makes no sense mathematically, but is included
 *		for backwards compatibility with QD3D.
 *
 *  @param p1               Address of first point of interest.
 *  @param p2               Address of second point of interest.
 *  @result                 Distance between the given points, treating
 *							w as a spatial coordinate.
 */
EXTERN_API_C ( float  )
Q3RationalPoint4D_Distance (
    const TQ3RationalPoint4D      *p1,
    const TQ3RationalPoint4D      *p2
);



/*!
 *  @function
 *      Q3RationalPoint4D_DistanceSquared
 *  @discussion
 *		Returns the squared Euclidian distance between two rational 4D points.
 *
 *      This operation makes no sense mathematically, but is included
 *		for backwards compatibility with QD3D.
 *
 *  @param p1               Address of first point of interest.
 *  @param p2               Address of second point of interest.
 *  @result                 Square of the distance between the given points, 
 *							treating w as a spatial coordinate.
 */
EXTERN_API_C ( float  )
Q3RationalPoint4D_DistanceSquared (
    const TQ3RationalPoint4D      *p1,
    const TQ3RationalPoint4D      *p2
);





//=============================================================================
//      Vector negation
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Vector2D_Negate
 *  @discussion
 *      Scale a 2D vector by a factor of -1.
 *
 *  @param vector2D         Address of vector to negate.
 *  @param result           Address of vector to set (may be the same as vector2D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Negate (
    const TQ3Vector2D             *vector2D,
    TQ3Vector2D                   *result
);



/*!
 *  @function
 *      Q3Vector3D_Negate
 *  @discussion
 *      Scale a 3D vector by a factor of -1.
 *
 *  @param vector3D         Address of vector to negate.
 *  @param result           Address of vector to set (may be the same as vector3D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Negate (
    const TQ3Vector3D             *vector3D,
    TQ3Vector3D                   *result
);





//=============================================================================
//      Vector scale
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Vector2D_Scale
 *  @discussion
 *      Scale a 2D vector by the given factor.
 *
 *  @param vector2D         Address of vector to scale.
 *  @param scalar           Scaling factor.
 *  @param result           Address of vector to set (may be the same as vector2D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Scale (
    const TQ3Vector2D             *vector2D,
    float                         scalar,
    TQ3Vector2D                   *result
);



/*!
 *  @function
 *      Q3Vector3D_Scale
 *  @discussion
 *      Scale a 3D vector by the given factor.
 *
 *  @param vector3D         Address of vector to scale.
 *  @param scalar           Scaling factor.
 *  @param result           Address of vector to set (may be the same as vector3D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Scale (
    const TQ3Vector3D             *vector3D,
    float                         scalar,
    TQ3Vector3D                   *result
);





//=============================================================================
//      Vector normalize
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Vector2D_Normalize
 *  @discussion
 *      Scale a 2D vector to length 1.
 *
 *      To obtain valid results, the length of vector2D must not be 0.
 *
 *  @param vector2D         Address of vector to normalize.
 *  @param result           Address of vector to set (may be the same as vector2D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Normalize (
    const TQ3Vector2D             *vector2D,
    TQ3Vector2D                   *result
);



/*!
 *  @function
 *      Q3Vector3D_Normalize
 *  @discussion
 *      Scale a 3D vector to length 1.
 *
 *      To obtain valid results, the length of vector3D must not be 0.
 *
 *  @param vector3D         Address of vector to normalize.
 *  @param result           Address of vector to set (may be the same as vector3D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Normalize (
    const TQ3Vector3D             *vector3D,
    TQ3Vector3D                   *result
);





//=============================================================================
//      Vector-vector addition/subtraction
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Vector2D_Add
 *  @discussion
 *      Add two 2D vectors.
 *
 *  @param v1               Address of first vector to add.
 *  @param v2               Address of second vector to add.
 *  @param result           Address of vector to set (may be the same as v1 and/or v2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Add (
    const TQ3Vector2D             *v1,
    const TQ3Vector2D             *v2,
    TQ3Vector2D                   *result
);



/*!
 *  @function
 *      Q3Vector3D_Add
 *  @discussion
 *      Add two 3D vectors.
 *
 *  @param v1               Address of first vector to add.
 *  @param v2               Address of second vector to add.
 *  @param result           Address of vector to set (may be the same as v1 and/or v2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Add (
    const TQ3Vector3D             *v1,
    const TQ3Vector3D             *v2,
    TQ3Vector3D                   *result
);



/*!
 *  @function
 *      Q3Vector2D_Subtract
 *  @discussion
 *      Subtract 2D vector v2 from v1.
 *
 *  @param v1               Address of first vector.
 *  @param v2               Address of vector to subtract from v1.
 *  @param result           Address of vector to set (may be the same as v1 and/or v2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Subtract (
    const TQ3Vector2D             *v1,
    const TQ3Vector2D             *v2,
    TQ3Vector2D                   *result
);



/*!
 *  @function
 *      Q3Vector3D_Subtract
 *  @discussion
 *      Subtract 3D vector v2 from v1.
 *
 *  @param v1               Address of first vector.
 *  @param v2               Address of vector to subtract from v1.
 *  @param result           Address of vector to set (may be the same as v1 and/or v2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Subtract (
    const TQ3Vector3D             *v1,
    const TQ3Vector3D             *v2,
    TQ3Vector3D                   *result
);





//=============================================================================
//      Point and Vector addition/subtraction
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Point2D_Vector2D_Add
 *  @discussion
 *      Add a 2D vector to a point.
 *
 *
 *  @param point2D          Address of a point.
 *  @param vector2D         Address of a vector to add.
 *  @param result           Address of point to set (may be the same as point2D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_Vector2D_Add (
    const TQ3Point2D              *point2D,
    const TQ3Vector2D             *vector2D,
    TQ3Point2D                    *result
);



/*!
 *  @function
 *      Q3Param2D_Vector2D_Add
 *  @discussion
 *      Add a 2D vector to a parametric (UV) point.
 *
 *  @param param2D          Address of a 2D parametric point.
 *  @param vector2D         Address of a vector to add.
 *  @param result           Address of point to set (may be the same as param2D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_Vector2D_Add (
    const TQ3Param2D              *param2D,
    const TQ3Vector2D             *vector2D,
    TQ3Param2D                    *result
);



/*!
 *  @function
 *      Q3Point3D_Vector3D_Add
 *  @discussion
 *      Add a 3D vector to a point.
 *
 *  @param point3D          Address of a point.
 *  @param vector3D         Address of a vector to add.
 *  @param result           Address of point to set (may be the same as point3D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_Vector3D_Add (
    const TQ3Point3D              *point3D,
    const TQ3Vector3D             *vector3D,
    TQ3Point3D                    *result
);



/*!
 *  @function
 *      Q3Point2D_Vector2D_Subtract
 *  @discussion
 *      Subtract a 2D vector from a point.
 *
 *  @param point2D          Address of a point.
 *  @param vector2D         Address of a vector to subtract.
 *  @param result           Address of point to set (may be the same as point2D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_Vector2D_Subtract (
    const TQ3Point2D              *point2D,
    const TQ3Vector2D             *vector2D,
    TQ3Point2D                    *result
);



/*!
 *  @function
 *      Q3Param2D_Vector2D_Subtract
 *  @discussion
 *      Subtract a 2D vector from a parametric (UV) point.
 *
 *  @param param2D          Address of a 2D parametric point.
 *  @param vector2D         Address of a vector to subtract.
 *  @param result           Address of point to set (may be the same as param2D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_Vector2D_Subtract (
    const TQ3Param2D              *param2D,
    const TQ3Vector2D             *vector2D,
    TQ3Param2D                    *result
);



/*!
 *  @function
 *      Q3Point3D_Vector3D_Subtract
 *  @discussion
 *      Subtract 3D vector from point.
 *
 *  @param point3D          Address of a point.
 *  @param vector3D         Address of a vector to subtract.
 *  @param result           Address of point to set (may be the same as point3D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_Vector3D_Subtract (
    const TQ3Point3D              *point3D,
    const TQ3Vector3D             *vector3D,
    TQ3Point3D                    *result
);





//=============================================================================
//      Point subtraction
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Point2D_Subtract
 *  @discussion
 *      Subtract the 2D point p2 from p1.
 *
 *  @param p1               Address of a point.
 *  @param p2               Address of point to subtract.
 *  @param result           Address of a vector to set with (p1-p2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Point2D_Subtract (
    const TQ3Point2D              *p1,
    const TQ3Point2D              *p2,
    TQ3Vector2D                   *result
);



/*!
 *  @function
 *      Q3Param2D_Subtract
 *  @discussion
 *      Subtract 2D parametric point p2 from p1.
 *
 *  @param p1               Address of a point.
 *  @param p2               Address of point to subtract.
 *  @param result           Address of a vector to set with (p1-p2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Param2D_Subtract (
    const TQ3Param2D              *p1,
    const TQ3Param2D              *p2,
    TQ3Vector2D                   *result
);



/*!
 *  @function
 *      Q3Point3D_Subtract
 *  @discussion
 *      Subtract 3D point p2 from p1.
 *
 *  @param p1               Address of a point.
 *  @param p2               Address of a point to subtract.
 *  @param result           Address of point to set (may be the same as p1 and/or p2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Point3D_Subtract (
    const TQ3Point3D              *p1,
    const TQ3Point3D              *p2,
    TQ3Vector3D                   *result
);





//=============================================================================
//      Point relative ratio
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Point2D_RRatio
 *  @discussion
 *      Return the point at ratio r2/(r1+r2) along the line segment from p1 to p2.
 *
 *		Put another way, this function gives you the weighted average of points
 *		p1 and p2, with the weights given by r1 and r2.  (Note that r1+r2 must
 *		be nonzero.)
 *
 *      NOTE: The QD3D docs claim that the ratio used is r1/(r1+r2), but
 *		it was found by direct experimentation that the QD3D library (1.6)
 *		in fact uses r2/(r1+r2) instead.  This is as it should be, if r1 is
 *		the weight of p1, and r2 is the weight of p2.
 *
 *		As usual, we do as QD3D does, not as the docs say.
 *
 *  @param p1               Address of one end of a line segment.
 *  @param p2               Address of the other end of a line segment.
 *  @param r1               Weight given to point p1.
 *  @param r2               Weight given to point p2.
 *  @param result           Address of point to set (may be the same as p1 and/or p2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_RRatio (
    const TQ3Point2D              *p1,
    const TQ3Point2D              *p2,
    float                         r1,
    float                         r2,
    TQ3Point2D                    *result
);



/*!
 *  @function
 *      Q3Param2D_RRatio
 *  @discussion
 *      Return the point at ratio r2/(r1+r2) along the line segment is
 *		parameter (UV) space from p1 to p2.
 *
 *		Put another way, this function gives you the weighted average of points
 *		p1 and p2, with the weights given by r1 and r2.  (Note that r1+r2 must
 *		be nonzero.)
 *
 *  @param p1               Address of one end of a line segment.
 *  @param p2               Address of the other end of a line segment.
 *  @param r1               Weight given to point p1.
 *  @param r2               Weight given to point p2.
 *  @param result           Address of point to set (may be the same as p1 and/or p2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_RRatio (
    const TQ3Param2D              *p1,
    const TQ3Param2D              *p2,
    float                         r1,
    float                         r2,
    TQ3Param2D                    *result
);



/*!
 *  @function
 *      Q3Point3D_RRatio
 *  @discussion
 *      Return the point at ratio r2/(r1+r2) along the line segment from p1 to p2.
 *
 *		Put another way, this function gives you the weighted average of points
 *		p1 and p2, with the weights given by r1 and r2.  (Note that r1+r2 must
 *		be nonzero.)
 *
 *      NOTE: The QD3D docs claim that the ratio used is r1/(r1+r2), but
 *		it was found by direct experimentation that the QD3D library (1.6)
 *		in fact uses r2/(r1+r2) instead.  This is as it should be, if r1 is
 *		the weight of p1, and r2 is the weight of p2.
 *
 *		As usual, we do as QD3D does, not as the docs say.
 *
 *  @param p1               Address of one end of a line segment.
 *  @param p2               Address of the other end of a line segment.
 *  @param r1               Weight given to point p1.
 *  @param r2               Weight given to point p2.
 *  @param result           Address of point to set (may be the same as p1 and/or p2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_RRatio (
    const TQ3Point3D              *p1,
    const TQ3Point3D              *p2,
    float                         r1,
    float                         r2,
    TQ3Point3D                    *result
);



/*!
 *  @function
 *      Q3RationalPoint4D_RRatio
 *  @discussion
 *      Return the point at ratio r2/(r1+r2) along the line segment from p1 to p2.
 *
 *		Put another way, this function gives you the weighted average of points
 *		p1 and p2, with the weights given by r1 and r2.  (Note that r1+r2 must
 *		be nonzero.)
 *
 *      NOTE: The QD3D docs claim that the ratio used is r1/(r1+r2), but
 *		it was found by direct experimentation that the QD3D library (1.6)
 *		in fact uses r2/(r1+r2) instead.  This is as it should be, if r1 is
 *		the weight of p1, and r2 is the weight of p2.
 *
 *		As usual, we do as QD3D does, not as the docs say.
 *
 *  @param p1               Address of one end of a line segment.
 *  @param p2               Address of the other end of a line segment.
 *  @param r1               Weight given to point p1.
 *  @param r2               Weight given to point p2.
 *  @param result           Address of point to set (may be the same as p1 and/or p2).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3RationalPoint4D * )
Q3RationalPoint4D_RRatio (
    const TQ3RationalPoint4D      *p1,
    const TQ3RationalPoint4D      *p2,
    float                         r1,
    float                         r2,
    TQ3RationalPoint4D            *result
);





//=============================================================================
//      Point affine combinations
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Point2D_AffineComb
 *  @discussion
 *      Return weighted combination of several 2D points.
 *
 *		Provide an array of points and a parallel array of weights, and
 *		this function will compute the weighted combination.  Note that
 *		the weights are NOT required to sum to 1, but the sum must be
 *		nonzero.
 *
 *		If you have only two points, use Q3Point2D_RRatio instead.
 *
 *  @param points2D         Array of 2D points.
 *  @param weights          Array of weights.
 *  @param numPoints        How many elements there are in each array.
 *  @param result           Address of point to set with the weighted combination.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_AffineComb (
    const TQ3Point2D              *points2D,
    const float                   *weights,
    TQ3Uns32                      numPoints,
    TQ3Point2D                    *result
);



/*!
 *  @function
 *      Q3Param2D_AffineComb
 *  @discussion
 *      Return weighted combination of several 2D parameter points.
 *
 *		Provide an array of points and a parallel array of weights, and
 *		this function will compute the weighted combination.  Note that
 *		the weights are NOT required to sum to 1, but the sum must be
 *		nonzero.
 *
 *		If you have only two points, use Q3Param2D_RRatio instead.
 *
 *  @param params2D         Array of 2D parameter points.
 *  @param weights          Array of weights.
 *  @param numPoints        How many elements there are in each array.
 *  @param result           Address of point to set with the weighted combination.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_AffineComb (
    const TQ3Param2D              *params2D,
    const float                   *weights,
    TQ3Uns32                      numPoints,
    TQ3Param2D                    *result
);



/*!
 *  @function
 *      Q3RationalPoint3D_AffineComb
 *  @discussion
 *		Compute the weighted combination of several 3D rational points.
 *
 *      This operation makes no sense mathematically, but is included
 *		for compatibility with QD3D.
 *
 *  @param rationalPoints3D Array of 3D rational points.
 *  @param weights          Array of weights.
 *  @param numPoints        How many elements there are in each array.
 *  @param result           Address of point to set with the weighted combination.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3RationalPoint3D * )
Q3RationalPoint3D_AffineComb (
    const TQ3RationalPoint3D      *rationalPoints3D,
    const float                   *weights,
    TQ3Uns32                      numPoints,
    TQ3RationalPoint3D            *result
);



/*!
 *  @function
 *      Q3Point3D_AffineComb
 *  @discussion
 *      Return weighted combination of several 3D points.
 *
 *		Provide an array of points and a parallel array of weights, and
 *		this function will compute the weighted combination.  Note that
 *		the weights are NOT required to sum to 1, but the sum must be
 *		nonzero.
 *
 *		If you have only two points, use Q3Point3D_RRatio instead.
 *
 *  @param points3D         Array of 3D points.
 *  @param weights          Array of weights.
 *  @param numPoints        How many elements there are in each array.
 *  @param result           Address of point to set with the weighted combination.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_AffineComb (
    const TQ3Point3D              *points3D,
    const float                   *weights,
    TQ3Uns32                      numPoints,
    TQ3Point3D                    *result
);



/*!
 *  @function
 *      Q3RationalPoint4D_AffineComb
 *  @discussion
 *		Compute the weighted combination of several 4D rational points.
 *
 *      This operation makes no sense mathematically, but is included
 *		for compatibility with QD3D.
 *
 *  @param rationalPoints4D Array of 4D rational points.
 *  @param weights          Array of weights.
 *  @param numPoints        How many elements there are in each array.
 *  @param result           Address of point to set with the weighted combination.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3RationalPoint4D * )
Q3RationalPoint4D_AffineComb (
    const TQ3RationalPoint4D      *rationalPoints4D,
    const float                   *weights,
    TQ3Uns32                      numPoints,
    TQ3RationalPoint4D            *result
);





//=============================================================================
//      Point and vector transformation
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Vector2D_Transform
 *  @discussion
 *      Transform a 2D vector by a 3x3 matrix.
 *
 *		Note that the translation and perspective components of the
 *		matrix is ignored (as if it were really a 2x2 matrix).
 *
 *		Contrast with E3Point2D_Transform, which does the full 3x3
 *		transformation.
 *
 *  @param vector2D         Address of a vector to transform.
 *  @param matrix3x3        Address of a 3x3 transformation matrix.
 *  @param result           Address of vector to set (may be the same as vector2D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector2D * )
Q3Vector2D_Transform (
    const TQ3Vector2D             *vector2D,
    const TQ3Matrix3x3            *matrix3x3,
    TQ3Vector2D                   *result
);



/*!
 *  @function
 *      Q3Vector3D_Transform
 *  @discussion
 *      Transform a 3D vector by a 4x4 matrix.
 *
 *		Note that the translation and perspective components of the
 *		matrix is ignored (as if it were really a 3x3 matrix).
 *
 *		Contrast with E3Point3D_Transform, which does the full 4x4
 *		transformation.
 *
 *  @param vector3D         Address of a vector to transform.
 *  @param matrix4x4        Address of a 4x4 transformation matrix.
 *  @param result           Address of vector to set (may be the same as vector3D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_Transform (
    const TQ3Vector3D             *vector3D,
    const TQ3Matrix4x4            *matrix4x4,
    TQ3Vector3D                   *result
);



/*!
 *  @function
 *      Q3Point2D_Transform
 *  @discussion
 *      Transform a 2D point by a 3x3 matrix.
 *
 *  @param point2D          Address of a point to transform.
 *  @param matrix3x3        Address of a 3x3 transformation matrix.
 *  @param result           Address of point to set (may be the same as point2D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point2D * )
Q3Point2D_Transform (
    const TQ3Point2D              *point2D,
    const TQ3Matrix3x3            *matrix3x3,
    TQ3Point2D                    *result
);



/*!
 *  @function
 *      Q3Param2D_Transform
 *  @discussion
 *      Transform a 2D parametric point by a 3x3 matrix.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point2D          Address of a point to transform.
 *  @param matrix3x3        Address of a 3x3 transformation matrix.
 *  @param result           Address of point to set (may be the same as point2D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Param2D * )
Q3Param2D_Transform (
    const TQ3Param2D              *param2D,
    const TQ3Matrix3x3            *matrix3x3,
    TQ3Param2D                    *result
);



/*!
 *  @function
 *      Q3RationalPoint3D_Transform
 *  @discussion
 *		Transform 3D rational point by 3x3 matrix.
 *
 *      Not supported by QD3D.
 *
 *  @param rationalPoint3D  Address of a point to transform.
 *  @param matrix3x3        Address of a 3x3 transformation matrix.
 *  @param result           Address of point to set (may be the same as rationalPoint3D).
 *  @result                 Convenience copy of result parameter.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3RationalPoint3D * )
Q3RationalPoint3D_Transform (
    const TQ3RationalPoint3D      *rationalPoint3D,
    const TQ3Matrix3x3            *matri3x3,
    TQ3RationalPoint3D            *result
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Point3D_Transform
 *  @discussion
 *      Transform a 3D point by a 4x4 matrix.
 *
 *  @param point3D          Address of a point to transform.
 *  @param matrix4x4        Address of a 4x4 transformation matrix.
 *  @param result           Address of point to set (may be the same as point3D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_Transform (
    const TQ3Point3D              *point3D,
    const TQ3Matrix4x4            *matrix4x4,
    TQ3Point3D                    *result
);



/*!
 *  @function
 *      Q3RationalPoint4D_Transform
 *  @discussion
 *      Transform a 4D rational point by a 4x4 matrix.
 *
 *  @param rationalPoint4D  Address of a point to transform.
 *  @param matrix4x4        Address of a 4x4 transformation matrix.
 *  @param result           Address of point to set (may be the same as rationalPoint4D).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3RationalPoint4D * )
Q3RationalPoint4D_Transform (
    const TQ3RationalPoint4D      *rationalPoint4D,
    const TQ3Matrix4x4            *matrix4x4,
    TQ3RationalPoint4D            *result
);



/*!
 *  @function
 *      Q3Vector2D_To2DTransformArray
 *  @discussion
 *      Not supported by QD3D.
 *
 *  @param inVectors2D      Description of the parameter.
 *  @param matrix3x3        Description of the parameter.
 *  @param outVectors2D     Description of the parameter.
 *  @param numVectors       Description of the parameter.
 *  @param inStructSize     Description of the parameter.
 *  @param outStructSize    Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3Vector2D_To2DTransformArray (
    const TQ3Vector2D             *inVectors2D,
    const TQ3Matrix3x3            *matrix3x3,
    TQ3Vector2D                   *outVectors2D,
    TQ3Int32                      numVectors,
    TQ3Uns32                      inStructSize,
    TQ3Uns32                      outStructSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Vector3D_To3DTransformArray
 *  @discussion
 *      Not supported by QD3D.
 *
 *  @param inVectors3D      Description of the parameter.
 *  @param matrix4x4        Description of the parameter.
 *  @param outVectors3D     Description of the parameter.
 *  @param numVectors       Description of the parameter.
 *  @param inStructSize     Description of the parameter.
 *  @param outStructSize    Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3Vector3D_To3DTransformArray (
    const TQ3Vector3D             *inVectors3D,
    const TQ3Matrix4x4            *matrix4x4,
    TQ3Vector3D                   *outVectors3D,
    TQ3Int32                      numVectors,
    TQ3Uns32                      inStructSize,
    TQ3Uns32                      outStructSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Point2D_To2DTransformArray
 *  @discussion
 *      Not supported by QD3D.
 *
 *  @param inPoints2D       Description of the parameter.
 *  @param matrix3x3        Description of the parameter.
 *  @param outPoints2D      Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param inStructSize     Description of the parameter.
 *  @param outStructSize    Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3Point2D_To2DTransformArray (
    const TQ3Point2D              *inPoints2D,
    const TQ3Matrix3x3            *matrix3x3,
    TQ3Point2D                    *outPoints2D,
    TQ3Int32                      numPoints,
    TQ3Uns32                      inStructSize,
    TQ3Uns32                      outStructSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3RationalPoint3D_To3DTransformArray
 *  @discussion
 *      Not supported by QD3D.
 *
 *  @param inRationalPoints3D Description of the parameter.
 *  @param matrix3x3        Description of the parameter.
 *  @param outRationalPoints3D Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param inStructSize     Description of the parameter.
 *  @param outStructSize    Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3RationalPoint3D_To3DTransformArray (
    const TQ3RationalPoint3D      *inRationalPoints3D,
    const TQ3Matrix3x3            *matrix3x3,
    TQ3RationalPoint3D            *outRationalPoints3D,
    TQ3Int32                      numPoints,
    TQ3Uns32                      inStructSize,
    TQ3Uns32                      outStructSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Point3D_To3DTransformArray
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param inPoints3D       Description of the parameter.
 *  @param matrix4x4        Description of the parameter.
 *  @param outPoints3D      Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param inStructSize     Description of the parameter.
 *  @param outStructSize    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point3D_To3DTransformArray (
    const TQ3Point3D              *inPoints3D,
    const TQ3Matrix4x4            *matrix4x4,
    TQ3Point3D                    *outPoints3D,
    TQ3Int32                      numPoints,
    TQ3Uns32                      inStructSize,
    TQ3Uns32                      outStructSize
);



/*!
 *  @function
 *      Q3Point3D_To4DTransformArray
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param inPoints3D       Description of the parameter.
 *  @param matrix4x4        Description of the parameter.
 *  @param outRationalPoints4D Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param inStructSize     Description of the parameter.
 *  @param outStructSize    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point3D_To4DTransformArray (
    const TQ3Point3D              *inPoints3D,
    const TQ3Matrix4x4            *matrix4x4,
    TQ3RationalPoint4D            *outRationalPoints4D,
    TQ3Int32                      numPoints,
    TQ3Uns32                      inStructSize,
    TQ3Uns32                      outStructSize
);



/*!
 *  @function
 *      Q3RationalPoint4D_To4DTransformArray
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param inRationalPoints4D Description of the parameter.
 *  @param matrix4x4        Description of the parameter.
 *  @param outRationalPoints4D Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param inStructSize     Description of the parameter.
 *  @param outStructSize    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RationalPoint4D_To4DTransformArray (
    const TQ3RationalPoint4D      *inRationalPoints4D,
    const TQ3Matrix4x4            *matrix4x4,
    TQ3RationalPoint4D            *outRationalPoints4D,
    TQ3Int32                      numPoints,
    TQ3Uns32                      inStructSize,
    TQ3Uns32                      outStructSize
);





//=============================================================================
//      Matrix functions
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Matrix3x3_SetIdentity
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix3x3        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_SetIdentity (
    TQ3Matrix3x3                  *matrix3x3
);



/*!
 *  @function
 *      Q3Matrix4x4_SetIdentity
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetIdentity (
    TQ3Matrix4x4                  *matrix4x4
);



/*!
 *  @function
 *      Q3Matrix3x3_SetTranslate
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix3x3        Description of the parameter.
 *  @param xTrans           Description of the parameter.
 *  @param yTrans           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_SetTranslate (
    TQ3Matrix3x3                  *matrix3x3,
    float                         xTrans,
    float                         yTrans
);



/*!
 *  @function
 *      Q3Matrix4x4_SetTranslate
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param xTrans           Description of the parameter.
 *  @param yTrans           Description of the parameter.
 *  @param zTrans           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetTranslate (
    TQ3Matrix4x4                  *matrix4x4,
    float                         xTrans,
    float                         yTrans,
    float                         zTrans
);



/*!
 *  @function
 *      Q3Matrix3x3_SetScale
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix3x3        Description of the parameter.
 *  @param xScale           Description of the parameter.
 *  @param yScale           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_SetScale (
    TQ3Matrix3x3                  *matrix3x3,
    float                         xScale,
    float                         yScale
);



/*!
 *  @function
 *      Q3Matrix4x4_SetScale
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param xScale           Description of the parameter.
 *  @param yScale           Description of the parameter.
 *  @param zScale           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetScale (
    TQ3Matrix4x4                  *matrix4x4,
    float                         xScale,
    float                         yScale,
    float                         zScale
);



/*!
 *  @function
 *      Q3Matrix3x3_SetRotate
 *  @discussion
 *      Not supported by QD3D.
 *
 *  @param matrix3x3        Description of the parameter.
 *  @param angle            Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_SetRotate (
    TQ3Matrix3x3                  *matrix3x3,
    float                         angle
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Matrix4x4_SetRotate_X
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param angle            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotate_X (
    TQ3Matrix4x4                  *matrix4x4,
    float                         angle
);



/*!
 *  @function
 *      Q3Matrix4x4_SetRotate_Y
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param angle            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotate_Y (
    TQ3Matrix4x4                  *matrix4x4,
    float                         angle
);



/*!
 *  @function
 *      Q3Matrix4x4_SetRotate_Z
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param angle            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotate_Z (
    TQ3Matrix4x4                  *matrix4x4,
    float                         angle
);



/*!
 *  @function
 *      Q3Matrix4x4_SetRotate_XYZ
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param xAngle           Description of the parameter.
 *  @param yAngle           Description of the parameter.
 *  @param zAngle           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotate_XYZ (
    TQ3Matrix4x4                  *matrix4x4,
    float                         xAngle,
    float                         yAngle,
    float                         zAngle
);



/*!
 *  @function
 *      Q3Matrix3x3_SetRotateAboutPoint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix3x3        Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @param angle            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_SetRotateAboutPoint (
    TQ3Matrix3x3                  *matrix3x3,
    const TQ3Point2D              *origin,
    float                         angle
);



/*!
 *  @function
 *      Q3Matrix4x4_SetRotateAboutPoint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @param xAngle           Description of the parameter.
 *  @param yAngle           Description of the parameter.
 *  @param zAngle           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotateAboutPoint (
    TQ3Matrix4x4                  *matrix4x4,
    const TQ3Point3D              *origin,
    float                         xAngle,
    float                         yAngle,
    float                         zAngle
);



/*!
 *  @function
 *      Q3Matrix4x4_SetRotateAboutAxis
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @param axis             Description of the parameter.
 *  @param angle            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotateAboutAxis (
    TQ3Matrix4x4                  *matrix4x4,
    const TQ3Point3D              *origin,
    const TQ3Vector3D             *axis,
    float                         angle
);



/*!
 *  @function
 *      Q3Matrix4x4_SetRotateVectorToVector
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param v1               Description of the parameter.
 *  @param v2               Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetRotateVectorToVector (
    TQ3Matrix4x4                  *matrix4x4,
    const TQ3Vector3D             *v1,
    const TQ3Vector3D             *v2
);



/*!
 *  @function
 *      Q3Matrix4x4_SetQuaternion
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param quaternion       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetQuaternion (
    TQ3Matrix4x4                  *matrix4x4,
    const TQ3Quaternion           *quaternion
);



/*!
 *  @function
 *      Q3Matrix3x3_Copy
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix3x3        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_Copy (
    const TQ3Matrix3x3            *matrix3x3,
    TQ3Matrix3x3                  *result
);



/*!
 *  @function
 *      Q3Matrix4x4_Copy
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_Copy (
    const TQ3Matrix4x4            *matrix4x4,
    TQ3Matrix4x4                  *result
);



/*!
 *  @function
 *      Q3Matrix3x3_Transpose
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix3x3        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_Transpose (
    const TQ3Matrix3x3            *matrix3x3,
    TQ3Matrix3x3                  *result
);



/*!
 *  @function
 *      Q3Matrix4x4_Transpose
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_Transpose (
    const TQ3Matrix4x4            *matrix4x4,
    TQ3Matrix4x4                  *result
);



/*!
 *  @function
 *      Q3Matrix3x3_Determinant
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix3x3        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( float  )
Q3Matrix3x3_Determinant (
    const TQ3Matrix3x3            *matrix3x3
);



/*!
 *  @function
 *      Q3Matrix4x4_Determinant
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( float  )
Q3Matrix4x4_Determinant (
    const TQ3Matrix4x4            *matrix4x4
);



/*!
 *  @function
 *      Q3Matrix3x3_Adjoint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix3x3        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_Adjoint (
    const TQ3Matrix3x3            *matrix3x3,
    TQ3Matrix3x3                  *result
);



/*!
 *  @function
 *      Q3Matrix3x3_Invert
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix3x3        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_Invert (
    const TQ3Matrix3x3            *matrix3x3,
    TQ3Matrix3x3                  *result
);



/*!
 *  @function
 *      Q3Matrix4x4_Invert
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_Invert (
    const TQ3Matrix4x4            *matrix4x4,
    TQ3Matrix4x4                  *result
);



/*!
 *  @function
 *      Q3Matrix3x3_Multiply
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param m1               Description of the parameter.
 *  @param m2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_Multiply (
    const TQ3Matrix3x3            *m1,
    const TQ3Matrix3x3            *m2,
    TQ3Matrix3x3                  *result
);



/*!
 *  @function
 *      Q3Matrix4x4_Multiply
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param m1               Description of the parameter.
 *  @param m2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_Multiply (
    const TQ3Matrix4x4            *m1,
    const TQ3Matrix4x4            *m2,
    TQ3Matrix4x4                  *result
);





//=============================================================================
//      Quaternion functions
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Quaternion_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param w                Description of the parameter.
 *  @param x                Description of the parameter.
 *  @param y                Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_Set (
    TQ3Quaternion                 *quaternion,
    float                         w,
    float                         x,
    float                         y,
    float                         z
);



/*!
 *  @function
 *      Q3Quaternion_SetIdentity
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetIdentity (
    TQ3Quaternion                 *quaternion
);



/*!
 *  @function
 *      Q3Quaternion_SetRotate_X
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param angle            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotate_X (
    TQ3Quaternion                 *quaternion,
    float                         angle
);



/*!
 *  @function
 *      Q3Quaternion_SetRotate_Y
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param angle            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotate_Y (
    TQ3Quaternion                 *quaternion,
    float                         angle
);



/*!
 *  @function
 *      Q3Quaternion_SetRotate_Z
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param angle            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotate_Z (
    TQ3Quaternion                 *quaternion,
    float                         angle
);



/*!
 *  @function
 *      Q3Quaternion_SetRotate_XYZ
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param xAngle           Description of the parameter.
 *  @param yAngle           Description of the parameter.
 *  @param zAngle           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotate_XYZ (
    TQ3Quaternion                 *quaternion,
    float                         xAngle,
    float                         yAngle,
    float                         zAngle
);



/*!
 *  @function
 *      Q3Quaternion_SetRotateAboutAxis
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param axis             Description of the parameter.
 *  @param angle            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotateAboutAxis (
    TQ3Quaternion                 *quaternion,
    const TQ3Vector3D             *axis,
    float                         angle
);



/*!
 *  @function
 *      Q3Quaternion_SetRotateVectorToVector
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param v1               Description of the parameter.
 *  @param v2               Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetRotateVectorToVector (
    TQ3Quaternion                 *quaternion,
    const TQ3Vector3D             *v1,
    const TQ3Vector3D             *v2
);



/*!
 *  @function
 *      Q3Quaternion_SetMatrix
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param matrix4x4        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetMatrix (
    TQ3Quaternion                 *quaternion,
    const TQ3Matrix4x4            *matrix4x4
);



/*!
 *  @function
 *      Q3Quaternion_Copy
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_Copy (
    const TQ3Quaternion           *quaternion,
    TQ3Quaternion                 *result
);



/*!
 *  @function
 *      Q3Quaternion_IsIdentity
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Quaternion_IsIdentity (
    const TQ3Quaternion           *quaternion
);



/*!
 *  @function
 *      Q3Quaternion_Dot
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param q1               Description of the parameter.
 *  @param q2               Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( float  )
Q3Quaternion_Dot (
    const TQ3Quaternion           *q1,
    const TQ3Quaternion           *q2
);



/*!
 *  @function
 *      Q3Quaternion_Normalize
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_Normalize (
    const TQ3Quaternion           *quaternion,
    TQ3Quaternion                 *result
);



/*!
 *  @function
 *      Q3Quaternion_Invert
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_Invert (
    const TQ3Quaternion           *quaternion,
    TQ3Quaternion                 *result
);



/*!
 *  @function
 *      Q3Quaternion_Multiply
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param q1               Description of the parameter.
 *  @param q2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_Multiply (
    const TQ3Quaternion           *q1,
    const TQ3Quaternion           *q2,
    TQ3Quaternion                 *result
);



/*!
 *  @function
 *      Q3Quaternion_MatchReflection
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param q1               Description of the parameter.
 *  @param q2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_MatchReflection (
    const TQ3Quaternion           *q1,
    const TQ3Quaternion           *q2,
    TQ3Quaternion                 *result
);



/*!
 *  @function
 *      Q3Quaternion_InterpolateFast
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param q1               Description of the parameter.
 *  @param q2               Description of the parameter.
 *  @param t                Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_InterpolateFast (
    const TQ3Quaternion           *q1,
    const TQ3Quaternion           *q2,
    float                         t,
    TQ3Quaternion                 *result
);



/*!
 *  @function
 *      Q3Quaternion_InterpolateLinear
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param q1               Description of the parameter.
 *  @param q2               Description of the parameter.
 *  @param t                Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_InterpolateLinear (
    const TQ3Quaternion           *q1,
    const TQ3Quaternion           *q2,
    float                         t,
    TQ3Quaternion                 *result
);



/*!
 *  @function
 *      Q3Vector3D_TransformQuaternion
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector3D         Description of the parameter.
 *  @param quaternion       Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Vector3D * )
Q3Vector3D_TransformQuaternion (
    const TQ3Vector3D             *vector3D,
    const TQ3Quaternion           *quaternion,
    TQ3Vector3D                   *result
);



/*!
 *  @function
 *      Q3Point3D_TransformQuaternion
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point3D          Description of the parameter.
 *  @param quaternion       Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Point3D * )
Q3Point3D_TransformQuaternion (
    const TQ3Point3D              *point3D,
    const TQ3Quaternion           *quaternion,
    TQ3Point3D                    *result
);





//=============================================================================
//      Bounding box functions
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3BoundingBox_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bBox             Description of the parameter.
 *  @param min              Description of the parameter.
 *  @param max              Description of the parameter.
 *  @param isEmpty          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_Set (
    TQ3BoundingBox                *bBox,
    const TQ3Point3D              *min,
    const TQ3Point3D              *max,
    TQ3Boolean                    isEmpty
);



/*!
 *  @function
 *      Q3BoundingBox_SetFromPoints3D
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bBox             Description of the parameter.
 *  @param points3D         Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param structSize       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_SetFromPoints3D (
    TQ3BoundingBox                *bBox,
    const TQ3Point3D              *points3D,
    TQ3Uns32                      numPoints,
    TQ3Uns32                      structSize
);



/*!
 *  @function
 *      Q3BoundingBox_SetFromRationalPoints4D
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bBox             Description of the parameter.
 *  @param rationalPoints4D Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param structSize       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_SetFromRationalPoints4D (
    TQ3BoundingBox                *bBox,
    const TQ3RationalPoint4D      *rationalPoints4D,
    TQ3Uns32                      numPoints,
    TQ3Uns32                      structSize
);



/*!
 *  @function
 *      Q3BoundingBox_Copy
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bBox             Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_Copy (
    const TQ3BoundingBox          *bBox,
    TQ3BoundingBox                *result
);



/*!
 *  @function
 *      Q3BoundingBox_Union
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param b1               Description of the parameter.
 *  @param b2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_Union (
    const TQ3BoundingBox          *b1,
    const TQ3BoundingBox          *b2,
    TQ3BoundingBox                *result
);



/*!
 *  @function
 *      Q3BoundingBox_UnionPoint3D
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bBox             Description of the parameter.
 *  @param point3D          Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_UnionPoint3D (
    const TQ3BoundingBox          *bBox,
    const TQ3Point3D              *point3D,
    TQ3BoundingBox                *result
);



/*!
 *  @function
 *      Q3BoundingBox_UnionRationalPoint4D
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bBox             Description of the parameter.
 *  @param rationalPoint4D  Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_UnionRationalPoint4D (
    const TQ3BoundingBox          *bBox,
    const TQ3RationalPoint4D      *rationalPoint4D,
    TQ3BoundingBox                *result
);





//=============================================================================
//      Bounding sphere functions
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3BoundingSphere_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bSphere          Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @param radius           Description of the parameter.
 *  @param isEmpty          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_Set (
    TQ3BoundingSphere             *bSphere,
    const TQ3Point3D              *origin,
    float                         radius,
    TQ3Boolean                    isEmpty
);



/*!
 *  @function
 *      Q3BoundingSphere_SetFromPoints3D
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bSphere          Description of the parameter.
 *  @param points3D         Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param structSize       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_SetFromPoints3D (
    TQ3BoundingSphere             *bSphere,
    const TQ3Point3D              *points3D,
    TQ3Uns32                      numPoints,
    TQ3Uns32                      structSize
);



/*!
 *  @function
 *      Q3BoundingSphere_SetFromRationalPoints4D
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bSphere          Description of the parameter.
 *  @param rationalPoints4D Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param structSize       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_SetFromRationalPoints4D (
    TQ3BoundingSphere             *bSphere,
    const TQ3RationalPoint4D      *rationalPoints4D,
    TQ3Uns32                      numPoints,
    TQ3Uns32                      structSize
);



/*!
 *  @function
 *      Q3BoundingSphere_Copy
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bSphere          Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_Copy (
    const TQ3BoundingSphere       *bSphere,
    TQ3BoundingSphere             *result
);



/*!
 *  @function
 *      Q3BoundingSphere_Union
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param s1               Description of the parameter.
 *  @param s2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_Union (
    const TQ3BoundingSphere       *s1,
    const TQ3BoundingSphere       *s2,
    TQ3BoundingSphere             *result
);



/*!
 *  @function
 *      Q3BoundingSphere_UnionPoint3D
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bSphere          Description of the parameter.
 *  @param point3D          Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_UnionPoint3D (
    const TQ3BoundingSphere       *bSphere,
    const TQ3Point3D              *point3D,
    TQ3BoundingSphere             *result
);



/*!
 *  @function
 *      Q3BoundingSphere_UnionRationalPoint4D
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param bSphere          Description of the parameter.
 *  @param rationalPoint4D  Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_UnionRationalPoint4D (
    const TQ3BoundingSphere       *bSphere,
    const TQ3RationalPoint4D      *rationalPoint4D,
    TQ3BoundingSphere             *result
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



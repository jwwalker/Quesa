/*! @header QuesaMath.h
        Declares the Quesa math utilities.
 */
/*  NAME:
        QuesaMath.h

    DESCRIPTION:
        Quesa public header.

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

#include <math.h>
#include <float.h>





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint on

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
 *      Available in inline form as Q3FastVector2D_Set.
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
 *      Available in inline form as Q3FastVector3D_Set.
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
 *      Available in inline form as Q3FastPoint2D_Set.
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
 *      Available in inline form as Q3FastParam2D_Set.
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
 *      Available in inline form as Q3FastRationalPoint3D_Set.
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
 *      Available in inline form as Q3FastPoint3D_Set.
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
 *      Available in inline form as Q3FastRationalPoint4D_Set.
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
 *      Available in inline form as Q3FastPolarPoint_Set.
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
 *      Available in inline form as Q3FastSphericalPoint_Set.
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
 *      Available in inline form as Q3FastVector2D_To3D.
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
 *      Available in inline form as Q3FastVector2D_ToRationalPoint3D.
 *
 *      <em>This function is not available in QD3D.</em>
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
 *      Available in inline form as Q3FastVector3D_To2D.
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
 *      Available in inline form as Q3FastRationalPoint3D_ToVector2D.
 *
 *      <em>This function is not available in QD3D.</em>
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
 *      Available in inline form as Q3FastVector3D_ToRationalPoint4D.
 *
 *      <em>This function is not available in QD3D.</em>
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
 *      Available in inline form as Q3FastRationalPoint4D_ToVector3D.
 *
 *      <em>This function is not available in QD3D.</em>
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
 *      Available in inline form as Q3FastPoint2D_To3D.
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
 *      Available in inline form as Q3FastRationalPoint3D_To2D.
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
 *      Available in inline form as Q3FastPoint3D_To4D.
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
 *      Available in inline form as Q3FastRationalPoint4D_To3D.
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
 *      Available in inline form as Q3FastPolarPoint_ToPoint2D.
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
 *      Available in inline form as Q3FastVector2D_Dot.
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
 *      Available in inline form as Q3FastVector3D_Dot.
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



/*!
 *  @function
 *      Q3Vector3D_DotArray
 *  @discussion
 *      Calculate an array of dot products.
 *
 *      Given two arrays of vectors, an array of dot products is returned along
 *      with an array of TQ3Booleans indicating which dot products are less than
 *      zero.
 *
 *      At least one of dotProducts or dotLessThanZero must be non-NULL, however
 *      one parameter may be set to NULL if that information is not required.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param numVectors       The number of vectors to process.
 *  @param firstVectors     The first set of vectors.
 *  @param secondVectors    The second set of vectors.
 *  @param dotProducts      Receives the dot products.
 *  @param dotLessThanZero  Receives the "< 0.0" status of the dot products.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3Vector3D_DotArray(
    TQ3Uns32                    numVectors,
    const TQ3Vector3D           *firstVectors,
    const TQ3Vector3D           *secondVectors,
    float                       *dotProducts,
    TQ3Boolean                  *dotLessThanZero
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





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
 *      Available in inline form as Q3FastVector2D_Cross.
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
 *      <em>This function is not available in QD3D.</em>
 *
 *      Available in inline form as Q3FastPoint2D_CrossProductTri.
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
 *      Available in inline form as Q3FastVector3D_Cross.
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
 *      Available in inline form as Q3FastPoint3D_CrossProductTri.
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



/*!
 *  @function
 *      Q3Triangle_CrossProductArray
 *  @discussion
 *      Calculate an array of triangle normals.
 *
 *      Triangles are specified as a contiguous array of triangle indices,
 *      and a contiguous array of points. The result is a contiguous array
 *      of triangle normals.
 *
 *      Triangles may be omitted from processing with the usageFlags parameter,
 *      which should point to an array of TQ3Uns8 flags. If usageFlags is not
 *      NULL, only triangles whose corresponding entry in this array is 0 will
 *      be processed.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param numTriangles     The number of triangles to process.
 *  @param usageFlags       The optional usage flags, indicating the triangles to process.
 *  @param theIndices       The triangle indices.
 *  @param thePoints        The triangle points.
 *  @param theNormals       Receives the triangle normals.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3Triangle_CrossProductArray(
    TQ3Uns32                    numTriangles,
    const TQ3Uns8               *usageFlags,
    const TQ3Uns32              *theIndices,
    const TQ3Point3D            *thePoints,
    TQ3Vector3D                 *theNormals
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      Vector length
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Vector2D_Length
 *  @discussion
 *      Return length of 2D vector.
 *
 *      Available in inline form as Q3FastVector2D_Length.
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
 *      Available in inline form as Q3FastVector2D_LengthSquared.
 *
 *      <em>This function is not available in QD3D.</em>
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
 *      Available in inline form as Q3FastVector3D_Length.
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
 *      Available in inline form as Q3FastVector3D_LengthSquared.
 *
 *      <em>This function is not available in QD3D.</em>
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
 *      Available in inline form as Q3FastPoint2D_Distance.
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
 *      Available in inline form as Q3FastPoint2D_DistanceSquared.
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
 *      Available in inline form as Q3FastParam2D_Distance.
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
 *      Available in inline form as Q3FastParam2D_DistanceSquared.
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
 *      Available in inline form as Q3FastRationalPoint3D_Distance.
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
 *      Available in inline form as Q3FastRationalPoint3D_DistanceSquared.
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
 *      Available in inline form as Q3FastPoint3D_Distance.
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
 *      Available in inline form as Q3FastPoint3D_DistanceSquared.
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
 *      Available in inline form as Q3FastRationalPoint4D_Distance.
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
 *      Available in inline form as Q3FastRationalPoint4D_DistanceSquared.
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
 *      Available in inline form as Q3FastVector2D_Negate.
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
 *      Available in inline form as Q3FastVector3D_Negate.
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
 *      Available in inline form as Q3FastVector2D_Scale.
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
 *      Available in inline form as Q3FastVector3D_Scale.
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
 *      Available in inline form as Q3FastVector2D_Normalize.
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
 *      Available in inline form as Q3FastVector3D_Normalize.
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
 *      Available in inline form as Q3FastVector2D_Add.
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
 *      Available in inline form as Q3FastVector3D_Add.
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
 *      Available in inline form as Q3FastVector2D_Subtract.
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
 *      Available in inline form as Q3FastVector3D_Subtract.
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
 *      Available in inline form as Q3FastPoint2D_Vector2D_Add.
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
 *      Available in inline form as Q3FastParam2D_Vector2D_Add.
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
 *      Available in inline form as Q3FastPoint3D_Vector3D_Add.
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
 *      Available in inline form as Q3FastPoint2D_Vector2D_Subtract.
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
 *      Available in inline form as Q3FastParam2D_Vector2D_Subtract.
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
 *      Available in inline form as Q3FastPoint3D_Vector3D_Subtract.
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
 *      Available in inline form as Q3FastPoint2D_Subtract.
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
 *      Available in inline form as Q3FastParam2D_Subtract.
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
 *      Available in inline form as Q3FastPoint3D_Subtract.
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
 *      Available in inline form as Q3FastPoint2D_RRatio.
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
 *      Available in inline form as Q3FastParam2D_RRatio.
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
 *      Available in inline form as Q3FastPoint3D_RRatio.
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
 *      Available in inline form as Q3FastRationalPoint4D_RRatio.
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
 *      <em>This function is not available in QD3D.</em>
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
 *		Transform an array of 2D vectors by a 3x3 matrix.
 *
 *		When you have many vectors to transform, this is a more efficient
 *		alternative to calling Q3Vector2D_Transform repeatedly.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param inVectors2D      Array of 2D vectors to transform.
 *  @param matrix3x3        Transformation matrix.
 *  @param outVectors2D     Array of vectors to receive output (may be the same as inVectors2D).
 *  @param numVectors       How many vectors are in each array.
 *  @param inStructSize     Size of one element of the input array, typically sizeof(TQ3Vector2D).
 *  @param outStructSize    Size of one element of the output array, typically sizeof(TQ3Vector2D).
 *  @result                 kQ3Success or some error code.
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
 *		Transform an array of 3D vectors by a 4x4 matrix.
 *
 *		When you have many vectors to transform, this is a more efficient
 *		alternative to calling Q3Vector3D_Transform repeatedly.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param inVectors3D      Array of 3D vectors to transform.
 *  @param matrix4x4        Transformation matrix.
 *  @param outVectors3D     Array of vectors to receive output (may be the same as inVectors3D).
 *  @param numVectors       How many vectors are in each array.
 *  @param inStructSize     Size of one element of the input array, typically sizeof(TQ3Vector3D).
 *  @param outStructSize    Size of one element of the output array, typically sizeof(TQ3Vector3D).
 *  @result                 kQ3Success or some error code.
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
 *		Transform an array of 2D points by a 3x3 matrix.
 *
 *		When you have many points to transform, this is a more efficient
 *		alternative to calling Q3Point2D_Transform repeatedly.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param inPoints2D       Array of 2D points to transform.
 *  @param matrix3x3        Transformation matrix.
 *  @param outPoints2D      Array of points to receive output (may be the same as inPoints2D).
 *  @param numPoints        How many points are in each array.
 *  @param inStructSize     Size of one element of the input array, typically sizeof(TQ3Point2D).
 *  @param outStructSize    Size of one element of the output array, typically sizeof(TQ3Point2D).
 *  @result                 kQ3Success or some error code.
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
 *		Transform an array of 3D rational points by a 3x3 matrix.
 *
 *		When you have many points to transform, this is a more efficient
 *		alternative to calling Q3RationalPoint3D_Transform repeatedly.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param inRationalPoints3D Array of 3D rational points to transform.
 *  @param matrix3x3        Transformation matrix.
 *  @param outRationalPoints3D Array of points to receive output (may be the same as inRationalPoints3D).
 *  @param numPoints        How many points are in each array.
 *  @param inStructSize     Size of one element of the input array, typically sizeof(TQ3RationalPoint3D).
 *  @param outStructSize    Size of one element of the output array, typically sizeof(TQ3RationalPoint3D).
 *  @result                 kQ3Success or some error code.
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
 *		Transform an array of 3D points by a 4x4 matrix.
 *
 *		When you have many points to transform, this is a more efficient
 *		alternative to calling Q3Point3D_Transform repeatedly.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param inPoints3D       Array of 3D points to transform.
 *  @param matrix4x4        Transformation matrix.
 *  @param outPoints3D      Array of points to receive output (may be the same as inPoints3D).
 *  @param numPoints        How many points are in each array.
 *  @param inStructSize     Size of one element of the input array, typically sizeof(TQ3Point3D).
 *  @param outStructSize    Size of one element of the output array, typically sizeof(TQ3Point3D).
 *  @result                 kQ3Success or some error code.
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
 *      Transform an array of 3D points by a 4x4 matrix into 4D rational points.
 *
 *  @param inPoints3D       Array of 3D points to transform.
 *  @param matrix4x4        Transformation matrix.
 *  @param outRationalPoints4D Array of points to receive output.
 *  @param numPoints        How many points are in each array.
 *  @param inStructSize     Size of one element of the input array, typically sizeof(TQ3Point3D).
 *  @param outStructSize    Size of one element of the output array, typically sizeof(TQ3RationalPoint4D).
 *  @result                 kQ3Success or some error code.
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
 *		Transform an array of 4D points by a 4x4 matrix.
 *
 *		When you have many points to transform, this is a more efficient
 *		alternative to calling Q3RationalPoint4D_Transform repeatedly.
 *
 *  @param inRationalPoints4D Array of 4D points to transform.
 *  @param matrix4x4        Transformation matrix.
 *  @param outRationalPoints4D Array of points to receive output (may be the same as inRationalPoints4D).
 *  @param numPoints        How many points are in each array.
 *  @param inStructSize     Size of one element of the input array, typically sizeof(TQ3RationalPoint4D).
 *  @param outStructSize    Size of one element of the output array, typically sizeof(TQ3RationalPoint4D).
 *  @result                 kQ3Success or some error code.
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
 *      Set a 3x3 matrix to the identity matrix.
 *
 *  @param matrix3x3        Address of matrix to set.
 *  @result                 Convenience copy of matrix3x3 parameter.
 */
EXTERN_API_C ( TQ3Matrix3x3 * )
Q3Matrix3x3_SetIdentity (
    TQ3Matrix3x3                  *matrix3x3
);



/*!
 *  @function
 *      Q3Matrix4x4_SetIdentity
 *  @discussion
 *      Set a 4x4 matrix to the identity matrix.
 *
 *  @param matrix4x4        Address of matrix to set.
 *  @result                 Convenience copy of matrix4x4 parameter.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Matrix4x4_SetIdentity (
    TQ3Matrix4x4                  *matrix4x4
);



/*!
 *  @function
 *      Q3Matrix3x3_SetTranslate
 *  @discussion
 *      Set a 3x3 matrix to translate in x, y.
 *
 *  @param matrix3x3        Address of matrix to set.
 *  @param xTrans           Amount to translate in x.
 *  @param yTrans           Amount to translate in y.
 *  @result                 Convenience copy of matrix3x3 parameter.
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
 *      Set a 4x4 matrix to translate in x, y, z.
 *
 *  @param matrix4x4        Address of matrix to set.
 *  @param xTrans           Amount to translate in x.
 *  @param yTrans           Amount to translate in y.
 *  @param zTrans           Amount to translate in z.
 *  @result                 Convenience copy of matrix4x4 parameter.
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
 *      Set 3x3 matrix to scale in x, y.
 *
 *  @param matrix3x3        Address of matrix to set.
 *  @param xScale           Amount to scale in x.
 *  @param yScale           Amount to scale in y.
 *  @result                 Convenience copy of matrix3x3 parameter.
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
 *      Set a 4x4 matrix to scale in x, y, z.
 *
 *  @param matrix4x4        Address of matrix to set.
 *  @param xScale           Amount to scale in x.
 *  @param yScale           Amount to scale in y.
 *  @param zScale           Amount to scale in z.
 *  @result                 Convenience copy of matrix4x4 parameter.
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
 *		Set a 3x3 matrix to rotate about the origin.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param matrix3x3        Address of matrix to set.
 *  @param angle            Angle to rotate (in radians).
 *  @result                 Convenience copy of matrix3x3 parameter.
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
 *      Set a 4x4 matrix to rotate about the X axis.
 *
 *  @param matrix4x4        Address of matrix to set.
 *  @param angle            Angle to rotate (in radians).
 *  @result                 Convenience copy of matrix4x4 parameter.
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
 *      Set a 4x4 matrix to rotate about the Y axis.
 *
 *  @param matrix4x4        Address of matrix to set.
 *  @param angle            Angle to rotate (in radians).
 *  @result                 Convenience copy of matrix4x4 parameter.
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
 *      Set a 4x4 matrix to rotate about the Z axis.
 *
 *  @param matrix4x4        Address of matrix to set.
 *  @param angle            Angle to rotate (in radians).
 *  @result                 Convenience copy of matrix4x4 parameter.
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
 *      Set a 4x4 matrix to rotate about the X, Y, Z axes (in that order).
 *
 *		This order of rotations is rarely useful, but it's kept for backwards
 *		compatibility with QD3D.
 *
 *  @param matrix4x4        Address of matrix to set.
 *  @param xAngle           Angle to rotate about the X axis (in radians).
 *  @param yAngle           Angle to rotate about the Y axis (in radians).
 *  @param zAngle           Angle to rotate about the Z axis (in radians).
 *  @result                 Convenience copy of matrix4x4 parameter.
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
 *      Set a 3x3 matrix to rotate about a point.
 *
 *		This is equivalent to translating the point to the origin,
 *		doing a rotation about the origin, and translating back.
 *
 *  @param matrix3x3        Address of matrix to set.
 *  @param origin           Address of a 2D point about which to rotate.
 *  @param angle            Angle to rotate (in radians).
 *  @result                 Convenience copy of matrix3x3 parameter.
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
 *      Set a 4x4 matrix to rotate about axes through apoint and 
 *		parallel to the X, Y, and Z axes (in that order).
 *
 *		This order of rotations is rarely useful, but it's kept for backwards
 *		compatibility with QD3D.
 *
 *  @param matrix4x4        Address of matrix to set.
 *  @param origin           Address of a 3D point about which to rotate.
 *  @param xAngle           Angle to rotate about the translated X axis (in radians).
 *  @param yAngle           Angle to rotate about the translated Y axis (in radians).
 *  @param zAngle           Angle to rotate about the translated Z axis (in radians).
 *  @result                 Convenience copy of matrix4x4 parameter.
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
 *      Set 4x4 matrix to rotate about an arbitrary origin and axis.
 *
 *		Note that for correct results, the axis should be normalized
 *		(i.e. have length = 1).
 *
 *  @param matrix4x4        Address of matrix to set.
 *  @param origin           Address of a 2D point about which to rotate.
 *  @param axis             Address of a 3D vector to use as the rotation axis.
 *  @param angle            Angle to rotate (in radians).
 *  @result                 Convenience copy of matrix4x4 parameter.
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
 *      Set a 4x4 matrix to rotate vector 'v1' to 'v2'.
 *
 *		Note that for correct results, both vectors should be normalized
 *		(i.e. have length = 1).
 *
 *  @param matrix4x4        Address of matrix to set.
 *  @param v1               Address of "starting" vector.
 *  @param v2               Address of "ending" vector.
 *  @result                 Convenience copy of matrix4x4 parameter.
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
 *      Set a 4x4 matrix from to the rotation represented by a quaternion.
 *
 *  @param matrix4x4        Address of matrix to set.
 *  @param quaternion       Address of the quaternion to imitate.
 *  @result                 Convenience copy of matrix4x4 parameter.
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
 *      Copy a 3x3 matrix.
 *
 *  @param matrix3x3        Address of source matrix.
 *  @param result           Address of destination matrix (may be the same as matrix3x3).
 *  @result                 Convenience copy of result parameter.
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
 *      Copy a 4x4 matrix.
 *
 *  @param matrix4x4        Address of source matrix.
 *  @param result           Address of destination matrix (may be the same as matrix4x4).
 *  @result                 Convenience copy of result parameter.
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
 *      Transpose a 3x3 matrix.
 *
 *  @param matrix3x3        Address of a matrix to transpose.
 *  @param result           Address of matrix to set (may be the same as matrix3x3).
 *  @result                 Convenience copy of result parameter.
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
 *      Transpose a 4x4 matrix.
 *
 *  @param matrix4x4        Address of a matrix to transpose.
 *  @param result           Address of matrix to set (may be the same as matrix4x4).
 *  @result                 Convenience copy of result parameter.
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
 *      Return the determinant of 3x3 matrix.
 *
 *  @param matrix3x3        Address of a matrix.
 *  @result                 Determinant of that matrix.
 */
EXTERN_API_C ( float  )
Q3Matrix3x3_Determinant (
    const TQ3Matrix3x3            *matrix3x3
);



/*!
 *  @function
 *      Q3Matrix4x4_Determinant
 *  @discussion
 *      Return the determinant of 4x4 matrix.
 *
 *  @param matrix4x4        Address of a matrix.
 *  @result                 Determinant of that matrix.
 */
EXTERN_API_C ( float  )
Q3Matrix4x4_Determinant (
    const TQ3Matrix4x4            *matrix4x4
);



/*!
 *  @function
 *      Q3Matrix3x3_Adjoint
 *  @discussion
 *      Calculate adjoint of 3x3 matrix.
 *
 *		The adjoint of a matrix is a scalar multiple of the inverse of
 *		the matrix. For some applications, the adjoint can be used in
 *		place of the inverse. In particular:
 *
 *			adjoint(A) = determinant(A) * inverse(A)
 *
 *  @param matrix3x3        Address of a matrix to calculate the adjoint of.
 *  @param result           Address of matrix to set (may be the same as matrix3x3).
 *  @result                 Convenience copy of result parameter.
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
 *      Calculate the inverse of a 3x3 non-singular matrix.
 *
 *  @param matrix3x3        Address of non-singular matrix to invert.
 *  @param result           Address of matrix to set (may be the same as matrix3x3).
 *  @result                 Convenience copy of result parameter.
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
 *      Calculate the inverse of a 4x4 non-singular matrix.
 *
 *  @param matrix4x4        Address of non-singular matrix to invert.
 *  @param result           Address of matrix to set (may be the same as matrix4x4).
 *  @result                 Convenience copy of result parameter.
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
 *      Multiply two 3x3 matrices.
 *
 *  @param m1               Address of first matrix.
 *  @param m2               Address of second matrix.
 *  @param result           Address of matrix to set with m1*m2 (may be the same as m1 and/or m2).
 *  @result                 Convenience copy of result parameter.
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
 *      Multiply two 4x4 matrices.
 *
 *  @param m1               Address of first matrix.
 *  @param m2               Address of second matrix.
 *  @param result           Address of matrix to set with m1*m2 (may be the same as m1 and/or m2).
 *  @result                 Convenience copy of result parameter.
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
 *      Set a quaternion with its individual w, x, y, and z components.
 *
 *      Available in inline form as Q3FastQuaternion_Set.
 *
 *  @param quaternion       Address of a quaternion to set.
 *  @param w                Value for w component.
 *  @param x                Value for x component.
 *  @param y                Value for y component.
 *  @param z                Value for z component.
 *  @result                 Convenience copy of quaternion parameter.
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
 *      Set a quaternion to the identity value (1,0,0,0).
 *
 *      Available in inline form as Q3FastQuaternion_SetIdentity.
 *
 *  @param quaternion       Address of a quaternion to set.
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3Quaternion * )
Q3Quaternion_SetIdentity (
    TQ3Quaternion                 *quaternion
);



/*!
 *  @function
 *      Q3Quaternion_SetRotate_X
 *  @discussion
 *      Set a quaternion to rotate about the X axis.
 *
 *  @param quaternion       Address of a quaternion to set.
 *  @param angle            Angle to rotate (in radians).
 *  @result                 Convenience copy of result parameter.
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
 *      Set a quaternion to rotate about the Y axis.
 *
 *  @param quaternion       Address of a quaternion to set.
 *  @param angle            Angle to rotate (in radians).
 *  @result                 Convenience copy of quaternion parameter.
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
 *      Set a quaternion to rotate about the Z axis.
 *
 *  @param quaternion       Address of a quaternion to set.
 *  @param angle            Angle to rotate (in radians).
 *  @result                 Convenience copy of quaternion parameter.
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
 *      Set a quaternion to rotate about the X, Y, and Z axes (in that order).
 *
 *		This order of rotations is rarely useful, but it's kept for backwards
 *		compatibility with QD3D.
 *
 *  @param quaternion       Address of a quaternion to set.
 *  @param xAngle           Angle to rotate about the X axis (in radians).
 *  @param yAngle           Angle to rotate about the Y axis (in radians).
 *  @param zAngle           Angle to rotate about the Z axis (in radians).
 *  @result                 Convenience copy of quaternion parameter.
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
 *      Set quaternion to rotate about arbitrary axis.
 *
 *		Note that for correct results, the axis should be normalized
 *		(i.e. have length = 1).
 *
 *  @param quaternion       Address of a quaternion to set.
 *  @param axis             Address of a 3D vector to use as the rotation axis.
 *  @param angle            Angle to rotate (in radians).
 *  @result                 Convenience copy of quaternion parameter.
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
 *      Set a quaternion to rotate vector 'v1' to 'v2'.
 *
 *		Note that for correct results, both vectors should be normalized
 *		(i.e. have length = 1).
 *
 *  @param quaternion       Address of a quaternion to set.
 *  @param v1               Address of "starting" vector.
 *  @param v2               Address of "ending" vector.
 *  @result                 Convenience copy of quaternion parameter.
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
 *      Set a quaternion from a 4x4 rotation matrix.
 *
 *		Note: The QD3D implementation of this function appears to be buggy.
 *		This can be demonstrated by starting with an arbitrary
 *		quaternion, converting to a matrix, then converting back (with
 *		this function).
 *
 *		QD3D's result is something ridiculous; in Quesa, this function 
 *		returns the original quaternion (or something equivalent).
 *
 *  @param quaternion       Address of a quaternion to set.
 *  @param matrix4x4        Address of a rotation matrix to imitate.
 *  @result                 Convenience copy of quaternion parameter.
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
 *      Copy a quaternion.
 *
 *      Available in inline form as Q3FastQuaternion_Copy.
 *
 *  @param quaternion       Address of source quaternion.
 *  @param result           Address of destination quaternion (may be the same as the first parameter).
 *  @result                 Convenience copy of result parameter.
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
 *      Return whether a quaternion is (roughly) the identity,
 *		i.e., (1,0,0,0).
 *
 *		Values for x, y, and z are considered close enough to 0
 *		if they are within FLT_EPSILON (a small number).
 *
 *		For correct results, the quaternion should first be normalized.
 *
 *  @param quaternion       Address of quaternion to test.
 *  @result                 True if quaternion is the identity.
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Quaternion_IsIdentity (
    const TQ3Quaternion           *quaternion
);



/*!
 *  @function
 *      Q3Quaternion_Dot
 *  @discussion
 *      Return the dot product of q1 and q2.
 *
 *      Available in inline form as Q3FastQuaternion_Dot.
 *
 *  @param q1               Address of one quaternion.
 *  @param q2               Address of another quaternion (may be the same as q1).
 *  @result                 Dot product of q1 and q2.
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
 *      Scale a quaternion to length 1.
 *
 *		This is often needed when combining or interpolating between
 *		quaternions, to keep accumulated error from causing your
 *		quaternion values to "blow up".
 *
 *      Available in inline form as Q3FastQuaternion_Normalize.
 *
 *  @param quaternion       Address of a quaternion to normalize.
 *  @param result           Address of quaternion to set (may be the same as the first parameter).
 *  @result                 Convenience copy of result parameter.
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
 *      Invert a quaternion.
 *
 *		For correct results, the quaternion should be normalized
 *		before inverting.
 *
 *      Available in inline form as Q3FastQuaternion_Invert.
 *
 *  @param quaternion       Address of a quaternion to invert.
 *  @param result           Address of quaternion to set (may be the same as the first parameter).
 *  @result                 Convenience copy of result parameter.
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
 *      Compute the product of two quaternions.
 *
 *		This is a very useful operation, since the rotation represented
 *		by q1*q2 is exactly the same as rotating by q1 and then by q2.
 *
 *  @param q1               Address of first quaternion.
 *  @param q2               Address of second quaternion.
 *  @param result           Address of quaternion to set (may be the same as q1 and/or q2).
 *  @result                 Convenience copy of result parameter.
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
 *      Set result to either q1 or -q1, whichever produces a positive dot
 *		product with q2 (i.e., whichever is "closer" to q2 in orientation).
 *
 *  @param q1               Address of source quaternion.
 *  @param q2               Address of quaternion to match.
 *  @param result           Address of quaternion to set (may be the same as q1 and/or q2).
 *  @result                 Convenience copy of result parameter.
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
 *      Compute a straight linear interpolation between two quaternions.
 *
 *		This does a true linear, not spherical, interpolation between
 *		q1 and q2.  It's fast, but not very proper for most uses.
 *
 *		The result is automatically normalized, so there is no need to
 *		do so yourself.
 *
 *  @param q1               Address of first quaternion.
 *  @param q2               Address of second quaternion.
 *  @param t                Fraction (0-1) of the way from q1 to q2.
 *  @param result           Address of quaternion to set (may be the same as q1 and/or q2).
 *  @result                 Convenience copy of result parameter.
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
 *      Compute a spherical linear interpolation between two quaternions.
 *
 *		Despite the name, this function does a SLERP (spherical linear
 *		interpolation) from q1 to q2.
 *		It falls back on a straight linear interpolation only when the
 *		cosine of the angle between them is less than 0.01.
 *		
 *		The cut-off point was chosen arbitrarily, and may not match
 *		that of QD3D.
 *
 *  @param q1               Address of first quaternion.
 *  @param q2               Address of second quaternion.
 *  @param t                Fraction (0-1) of the way from q1 to q2.
 *  @param result           Address of quaternion to set (may be the same as q1 and/or q2).
 *  @result                 Convenience copy of result parameter.
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
 *      Q3Quaternion_GetAxisAndAngle
 *  @discussion
 *      Get the rotation axis and/or angle represented by a quaternion.
 *
 *      Note that for correct results, the quaternion should be normalized.
 *
 *		If the quaternion represents a null rotation, then outAngle will be
 *		set to 0.0 and outAxis will be set to <0, 1, 0> (since in this case
 *		the rotation axis is undefined, but we want to always give you a 
 *		valid axis).
 *
 *		Either outAxis or outAngle may be null if you are not interested in
 *		that result.  (You could even pass null for both, but that would be
 *		rather pointless.)
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param quaternion       Address of a quaternion to inspect.
 *  @param outAxis          Address of a vector to set to the rotation axis (may be null).
 *  @param outAngle         Address of a float to set to the rotation angle (may be null).
 *  @result                 Convenience copy of outAxis parameter.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Vector3D * )
Q3Quaternion_GetAxisAndAngle (
	const TQ3Quaternion           *quaternion,
	TQ3Vector3D                   *outAxis,
	float                         *outAngle
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS


/*!
 *  @function
 *      Q3Vector3D_TransformQuaternion
 *  @discussion
 *      Transform a 3D vector by a quaternion.
 *
 *      Note that for correct results, the quaternion should be normalized.
 *
 *  @param vector3D         Address of a vector to transform.
 *  @param quaternion       Address of a quaternion to transform by.
 *  @param result           Address of a vector to set (may be the same as vector3D).
 *  @result                 Convenience copy of result parameter.
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
 *      Transform a 3D point by a quaternion.
 *
 *      Note that for correct results, the quaternion should be normalized.
 *
 *  @param point3D          Address of a point to transform.
 *  @param quaternion       Address of a quaternion to transform by.
 *  @param result           Address of a point to set (may be the same as point3D).
 *  @result                 Convenience copy of result parameter.
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
 *      Q3BoundingBox_Reset
 *  @discussion
 *      Reset (set to empty) a bounding box.
 *
 *      Available in inline form as Q3FastBoundingBox_Reset.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param bBox             Address of bounding box to reset.
 *  @result                 Convenience copy of bBox parameter.
 */
EXTERN_API_C ( TQ3BoundingBox * )
Q3BoundingBox_Reset (
    TQ3BoundingBox                *bBox
);



/*!
 *  @function
 *      Q3BoundingBox_Set
 *  @discussion
 *      Set a bounding box.
 *
 *      Available in inline form as Q3FastBoundingBox_Set.
 *
 *  @param bBox             Address of bounding box to set.
 *  @param min              Address of point indicating minimum X, Y, and Z.
 *  @param max              Address of point indicating maximum X, Y, and Z.
 *  @param isEmpty          True if the bounding box is empty, false otherwise.
 *  @result                 Convenience copy of bBox parameter.
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
 *      Set a bounding box to just enclose a set of 3D points.
 *
 *  @param bBox             Address of bounding box to set.
 *  @param points3D         Array of 3D points.
 *  @param numPoints        How many points are in the array.
 *  @param structSize       Size of each array element, typically sizeof(TQ3Point3D).
 *  @result                 Convenience copy of bBox parameter.
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
 *      Set a bounding box to just enclose a set of 4D rational points.
 *
 *  @param bBox             Address of bounding box to set.
 *  @param rationalPoints4D Array of 4D rational points.
 *  @param numPoints        How many points are in the array.
 *  @param structSize       Size of each array element, typically sizeof(TQ3RationalPoint4D).
 *  @result                 Convenience copy of bBox parameter.
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
 *      Copy a bounding box.
 *
 *      Available in inline form as Q3BFastoundingBox_Copy.
 *
 *  @param bBox             Address of source bounding box.
 *  @param result           Address of bounding box to set (may be the same as bBox).
 *  @result                 Convenience copy of result parameter.
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
 *      Compute the minimum bounding box that encloses both 'b1' and 'b2'.
 *
 *  @param b1               Address of one bounding box.
 *  @param b2               Address of another bounding box.
 *  @param result           Address of bounding box to set (may be the same as b1 and/or b2).
 *  @result                 Convenience copy of result parameter.
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
 *      Return the minimum bounding box that encloses both 'bBox' and 'point3D'.
 *
 *  @param bBox             Address of initial bounding box.
 *  @param point3D          Address of a point to enclose.
 *  @param result           Address of bounding box to set (may be the same as bBox).
 *  @result                 Convenience copy of result parameter.
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
 *      Return the minimum bounding box that encloses both 'bBox' and 'rationalPoint4D'.
 *
 *  @param bBox             Address of initial bounding box.
 *  @param rationalPoint4D  Address of a point to enclose.
 *  @param result           Address of bounding box to set (may be the same as bBox).
 *  @result                 Convenience copy of result parameter.
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
 *      Q3BoundingSphere_Reset
 *  @discussion
 *      Reset (set to empty) a bounding sphere.
 *
 *      Available in inline form as Q3FastBoundingSphere_Reset.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param bSphere          Address of bounding sphere to reset.
 *  @result                 Convenience copy of bSphere parameter.
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_Reset (
    TQ3BoundingSphere             *bSphere
);



/*!
 *  @function
 *      Q3BoundingSphere_Set
 *  @discussion
 *      Set a bounding sphere.
 *
 *      Available in inline form as Q3FastBoundingSphere_Set.
 *
 *  @param bSphere          Address of bounding sphere to set.
 *  @param origin           Address of point indicating sphere origin.
 *  @param radius           Sphere radius.
 *  @param isEmpty          True if the bounding sphere is empty, false otherwise.
 *  @result                 Convenience copy of bSphere parameter.
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
 *      Set a bounding sphere to just enclose a set of 3D points.
 *
 *  @param bSphere          Description of the parameter.
 *  @param points3D         Array of 3D points.
 *  @param numPoints        How many points are in the array.
 *  @param structSize       Size of each array element, typically sizeof(TQ3Point3D).
 *  @result                 Convenience copy of bSphere parameter.
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
 *      Set a bounding sphere to just enclose a set of 4D rational points.
 *
 *  @param bSphere          Address of bounding sphere to set.
 *  @param rationalPoints4D Array of 4D rational points.
 *  @param numPoints        How many points are in the array.
 *  @param structSize       Size of each array element, typically sizeof(TQ3RationalPoint4D).
 *  @result                 Convenience copy of bSphere parameter.
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
 *      Copy a bounding sphere.
 *
 *      Available in inline form as Q3FastBoundingSphere_Copy.
 *
 *  @param bSphere          Address of source bounding sphere.
 *  @param result           Address of bounding sphere to set (may be the same as bSphere).
 *  @result                 Convenience copy of result parameter.
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
 *      Compute the minimum bounding sphere that encloses both 's1' and 's2'.
 *
 *  @param s1               Address of one bounding sphere.
 *  @param s2               Address of another bounding sphere.
 *  @param result           Address of bounding sphere to set (may be the same as s1 and/or s2).
 *  @result                 Convenience copy of result parameter.
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
 *      Return the minimum bounding sphere that encloses both 'bSphere' and 'point3D'.
 *
 *  @param bSphere          Address of initial bounding sphere.
 *  @param point3D          Address of a point to enclose.
 *  @param result           Address of bounding sphere to set (may be the same as bSphere).
 *  @result                 Convenience copy of result parameter.
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
 *      Return the minimum bounding sphere that encloses both 'bSphere' and 'rationalPoint4D'.
 *
 *  @param bSphere          Address of initial bounding sphere.
 *  @param rationalPoint4D  Address of a point to enclose.
 *  @param result           Address of bounding sphere to set (may be the same as bSphere).
 *  @result                 Convenience copy of result parameter.
 */
EXTERN_API_C ( TQ3BoundingSphere * )
Q3BoundingSphere_UnionRationalPoint4D (
    const TQ3BoundingSphere       *bSphere,
    const TQ3RationalPoint4D      *rationalPoint4D,
    TQ3BoundingSphere             *result
);





//=============================================================================
//      Intersection functions
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Ray3D_IntersectSphere
 *  @discussion
 *      Test a ray for intersection against a sphere, and return the point
 *		of intersection if found.
 *
 *      The direction vector of the ray must be normalised.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theRay           The ray to test.
 *  @param theSphere        The sphere to test against.
 *  @param hitPoint         Receives the intersection point, if found.
 *  @result                 Indicates if the ray intersects the sphere.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Boolean  )
Q3Ray3D_IntersectSphere (
    const TQ3Ray3D                *theRay,
    const TQ3Sphere               *theSphere,
    TQ3Point3D                    *hitPoint
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Ray3D_IntersectBoundingBox
 *  @discussion
 *      Test a ray for intersection against a bounding box. If an
 *      intersection occurs, the point of intersection is returned.
 *
 *      The direction vector of the ray must be normalised.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theRay           The ray to test.
 *  @param theBounds        The bounding box to test against.
 *  @param hitPoint         Receives the intersection point, if found.
 *  @result                 Indicates if the ray intersects the bounding box.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Boolean  )
Q3Ray3D_IntersectBoundingBox (
    const TQ3Ray3D                *theRay,
    const TQ3BoundingBox          *theBounds,
    TQ3Point3D                    *hitPoint
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Ray3D_IntersectTriangle
 *  @discussion
 *      Test a ray for intersection against a triangle formed by three
 *      points. If an intersection occurs, returns the barycentric
 *      coordinates of the point of intersection and the distance along
 *      the ray.
 *
 *      Triangles may optionally be subject to backface culling, in
 *      which case a hit on the reverse side of the triangle will fail
 *      to result in an intersection.
 *
 *      Barycentric coordinates can be used to interpolate the triangle
 *      vertices to obtain the exact point of intersection, like so:
 *
 *        t = (1.0f - hitPoint.u - hitPoint.v);
 *        x = (point1.x * t) + (point2.x * hitPoint.u) + (point3.x * hitPoint.v);
 *        y = (point1.y * t) + (point2.y * hitPoint.u) + (point3.y * hitPoint.v);
 *        z = (point1.z * t) + (point2.z * hitPoint.u) + (point3.z * hitPoint.v);
 *
 *      Similar calculations can be made for vertex normals, UVs, or any
 *      other vertex attribute. The w component of hitPoint is set to the
 *      distance along the ray at which the intersection occurs.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theRay           The ray to test.
 *  @param point1           The first triangle vertex.
 *  @param point2           The second triangle vertex.
 *  @param point3           The third triangle vertex.
 *  @param cullBackfacing   Controls if back-facing triangles should be skipped.
 *  @param hitPoint         Receives the barycentric coordinates of the intersection, and the distance along the ray.
 *  @result                 Indicates if the ray intersects the triangle.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Boolean  )
Q3Ray3D_IntersectTriangle (
    const TQ3Ray3D                *theRay,
    const TQ3Point3D              *point1,
    const TQ3Point3D              *point2,
    const TQ3Point3D              *point3,
    TQ3Boolean                    cullBackfacing,
    TQ3Param3D                    *hitPoint
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      General math functions
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Math_SquareRoot
 *  @discussion
 *      Obtain a fast, but possibly inaccurate, square root.
 *
 *      The available precision depends on the current architecture, but will
 *      suffice for most non-accumulating 3D operations. If a reliable degree
 *      of precision is required, sqrt() should be used instead.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param x                The number whose square root should be returned.
 *  @result                 Approximate square root of x.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( float  )
Q3Math_SquareRoot (
    float                         x
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Math_InvSquareRoot
 *  @discussion
 *      Obtain a fast, but possibly inaccurate, inverse square root.
 *
 *      The available precision depends on the current architecture, but will
 *      suffice for most non-accumulating 3D operations. If a reliable degree
 *      of precision is required, 1.0/sqrt() should be used instead.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param x                The number whose inverse square root should be returned.
 *  @result                 Approximate inverse square root of x.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( float  )
Q3Math_InvSquareRoot (
    float                         x
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      Inline APIs
//-----------------------------------------------------------------------------
//		Note : Preserve the order of functions when adding new inline APIs.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS

// Implementation
#define __Q3FastVector2D_Set(_v, _x, _y)									\
	do																		\
		{																	\
		(_v)->x = (_x);														\
		(_v)->y = (_y);														\
		}																	\
	while (0)

#define __Q3FastVector3D_Set(_v, _x, _y, _z)								\
	do																		\
		{																	\
		(_v)->x = (_x);														\
		(_v)->y = (_y);														\
		(_v)->z = (_z);														\
		}																	\
	while (0)

#define __Q3FastPoint2D_Set(_p, _x, _y)										\
	do																		\
		{																	\
		(_p)->x = (_x);														\
		(_p)->y = (_y);														\
		}																	\
	while (0)

#define __Q3FastParam2D_Set(_p, _u, _v)										\
	do																		\
		{																	\
		(_p)->u = (_u);														\
		(_p)->v = (_v);														\
		}																	\
	while (0)

#define __Q3FastRationalPoint3D_Set(_p, _x, _y, _w)							\
	do																		\
		{																	\
		(_p)->x = (_x);														\
		(_p)->y = (_y);														\
		(_p)->w = (_w);														\
		}																	\
	while (0)

#define __Q3FastPoint3D_Set(_p, _x, _y, _z)									\
	do																		\
		{																	\
		(_p)->x = (_x);														\
		(_p)->y = (_y);														\
		(_p)->z = (_z);														\
		}																	\
	while (0)

#define __Q3FastRationalPoint4D_Set(_p, _x, _y, _z, _w)						\
	do																		\
		{																	\
		(_p)->x = (_x);														\
		(_p)->y = (_y);														\
		(_p)->z = (_z);														\
		(_p)->w = (_w);														\
		}																	\
	while (0)

#define __Q3FastPolarPoint_Set(_p, _r, _theta)								\
	do																		\
		{																	\
		(_p)->r     = (_r);													\
		(_p)->theta = (_theta);												\
		}																	\
	while (0)

#define __Q3FastSphericalPoint_Set(_p, _rho, _theta, _phi)					\
	do																		\
		{																	\
		(_p)->rho   = (_rho);												\
		(_p)->theta = (_theta);												\
		(_p)->phi   = (_phi);												\
		}																	\
	while (0)

#define __Q3FastVector2D_To3D(_v1, _v2)										\
	do																		\
		{																	\
		(_v2)->x = (_v1)->x;												\
		(_v2)->y = (_v1)->y;												\
		(_v2)->z = 1.0f;													\
		}																	\
	while (0)


#define __Q3FastVector2D_ToRationalPoint3D(_v, _p)							\
	do																		\
		{																	\
		(_p)->x = (_v)->x;													\
		(_p)->y = (_v)->y;													\
		(_p)->w = 0.0f;														\
		}																	\
	while (0)

#define __Q3FastVector3D_To2D(_v1, _v2)										\
	do																		\
		{																	\
		float invZ = 1.0f / (_v1)->z;										\
		(_v2)->x = (_v1)->x * invZ;											\
		(_v2)->y = (_v1)->y * invZ;											\
		}																	\
	while (0)

#define __Q3FastRationalPoint3D_ToVector2D(_p, _v)							\
	do																		\
		{																	\
		(_v)->x = (_p)->x;													\
		(_v)->y = (_p)->y;													\
		}																	\
	while (0)
	
#define __Q3FastVector3D_ToRationalPoint4D(_v, _p)							\
	do																		\
		{																	\
		(_p)->x = (_v)->x;													\
		(_p)->y = (_v)->y;													\
		(_p)->z = (_v)->z;													\
		(_p)->w = 0.0f;														\
		}																	\
	while (0)

#define __Q3FastRationalPoint4D_ToVector3D(_p, _v)							\
	do																		\
		{																	\
		(_v)->x = (_p)->x;													\
		(_v)->y = (_p)->y;													\
		(_v)->z = (_p)->z;													\
		}																	\
	while (0)

#define __Q3FastPoint2D_To3D(_p1, _p2)										\
	do																		\
		{																	\
		(_p2)->x = (_p1)->x;												\
		(_p2)->y = (_p1)->y;												\
		(_p2)->w = 1.0f;													\
		}																	\
	while (0)

#define __Q3FastRationalPoint3D_To2D(_p1, _p2)								\
	do																		\
		{																	\
		float invW = 1.0f / (_p1)->w;										\
		(_p2)->x = (_p1)->x * invW;											\
		(_p2)->y = (_p1)->y * invW;											\
		}																	\
	while (0)

#define __Q3FastPoint3D_To4D(_p1, _p2)										\
	do																		\
		{																	\
		(_p2)->x = (_p1)->x;												\
		(_p2)->y = (_p1)->y;												\
		(_p2)->z = (_p1)->z;												\
		(_p2)->w = 1.0f;													\
		}																	\
	while (0)

#define __Q3FastRationalPoint4D_To3D(_p1, _p2)								\
	do																		\
		{																	\
		float invW = 1.0f / (_p1)->w;										\
		(_p2)->x = (_p1)->x * invW;											\
		(_p2)->y = (_p1)->y * invW;											\
		(_p2)->z = (_p1)->z * invW;											\
		}																	\
	while (0)

#define __Q3FastPolarPoint_ToPoint2D(_p1, _p2)								\
	do																		\
		{																	\
		(_p2)->x = (_p1)->r * ((float) cos((_p1)->theta));					\
		(_p2)->y = (_p1)->r * ((float) sin((_p1)->theta));					\
		}																	\
	while (0)

#define __Q3FastVector2D_Dot(_v1, _v2)										\
	(																		\
		((_v1)->x * (_v2)->x) +												\
		((_v1)->y * (_v2)->y)												\
	)																		\

#define __Q3FastVector3D_Dot(_v1, _v2)										\
	(																		\
		((_v1)->x * (_v2)->x) +												\
		((_v1)->y * (_v2)->y) +												\
		((_v1)->z * (_v2)->z)												\
	)																		\

#define __Q3FastVector2D_Cross(_v1, _v2)									\
	(																		\
		((_v1)->x * (_v2)->y) -												\
		((_v1)->y * (_v2)->x)												\
	)																		\

#define __Q3FastPoint2D_CrossProductTri(_p1, _p2, _p3)						\
	(																		\
		(((_p2)->x - (_p1)->x) * ((_p3)->x - (_p2)->x)) -					\
		(((_p2)->y - (_p1)->y) * ((_p3)->y - (_p2)->y))						\
	)																		\

#define __Q3FastVector3D_Cross(_v1, _v2, _r)								\
	do																		\
		{																	\
		float rx = ((_v1)->y * (_v2)->z) - ((_v1)->z * (_v2)->y);			\
        float ry = ((_v1)->z * (_v2)->x) - ((_v1)->x * (_v2)->z);			\
        float rz = ((_v1)->x * (_v2)->y) - ((_v1)->y * (_v2)->x);			\
        																	\
        (_r)->x = rx;														\
        (_r)->y = ry;														\
        (_r)->z = rz;														\
		}																	\
	while (0)

#define __Q3FastPoint3D_CrossProductTri(_p1, _p2, _p3, _r)					\
	do																		\
		{																	\
		float _v1_x = (_p2)->x - (_p1)->x;									\
		float _v1_y = (_p2)->y - (_p1)->y;									\
		float _v1_z = (_p2)->z - (_p1)->z;									\
																			\
		float _v2_x = (_p3)->x - (_p2)->x;									\
		float _v2_y = (_p3)->y - (_p2)->y;									\
		float _v2_z = (_p3)->z - (_p2)->z;									\
																			\
		(_r)->x = (_v1_y * _v2_z) - (_v1_z * _v2_y);						\
        (_r)->y = (_v1_z * _v2_x) - (_v1_x * _v2_z);						\
        (_r)->z = (_v1_x * _v2_y) - (_v1_y * _v2_x);						\
		}																	\
	while (0)

#define __Q3FastVector2D_Length(_v)											\
	(																		\
		(float) sqrt(__Q3FastVector2D_LengthSquared(_v))					\
	)																		\

#define __Q3FastVector2D_LengthSquared(_v)									\
	(																		\
		((_v)->x * (_v)->x) +												\
		((_v)->y * (_v)->y)													\
	)																		\

#define __Q3FastVector3D_Length(_v)											\
	(																		\
		(float) sqrt(__Q3FastVector3D_LengthSquared(_v))					\
	)																		\

#define __Q3FastVector3D_LengthSquared(_v)									\
	(																		\
		((_v)->x * (_v)->x) +												\
		((_v)->y * (_v)->y)	+												\
		((_v)->z * (_v)->z)													\
	)																		\

#define __Q3FastPoint2D_Distance(_p1, _p2)									\
	(																		\
		(float) sqrt(__Q3FastPoint2D_DistanceSquared(_p1, _p2))				\
	)																		\

#define __Q3FastPoint2D_DistanceSquared(_p1, _p2)							\
	(																		\
		(((_p1)->x - (_p2)->x) * ((_p1)->x - (_p2)->x))	+					\
		(((_p1)->y - (_p2)->y) * ((_p1)->y - (_p2)->y))						\
	)																		\

#define __Q3FastParam2D_Distance(_p1, _p2)									\
	(																		\
		__Q3FastPoint2D_Distance((const TQ3Point2D*) _p1,					\
							     (const TQ3Point2D*) _p2)					\
	)																		\

#define __Q3FastParam2D_DistanceSquared(_p1, _p2)							\
	(																		\
		__Q3FastPoint2D_DistanceSquared((const TQ3Point2D*) _p1,			\
									    (const TQ3Point2D*) _p2)			\
	)																		\

#define __Q3FastRationalPoint3D_Distance(_p1, _p2)							\
	(																		\
		(float) sqrt(__Q3FastRationalPoint3D_DistanceSquared(_p1, _p2))		\
	)																		\

#define __Q3FastRationalPoint3D_DistanceSquared(_p1, _p2)					\
	(																		\
		(((_p1)->x - (_p2)->x) * ((_p1)->x - (_p2)->x))	+					\
		(((_p1)->y - (_p2)->y) * ((_p1)->y - (_p2)->y))	+					\
		(((_p1)->w - (_p2)->w) * ((_p1)->w - (_p2)->w))						\
	)																		\

#define __Q3FastPoint3D_Distance(_p1, _p2)									\
	(																		\
		(float) sqrt(__Q3FastPoint3D_DistanceSquared(_p1, _p2))				\
	)																		\

#define __Q3FastPoint3D_DistanceSquared(_p1, _p2)							\
	(																		\
		(((_p1)->x - (_p2)->x) * ((_p1)->x - (_p2)->x))	+					\
		(((_p1)->y - (_p2)->y) * ((_p1)->y - (_p2)->y))	+					\
		(((_p1)->z - (_p2)->z) * ((_p1)->z - (_p2)->z))						\
	)																		\

#define __Q3FastRationalPoint4D_Distance(_p1, _p2)							\
	(																		\
		(float) sqrt(__Q3FastRationalPoint4D_DistanceSquared(_p1, _p2))		\
	)																		\

#define __Q3FastRationalPoint4D_DistanceSquared(_p1, _p2)					\
	(																		\
		(((_p1)->x - (_p2)->x) * ((_p1)->x - (_p2)->x))	+					\
		(((_p1)->y - (_p2)->y) * ((_p1)->y - (_p2)->y))	+					\
		(((_p1)->z - (_p2)->z) * ((_p1)->z - (_p2)->z))	+					\
		(((_p1)->w - (_p2)->w) * ((_p1)->w - (_p2)->w))						\
	)																		\

#define __Q3FastVector2D_Negate(_v1, _v2)									\
	do																		\
		{																	\
		(_v2)->x = -(_v1)->x;												\
		(_v2)->y = -(_v1)->y;												\
		}																	\
	while (0)

#define __Q3FastVector3D_Negate(_v1, _v2)									\
	do																		\
		{																	\
		(_v2)->x = -(_v1)->x;												\
		(_v2)->y = -(_v1)->y;												\
		(_v2)->z = -(_v1)->z;												\
		}																	\
	while (0)

#define __Q3FastVector2D_Scale(_v1, _s, _v2)								\
	do																		\
		{																	\
		(_v2)->x = (_v1)->x * (_s);											\
		(_v2)->y = (_v1)->y * (_s);											\
		}																	\
	while (0)

#define __Q3FastVector3D_Scale(_v1, _s, _v2)								\
	do																		\
		{																	\
		(_v2)->x = (_v1)->x * (_s);											\
		(_v2)->y = (_v1)->y * (_s);											\
		(_v2)->z = (_v1)->z * (_s);											\
		}																	\
	while (0)

#define __Q3FastVector2D_Normalize(_v1, _v2)								\
	do																		\
		{																	\
		float invLength = 1.0f / __Q3FastVector2D_Length(_v1);				\
		__Q3FastVector2D_Scale(_v1, invLength, _v2);						\
		}																	\
	while (0)

#define __Q3FastVector3D_Normalize(_v1, _v2)								\
	do																		\
		{																	\
		float invLength = 1.0f / __Q3FastVector3D_Length(_v1);				\
		__Q3FastVector3D_Scale(_v1, invLength, _v2);						\
		}																	\
	while (0)


#define __Q3FastVector2D_Add(_v1, _v2, _r)									\
	do																		\
		{																	\
		(_r)->x = (_v1)->x + (_v2)->x;										\
		(_r)->y = (_v1)->y + (_v2)->y;										\
		}																	\
	while (0)

#define __Q3FastVector3D_Add(_v1, _v2, _r)									\
	do																		\
		{																	\
		(_r)->x = (_v1)->x + (_v2)->x;										\
		(_r)->y = (_v1)->y + (_v2)->y;										\
		(_r)->z = (_v1)->z + (_v2)->z;										\
		}																	\
	while (0)

#define __Q3FastVector2D_Subtract(_v1, _v2, _r)								\
	do																		\
		{																	\
		(_r)->x = (_v1)->x - (_v2)->x;										\
		(_r)->y = (_v1)->y - (_v2)->y;										\
		}																	\
	while (0)

#define __Q3FastVector3D_Subtract(_v1, _v2, _r)								\
	do																		\
		{																	\
		(_r)->x = (_v1)->x - (_v2)->x;										\
		(_r)->y = (_v1)->y - (_v2)->y;										\
		(_r)->z = (_v1)->z - (_v2)->z;										\
		}																	\
	while (0)

#define __Q3FastPoint2D_Vector2D_Add(_p, _v, _r)							\
	do																		\
		{																	\
		(_r)->x = (_p)->x + (_v)->x;										\
		(_r)->y = (_p)->y + (_v)->y;										\
		}																	\
	while (0)

#define __Q3FastParam2D_Vector2D_Add(_p, _v, _r)							\
	do																		\
		{																	\
		(_r)->u = (_p)->u + (_v)->x;										\
		(_r)->v = (_p)->v + (_v)->y;										\
		}																	\
	while (0)

#define __Q3FastPoint3D_Vector3D_Add(_p, _v, _r)							\
	do																		\
		{																	\
		(_r)->x = (_p)->x + (_v)->x;										\
		(_r)->y = (_p)->y + (_v)->y;										\
		(_r)->z = (_p)->z + (_v)->z;										\
		}																	\
	while (0)

#define __Q3FastPoint2D_Vector2D_Subtract(_p, _v, _r)						\
	do																		\
		{																	\
		(_r)->x = (_p)->x - (_v)->x;										\
		(_r)->y = (_p)->y - (_v)->y;										\
		}																	\
	while (0)

#define __Q3FastParam2D_Vector2D_Subtract(_p, _v, _r)						\
	do																		\
		{																	\
		(_r)->u = (_p)->u - (_v)->x;										\
		(_r)->v = (_p)->v - (_v)->y;										\
		}																	\
	while (0)

#define __Q3FastPoint3D_Vector3D_Subtract(_p, _v, _r)						\
	do																		\
		{																	\
		(_r)->x = (_p)->x - (_v)->x;										\
		(_r)->y = (_p)->y - (_v)->y;										\
		(_r)->z = (_p)->z - (_v)->z;										\
		}																	\
	while (0)

#define __Q3FastPoint2D_Subtract(_p1, _p2, _r)								\
	do																		\
		{																	\
		(_r)->x = (_p1)->x - (_p2)->x;										\
		(_r)->y = (_p1)->y - (_p2)->y;										\
		}																	\
	while (0)

#define __Q3FastParam2D_Subtract(_p1, _p2, _r)								\
	do																		\
		{																	\
		(_r)->x = (_p1)->u - (_p2)->u;										\
		(_r)->y = (_p1)->v - (_p2)->v;										\
		}																	\
	while (0)

#define __Q3FastPoint3D_Subtract(_p1, _p2, _r)								\
	do																		\
		{																	\
		(_r)->x = (_p1)->x - (_p2)->x;										\
		(_r)->y = (_p1)->y - (_p2)->y;										\
		(_r)->z = (_p1)->z - (_p2)->z;										\
		}																	\
	while (0)

#define __Q3FastPoint2D_RRatio(_p1, _p2, _r1, _r2, _result)					\
	do																		\
		{																	\
		float frac   = (_r2) / ((_r1) + (_r2));								\
		(_result)->x = (_p1)->x + (frac * ((_p2)->x - (_p1)->x));			\
		(_result)->y = (_p1)->y + (frac * ((_p2)->y - (_p1)->y));			\
		}																	\
	while (0)

#define __Q3FastParam2D_RRatio(_p1, _p2, _r1, _r2, _result)					\
	do																		\
		{																	\
		float frac   = (_r2) / ((_r1) + (_r2));								\
		(_result)->u = (_p1)->u + (frac * ((_p2)->u - (_p1)->u));			\
		(_result)->v = (_p1)->v + (frac * ((_p2)->v - (_p1)->v));			\
		}																	\
	while (0)

#define __Q3FastPoint3D_RRatio(_p1, _p2, _r1, _r2, _result)					\
	do																		\
		{																	\
		float frac   = (_r2) / ((_r1) + (_r2));								\
		(_result)->x = (_p1)->x + (frac * ((_p2)->x - (_p1)->x));			\
		(_result)->y = (_p1)->y + (frac * ((_p2)->y - (_p1)->y));			\
		(_result)->z = (_p1)->z + (frac * ((_p2)->z - (_p1)->z));			\
		}																	\
	while (0)

#define __Q3FastRationalPoint4D_RRatio(_p1, _p2, _r1, _r2, _result)			\
	do																		\
		{																	\
		float frac   = (_r2) / ((_r1) + (_r2));								\
		(_result)->x = (_p1)->x + (frac * ((_p2)->x - (_p1)->x));			\
		(_result)->y = (_p1)->y + (frac * ((_p2)->y - (_p1)->y));			\
		(_result)->z = (_p1)->z + (frac * ((_p2)->z - (_p1)->z));			\
		(_result)->w = (_p1)->w + (frac * ((_p2)->w - (_p1)->w));			\
		}																	\
	while (0)

#define __Q3FastQuaternion_Set(_q, _w, _x, _y, _z)							\
	do																		\
		{																	\
		(_q)->w = (_w);														\
		(_q)->x = (_x);														\
		(_q)->y = (_y);														\
		(_q)->z = (_z);														\
		}																	\
	while (0)

#define __Q3FastQuaternion_SetIdentity(_q)									\
	do																		\
		{																	\
		(_q)->w = 1.0f;														\
		(_q)->x = 0.0f;														\
		(_q)->y = 0.0f;														\
		(_q)->z = 0.0f;														\
		}																	\
	while (0)

#define __Q3FastQuaternion_Copy(_q1, _q2)									\
	do																		\
		{																	\
		*(_q2) = *(_q1);													\
		}																	\
	while (0)

#define __Q3FastQuaternion_Dot(_q1, _q2)									\
	(																		\
		((_q1)->w * (_q2)->w) +												\
		((_q1)->x * (_q2)->x) +												\
		((_q1)->y * (_q2)->y) +												\
		((_q1)->z * (_q2)->z)												\
	)																		\

#define __Q3FastQuaternion_Normalize(_q1, _q2)								\
	do																		\
		{																	\
		float qDot    = __Q3FastQuaternion_Dot(_q1, _q1);					\
		float qFactor = 1.0f / (float) sqrt(qDot);							\
																			\
		(_q2)->w = (_q1)->w * qFactor;										\
		(_q2)->x = (_q1)->x * qFactor;										\
		(_q2)->y = (_q1)->y * qFactor;										\
		(_q2)->z = (_q1)->z * qFactor;										\
		}																	\
	while (0)

#define __Q3FastQuaternion_Invert(_q1, _q2)									\
	do																		\
		{																	\
		(_q2)->w =  (_q1)->w;												\
		(_q2)->x = -(_q1)->x;												\
		(_q2)->y = -(_q1)->y;												\
		(_q2)->z = -(_q1)->z;												\
		}																	\
	while (0)

#define __Q3FastBoundingBox_Reset(_b)										\
	do																		\
		{																	\
		(_b)->min.x =														\
		(_b)->min.y =														\
		(_b)->min.z =														\
		(_b)->max.x =														\
		(_b)->max.y =														\
		(_b)->max.z = 0.0f;													\
		(_b)->isEmpty = kQ3True;											\
		}																	\
	while (0)

#define __Q3FastBoundingBox_Set(_b, _min, _max, _isEmpty)					\
	do																		\
		{																	\
		(_b)->min     = *(_min);											\
		(_b)->max     = *(_max);											\
		(_b)->isEmpty =  (_isEmpty);										\
		}																	\
	while (0)

#define __Q3FastBoundingBox_Copy(_b1, _b2)									\
	do																		\
		{																	\
		*(_b2) = *(_b1);													\
		}																	\
	while (0)

#define __Q3FastBoundingSphere_Reset(_s)									\
	do																		\
		{																	\
		(_s)->origin.x =													\
		(_s)->origin.y =													\
		(_s)->origin.z =													\
		(_s)->radius   = 0.0f;												\
		(_s)->isEmpty = kQ3True;											\
		}																	\
	while (0)

#define __Q3FastBoundingSphere_Set(_s, _origin, _radius, _isEmpty)			\
	do																		\
		{																	\
		(_s)->origin  = *(_origin);											\
		(_s)->radius  =  (_radius);											\
		(_s)->isEmpty =  (_isEmpty);										\
		}																	\
	while (0)

#define __Q3FastBoundingSphere_Copy(_s1, _s2)								\
	do																		\
		{																	\
		*(_s2) = *(_s1);													\
		}																	\
	while (0)



// Wrappers
#ifdef __cplusplus

	inline TQ3Vector2D *Q3FastVector2D_Set(TQ3Vector2D *vector2D, float x, float y)
	{
		__Q3FastVector2D_Set(vector2D, x, y);
		return(vector2D);
	}

	inline TQ3Vector3D *Q3FastVector3D_Set(TQ3Vector3D *vector3D, float x, float y, float z)
	{
		__Q3FastVector3D_Set(vector3D, x, y, z);
		return(vector3D);
	}

	inline TQ3Point2D * Q3FastPoint2D_Set(TQ3Point2D *point2D, float x, float y)
	{
		__Q3FastPoint2D_Set(point2D, x, y);
		return(point2D);
	}
	
	inline TQ3Param2D * Q3FastParam2D_Set(TQ3Param2D *param2D, float u, float v)
	{
		__Q3FastParam2D_Set(param2D, u, v);
		return(param2D);
	}
	
	inline TQ3RationalPoint3D * Q3FastRationalPoint3D_Set(TQ3RationalPoint3D *rationalPoint3D, float x, float y, float w)
	{
		__Q3FastRationalPoint3D_Set(rationalPoint3D, x, y, w);
		return(rationalPoint3D);
	}
	
	inline TQ3Point3D * Q3FastPoint3D_Set(TQ3Point3D *point3D, float x, float y, float z)
	{
		__Q3FastPoint3D_Set(point3D, x, y, z);
		return(point3D);
	}
	
	inline TQ3RationalPoint4D * Q3FastRationalPoint4D_Set(TQ3RationalPoint4D *rationalPoint4D, float x, float y, float z, float w)
	{
		__Q3FastRationalPoint4D_Set(rationalPoint4D, x, y, z, w);
		return(rationalPoint4D);
	}
	
	inline TQ3PolarPoint * Q3FastPolarPoint_Set(TQ3PolarPoint *polarPoint, float r, float theta)
	{
		__Q3FastPolarPoint_Set(polarPoint, r, theta);
		return(polarPoint);
	}
	
	inline TQ3SphericalPoint * Q3FastSphericalPoint_Set(TQ3SphericalPoint *sphericalPoint, float rho, float theta, float phi)
	{
		__Q3FastSphericalPoint_Set(sphericalPoint, rho, theta, phi);
		return(sphericalPoint);
	}
	
	inline TQ3Vector3D * Q3FastVector2D_To3D(const TQ3Vector2D *vector2D, TQ3Vector3D *result)
	{
		__Q3FastVector2D_To3D(vector2D, result);
		return(result);
	}
	
	inline TQ3RationalPoint3D * Q3FastVector2D_ToRationalPoint3D(const TQ3Vector2D *vector2D, TQ3RationalPoint3D *result)
	{
		__Q3FastVector2D_ToRationalPoint3D(vector2D, result);
		return(result);
	}
	
	inline TQ3Vector2D * Q3FastVector3D_To2D(const TQ3Vector3D *vector3D, TQ3Vector2D *result)
	{
		__Q3FastVector3D_To2D(vector3D, result);
		return(result);
	}
	
	inline TQ3Vector2D * Q3FastRationalPoint3D_ToVector2D(const TQ3RationalPoint3D *rationalPoint3D, TQ3Vector2D *result)
	{
		__Q3FastRationalPoint3D_ToVector2D(rationalPoint3D, result);
		return(result);
	}
	
	inline TQ3RationalPoint4D * Q3FastVector3D_ToRationalPoint4D(const TQ3Vector3D *vector3D, TQ3RationalPoint4D *result)
	{
		__Q3FastVector3D_ToRationalPoint4D(vector3D, result);
		return(result);
	}
	
	inline TQ3Vector3D * Q3FastRationalPoint4D_ToVector3D(const TQ3RationalPoint4D *rationalPoint4D, TQ3Vector3D *result)
	{
		__Q3FastRationalPoint4D_ToVector3D(rationalPoint4D, result);
		return(result);
	}
	
	inline TQ3RationalPoint3D * Q3FastPoint2D_To3D(const TQ3Point2D *point2D, TQ3RationalPoint3D *result)
	{
		__Q3FastPoint2D_To3D(point2D, result);
		return(result);
	}
	
	inline TQ3Point2D * Q3FastRationalPoint3D_To2D(const TQ3RationalPoint3D *rationalPoint3D, TQ3Point2D *result)
	{
		__Q3FastRationalPoint3D_To2D(rationalPoint3D, result);
		return(result);
	}
	
	inline TQ3RationalPoint4D * Q3FastPoint3D_To4D(const TQ3Point3D *point3D, TQ3RationalPoint4D *result)
	{
		__Q3FastPoint3D_To4D(point3D, result);
		return(result);
	}
	
	inline TQ3Point3D * Q3FastRationalPoint4D_To3D(const TQ3RationalPoint4D *rationalPoint4D, TQ3Point3D *result)
	{
		__Q3FastRationalPoint4D_To3D(rationalPoint4D, result);
		return(result);
	}

	inline TQ3Point2D *Q3FastPolarPoint_ToPoint2D(const TQ3PolarPoint *polarPoint, TQ3Point2D *result)
	{
		__Q3FastPolarPoint_ToPoint2D(polarPoint, result);
		return(result);
	}
	
	inline float Q3FastVector2D_Dot(const TQ3Vector2D *v1, const TQ3Vector2D *v2)
	{
		return(__Q3FastVector2D_Dot(v1, v2));
	}
	
	inline float Q3FastVector3D_Dot(const TQ3Vector3D *v1, const TQ3Vector3D *v2)
	{
		return(__Q3FastVector3D_Dot(v1, v2));
	}
	
	inline float Q3FastVector2D_Cross(const TQ3Vector2D *v1, const TQ3Vector2D *v2)
	{
		return(__Q3FastVector2D_Cross(v1, v2));
	}
	
	inline float Q3FastPoint2D_CrossProductTri(const TQ3Point2D *p1, const TQ3Point2D *p2, const TQ3Point2D *p3)
	{
		return(__Q3FastPoint2D_CrossProductTri(p1, p2, p3));
	}
	
	inline TQ3Vector3D *Q3FastVector3D_Cross(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result)
	{
		__Q3FastVector3D_Cross(v1, v2, result);
		return(result);
	}
	
	inline TQ3Vector3D *Q3FastPoint3D_CrossProductTri(const TQ3Point3D *p1, const TQ3Point3D *p2, const TQ3Point3D *p3, TQ3Vector3D *result)
	{
		__Q3FastPoint3D_CrossProductTri(p1, p2, p3, result);
		return(result);
	}
	
	inline float Q3FastVector2D_Length(const TQ3Vector2D *vector2D)
	{
		return(__Q3FastVector2D_Length(vector2D));
	}
	
	inline float Q3FastVector2D_LengthSquared(const TQ3Vector2D *vector2D)
	{
		return(__Q3FastVector2D_LengthSquared(vector2D));
	}
	
	inline float Q3FastVector3D_Length(const TQ3Vector3D *vector3D)
	{
		return(__Q3FastVector3D_Length(vector3D));
	}
	
	inline float Q3FastVector3D_LengthSquared(const TQ3Vector3D *vector3D)
	{
		return(__Q3FastVector3D_LengthSquared(vector3D));
	}
	
	inline float Q3FastPoint2D_Distance(const TQ3Point2D *p1, const TQ3Point2D *p2)
	{
		return(__Q3FastPoint2D_Distance(p1, p2));
	}
	
	inline float Q3FastPoint2D_DistanceSquared(const TQ3Point2D *p1, const TQ3Point2D *p2)
	{
		return(__Q3FastPoint2D_DistanceSquared(p1, p2));
	}
	
	inline float Q3FastParam2D_Distance(const TQ3Param2D *p1, const TQ3Param2D *p2)
	{
		return(__Q3FastParam2D_Distance(p1, p2));
	}
	
	inline float Q3FastParam2D_DistanceSquared(const TQ3Param2D *p1, const TQ3Param2D *p2)
	{
		return(__Q3FastParam2D_DistanceSquared(p1, p2));
	}
	
	inline float Q3FastRationalPoint3D_Distance(const TQ3RationalPoint3D *p1, const TQ3RationalPoint3D *p2)
	{
		return(__Q3FastRationalPoint3D_Distance(p1, p2));
	}
	
	inline float Q3FastRationalPoint3D_DistanceSquared(const TQ3RationalPoint3D *p1, const TQ3RationalPoint3D *p2)
	{
		return(__Q3FastRationalPoint3D_DistanceSquared(p1, p2));
	}
	
	inline float Q3FastPoint3D_Distance(const TQ3Point3D *p1, const TQ3Point3D *p2)
	{
		return(__Q3FastPoint3D_Distance(p1, p2));
	}
	
	inline float Q3FastPoint3D_DistanceSquared(const TQ3Point3D *p1, const TQ3Point3D *p2)
	{
		return(__Q3FastPoint3D_DistanceSquared(p1, p2));
	}
	
	inline float Q3FastRationalPoint4D_Distance(const TQ3RationalPoint4D *p1, const TQ3RationalPoint4D *p2)
	{
		return(__Q3FastRationalPoint4D_Distance(p1, p2));
	}
	
	inline float Q3FastRationalPoint4D_DistanceSquared(const TQ3RationalPoint4D *p1, const TQ3RationalPoint4D *p2)
	{
		return(__Q3FastRationalPoint4D_DistanceSquared(p1, p2));
	}
	
	inline TQ3Vector2D *Q3FastVector2D_Negate(const TQ3Vector2D *vector2D, TQ3Vector2D *result)
	{
		__Q3FastVector2D_Negate(vector2D, result);
		return(result);
	}
	
	inline TQ3Vector3D *Q3FastVector3D_Negate(const TQ3Vector3D *vector3D, TQ3Vector3D *result)
	{
		__Q3FastVector3D_Negate(vector3D, result);
		return(result);
	}
	
	inline TQ3Vector2D *Q3FastVector2D_Scale(const TQ3Vector2D *vector2D, float scalar, TQ3Vector2D *result)
	{
		__Q3FastVector2D_Scale(vector2D, scalar, result);
		return(result);
	}
	
	inline TQ3Vector3D *Q3FastVector3D_Scale(const TQ3Vector3D *vector3D, float scalar, TQ3Vector3D *result)
	{
		__Q3FastVector3D_Scale(vector3D, scalar, result);
		return(result);
	}
	
	inline TQ3Vector2D *Q3FastVector2D_Normalize(const TQ3Vector2D *vector2D, TQ3Vector2D *result)
	{
		__Q3FastVector2D_Normalize(vector2D, result);
		return(result);
	}
	
	inline TQ3Vector3D *Q3FastVector3D_Normalize(const TQ3Vector3D *vector3D, TQ3Vector3D *result)
	{
		__Q3FastVector3D_Normalize(vector3D, result);
		return(result);
	}
	
	inline TQ3Vector2D *Q3FastVector2D_Add(const TQ3Vector2D *v1, const TQ3Vector2D *v2, TQ3Vector2D *result)
	{
		__Q3FastVector2D_Add(v1, v2, result);
		return(result);
	}
	
	inline TQ3Vector3D *Q3FastVector3D_Add(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result)
	{
		__Q3FastVector3D_Add(v1, v2, result);
		return(result);
	}
	
	inline TQ3Vector2D *Q3FastVector2D_Subtract(const TQ3Vector2D *v1, const TQ3Vector2D *v2, TQ3Vector2D *result)
	{
		__Q3FastVector2D_Subtract(v1, v2, result);
		return(result);
	}
	
	inline TQ3Vector3D *Q3FastVector3D_Subtract(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result)
	{
		__Q3FastVector3D_Subtract(v1, v2, result);
		return(result);
	}
	
	inline TQ3Point2D *Q3FastPoint2D_Vector2D_Add(const TQ3Point2D *point2D, const TQ3Vector2D *vector2D, TQ3Point2D *result)
	{
		__Q3FastPoint2D_Vector2D_Add(point2D, vector2D, result);
		return(result);
	}
	
	inline TQ3Param2D *Q3FastParam2D_Vector2D_Add(const TQ3Param2D *param2D, const TQ3Vector2D *vector2D, TQ3Param2D *result)
	{
		__Q3FastParam2D_Vector2D_Add(param2D, vector2D, result);
		return(result);
	}
	
	inline TQ3Point3D *Q3FastPoint3D_Vector3D_Add(const TQ3Point3D *point3D, const TQ3Vector3D *vector3D, TQ3Point3D *result)
	{
		__Q3FastPoint3D_Vector3D_Add(point3D, vector3D, result);
		return(result);
	}
	
	inline TQ3Point2D *Q3FastPoint2D_Vector2D_Subtract(const TQ3Point2D *point2D, const TQ3Vector2D *vector2D, TQ3Point2D *result)
	{
		__Q3FastPoint2D_Vector2D_Subtract(point2D, vector2D, result);
		return(result);
	}
	
	inline TQ3Param2D *Q3FastParam2D_Vector2D_Subtract(const TQ3Param2D *param2D, const TQ3Vector2D *vector2D, TQ3Param2D *result)
	{
		__Q3FastParam2D_Vector2D_Subtract(param2D, vector2D, result);
		return(result);
	}
	
	inline TQ3Point3D *Q3FastPoint3D_Vector3D_Subtract(const TQ3Point3D *point3D, const TQ3Vector3D *vector3D, TQ3Point3D *result)
	{
		__Q3FastPoint3D_Vector3D_Subtract(point3D, vector3D, result);
		return(result);
	}
	
	inline TQ3Vector2D *Q3FastPoint2D_Subtract(const TQ3Point2D *p1, const TQ3Point2D *p2, TQ3Vector2D *result)
	{
		__Q3FastPoint2D_Subtract(p1, p2, result);
		return(result);
	}
	
	inline TQ3Vector2D *Q3FastParam2D_Subtract(const TQ3Param2D *p1, const TQ3Param2D *p2, TQ3Vector2D *result)
	{
		__Q3FastParam2D_Subtract(p1, p2, result);
		return(result);
	}
	
	inline TQ3Vector3D *Q3FastPoint3D_Subtract(const TQ3Point3D *p1, const TQ3Point3D *p2, TQ3Vector3D *result)
	{
		__Q3FastPoint3D_Subtract(p1, p2, result);
		return(result);
	}
	
	inline TQ3Point2D *Q3FastPoint2D_RRatio(const TQ3Point2D *p1, const TQ3Point2D *p2, float r1, float r2, TQ3Point2D *result)
	{
		__Q3FastPoint2D_RRatio(p1, p2, r1, r2, result);
		return(result);
	}
	
	inline TQ3Param2D *Q3FastParam2D_RRatio(const TQ3Param2D *p1, const TQ3Param2D *p2, float r1, float r2, TQ3Param2D *result)
	{
		__Q3FastParam2D_RRatio(p1, p2, r1, r2, result);
		return(result);
	}

	inline TQ3Point3D *Q3FastPoint3D_RRatio(const TQ3Point3D *p1, const TQ3Point3D *p2, float r1, float r2, TQ3Point3D *result)
	{
		__Q3FastPoint3D_RRatio(p1, p2, r1, r2, result);
		return(result);
	}
	
	inline TQ3RationalPoint4D *Q3FastRationalPoint4D_RRatio(const TQ3RationalPoint4D *p1, const TQ3RationalPoint4D *p2, float r1, float r2, TQ3RationalPoint4D *result)
	{
		__Q3FastRationalPoint4D_RRatio(p1, p2, r1, r2, result);
		return(result);
	}
	
	inline TQ3Quaternion *Q3FastQuaternion_Set(TQ3Quaternion *quaternion, float w, float x, float y, float z)
	{
		__Q3FastQuaternion_Set(quaternion, w, x, y, z);
		return(quaternion);
	}
	
	inline TQ3Quaternion *Q3FastQuaternion_SetIdentity(TQ3Quaternion *quaternion)
	{
		__Q3FastQuaternion_SetIdentity(quaternion);
		return(quaternion);
	}
	
	inline TQ3Quaternion *Q3FastQuaternion_Copy(const TQ3Quaternion *quaternion, TQ3Quaternion *result)
	{
		__Q3FastQuaternion_Copy(quaternion, result);
		return(result);
	}
	
	inline float Q3FastQuaternion_Dot(const TQ3Quaternion *q1, const TQ3Quaternion *q2)
	{
		return(__Q3FastQuaternion_Dot(q1, q2));
	}
	
	inline TQ3Quaternion *Q3FastQuaternion_Normalize(const TQ3Quaternion *quaternion, TQ3Quaternion *result)
	{
		__Q3FastQuaternion_Normalize(quaternion, result);
		return(result);
	}
	
	inline TQ3Quaternion *Q3FastQuaternion_Invert(const TQ3Quaternion *quaternion, TQ3Quaternion *result)
	{
		__Q3FastQuaternion_Invert(quaternion, result);
		return(result);
	}
	
	inline TQ3BoundingBox *Q3FastBoundingBox_Reset(TQ3BoundingBox *bBox)
	{
		__Q3FastBoundingBox_Reset(bBox);
		return(bBox);
	}
	
	inline TQ3BoundingBox *Q3FastBoundingBox_Set(TQ3BoundingBox *bBox, const TQ3Point3D *min, const TQ3Point3D *max, TQ3Boolean isEmpty)
	{
		__Q3FastBoundingBox_Set(bBox, min, max, isEmpty);
		return(bBox);
	}
	
	inline TQ3BoundingBox *Q3FastBoundingBox_Copy(const TQ3BoundingBox *bBox, TQ3BoundingBox *result)
	{
		__Q3FastBoundingBox_Copy(bBox, result);
		return(result);
	}
	
	inline TQ3BoundingSphere *Q3FastBoundingSphere_Reset(TQ3BoundingSphere *bSphere)
	{
		__Q3FastBoundingSphere_Reset(bSphere);
		return(bSphere);
	}
	
	inline TQ3BoundingSphere *Q3FastBoundingSphere_Set(TQ3BoundingSphere *bSphere, const TQ3Point3D *origin, float radius, TQ3Boolean isEmpty)
	{
		__Q3FastBoundingSphere_Set(bSphere, origin, radius, isEmpty);
		return(bSphere);
	}
	
	inline TQ3BoundingSphere *Q3FastBoundingSphere_Copy(const TQ3BoundingSphere *bSphere, TQ3BoundingSphere *result)
	{
		__Q3FastBoundingSphere_Copy(bSphere, result);
		return(result);
	}

#else
	#define Q3FastVector2D_Set							__Q3FastVector2D_Set
	#define Q3FastVector3D_Set							__Q3FastVector3D_Set
	#define Q3FastPoint2D_Set							__Q3FastPoint2D_Set
	#define Q3FastParam2D_Set							__Q3FastParam2D_Set
	#define Q3FastRationalPoint3D_Set					__Q3FastRationalPoint3D_Set
	#define Q3FastPoint3D_Set							__Q3FastPoint3D_Set
	#define Q3FastRationalPoint4D_Set					__Q3FastRationalPoint4D_Set
	#define Q3FastPolarPoint_Set						__Q3FastPolarPoint_Set
	#define Q3FastSphericalPoint_Set					__Q3FastSphericalPoint_Set
	#define Q3FastVector2D_To3D							__Q3FastVector2D_To3D
	#define Q3FastVector2D_ToRationalPoint3D			__Q3FastVector2D_ToRationalPoint3D
	#define Q3FastVector3D_To2D							__Q3FastVector3D_To2D
	#define Q3FastRationalPoint3D_ToVector2D			__Q3FastRationalPoint3D_ToVector2D
	#define Q3FastVector3D_ToRationalPoint4D			__Q3FastVector3D_ToRationalPoint4D
	#define Q3FastRationalPoint4D_ToVector3D			__Q3FastRationalPoint4D_ToVector3D
	#define Q3FastPoint2D_To3D							__Q3FastPoint2D_To3D
	#define Q3FastRationalPoint3D_To2D					__Q3FastRationalPoint3D_To2D
	#define Q3FastPoint3D_To4D							__Q3FastPoint3D_To4D
	#define Q3FastRationalPoint4D_To3D					__Q3FastRationalPoint4D_To3D
	#define Q3FastPolarPoint_ToPoint2D					__Q3FastPolarPoint_ToPoint2D
	#define Q3FastVector2D_Dot							__Q3FastVector2D_Dot
	#define Q3FastVector3D_Dot							__Q3FastVector3D_Dot
	#define Q3FastVector2D_Cross						__Q3FastVector2D_Cross
	#define Q3FastPoint2D_CrossProductTri				__Q3FastPoint2D_CrossProductTri
	#define Q3FastVector3D_Cross						__Q3FastVector3D_Cross
	#define Q3FastPoint3D_CrossProductTri				__Q3FastPoint3D_CrossProductTri
	#define Q3FastVector2D_Length						__Q3FastVector2D_Length
	#define Q3FastVector2D_LengthSquared				__Q3FastVector2D_LengthSquared
	#define Q3FastVector3D_Length						__Q3FastVector3D_Length
	#define Q3FastVector3D_LengthSquared				__Q3FastVector3D_LengthSquared
	#define Q3FastPoint2D_Distance						__Q3FastPoint2D_Distance
	#define Q3FastPoint2D_DistanceSquared				__Q3FastPoint2D_DistanceSquared
	#define Q3FastParam2D_Distance						__Q3FastParam2D_Distance
	#define Q3FastParam2D_DistanceSquared				__Q3FastParam2D_DistanceSquared
	#define Q3FastRationalPoint3D_Distance				__Q3FastRationalPoint3D_Distance
	#define Q3FastRationalPoint3D_DistanceSquared		__Q3FastRationalPoint3D_DistanceSquared
	#define Q3FastPoint3D_Distance						__Q3FastPoint3D_Distance
	#define Q3FastPoint3D_DistanceSquared				__Q3FastPoint3D_DistanceSquared
	#define Q3FastRationalPoint4D_Distance				__Q3FastRationalPoint4D_Distance
	#define Q3FastRationalPoint4D_DistanceSquared		__Q3FastRationalPoint4D_DistanceSquared
	#define Q3FastVector2D_Negate						__Q3FastVector2D_Negate
	#define Q3FastVector3D_Negate						__Q3FastVector3D_Negate
	#define Q3FastVector2D_Scale						__Q3FastVector2D_Scale
	#define Q3FastVector3D_Scale						__Q3FastVector3D_Scale
	#define Q3FastVector2D_Normalize					__Q3FastVector2D_Normalize
	#define Q3FastVector3D_Normalize					__Q3FastVector3D_Normalize
	#define Q3FastVector2D_Add							__Q3FastVector2D_Add
	#define Q3FastVector3D_Add							__Q3FastVector3D_Add
	#define Q3FastVector2D_Subtract						__Q3FastVector2D_Subtract
	#define Q3FastVector3D_Subtract						__Q3FastVector3D_Subtract
	#define Q3FastPoint2D_Vector2D_Add					__Q3FastPoint2D_Vector2D_Add
	#define Q3FastParam2D_Vector2D_Add					__Q3FastParam2D_Vector2D_Add
	#define Q3FastPoint3D_Vector3D_Add					__Q3FastPoint3D_Vector3D_Add
	#define Q3FastPoint2D_Vector2D_Subtract				__Q3FastPoint2D_Vector2D_Subtract
	#define Q3FastParam2D_Vector2D_Subtract				__Q3FastParam2D_Vector2D_Subtract
	#define Q3FastPoint3D_Vector3D_Subtract				__Q3FastPoint3D_Vector3D_Subtract
	#define Q3FastPoint2D_Subtract						__Q3FastPoint2D_Subtract
	#define Q3FastParam2D_Subtract						__Q3FastParam2D_Subtract
	#define Q3FastPoint3D_Subtract						__Q3FastPoint3D_Subtract
	#define Q3FastPoint2D_RRatio						__Q3FastPoint2D_RRatio
	#define Q3FastParam2D_RRatio						__Q3FastParam2D_RRatio
	#define Q3FastPoint3D_RRatio						__Q3FastPoint3D_RRatio
	#define Q3FastRationalPoint4D_RRatio				__Q3FastRationalPoint4D_RRatio
	#define Q3FastQuaternion_Set						__Q3FastQuaternion_Set
	#define Q3FastQuaternion_SetIdentity				__Q3FastQuaternion_SetIdentity
	#define Q3FastQuaternion_Copy						__Q3FastQuaternion_Copy
	#define Q3FastQuaternion_Dot						__Q3FastQuaternion_Dot
	#define Q3FastQuaternion_Normalize					__Q3FastQuaternion_Normalize
	#define Q3FastQuaternion_Invert						__Q3FastQuaternion_Invert
	#define Q3FastBoundingBox_Reset						__Q3FastBoundingBox_Reset
	#define Q3FastBoundingBox_Set						__Q3FastBoundingBox_Set
	#define Q3FastBoundingBox_Copy						__Q3FastBoundingBox_Copy
	#define Q3FastBoundingSphere_Reset					__Q3FastBoundingSphere_Reset
	#define Q3FastBoundingSphere_Set					__Q3FastBoundingSphere_Set
	#define Q3FastBoundingSphere_Copy					__Q3FastBoundingSphere_Copy
#endif



// Redirection
#if QUESA_ALLOW_INLINE_APIS
	#define Q3Vector2D_Set								Q3FastVector2D_Set
	#define Q3Vector3D_Set								Q3FastVector3D_Set
	#define Q3Point2D_Set								Q3FastPoint2D_Set
	#define Q3Param2D_Set								Q3FastParam2D_Set
	#define Q3RationalPoint3D_Set						Q3FastRationalPoint3D_Set
	#define Q3Point3D_Set								Q3FastPoint3D_Set
	#define Q3RationalPoint4D_Set						Q3FastRationalPoint4D_Set
	#define Q3PolarPoint_Set							Q3FastPolarPoint_Set
	#define Q3SphericalPoint_Set						Q3FastSphericalPoint_Set
	#define Q3Vector2D_To3D								Q3FastVector2D_To3D
	#define Q3Vector2D_ToRationalPoint3D				Q3FastVector2D_ToRationalPoint3D
	#define Q3Vector3D_To2D								Q3FastVector3D_To2D
	#define Q3RationalPoint3D_ToVector2D				Q3FastRationalPoint3D_ToVector2D
	#define Q3Vector3D_ToRationalPoint4D				Q3FastVector3D_ToRationalPoint4D
	#define Q3RationalPoint4D_ToVector3D				Q3FastRationalPoint4D_ToVector3D
	#define Q3Point2D_To3D								Q3FastPoint2D_To3D
	#define Q3RationalPoint3D_To2D						Q3FastRationalPoint3D_To2D
	#define Q3Point3D_To4D								Q3FastPoint3D_To4D
	#define Q3RationalPoint4D_To3D						Q3FastRationalPoint4D_To3D
	#define Q3PolarPoint_ToPoint2D						Q3FastPolarPoint_ToPoint2D
	#define Q3Vector2D_Dot								Q3FastVector2D_Dot
	#define Q3Vector3D_Dot								Q3FastVector3D_Dot
	#define Q3Vector2D_Cross							Q3FastVector2D_Cross
	#define Q3Point2D_CrossProductTri					Q3FastPoint2D_CrossProductTri
	#define Q3Vector3D_Cross							Q3FastVector3D_Cross
	#define Q3Point3D_CrossProductTri					Q3FastPoint3D_CrossProductTri
	#define Q3Vector2D_Length							Q3FastVector2D_Length
	#define Q3Vector2D_LengthSquared					Q3FastVector2D_LengthSquared
	#define Q3Vector3D_Length							Q3FastVector3D_Length
	#define Q3Vector3D_LengthSquared					Q3FastVector3D_LengthSquared
	#define Q3Point2D_Distance							Q3FastPoint2D_Distance
	#define Q3Point2D_DistanceSquared					Q3FastPoint2D_DistanceSquared
	#define Q3Param2D_Distance							Q3FastParam2D_Distance
	#define Q3Param2D_DistanceSquared					Q3FastParam2D_DistanceSquared
	#define Q3RationalPoint3D_Distance					Q3FastRationalPoint3D_Distance
	#define Q3RationalPoint3D_DistanceSquared			Q3FastRationalPoint3D_DistanceSquared
	#define Q3Point3D_Distance							Q3FastPoint3D_Distance
	#define Q3Point3D_DistanceSquared					Q3FastPoint3D_DistanceSquared
	#define Q3RationalPoint4D_Distance					Q3FastRationalPoint4D_Distance
	#define Q3RationalPoint4D_DistanceSquared			Q3FastRationalPoint4D_DistanceSquared
	#define Q3Vector2D_Negate							Q3FastVector2D_Negate
	#define Q3Vector3D_Negate							Q3FastVector3D_Negate
	#define Q3Vector2D_Scale							Q3FastVector2D_Scale
	#define Q3Vector3D_Scale							Q3FastVector3D_Scale
	#define Q3Vector2D_Normalize						Q3FastVector2D_Normalize
	#define Q3Vector3D_Normalize						Q3FastVector3D_Normalize
	#define Q3Vector2D_Add								Q3FastVector2D_Add
	#define Q3Vector3D_Add								Q3FastVector3D_Add
	#define Q3Vector2D_Subtract							Q3FastVector2D_Subtract
	#define Q3Vector3D_Subtract							Q3FastVector3D_Subtract
	#define Q3Point2D_Vector2D_Add						Q3FastPoint2D_Vector2D_Add
	#define Q3Param2D_Vector2D_Add						Q3FastParam2D_Vector2D_Add
	#define Q3Point3D_Vector3D_Add						Q3FastPoint3D_Vector3D_Add
	#define Q3Point2D_Vector2D_Subtract					Q3FastPoint2D_Vector2D_Subtract
	#define Q3Param2D_Vector2D_Subtract					Q3FastParam2D_Vector2D_Subtract
	#define Q3Point3D_Vector3D_Subtract					Q3FastPoint3D_Vector3D_Subtract
	#define Q3Point2D_Subtract							Q3FastPoint2D_Subtract
	#define Q3Param2D_Subtract							Q3FastParam2D_Subtract
	#define Q3Point3D_Subtract							Q3FastPoint3D_Subtract
	#define Q3Point2D_RRatio							Q3FastPoint2D_RRatio
	#define Q3Param2D_RRatio							Q3FastParam2D_RRatio
	#define Q3Point3D_RRatio							Q3FastPoint3D_RRatio
	#define Q3RationalPoint4D_RRatio					Q3FastRationalPoint4D_RRatio
	#define Q3Quaternion_Set							Q3FastQuaternion_Set
	#define Q3Quaternion_SetIdentity					Q3FastQuaternion_SetIdentity
	#define Q3Quaternion_Copy							Q3FastQuaternion_Copy
	#define Q3Quaternion_Dot							Q3FastQuaternion_Dot
	#define Q3Quaternion_Normalize						Q3FastQuaternion_Normalize
	#define Q3Quaternion_Invert							Q3FastQuaternion_Invert
	#define Q3BoundingBox_Reset							Q3FastBoundingBox_Reset
	#define Q3BoundingBox_Set							Q3FastBoundingBox_Set
	#define Q3BoundingBox_Copy							Q3FastBoundingBox_Copy
	#define Q3BoundingSphere_Reset						Q3FastBoundingSphere_Reset
	#define Q3BoundingSphere_Set						Q3FastBoundingSphere_Set
	#define Q3BoundingSphere_Copy						Q3FastBoundingSphere_Copy
#endif

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif



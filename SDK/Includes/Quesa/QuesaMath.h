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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector2D         Description of the parameter.
 *  @param x                Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector3D         Description of the parameter.
 *  @param x                Description of the parameter.
 *  @param y                Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point2D          Description of the parameter.
 *  @param x                Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param param2D          Description of the parameter.
 *  @param u                Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param rationalPoint3D  Description of the parameter.
 *  @param x                Description of the parameter.
 *  @param y                Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point3D          Description of the parameter.
 *  @param x                Description of the parameter.
 *  @param y                Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param rationalPoint4D  Description of the parameter.
 *  @param x                Description of the parameter.
 *  @param y                Description of the parameter.
 *  @param z                Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polarPoint       Description of the parameter.
 *  @param r                Description of the parameter.
 *  @param theta            Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param sphericalPoint   Description of the parameter.
 *  @param rho              Description of the parameter.
 *  @param theta            Description of the parameter.
 *  @param phi              Description of the parameter.
 *  @result                 Description of the function result.
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
 *      This operation makes no sense mathematically.
 *
 *  @param vector2D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      Not supported by QD3D.
 *
 *  @param vector2D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      This operation makes no sense mathematically.
 *
 *  @param vector3D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      Not supported by QD3D.
 *
 *  @param rationalPoint3D  Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      Not supported by QD3D.
 *
 *  @param vector3D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      Not supported by QD3D.
 *
 *  @param rationalPoint4D  Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      The Apple version incorrectly declares the type of 'result' to
 *      be TQ3Point3D rather than TQ3RationalPoint3D. At a binary level
 *      there is no difference, but at the source code level the Apple
 *      version forces the use of the incorrect type or type casting.
 *
 *  @param point2D          Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param rationalPoint3D  Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point3D          Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param rationalPoint4D  Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point2D          Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polarPoint       Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point3D          Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param sphericalPoint   Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param v1               Description of the parameter.
 *  @param v2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param v1               Description of the parameter.
 *  @param v2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param v1               Description of the parameter.
 *  @param v2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      Not supported by QD3D.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @param p3               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param v1               Description of the parameter.
 *  @param v2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @param p3               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector2D         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( float  )
Q3Vector2D_Length (
    const TQ3Vector2D             *vector2D
);



/*!
 *  @function
 *      Q3Vector2D_LengthSquared  
 *  @discussion
 *      Not supported by QD3D.
 *
 *  @param vector2D         Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector3D         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( float  )
Q3Vector3D_Length (
    const TQ3Vector3D             *vector3D
);



/*!
 *  @function
 *      Q3Vector3D_LengthSquared 
 *  @discussion
 *      Not supported by QD3D.
 *
 *  @param vector3D         Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      This operation makes no sense mathematically.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      This operation makes no sense mathematically.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      This operation makes no sense mathematically.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      This operation makes no sense mathematically.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector2D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector3D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector2D         Description of the parameter.
 *  @param scalar           Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector3D         Description of the parameter.
 *  @param scalar           Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector2D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector3D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param v1               Description of the parameter.
 *  @param v2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param v1               Description of the parameter.
 *  @param v2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param v1               Description of the parameter.
 *  @param v2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param v1               Description of the parameter.
 *  @param v2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point2D          Description of the parameter.
 *  @param vector2D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param param2D          Description of the parameter.
 *  @param vector2D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point3D          Description of the parameter.
 *  @param vector3D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point2D          Description of the parameter.
 *  @param vector2D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param param2D          Description of the parameter.
 *  @param vector2D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point3D          Description of the parameter.
 *  @param vector3D         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @param r1               Description of the parameter.
 *  @param r2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @param r1               Description of the parameter.
 *  @param r2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @param r1               Description of the parameter.
 *  @param r2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      This operation makes no sense mathematically.
 *
 *  @param p1               Description of the parameter.
 *  @param p2               Description of the parameter.
 *  @param r1               Description of the parameter.
 *  @param r2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param points2D         Description of the parameter.
 *  @param weights          Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param params2D         Description of the parameter.
 *  @param weights          Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      This operation makes no sense mathematically.
 *
 *  @param rationalPoints3D Description of the parameter.
 *  @param weights          Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param points3D         Description of the parameter.
 *  @param weights          Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      This operation makes no sense mathematically.
 *
 *  @param rationalPoints4D Description of the parameter.
 *  @param weights          Description of the parameter.
 *  @param numPoints        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector2D         Description of the parameter.
 *  @param matrix3x3        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector3D         Description of the parameter.
 *  @param matrix4x4        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point2D          Description of the parameter.
 *  @param matrix3x3        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param param2D          Description of the parameter.
 *  @param matrix3x3        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      Not supported by QD3D.
 *
 *  @param rationalPoint3D  Description of the parameter.
 *  @param matri3x3         Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point3D          Description of the parameter.
 *  @param matrix4x4        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param rationalPoint4D  Description of the parameter.
 *  @param matrix4x4        Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
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



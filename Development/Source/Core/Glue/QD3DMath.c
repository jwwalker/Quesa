/*  NAME:
        QD3DMath.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Math.h"





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
//      Q3Vector2D_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector2D *
Q3Vector2D_Set(TQ3Vector2D *vector2D, float x, float y)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector2D
		return(NULL);

	if (0) // Further checks on x
		return(NULL);

	if (0) // Further checks on y
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_Set(vector2D, x, y));
}





//=============================================================================
//      Q3Vector3D_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Vector3D_Set(TQ3Vector3D *vector3D, float x, float y, float z)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector3D
		return(NULL);

	if (0) // Further checks on x
		return(NULL);

	if (0) // Further checks on y
		return(NULL);

	if (0) // Further checks on z
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_Set(vector3D, x, y, z));
}





//=============================================================================
//      Q3Point2D_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point2D *
Q3Point2D_Set(TQ3Point2D *point2D, float x, float y)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point2D), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point2D
		return(NULL);

	if (0) // Further checks on x
		return(NULL);

	if (0) // Further checks on y
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_Set(point2D, x, y));
}





//=============================================================================
//      Q3Param2D_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Param2D *
Q3Param2D_Set(TQ3Param2D *param2D, float u, float v)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(param2D), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on param2D
		return(NULL);

	if (0) // Further checks on u
		return(NULL);

	if (0) // Further checks on v
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Param2D_Set(param2D, u, v));
}





//=============================================================================
//      Q3RationalPoint3D_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
Q3RationalPoint3D_Set(TQ3RationalPoint3D *rationalPoint3D, float x, float y, float w)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoint3D), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on rationalPoint3D
		return(NULL);

	if (0) // Further checks on x
		return(NULL);

	if (0) // Further checks on y
		return(NULL);

	if (0) // Further checks on w
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint3D_Set(rationalPoint3D, x, y, w));
}





//=============================================================================
//      Q3Point3D_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point3D *
Q3Point3D_Set(TQ3Point3D *point3D, float x, float y, float z)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point3D), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point3D
		return(NULL);

	if (0) // Further checks on x
		return(NULL);

	if (0) // Further checks on y
		return(NULL);

	if (0) // Further checks on z
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_Set(point3D, x, y, z));
}





//=============================================================================
//      Q3RationalPoint4D_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
Q3RationalPoint4D_Set(TQ3RationalPoint4D *rationalPoint4D, float x, float y, float z, float w)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoint4D), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on rationalPoint4D
		return(NULL);

	if (0) // Further checks on x
		return(NULL);

	if (0) // Further checks on y
		return(NULL);

	if (0) // Further checks on z
		return(NULL);

	if (0) // Further checks on w
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint4D_Set(rationalPoint4D, x, y, z, w));
}





//=============================================================================
//      Q3PolarPoint_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3PolarPoint *
Q3PolarPoint_Set(TQ3PolarPoint *polarPoint, float r, float theta)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polarPoint), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polarPoint
		return(NULL);

	if (0) // Further checks on r
		return(NULL);

	if (0) // Further checks on theta
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolarPoint_Set(polarPoint, r, theta));
}





//=============================================================================
//      Q3SphericalPoint_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3SphericalPoint *
Q3SphericalPoint_Set(TQ3SphericalPoint *sphericalPoint, float rho, float theta, float phi)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(sphericalPoint), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sphericalPoint
		return(NULL);

	if (0) // Further checks on rho
		return(NULL);

	if (0) // Further checks on theta
		return(NULL);

	if (0) // Further checks on phi
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SphericalPoint_Set(sphericalPoint, rho, theta, phi));
}





//=============================================================================
//      Q3Vector2D_To3D : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector3D *
Q3Vector2D_To3D(const TQ3Vector2D *vector2D, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector2D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_To3D(vector2D, result));
}





//=============================================================================
//      Q3Vector2D_ToRationalPoint3D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
Q3Vector2D_ToRationalPoint3D(const TQ3Vector2D *vector2D, TQ3RationalPoint3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector2D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_ToRationalPoint3D(vector2D, result));
}





//=============================================================================
//      Q3Vector3D_To2D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector2D *
Q3Vector3D_To2D(const TQ3Vector3D *vector3D, TQ3Vector2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector3D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_To2D(vector3D, result));
}





//=============================================================================
//      Q3RationalPoint3D_ToVector2D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector2D *
Q3RationalPoint3D_ToVector2D(const TQ3RationalPoint3D *rationalPoint3D, TQ3Vector2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoint3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on rationalPoint3D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint3D_ToVector2D(rationalPoint3D, result));
}





//=============================================================================
//      Q3Vector3D_ToRationalPoint4D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
Q3Vector3D_ToRationalPoint4D(const TQ3Vector3D *vector3D, TQ3RationalPoint4D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector3D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_ToRationalPoint4D(vector3D, result));
}





//=============================================================================
//      Q3RationalPoint4D_ToVector3D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3RationalPoint4D_ToVector3D(const TQ3RationalPoint4D *rationalPoint4D, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoint4D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on rationalPoint4D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint4D_ToVector3D(rationalPoint4D, result));
}





//=============================================================================
//      Q3Point2D_To3D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
Q3Point2D_To3D(const TQ3Point2D *point2D, TQ3RationalPoint3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point2D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_To3D(point2D, result));
}





//=============================================================================
//      Q3RationalPoint3D_To2D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point2D *
Q3RationalPoint3D_To2D(const TQ3RationalPoint3D *rationalPoint3D, TQ3Point2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoint3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (rationalPoint3D->w == 0.0f)
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint3D_To2D(rationalPoint3D, result));
}





//=============================================================================
//      Q3Point3D_To4D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
Q3Point3D_To4D(const TQ3Point3D *point3D, TQ3RationalPoint4D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point3D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_To4D(point3D, result));
}





//=============================================================================
//      Q3RationalPoint4D_To3D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point3D *
Q3RationalPoint4D_To3D(const TQ3RationalPoint4D *rationalPoint4D, TQ3Point3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoint4D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (rationalPoint4D->w == 0.0f)
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint4D_To3D(rationalPoint4D, result));
}





//=============================================================================
//      Q3Point2D_ToPolar : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3PolarPoint *
Q3Point2D_ToPolar(const TQ3Point2D *point2D, TQ3PolarPoint *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point2D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_ToPolar(point2D, result));
}





//=============================================================================
//      Q3PolarPoint_ToPoint2D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point2D *
Q3PolarPoint_ToPoint2D(const TQ3PolarPoint *polarPoint, TQ3Point2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polarPoint), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polarPoint
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolarPoint_ToPoint2D(polarPoint, result));
}





//=============================================================================
//      Q3Point3D_ToSpherical : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3SphericalPoint *
Q3Point3D_ToSpherical(const TQ3Point3D *point3D, TQ3SphericalPoint *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point3D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_ToSpherical(point3D, result));
}





//=============================================================================
//      Q3SphericalPoint_ToPoint3D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point3D *
Q3SphericalPoint_ToPoint3D(const TQ3SphericalPoint *sphericalPoint, TQ3Point3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(sphericalPoint), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sphericalPoint
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SphericalPoint_ToPoint3D(sphericalPoint, result));
}





//=============================================================================
//      Q3Vector2D_Dot : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
float
Q3Vector2D_Dot(const TQ3Vector2D *v1, const TQ3Vector2D *v2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on v1
		return(0.0f);

	if (0) // Further checks on v2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_Dot(v1, v2));
}





//=============================================================================
//      Q3Vector3D_Dot : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Vector3D_Dot(const TQ3Vector3D *v1, const TQ3Vector3D *v2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on v1
		return(0.0f);

	if (0) // Further checks on v2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_Dot(v1, v2));
}





//=============================================================================
//      Q3Vector2D_Cross : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
float
Q3Vector2D_Cross(const TQ3Vector2D *v1, const TQ3Vector2D *v2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on v1
		return(0.0f);

	if (0) // Further checks on v2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_Cross(v1, v2));
}





//=============================================================================
//      Q3Point2D_CrossProductTri : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Point2D_CrossProductTri(const TQ3Point2D *p1, const TQ3Point2D *p2, const TQ3Point2D *p3)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p3), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(0.0f);

	if (0) // Further checks on p2
		return(0.0f);

	if (0) // Further checks on p3
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_CrossProductTri(p1, p2, p3));
}





//=============================================================================
//      Q3Vector3D_Cross : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Vector3D_Cross(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on v1
		return(NULL);

	if (0) // Further checks on v2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_Cross(v1, v2, result));
}





//=============================================================================
//      Q3Point3D_CrossProductTri : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Point3D_CrossProductTri(const TQ3Point3D *p1, const TQ3Point3D *p2, const TQ3Point3D *p3, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p3), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(NULL);

	if (0) // Further checks on p2
		return(NULL);

	if (0) // Further checks on p3
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_CrossProductTri(p1, p2, p3, result));
}





//=============================================================================
//      Q3Vector2D_Length : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
float
Q3Vector2D_Length(const TQ3Vector2D *vector2D)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector2D
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_Length(vector2D));
}





//=============================================================================
//      Q3Vector2D_LengthSquared : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Vector2D_LengthSquared(const TQ3Vector2D *vector2D)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector2D
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_LengthSquared(vector2D));
}





//=============================================================================
//      Q3Vector3D_Length : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Vector3D_Length(const TQ3Vector3D *vector3D)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector3D
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_Length(vector3D));
}





//=============================================================================
//      Q3Vector3D_LengthSquared : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Vector3D_LengthSquared(const TQ3Vector3D *vector3D)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector3D
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_LengthSquared(vector3D));
}





//=============================================================================
//      Q3Point2D_Distance : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
float
Q3Point2D_Distance(const TQ3Point2D *p1, const TQ3Point2D *p2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(0.0f);

	if (0) // Further checks on p2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_Distance(p1, p2));
}





//=============================================================================
//      Q3Point2D_DistanceSquared : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Point2D_DistanceSquared(const TQ3Point2D *p1, const TQ3Point2D *p2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(0.0f);

	if (0) // Further checks on p2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_DistanceSquared(p1, p2));
}





//=============================================================================
//      Q3Param2D_Distance : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Param2D_Distance(const TQ3Param2D *p1, const TQ3Param2D *p2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(0.0f);

	if (0) // Further checks on p2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Param2D_Distance(p1, p2));
}





//=============================================================================
//      Q3Param2D_DistanceSquared : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Param2D_DistanceSquared(const TQ3Param2D *p1, const TQ3Param2D *p2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(0.0f);

	if (0) // Further checks on p2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Param2D_DistanceSquared(p1, p2));
}





//=============================================================================
//      Q3RationalPoint3D_Distance : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3RationalPoint3D_Distance(const TQ3RationalPoint3D *p1, const TQ3RationalPoint3D *p2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(0.0f);

	if (0) // Further checks on p2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint3D_Distance(p1, p2));
}





//=============================================================================
//      Q3RationalPoint3D_DistanceSquared : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3RationalPoint3D_DistanceSquared(const TQ3RationalPoint3D *p1, const TQ3RationalPoint3D *p2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(0.0f);

	if (0) // Further checks on p2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint3D_DistanceSquared(p1, p2));
}





//=============================================================================
//      Q3Point3D_Distance : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Point3D_Distance(const TQ3Point3D *p1, const TQ3Point3D *p2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(0.0f);

	if (0) // Further checks on p2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_Distance(p1, p2));
}





//=============================================================================
//      Q3Point3D_DistanceSquared : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Point3D_DistanceSquared(const TQ3Point3D *p1, const TQ3Point3D *p2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(0.0f);

	if (0) // Further checks on p2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_DistanceSquared(p1, p2));
}





//=============================================================================
//      Q3RationalPoint4D_Distance : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3RationalPoint4D_Distance(const TQ3RationalPoint4D *p1, const TQ3RationalPoint4D *p2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(0.0f);

	if (0) // Further checks on p2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint4D_Distance(p1, p2));
}





//=============================================================================
//      Q3RationalPoint4D_DistanceSquared : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3RationalPoint4D_DistanceSquared(const TQ3RationalPoint4D *p1, const TQ3RationalPoint4D *p2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(0.0f);

	if (0) // Further checks on p2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint4D_DistanceSquared(p1, p2));
}





//=============================================================================
//      Q3Vector2D_Negate : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
Q3Vector2D_Negate(const TQ3Vector2D *vector2D, TQ3Vector2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector2D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_Negate(vector2D, result));
}





//=============================================================================
//      Q3Vector3D_Negate : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Vector3D_Negate(const TQ3Vector3D *vector3D, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector3D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_Negate(vector3D, result));
}





//=============================================================================
//      Q3Vector2D_Scale : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
Q3Vector2D_Scale(const TQ3Vector2D *vector2D, float scalar, TQ3Vector2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector2D
		return(NULL);

	if (0) // Further checks on scalar
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_Scale(vector2D, scalar, result));
}





//=============================================================================
//      Q3Vector3D_Scale : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Vector3D_Scale(const TQ3Vector3D *vector3D, float scalar, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector3D
		return(NULL);

	if (0) // Further checks on scalar
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_Scale(vector3D, scalar, result));
}





//=============================================================================
//      Q3Vector2D_Normalize : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
Q3Vector2D_Normalize(const TQ3Vector2D *vector2D, TQ3Vector2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector2D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_Normalize(vector2D, result));
}





//=============================================================================
//      Q3Vector3D_Normalize : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Vector3D_Normalize(const TQ3Vector3D *vector3D, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector3D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_Normalize(vector3D, result));
}





//=============================================================================
//      Q3Vector2D_Add : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
Q3Vector2D_Add(const TQ3Vector2D *v1, const TQ3Vector2D *v2, TQ3Vector2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on v1
		return(NULL);

	if (0) // Further checks on v2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_Add(v1, v2, result));
}





//=============================================================================
//      Q3Vector3D_Add : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Vector3D_Add(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on v1
		return(NULL);

	if (0) // Further checks on v2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_Add(v1, v2, result));
}





//=============================================================================
//      Q3Vector2D_Subtract : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector2D *
Q3Vector2D_Subtract(const TQ3Vector2D *v1, const TQ3Vector2D *v2, TQ3Vector2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on v1
		return(NULL);

	if (0) // Further checks on v2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_Subtract(v1, v2, result));
}





//=============================================================================
//      Q3Vector3D_Subtract : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Vector3D_Subtract(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on v1
		return(NULL);

	if (0) // Further checks on v2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_Subtract(v1, v2, result));
}





//=============================================================================
//      Q3Point2D_Vector2D_Add : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Point2D *
Q3Point2D_Vector2D_Add(const TQ3Point2D *point2D, const TQ3Vector2D *vector2D, TQ3Point2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point2D
		return(NULL);

	if (0) // Further checks on vector2D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_Vector2D_Add(point2D, vector2D, result));
}





//=============================================================================
//      Q3Param2D_Vector2D_Add : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Param2D *
Q3Param2D_Vector2D_Add(const TQ3Param2D *param2D, const TQ3Vector2D *vector2D, TQ3Param2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(param2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on param2D
		return(NULL);

	if (0) // Further checks on vector2D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Param2D_Vector2D_Add(param2D, vector2D, result));
}





//=============================================================================
//      Q3Point3D_Vector3D_Add : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point3D *
Q3Point3D_Vector3D_Add(const TQ3Point3D *point3D, const TQ3Vector3D *vector3D, TQ3Point3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point3D
		return(NULL);

	if (0) // Further checks on vector3D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_Vector3D_Add(point3D, vector3D, result));
}





//=============================================================================
//      Q3Point2D_Vector2D_Subtract : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point2D *
Q3Point2D_Vector2D_Subtract(const TQ3Point2D *point2D, const TQ3Vector2D *vector2D, TQ3Point2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point2D
		return(NULL);

	if (0) // Further checks on vector2D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_Vector2D_Subtract(point2D, vector2D, result));
}





//=============================================================================
//      Q3Param2D_Vector2D_Subtract : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Param2D *
Q3Param2D_Vector2D_Subtract(const TQ3Param2D *param2D, const TQ3Vector2D *vector2D, TQ3Param2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(param2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on param2D
		return(NULL);

	if (0) // Further checks on vector2D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Param2D_Vector2D_Subtract(param2D, vector2D, result));
}





//=============================================================================
//      Q3Point3D_Vector3D_Subtract : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point3D *
Q3Point3D_Vector3D_Subtract(const TQ3Point3D *point3D, const TQ3Vector3D *vector3D, TQ3Point3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point3D
		return(NULL);

	if (0) // Further checks on vector3D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_Vector3D_Subtract(point3D, vector3D, result));
}





//=============================================================================
//      Q3Point2D_Subtract : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
Q3Point2D_Subtract(const TQ3Point2D *p1, const TQ3Point2D *p2, TQ3Vector2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(NULL);

	if (0) // Further checks on p2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_Subtract(p1, p2, result));
}





//=============================================================================
//      Q3Param2D_Subtract : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector2D *
Q3Param2D_Subtract(const TQ3Param2D *p1, const TQ3Param2D *p2, TQ3Vector2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(NULL);

	if (0) // Further checks on p2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Param2D_Subtract(p1, p2, result));
}





//=============================================================================
//      Q3Point3D_Subtract : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Point3D_Subtract(const TQ3Point3D *p1, const TQ3Point3D *p2, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(NULL);

	if (0) // Further checks on p2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_Subtract(p1, p2, result));
}





//=============================================================================
//      Q3Point2D_RRatio : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Point2D *
Q3Point2D_RRatio(const TQ3Point2D *p1, const TQ3Point2D *p2, float r1, float r2, TQ3Point2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(NULL);

	if (0) // Further checks on p2
		return(NULL);

	if ((r1 + r2) == 0.0f)
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_RRatio(p1, p2, r1, r2, result));
}





//=============================================================================
//      Q3Param2D_RRatio : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Param2D *
Q3Param2D_RRatio(const TQ3Param2D *p1, const TQ3Param2D *p2, float r1, float r2, TQ3Param2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(NULL);

	if (0) // Further checks on p2
		return(NULL);

	if (0) // Further checks on r1
		return(NULL);

	if (0) // Further checks on r2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Param2D_RRatio(p1, p2, r1, r2, result));
}





//=============================================================================
//      Q3Point3D_RRatio : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point3D *
Q3Point3D_RRatio(const TQ3Point3D *p1, const TQ3Point3D *p2, float r1, float r2, TQ3Point3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(NULL);

	if (0) // Further checks on p2
		return(NULL);

	if ((r1 + r2) == 0.0f)
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_RRatio(p1, p2, r1, r2, result));
}





//=============================================================================
//      Q3RationalPoint4D_RRatio : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
Q3RationalPoint4D_RRatio(const TQ3RationalPoint4D *p1, const TQ3RationalPoint4D *p2, float r1, float r2, TQ3RationalPoint4D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(p2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on p1
		return(NULL);

	if (0) // Further checks on p2
		return(NULL);

	if ((r1 + r2) == 0.0f)
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint4D_RRatio(p1, p2, r1, r2, result));
}





//=============================================================================
//      Q3Point2D_AffineComb : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Point2D *
Q3Point2D_AffineComb(const TQ3Point2D *points2D, const float *weights, TQ3Uns32 numPoints, TQ3Point2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(points2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(weights), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on points2D
		return(NULL);

	if (0) // Further checks on weights
		return(NULL);

	if (0) // Further checks on numPoints
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_AffineComb(points2D, weights, numPoints, result));
}





//=============================================================================
//      Q3Param2D_AffineComb : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Param2D *
Q3Param2D_AffineComb(const TQ3Param2D *params2D, const float *weights, TQ3Uns32 numPoints, TQ3Param2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(params2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(weights), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on params2D
		return(NULL);

	if (0) // Further checks on weights
		return(NULL);

	if (0) // Further checks on numPoints
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Param2D_AffineComb(params2D, weights, numPoints, result));
}





//=============================================================================
//      Q3RationalPoint3D_AffineComb : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
Q3RationalPoint3D_AffineComb(const TQ3RationalPoint3D *rationalPoints3D, const float *weights, TQ3Uns32 numPoints, TQ3RationalPoint3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoints3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(weights), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on rationalPoints3D
		return(NULL);

	if (0) // Further checks on weights
		return(NULL);

	if (0) // Further checks on numPoints
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint3D_AffineComb(rationalPoints3D, weights, numPoints, result));
}





//=============================================================================
//      Q3Point3D_AffineComb : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point3D *
Q3Point3D_AffineComb(const TQ3Point3D *points3D, const float *weights, TQ3Uns32 numPoints, TQ3Point3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(points3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(weights), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on points3D
		return(NULL);

	if (0) // Further checks on weights
		return(NULL);

	if (0) // Further checks on numPoints
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_AffineComb(points3D, weights, numPoints, result));
}





//=============================================================================
//      Q3RationalPoint4D_AffineComb : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
Q3RationalPoint4D_AffineComb(const TQ3RationalPoint4D *rationalPoints4D, const float *weights, TQ3Uns32 numPoints, TQ3RationalPoint4D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoints4D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(weights), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on rationalPoints4D
		return(NULL);

	if (0) // Further checks on weights
		return(NULL);

	if (0) // Further checks on numPoints
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint4D_AffineComb(rationalPoints4D, weights, numPoints, result));
}





//=============================================================================
//      Q3Vector2D_Transform : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
Q3Vector2D_Transform(const TQ3Vector2D *vector2D, const TQ3Matrix3x3 *matrix3x3, TQ3Vector2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector2D
		return(NULL);

	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_Transform(vector2D, matrix3x3, result));
}





//=============================================================================
//      Q3Vector3D_Transform : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Vector3D_Transform(const TQ3Vector3D *vector3D, const TQ3Matrix4x4 *matrix4x4, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector3D
		return(NULL);

	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_Transform(vector3D, matrix4x4, result));
}





//=============================================================================
//      Q3Point2D_Transform : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point2D *
Q3Point2D_Transform(const TQ3Point2D *point2D, const TQ3Matrix3x3 *matrix3x3, TQ3Point2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point2D
		return(NULL);

	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_Transform(point2D, matrix3x3, result));
}





//=============================================================================
//      Q3Param2D_Transform : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Param2D *
Q3Param2D_Transform(const TQ3Param2D *param2D, const TQ3Matrix3x3 *matrix3x3, TQ3Param2D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(param2D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on param2D
		return(NULL);

	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Param2D_Transform(param2D, matrix3x3, result));
}





//=============================================================================
//      Q3RationalPoint3D_Transform : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
Q3RationalPoint3D_Transform(const TQ3RationalPoint3D *rationalPoint3D, const TQ3Matrix3x3 *matrix3x3, TQ3RationalPoint3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoint3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on rationalPoint3D
		return(NULL);

	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint3D_Transform(rationalPoint3D, matrix3x3, result));
}





//=============================================================================
//      Q3Point3D_Transform : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point3D *
Q3Point3D_Transform(const TQ3Point3D *point3D, const TQ3Matrix4x4 *matrix4x4, TQ3Point3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point3D
		return(NULL);

	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_Transform(point3D, matrix4x4, result));
}





//=============================================================================
//      Q3RationalPoint4D_Transform : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
Q3RationalPoint4D_Transform(const TQ3RationalPoint4D *rationalPoint4D, const TQ3Matrix4x4 *matrix4x4, TQ3RationalPoint4D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoint4D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on rationalPoint4D
		return(NULL);

	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint4D_Transform(rationalPoint4D, matrix4x4, result));
}





//=============================================================================
//      Q3Vector2D_To2DTransformArray : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Vector2D_To2DTransformArray(const TQ3Vector2D *inVectors2D, const TQ3Matrix3x3 *matrix3x3, TQ3Vector2D *outVectors2D, TQ3Int32 numVectors, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(inVectors2D),  kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outVectors2D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(numVectors >= 1,            kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on inVectors2D
		return(kQ3Failure);

	if (0) // Further checks on matrix3x3
		return(kQ3Failure);

	if (0) // Further checks on outVectors2D
		return(kQ3Failure);

	if (0) // Further checks on numVectors
		return(kQ3Failure);

	if (0) // Further checks on inStructSize
		return(kQ3Failure);

	if (0) // Further checks on outStructSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector2D_To2DTransformArray(inVectors2D, matrix3x3, outVectors2D, (TQ3Uns32) numVectors, inStructSize, outStructSize));
}





//=============================================================================
//      Q3Vector3D_To3DTransformArray : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Vector3D_To3DTransformArray(const TQ3Vector3D *inVectors3D, const TQ3Matrix4x4 *matrix4x4, TQ3Vector3D *outVectors3D, TQ3Int32 numVectors, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(inVectors3D),  kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outVectors3D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(numVectors >= 1,            kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on inVectors3D
		return(kQ3Failure);

	if (0) // Further checks on matrix4x4
		return(kQ3Failure);

	if (0) // Further checks on outVectors3D
		return(kQ3Failure);

	if (0) // Further checks on numVectors
		return(kQ3Failure);

	if (0) // Further checks on inStructSize
		return(kQ3Failure);

	if (0) // Further checks on outStructSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_To3DTransformArray(inVectors3D, matrix4x4, outVectors3D, (TQ3Uns32) numVectors, inStructSize, outStructSize));
}





//=============================================================================
//      Q3Point2D_To2DTransformArray : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Point2D_To2DTransformArray(const TQ3Point2D *inPoints2D, const TQ3Matrix3x3 *matrix3x3, TQ3Point2D *outPoints2D, TQ3Int32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(inPoints2D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outPoints2D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(numPoints >= 1, kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on inPoints2D
		return(kQ3Failure);

	if (0) // Further checks on matrix3x3
		return(kQ3Failure);

	if (0) // Further checks on outPoints2D
		return(kQ3Failure);

	if (0) // Further checks on numPoints
		return(kQ3Failure);

	if (0) // Further checks on inStructSize
		return(kQ3Failure);

	if (0) // Further checks on outStructSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point2D_To2DTransformArray(inPoints2D, matrix3x3, outPoints2D, (TQ3Uns32) numPoints, inStructSize, outStructSize));
}





//=============================================================================
//      Q3RationalPoint3D_To3DTransformArray : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RationalPoint3D_To3DTransformArray(const TQ3RationalPoint3D *inRationalPoints3D, const TQ3Matrix3x3 *matrix3x3, TQ3RationalPoint3D *outRationalPoints3D, TQ3Int32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(inRationalPoints3D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outRationalPoints3D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(numPoints >= 1, kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on inRationalPoints3D
		return(kQ3Failure);

	if (0) // Further checks on matrix3x3
		return(kQ3Failure);

	if (0) // Further checks on outRationalPoints3D
		return(kQ3Failure);

	if (0) // Further checks on numPoints
		return(kQ3Failure);

	if (0) // Further checks on inStructSize
		return(kQ3Failure);

	if (0) // Further checks on outStructSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint3D_To3DTransformArray(inRationalPoints3D, matrix3x3, outRationalPoints3D, (TQ3Uns32) numPoints, inStructSize, outStructSize));
}





//=============================================================================
//      Q3Point3D_To3DTransformArray : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Point3D_To3DTransformArray(const TQ3Point3D *inPoints3D, const TQ3Matrix4x4 *matrix4x4, TQ3Point3D *outPoints3D, TQ3Int32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(inPoints3D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outPoints3D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(numPoints >= 1, kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on inPoints3D
		return(kQ3Failure);

	if (0) // Further checks on matrix4x4
		return(kQ3Failure);

	if (0) // Further checks on outPoints3D
		return(kQ3Failure);

	if (0) // Further checks on numPoints
		return(kQ3Failure);

	if (0) // Further checks on inStructSize
		return(kQ3Failure);

	if (0) // Further checks on outStructSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_To3DTransformArray(inPoints3D, matrix4x4, outPoints3D, (TQ3Uns32) numPoints, inStructSize, outStructSize));
}





//=============================================================================
//      Q3Point3D_To4DTransformArray : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Point3D_To4DTransformArray(const TQ3Point3D *inPoints3D, const TQ3Matrix4x4 *matrix4x4, TQ3RationalPoint4D *outRationalPoints4D, TQ3Int32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(inPoints3D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outRationalPoints4D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(numPoints >= 1, kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on inPoints3D
		return(kQ3Failure);

	if (0) // Further checks on matrix4x4
		return(kQ3Failure);

	if (0) // Further checks on outRationalPoints4D
		return(kQ3Failure);

	if (0) // Further checks on numPoints
		return(kQ3Failure);

	if (0) // Further checks on inStructSize
		return(kQ3Failure);

	if (0) // Further checks on outStructSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_To4DTransformArray(inPoints3D, matrix4x4, outRationalPoints4D, (TQ3Uns32) numPoints, inStructSize, outStructSize));
}





//=============================================================================
//      Q3RationalPoint4D_To4DTransformArray : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RationalPoint4D_To4DTransformArray(const TQ3RationalPoint4D *inRationalPoints4D, const TQ3Matrix4x4 *matrix4x4, TQ3RationalPoint4D *outRationalPoints4D, TQ3Int32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(inRationalPoints4D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outRationalPoints4D), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(numPoints >= 1, kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on inRationalPoints4D
		return(kQ3Failure);

	if (0) // Further checks on matrix4x4
		return(kQ3Failure);

	if (0) // Further checks on outRationalPoints4D
		return(kQ3Failure);

	if (0) // Further checks on numPoints
		return(kQ3Failure);

	if (0) // Further checks on inStructSize
		return(kQ3Failure);

	if (0) // Further checks on outStructSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RationalPoint4D_To4DTransformArray(inRationalPoints4D, matrix4x4, outRationalPoints4D, (TQ3Uns32) numPoints, inStructSize, outStructSize));
}





//=============================================================================
//      Q3Matrix3x3_SetIdentity : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Matrix3x3 *
Q3Matrix3x3_SetIdentity(TQ3Matrix3x3 *matrix3x3)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix3x3
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix3x3_SetIdentity(matrix3x3));
}





//=============================================================================
//      Q3Matrix4x4_SetIdentity : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_SetIdentity(TQ3Matrix4x4 *matrix4x4)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_SetIdentity(matrix4x4));
}





//=============================================================================
//      Q3Matrix3x3_SetTranslate : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
Q3Matrix3x3_SetTranslate(TQ3Matrix3x3 *matrix3x3, float xTrans, float yTrans)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on xTrans
		return(NULL);

	if (0) // Further checks on yTrans
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix3x3_SetTranslate(matrix3x3, xTrans, yTrans));
}





//=============================================================================
//      Q3Matrix4x4_SetTranslate : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_SetTranslate(TQ3Matrix4x4 *matrix4x4, float xTrans, float yTrans, float zTrans)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on xTrans
		return(NULL);

	if (0) // Further checks on yTrans
		return(NULL);

	if (0) // Further checks on zTrans
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_SetTranslate(matrix4x4, xTrans, yTrans, zTrans));
}





//=============================================================================
//      Q3Matrix3x3_SetScale : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
Q3Matrix3x3_SetScale(TQ3Matrix3x3 *matrix3x3, float xScale, float yScale)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on xScale
		return(NULL);

	if (0) // Further checks on yScale
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix3x3_SetScale(matrix3x3, xScale, yScale));
}





//=============================================================================
//      Q3Matrix4x4_SetScale : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_SetScale(TQ3Matrix4x4 *matrix4x4, float xScale, float yScale, float zScale)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on xScale
		return(NULL);

	if (0) // Further checks on yScale
		return(NULL);

	if (0) // Further checks on zScale
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_SetScale(matrix4x4, xScale, yScale, zScale));
}





//=============================================================================
//      Q3Matrix3x3_SetRotate : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
Q3Matrix3x3_SetRotate(TQ3Matrix3x3 *matrix3x3, float angle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on angle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix3x3_SetRotate(matrix3x3, angle));
}





//=============================================================================
//      Q3Matrix4x4_SetRotate_X : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_SetRotate_X(TQ3Matrix4x4 *matrix4x4, float angle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on angle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_SetRotate_X(matrix4x4, angle));
}





//=============================================================================
//      Q3Matrix4x4_SetRotate_Y : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_SetRotate_Y(TQ3Matrix4x4 *matrix4x4, float angle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on angle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_SetRotate_Y(matrix4x4, angle));
}





//=============================================================================
//      Q3Matrix4x4_SetRotate_Z : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_SetRotate_Z(TQ3Matrix4x4 *matrix4x4, float angle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on angle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_SetRotate_Z(matrix4x4, angle));
}





//=============================================================================
//      Q3Matrix4x4_SetRotate_XYZ : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_SetRotate_XYZ(TQ3Matrix4x4 *matrix4x4, float xAngle, float yAngle, float zAngle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on xAngle
		return(NULL);

	if (0) // Further checks on yAngle
		return(NULL);

	if (0) // Further checks on zAngle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_SetRotate_XYZ(matrix4x4, xAngle, yAngle, zAngle));
}





//=============================================================================
//      Q3Matrix3x3_SetRotateAboutPoint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
Q3Matrix3x3_SetRotateAboutPoint(TQ3Matrix3x3 *matrix3x3, const TQ3Point2D *origin, float angle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on origin
		return(NULL);

	if (0) // Further checks on angle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix3x3_SetRotateAboutPoint(matrix3x3, origin, angle));
}





//=============================================================================
//      Q3Matrix4x4_SetRotateAboutPoint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_SetRotateAboutPoint(TQ3Matrix4x4 *matrix4x4, const TQ3Point3D *origin, float xAngle, float yAngle, float zAngle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on origin
		return(NULL);

	if (0) // Further checks on xAngle
		return(NULL);

	if (0) // Further checks on yAngle
		return(NULL);

	if (0) // Further checks on zAngle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_SetRotateAboutPoint(matrix4x4, origin, xAngle, yAngle, zAngle));
}





//=============================================================================
//      Q3Matrix4x4_SetRotateAboutAxis : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_SetRotateAboutAxis(TQ3Matrix4x4 *matrix4x4, const TQ3Point3D *origin, const TQ3Vector3D *axis, float angle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(axis), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on origin
		return(NULL);

	if (0) // Further checks on axis
		return(NULL);

	if (0) // Further checks on angle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_SetRotateAboutAxis(matrix4x4, origin, axis, angle));
}





//=============================================================================
//      Q3Matrix4x4_SetRotateVectorToVector : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_SetRotateVectorToVector(TQ3Matrix4x4 *matrix4x4, const TQ3Vector3D *v1, const TQ3Vector3D *v2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v2), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on v1
		return(NULL);

	if (0) // Further checks on v2
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_SetRotateVectorToVector(matrix4x4, v1, v2));
}





//=============================================================================
//      Q3Matrix4x4_SetQuaternion : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_SetQuaternion(TQ3Matrix4x4 *matrix4x4, const TQ3Quaternion *quaternion)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on quaternion
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_SetQuaternion(matrix4x4, quaternion));
}





//=============================================================================
//      Q3Matrix3x3_Copy : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
Q3Matrix3x3_Copy(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix3x3_Copy(matrix3x3, result));
}





//=============================================================================
//      Q3Matrix4x4_Copy : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_Copy(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_Copy(matrix4x4, result));
}





//=============================================================================
//      Q3Matrix3x3_Transpose : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
Q3Matrix3x3_Transpose(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix3x3_Transpose(matrix3x3, result));
}





//=============================================================================
//      Q3Matrix4x4_Transpose : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_Transpose(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_Transpose(matrix4x4, result));
}





//=============================================================================
//      Q3Matrix3x3_Determinant : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Matrix3x3_Determinant(const TQ3Matrix3x3 *matrix3x3)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix3x3
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix3x3_Determinant(matrix3x3));
}





//=============================================================================
//      Q3Matrix4x4_Determinant : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Matrix4x4_Determinant(const TQ3Matrix4x4 *matrix4x4)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_Determinant(matrix4x4));
}





//=============================================================================
//      Q3Matrix3x3_Adjoint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
Q3Matrix3x3_Adjoint(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix3x3_Adjoint(matrix3x3, result));
}





//=============================================================================
//      Q3Matrix3x3_Invert : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
Q3Matrix3x3_Invert(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix3x3), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix3x3
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix3x3_Invert(matrix3x3, result));
}





//=============================================================================
//      Q3Matrix4x4_Invert : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_Invert(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on matrix4x4
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_Invert(matrix4x4, result));
}





//=============================================================================
//      Q3Matrix3x3_Multiply : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
Q3Matrix3x3_Multiply(const TQ3Matrix3x3 *m1, const TQ3Matrix3x3 *m2, TQ3Matrix3x3 *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(m1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(m2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on m1
		return(NULL);

	if (0) // Further checks on m2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix3x3_Multiply(m1, m2, result));
}





//=============================================================================
//      Q3Matrix4x4_Multiply : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
Q3Matrix4x4_Multiply(const TQ3Matrix4x4 *m1, const TQ3Matrix4x4 *m2, TQ3Matrix4x4 *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(m1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(m2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on m1
		return(NULL);

	if (0) // Further checks on m2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Matrix4x4_Multiply(m1, m2, result));
}





//=============================================================================
//      Q3Quaternion_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Quaternion *
Q3Quaternion_Set(TQ3Quaternion *quaternion, float w, float x, float y, float z)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on w
		return(NULL);

	if (0) // Further checks on x
		return(NULL);

	if (0) // Further checks on y
		return(NULL);

	if (0) // Further checks on z
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_Set(quaternion, w, x, y, z));
}





//=============================================================================
//      Q3Quaternion_SetIdentity : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetIdentity(TQ3Quaternion *quaternion)
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
	return(E3Quaternion_SetIdentity(quaternion));
}





//=============================================================================
//      Q3Quaternion_SetRotate_X : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetRotate_X(TQ3Quaternion *quaternion, float angle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on angle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_SetRotate_X(quaternion, angle));
}





//=============================================================================
//      Q3Quaternion_SetRotate_Y : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetRotate_Y(TQ3Quaternion *quaternion, float angle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on angle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_SetRotate_Y(quaternion, angle));
}





//=============================================================================
//      Q3Quaternion_SetRotate_Z : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetRotate_Z(TQ3Quaternion *quaternion, float angle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on angle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_SetRotate_Z(quaternion, angle));
}





//=============================================================================
//      Q3Quaternion_SetRotate_XYZ : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetRotate_XYZ(TQ3Quaternion *quaternion, float xAngle, float yAngle, float zAngle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on xAngle
		return(NULL);

	if (0) // Further checks on yAngle
		return(NULL);

	if (0) // Further checks on zAngle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_SetRotate_XYZ(quaternion, xAngle, yAngle, zAngle));
}





//=============================================================================
//      Q3Quaternion_SetRotateAboutAxis : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetRotateAboutAxis(TQ3Quaternion *quaternion, const TQ3Vector3D *axis, float angle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(axis), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on axis
		return(NULL);

	if (0) // Further checks on angle
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_SetRotateAboutAxis(quaternion, axis, angle));
}





//=============================================================================
//      Q3Quaternion_SetRotateVectorToVector : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetRotateVectorToVector(TQ3Quaternion *quaternion, const TQ3Vector3D *v1, const TQ3Vector3D *v2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(v2), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on v1
		return(NULL);

	if (0) // Further checks on v2
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_SetRotateVectorToVector(quaternion, v1, v2));
}





//=============================================================================
//      Q3Quaternion_SetMatrix : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetMatrix(TQ3Quaternion *quaternion, const TQ3Matrix4x4 *matrix4x4)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(matrix4x4), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on matrix4x4
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_SetMatrix(quaternion, matrix4x4));
}





//=============================================================================
//      Q3Quaternion_Copy : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_Copy(const TQ3Quaternion *quaternion, TQ3Quaternion *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_Copy(quaternion, result));
}





//=============================================================================
//      Q3Quaternion_IsIdentity : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Quaternion_IsIdentity(const TQ3Quaternion *quaternion)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_IsIdentity(quaternion));
}





//=============================================================================
//      Q3Quaternion_Dot : Quesa API entry point.
//-----------------------------------------------------------------------------
float
Q3Quaternion_Dot(const TQ3Quaternion *q1, const TQ3Quaternion *q2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(q1), 0.0f);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(q2), 0.0f);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on q1
		return(0.0f);

	if (0) // Further checks on q2
		return(0.0f);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_Dot(q1, q2));
}





//=============================================================================
//      Q3Quaternion_Normalize : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_Normalize(const TQ3Quaternion *quaternion, TQ3Quaternion *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_Normalize(quaternion, result));
}





//=============================================================================
//      Q3Quaternion_Invert : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_Invert(const TQ3Quaternion *quaternion, TQ3Quaternion *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_Invert(quaternion, result));
}





//=============================================================================
//      Q3Quaternion_Multiply : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_Multiply(const TQ3Quaternion *q1, const TQ3Quaternion *q2, TQ3Quaternion *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(q1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(q2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on q1
		return(NULL);

	if (0) // Further checks on q2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_Multiply(q1, q2, result));
}





//=============================================================================
//      Q3Quaternion_MatchReflection : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_MatchReflection(const TQ3Quaternion *q1, const TQ3Quaternion *q2, TQ3Quaternion *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(q1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(q2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on q1
		return(NULL);

	if (0) // Further checks on q2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_MatchReflection(q1, q2, result));
}





//=============================================================================
//      Q3Quaternion_InterpolateFast : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_InterpolateFast(const TQ3Quaternion *q1, const TQ3Quaternion *q2, float t, TQ3Quaternion *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(q1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(q2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on q1
		return(NULL);

	if (0) // Further checks on q2
		return(NULL);

	if (0) // Further checks on t
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_InterpolateFast(q1, q2, t, result));
}





//=============================================================================
//      Q3Quaternion_InterpolateLinear : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_InterpolateLinear(const TQ3Quaternion *q1, const TQ3Quaternion *q2, float t, TQ3Quaternion *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(q1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(q2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on q1
		return(NULL);

	if (0) // Further checks on q2
		return(NULL);

	if (0) // Further checks on t
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_InterpolateLinear(q1, q2, t, result));
}



//=============================================================================
//      Q3Quaternion_GetAxisAndAngle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Quaternion_GetAxisAndAngle(const TQ3Quaternion *quaternion, TQ3Vector3D *outAxis, float *outAngle)
{
	float qlength;

	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);



	// Debug build checks
#if Q3_DEBUG
	// Further checks on quaternion
	qlength = (float) sqrt(E3Quaternion_Dot(quaternion, quaternion));
	if (qlength < 1.0f - kQ3RealZero || qlength > 1.0f + kQ3RealZero)
		return(NULL);			// quaternion must be normalized

	if (0) // Further checks on outAngle
		return(NULL);

	if (0) // Further checks on outAxis
		return(NULL);

#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Quaternion_GetAxisAndAngle(quaternion, outAxis, outAngle));
}


//=============================================================================
//      Q3Vector3D_TransformQuaternion : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Vector3D *
Q3Vector3D_TransformQuaternion(const TQ3Vector3D *vector3D, const TQ3Quaternion *quaternion, TQ3Vector3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vector3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vector3D
		return(NULL);

	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Vector3D_TransformQuaternion(vector3D, quaternion, result));
}





//=============================================================================
//      Q3Point3D_TransformQuaternion : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Point3D *
Q3Point3D_TransformQuaternion(const TQ3Point3D *point3D, const TQ3Quaternion *quaternion, TQ3Point3D *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(quaternion), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point3D
		return(NULL);

	if (0) // Further checks on quaternion
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point3D_TransformQuaternion(point3D, quaternion, result));
}





//=============================================================================
//      Q3BoundingBox_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3BoundingBox *
Q3BoundingBox_Set(TQ3BoundingBox *bBox, const TQ3Point3D *min, const TQ3Point3D *max, TQ3Boolean isEmpty)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bBox), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(min), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(max), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bBox
		return(NULL);

	// Further checks on min and max
	if (min->x > max->x ||
		min->y > max->y ||
		min->z > max->z)
		return(NULL);

	if (0) // Further checks on isEmpty
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingBox_Set(bBox, min, max, isEmpty));
}





//=============================================================================
//      Q3BoundingBox_SetFromPoints3D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
Q3BoundingBox_SetFromPoints3D(TQ3BoundingBox *bBox, const TQ3Point3D *points3D, TQ3Uns32 numPoints, TQ3Uns32 structSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bBox),     NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(points3D), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bBox
		return(NULL);

	if (0) // Further checks on points3D
		return(NULL);

	if (0) // Further checks on numPoints
		return(NULL);

	if (0) // Further checks on structSize
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingBox_SetFromPoints3D(bBox, points3D, numPoints, structSize));
}





//=============================================================================
//      Q3BoundingBox_SetFromRationalPoints4D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
Q3BoundingBox_SetFromRationalPoints4D(TQ3BoundingBox *bBox, const TQ3RationalPoint4D *rationalPoints4D, TQ3Uns32 numPoints, TQ3Uns32 structSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bBox),     NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoints4D), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bBox
		return(NULL);

	if (0) // Further checks on rationalPoints4D
		return(NULL);

	if (0) // Further checks on numPoints
		return(NULL);

	if (0) // Further checks on structSize
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingBox_SetFromRationalPoints4D(bBox, rationalPoints4D, numPoints, structSize));
}





//=============================================================================
//      Q3BoundingBox_Copy : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
Q3BoundingBox_Copy(const TQ3BoundingBox *bBox, TQ3BoundingBox *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bBox), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bBox
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingBox_Copy(bBox, result));
}





//=============================================================================
//      Q3BoundingBox_Union : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
Q3BoundingBox_Union(const TQ3BoundingBox *b1, const TQ3BoundingBox *b2, TQ3BoundingBox *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(b1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(b2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on b1
		return(NULL);

	if (0) // Further checks on b2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingBox_Union(b1, b2, result));
}





//=============================================================================
//      Q3BoundingBox_UnionPoint3D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
Q3BoundingBox_UnionPoint3D(const TQ3BoundingBox *bBox, const TQ3Point3D *point3D, TQ3BoundingBox *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bBox), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bBox
		return(NULL);

	if (0) // Further checks on point3D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingBox_UnionPoint3D(bBox, point3D, result));
}





//=============================================================================
//      Q3BoundingBox_UnionRationalPoint4D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
Q3BoundingBox_UnionRationalPoint4D(const TQ3BoundingBox *bBox, const TQ3RationalPoint4D *rationalPoint4D, TQ3BoundingBox *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bBox), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoint4D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bBox
		return(NULL);

	if (0) // Further checks on rationalPoint4D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingBox_UnionRationalPoint4D(bBox, rationalPoint4D, result));
}





//=============================================================================
//      Q3BoundingSphere_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3BoundingSphere *
Q3BoundingSphere_Set(TQ3BoundingSphere *bSphere, const TQ3Point3D *origin, float radius, TQ3Boolean isEmpty)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bSphere), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bSphere
		return(NULL);

	if (0) // Further checks on origin
		return(NULL);

	if (0) // Further checks on radius
		return(NULL);

	if (0) // Further checks on isEmpty
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingSphere_Set(bSphere, origin, radius, isEmpty));
}





//=============================================================================
//      Q3BoundingSphere_SetFromPoints3D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
Q3BoundingSphere_SetFromPoints3D(TQ3BoundingSphere *bSphere, const TQ3Point3D *points3D, TQ3Uns32 numPoints, TQ3Uns32 structSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bSphere),  NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(points3D), NULL);
	Q3_REQUIRE_OR_RESULT(numPoints >= 1,         NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bSphere
		return(NULL);

	if (0) // Further checks on points3D
		return(NULL);

	if (0) // Further checks on numPoints
		return(NULL);

	if (0) // Further checks on structSize
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingSphere_SetFromPoints3D(bSphere, points3D, numPoints, structSize));
}





//=============================================================================
//      Q3BoundingSphere_SetFromRationalPoints4D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
Q3BoundingSphere_SetFromRationalPoints4D(TQ3BoundingSphere *bSphere, const TQ3RationalPoint4D *rationalPoints4D, TQ3Uns32 numPoints, TQ3Uns32 structSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bSphere),  NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoints4D), NULL);
	Q3_REQUIRE_OR_RESULT(numPoints >= 1,         NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bSphere
		return(NULL);

	if (0) // Further checks on rationalPoints4D
		return(NULL);

	if (0) // Further checks on numPoints
		return(NULL);

	if (0) // Further checks on structSize
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingSphere_SetFromRationalPoints4D(bSphere, rationalPoints4D, numPoints, structSize));
}





//=============================================================================
//      Q3BoundingSphere_Copy : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
Q3BoundingSphere_Copy(const TQ3BoundingSphere *bSphere, TQ3BoundingSphere *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bSphere), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bSphere
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingSphere_Copy(bSphere, result));
}





//=============================================================================
//      Q3BoundingSphere_Union : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
Q3BoundingSphere_Union(const TQ3BoundingSphere *s1, const TQ3BoundingSphere *s2, TQ3BoundingSphere *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(s1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(s2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on s1
		return(NULL);

	if (0) // Further checks on s2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingSphere_Union(s1, s2, result));
}





//=============================================================================
//      Q3BoundingSphere_UnionPoint3D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
Q3BoundingSphere_UnionPoint3D(const TQ3BoundingSphere *bSphere, const TQ3Point3D *point3D, TQ3BoundingSphere *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bSphere), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point3D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bSphere
		return(NULL);

	if (0) // Further checks on point3D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingSphere_UnionPoint3D(bSphere, point3D, result));
}





//=============================================================================
//      Q3BoundingSphere_UnionRationalPoint4D : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
Q3BoundingSphere_UnionRationalPoint4D(const TQ3BoundingSphere *bSphere, const TQ3RationalPoint4D *rationalPoint4D, TQ3BoundingSphere *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bSphere), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rationalPoint4D), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bSphere
		return(NULL);

	if (0) // Further checks on rationalPoint4D
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BoundingSphere_UnionRationalPoint4D(bSphere, rationalPoint4D, result));
}





//=============================================================================
//      Q3Ray3D_IntersectSphere : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Ray3D_IntersectSphere(const TQ3Ray3D *theRay, const TQ3Sphere *theSphere, TQ3Point3D *hitPoint)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRay), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theSphere), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(hitPoint), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theRay
		return(kQ3False);

	if (0) // Further checks on theSphere
		return(kQ3False);

	if (0) // Further checks on hitPoint
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ray3D_IntersectSphere(theRay, theSphere, hitPoint));
}





//=============================================================================
//      Q3Ray3D_IntersectBoundingBox : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Ray3D_IntersectBoundingBox(const TQ3Ray3D *theRay, const TQ3BoundingBox *theBounds, TQ3Point3D *hitPoint)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRay), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theBounds), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(hitPoint), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theRay
		return(kQ3False);

	if (0) // Further checks on theBounds
		return(kQ3False);

	if (0) // Further checks on hitPoint
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ray3D_IntersectBoundingBox(theRay, theBounds, hitPoint));
}





//=============================================================================
//      Q3Ray3D_IntersectTriangle : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Ray3D_IntersectTriangle(const TQ3Ray3D *theRay, const TQ3Point3D *point1, const TQ3Point3D *point2, const TQ3Point3D *point3, TQ3Boolean cullBackfacing, TQ3Param3D *hitPoint)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRay), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point1), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point2), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point3), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(hitPoint), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theRay
		return(kQ3False);

	if (0) // Further checks on point1
		return(kQ3False);

	if (0) // Further checks on point2
		return(kQ3False);

	if (0) // Further checks on point3
		return(kQ3False);

	if (0) // Further checks on hitPoint
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ray3D_IntersectTriangle(theRay, point1, point2, point3, cullBackfacing, hitPoint));
}


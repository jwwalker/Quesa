/*  NAME:
        E3Math.c

    DESCRIPTION:
        Math routines for Quesa.
        
        Note that these routines are allowed to call other E3foo routines for
        speed, to avoid to trip back out through the Q3foo interface.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Math.h"
#include "E3Utils.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Point2D_Set : Sets a 2D point.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3Point2D_Set(TQ3Point2D *point2D, float x, float y)
{
	point2D->x = x;
	point2D->y = y;
	return(point2D);
}





//=============================================================================
//      E3Param2D_Set : Sets a u,v parameter.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_Set(TQ3Param2D *param2D, float u, float v)
{
	param2D->u = u;
	param2D->v = v;
	return(param2D);
}





//=============================================================================
//      E3Point3D_Set : Sets a 3D point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_Set(TQ3Point3D *point3D, float x, float y, float z)
{
	point3D->x = x;
	point3D->y = y;
	point3D->z = z;
	return(point3D);
}





//=============================================================================
//      E3RationalPoint3D_Set : sets a RationalPoint3D.
//-----------------------------------------------------------------------------
//		Note : This is an extended 2D point used with 2D transformations.
//			   Untested.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
E3RationalPoint3D_Set(TQ3RationalPoint3D *point3D, float x, float y, float w)
{
	point3D->x = x;
	point3D->y = y;
	point3D->w = w;
	return(point3D);
}





//=============================================================================
//      E3RationalPoint4D_Set : sets a RationalPoint4D.
//-----------------------------------------------------------------------------
//		Note : This is an extended 3D point used with 3D transformations.
//			   Untested.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
E3RationalPoint4D_Set(TQ3RationalPoint4D *point4D, float x, float y, float z, float w)
{
	point4D->x = x;
	point4D->y = y;
	point4D->z = z;
	point4D->w = w;
	return(point4D);
}





//=============================================================================
//      E3Vector2D_Set : Sets a 2D vector.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Vector2D_Set(TQ3Vector2D *vector2D, float x, float y)
{
	vector2D->x = x;
	vector2D->y = y;
	return(vector2D);
}





//=============================================================================
//      E3Vector3D_Set : Sets a 3D vector.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_Set(TQ3Vector3D *vector3D, float x, float y, float z)
{
	vector3D->x = x;
	vector3D->y = y;
	vector3D->z = z;
	return(vector3D);
}





//=============================================================================
//      E3PolarPoint_Set : Sets a polar point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3PolarPoint *
E3PolarPoint_Set(TQ3PolarPoint *polarPoint, float r, float theta)
{
	polarPoint->r = r;
	polarPoint->theta = theta;
	return(polarPoint);
}





//=============================================================================
//      E3SphericalPoint_Set : Sets a spherical point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3SphericalPoint *
E3SphericalPoint_Set(TQ3SphericalPoint *sphericalPoint, float rho, float theta, float phi)
{
	sphericalPoint->rho   = rho;
	sphericalPoint->theta = theta;
	sphericalPoint->phi   = phi;
	return(sphericalPoint);
}





//=============================================================================
//      E3Point2D_To3D : Converts a 2D point to 3D, setting z to 1.
//-----------------------------------------------------------------------------
//		Note :	This is probably a bug on Apple's part -- this function sets
//				the "z" parameter to 1.  It looks like, by comparison with
//				QDPoint3D_To4D, that the parameter and return type should have
//				been Q3RationalPoint3D, not a Q3Point3D, and it should have
//				been "w" set to 1, not "z".
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point2D_To3D(const TQ3Point2D *point2D, TQ3Point3D *result)
{
	result->x = point2D->x;
	result->y = point2D->y;
	result->z = 1.0f;
	return(result);
}





//=============================================================================
//      E3RationalPoint3D_To2D : Converts 3D point to 2D.
//-----------------------------------------------------------------------------
//		Note : Does this by dividing x and y by w.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3RationalPoint3D_To2D(const TQ3RationalPoint3D *point3D, TQ3Point2D *result)
{
	float invw = 1.0f / point3D->w;
	result->x = point3D->x * invw;
	result->y = point3D->y * invw;
	return(result);
}





//============================================================================
//      E3Point3D_To4D : Converts a 3D point to rational 4D, setting w to 1.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
E3Point3D_To4D(const TQ3Point3D *point3D, TQ3RationalPoint4D *result)
{
	result->x = point3D->x;
	result->y = point3D->y;
	result->z = point3D->z;
	result->w = 1.0f;
	return(result);
}





//=============================================================================
//      E3RationalPoint4D_To3D : Converts rational 4D to 3D, dividing by w.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3RationalPoint4D_To3D(const TQ3RationalPoint4D *point4D, TQ3Point3D *result)
{
	float invw = 1.0f / point4D->w;
	result->x = point4D->x * invw;
	result->y = point4D->y * invw;
	result->z = point4D->z * invw;
	return(result);
}





//=============================================================================
//      E3Vector2D_To3D : Converts 2D vector to 3D, setting z to 1.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector2D_To3D(const TQ3Vector2D *vector2D, TQ3Vector3D *result)
{
	result->x = vector2D->x;
	result->y = vector2D->y;
	result->z = 1.0f;
	return(result);
}





//=============================================================================
//      E3Vector3D_To2D : 3D vector to 2D.
//-----------------------------------------------------------------------------
//		Note : Does this by dividing x and y by z.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Vector3D_To2D(const TQ3Vector3D *vector3D, TQ3Vector2D *result)
{
	float invw = 1.0f / vector3D->z;
	result->x = vector3D->x * invw;
	result->y = vector3D->y * invw;
	return(result);
}





//=============================================================================
//      E3Point2D_Subtract : Subtracts p2 from p1, storing in result.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Point2D_Subtract(const TQ3Point2D *p1, const TQ3Point2D *p2, TQ3Vector2D *result)
{
	result->x = p1->x - p2->x;
	result->y = p1->y - p2->y;
	return(result);
}





//=============================================================================
//      E3Param2D_Subtract : Subtracts p2 from p1, storing in result.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Param2D_Subtract(const TQ3Param2D *p1, const TQ3Param2D *p2, TQ3Vector2D *result)
{
	result->x = p1->u - p2->u;
	result->y = p1->v - p2->v;
	return(result);
}





//=============================================================================
//      E3Point3D_Subtract : Subtract a point from another.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Point3D_Subtract(const TQ3Point3D *p1, const TQ3Point3D *p2, TQ3Vector3D *result)
{
	// Subtract p2 from p1
	result->x = p1->x - p2->x;
	result->y = p1->y - p2->y;
	result->z = p1->z - p2->z;

	return(result);
}





//=============================================================================
//      E3Point2D_Distance : Return the Euclidean distance between two points.
//-----------------------------------------------------------------------------
float
E3Point2D_Distance(const TQ3Point2D *p1, const TQ3Point2D *p2)
{
	float a = p1->x - p2->x, b = p1->y - p2->y;
	return (float)sqrt(a*a + b*b);
}





//=============================================================================
//      E3Point2D_DistanceSquared : Return the squared Euclidean distance.
//-----------------------------------------------------------------------------
float
E3Point2D_DistanceSquared(const TQ3Point2D *p1, const TQ3Point2D *p2)
{
	float a = p1->x - p2->x, b = p1->y - p2->y;
	return(a*a + b*b);
}





//=============================================================================
//      E3Param2D_Distance : Return the Euclidean distance between two points.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
float
E3Param2D_Distance(const TQ3Param2D *p1, const TQ3Param2D *p2)
{
	float a = p1->u - p2->u, b = p1->v - p2->v;
	return (float)sqrt(a*a + b*b);
}





//=============================================================================
//      E3Param2D_DistanceSquared : Return the squared Euclidean distance.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
float
E3Param2D_DistanceSquared(const TQ3Param2D *p1, const TQ3Param2D *p2)
{
	float a = p1->u - p2->u, b = p1->v - p2->v;
	return(a*a + b*b);
}





//=============================================================================
//      E3RationalPoint3D_Distance : Return the Euclidean distance.
//-----------------------------------------------------------------------------
float
E3RationalPoint3D_Distance(const TQ3RationalPoint3D *p1, const
TQ3RationalPoint3D *p2)
{
	float a = p1->x - p2->x, b = p1->y - p2->y, c = p1->w - p2->w;
	return (float)sqrt(a*a + b*b + c*c);
}





//=============================================================================
//      E3RationalPoint3D_DistanceSquared :	Return the squared Euclidean
//											distance.
//-----------------------------------------------------------------------------
float
E3RationalPoint3D_DistanceSquared(const TQ3RationalPoint3D *p1,
								  const TQ3RationalPoint3D *p2)
{
	float a = p1->x - p2->x, b = p1->y - p2->y, c = p1->w - p2->w;
	return(a*a + b*b + c*c);
}





//=============================================================================
//      E3Point3D_Distance : Return the 3D Euclidean distance.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
float
E3Point3D_Distance(const TQ3Point3D *p1, const TQ3Point3D *p2)
{
	float a = p1->x - p2->x, b = p1->y - p2->y, c = p1->z - p2->z;
	return (float)sqrt(a*a + b*b + c*c);
}





//=============================================================================
//      E3Point3D_DistanceSquared : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
float
E3Point3D_DistanceSquared(const TQ3Point3D *p1, const TQ3Point3D *p2)
{
	float a = p1->x - p2->x, b = p1->y - p2->y, c = p1->z - p2->z;
	return(a*a + b*b + c*c);
}





//=============================================================================
//      E3RationalPoint4D_Distance : One-line description of the method.
//-----------------------------------------------------------------------------
float
E3RationalPoint4D_Distance(const TQ3RationalPoint4D *p1,
						   const TQ3RationalPoint4D *p2)
{
	float a = p1->x - p2->x, b = p1->y - p2->y, c = p1->z - p2->z, d = p1->w - p2->w;
	return (float)sqrt(a*a + b*b + c*c + d*d);
}





//=============================================================================
//      E3RationalPoint4D_DistanceSquared : One-line description of the method.
//-----------------------------------------------------------------------------
float
E3RationalPoint4D_DistanceSquared(const TQ3RationalPoint4D *p1,
								  const TQ3RationalPoint4D *p2)
{
	float a = p1->x - p2->x, b = p1->y - p2->y, c = p1->z - p2->z, d = p1->w - p2->w;
	return(a*a + b*b + c*c + d*d);
}





//=============================================================================
//      E3Point2D_RRatio :	Return the point along the segment from p1 to p2
//							at the ratio r2/(r1+r2).
//-----------------------------------------------------------------------------
//		Note :	The QD3D docs claim that the ratio used is r1/(r1+r2), but
//				we found by direct experimentation that the QD3D library (1.6)
//				in fact uses r2/(r1+r2) instead.  As usual, we do as QD3D does,
//				not as the docs say.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3Point2D_RRatio(const TQ3Point2D *p1, const TQ3Point2D *p2, float r1, float r2, TQ3Point2D *result)
{
	float frac = r2/(r1+r2);
	result->x = p1->x + frac * (p2->x - p1->x);
	result->y = p1->y + frac * (p2->y - p1->y);
	return(result);
}





//=============================================================================
//      E3Param2D_RRatio :	Return the point along the segment from p1 to p2
//							at the ratio r2/(r1+r2).
//-----------------------------------------------------------------------------
//		Note :	The QD3D docs claim that the ratio used is r1/(r1+r2), but
//				we found by direct experimentation that the QD3D library (1.6)
//				in fact uses r2/(r1+r2) instead.  As usual, we do as QD3D does,
//				not as the docs say.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_RRatio(const TQ3Param2D *p1, const TQ3Param2D *p2, float r1, float r2, TQ3Param2D *result)
{
	float frac = r2/(r1+r2);
	result->u = p1->u + frac * (p2->u - p1->u);
	result->v = p1->v + frac * (p2->v - p1->v);
	return(result);
}





//=============================================================================
//      E3Point3D_RRatio :	Return the point along the segment from p1 to p2
//							at the ratio r2/(r1+r2).
//-----------------------------------------------------------------------------
//		Note :	The QD3D docs claim that the ratio used is r1/(r1+r2), but
//				we found by direct experimentation that the QD3D library (1.6)
//				in fact uses r2/(r1+r2) instead.  As usual, we do as QD3D does,
//				not as the docs say.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_RRatio(const TQ3Point3D *p1, const TQ3Point3D *p2, float r1, float r2, TQ3Point3D *result)
{
	float frac = r2/(r1+r2);
	result->x = p1->x + frac * (p2->x - p1->x);
	result->y = p1->y + frac * (p2->y - p1->y);
	result->z = p1->z + frac * (p2->z - p1->z);
	return(result);
}





//=============================================================================
//      E3RationalPoint4D_RRatio :	Return the point along the segment from
//									p1 to p2 at the ratio r2/(r1+r2).
//-----------------------------------------------------------------------------
//		Note :	The QD3D docs claim that the ratio used is r1/(r1+r2), but
//				we found by direct experimentation that the QD3D library (1.6)
//				in fact uses r2/(r1+r2) instead.  As usual, we do as QD3D does,
//				not as the docs say.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
E3RationalPoint4D_RRatio(const TQ3RationalPoint4D *p1, const TQ3RationalPoint4D *p2, float r1, float r2, TQ3RationalPoint4D *result)
{
	float frac = r2/(r1+r2);
	result->x = p1->x + frac * (p2->x - p1->x);
	result->y = p1->y + frac * (p2->y - p1->y);
	result->z = p1->z + frac * (p2->z - p1->z);
	result->w = p1->w + frac * (p2->w - p1->w);
	return(result);
}





//=============================================================================
//      E3Point2D_Vector2D_Add : Add a 2D vector to a point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3Point2D_Vector2D_Add(const TQ3Point2D *point2D, const TQ3Vector2D *vector2D, TQ3Point2D *result)
{
	result->x = point2D->x + vector2D->x;
	result->y = point2D->y + vector2D->y;
	return(result);
}





//=============================================================================
//      E3Param2D_Vector2D_Add : Add a 2D vector to a 2D param.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_Vector2D_Add(const TQ3Param2D *param2D, const TQ3Vector2D *vector2D, TQ3Param2D *result)
{
	result->u = param2D->u + vector2D->x;
	result->v = param2D->v + vector2D->y;
	return(result);
}





//=============================================================================
//      E3Point3D_Vector3D_Add : Add a 3D vector to a point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_Vector3D_Add(const TQ3Point3D *point3D, const TQ3Vector3D *vector3D, TQ3Point3D *result)
{
	result->x = point3D->x + vector3D->x;
	result->y = point3D->y + vector3D->y;
	result->z = point3D->z + vector3D->z;
	return(result);
}





//=============================================================================
//      E3Point2D_Vector2D_Subtract : Subtract a 2D vector from a point.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3Point2D_Vector2D_Subtract(const TQ3Point2D *point2D, const TQ3Vector2D *vector2D, TQ3Point2D *result)
{
	result->x = point2D->x - vector2D->x;
	result->y = point2D->y - vector2D->y;
	return(result);
}





//=============================================================================
//      E3Param2D_Vector2D_Subtract : Subtract a 2D vector from a param.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_Vector2D_Subtract(const TQ3Param2D *param2D, const TQ3Vector2D *vector2D, TQ3Param2D *result)
{
	result->u = param2D->u - vector2D->x;
	result->v = param2D->v - vector2D->y;
	return(result);
}





//=============================================================================
//      E3Point3D_Vector3D_Subtract : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_Vector3D_Subtract(const TQ3Point3D *point3D, const TQ3Vector3D *vector3D, TQ3Point3D *result)
{
	result->x = point3D->x - vector3D->x;
	result->y = point3D->y - vector3D->y;
	result->z = point3D->z - vector3D->z;
	return(result);
}





//=============================================================================
//      E3Vector2D_Scale : Scale a 2D vector by the given factor.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Vector2D_Scale(const TQ3Vector2D *vector2D, float scalar, TQ3Vector2D *result)
{
	result->x = vector2D->x * scalar;
	result->y = vector2D->y * scalar;
	return(result);
}





//=============================================================================
//      E3Vector3D_Scale : Scale a 3D vector by the given factor.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_Scale(const TQ3Vector3D *vector3D, float scalar, TQ3Vector3D *result)
{
	result->x = vector3D->x * scalar;
	result->y = vector3D->y * scalar;
	result->z = vector3D->z * scalar;
	return(result);
}





//=============================================================================
//      E3Vector2D_Length : Returns the length of a 2D vector.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
float
E3Vector2D_Length(const TQ3Vector2D *vector2D)
{
	return (float)sqrt(vector2D->x * vector2D->x + vector2D->y * vector2D->y);
}





//=============================================================================
//      E3Vector3D_Length : Returns the length of a 3D vector.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
float
E3Vector3D_Length(const TQ3Vector3D *vector3D)
{
	return (float)sqrt(vector3D->x * vector3D->x + vector3D->y * vector3D->y + vector3D->z * vector3D->z);
}





//=============================================================================
//      E3Vector2D_Normalize :	Scales the given vector so that its new length
//								is 1.0f.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Vector2D_Normalize(const TQ3Vector2D *vector2D, TQ3Vector2D *result)
{
	float invlen = 1.0f / (float)sqrt(vector2D->x * vector2D->x + vector2D->y * vector2D->y);
	result->x = vector2D->x * invlen;
	result->y = vector2D->y * invlen;
	return(result);
}





//=============================================================================
//      E3Vector3D_Normalize :	Scales the given vector so that its new length
//								is 1.0f.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_Normalize(const TQ3Vector3D *vector3D, TQ3Vector3D *result)
{
	float invlen = 1.0f / (float)sqrt(vector3D->x * vector3D->x + vector3D->y * vector3D->y + vector3D->z * vector3D->z);
	result->x = vector3D->x * invlen;
	result->y = vector3D->y * invlen;
	result->z = vector3D->z * invlen;
	return(result);
}





//=============================================================================
//      E3Vector2D_Add : Add two 2D vectors.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Vector2D_Add(const TQ3Vector2D *v1, const TQ3Vector2D *v2, TQ3Vector2D *result)
{
	result->x = v1->x + v2->x;
	result->y = v1->y + v2->y;
	return(result);
}





//=============================================================================
//      E3Vector3D_Add : Add two 3D vectors.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_Add(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result)
{
	result->x = v1->x + v2->x;
	result->y = v1->y + v2->y;
	result->z = v1->z + v2->z;
	return(result);
}





//=============================================================================
//      E3Vector2D_Subtract : Subtract vector v2 from v1.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Vector2D_Subtract(const TQ3Vector2D *v1, const TQ3Vector2D *v2, TQ3Vector2D *result)
{
	result->x = v1->x - v2->x;
	result->y = v1->y - v2->y;
	return(result);
}





//=============================================================================
//      E3Vector3D_Subtract : Subtract vector v2 from v1.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_Subtract(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result)
{
	result->x = v1->x - v2->x;
	result->y = v1->y - v2->y;
	result->z = v1->z - v2->z;
	return(result);
}





//=============================================================================
//      E3Vector2D_Cross : Returns the 2D cross product of v1 and v2.
//-----------------------------------------------------------------------------
//		Note :	We assume the 2D vectors are really 3D vectors with z=0,
//				then return the z coordinate of the cross product (0,0,z).
//-----------------------------------------------------------------------------
float
E3Vector2D_Cross(const TQ3Vector2D *v1, const TQ3Vector2D *v2)
{
	return(v1->x * v2->y - v1->y * v2->x);
}





//=============================================================================
//      E3Vector3D_Cross : Calculate a cross product.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_Cross(const TQ3Vector3D *v1, const TQ3Vector3D *v2, TQ3Vector3D *result)
{
	// Return the cross product of v1 and v2
	result->x = (v1->y * v2->z) - (v1->z * v2->y);
	result->y = (v1->z * v2->x) - (v1->x * v2->z);
	result->z = (v1->x * v2->y) - (v1->y * v2->x);

	return(result);
}





//=============================================================================
//      E3Point3D_CrossProductTri : Calculate a cross product for a triangle.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Point3D_CrossProductTri(const TQ3Point3D *point1, const TQ3Point3D *point2, const TQ3Point3D *point3, TQ3Vector3D *crossVector)
{	TQ3Vector3D		v1, v2;

	// Calculate our vectors
	E3Point3D_Subtract(point1, point2, &v1);
	E3Point3D_Subtract(point1, point3, &v2);

	// Return the cross product
	return(E3Vector3D_Cross(&v1, &v2, crossVector));
}





//=============================================================================
//      E3Vector2D_Dot : Return the dot product of 2D vectors.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
float
E3Vector2D_Dot(const TQ3Vector2D *v1, const TQ3Vector2D *v2)
{
	return(v1->x * v2->x + v1->y * v2->y);
}





//=============================================================================
//      E3Vector3D_Dot : Return the dot product of 3D vectors.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
float
E3Vector3D_Dot(const TQ3Vector3D *v1, const TQ3Vector3D *v2)
{
	return(v1->x * v2->x + v1->y * v2->y + v1->z * v2->z);
}





//=============================================================================
//      E3Vector2D_Transform : Multiplies a 2D vector by a 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Vector2D_Transform(const TQ3Vector2D *v, const TQ3Matrix3x3 *matrix3x3, TQ3Vector2D *result)
{
	#define M matrix3x3->value
	result->x = v->x * M[0][0] + v->y * M[1][0];
	result->y = v->x * M[0][1] + v->y * M[1][1];

	return result;
	#undef M
}





//=============================================================================
//      E3Vector3D_Transform : multiplies the 3D vector by the 4x4 matrix.
//-----------------------------------------------------------------------------
//		Note :	'result' may be the same as 'vector'.
//
//				Note that the translation and perspective components of the
//				matrix is ignored (as if it were really a 3x3 matrix).
//
//				Contrast with E3Point3D_Transform, which does the full 4x4
//				transformation.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_Transform(const TQ3Vector3D *v, const TQ3Matrix4x4 *matrix4x4, TQ3Vector3D *result)
{
	#define M matrix4x4->value
	float vx = v->x, vy = v->y, vz = v->z;
	result->x = vx * M[0][0] + vy * M[1][0] + vz * M[2][0];
	result->y = vx * M[0][1] + vy * M[1][1] + vz * M[2][1];
	result->z = vx * M[0][2] + vy * M[1][2] + vz * M[2][2];

	return result;
	#undef M
}





//=============================================================================
//      E3Point2D_Transform : Transforms a 2D point by a 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : The result point and input point may be the same object.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3Point2D_Transform(const TQ3Point2D *p, const TQ3Matrix3x3 *matrix3x3, TQ3Point2D *result)
{
	#define M matrix3x3->value
	float newx = p->x * M[0][0] + p->y * M[1][0] + M[2][0];
	float newy = p->x * M[0][1] + p->y * M[1][1] + M[2][1];
	float neww = p->x * M[0][2] + p->y * M[1][2] + M[2][2];

	if (neww == 1.0f) {
		result->x = newx;
		result->y = newy;
	} else {
		float invw = 1.0f / neww;
		result->x = newx * invw;
		result->y = newy * invw;
	}
	return result;
	#undef M
}





//=============================================================================
//      E3Param2D_Transform : Transforms a 2D param by a 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : The result point and input point may be the same object.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_Transform(const TQ3Param2D *p, const TQ3Matrix3x3 *matrix3x3, TQ3Param2D *result)
{
	#define M matrix3x3->value
	float newx = p->u * M[0][0] + p->v * M[1][0] + M[2][0];
	float newy = p->u * M[0][1] + p->v * M[1][1] + M[2][1];
	float neww = p->u * M[0][2] + p->v * M[1][2] + M[2][2];

	if (neww == 1.0f) {
		result->u = newx;
		result->v = newy;
	} else {
		float invw = 1.0f / neww;
		result->u = newx * invw;
		result->v = newy * invw;
	}
	return result;
	#undef M
}





//=============================================================================
//      E3Point3D_Transform : Transforms the given 3D point by a 4x4 matrix.
//-----------------------------------------------------------------------------
//		Note : The result point and input point may be the same object.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_Transform(const TQ3Point3D *p, const TQ3Matrix4x4 *matrix4x4, TQ3Point3D *result)
{
	#define M matrix4x4->value
	float newx = p->x * M[0][0] + p->y * M[1][0] + p->z * M[2][0] + M[3][0];
	float newy = p->x * M[0][1] + p->y * M[1][1] + p->z * M[2][1] + M[3][1];
	float newz = p->x * M[0][2] + p->y * M[1][2] + p->z * M[2][2] + M[3][2];
	float neww = p->x * M[0][3] + p->y * M[1][3] + p->z * M[2][3] + M[3][3];

	if (neww == 1.0f) {
		result->x = newx;
		result->y = newy;
		result->z = newz;
	} else {
		float invw = 1.0f / neww;
		result->x = newx * invw;
		result->y = newy * invw;
		result->z = newz * invw;
	}
	return result;
	#undef M
}





//=============================================================================
//      E3RationalPoint4D_Transform : Transforms a rational 3D point.
//-----------------------------------------------------------------------------
//		Note : The result point and input point may be the same object.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
E3RationalPoint4D_Transform(const TQ3RationalPoint4D *p, const TQ3Matrix4x4
*matrix4x4, TQ3RationalPoint4D *result)
{
	#define M matrix4x4->value
	float newx = p->x * M[0][0] + p->y * M[1][0] + p->z * M[2][0] + p->w * M[3][0];
	float newy = p->x * M[0][1] + p->y * M[1][1] + p->z * M[2][1] + p->w * M[3][1];
	float newz = p->x * M[0][2] + p->y * M[1][2] + p->z * M[2][2] + p->w * M[3][2];
	float neww = p->x * M[0][3] + p->y * M[1][3] + p->z * M[2][3] + p->w * M[3][3];

	result->x = newx;
	result->y = newy;
	result->z = newz;
	result->w = neww;
	return result;
	#undef M
}





//=============================================================================
//      E3Point3D_To3DTransformArray : Transforms an array of points.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Point3D_To3DTransformArray(const TQ3Point3D		*inPoint3D,
								const TQ3Matrix4x4	*matrix,
								TQ3Point3D			*outPoint3D,
								long				numPoints,
								unsigned long		inStructSize,
								unsigned long		outStructSize)
{

	// we'll just call the standard transformation function here;
	// compiler may choose to inline
	const char *inp=(const char*)inPoint3D;
	char       *outp=(char*)outPoint3D;
	long i;
	for (i=0; i<numPoints; i++) {
		E3Point3D_Transform( (const TQ3Point3D*)inp, matrix, (TQ3Point3D*)outp );
		inp += inStructSize;
		outp += outStructSize;
	}

	return(kQ3Success);
}





//=============================================================================
//      E3Point3D_To4DTransformArray :	Convert an array of 3D points to 4D
//										Rational points, then transform by the
//										given matrix.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Point3D_To4DTransformArray(const TQ3Point3D		*inPoint3D,
								const TQ3Matrix4x4	*matrix,
								TQ3RationalPoint4D	*outPoint4D,
								long				numPoints,
								unsigned long		inStructSize,
								unsigned long		outStructSize)
{

	// we'll just call the standard transformation function here;
	// compiler may choose to inline
	// (this could probably be done more efficiently)
	const char *inp=(const char*)inPoint3D;
	char       *outp=(char*)outPoint4D;
	long i;
	for (i=0; i<numPoints; i++) {
		// start by extending the 3D point to 4D
		const TQ3Point3D *inpoint = (const TQ3Point3D*)inp;
		TQ3RationalPoint4D *outpoint = (TQ3RationalPoint4D*)outp;
		outpoint->x = inpoint->x;		// should we just do a memcpy here?!?
		outpoint->y = inpoint->y;
		outpoint->z = inpoint->z;
		outpoint->w = 1.0f;
		E3RationalPoint4D_Transform( outpoint, matrix, outpoint );
		inp += inStructSize;
		outp += outStructSize;
	}

	return(kQ3Success);
}





//=============================================================================
//      E3RationalPoint4D_To4DTransformArray : Transform an array of 4D points.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3RationalPoint4D_To4DTransformArray(const TQ3RationalPoint4D	*inPoint4D,
										const TQ3Matrix4x4		*matrix,
										TQ3RationalPoint4D		*outPoint4D,
										long					numPoints,
										unsigned long			inStructSize,
										unsigned long			outStructSize)
{
	// we'll just call the standard transformation function here;
	// compiler may choose to inline
	const char *inp=(const char*)inPoint4D;
	char       *outp=(char*)outPoint4D;
	long i;
	for (i=0; i<numPoints; i++) {
		E3RationalPoint4D_Transform( (const TQ3RationalPoint4D*)inp, matrix, (TQ3RationalPoint4D*)outp );
		inp += inStructSize;
		outp += outStructSize;
	}

	return(kQ3Success);
}





//=============================================================================
//      E3Vector2D_Negate : Scales the given vector by -1.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Vector2D_Negate(const TQ3Vector2D *vector2D, TQ3Vector2D *result)
{
	result->x = -vector2D->x;
	result->y = -vector2D->y;
	return(result);
}





//=============================================================================
//      E3Vector3D_Negate : Scales the given vector by -1.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_Negate(const TQ3Vector3D *vector3D, TQ3Vector3D *result)
{
	result->x = -vector3D->x;
	result->y = -vector3D->y;
	result->z = -vector3D->z;
	return(result);
}





//=============================================================================
//      E3Point2D_ToPolar : Converts 2D point to polar coordinates.
//-----------------------------------------------------------------------------
//		Note :	The angle (theta) here is measured counter-clockwise
//				from the +x axis.
//-----------------------------------------------------------------------------
TQ3PolarPoint *
E3Point2D_ToPolar(const TQ3Point2D *point2D, TQ3PolarPoint *result)
{
	// tricky business -- must watch out for singularities,
	// and getting in the proper quadrant
	float x = point2D->x, y = point2D->y;
	result->r = (float)sqrt(x*x + y*y);
	if (x > 0.1f || x < -0.1f) {
		result->theta = (float)atan(y/x);
		if (x < 0.0f) result->theta += kQ3Pi;
		else if (result->theta < 0.0f) result->theta += kQ32Pi;
	} else {
		result->theta = (float)atan(-x/y) + kQ3PiOver2;
		if (y < 0.0f) result->theta += kQ3Pi;
		else if (result->theta > kQ32Pi) result->theta -= kQ32Pi;
	}

	return(result);
}





//=============================================================================
//      E3PolarPoint_ToPoint2D : Convert polar point to cartesian coordinates.
//-----------------------------------------------------------------------------
//		Note :	The angle (theta) here is measured counter-clockwise
//				from the +x axis.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3PolarPoint_ToPoint2D(const TQ3PolarPoint *polarPoint, TQ3Point2D *result)
{
	float theta = polarPoint->theta, r = polarPoint->r;
	result->x = (float)cos(theta) * r;
	result->y = (float)sin(theta) * r;
	return(result);
}





//=============================================================================
//      E3Point3D_ToSpherical : Converts cartesian to spherical coordinates.
//-----------------------------------------------------------------------------
//		Note :	A pretty good reference for 3D coordinate conversions is:
//
//				http://www.geom.umn.edu/docs/reference/CRC-formulas/node42.html
//-----------------------------------------------------------------------------
TQ3SphericalPoint *
E3Point3D_ToSpherical(const TQ3Point3D *point3D, TQ3SphericalPoint *result)
{
	float x=point3D->x, y=point3D->y, z=point3D->z;
	float xsqr = x*x, ysqr = y*y;
	result->rho = (float)sqrt(xsqr + ysqr + z*z);
	
	//  If x is around zero, then we're colinear with the y-axis.
	//  So if y is negative, we're at a negative angle, and vice versa
	if (x > -0.001f && x < 0.001f) {
	  if (y < 0.0f) result->theta = kQ33PiOver2;
	  else result->theta = kQ3PiOver2;
	}
	
	//  Otherwise, we're either on the x-axis or in some quadrant of x-y:
	else {
	  result->theta = (float)atan(y/x);
	  //  If our x value is negative, then we need to convert the atan angle
	  //  into its positive mirror by adding Pi:
	  if (x < 0.0f) result->theta += kQ3Pi;
	  //  Finally, if the angle itself is negative, we need to make it positive:
	  else if (result->theta < 0.0f) result->theta += kQ32Pi;
	}

	if (result->rho != 1.0f) result->phi = (float)acos(z/result->rho);
	else result->phi = (float)acos(z);

	return (result);
}





//=============================================================================
//      E3SphericalPoint_ToPoint3D : Converts spherical to cartesian
//									 coordinates.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3SphericalPoint_ToPoint3D(const TQ3SphericalPoint *sphericalPoint, TQ3Point3D *result)
{
	float theta=sphericalPoint->theta, phi=sphericalPoint->phi, rho=sphericalPoint->rho;
	float rhosinphi = rho * (float)sin(phi);
	result->x = (float)cos(theta) * rhosinphi;
	result->y = (float)sin(theta) * rhosinphi;
	result->z = rho * (float)cos(phi);
	return(result);
}





//=============================================================================
//      E3Point2D_AffineComb : Weighted combination of a number of points.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3Point2D_AffineComb(const TQ3Point2D	*points2D,
						const float		*weights,
						unsigned long	nPoints,
						TQ3Point2D		*result)
{
	float x=0, y=0;
	const float *w=weights;
	const TQ3Point2D *p, *pend;
	for (p = points2D, pend=p+nPoints; p<pend; p++,w++) {
		x += *w * p->x;
		y += *w * p->y;
	}

	result->x = x;
	result->y = y;
	return(result);
}





//=============================================================================
//      E3Param2D_AffineComb : Weighted combination of a number of params.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_AffineComb(const TQ3Param2D *params2D, const float *weights, unsigned long nPoints, TQ3Param2D *result)
{
	float u=0, v=0;
	const float *w=weights;
	const TQ3Param2D *p, *pend;
	for (p = params2D, pend=p+nPoints; p<pend; p++,w++) {
		u += *w * p->u;
		v += *w * p->v;
	}

	result->u = u;
	result->v = v;
	return(result);
}





//=============================================================================
//      E3RationalPoint3D_AffineComb :	Weighted combination of a number of
//										points.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
E3RationalPoint3D_AffineComb(const TQ3RationalPoint3D *points3D, const float *weights, unsigned long numPoints, TQ3RationalPoint3D *result)
{
	float x=0, y=0, z=0;
	const float *w=weights;
	const TQ3RationalPoint3D *p = points3D, *pend=p+numPoints;
	for (p = points3D, pend=p+numPoints; p<pend; p++,w++) {
		x += *w * p->x;
		y += *w * p->y;
		z += *w * p->w;
	}

	result->x = x;
	result->y = y;
	result->w = z;
	return(result);
}





//=============================================================================
//      E3Point3D_AffineComb : Weighted combination of a number of points.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_AffineComb(const TQ3Point3D *points3D, const float *weights, unsigned long numPoints, TQ3Point3D *result)
{
	float x=0, y=0, z=0;
	const float *w=weights;
	const TQ3Point3D *p, *pend;
	for (p = points3D, pend=p+numPoints; p<pend;p++,w++) {
		x += *w * p->x;
		y += *w * p->y;
		z += *w * p->z;
	}

	result->x = x;
	result->y = y;
	result->z = z;
	return(result);
}





//=============================================================================
//      E3RationalPoint4D_AffineComb :	Weighted combination of a number of
//										points.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
E3RationalPoint4D_AffineComb(const TQ3RationalPoint4D *points4D, const float *weights, unsigned long numPoints, TQ3RationalPoint4D *result)
{
	float x=0, y=0, z=0, w=0;
	const float *wgt=weights;
	const TQ3RationalPoint4D *p, *pend;
	for (p = points4D, pend=p+numPoints;p<pend; p++,wgt++) {
		x += *wgt * p->x;
		y += *wgt * p->y;
		z += *wgt * p->z;
		w += *wgt * p->w;
	}

	result->x = x;
	result->y = y;
	result->z = z;
	result->w = w;
	return(result);
}





//=============================================================================
//      E3Matrix3x3_Copy : Copy a 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_Copy(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{
	if (result != matrix3x3)
		memcpy(result, matrix3x3, sizeof(TQ3Matrix3x3));

	return(result);
}





//=============================================================================
//      E3Matrix4x4_Copy : Copy a 4x4 matrix.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_Copy(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result)
{
	if (result != matrix4x4)
		memcpy(result, matrix4x4, sizeof(TQ3Matrix4x4));

	return(result);
}





//=============================================================================
//      E3Matrix3x3_SetIdentity : Initialise an identity matrix.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_SetIdentity(TQ3Matrix3x3 *matrix3x3)
{
	// Initialise the identity matrix
	E3Memory_Clear(matrix3x3, sizeof(TQ3Matrix3x3));

	matrix3x3->value[0][0] = 1.0f;
	matrix3x3->value[1][1] = 1.0f;
	matrix3x3->value[2][2] = 1.0f;

	return(matrix3x3);
}





//=============================================================================
//      E3Matrix4x4_SetIdentity : Initialise an identity matrix.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetIdentity(TQ3Matrix4x4 *matrix4x4)
{
	// Initialise the identity matrix
	E3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));

	matrix4x4->value[0][0] = 1.0f;
	matrix4x4->value[1][1] = 1.0f;
	matrix4x4->value[2][2] = 1.0f;
	matrix4x4->value[3][3] = 1.0f;

	return(matrix4x4);
}





//=============================================================================
//      E3Matrix3x3_Transpose : Transpose a matrix.
//-----------------------------------------------------------------------------
//		Note : result may be the same object as the input parameter.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_Transpose(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{
	if (result != matrix3x3) {
		memcpy(result, matrix3x3, sizeof(TQ3Matrix3x3));
	}
	E3Float_Swap(result->value[1][0], result->value[0][1]);
	E3Float_Swap(result->value[2][0], result->value[0][2]);
	E3Float_Swap(result->value[1][2], result->value[2][1]);
	return(result);
}





//=============================================================================
//      E3Matrix4x4_Transpose : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_Transpose(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result)
{
	if (result != matrix4x4) {
		memcpy(result, matrix4x4, sizeof(TQ3Matrix4x4));
	}
	E3Float_Swap(result->value[1][0], result->value[0][1]);
	E3Float_Swap(result->value[2][0], result->value[0][2]);
	E3Float_Swap(result->value[3][0], result->value[0][3]);
	E3Float_Swap(result->value[2][1], result->value[1][2]);
	E3Float_Swap(result->value[3][1], result->value[1][3]);
	E3Float_Swap(result->value[2][3], result->value[3][2]);
	return(result);
}





//=============================================================================
//      E3Matrix3x3_Invert : Inverts a 3x3 non-singular matrix.
//-----------------------------------------------------------------------------
//		Note : Returns NULL on any error (singular or NULL inputs).
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_Invert(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{
	float				det;
	TQ3Matrix3x3		temp,*resultBuffer;
	
	det = E3Matrix3x3_Determinant(matrix3x3);
	if (det != 0) {

		if (matrix3x3 == result)
		  resultBuffer = &temp;
		else
		  resultBuffer = result;
		  
		#define			A(X,Y)	matrix3x3->value[X][Y]
		#define			B(X,Y)	resultBuffer->value[Y][X]

		//  B(X,Y) does the transpose for us!

		B(0,0)			=  (A(1,1)*A(2,2)-A(2,1)*A(1,2))/det;
		B(1,0)			= -(A(1,0)*A(2,2)-A(2,0)*A(1,2))/det;
		B(2,0)			=  (A(1,0)*A(2,1)-A(2,0)*A(1,1))/det;

		B(0,1)			= -(A(0,1)*A(2,2)-A(2,1)*A(0,2))/det;
		B(1,1)			=  (A(0,0)*A(2,2)-A(2,0)*A(0,2))/det;
		B(2,1)			= -(A(0,0)*A(2,1)-A(2,0)*A(0,1))/det;

		B(0,2)			=  (A(0,1)*A(1,2)-A(1,1)*A(0,2))/det;
		B(1,2)			= -(A(0,0)*A(1,2)-A(1,0)*A(0,2))/det;
		B(2,2)			=  (A(0,0)*A(1,1)-A(1,0)*A(0,1))/det;

		#undef A
		#undef B

		if (matrix3x3 == result)
		  memcpy(result,resultBuffer,sizeof(TQ3Matrix3x3));

		return result;
	}
	else {
	    E3ErrorManager_PostError(kQ3ErrorNonInvertibleMatrix, kQ3False);
	}
	return NULL;
}





//=============================================================================
//      E3Matrix4x4_Invert : Inverts a 4x4 non-singular matrix.
//-----------------------------------------------------------------------------
//		Note : Returns NULL on any error (singular or NULL inputs).
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_Invert(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result)
{
	float				det;
	TQ3Matrix4x4		temp,*resultBuffer;
	
	det = E3Matrix4x4_Determinant(matrix4x4);
	if (det != 0) {

		if (matrix4x4 == result) {
			resultBuffer = &temp;
		} else {
			resultBuffer = result;
		}
		
		#define			A(X,Y)	matrix4x4->value[X][Y]
		#define			B(X,Y)	resultBuffer->value[Y][X]

		//  B(X,Y) is set up to do the transpose for us!

		B(0,0)	=  (A(1,1)*A(2,2)*A(3,3)+A(1,2)*A(2,3)*A(3,1)+A(1,3)*A(2,1)*A(3,2) - \
					A(3,1)*A(2,2)*A(1,3)-A(3,2)*A(2,3)*A(1,1)-A(3,3)*A(2,1)*A(1,2))/det;
		B(0,1)	= -(A(1,0)*A(2,2)*A(3,3)+A(1,2)*A(2,3)*A(3,0)+A(1,3)*A(2,0)*A(3,2) - \
					A(3,0)*A(2,2)*A(1,3)-A(3,2)*A(2,3)*A(1,0)-A(3,3)*A(2,0)*A(1,2))/det;
		B(0,2)	=  (A(1,0)*A(2,1)*A(3,3)+A(1,1)*A(2,3)*A(3,0)+A(1,3)*A(2,0)*A(3,1) - \
					A(3,0)*A(2,1)*A(1,3)-A(3,1)*A(2,3)*A(1,0)-A(3,3)*A(2,0)*A(1,1))/det;
		B(0,3)	= -(A(1,0)*A(2,1)*A(3,2)+A(1,1)*A(2,2)*A(3,0)+A(1,2)*A(2,0)*A(3,1) - \
					A(3,0)*A(2,1)*A(1,2)-A(3,1)*A(2,2)*A(1,0)-A(3,2)*A(2,0)*A(1,1))/det;

		B(1,0)	= -(A(0,1)*A(2,2)*A(3,3)+A(0,2)*A(2,3)*A(3,1)+A(0,3)*A(2,1)*A(3,2) - \
					A(3,1)*A(2,2)*A(0,3)-A(3,2)*A(2,3)*A(0,1)-A(3,3)*A(2,1)*A(0,2))/det;
		B(1,1)	=  (A(0,0)*A(2,2)*A(3,3)+A(0,2)*A(2,3)*A(3,0)+A(0,3)*A(2,0)*A(3,2) - \
					A(3,0)*A(2,2)*A(0,3)-A(3,2)*A(2,3)*A(0,0)-A(3,3)*A(2,0)*A(0,2))/det;
		B(1,2)	= -(A(0,0)*A(2,1)*A(3,3)+A(0,1)*A(2,3)*A(3,0)+A(0,3)*A(2,0)*A(3,1) - \
					A(3,0)*A(2,1)*A(0,3)-A(3,1)*A(2,3)*A(0,0)-A(3,3)*A(2,0)*A(0,1))/det;
		B(1,3)	=  (A(0,0)*A(2,1)*A(3,2)+A(0,1)*A(2,2)*A(3,0)+A(0,2)*A(2,0)*A(3,1) - \
					A(3,0)*A(2,1)*A(0,2)-A(3,1)*A(2,2)*A(0,0)-A(3,2)*A(2,0)*A(0,1))/det;

		B(2,0)	=  (A(0,1)*A(1,2)*A(3,3)+A(0,2)*A(1,3)*A(3,1)+A(0,3)*A(1,1)*A(3,2) - \
					A(3,1)*A(1,2)*A(0,3)-A(3,2)*A(1,3)*A(0,1)-A(3,3)*A(1,1)*A(0,2))/det;
		B(2,1)	= -(A(0,0)*A(1,2)*A(3,3)+A(0,2)*A(1,3)*A(3,0)+A(0,3)*A(1,0)*A(3,2) - \
					A(3,0)*A(1,2)*A(0,3)-A(3,2)*A(1,3)*A(0,0)-A(3,3)*A(1,0)*A(0,2))/det;
		B(2,2)	=  (A(0,0)*A(1,1)*A(3,3)+A(0,1)*A(1,3)*A(3,0)+A(0,3)*A(1,0)*A(3,1) - \
					A(3,0)*A(1,1)*A(0,3)-A(3,1)*A(1,3)*A(0,0)-A(3,3)*A(1,0)*A(0,1))/det;
		B(2,3)	= -(A(0,0)*A(1,1)*A(3,2)+A(0,1)*A(1,2)*A(3,0)+A(0,2)*A(1,0)*A(3,1) - \
					A(3,0)*A(1,1)*A(0,2)-A(3,1)*A(1,2)*A(0,0)-A(3,2)*A(1,0)*A(0,1))/det;

		B(3,0)	= -(A(0,1)*A(1,2)*A(2,3)+A(0,2)*A(1,3)*A(2,1)+A(0,3)*A(1,1)*A(2,2) - \
					A(2,1)*A(1,2)*A(0,3)-A(2,2)*A(1,3)*A(0,1)-A(2,3)*A(1,1)*A(0,2))/det;
		B(3,1)	=  (A(0,0)*A(1,2)*A(2,3)+A(0,2)*A(1,3)*A(2,0)+A(0,3)*A(1,0)*A(2,2) - \
					A(2,0)*A(1,2)*A(0,3)-A(2,2)*A(1,3)*A(0,0)-A(2,3)*A(1,0)*A(0,2))/det;
		B(3,2)	= -(A(0,0)*A(1,1)*A(2,3)+A(0,1)*A(1,3)*A(2,0)+A(0,3)*A(1,0)*A(2,1) - \
					A(2,0)*A(1,1)*A(0,3)-A(2,1)*A(1,3)*A(0,0)-A(2,3)*A(1,0)*A(0,1))/det;
		B(3,3)	=  (A(0,0)*A(1,1)*A(2,2)+A(0,1)*A(1,2)*A(2,0)+A(0,2)*A(1,0)*A(2,1) - \
					A(2,0)*A(1,1)*A(0,2)-A(2,1)*A(1,2)*A(0,0)-A(2,2)*A(1,0)*A(0,1))/det;

		#undef A
		#undef B

		if (matrix4x4 == result) {
			memcpy(result,resultBuffer,sizeof(TQ3Matrix4x4));
		}
		return result;
	}
	else {
		E3ErrorManager_PostError(kQ3ErrorNonInvertibleMatrix, kQ3False);
	}
	return NULL;
}





//=============================================================================
//      E3Matrix3x3_Adjoint : Transposed 3x3 co-factor matrix.
//-----------------------------------------------------------------------------
//		Note : Does the transpose while creating the adjoint matrix.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_Adjoint(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{
	TQ3Matrix3x3		temp,*resultBuffer;
	
	if (matrix3x3 == result) {
		resultBuffer = &temp;
	} else {
		resultBuffer = result;
	}
	
	#define			A(X,Y)	matrix3x3->value[X][Y]
	#define			B(X,Y)	resultBuffer->value[Y][X]
	
	//  B(X,Y) is set up to do the transpose for us!
	
	B(0,0)	=  (A(1,1)*A(2,2)-A(2,1)*A(1,2));
	B(0,1)	= -(A(1,0)*A(2,2)-A(2,0)*A(1,2));
	B(0,2)	=  (A(1,0)*A(2,1)-A(2,0)*A(1,1));
	
	B(1,0)	= -(A(0,1)*A(2,2)-A(2,1)*A(0,2));
	B(1,1)	=  (A(0,0)*A(2,2)-A(2,0)*A(0,2));
	B(1,2)	= -(A(0,0)*A(2,1)-A(2,0)*A(0,1));
	
	B(2,0)	=  (A(0,1)*A(1,2)-A(1,1)*A(0,2));
	B(2,1)	= -(A(0,0)*A(1,2)-A(1,0)*A(0,2));
	B(2,2)	=  (A(0,0)*A(1,1)-A(1,0)*A(0,1));
	
	#undef A
	#undef B
	
	if (matrix3x3 == result) {
		memcpy(result,resultBuffer,sizeof(TQ3Matrix3x3));
	}
	
	return result;
}





//=============================================================================
//      E3Matrix3x3_Multiply : Multiply two 3x3 matrices.
//-----------------------------------------------------------------------------
//		Note : result may be the same as either of the input matrices.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_Multiply(const TQ3Matrix3x3 *matrixA, const TQ3Matrix3x3 *matrixB, TQ3Matrix3x3 *result)
{
	// if result is the same as either input matrix,
	// then we need a temporary buffer which doesn't get clobbered;
	// otherwise, write directly to result matrix
	TQ3Matrix3x3 temp, *out;

	if (result==matrixA || result==matrixB) out=&temp;
	else out = result;
	
	//  * I unrolled the for() loops we were using here to cut any possible overhead
	//	  in the matrix multiplication.  -JTF 08/08/1999
	
	#define A(X,Y)			matrixA->value[X][Y]
	#define B(X,Y)			matrixB->value[X][Y]
	#define M(X,Y)			out->value[X][Y]
	
	M(0,0) = A(0,0) * B(0,0) + A(0,1) * B(1,0) + A(0,2) * B(2,0);
	M(0,1) = A(0,0) * B(0,1) + A(0,1) * B(1,1) + A(0,2) * B(2,1);
	M(0,2) = A(0,0) * B(0,2) + A(0,1) * B(1,2) + A(0,2) * B(2,2);
	
	M(1,0) = A(1,0) * B(0,0) + A(1,1) * B(1,0) + A(1,2) * B(2,0);
	M(1,1) = A(1,0) * B(0,1) + A(1,1) * B(1,1) + A(1,2) * B(2,1);
	M(1,2) = A(1,0) * B(0,2) + A(1,1) * B(1,2) + A(1,2) * B(2,2);
	
	M(2,0) = A(2,0) * B(0,0) + A(2,1) * B(1,0) + A(2,2) * B(2,0);
	M(2,1) = A(2,0) * B(0,1) + A(2,1) * B(1,1) + A(2,2) * B(2,1);
	M(2,2) = A(2,0) * B(0,2) + A(2,1) * B(1,2) + A(2,2) * B(2,2);
	
	#undef A
	#undef B
	#undef C

	if (out==&temp) {
		// copy from temp buffer to result if needed
		memcpy(result, &temp, sizeof(TQ3Matrix3x3));
	}
	return(result);
}





//=============================================================================
//      E3Matrix4x4_Multiply : Multiply two 4x4 matrices.
//-----------------------------------------------------------------------------
//		Note : result may be the same as either of the input matrices.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_Multiply(const TQ3Matrix4x4 *matrixA, const TQ3Matrix4x4 *matrixB, TQ3Matrix4x4 *result)
{
	// if result is the same as either input matrix,
	// then we need a temporary buffer which doesn't get clobbered;
	// otherwise, write directly to result matrix
	TQ3Matrix4x4 temp, *out;

	if (result==matrixA || result==matrixB) out=&temp;
	else out = result;
	
	//  * I unrolled the for() loops we were using here to cut any possible overhead
	//	  in the matrix multiplication.  -JTF 08/08/1999
	
	#define A(X,Y)			matrixA->value[X][Y]
	#define B(X,Y)			matrixB->value[X][Y]
	#define M(X,Y)			out->value[X][Y]
	
	M(0,0) = A(0,0) * B(0,0) + A(0,1) * B(1,0) + A(0,2) * B(2,0) + A(0,3) * B(3,0);
	M(0,1) = A(0,0) * B(0,1) + A(0,1) * B(1,1) + A(0,2) * B(2,1) + A(0,3) * B(3,1);
	M(0,2) = A(0,0) * B(0,2) + A(0,1) * B(1,2) + A(0,2) * B(2,2) + A(0,3) * B(3,2);
	M(0,3) = A(0,0) * B(0,3) + A(0,1) * B(1,3) + A(0,2) * B(2,3) + A(0,3) * B(3,3);
	
	M(1,0) = A(1,0) * B(0,0) + A(1,1) * B(1,0) + A(1,2) * B(2,0) + A(1,3) * B(3,0);
	M(1,1) = A(1,0) * B(0,1) + A(1,1) * B(1,1) + A(1,2) * B(2,1) + A(1,3) * B(3,1);
	M(1,2) = A(1,0) * B(0,2) + A(1,1) * B(1,2) + A(1,2) * B(2,2) + A(1,3) * B(3,2);
	M(1,3) = A(1,0) * B(0,3) + A(1,1) * B(1,3) + A(1,2) * B(2,3) + A(1,3) * B(3,3);
	
	M(2,0) = A(2,0) * B(0,0) + A(2,1) * B(1,0) + A(2,2) * B(2,0) + A(2,3) * B(3,0);
	M(2,1) = A(2,0) * B(0,1) + A(2,1) * B(1,1) + A(2,2) * B(2,1) + A(2,3) * B(3,1);
	M(2,2) = A(2,0) * B(0,2) + A(2,1) * B(1,2) + A(2,2) * B(2,2) + A(2,3) * B(3,2);
	M(2,3) = A(2,0) * B(0,3) + A(2,1) * B(1,3) + A(2,2) * B(2,3) + A(2,3) * B(3,3);
	
	M(3,0) = A(3,0) * B(0,0) + A(3,1) * B(1,0) + A(3,2) * B(2,0) + A(3,3) * B(3,0);
	M(3,1) = A(3,0) * B(0,1) + A(3,1) * B(1,1) + A(3,2) * B(2,1) + A(3,3) * B(3,1);
	M(3,2) = A(3,0) * B(0,2) + A(3,1) * B(1,2) + A(3,2) * B(2,2) + A(3,3) * B(3,2);
	M(3,3) = A(3,0) * B(0,3) + A(3,1) * B(1,3) + A(3,2) * B(2,3) + A(3,3) * B(3,3);
	
	#undef A
	#undef B
	#undef M

	if (out==&temp) {
		// copy from temp buffer to result if needed
		memcpy(result, &temp, sizeof(TQ3Matrix4x4));
	}
	return(result);
}





//=============================================================================
//      E3Matrix3x3_SetTranslate : Set the matrix for x,y translation.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_SetTranslate(TQ3Matrix3x3 *matrix3x3, float xTrans, float yTrans)
{
	E3Memory_Clear(matrix3x3, sizeof(TQ3Matrix3x3));

	matrix3x3->value[0][0] = 1.0f;
	matrix3x3->value[1][1] = 1.0f;
	matrix3x3->value[2][2] = 1.0f;
	matrix3x3->value[2][0] = xTrans;
	matrix3x3->value[2][1] = yTrans;

	return(matrix3x3);
}





//=============================================================================
//      E3Matrix3x3_SetScale : Set the matrix for scaling in x and y.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_SetScale(TQ3Matrix3x3 *matrix3x3, float xScale, float yScale)
{
	E3Memory_Clear(matrix3x3, sizeof(TQ3Matrix3x3));
	
	matrix3x3->value[0][0] = xScale;
	matrix3x3->value[1][1] = yScale;
	matrix3x3->value[2][2] = 1.0f;

	return(matrix3x3);
}





//=============================================================================
//      E3Matrix3x3_SetRotateAboutPoint : Create a rotation matrix based on a
//							center of rotation and an angle to rotate through.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_SetRotateAboutPoint(TQ3Matrix3x3 *matrix3x3, const TQ3Point2D
*origin, float angle)
{
	float			sina = (float)sin(angle);
	float			cosa = (float)cos(angle);
	
	E3Memory_Clear(matrix3x3, sizeof(TQ3Matrix3x3));

	#define M(X,Y)			matrix3x3->value[X][Y]
	#define Dx				origin->x
	#define Dy				origin->y
	
	M(0,0) = cosa;
	M(0,1) = sina;
	M(1,0) = -sina;
	M(1,1) = cosa;
	M(2,0) = -(Dx * cosa) + (Dy * sina) + Dx;
	M(2,1) = -(Dx * sina) - (Dy * cosa) + Dy;
	M(2,2) = 1.0f;
	
	#undef M
	#undef Dx
	#undef Dy
	
	return matrix3x3;
}





//=============================================================================
//      E3Matrix4x4_SetTranslate : Set the matrix for x,y translation.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetTranslate(TQ3Matrix4x4 *matrix4x4, float xTrans, float yTrans, float zTrans)
{
	E3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));

	matrix4x4->value[0][0] = 1.0f;
	matrix4x4->value[1][1] = 1.0f;
	matrix4x4->value[2][2] = 1.0f;
	matrix4x4->value[3][3] = 1.0f;
	matrix4x4->value[3][0] = xTrans;
	matrix4x4->value[3][1] = yTrans;
	matrix4x4->value[3][2] = zTrans;

	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetScale : Set the matrix for scaling in x, y, and z.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetScale(TQ3Matrix4x4 *matrix4x4, float xScale, float yScale, float zScale)
{
	E3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));
	
	matrix4x4->value[0][0] = xScale;
	matrix4x4->value[1][1] = yScale;
	matrix4x4->value[2][2] = zScale;
	matrix4x4->value[3][3] = 1.0f;

	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetRotateAboutPoint : Create a rotation matrix based on
//					a center of rotation and the angles around each axis.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotateAboutPoint(TQ3Matrix4x4 *matrix4x4, const TQ3Point3D *origin, float xAngle, float yAngle, float zAngle)
{
	float sinx = (float)sin(xAngle), cosx = (float)cos(xAngle);
	float siny = (float)sin(yAngle), cosy = (float)cos(yAngle);
	float sinz = (float)sin(zAngle), cosz = (float)cos(zAngle);

	E3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));
	#define M(X,Y)			matrix4x4->value[X][Y]
	#define Dx				origin->x
	#define Dy				origin->y
	#define Dz				origin->z

	M(0,0) = cosy * cosz;
	M(0,1) = cosy * sinz;
	M(0,2) = -siny;
	M(1,0) = sinx * siny * cosz - cosx * sinz;
	M(1,1) = sinx * siny * sinz + cosx * cosz;
	M(1,2) = sinx * cosy;
	M(2,0) = cosx * siny * cosz + sinx * sinz;
	M(2,1) = cosx * siny * sinz - sinx * cosz;
	M(2,2) = cosx * cosy;
	M(3,0) = -Dx * M(0,0) - Dy * M(1,0) - Dz * M(2,0) + Dx;
	M(3,1) = -Dx * M(0,1) - Dy * M(1,1) - Dz * M(2,1) + Dy;
	M(3,2) = -Dx * M(0,2) - Dy * M(1,2) - Dz * M(2,2) + Dz;
	M(3,3) = 1.0f;

	#undef M
	#undef Dx
	#undef Dy
	#undef Dz
	
	return(matrix4x4);
	
}





//=============================================================================
//      E3Matrix4x4_SetRotateAboutAxis : Rotate about an arbitrary axis
//						defined by an origin and an orientation vector.
//-----------------------------------------------------------------------------
//		Note : For best results, orientation should be already normalized.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotateAboutAxis(TQ3Matrix4x4 *matrix4x4, const TQ3Point3D *origin, const TQ3Vector3D *orientation, float angle)
{	
	float	cost, sint, cosp, sinp, cosa, sina;
	float	B00, B01;
	float	A00, A01, A02, A10, A11, A12, A20, A21, A30, A31, A32;

	#define M(X,Y)			matrix4x4->value[X][Y]
	#define B(X,Y)			tempMat.value[X][Y]
	#define Dx				origin->x
	#define Dy				origin->y
	#define Dz				origin->z
		
	//  If x is around zero, then we're colinear with the y-axis.
	//  So if y is negative, we're at a negative angle, and vice versa
	if (orientation->x > -0.001f && orientation->x < 0.001f) {
		cost = 0.0f;
	  	if (orientation->y < 0.0f) {
			sint = -1.0f;
	  	} else {
			sint = 1.0f;
	  	}
	//  Otherwise, we're either on the x-axis or in some quadrant of x-y
	} else {
		//theta = tan(orientation->y/orientation->x)
		//--> tan = sin/cos if vector(orientation->y/orientation->x) is normalized
		float invlen = 1.0f / (float)sqrt(orientation->x * orientation->x + orientation->y * orientation->y);
		sint = orientation->y * invlen;
		cost = orientation->x * invlen;
	}

	cosp = orientation->z;
			// sin^2(a) + cos^2(a) = 1
	sinp = (float)sqrt (1.0f - (cosp * cosp));
	cosa = (float)cos(angle);
	sina = (float)sin(angle);
	
	B00 =  cosp * cost;
	B01 =  cosp * sint;
	//B20 =  sinp * cost;   ---> A02
	//B21 =  sinp * sint;   ---> A12
	
	//A00 =  [cost * cosp] * cosa + sint * sina;
	//A01 =  [cost * cosp] * sina - sint * cosa;
	A00 =  B00 * cosa + sint * sina;
	A01 =  B00 * sina - sint * cosa;
	A02 =  cost * sinp;
	//A10 =  [sint * cosp] * cosa - cost * sina;
	//A11 =  [sint * cosp] * sina + cost * cosa;
	A10 =  B01 * cosa - cost * sina;
	A11 =  B01 * sina + cost * cosa;
	A12 =  sint * sinp;
	A20 = -sinp * cosa;
	A21 = -sinp * sina;
	A30 = -Dx*(A00)-Dy*(A10)-Dz*(A20);
	A31 = -Dx*(A01)-Dy*(A11)-Dz*(A21);
	A32 = -Dx*(A02)-Dy*(A12)-Dz*(cosp);
	
	M(0,0) = (A00) * (B00) + (A01) * (-sint) + (A02) * (A02);
	M(0,1) = (A00) * (B01) + (A01) * (cost) + (A02) * (A12);
	M(0,2) = (A00) * (-sinp) + (A02) * (cosp);
	M(0,3) = 0.0f;

	M(1,0) = (A10) * (B00) + (A11) * (-sint) + (A12) * (A02);
	M(1,1) = (A10) * (B01) + (A11) * (cost) + (A12) * (A12);
	M(1,2) = (A10) * (-sinp) + (A12) * (cosp);
	M(1,3) = 0.0f;
				
	M(2,0) = (A20) * (B00) + (A21) * (-sint) + (cosp) * (A02);
	M(2,1) = (A20) * (B01) + (A21) * (cost) + (cosp) * (A12);
	M(2,2) = (A20) * (-sinp) + (cosp) * (cosp);
	M(2,3) = 0.0f;
				
	M(3,0) = (A30) * (B00) + (A31) * (-sint) + (A32) * (A02) + Dx;
	M(3,1) = (A30) * (B01) + (A31) * (cost) + (A32) * (A12) + Dy;
	M(3,2) = (A30) * (-sinp) + (A32) * (cosp) + Dz;
	M(3,3) = 1.0f;
	
	#undef M
	#undef B
	#undef Dx
	#undef Dy
	#undef Dz
	
	return matrix4x4;
}





//=============================================================================
//      E3Matrix4x4_SetRotate_X : Set matrix to rotate about the X axis.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotate_X(TQ3Matrix4x4 *matrix4x4, float xangle)
{
	float sina = (float)sin(xangle), cosa = (float)cos(xangle);

	E3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));
	#define M matrix4x4->value

	M[0][0]= 1.0f;
	M[1][1]= cosa;
	M[1][2]= sina;
	M[2][1]= -sina;
	M[2][2]= cosa;
	M[3][3] = 1.0f;

	#undef M
	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetRotate_Y : Set matrix to rotate about the Y axis.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotate_Y(TQ3Matrix4x4 *matrix4x4, float yangle)
{
	float sina = (float)sin(yangle), cosa = (float)cos(yangle);

	E3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));
	#define M matrix4x4->value

	M[0][0]= cosa;
	M[0][2]= -sina;
	M[1][1]= 1.0f;
	M[2][0]= sina;
	M[2][2]= cosa;
	M[3][3] = 1.0f;

	#undef M
	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetRotate_Z : Set matrix to rotate about the Z axis.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotate_Z(TQ3Matrix4x4 *matrix4x4, float zangle)
{
	float sina = (float)sin(zangle), cosa = (float)cos(zangle);

	E3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));
	#define M matrix4x4->value

	M[0][0]= cosa;
	M[0][1]= sina;
	M[1][0]= -sina;
	M[1][1]= cosa;
	M[2][2]= 1.0f;
	M[3][3] = 1.0f;

	#undef M
	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetRotate_XYZ : Set matrix to rotate about three axes.
//-----------------------------------------------------------------------------
//		Note :	Rotates in X, Y, Z order (which is rarely useful).
//				This code could probably be optimized a bit.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotate_XYZ(TQ3Matrix4x4 *matrix4x4, float xangle, float yangle, float zangle)
{
	float sinx = (float)sin(xangle), cosx = (float)cos(xangle);
	float siny = (float)sin(yangle), cosy = (float)cos(yangle);
	float sinz = (float)sin(zangle), cosz = (float)cos(zangle);

	E3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));
	#define M matrix4x4->value

	M[0][0] = cosy*cosz;
	M[0][1] = cosy*sinz;
	M[0][2] = -siny;
	M[1][0] = sinx*siny*cosz - cosx*sinz;
	M[1][1] = sinx*siny*sinz + cosx*cosz;
	M[1][2] = sinx*cosy;
	M[2][0] = cosx*siny*cosz + sinx*sinz;
	M[2][1] = cosx*siny*sinz - sinx*cosz;
	M[2][2] = cosx*cosy;
	M[3][3] = 1.0f;

	#undef M
	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetRotateVectorToVector :	Define a rotation matrix
//						based upon the rotation of one vector into another.
//-----------------------------------------------------------------------------
//		Note :	See Foley et al (2nd ed) pp 220-222.
//
//				v1 and v2 should be normalized, and should not be equal.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotateVectorToVector(TQ3Matrix4x4 *matrix4x4, const TQ3Vector3D *v1, const TQ3Vector3D *v2)
{
	TQ3Matrix4x4 matrix2;
	TQ3Vector3D v1Normal;
	TQ3Vector3D v2Normal;
	TQ3Vector3D v2crossv1;
	TQ3Vector3D v1crossv2crossv1;
	TQ3Vector3D v2crossv1crossv2;



	// Normalize the input parameters - this is not required by the spec,
	// but the QD3D implementation accepts unnormalised inputs.
	E3Vector3D_Normalize (v1, &v1Normal);
	E3Vector3D_Normalize (v2, &v2Normal);



	// Check for v1 v2 equality (even better to use a treshold on the angles)
	if (v1Normal.x == v2Normal.x && v1Normal.y == v2Normal.y &&
		v1Normal.z == v2Normal.z)
		{
		// no need to rotate -> identity transform
		E3Matrix4x4_SetIdentity(matrix4x4);
		return matrix4x4;
		}



	// Check for vectors in the same line but opposite direction
	// the cross would be 0,0,0 what is not what we want
	if (v1Normal.x == -v2Normal.x && v1Normal.y == -v2Normal.y &&
		v1Normal.z == -v2Normal.z)
		// this vector is orthogonal to both of the vectors
		E3Vector3D_Set(&v2crossv1, -v1Normal.y, -v1Normal.z, v1Normal.x);
	else
		E3Vector3D_Cross (&v2Normal, &v1Normal, &v2crossv1);



	// Calculate the rotation matrix
	E3Vector3D_Normalize (&v2crossv1, &v2crossv1);
	
	E3Vector3D_Cross (&v1Normal, &v2crossv1, &v1crossv2crossv1);

	#define A(X,Y)	matrix4x4->value[X][Y]

	A(0,0) = v2crossv1.x; // x axis
	A(0,1) = v1crossv2crossv1.x; // y axis
	A(0,2) = v1Normal.x; // z axis
	A(0,3) = 0.0f;

	A(1,0) = v2crossv1.y; // x axis
	A(1,1) = v1crossv2crossv1.y; // y axis
	A(1,2) = v1Normal.y; // z axis
	A(1,3) = 0.0f;
				
	A(2,0) = v2crossv1.z; // x axis
	A(2,1) = v1crossv2crossv1.z; // y axis
	A(2,2) = v1Normal.z; // z axis
	A(2,3) = 0.0f;
				
	A(3,0) = 0.0f;
	A(3,1) = 0.0f;
	A(3,2) = 0.0f;
	A(3,3) = 1.0f;



	E3Vector3D_Cross (&v2Normal, &v2crossv1, &v2crossv1crossv2);

	#define B(X,Y)	matrix2.value[X][Y]

	B(0,0) = v2crossv1.x;
	B(0,1) = v2crossv1.y;
	B(0,2) = v2crossv1.z;
	B(0,3) = 0.0f;

	B(1,0) = v2crossv1crossv2.x;
	B(1,1) = v2crossv1crossv2.y;
	B(1,2) = v2crossv1crossv2.z;
	B(1,3) = 0.0f;
				
	B(2,0) = v2Normal.x;
	B(2,1) = v2Normal.y;
	B(2,2) = v2Normal.z;
	B(2,3) = 0.0f;
				
	B(3,0) = 0;
	B(3,1) = 0;
	B(3,2) = 0;
	B(3,3) = 1.0f;

	E3Matrix4x4_Multiply (matrix4x4, &matrix2, matrix4x4);

	#undef B
	#undef A

	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetQuaternion : Set matrix from a quaternion.
//-----------------------------------------------------------------------------
//		Note : Reference:
//
//				http://www.gamasutra.com/features/programming/
//										19980703/quaternions_08.htm
//
//				Except we have to transpose their matrix, as we use a
//				left-handed (?) coordinate system.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetQuaternion(TQ3Matrix4x4 *matrix, const TQ3Quaternion *quat)
{
	// calculate coefficients
	float x2 = quat->x + quat->x, y2 = quat->y + quat->y, z2 = quat->z + quat->z;
	float xx = quat->x * x2, xy = quat->x * y2, xz = quat->x * z2;
	float yy = quat->y * y2, yz = quat->y * z2, zz = quat->z * z2;
	float wx = quat->w * x2, wy = quat->w * y2, wz = quat->w * z2;

	#define m matrix->value
	m[0][0] = 1.0f - (yy + zz);   m[1][0] = xy - wz;
	m[2][0] = xz + wy;            m[3][0] = 0.0f;

	m[0][1] = xy + wz;            m[1][1] = 1.0f - (xx + zz);
	m[2][1] = yz - wx;            m[3][1] = 0.0f;

	m[0][2] = xz - wy;            m[1][2] = yz + wx;
	m[2][2] = 1.0f - (xx + yy);   m[3][2] = 0.0f;

	m[0][3] = 0.0f;               m[1][3] = 0.0f;
	m[2][3] = 0.0f;               m[3][3] = 1;

	return (matrix);
	#undef m
}





//=============================================================================
//      E3Matrix3x3_Determinant : Returns the determinant of the given matrix.
//-----------------------------------------------------------------------------
float
E3Matrix3x3_Determinant(const TQ3Matrix3x3 *matrix3x3)
{
	#define M matrix3x3->value
	return M[0][0]*M[1][1]*M[2][2]
		+  M[0][1]*M[1][2]*M[2][0]
		+  M[0][2]*M[1][0]*M[2][1]
		-  M[2][0]*M[1][1]*M[0][2]
		-  M[2][1]*M[1][2]*M[0][0]
		-  M[2][2]*M[1][0]*M[0][1];
	#undef M
}





//=============================================================================
//      E3Matrix4x4_Determinant : Returns the determinant of the given matrix.
//-----------------------------------------------------------------------------
float
E3Matrix4x4_Determinant(const TQ3Matrix4x4 *matrix4x4)
{
	#define A(X,Y)			matrix4x4->value[X][Y]
	float				value;
	
	value = A(0,0) * (A(1,1)*A(2,2)*A(3,3)+A(1,2)*A(2,3)*A(3,1)+A(1,3)*A(2,1)*A(3,2) \
					- A(3,1)*A(2,2)*A(1,3)-A(3,2)*A(2,3)*A(1,1)-A(3,3)*A(2,1)*A(1,2));
	value-= A(0,1) * (A(1,0)*A(2,2)*A(3,3)+A(1,2)*A(2,3)*A(3,0)+A(1,3)*A(2,0)*A(3,2) \
					- A(3,0)*A(2,2)*A(1,3)-A(3,2)*A(2,3)*A(1,0)-A(3,3)*A(2,0)*A(1,2));
	value+= A(0,2) * (A(1,0)*A(2,1)*A(3,3)+A(1,1)*A(2,3)*A(3,0)+A(1,3)*A(2,0)*A(3,1) \
					- A(3,0)*A(2,1)*A(1,3)-A(3,1)*A(2,3)*A(1,0)-A(3,3)*A(2,0)*A(1,1));
	value-= A(0,3) * (A(1,0)*A(2,1)*A(3,2)+A(1,1)*A(2,2)*A(3,0)+A(1,2)*A(2,0)*A(3,1) \
					- A(3,0)*A(2,1)*A(1,2)-A(3,1)*A(2,2)*A(1,0)-A(3,2)*A(2,0)*A(1,1));
	return value;
	#undef A
}




//=============================================================================
//      E3Quaternion_Set : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_Set(TQ3Quaternion *quaternion, float w, float x, float y, float z)
{
	quaternion->w = w;
	quaternion->x = x;
	quaternion->y = y;
	quaternion->z = z;
	
	return(quaternion);
}





//=============================================================================
//      E3Quaternion_SetIdentity : Set the given quaternion to (1,0,0,0).
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetIdentity(TQ3Quaternion *quaternion)
{
	quaternion->w = 1.0f;
	quaternion->x = quaternion->y = quaternion->z = 0.0f;

	return(quaternion);
}





//=============================================================================
//      E3Quaternion_Copy : Copy a quaternion.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_Copy(const TQ3Quaternion *src, TQ3Quaternion *dest)
{
	memcpy(dest, src, sizeof(TQ3Quaternion));
	return(dest);
}





//=============================================================================
//      E3Quaternion_IsIdentity :	Return TQ3True if the quaternion is
//									(1,0,0,0).
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Quaternion_IsIdentity(const TQ3Quaternion *q)
{
	return (TQ3Boolean)(1.0f==q->w && 0.0f==q->x && 0.0f==q->y && 0.0f==q->z);
}





//=============================================================================
//      E3Quaternion_Invert : Return/set the conjugate of the given quaternion.
//-----------------------------------------------------------------------------
//		Note :	This is poorly named and should be called "_Conjugate".
//				It is only equivalent to the true inverse when the quaternion
//				is normalized (has length == 1).
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_Invert(const TQ3Quaternion *quaternion, TQ3Quaternion *result)
{
	result->w = quaternion->w;
	result->x = -quaternion->x;
	result->y = -quaternion->y;
	result->z = -quaternion->z;
	return(result);
}





//=============================================================================
//      E3Quaternion_Normalize : Divide a quaternion by its length.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_Normalize(const TQ3Quaternion *q, TQ3Quaternion *result)
{
	float factor = 1.0f / (float)sqrt(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
	result->w = q->w * factor;
	result->x = q->x * factor;
	result->y = q->y * factor;
	result->z = q->z * factor;

	return(result);
}





//=============================================================================
//      E3Quaternion_Dot : Return the dot product of two quaternions.
//-----------------------------------------------------------------------------
float
E3Quaternion_Dot(const TQ3Quaternion *q1, const TQ3Quaternion *q2)
{
	return q1->w * q2->w + q1->x * q2->x + q1->y * q2->y + q1->z * q2->z;
}





//=============================================================================
//      E3Quaternion_Multiply : Find the product of two quaternions.
//-----------------------------------------------------------------------------
//		Note :	There is another algorithm which reduces the multiplies
//				but increases the adds.  Probably not a big difference on
//				the PowerPC.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_Multiply(const TQ3Quaternion *q1, const TQ3Quaternion *q2, TQ3Quaternion *result)
{
	float w1=q1->w, x1=q1->x, y1=q1->y, z1=q1->z;
	float w2=q2->w, x2=q2->x, y2=q2->y, z2=q2->z;

	result->w = w1*w2 - x1*x2 - y1*y2 - z1*z2;
	result->x = w1*x2 + x1*w2 - y1*z2 + z1*y2;
	result->y = w1*y2 + x1*z2 + y1*w2 - z1*x2;
	result->z = w1*z2 - x1*y2 + y1*x2 + z1*w2;
	return result;
}





//=============================================================================
//      E3Quaternion_SetRotateAboutAxis :	 Set quaternion from an axis and
//											angle.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotateAboutAxis(TQ3Quaternion *quaternion, const TQ3Vector3D *axis, float angle)
{
	float sina = (float)sin(angle);
	angle /= 2;
	quaternion->w = (float)cos(angle);
	quaternion->x = axis->x * sina;
	quaternion->y = axis->y * sina;
	quaternion->z = axis->z * sina;
	// ahhh... this is so much easier than matrices...
	return(quaternion);
}





//=============================================================================
//      E3Quaternion_SetRotate_XYZ : One-line description of the method.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotate_XYZ(TQ3Quaternion *quat, float xAngle, float yAngle, float zAngle)
{
	// calculate trig identities
	float cr = (float)cos(xAngle/2);
	float cp = (float)cos(yAngle/2);
	float cy = (float)cos(zAngle/2);

	float sr = (float)sin(xAngle/2);
	float sp = (float)sin(yAngle/2);
	float sy = (float)sin(zAngle/2);

	float cpcy = cp * cy;
	float spsy = sp * sy;

	quat->w = cr * cpcy + sr * spsy;
	quat->x = sr * cpcy - cr * spsy;
	quat->y = cr * sp * cy + sr * cp * sy;
	quat->z = cr * cp * sy - sr * sp * cy;
	return (quat);
}





//=============================================================================
//      E3Quaternion_SetRotate_X : Set a quaternion by angle around the X axis.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotate_X(TQ3Quaternion *quaternion, float angle)
{
	angle /= 2;
	quaternion->w = (float)cos(angle);
	quaternion->x = (float)sin(angle);
	quaternion->y = 0.0f;
	quaternion->z = 0.0f;
	return (quaternion);
}





//=============================================================================
//      E3Quaternion_SetRotate_Y : Set a quaternion by angle around the Y axis.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotate_Y(TQ3Quaternion *quaternion, float angle)
{
	angle /= 2;
	quaternion->w = (float)cos(angle);
	quaternion->x = 0.0f;
	quaternion->y = (float)sin(angle);
	quaternion->z = 0.0f;
	return (quaternion);
}





//=============================================================================
//      E3Quaternion_SetRotate_Z : Set a quaternion by angle around the Y axis.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotate_Z(TQ3Quaternion *quaternion, float angle)
{
	angle /= 2;
	quaternion->w = (float)cos(angle);
	quaternion->x = 0.0f;
	quaternion->y = 0.0f;
	quaternion->z = (float)sin(angle);
	return (quaternion);
}





//=============================================================================
//      E3Quaternion_SetMatrix : Set quaternion from a rotation matrix.
//-----------------------------------------------------------------------------
//		Note :	The QD3D implementation of this function appears to be buggy.
//
//				You can demonstrate this by starting with an arbitrary
//				quaternion, converting to a matrix, then converting back (with
//				this function).
//
//				QD3D's result is something ridiculous; this function returns
//				the original quaternion (or something equivalent). I don't know
//				know how to emulate QD3D's bug here.
//
//				Reference for this code:
//
//				http://www.gamasutra.com/features/programming/
//											19980703/quaternions_07.htm
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetMatrix(TQ3Quaternion *quat, const TQ3Matrix4x4 *matrix)
{
	#define m matrix->value
	float  tr, s, q[4];
	TQ3Uns32    i, j, k;

	TQ3Uns32 nxt[3] = {1, 2, 0};

	tr = m[0][0] + m[1][1] + m[2][2];

	// check the diagonal
	if (tr > 0.0) {
		s = (float)sqrt(tr + 1.0);
		quat->w = s / 2.0f;
		s = 0.5f / s;
		quat->x = (m[1][2] - m[2][1]) * s;
		quat->y = (m[2][0] - m[0][2]) * s;
		quat->z = (m[0][1] - m[1][0]) * s;
	} else {
		// diagonal is negative
		i = 0;
		if (m[1][1] > m[0][0]) i = 1;
		if (m[2][2] > m[i][i]) i = 2;
		j = nxt[i];
		k = nxt[j];

		s = (float) sqrt ((m[i][i] - (m[j][j] + m[k][k])) + 1.0);

		q[i] = s * 0.5f;

		if (s != 0.0f) s = 0.5f / s;

		q[3] = (m[j][k] - m[k][j]) * s;
		q[j] = (m[i][j] + m[j][i]) * s;
		q[k] = (m[i][k] + m[k][i]) * s;

		quat->x = q[0];
		quat->y = q[1];
		quat->z = q[2];
		quat->w = q[3];
	}
	return(quat);
	#undef m
}





//=============================================================================
//      E3Quaternion_SetRotateVectorToVector :	Define a quaternion based on
//												the rotation of one vector into
//												another.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotateVectorToVector(TQ3Quaternion *quaternion, const TQ3Vector3D *v1, const TQ3Vector3D *v2)
{	TQ3Matrix4x4		theMatrix;



	// Create a matrix to do the rotation, then convert it to a quaternion
	E3Matrix4x4_SetRotateVectorToVector(&theMatrix, v1, v2);
	E3Quaternion_SetMatrix(quaternion, &theMatrix);
	
	return(quaternion);
}





//=============================================================================
//      E3Quaternion_MatchReflection : sets result to either q1 or -q1,
//				whichever produces a positive dot product with q2.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_MatchReflection(const TQ3Quaternion *q1, const TQ3Quaternion *q2, TQ3Quaternion *result)
{
	float dot = q1->w * q2->w + q1->x * q2->x + q1->y * q2->y + q1->z * q2->z;
	if (dot > 0) *result = *q1;
	else {
		result->w = -q1->w;
		result->x = -q1->x;
		result->y = -q1->y;
		result->z = -q1->z;
	}

	return(result);
}





//=============================================================================
//      E3Quaternion_InterpolateFast : Do a straight linear interpolation.
//-----------------------------------------------------------------------------
//		Note : This does a true linear, not spherical, interpolation between
//			q1 and q2.  Fast, but not very proper for most uses.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_InterpolateFast(const TQ3Quaternion *q1, const TQ3Quaternion *q2, float t, TQ3Quaternion *result)
{
	float t1 = 1.0f - t;
	result->w = q1->w*t1 + q2->w*t;
	result->x = q1->x*t1 + q2->x*t;
	result->y = q1->y*t1 + q2->y*t;
	result->z = q1->z*t1 + q2->z*t;
	E3Quaternion_Normalize(result, result);
	return(result);
}





//=============================================================================
//      E3Quaternion_InterpolateLinear : Spherical linear interpolation.
//-----------------------------------------------------------------------------
//		Note : Despite the name, this function does a SLERP from q1 to q2.
//			It falls back on a straight linear interpolation only when the
//			cosine of the angle between them is less than 0.01.
//			(That cut-off point was chosen arbitrarily, and may not match
//			that of QD3D.)
//
//			This code adapted from:
// http://www.gamasutra.com/features/programming/19980703/quaternions_09.htm
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_InterpolateLinear(const TQ3Quaternion *q1, const TQ3Quaternion *q2, float t, TQ3Quaternion *result)
{
	float	to1[4];
	float	omega, cosom, sinom, scale0, scale1;

	// calc cosine
	cosom = q1->x * q2->x + q1->y * q2->y + q1->z * q2->z
	             + q1->w * q2->w;

	// adjust signs (if necessary)
	if ( cosom < 0.0f ){
		cosom = -cosom;
		to1[0] = - q2->x;
		to1[1] = - q2->y;
		to1[2] = - q2->z;
		to1[3] = - q2->w;
	} else  {
		to1[0] = q2->x;
		to1[1] = q2->y;
		to1[2] = q2->z;
		to1[3] = q2->w;
	}

	// calculate coefficients

	if ( (1.0f - cosom) > 0.01f ) {
		// standard case (slerp)
		omega = (float)acos(cosom);
		sinom = (float)sin(omega);
		scale0 = (float)sin((1.0 - t) * omega) / sinom;
		scale1 = (float)sin(t * omega) / sinom;
	} else {        
		// "q1" and "q2" quaternions are very close 
		//  ... so we can do a linear interpolation
		scale0 = 1.0f - t;
		scale1 = t;
	}
	// calculate final values
	result->x = scale0 * q1->x + scale1 * to1[0];
	result->y = scale0 * q1->y + scale1 * to1[1];
	result->z = scale0 * q1->z + scale1 * to1[2];
	result->w = scale0 * q1->w + scale1 * to1[3];
	return(result);
}





//=============================================================================
//      E3Vector3D_TransformQuaternion : Transform a vector by a unit quaternion.
//-----------------------------------------------------------------------------
//		Note : Untested.
//			This implementation could probably be improved combining the
//			operations and avoiding the function calls.
//		There is some curiousity in QD3D's implementation.  According to 
//		http://www.gamasutra.com/features/programming/19980703/quaternions_06.htm,
//		to rotate a vector v by quaternion q, you find q*v*inverse(q).
//		QD3D actually does conjugate(q)*v*q.  So there are two issues here:
//			1. QD3D's (and currently our) code only works for unit quaternions
//			2. why qinv*v*q, rather than q*v*qinv?
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_TransformQuaternion(const TQ3Vector3D *vector3D, const TQ3Quaternion *quaternion, TQ3Vector3D *result)
{
	TQ3Quaternion v = {0.0f, 0.0f, 0.0f, 0.0f};
	TQ3Quaternion qinv;

	v.x = vector3D->x;
	v.y = vector3D->y;
	v.z = vector3D->z;

	E3Quaternion_Invert( quaternion, &qinv );	// (actually returns the conjugate)

	// do the multiplication (note that order is significant)
	E3Quaternion_Multiply( &qinv, &v, &v );
	E3Quaternion_Multiply( &v, quaternion, &v );
	
	// and return the spatial part
	result->x = v.x;
	result->y = v.y;
	result->z = v.z;
	return(result);
}





//=============================================================================
//      E3Point3D_TransformQuaternion : Transform a point by a unit quaternion.
//-----------------------------------------------------------------------------
//		Note : See E3Vector3D_TransformQuaternion (same notes apply here).
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_TransformQuaternion(const TQ3Point3D *point3D, const TQ3Quaternion *quaternion, TQ3Point3D *result)
{
	TQ3Quaternion v = {0.0f, 0.0f, 0.0f, 0.0f};
	TQ3Quaternion qinv;

	v.x = point3D->x;
	v.y = point3D->y;
	v.z = point3D->z;

	E3Quaternion_Invert( quaternion, &qinv );	// (actually returns the conjugate)

	// do the multiplication (note that order is significant)
	E3Quaternion_Multiply( &qinv, &v, &v );
	E3Quaternion_Multiply( &v, quaternion, &v );
	
	// and return the spatial part
	result->x = v.x;
	result->y = v.y;
	result->z = v.z;
	return(result);
}





//=============================================================================
//      E3BoundingBox_Copy : Copies a bounding box.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_Copy(const TQ3BoundingBox *src, TQ3BoundingBox *dest)
{
	memcpy(dest, src, sizeof(TQ3BoundingBox));
	return(dest);
}





//=============================================================================
//      E3BoundingBox_Union :	Returns the bounding box that encloses all
//								the space enclosed by either of the given
//								bounding boxes.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_Union(const TQ3BoundingBox *v1, const TQ3BoundingBox *v2, TQ3BoundingBox *result)
{
	result->min.x = E3Num_Min(v1->min.x, v2->min.x);
	result->min.y = E3Num_Min(v1->min.y, v2->min.y);
	result->min.z = E3Num_Min(v1->min.z, v2->min.z);
	result->max.x = E3Num_Max(v1->max.x, v2->max.x);
	result->max.y = E3Num_Max(v1->max.y, v2->max.y);
	result->max.z = E3Num_Max(v1->max.z, v2->max.z);
	result->isEmpty = (TQ3Boolean)(v1->isEmpty && v2->isEmpty);

	return(result);
}





//=============================================================================
//      E3BoundingBox_Set : Set a bounding box from two points.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_Set(TQ3BoundingBox *bBox, const TQ3Point3D *min, const TQ3Point3D *max, TQ3Boolean isEmpty)
{
	bBox->min = *min;
	bBox->max = *max;
	bBox->isEmpty = isEmpty;

	return(bBox);
}





//=============================================================================
//      E3BoundingBox_UnionPoint3D : Expand the bounding box as needed
//									 to contain the given point.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_UnionPoint3D(const TQ3BoundingBox *bBox, const TQ3Point3D *point3D, TQ3BoundingBox *result)
{
	if (bBox->isEmpty) {
		result->min = result->max = *point3D;
		result->isEmpty = kQ3False;
	} else {
		result->min.x = E3Num_Min(point3D->x, bBox->min.x);
		result->min.y = E3Num_Min(point3D->y, bBox->min.y);
		result->min.z = E3Num_Min(point3D->z, bBox->min.z);
		result->max.x = E3Num_Max(point3D->x, bBox->max.x);
		result->max.y = E3Num_Max(point3D->y, bBox->max.y);
		result->max.z = E3Num_Max(point3D->z, bBox->max.z);
	}

	return(result);
}





//=============================================================================
//      E3BoundingBox_UnionRationalPoint4D : Expand the bounding box as needed
//											 to contain the given point.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_UnionRationalPoint4D(const TQ3BoundingBox *bBox, const TQ3RationalPoint4D *point4D, TQ3BoundingBox *result)
{
	if (bBox->isEmpty) {
		TQ3Point3D p;
		float w = point4D->w;
		p.x = point4D->x;
		p.y = point4D->y;
		p.z = point4D->z;
		if (w != 1.0) {
			p.x /= w;
			p.y /= w;
			p.z /= w;
		}
		result->min = result->max = p;
		result->isEmpty = kQ3False;
	} else {
		result->min.x = E3Num_Min(point4D->x, bBox->min.x);
		result->min.y = E3Num_Min(point4D->y, bBox->min.y);
		result->min.z = E3Num_Min(point4D->z, bBox->min.z);
		result->max.x = E3Num_Max(point4D->x, bBox->max.x);
		result->max.y = E3Num_Max(point4D->y, bBox->max.y);
		result->max.z = E3Num_Max(point4D->z, bBox->max.z);
	}

	return(result);
}





//=============================================================================
//      E3BoundingBox_SetFromPoints3D : Set the bounding box to just enclose
//										the given set of points.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_SetFromPoints3D(TQ3BoundingBox *bBox, const TQ3Point3D *points3D,
								unsigned long numPoints, unsigned long structSize)
{
	const char *inp=((const char*)points3D) + structSize;
	TQ3Uns32 i;
	bBox->min = bBox->max = *points3D;
	for (i=1; i<numPoints; i++) {
		float x=((const TQ3Point3D*)inp)->x, y=((const TQ3Point3D*)inp)->y, z=((const TQ3Point3D*)inp)->z;
		if (x < bBox->min.x) bBox->min.x = x;
		else if (x > bBox->max.x) bBox->max.x = x;
		if (y < bBox->min.y) bBox->min.y = y;
		else if (y > bBox->max.y) bBox->max.y = y;
		if (z < bBox->min.z) bBox->min.z = z;
		else if (z > bBox->max.z) bBox->max.z = z;

		inp += structSize;
	}
	bBox->isEmpty = kQ3False;
	return (bBox);
}





//=============================================================================
//      E3BoundingBox_SetFromRationalPoints4D : Set the bounding box to just
//												enclose the given set of points.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_SetFromRationalPoints4D(TQ3BoundingBox *bBox, const TQ3RationalPoint4D *points4D, unsigned long numPoints, unsigned long structSize)
{
	const char *inp=(const char*)points4D;
	TQ3Uns32 i;
	bBox->min = bBox->max = *((const TQ3Point3D*)points4D);
	for (i=0; i<numPoints; i++) {
		float x=((const TQ3RationalPoint4D*)inp)->x, y=((const TQ3RationalPoint4D*)inp)->y,
			  z=((const TQ3RationalPoint4D*)inp)->z, w=((const TQ3RationalPoint4D*)inp)->w;
		if (w != 1.0) {
			x /= w;
			y /= w;
			z /= w;
		}
		if (x < bBox->min.x) bBox->min.x = x;
		else if (x > bBox->max.x) bBox->max.x = x;
		if (y < bBox->min.y) bBox->min.y = y;
		else if (y > bBox->max.y) bBox->max.y = y;
		if (z < bBox->min.z) bBox->min.z = z;
		else if (z > bBox->max.z) bBox->max.z = z;

		inp += structSize;
	}
	bBox->isEmpty = kQ3False;
	return (bBox);
}





//=============================================================================
//      E3BoundingSphere_Copy : Copies a bounding sphere.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_Copy(const TQ3BoundingSphere *src, TQ3BoundingSphere *dest)
{
	memcpy(dest, src, sizeof(TQ3BoundingSphere));
	return(dest);
}





//=============================================================================
//      E3BoundingSphere_Union : Returns the bounding sphere that just
//								 encloses all the points in the two given
//								 bounding spheres.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_Union(const TQ3BoundingSphere *s1, const TQ3BoundingSphere *s2, TQ3BoundingSphere *result)
{

	// Approach: find the points on each sphere farthest from the other sphere
	//	by using similar triangles.  The union sphere has center halfway
	//	between these points, with radius of half their distance.

	if (s1->isEmpty) {
		if (s2->isEmpty) {
			result->isEmpty = kQ3True;
		} else {
			*result = *s2;		// could use memcpy instead; either works
		}
		return result;
	} else if (s2->isEmpty) {
		*result = *s1;
		return result;
	}

	{
	float x1=s1->origin.x, y1=s1->origin.y, z1=s1->origin.z;
	float x2=s2->origin.x, y2=s2->origin.y, z2=s2->origin.z;
	// find the deltas between their centers, and the distance.
	float dx = x2-x1, dy = y2-y1, dz = z2-z1, dist=(float)sqrt(dx*dx+dy*dy+dz*dz);
	if (dist > 0.0f) {
		// find the far points.
		float factor = s1->radius / dist;
		float fx1 = x1 - dx * factor;
		float fy1 = y1 - dy * factor;
		float fz1 = z1 - dz * factor;
		factor = s2->radius / dist;
		{
		float fx2 = x2 + dx * factor;
		float fy2 = y2 + dy * factor;
		float fz2 = z2 + dz * factor;
		// finish the job.
		result->origin.x = (fx1+fx2)/2;
		result->origin.y = (fy1+fy2)/2;
		result->origin.z = (fz1+fz2)/2;
		dx = fx1-fx2;
		dy = fy1-fy2;
		dz = fz1-fz2;
		}
		result->radius = (float)sqrt(dx*dx+dy*dy+dz*dz) / 2.0f;
	} else {
		// if the points are the same, then just take whichever radius is greater.
		result->origin = s1->origin;
		result->radius = E3Num_Max(s1->radius, s2->radius);
	}
	}

	result->isEmpty = kQ3False;
	return (result);
}





//=============================================================================
//      E3BoundingSphere_Set : Set a bounding sphere's parameters.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_Set(TQ3BoundingSphere *bSphere, const TQ3Point3D *origin, float radius, TQ3Boolean isEmpty)
{
	bSphere->origin = *origin;
	bSphere->radius = radius;
	bSphere->isEmpty = isEmpty;
	return(bSphere);
}





//=============================================================================
//      E3BoundingSphere_UnionPoint3D : Expands the bounding box as needed
//										to enclose the given point.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_UnionPoint3D(const TQ3BoundingSphere *bSphere, const TQ3Point3D *point3D, TQ3BoundingSphere *result)
{
	if (bSphere->isEmpty) {
		result->origin = *point3D;
		result->radius = 0.0f;
		result->isEmpty = kQ3False;
		return result;
	}
	// Approach here is similar to that for Union,
	// if we imagine the given point as a radius=0 sphere.
	{
	float x1=bSphere->origin.x, y1=bSphere->origin.y, z1=bSphere->origin.z;
	float x2=point3D->x, y2=point3D->y, z2=point3D->z;
	// find the deltas between their centers, and the distance.
	float dx = x2-x1, dy = y2-y1, dz = z2-z1, dist=(float)sqrt(dx*dx+dy*dy+dz*dz);
	if (dist > bSphere->radius) {
		// find the far points.
		float factor = bSphere->radius / dist;
		float fx1 = x1 - dx * factor;
		float fy1 = y1 - dy * factor;
		float fz1 = z1 - dz * factor;
		// finish the job.
		result->origin.x = (fx1+x2)/2;
		result->origin.y = (fy1+y2)/2;
		result->origin.z = (fz1+z2)/2;
		dx = fx1-x2;
		dy = fy1-y2;
		dz = fz1-z2;
		result->radius = (float)sqrt(dx*dx+dy*dy+dz*dz) / 2.0f;
	} else {
		// if the point is within the sphere, then no change is necessary.
		*result = *bSphere;
		return result;
	}
	}

	result->isEmpty = kQ3False;
	return (result);
}





//=============================================================================
//      E3BoundingSphere_UnionRationalPoint4D : Expands the bounding box as
//												needed to enclose the given
//												point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_UnionRationalPoint4D(const TQ3BoundingSphere *bSphere, const TQ3RationalPoint4D *point4D, TQ3BoundingSphere *result)
{
	if (bSphere->isEmpty) {
		float w = point4D->w;
		if (w == 1.0f) {
			result->origin = *((const TQ3Point3D*)point4D);
		} else {
			w = 1.0f / w;
			result->origin.x = point4D->x * w;
			result->origin.y = point4D->y * w;
			result->origin.z = point4D->z * w;
		}
		result->radius = 0.0f;
		result->isEmpty = kQ3False;
		return result;
	}
	// Approach here is similar to that for Union,
	// if we imagine the given point as a radius=0 sphere.
	{
	float x1=bSphere->origin.x, y1=bSphere->origin.y, z1=bSphere->origin.z;
	float x2=point4D->x, y2=point4D->y, z2=point4D->z, w=point4D->w;
	if (w != 1.0f) {
		x2 /= w;
		y2 /= w;
		z2 /= w;
	}
	// find the deltas between their centers, and the distance.
	{
	float dx = x2-x1, dy = y2-y1, dz = z2-z1, dist=(float)sqrt(dx*dx+dy*dy+dz*dz);
	if (dist > bSphere->radius) {
		// find the far points.
		float factor = bSphere->radius / dist;
		float fx1 = x1 - dx * factor;
		float fy1 = y1 - dy * factor;
		float fz1 = z1 - dz * factor;
		// finish the job.
		result->origin.x = (fx1+x2)/2;
		result->origin.y = (fy1+y2)/2;
		result->origin.z = (fz1+z2)/2;
		dx = fx1-x2;
		dy = fy1-y2;
		dz = fz1-z2;
		result->radius = (float)sqrt(dx*dx+dy*dy+dz*dz) / 2.0f;
	} else {
		// if the point is within the sphere, then no change is necessary.
		*result = *bSphere;
		return result;
	}
	}
	}

	result->isEmpty = kQ3False;
	return (result);
}





//=============================================================================
//      E3BoundingSphere_SetFromPoints3D : Set the sphere to just enclose
//										   the given set of points.
//-----------------------------------------------------------------------------
//		Note : This implementation is inefficient, and could be improved.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_SetFromPoints3D(TQ3BoundingSphere *bSphere, const TQ3Point3D *points3D,
									unsigned long numPoints, unsigned long structSize)
{
	const char *inp=((const char*)points3D) + structSize;
	TQ3Uns32 i;
	bSphere->origin = *points3D;
	bSphere->radius = 0.0f;
	for (i=1; i<numPoints; i++) {
		E3BoundingSphere_UnionPoint3D( bSphere, (const TQ3Point3D*)inp, bSphere );
		inp += structSize;
	}
	bSphere->isEmpty = kQ3False;
	return (bSphere);
}





//=============================================================================
//      E3BoundingSphere_SetFromRationalPoints4D : Set the sphere to just
//												   enclose the given set of
//												   points.
//-----------------------------------------------------------------------------
//		Note :	This implementation is inefficient, and could be improved.
//				Also note: QD3D does not produce the same answer as this
//				function for at least some inputs.
//
//				I think QD3D is wrong, and don't know exactly what it thinks
//				it's doing.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_SetFromRationalPoints4D(TQ3BoundingSphere *bSphere, const TQ3RationalPoint4D *points4D, unsigned long numPoints, unsigned long structSize)
{
	const char *inp=((const char*)points4D) + structSize;
	TQ3Uns32 i;
	bSphere->origin = *((const TQ3Point3D*)points4D);
	if (points4D->w != 1.0f) {
		float w = 1.0f / points4D->w;
		bSphere->origin.x *= w;
		bSphere->origin.y *= w;
		bSphere->origin.z *= w;
	}
	bSphere->radius = 0.0f;
	for (i=1; i<numPoints; i++) {
		E3BoundingSphere_UnionRationalPoint4D( bSphere, (const TQ3RationalPoint4D*)inp, bSphere );
		inp += structSize;
	}
	bSphere->isEmpty = kQ3False;
	return (bSphere);
}






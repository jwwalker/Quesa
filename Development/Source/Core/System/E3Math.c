/*  NAME:
        E3Math.c

    DESCRIPTION:
        Math routines for Quesa.
        
        Note that these routines are allowed to call other E3foo routines for
        speed, to avoid the trip back out through the Q3foo interface.

    COPYRIGHT:
        Quesa Copyright � 1999-2002, Quesa Developers.

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
//      Internal macros
//-----------------------------------------------------------------------------
#define E3Vector2D_IsBelowTolerance(vector2DPtr, tolerance) \
	(E3Vector2D_LengthSquared(vector2DPtr) < (tolerance) * (tolerance))

#define E3Vector3D_IsBelowTolerance(vector3DPtr, tolerance) \
	(E3Vector3D_LengthSquared(vector3DPtr) < (tolerance) * (tolerance))





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//		Overview of matrix determinants and inverses
//-----------------------------------------------------------------------------
//		To calculate the determinant of an NxN matrix with the explicit formula,
//		one must calculate N! terms, each involving N factors. Thus it is
//		necessary to perform N!*(N-1) multiplications and N!-1 additions and
//		subtractions.
//
//		To calculate the determinant of an NxN matrix using Gaussian elimination
//		requires N*(N-1)/2 divisions, (N-1)*(2*N^2-N+6)/6 multiplications and
//		N*(N-1)*(2*N-1)/6 subtractions.
//
// 			     Explicit Formula    Gaussian Elimination
//			N    mul/div add/sub        mul/div add/sub
//			2       2       1              3       1
//			3      12       5             10       5
//			4      72      23             23      14
//
//		We can see that, in terms of the number of floating point operations,
//		Gaussian elimination is faster than the explicit formula, even for N as
//		as small as 3 or 4.
//
//		To calculate the inverse of an NxN matrix using the explicit formula
//		(Cramer's rule), we must calculate 1 determinant of order N and NxN
//		determinants of order N-1. Then we must perform N^2 divisions. In total,
//		calculating the inverse requires N^2 divisions, N!*(N^2-N-1)
//		multiplications and (N+1)!-N^2-1 additions and subtractions.
//
//		To calculate the inverse of an NxN matrix using Gauss-Jordon
//		elimination requires N^2 divisions, N^2*(N-1) multiplications (and
//		N^2*(N-1) subtractions.
//
// 			      Cramer's Rule    Gauss-Jordon Elimination
//			N    mul/div add/sub        mul/div add/sub
//			2       6       1              8       4
//			3      39      14             27      18
//			4     280     103             64      48
//
//		We can see that, in terms of the number of floating point operations,
//		Gauss-Jordon elimination is faster than Cramer's rule, even for N as
//		as small as 3 or 4.
//	
//		More importantly, the explicit formulas for calculating the determinant
//		and inverse of a matrix are unstable: Round off errors are not always
//		negligible.
//
//		In conclusion, Gaussian elimination and Gauss-Jordon elimination are
//		preferable to the explicit formulas for finding the determinant and
//		inverse, respectively, of an NxN matrix, even for N as small as 3 or 4.
//-----------------------------------------------------------------------------





//=============================================================================
//      e3matrix_determinant : Returns the determinant of the given matrix.
//-----------------------------------------------------------------------------
//		Note :	The algorithm modifies the input matrix a.
//
//				The input matrix is an array of pointers to arrays of floats.
//				This data structure allows the same function to handle matrices
//				of different sizes without having to do do index arithmetic
//				explicitly:
//
//					a[i][j] <---> ((float*) a)[i*n+j]
//
//
//				This function uses Gaussian elimination with full pivoting to
//				reduce the matrix to upper triangular form. Then the determinant
//				is merely (plus or minus) the product of the diagonal elements.
//
//				Although the reduction would create 1's along the diagonal and
//				0's below the diagonal, these elements are not calculated because
//				they are not needed.
//
//				See Press, et al., "Numerical Recipes in C", 2nd ed., pp. 32 ff.
//-----------------------------------------------------------------------------
static float
e3matrix_determinant(float **a, TQ3Int32 n)
{
	#define A(x,y) a[x][y]
	
	TQ3Int32 iSign, iPivot, jPivot;
	TQ3Int32 i, j, k;
	float determinant, big, element;
	
	// iSign is +1 or -1, depending on the number of row or column exchanges
	iSign = 1;
	
	// Loop over n pivots
	for (k = 0; k < n; ++k)
	{
		// Search unpivoted part of matrix for largest element to pivot on
		big = -1.0f;
		for (i = k; i < n; ++i)
		{
			for (j = k; j < n; ++j)
			{
				// Calculate absolute value of current element
				element = A(i,j);
				if (element < 0.0f)
					element = -element;
				
				// Compare current element to largest element so far
				if (element > big)
				{
					big = element;
					iPivot = i;
					jPivot = j;
				}
			}
		}
		
		// If largest element is 0, the matrix is singular
		if (big == 0.0f)
			return(0.0f);
		
		// If necessary, put pivot element on diagonal at (k,k)
		if (iPivot != k)
		{
			// Exchange rows
			iSign = -iSign;
			for (j = k; j < n; ++j)
				E3Float_Swap(A(iPivot,j), A(k,j));
		}
		if (jPivot != k)
		{
			// Exchange columns
			iSign = -iSign;
			for (i = k; i < n; ++i)
				E3Float_Swap(A(i,jPivot), A(i,k));
		}
		
		// Divide pivot row (to the right of the pivot column) by pivot element
		//
		// Note: If we were dividing by the same element many times, it would
		// make sense to multiply by its inverse. Since we divide by a given
		// element at most 2 (3) times for a 3x3 (4x4) matrix -- and often
		// less -- it doesn't make sense to pay for the extra floating-point
		// operation.
		element = A(k,k);
		for (j = k+1; j < n; ++j)
			A(k,j) /= element;

		// Reduce rows below pivot row (and to the right of the pivot column)		
		for (i = k+1; i < n; ++i)
		{
			element = A(i,k);
			for (j = k+1; j < n; ++j)
				A(i,j) -= A(k,j)*element;
		}
	}
	
	// Now that the matrix is upper triangular, calculate the determinant as
	// the product of the diagonal elements
	determinant = A(0,0);
	for (k = 1; k < n; ++k)
		determinant *= A(k,k);
	if (iSign < 0)
		determinant = -determinant;
		
	return(determinant);
	
	#undef A
}





//=============================================================================
//      e3matrix_invert : Transforms the given matrix into its inverse.
//-----------------------------------------------------------------------------
//		Note :	The input matrix is an array of pointers to arrays of floats.
//				This data structure allows the same function to handle matrices
//				of different sizes without having to do do index arithmetic
//				explicitly:
//
//					a[i][j] <---> ((float*) a)[i*n+j]
//
//
//				This function uses Gauss-Jordon elimination with full pivoting
//				to transform the given matrix to the identity matrix while
//				transforming the identity matrix to the inverse. As the given
//				matrix is reduced to 1's and 0's column-by-column, the inverse
//				matrix is created in its place column-by-column.
//
//				See Press, et al., "Numerical Recipes in C", 2nd ed., pp. 32 ff.
//-----------------------------------------------------------------------------
static void
e3matrix_invert(float **a, TQ3Int32 n, TQ3Int32 *ipiv, TQ3Int32 *indxr, TQ3Int32 *indxc)
{
	#define A(x,y) a[x][y]
	
	TQ3Int32 irow, icol;
	TQ3Int32 i, j, k;   // *** WARNING: 'k' must be a SIGNED integer ***
	float big, element;

	// Initialize ipiv: ipiv[j] is 0 (1) if row/column j has not (has) been pivoted
	for (j = 0; j < n; ++j)
		ipiv[j] = 0;

	// Loop over n pivots
	for (k = 0; k < n; ++k)
	{
		// Search unpivoted part of matrix for largest element to pivot on
		big = -1.0f;
		for (i = 0; i < n; ++i)
		{
			if (ipiv[i])
				continue;
				
			for (j = 0; j < n; ++j)
			{
				if (ipiv[j])
					continue;
					
				// Calculate absolute value of current element
				element = A(i,j);
				if (element < 0.0f)
					element = -element;
				
				// Compare current element to largest element so far
				if (element > big)
				{
					big = element;
					irow = i;
					icol = j;
				}
			}
		}
		
		// If largest element is 0, the matrix is singular
		if (big == 0.0f)
		{
			E3ErrorManager_PostError(kQ3ErrorNonInvertibleMatrix, kQ3False);
			return;
		}
			
		// Mark pivot row and column
		++ipiv[icol];
		indxr[k] = irow;
		indxc[k] = icol;
		
		// If necessary, exchange rows to put pivot element on diagonal
		if (irow != icol)
		{
			for (j = 0; j < n; ++j)
				E3Float_Swap(A(irow,j), A(icol,j));
		}

		// Divide pivot row by pivot element
		//
		// Note: If we were dividing by the same element many times, it would
		// make sense to multiply by its inverse. Since we divide by a given
		// elemen only 3 (4) times for a 3x3 (4x4) matrix, it doesn't make sense
		// to pay for the extra floating-point operation.
		element = A(icol,icol);
		A(icol,icol) = 1.0f;	// overwrite original matrix with inverse
		for (j = 0; j < n; ++j)
			A(icol,j) /= element;

		// Reduce other rows
		for (i = 0; i < n; ++i)
		{
			if (i == icol)
				continue;

			element = A(i,icol);
			A(i,icol) = 0.0f;	// overwrite original matrix with inverse
			for (j = 0; j < n; ++j)
				A(i,j) -= A(icol,j)*element;
		}
	}
	
	// Permute columns
	for (k = n; --k >= 0; )   // *** WARNING: 'k' must be a SIGNED integer ***
	{
		if (indxr[k] != indxc[k])
		{
			for (i = 0; i < n; ++i)
				E3Float_Swap(A(i,indxr[k]), A(i,indxc[k]));
		}
	}
	
	#undef A
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Vector2D_Set : Set 2D vector.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
E3Vector2D_Set(TQ3Vector2D *vector2D, float x, float y)
{
	vector2D->x = x;
	vector2D->y = y;
	return(vector2D);
}





//=============================================================================
//      E3Vector3D_Set : Set 3D vector.
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
//      E3Point2D_Set : Set 2D point.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3Point2D_Set(TQ3Point2D *point2D, float x, float y)
{
	point2D->x = x;
	point2D->y = y;
	return(point2D);
}





//=============================================================================
//      E3Param2D_Set : Set 2D parametric point.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_Set(TQ3Param2D *param2D, float u, float v)
{
	param2D->u = u;
	param2D->v = v;
	return(param2D);
}





//=============================================================================
//      E3RationalPoint3D_Set : Set RationalPoint3D.
//-----------------------------------------------------------------------------
//		Note : This is an extended 2D point used with 2D transformations.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
E3RationalPoint3D_Set(TQ3RationalPoint3D *rationalPoint3D,
	float x, float y, float w)
{
	rationalPoint3D->x = x;
	rationalPoint3D->y = y;
	rationalPoint3D->w = w;
	return(rationalPoint3D);
}





//=============================================================================
//      E3Point3D_Set : Set 3D point.
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
//      E3RationalPoint4D_Set : Set RationalPoint4D.
//-----------------------------------------------------------------------------
//		Note : This is an extended 3D point used with 3D transformations.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
E3RationalPoint4D_Set(TQ3RationalPoint4D *rationalPoint4D,
	float x, float y, float z, float w)
{
	rationalPoint4D->x = x;
	rationalPoint4D->y = y;
	rationalPoint4D->z = z;
	rationalPoint4D->w = w;
	return(rationalPoint4D);
}





//=============================================================================
//      E3PolarPoint_Set : Set polar point.
//-----------------------------------------------------------------------------
TQ3PolarPoint *
E3PolarPoint_Set(TQ3PolarPoint *polarPoint, float r, float theta)
{
	polarPoint->r     = r;
	polarPoint->theta = theta;
	return(polarPoint);
}





//=============================================================================
//      E3SphericalPoint_Set : Set spherical point.
//-----------------------------------------------------------------------------
TQ3SphericalPoint *
E3SphericalPoint_Set(TQ3SphericalPoint *sphericalPoint,
	float rho, float theta, float phi)
{
	sphericalPoint->rho   = rho;
	sphericalPoint->theta = theta;
	sphericalPoint->phi   = phi;
	return(sphericalPoint);
}





//=============================================================================
//      E3Vector2D_To3D : Convert 2D vector to 3D, setting z to 1.
//-----------------------------------------------------------------------------
//		Note : This operation makes no sense mathematically.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector3D *
E3Vector2D_To3D(const TQ3Vector2D *vector2D, TQ3Vector3D *result)
{
	result->x = vector2D->x;
	result->y = vector2D->y;
	result->z = 1.0f;
	return(result);
}





//=============================================================================
//      E3Vector2D_ToRationalPoint3D : Convert 2D vector to 3D rational point,
//									   setting w to 0.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
E3Vector2D_ToRationalPoint3D(const TQ3Vector2D *vector2D, TQ3RationalPoint3D *result)
{
	result->x = vector2D->x;
	result->y = vector2D->y;
	result->w = 0.0f;
	return(result);
}





//=============================================================================
//      E3Vector3D_To2D : Convert 3D vector to 2D, dividing by z.
//-----------------------------------------------------------------------------
//		Note : This operation makes no sense mathematically.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Vector3D_To2D(const TQ3Vector3D *vector3D, TQ3Vector2D *result)
{
	float invz = 1.0f / vector3D->z;
	result->x = vector3D->x * invz;
	result->y = vector3D->y * invz;
	return(result);
}





//=============================================================================
//      E3RationalPoint3D_ToVector2D : Convert 3D rational point to 2D vector,
//									   discarding w.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3RationalPoint3D_ToVector2D(const TQ3RationalPoint3D *rationalPoint3D, TQ3Vector2D *result)
{
	result->x = rationalPoint3D->x;
	result->y = rationalPoint3D->y;
	return(result);
}





//=============================================================================
//      E3Vector3D_ToRationalPoint4D : Convert 3D vector to 4D rational point,
//									   setting w to 0.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
E3Vector3D_ToRationalPoint4D(const TQ3Vector3D *vector3D, TQ3RationalPoint4D *result)
{
	result->x = vector3D->x;
	result->y = vector3D->y;
	result->z = vector3D->z;
	result->w = 0.0f;
	return(result);
}





//=============================================================================
//      E3RationalPoint4D_ToVector3D : Convert 4D rational point to 3D vector,
//									   discarding w.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3RationalPoint4D_ToVector3D(const TQ3RationalPoint4D *rationalPoint4D, TQ3Vector3D *result)
{
	result->x = rationalPoint4D->x;
	result->y = rationalPoint4D->y;
	result->z = rationalPoint4D->z;
	return(result);
}





//=============================================================================
//      E3Point2D_To3D : Convert 2D point to rational 3D, setting w to 1.
//-----------------------------------------------------------------------------
//		Note :	The QD3D version incorrectly declares the type of 'result' to
//				be TQ3Point3D rather than TQ3RationalPoint3D.
//
//				At a binary level there is no difference, but at a source code
//				level the QD3D version forces the use of the incorrect type or
//				type casting.
//
//				Since the QD3D declaration is incorrect, we have corrected this
//				for Quesa - even though it may require a recast in your code.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
E3Point2D_To3D(const TQ3Point2D *point2D, TQ3RationalPoint3D *result)
{
	result->x = point2D->x;
	result->y = point2D->y;
	result->w = 1.0f;
	return(result);
}





//=============================================================================
//      E3RationalPoint3D_To2D : Convert rational 3D point to 2D, dividing by w.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3RationalPoint3D_To2D(const TQ3RationalPoint3D *rationalPoint3D,
	TQ3Point2D *result)
{
	float invw = 1.0f / rationalPoint3D->w;
	result->x = rationalPoint3D->x * invw;
	result->y = rationalPoint3D->y * invw;
	return(result);
}





//============================================================================
//      E3Point3D_To4D : Convert 3D point to rational 4D, setting w to 1.
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
//      E3RationalPoint4D_To3D : Convert rational 4D point to 3D, dividing by w.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3RationalPoint4D_To3D(const TQ3RationalPoint4D *rationalPoint4D,
	TQ3Point3D *result)
{
	float invw = 1.0f / rationalPoint4D->w;
	result->x = rationalPoint4D->x * invw;
	result->y = rationalPoint4D->y * invw;
	result->z = rationalPoint4D->z * invw;
	return(result);
}





//=============================================================================
//      E3Point2D_ToPolar : Convert 2D cartesian point to polar coordinates.
//-----------------------------------------------------------------------------
//		Note :	The angle (theta) here is measured counter-clockwise
//				from the +x axis.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3PolarPoint *
E3Point2D_ToPolar(const TQ3Point2D *point2D, TQ3PolarPoint *result)
{
	float x = point2D->x;
	float y = point2D->y;
	
	if (x == 0.0f && y == 0.0f)
	{
		result->r = result->theta = 0.0f;
	}
	else
	{
		result->r = (float) sqrt(x*x + y*y);
		result->theta = (float) atan2(y, x);
		if (result->theta < 0.0f)
			result->theta += kQ32Pi;
	} 

	return(result);
}





//=============================================================================
//      E3PolarPoint_ToPoint2D : Convert 2D polar point to cartesian
//								 coordinates.
//-----------------------------------------------------------------------------
//		Note :	The angle (theta) here is measured counter-clockwise
//				from the +x axis.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3PolarPoint_ToPoint2D(const TQ3PolarPoint *polarPoint, TQ3Point2D *result)
{
	result->x = polarPoint->r * ((float) cos(polarPoint->theta));
	result->y = polarPoint->r * ((float) sin(polarPoint->theta));
	return(result);
}





//=============================================================================
//      E3Point3D_ToSpherical : Convert 3D cartesian point to spherical
//								coordinates.
//-----------------------------------------------------------------------------
//		Note :	A pretty good reference for 3D coordinate conversions is:
//
//				http://www.geom.umn.edu/docs/reference/CRC-formulas/node42.html
//-----------------------------------------------------------------------------
TQ3SphericalPoint *
E3Point3D_ToSpherical(const TQ3Point3D *point3D, TQ3SphericalPoint *result)
{
	float x = point3D->x;
	float y = point3D->y;
	float z = point3D->z;
	
	if (x == 0.0f && y == 0.0f)
	{
		if (z >= 0.0f)
		{
			result->rho = z;
			result->phi = 0.0f;
		}
		else
		{
			result->rho = -z;
			result->phi = kQ3PiOver2;
		}
		result->theta = 0.0f;
	}
	else
	{
		result->rho = (float) sqrt(x*x + y*y + z*z);
		result->phi = (float) acos(z/result->rho);
		result->theta = (float) atan2(y, x);
		if (result->theta < 0.0f)
			result->theta += kQ32Pi;
	} 

	return(result);
}





//=============================================================================
//      E3SphericalPoint_ToPoint3D : Convert 3D spherical point to cartesian
//									 coordinates.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3SphericalPoint_ToPoint3D(const TQ3SphericalPoint *sphericalPoint,
	TQ3Point3D *result)
{
	float rhoSinPhi = sphericalPoint->rho * ((float) sin(sphericalPoint->phi));
	result->x = rhoSinPhi * ((float) cos(sphericalPoint->theta));
	result->y = rhoSinPhi * ((float) sin(sphericalPoint->theta));
	result->z = sphericalPoint->rho * ((float) cos(sphericalPoint->phi));
	return(result);
}





//=============================================================================
//      E3Vector2D_Dot : Return dot product of v1 and v2.
//-----------------------------------------------------------------------------
#pragma mark -
float
E3Vector2D_Dot(const TQ3Vector2D *v1, const TQ3Vector2D *v2)
{
	return(v1->x*v2->x + v1->y*v2->y);
}





//=============================================================================
//      E3Vector3D_Dot : Return dot product of v1 and v2.
//-----------------------------------------------------------------------------
float
E3Vector3D_Dot(const TQ3Vector3D *v1, const TQ3Vector3D *v2)
{
	return(v1->x*v2->x + v1->y*v2->y + v1->z*v2->z);
}





//=============================================================================
//      E3Vector2D_Cross : Return 2D cross product of v1 and v2.
//-----------------------------------------------------------------------------
//		Note :	We assume the 2D vectors are really 3D vectors with z=0,
//				then return the z coordinate of the cross product (0,0,z).
//-----------------------------------------------------------------------------
#pragma mark -
float
E3Vector2D_Cross(const TQ3Vector2D *v1, const TQ3Vector2D *v2)
{
	return(v1->x*v2->y - v1->y*v2->x);
}





//=============================================================================
//      E3Point2D_CrossProductTri :	Return cross product of triangle, that is
//									of the vectors p2-p1 and p3-p2.
//-----------------------------------------------------------------------------
float
E3Point2D_CrossProductTri(const TQ3Point2D *p1, const TQ3Point2D *p2,
	const TQ3Point2D *p3)
{
	TQ3Vector2D v1, v2;

	// Calculate our vectors
	E3Point2D_Subtract(p2, p1, &v1);
	E3Point2D_Subtract(p3, p2, &v2);

	// Return the cross product
	return(E3Vector2D_Cross(&v1, &v2));
}





//=============================================================================
//      E3Vector3D_Cross : Return 3D cross product of v1 and v2.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'v1' and/or 'v2'.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_Cross(const TQ3Vector3D *v1, const TQ3Vector3D *v2,
	TQ3Vector3D *result)
{
	// If result is alias of input, output to temporary
	TQ3Vector3D temp;
	TQ3Vector3D* output = (result == v1 || result == v2 ? &temp : result);
	
	// Calculate the cross product of v1 and v2
	output->x = v1->y*v2->z - v1->z*v2->y;
	output->y = v1->z*v2->x - v1->x*v2->z;
	output->z = v1->x*v2->y - v1->y*v2->x;
	
	if (output == &temp)
		*result = temp;

	return(result);
}





//=============================================================================
//      E3Point3D_CrossProductTri :	Return cross product of triangle, that is
//									of the vectors p2-p1 and p3-p2.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Point3D_CrossProductTri(const TQ3Point3D *p1, const TQ3Point3D *p2,
	const TQ3Point3D *p3, TQ3Vector3D *result)
{
	TQ3Vector3D v1, v2;

	// Calculate our vectors
	E3Point3D_Subtract(p2, p1, &v1);
	E3Point3D_Subtract(p3, p2, &v2);

	// Return the cross product
	return(E3Vector3D_Cross(&v1, &v2, result));
}





//=============================================================================
//      E3Vector2D_Length : Return length of 2D vector.
//-----------------------------------------------------------------------------
#pragma mark -
float
E3Vector2D_Length(const TQ3Vector2D *vector2D)
{
	return((float) sqrt(E3Vector2D_LengthSquared(vector2D)));
}





//=============================================================================
//      E3Vector2D_LengthSquared : Return squared length of 2D vector.
//-----------------------------------------------------------------------------
float
E3Vector2D_LengthSquared(const TQ3Vector2D *vector2D)
{
	return(vector2D->x*vector2D->x + vector2D->y*vector2D->y);
}





//=============================================================================
//      E3Vector3D_Length : Return length of 3D vector.
//-----------------------------------------------------------------------------
float
E3Vector3D_Length(const TQ3Vector3D *vector3D)
{
	return((float) sqrt(E3Vector3D_LengthSquared(vector3D)));
}





//=============================================================================
//      E3Vector3D_LengthSquared : Return squared length of 3D vector.
//-----------------------------------------------------------------------------
float
E3Vector3D_LengthSquared(const TQ3Vector3D *vector3D)
{
	return(vector3D->x*vector3D->x + vector3D->y*vector3D->y +
		vector3D->z*vector3D->z);
}





//=============================================================================
//      E3Point2D_Distance : Return Euclidean distance.
//-----------------------------------------------------------------------------
#pragma mark -
float
E3Point2D_Distance(const TQ3Point2D *p1, const TQ3Point2D *p2)
{
	return((float) sqrt(E3Point2D_DistanceSquared(p1, p2)));
}





//=============================================================================
//      E3Point2D_DistanceSquared : Return squared Euclidean distance.
//-----------------------------------------------------------------------------
float
E3Point2D_DistanceSquared(const TQ3Point2D *p1, const TQ3Point2D *p2)
{
	float dx = p1->x - p2->x;
	float dy = p1->y - p2->y;
	return(dx*dx + dy*dy);
}





//=============================================================================
//      E3Param2D_Distance : Return Euclidean distance.
//-----------------------------------------------------------------------------
//		Note : Equivalent to E3Point2D_Distance.
//-----------------------------------------------------------------------------
float
E3Param2D_Distance(const TQ3Param2D *p1, const TQ3Param2D *p2)
{
	return(E3Point2D_Distance((const TQ3Point2D*) p1, (const TQ3Point2D*) p2));
}





//=============================================================================
//      E3Param2D_DistanceSquared : Return squared Euclidean distance.
//-----------------------------------------------------------------------------
//		Note : Equivalent to E3Point2D_DistanceSquared.
//-----------------------------------------------------------------------------
float
E3Param2D_DistanceSquared(const TQ3Param2D *p1, const TQ3Param2D *p2)
{
	return(E3Point2D_DistanceSquared((const TQ3Point2D*) p1, (const TQ3Point2D*) p2));
}





//=============================================================================
//      E3RationalPoint3D_Distance : Return Euclidean distance.
//-----------------------------------------------------------------------------
//		Note : This operation makes no sense mathematically.
//-----------------------------------------------------------------------------
float
E3RationalPoint3D_Distance(const TQ3RationalPoint3D *p1,
						   const TQ3RationalPoint3D *p2)
{
	return((float) sqrt(E3RationalPoint3D_DistanceSquared(p1, p2)));
}





//=============================================================================
//      E3RationalPoint3D_DistanceSquared :	Return squared Euclidean distance.
//-----------------------------------------------------------------------------
//		Note : This operation makes no sense mathematically.
//-----------------------------------------------------------------------------
float
E3RationalPoint3D_DistanceSquared(const TQ3RationalPoint3D *p1,
								  const TQ3RationalPoint3D *p2)
{
	float a = p1->x - p2->x, b = p1->y - p2->y, c = p1->w - p2->w;
	return(a*a + b*b + c*c);
}





//=============================================================================
//      E3Point3D_Distance : Return Euclidean distance.
//-----------------------------------------------------------------------------
float
E3Point3D_Distance(const TQ3Point3D *p1, const TQ3Point3D *p2)
{
	return((float) sqrt(E3Point3D_DistanceSquared(p1, p2)));
}





//=============================================================================
//      E3Point3D_DistanceSquared : Return squared Euclidean distance.
//-----------------------------------------------------------------------------
float
E3Point3D_DistanceSquared(const TQ3Point3D *p1, const TQ3Point3D *p2)
{
	float dx = p1->x - p2->x;
	float dy = p1->y - p2->y;
	float dz = p1->z - p2->z;
	return((float) dx*dx + dy*dy + dz*dz);
}





//=============================================================================
//      E3RationalPoint4D_Distance : Return Euclidean distance.
//-----------------------------------------------------------------------------
//		Note : This operation makes no sense mathematically.
//-----------------------------------------------------------------------------
float
E3RationalPoint4D_Distance(const TQ3RationalPoint4D *p1,
						   const TQ3RationalPoint4D *p2)
{
	return((float) sqrt(E3RationalPoint4D_DistanceSquared(p1, p2)));
}





//=============================================================================
//      E3RationalPoint4D_DistanceSquared : Return squared Euclidean distance.
//-----------------------------------------------------------------------------
//		Note : This operation makes no sense mathematically.
//-----------------------------------------------------------------------------
float
E3RationalPoint4D_DistanceSquared(const TQ3RationalPoint4D *p1,
                                  const TQ3RationalPoint4D *p2)
{
	float a = p1->x - p2->x, b = p1->y - p2->y, c = p1->z - p2->z, d = p1->w - p2->w;
	return(a*a + b*b + c*c + d*d);
}





//=============================================================================
//      E3Vector2D_Negate : Scale 2D vector by -1.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'vector2D'.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
E3Vector2D_Negate(const TQ3Vector2D *vector2D, TQ3Vector2D *result)
{
	result->x = -vector2D->x;
	result->y = -vector2D->y;
	return(result);
}





//=============================================================================
//      E3Vector3D_Negate : Scale 3D vector vector by -1.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'vector3D'.
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
//      E3Vector2D_Scale : Scale 2D vector by the given factor.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'vector2D'.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
E3Vector2D_Scale(const TQ3Vector2D *vector2D, float scalar, TQ3Vector2D *result)
{
	result->x = vector2D->x * scalar;
	result->y = vector2D->y * scalar;
	return(result);
}





//=============================================================================
//      E3Vector3D_Scale : Scale 3D vector by the given factor.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'vector3D'.
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
//      E3Vector2D_Normalize :	Scale 2D vector to length 1.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'vector2D'.
//				To obtain valid results, |vector2D| must not be 0.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
E3Vector2D_Normalize(const TQ3Vector2D *vector2D, TQ3Vector2D *result)
{
	return(E3Vector2D_Scale(vector2D, 1.0f/E3Vector2D_Length(vector2D), result));
}





//=============================================================================
//      E3Vector3D_Normalize :	Scale 3D vector to length 1.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'vector3D'.
//				To obtain valid results, |vector2D| must not be 0.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_Normalize(const TQ3Vector3D *vector3D, TQ3Vector3D *result)
{
	return(E3Vector3D_Scale(vector3D, 1.0f/E3Vector3D_Length(vector3D), result));
}





//=============================================================================
//      E3Vector2D_Add : Add two 2D vectors.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'v1' and/or 'v2'.
//-----------------------------------------------------------------------------
#pragma mark -
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
//		Note : 'result' may be the same as 'v1' and/or 'v2'.
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
//      E3Vector2D_Subtract : Subtract 2D vector v2 from v1.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'v1' and/or 'v2'.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Vector2D_Subtract(const TQ3Vector2D *v1, const TQ3Vector2D *v2, TQ3Vector2D *result)
{
	result->x = v1->x - v2->x;
	result->y = v1->y - v2->y;
	return(result);
}





//=============================================================================
//      E3Vector3D_Subtract : Subtract 3D vector v2 from v1.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'v1' and/or 'v2'.
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
//      E3Point2D_Vector2D_Add : Add 2D vector to point.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'point2D'.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Point2D *
E3Point2D_Vector2D_Add(const TQ3Point2D *point2D, const TQ3Vector2D *vector2D,
	TQ3Point2D *result)
{
	result->x = point2D->x + vector2D->x;
	result->y = point2D->y + vector2D->y;
	return(result);
}





//=============================================================================
//      E3Param2D_Vector2D_Add : Add 2D vector to parametric point.
//-----------------------------------------------------------------------------
//		Note : Equivalent to E3Point2D_Vector2D_Add.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_Vector2D_Add(const TQ3Param2D *param2D, const TQ3Vector2D *vector2D,
	TQ3Param2D *result)
{
	return((TQ3Param2D*) E3Point2D_Vector2D_Add((const TQ3Point2D*) param2D, vector2D,
		(TQ3Point2D*) result));
}





//=============================================================================
//      E3Point3D_Vector3D_Add : Add 3D vector to point.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'point3D'.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_Vector3D_Add(const TQ3Point3D *point3D, const TQ3Vector3D *vector3D,
	TQ3Point3D *result)
{
	result->x = point3D->x + vector3D->x;
	result->y = point3D->y + vector3D->y;
	result->z = point3D->z + vector3D->z;
	return(result);
}





//=============================================================================
//      E3Point2D_Vector2D_Subtract : Subtract 2D vector from point.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'point2D'.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3Point2D_Vector2D_Subtract(const TQ3Point2D *point2D, const TQ3Vector2D *vector2D,
	TQ3Point2D *result)
{
	result->x = point2D->x - vector2D->x;
	result->y = point2D->y - vector2D->y;
	return(result);
}





//=============================================================================
//      E3Param2D_Vector2D_Subtract : Subtract 2D vector from parametric point.
//-----------------------------------------------------------------------------
//		Note : Equivalent to E3Point2D_Vector2D_Subtract.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_Vector2D_Subtract(const TQ3Param2D *param2D, const TQ3Vector2D *vector2D,
	TQ3Param2D *result)
{
	return((TQ3Param2D*) E3Point2D_Vector2D_Subtract((const TQ3Point2D*) param2D,
		vector2D, (TQ3Point2D*) result));
}





//=============================================================================
//      E3Point3D_Vector3D_Subtract : Subtract 3D vector from point.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'point3D'.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_Vector3D_Subtract(const TQ3Point3D *point3D, const TQ3Vector3D *vector3D,
	TQ3Point3D *result)
{
	result->x = point3D->x - vector3D->x;
	result->y = point3D->y - vector3D->y;
	result->z = point3D->z - vector3D->z;
	return(result);
}





//=============================================================================
//      E3Point2D_Subtract : Subtract 2D point p2 from p1.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'p1' and/or 'p2'.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
E3Point2D_Subtract(const TQ3Point2D *p1, const TQ3Point2D *p2, TQ3Vector2D *result)
{
	result->x = p1->x - p2->x;
	result->y = p1->y - p2->y;
	return(result);
}





//=============================================================================
//      E3Param2D_Subtract : Subtract 2D point parametric p2 from p1.
//-----------------------------------------------------------------------------
//		Note : Equivalent to E3Point2D_Subtract.
//-----------------------------------------------------------------------------
TQ3Vector2D *
E3Param2D_Subtract(const TQ3Param2D *p1, const TQ3Param2D *p2, TQ3Vector2D *result)
{
	return(E3Point2D_Subtract((const TQ3Point2D*) p1, (const TQ3Point2D*) p2, result));
}





//=============================================================================
//      E3Point3D_Subtract : Subtract 3D point p2 from p1.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'p1' and/or 'p2'.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Point3D_Subtract(const TQ3Point3D *p1, const TQ3Point3D *p2, TQ3Vector3D *result)
{
	result->x = p1->x - p2->x;
	result->y = p1->y - p2->y;
	result->z = p1->z - p2->z;
	return(result);
}




//=============================================================================
//      E3Point2D_RRatio :	Return point at ratio r2/(r1+r2) along segment from
//							p1 to p2.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'p1' and/or 'p2'.
//
//				The QD3D docs claim that the ratio used is r1/(r1+r2), but
//				we found by direct experimentation that the QD3D library (1.6)
//				in fact uses r2/(r1+r2) instead.
//
//				As usual, we do as QD3D does, not as the docs say.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Point2D *
E3Point2D_RRatio(const TQ3Point2D *p1, const TQ3Point2D *p2, float r1, float r2,
	TQ3Point2D *result)
{
	float frac = r2/(r1+r2);
	result->x = p1->x + frac*(p2->x - p1->x);
	result->y = p1->y + frac*(p2->y - p1->y);
	return(result);
}





//=============================================================================
//      E3Param2D_RRatio :	Return point at ratio r2/(r1+r2) along segment from
//							p1 to p2.
//-----------------------------------------------------------------------------
//		Note : Equivalent to E3Point2D_RRatio.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_RRatio(const TQ3Param2D *p1, const TQ3Param2D *p2, float r1, float r2,
	TQ3Param2D *result)
{
	return((TQ3Param2D*) E3Point2D_RRatio((const TQ3Point2D*) p1,
		(const TQ3Point2D*) p2, r1, r2, (TQ3Point2D*) result));
}





//=============================================================================
//      E3Point3D_RRatio :	Return point at ratio r2/(r1+r2) along segment from
//							p1 to p2.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'p1' and/or 'p2'.
//
//				The QD3D docs claim that the ratio used is r1/(r1+r2), but
//				we found by direct experimentation that the QD3D library (1.6)
//				in fact uses r2/(r1+r2) instead.
//
//				As usual, we do as QD3D does, not as the docs say.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_RRatio(const TQ3Point3D *p1, const TQ3Point3D *p2, float r1, float r2,
	TQ3Point3D *result)
{
	float frac = r2/(r1+r2);
	result->x = p1->x + frac*(p2->x - p1->x);
	result->y = p1->y + frac*(p2->y - p1->y);
	result->z = p1->z + frac*(p2->z - p1->z);
	return(result);
}





//=============================================================================
//      E3RationalPoint4D_RRatio :	Return point at ratio r2/(r1+r2) along
//									segment from p1 to p2.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'p1' and/or 'p2'.
//				This operation makes no sense mathematically.
//
//				The QD3D docs claim that the ratio used is r1/(r1+r2), but
//				we found by direct experimentation that the QD3D library (1.6)
//				in fact uses r2/(r1+r2) instead.
//
//				As usual, we do as QD3D does, not as the docs say.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
E3RationalPoint4D_RRatio(const TQ3RationalPoint4D *p1, const TQ3RationalPoint4D *p2,
	float r1, float r2, TQ3RationalPoint4D *result)
{
	float frac = r2/(r1+r2);
	result->x = p1->x + frac*(p2->x - p1->x);
	result->y = p1->y + frac*(p2->y - p1->y);
	result->z = p1->z + frac*(p2->z - p1->z);
	result->w = p1->w + frac*(p2->w - p1->w);
	return(result);
}





//=============================================================================
//      E3Point2D_AffineComb : Return weighted combination of several 2D points.
//-----------------------------------------------------------------------------
//		Note :	Weights are NOT required to sum to 1, but we they must not sum
//				to 0.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Point2D *
E3Point2D_AffineComb(const TQ3Point2D	*points2D,
					 const float		*weights,
					 TQ3Uns32			numPoints,
					 TQ3Point2D			*result)
{
	float x = 0.0f;
	float y = 0.0f;
	float totalWeight = 0.0f;
	TQ3Uns32 i;
	
	for (i = 0; i < numPoints; ++i)
	{
		x += points2D[i].x * weights[i];
		y += points2D[i].y * weights[i];
		totalWeight += weights[i];
	}

	result->x = x / totalWeight;
	result->y = y / totalWeight;
	
	return(result);
}





//=============================================================================
//      E3Param2D_AffineComb :  Return weighted combination of several 2D
//								parametric points.
//-----------------------------------------------------------------------------
//		Note : Equivalent to E3Point2D_AffineComb.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_AffineComb(const TQ3Param2D	*params2D,
					 const float		*weights,
					 TQ3Uns32			numPoints,
					 TQ3Param2D			*result)
{
	return((TQ3Param2D*) E3Point2D_AffineComb((const TQ3Point2D*) params2D, weights,
		numPoints, (TQ3Point2D*) result));
}





//=============================================================================
//      E3RationalPoint3D_AffineComb :	Return weighted combination of several
//										3D rational points.
//-----------------------------------------------------------------------------
//		Note :	Weights are NOT required to sum to 1, but we they must not sum
//				to 0.
//
//				This operation makes no sense mathematically.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
E3RationalPoint3D_AffineComb(const TQ3RationalPoint3D	*rationalPoints3D,
							 const float				*weights,
							 TQ3Uns32					numPoints,
							 TQ3RationalPoint3D			*result)
{
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.0f;
	float totalWeight = 0.0f;
	TQ3Uns32 i;
	
	for (i = 0; i < numPoints; ++i)
	{
		x += rationalPoints3D[i].x * weights[i];
		y += rationalPoints3D[i].y * weights[i];
		w += rationalPoints3D[i].w * weights[i];
		totalWeight += weights[i];
	}

	result->x = x / totalWeight;
	result->y = y / totalWeight;
	result->w = w / totalWeight;
	
	return(result);
}





//=============================================================================
//      E3Point3D_AffineComb :  Return weighted combination of several 3D points.
//-----------------------------------------------------------------------------
//		Note :	Weights are NOT required to sum to 1, but we they must not sum
//				to 0.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_AffineComb(const TQ3Point3D	*points3D,
					 const float		*weights,
					 TQ3Uns32			numPoints,
					 TQ3Point3D			*result)
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float totalWeight = 0.0f;
	TQ3Uns32 i;
	
	for (i = 0; i < numPoints; ++i)
	{
		x += points3D[i].x * weights[i];
		y += points3D[i].y * weights[i];
		z += points3D[i].z * weights[i];
		totalWeight += weights[i];
	}

	result->x = x / totalWeight;
	result->y = y / totalWeight;
	result->z = z / totalWeight;
	
	return(result);
}





//=============================================================================
//      E3RationalPoint4D_AffineComb :	Return weighted combination of several
//										4D rational points.
//-----------------------------------------------------------------------------
//		Note :	Weights are NOT required to sum to 1, but we they must not sum
//				to 0.
//
//				This operation makes no sense mathematically.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
E3RationalPoint4D_AffineComb(const TQ3RationalPoint4D	*rationalPoints4D,
							 const float				*weights,
							 TQ3Uns32					numPoints,
							 TQ3RationalPoint4D			*result)
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
	float totalWeight = 0.0f;
	TQ3Uns32 i;
	
	for (i = 0; i < numPoints; ++i)
	{
		x += rationalPoints4D[i].x * weights[i];
		y += rationalPoints4D[i].y * weights[i];
		z += rationalPoints4D[i].z * weights[i];
		w += rationalPoints4D[i].w * weights[i];
		totalWeight += weights[i];
	}

	result->x = x / totalWeight;
	result->y = y / totalWeight;
	result->z = z / totalWeight;
	result->w = w / totalWeight;
	
	return(result);
}





//=============================================================================
//      E3Vector2D_Transform : Transform 2D vector by 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note :	'result' may be the same as 'vector2D'.
//
//				Note that the translation and perspective components of the
//				matrix is ignored (as if it were really a 2x2 matrix).
//
//				Contrast with E3Point2D_Transform, which does the full 3x3
//				transformation.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Vector2D *
E3Vector2D_Transform(const TQ3Vector2D *vector2D, const TQ3Matrix3x3 *matrix3x3,
	TQ3Vector2D *result)
{
	// Save input to avoid problems when result is same as input
	float x = vector2D->x;
	float y = vector2D->y;
	
	#define M(x,y) matrix3x3->value[x][y]
	result->x = x*M(0,0) + y*M(1,0);
	result->y = x*M(0,1) + y*M(1,1);
	#undef M

	return(result);
}





//=============================================================================
//      E3Vector3D_Transform : Transform 3D vector by 4x4 matrix.
//-----------------------------------------------------------------------------
//		Note :	'result' may be the same as 'vector3D'.
//
//				Note that the translation and perspective components of the
//				matrix is ignored (as if it were really a 3x3 matrix).
//
//				Contrast with E3Point3D_Transform, which does the full 4x4
//				transformation.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_Transform(const TQ3Vector3D *vector3D, const TQ3Matrix4x4 *matrix4x4,
	TQ3Vector3D *result)
{
	// Save input to avoid problems when result is same as input
	float x = vector3D->x;
	float y = vector3D->y;
	float z = vector3D->z;
	
	#define M(x,y) matrix4x4->value[x][y]
	result->x = x*M(0,0) + y*M(1,0) + z*M(2,0);
	result->y = x*M(0,1) + y*M(1,1) + z*M(2,1);
	result->z = x*M(0,2) + y*M(1,2) + z*M(2,2);
	#undef M

	return(result);
}





//=============================================================================
//      E3Point2D_Transform : Transform 2D point by 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'point2D'.
//-----------------------------------------------------------------------------
TQ3Point2D *
E3Point2D_Transform(const TQ3Point2D *point2D, const TQ3Matrix3x3 *matrix3x3,
	TQ3Point2D *result)
{
	// Save input to avoid problems when result is same as input
	float x = point2D->x;
	float y = point2D->y;
	float neww;
	
	#define M(x,y) matrix3x3->value[x][y]
	result->x = x*M(0,0) + y*M(1,0) + M(2,0);
	result->y = x*M(0,1) + y*M(1,1) + M(2,1);
	neww = x*M(0,2) + y*M(1,2) + M(2,2);
	#undef M

	if (neww != 1.0f)
	{
		float invw = 1.0f / neww;
		result->x *= invw;
		result->y *= invw;
	}
	
	return(result);
}





//=============================================================================
//      E3Param2D_Transform : Transform 2D parametric point by 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : Equivalent to E3Point2D_Transform.
//-----------------------------------------------------------------------------
TQ3Param2D *
E3Param2D_Transform(const TQ3Param2D *param2D, const TQ3Matrix3x3 *matrix3x3,
	TQ3Param2D *result)
{
	return((TQ3Param2D*) E3Point2D_Transform((const TQ3Point2D*) param2D, matrix3x3,
		(TQ3Point2D*) result));
}





//=============================================================================
//      E3RationalPoint3D_Transform : Transform 3D rational point by 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'point3D'.
//-----------------------------------------------------------------------------
TQ3RationalPoint3D *
E3RationalPoint3D_Transform(const TQ3RationalPoint3D *rationalPoint3D,
	const TQ3Matrix3x3 *matrix3x3, TQ3RationalPoint3D *result)
{
	// Save input to avoid problems when result is same as input
	float x = rationalPoint3D->x;
	float y = rationalPoint3D->y;
	float w = rationalPoint3D->w;
	
	#define M(x,y) matrix3x3->value[x][y]
	result->x = x*M(0,0) + y*M(1,0) + w*M(2,0);
	result->y = x*M(0,1) + y*M(1,1) + w*M(2,1);
	result->w = x*M(0,2) + y*M(1,2) + w*M(2,2);
	#undef M
	
	return(result);
}





//=============================================================================
//      E3Point3D_Transform : Transform 3D point by 4x4 matrix.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'point3D'.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_Transform(const TQ3Point3D *point3D, const TQ3Matrix4x4 *matrix4x4,
	TQ3Point3D *result)
{
	// Save input to avoid problems when result is same as input
	float x = point3D->x;
	float y = point3D->y;
	float z = point3D->z;
	float neww;
	
	#define M(x,y) matrix4x4->value[x][y]
	result->x = x*M(0,0) + y*M(1,0) + z*M(2,0) + M(3,0);
	result->y = x*M(0,1) + y*M(1,1) + z*M(2,1) + M(3,1);
	result->z = x*M(0,2) + y*M(1,2) + z*M(2,2) + M(3,2);
	neww = x*M(0,3) + y*M(1,3) + z*M(2,3) + M(3,3);
	#undef M

	if (neww != 1.0f)
	{
		float invw = 1.0f / neww;
		result->x *= invw;
		result->y *= invw;
		result->z *= invw;
	}
	
	return(result);
}





//=============================================================================
//      E3RationalPoint4D_Transform : Transform 4D rational point by 4x4 matrix.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'point4D'.
//-----------------------------------------------------------------------------
TQ3RationalPoint4D *
E3RationalPoint4D_Transform(const TQ3RationalPoint4D *rationalPoint4D,
	const TQ3Matrix4x4 *matrix4x4, TQ3RationalPoint4D *result)
{
	// Save input to avoid problems when result is same as input
	float x = rationalPoint4D->x;
	float y = rationalPoint4D->y;
	float z = rationalPoint4D->z;
	float w = rationalPoint4D->w;
	
	#define M(x,y) matrix4x4->value[x][y]
	result->x = x*M(0,0) + y*M(1,0) + z*M(2,0) + w*M(3,0);
	result->y = x*M(0,1) + y*M(1,1) + z*M(2,1) + w*M(3,1);
	result->z = x*M(0,2) + y*M(1,2) + z*M(2,2) + w*M(3,2);
	result->w = x*M(0,3) + y*M(1,3) + z*M(2,3) + w*M(3,3);
	#undef M
	
	return(result);
}





//=============================================================================
//      E3Vector2D_To2DTransformArray :	Transform array of 2D vectors by 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : 'outVectors2D' may be the same as 'inVectors2D'.
//-----------------------------------------------------------------------------
TQ3Status
E3Vector2D_To2DTransformArray(const TQ3Vector2D		*inVectors2D,
							  const TQ3Matrix3x3	*matrix3x3,
							  TQ3Vector2D			*outVectors2D,
							  TQ3Uns32				numVectors,
							  TQ3Uns32				inStructSize,
							  TQ3Uns32				outStructSize)
{
	const char* in = (const char*) inVectors2D;
	char* out = (char*) outVectors2D;
	TQ3Uns32 i;
	
	for (i = 0; i < numVectors; ++i)
	{
		E3Vector2D_Transform((const TQ3Vector2D*) in, matrix3x3, (TQ3Vector2D*) out);
		in += inStructSize;
		out += outStructSize;
	}

	return(kQ3Success);
}





//=============================================================================
//      E3Vector3D_To3DTransformArray :	Transform array of 3D vectors by 4x4 matrix.
//-----------------------------------------------------------------------------
//		Note : 'outVectors3D' may be the same as 'inVectors3D'.
//-----------------------------------------------------------------------------
TQ3Status
E3Vector3D_To3DTransformArray(const TQ3Vector3D		*inVectors3D,
							  const TQ3Matrix4x4	*matrix4x4,
							  TQ3Vector3D			*outVectors3D,
							  TQ3Uns32				numVectors,
							  TQ3Uns32				inStructSize,
							  TQ3Uns32				outStructSize)
{
	const char* in = (const char*) inVectors3D;
	char* out = (char*) outVectors3D;
	TQ3Uns32 i;
	
	for (i = 0; i < numVectors; ++i)
	{
		E3Vector3D_Transform((const TQ3Vector3D*) in, matrix4x4, (TQ3Vector3D*) out);
		in += inStructSize;
		out += outStructSize;
	}

	return(kQ3Success);
}





//=============================================================================
//      E3Point2D_To2DTransformArray :	Transform array of 2D points by 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : 'outPoints2D' may be the same as 'inPoints2D'.
//-----------------------------------------------------------------------------
TQ3Status
E3Point2D_To2DTransformArray(const TQ3Point2D		*inPoints2D,
							 const TQ3Matrix3x3		*matrix3x3,
							 TQ3Point2D				*outPoints2D,
							 TQ3Uns32				numPoints,
							 TQ3Uns32				inStructSize,
							 TQ3Uns32				outStructSize)
{
	const char* in = (const char*) inPoints2D;
	char* out = (char*) outPoints2D;
	TQ3Uns32 i;
	
	for (i = 0; i < numPoints; ++i)
	{
		E3Point2D_Transform((const TQ3Point2D*) in, matrix3x3, (TQ3Point2D*) out);
		in += inStructSize;
		out += outStructSize;
	}

	return(kQ3Success);
}





//=============================================================================
//      E3RationalPoint3D_To3DTransformArray :	Transform array of 3D rational
//												points by 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : 'outRationalPoints3D' may be the same as 'inRationalPoints3D'.
//-----------------------------------------------------------------------------
TQ3Status
E3RationalPoint3D_To3DTransformArray(const TQ3RationalPoint3D	*inRationalPoints3D,
									 const TQ3Matrix3x3			*matrix3x3,
									 TQ3RationalPoint3D			*outRationalPoints3D,
									 TQ3Uns32					numPoints,
									 TQ3Uns32					inStructSize,
									 TQ3Uns32					outStructSize)
{
	const char* in = (const char*) inRationalPoints3D;
	char* out = (char*) outRationalPoints3D;
	TQ3Uns32 i;
	
	for (i = 0; i < numPoints; ++i)
	{
		E3RationalPoint3D_Transform((const TQ3RationalPoint3D*) in, matrix3x3,
			(TQ3RationalPoint3D*) out);
		in += inStructSize;
		out += outStructSize;
	}

	return(kQ3Success);
}





//=============================================================================
//      E3Point3D_To3DTransformArray :	Transform array of 3D points.
//-----------------------------------------------------------------------------
//		Note : 'outPoints3D' may be the same as 'inPoints3D'.
//-----------------------------------------------------------------------------
TQ3Status
E3Point3D_To3DTransformArray(const TQ3Point3D		*inPoints3D,
							 const TQ3Matrix4x4		*matrix4x4,
							 TQ3Point3D				*outPoints3D,
							 TQ3Uns32				numPoints,
							 TQ3Uns32				inStructSize,
							 TQ3Uns32				outStructSize)
{	const char	*in  = (const char*) inPoints3D;
	char		*out = (char *) outPoints3D;
	TQ3Uns32	i;



	// Transform the points - will be in-lined in release builds
	for (i = 0; i < numPoints; ++i)
		{
		E3Point3D_Transform((const TQ3Point3D *) in, matrix4x4, (TQ3Point3D *) out);
		in  += inStructSize;
		out += outStructSize;
		}

	return(kQ3Success);
}





//=============================================================================
//      E3Point3D_To4DTransformArray :	Transform array of 3D points by 4x4
//										matrix into 4D rational points.
//-----------------------------------------------------------------------------
TQ3Status
E3Point3D_To4DTransformArray(const TQ3Point3D		*inPoints3D,
							 const TQ3Matrix4x4		*matrix4x4,
							 TQ3RationalPoint4D		*outRationalPoints4D,
							 TQ3Uns32				numPoints,
							 TQ3Uns32				inStructSize,
							 TQ3Uns32				outStructSize)
{
	const TQ3Point3D* in = inPoints3D;
	TQ3RationalPoint4D* out = outRationalPoints4D;
	TQ3Uns32 i;
	
	for (i = 0; i < numPoints; ++i)
	{
		#define M(x,y) matrix4x4->value[x][y]
		out->x = in->x*M(0,0) + in->y*M(1,0) + in->z*M(2,0) + M(3,0);
		out->y = in->x*M(0,1) + in->y*M(1,1) + in->z*M(2,1) + M(3,1);
		out->z = in->x*M(0,2) + in->y*M(1,2) + in->z*M(2,2) + M(3,2);
		out->w = in->x*M(0,3) + in->y*M(1,3) + in->z*M(2,3) + M(3,3);
		#undef M
		
		in = (TQ3Point3D*) (((const char*) in) + inStructSize);
		out = (TQ3RationalPoint4D*) (((const char*) out) + outStructSize);
	}

	return(kQ3Success);
}





//=============================================================================
//      E3RationalPoint4D_To4DTransformArray :	Transform array of 4D rational
//												points by 4x4 matrix.
//-----------------------------------------------------------------------------
//		Note : 'outRationalPoints4D' may be the same as 'inRationalPoints4D'.
//-----------------------------------------------------------------------------
TQ3Status
E3RationalPoint4D_To4DTransformArray(const TQ3RationalPoint4D	*inRationalPoints4D,
										const TQ3Matrix4x4		*matrix4x4,
										TQ3RationalPoint4D		*outRationalPoints4D,
										TQ3Uns32				numPoints,
										TQ3Uns32				inStructSize,
										TQ3Uns32				outStructSize)
{
	const char* in = (const char*) inRationalPoints4D;
	char* out = (char*) outRationalPoints4D;
	TQ3Uns32 i;
	
	for (i = 0; i < numPoints; ++i)
	{
		E3RationalPoint4D_Transform((const TQ3RationalPoint4D*) in, matrix4x4,
			(TQ3RationalPoint4D*) out);
		in += inStructSize;
		out += outStructSize;
	}

	return(kQ3Success);
}





//=============================================================================
//      E3Matrix3x3_SetIdentity : Set 3x3 identity matrix.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Matrix3x3 *
E3Matrix3x3_SetIdentity(TQ3Matrix3x3 *matrix3x3)
{
	Q3Memory_Clear(matrix3x3, sizeof(TQ3Matrix3x3));

	#define M(x,y) matrix3x3->value[x][y]
	
	M(0,0) = 1.0f;
	
	M(1,1) = 1.0f;
	
	M(2,2) = 1.0f;
	
	#undef M

	return(matrix3x3);
}





//=============================================================================
//      E3Matrix4x4_SetIdentity : Set 4x4 identity matrix.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetIdentity(TQ3Matrix4x4 *matrix4x4)
{
	Q3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));

	#define M(x,y) matrix4x4->value[x][y]
	
	M(0,0) = 1.0f;
	
	M(1,1) = 1.0f;
	
	M(2,2) = 1.0f;
	
	M(3,3) = 1.0f;
	
	#undef M

	return(matrix4x4);
}





//=============================================================================
//      E3Matrix3x3_SetTranslate : Set 3x3 matrix to translate in x, y.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_SetTranslate(TQ3Matrix3x3 *matrix3x3, float xTrans, float yTrans)
{
	Q3Memory_Clear(matrix3x3, sizeof(TQ3Matrix3x3));

	#define M(x,y) matrix3x3->value[x][y]

	M(0,0) = 1.0f;
	
	M(1,1) = 1.0f;
	
	M(2,0) = xTrans;
	M(2,1) = yTrans;
	M(2,2) = 1.0f;
	
	#undef M

	return(matrix3x3);
}





//=============================================================================
//      E3Matrix4x4_SetTranslate : Set 4x4 matrix to translate in x, y, z.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetTranslate(TQ3Matrix4x4 *matrix4x4,
	float xTrans, float yTrans, float zTrans)
{
	Q3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));

	#define M(x,y) matrix4x4->value[x][y]

	M(0,0) = 1.0f;
	
	M(1,1) = 1.0f;
	
	M(2,2) = 1.0f;
	
	M(3,0) = xTrans;
	M(3,1) = yTrans;
	M(3,2) = zTrans;
	M(3,3) = 1.0f;
	
	#undef M

	return(matrix4x4);
}





//=============================================================================
//      E3Matrix3x3_SetScale : Set 3x3 matrix to scale in x, y.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_SetScale(TQ3Matrix3x3 *matrix3x3, float xScale, float yScale)
{
	Q3Memory_Clear(matrix3x3, sizeof(TQ3Matrix3x3));
	
	#define M(x,y) matrix3x3->value[x][y]

	M(0,0) = xScale;
	
	M(1,1) = yScale;
	
	M(2,2) = 1.0f;

	#undef M

	return(matrix3x3);
}





//=============================================================================
//      E3Matrix4x4_SetScale : Set 4x4 matrix to scale in x, y, z.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetScale(TQ3Matrix4x4 *matrix4x4,
	float xScale, float yScale, float zScale)
{
	Q3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));

	#define M(x,y) matrix4x4->value[x][y]
	
	M(0,0) = xScale;
	
	M(1,1) = yScale;
	
	M(2,2) = zScale;
	
	M(3,3) = 1.0f;
	
	#undef M

	return(matrix4x4);
}





//=============================================================================
//      E3Matrix3x3_SetRotate : Set 3x3 matrix to rotate about origin.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_SetRotate(TQ3Matrix3x3 *matrix3x3, float angle)
{
	float cosAngle = (float) cos(angle);
	float sinAngle = (float) sin(angle);

	Q3Memory_Clear(matrix3x3, sizeof(TQ3Matrix3x3));
	
	#define M(x,y) matrix3x3->value[x][y]

	M(0,0) =  cosAngle;
	M(0,1) =  sinAngle;
	
	M(1,0) = -sinAngle;
	M(1,1) =  cosAngle;
	
	M(2,2) =  1.0f;

	#undef M
	
	return(matrix3x3);
}





//=============================================================================
//      E3Matrix4x4_SetRotate_X : Set 4x4 matrix to rotate about X axis.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotate_X(TQ3Matrix4x4 *matrix4x4, float angle)
{
	float cosAngle = (float) cos(angle);
	float sinAngle = (float) sin(angle);

	Q3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));
	
	#define M(x,y) matrix4x4->value[x][y]

	M(0,0) =  1.0f;
	
	M(1,1) =  cosAngle;
	M(1,2) =  sinAngle;
	
	M(2,1) = -sinAngle;
	M(2,2) =  cosAngle;
	
	M(3,3) =  1.0f;

	#undef M
	
	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetRotate_Y : Set 4x4 matrix to rotate about Y axis.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotate_Y(TQ3Matrix4x4 *matrix4x4, float angle)
{
	float cosAngle = (float) cos(angle);
	float sinAngle = (float) sin(angle);

	Q3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));
	
	#define M(x,y) matrix4x4->value[x][y]

	M(0,0) =  cosAngle;
	M(0,2) = -sinAngle;
	
	M(1,1) =  1.0f;
	
	M(2,0) =  sinAngle;
	M(2,2) =  cosAngle;
	
	M(3,3) =  1.0f;

	#undef M
	
	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetRotate_Z : Set 4x4 matrix to rotate about Z axis.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotate_Z(TQ3Matrix4x4 *matrix4x4, float angle)
{
	float cosAngle = (float) cos(angle);
	float sinAngle = (float) sin(angle);

	Q3Memory_Clear(matrix4x4, sizeof(TQ3Matrix4x4));
	
	#define M(x,y) matrix4x4->value[x][y]

	M(0,0) =  cosAngle;
	M(0,1) =  sinAngle;
	
	M(1,0) = -sinAngle;
	M(1,1) =  cosAngle;
	
	M(2,2) =  1.0f;
	
	M(3,3) =  1.0f;

	#undef M
	
	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetRotate_XYZ :	Set 4x4 matrix to rotate about X, Y, Z axes
//									(in that order -- which is rarely useful).
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotate_XYZ(TQ3Matrix4x4 *matrix4x4,
	float xAngle, float yAngle, float zAngle)
{
	float cosX = (float) cos(xAngle);
	float sinX = (float) sin(xAngle);
	float cosY = (float) cos(yAngle);
	float sinY = (float) sin(yAngle);
	float cosZ = (float) cos(zAngle);
	float sinZ = (float) sin(zAngle);
	
	float sinXsinY = sinX*sinY;
	float cosXsinY = cosX*sinY;
	
	#define M(x,y) matrix4x4->value[x][y]

	M(0,0) =  cosY*cosZ;
	M(0,1) =  cosY*sinZ;
	M(0,2) = -sinY;
	M(0,3) =  0.0f;
	
	M(1,0) =  sinXsinY*cosZ - cosX*sinZ;
	M(1,1) =  sinXsinY*sinZ + cosX*cosZ;
	M(1,2) =  sinX*cosY;
	M(1,3) =  0.0f;
	
	M(2,0) =  cosXsinY*cosZ + sinX*sinZ;
	M(2,1) =  cosXsinY*sinZ - sinX*cosZ;
	M(2,2) =  cosX*cosY;
	M(2,3) =  0.0f;
	
	M(3,0) =  0.0f;
	M(3,1) =  0.0f;
	M(3,2) =  0.0f;
	M(3,3) =  1.0f;

	#undef M
	
	return(matrix4x4);
}





//=============================================================================
//      E3Matrix3x3_SetRotateAboutPoint : Set 3x3 matrix to rotate about point.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_SetRotateAboutPoint(TQ3Matrix3x3 *matrix3x3,
	const TQ3Point2D *origin, float angle)
{
	float cosAngle = (float) cos(angle);
	float sinAngle = (float) sin(angle);

	#define M(x,y)	matrix3x3->value[x][y]
	#define Dx		origin->x
	#define Dy		origin->y
	
	M(0,0) =  cosAngle;
	M(0,1) =  sinAngle;
	M(0,2) =  0.0f;
	
	M(1,0) = -sinAngle;
	M(1,1) =  cosAngle;
	M(1,2) =  0.0f;
	
	M(2,0) =  Dx - Dx*cosAngle + Dy*sinAngle;   // = Dx - Dx*M(0,0) - Dy*M(1,0)
	M(2,1) =  Dy - Dx*sinAngle - Dy*cosAngle;   // = Dx - Dx*M(0,1) - Dy*M(1,1)
	M(2,2) =  1.0f;
	
	#undef M
	#undef Dx
	#undef Dy
	
	return(matrix3x3);
}





//=============================================================================
//      E3Matrix4x4_SetRotateAboutPoint : Set 4x4 matrix to rotate about axes through
//										  point and parallel to X, Y, Z axes
//										  (in that order -- which is rarely useful).
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotateAboutPoint(TQ3Matrix4x4 *matrix4x4,
	const TQ3Point3D *origin, float xAngle, float yAngle, float zAngle)
{
	float cosX = (float) cos(xAngle);
	float sinX = (float) sin(xAngle);
	float cosY = (float) cos(yAngle);
	float sinY = (float) sin(yAngle);
	float cosZ = (float) cos(zAngle);
	float sinZ = (float) sin(zAngle);
	
	float sinXsinY = sinX*sinY;
	float cosXsinY = cosX*sinY;

	#define M(x,y)	matrix4x4->value[x][y]
	#define Dx		origin->x
	#define Dy		origin->y
	#define Dz		origin->z

	M(0,0) =  cosY*cosZ;
	M(0,1) =  cosY*sinZ;
	M(0,2) = -sinY;
	M(0,3) =  0.0f;
	
	M(1,0) =  sinXsinY*cosZ - cosX*sinZ;
	M(1,1) =  sinXsinY*sinZ + cosX*cosZ;
	M(1,2) =  sinX*cosY;
	M(1,3) =  0.0f;
	
	M(2,0) =  cosXsinY*cosZ + sinX*sinZ;
	M(2,1) =  cosXsinY*sinZ - sinX*cosZ;
	M(2,2) =  cosX*cosY;
	M(2,3) =  0.0f;
	
	M(3,0) =  Dx - Dx*M(0,0) - Dy*M(1,0) - Dz*M(2,0);
	M(3,1) =  Dy - Dx*M(0,1) - Dy*M(1,1) - Dz*M(2,1);
	M(3,2) =  Dz - Dx*M(0,2) - Dy*M(1,2) - Dz*M(2,2);
	M(3,3) =  1.0f;

	#undef M
	#undef Dx
	#undef Dy
	#undef Dz
	
	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetRotateAboutAxis :	Set 4x4 matrix to rotate about
//											arbitrary origin and axis.
//-----------------------------------------------------------------------------
//		Note :	See 'Rotation Tools', pp. 465-469, by Michael E. Pique, and
//				'Matrices and Transformations', pp. 472-475, by Ronald Goldman,
//				both in "Graphics Gems".
//
//				For correct results, |axis| == 1.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotateAboutAxis(TQ3Matrix4x4 *matrix4x4,
	const TQ3Point3D *origin, const TQ3Vector3D *axis, float angle)
{
	float c = (float) cos(angle);
	float s = (float) sin(angle);
	float t = 1 - c;
	float tx = t*axis->x;
	float ty = t*axis->y;
	float tz = t*axis->z;
	float sx = s*axis->x;
	float sy = s*axis->y;
	float sz = s*axis->z;

	#define M(x,y)	matrix4x4->value[x][y]
	#define Dx		origin->x
	#define Dy		origin->y
	#define Dz		origin->z

	M(0,0) = tx*axis->x + c;
	M(0,1) = tx*axis->y + sz;
	M(0,2) = tx*axis->z - sy;
	M(0,3) = 0.0f;
	
	M(1,0) = ty*axis->x - sz;
	M(1,1) = ty*axis->y + c;
	M(1,2) = ty*axis->z + sx;
	M(1,3) = 0.0f;
	
	M(2,0) = tz*axis->x + sy;
	M(2,1) = tz*axis->y - sx;
	M(2,2) = tz*axis->z + c;
	M(2,3) = 0.0f;
	
	M(3,0) = Dx - Dx*M(0,0) - Dy*M(1,0) - Dz*M(2,0);
	M(3,1) = Dy - Dx*M(0,1) - Dy*M(1,1) - Dz*M(2,1);
	M(3,2) = Dz - Dx*M(0,2) - Dy*M(1,2) - Dz*M(2,2);
	M(3,3) = 1.0f;

	#undef M
	#undef Dx
	#undef Dy
	#undef Dz
	
	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetRotateVectorToVector :	Set 4x4 matrix to rotate 'v1'
//												to 'v2'.
//-----------------------------------------------------------------------------
//		Note :	See Foley, et al., 2nd ed., pp. 220-222.
//
//				For correct results, |v1| == 1 && |v2| == 1.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetRotateVectorToVector(TQ3Matrix4x4 *matrix4x4,
	const TQ3Vector3D *v1, const TQ3Vector3D *v2)
{
	// We accomplish the rotation by creating two orthonormal vector triads:
	//
	//		(u, v, w) -> (u', v', w)
	//
	// The rotation is about the axis w. It rotates u (=v1) into u' (=v2). It
	// also rotates v into v', which are vectors rotated 90 degrees in the plane
	// of rotation from u and u', respectively.
	//
	// To construct the rotation matrix, we rotate into and out of the basis
	// vectors i, j, k:
	//
	//		(u, v, w) -> (i, j, k) -> (u', v', w)
	//
	// Thus the rotation matrix is the product of two rotation matrices, a and b:
	//
	//		| ux vx wx |   | ux' uy' uz' |
	//		| uy vy wy | . | vx' vy' vz' |
	//		| uz vz wz |   | wx  wy  wz  |
	//
	// To see this, simply multiply this composite matrix by the u, v or w row
	// vector on the left and see that the result is u', v' or w, respectively.
	
	TQ3Vector3D u, uPrime, v, vPrime, w;
	TQ3Matrix3x3 a, b;

	// Construct vector w (axis of rotation) perpendicular to v1 and v2	
	E3Vector3D_Cross(v1, v2, &w);
	
	// Check if vector w is roughly zero
	if (E3Vector3D_IsBelowTolerance(&w, 10.0f*FLT_EPSILON))
	{
		// Vectors v1 and v2 are approximately parallel or approximately anti-parallel
		// (within 1.192092896e-07 radians or roughly 0.000007 degrees!)

		TQ3Vector3D v2Proxy;
		TQ3Int32 iSmall, i;
		float valueSmall;
		
		// Determine v1 component of smallest absolute value
		iSmall = 0;
		valueSmall = (float) fabs(v1->x);
		for (i = 1; i < 3; ++i)
		{
			float value;
			
			value = (float) fabs(((float*) (v1))[i]);
			if (value < valueSmall)
			{
				iSmall = i;
				valueSmall = value;
			}
		}
		
		// Construct corresponding basis vector
		for (i = 0; i < 3; ++i)
			((float*) (&v2Proxy))[i] = (i == iSmall ? 1.0f : 0.0f);
		
		// Construct vector w (axis of rotation) perpendicular to v1 and v2Proxy
		E3Vector3D_Cross(v1, &v2Proxy, &w);
	}
	
	E3Vector3D_Normalize(&w, &w);
	
	u = *v1;
	uPrime = *v2;
	E3Vector3D_Cross(&w, &u, &v);
	E3Vector3D_Cross(&w, &uPrime, &vPrime);

	#define A(x,y)	a.value[x][y]
	#define B(x,y)	b.value[x][y]
	#define M(x,y)	matrix4x4->value[x][y]
	
	A(0,0) = u.x;
	A(0,1) = v.x;
	A(0,2) = w.x;

	A(1,0) = u.y;
	A(1,1) = v.y;
	A(1,2) = w.y;
				
	A(2,0) = u.z;
	A(2,1) = v.z;
	A(2,2) = w.z;
	
	B(0,0) = uPrime.x;
	B(0,1) = uPrime.y;
	B(0,2) = uPrime.z;

	B(1,0) = vPrime.x;
	B(1,1) = vPrime.y;
	B(1,2) = vPrime.z;
				
	B(2,0) = w.x;
	B(2,1) = w.y;
	B(2,2) = w.z;
	
	// Now multiply the two 3x3 matrices a and b to get the 3x3 part of the result,
	// filling out the rest of the result as an identity matrix (since we are
	// rotating about the point <0,0,0>)
	M(0,0) = A(0,0)*B(0,0) + A(0,1)*B(1,0) + A(0,2)*B(2,0);
	M(0,1) = A(0,0)*B(0,1) + A(0,1)*B(1,1) + A(0,2)*B(2,1);
	M(0,2) = A(0,0)*B(0,2) + A(0,1)*B(1,2) + A(0,2)*B(2,2);
	M(0,3) = 0.0f;
	
	M(1,0) = A(1,0)*B(0,0) + A(1,1)*B(1,0) + A(1,2)*B(2,0);
	M(1,1) = A(1,0)*B(0,1) + A(1,1)*B(1,1) + A(1,2)*B(2,1);
	M(1,2) = A(1,0)*B(0,2) + A(1,1)*B(1,2) + A(1,2)*B(2,2);
	M(1,3) = 0.0f;
	
	M(2,0) = A(2,0)*B(0,0) + A(2,1)*B(1,0) + A(2,2)*B(2,0);
	M(2,1) = A(2,0)*B(0,1) + A(2,1)*B(1,1) + A(2,2)*B(2,1);
	M(2,2) = A(2,0)*B(0,2) + A(2,1)*B(1,2) + A(2,2)*B(2,2);
	M(2,3) = 0.0f;
	
	M(3,0) = 0.0f;
	M(3,1) = 0.0f;
	M(3,2) = 0.0f;
	M(3,3) = 1.0f;

	#undef A
	#undef B
	#undef M

	return(matrix4x4);
}





//=============================================================================
//      E3Matrix4x4_SetQuaternion : Set 4x4 matrix from quaternion.
//-----------------------------------------------------------------------------
//		Note :	See http://www.gamasutra.com/features/19980703/quaternions_01.htm
//				Since Quesa uses row vectors rather than the more conventional
//				column vectors, we multiply in the opposite order: row*matrix rather
//				matrix*column. Thus our matrices are the transpose of theirs.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_SetQuaternion(TQ3Matrix4x4 *matrix4x4, const TQ3Quaternion *quaternion)
{
	// calculate coefficients
	float x2 = quaternion->x + quaternion->x;
	float y2 = quaternion->y + quaternion->y;
	float z2 = quaternion->z + quaternion->z;
	float xx = quaternion->x * x2;
	float xy = quaternion->x * y2;
	float xz = quaternion->x * z2;
	float yy = quaternion->y * y2;
	float yz = quaternion->y * z2;
	float zz = quaternion->z * z2;
	float wx = quaternion->w * x2;
	float wy = quaternion->w * y2;
	float wz = quaternion->w * z2;

	#define M(x,y) matrix4x4->value[x][y]
	
	M(0,0) = 1.0f - (yy + zz);
	M(0,1) = xy + wz;
	M(0,2) = xz - wy;
	M(0,3) = 0.0f;

	M(1,0) = xy - wz;
	M(1,1) = 1.0f - (xx + zz);
	M(1,2) = yz + wx;
	M(1,3) = 0.0f;
	
	M(2,0) = xz + wy;
	M(2,1) = yz - wx;
	M(2,2) = 1.0f - (xx + yy);
	M(2,3) = 0.0f;
	
	M(3,0) = 0.0f;
	M(3,1) = 0.0f;
	M(3,2) = 0.0f;
	M(3,3) = 1.0f;
	
	#undef M

	return(matrix4x4);
}





//=============================================================================
//      E3Matrix3x3_Copy : Copy 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'matrix3x3'.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_Copy(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{
	if (result != matrix3x3)
		*result = *matrix3x3;

	return(result);
}





//=============================================================================
//      E3Matrix4x4_Copy : Copy 4x4 matrix.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'matrix4x4'.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_Copy(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result)
{
	if (result != matrix4x4)
		*result = *matrix4x4;

	return(result);
}





//=============================================================================
//      E3Matrix3x3_Transpose : Transpose 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same 'matrix3x3'.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_Transpose(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{
	TQ3Int32 i, j;
	
	if (result != matrix3x3)
	{
		for (i = 0; i < 3; ++i)
			for (j = 0; j < 3; ++j)
				result->value[i][j] = matrix3x3->value[j][i];
	}
	else
	{
		E3Float_Swap(result->value[1][0], result->value[0][1]);
		E3Float_Swap(result->value[2][0], result->value[0][2]);
		E3Float_Swap(result->value[1][2], result->value[2][1]);
	}
	return(result);
}





//=============================================================================
//      E3Matrix4x4_Transpose : Transpose 4x4 matrix.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same 'matrix4x4'.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_Transpose(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result)
{
	TQ3Int32 i, j;
	
	if (result != matrix4x4)
	{
		for (i = 0; i < 4; ++i)
			for (j = 0; j < 4; ++j)
				result->value[i][j] = matrix4x4->value[j][i];
	}
	else
	{
		E3Float_Swap(result->value[1][0], result->value[0][1]);
		E3Float_Swap(result->value[2][0], result->value[0][2]);
		E3Float_Swap(result->value[3][0], result->value[0][3]);
		E3Float_Swap(result->value[2][1], result->value[1][2]);
		E3Float_Swap(result->value[3][1], result->value[1][3]);
		E3Float_Swap(result->value[2][3], result->value[3][2]);
	}
	return(result);
}





//=============================================================================
//      E3Matrix3x3_Determinant : Return determinant of 3x3 matrix.
//-----------------------------------------------------------------------------
float
E3Matrix3x3_Determinant(const TQ3Matrix3x3 *matrix3x3)
{
	TQ3Int32 i, j;
	float *a[3];
	float temp[3][3];
	
	for (i = 0; i < 3; ++i)
	{
		a[i] = temp[i];
		for (j = 0; j < 3; ++j)
			temp[i][j] = matrix3x3->value[i][j];
	}
		
	return(e3matrix_determinant(a, 3));
}





//=============================================================================
//      E3Matrix4x4_Determinant : Return determinant of 4x4 matrix.
//-----------------------------------------------------------------------------
float
E3Matrix4x4_Determinant(const TQ3Matrix4x4 *matrix4x4)
{
	TQ3Int32 i, j;
	float *a[4];
	float temp[4][4];
	
	for (i = 0; i < 4; ++i)
	{
		a[i] = temp[i];
		for (j = 0; j < 4; ++j)
			temp[i][j] = matrix4x4->value[i][j];
	}
		
	return(e3matrix_determinant(a, 4));
}





//=============================================================================
//      E3Matrix3x3_Adjoint : Calculate adjoint of 3x3 matrix.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same 'matrix3x3'.
//
//				The adjoint of a matrix is a scalar multiple of the inverse of
//				the matrix. For some applications, the adjoint can be used in
//				place of the inverse. In particular:
//
//					adjoint(A) = determinant(A) * inverse(A)
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_Adjoint(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{
	// If result is alias of input, output to temporary
	TQ3Matrix3x3 temp;
	TQ3Matrix3x3* output = (result == matrix3x3 ? &temp : result);
	
	#define A(x,y) matrix3x3->value[x][y]
	#define B(x,y) output->value[y][x]
	
	//  B(x,y) is set up to do the transpose for us!
	
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
	
	if (output == &temp)
		*result = temp;
	
	return(result);
}





//=============================================================================
//      E3Matrix3x3_Invert : Invert 3x3 non-singular matrix.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same 'matrix3x3'.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_Invert(const TQ3Matrix3x3 *matrix3x3, TQ3Matrix3x3 *result)
{
	TQ3Int32 i;
	TQ3Int32 ipiv[3], indxr[3], indxc[3];
	float *a[3];

	if (result != matrix3x3)
		*result = *matrix3x3;
			
	for (i = 0; i < 3; ++i)
		a[i] = result->value[i];
		
	e3matrix_invert(a, 3, ipiv, indxr, indxc);
		
	return(result);
}





//=============================================================================
//      E3Matrix4x4_Invert : Invert 4x4 non-singular matrix.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same 'matrix4x4'.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_Invert(const TQ3Matrix4x4 *matrix4x4, TQ3Matrix4x4 *result)
{
	TQ3Int32 i;
	TQ3Int32 ipiv[4], indxr[4], indxc[4];
	float *a[4];

	if (result != matrix4x4)
		*result = *matrix4x4;
			
	for (i = 0; i < 4; ++i)
		a[i] = result->value[i];
		
	e3matrix_invert(a, 4, ipiv, indxr, indxc);
		
	return(result);
}





//=============================================================================
//      E3Matrix3x3_Multiply : Multiply two 3x3 matrices.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'm1' and/or 'm2'.
//-----------------------------------------------------------------------------
TQ3Matrix3x3 *
E3Matrix3x3_Multiply(const TQ3Matrix3x3 *m1, const TQ3Matrix3x3 *m2, TQ3Matrix3x3 *result)
{
	// If result is alias of input, output to temporary
	TQ3Matrix3x3 temp;
	TQ3Matrix3x3* output = (result == m1 || result == m2 ? &temp : result);
	
	//  * I unrolled the for() loops we were using here to cut any possible overhead
	//	  in the matrix multiplication.  -JTF 08/08/1999
	
	#define A(x,y)	m1->value[x][y]
	#define B(x,y)	m2->value[x][y]
	#define M(x,y)	output->value[x][y]
	
	M(0,0) = A(0,0)*B(0,0) + A(0,1)*B(1,0) + A(0,2)*B(2,0);
	M(0,1) = A(0,0)*B(0,1) + A(0,1)*B(1,1) + A(0,2)*B(2,1);
	M(0,2) = A(0,0)*B(0,2) + A(0,1)*B(1,2) + A(0,2)*B(2,2);
	
	M(1,0) = A(1,0)*B(0,0) + A(1,1)*B(1,0) + A(1,2)*B(2,0);
	M(1,1) = A(1,0)*B(0,1) + A(1,1)*B(1,1) + A(1,2)*B(2,1);
	M(1,2) = A(1,0)*B(0,2) + A(1,1)*B(1,2) + A(1,2)*B(2,2);
	
	M(2,0) = A(2,0)*B(0,0) + A(2,1)*B(1,0) + A(2,2)*B(2,0);
	M(2,1) = A(2,0)*B(0,1) + A(2,1)*B(1,1) + A(2,2)*B(2,1);
	M(2,2) = A(2,0)*B(0,2) + A(2,1)*B(1,2) + A(2,2)*B(2,2);
	
	#undef A
	#undef B
	#undef C

	if (output == &temp)
		*result = temp;

	return(result);
}





//=============================================================================
//      E3Matrix4x4_Multiply : Multiply two 4x4 matrices.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'm1' and/or 'm2'.
//-----------------------------------------------------------------------------
TQ3Matrix4x4 *
E3Matrix4x4_Multiply(const TQ3Matrix4x4 *m1, const TQ3Matrix4x4 *m2, TQ3Matrix4x4 *result)
{
	// If result is alias of input, output to temporary
	TQ3Matrix4x4 temp;
	TQ3Matrix4x4* output = (result == m1 || result == m2 ? &temp : result);
	
	//  * I unrolled the for() loops we were using here to cut any possible overhead
	//	  in the matrix multiplication.  -JTF 08/08/1999
	
	#define A(x,y)	m1->value[x][y]
	#define B(x,y)	m2->value[x][y]
	#define M(x,y)	output->value[x][y]
	
	M(0,0) = A(0,0)*B(0,0) + A(0,1)*B(1,0) + A(0,2)*B(2,0) + A(0,3)*B(3,0);
	M(0,1) = A(0,0)*B(0,1) + A(0,1)*B(1,1) + A(0,2)*B(2,1) + A(0,3)*B(3,1);
	M(0,2) = A(0,0)*B(0,2) + A(0,1)*B(1,2) + A(0,2)*B(2,2) + A(0,3)*B(3,2);
	M(0,3) = A(0,0)*B(0,3) + A(0,1)*B(1,3) + A(0,2)*B(2,3) + A(0,3)*B(3,3);
	
	M(1,0) = A(1,0)*B(0,0) + A(1,1)*B(1,0) + A(1,2)*B(2,0) + A(1,3)*B(3,0);
	M(1,1) = A(1,0)*B(0,1) + A(1,1)*B(1,1) + A(1,2)*B(2,1) + A(1,3)*B(3,1);
	M(1,2) = A(1,0)*B(0,2) + A(1,1)*B(1,2) + A(1,2)*B(2,2) + A(1,3)*B(3,2);
	M(1,3) = A(1,0)*B(0,3) + A(1,1)*B(1,3) + A(1,2)*B(2,3) + A(1,3)*B(3,3);
	
	M(2,0) = A(2,0)*B(0,0) + A(2,1)*B(1,0) + A(2,2)*B(2,0) + A(2,3)*B(3,0);
	M(2,1) = A(2,0)*B(0,1) + A(2,1)*B(1,1) + A(2,2)*B(2,1) + A(2,3)*B(3,1);
	M(2,2) = A(2,0)*B(0,2) + A(2,1)*B(1,2) + A(2,2)*B(2,2) + A(2,3)*B(3,2);
	M(2,3) = A(2,0)*B(0,3) + A(2,1)*B(1,3) + A(2,2)*B(2,3) + A(2,3)*B(3,3);
	
	M(3,0) = A(3,0)*B(0,0) + A(3,1)*B(1,0) + A(3,2)*B(2,0) + A(3,3)*B(3,0);
	M(3,1) = A(3,0)*B(0,1) + A(3,1)*B(1,1) + A(3,2)*B(2,1) + A(3,3)*B(3,1);
	M(3,2) = A(3,0)*B(0,2) + A(3,1)*B(1,2) + A(3,2)*B(2,2) + A(3,3)*B(3,2);
	M(3,3) = A(3,0)*B(0,3) + A(3,1)*B(1,3) + A(3,2)*B(2,3) + A(3,3)*B(3,3);
	
	#undef A
	#undef B
	#undef M

	if (output == &temp)
		*result = temp;

	return(result);
}





//=============================================================================
//      E3Quaternion_Set : Set quaternion.
//-----------------------------------------------------------------------------
#pragma mark -
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
//      E3Quaternion_SetIdentity : Set quaternion to (1,0,0,0).
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetIdentity(TQ3Quaternion *quaternion)
{
	quaternion->w = 1.0f;
	quaternion->x = quaternion->y = quaternion->z = 0.0f;

	return(quaternion);
}





//=============================================================================
//      E3Quaternion_SetRotate_X : Set quaternion to rotate about X axis.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotate_X(TQ3Quaternion *quaternion, float angle)
{
	float halfAngle = 0.5f*angle;
	
	quaternion->w = (float) cos(halfAngle);
	quaternion->x = (float) sin(halfAngle);
	quaternion->y = 0.0f;
	quaternion->z = 0.0f;
	
	return(quaternion);
}





//=============================================================================
//      E3Quaternion_SetRotate_Y : Set quaternion to rotate about Y axis.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotate_Y(TQ3Quaternion *quaternion, float angle)
{
	float halfAngle = 0.5f*angle;
	
	quaternion->w = (float) cos(halfAngle);
	quaternion->x = 0.0f;
	quaternion->y = (float) sin(halfAngle);
	quaternion->z = 0.0f;
	
	return(quaternion);
}





//=============================================================================
//      E3Quaternion_SetRotate_Z : Set quaternion to rotate about Z axis.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotate_Z(TQ3Quaternion *quaternion, float angle)
{
	float halfAngle = 0.5f*angle;
	
	quaternion->w = (float) cos(halfAngle);
	quaternion->x = 0.0f;
	quaternion->y = 0.0f;
	quaternion->z = (float) sin(halfAngle);
	
	return(quaternion);
}





//=============================================================================
//      E3Quaternion_SetRotate_XYZ :	Set quaternion to rotate about X, Y, Z
//										axes (in that order -- which is rarely
//										useful).
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotate_XYZ(TQ3Quaternion *quaternion,
	float xAngle, float yAngle, float zAngle)
{
	float xHalfAngle = 0.5f*xAngle;
	float yHalfAngle = 0.5f*yAngle;
	float zHalfAngle = 0.5f*zAngle;

	float cosHalfX = (float) cos(xHalfAngle);
	float sinHalfX = (float) sin(xHalfAngle);
	float cosHalfY = (float) cos(yHalfAngle);
	float sinHalfY = (float) sin(yHalfAngle);
	float cosHalfZ = (float) cos(zHalfAngle);
	float sinHalfZ = (float) sin(zHalfAngle);

	float cosHalfYcosHalfZ = cosHalfY*cosHalfZ;
	float sinHalfYsinHalfZ = sinHalfY*sinHalfZ;

	quaternion->w = cosHalfX*cosHalfYcosHalfZ  + sinHalfX*sinHalfYsinHalfZ;
	quaternion->x = sinHalfX*cosHalfYcosHalfZ  - cosHalfX*sinHalfYsinHalfZ;
	quaternion->y = cosHalfX*sinHalfY*cosHalfZ + sinHalfX*cosHalfY*sinHalfZ;
	quaternion->z = cosHalfX*cosHalfY*sinHalfZ - sinHalfX*sinHalfY*cosHalfZ;
	
	return(quaternion);
}





//=============================================================================
//      E3Quaternion_SetRotateAboutAxis :	Set quaternion to rotate about
//											arbitrary axis.
//-----------------------------------------------------------------------------
//		Note : For correct results, |axis| == 1.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotateAboutAxis(TQ3Quaternion *quaternion,
	const TQ3Vector3D *axis, float angle)
{
	float halfAngle = 0.5f*angle;
	float cosHalfAngle = (float) cos(halfAngle);
	float sinHalfAngle = (float) sin(halfAngle);

	quaternion->w = cosHalfAngle;
	quaternion->x = axis->x * sinHalfAngle;
	quaternion->y = axis->y * sinHalfAngle;
	quaternion->z = axis->z * sinHalfAngle;
	
	// ahhh... this is so much easier than matrices...
	return(quaternion);
}





//=============================================================================
//      E3Quaternion_SetRotateVectorToVector :	Set quaternion to rotate 'v1'
//												to 'v2'.
//-----------------------------------------------------------------------------
//		Note : For correct results, |v1| == 1 && |v2| == 1.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetRotateVectorToVector(TQ3Quaternion *quaternion,
	const TQ3Vector3D *v1, const TQ3Vector3D *v2)
{
	float cosAngle;
	TQ3Vector3D axis;
	
	cosAngle = E3Vector3D_Dot(v1, v2);
	E3Vector3D_Cross(v1, v2, &axis);
	
	// Note: sin(angle) = |axis|
		
	if (! E3Vector3D_IsBelowTolerance(&axis, 10.0f*FLT_EPSILON))
	{
		// Vectors are neither approximately parallel nor approximately anti-parallel
		
		float cosHalfAngle;
		float factor;
		
		cosHalfAngle = (float) sqrt((1 + cosAngle) * 0.5f);
		
		// Note: sin(angle/2) = sin(angle) / (2 * cos(angle/2)) = sin(angle) * factor
		
		factor = 1.0f / (2.0f * cosHalfAngle);
		
		quaternion->w = cosHalfAngle;
		quaternion->x = axis.x * factor;
		quaternion->y = axis.y * factor;
		quaternion->z = axis.z * factor;
	}
	else
	{
		// Vectors are approximately parallel or approximately anti-parallel
		
		// cos(angle) is approximately +1 or approximately -1
		
		if (cosAngle < 0.0f)
		{
			// Vectors are approximately anti-parallel
			
			TQ3Vector3D v2Prime;
			TQ3Int32 iSmall, i;
			float valueSmall;
			float factor;
			
			// Determine v1 component of smallest in absolute value
			iSmall = 0;
			valueSmall = (float) fabs (v1->x);
			for (i = 1; i < 3; ++i)
			{
				float value;
				
				value = (float) fabs(((float*) v1)[i]);
				if (value < valueSmall)
				{
					iSmall = i;
					valueSmall = value;
				}
			}
			
			// Construct corresponding basis vector
			for (i = 0; i < 3; ++i)
				((float*) &v2Prime)[i] = (i == iSmall ? 1.0f : 0.0f);
			
			// Axis is vector perpendicular to v1 and v2Prime
			E3Vector3D_Cross(v1, &v2Prime, &axis);
			
			// Note: 1 = sin(angle/2) = |axis| * factor
			
			factor = 1.0f / E3Vector3D_Length(&axis);
			
			quaternion->w = 0.0f;
			quaternion->x = axis.x * factor;
			quaternion->y = axis.y * factor;
			quaternion->z = axis.z * factor;
		}
		else
		{
			// Vectors are approximately parallel
			
			quaternion->w = 1.0f;
			quaternion->x = 0.0f;
			quaternion->y = 0.0f;
			quaternion->z = 0.0f;
		}
	}
	
	return(quaternion);
}





//=============================================================================
//      E3Quaternion_SetMatrix : Set quaternion from rotation matrix.
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
//				http://www.gamasutra.com/features/19980703/quaternions_01.htm
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_SetMatrix(TQ3Quaternion *quaternion, const TQ3Matrix4x4 *matrix4x4)
{
	float tr, s, q[4];
	TQ3Uns32 i, j, k;

	TQ3Uns32 nxt[3] = {1, 2, 0};

	#define M(x,y) matrix4x4->value[x][y]
	
	tr = M(0,0) + M(1,1) + M(2,2);

	// check the diagonal
	if (tr > 0.0) {
		s = (float)sqrt(tr + 1.0);
		quaternion->w = s / 2.0f;
		s = 0.5f / s;
		quaternion->x = (M(1,2) - M(2,1)) * s;
		quaternion->y = (M(2,0) - M(0,2)) * s;
		quaternion->z = (M(0,1) - M(1,0)) * s;
	}
	else
	{
		// diagonal is negative
		i = 0;
		if (M(1,1) > M(0,0)) i = 1;
		if (M(2,2) > M(i,i)) i = 2;
		j = nxt[i];
		k = nxt[j];

		s = (float) sqrt ((M(i,i) - (M(j,j) + M(k,k))) + 1.0);

		q[i] = s * 0.5f;

		if (s != 0.0f) s = 0.5f / s;

		q[3] = (M(j,k) - M(k,j)) * s;
		q[j] = (M(i,j) + M(j,i)) * s;
		q[k] = (M(i,k) + M(k,i)) * s;

		quaternion->x = q[0];
		quaternion->y = q[1];
		quaternion->z = q[2];
		quaternion->w = q[3];
	}
	
	#undef M
	
	return(quaternion);
}





//=============================================================================
//      E3Quaternion_Copy : Copy quaternion.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'quaternion'.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_Copy(const TQ3Quaternion *quaternion, TQ3Quaternion *result)
{
	if (result != quaternion)
		*result = *quaternion;

	return(result);
}





//=============================================================================
//      E3Quaternion_IsIdentity : Return if quaternion is (1,0,0,0).
//-----------------------------------------------------------------------------
//		Note :	The QuickDraw 3D 1.6 version appears to tolerate x, y or z
//				within FLT_EPSILON of 0.
//
//				Note : For correct results, |quaternion] == 1.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Quaternion_IsIdentity(const TQ3Quaternion *quaternion)
{
	return((TQ3Boolean)
	(
		quaternion->x <= FLT_EPSILON && quaternion->x >= -FLT_EPSILON &&
		quaternion->y <= FLT_EPSILON && quaternion->y >= -FLT_EPSILON &&
		quaternion->z <= FLT_EPSILON && quaternion->z >= -FLT_EPSILON
	));
}





//=============================================================================
//      E3Quaternion_Dot : Return dot product of q1 and q2.
//-----------------------------------------------------------------------------
float
E3Quaternion_Dot(const TQ3Quaternion *q1, const TQ3Quaternion *q2)
{
	return(q1->w*q2->w + q1->x*q2->x + q1->y*q2->y + q1->z*q2->z);
}





//=============================================================================
//      E3Quaternion_Normalize : Scale quaternion to length 1.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'quaternion'.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_Normalize(const TQ3Quaternion *quaternion, TQ3Quaternion *result)
{
	float factor = 1.0f / (float) sqrt(E3Quaternion_Dot(quaternion, quaternion));
	
	result->w = quaternion->w * factor;
	result->x = quaternion->x * factor;
	result->y = quaternion->y * factor;
	result->z = quaternion->z * factor;

	return(result);
}





//=============================================================================
//      E3Quaternion_Invert : Invert quaternion.
//-----------------------------------------------------------------------------
//		Note :	'result' may be the same as 'quaternion'.
//
//				For correct results, |quaternion] == 1.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_Invert(const TQ3Quaternion *quaternion, TQ3Quaternion *result)
{
	result->w =  quaternion->w;
	result->x = -quaternion->x;
	result->y = -quaternion->y;
	result->z = -quaternion->z;
	
	return(result);
}





//=============================================================================
//      E3Quaternion_Multiply : Multiply 'q2' by 'q1'.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'q1' and/or 'q2'.
//
//				There is another algorithm which reduces the multiplies
//				but increases the adds.  Probably not a big difference on
//				the PowerPC.
//
//				This function actually multiplies the two quaternions in
//				the reverse order. Since Quesa uses row vectors rather than
//				the more conventional column vectors, we multiply in the
//				opposite order: row*matrix rather matrix*column. In order
//				to make quaternion and matrix multiplication correspond, it
//				is necessary to include this "hack" which allows us to
//				to think of composing transforms/matrices/quaternions
//				from left-to-right while implementing the conventional
//				quaternion composition from right-to-left.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_Multiply(const TQ3Quaternion *q1, const TQ3Quaternion *q2, TQ3Quaternion *result)
{
	// If result is alias of input, output to temporary
	TQ3Quaternion temp;
	TQ3Quaternion* output = (result == q1 || result == q2 ? &temp : result);

	// Forward multiplication (q1 * q2)
	/*
    result->w = q1->w*q2->w - q1->x*q2->x - q1->y*q2->y - q1->z*q2->z;
    result->x = q1->w*q2->x + q1->x*q2->w + q1->y*q2->z - q1->z*q2->y;
    result->y = q1->w*q2->y + q1->y*q2->w + q1->z*q2->x - q1->x*q2->z;
    result->z = q1->w*q2->z + q1->z*q2->w + q1->x*q2->y - q1->y*q2->x;
    */

	// Reverse multiplication (q2 * q1)
	output->w = q1->w*q2->w - q1->x*q2->x - q1->y*q2->y - q1->z*q2->z;
	output->x = q1->w*q2->x + q1->x*q2->w - q1->y*q2->z + q1->z*q2->y;
	output->y = q1->w*q2->y + q1->y*q2->w - q1->z*q2->x + q1->x*q2->z;
	output->z = q1->w*q2->z + q1->z*q2->w - q1->x*q2->y + q1->y*q2->x;
	
	if (output == &temp)
		*result = temp;
	
	return(result);
}





//=============================================================================
//      E3Quaternion_MatchReflection : sets result to either q1 or -q1,
//				whichever produces a positive dot product with q2.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_MatchReflection(const TQ3Quaternion *q1, const TQ3Quaternion *q2,
	TQ3Quaternion *result)
{
	float dot = q1->w*q2->w + q1->x*q2->x + q1->y*q2->y + q1->z*q2->z;
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
//		Note :	This does a true linear, not spherical, interpolation between
//				q1 and q2.  Fast, but not very proper for most uses.
//
//				Untested.
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
//		Note : Untested.
//-----------------------------------------------------------------------------
//		Note :	Despite the name, this function does a SLERP from q1 to q2.
//				It falls back on a straight linear interpolation only when the
//				cosine of the angle between them is less than 0.01.
//				
//				The cut-off point was chosen arbitrarily, and may not match
//				that of QD3D.
//
//				This code adapted from:
//				http://www.gamasutra.com/features/19980703/quaternions_01.htm
//
//				Untested.
//-----------------------------------------------------------------------------
TQ3Quaternion *
E3Quaternion_InterpolateLinear(const TQ3Quaternion *q1, const TQ3Quaternion *q2, float t, TQ3Quaternion *result)
{
	float	to1[4];
	float	omega, cosom, sinom, scale0, scale1;

	// calc cosine
	cosom = q1->x*q2->x + q1->y*q2->y + q1->z*q2->z
	             + q1->w*q2->w;

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
	result->x = scale0*q1->x + scale1*to1[0];
	result->y = scale0*q1->y + scale1*to1[1];
	result->z = scale0*q1->z + scale1*to1[2];
	result->w = scale0*q1->w + scale1*to1[3];
	return(result);
}


//=============================================================================
//      E3Quaternion_GetAxisAndAngle : Get the rotation axis and angle
//				represented by a quaternion.
//-----------------------------------------------------------------------------
//		Note :	'outAxis' or 'outAngle' may be NULL.
//
//				For correct results, |quaternion] == 1.
//
//				References:
//					http://www.cs.ualberta.ca/~andreas/math/matrfaq_latest.html
//					book: "3D Game Engine Design"
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Quaternion_GetAxisAndAngle(const TQ3Quaternion *q, TQ3Vector3D *outAxis, float *outAngle)
{
	if (q->w > 1.0f - kQ3RealZero || q->w < -1.0f + kQ3RealZero)
		{
		// |w| = 1 means this is a null rotation.
		// Return 0 for the angle, and pick an arbitrary axis.
		if (outAngle) *outAngle = 0.0f;
		if (outAxis)
			{
			outAxis->x = 0.0f;
			outAxis->y = 1.0f;
			outAxis->z = 0.0f;
			}
		}
	else
		{
		// |w| != 1, so we have a non-null rotation.
		// w is the cosine of the half-angle.
		float coshalf = q->w;
		if (outAngle)
			*outAngle = 2.0f * (float)acos(coshalf);
		if (outAxis)
			{
			float sinhalf = sqrt( 1.0f - coshalf * coshalf );
			outAxis->x = q->x / sinhalf;
			outAxis->y = q->y / sinhalf;
			outAxis->z = q->z / sinhalf;
			}
		}

	return (outAxis);
}


//=============================================================================
//      E3Vector3D_TransformQuaternion : Transform 3D vector by quaternion.
//-----------------------------------------------------------------------------
//		Note :	'result' may be the same as 'vector3D'.
//
//				For correct results, |quaternion] == 1.
//
//				http://www.gamasutra.com/features/19980703/quaternions_01.htm
//
//				Here we do the multiplication by hand (in the conventional
//				forward order), gaining efficiency by making use of the fact
//				that the w component of a vector is 0.
//
//				If we were to call E3Quaternion_Multiply, we would have to
//				compensate for the fact that that function mutiplies in the
//				reverse order.
//-----------------------------------------------------------------------------
TQ3Vector3D *
E3Vector3D_TransformQuaternion(const TQ3Vector3D *vector3D, const TQ3Quaternion *quaternion,
	TQ3Vector3D *result)
{
	TQ3Quaternion temp;
	
	// Multiply quaternion by vector3D (noting that vector3D->w is 0)
	temp.w = quaternion->x*vector3D->x + quaternion->y*vector3D->y + quaternion->z*vector3D->z;
	temp.x = quaternion->w*vector3D->x + quaternion->y*vector3D->z - quaternion->z*vector3D->y;
	temp.y = quaternion->w*vector3D->y + quaternion->z*vector3D->x - quaternion->x*vector3D->z;
	temp.z = quaternion->w*vector3D->z + quaternion->x*vector3D->y - quaternion->y*vector3D->x;
	
	// Multiply (quaternion*vector3D) by inverse(quaternion) (noting that result->w is 0)
	result->x = temp.w*quaternion->x + temp.x*quaternion->w - (temp.y*quaternion->z - temp.z*quaternion->y);
	result->y = temp.w*quaternion->y + temp.y*quaternion->w + (temp.x*quaternion->z - temp.z*quaternion->x);
	result->z = temp.w*quaternion->z + temp.z*quaternion->w - (temp.x*quaternion->y - temp.y*quaternion->x);
	
	return(result);
}





//=============================================================================
//      E3Point3D_TransformQuaternion : Transform 3D point by quaternion.
//-----------------------------------------------------------------------------
//		Note : Equivalent to E3Vector3D_TransformQuaternion.
//-----------------------------------------------------------------------------
TQ3Point3D *
E3Point3D_TransformQuaternion(const TQ3Point3D *point3D, const TQ3Quaternion *quaternion,
	TQ3Point3D *result)
{
	return((TQ3Point3D*)
		E3Vector3D_TransformQuaternion((const TQ3Vector3D*) point3D, quaternion,
			(TQ3Vector3D*) result));
}





//=============================================================================
//      E3BoundingBox_Set : Set bounding box.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3BoundingBox *
E3BoundingBox_Set(TQ3BoundingBox *bBox,
	const TQ3Point3D *min, const TQ3Point3D *max, TQ3Boolean isEmpty)
{
	bBox->min = *min;
	bBox->max = *max;
	bBox->isEmpty = isEmpty;

	return(bBox);
}





//=============================================================================
//      E3BoundingBox_SetFromPoints3D :	Set bounding box to just enclose set
//										of 3D points.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_SetFromPoints3D(TQ3BoundingBox *bBox,
	const TQ3Point3D *points3D, TQ3Uns32 numPoints, TQ3Uns32 structSize)
{
	if (numPoints == 0)
	{
		E3Point3D_Set(&bBox->min, 0.0f, 0.0f, 0.0f);
		E3Point3D_Set(&bBox->max, 0.0f, 0.0f, 0.0f);
		bBox->isEmpty = kQ3True;
	}
	else
	{
		const char* in = (const char*) points3D;
		TQ3Uns32 i;
		
		bBox->min = bBox->max = *points3D;
		bBox->isEmpty = kQ3False;
		
		for (i = 1; i < numPoints; ++i)
		{
			float x, y, z;
				
			in += structSize;
			
			x = ((const TQ3Point3D*) in)->x;
			y = ((const TQ3Point3D*) in)->y;
			z = ((const TQ3Point3D*) in)->z;
			
			if (x < bBox->min.x)
				bBox->min.x = x;
			else if (x > bBox->max.x)
				bBox->max.x = x;
			if (y < bBox->min.y)
				bBox->min.y = y;
			else if (y > bBox->max.y)
				bBox->max.y = y;
			if (z < bBox->min.z)
				bBox->min.z = z;
			else if (z > bBox->max.z)
				bBox->max.z = z;
		}
	}

	return(bBox);
}





//=============================================================================
//      E3BoundingBox_SetFromRationalPoints4D :	Set bounding box to just enclose
//												set of 4D rational points.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_SetFromRationalPoints4D(TQ3BoundingBox *bBox,
	const TQ3RationalPoint4D *rationalPoints4D, TQ3Uns32 numPoints, TQ3Uns32 structSize)
{
	if (numPoints == 0)
	{
		E3Point3D_Set(&bBox->min, 0.0f, 0.0f, 0.0f);
		E3Point3D_Set(&bBox->max, 0.0f, 0.0f, 0.0f);
		bBox->isEmpty = kQ3True;
	}
	else
	{
		TQ3Point3D point3D;
		const char* in = (const char*) rationalPoints4D;
		TQ3Uns32 i;
		
		E3RationalPoint4D_To3D(rationalPoints4D, &point3D);
		bBox->min = bBox->max = point3D;
		bBox->isEmpty = kQ3False;
		
		for (i = 1; i < numPoints; ++i)
		{
			in += structSize;
			
			E3RationalPoint4D_To3D((const TQ3RationalPoint4D*) in, &point3D);
			
			if (point3D.x < bBox->min.x)
				bBox->min.x = point3D.x;
			else if (point3D.x > bBox->max.x)
				bBox->max.x = point3D.x;
			if (point3D.y < bBox->min.y)
				bBox->min.y = point3D.y;
			else if (point3D.y > bBox->max.y)
				bBox->max.y = point3D.y;
			if (point3D.z < bBox->min.z)
				bBox->min.z = point3D.z;
			else if (point3D.z > bBox->max.z)
				bBox->max.z = point3D.z;
		}
	}

	return(bBox);
}





//=============================================================================
//      E3BoundingBox_Copy : Copy bounding box.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'bBox'.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_Copy(const TQ3BoundingBox *bBox, TQ3BoundingBox *result)
{
	if (result != bBox)
		*result = *bBox;

	return(result);
}





//=============================================================================
//      E3BoundingBox_Union :	Return minimum bounding box that encloses both
//								'b1' and 'b2'.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'b1' or 'b2'.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_Union(const TQ3BoundingBox *b1, const TQ3BoundingBox *b2,
	TQ3BoundingBox *result)
{
	if (b1->isEmpty)
	{
		if (b2->isEmpty)
		{
			E3Point3D_Set(&result->min, 0.0f, 0.0f, 0.0f);
			E3Point3D_Set(&result->max, 0.0f, 0.0f, 0.0f);
			result->isEmpty = kQ3True;
		}
		else
			E3BoundingBox_Copy(b2, result);
	}
	else
	{
		if (b2->isEmpty)
			E3BoundingBox_Copy(b1, result);
		else
		{
			result->min.x = E3Num_Min(b1->min.x, b2->min.x);
			result->min.y = E3Num_Min(b1->min.y, b2->min.y);
			result->min.z = E3Num_Min(b1->min.z, b2->min.z);
			result->max.x = E3Num_Max(b1->max.x, b2->max.x);
			result->max.y = E3Num_Max(b1->max.y, b2->max.y);
			result->max.z = E3Num_Max(b1->max.z, b2->max.z);
			result->isEmpty = kQ3False;
		}
	}

	return(result);
}





//=============================================================================
//      E3BoundingBox_UnionPoint3D :	Return minimum bounding box that encloses
//										both 'bBox' and 'point3D'.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'bBox'.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_UnionPoint3D(const TQ3BoundingBox *bBox, const TQ3Point3D *point3D,
	TQ3BoundingBox *result)
{
	if (bBox->isEmpty)
	{
		result->min = result->max = *point3D;
	}
	else
	{
		result->min.x = E3Num_Min(point3D->x, bBox->min.x);
		result->min.y = E3Num_Min(point3D->y, bBox->min.y);
		result->min.z = E3Num_Min(point3D->z, bBox->min.z);
		result->max.x = E3Num_Max(point3D->x, bBox->max.x);
		result->max.y = E3Num_Max(point3D->y, bBox->max.y);
		result->max.z = E3Num_Max(point3D->z, bBox->max.z);
	}
	result->isEmpty = kQ3False;

	return(result);
}





//=============================================================================
//      E3BoundingBox_UnionRationalPoint4D :	Return minimum bounding box that
//												encloses both 'bBox' and
//												'rationalPoint4D'.
//-----------------------------------------------------------------------------
TQ3BoundingBox *
E3BoundingBox_UnionRationalPoint4D(const TQ3BoundingBox *bBox,
	const TQ3RationalPoint4D *rationalPoint4D, TQ3BoundingBox *result)
{
	TQ3Point3D point3D;
	
	E3RationalPoint4D_To3D(rationalPoint4D, &point3D);

	return(E3BoundingBox_UnionPoint3D(bBox, &point3D, result));
}





//=============================================================================
//      E3BoundingSphere_Set : Set bounding sphere.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3BoundingSphere *
E3BoundingSphere_Set(TQ3BoundingSphere *bSphere,
	const TQ3Point3D *origin, float radius, TQ3Boolean isEmpty)
{
	bSphere->origin = *origin;
	bSphere->radius = radius;
	bSphere->isEmpty = isEmpty;
	
	return(bSphere);
}





//=============================================================================
//      E3BoundingSphere_SetFromPoints3D :	Set bounding sphere to just enclose
//											set of 3D points.
//-----------------------------------------------------------------------------
//		Note :	This implementation is inefficient, and could be improved.
//
//				Untested.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_SetFromPoints3D(TQ3BoundingSphere *bSphere, const TQ3Point3D *points3D,
									TQ3Uns32 numPoints, TQ3Uns32 structSize)
{
	const char *inp=((const char*)points3D) + structSize;
	TQ3Uns32 i;
	bSphere->origin = *points3D;
	bSphere->radius = 0.0f;
	for (i=1; i<numPoints; ++i) {
		E3BoundingSphere_UnionPoint3D( bSphere, (const TQ3Point3D*)inp, bSphere );
		inp += structSize;
	}
	bSphere->isEmpty = kQ3False;
	return(bSphere);
}





//=============================================================================
//      E3BoundingSphere_SetFromRationalPoints4D :	Set bounding sphere to just
//													enclose set of 4D rational
//													points.
//-----------------------------------------------------------------------------
//		Note :	This implementation is inefficient, and could be improved.
//				Also note: QD3D does not produce the same answer as this
//				function for at least some inputs.
//
//				I think QD3D is wrong, and don't know exactly what it thinks
//				it's doing.
//
//				Untested.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_SetFromRationalPoints4D(TQ3BoundingSphere *bSphere, const TQ3RationalPoint4D *rationalPoints4D, TQ3Uns32 numPoints, TQ3Uns32 structSize)
{
	const char *inp=((const char*)rationalPoints4D) + structSize;
	TQ3Uns32 i;
	bSphere->origin = *((const TQ3Point3D*)rationalPoints4D);
	if (rationalPoints4D->w != 1.0f) {
		float w = 1.0f / rationalPoints4D->w;
		bSphere->origin.x *= w;
		bSphere->origin.y *= w;
		bSphere->origin.z *= w;
	}
	bSphere->radius = 0.0f;
	for (i=1; i<numPoints; ++i) {
		E3BoundingSphere_UnionRationalPoint4D( bSphere, (const TQ3RationalPoint4D*)inp, bSphere );
		inp += structSize;
	}
	bSphere->isEmpty = kQ3False;
	return(bSphere);
}





//=============================================================================
//      E3BoundingSphere_Copy : Copy bounding sphere.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'bSphere'.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_Copy(const TQ3BoundingSphere *bSphere, TQ3BoundingSphere *result)
{
	if (result != bSphere)
		*result = *bSphere;

	return(result);
}





//=============================================================================
//      E3BoundingSphere_Union :	Return minimum bounding sphere that encloses
//									both 's1' and 's2'.
//-----------------------------------------------------------------------------
//		Note :	'result' may be the same as 's1' or 's2'.
//
//				Untested.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_Union(const TQ3BoundingSphere *s1, const TQ3BoundingSphere *s2,
	TQ3BoundingSphere *result)
{

	// Approach: find the points on each sphere farthest from the other sphere
	//	by using similar triangles.  The union sphere has center halfway
	//	between these points, with radius of half their distance.

	if (s1->isEmpty)
	{
		if (s2->isEmpty)
		{
			E3Point3D_Set(&result->origin, 0.0f, 0.0f, 0.0f);
			result->radius = 0.0f;
			result->isEmpty = kQ3True;
		}
		else
			E3BoundingSphere_Copy(s2, result);
	}
	else
	{
		if (s2->isEmpty)
		{
			E3BoundingSphere_Copy(s1, result);
		}
		else
		{
			float x1=s1->origin.x, y1=s1->origin.y, z1=s1->origin.z;
			float x2=s2->origin.x, y2=s2->origin.y, z2=s2->origin.z;
			// find the deltas between their centers, and the distance.
			float dx = x2-x1, dy = y2-y1, dz = z2-z1, dist=(float)sqrt(dx*dx+dy*dy+dz*dz);
			
			if (dist > 0.0f)
			{
				// find the far points.
				float factor = s1->radius / dist;
				float fx1 = x1 - dx*factor;
				float fy1 = y1 - dy*factor;
				float fz1 = z1 - dz*factor;
				
				factor = s2->radius / dist;
				{
					float fx2 = x2 + dx*factor;
					float fy2 = y2 + dy*factor;
					float fz2 = z2 + dz*factor;
					// finish the job.
					result->origin.x = (fx1+fx2)/2;
					result->origin.y = (fy1+fy2)/2;
					result->origin.z = (fz1+fz2)/2;
					dx = fx1-fx2;
					dy = fy1-fy2;
					dz = fz1-fz2;
				}
				result->radius = (float)sqrt(dx*dx+dy*dy+dz*dz) / 2.0f;
			}
			else
			{
				// if the points are the same, then just take whichever radius is greater.
				result->origin = s1->origin;
				result->radius = E3Num_Max(s1->radius, s2->radius);
			}
			result->isEmpty = kQ3False;
		}
	}

	return(result);
}





//=============================================================================
//      E3BoundingSphere_UnionPoint3D :	Return minimum bounding sphere that
//										encloses both 'bSphere' and 'point3D'.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_UnionPoint3D(const TQ3BoundingSphere *bSphere, const TQ3Point3D *point3D,
	TQ3BoundingSphere *result)
{
	if (bSphere->isEmpty)
	{
		result->origin = *point3D;
		result->radius = 0.0f;
	}
	else
	{
		// Approach here is similar to that for Union,
		// if we imagine the given point as a radius=0 sphere.
		float x1=bSphere->origin.x, y1=bSphere->origin.y, z1=bSphere->origin.z;
		float x2=point3D->x, y2=point3D->y, z2=point3D->z;
		// find the deltas between their centers, and the distance.
		float dx = x2-x1, dy = y2-y1, dz = z2-z1, dist=(float)sqrt(dx*dx+dy*dy+dz*dz);
		if (dist > bSphere->radius)
		{
			// find the far points.
			float factor = bSphere->radius / dist;
			float fx1 = x1 - dx*factor;
			float fy1 = y1 - dy*factor;
			float fz1 = z1 - dz*factor;
			// finish the job.
			result->origin.x = (fx1+x2)/2;
			result->origin.y = (fy1+y2)/2;
			result->origin.z = (fz1+z2)/2;
			dx = fx1-x2;
			dy = fy1-y2;
			dz = fz1-z2;
			result->radius = (float)sqrt(dx*dx+dy*dy+dz*dz) / 2.0f;
		}
		else
		{
			// if the point is within the sphere, then no change is necessary.
			E3BoundingSphere_Copy(bSphere, result);
			return(result);
		}
	}

	result->isEmpty = kQ3False;
	
	return(result);
}





//=============================================================================
//      E3BoundingSphere_UnionRationalPoint4D :	Return minimum bounding sphere
//												that encloses both 'bSphere'
//												and 'rationalPoint4D'.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3BoundingSphere *
E3BoundingSphere_UnionRationalPoint4D(const TQ3BoundingSphere *bSphere,
	const TQ3RationalPoint4D *rationalPoint4D, TQ3BoundingSphere *result)
{
	TQ3Point3D point3D;
	
	E3RationalPoint4D_To3D(rationalPoint4D, &point3D);

	return(E3BoundingSphere_UnionPoint3D(bSphere, &point3D, result));
}





//=============================================================================
//      E3Ray3D_IntersectSphere : Perform a ray/sphere intersection test.
//-----------------------------------------------------------------------------
//		Note :	We assume that the ray direction vector has been normalised.
//				The algorithm is from 'Real Time Rendering', section 10.3.2. 
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Boolean
E3Ray3D_IntersectSphere(const TQ3Ray3D *theRay, const TQ3Sphere *theSphere, TQ3Point3D *hitPoint)
{	TQ3Vector3D			sphereToRay, intersectVector;
	float				d, q, t, l2, r2, m2;



	// Prepare to intersect
	//
	// First calculate the vector from the sphere to the ray origin, its
	// length squared, the projection of this vector onto the ray direction,
	// and the squared radius of the sphere.
	E3Point3D_Subtract(&theSphere->origin, &theRay->origin, &sphereToRay);
	l2 = E3Vector3D_LengthSquared(&sphereToRay);
	d  = E3Vector3D_Dot(&sphereToRay, &theRay->direction);
	r2 = theSphere->radius * theSphere->radius;



	// If the sphere is behind the ray origin, they don't intersect
	if (d < 0.0f && l2 > r2)
		return(kQ3False);



	// Calculate the squared distance from the sphere center to the projection.
	// If it's greater than the radius then they don't intersect.
	m2 = (l2 - (d * d));
	if (m2 > r2)
		return(kQ3False);



	// Calculate the distance along the ray to the intersection point
	q = (float) sqrt(r2 - m2);
	if (l2 > r2)
		t = d - q;
	else
		t = d + q;



	// Calculate the intersection point
	E3Vector3D_Scale(&theRay->direction, t, &intersectVector);
	E3Point3D_Vector3D_Add(&theRay->origin, &intersectVector, hitPoint);

	return(kQ3True);
}





//=============================================================================
//      E3Ray3D_IntersectBoundingBox : Perform a ray/box intersection test.
//-----------------------------------------------------------------------------
//		Note :	We assume that the ray direction vector has been normalised.
//				The algorithm is Andrew Woo's "Fast Ray-Box Intersection"
//				from "Graphics Gems".
//-----------------------------------------------------------------------------
TQ3Boolean
E3Ray3D_IntersectBoundingBox(const TQ3Ray3D *theRay, const TQ3BoundingBox *theBounds, TQ3Point3D *hitPoint)
{	float			candidatePlane[3], maxT[3], coord[3];
	float			minB[3], maxB[3], origin[3], dir[3];
	TQ3Uns32		i, whichPlane, right, left, middle;
	TQ3Int8			quadrant[3];
	TQ3Boolean		isInside;



	// Initialise ourselves
	minB[0]   = theBounds->min.x;
	minB[1]   = theBounds->min.y;
	minB[2]   = theBounds->min.z;

	maxB[0]   = theBounds->max.x;
	maxB[1]   = theBounds->max.y;
	maxB[2]   = theBounds->max.z;

	origin[0] = theRay->origin.x;
	origin[1] = theRay->origin.y;
	origin[2] = theRay->origin.z;

	dir[0]    = theRay->direction.x;
	dir[1]    = theRay->direction.y;
	dir[2]    = theRay->direction.z;

	isInside  = kQ3True;
	right     = 0;
	left      = 1;
	middle    = 2;



	// Find candidate planes
	for (i = 0; i < 3; i++)
		{
		if (origin[i] < minB[i])
			{
			quadrant[i]       = (TQ3Int8) left;
			candidatePlane[i] = minB[i];
			isInside          = kQ3False;
			}
		else if (origin[i] > maxB[i])
			{
			quadrant[i]       = (TQ3Int8) right;
			candidatePlane[i] = maxB[i];
			isInside          = kQ3False;
			}
		else
			quadrant[i] = (TQ3Int8) middle;
		}



	// Check for the ray origin being inside the bounding box
	if (isInside)
		{
		*hitPoint = theRay->origin;
		return(kQ3True);
		}



	// Calculate T distances to candidate planes
	for (i = 0; i < 3; i++)
		{
		if (quadrant[i] != (TQ3Int8) middle && dir[i] != 0.0f)
			maxT[i] = (candidatePlane[i] - origin[i]) / dir[i];
		else
			maxT[i] = -1.0f;
		}



	// Get largest of the maxT's for final choice of intersection
	whichPlane = 0;
	for (i = 1; i < 3; i++)
		{
		if (maxT[whichPlane] < maxT[i])
			whichPlane = i;
		}



	// Check final candidate actually inside box
	if (maxT[whichPlane] < 0.0f)
		return(kQ3False);
	
	for (i = 0; i < 3; i++)
		{
		if (whichPlane != i)
			{
			coord[i] = origin[i] + maxT[whichPlane] * dir[i];
			if (coord[i] < minB[i] || coord[i] > maxB[i])
				return(kQ3False);
			}
		else
			coord[i] = candidatePlane[i];
		}
	
	hitPoint->x = coord[0];
	hitPoint->y = coord[1];
	hitPoint->z = coord[2];
	
	return(kQ3True);
}





//=============================================================================
//      E3Ray3D_IntersectTriangle : Perform a ray/triangle intersection test.
//-----------------------------------------------------------------------------
//		Note :	Uses the Moller-Trumbore algorithm to test for intersection,
//				and if found returns the barycentric coordinates of the
//				intersection point. The third component of hitPoint, w, is
//				used to store the distance along the ray to the plane in
//				which the triangle lies.
//
//				Details at:
//				<http://www.acm.org/jgt/papers/MollerTrumbore97/>.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Ray3D_IntersectTriangle(const TQ3Ray3D		*theRay,
							const TQ3Point3D	*point1,
							const TQ3Point3D	*point2,
							const TQ3Point3D	*point3,
							TQ3Boolean			cullBackfacing,
							TQ3Param3D			*hitPoint)
{	TQ3Vector3D		edge1, edge2, tvec, pvec, qvec;
	float			det, invDet;



	// Calculate the two edges which share vertex 1
	E3Point3D_Subtract(point2, point1, &edge1);
	E3Point3D_Subtract(point3, point1, &edge2);



	// Begin calculating the determinant - also used to calculate u. If the
	// determinant is near zero, the ray lies in the plane of the triangle.
	E3Vector3D_Cross(&theRay->direction, &edge2, &pvec);
	det = E3Vector3D_Dot(&edge1, &pvec);



	// Handle triangles with back-face culling
	if (cullBackfacing)
		{
		// Test for ray coinciding with triangle plane
		if (det < kQ3RealZero)
			return(kQ3False);


		// Calculate the distance between vertex 1 and the ray origin
		E3Point3D_Subtract(&theRay->origin, point1, &tvec);


		// Calculate u, and test for a miss
		hitPoint->u = E3Vector3D_Dot(&tvec, &pvec);
		if (hitPoint->u < 0.0f || hitPoint->u > det)
			return(kQ3False);


		// Calculate v, and test for a miss		
		E3Vector3D_Cross(&tvec, &edge1, &qvec);
		hitPoint->v = E3Vector3D_Dot(&theRay->direction, &qvec);
		if (hitPoint->v < 0.0f || (hitPoint->u + hitPoint->v) > det)
			return(kQ3False);


		// Calculate w, and scale the parameters
		hitPoint->w  = E3Vector3D_Dot(&edge2, &qvec);
		invDet = 1.0f / det;

		hitPoint->w *= invDet;
		hitPoint->u *= invDet;
		hitPoint->v *= invDet;
		}


	// Handle triangles with no culling
	else
		{
		// Test for ray coinciding with triangle plane
		if (det > -kQ3RealZero && det < kQ3RealZero)
			return(kQ3False);
		
		invDet = 1.0f / det;


		// Calculate the distance between vertex 1 and the ray origin
		E3Point3D_Subtract(&theRay->origin, point1, &tvec);


		// Calculate u, and test for a miss
		hitPoint->u = E3Vector3D_Dot(&tvec, &pvec) * invDet;
		if (hitPoint->u < 0.0f || hitPoint->u > 1.0f)
			return(kQ3False);


		// Calculate v, and test for a miss		
		E3Vector3D_Cross(&tvec, &edge1, &qvec);
		hitPoint->v = E3Vector3D_Dot(&theRay->direction, &qvec) * invDet;
		if (hitPoint->v < 0.0f || (hitPoint->u + hitPoint->v) > 1.0f)
			return(kQ3False);
		
		
		// Calculate w
		hitPoint->w = E3Vector3D_Dot(&edge2, &qvec) * invDet;
		}



	// The ray intersects the triangle	
	return(kQ3True);
}



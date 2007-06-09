/*  NAME:
        E3Math_Intersect.cpp

    DESCRIPTION:
        Math routines for Quesa, involving geometric intersections.
        
        Note that these routines are allowed to call other E3foo routines for
        speed, to avoid the trip back out through the Q3foo interface.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Math.h"
#include "E3Math_Intersect.h"
#include <limits>





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------

static bool IsPointInCone( const TQ3Point3D& inPt,
							const TQ3Ray3D& inConeAxis,
							float inConeCosine )
{
	TQ3Vector3D	apexToPt;
	Q3FastPoint3D_Subtract( &inPt, &inConeAxis.origin, &apexToPt );
	
	return Q3FastVector3D_Dot( &apexToPt, & inConeAxis.direction ) >=
		inConeCosine * Q3FastVector3D_Length( &apexToPt );
}


static void e3BoundingBox_GetEdgeRays(
								const TQ3BoundingBox& inBounds,
								TQ3Ray3D* out12Edges,
								float* out12MaxParam )
{
	int		rayIndex = 0;
	TQ3Ray3D	theRay;
	float		maxParam;
	
	// edges parallel to x axis
	maxParam = inBounds.max.x - inBounds.min.x;
	theRay.direction.x = 1.0f;
	theRay.direction.y = theRay.direction.z = 0.0f;
	theRay.origin = inBounds.min;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
	theRay.origin.y = inBounds.max.y;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
	theRay.origin.z = inBounds.max.z;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
	theRay.origin.y = inBounds.min.y;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
	
	// Edges parallel to y axis
	maxParam = inBounds.max.y - inBounds.min.y;
	theRay.direction.y = 1.0f;
	theRay.direction.x = theRay.direction.z = 0.0f;
	theRay.origin = inBounds.min;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
	theRay.origin.x = inBounds.max.x;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
	theRay.origin.z = inBounds.max.z;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
	theRay.origin.x = inBounds.min.x;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
	
	// Edges parallel to z axis
	maxParam = inBounds.max.z - inBounds.min.z;
	theRay.direction.z = 1.0f;
	theRay.direction.x = theRay.direction.y = 0.0f;
	theRay.origin = inBounds.min;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
	theRay.origin.x = inBounds.max.x;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
	theRay.origin.y = inBounds.max.y;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
	theRay.origin.x = inBounds.min.x;
	out12MaxParam[ rayIndex ] = maxParam;
	out12Edges[ rayIndex++ ] = theRay;
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
#pragma mark -

//=============================================================================
//      E3Ray3D_IntersectSphere : Perform a ray/sphere intersection test.
//-----------------------------------------------------------------------------
//		Note :	We assume that the ray direction vector has been normalised.
//				The algorithm is from 'Real Time Rendering', section 10.3.2. 
//-----------------------------------------------------------------------------
TQ3Boolean
E3Ray3D_IntersectSphere(const TQ3Ray3D *theRay, const TQ3Sphere *theSphere, TQ3Point3D *hitPoint)
{	TQ3Vector3D			sphereToRay, intersectVector;
	float				d, q, t, l2, r2, m2;



	// Prepare to intersect
	//
	// First calculate the vector from the sphere to the ray origin, its
	// length squared, the projection of this vector onto the ray direction,
	// and the squared radius of the sphere.
	Q3FastPoint3D_Subtract(&theSphere->origin, &theRay->origin, &sphereToRay);
	l2 = Q3FastVector3D_LengthSquared(&sphereToRay);
	d  = Q3FastVector3D_Dot(&sphereToRay, &theRay->direction);
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
	q = E3Math_SquareRoot(r2 - m2);
	if (l2 > r2)
		t = d - q;
	else
		t = d + q;



	// Calculate the intersection point
	Q3FastVector3D_Scale(&theRay->direction, t, &intersectVector);
	Q3FastPoint3D_Vector3D_Add(&theRay->origin, &intersectVector, hitPoint);

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
		if (hitPoint) *hitPoint = theRay->origin;
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
	
	if (hitPoint)
		{
		hitPoint->x = coord[0];
		hitPoint->y = coord[1];
		hitPoint->z = coord[2];
		}
	
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
	Q3FastPoint3D_Subtract(point2, point1, &edge1);
	Q3FastPoint3D_Subtract(point3, point1, &edge2);



	// Begin calculating the determinant - also used to calculate u. If the
	// determinant is near zero, the ray lies in the plane of the triangle.
	// However, some care is needed; we can get a false positive on "near zero"
	// if the triangle is small.
	Q3FastVector3D_Cross(&theRay->direction, &edge2, &pvec);
	det = Q3FastVector3D_Dot(&edge1, &pvec);
	float	testDet = det;
	if (fabsf( det ) < kQ3RealZero)
	{
		TQ3Vector3D	faceNorm;
		Q3FastVector3D_Cross( &edge2, &edge1, &faceNorm );
		Q3FastVector3D_Normalize( &faceNorm, &faceNorm );
		testDet = Q3FastVector3D_Dot( &faceNorm, &theRay->direction );
	}


	// Handle triangles with back-face culling
	if (cullBackfacing)
		{
		// Test for ray coinciding with triangle plane, or backface hit
		if (testDet < kQ3RealZero)
			return(kQ3False);


		// Calculate the distance between vertex 1 and the ray origin
		Q3FastPoint3D_Subtract(&theRay->origin, point1, &tvec);


		// Calculate u, and test for a miss
		hitPoint->u = Q3FastVector3D_Dot(&tvec, &pvec);
		if (hitPoint->u < 0.0f || hitPoint->u > det)
			return(kQ3False);


		// Calculate v, and test for a miss		
		Q3FastVector3D_Cross(&tvec, &edge1, &qvec);
		hitPoint->v = Q3FastVector3D_Dot(&theRay->direction, &qvec);
		if (hitPoint->v < 0.0f || (hitPoint->u + hitPoint->v) > det)
			return(kQ3False);


		// Calculate w, and scale the parameters
		hitPoint->w  = Q3FastVector3D_Dot(&edge2, &qvec);
		invDet = 1.0f / det;

		hitPoint->w *= invDet;
		hitPoint->u *= invDet;
		hitPoint->v *= invDet;
		}


	// Handle triangles with no culling
	else
		{
		// Test for ray coinciding with triangle plane
		if (testDet > -kQ3RealZero && testDet < kQ3RealZero)
			return(kQ3False);
		
		invDet = 1.0f / det;


		// Calculate the distance between vertex 1 and the ray origin
		Q3FastPoint3D_Subtract(&theRay->origin, point1, &tvec);


		// Calculate u, and test for a miss
		hitPoint->u = Q3FastVector3D_Dot(&tvec, &pvec) * invDet;
		if (hitPoint->u < 0.0f || hitPoint->u > 1.0f)
			return(kQ3False);


		// Calculate v, and test for a miss		
		Q3FastVector3D_Cross(&tvec, &edge1, &qvec);
		hitPoint->v = Q3FastVector3D_Dot(&theRay->direction, &qvec) * invDet;
		if (hitPoint->v < 0.0f || (hitPoint->u + hitPoint->v) > 1.0f)
			return(kQ3False);
		
		
		// Calculate w
		hitPoint->w = Q3FastVector3D_Dot(&edge2, &qvec) * invDet;
		}



	// The ray intersects the triangle	
	return (hitPoint->w >= 0.0f ? kQ3True : kQ3False);
}





/*!
	@function	E3Ray3D_IntersectCone
	@abstract	Compute the intersections between a ray and a cone surface.
	@discussion	This function computes the minimum and maximum values of t
				such that inRay.origin + t * inRay.direction is in the cone.
	
				If the ray misses the cone, the function result will be false
				and the output parameters should be ignored.  if the ray origin
				is in the interior of the cone, outMinParam will be set to 0.
				It is possible that there is no largest value of t, in which
				case outMaxParam will be set to INFINITY.

	@param		inRay			A ray with a normalized direction.
	@param		inConeAxis		Axis ray of cone with a normalized direction.
	@param		inConeCosine	Cosine of angle from axis ray to surface of
								cone, in interval (0, 1).
	@param		outMinParam		Receives least parameter for which the ray is
								within the cone, not less than 0.0.
	@param		outMaxParam		Receives greatest parameter for which the ray is
								within the cone, or INFINITY.
	@result		True if the ray hits the cone.
*/
bool	E3Ray3D_IntersectCone( const TQ3Ray3D& inRay,
							const TQ3Ray3D& inConeAxis,
							float inConeCosine,
							float& outMinParam,
							float& outMaxParam )
{
	bool	didIntersect = false;
	
	// The cone can be described as the points x such that
	// 		dot( x - inConeAxis.origin, inConeAxis.direction ) >=
	//			inConeCosine * length( x - inConeAxis.origin ).
	// Let's check whether the start of the ray is inside the cone.
	
	TQ3Vector3D	diff;
	Q3FastPoint3D_Subtract( &inRay.origin, &inConeAxis.origin, &diff );
	float	diffDotAxis = Q3FastVector3D_Dot( &diff, &inConeAxis.direction );
	float	diffLen = Q3FastVector3D_Length( &diff );
	bool	isRayOriginInCone = (diffDotAxis >= inConeCosine * diffLen);
	
	// Replacing x by inRay.origin + t * inRay.direction, we are seeking t >= 0
	// such that
	//		dot( inRay.origin + t * inRay.direction - inConeAxis.origin,
	//			inConeAxis.direction ) >=
	//		inConeCosine * length( inRay.origin + t * inRay.direction -
	//			inConeAxis.origin ) .
	// If we let diff = inRay.origin - inConeAxis.origin, this becomes
	//		dot( diff + t * inRay.direction, inConeAxis.direction ) >=
	//		inConeCosine * length( diff + t * inRay.direction ) .
	//	Letting t increase toward infinity, we see that the ray is eventually
	// inside the cone just in case
	// 		dot( inRay.direction, inConeAxis.direction ) > inConeCosine .
	float	rayDotAxis = Q3FastVector3D_Dot( &inRay.direction,
		&inConeAxis.direction );
	bool	isRayEndInCone = (rayDotAxis > inConeCosine);
	
	// Replacing length( diff + t * inRay.direction ) by
	// sqrt( dot( diff + t * inRay.direction, diff + t * inRay.direction ) )
	// and expanding out the dot products, this becomes
	//		diffDotAxis + t * rayDotAxis >=
	//			inConeCosine * sqrt( diffDotDiff + 2 * t * diffDotRay + t * t ) .
	float	diffDotDiff = diffLen * diffLen;
	float	diffDotRay = Q3FastVector3D_Dot( &diff, &inRay.direction );
	
	// Let us square both sides to get rid of the square root.  (This may
	// produce extraneous solutions such that diffDotAxis + t * rayDotAxis is
	// negative.)
	//		square(diffDotAxis + t * rayDotAxis) >=
	//			square(inConeCosine) * ( diffDotDiff + 2 * t * diffDotRay + t * t ) .
	// This is a quadratic equation, which can be solved by the familiar
	// quadratic formula.
	float	cosSq = inConeCosine * inConeCosine;
	float	a = cosSq - rayDotAxis * rayDotAxis;
	float	b = 2.0f * (cosSq * diffDotRay - diffDotAxis * rayDotAxis);
	float	c = cosSq * diffDotDiff - diffDotAxis * diffDotAxis;
	float	t;
	
	if (a == 0.0f)
	{
		if (b == 0.0f)
		{
			if (c == 0.0f)
			{
				// The ray lies within the doubled cone
				if (diffDotRay > 0.0f)
				{
					if (isRayOriginInCone)
					{
						outMinParam = 0.0f;
						outMaxParam = std::numeric_limits<float>::infinity();
						didIntersect = true;
					}
					else
					{
						outMinParam = diffLen;
						outMaxParam = std::numeric_limits<float>::infinity();
						didIntersect = true;
					}
				}
				else
				{
					if (isRayOriginInCone)
					{
						outMinParam = 0.0f;
						outMaxParam = diffLen;
						didIntersect = true;
					}
					else
					{
						// no intersection.
					}
				}
			}
			else
			{
				// no solution.
			}
		}
		else
		{
			t = -c / b;
			if (t >= 0.0f)
			{
				if (isRayEndInCone)
				{
					outMinParam = t;
					outMaxParam = std::numeric_limits<float>::infinity();
					didIntersect = true;
				}
			}
			else
			{
				if (isRayEndInCone)
				{
					outMinParam = 0.0f;
					outMaxParam = std::numeric_limits<float>::infinity();
					didIntersect = true;
				}
			}
		}
	}
	else	// a != 0.0
	{
		float	discriminant = b * b - 4.0f * a * c;
		if (discriminant >= 0.0f)
		{
			float	rootDisc = sqrt( discriminant );
			float	oneOver2a = 0.5f / a;
			float	root1, root2;	// we will have root1 <= root2
			if (a > 0.0f)
			{
				root1 = oneOver2a * (- b - rootDisc);
				root2 = oneOver2a * (- b + rootDisc);
			}
			else
			{
				root1 = oneOver2a * (- b + rootDisc);
				root2 = oneOver2a * (- b - rootDisc);
			}
			
			// Test for extraneous roots.
			bool	isRoot1Actual = (diffDotAxis + root1 * rayDotAxis >= 0.0f);
			bool	isRoot2Actual = (diffDotAxis + root2 * rayDotAxis >= 0.0f);
			if ( isRoot1Actual && isRoot2Actual )
			{
				if (root1 >= 0.0f)	// both solutions nonnegative
				{
					outMinParam = root1;
					outMaxParam = root2;
					didIntersect = true;
				}
				else if (root2 >= 0.0f)	// one solution negative
				{
					outMinParam = 0.0f;
					outMaxParam = root2;
					didIntersect = true;
				}
			}
			else if ( isRoot1Actual && ! isRoot2Actual )
			{
				if (root1 >= 0.0f)
				{
					outMinParam = 0.0f;
					outMaxParam = root1;
					didIntersect = true;
				}
			}
			else if ( ! isRoot1Actual && isRoot2Actual )
			{
				if (root2 >= 0.0f)
				{
					outMinParam = root2;
					outMaxParam = std::numeric_limits<float>::infinity();
					didIntersect = true;
				}
				else
				{
					outMinParam = 0.0f;
					outMaxParam = std::numeric_limits<float>::infinity();
					didIntersect = true;
				}
				Q3_ASSERT( isRayEndInCone );
			}
		}
	}
	
	return didIntersect;
}





/*!
	@function	E3Cone_IntersectBoundingBox
	@abstract	Determine whether a cone intersects a bounding box.
	@param		inConeAxis		Axis ray of cone with a normalized direction.
	@param		inConeCosine	Cosine of angle from axis ray to surface of
								cone, in interval (0, 1).
	@param		inBox			A bounding box.
	@result		True if there is an intersection.
*/
bool	E3Cone_IntersectBoundingBox(
									const TQ3Ray3D& inConeAxis,
									float inConeCosine,
									const TQ3BoundingBox& inBox )
{
	bool	didIntersect = false;
	
	if (inBox.isEmpty)
	{
		return false;
	}
	
	// If any of the corners of the box are in the cone, they intersect.
	TQ3Point3D	theCorners[8];
	E3BoundingBox_GetCorners( &inBox, theCorners );
	int	i;
	for (i = 0; i < 8; ++i)
	{
		if (IsPointInCone( theCorners[i], inConeAxis, inConeCosine ))
		{
			didIntersect = true;
			break;
		}
	}
	
	// If the cone axis intersects the box, they intersect.
	TQ3Point3D	hitPt;
	if ( ! didIntersect &&
		(kQ3True == E3Ray3D_IntersectBoundingBox( &inConeAxis, &inBox, &hitPt )) )
	{
		didIntersect = true;
	}
	
	// In the remaining hard case, the surface of the cone must intersect an edge.
	if ( ! didIntersect )
	{
		TQ3Ray3D	edgeRays[12];
		float		maxParam[12];
		e3BoundingBox_GetEdgeRays( inBox, edgeRays, maxParam );
		for (i = 0; i < 12; ++i)
		{
			float	highParam, lowParam;
			
			if ( E3Ray3D_IntersectCone( edgeRays[i], inConeAxis, inConeCosine,
				lowParam, highParam ) &&
				(lowParam < maxParam[i]) )
			{
				didIntersect = true;
				break;
			}
		}
	}
	
	return didIntersect;
}

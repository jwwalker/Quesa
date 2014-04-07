/*  NAME:
        E3Math_Intersect.cpp

    DESCRIPTION:
        Math routines for Quesa, involving geometric intersections.
        
        Note that these routines are allowed to call other E3foo routines for
        speed, to avoid the trip back out through the Q3foo interface.

    COPYRIGHT:
        Copyright (c) 1999-2014, Quesa Developers. All rights reserved.

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
#include "E3View.h"
#include "QuesaMathOperators.hpp"

// Remove any macro definitions of min, max, so that the C++ library versions will work.
#undef min
#undef max

#include <limits>
#include <algorithm>
#include <cmath>




//=============================================================================
//      Local Constants
//-----------------------------------------------------------------------------
enum HalfPlaneResult // result of testing whether something is inside a half-plane
{
	kHalfPlaneResult_AllInside = 0,
	kHalfPlaneResult_PartInside,
	kHalfPlaneResult_AllOutside
};

const float kQ3Infinity = std::numeric_limits<float>::infinity();


//=============================================================================
//      Local Types
//-----------------------------------------------------------------------------
namespace
{
	struct Interval
	{
		float	minimum;	// may be - kQ3Infinity
		float	maximum;	// may be kQ3Infinity
	
		bool	IsEmpty() const { return minimum > maximum; }
	};
	
	/*!
		@struct		Frustum
		@abstract	A view frustum represented as the intersection of 6 half-planes.
		@discussion	A plane is represented as a rational point, where the "inside"
					of the half-plane is the points [x, y, z, 1] such that
					P dot [x, y, z, 1] <= 0.
	*/
	struct Frustum
	{
		TQ3RationalPoint4D planes[6];
	};
	
	struct TQ3Ray4D	// like TQ3Ray3D, but origin may be an infinite point
	{
		TQ3RationalPoint4D	origin;
		TQ3Vector3D			direction;
	};
	
	struct FrustumEdge
	{
		TQ3RationalPoint4D	start;
		TQ3RationalPoint4D	end;
	};
	
	struct FrustumEdges
	{
		FrustumEdge			edge[12];
	};
	
	
	/*!
		@struct		ShadowCylinder
		@abstract	The shadow of a bounding sphere, cast by a directional
					light, is contained in a shadow cylinder.  This is a
					half-infinite cylinder.
	*/
	struct ShadowCylinder
	{
		TQ3Ray3D	axis;	// axis.direction is assumed normalized
		float		radius;
	};
	
	/*!
		@struct		ShadowCone
		@abstract	The shadow of a bounding sphere, cast by a positional
					light, is contained in a frustum of a cone.
		@discussion	We describe the slope of the cone by the sine of the
					angle between the axis of symmetry and any line
					from the apex along the surface.
	*/
	struct ShadowCone
	{
		TQ3Ray3D	axis;				// axis.direction is assumed normalized
		float		apexToBasePlane;	// always positive
		float		angleSine;			// in interval (0, 1)
	};
	
	
	/*!
		@struct		EdgeRay
		@abstract	A ray representing an edge of the view frustum.
		@discussion	The edge may be a finite segment with a parameter
					range [0, 1], or may be a ray of infinite points
					with parameter range [0, 1], or may be an infinitely
					long ray with parameter range [0, infinity].
	*/
	struct EdgeRay
	{
		TQ3Ray3D	ray;
		Interval	paramRange;
		bool		isRayAtInfinity;	// Interpret ray.origin as an infinite point?
	};
}



//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------

static TQ3Vector3D InfinitePointToVector( const TQ3RationalPoint4D& inInfinitePt )
{
	TQ3Vector3D vec = {
		inInfinitePt.x, inInfinitePt.y, inInfinitePt.z
	};
	return vec;
}

static TQ3Point3D FinitePointToPoint( const TQ3RationalPoint4D& inInfinitePt )
{
	TQ3Point3D pt;
	Q3FastRationalPoint4D_To3D( &inInfinitePt, &pt );
	return pt;
}

static TQ3Point3D VectorToPoint( const TQ3Vector3D& inVec )
{
	TQ3Point3D asPt = { inVec.x, inVec.y, inVec.z };
	return asPt;
}

static void MakeEdgeRayFromEdge( const FrustumEdge& inEdge, EdgeRay& outEdgeRay )
{
	outEdgeRay.paramRange.minimum = 0.0f;
	if (inEdge.start.w == 0.0f) // hence both are infinite points
	{
		outEdgeRay.ray.origin = VectorToPoint( InfinitePointToVector( inEdge.start ) );
		outEdgeRay.ray.direction = InfinitePointToVector( inEdge.end ) - InfinitePointToVector( inEdge.start );
		outEdgeRay.paramRange.maximum = 1.0f;
		outEdgeRay.isRayAtInfinity = true;
	}
	else if (inEdge.end.w == 0.0f) // only end point is infinite
	{
		outEdgeRay.ray.origin = FinitePointToPoint( inEdge.start );
		outEdgeRay.ray.direction = InfinitePointToVector( inEdge.end );
		outEdgeRay.paramRange.maximum = kQ3Infinity;
		outEdgeRay.isRayAtInfinity = false;
	}
	else	// both are finite points
	{
		outEdgeRay.ray.origin = FinitePointToPoint( inEdge.start );
		outEdgeRay.ray.direction = FinitePointToPoint( inEdge.end ) - FinitePointToPoint( inEdge.start );
		outEdgeRay.paramRange.maximum = 1.0f;
		outEdgeRay.isRayAtInfinity = false;
	}
}


static bool IsPointInSphere( const TQ3Point3D& inPt, const TQ3BoundingSphere& inSphere )
{
	float dist = Q3FastPoint3D_DistanceSquared( &inPt, &inSphere.origin );
	return dist < (inSphere.radius * inSphere.radius + kQ3RealZero);
}



/*!
	@function	Intersect
	@abstract	Compute the intersection of two intervals.
*/
static Interval Intersect( const Interval& inOne, const Interval& inTwo )
{
	Interval result;
	result.minimum = std::max( inOne.minimum, inTwo.minimum );
	result.maximum = std::min( inOne.maximum, inTwo.maximum );
	return result;
}


/*!
	@function	Ray3D_IntersectHalfPlane
	@abstract	Find the interval of nonnegative ray parameters that produce a point
				on the "inside" of a plane.
	@discussion	The "inside" is the points such that [x,y,z,1] dot inPlane <= 0.
	
				Be aware that the resulting interval could be empty.
*/
static Interval Ray3D_IntersectHalfPlane( const TQ3Ray3D& inRay, const TQ3RationalPoint4D& inPlane )
{
	float originDotP = inRay.origin.x * inPlane.x + inRay.origin.y * inPlane.y +
		inRay.origin.z * inPlane.z + inPlane.w;
	float dirDotP = inRay.direction.x * inPlane.x + inRay.direction.y * inPlane.y +
		inRay.direction.z * inPlane.z;
	// We now have to solve originDotP + t * dirDotP <= 0.
	Interval solutions;
	if (fabsf( dirDotP ) < kQ3RealZero)
	{
		if (originDotP <= 0.0f)	// whole ray
		{
			solutions.minimum = 0.0f;
			solutions.maximum = kQ3Infinity;
		}
		else // no solution
		{
			solutions.minimum = 0.0f;
			solutions.maximum = -1.0f;
		}
	}
	else if (dirDotP > 0.0f)
	{
		solutions.minimum = 0.0f;
		solutions.maximum = - originDotP / dirDotP;
	}
	else // dirDotP < 0.0f
	{
		solutions.minimum = std::max( 0.0f, - originDotP / dirDotP );
		solutions.maximum = kQ3Infinity;
	}
	
	return solutions;
}



/*!
	@function	Ray3D_IntersectFrustum
	@abstract	Find the interval of nonnegative ray parameters that produce a point
				inside a given frustum.
*/
static Interval Ray3D_IntersectFrustum( const TQ3Ray3D& inRay, const Frustum& inFrustum )
{
	Interval solution;
	solution.minimum = 0.0f;
	solution.maximum = kQ3Infinity;
	
	for (int i = 0; i < 6; ++i)
	{
		Interval oneSol = Ray3D_IntersectHalfPlane( inRay, inFrustum.planes[i] );
		solution = Intersect( solution, oneSol );
		if (solution.IsEmpty())
		{
			break;
		}
	}
	
	return solution;
}



/*!
	@function	Ray3D_IntersectShadowCylinder
	@abstract	Find nonnegative parameter values of a ray that meet a half-infinite cylinder.
	@discussion	We assume that the axis direction of the cylinder is a unit vector, but we only
				assume that the direction of the ray is nonzero. 
*/
static Interval Ray3D_IntersectShadowCylinder( const TQ3Ray3D& inRay, const ShadowCylinder& inCyl )
{
	Interval solution;
	solution.minimum = 0.0f;
	solution.maximum = kQ3Infinity;
	
	// In order for a ray point, rayOrigin + t * rayDir, to be on the right side of the
	// cylinder's base plane, we need
	// (rayOrigin + t * rayDir - cylOrigin) dot cylDir >= 0, or
	// (rayOrigin - cylOrigin) dot cylDir + t * rayDir dot cylDir >= 0 .
	float constantTerm = Q3Dot3D( inRay.origin - inCyl.axis.origin, inCyl.axis.direction );
	float linearCoeff = Q3Dot3D( inRay.direction, inCyl.axis.direction );
	if (fabsf( linearCoeff ) < kQ3RealZero)
	{
		if (constantTerm >= 0.0f)
		{
			// no constraint on solution
		}
		else
		{
			// no solution!
			solution.maximum = -1.0f;
		}
	}
	else
	{
		float tBoundary = - constantTerm / linearCoeff;
		if (linearCoeff > 0.0f)
		{
			solution.minimum = std::max( tBoundary, 0.0f );
		}
		else // linearCoeff < 0.0f, since we know it's not zero
		{
			solution.maximum = tBoundary;
		}
	}
	
	if (! solution.IsEmpty())
	{
		// For convenience of discussion, let Q = inCyl.axis.origin, w = inCyl.axis.direction,
		// and r = inCyl.radius.
		// For any point X, the point of the cylinder axis nearest to X is
		// Q + ((X - Q) dot w) * w.
		// The point X belongs to the cylinder (no longer worrying about the base plane) iff
		// dist( X, Q + ((X - Q) dot w) * w ) <= r.
		// Square both sides, and express the left hand side as a dot product.
		// (X - Q - ((X - Q) dot w) * w) dot (X - Q - ((X - Q) dot w) * w) <= r*r.
		// For convenience let y = X - Q (a vector), and then we have
		// (y - (y dot w) w) dot (y - (y dot w) w) <= r^2 .
		// If we expand the dot product and simplify, we are left with
		// y dot y  -  (y dot w)^2 <= r^2.
		
		// Now let's bring the ray into the discussion.  Let P = inRay.origin and v = inRay.direction,
		// so points on the ray have the form P + t v for t >= 0.
		// Let X = P + t v, so y = X - Q = P + t v - Q.
		// Furthermore, define u = P - Q, so y = u + t v.  Now we have
		// (u + t v) dot (u + t v) - ((u + t v) dot w)^2 <= r^2.  Expand it out:
		// u dot u + 2 t u dot v + t^2 v dot v - (u dot w + t v dot w)^2 <= r^2
		// u dot u + 2 t u dot v + t^2 v dot v - (u dot w)^2 - 2 t (u dot w)(v dot w) - t^2(v dot w)^2 <= r^2
		// And collect powers of t.
		// t^2(v dot w - (v dot w)^2) + 2t(u dot v - (u dot w)(v dot w)) + u dot u - (u dot w)^2 <= r^2
		// If we let z = v - (v dot w) w, and recall that w is a unit vector, we can verify that
		// z dot z = v dot w - (v dot w)^2 and
		// u dot z = u dot v - (u dot w)(v dot w), so the inequality becomes
		// t^2(z dot z) + 2 t (u dot z) + u dot u - (u dot w)^2 <= r^2
		// Since the leading coefficient z dot z cannot be negative, we can never have a
		// solution set with two branches.
		TQ3Vector3D u = inRay.origin - inCyl.axis.origin;
		TQ3Vector3D z = inRay.direction -
			Q3Dot3D( inRay.direction, inCyl.axis.direction ) * inCyl.axis.direction;
		float A = Q3Dot3D( z, z );
		float B = 2.0f * Q3Dot3D( u, z );
		float uDotW = Q3Dot3D( u, inCyl.axis.direction );
		float C = Q3Dot3D( u, u ) - uDotW * uDotW - inCyl.radius * inCyl.radius;
		// Now it's A t^2 + B t + C <= 0 .
		if (fabsf( A ) < kQ3RealZero) // degenerates to a linear inequality?
		{
			if (fabsf( B ) < kQ3RealZero)
			{
				if (C > 0.0f)
				{
					// no solutions
					solution.maximum = -1.0f;
				}
			}
			else
			{
				float tBound = - C / B;
				if (B > 0.0f)
				{
					solution.maximum = std::min( solution.maximum, tBound );
				}
				else
				{
					solution.minimum = std::max( solution.minimum, tBound );
				}
			}
		}
		else // common case, A > 0
		{
			float discrim = B*B - 4.0f * A * C;
			if (discrim < 0.0f)
			{
				// no solutions
				solution.maximum = -1.0f;
			}
			else
			{
				float rootDiscrim = std::sqrt( discrim );
				float denom = 1.0f / (2.0f * A);
				Interval quadSol;
				quadSol.minimum = (-B - rootDiscrim) * denom;
				quadSol.maximum = (-B + rootDiscrim) * denom;
				solution = Intersect( solution, quadSol );
			}
		}
	}
	
	return solution;
}


/*!
	@function	Ray3D_IntersectShadowCone
	@abstract	Find nonnegative parameter values of a ray that meet a shadow cone.
	@discussion	We assume that the axis direction of the cone is a unit vector, but we only
				assume that the direction of the ray is nonzero. 
*/
static Interval Ray3D_IntersectShadowCone( const TQ3Ray3D& inRay, const ShadowCone& inCone )
{
	Interval solution;
	solution.minimum = 0.0f;
	solution.maximum = kQ3Infinity;
	
	// In order for a ray point, rayOrigin + t * rayDir, to be on the right side of the
	// cone's base plane, we need
	// (rayOrigin + t * rayDir - coneOrigin) dot coneDir >= apexToBasePlane, or
	// (rayOrigin - coneOrigin) dot coneDir - apexToBasePlane + t * rayDir dot coneDir >= 0 .
	float constantTerm = Q3Dot3D( inRay.origin - inCone.axis.origin, inCone.axis.direction ) -
		inCone.apexToBasePlane;
	float linearCoeff = Q3Dot3D( inRay.direction, inCone.axis.direction );
	if (fabsf( linearCoeff ) < kQ3RealZero)
	{
		if (constantTerm >= 0.0f)
		{
			// no constraint on solution
		}
		else
		{
			// no solution!
			solution.maximum = -1.0f;
		}
	}
	else
	{
		float tBoundary = - constantTerm / linearCoeff;
		if (linearCoeff > 0.0f)
		{
			solution.minimum = std::max( tBoundary, 0.0f );
		}
		else // linearCoeff < 0.0f, since we know it's not zero
		{
			solution.maximum = tBoundary;
		}
	}
	
	if (! solution.IsEmpty())
	{
		// For convenience, let A = inCone.axis.origin and w = inCone.axis.direction.
		// Let m be the cosine of the angle between the axis and surface of the cone.
		// Now ignoring the base plane, a point X belongs to the cone just in case
		// angle( X-A, w ) <= angle( w, surface ), or
		// cos( angle( X-A, w ) ) >= m.
		// Let u = X - A.  Then our condition is
		// cos( angle( u, w ) ) >= m, or
		// (u dot w) / sqrt(u dot u) >= m, or
		// u dot w >= m sqrt( u dot u ) .
		// Squaring both sides (possibly producing extra solutions),
		// (u dot w)^2 >= m^2 (u dot u) , or
		// m^2 (u dot u) - (u dot w)^2 <= 0 .
		// What we're actually given is s = inCone.angleSine, and a trigonometric
		// identity says that m^2 = 1 - s^2.
		// We want to investigate when the ray is in the cone.
		// Let P = inRay.origin and v = inRay.direction, so that a point on the ray
		// is of the form P + t v for t >= 0.
		// Let X = P + t v, so u = P + t v - A.
		// Let h = P - A, so u = h + t v.  Now our inequality becomes
		// m^2 (h + t v) dot (h + t v) - ((h + t v) dot w)^2 <= 0 .
		// Expanding and collecting,
		// t2(m^2 (v dot v) - (v dot w)^2) + 2t(m^2 v dot h - (h dot w)(v dot w)) +
		//   m^2 h dot h - (h dot w)^2 <= 0 .
		const TQ3Vector3D& v( inRay.direction );
		const TQ3Vector3D& w( inCone.axis.direction );
		TQ3Vector3D h = inRay.origin - inCone.axis.origin;
		float msq = 1.0f - inCone.angleSine * inCone.angleSine;
		float vDotW = Q3Dot3D( v, w );
		float hDotW = Q3Dot3D( h, w );
		float A = msq * Q3Dot3D( v, v ) - vDotW * vDotW;
		float B = 2.0f * (msq * Q3Dot3D( v, h ) - hDotW * vDotW);
		float C = msq * Q3Dot3D( h, h ) - hDotW * hDotW;
		// Now it's A t^2 + B t + C <= 0 .
		float discrim = B*B - 4.0f * A * C;
		if (fabsf( A ) < kQ3RealZero) // degenerates to a linear inequality?
		{
			if (fabsf( B ) < kQ3RealZero)
			{
				if (C > 0.0f)
				{
					// no solutions
					solution.maximum = -1.0f;
				}
			}
			else
			{
				float tBound = - C / B;
				if (B > 0.0f)
				{
					solution.maximum = std::min( solution.maximum, tBound );
				}
				else
				{
					solution.minimum = std::max( solution.minimum, tBound );
				}
			}
		}
		else if (discrim < 0.0f)
		{
			// no solutions!
			solution.maximum = -1.0f;
		}
		else
		{
			float rootDiscrim = std::sqrt( discrim );
			float denom = 1.0f / (2.0f * A);
			Interval quadSol;
			if (A > 0.0f)
			{
				// Single solution set, between the roots of the equation.
				quadSol.minimum = (-B - rootDiscrim) * denom;
				quadSol.maximum = (-B + rootDiscrim) * denom;
			}
			else // A < 0
			{
				// The solution set has the form (-inf, root1] union [root2, inf),
				// Which part we really want depends on the sign of v dot w.
				if (vDotW > 0.0f)
				{
					// Note that here denom < 0, so we get the higher root
					// by choosing the minus sign before rootDiscrim.
					quadSol.minimum = (-B - rootDiscrim) * denom;
					quadSol.maximum = kQ3Infinity;
				}
				else
				{
					quadSol.minimum = 0.0f;
					quadSol.maximum = (-B + rootDiscrim) * denom;
				}
			}
			
			solution = Intersect( solution, quadSol );
		}
	}

	return solution;
}


/*!
	@function	TestBoundingBoxAgainstHalfPlane
	@abstract	Test whether a bounding box is all inside a half-plane, all
				outside, or neither.
	@discussion	As discussed with GetFrustumPlanesInFrustumSpace, the inside of
				the half-plane consists of points (x, y, z) such that
				(x, y, z, 1) dot inPlane <= 0.0.
				
				The obvious way to test a bounding box would be to test each of
				the 8 corners against the half-plane.  Doing so would use 32
				multiplications, or 24 if you omit the 1 * inPlane.w products.
				But by taking advantage of the linearity of the dot product and
				the special structure of the bounding box, we can do the test
				with only 6 multiplications.
*/
static HalfPlaneResult TestBoundingBoxAgainstHalfPlane(
							const TQ3BoundingBox& inBBox,
							const TQ3RationalPoint4D& inPlane )
{
	float baseValue = inBBox.min.x * inPlane.x + inBBox.min.y * inPlane.y +
		inBBox.min.z * inPlane.z + inPlane.w;
	float minValue = baseValue;
	float maxValue = baseValue;
	TQ3Vector3D	diff;
	Q3FastPoint3D_Subtract( &inBBox.max, &inBBox.min, &diff );
	if (inPlane.x > 0.0f)
	{
		maxValue += inPlane.x * diff.x;
	}
	else
	{
		minValue += inPlane.x * diff.x;
	}
	
	if (inPlane.y > 0.0f)
	{
		maxValue += inPlane.y * diff.y;
	}
	else
	{
		minValue += inPlane.y * diff.y;
	}
	
	if (inPlane.z > 0.0f)
	{
		maxValue += inPlane.z * diff.z;
	}
	else
	{
		minValue += inPlane.z * diff.z;
	}
	
	HalfPlaneResult result = kHalfPlaneResult_PartInside;
	if (maxValue <= 0.0f)
	{
		result = kHalfPlaneResult_AllInside;
	}
	else if (minValue > 0.0f)
	{
		result = kHalfPlaneResult_AllOutside;
	}
	
	return result;
}

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


static bool
IsPointInsidePlane( const TQ3Point3D& inPt, const TQ3RationalPoint4D& inPlane )
{
	return inPt.x * inPlane.x + inPt.y * inPlane.y + inPt.z * inPlane.z +
		inPlane.w <= 0.0f;
}

static bool
IsPointInsidePlane( const TQ3RationalPoint4D& inPt, const TQ3RationalPoint4D& inPlane )
{
	return inPt.x * inPlane.x + inPt.y * inPlane.y + inPt.z * inPlane.z +
		inPt.w * inPlane.w <= 0.0f;
}


/*!
	@function	GetFrustumPlanesInFrustumSpace
	@abstract	Get the boundary planes of the view frustum in frustum coordinates.
	@discussion	The visible region of frustum space ranges from 0 (near) to -1
				(far) in z, and -1 to 1 in x and y.
				
				For an example of how we represent the planes, consider the
				bottom plane, y = -1.  The inner half-space is given by
				y >= -1.  Put this in a form where we have something less or
				equal to 0, as - y -1 <= 0.  This can be expressed using a dot
				product as [0, -1, 0, -1] dot [x, y, z, 1] <= 0.  We represent
				this with the rational point [0, -1, 0, -1].
*/
static const Frustum&
GetFrustumPlanesInFrustumSpace()
{
	static const Frustum	theFrustum =
	{
		{
			{ 0.0f, 0.0f, 1.0f, 0.0f },		// front (near)
			{ 0.0f, 0.0f, -1.0f, -1.0f },	// rear (far)
			{ 0.0f, -1.0f, 0.0f, -1.0f },	// bottom
			{ 0.0f, 1.0f, 0.0f, -1.0f },	// top
			{ -1.0f, 0.0f, 0.0f, -1.0f },	// left
			{ 1.0f, 0.0f, 0.0f, -1.0f }		// right
		}
	};
	return theFrustum;
}



/*!
	@function	GetFrustumPlanesInWorldSpace
	@abstract	Get the boundary planes of the view frustum in world coordinates.
	@discussion	See the discussion of GetFrustumPlanesInFrustumSpace to see how
				planes are represented as rational points.
*/
static void
GetFrustumPlanesInWorldSpace(
			TQ3CameraObject inCamera,
			Frustum& outFrustum )
{
	const Frustum&	frustumInFrustumSpace( GetFrustumPlanesInFrustumSpace() );
	
	// If we were transforming points, we would use the frustum to world
	// matrix.  But a plane is more like a normal vector, so we the inverse
	// transpose.
	TQ3Matrix4x4	worldToFrustum, worldToFrustumTranspose;
	Q3Camera_GetWorldToFrustum( inCamera, &worldToFrustum );
	Q3Matrix4x4_Transpose( &worldToFrustum, &worldToFrustumTranspose );
	
	for (int i = 0; i < 6; ++i)
	{
		E3RationalPoint4D_Transform( &frustumInFrustumSpace.planes[i],
			&worldToFrustumTranspose,
			&outFrustum.planes[i] );
	}
}



/*!
	@function	GetFrustumInLocalSpace
	@abstract	Get the boundary planes of the view frustum in local coordinates.
	@discussion	See the discussion of GetFrustumPlanesInFrustumSpace to see how
				planes are represented as rational points.
*/
static void
GetFrustumInLocalSpace(
			TQ3ViewObject inView,
			Frustum& outFrustum )
{
	const Frustum&	frustumInFrustumSpace( GetFrustumPlanesInFrustumSpace() );

	// If we were transforming points, we would use the frustum to local
	// matrix.  But a plane is more like a normal vector, so we the inverse
	// transpose.
	const TQ3Matrix4x4& localTofrustum( E3View_State_GetMatrixLocalToFrustum( inView ) );
	TQ3Matrix4x4 localToFrustumTranspose;
	E3Matrix4x4_Transpose( &localTofrustum, &localToFrustumTranspose );

	for (int i = 0; i < 6; ++i)
	{
		E3RationalPoint4D_Transform( &frustumInFrustumSpace.planes[i],
			&localToFrustumTranspose,
			&outFrustum.planes[i] );
	}
}



/*!
	@function	GetFrustumCornersInLocalSpace
	@abstract	Compute the view frustum corners in local coordinates,
				or determine that the frustum is infinite.
*/
static void
GetFrustumCornersInLocalSpace(	TQ3ViewObject inView, TQ3RationalPoint4D* out8Points )
{
	const TQ3RationalPoint4D	frustumCornersInFrustumSpace[8] =
	{
		{ -1.0f, -1.0f, 0.0f, 1.0f },	// front bottom left
		{ -1.0f, 1.0f, 0.0f, 1.0f },	// front top left
		{ 1.0f, -1.0f, 0.0f, 1.0f },	// front bottom right
		{ 1.0f, 1.0f, 0.0f, 1.0f },		// front top right
		{ -1.0f, -1.0f, -1.0f, 1.0f },	// rear bottom left
		{ -1.0f, 1.0f, -1.0f, 1.0f },	// rear top left
		{ 1.0f, -1.0f, -1.0f, 1.0f },	// rear bottom right
		{ 1.0f, 1.0f, -1.0f, 1.0f }		// rear top right
	};
	const TQ3Matrix4x4&	frustumToLocalMatrix(
		E3View_State_GetMatrixFrustumToLocal( inView ) );
	
	int i;
	for (i = 0; i < 8; ++i)
	{
		E3RationalPoint4D_Transform( &frustumCornersInFrustumSpace[i],
			&frustumToLocalMatrix,
			&out8Points[i] );
	}
	
	// Test whether we are in the infinite yon case.
	// This test works for both view angle aspect and orthographic cameras.
	// If we ever support another type of camera, it should be checked.
	const TQ3Matrix4x4& cameraToFrustumMatrix(
		E3View_State_GetMatrixCameraToFrustum( inView ) );
	bool	isInfiniteYon = (cameraToFrustumMatrix.value[2][2] +
		cameraToFrustumMatrix.value[2][3] == 0.0f);
	
	// In the infinite yon case, the 4 far corners should be infinite points.
	// Due to floating point inaccuracy, the w coordinates may not be exactly
	// zero.  Make them so.
	if (isInfiniteYon)
	{
		for (i = 4; i < 8; ++i)
		{
			out8Points[i].w = 0.0f;
		}
	}
}


/*!
	@function	GetFrustumCornersInWorldSpace
	@abstract	Compute the view frustum corners in world coordinates,
				or determine that the frustum is infinite.
*/
static void
GetFrustumCornersInWorldSpace(	TQ3CameraObject inCamera, TQ3RationalPoint4D* out8Points )
{
	TQ3RationalPoint4D	frustumCornersInFrustumSpace[8] =
	{
		{ -1.0f, -1.0f, 0.0f, 1.0f },	// front bottom left
		{ -1.0f, 1.0f, 0.0f, 1.0f },	// front top left
		{ 1.0f, -1.0f, 0.0f, 1.0f },	// front bottom right
		{ 1.0f, 1.0f, 0.0f, 1.0f },		// front top right
		{ -1.0f, -1.0f, -1.0f, 1.0f },	// rear bottom left
		{ -1.0f, 1.0f, -1.0f, 1.0f },	// rear top left
		{ 1.0f, -1.0f, -1.0f, 1.0f },	// rear bottom right
		{ 1.0f, 1.0f, -1.0f, 1.0f }		// rear top right
	};
	TQ3Matrix4x4	worldToFrustum, frustumToWorld;
	Q3Camera_GetWorldToFrustum( inCamera, &worldToFrustum );
	Q3Matrix4x4_Invert( &worldToFrustum, &frustumToWorld );
	
	int i;
	for (i = 0; i < 8; ++i)
	{
		E3RationalPoint4D_Transform( &frustumCornersInFrustumSpace[i],
			&frustumToWorld, &out8Points[i] );
	}
	
	// Test whether we are in the infinite yon case.
	// This test works for both view angle aspect and orthographic cameras.
	// If we ever support another type of camera, it should be checked.
	TQ3Matrix4x4	cameraToFrustumMatrix;
	Q3Camera_GetViewToFrustum( inCamera, &cameraToFrustumMatrix );
	bool	isInfiniteYon = (cameraToFrustumMatrix.value[2][2] +
		cameraToFrustumMatrix.value[2][3] == 0.0f);
	
	// In the infinite yon case, the 4 far corners should be infinite points.
	// Due to floating point inaccuracy, the w coordinates may not be exactly
	// zero.  Make them so.
	if (isInfiniteYon)
	{
		for (i = 4; i < 8; ++i)
		{
			out8Points[i].w = 0.0f;
		}
	}
}

static void MakeOneEdge(	const TQ3RationalPoint4D& inStart,
							const TQ3RationalPoint4D& inEnd,
							TQ3Vector3D& outEdge )
{
	if (inEnd.w == 0.0f)	// infinite point
	{
		outEdge = InfinitePointToVector( inEnd );
	}
	else
	{
		TQ3Point3D	startPt, endPt;
		Q3FastRationalPoint4D_To3D( &inStart, &startPt );
		Q3FastRationalPoint4D_To3D( &inEnd, &endPt );
		Q3FastPoint3D_Subtract( &endPt, &startPt, &outEdge );
	}
}

/*!
	@function	GetFrustumEdges
	@abstract	Given the corners of the frustum, compute the
				edge directions.
	@discussion	We assume that the corners are computed by
				GetFrustumCornersInLocalSpace.
				
				It is not important that the edge directions be unit vectors,
				but we must account for the possibility that rear frustum
				corners may be infinite.
*/
static void GetFrustumEdges( const TQ3RationalPoint4D* in8Corners,
										TQ3Vector3D* out6Edges )
{
	MakeOneEdge( in8Corners[0], in8Corners[4], out6Edges[0] );	// bottom left
	MakeOneEdge( in8Corners[1], in8Corners[5], out6Edges[1] );	// top left
	MakeOneEdge( in8Corners[2], in8Corners[6], out6Edges[2] );	// bottom right
	MakeOneEdge( in8Corners[3], in8Corners[7], out6Edges[3] );	// top right

	MakeOneEdge( in8Corners[0], in8Corners[2], out6Edges[4] );	// horizontal front
	MakeOneEdge( in8Corners[0], in8Corners[1], out6Edges[5] );	// vertical front
}

static void MakeOneEdge(	const TQ3RationalPoint4D& inStart,
							const TQ3RationalPoint4D& inEnd,
							FrustumEdge& outEdge )
{
	outEdge.start = inStart;
	outEdge.end = inEnd;
}

/*!
	@function	GetLocalFrustumEdgeRays
	@abstract	Get the edges of the view frustum in local coordinates.
*/
static void GetLocalFrustumEdges( TQ3ViewObject inView,
									FrustumEdges& outEdges )
{
	TQ3RationalPoint4D corners[8];
	GetFrustumCornersInLocalSpace( inView, corners );

	MakeOneEdge( corners[0], corners[4], outEdges.edge[0] );	// bottom left
	MakeOneEdge( corners[1], corners[5], outEdges.edge[1] );	// top left
	MakeOneEdge( corners[2], corners[6], outEdges.edge[2] );	// bottom right
	MakeOneEdge( corners[3], corners[7], outEdges.edge[3] );	// top right

	MakeOneEdge( corners[0], corners[2], outEdges.edge[4] );	// front bottom
	MakeOneEdge( corners[1], corners[3], outEdges.edge[5] );	// front top
	MakeOneEdge( corners[0], corners[1], outEdges.edge[6] );	// front left
	MakeOneEdge( corners[2], corners[3], outEdges.edge[7] );	// front right

	MakeOneEdge( corners[4], corners[6], outEdges.edge[8] );	// rear bottom
	MakeOneEdge( corners[5], corners[7], outEdges.edge[9] );	// rear top
	MakeOneEdge( corners[4], corners[5], outEdges.edge[10] );	// rear left
	MakeOneEdge( corners[6], corners[7], outEdges.edge[11] );	// rear right
}

static float VecDotPoint(	const TQ3Vector3D& inDirection,
							const TQ3Point3D& inPoint )
{
	return inDirection.x * inPoint.x +
		inDirection.y * inPoint.y +
		inDirection.z * inPoint.z;
}

static float VecDotPoint(	const TQ3Vector3D& inDirection,
							const TQ3RationalPoint4D& inPoint )
{
	float	theDot = inDirection.x * inPoint.x +
		inDirection.y * inPoint.y +
		inDirection.z * inPoint.z;
	
	if (inPoint.w == 0.0f)
	{
		if (theDot > 0.0f)
		{
			theDot = kQ3Infinity;
		}
		else if (theDot < 0.0f)
		{
			theDot = - kQ3Infinity;
		}
	}
	else
	{
		theDot /= inPoint.w;
	}
	
	return theDot;
}

static bool
DirectionSeparatesCorners(	const TQ3Vector3D& inDirection,
							const TQ3Point3D* inBoxCorners,
							const TQ3RationalPoint4D* inFrustumCorners )
{
	float	boxDot = VecDotPoint( inDirection, inBoxCorners[0] );
	float	frustDot = VecDotPoint( inDirection, inFrustumCorners[0] );

	float	minBoxDot = boxDot;
	float	maxBoxDot = boxDot;

	float	minFrustDot = frustDot;
	float	maxFrustDot = frustDot;
	
	int i;
	for (i = 1; i < 8; ++i)
	{
		boxDot = VecDotPoint( inDirection, inBoxCorners[i] );
		frustDot = VecDotPoint( inDirection, inFrustumCorners[i] );
		
		if (boxDot < minBoxDot)
		{
			minBoxDot = boxDot;
		}
		else if (boxDot > maxBoxDot)
		{
			maxBoxDot = boxDot;
		}
		
		if (frustDot < minFrustDot)
		{
			minFrustDot = frustDot;
		}
		else if (frustDot > maxFrustDot)
		{
			maxFrustDot = frustDot;
		}
	}
	
	return (minFrustDot > maxBoxDot) || (minBoxDot > maxFrustDot);
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
	TQ3Uns32		i, whichPlane;
	TQ3Boolean		isInside;
	enum EQuadrant
	{
		kRight,
		kLeft,
		kMiddle
	};
	EQuadrant			quadrant[3];



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



	// Find candidate planes
	for (i = 0; i < 3; i++)
	{
		if (origin[i] < minB[i])
		{
			quadrant[i]       = kLeft;
			candidatePlane[i] = minB[i];
			isInside          = kQ3False;
		}
		else if (origin[i] > maxB[i])
		{
			quadrant[i]       = kRight;
			candidatePlane[i] = maxB[i];
			isInside          = kQ3False;
		}
		else
		{
			quadrant[i] = kMiddle;
			// The next line is not mathematically needed,
			// but serves to remove a bogus warning about
			// an uninitialized value.
			candidatePlane[i] = 0.0f;
		}
	}



	// Check for the ray origin being inside the bounding box
	if (isInside)
	{
		if (hitPoint)
			*hitPoint = theRay->origin;
	}
	else
	{
		// Calculate T distances to candidate planes
		for (i = 0; i < 3; i++)
		{
			if ((quadrant[i] != kMiddle) && (dir[i] != 0.0f))
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
		if (maxT[whichPlane] >= 0.0f)
		{
			isInside = kQ3True;
			
			for (i = 0; i < 3; i++)
			{
				if (whichPlane != i)
				{
					coord[i] = origin[i] + maxT[whichPlane] * dir[i];
					if (coord[i] < minB[i] || coord[i] > maxB[i])
					{
						isInside = kQ3False;
						break;
					}
				}
				else
					coord[whichPlane] = candidatePlane[whichPlane];
			}
			
			if ( isInside && (hitPoint != NULL) )
			{
				hitPoint->x = coord[0];
				hitPoint->y = coord[1];
				hitPoint->z = coord[2];
			}
		}
	}


	
	return isInside;
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
//				<http://www.graphics.cornell.edu/pubs/1997/MT97.pdf>
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
						outMaxParam = kQ3Infinity;
						didIntersect = true;
					}
					else
					{
						outMinParam = diffLen;
						outMaxParam = kQ3Infinity;
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
					outMaxParam = kQ3Infinity;
					didIntersect = true;
				}
			}
			else
			{
				if (isRayEndInCone)
				{
					outMinParam = 0.0f;
					outMaxParam = kQ3Infinity;
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
			float	rootDisc = sqrtf( discriminant );
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
					outMaxParam = kQ3Infinity;
					didIntersect = true;
				}
				else
				{
					outMinParam = 0.0f;
					outMaxParam = kQ3Infinity;
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


/*!
	@function	E3BoundingBox_IntersectViewFrustum
	@abstract	Determine whether a bounding box in local coordinates
				intersects the view frustum.
	@param		inView			The view object.
	@param		inLocalBox		A bounding box in local coordinates.
	@result		True if the box intersects the frustum.
*/
bool	E3BoundingBox_IntersectViewFrustum(
									TQ3ViewObject inView,
									const TQ3BoundingBox& inLocalBox )
{
	if (inLocalBox.isEmpty)
	{
		return false;
	}
	
	// It is tempting to do a quick and dirty test by transforming the box
	// corners to frustum coordinates and then testing them against the simple
	// frustum bounds.  However, that can give an incorrect result in some
	// cases where a large bounding box is partially behind the camera.
	
	// The basic principle we use is that if two convex polyhedrons do not
	// intersect, then there is a plane that separates them, and if there is a
	// separating plane, there is one that is either parallel to a bounding
	// plane of one of the polyhedrons, or one normal to the cross product of
	// an edge of one polyhedron and an edge of the other.
	
	// Phase 1: test bounding box corners against frustum planes, in local
	// coordinates.  If the whole box is inside of all planes, we can
	// return true.  If the whole box is outside of some plane, we can
	// return false.  This phase should usually suffice in the common case
	// where the bounding box is small relative to the view frustum.
	const TQ3RationalPoint4D*	localFrustumPlanes =
		E3View_State_GetFrustumPlanesInLocalSpace( inView );
	int	planeIndex;
	bool	isAllInside = true;
	HalfPlaneResult halfPlaneTest;
	
	for (planeIndex = 0; planeIndex < 6; ++planeIndex)
	{
		halfPlaneTest = TestBoundingBoxAgainstHalfPlane( inLocalBox,
			localFrustumPlanes[ planeIndex ] );
		
		if (halfPlaneTest == kHalfPlaneResult_AllOutside)
		{
			return false;
		}
		else if (halfPlaneTest == kHalfPlaneResult_PartInside)
		{
			isAllInside = false;
		}
	}
	if (isAllInside)
	{
		// All corners are inside all planes, we are done.
		return true;
	}
	
	
	// Phase 2: Test frustum corners against bounding box planes, in local
	// coordinates.  This is conceptually similar to phase 1, but the far
	// corners of the frustum might be infinite points, so we must use rational
	// coordinates.
	TQ3RationalPoint4D	frustumLocalCorners[8];
	GetFrustumCornersInLocalSpace( inView, frustumLocalCorners );
	TQ3RationalPoint4D	boxLocalPlanes[6] =
	{
		{ -1.0f, 0.0f, 0.0f, inLocalBox.min.x },
		{ 1.0f, 0.0f, 0.0f, -inLocalBox.max.x },
		{ 0.0f, -1.0f, 0.0f, inLocalBox.min.y },
		{ 0.0f, 1.0f, 0.0f, -inLocalBox.max.y },
		{ 0.0f, 0.0f, -1.0f, inLocalBox.min.z },
		{ 0.0f, 0.0f, 1.0f, -inLocalBox.max.z }
	};
	bool isACornerOutside = false;
	for (planeIndex = 0; planeIndex < 6; ++planeIndex)
	{
		bool isACornerInside = false;
		for (int cornerIndex = 0; cornerIndex < 8; ++cornerIndex)
		{
			if (IsPointInsidePlane( frustumLocalCorners[cornerIndex],
				boxLocalPlanes[planeIndex] ))
			{
				isACornerInside = true;
			}
			else
			{
				isACornerOutside = true;
			}
		}
		if (! isACornerInside)
		{
			// All corners are outside this plane, we are done.
			return false;
		}
	}
	if (! isACornerOutside)
	{
		// All corners are inside all planes, we are done.
		return true;
	}
	
	
	// Phase 3: Look for a separating plane determined by a cross product of
	// edge vectors.  Note that a box has only 3 distinct edge directions, and
	// a frustum has only 6.
	TQ3Vector3D	frustumLocalEdges[6];
	GetFrustumEdges( frustumLocalCorners, frustumLocalEdges );
	TQ3Point3D	boxLocalCorners[8];
	E3BoundingBox_GetCorners( &inLocalBox, boxLocalCorners );
	TQ3Vector3D	boxLocalEdges[3] =
	{
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
	};
	TQ3Vector3D	crossProds[18];
	int	i, j;
	for (i = 0; i < 6; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			Q3FastVector3D_Cross( &frustumLocalEdges[i], &boxLocalEdges[j],
				&crossProds[ 6 * j + i ] );
		}
	}
	for (i = 0; i < 18; ++i)
	{
		if (DirectionSeparatesCorners( crossProds[i], boxLocalCorners, frustumLocalCorners ))
		{
			return false;
		}
	}


	return true;
}


/*!
	@function	E3BoundingBox_IntersectCameraFrustum
	@abstract	Determine whether a bounding box in world coordinates
				intersects the view frustum of a camera.
	@param		inCamera		The camera object.
	@param		inWorldBox		A bounding box in world coordinates.
	@result		True if the box intersects the frustum.
*/
bool	E3BoundingBox_IntersectCameraFrustum(
									TQ3CameraObject inCamera,
									const TQ3BoundingBox& inWorldBox )
{
	if (inWorldBox.isEmpty)
	{
		return false;
	}
	
	// It is tempting to do a quick and dirty test by transforming the box
	// corners to frustum coordinates and then testing them against the simple
	// frustum bounds.  However, that can give an incorrect result in some
	// cases where a large bounding box is partially behind the camera.
	
	// The basic principle we use is that if two convex polyhedrons do not
	// intersect, then there is a plane that separates them, and if there is a
	// separating plane, there is one that is either parallel to a bounding
	// plane of one of the polyhedrons, or one normal to the cross product of
	// an edge of one polyhedron and an edge of the other.
	
	// Phase 1: test bounding box corners against frustum planes, in world
	// coordinates.  If the whole box is inside of all planes, we can
	// return true.  If the whole box is outside of some plane, we can
	// return false.  This phase should usually suffice in the common case
	// where the bounding box is small relative to the view frustum.
	Frustum	worldFrustum;
	GetFrustumPlanesInWorldSpace( inCamera, worldFrustum );
	int	planeIndex;
	bool	isAllInside = true;
	HalfPlaneResult halfPlaneTest;
	
	for (planeIndex = 0; planeIndex < 6; ++planeIndex)
	{
		halfPlaneTest = TestBoundingBoxAgainstHalfPlane( inWorldBox,
			worldFrustum.planes[ planeIndex ] );
		
		if (halfPlaneTest == kHalfPlaneResult_AllOutside)
		{
			return false;
		}
		else if (halfPlaneTest == kHalfPlaneResult_PartInside)
		{
			isAllInside = false;
		}
	}
	if (isAllInside)
	{
		// All corners are inside all planes, we are done.
		return true;
	}
	
	
	// Phase 2: Test frustum corners against bounding box planes, in local
	// coordinates.  This is conceptually similar to phase 1, but the far
	// corners of the frustum might be infinite points, so we must use rational
	// coordinates.
	TQ3RationalPoint4D	frustumWorldCorners[8];
	GetFrustumCornersInWorldSpace( inCamera, frustumWorldCorners );
	TQ3RationalPoint4D	boxWorldPlanes[6] =
	{
		{ -1.0f, 0.0f, 0.0f, inWorldBox.min.x },
		{ 1.0f, 0.0f, 0.0f, -inWorldBox.max.x },
		{ 0.0f, -1.0f, 0.0f, inWorldBox.min.y },
		{ 0.0f, 1.0f, 0.0f, -inWorldBox.max.y },
		{ 0.0f, 0.0f, -1.0f, inWorldBox.min.z },
		{ 0.0f, 0.0f, 1.0f, -inWorldBox.max.z }
	};
	bool isACornerOutside = false;
	for (planeIndex = 0; planeIndex < 6; ++planeIndex)
	{
		bool isACornerInside = false;
		for (int cornerIndex = 0; cornerIndex < 8; ++cornerIndex)
		{
			if (IsPointInsidePlane( frustumWorldCorners[cornerIndex],
				boxWorldPlanes[planeIndex] ))
			{
				isACornerInside = true;
			}
			else
			{
				isACornerOutside = true;
			}
		}
		if (! isACornerInside)
		{
			// All corners are outside this plane, we are done.
			return false;
		}
	}
	if (! isACornerOutside)
	{
		// All corners are inside all planes, we are done.
		return true;
	}
	
	
	// Phase 3: Look for a separating plane determined by a cross product of
	// edge vectors.  Note that a box has only 3 distinct edge directions, and
	// a frustum has only 6.
	TQ3Vector3D	frustumWorldEdges[6];
	GetFrustumEdges( frustumWorldCorners, frustumWorldEdges );
	TQ3Point3D	boxWorldCorners[8];
	E3BoundingBox_GetCorners( &inWorldBox, boxWorldCorners );
	TQ3Vector3D	boxWorldEdges[3] =
	{
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
	};
	TQ3Vector3D	crossProds[18];
	int	i, j;
	for (i = 0; i < 6; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			Q3FastVector3D_Cross( &frustumWorldEdges[i], &boxWorldEdges[j],
				&crossProds[ 6 * j + i ] );
		}
	}
	for (i = 0; i < 18; ++i)
	{
		if (DirectionSeparatesCorners( crossProds[i], boxWorldCorners, frustumWorldCorners ))
		{
			return false;
		}
	}


	return true;
}


/*!
	@function	E3Math_CalcLocalFrustumPlanes
	@abstract	Compute the boundary half-planes of the view frustum in local
				coordinates.
	@discussion	A half-plane is expressed as a rational point, where a point
				(x, y, z) is considered "inside" the half-plane whenever
				(x, y, z, 1) dot plane <= 0.0.
	@param		inLocalToFrustum	Local to frustum matrix.
	@param		out6Planes			Array of 6 rational points to receive the
									planes.
*/
void	E3Math_CalcLocalFrustumPlanes(
									const TQ3Matrix4x4& inLocalToFrustum,
									TQ3RationalPoint4D* out6Planes )
{
	const Frustum&	planesInFrustumSpace =
		GetFrustumPlanesInFrustumSpace();
	
	// If we were transforming points, we would use the frustum to local
	// matrix.  But a plane is more like a normal vector, so we the inverse
	// transpose.
	TQ3Matrix4x4	localToFrustumTranspose;
	E3Matrix4x4_Transpose( &inLocalToFrustum,
		&localToFrustumTranspose );
	
	for (int i = 0; i < 6; ++i)
	{
		E3RationalPoint4D_Transform( &planesInFrustumSpace.planes[i],
			&localToFrustumTranspose,
			&out6Planes[i] );
	}
}


/*!
	@function	E3BoundingSphere_DirectionalShadowIntersectsViewFrustum
	@abstract	Test whether a shadow of a sphere cast by directional
				light might be partly visible.
*/
static bool E3BoundingSphere_DirectionalShadowIntersectsViewFrustum(
									TQ3ViewObject inView,
									const TQ3BoundingSphere& inLocalSphere,
									const TQ3Vector3D& inLocalLightDir )
{
	bool mayIntersect = false;
	
	// Set up the shadow cylinder.
	ShadowCylinder cyl;
	cyl.radius = inLocalSphere.radius;
	Q3FastVector3D_Normalize( &inLocalLightDir, &cyl.axis.direction );
	cyl.axis.origin = inLocalSphere.origin - inLocalSphere.radius * cyl.axis.direction;
	
	// Get the frustum planes.
	Frustum localFrustum;
	GetFrustumInLocalSpace( inView, localFrustum );
	
	// Test whether the axis ray intersects the frustum.
	if ( Ray3D_IntersectFrustum( cyl.axis, localFrustum ).IsEmpty() )
	{
		// If the axis does not intersect the frustum, then either there is no
		// intersection, or else the shadow cylinder intersects an edge.
		FrustumEdges localEdges;
		GetLocalFrustumEdges( inView, localEdges );
		
		// Of the 12 edges, the last 4 edges are at infinity.
		// The cylinder could only hit a line at infinity if the cylinder's
		// axis hits that line, and we have already verified that the axis
		// does not hit the frustum, so we can skip those 4 edges.
		for (int i = 0; i < 8; ++i)
		{
			EdgeRay edgeRay;
			MakeEdgeRayFromEdge( localEdges.edge[i], edgeRay );
			
			Interval solRange = Ray3D_IntersectShadowCylinder( edgeRay.ray, cyl );
			Interval hitRange = Intersect( solRange, edgeRay.paramRange );
			if (! hitRange.IsEmpty())
			{
				mayIntersect = true;
				break;
			}
		}
	}
	else // intersection is not empty
	{
		mayIntersect = true;
	}
	
	return mayIntersect;
}

/*!
	@function	E3BoundingSphere_PositionalShadowIntersectsViewFrustum
	@abstract	Test whether a shadow of a sphere cast by positional
				light might be partly visible.
*/
static bool E3BoundingSphere_PositionalShadowIntersectsViewFrustum(
									TQ3ViewObject inView,
									const TQ3BoundingSphere& inLocalSphere,
									const TQ3Point3D& inLocalLightPos )
{
	bool mayIntersect = false;

	// Get the frustum planes.
	Frustum localFrustum;
	GetFrustumInLocalSpace( inView, localFrustum );
	
	// Set up the shadow cone.
	ShadowCone	cone;
	cone.axis.direction = Q3Normalize3D( inLocalSphere.origin - inLocalLightPos );
	cone.axis.origin = inLocalLightPos;
	float lightToSphereCenter = Q3Length3D( inLocalSphere.origin - inLocalLightPos );
	cone.apexToBasePlane = lightToSphereCenter - inLocalSphere.radius;
	cone.angleSine = inLocalSphere.radius / lightToSphereCenter;
	
	// Test whether the axis ray (not including the part on the wrong side of the
	// base plane) intersects the view frustum.
	Interval hitParams = Ray3D_IntersectFrustum( cone.axis, localFrustum );
	hitParams.minimum = std::max( hitParams.minimum, cone.apexToBasePlane );
	if (hitParams.IsEmpty())
	{
		// If the axis does not intersect the frustum, then either there is no
		// intersection, or else the shadow cone intersects an edge.
		FrustumEdges localEdges;
		GetLocalFrustumEdges( inView, localEdges );
		
		static const TQ3Point3D kZeroPt = { 0.0f, 0.0f, 0.0f };
		
		for (int i = 0; i < 12; ++i)
		{
			EdgeRay edgeRay;
			MakeEdgeRayFromEdge( localEdges.edge[i], edgeRay );
			Interval solution;
			if (edgeRay.isRayAtInfinity)
			{
				ShadowCone coneForInf;
				coneForInf.axis.origin = kZeroPt;
				solution = Ray3D_IntersectShadowCone( edgeRay.ray, coneForInf );
			}
			else // ordinary ray
			{
				solution = Ray3D_IntersectShadowCone( edgeRay.ray, cone );
			}
			hitParams = Intersect( solution, edgeRay.paramRange );
			if (! hitParams.IsEmpty())
			{
				mayIntersect = true;
				break;
			}
		}
	}
	else // intersection is not empty
	{
		mayIntersect = true;
	}
	
	return mayIntersect;
}

/*!
	@function	E3BoundingBox_ShadowIntersectsViewFrustum
	@abstract	Test whether a bounding box may cast a shadow into the view
				frustum.
	@param		inView			The view object.
	@param		inLocalBox		A bounding box in local coordinates.
	@param		inWorldLightPos	Position of the shadow-casting light in world
								coordinates.  The w component is 0.0 for a
								directional light, 1.0 for a positional light.
	@result		True means the shadow may be visible, false means definitely not.
*/
bool	E3BoundingBox_ShadowIntersectsViewFrustum(
									TQ3ViewObject inView,
									const TQ3BoundingBox& inLocalBox,
									const TQ3RationalPoint4D& inWorldLightPos )
{
	bool couldShadowBeVisible = true;

	// Convert the light position to local coordinates.
	const TQ3Matrix4x4& localToWorld( *E3View_State_GetMatrixLocalToWorld( inView ) );
	TQ3Matrix4x4 worldToLocal;
	E3Matrix4x4_Invert( &localToWorld, &worldToLocal );
	TQ3RationalPoint4D localLightPos;
	E3RationalPoint4D_Transform( &inWorldLightPos, &worldToLocal, &localLightPos );
	
	// Make a bounding sphere that encloses the bounding box.
	TQ3BoundingSphere	boundingSphere;
	Q3FastPoint3D_RRatio( &inLocalBox.min, &inLocalBox.max, 1.0f, 1.0f, &boundingSphere.origin );
	boundingSphere.radius = Q3FastPoint3D_Distance( &inLocalBox.min, &boundingSphere.origin );
	boundingSphere.isEmpty = kQ3False;
	
	// If the light is a finite light inside the bounding sphere, then the
	// shadow could be anywhere.
	if ( (localLightPos.w != 0.0f) &&
		IsPointInSphere( FinitePointToPoint( localLightPos ), boundingSphere ) )
	{
		couldShadowBeVisible = true;
	}
	else // light coming from outside the sphere
	{
		if (localLightPos.w == 0.0f)	// directional light
		{
			couldShadowBeVisible = E3BoundingSphere_DirectionalShadowIntersectsViewFrustum( inView,
				boundingSphere, InfinitePointToVector( localLightPos ) );
		}
		else // positional light
		{
			couldShadowBeVisible = E3BoundingSphere_PositionalShadowIntersectsViewFrustum( inView,
				boundingSphere, FinitePointToPoint( localLightPos ) );
		}
	}
	
	return couldShadowBeVisible;
}


/*!
	@function	E3BoundingBox_Intersect
	@abstract	Intersect two bounding boxes.
	@param		inBox1		A bounding box.
	@param		inBox2		A bounding box.
	@param		outSect		Receives the intersection.  May refer to the same
							storage as inBox1 or inBox2.
	@result		True if the intersection is nonempty.
*/
bool	E3BoundingBox_Intersect( const TQ3BoundingBox& inBox1,
								const TQ3BoundingBox& inBox2,
								TQ3BoundingBox& outSect )
{
	// Empty until proven nonempty
	outSect.isEmpty = kQ3True;

	if ( (inBox1.isEmpty == kQ3False) && (inBox2.isEmpty == kQ3False) )
	{
		outSect.min.x = E3Num_Max( inBox1.min.x, inBox2.min.x );
		outSect.max.x = E3Num_Min( inBox1.max.x, inBox2.max.x );
		
		if (outSect.min.x <= outSect.max.x)
		{
			outSect.min.y = E3Num_Max( inBox1.min.y, inBox2.min.y );
			outSect.max.y = E3Num_Min( inBox1.max.y, inBox2.max.y );
			
			if (outSect.min.y <= outSect.max.y)
			{
				outSect.min.z = E3Num_Max( inBox1.min.z, inBox2.min.z );
				outSect.max.z = E3Num_Min( inBox1.max.z, inBox2.max.z );
				
				if (outSect.min.z <= outSect.max.z)
				{
					outSect.isEmpty = kQ3False;
				}
			}
		}
	}
	
	return outSect.isEmpty == kQ3False;
}

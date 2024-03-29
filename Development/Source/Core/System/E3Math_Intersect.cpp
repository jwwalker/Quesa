/*  NAME:
        E3Math_Intersect.cpp

    DESCRIPTION:
        Math routines for Quesa, involving geometric intersections.
        
        Note that these routines are allowed to call other E3foo routines for
        speed, to avoid the trip back out through the Q3foo interface.

    COPYRIGHT:
        Copyright (c) 1999-2020, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#include "E3Camera.h"
#include "E3Math.h"
#include "E3Math_Intersect.h"
#include "E3View.h"
#include "QuesaMathOperators.hpp"
#include "CQ3ObjectRef_Gets.h"

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

static float Infinity()
{
	static float sInfinity = std::numeric_limits<float>::infinity();
	return sInfinity;
}

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
		outEdgeRay.paramRange.maximum = Infinity();
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

static bool IsPointInCone( const TQ3Point3D& inPt,
							const TQ3Ray3D& inConeAxis,
							float inConeCosine )
{
	TQ3Vector3D	apexToPt = inPt - inConeAxis.origin;
	
	return Q3Dot3D( apexToPt, inConeAxis.direction ) >=
		inConeCosine * Q3Length3D( apexToPt );
}

static bool IsPointInCone( const TQ3RationalPoint4D& inPt,
							const TQ3Ray3D& inConeAxis,
							float inConeCosine )
{
	TQ3Vector3D	toPt;
	if (fabsf(inPt.w) < FLT_EPSILON) // point at infinity
	{
		Q3FastRationalPoint4D_ToVector3D( &inPt, &toPt );
	}
	else
	{
		TQ3Point3D testPt;
		Q3FastRationalPoint4D_To3D( &inPt, &testPt );
		toPt = testPt - inConeAxis.origin;
	}
	return Q3Dot3D( toPt, inConeAxis.direction ) >=
		inConeCosine * Q3Length3D( toPt );
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
			solutions.maximum = Infinity();
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
		solutions.maximum = Infinity();
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
	solution.maximum = Infinity();
	
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
	solution.maximum = Infinity();
	
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
	solution.maximum = Infinity();
	
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
					quadSol.maximum = Infinity();
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
	@function	Ray3D_IntersectCone
	@abstract	Compute the intersections between a ray and a cone surface.
	@discussion	This function computes the minimum and maximum values of t
				such that inRay.origin + t * inRay.direction is in the cone.
	
				If the ray misses the cone, the function result will be false
				and the output parameters should be ignored.  if the ray origin
				is in the interior of the cone, outMinParam will be set to 0.
				It is possible that there is no largest value of t, in which
				case outMaxParam will be set to INFINITY.
				
				This function is like E3Ray3D_IntersectCone, except that the
				ray might be at infinity.

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
static bool	Ray4D_IntersectCone( const TQ3Ray4D& inRay,
							const TQ3Ray3D& inConeAxis,
							float inConeCosine,
							float& outMinParam,
							float& outMaxParam )
{
	bool didIntersect = false;
	if (fabsf(inRay.origin.w) > FLT_EPSILON)
	{
		TQ3Ray3D finiteRay;
		Q3FastRationalPoint4D_To3D( &inRay.origin, &finiteRay.origin );
		finiteRay.direction = inRay.direction;
		didIntersect = E3Ray3D_IntersectCone( finiteRay, inConeAxis, inConeCosine, outMinParam, outMaxParam );
	}
	else
	{
		// Let's check whether the start of the ray is inside the cone.
		bool	isRayOriginInCone = IsPointInCone( inRay.origin, inConeAxis, inConeCosine );
		
		// If you consider a point on the ray to be inRay.origin + t * inRay.direction,
		// and consider what happens as t increases toward infinity, we see that the "end"
		// of the ray is an infinite point whose direction is inRay.direction.  Therefore
		// the far end of the ray is in the cone just in case
		// dot( inRay.direction, inConeAxis.direction ) > inConeCosine .
		float	rayDotAxis = Q3Dot3D( inRay.direction, inConeAxis.direction );
		bool	isRayEndInCone = (rayDotAxis > inConeCosine);

		// The finite points in the cone can be described as the points x such that
		// 		dot( x - inConeAxis.origin, inConeAxis.direction ) >=
		//			inConeCosine * length( x - inConeAxis.origin ).
		// However when we consider points at infinity, x is basically just a direction,
		// and inConeAxis.origin becomes irrelevant, and we essentially have
		// 		dot( x, inConeAxis.direction ) >= inConeCosine * length( x ).
		// Replacing x by inRay.origin + t * inRay.direction, we are seeking t >= 0
		// such that
		//		dot( inRay.origin + t * inRay.direction, inConeAxis.direction ) >=
		//		inConeCosine * length( inRay.origin + t * inRay.direction ) .

		// Replacing length( inRay.origin + t * inRay.direction ) by
		// sqrt( dot( inRay.origin + t * inRay.direction, inRay.origin + t * inRay.direction ) )
		// and expanding out the dot products, this becomes
		//		rayOriginDotAxis + t * rayDotAxis >=
		//			inConeCosine * sqrt( rayOriginDotRayOrigin + 2 * t * rayOriginDotRayDir + t * t ) .
		TQ3Vector3D rayOriginDir;
		Q3FastRationalPoint4D_ToVector3D( &inRay.origin, &rayOriginDir );
		float	rayOriginLen = Q3Length3D( rayOriginDir );
		float rayOriginDotAxis = Q3Dot3D( rayOriginDir, inConeAxis.direction );
		float rayOriginDotRayOrigin = Q3Dot3D( rayOriginDir, rayOriginDir );
		float rayOriginDotRayDir = Q3Dot3D( rayOriginDir, inRay.direction );
		// Let us square both sides to get rid of the square root.  (This may
		// produce extraneous solutions such that rayOriginDotAxis + t * rayDotAxis is
		// negative.)
		//		square(rayOriginDotAxis + t * rayDotAxis) >=
		//			square(inConeCosine) * ( rayOriginDotRayOrigin + 2 * t * rayOriginDotRayDir + t * t ) .
		// This is a quadratic equation, which can be solved by the familiar
		// quadratic formula.
		float	cosSq = inConeCosine * inConeCosine;
		float	a = cosSq - rayDotAxis * rayDotAxis;
		float	b = 2.0f * (cosSq * rayOriginDotRayDir - rayOriginDotAxis * rayDotAxis);
		float	c = cosSq * rayOriginDotRayOrigin - rayOriginDotAxis * rayOriginDotAxis;
		float	t;

		if (a == 0.0f)
		{
			if (b == 0.0f)
			{
				if (c == 0.0f)
				{
					// The ray lies within the doubled cone
					if (rayDotAxis > 0.0f)
					{
						if (isRayOriginInCone)
						{
							outMinParam = 0.0f;
							outMaxParam = Infinity();
							didIntersect = true;
						}
						else
						{
							outMinParam = rayOriginLen;
							outMaxParam = Infinity();
							didIntersect = true;
						}
					}
					else
					{
						if (isRayOriginInCone)
						{
							outMinParam = 0.0f;
							outMaxParam = rayOriginLen;
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
						outMaxParam = Infinity();
						didIntersect = true;
					}
				}
				else
				{
					if (isRayEndInCone)
					{
						outMinParam = 0.0f;
						outMaxParam = Infinity();
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
				bool	isRoot1Actual = (rayOriginDotAxis + root1 * rayDotAxis >= 0.0f);
				bool	isRoot2Actual = (rayOriginDotAxis + root2 * rayDotAxis >= 0.0f);
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
						outMaxParam = Infinity();
						didIntersect = true;
					}
					else
					{
						outMinParam = 0.0f;
						outMaxParam = Infinity();
						didIntersect = true;
					}
				}
			}
		}
	}
	
	return didIntersect;
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

static void MakeOneEdgeRay(	const TQ3RationalPoint4D& inStart,
							const TQ3RationalPoint4D& inEnd,
							TQ3Ray4D& outEdgeRay,
							bool& outParamToInfinity )
{
	TQ3Point3D finiteStartPt, finiteEndPt;
	TQ3Vector3D infiniteStartDir, infiniteEndDir;
	
	if (fabsf( inStart.w ) > FLT_EPSILON) // inStart is a finite point
	{
		outEdgeRay.origin = inStart;
		if (fabsf( inEnd.w ) > FLT_EPSILON) // inEnd is a finite point
		{
			Q3RationalPoint4D_To3D( &inStart, &finiteStartPt );
			Q3RationalPoint4D_To3D( &inEnd, &finiteEndPt );
			outEdgeRay.direction = Q3Normalize3D( finiteEndPt - finiteStartPt );
			outParamToInfinity = false;
		}
		else // inEnd is an infinite point
		{
			Q3RationalPoint4D_ToVector3D( &inEnd, &infiniteEndDir );
			outEdgeRay.direction = Q3Normalize3D( infiniteEndDir );
			outParamToInfinity = true;
		}
	}
	else // inStart is an infinite point
	{
		if (fabsf( inEnd.w ) > FLT_EPSILON) // inEnd is a finite point
		{
			MakeOneEdgeRay( inEnd, inStart, outEdgeRay, outParamToInfinity );
		}
		else // inEnd is an infinite point
		{
			Q3RationalPoint4D_ToVector3D( &inStart, &infiniteStartDir );
			Q3RationalPoint4D_ToVector3D( &inEnd, &infiniteEndDir );
			outEdgeRay.origin = inStart;
			outEdgeRay.direction = Q3Normalize3D( infiniteEndDir - infiniteStartDir );
			outParamToInfinity = false;
		}
	}
}


static void GetFrustumEdgeRays( const TQ3RationalPoint4D* in8Corners,
								TQ3Ray4D* out12Edges,
								bool* out12ParamToInfinity )
{
	MakeOneEdgeRay( in8Corners[0], in8Corners[4], out12Edges[0], out12ParamToInfinity[0] );	// bottom left
	MakeOneEdgeRay( in8Corners[1], in8Corners[5], out12Edges[1], out12ParamToInfinity[1] );	// top left
	MakeOneEdgeRay( in8Corners[2], in8Corners[6], out12Edges[2], out12ParamToInfinity[2] );	// bottom right
	MakeOneEdgeRay( in8Corners[3], in8Corners[7], out12Edges[3], out12ParamToInfinity[3] );	// top right

	MakeOneEdgeRay( in8Corners[0], in8Corners[2], out12Edges[4], out12ParamToInfinity[4] );	// front bottom
	MakeOneEdgeRay( in8Corners[1], in8Corners[3], out12Edges[5], out12ParamToInfinity[5] );	// front top
	MakeOneEdgeRay( in8Corners[0], in8Corners[1], out12Edges[6], out12ParamToInfinity[6] );	// front left
	MakeOneEdgeRay( in8Corners[2], in8Corners[3], out12Edges[7], out12ParamToInfinity[7] );	// front right

	MakeOneEdgeRay( in8Corners[4], in8Corners[6], out12Edges[8], out12ParamToInfinity[8] );	// rear bottom
	MakeOneEdgeRay( in8Corners[5], in8Corners[7], out12Edges[9], out12ParamToInfinity[9] );	// rear top
	MakeOneEdgeRay( in8Corners[4], in8Corners[5], out12Edges[10], out12ParamToInfinity[10] );	// rear left
	MakeOneEdgeRay( in8Corners[6], in8Corners[7], out12Edges[11], out12ParamToInfinity[11] );	// rear right
	
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
			theDot = Infinity();
		}
		else if (theDot < 0.0f)
		{
			theDot = - Infinity();
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
			
			if ( isInside && (hitPoint != nullptr) )
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
/*!
	@function	E3Ray3D_IntersectTriangle
	@abstract	Find the intersection between a ray and a triangle.
	@discussion	If the ray is in the plane of the triangle, we report no hit
				even though there may mathematically be infinitely many points
				of intersection.
				
				If we do detect a hit, then hitPoint->u and hitPoint->v are two
				barycentric coordinates of the intersection point, and
				hitPoint->w is the distance along the ray.  To be precise,
				theRay->origin + hitPoint->w * theRay->direction ==
				(1.0 - hitPoint->u - hitPoint->v) * point1 +
				hitPoint->u * point2 + hitPoint->v * point3.
				
				If the result is true, then the computed parameters will satisfy
				u >= 0, v >= 0, u + v <= 1, and w >= 0.
	@param		theRay			A ray.
	@param		point1			A point (a vertex of a triangle).
	@param		point2			A point (a vertex of a triangle).
	@param		point3			A point (a vertex of a triangle).
	@param		cullBackfacing	Whether to omit a hit on the back face.
	@param		outHitPoint		Receives intersection data as described above.
	@result		True if we detected a hit.
*/
TQ3Boolean
E3Ray3D_IntersectTriangle(	const TQ3Ray3D&		theRay,
							const TQ3Point3D&	point1,
							const TQ3Point3D&	point2,
							const TQ3Point3D&	point3,
							TQ3Boolean			cullBackfacing,
							TQ3Param3D&			outHitPoint)
{	TQ3Vector3D		edge1, edge2, tvec, pvec, qvec;
	float			det, invDet;



	// Calculate the two edges which share vertex 1
	edge1 = point2 - point1;
	edge2 = point3 - point1;



	// Begin calculating the determinant - also used to calculate u. If the
	// determinant is near zero, the ray lies in the plane of the triangle.
	// However, some care is needed; we can get a false positive on "near zero"
	// if the triangle is small.
	pvec = Q3Cross3D( theRay.direction, edge2 );
	det = Q3Dot3D( edge1, pvec );
	float	testDet = det;
	if (fabsf( det ) < kQ3RealZero)
	{
		TQ3Vector3D	faceNorm = Q3Normalize3D( Q3Cross3D( edge2, edge1 ) );
		testDet = Q3Dot3D( faceNorm, theRay.direction );
	}


	// Handle triangles with back-face culling
	if (cullBackfacing)
	{
		// Test for ray coinciding with triangle plane, or backface hit
		if (testDet < kQ3RealZero)
			return(kQ3False);


		// Calculate the distance between vertex 1 and the ray origin
		tvec = theRay.origin - point1;


		// Calculate u, and test for a miss
		outHitPoint.u = Q3Dot3D( tvec, pvec );
		if (outHitPoint.u < 0.0f || outHitPoint.u > det)
			return(kQ3False);


		// Calculate v, and test for a miss
		qvec = Q3Cross3D( tvec, edge1 );	
		outHitPoint.v = Q3Dot3D( theRay.direction, qvec );
		if (outHitPoint.v < 0.0f || (outHitPoint.u + outHitPoint.v) > det)
			return(kQ3False);


		// Calculate w, and scale the parameters
		outHitPoint.w  = Q3Dot3D( edge2, qvec );
		invDet = 1.0f / det;

		outHitPoint.w *= invDet;
		outHitPoint.u *= invDet;
		outHitPoint.v *= invDet;
	}


	// Handle triangles with no culling
	else
	{
		// Test for ray coinciding with triangle plane
		if (testDet > -kQ3RealZero && testDet < kQ3RealZero)
			return(kQ3False);
		
		invDet = 1.0f / det;


		// Calculate the distance between vertex 1 and the ray origin
		tvec = theRay.origin - point1;


		// Calculate u, and test for a miss
		outHitPoint.u = Q3Dot3D( tvec, pvec ) * invDet;
		if (outHitPoint.u < 0.0f || outHitPoint.u > 1.0f)
			return(kQ3False);


		// Calculate v, and test for a miss
		qvec = Q3Cross3D( tvec, edge1 );
		outHitPoint.v = Q3Dot3D( theRay.direction, qvec ) * invDet;
		if (outHitPoint.v < 0.0f || (outHitPoint.u + outHitPoint.v) > 1.0f)
			return(kQ3False);
		
		
		// Calculate w
		outHitPoint.w = Q3Dot3D( edge2, qvec ) * invDet;
	}



	// The ray intersects the triangle	
	return (outHitPoint.w >= 0.0f ? kQ3True : kQ3False);
}




/*!
	@function	E3Ray3D_IntersectPlaneOfTriangle
	@abstract	Find the intersection between a ray and the plane of a triangle.
	@discussion	If the ray is in the plane of the triangle, we report no hit
				even though there may mathematically be infinitely many points
				of intersection.
				
				This function is similar to E3Ray3D_IntersectTriangle, but finds
				any intersection with the plane of the triangle.  Therefore,
				there are no constraints on the barycentric coordinates u and v,
				but we do still require that w >= 0.
				
				If we do detect a hit, then hitPoint->u and hitPoint->v are two
				barycentric coordinates of the intersection point, and
				hitPoint->w is the distance along the ray.  To be precise,
				theRay->origin + hitPoint->w * theRay->direction ==
				(1.0 - hitPoint->u - hitPoint->v) * point1 +
				hitPoint->u * point2 + hitPoint->v * point3.
	@param		theRay			A ray.
	@param		point1			A point (a vertex of a triangle).
	@param		point2			A point (a vertex of a triangle).
	@param		point3			A point (a vertex of a triangle).
	@param		cullBackfacing	Whether to omit a hit on the back face.
	@param		outHitPoint		Receives intersection data as described above.
	@result		True if we detected a hit.
*/
TQ3Boolean		E3Ray3D_IntersectPlaneOfTriangle(
											const TQ3Ray3D&		theRay,
											const TQ3Point3D&	point1,
											const TQ3Point3D&	point2,
											const TQ3Point3D&	point3,
											TQ3Boolean			cullBackfacing,
											TQ3Param3D&			outHitPoint)
{
	// Calculate the two edges which share vertex 1
	TQ3Vector3D edge1 = point2 - point1;
	TQ3Vector3D edge2 = point3 - point1;
	
	// Begin calculating the determinant - also used to calculate u. If the
	// determinant is near zero, the ray lies in the plane of the triangle.
	// However, some care is needed; we can get a false positive on "near zero"
	// if the triangle is small.
	TQ3Vector3D pvec = Q3Cross3D( theRay.direction, edge2 );
	float det = Q3Dot3D( edge1, pvec );
	float	testDet = det;
	if (fabsf( det ) < kQ3RealZero)
	{
		TQ3Vector3D	faceNorm = Q3Normalize3D( Q3Cross3D( edge2, edge1 ) );
		testDet = Q3Dot3D( faceNorm, theRay.direction );
	}
	
	// Test for misses by back-face culling or ray parallel to plane
	if (cullBackfacing)
	{
		if (testDet < kQ3RealZero)
		{
			return kQ3False;
		}
	}
	else
	{
		if (fabsf( testDet ) < kQ3RealZero)
		{
			return kQ3False;
		}
	}
	
	float invDet = 1.0f / det;
	
	// Calculate the vector between vertex 1 and the ray origin
	TQ3Vector3D tvec = theRay.origin - point1;
	
	// Calculate u
	outHitPoint.u = Q3Dot3D( tvec, pvec ) * invDet;
	
	// Calculate v
	TQ3Vector3D qvec = Q3Cross3D( tvec, edge1 );
	outHitPoint.v = Q3Dot3D( theRay.direction, qvec ) * invDet;
	
	// Calculate w
	outHitPoint.w = Q3Dot3D( edge2, qvec ) * invDet;
	
	return (outHitPoint.w >= 0.0f ? kQ3True : kQ3False);
}





/*!
	@function	E3Ray3D_NearTriangle
	@abstract	Find the nearest points of a ray and a triangle.
	@discussion	This function can return kQ3False under 2 conditions:
				(1) The triangle is edge-on to the ray, i.e.,
					Dot( frontFaceVec, theRay.direction ) = 0.
				(2) cullBackfacing == kQ3True, and the ray faces the back face
					of the triangle, i.e.,
					Dot( frontFaceVec, theRay.direction ) > 0.
				
				Otherwise, we find parameters u, v, w such that
				u >= 0,
				v >= 0;
				u + v <= 1;
				w >= 0;
				and the distance between the ray point
				theRay.origin + w * theRay.direction
				and the triangle point
				(1 - u - v) * point1 + u * point2 + v * point3
				is minimized.
	@param		theRay			A ray.
	@param		point1			A point (a vertex of a triangle).
	@param		point2			A point (a vertex of a triangle).
	@param		point3			A point (a vertex of a triangle).
	@param		cullBackfacing	Whether to omit a hit on the back face.
	@param		outHitPoint		Receives intersection data as described above.
	@result		True if we detected a hit.
*/
TQ3Boolean		E3Ray3D_NearTriangle(
											const TQ3Ray3D&		theRay,
											const TQ3Point3D&	point1,
											const TQ3Point3D&	point2,
											const TQ3Point3D&	point3,
											TQ3Boolean			cullBackfacing,
											TQ3Param3D&			outHitPoint )
{
	TQ3Param3D planeHit;
	TQ3Boolean didHit = E3Ray3D_IntersectPlaneOfTriangle( theRay,
		point1, point2, point3, cullBackfacing, planeHit );
	
	if (didHit)
	{
		// If the ray hit inside the triangle, then the work is done.
		if ( (planeHit.u >= 0.0f) && (planeHit.v >= 0.0f) &&
			(planeHit.u + planeHit.v <= 1.0f) )
		{
			outHitPoint = planeHit;
		}
		else // missed the triangle, nearest point is along an edge
		{
			float rayParam12, segParam12;
			E3Math_RayNearestLineSegment( theRay, point1, point2,
				rayParam12, segParam12 );
			TQ3Point3D rayPt12 = theRay.origin + rayParam12 * theRay.direction;
			TQ3Point3D triPt12 = (1.0f - segParam12) * point1 + segParam12 * point2;
			float lenSq12 = Q3LengthSquared3D( rayPt12 - triPt12 );
			
			float rayParam13, segParam13;
			E3Math_RayNearestLineSegment( theRay, point1, point3,
				rayParam13, segParam13 );
			TQ3Point3D rayPt13 = theRay.origin + rayParam13 * theRay.direction;
			TQ3Point3D triPt13 = (1.0f - segParam13) * point1 + segParam13 * point3;
			float lenSq13 = Q3LengthSquared3D( rayPt13 - triPt13 );
			
			float rayParam23, segParam23;
			E3Math_RayNearestLineSegment( theRay, point2, point3,
				rayParam23, segParam23 );
			TQ3Point3D rayPt23 = theRay.origin + rayParam23 * theRay.direction;
			TQ3Point3D triPt23 = (1.0f - segParam23) * point2 + segParam23 * point3;
			float lenSq23 = Q3LengthSquared3D( rayPt23 - triPt23 );
			
			if ( (lenSq12 <= lenSq13) && (lenSq12 <= lenSq23) )
			{
				outHitPoint.w = rayParam12;
				outHitPoint.u = segParam12;
				outHitPoint.v = 0.0f;
			}
			else if ( (lenSq13 <= lenSq12) && (lenSq13 <= lenSq23) )
			{
				outHitPoint.w = rayParam13;
				outHitPoint.u = 0.0f;
				outHitPoint.v = segParam13;
			}
			else // (lenSq23 <= lenSq12) && (lenSq23 <= lenSq13)
			{
				outHitPoint.w = rayParam23;
				outHitPoint.u = 1.0f - segParam23;
				outHitPoint.v = segParam23;
			}
		}
	}
	
	return didHit;
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
	
	TQ3Vector3D	diff = inRay.origin - inConeAxis.origin;
	float	diffDotAxis = Q3Dot3D( diff, inConeAxis.direction );
	float	diffLen = Q3Length3D( diff );
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
	// Letting t increase toward infinity, we see that the ray is eventually
	// inside the cone just in case
	// 		dot( inRay.direction, inConeAxis.direction ) > inConeCosine .
	float	rayDotAxis = Q3Dot3D( inRay.direction, inConeAxis.direction );
	bool	isRayEndInCone = (rayDotAxis > inConeCosine);
	
	// Replacing length( diff + t * inRay.direction ) by
	// sqrt( dot( diff + t * inRay.direction, diff + t * inRay.direction ) )
	// and expanding out the dot products, this becomes
	//		diffDotAxis + t * rayDotAxis >=
	//			inConeCosine * sqrt( diffDotDiff + 2 * t * diffDotRay + t * t ) .
	float	diffDotDiff = diffLen * diffLen;
	float	diffDotRay = Q3Dot3D( diff, inRay.direction );
	
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
						outMaxParam = Infinity();
						didIntersect = true;
					}
					else
					{
						outMinParam = diffLen;
						outMaxParam = Infinity();
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
					outMaxParam = Infinity();
					didIntersect = true;
				}
			}
			else
			{
				if (isRayEndInCone)
				{
					outMinParam = 0.0f;
					outMaxParam = Infinity();
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
					outMaxParam = Infinity();
					didIntersect = true;
				}
				else
				{
					outMinParam = 0.0f;
					outMaxParam = Infinity();
					didIntersect = true;
				}
			}
		}
	}
	
	return didIntersect;
}





/*!
	@function	E3Math_LineNearestPoint
	
	@abstract	Find the point on a line that is nearest to a given point.
	
	@discussion	If what you really want is the point on a RAY nearest a given
				point, then just clamp the result to be nonnegative.
	
	@param		inRay		A 3D ray.  The direction need not be normalized,
							but must be nonzero.
	@param		inPt		A point.
	@result		A number t such that inRay.origin + t * inRay.direction is as
				near as possible to inPt.
*/
float	E3Math_LineNearestPoint(
								const TQ3Ray3D& inRay,
								const TQ3Point3D& inPt )
{
	float t = Q3Dot3D( inPt - inRay.origin, inRay.direction ) /
		Q3Dot3D( inRay.direction, inRay.direction );
	return t;
}





/*!
	@function	E3Math_RayNearestLineSegment
	
	@abstract	Find points on a ray and a line segment that are as near as
				possible to each other.
	
	@discussion	Find outRayParam >= 0 and outSegParam in [0,1] such that the
				points inRay.origin + outRayParam * inRay.direction and
				(1.0 - outSegParam) * inPtA + outSegParam * inPtB are as near
				to each other as possible.
				
				If the ray and the segment happen to be parallel, then the
				solution is not unique, and this function just returns one of
				the solutions.
	
	@param		inRay		A 3D ray.    The direction need not be normalized,
							but must be nonzero.
	@param		inPtA		A point.
	@param		inPtB		A point.
	@param		outRayParam	Returns a parameter for the ray, always nonnegative.
	@param		outSegParam	Returns a parameter for the line segment, between
							0 and 1 inclusive.
*/
void	E3Math_RayNearestLineSegment(
								const TQ3Ray3D& inRay,
								const TQ3Point3D& inPtA,
								const TQ3Point3D& inPtB,
								float& outRayParam,
								float& outSegParam )
{
	TQ3Vector3D v = inPtB - inPtA;
	// We want to find numbers s, t to minimize the distance between
	// inRay.origin + s * inRay.direction and inPtA + t * v.  In other words,
	// we want to minimize the length of the vector
	// inRay.origin + s * inRay.direction - (inPtA + t * v).
	TQ3Vector3D g = inRay.origin - inPtA;
	// Now we want to minimize the length of the vector
	// w = g + s * inRay.direction - t * v.
	// To keep thing briefer, let's use r to stand for inRay.direction, so that
	// we want to minimize the length of w = g + s * r - t * v.
	// This will happen when w is orthogonal to both r and v.
	// That gives us two linear equations,
	// 0 = Dot( w, r ) = Dot( g, r ) + s * Dot( r, r ) - t * Dot( v, r ), and
	// 0 = Dot( w, v ) = Dot( g, v ) + s * Dot( r, v ) - t * Dot( v, v ).
	// If we let a = Dot( r, r ), b = Dot( r, v ), c = Dot( v, v ),
	// d = Dot( g, r ), and e = Dot( g, v ), then the equations are
	// s * a - t * b = -d
	// s * b - t * c = -e
	// or in matrix form
	// a -b | -d
	// b -c | -e .
	// This system has the solution
	// s = (d*c - b*e) / (b*b - a*c), t = (b*d - a*e) / (b*b - a*c) .
	// Of course, we need to avoid division by zero.  The denominator is
	// b*b - a*c = Dot( r, v ) * Dot( r, v ) - Dot( r, r ) * Dot( v, v )
	// = |r|^2 |v|^2 cos^2(angle) - |r|^2 |v|^2
	// = |r|^2 |v|^2 (cos^2(angle) - 1)
	// = - sin^2(angle) |r|^2 |v|^2.
	// This is zero when the sine of the angle is 0 (meaning that r and v have
	// the same or opposite directions) or if r or v are zero.  However we
	// assumed that r is not zero.
	float c = Q3Dot3D( v, v );
	if (c < kQ3RealZero) // i.e., inPtA and inPtB are essentially the same
	{
		outRayParam = std::max( 0.0f, E3Math_LineNearestPoint( inRay, inPtA ) );
		outSegParam = 0.0f;
	}
	else
	{
		float a = Q3Dot3D( inRay.direction, inRay.direction );
		float b = Q3Dot3D( inRay.direction, v );
		float denom = b*b - a*c;
		if (fabsf( denom ) < kQ3RealZero) // ray & segment parallel or opposite
		{
			// Test segment ends
			float rayParamA = std::max( 0.0f, E3Math_LineNearestPoint( inRay, inPtA ) );
			TQ3Point3D rayPtA = inRay.origin + rayParamA * inRay.direction;
			float rayParamB = std::max( 0.0f, E3Math_LineNearestPoint( inRay, inPtB ) );
			TQ3Point3D rayPtB = inRay.origin + rayParamB * inRay.direction;
			if ( Q3LengthSquared3D( inPtA - rayPtA ) <
				Q3LengthSquared3D( inPtB - rayPtB ) )
			{
				outRayParam = rayParamA;
				outSegParam = 0.0f;
			}
			else
			{
				outRayParam = rayParamB;
				outSegParam = 1.0f;
			}
		}
		else // general case
		{
			float d = Q3Dot3D( inRay.direction, g );
			float e = Q3Dot3D( v, g );
			outRayParam = (d*c - b*e) / denom;
			outSegParam = (b*d - a*e) / denom;
			if ( (outSegParam < 0.0f) || (outSegParam > 1.0f) )
			{
				TQ3Point3D segNearPt;
				if (outSegParam < 0.0f)
				{
					outSegParam = 0.0f;
					segNearPt = inPtA;
				}
				else
				{
					outSegParam = 1.0f;
					segNearPt = inPtB;
				}
				outRayParam = E3Math_LineNearestPoint( inRay, segNearPt );
			}
			if (outRayParam < 0.0f)
			{
				outRayParam = 0.0f;
			}
		}
	}
}




/*!
	@function	E3Cone_IntersectViewFrustum
	@abstract	Determine whether a cone intersects a view frustum.
	@param		inConeAxis		Axis ray of cone with a normalized direction,
								in world coordinates.
	@param		inConeCosine	Cosine of angle from axis ray to surface of
								cone, in interval (0, 1).
	@param		inCamera		A camera object.
	@result		True if there is an intersection.
*/
bool	E3Cone_IntersectViewFrustum(
									const TQ3Ray3D& inConeAxis,
									float inConeCosine,
									TQ3CameraObject inCamera )
{
	bool	didIntersect = false;
	
	// A nonlinear camera doesn't really have a frustum.  Err on the side
	// of caution/laziness and say they always intersect.
	if ( Q3Object_IsType( inCamera, kQ3CameraTypeFisheye) ||
		Q3Object_IsType( inCamera, kQ3CameraTypeAllSeeing) )
	{
		return true;
	}
	
	// If any of the corners of the frustum are in the cone, they intersect.
	TQ3RationalPoint4D frustumCorners[8];
	GetFrustumCornersInWorldSpace( inCamera, frustumCorners );
	int	i;
	for (i = 0; i < 8; ++i)
	{
		if (IsPointInCone( frustumCorners[i], inConeAxis, inConeCosine ))
		{
			didIntersect = true;
			break;
		}
	}
	
	// If the cone axis intersects the frustum, they intersect.
	Frustum theFrustum;
	if (! didIntersect)
	{
		GetFrustumPlanesInWorldSpace( inCamera, theFrustum );
		Interval hits = Ray3D_IntersectFrustum( inConeAxis, theFrustum );
		didIntersect = ! hits.IsEmpty();
	}
	
	// In the remaining hard case, the surface of the cone must intersect an edge.
	if (! didIntersect)
	{
		TQ3Ray4D edgeRay[12];
		bool rayParamToInf[12];
		GetFrustumEdgeRays( frustumCorners, edgeRay, rayParamToInf );
	
		for (i = 0; i < 12; ++i)
		{
			float	highParam, lowParam;
			
			if (Ray4D_IntersectCone( edgeRay[i], inConeAxis, inConeCosine, highParam, lowParam ) &&
				(rayParamToInf[i] || (lowParam < 1.0f)) )
			{
				didIntersect = true;
				break;
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
	
	// With some special kinds of camera, there may be no view frustum.
	CQ3ObjectRef theCamera( CQ3View_GetCamera( inView ) );
	if ( Q3Object_IsType( (TQ3Object _Nonnull) theCamera.get(), kQ3CameraTypeAllSeeing ) ||
		Q3Object_IsType( (TQ3Object _Nonnull) theCamera.get(), kQ3CameraTypeFisheye ) )
	{
		return true;
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
	TQ3Point3D	boxLocalCorners[8];
	E3BoundingBox_GetCorners( &inLocalBox, boxLocalCorners );
	TQ3Vector3D	frustumLocalEdges[6];
	GetFrustumEdges( frustumLocalCorners, frustumLocalEdges );
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
	
	// With some special kinds of camera, there may be no view frustum.
	if ( Q3Object_IsType( inCamera, kQ3CameraTypeAllSeeing ) ||
		Q3Object_IsType( inCamera, kQ3CameraTypeFisheye ) )
	{
		return true;
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

static float e3math_angleBetweenVectors( const TQ3Vector3D& A, const TQ3Vector3D& B )
{
	return atan2f( Q3Length3D( Q3Cross3D( A, B ) ), Q3Dot3D( A, B ) );
}


static float e3math_distanceFromPointToFisheyeViewVolume(
								const TQ3Point3D& inWorldPt,
								TQ3CameraObject inCamera )
{
	float distance = 0.0f;
	
	// Convert the point to view coordinates
	TQ3Matrix4x4 worldToView;
	( (E3Camera*) inCamera )->GetWorldToView( &worldToView );
	TQ3Point3D viewPt = inWorldPt * worldToView;
	
	// Find the angle of view of the fisheye camera.
	TQ3FisheyeCameraData data;
	( (E3FisheyeCamera*) inCamera )->GetData( &data );
	float angleOfView = Q3FisheyeCamera_CalcAngleOfView( &data.sensorSize,
		data.mappingFunction, data.croppingFormat, data.focalLength );
	float angleToCone = 0.5f * angleOfView;
	
	// Find the angle between the target point and the center of view.
	float angleToTarget = e3math_angleBetweenVectors( Q3PointToVector3D(viewPt),
		TQ3Vector3D{ 0.0f, 0.0f, -1.0f } );
	
	if (angleToTarget > angleToCone) // the target point is not viewable
	{
		if (fabsf( angleOfView - kQ3Pi ) < kQ3RealZero)
		{
			// 180 degree angle of view, so the viewable stuff is just
			// everything with a negative z in view coordinates.
			distance = viewPt.z;
		}
		else if (angleOfView < kQ3Pi)
		{
			// The viewable volume is a cone ahead of the camera.
			distance = E3Math_DistanceFromPointToCone( viewPt, TQ3Point3D{ 0.0f, 0.0f, 0.0f },
				TQ3Vector3D{ 0.0f, 0.0f, -1.0f }, angleToCone );
		}
		else // angleOfView > kQ3Pi
		{
			// The non-viewable volume is a cone behind the camera.
			distance = E3Math_DistanceFromPointToCone( viewPt, TQ3Point3D{ 0.0f, 0.0f, 0.0f },
				TQ3Vector3D{ 0.0f, 0.0f, 1.0f }, kQ3Pi - angleToCone );
		}
	}
	
	return distance;
}

/*!
	@function	E3Math_DistanceFromPointToViewFrustum
	@abstract	Compute the distance from a point to the view frustum of a
				camera.
	@param		inWorldPt	A point in world coordinates.
	@param		inCamera	A camera object.
	@result		Distance.  (If the point is inside the frustum, this is zero.)
*/
float	E3Math_DistanceFromPointToViewFrustum(
								const TQ3Point3D& inWorldPt,
								TQ3CameraObject inCamera )
{
	float distance = 0.0f;
	
	if (Q3Object_IsType( inCamera, kQ3CameraTypeAllSeeing))
	{
		// everything is inside the view volume
		return distance;
	}
	else if (Q3Object_IsType( inCamera, kQ3CameraTypeFisheye))
	{
		return e3math_distanceFromPointToFisheyeViewVolume( inWorldPt, inCamera );
	}
	
	Frustum viewFrustum;
	GetFrustumPlanesInWorldSpace( inCamera, viewFrustum );
	bool inFrustum = true;
	float planeDots[6];
	int i, j;
	for (i = 0; i < 6; ++i)
	{
		planeDots[i] = inWorldPt.x * viewFrustum.planes[i].x +
			inWorldPt.y * viewFrustum.planes[i].y +
			inWorldPt.z * viewFrustum.planes[i].z +
			viewFrustum.planes[i].w;
		if (planeDots[i] > 0.0f)
		{
			inFrustum = false;
		}
	}
	
	if (inFrustum)
	{
		distance = 0.0;
	}
	else
	{
		// The far plane of the frustum is probably at infinity, so I will
		// ignore that plane and its 4 edges and 4 corners.
		// So, the point of the frustum nearest to inWorldPt could be:
		// 1. On one of the 5 finite planes, on the inner side of the other
		//    4 finite planes, or
		// 2. At one of the 4 near corners, or
		// 3. Along one of the 8 edges.
		TQ3RationalPoint4D frustumWorldCorners[8];
		GetFrustumCornersInWorldSpace( inCamera, frustumWorldCorners );
		// NBL, NTL, NBR, NTR, FBL, FTL, FBR, FTR (N=near, F=far, B=bottom etc)
		TQ3Point3D	nearCorners[4];
		for (i = 0; i < 4; ++i)
		{
			Q3FastRationalPoint4D_To3D( &frustumWorldCorners[i], &nearCorners[i] );
		}
		TQ3Vector3D	frustumWorldEdges[6];
		GetFrustumEdges( frustumWorldCorners, frustumWorldEdges );
		// bottom left, top left, bottom right, top right, horizontal front, vertical front.
		TQ3Ray3D edgeRays[8] =
		{
			{ nearCorners[0], frustumWorldEdges[4] }, // NBL to NBR
			{ nearCorners[1], frustumWorldEdges[4] }, // NTL to NTR
			{ nearCorners[0], frustumWorldEdges[5] }, // NBL to NTL
			{ nearCorners[2], frustumWorldEdges[5] }, // NBR to NTR
			{ nearCorners[0], frustumWorldEdges[0] }, // NBL to infinity
			{ nearCorners[1], frustumWorldEdges[1] }, // NTL to infinity
			{ nearCorners[2], frustumWorldEdges[2] }, // NBR to infinity
			{ nearCorners[3], frustumWorldEdges[3] }  // NTR to infinity
		};
		// Note that for the 4 near rays, the edge is origin + t * direction
		// for t from 0 to 1, while for the 4 infinite rays, t is from 0 to infinity.
		
		float minDistSq = FLT_MAX;
		float distSq;
		
		// Case 1: minimize distance to the 5 finite faces of the frustum.
		for (i = 0; i < 6; ++i)
		{
			if (i == 1)
			{
				continue;	// the far plane, which we ignore
			}
			TQ3Vector3D P = {
				viewFrustum.planes[i].x, viewFrustum.planes[i].y, viewFrustum.planes[i].z
			};
			// Suppose we define P as above, and take planeW = viewFrustum.planes[i].w.
			// the plane consists of points X such that Dot( X, P ) + planeW = 0,
			// and the inner half-plane is X such that Dot( X, P ) + planeW <= 0.
			// Let A be short for inWorldPt.  Then
			// planeDots[i] = Dot( A, P ) + planeW.
			// In order for a point X to be a point on the plane nearest to A, the vector A - X
			// must be parallel to P, meaning there is a scalar t such that A - X = t P.
			// If we dot both sides with P, we have Dot( A, P ) - Dot( X, P ) = t Dot( P, P ).
			// But in order for X to be on the plane, Dot( X, P ) + planeW = 0, so
			// Dot( X, P ) = - planew.  Thus we obtain
			// Dot( A, P ) + planeW = t Dot( P, P ), or
			// planeDots[i] = t Dot( P, P ).  Therefore,
			// t = planeDots[i] / Dot( P, P ).  Plugging this back into A - X = t P, we have
			// A - X = (planeDots[i] / Dot( P, P )) P, so
			// X = A - (planeDots[i] / Dot( P, P )) P.
			TQ3Point3D nearestPt = inWorldPt - (planeDots[i] / Q3Dot3D( P, P )) * P;
			// Test whether this point is on the inner side of the other planes.
			bool isInner = true;
			for (j = 0; j < 6; ++j)
			{
				if ( (j != i) && (j != 1) )
				{
					float test = nearestPt.x * viewFrustum.planes[j].x +
								nearestPt.y * viewFrustum.planes[j].y +
								nearestPt.z * viewFrustum.planes[j].z +
								viewFrustum.planes[j].w;
					if (test > 0.0f)
					{
						isInner = false;
						break;
					}
				}
			}
			if (isInner)
			{
				distSq = Q3LengthSquared3D( nearestPt - inWorldPt );
				if (distSq < minDistSq)
				{
					minDistSq = distSq;
				}
			}
		}
		
		// Case 2: minimize distance to the 4 near corners.
		for (i = 0; i < 4; ++i)
		{
			distSq = Q3LengthSquared3D( nearCorners[i] - inWorldPt );
			if (distSq < minDistSq)
			{
				minDistSq = distSq;
			}
		}
		
		// Case 3: minimize distance to the 8 edges.
		// In general, to find the nearest point on a line segment or ray,
		// you'd have to consider the ends, but we have already handled the
		// 4 finite corners.
		for (i = 0; i < 8; ++i)
		{
			TQ3Point3D orig = edgeRays[i].origin;
			TQ3Vector3D dir = edgeRays[i].direction;
			float param = Q3Dot3D( inWorldPt - orig, dir ) / Q3Dot3D( dir, dir );
			if ( (param > 0.0f) && ((i >= 4) || (param < 1.0f)) )
			{
				TQ3Point3D nearPt = orig + param * dir;
				distSq = Q3LengthSquared3D( nearPt - inWorldPt );
				if (distSq < minDistSq)
				{
					minDistSq = distSq;
				}
			}
		}
		
		distance = sqrtf( minDistSq );
	}
	
	return distance;
}

static TQ3Vector3D PickVectorOrthogonalToVector( const TQ3Vector3D& inDir )
{
	float	mags[3] = { fabsf( inDir.x ), fabsf( inDir.y ), fabsf( inDir.z ) };
	float*	theMin = std::min_element( &mags[0], &mags[3] );
	long minIndex = theMin - &mags[0];
	TQ3Vector3D result;
	if (minIndex == 0) // x coord is least
	{
		result.x = 0.0f;
		result.y = inDir.z;
		result.z = - inDir.y;
	}
	else if (minIndex == 1) // y least
	{
		result.x = inDir.z;
		result.z = - inDir.x;
		result.y = 0.0f;
	}
	else // z least
	{
		result.x = inDir.y;
		result.y = - inDir.x;
		result.z = 0.0f;
	}
	return result;
}


/*!
	@function	E3Math_DistanceFromPointToCone
	@abstract	Compute the minimum distance from a point to an infinite right
				circular cone.
	@discussion	If P is any point on the cone, then the angle from the vector
				P - inConeApex to the vector inConeAxis is inAngleToSurface.
	@param		inPoint		A point.
	@param		inConeApex	Apex of the cone.
	@param		inConeAxis	Axis direction of the cone.  Must be a unit vector.
	@param		inAngleToSurface	Angle from axis to the surface.
									This should be no more than pi/2.
	@result		The distance.
*/
float	E3Math_DistanceFromPointToCone(
								const TQ3Point3D& inPoint,
								const TQ3Point3D& inConeApex,
								const TQ3Vector3D& inConeAxis,
								float inAngleToSurface )
{
	// Find vectors that, together with inConeAxis, make a right-handed orthonormal basis.
	TQ3Vector3D major = Q3Normalize3D( PickVectorOrthogonalToVector( inConeAxis ) );
	TQ3Vector3D minor = Q3Normalize3D( Q3Cross3D( inConeAxis, major ) );
	
	// Let's consider a coordinate system in which the apex of the cone is at the origin.
	TQ3Point3D pointInApexOriginSpace = Q3VectorToPoint3D( inPoint - inConeApex );
	
	// Express the point in the coordinates of this basis.
	float p0 = Q3Dot3D( major, Q3PointToVector3D(pointInApexOriginSpace) );
	float p1 = Q3Dot3D( minor, Q3PointToVector3D(pointInApexOriginSpace) );
	float p2 = Q3Dot3D( inConeAxis, Q3PointToVector3D(pointInApexOriginSpace) );
	
	// For brevity let's use theta instead of inAngleToSurface.
	// If s is a parameter for distance in the axis direction, and t is an angle of
	// rotation about the axis, then we can express points on the cone by the
	// parametric equation
	// f( s, t ) = s inConeAxis + s tan(theta) (cos(t) major + sin(t) minor) ,
	// which in our chosen coordinates with basis major, minor, inConeAxis is
	// f( s, t ) = ( s tan(theta) cos(t), s tan(theta) sin(t), s ) .
	// We want to minimize the square of the distance between our target point P and f( s, t ).
	// This is dot( P - f( s, t ), P - f( s, t ) ) =
	// dot(P, P) - 2 dot( P, f( s, t ) ) + dot( f( s, t ), f( s, t ) ) .
	// We may forget the constant term dot(P, P).
	// The last term is dot( f( s, t ), f( s, t ) ) =
	// s^2 tan^2(theta) cos^2(t) + s^2 tan^2(theta) sin^2(t) + s^2
	// = s^2 ( tan^2(theta) + 1 ) = s^2 sec^2(theta).  Thus we want to minimize
	// g( s, t ) = -2 p0 s tan(theta) cos(t) - 2 p1 s tan(theta) sin(t) - 2 p2 s + s^2 sec^2(theta) .
	
	// The partial derivative of g with respect to t is
	// 2 p0 s tan(theta) sin(t) - 2 p1 s tan(theta) cos(t)
	// which we set to 0.  The 2, the s, and the tan(theta) go away, leaving
	// p0 sin(t) - p1 cos(t) = 0, hence tan(t) = p1/p0.  Algebraically there may be
	// 2 solutions, but looking at it geometrically I think we want t = atan2( p1, p0 ) .
	float t = atan2f( p1, p0 );
	
	// Now let's take the partial derivative of g with respect to s and set it to 0.
	// -2 p0 tan(theta) cos(t) - 2 p1 tan(theta) sin(t) - 2 p2 + 2 s sec^2(theta) = 0
	// s sec^2(theta) = p0 tan(theta) cos(t) + p1 tan(theta) sin(t) + p2
	// s = cos^2(theta) (p0 tan(theta) cos(t) + p1 tan(theta) sin(t) + p2)
	// We'll have to ensure that s >= 0, and consider the apex as a possible nearest point.
	float cosTheta = cosf( inAngleToSurface );
	float tanTheta = tanf( inAngleToSurface );
	float cosT = cosf( t );
	float sinT = sinf( t );
	float cosSq = cosTheta * cosTheta;
	float c = cosSq * tanTheta;
	float s = p0 * c * cosT + p1 * c * sinT + cosSq * p2;
	if (s < 0.0f)
	{
		s = 0.0f;
	}
	
	TQ3Point3D nearPt = inConeApex + s * inConeAxis + s * tanTheta * ( cosT * major + sinT * minor );
	float distance = Q3Length3D( nearPt - inPoint );
	return distance;
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
	
	// With some special kinds of camera, there may be no view frustum.
	CQ3ObjectRef theCamera( CQ3View_GetCamera( inView ) );
	if ( Q3Object_IsType( (TQ3Object _Nonnull) theCamera.get(), kQ3CameraTypeAllSeeing ) ||
		Q3Object_IsType( (TQ3Object _Nonnull) theCamera.get(), kQ3CameraTypeFisheye ) )
	{
		return true;
	}

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

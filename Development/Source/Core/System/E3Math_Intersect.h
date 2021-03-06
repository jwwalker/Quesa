/*  NAME:
        E3Math_Intersect.h

    DESCRIPTION:
        Header file for E3Math_Intersect.cpp.

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
#ifndef E3MATH_INTERSECT_HDR
#define E3MATH_INTERSECT_HDR

TQ3Boolean		E3Ray3D_IntersectSphere(const TQ3Ray3D *theRay,
										const TQ3Sphere *theSphere,
										TQ3Point3D *hitPoint);

TQ3Boolean		E3Ray3D_IntersectBoundingBox(const TQ3Ray3D *theRay,
										const TQ3BoundingBox *theBounds,
										TQ3Point3D *hitPoint);

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
TQ3Boolean		E3Ray3D_IntersectTriangle(	const TQ3Ray3D&		theRay,
											const TQ3Point3D&	point1,
											const TQ3Point3D&	point2,
											const TQ3Point3D&	point3,
											TQ3Boolean			cullBackfacing,
											TQ3Param3D&			outHitPoint);


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
											TQ3Param3D&			outHitPoint );


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
											TQ3Param3D&			outHitPoint );


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
							float& outMaxParam );


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
								const TQ3Point3D& inPt );


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
								float& outSegParam );


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
								TQ3CameraObject inCamera );


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
								float inAngleToSurface );

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
									TQ3CameraObject inCamera );


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
									const TQ3BoundingBox& inBox );


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
									const TQ3BoundingBox& inLocalBox );


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
									const TQ3BoundingBox& inWorldBox );

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
									TQ3RationalPoint4D* out6Planes );


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
									const TQ3RationalPoint4D& inWorldLightPos );


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
								TQ3BoundingBox& outSect );

#endif

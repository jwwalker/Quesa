/*  NAME:
        E3Math_Intersect.h

    DESCRIPTION:
        Header file for E3Math_Intersect.cpp.

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
#ifndef E3MATH_INTERSECT_HDR
#define E3MATH_INTERSECT_HDR

TQ3Boolean		E3Ray3D_IntersectSphere(const TQ3Ray3D *theRay,
										const TQ3Sphere *theSphere,
										TQ3Point3D *hitPoint);

TQ3Boolean		E3Ray3D_IntersectBoundingBox(const TQ3Ray3D *theRay,
										const TQ3BoundingBox *theBounds,
										TQ3Point3D *hitPoint);

TQ3Boolean		E3Ray3D_IntersectTriangle(const TQ3Ray3D		*theRay,
													const TQ3Point3D	*point1,
													const TQ3Point3D	*point2,
													const TQ3Point3D	*point3,
													TQ3Boolean			cullBackfacing,
													TQ3Param3D			*hitPoint);



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

#endif

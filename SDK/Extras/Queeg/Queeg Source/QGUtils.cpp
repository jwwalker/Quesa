/*  NAME:
        QGUtils.cpp
        
    DESCRIPTION:
		General 3D utilities to be used with (or without) the QUEEG game classes
		
		
	OTHER NOTES:	

		for triangle/triangle intersection tests, see:
		http://www.ce.chalmers.se/staff/tomasm/code/opttritri.txt

		

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

#include "QGUtils.h"
#include "QuesaMath.h"
#include "Math.h"


//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------

const float kQGAngleSlop	= 0.0001f;







//=============================================================================
//		QGLineIntersectsFacet : determine whether or not the line p1,p2 intersects
//								the 3 vertex facet bounded by pa,pb,pc
//								Return true/false and the intersection point outpoint
//	To-Do: don't assume that outpoint is given; stuff it only if not null.
//-----------------------------------------------------------------------------

//	The equation of the line is outpoint = p1 + mu (p2 - p1)
//	The equation of the plane is a x + b y + c z + d = 0
//								 n.x x + n.y y + n.z z + d = 0

//	This code is adapted from code by Paul Bourke, downloaded from:
//  http://www.swin.edu.au/astronomy/pbourke/geometry/linefacet/


TQ3Boolean QG_LineIntersectsFacet(const TQ3Point3D& p1,const TQ3Point3D& p2,
									 const TQ3Point3D& pa, const TQ3Point3D& pb, const TQ3Point3D& pc,//facet
									 TQ3Point3D *outpoint) // intersection point
{

	float d;
	float a1,a2,a3;
	float totalAngles,denom,mu;
	TQ3Vector3D	n,pa1,pa2,pa3;


	// Calculate the parameters for the plane 
	n.x = (pb.y - pa.y)*(pc.z - pa.z) - (pb.z - pa.z)*(pc.y - pa.y);
	n.y = (pb.z - pa.z)*(pc.x - pa.x) - (pb.x - pa.x)*(pc.z - pa.z);
	n.z = (pb.x - pa.x)*(pc.y - pa.y) - (pb.y - pa.y)*(pc.x - pa.x);
	Q3Vector3D_Normalize(&n, &n);
	
	d = - n.x * pa.x - n.y * pa.y - n.z * pa.z;

	// Calculate the position on the line that intersects the plane 
	denom = n.x * (p2.x - p1.x) + n.y * (p2.y - p1.y) + n.z * (p2.z - p1.z);
	if (abs(denom) < kQ3RealZero)			
		return(kQ3False);		// Line and plane don't intersect 
		

		
	mu = - (d + n.x * p1.x + n.y * p1.y + n.z * p1.z) / denom;
	outpoint->x = p1.x + mu * (p2.x - p1.x);
	outpoint->y = p1.y + mu * (p2.y - p1.y);
	outpoint->z = p1.z + mu * (p2.z - p1.z);
	if (mu < 0.0f || mu > 1.0f)	
		return(kQ3False);		// Intersection not along line segment 



	// Determine whether or not the intersection point is bounded by pa,pb,pc 
	pa1.x = pa.x - outpoint->x;
	pa1.y = pa.y - outpoint->y;
	pa1.z = pa.z - outpoint->z;
	Q3Vector3D_Normalize(&pa1, &pa1);
	
	
	pa2.x = pb.x - outpoint->x;
	pa2.y = pb.y - outpoint->y;
	pa2.z = pb.z - outpoint->z;
	Q3Vector3D_Normalize(&pa2, &pa2);

	
	pa3.x = pc.x - outpoint->x;
	pa3.y = pc.y - outpoint->y;
	pa3.z = pc.z - outpoint->z;
	Q3Vector3D_Normalize(&pa3, &pa3);
	
	a1 = pa1.x*pa2.x + pa1.y*pa2.y + pa1.z*pa2.z;
	a2 = pa2.x*pa3.x + pa2.y*pa3.y + pa2.z*pa3.z;
	a3 = pa3.x*pa1.x + pa3.y*pa1.y + pa3.z*pa1.z;
	
	totalAngles = Q3Math_RadiansToDegrees(acos(a1) + acos(a2) + acos(a3));
	totalAngles = fabs(totalAngles - 360.0f);

	if (totalAngles >  (kQ3RealZero + kQGAngleSlop) ) 
		return(kQ3False);

	if (isnan(totalAngles))
 		return (kQ3False);

	return(kQ3True);
}


//=============================================================================
//		QGLineIntersectsBox : determine whether or not the line segment
//			p1,p2 intersects the given bounding box.
//			Return true/false, and the intersection point 'outpoint'.
//-----------------------------------------------------------------------------

TQ3Boolean QG_LineIntersectsBox(const TQ3Point3D& p1,const TQ3Point3D& p2, // line segment
									const TQ3BoundingBox& bbox,		// axis-aligned bounding box
									TQ3Point3D *outpoint)
{
	//	The equation of the line is p = p1 + u (p2 - p1)
	//	The equation of a plane is a x + b y + c z + d = 0

	//	We check 3 sides of the box for intersection, taking advantage
	//	of the fact that the box is axis-aligned.
	if (bbox.isEmpty) return kQ3False;
	float x,y,z,u;
	TQ3Boolean hit = kQ3False;
	
	//	--------------- Front/Back (Z) ---------------
	// compute the intersection point,
	// and see if it's both within the line segment,
	// and within the bounding square on this plane
	u = (bbox.min.z - p1.z) / (p2.z - p1.z);
	if (u >= 0.0f and u <= 1.0f) {
		x = p1.x + u * (p2.x - p1.x);
		if (x >= bbox.min.x and x <= bbox.max.x) {
			y = p1.y + u * (p2.y - p1.y);
			if (y >= bbox.min.y and y <= bbox.max.y) goto hit;
		}
	}
	u = (bbox.max.z - p1.z) / (p2.z - p1.z);
	if (u >= 0.0f and u <= 1.0f) {
		x = p1.x + u * (p2.x - p1.x);
		if (x >= bbox.min.x and x <= bbox.max.x) {
			y = p1.y + u * (p2.y - p1.y);
			if (y >= bbox.min.y and y <= bbox.max.y) goto hit;
		}
	}
		

	//	--------------- Left/Right (X) ---------------
	u = (bbox.min.x - p1.x) / (p2.x - p1.x);
	if (u >= 0.0f and u <= 1.0f) {
		z = p1.z + u * (p2.z - p1.z);
		if (z >= bbox.min.z and z <= bbox.max.z) {
			y = p1.y + u * (p2.y - p1.y);
			if (y >= bbox.min.y and y <= bbox.max.y) goto hit;
		}
	}
	u = (bbox.max.x - p1.x) / (p2.x - p1.x);
	if (u >= 0.0f and u <= 1.0f) {
		z = p1.z + u * (p2.z - p1.z);
		if (z >= bbox.min.z and z <= bbox.max.z) {
			y = p1.y + u * (p2.y - p1.y);
			if (y >= bbox.min.y and y <= bbox.max.y) goto hit;
		}
	}
		
	
	//	--------------- Top/Bottom (Y) ---------------
	u = (bbox.min.y - p1.y) / (p2.y - p1.y);
	if (u >= 0.0f and u <= 1.0f) {
		x = p1.x + u * (p2.x - p1.x);
		if (x >= bbox.min.x and x <= bbox.max.x) {
			z = p1.z + u * (p2.z - p1.z);
			if (z >= bbox.min.z and z <= bbox.max.z) goto hit;
		}
	}
	u = (bbox.max.y - p1.y) / (p2.y - p1.y);
	if (u >= 0.0f and u <= 1.0f) {
		x = p1.x + u * (p2.x - p1.x);
		if (x >= bbox.min.x and x <= bbox.max.x) {
			z = p1.z + u * (p2.z - p1.z);
			if (z >= bbox.min.z and z <= bbox.max.z) goto hit;
		}
	}
		
	return kQ3False;

hit:
	// It's a hit!
	if (outpoint) {
		outpoint->x = x;
		outpoint->y = y;
		outpoint->z = z;
	}
	return kQ3True;
}

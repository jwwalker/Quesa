/*  NAME:
        E3GeometryEllipsoid.c

    DESCRIPTION:
        Implementation of Quesa Ellipsoid geometry class.

    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

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
#include "E3View.h"
#include "E3Geometry.h"
#include "E3GeometryTriMesh.h"
#include "E3GeometryEllipsoid.h"
#include "CQ3ObjectRef.h"

#include <vector>





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Ellipsoid : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypeEllipsoid, E3Ellipsoid, E3Geometry )
public :

	TQ3EllipsoidData			instanceData ;

	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_ellipsoid_copydata : Copy TQ3EllipsoidData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_ellipsoid_copydata(const TQ3EllipsoidData *src, TQ3EllipsoidData *dst, TQ3Boolean isDuplicate)
{
	TQ3Status		qd3dStatus = kQ3Success;

	// copy raw data
	const TQ3Uns32 theSize = sizeof(TQ3Point3D)	// origin
		+ 3*sizeof(TQ3Vector3D)		// orientation, major & minor axes
		+ 4*sizeof(float)			// u/v min/max
		+ sizeof(TQ3EndCap);		// endcaps
	Q3Memory_Copy(src, dst, theSize );

	// copy or shared-replace the attributes
	if (isDuplicate)
	{
		if (src->interiorAttributeSet != NULL)
		{
			dst->interiorAttributeSet = Q3Object_Duplicate(src->interiorAttributeSet);
			if (dst->interiorAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->interiorAttributeSet = NULL;

		if (src->ellipsoidAttributeSet != NULL)
		{
			dst->ellipsoidAttributeSet = Q3Object_Duplicate(src->ellipsoidAttributeSet);
			if (dst->ellipsoidAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->ellipsoidAttributeSet = NULL;
	}
	else {
		E3Shared_Replace(&dst->interiorAttributeSet, src->interiorAttributeSet);
		E3Shared_Replace(&dst->ellipsoidAttributeSet, src->ellipsoidAttributeSet);
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_ellipsoid_disposedata : Dispose of a TQ3EllipsoidData.
//-----------------------------------------------------------------------------
static void
e3geom_ellipsoid_disposedata(TQ3EllipsoidData *theEllipsoid)
{
	Q3Object_CleanDispose(&theEllipsoid->interiorAttributeSet );
	Q3Object_CleanDispose(&theEllipsoid->ellipsoidAttributeSet );
}	





//=============================================================================
//      e3geom_ellipsoid_new : Ellipsoid new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_ellipsoid_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3EllipsoidData			*instanceData = (TQ3EllipsoidData *)       privateData;
	const TQ3EllipsoidData	*ellipsoidData     = (const TQ3EllipsoidData *) paramData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)

	qd3dStatus = e3geom_ellipsoid_copydata(ellipsoidData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_ellipsoid_delete : Ellipsoid delete method.
//-----------------------------------------------------------------------------
static void
e3geom_ellipsoid_delete(TQ3Object theObject, void *privateData)
{	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) privateData;
#pragma unused(theObject)

	// Dispose of our instance data
	e3geom_ellipsoid_disposedata(instanceData);
}





//=============================================================================
//      e3geom_ellipsoid_duplicate : Ellipsoid duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_ellipsoid_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TQ3EllipsoidData	*fromInstanceData = (const TQ3EllipsoidData *) fromPrivateData;
	TQ3EllipsoidData			*toInstanceData   = (TQ3EllipsoidData *)       toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_ellipsoid_copydata( fromInstanceData, toInstanceData, kQ3True );

	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_ellipsoid_disposedata(toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_ellipsoid_create_disk_cap : Create a top or bottom cap
//-----------------------------------------------------------------------------
static void
e3geom_ellipsoid_create_disk_cap(
							const TQ3Point3D& origin,
							const TQ3Vector3D& major,
							const TQ3Vector3D& minor,
							bool isRightHanded,
							float uMin, float uMax,
							TQ3Uns32 uSegments,
							std::vector<TQ3Point3D>& points,
							std::vector<TQ3Vector3D>& vertNormals,
							std::vector<TQ3Param2D>& uvs,
							std::vector<TQ3TriMeshTriangleData>& triangles,
							std::vector<TQ3Vector3D>& faceNormals )
{
	// The face and vertex normal is major x minor if right handed.
	TQ3Vector3D	theNormal;
	Q3FastVector3D_Cross( &major, &minor, &theNormal );
	Q3FastVector3D_Normalize( &theNormal, &theNormal );
	if (! isRightHanded)
	{
		Q3FastVector3D_Negate( &theNormal, &theNormal );
	}
	
	TQ3Uns32	prevPoints = points.size();
	TQ3Uns32	prevFaces = triangles.size();
	
	TQ3Uns32	numPoints = uSegments + 2;
	TQ3Uns32	numFaces = uSegments;
	
	points.resize( prevPoints + numPoints );
	vertNormals.resize( prevPoints + numPoints );
	uvs.resize( prevPoints + numPoints );
	triangles.resize( prevFaces + numFaces );
	faceNormals.resize( prevFaces + numFaces );
	
	const float uDiff = uMax - uMin;
	const float	uDelta = uDiff / uSegments;
	const float uDeltaAngle = kQ32Pi * uDelta;
	
	// The origin is the point with index prevPoints.
	points[ prevPoints ] = origin;
	vertNormals[ prevPoints ] = theNormal;
	uvs[ prevPoints ].u = uvs[ prevPoints ].v = 0.5f;
	
	float	uAngle;
	TQ3Uns32	u;
	TQ3Uns32	pNum = prevPoints + 1;
	TQ3Uns32	fNum = prevFaces;
	TQ3Point3D	thePoint;
	TQ3Vector3D	vec;
	
	for (u = 0, uAngle = uMin * kQ32Pi; u <= uSegments; ++u, uAngle += uDeltaAngle)
	{
		float	cosUAngle = cosf(uAngle);
		float	sinUAngle = sinf(uAngle);
		Q3FastVector3D_Scale( &major, cosUAngle, &vec );
		Q3FastPoint3D_Vector3D_Add( &origin, &vec, &thePoint );
		Q3FastVector3D_Scale( &minor, sinUAngle, &vec );
		Q3FastPoint3D_Vector3D_Add( &thePoint, &vec, &thePoint );
		points[ pNum ] = thePoint;
		vertNormals[ pNum ] = theNormal;
		uvs[ pNum ].u = 0.5f * (cosUAngle + 1.0f);
		uvs[ pNum ].v = 0.5f * (sinUAngle + 1.0f);
		
		if (u > 0)
		{
			// Handle triangle with this point, the previous one, and the origin.
			TQ3TriMeshTriangleData	theFace;
			if (isRightHanded)
			{
				theFace.pointIndices[0] = prevPoints;
				theFace.pointIndices[1] = pNum - 1;
				theFace.pointIndices[2] = pNum;
			}
			else
			{
				theFace.pointIndices[0] = prevPoints;
				theFace.pointIndices[1] = pNum;
				theFace.pointIndices[2] = pNum - 1;
			}
			triangles[ fNum ] = theFace;
			faceNormals[ fNum ] = theNormal;
			++fNum;
		}
		
		++pNum;
	}

	
	Q3_ASSERT( pNum == prevPoints + numPoints );
	Q3_ASSERT( fNum == prevFaces + numFaces );
}





//=============================================================================
//      e3geom_ellipsoid_create_interior_cap : Create one face of interior cap
//-----------------------------------------------------------------------------
static void
e3geom_ellipsoid_create_interior_cap(
							const TQ3Point3D& origin,
							const TQ3Vector3D& orientation,
							const TQ3Vector3D& radius,
							float vMin, float vMax,
							TQ3Uns32 vSegments,
							bool isUMin,
							std::vector<TQ3Point3D>& points,
							std::vector<TQ3Vector3D>& vertNormals,
							std::vector<TQ3Param2D>& uvs,
							std::vector<TQ3TriMeshTriangleData>& triangles,
							std::vector<TQ3Vector3D>& faceNormals )
{
	// The face and vertex normal is radius x orientation if this is the uMin case.
	TQ3Vector3D	theNormal;
	Q3FastVector3D_Cross( &radius, &orientation, &theNormal );
	Q3FastVector3D_Normalize( &theNormal, &theNormal );
	if (! isUMin)
	{
		Q3FastVector3D_Negate( &theNormal, &theNormal );
	}
	
	TQ3Uns32	prevPoints = points.size();
	TQ3Uns32	prevFaces = triangles.size();
	
	bool isSouthCut = (vMin > kQ3RealZero);
	bool isNorthCut = (vMax < 1.0f - kQ3RealZero);
	
	TQ3Uns32	numPoints = vSegments + 2;
	TQ3Uns32	numFaces = vSegments;
	if (isSouthCut)
	{
		numPoints += 1;
		numFaces += 1;
	}
	if (isNorthCut)
	{
		numPoints += 1;
		numFaces += 1;
	}

	points.resize( prevPoints + numPoints );
	vertNormals.resize( prevPoints + numPoints );
	uvs.resize( prevPoints + numPoints );
	triangles.resize( prevFaces + numFaces );
	faceNormals.resize( prevFaces + numFaces );
	
	TQ3Uns32	pNum = prevPoints;
	TQ3Uns32	fNum = prevFaces;

	// The origin is the point with index prevPoints.
	points[ pNum ] = origin;
	vertNormals[ pNum ] = theNormal;
	uvs[ pNum ].u = uvs[ pNum ].v = 0.5f;
	++pNum;
	
	// If the north pole is cut off, we have an extra point between the north
	// pole and the origin.
	TQ3Point3D	thePoint;
	TQ3Vector3D	vec;
	TQ3Uns32	northExtra = 0, southExtra = 0;
	if (isNorthCut)
	{
		Q3FastVector3D_Scale( &orientation, -cosf( kQ3Pi * vMax ), &vec );
		Q3FastPoint3D_Vector3D_Add( &origin, &vec, &thePoint );
		northExtra = pNum;
		points[ pNum ] = thePoint;
		vertNormals[ pNum ] = theNormal;
		uvs[ pNum ].u = 0.5f;
		uvs[ pNum ].v = 0.5f * (1.0f - cosf( kQ3Pi * vMax ));
		++pNum;
	}
	
	if (isSouthCut)
	{
		Q3FastVector3D_Scale( &orientation, -cosf( kQ3Pi * vMin ), &vec );
		Q3FastPoint3D_Vector3D_Add( &origin, &vec, &thePoint );
		southExtra = pNum;
		points[ pNum ] = thePoint;
		vertNormals[ pNum ] = theNormal;
		uvs[ pNum ].u = 0.5f;
		uvs[ pNum ].v = 0.5f * (1.0f - cosf( kQ3Pi * vMin ));
		++pNum;
	}
	
	const float vDiff = vMax - vMin;
	const float	vDelta = vDiff / vSegments;
	const float vDeltaAngle = kQ3Pi * vDelta;
	
	float		vAngle;
	TQ3Uns32	v;
	TQ3TriMeshTriangleData	theFace;
	theFace.pointIndices[0] = prevPoints;

	for (v = 0, vAngle = vMin * kQ3Pi; v <= vSegments; ++v, vAngle += vDeltaAngle)
	{
		float	cosVAngle = cosf(vAngle);
		float	sinVAngle = sinf(vAngle);
		Q3FastVector3D_Scale( &orientation, -cosVAngle, &vec );
		Q3FastPoint3D_Vector3D_Add( &origin, &vec, &thePoint );
		Q3FastVector3D_Scale( &radius, sinVAngle, &vec );
		Q3FastPoint3D_Vector3D_Add( &thePoint, &vec, &thePoint );
		points[ pNum ] = thePoint;
		vertNormals[ pNum ] = theNormal;
		TQ3Param2D	theUV;
		if (isUMin)
		{
			theUV.u = 0.5f * (sinVAngle + 1.0f);
		}
		else
		{
			theUV.u = 0.5f * (-sinVAngle + 1.0f);
		}
		theUV.v = 0.5f * (1.0f - cosVAngle);
		uvs[ pNum ] = theUV;
		
		if (v > 0)
		{
			if (isUMin)
			{
				theFace.pointIndices[1] = pNum - 1;
				theFace.pointIndices[2] = pNum;
			}
			else
			{
				theFace.pointIndices[1] = pNum;
				theFace.pointIndices[2] = pNum - 1;
			}
			triangles[ fNum ] = theFace;
			faceNormals[ fNum ] = theNormal;
			++fNum;
		}
		++pNum;
	}
	
	if (isNorthCut)
	{
		if (isUMin)
		{
			theFace.pointIndices[1] = pNum - 1;
			theFace.pointIndices[2] = northExtra;
		}
		else
		{
			theFace.pointIndices[1] = northExtra;
			theFace.pointIndices[2] = pNum - 1;
		}
		triangles[ fNum ] = theFace;
		faceNormals[ fNum ] = theNormal;
		++fNum;
	}
	
	if (isSouthCut)
	{
		if (isUMin)
		{
			theFace.pointIndices[1] = southExtra;
			theFace.pointIndices[2] = southExtra + 1;
		}
		else
		{
			theFace.pointIndices[1] = southExtra + 1;
			theFace.pointIndices[2] = southExtra;
		}
		triangles[ fNum ] = theFace;
		faceNormals[ fNum ] = theNormal;
		++fNum;
	}
	
	Q3_ASSERT( pNum == prevPoints + numPoints );
	Q3_ASSERT( fNum == prevFaces + numFaces );
}





//=============================================================================
//      e3geom_ellipsoid_create_caps : Create caps
//-----------------------------------------------------------------------------
static CQ3ObjectRef
e3geom_ellipsoid_create_caps(
							const TQ3EllipsoidData& geomData,
							float uMin, float uMax,
							float vMin, float vMax,
							TQ3Uns32 uSegments,
							TQ3Uns32 vSegments,
							bool isTopCapNeeded,
							bool isBottomCapNeeded,
							bool isInteriorCapNeeded )
{
	// Test whether the system is right-handed.
	TQ3Vector3D	majxMin;
	Q3FastVector3D_Cross( &geomData.majorRadius, &geomData.minorRadius, &majxMin );
	bool	isRightHanded = (Q3FastVector3D_Dot( &majxMin, &geomData.orientation ) > 0.0f);

	// We will put all 3 caps in the same TriMesh.
	std::vector<TQ3Point3D>					points;
	std::vector<TQ3Vector3D>				vertNormals;
	std::vector<TQ3Param2D>					uvs;
	std::vector<TQ3TriMeshTriangleData>		triangles;
	std::vector<TQ3Vector3D>				faceNormals;
	
	TQ3Vector3D	major, minor, vec, radius;
	TQ3Point3D	origin;
	
	if (isTopCapNeeded)
	{
		Q3FastVector3D_Scale( &geomData.orientation, -cosf(kQ3Pi * vMax), &vec );
		Q3FastPoint3D_Vector3D_Add( &geomData.origin, &vec, &origin );
		
		float	sinVMax = sinf( kQ3Pi * vMax );
		Q3FastVector3D_Scale( &geomData.majorRadius, sinVMax, &major );
		Q3FastVector3D_Scale( &geomData.minorRadius, sinVMax, &minor );
		
		e3geom_ellipsoid_create_disk_cap( origin, major, minor, isRightHanded,
			uMin, uMax, uSegments,
			points, vertNormals, uvs, triangles, faceNormals );
	}
	
	if (isBottomCapNeeded)
	{
		Q3FastVector3D_Scale( &geomData.orientation, -cosf(kQ3Pi * vMin), &vec );
		Q3FastPoint3D_Vector3D_Add( &geomData.origin, &vec, &origin );
		
		float	sinVMin = sinf( kQ3Pi * vMin );
		Q3FastVector3D_Scale( &geomData.majorRadius, sinVMin, &major );
		Q3FastVector3D_Scale( &geomData.minorRadius, sinVMin, &minor );
		
		e3geom_ellipsoid_create_disk_cap( origin, major, minor, ! isRightHanded,
			uMin, uMax, uSegments,
			points, vertNormals, uvs, triangles, faceNormals );
	}
	
	if (isInteriorCapNeeded)
	{
		// uMin interior part
		Q3FastVector3D_Scale( &geomData.majorRadius, cosf( kQ32Pi * uMin ), &radius );
		Q3FastVector3D_Scale( &geomData.minorRadius, sinf( kQ32Pi * uMin ), &vec );
		Q3FastVector3D_Add( &radius, &vec, &radius );
		
		e3geom_ellipsoid_create_interior_cap( geomData.origin, geomData.orientation, radius,
			vMin, vMax, vSegments, true,
			points, vertNormals, uvs, triangles, faceNormals );

		// uMax interior part
		Q3FastVector3D_Scale( &geomData.majorRadius, cosf( kQ32Pi * uMax ), &radius );
		Q3FastVector3D_Scale( &geomData.minorRadius, sinf( kQ32Pi * uMax ), &vec );
		Q3FastVector3D_Add( &radius, &vec, &radius );
		
		e3geom_ellipsoid_create_interior_cap( geomData.origin, geomData.orientation, radius,
			vMin, vMax, vSegments, false,
			points, vertNormals, uvs, triangles, faceNormals );
	}
	
	TQ3TriMeshAttributeData	vertAtts[] =
	{
		{ kQ3AttributeTypeNormal, &vertNormals[0], NULL },
		{ kQ3AttributeTypeSurfaceUV, &uvs[0], NULL }
	};
	TQ3TriMeshAttributeData faceAtts[] =
	{
		{ kQ3AttributeTypeNormal, &faceNormals[0], NULL },
	};
	TQ3TriMeshData	tmData =
	{
		geomData.interiorAttributeSet,			// triMeshAttributeSet
		triangles.size(),						// numTriangles
		&triangles[0],							// triangles
		sizeof(faceAtts)/sizeof(faceAtts[0]),	// numTriangleAttributeTypes
		faceAtts,								// triangleAttributeTypes
		0,										// numEdges
		NULL,									// edges
		0,										// numEdgeAttributeTypes,
		NULL,									// edgeAttributeTypes
		points.size(),							// numPoints
		&points[0],								// points
		sizeof(vertAtts)/sizeof(vertAtts[0]),	// numVertexAttributeTypes
		vertAtts								// vertexAttributeTypes
	};
	Q3BoundingBox_SetFromPoints3D( &tmData.bBox,
									tmData.points,
									tmData.numPoints,
									sizeof(TQ3Point3D) );
	
	
	return  CQ3ObjectRef( Q3TriMesh_New( &tmData ) );
}





//=============================================================================
//      e3geom_ellipsoid_create_face : Create main surface
//-----------------------------------------------------------------------------
static CQ3ObjectRef
e3geom_ellipsoid_create_face( const TQ3EllipsoidData& geomData,
							float uMin, float uMax,
							float vMin, float vMax,
							bool isNorthPresent,
							bool isSouthPresent,
							TQ3Uns32 uSegments,
							TQ3Uns32 vSegments )
{
	TQ3TriMeshData				triMeshData;
	TQ3Uns32 u, v, uMaxIndex;
	float uang=0.0f, vang;
	TQ3Vector3D vec, vec2, axis;	// (just temporaries used for intermediate results)
	TQ3Uns32 pnum = 0, tnum = 0;
	TQ3Vector3D		majXOrient, minXOrient, majXMinor;
	float			sinUAngle, cosUAngle, sinVAngle, cosVAngle;


	
	// In order to have proper texture coordinates, we will use extra points
	// along the longitudinal seam and at the poles.
	// 
	// We'll construct the ellipsoid out of vSegments - 1 latitude circles, each
	// containing uSegments + 1 points at equally spaced longitudes (the first and
	// last of these being at the same physical location), plus north and
	// south pole points.

	TQ3Uns32	numPoints = (uSegments + 1) * (vSegments + 1);
	TQ3Uns32	numTriangles = uSegments * vSegments * 2;
	if (isNorthPresent)
	{
		// The north strip needs only one triangle, and one north polar point,
		// for each u segment.
		numTriangles -= uSegments;
		numPoints -= 1;
	}
	if (isSouthPresent)
	{
		// The south strip needs only one triangle, and one south polar point,
		// for each u segment.
		numTriangles -= uSegments;
		numPoints -= 1;
	}



	// Allocate some memory for the TriMesh
	std::vector<TQ3Point3D>					points( numPoints );
	std::vector<TQ3Vector3D>				normals( numPoints );
	std::vector<TQ3Param2D>					uvs( numPoints );
	std::vector<TQ3TriMeshTriangleData>		triangles( numTriangles );
	std::vector<TQ3Vector3D>				faceNormals( numTriangles );



	// Get the UV ranges
	const float uDiff = uMax - uMin;
	const float vDiff = vMax - vMin;
	const float	uDelta = uDiff / uSegments;
	const float vDelta = vDiff / vSegments;



	const float uDeltaAngle = kQ32Pi * uDelta;
	const float vDeltaAngle = kQ3Pi * vDelta;

	// Normal vector computations:
	// The ellipsoid has a parametric equation
	// f(u,v) = origin - cos(v)orientation + sin(v)(cos(u)majorRadius + sin(u)minorRadius)
	// where u ranges from 0 to 2¹ and v ranges from 0 to ¹.
	// If you consider the case where (majorRadius, minorRadius, orientation) form a
	// right-handed system, with orientation pointing "up", then u increases in a
	// counterclockwise direction and v increases from bottom to top.
	// A normal vector can be computed as the cross product of the two partials,
	// (-sin(u)sin(v)majorRadius + cos(u)sin(v)minorRadius) x
	// (sin(v)orientation + cos(u)cos(v)majorRadius + sin(u)cos(v)minorRadius)
	// = sin(v)[ -sin(u)sin(v)majorRadius x orientation
	//			- sin(u)sin(u)cos(v)majorRadius x minorRadius
	//			+ cos(u)sin(v)minorRadius x orientation
	//			+ cos(u)cos(u)cos(v)minorRadius x majorRadius ]
	// In the right-handed case, this gives us an inward-pointing normal, and in the
	// left-handed case it's outward-pointing.
	// Since we're going to normalize the vector anyway, we can forget the outer scalar
	// factor of sin(v).  The rest simplifies to
	// - sin(u)sin(v)majorRadius x orientation
	// + cos(u)sin(v)minorRadius x orientation
	//  - cos(v)majorRadius x minorRadius .
	// Best to compute those 3 cross products outside of any loops.
	Q3Vector3D_Cross( &geomData.majorRadius, &geomData.orientation, &majXOrient );
	Q3Vector3D_Cross( &geomData.minorRadius, &geomData.orientation, &minXOrient );
	Q3Vector3D_Cross( &geomData.majorRadius, &geomData.minorRadius, &majXMinor );
	
	// Right or left handed?
	bool isRightHanded = (Q3Vector3D_Dot( &majXMinor, &geomData.orientation ) > 0.0);
	
	// At the v = 0 (south) pole, our normal vector formula boils down to -majXMinor, and
	// at the v = ¹ (north) pole it becomes majXMinor.  When the system is left-handed,
	// the normals need to be negated to point outward.
	TQ3Point3D		pole0, polePi;
	Q3Point3D_Vector3D_Add( &geomData.origin, &geomData.orientation, &polePi );
	Q3Point3D_Vector3D_Subtract( &geomData.origin, &geomData.orientation, &pole0 );
	TQ3Vector3D		normPole0, normPolePi;
	if (isRightHanded)
	{
		normPolePi = majXMinor;
	}
	else
	{
		Q3Vector3D_Negate( &majXMinor, &normPolePi );
	}
	Q3Vector3D_Normalize( &normPolePi, &normPolePi );
	Q3Vector3D_Negate( &normPolePi, &normPole0 );


	// Start filling in points, normals, uvs
	pnum = 0;		// what point we're working on

	
	for (v = 0, vang = kQ3Pi * vMin;
		v <= vSegments;
		++v, vang += vDeltaAngle)
	{
		// for row v... find the points around the circle (by u)
		sinVAngle = (float)sin(vang);
		cosVAngle = (float)cos(vang);
		
		uMaxIndex = uSegments;
		if ( (isSouthPresent && (v==0)) ||
			(isNorthPresent && (v == vSegments)) )
		{
			// Omit final point on polar edges
			uMaxIndex -= 1;
		}
		
		
		for (u = 0, uang = kQ32Pi * uMin; u <= uMaxIndex; ++u, uang += uDeltaAngle)
		{
			sinUAngle = (float)sin(uang);
			cosUAngle = (float)cos(uang);
			
			// start with point on equator around <0,0,0>: cos(major) + sin(minor)
			Q3FastVector3D_Scale( &geomData.majorRadius, cosUAngle, &vec );
			Q3FastVector3D_Scale( &geomData.minorRadius, sinUAngle, &vec2 );
			Q3FastVector3D_Add( &vec2, &vec, &vec );
			
			// then, scale this and shift it into the proper row
			Q3FastVector3D_Scale( &vec, sinVAngle, &vec );
			Q3FastVector3D_Scale( &geomData.orientation, cosVAngle, &axis );
			Q3FastVector3D_Subtract( &vec, &axis, &vec );
			Q3FastPoint3D_Vector3D_Add( &geomData.origin, &vec, &points[pnum] );
			
			// Compute the vertex normal vector
			Q3FastVector3D_Scale( &majXOrient, - sinUAngle * sinVAngle, &normals[pnum] );
			Q3FastVector3D_Scale( &minXOrient, cosUAngle * sinVAngle, &vec );
			Q3FastVector3D_Add( &vec, &normals[pnum], &normals[pnum] );
			Q3FastVector3D_Scale( &majXMinor, - cosVAngle, &vec );
			Q3FastVector3D_Add( &vec, &normals[pnum], &normals[pnum] );
			if (! isRightHanded)
				Q3FastVector3D_Negate( &normals[pnum], &normals[pnum] );
			Q3FastVector3D_Normalize( &normals[pnum], &normals[pnum] );

			
			// Set up the UVs
			uvs[pnum].u = uDelta * u;
			uvs[pnum].v = vDelta * v;


			// Set up triangles for u, u+1 and v, v+1
			if ( (u < uSegments) && (v < vSegments) )
			{
				// end caps
				if ( isSouthPresent && (v==0) )
				{
					triangles[tnum].pointIndices[0] = uSegments + pnum + 1;
					triangles[tnum].pointIndices[1] = uSegments + pnum;
					triangles[tnum].pointIndices[2] = pnum;
					tnum++;
				}
				else if ( isNorthPresent && (v == vSegments-1) )
				{
					triangles[tnum].pointIndices[0] = pnum;
					triangles[tnum].pointIndices[1] = pnum + 1;
					triangles[tnum].pointIndices[2] = uSegments + 1 + pnum;
					tnum++;
				}
				else
				{
					triangles[tnum].pointIndices[0] = pnum;
					triangles[tnum].pointIndices[1] = pnum + 1;
					triangles[tnum].pointIndices[2] = pnum + uSegments + 1;
					tnum++;
					triangles[tnum].pointIndices[0] = pnum + 1;
					triangles[tnum].pointIndices[1] = pnum + uSegments + 1 + 1;
					triangles[tnum].pointIndices[2] = pnum + uSegments + 1;
					tnum++;
				}
			}

			pnum++;
		}
	}
	
	
	
	// Compute face normals
	Q3Triangle_CrossProductArray( numTriangles, NULL,
		&triangles[0].pointIndices[0], &points[0], &faceNormals[0] );



	// set up remaining trimesh data
	TQ3TriMeshAttributeData vertexAttributes[2] =
	{
		{ kQ3AttributeTypeNormal, &normals[0], NULL },
		{ kQ3AttributeTypeSurfaceUV, &uvs[0], NULL }
	};
	TQ3TriMeshAttributeData	faceAttributes[1] =
	{
		{ kQ3AttributeTypeNormal, &faceNormals[0], NULL },
	};

	triMeshData.triMeshAttributeSet		= NULL;
	triMeshData.numPoints                 = numPoints;
	triMeshData.points                    = &points[0];
	triMeshData.numTriangles              = numTriangles;
	triMeshData.triangles                 = &triangles[0];
	triMeshData.numTriangleAttributeTypes = 1;
	triMeshData.triangleAttributeTypes    = faceAttributes;
	triMeshData.numEdges                  = 0;
	triMeshData.edges                     = NULL;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 2;
	triMeshData.vertexAttributeTypes      = vertexAttributes;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox,
									triMeshData.points,
									numPoints,
									sizeof(TQ3Point3D));



	// Create the TriMesh
	return CQ3ObjectRef( Q3TriMesh_New(&triMeshData) );
}



//=============================================================================
//      e3geom_ellipsoid_cache_new : Ellipsoid cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_ellipsoid_cache_new( TQ3ViewObject theView, TQ3GeometryObject theGeom,
							const TQ3EllipsoidData *geomData )
{
	// Check for coplanar axes.
	if (E3Geometry_IsDegenerateTriple( &geomData->orientation, &geomData->majorRadius,
		&geomData->minorRadius ))
	{
		E3ErrorManager_PostError( kQ3ErrorDegenerateGeometry, kQ3False );
		return NULL;
	}
	
	
	
	// Validate u and v bounds.
	
	//   Clamp to interval [0, 1].
	float uMin  = E3Num_Clamp(geomData->uMin, 0.0f, 1.0f);
	float uMax  = E3Num_Clamp(geomData->uMax, 0.0f, 1.0f);
	float vMin  = E3Num_Clamp(geomData->vMin, 0.0f, 1.0f);
	float vMax  = E3Num_Clamp(geomData->vMax, 0.0f, 1.0f);
	//   Do not allow upper and lower bound to be the same.
	if ( (E3Float_Abs( uMin - uMax ) <= kQ3RealZero) ||
		(E3Float_Abs( vMin - vMax ) <= kQ3RealZero) )
	{
		E3ErrorManager_PostError( kQ3ErrorDegenerateGeometry, kQ3False );
		return NULL;
	}
	//   Do not allow vMin to be greater than vMax.
	if (vMin > vMax)
	{
		E3Float_Swap( vMin, vMax );
	}
	//   It might make sense for uMin to be greater than uMax, to indicate
	//   going the other way around the circle.  But we prefer to enforce
	//   uMin < uMax, at the expense of allowing uMax to be greater than 1.
	if (uMin > uMax)
	{
		uMax += 1.0f;
	}
	
	
	
	// Determine whether the surface is missing either pole or the u seam.
	bool	isNorthPolePresent = (1.0f - vMax) < kQ3RealZero;
	bool	isSouthPolePresent = vMin < kQ3RealZero;
	bool	isCircleComplete = E3Float_Abs( uMax - uMin - 1.0f ) < kQ3RealZero;
	
	
	
	// Make a group and add an orientation style.
	CQ3ObjectRef	resultGroup( Q3DisplayGroup_New() );
	CQ3ObjectRef	orientationStyle( Q3OrientationStyle_New(
		kQ3OrientationStyleCounterClockwise ) );
	Q3Group_AddObject( resultGroup.get(), orientationStyle.get() );
	
	
	// If there is an ellipsoidAttributeSet, add it to the group.  It is
	// necessary to do this, instead of putting it in the main TriMesh, so
	// that these attributes can be inherited by caps even if there is also
	// an interiorAttributeSet.
	if (geomData->ellipsoidAttributeSet != NULL)
	{
		Q3Group_AddObject( resultGroup.get(), geomData->ellipsoidAttributeSet );
	}
	
	
	// Determine the number of divisions in the u and v directions using the
	// subdivision style.
	TQ3Uns32 uSegments = 16;		// how many longitude lines
	TQ3Uns32 vSegments = 8;		// how many latitude lines
	TQ3SubdivisionStyleData subdivisionData;
	if (Q3View_GetSubdivisionStyleState( theView, &subdivisionData ) == kQ3Success)
	{
		switch (subdivisionData.method) {
			case kQ3SubdivisionMethodConstant:
				// uSegments and vSegments are given directly
				uSegments = (TQ3Uns32) subdivisionData.c1;
				vSegments = (TQ3Uns32) subdivisionData.c2;
				break;
			
			case kQ3SubdivisionMethodWorldSpace:
				// keep the length of any side less than or equal to c1;
				// so divide the circumference by c1
				{
					TQ3Matrix4x4	localToWorld;
					float			majLen, minLen, orientLen, bigLen;
					TQ3Vector3D		vec;
					
					// Find the lengths of the vectors, in world space.
					Q3View_GetLocalToWorldMatrixState( theView, &localToWorld );
					Q3Vector3D_Transform( &geomData->majorRadius, &localToWorld, &vec );
					majLen = Q3Vector3D_Length( &vec );
					Q3Vector3D_Transform( &geomData->minorRadius, &localToWorld, &vec );
					minLen = Q3Vector3D_Length( &vec );
					Q3Vector3D_Transform( &geomData->orientation, &localToWorld, &vec );
					orientLen = Q3Vector3D_Length( &vec );
					
					// The u direction depends only on the major and minor axes
					bigLen = E3Num_Max( majLen, minLen );
					uSegments = (TQ3Uns32) ((kQ32Pi * bigLen) / subdivisionData.c1);
					
					// In the v direction, the orientation acts as one of the radii.
					// Here we use pi rather than 2pi, since v goes half way around.
					bigLen = E3Num_Max( bigLen, orientLen );
					vSegments = (TQ3Uns32) ((kQ3Pi * bigLen) / subdivisionData.c1);
				}
				break;

			case kQ3SubdivisionMethodScreenSpace:
				// Not implemented
				E3ErrorManager_PostWarning( kQ3WarningUnsupportedSubdivisionStyle );
				break;
			
			default:
				Q3_ASSERT(!"Unknown subdivision method");
				E3ErrorManager_PostWarning( kQ3WarningUnsupportedSubdivisionStyle );
				break;
		}

		// sanity checking -- important in case the user screws up the subdivisionData
		if (uSegments < 3) uSegments = 3;
		if (vSegments < 3) vSegments = 3;
	}
	
	
	
	// Make the main surface geometry
	CQ3ObjectRef	theTriMesh( e3geom_ellipsoid_create_face( *geomData,
		uMin, uMax, vMin, vMax, isNorthPolePresent, isSouthPolePresent,
		uSegments, vSegments ) );
	Q3Group_AddObject( resultGroup.get(), theTriMesh.get() );


	// Do we need to add caps?
	bool	isTopCapNeeded = (! isNorthPolePresent) &&
		((geomData->caps & kQ3EndCapMaskTop) != 0);
	bool	isBottomCapNeeded = (! isSouthPolePresent) &&
		((geomData->caps & kQ3EndCapMaskBottom) != 0);
	bool	isInteriarCapNeeded = (! isCircleComplete) &&
		((geomData->caps & kQ3EndCapMaskInterior) != 0);
	if (isTopCapNeeded || isBottomCapNeeded || isInteriarCapNeeded)
	{
		CQ3ObjectRef	theCaps( e3geom_ellipsoid_create_caps( *geomData,
			uMin, uMax, vMin, vMax, uSegments, vSegments,
			isTopCapNeeded, isBottomCapNeeded, isInteriarCapNeeded ) );
		Q3Group_AddObject( resultGroup.get(), theCaps.get() );
	}


	// Return the cached geometry
	return Q3Shared_GetReference( resultGroup.get() );
}





//=============================================================================
//      e3geom_ellipsoid_get_attribute : Ellipsoid get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_ellipsoid_get_attribute ( E3Ellipsoid* ellipsoid )
	{
	// Return the address of the geometry attribute set
	return & ellipsoid->instanceData.ellipsoidAttributeSet ;
	}





//=============================================================================
//      e3geom_ellipsoid_metahandler : Ellipsoid metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_ellipsoid_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_cache_new;
			break;

		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_get_attribute;
			break;
		
		case kQ3XMethodTypeGeomUsesSubdivision:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryEllipsoid_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryEllipsoid_RegisterClass(void)
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryEllipsoid,
								e3geom_ellipsoid_metahandler,
								E3Ellipsoid ) ;
	}





//=============================================================================
//      E3GeometryEllipsoid_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryEllipsoid_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypeEllipsoid, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Ellipsoid_New : Create an ellipsoid object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Ellipsoid_New(const TQ3EllipsoidData *ellipsoidData)
{	TQ3Object		theObject;


	if (ellipsoidData == NULL)
	{
		TQ3EllipsoidData	defaultData = {
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			0.0f, 1.0f, 0.0f, 1.0f,
			kQ3EndCapNone,
			NULL, NULL
		};
		theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeEllipsoid, kQ3False, &defaultData ) ;
	}
	else
	{
		theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeEllipsoid, kQ3False, ellipsoidData ) ;
	}


	return(theObject);
}





//=============================================================================
//      E3Ellipsoid_Submit : Submit an ellipsoid.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_Submit(const TQ3EllipsoidData *ellipsoidData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeEllipsoid, ellipsoidData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Ellipsoid_SetData : Set the data for an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetData(TQ3GeometryObject theEllipsoid, const TQ3EllipsoidData *ellipsoidData)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	// first, free the old data
	e3geom_ellipsoid_disposedata ( & ellipsoid->instanceData ) ;

	// then copy in the new data
	TQ3Status qd3dStatus = e3geom_ellipsoid_copydata ( ellipsoidData, & ellipsoid->instanceData, kQ3False ) ;
	
	Q3Shared_Edited ( ellipsoid ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3Ellipsoid_GetData : Get the data for an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetData(TQ3GeometryObject theEllipsoid, TQ3EllipsoidData *ellipsoidData)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	// Copy the data out of the Ellipsoid
	ellipsoidData->interiorAttributeSet = NULL ;
	ellipsoidData->ellipsoidAttributeSet = NULL ;
	return e3geom_ellipsoid_copydata ( & ellipsoid->instanceData, ellipsoidData, kQ3False ) ;
	}





//=============================================================================
//      E3Ellipsoid_SetOrigin : Set the origin of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetOrigin(TQ3GeometryObject theEllipsoid, const TQ3Point3D *origin)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( origin, & ellipsoid->instanceData.origin, sizeof(TQ3Point3D) ) ;

	Q3Shared_Edited ( ellipsoid ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_SetOrientation : Set the orientation of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetOrientation(TQ3GeometryObject theEllipsoid, const TQ3Vector3D *orientation)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( orientation, & ellipsoid->instanceData.orientation, sizeof(TQ3Vector3D) ) ;

	Q3Shared_Edited ( ellipsoid ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_SetMajorRadius : Set the major radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetMajorRadius(TQ3GeometryObject theEllipsoid, const TQ3Vector3D *majorRadius)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( majorRadius, & ellipsoid->instanceData.majorRadius, sizeof(TQ3Vector3D) ) ;

	Q3Shared_Edited ( ellipsoid ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_SetMinorRadius : Set the minor radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetMinorRadius(TQ3GeometryObject theEllipsoid, const TQ3Vector3D *minorRadius)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( minorRadius, & ellipsoid->instanceData.minorRadius, sizeof(TQ3Vector3D) ) ;

	Q3Shared_Edited ( ellipsoid ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_GetOrigin : Get the origin of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetOrigin(TQ3GeometryObject theEllipsoid, TQ3Point3D *origin)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( & ellipsoid->instanceData.origin, origin, sizeof(TQ3Point3D) ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_GetOrientation : Get the orientation of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetOrientation(TQ3GeometryObject theEllipsoid, TQ3Vector3D *orientation)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( & ellipsoid->instanceData.orientation, orientation, sizeof(TQ3Vector3D) ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_GetMajorRadius : Get the major radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetMajorRadius(TQ3GeometryObject theEllipsoid, TQ3Vector3D *majorRadius)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( & ellipsoid->instanceData.majorRadius, majorRadius, sizeof(TQ3Vector3D) ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_GetMinorRadius : Get the minor radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetMinorRadius(TQ3GeometryObject theEllipsoid, TQ3Vector3D *minorRadius)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( & ellipsoid->instanceData.minorRadius, minorRadius, sizeof(TQ3Vector3D) ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_EmptyData : Empty the data for an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_EmptyData(TQ3EllipsoidData *ellipsoidData)
{

	// Dispose of the data
	e3geom_ellipsoid_disposedata(ellipsoidData);

	return(kQ3Success);
}




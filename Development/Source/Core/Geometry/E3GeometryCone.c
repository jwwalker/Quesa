/*  NAME:
        E3GeometryCone.c

    DESCRIPTION:
        Implementation of Quesa Cone geometry class.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#include "E3View.h"
#include "E3Geometry.h"
#include "E3GeometryTriMesh.h"
#include "E3GeometryCone.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_cone_copydata : Copy TQ3ConeData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cone_copydata(const TQ3ConeData *src, TQ3ConeData *dst, TQ3Boolean isDuplicate)
{
	TQ3Status		qd3dStatus = kQ3Success;

	// copy raw data
	const TQ3Uns32 theSize = sizeof(TQ3Point3D)	// origin
		+ 3*sizeof(TQ3Vector3D)	// orientation and 2 axes
		+ 4*sizeof(float)		// u/v min/max
		+ sizeof(TQ3EndCap);	// endcaps
	Q3Memory_Copy( src, dst, theSize );

	// copy or shared-replace the attributes
	if (isDuplicate)
	{
		if (src->interiorAttributeSet != NULL)
		{
			dst->interiorAttributeSet = Q3Object_Duplicate(src->interiorAttributeSet);
			if (dst->interiorAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->interiorAttributeSet = NULL;

		if (src->faceAttributeSet != NULL)
		{
			dst->faceAttributeSet = Q3Object_Duplicate(src->faceAttributeSet);
			if (dst->faceAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->faceAttributeSet = NULL;

		if (src->bottomAttributeSet != NULL)
		{
			dst->bottomAttributeSet = Q3Object_Duplicate(src->bottomAttributeSet);
			if (dst->bottomAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->bottomAttributeSet = NULL;

		if (src->coneAttributeSet != NULL)
		{
			dst->coneAttributeSet = Q3Object_Duplicate(src->coneAttributeSet);
			if (dst->coneAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->coneAttributeSet = NULL;
	}
	else {
		E3Shared_Replace(&dst->interiorAttributeSet, src->interiorAttributeSet);
		E3Shared_Replace(&dst->faceAttributeSet, src->faceAttributeSet);
		E3Shared_Replace(&dst->bottomAttributeSet, src->bottomAttributeSet);
		E3Shared_Replace(&dst->coneAttributeSet, src->coneAttributeSet);
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_cone_disposedata : Dispose of a TQ3ConeData.
//-----------------------------------------------------------------------------
static void
e3geom_cone_disposedata(TQ3ConeData *theCone)
{
	Q3Object_CleanDispose(&theCone->interiorAttributeSet );
	Q3Object_CleanDispose(&theCone->faceAttributeSet );
	Q3Object_CleanDispose(&theCone->bottomAttributeSet );
	Q3Object_CleanDispose(&theCone->coneAttributeSet );
}	





//=============================================================================
//      e3geom_cone_new : Cone new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cone_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3ConeData			*instanceData = (TQ3ConeData *)       privateData;
	const TQ3ConeData	*coneData     = (const TQ3ConeData *) paramData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)

	qd3dStatus = e3geom_cone_copydata(coneData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_cone_delete : Cone delete method.
//-----------------------------------------------------------------------------
static void
e3geom_cone_delete(TQ3Object theObject, void *privateData)
{	TQ3ConeData		*instanceData = (TQ3ConeData *) privateData;
#pragma unused(theObject)

	// Dispose of our instance data
	e3geom_cone_disposedata(instanceData);
}





//=============================================================================
//      e3geom_cone_duplicate : Cone duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cone_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TQ3ConeData	*fromInstanceData = (const TQ3ConeData *) fromPrivateData;
	TQ3ConeData			*toInstanceData   = (TQ3ConeData *)       toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_cone_copydata( fromInstanceData, toInstanceData, kQ3True );

	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_cone_disposedata(toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_cone_create_face : Helper for e3geom_cone_cache_new.
//-----------------------------------------------------------------------------
static void e3geom_cone_create_face( TQ3GroupObject ioGroup, const TQ3ConeData* inData,
	TQ3Uns32 inNumSides, TQ3Uns32 inNumBands, TQ3Boolean inTipPresent )
{
	float				ang, dang, cosAngle, sinAngle;
	float				startAngle, endAngle;
	float				v, vStep;
	TQ3Vector3D			workVec, otherVec, sideVec, radialVec;
	TQ3Vector3D			vertNormVec;
	TQ3Point3D			bottomPt;
	TQ3Vector3D			majXMinor, majXOrient, minXOrient;
	TQ3Param2D			*uvs;
	TQ3TriMeshData		triMeshData;
	TQ3GeometryObject	theTriMesh;
	TQ3Point3D 			*points;
	TQ3Vector3D 		*normals;
	TQ3TriMeshTriangleData *triangles;
	TQ3TriMeshAttributeData vertexAttributes[2];
	TQ3Uns32			numpoints, numFaces;
	TQ3Uns32			i, j;



	// Turn the u limits into a counterclockwise angle range in radians, and
	// figure the angle step size.
	startAngle = inData->uMin * kQ32Pi;
	endAngle = inData->uMax * kQ32Pi;
	dang = (endAngle - startAngle) / inNumSides;



	// Band step size
	vStep = (inData->vMax - inData->vMin) / inNumBands;



	// Normal computation:
	// The cone has a parametric equation
	// f(u,v) = origin + v*orientation + (1-v)(cos(u)majorRadius + sin(u)minorRadius)
	// where u ranges from 0 to 2pi and v ranges from 0 to 1 (0 being the base).
	// Therefore we can get a surface normal as the cross product of partial
	// derivatives,
	// ((1-v)(-sin(u)majorRadius + cos(u)minorRadius) x
	//		(orientation - cos(u) majorRadius - sin(u)minorRadius)
	// = (1-v)[ -sin(u)majorRadius x orientation +sin(u)sin(u)majorRadius x minorRadius
	//			+ cos(u)minorRadius x orientation - cos(u)cos(u)minorRadius x majorRadius ]
	// = (1-v)[ -sin(u)majorRadius x orientation + cos(u)minorRadius x orientation
	//			+ majorRadius x minorRadius ].
	// So long as (majorRadius, minorRadius, orientation) forms a right-handed system,
	// this will be an outward normal.  We can omit the nonnegative scalar 1-v.

	// Best to compute those 3 cross products outside of any loops.
	Q3Vector3D_Cross( &inData->majorRadius, &inData->minorRadius, &majXMinor );
	Q3Vector3D_Cross( &inData->majorRadius, &inData->orientation, &majXOrient );
	Q3Vector3D_Cross( &inData->minorRadius, &inData->orientation, &minXOrient );



	// We will use (inNumSides + 1) discrete values of the u parameter, and inNumBands + 1
	// discrete values of the v parameter, hence the number of points is...
	numpoints = (inNumBands + 1) * (inNumSides + 1);



	// In most bands, there are two triangles for each side.  But if there is a tip, then
	// the last band has one triangle per side.
	if (inTipPresent)
	{
		numFaces = inNumSides * (2 * inNumBands - 1);
	}
	else
	{
		numFaces = 2 * inNumSides * inNumBands;
	}



	// Allocate some memory for the TriMesh
	points    = (TQ3Point3D *)             Q3Memory_Allocate( numpoints*sizeof(TQ3Point3D) );
	normals   = (TQ3Vector3D *)            Q3Memory_Allocate( numpoints*sizeof(TQ3Vector3D) );
	uvs       = (TQ3Param2D  *)            Q3Memory_Allocate( numpoints * sizeof(TQ3Param2D) );
	triangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate( numFaces*sizeof(TQ3TriMeshTriangleData) );
	if (points == NULL || normals == NULL || uvs == NULL || triangles == NULL)
	{
		Q3Memory_Free(&points);
		Q3Memory_Free(&normals);
		Q3Memory_Free(&uvs);
		Q3Memory_Free(&triangles);
		
		return;
	}



	// Compute points etc.
	// Points (and normals and uvs) will be indexed with point i on band j being
	//	points[(inNumSides+1)*j + i].
	// Triangles (and face normals) will be indexed such that between points i and i+1,
	// and between bands j and j+1, there are two triangles, numbered 2*(inNumSides*j+i) and
	// 2*(inNumSides*j+i)+1.  The exception is that if the tip is present, then the last
	// band (j = inNumBands - 1) has only one triangle per side, number 2*(inNumSides*j)+i.
	ang = 0.0f;
	for (i=0, ang = startAngle; i <= inNumSides; ++i, ang += dang)
	{
		cosAngle = (float) cos(ang);
		sinAngle = (float) sin(ang);

		// Compute part of what we need for points.
		// In the parametric equation for the cone, for a fixed u let
		//	radialVec = cos(u)majorRadius + sin(u)minorRadius
		// and then the equation becomes
		// f(u,v) = origin + v*orientation + (1-v)radialVec
		//		  = origin + radialVec + v*(orientation - radialVec).
		// We will let bottomPt stand for origin + radialVec, and let
		// sideVec stand for orientation - radialVec, so the points at this
		// value of u will be given by bottomPt + v * sideVec.
		Q3Vector3D_Scale( &inData->majorRadius, cosAngle, &workVec );
		Q3Vector3D_Scale( &inData->minorRadius, sinAngle, &radialVec );
		Q3Vector3D_Add( &workVec, &radialVec, &radialVec );
		Q3Vector3D_Subtract( &inData->orientation, &radialVec, &sideVec );
		Q3Point3D_Vector3D_Add( &inData->origin, &radialVec, &bottomPt );
		

		// The vertex normal is the same for all points at this angle.
		Q3Vector3D_Scale( &majXOrient, - sinAngle, &workVec );
		Q3Vector3D_Scale( &minXOrient, cosAngle, &otherVec );
		Q3Vector3D_Add( &workVec, &otherVec, &workVec );
		Q3Vector3D_Add( &workVec, &majXMinor, &vertNormVec );

		
		for (j = 0, v = inData->vMin; j <= inNumBands; ++j, v += vStep)
		{
			// Compute a point
			Q3Vector3D_Scale( &sideVec, v, &workVec );
			Q3Point3D_Vector3D_Add( &bottomPt, &workVec, &points[(inNumSides+1)*j + i] );
			
			// Vertex normal
			normals[(inNumSides+1)*j + i] = vertNormVec;
			
			// Surface parameters
			uvs[(inNumSides+1)*j + i].u = i / ((float) inNumSides);
			uvs[(inNumSides+1)*j + i].v = v;
			
			// Triangles and face normals
			if ( (i > 0) && (j > 0) )
			{
				if ( inTipPresent && (j == inNumBands) )
				{
					Q3_ASSERT( 2*(inNumSides*(j-1))+i-1 < numFaces );
					triangles[2*(inNumSides*(j-1))+i-1].pointIndices[0] =
						(inNumSides+1)*(j-1) + i-1;
					triangles[2*(inNumSides*(j-1))+i-1].pointIndices[1] =
						(inNumSides+1)*(j-1) + i;
					triangles[2*(inNumSides*(j-1))+i-1].pointIndices[2] =
						(inNumSides+1)*(j) + i;
				}
				else
				{
					Q3_ASSERT( 2*(inNumSides*(j-1)+i-1)+1 < numFaces );
					
					triangles[2*(inNumSides*(j-1)+i-1)].pointIndices[0] =
						(inNumSides+1)*(j-1) + i-1;
					triangles[2*(inNumSides*(j-1)+i-1)].pointIndices[1] =
						(inNumSides+1)*(j) + i;
					triangles[2*(inNumSides*(j-1)+i-1)].pointIndices[2] =
						(inNumSides+1)*(j) + i-1;

					triangles[2*(inNumSides*(j-1)+i-1)+1].pointIndices[0] =
						(inNumSides+1)*(j-1) + i-1;
					triangles[2*(inNumSides*(j-1)+i-1)+1].pointIndices[1] =
						(inNumSides+1)*(j-1) + i;
					triangles[2*(inNumSides*(j-1)+i-1)+1].pointIndices[2] =
						(inNumSides+1)*(j) + i;
				}
			}
		}
	}



	// Set up TriMesh data
	vertexAttributes[0].attributeType     = kQ3AttributeTypeNormal;
	vertexAttributes[0].data              = normals;
	vertexAttributes[0].attributeUseArray = NULL;

	vertexAttributes[1].attributeType     = kQ3AttributeTypeSurfaceUV;
	vertexAttributes[1].data              = uvs;
	vertexAttributes[1].attributeUseArray = NULL;
	
	triMeshData.numPoints                 = numpoints;
	triMeshData.points                    = points;
	triMeshData.numTriangles              = numFaces;
	triMeshData.triangles                 = triangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = 0;
	triMeshData.edges                     = NULL;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 2;
	triMeshData.vertexAttributeTypes      = vertexAttributes;
	triMeshData.triMeshAttributeSet = inData->faceAttributeSet;
	Q3BoundingBox_SetFromPoints3D( &triMeshData.bBox,
									points,
									numpoints,
									sizeof(TQ3Point3D));



	// Create the TriMesh and add to the group
	theTriMesh = Q3TriMesh_New(&triMeshData);
	if (theTriMesh != NULL)
		{
		E3TriMesh_AddTriangleNormals(theTriMesh, kQ3OrientationStyleCounterClockwise);
		Q3Group_AddObjectAndDispose( ioGroup, &theTriMesh );
		}



	// Clean up
	Q3Memory_Free(&points);
	Q3Memory_Free(&normals);
	Q3Memory_Free(&uvs);
	Q3Memory_Free(&triangles);
}





//=============================================================================
//      e3geom_cone_create_interior : Create the interior of the cone.
//-----------------------------------------------------------------------------
static void e3geom_cone_create_interior( TQ3GroupObject ioGroup, const TQ3ConeData* inData,
	TQ3Boolean inHasTip )
{
	
	if (inHasTip == kQ3False)	// Disk at top?
	{
		TQ3DiskData			topDisk;
		TQ3GeometryObject	topGeom;
		TQ3Vector3D			workVec;
		
		Q3Vector3D_Scale( &inData->orientation, inData->vMax, &workVec );
		Q3Point3D_Vector3D_Add( &inData->origin, &workVec, &topDisk.origin );
		Q3Vector3D_Scale( &inData->majorRadius, 1.0f - inData->vMax, &topDisk.majorRadius );
		Q3Vector3D_Scale( &inData->minorRadius, 1.0f - inData->vMax, &topDisk.minorRadius );
		topDisk.uMin = inData->uMin;
		topDisk.uMax = inData->uMax;
		if (topDisk.uMax > 1.0f)
		{
			topDisk.uMax -= 1.0f;
		}
		topDisk.vMin = 0.0f;
		topDisk.vMax = 1.0f;
		topDisk.diskAttributeSet = inData->interiorAttributeSet;
		
		topGeom = Q3Disk_New( &topDisk );
		if (topGeom != NULL)
		{
			Q3Group_AddObjectAndDispose( ioGroup, &topGeom );
		}
	}
	
	// If the u values don't go all the way around, there's more interior.
	if (inData->uMax - inData->uMin < 1.0f - kQ3RealZero)
	{
		float			cosAngle, sinAngle;
		float			startAngle, endAngle;
		TQ3Point3D		bottomCenter, topCenter;
		TQ3Vector3D		workVec, radialVec;
		TQ3Point3D		interiorPts[4];
		TQ3Vector3D		interiorPtNorms[4];
		TQ3Param2D		interiorUVs[4];
		TQ3Vector3D		interiorFaceNorms[2];
		TQ3TriMeshTriangleData	interiorTris[2] = {
			{ { 0, 1, 2 } }, { { 1, 3, 2 } }
		};
		TQ3TriMeshAttributeData	intPtAtts[2] = {
			{ kQ3AttributeTypeNormal, NULL, NULL },
			{ kQ3AttributeTypeSurfaceUV, NULL, NULL }
		};
		TQ3TriMeshAttributeData	intFaceAtts = {
			kQ3AttributeTypeNormal, NULL, NULL
		};
		TQ3TriMeshData	intTriMeshData;
		TQ3GeometryObject	intGeom = NULL;
		
		intPtAtts[0].data = interiorPtNorms;
		intPtAtts[1].data = interiorUVs;
		intFaceAtts.data = interiorFaceNorms;
		
		// Set TriMesh fields that are the same in all cases
		intTriMeshData.triMeshAttributeSet = inData->interiorAttributeSet;
		intTriMeshData.triangles = interiorTris;
		intTriMeshData.numTriangleAttributeTypes = 1;
		intTriMeshData.triangleAttributeTypes = &intFaceAtts;
		intTriMeshData.numEdges = 0;
		intTriMeshData.edges = NULL;
		intTriMeshData.numEdgeAttributeTypes = 0;
		intTriMeshData.edgeAttributeTypes = NULL;
		intTriMeshData.points = interiorPts;
		intTriMeshData.numVertexAttributeTypes = 2;
		intTriMeshData.vertexAttributeTypes = intPtAtts;

		// Find the bottom and top centers.
		Q3Vector3D_Scale( &inData->orientation, inData->vMin, &workVec );
		Q3Point3D_Vector3D_Add( &inData->origin, &workVec, &bottomCenter );
		Q3Vector3D_Scale( &inData->orientation, inData->vMax - inData->vMin, &workVec );
		Q3Point3D_Vector3D_Add( &bottomCenter, &workVec, &topCenter );

		startAngle = inData->uMin * kQ32Pi;
		endAngle = inData->uMax * kQ32Pi;

		if (inHasTip)	// interior is two one-triangle TriMeshes
		{
			intTriMeshData.numPoints = 3;
			intTriMeshData.numTriangles  = 1;
			
			// Left triangle
			cosAngle = (float) cos(endAngle);
			sinAngle = (float) sin(endAngle);
			Q3Vector3D_Scale( &inData->majorRadius, (1.0f - inData->vMin) * cosAngle, &workVec );
			Q3Vector3D_Scale( &inData->minorRadius, (1.0f - inData->vMin) * sinAngle, &radialVec );
			Q3Vector3D_Add( &workVec, &radialVec, &radialVec );
			Q3Point3D_Vector3D_Add( &bottomCenter, &radialVec, &interiorPts[0] );	// bottom left
			interiorPts[1] = bottomCenter;
			interiorPts[2] = topCenter;
			Q3Vector3D_Cross( &inData->orientation, &radialVec, &workVec );
			interiorPtNorms[0] = interiorPtNorms[1] = interiorPtNorms[2] = workVec;
			interiorFaceNorms[0] = workVec;
			interiorUVs[0].u = 0.0f;
			interiorUVs[0].v = 0.0f;
			interiorUVs[1].u = 0.5f;
			interiorUVs[1].v = 0.0f;
			interiorUVs[2].u = 0.5f;
			interiorUVs[2].v = 1.0f;
			Q3BoundingBox_SetFromPoints3D( &intTriMeshData.bBox, intTriMeshData.points,
				intTriMeshData.numPoints, sizeof(TQ3Point3D) );
			intGeom = Q3TriMesh_New( &intTriMeshData );
			if (intGeom != NULL)
			{
				E3TriMesh_AddTriangleNormals(intGeom, kQ3OrientationStyleCounterClockwise);
				Q3Group_AddObjectAndDispose( ioGroup, &intGeom );
			}
			
			// Right triangle
			cosAngle = (float) cos(startAngle);
			sinAngle = (float) sin(startAngle);
			Q3Vector3D_Scale( &inData->majorRadius, (1.0f - inData->vMin) * cosAngle, &workVec );
			Q3Vector3D_Scale( &inData->minorRadius, (1.0f - inData->vMin) * sinAngle, &radialVec );
			Q3Vector3D_Add( &workVec, &radialVec, &radialVec );
			Q3Point3D_Vector3D_Add( &bottomCenter, &radialVec, &interiorPts[1] );	// bottom right
			interiorPts[0] = bottomCenter;
			interiorPts[2] = topCenter;
			Q3Vector3D_Cross( &radialVec, &inData->orientation, &workVec );
			interiorPtNorms[0] = interiorPtNorms[1] = interiorPtNorms[2] = workVec;
			interiorFaceNorms[0] = workVec;
			interiorUVs[0].u = 0.5f;
			interiorUVs[1].u = 1.0f;
			Q3BoundingBox_SetFromPoints3D( &intTriMeshData.bBox, intTriMeshData.points,
				intTriMeshData.numPoints, sizeof(TQ3Point3D) );
			intGeom = Q3TriMesh_New( &intTriMeshData );
			if (intGeom != NULL)
			{
				E3TriMesh_AddTriangleNormals(intGeom, kQ3OrientationStyleCounterClockwise);
				Q3Group_AddObjectAndDispose( ioGroup, &intGeom );
			}
		}
		else	// interior is two two-triangle TriMeshes
		{
			intTriMeshData.numPoints = 4;
			intTriMeshData.numTriangles  = 2;
			
			// Left side
			cosAngle = (float) cos(endAngle);
			sinAngle = (float) sin(endAngle);
			Q3Vector3D_Scale( &inData->majorRadius, cosAngle, &workVec );
			Q3Vector3D_Scale( &inData->minorRadius, sinAngle, &radialVec );
			Q3Vector3D_Add( &workVec, &radialVec, &radialVec );
			Q3Vector3D_Scale( &radialVec, 1.0f - inData->vMin, &workVec );
			Q3Point3D_Vector3D_Add( &bottomCenter, &workVec, &interiorPts[0] );
			interiorPts[1] = bottomCenter;
			interiorPts[3] = topCenter;
			Q3Vector3D_Scale( &radialVec, 1.0f - inData->vMax, &workVec );
			Q3Point3D_Vector3D_Add( &topCenter, &workVec, &interiorPts[2] );
			
			Q3Vector3D_Cross( &inData->orientation, &radialVec, &workVec );
			interiorPtNorms[0] = interiorPtNorms[1] = interiorPtNorms[2] = interiorPtNorms[3] =
				workVec;
			interiorFaceNorms[0] = interiorFaceNorms[1] = workVec;

			interiorUVs[0].u = 0.0f;
			interiorUVs[0].v = 0.0f;
			interiorUVs[1].u = 0.5f;
			interiorUVs[1].v = 0.0f;
			interiorUVs[2].u = 0.5f * (1.0f - (1.0f - inData->vMax) / (1.0f - inData->vMin));
			interiorUVs[2].v = 1.0f;
			interiorUVs[3].u = 0.5f;
			interiorUVs[3].v = 1.0f;
			Q3BoundingBox_SetFromPoints3D( &intTriMeshData.bBox, intTriMeshData.points,
				intTriMeshData.numPoints, sizeof(TQ3Point3D) );
			intGeom = Q3TriMesh_New( &intTriMeshData );
			if (intGeom != NULL)
			{
				E3TriMesh_AddTriangleNormals(intGeom, kQ3OrientationStyleCounterClockwise);
				Q3Group_AddObjectAndDispose( ioGroup, &intGeom );
			}
			
			// Right side
			cosAngle = (float) cos(startAngle);
			sinAngle = (float) sin(startAngle);
			Q3Vector3D_Scale( &inData->majorRadius, cosAngle, &workVec );
			Q3Vector3D_Scale( &inData->minorRadius, sinAngle, &radialVec );
			Q3Vector3D_Add( &workVec, &radialVec, &radialVec );
			Q3Vector3D_Scale( &radialVec, 1.0f - inData->vMin, &workVec );
			Q3Point3D_Vector3D_Add( &bottomCenter, &workVec, &interiorPts[1] );
			interiorPts[0] = bottomCenter;
			interiorPts[2] = topCenter;
			Q3Vector3D_Scale( &radialVec, 1.0f - inData->vMax, &workVec );
			Q3Point3D_Vector3D_Add( &topCenter, &workVec, &interiorPts[3] );

			Q3Vector3D_Cross( &radialVec, &inData->orientation, &workVec );
			interiorPtNorms[0] = interiorPtNorms[1] = interiorPtNorms[2] = interiorPtNorms[3] =
				workVec;
			interiorFaceNorms[0] = interiorFaceNorms[1] = workVec;

			interiorUVs[0].u = 0.5f;
			interiorUVs[0].v = 0.0f;
			interiorUVs[1].u = 1.0f;
			interiorUVs[1].v = 0.0f;
			interiorUVs[2].u = 0.5f;
			interiorUVs[2].v = 1.0f;
			interiorUVs[3].u = 0.5f * (1.0f + (1.0f - inData->vMax) / (1.0f - inData->vMin));
			interiorUVs[3].v = 1.0f;
			Q3BoundingBox_SetFromPoints3D( &intTriMeshData.bBox, intTriMeshData.points,
				intTriMeshData.numPoints, sizeof(TQ3Point3D) );
			intGeom = Q3TriMesh_New( &intTriMeshData );
			if (intGeom != NULL)
			{
				E3TriMesh_AddTriangleNormals(intGeom, kQ3OrientationStyleCounterClockwise);
				Q3Group_AddObjectAndDispose( ioGroup, &intGeom );
			}
		}
	}
}





//=============================================================================
//      e3geom_cone_cache_new : Cone cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_cone_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3ConeData *geomData)
{
#pragma unused( theGeom )
	float						uMin, uMax, vMin, vMax, dotCross;
	TQ3SubdivisionStyleData		subdivisionData;
	TQ3Boolean					isTipPresent;
	TQ3Uns32					sides = 10;
	TQ3Uns32					bands = 10;
	TQ3ConeData					faceData;
	TQ3StyleObject				theStyle;
	TQ3GroupObject				theGroup;
	TQ3Vector3D					workVec;



	// Get the subdivision style, to figure out how many sides we should have.
	if (Q3View_GetSubdivisionStyleState( theView, &subdivisionData ) == kQ3Success)
	{
		switch (subdivisionData.method)
		{
			case kQ3SubdivisionMethodConstant:
				sides = (TQ3Uns32) subdivisionData.c1;
				bands = (TQ3Uns32) subdivisionData.c2;
				break;
			
			case kQ3SubdivisionMethodWorldSpace:
				// Keep the length of a side less than or equal to c1
				{
					TQ3Matrix4x4	localToWorld;
					TQ3Vector3D		bigRadius;
					
					// Find the longer of the two radius vectors.
					bigRadius = geomData->majorRadius;
					if (Q3Vector3D_LengthSquared( &geomData->majorRadius ) <
						Q3Vector3D_LengthSquared( &geomData->minorRadius ) )
					{
						bigRadius = geomData->minorRadius;
					}
					
					// For number of sides in the u direction, divide an approximation
					// of the circumference by  c1.
					Q3View_GetLocalToWorldMatrixState( theView, &localToWorld );
					Q3Vector3D_Transform( &bigRadius, &localToWorld, &workVec );
					sides = (TQ3Uns32) ((kQ32Pi * Q3Vector3D_Length(&workVec))
						/ subdivisionData.c1);
					
					// For number of sides in the v direction, divide slant height by c1.
					Q3Vector3D_Subtract( &geomData->orientation, &bigRadius, &workVec );
					Q3Vector3D_Transform( &workVec, &localToWorld, &workVec );
					bands = (TQ3Uns32) (Q3Vector3D_Length( &workVec ) / subdivisionData.c1);
				}
				break;
			
			case kQ3SubdivisionMethodScreenSpace:
				// Not implemented
				break;
			
			case kQ3SubdivisionMethodSize32:
			default:
				Q3_ASSERT(!"Unknown subdivision method");
				break;
		}
	}
	sides = E3Num_Clamp( sides, 3, 256 );
	bands = E3Num_Clamp( bands, 1, 256 );



	// Create a group to hold the cached geometry
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
	{
		E3ErrorManager_PostError( kQ3ErrorOutOfMemory, kQ3False );
		return NULL;
	}



	// Add the orientation style
	//
	// All of the TriMeshes which form the cone have triangle normals created in a CCW style,
	// so we need to add an orientation to our group to ensure they are always treated as such.
	theStyle = Q3OrientationStyle_New(kQ3OrientationStyleCounterClockwise);
	Q3Group_AddObjectAndDispose(theGroup, &theStyle);



	// Add the cone attributes	
	if (geomData->coneAttributeSet != NULL)
		Q3Group_AddObject( theGroup, geomData->coneAttributeSet );



	// Test whether the geometry is degenerate.
	// This takes a bit of care.  One might think it would suffice to compute
	// the triple product of the 3 axis vectors.  But if they are somewhat
	// small, say length 0.003, and orthogonal, then the triple product will
	// be less than kQ3RealZero.
	if ( (Q3Vector3D_Length(&geomData->majorRadius) < kQ3RealZero) ||
		(Q3Vector3D_Length(&geomData->minorRadius) < kQ3RealZero) ||
		(Q3Vector3D_Length(&geomData->orientation) < kQ3RealZero) )
	{
		E3ErrorManager_PostError( kQ3ErrorDegenerateGeometry, kQ3False );
		return theGroup;
	}
	else
	{
		TQ3Vector3D		majNorm, minNorm, orientNorm, majXMinor;
		
		Q3Vector3D_Normalize( &geomData->majorRadius, &majNorm );
		Q3Vector3D_Normalize( &geomData->minorRadius, &minNorm );
		Q3Vector3D_Normalize( &geomData->orientation, &orientNorm );
		Q3Vector3D_Cross( &majNorm, &minNorm, &majXMinor );
		dotCross = Q3Vector3D_Dot( &orientNorm, &majXMinor );
		if (E3Float_Abs( dotCross ) < kQ3RealZero)
		{
			E3ErrorManager_PostError( kQ3ErrorDegenerateGeometry, kQ3False );
			return theGroup;
		}
	}



	// Get the UV limits and make sure they are valid
	uMin  = E3Num_Clamp( geomData->uMin, 0.0f, 1.0f );
	uMax  = E3Num_Clamp( geomData->uMax, 0.0f, 1.0f );
	vMin  = E3Num_Clamp( geomData->vMin, 0.0f, 1.0f );
	vMax  = E3Num_Clamp( geomData->vMax, 0.0f, 1.0f );
	// It is possible for uMin to be greater than uMax, so that
	// we can specify which way to wrap around the circle.
	// But it doesn't make sense for vMin to be greater than vMax.
	if (vMin > vMax)
		E3Float_Swap( vMin, vMax );



	// Another sort of degeneracy...
	if ( (E3Float_Abs( uMin - uMax ) <= kQ3RealZero) || (E3Float_Abs( vMin - vMax ) <= kQ3RealZero) )
	{
		E3ErrorManager_PostError( kQ3ErrorDegenerateGeometry, kQ3False );
		return theGroup;
	}



	// Turn a wraparound u interval into an increasing interval.
	if (uMin > uMax)
	{
		uMax += 1.0f;
	}



	// If the tip is included, the last band has only one triangle per side.
	isTipPresent = (1.0f - vMax <= kQ3RealZero)? kQ3True : kQ3False;

	
	
	// Create the face part(s)
	faceData = *geomData;
	faceData.uMin = uMin;
	faceData.uMax = uMax;
	faceData.vMin = vMin;
	faceData.vMax = vMax;
	e3geom_cone_create_face( theGroup, &faceData, sides, bands, isTipPresent );



	// Add the bottom cap, if any.
	if (geomData->caps & kQ3EndCapMaskBottom)
	{
		TQ3DiskData			botDisk;
		TQ3GeometryObject	botGeom;
		TQ3Vector3D			workVec;
		
		Q3Vector3D_Scale( &geomData->orientation, vMin, &workVec );
		Q3Point3D_Vector3D_Add( &geomData->origin, &workVec, &botDisk.origin );
		// In order to make the bottom be the front face, we flip the orientation.
		Q3Vector3D_Scale( &geomData->majorRadius, 1.0f - vMin, &botDisk.majorRadius );
		Q3Vector3D_Scale( &geomData->minorRadius, vMin - 1.0f, &botDisk.minorRadius );
		botDisk.uMin = 1.0f - geomData->uMax;
		botDisk.uMax = 1.0f - geomData->uMin;
		botDisk.vMin = 0.0f;
		botDisk.vMax = 1.0f;
		botDisk.diskAttributeSet = geomData->bottomAttributeSet;
		
		botGeom = Q3Disk_New( &botDisk );
		if (botGeom != NULL)
		{
			Q3Group_AddObjectAndDispose(theGroup, &botGeom);
		}
	}



	// If the interior cap flag is set, we might have one or two more pieces.
	if (geomData->caps & kQ3EndCapMaskInterior)
	{
		TQ3ConeData		intData;
		
		intData = *geomData;
		intData.uMin = uMin;
		intData.uMax = uMax;
		intData.vMin = vMin;
		intData.vMax = vMax;
		
		e3geom_cone_create_interior( theGroup, &intData, isTipPresent );
	}



	// Return the cached geometry
	return (theGroup);
}





//=============================================================================
//      e3geom_cone_get_attribute : Cone get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_cone_get_attribute(TQ3GeometryObject theObject)
{	TQ3ConeData		*instanceData = (TQ3ConeData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->coneAttributeSet);
}





//=============================================================================
//      e3geom_cone_metahandler : Cone metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_cone_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_cone_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_cone_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_cone_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_cone_cache_new;
			break;

		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_cone_get_attribute;
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
//      E3GeometryCone_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryCone_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeCone,
											kQ3ClassNameGeometryCone,
											e3geom_cone_metahandler,
											sizeof(TQ3ConeData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryCone_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryCone_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeCone, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Cone_New : Create a cone object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Cone_New(const TQ3ConeData *coneData)
{	TQ3Object		theObject;



	if (coneData == NULL)
	{
		TQ3ConeData		defaultCone = {
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			0.0f, 1.0f, 0.0f, 1.0f,
			kQ3EndCapNone,
			NULL, NULL, NULL, NULL
		};
		theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeCone, kQ3False, &defaultCone);
	}
	else
	{
		theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeCone, kQ3False, coneData);
	}

	return(theObject);
}





//=============================================================================
//      E3Cone_Submit : Submit a cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_Submit(const TQ3ConeData *coneData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeCone, coneData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Cone_SetData : One-line description of the method.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetData(TQ3GeometryObject theCone, const TQ3ConeData *coneData)
{	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;
	TQ3Status		qd3dStatus;



	// first, free the old data
	e3geom_cone_disposedata(instanceData);



	// then copy in the new data
	qd3dStatus = e3geom_cone_copydata(coneData, instanceData, kQ3False);
	Q3Shared_Edited(theCone);

	return(qd3dStatus);
}





//=============================================================================
//      E3Cone_GetData : Get a copy of the cone's internal data.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetData(TQ3GeometryObject theCone, TQ3ConeData *coneData)
{	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;
	TQ3Status		qd3dStatus;



	// Copy the data out of the Cone
	coneData->interiorAttributeSet = NULL;
	coneData->faceAttributeSet = NULL;
	coneData->bottomAttributeSet = NULL;
	coneData->coneAttributeSet = NULL;
	qd3dStatus = e3geom_cone_copydata(instanceData, coneData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      E3Cone_SetOrigin : Set the cone origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetOrigin(TQ3GeometryObject theCone, const TQ3Point3D *origin)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	Q3Memory_Copy( origin, &instanceData->origin, sizeof(TQ3Point3D) );

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetOrientation : Set the cone orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetOrientation(TQ3GeometryObject theCone, const TQ3Vector3D *orientation)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	Q3Memory_Copy( orientation, &instanceData->orientation, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetMajorRadius : Set the cone major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetMajorRadius(TQ3GeometryObject theCone, const TQ3Vector3D *majorRadius)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	Q3Memory_Copy( majorRadius, &instanceData->majorRadius, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetMinorRadius : Set the cone minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetMinorRadius(TQ3GeometryObject theCone, const TQ3Vector3D *minorRadius)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	Q3Memory_Copy( minorRadius, &instanceData->minorRadius, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetOrigin : Get the cone's origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetOrigin(TQ3GeometryObject theCone, TQ3Point3D *origin)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	Q3Memory_Copy( &instanceData->origin, origin, sizeof(TQ3Point3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetOrientation : Get the cone orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetOrientation(TQ3GeometryObject theCone, TQ3Vector3D *orientation)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	Q3Memory_Copy( &instanceData->orientation, orientation, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetMajorRadius : Get the cone's major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetMajorRadius(TQ3GeometryObject theCone, TQ3Vector3D *majorRadius)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	Q3Memory_Copy( &instanceData->majorRadius, majorRadius, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetMinorRadius : Get the cone's minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetMinorRadius(TQ3GeometryObject theCone, TQ3Vector3D *minorRadius)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	Q3Memory_Copy( &instanceData->minorRadius, minorRadius, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetCaps : Set the cone endcap options.
//-----------------------------------------------------------------------------
//		Note :	The kQ3EndCapMaskTop value is not valid for cones - perhaps
//				post a warning if this is set?
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetCaps(TQ3GeometryObject theCone, TQ3EndCap caps)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	instanceData->caps = caps;

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetCaps : Get the cone endcap options.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetCaps(TQ3GeometryObject theCone, TQ3EndCap *caps)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	*caps = instanceData->caps;
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetBottomAttributeSet :	Set the attribute set used for
//										the bottom of the cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetBottomAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet bottomAttributeSet)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	E3Shared_Replace(&instanceData->bottomAttributeSet, bottomAttributeSet);

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetBottomAttributeSet :	Get the attribute set used for
//										the bottom of the cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetBottomAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet *bottomAttributeSet)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	E3Shared_Acquire(bottomAttributeSet, instanceData->bottomAttributeSet);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetFaceAttributeSet : Set the attribute set used for
//									 the sides (?) of the cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetFaceAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet faceAttributeSet)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	E3Shared_Replace(&instanceData->faceAttributeSet, faceAttributeSet);

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetFaceAttributeSet : Get the attribute set used for
//									 the sides (?) of the cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetFaceAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet *faceAttributeSet)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	E3Shared_Acquire(faceAttributeSet, instanceData->faceAttributeSet);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetInteriorAttributeSet : Set the attribute set used for
//									 the interior of the cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetInteriorAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet intAttributeSet)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	E3Shared_Replace(&instanceData->interiorAttributeSet, intAttributeSet);

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetInteriorAttributeSet : Get the attribute set used for
//									 the interior of the cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetInteriorAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet *intAttributeSet)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	E3Shared_Acquire(intAttributeSet, instanceData->interiorAttributeSet);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_EmptyData : Empty the data for a Cone object.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_EmptyData(TQ3ConeData *coneData)
{

	// Dispose of the data
	e3geom_cone_disposedata(coneData);

	return(kQ3Success);
}


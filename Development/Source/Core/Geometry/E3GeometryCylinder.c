/*  NAME:
        E3GeometryCylinder.c

    DESCRIPTION:
        Implementation of Quesa Cylinder geometry class.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
#include "E3GeometryCylinder.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Cylinder : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypeCylinder, E3Cylinder, E3Geometry )
public :

	TQ3CylinderData			instanceData ;

	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_cylinder_copydata : Copy TQ3CylinderData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cylinder_copydata(const TQ3CylinderData *src, TQ3CylinderData *dst, TQ3Boolean isDuplicate)
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

		if (src->topAttributeSet != NULL)
		{
			dst->topAttributeSet = Q3Object_Duplicate(src->topAttributeSet);
			if (dst->topAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->topAttributeSet = NULL;

		if (src->bottomAttributeSet != NULL)
		{
			dst->bottomAttributeSet = Q3Object_Duplicate(src->bottomAttributeSet);
			if (dst->bottomAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->bottomAttributeSet = NULL;

		if (src->cylinderAttributeSet != NULL)
		{
			dst->cylinderAttributeSet = Q3Object_Duplicate(src->cylinderAttributeSet);
			if (dst->cylinderAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->cylinderAttributeSet = NULL;
	}
	else {
		E3Shared_Replace(&dst->interiorAttributeSet, src->interiorAttributeSet);
		E3Shared_Replace(&dst->faceAttributeSet, src->faceAttributeSet);
		E3Shared_Replace(&dst->topAttributeSet, src->topAttributeSet);
		E3Shared_Replace(&dst->bottomAttributeSet, src->bottomAttributeSet);
		E3Shared_Replace(&dst->cylinderAttributeSet, src->cylinderAttributeSet);
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_cylinder_disposedata : Dispose of a TQ3CylinderData.
//-----------------------------------------------------------------------------
static void
e3geom_cylinder_disposedata(TQ3CylinderData *theCylinder)
{
	Q3Object_CleanDispose(&theCylinder->interiorAttributeSet );
	Q3Object_CleanDispose(&theCylinder->faceAttributeSet );
	Q3Object_CleanDispose(&theCylinder->topAttributeSet );
	Q3Object_CleanDispose(&theCylinder->bottomAttributeSet );
	Q3Object_CleanDispose(&theCylinder->cylinderAttributeSet );
}	





//=============================================================================
//      e3geom_cylinder_new : Cylinder new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cylinder_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3CylinderData			*instanceData = (TQ3CylinderData *)       privateData;
	const TQ3CylinderData	*cylinderData     = (const TQ3CylinderData *) paramData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)

	qd3dStatus = e3geom_cylinder_copydata(cylinderData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_cylinder_delete : Cylinder delete method.
//-----------------------------------------------------------------------------
static void
e3geom_cylinder_delete(TQ3Object theObject, void *privateData)
{	TQ3CylinderData		*instanceData = (TQ3CylinderData *) privateData;
#pragma unused(theObject)

	// Dispose of our instance data
	e3geom_cylinder_disposedata(instanceData);
}





//=============================================================================
//      e3geom_cylinder_duplicate : Cylinder duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cylinder_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TQ3CylinderData	*fromInstanceData = (const TQ3CylinderData *) fromPrivateData;
	TQ3CylinderData			*toInstanceData   = (TQ3CylinderData *)       toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_cylinder_copydata( fromInstanceData, toInstanceData, kQ3True );

	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_cylinder_disposedata(toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_cylinder_cache_new : Cylinder cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_cylinder_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3CylinderData *geomData)
{	float						ang=0.0f, dang, cosAngle, sinAngle;
	float						startAngle, endAngle, angleRange;
	TQ3Vector3D					orientCrossMaj, orientCrossMin;
	float						uMin, uMax, vMin, vMax;
	TQ3Point3D					bottomCenter, topCenter;
	TQ3TriMeshAttributeData		vertexAttributes[2];
	TQ3Boolean					isPartAngleRange;
	TQ3SubdivisionStyleData		subdivisionData;
	TQ3Vector3D					workVec, sideVec;
	TQ3TriMeshData				triMeshData;
	TQ3GeometryObject			theTriMesh;
	TQ3Uns32					sides = 10;
	TQ3TriMeshTriangleData		*triangles;
	TQ3Uns32					numpoints;
	TQ3Vector3D					*normals;
	TQ3StyleObject				theStyle;
	TQ3GroupObject				theGroup;
	TQ3Point3D					*points;
	TQ3Param2D					*uvs;
	TQ3Uns32					i;
	TQ3Vector3D					v;



	// Get the UV limits and make sure they are valid
	uMin  = E3Num_Clamp(geomData->uMin, 0.0f, 1.0f);
	uMax  = E3Num_Clamp(geomData->uMax, 0.0f, 1.0f);
	vMin  = E3Num_Clamp(geomData->vMin, 0.0f, 1.0f);
	vMax  = E3Num_Clamp(geomData->vMax, 0.0f, 1.0f);
	// It is possible for uMin to be greater than uMax, so that
	// we can specify which way to wrap around the circle.
	// But it doesn't make sense for vMin to be greater than vMax.
	if (vMin > vMax)
		E3Float_Swap( vMin, vMax );
	
	
	
	// Turn the u limits into an angle range in radians.
	startAngle = uMin * kQ32Pi;
	endAngle   = uMax * kQ32Pi;
	if (startAngle > endAngle)
		startAngle -= kQ32Pi;
	angleRange = endAngle - startAngle;
	isPartAngleRange = E3Float_Abs( angleRange - kQ32Pi ) > kQ3RealZero?
		kQ3True : kQ3False;



	// Find the center of the top and the bottom.
	Q3Vector3D_Scale( &geomData->orientation, vMin, &workVec );
	Q3Point3D_Vector3D_Add( &geomData->origin, &workVec, &bottomCenter );
	Q3Vector3D_Scale( &geomData->orientation, vMax - vMin, &sideVec );
	Q3Point3D_Vector3D_Add( &bottomCenter, &sideVec, &topCenter );
	


	// Get the subdivision style, to figure out how many sides we should have.
	if (Q3View_GetSubdivisionStyleState( theView, &subdivisionData ) == kQ3Success) {
		switch (subdivisionData.method) {
			case kQ3SubdivisionMethodConstant:
				// for a cylinder, parameter c1 is the number of sides and c2 is unused
				sides = (TQ3Uns32) subdivisionData.c1;
				break;
			
			case kQ3SubdivisionMethodWorldSpace:
				// keep the length of any side less than or equal to c1
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

					// divide the circumference by c1
					Q3View_GetLocalToWorldMatrixState( theView, &localToWorld );
					Q3Vector3D_Transform( &bigRadius, &localToWorld, &workVec );
					sides = (TQ3Uns32) ((kQ32Pi * Q3Vector3D_Length(&workVec))
							/ subdivisionData.c1);
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
	sides = E3Num_Clamp(sides, 3, 256);	// sanity checking

	numpoints = sides*2;


	// In order to have proper uv parameterization we need an extra set of vertices
	// for the closing triangle (since it is not easy to convince any renderer to
	// have multiple uvs for a vertex)
	//
	// So, we add on 2 more points to hold these two duplicate vertices
	numpoints += 2;



	// Create a group to hold the cached geometry
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
	{
		E3ErrorManager_PostError( kQ3ErrorOutOfMemory, kQ3False );
		return NULL;
	}



	// Add the orientation style
	//
	// All of the TriMeshes which form the cylinder have triangle normals created in a CCW style,
	// so we need to add an orientation to our group to ensure they are always treated as such.
	theStyle = Q3OrientationStyle_New(kQ3OrientationStyleCounterClockwise);
	Q3Group_AddObjectAndDispose(theGroup, &theStyle);



	// Add the cone attributes	
	if (geomData->cylinderAttributeSet != NULL)
		Q3Group_AddObject( theGroup, geomData->cylinderAttributeSet );



	// Allocate some memory for the TriMesh
	points    = (TQ3Point3D *)             Q3Memory_Allocate(numpoints*sizeof(TQ3Point3D) );
	normals   = (TQ3Vector3D *)            Q3Memory_Allocate(numpoints*sizeof(TQ3Vector3D) );
	uvs       = (TQ3Param2D  *)            Q3Memory_Allocate(numpoints*sizeof(TQ3Param2D));
	triangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate( 2*sides*sizeof(TQ3TriMeshTriangleData) );

	if (points == NULL || normals == NULL || uvs == NULL || triangles == NULL)
		{
		Q3Memory_Free(&points);
		Q3Memory_Free(&normals);
		Q3Memory_Free(&uvs);
		Q3Memory_Free(&triangles);
		
		return(theGroup);
		}



	// now, define each side, as a cosine/sine combination of major and minor radius vectors
	// (also compute vertex normals while we're at it)
	
	// A cylinder has a parametric equation
	// f(s,t) = origin + s*orientation + cos(t)majorRadius + sin(t)minorRadius .
	// We can derive a normal vector by taking the cross product of the two partial
	// derivatives:
	// (partial f with respect to s) x (partial f with respect to t) =
	// 		orientation x (-sin(t)majorRadius + cos(t)minorRadius) =
	//		-sin(t)(orientation x majorRadius) + cos(t)(orientation x minorRadius) .
	// If (majorRadius, minorRadius, orientation) forms a right-handed system, then
	// this normal points inward, so will need to be negated.
	Q3Vector3D_Cross( &geomData->orientation, &geomData->minorRadius, &orientCrossMin );
	Q3Vector3D_Cross( &geomData->orientation, &geomData->majorRadius, &orientCrossMaj );
	
	
	
	// Test whether the geometry is degenerate.
	if (E3Geometry_IsDegenerateTriple( &geomData->orientation, &geomData->majorRadius,
		&geomData->minorRadius ))
	{
		E3ErrorManager_PostError( kQ3ErrorDegenerateGeometry, kQ3False );
		return theGroup;
	}



	// Compute points, normals, UVs, and triangles for each side
	dang = angleRange / sides;
	
	for (i=0, ang = startAngle; i <= sides; ++i, ang += dang)
	{
		cosAngle = (float) cos(ang);
		sinAngle = (float) sin(ang);

		// bottom point is bottomCenter + cos(major) + sin(minor)
		Q3Vector3D_Scale( &geomData->majorRadius, cosAngle, &v );
		Q3Point3D_Vector3D_Add( &bottomCenter, &v, &points[i] );
		Q3Vector3D_Scale( &geomData->minorRadius, sinAngle, &v );
		Q3Point3D_Vector3D_Add( &points[i], &v, &points[i] );

		// compute normal of bottom point
		Q3Vector3D_Scale( &orientCrossMaj, -sinAngle, &normals[i] );
		Q3Vector3D_Scale( &orientCrossMin, cosAngle, &v );
		Q3Vector3D_Add( &normals[i], &v, &normals[i] );
		Q3Vector3D_Negate( &normals[i], &normals[i] );

		// corresponding top point is bottom point + sideVec
		Q3Point3D_Vector3D_Add( &points[i], &sideVec, &points[i+sides+1] );

		// and its normal is exactly the same as the bottom one
		normals[i+sides+1] = normals[i];		

		// uvs come from the surface parameterisation
		uvs[i].u         = i / (float) sides;
		uvs[i].v         = 0.0f;
		uvs[i + sides].u = uvs[i].u;
		uvs[i + sides].v = 1.0f;

		if (i<sides)
		{
			// make the triangle with point up
			triangles[i].pointIndices[0] = i+sides+1;
			triangles[i].pointIndices[1] = i;
			triangles[i].pointIndices[2] = i+1;
			
			// make the triangle with point down
			triangles[i+sides].pointIndices[0] = i+sides+1;
			triangles[i+sides].pointIndices[1] = i+1;
			triangles[i+sides].pointIndices[2] = i+sides+2;
		}
	}



	// set up remaining trimesh data
	vertexAttributes[0].attributeType     = kQ3AttributeTypeNormal;
	vertexAttributes[0].data              = normals;
	vertexAttributes[0].attributeUseArray = NULL;

	vertexAttributes[1].attributeType     = kQ3AttributeTypeSurfaceUV;
	vertexAttributes[1].data              = uvs;
	vertexAttributes[1].attributeUseArray = NULL;

	triMeshData.numPoints                 = numpoints;
	triMeshData.points                    = points;
	triMeshData.numTriangles              = 2*sides;
	triMeshData.triangles                 = triangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = 0;
	triMeshData.edges                     = NULL;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 2;
	triMeshData.vertexAttributeTypes      = vertexAttributes;
	triMeshData.triMeshAttributeSet = geomData->faceAttributeSet;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox,
									triMeshData.points,
									numpoints,
									sizeof(TQ3Point3D));



	// finally, create the TriMesh and add to the group
	theTriMesh = Q3TriMesh_New(&triMeshData);
	if (theTriMesh != NULL)
		{
		E3TriMesh_AddTriangleNormals(theTriMesh, kQ3OrientationStyleCounterClockwise);
		Q3Group_AddObjectAndDispose(theGroup, &theTriMesh);
		}



	// Add the bottom end cap, if any, being careful about the orientation.
	if (geomData->caps & kQ3EndCapMaskBottom)
	{
		TQ3DiskData			botDisk;
		TQ3GeometryObject	botGeom;
		
		botDisk.origin = bottomCenter;
		botDisk.majorRadius = geomData->majorRadius;
		Q3Vector3D_Negate( &geomData->minorRadius, &botDisk.minorRadius );
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



	// Add the top end cap, if any
	if (geomData->caps & kQ3EndCapMaskTop)
	{
		TQ3DiskData			topDisk;
		TQ3GeometryObject	topGeom;
		
		topDisk.origin = topCenter;
		topDisk.majorRadius = geomData->majorRadius;
		topDisk.minorRadius = geomData->minorRadius;
		topDisk.uMin = geomData->uMin;
		topDisk.uMax = geomData->uMax;
		topDisk.vMin = 0.0f;
		topDisk.vMax = 1.0f;
		topDisk.diskAttributeSet = geomData->topAttributeSet;
		
		topGeom = Q3Disk_New( &topDisk );
		if (topGeom != NULL)
		{
			Q3Group_AddObjectAndDispose(theGroup, &topGeom);
		}
	}
	
	
	
	// Add interior cap if appropriate
	if ( isPartAngleRange && ((geomData->caps & kQ3EndCapMaskInterior) != 0) )
	{
		// The interior looks like a folded rectangle.  In order to have a
		// sharp edge along the fold, we must use two TriMeshes, each of
		// which is a rectangle.
		TQ3Point3D		interiorPts[4];
		TQ3Vector3D		interiorPtNorms[4];
		TQ3Param2D		interiorUVs[4];
		TQ3TriMeshTriangleData	interiorTris[2] = {
			{ { 0, 3, 1 } }, { { 0, 2, 3 } }
		};
		TQ3TriMeshAttributeData	interiorPtAtts[2] = {
			{ kQ3AttributeTypeNormal, NULL, NULL },
			{ kQ3AttributeTypeSurfaceUV, NULL, NULL }
		};
		TQ3TriMeshData	intTriMeshData;
		TQ3GeometryObject	intGeom = NULL;
		
		// First half of interior, from ending edge of face to center line.
		ang = endAngle;
		cosAngle = (float) cos(ang);
		sinAngle = (float) sin(ang);

		// Define points
		Q3Vector3D_Scale( &geomData->majorRadius, cosAngle, &v );
		Q3Vector3D_Scale( &geomData->minorRadius, sinAngle, &workVec );
		Q3Vector3D_Add( &v, &workVec, &v );
		Q3Point3D_Vector3D_Add( &bottomCenter, &v, &interiorPts[0] );
		Q3Point3D_Vector3D_Add( &interiorPts[0], &sideVec, &interiorPts[1] );
		interiorPts[2] = bottomCenter;
		Q3Point3D_Vector3D_Add( &interiorPts[2], &sideVec, &interiorPts[3] );
		
		// Define normal
		Q3Vector3D_Cross( &sideVec, &v, &workVec );
		interiorPtNorms[0] = interiorPtNorms[1] = interiorPtNorms[2] =
			interiorPtNorms[3] = workVec;
		
		// Define surface UVs
		interiorUVs[0].u = 0.0f;
		interiorUVs[0].v = 0.0f;
		interiorUVs[1].u = 0.0f;
		interiorUVs[1].v = 1.0f;
		interiorUVs[2].u = 0.5f;
		interiorUVs[2].v = 0.0f;
		interiorUVs[3].u = 0.5f;
		interiorUVs[3].v = 1.0f;
		
		// Set up TriMesh data
		interiorPtAtts[0].data = interiorPtNorms;
		interiorPtAtts[1].data = interiorUVs;
		intTriMeshData.triMeshAttributeSet = geomData->interiorAttributeSet;
		intTriMeshData.numTriangles = 2;
		intTriMeshData.triangles = interiorTris;
		intTriMeshData.numTriangleAttributeTypes = 0;
		intTriMeshData.triangleAttributeTypes = NULL;
		intTriMeshData.numEdges = 0;
		intTriMeshData.edges = NULL;
		intTriMeshData.numEdgeAttributeTypes = 0;
		intTriMeshData.edgeAttributeTypes = NULL;
		intTriMeshData.numPoints = 4;
		intTriMeshData.points = interiorPts;
		intTriMeshData.numVertexAttributeTypes = 2;
		intTriMeshData.vertexAttributeTypes = interiorPtAtts;
		Q3BoundingBox_SetFromPoints3D( &intTriMeshData.bBox,
									intTriMeshData.points,
									4,
									sizeof(TQ3Point3D));
		
		// Make the first part of the interior
		intGeom = Q3TriMesh_New( &intTriMeshData );
		if (intGeom != NULL)
			{
			E3TriMesh_AddTriangleNormals(intGeom, kQ3OrientationStyleCounterClockwise);
			Q3Group_AddObjectAndDispose(theGroup, &intGeom);
			}
		
		// Second part of interior, center to start edge
		ang = startAngle;
		cosAngle = (float) cos(ang);
		sinAngle = (float) sin(ang);
		interiorPts[0] = bottomCenter;
		Q3Point3D_Vector3D_Add( &interiorPts[0], &sideVec, &interiorPts[1] );
		Q3Vector3D_Scale( &geomData->majorRadius, cosAngle, &v );
		Q3Vector3D_Scale( &geomData->minorRadius, sinAngle, &workVec );
		Q3Vector3D_Add( &v, &workVec, &v );
		Q3Point3D_Vector3D_Add( &bottomCenter, &v, &interiorPts[2] );
		Q3Point3D_Vector3D_Add( &interiorPts[2], &sideVec, &interiorPts[3] );
		
		// Define normal
		Q3Vector3D_Cross( &sideVec, &v, &workVec );
		interiorPtNorms[0] = interiorPtNorms[1] = interiorPtNorms[2] =
			interiorPtNorms[3] = workVec;
		
		// update surface UVs
		interiorUVs[0].u = 0.5f;
		interiorUVs[1].u = 0.5f;
		interiorUVs[2].u = 1.0f;
		interiorUVs[3].u = 1.0f;
		
		// Make the second part of the interior
		intGeom = Q3TriMesh_New( &intTriMeshData );
		if (intGeom != NULL)
			{
			E3TriMesh_AddTriangleNormals(intGeom, kQ3OrientationStyleCounterClockwise);
			Q3Group_AddObjectAndDispose(theGroup, &intGeom);
			}
	}



	// Clean up
	Q3Memory_Free(&points);
	Q3Memory_Free(&normals);
	Q3Memory_Free(&uvs);
	Q3Memory_Free(&triangles);



	// Return the cached geometry
	return(theGroup);
}





//=============================================================================
//      e3geom_cylinder_get_attribute : Cylinder get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_cylinder_get_attribute ( E3Cylinder* cylinder )
	{
	// Return the address of the geometry attribute set
	return &cylinder->instanceData.cylinderAttributeSet ;
	}





//=============================================================================
//      e3geom_cylinder_metahandler : Cylinder metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_cylinder_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_cache_new;
			break;

		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_get_attribute;
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
//      E3GeometryCylinder_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryCylinder_RegisterClass(void)
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryCylinder,
								e3geom_cylinder_metahandler,
								E3Cylinder ) ;
	}





//=============================================================================
//      E3GeometryCylinder_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryCylinder_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypeCylinder, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Cylinder_New : Create a cylinder object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Cylinder_New(const TQ3CylinderData *cylinderData)
{	TQ3Object		theObject;



	if (cylinderData == NULL)
	{
		TQ3CylinderData	defaultCylinder = {
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			0.0f, 1.0f, 0.0f, 1.0f,
			kQ3EndCapNone,
			NULL, NULL, NULL, NULL, NULL
		};
		theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeCylinder, kQ3False, &defaultCylinder ) ;
	}
	else
	{
		theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeCylinder, kQ3False, cylinderData ) ;
	}


	return(theObject);
}





//=============================================================================
//      E3Cylinder_Submit : Submit a cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_Submit(const TQ3CylinderData *cylinderData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeCylinder, cylinderData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Cylinder_SetData : One-line description of the method.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetData(TQ3GeometryObject theCylinder, const TQ3CylinderData *cylinderData)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	// first, free the old data
	e3geom_cylinder_disposedata ( & cylinder->instanceData ) ;



	// then copy in the new data
	TQ3Status qd3dStatus = e3geom_cylinder_copydata ( cylinderData, & cylinder->instanceData, kQ3False ) ;
	Q3Shared_Edited ( cylinder ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3Cylinder_GetData : Get a copy of the cylinder's internal data.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetData(TQ3GeometryObject theCylinder, TQ3CylinderData *cylinderData)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	// Copy the data out of the Cylinder
	cylinderData->interiorAttributeSet = NULL;
	cylinderData->faceAttributeSet = NULL;
	cylinderData->topAttributeSet = NULL;
	cylinderData->bottomAttributeSet = NULL;
	cylinderData->cylinderAttributeSet = NULL;
	
	return e3geom_cylinder_copydata ( &cylinder->instanceData, cylinderData, kQ3False ) ;
	}





//=============================================================================
//      E3Cylinder_SetOrigin : Set the cylinder origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetOrigin(TQ3GeometryObject theCylinder, const TQ3Point3D *origin)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	Q3Memory_Copy ( origin, & cylinder->instanceData.origin, sizeof(TQ3Point3D) ) ;

	Q3Shared_Edited ( cylinder ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_SetOrientation : Set the cylinder orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetOrientation(TQ3GeometryObject theCylinder, const TQ3Vector3D *orientation)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	Q3Memory_Copy ( orientation, & cylinder->instanceData.orientation, sizeof(TQ3Vector3D) ) ;

	Q3Shared_Edited ( cylinder ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_SetMajorRadius : Set the cylinder major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetMajorRadius(TQ3GeometryObject theCylinder, const TQ3Vector3D *majorRadius)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	Q3Memory_Copy ( majorRadius, & cylinder->instanceData.majorRadius, sizeof(TQ3Vector3D) ) ;

	Q3Shared_Edited ( cylinder ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_SetMinorRadius : Set the cylinder minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetMinorRadius(TQ3GeometryObject theCylinder, const TQ3Vector3D *minorRadius)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	Q3Memory_Copy ( minorRadius, & cylinder->instanceData.minorRadius, sizeof(TQ3Vector3D) ) ;

	Q3Shared_Edited ( cylinder ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_GetOrigin : Get the cylinder's origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetOrigin(TQ3GeometryObject theCylinder, TQ3Point3D *origin)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	Q3Memory_Copy ( & cylinder->instanceData.origin, origin, sizeof(TQ3Point3D) ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_GetOrientation : Get the cylinder orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetOrientation(TQ3GeometryObject theCylinder, TQ3Vector3D *orientation)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	Q3Memory_Copy ( & cylinder->instanceData.orientation, orientation, sizeof(TQ3Vector3D) ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_GetMajorRadius : Get the cylinder's major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetMajorRadius(TQ3GeometryObject theCylinder, TQ3Vector3D *majorRadius)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	Q3Memory_Copy ( & cylinder->instanceData.majorRadius, majorRadius, sizeof(TQ3Vector3D) ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_GetMinorRadius : Get the cylinder's minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetMinorRadius(TQ3GeometryObject theCylinder, TQ3Vector3D *minorRadius)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	Q3Memory_Copy ( &cylinder->instanceData.minorRadius, minorRadius, sizeof(TQ3Vector3D) ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_SetCaps : Set the cylinder endcap options.
//-----------------------------------------------------------------------------
//		Note :	The kQ3EndCapMaskTop value is not valid for cylinders - perhaps
//				post a warning if this is set?
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetCaps(TQ3GeometryObject theCylinder, TQ3EndCap caps)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	cylinder->instanceData.caps = caps ;

	Q3Shared_Edited ( cylinder ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_GetCaps : Get the cylinder endcap options.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetCaps(TQ3GeometryObject theCylinder, TQ3EndCap *caps)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	*caps = cylinder->instanceData.caps ;
	return kQ3Success ;
	}



//=============================================================================
//      E3Cylinder_SetTopAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetTopAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet topAttributeSet)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	E3Shared_Replace ( & cylinder->instanceData.topAttributeSet, topAttributeSet ) ;

	Q3Shared_Edited ( cylinder ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_GetTopAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetTopAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet *topAttributeSet)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	E3Shared_Acquire ( topAttributeSet, cylinder->instanceData.topAttributeSet ) ;
	return kQ3Success ;
	}



//=============================================================================
//      E3Cylinder_SetBottomAttributeSet :	Set the attribute set used for
//										the bottom of the cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetBottomAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet bottomAttributeSet)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	E3Shared_Replace ( & cylinder->instanceData.bottomAttributeSet, bottomAttributeSet ) ;

	Q3Shared_Edited ( cylinder ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_GetBottomAttributeSet :	Get the attribute set used for
//										the bottom of the cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetBottomAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet *bottomAttributeSet)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	E3Shared_Acquire ( bottomAttributeSet, cylinder->instanceData.bottomAttributeSet ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_SetFaceAttributeSet : Set the attribute set used for
//									 the sides (?) of the cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetFaceAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet faceAttributeSet)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	E3Shared_Replace ( &cylinder->instanceData.faceAttributeSet, faceAttributeSet ) ;

	Q3Shared_Edited ( cylinder ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_GetFaceAttributeSet : Get the attribute set used for
//									 the sides (?) of the cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetFaceAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet *faceAttributeSet)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	E3Shared_Acquire ( faceAttributeSet, cylinder->instanceData.faceAttributeSet ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_SetInteriorAttributeSet : Set the attribute set used for
//									 the interior of the cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetInteriorAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet intAttributeSet)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	E3Shared_Replace ( &cylinder->instanceData.interiorAttributeSet, intAttributeSet ) ;

	Q3Shared_Edited ( cylinder ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_GetInteriorAttributeSet : Get the attribute set used for
//									 the interior of the cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetInteriorAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet *intAttributeSet)
	{
	E3Cylinder* cylinder = (E3Cylinder*) theCylinder ;

	E3Shared_Acquire ( intAttributeSet, cylinder->instanceData.interiorAttributeSet ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Cylinder_EmptyData : Empty the data for a Cylinder object.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_EmptyData(TQ3CylinderData *cylinderData)
{

	// Dispose of the data
	e3geom_cylinder_disposedata(cylinderData);

	return(kQ3Success);
}


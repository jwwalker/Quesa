/*  NAME:
        E3GeometryTriMesh.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3Prefix.h"
#include "E3View.h"
#include "E3Pick.h"
#include "E3Set.h"
#include "E3Math.h"
#include "E3Geometry.h"
#include "E3GeometryTriMesh.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_trimesh_clone : Clone a block of memory.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_clone(void *srcPtr, void **dstPtr, TQ3Uns32 theSize)
{


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(srcPtr  != NULL, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(dstPtr  != NULL, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(theSize != 0,    kQ3Failure);



	// Alocate the memory	
	*dstPtr = Q3Memory_Allocate(theSize);
	if (*dstPtr == NULL)
		return(kQ3Failure);



	// Copy it over
	Q3Memory_Copy(srcPtr, *dstPtr, theSize);
	
	return(kQ3Success);
}





//=============================================================================
//		e3geom_trimesh_disposeattributes: Free a TQ3TriMeshAttributeData array.
//-----------------------------------------------------------------------------
static void
e3geom_trimesh_disposeattributes(TQ3Uns32						numAttributeTypes,
									TQ3TriMeshAttributeData		**attributeTypes)
{
	TQ3Uns32 i;
	
	if (*attributeTypes != NULL)
		{
		for (i=0; i<numAttributeTypes; i++)
			{
			Q3Memory_Free( &((*attributeTypes)[i].data) );
			Q3Memory_Free( &((*attributeTypes)[i].attributeUseArray) );
			}
		}

	Q3Memory_Free( attributeTypes );
}





//=============================================================================
//      e3geom_trimesh_copyattributes : Copy a TQ3TriMeshAttributeData array.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_copyattributes(TQ3Uns32						numAttributeTypes,
								TQ3Uns32					numElements,
								TQ3TriMeshAttributeData		*srcAttributeTypes,
								TQ3TriMeshAttributeData		**destAttributeTypes)
{
	TQ3Uns32				i, bytes, attrSize;	
	TQ3Status				qd3dStatus;
	TQ3AttributeType		attrType;
	E3ClassInfoPtr			theClass;

	if (numAttributeTypes < 1) {
		*destAttributeTypes = NULL;
		return(kQ3Success);
	}
	
	qd3dStatus = e3geom_trimesh_clone(srcAttributeTypes,
									  (void **) destAttributeTypes,
									  numAttributeTypes * sizeof(TQ3TriMeshAttributeData));
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);

	for (i=0; i<numAttributeTypes && qd3dStatus == kQ3Success; i++)
		{
		// We must make a copy of the data elements; but first, we must figure
		// out how big they are (depends on the attribute type)
		attrType = srcAttributeTypes[i].attributeType;
		attrType = E3Attribute_AttributeToClassType(attrType);
		theClass = E3ClassTree_GetClassByType(attrType);
		if (theClass != NULL)
			{
			// Copy the attribute data
			attrSize = E3ClassTree_GetInstanceSize(theClass);
			bytes    = numElements * attrSize;
			if (bytes != 0 )
				qd3dStatus = e3geom_trimesh_clone(srcAttributeTypes[i].data,
												  &(*destAttributeTypes)[i].data,
												  bytes);
			else
				(*destAttributeTypes)[i].data = NULL;


			// Copy the custom attribute useage flags
			bytes = numElements * sizeof(char);
			if (bytes != 0 && srcAttributeTypes[i].attributeUseArray != NULL)
				qd3dStatus = e3geom_trimesh_clone(srcAttributeTypes[i].attributeUseArray,
												  (void **) &(*destAttributeTypes)[i].attributeUseArray,
												  bytes);
			else
				(*destAttributeTypes)[i].attributeUseArray = NULL;
			}
		}
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_disposedata : Dispose of a TQ3TriMeshData.
//-----------------------------------------------------------------------------
static void
e3geom_trimesh_disposedata(TQ3TriMeshData *theTriMesh)
{


	// Dispose of the TriMesh
	Q3Object_CleanDispose(&theTriMesh->triMeshAttributeSet );

	Q3Memory_Free( &theTriMesh->triangles );
	e3geom_trimesh_disposeattributes( theTriMesh->numTriangleAttributeTypes,
									  &theTriMesh->triangleAttributeTypes );

	Q3Memory_Free( &theTriMesh->edges );
	e3geom_trimesh_disposeattributes( theTriMesh->numEdgeAttributeTypes,
									  &theTriMesh->edgeAttributeTypes );

	Q3Memory_Free( &theTriMesh->points );
	e3geom_trimesh_disposeattributes( theTriMesh->numVertexAttributeTypes,
									  &theTriMesh->vertexAttributeTypes );
}





//=============================================================================
//      e3geom_trimesh_copydata : Copy TQ3TriMeshData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_copydata(const TQ3TriMeshData *src, TQ3TriMeshData *dst, TQ3Boolean isDuplicate)
{	TQ3Status		qd3dStatus = kQ3Success;
	TQ3Uns32		n;



	// 0. Initialise ourselves
	Q3Memory_Clear(dst, sizeof(TQ3TriMeshData));



	// 1. triMeshAttributeSet
	if (isDuplicate)
		{
		if (src->triMeshAttributeSet != NULL)
			{
			dst->triMeshAttributeSet = Q3Object_Duplicate(src->triMeshAttributeSet);
			if (dst->triMeshAttributeSet == NULL)
				qd3dStatus = kQ3Failure;
			}
		}
	else
		E3Shared_Replace(&dst->triMeshAttributeSet, src->triMeshAttributeSet);



	// 2. triangles
	if (qd3dStatus == kQ3Success)
		{
		n = src->numTriangles;
		if (n)
			{
			qd3dStatus = e3geom_trimesh_clone( src->triangles, (void **) &dst->triangles,
										       n * sizeof(TQ3TriMeshTriangleData) );
			if (qd3dStatus == kQ3Success)
				dst->numTriangles = n;
			}
		}



	// 3. triangleAttributeTypes
	if (qd3dStatus == kQ3Success)
		{
		qd3dStatus = e3geom_trimesh_copyattributes( src->numTriangleAttributeTypes, n,
							 					    src->triangleAttributeTypes,
												   &dst->triangleAttributeTypes );
		if (qd3dStatus == kQ3Success)
			dst->numTriangleAttributeTypes = src->numTriangleAttributeTypes;
		}



	// 4. edges
	if (qd3dStatus == kQ3Success)
		{
		n = src->numEdges;
		if (n)
			{
			qd3dStatus = e3geom_trimesh_clone( src->edges, (void **) &dst->edges,
										       n * sizeof(TQ3TriMeshEdgeData) );
			if (qd3dStatus == kQ3Success)
				dst->numEdges = n;
			}
		}



	// 5. edgeAttributesTypes
	if (qd3dStatus == kQ3Success)
		{
		qd3dStatus = e3geom_trimesh_copyattributes( src->numEdgeAttributeTypes, n,
												    src->edgeAttributeTypes,
												   &dst->edgeAttributeTypes );	
		if (qd3dStatus == kQ3Success)
			dst->numEdgeAttributeTypes = src->numEdgeAttributeTypes;
		}



	// 6. points (vertices)
	if (qd3dStatus == kQ3Success)
		{
		n = src->numPoints;
		if (n)
			{
			qd3dStatus = e3geom_trimesh_clone( src->points, (void **) &dst->points, n * sizeof(TQ3Point3D) );
			if (qd3dStatus == kQ3Success)
				dst->numPoints = n;
			}
		}



	// 7. vertexAttributeTypes
	if (qd3dStatus == kQ3Success)
		{
		qd3dStatus = e3geom_trimesh_copyattributes( src->numVertexAttributeTypes, n,
												    src->vertexAttributeTypes,
												   &dst->vertexAttributeTypes );
		if (qd3dStatus == kQ3Success)
			dst->numVertexAttributeTypes = src->numVertexAttributeTypes;
		}



	// 8. bounding box
	if (qd3dStatus == kQ3Success)
		{
        if (src->bBox.isEmpty)
            Q3BoundingBox_SetFromPoints3D(&dst->bBox,
                                           dst->points,
                                           dst->numPoints,
                                           sizeof(TQ3Point3D));
        else
            Q3BoundingBox_Copy(&src->bBox, &dst->bBox);
		}



	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_trimesh_disposedata(dst);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_new : TriMesh new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3TriMeshData			*instanceData  = (TQ3TriMeshData *)       privateData;
	const TQ3TriMeshData	*trimeshData  = (const TQ3TriMeshData *) paramData;
	TQ3Status				qd3dStatus;
#pragma unused(theObject)



	qd3dStatus = e3geom_trimesh_copydata(trimeshData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_delete : TriMesh delete method.
//-----------------------------------------------------------------------------
static void
e3geom_trimesh_delete(TQ3Object theObject, void *privateData)
{	TQ3TriMeshData		*instanceData = (TQ3TriMeshData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	e3geom_trimesh_disposedata(instanceData);
}





//=============================================================================
//      e3geom_trimesh_duplicate : TriMesh duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,  void       *toPrivateData)
{	const TQ3TriMeshData		*fromData = (const TQ3TriMeshData *) fromPrivateData;
	TQ3TriMeshData				*toData   = (TQ3TriMeshData *)       toPrivateData;
	TQ3Status					qd3dStatus;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_trimesh_copydata( fromData, toData, kQ3True );

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_triangle_new : Retrieve a triangle from the TriMesh.
//-----------------------------------------------------------------------------
static void
e3geom_trimesh_triangle_new(const TQ3TriMeshData *theTriMesh, TQ3Uns32 theIndex, TQ3TriangleData *theTriangle)
{	TQ3Uns32			n, m, vertIndex, attrSize;
	TQ3ObjectType		attrType;
	E3ClassInfoPtr		theClass;



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theTriMesh));
	Q3_REQUIRE(theIndex >= 0 && theIndex < theTriMesh->numTriangles);
	Q3_REQUIRE(Q3_VALID_PTR(theTriangle));



	// Set up the triangle
	Q3Memory_Clear(theTriangle, sizeof(TQ3TriangleData));
	E3Shared_Acquire(&theTriangle->triangleAttributeSet, theTriMesh->triMeshAttributeSet);



	// Set up the vertices
	for (n = 0; n < 3; n++)
		{
		// Set up the point
		vertIndex = theTriMesh->triangles[theIndex].pointIndices[n];
		theTriangle->vertices[n].point = theTriMesh->points[vertIndex];


		// Set up the attributes
		if (theTriMesh->numVertexAttributeTypes != 0)
			{
			Q3_ASSERT(Q3_VALID_PTR(theTriMesh->vertexAttributeTypes));
			theTriangle->vertices[n].attributeSet = Q3AttributeSet_New();
			if (theTriangle->vertices[n].attributeSet != NULL)
				{
				for (m = 0; m < theTriMesh->numVertexAttributeTypes; m++)
					{
					attrType = theTriMesh->vertexAttributeTypes[m].attributeType;
					attrType = E3Attribute_AttributeToClassType(attrType);
					theClass = E3ClassTree_GetClassByType(attrType);
					if (theClass != NULL)
						{
						attrSize = E3ClassTree_GetInstanceSize(theClass);
						Q3AttributeSet_Add(theTriangle->vertices[n].attributeSet, attrType,
											(TQ3Uns8 *) theTriMesh->vertexAttributeTypes[m].data + (vertIndex * attrSize));
						}
					}
				}
			}
		}
}





//=============================================================================
//      e3geom_trimesh_triangle_delete : Delete the data for a single triangle.
//-----------------------------------------------------------------------------
static void
e3geom_trimesh_triangle_delete(TQ3TriangleData *theTriangle)
{	TQ3Uns32		n;



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theTriangle));



	// Dispose of the triangle
	for (n = 0; n < 3; n++)
		Q3Object_CleanDispose(&theTriangle->vertices[n].attributeSet);

	Q3Object_CleanDispose(&theTriangle->triangleAttributeSet);
}





//=============================================================================
//      e3geom_trimesh_cache_new : TriMesh cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_trimesh_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3TriMeshData *geomData)
{	TQ3TriangleData			triangleData;
	TQ3GeometryObject		theTriangle;
	TQ3GroupObject			theGroup;
	TQ3Uns32				n;
#pragma unused(theView)



	// Create a group to hold the cached geometry
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Add the cached form to the group
	for (n = 0; n < geomData->numTriangles; n++)
		{
		// Extract the triangle
		e3geom_trimesh_triangle_new(geomData, n, &triangleData);


		// Create it
		theTriangle = Q3Triangle_New(&triangleData);
		if (theTriangle != NULL)
			Q3Group_AddObjectAndDispose(theGroup, &theTriangle);


		// Clean up
		e3geom_trimesh_triangle_delete(&triangleData);
		}



	// Finish off the group state (in-line, since we don't make any view state changes)
	Q3DisplayGroup_SetState(theGroup, kQ3DisplayGroupStateMaskIsInline  |
									  kQ3DisplayGroupStateMaskIsDrawn   |
									  kQ3DisplayGroupStateMaskIsWritten |
									  kQ3DisplayGroupStateMaskIsPicked);

	return(theGroup);
}





//=============================================================================
//      e3geom_trimesh_bounds_to_corners : Compute the 8 corners of a bounding box.
//-----------------------------------------------------------------------------
static void
e3geom_trimesh_bounds_to_corners( const TQ3BoundingBox* inBounds,
	TQ3Point3D* out8Corners )
{
	out8Corners[0] = inBounds->min;
	out8Corners[7] = inBounds->max;

	out8Corners[1] = out8Corners[2] = out8Corners[3] = out8Corners[0];
	out8Corners[1].x = out8Corners[7].x;
	out8Corners[2].y = out8Corners[7].y;
	out8Corners[3].z = out8Corners[7].z;

	out8Corners[4] = out8Corners[5] = out8Corners[6] = out8Corners[7];
	out8Corners[4].x = out8Corners[0].x;
	out8Corners[5].y = out8Corners[0].y;
	out8Corners[6].z = out8Corners[0].z;
}





//=============================================================================
//      e3geom_trimesh_pick_with_ray : TriMesh ray picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_pick_with_ray(TQ3ViewObject			theView,
								TQ3PickObject		thePick,
								const TQ3Ray3D		*theRay,
								TQ3Object			theObject,
								const void			*objectData)
{	const TQ3TriMeshData		*instanceData = (const TQ3TriMeshData *) objectData;
	TQ3Uns32					n, numPoints, v0, v1, v2;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Boolean					haveUV, cullBackface;
	TQ3Param2D					hitUV, *resultUV;
	TQ3BackfacingStyle			backfacingStyle;
	TQ3TriangleData				worldTriangle;
	TQ3Point3D					*worldPoints;
	TQ3Vector3D					hitNormal;
	TQ3Point3D					hitXYZ;
	TQ3Param3D					theHit;



	// Transform our points
	numPoints   = instanceData->numPoints;
	worldPoints = (TQ3Point3D *) Q3Memory_Allocate(numPoints * sizeof(TQ3Point3D));
	if (worldPoints == NULL)
		return(kQ3Failure);

	Q3Point3D_To3DTransformArray(instanceData->points,
								 E3View_State_GetLocalToWorld(theView),
								 worldPoints,
								 numPoints,
								 sizeof(TQ3Point3D),
								 sizeof(TQ3Point3D));



	// Determine if we should cull back-facing triangles or not
	qd3dStatus   = E3View_GetBackfacingStyleState(theView, &backfacingStyle);
	cullBackface = (TQ3Boolean)(qd3dStatus == kQ3Success && backfacingStyle == kQ3BackfacingStyleRemove);



	// See if we fall within the pick
	//
	// Note we do not use any vertex/edge tolerances supplied for the pick, since
	// QD3D's blue book appears to suggest neither are used for triangles.
	for (n = 0; n < instanceData->numTriangles && qd3dStatus == kQ3Success; n++)
		{
		// Grab the vertex indicies
		v0 = instanceData->triangles[n].pointIndices[0];
		v1 = instanceData->triangles[n].pointIndices[1];
		v2 = instanceData->triangles[n].pointIndices[2];
		Q3_ASSERT(v0 >= 0 && v0 < instanceData->numPoints);
		Q3_ASSERT(v1 >= 0 && v1 < instanceData->numPoints);
		Q3_ASSERT(v2 >= 0 && v2 < instanceData->numPoints);



		// Pick the triangle
		if (E3Ray3D_IntersectTriangle(theRay, &worldPoints[v0], &worldPoints[v1], &worldPoints[v2], cullBackface, &theHit))
			{
			// Create the triangle, and update the vertices to the transformed coordinates
			e3geom_trimesh_triangle_new(instanceData, n, &worldTriangle);
			worldTriangle.vertices[0].point = worldPoints[v0];
			worldTriangle.vertices[1].point = worldPoints[v1];
			worldTriangle.vertices[2].point = worldPoints[v2];


			// Obtain the XYZ, normal, and UV for the hit point. We always return an
			// XYZ and normal for the hit, however we need to cope with missing UVs.
			E3Triangle_InterpolateHit(&worldTriangle, &theHit, &hitXYZ, &hitNormal, &hitUV, &haveUV);
			resultUV = (haveUV ? &hitUV : NULL);


			// Record the hit
			qd3dStatus = E3Pick_RecordHit(thePick, theView, &hitXYZ, &hitNormal, resultUV, NULL);


			// Clean up
			e3geom_trimesh_triangle_delete(&worldTriangle);
			}
		}



	// Clean up
	Q3Memory_Free(&worldPoints);

	return(qd3dStatus);			
}





//=============================================================================
//      e3geom_trimesh_pick_with_rect : TriMesh rect picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_pick_with_rect(TQ3ViewObject			theView,
								TQ3PickObject		thePick,
								const TQ3Area		*theRect,
								TQ3Object			theObject,
								const void			*objectData)
{	const TQ3TriMeshData		*instanceData = (const TQ3TriMeshData *) objectData;
	TQ3Matrix4x4				theMatrix, worldToFrustum, frustumToWindow;
	TQ3Uns32					n, numPoints, v0, v1, v2;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Point2D					triVertices[3];
	TQ3Point3D					*windowPoints;



	// Transform our points from local coordinates to window coordinates
	numPoints    = instanceData->numPoints;
	windowPoints = (TQ3Point3D *) Q3Memory_Allocate(numPoints * sizeof(TQ3Point3D));
	if (windowPoints == NULL)
		return(kQ3Failure);

	Q3View_GetWorldToFrustumMatrixState(theView,  &worldToFrustum);
	Q3View_GetFrustumToWindowMatrixState(theView, &frustumToWindow);
	Q3Matrix4x4_Multiply(E3View_State_GetLocalToWorld(theView), &worldToFrustum, &theMatrix);
	Q3Matrix4x4_Multiply(&theMatrix, &frustumToWindow, &theMatrix);

	Q3Point3D_To3DTransformArray(instanceData->points, &theMatrix, windowPoints, numPoints, sizeof(TQ3Point3D), sizeof(TQ3Point3D));



	// See if we fall within the pick
	for (n = 0; n < instanceData->numTriangles && qd3dStatus == kQ3Success; n++)
		{
		// Grab the vertex indicies
		v0 = instanceData->triangles[n].pointIndices[0];
		v1 = instanceData->triangles[n].pointIndices[1];
		v2 = instanceData->triangles[n].pointIndices[2];
		Q3_ASSERT(v0 >= 0 && v0 < instanceData->numPoints);
		Q3_ASSERT(v1 >= 0 && v1 < instanceData->numPoints);
		Q3_ASSERT(v2 >= 0 && v2 < instanceData->numPoints);


		// Set up the 2D component of the triangle
		triVertices[0].x = windowPoints[v0].x;
		triVertices[0].y = windowPoints[v0].y;

		triVertices[1].x = windowPoints[v1].x;
		triVertices[1].y = windowPoints[v1].y;

		triVertices[2].x = windowPoints[v2].x;
		triVertices[2].y = windowPoints[v2].y;


		// See if this triangle falls within the pick
		if (E3Rect_ContainsLine(theRect, &triVertices[0], &triVertices[1]) ||
			E3Rect_ContainsLine(theRect, &triVertices[0], &triVertices[2]) ||
			E3Rect_ContainsLine(theRect, &triVertices[1], &triVertices[2]))
			{
			qd3dStatus = E3Pick_RecordHit(thePick, theView, NULL, NULL, NULL, NULL);
			break;
			}
		}



	// Clean up
	Q3Memory_Free(&windowPoints);

	return(qd3dStatus);			
}





//=============================================================================
//      e3geom_trimesh_pick_screen_bounds : Get our screen bounding rect.
//-----------------------------------------------------------------------------
//		Note :	Returns a 2D bounding rect that encloses the eight vertices of
//				our bounding box when projected to the screen.
//-----------------------------------------------------------------------------
static void
e3geom_trimesh_pick_screen_bounds(TQ3ViewObject theView, TQ3Object theObject, const void *objectData, TQ3Area *windowBounds)
{	const TQ3TriMeshData	*instanceData = (const TQ3TriMeshData *) objectData;
	TQ3Matrix4x4			theMatrix, worldToFrustum, frustumToWindow;
	TQ3Point3D				worldPoints[8], windowPoints[8];
	TQ3Uns32				n;



	// Obtain the eight corners of our bounding box
	e3geom_trimesh_bounds_to_corners( &instanceData->bBox, worldPoints );



	// Transform the eight corners from world coordinates to window coordinates
	Q3View_GetWorldToFrustumMatrixState(theView,  &worldToFrustum);
	Q3View_GetFrustumToWindowMatrixState(theView, &frustumToWindow);
	Q3Matrix4x4_Multiply(E3View_State_GetLocalToWorld(theView), &worldToFrustum, &theMatrix);
	Q3Matrix4x4_Multiply(&theMatrix, &frustumToWindow, &theMatrix);

	Q3Point3D_To3DTransformArray(worldPoints, &theMatrix, windowPoints, 8, sizeof(TQ3Point3D), sizeof(TQ3Point3D));



	// Calculate the 2D bounding box formed from these points
	windowBounds->min.x = windowBounds->max.x = windowPoints[0].x;
	windowBounds->min.y = windowBounds->max.y = windowPoints[0].y;

	for (n = 0; n < 8; n++)
		{
		if (windowPoints[n].x < windowBounds->min.x)
			windowBounds->min.x = windowPoints[n].x;

		else if (windowPoints[n].x > windowBounds->max.x)
			windowBounds->max.x = windowPoints[n].x;
		
		if (windowPoints[n].y < windowBounds->min.y)
			windowBounds->min.y = windowPoints[n].y;

		else if (windowPoints[n].y > windowBounds->max.y)
			windowBounds->max.y = windowPoints[n].y;
		}
}





//=============================================================================
//      e3geom_trimesh_pick_window_point : TriMesh window-point picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_pick_window_point(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3TriMeshData		*instanceData = (const TQ3TriMeshData *) objectData;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3WindowPointPickData		pickData;
	TQ3Ray3D					theRay;


	// Get the pick data
	Q3WindowPointPick_GetData(thePick, &pickData);


	// See if the pick ray falls within our bounding box.
	E3View_GetRayThroughPickPoint(theView, &theRay);
	if (kQ3True == E3Ray3D_IntersectBoundingBox(&theRay, &instanceData->bBox, NULL))
		{
		// It does, so do the actual triangle-level hit test.
		qd3dStatus = e3geom_trimesh_pick_with_ray(theView, thePick, &theRay, theObject, objectData);
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_pick_window_rect : TriMesh window-rect picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_pick_window_rect(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Area						windowBounds;
	TQ3WindowRectPickData		pickData;



	// Get the pick data
	Q3WindowRectPick_GetData(thePick, &pickData);



	// Obtain our window bounding rectangle
	e3geom_trimesh_pick_screen_bounds(theView, theObject, objectData, &windowBounds);



	// See if we fall within the pick - identifying trivial matches if we can
	if (E3Rect_ContainsRect(&windowBounds, &pickData.rect))
		qd3dStatus = E3Pick_RecordHit(thePick, theView, NULL, NULL, NULL, NULL);

	else if (E3Rect_IntersectRect(&windowBounds, &pickData.rect))
		qd3dStatus = e3geom_trimesh_pick_with_rect(theView, thePick, &pickData.rect, theObject, objectData);


	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_pick_world_ray : TriMesh world-ray picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_pick_world_ray(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3TriMeshData		*instanceData = (const TQ3TriMeshData *) objectData;
	TQ3BoundingBox				worldBounds;
	TQ3Status					qd3dStatus;
	TQ3WorldRayPickData			pickData;
	TQ3Point3D					hitHYZ;
	TQ3Point3D					boxCorners[8];



	// Get the pick data
	Q3WorldRayPick_GetData(thePick, &pickData);



	// Before calling e3geom_trimesh_pick_with_ray, we want to cull out cases where
	// the ray is nowhere near the trimesh.  Previous code here tried to transform the
	// bounding box by transforming its min and max corners, which is incorrect.
	// To get an exact bounding box, we'd have to transform all the points and then
	// use Q3BoundingBox_SetFromPoints3D, but that seems too much work for present
	// purposes.  So, let's find the 8 corners of the local bounding box, transform
	// them, and then find the bounding box of those points.
	e3geom_trimesh_bounds_to_corners( &instanceData->bBox, boxCorners );
	Q3Point3D_To3DTransformArray(boxCorners,
								 E3View_State_GetLocalToWorld(theView),
								 boxCorners,
								 8,
								 sizeof(TQ3Point3D),
								 sizeof(TQ3Point3D));
	Q3BoundingBox_SetFromPoints3D( &worldBounds, boxCorners, 8, sizeof(TQ3Point3D) );
	


	// See if we fall within the pick
	if (Q3Ray3D_IntersectBoundingBox(&pickData.ray, &worldBounds, &hitHYZ))
		qd3dStatus = e3geom_trimesh_pick_with_ray(theView, thePick, &pickData.ray, theObject, objectData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_pick : TriMesh picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3TriMeshData	*instanceData = (const TQ3TriMeshData *) objectData;
	TQ3Status				qd3dStatus;
	TQ3PickObject			thePick;



	// Validate our state
	Q3_ASSERT( instanceData->bBox.isEmpty == kQ3False );



	// Handle the pick
	thePick = E3View_AccessPick(theView);
	switch (Q3Pick_GetType(thePick)) {
		case kQ3PickTypeWindowPoint:
			qd3dStatus = e3geom_trimesh_pick_window_point(theView, thePick, theObject, objectData);
			break;

		case kQ3PickTypeWindowRect:
			qd3dStatus = e3geom_trimesh_pick_window_rect(theView, thePick, theObject, objectData);
			break;

		case kQ3PickTypeWorldRay:
			qd3dStatus = e3geom_trimesh_pick_world_ray(theView, thePick, theObject, objectData);
			break;

		default:
			qd3dStatus = kQ3Failure;
			break;
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_bounds : TriMesh bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3TriMeshData			*instanceData = (const TQ3TriMeshData *) objectData;
	TQ3Point3D				boundCorners[8];
	TQ3BoundingMethod		boundingMethod;
#pragma unused(objectType)
#pragma unused(theObject)


	Q3_ASSERT( instanceData->bBox.isEmpty == kQ3False );


	// Calculate the exact bounds from our points
	boundingMethod = E3View_GetBoundingMethod(theView);
	if (boundingMethod == kQ3BoxBoundsExact || boundingMethod == kQ3SphereBoundsExact)
		E3View_UpdateBounds(theView, instanceData->numPoints, sizeof(TQ3Point3D), instanceData->points);


	// And our approximate bounds from our bounding box
	//
	// In local coordinates, taking the bounding box of the min and max points does recreate
	// the bounding box. However, even in "approximate" mode, E3View_UpdateBounds uses the
	// exact method for a bounding box of a small numbers of points, hence the min and max
	// points would be converted to world coordinates before taking a bounding box.
	//
	// This can give a bad result, even an empty bounding box. To prevent this, we take
	// the bounds of our eight corners and not just the two min/max points.
	else
		{
		e3geom_trimesh_bounds_to_corners( &instanceData->bBox, boundCorners );
		
		E3View_UpdateBounds(theView, 8, sizeof(TQ3Point3D), boundCorners);
		}
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_trimesh_get_attribute : TriMesh get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_trimesh_get_attribute(TQ3GeometryObject theObject)
{	TQ3TriMeshData		*instanceData = (TQ3TriMeshData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->triMeshAttributeSet);
}





//=============================================================================
//      e3geom_trimesh_metahandler : TriMesh metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_trimesh_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_get_attribute;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryTriMesh_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryTriMesh_RegisterClass(void)
{	TQ3Status		qd3dStatus;


	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeTriMesh,
											kQ3ClassNameGeometryTriMesh,
											e3geom_trimesh_metahandler,
											sizeof(TQ3TriMeshData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryTriMesh_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryTriMesh_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeTriMesh, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3TriMesh_New : Create a TriMesh object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3TriMesh_New(const TQ3TriMeshData *triMeshData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeTriMesh, kQ3False, triMeshData);
	return(theObject);
}





//=============================================================================
//      E3TriMesh_Submit : Submit a TriMesh.
//-----------------------------------------------------------------------------
TQ3Status
E3TriMesh_Submit(const TQ3TriMeshData *triMeshData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeTriMesh, triMeshData);
	return(qd3dStatus);
}





//=============================================================================
//      E3TriMesh_SetData : Set the data for a TriMesh object.
//-----------------------------------------------------------------------------
TQ3Status
E3TriMesh_SetData(TQ3GeometryObject triMesh, const TQ3TriMeshData *triMeshData)
{	TQ3TriMeshData		*instanceData = (TQ3TriMeshData *) triMesh->instanceData;
	TQ3Status			qd3dStatus;



	// first, free the old data
	e3geom_trimesh_disposedata(instanceData);



	// then copy in the new data
	qd3dStatus = e3geom_trimesh_copydata(triMeshData, instanceData, kQ3False);

	Q3Shared_Edited(triMesh);

	return(qd3dStatus);
}





//=============================================================================
//      E3TriMesh_GetData : Return the data for a TriMesh object.
//-----------------------------------------------------------------------------
TQ3Status
E3TriMesh_GetData(TQ3GeometryObject triMesh, TQ3TriMeshData *triMeshData)
{	TQ3TriMeshData		*instanceData = (TQ3TriMeshData *) triMesh->instanceData;
	TQ3Status			qd3dStatus;
	


	// Copy the data out of the TriMesh
	triMeshData->triMeshAttributeSet = NULL;
	qd3dStatus = e3geom_trimesh_copydata(instanceData, triMeshData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      E3TriMesh_EmptyData : Empty the data for a TriMesh object.
//-----------------------------------------------------------------------------
TQ3Status
E3TriMesh_EmptyData(TQ3TriMeshData *triMeshData)
{


	// Dispose of the data
	e3geom_trimesh_disposedata(triMeshData);

	return(kQ3Success);
}





//=============================================================================
//      E3TriMesh_LockData : Lock a TriMesh for direct access.
//-----------------------------------------------------------------------------
//		Note :	Our current implementation does not require locking, and so we
//				can simply return a pointer to the TriMesh instance data.
//
//				We will eventually be able to move responsibility for geometry
//				data down to the renderer objects, at which time we will need
//				to keep a flag indicating if a TriMesh is locked.
//
//				We will then need to update the renderer object after a TriMesh
//				is unlocked after being locked without read-only access.
//-----------------------------------------------------------------------------
TQ3Status
E3TriMesh_LockData(TQ3GeometryObject triMesh, TQ3Boolean readOnly, TQ3TriMeshData **triMeshData)
{	TQ3TriMeshData		*instanceData = (TQ3TriMeshData *) triMesh->instanceData;



	// Return a pointer to our instance data
	*triMeshData = instanceData;
	
	return(kQ3Success);
}





//=============================================================================
//      E3TriMesh_UnlockData : Unlock a TriMesh after direct access.
//-----------------------------------------------------------------------------
TQ3Status
E3TriMesh_UnlockData(TQ3GeometryObject triMesh)
{


	// Nothing to do
	//
	// See E3TriMesh_LockData for implementation notes.
	return(kQ3Success);
}



/*  NAME:
        E3GeometryTriGrid.c

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
#include "E3Geometry.h"
#include "E3GeometryTriGrid.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_trigrid_copydata : Copy TQ3TriGridData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trigrid_copydata(const TQ3TriGridData *src, TQ3TriGridData *dst, TQ3Boolean isDuplicate)
{
	TQ3Status	qd3dStatus = kQ3Success;
	TQ3Uns32	qtyVerts;		// how many vertices we have
	TQ3Uns32	qtyFacets;		// how many triangles we have
	TQ3Uns32	theSize;
	TQ3Uns32	i;
		
	qtyVerts = src->numRows * src->numColumns;
	qtyFacets = 2 * (src->numRows-1) * (src->numColumns-1);
	dst->facetAttributeSet = NULL;
	dst->triGridAttributeSet = NULL;
	
	// copy raw data
	theSize = sizeof(TQ3Uns32)			// numRows
		+ sizeof(TQ3Uns32);				// numCols
	Q3Memory_Copy( src, dst, theSize );

	// copy the vertices
	dst->vertices = (TQ3Vertex3D *) Q3Memory_AllocateClear( sizeof(TQ3Vertex3D) * qtyVerts );
	if (dst->vertices == NULL) {
		dst->numRows = dst->numColumns = 0;
		return kQ3Failure;
	}
	
	for(i=0; i< qtyVerts; i++){
		dst->vertices[i].point = src->vertices[i].point;
		}

	// copy or shared-replace the attributes
	if (isDuplicate)
	{
	
		for(i=0; i< qtyVerts; i++){
			if(src->vertices[i].attributeSet != NULL)
				dst->vertices[i].attributeSet = Q3Object_Duplicate(src->vertices[i].attributeSet);
			}
			
		if (src->facetAttributeSet != NULL)
		{
			// facetAttributeSet is actually an array of attribute sets
			dst->facetAttributeSet = (TQ3AttributeSet *) Q3Memory_AllocateClear( sizeof(TQ3AttributeSet) * qtyFacets );
			if (dst->facetAttributeSet != NULL) {
				for (i=0; i<qtyFacets; i++) {
					if(src->facetAttributeSet[i] != NULL){
						dst->facetAttributeSet[i] = Q3Object_Duplicate(src->facetAttributeSet[i]);
						if (dst->facetAttributeSet[i] == NULL){
							qd3dStatus = kQ3Failure;
							break;
							}
					}
				}
			}
		}

		if (src->triGridAttributeSet != NULL)
		{
			dst->triGridAttributeSet = Q3Object_Duplicate(src->triGridAttributeSet);
			if (dst->triGridAttributeSet == NULL) qd3dStatus = kQ3Failure;
		}
	}
	else {
		for(i=0; i< qtyVerts; i++){
			if(src->vertices[i].attributeSet != NULL)
				E3Shared_Replace(&dst->vertices[i].attributeSet, src->vertices[i].attributeSet);
			}
			
		if (src->facetAttributeSet != NULL)
		{
			// facetAttributeSet is actually an array of attribute sets
			dst->facetAttributeSet = (TQ3AttributeSet *) Q3Memory_AllocateClear( sizeof(TQ3AttributeSet) * qtyFacets );
			if (dst->facetAttributeSet != NULL) {
				for (i=0; i<qtyFacets; i++) {
					E3Shared_Replace(&dst->facetAttributeSet[i], src->facetAttributeSet[i]);
				}
			}
		}
		
		E3Shared_Replace(&dst->triGridAttributeSet, src->triGridAttributeSet);
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_trigrid_disposedata : Dispose of a TQ3TriGridData.
//-----------------------------------------------------------------------------
static void
e3geom_trigrid_disposedata(TQ3TriGridData *theTriGrid)
{
	TQ3Uns32	qtyFacets;		// how many triangles we have
	TQ3Uns32	i;
	TQ3Uns32	qtyVertices;

	if (theTriGrid->facetAttributeSet != NULL) {
		qtyFacets = 2 * (theTriGrid->numRows-1) * (theTriGrid->numColumns-1);
		for (i=0; i<qtyFacets; i++) {
			E3Object_DisposeAndForget( theTriGrid->facetAttributeSet[i] );
		}
	}
	
	qtyVertices = theTriGrid->numRows * theTriGrid->numColumns;
	for (i = 0; i < qtyVertices; ++i)
	{
		E3Object_DisposeAndForget( theTriGrid->vertices[ i ].attributeSet );
	}
	
	E3Object_DisposeAndForget( theTriGrid->triGridAttributeSet );
}	





//=============================================================================
//      e3geom_trigrid_new : TriGrid new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trigrid_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3TriGridData			*instanceData = (TQ3TriGridData *)       privateData;
	const TQ3TriGridData	*trigridData     = (const TQ3TriGridData *) paramData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)

	qd3dStatus = e3geom_trigrid_copydata(trigridData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trigrid_delete : TriGrid delete method.
//-----------------------------------------------------------------------------
static void
e3geom_trigrid_delete(TQ3Object theObject, void *privateData)
{	TQ3TriGridData		*instanceData = (TQ3TriGridData *) privateData;
#pragma unused(theObject)

	// Dispose of our instance data
	e3geom_trigrid_disposedata(instanceData);
}





//=============================================================================
//      e3geom_trigrid_duplicate : TriGrid duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trigrid_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TQ3TriGridData	*fromInstanceData = (const TQ3TriGridData *) fromPrivateData;
	TQ3TriGridData			*toInstanceData   = (TQ3TriGridData *)       toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_trigrid_copydata( fromInstanceData, toInstanceData, kQ3True );

	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_trigrid_disposedata(toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trigrid_addtriangle : add one triangle to a group
//				for use in building a cached geometry.
//-----------------------------------------------------------------------------
static void 
e3geom_trigrid_addtriangle(TQ3GroupObject group, const TQ3TriGridData *geomData,
                           TQ3Uns32 n0, TQ3Uns32 n1, TQ3Uns32 n2, TQ3Uns32 tnum)
{
	TQ3TriangleData triangleData;
	TQ3GeometryObject	theTriangle;

	// copy vertex data (including vertex attributes)
	Q3Memory_Copy( &geomData->vertices[n0], &triangleData.vertices[0], sizeof(TQ3Vertex3D) );
	Q3Memory_Copy( &geomData->vertices[n1], &triangleData.vertices[1], sizeof(TQ3Vertex3D) );
	Q3Memory_Copy( &geomData->vertices[n2], &triangleData.vertices[2], sizeof(TQ3Vertex3D) );

	// copy triangle attributes	
	if (geomData->facetAttributeSet != NULL) {
		E3AttributeSet_Combine( geomData->triGridAttributeSet, geomData->facetAttributeSet[tnum],
				&triangleData.triangleAttributeSet );
	} else {
		triangleData.triangleAttributeSet = geomData->triGridAttributeSet;	
	}
	
	// build and add the triangle
	theTriangle = Q3Triangle_New(&triangleData);
	if (theTriangle != NULL) E3Group_AddObjectAndDispose(group, theTriangle);	
}


//=============================================================================
//      e3geom_trigrid_cache_new : TriGrid cache new method.
//-----------------------------------------------------------------------------
//	Note: for reference, see
// http://developer.apple.com/techpubs/quicktime/qtdevdocs/QD3D/qd3dgeometry.29.htm
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_trigrid_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3TriGridData *geomData)
{
#pragma unused(theView)

	TQ3GroupObject		theGroup;
	TQ3TriMeshData		triMeshData;
	TQ3GeometryObject	theTriMesh;

	TQ3TriMeshTriangleData *triangles;
	TQ3Point3D *points;
	TQ3Uns32 numpoints, numtriangles;
	TQ3Uns32 i, col, row, tnum=0, vnum=0;
	TQ3Boolean cacheAsTriangles = kQ3False;		// to always test triangles, set this to true!
	
	numpoints = geomData->numRows * geomData->numColumns;
	numtriangles = 2 * (geomData->numRows-1) * (geomData->numColumns-1);

	// decide whether to cache as triangles or a trimesh
	if (geomData->facetAttributeSet != NULL) cacheAsTriangles = kQ3True;
	for (i=0; i<numpoints && cacheAsTriangles==kQ3False; i++) {
		if (geomData->vertices[i].attributeSet != NULL) cacheAsTriangles = kQ3True;
	}

	// --------------------------- caching as triangles -----------------------
	if (cacheAsTriangles) {
		// Create a group to hold the cached geometry
		theGroup = Q3DisplayGroup_New();
		if (theGroup == NULL)
			return(NULL);

		// Add a bunch of triangles to the group
		for (row=0; row < geomData->numRows-1; row++) {
			for (col=0; col < geomData->numColumns-1; col++) {
				vnum = (row * geomData->numColumns) + col;
				e3geom_trigrid_addtriangle( theGroup, geomData, 
					vnum,
					vnum + 1 + ((col & 1) ? geomData->numColumns : 0),
					vnum + geomData->numColumns,
					tnum );
				tnum++;
				e3geom_trigrid_addtriangle( theGroup, geomData, 
					vnum + ((col & 1) ? 0 : geomData->numColumns),
					vnum + 1,
					vnum + geomData->numColumns + 1,
					tnum );
				tnum++;
			}
		}		

		// Finish off the group state (in-line, since we don't make any view state changes)
		Q3DisplayGroup_SetState(theGroup, kQ3DisplayGroupStateMaskIsInline  |
										  kQ3DisplayGroupStateMaskIsDrawn   |
										  kQ3DisplayGroupStateMaskIsWritten |
										  kQ3DisplayGroupStateMaskIsPicked);

		return theGroup;
	}
	
	// --------------------------- caching as a trimesh -----------------------

	// Create a TriMesh
	triangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate( numtriangles*sizeof(TQ3TriMeshTriangleData) );
	if (triangles == NULL) return NULL;
	points = (TQ3Point3D *) Q3Memory_Allocate( numpoints*sizeof(TQ3Point3D) );
	if (points == NULL) {
		Q3Memory_Free(triangles);
		return NULL;
	}
	
	for (i=0; i<numpoints; i++) {
		Q3Memory_Copy(&geomData->vertices[i], &points[i], sizeof(TQ3Point3D));
	}

	for (row=0; row < geomData->numRows-1; row++) {
		for (col=0; col < geomData->numColumns-1; col++) {
			vnum = (row * geomData->numColumns) + col;
			triangles[tnum].pointIndices[0] = vnum;
			triangles[tnum].pointIndices[1] = vnum + 1 + ((col & 1) ? geomData->numColumns : 0);
			triangles[tnum].pointIndices[2] = vnum + geomData->numColumns;
			tnum++;
			triangles[tnum].pointIndices[0] = vnum + ((col & 1) ? 0 : geomData->numColumns);
			triangles[tnum].pointIndices[1] = vnum + 1;
			triangles[tnum].pointIndices[2] = vnum + geomData->numColumns + 1;
			tnum++;
		}
	}

	// set up remaining trimesh data
	triMeshData.numPoints                 = numpoints;
	triMeshData.points                    = points;
	triMeshData.numTriangles              = tnum; // numtriangles;
	triMeshData.triangles                 = triangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = 0;
	triMeshData.edges                     = NULL;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 0;
	triMeshData.vertexAttributeTypes      = NULL;
	triMeshData.triMeshAttributeSet       = geomData->triGridAttributeSet;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox, triMeshData.points, numpoints, sizeof(TQ3Point3D));

	theTriMesh = Q3TriMesh_New(&triMeshData);

	// Clean up
	Q3Memory_Free(&points);
	Q3Memory_Free(&triangles);

	return theTriMesh;
}





//=============================================================================
//      e3geom_trigrid_bounds : TriGrid bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trigrid_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3TriGridData			*instanceData = (const TQ3TriGridData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)

	TQ3Uns32	qtyVerts = instanceData->numRows * instanceData->numColumns;


	// Update the bounds
	E3View_UpdateBounds(theView, qtyVerts, sizeof(TQ3Vertex3D), (TQ3Point3D*)(instanceData->vertices));
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_trigrid_get_attribute : TriGrid get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_trigrid_get_attribute(TQ3GeometryObject theObject)
{	TQ3TriGridData		*instanceData = (TQ3TriGridData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->triGridAttributeSet);
}





//=============================================================================
//      e3geom_trigrid_metahandler : TriGrid metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_trigrid_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_trigrid_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_trigrid_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_trigrid_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_trigrid_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_trigrid_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_trigrid_get_attribute;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryTriGrid_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryTriGrid_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeTriGrid,
											kQ3ClassNameGeometryTriGrid,
											e3geom_trigrid_metahandler,
											sizeof(TQ3TriGridData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryTriGrid_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryTriGrid_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeTriGrid, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3TriGrid_New : Create a TriGrid object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3TriGrid_New(const TQ3TriGridData *triGridData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeTriGrid, kQ3False, triGridData);
	return(theObject);
}





//=============================================================================
//      E3TriGrid_Submit : Submit a TriGrid.
//-----------------------------------------------------------------------------
TQ3Status
E3TriGrid_Submit(const TQ3TriGridData *triGridData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeTriGrid, triGridData);
	return(qd3dStatus);
}





//=============================================================================
//      E3TriGrid_SetData : Set the data for a trigrid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3TriGrid_SetData(TQ3GeometryObject trigrid, const TQ3TriGridData *trigridData)
{
	TQ3TriGridData		*instanceData = (TQ3TriGridData *) trigrid->instanceData;
	TQ3Status		qd3dStatus;

	// first, free the old data
	e3geom_trigrid_disposedata(instanceData);

	// then copy in the new data
	qd3dStatus = e3geom_trigrid_copydata(trigridData, instanceData, kQ3False);
	Q3Shared_Edited(trigrid);

	return(qd3dStatus);
}





//=============================================================================
//      E3TriGrid_GetData : Get the data for a trigrid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3TriGrid_GetData(TQ3GeometryObject trigrid, TQ3TriGridData *trigridData)
{
	TQ3TriGridData		*instanceData = (TQ3TriGridData *) trigrid->instanceData;
	TQ3Status		qd3dStatus;

	// Copy the data out of the TriGrid
	trigridData->facetAttributeSet = NULL;
	trigridData->triGridAttributeSet = NULL;
	qd3dStatus = e3geom_trigrid_copydata(instanceData, trigridData, kQ3False);

	return(qd3dStatus);

}




//=============================================================================
//      E3TriGrid_EmptyData : Empty the data for a trigrid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3TriGrid_EmptyData(TQ3TriGridData *trigridData)
{

	// Dispose of the data
	e3geom_trigrid_disposedata(trigridData);

	return(kQ3Success);
}




//=============================================================================
//      E3TriGrid_GetVertexPosition : Get the position (in space) of a
//			vertex in the grid, specified by row and column.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3TriGrid_GetVertexPosition(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex,
			TQ3Uns32 columnIndex, TQ3Point3D *position)
{
	TQ3TriGridData		*trigridData = (TQ3TriGridData *) triGrid->instanceData;
	TQ3Uns32 idx = rowIndex * trigridData->numColumns + columnIndex;
	*position = trigridData->vertices[idx].point;
	
	return(kQ3Success);
}





//=============================================================================
//      E3TriGrid_SetVertexPosition : Set the position (in space) of a
//			vertex in the grid, specified by row and column.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3TriGrid_SetVertexPosition(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex,
			TQ3Uns32 columnIndex, const TQ3Point3D *position)
{
	TQ3TriGridData		*trigridData = (TQ3TriGridData *) triGrid->instanceData;
	TQ3Uns32 idx = rowIndex * trigridData->numColumns + columnIndex;
	trigridData->vertices[idx].point = *position;
	
	Q3Shared_Edited(triGrid);
	return(kQ3Success);
}





//=============================================================================
//      E3TriGrid_GetVertexAttributeSet : Get the attribute set of a
//			vertex in the grid, specified by row and column.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3TriGrid_GetVertexAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex,
			TQ3Uns32 columnIndex, TQ3AttributeSet *attributeSet)
{
	TQ3TriGridData		*trigridData = (TQ3TriGridData *) triGrid->instanceData;
	TQ3Uns32 idx = rowIndex * trigridData->numColumns + columnIndex;
	E3Shared_Acquire(attributeSet, trigridData->vertices[idx].attributeSet);
	
	return(kQ3Success);
}





//=============================================================================
//      E3TriGrid_SetVertexAttributeSet : Set the attribute set of a
//			vertex in the grid, specified by row and column.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3TriGrid_SetVertexAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, TQ3AttributeSet attributeSet)
{
	TQ3TriGridData		*trigridData = (TQ3TriGridData *) triGrid->instanceData;
	TQ3Uns32 idx = rowIndex * trigridData->numColumns + columnIndex;
	E3Shared_Replace(&trigridData->vertices[idx].attributeSet, attributeSet);

	Q3Shared_Edited(triGrid);
	return(kQ3Success);
}





//=============================================================================
//      E3TriGrid_GetFacetAttributeSet : Get the attribute set of a facet.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3TriGrid_GetFacetAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 faceIndex,
			TQ3AttributeSet *facetAttributeSet)
{
	TQ3TriGridData		*trigridData = (TQ3TriGridData *) triGrid->instanceData;
	if (trigridData->facetAttributeSet) {
		E3Shared_Acquire(facetAttributeSet, trigridData->facetAttributeSet[faceIndex]);
	} else *facetAttributeSet = NULL;
	
	return(kQ3Success);
}





//=============================================================================
//      E3TriGrid_SetFacetAttributeSet : Set the attribute set of a facet.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3TriGrid_SetFacetAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 faceIndex,
			TQ3AttributeSet facetAttributeSet)
{
	TQ3TriGridData		*trigridData = (TQ3TriGridData *) triGrid->instanceData;
	if (trigridData->facetAttributeSet) {
		E3Shared_Replace(&trigridData->facetAttributeSet[faceIndex], facetAttributeSet);
	} else return(kQ3Failure);	

	Q3Shared_Edited(triGrid);
	return(kQ3Success);
}

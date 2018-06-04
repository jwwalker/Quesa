/*  NAME:
        E3GeometryPolygon.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3Set.h"
#include "E3View.h"
#include "E3Geometry.h"
#include "E3GeometryTriMesh.h"
#include "E3GeometryPolygon.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Polygon : public E3Geometry // This is a leaf class so no other files use this,
									// so it can be local and hance all the fields public
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypePolygon, E3Polygon, E3Geometry )
public :

	TQ3PolygonData		instanceData ;
	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_polygon_gather_vertex_attribute : Gather vertex attributes.
//-----------------------------------------------------------------------------
static TQ3AttributeSet
e3geom_polygon_gather_vertex_attribute(const void *userData, TQ3Uns32 setIndex)
{	const TQ3PolygonData	*geomData = (const TQ3PolygonData *) userData;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(setIndex < geomData->numVertices, NULL);



	// Return the appropriate attribute set
	return(geomData->vertices[setIndex].attributeSet);
}





//=============================================================================
//      e3geom_polygon_new : Polygon new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polygon_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PolygonData			*instanceData = (TQ3PolygonData *)       privateData;
	const TQ3PolygonData	*polygonData  = (const TQ3PolygonData *) paramData;
	TQ3Status				qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3PolygonData));
	
	qd3dStatus = Q3Polygon_SetData(theObject, polygonData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_polygon_delete : Polygon delete method.
//-----------------------------------------------------------------------------
static void
e3geom_polygon_delete(TQ3Object theObject, void *privateData)
{	TQ3PolygonData		*instanceData = (TQ3PolygonData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	Q3Polygon_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_polygon_duplicate : Polygon duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polygon_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	TQ3PolygonData		*toInstanceData = (TQ3PolygonData *) toPrivateData;
	TQ3Status			qd3dStatus;
	TQ3AttributeSet		dupSet;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Polygon_GetData(fromObject, toInstanceData);
	
	if ( (qd3dStatus == kQ3Success) &&
		(toInstanceData->polygonAttributeSet != NULL) )
	{
		dupSet = Q3Object_Duplicate( toInstanceData->polygonAttributeSet );
		Q3Object_Dispose( toInstanceData->polygonAttributeSet );
		toInstanceData->polygonAttributeSet = dupSet;
		if (dupSet == NULL)
		{
			qd3dStatus = kQ3Failure;
		}
	}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_polygon_cache_new : Polygon cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_polygon_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3PolygonData *geomData)
{	TQ3TriMeshAttributeData		vertexAttributes[kQ3AttributeTypeNumTypes];
	TQ3Uns32					n, numEdges, numTriangles;
	TQ3TriMeshAttributeData		edgeAttributes[1];
	TQ3OrientationStyle			theOrientation;
	TQ3TriMeshTriangleData		*theTriangles;
	TQ3TriMeshData				triMeshData;
	TQ3GeometryObject			theTriMesh;
	TQ3Point3D					*thePoints;
	TQ3TriMeshEdgeData			*theEdges;



	// Work out how many edges and triangles we need
	Q3_ASSERT(geomData->numVertices >= 3);
	numEdges     = geomData->numVertices;
	numTriangles = geomData->numVertices - 2;



	// Allocate the memory we need for the TriMesh data
	thePoints    = (TQ3Point3D *)             Q3Memory_Allocate(static_cast<TQ3Uns32>(geomData->numVertices * sizeof(TQ3Point3D)));
	theTriangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate(static_cast<TQ3Uns32>(numTriangles * sizeof(TQ3TriMeshTriangleData)));
	theEdges     = (TQ3TriMeshEdgeData *)     Q3Memory_Allocate(static_cast<TQ3Uns32>(numEdges     * sizeof(TQ3TriMeshEdgeData)));

	if (thePoints == NULL || theTriangles == NULL || theEdges == NULL)
		{
		Q3Memory_Free(&thePoints);
		Q3Memory_Free(&theTriangles);
		Q3Memory_Free(&theEdges);
		
		return(NULL);
		}



	// Initialise the points, edges, and triangles
	for (n = 0; n < geomData->numVertices; n++)
		thePoints[n] = geomData->vertices[n].point;

	for (n = 0; n < numEdges; n++)
		{
		theEdges[n].pointIndices[0]    = n;
		theEdges[n].pointIndices[1]    = (n == (numEdges-1)) ? 0 : (n + 1);
		theEdges[n].triangleIndices[0] = n - 1;
		theEdges[n].triangleIndices[1] = kQ3ArrayIndexNULL;
		}
	
	theEdges[0].triangleIndices[0] = 0;
	theEdges[numEdges - 1].triangleIndices[0] = numEdges - 3;
		
	for (n = 0; n < numTriangles; n++)
		{
		theTriangles[n].pointIndices[0] = 0;
		theTriangles[n].pointIndices[1] = n + 1;
		theTriangles[n].pointIndices[2] = n + 2;
		}



	// Initialise the TriMesh data
	triMeshData.numPoints                 = geomData->numVertices;
	triMeshData.points                    = thePoints;
	triMeshData.numTriangles              = numTriangles;
	triMeshData.triangles                 = theTriangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = numEdges;
	triMeshData.edges                     = theEdges;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 0;
	triMeshData.vertexAttributeTypes      = NULL;
	triMeshData.triMeshAttributeSet       = geomData->polygonAttributeSet;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox, triMeshData.points, triMeshData.numPoints, sizeof(TQ3Point3D));



	// Set up the vertex attributes
	n = 0;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polygon_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeSurfaceUV))
		n++;
	else
	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polygon_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeShadingUV))
		n++;
	
	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polygon_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeNormal))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polygon_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeAmbientCoefficient))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polygon_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeDiffuseColor))
		{
		// Set up some edge colours as well, just reusing the vertex colours
		Q3_ASSERT(numEdges == geomData->numVertices);
		edgeAttributes[0] = vertexAttributes[n];
		
		triMeshData.numEdgeAttributeTypes   = 1;
		triMeshData.edgeAttributeTypes      = edgeAttributes;
		n++;
		}

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polygon_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeSpecularColor))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polygon_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeSpecularControl))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polygon_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeTransparencyColor))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polygon_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeSurfaceTangent))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polygon_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeHighlightState))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polygon_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeSurfaceShader))
		n++;

	Q3_ASSERT(n < (sizeof(vertexAttributes) / sizeof(TQ3TriMeshAttributeData)));
	if (n != 0)
		{
		triMeshData.numVertexAttributeTypes = n;
		triMeshData.vertexAttributeTypes    = vertexAttributes;
		}



	// Create the TriMesh
	theTriMesh = Q3TriMesh_New(&triMeshData);
	if (theTriMesh != NULL)
		{
		theOrientation = E3View_State_GetStyleOrientation(theView);
		E3TriMesh_AddTriangleNormals(theTriMesh, theOrientation);
		}
		
	
	
	// Clean up
	Q3Memory_Free(&thePoints);
	Q3Memory_Free(&theTriangles);
	Q3Memory_Free(&theEdges);
	
	for (n = 0; n < triMeshData.numVertexAttributeTypes; n++)
		{
		Q3Memory_Free(&triMeshData.vertexAttributeTypes[n].data);
		Q3Memory_Free(&triMeshData.vertexAttributeTypes[n].attributeUseArray);
		}

	return(theTriMesh);
}





//=============================================================================
//      e3geom_polygon_bounds : Polygon bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polygon_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3PolygonData		*instanceData = (const TQ3PolygonData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	E3View_UpdateBounds(theView, instanceData->numVertices, sizeof(TQ3Vertex3D), &instanceData->vertices[0].point);
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_polygon_get_attribute : Polygon get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_polygon_get_attribute(TQ3GeometryObject theObject)
	{
	// Return the address of the geometry attribute set
	return & ( (E3Polygon*) theObject )->instanceData.polygonAttributeSet ;
	}





//=============================================================================
//      e3geom_polygon_metahandler : Polygon metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_polygon_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_polygon_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_polygon_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_polygon_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_polygon_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_polygon_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_polygon_get_attribute;
			break;

		case kQ3XMethodTypeGeomUsesOrientation:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryPolygon_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryPolygon_RegisterClass(void)
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryPolygon,
								e3geom_polygon_metahandler,
								E3Polygon ) ;
	}





//=============================================================================
//      E3GeometryPolygon_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryPolygon_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypePolygon, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Polygon_New : Create a polygon object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Polygon_New(const TQ3PolygonData *polygonData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypePolygon, kQ3False, polygonData);
	return(theObject);
}





//=============================================================================
//      E3Polygon_Submit : Submit a polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_Submit(const TQ3PolygonData *polygonData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypePolygon, polygonData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Polygon_SetData : Set the data for a polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_SetData ( TQ3GeometryObject thePolygon, const TQ3PolygonData *polygonData )
	{
	E3Polygon* poly = (E3Polygon*) thePolygon ;

	// Allocate some space for the new vertices
	TQ3Vertex3D* newVertices = (TQ3Vertex3D *) Q3Memory_Allocate(static_cast<TQ3Uns32>(polygonData->numVertices * sizeof(TQ3Vertex3D)));
	if (newVertices == NULL)
		return(kQ3Failure);



	// Dispose of the existing data
	for ( TQ3Uns32 m = 0 ; m < poly->instanceData.numVertices ; ++m )
		Q3Object_CleanDispose(&poly->instanceData.vertices[m].attributeSet);

	Q3Memory_Free(&poly->instanceData.vertices);



	// Copy the new data
	poly->instanceData.numVertices = polygonData->numVertices;
	poly->instanceData.vertices    = newVertices;

	for ( TQ3Uns32 n = 0 ; n < poly->instanceData.numVertices ; ++n )
		{
		poly->instanceData.vertices[n].point = polygonData->vertices[n].point;
		E3Shared_Acquire(&poly->instanceData.vertices[n].attributeSet, polygonData->vertices[n].attributeSet);
		}

	E3Shared_Replace(&poly->instanceData.polygonAttributeSet, polygonData->polygonAttributeSet);

	Q3Shared_Edited(thePolygon);

	return kQ3Success ;
	}





//=============================================================================
//      E3Polygon_GetData : Get the data for a polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_GetData ( TQ3GeometryObject thePolygon, TQ3PolygonData *polygonData )
	{
	E3Polygon* poly = (E3Polygon*) thePolygon ;



	// Allocate some space for the new vertices
	TQ3Vertex3D* newVertices = (TQ3Vertex3D *) Q3Memory_Allocate ( static_cast<TQ3Uns32>(poly->instanceData.numVertices * sizeof ( TQ3Vertex3D )) ) ;
	if (newVertices == NULL)
		return(kQ3Failure);



	// Copy the data
	polygonData->numVertices = poly->instanceData.numVertices;
	polygonData->vertices    = newVertices;

	for ( TQ3Uns32 n = 0 ; n < polygonData->numVertices ; ++n )
		{
		polygonData->vertices[n].point = poly->instanceData.vertices[n].point;
		E3Shared_Acquire(& polygonData->vertices[n].attributeSet, poly->instanceData.vertices[n].attributeSet);
		}

	E3Shared_Acquire(&polygonData->polygonAttributeSet, poly->instanceData.polygonAttributeSet);

	return kQ3Success ;
	}





//=============================================================================
//      E3Polygon_EmptyData : Empty the data for a polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_EmptyData(TQ3PolygonData *polygonData)
{	TQ3Uns32		n;



	// Dispose of the data
	for (n = 0; n < polygonData->numVertices; n++)
		Q3Object_CleanDispose(&polygonData->vertices[n].attributeSet);

	Q3Memory_Free(&polygonData->vertices);
	Q3Object_CleanDispose(&polygonData->polygonAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Polygon_GetVertexPosition : Get the position for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_GetVertexPosition ( TQ3GeometryObject thePolygon, TQ3Uns32 index, TQ3Point3D *point )
	{
	// Get the vertex position
	*point = ( (E3Polygon*) thePolygon )->instanceData.vertices [ index ].point ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Polygon_SetVertexPosition : Set the position for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_SetVertexPosition ( TQ3GeometryObject thePolygon, TQ3Uns32 index, const TQ3Point3D *point )
	{
	// Set the vertex position
	( (E3Polygon*) thePolygon )->instanceData.vertices [ index ].point = *point ;
	
	Q3Shared_Edited ( thePolygon ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Polygon_GetVertexAttributeSet : Get the attribute set for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_GetVertexAttributeSet ( TQ3GeometryObject thePolygon, TQ3Uns32 index, TQ3AttributeSet *attributeSet )
	{
	// Get the vertex attribute set
	E3Shared_Acquire ( attributeSet, ( (E3Polygon*) thePolygon )->instanceData.vertices [ index ].attributeSet ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Polygon_SetVertexAttributeSet : Set the attribute set for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_SetVertexAttributeSet ( TQ3GeometryObject thePolygon, TQ3Uns32 index, TQ3AttributeSet attributeSet )
	{
	// Set the vertex attribute set
	E3Shared_Replace ( & ( (E3Polygon*) thePolygon )->instanceData.vertices [ index ].attributeSet, attributeSet ) ;
	
	Q3Shared_Edited ( thePolygon ) ;

	return kQ3Success ;
	}

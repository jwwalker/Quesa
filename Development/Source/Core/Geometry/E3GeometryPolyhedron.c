/*  NAME:
        E3GeometryPolyhedron.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#include "E3Set.h"
#include "E3Geometry.h"
#include "E3GeometryPolyhedron.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_polyhedron_set_edge : Set a TriMesh edge.
//-----------------------------------------------------------------------------
static void
e3geom_polyhedron_set_edge(TQ3TriMeshEdgeData			*theEdge,
							TQ3PolyhedronTriangleData	*theTriangle,
							TQ3Uns32 t1, TQ3Uns32 t2,
							TQ3Uns32 v1, TQ3Uns32 v2)
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theEdge));
	Q3_REQUIRE(Q3_VALID_PTR(theTriangle));



	// Initialise the edge
	theEdge->pointIndices[0]    = theTriangle->vertexIndices[v1];
	theEdge->pointIndices[1]    = theTriangle->vertexIndices[v2];
	theEdge->triangleIndices[0] = t1;
	theEdge->triangleIndices[1] = t2;
}





//=============================================================================
//      e3geom_polyhedron_add_vertex_attribute : Populate an attribute array.
//-----------------------------------------------------------------------------
//		Note :	Given an attribute type, we collect the data for the vertices
//				and create the appropriate attribute data for the TriMesh.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3geom_polyhedron_add_vertex_attribute(const TQ3PolyhedronData		*instanceData,
										TQ3TriMeshAttributeData		*triMeshAttribute,
										TQ3AttributeType			attributeType)
{	TQ3Uns32			n, attributeSize;
	TQ3Boolean			foundAttribute;
	TQ3AttributeSet		theAttributes;
	void				*dataPtr;
	E3ClassInfoPtr		theClass;
	


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(instanceData),     kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triMeshAttribute), kQ3False);



	// Determine if any of the vertices of the polyhedron contain this attribute. If
	// none of them do, we can skip it - but if one of them does, they all must
	// (since we're using a TriMesh).
	foundAttribute = kQ3False;
	for (n = 0; n < instanceData->numVertices && !foundAttribute; n++)
		{
		if (instanceData->vertices[n].attributeSet != NULL &&
			Q3AttributeSet_Contains(instanceData->vertices[n].attributeSet, attributeType))
			foundAttribute = kQ3True;
		}

	if (!foundAttribute)
		return(kQ3False);



	// Work out the size of data used for the attribute
	theClass = E3ClassTree_GetClassByType(E3Attribute_AttributeToClassType(attributeType));
	if (theClass == NULL)
		return(kQ3False);
	
	attributeSize = E3ClassTree_GetInstanceSize(theClass);



	// Set up the attribute array within the TriMesh
	triMeshAttribute->attributeType     = attributeType;
	triMeshAttribute->data              = Q3Memory_AllocateClear(instanceData->numVertices * attributeSize);
	triMeshAttribute->attributeUseArray = NULL;

	if (triMeshAttribute->data == NULL)
		return(kQ3False);



	// Set up the values within the attribute array
	for (n = 0; n < instanceData->numVertices; n++)
		{
		// Figure out where the data should be stored
		dataPtr = ((TQ3Uns8 *) triMeshAttribute->data) + (n * attributeSize);



		// Get the final attribute set for this vertex
		E3AttributeSet_Combine(instanceData->polyhedronAttributeSet, instanceData->vertices[n].attributeSet, &theAttributes);
		if (theAttributes != NULL)
			{
			// If the attribute is present, get the value
			if (Q3AttributeSet_Contains(theAttributes, attributeType))
				Q3AttributeSet_Get(theAttributes, attributeType, dataPtr);
			
			// Or use a default
			else
				{
				switch (attributeType) {
					case kQ3AttributeTypeAmbientCoefficient:
						*((float *) dataPtr) = kQ3ViewDefaultAmbientCoefficient;
						break;

					case kQ3AttributeTypeDiffuseColor:
						Q3ColorRGB_Set((TQ3ColorRGB *) dataPtr, kQ3ViewDefaultDiffuseColor);
						break;
						
					case kQ3AttributeTypeSpecularColor:
						Q3ColorRGB_Set((TQ3ColorRGB *) dataPtr, kQ3ViewDefaultSpecularColor);
						break;

					case kQ3AttributeTypeSpecularControl:
						*((float *) dataPtr) = kQ3ViewDefaultSpecularControl;
						break;

					case kQ3AttributeTypeTransparencyColor:
						Q3ColorRGB_Set((TQ3ColorRGB *) dataPtr, kQ3ViewDefaultTransparency);
						break;
					
					case kQ3AttributeTypeNormal:
						Q3Point3D_CrossProductTri(&instanceData->vertices[0].point,
												  &instanceData->vertices[1].point,
												  &instanceData->vertices[2].point,
												  (TQ3Vector3D *) dataPtr);
						break;

					case kQ3AttributeTypeHighlightState:
						*((TQ3Switch *) dataPtr) = kQ3ViewDefaultHighlightState;
						break;

					case kQ3AttributeTypeSurfaceUV: 
					case kQ3AttributeTypeShadingUV: 
					case kQ3AttributeTypeSurfaceTangent: 
					case kQ3AttributeTypeSurfaceShader:
					default:
						// Assume 0s will be OK
						break;
					}
				}
			
			
			// Clean up
			Q3Object_Dispose(theAttributes);
			}
		}
	
	return(kQ3True);
}





//=============================================================================
//      e3geom_polyhedron_add_edge_attribute : Populate an attribute array.
//-----------------------------------------------------------------------------
//		Note :	Given an attribute type, we collect the data for the edges
//				and create the appropriate attribute data for the TriMesh.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3geom_polyhedron_add_edge_attribute(const TQ3PolyhedronData		*instanceData,
										TQ3TriMeshAttributeData		*triMeshAttribute,
										TQ3AttributeType			attributeType)
{	TQ3Uns32			n, attributeSize;
	TQ3Boolean			foundAttribute;
	TQ3AttributeSet		theAttributes;
	void				*dataPtr;
	E3ClassInfoPtr		theClass;
	


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(instanceData),     kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triMeshAttribute), kQ3False);



	// Determine if any of the edges of the polyhedron contain this attribute. If
	// none of them do, we can skip it - but if one of them does, they all must
	// (since we're using a TriMesh).
	foundAttribute = kQ3False;
	for (n = 0; n < instanceData->numEdges && !foundAttribute; n++)
		{
		if (instanceData->edges[n].edgeAttributeSet != NULL &&
			Q3AttributeSet_Contains(instanceData->edges[n].edgeAttributeSet, attributeType))
			foundAttribute = kQ3True;
		}

	if (!foundAttribute)
		return(kQ3False);



	// Work out the size of data used for the attribute
	theClass = E3ClassTree_GetClassByType(E3Attribute_AttributeToClassType(attributeType));
	if (theClass == NULL)
		return(kQ3False);
	
	attributeSize = E3ClassTree_GetInstanceSize(theClass);



	// Set up the attribute array within the TriMesh
	triMeshAttribute->attributeType     = attributeType;
	triMeshAttribute->data              = Q3Memory_AllocateClear(instanceData->numEdges * attributeSize);
	triMeshAttribute->attributeUseArray = NULL;

	if (triMeshAttribute->data == NULL)
		return(kQ3False);



	// Set up the values within the attribute array
	for (n = 0; n < instanceData->numEdges; n++)
		{
		// Figure out where the data should be stored
		dataPtr = ((TQ3Uns8 *) triMeshAttribute->data) + (n * attributeSize);



		// Get the final attribute set for this edge
		E3AttributeSet_Combine(instanceData->polyhedronAttributeSet, instanceData->edges[n].edgeAttributeSet, &theAttributes);
		if (theAttributes != NULL)
			{
			// If the attribute is present, get the value
			if (Q3AttributeSet_Contains(theAttributes, attributeType))
				Q3AttributeSet_Get(theAttributes, attributeType, dataPtr);
			
			// Or use a default
			else
				{
				switch (attributeType) {
					case kQ3AttributeTypeAmbientCoefficient:
						*((float *) dataPtr) = kQ3ViewDefaultAmbientCoefficient;
						break;

					case kQ3AttributeTypeDiffuseColor:
						Q3ColorRGB_Set((TQ3ColorRGB *) dataPtr, kQ3ViewDefaultDiffuseColor);
						break;
						
					case kQ3AttributeTypeSpecularColor:
						Q3ColorRGB_Set((TQ3ColorRGB *) dataPtr, kQ3ViewDefaultSpecularColor);
						break;

					case kQ3AttributeTypeSpecularControl:
						*((float *) dataPtr) = kQ3ViewDefaultSpecularControl;
						break;

					case kQ3AttributeTypeTransparencyColor:
						Q3ColorRGB_Set((TQ3ColorRGB *) dataPtr, kQ3ViewDefaultTransparency);
						break;
					
					case kQ3AttributeTypeNormal:
						// Assume anything will beOK
						Q3Vector3D_Set((TQ3Vector3D *) dataPtr, 0.0f, 1.0f, 0.0f);
						break;

					case kQ3AttributeTypeHighlightState:
						*((TQ3Switch *) dataPtr) = kQ3ViewDefaultHighlightState;
						break;

					case kQ3AttributeTypeSurfaceUV: 
					case kQ3AttributeTypeShadingUV: 
					case kQ3AttributeTypeSurfaceTangent: 
					case kQ3AttributeTypeSurfaceShader:
					default:
						// Assume 0s will be OK
						break;
					}
				}
			
			
			// Clean up
			Q3Object_Dispose(theAttributes);
			}
		}
	
	return(kQ3True);
}





//=============================================================================
//      e3geom_polyhedron_new : Polyhedron new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyhedron_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PolyhedronData			*instanceData	= (TQ3PolyhedronData *)       privateData;
	const TQ3PolyhedronData		*polyhedronData = (const TQ3PolyhedronData *) paramData;
	TQ3Status					qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3PolyhedronData));
	
	qd3dStatus = Q3Polyhedron_SetData(theObject, polyhedronData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_polyhedron_delete : Polyhedron delete method.
//-----------------------------------------------------------------------------
static void
e3geom_polyhedron_delete(TQ3Object theObject, void *privateData)
{	TQ3PolyhedronData		*instanceData = (TQ3PolyhedronData *) privateData;
	TQ3Status				qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3Polyhedron_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_polyhedron_duplicate : Polyhedron duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyhedron_duplicate(TQ3Object fromObject, const void *fromPrivateData,
							TQ3Object toObject,   void       *toPrivateData)
{	TQ3PolyhedronData		*toInstanceData = (TQ3PolyhedronData *) toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Polyhedron_GetData(fromObject, toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_polyhedron_cache_new : Polyhedron cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_polyhedron_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3PolyhedronData *geomData)
{	TQ3TriMeshAttributeData		vertexAttributes[kQ3AttributeTypeNumTypes];
	TQ3TriMeshAttributeData		edgeAttributes[kQ3AttributeTypeNumTypes];
	TQ3TriMeshAttributeData		*vertexColours;
	TQ3Uns32					n, m, numEdges;
	TQ3TriMeshTriangleData		*theTriangles;
	TQ3TriMeshData				triMeshData;
	TQ3GeometryObject			theTriMesh;
	TQ3Point3D					*thePoints;
	TQ3TriMeshEdgeData			*theEdges;
#pragma unused(theView)



	// Decide if we need to create the edges ourselves, or if they've already been
	// specified. If we need to create them ourselves, we also need to run through
	// the triangles to see how many we actually need (if any).
	if (geomData->numEdges != 0)
		numEdges = geomData->numEdges;

	else
		{
		numEdges = 0;
		for (n = 0; n < geomData->numTriangles; n++)
			{
			if (geomData->triangles[n].edgeFlag & kQ3PolyhedronEdge01)
				numEdges++;

			if (geomData->triangles[n].edgeFlag & kQ3PolyhedronEdge12)
				numEdges++;

			if (geomData->triangles[n].edgeFlag & kQ3PolyhedronEdge20)
				numEdges++;
			}
		}



	// Allocate the memory we need for the TriMesh data
	thePoints    = (TQ3Point3D *)             Q3Memory_Allocate(geomData->numVertices  * sizeof(TQ3Point3D));
	theTriangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate(geomData->numTriangles * sizeof(TQ3TriMeshTriangleData));
	theEdges     = NULL;
	
	if (numEdges != 0)
		theEdges = (TQ3TriMeshEdgeData *) Q3Memory_Allocate(numEdges * sizeof(TQ3TriMeshEdgeData));

	if (thePoints == NULL || theTriangles == NULL || (theEdges == NULL && numEdges != 0))
		{
		Q3Memory_Free(&thePoints);
		Q3Memory_Free(&theTriangles);
		Q3Memory_Free(&theEdges);
		
		return(NULL);
		}



	// Initialise the points and triangles
	for (n = 0; n < geomData->numVertices; n++)
		thePoints[n] = geomData->vertices[n].point;
		
	for (n = 0; n < geomData->numTriangles; n++)
		memcpy(theTriangles[n].pointIndices, geomData->triangles[n].vertexIndices, sizeof(theTriangles[n].pointIndices));



	// Initialise the edges
	if (numEdges != 0)
		{
		// If they've been supplied, use them as-is
		if (geomData->numEdges != 0)
			{
			for (n = 0; n < geomData->numEdges; n++)
				{
				memcpy(theEdges[n].pointIndices,    geomData->edges[n].vertexIndices,   sizeof(theEdges[n].pointIndices));
				memcpy(theEdges[n].triangleIndices, geomData->edges[n].triangleIndices, sizeof(theEdges[n].triangleIndices));
				}
			}
		
		
		// Otherwise, create them by hand
		else
			{
			m = 0;
			for (n = 0; n < geomData->numTriangles; n++)
				{
				if (geomData->triangles[n].edgeFlag & kQ3PolyhedronEdge01)
					{
					e3geom_polyhedron_set_edge(&theEdges[m], &geomData->triangles[n], n, n, 0, 1);
					m++;
					}

				if (geomData->triangles[n].edgeFlag & kQ3PolyhedronEdge12)
					{
					e3geom_polyhedron_set_edge(&theEdges[m], &geomData->triangles[n], n, n, 1, 2);
					m++;
					}

				if (geomData->triangles[n].edgeFlag & kQ3PolyhedronEdge20)
					{
					e3geom_polyhedron_set_edge(&theEdges[m], &geomData->triangles[n], n, n, 2, 0);
					m++;
					}
				}
			Q3_ASSERT(m == numEdges);
			}
		}



	// Initialise the TriMesh data
	triMeshData.numPoints                 = geomData->numVertices;
	triMeshData.points                    = thePoints;
	triMeshData.numTriangles              = geomData->numTriangles;
	triMeshData.triangles                 = theTriangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = numEdges;
	triMeshData.edges                     = theEdges;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 0;
	triMeshData.vertexAttributeTypes      = NULL;
	triMeshData.triMeshAttributeSet       = geomData->polyhedronAttributeSet;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox, triMeshData.points, triMeshData.numPoints, sizeof(TQ3Point3D));



	// Set up the vertex attributes (saving any colours we find)
	vertexColours = NULL;
	n             = 0;

	if (e3geom_polyhedron_add_vertex_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeNormal))
		n++;
		
	if (e3geom_polyhedron_add_vertex_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSurfaceUV))
		n++;
	else if (e3geom_polyhedron_add_vertex_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeShadingUV))
		n++;

	if (e3geom_polyhedron_add_vertex_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeAmbientCoefficient))
		n++;

	if (e3geom_polyhedron_add_vertex_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeDiffuseColor))
		{
		vertexColours = &vertexAttributes[n];
		n++;
		}
		
	if (e3geom_polyhedron_add_vertex_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSpecularColor))
		n++;
		
	if (e3geom_polyhedron_add_vertex_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSpecularControl))
		n++;
		
	if (e3geom_polyhedron_add_vertex_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeTransparencyColor))
		n++;
		
	if (e3geom_polyhedron_add_vertex_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeHighlightState))
		n++;
		
	if (e3geom_polyhedron_add_vertex_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSurfaceShader))
		n++;

	Q3_ASSERT(n < (sizeof(vertexAttributes) / sizeof(TQ3TriMeshAttributeData)));
	if (n != 0)
		{
		triMeshData.numVertexAttributeTypes = n;
		triMeshData.vertexAttributeTypes    = vertexAttributes;
		}



	// Set up the edge attributes
	n = 0;

	if (e3geom_polyhedron_add_edge_attribute(geomData, &edgeAttributes[n], kQ3AttributeTypeNormal))
		n++;
		
	if (e3geom_polyhedron_add_edge_attribute(geomData, &edgeAttributes[n], kQ3AttributeTypeAmbientCoefficient))
		n++;

	if (e3geom_polyhedron_add_edge_attribute(geomData, &edgeAttributes[n], kQ3AttributeTypeDiffuseColor))
		n++;
	else if (vertexColours != NULL)
		{
		// If we can reuse the vertex colours, just fall back to them
		if (geomData->numEdges <= geomData->numVertices)
			{
			edgeAttributes[n] = *vertexColours;
			n++;
			}
		}
		
	if (e3geom_polyhedron_add_edge_attribute(geomData, &edgeAttributes[n], kQ3AttributeTypeSpecularColor))
		n++;
		
	if (e3geom_polyhedron_add_edge_attribute(geomData, &edgeAttributes[n], kQ3AttributeTypeSpecularControl))
		n++;
		
	if (e3geom_polyhedron_add_edge_attribute(geomData, &edgeAttributes[n], kQ3AttributeTypeTransparencyColor))
		n++;
		
	if (e3geom_polyhedron_add_edge_attribute(geomData, &edgeAttributes[n], kQ3AttributeTypeHighlightState))
		n++;
		
	if (e3geom_polyhedron_add_edge_attribute(geomData, &edgeAttributes[n], kQ3AttributeTypeSurfaceShader))
		n++;

	Q3_ASSERT(n < (sizeof(edgeAttributes) / sizeof(TQ3TriMeshAttributeData)));
	if (n != 0)
		{
		triMeshData.numEdgeAttributeTypes = n;
		triMeshData.edgeAttributeTypes    = edgeAttributes;
		}



	// Create the TriMesh and clean up
	theTriMesh = Q3TriMesh_New(&triMeshData);

	Q3Memory_Free(&thePoints);
	Q3Memory_Free(&theTriangles);
	Q3Memory_Free(&theEdges);
	
	for (n = 0; n < triMeshData.numVertexAttributeTypes; n++)
		Q3Memory_Free(&triMeshData.vertexAttributeTypes[n].data);

	return(theTriMesh);
}





//=============================================================================
//      e3geom_polyhedron_pick : Polyhedron picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyhedron_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_polyhedron_bounds : Polyhedron bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyhedron_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3PolyhedronData			*instanceData = (const TQ3PolyhedronData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	E3View_UpdateBounds(theView,
						instanceData->numVertices,
						sizeof(TQ3Vertex3D),
						&instanceData->vertices[0].point);
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_polyhedron_get_attribute : Polyhedron get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_polyhedron_get_attribute(TQ3GeometryObject theObject)
{	TQ3PolyhedronData		*instanceData = (TQ3PolyhedronData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->polyhedronAttributeSet);
}





//=============================================================================
//      e3geom_polyhedron_metahandler : Polyhedron metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_polyhedron_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_polyhedron_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_polyhedron_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_polyhedron_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_polyhedron_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_polyhedron_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_polyhedron_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_polyhedron_get_attribute;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryPolyhedron_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryPolyhedron_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypePolyhedron,
											kQ3ClassNameGeometryPolyhedron,
											e3geom_polyhedron_metahandler,
											sizeof(TQ3PolyhedronData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryPolyhedron_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryPolyhedron_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypePolyhedron, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Polyhedron_New : Create a polyhedron object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Polyhedron_New(const TQ3PolyhedronData *polyhedronData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypePolyhedron, kQ3False, polyhedronData);
	return(theObject);
}





//=============================================================================
//      E3Polyhedron_Submit : Submit a polyhedron.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_Submit(const TQ3PolyhedronData *polyhedronData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypePolyhedron, polyhedronData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Polyhedron_SetData : Set the data for a polyhedron.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_SetData(TQ3GeometryObject thePolyhedron, const TQ3PolyhedronData *polyhedronData)
{	TQ3PolyhedronData			*instanceData = (TQ3PolyhedronData *) thePolyhedron->instanceData;
	TQ3PolyhedronTriangleData	*newTriangles;
	TQ3Vertex3D					*newVertices;
	TQ3PolyhedronEdgeData		*newEdges;
	TQ3Uns32					n;



	// Allocate the space for the new data
	newVertices  = (TQ3Vertex3D *)               Q3Memory_Allocate(polyhedronData->numVertices  * sizeof(TQ3Vertex3D));
	newTriangles = (TQ3PolyhedronTriangleData *) Q3Memory_Allocate(polyhedronData->numTriangles * sizeof(TQ3PolyhedronTriangleData));
	newEdges     = NULL;
	
	if (polyhedronData->edges != NULL)
		newEdges = (TQ3PolyhedronEdgeData *) Q3Memory_Allocate(polyhedronData->numEdges * sizeof(TQ3PolyhedronEdgeData));



	// Handle failure
	if (newVertices == NULL || newTriangles == NULL || (newEdges == NULL && polyhedronData->edges != NULL))
		{
		Q3Memory_Free(&newVertices);
		Q3Memory_Free(&newTriangles);
		Q3Memory_Free(&newEdges);
		
		return(kQ3Failure);
		}



	// Dispose of the existing data
	for (n = 0; n < instanceData->numVertices; n++)
		E3Object_DisposeAndForget(instanceData->vertices[n].attributeSet);

	for (n = 0; n < instanceData->numTriangles; n++)
		E3Object_DisposeAndForget(instanceData->triangles[n].triangleAttributeSet);

	for (n = 0; n < instanceData->numEdges; n++)
		E3Object_DisposeAndForget(instanceData->edges[n].edgeAttributeSet);

	Q3Memory_Free(&instanceData->vertices);
	Q3Memory_Free(&instanceData->triangles);
	Q3Memory_Free(&instanceData->edges);



	// Copy the new data
	instanceData->numVertices  = polyhedronData->numVertices;
	instanceData->vertices     = newVertices;
	instanceData->numEdges     = polyhedronData->numEdges;
	instanceData->edges        = newEdges;
	instanceData->numTriangles = polyhedronData->numTriangles;
	instanceData->triangles    = newTriangles;
	
	E3Shared_Replace(&instanceData->polyhedronAttributeSet, polyhedronData->polyhedronAttributeSet);
	
	for (n = 0; n < instanceData->numVertices; n++)
		{
		instanceData->vertices[n].point = polyhedronData->vertices[n].point;
		E3Shared_Acquire( &instanceData->vertices[n].attributeSet,
			             polyhedronData->vertices[n].attributeSet);
		}
	
	for (n = 0; n < instanceData->numTriangles; n++)
		{
		memcpy(  instanceData->triangles[n].vertexIndices,
			   polyhedronData->triangles[n].vertexIndices,
			   sizeof(instanceData->triangles[n].vertexIndices));

		instanceData->triangles[n].edgeFlag = polyhedronData->triangles[n].edgeFlag;

		E3Shared_Acquire( &instanceData->triangles[n].triangleAttributeSet,
			             polyhedronData->triangles[n].triangleAttributeSet);
		}
	
	for (n = 0; n < instanceData->numEdges; n++)
		{
		memcpy(  instanceData->edges[n].vertexIndices,
			   polyhedronData->edges[n].vertexIndices,
			   sizeof(instanceData->edges[n].vertexIndices));

		memcpy(  instanceData->edges[n].triangleIndices,
			   polyhedronData->edges[n].triangleIndices,
			   sizeof(instanceData->edges[n].triangleIndices));

		E3Shared_Acquire( &instanceData->edges[n].edgeAttributeSet,
			             polyhedronData->edges[n].edgeAttributeSet);
		}

	Q3Shared_Edited(thePolyhedron);

	return(kQ3Success);
}





//=============================================================================
//      E3Polyhedron_GetData : Get the data for a polyhedron.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_GetData(TQ3GeometryObject thePolyhedron, TQ3PolyhedronData *polyhedronData)
{	TQ3PolyhedronData			*instanceData = (TQ3PolyhedronData *) thePolyhedron->instanceData;
	TQ3PolyhedronTriangleData	*newTriangles;
	TQ3Vertex3D					*newVertices;
	TQ3PolyhedronEdgeData		*newEdges;
	TQ3Uns32					n;



	// Allocate the space for the new data
	newVertices  = (TQ3Vertex3D *)               Q3Memory_Allocate(instanceData->numVertices  * sizeof(TQ3Vertex3D));
	newTriangles = (TQ3PolyhedronTriangleData *) Q3Memory_Allocate(instanceData->numTriangles * sizeof(TQ3PolyhedronTriangleData));
	newEdges     = NULL;
	
	if (instanceData->edges != NULL)
		newEdges = (TQ3PolyhedronEdgeData *) Q3Memory_Allocate(instanceData->numEdges * sizeof(TQ3PolyhedronEdgeData));



	// Handle failure
	if (newVertices == NULL || newTriangles == NULL || (newEdges == NULL && instanceData->edges != NULL))
		{
		Q3Memory_Free(&newVertices);
		Q3Memory_Free(&newTriangles);
		Q3Memory_Free(&newEdges);
		
		return(kQ3Failure);
		}



	// Return the new data
	polyhedronData->numVertices  = instanceData->numVertices;
	polyhedronData->vertices     = newVertices;
	polyhedronData->numEdges     = instanceData->numEdges;
	polyhedronData->edges        = newEdges;
	polyhedronData->numTriangles = instanceData->numTriangles;
	polyhedronData->triangles    = newTriangles;
	
	E3Shared_Replace(&polyhedronData->polyhedronAttributeSet, instanceData->polyhedronAttributeSet);
	
	for (n = 0; n < polyhedronData->numVertices; n++)
		{
		polyhedronData->vertices[n].point = instanceData->vertices[n].point;
		E3Shared_Acquire(&polyhedronData->vertices[n].attributeSet,
			                instanceData->vertices[n].attributeSet);
		}
	
	for (n = 0; n < polyhedronData->numTriangles; n++)
		{
		memcpy(polyhedronData->triangles[n].vertexIndices,
			     instanceData->triangles[n].vertexIndices,
			     sizeof(polyhedronData->triangles[n].vertexIndices));

		polyhedronData->triangles[n].edgeFlag = instanceData->triangles[n].edgeFlag;

		E3Shared_Acquire(&polyhedronData->triangles[n].triangleAttributeSet,
			                instanceData->triangles[n].triangleAttributeSet);
		}
	
	for (n = 0; n < polyhedronData->numEdges; n++)
		{
		memcpy(polyhedronData->edges[n].vertexIndices,
			     instanceData->edges[n].vertexIndices,
			     sizeof(polyhedronData->edges[n].vertexIndices));

		memcpy(polyhedronData->edges[n].triangleIndices,
			     instanceData->edges[n].triangleIndices,
			     sizeof(polyhedronData->edges[n].triangleIndices));

		E3Shared_Acquire(&polyhedronData->edges[n].edgeAttributeSet,
			                instanceData->edges[n].edgeAttributeSet);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3Polyhedron_EmptyData : Release the data for a polyhedron.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_EmptyData(TQ3PolyhedronData *polyhedronData)
{	TQ3Uns32		n;



	// Release the data
	for (n = 0; n < polyhedronData->numVertices; n++)
		E3Object_DisposeAndForget(polyhedronData->vertices[n].attributeSet);

	for (n = 0; n < polyhedronData->numTriangles; n++)
		E3Object_DisposeAndForget(polyhedronData->triangles[n].triangleAttributeSet);

	for (n = 0; n < polyhedronData->numEdges; n++)
		E3Object_DisposeAndForget(polyhedronData->edges[n].edgeAttributeSet);

	Q3Memory_Free(&polyhedronData->vertices);
	Q3Memory_Free(&polyhedronData->triangles);
	Q3Memory_Free(&polyhedronData->edges);

	E3Object_DisposeAndForget(polyhedronData->polyhedronAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Polyhedron_GetVertexPosition : Get the position of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_GetVertexPosition(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, TQ3Point3D *point)
{	TQ3PolyhedronData		*instanceData = (TQ3PolyhedronData *) thePolyhedron->instanceData;



	// Get the vertex position
	*point = instanceData->vertices[index].point;

	return(kQ3Success);
}





//=============================================================================
//      E3Polyhedron_SetVertexPosition : Set the position of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_SetVertexPosition(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, const TQ3Point3D *point)
{	TQ3PolyhedronData		*instanceData = (TQ3PolyhedronData *) thePolyhedron->instanceData;



	// Set the vertex position
	instanceData->vertices[index].point = *point;

	Q3Shared_Edited(thePolyhedron);

	return(kQ3Success);
}





//=============================================================================
//      E3Polyhedron_GetVertexAttributeSet : Get the attribute set of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_GetVertexAttributeSet(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
{	TQ3PolyhedronData		*instanceData = (TQ3PolyhedronData *) thePolyhedron->instanceData;



	// Get the vertex attribute set
	E3Shared_Acquire(attributeSet, instanceData->vertices[index].attributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Polyhedron_SetVertexAttributeSet : Set the attribute set of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_SetVertexAttributeSet(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, TQ3AttributeSet attributeSet)
{	TQ3PolyhedronData		*instanceData = (TQ3PolyhedronData *) thePolyhedron->instanceData;



	// Set the vertex attribute set
	E3Shared_Replace(&instanceData->vertices[index].attributeSet, attributeSet);

	Q3Shared_Edited(thePolyhedron);

	return(kQ3Success);
}





//=============================================================================
//      E3Polyhedron_GetTriangleData : Get the data for a triangle.
//-----------------------------------------------------------------------------
//		Note :	Unclear if QD3D does a bitwise copy or updates the attribute
//				set reference count as we do - may need to change to a simple
//				bitwise copy to prevent memory leaks.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_GetTriangleData(TQ3GeometryObject thePolyhedron, TQ3Uns32 triangleIndex, TQ3PolyhedronTriangleData *triangleData)
{	TQ3PolyhedronData		*instanceData = (TQ3PolyhedronData *) thePolyhedron->instanceData;



	// Get the triangle data
	memcpy(triangleData->vertexIndices,
		   instanceData->triangles[triangleIndex].vertexIndices,
		   sizeof(triangleData->vertexIndices));

	triangleData->edgeFlag = instanceData->triangles[triangleIndex].edgeFlag;

	E3Shared_Acquire(&triangleData->triangleAttributeSet,
		              instanceData->triangles[triangleIndex].triangleAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Polyhedron_SetTriangleData : Set the data for a triangle.
//-----------------------------------------------------------------------------
//		Note :	Unclear if QD3D does a bitwise copy or updates the attribute
//				set reference count as we do - may need to change to a simple
//				bitwise copy to prevent memory leaks.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_SetTriangleData(TQ3GeometryObject thePolyhedron, TQ3Uns32 triangleIndex, const TQ3PolyhedronTriangleData *triangleData)
{	TQ3PolyhedronData		*instanceData = (TQ3PolyhedronData *) thePolyhedron->instanceData;



	// Set the triangle data
	memcpy(instanceData->triangles[triangleIndex].vertexIndices,
		   triangleData->vertexIndices,
		   sizeof(instanceData->triangles[triangleIndex].vertexIndices));

	instanceData->triangles[triangleIndex].edgeFlag = triangleData->edgeFlag;

	E3Shared_Acquire(&instanceData->triangles[triangleIndex].triangleAttributeSet,
					  triangleData->triangleAttributeSet);

	Q3Shared_Edited(thePolyhedron);

	return(kQ3Success);
}





//=============================================================================
//      E3Polyhedron_GetEdgeData : Get the data for an edge.
//-----------------------------------------------------------------------------
//		Note :	Unclear if QD3D does a bitwise copy or updates the attribute
//				set reference count as we do - may need to change to a simple
//				bitwise copy to prevent memory leaks.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_GetEdgeData(TQ3GeometryObject thePolyhedron, TQ3Uns32 edgeIndex, TQ3PolyhedronEdgeData *edgeData)
{	TQ3PolyhedronData		*instanceData = (TQ3PolyhedronData *) thePolyhedron->instanceData;



	// Get the edge data
	memcpy(edgeData->vertexIndices,
		   instanceData->edges[edgeIndex].vertexIndices,
		   sizeof(edgeData->vertexIndices));

	memcpy(edgeData->triangleIndices,
		   instanceData->edges[edgeIndex].triangleIndices,
		   sizeof(edgeData->triangleIndices));

	E3Shared_Acquire(&edgeData->edgeAttributeSet,
		              instanceData->edges[edgeIndex].edgeAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Polyhedron_SetEdgeData : Set the data for an edge.
//-----------------------------------------------------------------------------
//		Note :	Unclear if QD3D does a bitwise copy or updates the attribute
//				set reference count as we do - may need to change to a simple
//				bitwise copy to prevent memory leaks.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_SetEdgeData(TQ3GeometryObject thePolyhedron, TQ3Uns32 edgeIndex, const TQ3PolyhedronEdgeData *edgeData)
{	TQ3PolyhedronData		*instanceData = (TQ3PolyhedronData *) thePolyhedron->instanceData;



	// Set the edge data
	memcpy(instanceData->edges[edgeIndex].vertexIndices,
		   edgeData->vertexIndices,
		   sizeof(instanceData->edges[edgeIndex].vertexIndices));

	memcpy(instanceData->edges[edgeIndex].triangleIndices,
		   edgeData->triangleIndices,
		   sizeof(instanceData->edges[edgeIndex].triangleIndices));

	E3Shared_Acquire(&instanceData->edges[edgeIndex].edgeAttributeSet,
					  edgeData->edgeAttributeSet);

	Q3Shared_Edited(thePolyhedron);

	return(kQ3Success);
}

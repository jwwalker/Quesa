/*  NAME:
        E3GeometryPolygon.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#include "E3GeometryPolygon.h"
#include "E3Set.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_polygon_add_attribute : Populate an attribute array.
//-----------------------------------------------------------------------------
//		Note :	Given an attribute type, we collect the data for the vertices
//				and create the appropriate attribute data for the TriMesh.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3geom_polygon_add_attribute(const TQ3PolygonData			*instanceData,
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



	// Determine if any of the vertices of the polygon contain this attribute. If
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
	triMeshAttribute->data              = E3Memory_AllocateClear(instanceData->numVertices * attributeSize);
	triMeshAttribute->attributeUseArray = NULL;

	if (triMeshAttribute->data == NULL)
		return(kQ3False);



	// Set up the values within the attribute array
	for (n = 0; n < instanceData->numVertices; n++)
		{
		// Figure out where the data should be stored
		dataPtr = ((TQ3Uns8 *) triMeshAttribute->data) + (n * attributeSize);



		// Get the final attribute set for this vertex
		E3AttributeSet_Combine(instanceData->polygonAttributeSet, instanceData->vertices[n].attributeSet, &theAttributes);
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
//      e3geom_polygon_new : Polygon new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polygon_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PolygonData			*instanceData = (TQ3PolygonData *)       privateData;
	const TQ3PolygonData	*polygonData  = (const TQ3PolygonData *) paramData;
	TQ3Status				qd3dStatus;



	// Initialise our instance data
	E3Memory_Clear(instanceData, sizeof(TQ3PolygonData));
	
	qd3dStatus = Q3Polygon_SetData(theObject, polygonData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_polygon_delete : Polygon delete method.
//-----------------------------------------------------------------------------
static void
e3geom_polygon_delete(TQ3Object theObject, void *privateData)
{	TQ3PolygonData		*instanceData = (TQ3PolygonData *) privateData;
	TQ3Status				qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3Polygon_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_polygon_duplicate : Polygon duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polygon_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	TQ3PolygonData		*toInstanceData = (TQ3PolygonData *) toPrivateData;
	TQ3Status			qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Polygon_GetData(fromObject, toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_polygon_cache_new : Polygon cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_polygon_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3PolygonData *geomData)
{	TQ3Uns32					n, numEdges, numTriangles;
	TQ3TriMeshAttributeData		vertexAttributes[kQ3AttributeTypeNumTypes];
	TQ3TriMeshAttributeData		edgeAttributes[1];
	TQ3TriMeshTriangleData		*theTriangles;
	TQ3TriMeshData				triMeshData;
	TQ3GeometryObject			theTriMesh;
	TQ3Point3D					*thePoints;
	TQ3TriMeshEdgeData			*theEdges;
#pragma unused(theView)



	// Work out how many edges and triangles we need
	Q3_ASSERT(geomData->numVertices >= 3);
	numEdges     = geomData->numVertices;
	numTriangles = geomData->numVertices - 2;



	// Allocate the memory we need for the TriMesh data
	thePoints    = (TQ3Point3D *)             E3Memory_Allocate(geomData->numVertices * sizeof(TQ3Point3D));
	theTriangles = (TQ3TriMeshTriangleData *) E3Memory_Allocate(numTriangles * sizeof(TQ3TriMeshTriangleData));
	theEdges     = (TQ3TriMeshEdgeData *)     E3Memory_Allocate(numEdges     * sizeof(TQ3TriMeshEdgeData));

	if (thePoints == NULL || theTriangles == NULL || theEdges == NULL)
		{
		E3Memory_Free(&thePoints);
		E3Memory_Free(&theTriangles);
		E3Memory_Free(&theEdges);
		
		return(NULL);
		}



	// Initialise the points, edges, and triangles
	for (n = 0; n < geomData->numVertices; n++)
		thePoints[n] = geomData->vertices[n].point;

	for (n = 0; n < numEdges; n++)
		{
		theEdges[n].pointIndices[0]    = n;
		theEdges[n].pointIndices[1]    = (n == (numEdges-1)) ? 0 : (n + 1);
		theEdges[n].triangleIndices[1] = kQ3ArrayIndexNULL;
		theEdges[n].triangleIndices[1] = kQ3ArrayIndexNULL;
		}
		
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
	
	if (e3geom_polygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeNormal))
		n++;
		
	if (e3geom_polygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSurfaceUV))
		n++;
	else if (e3geom_polygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeShadingUV))
		n++;
		
	if (e3geom_polygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeAmbientCoefficient))
		n++;

	if (e3geom_polygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeDiffuseColor))
		{
		// Set up some edge colours as well, just reusing the vertex colours
		Q3_ASSERT(numEdges == geomData->numVertices);
		edgeAttributes[0] = vertexAttributes[n];
		
		triMeshData.numEdgeAttributeTypes   = 1;
		triMeshData.edgeAttributeTypes      = edgeAttributes;
		n++;
		}
		
	if (e3geom_polygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSpecularColor))
		n++;
		
	if (e3geom_polygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSpecularControl))
		n++;
		
	if (e3geom_polygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeTransparencyColor))
		n++;
		
	if (e3geom_polygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeHighlightState))
		n++;
		
	if (e3geom_polygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSurfaceShader))
		n++;

	Q3_ASSERT(n < (sizeof(vertexAttributes) / sizeof(TQ3TriMeshAttributeData)));
	if (n != 0)
		{
		triMeshData.numVertexAttributeTypes = n;
		triMeshData.vertexAttributeTypes    = vertexAttributes;
		}



	// Create the TriMesh and clean up
	theTriMesh = Q3TriMesh_New(&triMeshData);

	E3Memory_Free(&thePoints);
	E3Memory_Free(&theTriangles);
	E3Memory_Free(&theEdges);
	
	for (n = 0; n < triMeshData.numVertexAttributeTypes; n++)
		E3Memory_Free(&triMeshData.vertexAttributeTypes[n].data);

	return(theTriMesh);
}





//=============================================================================
//      e3geom_polygon_pick : Polygon picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polygon_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
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
{	TQ3PolygonData		*instanceData = (TQ3PolygonData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->polygonAttributeSet);
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

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_polygon_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_polygon_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_polygon_get_attribute;
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
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypePolygon,
											kQ3ClassNameGeometryPolygon,
											e3geom_polygon_metahandler,
											sizeof(TQ3PolygonData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryPolygon_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryPolygon_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypePolygon, kQ3True);
	
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
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypePolygon, kQ3False, polygonData);
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
E3Polygon_SetData(TQ3GeometryObject thePolygon, const TQ3PolygonData *polygonData)
{	TQ3PolygonData		*instanceData = (TQ3PolygonData *) thePolygon->instanceData;
	TQ3Vertex3D			*newVertices;
	TQ3Uns32			n;



	// Allocate some space for the new vertices
	newVertices = (TQ3Vertex3D *) E3Memory_Allocate(polygonData->numVertices * sizeof(TQ3Vertex3D));
	if (newVertices == NULL)
		return(kQ3Failure);



	// Dispose of the existing data
	for (n = 0; n < instanceData->numVertices; n++)
		E3Object_DisposeAndForget(instanceData->vertices[n].attributeSet);

	E3Memory_Free(&instanceData->vertices);



	// Copy the new data
	instanceData->numVertices = polygonData->numVertices;
	instanceData->vertices    = newVertices;

	for (n = 0; n < instanceData->numVertices; n++)
		{
		instanceData->vertices[n].point = polygonData->vertices[n].point;
		E3Shared_Acquire(&instanceData->vertices[n].attributeSet, polygonData->vertices[n].attributeSet);
		}

	E3Shared_Replace(&instanceData->polygonAttributeSet, polygonData->polygonAttributeSet);

	Q3Shared_Edited(thePolygon);

	return(kQ3Success);
}





//=============================================================================
//      E3Polygon_GetData : Get the data for a polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_GetData(TQ3GeometryObject thePolygon, TQ3PolygonData *polygonData)
{	TQ3PolygonData		*instanceData = (TQ3PolygonData *) thePolygon->instanceData;
	TQ3Vertex3D			*newVertices;
	TQ3Uns32			n;



	// Allocate some space for the new vertices
	newVertices = (TQ3Vertex3D *) E3Memory_Allocate(instanceData->numVertices * sizeof(TQ3Vertex3D));
	if (newVertices == NULL)
		return(kQ3Failure);



	// Copy the data
	polygonData->numVertices = instanceData->numVertices;
	polygonData->vertices    = newVertices;

	for (n = 0; n < polygonData->numVertices; n++)
		{
		polygonData->vertices[n].point = instanceData->vertices[n].point;
		E3Shared_Acquire(& polygonData->vertices[n].attributeSet, instanceData->vertices[n].attributeSet);
		}

	E3Shared_Acquire(&polygonData->polygonAttributeSet, instanceData->polygonAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Polygon_EmptyData : Empty the data for a polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_EmptyData(TQ3PolygonData *polygonData)
{	TQ3Uns32		n;



	// Dispose of the data
	for (n = 0; n < polygonData->numVertices; n++)
		E3Object_DisposeAndForget(polygonData->vertices[n].attributeSet);

	E3Memory_Free(&polygonData->vertices);
	E3Object_DisposeAndForget(polygonData->polygonAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Polygon_GetVertexPosition : Get the position for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_GetVertexPosition(TQ3GeometryObject thePolygon, unsigned long index, TQ3Point3D *point)
{	TQ3PolygonData		*instanceData = (TQ3PolygonData *) thePolygon->instanceData;



	// Get the vertex position
	*point = instanceData->vertices[index].point;
	
	return(kQ3Success);
}





//=============================================================================
//      E3Polygon_SetVertexPosition : Set the position for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_SetVertexPosition(TQ3GeometryObject thePolygon, unsigned long index, const TQ3Point3D *point)
{	TQ3PolygonData		*instanceData = (TQ3PolygonData *) thePolygon->instanceData;



	// Set the vertex position
	instanceData->vertices[index].point = *point;
	
	Q3Shared_Edited(thePolygon);

	return(kQ3Success);
}





//=============================================================================
//      E3Polygon_GetVertexAttributeSet : Get the attribute set for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_GetVertexAttributeSet(TQ3GeometryObject thePolygon, unsigned long index, TQ3AttributeSet *attributeSet)
{	TQ3PolygonData		*instanceData = (TQ3PolygonData *) thePolygon->instanceData;



	// Get the vertex attribute set
	E3Shared_Acquire(attributeSet, instanceData->vertices[index].attributeSet);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Polygon_SetVertexAttributeSet : Set the attribute set for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polygon_SetVertexAttributeSet(TQ3GeometryObject thePolygon, unsigned long index, TQ3AttributeSet attributeSet)
{	TQ3PolygonData		*instanceData = (TQ3PolygonData *) thePolygon->instanceData;



	// Set the vertex attribute set
	E3Shared_Replace(&instanceData->vertices[index].attributeSet, attributeSet);
	
	Q3Shared_Edited(thePolygon);

	return(kQ3Success);
}

/*  NAME:
        E3GeometryGeneralPolygon.c

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
#include "E3GeometryGeneralPolygon.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_generalpolygon_add_attribute : Populate an attribute array.
//-----------------------------------------------------------------------------
//		Note :	Given an attribute type, we collect the data for the vertices
//				and create the appropriate attribute data for the TriMesh.
//
//				This routine will need revising when we support holes: right
//				now it just uses the first contour.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3geom_generalpolygon_add_attribute(const TQ3GeneralPolygonData		*instanceData,
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
	for (n = 0; n < instanceData->contours[0].numVertices && !foundAttribute; n++)
		{
		if (instanceData->contours[0].vertices[n].attributeSet != NULL &&
			Q3AttributeSet_Contains(instanceData->contours[0].vertices[n].attributeSet, attributeType))
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
	triMeshAttribute->data              = Q3Memory_AllocateClear(instanceData->contours[0].numVertices * attributeSize);
	triMeshAttribute->attributeUseArray = NULL;

	if (triMeshAttribute->data == NULL)
		return(kQ3False);



	// Set up the values within the attribute array
	for (n = 0; n < instanceData->contours[0].numVertices; n++)
		{
		// Figure out where the data should be stored
		dataPtr = ((TQ3Uns8 *) triMeshAttribute->data) + (n * attributeSize);



		// Get the final attribute set for this vertex
		E3AttributeSet_Combine(instanceData->generalPolygonAttributeSet,
							   instanceData->contours[0].vertices[n].attributeSet,
							   &theAttributes);
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
						Q3Point3D_CrossProductTri(&instanceData->contours[0].vertices[0].point,
												  &instanceData->contours[0].vertices[1].point,
												  &instanceData->contours[0].vertices[2].point,
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
//      e3geom_generalpolygon_new : GeneralPolygon new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3GeneralPolygonData			*instanceData		= (TQ3GeneralPolygonData *)       privateData;
	const TQ3GeneralPolygonData		*generalPolygonData = (const TQ3GeneralPolygonData *) paramData;
	TQ3Status						qd3dStatus;
#pragma unused(theObject)



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3GeneralPolygonData));
	
	qd3dStatus = Q3GeneralPolygon_SetData(theObject, generalPolygonData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_generalpolygon_delete : GeneralPolygon delete method.
//-----------------------------------------------------------------------------
static void
e3geom_generalpolygon_delete(TQ3Object theObject, void *privateData)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) privateData;
	TQ3Status					qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3GeneralPolygon_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_generalpolygon_duplicate : GeneralPolygon duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  		TQ3Object toObject,   void       *toPrivateData)
{	TQ3GeneralPolygonData		*toInstanceData = (TQ3GeneralPolygonData *) toPrivateData;
	TQ3Status					qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3GeneralPolygon_GetData(fromObject, toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_generalpolygon_cache_new : GeneralPolygon cache new method.
//-----------------------------------------------------------------------------
//		Note :	This implementation doesn't currently support holes or concave
//				polygons - it's really just the same code as for the simple
//				polygon.
//
//				There is code within glut (also under the LGPL) that could be
//				used to extend this.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_generalpolygon_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3GeneralPolygonData *geomData)
{	TQ3Uns32					n, numVertices, numEdges, numTriangles;
	TQ3TriMeshAttributeData		vertexAttributes[kQ3AttributeTypeNumTypes];
	TQ3TriMeshAttributeData		edgeAttributes[1];
	TQ3TriMeshTriangleData		*theTriangles;
	TQ3TriMeshData				triMeshData;
	TQ3GeometryObject			theTriMesh;
	TQ3Point3D					*thePoints;
	TQ3TriMeshEdgeData			*theEdges;
#pragma unused(theView)



	// Work out how many points, edges, and triangles we need
	for (numVertices = 0, n = 0; n < geomData->numContours; n++)
		numVertices += geomData->contours[n].numVertices;

	Q3_ASSERT(numVertices >= 3);
	numEdges     = numVertices;
	numTriangles = numVertices - 2;



	// Allocate the memory we need for the TriMesh data
	thePoints    = (TQ3Point3D *)             Q3Memory_Allocate(numVertices * sizeof(TQ3Point3D));
	theTriangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate(numTriangles * sizeof(TQ3TriMeshTriangleData));
	theEdges     = (TQ3TriMeshEdgeData *)     Q3Memory_Allocate(numEdges     * sizeof(TQ3TriMeshEdgeData));

	if (thePoints == NULL || theTriangles == NULL || theEdges == NULL)
		{
		Q3Memory_Free(&thePoints);
		Q3Memory_Free(&theTriangles);
		Q3Memory_Free(&theEdges);
		
		return(NULL);
		}



	// Initialise the points, edges, and triangles
	for (n = 0; n < numVertices; n++)
		thePoints[n] = geomData->contours[0].vertices[n].point;

	for (n = 0; n < numEdges; n++)
		{
		theEdges[n].pointIndices[0]    = n;
		theEdges[n].pointIndices[1]    = (n == (numEdges-1)) ? 0 : (n + 1);
		theEdges[n].triangleIndices[0] = kQ3ArrayIndexNULL;
		theEdges[n].triangleIndices[1] = kQ3ArrayIndexNULL;
		}
		
	for (n = 0; n < numTriangles; n++)
		{
		theTriangles[n].pointIndices[0] = 0;
		theTriangles[n].pointIndices[1] = n + 1;
		theTriangles[n].pointIndices[2] = n + 2;
		}



	// Initialise the TriMesh data
	triMeshData.numPoints                 = numVertices;
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
	triMeshData.triMeshAttributeSet       = geomData->generalPolygonAttributeSet;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox, triMeshData.points, triMeshData.numPoints, sizeof(TQ3Point3D));



	// Set up the vertex attributes
	n = 0;
	
	if (e3geom_generalpolygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeNormal))
		n++;
		
	if (e3geom_generalpolygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSurfaceUV))
		n++;
	else if (e3geom_generalpolygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeShadingUV))
		n++;

	if (e3geom_generalpolygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeAmbientCoefficient))
		n++;

	if (e3geom_generalpolygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeDiffuseColor))
		{
		// Set up some edge colours as well, just reusing the vertex colours
		Q3_ASSERT(numEdges == numVertices);
		edgeAttributes[0] = vertexAttributes[n];
		
		triMeshData.numEdgeAttributeTypes   = 1;
		triMeshData.edgeAttributeTypes      = edgeAttributes;
		n++;
		}
		
	if (e3geom_generalpolygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSpecularColor))
		n++;
		
	if (e3geom_generalpolygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSpecularControl))
		n++;
		
	if (e3geom_generalpolygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeTransparencyColor))
		n++;
		
	if (e3geom_generalpolygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeHighlightState))
		n++;
		
	if (e3geom_generalpolygon_add_attribute(geomData, &vertexAttributes[n], kQ3AttributeTypeSurfaceShader))
		n++;

	Q3_ASSERT(n < (sizeof(vertexAttributes) / sizeof(TQ3TriMeshAttributeData)));
	if (n != 0)
		{
		triMeshData.numVertexAttributeTypes = n;
		triMeshData.vertexAttributeTypes    = vertexAttributes;
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
//      e3geom_generalpolygon_pick : GeneralPolygon picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_generalpolygon_bounds : GeneralPolygon bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3GeneralPolygonData			*instanceData = (const TQ3GeneralPolygonData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds (we only need to submit the first contour, as this
	// forms the outline of the polygon - subsequent contours form holes
	// cut out of that outline)
	E3View_UpdateBounds(theView,
						instanceData->contours[0].numVertices,
						sizeof(TQ3Vertex3D),
						&instanceData->contours[0].vertices[0].point);

	return(kQ3Success);
}





//=============================================================================
//      e3geom_generalpolygon_get_attribute : GeneralPolygon get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_generalpolygon_get_attribute(TQ3GeometryObject theObject)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->generalPolygonAttributeSet);
}





//=============================================================================
//      e3geom_generalpolygon_metahandler : GeneralPolygon metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_generalpolygon_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_get_attribute;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryGeneralPolygon_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryGeneralPolygon_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeGeneralPolygon,
											kQ3ClassNameGeometryGeneralPolygon,
											e3geom_generalpolygon_metahandler,
											sizeof(TQ3GeneralPolygonData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryGeneralPolygon_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryGeneralPolygon_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeGeneralPolygon, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3GeneralPolygon_New : Create a general polygon object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3GeneralPolygon_New(const TQ3GeneralPolygonData *generalPolygonData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeGeneralPolygon, kQ3False, generalPolygonData);
	return(theObject);
}





//=============================================================================
//      E3GeneralPolygon_Submit : Submit a general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_Submit(const TQ3GeneralPolygonData *generalPolygonData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeGeneralPolygon, generalPolygonData);
	return(qd3dStatus);
}





//=============================================================================
//      E3GeneralPolygon_SetData : Set the data fora general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetData(TQ3GeometryObject generalPolygon, const TQ3GeneralPolygonData *generalPolygonData)
{	TQ3GeneralPolygonData			*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;
	TQ3GeneralPolygonContourData	*newContours;
	TQ3Status						qd3dStatus;
	TQ3Uns32						n, m;



	// Allocate some space for the new contours
	newContours = (TQ3GeneralPolygonContourData *) Q3Memory_Allocate(generalPolygonData->numContours * sizeof(TQ3GeneralPolygonContourData));
	if (newContours == NULL)
		return(kQ3Failure);



	// Allocate some space for the new vertices
	qd3dStatus = kQ3Success;
	for (n = 0; n < generalPolygonData->numContours && qd3dStatus == kQ3Success; n++)
		{
		newContours[n].vertices = (TQ3Vertex3D *) Q3Memory_Allocate(generalPolygonData->contours[n].numVertices * sizeof(TQ3Vertex3D));
		if (newContours[n].vertices == NULL)
			qd3dStatus = kQ3Failure;
		}



	// Handle failure
	if (qd3dStatus != kQ3Success)
		{
		for (n = 0; n < generalPolygonData->numContours; n++)
			Q3Memory_Free(&newContours[n].vertices);

		Q3Memory_Free(&newContours);
		
		return(qd3dStatus);
		}



	// Dispose of the existing data
	for (n = 0; n < instanceData->numContours; n++)
		{
		for (m = 0; m < instanceData->contours[n].numVertices; m++)
			E3Object_DisposeAndForget(instanceData->contours[n].vertices[m].attributeSet);

		Q3Memory_Free(&instanceData->contours[n].vertices);
		}

	Q3Memory_Free(&instanceData->contours);



	// Copy the new data
	instanceData->numContours = generalPolygonData->numContours;
	instanceData->contours    = newContours;
	instanceData->shapeHint   = generalPolygonData->shapeHint;

	E3Shared_Replace(&instanceData->generalPolygonAttributeSet, generalPolygonData->generalPolygonAttributeSet);

	for (n = 0; n < instanceData->numContours; n++)
		{
		instanceData->contours[n].numVertices = generalPolygonData->contours[n].numVertices;

		for (m = 0; m < instanceData->contours[n].numVertices; m++)
			{
			instanceData->contours[n].vertices[m].point = generalPolygonData->contours[n].vertices[m].point;
			E3Shared_Acquire(     &instanceData->contours[n].vertices[m].attributeSet,
			                 generalPolygonData->contours[n].vertices[m].attributeSet);
			}
		}

	Q3Shared_Edited(generalPolygon);

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_GetData : Get the data for a general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetData(TQ3GeometryObject generalPolygon, TQ3GeneralPolygonData *generalPolygonData)
{	TQ3GeneralPolygonData			*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;
	TQ3GeneralPolygonContourData	*newContours;
	TQ3Status						qd3dStatus;
	TQ3Uns32						n, m;



	// Allocate some space for the new contours
	newContours = (TQ3GeneralPolygonContourData *) Q3Memory_Allocate(instanceData->numContours * sizeof(TQ3GeneralPolygonContourData));
	if (newContours == NULL)
		return(kQ3Failure);



	// Allocate some space for the new vertices
	qd3dStatus = kQ3Success;
	for (n = 0; n < instanceData->numContours && qd3dStatus == kQ3Success; n++)
		{
		newContours[n].vertices = (TQ3Vertex3D *) Q3Memory_Allocate(instanceData->contours[n].numVertices * sizeof(TQ3Vertex3D));
		if (newContours[n].vertices == NULL)
			qd3dStatus = kQ3Failure;
		}



	// Handle failure
	if (qd3dStatus != kQ3Success)
		{
		for (n = 0; n < instanceData->numContours; n++)
			Q3Memory_Free(&newContours[n].vertices);

		Q3Memory_Free(&newContours);
		
		return(qd3dStatus);
		}



	// Return the new data
	generalPolygonData->numContours = instanceData->numContours;
	generalPolygonData->contours    = newContours;
	generalPolygonData->shapeHint   = instanceData->shapeHint;

	E3Shared_Replace(&generalPolygonData->generalPolygonAttributeSet, instanceData->generalPolygonAttributeSet);

	for (n = 0; n < generalPolygonData->numContours; n++)
		{
		generalPolygonData->contours[n].numVertices = instanceData->contours[n].numVertices;

		for (m = 0; m < generalPolygonData->contours[n].numVertices; m++)
			{
			generalPolygonData->contours[n].vertices[m].point = instanceData->contours[n].vertices[m].point;
			E3Shared_Acquire(&generalPolygonData->contours[n].vertices[m].attributeSet,
									instanceData->contours[n].vertices[m].attributeSet);
			}
		}

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_EmptyData : Release the data for a general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_EmptyData(TQ3GeneralPolygonData *generalPolygonData)
{	TQ3Uns32		n, m;



	// Release the data
	for (n = 0; n < generalPolygonData->numContours; n++)
		{
		for (m = 0; m < generalPolygonData->contours[n].numVertices; m++)
			E3Object_DisposeAndForget(generalPolygonData->contours[n].vertices[m].attributeSet);

		Q3Memory_Free(&generalPolygonData->contours[n].vertices);
		}

	Q3Memory_Free(&generalPolygonData->contours);
	E3Object_DisposeAndForget(generalPolygonData->generalPolygonAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_GetVertexPosition : Get the position of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetVertexPosition(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3Point3D *position)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Get the vertex position
	*position = instanceData->contours[contourIndex].vertices[pointIndex].point;

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_SetVertexPosition : Set the position of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetVertexPosition(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, const TQ3Point3D *position)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Set the vertex position
	instanceData->contours[contourIndex].vertices[pointIndex].point = *position;

	Q3Shared_Edited(generalPolygon);

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_GetVertexAttributeSet : Get vertex attributes.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetVertexAttributeSet(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3AttributeSet *attributeSet)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Get the vertex attribute set
	E3Shared_Acquire(attributeSet, instanceData->contours[contourIndex].vertices[pointIndex].attributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_SetVertexAttributeSet : Set vertex attributes.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetVertexAttributeSet(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3AttributeSet attributeSet)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Set the vertex attribute set
	E3Shared_Replace(&instanceData->contours[contourIndex].vertices[pointIndex].attributeSet, attributeSet);

	Q3Shared_Edited(generalPolygon);

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_GetShapeHint : Get the polygon shape hint.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetShapeHint(TQ3GeometryObject generalPolygon, TQ3GeneralPolygonShapeHint *shapeHint)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Get the shape hint
	*shapeHint = instanceData->shapeHint;
	
	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_SetShapeHint : Set the polygon shape hint.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetShapeHint(TQ3GeometryObject generalPolygon, TQ3GeneralPolygonShapeHint shapeHint)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Set the shape hint
	instanceData->shapeHint = shapeHint;
	
	Q3Shared_Edited(generalPolygon);

	return(kQ3Success);
}



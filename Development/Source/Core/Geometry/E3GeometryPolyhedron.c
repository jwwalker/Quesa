/*  NAME:
        E3GeometryPolyhedron.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3Set.h"
#include "E3Geometry.h"
#include "E3GeometryTriMesh.h"
#include "E3GeometryPolyhedron.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Polyhedron : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypePolyhedron, E3Polyhedron, E3Geometry )
public :

	TQ3PolyhedronData	instanceData ;

	} ;
	


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
//      e3geom_polyhedron_gather_vertex_attribute : Gather vertex attributes.
//-----------------------------------------------------------------------------
static TQ3AttributeSet
e3geom_polyhedron_gather_vertex_attribute(const void *userData, TQ3Uns32 setIndex)
{	const TQ3PolyhedronData		*geomData = (const TQ3PolyhedronData *) userData;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(setIndex < geomData->numVertices, NULL);



	// Return the appropriate attribute set
	return(geomData->vertices[setIndex].attributeSet);
}





//=============================================================================
//      e3geom_polyhedron_gather_triangle_attribute : Gather tri attributes.
//-----------------------------------------------------------------------------
static TQ3AttributeSet
e3geom_polyhedron_gather_triangle_attribute(const void *userData, TQ3Uns32 setIndex)
{	const TQ3PolyhedronData		*geomData = (const TQ3PolyhedronData *) userData;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(setIndex < geomData->numTriangles, NULL);



	// Return the appropriate attribute set
	return(geomData->triangles[setIndex].triangleAttributeSet);
}





//=============================================================================
//      e3geom_polyhedron_gather_edge_attribute : Gather edge attributes.
//-----------------------------------------------------------------------------
static TQ3AttributeSet
e3geom_polyhedron_gather_edge_attribute(const void *userData, TQ3Uns32 setIndex)
{	const TQ3PolyhedronData		*geomData = (const TQ3PolyhedronData *) userData;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(setIndex < geomData->numEdges, NULL);



	// Return the appropriate attribute set
	return(geomData->edges[setIndex].edgeAttributeSet);
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
#pragma unused(theObject)



	// Dispose of our instance data
	Q3Polyhedron_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_polyhedron_copydata : Copy TQ3PolyhedronData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyhedron_copydata( const TQ3PolyhedronData* src,
	TQ3PolyhedronData* dst, TQ3Boolean isDuplicate )
{
	TQ3Status	q3status = kQ3Success;
	TQ3Uns32	n;
	
	
	
	// Allocate memory
	dst->vertices = (TQ3Vertex3D *) Q3Memory_Allocate( static_cast<TQ3Uns32>(src->numVertices * sizeof(TQ3Vertex3D)) );
	dst->triangles = (TQ3PolyhedronTriangleData *) Q3Memory_Allocate( static_cast<TQ3Uns32>(src->numTriangles *
		sizeof(TQ3PolyhedronTriangleData)) );
	if (src->numEdges > 0)
	{
		dst->edges = (TQ3PolyhedronEdgeData *) Q3Memory_Allocate( static_cast<TQ3Uns32>(src->numEdges *
			sizeof(TQ3PolyhedronEdgeData)) );
	}
	else
	{
		dst->edges = NULL;
	}
	
	
	
	// Check for memory failure
	if ( (dst->vertices == NULL) || (dst->triangles == NULL) ||
		( (dst->edges == NULL) && (src->edges != NULL) ) )
	{
		Q3Memory_Free( &dst->vertices );
		Q3Memory_Free( &dst->triangles );
		Q3Memory_Free( &dst->edges );
		
		return kQ3Failure;
	}
	
	
	
	// Copy non-array fields.
	dst->numVertices = src->numVertices;
	dst->numEdges = src->numEdges;
	dst->numTriangles = src->numTriangles;
	if (src->polyhedronAttributeSet == NULL)
	{
		dst->polyhedronAttributeSet = NULL;
	}
	else if (isDuplicate)
	{
		dst->polyhedronAttributeSet = Q3Object_Duplicate( src->polyhedronAttributeSet );
		if (dst->polyhedronAttributeSet == NULL)
			q3status = kQ3Failure;
	}
	else
	{
		E3Shared_Acquire( &dst->polyhedronAttributeSet, src->polyhedronAttributeSet );
	}
	
	
	
	// Copy vertices
	for (n = 0; n < dst->numVertices; ++n)
	{
		dst->vertices[n].point = src->vertices[n].point;
		if (src->vertices[n].attributeSet == NULL)
		{
			dst->vertices[n].attributeSet = NULL;
		}
		else if (isDuplicate)
		{
			dst->vertices[n].attributeSet = Q3Object_Duplicate( src->vertices[n].attributeSet );
			if (dst->vertices[n].attributeSet == NULL)
				q3status = kQ3Failure;
		}
		else
		{
			E3Shared_Acquire( &dst->vertices[n].attributeSet, src->vertices[n].attributeSet );
		}
	}
	
	
	
	// Copy edges
	if (dst->edges != NULL)
	{
		for (n = 0; n < dst->numEdges; ++n)
		{
			dst->edges[n].vertexIndices[0] = src->edges[n].vertexIndices[0];
			dst->edges[n].vertexIndices[1] = src->edges[n].vertexIndices[1];
			dst->edges[n].triangleIndices[0] = src->edges[n].triangleIndices[0];
			dst->edges[n].triangleIndices[1] = src->edges[n].triangleIndices[1];
			if (src->edges[n].edgeAttributeSet == NULL)
			{
				dst->edges[n].edgeAttributeSet = NULL;
			}
			else if (isDuplicate)
			{
				dst->edges[n].edgeAttributeSet = Q3Object_Duplicate( src->edges[n].edgeAttributeSet );
				if (dst->edges[n].edgeAttributeSet == NULL)
					q3status = kQ3Failure;
			}
			else
			{
				E3Shared_Acquire( &dst->edges[n].edgeAttributeSet, src->edges[n].edgeAttributeSet );
			}
		}
	}
	
	
	
	// Copy triangles
	for (n = 0; n < dst->numTriangles; ++n)
	{
		dst->triangles[n].vertexIndices[0] = src->triangles[n].vertexIndices[0];
		dst->triangles[n].vertexIndices[1] = src->triangles[n].vertexIndices[1];
		dst->triangles[n].vertexIndices[2] = src->triangles[n].vertexIndices[2];
		dst->triangles[n].edgeFlag = src->triangles[n].edgeFlag;
		if (src->triangles[n].triangleAttributeSet == NULL)
		{
			dst->triangles[n].triangleAttributeSet = NULL;
		}
		else if (isDuplicate)
		{
			dst->triangles[n].triangleAttributeSet = Q3Object_Duplicate(
				src->triangles[n].triangleAttributeSet );
			if (dst->triangles[n].triangleAttributeSet == NULL)
				q3status = kQ3Failure;
		}
		else
		{
			E3Shared_Acquire( &dst->triangles[n].triangleAttributeSet,
				src->triangles[n].triangleAttributeSet );
		}
	}
	
	
	
	// Clean up after failure
	if (q3status == kQ3Failure)
	{
		E3Polyhedron_EmptyData( dst );
	}
	
	
	return q3status;
}





//=============================================================================
//      e3geom_polyhedron_duplicate : Polyhedron duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyhedron_duplicate(TQ3Object fromObject, const void *fromPrivateData,
							TQ3Object toObject,   void       *toPrivateData)
{	TQ3PolyhedronData		*toInstanceData = (TQ3PolyhedronData *) toPrivateData;
	TQ3PolyhedronData		*fromInstanceData = (TQ3PolyhedronData *) fromPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = e3geom_polyhedron_copydata(fromInstanceData, toInstanceData, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_polyhedron_cache_new : Polyhedron cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_polyhedron_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3PolyhedronData *geomData)
{	TQ3TriMeshAttributeData		triangleAttributes[kQ3AttributeTypeNumTypes];
	TQ3TriMeshAttributeData		vertexAttributes[kQ3AttributeTypeNumTypes];
	TQ3TriMeshAttributeData		edgeAttributes[kQ3AttributeTypeNumTypes];
	TQ3OrientationStyle			theOrientation;
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
	thePoints    = (TQ3Point3D *)             Q3Memory_Allocate(static_cast<TQ3Uns32>(geomData->numVertices  * sizeof(TQ3Point3D)));
	theTriangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate(static_cast<TQ3Uns32>(geomData->numTriangles * sizeof(TQ3TriMeshTriangleData)));
	theEdges     = NULL;
	
	if (numEdges != 0)
		theEdges = (TQ3TriMeshEdgeData *) Q3Memory_Allocate(static_cast<TQ3Uns32>(numEdges * sizeof(TQ3TriMeshEdgeData)));

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
		Q3Memory_Copy(geomData->triangles[n].vertexIndices, theTriangles[n].pointIndices, sizeof(theTriangles[n].pointIndices));



	// Initialise the edges
	if (numEdges != 0)
		{
		// If they've been supplied, use them as-is
		if (geomData->numEdges != 0)
			{
			for (n = 0; n < geomData->numEdges; n++)
				{
				Q3Memory_Copy(geomData->edges[n].vertexIndices,   theEdges[n].pointIndices,    sizeof(theEdges[n].pointIndices));
				Q3Memory_Copy(geomData->edges[n].triangleIndices, theEdges[n].triangleIndices, sizeof(theEdges[n].triangleIndices));
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



	// Set up the triangle attributes
	n = 0;

	if (E3TriMeshAttribute_GatherArray(geomData->numTriangles, e3geom_polyhedron_gather_triangle_attribute, geomData,
											&triangleAttributes[n], kQ3AttributeTypeNormal))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numTriangles, e3geom_polyhedron_gather_triangle_attribute, geomData,
											&triangleAttributes[n], kQ3AttributeTypeAmbientCoefficient))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numTriangles, e3geom_polyhedron_gather_triangle_attribute, geomData,
											&triangleAttributes[n], kQ3AttributeTypeDiffuseColor))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numTriangles, e3geom_polyhedron_gather_triangle_attribute, geomData,
											&triangleAttributes[n], kQ3AttributeTypeSpecularColor))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numTriangles, e3geom_polyhedron_gather_triangle_attribute, geomData,
											&triangleAttributes[n], kQ3AttributeTypeSpecularControl))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numTriangles, e3geom_polyhedron_gather_triangle_attribute, geomData,
											&triangleAttributes[n], kQ3AttributeTypeTransparencyColor))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numTriangles, e3geom_polyhedron_gather_triangle_attribute, geomData,
											&triangleAttributes[n], kQ3AttributeTypeHighlightState))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numTriangles, e3geom_polyhedron_gather_triangle_attribute, geomData,
											&triangleAttributes[n], kQ3AttributeTypeSurfaceShader))
		n++;

	Q3_ASSERT(n < (sizeof(triangleAttributes) / sizeof(TQ3TriMeshAttributeData)));
	if (n != 0)
		{
		triMeshData.numTriangleAttributeTypes = n;
		triMeshData.triangleAttributeTypes    = triangleAttributes;
		}



	// Set up the vertex attributes
	n = 0;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polyhedron_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeSurfaceUV))
		n++;
	else
	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polyhedron_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeShadingUV))
		n++;
	
	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polyhedron_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeNormal))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polyhedron_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeAmbientCoefficient))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polyhedron_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeDiffuseColor))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polyhedron_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeSpecularColor))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polyhedron_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeSpecularControl))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polyhedron_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeTransparencyColor))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polyhedron_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeSurfaceTangent))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polyhedron_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeHighlightState))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numVertices, e3geom_polyhedron_gather_vertex_attribute, geomData,
											&vertexAttributes[n], kQ3AttributeTypeSurfaceShader))
		n++;

	Q3_ASSERT(n < (sizeof(vertexAttributes) / sizeof(TQ3TriMeshAttributeData)));
	if (n != 0)
		{
		triMeshData.numVertexAttributeTypes = n;
		triMeshData.vertexAttributeTypes    = vertexAttributes;
		}



	// Set up the edge attributes
	n = 0;

	if (E3TriMeshAttribute_GatherArray(geomData->numEdges, e3geom_polyhedron_gather_edge_attribute, geomData,
											&edgeAttributes[n], kQ3AttributeTypeNormal))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numEdges, e3geom_polyhedron_gather_edge_attribute, geomData,
											&edgeAttributes[n], kQ3AttributeTypeAmbientCoefficient))
		n++;

	if (E3TriMeshAttribute_GatherArray(geomData->numEdges, e3geom_polyhedron_gather_edge_attribute, geomData,
											&edgeAttributes[n], kQ3AttributeTypeDiffuseColor))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numEdges, e3geom_polyhedron_gather_edge_attribute, geomData,
											&edgeAttributes[n], kQ3AttributeTypeSpecularColor))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numEdges, e3geom_polyhedron_gather_edge_attribute, geomData,
											&edgeAttributes[n], kQ3AttributeTypeSpecularControl))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numEdges, e3geom_polyhedron_gather_edge_attribute, geomData,
											&edgeAttributes[n], kQ3AttributeTypeTransparencyColor))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numEdges, e3geom_polyhedron_gather_edge_attribute, geomData,
											&edgeAttributes[n], kQ3AttributeTypeHighlightState))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(geomData->numEdges, e3geom_polyhedron_gather_edge_attribute, geomData,
											&edgeAttributes[n], kQ3AttributeTypeSurfaceShader))
		n++;

	Q3_ASSERT(n < (sizeof(edgeAttributes) / sizeof(TQ3TriMeshAttributeData)));
	if (n != 0)
		{
		triMeshData.numEdgeAttributeTypes = n;
		triMeshData.edgeAttributeTypes    = edgeAttributes;
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
	
	for (n = 0; n < triMeshData.numTriangleAttributeTypes; n++)
		{
		Q3Memory_Free(&triMeshData.triangleAttributeTypes[n].data);
		Q3Memory_Free(&triMeshData.triangleAttributeTypes[n].attributeUseArray);
		}
	
	for (n = 0; n < triMeshData.numVertexAttributeTypes; n++)
		{
		Q3Memory_Free(&triMeshData.vertexAttributeTypes[n].data);
		Q3Memory_Free(&triMeshData.vertexAttributeTypes[n].attributeUseArray);
		}
	
	for (n = 0; n < triMeshData.numEdgeAttributeTypes; n++)
		{
		Q3Memory_Free(&triMeshData.edgeAttributeTypes[n].data);
		Q3Memory_Free(&triMeshData.edgeAttributeTypes[n].attributeUseArray);
		}

	return(theTriMesh);
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
e3geom_polyhedron_get_attribute ( E3Polyhedron* polyhedron )
	{
	// Return the address of the geometry attribute set
	return & polyhedron->instanceData.polyhedronAttributeSet ;
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

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_polyhedron_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_polyhedron_get_attribute;
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
//      E3GeometryPolyhedron_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryPolyhedron_RegisterClass(void)
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryPolyhedron,
								e3geom_polyhedron_metahandler,
								E3Polyhedron ) ;
	}





//=============================================================================
//      E3GeometryPolyhedron_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryPolyhedron_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypePolyhedron, kQ3True);
	
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
	theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypePolyhedron, kQ3False, polyhedronData);
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
	{
	E3Polyhedron* polyhedron = (E3Polyhedron*) thePolyhedron ;

	E3Polyhedron_EmptyData ( & polyhedron->instanceData ) ;

	TQ3Status q3status = e3geom_polyhedron_copydata ( polyhedronData,  & polyhedron->instanceData, kQ3False ) ;

	Q3Shared_Edited ( polyhedron ) ;

	return q3status ;
	}





//=============================================================================
//      E3Polyhedron_GetData : Get the data for a polyhedron.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_GetData(TQ3GeometryObject thePolyhedron, TQ3PolyhedronData *polyhedronData)
	{
	E3Polyhedron* polyhedron = (E3Polyhedron*) thePolyhedron ;

	return e3geom_polyhedron_copydata ( & polyhedron->instanceData, polyhedronData, kQ3False ) ;
	}





//=============================================================================
//      E3Polyhedron_EmptyData : Release the data for a polyhedron.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_EmptyData(TQ3PolyhedronData *polyhedronData)
{	TQ3Uns32		n;



	// Release the data
	for (n = 0; n < polyhedronData->numVertices; n++)
		Q3Object_CleanDispose(&polyhedronData->vertices[n].attributeSet);

	for (n = 0; n < polyhedronData->numTriangles; n++)
		Q3Object_CleanDispose(&polyhedronData->triangles[n].triangleAttributeSet);

	for (n = 0; n < polyhedronData->numEdges; n++)
		Q3Object_CleanDispose(&polyhedronData->edges[n].edgeAttributeSet);

	Q3Memory_Free(&polyhedronData->vertices);
	Q3Memory_Free(&polyhedronData->triangles);
	Q3Memory_Free(&polyhedronData->edges);

	Q3Object_CleanDispose(&polyhedronData->polyhedronAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Polyhedron_GetVertexPosition : Get the position of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_GetVertexPosition(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, TQ3Point3D *point)
	{
	E3Polyhedron* polyhedron = (E3Polyhedron*) thePolyhedron ;

	// Get the vertex position
	*point = polyhedron->instanceData.vertices [ index ].point ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Polyhedron_SetVertexPosition : Set the position of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_SetVertexPosition(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, const TQ3Point3D *point)
	{
	E3Polyhedron* polyhedron = (E3Polyhedron*) thePolyhedron ;

	// Set the vertex position
	polyhedron->instanceData.vertices [ index ].point = *point ;

	Q3Shared_Edited ( polyhedron ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Polyhedron_GetVertexAttributeSet : Get the attribute set of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_GetVertexAttributeSet(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
	{
	E3Polyhedron* polyhedron = (E3Polyhedron*) thePolyhedron ;

	// Get the vertex attribute set
	E3Shared_Acquire ( attributeSet, polyhedron->instanceData.vertices [ index ].attributeSet ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Polyhedron_SetVertexAttributeSet : Set the attribute set of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Polyhedron_SetVertexAttributeSet(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, TQ3AttributeSet attributeSet)
	{
	E3Polyhedron* polyhedron = (E3Polyhedron*) thePolyhedron ;

	// Set the vertex attribute set
	E3Shared_Replace ( &polyhedron->instanceData.vertices [ index ].attributeSet, attributeSet ) ;

	Q3Shared_Edited ( polyhedron ) ;

	return kQ3Success ;
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
	{
	E3Polyhedron* polyhedron = (E3Polyhedron*) thePolyhedron ;



	// Get the triangle data
	Q3Memory_Copy ( polyhedron->instanceData.triangles [ triangleIndex ].vertexIndices,
		   triangleData->vertexIndices,
		   sizeof ( triangleData->vertexIndices ) ) ;

	triangleData->edgeFlag = polyhedron->instanceData.triangles [ triangleIndex ].edgeFlag ;

	E3Shared_Acquire ( & triangleData->triangleAttributeSet,
		              polyhedron->instanceData.triangles [ triangleIndex ].triangleAttributeSet ) ;

	return kQ3Success ;
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
	{
	E3Polyhedron* polyhedron = (E3Polyhedron*) thePolyhedron ;



	// Set the triangle data
	Q3Memory_Copy ( triangleData->vertexIndices,
		   polyhedron->instanceData.triangles [ triangleIndex ].vertexIndices,
		   sizeof ( polyhedron->instanceData.triangles [ triangleIndex ].vertexIndices ) ) ;

	polyhedron->instanceData.triangles [ triangleIndex ].edgeFlag = triangleData->edgeFlag ;

	E3Shared_Acquire ( & polyhedron->instanceData.triangles [ triangleIndex ].triangleAttributeSet,
					  triangleData->triangleAttributeSet ) ;

	Q3Shared_Edited ( polyhedron ) ;

	return kQ3Success ;
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
	{
	E3Polyhedron* polyhedron = (E3Polyhedron*) thePolyhedron ;

	// Get the edge data
	Q3Memory_Copy ( polyhedron->instanceData.edges [ edgeIndex ].vertexIndices,
		   edgeData->vertexIndices,
		   sizeof ( edgeData->vertexIndices ) ) ;

	Q3Memory_Copy ( polyhedron->instanceData.edges [ edgeIndex ].triangleIndices,
		   edgeData->triangleIndices,
		   sizeof ( edgeData->triangleIndices ) ) ;

	E3Shared_Acquire ( & edgeData->edgeAttributeSet,
		              polyhedron->instanceData.edges [ edgeIndex ].edgeAttributeSet ) ;

	return kQ3Success ;
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
	{
	E3Polyhedron* polyhedron = (E3Polyhedron*) thePolyhedron ;

	// Set the edge data
	Q3Memory_Copy ( edgeData->vertexIndices,
		   polyhedron->instanceData.edges [ edgeIndex ].vertexIndices,
		   sizeof ( polyhedron->instanceData.edges [ edgeIndex ].vertexIndices ) ) ;

	Q3Memory_Copy ( edgeData->triangleIndices,
		   polyhedron->instanceData.edges [ edgeIndex ].triangleIndices,
		   sizeof ( polyhedron->instanceData.edges [ edgeIndex ].triangleIndices ) ) ;

	E3Shared_Acquire ( & polyhedron->instanceData.edges [ edgeIndex ].edgeAttributeSet,
					  edgeData->edgeAttributeSet ) ;

	Q3Shared_Edited ( polyhedron ) ;

	return kQ3Success ;
	}

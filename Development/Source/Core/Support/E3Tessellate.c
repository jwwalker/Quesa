/*  NAME:
        E3Tessellate.c

    DESCRIPTION:
        Quesa tessellator functions.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "GLPrefix.h"
#include "E3Set.h"
#include "E3Tessellate.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// GLU callback
#ifndef CALLBACK
	#define CALLBACK
#endif

typedef void (CALLBACK *GLcallback)();


// Tessellator state
typedef struct E3TessellateState {
	TQ3TriMeshData		triMeshData;

	TQ3Uns32			numTriMeshVertices;
	TQ3Vertex3D			**triMeshVertexList;

	TQ3Uns32			numTempVertices;
	TQ3Vertex3D			**tempVertexList;

	GLenum				errorState;
	TQ3Boolean			edgeState;

	TQ3Uns32			vertexInTriangle;
	TQ3Boolean			vertexEdgeState[3];
	TQ3Uns32			vertexIndices[3];
	
} E3TessellateState;


// Vertex attribute state
typedef struct E3CombinedAttribute {
	TQ3Uns32			attributeMask;
	TQ3Param2D			surfaceUV;
	TQ3Param2D			shadingUV;
	TQ3Vector3D			normal;
	float				ambientCoefficient;
	TQ3ColorRGB			diffuseColour;
	TQ3ColorRGB			specularColour;
	float				specularControl;
	TQ3ColorRGB			transparencyColour;
	TQ3Tangent2D		surfaceTangent;
} E3CombinedAttribute;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3tessellate_add_vertex : Add or locate a vertex within the list.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3tessellate_add_vertex(E3TessellateState *theState, TQ3Vertex3D *theVertex)
{	TQ3Uns32		vertexIndex, theSize;
	TQ3Status		qd3dStatus;



	// Search the list for this vertex
	for (vertexIndex = 0; vertexIndex < theState->numTriMeshVertices; vertexIndex++)
		{
		if (theState->triMeshVertexList[vertexIndex] == theVertex)
			break;
		}



	// If we found it, return the index
	if (vertexIndex < theState->numTriMeshVertices)
		return(vertexIndex);



	// Grow the vertex array - handle failure by punting to the first vertex
	theSize    = (theState->numTriMeshVertices + 1) * sizeof(TQ3Vertex3D*);
	qd3dStatus = Q3Memory_Reallocate(&theState->triMeshVertexList, theSize);
	if (qd3dStatus != kQ3Success)
		return(0);



	// Save the latest vertex
	vertexIndex                              = theState->numTriMeshVertices;
	theState->triMeshVertexList[vertexIndex] = theVertex;
	theState->numTriMeshVertices++;
	
	return(vertexIndex);
}





//=============================================================================
//      e3tessellate_add_triangle : Add a triangle to the list.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3tessellate_add_triangle(E3TessellateState *theState, TQ3Uns32 vertexIndices[3])
{	TQ3TriMeshTriangleData		*theTriangle;
	TQ3Status					qd3dStatus;
	TQ3Uns32					theSize, n;



	// Grow the triangle array
	theSize    = (theState->triMeshData.numTriangles + 1) * sizeof(TQ3TriMeshTriangleData);
	qd3dStatus = Q3Memory_Reallocate(&theState->triMeshData.triangles, theSize);
	if (qd3dStatus != kQ3Success)
		return(kQ3False);



	// Grab the latest triangle and update the count
	theTriangle = &theState->triMeshData.triangles[theState->triMeshData.numTriangles];
	theState->triMeshData.numTriangles++;



	// Initialise the new triangle
	for (n = 0; n < 3; n++)
		theTriangle->pointIndices[n] = vertexIndices[n];

	return(kQ3True);
}





//=============================================================================
//      e3tessellate_add_edge : Add an edge to the list.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3tessellate_add_edge(E3TessellateState *theState, TQ3Uns32 vertexIndex0, TQ3Uns32 vertexIndex1)
{	TQ3Status				qd3dStatus;
	TQ3TriMeshEdgeData		*theEdge;
	TQ3Uns32				theSize;



	// Grow the edge array
	theSize    = (theState->triMeshData.numEdges + 1) * sizeof(TQ3TriMeshEdgeData);
	qd3dStatus = Q3Memory_Reallocate(&theState->triMeshData.edges, theSize);
	if (qd3dStatus != kQ3Success)
		return(kQ3False);



	// Grab the latest edge and update the count
	theEdge = &theState->triMeshData.edges[theState->triMeshData.numEdges];
	theState->triMeshData.numEdges++;



	// Initialise the new edge
	theEdge->pointIndices[0]    = vertexIndex0;
	theEdge->pointIndices[1]    = vertexIndex1;
	theEdge->triangleIndices[0] = theState->triMeshData.numTriangles - 1;
	theEdge->triangleIndices[1] = kQ3ArrayIndexNULL;
	
	return(kQ3True);
}





//=============================================================================
//      e3tessellate_attribute_get : Get vertex attribute data.
//-----------------------------------------------------------------------------
static void
e3tessellate_attribute_get(const TQ3Vertex3D *theVertex, E3CombinedAttribute *theState, TQ3AttributeTypes theType)
{	TQ3Status		qd3dStatus;
	TQ3Uns32		typeMask;
	void			*theData;



	// Find the data pointer
	switch (theType) {
		case kQ3AttributeTypeSurfaceUV:
			theData = &theState->surfaceUV;
			break;
			
		case kQ3AttributeTypeShadingUV:
			theData = &theState->shadingUV;
			break;
			
		case kQ3AttributeTypeNormal:
			theData = &theState->normal;
			break;
			
		case kQ3AttributeTypeAmbientCoefficient:
			theData = &theState->ambientCoefficient;
			break;
			
		case kQ3AttributeTypeDiffuseColor:
			theData = &theState->diffuseColour;
			break;
			
		case kQ3AttributeTypeSpecularColor:
			theData = &theState->specularColour;
			break;
			
		case kQ3AttributeTypeSpecularControl:
			theData = &theState->specularControl;
			break;
			
		case kQ3AttributeTypeTransparencyColor:
			theData = &theState->transparencyColour;
			break;
			
		case kQ3AttributeTypeSurfaceTangent:
			theData = &theState->surfaceTangent;
			break;
		
		default:
			Q3_ASSERT(!"Can never happen");
			return;
			break;
		}



	// Grab the attribute data
	typeMask   = 1 << (theType - 1);
	qd3dStatus = Q3AttributeSet_Get(theVertex->attributeSet, theType, theData);
	if (qd3dStatus == kQ3Success)
		theState->attributeMask |= typeMask;
}





//=============================================================================
//      e3tessellate_attribute_set : Set vertex attribute data.
//-----------------------------------------------------------------------------
static void
e3tessellate_attribute_set(const TQ3Vertex3D *theVertex, const E3CombinedAttribute *theState, TQ3AttributeTypes theType)
{	TQ3Status		qd3dStatus;
	TQ3Uns32		typeMask;
	const void		*theData;



	// Find the data pointer
	switch (theType) {
		case kQ3AttributeTypeSurfaceUV:
			theData = &theState->surfaceUV;
			break;
			
		case kQ3AttributeTypeShadingUV:
			theData = &theState->shadingUV;
			break;
			
		case kQ3AttributeTypeNormal:
			theData = &theState->normal;
			break;
			
		case kQ3AttributeTypeAmbientCoefficient:
			theData = &theState->ambientCoefficient;
			break;
			
		case kQ3AttributeTypeDiffuseColor:
			theData = &theState->diffuseColour;
			break;
			
		case kQ3AttributeTypeSpecularColor:
			theData = &theState->specularColour;
			break;
			
		case kQ3AttributeTypeSpecularControl:
			theData = &theState->specularControl;
			break;
			
		case kQ3AttributeTypeTransparencyColor:
			theData = &theState->transparencyColour;
			break;
			
		case kQ3AttributeTypeSurfaceTangent:
			theData = &theState->surfaceTangent;
			break;
		
		default:
			Q3_ASSERT(!"Can never happen");
			return;
			break;
		}



	// Add the attribute data if it's present
	typeMask = 1 << (theType - 1);
	if (theState->attributeMask & typeMask)
		qd3dStatus = Q3AttributeSet_Add(theVertex->attributeSet, theType, theData);
}





//=============================================================================
//      e3tessellate_attribute_blend : Incorporate a vertex's attribute data.
//-----------------------------------------------------------------------------
static void
e3tessellate_attribute_blend(E3CombinedAttribute *dstState, const E3CombinedAttribute *srcState, float theWeight)
{


	// Blend the attribute state
	if (srcState->attributeMask & kQ3XAttributeMaskSurfaceUV)
		{
		dstState->attributeMask |= kQ3XAttributeMaskSurfaceUV;
		dstState->surfaceUV.u   += theWeight * srcState->surfaceUV.u;
		dstState->surfaceUV.v   += theWeight * srcState->surfaceUV.v;
		}
		
	if (srcState->attributeMask & kQ3XAttributeMaskShadingUV)
		{
		dstState->attributeMask |= kQ3XAttributeMaskShadingUV;
		dstState->shadingUV.u   += theWeight * srcState->shadingUV.u;
		dstState->shadingUV.v   += theWeight * srcState->shadingUV.v;
		}

	if (srcState->attributeMask & kQ3XAttributeMaskNormal)
		{
		dstState->attributeMask |= kQ3XAttributeMaskNormal;
		dstState->normal.x      += theWeight * srcState->normal.x;
		dstState->normal.y      += theWeight * srcState->normal.y;
		dstState->normal.z      += theWeight * srcState->normal.z;
		}
	
	if (srcState->attributeMask & kQ3XAttributeMaskAmbientCoefficient)
		{
		dstState->attributeMask      |= kQ3XAttributeMaskAmbientCoefficient;
		dstState->ambientCoefficient += theWeight * srcState->ambientCoefficient;
		}

	if (srcState->attributeMask & kQ3XAttributeMaskDiffuseColor)
		{
		dstState->attributeMask   |= kQ3XAttributeMaskDiffuseColor;
		dstState->diffuseColour.r += theWeight * srcState->diffuseColour.r;
		dstState->diffuseColour.g += theWeight * srcState->diffuseColour.g;
		dstState->diffuseColour.b += theWeight * srcState->diffuseColour.b;
		}

	if (srcState->attributeMask & kQ3XAttributeMaskSpecularColor)
		{
		dstState->attributeMask    |= kQ3XAttributeMaskSpecularColor;
		dstState->specularColour.r += theWeight * srcState->specularColour.r;
		dstState->specularColour.g += theWeight * srcState->specularColour.g;
		dstState->specularColour.b += theWeight * srcState->specularColour.b;
		}

	if (srcState->attributeMask & kQ3XAttributeMaskSpecularControl)
		{
		dstState->attributeMask   |= kQ3XAttributeMaskSpecularControl;
		dstState->specularControl += theWeight * srcState->specularControl;
		}

	if (srcState->attributeMask & kQ3XAttributeMaskTransparencyColor)
		{
		dstState->attributeMask        |= kQ3XAttributeMaskTransparencyColor;
		dstState->transparencyColour.r += theWeight * srcState->transparencyColour.r;
		dstState->transparencyColour.g += theWeight * srcState->transparencyColour.g;
		dstState->transparencyColour.b += theWeight * srcState->transparencyColour.b;
		}

	if (srcState->attributeMask & kQ3XAttributeMaskSurfaceTangent)
		{
		dstState->attributeMask |= kQ3XAttributeMaskSurfaceTangent;

		dstState->surfaceTangent.uTangent.x += theWeight * srcState->surfaceTangent.uTangent.x;
		dstState->surfaceTangent.uTangent.y += theWeight * srcState->surfaceTangent.uTangent.y;
		dstState->surfaceTangent.uTangent.z += theWeight * srcState->surfaceTangent.uTangent.z;

		dstState->surfaceTangent.vTangent.x += theWeight * srcState->surfaceTangent.vTangent.x;
		dstState->surfaceTangent.vTangent.y += theWeight * srcState->surfaceTangent.vTangent.y;
		dstState->surfaceTangent.vTangent.z += theWeight * srcState->surfaceTangent.vTangent.z;
		}
}





//=============================================================================
//      e3tessellate_gather_vertex_attribute : Gather vertex attributes.
//-----------------------------------------------------------------------------
static TQ3AttributeSet
e3tessellate_gather_vertex_attribute(const void *userData, TQ3Uns32 setIndex)
{	const E3TessellateState		*theState = (const E3TessellateState *) userData;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(setIndex < theState->numTriMeshVertices, NULL);



	// Return the appropriate attribute set
	return(theState->triMeshVertexList[setIndex]->attributeSet);
}





//=============================================================================
//      e3tessellate_callback_begin : Begin a new triangle.
//-----------------------------------------------------------------------------
static void CALLBACK
e3tessellate_callback_begin(GLenum which, void *userData)
{	E3TessellateState		*theState = (E3TessellateState *) userData;



	// Validate our state - since we asked for edge data, we should only see triangles
	Q3_ASSERT(which == GL_TRIANGLES);



	// Start a new triangle
	theState->vertexInTriangle = 0;

	Q3Memory_Clear(theState->vertexEdgeState, sizeof(theState->vertexEdgeState));
	Q3Memory_Clear(theState->vertexIndices,   sizeof(theState->vertexIndices));
}





//=============================================================================
//      e3tessellate_callback_end : End the current triangle.
//-----------------------------------------------------------------------------
static void CALLBACK
e3tessellate_callback_end(void *userData)
{	E3TessellateState	*theState = (E3TessellateState *) userData;
	TQ3Boolean			wasAdded;



	// Add the triangle, and then any edges
	wasAdded = e3tessellate_add_triangle(theState, theState->vertexIndices);
	if (wasAdded)
		{
		// Add an edge between vertex 0 and 1 if required
		if (theState->vertexEdgeState[0])
			e3tessellate_add_edge(theState, theState->vertexIndices[0], theState->vertexIndices[1]);


		// Add an edge between vertex 1 and 2 if required
		if (theState->vertexEdgeState[1])
			e3tessellate_add_edge(theState, theState->vertexIndices[1], theState->vertexIndices[2]);


		// Add an edge between vertex 2 and 0 if required
		if (theState->vertexEdgeState[2])
			e3tessellate_add_edge(theState, theState->vertexIndices[2], theState->vertexIndices[0]);
		}
}





//=============================================================================
//      e3tessellate_callback_edge : Update the edge state.
//-----------------------------------------------------------------------------
static void CALLBACK
e3tessellate_callback_edge(GLboolean isEdge, void *userData)
{	E3TessellateState		*theState = (E3TessellateState *) userData;



	// Update the current edge state
	theState->edgeState = (TQ3Boolean) isEdge;
}





//=============================================================================
//      e3tessellate_callback_vertex : Process another vertex.
//-----------------------------------------------------------------------------
static void CALLBACK
e3tessellate_callback_vertex(GLvoid *vertex, void *userData)
{	E3TessellateState		*theState  = (E3TessellateState *) userData;
	TQ3Vertex3D				*theVertex = (TQ3Vertex3D       *) vertex;
	TQ3Uns32				vertexIndex;



	// If we're not getting individual begin/end callbacks for each triangle,
	// our index will eventually overflow - once this happens, we need to
	// close this triangle and start the next one ourselves.
	if (theState->vertexInTriangle == 3)
		{
		e3tessellate_callback_end(userData);
		e3tessellate_callback_begin(GL_TRIANGLES, userData);
		}



	// Add the vertex to the list
	vertexIndex = e3tessellate_add_vertex(theState, theVertex);



	// Update the triangle state
	theState->vertexEdgeState[theState->vertexInTriangle] = theState->edgeState;
	theState->vertexIndices[  theState->vertexInTriangle] = vertexIndex;

	theState->vertexInTriangle++;
}





//=============================================================================
//      e3tessellate_callback_error : Store an error code in the tessellator state.
//-----------------------------------------------------------------------------
static void CALLBACK
e3tessellate_callback_error(GLenum errorCode, void *userData)
{	E3TessellateState		*theState = (E3TessellateState *) userData;



	// Set the error state
	theState->errorState = errorCode;	
}





//=============================================================================
//      e3tessellate_callback_combine : Combine vertices to form a new vertex.
//-----------------------------------------------------------------------------
static void CALLBACK
e3tessellate_callback_combine(const GLdouble		pointIn[3],
								const void			*dataIn[4],
								const GLfloat		theWeights[4],
								void				**dataOut,
								void				*userData)
{	E3TessellateState		*theState = (E3TessellateState *) userData;
	E3CombinedAttribute		combinedState, vertexState[4];
	TQ3Point3D				combinedPoint;
	const TQ3Vertex3D		*theVertex;
	TQ3Vertex3D				*newVertex;
	TQ3Uns32				n, theSize;
	TQ3Status				qd3dStatus;



	// Initialise the result
	*dataOut = NULL;



	// Collect the attribute state that can be interpolated
	//
	// Hilights, surface shaders, and custom attributes can't be interpolated so we just
	// have to skip them - custom attributes could be extended to handle interpolation?
	Q3Memory_Clear(vertexState, sizeof(vertexState));

	for (n = 0; n < 4; n++)
		{
		theVertex = (const TQ3Vertex3D *) dataIn[n];
		if (theVertex != NULL && theVertex->attributeSet != NULL)
			{
			e3tessellate_attribute_get(theVertex, &vertexState[n], kQ3AttributeTypeSurfaceUV);
			e3tessellate_attribute_get(theVertex, &vertexState[n], kQ3AttributeTypeShadingUV);
			e3tessellate_attribute_get(theVertex, &vertexState[n], kQ3AttributeTypeNormal);
			e3tessellate_attribute_get(theVertex, &vertexState[n], kQ3AttributeTypeAmbientCoefficient);
			e3tessellate_attribute_get(theVertex, &vertexState[n], kQ3AttributeTypeDiffuseColor);
			e3tessellate_attribute_get(theVertex, &vertexState[n], kQ3AttributeTypeSpecularColor);
			e3tessellate_attribute_get(theVertex, &vertexState[n], kQ3AttributeTypeSpecularControl);
			e3tessellate_attribute_get(theVertex, &vertexState[n], kQ3AttributeTypeTransparencyColor);
			e3tessellate_attribute_get(theVertex, &vertexState[n], kQ3AttributeTypeSurfaceTangent);
			}
		}



	// Blend the four vertices together to form the combined vertex
	//
	// We blend by using the supplied weights, however we don't redistribute weights if
	// an attribute isn't present. Since we blend by addition, we reply on vertexState
	// being cleared to 0 above so that attributes which aren't present will have no
	// effect on the output.
	//
	// We might get better results if we redistribute the weights based on the presence of
	// the corresponding attribute (e.g., if only 3 vertices have a colour, should the
	// weight for the 4th vertex be redistributed amongst the other 3?). To be decided.
	Q3Memory_Clear(&combinedState, sizeof(combinedState));

	combinedPoint.x = (float) pointIn[0];
	combinedPoint.y = (float) pointIn[1];
	combinedPoint.z = (float) pointIn[2];

	for (n = 0; n < 4; n++)
		e3tessellate_attribute_blend(&combinedState, &vertexState[n], theWeights[n]);



	// Allocate another vertex
	newVertex = (TQ3Vertex3D *) Q3Memory_Allocate(sizeof(TQ3Vertex3D));
	if (newVertex == NULL)
		return;



	// Grow the temporary vertex array, and save the new vertex
	theSize    = (theState->numTempVertices + 1) * sizeof(TQ3Vertex3D*);
	qd3dStatus = Q3Memory_Reallocate(&theState->tempVertexList, theSize);
	if (qd3dStatus != kQ3Success)
		{
		Q3Memory_Free(&newVertex);
		return;
		}

	theState->tempVertexList[theState->numTempVertices] = newVertex;
	theState->numTempVertices++;



	// Set up the vertex, and return it
	newVertex->point        = combinedPoint;
	newVertex->attributeSet = (combinedState.attributeMask == 0 ? NULL : Q3AttributeSet_New());

	if (newVertex->attributeSet != NULL)
		{
		e3tessellate_attribute_set(newVertex, &combinedState, kQ3AttributeTypeSurfaceUV);
		e3tessellate_attribute_set(newVertex, &combinedState, kQ3AttributeTypeShadingUV);
		e3tessellate_attribute_set(newVertex, &combinedState, kQ3AttributeTypeNormal);
		e3tessellate_attribute_set(newVertex, &combinedState, kQ3AttributeTypeAmbientCoefficient);
		e3tessellate_attribute_set(newVertex, &combinedState, kQ3AttributeTypeDiffuseColor);
		e3tessellate_attribute_set(newVertex, &combinedState, kQ3AttributeTypeSpecularColor);
		e3tessellate_attribute_set(newVertex, &combinedState, kQ3AttributeTypeSpecularControl);
		e3tessellate_attribute_set(newVertex, &combinedState, kQ3AttributeTypeTransparencyColor);
		e3tessellate_attribute_set(newVertex, &combinedState, kQ3AttributeTypeSurfaceTangent);
		}

	*dataOut = newVertex;
}





//=============================================================================
//      e3tessellate_create_trimesh : Create the TriMesh from the tessellator.
//-----------------------------------------------------------------------------
static TQ3GeometryObject
e3tessellate_create_trimesh(E3TessellateState *theState, TQ3AttributeSet triMeshAttributes)
{	TQ3TriMeshAttributeData		*theAttributes;
	TQ3Uns32					n, numPoints;
	TQ3Point3D					*thePoints;
	TQ3GeometryObject			theTriMesh;



	// Allocate the vertex points and attributes arrays
	thePoints     = (TQ3Point3D              *) Q3Memory_Allocate(theState->numTriMeshVertices * sizeof(TQ3Point3D));
	
	if (thePoints == NULL)
		return NULL;
	
	theAttributes = (TQ3TriMeshAttributeData *) Q3Memory_Allocate(kQ3AttributeTypeNumTypes     * sizeof(TQ3TriMeshAttributeData));
	
	if (theAttributes == NULL)
		{
		Q3Memory_Free( &thePoints );
		return(NULL);
		}



	// Set up the points
	numPoints                       = theState->numTriMeshVertices;
	theState->triMeshData.numPoints = numPoints;
	theState->triMeshData.points    = thePoints;

	for (n = 0; n < numPoints; n++)
		theState->triMeshData.points[n] = theState->triMeshVertexList[n]->point;

	Q3BoundingBox_SetFromPoints3D(&theState->triMeshData.bBox, thePoints, numPoints, sizeof(TQ3Point3D));



	// Set up the attributes
	n = 0;
	
	if (E3TriMeshAttribute_GatherArray(theState->numTriMeshVertices, e3tessellate_gather_vertex_attribute, theState,
											&theAttributes[n], kQ3AttributeTypeSurfaceUV))
		n++;
	else
	if (E3TriMeshAttribute_GatherArray(theState->numTriMeshVertices, e3tessellate_gather_vertex_attribute, theState,
											&theAttributes[n], kQ3AttributeTypeShadingUV))
		n++;

	if (E3TriMeshAttribute_GatherArray(theState->numTriMeshVertices, e3tessellate_gather_vertex_attribute, theState,
											&theAttributes[n], kQ3AttributeTypeNormal))
		n++;

	if (E3TriMeshAttribute_GatherArray(theState->numTriMeshVertices, e3tessellate_gather_vertex_attribute, theState,
											&theAttributes[n], kQ3AttributeTypeAmbientCoefficient))
		n++;

	if (E3TriMeshAttribute_GatherArray(theState->numTriMeshVertices, e3tessellate_gather_vertex_attribute, theState,
											&theAttributes[n], kQ3AttributeTypeDiffuseColor))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(theState->numTriMeshVertices, e3tessellate_gather_vertex_attribute, theState,
											&theAttributes[n], kQ3AttributeTypeSpecularColor))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(theState->numTriMeshVertices, e3tessellate_gather_vertex_attribute, theState,
											&theAttributes[n], kQ3AttributeTypeSpecularControl))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(theState->numTriMeshVertices, e3tessellate_gather_vertex_attribute, theState,
											&theAttributes[n], kQ3AttributeTypeTransparencyColor))
		n++;
		
	if (E3TriMeshAttribute_GatherArray(theState->numTriMeshVertices, e3tessellate_gather_vertex_attribute, theState,
											&theAttributes[n], kQ3AttributeTypeSurfaceTangent))
		n++;
	
	Q3_ASSERT(n < kQ3AttributeTypeNumTypes);
	theState->triMeshData.numVertexAttributeTypes = n;
	theState->triMeshData.vertexAttributeTypes    = theAttributes;
	theState->triMeshData.triMeshAttributeSet     = triMeshAttributes;



	// Create the TriMesh
	theTriMesh = Q3TriMesh_New(&theState->triMeshData);

	return(theTriMesh);
}





//=============================================================================
//      e3tessellate_dispose_state : Dispose of the tessellator state.
//-----------------------------------------------------------------------------
static void
e3tessellate_dispose_state(E3TessellateState *theState)
{	TQ3Uns32	n;



	// Dispose of the TriMesh attributes
	for (n = 0; n < theState->triMeshData.numTriangleAttributeTypes; n++)
		{
		Q3Memory_Free(&theState->triMeshData.triangleAttributeTypes[n].data);
		Q3Memory_Free(&theState->triMeshData.triangleAttributeTypes[n].attributeUseArray);
		}
	
	for (n = 0; n < theState->triMeshData.numEdgeAttributeTypes; n++)
		{
		Q3Memory_Free(&theState->triMeshData.edgeAttributeTypes[n].data);
		Q3Memory_Free(&theState->triMeshData.edgeAttributeTypes[n].attributeUseArray);
		}

	for (n = 0; n < theState->triMeshData.numVertexAttributeTypes; n++)
		{
		Q3Memory_Free(&theState->triMeshData.vertexAttributeTypes[n].data);
		Q3Memory_Free(&theState->triMeshData.vertexAttributeTypes[n].attributeUseArray);
		}



	// Dispose of the TriMesh data
	Q3Memory_Free(&theState->triMeshData.triangles);
	Q3Memory_Free(&theState->triMeshData.triangleAttributeTypes);
	Q3Memory_Free(&theState->triMeshData.edges);
	Q3Memory_Free(&theState->triMeshData.edgeAttributeTypes);
	Q3Memory_Free(&theState->triMeshData.points);
	Q3Memory_Free(&theState->triMeshData.vertexAttributeTypes);



	// Dispose of the tessellator state
	for (n = 0; n < theState->numTempVertices; n++)
		{
		if (theState->tempVertexList[n]->attributeSet != NULL)
			Q3Object_Dispose(theState->tempVertexList[n]->attributeSet);
		
		Q3Memory_Free(&theState->tempVertexList[n]);
		}
	
	Q3Memory_Free(&theState->triMeshVertexList);
	Q3Memory_Free(&theState->tempVertexList);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Tessellate_Contours : Tessellate a list of contours into one TriMesh.
//-----------------------------------------------------------------------------
//		Note :	Contours form a planar polygon, with each contour defined by
//				one or more lists of vertices. Contours may be concave or
//				convex, and may be nested and overlapping.
//
//				Overlapping contours form holes, with the even-odd rule used to
//				determine which portion of the polygon is to be removed.
//-----------------------------------------------------------------------------
TQ3Object
E3Tessellate_Contours(TQ3Uns32 numContours, const TQ3Contour *theContours, TQ3AttributeSet theAttributes)
{	GLdouble				vertCoords[3];
	TQ3Vertex3D				*theVertex;
	TQ3GeometryObject		theTriMesh;
	GLUtriangulatorObj		*theTess;
	E3TessellateState		theState;
	TQ3Uns32				n, m;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(numContours >= 1,          NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theContours), NULL);



	// Create the tessellator
	theTess = gluNewTess();
	if (theTess == NULL)
		{
		E3ErrorManager_PostError(kQ3ErrorOutOfMemory, kQ3False);
		return(NULL);
		}

	// Set it up
	gluTessProperty(theTess, GLU_TESS_WINDING_RULE,   GLU_TESS_WINDING_ODD);
	gluTessCallback(theTess, GLU_TESS_BEGIN_DATA,     (GLcallback) e3tessellate_callback_begin);
	gluTessCallback(theTess, GLU_TESS_END_DATA,       (GLcallback) e3tessellate_callback_end);
	gluTessCallback(theTess, GLU_TESS_EDGE_FLAG_DATA, (GLcallback) e3tessellate_callback_edge);
	gluTessCallback(theTess, GLU_TESS_VERTEX_DATA,    (GLcallback) e3tessellate_callback_vertex);
	gluTessCallback(theTess, GLU_TESS_ERROR_DATA,     (GLcallback) e3tessellate_callback_error);
	gluTessCallback(theTess, GLU_TESS_COMBINE_DATA,   (GLcallback) e3tessellate_callback_combine);



	// Set up our state
	Q3Memory_Clear(&theState, sizeof(theState));
	theTriMesh = NULL;



	// Feed the contours into the tessellator
	gluTessBeginPolygon(theTess, &theState);
	for (n = 0; n < numContours; n++)
		{
         gluTessBeginContour(theTess);
         for (m = 0; m < theContours[n].numVertices; m++)
         	{
         	theVertex     = &theContours[n].theVertices[m];
         	vertCoords[0] = (GLdouble) theVertex->point.x;
         	vertCoords[1] = (GLdouble) theVertex->point.y;
         	vertCoords[2] = (GLdouble) theVertex->point.z;

			gluTessVertex(theTess, vertCoords, theVertex);
			}
		gluTessEndContour(theTess);
		}
	gluTessEndPolygon(theTess);



	// Create the TriMesh if all went well
	if ( (theState.errorState == GL_NO_ERROR) && (theState.numTriMeshVertices > 0) )
		theTriMesh = e3tessellate_create_trimesh(&theState, theAttributes);



	// Clean up
	e3tessellate_dispose_state(&theState);
	gluDeleteTess(theTess);
	
	return(theTriMesh);
}

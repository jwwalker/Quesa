/*  NAME:
        IRGeometryTriMesh.c

    DESCRIPTION:
        Quesa interactive renderer TriMesh implementation.

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
#include "IRPrefix.h"
#include "IRGeometry.h"
#include "IRGeometryTriMesh.h"
#include "IRTexture.h"
#include "IRTransparent.h"
#include "IRUpdate.h"

#include "GLDrawContext.h"
#include "GLUtils.h"




//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
const TQ3Uns32 kVAUnknownParent									= 0xFFFFFFFF;
const TQ3Uns32 kVAUnknownParentByte								= 0xFF;





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Vertex array
typedef struct TQ3VertexArray {
	// Global data
	TQ3InteractiveData			*instanceData;
	TQ3TriMeshData				*geomData;
	TQ3ViewObject				theView;


	// Misc state
	TQ3Boolean					renderEdges;


	// Geometry state
	TQ3Boolean					geomNeedsUVs;
	TQ3Boolean					geomIsHilighted;
	TQ3ColorRGB					geomDiffuse;
	TQ3ColorRGB					geomTransparency;
	TQ3ColorRGB					geomHilightDiffuse;
	TQ3ColorRGB					geomHilightTransparency;
	TQ3ColorRGB					geomFinalDiffuse;
	TQ3ColorRGB					geomFinalTransparency;
	

	// Attribute state
	TQ3ColorRGB					*geomTriDiffuseData;			// Attribute from geomData
	TQ3Uns8						*geomTriDiffuseFlag;			// Attribute from geomData
	TQ3ColorRGB					*geomTriTransparencyData;		// Attribute from geomData
	TQ3Uns8						*geomTriTransparencyFlag;		// Attribute from geomData
	TQ3Vector3D					*geomTriNormalsData;			// Attribute from geomData
	TQ3Uns8						*geomTriNormalsFlag;			// Attribute from geomData
	TQ3Switch					*geomTriHighlightData;			// Attribute from geomData
	TQ3Uns8						*geomTriHighlightFlag;			// Attribute from geomData
	TQ3ColorRGB					*geomEdgeDiffuseData;			// Attribute from geomData
	TQ3Uns8						*geomEdgeDiffuseFlag;			// Attribute from geomData
	TQ3ColorRGB					*geomEdgeTransparencyData;		// Attribute from geomData
	TQ3Uns8						*geomEdgeTransparencyFlag;		// Attribute from geomData
	TQ3Switch					*geomEdgeHighlightData;			// Attribute from geomData
	TQ3Uns8						*geomEdgeHighlightFlag;			// Attribute from geomData
	TQ3ColorRGB					*geomVertexDiffuseData;			// Attribute from geomData
	TQ3Uns8						*geomVertexDiffuseFlag;			// Attribute from geomData
	TQ3ColorRGB					*geomVertexTransparencyData;	// Attribute from geomData
	TQ3Uns8						*geomVertexTransparencyFlag;	// Attribute from geomData
	TQ3Vector3D					*geomVertexNormalsData;			// Attribute from geomData
	TQ3Uns8						*geomVertexNormalsFlag;			// Attribute from geomData
	TQ3Param2D					*geomVertexUVsData;				// Attribute from geomData
	TQ3Uns8						*geomVertexUVsFlag;				// Attribute from geomData
	TQ3Switch					*geomVertexHighlightData;		// Attribute from geomData
	TQ3Uns8						*geomVertexHighlightFlag;		// Attribute from geomData


	// Colour state
	TQ3ColorRGB					*currentGeomColour;				// geomDiffuse           or geomTransparency
	TQ3ColorRGB					*currentHilightColour;			// geomHilightDiffuse    or geomHilightTransparency
	TQ3ColorRGB					*currentTriColourData;			// geomTriDiffuseData    or geomTriTransparencyData
	TQ3Uns8						*currentTriColourFlag;			// geomTriDiffuseFlag    or geomTriTransparencyFlag
	TQ3ColorRGB					*currentEdgeColourData;			// geomEdgeDiffuseData   or geomEdgeTransparencyData
	TQ3Uns8						*currentEdgeColourFlag;			// geomEdgeDiffuseFlag   or geomEdgeTransparencyFlag
	TQ3ColorRGB					*currentVertexColourData;		// geomVertexDiffuseData or geomVertexTransparencyData
	TQ3Uns8						*currentVertexColourFlag;		// geomVertexDiffuseFlag or geomVertexTransparencyFlag
	TQ3ColorRGB					*currentParentColourData;		// currentTriColourData  or currentEdgeColourData
	TQ3Uns8						*currentParentColourFlag;		// currentTriColourFlag or currentEdgeColourFlag
	TQ3ColorRGB					**currentVertexColourPtr;		// vertexDiffuse         or vertexTransparency
	TQ3TriFlags					currentTriFlags;				// Triangle flags to match for processing


	// Index state
	TQ3Uns32					numIndices;						// Number of indices used in theIndices
	TQ3Uns32					*theIndices;					// Always scratch


	// Triangle state
	TQ3TriFlags					*triFlags;						// Always scratch
	TQ3Vector3D					*triNormals;					// May be geomTriNormalsData, may be scratch


	// Vertex state
	TQ3Point3D					*vertexPoints;					// Always geomData->points
	TQ3ColorRGB					*vertexDiffuse;					// May be geomVertexDiffuseData,      may be scratch
	TQ3ColorRGB					*vertexTransparency;			// May be geomVertexTransparencyData, may be scratch
	TQ3Vector3D					*vertexNormals;					// May be geomVertexNormalsData,      may be scratch or NULL
	TQ3Param2D					*vertexUVs;						// Always geomVertexUVsData,          may be NULL
	TQ3Uns32					*vertexParents;					// Always scratch
} TQ3VertexArray;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      ir_geom_trimesh_calc_edge_colour : Calculate edge colours.
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_calc_edge_colour(TQ3VertexArray *vertexArray, TQ3Uns32 edgeIndex, TQ3ColorRGB *theColour)
{	TQ3TriMeshEdgeData			*theEdge;
	TQ3Uns32					i0, i1;
	


	// Determines the appropriate colour for an edge lacking colour
	//
	// Edges which do not specify a colour attribute will inherit their colour
	// from the parent triangles if they exist, or the geometry colour if not.
	//
	// As well as the normal complication of the TQ3TriMeshAttributeData usage
	// array, the triangle references are themselves optional - so we need to
	// test to make sure the triangles exist before inheriting from them.
	//
	// Which triangle the edge inherits from seems to be undefined in QD3D, so
	// we inherit from the first triangle in preference to the second.
	theEdge = &vertexArray->geomData->edges[edgeIndex];
	i0      = theEdge->triangleIndices[0];
	i1      = theEdge->triangleIndices[1];



	// Use the triangle colour if the edge has parent triangles
	if (i0 != kQ3ArrayIndexNULL || i1 != kQ3ArrayIndexNULL)
		{
		// Use the triangle colour if present and used
		//
		// If the triangle colour's have a usage array, we also need to test that the
		// edge's triangle reference is valid before checking that usage array slot.
		if (vertexArray->currentTriColourData != NULL &&
		   (vertexArray->currentTriColourFlag == NULL ||
			(i0 != kQ3ArrayIndexNULL && vertexArray->currentTriColourFlag[i0]) ||
			(i1 != kQ3ArrayIndexNULL && vertexArray->currentTriColourFlag[i1])))
			{
			// If both triangles have colours, use the first one
			if (vertexArray->currentTriColourFlag == NULL)
				*theColour = vertexArray->currentTriColourData[i0];


			// Otherwise, use the first one if it's present and used
			else if (i0 != kQ3ArrayIndexNULL && vertexArray->currentTriColourFlag[i0])
				*theColour = vertexArray->currentTriColourData[i0];


			// Otherwise, use the second one if it's present and used
			else if (i1 != kQ3ArrayIndexNULL && vertexArray->currentTriColourFlag[i1])
				*theColour = vertexArray->currentTriColourData[i1];


			// Otherwise, we should never have got here!
			else
				Q3_ASSERT(!"Undefined edge colour - should never happen");
			}

		// Otherwise, use the geometry colour		
		else
			*theColour = *vertexArray->currentGeomColour;
		}


	// Otherwise, use the geometry colour
	else
		*theColour = *vertexArray->currentGeomColour;
}





//=============================================================================
//      ir_geom_trimesh_calc_vertex_colour : Calculate a vertex colour.
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_calc_vertex_colour(TQ3VertexArray			*vertexArray,
										TQ3Uns32			vertIndex,
										TQ3Uns32			parentIndex,
										TQ3ColorRGB			*theColour)
{	TQ3Switch		*parentHilightData, *vertexHilightData;
	TQ3Uns8			*parentHilightFlag, *vertexHilightFlag;
	TQ3Boolean		parentHilighted, vertexHilighted;
	TQ3ColorRGB		parentColour;



	// The vertex colour is selected from the hilight colour, the vertex colour, the
	// parent triangle/edge colour, or the geometry colour (in that order).
	//
	// The situation is complicated by the presence of hilights, and by the fact that the
	// colour and hilight attributes for vertices, triangles, and edges are specified with
	// the TQ3TriMeshAttributeData structure. This allows for an optional set of is-used
	// fields for each item in the array, which introduces the need for additional tests.
	//
	// The full sequence we need to follow is:
	//
	//		if geometry has hilight or
	//		   parent   has hilight or
	//         vertex   has hilight
	//			use hilight colour
	//
	//		else if vertex has colour
	//			use vertex colour
	//
	//		else if parent has colour
	//			use parent colour
	//
	//		else
	//			use geometry colour
	//
	// Due to the presence of the is-used fields, the "[vertex/parent] has hilight" tests
	// must test:
	//
	//		- That the hilight is present
	//		- That the hilight value is kQ3On
	//		- That the hilight is-used field is active (if supplied)
	//
	// If these three tests are met, the hilight value should be used for the vertex/parent.
	// The has-colour tests are similar, but without the need to test the actual value.
	//
	//
	// In addition, the final "else use geometry colour" case is more complex where an edge
	// is the parent, since an edge itself may have two optional pseudo-parents (the triangles
	// it bridges). If an edge is missing an attribute, values should be inherited from the
	// edge parents before the geometry.
	//
	// Since this is a special case, and quite complex, we handle this with a separate routine
	// which will return either the edge's parent triangle colour or the geometry colour.
	//
	//
	// We are slightly inefficient, in that we will recalculate the parent state for each
	// triangle/edge on-demand, rather than caching any previously calculated values: but
	// since this is the slow path anyway, we assume that's OK.
	vertexHilightData = vertexArray->geomVertexHighlightData;
	vertexHilightFlag = vertexArray->geomVertexHighlightFlag;

	if (vertexArray->renderEdges)
		{
		parentHilightData = vertexArray->geomEdgeHighlightData;
		parentHilightFlag = vertexArray->geomEdgeHighlightFlag;
		}
	else
		{
		parentHilightData = vertexArray->geomTriHighlightData;
		parentHilightFlag = vertexArray->geomTriHighlightFlag;
		}

	parentHilighted = (parentHilightData != NULL && parentHilightData[parentIndex] &&
					  (parentHilightFlag == NULL || parentHilightFlag[parentIndex]));



	// If the geometry or parent are hilighted, use the hilight colour
	if (vertexArray->geomIsHilighted || parentHilighted)
		*theColour = *vertexArray->currentHilightColour;

	else
		{
		// Check to see if the vertex hilight is present, on, and used
		vertexHilighted = (vertexHilightData != NULL && vertexHilightData[vertIndex] &&
						  (vertexHilightFlag == NULL || vertexHilightFlag[vertIndex]));


		// If the vertex is hilighted, use the hilight colour
		if (vertexHilighted)
			*theColour = *vertexArray->currentHilightColour;


		// Or calculate the real colour for the vertex
		else
			{
			// Use the vertex colour if present and used
			if (vertexArray->currentVertexColourData != NULL &&
			   (vertexArray->currentVertexColourFlag == NULL || vertexArray->currentVertexColourFlag[vertIndex]))
				*theColour = vertexArray->currentVertexColourData[vertIndex];
			
			
			// Or use the parent colour
			else
				{
				// Use the parent colour if present and used
				if (vertexArray->currentParentColourData != NULL &&
				   (vertexArray->currentParentColourFlag == NULL || vertexArray->currentParentColourFlag[parentIndex]))
					parentColour = vertexArray->currentParentColourData[parentIndex];
					
					
				// Or use the parent's parent colour
				else
					{
					// If we're rendering edges, the parent may involve the edge's triangles
					if (vertexArray->renderEdges)
						ir_geom_trimesh_calc_edge_colour(vertexArray, parentIndex, &parentColour);
							
					// If we're rendering triangles, the parent is the geometry
					else
						parentColour = *vertexArray->currentGeomColour;
					}


				// Set the vertex to the parent colour
				*theColour = parentColour;
				}
			}
		}
}





//=============================================================================
//      ir_geom_trimesh_vertex_colours_all_supplied : Vertex colours.
//-----------------------------------------------------------------------------
//		Note :	Identifies the case where the application supplied per-vertex
//				colours can be used directly.
//-----------------------------------------------------------------------------
static TQ3Boolean
ir_geom_trimesh_vertex_colours_all_supplied(TQ3VertexArray *vertexArray)
{


	// Check for per-vertex colours without a usage array
	if (vertexArray->currentVertexColourData != NULL &&
		vertexArray->currentVertexColourFlag == NULL)
		{
		*vertexArray->currentVertexColourPtr = vertexArray->currentVertexColourData;
		return(kQ3True);
		}
	
	return(kQ3False);
}





//=============================================================================
//      ir_geom_trimesh_vertex_colours_single_colour : Vertex colours.
//-----------------------------------------------------------------------------
//		Note :	Identifies the case where the TriMesh is rendered with a single
//				colour, rather than per-vertex colours.
//-----------------------------------------------------------------------------
static TQ3Boolean
ir_geom_trimesh_vertex_colours_single_colour(TQ3VertexArray *vertexArray)
{


	// Check for a single per-geometry colour (i.e., no triangle or vertex colours)
	if (vertexArray->currentTriColourData    == NULL &&
		vertexArray->currentVertexColourData == NULL)
		{
		*vertexArray->currentVertexColourPtr = NULL;
		return(kQ3True);
		}
	
	return(kQ3False);
}





//=============================================================================
//      ir_geom_trimesh_vertex_colours_inherit : Vertex colours.
//-----------------------------------------------------------------------------
//		Note :	The general case - derive the correct colour for each vertex,
//				given the supplied colours at the vertex, triangle/edge, and
//				geometry level (as well as hilights at any of those levels).
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_vertex_colours_inherit(TQ3VertexArray *vertexArray)
{	TQ3ColorRGB		*vertexColours;
	TQ3Uns32		n,  m;



	// Calculate the values into the scratch space
	vertexColours = *vertexArray->currentVertexColourPtr;
	for (n = 0; n < vertexArray->geomData->numPoints; n++)
		{
		m = vertexArray->vertexParents[n];
		if (E3Bit_AnySet(vertexArray->triFlags[m], vertexArray->currentTriFlags))
			ir_geom_trimesh_calc_vertex_colour(vertexArray, n, m, &vertexColours[n]);
		}
}





//=============================================================================
//      ir_geom_trimesh_add_transparent : Add a transparent triangle/edge.
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_add_transparent(TQ3VertexArray *vertexArray, TQ3Uns32 numVerts, TQ3Uns32 *theIndices)
{	TQ3FVertex3D	theVertices[3];
	TQ3Uns32		n;



	// Prepare the data
	for (n = 0; n < numVerts; n++)
		{
		theVertices[n].theFlags = kQ3FVertexHaveDiffuse | kQ3FVertexHaveTransparency;
		theVertices[n].thePoint = vertexArray->geomData->points[theIndices[n]];

		if (vertexArray->vertexDiffuse != NULL)
			theVertices[n].colourDiffuse = vertexArray->vertexDiffuse[theIndices[n]];
		else
			theVertices[n].colourDiffuse = vertexArray->geomFinalDiffuse;

		if (vertexArray->vertexTransparency != NULL)
			theVertices[n].colourTransparency = vertexArray->vertexTransparency[theIndices[n]];
		else
			theVertices[n].colourTransparency = vertexArray->geomFinalTransparency;

		if (vertexArray->vertexNormals != NULL)
			{
			theVertices[n].theFlags |= kQ3FVertexHaveNormal;
			theVertices[n].theNormal = vertexArray->vertexNormals[theIndices[n]];
			}

		if (vertexArray->vertexUVs != NULL)
			{
			theVertices[n].theFlags |= kQ3FVertexHaveUV;
			theVertices[n].theUV     = vertexArray->vertexUVs[theIndices[n]];
			}
		}



	// And submit the appropriate object
	if (numVerts == 2)
		IRTransBuffer_AddLine(vertexArray->theView, vertexArray->instanceData, theVertices);
	else
		IRTransBuffer_AddTriangle(vertexArray->theView, vertexArray->instanceData, theVertices);
}





//=============================================================================
//      ir_geom_trimesh_build_vertex_colours : Build the vertex colours.
//-----------------------------------------------------------------------------
//		Note :	Colours are the most complex vertex attribute, due to the need
//				to inherit colours from parent triangles, edges, or the
//				geometry - as well as the usage array at each level, and the
//				possible presence of an overriding hilight colour (at each
//				level, also with a usage array and a switch!).
//
//				To save code duplication, we can be invoked for either the
//				diffuse or the transparency colour - the tests are the same
//				for either case, we just use a different set of pointers.
//
//
//				The final result from this function will be one of two cases:
//
//					1. vertexArray->vertexDiffuse will point to an array of
//					individual vertex colours to pass to OpenGL.
//
//					2. vertexArray->vertexDiffuse will be NULL, and all
//					vertices will be drawn in vertexArray->geomDiffuse.
//
//				If calcDiffuse is false, the equivalent transparency colours
//				will be returned.
//
//
//				We identify a couple of common fast-path cases which allow us
//				to produce a quick solution to either 1 or 2, and have a
//				fall-back case which performs the full colour derivation model
//				to obtain the final colour for each vertex.
//
//				Over time we can probably identify further cases which should
//				have their own fast-path, but the fall-back case means we will
//				always produce the correct results (at the cost of speed).
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_build_vertex_colours(TQ3VertexArray *vertexArray, TQ3Boolean calcDiffuse)
{


	// Set up our pointers
	//
	// We prepare a set of pointers to the appropriate diffuse or transparency
	// colours, which allows us to detect and handle fast-paths for both cases.
	if (calcDiffuse)
		{
		vertexArray->currentGeomColour       = &vertexArray->geomDiffuse;
		vertexArray->currentHilightColour    = &vertexArray->geomHilightDiffuse;
		vertexArray->currentTriColourData    =  vertexArray->geomTriDiffuseData;
		vertexArray->currentTriColourFlag    =  vertexArray->geomTriDiffuseFlag;
		vertexArray->currentEdgeColourData   =  vertexArray->geomEdgeDiffuseData;
		vertexArray->currentEdgeColourFlag   =  vertexArray->geomEdgeDiffuseFlag;
		vertexArray->currentVertexColourData =  vertexArray->geomVertexDiffuseData;
		vertexArray->currentVertexColourFlag =  vertexArray->geomVertexDiffuseFlag;
		vertexArray->currentVertexColourPtr  = &vertexArray->vertexDiffuse;
		vertexArray->currentTriFlags         = kQ3TriFlagVisible;
		}
	else
		{
		vertexArray->currentGeomColour       = &vertexArray->geomTransparency;
		vertexArray->currentHilightColour    = &vertexArray->geomHilightTransparency;
		vertexArray->currentTriColourData    =  vertexArray->geomTriTransparencyData;
		vertexArray->currentTriColourFlag    =  vertexArray->geomTriTransparencyFlag;
		vertexArray->currentEdgeColourData   =  vertexArray->geomEdgeTransparencyData;
		vertexArray->currentEdgeColourFlag   =  vertexArray->geomEdgeTransparencyFlag;
		vertexArray->currentVertexColourData =  vertexArray->geomVertexTransparencyData;
		vertexArray->currentVertexColourFlag =  vertexArray->geomVertexTransparencyFlag;
		vertexArray->currentVertexColourPtr  = &vertexArray->vertexTransparency;
		vertexArray->currentTriFlags         = kQ3TriFlagVisible | kQ3TriFlagTransparent;
		}

	if (vertexArray->renderEdges)
		{
		vertexArray->currentParentColourData = vertexArray->currentEdgeColourData;
		vertexArray->currentParentColourFlag = vertexArray->currentEdgeColourFlag;
		}
	else
		{
		vertexArray->currentParentColourData = vertexArray->currentTriColourData;
		vertexArray->currentParentColourFlag = vertexArray->currentTriColourFlag;
		}



	// Fast path - app supplies all vertex colours
	if (ir_geom_trimesh_vertex_colours_all_supplied(vertexArray))
		return;



	// Fast path - TriMesh is a single colour
	if (ir_geom_trimesh_vertex_colours_single_colour(vertexArray))
		return;



	// For anything else, we need to perform the full inheritence test
	ir_geom_trimesh_vertex_colours_inherit(vertexArray);
}





//=============================================================================
//      ir_geom_trimesh_build_vertex_normals : Build the vertex normals.
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_build_vertex_normals(TQ3VertexArray *vertexArray)
{	TQ3Uns32	n, m;



	// Decide if we need vertex normals
	if (vertexArray->renderEdges || vertexArray->instanceData->stateInterpolation == kQ3InterpolationStyleNone)
		{
		vertexArray->vertexNormals = NULL;
		return;
		}



	// If normals have been supplied, ensure we have them all
	if (vertexArray->geomVertexNormalsData != NULL)
		{
		// Adjust our pointer to use the supplied data
		vertexArray->vertexNormals = vertexArray->geomVertexNormalsData;


		// Fill in any missing values
		if (vertexArray->geomVertexNormalsFlag != NULL)
			{
			for (n = 0; n < vertexArray->geomData->numPoints; n++)
				{
				if (!vertexArray->geomVertexNormalsFlag[n])
					{
					m = vertexArray->vertexParents[n];
					if (E3Bit_IsSet(vertexArray->triFlags[m], kQ3TriFlagVisible))
						vertexArray->vertexNormals[n] = vertexArray->triNormals[m];
					}
				}
			}
		}
	
	
	
	// Otherwise, calculate the values into the scratch space
	else
		{
		for (n = 0; n < vertexArray->geomData->numPoints; n++)
			{
			m = vertexArray->vertexParents[n];
			if (E3Bit_IsSet(vertexArray->triFlags[m], kQ3TriFlagVisible))
				vertexArray->vertexNormals[n] = vertexArray->triNormals[m];
			}
		}



	// Validate the vertices
	//
	// At present only normals are validated and so we do validation here.
	IRGeometry_Validate_Vertices(vertexArray->instanceData,
								 vertexArray->geomData->numPoints,
								 sizeof(TQ3Vector3D),
								 vertexArray->vertexNormals);
}





//=============================================================================
//      ir_geom_trimesh_build_vertex_uvs : Build the vertex UVs.
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_build_vertex_uvs(TQ3VertexArray *vertexArray)
{	TQ3Uns32	n;



	// Decide if we need vertex UVs
	if (!vertexArray->geomNeedsUVs)
		{
		vertexArray->vertexUVs = NULL;
		return;
		}



	// If UVs have been supplied, ensure we have them all	
	if (vertexArray->geomVertexUVsData != NULL)
		{
		// Adjust our pointer to use the supplied data
		vertexArray->vertexUVs = vertexArray->geomVertexUVsData;


		// Fill in any missing values
		if (vertexArray->geomVertexUVsFlag != NULL)
			{
			for (n = 0; n < vertexArray->geomData->numPoints; n++)
				{
				if (!vertexArray->geomVertexUVsFlag[n])
					{
					vertexArray->vertexUVs[n].u = 0.0f;
					vertexArray->vertexUVs[n].v = 0.0f;
					}
				}
			}
		}
}





//=============================================================================
//      ir_geom_trimesh_build_triangles : Build the triangle list.
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_build_triangles(TQ3VertexArray *vertexArray)
{	TQ3Uns32		*theIndices;
	TQ3Status		qd3dStatus;
	TQ3Uns32		n;



	// Initialise the triangle normals
	//
	// If normals have been supplied, ensure we have them all
	if (vertexArray->geomTriNormalsData != NULL)
		{
		// Adjust our pointer to use the supplied data
		vertexArray->triNormals = vertexArray->geomTriNormalsData;


		// Fill in any missing values
		if (vertexArray->geomTriNormalsFlag != NULL)
			IRGeometry_Generate_Triangle_Normals(vertexArray->instanceData,
												 vertexArray->geomData->numTriangles,
												 vertexArray->geomTriNormalsFlag,
												 (TQ3Uns32 *) vertexArray->geomData->triangles,
												 vertexArray->geomData->points,
												 vertexArray->triNormals);
		}


	// Otherwise, calculate the values into the scratch space
	else
		IRGeometry_Generate_Triangle_Normals(vertexArray->instanceData,
											 vertexArray->geomData->numTriangles,
											 NULL,
											 (TQ3Uns32 *) vertexArray->geomData->triangles,
											 vertexArray->geomData->points,
											 vertexArray->triNormals);



	// Validate the triangles
	IRGeometry_Validate_Triangles(vertexArray->instanceData,
								  vertexArray->geomData->numTriangles,
								  (TQ3Uns32 *) vertexArray->geomData->triangles,
								  vertexArray->geomData->points,
								  vertexArray->triNormals);



	// Initialise the vertex parent list
	//
	// Note that we assume the first triangle which references a given vertex
	// acts as the parent for inheriting attributes such as colour/normal.
	for (n = 0; n < vertexArray->geomData->numTriangles; n++)
		{
		theIndices = vertexArray->geomData->triangles[n].pointIndices;
		
		if (vertexArray->vertexParents[theIndices[0]] == kVAUnknownParent)
			vertexArray->vertexParents[theIndices[0]] = n;
		
		if (vertexArray->vertexParents[theIndices[1]] == kVAUnknownParent)
			vertexArray->vertexParents[theIndices[1]] = n;
		
		if (vertexArray->vertexParents[theIndices[2]] == kVAUnknownParent)
			vertexArray->vertexParents[theIndices[2]] = n;
		}



	// Initialise the triangle flags
	qd3dStatus = IRGeometry_Generate_Triangle_Flags(vertexArray->instanceData,
											 		vertexArray->geomData->numTriangles,
											 		(TQ3Uns32 *) vertexArray->geomData->triangles,
											 		vertexArray->geomData->points,
											 		vertexArray->triNormals,
													vertexArray->triFlags);

	if (qd3dStatus != kQ3Success)
		Q3Memory_Clear(vertexArray->triFlags, vertexArray->geomData->numTriangles * sizeof(TQ3TriFlags));
}





//=============================================================================
//      ir_geom_trimesh_build_transparent : Identify transparent triangles.
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_build_transparent(TQ3VertexArray *vertexArray)
{	TQ3Boolean		isTransparent, allTransparent;
	TQ3Uns32		*theIndices;
	TQ3ColorRGB		*theColour;
	TQ3Uns32		n, m;



	// Start off assuming we're opaque
	isTransparent  = kQ3False;
	allTransparent = kQ3False;



	// If the geometry is transparent, mark every triangle as transparent
	//
	// We pick up transparent geometry either from the colour or the current texture.
	theColour = (vertexArray->geomIsHilighted ? &vertexArray->geomHilightTransparency : &vertexArray->geomTransparency);
	if (theColour->r != 1.0f || theColour->g != 1.0f || theColour->b != 1.0f ||
		vertexArray->instanceData->stateTextureIsTransparent)
		{
		isTransparent  = kQ3True;
		allTransparent = kQ3True;

		for (n = 0; n < vertexArray->geomData->numTriangles; n++)
			E3Bit_Set(vertexArray->triFlags[n], kQ3TriFlagTransparent);
		}



	// Check for transparent triangles
	if (!allTransparent)
		{
		if (vertexArray->geomTriTransparencyData != NULL)
			{
			isTransparent  = kQ3True;
			allTransparent = (vertexArray->geomTriTransparencyFlag == NULL);
			
			for (n = 0; n < vertexArray->geomData->numTriangles; n++)
				{
				if (vertexArray->geomTriTransparencyFlag == NULL || vertexArray->geomTriTransparencyFlag[n])
					E3Bit_Set(vertexArray->triFlags[n], kQ3TriFlagTransparent);
				}
			}
		}



	// Check for transparent edges
	//
	// Note that edges are flagged as transparent by marking their associated triangles
	// as transparent. Since the transparency colour is calculated for each vertex, we
	// can use the triangle flag as a convient way to indicate transparent edges rather
	// than keeping a separate set of flags for each edge.
	if (!allTransparent && vertexArray->renderEdges)
		{
		if (vertexArray->geomEdgeTransparencyData != NULL)
			{
			isTransparent  = kQ3True;
			allTransparent = (vertexArray->geomEdgeTransparencyFlag == NULL);
			
			for (n = 0; n < vertexArray->geomData->numEdges; n++)
				{
				if (vertexArray->geomEdgeTransparencyFlag == NULL || vertexArray->geomEdgeTransparencyFlag[n])
					{
					theIndices = vertexArray->geomData->edges[n].triangleIndices;
					E3Bit_Set(vertexArray->triFlags[theIndices[0]], kQ3TriFlagTransparent);
					E3Bit_Set(vertexArray->triFlags[theIndices[1]], kQ3TriFlagTransparent);
					}
				}
			}
		}



	// Check for transparent vertices
	if (!allTransparent)
		{
		if (vertexArray->geomVertexTransparencyData != NULL)
			{
			isTransparent  = kQ3True;
			allTransparent = (vertexArray->geomVertexTransparencyFlag == NULL);
			
			for (n = 0; n < vertexArray->geomData->numPoints; n++)
				{
				if (vertexArray->geomVertexTransparencyFlag == NULL || vertexArray->geomVertexTransparencyFlag[n])
					{
					m = vertexArray->vertexParents[n];
					E3Bit_Set(vertexArray->triFlags[m], kQ3TriFlagTransparent);
					}
				}
			}
		}



	// If we're going to need them, calculate the transparency colours
	if (isTransparent)
		ir_geom_trimesh_build_vertex_colours(vertexArray, kQ3False);
}





//=============================================================================
//      ir_geom_trimesh_build_indices : Build the indicies.
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_build_indices(TQ3VertexArray *vertexArray)
{	TQ3TriFlags		edgeTriFlag0, edgeTriFlag1;
	TQ3Uns32		*theIndices;
	TQ3Uns32		n, m;



	// Build the edge index list
	if (vertexArray->renderEdges)
		{
		// Build the list of edges to render
		for (n = 0; n < vertexArray->geomData->numEdges; n++)
			{
			// Get the flags for the edge triangles
			edgeTriFlag0 = kQ3TriFlagNone;
			edgeTriFlag1 = kQ3TriFlagNone;
			theIndices   = vertexArray->geomData->edges[n].triangleIndices;

			if (theIndices[0] != kQ3ArrayIndexNULL)
				edgeTriFlag0 = vertexArray->triFlags[theIndices[0]];

			if (theIndices[1] != kQ3ArrayIndexNULL)
				edgeTriFlag1 = vertexArray->triFlags[theIndices[1]];


			// Examine visible edges
			//
			// As per QD3D, and edge is visible if it references a visible triangle
			if (E3Bit_IsSet(edgeTriFlag0, kQ3TriFlagVisible) || E3Bit_IsSet(edgeTriFlag1, kQ3TriFlagVisible))
				{
				// Get the indices
				theIndices = vertexArray->geomData->edges[n].pointIndices;


				// Save transparent edges for later
				if (E3Bit_IsSet(edgeTriFlag0, kQ3TriFlagTransparent) || E3Bit_IsSet(edgeTriFlag1, kQ3TriFlagTransparent))
					ir_geom_trimesh_add_transparent(vertexArray, 2, theIndices);


				// And accumulate opaque edges into the array
				else
					{
					for (m = 0; m < 2; m++)
						vertexArray->theIndices[vertexArray->numIndices + m] = theIndices[m];
	
					vertexArray->numIndices += 2;
					}
				}
			}
		}
	
	
	
	// Build the triangle index list
	else
		{
		// Build the list of indices to render
		for (n = 0; n < vertexArray->geomData->numTriangles; n++)
			{
			// Examine visible triangles
			if (E3Bit_IsSet(vertexArray->triFlags[n], kQ3TriFlagVisible))
				{
				// Get the indices
				theIndices = vertexArray->geomData->triangles[n].pointIndices;


				// Save transparent triangles for later
				if (E3Bit_IsSet(vertexArray->triFlags[n], kQ3TriFlagTransparent))
					ir_geom_trimesh_add_transparent(vertexArray, 3, theIndices);


				// And accumulate opaque triangles into the array
				else
					{
					for (m = 0; m < 3; m++)
						vertexArray->theIndices[vertexArray->numIndices + m] = theIndices[m];
	
					vertexArray->numIndices += 3;
					}
				}
			}
		}
}





//=============================================================================
//      ir_geom_trimesh_initialise : Prepare to render a TriMesh.
//-----------------------------------------------------------------------------
static TQ3Status
ir_geom_trimesh_initialise(TQ3ViewObject				theView,
								TQ3InteractiveData		*instanceData,
								TQ3TriMeshData			*geomData,
								TQ3AttributeSet			geomAttributes,
								TQ3VertexArray			*vertexArray)
{	TQ3Uns32					sizeVertNormals, sizeVertDiffuse, sizeVertTransparency, sizeVertParents;
	TQ3Uns32					offVertNormals, offVertDiffuse, offVertTransparency, offVertParents;
	TQ3Uns32					sizeIndices, sizeTriFlags, sizeTriNormals;
	TQ3Uns32					offIndices, offTriFlags, offTriNormals;
	TQ3Uns32					n, numIndices, requiredSize;
	TQ3TriMeshAttributeData		*theAttribute;
	TQ3Boolean					renderEdges;
	TQ3Status					qd3dStatus;
	TQ3FVertex3D				tmpFVertex;
	TQ3Vertex3D					tmpVertex;


	// Calculate our basic triangle/edge values
	renderEdges = (instanceData->stateFill == kQ3FillStyleEdges && geomData->numEdges != 0);
	numIndices  = (renderEdges ? (geomData->numEdges * 2) : (geomData->numTriangles * 3));



	// Grow the vertex array scratch space
	//
	// We assume the worst case, and allocate space for every pointer we could
	// possibly need. In practice the largest object in a scene will cause the
	// scratch space to expand once, and smaller objects will fit in that space.
	//
	// Possible optimisation - don't allocate normals or transparency colours if
	// we don't really need them for rendering (we just assume they are for now).
	sizeIndices          = (numIndices             * sizeof(TQ3Uns32));
	sizeTriFlags         = (geomData->numTriangles * sizeof(TQ3TriFlags));
	sizeTriNormals       = (geomData->numTriangles * sizeof(TQ3Vector3D));
	sizeVertDiffuse      = (geomData->numPoints    * sizeof(TQ3ColorRGB));
	sizeVertTransparency = (geomData->numPoints    * sizeof(TQ3ColorRGB));
	sizeVertNormals      = (geomData->numPoints    * sizeof(TQ3Vector3D));
	sizeVertParents      = (geomData->numPoints    * sizeof(TQ3Uns32));

	requiredSize = (sizeIndices     + 
					sizeTriFlags    + sizeTriNormals + 
					sizeVertDiffuse + sizeVertTransparency + sizeVertNormals + sizeVertParents);

	if (requiredSize > instanceData->triMeshSize)
		{
		qd3dStatus = Q3Memory_Reallocate(&instanceData->triMeshData, requiredSize);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);
		
		instanceData->triMeshSize = requiredSize;
		}



	// Set up the vertex array
	Q3Memory_Clear(vertexArray, sizeof(TQ3VertexArray));
	
	vertexArray->instanceData  = instanceData;
	vertexArray->geomData      = geomData;
	vertexArray->theView       = theView;



	// Set up the misc/geometry state
	tmpVertex.attributeSet = geomAttributes;
	IRGeometry_Generate_Vertex_State(instanceData, NULL, &tmpVertex, &tmpFVertex);
	if (!E3Bit_IsSet(tmpFVertex.theFlags, kQ3FVertexHaveTransparency))
		Q3ColorRGB_Set(&tmpFVertex.colourTransparency, 1.0f, 1.0f, 1.0f);

	vertexArray->renderEdges           = renderEdges;
	vertexArray->geomIsHilighted       = (instanceData->stateGeomHilightState == kQ3On && instanceData->stateHilight != NULL);
	vertexArray->geomNeedsUVs          = (instanceData->stateTextureActive             && !vertexArray->renderEdges);
	vertexArray->geomDiffuse           = tmpFVertex.colourDiffuse;
	vertexArray->geomTransparency      = tmpFVertex.colourTransparency;
	vertexArray->geomFinalDiffuse      = vertexArray->geomDiffuse;
	vertexArray->geomFinalTransparency = vertexArray->geomTransparency;

	if (vertexArray->geomIsHilighted)
		{
		vertexArray->geomHilightDiffuse      = vertexArray->geomDiffuse;
		vertexArray->geomHilightTransparency = vertexArray->geomTransparency;
		
		Q3AttributeSet_Get(instanceData->stateHilight, kQ3AttributeTypeDiffuseColor,      &vertexArray->geomHilightDiffuse);
		Q3AttributeSet_Get(instanceData->stateHilight, kQ3AttributeTypeTransparencyColor, &vertexArray->geomHilightTransparency);

		vertexArray->geomFinalDiffuse      = vertexArray->geomHilightDiffuse;
		vertexArray->geomFinalTransparency = vertexArray->geomHilightTransparency;
		}



	// Set up the attribute state
	for (n = 0; n < geomData->numTriangleAttributeTypes; n++)
		{
		theAttribute = &geomData->triangleAttributeTypes[n];

		switch (theAttribute->attributeType) {
			case kQ3AttributeTypeDiffuseColor:
				vertexArray->geomTriDiffuseData = (TQ3ColorRGB *) theAttribute->data;
				vertexArray->geomTriDiffuseFlag = (TQ3Uns8     *) theAttribute->attributeUseArray;
				break;

			case kQ3AttributeTypeTransparencyColor:
				vertexArray->geomTriTransparencyData = (TQ3ColorRGB *) theAttribute->data;
				vertexArray->geomTriTransparencyFlag = (TQ3Uns8     *) theAttribute->attributeUseArray;
				break;

			case kQ3AttributeTypeNormal:
				vertexArray->geomTriNormalsData = (TQ3Vector3D *) theAttribute->data;
				vertexArray->geomTriNormalsFlag = (TQ3Uns8     *) theAttribute->attributeUseArray;
				break;

			case kQ3AttributeTypeHighlightState:
				vertexArray->geomTriHighlightData = (TQ3Switch *) theAttribute->data;
				vertexArray->geomTriHighlightFlag = (TQ3Uns8   *) theAttribute->attributeUseArray;
				break;
			}
		}


	for (n = 0; n < geomData->numEdgeAttributeTypes; n++)
		{
		theAttribute = &geomData->edgeAttributeTypes[n];

		switch (theAttribute->attributeType) {
			case kQ3AttributeTypeDiffuseColor:
				vertexArray->geomEdgeDiffuseData = (TQ3ColorRGB *) theAttribute->data;
				vertexArray->geomEdgeDiffuseFlag = (TQ3Uns8     *) theAttribute->attributeUseArray;
				break;

			case kQ3AttributeTypeTransparencyColor:
				vertexArray->geomEdgeTransparencyData = (TQ3ColorRGB *) theAttribute->data;
				vertexArray->geomEdgeTransparencyFlag = (TQ3Uns8     *) theAttribute->attributeUseArray;
				break;

			case kQ3AttributeTypeHighlightState:
				vertexArray->geomEdgeHighlightData = (TQ3Switch *) theAttribute->data;
				vertexArray->geomEdgeHighlightFlag = (TQ3Uns8   *) theAttribute->attributeUseArray;
				break;
			}
		}


	for (n = 0; n < geomData->numVertexAttributeTypes; n++)
		{
		theAttribute = &geomData->vertexAttributeTypes[n];

		switch (theAttribute->attributeType) {
			case kQ3AttributeTypeDiffuseColor:
				vertexArray->geomVertexDiffuseData = (TQ3ColorRGB *) theAttribute->data;
				vertexArray->geomVertexDiffuseFlag = (TQ3Uns8     *) theAttribute->attributeUseArray;
				break;

			case kQ3AttributeTypeTransparencyColor:
				vertexArray->geomVertexTransparencyData = (TQ3ColorRGB *) theAttribute->data;
				vertexArray->geomVertexTransparencyFlag = (TQ3Uns8     *) theAttribute->attributeUseArray;
				break;

			case kQ3AttributeTypeNormal:
				vertexArray->geomVertexNormalsData = (TQ3Vector3D *) theAttribute->data;
				vertexArray->geomVertexNormalsFlag = (TQ3Uns8     *) theAttribute->attributeUseArray;
				break;

			case kQ3AttributeTypeShadingUV:
			case kQ3AttributeTypeSurfaceUV:
				vertexArray->geomVertexUVsData = (TQ3Param2D *) theAttribute->data;
				vertexArray->geomVertexUVsFlag = (TQ3Uns8    *) theAttribute->attributeUseArray;
				break;

			case kQ3AttributeTypeHighlightState:
				vertexArray->geomVertexHighlightData = (TQ3Switch *) theAttribute->data;
				vertexArray->geomVertexHighlightFlag = (TQ3Uns8   *) theAttribute->attributeUseArray;
				break;
			}
		}
		


	// Set up the vertex array pointers
	//
	// Our pointers are set up to point to the scratch space in case we need to copy
	// the TriMesh data, and may be overwritten to point to the TriMesh data (if no
	// copying is required) or set to NULL (if the data is not used).
	//
	// Note that vertexPoints is always just set to the supplied point data for now,
	// since we don't support inserting new vertices to handle shared inheritence
	// conflicts - so we may as well use the application data without copying.
	offIndices          = 0;
	offTriFlags         = offIndices          + sizeIndices;
	offTriNormals       = offTriFlags         + sizeTriFlags;
	offVertDiffuse      = offTriNormals       + sizeTriNormals;
	offVertTransparency = offVertDiffuse      + sizeVertDiffuse;
	offVertNormals      = offVertTransparency + sizeVertTransparency;
	offVertParents      = offVertNormals      + sizeVertNormals;

	vertexArray->theIndices         = (TQ3Uns32    *) &instanceData->triMeshData[offIndices];
	vertexArray->triFlags           = (TQ3TriFlags *) &instanceData->triMeshData[offTriFlags];
	vertexArray->triNormals         = (TQ3Vector3D *) &instanceData->triMeshData[offTriNormals];
	vertexArray->vertexPoints       = geomData->points;
	vertexArray->vertexDiffuse      = (TQ3ColorRGB *) &instanceData->triMeshData[offVertDiffuse];
	vertexArray->vertexTransparency = (TQ3ColorRGB *) &instanceData->triMeshData[offVertTransparency];
	vertexArray->vertexNormals      = (TQ3Vector3D *) &instanceData->triMeshData[offVertNormals];
	vertexArray->vertexParents      = (TQ3Uns32    *) &instanceData->triMeshData[offVertParents];

	memset(vertexArray->vertexParents, kVAUnknownParentByte, sizeVertParents);



	// Verify our pointers are aligned
	//
	// Not strictly an error, but for performance we want to make sure that our pointers are
	// at least 4-byte aligned (or we'll take a large performance hit on PowerPC). All our
	// items are multiples of 4 bytes at present, but this will catch us if we allocate any
	// single byte entries in the future.
	Q3_ASSERT(((TQ3Uns32) vertexArray->theIndices)         % 4 == 0);
	Q3_ASSERT(((TQ3Uns32) vertexArray->triFlags)           % 4 == 0);
	Q3_ASSERT(((TQ3Uns32) vertexArray->triNormals)         % 4 == 0);
	Q3_ASSERT(((TQ3Uns32) vertexArray->vertexPoints)       % 4 == 0);
	Q3_ASSERT(((TQ3Uns32) vertexArray->vertexDiffuse)      % 4 == 0);
	Q3_ASSERT(((TQ3Uns32) vertexArray->vertexTransparency) % 4 == 0);
	Q3_ASSERT(((TQ3Uns32) vertexArray->vertexNormals)      % 4 == 0);
	Q3_ASSERT(((TQ3Uns32) vertexArray->vertexParents)      % 4 == 0);

	return(kQ3Success);
}





//=============================================================================
//      ir_geom_trimesh_render : Render a TriMesh.
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_render(TQ3InteractiveData *instanceData,TQ3VertexArray *vertexArray)
{	TQ3Uns32	n, m;



	// Prepare to render
	glVertexPointer(3, GL_FLOAT, sizeof(TQ3Point3D), vertexArray->vertexPoints);

	GLUtils_UpdateClientState( (TQ3Boolean)(vertexArray->vertexDiffuse != NULL),
		&instanceData->glClientStateColor, GL_COLOR_ARRAY );
	
	if (vertexArray->vertexDiffuse != NULL)
		{
		glColorPointer(3, GL_FLOAT, sizeof(TQ3ColorRGB), vertexArray->vertexDiffuse);
		}
	else
		glColor3fv(&vertexArray->geomFinalDiffuse.r);


	GLUtils_UpdateClientState( (TQ3Boolean)(vertexArray->vertexNormals != NULL),
		&instanceData->glClientStateNormal, GL_NORMAL_ARRAY );

	if (vertexArray->vertexNormals != NULL)
		{
		glNormalPointer(GL_FLOAT, sizeof(TQ3Vector3D), vertexArray->vertexNormals);
		}

	GLUtils_UpdateClientState( (TQ3Boolean)(vertexArray->vertexUVs != NULL),
		&instanceData->glClientStateUV, GL_TEXTURE_COORD_ARRAY );

	if (vertexArray->vertexUVs != NULL)
		{
		glTexCoordPointer(2, GL_FLOAT, sizeof(TQ3Param2D), vertexArray->vertexUVs);
		}



	// Render the TriMesh (as edges, flat shaded, or smooth shaded)
	//
	// In edge rendering or smooth shaded triangles we render the TriMesh
	// edge/triangle list as-is as one primitive.
	//
	// In flat rendering, we must render each triangle with a per-triangle normal
	// rather than a per-vertex normal. Since OpenGL vertex arrays do not allow
	// for per-triangle normals, we need to submit each triangle individually.
	if (vertexArray->renderEdges)
		glDrawElements(GL_LINES, vertexArray->numIndices, GL_UNSIGNED_INT, vertexArray->theIndices);

	else if (vertexArray->instanceData->stateInterpolation == kQ3InterpolationStyleNone)
		{
		for (n = 0, m = 0; n < vertexArray->geomData->numTriangles; n++)
			{
			if (E3Bit_IsSet(vertexArray->triFlags[n], kQ3TriFlagVisible))
				{
				glNormal3fv((float *) &vertexArray->triNormals[n]);
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &vertexArray->theIndices[m]);
				m += 3;
				}
			}
		}

	else
		glDrawElements(GL_TRIANGLES, vertexArray->numIndices, GL_UNSIGNED_INT, vertexArray->theIndices);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRGeometry_TriMesh_Terminate : Terminate the TriMesh state.
//-----------------------------------------------------------------------------
#pragma mark -
void
IRGeometry_TriMesh_Terminate(TQ3InteractiveData *instanceData)
{


	// Dispose of the TriMesh state
	instanceData->triMeshSize = 0;
	Q3Memory_Free(&instanceData->triMeshData);
}





//=============================================================================
//      IRGeometry_Submit_TriMesh : TriMesh handler.
//-----------------------------------------------------------------------------
//		Note :	We attempt to use a single glDrawElements call to draw the
//				TriMesh, and set up our vertex array pointers to either the
//				application supplied data (if we can use it directly) or to
//				an area of scratch space (if we need to modify/calculate more
//				data).
//
//				Our main restriction compared to QD3D's TriMesh rendering is
//				that vertices will inherit attributes (colour, normal, etc)
//				from the first parent (triangle/edge) that references them: in
//				QD3D I think additional vertices would be inserted into the
//				rendering stream to allow each parent to get its own unique
//				view of the vertices they used - deferred for now.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Submit_TriMesh(TQ3ViewObject				theView,
							TQ3InteractiveData		*instanceData,
							TQ3GeometryObject		theGeom,
							TQ3TriMeshData			*geomData)
{	TQ3Boolean			hadAttributeTexture;
	TQ3VertexArray		vertexArray;
	TQ3Status			qd3dStatus;



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Update our state for this object and the texture mapping
	hadAttributeTexture = IRGeometry_Attribute_Handler(theView, geomData->triMeshAttributeSet,
											instanceData, kQ3XAttributeMaskGeometry | kQ3XAttributeMaskSurfaceShader);




	// Construct the vertex array
	qd3dStatus = ir_geom_trimesh_initialise(theView, instanceData, geomData, geomData->triMeshAttributeSet, &vertexArray);
	if (qd3dStatus != kQ3Success)
		return(kQ3Success);

	ir_geom_trimesh_build_triangles(&vertexArray);
	ir_geom_trimesh_build_vertex_colours(&vertexArray, kQ3True);
	ir_geom_trimesh_build_vertex_normals(&vertexArray);
	ir_geom_trimesh_build_vertex_uvs(&vertexArray);
	ir_geom_trimesh_build_transparent(&vertexArray);
	ir_geom_trimesh_build_indices(&vertexArray);



	// If we have anything to render, do it
	if (vertexArray.numIndices != 0)
		ir_geom_trimesh_render(instanceData, &vertexArray);



	// Clean up
	IRRenderer_Texture_Postamble(theView, instanceData, hadAttributeTexture, vertexArray.vertexUVs != NULL);

	return(kQ3Success);
}

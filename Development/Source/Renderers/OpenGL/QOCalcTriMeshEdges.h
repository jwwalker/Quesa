#pragma once
/*  NAME:
        QOCalcTriMeshEdges.h

    DESCRIPTION:
        Header file for QOCalcTriMeshEdges.cpp.

    COPYRIGHT:
        Copyright (c) 2007-2008, Quesa Developers. All rights reserved.

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
#ifndef QOCALC_TRIMESH_EDGES_H
#define QOCALC_TRIMESH_EDGES_H

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include <vector>

//=============================================================================
//      Data types
//-----------------------------------------------------------------------------

/*!
	@struct		TQ3TriangleEdges
	@abstract	Structure that identifies the edges belonging to a particular
				triangle, using indices into a separate array of edges.
	@discussion	We require that edge 0 joints vertices 0 and 1, edge 1 joins
				vertices 1 and 2, and edge 2 joins vertices 2 and 0, but we do
				not make any assumption about the order of the vertices in an
				edge.
*/
struct TQ3TriangleEdges
{
	TQ3Uns32		edgeIndices[3];
};

/*!
	@struct		TQ3EdgeEnds
	@abstract	Structure holding the indices of the end points of an edge, in
				no particular order.
	@discussion	This can be regarded as a simplification of the
				TQ3TriMeshEdgeData structure.
*/
struct TQ3EdgeEnds
{
    TQ3Uns32		pointIndices[2];
};

/*!
	@struct		TQ3EdgeVec
	@abstract	Array of edges.
*/
typedef std::vector< TQ3EdgeEnds >		TQ3EdgeVec;

/*!
	@struct		TQ3TriangleToEdgeVec
	@abstract	Array of triangle edge structures.
*/
typedef std::vector< TQ3TriangleEdges >		TQ3TriangleToEdgeVec;


//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------

/*!
	@function	QOCalcTriMeshEdges
	@abstract	Compute edges and their ownership by faces for a TriMesh.
	@discussion	Note that we cannot in general assume that no more than 2
				triangles own a given edge, we do know that a triangle has
				exactly 3 edges.  This is why we can map from faces to edges
				but not the other way around.
	@param		inData		TriMesh data.  Only the triangles and numTriangles
							fields are used.
	@param		outEdges			Receives array of edges.
	@param		outFacesToEdges		Receives array mapping faces to edges.
*/
void QOCalcTriMeshEdges( 	const TQ3TriMeshData& inData,
							TQ3EdgeVec& outEdges,
							TQ3TriangleToEdgeVec& outFacesToEdges );


/*!
	@function	QOGetCachedTriMeshEdges
	@abstract	Get TriMesh edges cached in a property.
	@discussion	If the cached data is present and not stale, it is simply
				copied to the output.  Otherwise, it is computed using
				E3CalcTriMeshEdges and cached.
	@param		inGeom				A TriMesh object.
	@param		ioScratchBuffer		A buffer for temporary use.
	@param		outEdges			Receives array of edges.
	@param		outFacesToEdges		Receives array mapping faces to edges.
*/
void QOGetCachedTriMeshEdges( TQ3GeometryObject inGeom,
							std::vector<char>& ioScratchBuffer,
							TQ3EdgeVec& outEdges,
							TQ3TriangleToEdgeVec& outFacesToEdges );

#endif

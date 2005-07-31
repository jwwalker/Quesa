#pragma once


//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------


/*!
	@function	E3TriMesh_OptimizeData
	
	@abstract	Modify TriMesh data, if needed, for efficient use by the
				interactive renderer.
	
	@discussion	This operation modifies TriMesh data so that:
				<ol>
					<li>Face normals exist.</li>
					<li>Vertex normals exist.</li>
					<li>If Face colors exist, then vertex colors exist.</li>
				</ol>
				
				If face normals do not already exist, they will be computed
				by cross products of edges.  The orientation will be assumed
				to be counterclockwise.
				
				If vertex normals do not already exist, they will be derived
				from face normals.  When a vertex belongs to faces with
				different normals, the vertex will be duplicated.
				
				If a color attribute (diffuse, transparent, or specular) exists
				on faces but not vertices, it will be converted to a vertex
				attribute, duplicating vertices when needed.
				
				If no optimization is needed, outDidChange will return kQ3False
				and outData will be cleared to zero.  If optimization was
				performed, indicated by outDidChange being kQ3True, then you
				are responsible for calling Q3TriMesh_EmptyData on the outData
				structure when you are done with it.
	
	@param		inData			TriMesh data.
	@param		outData			Receives new TriMesh data, if outDidChange is true.
	@param		outDidChange	Receives a flag indicating whether new data
								was created.
	@result		Success or failure of the operation.
*/
TQ3Status E3TriMesh_OptimizeData( const TQ3TriMeshData& inData,
								TQ3TriMeshData& outData,
								TQ3Boolean& outDidChange );


/*!
	@function	E3TriMesh_Optimize
	
	@abstract	Modify a TriMesh, if needed, for efficient use by the
				interactive renderer.
	
	@discussion	See discussion of E3TriMesh_OptimizeData for the optimizations
				that are performed.  If no optimizations are needed, NULL
				is returned.
	
	@param		inTriMesh		A TriMesh geometry.
	@result		A TriMesh or NULL.
*/
TQ3GeometryObject E3TriMesh_Optimize( TQ3GeometryObject inTriMesh );

//
//  SynthesizeTriMeshVertexNormals.cpp
//  FilamentViewer
//
//  Created by James Walker on 6/3/21.
//
/* ___________________________________________________________________________
   COPYRIGHT:
        Copyright (c) 2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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

#include "SynthesizeTriMeshVertexNormals.h"

#include "FindTriMeshVertexData.h"

#include <Quesa/QuesaGeometry.h>
#include <Quesa/QuesaMathOperators.hpp>

#include <algorithm>
#include <vector>
#include <assert.h>


///MARK:-
/*!
	@function	SynthesizeTriMeshVertexNormals
	
	@abstract	Given a TriMesh which presumably does not already have vertex normals, create a
				set of vertex normals for it by computing weighted averages of face normals.
	
	@param		inTMData	TriMesh data.
	@param		outNormals	Receives synthesized vertex normals.
*/
void	SynthesizeTriMeshVertexNormals( const TQ3TriMeshData& inTMData,
										std::vector<TQ3Vector3D>& outNormals )
{
	// Initialize the normals to be zero
	outNormals.resize( inTMData.numPoints, TQ3Vector3D{ 0.0f, 0.0f, 0.0f } );
	
	// For each face, add the cross product of the edges to each of the 3
	// vertex normals.  Note that these are not unit vectors, so we will be
	// making a weighted average, with bigger triangles having more weight.
	for (TQ3Uns32 tri = 0; tri < inTMData.numTriangles; ++tri)
	{
		const TQ3TriMeshTriangleData&	triData( inTMData.triangles[tri] );
		TQ3Vector3D prod;
		Q3FastPoint3D_CrossProductTri( &inTMData.points[ triData.pointIndices[0] ],
										&inTMData.points[ triData.pointIndices[1] ],
										&inTMData.points[ triData.pointIndices[2] ],
										&prod );
		outNormals[ triData.pointIndices[0] ] += prod;
		outNormals[ triData.pointIndices[1] ] += prod;
		outNormals[ triData.pointIndices[2] ] += prod;
	}
	
	// Compute final values of normals
	for (TQ3Uns32 ptIndex = 0; ptIndex < inTMData.numPoints; ++ptIndex)
	{
		float	sumLen = Q3Length3D( outNormals[ptIndex] );
		if (sumLen < kQ3RealZero)
		{
			constexpr TQ3Vector3D	anyNormal = { 1.0f, 0.0f, 0.0f };
			outNormals[ ptIndex ] = anyNormal;
		}
		else
		{
			outNormals[ptIndex] *= 1.0f/sumLen;
		}
	}
}

/*!
	@function	CopyOrSynthesizeVertexNormals
	
	@abstract	If TriMesh data contains vertex normal attributes, copy them to a vector, otherwise
				synthesize them in a simple way.
	
	@param	inTMData		The TriMesh data.
	@param	outNormals		Receives synthesized vertex normals.
*/
void	CopyOrSynthesizeVertexNormals(
						const TQ3TriMeshData& inTMData,
						std::unique_ptr< std::vector<TQ3Vector3D> >& outNormals )
{
	const TQ3Vector3D* normals = reinterpret_cast<const TQ3Vector3D*>(
		FindTriMeshVertexData( &inTMData, kQ3AttributeTypeNormal ) );
	
	// If we found them, copy them.
	if (normals != nullptr)
	{
		outNormals.reset( new std::vector<TQ3Vector3D>( &normals[0],
			&normals[inTMData.numPoints] ) );
	}
	else
	{
		SynthesizeTriMeshVertexNormals( inTMData, *outNormals );
	}
}

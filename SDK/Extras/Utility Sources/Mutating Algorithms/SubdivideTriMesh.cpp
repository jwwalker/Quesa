/*  NAME:
        SubdivideTriMesh.cpp

    DESCRIPTION:
        Quesa utility header.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2020, Quesa Developers. All rights reserved.

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

#include "SubdivideTriMesh.h"

#include "FindTriMeshFaceData.h"
#include "FindTriMeshVertexData.h"

#include <Quesa/QuesaGeometry.h>
#include <Quesa/QuesaSet.h>
#include <Quesa/QuesaMathOperators.hpp>

#include <vector>
#include <map>
#include <utility>

namespace
{
	typedef size_t VertIndex;
	
	struct Vertex
	{
		VertIndex	x;
		VertIndex	y;
		VertIndex	z;
		VertIndex	a;
		VertIndex	b;
		VertIndex	c;
		
		void		Swap12();
		void		Swap13();
		void		Swap23();
		void		Standardize();
		bool		IsStandard() const;
		bool		operator<( const Vertex& B ) const;
	};
	
	struct Edge
	{
		VertIndex	first;
		VertIndex	second;
	};
}

bool Vertex::operator<( const Vertex& B ) const
{
	return (x < B.x) ||
		(
			(x == B.x) &&
			(
				(y < B.y) ||
				(
					(y == B.y) &&
					(
						(z < B.z) ||
						(
							(z == B.z) &&
							(
								(a < B.a) ||
								(
									(a == B.a) &&
									(
										(b < B.b) ||
										(
											(b == B.b) &&
											(
												c < B.c
											)
										)
									)
								)
							)
						)
					)
				)
			)
		);
}

void	Vertex::Swap12()
{
	std::swap( x, y );
	std::swap( a, b );
}

void	Vertex::Swap13()
{
	std::swap( x, z );
	std::swap( a, c );
}

void	Vertex::Swap23()
{
	std::swap( y, z );
	std::swap( b, c );
}

void	Vertex::Standardize()
{
	// Among a, b, c, put zeros toward the right.
	// Possible contrary forms: (0, *, *), (*, 0, *), (0, 0, *), (0, *, 0)
	if ( (b == 0) && (c != 0) )
	{
		Swap23();
	}
	// At this point, possible contrary forms are (0, *, *), (0, *, 0)
	if ( (a == 0) && (b != 0) )
	{
		Swap12();
	}
	// At this point, possible contrary forms are (*, 0, *)
	if ( (b == 0) && (c != 0) )
	{
		Swap23();
	}
	// In the case of the interior of an edge, which looks like
	// (x, y, 0, a, b, 0), ensure that x < y.
	if ( (y < x) && (a != 0) && (b != 0) && (c == 0) )
	{
		Swap12();
	}
	if (c == 0)
	{
		z = 0;
	}
	if (b == 0)
	{
		y = 0;
	}
}

bool	Vertex::IsStandard() const
{
	bool isGood = true;
	
	// Check barycentric zeros toward the right
	if ( ((a == 0) && (b != 0)) ||
		((a == 0) && (c != 0)) ||
		((b == 0) && (c != 0)) )
	{
		isGood = false;
	}
	
	// Check edge interiors have x < y
	if ( (a != 0) && (b != 0) && (c == 0) && (x >= y) )
	{
		isGood = false;
	}
	
	// Check that the vertex index corresponding to a zero
	// barycentric coordinate is 0.
	if ( (b == 0) && (y != 0) )
	{
		isGood = false;
	}
	if ( (c == 0) && (z != 0) )
	{
		isGood = false;
	}
	
	return isGood;
}

static TQ3TriMeshTriangleData MakeFace( VertIndex x, VertIndex y, VertIndex z )
{
	TQ3TriMeshTriangleData face = { { static_cast<TQ3Uns32>(x),
		static_cast<TQ3Uns32>(y), static_cast<TQ3Uns32>(z) } };
	return face;
}

static bool operator<( const TQ3TriMeshTriangleData& A, const TQ3TriMeshTriangleData& B )
{
	return (A.pointIndices[0] < B.pointIndices[0]) ||
		(
			(A.pointIndices[0] == B.pointIndices[0]) &&
			(
				(A.pointIndices[1] < B.pointIndices[1]) ||
				(
					(A.pointIndices[1] == B.pointIndices[1]) &&
					(A.pointIndices[2] < B.pointIndices[2])
				)
			)
		);
}

/*!
	@function	SubdivideTriMesh
	
	@abstract	Subdivide each triangle of a TriMesh by drawing a certain number of splitting lines
				parallel to each side.
	
	@discussion	Although barycentric coordinates can be helpful in understanding this subdivision,
				this method is NOT what is known as "barycentric subdivision".  Whereas barycentric
				subdivision produces triangles more acute than the original triangle, this method
				produces triangles similar (in the technical sense) to the original.
				
				Each original triangle will be split into (inSplits + 1)*(inSplits + 1) triangles.
				
				Vertex normal vectors, vertex texture coordinates, and face normal vectors are
				maintained appropriately.  Currently no other per-vertex or per-face attributes are
				handled.
	@param		inTriMesh		A TriMesh object.
	@param		inSplits		Number of splitting lines parallel to each edge of each triangle.
								Must be at least 1.
	@result		A new subdivided TriMesh.
*/
TQ3GeometryObject SubdivideTriMesh( TQ3GeometryObject inTriMesh,
									TQ3Uns32 inSplits )
{
	TQ3GeometryObject result = nullptr;
	// Test for bad parameters
	if ( (inTriMesh == nullptr) || (inSplits < 1) ||
		(! Q3Object_IsType( inTriMesh, kQ3GeometryTypeTriMesh )) )
	{
		return result;
	}
	
	const VertIndex n = inSplits;
	TQ3TriMeshData* oldData;
	Q3TriMesh_LockData( inTriMesh, kQ3True, &oldData );
	
	/*
		Vertices of the new TriMesh will be of 3 types:  Original vertices,
		vertices in the interiors of original edges, and vertices in the
		interiors of original triangles.
	
		Vertices in the interiors of original triangles can be written in
		barycentric coordinates in the form
		
		a/(n+1) * points[x] + b/(n+1) * points[y] + c/(n+1) * points[z]
		
		where x, y, and z are indices of original vertices and a, b, c are
		positive integers that add up to n+1.  We can describe such a vertex
		by a Vertex structure containing the numbers (x, y, z, a, b, c).
		Similarly, a vertex in the interior of an original edge can be
		represented by a Vertex of the form (x, y, 0, a, b, 0), and an
		original vertex can be represented by (x, 0, 0, 1, 0, 0).
		
		We will need to go back and forth between the Vertex and simple
		indices, so we will begin by constructing maps in both directions.
	*/
	std::vector< Vertex > vertexOfIndex;
	std::map< Vertex, VertIndex >	indexOfVertex;
	vertexOfIndex.reserve( (n+2)*(n+3)/2 * oldData->numTriangles );
	// Start with original vertices.
	vertexOfIndex.resize( oldData->numPoints );
	VertIndex index;
	Vertex vert = { 0, 0, 0, n+1, 0, 0 };
	for (index = 0; index < oldData->numPoints; ++index)
	{
		vert.x = index;
		vertexOfIndex[ index ] = vert;
		indexOfVertex[ vert ] = index;
	}
	// For new vertices, we traverse faces.
	TQ3Uns32 faceIndex;
	for (faceIndex = 0; faceIndex < oldData->numTriangles; ++faceIndex)
	{
		const TQ3TriMeshTriangleData& face( oldData->triangles[ faceIndex ] );
		// vertices interior to the face.
		vert.x = face.pointIndices[0];
		vert.y = face.pointIndices[1];
		vert.z = face.pointIndices[2];
		for (vert.a = 1; vert.a < n; ++vert.a)
		{
			for (vert.b = 1; vert.b <= n - vert.a; ++vert.b)
			{
				vert.c = n + 1 - vert.a - vert.b;
				index = vertexOfIndex.size();
				vertexOfIndex.push_back( vert );
				indexOfVertex[ vert ] = index;
			}
		}
		// Vertices interior to edges of the face.  Here we must be more
		// careful, since edges may be shared among faces.  We will require
		// x < y to maintain uniqueness.
		vert.z = 0;
		vert.c = 0;
		Edge	edgeEnds[3] =
		{
			{ face.pointIndices[0], face.pointIndices[1] },
			{ face.pointIndices[0], face.pointIndices[2] },
			{ face.pointIndices[1], face.pointIndices[2] },
		};
		for (int whichEdge = 0; whichEdge < 3; ++whichEdge)
		{
			vert.x = edgeEnds[ whichEdge ].first;
			vert.y = edgeEnds[ whichEdge ].second;
			if (vert.y < vert.x)
			{
				std::swap( vert.x, vert.y );
			}
			for (vert.a = 1; vert.a <= n; ++vert.a)
			{
				vert.b = n + 1 - vert.a;
				if (indexOfVertex.count( vert ) == 0)
				{
					index = vertexOfIndex.size();
					vertexOfIndex.push_back( vert );
					indexOfVertex[ vert ] = index;
				}
			}
		}
	}
	const VertIndex kNewVertCount = vertexOfIndex.size();
	
	// Compute locations of the new points.
	std::vector<TQ3Point3D> newPoints( kNewVertCount );
	float denom = n + 1.0f;
	for (index = 0; index < kNewVertCount; ++index)
	{
		Vertex avert( vertexOfIndex[ index ] );
		newPoints[index] =
			(avert.a / denom) * oldData->points[ avert.x ] +
			(avert.b / denom) * oldData->points[ avert.y ] +
			(avert.c / denom) * oldData->points[ avert.z ];
	}
	
	// Next, construct subdivided faces of each face.
	// While we're at it, map from new triangles back to original face indices.
	std::vector<TQ3TriMeshTriangleData>	newFaces;
	newFaces.reserve( oldData->numTriangles * (n+1) * (n+1) );
	std::map< TQ3TriMeshTriangleData, TQ3Uns32 > faceToIndex;
	TQ3TriMeshTriangleData newFace;
	for (faceIndex = 0; faceIndex < oldData->numTriangles; ++faceIndex)
	{
		const TQ3TriMeshTriangleData& face( oldData->triangles[ faceIndex ] );
		vert.x = face.pointIndices[0];
		vert.y = face.pointIndices[1];
		vert.z = face.pointIndices[2];
		for (VertIndex a = 1; a <= n+1; ++a)
		{
			for (VertIndex b = 0; b <= n+1 - a; ++b)
			{
				VertIndex c = n + 1 - a - b;
				Vertex v0 = { vert.x, vert.y, vert.z, a, b, c };
				Vertex v1 = { vert.x, vert.y, vert.z, a-1, b+1, c };
				Vertex v2 = { vert.x, vert.y, vert.z, a-1, b, c+1 };
				v0.Standardize();
				v1.Standardize();
				v2.Standardize();
				newFace = MakeFace( indexOfVertex[ v0 ],
					indexOfVertex[ v1 ], indexOfVertex[ v2 ] );
				newFaces.push_back( newFace );
				faceToIndex[ newFace ] = faceIndex;
				if (b != 0)
				{
					Vertex w1 = { vert.x, vert.y, vert.z, a-1, b, c+1 };
					Vertex w2 = { vert.x, vert.y, vert.z, a, b-1, c+1 };
					w1.Standardize();
					w2.Standardize();
					newFace = MakeFace( indexOfVertex[ v0 ],
						indexOfVertex[ w1 ], indexOfVertex[ w2 ] );
					newFaces.push_back( newFace );
					faceToIndex[ newFace ] = faceIndex;
				}
			}
		}
	}
	
	const TQ3Uns32 kNewFaceCount = (const TQ3Uns32) newFaces.size();
	
	// start on vertex attributes
	std::vector<TQ3TriMeshAttributeData> vertAtts;
	
	// Maybe vertex normals
	const TQ3Vector3D* oldVertNorms = (const TQ3Vector3D*) FindTriMeshVertexData(
		oldData, kQ3AttributeTypeNormal );
	TQ3Vector3D* newVertNorms = nullptr;
	std::vector<TQ3Vector3D> vertNorms;
	if (oldVertNorms != nullptr)
	{
		vertNorms.resize( kNewVertCount );
		newVertNorms = &vertNorms[0];
		for (index = 0; index < kNewVertCount; ++index)
		{
			Vertex avert = vertexOfIndex[ index ];
			newVertNorms[index] = Q3Normalize3D(
				(avert.a / denom) * oldVertNorms[ avert.x ] +
				(avert.b / denom) * oldVertNorms[ avert.y ] +
				(avert.c / denom) * oldVertNorms[ avert.z ] );
		}
		TQ3TriMeshAttributeData att = {
			kQ3AttributeTypeNormal,
			newVertNorms,
			nullptr
		};
		vertAtts.push_back( att );
	}
	
	// Maybe vertex UVs
	TQ3Param2D* newUVs = nullptr;
	std::vector<TQ3Param2D> vertUVs;
	const TQ3Param2D* oldUVs = (const TQ3Param2D*) FindTriMeshVertexData(
		oldData, kQ3AttributeTypeSurfaceUV );
	if (oldUVs == nullptr)
	{
		oldUVs = (const TQ3Param2D*) FindTriMeshVertexData(
			oldData, kQ3AttributeTypeShadingUV );
	}
	if (oldUVs != nullptr)
	{
		vertUVs.resize( kNewVertCount );
		newUVs = &vertUVs[0];
		for (index = 0; index < kNewVertCount; ++index)
		{
			Vertex avert = vertexOfIndex[ index ];
			newUVs[index] =
				(avert.a / denom) * oldUVs[ avert.x ] +
				(avert.b / denom) * oldUVs[ avert.y ] +
				(avert.c / denom) * oldUVs[ avert.z ];
		}
		TQ3TriMeshAttributeData att = {
			kQ3AttributeTypeSurfaceUV,
			newUVs,
			nullptr
		};
		vertAtts.push_back( att );
	}
	
	// Maybe face normals
	TQ3TriMeshAttributeData faceNormAtt = {
		kQ3AttributeTypeNormal, nullptr, nullptr
	};
	std::vector<TQ3Vector3D> faceNorms;
	const TQ3Vector3D* oldFaceNorms = (const TQ3Vector3D*) FindTriMeshFaceData(
		oldData, kQ3AttributeTypeNormal );
	if (oldFaceNorms != nullptr)
	{
		faceNorms.resize( kNewFaceCount );
		faceNormAtt.data = &faceNorms[0];
		for (faceIndex = 0; faceIndex < kNewFaceCount; ++faceIndex)
		{
			const TQ3TriMeshTriangleData& face( newFaces[ faceIndex ] );
			TQ3Uns32 oldFaceIndex = faceToIndex[ face ];
			faceNorms[ faceIndex ] = oldFaceNorms[ oldFaceIndex ];
		}
	}
	
	// Build new TriMesh data
	TQ3TriMeshData tmData = {
		oldData->triMeshAttributeSet,
		kNewFaceCount,
		&newFaces[0],
		(oldFaceNorms != nullptr)? 1U : 0U,
		&faceNormAtt,
		0,
		nullptr,
		0,
		nullptr,
		static_cast<TQ3Uns32>(kNewVertCount),
		&newPoints[0],
		static_cast<TQ3Uns32>(vertAtts.size()),
		vertAtts.empty()? nullptr : &vertAtts[0],
		oldData->bBox
	};
	
	Q3TriMesh_UnlockData( inTriMesh );
	
	result = Q3TriMesh_New( &tmData );
	
	return result;
}

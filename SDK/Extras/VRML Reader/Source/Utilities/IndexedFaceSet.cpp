/*  NAME:
        IndexedFaceSet.cpp

    DESCRIPTION:
        Code to handle IndexedFaceSet nodes in both VRML 1 and 2.

    COPYRIGHT:
        Copyright (c) 2005, Quesa Developers. All rights reserved.

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
#include "IndexedFaceSet.h"

#include "SignedAngleBetweenVectors.h"

#if __MACH__
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/QuesaMath.h>
	#include <Quesa/QuesaStyle.h>
	#include <Quesa/QuesaCustomElements.h>
#else
	#include <QuesaGroup.h>
	#include <QuesaMath.h>
	#include <QuesaStyle.h>
	#include <QuesaCustomElements.h>
#endif

#include <algorithm>
#include <cmath>

namespace
{
	const float		kDegenerateLengthSquared	= 1.0e-13f;
	
}

/*!
	@function				ReverseFaceOrientations
	@abstract				Reverse the order of vertices in each face.
							Call this if the faces are initially
							clockwise.
*/
void	CIndexedFaceSet::ReverseFaceOrientations()
{
	for (std::vector<SFace>::iterator i = mFaces.begin(); i != mFaces.end(); ++i)
	{
		std::vector<VertIndex>&	faceVerts( i->mVertices );
		std::reverse( faceVerts.begin(), faceVerts.end() );
	}
}


/*!
	@function				CalcFaceNormals
	@abstract				If face normals were not provided, we must
							compute them by edge cross products.
							As a side effect, we also find non-convex faces.
*/
void	CIndexedFaceSet::CalcFaceNormals()
{
	if (mFaceNormals.empty())
	{
		std::vector<TQ3Vector3D>	edges;
		TQ3Vector3D	theNormal;
		bool	hasDegenerateFaces = false;
		
		for (std::vector<SFace>::iterator i = mFaces.begin(); i != mFaces.end(); ++i)
		{
			std::vector<VertIndex>&	faceVerts( i->mVertices );
			const VertIndex	faceSize = faceVerts.size();
			if (faceSize == 3)
			{
				Q3FastPoint3D_CrossProductTri(
					&mPositions[ mVertices[ faceVerts[0] ].mPosition ],
					&mPositions[ mVertices[ faceVerts[1] ].mPosition ],
					&mPositions[ mVertices[ faceVerts[2] ].mPosition ],
					&theNormal );
				float	lenSq = Q3FastVector3D_LengthSquared( &theNormal );
				// If the face is degenerate, leave its normal at kNoIndex.
				if (lenSq > kDegenerateLengthSquared)
				{
					float	len = sqrt( lenSq );
					Q3FastVector3D_Scale( &theNormal, 1.0f/len, &theNormal );
					i->mNormal = mFaceNormals.size();
					mFaceNormals.push_back( theNormal );
				}
				else
				{
					hasDegenerateFaces = true;
				}
			}
			else if (faceSize > 3)
			{
				// Even assuming that each face is convex, it is possible that if
				// we choose two edges at random, their cross product may be zero.
				// So, I will find the longest of all edge cross products.
				
				// Compute edges.
				edges.clear();
				TQ3Vector3D	edgeVec;
				VertIndex	j, k;
				for (j = 0; j < faceSize; ++j)
				{
					Q3FastPoint3D_Subtract(
						&mPositions[ mVertices[ faceVerts[ (j + 1) % faceSize ] ].mPosition ],
						&mPositions[ mVertices[ faceVerts[ j ] ].mPosition ],
						&edgeVec );
					edges.push_back( edgeVec );
				}
			
				// Find longest cross product.
				TQ3Vector3D	bestCross, oneCross;
				float		bestCrossLenSq = -1.0f;
				for (j = 0; j < faceSize - 1; ++j)
				{
					for (k = j + 1; k < faceSize; ++k)
					{
						Q3FastVector3D_Cross( &edges[j], &edges[k], &oneCross );
						float	oneLenSq = Q3FastVector3D_LengthSquared( &oneCross );
						if (oneLenSq > bestCrossLenSq)
						{
							bestCrossLenSq = oneLenSq;
							bestCross = oneCross;
						}
					}
				}
				
				if (bestCrossLenSq > kDegenerateLengthSquared)
				{
					float	bestLen = sqrt( bestCrossLenSq );

					// We do not know whether the cross product we found was
					// outward or inward facing.  If we sum the angles between
					// adjacent edges relative to the cross product, we
					// should get a positive number if the cross product is
					// the right sign.
					
					// If the angles are not all the same sign, we know that the
					// face is not convex.
					bool	isSomeAnglePositive = false;
					bool	isSomeAngleNegative = false;
					
					float	angleSum = 0.0f;
					for (j = 0; j < faceSize; ++j)
					{
						float	oneAngle = SignedAngleBetweenVectors(
							edges[ j ],
							edges[ (j + 1) % faceSize ],
							bestCross );
						angleSum += oneAngle;
						if (oneAngle > FLT_EPSILON)
						{
							isSomeAnglePositive = true;
						}
						else if (oneAngle < -FLT_EPSILON)
						{
							isSomeAngleNegative = true;
						}
					}

					if ( angleSum < 0.0f )
					{
						bestLen = -bestLen;
					}
					Q3FastVector3D_Scale( &bestCross, 1.0f/bestLen, &theNormal );
					
					i->mNormal = mFaceNormals.size();
					mFaceNormals.push_back( theNormal );

					if (isSomeAnglePositive and isSomeAngleNegative)
					{
						i->mIsConvex = false;
					}
				}
				else
				{
					hasDegenerateFaces = true;
				}
			}
		}
		
		if (hasDegenerateFaces)
		{
			RemoveDegenerateFaces();
		}
	}
}

/*!
	@function				RemoveDegenerateFaces
	@abstract				Remove degenerate faces (those for which we
							were not able to compute a sensible face
							normal) and their vertices.
*/
void	CIndexedFaceSet::RemoveDegenerateFaces()
{
	// Since faces have vertex indices and a vertex has a face index,
	// we must be careful to fix up indices when we remove some faces
	// and vertices.
	const FaceIndex	kNumOldFaces = GetFaces().size();
	std::vector<FaceIndex>	oldToNewFaceIndex( kNumOldFaces );
	std::vector<SFace>	newFaces;
	newFaces.reserve( kNumOldFaces );
	FaceIndex	i;
	for (i = 0; i < kNumOldFaces; ++i)
	{
		if (GetFaces()[i].mNormal == kNoIndex)
		{
			oldToNewFaceIndex[i] = kNoIndex;
		}
		else
		{
			oldToNewFaceIndex[i] = newFaces.size();
			newFaces.push_back( GetFaces()[i] );
		}
	}
	
	const VertIndex	kNumOldVertices = GetVertices().size();
	std::vector<VertIndex>	oldToNewVertIndex( kNumOldVertices );
	std::vector<SVertex>	newVertices;
	newVertices.reserve( kNumOldVertices );
	VertIndex	v;
	for (v = 0; v < kNumOldVertices; ++v)
	{
		SVertex&	oldVert( GetVertices()[v] );
		if (oldToNewFaceIndex[ oldVert.mFace ] == kNoIndex)
		{
			oldToNewVertIndex[v] = kNoIndex;
		}
		else
		{
			oldToNewVertIndex[v] = newVertices.size();
			oldVert.mFace = oldToNewFaceIndex[ oldVert.mFace ];
			newVertices.push_back( oldVert );
		}
	}
	
	const FaceIndex	kNumNewFaces = newFaces.size();
	
	for (i = 0; i < kNumNewFaces; ++i)
	{
		std::vector<VertIndex>&	vertsOnFace( newFaces[i].mVertices );
		const VertIndex kNumVertsOnFace = vertsOnFace.size();
		for (v = 0; v < kNumVertsOnFace; ++v)
		{
			vertsOnFace[v] = oldToNewVertIndex[ vertsOnFace[v] ];
		}
	}
	
	GetFaces().swap( newFaces );
	GetVertices().swap( newVertices );
}


/*!
	@function				FindVerticesAtPosition
	@abstract				Group vertices at a given position.
	@discussion				We compute the vectors mLastVertAtPosition
							and mPrevVertAtSamePosition, which together
							define a linked list of vertices at a given
							position.  We skip vertices belonging to
							degenerate faces.
*/
void	CIndexedFaceSet::FindVerticesAtPosition()
{
	mLastVertAtPosition.resize( mPositions.size(), kNoIndex );
	mPrevVertAtSamePosition.resize( mVertices.size(), kNoIndex );
	
	for (VertIndex i = 0; i < mVertices.size(); ++i)
	{
		const SVertex&	theVertex( mVertices[i] );
		
		if (mFaces[ theVertex.mFace ].mNormal != kNoIndex)
		{
			mPrevVertAtSamePosition[i] = mLastVertAtPosition[ theVertex.mPosition ];
			mLastVertAtPosition[ theVertex.mPosition ] = i;
		}
	}
}

/*!
	@function				CalcVertexNormals
	@abstract				If vertex normals were not provided,
							calculate them, taking into account a given
							crease angle.
*/
void	CIndexedFaceSet::CalcVertexNormals( float inCreaseCosine )
{
	if (mVertexNormals.empty())
	{
		// Compute the vectors mFirstVertWithNormal and mNextVertSharingNormal
		// that group vertices that will share a normal vector.
		// In the process we will also set the mNormal index for each vertex,
		// except vertices belonging to degenerate faces.
		mNextVertSharingNormal.resize( mVertices.size(), kNoIndex );
		
		for (PositionIndex p = 0; p < mPositions.size(); ++p)
		{
			for (VertIndex v = mLastVertAtPosition[p]; v != kNoIndex;
				v = mPrevVertAtSamePosition[v])
			{
				FaceNormIndex	v_faceNorm = mFaces[ mVertices[v].mFace ].mNormal;
				if (v_faceNorm == kNoIndex)
					continue;	// skip degenerate faces
				
				// If this vertex's face is at a small angle to a later face at
				// the same position, we can share a normal.
				for (VertIndex w = mLastVertAtPosition[p]; w != v;
					w = mPrevVertAtSamePosition[w])
				{
					FaceNormIndex	w_faceNorm = mFaces[ mVertices[w].mFace ].mNormal;
					
					if (w_faceNorm == kNoIndex)
						continue;
					
					float	dotProd = Q3FastVector3D_Dot(
						&mFaceNormals[ v_faceNorm ], &mFaceNormals[ w_faceNorm ] );
					
					if (dotProd > inCreaseCosine)
					{
						mVertices[ v ].mNormal = mVertices[ w ].mNormal;
						mNextVertSharingNormal[ v ] = w;
						mFirstVertWithNormal[ mVertices[ v ].mNormal ] = v;
						break;
					}
				}
				if (mVertices[ v ].mNormal == kNoIndex)
				{
					mVertices[ v ].mNormal = mFirstVertWithNormal.size();
					mFirstVertWithNormal.push_back( v );
				}
			}
		}
		
		TQ3Vector3D	vertNorm;
		for (VertNormIndex i = 0; i < mFirstVertWithNormal.size(); ++i)
		{
			TQ3Vector3D	normSum = { 0.0f, 0.0f, 0.0f };
			
			for (VertIndex j = mFirstVertWithNormal[i]; j != kNoIndex;
				j = mNextVertSharingNormal[j])
			{
				Q3FastVector3D_Add( &normSum, &mFaceNormals[ mVertices[j].mFace ],
					&normSum );
			}
			Q3FastVector3D_Normalize( &normSum, &vertNorm );
			mVertexNormals.push_back( vertNorm );
		}
	}
}

/*!
	@function				IdentifyDistinctPoints
	@abstract				Determine the points of the final TriMesh.
	@discussion				Two vertices sharing the same position may
							or may not be considered the same point,
							depending on whether they also share normal
							vectors, vertex colors, face colors, and
							texture coordinates.  We will compute the
							vectors mPointToPosition, mVertToPoint and
							mPointToVert.
*/
void	CIndexedFaceSet::IdentifyDistinctPoints()
{
	mVertToPoint.resize( mVertices.size(), kNoIndex );
	
	for (PositionIndex p = 0; p < mPositions.size(); ++p)
	{
		for (VertIndex v = mLastVertAtPosition[p]; v != kNoIndex;
			v = mPrevVertAtSamePosition[v])
		{
			const SVertex&	vertV( mVertices[v] );
			
			for (VertIndex w = mLastVertAtPosition[p]; w != v;
				w = mPrevVertAtSamePosition[w])
			{
				const SVertex&	vertW( mVertices[w] );
				
				if ( (vertV.mNormal == vertW.mNormal) and
					(vertV.mColor == vertW.mColor) and
					(vertV.mTexCoord == vertW.mTexCoord) and
					(mFaces[ vertV.mFace ].mColor == mFaces[ vertW.mFace ].mColor) )
				{
					mVertToPoint[ v ] = mVertToPoint[ w ];
					break;
				}
			}
			if (mVertToPoint[ v ] == kNoIndex)
			{
				mVertToPoint[ v ] = mPointToPosition.size();
				mPointToVert.push_back( v );
				mPointToPosition.push_back( vertV.mPosition );
			}
		}
	}
}

/*!
	@function				TriangulateFaces
	@abstract				Split each nondegenerate face into triangles.
	@param					outTriangles		Receives triangles, each
												one containing 3 point
												indices.
*/
void	CIndexedFaceSet::ComputeTriMeshFaces( TriangleVec& outTriangles ) const
{
	const FaceIndex kNumFaces = mFaces.size();
	
	for (FaceIndex i = 0; i < kNumFaces; ++i)
	{
		const SFace& theFace( mFaces[i] );
		if (theFace.mNormal != kNoIndex)
		{
			if (theFace.mIsConvex)
			{
				TriangulateConvexFace( theFace, outTriangles );
			}
			else
			{
				TriangulateNonconvexFace( theFace, outTriangles );
			}
		}
	}
}

void	CIndexedFaceSet::TriangulateConvexFace( const SFace& inFace,
									TriangleVec& outTriangles ) const
{
	for (VertIndex i = 2; i < inFace.mVertices.size(); ++i)
	{
		// Make a triangle from the instances 0, i-1, and i
		TQ3TriMeshTriangleData	aTri;
		aTri.pointIndices[0] = mVertToPoint[ inFace.mVertices[0] ];
		aTri.pointIndices[1] = mVertToPoint[ inFace.mVertices[i-1] ];
		aTri.pointIndices[2] = mVertToPoint[ inFace.mVertices[i] ];
		outTriangles.push_back( aTri );
	}
}

void	CIndexedFaceSet::TriangulateNonconvexFace( const SFace& inFace,
									TriangleVec& outTriangles ) const
{
	// We will need to have the face normal handy.
	TQ3Vector3D	faceNormal = mFaceNormals[ inFace.mNormal ];
	
	// Copy the vertex indices of the face to a vector we can play with.
	const VertIndex kNumVerts = inFace.mVertices.size();
	std::vector<VertIndex>	verts( inFace.mVertices );

	// As long as there are more than 3 vertices, we find a convex corner
	// and split off a triangle there.
	while (verts.size() > 3)
	{
		int	numVerts = verts.size();
		VertIndex i;
		for (i = 0; i < kNumVerts; ++i)
		{
			TQ3Vector3D		cornerCross;
			Q3FastPoint3D_CrossProductTri(
				&mPositions[ mVertices[ verts[i] ].mPosition ],
				&mPositions[ mVertices[ verts[ (i+1) % kNumVerts ] ].mPosition ],
				&mPositions[ mVertices[ verts[ (i+2) % kNumVerts ] ].mPosition ],
				&cornerCross );
			float	crossDot = Q3FastVector3D_Dot( &cornerCross, &faceNormal );
			if (crossDot > FLT_EPSILON)
			{
				// Oops, not good enough, we must make sure that no other
				// vertices are inside the triangle.
				bool	isSomethingInside = false;
				TQ3Ray3D	hitRay;
				TQ3Param3D	hitParam;
				hitRay.direction = faceNormal;
				for (int j = 0; j < kNumVerts; ++j)
				{
					if ( (j != i) and (j != ((i+1) % kNumVerts)) and
						(j != ((i+2) % kNumVerts)) )
					{
						hitRay.origin = mPositions[ mVertices[ verts[j] ].mPosition ];
						if (Q3Ray3D_IntersectTriangle( &hitRay,
							&mPositions[ mVertices[ verts[i] ].mPosition ],
							&mPositions[ mVertices[ verts[ (i+1) % kNumVerts ] ].mPosition ],
							&mPositions[ mVertices[ verts[ (i+2) % kNumVerts ] ].mPosition ],
							kQ3False, &hitParam ))
						{
							isSomethingInside = true;
							break;
						}
					}
				}
				
				if (not isSomethingInside)
				{
					TQ3TriMeshTriangleData	aTri = {
						mVertToPoint[ verts[i] ],
						mVertToPoint[ verts[ (i+1) % kNumVerts ] ],
						mVertToPoint[ verts[ (i+2) % kNumVerts ] ]
					};
					outTriangles.push_back( aTri );
					
					verts.erase( verts.begin() + ((i+1) % numVerts) );
					break;
				}
			}
		}
		if (i == kNumVerts)
		{
			break;	// bad, this should not happen
		}
	}
	
	if (verts.size() == 3)
	{
		TQ3TriMeshTriangleData	lastTri = {
			mVertToPoint[ verts[0] ],
			mVertToPoint[ verts[ 1 ] ],
			mVertToPoint[ verts[ 2 ] ]
		};
		outTriangles.push_back( lastTri );
	}
}

/*!
	@function				ComputeTriMeshFaceNormals
	@abstract				Build the vector of triangle normals for use
							in a TriMesh.
	@param					outNormals			Receives the normals.
*/
void	CIndexedFaceSet::ComputeTriMeshFaceNormals(
									std::vector<TQ3Vector3D>& outNormals ) const
{
	outNormals.reserve( mVertices.size() - 2 * mFaces.size() );
	
	for (std::vector<SFace>::const_iterator i = mFaces.begin(); i != mFaces.end(); ++i)
	{
		if (i->mNormal != kNoIndex)
		{
			TQ3Vector3D	faceNorm( mFaceNormals[ i->mNormal ] );
			
			for (VertIndex j = 0; j < i->mVertices.size() - 2; ++j)
			{
				outNormals.push_back( faceNorm );
			}
		}
	}
}

/*!
	@function				ComputeTriMeshFaceColors
	@abstract				Build the vector of triangle colors, if any, for use
							in a TriMesh.
	@param					outNormals			Receives the normals.
*/
void	CIndexedFaceSet::ComputeTriMeshFaceColors(
									std::vector<TQ3ColorRGB>& outColors ) const
{
	if ( (not mColors.empty()) and (mFaces[0].mColor != kNoIndex) )
	{
		outColors.reserve( mVertices.size() - 2 * mFaces.size() );
		
		for (std::vector<SFace>::const_iterator i = mFaces.begin(); i != mFaces.end(); ++i)
		{
			if (i->mNormal != kNoIndex)
			{
				TQ3ColorRGB	faceColor( mColors[ i->mColor ] );
			
				for (VertIndex j = 0; j < i->mVertices.size() - 2; ++j)
				{
					outColors.push_back( faceColor );
				}
			}
		}
	}
}

/*!
	@function				ComputeTriMeshPositions
	@abstract				Build the vector of points for a TriMesh.
	@param					outPoints			Receives the points.
*/
void	CIndexedFaceSet::ComputeTriMeshPositions(
									std::vector<TQ3Point3D>& outPoints ) const
{
	const PointIndex	kNumPoints = mPointToPosition.size();
	outPoints.resize( kNumPoints );
	
	for (PointIndex i = 0; i < kNumPoints; ++i)
	{
		outPoints[i] = mPositions[ mPointToPosition[i] ];
	}
}

/*!
	@function				ComputeTriMeshVertexNormals
	@abstract				Build the vector of vertex normals for a TriMesh.
	@param					outNormals			Receives the normals.
*/
void	CIndexedFaceSet::ComputeTriMeshVertexNormals(
									std::vector<TQ3Vector3D>& outNormals ) const
{
	const PointIndex	kNumPoints = mPointToVert.size();
	outNormals.resize( kNumPoints );
	
	for (PointIndex i = 0; i < kNumPoints; ++i)
	{
		outNormals[i] = mVertexNormals[ mVertices[ mPointToVert[i] ].mNormal ];
	}
}

/*!
	@function				ComputeTriMeshVertexColors
	@abstract				Build the vector of vertex colors for a TriMesh.
	@param					outColors			Receives the colors.
*/
void	CIndexedFaceSet::ComputeTriMeshVertexColors(
									std::vector<TQ3ColorRGB>& outColors ) const
{
	if ( (not mColors.empty()) and (mVertices[0].mColor != kNoIndex) )
	{
		const PointIndex	kNumPoints = mPointToVert.size();
		outColors.resize( kNumPoints );
		
		for (PointIndex i = 0; i < kNumPoints; ++i)
		{
			outColors[i] = mColors[ mVertices[ mPointToVert[i] ].mColor ];
		}
	}
}

/*!
	@function				ComputeTriMeshTexCoords
	@abstract				Build the vector of vertex texture coordinates
							for a TriMesh.
	@discussion				If no texture coordinates were provided, we
							synthesize them based on point positions and
							bounds, according to VRML conventions.
	@param					inPoints			The TriMesh points as
												computed by
												ComputeTriMeshPositions.
	@param					inBounds			The TriMesh bounding box.
	@param					outColors			Receives the texture coordinates.
*/
void	CIndexedFaceSet::ComputeTriMeshTexCoords(
									const std::vector<TQ3Point3D>& inPoints,
									const TQ3BoundingBox& inBounds,
									std::vector<TQ3Param2D>& outCoords ) const
{
	const PointIndex	kNumPoints = inPoints.size();
	outCoords.resize( kNumPoints );
	PointIndex i;

	if ( (not mTexCoords.empty()) and (mVertices[0].mTexCoord != kNoIndex) )
	{
		for (i = 0; i < kNumPoints; ++i)
		{
			outCoords[i] = mTexCoords[ mVertices[ mPointToVert[i] ].mTexCoord ];
		}
	}
	else
	{
		// Find the largest and second largest axes of the bounds
		float	extents[3] = {
			inBounds.max.x - inBounds.min.x,
			inBounds.max.y - inBounds.min.y,
			inBounds.max.z - inBounds.min.z
		};
		int	majorAxis, minorAxis;
		if (extents[0] > extents[1])
		{
			if (extents[1] > extents[2])
			{
				majorAxis = 0;
				minorAxis = 1;
			}
			else	// extents[2] >= extents[1]
			{
				if (extents[0] > extents[2])
				{
					majorAxis = 0;
					minorAxis = 2;
				}
				else	// extents[2] >= extents[0]
				{
					majorAxis = 2;
					minorAxis = 0;
				}
			}
		}
		else	// extents[1] >= extents[0]
		{
			if (extents[1] > extents[2])
			{
				majorAxis = 1;
				if (extents[0] > extents[2])
				{
					minorAxis = 0;
				}
				else
				{
					minorAxis = 2;
				}
			}
			else	// extents[2] >= extents[1]
			{
				majorAxis = 2;
				minorAxis = 1;
			}
		}
		
		const float*	minBounds = &inBounds.min.x;
		float	majorAxisMin = minBounds[majorAxis];
		float	minorAxisMin = minBounds[minorAxis];
		float	majorExtent = extents[majorAxis];
		
		for (i = 0; i < kNumPoints; ++i)
		{
			const float*	coords = &inPoints[i].x;
			outCoords[i].u = (coords[majorAxis] - majorAxisMin) / majorExtent;
			outCoords[i].v = (coords[minorAxis] - minorAxisMin) / majorExtent;
		}
	}
}


/*!
	@function				CreateTriMesh
	@abstract				Create a TriMesh object from the data in this
							object.
	@result					A Quesa TriMesh object.
*/
CQ3ObjectRef	CIndexedFaceSet::CreateTriMesh() const
{
	std::vector<TQ3Point3D>	thePoints;
	TriangleVec				theTriangles;
	std::vector<TQ3Vector3D>	faceNormals;
	std::vector<TQ3ColorRGB>	faceColors;
	std::vector<TQ3Vector3D>	vertexNormals;
	std::vector<TQ3ColorRGB>	vertexColors;
	std::vector<TQ3Param2D>		vertexUVs;
	TQ3BoundingBox	bounds;

	ComputeTriMeshFaces( theTriangles );
	ComputeTriMeshFaceNormals( faceNormals );
	ComputeTriMeshFaceColors( faceColors );
	ComputeTriMeshPositions( thePoints );
	if (thePoints.empty())
	{
		throw std::runtime_error("IndexedFaceSet with no points");
	}
	ComputeTriMeshVertexNormals( vertexNormals );
	ComputeTriMeshVertexColors( vertexColors );
	Q3BoundingBox_SetFromPoints3D( &bounds, &thePoints[0], thePoints.size(),
		sizeof(TQ3Point3D) );
	ComputeTriMeshTexCoords( thePoints, bounds, vertexUVs );
	
	// Set up vertex attributes
	TQ3TriMeshAttributeData		vertAtts[3];
	int	vertAttNum = 0;
	// Vertex normal, always present
	vertAtts[ vertAttNum ].attributeType = kQ3AttributeTypeNormal;
	vertAtts[ vertAttNum ].data = &vertexNormals[0];
	vertAtts[ vertAttNum ].attributeUseArray = NULL;
	// texture coordinates
	if (not vertexUVs.empty())
	{
		vertAttNum += 1;
		vertAtts[ vertAttNum ].attributeType = kQ3AttributeTypeSurfaceUV;
		vertAtts[ vertAttNum ].data = &vertexUVs[0];
		vertAtts[ vertAttNum ].attributeUseArray = NULL;
	}
	// vertex colors
	if (not vertexColors.empty())
	{
		vertAttNum += 1;
		vertAtts[ vertAttNum ].attributeType = kQ3AttributeTypeDiffuseColor;
		vertAtts[ vertAttNum ].data = &vertexColors[0];
		vertAtts[ vertAttNum ].attributeUseArray = NULL;
	}
	
	// Set up face attributes
	TQ3TriMeshAttributeData		faceAtts[3];
	int	faceAttNum = 0;
	// face normal, always present
	faceAtts[ faceAttNum ].attributeType = kQ3AttributeTypeNormal;
	faceAtts[ faceAttNum ].data = &faceNormals[0];
	faceAtts[ faceAttNum ].attributeUseArray = NULL;
	// face colors
	if (not faceColors.empty())
	{
		faceAttNum += 1;
		faceAtts[ faceAttNum ].attributeType = kQ3AttributeTypeDiffuseColor;
		faceAtts[ faceAttNum ].data = &faceColors[0];
		faceAtts[ faceAttNum ].attributeUseArray = NULL;
	}
	
	// Create TriMesh
	TQ3TriMeshData		triMeshData =
	{
		NULL,					// triMeshAttributeSet
		theTriangles.size(),	// numTriangles
		&theTriangles[0],		// triangles
		faceAttNum + 1,			// numTriangleAttributeTypes
		faceAtts,				// triangleAttributeTypes
		0,						// numEdges
		NULL,					// edges
		0,						// numEdgeAttributeTypes
		NULL,					// edgeAttributeTypes
		thePoints.size(),		// numPoints
		&thePoints[0],			// points
		vertAttNum + 1,			// numVertexAttributeTypes
		vertAtts,				// vertexAttributeTypes
		bounds			// bBox
	};
	CQ3ObjectRef	theTriMesh( Q3TriMesh_New( &triMeshData ) );
	return theTriMesh;
}



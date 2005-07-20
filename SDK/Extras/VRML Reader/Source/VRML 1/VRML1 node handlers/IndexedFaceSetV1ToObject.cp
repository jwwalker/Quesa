/*  NAME:
        IndexedFaceSetV1ToObject.cp

    DESCRIPTION:
        VRML 1 node handler.

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
#include "IndexedFaceSetV1ToObject.h"

#include "CVRMLReader.h"
#include "GetArrayFromNodeField.h"
#include "GetIndexedMaterial.h"
#include "GetIntVecFromField.h"
#include "PolyValue.h"
#include "SignedAngleBetweenVectors.h"
#include "SVRML1State.h"
#include "VRML_1_constants.h"
#include "VRML-reader-prefix.h"

#include <ostream>
#include <cmath>

#include <QuesaGeometry.h>
#include <QuesaGroup.h>
#include <QuesaMath.h>
#include <QuesaStyle.h>
#include <QuesaCustomElements.h>

#if TARGET_API_MAC_CARBON && DEBUG
	//#define	DEBUG_INTERNAL	1
	#define DEBUG_BREAK_ONLY	1
	#define	kComponentSignatureString	"VRML-Reader"
	
	#include <Debugging.h>
#else
	#define check(assertion)
	#define debug_string( cstring )
#endif

namespace
{
	typedef std::vector<TQ3TriMeshTriangleData>	TriangleVec;
	
	#pragma mark struct IndexedFaceMaker
	struct	IndexedFaceMaker
	{
									IndexedFaceMaker(
											PolyValue& ioNode,
											CVRMLReader& inReader );

		bool						HasEnoughData() const;
		void						FindDictinctVertices();
		void						ComputeFaceNormals();
		void						FindFaces();
		void						FindBackLinks();
		int							FindPrevSimilarPoint( int inPtInstanceIndex );
		bool						ArePointsSimilar( int inPt1, int inPt2 );
		void						ComputeVertices();
		void						ComputeTriangles();
		void						ConvexFaceToTriangles( int inFaceIndex );
		void						NonConvexFaceToTriangles( int inFaceIndex );
		void						ComputeTriMeshVertexNormals();
		void						ComputeTriMeshFaceNormals();
		void						ComputeTriMeshVertexColors();
		void						ComputeTriMeshFaceColors();
		void						ComputeTriMeshVertexUVs();
		void						ComputeTriMeshBounds();
		void						ComputeTriMeshArrays();
		CQ3ObjectRef				CreateObject();
					
		PolyValue&					mNode;
		CVRMLReader&				mReader;
		const SVRML1State&			mState;
		PolyValue::Dictionary&		mNodeDict;
		bool						mIsConvex;
		bool						mIsSolid;
		bool						mIsColorPerVertex;
		bool						mIsColorPerFace;
		bool						mIsNormalPerVertex;
		float						mCreaseAngle;
		float						mCreaseAngleCosine;
		const std::vector<TQ3Point3D>&	mPoints;
		const std::vector<TQ3Vector3D>&	mNormals;
		const std::vector<TQ3ColorRGB>&	mColors;
		const std::vector<TQ3Param2D>&	mUVs;
		PolyValue::IntVec			mInstanceToPoint;
		PolyValue::IntVec			mColorIndex;
		PolyValue::IntVec			mNormalIndex;
		PolyValue::IntVec			mUVIndex;
		PolyValue::IntVec			mVertexToPoint;
		PolyValue::IntVec			mInstanceToVertex;
		PolyValue::IntVec			mVertexToInstance;
		std::vector<TQ3Vector3D>	mComputedFaceNormals;
		PolyValue::IntVec			mFaceStarts;
		PolyValue::IntVec			mFaceSizes;
		std::vector<bool>			mIsFaceConvex;
		std::vector<bool>			mIsFaceDegenerate;
		PolyValue::IntVec			mInstanceToFace;
		PolyValue::IntVec			mPrevPointInstance;
		std::vector<TQ3Point3D>		mTriMeshPoints;
		TriangleVec					mTriMeshFaces;
		std::vector<TQ3Vector3D>	mTriMeshVertexNormals;
		std::vector<TQ3Vector3D>	mTriMeshFaceNormals;
		std::vector<TQ3ColorRGB>	mTriMeshVertexColors;
		std::vector<TQ3ColorRGB>	mTriMeshFaceColors;
		std::vector<TQ3Param2D>		mTriMeshVertexUVs;
		TQ3BoundingBox				mTriMeshBounds;
	};
}

/*!
	@function	StandardizeIndexVector
	@abstract	Make an index vector end with -1.
*/
static void StandardizeIndexVector( PolyValue::IntVec& ioVec )
{
	if ( (not ioVec.empty()) and (ioVec.back() >= 0) )
	{
		ioVec.push_back( -1 );
	}
}

using namespace std;	// for cos; cannot use std::cos with stupid BSD headers

IndexedFaceMaker::IndexedFaceMaker(
										PolyValue& ioNode,
										CVRMLReader& inReader )
	: mNode( ioNode )
	, mReader( inReader )
	, mState( mReader.GetVRML1State() )
	, mNodeDict( ioNode.GetDictionary() )
	, mIsConvex( mState.convexFaceType )
	, mIsSolid( mState.solidShapeType )
	, mIsColorPerVertex( (mState.materialBinding == eVRML1Value_PER_VERTEX) or
		(mState.materialBinding == eVRML1Value_PER_VERTEX_INDEXED) )
	, mIsColorPerFace( (mState.materialBinding == eVRML1Value_PER_PART) or
		(mState.materialBinding == eVRML1Value_PER_PART_INDEXED) or
		(mState.materialBinding == eVRML1Value_PER_FACE) or
		(mState.materialBinding == eVRML1Value_PER_FACE_INDEXED) )
	, mIsNormalPerVertex( (mState.normalBinding == eVRML1Value_PER_VERTEX) or
		(mState.normalBinding == eVRML1Value_PER_VERTEX_INDEXED) )
	, mCreaseAngle( mState.creaseAngle )
	, mCreaseAngleCosine( cos(mCreaseAngle) )
	, mPoints( mState.coordinate3 )
	, mNormals( mState.normal )
	, mColors( mState.diffuseColor )
	, mUVs( mState.textureCoordinate2 )
{
	GetIntVecFromField( mNodeDict, "coordIndex", mInstanceToPoint );
	GetIntVecFromField( mNodeDict, "materialIndex", mColorIndex );
	GetIntVecFromField( mNodeDict, "normalIndex", mNormalIndex );
	GetIntVecFromField( mNodeDict, "textureCoordIndex", mUVIndex );
	StandardizeIndexVector( mInstanceToPoint );
	StandardizeIndexVector( mColorIndex );
	StandardizeIndexVector( mNormalIndex );
	StandardizeIndexVector( mUVIndex );
}

bool	IndexedFaceMaker::HasEnoughData() const
{
	bool	hasEnough = true;
	
	if (mPoints.empty())
	{
		hasEnough = false;
		if (mReader.GetDebugStream() != NULL)
		{
			*mReader.GetDebugStream() << "No points in IndexedFaceSet." << std::endl;
		}
	}
	else if (mInstanceToPoint.empty())
	{
		hasEnough = false;
		if (mReader.GetDebugStream() != NULL)
		{
			*mReader.GetDebugStream() << "No faces in IndexedFaceSet." << std::endl;
		}
	}
	
	return hasEnough;
}

/*!
	@function	FindFaces
	@abstract	Find indices into mInstanceToPoint where faces start.
				We will ignore any face with fewer than 3 points.
				Also we compute an array that maps each position of
				mInstanceToPoint to a face number or -1.
*/
void	IndexedFaceMaker::FindFaces()
{
	const int kNumIndices = mInstanceToPoint.size();
	int	nextFaceStart = 0;
	int	i;
	for (i = 0; i < kNumIndices; ++i)
	{
		if ( (mInstanceToPoint[i] < 0) and (i - nextFaceStart > 2) )
		{
			mFaceStarts.push_back( nextFaceStart );
			mFaceSizes.push_back( i - nextFaceStart );
			nextFaceStart = i + 1;
		}
	}
	
	// The last face need not be followed by -1.
	if ( (nextFaceStart < kNumIndices) and (kNumIndices - nextFaceStart > 2) )
	{
		mFaceStarts.push_back( nextFaceStart );
		mFaceSizes.push_back( i - nextFaceStart );
	}
	
	// Compute mInstanceToFace
	mInstanceToFace.resize( mInstanceToPoint.size(), -1 );
	for (int j = 0; j < mFaceStarts.size(); ++j)
	{
		for (i = mFaceStarts[j]; i < kNumIndices; ++i)
		{
			if (mInstanceToPoint[i] < 0)
			{
				break;
			}
			mInstanceToFace[i] = j;
		}
	}
}

/*!
	@function	ComputeFaceNormals
	@abstract	Compute outward-facing face normals.
*/
void	IndexedFaceMaker::ComputeFaceNormals()
{
	const int kNumFaces = mFaceStarts.size();
	mIsFaceConvex.resize( kNumFaces, true );
	mIsFaceDegenerate.resize( kNumFaces, false );
	std::vector<TQ3Vector3D>	edges;
	
	for (int i = 0; i < kNumFaces; ++i)
	{
		int	startIndex = mFaceStarts[i];
		int	faceSize = mFaceSizes[i];
		TQ3Vector3D	theNormal;
		
		if (faceSize == 3)
		{
			Q3FastPoint3D_CrossProductTri( &mPoints[ mInstanceToPoint[ startIndex ] ],
				&mPoints[ mInstanceToPoint[ startIndex + 1 ] ],
				&mPoints[ mInstanceToPoint[ startIndex + 2 ] ],
				&theNormal );
			float	lenSq = Q3FastVector3D_LengthSquared( &theNormal );
			if (lenSq < FLT_EPSILON * 0.01f)
			{
				theNormal.x = 1.0f;
				theNormal.y = theNormal.z = 0.0f;
				mIsFaceDegenerate[ i ] = true;
			}
			else
			{
				float	len = sqrt( lenSq );
				if (mState.vertexOrdering == eVRML1VertexOrdering_CLOCKWISE)
				{
					len = -len;
				}
				else if (mState.vertexOrdering == eVRML1VertexOrdering_UNKNOWN_ORDERING)
				{
					// The only hope of figuring out what is the front face is by
					// looking at vertex normals.
					if (mIsNormalPerVertex)
					{
						TQ3Vector3D	aVertNormal;
						if (mNormalIndex.empty())	// direct vertex normals
						{
							aVertNormal = mNormals[ mInstanceToPoint[ startIndex ] ];
						}
						else
						{
							aVertNormal = mNormals[ mNormalIndex[ startIndex ] ];
						}
						if (Q3FastVector3D_Dot( &theNormal, &aVertNormal ) < 0.0f)
						{
							len = -len;
						}
					}
				}
				Q3FastVector3D_Scale( &theNormal, 1.0f/len, &theNormal );
			}
		}
		else
		{
			// Even assuming that each face is convex, it is possible that if
			// we choose two edges at random, their cross product may be zero.
			// So, I will find the longest of all edge cross products.
			
			// Compute edges.
			edges.clear();
			TQ3Vector3D	edgeVec;
			int	j, k;
			for (j = 0; j < faceSize; ++j)
			{
				Q3FastPoint3D_Subtract(
					&mPoints[ mInstanceToPoint[ startIndex + j ] ],
					&mPoints[ mInstanceToPoint[ startIndex + ((j + 1) % faceSize) ] ],
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
			if (bestCrossLenSq < FLT_EPSILON * 0.01f)
			{
				theNormal.x = 1.0f;
				theNormal.y = theNormal.z = 0.0f;
				mIsFaceDegenerate[ i ] = true;
			}
			else
			{
				float	bestLen = sqrt( bestCrossLenSq );
				// We do not know whether the cross product we found was
				// outward or inward facing.  If we sum the angles between
				// adjacent edges relative to the cross product, we
				// should get a positive number if the cross product is
				// the right sign and if mIsCcw is true.
				
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
				
				if (mState.vertexOrdering == eVRML1VertexOrdering_COUNTERCLOCKWISE)
				{
					if (angleSum < 0.0f)
					{
						bestLen = -bestLen;
					}
				}
				else if (mState.vertexOrdering == eVRML1VertexOrdering_CLOCKWISE)
				{
					if (angleSum > 0.0f)
					{
						bestLen = -bestLen;
					}
				}
				else
				{
					// The only hope of figuring out what is the front face is by
					// looking at vertex normals.
					if (mIsNormalPerVertex)
					{
						TQ3Vector3D	aVertNormal;
						if (mNormalIndex.empty())	// direct vertex normals
						{
							aVertNormal = mNormals[ mInstanceToPoint[ startIndex ] ];
						}
						else
						{
							aVertNormal = mNormals[ mNormalIndex[ startIndex ] ];
						}
						if (Q3FastVector3D_Dot( &bestCross, &aVertNormal ) < 0.0f)
						{
							bestLen = -bestLen;
						}
					}
				}

				Q3FastVector3D_Scale( &bestCross, 1.0f/bestLen, &theNormal );
				
				if (isSomeAnglePositive and isSomeAngleNegative)
				{
					mIsFaceConvex[i] = false;
				}
			}
		}
		mComputedFaceNormals.push_back( theNormal );
	}
}

/*!
	@function	FindBackLinks
	@abstract	For each point instance in mInstanceToPoint, find the previous instance
				of the same point and record its index within mInstanceToPoint in an
				array mPrevPointInstance.
*/
void	IndexedFaceMaker::FindBackLinks()
{
	mPrevPointInstance.resize( mInstanceToPoint.size(), -1 );
	PolyValue::IntVec	mostRecentPointInstance( mPoints.size(), -1 );
	const int kNumCoordIndices = mInstanceToPoint.size();
	for (int i = 0; i < kNumCoordIndices; ++i)
	{
		int	pointIndex = mInstanceToPoint[i];
		if (pointIndex >= 0)
		{
			if (mostRecentPointInstance[ pointIndex ] >= 0)
			{
				mPrevPointInstance[i] = mostRecentPointInstance[ pointIndex ];
			}
			mostRecentPointInstance[ pointIndex ] = i;
		}
	}
}

/*!
	@function	FindPrevSimilarPoint
	
	@abstract	Given a point instance index (index into mInstanceToPoint), find a
				previous instance of the same point that can be considered to
				be the same vertex.
*/
int		IndexedFaceMaker::FindPrevSimilarPoint( int inPtInstanceIndex )
{
	int	prevSimilarIndex = -1;
	int	prevIndex;
	for (prevIndex = mPrevPointInstance[inPtInstanceIndex];
		prevIndex >= 0;
		prevIndex = mPrevPointInstance[prevIndex])
	{
		if (ArePointsSimilar( inPtInstanceIndex, prevIndex ))
		{
			prevSimilarIndex = prevIndex;
			break;
		}
	}
	return prevSimilarIndex;
}

/*!
	@function	ArePointsSimilar
	@abstract	Determine whether two instances of the same point should be
				treated as the same vertex.
*/
bool	IndexedFaceMaker::ArePointsSimilar( int inPt1, int inPt2 )
{
	bool	isSame = true;
	int		face1, face2;
	
	if (mNormals.empty())	// no normals
	{
		// test angle between faces versus crease angle
		face1 = mInstanceToFace[ inPt1 ];
		face2 = mInstanceToFace[ inPt2 ];
		if ( (face1 >= 0) and (face2 >= 0) and (face1 != face2) )
		{
			float	dotProd = Q3FastVector3D_Dot( &mComputedFaceNormals[face1],
				&mComputedFaceNormals[face2] );
			if (dotProd < mCreaseAngleCosine)
			{
				isSame = false;
			}
		}
	}
	else if (mIsNormalPerVertex and (not mNormalIndex.empty()) )
	{
		if (mNormalIndex[inPt1] != mNormalIndex[inPt2])
		{
			isSame = false;
		}
	}
	else if (not mIsNormalPerVertex)	// per-face normals
	{
		if (mNormalIndex.empty())
		{
			isSame = false;
		}
		else	// indexed per-face normals
		{
			face1 = mInstanceToFace[ inPt1 ];
			face2 = mInstanceToFace[ inPt2 ];
			if (mNormalIndex[face1] != mNormalIndex[face2])
			{
				isSame = false;
			}
		}
	}
	
	// If the instances seem the same so far, colors may distinguish them.
	if ( isSame and (not mColors.empty()) )
	{
		if (mIsColorPerVertex and (not mColorIndex.empty()))
		{
			if (mColorIndex[inPt1] != mColorIndex[inPt2])
			{
				isSame = false;
			}
		}
		else if (mIsColorPerFace)		// per-face colors
		{
			if (mColorIndex.empty())
			{
				isSame = false;
			}
			else	// indexed per-face colors
			{
				face1 = mInstanceToFace[ inPt1 ];
				face2 = mInstanceToFace[ inPt2 ];
				if (mColorIndex[face1] != mColorIndex[face2])
				{
					isSame = false;
				}
			}
		}
	}
	
	// If the instances are the same so far, texture coordinates may distinguish them.
	if ( isSame and (not mUVs.empty()) and (not mUVIndex.empty()) )
	{
		if (mUVIndex[inPt1] != mUVIndex[inPt2])
		{
			isSame = false;
		}
	}
	
	return isSame;
}

/*!
	@function	FindDictinctVertices
	
	@abstract	When a point P belongs to two faces A and B, we sometimes want
				to consider P-in-A and P-in-B to be different vertices.
				We will construct a vector mVertexToPoint holding indices into
				mPoints, and change mInstanceToPoint so that it contains indices
				into mVertexToPoint rather than indices into mPoints.
*/
void	IndexedFaceMaker::FindDictinctVertices()
{
	if (mNormals.empty() or mIsNormalPerVertex)
	{
		ComputeFaceNormals();
	}
	FindBackLinks();
	
	const int kNumCoordIndices = mInstanceToPoint.size();
	mInstanceToVertex.resize( kNumCoordIndices );
	int	i;
	
	for (i = 0; i < kNumCoordIndices; ++i)
	{
		if (mInstanceToPoint[i] < 0)
		{
			mInstanceToVertex[i] = -1;
		}
		else
		{
			int	prevSimilar = FindPrevSimilarPoint( i );
			
			if (prevSimilar < 0)
			{
				mVertexToPoint.push_back( mInstanceToPoint[i] );
				mVertexToInstance.push_back( i );
				mInstanceToVertex[i] = mVertexToPoint.size() - 1;
			}
			else
			{
				mInstanceToVertex[i] = mInstanceToVertex[ prevSimilar ];
			}
		}
	}

#if DEBUG
	// Verify that we have factored mInstanceToPoint into
	// mInstanceToVertex and mVertexToPoint.
	for (i = 0; i < kNumCoordIndices; ++i)
	{
		check( ((mInstanceToPoint[i] < 0) and (mInstanceToVertex[i] < 0)) or
			(
				(mInstanceToPoint[i] >= 0) and (mInstanceToVertex[i] >= 0) and
				(mVertexToPoint[ mInstanceToVertex[i] ] == mInstanceToPoint[i])
			)
		);
	}
#endif
}

/*!
	@function	ComputeVertices
	@abstract	Compute the vector of points that will go into the TriMesh.
*/
void	IndexedFaceMaker::ComputeVertices()
{
	const int kNumVert = mVertexToPoint.size();
	mTriMeshPoints.resize( kNumVert );
	
	for (int i = 0; i < kNumVert; ++i)
	{
		mTriMeshPoints[i] = mPoints[ mVertexToPoint[i] ];
	}
}

/*!
	@function	ComputeTriangles
	@abstract	Compute the vector of triangles that will go into the TriMesh,
				bearing in mind that a face with k vertices will be split into
				k-2 triangles.
*/
void	IndexedFaceMaker::ComputeTriangles()
{
	const int kNumFace = mFaceStarts.size();
	mTriMeshFaces.reserve( mInstanceToPoint.size() - 3 * kNumFace );
	int	f;
	
	for (f = 0; f < kNumFace; ++f)
	{
		if (not mIsFaceDegenerate[ f ])
		{
			if (mIsFaceConvex[ f ])
			{
				ConvexFaceToTriangles( f );
			}
			else
			{
				NonConvexFaceToTriangles( f );
			}
		}
	}
}

/*!
	@function	ConvexFaceToTriangles
	@abstract	Split a convex face into triangles.
*/
void	IndexedFaceMaker::ConvexFaceToTriangles( int inFaceIndex )
{
	int	faceStart = mFaceStarts[ inFaceIndex ];
	for (int i = faceStart + 2; mInstanceToVertex[i] >= 0; ++i)
	{
		// Make a triangle from the instances faceStart, i-1, and i
		TQ3TriMeshTriangleData	aTri;
		aTri.pointIndices[0] = mInstanceToVertex[faceStart];
		if (mState.vertexOrdering == eVRML1VertexOrdering_COUNTERCLOCKWISE)
		{
			aTri.pointIndices[1] = mInstanceToVertex[i - 1];
			aTri.pointIndices[2] = mInstanceToVertex[i];
		}
		else if (mState.vertexOrdering == eVRML1VertexOrdering_CLOCKWISE)
		{
			aTri.pointIndices[1] = mInstanceToVertex[i];
			aTri.pointIndices[2] = mInstanceToVertex[i - 1];
		}
		else	// unknown order
		{
			// Pick a way, and then see if we guessed right using the face normal.
			aTri.pointIndices[1] = mInstanceToVertex[i];
			aTri.pointIndices[2] = mInstanceToVertex[i - 1];
			TQ3Vector3D	triangleNormal, faceNormal;
			Q3FastPoint3D_CrossProductTri( &mPoints[ mVertexToPoint[ aTri.pointIndices[0] ] ],
				&mPoints[ mVertexToPoint[ aTri.pointIndices[1] ] ],
				&mPoints[ mVertexToPoint[ aTri.pointIndices[2] ] ],
				&triangleNormal );
			if ( (not mIsNormalPerVertex) and (not mNormals.empty()) )	// have face normals
			{
				if (mNormalIndex.empty())
				{
					faceNormal = mNormals[ inFaceIndex ];
				}
				else
				{
					faceNormal = mNormals[ mNormalIndex[ inFaceIndex ] ];
				}
			}
			else
			{
				faceNormal = mComputedFaceNormals[ inFaceIndex ];
			}
			if (Q3FastVector3D_Dot( &faceNormal, &triangleNormal ))
			{
				std::swap( aTri.pointIndices[1], aTri.pointIndices[2] );
			}
		}
		mTriMeshFaces.push_back( aTri );
	
	#if DEBUG
		// The counterclockwise normal of the triangle should be in the same
		// direction as the face normal.
		TQ3Vector3D		faceNormal = mComputedFaceNormals[ inFaceIndex ];
		TQ3Vector3D		triNormal;
		Q3FastPoint3D_CrossProductTri( &mPoints[ mVertexToPoint[ aTri.pointIndices[0] ] ],
			&mPoints[ mVertexToPoint[ aTri.pointIndices[1] ] ],
			&mPoints[ mVertexToPoint[ aTri.pointIndices[2] ] ],
			&triNormal );
		Q3FastVector3D_Normalize( &triNormal, &triNormal );
		float	normDot = Q3FastVector3D_Dot( &faceNormal, &triNormal );
		check( normDot > 0.0f );
	#endif
	}
}

/*!
	@function	NonConvexFaceToTriangles
	@abstract	Split a non-convex face into triangles.
*/
void	IndexedFaceMaker::NonConvexFaceToTriangles( int inFaceIndex )
{
	// We will need to have the face normal handy.
	TQ3Vector3D	faceNormal = mComputedFaceNormals[ inFaceIndex ];
	
	// Copy the vertex indices of the face to a vector we can play with.
	const int kNumVerts = mFaceSizes[ inFaceIndex ];
	PolyValue::IntVec	verts( kNumVerts );
	const int	faceStart = mFaceStarts[ inFaceIndex ];
	int	i;
	for (i = 0; i < kNumVerts; ++i)
	{
		verts[ i ] = mInstanceToVertex[ faceStart + i ];
	}
	
	// If the face is clockwise, flip the normal and the face for
	// simplicity.
	if (mState.vertexOrdering == eVRML1VertexOrdering_CLOCKWISE)
	{
		Q3FastVector3D_Scale( &faceNormal, -1.0f, &faceNormal );
		std::reverse( verts.begin(), verts.end() );
	}
	// TODO: figure out what to do when the ordering is unknown.
	
	// As long as there are more than 3 vertices, we find a convex corner
	// and split off a triangle there.
	while (verts.size() > 3)
	{
		int	numVerts = verts.size();
		for (i = 0; i < numVerts; ++i)
		{
			TQ3Vector3D		cornerCross;
			Q3FastPoint3D_CrossProductTri(
				&mPoints[ mVertexToPoint[ verts[i] ] ],
				&mPoints[ mVertexToPoint[ verts[ (i+1) % numVerts ] ] ],
				&mPoints[ mVertexToPoint[ verts[ (i+2) % numVerts ] ] ],
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
				for (int j = 0; j < numVerts; ++j)
				{
					if ( (j != i) and (j != ((i+1) % numVerts)) and (j != ((i+2) % numVerts)) )
					{
						hitRay.origin = mPoints[ mVertexToPoint[ verts[j] ] ];
						if (Q3Ray3D_IntersectTriangle( &hitRay,
							&mPoints[ mVertexToPoint[ verts[i] ] ],
							&mPoints[ mVertexToPoint[ verts[ (i+1) % numVerts ] ] ],
							&mPoints[ mVertexToPoint[ verts[ (i+2) % numVerts ] ] ],
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
						verts[i], verts[ (i+1) % numVerts ], verts[ (i+2) % numVerts ]
					};
					mTriMeshFaces.push_back( aTri );
					
					verts.erase( verts.begin() + ((i+1) % numVerts) );
					break;
				}
			}
		}
		if (i == numVerts)
		{
			break;	// bad, this should not happen
		}
	}
	
	if (verts.size() == 3)
	{
		TQ3TriMeshTriangleData	lastTri = {
			verts[0], verts[ 1 ], verts[ 2 ]
		};
		mTriMeshFaces.push_back( lastTri );
	}
}


/*!
	@function	ComputeTriMeshVertexNormals
	@abstract	Compute the vector of vertex normals for the TriMesh.
*/
void	IndexedFaceMaker::ComputeTriMeshVertexNormals()
{
	const int kNumVert = mVertexToPoint.size();
	mTriMeshVertexNormals.resize( kNumVert );
	
	for (int v = 0; v < kNumVert; ++v)
	{
		if (mNormals.empty())	// no normals provided
		{
			mTriMeshVertexNormals[v] = mComputedFaceNormals[ mInstanceToFace[
				mVertexToInstance[v] ] ];
		}
		else if (mIsNormalPerVertex)
		{
			if (mNormalIndex.empty())	// direct vertex normals
			{
				mTriMeshVertexNormals[v] = mNormals[ mVertexToPoint[v] ];
			}
			else						// indexed vertex normals
			{
				mTriMeshVertexNormals[v] = mNormals[ mNormalIndex[ mVertexToInstance[v] ] ];
			}
		}
		else	// per-face normals
		{
			if (mNormalIndex.empty())	// direct face normals
			{
				mTriMeshVertexNormals[v] = mNormals[ mInstanceToFace[ mVertexToInstance[v] ] ];
			}
			else						// indexed face normals
			{
				mTriMeshVertexNormals[v] = mNormals[ mNormalIndex[ mInstanceToFace[
					mVertexToInstance[v] ] ] ];
			}
		}
	}
}

/*!
	@function	ComputeTriMeshFaceNormals
	@abstract	Compute the vector of triangle normals for the TriMesh,
				bearing in mind that a face with k vertices will be split into
				k-2 triangles.
*/
void	IndexedFaceMaker::ComputeTriMeshFaceNormals()
{
	const int kNumFace = mFaceStarts.size();
	const int kNumTri = mTriMeshFaces.size();
	
	mTriMeshFaceNormals.reserve( kNumTri );
	int	f, i;
	
	if (mNormals.empty() or mIsNormalPerVertex)	// no face normals provided
	{
		for (f = 0; f < kNumFace; ++f)
		{
			if (not mIsFaceDegenerate[ f ])
			{
				for (i = 0; i < mFaceSizes[f]-2; ++i)
				{
					mTriMeshFaceNormals.push_back( mComputedFaceNormals[f] );
				}
			}
		}
	}
	else if (mNormalIndex.empty())	// direct face normals
	{
		for (f = 0; f < kNumFace; ++f)
		{
			for (i = 0; i < mFaceSizes[f]-2; ++i)
			{
				mTriMeshFaceNormals.push_back( mNormals[f] );
			}
		}
	}
	else	// indexed face normals
	{
		for (f = 0; f < kNumFace; ++f)
		{
			for (i = 0; i < mFaceSizes[f]-2; ++i)
			{
				mTriMeshFaceNormals.push_back( mNormals[ mNormalIndex[ f ] ] );
			}
		}
	}
}


/*!
	@function	ComputeTriMeshVertexColors
	@abstract	Compute an array of vertex colors, if we have vertex colors.
*/
void	IndexedFaceMaker::ComputeTriMeshVertexColors()
{
	if ( (mColors.size() > 1) and mIsColorPerVertex )
	{
		const int kNumVert = mVertexToPoint.size();
		mTriMeshVertexColors.resize( kNumVert );
		int	v;
		
		if (mColorIndex.empty())
		{
			for (v = 0; v < kNumVert; ++v)
			{
				mTriMeshVertexColors[v] = mColors[ mVertexToPoint[v] ];
			}
		}
		else	// indexed vertex colors
		{
			for (v = 0; v < kNumVert; ++v)
			{
				mTriMeshVertexColors[v] = mColors[ mColorIndex[ mVertexToInstance[v] ] ];
			}
		}
	}
}

/*!
	@function	ComputeTriMeshFaceColors
	@abstract	Compute an array of face colors, if we have face colors.
*/
void	IndexedFaceMaker::ComputeTriMeshFaceColors()
{
	if ( (mColors.size() > 1) and mIsColorPerFace )
	{
		const int kNumFace = mFaceStarts.size();
		mTriMeshFaceColors.reserve( mTriMeshFaces.size() );
		int	f, i;
		
		if (mColorIndex.empty())
		{
			for (f = 0; f < kNumFace; ++f)
			{
				for (i = 0; i < mFaceSizes[f]-2; ++i)
				{
					mTriMeshFaceColors.push_back( mColors[f] );
				}
			}
		}
		else	// indexed face colors
		{
			for (f = 0; f < kNumFace; ++f)
			{
				for (i = 0; i < mFaceSizes[f]-2; ++i)
				{
					mTriMeshFaceColors.push_back( mColors[ mColorIndex[ f ] ] );
				}
			}
		}
	}
}

/*!
	@function	ComputeTriMeshBounds
	@abstract	Compute the bounding box of the points.
*/
void	IndexedFaceMaker::ComputeTriMeshBounds()
{
	Q3BoundingBox_SetFromPoints3D( &mTriMeshBounds, &mPoints[0], mPoints.size(),
		sizeof(TQ3Point3D) );
}

/*!
	@function	ComputeTriMeshVertexUVs
	@abstract	Compute an array of texture coordinates.
	@discussion	ComputeTriMeshBounds must have been called earlier.
*/
void	IndexedFaceMaker::ComputeTriMeshVertexUVs()
{
	const int kNumVert = mVertexToPoint.size();
	mTriMeshVertexUVs.resize( kNumVert );
	int	v;

	if (not mUVs.empty())
	{
		if (mUVIndex.empty())
		{
			for (v = 0; v < kNumVert; ++v)
			{
				mTriMeshVertexUVs[v] = mUVs[ mVertexToPoint[v] ];
			}
		}
		else
		{
			for (v = 0; v < kNumVert; ++v)
			{
				mTriMeshVertexUVs[v] = mUVs[ mUVIndex[ mVertexToInstance[v] ] ];
			}
		}
	}
	else	// must synthesize some UVs
	{
		// Find the largest and second largest axes of the bounds
		float	extents[3] = {
			mTriMeshBounds.max.x - mTriMeshBounds.min.x,
			mTriMeshBounds.max.y - mTriMeshBounds.min.y,
			mTriMeshBounds.max.z - mTriMeshBounds.min.z
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
		
		float*	minBounds = &mTriMeshBounds.min.x;
		float	majorAxisMin = minBounds[majorAxis];
		float	minorAxisMin = minBounds[minorAxis];
		float	majorExtent = extents[majorAxis];
		
		for (v = 0; v < kNumVert; ++v)
		{
			float*	coords = &mTriMeshPoints[v].x;
			mTriMeshVertexUVs[v].u = (coords[majorAxis] - majorAxisMin) / majorExtent;
			mTriMeshVertexUVs[v].v = (coords[minorAxis] - minorAxisMin) / majorExtent;
		}
	}
}

/*!
	@function	ComputeTriMeshArrays
	@abstract	Compute various arrays we will need to build a TriMesh.
*/
void	IndexedFaceMaker::ComputeTriMeshArrays()
{
	ComputeVertices();
	ComputeTriangles();
	ComputeTriMeshVertexNormals();
	ComputeTriMeshFaceNormals();
	ComputeTriMeshVertexColors();
	ComputeTriMeshFaceColors();
	ComputeTriMeshBounds();
	ComputeTriMeshVertexUVs();
}

/*!
	@function	CreateObject
	@abstract	Create a Quesa object.
	@discussion	Usually we produce a TriMesh object.  However, if the "solid"
				field has the value FALSE, we must create a display group
				containing a backfacing-both style object plus a TriMesh.
	@result		A Quesa shape, or NULL on failure.
*/
CQ3ObjectRef	IndexedFaceMaker::CreateObject()
{
	// Set up vertex attributes
	TQ3TriMeshAttributeData		vertAtts[3];
	int	vertAttNum = 0;
	// Vertex normal, always present
	vertAtts[ vertAttNum ].attributeType = kQ3AttributeTypeNormal;
	vertAtts[ vertAttNum ].data = &mTriMeshVertexNormals[0];
	vertAtts[ vertAttNum ].attributeUseArray = NULL;
	// texture coordinates
	if (not mTriMeshVertexUVs.empty())
	{
		vertAttNum += 1;
		vertAtts[ vertAttNum ].attributeType = kQ3AttributeTypeSurfaceUV;
		vertAtts[ vertAttNum ].data = &mTriMeshVertexUVs[0];
		vertAtts[ vertAttNum ].attributeUseArray = NULL;
	}
	// vertex colors
	if (not mTriMeshVertexColors.empty())
	{
		vertAttNum += 1;
		vertAtts[ vertAttNum ].attributeType = kQ3AttributeTypeDiffuseColor;
		vertAtts[ vertAttNum ].data = &mTriMeshVertexColors[0];
		vertAtts[ vertAttNum ].attributeUseArray = NULL;
	}
	
	// Set up face attributes
	TQ3TriMeshAttributeData		faceAtts[3];
	int	faceAttNum = 0;
	// face normal, always present
	faceAtts[ faceAttNum ].attributeType = kQ3AttributeTypeNormal;
	faceAtts[ faceAttNum ].data = &mTriMeshFaceNormals[0];
	faceAtts[ faceAttNum ].attributeUseArray = NULL;
	// face colors
	if (not mTriMeshFaceColors.empty())
	{
		faceAttNum += 1;
		faceAtts[ faceAttNum ].attributeType = kQ3AttributeTypeDiffuseColor;
		faceAtts[ faceAttNum ].data = &mTriMeshFaceColors[0];
		faceAtts[ faceAttNum ].attributeUseArray = NULL;
	}
	
	CQ3ObjectRef	overallColor;
	if ( (mState.materialBinding == eVRML1Value_OVERALL) or
		(mState.materialBinding == eVRML1Value_DEFAULT) )
	{
		overallColor = GetIndexedMaterial( mReader, 0 );
	}
	
	// Create TriMesh
	TQ3TriMeshData		triMeshData =
	{
		overallColor.get(),		// triMeshAttributeSet
		mTriMeshFaces.size(),	// numTriangles
		&mTriMeshFaces[0],		// triangles
		faceAttNum + 1,			// numTriangleAttributeTypes
		faceAtts,				// triangleAttributeTypes
		0,						// numEdges
		NULL,					// edges
		0,						// numEdgeAttributeTypes
		NULL,					// edgeAttributeTypes
		mTriMeshPoints.size(),	// numPoints
		&mTriMeshPoints[0],		// points
		vertAttNum + 1,			// numVertexAttributeTypes
		vertAtts,				// vertexAttributeTypes
		mTriMeshBounds			// bBox
	};
	CQ3ObjectRef	theTriMesh( Q3TriMesh_New( &triMeshData ) );
	CQ3ObjectRef	resultShape;

	if (theTriMesh.isvalid())
	{
		// If this geometry was named with DEF, set that as the name of the
		// Quesa object.
		if (IsKeyPresent( mNodeDict, "[name]" ))
		{
			PolyValue&	nameValue( mNodeDict["[name]"] );
			const std::string&	theName( nameValue.GetString() );
			::CENameElement_SetData( theTriMesh.get(), theName.c_str() );
		}
		
		if (mIsSolid)
		{
			resultShape = theTriMesh;
		}
		else
		{
			resultShape = CQ3ObjectRef( Q3DisplayGroup_New() );
			ThrowIfNullQuesaOb_( resultShape );
			CQ3ObjectRef	theStyle( Q3BackfacingStyle_New( kQ3BackfacingStyleFlip ) );
			ThrowIfNullQuesaOb_( theStyle );
			
			Q3Group_AddObject( resultShape.get(), theStyle.get() );
			Q3Group_AddObject( resultShape.get(), theTriMesh.get() );
		}
	}
	
	return resultShape;
}

/*!
	@function	IndexedFaceSetV1ToObject
	
	@abstract	Attempt to convert a VRML 1 IndexedFaceSet node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	IndexedFaceSetV1ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	CQ3ObjectRef	theObject;
	
	IndexedFaceMaker	maker( ioNode, inReader );
	
	if (maker.HasEnoughData())
	{
		maker.FindFaces();
		maker.FindDictinctVertices();
		maker.ComputeTriMeshArrays();
		theObject = maker.CreateObject();
	}
	
	return theObject;
}

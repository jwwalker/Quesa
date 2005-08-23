/*  NAME:
        E3GeometryTriMeshOptimize.cpp

    DESCRIPTION:
        Functions to optimize a TriMesh for use by the interactive renderer.

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
#include "E3GeometryTriMeshOptimize.h"

#include "E3Memory.h"
#include "E3Utils.h"
#include "E3Set.h"
#include "E3ClassTree.h"
#include "QuesaMath.h"

#include <vector>
#include <algorithm>

#define	EQ3ThrowIfMemFail_( x )		do { if ( (x) == NULL ) { \
										throw std::bad_alloc();	\
										} } while (false)

#define	EQ3ThrowIf_( x )			do { if ( (x) ) { \
										throw std::exception();	\
										} } while (false)

namespace
{
	const float		kDegenerateLengthSquared	= 1.0e-12f;
	
	typedef	std::vector< TQ3Vector3D >	VecVec;
	
	typedef std::vector< TQ3Int32 >		IntVec;

	class TriMeshOptimizer
	{
	public:
								TriMeshOptimizer(
										const TQ3TriMeshData& inData,
										TQ3TriMeshData& outData );

		TQ3Boolean				Optimize();
	
	private:
		void					FindOrigAtts();
		TQ3Boolean				IsOptNeeded() const;
		void					EnsureFaceNormals();
		void					MakeInstanceToPoint();
		void					FindBackLinks();
		bool					ArePointsSimilar( TQ3Int32 inPt1, TQ3Int32 inPt2 );
		TQ3Int32				FindPrevSimilarPoint( TQ3Int32 inPtInstanceIndex );
		void					FindDictinctVertices();
		void					BuildNewTriMesh();
		void					BuildFaces();
		void					BuildFaceAttributes();
		void					BuildEdges();
		void					BuildEdgeAttributes();
		void					BuildPoints();
		void					BuildVertexAttributes();
		void					ComputeBounds();
	
		const TQ3TriMeshData&	mOrigData;
		TQ3TriMeshData&			mResultData;
		
		const TQ3Vector3D*		mOrigFaceNormals;
		const TQ3Vector3D*		mOrigVertexNormals;
		const TQ3ColorRGB*		mOrigFaceColor;
		const TQ3ColorRGB*		mOrigVertexColor;
		const TQ3ColorRGB*		mOrigFaceTransparency;
		const TQ3ColorRGB*		mOrigVertexTransparency;
		const TQ3ColorRGB*		mOrigFaceSpecularColor;
		const TQ3ColorRGB*		mOrigVertexSpecularColor;
		
		VecVec					mComputedFaceNormals;
		const TQ3Vector3D*		mResultFaceNormals;
		IntVec					mInstanceToPoint;
		IntVec					mPrevPointInstance;
		IntVec					mInstanceToVertex;
		IntVec					mVertexToPoint;
		IntVec					mVertexToFace;
		IntVec					mPointToVertex;
	};
}

TriMeshOptimizer::TriMeshOptimizer(
										const TQ3TriMeshData& inData,
										TQ3TriMeshData& outData )
	: mOrigData( inData )
	, mResultData( outData )
	, mOrigFaceNormals( NULL )
	, mOrigVertexNormals( NULL )
	, mOrigFaceColor( NULL )
	, mOrigVertexColor( NULL )
	, mOrigFaceTransparency( NULL )
	, mOrigVertexTransparency( NULL )
	, mOrigFaceSpecularColor( NULL )
	, mOrigVertexSpecularColor( NULL )
	, mResultFaceNormals( NULL )
{
}

TQ3Boolean		TriMeshOptimizer::Optimize()
{
	FindOrigAtts();
	
	TQ3Boolean	optNeeded = IsOptNeeded();
	
	if (optNeeded)
	{
		EnsureFaceNormals();
		MakeInstanceToPoint();
		FindBackLinks();
		FindDictinctVertices();
		BuildNewTriMesh();
	}
	
	return optNeeded;
}

TQ3Boolean		TriMeshOptimizer::IsOptNeeded() const
{
	TQ3Boolean	needOpt = kQ3False;
	
	if ( (mOrigFaceNormals == NULL) ||
		(mOrigVertexNormals == NULL) ||
		((mOrigFaceColor != NULL) && (mOrigVertexColor == NULL)) ||
		((mOrigFaceTransparency != NULL) && (mOrigVertexTransparency == NULL)) ||
		((mOrigFaceSpecularColor != NULL) && (mOrigVertexSpecularColor == NULL))
	)
	{
		needOpt = kQ3True;
	}
	
	return needOpt;
}

/*!
	@function	MakeInstanceToPoint
	
	@abstract	For convenience, convert the triangles array into
				a straight array of point indices.
*/
void	TriMeshOptimizer::MakeInstanceToPoint()
{
	mInstanceToPoint.resize( mOrigData.numTriangles * 3 );
	
	for (TQ3Uns32 i = 0; i < mOrigData.numTriangles; ++i)
	{
		mInstanceToPoint[ 3 * i ] = mOrigData.triangles[i].pointIndices[0];
		mInstanceToPoint[ 3 * i + 1 ] = mOrigData.triangles[i].pointIndices[1];
		mInstanceToPoint[ 3 * i + 2 ] = mOrigData.triangles[i].pointIndices[2];
	}
}

/*!
	@function	FindBackLinks
	
	@abstract	For each point instance in mInstanceToPoint, find the previous instance
				of the same point and record its index within mInstanceToPoint in an
				array mPrevPointInstance.  When there is no previous instance,
				record -1.
*/
void	TriMeshOptimizer::FindBackLinks()
{
	const TQ3Int32 kNumCoordIndices = mInstanceToPoint.size();
	mPrevPointInstance.resize( kNumCoordIndices, -1 );
	IntVec	mostRecentPointInstance( mOrigData.numPoints, -1 );
	
	for (TQ3Int32 i = 0; i < kNumCoordIndices; ++i)
	{
		TQ3Int32	pointIndex = mInstanceToPoint[i];
		if (mostRecentPointInstance[ pointIndex ] >= 0)
		{
			mPrevPointInstance[i] = mostRecentPointInstance[ pointIndex ];
		}
		mostRecentPointInstance[ pointIndex ] = i;
	}
}

static bool IsSameColor( const TQ3ColorRGB& inOne, const TQ3ColorRGB& inTwo )
{
	return fabsf(inOne.r - inTwo.r) + fabsf(inOne.g - inTwo.g) +
		fabsf(inOne.b - inTwo.b) < FLT_EPSILON;
}

/*!
	@function	ArePointsSimilar
	
	@abstract	Determine whether two instances of the same point (given by
				indices into mInstanceToPoint) should be treated as the same
				vertex.
*/
bool	TriMeshOptimizer::ArePointsSimilar( TQ3Int32 inPt1, TQ3Int32 inPt2 )
{
	bool	isSame = true;
	TQ3Int32	face1, face2;
	
	face1 = inPt1 / 3;
	face2 = inPt2 / 3;
	
	// If vertex normals do not exist, similar instances must have the same
	// face normal.
	if (mOrigVertexNormals == NULL)
	{
		float	dotProd = Q3FastVector3D_Dot( &mResultFaceNormals[face1],
			&mResultFaceNormals[ face2 ] );
		
		if (dotProd < 1.0f - FLT_EPSILON)
		{
			isSame = false;
		}
	}
	
	// If the instances seem the same so far, and there are face colors but
	// not vertex colors, then the face colors may distinguish them.
	if ( isSame && (mOrigFaceColor != NULL) && (mOrigVertexColor == NULL) )
	{
		isSame = IsSameColor( mOrigFaceColor[face1], mOrigFaceColor[face2] );
	}
	
	// Same for transparency color.
	if ( isSame && (mOrigFaceTransparency != NULL) && (mOrigVertexTransparency == NULL) )
	{
		isSame = IsSameColor( mOrigFaceTransparency[face1], mOrigFaceTransparency[face2] );
	}
	
	// Same for specular color.
	if ( isSame && (mOrigFaceSpecularColor != NULL) && (mOrigVertexSpecularColor == NULL) )
	{
		isSame = IsSameColor( mOrigFaceSpecularColor[face1], mOrigFaceSpecularColor[face2] );
	}
	
	return isSame;
}

/*!
	@function	FindPrevSimilarPoint
	
	@abstract	Given a point instance index (index into mInstanceToPoint), find a
				previous instance of the same point that can be considered to
				be the same vertex.
*/
TQ3Int32	TriMeshOptimizer::FindPrevSimilarPoint( TQ3Int32 inPtInstanceIndex )
{
	TQ3Int32	prevSimilarIndex = -1;
	TQ3Int32	prevIndex;
	for (prevIndex = mPrevPointInstance[ inPtInstanceIndex ];
		prevIndex >= 0;
		prevIndex = mPrevPointInstance[ prevIndex ])
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
	@function	FindDictinctVertices
	
	@abstract	Decide how points need to be duplicated.
	
	@discussion	Let's say the original TriMesh has F faces and P points.
				The array mInstanceToPoint can be thought of as a function
				from the set {0... 3F-1} to the set {0... P-1}.
				Here we will construct an array mInstanceToVertex,
				mapping {0... 3F-1} to {0... V-1}, and an array mVertexToPoint
				mapping {0... V-1} to {0... P-1}, such that
				mVertexToPoint[ mInstanceToVertex[i] ] == mInstanceToPoint[i]
				for each i, and if i and j are instances that are not
				"similar", mInstanceToVertex[i] != mInstanceToVertex[j].
				We also need an array mVertexToFace that will map a vertex
				to one of the faces that contains it, and an array
				mPointToVertex that maps a point to one of the vertices at
				that location.
*/
void	TriMeshOptimizer::FindDictinctVertices()
{
	const TQ3Int32	kNumInstances = mInstanceToPoint.size();
	mInstanceToVertex.resize( kNumInstances );
	mPointToVertex.resize( mOrigData.numPoints, -1 );
	TQ3Int32	i;
	
	for (i = 0; i < kNumInstances; ++i)
	{
		TQ3Int32	prevSimilar = FindPrevSimilarPoint( i );
		if (prevSimilar < 0)
		{
			// new vertex
			mVertexToPoint.push_back( mInstanceToPoint[i] );
			mInstanceToVertex[i] = mVertexToPoint.size() - 1;
			mVertexToFace.push_back( i / 3 );
			mPointToVertex[ mInstanceToPoint[i] ] = mVertexToPoint.size() - 1;
		}
		else
		{
			mInstanceToVertex[i] = mInstanceToVertex[ prevSimilar ];
		}
	}
}

/*!
	@function	BuildNewTriMesh
	
	@abstract	Set up the new TriMesh data.
*/
void	TriMeshOptimizer::BuildNewTriMesh()
{
	E3Shared_Acquire( &mResultData.triMeshAttributeSet, mOrigData.triMeshAttributeSet );
	BuildFaces();
	BuildFaceAttributes();
	BuildEdges();
	BuildEdgeAttributes();
	BuildPoints();
	BuildVertexAttributes();
	ComputeBounds();
}

void	TriMeshOptimizer::BuildFaces()
{
	mResultData.numTriangles = mOrigData.numTriangles;
	mResultData.triangles = static_cast<TQ3TriMeshTriangleData*>(
		E3Memory_Allocate( mResultData.numTriangles *
			sizeof(TQ3TriMeshTriangleData) ) );
	EQ3ThrowIfMemFail_( mResultData.triangles );
	
	for (TQ3Uns32 i = 0; i < mResultData.numTriangles; ++i)
	{
		mResultData.triangles[i].pointIndices[0] = mInstanceToVertex[ 3 * i ];
		mResultData.triangles[i].pointIndices[1] = mInstanceToVertex[ 3 * i + 1 ];
		mResultData.triangles[i].pointIndices[2] = mInstanceToVertex[ 3 * i + 2 ];
	}
}

static TQ3Uns32	GetAttributeSize( TQ3AttributeType inAttType )
{
	TQ3ObjectType	attType = E3Attribute_AttributeToClassType( inAttType );
	E3ClassInfoPtr theClass = E3ClassTree::GetClass( attType );
	EQ3ThrowIf_( theClass == NULL );
	TQ3Uns32 attrSize = theClass->GetInstanceSize();
	return attrSize;
}

static void CopyAttributeData(
							TQ3Uns32 inNumElements, 
							const TQ3TriMeshAttributeData& inSrc,
							TQ3TriMeshAttributeData& outDest )
{
	outDest.attributeType = inSrc.attributeType;
	
	TQ3Uns32 attrSize = GetAttributeSize( inSrc.attributeType );
	TQ3Uns32 bytes    = inNumElements * attrSize;
	if (bytes == 0)
	{
		outDest.data = NULL;
	}
	else
	{
		outDest.data = E3Memory_Allocate( bytes );
		EQ3ThrowIfMemFail_( outDest.data );
		E3Memory_Copy( inSrc.data, outDest.data, bytes );
	}
	
	if ( (inSrc.attributeUseArray == NULL) || (inNumElements == 0) )
	{
		outDest.attributeUseArray = NULL;
	}
	else
	{
		bytes = inNumElements;
		outDest.attributeUseArray = static_cast<char*>( E3Memory_Allocate( bytes ) );
		EQ3ThrowIfMemFail_( outDest.attributeUseArray );
		E3Memory_Copy( inSrc.attributeUseArray, outDest.attributeUseArray, bytes );
	}
}

void	TriMeshOptimizer::BuildFaceAttributes()
{
	const TQ3Uns32	kNumAtts = mOrigData.numTriangleAttributeTypes;
	mResultData.numTriangleAttributeTypes = kNumAtts;
	if (mOrigFaceNormals == NULL)
	{
		mResultData.numTriangleAttributeTypes += 1;
	}
	mResultData.triangleAttributeTypes = static_cast<TQ3TriMeshAttributeData*>(
		E3Memory_AllocateClear( mResultData.numTriangleAttributeTypes *
			sizeof(TQ3TriMeshAttributeData) ) );
	EQ3ThrowIfMemFail_( mResultData.triangleAttributeTypes );
	TQ3Uns32	i;
	
	for (i = 0; i < kNumAtts; ++i)
	{
		CopyAttributeData( mOrigData.numTriangles,
			mOrigData.triangleAttributeTypes[i],
			mResultData.triangleAttributeTypes[i] );
	}
	
	if (mOrigFaceNormals == NULL)
	{
		mResultData.triangleAttributeTypes[ kNumAtts ].attributeType
			= kQ3AttributeTypeNormal;
		
		mResultData.triangleAttributeTypes[ kNumAtts ].data =
			E3Memory_Allocate( mOrigData.numTriangles * sizeof(TQ3Vector3D) );
		EQ3ThrowIfMemFail_( mResultData.triangleAttributeTypes[ kNumAtts ].data );
		E3Memory_Copy( &mComputedFaceNormals[0],
			mResultData.triangleAttributeTypes[ kNumAtts ].data,
			mOrigData.numTriangles * sizeof(TQ3Vector3D) );
	}
}

void	TriMeshOptimizer::BuildEdges()
{
	mResultData.numEdges = mOrigData.numEdges;
	
	if (mResultData.numEdges == 0)
	{
		mResultData.edges = NULL;
	}
	else
	{
		mResultData.edges = static_cast<TQ3TriMeshEdgeData*>(
			E3Memory_Allocate( mOrigData.numEdges * sizeof(TQ3TriMeshEdgeData) ) );
		EQ3ThrowIfMemFail_( mResultData.edges );
		E3Memory_Copy( mOrigData.edges, mResultData.edges,
			mOrigData.numEdges * sizeof(TQ3TriMeshEdgeData) );
		
		// Update point indices
		for (TQ3Uns32 i = 0; i < mOrigData.numEdges; ++i)
		{
			mResultData.edges[i].pointIndices[0] =
				mPointToVertex[ mResultData.edges[i].pointIndices[0] ];
			mResultData.edges[i].pointIndices[1] =
				mPointToVertex[ mResultData.edges[i].pointIndices[1] ];
		}
	}
}

void	TriMeshOptimizer::BuildEdgeAttributes()
{
	mResultData.numEdgeAttributeTypes = mOrigData.numEdgeAttributeTypes;
	
	mResultData.edgeAttributeTypes = static_cast<TQ3TriMeshAttributeData*>(
		E3Memory_AllocateClear( mResultData.numEdgeAttributeTypes *
			sizeof(TQ3TriMeshAttributeData) ) );
	EQ3ThrowIfMemFail_( mResultData.edgeAttributeTypes );
	
	for (TQ3Uns32 i = 0; i < mOrigData.numEdgeAttributeTypes; ++i)
	{
		CopyAttributeData( mOrigData.numEdges,
			mOrigData.edgeAttributeTypes[i],
			mResultData.edgeAttributeTypes[i] );
	}
}

void	TriMeshOptimizer::BuildPoints()
{
	mResultData.numPoints = mVertexToPoint.size();
	
	mResultData.points = static_cast<TQ3Point3D*>(
		E3Memory_AllocateClear( mResultData.numPoints * sizeof(TQ3Point3D) ) );
	EQ3ThrowIfMemFail_( mResultData.points );
	
	for (TQ3Uns32 i = 0; i < mResultData.numPoints; ++i)
	{
		mResultData.points[i] = mOrigData.points[ mVertexToPoint[i] ];
	}
}

void	TriMeshOptimizer::BuildVertexAttributes()
{
	bool	isAddingNormals = (mOrigVertexNormals == NULL);
	bool	isAddingColor = ((mOrigFaceColor != NULL) && (mOrigVertexColor == NULL));
	bool	isAddingTransparency = ((mOrigFaceTransparency != NULL) &&
		(mOrigVertexTransparency == NULL));
	bool	isAddingSpecular = ((mOrigFaceSpecularColor != NULL) &&
		(mOrigVertexSpecularColor == NULL));
	
	mResultData.numVertexAttributeTypes = mOrigData.numVertexAttributeTypes;
	if (isAddingNormals)
	{
		mResultData.numVertexAttributeTypes += 1;
	}
	if (isAddingColor)
	{
		mResultData.numVertexAttributeTypes += 1;
	}
	if (isAddingTransparency)
	{
		mResultData.numVertexAttributeTypes += 1;
	}
	if (isAddingSpecular)
	{
		mResultData.numVertexAttributeTypes += 1;
	}
	
	mResultData.vertexAttributeTypes = static_cast<TQ3TriMeshAttributeData*>(
		E3Memory_AllocateClear( mResultData.numVertexAttributeTypes *
			sizeof(TQ3TriMeshAttributeData) ) );
	EQ3ThrowIfMemFail_( mResultData.vertexAttributeTypes );
	TQ3Uns32	i, j;

	for (i = 0; i < mOrigData.numVertexAttributeTypes; ++i)
	{
		mResultData.vertexAttributeTypes[i].attributeType =
			mOrigData.vertexAttributeTypes[i].attributeType;
		
		TQ3Uns32 attrSize = GetAttributeSize(
			mOrigData.vertexAttributeTypes[i].attributeType );
		char*	srcData;
		char*	dstData;
		
		if (mResultData.numPoints > 0)
		{
			mResultData.vertexAttributeTypes[i].data =
				E3Memory_Allocate( attrSize * mResultData.numPoints );
			EQ3ThrowIfMemFail_( mResultData.vertexAttributeTypes[i].data );
			
			srcData = static_cast<char*>( mOrigData.vertexAttributeTypes[i].data );
			dstData = static_cast<char*>( mResultData.vertexAttributeTypes[i].data );
			
			for (j = 0; j < mResultData.numPoints; ++j)
			{
				E3Memory_Copy( srcData + mVertexToPoint[j] * attrSize,
					dstData + j * attrSize, attrSize );
			}
			
			if (mOrigData.vertexAttributeTypes[i].attributeUseArray != NULL)
			{
				mResultData.vertexAttributeTypes[i].attributeUseArray =
					static_cast<char*>( E3Memory_Allocate( mResultData.numPoints ) );
				EQ3ThrowIfMemFail_( mResultData.vertexAttributeTypes[i].attributeUseArray );
				
				for (j = 0; j < mResultData.numPoints; ++j)
				{
					mResultData.vertexAttributeTypes[i].attributeUseArray[ j ] =
						mOrigData.vertexAttributeTypes[i].attributeUseArray[
							mVertexToPoint[j] ];
				}
			}
		}
	}
	
	if (isAddingNormals)
	{
		mResultData.vertexAttributeTypes[i].attributeType = kQ3AttributeTypeNormal;
		TQ3Vector3D*	vertNormals = static_cast<TQ3Vector3D*>(
			E3Memory_Allocate( mResultData.numPoints * sizeof(TQ3Vector3D) ) );
		EQ3ThrowIfMemFail_( vertNormals );
		mResultData.vertexAttributeTypes[i].data = vertNormals;
		
		for (j = 0; j < mResultData.numPoints; ++j)
		{
			vertNormals[j] = mResultFaceNormals[ mVertexToFace[j] ];
		}
		++i;
	}
	
	if (isAddingColor)
	{
		mResultData.vertexAttributeTypes[i].attributeType = kQ3AttributeTypeDiffuseColor;
		TQ3ColorRGB*	vertColors = static_cast<TQ3ColorRGB*>(
			E3Memory_Allocate( mResultData.numPoints * sizeof(TQ3ColorRGB) ) );
		EQ3ThrowIfMemFail_( vertColors );
		mResultData.vertexAttributeTypes[i].data = vertColors;
		
		for (j = 0; j < mResultData.numPoints; ++j)
		{
			vertColors[j] = mOrigFaceColor[ mVertexToFace[j] ];
		}
		++i;
	}
	
	if (isAddingTransparency)
	{
		mResultData.vertexAttributeTypes[i].attributeType = kQ3AttributeTypeTransparencyColor;
		TQ3ColorRGB*	vertTrans = static_cast<TQ3ColorRGB*>(
			E3Memory_Allocate( mResultData.numPoints * sizeof(TQ3ColorRGB) ) );
		EQ3ThrowIfMemFail_( vertTrans );
		mResultData.vertexAttributeTypes[i].data = vertTrans;
		
		for (j = 0; j < mResultData.numPoints; ++j)
		{
			vertTrans[j] = mOrigFaceTransparency[ mVertexToFace[j] ];
		}
		++i;
	}
	
	if (isAddingSpecular)
	{
		mResultData.vertexAttributeTypes[i].attributeType = kQ3AttributeTypeSpecularColor;
		TQ3ColorRGB*	vertSpecs = static_cast<TQ3ColorRGB*>(
			E3Memory_Allocate( mResultData.numPoints * sizeof(TQ3ColorRGB) ) );
		EQ3ThrowIfMemFail_( vertSpecs );
		mResultData.vertexAttributeTypes[i].data = vertSpecs;
		
		for (j = 0; j < mResultData.numPoints; ++j)
		{
			vertSpecs[j] = mOrigFaceSpecularColor[ mVertexToFace[j] ];
		}
		++i;
	}
}

void	TriMeshOptimizer::ComputeBounds()
{
	Q3BoundingBox_SetFromPoints3D( &mResultData.bBox, mResultData.points,
		mResultData.numPoints, sizeof(TQ3Point3D) );
}


/*!
	@function	EnsureFaceNormals
	
	@abstract	If face normals were not provided, create them.
*/
void	TriMeshOptimizer::EnsureFaceNormals()
{
	if (mOrigFaceNormals == NULL)
	{
		mComputedFaceNormals.resize( mOrigData.numTriangles );
		
		TQ3Vector3D	theNormal;
		TQ3Uns32	i;
		
		for (i = 0; i < mOrigData.numTriangles; ++i)
		{
			typedef TQ3TriMeshTriangleData&	TQ3TriMeshTriangleDataRef; // limited VC++ 6.0

			TQ3TriMeshTriangleDataRef	aFace( mOrigData.triangles[i] );

			Q3FastPoint3D_CrossProductTri(
				&mOrigData.points[ aFace.pointIndices[0] ],
				&mOrigData.points[ aFace.pointIndices[1] ],
				&mOrigData.points[ aFace.pointIndices[2] ],
				&theNormal );
			float	lenSq = Q3FastVector3D_LengthSquared( &theNormal );
			
			if (lenSq < kDegenerateLengthSquared)
			{
				theNormal.x = 1.0f;
				theNormal.y = theNormal.z = 0.0f;
			}
			else
			{
				float	len = sqrt( lenSq );
				Q3FastVector3D_Scale( &theNormal, 1.0f/len, &theNormal );
			}
			mComputedFaceNormals[i] = theNormal;
		}
		mResultFaceNormals = &mComputedFaceNormals[0];
	}
	else
	{
		mResultFaceNormals = mOrigFaceNormals;
	}
}

void	TriMeshOptimizer::FindOrigAtts()
{
	TQ3Uns32	i;
	
	for (i = 0; i < mOrigData.numTriangleAttributeTypes; ++i)
	{
		switch (mOrigData.triangleAttributeTypes[i].attributeType)
		{
			case kQ3AttributeTypeNormal:
				mOrigFaceNormals = static_cast<const TQ3Vector3D*>(
					mOrigData.triangleAttributeTypes[i].data );
				break;
			
			case kQ3AttributeTypeDiffuseColor:
				mOrigFaceColor = static_cast<const TQ3ColorRGB*>(
					mOrigData.triangleAttributeTypes[i].data );
				break;
			
			case kQ3AttributeTypeTransparencyColor:
				mOrigFaceTransparency = static_cast<const TQ3ColorRGB*>(
					mOrigData.triangleAttributeTypes[i].data );
				break;
			
			case kQ3AttributeTypeSpecularColor:
				mOrigFaceSpecularColor = static_cast<const TQ3ColorRGB*>(
					mOrigData.triangleAttributeTypes[i].data );
				break;
		}
	}

	
	for (i = 0; i < mOrigData.numVertexAttributeTypes; ++i)
	{
		switch (mOrigData.vertexAttributeTypes[i].attributeType)
		{
			case kQ3AttributeTypeNormal:
				mOrigVertexNormals = static_cast<const TQ3Vector3D*>(
					mOrigData.vertexAttributeTypes[i].data );
				break;
			
			case kQ3AttributeTypeDiffuseColor:
				mOrigVertexColor = static_cast<const TQ3ColorRGB*>(
					mOrigData.vertexAttributeTypes[i].data );
				break;
			
			case kQ3AttributeTypeTransparencyColor:
				mOrigVertexTransparency = static_cast<const TQ3ColorRGB*>(
					mOrigData.vertexAttributeTypes[i].data );
				break;
			
			case kQ3AttributeTypeSpecularColor:
				mOrigVertexSpecularColor = static_cast<const TQ3ColorRGB*>(
					mOrigData.vertexAttributeTypes[i].data );
				break;
		}
	}
}

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
								TQ3Boolean& outDidChange )
{
	TQ3Status	theStatus = kQ3Success;
	outDidChange = kQ3False;
	E3Memory_Clear( &outData, sizeof(TQ3TriMeshData) );
	
	try
	{
		TriMeshOptimizer	optimizer( inData, outData );
		
		outDidChange = optimizer.Optimize();
	}
	catch (...)
	{
		theStatus = kQ3Failure;
		E3Memory_Clear( &outData, sizeof(TQ3TriMeshData) );
	}
	
	return theStatus;
}


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
TQ3GeometryObject E3TriMesh_Optimize( TQ3GeometryObject inTriMesh )
{
	TQ3GeometryObject	theResult = NULL;
	
	TQ3TriMeshData*	origData = NULL;
	
	if (kQ3Success == Q3TriMesh_LockData( inTriMesh, kQ3True, &origData ))
	{
		TQ3Boolean	didChange = kQ3False;
		TQ3TriMeshData	optData;
		
		if ( (kQ3Success == E3TriMesh_OptimizeData( *origData, optData, didChange )) &&
			(didChange == kQ3True) )
		{
			theResult = Q3TriMesh_New( &optData );
			
			Q3TriMesh_EmptyData( &optData );
		}
	}
	
	return theResult;
}

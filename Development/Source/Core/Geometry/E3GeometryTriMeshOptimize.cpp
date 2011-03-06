/*  NAME:
        E3GeometryTriMeshOptimize.cpp

    DESCRIPTION:
        Functions to optimize a TriMesh for use by the interactive renderer.

    COPYRIGHT:
        Copyright (c) 2005-2011, Quesa Developers. All rights reserved.

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

/*
	DISCUSSION
	
	Due to the needs of OpenGL, color attributes and normal vectors must exist
	on vertices rather than faces or edges.  Therefore we must make vertices
	inherit attributes from the faces or edges that contain them.  However, if a
	vertex belongs to multiple faces of different colors, for instance, we have
	a problem.  We must duplicate the point, so that the point in its role as
	a vertex of one face is considered to be different from the point in its
	role as a vertex of a face of a different color.
	
	Let's adopt some terminology for this discussion and the code below.
	
	A "point" will be a point as provided by the original TriMesh.
	
	A "vertex" will be a point of the new optimized TriMesh.  Since these are
	produced by (at most) duplicating original points, they can be identified by
	indices into the original array of points.
	
	An "instance" will be a point in a specific role, as a corner of a face, as
	an endpoint of an edge, or as a point on its own.  Consequently, an instance
	has an "owner", which consists of a type (face, edge, or point) and an index
	into one of the arrays making up the original TriMesh.
	
	Of course we could simply treat each instance as a vertex, but that would be
	wasteful.  It would produce a TriMesh with many more points than is usually
	necessary, and which could not be accelerated using triangle strips.  Thus,
	when two instances refer to the same point, we must treat them as the same
	vertex unless there is a specific reason not to, such as a color conflict.
*/

namespace
{
	const float		kDegenerateLengthSquared	= 1.0e-12f;
	
	typedef	std::vector< TQ3Vector3D >	VecVec;
	
	typedef std::vector< TQ3Int32 >		IntVec;
	
	const TQ3Vector3D	kDefaultNormal = { 1.0f, 0.0f, 0.0f };
	
	const TQ3ColorRGB	kDefaultDiffuseColor = { 1.0f, 1.0f, 1.0f };
	const TQ3ColorRGB	kDefaultSpecularColor = { 0.5f, 0.5f, 0.5f };
	const TQ3ColorRGB	kDefaultTransparencyColor = { 1.0f, 1.0f, 1.0f };
	
	enum OwnerType
	{
		kOwnerTypeFace,
		kOwnerTypeEdge,
		kOwnerTypePoint
	};
	
	struct Owner
	{
		OwnerType	mType;
		TQ3Int32	mIndex;
	};

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
		bool					AreInstancesSimilar( TQ3Int32 inPt1, TQ3Int32 inPt2 );
		TQ3Int32				FindPrevSimilarInstance( TQ3Int32 inPtInstanceIndex );
		void					FindDistinctVertices();
		void					BuildNewTriMesh();
		void					BuildFaces();
		void					BuildFaceAttributes();
		void					BuildEdges();
		void					BuildEdgeAttributes();
		void					BuildPoints();
		void					BuildVertexAttributes();
		void					ComputeBounds();
		Owner					GetOwnerOfInstance( TQ3Int32 inInstance ) const;
		TQ3Vector3D				GetNormalFromOwner( const Owner& inOwner ) const;
		TQ3ColorRGB				GetDiffColorFromOwner( const Owner& inOwner ) const;
		TQ3ColorRGB				GetSpecColorFromOwner( const Owner& inOwner ) const;
		TQ3ColorRGB				GetTransColorFromOwner( const Owner& inOwner ) const;
	
		const TQ3TriMeshData&	mOrigData;
		TQ3TriMeshData&			mResultData;
		
		const TQ3Vector3D*		mOrigFaceNormals;
		const TQ3ColorRGB*		mOrigFaceColor;
		const TQ3ColorRGB*		mOrigFaceTransparency;
		const TQ3ColorRGB*		mOrigFaceSpecularColor;
		
		const TQ3Vector3D*		mOrigEdgeNormals;
		const TQ3ColorRGB*		mOrigEdgeColor;
		const TQ3ColorRGB*		mOrigEdgeTransparency;
		const TQ3ColorRGB*		mOrigEdgeSpecularColor;
		
		const TQ3Vector3D*		mOrigVertexNormals;
		const TQ3ColorRGB*		mOrigVertexColor;
		const TQ3ColorRGB*		mOrigVertexTransparency;
		const TQ3ColorRGB*		mOrigVertexSpecularColor;
		
		VecVec					mComputedFaceNormals;
		const TQ3Vector3D*		mResultFaceNormals;
		IntVec					mInstanceToPoint;
		IntVec					mPrevPointInstance;
		IntVec					mInstanceToVertex;
		IntVec					mVertexToPoint;
		std::vector<Owner>		mVertexToOwner;
	};
}

TriMeshOptimizer::TriMeshOptimizer(
										const TQ3TriMeshData& inData,
										TQ3TriMeshData& outData )
	: mOrigData( inData )
	, mResultData( outData )
	
	, mOrigFaceNormals( NULL )
	, mOrigFaceColor( NULL )
	, mOrigFaceTransparency( NULL )
	, mOrigFaceSpecularColor( NULL )

	, mOrigEdgeNormals( NULL )
	, mOrigEdgeColor( NULL )
	, mOrigEdgeTransparency( NULL )
	, mOrigEdgeSpecularColor( NULL )

	, mOrigVertexNormals( NULL )
	, mOrigVertexColor( NULL )
	, mOrigVertexTransparency( NULL )
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
		FindDistinctVertices();
		BuildNewTriMesh();
	}
	
	return optNeeded;
}


/*!
	@function	IsOptNeeded
	
	@abstract	Determine whether the TriMesh is missing some vertex attribute
				data that we could synthesize.
*/
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
	
	@abstract	Create an array of "instances", indexes of points playing a
				specific role.
*/
void	TriMeshOptimizer::MakeInstanceToPoint()
{
	mInstanceToPoint.reserve( mOrigData.numTriangles * 3 +
		mOrigData.numEdges * 2 + mOrigData.numPoints );
	
	TQ3Uns32 i;
	
	for (i = 0; i < mOrigData.numTriangles; ++i)
	{
		mInstanceToPoint.push_back( mOrigData.triangles[i].pointIndices[0] );
		mInstanceToPoint.push_back( mOrigData.triangles[i].pointIndices[1] );
		mInstanceToPoint.push_back( mOrigData.triangles[i].pointIndices[2] );
	}
	
	for (i = 0; i < mOrigData.numEdges; ++i)
	{
		mInstanceToPoint.push_back( mOrigData.edges[i].pointIndices[0] );
		mInstanceToPoint.push_back( mOrigData.edges[i].pointIndices[1] );
	}
	
	// Let us only include instances points that do not occur in faces or edges.
	std::vector<bool>	isPointUsedInFaceOrEdge( mOrigData.numPoints, false );
	for (i = 0; i < mInstanceToPoint.size(); ++i)
	{
		isPointUsedInFaceOrEdge[ mInstanceToPoint[i] ] = true;
	}
	
	for (i = 0; i < mOrigData.numPoints; ++i)
	{
		if (! isPointUsedInFaceOrEdge[i])
		{
			mInstanceToPoint.push_back( i );
		}
	}
}


/*!
	@function	GetOwnerOfInstance
	
	@abstract	Given an instance, find the face, edge, or isolated point that
				owns it.
*/
Owner	TriMeshOptimizer::GetOwnerOfInstance( TQ3Int32 inInstance ) const
{
	Owner	theOwner;
	
	if (inInstance < static_cast<TQ3Int32>(mOrigData.numTriangles * 3))
	{
		theOwner.mType = kOwnerTypeFace;
		theOwner.mIndex = inInstance / 3;
	}
	else if (inInstance < static_cast<TQ3Int32>(mOrigData.numTriangles * 3 + mOrigData.numEdges * 2))
	{
		theOwner.mType = kOwnerTypeEdge;
		theOwner.mIndex = (inInstance - mOrigData.numTriangles * 3) / 2;
	}
	else
	{
		theOwner.mType = kOwnerTypePoint;
		theOwner.mIndex = mInstanceToPoint[ inInstance ];
	}
	
	return theOwner;
}

/*!
	@function	GetNormalFromOwner
	
	@abstract	When vertex normals do not exist, inherit normals from owners.
*/
TQ3Vector3D	TriMeshOptimizer::GetNormalFromOwner( const Owner& inOwner ) const
{
	TQ3Vector3D	theNormal = kDefaultNormal;
	
	if (inOwner.mType == kOwnerTypeFace)
	{
		theNormal = mResultFaceNormals[ inOwner.mIndex ];
	}
	else if (inOwner.mType == kOwnerTypeEdge)
	{
		if (mOrigEdgeNormals != NULL)
		{
			theNormal = mOrigEdgeNormals[ inOwner.mIndex ];
		}
	}
	
	return theNormal;
}

/*!
	@function	GetDiffColorFromOwner
	
	@abstract	When vertex colors do not exist, inherit colors from owners.
*/
TQ3ColorRGB	TriMeshOptimizer::GetDiffColorFromOwner( const Owner& inOwner ) const
{
	TQ3ColorRGB	theColor( kDefaultDiffuseColor );
	
	if (inOwner.mType == kOwnerTypeFace)
	{
		theColor = mOrigFaceColor[ inOwner.mIndex ];
	}
	else if (inOwner.mType == kOwnerTypeEdge)
	{
		if (mOrigEdgeColor != NULL)
		{
			theColor = mOrigEdgeColor[ inOwner.mIndex ];
		}
	}

	return theColor;
}

/*!
	@function	GetSpecColorFromOwner
	
	@abstract	When vertex colors do not exist, inherit colors from owners.
*/
TQ3ColorRGB	TriMeshOptimizer::GetSpecColorFromOwner( const Owner& inOwner ) const
{
	TQ3ColorRGB	theColor( kDefaultSpecularColor );
	
	if (inOwner.mType == kOwnerTypeFace)
	{
		theColor = mOrigFaceSpecularColor[ inOwner.mIndex ];
	}
	else if (inOwner.mType == kOwnerTypeEdge)
	{
		if (mOrigEdgeSpecularColor != NULL)
		{
			theColor = mOrigEdgeSpecularColor[ inOwner.mIndex ];
		}
	}

	return theColor;
}

/*!
	@function	GetTransColorFromOwner
	
	@abstract	When vertex colors do not exist, inherit colors from owners.
*/
TQ3ColorRGB	TriMeshOptimizer::GetTransColorFromOwner( const Owner& inOwner ) const
{
	TQ3ColorRGB	theColor( kDefaultTransparencyColor );
	
	if (inOwner.mType == kOwnerTypeFace)
	{
		theColor = mOrigFaceTransparency[ inOwner.mIndex ];
	}
	else if (inOwner.mType == kOwnerTypeEdge)
	{
		if (mOrigEdgeTransparency != NULL)
		{
			theColor = mOrigEdgeTransparency[ inOwner.mIndex ];
		}
	}

	return theColor;
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
		else
		{
			mPrevPointInstance[i] = -1;
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
	@function	AreInstancesSimilar
	
	@abstract	Determine whether two instances of the same point (given by
				indices into mInstanceToPoint) should be treated as the same
				vertex.
*/
bool	TriMeshOptimizer::AreInstancesSimilar( TQ3Int32 inPt1, TQ3Int32 inPt2 )
{
	Q3_ASSERT( mInstanceToPoint[ inPt1 ] == mInstanceToPoint[ inPt2 ] );
	bool	isSame = true;
	
	Owner	owner1( GetOwnerOfInstance( inPt1 ) );
	Owner	owner2( GetOwnerOfInstance( inPt2 ) );
	
	// If vertex normals do not exist, similar instances must have the same
	// face normal.
	if (mOrigVertexNormals == NULL)
	{
		TQ3Vector3D	norm1( GetNormalFromOwner( owner1 ) );
		TQ3Vector3D	norm2( GetNormalFromOwner( owner2 ) );
		
		float	dotProd = Q3FastVector3D_Dot( &norm1, &norm2 );
		
		if (dotProd < 1.0f - FLT_EPSILON)
		{
			isSame = false;
		}
	}
	
	// If the instances seem the same so far, and there are face colors but
	// not vertex colors, then the face colors may distinguish them.
	if ( isSame && (mOrigFaceColor != NULL) && (mOrigVertexColor == NULL) )
	{
		TQ3ColorRGB	diff1( GetDiffColorFromOwner( owner1 ) );
		TQ3ColorRGB	diff2( GetDiffColorFromOwner( owner2 ) );
		isSame = IsSameColor( diff1, diff2 );
	}
	
	// Same for transparency color.
	if ( isSame && (mOrigFaceTransparency != NULL) && (mOrigVertexTransparency == NULL) )
	{
		TQ3ColorRGB	trans1( GetTransColorFromOwner( owner1 ) );
		TQ3ColorRGB	trans2( GetTransColorFromOwner( owner2 ) );
		isSame = IsSameColor( trans1, trans2 );
	}
	
	// Same for specular color.
	if ( isSame && (mOrigFaceSpecularColor != NULL) && (mOrigVertexSpecularColor == NULL) )
	{
		TQ3ColorRGB	spec1( GetSpecColorFromOwner( owner1 ) );
		TQ3ColorRGB	spec2( GetSpecColorFromOwner( owner2 ) );
		isSame = IsSameColor( spec1, spec2 );
	}
	
	return isSame;
}

/*!
	@function	FindPrevSimilarInstance
	
	@abstract	Given a point instance index (index into mInstanceToPoint), find a
				previous instance of the same point that can be considered to
				be the same vertex.
*/
TQ3Int32	TriMeshOptimizer::FindPrevSimilarInstance( TQ3Int32 inPtInstanceIndex )
{
	TQ3Int32	prevSimilarIndex = -1;
	TQ3Int32	prevIndex;
	for (prevIndex = mPrevPointInstance[ inPtInstanceIndex ];
		prevIndex >= 0;
		prevIndex = mPrevPointInstance[ prevIndex ])
	{
		if (prevIndex >= (TQ3Int32)mInstanceToPoint.size())
		{
			break;
		}
		else if (prevIndex < 0)
		{
			break;
		}
		else if (AreInstancesSimilar( inPtInstanceIndex, prevIndex ))
		{
			prevSimilarIndex = prevIndex;
			break;
		}
	}
	return prevSimilarIndex;
}


/*!
	@function	FindDistinctVertices
	
	@abstract	Decide how points need to be duplicated.
	
	@discussion	At the time this function is called, we have a function from the
				set of instances to the set of points, provided by the array
				mInstanceToPoint, and we have a function from instances to
				owners provided by the method GetOwnerOfInstance.
				We need to define the set of vertices by factoring these known
				functions through the vertices.  To be precise, we will define
				arrays mInstanceToVertex, mVertexToPoint, and mVertexToOwner
				such that:
				
				(1) mVertexToPoint[ mInstanceToVertex[i] ] == mInstanceToPoint[i]
				for each i
				
				(2) if i and j are instances that do not map to the same point,
				or do map to the same point but have owners with conflicting
				attributes, then mInstanceToVertex[i] != mInstanceToVertex[j]
				
				(3) mVertexToOwner[ mInstanceToVertex[i] ] == GetOwnerOfInstance(i)
				for each i
*/
void	TriMeshOptimizer::FindDistinctVertices()
{
	const TQ3Int32	kNumInstances = mInstanceToPoint.size();
	mInstanceToVertex.resize( kNumInstances );
	TQ3Int32	i;

	for (i = 0; i < kNumInstances; ++i)
	{
		TQ3Int32	prevSimilar = FindPrevSimilarInstance( i );
		if (prevSimilar < 0)
		{
			// New vertex
			TQ3Int32	nextVertIndex = mVertexToPoint.size();
			mVertexToPoint.push_back( mInstanceToPoint[i] );
			// hence mVertexToPoint[ nextVertIndex ] == mInstanceToPoint[i]
			mInstanceToVertex[i] = nextVertIndex;
			// and now (1) is satisfied
			
			mVertexToOwner.push_back( GetOwnerOfInstance(i) );
		}
		else	// this instance maps to the same vertex as the previous one
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
		
		if (inSrc.attributeType == kQ3AttributeTypeSurfaceShader)
		{
			TQ3Object*	obArray = (TQ3Object*) outDest.data;
			
			for (TQ3Uns32 i = 0; i < inNumElements; ++i)
			{
				if (obArray[i] != NULL)
				{
					Q3Shared_GetReference( obArray[i] );
				}
			}
		}
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
		TQ3Int32	instanceIndex = mOrigData.numTriangles * 3;
		for (TQ3Uns32 i = 0; i < mOrigData.numEdges; ++i)
		{
			mResultData.edges[i].pointIndices[0] =
				mInstanceToVertex[ instanceIndex++ ];
			mResultData.edges[i].pointIndices[1] =
				mInstanceToVertex[ instanceIndex++ ];
		}
	}
}

void	TriMeshOptimizer::BuildEdgeAttributes()
{
	mResultData.numEdgeAttributeTypes = mOrigData.numEdgeAttributeTypes;
	
	if (mResultData.numEdgeAttributeTypes == 0)
	{
		mResultData.edgeAttributeTypes = NULL;
	}
	else
	{
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
}

void	TriMeshOptimizer::BuildPoints()
{
	mResultData.numPoints = mVertexToPoint.size();
	
	if (mResultData.numPoints == 0)
	{
		mResultData.points = NULL;
	}
	else
	{
		mResultData.points = static_cast<TQ3Point3D*>(
			E3Memory_AllocateClear( mResultData.numPoints * sizeof(TQ3Point3D) ) );
		EQ3ThrowIfMemFail_( mResultData.points );
		
		for (TQ3Uns32 i = 0; i < mResultData.numPoints; ++i)
		{
			mResultData.points[i] = mOrigData.points[ mVertexToPoint[i] ];
		}
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

	// Copy original vertex attribute data
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
			vertNormals[j] = GetNormalFromOwner( mVertexToOwner[j] );
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
			vertColors[j] = GetDiffColorFromOwner( mVertexToOwner[j] );
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
			vertTrans[j] = GetTransColorFromOwner( mVertexToOwner[j] );
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
			vertSpecs[j] = GetSpecColorFromOwner( mVertexToOwner[j] );
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

/*!
	@function	FindOrigAtts
	
	@abstract	Locate the arrays of attribute data within the original TriMesh
				data.
*/
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

	
	for (i = 0; i < mOrigData.numEdgeAttributeTypes; ++i)
	{
		switch (mOrigData.edgeAttributeTypes[i].attributeType)
		{
			case kQ3AttributeTypeNormal:
				mOrigEdgeNormals = static_cast<const TQ3Vector3D*>(
					mOrigData.edgeAttributeTypes[i].data );
				break;
			
			case kQ3AttributeTypeDiffuseColor:
				mOrigEdgeColor = static_cast<const TQ3ColorRGB*>(
					mOrigData.edgeAttributeTypes[i].data );
				break;
			
			case kQ3AttributeTypeTransparencyColor:
				mOrigEdgeTransparency = static_cast<const TQ3ColorRGB*>(
					mOrigData.edgeAttributeTypes[i].data );
				break;
			
			case kQ3AttributeTypeSpecularColor:
				mOrigEdgeSpecularColor = static_cast<const TQ3ColorRGB*>(
					mOrigData.edgeAttributeTypes[i].data );
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

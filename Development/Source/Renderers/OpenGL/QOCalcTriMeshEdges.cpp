/*  NAME:
        QOCalcTriMeshEdges.cpp

    DESCRIPTION:
        Source code to compute edges of a TriMesh.

    COPYRIGHT:
        Copyright (c) 2007-2011, Quesa Developers. All rights reserved.

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
#include "QOCalcTriMeshEdges.h"
#include "E3Main.h"

#include <algorithm>


namespace
{
	const TQ3ObjectType	kPropertyTypeEdgeCache	= Q3_OBJECT_TYPE('t', 'm', 'e', 'c');
	
	struct EdgeCacheRec
	{
		TQ3Uns32			edgeCount;
		TQ3Uns32			faceCount;
		TQ3Uns32			editIndex;
		// Followed by:
		// Variable-size array of TQ3EdgeEnds
		// Variable-size array of TQ3TriangleEdges
	};
	
	struct EdgeIndexCompare
	{
				EdgeIndexCompare( const TQ3EdgeEnds* inEdges )
					: mEdges( inEdges ) {}
				EdgeIndexCompare( const EdgeIndexCompare& inOther )
					: mEdges( inOther.mEdges ) {}
		
		bool	operator()( TQ3Uns32 inA, TQ3Uns32 inB ) const
					{
						const TQ3EdgeEnds&	ea( mEdges[inA] );
						const TQ3EdgeEnds&	eb( mEdges[inB] );
						
						return
							(ea.pointIndices[0] < eb.pointIndices[0])
							||
							(
								(ea.pointIndices[0] == eb.pointIndices[0])
								&&
								(ea.pointIndices[1] < eb.pointIndices[1])
							);
					}
		
		const TQ3EdgeEnds*	mEdges;
	};
}


static bool HasSameEnds( const TQ3EdgeEnds& A, const TQ3EdgeEnds& B )
{
	return
		(A.pointIndices[0] == B.pointIndices[0])
		&&
		(A.pointIndices[1] == B.pointIndices[1]);
}



static void SetEdge(
						TQ3Uns32 inStart,
						TQ3Uns32 inEnd,
						TQ3EdgeEnds& outEdge )
{
	TQ3Uns32	lessPt, greaterPt;
	if (inStart < inEnd)
	{
		lessPt = inStart;
		greaterPt = inEnd;
	}
	else
	{
		lessPt = inEnd;
		greaterPt = inStart;
	}
	outEdge.pointIndices[0] = lessPt;
	outEdge.pointIndices[1] = greaterPt;
}

/*!
	@function	CreateRedundantEdges
	@abstract	Create 3 edges for each face.  This will often create multiple
				copies of each physical edge.
*/
static void CreateRedundantEdges(
							const TQ3TriMeshData& inData,
							TQ3EdgeVec& outEdges,
							TQ3TriangleToEdgeVec& outFacesToEdges )
{
	outEdges.resizeNotPreserving( 3 * inData.numTriangles );
	outFacesToEdges.resizeNotPreserving( inData.numTriangles );
	
	TQ3Uns32	edgeNum = 0;
	TQ3Uns32	faceNum = 0;
	
	for ( TQ3Uns32 i = 0; i < inData.numTriangles; ++i )
	{
		TQ3TriangleEdges	theEdges = {
			edgeNum, edgeNum + 1, edgeNum + 2
		};
		outFacesToEdges[ faceNum++ ] = theEdges;
		
		const TQ3TriMeshTriangleData& theFace( inData.triangles[i] );
		SetEdge( theFace.pointIndices[0], theFace.pointIndices[1], outEdges[edgeNum++] );
		SetEdge( theFace.pointIndices[1], theFace.pointIndices[2], outEdges[edgeNum++] );
		SetEdge( theFace.pointIndices[2], theFace.pointIndices[0], outEdges[edgeNum++] );
	}
}

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
									You may pass NULL if you do not need this
									information.
*/
void QOCalcTriMeshEdges( 	const TQ3TriMeshData& inData,
							TQ3EdgeVec& outEdges,
							TQ3TriangleToEdgeVec* outFacesToEdges )
{
	outEdges.clear();
	
	// First create edges and faces without regard to making edges unique.
	TQ3EdgeVec				edgesWithCopies;
	TQ3TriangleToEdgeVec	facesWithCopies;
	
	CreateRedundantEdges( inData, edgesWithCopies, facesWithCopies );
	
	// Make edge indices, and sort the indices by edge ends.
	const TQ3Uns32	kNumEdgesWithCopies = edgesWithCopies.size();
	E3FastArray<TQ3Uns32>	edgeNumVec( kNumEdgesWithCopies );
	TQ3Uns32*	edgeNums = &edgeNumVec[0];	// optimization
	for (TQ3Uns32 i = 0; i < kNumEdgesWithCopies; ++i)
	{
		edgeNums[i] = i;
	}
	EdgeIndexCompare	comparator( &edgesWithCopies[0] );
	std::sort( &edgeNums[0], &edgeNums[kNumEdgesWithCopies], comparator );
	
	// We will construct an array that maps indices of old edges with copies
	// to new uniquified edges.
	E3FastArray<TQ3Uns32>	edgeToUniqueEdge( kNumEdgesWithCopies );
	
	TQ3Uns32	runStart, nextRunStart;
	runStart = nextRunStart = 0;
	
	while (runStart < kNumEdgesWithCopies)
	{
		nextRunStart = runStart + 1;
		while ( (nextRunStart < kNumEdgesWithCopies) &&
			HasSameEnds( edgesWithCopies[ edgeNums[ runStart ] ],
				edgesWithCopies[ edgeNums[ nextRunStart ] ] ) )
		{
			++nextRunStart;
		}
		// At this point, the edges with index runStart to nextRunStart-1 all
		// have the same ends.
		
		// Save one copy of the edge.
		outEdges.push_back( edgesWithCopies[ edgeNums[ runStart ] ] );
		
		// Map old to new indices.
		TQ3Uns32	uniqueEdgeIndex = outEdges.size() - 1;
		for (TQ3Uns32 j = runStart; j < nextRunStart; ++j)
		{
			edgeToUniqueEdge[ edgeNums[j] ] = uniqueEdgeIndex;
		}
		
		runStart = nextRunStart;
	}
	
	// Create faces with new indices.
	if (outFacesToEdges != NULL)
	{
		const TQ3Uns32 kNumFaces = facesWithCopies.size();
		outFacesToEdges->resizeNotPreserving( kNumFaces );
		for (TQ3Uns32 f = 0; f < kNumFaces; ++f)
		{
			const TQ3TriangleEdges& oldFace( facesWithCopies[f] );
			TQ3TriangleEdges	newFace = {
				{
					edgeToUniqueEdge[ oldFace.edgeIndices[ 0 ] ],
					edgeToUniqueEdge[ oldFace.edgeIndices[ 1 ] ],
					edgeToUniqueEdge[ oldFace.edgeIndices[ 2 ] ]
				}
			};
			(*outFacesToEdges)[f] = newFace;
		}
	}
}


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
							E3FastArray<char>& ioScratchBuffer,
							TQ3EdgeVec& outEdges,
							TQ3TriangleToEdgeVec& outFacesToEdges )
{
	bool	haveCachedData = false;
	TQ3Uns32	geomEdits = Q3Shared_GetEditIndex( inGeom );
	int			propSize;
	const char*	propData = reinterpret_cast<const char*>(
		inGeom->GetPropertyAddress( kPropertyTypeEdgeCache ) );
	
	if (propData != NULL)
	{
		const EdgeCacheRec*	cacheData = reinterpret_cast<const EdgeCacheRec*>( propData );
		if (cacheData->editIndex == geomEdits)
		{
			outEdges.resizeNotPreserving( cacheData->edgeCount );
			E3Memory_Copy( propData + sizeof(EdgeCacheRec), &outEdges[0],
				cacheData->edgeCount * sizeof(TQ3EdgeEnds) );
			
			outFacesToEdges.resizeNotPreserving( cacheData->faceCount );
			E3Memory_Copy( propData + sizeof(EdgeCacheRec) +
				cacheData->edgeCount * sizeof(TQ3EdgeEnds),
				&outFacesToEdges[0],
				cacheData->faceCount * sizeof(TQ3TriangleEdges) );
			
			haveCachedData = true;
		}
	}
	
	if (! haveCachedData)
	{
		TQ3TriMeshData*	tmData = NULL;
		Q3TriMesh_LockData( inGeom, kQ3True, &tmData );
		
		QOCalcTriMeshEdges( *tmData, outEdges, &outFacesToEdges );
		
		Q3TriMesh_UnlockData( inGeom );
		
		propSize = sizeof(EdgeCacheRec) +
			outEdges.size() * sizeof(TQ3EdgeEnds) +
			outFacesToEdges.size() * sizeof(TQ3TriangleEdges);
		if (ioScratchBuffer.size() < propSize)
		{
			ioScratchBuffer.resizeNotPreserving( propSize );
		}
		EdgeCacheRec*	cacheData = reinterpret_cast<EdgeCacheRec*>( &ioScratchBuffer[0] );
		cacheData->editIndex = geomEdits;
		cacheData->edgeCount = outEdges.size();
		cacheData->faceCount = outFacesToEdges.size();
		E3Memory_Copy( &outEdges[0], &ioScratchBuffer[0] + sizeof(EdgeCacheRec),
				cacheData->edgeCount * sizeof(TQ3EdgeEnds) );
		E3Memory_Copy( &outFacesToEdges[0],
			&ioScratchBuffer[0] + sizeof(EdgeCacheRec) +
			cacheData->edgeCount * sizeof(TQ3EdgeEnds),
			cacheData->faceCount * sizeof(TQ3TriangleEdges) );
		Q3Object_SetProperty( inGeom, kPropertyTypeEdgeCache, propSize, cacheData );
		Q3Shared_SetEditIndex( inGeom, geomEdits );
	}
}


/*!
	@function	QOAccessCachedTriMeshEdges
	@abstract	Get read-only access to edge data cached in a TriMesh property.
	@discussion	If the cached data is present and not stale, it is simply
				returned as the output.  Otherwise, it is computed using
				E3CalcTriMeshEdges and cached.
				
				Although this function returns data in the same kind of parameters
				as QOGetCachedTriMeshEdges, this function returns arrays that
				do not own their data and hence may not be modified.
	@param		inGeom				A TriMesh object.
	@param		ioScratchBuffer		A buffer for temporary use.
	@param		outEdges			Receives array of edges.
	@param		outFacesToEdges		Receives array mapping faces to edges.
*/
void QOAccessCachedTriMeshEdges( TQ3GeometryObject inGeom,
							E3FastArray<char>& ioScratchBuffer,
							TQ3EdgeVec& outEdges,
							TQ3TriangleToEdgeVec& outFacesToEdges )
{
	bool	haveCachedData = false;
	TQ3Uns32	geomEdits = Q3Shared_GetEditIndex( inGeom );
	int			propSize;
	const char*	propData = reinterpret_cast<const char*>(
		inGeom->GetPropertyAddress( kPropertyTypeEdgeCache ) );
	
	if (propData != NULL)
	{
		const EdgeCacheRec*	cacheData = reinterpret_cast<const EdgeCacheRec*>( propData );
		if (cacheData->editIndex == geomEdits)
		{
			outEdges.SetUnownedData( cacheData->edgeCount,
				reinterpret_cast<const TQ3EdgeEnds*>( propData + sizeof(EdgeCacheRec) ) );
			
			outFacesToEdges.SetUnownedData( cacheData->faceCount,
				reinterpret_cast<const TQ3TriangleEdges*>(
					propData + sizeof(EdgeCacheRec) +
					cacheData->edgeCount * sizeof(TQ3EdgeEnds) ) );

			haveCachedData = true;
		}
	}
	
	if (! haveCachedData)
	{
		TQ3EdgeVec				computedEdges;
		TQ3TriangleToEdgeVec	computedFacesToEdges;

		TQ3TriMeshData*	tmData = NULL;
		Q3TriMesh_LockData( inGeom, kQ3True, &tmData );
		
		QOCalcTriMeshEdges( *tmData, computedEdges, &computedFacesToEdges );
		
		Q3TriMesh_UnlockData( inGeom );

		propSize = sizeof(EdgeCacheRec) +
			computedEdges.size() * sizeof(TQ3EdgeEnds) +
			computedFacesToEdges.size() * sizeof(TQ3TriangleEdges);
		if (ioScratchBuffer.size() < propSize)
		{
			ioScratchBuffer.resizeNotPreserving( propSize );
		}
		EdgeCacheRec*	cacheData = reinterpret_cast<EdgeCacheRec*>( &ioScratchBuffer[0] );
		cacheData->editIndex = geomEdits;
		cacheData->edgeCount = computedEdges.size();
		cacheData->faceCount = computedFacesToEdges.size();
		E3Memory_Copy( &computedEdges[0], &ioScratchBuffer[0] + sizeof(EdgeCacheRec),
				cacheData->edgeCount * sizeof(TQ3EdgeEnds) );
		E3Memory_Copy( &computedFacesToEdges[0],
			&ioScratchBuffer[0] + sizeof(EdgeCacheRec) +
			cacheData->edgeCount * sizeof(TQ3EdgeEnds),
			cacheData->faceCount * sizeof(TQ3TriangleEdges) );
		Q3Object_SetProperty( inGeom, kPropertyTypeEdgeCache, propSize, cacheData );
		Q3Shared_SetEditIndex( inGeom, geomEdits );
		
		propData = reinterpret_cast<const char*>(
			inGeom->GetPropertyAddress( kPropertyTypeEdgeCache ) );

		outEdges.SetUnownedData( cacheData->edgeCount,
			reinterpret_cast<const TQ3EdgeEnds*>( propData + sizeof(EdgeCacheRec) ) );
		
		outFacesToEdges.SetUnownedData( cacheData->faceCount,
			reinterpret_cast<const TQ3TriangleEdges*>(
				propData + sizeof(EdgeCacheRec) +
				cacheData->edgeCount * sizeof(TQ3EdgeEnds) ) );
	}
}

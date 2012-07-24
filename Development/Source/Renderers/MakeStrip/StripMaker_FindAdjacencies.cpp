/*  NAME:
        StripMaker_FindAdjacencies.cpp

    DESCRIPTION:
        Source for Quesa triangle strip maker.
    
    REMARKS:
    	This triangle strip maker is based on an article by Pierre Terdiman at
    	<http://www.codercorner.com/Strips.htm>, but is not based on his source
    	code.
		    
    COPYRIGHT:
        Copyright (c) 2007-2012, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://quesa.sourceforge.net/>
        
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
#include "StripMaker.h"

#include <algorithm>


//=============================================================================
//      Local Types
//-----------------------------------------------------------------------------

namespace StripMaker
{
	/*!
		@struct		Edge
		@abstract	An edge.  Used to compute face adjacencies.
		@field		vertex		Vertex indices in increasing order.
		@field		ownerFace	Index of face from which we obtained the edge.
		@field		isFlipped	True if the order of vertices here is the
								opposite of the orientation order of the
								vertices in the owner face.
		@field		edgeIndex	Which edge of the owner, 0, 1, or 2.
	*/
	struct Edge
	{
		TQ3Uns32	vertex[2];
		TQ3Uns32	ownerFace;
		bool		isFlipped;
		TQ3Uns32	edgeIndex;
	};
	
	/*!
		@typedef	EdgeVec
		@abstract	Vector of edges.
	*/
	typedef std::vector< Edge >	EdgeVec;


//=============================================================================
//      Local Functions
//-----------------------------------------------------------------------------

	static bool operator<( const Edge& A, const Edge& B )
	{
		return
			(A.vertex[0] < B.vertex[0])
			||
			(
				(A.vertex[0] == B.vertex[0])
				&&
				(
					(A.vertex[1] < B.vertex[1])
					||
					(
						(A.vertex[1] == B.vertex[1])
						&&
						(A.isFlipped < B.isFlipped)
					)
				)
			);
	}
}

using namespace StripMaker;

static bool HasSameEnds( const Edge& A, const Edge& B )
{
	return
		(A.vertex[0] == B.vertex[0])
		&&
		(A.vertex[1] == B.vertex[1]);
}

static void MakeEdge(
					TQ3Uns32 inFaceIndex,
					TQ3Uns32 inEdgeIndex,
					TQ3Uns32 inStart,
					TQ3Uns32 inEnd,
					EdgeVec& ioEdges )
{
	Edge	theEdge;
	theEdge.ownerFace = inFaceIndex;
	theEdge.edgeIndex = inEdgeIndex;
	
	if (inStart < inEnd)
	{
		theEdge.vertex[0] = inStart;
		theEdge.vertex[1] = inEnd;
		theEdge.isFlipped = false;
	}
	else
	{
		theEdge.vertex[0] = inEnd;
		theEdge.vertex[1] = inStart;
		theEdge.isFlipped = true;
	}
	ioEdges.push_back( theEdge );
}

static void CreateEdges( const FaceVec& inFaces, EdgeVec& outEdges )
{
	outEdges.clear();
	outEdges.reserve( 3 * inFaces.size() );
	const TQ3Uns32	kNumFaces = static_cast<TQ3Uns32>(inFaces.size());
	
	for (TQ3Uns32 i = 0; i < kNumFaces; ++i)
	{
		const Face& theFace( inFaces[i] );
		MakeEdge( i, 2, theFace.vertex[0], theFace.vertex[1], outEdges );
		MakeEdge( i, 0, theFace.vertex[1], theFace.vertex[2], outEdges );
		MakeEdge( i, 1, theFace.vertex[2], theFace.vertex[0], outEdges );
	}
}

static void MakeAdjacent( const Edge& inA, const Edge& inB, FaceVec& ioFaces )
{
	Face&	faceA( ioFaces[ inA.ownerFace ] );
	Face&	faceB( ioFaces[ inB.ownerFace ] );
	
	faceA.adjFace[ inA.edgeIndex ] = inB.ownerFace;
	faceA.adjEdge[ inA.edgeIndex ] = inB.edgeIndex;
	
	faceB.adjFace[ inB.edgeIndex ] = inA.ownerFace;
	faceB.adjEdge[ inB.edgeIndex ] = inA.edgeIndex;
}

	/*!
		@function	FindAdjacencies
		@abstract	Initialize the adjFace and adjEdge links in a vector of
					faces.
	*/
void StripMaker::FindAdjacencies( FaceVec& ioFaces )
{
	EdgeVec	theEdges;
	CreateEdges( ioFaces, theEdges );
	std::sort( theEdges.begin(), theEdges.end() );
	
	const TQ3Uns32 kNumEdges = static_cast<TQ3Uns32>(theEdges.size());
	TQ3Uns32	runStart, nextRunStart;
	runStart = nextRunStart = 0;
	
	while (runStart < kNumEdges)
	{
		nextRunStart = runStart + 1;
		while ( (nextRunStart < kNumEdges) &&
			HasSameEnds( theEdges[runStart], theEdges[nextRunStart] ) )
		{
			++nextRunStart;
		}
		// At this point, the edges with index runStart to nextRunStart-1 all
		// have the same ends.  The ones with isFlipped == true will be
		// grouped at the end of that run.
		TQ3Uns32	pairFirst = runStart;
		TQ3Uns32	pairLast = nextRunStart-1;
		while ( (pairFirst < pairLast) &&
			(theEdges[pairFirst].isFlipped != theEdges[pairLast].isFlipped) )
		{
			MakeAdjacent( theEdges[pairFirst], theEdges[pairLast], ioFaces );
			
			++pairFirst;
			--pairLast;
		}
		runStart = nextRunStart;
	}
}


/*  NAME:
        StripMaker_MakeSimpleStrip.cpp

    DESCRIPTION:
        Source for Quesa triangle strip maker.
    
    REMARKS:
    	This triangle strip maker is based on an article by Pierre Terdiman at
    	<http://www.codercorner.com/Strips.htm>, but is not based on his source
    	code.
		    
    COPYRIGHT:
        Copyright (c) 2007-2010, Quesa Developers. All rights reserved.

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

using namespace StripMaker;

//=============================================================================
//      Functions
//-----------------------------------------------------------------------------


#if Q3_DEBUG
TQ3Uns32 StripMaker::StripSize( const IndVec& inStrip )
{
	TQ3Uns32 theCount = 0;
	const TQ3Uns32 kIndexCount = inStrip.size();
	
	for (TQ3Uns32 i = 0; (i+2) < kIndexCount; ++i)
	{
		if ( (inStrip[i] != inStrip[i+1]) && (inStrip[i+1] != inStrip[i+2]) )
		{
			theCount += 1;
		}
	}
	return theCount;
}
#endif


static void MakeForwardStrip(
				TQ3Uns32 inStartVert,
				TQ3Uns32 inStartFace,
				const FaceVec& inFaces,
				FreeFaceSet& ioFreeFaces,
				IndVec& outStrip )
{
	outStrip.clear();
	const Face*	curFace( &inFaces[inStartFace] );
	outStrip.push_back( curFace->vertex[inStartVert] );
	outStrip.push_back( curFace->vertex[(inStartVert + 1) % 3] );
	outStrip.push_back( curFace->vertex[(inStartVert + 2) % 3] );
	
	TQ3Uns32 outEdgeNum = inStartVert;
	bool	addEdge = false;
	TQ3Uns32	nextFaceNum = curFace->adjFace[ outEdgeNum ];
	
	while ( (nextFaceNum != kInvalidIndex) &&
		ioFreeFaces.IsFree( nextFaceNum ) )
	{
		TQ3Uns32	inEdgeNum = curFace->adjEdge[ outEdgeNum ];
		curFace = &inFaces[ nextFaceNum ];
		outStrip.push_back( curFace->vertex[ inEdgeNum ] );
		ioFreeFaces.SetUsed( nextFaceNum );
		if (addEdge)
		{
			outEdgeNum = (inEdgeNum + 1) % 3;
		}
		else
		{
			outEdgeNum = (inEdgeNum + 2) % 3;
		}
		addEdge = ! addEdge;
		nextFaceNum = curFace->adjFace[ outEdgeNum ];
	}
}

static void MakeReverseStrip(
				TQ3Uns32 inStartVert,
				TQ3Uns32 inStartFace,
				const FaceVec& inFaces,
				FreeFaceSet& ioFreeFaces,
				IndVec& outStrip )
{
	outStrip.clear();
	const Face*	curFace( &inFaces[inStartFace] );
	TQ3Uns32 outEdgeNum = (inStartVert + 2) % 3;
	bool	addEdge = true;
	TQ3Uns32	nextFaceNum = curFace->adjFace[ outEdgeNum ];
	
	while ( (nextFaceNum != kInvalidIndex) &&
		ioFreeFaces.IsFree( nextFaceNum ) )
	{
		TQ3Uns32	inEdgeNum = curFace->adjEdge[ outEdgeNum ];
		curFace = &inFaces[ nextFaceNum ];
		outStrip.push_back( curFace->vertex[ inEdgeNum ] );
		ioFreeFaces.SetUsed( nextFaceNum );
		if (addEdge)
		{
			outEdgeNum = (inEdgeNum + 1) % 3;
		}
		else
		{
			outEdgeNum = (inEdgeNum + 2) % 3;
		}
		addEdge = ! addEdge;
		nextFaceNum = curFace->adjFace[ outEdgeNum ];
	}
}

static void MakeWholeStrip( const IndVec& inForward,
							const IndVec& inReverse,
							IndVec& outWhole )
{
	outWhole.clear();
	const TQ3Uns32	kForwardSize = inForward.size();
	const TQ3Uns32	kReverseSize = inReverse.size();
	
	if ( (kReverseSize % 2) == 0 )	// even length
	{
		outWhole.reserve( kForwardSize + kReverseSize );
	}
	else	// odd
	{
		outWhole.reserve( kForwardSize + kReverseSize + 1 );
		outWhole.push_back( inReverse.back() );
	}
	
	// append reverse of inReverse
	outWhole.insert( outWhole.end(), inReverse.rbegin(), inReverse.rend() );
	
	// append inForward
	outWhole.insert( outWhole.end(), inForward.begin(), inForward.end() );
}

/*!
	@function	MakeSimpleStrip
	@abstract	Given a starting point, extend it in both directions to
				make a maximal strip with no degenerate faces except
				possibly at the beginning.
	@param		inStartVert		Which vertex (0, 1, or 2) of the starting
								face should start the strip.
	@param		inStartFace		Index of the starting face.
	@param		inFaces			All of the faces.
	@param		ioFreeFaces		The set of faces not yet used in strips.
								When the function returns, this will be
								updated to remove faces of the new strip.
	@param		ioScratch1		Scratch vector.
	@param		ioScratch2		Scratch vector.
	@param		outStrip		Receives the new strip.
*/
void StripMaker::MakeSimpleStrip(
				TQ3Uns32 inStartVert,
				TQ3Uns32 inStartFace,
				const FaceVec& inFaces,
				FreeFaceSet& ioFreeFaces,
				IndVec& ioScratch1,
				IndVec& ioScratch2,
				IndVec& outStrip )
{
	ioFreeFaces.SetUsed( inStartFace );
	
	MakeForwardStrip( inStartVert, inStartFace, inFaces, ioFreeFaces,
		ioScratch1 );
		
	MakeReverseStrip( inStartVert, inStartFace, inFaces, ioFreeFaces,
		ioScratch2 );
	
	MakeWholeStrip( ioScratch1, ioScratch2, outStrip );
}

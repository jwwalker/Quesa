/*  NAME:
        MakeStrip.cpp

    DESCRIPTION:
        Source for Quesa triangle strip maker.
    
    REMARKS:
    	This triangle strip maker is based on an article by Pierre Terdiman at
    	<http://www.codercorner.com/Strips.htm>, but is not based on his source
    	code.
		    
    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

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

#include "MakeStrip.h"

#include "StripMaker.h"

#include <algorithm>

using namespace StripMaker;

//=============================================================================
//      Local Functions
//-----------------------------------------------------------------------------

/*!
	@function	InitFreeFaceIndices
	@abstract	Initialize a set containing indices of all faces.
*/
static void InitFreeFaceIndices( TQ3Uns32 inNumFaces, FaceSet& outFreeSet )
{
	outFreeSet.resize( inNumFaces, 1 );
}


/*!
	@function	NextFreeFace
	@abstract	Choose a face that has not yet been used.
*/
static bool	NextFreeFace( const FaceSet& inFreeSet, TQ3Uns32& ioFaceIndex )
{
	bool	foundFree = false;
	
	TQ3Uns32	faceIndex = kInvalidIndex;
	FaceSet::const_iterator i = std::find( inFreeSet.begin() + ioFaceIndex,
		inFreeSet.end(), 1 );
	if (i != inFreeSet.end())
	{
		ioFaceIndex = i - inFreeSet.begin();
		foundFree= true;
	}
	return foundFree;
}

//=============================================================================
//      Public Functions
//-----------------------------------------------------------------------------

/*!
	@function	MakeStrip
	
	@abstract	Make a triangle strip, respecting the original orientations of
				the faces.
				
	@discussion	The set of oriented faces is to be given as a single array.
				For example, the array { 7, 4, 5, 0, 3, 2, 6, 1, 9 } represents
				the 3 faces (7, 4, 5), (0, 3, 2), and (6, 1, 9).
				
				In order that there can be a single strip with correct
				orientations, it is likely that the strip contains some
				degenerate triangles, i.e., triangles in which the 3 vertex
				indices are not all distinct.
	
	@param		inNumFaces	Number of faces in following array.
	@param		inFaces		Vertex indices of faces.  The size of this array
							must be 3 times inNumFaces.
	@param		outStrip	Vertex indices of a triangle strip.
*/
void	MakeStrip(
					TQ3Uns32 inNumFaces,
					const TQ3Uns32* inFaces,
					std::vector<TQ3Uns32>& outStrip )
{
	outStrip.clear();
	outStrip.reserve( 3 * inNumFaces );
	FaceVec	theFaces;
	InitFaces( inNumFaces, inFaces, theFaces );
	FindAdjacencies( theFaces );
	
	FaceSet	freeFaces, free0, free1, free2, freeBest;
	IndVec	strip0, strip1, strip2, stripBest, scratch1, scratch2;
	InitFreeFaceIndices( inNumFaces, freeFaces );
	TQ3Uns32	startFaceIndex = 0;
	
	
	// Pick a free face
	while ( NextFreeFace( freeFaces, startFaceIndex ) )
	{
		// Pick one of 3 starting vertices of this face.
		// Each choice will produce a certain path and a certain set of
		// remaining faces.
		free0 = freeFaces;
		free1 = freeFaces;
		free2 = freeFaces;

		MakeSimpleStrip( 0, startFaceIndex, theFaces, free0, scratch1,
			scratch2, strip0 );
		MakeSimpleStrip( 1, startFaceIndex, theFaces, free1, scratch1,
			scratch2, strip1 );
		MakeSimpleStrip( 2, startFaceIndex, theFaces, free2, scratch1,
			scratch2, strip2 );
		
		// Pick the longest strip.
		if ( (strip0.size() >= strip1.size()) &&
			(strip0.size() >= strip2.size()) )
		{
			stripBest.swap( strip0 );
			freeBest.swap( free0 );
		}
		else if (strip1.size() >= strip2.size())
		{
			stripBest.swap( strip1 );
			freeBest.swap( free1 );
		}
		else
		{
			stripBest.swap( strip2 );
			freeBest.swap( free2 );
		}
		
		JoinStrips( outStrip, stripBest );
		
		freeFaces.swap( freeBest );
	}
}

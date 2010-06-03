/*  NAME:
       StripMaker.h

    DESCRIPTION:
        Header for Quesa triangle strip maker.
    
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

#include "E3Prefix.h"

#include <vector>

namespace StripMaker
{
//=============================================================================
//      Constants
//-----------------------------------------------------------------------------

	/*!
		@constant	kInvalidIndex
		@abstract	Special invalid value for index to a face or edge.
	*/
	const TQ3Uns32	kInvalidIndex		= 0xFFFFFFFFU;
	
//=============================================================================
//      Types
//-----------------------------------------------------------------------------
	
	/*!
		class		FreeFaceSet
		@abstract	Object representing a set of face indices.
		@discussion	Originally, a FreeFaceSet was just a naked vector of TQ3Uns8
					flags.  Profiling MakeStrip showed a substantial amount of
					time being spent copying face sets using operator=.  In this
					class, only an interval within the vector is really defined
					and copied.  Indices less than mFirstMaybeFree are assumed
					to be used, and indices greater or equal to mFirstAllFree
					are assumed to be free.
	*/
	class FreeFaceSet
	{
	public:
						FreeFaceSet( TQ3Uns32 inFaceCount );
						FreeFaceSet( const FreeFaceSet& inOther );
					
		void			swap( FreeFaceSet& ioOther );
		
		FreeFaceSet&	operator=( const FreeFaceSet& inOther );
		
		TQ3Uns32		FindNextFree();
		
		bool			IsFree( TQ3Uns32 inIndex ) const;
		void			SetUsed( TQ3Uns32 inIndex );
	
	private:
		std::vector<TQ3Uns8>	mFreeFlags;
		TQ3Uns32				mFirstMaybeFree;
		TQ3Uns32				mFirstAllFree;
	};
	
	/*!
		@typedef	IndVec
		@abstract	Vector of indices.
	*/
	typedef	std::vector< TQ3Uns32 >		IndVec;
	
	/*!
		@struct		Face
		@abstract	A face (triangle) and its adjacency.
		@field		vertex	The vertex indices in their original order.
		@field		adjFace	The index of the face adjacent to a certain edge.
							We consider the edges to be in the order
							0 = (1, 2), 1 = (2, 0), 2 = (0, 1).  Initially
							kInvalidIndex.
		@field		adjEdge	If adjFace[i] is not kInvalidIndex, then adjEdge[i]
							indicates which edge of that face leads back to
							this face.
	*/
	struct	Face
	{
		TQ3Uns32	vertex[3];
		TQ3Uns32	adjFace[3];
		TQ3Uns32	adjEdge[3];
	};

	/*!
		@typedef	FaceVec
		@abstract	Vector of faces.
	*/
	typedef std::vector< Face >	FaceVec;
	
//=============================================================================
//      Function declarations
//-----------------------------------------------------------------------------

	/*!
		@function	InitFaces
		@abstract	Initialize a vector of faces with given vertex numbers,
					but using kInvalidIndex for all face and edge links.
	*/
	void InitFaces(
					TQ3Uns32 inNumFaces,
					const TQ3Uns32* inFaces,
					FaceVec& outFaces );

	/*!
		@function	FindAdjacencies
		@abstract	Initialize the adjFace and adjEdge links in a vector of
					faces.
	*/
	void FindAdjacencies( FaceVec& ioFaces );
	
	
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
	void MakeSimpleStrip(
					TQ3Uns32 inStartVert,
					TQ3Uns32 inStartFace,
					const FaceVec& inFaces,
					FreeFaceSet& ioFreeFaces,
					IndVec& ioScratch1,
					IndVec& ioScratch2,
					IndVec& outStrip );
	
	/*!
		@function	JoinStrips
		@abstract	Joint two oriented triangle strips to make one big one,
					by inserting extra vertices that make degenerate triangles.
		@param		ioA				On input, first strip; on output, joined
									strip.
		@param		inB				Second strip.
	*/
	void JoinStrips( IndVec& ioA, const IndVec& inB );
}

/*  NAME:
        StripMaker_InitFaces.cpp

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

#include "StripMaker.h"

//=============================================================================
//      Functions
//-----------------------------------------------------------------------------

	/*!
		@function	InitFaces
		@abstract	Initialize a vector of faces with given vertex numbers,
					but using kInvalidIndex for all face and edge links.
	*/
void StripMaker::InitFaces(
					TQ3Uns32 inNumFaces,
					const TQ3Uns32* inFaces,
					FaceVec& outFaces )
{
	outFaces.resize( inNumFaces );
	
	for (TQ3Uns32 i = 0; i < inNumFaces; ++i)
	{
		Face&	theFace( outFaces[i] );
		theFace.vertex[0] = inFaces[ 3*i ];
		theFace.vertex[1] = inFaces[ 3*i + 1 ];
		theFace.vertex[2] = inFaces[ 3*i + 2 ];
		theFace.adjFace[0] = kInvalidIndex;
		theFace.adjFace[1] = kInvalidIndex;
		theFace.adjFace[2] = kInvalidIndex;
		theFace.adjEdge[0] = kInvalidIndex;
		theFace.adjEdge[1] = kInvalidIndex;
		theFace.adjEdge[2] = kInvalidIndex;
	}
}

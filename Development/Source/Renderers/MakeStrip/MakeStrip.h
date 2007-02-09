/*  NAME:
       MakeStrip.h

    DESCRIPTION:
        Header for Quesa triangle strip maker.
    
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
#ifndef	MAKESTRIP_HDR
#define	MAKESTRIP_HDR

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"

#include <vector>



//=============================================================================
//      Function declarations
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
					std::vector<TQ3Uns32>& outStrip );

#endif	// MAKESTRIP_HDR

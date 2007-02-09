/*  NAME:
        StripMaker_JoinStrips.cpp

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
	@function	JoinStrips
	@abstract	Joint two oriented triangle strips to make one big one,
				by inserting extra vertices that make degenerate triangles.
	@param		ioA				On input, first strip; on output, joined
								strip.
	@param		inB				Second strip.
*/
void StripMaker::JoinStrips( IndVec& ioA, const IndVec& inB )
{
	// If either one is empty, we can just concatenate the two and be done.
	if ( ioA.empty() || inB.empty() )
	{
		ioA.insert( ioA.end(), inB.begin(), inB.end() );
	}
	else	// both nonempty
	{
		// Duplicate its last vertex of A.
		ioA.push_back( ioA.back() );
		
		// special case when second strip starts with a duplicated vertex
		if ( (inB.size() >= 2) && (inB[0] == inB[1]) )
		{
			if ( (ioA.size() % 2) == 1 )	// 1st strip was even (now odd)
			{
				ioA.push_back( ioA.back() );
			}
		}
		else // usual case
		{
			if ( (ioA.size() % 2) == 0 )	// 1st strip was odd (now even)
			{
				ioA.push_back( ioA.back() );
			}
			ioA.push_back( inB.front() );
		}
		
		// Append B
		ioA.insert( ioA.end(), inB.begin(), inB.end() );
	}
}

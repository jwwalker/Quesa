#pragma once
/*  NAME:
        ParseVRML.h

    DESCRIPTION:
        Header file for ParseVRML.cp.

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

#include "PolyValue.h"
#include <string>
#include <iosfwd>


/*!
	@function	ParseVRML
	
	@abstract	Parse VRML, producing an array of nodes in PolyValue dictionary format.
	
	@discussion	This first phase of VRML parsing does not know much about VRML
				semantics.  It does not distinguish between VRML 1 and VRML 2,
				and does not know the differences between specific node types.
				
				In order to avoid backtracking, the grammar allows some things
				that are not allowed in VRML.  For instance, it allows an array
				to contain any mix of values, whereas VRML really only allows
				an array of numbers, an array of strings, or an array of nodes.

	@param		inVRMLText		The VRML input data.
	@param		outNodes		Receives an array of dictionaries.
	@param		ioDebugStream	Receives text describing the parsing process.
								You may pass NULL if you do not want this.
*/
bool	ParseVRML( const std::string& inVRMLText, PolyValue::PolyVec& outNodes,
				std::ostream* ioDebugStream );


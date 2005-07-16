#pragma once
/*  NAME:
        VRMLSkipParser.h

    DESCRIPTION:
        Header file for VRMLSkipParser.cp.

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

#undef check
#pragma warn_unusedarg	off
#include "boost/spirit.hpp"
#pragma warn_unusedarg	reset

/*
	A skip parser is used to skip characters that are not to be parsed.
	In the case of VRML, this means skipping white space, commas, and
	comments.

	To optimize my skip parser, I was originally using the technique
	described at <http://www.boost.org/libs/spirit/doc/techniques.html>.
	However, I ran into a bug in CodeWarrior's Mach-O compiler, so I wrote a
	bare-bones skip parser that does not use a grammar or sub_grammar.
	This is actually faster.
*/

namespace VRML_reader
{
	using namespace boost::spirit;
	
	struct Skipper : parser<Skipper>
	{
		typedef		scanner<const char *,
						scanner_policies< no_skipper_iteration_policy<
							skip_parser_iteration_policy<Skipper, iteration_policy> >,
							match_policy, action_policy> >
					ScannerT;
		
		bool parse( const ScannerT& scan ) const;
	};
}

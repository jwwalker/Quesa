/*  NAME:
        UnBackslashEscape.cp

    DESCRIPTION:
       Utility for string literals.

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
#include "UnBackslashEscape.h"

#include <string>

/*!
	@function	UnBackslashEscape
	
	@abstract	Given a string in which a backslash is used to escape
				double quotes and backslashes, find the original
				string.
	
	@param		ioString		String to transform.
*/
void	UnBackslashEscape( std::string& ioString )
{
	// If there are no backslashes, which will probably be common,
	// we want to bail out fast.
	std::string::size_type	backslashLoc = ioString.find('\\');
	if (backslashLoc != std::string::npos)
	{
		std::string	resultStr;
		bool	sawBackslash = false;
		for (std::string::iterator i = ioString.begin(); i != ioString.end(); ++i)
		{
			if (sawBackslash)
			{
				resultStr += *i;
				sawBackslash = false;
			}
			else if (*i == '\\')
			{
				sawBackslash = true;
			}
			else
			{
				resultStr += *i;
			}
		}
		ioString.swap( resultStr );
	}
}

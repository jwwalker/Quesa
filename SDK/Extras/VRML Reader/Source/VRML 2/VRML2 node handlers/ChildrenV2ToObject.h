#pragma once
/*  NAME:
        ChildrenV2ToObject.h

    DESCRIPTION:
        Header file for ChildrenV2ToObject.cp.

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


#include "CQ3ObjectRef.h"
class PolyValue;
class CVRMLReader;

/*!
	@function	ChildrenV2ToObject
	
	@abstract	Look for a "children" field in the given node, and convert that
				to a Quesa DisplayGroup object.

	@discussion	The "children" field should contain either a single node or a
				bracketed array of nodes.  If it is a single node,  or if the
				array only contains one node, we will not bother to create a
				DisplayGroup, but simply return the Quesa version of the child.
	
	@param		ioNode			Node to convert.
	@param		inReader		The reader object.
	@param		outMadeGroup	Receives a flag indicating whether the operation
								made a group of more than one child.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	ChildrenV2ToObject( PolyValue& ioNode,
									CVRMLReader& inReader,
									bool& outMadeGroup );

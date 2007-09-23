/*  NAME:
        NodeV2ToObject.cp

    DESCRIPTION:
        Main function to convert a VRML 2 node to a Quesa object.

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
#include "NodeV2ToObject.h"

#include "CVRMLReader.h"
#include "GetCachedObject.h"
#include "GroupV2ToObject.h"
#include "PolyValue.h"
#include "ShapeV2ToObject.h"
#include "SwitchV2ToObject.h"
#include "TransformV2ToObject.h"

#include <ostream>

/*!
	@function	NodeV2ToObject
	
	@abstract	Attempt to convert a VRML 2 node to a Quesa object.
	
	@discussion	The reason that the node is not passed as a const reference is
				that whenever we successfully produce a Quesa object from a
				node, we cache that object in the node.  This is not only a
				speed optimization, it is also necessary to guarantee that we
				will have multiple Quesa references whenever there are multiple
				references to a VRML node.
	
	@param		ioNode		Node to convert.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	NodeV2ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	CQ3ObjectRef	theObject;
	
	if (ioNode.GetType() == PolyValue::kDataTypeDictionary)
	{
		theObject = GetCachedObject( ioNode );
		
		if (not theObject.isvalid())
		{
			PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );
			const PolyValue&	nodeTypeValue( theDict["[type]"] );
			const std::string&	nodeType( nodeTypeValue.GetString() );
			
			if (nodeType == "Shape")
			{
				theObject = ShapeV2ToObject( ioNode, inReader );
			}
			else if (nodeType == "Transform")
			{
				theObject = TransformV2ToObject( ioNode, inReader );
			}
			else if ( (nodeType == "Group") or (nodeType == "Anchor") )
			{
				theObject = GroupV2ToObject( ioNode, inReader );
			}
			else if (nodeType == "Switch")
			{
				theObject = SwitchV2ToObject( ioNode, inReader );
			}
			else if (inReader.GetDebugStream() != NULL)
			{
				*inReader.GetDebugStream() << "No handler for node of type '" <<
					nodeType << "'." << std::endl;
			}
		}
	}
	
	return theObject;
}

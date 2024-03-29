/*  NAME:
        SwitchV2ToObject.h

    DESCRIPTION:
         VRML 2 node handler.

    COPYRIGHT:
        Copyright (c) 2007-2009, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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

#include "SwitchV2ToObject.h"

#include "ChildrenV2ToObject.h"
#include "GetCachedObject.h"
#include "GetObjectName.h"
#include "IsKeyPresent.h"
#include "PolyValue.h"
#include "SetGroupName.h"

	#include <Quesa/Q3GroupIterator.h>

/*!
	@function	SwitchV2ToObject
	
	@abstract	Attempt to convert a VRML 2 Switch node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	SwitchV2ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	bool	madeGroup;
	CQ3ObjectRef	theObject( ChildrenV2ToObject( ioNode, inReader, madeGroup,
		"choice" ) );
	
	if (theObject.isvalid())
	{
		PolyValue::Dictionary&	nodeDict( ioNode.GetDictionary() );
		
		SetGroupName( theObject, nodeDict );
		
		SetCachedObject( ioNode, theObject );
		
		// A switch node should display whichever child is indicated by the
		// whichChoice index.  The default index is -1, meaning nothing is shown.
		int	whichChoice = -1;
		if (IsKeyPresent( nodeDict, "whichChoice" ))
		{
			PolyValue&	whichChoiceValue( nodeDict["whichChoice"] );
			if (whichChoiceValue.IsNumber())
			{
				whichChoice = whichChoiceValue.GetInt();
			}
		}
		
		if ( whichChoice < 0 )
		{
			theObject = CQ3ObjectRef();	// nothing shown
		}
		else if (madeGroup)
		{
			CQ3ObjectRef	switchGroup( theObject );
			theObject = CQ3ObjectRef();
			int		childIndex = 0;
			Q3GroupIterator		iter( switchGroup.get() );
			CQ3ObjectRef	theChild;
			while ( (theChild = iter.NextObject()).isvalid() )
			{
				if (childIndex == whichChoice)
				{
					theObject = theChild;
					break;
				}
				++childIndex;
			}
		}
		else	// single child
		{
			if (whichChoice > 0)
			{
				theObject = CQ3ObjectRef();	// nothing shown
			}
		}
	}
	
	return theObject;
}

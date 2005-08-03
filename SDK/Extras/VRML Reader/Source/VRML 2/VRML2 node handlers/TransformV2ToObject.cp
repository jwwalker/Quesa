/*  NAME:
        TransformV2ToObject.cp

    DESCRIPTION:
        VRML 2 node handler.

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
#include "TransformV2ToObject.h"

#include "ChildrenV2ToObject.h"
#include "GetCachedObject.h"
#include "IsKeyPresent.h"
#include "MakeTransform.h"
#include "PolyValue.h"
#include "PrependObjectToGroup.h"
#include "VRML-reader-prefix.h"

#include <QuesaGroup.h>
#include <QuesaCustomElements.h>


/*!
	@function	TransformV2ToObject
	
	@abstract	Attempt to convert a VRML 2 Transform node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	TransformV2ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	bool	madeGroup;
	CQ3ObjectRef	theObject( ChildrenV2ToObject( ioNode, inReader, madeGroup ) );
		
	if (theObject.isvalid())
	{
		CQ3ObjectRef	theTransform = MakeTransform( ioNode.GetDictionary() );
		if (theTransform.isvalid())
		{
			// ChildrenV2ToObject may have produced a single child object
			// rather than a group.   If so, we need to make a group.
			if (not madeGroup)
			{
				CQ3ObjectRef	newGroup( Q3DisplayGroup_New() );
				ThrowIfNullQuesaOb_( newGroup );
				Q3Group_AddObject( newGroup.get(), theObject.get() );
				theObject = newGroup;
			}
			PrependObjectToGroup( theTransform, theObject );
		}
		
		// If this node was named with DEF, set that as the name of the
		// Quesa object.
		if (IsKeyPresent( ioNode.GetDictionary(), "[name]" ))
		{
			PolyValue&	nameValue( ioNode.GetDictionary()["[name]"] );
			const std::string&	theName( nameValue.GetString() );
			::CENameElement_SetData( theObject.get(), theName.c_str() );
		}
		
		SetCachedObject( ioNode, theObject );
	}
	
	return theObject;
}

/*  NAME:
        TransformSeparatorV1ToObject.cp

    DESCRIPTION:
        VRML 1 node handler.

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
#include "TransformSeparatorV1ToObject.h"

#include "CVRMLReader.h"
#include "GetCachedObject.h"
#include "IsKeyPresent.h"
#include "NodeV1ToObject.h"
#include "PolyValue.h"
#include "VRML-reader-prefix.h"

#if __MACH__
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/QuesaCustomElements.h>
#else
	#include <QuesaGroup.h>
	#include <QuesaCustomElements.h>
#endif



/*!
	@function	TransformSeparatorV1ToObject
	
	@abstract	Attempt to convert a VRML 1 TransformSeparator node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	TransformSeparatorV1ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	CQ3ObjectRef	theGroup;
	
	PolyValue::Dictionary&	nodeDict( ioNode.GetDictionary() );
	if (IsKeyPresent( nodeDict, "children" ))
	{
		PolyValue&	childrenNode( nodeDict["children"] );
		if (childrenNode.GetType() == PolyValue::kDataTypeArray)
		{
			PolyValue::PolyVec&	childVec( childrenNode.GetPolyVec() );
			
			if (not childVec.empty())
			{
				theGroup = CQ3ObjectRef( Q3DisplayGroup_New() );
				ThrowIfNullQuesaOb_( theGroup );
				
				for (PolyValue::PolyVec::iterator i = childVec.begin();
						i != childVec.end(); ++i)
				{
					CQ3ObjectRef	aChild( NodeV1ToObject( *i, inReader ) );
					if (aChild.isvalid())
					{
						Q3Group_AddObject( theGroup.get(), aChild.get() );
					}
				}

				// If this Separator was named with DEF, set that as the name of the
				// Quesa object.
				if (IsKeyPresent( nodeDict, "[name]" ))
				{
					PolyValue&	nameValue( nodeDict["[name]"] );
					const std::string&	theName( nameValue.GetString() );
					::CENameElement_SetData( theGroup.get(), theName.c_str() );
				}
			}
		}
	}
	
	return theGroup;
}


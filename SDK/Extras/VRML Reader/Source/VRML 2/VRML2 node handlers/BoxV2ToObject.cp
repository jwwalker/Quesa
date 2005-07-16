/*  NAME:
        BoxV2ToObject.cp

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
#include "BoxV2ToObject.h"

#include "IsKeyPresent.h"
#include "PolyValue.h"

#include <QuesaGeometry.h>

/*!
	@function	BoxV2ToObject
	
	@abstract	Attempt to convert a VRML 2 Box node to a Quesa object.
	
	@param		ioNode		Node to convert.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	BoxV2ToObject( PolyValue& ioNode )
{
	TQ3Vector3D		sizes = {
		2.0f, 2.0f, 2.0f
	};
	
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );

	if (IsKeyPresent( theDict, "size" ))
	{
		PolyValue&	sizeField( theDict["size"] );
		
		if ( (sizeField.GetType() == PolyValue::kDataTypeArrayOfFloat) or
			(sizeField.GetType() == PolyValue::kDataTypeArrayOfInt) )
		{
			PolyValue::FloatVec&	theFloats( sizeField.GetFloatVec() );
			if (theFloats.size() == 3)
			{
				sizes.x = theFloats[0];
				sizes.y = theFloats[1];
				sizes.z = theFloats[2];
			}
		}
	}
	
	TQ3BoxData boxData = {
		{ -sizes.x/2, -sizes.y/2, -sizes.z/2 },
		{ 0.0f, sizes.y, 0.0f },
		{ 0.0f, 0.0f, sizes.z },
		{ sizes.x, 0.0f, 0.0f },
		NULL, NULL
	};
	
	CQ3ObjectRef	theObject( Q3Box_New( &boxData ) );
	
	return theObject;
}

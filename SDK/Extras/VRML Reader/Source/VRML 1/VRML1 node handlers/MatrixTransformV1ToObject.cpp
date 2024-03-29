/*  NAME:
        MatrixTransformV1ToObject.cp

    DESCRIPTION:
        VRML 1 node handler.

    COPYRIGHT:
        Copyright (c) 2005, Quesa Developers. All rights reserved.

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
#include "MatrixTransformV1ToObject.h"

#include "IsKeyPresent.h"
#include "GetCachedObject.h"
#include "PolyValue.h"

	#include <Quesa/QuesaTransform.h>

/*!
	@function	MatrixTransformV1ToObject
	
	@abstract	Attempt to convert a VRML 1 MatrixTransform node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	MatrixTransformV1ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
#pragma unused( inReader )
	CQ3ObjectRef	theTransform;
	
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );

	if (IsKeyPresent( theDict, "matrix" ))
	{
		PolyValue&	transValue( theDict["matrix"] );
		if (transValue.IsNumberVec())
		{
			PolyValue::FloatVec& matVec( transValue.GetFloatVec() );
			if (matVec.size() == 16)
			{
				TQ3Matrix4x4	theMatrix = {{
					static_cast<float>(matVec[0]), static_cast<float>(matVec[1]), static_cast<float>(matVec[2]), static_cast<float>(matVec[3]),
					static_cast<float>(matVec[4]), static_cast<float>(matVec[5]), static_cast<float>(matVec[6]), static_cast<float>(matVec[7]),
					static_cast<float>(matVec[8]), static_cast<float>(matVec[9]), static_cast<float>(matVec[10]), static_cast<float>(matVec[11]),
					static_cast<float>(matVec[12]), static_cast<float>(matVec[13]), static_cast<float>(matVec[14]), static_cast<float>(matVec[15])
				}};
				theTransform = CQ3ObjectRef( Q3MatrixTransform_New( &theMatrix ) );
				
				if (theTransform.isvalid())
				{
					SetCachedObject( ioNode, theTransform );
				}
			}
		}
	}
	
	return theTransform;
}

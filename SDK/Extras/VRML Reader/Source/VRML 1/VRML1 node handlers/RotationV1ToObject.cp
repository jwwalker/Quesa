#include "RotationV1ToObject.h"

#include "CVRMLReader.h"
#include "IsKeyPresent.h"
#include "GetCachedObject.h"
#include "PolyValue.h"

#include <QuesaMath.h>
/*  NAME:
        RotationV1ToObject.cp

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
#include <QuesaTransform.h>

#include <ostream>

/*!
	@function	RotationV1ToObject
	
	@abstract	Attempt to convert a VRML 1 Rotation node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	RotationV1ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
#pragma unused( inReader )
	CQ3ObjectRef	theTransform;
	
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );

	if (IsKeyPresent( theDict, "rotation" ))
	{
		PolyValue&	transValue( theDict["rotation"] );
		if (transValue.IsNumberVec())
		{
			PolyValue::FloatVec& transVec( transValue.GetFloatVec() );
			if (transVec.size() == 4)
			{
				TQ3RotateAboutAxisTransformData	rotData;
				rotData.origin.x = 0.0f;
				rotData.origin.y = 0.0f;
				rotData.origin.z = 0.0f;
				rotData.orientation.x = transVec[0];
				rotData.orientation.y = transVec[1];
				rotData.orientation.z = transVec[2];
				rotData.radians = transVec[3];
				
				// The axis should be normalized
				float	axisLen = Q3FastVector3D_Length( &rotData.orientation );
				if (axisLen > FLT_EPSILON)
				{
					Q3FastVector3D_Scale( &rotData.orientation, 1.0f/axisLen,
						&rotData.orientation );
					
					theTransform = CQ3ObjectRef( Q3RotateAboutAxisTransform_New( &rotData ) );
					
					if (theTransform.isvalid())
					{
						SetCachedObject( ioNode, theTransform );
					}
				}
				else if (inReader.GetDebugStream() != NULL)
				{
					*inReader.GetDebugStream() << "Rotation node has zero axis!" << std::endl;
				}
			}
		}
	}
	
	return theTransform;
}

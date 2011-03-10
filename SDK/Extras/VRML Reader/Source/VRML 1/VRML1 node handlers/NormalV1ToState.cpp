/*  NAME:
        NormalV1ToState.cp

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
#include "NormalV1ToState.h"

#include "CVRMLReader.h"
#include "GetArrayFromNodeField.h"
#include "IsKeyPresent.h"
#include "PolyValue.h"
#include "SVRML1State.h"

/*!
	@function	NormalV1ToState
	
	@abstract	Read a VRML 1 Normal node and use it to modify the
				current state.
	
	@param		ioNode		Node to convert.
	@param		ioReader	The reader object.
*/
void	NormalV1ToState( PolyValue& ioNode, CVRMLReader& ioReader )
{
	SVRML1State&	curState( ioReader.GetVRML1State() );
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );

	if (IsKeyPresent( theDict, "vector" ))
	{
		const PolyValue::FloatVec&	normVec( theDict["vector"].GetFloatVec() );
		
		if ( (normVec.size() % 3) == 0 )
		{
			std::vector<TQ3Vector3D>	theNorms;
			FloatVecToStructureVec( normVec, theNorms );
			curState.normal.swap( theNorms );
		}
	}	
}

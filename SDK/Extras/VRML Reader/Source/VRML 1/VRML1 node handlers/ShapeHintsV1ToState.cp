/*  NAME:
        ShapeHintsV1ToState.cp

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
#include "ShapeHintsV1ToState.h"

#include "CVRMLReader.h"
#include "IsKeyPresent.h"
#include "PolyValue.h"
#include "SVRML1State.h"
#include "VRML_1_constants.h"


/*!
	@function	ShapeHintsV1ToState
	
	@abstract	Read a VRML 1 ShapeHints node and use it to modify the
				current state.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
*/
void	ShapeHintsV1ToState( PolyValue& ioNode, CVRMLReader& ioReader )
{
	int	vertexOrder = eVRML1VertexOrdering_UNKNOWN_ORDERING;
	bool isSolid = false;
	bool isConvex = true;
	float	creaseAngle = 0.5f;
	
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );
	
	if (IsKeyPresent( theDict, "vertexOrdering" ))
	{
		vertexOrder = theDict["vertexOrdering"].GetInt();
	}
	
	if (IsKeyPresent( theDict, "shapeType" ))
	{
		int	shapeEnum = theDict["shapeType"].GetInt();
		isSolid = (shapeEnum == eVRML1ShapeType_SOLID);
	}
	
	if (IsKeyPresent( theDict, "faceType" ))
	{
		int	faceEnum = theDict["faceType"].GetInt();
		isConvex = (faceEnum == eVRML1FaceType_CONVEX);
	}
	
	if (IsKeyPresent( theDict, "creaseAngle" ))
	{
		creaseAngle = theDict["creaseAngle"].GetFloat();
	}
	
	SVRML1State&	curState( ioReader.GetVRML1State() );
	
	curState.vertexOrdering = vertexOrder;
	curState.creaseAngle = creaseAngle;
	curState.solidShapeType = isSolid;
	curState.convexFaceType = isConvex;
}

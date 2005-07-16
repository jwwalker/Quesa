/*  NAME:
        ConeV2ToObject.cp

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
#include "ConeV2ToObject.h"


#include "GetCachedObject.h"
#include "IsKeyPresent.h"
#include "PolyValue.h"

#include <QuesaGeometry.h>


/*!
	@function	ConeV2ToObject
	
	@abstract	Attempt to convert a VRML 2 Cone node to a Quesa object.
	
	@param		ioNode		Node to convert.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	ConeV2ToObject( PolyValue& ioNode )
{
	CQ3ObjectRef	theCone;
	
	bool	hasBottom = true;
	bool	hasSide = true;
	float	height = 2.0f;
	float	radius = 1.0f;
	
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );
	
	if (IsKeyPresent( theDict, "bottom" ))
	{
		hasBottom = theDict["bottom"].GetBool();
	}
	
	if (IsKeyPresent( theDict, "side" ))
	{
		hasSide = theDict["side"].GetBool();
	}
	
	if (IsKeyPresent( theDict, "height" ))
	{
		height = theDict["height"].GetFloat();
	}
	
	if (IsKeyPresent( theDict, "bottomRadius" ))
	{
		radius = theDict["bottomRadius"].GetFloat();
	}
	
	if (hasSide)
	{
		TQ3ConeData coneData = {
			{ 0.0f, -height/2, 0.0f },
			{ 0.0f, height, 0.0f },
			{ 0.0f, 0.0f, radius },
			{ radius, 0.0f, 0.0f },
			0.0f, 1.0f, 0.0f, 1.0f,
			(hasBottom? kQ3EndCapMaskBottom : 0),
			NULL, NULL, NULL, NULL
		};
		
		theCone = CQ3ObjectRef( Q3Cone_New( &coneData ) );
	}
	else // no side, just bottom disk
	{
		TQ3DiskData	bottomData = {
			{ 0.0f, -height/2, 0.0f },
			{ radius, 0.0f, 0.0f },
			{ 0.0f, 0.0f, radius },
			0.0f, 1.0f, 0.0f, 1.0f,
			NULL
		};
		theCone = CQ3ObjectRef( Q3Disk_New( &bottomData ) );
	}

	return theCone;
}


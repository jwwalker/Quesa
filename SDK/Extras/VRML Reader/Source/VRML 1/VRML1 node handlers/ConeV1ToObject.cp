/*  NAME:
        ConeV1ToObject.cp

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
#include "ConeV1ToObject.h"

#include "CVRMLReader.h"
#include "IsKeyPresent.h"
#include "GetIndexedMaterial.h"
#include "PolyValue.h"
#include "SVRML1State.h"
#include "VRML_1_constants.h"

#if __MACH__
	#include <Quesa/QuesaGeometry.h>
#else
	#include <QuesaGeometry.h>
#endif

/*!
	@function	ConeV1ToObject
	
	@abstract	Attempt to convert a VRML 1 Cone node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	ConeV1ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	CQ3ObjectRef	theCone;
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );
	
	float	radius = 1.0f;
	if (IsKeyPresent( theDict, "bottomRadius" ))
	{
		radius = theDict["bottomRadius"].GetFloat();
	}
	
	float	height = 2.0f;
	if (IsKeyPresent( theDict, "height" ))
	{
		height = theDict["height"].GetFloat();
	}
	
	int		partsMask = eVRML1Parts_ALL;
	if (IsKeyPresent( theDict, "parts" ))
	{
		partsMask = theDict["parts"].GetInt();
	}
	
	bool	hasBottom = ((partsMask & eVRML1Parts_BOTTOM) != 0);
	
	int	curMaterialBinding = inReader.GetVRML1State().materialBinding;
	bool	isMultiColored = (curMaterialBinding == eVRML1Value_PER_PART) or
		(curMaterialBinding == eVRML1Value_PER_PART_INDEXED);
	
	if ((partsMask & eVRML1Parts_SIDES) != 0)
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
		
		CQ3ObjectRef	sideColor, bottomColor, allColor;
		if (isMultiColored)
		{
			sideColor = GetIndexedMaterial( inReader, 0 );
			bottomColor = GetIndexedMaterial( inReader, 1 );
			
			coneData.faceAttributeSet = sideColor.get();
			coneData.bottomAttributeSet = bottomColor.get();
		}
		else
		{
			allColor = GetIndexedMaterial( inReader, 0 );
			coneData.coneAttributeSet = allColor.get();
		}
		
		theCone = CQ3ObjectRef( Q3Cone_New( &coneData ) );
	}
	else if (hasBottom) // no side, just bottom disk
	{
		CQ3ObjectRef	diskColor = GetIndexedMaterial( inReader,
			isMultiColored? 1 : 0 );
		TQ3DiskData	bottomData = {
			{ 0.0f, -height/2, 0.0f },
			{ radius, 0.0f, 0.0f },
			{ 0.0f, 0.0f, radius },
			0.0f, 1.0f, 0.0f, 1.0f,
			diskColor.get()
		};
		theCone = CQ3ObjectRef( Q3Disk_New( &bottomData ) );
	}
	
	return theCone;
}

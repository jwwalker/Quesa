/*  NAME:
        CylinderV1ToObject.cp

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
#include "CylinderV1ToObject.h"

#include "CVRMLReader.h"
#include "IsKeyPresent.h"
#include "GetIndexedMaterial.h"
#include "PolyValue.h"
#include "SVRML1State.h"
#include "VRML_1_constants.h"
#include "VRML-reader-prefix.h"

#include <QuesaGeometry.h>
#include <QuesaGroup.h>

/*!
	@function	CylinderV1ToObject
	
	@abstract	Attempt to convert a VRML 1 Cylinder node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	CylinderV1ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	CQ3ObjectRef	theCylinder;
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );
	
	
	float	radius = 1.0f;
	if (IsKeyPresent( theDict, "radius" ))
	{
		radius = theDict["radius"].GetFloat();
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
	bool	hasTop = ((partsMask & eVRML1Parts_TOP) != 0);
	bool	hasSides = ((partsMask & eVRML1Parts_SIDES) != 0);
	
	int	curMaterialBinding = inReader.GetVRML1State().materialBinding;
	bool	isMultiColored = (curMaterialBinding == eVRML1Value_PER_PART) or
		(curMaterialBinding == eVRML1Value_PER_PART_INDEXED);
	
	TQ3CylinderData	cylData = {
		{ 0.0f, -height/2, 0.0f },
		{ 0.0f, height, 0.0f },
		{ 0.0f, 0.0f, radius },
		{ radius, 0.0f, 0.0f },
		0.0f, 1.0f, 0.0f, 1.0f,
		0, NULL, NULL, NULL, NULL, NULL
	};
	CQ3ObjectRef	sideColor, bottomColor, topColor, allColor;
	
	if (isMultiColored)
	{
		sideColor = GetIndexedMaterial( inReader, 0 );
		topColor = GetIndexedMaterial( inReader, 1 );
		bottomColor = GetIndexedMaterial( inReader, 2 );
	}
	else
	{
		allColor = GetIndexedMaterial( inReader, 0 );
	}
	
	if (hasSides)
	{
		if (isMultiColored)
		{
			cylData.topAttributeSet = topColor.get();
			cylData.faceAttributeSet = sideColor.get();
			cylData.bottomAttributeSet = bottomColor.get();
		}
		else
		{
			cylData.cylinderAttributeSet = allColor.get();
		}
		
		if (hasTop)
		{
			cylData.caps |= kQ3EndCapMaskTop;
		}
		
		if (hasBottom)
		{
			cylData.caps |= kQ3EndCapMaskBottom;
		}
		
		theCylinder = CQ3ObjectRef( Q3Cylinder_New( &cylData ) );
	}
	else if (hasTop or hasBottom)
	{
		theCylinder = CQ3ObjectRef( Q3DisplayGroup_New() );
		ThrowIfNullQuesaOb_( theCylinder );

		if (hasTop)
		{
			TQ3DiskData	topData = {
				{ 0.0f, height/2, 0.0f },
				{ 0.0f, 0.0f, radius },
				{ radius, 0.0f, 0.0f },
				0.0f, 1.0f, 0.0f, 1.0f,
				NULL
			};
			if (isMultiColored)
			{
				topData.diskAttributeSet = topColor.get();
			}
			else
			{
				topData.diskAttributeSet = allColor.get();
			}
			CQ3ObjectRef	topDisk( Q3Disk_New( &topData ) );
			ThrowIfNullQuesaOb_( topDisk );
			Q3Group_AddObject( theCylinder.get(), topDisk.get() );
		}
		
		if (hasBottom)
		{
			TQ3DiskData	bottomData = {
				{ 0.0f, -height/2, 0.0f },
				{ radius, 0.0f, 0.0f },
				{ 0.0f, 0.0f, radius },
				0.0f, 1.0f, 0.0f, 1.0f,
				NULL
			};
			if (isMultiColored)
			{
				bottomData.diskAttributeSet = bottomColor.get();
			}
			else
			{
				bottomData.diskAttributeSet = allColor.get();
			}
			CQ3ObjectRef	bottomDisk( Q3Disk_New( &bottomData ) );
			ThrowIfNullQuesaOb_( bottomDisk );
			Q3Group_AddObject( theCylinder.get(), bottomDisk.get() );
		}
	}
	
	return theCylinder;
}


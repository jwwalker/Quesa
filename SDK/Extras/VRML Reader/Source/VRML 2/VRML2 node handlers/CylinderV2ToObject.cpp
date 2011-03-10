/*  NAME:
        CylinderV2ToObject.cp

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
#include "CylinderV2ToObject.h"

#include "IsKeyPresent.h"
#include "PolyValue.h"
#include "VRML-reader-prefix.h"

#if __MACH__
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaGroup.h>
#else
	#include <QuesaGeometry.h>
	#include <QuesaGroup.h>
#endif

/*!
	@function	CylinderV2ToObject
	
	@abstract	Attempt to convert a VRML 2 Cylinder node to a Quesa object.
	
	@param		ioNode		Node to convert.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	CylinderV2ToObject( PolyValue& ioNode )
{
	CQ3ObjectRef	theCyl;
	
	bool	hasBottom = true;
	bool	hasTop = true;
	bool	hasSide = true;
	float	height = 2.0f;
	float	radius = 1.0f;
	
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );
	
	if (IsKeyPresent( theDict, "bottom" ))
	{
		hasBottom = theDict["bottom"].GetBool();
	}
	
	if (IsKeyPresent( theDict, "top" ))
	{
		hasTop = theDict["top"].GetBool();
	}
	
	if (IsKeyPresent( theDict, "side" ))
	{
		hasSide = theDict["side"].GetBool();
	}
	
	if (IsKeyPresent( theDict, "height" ))
	{
		height = theDict["height"].GetFloat();
	}
	
	if (IsKeyPresent( theDict, "radius" ))
	{
		radius = theDict["radius"].GetFloat();
	}
	
	if (hasSide)
	{
		TQ3CylinderData	cylData = {
			{ 0.0f, -height/2, 0.0f },
			{ 0.0f, height, 0.0f },
			{ 0.0f, 0.0f, radius },
			{ radius, 0.0f, 0.0f },
			0.0f, 1.0f, 0.0f, 1.0f,
			(hasTop? kQ3EndCapMaskTop : 0) | (hasBottom? kQ3EndCapMaskBottom : 0),
			NULL, NULL, NULL, NULL, NULL
		};
		
		theCyl = CQ3ObjectRef( Q3Cylinder_New( &cylData ) );
	}
	else if (hasTop or hasBottom)	// if no sides, it is just disks
	{
		theCyl = CQ3ObjectRef( Q3DisplayGroup_New() );
		ThrowIfNullQuesaOb_( theCyl );
		
		if (hasTop)
		{
			TQ3DiskData	topData = {
				{ 0.0f, height/2, 0.0f },
				{ 0.0f, 0.0f, radius },
				{ radius, 0.0f, 0.0f },
				0.0f, 1.0f, 0.0f, 1.0f,
				NULL
			};
			CQ3ObjectRef	topDisk( Q3Disk_New( &topData ) );
			ThrowIfNullQuesaOb_( topDisk );
			Q3Group_AddObject( theCyl.get(), topDisk.get() );
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
			CQ3ObjectRef	bottomDisk( Q3Disk_New( &bottomData ) );
			ThrowIfNullQuesaOb_( bottomDisk );
			Q3Group_AddObject( theCyl.get(), bottomDisk.get() );
		}
	}
	
	return theCyl;
}

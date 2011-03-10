/*  NAME:
        CubeV1ToObject.cp

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
#include "CubeV1ToObject.h"

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
	@function	CubeV1ToObject
	
	@abstract	Attempt to convert a VRML 1 Cube node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	CubeV1ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	CQ3ObjectRef	theCube;
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );
	
	float	width = 2.0f;
	float	height = 2.0f;
	float	depth = 2.0f;
	
	if (IsKeyPresent( theDict, "width" ))
	{
		width = theDict["width"].GetFloat();
	}
	if (IsKeyPresent( theDict, "height" ))
	{
		height = theDict["height"].GetFloat();
	}
	if (IsKeyPresent( theDict, "depth" ))
	{
		depth = theDict["depth"].GetFloat();
	}
	
	int	curMaterialBinding = inReader.GetVRML1State().materialBinding;
	bool	isMultiColored = (curMaterialBinding == eVRML1Value_PER_PART) or
		(curMaterialBinding == eVRML1Value_PER_PART_INDEXED) or
		(curMaterialBinding == eVRML1Value_PER_FACE) or
		(curMaterialBinding == eVRML1Value_PER_FACE_INDEXED);

	TQ3BoxData boxData = {
		{ -width/2, -height/2, -depth/2 },
		{ 0.0f, height, 0.0f },
		{ 0.0f, 0.0f, depth },
		{ width, 0.0f, 0.0f },
		NULL, NULL
	};
	
	if (isMultiColored)
	{
		CQ3ObjectRef	faceAttHolders[6];
		faceAttHolders[0] = GetIndexedMaterial( inReader, 4 );
		faceAttHolders[1] = GetIndexedMaterial( inReader, 5 );
		faceAttHolders[2] = GetIndexedMaterial( inReader, 0 );
		faceAttHolders[3] = GetIndexedMaterial( inReader, 1 );
		faceAttHolders[4] = GetIndexedMaterial( inReader, 3 );
		faceAttHolders[5] = GetIndexedMaterial( inReader, 2 );
		TQ3AttributeSet	faceAtts[6] = {
			faceAttHolders[0].get(),
			faceAttHolders[1].get(),
			faceAttHolders[2].get(),
			faceAttHolders[3].get(),
			faceAttHolders[4].get(),
			faceAttHolders[5].get()
		};
		boxData.faceAttributeSet = faceAtts;
		theCube = CQ3ObjectRef( Q3Box_New( &boxData ) );
	}
	else
	{
		CQ3ObjectRef	overallMaterial( GetIndexedMaterial( inReader, 0 ) );
		boxData.boxAttributeSet = overallMaterial.get();
		theCube = CQ3ObjectRef( Q3Box_New( &boxData ) );
	}
	
	return theCube;
}

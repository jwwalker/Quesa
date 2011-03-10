/*  NAME:
        PointSetV1ToObject.cp

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
#include "PointSetV1ToObject.h"

#include "CVRMLReader.h"
#include "GetArrayFromNodeField.h"
#include "GetIndexedMaterial.h"
#include "GetIntVecFromField.h"
#include "IsKeyPresent.h"
#include "PolyValue.h"
#include "SVRML1State.h"
#include "VRML_1_constants.h"
#include "VRML-reader-prefix.h"

#if __MACH__
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/QuesaSet.h>
	#include <Quesa/QuesaShader.h>
#else
	#include <QuesaGeometry.h>
	#include <QuesaGroup.h>
	#include <QuesaSet.h>
	#include <QuesaShader.h>
#endif

/*!
	@function	PointSetV1ToObject
	
	@abstract	Attempt to convert a VRML 1 PointSet node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	PointSetV1ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	CQ3ObjectRef	theObject;
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );

	int	startIndex = 0;
	if (IsKeyPresent( theDict, "startIndex" ))
	{
		startIndex = theDict["startIndex"].GetInt();
	}
	
	int numPoints = -1;
	if (IsKeyPresent( theDict, "numPoints" ))
	{
		numPoints = theDict["numPoints"].GetInt();
	}
	
	const SVRML1State&	theState( inReader.GetVRML1State() );
	const std::vector<TQ3Point3D>&	thePoints( theState.coordinate3 );
	const std::vector<TQ3ColorRGB>&	theColors( theState.diffuseColor );
	
	if ( (startIndex < thePoints.size()) and (numPoints != 0) )
	{
		theObject = CQ3ObjectRef( Q3DisplayGroup_New() );
		ThrowIfNullQuesaOb_( theObject );
		
		int		endIndex = (numPoints < 0)? thePoints.size() :
			std::min<int>( startIndex + numPoints, thePoints.size() );

		// In VRML 2, points are not lit.  I will take that easy way
		// instead of worrying about normals.
		CQ3ObjectRef	theShader( Q3NULLIllumination_New() );
		ThrowIfNullQuesaOb_( theShader );
		Q3Group_AddObject( theObject.get(), theShader.get() );
		
		CQ3ObjectRef	materialAtts( GetIndexedMaterial( inReader, 0 ) );
		
		bool	isColorPerVertex = not ((theState.materialBinding == eVRML1Value_OVERALL) or
			(theState.materialBinding == eVRML1Value_DEFAULT));
		int	i;
		
		if (isColorPerVertex and (theColors.size() >= endIndex) )
		{
			for (i = startIndex; i < endIndex; ++i)
			{
				CQ3ObjectRef	attSet( Q3Object_Duplicate( materialAtts.get() ) );
				TQ3PointData	pointData = {
					thePoints[i],
					attSet.get()
				};
				Q3AttributeSet_Add( attSet.get(), kQ3AttributeTypeDiffuseColor,
					&theColors[i] );
				CQ3ObjectRef	point( Q3Point_New( &pointData ) );
				ThrowIfNullQuesaOb_( point );
				Q3Group_AddObject( theObject.get(), point.get() );
			}
		}
		else
		{
			for (i = startIndex; i < endIndex; ++i)
			{
				TQ3PointData	pointData2 = {
					thePoints[i], materialAtts.get()
				};
				CQ3ObjectRef	point2( Q3Point_New( &pointData2 ) );
				ThrowIfNullQuesaOb_( point2 );
				Q3Group_AddObject( theObject.get(), point2.get() );
			}
		}
	}
	
	return theObject;
}

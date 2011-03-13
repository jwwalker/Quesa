/*  NAME:
        PointSetV2ToObject.cp

    DESCRIPTION:
        VRML 2 node handler.

    COPYRIGHT:
        Copyright (c) 2005-2011, Quesa Developers. All rights reserved.

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
#include "PointSetV2ToObject.h"

#include "GetArrayFromNodeField.h"
#include "PolyValue.h"
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
	@function	PointSetV2ToObject
	
	@abstract	Attempt to convert a VRML 2 PointSet node to a Quesa object.
	
	@param		ioNode		Node to convert.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	PointSetV2ToObject( PolyValue& ioNode )
{
	CQ3ObjectRef	theObject;
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );
	
	std::vector<TQ3Point3D>	thePoints;
	
	GetNodeArray( theDict, "coord", "Coordinate", "point", thePoints );
	
	if (not thePoints.empty())
	{
		theObject = CQ3ObjectRef( Q3DisplayGroup_New() );
		ThrowIfNullQuesaOb_( theObject );

		// The VRML 2 spec says that points are not lit.
		CQ3ObjectRef	theShader( Q3NULLIllumination_New() );
		ThrowIfNullQuesaOb_( theShader );
		Q3Group_AddObject( theObject.get(), theShader.get() );
		
		std::vector<TQ3ColorRGB>	theColors;
		GetNodeArray( theDict, "color", "Color", "color", theColors );
		unsigned int		i;
		const unsigned int kNumPoints = thePoints.size();
		
		if (theColors.size() >= kNumPoints)
		{
			for (i = 0; i < kNumPoints; ++i)
			{
				CQ3ObjectRef	attSet( Q3AttributeSet_New() );
				ThrowIfNullQuesaOb_( attSet );
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
		else	// no colors
		{
			for (i = 0; i < kNumPoints; ++i)
			{
				TQ3PointData	pointData2 = {
					thePoints[i], NULL
				};
				CQ3ObjectRef	point2( Q3Point_New( &pointData2 ) );
				ThrowIfNullQuesaOb_( point2 );
				Q3Group_AddObject( theObject.get(), point2.get() );
			}
		}
	}
	
	return theObject;
}

/*  NAME:
        IndexedLineSetV1ToObject.cp

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
#include "IndexedLineSetV1ToObject.h"

#include "CVRMLReader.h"
#include "GetArrayFromNodeField.h"
#include "GetIndexedMaterial.h"
#include "GetIntVecFromField.h"
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
	@function	StandardizeIndexVector
	@abstract	Make an index vector end with -1.
*/
static void StandardizeIndexVector( PolyValue::IntVec& ioVec )
{
	if ( (not ioVec.empty()) and (ioVec.back() >= 0) )
	{
		ioVec.push_back( -1 );
	}
}



/*!
	@function	IndexedLineSetV1ToObject
	
	@abstract	Attempt to convert a VRML 1 IndexedLineSet node to a Quesa object.
	
	@param		ioNode			Node to convert.
	@param		inReader		The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	IndexedLineSetV1ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	CQ3ObjectRef	theObject( Q3DisplayGroup_New() );
	ThrowIfNullQuesaOb_( theObject );
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );
	int	polylineNum = 0;
	
	// I am not going to bother with normals, so it is better not to use lighting.
	CQ3ObjectRef	theShader( Q3NULLIllumination_New() );
	ThrowIfNullQuesaOb_( theShader );
	Q3Group_AddObject( theObject.get(), theShader.get() );
	
	// Gather data
	PolyValue::IntVec		pointIndices;
	PolyValue::IntVec		colorIndices;
	GetIntVecFromField( theDict, "coordIndex", pointIndices );
	GetIntVecFromField( theDict, "materialIndex", colorIndices );
	StandardizeIndexVector( pointIndices );
	StandardizeIndexVector( colorIndices );
	const SVRML1State&	theState( inReader.GetVRML1State() );
	const std::vector<TQ3Point3D>&	thePoints( theState.coordinate3 );
	const std::vector<TQ3ColorRGB>&	theColors( theState.diffuseColor );
	bool isColorPerVertex( (theState.materialBinding == eVRML1Value_PER_VERTEX) or
		(theState.materialBinding == eVRML1Value_PER_VERTEX_INDEXED) );
	// Note: the VRML 1 spec, unlike the VRML 2 spec, indicates that colors
	// can be per segment rather than per polyline.  Currently I will not
	// support that.
	bool isColorPerPolyline( (theState.materialBinding == eVRML1Value_PER_PART) or
		(theState.materialBinding == eVRML1Value_PER_PART_INDEXED) or
		(theState.materialBinding == eVRML1Value_PER_FACE) or
		(theState.materialBinding == eVRML1Value_PER_FACE_INDEXED) );
	

	// Start building polylines
	std::vector<TQ3Vertex3D>	vertices;
	std::vector<CQ3ObjectRef>	vertAttSets;
	
	for (int i = 0; i < pointIndices.size(); ++i)
	{
		if (pointIndices[i] < 0)
		{
			// FInish a PolyLine
			TQ3PolyLineData	polyData;
			polyData.numVertices = vertices.size();
			polyData.vertices = &vertices[0];
			polyData.segmentAttributeSet = NULL;
			CQ3ObjectRef	polyAtts;
			if (not theColors.empty())
			{
				polyAtts = GetIndexedMaterial( inReader, 0 );
				
				if (isColorPerPolyline)
				{
					TQ3ColorRGB	polyColor;
					if (colorIndices.empty())
					{
						polyColor = theColors[ polylineNum ];
					}
					else
					{
						polyColor = theColors[ colorIndices[polylineNum] ];
					}
					Q3AttributeSet_Add( polyAtts.get(), kQ3AttributeTypeDiffuseColor,
						&polyColor );
				}

				polyData.polyLineAttributeSet = polyAtts.get();
			}
			else
			{
				polyData.polyLineAttributeSet = NULL;
			}
			CQ3ObjectRef	polyLine( Q3PolyLine_New( &polyData ) );
			ThrowIfNullQuesaOb_( polyLine );
			Q3Group_AddObject( theObject.get(), polyLine.get() );
			vertices.clear();
			vertAttSets.clear();
			polylineNum += 1;
		}
		else
		{
			TQ3Vertex3D	aVertex;
			aVertex.point = thePoints[ pointIndices[i] ];
			if (isColorPerVertex and (not theColors.empty()))
			{
				TQ3ColorRGB	vertColor;
				if (colorIndices.empty())
				{
					vertColor = theColors[ pointIndices[i] ];
				}
				else
				{
					vertColor = theColors[ colorIndices[i] ];
				}
				CQ3ObjectRef	vertAtts( Q3AttributeSet_New() );
				ThrowIfNullQuesaOb_( vertAtts );
				Q3AttributeSet_Add( vertAtts.get(), kQ3AttributeTypeDiffuseColor,
					&vertColor );
				vertAttSets.push_back( vertAtts );
				aVertex.attributeSet = vertAtts.get();
			}
			else
			{
				aVertex.attributeSet = NULL;
			}
			vertices.push_back( aVertex );
		}
	}

	if (polylineNum == 0)
	{
		// no sense returning an empty display group
		theObject = CQ3ObjectRef();
	}
	
	return theObject;
}


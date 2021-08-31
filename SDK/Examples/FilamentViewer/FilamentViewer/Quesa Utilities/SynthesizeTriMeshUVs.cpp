//
//  SynthesizeTriMeshUVs.cpp
//  FilamentViewer
//
//  Created by James Walker on 6/3/21.
//
/* ___________________________________________________________________________
   COPYRIGHT:
        Copyright (c) 2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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

#include "SynthesizeTriMeshUVs.h"

#include "FindTriMeshVertexData.h"

#include <Quesa/QuesaGeometry.h>
#include <Quesa/QuesaMathOperators.hpp>

#include <algorithm>
#include <vector>
#include <assert.h>

namespace
{
	enum EBoxSide
	{
		kBoxSide_Undefined = 0,
		kBoxSide_Front = 1,
		kBoxSide_Right,
		kBoxSide_Back,
		kBoxSide_Left,
		kBoxSide_Top,
		kBoxSide_Bottom,
		
		kBoxSide_NumValues
	};
	
}


/*!
	@function	SelectBoxSide
	@abstract	Find out which side of the bounding box a ray
				points to.
	@param		inInternalPt	A location inside a mesh.
	@param		inDirection		A direction vector.
	@param		inBox			The bounding box.
*/
static EBoxSide	SelectBoxSide( const TQ3Point3D& inInternalPt,
								const TQ3Vector3D& inDirection,
								const TQ3BoundingBox& inBox )
{
	EBoxSide	theSide = kBoxSide_Undefined;
	float	t;
	TQ3Point3D	testPt;
	
	// check front (z > 0)
	if (inDirection.z > kQ3RealZero)
	{
		t = (inBox.max.z - inInternalPt.z) / inDirection.z;
		testPt = inInternalPt + t * inDirection;
		if ( (testPt.x > inBox.min.x) and (testPt.x < inBox.max.x) and
			(testPt.y > inBox.min.y) and (testPt.y < inBox.max.y) )
		{
			theSide = kBoxSide_Front;
		}
	}
	// check rear (z < 0)
	if (inDirection.z < -kQ3RealZero)
	{
		t = (inBox.min.z - inInternalPt.z) / inDirection.z;
		testPt = inInternalPt + t * inDirection;
		if ( (testPt.x > inBox.min.x) and (testPt.x < inBox.max.x) and
			(testPt.y > inBox.min.y) and (testPt.y < inBox.max.y) )
		{
			theSide = kBoxSide_Back;
		}
	}
	// check top (y > 0)
	if (inDirection.y > kQ3RealZero)
	{
		t = (inBox.max.y - inInternalPt.y) / inDirection.y;
		testPt = inInternalPt + t * inDirection;
		if ( (testPt.x > inBox.min.x) and (testPt.x < inBox.max.x) and
			(testPt.z > inBox.min.z) and (testPt.z < inBox.max.z) )
		{
			theSide = kBoxSide_Top;
		}
	}
	// check bottom (y < 0)
	if (inDirection.y < -kQ3RealZero)
	{
		t = (inBox.min.y - inInternalPt.y) / inDirection.y;
		testPt = inInternalPt + t * inDirection;
		if ( (testPt.x > inBox.min.x) and (testPt.x < inBox.max.x) and
			(testPt.z > inBox.min.z) and (testPt.z < inBox.max.z) )
		{
			theSide = kBoxSide_Bottom;
		}
	}
	// check right (x > 0)
	if (inDirection.x > kQ3RealZero)
	{
		t = (inBox.max.x - inInternalPt.x) / inDirection.x;
		testPt = inInternalPt + t * inDirection;
		if ( (testPt.z > inBox.min.z) and (testPt.z < inBox.max.z) and
			(testPt.y > inBox.min.y) and (testPt.y < inBox.max.y) )
		{
			theSide = kBoxSide_Right;
		}
	}
	// check left (x < 0)
	if (inDirection.x < -kQ3RealZero)
	{
		t = (inBox.min.x - inInternalPt.x) / inDirection.x;
		testPt = inInternalPt + t * inDirection;
		if ( (testPt.z > inBox.min.z) and (testPt.z < inBox.max.z) and
			(testPt.y > inBox.min.y) and (testPt.y < inBox.max.y) )
		{
			theSide = kBoxSide_Left;
		}
	}
	
	return theSide;
}

/*!
	@function	FindMainSide
	@abstract	Find which side of the bounding box is pointed to by
				the most face normals.
	@param		inData			TriMesh data.
	@param		inOutsetBox		A slightly expanded bounding box.
*/
static EBoxSide	FindMainSide( const TQ3TriMeshData& inData,
						const TQ3BoundingBox& inOutsetBox )
{
	float	sideWeights[ kBoxSide_NumValues ];
	std::fill( &sideWeights[0], &sideWeights[ kBoxSide_NumValues ], 0.0f );
	
	for (long tri = inData.numTriangles - 1; tri >= 0; --tri)
	{
		const TQ3TriMeshTriangleData&	triData( inData.triangles[tri] );
		const TQ3Point3D&	pt0( inData.points[ triData.pointIndices[0] ] );
		const TQ3Point3D&	pt1( inData.points[ triData.pointIndices[1] ] );
		const TQ3Point3D&	pt2( inData.points[ triData.pointIndices[2] ] );
		TQ3Vector3D	normalVec;
		Q3Point3D_CrossProductTri( &pt0, &pt1, &pt2, &normalVec );
		TQ3Point3D	baryCenter = (1.0f / 3.0f) * (pt0 + pt1 + pt2);
		EBoxSide	theSide = SelectBoxSide( baryCenter, normalVec, inOutsetBox );
		sideWeights[ theSide ] += Q3Vector3D_Length( &normalVec );
	}

	// Find the majority side
	float*	majIter = std::max_element( &sideWeights[0],
		&sideWeights[ kBoxSide_NumValues ] );
	EBoxSide	mainSide = static_cast<EBoxSide>( majIter - &sideWeights[0] );
	return mainSide;
}

/*!
	@function	ProjectBoxUVFull
	@abstract	Compute UV coordinates for a point with the full-range style.
*/
static void ProjectBoxUVFull( const TQ3Point3D& inPt, EBoxSide inSide,
	const TQ3BoundingBox& inBox, TQ3Param2D& outUV )
{
	TQ3Vector3D dimens = inBox.max - inBox.min;
	switch (inSide)
	{
		default:
			break;
			
		case kBoxSide_Top:
			outUV.u = (inPt.x - inBox.min.x) / dimens.x;
			outUV.v = (inBox.max.z - inPt.z) / dimens.z;
			break;
		
		case kBoxSide_Bottom:
			outUV.u = (inPt.x - inBox.min.x) / dimens.x;
			outUV.v = (inPt.z - inBox.min.z) / dimens.z;
			break;
		
		case kBoxSide_Front:
			outUV.u = (inPt.x - inBox.min.x) / dimens.x;
			outUV.v = (inPt.y - inBox.min.y) / dimens.y;
			break;

		case kBoxSide_Back:
			outUV.u = (inBox.max.x - inPt.x) / dimens.x;
			outUV.v = (inPt.y - inBox.min.y) / dimens.y;
			break;
		
		case kBoxSide_Right:
			outUV.u = (inBox.max.z - inPt.z) / dimens.z;
			outUV.v = (inPt.y - inBox.min.y) / dimens.y;
			break;
		
		case kBoxSide_Left:
			outUV.u = (inPt.z - inBox.min.z) / dimens.z;
			outUV.v = (inPt.y - inBox.min.y) / dimens.y;
			break;
	}
}

/*!
	@function	ProjectBoxUVProportional
	@abstract	Compute UV coordinates for a point with the proportional-range style.
*/
static void ProjectBoxUVProportional( const TQ3Point3D& inPt,
		EBoxSide inSide,
		const TQ3BoundingBox& inBox, TQ3Param2D& outUV )
{
	TQ3Vector3D dimens = inBox.max - inBox.min;
	float	theDim;
	
	switch (inSide)
	{
		default:
			break;
			
		case kBoxSide_Top:
			theDim = std::max( dimens.x, dimens.z );
			outUV.u = (inPt.x - inBox.min.x) / theDim;
			outUV.v = (inBox.max.z - inPt.z) / theDim;
			break;
		
		case kBoxSide_Bottom:
			theDim = std::max( dimens.x, dimens.z );
			outUV.u = (inPt.x - inBox.min.x) / theDim;
			outUV.v = (inPt.z - inBox.min.z) / theDim;
			break;
		
		case kBoxSide_Front:
			theDim = std::max( dimens.x, dimens.y );
			outUV.u = (inPt.x - inBox.min.x) / theDim;
			outUV.v = (inPt.y - inBox.min.y) / theDim;
			break;

		case kBoxSide_Back:
			theDim = std::max( dimens.x, dimens.y );
			outUV.u = (inBox.max.x - inPt.x) / theDim;
			outUV.v = (inPt.y - inBox.min.y) / theDim;
			break;
		
		case kBoxSide_Right:
			theDim = std::max( dimens.y, dimens.z );
			outUV.u = (inBox.max.z - inPt.z) / theDim;
			outUV.v = (inPt.y - inBox.min.y) / theDim;
			break;
		
		case kBoxSide_Left:
			theDim = std::max( dimens.y, dimens.z );
			outUV.u = (inPt.z - inBox.min.z) / theDim;
			outUV.v = (inPt.y - inBox.min.y) / theDim;
			break;
	}
}


///MARK:-

/*!
	@function	SynthesizeTriMeshUVs

	@abstract	Synthesize texture coordinates in a way that works well for flat parts.
	
	@discussion	For each vertex, find the vertex normal and see which side of the
				local bounding box it points to.  Pick the side that the most face
				normals point to.  Then assign UVs by projecting to that box side.
				
				In the full range case, both u and v vary over the full range of 0 to 1.
				In the proportional case, the ranges of u and v are proportional to the
				dimensions of the part.  For example, suppose the part has an aspect
				ratio of 3.  Then while u would have the full 0 to 1 range, v would
				range between 0 and 1/3.
				
	@param	inTMData			The TriMesh data.
	@param	inProportional		Whether to use the proportional style,
								see discussion.
	@param	outUVs				Receives synthesized UVs.
*/
void	SynthesizeTriMeshUVs( const TQ3TriMeshData& inTMData,
							bool inProportional,
							std::vector<TQ3Param2D>& outUVs )
{
	// I need there to be some distance between the point and the box,
	// which there won't be if the TriMesh is perfectly flat.
	// So expand the box.
	TQ3BoundingBox	expandedBox = inTMData.bBox;
	TQ3Vector3D change = 0.01f * (expandedBox.max - expandedBox.min);
	expandedBox.min -= change;
	expandedBox.max += change;

	// Find the majority side
	EBoxSide	mainSide = FindMainSide( inTMData, expandedBox );
	assert( mainSide != kBoxSide_Undefined );
	
	// Compute UVs
	outUVs.resize( inTMData.numPoints );
	TQ3Uns32 i;
	if (inProportional)
	{
		for (i = 0; i < inTMData.numPoints; ++i)
		{
			ProjectBoxUVProportional( inTMData.points[i], mainSide,
					inTMData.bBox, outUVs[i] );
		}
	}
	else // full-range
	{
		for (i = 0; i < inTMData.numPoints; ++i)
		{
			ProjectBoxUVFull( inTMData.points[i], mainSide,
					inTMData.bBox, outUVs[i] );
		}
	}
}


/*!
	@function	CopyOrSynthesizeUVs
	
	@abstract	If TriMesh data contains vertex UV attributes, copy them to a vector, otherwise
				synthesize them in a simple way.
	
	@param	inTMData			The TriMesh data.
	@param	outUVs				Receives synthesized UVs.
*/
void	CopyOrSynthesizeUVs( const TQ3TriMeshData& inTMData,
							std::unique_ptr< std::vector<TQ3Param2D> >& outUVs )
{
	// Look for existing UVs
	const TQ3Param2D* uvs = reinterpret_cast<const TQ3Param2D*>(
		FindTriMeshVertexData( &inTMData, kQ3AttributeTypeSurfaceUV ) );
	if (uvs == nullptr)
	{
		uvs = reinterpret_cast<const TQ3Param2D*>(
			FindTriMeshVertexData( &inTMData, kQ3AttributeTypeShadingUV ) );
	}
	
	// If we found them, copy them.
	if (uvs != nullptr)
	{
		outUVs.reset( new std::vector<TQ3Param2D>( uvs, &uvs[inTMData.numPoints] ) );
	}
	else
	{
		outUVs.reset( new std::vector<TQ3Param2D> );
		SynthesizeTriMeshUVs( inTMData, false, *outUVs );
	}
}

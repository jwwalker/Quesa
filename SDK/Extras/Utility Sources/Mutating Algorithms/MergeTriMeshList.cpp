/*  NAME:
        MergeTriMeshList.cpp

    DESCRIPTION:
        Quesa utility source.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2007-2008, Quesa Developers. All rights reserved.

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

#include "MergeTriMeshList.h"

#include "FindTriMeshFaceData.h"
#include "FindTriMeshVertexData.h"

#if !TARGET_RT_MAC_MACHO
	#include "CQ3ObjectRef_Gets.h"
	#include "QuesaGeometry.h"
	#include "QuesaMath.h"
#else
	#include <Quesa/CQ3ObjectRef_Gets.h>
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaMath.h>
#endif

namespace
{
	
	class TrimMeshPeek
	{
	public:
								TrimMeshPeek( CQ3ObjectRef inTriMesh );
								~TrimMeshPeek();
						
		const TQ3TriMeshData&	operator*() const { return *mData; }
		
	private:
		CQ3ObjectRef	mObject;
		TQ3TriMeshData*	mData;
	};
}

TrimMeshPeek::TrimMeshPeek( CQ3ObjectRef inTriMesh )
	: mObject( inTriMesh )
	, mData( NULL )
{
	Q3TriMesh_LockData( mObject.get(), kQ3True, &mData );
}

TrimMeshPeek::~TrimMeshPeek()
{
	Q3TriMesh_UnlockData( mObject.get() );
}

template <typename T>
static void AppendArrayToVec( std::vector<T>& ioVec,
							const T* inArray, long inCount )
{
	if (inCount > 0)
	{
		long	prevSize = ioVec.size();
		ioVec.resize( prevSize + inCount );
		std::memcpy( &ioVec[prevSize], inArray, inCount * sizeof(T) );
	}
}

template <typename T>
static T* VecAddr( std::vector<T>& ioVec )
{
	return ioVec.empty()? NULL : &ioVec[0];
}


/*!
	@function	MergeTriMeshList
	
	@abstract	Merge a vector of compatible TriMeshes into a single TriMesh.
	
	@discussion	The merging takes into account face normals, vertex normals,
				vertex diffuse colors, vertex specular colors, vertex specular
				controls, vertex transparency colors, and vertex surface UVs.
				Therefore, for each of these attributes, either all of the
				TriMeshes should have the attribute, or none should.
	
	@param		inObs		Two or more TriMeshes.
	@result		A new TriMesh.
*/
CQ3ObjectRef	MergeTriMeshList( const std::vector< CQ3ObjectRef >& inObs )
{
	CQ3ObjectRef	theResult;
	CQ3ObjectRef	overallAtts( CQ3Geometry_GetAttributeSet( inObs[0].get() ) );

	std::vector<TQ3Point3D>		mergedPoints;
	std::vector<TQ3TriMeshTriangleData>	mergedFaces;
	std::vector<TQ3TriMeshEdgeData>	mergedEdges;

	std::vector<TQ3Vector3D>	mergedFaceNormals;
	std::vector<TQ3Vector3D>	mergedVertNormals;
	std::vector<TQ3Param2D>		mergedVertUVs;
	std::vector<TQ3ColorRGB>	mergedVertDiffColors;
	std::vector<TQ3ColorRGB>	mergedVertSpecColors;
	std::vector<TQ3ColorRGB>	mergedVertTransColors;
	std::vector<float>			mergedVertSpecControls;
	
	for (std::vector< CQ3ObjectRef >::const_iterator i = inObs.begin();
		i != inObs.end(); ++i)
	{
		TrimMeshPeek	tmPeek( *i );
		const TQ3TriMeshData&	tmData( *tmPeek );
		long	numPointsBefore = mergedPoints.size();
		long	numFacesBefore = mergedFaces.size();
		long	numEdgesBefore = mergedEdges.size();

		AppendArrayToVec( mergedPoints, tmData.points, tmData.numPoints );
		AppendArrayToVec( mergedFaces, tmData.triangles, tmData.numTriangles );
		AppendArrayToVec( mergedEdges, tmData.edges, tmData.numEdges );
		
		// Update point indices in faces
		TQ3Uns32 j;
		for (j = numFacesBefore; j < mergedFaces.size(); ++j)
		{
			TQ3TriMeshTriangleData&	theFace( mergedFaces[j] );
			theFace.pointIndices[0] += numPointsBefore;
			theFace.pointIndices[1] += numPointsBefore;
			theFace.pointIndices[2] += numPointsBefore;
		}
		
		// Update point indices and face indices in edges
		for (j = numEdgesBefore; j < mergedEdges.size(); ++j)
		{
			TQ3TriMeshEdgeData&	theEdge( mergedEdges[j] );
			theEdge.pointIndices[0] += numPointsBefore;
			theEdge.pointIndices[1] += numPointsBefore;
			theEdge.triangleIndices[0] += numFacesBefore;
			theEdge.triangleIndices[1] += numFacesBefore;
		}
		
		const TQ3Vector3D*	faceNorms = reinterpret_cast<const TQ3Vector3D*>(
			FindTriMeshFaceData( &tmData, kQ3AttributeTypeNormal ) );
		if (faceNorms != NULL)
		{
			AppendArrayToVec( mergedFaceNormals, faceNorms, tmData.numTriangles );
		}
		
		const TQ3Vector3D*	vertNorms = reinterpret_cast<const TQ3Vector3D*>(
			FindTriMeshVertexData( &tmData, kQ3AttributeTypeNormal ) );
		if (vertNorms != NULL)
		{
			AppendArrayToVec( mergedVertNormals, vertNorms, tmData.numPoints );
		}
		
		const TQ3Param2D*	vertUVs = reinterpret_cast<const TQ3Param2D*>(
			FindTriMeshVertexData( &tmData, kQ3AttributeTypeSurfaceUV ) );
		if (vertUVs == NULL)
		{
			vertUVs = reinterpret_cast<const TQ3Param2D*>(
				FindTriMeshVertexData( &tmData, kQ3AttributeTypeShadingUV ) );
		}
		if (vertUVs != NULL)
		{
			AppendArrayToVec( mergedVertUVs, vertUVs, tmData.numPoints );
		}
		
		const TQ3ColorRGB*	vertDiffColors = reinterpret_cast<const TQ3ColorRGB*>(
			FindTriMeshVertexData( &tmData, kQ3AttributeTypeDiffuseColor ) );
		if (vertDiffColors != NULL)
		{
			AppendArrayToVec( mergedVertDiffColors, vertDiffColors,
				tmData.numPoints );
		}
		
		const TQ3ColorRGB*	vertSpecColors = reinterpret_cast<const TQ3ColorRGB*>(
			FindTriMeshVertexData( &tmData, kQ3AttributeTypeSpecularColor ) );
		if (vertSpecColors != NULL)
		{
			AppendArrayToVec( mergedVertSpecColors, vertSpecColors,
				tmData.numPoints );
		}
		
		const float*	vertSpecControls = reinterpret_cast<const float*>(
			FindTriMeshVertexData( &tmData, kQ3AttributeTypeSpecularColor ) );
		if (vertSpecControls != NULL)
		{
			AppendArrayToVec( mergedVertSpecControls, vertSpecControls,
				tmData.numPoints );
		}
		
		const TQ3ColorRGB*	vertTransColors = reinterpret_cast<const TQ3ColorRGB*>(
			FindTriMeshVertexData( &tmData, kQ3AttributeTypeTransparencyColor ) );
		if (vertTransColors != NULL)
		{
			AppendArrayToVec( mergedVertTransColors, vertTransColors,
				tmData.numPoints );
		}
	}

	TQ3BoundingBox	theBounds;
	Q3BoundingBox_SetFromPoints3D( &theBounds, VecAddr(mergedPoints),
		mergedPoints.size(), sizeof(TQ3Point3D) );

	TQ3TriMeshAttributeData	faceAtt = {
		kQ3AttributeTypeNormal,
		VecAddr(mergedFaceNormals),
		NULL
	};
	
	std::vector<TQ3TriMeshAttributeData>	vertAtts;
	
	if (mergedVertNormals.size() == mergedPoints.size())
	{
		TQ3TriMeshAttributeData	normAtt = {
			kQ3AttributeTypeNormal, VecAddr(mergedVertNormals), NULL
		};
		vertAtts.push_back( normAtt );
	}
	
	if (mergedVertUVs.size() == mergedPoints.size())
	{
		TQ3TriMeshAttributeData	uvAtt = {
			kQ3AttributeTypeSurfaceUV, VecAddr(mergedVertUVs), NULL
		};
		vertAtts.push_back( uvAtt );
	}
	
	if (mergedVertDiffColors.size() == mergedPoints.size())
	{
		TQ3TriMeshAttributeData	diffAtt = {
			kQ3AttributeTypeDiffuseColor, VecAddr(mergedVertDiffColors), NULL
		};
		vertAtts.push_back( diffAtt );
	}
	
	if (mergedVertSpecColors.size() == mergedPoints.size())
	{
		TQ3TriMeshAttributeData	specAtt = {
			kQ3AttributeTypeSpecularColor, VecAddr(mergedVertSpecColors), NULL
		};
		vertAtts.push_back( specAtt );
	}
	
	if (mergedVertTransColors.size() == mergedPoints.size())
	{
		TQ3TriMeshAttributeData	transAtt = {
			kQ3AttributeTypeTransparencyColor, VecAddr(mergedVertTransColors), NULL
		};
		vertAtts.push_back( transAtt );
	}
	
	if (mergedVertSpecControls.size() == mergedPoints.size())
	{
		TQ3TriMeshAttributeData	specCtlAtt = {
			kQ3AttributeTypeSpecularControl, VecAddr(mergedVertSpecControls), NULL
		};
		vertAtts.push_back( specCtlAtt );
	}
	
	TQ3TriMeshData	mergedData = {
		overallAtts.get(),
		mergedFaces.size(),
		VecAddr(mergedFaces),
		(mergedFaceNormals.size() == mergedFaces.size()? 1 : 0),
		&faceAtt,
		mergedEdges.size(),
		VecAddr(mergedEdges),
		0,
		NULL,
		mergedPoints.size(),
		VecAddr(mergedPoints),
		vertAtts.size(),
		VecAddr(vertAtts),
		theBounds
	};
	
	if ( (mergedData.numPoints > 0) && (mergedData.numTriangles > 0) )
	{
		theResult = CQ3ObjectRef( Q3TriMesh_New( &mergedData ) );
	}
	
	return theResult;
}

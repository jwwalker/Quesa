/*  NAME:
        MergeNearTriMeshPoints.cpp

    DESCRIPTION:
        Quesa utility source.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2008, Quesa Developers. All rights reserved.

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

#include "MergeNearTriMeshPoints.h"

#include "FindTriMeshVertexData.h"

#if !TARGET_RT_MAC_MACHO
	#include "QuesaGeometry.h"
	#include "QuesaMath.h"
	#include "QuesaSet.h"
#else
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaMath.h>
	#include <Quesa/QuesaSet.h>
#endif

#include <vector>
#include <cmath>

/*!
	@function	MergeNearTriMeshPoints
	
	@abstract	Simplify a TriMesh by identifying points that are sufficiently
				near to each other in position, normal vector, and surface UV.
	
	@discussion	If the TriMesh has any per-vertex attribute data other than
				normal and UV, it will be discarded.  We assume that the normal
				vectors are unit length.
				
				The time taken by the algorithm is approximately proportional to
				the square of the number of points.
	
	@param		ioMesh					A TriMesh object to be updated.
	@param		inDistanceThreshold		If the distance between two points is
										greater than this, they will be
										considered distinct.
	@param		inNormalThreshold		If the angle between the normal vectors
										of two points is greater than this angle
										in radians, then the points will be
										considered distinct.
	@param		inUVThreshold			If the distance between the texture
										coordinates of two points is greater
										than this value, then the points will be
										considered distinct.
	
	@result		Reduction in number of points of the TriMesh.  If this is 0,
				then the object has not been modified.
*/
TQ3Uns32	MergeNearTriMeshPoints( TQ3GeometryObject ioMesh,
								float inDistanceThreshold,
								float inNormalThreshold,
								float inUVThreshold )
{
	TQ3Uns32	pointCountReduction = 0;
	
	TQ3TriMeshData	origTMData;
	if (kQ3Success == Q3TriMesh_GetData( ioMesh, &origTMData ))
	{
		const TQ3Param2D*	uvArray = reinterpret_cast<const TQ3Param2D*>(
			FindTriMeshVertexData( &origTMData, kQ3AttributeTypeSurfaceUV ) );
		if (uvArray == NULL)
		{
			uvArray = reinterpret_cast<const TQ3Param2D*>(
				FindTriMeshVertexData( &origTMData, kQ3AttributeTypeShadingUV ) );
		}
		
		const TQ3Vector3D*	normalArray = reinterpret_cast<const TQ3Vector3D*>(
			FindTriMeshVertexData( &origTMData, kQ3AttributeTypeNormal ) );
		
		if ( (normalArray != NULL) && (uvArray != NULL) )
		{
			const TQ3Point3D* points = origTMData.points;
			const TQ3Uns32	kNumOrigPoints = origTMData.numPoints;
			
			float distSqThreshold = inDistanceThreshold * inDistanceThreshold;
			float uvSqThreshold = inUVThreshold * inUVThreshold;
			float normalDotThreshold = std::cos( inNormalThreshold );
			
			std::vector<TQ3Uns32>	firstOfCluster( kNumOrigPoints );
			TQ3Uns32	i, j;
			
			for (i = 0; i < kNumOrigPoints; ++i)
			{
				firstOfCluster[i] = i;	// until further notice

			#if PP_Debug
				TQ3Point3D	pt_i = points[i];	// debug
				TQ3Param2D	uv_i = uvArray[i];
				TQ3Vector3D	norm_i = normalArray[i];
			#endif
				
				for (j = 0; j < i; ++j)
				{
					if (firstOfCluster[j] == j)
					{
						// Is point i equivalent to point j?
						if ( (Q3FastPoint3D_DistanceSquared( &points[i],
								&points[j] ) < distSqThreshold) &&
							(Q3FastParam2D_DistanceSquared( &uvArray[i],
								&uvArray[j] ) < uvSqThreshold) &&
							(Q3FastVector3D_Dot( &normalArray[i],
								&normalArray[j] ) > normalDotThreshold )
						)
						{
							firstOfCluster[i] = j;
							pointCountReduction += 1;
							break;
						}
					}
				}
			}
			
			if (pointCountReduction > 0)
			{
				const TQ3Uns32	kNumNewPoints = kNumOrigPoints - pointCountReduction;
				std::vector<TQ3Uns32>	oldToNewIndex( kNumOrigPoints );
				std::vector<TQ3Point3D>	newPts( kNumNewPoints );
				std::vector<TQ3Vector3D>	normals( kNumNewPoints );
				std::vector<TQ3Param2D>		uvs( kNumNewPoints );
				TQ3Uns32	nextIndex = 0;
				
				for (i = 0; i < kNumOrigPoints; ++i)
				{
					if (firstOfCluster[i] == i)
					{
						oldToNewIndex[i] = nextIndex;
						newPts[ nextIndex ] = origTMData.points[ i ];
						normals[ nextIndex ] = normalArray[ i ];
						uvs[ nextIndex ] = uvArray[ i ];
						++nextIndex;
					}
					else
					{
						oldToNewIndex[i] = oldToNewIndex[ firstOfCluster[i] ];
					}
				}
				
				std::vector<TQ3TriMeshTriangleData>	faces( origTMData.numTriangles );
				for (i = 0; i < origTMData.numTriangles; ++i)
				{
					for (j = 0; j < 3; ++j)
					{
						faces[i].pointIndices[j] =
							oldToNewIndex[ origTMData.triangles[i].pointIndices[j] ];
					}
				}
				
				
				TQ3TriMeshAttributeData	vertAtts[2] =
				{
					{ kQ3AttributeTypeNormal, &normals[0], NULL },
					{ kQ3AttributeTypeSurfaceUV, &uvs[0], NULL }
				};
				
				TQ3TriMeshData	newTMData =
				{
					origTMData.triMeshAttributeSet,
					
					origTMData.numTriangles,
					&faces[0],
					
					origTMData.numTriangleAttributeTypes,
					origTMData.triangleAttributeTypes,
					
					0,
					NULL,
					0,
					NULL,
					
					kNumNewPoints,
					&newPts[0],
					
					2,
					vertAtts,
					
					origTMData.bBox
				};
				
				Q3TriMesh_SetData( ioMesh, &newTMData );
			}
		}
		
		Q3TriMesh_EmptyData( &origTMData );
	}
	
	return pointCountReduction;
}

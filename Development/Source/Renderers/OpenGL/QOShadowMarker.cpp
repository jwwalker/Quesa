/*  NAME:
      QOShadowMarker.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2010, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://quesa.sourceforge.net/>
        
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
    
    REFERENCES:
    	Morgan McGuire, "Efficient Shadow Volume Rendering", in "GPU Gems",
    	2004, Addison-Wesley.  ISBN: 978-0321228321
    	
    	G. Aldridge, E. Woods. "Robust, geometry-independent shadow volumes",
    	in Proceedings of the 2nd international conference on Computer graphics
    	and interactive techniques in Australasia and SouthEast Asia (Graphite
    	2004). 15-18th June, Singapore, 2004, ACM Press, New York, New York,
    	pp. 250-253.
    	Available on web:
    	<http://www.hitlabnz.org/publications/2004-Graphite-ShadowVols-Aldridge.pdf>
    ___________________________________________________________________________
*/

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QOShadowMarker.h"
#include "QORenderer.h"


#include <cmath>


/*!
	@function	CalcLocalLightPosition
	@abstract	Calculate the light position in local coordinates.
*/
TQ3RationalPoint4D		QORenderer::ShadowMarker::CalcLocalLightPosition()
{
	TQ3RationalPoint4D	cameraLightPos = {
		mGLLightPosition[0],
		mGLLightPosition[1],
		mGLLightPosition[2],
		mGLLightPosition[3]
	};
	TQ3RationalPoint4D	localLightPos;
	const TQ3Matrix4x4&	cameraToLocal( mMatrixState.GetCameraToLocal() );
	Q3RationalPoint4D_Transform( &cameraLightPos,
		&cameraToLocal, &localLightPos );
	
	// Force the w component to be either 0 or 1.
	if ( (localLightPos.w != 0.0f) && (localLightPos.w != 1.0f) )
	{
		if (fabsf( localLightPos.w ) < kQ3RealZero)
		{
			localLightPos.w = 0.0f;
		}
		else
		{
			localLightPos.x /= localLightPos.w;
			localLightPos.y /= localLightPos.w;
			localLightPos.z /= localLightPos.w;
			localLightPos.w = 1.0f;
		}
	}
	
	return localLightPos;
}

/*!
	@function	ComputeFaceNormals
	@abstract	Compute a normal (which need not be unit length) for
				each triangle.
*/
static void ComputeFaceNormals( const TQ3TriMeshData& inTMData,
								std::vector<TQ3Vector3D>& outNormals )
{
	const TQ3Uns32 kNumFaces = inTMData.numTriangles;
	outNormals.resize( kNumFaces );
	
	for (TQ3Uns32 i = 0; i < kNumFaces; ++i)
	{
		Q3FastPoint3D_CrossProductTri(
			&inTMData.points[ inTMData.triangles[i].pointIndices[0] ],
			&inTMData.points[ inTMData.triangles[i].pointIndices[1] ],
			&inTMData.points[ inTMData.triangles[i].pointIndices[2] ],
			&outNormals[i] );
	}
}

/*!
	@function	FindLitFaces
	@abstract	Determine which of the triangles face toward the light.
*/
static void FindLitFaces( const TQ3RationalPoint4D& inLightPos,
						const TQ3TriMeshData& inTMData,
						const TQ3Vector3D* inFaceNormals,
						TQ3Uns8* outFlags )
{
	const TQ3Uns32 kNumFaces = inTMData.numTriangles;

	// Compute face normals if we do not have them.
	std::vector<TQ3Vector3D>	faceNormals;
	if (inFaceNormals == NULL)
	{
		ComputeFaceNormals( inTMData, faceNormals );
		inFaceNormals = &faceNormals[0];
	}
	
	TQ3Vector3D	toLight;
	TQ3Uns32	faceNum;
	
	if (inLightPos.w == 0.0f)	// directional
	{
		toLight.x = inLightPos.x;
		toLight.y = inLightPos.y;
		toLight.z = inLightPos.z;
		
		for (faceNum = 0; faceNum < kNumFaces; ++faceNum)
		{
			outFlags[ faceNum ] = Q3FastVector3D_Dot( &toLight,
				&inFaceNormals[ faceNum ] ) > 0.0f;
		}
	}
	else // point or spot
	{
		TQ3Point3D	lightPos3 = { inLightPos.x, inLightPos.y, inLightPos.z };
		
		for (faceNum = 0; faceNum < kNumFaces; ++faceNum)
		{
			Q3FastPoint3D_Subtract( &lightPos3,
				&inTMData.points[ inTMData.triangles[faceNum].pointIndices[0] ],
				&toLight  );
			
			outFlags[ faceNum ] = Q3FastVector3D_Dot( &toLight,
				&inFaceNormals[ faceNum ] ) > 0.0f;
		}
	}
}


/*!
	@function	GetTriMeshEdges
	@abstract	Retrieve or compute data about the edges and how they are
				owned by triangles.
*/
void QORenderer::ShadowMarker::GetTriMeshEdges( TQ3GeometryObject inTMObject,
						const TQ3TriMeshData& inTMData )
{
	if (inTMObject == NULL)
	{
		QOCalcTriMeshEdges( inTMData, mShadowEdges, &mShadowFacesToEdges );
	}
	else
	{
		QOGetCachedTriMeshEdges( inTMObject, mScratchBuffer, mShadowEdges,
			mShadowFacesToEdges );
	}

}


/*!
	@function	FlipTowardLight
	@abstract	Reverse the orientation of triangles that do not face the
				light.
*/
static void FlipTowardLight(
							const TQ3Uns32 inNumFaces,
							const TQ3Uns8* inLitFaceFlags,
							TQ3TriMeshTriangleData* ioFaces,
							TQ3TriangleToEdgeVec& ioFacesToEdges )
{
	TQ3Uns32	i;
	for (i = 0; i < inNumFaces; ++i)
	{
		if (! inLitFaceFlags[ i ])
		{
			std::swap( ioFaces[i].pointIndices[0], ioFaces[i].pointIndices[2] );
			std::swap( ioFacesToEdges[i].edgeIndices[0],
				ioFacesToEdges[i].edgeIndices[1] );
		}
	}
}




/*!
	@function	MarkShadowOfTriMeshDirectional
	@abstract	Mark the shadow of a TriMesh by a directional light.
*/
void QORenderer::ShadowMarker::MarkShadowOfTriMeshDirectional(
							const TQ3TriMeshData& inTMData,
							const TQ3TriMeshTriangleData* inFaces,
							const TQ3RationalPoint4D& inLocalLightPos )
{
	TQ3Uns32 i;

	// Set up original vertices and vertices extruded to infinity.
	const TQ3Uns32	kNumPoints = inTMData.numPoints;
	mShadowPoints.resize( kNumPoints + 1 );
	TQ3RationalPoint4D*	verts = &mShadowPoints[0];
	for (i = 0; i < kNumPoints; ++i)
	{
		TQ3RationalPoint4D	thePoint = {
			inTMData.points[i].x,
			inTMData.points[i].y,
			inTMData.points[i].z,
			1.0f
		};
		verts[i] = thePoint;
	}

	TQ3RationalPoint4D	oppositePt = {
		- inLocalLightPos.x,
		- inLocalLightPos.y,
		- inLocalLightPos.z,
		0.0f
	};
	verts[ kNumPoints ] = oppositePt;

	// Pass the positions to OpenGL.
	glVertexPointer( 4, GL_FLOAT, 0, verts );
	
	// Set up edge counters.
	const TQ3Uns32	kNumEdges = mShadowEdges.size();
	const TQ3EdgeEnds* theEdges = &mShadowEdges[0];
	if (mShadowEdgeCounters.size() < kNumEdges)
	{
		mShadowEdgeCounters.resize( kNumEdges );
	}
	TQ3Int32*	edgeCounter = &mShadowEdgeCounters[0];
	std::fill( edgeCounter, edgeCounter + kNumEdges, 0 );

	// Render front cap.
	const TQ3Uns32	kNumFaces = inTMData.numTriangles;
	if (mShadowVertIndices.size() < kNumFaces * 3)
	{
		mShadowVertIndices.resize( kNumFaces * 3 );
	}
	TQ3Uns32	numVertIndices = 0;
	GLuint*		vertIndices = &mShadowVertIndices[0];
	const TQ3TriangleEdges* facesToEdges = &mShadowFacesToEdges[0];
	const TQ3Uns8* litFaceFlags = &mLitFaceFlags[0];
	TQ3Uns32 e;
	for (i = 0; i < kNumFaces; ++i)
	{
		if ( (mStyleState.mBackfacing != kQ3BackfacingStyleRemove) ||
			litFaceFlags[i] )
		{
			const TQ3TriMeshTriangleData& theFace( inFaces[i] );
			
			// Render front cap
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[0];
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[1];
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[2];

			// Update edges of this face.
			const TQ3TriangleEdges& faceEdges( facesToEdges[ i ] );
			for (e = 0; e < 3; ++e)
			{
				TQ3Uns32	whichEdge = faceEdges.edgeIndices[e];
				
				if (theEdges[ whichEdge ].pointIndices[0] ==
					theFace.pointIndices[e])
				{
					// edge is directed the same as triangle winding
					edgeCounter[ whichEdge ] += 1;
				}
				else
				{
					edgeCounter[ whichEdge ] -= 1;
				}
			}
		}
	}
	glDrawElements( GL_TRIANGLES, numVertIndices, GL_UNSIGNED_INT,
		vertIndices );
	
	
	// Draw side triangles.
	if (mShadowVertIndices.size() < 3 * kNumFaces * 3)
	{
		mShadowVertIndices.resize( 3 * kNumFaces * 3 );
	}
	vertIndices = &mShadowVertIndices[0];
	numVertIndices = 0;
	for (i = 0; i < kNumEdges; ++i)
	{
		const TQ3EdgeEnds&	theEdge( theEdges[ i ] );
		TQ3Uns32	edgeStart = theEdge.pointIndices[0];
		TQ3Uns32	edgeEnd = theEdge.pointIndices[1];

		while (edgeCounter[i] > 0)
		{
			vertIndices[ numVertIndices++ ] = edgeEnd;
			vertIndices[ numVertIndices++ ] = edgeStart;
			vertIndices[ numVertIndices++ ] = kNumPoints;
			edgeCounter[i] -= 1;
		}
		while (edgeCounter[i] < 0)
		{
			vertIndices[ numVertIndices++ ] = edgeStart;
			vertIndices[ numVertIndices++ ] = edgeEnd;
			vertIndices[ numVertIndices++ ] = kNumPoints;
			edgeCounter[i] += 1;
		}
	}
	glDrawElements( GL_TRIANGLES, numVertIndices, GL_UNSIGNED_INT,
		vertIndices );
}


/*!
	@function	MarkShadowOfTriMeshPositional
	@abstract	Mark the shadow of a TriMesh by a point or spot light.
*/
void QORenderer::ShadowMarker::MarkShadowOfTriMeshPositional(
							const TQ3TriMeshData& inTMData,
							const TQ3TriMeshTriangleData* inFaces,
							const TQ3RationalPoint4D& inLocalLightPos )
{
	TQ3Uns32 i;
	
	// Set up original vertices and vertices extruded to infinity.
	const TQ3Uns32	kNumPoints = inTMData.numPoints;
	mShadowPoints.resize( 2 * kNumPoints );
	TQ3RationalPoint4D*	verts = &mShadowPoints[0];

	for (i = 0; i < kNumPoints; ++i)
	{
		TQ3RationalPoint4D	thePoint = {
			inTMData.points[i].x,
			inTMData.points[i].y,
			inTMData.points[i].z,
			1.0f
		};
		verts[i] = thePoint;
	}
	
	for (i = 0; i < kNumPoints; ++i)
	{
		TQ3RationalPoint4D	diffPt =
		{
			verts[i].x - inLocalLightPos.x,
			verts[i].y - inLocalLightPos.y,
			verts[i].z - inLocalLightPos.z,
			0.0f
		};
		verts[ i + kNumPoints ] = diffPt;
	}

	// Pass the positions to OpenGL.
	glVertexPointer( 4, GL_FLOAT, 0, verts );
	
	// Set up edge counters.
	const TQ3Uns32	kNumEdges = mShadowEdges.size();
	const TQ3EdgeEnds* theEdges = &mShadowEdges[0];
	if (mShadowEdgeCounters.size() < kNumEdges)
	{
		mShadowEdgeCounters.resize( kNumEdges );
	}
	TQ3Int32*	edgeCounter = &mShadowEdgeCounters[0];
	std::fill( edgeCounter, edgeCounter + kNumEdges, 0 );
	
	// Render front and back caps.
	const TQ3Uns32	kNumFaces = inTMData.numTriangles;
	if (mShadowVertIndices.size() < kNumFaces * 6)
	{
		mShadowVertIndices.resize( kNumFaces * 6 );
	}
	TQ3Uns32	numVertIndices = 0;
	GLuint*		vertIndices = &mShadowVertIndices[0];
	const TQ3TriangleEdges* facesToEdges = &mShadowFacesToEdges[0];
	const TQ3Uns8* litFaceFlags = &mLitFaceFlags[0];
	TQ3Uns32 e;
	for (i = 0; i < kNumFaces; ++i)
	{
		if ( (mStyleState.mBackfacing != kQ3BackfacingStyleRemove) ||
			litFaceFlags[i] )
		{
			const TQ3TriMeshTriangleData& theFace( inFaces[i] );
			
			// Render front cap
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[0];
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[1];
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[2];

			// Render back cap.
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[2] + kNumPoints;
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[1] + kNumPoints;
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[0] + kNumPoints;
			
			// Update edges of this face.
			const TQ3TriangleEdges& faceEdges( facesToEdges[ i ] );
			for (e = 0; e < 3; ++e)
			{
				TQ3Uns32	whichEdge = faceEdges.edgeIndices[e];
				
				if (theEdges[ whichEdge ].pointIndices[0] ==
					theFace.pointIndices[e])
				{
					// edge is directed the same as triangle winding
					edgeCounter[ whichEdge ] += 1;
				}
				else
				{
					edgeCounter[ whichEdge ] -= 1;
				}
			}
		}
	}
	glDrawElements( GL_TRIANGLES, numVertIndices, GL_UNSIGNED_INT,
		vertIndices );

	// Render side silhouette quads.
	if (mShadowVertIndices.size() < 3 * kNumFaces * 4)
	{
		mShadowVertIndices.resize( 3 * kNumFaces * 4 );
	}
	numVertIndices = 0;
	vertIndices = &mShadowVertIndices[0];
	for (i = 0; i < kNumEdges; ++i)
	{
		const TQ3EdgeEnds&	theEdge( theEdges[ i ] );
		TQ3Uns32	edgeStart = theEdge.pointIndices[0];
		TQ3Uns32	edgeEnd = theEdge.pointIndices[1];

		while (edgeCounter[i] > 0)
		{
			vertIndices[ numVertIndices++ ] = edgeEnd;
			vertIndices[ numVertIndices++ ] = edgeStart;
			vertIndices[ numVertIndices++ ] = edgeStart + kNumPoints;
			vertIndices[ numVertIndices++ ] = edgeEnd + kNumPoints;
			edgeCounter[i] -= 1;
		}
		while (edgeCounter[i] < 0)
		{
			vertIndices[ numVertIndices++ ] = edgeStart;
			vertIndices[ numVertIndices++ ] = edgeEnd;
			vertIndices[ numVertIndices++ ] = edgeEnd + kNumPoints;
			vertIndices[ numVertIndices++ ] = edgeStart + kNumPoints;
			edgeCounter[i] += 1;
		}
	}
	glDrawElements( GL_QUADS, numVertIndices, GL_UNSIGNED_INT,
		vertIndices );
}

/*!
	@function	MarkShadowOfTriMesh
	@abstract	Mark the shadow of a TriMesh in the stencil buffer.
*/
void	QORenderer::ShadowMarker::MarkShadowOfTriMesh(
								TQ3GeometryObject inTMObject,
								const TQ3TriMeshData& inTMData,
								const TQ3Vector3D* inFaceNormals )
{
	if (! mStyleState.mIsCastingShadows)
	{
		return;
	}
	TQ3RationalPoint4D	localLightPos( CalcLocalLightPosition() );
	
	GetTriMeshEdges( inTMObject, inTMData );
	
	const TQ3Uns32	kNumFaces = inTMData.numTriangles;
	mLitFaceFlags.resize( kNumFaces );
	FindLitFaces( localLightPos, inTMData, inFaceNormals, &mLitFaceFlags[0] );
	
	// If we are not removing backfaces, flip all faces toward the light.
	const TQ3TriMeshTriangleData*	faces = inTMData.triangles;
	if (mStyleState.mBackfacing != kQ3BackfacingStyleRemove)
	{
		mFlippedFaces.resize( kNumFaces );
		std::copy( faces, faces + kNumFaces, mFlippedFaces.begin() );
		FlipTowardLight( kNumFaces, &mLitFaceFlags[0], &mFlippedFaces[0],
			mShadowFacesToEdges );
		faces = &mFlippedFaces[0];
	}
	
	
	if (localLightPos.w == 0.0f)
	{
		MarkShadowOfTriMeshDirectional( inTMData, faces, localLightPos );
	}
	else
	{
		MarkShadowOfTriMeshPositional( inTMData, faces, localLightPos );
	}
}


/*!
	@function	MarkShadowOfTriangle
	@abstract	Mark the shadow of a triangle in the stencil buffer.
*/
void	QORenderer::ShadowMarker::MarkShadowOfTriangle(
								const Vertex* inVertices )
{
	if (! mStyleState.mIsCastingShadows)
	{
		return;
	}
	TQ3RationalPoint4D	localLightPos( CalcLocalLightPosition() );
	
	// Determine whether the triangle faces toward or away from the light.
	TQ3Vector3D		faceNormal;
	Q3FastPoint3D_CrossProductTri( &inVertices[0].point, &inVertices[1].point,
		&inVertices[2].point, &faceNormal );
	TQ3Vector3D	toLight;
	if (localLightPos.w == 0.0f)	// directional light
	{
		toLight.x = localLightPos.x;
		toLight.y = localLightPos.y;
		toLight.z = localLightPos.z;
	}
	else	// point or spot light
	{
		toLight.x = localLightPos.x - inVertices[0].point.x;
		toLight.y = localLightPos.y - inVertices[0].point.y;
		toLight.z = localLightPos.z - inVertices[0].point.z;
	}
	float	towardLight = Q3FastVector3D_Dot( &toLight, &faceNormal );
	
	// If the triangle is away from the light and we are removing backfaces,
	// then the triangle should be invisible from the light and hence not cast
	// a shadow.
	if ( (towardLight < 0.0f) && (mStyleState.mBackfacing == kQ3BackfacingStyleRemove) )
	{
		return;
	}
	
	// Make an array of vertex positions in local coordinates.
	mShadowPoints.reserve( 6 );
	mShadowPoints.clear();
	int i;
	// First the finite points.
	for (i = 0; i < 3; ++i)
	{
		TQ3RationalPoint4D	thePoint = {
			inVertices[i].point.x,
			inVertices[i].point.y,
			inVertices[i].point.z,
			1.0f
		};
		mShadowPoints.push_back( thePoint );
	}
	
	// If the triangle faces away from the light, flip it.
	if (towardLight < 0.0f)
	{
		std::swap( mShadowPoints[1], mShadowPoints[2] );
	}
	
	// Now the infinite points.
	if (localLightPos.w == 0.0f)	// directional light
	{
		TQ3RationalPoint4D	oppositePt = {
			- localLightPos.x,
			- localLightPos.y,
			- localLightPos.z,
			0.0f
		};
		mShadowPoints.push_back( oppositePt );
	}
	else	// point or spot light
	{
		for (i = 0; i < 3; ++i)
		{
			TQ3RationalPoint4D	diffPt =
			{
				mShadowPoints[i].x - localLightPos.x,
				mShadowPoints[i].y - localLightPos.y,
				mShadowPoints[i].z - localLightPos.z,
				0.0f,
			};
			mShadowPoints.push_back( diffPt );
		}
	}
	
	// Pass the positions to OpenGL.
	glVertexPointer( 4, GL_FLOAT, 0, &mShadowPoints[0] );
	
	// Draw the shadow sides.
	if (localLightPos.w == 0.0f)	// directional light
	{
		GLuint	dirTris[] = {
			3, 1, 0, 2, 1
		};
		glDrawElements( GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, dirTris );
	}
	else	// point or spot light
	{
		GLuint	pointQuads[] = {
			0, 3, 1, 4, 2, 5, 0, 3
		};
		glDrawElements( GL_QUAD_STRIP, 8, GL_UNSIGNED_INT, pointQuads );
	}
	
	// Draw the light cap.
	{
		GLuint	capTri[] = { 0, 1, 2 };
		glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, capTri );
	}
	
	// Draw the dark cap (with opposite orientation).
	if (localLightPos.w > 0.0f)	// point/spot light
	{
		GLuint	darkTri[] = { 5, 4, 3 };
		glDrawElements( GL_TRIANGLES, 3, GL_UNSIGNED_INT, darkTri );
	}
}

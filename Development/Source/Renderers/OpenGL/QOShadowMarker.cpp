/*  NAME:
      QOShadowMarker.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2014, Quesa Developers. All rights reserved.

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
    	<http://www.hitlabnz.org/administrator/components/com_jresearch/files/
		publications/2004-Graphite-ShadowVols-Aldridge.pdf>
    ___________________________________________________________________________
*/

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QOShadowMarker.h"
#include "QORenderer.h"
#include "GLShadowVolumeManager.h"
#include "GLUtils.h"


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
								E3FastArray<TQ3Vector3D>& outNormals )
{
	const TQ3Uns32 kNumFaces = inTMData.numTriangles;
	outNormals.resizeNotPreserving( kNumFaces );
	
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
	E3FastArray<TQ3Vector3D>	faceNormals;
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
		QOAccessCachedTriMeshEdges( inTMObject, mScratchBuffer, mShadowEdges,
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



static bool IsFaceVisible( TQ3BackfacingStyle inBackfacing, bool inFrontFace )
{
	bool	isVis = true;
	
	if (inBackfacing == kQ3BackfacingStyleRemove)
	{
		if (! inFrontFace)
		{
			isVis = false;
		}
	}
	else if (inBackfacing == kQ3BackfacingStyleRemoveFront)
	{
		if (inFrontFace)
		{
			isVis = false;
		}
	}
	
	return isVis;
}


/*!
	@function	BuildShadowOfTriMeshDirectional
	@abstract	Compute the shadow geometry for a TriMesh, using a directional
				light.  Store the vertices in mShadowPoints and the triangle
				indices in mShadowVertIndices.
*/
void	QORenderer::ShadowMarker::BuildShadowOfTriMeshDirectional(
								const TQ3TriMeshData& inTMData,
								const TQ3TriMeshTriangleData* inFaces,
								const TQ3TriangleEdges* inFacesToEdges,
								const TQ3RationalPoint4D& inLocalLightPos,
								TQ3Uns32& outNumTriIndices )
{
	TQ3Uns32 i;

	// Set up original vertices and vertices extruded to infinity.
	const TQ3Uns32	kNumPoints = inTMData.numPoints;
	mShadowPoints.resizeNotPreserving( kNumPoints + 1 );
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

	// Set up edge counters.
	const TQ3Uns32	kNumEdges = mShadowEdges.size();
	const TQ3EdgeEnds* theEdges = &mShadowEdges[0];
	if (mShadowEdgeCounters.size() < kNumEdges)
	{
		mShadowEdgeCounters.resizeNotPreserving( kNumEdges );
	}
	TQ3Int32*	edgeCounter = &mShadowEdgeCounters[0];
	std::fill( edgeCounter, edgeCounter + kNumEdges, 0 );
	
	// Make the array of shadow vertices big enough.
	// The number of faces in the front cap is at most the number of faces in
	// the given mesh, while the number of side faces is at most 3 times that
	// many, so the total number of shadow geometry faces is at most 4 times
	// the number of faces of the TriMesh.  Each triangular face needs 3 indices,
	// so the number of indices needed is 12 times the number of TriMesh faces.
	const TQ3Uns32	kNumFaces = inTMData.numTriangles;
	if (mShadowVertIndices.size() < kNumFaces * 12)
	{
		mShadowVertIndices.resizeNotPreserving( kNumFaces * 12 );
	}

	// Build front cap.
	TQ3Uns32	numVertIndices = 0;
	GLuint*		vertIndices = &mShadowVertIndices[0];
	const TQ3Uns8* litFaceFlags = &mLitFaceFlags[0];
	TQ3Uns32 e;
	for (i = 0; i < kNumFaces; ++i)
	{
		if ( IsFaceVisible( mStyleState.mBackfacing, litFaceFlags[i] != 0 ) )
		{
			const TQ3TriMeshTriangleData& theFace( inFaces[i] );
			
			// Render front cap
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[0];
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[1];
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[2];

			// Update edges of this face.
			const TQ3TriangleEdges& faceEdges( inFacesToEdges[ i ] );
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
	
	// Build side triangles.
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
	
	outNumTriIndices = numVertIndices;
}


/*!
	@function	BuildShadowOfTriMeshPositional
	@abstract	Compute the shadow geometry for a TriMesh, using a positional
				light.  Store the vertices in mShadowPoints and the triangle
				and quad indices in mShadowVertIndices.
*/
void	QORenderer::ShadowMarker::BuildShadowOfTriMeshPositional(
								const TQ3TriMeshData& inTMData,
								const TQ3TriMeshTriangleData* inFaces,
								const TQ3TriangleEdges* inFacesToEdges,
								const TQ3RationalPoint4D& inLocalLightPos,
								TQ3Uns32& outNumTriIndices,
								TQ3Uns32& outNumQuadIndices )
{
	TQ3Uns32 i;
	
	// Set up original vertices and vertices extruded to infinity.
	const TQ3Uns32	kNumPoints = inTMData.numPoints;
	mShadowPoints.resizeNotPreserving( 2 * kNumPoints );
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
	
	// Set up edge counters.
	const TQ3Uns32	kNumEdges = mShadowEdges.size();
	const TQ3EdgeEnds* theEdges = &mShadowEdges[0];
	if (mShadowEdgeCounters.size() < kNumEdges)
	{
		mShadowEdgeCounters.resizeNotPreserving( kNumEdges );
	}
	TQ3Int32*	edgeCounter = &mShadowEdgeCounters[0];
	std::fill( edgeCounter, edgeCounter + kNumEdges, 0 );
	
	// Allocate space for indices.
	// The front and back cap may contain up to two triangles for each original
	// triangle, and the side silhouette may have up to 3 quads for each original
	// triangle.  So the number of indices needed is at most
	// 2*3 + 3*4 = 18 times the number of faces.
	const TQ3Uns32	kNumFaces = inTMData.numTriangles;
	if (mShadowVertIndices.size() < kNumFaces * 18)
	{
		mShadowVertIndices.resizeNotPreserving( kNumFaces * 18 );
	}
	
	// Build front and back caps.
	TQ3Uns32	numVertIndices = 0;
	GLuint*		vertIndices = &mShadowVertIndices[0];
	const TQ3Uns8* litFaceFlags = &mLitFaceFlags[0];
	TQ3Uns32 e;
	for (i = 0; i < kNumFaces; ++i)
	{
		if ( IsFaceVisible( mStyleState.mBackfacing, litFaceFlags[i] != 0 ) )
		{
			const TQ3TriMeshTriangleData& theFace( inFaces[i] );
			
			// Add front cap
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[0];
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[1];
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[2];

			// Add back cap.
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[2] + kNumPoints;
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[1] + kNumPoints;
			vertIndices[ numVertIndices++ ] = theFace.pointIndices[0] + kNumPoints;
			
			// Update edges of this face.
			const TQ3TriangleEdges& faceEdges( inFacesToEdges[ i ] );
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
	outNumTriIndices = numVertIndices;

	// Build side silhouette quads.
	vertIndices = &mShadowVertIndices[numVertIndices];
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
	outNumQuadIndices = numVertIndices;
}


/*!
	@function	BuildShadowOfTriMesh
	@abstract	Compute the shadow geometry for a TriMesh, using a positional
				light.  Store the vertices in mShadowPoints and the triangle
				and quad indices in mShadowVertIndices.
*/
void	QORenderer::ShadowMarker::BuildShadowOfTriMesh(
								TQ3GeometryObject inTMObject,
								const TQ3TriMeshData& inTMData,
								const TQ3Vector3D* inFaceNormals,
								const TQ3RationalPoint4D& inLocalLightPos,
								TQ3Uns32& outNumTriIndices,
								TQ3Uns32& outNumQuadIndices )
{
	const TQ3TriMeshTriangleData*	faces;
	const TQ3TriangleEdges* facesToEdges;

	CalcFacesAndEdgesForShadows( inTMObject, inTMData, inFaceNormals,
			inLocalLightPos, faces, facesToEdges );

	if (inLocalLightPos.w == 0.0f)
	{
		BuildShadowOfTriMeshDirectional( inTMData, faces, facesToEdges,
			inLocalLightPos, outNumTriIndices );
		outNumQuadIndices = 0;
	}
	else
	{
		BuildShadowOfTriMeshPositional( inTMData, faces, facesToEdges,
			inLocalLightPos, outNumTriIndices, outNumQuadIndices );
	}
}


void	QORenderer::ShadowMarker::MarkShadowOfTriMeshImmediate(
								TQ3GeometryObject inTMObject,
								const TQ3TriMeshData& inTMData,
								const TQ3Vector3D* inFaceNormals,
								const TQ3RationalPoint4D& inLocalLightPos )
{
	TQ3Uns32 numTriIndices, numQuadIndices;
	
	BuildShadowOfTriMesh( inTMObject, inTMData, inFaceNormals, inLocalLightPos,
		numTriIndices, numQuadIndices );

	// Pass the positions to OpenGL.
	glVertexPointer( 4, GL_FLOAT, 0, &mShadowPoints[0] );

	// Render triangles.
	Q3_CHECK_DRAW_ELEMENTS( mShadowPoints.size(), numTriIndices,
		(const TQ3Uns32*)&mShadowVertIndices[0] );
	glDrawElements( GL_TRIANGLES, numTriIndices, GL_UNSIGNED_INT,
		&mShadowVertIndices[0] );
	
	// Render quads, if any.
	if (numQuadIndices > 0)
	{
		Q3_CHECK_DRAW_ELEMENTS( mShadowPoints.size(), numQuadIndices,
			(const TQ3Uns32*)&mShadowVertIndices[numTriIndices] );
		glDrawElements( GL_QUADS, numQuadIndices, GL_UNSIGNED_INT,
			&mShadowVertIndices[numTriIndices] );
	}
}


/*!
	@function	CalcFacesAndEdgesForShadows
	@abstract	Compute information about faces and edges of a TriMesh, which
				will be needed for shadow marking unless we are able to use a
				previously cached shadow volume from a VBO.
	@discussion	Besides returning two array pointers, this function has side
				effects on several member variables:
				mShadowEdges
				mShadowFacesToEdges
				mLitFaceFlags
				mFlippedFaces
				mFlippedFacesToEdges
*/
void	QORenderer::ShadowMarker::CalcFacesAndEdgesForShadows(
								TQ3GeometryObject inTMObject,
								const TQ3TriMeshData& inTMData,
								const TQ3Vector3D* inFaceNormals,
								const TQ3RationalPoint4D& inLocalLightPos,
								const TQ3TriMeshTriangleData*& outFaces,
								const TQ3TriangleEdges*& outFacesToEdges )
{
	GetTriMeshEdges( inTMObject, inTMData );
	
	const TQ3Uns32	kNumFaces = inTMData.numTriangles;
	mLitFaceFlags.resizeNotPreserving( kNumFaces );
	FindLitFaces( inLocalLightPos, inTMData, inFaceNormals, &mLitFaceFlags[0] );
	
	outFaces = inTMData.triangles;
	outFacesToEdges = &mShadowFacesToEdges[0];

	// If we are not removing backfaces, flip all faces toward the light.
	if (mStyleState.mBackfacing != kQ3BackfacingStyleRemove)
	{
		mFlippedFaces.resizeNotPreserving( kNumFaces );
		std::copy( outFaces, outFaces + kNumFaces, &mFlippedFaces[0] );
		
		mFlippedFacesToEdges = mShadowFacesToEdges;		// make a mutable copy
		
		FlipTowardLight( kNumFaces, &mLitFaceFlags[0], &mFlippedFaces[0],
			mFlippedFacesToEdges );
		
		outFaces = &mFlippedFaces[0];
		outFacesToEdges = &mFlippedFacesToEdges[0];
	}
}

/*!
	@function	MarkShadowOfTriMesh
	@abstract	Mark the shadow of a TriMesh in the stencil buffer.
*/
void	QORenderer::ShadowMarker::MarkShadowOfTriMesh(
								TQ3GeometryObject inTMObject,
								const TQ3TriMeshData& inTMData,
								const TQ3Vector3D* inFaceNormals,
								TQ3LightObject inLight )
{
	if (! mStyleState.mIsCastingShadows)
	{
		return;
	}
	TQ3RationalPoint4D	localLightPos( CalcLocalLightPosition() );
	
	if ( (inTMObject == NULL) ||
		(mGLExtensions.vertexBufferObjects == kQ3False) ||
		(! mIsCachingShadows) )
	{
		MarkShadowOfTriMeshImmediate( inTMObject, inTMData, inFaceNormals,
			localLightPos );
	}
	else
	{
		if (kQ3False == ShadowVolMgr::RenderShadowVolume( mGLContext, mBufferFuncs, inTMObject,
			inLight, localLightPos ))
		{
			TQ3Uns32 numTriIndices, numQuadIndices;
			
			BuildShadowOfTriMesh( inTMObject, inTMData, inFaceNormals,
				localLightPos, numTriIndices, numQuadIndices );
			
			Q3_CHECK_DRAW_ELEMENTS( mShadowPoints.size(),
				numTriIndices + numQuadIndices, (const TQ3Uns32*)&mShadowVertIndices[0] );
			ShadowVolMgr::AddShadowVolume( mGLContext, mBufferFuncs, inTMObject, inLight,
				localLightPos, mShadowPoints.size(), &mShadowPoints[0],
				numTriIndices, numQuadIndices, &mShadowVertIndices[0] );
			
			ShadowVolMgr::RenderShadowVolume( mGLContext, mBufferFuncs, inTMObject, inLight,
				localLightPos );
		}
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
	if ( ! IsFaceVisible( mStyleState.mBackfacing, towardLight > 0.0f ))
	{
		return;
	}
	
	// Make an array of vertex positions in local coordinates.
	mShadowPoints.resizeNotPreserving( 6 );
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

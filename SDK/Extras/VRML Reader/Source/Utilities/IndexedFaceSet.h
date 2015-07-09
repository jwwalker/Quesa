#pragma once
/*  NAME:
        IndexedFaceSet.h

    DESCRIPTION:
       Header file for IndexedFaceSet.cpp.

    COPYRIGHT:
        Copyright (c) 2005-2015, Quesa Developers. All rights reserved.

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

#if __MACH__
	#include <Quesa/Quesa.h>
	#include <Quesa/CQ3ObjectRef.h>
	#include <Quesa/QuesaGeometry.h>
#else
	#include <Quesa.h>
	#include <CQ3ObjectRef.h>
	#include <QuesaGeometry.h>
#endif

#include <vector>

	
class	CIndexedFaceSet
{
public:
	typedef long int	VertIndex;
	typedef long int	FaceIndex;
	typedef long int	PositionIndex;
	typedef long int	VertNormIndex;
	typedef long int	FaceNormIndex;
	typedef long int	ColorIndex;
	typedef long int	TexCoordIndex;
	typedef long int	PointIndex;
	
	typedef std::vector<TQ3TriMeshTriangleData>	TriangleVec;
	
	enum
	{
		kNoIndex = -1
	};
	
	struct SVertex
	{
		PositionIndex	mPosition;
		FaceIndex		mFace;
		VertNormIndex	mNormal;	// may be kNoIndex initially
		ColorIndex		mColor;		// may be kNoIndex
		TexCoordIndex	mTexCoord;	// may be kNoIndex
	};
	
	struct SFace
	{
		std::vector<VertIndex>	mVertices;
		FaceNormIndex			mNormal;	// may be kNoIndex initially
		ColorIndex				mColor;		// may be kNoIndex
		bool					mIsConvex;
	};

	// Accessors
	std::vector<SFace>&			GetFaces() { return mFaces; }
	const std::vector<SFace>&	GetFaces() const { return mFaces; }
	
	std::vector<SVertex>&		GetVertices() { return mVertices; }
	const std::vector<SVertex>&	GetVertices() const { return mVertices; }
	
	std::vector<TQ3Point3D>&	GetPositions() { return mPositions; }
	const std::vector<TQ3Point3D>&	GetPositions() const { return mPositions; }
	
	std::vector<TQ3ColorRGB>&	GetColors() { return mColors; }
	std::vector<TQ3Param2D>&	GetTexCoords() { return mTexCoords; }
	std::vector<TQ3Vector3D>&	GetFaceNormals() { return mFaceNormals; }
	std::vector<TQ3Vector3D>&	GetVertexNormals() { return mVertexNormals; }
	
	/*!
		@function				ReverseFaceOrientations
		@abstract				Reverse the order of vertices in each face.
								Call this if the faces are initially
								clockwise.
	*/
	void						ReverseFaceOrientations();
	
	/*!
		@function				CalcFaceNormals
		@abstract				If face normals were not provided, we must
								compute them by edge cross products.
								As a side effect, we also find non-convex
								faces.
	*/
	void						CalcFaceNormals();
		
	/*!
		@function				FindVerticesAtPosition
		@abstract				Group vertices at a given position.
		@discussion				We compute the vectors mLastVertAtPosition
								and mPrevVertAtSamePosition, which together
								define a linked list of vertices at a given
								position.
	*/
	void						FindVerticesAtPosition();
	
	/*!
		@function				CalcVertexNormals
		@abstract				If vertex normals were not provided,
								calculate them, taking into account a given
								crease angle.
	*/
	void						CalcVertexNormals( float inCreaseCosine );
	
	/*!
		@function				IdentifyDistinctPoints
		@abstract				Determine the points of the final TriMesh.
		@discussion				Two vertices sharing the same position may
								or may not be considered the same point,
								depending on whether they also share normal
								vectors, vertex colors, face colors, and
								texture coordinates.  We will compute the
								vectors mPointToPosition, mVertToPoint and
								mPointToVert.
	*/
	void						IdentifyDistinctPoints();
	
	/*!
		@function				CreateTriMesh
		@abstract				Create a TriMesh object from the data in this
								object.
		@result					A Quesa TriMesh object.
	*/
	CQ3ObjectRef				CreateTriMesh() const;

private:
	// Primary data
	std::vector<SFace>			mFaces;
	std::vector<SVertex>		mVertices;
	
	std::vector<TQ3Point3D>		mPositions;
	std::vector<TQ3Vector3D>	mFaceNormals;
	std::vector<TQ3Vector3D>	mVertexNormals;
	std::vector<TQ3ColorRGB>	mColors;
	std::vector<TQ3Param2D>		mTexCoords;

	// Derived data
	std::vector<VertIndex>		mLastVertAtPosition;
	std::vector<VertIndex>		mPrevVertAtSamePosition;
	
	std::vector<VertIndex>		mFirstVertWithNormal;
	std::vector<VertIndex>		mNextVertSharingNormal;
	
	std::vector<PositionIndex>	mPointToPosition;
	std::vector<PointIndex>		mVertToPoint;
	std::vector<VertIndex>		mPointToVert;
	
	/*!
		@function				RemoveDegenerateFaces
		@abstract				Remove degenerate faces (those for which we
								were not able to compute a sensible face
								normal) and their vertices.
	*/
	void						RemoveDegenerateFaces();

	/*!
		@function				ComputeTriMeshFaces
		@abstract				Split each nondegenerate face into triangles.
		@param					outTriangles		Receives triangles, each
													one containing 3 point
													indices.
	*/
	void						ComputeTriMeshFaces( TriangleVec& outTriangles ) const;
	
	/*!
		@function				ComputeTriMeshFaceNormals
		@abstract				Build the vector of triangle normals for use
								in a TriMesh.
		@param					outNormals			Receives the normals.
	*/
	void						ComputeTriMeshFaceNormals(
										std::vector<TQ3Vector3D>& outNormals ) const;
	
	/*!
		@function				ComputeTriMeshFaceColors
		@abstract				Build the vector of triangle colors, if any, for use
								in a TriMesh.
		@param					outNormals			Receives the normals.
	*/
	void						ComputeTriMeshFaceColors(
										std::vector<TQ3ColorRGB>& outColors ) const;
	
	/*!
		@function				ComputeTriMeshPositions
		@abstract				Build the vector of points for a TriMesh.
		@param					outPoints			Receives the points.
	*/
	void						ComputeTriMeshPositions(
										std::vector<TQ3Point3D>& outPoints ) const;

	/*!
		@function				ComputeTriMeshVertexNormals
		@abstract				Build the vector of vertex normals for a TriMesh.
		@param					outNormals			Receives the normals.
	*/
	void						ComputeTriMeshVertexNormals(
										std::vector<TQ3Vector3D>& outNormals ) const;
	/*!
		@function				ComputeTriMeshVertexColors
		@abstract				Build the vector of vertex colors for a TriMesh.
		@param					outColors			Receives the colors.
	*/
	void						ComputeTriMeshVertexColors(
										std::vector<TQ3ColorRGB>& outColors ) const;

	/*!
		@function				ComputeTriMeshTexCoords
		@abstract				Build the vector of vertex texture coordinates
								for a TriMesh.
		@discussion				If no texture coordinates were provided, we
								synthesize them based on point positions and
								bounds, according to VRML conventions.
		@param					inPoints			The TriMesh points as
													computed by
													ComputeTriMeshPositions.
		@param					inBounds			The TriMesh bounding box.
		@param					outColors			Receives the texture coordinates.
	*/
	void						ComputeTriMeshTexCoords(
										const std::vector<TQ3Point3D>& inPoints,
										const TQ3BoundingBox& inBounds,
										std::vector<TQ3Param2D>& outCoords ) const;

	void						TriangulateConvexFace( const SFace& inFace,
										TriangleVec& outTriangles ) const;
	void						TriangulateNonconvexFace( const SFace& inFace,
										TriangleVec& outTriangles ) const;

	/*!
		@function				CalcAngleAtVertex
		@abstract				Compute the angle that a face makes at a vertex,
								for use in weighing the face normals to compute
								a vertex normal.
		@param					inVertIndex			Index of a vertex.
		@result					An angle in radians.
	*/
	float						CalcAngleAtVertex( VertIndex inVertIndex ) const;
};

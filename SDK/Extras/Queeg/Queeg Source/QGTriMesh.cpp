/*  NAME:
        QGTriMesh.cpp
        
    DESCRIPTION:
		Class for loading and manipulating trimesh structures

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QGTriMesh.h"
#include "QueegMacros.h"
#include "QGUtils.h"

#include <math.h>
#include <string.h>





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
#ifndef SWAP
#define SWAP(a,b) a ^= b; b ^= a; a ^= b
#endif





//=============================================================================
//      QGTriMesh::QGTriMesh :	Construct a trimesh object from scratch,
//			allocating space for the given number of vertices and triangles,
//			with or without texture UVs and vertex normals.
//-----------------------------------------------------------------------------
QGTriMesh::QGTriMesh( TQ3Uns32 qtyVertices, TQ3Uns32 qtyTriangles, 
		TQ3Boolean hasUV, TQ3Boolean smooth )
: mBoundsDirty(kQ3True)
{
	// initialize array space to 0, for neat clean-up in case of error
	mTriMeshData.points = 0;
	mTriMeshData.triangles = 0;
	mTriMeshData.vertexAttributeTypes = 0;
	mTriMeshData.triangleAttributeTypes = 0;
	mTriMeshData.edges = 0;
	
	// initialize edges (not used)
	mTriMeshData.numEdges = 0;
	mTriMeshData.numEdgeAttributeTypes = 0;
	mTriMeshData.edgeAttributeTypes = NULL;

	try {
		// initalize vertex points
		mTriMeshData.numPoints = qtyVertices;
		mTriMeshData.points = new TQ3Point3D[qtyVertices];
		ASSERT(mTriMeshData.points);
		
		// initialize triangles
		mTriMeshData.numTriangles = qtyTriangles;
		mTriMeshData.triangles = new TQ3TriMeshTriangleData[qtyTriangles];
		ASSERT(mTriMeshData.triangles);

		// initialize vertex attributes
		// the first attribute is the vertex normals (if needed);
		// the next (if needed) is UV coordinates for a texture map
		mSmooth = smooth;
		mTextured = hasUV;
		mTriMeshData.numVertexAttributeTypes = (smooth ? 1 : 0) + (hasUV ? 1 : 0);
		if (mTriMeshData.numVertexAttributeTypes) {
			mTriMeshData.vertexAttributeTypes = new TQ3TriMeshAttributeData[mTriMeshData.numVertexAttributeTypes];
			ASSERT(mTriMeshData.vertexAttributeTypes);
		} else {
			mTriMeshData.vertexAttributeTypes = NULL;
		}

		short anum = 0;		// attribute number
		if (smooth) {
			mTriMeshData.vertexAttributeTypes[anum].attributeType = kQ3AttributeTypeNormal;
			mTriMeshData.vertexAttributeTypes[anum].data = new TQ3Vector3D[qtyVertices];
			ASSERT(mTriMeshData.vertexAttributeTypes[anum].data);
			mTriMeshData.vertexAttributeTypes[anum].attributeUseArray = NULL;
			anum++;
		}
		if (hasUV) {
			mTriMeshData.vertexAttributeTypes[anum].attributeType = kQ3AttributeTypeShadingUV;
			mTriMeshData.vertexAttributeTypes[anum].data = new TQ3Param2D[qtyVertices];
			ASSERT(mTriMeshData.vertexAttributeTypes[anum].data);
			mTriMeshData.vertexAttributeTypes[anum].attributeUseArray = NULL;
		}
		
		// initialize triangle attributes
		TQ3Vector3D *normals = new TQ3Vector3D[qtyTriangles];
		ASSERT(normals);
		
		mTriMeshData.numTriangleAttributeTypes = 1;
		mTriMeshData.triangleAttributeTypes = new TQ3TriMeshAttributeData[mTriMeshData.numTriangleAttributeTypes];
		ASSERT(mTriMeshData.triangleAttributeTypes);

		mTriMeshData.triangleAttributeTypes[0].attributeType = kQ3AttributeTypeNormal;
		mTriMeshData.triangleAttributeTypes[0].data = normals;
		mTriMeshData.triangleAttributeTypes[0].attributeUseArray = NULL;

		// initialize the mesh global attributes
		mTriMeshData.triMeshAttributeSet = Q3AttributeSet_New();
		ASSERT(mTriMeshData.triMeshAttributeSet);
	} catch (...) {
		// any exception thrown by the above indicates allocation failure
		delete[] mTriMeshData.points;					mTriMeshData.points = NULL;
		delete[] mTriMeshData.triangles;				mTriMeshData.triangles = NULL;
		delete[] mTriMeshData.vertexAttributeTypes;		mTriMeshData.vertexAttributeTypes = NULL;
		delete[] mTriMeshData.triangleAttributeTypes;	mTriMeshData.triangleAttributeTypes = NULL;
		delete[] mTriMeshData.edges;					mTriMeshData.edges = NULL;
		mTriMeshData.numPoints = mTriMeshData.numTriangles
			= mTriMeshData.numVertexAttributeTypes = mTriMeshData.numTriangleAttributeTypes = 0;
	}		
}





//=============================================================================
//      QGTriMesh::QGTriMesh :	Construct a trimesh object a TQ3TriMesh.
//			Only the texture UVs and vertex normals are kept; other
//			attributes are ignored.
//-----------------------------------------------------------------------------
QGTriMesh::QGTriMesh(TQ3GeometryObject srcObj)
: mBoundsDirty(kQ3True)
{
	// construct the QGTriMesh by copying the data from a QD3D object
	
	// first, make sure the type is right
	ASSERT(Q3Object_IsType( srcObj, kQ3GeometryTypeTriMesh ));
	
	// get source data
	TQ3TriMeshData srcData;
	Q3TriMesh_GetData( srcObj, &srcData );

	// copy copyable data
	memcpy( &mTriMeshData, &srcData, sizeof( TQ3TriMeshData ) );
	
	// initialize array space to 0, for neat clean-up in case of error
	mTriMeshData.points = 0;
	mTriMeshData.triangles = 0;
	mTriMeshData.vertexAttributeTypes = 0;
	mTriMeshData.triangleAttributeTypes = 0;
	mTriMeshData.edges = 0;
	
	try {
		short i;
		
		// initalize vertex points
		mTriMeshData.points = new TQ3Point3D[mTriMeshData.numPoints];
		ASSERT(mTriMeshData.points);
		memcpy( mTriMeshData.points, srcData.points, 
					sizeof(TQ3Point3D)*mTriMeshData.numPoints );
		
		// initialize triangles
		mTriMeshData.triangles = new TQ3TriMeshTriangleData[mTriMeshData.numTriangles];
		ASSERT(mTriMeshData.triangles);
		memcpy( mTriMeshData.triangles, srcData.triangles, 
					sizeof(TQ3TriMeshTriangleData)*mTriMeshData.numTriangles );

		// initialize vertex attributes
		// the first attribute must be the vertex normals (if needed);
		// the next (if needed) must be UV coordinates for a texture map	
		mSmooth = kQ3False;
		mTextured = kQ3False;
		short smoothAttribNum, uvAttribNum;
		mTriMeshData.numVertexAttributeTypes = 0;
		for (i=0; i < srcData.numVertexAttributeTypes; i++) {
			if (srcData.vertexAttributeTypes[i].attributeType == kQ3AttributeTypeNormal) {
				mSmooth = kQ3True;
				smoothAttribNum = i;
				mTriMeshData.numVertexAttributeTypes++;
			} else
			if (srcData.vertexAttributeTypes[i].attributeType == kQ3AttributeTypeShadingUV) {
				mTextured = kQ3True;
				uvAttribNum = i;
				mTriMeshData.numVertexAttributeTypes++;
			} 
		}
		
		// now we've found the UV and normal attributes, copy the data
		if (mTriMeshData.numVertexAttributeTypes) {
			mTriMeshData.vertexAttributeTypes = new TQ3TriMeshAttributeData[mTriMeshData.numVertexAttributeTypes];
			ASSERT(mTriMeshData.vertexAttributeTypes);
		} else {
			mTriMeshData.vertexAttributeTypes = NULL;
		}

		short anum = 0;		// attribute number
		if (mSmooth) {
			mTriMeshData.vertexAttributeTypes[anum].attributeType = kQ3AttributeTypeNormal;
			mTriMeshData.vertexAttributeTypes[anum].data = new TQ3Vector3D[mTriMeshData.numPoints];
			ASSERT(mTriMeshData.vertexAttributeTypes[anum].data);
			memcpy( mTriMeshData.vertexAttributeTypes[anum].data,
					   srcData.vertexAttributeTypes[smoothAttribNum].data, 
					   sizeof(TQ3Vector3D) * mTriMeshData.numPoints );
			mTriMeshData.vertexAttributeTypes[anum].attributeUseArray = NULL;
			anum++;
		}
		if (mTextured) {
			mTriMeshData.vertexAttributeTypes[anum].attributeType = kQ3AttributeTypeShadingUV;
			mTriMeshData.vertexAttributeTypes[anum].data = new TQ3Param2D[mTriMeshData.numPoints];
			ASSERT(mTriMeshData.vertexAttributeTypes[anum].data);
			memcpy( mTriMeshData.vertexAttributeTypes[anum].data,
					   srcData.vertexAttributeTypes[uvAttribNum].data, 
					   sizeof(TQ3Param2D) * mTriMeshData.numPoints );
			mTriMeshData.vertexAttributeTypes[anum].attributeUseArray = NULL;
		}
		

		// initialize triangle attributes...
		// the only one we care about is triangle normals,
		// and we compute those manually, so just allocate space here
		// (NOTE: we could have done the same for vertex normals, but not uv's!)
		TQ3Vector3D *normals = new TQ3Vector3D[mTriMeshData.numTriangles];
		ASSERT(normals);
		
		mTriMeshData.numTriangleAttributeTypes = 1;
		mTriMeshData.triangleAttributeTypes = new TQ3TriMeshAttributeData[mTriMeshData.numTriangleAttributeTypes];
		ASSERT(mTriMeshData.triangleAttributeTypes);

		mTriMeshData.triangleAttributeTypes[0].attributeType = kQ3AttributeTypeNormal;
		mTriMeshData.triangleAttributeTypes[0].data = normals;
		mTriMeshData.triangleAttributeTypes[0].attributeUseArray = NULL;

		TQ3Boolean foundem = kQ3False;
		for (i=0; i < srcData.numTriangleAttributeTypes and !foundem; i++) {
			if (srcData.triangleAttributeTypes[i].attributeType == kQ3AttributeTypeNormal) {
				memcpy( mTriMeshData.triangleAttributeTypes[0].data,
							srcData.triangleAttributeTypes[i].data,
							sizeof(TQ3Vector3D) * mTriMeshData.numTriangles );
				foundem = kQ3True;
			}
		}
		if (!foundem) ComputeNormals();
		
		// initialize edges (not used)
		mTriMeshData.numEdges = 0;
		mTriMeshData.edges = NULL;
		mTriMeshData.numEdgeAttributeTypes = 0;
		mTriMeshData.edgeAttributeTypes = NULL;

		// initialize the mesh global attributes
		// ...simply clone the source mesh's attribute data
		if (srcData.triMeshAttributeSet) {
			mTriMeshData.triMeshAttributeSet = Q3Object_Duplicate( srcData.triMeshAttributeSet );
		} else srcData.triMeshAttributeSet = NULL;
	} catch (...) {
		// any exception thrown by the above indicates allocation failure
		delete[] mTriMeshData.points;					mTriMeshData.points = NULL;
		delete[] mTriMeshData.triangles;				mTriMeshData.triangles = NULL;
		delete[] mTriMeshData.vertexAttributeTypes;		mTriMeshData.vertexAttributeTypes = NULL;
		delete[] mTriMeshData.triangleAttributeTypes;	mTriMeshData.triangleAttributeTypes = NULL;
		delete[] mTriMeshData.edges;					mTriMeshData.edges = NULL;
		mTriMeshData.numPoints = mTriMeshData.numTriangles
			= mTriMeshData.numVertexAttributeTypes = mTriMeshData.numTriangleAttributeTypes = 0;
		Q3TriMesh_EmptyData( &srcData );	// release source data
	}		
	
	// release source data
	Q3TriMesh_EmptyData( &srcData );
}





//=============================================================================
//      QGTriMesh::QGTriMesh :	Construct a trimesh object another one
//			(i.e., this is a copy-constructor).
//-----------------------------------------------------------------------------
QGTriMesh::QGTriMesh(const QGTriMesh &src)
{
	// copy numeric data in one big block
	TQ3TriMeshData srcData = src.mTriMeshData;
	memcpy( &mTriMeshData, &srcData,  sizeof( TQ3TriMeshData ) );
	
	// initialize array space to 0, for neat clean-up in case of error
	mTriMeshData.points = 0;
	mTriMeshData.triangles = 0;
	mTriMeshData.vertexAttributeTypes = 0;
	mTriMeshData.triangleAttributeTypes = 0;
	mTriMeshData.edges = 0;
	
	try {
		// initalize vertex points
		if (mTriMeshData.numPoints) {
			mTriMeshData.points = new TQ3Point3D[mTriMeshData.numPoints];
			memcpy( mTriMeshData.points, srcData.points, 
						sizeof(TQ3Point3D)*mTriMeshData.numPoints );
		}
		
		// initialize triangles
		if (mTriMeshData.numTriangles) {
		mTriMeshData.triangles = new TQ3TriMeshTriangleData[mTriMeshData.numTriangles];
		memcpy( mTriMeshData.triangles, srcData.triangles, 
					sizeof(TQ3TriMeshTriangleData)*mTriMeshData.numTriangles );
		}
		
		// initialize vertex attributes
		if (mTriMeshData.numVertexAttributeTypes) {
			mTriMeshData.vertexAttributeTypes = new TQ3TriMeshAttributeData[mTriMeshData.numVertexAttributeTypes];
			memcpy( mTriMeshData.vertexAttributeTypes, srcData.vertexAttributeTypes, 
							sizeof(TQ3TriMeshAttributeData) * mTriMeshData.numVertexAttributeTypes );
			for (short anum=0; anum<mTriMeshData.numVertexAttributeTypes; anum++) {
				if (mTriMeshData.vertexAttributeTypes[anum].attributeType == kQ3AttributeTypeShadingUV) {
					mTriMeshData.vertexAttributeTypes[anum].data = new TQ3Param2D[mTriMeshData.numPoints];
					memcpy( mTriMeshData.vertexAttributeTypes[anum].data,
							   srcData.vertexAttributeTypes[anum].data, 
							   sizeof(TQ3Param2D) * mTriMeshData.numPoints );
				} else {
					mTriMeshData.vertexAttributeTypes[anum].data = new TQ3Vector3D[mTriMeshData.numPoints];
					memcpy( mTriMeshData.vertexAttributeTypes[anum].data,
							   srcData.vertexAttributeTypes[anum].data, 
							   sizeof(TQ3Vector3D) * mTriMeshData.numPoints );
				}
				mTriMeshData.vertexAttributeTypes[anum].attributeUseArray = NULL;
			}	
		}
		
		// initialize triangle attributes...
		if (mTriMeshData.numTriangles) {
			mTriMeshData.triangleAttributeTypes = new TQ3TriMeshAttributeData[mTriMeshData.numTriangleAttributeTypes];
			ASSERT(mTriMeshData.triangleAttributeTypes);

			mTriMeshData.triangleAttributeTypes[0].attributeType = kQ3AttributeTypeNormal;
			mTriMeshData.triangleAttributeTypes[0].data = new TQ3Vector3D[mTriMeshData.numTriangles];
			memcpy( mTriMeshData.triangleAttributeTypes[0].data,
					srcData.triangleAttributeTypes[0].data,
					sizeof(TQ3Vector3D) * mTriMeshData.numTriangles );
			mTriMeshData.triangleAttributeTypes[0].attributeUseArray = NULL;
		}

		// initialize the mesh global attributes
		// ...simply clone the source mesh's attribute data
		mTriMeshData.triMeshAttributeSet = Q3Object_Duplicate( srcData.triMeshAttributeSet );
		ASSERT(mTriMeshData.triMeshAttributeSet);
		
	} catch (...) {
		// any exception thrown by the above indicates allocation failure
		delete[] mTriMeshData.points;					mTriMeshData.points = NULL;
		delete[] mTriMeshData.triangles;				mTriMeshData.triangles = NULL;
		delete[] mTriMeshData.vertexAttributeTypes;		mTriMeshData.vertexAttributeTypes = NULL;
		delete[] mTriMeshData.triangleAttributeTypes;	mTriMeshData.triangleAttributeTypes = NULL;
		delete[] mTriMeshData.edges;					mTriMeshData.edges = NULL;
		mTriMeshData.numPoints = mTriMeshData.numTriangles
			= mTriMeshData.numVertexAttributeTypes = mTriMeshData.numTriangleAttributeTypes = 0;
	}		
	
	// finally (whew!), copy other data members
	mSmooth = src.mSmooth;
	mTextured = src.mTextured;
	mBoundsDirty = src.mBoundsDirty;
}





//=============================================================================
//      QGTriMesh::~QGTriMesh :	Dispose of a trimesh object.
//-----------------------------------------------------------------------------
QGTriMesh::~QGTriMesh()
{
	// we have to dispose of all the parts allocated to the trimesh data...
	delete[] mTriMeshData.points;
	delete[] mTriMeshData.triangles;
	while (mTriMeshData.numVertexAttributeTypes--) {
		delete[] mTriMeshData.vertexAttributeTypes[mTriMeshData.numVertexAttributeTypes].data;
	}
	delete[] mTriMeshData.vertexAttributeTypes;
	delete[] mTriMeshData.triangleAttributeTypes[0].data;
	delete[] mTriMeshData.triangleAttributeTypes;

	// and clear the attribute set
	if (mTriMeshData.triMeshAttributeSet) {
		Q3Object_Dispose( mTriMeshData.triMeshAttributeSet );
	}
}





//=============================================================================
//      QGTriMesh::BuildMesh :	An empty function which may be used as
//			a standard interface for derived classes to construct their
//			trimesh data on the fly.
//-----------------------------------------------------------------------------
void QGTriMesh::BuildMesh()
{
	#if DEBUG
		DEBUGSTR("\pBuildMesh() called on base QGTriMesh!");
	#endif
}





//=============================================================================
//      QGTriMesh::Submit :	Recalculate the bounding box if needed, then
//			submit the trimesh to the given view.
//-----------------------------------------------------------------------------
TQ3Status QGTriMesh::Submit(const TQ3ViewObject& view)
{
	// if dirty, recalculate the bounding box
	if (mBoundsDirty) DoneWithChanges();
	
	// then, sumbit the TQ3TriMeshData
	return Q3TriMesh_Submit( &mTriMeshData, view );
}





//=============================================================================
//      QGTriMesh::ComputeNormals :	Calculates triangle and vertex normals
//			for the current geometry.  (Vertex normals are optional.)
//-----------------------------------------------------------------------------
void QGTriMesh::ComputeNormals( TQ3Boolean doVertexNormals )
{
	// This function assumes the triangles and vertices are all set up,
	// and computes appropriate vertex and triangle normals.
	
	// First, compute the triangle normals, by taking the cross product
	// of the first two segments.
	TQ3Uns32 t;
	for (t=0; t<mTriMeshData.numTriangles; t++) {
		TQ3Point3D *p0 = &mTriMeshData.points[mTriMeshData.triangles[t].pointIndices[0]];
		TQ3Point3D *p1 = &mTriMeshData.points[mTriMeshData.triangles[t].pointIndices[1]];
		TQ3Point3D *p2 = &mTriMeshData.points[mTriMeshData.triangles[t].pointIndices[2]];
		TQ3Vector3D v1, v2;
		Q3Point3D_Subtract( p0, p1, &v1 );
		Q3Point3D_Subtract( p1, p2, &v2 );
		Q3Vector3D_Cross( &v1, &v2, &FaceNormals()[t] );
	}

	if (mSmooth and doVertexNormals) {
		// Triangle normals aren't used if you're using vector normals...
		// But they're a good first step anyway.

		// Now, we need to compute vector normals by averaging the face
		// normals adjoining each vertex.
		TQ3Uns32 v;
		for (v=0; v<mTriMeshData.numPoints; v++) {
			VertexNormals()[v].x = VertexNormals()[v].y = VertexNormals()[v].z = 0;	
		}
		
		for (t=0; t<mTriMeshData.numTriangles; t++) {
			TQ3Uns32 v = mTriMeshData.triangles[t].pointIndices[0];
			// OFI: replace this with fast inline code:
			Q3Vector3D_Add( &VertexNormals()[v], &FaceNormals()[t], &VertexNormals()[v] );
		}

		for (v=0; v<mTriMeshData.numPoints; v++) {
			Q3Vector3D_Normalize( &VertexNormals()[v], &VertexNormals()[v] );
		}
	}
}





//=============================================================================
//      QGTriMesh::Move : Shifts all the vertex coordinates in X, Y, and Z.
//-----------------------------------------------------------------------------
void QGTriMesh::Move(const float dx, const float dy, const float dz)
{
	// update all triangle vertices...
	// since this is mere translation, no need to update normals etc.
	
	// Loop over all points in the mesh, applying the delta.
	for (TQ3Uns32 i=0; i<mTriMeshData.numPoints; i++) {
		mTriMeshData.points[i].x += dx;
		mTriMeshData.points[i].y += dy;
		mTriMeshData.points[i].z += dz;
	}
	mBoundsDirty = kQ3True;		// note that we now need to recompute the bounding box
						// OFI: shift it manually here
}





//=============================================================================
//      QGTriMesh::Scale : Multiplies all the vertex coordinates by the
//			given scale factor.
//-----------------------------------------------------------------------------
void QGTriMesh::Scale(const float scaleFactor)
{
	ASSERT( scaleFactor );
	// update all triangle vertices...
	// When scaling uniformly, no need to update normals.

	// Loop over all points in the mesh, applying the scaling.
	for (TQ3Uns32 i=0; i<mTriMeshData.numPoints; i++) {
		mTriMeshData.points[i].x *= scaleFactor;
		mTriMeshData.points[i].y *= scaleFactor;
		mTriMeshData.points[i].z *= scaleFactor;
	}
	mBoundsDirty = kQ3True;		// note that we now need to recompute the bounding box
}





//=============================================================================
//      QGTriMesh::Scale : Multiplies all the vertex coordinates by the
//			separate scale factors in X, Y, and Z.
//-----------------------------------------------------------------------------
void QGTriMesh::Scale(const float scaleX, const float scaleY, const float scaleZ)
{
	ASSERT( scaleX and scaleY and scaleZ );
	// update all triangle vertices...
	// Since we're probably scaling non-uniformly, we must then update normals.
	
	// Loop over all points in the mesh, applying the scaling.
	float len = sqrt( scaleX*scaleX + scaleY*scaleY + scaleZ*scaleZ );
	for (TQ3Uns32 i=0; i<mTriMeshData.numPoints; i++) {
		mTriMeshData.points[i].x *= scaleX;
		mTriMeshData.points[i].y *= scaleY;
		mTriMeshData.points[i].z *= scaleZ;
		if (mSmooth) {
			VertexNormals()[i].x = (VertexNormals()[i].x * scaleX) / len;
			VertexNormals()[i].y = (VertexNormals()[i].y * scaleY) / len;
			VertexNormals()[i].z = (VertexNormals()[i].z * scaleZ) / len;
		}
	}
	ComputeNormals(kQ3False);
	mBoundsDirty = kQ3True;		// note that we now need to recompute the bounding box
}





//=============================================================================
//      QGTriMesh::Transform : Transforms all vertex coordinates by an
//			arbitrary matrix.
//-----------------------------------------------------------------------------
void QGTriMesh::Transform( const TQ3Matrix4x4& transform )
{
	TQ3Uns32 i;
	if (!mSmooth) {

		// No smoothing (vertex normals)?  Just transform the points...
		for (i=0; i<mTriMeshData.numPoints; i++) {
			Q3Point3D_Transform(&mTriMeshData.points[i], &transform, &mTriMeshData.points[i]);
		}
		// ...then recompute triangle normals
		ComputeNormals(kQ3False);

	} else {

		// We want to recompute normals according to the transformation, rather than
		// computing them from scratch.  Start by transforming 0, which will act as
		// the base of a normal; we can then just transform each normal and subtract
		// nbase, and normalize, to get new normals!
		TQ3Vector3D nbase = {0,0,0};
		Q3Vector3D_Transform(&nbase, &transform, &nbase);
		for (i=0; i<mTriMeshData.numPoints; i++) {
			// transform the point
			Q3Point3D_Transform(&mTriMeshData.points[i], &transform, &mTriMeshData.points[i]);
			// transform its normal vector, and normalize
			Q3Vector3D_Transform(&VertexNormals()[i], &transform, &VertexNormals()[i]);
			float len = Q3Vector3D_Length( &VertexNormals()[i] );
			VertexNormals()[i].x = (VertexNormals()[i].x - nbase.x) / len;
			VertexNormals()[i].y = (VertexNormals()[i].y - nbase.y) / len;
			VertexNormals()[i].z = (VertexNormals()[i].z - nbase.z) / len;
		}
	}
	
	mBoundsDirty = kQ3True;		// note that we now need to recompute the bounding box
	// significant OFI: transform the bounding box as well!
}





//=============================================================================
//      QGTriMesh::FlipSurface : Inverts all the faces in the trimesh,
//			so that the back surface becomes the front.
//-----------------------------------------------------------------------------
void QGTriMesh::FlipSurface()
{
	// flip every triangle in the mesh, making the other surface the front
	for (TQ3Uns32 i=0; i<mTriMeshData.numTriangles; i++) {
		SWAP(mTriMeshData.triangles[i].pointIndices[0], mTriMeshData.triangles[i].pointIndices[1]);
	}
}





//=============================================================================
//      QGTriMesh::LineIntersectsMesh : returns whether a given Line 
//			intersects the mesh, outPoint contains the point hit (if any).
//			should possibly return the triangle hit too.
//-----------------------------------------------------------------------------
TQ3Boolean QGTriMesh::LineIntersectsMesh(const TQ3Point3D& p1,const TQ3Point3D& p2, TQ3Point3D *outPoint)//intersection point
{
	// check bounding box first
	if (not QG_LineIntersectsBox(p1, p2, mTriMeshData.bBox)) return kQ3False;

	// check every triangle in the mesh
	for (TQ3Uns32 i=0; i<mTriMeshData.numTriangles; i++) {
		TQ3Uns32 *indices = mTriMeshData.triangles[i].pointIndices;
		if (QG_LineIntersectsFacet(p1,p2,
									mTriMeshData.points[indices[0]],
									mTriMeshData.points[indices[1]],
									mTriMeshData.points[indices[2]],
									outPoint)) return kQ3True;
	}
	
	return kQ3False;
}


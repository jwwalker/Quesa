//	QGTriMesh.h
//
//	This file encapsulates a QuickDraw 3D QGTriMesh in a nice C++ class,
//	to handle some of the housekeeping for you.
//
//	There are two ways to build a QGTriMesh.  First, you may have it
//	copy its data from an existing Quesa TriMesh (or another QGTriMesh);
//	this is handy when grabbing TriMeshes from a 3DMF file, for example.
//	Second, you may derive your own class from QGTriMesh and implement the
//	BuildMesh method; this is handy when you need to build a mesh on the
//	fly.
//
//	The QGTriMesh may be altered in limited ways after it is created:
//	You may change the coordinates of the points, or other values in
//	the array (such as vertex normals), but you may not change the
//	number of points or faces present.

#ifndef QGTRIMESH_H
#define QGTRIMESH_H

#include "Quesa.h"
#include "QuesaGeometry.h"
#include "QuesaMath.h"

class QGTriMesh
{
  public:
	QGTriMesh(TQ3Uns32 qtyVertices, TQ3Uns32 qtyTriangles, 
			TQ3Boolean textured=kQ3False, TQ3Boolean smooth=kQ3True);		// constructor
	QGTriMesh(const QGTriMesh&);				// constructor -- copies all data
	QGTriMesh(TQ3GeometryObject triMesh);		// constructor -- copies Quesa Trimesh data
	virtual ~QGTriMesh();		// destructor
	
	// this method actually builds the mesh -- derived classes must override
	virtual void BuildMesh();
	
	// utilities to help you build it
	void ComputeNormals( TQ3Boolean doVertexNormals=kQ3True );
	void FlipSurface();		// (flips every triangle, making it face the other way)

	// check for intersect of a given line and each mesh triangle
	virtual TQ3Boolean LineIntersectsMesh(const TQ3Point3D& p1,const TQ3Point3D& p2, TQ3Point3D *outPoint);

	// call this to submit the trimesh data for rendering:
	TQ3Status Submit( const TQ3ViewObject& );
	
	// manipulators
	virtual void Move(float dx, float dy, float dz);
	virtual void Scale(float scaleFactor);
	virtual void Scale(float scaleX, float scaleY, float scaleZ);
	virtual void Transform( const TQ3Matrix4x4& transform );
	
	// call these before and after changing the points in the mesh:
	virtual inline void PrepareForChanges();
	virtual inline void DoneWithChanges();

	// inline accessors which make it easier to get at commonly needed data
	// (be sure to call PrepareForChanges and DoneWithChanges when changing these!)
	inline short QtyPoints() const { return mTriMeshData.numPoints; }
	inline TQ3Point3D*	Points() { return (TQ3Point3D*)mTriMeshData.points; }
	inline TQ3Vector3D* VertexNormals()
		{ return mSmooth ? (TQ3Vector3D*)mTriMeshData.vertexAttributeTypes[0].data : 0; }
	inline TQ3Param2D* VertexUVs()
		{ return mTextured ? (TQ3Param2D*)mTriMeshData.vertexAttributeTypes[mSmooth?1:0].data : 0; }

	inline short QtyTriangles() const { return mTriMeshData.numTriangles; }
	inline TQ3TriMeshTriangleData* Triangles() { return (TQ3TriMeshTriangleData*)mTriMeshData.triangles; }	
	inline TQ3Vector3D* FaceNormals() { return (TQ3Vector3D*)mTriMeshData.triangleAttributeTypes[0].data; }

	inline TQ3AttributeSet Attributes() { return mTriMeshData.triMeshAttributeSet; }
	inline TQ3BoundingBox& Bounds() { return mTriMeshData.bBox; }
	
  private:
	TQ3TriMeshData mTriMeshData;	// the QGTriMesh data structure
	TQ3Boolean mSmooth;				// if kQ3True, keep vertex normals for smooth shading
	TQ3Boolean mTextured;				// if kQ3True, keep vertex UVs for texture mapping
	TQ3Boolean mBoundsDirty;			// if kQ3True, bounding box needs recalculated etc.
									// because vertex data has changed
	
	// don't call this:
	QGTriMesh& operator=(const QGTriMesh&);
};

inline void QGTriMesh::PrepareForChanges()
{
	mBoundsDirty = kQ3True;
}

inline void QGTriMesh::DoneWithChanges()
{
	// recalculate bounding box if needed
	if (mBoundsDirty) {
		Q3BoundingBox_SetFromPoints3D( 
			&mTriMeshData.bBox, mTriMeshData.points, mTriMeshData.numPoints, sizeof(TQ3Point3D));
		mBoundsDirty = kQ3False;
	}
}

#endif

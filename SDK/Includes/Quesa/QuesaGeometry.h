/*! @header QuesaGeometry.h
        Declares the Quesa geometry objects.
 */
/*  NAME:
        QuesaGeometry.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#ifndef QUESA_GEOMETRY_HDR
#define QUESA_GEOMETRY_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"
#include "QuesaSet.h"

// Disable QD3D header
#if defined(__QD3DGEOMETRY__)
#error
#endif

#define __QD3DGEOMETRY__





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// General polygon hints
typedef enum {
	kQ3GeneralPolygonShapeHintComplex			= 0,
	kQ3GeneralPolygonShapeHintConcave			= 1,
	kQ3GeneralPolygonShapeHintConvex			= 2
} TQ3GeneralPolygonShapeHint;


// Nurb limits
#define kQ3NURBCurveMaxOrder					16
#define kQ3NURBPatchMaxOrder					11


// Polyhedron edge masks
typedef enum {
	kQ3PolyhedronEdgeNone						= 0,
	kQ3PolyhedronEdge01							= (1 << 0),
	kQ3PolyhedronEdge12							= (1 << 1),
	kQ3PolyhedronEdge20							= (1 << 2),
	kQ3PolyhedronEdgeAll						= (kQ3PolyhedronEdge01 | kQ3PolyhedronEdge12 | kQ3PolyhedronEdge20)
} TQ3PolyhedronEdgeMasks;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Box data
typedef struct {
	TQ3Point3D									origin;
	TQ3Vector3D									orientation;
	TQ3Vector3D									majorAxis;
	TQ3Vector3D									minorAxis;
	TQ3AttributeSet								*faceAttributeSet;
	TQ3AttributeSet								boxAttributeSet;
} TQ3BoxData;


// Cone data
typedef struct {
	TQ3Point3D									origin;
	TQ3Vector3D									orientation;
	TQ3Vector3D									majorRadius;
	TQ3Vector3D									minorRadius;
	float										uMin;
	float										uMax;
	float										vMin;
	float										vMax;
	TQ3EndCap									caps;
	TQ3AttributeSet								interiorAttributeSet;
	TQ3AttributeSet								faceAttributeSet;
	TQ3AttributeSet								bottomAttributeSet;
	TQ3AttributeSet								coneAttributeSet;
} TQ3ConeData;


// Cylinder data
typedef struct {
	TQ3Point3D									origin;
	TQ3Vector3D									orientation;
	TQ3Vector3D									majorRadius;
	TQ3Vector3D									minorRadius;
	float										uMin;
	float										uMax;
	float										vMin;
	float										vMax;
	TQ3EndCap									caps;
	TQ3AttributeSet								interiorAttributeSet;
	TQ3AttributeSet								topAttributeSet;
	TQ3AttributeSet								faceAttributeSet;
	TQ3AttributeSet								bottomAttributeSet;
	TQ3AttributeSet								cylinderAttributeSet;
} TQ3CylinderData;


// Disk data
typedef struct {
	TQ3Point3D									origin;
	TQ3Vector3D									majorRadius;
	TQ3Vector3D									minorRadius;
	float										uMin;
	float										uMax;
	float										vMin;
	float										vMax;
	TQ3AttributeSet								diskAttributeSet;
} TQ3DiskData;


// Ellipse data
typedef struct {
	TQ3Point3D									origin;
	TQ3Vector3D									majorRadius;
	TQ3Vector3D									minorRadius;
	float										uMin;
	float										uMax;
	TQ3AttributeSet								ellipseAttributeSet;
} TQ3EllipseData;


// Ellipsoid data
typedef struct {
	TQ3Point3D									origin;
	TQ3Vector3D									orientation;
	TQ3Vector3D									majorRadius;
	TQ3Vector3D									minorRadius;
	float										uMin;
	float										uMax;
	float										vMin;
	float										vMax;
	TQ3EndCap									caps;
	TQ3AttributeSet								interiorAttributeSet;
	TQ3AttributeSet								ellipsoidAttributeSet;
} TQ3EllipsoidData;


// General polygon contour
typedef struct {
	TQ3Uns32									numVertices;
	TQ3Vertex3D									*vertices;
} TQ3GeneralPolygonContourData;


// General polygon data
typedef struct {
	TQ3Uns32									numContours;
	TQ3GeneralPolygonContourData				*contours;
	TQ3GeneralPolygonShapeHint					shapeHint;
	TQ3AttributeSet								generalPolygonAttributeSet;
} TQ3GeneralPolygonData;


// Line data
typedef struct {
	TQ3Vertex3D									vertices[2];
	TQ3AttributeSet								lineAttributeSet;
} TQ3LineData;


// Marker data
typedef struct {
	TQ3Point3D									location;
	TQ3Int32									xOffset;
	TQ3Int32									yOffset;
	TQ3Bitmap									bitmap;
	TQ3AttributeSet								markerAttributeSet;
} TQ3MarkerData;


// Mesh data (all opaque)
typedef struct OpaqueTQ3MeshComponent			*TQ3MeshComponent;
typedef struct OpaqueTQ3MeshVertex				*TQ3MeshVertex;
typedef struct OpaqueTQ3MeshFace				*TQ3MeshFace;
typedef struct OpaqueTQ3MeshEdge				*TQ3MeshEdge;
typedef struct OpaqueTQ3MeshContour				*TQ3MeshContour;

typedef struct {
	void										*var1;
	void										*var2;
	void										*var3;
	struct {
		void									*field1;
		char									field2[4];
	} var4;
} TQ3MeshIterator;


// NURB curve data
typedef struct {
	TQ3Uns32									order;
	TQ3Uns32									numPoints;
	TQ3RationalPoint4D							*controlPoints;
	float										*knots;
	TQ3AttributeSet								curveAttributeSet;
} TQ3NURBCurveData;


// NURB patch data
typedef struct {
	TQ3Uns32									order;
	TQ3Uns32									numPoints;
	TQ3RationalPoint3D							*controlPoints;
	float										*knots;
} TQ3NURBPatchTrimCurveData;

typedef struct {
	TQ3Uns32									numTrimCurves;
	TQ3NURBPatchTrimCurveData					*trimCurves;
} TQ3NURBPatchTrimLoopData;

typedef struct {
	TQ3Uns32									uOrder;
	TQ3Uns32									vOrder;
	TQ3Uns32									numRows;
	TQ3Uns32									numColumns;
	TQ3RationalPoint4D							*controlPoints;
	float										*uKnots;
	float										*vKnots;
	TQ3Uns32									numTrimLoops;
	TQ3NURBPatchTrimLoopData					*trimLoops;
	TQ3AttributeSet								patchAttributeSet;
} TQ3NURBPatchData;


// Pixmap marker data
typedef struct {
	TQ3Point3D									position;
	TQ3Int32									xOffset;
	TQ3Int32									yOffset;
	TQ3StoragePixmap							pixmap;
	TQ3AttributeSet								pixmapMarkerAttributeSet;
} TQ3PixmapMarkerData;


// Point data
typedef struct {
	TQ3Point3D									point;
	TQ3AttributeSet								pointAttributeSet;
} TQ3PointData;


// Polygon data
typedef struct {
	TQ3Uns32									numVertices;
	TQ3Vertex3D									*vertices;
	TQ3AttributeSet								polygonAttributeSet;
} TQ3PolygonData;


// Polyhedron data
typedef TQ3Uns32								TQ3PolyhedronEdge;

typedef struct {
	TQ3Uns32									vertexIndices[2];
	TQ3Uns32									triangleIndices[2];
	TQ3AttributeSet								edgeAttributeSet;
} TQ3PolyhedronEdgeData;

typedef struct {
	TQ3Uns32									vertexIndices[3];
	TQ3PolyhedronEdge							edgeFlag;
	TQ3AttributeSet								triangleAttributeSet;
} TQ3PolyhedronTriangleData;

typedef struct {
	TQ3Uns32									numVertices;
	TQ3Vertex3D									*vertices;
	TQ3Uns32									numEdges;
	TQ3PolyhedronEdgeData						*edges;
	TQ3Uns32									numTriangles;
	TQ3PolyhedronTriangleData					*triangles;
	TQ3AttributeSet								polyhedronAttributeSet;
} TQ3PolyhedronData;


// Polyline data
typedef struct {
	TQ3Uns32									numVertices;
	TQ3Vertex3D									*vertices;
	TQ3AttributeSet								*segmentAttributeSet;
	TQ3AttributeSet								polyLineAttributeSet;
} TQ3PolyLineData;


// Torus data
typedef struct {
	TQ3Point3D									origin;
	TQ3Vector3D									orientation;
	TQ3Vector3D									majorRadius;
	TQ3Vector3D									minorRadius;
	float										ratio;
	float										uMin;
	float										uMax;
	float										vMin;
	float										vMax;
	TQ3EndCap									caps;
	TQ3AttributeSet								interiorAttributeSet;
	TQ3AttributeSet								torusAttributeSet;
} TQ3TorusData;


// Triangle data
typedef struct {
	TQ3Vertex3D									vertices[3];
	TQ3AttributeSet								triangleAttributeSet;
} TQ3TriangleData;


// TriGrid data
typedef struct {
	TQ3Uns32									numRows;
	TQ3Uns32									numColumns;
	TQ3Vertex3D									*vertices;
	TQ3AttributeSet								*facetAttributeSet;
	TQ3AttributeSet								triGridAttributeSet;
} TQ3TriGridData;


// TriMesh data
typedef struct {
	TQ3Uns32									pointIndices[3];
} TQ3TriMeshTriangleData;

typedef struct {
	TQ3Uns32									pointIndices[2];
	TQ3Uns32									triangleIndices[2];
} TQ3TriMeshEdgeData;

typedef struct {
	TQ3AttributeType							attributeType;
	void										*data;
	char										*attributeUseArray;
} TQ3TriMeshAttributeData;

typedef struct {
	TQ3AttributeSet								triMeshAttributeSet;

	TQ3Uns32									numTriangles;
	TQ3TriMeshTriangleData						*triangles;

	TQ3Uns32									numTriangleAttributeTypes;
	TQ3TriMeshAttributeData						*triangleAttributeTypes;

	TQ3Uns32									numEdges;
	TQ3TriMeshEdgeData							*edges;

	TQ3Uns32									numEdgeAttributeTypes;
	TQ3TriMeshAttributeData						*edgeAttributeTypes;

	TQ3Uns32									numPoints;
	TQ3Point3D									*points;

	TQ3Uns32									numVertexAttributeTypes;
	TQ3TriMeshAttributeData						*vertexAttributeTypes;

	TQ3BoundingBox								bBox;
} TQ3TriMeshData;





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Mesh iterators
#define	Q3ForEachMeshComponent(_m, _c, _i)					\
		for ((_c) = Q3Mesh_FirstMeshComponent((_m), (_i));	\
			 (_c) != NULL;									\
			 (_c) = Q3Mesh_NextMeshComponent((_i)))

#define Q3ForEachComponentVertex(_c, _v, _i)				\
		for ((_v) = Q3Mesh_FirstComponentVertex((_c), (_i));\
			 (_v) != NULL;									\
			 (_v) = Q3Mesh_NextComponentVertex((_i)))

#define Q3ForEachComponentEdge(_c, _e, _i)					\
		for ((_e) = Q3Mesh_FirstComponentEdge((_c), (_i));	\
			 (_e) != NULL;									\
			 (_e) = Q3Mesh_NextComponentEdge((_i)))

#define Q3ForEachMeshVertex(_m, _v, _i)						\
		for ((_v) = Q3Mesh_FirstMeshVertex((_m), (_i));		\
			 (_v) != NULL;									\
			 (_v) = Q3Mesh_NextMeshVertex((_i)))

#define Q3ForEachMeshFace(_m, _f, _i)						\
		for ((_f) = Q3Mesh_FirstMeshFace((_m), (_i));		\
			 (_f) != NULL;									\
			 (_f) = Q3Mesh_NextMeshFace((_i)))

#define Q3ForEachMeshEdge(_m, _e, _i)						\
		for ((_e) = Q3Mesh_FirstMeshEdge((_m), (_i));		\
			 (_e) != NULL;									\
			 (_e) = Q3Mesh_NextMeshEdge((_i)))

#define Q3ForEachVertexEdge(_v, _e, _i)						\
		for ((_e) = Q3Mesh_FirstVertexEdge((_v), (_i));		\
			 (_e) != NULL;									\
			 (_e) = Q3Mesh_NextVertexEdge((_i)))


#define Q3ForEachVertexVertex(_v, _n, _i)					\
		for ((_n) = Q3Mesh_FirstVertexVertex((_v), (_i));	\
			 (_n) != NULL;									\
			 (_n) = Q3Mesh_NextVertexVertex((_i)))

#define Q3ForEachVertexFace(_v, _f, _i)						\
		for ((_f) = Q3Mesh_FirstVertexFace((_v), (_i));		\
			 (_f) != NULL;									\
			 (_f) = Q3Mesh_NextVertexFace((_i)))

#define Q3ForEachFaceEdge(_f, _e, _i)						\
		for ((_e) = Q3Mesh_FirstFaceEdge((_f), (_i));		\
			 (_e) != NULL;									\
			 (_e) = Q3Mesh_NextFaceEdge((_i)))

#define Q3ForEachFaceVertex(_f, _v, _i)						\
		for ((_v) = Q3Mesh_FirstFaceVertex((_f), (_i));		\
			 (_v) != NULL;									\
			 (_v) = Q3Mesh_NextFaceVertex((_i)))
	
#define Q3ForEachFaceFace(_f, _n, _i)						\
		for ((_n) = Q3Mesh_FirstFaceFace((_f), (_i));  		\
			 (_n) != NULL;									\
			 (_n) = Q3Mesh_NextFaceFace((_i)))
		  
#define Q3ForEachFaceContour(_f, _h, _i)					\
		for ((_h) = Q3Mesh_FirstFaceContour((_f), (_i));	\
		     (_h) != NULL;									\
		     (_h) = Q3Mesh_NextFaceContour((_i)))

#define Q3ForEachContourEdge(_h, _e, _i)					\
		for ((_e) = Q3Mesh_FirstContourEdge((_h), (_i));	\
		     (_e) != NULL;									\
		     (_e) = Q3Mesh_NextContourEdge((_i)))

#define Q3ForEachContourVertex(_h, _v, _i)					\
		for ((_v) = Q3Mesh_FirstContourVertex((_h), (_i));	\
		     (_v) != NULL;									\
		     (_v) = Q3Mesh_NextContourVertex((_i)))

#define Q3ForEachContourFace(_h, _f, _i)					\
		for ((_f) = Q3Mesh_FirstContourFace((_h), (_i));	\
		     (_f) != NULL;									\
		     (_f) = Q3Mesh_NextContourFace((_i)))





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*
 *	Q3Geometry_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Geometry_GetType (
	TQ3GeometryObject             geometry
);



/*
 *	Q3Geometry_GetAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Geometry_GetAttributeSet (
	TQ3GeometryObject             geometry,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3Geometry_SetAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Geometry_SetAttributeSet (
	TQ3GeometryObject             geometry,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3Geometry_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Geometry_Submit (
	TQ3GeometryObject             geometry,
	TQ3ViewObject                 view
);



/*
 *	Q3Box_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Box_New (
	const TQ3BoxData              *boxData
);



/*
 *	Q3Box_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_Submit (
	const TQ3BoxData              *boxData,
	TQ3ViewObject                 view
);



/*
 *	Q3Box_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetData (
	TQ3GeometryObject             box,
	const TQ3BoxData              *boxData
);



/*
 *	Q3Box_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetData (
	TQ3GeometryObject             box,
	TQ3BoxData                    *boxData
);



/*
 *	Q3Box_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_EmptyData (
	TQ3BoxData                    *boxData
);



/*
 *	Q3Box_SetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetOrigin (
	TQ3GeometryObject             box,
	const TQ3Point3D              *origin
);



/*
 *	Q3Box_SetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetOrientation (
	TQ3GeometryObject             box,
	const TQ3Vector3D             *orientation
);



/*
 *	Q3Box_SetMajorAxis
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetMajorAxis (
	TQ3GeometryObject             box,
	const TQ3Vector3D             *majorAxis
);



/*
 *	Q3Box_SetMinorAxis
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetMinorAxis (
	TQ3GeometryObject             box,
	const TQ3Vector3D             *minorAxis
);



/*
 *	Q3Box_GetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetOrigin (
	TQ3GeometryObject             box,
	TQ3Point3D                    *origin
);



/*
 *	Q3Box_GetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetOrientation (
	TQ3GeometryObject             box,
	TQ3Vector3D                   *orientation
);



/*
 *	Q3Box_GetMajorAxis
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetMajorAxis (
	TQ3GeometryObject             box,
	TQ3Vector3D                   *majorAxis
);



/*
 *	Q3Box_GetMinorAxis
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetMinorAxis (
	TQ3GeometryObject             box,
	TQ3Vector3D                   *minorAxis
);



/*
 *	Q3Box_GetFaceAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetFaceAttributeSet (
	TQ3GeometryObject             box,
	TQ3Uns32                      faceIndex,
	TQ3AttributeSet               *faceAttributeSet
);



/*
 *	Q3Box_SetFaceAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetFaceAttributeSet (
	TQ3GeometryObject             box,
	TQ3Uns32                      faceIndex,
	TQ3AttributeSet               faceAttributeSet
);



/*
 *	Q3Cone_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Cone_New (
	const TQ3ConeData             *coneData
);



/*
 *	Q3Cone_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_Submit (
	const TQ3ConeData             *coneData,
	TQ3ViewObject                 view
);



/*
 *	Q3Cone_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetData (
	TQ3GeometryObject             cone,
	const TQ3ConeData             *coneData
);



/*
 *	Q3Cone_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetData (
	TQ3GeometryObject             cone,
	TQ3ConeData                   *coneData
);



/*
 *	Q3Cone_SetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetOrigin (
	TQ3GeometryObject             cone,
	const TQ3Point3D              *origin
);



/*
 *	Q3Cone_SetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetOrientation (
	TQ3GeometryObject             cone,
	const TQ3Vector3D             *orientation
);



/*
 *	Q3Cone_SetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetMajorRadius (
	TQ3GeometryObject             cone,
	const TQ3Vector3D             *majorRadius
);



/*
 *	Q3Cone_SetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetMinorRadius (
	TQ3GeometryObject             cone,
	const TQ3Vector3D             *minorRadius
);



/*
 *	Q3Cone_GetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetOrigin (
	TQ3GeometryObject             cone,
	TQ3Point3D                    *origin
);



/*
 *	Q3Cone_GetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetOrientation (
	TQ3GeometryObject             cone,
	TQ3Vector3D                   *orientation
);



/*
 *	Q3Cone_GetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetMajorRadius (
	TQ3GeometryObject             cone,
	TQ3Vector3D                   *majorRadius
);



/*
 *	Q3Cone_GetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetMinorRadius (
	TQ3GeometryObject             cone,
	TQ3Vector3D                   *minorRadius
);



/*
 *	Q3Cone_SetCaps
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetCaps (
	TQ3GeometryObject             cone,
	TQ3EndCap                     caps
);



/*
 *	Q3Cone_GetCaps
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetCaps (
	TQ3GeometryObject             cone,
	TQ3EndCap                     *caps
);



/*
 *	Q3Cone_SetBottomAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetBottomAttributeSet (
	TQ3GeometryObject             cone,
	TQ3AttributeSet               bottomAttributeSet
);



/*
 *	Q3Cone_GetBottomAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetBottomAttributeSet (
	TQ3GeometryObject             cone,
	TQ3AttributeSet               *bottomAttributeSet
);



/*
 *	Q3Cone_SetFaceAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetFaceAttributeSet (
	TQ3GeometryObject             cone,
	TQ3AttributeSet               faceAttributeSet
);



/*
 *	Q3Cone_GetFaceAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetFaceAttributeSet (
	TQ3GeometryObject             cone,
	TQ3AttributeSet               *faceAttributeSet
);



/*
 *	Q3Cone_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_EmptyData (
	TQ3ConeData                   *coneData
);



/*
 *	Q3Cylinder_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Cylinder_New (
	const TQ3CylinderData         *cylinderData
);



/*
 *	Q3Cylinder_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_Submit (
	const TQ3CylinderData         *cylinderData,
	TQ3ViewObject                 view
);



/*
 *	Q3Cylinder_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetData (
	TQ3GeometryObject             cylinder,
	const TQ3CylinderData         *cylinderData
);



/*
 *	Q3Cylinder_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetData (
	TQ3GeometryObject             cylinder,
	TQ3CylinderData               *cylinderData
);



/*
 *	Q3Cylinder_SetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetOrigin (
	TQ3GeometryObject             cylinder,
	const TQ3Point3D              *origin
);



/*
 *	Q3Cylinder_SetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetOrientation (
	TQ3GeometryObject             cylinder,
	const TQ3Vector3D             *orientation
);



/*
 *	Q3Cylinder_SetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetMajorRadius (
	TQ3GeometryObject             cylinder,
	const TQ3Vector3D             *majorRadius
);



/*
 *	Q3Cylinder_SetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetMinorRadius (
	TQ3GeometryObject             cylinder,
	const TQ3Vector3D             *minorRadius
);



/*
 *	Q3Cylinder_GetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetOrigin (
	TQ3GeometryObject             cylinder,
	TQ3Point3D                    *origin
);



/*
 *	Q3Cylinder_GetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetOrientation (
	TQ3GeometryObject             cylinder,
	TQ3Vector3D                   *orientation
);



/*
 *	Q3Cylinder_GetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetMajorRadius (
	TQ3GeometryObject             cylinder,
	TQ3Vector3D                   *majorRadius
);



/*
 *	Q3Cylinder_GetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetMinorRadius (
	TQ3GeometryObject             cylinder,
	TQ3Vector3D                   *minorRadius
);



/*
 *	Q3Cylinder_SetCaps
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetCaps (
	TQ3GeometryObject             cylinder,
	TQ3EndCap                     caps
);



/*
 *	Q3Cylinder_GetCaps
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetCaps (
	TQ3GeometryObject             cylinder,
	TQ3EndCap                     *caps
);



/*
 *	Q3Cylinder_SetTopAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetTopAttributeSet (
	TQ3GeometryObject             cylinder,
	TQ3AttributeSet               topAttributeSet
);



/*
 *	Q3Cylinder_GetTopAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetTopAttributeSet (
	TQ3GeometryObject             cylinder,
	TQ3AttributeSet               *topAttributeSet
);



/*
 *	Q3Cylinder_SetBottomAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetBottomAttributeSet (
	TQ3GeometryObject             cylinder,
	TQ3AttributeSet               bottomAttributeSet
);



/*
 *	Q3Cylinder_GetBottomAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetBottomAttributeSet (
	TQ3GeometryObject             cylinder,
	TQ3AttributeSet               *bottomAttributeSet
);



/*
 *	Q3Cylinder_SetFaceAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetFaceAttributeSet (
	TQ3GeometryObject             cylinder,
	TQ3AttributeSet               faceAttributeSet
);



/*
 *	Q3Cylinder_GetFaceAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetFaceAttributeSet (
	TQ3GeometryObject             cylinder,
	TQ3AttributeSet               *faceAttributeSet
);



/*
 *	Q3Cylinder_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_EmptyData (
	TQ3CylinderData               *cylinderData
);



/*
 *	Q3Disk_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Disk_New (
	const TQ3DiskData             *diskData
);



/*
 *	Q3Disk_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_Submit (
	const TQ3DiskData             *diskData,
	TQ3ViewObject                 view
);



/*
 *	Q3Disk_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_SetData (
	TQ3GeometryObject             disk,
	const TQ3DiskData             *diskData
);



/*
 *	Q3Disk_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_GetData (
	TQ3GeometryObject             disk,
	TQ3DiskData                   *diskData
);



/*
 *	Q3Disk_SetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_SetOrigin (
	TQ3GeometryObject             disk,
	const TQ3Point3D              *origin
);



/*
 *	Q3Disk_SetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_SetMajorRadius (
	TQ3GeometryObject             disk,
	const TQ3Vector3D             *majorRadius
);



/*
 *	Q3Disk_SetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_SetMinorRadius (
	TQ3GeometryObject             disk,
	const TQ3Vector3D             *minorRadius
);



/*
 *	Q3Disk_GetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_GetOrigin (
	TQ3GeometryObject             disk,
	TQ3Point3D                    *origin
);



/*
 *	Q3Disk_GetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_GetMajorRadius (
	TQ3GeometryObject             disk,
	TQ3Vector3D                   *majorRadius
);



/*
 *	Q3Disk_GetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_GetMinorRadius (
	TQ3GeometryObject             disk,
	TQ3Vector3D                   *minorRadius
);



/*
 *	Q3Disk_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_EmptyData (
	TQ3DiskData                   *diskData
);



/*
 *	Q3Ellipse_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Ellipse_New (
	const TQ3EllipseData          *ellipseData
);



/*
 *	Q3Ellipse_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_Submit (
	const TQ3EllipseData          *ellipseData,
	TQ3ViewObject                 view
);



/*
 *	Q3Ellipse_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetData (
	TQ3GeometryObject             ellipse,
	const TQ3EllipseData          *ellipseData
);



/*
 *	Q3Ellipse_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetData (
	TQ3GeometryObject             ellipse,
	TQ3EllipseData                *ellipseData
);



/*
 *	Q3Ellipse_SetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetOrigin (
	TQ3GeometryObject             ellipse,
	const TQ3Point3D              *origin
);



/*
 *	Q3Ellipse_SetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetMajorRadius (
	TQ3GeometryObject             ellipse,
	const TQ3Vector3D             *majorRadius
);



/*
 *	Q3Ellipse_SetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetMinorRadius (
	TQ3GeometryObject             ellipse,
	const TQ3Vector3D             *minorRadius
);



/*
 *	Q3Ellipse_GetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetOrigin (
	TQ3GeometryObject             ellipse,
	TQ3Point3D                    *origin
);



/*
 *	Q3Ellipse_GetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetMajorRadius (
	TQ3GeometryObject             ellipse,
	TQ3Vector3D                   *majorRadius
);



/*
 *	Q3Ellipse_GetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetMinorRadius (
	TQ3GeometryObject             ellipse,
	TQ3Vector3D                   *minorRadius
);



/*
 *	Q3Ellipse_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_EmptyData (
	TQ3EllipseData                *ellipseData
);



/*
 *	Q3Ellipsoid_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Ellipsoid_New (
	const TQ3EllipsoidData        *ellipsoidData
);



/*
 *	Q3Ellipsoid_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_Submit (
	const TQ3EllipsoidData        *ellipsoidData,
	TQ3ViewObject                 view
);



/*
 *	Q3Ellipsoid_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetData (
	TQ3GeometryObject             ellipsoid,
	const TQ3EllipsoidData        *ellipsoidData
);



/*
 *	Q3Ellipsoid_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetData (
	TQ3GeometryObject             ellipsoid,
	TQ3EllipsoidData              *ellipsoidData
);



/*
 *	Q3Ellipsoid_SetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetOrigin (
	TQ3GeometryObject             ellipsoid,
	const TQ3Point3D              *origin
);



/*
 *	Q3Ellipsoid_SetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetOrientation (
	TQ3GeometryObject             ellipsoid,
	const TQ3Vector3D             *orientation
);



/*
 *	Q3Ellipsoid_SetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetMajorRadius (
	TQ3GeometryObject             ellipsoid,
	const TQ3Vector3D             *majorRadius
);



/*
 *	Q3Ellipsoid_SetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetMinorRadius (
	TQ3GeometryObject             ellipsoid,
	const TQ3Vector3D             *minorRadius
);



/*
 *	Q3Ellipsoid_GetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetOrigin (
	TQ3GeometryObject             ellipsoid,
	TQ3Point3D                    *origin
);



/*
 *	Q3Ellipsoid_GetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetOrientation (
	TQ3GeometryObject             ellipsoid,
	TQ3Vector3D                   *orientation
);



/*
 *	Q3Ellipsoid_GetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetMajorRadius (
	TQ3GeometryObject             ellipsoid,
	TQ3Vector3D                   *majorRadius
);



/*
 *	Q3Ellipsoid_GetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetMinorRadius (
	TQ3GeometryObject             ellipsoid,
	TQ3Vector3D                   *minorRadius
);



/*
 *	Q3Ellipsoid_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_EmptyData (
	TQ3EllipsoidData              *ellipsoidData
);



/*
 *	Q3GeneralPolygon_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3GeneralPolygon_New (
	const TQ3GeneralPolygonData   *generalPolygonData
);



/*
 *	Q3GeneralPolygon_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_Submit (
	const TQ3GeneralPolygonData   *generalPolygonData,
	TQ3ViewObject                 view
);



/*
 *	Q3GeneralPolygon_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetData (
	TQ3GeometryObject             generalPolygon,
	const TQ3GeneralPolygonData   *generalPolygonData
);



/*
 *	Q3GeneralPolygon_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetData (
	TQ3GeometryObject             polygon,
	TQ3GeneralPolygonData         *generalPolygonData
);



/*
 *	Q3GeneralPolygon_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_EmptyData (
	TQ3GeneralPolygonData         *generalPolygonData
);



/*
 *	Q3GeneralPolygon_GetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetVertexPosition (
	TQ3GeometryObject             generalPolygon,
	TQ3Uns32                      contourIndex,
	TQ3Uns32                      pointIndex,
	TQ3Point3D                    *position
);



/*
 *	Q3GeneralPolygon_SetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetVertexPosition (
	TQ3GeometryObject             generalPolygon,
	TQ3Uns32                      contourIndex,
	TQ3Uns32                      pointIndex,
	const TQ3Point3D              *position
);



/*
 *	Q3GeneralPolygon_GetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetVertexAttributeSet (
	TQ3GeometryObject             generalPolygon,
	TQ3Uns32                      contourIndex,
	TQ3Uns32                      pointIndex,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3GeneralPolygon_SetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetVertexAttributeSet (
	TQ3GeometryObject             generalPolygon,
	TQ3Uns32                      contourIndex,
	TQ3Uns32                      pointIndex,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3GeneralPolygon_SetShapeHint
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetShapeHint (
	TQ3GeometryObject             generalPolygon,
	TQ3GeneralPolygonShapeHint    shapeHint
);



/*
 *	Q3GeneralPolygon_GetShapeHint
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetShapeHint (
	TQ3GeometryObject             generalPolygon,
	TQ3GeneralPolygonShapeHint    *shapeHint
);



/*
 *	Q3Line_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Line_New (
	const TQ3LineData             *lineData
);



/*
 *	Q3Line_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_Submit (
	const TQ3LineData             *lineData,
	TQ3ViewObject                 view
);



/*
 *	Q3Line_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_GetData (
	TQ3GeometryObject             line,
	TQ3LineData                   *lineData
);



/*
 *	Q3Line_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_SetData (
	TQ3GeometryObject             line,
	const TQ3LineData             *lineData
);



/*
 *	Q3Line_GetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_GetVertexPosition (
	TQ3GeometryObject             line,
	TQ3Uns32                      index,
	TQ3Point3D                    *position
);



/*
 *	Q3Line_SetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_SetVertexPosition (
	TQ3GeometryObject             line,
	TQ3Uns32                      index,
	const TQ3Point3D              *position
);



/*
 *	Q3Line_GetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_GetVertexAttributeSet (
	TQ3GeometryObject             line,
	TQ3Uns32                      index,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3Line_SetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_SetVertexAttributeSet (
	TQ3GeometryObject             line,
	TQ3Uns32                      index,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3Line_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_EmptyData (
	TQ3LineData                   *lineData
);



/*
 *	Q3Marker_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Marker_New (
	const TQ3MarkerData           *markerData
);



/*
 *	Q3Marker_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_Submit (
	const TQ3MarkerData           *markerData,
	TQ3ViewObject                 view
);



/*
 *	Q3Marker_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_SetData (
	TQ3GeometryObject             geometry,
	const TQ3MarkerData           *markerData
);



/*
 *	Q3Marker_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_GetData (
	TQ3GeometryObject             geometry,
	TQ3MarkerData                 *markerData
);



/*
 *	Q3Marker_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_EmptyData (
	TQ3MarkerData                 *markerData
);



/*
 *	Q3Marker_GetPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_GetPosition (
	TQ3GeometryObject             marker,
	TQ3Point3D                    *location
);



/*
 *	Q3Marker_SetPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_SetPosition (
	TQ3GeometryObject             marker,
	const TQ3Point3D              *location
);



/*
 *	Q3Marker_GetXOffset
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_GetXOffset (
	TQ3GeometryObject             marker,
	TQ3Int32                      *xOffset
);



/*
 *	Q3Marker_SetXOffset
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_SetXOffset (
	TQ3GeometryObject             marker,
	TQ3Int32                      xOffset
);



/*
 *	Q3Marker_GetYOffset
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_GetYOffset (
	TQ3GeometryObject             marker,
	TQ3Int32                      *yOffset
);



/*
 *	Q3Marker_SetYOffset
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_SetYOffset (
	TQ3GeometryObject             marker,
	TQ3Int32                      yOffset
);



/*
 *	Q3Marker_GetBitmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_GetBitmap (
	TQ3GeometryObject             marker,
	TQ3Bitmap                     *bitmap
);



/*
 *	Q3Marker_SetBitmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_SetBitmap (
	TQ3GeometryObject             marker,
	const TQ3Bitmap               *bitmap
);



/*
 *	Q3Mesh_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Mesh_New (
	void
);



/*
 *	Q3Mesh_VertexNew
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_VertexNew (
	TQ3GeometryObject             mesh,
	const TQ3Vertex3D             *vertex
);



/*
 *	Q3Mesh_FaceNew
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_FaceNew (
	TQ3GeometryObject             mesh,
	TQ3Uns32                      numVertices,
	const TQ3MeshVertex           *vertices,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3Mesh_VertexDelete
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_VertexDelete (
	TQ3GeometryObject             mesh,
	TQ3MeshVertex                 vertex
);



/*
 *	Q3Mesh_FaceDelete
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_FaceDelete (
	TQ3GeometryObject             mesh,
	TQ3MeshFace                   face
);



/*
 *	Q3Mesh_DelayUpdates
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_DelayUpdates (
	TQ3GeometryObject             mesh
);



/*
 *	Q3Mesh_ResumeUpdates
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_ResumeUpdates (
	TQ3GeometryObject             mesh
);



/*
 *	Q3Mesh_FaceToContour
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshContour  )
Q3Mesh_FaceToContour (
	TQ3GeometryObject             mesh,
	TQ3MeshFace                   containerFace,
	TQ3MeshFace                   face
);



/*
 *	Q3Mesh_ContourToFace
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_ContourToFace (
	TQ3GeometryObject             mesh,
	TQ3MeshContour                contour
);



/*
 *	Q3Mesh_GetNumComponents
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumComponents (
	TQ3GeometryObject             mesh,
	TQ3Uns32                      *numComponents
);



/*
 *	Q3Mesh_GetNumEdges
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumEdges (
	TQ3GeometryObject             mesh,
	TQ3Uns32                      *numEdges
);



/*
 *	Q3Mesh_GetNumVertices
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumVertices (
	TQ3GeometryObject             mesh,
	TQ3Uns32                      *numVertices
);



/*
 *	Q3Mesh_GetNumFaces
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumFaces (
	TQ3GeometryObject             mesh,
	TQ3Uns32                      *numFaces
);



/*
 *	Q3Mesh_GetNumCorners
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumCorners (
	TQ3GeometryObject             mesh,
	TQ3Uns32                      *numCorners
);



/*
 *	Q3Mesh_GetOrientable
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetOrientable (
	TQ3GeometryObject             mesh,
	TQ3Boolean                    *orientable
);



/*
 *	Q3Mesh_GetComponentNumVertices
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentNumVertices (
	TQ3GeometryObject             mesh,
	TQ3MeshComponent              component,
	TQ3Uns32                      *numVertices
);



/*
 *	Q3Mesh_GetComponentNumEdges
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentNumEdges (
	TQ3GeometryObject             mesh,
	TQ3MeshComponent              component,
	TQ3Uns32                      *numEdges
);



/*
 *	Q3Mesh_GetComponentBoundingBox
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentBoundingBox (
	TQ3GeometryObject             mesh,
	TQ3MeshComponent              component,
	TQ3BoundingBox                *boundingBox
);



/*
 *	Q3Mesh_GetComponentOrientable
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentOrientable (
	TQ3GeometryObject             mesh,
	TQ3MeshComponent              component,
	TQ3Boolean                    *orientable
);



/*
 *	Q3Mesh_GetVertexCoordinates
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexCoordinates (
	TQ3GeometryObject             mesh,
	TQ3MeshVertex                 vertex,
	TQ3Point3D                    *coordinates
);



/*
 *	Q3Mesh_GetVertexIndex
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexIndex (
	TQ3GeometryObject             mesh,
	TQ3MeshVertex                 vertex,
	TQ3Uns32                      *index
);



/*
 *	Q3Mesh_GetVertexOnBoundary
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexOnBoundary (
	TQ3GeometryObject             mesh,
	TQ3MeshVertex                 vertex,
	TQ3Boolean                    *onBoundary
);



/*
 *	Q3Mesh_GetVertexComponent
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexComponent (
	TQ3GeometryObject             mesh,
	TQ3MeshVertex                 vertex,
	TQ3MeshComponent              *component
);



/*
 *	Q3Mesh_GetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexAttributeSet (
	TQ3GeometryObject             mesh,
	TQ3MeshVertex                 vertex,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3Mesh_SetVertexCoordinates
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetVertexCoordinates (
	TQ3GeometryObject             mesh,
	TQ3MeshVertex                 vertex,
	const TQ3Point3D              *coordinates
);



/*
 *	Q3Mesh_SetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetVertexAttributeSet (
	TQ3GeometryObject             mesh,
	TQ3MeshVertex                 vertex,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3Mesh_GetFaceNumVertices
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceNumVertices (
	TQ3GeometryObject             mesh,
	TQ3MeshFace                   face,
	TQ3Uns32                      *numVertices
);



/*
 *	Q3Mesh_GetFacePlaneEquation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFacePlaneEquation (
	TQ3GeometryObject             mesh,
	TQ3MeshFace                   face,
	TQ3PlaneEquation              *planeEquation
);



/*
 *	Q3Mesh_GetFaceNumContours
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceNumContours (
	TQ3GeometryObject             mesh,
	TQ3MeshFace                   face,
	TQ3Uns32                      *numContours
);



/*
 *	Q3Mesh_GetFaceIndex
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceIndex (
	TQ3GeometryObject             mesh,
	TQ3MeshFace                   face,
	TQ3Uns32                      *index
);



/*
 *	Q3Mesh_GetFaceComponent
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceComponent (
	TQ3GeometryObject             mesh,
	TQ3MeshFace                   face,
	TQ3MeshComponent              *component
);



/*
 *	Q3Mesh_GetFaceAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceAttributeSet (
	TQ3GeometryObject             mesh,
	TQ3MeshFace                   face,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3Mesh_SetFaceAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetFaceAttributeSet (
	TQ3GeometryObject             mesh,
	TQ3MeshFace                   face,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3Mesh_GetEdgeVertices
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeVertices (
	TQ3GeometryObject             mesh,
	TQ3MeshEdge                   edge,
	TQ3MeshVertex                 *vertex1,
	TQ3MeshVertex                 *vertex2
);



/*
 *	Q3Mesh_GetEdgeFaces
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeFaces (
	TQ3GeometryObject             mesh,
	TQ3MeshEdge                   edge,
	TQ3MeshFace                   *face1,
	TQ3MeshFace                   *face2
);



/*
 *	Q3Mesh_GetEdgeOnBoundary
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeOnBoundary (
	TQ3GeometryObject             mesh,
	TQ3MeshEdge                   edge,
	TQ3Boolean                    *onBoundary
);



/*
 *	Q3Mesh_GetEdgeComponent
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeComponent (
	TQ3GeometryObject             mesh,
	TQ3MeshEdge                   edge,
	TQ3MeshComponent              *component
);



/*
 *	Q3Mesh_GetEdgeAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeAttributeSet (
	TQ3GeometryObject             mesh,
	TQ3MeshEdge                   edge,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3Mesh_SetEdgeAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetEdgeAttributeSet (
	TQ3GeometryObject             mesh,
	TQ3MeshEdge                   edge,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3Mesh_GetContourFace
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetContourFace (
	TQ3GeometryObject             mesh,
	TQ3MeshContour                contour,
	TQ3MeshFace                   *face
);



/*
 *	Q3Mesh_GetContourNumVertices
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetContourNumVertices (
	TQ3GeometryObject             mesh,
	TQ3MeshContour                contour,
	TQ3Uns32                      *numVertices
);



/*
 *	Q3Mesh_GetCornerAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetCornerAttributeSet (
	TQ3GeometryObject             mesh,
	TQ3MeshVertex                 vertex,
	TQ3MeshFace                   face,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3Mesh_SetCornerAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetCornerAttributeSet (
	TQ3GeometryObject             mesh,
	TQ3MeshVertex                 vertex,
	TQ3MeshFace                   face,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3Mesh_FirstMeshComponent
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshComponent  )
Q3Mesh_FirstMeshComponent (
	TQ3GeometryObject             mesh,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextMeshComponent
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshComponent  )
Q3Mesh_NextMeshComponent (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstComponentVertex
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_FirstComponentVertex (
	TQ3MeshComponent              component,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextComponentVertex
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_NextComponentVertex (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstComponentEdge
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_FirstComponentEdge (
	TQ3MeshComponent              component,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextComponentEdge
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_NextComponentEdge (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstMeshVertex
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_FirstMeshVertex (
	TQ3GeometryObject             mesh,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextMeshVertex
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_NextMeshVertex (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstMeshFace
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_FirstMeshFace (
	TQ3GeometryObject             mesh,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextMeshFace
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_NextMeshFace (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstMeshEdge
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_FirstMeshEdge (
	TQ3GeometryObject             mesh,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextMeshEdge
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_NextMeshEdge (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstVertexEdge
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_FirstVertexEdge (
	TQ3MeshVertex                 vertex,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextVertexEdge
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_NextVertexEdge (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstVertexVertex
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_FirstVertexVertex (
	TQ3MeshVertex                 vertex,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextVertexVertex
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_NextVertexVertex (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstVertexFace
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_FirstVertexFace (
	TQ3MeshVertex                 vertex,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextVertexFace
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_NextVertexFace (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstFaceEdge
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_FirstFaceEdge (
	TQ3MeshFace                   face,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextFaceEdge
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_NextFaceEdge (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstFaceVertex
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_FirstFaceVertex (
	TQ3MeshFace                   face,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextFaceVertex
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_NextFaceVertex (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstFaceFace
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_FirstFaceFace (
	TQ3MeshFace                   face,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextFaceFace
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_NextFaceFace (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstFaceContour
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshContour  )
Q3Mesh_FirstFaceContour (
	TQ3MeshFace                   face,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextFaceContour
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshContour  )
Q3Mesh_NextFaceContour (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstContourEdge
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_FirstContourEdge (
	TQ3MeshContour                contour,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextContourEdge
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_NextContourEdge (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstContourVertex
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_FirstContourVertex (
	TQ3MeshContour                contour,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextContourVertex
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_NextContourVertex (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_FirstContourFace
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_FirstContourFace (
	TQ3MeshContour                contour,
	TQ3MeshIterator               *iterator
);



/*
 *	Q3Mesh_NextContourFace
 *		Description of function
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_NextContourFace (
	TQ3MeshIterator               *iterator
);



/*
 *	Q3NURBCurve_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3NURBCurve_New (
	const TQ3NURBCurveData        *curveData
);



/*
 *	Q3NURBCurve_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_Submit (
	const TQ3NURBCurveData        *curveData,
	TQ3ViewObject                 view
);



/*
 *	Q3NURBCurve_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_SetData (
	TQ3GeometryObject             curve,
	const TQ3NURBCurveData        *nurbCurveData
);



/*
 *	Q3NURBCurve_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_GetData (
	TQ3GeometryObject             curve,
	TQ3NURBCurveData              *nurbCurveData
);



/*
 *	Q3NURBCurve_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_EmptyData (
	TQ3NURBCurveData              *nurbCurveData
);



/*
 *	Q3NURBCurve_SetControlPoint
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_SetControlPoint (
	TQ3GeometryObject             curve,
	TQ3Uns32                      pointIndex,
	const TQ3RationalPoint4D      *point4D
);



/*
 *	Q3NURBCurve_GetControlPoint
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_GetControlPoint (
	TQ3GeometryObject             curve,
	TQ3Uns32                      pointIndex,
	TQ3RationalPoint4D            *point4D
);



/*
 *	Q3NURBCurve_SetKnot
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_SetKnot (
	TQ3GeometryObject             curve,
	TQ3Uns32                      knotIndex,
	float                         knotValue
);



/*
 *	Q3NURBCurve_GetKnot
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_GetKnot (
	TQ3GeometryObject             curve,
	TQ3Uns32                      knotIndex,
	float                         *knotValue
);



/*
 *	Q3NURBPatch_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3NURBPatch_New (
	const TQ3NURBPatchData        *nurbPatchData
);



/*
 *	Q3NURBPatch_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_Submit (
	const TQ3NURBPatchData        *nurbPatchData,
	TQ3ViewObject                 view
);



/*
 *	Q3NURBPatch_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetData (
	TQ3GeometryObject             nurbPatch,
	const TQ3NURBPatchData        *nurbPatchData
);



/*
 *	Q3NURBPatch_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetData (
	TQ3GeometryObject             nurbPatch,
	TQ3NURBPatchData              *nurbPatchData
);



/*
 *	Q3NURBPatch_SetControlPoint
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetControlPoint (
	TQ3GeometryObject             nurbPatch,
	TQ3Uns32                      rowIndex,
	TQ3Uns32                      columnIndex,
	const TQ3RationalPoint4D      *point4D
);



/*
 *	Q3NURBPatch_GetControlPoint
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetControlPoint (
	TQ3GeometryObject             nurbPatch,
	TQ3Uns32                      rowIndex,
	TQ3Uns32                      columnIndex,
	TQ3RationalPoint4D            *point4D
);



/*
 *	Q3NURBPatch_SetUKnot
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetUKnot (
	TQ3GeometryObject             nurbPatch,
	TQ3Uns32                      knotIndex,
	float                         knotValue
);



/*
 *	Q3NURBPatch_SetVKnot
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetVKnot (
	TQ3GeometryObject             nurbPatch,
	TQ3Uns32                      knotIndex,
	float                         knotValue
);



/*
 *	Q3NURBPatch_GetUKnot
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetUKnot (
	TQ3GeometryObject             nurbPatch,
	TQ3Uns32                      knotIndex,
	float                         *knotValue
);



/*
 *	Q3NURBPatch_GetVKnot
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetVKnot (
	TQ3GeometryObject             nurbPatch,
	TQ3Uns32                      knotIndex,
	float                         *knotValue
);



/*
 *	Q3NURBPatch_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_EmptyData (
	TQ3NURBPatchData              *nurbPatchData
);



/*
 *	Q3PixmapMarker_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3PixmapMarker_New (
	const TQ3PixmapMarkerData     *pixmapMarkerData
);



/*
 *	Q3PixmapMarker_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_Submit (
	const TQ3PixmapMarkerData     *pixmapMarkerData,
	TQ3ViewObject                 view
);



/*
 *	Q3PixmapMarker_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetData (
	TQ3GeometryObject             geometry,
	const TQ3PixmapMarkerData     *pixmapMarkerData
);



/*
 *	Q3PixmapMarker_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetData (
	TQ3GeometryObject             geometry,
	TQ3PixmapMarkerData           *pixmapMarkerData
);



/*
 *	Q3PixmapMarker_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_EmptyData (
	TQ3PixmapMarkerData           *pixmapMarkerData
);



/*
 *	Q3PixmapMarker_GetPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetPosition (
	TQ3GeometryObject             pixmapMarker,
	TQ3Point3D                    *position
);



/*
 *	Q3PixmapMarker_SetPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetPosition (
	TQ3GeometryObject             pixmapMarker,
	const TQ3Point3D              *position
);



/*
 *	Q3PixmapMarker_GetXOffset
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetXOffset (
	TQ3GeometryObject             pixmapMarker,
	TQ3Int32                      *xOffset
);



/*
 *	Q3PixmapMarker_SetXOffset
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetXOffset (
	TQ3GeometryObject             pixmapMarker,
	TQ3Int32                      xOffset
);



/*
 *	Q3PixmapMarker_GetYOffset
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetYOffset (
	TQ3GeometryObject             pixmapMarker,
	TQ3Int32                      *yOffset
);



/*
 *	Q3PixmapMarker_SetYOffset
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetYOffset (
	TQ3GeometryObject             pixmapMarker,
	TQ3Int32                      yOffset
);



/*
 *	Q3PixmapMarker_GetPixmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetPixmap (
	TQ3GeometryObject             pixmapMarker,
	TQ3StoragePixmap              *pixmap
);



/*
 *	Q3PixmapMarker_SetPixmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetPixmap (
	TQ3GeometryObject             pixmapMarker,
	const TQ3StoragePixmap        *pixmap
);



/*
 *	Q3Point_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Point_New (
	const TQ3PointData            *pointData
);



/*
 *	Q3Point_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_Submit (
	const TQ3PointData            *pointData,
	TQ3ViewObject                 view
);



/*
 *	Q3Point_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_GetData (
	TQ3GeometryObject             point,
	TQ3PointData                  *pointData
);



/*
 *	Q3Point_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_SetData (
	TQ3GeometryObject             point,
	const TQ3PointData            *pointData
);



/*
 *	Q3Point_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_EmptyData (
	TQ3PointData                  *pointData
);



/*
 *	Q3Point_SetPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_SetPosition (
	TQ3GeometryObject             point,
	const TQ3Point3D              *position
);



/*
 *	Q3Point_GetPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_GetPosition (
	TQ3GeometryObject             point,
	TQ3Point3D                    *position
);



/*
 *	Q3Polygon_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Polygon_New (
	const TQ3PolygonData          *polygonData
);



/*
 *	Q3Polygon_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_Submit (
	const TQ3PolygonData          *polygonData,
	TQ3ViewObject                 view
);



/*
 *	Q3Polygon_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_SetData (
	TQ3GeometryObject             polygon,
	const TQ3PolygonData          *polygonData
);



/*
 *	Q3Polygon_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_GetData (
	TQ3GeometryObject             polygon,
	TQ3PolygonData                *polygonData
);



/*
 *	Q3Polygon_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_EmptyData (
	TQ3PolygonData                *polygonData
);



/*
 *	Q3Polygon_GetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_GetVertexPosition (
	TQ3GeometryObject             polygon,
	TQ3Uns32                      index,
	TQ3Point3D                    *point
);



/*
 *	Q3Polygon_SetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_SetVertexPosition (
	TQ3GeometryObject             polygon,
	TQ3Uns32                      index,
	const TQ3Point3D              *point
);



/*
 *	Q3Polygon_GetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_GetVertexAttributeSet (
	TQ3GeometryObject             polygon,
	TQ3Uns32                      index,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3Polygon_SetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_SetVertexAttributeSet (
	TQ3GeometryObject             polygon,
	TQ3Uns32                      index,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3Polyhedron_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Polyhedron_New (
	const TQ3PolyhedronData       *polyhedronData
);



/*
 *	Q3Polyhedron_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_Submit (
	const TQ3PolyhedronData       *polyhedronData,
	TQ3ViewObject                 view
);



/*
 *	Q3Polyhedron_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetData (
	TQ3GeometryObject             polyhedron,
	const TQ3PolyhedronData       *polyhedronData
);



/*
 *	Q3Polyhedron_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetData (
	TQ3GeometryObject             polyhedron,
	TQ3PolyhedronData             *polyhedronData
);



/*
 *	Q3Polyhedron_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_EmptyData (
	TQ3PolyhedronData             *polyhedronData
);



/*
 *	Q3Polyhedron_SetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetVertexPosition (
	TQ3GeometryObject             polyhedron,
	TQ3Uns32                      index,
	const TQ3Point3D              *point
);



/*
 *	Q3Polyhedron_GetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetVertexPosition (
	TQ3GeometryObject             polyhedron,
	TQ3Uns32                      index,
	TQ3Point3D                    *point
);



/*
 *	Q3Polyhedron_SetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetVertexAttributeSet (
	TQ3GeometryObject             polyhedron,
	TQ3Uns32                      index,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3Polyhedron_GetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetVertexAttributeSet (
	TQ3GeometryObject             polyhedron,
	TQ3Uns32                      index,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3Polyhedron_GetTriangleData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetTriangleData (
	TQ3GeometryObject             polyhedron,
	TQ3Uns32                      triangleIndex,
	TQ3PolyhedronTriangleData     *triangleData
);



/*
 *	Q3Polyhedron_SetTriangleData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetTriangleData (
	TQ3GeometryObject             polyhedron,
	TQ3Uns32                      triangleIndex,
	const TQ3PolyhedronTriangleData *triangleData
);



/*
 *	Q3Polyhedron_GetEdgeData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetEdgeData (
	TQ3GeometryObject             polyhedron,
	TQ3Uns32                      edgeIndex,
	TQ3PolyhedronEdgeData         *edgeData
);



/*
 *	Q3Polyhedron_SetEdgeData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetEdgeData (
	TQ3GeometryObject             polyhedron,
	TQ3Uns32                      edgeIndex,
	const TQ3PolyhedronEdgeData   *edgeData
);



/*
 *	Q3PolyLine_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3PolyLine_New (
	const TQ3PolyLineData         *polylineData
);



/*
 *	Q3PolyLine_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_Submit (
	const TQ3PolyLineData         *polyLineData,
	TQ3ViewObject                 view
);



/*
 *	Q3PolyLine_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetData (
	TQ3GeometryObject             polyLine,
	const TQ3PolyLineData         *polyLineData
);



/*
 *	Q3PolyLine_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetData (
	TQ3GeometryObject             polyLine,
	TQ3PolyLineData               *polyLineData
);



/*
 *	Q3PolyLine_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_EmptyData (
	TQ3PolyLineData               *polyLineData
);



/*
 *	Q3PolyLine_GetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetVertexPosition (
	TQ3GeometryObject             polyLine,
	TQ3Uns32                      index,
	TQ3Point3D                    *position
);



/*
 *	Q3PolyLine_SetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetVertexPosition (
	TQ3GeometryObject             polyLine,
	TQ3Uns32                      index,
	const TQ3Point3D              *position
);



/*
 *	Q3PolyLine_GetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetVertexAttributeSet (
	TQ3GeometryObject             polyLine,
	TQ3Uns32                      index,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3PolyLine_SetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetVertexAttributeSet (
	TQ3GeometryObject             polyLine,
	TQ3Uns32                      index,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3PolyLine_GetSegmentAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetSegmentAttributeSet (
	TQ3GeometryObject             polyLine,
	TQ3Uns32                      index,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3PolyLine_SetSegmentAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetSegmentAttributeSet (
	TQ3GeometryObject             polyLine,
	TQ3Uns32                      index,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3Torus_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Torus_New (
	const TQ3TorusData            *torusData
);



/*
 *	Q3Torus_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_Submit (
	const TQ3TorusData            *torusData,
	TQ3ViewObject                 view
);



/*
 *	Q3Torus_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetData (
	TQ3GeometryObject             torus,
	const TQ3TorusData            *torusData
);



/*
 *	Q3Torus_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetData (
	TQ3GeometryObject             torus,
	TQ3TorusData                  *torusData
);



/*
 *	Q3Torus_SetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetOrigin (
	TQ3GeometryObject             torus,
	const TQ3Point3D              *origin
);



/*
 *	Q3Torus_SetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetOrientation (
	TQ3GeometryObject             torus,
	const TQ3Vector3D             *orientation
);



/*
 *	Q3Torus_SetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetMajorRadius (
	TQ3GeometryObject             torus,
	const TQ3Vector3D             *majorRadius
);



/*
 *	Q3Torus_SetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetMinorRadius (
	TQ3GeometryObject             torus,
	const TQ3Vector3D             *minorRadius
);



/*
 *	Q3Torus_SetRatio
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetRatio (
	TQ3GeometryObject             torus,
	float                         ratio
);



/*
 *	Q3Torus_GetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetOrigin (
	TQ3GeometryObject             torus,
	TQ3Point3D                    *origin
);



/*
 *	Q3Torus_GetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetOrientation (
	TQ3GeometryObject             torus,
	TQ3Vector3D                   *orientation
);



/*
 *	Q3Torus_GetMajorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetMajorRadius (
	TQ3GeometryObject             torus,
	TQ3Vector3D                   *majorRadius
);



/*
 *	Q3Torus_GetMinorRadius
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetMinorRadius (
	TQ3GeometryObject             torus,
	TQ3Vector3D                   *minorRadius
);



/*
 *	Q3Torus_GetRatio
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetRatio (
	TQ3GeometryObject             torus,
	float                         *ratio
);



/*
 *	Q3Torus_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_EmptyData (
	TQ3TorusData                  *torusData
);



/*
 *	Q3Triangle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Triangle_New (
	const TQ3TriangleData         *triangleData
);



/*
 *	Q3Triangle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_Submit (
	const TQ3TriangleData         *triangleData,
	TQ3ViewObject                 view
);



/*
 *	Q3Triangle_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_SetData (
	TQ3GeometryObject             triangle,
	const TQ3TriangleData         *triangleData
);



/*
 *	Q3Triangle_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_GetData (
	TQ3GeometryObject             triangle,
	TQ3TriangleData               *triangleData
);



/*
 *	Q3Triangle_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_EmptyData (
	TQ3TriangleData               *triangleData
);



/*
 *	Q3Triangle_GetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_GetVertexPosition (
	TQ3GeometryObject             triangle,
	TQ3Uns32                      index,
	TQ3Point3D                    *point
);



/*
 *	Q3Triangle_SetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_SetVertexPosition (
	TQ3GeometryObject             triangle,
	TQ3Uns32                      index,
	const TQ3Point3D              *point
);



/*
 *	Q3Triangle_GetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_GetVertexAttributeSet (
	TQ3GeometryObject             triangle,
	TQ3Uns32                      index,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3Triangle_SetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_SetVertexAttributeSet (
	TQ3GeometryObject             triangle,
	TQ3Uns32                      index,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3TriGrid_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3TriGrid_New (
	const TQ3TriGridData          *triGridData
);



/*
 *	Q3TriGrid_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_Submit (
	const TQ3TriGridData          *triGridData,
	TQ3ViewObject                 view
);



/*
 *	Q3TriGrid_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetData (
	TQ3GeometryObject             triGrid,
	const TQ3TriGridData          *triGridData
);



/*
 *	Q3TriGrid_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetData (
	TQ3GeometryObject             triGrid,
	TQ3TriGridData                *triGridData
);



/*
 *	Q3TriGrid_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_EmptyData (
	TQ3TriGridData                *triGridData
);



/*
 *	Q3TriGrid_GetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetVertexPosition (
	TQ3GeometryObject             triGrid,
	TQ3Uns32                      rowIndex,
	TQ3Uns32                      columnIndex,
	TQ3Point3D                    *position
);



/*
 *	Q3TriGrid_SetVertexPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetVertexPosition (
	TQ3GeometryObject             triGrid,
	TQ3Uns32                      rowIndex,
	TQ3Uns32                      columnIndex,
	const TQ3Point3D              *position
);



/*
 *	Q3TriGrid_GetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetVertexAttributeSet (
	TQ3GeometryObject             triGrid,
	TQ3Uns32                      rowIndex,
	TQ3Uns32                      columnIndex,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3TriGrid_SetVertexAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetVertexAttributeSet (
	TQ3GeometryObject             triGrid,
	TQ3Uns32                      rowIndex,
	TQ3Uns32                      columnIndex,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3TriGrid_GetFacetAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetFacetAttributeSet (
	TQ3GeometryObject             triGrid,
	TQ3Uns32                      faceIndex,
	TQ3AttributeSet               *facetAttributeSet
);



/*
 *	Q3TriGrid_SetFacetAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetFacetAttributeSet (
	TQ3GeometryObject             triGrid,
	TQ3Uns32                      faceIndex,
	TQ3AttributeSet               facetAttributeSet
);



/*
 *	Q3TriMesh_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3TriMesh_New (
	const TQ3TriMeshData          *triMeshData
);



/*
 *	Q3TriMesh_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriMesh_Submit (
	const TQ3TriMeshData          *triMeshData,
	TQ3ViewObject                 view
);



/*
 *	Q3TriMesh_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriMesh_SetData (
	TQ3GeometryObject             triMesh,
	const TQ3TriMeshData          *triMeshData
);



/*
 *	Q3TriMesh_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriMesh_GetData (
	TQ3GeometryObject             triMesh,
	TQ3TriMeshData                *triMeshData
);



/*
 *	Q3TriMesh_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TriMesh_EmptyData (
	TQ3TriMeshData                *triMeshData
);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


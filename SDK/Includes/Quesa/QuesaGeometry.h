/*! @header QuesaGeometry.h
        Declares the Quesa geometry objects.
 */
/*  NAME:
        QuesaGeometry.h

    DESCRIPTION:
        Quesa public header.

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
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// General polygon hints
typedef enum {
    kQ3GeneralPolygonShapeHintComplex           = 0,
    kQ3GeneralPolygonShapeHintConcave           = 1,
    kQ3GeneralPolygonShapeHintConvex            = 2
} TQ3GeneralPolygonShapeHint;


// Nurb limits
#define kQ3NURBCurveMaxOrder                    16
#define kQ3NURBPatchMaxOrder                    11


// Polyhedron edge masks
typedef enum {
    kQ3PolyhedronEdgeNone                       = 0,
    kQ3PolyhedronEdge01                         = (1 << 0),
    kQ3PolyhedronEdge12                         = (1 << 1),
    kQ3PolyhedronEdge20                         = (1 << 2),
    kQ3PolyhedronEdgeAll                        = (kQ3PolyhedronEdge01 | kQ3PolyhedronEdge12 | kQ3PolyhedronEdge20)
} TQ3PolyhedronEdgeMasks;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Box data
typedef struct TQ3BoxData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Vector3D                                 majorAxis;
    TQ3Vector3D                                 minorAxis;
    TQ3AttributeSet                             *faceAttributeSet;
    TQ3AttributeSet                             boxAttributeSet;
} TQ3BoxData;


// Cone data
typedef struct TQ3ConeData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       uMin;
    float                                       uMax;
    float                                       vMin;
    float                                       vMax;
    TQ3EndCap                                   caps;
    TQ3AttributeSet                             interiorAttributeSet;
    TQ3AttributeSet                             faceAttributeSet;
    TQ3AttributeSet                             bottomAttributeSet;
    TQ3AttributeSet                             coneAttributeSet;
} TQ3ConeData;


// Cylinder data
typedef struct TQ3CylinderData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       uMin;
    float                                       uMax;
    float                                       vMin;
    float                                       vMax;
    TQ3EndCap                                   caps;
    TQ3AttributeSet                             interiorAttributeSet;
    TQ3AttributeSet                             topAttributeSet;
    TQ3AttributeSet                             faceAttributeSet;
    TQ3AttributeSet                             bottomAttributeSet;
    TQ3AttributeSet                             cylinderAttributeSet;
} TQ3CylinderData;


// Disk data
typedef struct TQ3DiskData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       uMin;
    float                                       uMax;
    float                                       vMin;
    float                                       vMax;
    TQ3AttributeSet                             diskAttributeSet;
} TQ3DiskData;


// Ellipse data
typedef struct TQ3EllipseData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       uMin;
    float                                       uMax;
    TQ3AttributeSet                             ellipseAttributeSet;
} TQ3EllipseData;


// Ellipsoid data
typedef struct TQ3EllipsoidData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       uMin;
    float                                       uMax;
    float                                       vMin;
    float                                       vMax;
    TQ3EndCap                                   caps;
    TQ3AttributeSet                             interiorAttributeSet;
    TQ3AttributeSet                             ellipsoidAttributeSet;
} TQ3EllipsoidData;


// General polygon contour
typedef struct TQ3GeneralPolygonContourData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 *vertices;
} TQ3GeneralPolygonContourData;


// General polygon data
typedef struct TQ3GeneralPolygonData {
    TQ3Uns32                                    numContours;
    TQ3GeneralPolygonContourData                *contours;
    TQ3GeneralPolygonShapeHint                  shapeHint;
    TQ3AttributeSet                             generalPolygonAttributeSet;
} TQ3GeneralPolygonData;


// Line data
typedef struct TQ3LineData {
    TQ3Vertex3D                                 vertices[2];
    TQ3AttributeSet                             lineAttributeSet;
} TQ3LineData;


// Marker data
typedef struct TQ3MarkerData {
    TQ3Point3D                                  location;
    TQ3Int32                                    xOffset;
    TQ3Int32                                    yOffset;
    TQ3Bitmap                                   bitmap;
    TQ3AttributeSet                             markerAttributeSet;
} TQ3MarkerData;


// Mesh data (all opaque)
typedef struct OpaqueTQ3MeshComponent           *TQ3MeshComponent;
typedef struct OpaqueTQ3MeshVertex              *TQ3MeshVertex;
typedef struct OpaqueTQ3MeshFace                *TQ3MeshFace;
typedef struct OpaqueTQ3MeshEdge                *TQ3MeshEdge;
typedef struct OpaqueTQ3MeshContour             *TQ3MeshContour;

typedef struct TQ3MeshIterator {
    void                                        *var1;
    void                                        *var2;
    void                                        *var3;
    struct {
        void                                    *field1;
        char                                    field2[4];
    } var4;
} TQ3MeshIterator;


// NURB curve data
typedef struct TQ3NURBCurveData {
    TQ3Uns32                                    order;
    TQ3Uns32                                    numPoints;
    TQ3RationalPoint4D                          *controlPoints;
    float                                       *knots;
    TQ3AttributeSet                             curveAttributeSet;
} TQ3NURBCurveData;


// NURB patch data
typedef struct TQ3NURBPatchTrimCurveData {
    TQ3Uns32                                    order;
    TQ3Uns32                                    numPoints;
    TQ3RationalPoint3D                          *controlPoints;
    float                                       *knots;
} TQ3NURBPatchTrimCurveData;

typedef struct TQ3NURBPatchTrimLoopData {
    TQ3Uns32                                    numTrimCurves;
    TQ3NURBPatchTrimCurveData                   *trimCurves;
} TQ3NURBPatchTrimLoopData;

typedef struct TQ3NURBPatchData {
    TQ3Uns32                                    uOrder;
    TQ3Uns32                                    vOrder;
    TQ3Uns32                                    numRows;
    TQ3Uns32                                    numColumns;
    TQ3RationalPoint4D                          *controlPoints;
    float                                       *uKnots;
    float                                       *vKnots;
    TQ3Uns32                                    numTrimLoops;
    TQ3NURBPatchTrimLoopData                    *trimLoops;
    TQ3AttributeSet                             patchAttributeSet;
} TQ3NURBPatchData;


// Pixmap marker data
typedef struct TQ3PixmapMarkerData {
    TQ3Point3D                                  position;
    TQ3Int32                                    xOffset;
    TQ3Int32                                    yOffset;
    TQ3StoragePixmap                            pixmap;
    TQ3AttributeSet                             pixmapMarkerAttributeSet;
} TQ3PixmapMarkerData;


// Point data
typedef struct TQ3PointData {
    TQ3Point3D                                  point;
    TQ3AttributeSet                             pointAttributeSet;
} TQ3PointData;


// Polygon data
typedef struct TQ3PolygonData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 *vertices;
    TQ3AttributeSet                             polygonAttributeSet;
} TQ3PolygonData;


// Polyhedron data
typedef TQ3Uns32                                TQ3PolyhedronEdge;

typedef struct TQ3PolyhedronEdgeData {
    TQ3Uns32                                    vertexIndices[2];
    TQ3Uns32                                    triangleIndices[2];
    TQ3AttributeSet                             edgeAttributeSet;
} TQ3PolyhedronEdgeData;

typedef struct TQ3PolyhedronTriangleData {
    TQ3Uns32                                    vertexIndices[3];
    TQ3PolyhedronEdge                           edgeFlag;
    TQ3AttributeSet                             triangleAttributeSet;
} TQ3PolyhedronTriangleData;

typedef struct TQ3PolyhedronData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 *vertices;
    TQ3Uns32                                    numEdges;
    TQ3PolyhedronEdgeData                       *edges;
    TQ3Uns32                                    numTriangles;
    TQ3PolyhedronTriangleData                   *triangles;
    TQ3AttributeSet                             polyhedronAttributeSet;
} TQ3PolyhedronData;


// Polyline data
typedef struct TQ3PolyLineData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 *vertices;
    TQ3AttributeSet                             *segmentAttributeSet;
    TQ3AttributeSet                             polyLineAttributeSet;
} TQ3PolyLineData;


// Torus data
typedef struct TQ3TorusData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       ratio;
    float                                       uMin;
    float                                       uMax;
    float                                       vMin;
    float                                       vMax;
    TQ3EndCap                                   caps;
    TQ3AttributeSet                             interiorAttributeSet;
    TQ3AttributeSet                             torusAttributeSet;
} TQ3TorusData;


// Triangle data
typedef struct TQ3TriangleData {
    TQ3Vertex3D                                 vertices[3];
    TQ3AttributeSet                             triangleAttributeSet;
} TQ3TriangleData;


// TriGrid data
typedef struct TQ3TriGridData {
    TQ3Uns32                                    numRows;
    TQ3Uns32                                    numColumns;
    TQ3Vertex3D                                 *vertices;
    TQ3AttributeSet                             *facetAttributeSet;
    TQ3AttributeSet                             triGridAttributeSet;
} TQ3TriGridData;


// TriMesh data
typedef struct TQ3TriMeshTriangleData {
    TQ3Uns32                                    pointIndices[3];
} TQ3TriMeshTriangleData;

typedef struct TQ3TriMeshEdgeData {
    TQ3Uns32                                    pointIndices[2];
    TQ3Uns32                                    triangleIndices[2];
} TQ3TriMeshEdgeData;

typedef struct TQ3TriMeshAttributeData {
    TQ3AttributeType                            attributeType;
    void                                        *data;
    char                                        *attributeUseArray;
} TQ3TriMeshAttributeData;

typedef struct TQ3TriMeshData {
    TQ3AttributeSet                             triMeshAttributeSet;

    TQ3Uns32                                    numTriangles;
    TQ3TriMeshTriangleData                      *triangles;

    TQ3Uns32                                    numTriangleAttributeTypes;
    TQ3TriMeshAttributeData                     *triangleAttributeTypes;

    TQ3Uns32                                    numEdges;
    TQ3TriMeshEdgeData                          *edges;

    TQ3Uns32                                    numEdgeAttributeTypes;
    TQ3TriMeshAttributeData                     *edgeAttributeTypes;

    TQ3Uns32                                    numPoints;
    TQ3Point3D                                  *points;

    TQ3Uns32                                    numVertexAttributeTypes;
    TQ3TriMeshAttributeData                     *vertexAttributeTypes;

    TQ3BoundingBox                              bBox;
} TQ3TriMeshData;





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Mesh iterators
#define Q3ForEachMeshComponent(_m, _c, _i)                  \
        for ((_c) = Q3Mesh_FirstMeshComponent((_m), (_i));  \
             (_c) != NULL;                                  \
             (_c) = Q3Mesh_NextMeshComponent((_i)))

#define Q3ForEachComponentVertex(_c, _v, _i)                \
        for ((_v) = Q3Mesh_FirstComponentVertex((_c), (_i));\
             (_v) != NULL;                                  \
             (_v) = Q3Mesh_NextComponentVertex((_i)))

#define Q3ForEachComponentEdge(_c, _e, _i)                  \
        for ((_e) = Q3Mesh_FirstComponentEdge((_c), (_i));  \
             (_e) != NULL;                                  \
             (_e) = Q3Mesh_NextComponentEdge((_i)))

#define Q3ForEachMeshVertex(_m, _v, _i)                     \
        for ((_v) = Q3Mesh_FirstMeshVertex((_m), (_i));     \
             (_v) != NULL;                                  \
             (_v) = Q3Mesh_NextMeshVertex((_i)))

#define Q3ForEachMeshFace(_m, _f, _i)                       \
        for ((_f) = Q3Mesh_FirstMeshFace((_m), (_i));       \
             (_f) != NULL;                                  \
             (_f) = Q3Mesh_NextMeshFace((_i)))

#define Q3ForEachMeshEdge(_m, _e, _i)                       \
        for ((_e) = Q3Mesh_FirstMeshEdge((_m), (_i));       \
             (_e) != NULL;                                  \
             (_e) = Q3Mesh_NextMeshEdge((_i)))

#define Q3ForEachVertexEdge(_v, _e, _i)                     \
        for ((_e) = Q3Mesh_FirstVertexEdge((_v), (_i));     \
             (_e) != NULL;                                  \
             (_e) = Q3Mesh_NextVertexEdge((_i)))


#define Q3ForEachVertexVertex(_v, _n, _i)                   \
        for ((_n) = Q3Mesh_FirstVertexVertex((_v), (_i));   \
             (_n) != NULL;                                  \
             (_n) = Q3Mesh_NextVertexVertex((_i)))

#define Q3ForEachVertexFace(_v, _f, _i)                     \
        for ((_f) = Q3Mesh_FirstVertexFace((_v), (_i));     \
             (_f) != NULL;                                  \
             (_f) = Q3Mesh_NextVertexFace((_i)))

#define Q3ForEachFaceEdge(_f, _e, _i)                       \
        for ((_e) = Q3Mesh_FirstFaceEdge((_f), (_i));       \
             (_e) != NULL;                                  \
             (_e) = Q3Mesh_NextFaceEdge((_i)))

#define Q3ForEachFaceVertex(_f, _v, _i)                     \
        for ((_v) = Q3Mesh_FirstFaceVertex((_f), (_i));     \
             (_v) != NULL;                                  \
             (_v) = Q3Mesh_NextFaceVertex((_i)))
    
#define Q3ForEachFaceFace(_f, _n, _i)                       \
        for ((_n) = Q3Mesh_FirstFaceFace((_f), (_i));       \
             (_n) != NULL;                                  \
             (_n) = Q3Mesh_NextFaceFace((_i)))
          
#define Q3ForEachFaceContour(_f, _h, _i)                    \
        for ((_h) = Q3Mesh_FirstFaceContour((_f), (_i));    \
             (_h) != NULL;                                  \
             (_h) = Q3Mesh_NextFaceContour((_i)))

#define Q3ForEachContourEdge(_h, _e, _i)                    \
        for ((_e) = Q3Mesh_FirstContourEdge((_h), (_i));    \
             (_e) != NULL;                                  \
             (_e) = Q3Mesh_NextContourEdge((_i)))

#define Q3ForEachContourVertex(_h, _v, _i)                  \
        for ((_v) = Q3Mesh_FirstContourVertex((_h), (_i));  \
             (_v) != NULL;                                  \
             (_v) = Q3Mesh_NextContourVertex((_i)))

#define Q3ForEachContourFace(_h, _f, _i)                    \
        for ((_f) = Q3Mesh_FirstContourFace((_h), (_i));    \
             (_f) != NULL;                                  \
             (_f) = Q3Mesh_NextContourFace((_i)))





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Geometry_GetType
 *  @discussion
 *      Returns the type of the GeometryObject passed in.
 *
 *      This function is used to determine the type of geometry object the parameter
 *      is a reference to. If 'geometry' is invalid, kQ3ObjectTypeInvalid will be
 *		returned.
 *
 *  @param geometry         A reference to a geometry object.
 *  @result                 A constant enumerating the type of 'geometry'.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Geometry_GetType (
    TQ3GeometryObject             geometry
);



/*!
 *  @function
 *      Q3Geometry_GetAttributeSet
 *  @discussion
 *      Returns the attribute set associated with a geometric object.
 *
 *      This function fills the TQ3AttributeSet* parameter out with the
 *		attribute set of the passed in TQ3GeometryObject reference.
 *
 *		The reference count of the attribute set is incremented.
 *
 *		To decrease the reference count when finished with the attribute set, use
 *		Q3ObjectDispose().
 *
 *  @param geometry         A reference to a geometry object.
 *  @param attributeSet     A pointer to an attribute set to be filled out.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Geometry_GetAttributeSet (
    TQ3GeometryObject             geometry,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3Geometry_SetAttributeSet
 *  @discussion
 *      Sets the attribute set associated with a geometric object.
 *
 *      Applies the attribute set given by the TQ3AttributeSet parameter onto
 *		the TQ3GeometryObject reference passed in.
 *
 *  @param geometry         A reference to a geometry object.
 *  @param attributeSet     The attribute set to apply to the object.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Geometry_SetAttributeSet (
    TQ3GeometryObject             geometry,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3Geometry_Submit
 *  @discussion
 *      Submits a geometry object for drawing, picking, bounding, or writing.
 *
 *      Submits the passed in TQ3GeometryObject to the supplied view for
 *      drawing, picking, bounding, or writing.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param geometry         A reference to a (retained) geometry object.
 *  @param view             The view to submit the geometric object to.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Geometry_Submit (
    TQ3GeometryObject             geometry,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Box_New
 *  @discussion
 *      Constructs a new Box geometry object.
 *
 *  @param boxData          Instance data passed in specifying the parameters of the new box object.
 *  @result                 A reference to the new geometry object.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Box_New (
    const TQ3BoxData              *boxData
);



/*!
 *  @function
 *      Q3Box_Submit
 *  @discussion
 *		Submits a box for drawing, picking, bounding, or writing.
 *
 *		Submits the box described by the 'boxData' parameter to the
 *		supplied view for drawing, picking, bounding, or writing.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param boxData			A pointer to a TQ3BoxData structure specifying the box to be drawn.
 *  @param view				The view to submit the box to.
 *  @result					The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_Submit (
    const TQ3BoxData              *boxData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Box_SetData
 *  @discussion
 *      Sets the properties of an existing box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param boxData          A pointer to the box data to apply to the box object.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetData (
    TQ3GeometryObject             box,
    const TQ3BoxData              *boxData
);



/*!
 *  @function
 *      Q3Box_GetData
 *  @discussion
 *      Gets the properties of an existing box object.
 *
 *		Memory is allocated for the 'boxData' parameter, and Q3Box_EmptyData
 *		must be called to dispose of this memory.
 *
 *  @param box              A reference to a box geometry object.
 *  @param boxData          A pointer to the box data to fill with box object's description.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetData (
    TQ3GeometryObject             box,
    TQ3BoxData                    *boxData
);



/*!
 *  @function
 *      Q3Box_EmptyData
 *  @discussion
 *      Releases the memory allocated by a prior call to Q3Box_GetData.
 *
 *  @param boxData          A pointer to the box data allocated by Q3Box_GetData().
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_EmptyData (
    TQ3BoxData                    *boxData
);



/*!
 *  @function
 *      Q3Box_SetOrigin
 *  @discussion
 *      Sets the origin of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param origin           The point to which to set 'box's origin.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetOrigin (
    TQ3GeometryObject             box,
    const TQ3Point3D              *origin
);



/*!
 *  @function
 *      Q3Box_SetOrientation
 *  @discussion
 *      Sets the orientation of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param orientation      The vector to which to set 'box's orientation.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetOrientation (
    TQ3GeometryObject             box,
    const TQ3Vector3D             *orientation
);



/*!
 *  @function
 *      Q3Box_SetMajorAxis
 *  @discussion
 *      Sets the major axis of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param majorAxis        The vector to which to set 'box's major axis.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetMajorAxis (
    TQ3GeometryObject             box,
    const TQ3Vector3D             *majorAxis
);



/*!
 *  @function
 *      Q3Box_SetMinorAxis
 *  @discussion
 *      Sets the minor axis of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param minorAxis        The vector to which to set 'box's minor axis.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetMinorAxis (
    TQ3GeometryObject             box,
    const TQ3Vector3D             *minorAxis
);



/*!
 *  @function
 *      Q3Box_GetOrigin
 *  @discussion
 *      Gets the origin of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param origin           The point to be filled out with 'box's origin.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetOrigin (
    TQ3GeometryObject             box,
    TQ3Point3D                    *origin
);



/*!
 *  @function
 *      Q3Box_GetOrientation
 *  @discussion
 *      Gets the orientation of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param orientation      The vector to be filled with 'box's orientation.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetOrientation (
    TQ3GeometryObject             box,
    TQ3Vector3D                   *orientation
);



/*!
 *  @function
 *      Q3Box_GetMajorAxis
 *  @discussion
 *      Gets the major axis of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param majorAxis        The vector to be filled with 'box's major axis.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetMajorAxis (
    TQ3GeometryObject             box,
    TQ3Vector3D                   *majorAxis
);



/*!
 *  @function
 *      Q3Box_GetMinorAxis
 *  @discussion
 *      Gets the minor axis of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param minorAxis        The vector to be filled with 'box's minor axis.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetMinorAxis (
    TQ3GeometryObject             box,
    TQ3Vector3D                   *minorAxis
);



/*!
 *  @function
 *      Q3Box_GetFaceAttributeSet
 *  @discussion
 *      Gets the attribute set associated with a certain face of a box object.
 *
 *      The TQ3AttributeSet* parameter is filled with the attribute set
 *      of the face indexed with 'faceIndex' of the box geometry object.
 *		faceIndex must be between 0 and 5 (inclusive).
 *
 *		The reference count of the returned TQ3AttributeSet is incremented.
 *
 *  @param box              A reference to a box geometry object.
 *  @param faceIndex        The index of the face of the box.
 *  @param faceAttributeSet A pointer to an attribute set to be filled out.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_GetFaceAttributeSet (
    TQ3GeometryObject             box,
    TQ3Uns32                      faceIndex,
    TQ3AttributeSet               *faceAttributeSet
);



/*!
 *  @function
 *      Q3Box_SetFaceAttributeSet
 *  @discussion
 *      Sets the attribute set associated with a certain face of a box object.
 *
 *      This function applies the TQ3AttributeSet* parameter to the attribute set
 *      of the face indexed with 'faceIndex' of the box geometry object.
 *		faceIndex must be between 0 and 5 (inclusive).
 *
 *  @param box              A reference to a box geometry object.
 *  @param faceIndex        The index of the face of the box.
 *  @param faceAttributeSet A pointer to an attribute set to be applied.
 *  @result                 The error status of the function.
 */
EXTERN_API_C ( TQ3Status  )
Q3Box_SetFaceAttributeSet (
    TQ3GeometryObject             box,
    TQ3Uns32                      faceIndex,
    TQ3AttributeSet               faceAttributeSet
);



/*!
 *  @function
 *      Q3Cone_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param coneData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Cone_New (
    const TQ3ConeData             *coneData
);



/*!
 *  @function
 *      Q3Cone_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param coneData         Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_Submit (
    const TQ3ConeData             *coneData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Cone_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param coneData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetData (
    TQ3GeometryObject             cone,
    const TQ3ConeData             *coneData
);



/*!
 *  @function
 *      Q3Cone_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param coneData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetData (
    TQ3GeometryObject             cone,
    TQ3ConeData                   *coneData
);



/*!
 *  @function
 *      Q3Cone_SetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetOrigin (
    TQ3GeometryObject             cone,
    const TQ3Point3D              *origin
);



/*!
 *  @function
 *      Q3Cone_SetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetOrientation (
    TQ3GeometryObject             cone,
    const TQ3Vector3D             *orientation
);



/*!
 *  @function
 *      Q3Cone_SetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetMajorRadius (
    TQ3GeometryObject             cone,
    const TQ3Vector3D             *majorRadius
);



/*!
 *  @function
 *      Q3Cone_SetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetMinorRadius (
    TQ3GeometryObject             cone,
    const TQ3Vector3D             *minorRadius
);



/*!
 *  @function
 *      Q3Cone_GetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetOrigin (
    TQ3GeometryObject             cone,
    TQ3Point3D                    *origin
);



/*!
 *  @function
 *      Q3Cone_GetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetOrientation (
    TQ3GeometryObject             cone,
    TQ3Vector3D                   *orientation
);



/*!
 *  @function
 *      Q3Cone_GetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetMajorRadius (
    TQ3GeometryObject             cone,
    TQ3Vector3D                   *majorRadius
);



/*!
 *  @function
 *      Q3Cone_GetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetMinorRadius (
    TQ3GeometryObject             cone,
    TQ3Vector3D                   *minorRadius
);



/*!
 *  @function
 *      Q3Cone_SetCaps
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param caps             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetCaps (
    TQ3GeometryObject             cone,
    TQ3EndCap                     caps
);



/*!
 *  @function
 *      Q3Cone_GetCaps
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param caps             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetCaps (
    TQ3GeometryObject             cone,
    TQ3EndCap                     *caps
);



/*!
 *  @function
 *      Q3Cone_SetBottomAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param bottomAttributeSet Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetBottomAttributeSet (
    TQ3GeometryObject             cone,
    TQ3AttributeSet               bottomAttributeSet
);



/*!
 *  @function
 *      Q3Cone_GetBottomAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param bottomAttributeSet Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetBottomAttributeSet (
    TQ3GeometryObject             cone,
    TQ3AttributeSet               *bottomAttributeSet
);



/*!
 *  @function
 *      Q3Cone_SetFaceAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param faceAttributeSet Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_SetFaceAttributeSet (
    TQ3GeometryObject             cone,
    TQ3AttributeSet               faceAttributeSet
);



/*!
 *  @function
 *      Q3Cone_GetFaceAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cone             Description of the parameter.
 *  @param faceAttributeSet Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_GetFaceAttributeSet (
    TQ3GeometryObject             cone,
    TQ3AttributeSet               *faceAttributeSet
);



/*!
 *  @function
 *      Q3Cone_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param coneData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_EmptyData (
    TQ3ConeData                   *coneData
);



/*!
 *  @function
 *      Q3Cylinder_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinderData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Cylinder_New (
    const TQ3CylinderData         *cylinderData
);



/*!
 *  @function
 *      Q3Cylinder_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinderData     Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_Submit (
    const TQ3CylinderData         *cylinderData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Cylinder_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param cylinderData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetData (
    TQ3GeometryObject             cylinder,
    const TQ3CylinderData         *cylinderData
);



/*!
 *  @function
 *      Q3Cylinder_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param cylinderData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetData (
    TQ3GeometryObject             cylinder,
    TQ3CylinderData               *cylinderData
);



/*!
 *  @function
 *      Q3Cylinder_SetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetOrigin (
    TQ3GeometryObject             cylinder,
    const TQ3Point3D              *origin
);



/*!
 *  @function
 *      Q3Cylinder_SetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetOrientation (
    TQ3GeometryObject             cylinder,
    const TQ3Vector3D             *orientation
);



/*!
 *  @function
 *      Q3Cylinder_SetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetMajorRadius (
    TQ3GeometryObject             cylinder,
    const TQ3Vector3D             *majorRadius
);



/*!
 *  @function
 *      Q3Cylinder_SetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetMinorRadius (
    TQ3GeometryObject             cylinder,
    const TQ3Vector3D             *minorRadius
);



/*!
 *  @function
 *      Q3Cylinder_GetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetOrigin (
    TQ3GeometryObject             cylinder,
    TQ3Point3D                    *origin
);



/*!
 *  @function
 *      Q3Cylinder_GetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetOrientation (
    TQ3GeometryObject             cylinder,
    TQ3Vector3D                   *orientation
);



/*!
 *  @function
 *      Q3Cylinder_GetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetMajorRadius (
    TQ3GeometryObject             cylinder,
    TQ3Vector3D                   *majorRadius
);



/*!
 *  @function
 *      Q3Cylinder_GetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetMinorRadius (
    TQ3GeometryObject             cylinder,
    TQ3Vector3D                   *minorRadius
);



/*!
 *  @function
 *      Q3Cylinder_SetCaps
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param caps             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetCaps (
    TQ3GeometryObject             cylinder,
    TQ3EndCap                     caps
);



/*!
 *  @function
 *      Q3Cylinder_GetCaps
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param caps             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetCaps (
    TQ3GeometryObject             cylinder,
    TQ3EndCap                     *caps
);



/*!
 *  @function
 *      Q3Cylinder_SetTopAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param topAttributeSet  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetTopAttributeSet (
    TQ3GeometryObject             cylinder,
    TQ3AttributeSet               topAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_GetTopAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param topAttributeSet  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetTopAttributeSet (
    TQ3GeometryObject             cylinder,
    TQ3AttributeSet               *topAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_SetBottomAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param bottomAttributeSet Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetBottomAttributeSet (
    TQ3GeometryObject             cylinder,
    TQ3AttributeSet               bottomAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_GetBottomAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param bottomAttributeSet Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetBottomAttributeSet (
    TQ3GeometryObject             cylinder,
    TQ3AttributeSet               *bottomAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_SetFaceAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param faceAttributeSet Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetFaceAttributeSet (
    TQ3GeometryObject             cylinder,
    TQ3AttributeSet               faceAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_GetFaceAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinder         Description of the parameter.
 *  @param faceAttributeSet Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetFaceAttributeSet (
    TQ3GeometryObject             cylinder,
    TQ3AttributeSet               *faceAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param cylinderData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_EmptyData (
    TQ3CylinderData               *cylinderData
);



/*!
 *  @function
 *      Q3Disk_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param diskData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Disk_New (
    const TQ3DiskData             *diskData
);



/*!
 *  @function
 *      Q3Disk_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param diskData         Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_Submit (
    const TQ3DiskData             *diskData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Disk_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param disk             Description of the parameter.
 *  @param diskData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_SetData (
    TQ3GeometryObject             disk,
    const TQ3DiskData             *diskData
);



/*!
 *  @function
 *      Q3Disk_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param disk             Description of the parameter.
 *  @param diskData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_GetData (
    TQ3GeometryObject             disk,
    TQ3DiskData                   *diskData
);



/*!
 *  @function
 *      Q3Disk_SetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param disk             Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_SetOrigin (
    TQ3GeometryObject             disk,
    const TQ3Point3D              *origin
);



/*!
 *  @function
 *      Q3Disk_SetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param disk             Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_SetMajorRadius (
    TQ3GeometryObject             disk,
    const TQ3Vector3D             *majorRadius
);



/*!
 *  @function
 *      Q3Disk_SetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param disk             Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_SetMinorRadius (
    TQ3GeometryObject             disk,
    const TQ3Vector3D             *minorRadius
);



/*!
 *  @function
 *      Q3Disk_GetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param disk             Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_GetOrigin (
    TQ3GeometryObject             disk,
    TQ3Point3D                    *origin
);



/*!
 *  @function
 *      Q3Disk_GetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param disk             Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_GetMajorRadius (
    TQ3GeometryObject             disk,
    TQ3Vector3D                   *majorRadius
);



/*!
 *  @function
 *      Q3Disk_GetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param disk             Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_GetMinorRadius (
    TQ3GeometryObject             disk,
    TQ3Vector3D                   *minorRadius
);



/*!
 *  @function
 *      Q3Disk_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param diskData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_EmptyData (
    TQ3DiskData                   *diskData
);



/*!
 *  @function
 *      Q3Ellipse_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipseData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Ellipse_New (
    const TQ3EllipseData          *ellipseData
);



/*!
 *  @function
 *      Q3Ellipse_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipseData      Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_Submit (
    const TQ3EllipseData          *ellipseData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Ellipse_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipse          Description of the parameter.
 *  @param ellipseData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetData (
    TQ3GeometryObject             ellipse,
    const TQ3EllipseData          *ellipseData
);



/*!
 *  @function
 *      Q3Ellipse_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipse          Description of the parameter.
 *  @param ellipseData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetData (
    TQ3GeometryObject             ellipse,
    TQ3EllipseData                *ellipseData
);



/*!
 *  @function
 *      Q3Ellipse_SetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipse          Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetOrigin (
    TQ3GeometryObject             ellipse,
    const TQ3Point3D              *origin
);



/*!
 *  @function
 *      Q3Ellipse_SetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipse          Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetMajorRadius (
    TQ3GeometryObject             ellipse,
    const TQ3Vector3D             *majorRadius
);



/*!
 *  @function
 *      Q3Ellipse_SetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipse          Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetMinorRadius (
    TQ3GeometryObject             ellipse,
    const TQ3Vector3D             *minorRadius
);



/*!
 *  @function
 *      Q3Ellipse_GetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipse          Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetOrigin (
    TQ3GeometryObject             ellipse,
    TQ3Point3D                    *origin
);



/*!
 *  @function
 *      Q3Ellipse_GetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipse          Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetMajorRadius (
    TQ3GeometryObject             ellipse,
    TQ3Vector3D                   *majorRadius
);



/*!
 *  @function
 *      Q3Ellipse_GetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipse          Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetMinorRadius (
    TQ3GeometryObject             ellipse,
    TQ3Vector3D                   *minorRadius
);



/*!
 *  @function
 *      Q3Ellipse_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipseData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_EmptyData (
    TQ3EllipseData                *ellipseData
);



/*!
 *  @function
 *      Q3Ellipsoid_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoidData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Ellipsoid_New (
    const TQ3EllipsoidData        *ellipsoidData
);



/*!
 *  @function
 *      Q3Ellipsoid_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoidData    Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_Submit (
    const TQ3EllipsoidData        *ellipsoidData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Ellipsoid_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoid        Description of the parameter.
 *  @param ellipsoidData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetData (
    TQ3GeometryObject             ellipsoid,
    const TQ3EllipsoidData        *ellipsoidData
);



/*!
 *  @function
 *      Q3Ellipsoid_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoid        Description of the parameter.
 *  @param ellipsoidData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetData (
    TQ3GeometryObject             ellipsoid,
    TQ3EllipsoidData              *ellipsoidData
);



/*!
 *  @function
 *      Q3Ellipsoid_SetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoid        Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetOrigin (
    TQ3GeometryObject             ellipsoid,
    const TQ3Point3D              *origin
);



/*!
 *  @function
 *      Q3Ellipsoid_SetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoid        Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetOrientation (
    TQ3GeometryObject             ellipsoid,
    const TQ3Vector3D             *orientation
);



/*!
 *  @function
 *      Q3Ellipsoid_SetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoid        Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetMajorRadius (
    TQ3GeometryObject             ellipsoid,
    const TQ3Vector3D             *majorRadius
);



/*!
 *  @function
 *      Q3Ellipsoid_SetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoid        Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetMinorRadius (
    TQ3GeometryObject             ellipsoid,
    const TQ3Vector3D             *minorRadius
);



/*!
 *  @function
 *      Q3Ellipsoid_GetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoid        Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetOrigin (
    TQ3GeometryObject             ellipsoid,
    TQ3Point3D                    *origin
);



/*!
 *  @function
 *      Q3Ellipsoid_GetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoid        Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetOrientation (
    TQ3GeometryObject             ellipsoid,
    TQ3Vector3D                   *orientation
);



/*!
 *  @function
 *      Q3Ellipsoid_GetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoid        Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetMajorRadius (
    TQ3GeometryObject             ellipsoid,
    TQ3Vector3D                   *majorRadius
);



/*!
 *  @function
 *      Q3Ellipsoid_GetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoid        Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetMinorRadius (
    TQ3GeometryObject             ellipsoid,
    TQ3Vector3D                   *minorRadius
);



/*!
 *  @function
 *      Q3Ellipsoid_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param ellipsoidData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_EmptyData (
    TQ3EllipsoidData              *ellipsoidData
);



/*!
 *  @function
 *      Q3GeneralPolygon_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param generalPolygonData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3GeneralPolygon_New (
    const TQ3GeneralPolygonData   *generalPolygonData
);



/*!
 *  @function
 *      Q3GeneralPolygon_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param generalPolygonData Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_Submit (
    const TQ3GeneralPolygonData   *generalPolygonData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3GeneralPolygon_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param generalPolygon   Description of the parameter.
 *  @param generalPolygonData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetData (
    TQ3GeometryObject             generalPolygon,
    const TQ3GeneralPolygonData   *generalPolygonData
);



/*!
 *  @function
 *      Q3GeneralPolygon_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polygon          Description of the parameter.
 *  @param generalPolygonData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetData (
    TQ3GeometryObject             polygon,
    TQ3GeneralPolygonData         *generalPolygonData
);



/*!
 *  @function
 *      Q3GeneralPolygon_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param generalPolygonData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_EmptyData (
    TQ3GeneralPolygonData         *generalPolygonData
);



/*!
 *  @function
 *      Q3GeneralPolygon_GetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param generalPolygon   Description of the parameter.
 *  @param contourIndex     Description of the parameter.
 *  @param pointIndex       Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetVertexPosition (
    TQ3GeometryObject             generalPolygon,
    TQ3Uns32                      contourIndex,
    TQ3Uns32                      pointIndex,
    TQ3Point3D                    *position
);



/*!
 *  @function
 *      Q3GeneralPolygon_SetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param generalPolygon   Description of the parameter.
 *  @param contourIndex     Description of the parameter.
 *  @param pointIndex       Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetVertexPosition (
    TQ3GeometryObject             generalPolygon,
    TQ3Uns32                      contourIndex,
    TQ3Uns32                      pointIndex,
    const TQ3Point3D              *position
);



/*!
 *  @function
 *      Q3GeneralPolygon_GetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param generalPolygon   Description of the parameter.
 *  @param contourIndex     Description of the parameter.
 *  @param pointIndex       Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetVertexAttributeSet (
    TQ3GeometryObject             generalPolygon,
    TQ3Uns32                      contourIndex,
    TQ3Uns32                      pointIndex,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3GeneralPolygon_SetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param generalPolygon   Description of the parameter.
 *  @param contourIndex     Description of the parameter.
 *  @param pointIndex       Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetVertexAttributeSet (
    TQ3GeometryObject             generalPolygon,
    TQ3Uns32                      contourIndex,
    TQ3Uns32                      pointIndex,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3GeneralPolygon_SetShapeHint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param generalPolygon   Description of the parameter.
 *  @param shapeHint        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetShapeHint (
    TQ3GeometryObject             generalPolygon,
    TQ3GeneralPolygonShapeHint    shapeHint
);



/*!
 *  @function
 *      Q3GeneralPolygon_GetShapeHint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param generalPolygon   Description of the parameter.
 *  @param shapeHint        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetShapeHint (
    TQ3GeometryObject             generalPolygon,
    TQ3GeneralPolygonShapeHint    *shapeHint
);



/*!
 *  @function
 *      Q3Line_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param lineData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Line_New (
    const TQ3LineData             *lineData
);



/*!
 *  @function
 *      Q3Line_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param lineData         Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_Submit (
    const TQ3LineData             *lineData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Line_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param line             Description of the parameter.
 *  @param lineData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_GetData (
    TQ3GeometryObject             line,
    TQ3LineData                   *lineData
);



/*!
 *  @function
 *      Q3Line_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param line             Description of the parameter.
 *  @param lineData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_SetData (
    TQ3GeometryObject             line,
    const TQ3LineData             *lineData
);



/*!
 *  @function
 *      Q3Line_GetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param line             Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_GetVertexPosition (
    TQ3GeometryObject             line,
    TQ3Uns32                      index,
    TQ3Point3D                    *position
);



/*!
 *  @function
 *      Q3Line_SetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param line             Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_SetVertexPosition (
    TQ3GeometryObject             line,
    TQ3Uns32                      index,
    const TQ3Point3D              *position
);



/*!
 *  @function
 *      Q3Line_GetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param line             Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_GetVertexAttributeSet (
    TQ3GeometryObject             line,
    TQ3Uns32                      index,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3Line_SetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param line             Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_SetVertexAttributeSet (
    TQ3GeometryObject             line,
    TQ3Uns32                      index,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3Line_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param lineData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_EmptyData (
    TQ3LineData                   *lineData
);



/*!
 *  @function
 *      Q3Marker_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param markerData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Marker_New (
    const TQ3MarkerData           *markerData
);



/*!
 *  @function
 *      Q3Marker_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param markerData       Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_Submit (
    const TQ3MarkerData           *markerData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Marker_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param geometry         Description of the parameter.
 *  @param markerData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_SetData (
    TQ3GeometryObject             geometry,
    const TQ3MarkerData           *markerData
);



/*!
 *  @function
 *      Q3Marker_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param geometry         Description of the parameter.
 *  @param markerData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_GetData (
    TQ3GeometryObject             geometry,
    TQ3MarkerData                 *markerData
);



/*!
 *  @function
 *      Q3Marker_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param markerData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_EmptyData (
    TQ3MarkerData                 *markerData
);



/*!
 *  @function
 *      Q3Marker_GetPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param marker           Description of the parameter.
 *  @param location         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_GetPosition (
    TQ3GeometryObject             marker,
    TQ3Point3D                    *location
);



/*!
 *  @function
 *      Q3Marker_SetPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param marker           Description of the parameter.
 *  @param location         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_SetPosition (
    TQ3GeometryObject             marker,
    const TQ3Point3D              *location
);



/*!
 *  @function
 *      Q3Marker_GetXOffset
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param marker           Description of the parameter.
 *  @param xOffset          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_GetXOffset (
    TQ3GeometryObject             marker,
    TQ3Int32                      *xOffset
);



/*!
 *  @function
 *      Q3Marker_SetXOffset
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param marker           Description of the parameter.
 *  @param xOffset          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_SetXOffset (
    TQ3GeometryObject             marker,
    TQ3Int32                      xOffset
);



/*!
 *  @function
 *      Q3Marker_GetYOffset
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param marker           Description of the parameter.
 *  @param yOffset          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_GetYOffset (
    TQ3GeometryObject             marker,
    TQ3Int32                      *yOffset
);



/*!
 *  @function
 *      Q3Marker_SetYOffset
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param marker           Description of the parameter.
 *  @param yOffset          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_SetYOffset (
    TQ3GeometryObject             marker,
    TQ3Int32                      yOffset
);



/*!
 *  @function
 *      Q3Marker_GetBitmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param marker           Description of the parameter.
 *  @param bitmap           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_GetBitmap (
    TQ3GeometryObject             marker,
    TQ3Bitmap                     *bitmap
);



/*!
 *  @function
 *      Q3Marker_SetBitmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param marker           Description of the parameter.
 *  @param bitmap           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_SetBitmap (
    TQ3GeometryObject             marker,
    const TQ3Bitmap               *bitmap
);



/*!
 *  @function
 *      Q3Mesh_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Mesh_New (
    void
);



/*!
 *  @function
 *      Q3Mesh_VertexNew
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_VertexNew (
    TQ3GeometryObject             mesh,
    const TQ3Vertex3D             *vertex
);



/*!
 *  @function
 *      Q3Mesh_FaceNew
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param numVertices      Description of the parameter.
 *  @param vertices         Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_FaceNew (
    TQ3GeometryObject             mesh,
    TQ3Uns32                      numVertices,
    const TQ3MeshVertex           *vertices,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3Mesh_VertexDelete
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_VertexDelete (
    TQ3GeometryObject             mesh,
    TQ3MeshVertex                 vertex
);



/*!
 *  @function
 *      Q3Mesh_FaceDelete
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param face             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_FaceDelete (
    TQ3GeometryObject             mesh,
    TQ3MeshFace                   face
);



/*!
 *  @function
 *      Q3Mesh_DelayUpdates
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_DelayUpdates (
    TQ3GeometryObject             mesh
);



/*!
 *  @function
 *      Q3Mesh_ResumeUpdates
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_ResumeUpdates (
    TQ3GeometryObject             mesh
);



/*!
 *  @function
 *      Q3Mesh_FaceToContour
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param containerFace    Description of the parameter.
 *  @param face             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshContour  )
Q3Mesh_FaceToContour (
    TQ3GeometryObject             mesh,
    TQ3MeshFace                   containerFace,
    TQ3MeshFace                   face
);



/*!
 *  @function
 *      Q3Mesh_ContourToFace
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param contour          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_ContourToFace (
    TQ3GeometryObject             mesh,
    TQ3MeshContour                contour
);



/*!
 *  @function
 *      Q3Mesh_GetNumComponents
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param numComponents    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumComponents (
    TQ3GeometryObject             mesh,
    TQ3Uns32                      *numComponents
);



/*!
 *  @function
 *      Q3Mesh_GetNumEdges
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param numEdges         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumEdges (
    TQ3GeometryObject             mesh,
    TQ3Uns32                      *numEdges
);



/*!
 *  @function
 *      Q3Mesh_GetNumVertices
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param numVertices      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumVertices (
    TQ3GeometryObject             mesh,
    TQ3Uns32                      *numVertices
);



/*!
 *  @function
 *      Q3Mesh_GetNumFaces
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param numFaces         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumFaces (
    TQ3GeometryObject             mesh,
    TQ3Uns32                      *numFaces
);



/*!
 *  @function
 *      Q3Mesh_GetNumCorners
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param numCorners       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumCorners (
    TQ3GeometryObject             mesh,
    TQ3Uns32                      *numCorners
);



/*!
 *  @function
 *      Q3Mesh_GetOrientable
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param orientable       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetOrientable (
    TQ3GeometryObject             mesh,
    TQ3Boolean                    *orientable
);



/*!
 *  @function
 *      Q3Mesh_GetComponentNumVertices
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param component        Description of the parameter.
 *  @param numVertices      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentNumVertices (
    TQ3GeometryObject             mesh,
    TQ3MeshComponent              component,
    TQ3Uns32                      *numVertices
);



/*!
 *  @function
 *      Q3Mesh_GetComponentNumEdges
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param component        Description of the parameter.
 *  @param numEdges         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentNumEdges (
    TQ3GeometryObject             mesh,
    TQ3MeshComponent              component,
    TQ3Uns32                      *numEdges
);



/*!
 *  @function
 *      Q3Mesh_GetComponentBoundingBox
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param component        Description of the parameter.
 *  @param boundingBox      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentBoundingBox (
    TQ3GeometryObject             mesh,
    TQ3MeshComponent              component,
    TQ3BoundingBox                *boundingBox
);



/*!
 *  @function
 *      Q3Mesh_GetComponentOrientable
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param component        Description of the parameter.
 *  @param orientable       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentOrientable (
    TQ3GeometryObject             mesh,
    TQ3MeshComponent              component,
    TQ3Boolean                    *orientable
);



/*!
 *  @function
 *      Q3Mesh_GetVertexCoordinates
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @param coordinates      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexCoordinates (
    TQ3GeometryObject             mesh,
    TQ3MeshVertex                 vertex,
    TQ3Point3D                    *coordinates
);



/*!
 *  @function
 *      Q3Mesh_GetVertexIndex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @param index            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexIndex (
    TQ3GeometryObject             mesh,
    TQ3MeshVertex                 vertex,
    TQ3Uns32                      *index
);



/*!
 *  @function
 *      Q3Mesh_GetVertexOnBoundary
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @param onBoundary       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexOnBoundary (
    TQ3GeometryObject             mesh,
    TQ3MeshVertex                 vertex,
    TQ3Boolean                    *onBoundary
);



/*!
 *  @function
 *      Q3Mesh_GetVertexComponent
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @param component        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexComponent (
    TQ3GeometryObject             mesh,
    TQ3MeshVertex                 vertex,
    TQ3MeshComponent              *component
);



/*!
 *  @function
 *      Q3Mesh_GetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexAttributeSet (
    TQ3GeometryObject             mesh,
    TQ3MeshVertex                 vertex,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3Mesh_SetVertexCoordinates
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @param coordinates      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetVertexCoordinates (
    TQ3GeometryObject             mesh,
    TQ3MeshVertex                 vertex,
    const TQ3Point3D              *coordinates
);



/*!
 *  @function
 *      Q3Mesh_SetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetVertexAttributeSet (
    TQ3GeometryObject             mesh,
    TQ3MeshVertex                 vertex,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3Mesh_GetFaceNumVertices
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param face             Description of the parameter.
 *  @param numVertices      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceNumVertices (
    TQ3GeometryObject             mesh,
    TQ3MeshFace                   face,
    TQ3Uns32                      *numVertices
);



/*!
 *  @function
 *      Q3Mesh_GetFacePlaneEquation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param face             Description of the parameter.
 *  @param planeEquation    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFacePlaneEquation (
    TQ3GeometryObject             mesh,
    TQ3MeshFace                   face,
    TQ3PlaneEquation              *planeEquation
);



/*!
 *  @function
 *      Q3Mesh_GetFaceNumContours
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param face             Description of the parameter.
 *  @param numContours      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceNumContours (
    TQ3GeometryObject             mesh,
    TQ3MeshFace                   face,
    TQ3Uns32                      *numContours
);



/*!
 *  @function
 *      Q3Mesh_GetFaceIndex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param face             Description of the parameter.
 *  @param index            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceIndex (
    TQ3GeometryObject             mesh,
    TQ3MeshFace                   face,
    TQ3Uns32                      *index
);



/*!
 *  @function
 *      Q3Mesh_GetFaceComponent
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param face             Description of the parameter.
 *  @param component        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceComponent (
    TQ3GeometryObject             mesh,
    TQ3MeshFace                   face,
    TQ3MeshComponent              *component
);



/*!
 *  @function
 *      Q3Mesh_GetFaceAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param face             Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceAttributeSet (
    TQ3GeometryObject             mesh,
    TQ3MeshFace                   face,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3Mesh_SetFaceAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param face             Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetFaceAttributeSet (
    TQ3GeometryObject             mesh,
    TQ3MeshFace                   face,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3Mesh_GetEdgeVertices
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param edge             Description of the parameter.
 *  @param vertex1          Description of the parameter.
 *  @param vertex2          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeVertices (
    TQ3GeometryObject             mesh,
    TQ3MeshEdge                   edge,
    TQ3MeshVertex                 *vertex1,
    TQ3MeshVertex                 *vertex2
);



/*!
 *  @function
 *      Q3Mesh_GetEdgeFaces
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param edge             Description of the parameter.
 *  @param face1            Description of the parameter.
 *  @param face2            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeFaces (
    TQ3GeometryObject             mesh,
    TQ3MeshEdge                   edge,
    TQ3MeshFace                   *face1,
    TQ3MeshFace                   *face2
);



/*!
 *  @function
 *      Q3Mesh_GetEdgeOnBoundary
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param edge             Description of the parameter.
 *  @param onBoundary       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeOnBoundary (
    TQ3GeometryObject             mesh,
    TQ3MeshEdge                   edge,
    TQ3Boolean                    *onBoundary
);



/*!
 *  @function
 *      Q3Mesh_GetEdgeComponent
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param edge             Description of the parameter.
 *  @param component        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeComponent (
    TQ3GeometryObject             mesh,
    TQ3MeshEdge                   edge,
    TQ3MeshComponent              *component
);



/*!
 *  @function
 *      Q3Mesh_GetEdgeAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param edge             Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeAttributeSet (
    TQ3GeometryObject             mesh,
    TQ3MeshEdge                   edge,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3Mesh_SetEdgeAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param edge             Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetEdgeAttributeSet (
    TQ3GeometryObject             mesh,
    TQ3MeshEdge                   edge,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3Mesh_GetContourFace
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param contour          Description of the parameter.
 *  @param face             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetContourFace (
    TQ3GeometryObject             mesh,
    TQ3MeshContour                contour,
    TQ3MeshFace                   *face
);



/*!
 *  @function
 *      Q3Mesh_GetContourNumVertices
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param contour          Description of the parameter.
 *  @param numVertices      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetContourNumVertices (
    TQ3GeometryObject             mesh,
    TQ3MeshContour                contour,
    TQ3Uns32                      *numVertices
);



/*!
 *  @function
 *      Q3Mesh_GetCornerAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @param face             Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetCornerAttributeSet (
    TQ3GeometryObject             mesh,
    TQ3MeshVertex                 vertex,
    TQ3MeshFace                   face,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3Mesh_SetCornerAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @param face             Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetCornerAttributeSet (
    TQ3GeometryObject             mesh,
    TQ3MeshVertex                 vertex,
    TQ3MeshFace                   face,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3Mesh_FirstMeshComponent
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshComponent  )
Q3Mesh_FirstMeshComponent (
    TQ3GeometryObject             mesh,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextMeshComponent
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshComponent  )
Q3Mesh_NextMeshComponent (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstComponentVertex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param component        Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_FirstComponentVertex (
    TQ3MeshComponent              component,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextComponentVertex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_NextComponentVertex (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstComponentEdge
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param component        Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_FirstComponentEdge (
    TQ3MeshComponent              component,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextComponentEdge
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_NextComponentEdge (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstMeshVertex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_FirstMeshVertex (
    TQ3GeometryObject             mesh,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextMeshVertex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_NextMeshVertex (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstMeshFace
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_FirstMeshFace (
    TQ3GeometryObject             mesh,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextMeshFace
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_NextMeshFace (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstMeshEdge
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mesh             Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_FirstMeshEdge (
    TQ3GeometryObject             mesh,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextMeshEdge
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_NextMeshEdge (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstVertexEdge
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vertex           Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_FirstVertexEdge (
    TQ3MeshVertex                 vertex,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextVertexEdge
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_NextVertexEdge (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstVertexVertex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vertex           Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_FirstVertexVertex (
    TQ3MeshVertex                 vertex,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextVertexVertex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_NextVertexVertex (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstVertexFace
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vertex           Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_FirstVertexFace (
    TQ3MeshVertex                 vertex,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextVertexFace
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_NextVertexFace (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstFaceEdge
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param face             Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_FirstFaceEdge (
    TQ3MeshFace                   face,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextFaceEdge
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_NextFaceEdge (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstFaceVertex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param face             Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_FirstFaceVertex (
    TQ3MeshFace                   face,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextFaceVertex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_NextFaceVertex (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstFaceFace
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param face             Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_FirstFaceFace (
    TQ3MeshFace                   face,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextFaceFace
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_NextFaceFace (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstFaceContour
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param face             Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshContour  )
Q3Mesh_FirstFaceContour (
    TQ3MeshFace                   face,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextFaceContour
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshContour  )
Q3Mesh_NextFaceContour (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstContourEdge
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param contour          Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_FirstContourEdge (
    TQ3MeshContour                contour,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextContourEdge
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshEdge  )
Q3Mesh_NextContourEdge (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstContourVertex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param contour          Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_FirstContourVertex (
    TQ3MeshContour                contour,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextContourVertex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshVertex  )
Q3Mesh_NextContourVertex (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstContourFace
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param contour          Description of the parameter.
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_FirstContourFace (
    TQ3MeshContour                contour,
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3Mesh_NextContourFace
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param iterator         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3MeshFace  )
Q3Mesh_NextContourFace (
    TQ3MeshIterator               *iterator
);



/*!
 *  @function
 *      Q3NURBCurve_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param curveData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3NURBCurve_New (
    const TQ3NURBCurveData        *curveData
);



/*!
 *  @function
 *      Q3NURBCurve_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param curveData        Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_Submit (
    const TQ3NURBCurveData        *curveData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3NURBCurve_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param curve            Description of the parameter.
 *  @param nurbCurveData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_SetData (
    TQ3GeometryObject             curve,
    const TQ3NURBCurveData        *nurbCurveData
);



/*!
 *  @function
 *      Q3NURBCurve_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param curve            Description of the parameter.
 *  @param nurbCurveData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_GetData (
    TQ3GeometryObject             curve,
    TQ3NURBCurveData              *nurbCurveData
);



/*!
 *  @function
 *      Q3NURBCurve_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbCurveData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_EmptyData (
    TQ3NURBCurveData              *nurbCurveData
);



/*!
 *  @function
 *      Q3NURBCurve_SetControlPoint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param curve            Description of the parameter.
 *  @param pointIndex       Description of the parameter.
 *  @param point4D          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_SetControlPoint (
    TQ3GeometryObject             curve,
    TQ3Uns32                      pointIndex,
    const TQ3RationalPoint4D      *point4D
);



/*!
 *  @function
 *      Q3NURBCurve_GetControlPoint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param curve            Description of the parameter.
 *  @param pointIndex       Description of the parameter.
 *  @param point4D          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_GetControlPoint (
    TQ3GeometryObject             curve,
    TQ3Uns32                      pointIndex,
    TQ3RationalPoint4D            *point4D
);



/*!
 *  @function
 *      Q3NURBCurve_SetKnot
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param curve            Description of the parameter.
 *  @param knotIndex        Description of the parameter.
 *  @param knotValue        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_SetKnot (
    TQ3GeometryObject             curve,
    TQ3Uns32                      knotIndex,
    float                         knotValue
);



/*!
 *  @function
 *      Q3NURBCurve_GetKnot
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param curve            Description of the parameter.
 *  @param knotIndex        Description of the parameter.
 *  @param knotValue        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_GetKnot (
    TQ3GeometryObject             curve,
    TQ3Uns32                      knotIndex,
    float                         *knotValue
);



/*!
 *  @function
 *      Q3NURBPatch_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbPatchData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3NURBPatch_New (
    const TQ3NURBPatchData        *nurbPatchData
);



/*!
 *  @function
 *      Q3NURBPatch_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbPatchData    Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_Submit (
    const TQ3NURBPatchData        *nurbPatchData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3NURBPatch_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbPatch        Description of the parameter.
 *  @param nurbPatchData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetData (
    TQ3GeometryObject             nurbPatch,
    const TQ3NURBPatchData        *nurbPatchData
);



/*!
 *  @function
 *      Q3NURBPatch_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbPatch        Description of the parameter.
 *  @param nurbPatchData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetData (
    TQ3GeometryObject             nurbPatch,
    TQ3NURBPatchData              *nurbPatchData
);



/*!
 *  @function
 *      Q3NURBPatch_SetControlPoint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbPatch        Description of the parameter.
 *  @param rowIndex         Description of the parameter.
 *  @param columnIndex      Description of the parameter.
 *  @param point4D          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetControlPoint (
    TQ3GeometryObject             nurbPatch,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    const TQ3RationalPoint4D      *point4D
);



/*!
 *  @function
 *      Q3NURBPatch_GetControlPoint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbPatch        Description of the parameter.
 *  @param rowIndex         Description of the parameter.
 *  @param columnIndex      Description of the parameter.
 *  @param point4D          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetControlPoint (
    TQ3GeometryObject             nurbPatch,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    TQ3RationalPoint4D            *point4D
);



/*!
 *  @function
 *      Q3NURBPatch_SetUKnot
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbPatch        Description of the parameter.
 *  @param knotIndex        Description of the parameter.
 *  @param knotValue        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetUKnot (
    TQ3GeometryObject             nurbPatch,
    TQ3Uns32                      knotIndex,
    float                         knotValue
);



/*!
 *  @function
 *      Q3NURBPatch_SetVKnot
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbPatch        Description of the parameter.
 *  @param knotIndex        Description of the parameter.
 *  @param knotValue        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetVKnot (
    TQ3GeometryObject             nurbPatch,
    TQ3Uns32                      knotIndex,
    float                         knotValue
);



/*!
 *  @function
 *      Q3NURBPatch_GetUKnot
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbPatch        Description of the parameter.
 *  @param knotIndex        Description of the parameter.
 *  @param knotValue        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetUKnot (
    TQ3GeometryObject             nurbPatch,
    TQ3Uns32                      knotIndex,
    float                         *knotValue
);



/*!
 *  @function
 *      Q3NURBPatch_GetVKnot
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbPatch        Description of the parameter.
 *  @param knotIndex        Description of the parameter.
 *  @param knotValue        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetVKnot (
    TQ3GeometryObject             nurbPatch,
    TQ3Uns32                      knotIndex,
    float                         *knotValue
);



/*!
 *  @function
 *      Q3NURBPatch_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param nurbPatchData    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_EmptyData (
    TQ3NURBPatchData              *nurbPatchData
);



/*!
 *  @function
 *      Q3PixmapMarker_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmapMarkerData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3PixmapMarker_New (
    const TQ3PixmapMarkerData     *pixmapMarkerData
);



/*!
 *  @function
 *      Q3PixmapMarker_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmapMarkerData Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_Submit (
    const TQ3PixmapMarkerData     *pixmapMarkerData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3PixmapMarker_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param geometry         Description of the parameter.
 *  @param pixmapMarkerData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetData (
    TQ3GeometryObject             geometry,
    const TQ3PixmapMarkerData     *pixmapMarkerData
);



/*!
 *  @function
 *      Q3PixmapMarker_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param geometry         Description of the parameter.
 *  @param pixmapMarkerData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetData (
    TQ3GeometryObject             geometry,
    TQ3PixmapMarkerData           *pixmapMarkerData
);



/*!
 *  @function
 *      Q3PixmapMarker_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmapMarkerData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_EmptyData (
    TQ3PixmapMarkerData           *pixmapMarkerData
);



/*!
 *  @function
 *      Q3PixmapMarker_GetPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmapMarker     Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetPosition (
    TQ3GeometryObject             pixmapMarker,
    TQ3Point3D                    *position
);



/*!
 *  @function
 *      Q3PixmapMarker_SetPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmapMarker     Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetPosition (
    TQ3GeometryObject             pixmapMarker,
    const TQ3Point3D              *position
);



/*!
 *  @function
 *      Q3PixmapMarker_GetXOffset
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmapMarker     Description of the parameter.
 *  @param xOffset          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetXOffset (
    TQ3GeometryObject             pixmapMarker,
    TQ3Int32                      *xOffset
);



/*!
 *  @function
 *      Q3PixmapMarker_SetXOffset
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmapMarker     Description of the parameter.
 *  @param xOffset          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetXOffset (
    TQ3GeometryObject             pixmapMarker,
    TQ3Int32                      xOffset
);



/*!
 *  @function
 *      Q3PixmapMarker_GetYOffset
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmapMarker     Description of the parameter.
 *  @param yOffset          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetYOffset (
    TQ3GeometryObject             pixmapMarker,
    TQ3Int32                      *yOffset
);



/*!
 *  @function
 *      Q3PixmapMarker_SetYOffset
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmapMarker     Description of the parameter.
 *  @param yOffset          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetYOffset (
    TQ3GeometryObject             pixmapMarker,
    TQ3Int32                      yOffset
);



/*!
 *  @function
 *      Q3PixmapMarker_GetPixmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmapMarker     Description of the parameter.
 *  @param pixmap           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetPixmap (
    TQ3GeometryObject             pixmapMarker,
    TQ3StoragePixmap              *pixmap
);



/*!
 *  @function
 *      Q3PixmapMarker_SetPixmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmapMarker     Description of the parameter.
 *  @param pixmap           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetPixmap (
    TQ3GeometryObject             pixmapMarker,
    const TQ3StoragePixmap        *pixmap
);



/*!
 *  @function
 *      Q3Point_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pointData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Point_New (
    const TQ3PointData            *pointData
);



/*!
 *  @function
 *      Q3Point_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pointData        Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_Submit (
    const TQ3PointData            *pointData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Point_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point            Description of the parameter.
 *  @param pointData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_GetData (
    TQ3GeometryObject             point,
    TQ3PointData                  *pointData
);



/*!
 *  @function
 *      Q3Point_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point            Description of the parameter.
 *  @param pointData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_SetData (
    TQ3GeometryObject             point,
    const TQ3PointData            *pointData
);



/*!
 *  @function
 *      Q3Point_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pointData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_EmptyData (
    TQ3PointData                  *pointData
);



/*!
 *  @function
 *      Q3Point_SetPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point            Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_SetPosition (
    TQ3GeometryObject             point,
    const TQ3Point3D              *position
);



/*!
 *  @function
 *      Q3Point_GetPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point            Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_GetPosition (
    TQ3GeometryObject             point,
    TQ3Point3D                    *position
);



/*!
 *  @function
 *      Q3Polygon_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polygonData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Polygon_New (
    const TQ3PolygonData          *polygonData
);



/*!
 *  @function
 *      Q3Polygon_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polygonData      Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_Submit (
    const TQ3PolygonData          *polygonData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Polygon_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polygon          Description of the parameter.
 *  @param polygonData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_SetData (
    TQ3GeometryObject             polygon,
    const TQ3PolygonData          *polygonData
);



/*!
 *  @function
 *      Q3Polygon_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polygon          Description of the parameter.
 *  @param polygonData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_GetData (
    TQ3GeometryObject             polygon,
    TQ3PolygonData                *polygonData
);



/*!
 *  @function
 *      Q3Polygon_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polygonData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_EmptyData (
    TQ3PolygonData                *polygonData
);



/*!
 *  @function
 *      Q3Polygon_GetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polygon          Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param point            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_GetVertexPosition (
    TQ3GeometryObject             polygon,
    TQ3Uns32                      index,
    TQ3Point3D                    *point
);



/*!
 *  @function
 *      Q3Polygon_SetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polygon          Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param point            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_SetVertexPosition (
    TQ3GeometryObject             polygon,
    TQ3Uns32                      index,
    const TQ3Point3D              *point
);



/*!
 *  @function
 *      Q3Polygon_GetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polygon          Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_GetVertexAttributeSet (
    TQ3GeometryObject             polygon,
    TQ3Uns32                      index,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3Polygon_SetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polygon          Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_SetVertexAttributeSet (
    TQ3GeometryObject             polygon,
    TQ3Uns32                      index,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3Polyhedron_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedronData   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Polyhedron_New (
    const TQ3PolyhedronData       *polyhedronData
);



/*!
 *  @function
 *      Q3Polyhedron_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedronData   Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_Submit (
    const TQ3PolyhedronData       *polyhedronData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Polyhedron_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedron       Description of the parameter.
 *  @param polyhedronData   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetData (
    TQ3GeometryObject             polyhedron,
    const TQ3PolyhedronData       *polyhedronData
);



/*!
 *  @function
 *      Q3Polyhedron_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedron       Description of the parameter.
 *  @param polyhedronData   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetData (
    TQ3GeometryObject             polyhedron,
    TQ3PolyhedronData             *polyhedronData
);



/*!
 *  @function
 *      Q3Polyhedron_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedronData   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_EmptyData (
    TQ3PolyhedronData             *polyhedronData
);



/*!
 *  @function
 *      Q3Polyhedron_SetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedron       Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param point            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetVertexPosition (
    TQ3GeometryObject             polyhedron,
    TQ3Uns32                      index,
    const TQ3Point3D              *point
);



/*!
 *  @function
 *      Q3Polyhedron_GetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedron       Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param point            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetVertexPosition (
    TQ3GeometryObject             polyhedron,
    TQ3Uns32                      index,
    TQ3Point3D                    *point
);



/*!
 *  @function
 *      Q3Polyhedron_SetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedron       Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetVertexAttributeSet (
    TQ3GeometryObject             polyhedron,
    TQ3Uns32                      index,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3Polyhedron_GetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedron       Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetVertexAttributeSet (
    TQ3GeometryObject             polyhedron,
    TQ3Uns32                      index,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3Polyhedron_GetTriangleData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedron       Description of the parameter.
 *  @param triangleIndex    Description of the parameter.
 *  @param triangleData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetTriangleData (
    TQ3GeometryObject             polyhedron,
    TQ3Uns32                      triangleIndex,
    TQ3PolyhedronTriangleData     *triangleData
);



/*!
 *  @function
 *      Q3Polyhedron_SetTriangleData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedron       Description of the parameter.
 *  @param triangleIndex    Description of the parameter.
 *  @param triangleData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetTriangleData (
    TQ3GeometryObject             polyhedron,
    TQ3Uns32                      triangleIndex,
    const TQ3PolyhedronTriangleData *triangleData
);



/*!
 *  @function
 *      Q3Polyhedron_GetEdgeData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedron       Description of the parameter.
 *  @param edgeIndex        Description of the parameter.
 *  @param edgeData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetEdgeData (
    TQ3GeometryObject             polyhedron,
    TQ3Uns32                      edgeIndex,
    TQ3PolyhedronEdgeData         *edgeData
);



/*!
 *  @function
 *      Q3Polyhedron_SetEdgeData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyhedron       Description of the parameter.
 *  @param edgeIndex        Description of the parameter.
 *  @param edgeData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetEdgeData (
    TQ3GeometryObject             polyhedron,
    TQ3Uns32                      edgeIndex,
    const TQ3PolyhedronEdgeData   *edgeData
);



/*!
 *  @function
 *      Q3PolyLine_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polylineData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3PolyLine_New (
    const TQ3PolyLineData         *polylineData
);



/*!
 *  @function
 *      Q3PolyLine_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyLineData     Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_Submit (
    const TQ3PolyLineData         *polyLineData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3PolyLine_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyLine         Description of the parameter.
 *  @param polyLineData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetData (
    TQ3GeometryObject             polyLine,
    const TQ3PolyLineData         *polyLineData
);



/*!
 *  @function
 *      Q3PolyLine_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyLine         Description of the parameter.
 *  @param polyLineData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetData (
    TQ3GeometryObject             polyLine,
    TQ3PolyLineData               *polyLineData
);



/*!
 *  @function
 *      Q3PolyLine_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyLineData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_EmptyData (
    TQ3PolyLineData               *polyLineData
);



/*!
 *  @function
 *      Q3PolyLine_GetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyLine         Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetVertexPosition (
    TQ3GeometryObject             polyLine,
    TQ3Uns32                      index,
    TQ3Point3D                    *position
);



/*!
 *  @function
 *      Q3PolyLine_SetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyLine         Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetVertexPosition (
    TQ3GeometryObject             polyLine,
    TQ3Uns32                      index,
    const TQ3Point3D              *position
);



/*!
 *  @function
 *      Q3PolyLine_GetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyLine         Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetVertexAttributeSet (
    TQ3GeometryObject             polyLine,
    TQ3Uns32                      index,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3PolyLine_SetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyLine         Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetVertexAttributeSet (
    TQ3GeometryObject             polyLine,
    TQ3Uns32                      index,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3PolyLine_GetSegmentAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyLine         Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetSegmentAttributeSet (
    TQ3GeometryObject             polyLine,
    TQ3Uns32                      index,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3PolyLine_SetSegmentAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param polyLine         Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetSegmentAttributeSet (
    TQ3GeometryObject             polyLine,
    TQ3Uns32                      index,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3Torus_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torusData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Torus_New (
    const TQ3TorusData            *torusData
);



/*!
 *  @function
 *      Q3Torus_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torusData        Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_Submit (
    const TQ3TorusData            *torusData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Torus_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param torusData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetData (
    TQ3GeometryObject             torus,
    const TQ3TorusData            *torusData
);



/*!
 *  @function
 *      Q3Torus_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param torusData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetData (
    TQ3GeometryObject             torus,
    TQ3TorusData                  *torusData
);



/*!
 *  @function
 *      Q3Torus_SetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetOrigin (
    TQ3GeometryObject             torus,
    const TQ3Point3D              *origin
);



/*!
 *  @function
 *      Q3Torus_SetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetOrientation (
    TQ3GeometryObject             torus,
    const TQ3Vector3D             *orientation
);



/*!
 *  @function
 *      Q3Torus_SetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetMajorRadius (
    TQ3GeometryObject             torus,
    const TQ3Vector3D             *majorRadius
);



/*!
 *  @function
 *      Q3Torus_SetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetMinorRadius (
    TQ3GeometryObject             torus,
    const TQ3Vector3D             *minorRadius
);



/*!
 *  @function
 *      Q3Torus_SetRatio
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param ratio            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_SetRatio (
    TQ3GeometryObject             torus,
    float                         ratio
);



/*!
 *  @function
 *      Q3Torus_GetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetOrigin (
    TQ3GeometryObject             torus,
    TQ3Point3D                    *origin
);



/*!
 *  @function
 *      Q3Torus_GetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetOrientation (
    TQ3GeometryObject             torus,
    TQ3Vector3D                   *orientation
);



/*!
 *  @function
 *      Q3Torus_GetMajorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param majorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetMajorRadius (
    TQ3GeometryObject             torus,
    TQ3Vector3D                   *majorRadius
);



/*!
 *  @function
 *      Q3Torus_GetMinorRadius
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param minorRadius      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetMinorRadius (
    TQ3GeometryObject             torus,
    TQ3Vector3D                   *minorRadius
);



/*!
 *  @function
 *      Q3Torus_GetRatio
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torus            Description of the parameter.
 *  @param ratio            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_GetRatio (
    TQ3GeometryObject             torus,
    float                         *ratio
);



/*!
 *  @function
 *      Q3Torus_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param torusData        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_EmptyData (
    TQ3TorusData                  *torusData
);



/*!
 *  @function
 *      Q3Triangle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triangleData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Triangle_New (
    const TQ3TriangleData         *triangleData
);



/*!
 *  @function
 *      Q3Triangle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triangleData     Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_Submit (
    const TQ3TriangleData         *triangleData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Triangle_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triangle         Description of the parameter.
 *  @param triangleData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_SetData (
    TQ3GeometryObject             triangle,
    const TQ3TriangleData         *triangleData
);



/*!
 *  @function
 *      Q3Triangle_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triangle         Description of the parameter.
 *  @param triangleData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_GetData (
    TQ3GeometryObject             triangle,
    TQ3TriangleData               *triangleData
);



/*!
 *  @function
 *      Q3Triangle_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triangleData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_EmptyData (
    TQ3TriangleData               *triangleData
);



/*!
 *  @function
 *      Q3Triangle_GetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triangle         Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param point            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_GetVertexPosition (
    TQ3GeometryObject             triangle,
    TQ3Uns32                      index,
    TQ3Point3D                    *point
);



/*!
 *  @function
 *      Q3Triangle_SetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triangle         Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param point            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_SetVertexPosition (
    TQ3GeometryObject             triangle,
    TQ3Uns32                      index,
    const TQ3Point3D              *point
);



/*!
 *  @function
 *      Q3Triangle_GetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triangle         Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_GetVertexAttributeSet (
    TQ3GeometryObject             triangle,
    TQ3Uns32                      index,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3Triangle_SetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triangle         Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Triangle_SetVertexAttributeSet (
    TQ3GeometryObject             triangle,
    TQ3Uns32                      index,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3TriGrid_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triGridData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3TriGrid_New (
    const TQ3TriGridData          *triGridData
);



/*!
 *  @function
 *      Q3TriGrid_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triGridData      Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_Submit (
    const TQ3TriGridData          *triGridData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3TriGrid_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triGrid          Description of the parameter.
 *  @param triGridData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetData (
    TQ3GeometryObject             triGrid,
    const TQ3TriGridData          *triGridData
);



/*!
 *  @function
 *      Q3TriGrid_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triGrid          Description of the parameter.
 *  @param triGridData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetData (
    TQ3GeometryObject             triGrid,
    TQ3TriGridData                *triGridData
);



/*!
 *  @function
 *      Q3TriGrid_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triGridData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_EmptyData (
    TQ3TriGridData                *triGridData
);



/*!
 *  @function
 *      Q3TriGrid_GetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triGrid          Description of the parameter.
 *  @param rowIndex         Description of the parameter.
 *  @param columnIndex      Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetVertexPosition (
    TQ3GeometryObject             triGrid,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    TQ3Point3D                    *position
);



/*!
 *  @function
 *      Q3TriGrid_SetVertexPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triGrid          Description of the parameter.
 *  @param rowIndex         Description of the parameter.
 *  @param columnIndex      Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetVertexPosition (
    TQ3GeometryObject             triGrid,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    const TQ3Point3D              *position
);



/*!
 *  @function
 *      Q3TriGrid_GetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triGrid          Description of the parameter.
 *  @param rowIndex         Description of the parameter.
 *  @param columnIndex      Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetVertexAttributeSet (
    TQ3GeometryObject             triGrid,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3TriGrid_SetVertexAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triGrid          Description of the parameter.
 *  @param rowIndex         Description of the parameter.
 *  @param columnIndex      Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetVertexAttributeSet (
    TQ3GeometryObject             triGrid,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3TriGrid_GetFacetAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triGrid          Description of the parameter.
 *  @param faceIndex        Description of the parameter.
 *  @param facetAttributeSet Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetFacetAttributeSet (
    TQ3GeometryObject             triGrid,
    TQ3Uns32                      faceIndex,
    TQ3AttributeSet               *facetAttributeSet
);



/*!
 *  @function
 *      Q3TriGrid_SetFacetAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triGrid          Description of the parameter.
 *  @param faceIndex        Description of the parameter.
 *  @param facetAttributeSet Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetFacetAttributeSet (
    TQ3GeometryObject             triGrid,
    TQ3Uns32                      faceIndex,
    TQ3AttributeSet               facetAttributeSet
);



/*!
 *  @function
 *      Q3TriMesh_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triMeshData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3TriMesh_New (
    const TQ3TriMeshData          *triMeshData
);



/*!
 *  @function
 *      Q3TriMesh_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triMeshData      Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriMesh_Submit (
    const TQ3TriMeshData          *triMeshData,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3TriMesh_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triMesh          Description of the parameter.
 *  @param triMeshData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriMesh_SetData (
    TQ3GeometryObject             triMesh,
    const TQ3TriMeshData          *triMeshData
);



/*!
 *  @function
 *      Q3TriMesh_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triMesh          Description of the parameter.
 *  @param triMeshData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriMesh_GetData (
    TQ3GeometryObject             triMesh,
    TQ3TriMeshData                *triMeshData
);



/*!
 *  @function
 *      Q3TriMesh_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param triMeshData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriMesh_EmptyData (
    TQ3TriMeshData                *triMeshData
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



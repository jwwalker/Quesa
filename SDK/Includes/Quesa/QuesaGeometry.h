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
#pragma enumsalwaysint on

#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
 *	@enum	TQ3GeneralPolygonShapeHint
 *	@discussion
 *		A general polygon has a shape hint that may be used by the renderer to
 *		optimize drawing.
 *	@constant	kQ3GeneralPolygonShapeHintComplex	The general polygon has more
 *							than one contour, or is self-intersecting, or it is not
 *							known whether it is concave or convex.
 *	@constant	kQ3GeneralPolygonShapeHintConcave	There is exactly one contour, which
 *							is concave.
 *	@constant	kQ3GeneralPolygonShapeHintConvex	There is exactly one contour, which
 *							is convex.
 */
typedef enum TQ3GeneralPolygonShapeHint {
    kQ3GeneralPolygonShapeHintComplex           = 0,
    kQ3GeneralPolygonShapeHintConcave           = 1,
    kQ3GeneralPolygonShapeHintConvex            = 2
} TQ3GeneralPolygonShapeHint;


/*!
 *	@enum Nurb&nbsp;Limits
 *	@discussion
 *		Miscellaneous limits for NURB curves and patches.
 *	@constant	kQ3NURBCurveMaxOrder	Maximum order for NURB curves.
 *	@constant	kQ3NURBPatchMaxOrder	Maximum order for NURB patches. Note that this was
 *                                      set to 11 in QD3DGeometry.h, however a Develop article
 *                                      on NURBs indicated it should be 16.
 */
enum {
	kQ3NURBCurveMaxOrder = 16,
	kQ3NURBPatchMaxOrder = 16
};


/*!
 *	@enum	TQ3PolyhedronEdgeMasks
 *	@discussion
 *		These are flags indicating which edges of a polyhedral triangle should be
 *		rendered.
 *	@constant	kQ3PolyhedronEdgeNone		No Edge.
 *	@constant	kQ3PolyhedronEdge01			Render the edge between vertex 0 and vertex 1.
 *	@constant	kQ3PolyhedronEdge12			Render the edge between vertex 1 and vertex 2.
 *	@constant	kQ3PolyhedronEdge20			Render the edge between vertex 2 and vertex 0.
 *	@constant	kQ3PolyhedronEdgeAll		Render all the edges.
 */
typedef enum TQ3PolyhedronEdgeMasks {
    kQ3PolyhedronEdgeNone                       = 0,
    kQ3PolyhedronEdge01                         = (1 << 0),
    kQ3PolyhedronEdge12                         = (1 << 1),
    kQ3PolyhedronEdge20                         = (1 << 2),
    kQ3PolyhedronEdgeAll                        = (kQ3PolyhedronEdge01 | kQ3PolyhedronEdge12 | kQ3PolyhedronEdge20)
} TQ3PolyhedronEdgeMasks;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
/*!
 *	@struct		TQ3BoxData
 *	@discussion
 *		Data describing the state of a box object.
 *	@field		origin				Origin of the box (one of the corners).
 *	@field		orientation			Orientation vector of the box.
 *	@field		majorAxis			Major axis of the box.
 *	@field		minorAxis			Minor axis of the box.
 *	@field		faceAttributeSet	Array of attribute set objects for the 6 faces.
 *									This field may be NULL, or individual sets in the
 *									array may be NULL.
 *	@field		boxAttributeSet		Attribute set object holding attributes that apply
 *									to all of the faces.  May be NULL.
 */
typedef struct TQ3BoxData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Vector3D                                 majorAxis;
    TQ3Vector3D                                 minorAxis;
    TQ3AttributeSet                             *faceAttributeSet;
    TQ3AttributeSet                             boxAttributeSet;
} TQ3BoxData;


/*!
 *	@struct		TQ3ConeData
 *	@discussion
 *		Data describing the state of a cone object.  The orientation, major radius,
 *		and minor radius vectors need not be orthogonal, though they should be
 *		independent.
 *
 *		The values <code>uMin</code>, <code>uMax</code>, <code>vMin</code>, and <code>vMax</code>
 *		were apparently intended to allow partial cones, e.g., a cone with a wedge
 *		cut out of it.
 *		But Quesa does not implement this feature, and I do not believe that QuickDraw 3D
 *		ever did either.
 *
 *	@field		origin					The center of the base of the cone.
 *	@field		orientation				Vector from the origin to the tip of the cone.
 *	@field		majorRadius				A vector from the origin to a point on the perimeter
 *										of the base.
 *	@field		minorRadius				A vector from the origin to a point on the perimeter
 *										of the base.
 *	@field		uMin					Minimum value of the u parameter, which goes around
 *										the base.  Typically 0.
 *	@field		uMax					Maximum value of the u parameter, which goes around
 *										the base.  Typically 1.
 *	@field		vMin					Minimum value of the v parameter, which goes from
 *										base to tip.  Typically 0.
 *	@field		vMax					Minimum value of the v parameter, which goes from
 *										base to tip.  Typically 1.
 *	@field		caps					End cap masks, either <code>kQ3EndCapMaskBottom</code>
 *										or <code>kQ3EndCapNone</code>.
 *	@field		interiorAttributeSet	Interior attributes.  Currently unused by Quesa
 *										rendering, so leave it NULL.
 *	@field		faceAttributeSet		Attributes that affect the face but not the bottom.
 *										May be NULL.
 *	@field		bottomAttributeSet		Attributes that affect the bottom end cap.  May be NULL.
 *	@field		coneAttributeSet		Attributes for all parts of the cone.  May be NULL.
 */
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


/*!
 *	@struct		TQ3CylinderData
 *	@discussion
 *		Data describing the state of a cylinder object.  The orientation, major radius,
 *		and minor radius vectors need not be orthogonal, though they should be
 *		independent.
 *
 *		The values <code>uMin</code>, <code>uMax</code>, <code>vMin</code>, and <code>vMax</code>
 *		were apparently intended to allow partial cylinders, e.g., a cylinder with a wedge
 *		cut out of it.
 *		But Quesa does not implement this feature, and I do not believe that QuickDraw 3D
 *		ever did either.
 *
 *	@field		origin					The center of the base of the cylinder.
 *	@field		orientation				Vector from the origin to the center of the opposite end.
 *	@field		majorRadius				A vector from the origin to a point on the perimeter
 *										of the base.
 *	@field		minorRadius				A vector from the origin to a point on the perimeter
 *										of the base.
 *	@field		uMin					Minimum value of the u parameter, which goes around
 *										the base.  Typically 0.
 *	@field		uMax					Maximum value of the u parameter, which goes around
 *										the base.  Typically 1.
 *	@field		vMin					Minimum value of the v parameter, which goes from the
 *										base to the other end.  Typically 0.
 *	@field		vMax					Minimum value of the v parameter, which goes from the
 *										base to the other end.  Typically 1.
 *	@field		caps					End cap masks, determining whether the cylinder is
 *										closed on one end, the other, or both.
 *	@field		interiorAttributeSet	Interior attributes.  Currently unused by Quesa
 *										rendering, so leave it NULL.
 *	@field		topAttributeSet			Attributes that affect the top end cap.  May be NULL.
 *	@field		faceAttributeSet		Attributes that affect the face but not the bottom or top.
 *										May be NULL.
 *	@field		bottomAttributeSet		Attributes that affect the bottom end cap.  May be NULL.
 *	@field		cylinderAttributeSet	Attributes for all parts of the cylinder.  May be NULL.
 */
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


/*!
 *	@struct		TQ3DiskData
 *	@discussion
 *		Data describing the state of a disk object (a filled ellipse).  The major radius
 *		and minor radius vectors need not be orthogonal, though they should be independent.
 *
 *		The values <code>uMin</code>, <code>uMax</code>, <code>vMin</code>, and <code>vMax</code>
 *		were apparently intended to allow partial disks, e.g., a disk with a wedge
 *		cut out of it.
 *		But Quesa does not implement this feature, and I do not believe that QuickDraw 3D
 *		ever did either.
 *
 *	@field		origin					The center of the disk.
 *	@field		majorRadius				A vector from the origin to a point on the perimeter
 *										of the disk.
 *	@field		minorRadius				A vector from the origin to a point on the perimeter
 *										of the disk.
 *	@field		uMin					Minimum value of the u parameter, which goes around
 *										the perimeter.  Typically 0.
 *	@field		uMax					Maximum value of the u parameter, which goes around
 *										the perimeter.  Typically 1.
 *	@field		vMin					Minimum value of the v parameter, which goes from the
 *										perimeter to the origin.  Typically 0.
 *	@field		vMax					Minimum value of the v parameter, which goes from the
 *										perimeter to the origin.  Typically 1.
 *	@field		diskAttributeSet		Attributes for the disk.  May be NULL.
 */
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


/*!
 *	@struct		TQ3EllipseData
 *	@discussion
 *		Data describing the state of an ellipse.  The major radius and minor radius
 *		vectors need not be orthogonal, though they should be independent.
 *		You can make a partial ellipse by using values other than 0 and 1 for the
 *		<code>uMin</code> and <code>uMax</code> fields.
 *	@field		origin					Center of the ellipse.
 *	@field		majorRadius				A vector from the origin to a point on the curve.
 *	@field		minorRadius				Another vector from the origin to a point on the curve.
 *	@field		uMin					Minimum value of the u parameter.  Typically 0.
 *	@field		uMax					Maximum value of the u parameter.  Typically 1.
 *	@field		ellipseAttributeSet		Attributes for the ellipse.  May be NULL.
 */
typedef struct TQ3EllipseData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       uMin;
    float                                       uMax;
    TQ3AttributeSet                             ellipseAttributeSet;
} TQ3EllipseData;


/*!
 *	@struct		TQ3EllipsoidData
 *	@discussion
 *		Data describing the state of an ellipsoid.  The major axis, minor axis, and
 *		orientation vectors need not be orthogonal, but should be independent.
 *		The ellipsoid can be expressed by a parametric equation
 *
 *		f(u,v) = origin + cos(&pi;v)orientation + sin(&pi;v)(cos(2&pi;u)majorRadius + sin(2&pi;u)minorRadius)
 *
 *		where u and v range from 0 to 1.
 *
 *		The values <code>uMin</code>, <code>uMax</code>, <code>vMin</code>, and <code>vMax</code>
 *		were apparently intended to allow partial ellipsoids, e.g., an ellipsoid with a wedge
 *		cut out of it.
 *		But Quesa does not implement this feature, and I do not believe that QuickDraw 3D
 *		ever did either.
 *
 *	@field		origin					Center of the ellipsoid.
 *	@field		orientation				A vector from the origin to a point on the ellipsoid.
 *	@field		majorRadius				A vector from the origin to a point on the ellipsoid.
 *	@field		minorRadius				A vector from the origin to a point on the ellipsoid.
 *	@field		uMin					Minimum value of the u parameter, which goes around
 *										the perimeter (the ellipse determined by the major and
 *										minor axes).  Typically 0.
 *	@field		uMax					Maximum value of the u parameter.  Typically 1.
 *	@field		vMin					Minimum value of the v parameter, which goes from the
 *										south pole (origin - orientation) to the north pole
 *										(origin + orientation).  Typically 0.
 *	@field		vMax					Minimum value of the v parameter.  Typically 1.
 *	@field		caps					Style of caps to be used on partial ellipsoids.
 *										Currently ignored by Quesa.
 *	@field		interiorAttributeSet	Interior attributes.  Currently unused by Quesa
 *										rendering, so leave it NULL.
 *	@field		ellipsoidAttributeSet	Attributes for the ellipsoid surface.  May be NULL.
 */
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


/*!
 *	@struct		TQ3GeneralPolygonContourData
 *	@discussion
 *		An ordered list of vertices forming a contour of a general polygon.
 *		Used within the <code>TQ3GeneralPolygonData</code> structure.
 *	@field		numVertices				Number of vertices.  Must be at least 3.
 *	@field		vertices				Array of vertices.
 */
typedef struct TQ3GeneralPolygonContourData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 *vertices;
} TQ3GeneralPolygonContourData;


// General polygon data
/*!
 *	@struct		TQ3GeneralPolygonData
 *	@discussion
 *		Data describing a general polygon.  A general polygon is a closed figure
 *		defined by one or more coplanar closed curves called contours.  If there are
 *		holes, the even-odd rule determines which parts are inside.
 *	@field		numContours				Number of contours in the following array.
 *										Must be at least 1.
 *	@field		contours				Array of contours.
 *	@field		shapeHint				Information about the shape of the general polygon,
 *										which may be used by the renderer to optimize
 *										drawing.
 *	@field		generalPolygonAttributeSet		Set of attributes for the polygon.
 *										May be NULL.
 */
typedef struct TQ3GeneralPolygonData {
    TQ3Uns32                                    numContours;
    TQ3GeneralPolygonContourData                *contours;
    TQ3GeneralPolygonShapeHint                  shapeHint;
    TQ3AttributeSet                             generalPolygonAttributeSet;
} TQ3GeneralPolygonData;


/*!
 *	@struct		TQ3LineData
 *	@discussion
 *		Data describing a line.
 *	@field		vertices				Array of two vertices, the ends of the line.
 *	@field		lineAttributeSet		Set of attributes for the line.  May be NULL.
 */
typedef struct TQ3LineData {
    TQ3Vertex3D                                 vertices[2];
    TQ3AttributeSet                             lineAttributeSet;
} TQ3LineData;


/*!
 *	@struct		TQ3MarkerData
 *	@discussion
 *		Data describing a bitmap marker, a 2-dimensional image drawn on top of a scene
 *		at a specified location.
 *	@field		location				Location of the marker, in world coordinates.
 *	@field		xOffset					Horizontal offset from the <code>location</code> to the
 *										upper left corner of the marker, in pixels.
 *	@field		yOffset					Vertical offset from the <code>location</code> to the
 *										upper left corner of the marker, in pixels.
 *	@field		bitmap					A bitmap.  Each bit corresponds to a pixel in the image.
 *	@field		markerAttributeSet		Marker attributes, which can for instance modify the color
 *										or transparency of the 1 bits.  May be NULL.
 */
typedef struct TQ3MarkerData {
    TQ3Point3D                                  location;
    TQ3Int32                                    xOffset;
    TQ3Int32                                    yOffset;
    TQ3Bitmap                                   bitmap;
    TQ3AttributeSet                             markerAttributeSet;
} TQ3MarkerData;


// Mesh data (all opaque)
/*!
 *	@typedef	TQ3MeshComponent
 *	@discussion	Opaque pointer representing a connected component of a Mesh.
 */
typedef struct OpaqueTQ3MeshComponent           *TQ3MeshComponent;
/*!
 *	@typedef	TQ3MeshVertex
 *	@discussion	Opaque pointer representing a vertex of a Mesh.
 */
typedef struct OpaqueTQ3MeshVertex              *TQ3MeshVertex;
/*!
 *	@typedef	TQ3MeshFace
 *	@discussion	Opaque pointer representing a face of a Mesh.  This is a polygonal
 *				figure, normally planar, which may contain holes.
 */
typedef struct OpaqueTQ3MeshFace                *TQ3MeshFace;
/*!
 *	@typedef	TQ3MeshEdge
 *	@discussion	Opaque pointer representing an edge of a Mesh, a straight line
 *				segment that connects two vertices.
 */
typedef struct OpaqueTQ3MeshEdge                *TQ3MeshEdge;
/*!
 *	@typedef	TQ3MeshContour
 *	@discussion	Opaque pointer representing a contour of a Mesh, one of the closed
 *				paths that bounds a face.
 */
typedef struct OpaqueTQ3MeshContour             *TQ3MeshContour;

/*!
 *	@struct		TQ3MeshIterator
 *	@discussion	This structure is used for iterating through various parts of a
 *				Mesh.  You should consider it opaque, because the meanings of the
 *				fields are not documented.
 */
typedef struct TQ3MeshIterator {
    void                                        *var1;
    void                                        *var2;
    void                                        *var3;
    struct {
        void                                    *field1;
        char                                    field2[4];
    } var4;
} TQ3MeshIterator;


/*!
 *	@struct		TQ3NURBCurveData
 *	@discussion	Data defining a NURBS curve, a 3-dimensional curve represented by
 *				a nonuniform rational B-spline equation.
 *	@field		order							The order of the curve, one more than the
 *												degree of the polynomials defining the curve.
 *												Must be greater than one.
 *	@field		numPoints						Number of control points.  Must be greater than
 *												or equal to the order.
 *	@field		controlPoints					Array of rational 4-dimensional control points.
 *	@field		knots							Array of knots that define the curve.  The number
 *												of knots must equal the sum of <code>order</code>
 *												and <code>numPoints</code>.  The values must be
 *												nondecreasing.
 *	@field		curveAttributeSet				Set of attributes for the curve.  May be NULL.
 */
typedef struct TQ3NURBCurveData {
    TQ3Uns32                                    order;
    TQ3Uns32                                    numPoints;
    TQ3RationalPoint4D                          *controlPoints;
    float                                       *knots;
    TQ3AttributeSet                             curveAttributeSet;
} TQ3NURBCurveData;


/*!
 *	@struct		TQ3NURBPatchTrimCurveData
 *	@discussion
 *		Curve that trims a NURB patch.  Note that this is similar to TQ3NURBCurveData,
 *		but lacks an attribute set.
 *	@field		order							The order of the curve, one more than the
 *												degree of the polynomials defining the curve.
 *												Must be greater than one.
 *	@field		numPoints						Number of control points.  Must be greater than
 *												or equal to the order.
 *	@field		controlPoints					Array of rational 4-dimensional control points.
 *	@field		knots							Array of knots that define the curve.  The number
 *												of knots must equal the sum of <code>order</code>
 *												and <code>numPoints</code>.  The values must be
 *												nondecreasing.
 */
typedef struct TQ3NURBPatchTrimCurveData {
    TQ3Uns32                                    order;
    TQ3Uns32                                    numPoints;
    TQ3RationalPoint3D                          *controlPoints;
    float                                       *knots;
} TQ3NURBPatchTrimCurveData;

/*!
 *	@struct		TQ3NURBPatchTrimLoopData
 *	@discussion
 *		Data describing curves that trim a NURB patch.
 *	@field		numTrimCurves			Number of curves in the following array.
 *	@field		trimCurves				Pointer to an array of curves.
 */
typedef struct TQ3NURBPatchTrimLoopData {
    TQ3Uns32                                    numTrimCurves;
    TQ3NURBPatchTrimCurveData                   *trimCurves;
} TQ3NURBPatchTrimLoopData;

/*!
 *	@struct		TQ3NURBPatchData
 *	@discussion
 *		Data describing a NURB patch, a surface defined by a ratio of B-spline surfaces.
 *	@field		uOrder					Order of the NURB patch in the u parametric direction.
 *										The order is one greater than the degree of the
 *										polynomial functions involved, and must be
 *										greater than one.
 *	@field		vOrder					Order of the NURB patch in the v parametric direction.
 *										The order is one greater than the degree of the
 *										polynomial functions involved, and must be
 *										greater than one.
 *	@field		numRows					Number of control points in the u parametric equation.
 *										Must be greater than 1.
 *	@field		numColumns				Number of control points in the v parametric equation.
 *										Must be greater than 1.
 *	@field		controlPoints			Array of rational 4-dimensional control points.
 *										They are listed first in the direction of increasing u
 *										and then in the direction of increasing v.
 *										The number of control points is <code>numRows</code> times
 *										<code>numColumns</code>.
 *	@field		uKnots					Array of knots in the u parametric direction.  The
 *										number of these knots is the sum of <code>uOrder</code>
 *										and <code>numColumns</code>.  The values must be
 *										nondecreasing.
 *	@field		vKnots					Array of knots in the v parametric direction.  The
 *										number of these knots is the sum of <code>vOrder</code>
 *										and <code>numRows</code>.  The values must be
 *										nondecreasing.
 *	@field		numTrimLoops			Number of trim loops in the following array.
 *										Currently this should be 0.
 *	@field		trimLoops				Pointer to an array of trim loop structures.
 *										Currently this should be NULL.
 *	@field		patchAttributeSet		Set of attributes for the patch.  May be NULL.
 */
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


/*!
 *	@struct		TQ3PixmapMarkerData
 *	@discussion
 *		Data describing a pixmap marker, a 2-dimensional color image drawn on top of a scene
 *		at a specified location.
 *	@field		position				Location of the marker, in world coordinates.
 *	@field		xOffset					Horizontal offset from the <code>position</code> to the
 *										upper left corner of the marker, in pixels.
 *	@field		yOffset					Vertical offset from the <code>position</code> to the
 *										upper left corner of the marker, in pixels.
 *	@field		pixmap					A pixmap.
 *	@field		pixmapMarkerAttributeSet		A set of attributes for the marker.  May be NULL.
 */
typedef struct TQ3PixmapMarkerData {
    TQ3Point3D                                  position;
    TQ3Int32                                    xOffset;
    TQ3Int32                                    yOffset;
    TQ3StoragePixmap                            pixmap;
    TQ3AttributeSet                             pixmapMarkerAttributeSet;
} TQ3PixmapMarkerData;


/*!
 *	@struct		TQ3PointData
 *	@discussion
 *		Data describing a point object.
 *	@field		point					Location of the point.
 *	@field		pointAttributeSet		Set of attributes for the point.  May be NULL.
 */
typedef struct TQ3PointData {
    TQ3Point3D                                  point;
    TQ3AttributeSet                             pointAttributeSet;
} TQ3PointData;


/*!
 *	@struct		TQ3PolygonData
 *	@discussion
 *		Data describing a simple convex polygon.
 *	@field		numVertices				Number of vertices.  Must be at least 3.
 *	@field		vertices				Pointer to an array of vertices.
 *	@field		polygonAttributeSet		Set of attributes for the polygon.  May be NULL.
 */
typedef struct TQ3PolygonData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 *vertices;
    TQ3AttributeSet                             polygonAttributeSet;
} TQ3PolygonData;


// Polyhedron data
/*!
 *	@typedef	TQ3PolyhedronEdge
 *	@discussion
 *		A combination of masks of type TQ3PolyhedronEdgeMasks indicating which edges of
 *		a triangle are to be rendered.
 */
typedef TQ3Uns32                                TQ3PolyhedronEdge;

/*!
 *	@struct		TQ3PolyhedronEdgeData
 *	@discussion
 *		Data describing an edge within a polyhedron.
 *	@field		vertexIndices			Indices of the two vertices that are ends of the edge.
 *	@field		triangleIndices			Indices of the two triangles that contain the edge.
 *	@field		edgeAttributeSet		Set of attributes for the edge.  May be NULL.
 */
typedef struct TQ3PolyhedronEdgeData {
    TQ3Uns32                                    vertexIndices[2];
    TQ3Uns32                                    triangleIndices[2];
    TQ3AttributeSet                             edgeAttributeSet;
} TQ3PolyhedronEdgeData;

/*!
 *	@struct		TQ3PolyhedronTriangleData
 *	@discussion
 *		Data describing a triangle within a polyhedron.
 *	@field		vertexIndices			Indices of the 3 vertices of the triangle.
 *	@field		edgeFlag				Flags indicating which edges of the triangle should
 *										be rendered.  See <code>TQ3PolyhedronEdgeMasks</code>.
 *	@field		triangleAttributeSet	Set of attributes for the triangle.  May be NULL.
 */
typedef struct TQ3PolyhedronTriangleData {
    TQ3Uns32                                    vertexIndices[3];
    TQ3PolyhedronEdge                           edgeFlag;
    TQ3AttributeSet                             triangleAttributeSet;
} TQ3PolyhedronTriangleData;

/*!
 *	@struct		TQ3PolyhedronData
 *	@discussion
 *		Data describing a polyhedron.
 *	@field		numVertices				Number of vertices in the following array.
 *	@field		vertices				Pointer to array of vertices of the polyhedron.
 *	@field		numEdges				Number of edges in the following array.  May be 0
 *										if you do not want to specify any edges.
 *	@field		edges					Pointer to an array of edges.  May be NULL if you
 *										also specify 0 for <code>numEdges</code>.
 *	@field		numTriangles			Number of triangles (faces) in the polygon.
 *	@field		triangles				Pointer to an array of triangles.
 *	@field		polyhedronAttributeSet	Set of attributes for the polyhedron.  May be NULL.
 */
typedef struct TQ3PolyhedronData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 *vertices;
    TQ3Uns32                                    numEdges;
    TQ3PolyhedronEdgeData                       *edges;
    TQ3Uns32                                    numTriangles;
    TQ3PolyhedronTriangleData                   *triangles;
    TQ3AttributeSet                             polyhedronAttributeSet;
} TQ3PolyhedronData;


/*!
 *	@struct		TQ3PolyLineData
 *	@discussion
 *		Data describing a connected but not closed curve made up of several straight line
 *		segments.
 *	@field		numVertices				Number of vertices of the curve (one more than the
 *										number of line segments).  Must be at least 2.
 *	@field		vertices				Pointer to an array of vertices.
 *	@field		segmentAttributeSet		Pointer to an array of attribute sets, one for each
 *										segment.  If you do not want to assign attributes
 *										to any segment, this pointer may be NULL.
 *	@field		polyLineAttributeSet	Set of attributes for the whole curve.  May be NULL.
 */
typedef struct TQ3PolyLineData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 *vertices;
    TQ3AttributeSet                             *segmentAttributeSet;
    TQ3AttributeSet                             polyLineAttributeSet;
} TQ3PolyLineData;


/*!
 *	@struct		TQ3TorusData
 *	@discussion
 *		Data describing a generalized torus.  A torus is a surface formed by rotating an
 *		ellipse about an axis that is in the same plane as the ellipse but does not pass
 *		through the ellipse.
 *
 *		The kind of torus usually encountered in elementary mathematics, with circular
 *		cross sections, would be one where the <code>orientation</code>, <code>majorRadius</code>,
 *		and <code>minorRadius</code> vectors are mutually orthogonal, where
 *		<code>majorRadius</code> and <code>minorRadius</code> have the same length, and where
 *		<code>ratio</code> is 1.
 *
 *		The values <code>uMin</code>, <code>uMax</code>, <code>vMin</code>, and <code>vMax</code>
 *		were apparently intended to allow partial tori, e.g., a torus with a wedge
 *		cut out of it.
 *		But Quesa does not implement this feature, and I do not believe that QuickDraw 3D
 *		ever did either.
 *
 *	@field		origin					Center of rotation.
 *	@field		orientation				Vector whose direction is the axis of rotation, and
 *										whose length is the length of the radius of the ellipse
 *										in the direction of the axis of rotation.
 *	@field		majorRadius				Vector from the origin to the center of the ellipse.
 *	@field		minorRadius				Vector from the origin to the center of a different
 *										cross-sectional ellipse.
 *	@field		ratio					The ratio between the radius of the ellipse in the direction
 *										of <code>majorRadius</code>, and the length of
 *										<code>orientation</code>.
 *	@field		uMin					Minimum value in the u parametric direction (the long way
 *										around.)  Normally 0.
 *	@field		uMax					Maximum value in the u parametric direction (the long way
 *										around.)  Normally 1.
 *	@field		vMin					Minimum value in the v parametric direction (the short way
 *										around.)  Normally 0.
 *	@field		vMax					Maximum value in the v parametric direction (the short way
 *										around.)  Normally 1.
 *	@field		caps					Cap style.  Should be kQ3EndCapNone.
 */
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


/*!
 *	@struct		TQ3TriangleData
 *	@discussion
 *		Data defining a single triangle.
 *	@field		vertices				The 3 vertices of the triangle.
 *	@field		triangleAttributeSet	Set of attributes for the triangle.  May be NULL.
 */
typedef struct TQ3TriangleData {
    TQ3Vertex3D                                 vertices[3];
    TQ3AttributeSet                             triangleAttributeSet;
} TQ3TriangleData;


/*!
 *	@struct		TQ3TriGridData
 *	@discussion
 *		Data describing a TriGrid object.  A TriGrid is defined by a grid of points in
 *		3-dimensional space.  Each set of 4 adjacent points (in the mth and (m+1)st row and
 *		nth and (n+1)st column) defines a quadrilateral that can be subdivided into
 *		triangles in 2 natural ways.  This subdivision is done in an alternating way,
 *		such that the vertex in the first row and first column belongs to two triangles.
 *	@field		numRows					Number of rows of vertices.  Should be at least 2.
 *	@field		numColumns				Number of columns of vertices.  Should be at least 2.
 *	@field		vertices				Pointer to an array of vertices, listed in rectangular
 *										order, first by increasing columns and then by
 *										increasing rows.  The number of vertices should be
 *										<code>numRows</code> times <code>numColumns</code>.
 *	@field		facetAttributeSet		Pointer to an array of triangle attributes.  May be
 *										NULL, but otherwise should point to an array of
 *										2&times;(<code>numRows</code>-1)&times;(<code>numColumns</code>-1)
 *										attribute sets.
 *	@field		triGridAttributeSet		Set of attributes for the whole TriGrid object.
 *										May be NULL.	
 */
typedef struct TQ3TriGridData {
    TQ3Uns32                                    numRows;
    TQ3Uns32                                    numColumns;
    TQ3Vertex3D                                 *vertices;
    TQ3AttributeSet                             *facetAttributeSet;
    TQ3AttributeSet                             triGridAttributeSet;
} TQ3TriGridData;


/*!
 *	@struct		TQ3TriMeshTriangleData
 *	@discussion
 *		Data defining a triangle within a TriMesh.
 *	@field		pointIndices			Array of 3 indices into the TriMesh array of points.
 */
typedef struct TQ3TriMeshTriangleData {
    TQ3Uns32                                    pointIndices[3];
} TQ3TriMeshTriangleData;

/*!
 *	@struct		TQ3TriMeshEdgeData
 *	@discussion
 *		Data defining an edge within a TriMesh.
 *	@field		pointIndices			Array of 2 indices into the TriMesh array of points,
 *										the ends of the edge.
 *	@field		triangleIndices			Array of 2 indices into the TriMesh array of triangles,
 *										the triangles containing the edge.
 */
typedef struct TQ3TriMeshEdgeData {
    TQ3Uns32                                    pointIndices[2];
    TQ3Uns32                                    triangleIndices[2];
} TQ3TriMeshEdgeData;

/*!
 *	@struct		TQ3TriMeshAttributeData
 *	@discussion
 *		A structure holding an array of attribute data of a particular type.
 *	@field		attributeType			Type of the attribute.
 *	@field		data					Pointer to an array of attribute data.  The number of
 *										members in the array is determined by context in the
 *										enclosing <code>TQ3TriMeshData</code> structure; for
 *										instance, if these are vertex attributes, the number
 *										of members is the number of vertices.  The size of each
 *										member depends on the attribute type.
 *	@field		attributeUseArray		For non-custom attribute types, this must be NULL.
 *										For custom attribute types, it can point to an array
 *										of 1-byte 0s and 1s, with 1s indicating which items
 *										have the custom attribute.
 */
typedef struct TQ3TriMeshAttributeData {
    TQ3AttributeType                            attributeType;
    void                                        *data;
    char                                        *attributeUseArray;
} TQ3TriMeshAttributeData;

/*!
 *	@struct		TQ3TriMeshData
 *	@discussion
 *		Structure describing a TriMesh object, which is an object composed of vertices, edges,
 *		and triangular faces.  The main difference between a TriMesh and a Polyhedron is that
 *		attribute data for vertices, edges, and faces are not stored in attribute sets, but
 *		in arrays of attribute data.  This is normally more efficient, so long as you are
 *		going to be assigning an attribute to every vertex, or every edge, or every face.
 *	@field		triMeshAttributeSet		Set of attributes for the whole object.  May be NULL.
 *	@field		numTriangles			Number of triangles in the following array.
 *	@field		triangles				Pointer to an array of triangle data.
 *	@field		numTriangleAttributeTypes	Number of triangle attribute types listed in the
 *											following array.
 *	@field		triangleAttributeTypes		Pointer to an array of attribute data for triangles (faces).
 *											May be NULL, if <code>numTriangleAttributeTypes</code> is 0.
 *	@field		numEdges				Number of edges in the following array.
 *	@field		edges					Pointer to an array of edge data.  If you do not wish to
 *										specify any edges, you can set this to NULL, and set
 *										<code>numEdges</code> to 0.
 *	@field		numEdgeAttributeTypes	Number of edge attribute types in the following array.
 *	@field		edgeAttributeTypes		Pointer to an array of edge attribute types.  May be NULL,
 *										provided that <code>numEdgeAttributeTypes</code> is 0.
 *	@field		numPoints				Number of points (vertices).
 *	@field		points					Pointer to an array of points.
 *	@field		numVertexAttributeTypes	Number of vertex attribute types in the following array.
 *	@field		vertexAttributeTypes	Pointer to an array of vertex attribute data.  May be NULL,
 *										so long as <code>numVertexAttributeTypes</code> is 0.
 *	@field		bBox					Bounding box of the TriMesh.
 */
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
/*!
 *	@function		Q3ForEachMeshComponent
 *	@discussion
 *		Macro to aid in iterating over components of a Mesh object.  Example:
 *
 *		<blockquote><pre><code>
 *		TQ3MeshIterator		iter;
 *		TQ3MeshComponent	comp;
 *		Q3ForEachMeshComponent( myMesh, comp, &amp;iter )
 *		{
 *		&nbsp;	DoSomething( comp );
 *		}
 *		</code></pre></blockquote>
 *	@param		_m						The mesh object.
 *	@param		_c						The component.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachMeshComponent(_m, _c, _i)                  \
        for ((_c) = Q3Mesh_FirstMeshComponent((_m), (_i));  \
             (_c) != NULL;                                  \
             (_c) = Q3Mesh_NextMeshComponent((_i)))

/*!
 *	@function		Q3ForEachComponentVertex
 *	@discussion
 *		Macro to aid in iterating over vertices of a component of a Mesh object.
 *	@param		_c						The component.
 *	@param		_v						The vertex.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachComponentVertex(_c, _v, _i)                \
        for ((_v) = Q3Mesh_FirstComponentVertex((_c), (_i));\
             (_v) != NULL;                                  \
             (_v) = Q3Mesh_NextComponentVertex((_i)))

/*!
 *	@function		Q3ForEachComponentEdge
 *	@discussion
 *		Macro to aid in iterating over edges of a component of a Mesh object.
 *	@param		_c						The component.
 *	@param		_e						The edge.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachComponentEdge(_c, _e, _i)                  \
        for ((_e) = Q3Mesh_FirstComponentEdge((_c), (_i));  \
             (_e) != NULL;                                  \
             (_e) = Q3Mesh_NextComponentEdge((_i)))

/*!
 *	@function		Q3ForEachMeshVertex
 *	@discussion
 *		Macro to aid in iterating over vertices of a Mesh object.
 *	@param		_m						The mesh.
 *	@param		_v						The vertex.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachMeshVertex(_m, _v, _i)                     \
        for ((_v) = Q3Mesh_FirstMeshVertex((_m), (_i));     \
             (_v) != NULL;                                  \
             (_v) = Q3Mesh_NextMeshVertex((_i)))

/*!
 *	@function		Q3ForEachMeshFace
 *	@discussion
 *		Macro to aid in iterating over faces of a Mesh object.
 *	@param		_m						The mesh.
 *	@param		_f						The face.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachMeshFace(_m, _f, _i)                       \
        for ((_f) = Q3Mesh_FirstMeshFace((_m), (_i));       \
             (_f) != NULL;                                  \
             (_f) = Q3Mesh_NextMeshFace((_i)))

/*!
 *	@function		Q3ForEachMeshEdge
 *	@discussion
 *		Macro to aid in iterating over edges of a Mesh object.
 *	@param		_m						The mesh.
 *	@param		_e						The edge.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachMeshEdge(_m, _e, _i)                       \
        for ((_e) = Q3Mesh_FirstMeshEdge((_m), (_i));       \
             (_e) != NULL;                                  \
             (_e) = Q3Mesh_NextMeshEdge((_i)))

/*!
 *	@function		Q3ForEachVertexEdge
 *	@discussion
 *		Macro to aid in iterating over edges incident to a vertex of a Mesh object.
 *	@param		_v						The vertex.
 *	@param		_e						The edge.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachVertexEdge(_v, _e, _i)                     \
        for ((_e) = Q3Mesh_FirstVertexEdge((_v), (_i));     \
             (_e) != NULL;                                  \
             (_e) = Q3Mesh_NextVertexEdge((_i)))


/*!
 *	@function		Q3ForEachVertexVertex
 *	@discussion
 *		Macro to aid in iterating over vertices adjacent to a vertex of a Mesh object.
 *	@param		_v						The vertex.
 *	@param		_n						An adjacent vertex.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachVertexVertex(_v, _n, _i)                   \
        for ((_n) = Q3Mesh_FirstVertexVertex((_v), (_i));   \
             (_n) != NULL;                                  \
             (_n) = Q3Mesh_NextVertexVertex((_i)))

/*!
 *	@function		Q3ForEachVertexFace
 *	@discussion
 *		Macro to aid in iterating over faces incident to a vertex of a Mesh object.
 *	@param		_v						The vertex.
 *	@param		_f						An incident face.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachVertexFace(_v, _f, _i)                     \
        for ((_f) = Q3Mesh_FirstVertexFace((_v), (_i));     \
             (_f) != NULL;                                  \
             (_f) = Q3Mesh_NextVertexFace((_i)))

/*!
 *	@function		Q3ForEachFaceEdge
 *	@discussion
 *		Macro to aid in iterating over edges incident to a face of a Mesh object.
 *	@param		_f						The face.
 *	@param		_e						An incident edge.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachFaceEdge(_f, _e, _i)                       \
        for ((_e) = Q3Mesh_FirstFaceEdge((_f), (_i));       \
             (_e) != NULL;                                  \
             (_e) = Q3Mesh_NextFaceEdge((_i)))

/*!
 *	@function		Q3ForEachFaceVertex
 *	@discussion
 *		Macro to aid in iterating over vertices incident to a face of a Mesh object.
 *	@param		_f						The face.
 *	@param		_v						An incident vertex.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachFaceVertex(_f, _v, _i)                     \
        for ((_v) = Q3Mesh_FirstFaceVertex((_f), (_i));     \
             (_v) != NULL;                                  \
             (_v) = Q3Mesh_NextFaceVertex((_i)))
    
/*!
 *	@function		Q3ForEachFaceFace
 *	@discussion
 *		Macro to aid in iterating over faces adjacent to a face of a Mesh object.
 *	@param		_f						The face.
 *	@param		_n						An incident face.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachFaceFace(_f, _n, _i)                       \
        for ((_n) = Q3Mesh_FirstFaceFace((_f), (_i));       \
             (_n) != NULL;                                  \
             (_n) = Q3Mesh_NextFaceFace((_i)))
          
/*!
 *	@function		Q3ForEachFaceContour
 *	@discussion
 *		Macro to aid in iterating over contours of a face of a Mesh object.
 *	@param		_f						The face.
 *	@param		_h						A contour of the face.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachFaceContour(_f, _h, _i)                    \
        for ((_h) = Q3Mesh_FirstFaceContour((_f), (_i));    \
             (_h) != NULL;                                  \
             (_h) = Q3Mesh_NextFaceContour((_i)))

/*!
 *	@function		Q3ForEachContourEdge
 *	@discussion
 *		Macro to aid in iterating over edges of a contour of a Mesh object.
 *	@param		_h						The contour.
 *	@param		_e						An edge of the contour.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachContourEdge(_h, _e, _i)                    \
        for ((_e) = Q3Mesh_FirstContourEdge((_h), (_i));    \
             (_e) != NULL;                                  \
             (_e) = Q3Mesh_NextContourEdge((_i)))

/*!
 *	@function		Q3ForEachContourVertex
 *	@discussion
 *		Macro to aid in iterating over vertices of a contour of a Mesh object.
 *	@param		_h						The contour.
 *	@param		_v						A vertex of the contour.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachContourVertex(_h, _v, _i)                  \
        for ((_v) = Q3Mesh_FirstContourVertex((_h), (_i));  \
             (_v) != NULL;                                  \
             (_v) = Q3Mesh_NextContourVertex((_i)))

/*!
 *	@function		Q3ForEachContourFace
 *	@discussion
 *		Macro to aid in iterating over edges of a contour of a Mesh object.
 *	@param		_h						The contour.
 *	@param		_v						An edge of the contour.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
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
 *      is a reference to. If 'geometry' is invalid, <code>kQ3ObjectTypeInvalid</code> will be
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
 *      This function fills the <code>TQ3AttributeSet*</code> parameter out with the
 *		attribute set of the passed in <code>TQ3GeometryObject</code> reference.
 *
 *		The reference count of the attribute set is incremented.
 *
 *		To decrease the reference count when finished with the attribute set, use
 *		<code>Q3ObjectDispose()</code>.
 *
 *  @param geometry         A reference to a geometry object.
 *  @param attributeSet     Receives a reference to the attribute set of the geometry.
 *  @result                 Success or failure of the operation.
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
 *      Applies the attribute set given by the <code>TQ3AttributeSet</code> parameter onto
 *		the <code>TQ3GeometryObject</code> reference passed in.
 *
 *  @param geometry         A reference to a geometry object.
 *  @param attributeSet     The attribute set to apply to the object.
 *  @result                 Success or failure of the operation.
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
 *      Submits the passed in <code>TQ3GeometryObject</code> to the supplied view for
 *      drawing, picking, bounding, or writing.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param geometry         A reference to a (retained) geometry object.
 *  @param view             The view to submit the geometric object to.
 *  @result                 Success or failure of the operation.
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
 *  @result                 A reference to the new geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Box_New (
    const TQ3BoxData              *boxData
);



/*!
 *  @function
 *      Q3Box_Submit
 *  @discussion
 *		Submits a box for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param boxData			A pointer to a <code>TQ3BoxData</code> structure specifying the box
 *							to be drawn.
 *  @param view				The view to submit the box to.
 *  @result					Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *		Memory is allocated for the 'boxData' parameter, and <code>Q3Box_EmptyData</code>
 *		must be called to dispose of this memory.
 *
 *  @param box              A reference to a box geometry object.
 *  @param boxData          Receives the box object's description.
 *  @result                 Success or failure of the operation.
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
 *      Releases the memory allocated by a prior call to <code>Q3Box_GetData</code>.
 *
 *  @param boxData          A pointer to the box data allocated by <code>Q3Box_GetData()</code>.
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @param origin           Receives the origin of the box object.
 *  @result                 Success or failure of the operation.
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
 *  @param orientation      Receives the orientation vector of the box.
 *  @result                 Success or failure of the operation.
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
 *  @param majorAxis        Receives the major axis of the box.
 *  @result                 Success or failure of the operation.
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
 *  @param minorAxis        Receives the minor axis of the box.
 *  @result                 Success or failure of the operation.
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
 *      The <code>TQ3AttributeSet*</code> parameter is filled with the attribute set
 *      of the face indexed with 'faceIndex' of the box geometry object.
 *		faceIndex must be between 0 and 5 (inclusive).
 *
 *		The reference count of the returned <code>TQ3AttributeSet</code> is incremented.
 *
 *  @param box              A reference to a box geometry object.
 *  @param faceIndex        The index of the face of the box.
 *  @param faceAttributeSet Receives a reference to the attribute set of the box face, or
 *							NULL if the face does not have an attribute set.
 *  @result                 Success or failure of the operation.
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
 *      This function applies the <code>TQ3AttributeSet*</code> parameter to the attribute set
 *      of the face indexed with <code>faceIndex</code> of the box geometry object.
 *		<code>faceIndex</code> must be between 0 and 5 (inclusive).
 *
 *  @param box              A reference to a box geometry object.
 *  @param faceIndex        The index of the face of the box.
 *  @param faceAttributeSet An attribute set to be applied.
 *  @result                 Success or failure of the operation.
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
 *      Create a cone geometry object.
 *
 *  @param coneData         Pointer to data describing the cone.
 *  @result                 The new cone object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Cone_New (
    const TQ3ConeData             *coneData
);



/*!
 *  @function
 *      Q3Cone_Submit
 *  @discussion
 *      Submits a cone in immediate mode for rendering, bounding, picking, or writing.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param coneData         Pointer to data describing a cone.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a cone object by supplying new data.
 *
 *  @param cone             The cone object.
 *  @param coneData         Pointer to data describing a cone.
 *  @result                 Success or failure of the operation.
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
 *      Get data describing a cone object.
 *
 *		This may allocate memory, which should be freed using <code>Q3Cone_EmptyData</code>.
 *
 *  @param cone             The cone object.
 *  @param coneData         Receives data describing the cone.
 *  @result                 Success or failure of the operation.
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
 *      Change the origin of a cone object.
 *
 *  @param cone             The cone object.
 *  @param origin           The new origin.
 *  @result                 Success or failure of the operation.
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
 *      Change the orientation vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param orientation      The new orientation vector.
 *  @result                 Success or failure of the operation.
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
 *      Change the major radius vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param majorRadius      New major radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Change the minor radius vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param minorRadius      New minor radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Get the origin of a cone object.
 *
 *  @param cone             The cone object.
 *  @param origin           Receives the origin.
 *  @result                 Success or failure of the operation.
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
 *      Get the orientation vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param orientation      Receives the orientation vector.
 *  @result                 Success or failure of the operation.
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
 *      Get the major radius vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param majorRadius      Receives the major radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Get the minor radius vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param minorRadius      Receives the minor radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Set the end cap flags of a cone object.
 *
 *      Currently, this simply determines whether the cone will have a bottom cap.
 *
 *  @param cone             The cone object.
 *  @param caps             End cap value (<code>kQ3EndCapMaskBottom</code> or
 *							<code>kQ3EndCapNone</code>).
 *  @result                 Success or failure of the operation.
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
 *      Get the end cap flags of a cone object.
 *
 *  @param cone             The cone object.
 *  @param caps             Receives the end cap flags.  See <code>TQ3EndCapMasks</code>,
 *							defined in Quesa.h, for values.
 *  @result                 Success or failure of the operation.
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
 *      Change the bottom attribute set of a cone object.
 *
 *      Naturally, this only makes sense if the cone has a bottom end cap.
 *
 *  @param cone             	The cone object.
 *  @param bottomAttributeSet	New bottom attribute set.  May be NULL.
 *  @result                 	Success or failure of the operation.
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
 *      Get the attribute set for the bottom cap of a cone object.
 *
 *  @param cone             	The cone object.
 *  @param bottomAttributeSet	Receives a reference to the bottom attribute set, or NULL.
 *  @result                 	Success or failure of the operation.
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
 *      Set the attribute set for the face (as opposed to the bottom cap) of a cone.
 *
 *  @param cone             The cone object.
 *  @param faceAttributeSet New face attribute set.  May be NULL.
 *  @result                 Success or failure of the operation.
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
 *      Get the attribute set for the face (as opposed to the bottom cap) of a cone.
 *
 *  @param cone             The cone object.
 *  @param faceAttributeSet Receives the attribute set for the face, or NULL.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3Cone_GetData</code>.
 *
 *  @param coneData         Data describing a cone, previously obtained with
 *							<code>Q3Cone_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cone_EmptyData (
    TQ3ConeData                   *coneData
);



/*!
 *  @function
 *      Q3Cylinder_New
 *  @discussion
 *      Create a new Cylinder object.
 *
 *  @param cylinderData     Data describing a cylinder.
 *  @result                 Reference to a new Cylinder geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Cylinder_New (
    const TQ3CylinderData         *cylinderData
);



/*!
 *  @function
 *      Q3Cylinder_Submit
 *  @discussion
 *		Submits a cylinder for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param cylinderData     Data describing a cylinder.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a cylinder object by supplying all new data.
 *
 *  @param cylinder         A cylinder object.
 *  @param cylinderData     Data describing a cylinder.
 *  @result                 Success or failure of the operation.
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
 *      Get data describing a cylinder.
 *
 *      This operation may allocate memory, which should be freed using
 *		<code>Q3Cylinder_EmptyData</code>.
 *
 *  @param cylinder         A cylinder object.
 *  @param cylinderData     Receives data describing the cylinder.
 *  @result                 Success or failure of the operation.
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
 *      Change the origin of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param origin           New point of origin.
 *  @result                 Success or failure of the operation.
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
 *      Change the orientation vector of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param orientation      New orientation vector for the cylinder.
 *  @result                 Success or failure of the operation.
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
 *      Change the major radius vector of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param majorRadius      New major radius vector for the cylinder.
 *  @result                 Success or failure of the operation.
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
 *      Change the minor radius vector of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param minorRadius      New minor radius vector for the cylinder.
 *  @result                 Success or failure of the operation.
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
 *      Get the origin of a cylinder.
 *
 *  @param cylinder         A cylinder object.
 *  @param origin           Receives the origin of the cylinder.
 *  @result                 Success or failure of the operation.
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
 *      Get the orientation vector of a cylinder.
 *
 *  @param cylinder         A cylinder object.
 *  @param orientation      Receives the orientation vector of the cylinder.
 *  @result                 Success or failure of the operation.
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
 *      Get the major radius vector of a cylinder.
 *
 *  @param cylinder         A cylinder object.
 *  @param majorRadius      Receives the major radius vector of the cylinder.
 *  @result                 Success or failure of the operation.
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
 *      Get the minor radius vector of a cylinder.
 *
 *  @param cylinder         A cylinder object.
 *  @param minorRadius      Receives the minor radius vector of the cylinder.
 *  @result                 Success or failure of the operation.
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
 *      Set the cap flags of a cylinder object.  This determines whether the
 *		cylinder is closed off at one end or the other.  The end containing the
 *		origin is considered the bottom.
 *
 *  @param cylinder         A cylinder object.
 *  @param caps             The end cap flags.  See <code>TQ3EndCapMasks</code>,
 *							defined in Quesa.h, for values.
 *  @result                 Success or failure of the operation.
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
 *      Get the cap flags of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param caps             Receives the end cap flags.  See <code>TQ3EndCapMasks</code>,
 *							defined in Quesa.h, for values.
 *  @result                 Success or failure of the operation.
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
 *      Set the attribute set for the top cap of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param topAttributeSet  New attribute set for the top cap.
 *  @result                 Success or failure of the operation.
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
 *      Get the attribute set for the top cap of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param topAttributeSet  Receives the attribute set for the top cap, or NULL.
 *  @result                 Success or failure of the operation.
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
 *      Set the attribute set for the bottom cap of a cylinder object.
 *
 *  @param cylinder         	A cylinder object.
 *  @param bottomAttributeSet	New attribute set for the bottom cap.
 *  @result                		Success or failure of the operation.
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
 *      Get the attribute set for the bottom cap of a cylinder object.
 *
 *  @param cylinder         	A cylinder object.
 *  @param bottomAttributeSet	Receives the attribute set for the bottom cap, or NULL.
 *  @result						Success or failure of the operation.
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
 *      Set the attribute set for the face of a cylinder object (i.e., neither end).
 *
 *  @param cylinder         A cylinder object.
 *  @param faceAttributeSet New attribute set for the face of the cylinder.
 *  @result                 Success or failure of the operation.
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
 *      Get the attribute set for the face of a cylinder object (i.e., neither end).
 *
 *  @param cylinder         A cylinder object.
 *  @param faceAttributeSet Receives the attribute set for the face, or NULL.
 *  @result                 Success or failure of the operation.
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
 *      Free memory allocated by <code>Q3Cylinder_GetData</code>.
 *
 *  @param cylinderData     Cylinder data previously obtained using <code>Q3Cylinder_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3Cylinder_EmptyData (
    TQ3CylinderData               *cylinderData
);



/*!
 *  @function
 *      Q3Disk_New
 *  @discussion
 *      Create a new disk object.
 *
 *  @param diskData         Data describing a disk object.
 *  @result                 Reference to a new Disk geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Disk_New (
    const TQ3DiskData             *diskData
);



/*!
 *  @function
 *      Q3Disk_Submit
 *  @discussion
 *		Submits a disk for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param diskData         Data describing a disk object.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a disk object by supplying a whole new set of data.
 *
 *  @param disk             A disk geometry object.
 *  @param diskData         Data describing a disk object.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a Disk object.
 *
 *		This may allocate memory, which should be freed using <code>Q3Disk_EmptyData</code>.
 *
 *  @param disk             A disk geometry object.
 *  @param diskData         Data describing a disk object.
 *  @result                 Success or failure of the operation.
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
 *      Change the origin of a Disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param origin           New origin for the disk.
 *  @result                 Success or failure of the operation.
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
 *      Change the major radius vector of a disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param majorRadius      New major radius vector for the disk object.
 *  @result                 Success or failure of the operation.
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
 *      Change the minor radius vector of a disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param minorRadius      New minor radius vector for the disk object.
 *  @result                 Success or failure of the operation.
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
 *      Get the origin of a disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param origin           Receives the origin of the disk.
 *  @result                 Success or failure of the operation.
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
 *      Get the major radius vector of a disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param majorRadius      Receives the major radius vector of the disk.
 *  @result                 Success or failure of the operation.
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
 *      Get the minor radius vector of a disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param minorRadius      Receives the minor radius vector of the disk.
 *  @result                 Success or failure of the operation.
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
 *      Free memory allocated by <code>Q3Disk_GetData</code>.
 *
 *  @param diskData         Data describing a disk object, previously obtained by
 *							<code>Q3Disk_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3Disk_EmptyData (
    TQ3DiskData                   *diskData
);



/*!
 *  @function
 *      Q3Ellipse_New
 *  @discussion
 *      Create a new ellipse geometry object.
 *
 *  @param ellipseData      Data describing an ellipse.
 *  @result                 Reference to a new Ellipse geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Ellipse_New (
    const TQ3EllipseData          *ellipseData
);



/*!
 *  @function
 *      Q3Ellipse_Submit
 *  @discussion
 *		Submits an ellipse for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param ellipseData      Data describing an ellipse.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify an ellipse object by supplying a full new set of data.
 *
 *  @param ellipse          An ellipse object.
 *  @param ellipseData      Data describing an ellipse.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of an Ellipse object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Ellipse_EmptyData</code>.
 *
 *  @param ellipse          An ellipse object.
 *  @param ellipseData      Receives data describing the ellipse object.
 *  @result                 Success or failure of the operation.
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
 *      Change the origin of an ellipse object.
 *
 *  @param ellipse          The ellipse object.
 *  @param origin           The new origin.
 *  @result                 Success or failure of the operation.
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
 *      Change the major radius vector of an Ellipse object.
 *
 *  @param ellipse          The Ellipse object.
 *  @param majorRadius      New major radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Change the minor radius vector of an Ellipse object.
 *
 *  @param ellipse          The Ellipse object.
 *  @param minorRadius      New minor radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Get the origin of an Ellipse object.
 *
 *  @param ellipse          The Ellipse object.
 *  @param origin           Receives the origin.
 *  @result                 Success or failure of the operation.
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
 *      Get the major radius vector of an Ellipse object.
 *
 *  @param ellipse          The Ellipse object.
 *  @param majorRadius      Receives the major radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Get the minor radius vector of an Ellipse object.
 *
 *  @param ellipse          The Ellipse object.
 *  @param minorRadius      Receives the minor radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3Ellipse_GetData</code>.
 *
 *  @param ellipseData      Data describing an ellipse, previously obtained with
 *							<code>Q3Ellipse_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipse_EmptyData (
    TQ3EllipseData                *ellipseData
);



/*!
 *  @function
 *      Q3Ellipsoid_New
 *  @discussion
 *      Create a new ellipsoid geometry object.
 *
 *  @param ellipsoidData    Data describing an ellipsoid.
 *  @result                 Reference to a new Ellipsoid geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Ellipsoid_New (
    const TQ3EllipsoidData        *ellipsoidData
);



/*!
 *  @function
 *      Q3Ellipsoid_Submit
 *  @discussion
 *		Submits an ellipsoid for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param ellipsoidData    Data describing an ellipsoid.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify an ellipsoid object by supplying a full new set of data.
 *
 *  @param ellipsoid        An ellipsoid object.
 *  @param ellipsoidData    Data describing an ellipsoid.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of an Ellipse object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Ellipsoid_EmptyData</code>.
 *
 *  @param ellipsoid        An ellipsoid object.
 *  @param ellipsoidData    Receives data describing the ellipsoid object.
 *  @result                 Success or failure of the operation.
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
 *      Change the origin of an ellipsoid object.
 *
 *  @param ellipsoid        The ellipsoid object.
 *  @param origin           The new origin.
 *  @result                 Success or failure of the operation.
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
 *      Change the orientation vector of an Ellipsoid object.
 *
 *  @param ellipsoid        An Ellipsoid object.
 *  @param orientation      New orientation vector for the Ellipsoid.
 *  @result                 Success or failure of the operation.
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
 *      Change the major radius vector of an Ellipsoid object.
 *
 *  @param ellipsoid        The Ellipsoid object.
 *  @param majorRadius      New major radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Change the minor radius vector of an Ellipsoid object.
 *
 *  @param ellipsoid        The Ellipsoid object.
 *  @param minorRadius      New minor radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Get the origin of an Ellipsoid object.
 *
 *  @param ellipsoid        The Ellipsoid object.
 *  @param origin           Receives the origin.
 *  @result                 Success or failure of the operation.
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
 *      Get the orientation vector of an Ellipsoid.
 *
 *  @param ellipsoid        An Ellipsoid object.
 *  @param orientation      Receives the orientation vector of the Ellipsoid.
 *  @result                 Success or failure of the operation.
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
 *      Get the major radius vector of an Ellipsoid object.
 *
 *  @param ellipsoid        The Ellipsoid object.
 *  @param majorRadius      Receives the major radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Get the minor radius vector of an Ellipsoid object.
 *
 *  @param ellipsoid        The Ellipsoid object.
 *  @param minorRadius      Receives the minor radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3Ellipsoid_GetData</code>.
 *
 *  @param ellipsoidData    Data describing an Ellipsoid, previously obtained with
 *							<code>Q3Ellipsoid_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_EmptyData (
    TQ3EllipsoidData              *ellipsoidData
);



/*!
 *  @function
 *      Q3GeneralPolygon_New
 *  @discussion
 *      Create a new general polygon geometry object.
 *
 *  @param generalPolygonData	Data describing a general polygon.
 *  @result						Reference to a new General Polygon geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3GeneralPolygon_New (
    const TQ3GeneralPolygonData   *generalPolygonData
);



/*!
 *  @function
 *      Q3GeneralPolygon_Submit
 *  @discussion
 *		Submits a general polygon for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param generalPolygonData	Data describing a general polygon.
 *  @param view					A view object.
 *  @result						Success or failure of the operation.
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
 *      Modify a general polygon object by supplying a full new set of data.
 *
 *  @param generalPolygon   	A general polygon object.
 *  @param generalPolygonData	Data describing a general polygon object.
 *  @result						Success or failure of the operation.
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
 *      Get the data of a General Polygon object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3GeneralPolygon_EmptyData</code>.
 *
 *  @param polygon          	A general polygon object.
 *  @param generalPolygonData	Receives data describing the general polygon object.
 *  @result						Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3GeneralPolygon_GetData</code>.
 *
 *  @param generalPolygonData	Data describing a General Polygon, previously obtained with
 *								<code>Q3GeneralPolygon_GetData</code>.
 *  @result						Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_EmptyData (
    TQ3GeneralPolygonData         *generalPolygonData
);



/*!
 *  @function
 *      Q3GeneralPolygon_GetVertexPosition
 *  @discussion
 *      Get the 3D position of a vertex in a general polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param contourIndex		A 0-based index into the array of contours.
 *  @param pointIndex       A 0-based index into the array of vertices of a contour.
 *  @param position         Receives the position of the vertex.
 *  @result                 Success or failure of the operation.
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
 *      Set the 3D position of a vertex in a general polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param contourIndex		A 0-based index into the array of contours.
 *  @param pointIndex       A 0-based index into the array of vertices of a contour.
 *  @param position         New position for the vertex.
 *  @result                 Success or failure of the operation.
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
 *      Get the attribute set of a vertex of a general polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param contourIndex		A 0-based index into the array of contours.
 *  @param pointIndex       A 0-based index into the array of vertices of a contour.
 *  @param attributeSet     Receives a new reference to the vertex attribute set, or NULL.
 *  @result                 Success or failure of the operation.
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
 *      Assign an attribute set to a vertex of a general polygon.
 *
 *      The function increments the reference count of the attribute set.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param contourIndex		A 0-based index into the array of contours.
 *  @param pointIndex       A 0-based index into the array of vertices of a contour.
 *  @param attributeSet     Attribute set for the vertex.
 *  @result                 Success or failure of the operation.
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
 *      Set the shape hint of a general polygon.
 *
 *      The hint may allow a renderer to optimize drawing the polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param shapeHint        The shape hint.
 *  @result                 Success or failure of the operation.
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
 *      Get the shape hint of a general polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param shapeHint        Receives the shape hint.
 *  @result                 Success or failure of the operation.
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
 *      Create a new line geometry object.
 *
 *  @param lineData         Data describing a line.
 *  @result                 Reference to a new Line geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Line_New (
    const TQ3LineData             *lineData
);



/*!
 *  @function
 *      Q3Line_Submit
 *  @discussion
 *		Submits a line for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param lineData         Data describing a line.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a Line object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Line_EmptyData</code>.
 *
 *  @param line             A Line object.
 *  @param lineData         Receives data describing the Line object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a line object by supplying a full new set of data.
 *
 *  @param line             A line object.
 *  @param lineData         Data describing a line.
 *  @result                 Success or failure of the operation.
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
 *      Get the position of one of the ends of a line.
 *
 *  @param line             A line object.
 *  @param index            An index, 0 or 1, specifying an end of the line.
 *  @param position         Receives the position of the vertex.
 *  @result                 Success or failure of the operation.
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
 *      Change the position of one of the ends of a line object.
 *
 *  @param line             A line object.
 *  @param index            An index, 0 or 1, specifying an end of the line.
 *  @param position         New position of the vertex.
 *  @result                 Success or failure of the operation.
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
 *      Get the attribute set of one of the ends of a line object.
 *
 *  @param line             A line object.
 *  @param index            An index, 0 or 1, specifying an end of the line.
 *  @param attributeSet     Receives a new reference to the attribute set, or NULL.
 *  @result                 Success or failure of the operation.
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
 *      Assign an attribute set to one of the ends of a line.
 *
 *      Any previous attribute set on the vertex is disposed.
 *		The new attribute set has its reference count incremented.
 *
 *  @param line             A line object.
 *  @param index            An index, 0 or 1, specifying an end of the line.
 *  @param attributeSet     The new attribute set.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3Line_GetData</code>.
 *
 *  @param lineData         Data describing a Line, previously obtained with
 *							<code>Q3Line_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3Line_EmptyData (
    TQ3LineData                   *lineData
);



/*!
 *  @function
 *      Q3Marker_New
 *  @discussion
 *      Create a new marker geometry object.
 *
 *  @param markerData       Data describing a marker.
 *  @result                 Reference to a new Marker geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Marker_New (
    const TQ3MarkerData           *markerData
);



/*!
 *  @function
 *      Q3Marker_Submit
 *  @discussion
 *		Submits a marker for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param markerData       Data describing a marker.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a marker object by supplying a full new set of data.
 *
 *  @param geometry         A marker object.
 *  @param markerData       Data describing a marker.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a Marker object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Marker_EmptyData</code>.
 *
 *  @param geometry         A Marker object.
 *  @param markerData       Receives data describing the Marker object.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3Marker_GetData</code>.
 *
 *  @param markerData       Data describing a Marker, previously obtained with
 *							<code>Q3Marker_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3Marker_EmptyData (
    TQ3MarkerData                 *markerData
);



/*!
 *  @function
 *      Q3Marker_GetPosition
 *  @discussion
 *      Get the position of a Marker object.
 *
 *      The position corresponds to the <code>location</code> field of the
 *		<code>TQ3MarkerData</code> structure.
 *
 *  @param marker			A marker object.
 *  @param location         Receives the location of the marker object.
 *  @result                 Success or failure of the operation.
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
 *      Set the position of a Marker object.
 *
 *      The position corresponds to the <code>location</code> field of the
 *		<code>TQ3MarkerData</code> structure.
 *
 *  @param marker			A marker object.
 *  @param location         New location of the marker object.
 *  @result                 Success or failure of the operation.
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
 *      Get the horizontal offset, in screen pixels, from the marker location to the
 *		upper left corner of the marker bitmap.
 *
 *  @param marker			A marker object.
 *  @param xOffset          Receives the horizontal offset in pixels.
 *  @result                 Success or failure of the operation.
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
 *      Set the horizontal offset, in screen pixels, from the marker location to the
 *		upper left corner of the marker bitmap.
 *
 *  @param marker			A marker object.
 *  @param xOffset          New horizontal offset in pixels.
 *  @result                 Success or failure of the operation.
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
 *      Get the vertical offset, in screen pixels, from the marker location to the
 *		upper left corner of the marker bitmap.
 *
 *  @param marker			A marker object.
 *  @param yOffset          Receives the vertical offset in pixels.
 *  @result                 Success or failure of the operation.
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
 *      Set the vertical offset, in screen pixels, from the marker location to the
 *		upper left corner of the marker bitmap.
 *
 *  @param marker			A marker object.
 *  @param yOffset          New vertical offset in pixels.
 *  @result                 Success or failure of the operation.
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
 *      Get the bitmap of a Marker object.
 *
 *      This returns a copy of the bitmap of the marker.  When you are done
 *      with it, use <code>Q3Bitmap_Empty</code> to release the memory.
 *
 *  @param marker			A marker object.
 *  @param bitmap           Receives a copy of the bitmap of the marker.
 *  @result                 Success or failure of the operation.
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
 *      Change the bitmap of a Marker object.
 *
 *      This makes a copy of the bitmap data, so you can dispose of the
 *      original.
 *
 *  @param marker			A marker object.
 *  @param bitmap           New bitmap to be used by the marker.
 *  @result                 Success or failure of the operation.
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
 *      Create a new empty Mesh geometry object.
 *
 *  @result                 Reference to a new Mesh geometry object, or NULL on failure.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *      Create a new NURB curve geometry object.
 *
 *  @param curveData        Data describing a NURB curve.
 *  @result                 Reference to a new NURB curve geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3NURBCurve_New (
    const TQ3NURBCurveData        *curveData
);



/*!
 *  @function
 *      Q3NURBCurve_Submit
 *  @discussion
 *		Submits a NURB curve for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param curveData        Data describing a NURB curve.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a NURB curve object by supplying a full new set of data.
 *
 *  @param curve            A NURB curve object.
 *  @param nurbCurveData    Data describing a NURB curve object.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a NURB Curve object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3NURBCurve_EmptyData</code>.
 *
 *  @param curve            A NURB Curve object.
 *  @param nurbCurveData    Receives data describing the NURB Curve object.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3NURBCurve_GetData</code>.
 *
 *  @param nurbCurveData    Data describing a NURB Curve, previously obtained with
 *							<code>Q3NURBCurve_GetData</code>.
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *      Create a new NURB patch geometry object.
 *
 *  @param nurbPatchData    Data describing a NURB patch.
 *  @result                 Reference to a new NURB patch geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3NURBPatch_New (
    const TQ3NURBPatchData        *nurbPatchData
);



/*!
 *  @function
 *      Q3NURBPatch_Submit
 *  @discussion
 *		Submits a NURB patch for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param nurbPatchData    Data describing a NURB patch.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a NURB patch object by supplying a full new set of data.
 *
 *  @param nurbPatch        A NURB patch object.
 *  @param nurbPatchData    Data describing a NURB patch object.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a NURB Patch object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3NURBPatch_EmptyData</code>.
 *
 *  @param nurbPatch        A NURB Patch object.
 *  @param nurbPatchData    Receives data describing the NURB Patch object.
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3NURBPatch_GetData</code>.
 *
 *  @param nurbPatchData    Data describing a NURB Patch, previously obtained with
 *							<code>Q3NURBPatch_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_EmptyData (
    TQ3NURBPatchData              *nurbPatchData
);



/*!
 *  @function
 *      Q3PixmapMarker_New
 *  @discussion
 *      Create a new pixmap marker geometry object.
 *
 *  @param pixmapMarkerData Data describing a pixmap marker.
 *  @result                 Reference to a new pixmap marker geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3PixmapMarker_New (
    const TQ3PixmapMarkerData     *pixmapMarkerData
);



/*!
 *  @function
 *      Q3PixmapMarker_Submit
 *  @discussion
 *		Submits a pixmap marker for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param pixmapMarkerData Data describing a pixmap marker.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a pixmap marker object by supplying a full new set of data.
 *
 *  @param geometry         A pixmap marker object.
 *  @param pixmapMarkerData Data describing a pixmap marker object.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a Pixmap Marker object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3PixmapMarker_EmptyData</code>.
 *
 *  @param geometry         A Pixmap Marker object.
 *  @param pixmapMarkerData Receives data describing the Pixmap Marker object.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3PixmapMarker_GetData</code>.
 *
 *  @param pixmapMarkerData Data describing a Pixmap Marker, previously obtained with
 *							<code>Q3PixmapMarker_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_EmptyData (
    TQ3PixmapMarkerData           *pixmapMarkerData
);



/*!
 *  @function
 *      Q3PixmapMarker_GetPosition
 *  @discussion
 *      Get the position of the pixmap marker in world coordinates.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param position         Receives the position of the pixmap marker.
 *  @result                 Success or failure of the operation.
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
 *      Change the position of a pixmap marker object.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param position         New position of the pixmap marker in world coordinates.
 *  @result                 Success or failure of the operation.
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
 *      Get the horizontal offset, in screen pixels, from the marker position to
 *		the upper left corner of the marker image.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param xOffset          Receives the horizontal offset of the marker in pixels.
 *  @result                 Success or failure of the operation.
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
 *      Set the horizontal offset, in screen pixels, from the marker position to
 *		the upper left corner of the marker image.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param xOffset          The horizontal offset of the marker in pixels.
 *  @result                 Success or failure of the operation.
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
 *      Get the vertical offset, in screen pixels, from the marker position to
 *		the upper left corner of the marker image.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param yOffset          Receives the vertical offset of the marker in pixels.
 *  @result                 Success or failure of the operation.
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
 *      Set the vertical offset, in screen pixels, from the marker position to
 *		the upper left corner of the marker image.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param yOffset          The vertical offset of the marker in pixels.
 *  @result                 Success or failure of the operation.
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
 *      Get a copy of the pixmap used within a pixmap marker, including
 *		a new reference to the image storage.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param pixmap           Receives the pixmap structure of the marker.
 *  @result                 Success or failure of the operation.
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
 *      Change the pixmap used by a pixmap marker.  The function makes a copy of the data,
 *		including incrementing the reference count of the image storage.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param pixmap           New pixmap to be used by the marker.
 *  @result                 Success or failure of the operation.
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
 *      Create a new point geometry object.
 *
 *  @param pointData        Data describing a point.
 *  @result                 Reference to a new Point geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Point_New (
    const TQ3PointData            *pointData
);



/*!
 *  @function
 *      Q3Point_Submit
 *  @discussion
 *		Submits a point for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param pointData        Data describing a point.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a Point object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Point_EmptyData</code>.
 *
 *  @param point            A Point object.
 *  @param pointData        Receives data describing the Point object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a point object by supplying a full new set of data.
 *
 *  @param point            A point object
 *  @param pointData        Data describing a point object.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3Point_GetData</code>.
 *
 *  @param pointData        Data describing a Point, previously obtained with
 *							<code>Q3Point_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point_EmptyData (
    TQ3PointData                  *pointData
);



/*!
 *  @function
 *      Q3Point_SetPosition
 *  @discussion
 *      Change the position of a point object.
 *
 *  @param point            A point object.
 *  @param position         New position for the point.
 *  @result                 Success or failure of the operation.
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
 *      Get the position of a point object.
 *
 *  @param point            A point object.
 *  @param position         Receives the position of the point.
 *  @result                 Success or failure of the operation.
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
 *      Create a new polygon geometry object.
 *
 *  @param polygonData      Data describing a polygon.
 *  @result                 Reference to a new Polygon geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Polygon_New (
    const TQ3PolygonData          *polygonData
);



/*!
 *  @function
 *      Q3Polygon_Submit
 *  @discussion
 *		Submits a polygon for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param polygonData      Data describing a polygon.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a polygon object by supplying a full new set of data.
 *
 *  @param polygon          A polygon object.
 *  @param polygonData      Data describing a polygon.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a Polygon object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Polygon_EmptyData</code>.
 *
 *  @param polygon          A Polygon object.
 *  @param polygonData      Receives data describing the Polygon object.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3Polygon_GetData</code>.
 *
 *  @param polygonData      Data describing a Polygon, previously obtained with
 *							<code>Q3Polygon_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3Polygon_EmptyData (
    TQ3PolygonData                *polygonData
);



/*!
 *  @function
 *      Q3Polygon_GetVertexPosition
 *  @discussion
 *      Get the 3D position of a vertex of a polygon object.
 *
 *  @param polygon          A polygon object.
 *  @param index            A 0-based index into the array of vertices of the polygon.
 *  @param point            Receives the position of the vertex.
 *  @result                 Success or failure of the operation.
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
 *      Change the 3D position of a vertex of a polygon object.
 *
 *  @param polygon          A polygon object.
 *  @param index            A 0-based index into the array of vertices of the polygon.
 *  @param point            New position of the vertex.
 *  @result                 Success or failure of the operation.
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
 *      Get the attribute set of a vertex in a polygon object.
 *
 *  @param polygon          A polygon object.
 *  @param index            A 0-based index into the array of vertices of the polygon.
 *  @param attributeSet     Receives a new reference to the attribute set, or NULL.
 *  @result                 Success or failure of the operation.
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
 *      Change the attribute set of a vertex in a polygon object.
 *
 *  @param polygon          A polygon object.
 *  @param index            A 0-based index into the array of vertices of the polygon.
 *  @param attributeSet     New attribute set for the vertex.
 *  @result                 Success or failure of the operation.
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
 *      Create a new polyhedron geometry object.
 *
 *  @param polyhedronData   Data describing a polyhedron.
 *  @result                 Reference to a new Polyhedron geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Polyhedron_New (
    const TQ3PolyhedronData       *polyhedronData
);



/*!
 *  @function
 *      Q3Polyhedron_Submit
 *  @discussion
 *		Submits a polyhedron for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param polyhedronData   Data describing a polyhedron.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a polyhedron object by supplying a full new set of data.
 *
 *  @param polyhedron       A polyhedron object.
 *  @param polyhedronData   Data describing a polyhedron.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a Polyhedron object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Polyhedron_EmptyData</code>.
 *
 *  @param polyhedron       A Polyhedron object.
 *  @param polyhedronData   Receives data describing the Polyhedron object.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3Polyhedron_GetData</code>.
 *
 *  @param polyhedronData   Data describing a Polyhedron, previously obtained with
 *							<code>Q3Polyhedron_GetData</code>.
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *      Create a new polyline geometry object.
 *
 *  @param polylineData     Data describing a polyline.
 *  @result                 Reference to a new Polyline geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3PolyLine_New (
    const TQ3PolyLineData         *polylineData
);



/*!
 *  @function
 *      Q3PolyLine_Submit
 *  @discussion
 *		Submits a PolyLine for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param polyLineData     Data describing a PolyLine.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a PolyLine object by supplying a full new set of data.
 *
 *  @param polyLine         A PolyLine object.
 *  @param polyLineData     Data describing a PolyLine.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a PolyLine object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3PolyLine_EmptyData</code>.
 *
 *  @param polyLine         A PolyLine object.
 *  @param polyLineData     Receives data describing the PolyLine object.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3PolyLine_GetData</code>.
 *
 *  @param polyLineData     Data describing a PolyLine, previously obtained with
 *							<code>Q3PolyLine_GetData</code>.
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *      Create a new torus geometry object.
 *
 *  @param torusData        Data describing a torus.
 *  @result                 Reference to a new Torus geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Torus_New (
    const TQ3TorusData            *torusData
);



/*!
 *  @function
 *      Q3Torus_Submit
 *  @discussion
 *		Submits a torus for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param torusData        Data describing a torus.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a torus object by supplying a full new set of data.
 *
 *  @param torus            A torus object.
 *  @param torusData        Data describing a torus.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a Torus object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Torus_EmptyData</code>.
 *
 *  @param torus            A Torus object.
 *  @param torusData        Receives data describing the Torus object.
 *  @result                 Success or failure of the operation.
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
 *      Change the origin of a torus object.
 *
 *  @param torus            The torus object.
 *  @param origin           The new origin.
 *  @result                 Success or failure of the operation.
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
 *      Change the orientation vector of a torus object.
 *
 *  @param torus            A torus object.
 *  @param orientation      New orientation vector for the torus.
 *  @result                 Success or failure of the operation.
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
 *      Change the major radius vector of a Torus object.
 *
 *  @param torus            The Torus object.
 *  @param majorRadius      New major radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Change the minor radius vector of a Torus object.
 *
 *  @param torus            The Torus object.
 *  @param minorRadius      New minor radius vector.
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *      Get the origin of a Torus object.
 *
 *  @param torus            The Torus object.
 *  @param origin           Receives the origin.
 *  @result                 Success or failure of the operation.
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
 *      Get the orientation vector of a torus.
 *
 *  @param torus            A torus object.
 *  @param orientation      Receives the orientation vector of the torus.
 *  @result                 Success or failure of the operation.
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
 *      Get the major radius vector of a Torus object.
 *
 *  @param torus            The Torus object.
 *  @param majorRadius      Receives the major radius vector.
 *  @result                 Success or failure of the operation.
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
 *      Get the minor radius vector of a Torus object.
 *
 *  @param torus            The Torus object.
 *  @param minorRadius      Receives the minor radius vector.
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3Torus_GetData</code>.
 *
 *  @param torusData        Data describing a Torus, previously obtained with
 *							<code>Q3Torus_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3Torus_EmptyData (
    TQ3TorusData                  *torusData
);



/*!
 *  @function
 *      Q3Triangle_New
 *  @discussion
 *      Create a new triangle geometry object.
 *
 *  @param triangleData     Data describing a triangle.
 *  @result                 Reference to a new Triangle geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3Triangle_New (
    const TQ3TriangleData         *triangleData
);



/*!
 *  @function
 *      Q3Triangle_Submit
 *  @discussion
 *		Submits a triangle for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param triangleData     Data describing a triangle.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a triangle object by supplying a full new set of data.
 *
 *  @param triangle         A triangle object.
 *  @param triangleData     Data describing a triangle.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a Triangle object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Triangle_EmptyData</code>.
 *
 *  @param triangle         A Triangle object.
 *  @param triangleData     Receives data describing the Triangle object.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3Triangle_GetData</code>.
 *
 *  @param triangleData     Data describing a Triangle, previously obtained with
 *							<code>Q3Triangle_GetData</code>.
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *      Create a new TriGrid geometry object.
 *
 *  @param triGridData      Data describing a TriGrid.
 *  @result                 Reference to a new TriGrid geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3TriGrid_New (
    const TQ3TriGridData          *triGridData
);



/*!
 *  @function
 *      Q3TriGrid_Submit
 *  @discussion
 *		Submits a TriGrid for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param triGridData      Data describing a TriGrid.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a TriGrid object by supplying a full new set of data.
 *
 *  @param triGrid          A TriGrid object.
 *  @param triGridData      Data describing a TriGrid.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a TriGrid object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3TriGrid_EmptyData</code>.
 *
 *  @param triGrid          A TriGrid object.
 *  @param triGridData      Receives data describing the TriGrid object.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3TriGrid_GetData</code>.
 *
 *  @param triGridData      Data describing a TriGrid, previously obtained with
 *							<code>Q3TriGrid_GetData</code>.
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *  @result                 Success or failure of the operation.
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
 *      Create a new TriMesh geometry object.
 *
 *		If you wish, you can set the <code>isEmpty</code> flag in the <code>bBox</code> field,
 *		and Quesa will compute the bounding box.
 *
 *  @param triMeshData      Data describing a TriMesh.
 *  @result                 Reference to a new TriMesh geometry object, or NULL on failure.
 */
EXTERN_API_C ( TQ3GeometryObject  )
Q3TriMesh_New (
    const TQ3TriMeshData          *triMeshData
);



/*!
 *  @function
 *      Q3TriMesh_Submit
 *  @discussion
 *		Submits a TriMesh for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param triMeshData      Data describing a TriMesh.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
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
 *      Modify a TriMesh object by supplying a full new set of data.
 *
 *		If you wish, you can set the <code>isEmpty</code> flag in the <code>bBox</code> field,
 *		and Quesa will compute the bounding box.
 *
 *  @param triMesh          A TriMesh object.
 *  @param triMeshData      Data describing a TriMesh.
 *  @result                 Success or failure of the operation.
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
 *      Get the data of a TriMesh object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3TriMesh_EmptyData</code>.
 *
 *  @param triMesh          A TriMesh object.
 *  @param triMeshData      Receives data describing the TriMesh object.
 *  @result                 Success or failure of the operation.
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
 *      Release memory allocated by <code>Q3TriMesh_GetData</code>.
 *
 *  @param triMeshData      Data describing a TriMesh, previously obtained with
 *							<code>Q3TriMesh_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3TriMesh_EmptyData (
    TQ3TriMeshData                *triMeshData
);



/*!
 *  @function
 *      Q3TriMesh_LockData
 *  @discussion
 *      Lock a Trimesh for direct access.
 *
 *      Returns a pointer to the internal TQ3TriMeshData for a TriMesh,
 *      allowing direct access without the need to copy TriMesh data out
 *      of and back in to Quesa.
 *
 *      The readOnly flag should be used to indicate if the application
 *      needs to make changes to the TriMesh data, or if the pointer
 *      should be considered const.
 *
 *      When the application no longer needs access to the TriMesh data,
 *      it must unlock the TriMesh with Q3TriMesh_UnlockData. Changes to
 *      the TriMesh data may not be relayed to renderers until the TriMesh
 *      has been unlocked.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param triMesh          The TriMesh to lock.
 *  @param readOnly         Indicates if the returned data is read-only.
 *  @param triMeshData      Receives a pointer to the TQ3TriMeshData for
 *                          the TriMesh.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3TriMesh_LockData (
    TQ3GeometryObject             triMesh,
    TQ3Boolean                    readOnly,
    TQ3TriMeshData                **triMeshData
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3TriMesh_UnlockData
 *  @discussion
 *      Unlocks a TriMesh previously locked with Q3TriMesh_LockData.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param triMesh          The TriMesh to unlock.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3TriMesh_UnlockData (
    TQ3GeometryObject             triMesh
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif



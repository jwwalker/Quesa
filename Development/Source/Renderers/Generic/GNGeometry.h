/*  NAME:
        GNGeometry.h

    DESCRIPTION:
        Header file for GNGeometry.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#ifndef GNGEOMETRY_HDR
#define GNGEOMETRY_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Required geometries
TQ3Status			GNGeometry_Triangle(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriangleData			*geomData);

TQ3Status			GNGeometry_Line(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3LineData				*geomData);

TQ3Status			GNGeometry_Point(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PointData			*geomData);

TQ3Status			GNGeometry_Marker(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3MarkerData			*geomData);

TQ3Status			GNGeometry_PixmapMarker(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PixmapMarkerData		*geomData);


// Optional geometries
TQ3Status			GNGeometry_Box(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3BoxData				*geomData);

TQ3Status			GNGeometry_GeneralPolygon(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3GeneralPolygonData	*geomData);

TQ3Status			GNGeometry_Mesh(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								void					*geomData);

TQ3Status			GNGeometry_NURBCurve(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3NURBCurveData		*geomData);

TQ3Status			GNGeometry_NURBPatch(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3NURBPatchData		*geomData);

TQ3Status			GNGeometry_Polygon(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolygonData			*geomData);

TQ3Status			GNGeometry_PolyLine(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolyLineData			*geomData);

TQ3Status			GNGeometry_TriGrid(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriGridData			*geomData);

TQ3Status			GNGeometry_Cone(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3ConeData				*geomData);

TQ3Status			GNGeometry_Cylinder(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3CylinderData			*geomData);

TQ3Status			GNGeometry_Disk(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3DiskData				*geomData);

TQ3Status			GNGeometry_Ellipse(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3EllipseData			*geomData);

TQ3Status			GNGeometry_Ellipsoid(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3EllipsoidData		*geomData);

TQ3Status			GNGeometry_Polyhedron(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolyhedronData		*geomData);

TQ3Status			GNGeometry_Torus(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TorusData			*geomData);

TQ3Status			GNGeometry_TriMesh(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriMeshData			*geomData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


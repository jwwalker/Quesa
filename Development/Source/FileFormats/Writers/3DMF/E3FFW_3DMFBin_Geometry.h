/*  NAME:
        E3FFW_3DMFBin_Geometry.h

    DESCRIPTION:
        Header file for E3FFW_3DMFBin_Geometry.c.

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
#ifndef E3FFW_3DMFBIN_GEOMETRY_HDR
#define E3FFW_3DMFBIN_GEOMETRY_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here

#include "E3FFR_3DMF.h"




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
TQ3Status			E3FFW_3DMF_Triangle(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriangleData			*geomData);

TQ3Status			E3FFW_3DMF_Line(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3LineData				*geomData);

TQ3Status			E3FFW_3DMF_Point(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PointData			*geomData);

TQ3Status			E3FFW_3DMF_Marker(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3MarkerData			*geomData);

TQ3Status			E3FFW_3DMF_PixmapMarker(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PixmapMarkerData		*geomData);


// Optional geometries
TQ3Status			E3FFW_3DMF_Box(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3BoxData				*geomData);

TQ3Status			E3FFW_3DMF_GeneralPolygon(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3GeneralPolygonData	*geomData);

TQ3Status			E3FFW_3DMF_Mesh(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								void					*geomData);

TQ3Status			E3FFW_3DMF_NURBCurve(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3NURBCurveData		*geomData);

TQ3Status			E3FFW_3DMF_NURBPatch(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3NURBPatchData		*geomData);

TQ3Status			E3FFW_3DMF_Polygon(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolygonData			*geomData);

TQ3Status			E3FFW_3DMF_PolyLine(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolyLineData			*geomData);

TQ3Status			E3FFW_3DMF_TriGrid(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriGridData			*geomData);

TQ3Status			E3FFW_3DMF_Cone(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3ConeData				*geomData);

TQ3Status			E3FFW_3DMF_Cylinder(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3CylinderData			*geomData);

TQ3Status			E3FFW_3DMF_Disk(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3DiskData				*geomData);

TQ3Status			E3FFW_3DMF_Ellipse(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3EllipseData			*geomData);

TQ3Status			E3FFW_3DMF_Ellipsoid(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3EllipsoidData		*geomData);

TQ3Status			E3FFW_3DMF_Polyhedron(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolyhedronData		*geomData);

TQ3Status			E3FFW_3DMF_Torus(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TorusData			*geomData);

TQ3Status			E3FFW_3DMF_TriMesh(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriMeshData			*geomData);


TQ3Status			E3FFW_3DMF_RegisterGeom();
TQ3Status			E3FFW_3DMF_UnregisterGeom();





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        GNGeometry.c

    DESCRIPTION:
        Quesa generic renderer geometry methods.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "GNPrefix.h"
#include "GNGeometry.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      GNGeometry_Triangle : Triangle handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Triangle(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriangleData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Line : Line handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Line(TQ3ViewObject			theView,
				void					*instanceData,
				TQ3GeometryObject		theGeom,
				TQ3LineData				*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Point : Point handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Point(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PointData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Marker : Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Marker(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3MarkerData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_PixmapMarker : Pixmap Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_PixmapMarker(TQ3ViewObject			theView,
						void					*instanceData,
						TQ3GeometryObject		theGeom,
						TQ3PixmapMarkerData		*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Box : Box handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Box(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3BoxData				*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_GeneralPolygon : General Polygon handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_GeneralPolygon(TQ3ViewObject				theView,
							void					*instanceData,
							TQ3GeometryObject		theGeom,
							TQ3GeneralPolygonData	*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Mesh : Mesh handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Mesh(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					void					*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_NURBCurve : NURB Curve handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_NURBCurve(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3NURBCurveData		*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_NURBPatch : NURB Patch handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_NURBPatch(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3NURBPatchData		*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Polygon : Polygon handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Polygon(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolygonData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_PolyLine : PolyLine handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_PolyLine(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolyLineData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_TriGrid : TridGrid handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_TriGrid(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriGridData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Cone : Cone handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Cone(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3ConeData				*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Cylinder : Cylinder handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Cylinder(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3CylinderData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Disk : Disk handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Disk(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3DiskData				*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Ellipse : Ellipse handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Ellipse(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3EllipseData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Ellipsoid : Ellipsoid handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Ellipsoid(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3EllipsoidData		*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Polyhedron : Polyhedron handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Polyhedron(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolyhedronData		*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_Torus : Torus handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_Torus(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TorusData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNGeometry_TriMesh : TriMesh handler.
//-----------------------------------------------------------------------------
TQ3Status
GNGeometry_TriMesh(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriMeshData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// We're done
	return(kQ3Success);
}



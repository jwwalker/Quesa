/*  NAME:
        E3FFW_3DMFBin_Geometry.c

    DESCRIPTION:
        Quesa 3DMFBin writer geometry methods.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#include "E3Prefix.h"
#include "E3FFW_3DMFBin_Geometry.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFW_3DMFBin_Triangle : Triangle handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Triangle(TQ3ViewObject			theView,
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
//      E3FFW_3DMFBin_Line : Line handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Line(TQ3ViewObject			theView,
				void					*instanceData,
				TQ3GeometryObject		theGeom,
				TQ3LineData				*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Point : Point handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Point(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PointData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Marker : Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Marker(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3MarkerData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_PixmapMarker : Pixmap Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_PixmapMarker(TQ3ViewObject			theView,
						void					*instanceData,
						TQ3GeometryObject		theGeom,
						TQ3PixmapMarkerData		*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Box : Box handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Box(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3BoxData				*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_GeneralPolygon : General Polygon handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_GeneralPolygon(TQ3ViewObject				theView,
							void					*instanceData,
							TQ3GeometryObject		theGeom,
							TQ3GeneralPolygonData	*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Mesh : Mesh handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Mesh(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					void					*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_NURBCurve : NURB Curve handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_NURBCurve(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3NURBCurveData		*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_NURBPatch : NURB Patch handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_NURBPatch(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3NURBPatchData		*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Polygon : Polygon handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Polygon(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolygonData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_PolyLine : PolyLine handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_PolyLine(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolyLineData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_TriGrid : TridGrid handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_TriGrid(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriGridData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Cone : Cone handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Cone(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3ConeData				*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Cylinder : Cylinder handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Cylinder(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3CylinderData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Disk : Disk handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Disk(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3DiskData				*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Ellipse : Ellipse handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Ellipse(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3EllipseData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Ellipsoid : Ellipsoid handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Ellipsoid(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3EllipsoidData		*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Polyhedron : Polyhedron handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Polyhedron(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolyhedronData		*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_Torus : Torus handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Torus(TQ3ViewObject				theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TorusData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_TriMesh : TriMesh handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_TriMesh(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriMeshData			*geomData)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theGeom)
#pragma unused(geomData)



	// To Be implemented
	return(kQ3Failure);
}



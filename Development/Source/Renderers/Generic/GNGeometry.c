/*  NAME:
        GNGeometry.c

    DESCRIPTION:
        Quesa generic renderer geometry methods.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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



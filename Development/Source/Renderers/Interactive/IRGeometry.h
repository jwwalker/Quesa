/*  NAME:
        IRGeometry.h

    DESCRIPTION:
        Header file for IRGeometry.c.

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
#ifndef __IRGEOMETRY__
#define __IRGEOMETRY__
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
TQ3Status			IRGeometry_Triangle(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriangleData			*geomData);

TQ3Status			IRGeometry_Line(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3LineData				*geomData);

TQ3Status			IRGeometry_Point(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PointData			*geomData);

TQ3Status			IRGeometry_Marker(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3MarkerData			*geomData);

TQ3Status			IRGeometry_PixmapMarker(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PixmapMarkerData		*geomData);

TQ3Status			IRGeometry_PolyLine(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolyLineData			*geomData);

TQ3Status			IRGeometry_TriMesh(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriMeshData			*geomData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


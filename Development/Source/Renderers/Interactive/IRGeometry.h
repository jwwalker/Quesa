/*  NAME:
        IRGeometry.h

    DESCRIPTION:
        Header file for IRGeometry.c.

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
#ifndef IRGEOMETRY_HDR
#define IRGEOMETRY_HDR
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

enum{
    kQ3XAttributeMaskGeometry               =kQ3XAttributeMaskAmbientCoefficient |
                                             kQ3XAttributeMaskDiffuseColor       |
                                             kQ3XAttributeMaskSpecularControl    |
                                             kQ3XAttributeMaskTransparencyColor  |
                                             kQ3XAttributeMaskHighlightState     |
                                             kQ3XAttributeMaskSpecularColor
};



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
void				IRGeometry_Terminate(
								TQ3InteractiveData		*instanceData);

TQ3Boolean			IRGeometry_Attribute_Handler(
								TQ3ViewObject theView,
								TQ3AttributeSet geomAttributes,
							 	TQ3InteractiveData *instanceData,
							 	TQ3XAttributeMask needAttributesMask);
							 	
void				IRGeometry_Vertex_GetState(
								TQ3InteractiveData		*instanceData,
								TQ3Vertex3D				*srcVertex,
								TQ3FVertex3D			*dstVertex);

TQ3Status			IRGeometry_Triangle_CalcFlags(
								TQ3InteractiveData		*instanceData,
								TQ3Uns32				numTriangles,
								const TQ3Uns32			*theIndices,
								const TQ3Point3D		*thePoints,
								const TQ3Vector3D		*theNormals,
								TQ3TriFlags				*theFlags);
								
TQ3Boolean			IRGeometry_Triangle_IsVisible(
								TQ3InteractiveData		*instanceData,
								const TQ3Vector3D		*triNormal);

TQ3Status			IRGeometry_Submit_Triangle(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriangleData			*geomData);

TQ3Status			IRGeometry_Submit_Line(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3LineData				*geomData);

TQ3Status			IRGeometry_Submit_Point(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PointData			*geomData);

TQ3Status			IRGeometry_Submit_Marker(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3MarkerData			*geomData);

TQ3Status			IRGeometry_Submit_PixmapMarker(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PixmapMarkerData		*geomData);

TQ3Status			IRGeometry_Submit_PolyLine(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolyLineData			*geomData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


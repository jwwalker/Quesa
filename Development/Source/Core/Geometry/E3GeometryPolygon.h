/*  NAME:
        E3GeometryPolygon.h

    DESCRIPTION:
        Header file for E3GeometryPolygon.c.

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
#ifndef __E3GEOMETRY_POLYGON__
#define __E3GEOMETRY_POLYGON__
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
TQ3Status			E3GeometryPolygon_RegisterClass(void);
TQ3Status			E3GeometryPolygon_UnregisterClass(void);

TQ3GeometryObject	E3Polygon_New(const TQ3PolygonData *polygonData);
TQ3Status			E3Polygon_Submit(const TQ3PolygonData *polygonData, TQ3ViewObject theView);
TQ3Status			E3Polygon_SetData(TQ3GeometryObject thePolygon, const TQ3PolygonData *polygonData);
TQ3Status			E3Polygon_GetData(TQ3GeometryObject thePolygon, TQ3PolygonData *polygonData);
TQ3Status			E3Polygon_EmptyData(TQ3PolygonData *polygonData);
TQ3Status			E3Polygon_GetVertexPosition(TQ3GeometryObject thePolygon, TQ3Uns32 index, TQ3Point3D *point);
TQ3Status			E3Polygon_SetVertexPosition(TQ3GeometryObject thePolygon, TQ3Uns32 index, const TQ3Point3D *point);
TQ3Status			E3Polygon_GetVertexAttributeSet(TQ3GeometryObject thePolygon, TQ3Uns32 index, TQ3AttributeSet *attributeSet);
TQ3Status			E3Polygon_SetVertexAttributeSet(TQ3GeometryObject thePolygon, TQ3Uns32 index, TQ3AttributeSet attributeSet);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


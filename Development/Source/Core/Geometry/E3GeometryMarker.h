/*  NAME:
        E3GeometryMarker.h

    DESCRIPTION:
        Header file for E3GeometryMarker.c.

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
#ifndef E3GEOMETRY_MARKER_HDR
#define E3GEOMETRY_MARKER_HDR
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
TQ3Status			E3GeometryMarker_RegisterClass(void);
TQ3Status			E3GeometryMarker_UnregisterClass(void);

TQ3GeometryObject	E3Marker_New(const TQ3MarkerData *markerData);
TQ3Status			E3Marker_Submit(const TQ3MarkerData *markerData, TQ3ViewObject theView);
TQ3Status			E3Marker_SetData(TQ3GeometryObject theGeom, const TQ3MarkerData *markerData);
TQ3Status			E3Marker_GetData(TQ3GeometryObject theGeom, TQ3MarkerData *markerData);
TQ3Status			E3Marker_EmptyData(TQ3MarkerData *markerData);
TQ3Status			E3Marker_GetPosition(TQ3GeometryObject theMarker, TQ3Point3D *location);
TQ3Status			E3Marker_SetPosition(TQ3GeometryObject theMarker, const TQ3Point3D *location);
TQ3Status			E3Marker_GetXOffset(TQ3GeometryObject theMarker, TQ3Int32 *xOffset);
TQ3Status			E3Marker_SetXOffset(TQ3GeometryObject theMarker, TQ3Int32 xOffset);
TQ3Status			E3Marker_GetYOffset(TQ3GeometryObject theMarker, TQ3Int32 *yOffset);
TQ3Status			E3Marker_SetYOffset(TQ3GeometryObject theMarker, TQ3Int32 yOffset);
TQ3Status			E3Marker_GetBitmap(TQ3GeometryObject theMarker, TQ3Bitmap *bitmap);
TQ3Status			E3Marker_SetBitmap(TQ3GeometryObject theMarker, const TQ3Bitmap *bitmap);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


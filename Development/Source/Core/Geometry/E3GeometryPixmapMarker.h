/*  NAME:
        E3GeometryPixmapMarker.h

    DESCRIPTION:
        Header file for E3GeometryPixmapMarker.c.

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
#ifndef __E3GEOMETRY_PIXMAPMARKER__
#define __E3GEOMETRY_PIXMAPMARKER__
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
TQ3Status			E3GeometryPixmapMarker_RegisterClass(void);
TQ3Status			E3GeometryPixmapMarker_UnregisterClass(void);

TQ3GeometryObject	E3PixmapMarker_New(const TQ3PixmapMarkerData *pixmapMarkerData);
TQ3Status			E3PixmapMarker_Submit(const TQ3PixmapMarkerData *pixmapMarkerData, TQ3ViewObject theView);
TQ3Status			E3PixmapMarker_SetData(TQ3GeometryObject theGeom, const TQ3PixmapMarkerData *pixmapMarkerData);
TQ3Status			E3PixmapMarker_GetData(TQ3GeometryObject theGeom, TQ3PixmapMarkerData *pixmapMarkerData);
TQ3Status			E3PixmapMarker_EmptyData(TQ3PixmapMarkerData *pixmapMarkerData);
TQ3Status			E3PixmapMarker_GetPosition(TQ3GeometryObject pixmapMarker, TQ3Point3D *position);
TQ3Status			E3PixmapMarker_SetPosition(TQ3GeometryObject pixmapMarker, const TQ3Point3D *position);
TQ3Status			E3PixmapMarker_GetXOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 *xOffset);
TQ3Status			E3PixmapMarker_SetXOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 xOffset);
TQ3Status			E3PixmapMarker_GetYOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 *yOffset);
TQ3Status			E3PixmapMarker_SetYOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 yOffset);
TQ3Status			E3PixmapMarker_GetPixmap(TQ3GeometryObject pixmapMarker, TQ3StoragePixmap *pixmap);
TQ3Status			E3PixmapMarker_SetPixmap(TQ3GeometryObject pixmapMarker, const TQ3StoragePixmap *pixmap);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


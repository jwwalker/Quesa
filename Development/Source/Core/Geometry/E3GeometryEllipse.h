/*  NAME:
        E3GeometryEllipse.h

    DESCRIPTION:
        Header file for E3GeometryEllipse.c.

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
#ifndef E3GEOMETRY_ELLIPSE_HDR
#define E3GEOMETRY_ELLIPSE_HDR
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
TQ3Status			E3GeometryEllipse_RegisterClass(void);
TQ3Status			E3GeometryEllipse_UnregisterClass(void);

TQ3GeometryObject	E3Ellipse_New(const TQ3EllipseData *ellipseData);
TQ3Status			E3Ellipse_Submit(const TQ3EllipseData *ellipseData, TQ3ViewObject theView);
TQ3Status			E3Ellipse_SetData(TQ3GeometryObject theEllipse, const TQ3EllipseData *ellipseData);
TQ3Status			E3Ellipse_GetData(TQ3GeometryObject theEllipse, TQ3EllipseData *ellipseData);
TQ3Status			E3Ellipse_EmptyData(TQ3EllipseData *ellipseData);
TQ3Status			E3Ellipse_SetOrigin(TQ3GeometryObject theEllipse, const TQ3Point3D *origin);
TQ3Status			E3Ellipse_SetMajorRadius(TQ3GeometryObject theEllipse, const TQ3Vector3D *majorRadius);
TQ3Status			E3Ellipse_SetMinorRadius(TQ3GeometryObject theEllipse, const TQ3Vector3D *minorRadius);
TQ3Status			E3Ellipse_GetOrigin(TQ3GeometryObject theEllipse, TQ3Point3D *origin);
TQ3Status			E3Ellipse_GetMajorRadius(TQ3GeometryObject theEllipse, TQ3Vector3D *majorRadius);
TQ3Status			E3Ellipse_GetMinorRadius(TQ3GeometryObject theEllipse, TQ3Vector3D *minorRadius);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


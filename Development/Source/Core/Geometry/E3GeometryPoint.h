/*  NAME:
        E3GeometryPoint.h

    DESCRIPTION:
        Header file for E3GeometryPoint.c.

    COPYRIGHT:
        Quesa Copyright � 1999-2002, Quesa Developers.
        
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
#ifndef E3GEOMETRY_POINT_HDR
#define E3GEOMETRY_POINT_HDR
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
TQ3Status			E3GeometryPoint_RegisterClass(void);
TQ3Status			E3GeometryPoint_UnregisterClass(void);

TQ3GeometryObject	E3Point_New(const TQ3PointData *pointData);
TQ3Status			E3Point_Submit(const TQ3PointData *pointData, TQ3ViewObject theView);
TQ3Status			E3Point_GetData(TQ3GeometryObject thePoint, TQ3PointData *pointData);
TQ3Status			E3Point_SetData(TQ3GeometryObject thePoint, const TQ3PointData *pointData);
TQ3Status			E3Point_EmptyData(TQ3PointData *pointData);
TQ3Status			E3Point_SetPosition(TQ3GeometryObject thePoint, const TQ3Point3D *position);
TQ3Status			E3Point_GetPosition(TQ3GeometryObject thePoint, TQ3Point3D *position);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


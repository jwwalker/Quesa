/*  NAME:
        E3GeometryTorus.h

    DESCRIPTION:
        Header file for E3GeometryTorus.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#ifndef E3GEOMETRY_TORUS_HDR
#define E3GEOMETRY_TORUS_HDR
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
TQ3Status			E3GeometryTorus_RegisterClass(void);
TQ3Status			E3GeometryTorus_UnregisterClass(void);

TQ3GeometryObject	E3Torus_New(const TQ3TorusData *torusData);
TQ3Status			E3Torus_Submit(const TQ3TorusData *torusData, TQ3ViewObject theView);
TQ3Status			E3Torus_SetData(TQ3GeometryObject theTorus, const TQ3TorusData *torusData);
TQ3Status			E3Torus_GetData(TQ3GeometryObject theTorus, TQ3TorusData *torusData);
TQ3Status			E3Torus_SetOrigin(TQ3GeometryObject theTorus, const TQ3Point3D *origin);
TQ3Status			E3Torus_SetOrientation(TQ3GeometryObject theTorus, const TQ3Vector3D *orientation);
TQ3Status			E3Torus_SetMajorRadius(TQ3GeometryObject theTorus, const TQ3Vector3D *majorRadius);
TQ3Status			E3Torus_SetMinorRadius(TQ3GeometryObject theTorus, const TQ3Vector3D *minorRadius);
TQ3Status			E3Torus_SetRatio(TQ3GeometryObject theTorus, float ratio);
TQ3Status			E3Torus_GetOrigin(TQ3GeometryObject theTorus, TQ3Point3D *origin);
TQ3Status			E3Torus_GetOrientation(TQ3GeometryObject theTorus, TQ3Vector3D *orientation);
TQ3Status			E3Torus_GetMajorRadius(TQ3GeometryObject theTorus, TQ3Vector3D *majorRadius);
TQ3Status			E3Torus_GetMinorRadius(TQ3GeometryObject theTorus, TQ3Vector3D *minorRadius);
TQ3Status			E3Torus_GetRatio(TQ3GeometryObject theTorus, float *ratio);
TQ3Status			E3Torus_EmptyData(TQ3TorusData *torusData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


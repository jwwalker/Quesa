/*  NAME:
        E3GeometryBox.h

    DESCRIPTION:
        Header file for E3GeometryBox.c.

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
#ifndef E3GEOMETRY_BOX_HDR
#define E3GEOMETRY_BOX_HDR
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
TQ3Status			E3GeometryBox_RegisterClass(void);
TQ3Status			E3GeometryBox_UnregisterClass(void);

TQ3GeometryObject	E3Box_New(const TQ3BoxData *boxData);
TQ3Status			E3Box_Submit(const TQ3BoxData *boxData, TQ3ViewObject theView);
TQ3Status			E3Box_SetData(TQ3GeometryObject theBox, const TQ3BoxData *boxData);
TQ3Status			E3Box_GetData(TQ3GeometryObject theBox, TQ3BoxData *boxData);
TQ3Status			E3Box_EmptyData(TQ3BoxData *boxData);
TQ3Status			E3Box_SetOrigin(TQ3GeometryObject theBox, const TQ3Point3D *origin);
TQ3Status			E3Box_SetOrientation(TQ3GeometryObject theBox, const TQ3Vector3D *orientation);
TQ3Status			E3Box_SetMajorAxis(TQ3GeometryObject theBox, const TQ3Vector3D *majorAxis);
TQ3Status			E3Box_SetMinorAxis(TQ3GeometryObject theBox, const TQ3Vector3D *minorAxis);
TQ3Status			E3Box_GetOrigin(TQ3GeometryObject theBox, TQ3Point3D *origin);
TQ3Status			E3Box_GetOrientation(TQ3GeometryObject theBox, TQ3Vector3D *orientation);
TQ3Status			E3Box_GetMajorAxis(TQ3GeometryObject theBox, TQ3Vector3D *majorAxis);
TQ3Status			E3Box_GetMinorAxis(TQ3GeometryObject theBox, TQ3Vector3D *minorAxis);
TQ3Status			E3Box_GetFaceAttributeSet(TQ3GeometryObject theBox, TQ3Uns32 faceIndex, TQ3AttributeSet *faceAttributeSet);
TQ3Status			E3Box_SetFaceAttributeSet(TQ3GeometryObject theBox, TQ3Uns32 faceIndex, TQ3AttributeSet faceAttributeSet);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        E3GeometryCone.h

    DESCRIPTION:
        Header file for E3GeometryCone.c.

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
#ifndef E3GEOMETRY_CONE_HDR
#define E3GEOMETRY_CONE_HDR
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
TQ3Status			E3GeometryCone_RegisterClass(void);
TQ3Status			E3GeometryCone_UnregisterClass(void);

TQ3GeometryObject	E3Cone_New(const TQ3ConeData *coneData);
TQ3Status			E3Cone_Submit(const TQ3ConeData *coneData, TQ3ViewObject theView);
TQ3Status			E3Cone_SetData(TQ3GeometryObject theCone, const TQ3ConeData *coneData);
TQ3Status			E3Cone_GetData(TQ3GeometryObject theCone, TQ3ConeData *coneData);
TQ3Status			E3Cone_SetOrigin(TQ3GeometryObject theCone, const TQ3Point3D *origin);
TQ3Status			E3Cone_SetOrientation(TQ3GeometryObject theCone, const TQ3Vector3D *orientation);
TQ3Status			E3Cone_SetMajorRadius(TQ3GeometryObject theCone, const TQ3Vector3D *majorRadius);
TQ3Status			E3Cone_SetMinorRadius(TQ3GeometryObject theCone, const TQ3Vector3D *minorRadius);
TQ3Status			E3Cone_GetOrigin(TQ3GeometryObject theCone, TQ3Point3D *origin);
TQ3Status			E3Cone_GetOrientation(TQ3GeometryObject theCone, TQ3Vector3D *orientation);
TQ3Status			E3Cone_GetMajorRadius(TQ3GeometryObject theCone, TQ3Vector3D *majorRadius);
TQ3Status			E3Cone_GetMinorRadius(TQ3GeometryObject theCone, TQ3Vector3D *minorRadius);
TQ3Status			E3Cone_SetCaps(TQ3GeometryObject theCone, TQ3EndCap caps);
TQ3Status			E3Cone_GetCaps(TQ3GeometryObject theCone, TQ3EndCap *caps);
TQ3Status			E3Cone_SetBottomAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet bottomAttributeSet);
TQ3Status			E3Cone_GetBottomAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet *bottomAttributeSet);
TQ3Status			E3Cone_SetFaceAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet faceAttributeSet);
TQ3Status			E3Cone_GetFaceAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet *faceAttributeSet);
TQ3Status			E3Cone_EmptyData(TQ3ConeData *coneData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


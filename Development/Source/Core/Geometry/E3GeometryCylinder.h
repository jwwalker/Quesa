/*  NAME:
        E3GeometryCylinder.h

    DESCRIPTION:
        Header file for E3GeometryCylinder.c.

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
#ifndef E3GEOMETRY_CYLINDER_HDR
#define E3GEOMETRY_CYLINDER_HDR
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
TQ3Status			E3GeometryCylinder_RegisterClass(void);
TQ3Status			E3GeometryCylinder_UnregisterClass(void);

TQ3GeometryObject	E3Cylinder_New(const TQ3CylinderData *cylinderData);
TQ3Status			E3Cylinder_Submit(const TQ3CylinderData *cylinderData, TQ3ViewObject theView);
TQ3Status			E3Cylinder_SetData(TQ3GeometryObject theCylinder, const TQ3CylinderData *cylinderData);
TQ3Status			E3Cylinder_GetData(TQ3GeometryObject theCylinder, TQ3CylinderData *cylinderData);
TQ3Status			E3Cylinder_SetOrigin(TQ3GeometryObject theCylinder, const TQ3Point3D *origin);
TQ3Status			E3Cylinder_SetOrientation(TQ3GeometryObject theCylinder, const TQ3Vector3D *orientation);
TQ3Status			E3Cylinder_SetMajorRadius(TQ3GeometryObject theCylinder, const TQ3Vector3D *majorRadius);
TQ3Status			E3Cylinder_SetMinorRadius(TQ3GeometryObject theCylinder, const TQ3Vector3D *minorRadius);
TQ3Status			E3Cylinder_GetOrigin(TQ3GeometryObject theCylinder, TQ3Point3D *origin);
TQ3Status			E3Cylinder_GetOrientation(TQ3GeometryObject theCylinder, TQ3Vector3D *orientation);
TQ3Status			E3Cylinder_GetMajorRadius(TQ3GeometryObject theCylinder, TQ3Vector3D *majorRadius);
TQ3Status			E3Cylinder_GetMinorRadius(TQ3GeometryObject theCylinder, TQ3Vector3D *minorRadius);
TQ3Status			E3Cylinder_SetCaps(TQ3GeometryObject theCylinder, TQ3EndCap caps);
TQ3Status			E3Cylinder_GetCaps(TQ3GeometryObject theCylinder, TQ3EndCap *caps);
TQ3Status			E3Cylinder_SetTopAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet topAttributeSet);
TQ3Status			E3Cylinder_GetTopAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet *topAttributeSet);
TQ3Status			E3Cylinder_SetBottomAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet bottomAttributeSet);
TQ3Status			E3Cylinder_GetBottomAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet *bottomAttributeSet);
TQ3Status			E3Cylinder_SetFaceAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet faceAttributeSet);
TQ3Status			E3Cylinder_GetFaceAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet *faceAttributeSet);
TQ3Status			E3Cylinder_EmptyData(TQ3CylinderData *cylinderData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        E3GeometryCylinder.h

    DESCRIPTION:
        Header file for E3GeometryCylinder.c.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
TQ3Status			E3Cylinder_SetInteriorAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet intAttributeSet);
TQ3Status			E3Cylinder_GetInteriorAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet *intAttributeSet);
TQ3Status			E3Cylinder_EmptyData(TQ3CylinderData *cylinderData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


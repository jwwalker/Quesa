/*  NAME:
        E3GeometryCone.h

    DESCRIPTION:
        Header file for E3GeometryCone.c.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
TQ3Status			E3Cone_SetInteriorAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet intAttributeSet);
TQ3Status			E3Cone_GetInteriorAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet *intAttributeSet);
TQ3Status			E3Cone_EmptyData(TQ3ConeData *coneData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        E3GeometryBox.h

    DESCRIPTION:
        Header file for E3GeometryBox.c.

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
TQ3Status			E3Box_GetData(TQ3GeometryObject theBox, TQ3BoxData *boxData);
TQ3Status			E3Box_SetData(TQ3GeometryObject theBox, const TQ3BoxData *boxData);
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


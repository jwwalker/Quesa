/*  NAME:
        E3GeometryTorus.h

    DESCRIPTION:
        Header file for E3GeometryTorus.c.

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


/*  NAME:
        E3GeometryEllipse.h

    DESCRIPTION:
        Header file for E3GeometryEllipse.c.

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


/*  NAME:
        E3GeometryGeneralPolygon.h

    DESCRIPTION:
        Header file for E3GeometryGeneralPolygon.c.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#ifndef E3GEOMETRY_GENERALPOLYGON_HDR
#define E3GEOMETRY_GENERALPOLYGON_HDR
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
TQ3Status			E3GeometryGeneralPolygon_RegisterClass(void);
TQ3Status			E3GeometryGeneralPolygon_UnregisterClass(void);

TQ3GeometryObject	E3GeneralPolygon_New(const TQ3GeneralPolygonData *generalPolygonData);
TQ3Status			E3GeneralPolygon_Submit(const TQ3GeneralPolygonData *generalPolygonData, TQ3ViewObject theView);
TQ3Status			E3GeneralPolygon_SetData(TQ3GeometryObject generalPolygon, const TQ3GeneralPolygonData *generalPolygonData);
TQ3Status			E3GeneralPolygon_GetData(TQ3GeometryObject generalPolygon, TQ3GeneralPolygonData *generalPolygonData);
TQ3Status			E3GeneralPolygon_EmptyData(TQ3GeneralPolygonData *generalPolygonData);
TQ3Uns32			E3GeneralPolygon_GetNumContours(TQ3GeometryObject theGeneralPolygon);
TQ3Uns32			E3GeneralPolygon_GetNumVertices(TQ3GeometryObject theGeneralPolygon, TQ3Uns32 contourIndex);
TQ3Status			E3GeneralPolygon_GetVertexPosition(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3Point3D *position);
TQ3Status			E3GeneralPolygon_SetVertexPosition(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, const TQ3Point3D *position);
TQ3Status			E3GeneralPolygon_GetVertexAttributeSet(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3AttributeSet *attributeSet);
TQ3Status			E3GeneralPolygon_SetVertexAttributeSet(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3AttributeSet attributeSet);
TQ3Status			E3GeneralPolygon_SetShapeHint(TQ3GeometryObject generalPolygon, TQ3GeneralPolygonShapeHint shapeHint);
TQ3Status			E3GeneralPolygon_GetShapeHint(TQ3GeometryObject generalPolygon, TQ3GeneralPolygonShapeHint *shapeHint);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


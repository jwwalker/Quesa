/*  NAME:
        E3GeometryPolygon.h

    DESCRIPTION:
        Header file for E3GeometryPolygon.c.

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
#ifndef E3GEOMETRY_POLYGON_HDR
#define E3GEOMETRY_POLYGON_HDR
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
TQ3Status			E3GeometryPolygon_RegisterClass(void);
TQ3Status			E3GeometryPolygon_UnregisterClass(void);

TQ3GeometryObject	E3Polygon_New(const TQ3PolygonData *polygonData);
TQ3Status			E3Polygon_Submit(const TQ3PolygonData *polygonData, TQ3ViewObject theView);
TQ3Status			E3Polygon_SetData(TQ3GeometryObject thePolygon, const TQ3PolygonData *polygonData);
TQ3Status			E3Polygon_GetData(TQ3GeometryObject thePolygon, TQ3PolygonData *polygonData);
TQ3Status			E3Polygon_EmptyData(TQ3PolygonData *polygonData);
TQ3Status			E3Polygon_GetVertexPosition(TQ3GeometryObject thePolygon, TQ3Uns32 index, TQ3Point3D *point);
TQ3Status			E3Polygon_SetVertexPosition(TQ3GeometryObject thePolygon, TQ3Uns32 index, const TQ3Point3D *point);
TQ3Status			E3Polygon_GetVertexAttributeSet(TQ3GeometryObject thePolygon, TQ3Uns32 index, TQ3AttributeSet *attributeSet);
TQ3Status			E3Polygon_SetVertexAttributeSet(TQ3GeometryObject thePolygon, TQ3Uns32 index, TQ3AttributeSet attributeSet);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


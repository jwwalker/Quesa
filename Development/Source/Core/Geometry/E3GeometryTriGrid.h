/*  NAME:
        E3GeometryTriGrid.h

    DESCRIPTION:
        Header file for E3GeometryTriGrid.c.

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
#ifndef E3GEOMETRY_TRIGRID_HDR
#define E3GEOMETRY_TRIGRID_HDR
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
TQ3Status			E3GeometryTriGrid_RegisterClass(void);
TQ3Status			E3GeometryTriGrid_UnregisterClass(void);

TQ3GeometryObject	E3TriGrid_New(const TQ3TriGridData *triGridData);
TQ3Status			E3TriGrid_Submit(const TQ3TriGridData *triGridData, TQ3ViewObject theView);
TQ3Status			E3TriGrid_SetData(TQ3GeometryObject triGrid, const TQ3TriGridData *triGridData);
TQ3Status			E3TriGrid_GetData(TQ3GeometryObject triGrid, TQ3TriGridData *triGridData);
TQ3Status			E3TriGrid_EmptyData(TQ3TriGridData *triGridData);
TQ3Status			E3TriGrid_GetVertexPosition(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, TQ3Point3D *position);
TQ3Status			E3TriGrid_SetVertexPosition(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, const TQ3Point3D *position);
TQ3Status			E3TriGrid_GetVertexAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, TQ3AttributeSet *attributeSet);
TQ3Status			E3TriGrid_SetVertexAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, TQ3AttributeSet attributeSet);
TQ3Status			E3TriGrid_GetFacetAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 faceIndex, TQ3AttributeSet *facetAttributeSet);
TQ3Status			E3TriGrid_SetFacetAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 faceIndex, TQ3AttributeSet facetAttributeSet);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


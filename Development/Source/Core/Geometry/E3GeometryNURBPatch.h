/*  NAME:
        E3GeometryNURBPatch.h

    DESCRIPTION:
        Header file for E3GeometryNURBPatch.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#ifndef E3GEOMETRY_NURBPATCH_HDR
#define E3GEOMETRY_NURBPATCH_HDR
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
TQ3Status			E3GeometryNURBPatch_RegisterClass(void);
TQ3Status			E3GeometryNURBPatch_UnregisterClass(void);

TQ3GeometryObject	E3NURBPatch_New(const TQ3NURBPatchData *nurbPatchData);
TQ3Status			E3NURBPatch_Submit(const TQ3NURBPatchData *nurbPatchData, TQ3ViewObject theView);
TQ3Status			E3NURBPatch_SetData(TQ3GeometryObject nurbPatch, const TQ3NURBPatchData *nurbPatchData);
TQ3Status			E3NURBPatch_GetData(TQ3GeometryObject nurbPatch, TQ3NURBPatchData *nurbPatchData);
TQ3Status			E3NURBPatch_SetControlPoint(TQ3GeometryObject nurbPatch, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, const TQ3RationalPoint4D *point4D);
TQ3Status			E3NURBPatch_GetControlPoint(TQ3GeometryObject nurbPatch, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, TQ3RationalPoint4D *point4D);
TQ3Status			E3NURBPatch_SetUKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float knotValue);
TQ3Status			E3NURBPatch_SetVKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float knotValue);
TQ3Status			E3NURBPatch_GetUKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float *knotValue);
TQ3Status			E3NURBPatch_GetVKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float *knotValue);
TQ3Status			E3NURBPatch_EmptyData(TQ3NURBPatchData *nurbPatchData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


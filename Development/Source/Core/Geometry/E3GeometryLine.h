/*  NAME:
        E3GeometryLine.h

    DESCRIPTION:
        Header file for E3GeometryLine.c.

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
#ifndef __E3GEOMETRY_LINE__
#define __E3GEOMETRY_LINE__
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
TQ3Status			E3GeometryLine_RegisterClass(void);
TQ3Status			E3GeometryLine_UnregisterClass(void);

TQ3GeometryObject	E3Line_New(const TQ3LineData *lineData);
TQ3Status			E3Line_Submit(const TQ3LineData *lineData, TQ3ViewObject theView);
TQ3Status			E3Line_GetData(TQ3GeometryObject theLine, TQ3LineData *lineData);
TQ3Status			E3Line_SetData(TQ3GeometryObject theLine, const TQ3LineData *lineData);
TQ3Status			E3Line_GetVertexPosition(TQ3GeometryObject theLine, unsigned long index, TQ3Point3D *position);
TQ3Status			E3Line_SetVertexPosition(TQ3GeometryObject theLine, unsigned long index, const TQ3Point3D *position);
TQ3Status			E3Line_GetVertexAttributeSet(TQ3GeometryObject theLine, unsigned long index, TQ3AttributeSet *attributeSet);
TQ3Status			E3Line_SetVertexAttributeSet(TQ3GeometryObject theLine, unsigned long index, TQ3AttributeSet attributeSet);
TQ3Status			E3Line_EmptyData(TQ3LineData *lineData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        E3Geometry.h

    DESCRIPTION:
        Header file for E3Geometry.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#ifndef E3GEOMETRY_HDR
#define E3GEOMETRY_HDR
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
TQ3Status			E3Geometry_RegisterClass(void);
TQ3Status			E3Geometry_UnregisterClass(void);

TQ3ObjectType		E3Geometry_GetType(TQ3GeometryObject theGeom);
TQ3Status			E3Geometry_GetAttributeSet(TQ3GeometryObject theGeom, TQ3AttributeSet *attributeSet);
TQ3Status			E3Geometry_SetAttributeSet(TQ3GeometryObject theGeom, TQ3AttributeSet attributeSet);
TQ3Status			E3Geometry_Submit(TQ3GeometryObject theGeom, TQ3ViewObject theView);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


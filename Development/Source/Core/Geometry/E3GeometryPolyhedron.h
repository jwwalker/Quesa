/*  NAME:
        E3GeometryPolyhedron.h

    DESCRIPTION:
        Header file for E3GeometryPolyhedron.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#ifndef E3GEOMETRY_POLYHEDRON_HDR
#define E3GEOMETRY_POLYHEDRON_HDR
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
TQ3Status			E3GeometryPolyhedron_RegisterClass(void);
TQ3Status			E3GeometryPolyhedron_UnregisterClass(void);

TQ3GeometryObject	E3Polyhedron_New(const TQ3PolyhedronData *polyhedronData);
TQ3Status			E3Polyhedron_Submit(const TQ3PolyhedronData *polyhedronData, TQ3ViewObject theView);
TQ3Status			E3Polyhedron_SetData(TQ3GeometryObject thePolyhedron, const TQ3PolyhedronData *polyhedronData);
TQ3Status			E3Polyhedron_GetData(TQ3GeometryObject thePolyhedron, TQ3PolyhedronData *polyhedronData);
TQ3Status			E3Polyhedron_EmptyData(TQ3PolyhedronData *polyhedronData);
TQ3Status			E3Polyhedron_GetVertexPosition(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, TQ3Point3D *point);
TQ3Status			E3Polyhedron_SetVertexPosition(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, const TQ3Point3D *point);
TQ3Status			E3Polyhedron_GetVertexAttributeSet(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, TQ3AttributeSet *attributeSet);
TQ3Status			E3Polyhedron_SetVertexAttributeSet(TQ3GeometryObject thePolyhedron, TQ3Uns32 index, TQ3AttributeSet attributeSet);
TQ3Status			E3Polyhedron_GetTriangleData(TQ3GeometryObject thePolyhedron, TQ3Uns32 triangleIndex, TQ3PolyhedronTriangleData *triangleData);
TQ3Status			E3Polyhedron_SetTriangleData(TQ3GeometryObject thePolyhedron, TQ3Uns32 triangleIndex, const TQ3PolyhedronTriangleData *triangleData);
TQ3Status			E3Polyhedron_GetEdgeData(TQ3GeometryObject thePolyhedron, TQ3Uns32 edgeIndex, TQ3PolyhedronEdgeData *edgeData);
TQ3Status			E3Polyhedron_SetEdgeData(TQ3GeometryObject thePolyhedron, TQ3Uns32 edgeIndex, const TQ3PolyhedronEdgeData *edgeData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


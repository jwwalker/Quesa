/*  NAME:
        E3GeometryTriMesh.h

    DESCRIPTION:
        Header file for E3GeometryTriMesh.c.

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
#ifndef E3GEOMETRY_TRIMESH_HDR
#define E3GEOMETRY_TRIMESH_HDR
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
TQ3Status			E3GeometryTriMesh_RegisterClass(void);
TQ3Status			E3GeometryTriMesh_UnregisterClass(void);

TQ3GeometryObject	E3TriMesh_New(const TQ3TriMeshData *triMeshData);
TQ3Status			E3TriMesh_Submit(const TQ3TriMeshData *triMeshData, TQ3ViewObject theView);
TQ3Status			E3TriMesh_SetData(TQ3GeometryObject triMesh, const TQ3TriMeshData *triMeshData);
TQ3Status			E3TriMesh_GetData(TQ3GeometryObject triMesh, TQ3TriMeshData *triMeshData);
TQ3Status			E3TriMesh_EmptyData(TQ3TriMeshData *triMeshData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


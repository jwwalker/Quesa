/*  NAME:
        E3GeometryDisk.h

    DESCRIPTION:
        Header file for E3GeometryDisk.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#ifndef E3GEOMETRY_DISK_HDR
#define E3GEOMETRY_DISK_HDR
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
TQ3Status			E3GeometryDisk_RegisterClass(void);
TQ3Status			E3GeometryDisk_UnregisterClass(void);

TQ3GeometryObject	E3Disk_New(const TQ3DiskData *diskData);
TQ3Status			E3Disk_Submit(const TQ3DiskData *diskData, TQ3ViewObject theView);
TQ3Status			E3Disk_SetData(TQ3GeometryObject theDisk, const TQ3DiskData *diskData);
TQ3Status			E3Disk_GetData(TQ3GeometryObject theDisk, TQ3DiskData *diskData);
TQ3Status			E3Disk_EmptyData(TQ3DiskData *diskData);
TQ3Status			E3Disk_SetOrigin(TQ3GeometryObject theDisk, const TQ3Point3D *origin);
TQ3Status			E3Disk_SetMajorRadius(TQ3GeometryObject theDisk, const TQ3Vector3D *majorRadius);
TQ3Status			E3Disk_SetMinorRadius(TQ3GeometryObject theDisk, const TQ3Vector3D *minorRadius);
TQ3Status			E3Disk_GetOrigin(TQ3GeometryObject theDisk, TQ3Point3D *origin);
TQ3Status			E3Disk_GetMajorRadius(TQ3GeometryObject theDisk, TQ3Vector3D *majorRadius);
TQ3Status			E3Disk_GetMinorRadius(TQ3GeometryObject theDisk, TQ3Vector3D *minorRadius);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        RS_APISurface.h

    DESCRIPTION:
        A procedural api for RayShade.

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
*/

#ifndef _RT_GEOMETRY_H_
#define _RT_GEOMETRY_H_

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
#else
	#include <QD3D.h>
#endif

#include "RT.h"
/******************************************************************************
 **																			 **
 **								Data Structure							     **
 **																			 **
 *****************************************************************************/
typedef enum TRT_TriangleType {
	kRTTriangleType_Flat,	/* Flat triangle, normals ignored */
	kRTTriangleType_Phong	/* Phong triangle, normals should be included */ 
} TRT_TriangleType;
/******************************************************************************
 **																			 **
 **							Submit functions							     **
 **																			 **
 *****************************************************************************/
TQ3Status RT_SubmitTriangle(
				TRTDrawContext		*inDrawContext,
				TRT_TriangleType	inTriangleType,
				TQ3Point3D			inVertices[3],
				TQ3Vector3D			inNormals[3],
				TQ3Param2D			inParams[3]);


#endif	/* _RT_H_ */ 
/*  NAME:
        RT_Geometry.c

    DESCRIPTION:
        RayShade surface handling

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
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "RSPrefix.h"

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
	#include <QuesaMath.h>
#else
	#include <QD3D.h>
	#include <QD3DMath.h>
#endif

#include "RT_Geometry.h"
#include "RT_DrawContext.h"

#include "libobj/geom.h"
#include "libobj/triangle.h"
#include "libcommon/vector.h"


/*===========================================================================*\
 *
 *	Routine:	rt_SubmitGeometry()
 *
 *	Comments:	Submits a geometry to the RayShade  database
 *
\*===========================================================================*/
static 
TQ3Status rt_SubmitGeometry(
				TRTDrawContext		*inDrawContext,
				Geom				*inGeom)
{
	if (!inGeom)
		return kQ3Failure;
		
	inGeom->texture = inDrawContext->currentTexture;
	inGeom->surf = inDrawContext->currentSurface;
	inGeom->prims = 1;
	
	inGeom->next = inDrawContext->objects;
	inDrawContext->objects = inGeom;
	
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	RT_SubmitEllipsoid()
 *
 *	Comments:	Submits a triangle to the RayShade database
 *
 *  ToDo: 	u and v clipping are disbled.
 *
\*===========================================================================*/
static TQ3Status
RT_SubmitEllipsoid(
				TRTDrawContext		* /*inDrawContext*/,
				TQ3Point3D			/*inPosition*/,
				TQ3Vector3D			/*inOrientation*/,
				TQ3Vector3D			/*inMajorRadius*/,
				TQ3Vector3D			/*inMinorRadius*/,
				float				/*inUMin*/,
				float				/*inUMax*/,
				float				/*inVMin*/,
				float				/*inVMax*/)
{
	return kQ3Failure;
}
/*===========================================================================*\
 *
 *	Routine:	RT_SubmitTriangle()
 *
 *	Comments:	Submits a triangle to the RayShade database
 *
\*===========================================================================*/
TQ3Status RT_SubmitTriangle(
				TRTDrawContext		*inDrawContext,
				TRT_TriangleType	inTriangleType,
				TQ3Point3D			inVertices[3],
				TQ3Vector3D			inNormals[3],
				TQ3Param2D			inParams[3])

{
	Vector	theVertices[3];
	Vector	theNormals[3];
	Vec2d	theParams[3];
	Geom	*theTriangle = NULL;
	TQ3Status result;
//&&
	CTexture *saveCurrentTexture;
//&&
	if ((inTriangleType == kRTTriangleType_Phong) && (inNormals == NULL))
		return kQ3Failure;
	
	for (int i = 0; i < 3; i++)
	{
		theVertices[i].x = inVertices[i].x;
		theVertices[i].y = inVertices[i].y;
		theVertices[i].z = inVertices[i].z;
	}
	
	if (inParams)
	{
		for (int i = 0; i < 3; i++)
		{
			theParams[i].u = inParams[i].u;
			theParams[i].v = inParams[i].v;
		}
	}
	/*
	 * Check if it's a degenerated triangle, if so then simply ignore:
	 */
	{
		Vector e[3];
		Vector ptmp;
		
		VecSub(theVertices[1], theVertices[0], &e[0]);
		VecSub(theVertices[2], theVertices[1], &e[1]);
		VecSub(theVertices[0], theVertices[2], &e[2]);	
		VecCross(&e[0], &e[1], &ptmp);
		if (VecNormalize(&ptmp) <  DBL_EPSILON)
		{
			
			return kQ3Success;
		}
		
		
	}
	
	if (inTriangleType == kRTTriangleType_Phong)
	{
		for (int i = 0; i < 3; i++)
		{
			theNormals[i].x = inNormals[i].x;
			theNormals[i].y = inNormals[i].y;
			theNormals[i].z = inNormals[i].z;
		}
		/*
		 * If any of the attached normals are 0 => ignore... 
		 */
		if ((VecNormalize(&theNormals[0]) < DBL_EPSILON) ||
			(VecNormalize(&theNormals[1]) < DBL_EPSILON) ||
			(VecNormalize(&theNormals[2]) < DBL_EPSILON))
		{
			return kQ3Success;
		} 
		if (inParams == NULL)
			theTriangle = GeomTriangleCreate(PHONGTRI,&theVertices[0],&theVertices[1],&theVertices[2],
		                   &theNormals[0],&theNormals[1],&theNormals[2],
		                   NULL,NULL,NULL,0);
		else
			theTriangle = GeomTriangleCreate(PHONGTRI,&theVertices[0],&theVertices[1],&theVertices[2],
		                   &theNormals[0],&theNormals[1],&theNormals[2],
		                   &theParams[0],&theParams[1],&theParams[2],0);				
	}
	else
	{
		if (inParams == NULL)
			theTriangle = GeomTriangleCreate(FLATTRI,&theVertices[0],&theVertices[1],&theVertices[2],
		                   	NULL,NULL,NULL,
		                   	NULL,NULL,NULL,0);
		else
			theTriangle = GeomTriangleCreate(FLATTRI,&theVertices[0],&theVertices[1],&theVertices[2],
		                   	NULL,NULL,NULL,
		                   	&theParams[0],&theParams[1],&theParams[2],0);					
	}
//&&
	saveCurrentTexture=	inDrawContext->currentTexture;
	if (inParams == NULL)
		inDrawContext->currentTexture=NULL;

//&&
	result = rt_SubmitGeometry(inDrawContext,theTriangle);
//&&
	inDrawContext->currentTexture=	saveCurrentTexture;
//&&
	
	return result;
}

/*  NAME:
        RT_Geometry.c

    DESCRIPTION:
        RayShade surface handling

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

/*  NAME:
        RS_Geometry.c

    DESCRIPTION:
        RayShade geometry handlers

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "RSPrefix.h"

#include "RSPlugin.h"
#include "RS_Attributes.h"

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
	#include <QuesaRenderer.h>
#else
	#include <QD3D.h>
	#include <QD3DRenderer.h>
#endif


#include "RT_Geometry.h"



#if RS_DEBUG
/*===========================================================================*\
 *
 *	Routine:	RS_Geometry_Triangle_QD()
 *
 *	Comments:	This is for debugging only draw the triangle as a wireframe
 *
\*===========================================================================*/
TQ3Status RS_Geometry_Triangle_QD(
						TQ3ViewObject 		pView,
						TRSRendererPrivate	*irsdata,
						TQ3GeometryObject	pGeom, 
						TQ3TriangleData		*pTriangleData)
{
	// local to Frustum
	float M00 = irsdata->localToFrustum.value[0][0];
	float M01 = irsdata->localToFrustum.value[0][1];
	float M02 = irsdata->localToFrustum.value[0][2];
	float M03 = irsdata->localToFrustum.value[0][3];

	float M10 = irsdata->localToFrustum.value[1][0];
	float M11 = irsdata->localToFrustum.value[1][1];
	float M12 = irsdata->localToFrustum.value[1][2];
	float M13 = irsdata->localToFrustum.value[1][3];

	float M20 = irsdata->localToFrustum.value[2][0];
	float M21 = irsdata->localToFrustum.value[2][1];
	float M22 = irsdata->localToFrustum.value[2][2];
	float M23 = irsdata->localToFrustum.value[2][3];

	float M30 = irsdata->localToFrustum.value[3][0];
	float M31 = irsdata->localToFrustum.value[3][1];
	float M32 = irsdata->localToFrustum.value[3][2];
	float M33 = irsdata->localToFrustum.value[3][3];
	
	// Triangle data
	
	TQ3DrawContextObject DrawContext ;
	if ( Q3View_GetDrawContext ( pView , &DrawContext ) != kQ3Failure )
		{
		CWindowPtr Window ;
		if ( Q3MacDrawContext_GetWindow ( DrawContext , &Window ) != kQ3Failure )
			{
			TQ3Point2D point2D [ 3 ] ;
			long j;
            RGBColor TheColour;
            
			for ( j = 0 ; j < 3 ; ++j )
				{
				TQ3Point3D point3D = pTriangleData->vertices [ j ].point;
				float w;
				
				point2D [ j ].x = point3D.x * M00 + point3D.y * M10 + point3D.z * M20 + M30 ;
				point2D [ j ].y = point3D.x * M01 + point3D.y * M11 + point3D.z * M21 + M31 ;

				w = 1.0f / ( point3D.x * M03 + point3D.y * M13 + point3D.z * M23 + M33 ) ;
			
				point2D [ j ].x = irsdata->XOffset + point2D [ j ].x * w * irsdata->XScale ;
				point2D [ j ].y = irsdata->YOffset - point2D [ j ].y * w * irsdata->YScale ;
				} ;
			SetPort ( ( GrafPort* ) ( Window ) ) ;
			TheColour = ColourFromAttrSet ( pTriangleData->triangleAttributeSet ) ;
			RGBForeColor ( &TheColour ) ;
			MoveTo ( point2D [ 0 ].x , point2D [ 0 ].y ) ;
			LineTo ( point2D [ 1 ].x , point2D [ 1 ].y ) ;
			LineTo ( point2D [ 2 ].x , point2D [ 2 ].y ) ;
			LineTo ( point2D [ 0 ].x , point2D [ 0 ].y ) ;
			}	
		}
	}
}
#endif /* RS_DEBUG */

/*===========================================================================*\
 *
 *	Routine:	RS_Geometry_Triangle()
 *
 *	Comments:	
 *
\*===========================================================================*/
TQ3Status RS_Geometry_Triangle(
						TQ3ViewObject 				pView,
						TRSRendererPrivate			*rsPrivate,
						TQ3GeometryObject			/*pGeom*/, 
						const TQ3TriangleData		*pTriangleData)
{
	TQ3Status			theStatus;     
	/*
	 * Update the attributes if neccesary
	 */
	theStatus = RS_UpdateAttributes(pView,rsPrivate,pTriangleData->triangleAttributeSet);
	if (theStatus != kQ3Success)
		return theStatus;
	
	/*
	 * Submit the triangle to the raytracer
	 */
	{ 
	    TQ3Point3D      	theVertices[3];
	    TQ3Vector3D     	theNormals[3];
	    TQ3Param2D   	 	theParams[3];
	    TRT_TriangleType	theTriangleType = kRTTriangleType_Phong;
	    TQ3Boolean			theTextureParams = kQ3True;
	   
	    for (int j = 0 ; j < 3 ; ++j )
		{
			TQ3Point3D 			thePoint3D;
			TQ3Vector3D 		theNormal3D;
			TQ3Param2D			theParam2D;
			TQ3XAttributeMask	theAttributeMask;	
			
			thePoint3D 			= pTriangleData->vertices[j].point;
			theAttributeMask 	= Q3XAttributeSet_GetMask(pTriangleData->vertices[j].attributeSet);
			
			if (theTriangleType == kRTTriangleType_Phong)
			{
				if (theAttributeMask & kQ3XAttributeMaskNormal)
				{
					Q3AttributeSet_Get(pTriangleData->vertices[j].attributeSet,kQ3AttributeTypeNormal,&theNormal3D ); 
					Q3Vector3D_Transform(&theNormal3D,&(rsPrivate->localToWorld),&theNormals[j]);	
				}
				else
					theTriangleType = kRTTriangleType_Flat;
			}
			if (theTextureParams)
			{
				if (theAttributeMask & kQ3XAttributeMaskSurfaceUV)
				{
					
					Q3AttributeSet_Get(pTriangleData->vertices[j].attributeSet,kQ3AttributeTypeSurfaceUV,&theParam2D); 
					Q3Param2D_Transform(&theParam2D,&rsPrivate->uvTransform,&theParams[j]);
				}
				else
					if (theAttributeMask & kQ3XAttributeMaskShadingUV)
					{

					Q3AttributeSet_Get(pTriangleData->vertices[j].attributeSet,kQ3AttributeTypeShadingUV,&theParam2D); 
					Q3Param2D_Transform(&theParam2D,&rsPrivate->uvTransform,&theParams[j]);
					}
					else
						theTextureParams = kQ3False;
			}
			
			Q3Point3D_Transform(&thePoint3D,&(rsPrivate->localToWorld),&theVertices[j]);
		}
		
		theStatus = RT_SubmitTriangle(rsPrivate->raytracer,theTriangleType,theVertices,theNormals,
			(theTextureParams ? theParams : NULL));
		if (theStatus != kQ3Success)
			return theStatus;
	}   

	return kQ3Success;
}

/*===========================================================================*\
 *
 *	Routine	:	RS_Geometry_Marker()
 *
 *	Comment	:	We are ignoring markers, maybe we could cache them and render 
 *				in the end of the ray tracing. 
 *
\*===========================================================================*/
TQ3Status RS_Geometry_Marker(
	TQ3ViewObject 			/*view*/, 
	TRSRendererPrivate		* /*rsPrivate*/,
	TQ3GeometryObject 		/*marker*/,
	const TQ3MarkerData		* /*markerData*/)
{
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine	:	RS_Geometry_Marker()
 *
 *	Comment	:	We are ignoring markers, maybe we could cache them and render 
 *				in the end of the ray tracing. 
 *
\*===========================================================================*/
TQ3Status RS_Geometry_PixmapMarker(
	TQ3ViewObject 				/*view*/, 
	TRSRendererPrivate			* /*srPrivate*/,
	TQ3GeometryObject 			/*pixmapMarker*/,
	const TQ3PixmapMarkerData	* /*pixmapMarkerData*/)
{
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	RS_Geometry_Point()
 *
 *	Comments:	Points are meaningless in a ray tracer. Or we can should draw 
 *		a small sphere here?!
 *
\*===========================================================================*/
TQ3Status RS_Geometry_Point(
	TQ3ViewObject 			/*view*/, 
	TRSRendererPrivate		* /*rsPrivate*/,
	TQ3GeometryObject 		/*point*/,
	const TQ3PointData		* /*pointData*/)
{
	return kQ3Success;
}
/*===========================================================================*\
 *
 *	Routine:	RS_Geometry_Point()
 *
 *	Comments:	Lines are meaningless in a ray tracer. Or we can should draw 
 *		a thin cilinder here?!
 *
\*===========================================================================*/
TQ3Status RS_Geometry_Line(
	TQ3ViewObject 			/*view*/, 
	TRSRendererPrivate		* /*rsPrivate*/,
	TQ3GeometryObject 		/*line*/,
	const TQ3LineData		* /*lineData*/)
{
	return kQ3Success;
}


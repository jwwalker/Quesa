/*  NAME:
        RSPlugin.h

    DESCRIPTION:
        RayShade plugin renderer for QD3D/Quesa

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
#ifndef _RSPLUGIN_H_
#define _RSPLUGIN_H_

#include "RSPrefix.h"

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
	#include <QuesaGeometry.h>
#else
	#include <QD3D.h>
	#include <QD3DGeometry.h>
#endif

#include "RT.h"
#include "RT_Surface.h"


/******************************************************************************
 **																			 **
 **								Data Structure							     **
 **																			 **
 *****************************************************************************/
typedef
struct TRSRendererPrivate
{
	TQ3Matrix4x4 	localToWorld;

    TRTDrawContext	*raytracer;
    /* 
     * The cache of the current surface.
     */
    TRTSurfaceData  surfaceCache;
    TQ3Boolean		isCurrentSurface;
    
    /*
     * The current texture map.
     */
    TQ3Matrix3x3	uvTransform;
    
    /*
     * Rasteriser informations.
     */

#if RS_DEBUG
	TQ3Matrix4x4 	localToFrustum;
	float 			XScale;
	float 			YScale;
	float 			XOffset;
	float 			YOffset;
#endif
    
} TRSRendererPrivate;


/******************************************************************************
 **																			 **
 **							State Update Routines						     **
 **																			 **
 *****************************************************************************/
/*
 *	Matrices
 */
TQ3Status RS_Update_LocalToWorldMatrix(
	TQ3ViewObject 			view,
	TRSRendererPrivate		*srPrivate,
	TQ3Matrix4x4			*localToWorld);
TQ3Status RS_Update_SurfaceShader(
				TQ3ViewObject 			view,
				TRSRendererPrivate		*rsPrivate,
				TQ3Object 				*shaderObject);
#if RS_DEBUG
TQ3Status RS_Update_LocalToFrustumMatrix(
	TQ3ViewObject 			view,
	TRSRendererPrivate		*srPrivate,
	TQ3Matrix4x4			*localToFrustum);
#endif
/******************************************************************************
 **																			 **
 **						Entry Points into Renderer						     **
 **																			 **
 *****************************************************************************/
TQ3Status RS_Geometry_Triangle(
	TQ3ViewObject 				view, 
	TRSRendererPrivate			*srPrivate,
	TQ3GeometryObject 			triangle, 
	const TQ3TriangleData		*triangleData);
		
TQ3Status RS_Geometry_Line(
	TQ3ViewObject 				view, 
	TRSRendererPrivate			*srPrivate,
	TQ3GeometryObject 			line,
	const TQ3LineData			*lineData);

TQ3Status RS_Geometry_Point(
	TQ3ViewObject 				view, 
	TRSRendererPrivate			*srPrivate,
	TQ3GeometryObject 			point,
	const TQ3PointData			*pointData);

TQ3Status RS_Geometry_Marker(
	TQ3ViewObject 				view, 
	TRSRendererPrivate			*srPrivate,
	TQ3GeometryObject 			marker,
	const TQ3MarkerData			*markerData);

TQ3Status RS_Geometry_PixmapMarker(
	TQ3ViewObject 				view, 
	TRSRendererPrivate			*srPrivate,
	TQ3GeometryObject 			pixmapMarker,
	const TQ3PixmapMarkerData	*pixmapMarkerData);
	

	


#endif /* _RSPLUGIN_H_ */
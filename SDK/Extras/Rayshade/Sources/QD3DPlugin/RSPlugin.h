/*  NAME:
        RSPlugin.h

    DESCRIPTION:
        RayShade plugin renderer for QD3D/Quesa

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
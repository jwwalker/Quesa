/*  NAME:
        GLShadowVolumeManager.h

    DESCRIPTION:
        Header file for GLShadowVolumeManager.cpp.
       
    REMARKS:
    	Do not call these functions without having verified that the current
    	OpenGL renderer supports vertex buffer objects.  You can do that using
    	GLUtils_CheckExtensions.

    COPYRIGHT:
        Copyright (c) 2011-2014, Quesa Developers. All rights reserved.

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
    ___________________________________________________________________________
*/
#ifndef GLSHADOWVOLMANAGER_HDR
#define GLSHADOWVOLMANAGER_HDR

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "GLPrefix.h"
#include "GLVBOManager.h"


//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
namespace ShadowVolMgr
{

/*!
	@function	StartFrame
	@abstract	Update the limit on memory that can be used in this cache.
	@param			glContext		An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
	@param			memLimitK		New memory limit in K-bytes.
*/
void			StartFrame(			TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3Uns32 memLimitK );

/*!
	@function	RenderShadowVolume
	@abstract	Look for a cached shadow volume for a given geometry, OpenGL
				context, and shadow-casting light.  If we find one, render it.
	@discussion		If we find the object in the cache, but the cached object
					is stale, we delete it from the cache and return false.
	@param			glContext		An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
	@param			inGeom			A geometry object.
	@param			inLight			A light object.
	@param			inLocalLightPos	The position of the light in local coordinates.
	@result			True if the object was found and rendered.
*/
TQ3Boolean		RenderShadowVolume(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos );

/*!
	@function	AddShadowVolume
	@abstract	Add a shadow volume mesh to the cache.  Do not call this unless
				RenderCachedShadowVolume has just returned false.
	@param			glContext			An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
	@param			inGeom				A geometry object.
	@param			inLight				A light object.
	@param			inLocalLightPos		The position of the light in local coordinates.
	@param			inNumPoints			Number of points (vertices).
	@param			inPoints			Array of point locations.
	@param			inNumTriIndices		Number of triangle vertex indices to follow.
	@param			inNumQuadIndices	Number of quad vertex indices to follow.
	@param			inVertIndices		Array of vertex indices (triangle, then quad).
*/
void			AddShadowVolume(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3GeometryObject inGeom,
									TQ3LightObject inLight,
									const TQ3RationalPoint4D& inLocalLightPos,
									TQ3Uns32 inNumPoints,
									const TQ3RationalPoint4D* inPoints,
									TQ3Uns32 inNumTriIndices,
									TQ3Uns32 inNumQuadIndices,
									const GLuint* inVertIndices );


/*!
	@function		Flush
	@abstract		Delete any cached VBOs for geometries that are no longer
					referenced elsewhere, or lights that are no longer
					referenced elsewhere.
	@param			glContext		An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
	@param			inRenderer		A Quesa renderer.
*/
void				Flush(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3RendererObject inRenderer );

/*!
	@function		CountVBOs
	@abstract		Count how many references the shadow VBO manager holds for a given
					geometry, counting all GL contexts and all lights.
	@param			inGeom			A geometry object.
	@result			Number of VBOs referencing the geometry.
*/
TQ3Uns32			CountVBOs( TQ3GeometryObject inGeom );


}

#endif

/*  NAME:
        GLVBOManager.h

    DESCRIPTION:
        Header file for GLVBOManager.cpp.
       
    REMARKS:
    	Do not call these functions without having verified that the current
    	OpenGL renderer supports vertex buffer objects.  You can do that using
    	GLUtils_CheckExtensions.

    COPYRIGHT:
        Copyright (c) 2007-2008, Quesa Developers. All rights reserved.

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
#ifndef GLVBOMANAGER_HDR
#define GLVBOMANAGER_HDR

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "GLPrefix.h"
#include "QuesaStyle.h"



//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------

/*!
	@function		RenderCachedVBO
	@abstract		Look for a cached VBO for the given geometry and OpenGL
					context.  If we find one, render it.
	@discussion		If we find the object in the cache, but the cached object
					is stale, we delete it from the cache and return false.
					
					The caller should have activated the GL context and called
					glEnableClientState to enable or disable arrays as
					appropriate.
	@param			glContext		An OpenGL context.
	@param			inGeom			A geometry object.
	@param			inFillStyle		Current fill Style.
	@result			True if the object was found and rendered.
*/
TQ3Boolean			RenderCachedVBO(
									TQ3GLContext glContext,
									TQ3GeometryObject inGeom,
									TQ3FillStyle inFillStyle );

/*!
	@function		AddVBOToCache
	@abstract		Add VBO data to the cache.  Do not call this unless
					RenderCachedVBO has just returned false.
	@param			glContext		An OpenGL context.
	@param			inGeom			A geometry object.
	@param			inNumPoints		Number of points (vertices).
	@param			inPoints		Array of point locations.
	@param			inNormals		Array of normal vectors (or NULL).
	@param			inColors		Array of vertex colors (or NULL).
	@param			inUVs			Array of vertex UV coordinates (or NULL).
	@param			inMode			OpenGL mode, e.g., GL_TRIANGLES.
	@param			inNumIndices	Number of vertex indices to follow.
	@param			inIndices		Array of vertex indices.
*/
void				AddVBOToCache(
									TQ3GLContext glContext,
									TQ3GeometryObject inGeom,
									TQ3Uns32 inNumPoints,
									const TQ3Point3D* inPoints,
									const TQ3Vector3D* inNormals,
									const TQ3ColorRGB* inColors,
									const TQ3Param2D* inUVs,
									GLenum inMode,
									TQ3Uns32 inNumIndices,
									const TQ3Uns32* inIndices );

/*!
	@function		FlushVBOCache
	@abstract		Delete any cached VBOs for geometries that are no longer
					referenced elsewhere.
	@param			glContext		An OpenGL context.
*/
void				FlushVBOCache(
									TQ3GLContext glContext );


//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif


#endif

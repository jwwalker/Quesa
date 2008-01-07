/*  NAME:
        GLDisplayListManager.h

    DESCRIPTION:
        Header file for GLDisplayListManager.cpp.
       
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
#ifndef GLDISPLAYLISTMANAGER_HDR
#define GLDISPLAYLISTMANAGER_HDR

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
	@function		RenderCachedDisplayList
	@abstract		Look for a cached display list for the given geometry and
					OpenGL context.  If we find one, render it.
	@discussion		If we find the object in the cache, but the cached object
					is stale, we delete it from the cache and return false.
					
					The caller should have activated the GL context.
	@param			glContext		An OpenGL context.
	@param			inGeom			A geometry object.
	@param			inFillStyle		Current fill Style.
	@result			True if the object was found and rendered.
*/
TQ3Boolean			RenderCachedDisplayList(
									TQ3GLContext glContext,
									TQ3GeometryObject inGeom,
									TQ3FillStyle inFillStyle );

/*!
	@function		CacheDisplayList
	@abstract		Add a display list to the cache.
	@discussion		If RenderCachedDisplayList returns kQ3False, the caller
					should create a new display list as follows:
					
					<blockquote><pre><code>
					GLuint	displayListID = glGenLists( 1 );
					glNewList( displayListID, GL_COMPILE );
					
					IssueSomeRenderingCommands();
					
					glEndList();
					CacheDisplayList( displayListID, glContext, geom );
					</code></pre></blockquote>
					
					and finally call RenderCachedDisplayList again.
	@param			inDisplayList	A display list name.
	@param			glContext		An OpenGL context.
	@param			inGeom			A geometry object.
	@param			inMode			OpenGL mode, e.g., GL_TRIANGLES.
*/
void				CacheDisplayList(
									GLuint inDisplayList,
									TQ3GLContext glContext,
									TQ3GeometryObject inGeom,
									GLenum inMode );


/*!
	@function		FlushDisplayListCache
	@abstract		Delete any cached display lists for geometries that are no
					longer referenced elsewhere.
	@param			glContext		An OpenGL context.
*/
void				FlushDisplayListCache(
									TQ3GLContext glContext );


//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif


#endif

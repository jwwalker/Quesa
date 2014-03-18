/*  NAME:
        GLVBOManager.h

    DESCRIPTION:
        Header file for GLVBOManager.cpp.
       
    REMARKS:
    	Do not call these functions without having verified that the current
    	OpenGL renderer supports vertex buffer objects.  You can do that using
    	GLUtils_CheckExtensions.

    COPYRIGHT:
        Copyright (c) 2007-2014, Quesa Developers. All rights reserved.

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

#ifndef GL_PROC_TYPE
	#if QUESA_OS_WIN32
		#define GL_PROC_TYPE	__stdcall
	#else
		#define GL_PROC_TYPE
	#endif
#endif


//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "GLPrefix.h"
#include "QuesaStyle.h"
#include <cstddef>


//=============================================================================
//      Types
//-----------------------------------------------------------------------------

#ifndef GL_ARB_vertex_buffer_object
    typedef std::ptrdiff_t GLintptrARB;
    typedef std::ptrdiff_t GLsizeiptrARB;
#endif

typedef void (GL_PROC_TYPE * BindBufferProcPtr) (GLenum target,
												GLuint buffer);
typedef void (GL_PROC_TYPE * DeleteBuffersProcPtr) (GLsizei n,
												const GLuint *buffers);
typedef void (GL_PROC_TYPE * GenBuffersProcPtr) (GLsizei n,
												GLuint *buffers);
typedef GLboolean (GL_PROC_TYPE * IsBufferProcPtr) (GLuint buffer);
typedef void (GL_PROC_TYPE * BufferDataProcPtr) (GLenum target,
												GLsizeiptrARB size,
												const GLvoid *data,
												GLenum usage);
typedef void (GL_PROC_TYPE * BufferSubDataProcPtr) (GLenum target,
												GLintptrARB offset,
												GLsizeiptrARB size,
												const GLvoid *data);
typedef void (GL_PROC_TYPE * ClientActiveTextureProcPtr)( GLenum unit );
typedef void (GL_PROC_TYPE * MultiTexCoord1fProcPtr)( GLenum unit, GLfloat s );
typedef void (GL_PROC_TYPE * GetBufferParameterivProcPtr)(GLenum target, GLenum value, GLint * data);


struct GLBufferFuncs
{
			GLBufferFuncs();

	void	Initialize( const TQ3GLExtensions& inExts );
	void	InitializeForDelete();
	
	GenBuffersProcPtr			glGenBuffersProc;
	BindBufferProcPtr			glBindBufferProc;
	DeleteBuffersProcPtr		glDeleteBuffersProc;
	IsBufferProcPtr				glIsBufferProc;
	BufferDataProcPtr			glBufferDataProc;
	BufferSubDataProcPtr		glBufferSubDataProc;
	ClientActiveTextureProcPtr	glClientActiveTextureProc;
	MultiTexCoord1fProcPtr		glMultiTexCoord1fProc;
	GetBufferParameterivProcPtr	glGetBufferParameterivProc;

private:
			GLBufferFuncs( const GLBufferFuncs& inOther );
};


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
	@function		UpdateVBOCacheLimit
	@abstract		Update the limit on memory that can be used in this cache.
	@param			glContext		An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
	@param			inMaxMemK		New memory limit in K-bytes.
*/
void				UpdateVBOCacheLimit(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3Uns32 inMaxMemK );

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
	@param			inFuncs			OpenGL buffer function pointers.
	@param			inGeom			A geometry object.
	@param			inMode			OpenGL mode, e.g., GL_TRIANGLES.
	@result			True if the object was found and rendered.
*/
TQ3Boolean			RenderCachedVBO(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs,
									TQ3GeometryObject inGeom,
									GLenum inMode );

/*!
	@function		AddVBOToCache
	@abstract		Add VBO data to the cache.  Do not call this unless
					RenderCachedVBO has just returned false.
	@param			glContext		An OpenGL context.
	@param			inFuncs			OpenGL buffer function pointers.
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
									const GLBufferFuncs& inFuncs,
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
	@param			inFuncs			OpenGL buffer function pointers.
*/
void				FlushVBOCache(
									TQ3GLContext glContext,
									const GLBufferFuncs& inFuncs );


/*!
	@function		CountVBOs
	@abstract		Count how many references the VBO manager holds for a given
					geometry, counting all GL contexts and all modes.
	@param			inGeom			A geometry object.
	@result			Number of VBOs referencing the geometry.
*/
TQ3Uns32			CountVBOs( TQ3GeometryObject inGeom );

//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif


#endif

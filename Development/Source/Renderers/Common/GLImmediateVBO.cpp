/*  NAME:
        GLImmediateVBO.cpp

    DESCRIPTION:
        Quesa OpenGL vertex buffer object for immediate use.
       
    COPYRIGHT:
        Copyright (c) 2018-2019, Quesa Developers. All rights reserved.

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
#include "GLImmediateVBO.h"

#include "GLGPUSharing.h"
#include "GLUtils.h"
#include "QORenderer.h"

#ifndef GL_ARRAY_BUFFER
	#define GL_ARRAY_BUFFER					0x8892
	#define GL_ELEMENT_ARRAY_BUFFER			0x8893
	#define GL_STATIC_DRAW                  0x88E4
	#define GL_DYNAMIC_DRAW                 0x88E8
#endif


namespace
{
	const TQ3Uns32	kImmediateVBOCacheKey	= Q3_FOUR_CHARACTER_CONSTANT('v', 'b', 'o', 'i');
	
	const GLsizeiptr	kAbsentBuffer	= -1;
	
	class ImmedVBOCache : public CQ3GPSharedCache
	{
	public:
						ImmedVBOCache();
						~ImmedVBOCache();
						
		void			InitBuffers( const QORenderer::GLFuncs& inFuncs );
		void			UpdateBufferSizes( const QORenderer::GLFuncs& inFuncs,
											GLsizeiptr inArrayDataSize,
											GLsizeiptr inElementDataSize );
		
		void			BindArrayBuffer( const QORenderer::GLFuncs& inFuncs ) const;
		void			BindElementBuffer( const QORenderer::GLFuncs& inFuncs ) const;
		
	private:
		GLuint			_GLBufferNames[2];	// 0 is array, 1 is element
		GLsizeiptr		_GLBufferSize[2];
	};
}

static ImmedVBOCache* GetVBOCache( TQ3GLContext glContext )
{
	ImmedVBOCache*	theCache = static_cast<ImmedVBOCache*>( GLGPUSharing_GetCache(
		glContext, kImmediateVBOCacheKey ) );
	
	if (theCache == nullptr)
	{
		theCache = new(std::nothrow) ImmedVBOCache;
		
		if (theCache != nullptr)
		{
			//Q3_MESSAGE_FMT("Created kImmediateVBOCacheKey %p for GL context %p", theCache, glContext);
			GLGPUSharing_AddCache( glContext, kImmediateVBOCacheKey, theCache );
		}
	}
	
	return theCache;
}



ImmedVBOCache::ImmedVBOCache()
{
	_GLBufferNames[0] = _GLBufferNames[1] = 0;
	_GLBufferSize[0] = _GLBufferSize[1] = 0;
}

ImmedVBOCache::~ImmedVBOCache()
{
	QORenderer::GLFuncs funcs;
	funcs.InitializeForBufferDelete();
	(*funcs.glDeleteBuffersProc)( 2, _GLBufferNames );
}

void	ImmedVBOCache::InitBuffers( const QORenderer::GLFuncs& inFuncs )
{
	if (_GLBufferNames[0] == 0)
	{
		(*inFuncs.glGenBuffersProc)( 2, _GLBufferNames );
	}
}

void	ImmedVBOCache::UpdateBufferSizes( const QORenderer::GLFuncs& inFuncs,
										GLsizeiptr inArrayDataSize,
										GLsizeiptr inElementDataSize )
{
	if (inArrayDataSize > _GLBufferSize[0])
	{
		(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, _GLBufferNames[0] );
		_GLBufferSize[0] = inArrayDataSize * 2;
		(*inFuncs.glBufferDataProc)( GL_ARRAY_BUFFER, _GLBufferSize[0], nullptr,
			GL_DYNAMIC_DRAW );
	}
	
	if (inElementDataSize > _GLBufferSize[1])
	{
		(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, _GLBufferNames[1] );
		_GLBufferSize[1] = inElementDataSize * 2;
		(*inFuncs.glBufferDataProc)( GL_ELEMENT_ARRAY_BUFFER, _GLBufferSize[1], nullptr,
			GL_DYNAMIC_DRAW );
	}
}

void	ImmedVBOCache::BindArrayBuffer( const QORenderer::GLFuncs& inFuncs ) const
{
	(*inFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, _GLBufferNames[0] );
}

void	ImmedVBOCache::BindElementBuffer( const QORenderer::GLFuncs& inFuncs ) const
{
	(*inFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, _GLBufferNames[1] );
}

#pragma mark -

/*!
	@function	RenderImmediateVBO
	
	@abstract	Copy some data into a VBO and immmediately render it,
				on a one-shot basis.
	
	@discussion	If you pass NULL for inIndices, then the rendering will be
				done using glDrawArrays and inNumIndices will be ignored,
				otherwise rendering will be done using glDrawElements.
	
	@param		inMode			What kind of primitives to render.
	@param		inRenderer		A renderer.
	@param		inNumPoints		Number of points.
	@param		inPoints		Array of points.
	@param		inNormals		Array of per-vertex normals, or NULL.
	@param		inColors		Array of per-vertex colors, or NULL.
	@param		inUVs			Array of texture UV coordinates, or NULL.
	@param		inNumIndices	Number of indices in the following array.
	@param		inIndices		Array of point indices.  May be NULL,
								see discussion.
*/
void			RenderImmediateVBO(
									GLenum inMode,
									const QORenderer::Renderer& inRenderer,
									TQ3Uns32 inNumPoints,
									const TQ3Point3D* inPoints,
									const TQ3Vector3D* inNormals,
									const TQ3ColorRGB* inColors,
									const TQ3Param2D* inUVs,
									TQ3Uns32 inNumIndices,
									const TQ3Uns32* inIndices )
{
	ImmedVBOCache* theCache = GetVBOCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		theCache->InitBuffers( inRenderer.Funcs() );
		
		// Compute sub-buffer sizes for array data
		GLsizeiptr	vertexDataSize = static_cast<GLsizeiptr>(inNumPoints * sizeof(TQ3Point3D));
		GLsizeiptr	normalDataSize = (inNormals == nullptr)? 0 :
			static_cast<GLsizeiptr>(inNumPoints * sizeof(TQ3Vector3D));
		GLsizeiptr	colorDataSize = (inColors == nullptr)? 0 :
			static_cast<GLsizeiptr>(inNumPoints * sizeof(TQ3ColorRGB));
		GLsizeiptr	uvDataSize = (inUVs == nullptr)? 0 :
			static_cast<GLsizeiptr>(inNumPoints * sizeof(TQ3Param2D));
		GLsizeiptr	totalArrayDataSize = vertexDataSize + normalDataSize +
			colorDataSize + uvDataSize;
		
		// Compute sub-buffer offsets
		GLsizeiptr normalOffset = (normalDataSize == 0)? kAbsentBuffer : vertexDataSize;
		GLsizeiptr colorOffset = (colorDataSize == 0)? kAbsentBuffer : vertexDataSize + normalDataSize;
		GLsizeiptr uvOffset = (uvDataSize == 0)? kAbsentBuffer : vertexDataSize + normalDataSize + colorDataSize;

		// Buffer size for element data
		if (inIndices == nullptr)
		{
			inNumIndices = 0;
		}
		GLsizeiptr	elementDataSize = inNumIndices * sizeof(TQ3Uns32);
		
		theCache->UpdateBufferSizes( inRenderer.Funcs(), totalArrayDataSize, elementDataSize );
		
		// Copy the data into the array VBO
		theCache->BindArrayBuffer( inRenderer.Funcs() );
		const GLvoid *	dataAddr;
		dataAddr = inPoints;
		(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER, 0,
			vertexDataSize, dataAddr );
		CHECK_GL_ERROR;
		if (inNormals != nullptr)
		{
			dataAddr = inNormals;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER,
				normalOffset, normalDataSize, dataAddr );
			CHECK_GL_ERROR;
		}
		if (inColors != nullptr)
		{
			dataAddr = inColors;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER,
				colorOffset, colorDataSize, dataAddr );
			CHECK_GL_ERROR;
		}
		if (inUVs != nullptr)
		{
			dataAddr = inUVs;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER,
				uvOffset, uvDataSize, dataAddr );
			CHECK_GL_ERROR;
		}
			
		// Associate vertex data pointers with sub-buffers
		inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mVertexAttribLoc,
			3, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( 0U ) );
		if (inNormals != nullptr)
		{
			inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mNormalAttribLoc,
				3, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( normalOffset ) );
		}
		if (inColors != nullptr)
		{
			inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mColorAttribLoc,
				3, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( colorOffset ) );
		}
		if (inUVs != nullptr)
		{
			inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mTexCoordAttribLoc,
				2, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( uvOffset ) );
		}
		
		if (inIndices == nullptr)
		{
			(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
			glDrawArrays( inMode, 0, inNumPoints );
		}
		else
		{
			// Copy data into element VBO
			theCache->BindElementBuffer( inRenderer.Funcs() );
			CHECK_GL_ERROR;
			dataAddr = inIndices;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ELEMENT_ARRAY_BUFFER,
				0, elementDataSize, dataAddr );
			CHECK_GL_ERROR;
			
			// Draw the elements
			glDrawElements( inMode, inNumIndices, GL_UNSIGNED_INT, GLBufferObPtr( 0U ) );
		}
		
		(*inRenderer.Funcs().glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
		(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
}

/*!
	@function	RenderImmediateTransVBO
	
	@abstract	Copy some data into a VBO and immmediately render it,
				on a one-shot basis.
	
	@discussion	If you pass NULL for inIndices, then the rendering will be
				done using glDrawArrays and inNumIndices will be ignored,
				otherwise rendering will be done using glDrawElements.
	
	@param		inMode			What kind of primitives to render.
	@param		inRenderer		A renderer.
	@param		inNumPoints		Number of points.
	@param		inPoints		Array of points.
	@param		inNormals		Array of per-vertex normals, or NULL.
	@param		inColors		Array of per-vertex colors, or NULL.
	@param		inUVs			Array of texture UV coordinates, or NULL.
	@param		inNumIndices	Number of indices in the following array.
	@param		inIndices		Array of point indices.  May be NULL,
								see discussion.
*/
void			RenderImmediateTransVBO(
									GLenum inMode,
									const QORenderer::Renderer& inRenderer,
									TQ3Uns32 inNumPoints,
									const TQ3Point3D* _Nonnull inPoints,
									const TQ3Vector3D* _Nullable inNormals,
									const TQ3ColorRGBA* _Nullable inColors,
									const TQ3Param2D* _Nullable inUVs,
									TQ3Uns32 inNumIndices,
									const TQ3Uns32* _Nullable inIndices )
{
	ImmedVBOCache* theCache = GetVBOCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		theCache->InitBuffers( inRenderer.Funcs() );
		
		// Compute sub-buffer sizes for array data
		GLsizeiptr	vertexDataSize = static_cast<GLsizeiptr>(inNumPoints * sizeof(TQ3Point3D));
		GLsizeiptr	normalDataSize = (inNormals == nullptr)? 0 :
			static_cast<GLsizeiptr>(inNumPoints * sizeof(TQ3Vector3D));
		GLsizeiptr	colorDataSize = (inColors == nullptr)? 0 :
			static_cast<GLsizeiptr>(inNumPoints * sizeof(TQ3ColorRGBA));
		GLsizeiptr	uvDataSize = (inUVs == nullptr)? 0 :
			static_cast<GLsizeiptr>(inNumPoints * sizeof(TQ3Param2D));
		GLsizeiptr	totalArrayDataSize = vertexDataSize + normalDataSize +
			colorDataSize + uvDataSize;
		
		// Compute sub-buffer offsets
		GLsizeiptr normalOffset = (normalDataSize == 0)? kAbsentBuffer : vertexDataSize;
		GLsizeiptr colorOffset = (colorDataSize == 0)? kAbsentBuffer : vertexDataSize + normalDataSize;
		GLsizeiptr uvOffset = (uvDataSize == 0)? kAbsentBuffer : vertexDataSize + normalDataSize + colorDataSize;

		// Buffer size for element data
		if (inIndices == nullptr)
		{
			inNumIndices = 0;
		}
		GLsizeiptr	elementDataSize = inNumIndices * sizeof(TQ3Uns32);
		
		theCache->UpdateBufferSizes( inRenderer.Funcs(), totalArrayDataSize, elementDataSize );
		
		// Copy the data into the array VBO
		theCache->BindArrayBuffer( inRenderer.Funcs() );
		const GLvoid *	dataAddr;
		dataAddr = inPoints;
		(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER, 0,
			vertexDataSize, dataAddr );
		CHECK_GL_ERROR;
		if (inNormals != nullptr)
		{
			dataAddr = inNormals;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER,
				normalOffset, normalDataSize, dataAddr );
			CHECK_GL_ERROR;
		}
		if (inColors != nullptr)
		{
			dataAddr = inColors;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER,
				colorOffset, colorDataSize, dataAddr );
			CHECK_GL_ERROR;
		}
		if (inUVs != nullptr)
		{
			dataAddr = inUVs;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER,
				uvOffset, uvDataSize, dataAddr );
			CHECK_GL_ERROR;
		}
			
		// Associate vertex data pointers with sub-buffers
		inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mVertexAttribLoc,
			3, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( 0U ) );
		if (inNormals != nullptr)
		{
			inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mNormalAttribLoc,
				3, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( normalOffset ) );
		}
		if (inColors != nullptr)
		{
			inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mColorAttribLoc,
				4, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( colorOffset ) );
		}
		if (inUVs != nullptr)
		{
			inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mTexCoordAttribLoc,
				2, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( uvOffset ) );
		}
		
		if (inIndices == nullptr)
		{
			(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
			glDrawArrays( inMode, 0, inNumPoints );
		}
		else
		{
			// Copy data into element VBO
			theCache->BindElementBuffer( inRenderer.Funcs() );
			CHECK_GL_ERROR;
			dataAddr = inIndices;
			(*inRenderer.Funcs().glBufferSubDataProc)( GL_ELEMENT_ARRAY_BUFFER,
				0, elementDataSize, dataAddr );
			CHECK_GL_ERROR;
			
			// Draw the elements
			glDrawElements( inMode, inNumIndices, GL_UNSIGNED_INT, GLBufferObPtr( 0U ) );
		}
		
		(*inRenderer.Funcs().glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
		(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
}

/*!
	@function	RenderImmediateShadowVolumeVBO
	
	@abstract	Copy shadow volume data to a VBO and immediately render it on
				a one-shot basis.

	@param		inRenderer		A renderer.
	@param		inNumPoints		Number of points.
	@param		inPoints		Array of rational points.
	@param		inNumIndices	Number of indices in the following array.
	@param		inIndices		Array of point indices.
*/
void			RenderImmediateShadowVolumeVBO(
									const QORenderer::Renderer& inRenderer,
									TQ3Uns32 inNumPoints,
									const TQ3RationalPoint4D* _Nonnull inPoints,
									TQ3Uns32 inNumIndices,
									const TQ3Uns32* _Nonnull inIndices )
{
	ImmedVBOCache* theCache = GetVBOCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		theCache->InitBuffers( inRenderer.Funcs() );
		
		// Compute sub-buffer sizes for array data
		GLsizeiptr	vertexDataSize = static_cast<GLsizeiptr>(inNumPoints * sizeof(TQ3RationalPoint4D));
		
		// Buffer size for element data
		GLsizeiptr	elementDataSize = inNumIndices * sizeof(TQ3Uns32);
		
		theCache->UpdateBufferSizes( inRenderer.Funcs(), vertexDataSize, elementDataSize );
		
		// Copy the data into the array VBO
		theCache->BindArrayBuffer( inRenderer.Funcs() );
		const GLvoid *	dataAddr;
		dataAddr = inPoints;
		(*inRenderer.Funcs().glBufferSubDataProc)( GL_ARRAY_BUFFER, 0,
			vertexDataSize, dataAddr );
		CHECK_GL_ERROR;
			
		// Associate vertex data pointers with sub-buffers
		inRenderer.SLFuncs().glVertexAttribPointer( inRenderer.Shader().CurrentProgram()->mVertexAttribLoc,
			4, GL_FLOAT, GL_FALSE, 0, GLBufferObPtr( 0U ) );
		
		// Copy data into element VBO
		theCache->BindElementBuffer( inRenderer.Funcs() );
		CHECK_GL_ERROR;
		dataAddr = inIndices;
		(*inRenderer.Funcs().glBufferSubDataProc)( GL_ELEMENT_ARRAY_BUFFER,
			0, elementDataSize, dataAddr );
		CHECK_GL_ERROR;
		
		// Draw the elements
		glDrawElements( GL_TRIANGLES, inNumIndices, GL_UNSIGNED_INT, GLBufferObPtr( 0U ) );
		
		(*inRenderer.Funcs().glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
		(*inRenderer.Funcs().glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}
}


/*!
	@function	SetUpImmediateVBO
	
	@abstract	Initialize a VBO of a needed size.
	
	@param		inRenderer		A renderer.
	@param		inVertexDataSize	Size in bytes of vertex data.
	@param		inElementDataSize	Size in bytes of element index data.
	@result		True if we are ready for further operations.
*/
bool			SetUpImmediateVBO(
									const QORenderer::Renderer& inRenderer,
									GLsizeiptr inVertexDataSize,
									GLsizeiptr inElementDataSize )
{
	ImmedVBOCache* theCache = GetVBOCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		theCache->InitBuffers( inRenderer.Funcs() );
		
		theCache->UpdateBufferSizes( inRenderer.Funcs(), inVertexDataSize, inElementDataSize );
	}
	
	return (theCache != nullptr);
}


/*!
	@function	BindImmediateVBOArrayBuffer
	
	@abstract	Bind a VBO to GL_ARRAY_BUFFER, preparing to supply
				vertex data.

	@param		inRenderer		A renderer.
*/
void			BindImmediateVBOArrayBuffer(
									const QORenderer::Renderer& inRenderer )
{
	ImmedVBOCache* theCache = GetVBOCache( inRenderer.GLContext() );
	
	if (theCache != nullptr)
	{
		theCache->BindArrayBuffer( inRenderer.Funcs() );
	}
}

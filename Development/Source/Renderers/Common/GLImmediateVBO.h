/*! @header GLImmediateVBO.h
        
	@ignore	_Nullable
	@ignore _Nonnull
	@ignore	_Null_unspecified
 */
/*  NAME:
        GLImmediateVBO.h

    DESCRIPTION:
        Header file for GLImmediateVBO.cpp.
       
    REMARKS:
    	This VBO will be used for uncached geometry, to be loaded into the
    	VBO and immediately rendered, which previously would have been
    	handled without a VBO.

    COPYRIGHT:
        Copyright (c) 2018, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#ifndef GLImmediateVBO_h
#define GLImmediateVBO_h

#include "GLVBOManager.h"

namespace QORenderer
{
	class Renderer;
}


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
									const TQ3Point3D* _Nonnull inPoints,
									const TQ3Vector3D* _Nullable inNormals,
									const TQ3ColorRGB* _Nullable inColors,
									const TQ3Param2D* _Nullable inUVs,
									TQ3Uns32 inNumIndices,
									const TQ3Uns32* _Nullable inIndices );

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
									const TQ3Uns32* _Nullable inIndices );

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
									const TQ3Uns32* _Nonnull inIndices );

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
									GLsizeiptr inElementDataSize );

/*!
	@function	BindImmediateVBOArrayBuffer
	
	@abstract	Bind a VBO to GL_ARRAY_BUFFER, preparing to supply
				vertex data.

	@param		inRenderer		A renderer.
*/
void			BindImmediateVBOArrayBuffer(
									const QORenderer::Renderer& inRenderer );

#endif /* GLImmediateVBO_h */

/*  NAME:
       QOOpaqueTriBuffer.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2021, Quesa Developers. All rights reserved.

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

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QORenderer.h"
#include "GLImmediateVBO.h"
#include <cstddef>

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER		0x8892
#define GL_ELEMENT_ARRAY_BUFFER		0x8893
#endif

//=============================================================================
//      Local functions
//-----------------------------------------------------------------------------
using namespace QORenderer;

static bool operator==( const TQ3ColorRGB& inOne, const TQ3ColorRGB& inTwo )
{
	return fabsf( inOne.r - inTwo.r ) +
		fabsf( inOne.g - inTwo.g ) +
		fabsf( inOne.b - inTwo.b ) < kQ3RealZero;
}

static bool operator!=( const TQ3ColorRGB& inOne, const TQ3ColorRGB& inTwo )
{
	return ! (inOne == inTwo);
}


//=============================================================================
//      Class Implementation
//-----------------------------------------------------------------------------

/*!
	@function	OpaqueTriBuffer (constructor)
	@abstract	Initialize the buffer.
*/
QORenderer::OpaqueTriBuffer::OpaqueTriBuffer(
									Renderer& inRenderer  )
	: mRenderer( inRenderer )
	, mTriBufferFlags( kVertexFlagNone )
{
}


/*!
	@function	Flush
	@abstract	Draw and empty the buffer.
*/
void	QORenderer::OpaqueTriBuffer::Flush()
{
	if (! mTriBuffer.empty())
	{
		// Allow usual lighting
		mRenderer.mLights.SetLowDimensionalMode( false, mRenderer.mViewIllumination );

		// Maybe update fragment program.
		mRenderer.mPPLighting.PreGeomSubmit( nullptr, 2 );

		// Maybe emissive material
		if ( (mTriBufferFlags & kVertexHaveEmissive) != 0 )
		{
			mRenderer.SetEmissiveMaterial( mTriBuffer.back().emissiveColor );
		}

		const int kNumVertices = static_cast<int>(mTriBuffer.size());
		
		const bool haveNormal = (mTriBufferFlags & kVertexHaveNormal) != 0 && mRenderer.CurrentShaderHasNormalAttrib();
		const bool haveUV = (mTriBufferFlags & kVertexHaveUV) != 0;
		const bool haveDiffuse = (mTriBufferFlags & kVertexHaveDiffuse) != 0;
		
		mRenderer.mGLClientStates.EnableNormalArray( haveNormal );
		mRenderer.mGLClientStates.EnableTextureArray( haveUV );
		mRenderer.mGLClientStates.EnableColorArray( haveDiffuse );
		
		if (SetUpImmediateVBO( mRenderer,
			mTriBuffer.size() * sizeof(QORenderer::Vertex), 0 ))
		{
			// Load interleaved vertex data into VBO
			BindImmediateVBOArrayBuffer( mRenderer );
			(*mRenderer.mFuncs.glBufferSubDataProc)( GL_ARRAY_BUFFER, 0,
				mTriBuffer.size() * sizeof(QORenderer::Vertex), &mTriBuffer[0] );
				
			// Set up pointers
			mRenderer.mSLFuncs.glVertexAttribPointer( mRenderer.mPPLighting.CurrentProgram()->mVertexAttribLoc,
				3, GL_FLOAT, GL_FALSE, sizeof(QORenderer::Vertex),
				GLBufferObPtr( (GLuint)offsetof( QORenderer::Vertex, point ) ) );
			
			if ( haveNormal )
			{
				mRenderer.mSLFuncs.glVertexAttribPointer( mRenderer.mPPLighting.CurrentProgram()->mNormalAttribLoc,
					3, GL_FLOAT, GL_FALSE, sizeof(QORenderer::Vertex),
					GLBufferObPtr( (GLuint)offsetof( QORenderer::Vertex, normal ) ) );
			}
			
			if ( haveUV )
			{
				mRenderer.mSLFuncs.glVertexAttribPointer( mRenderer.mPPLighting.CurrentProgram()->mTexCoordAttribLoc,
					2, GL_FLOAT, GL_FALSE, sizeof(QORenderer::Vertex),
					GLBufferObPtr( (GLuint)offsetof( QORenderer::Vertex, uv ) ) );
			}
			
			if ( haveDiffuse )
			{
				mRenderer.mSLFuncs.glVertexAttribPointer( mRenderer.mPPLighting.CurrentProgram()->mColorAttribLoc,
					3, GL_FLOAT, GL_FALSE, sizeof(QORenderer::Vertex),
					GLBufferObPtr( (GLuint)offsetof( QORenderer::Vertex, diffuseColor ) ) );
			}
			
			// Draw
			glDrawArrays( GL_TRIANGLES, 0, kNumVertices );
			
			(*mRenderer.mFuncs.glBindBufferProc)( GL_ARRAY_BUFFER, 0 );
			(*mRenderer.mFuncs.glBindBufferProc)( GL_ELEMENT_ARRAY_BUFFER, 0 );
		}		

		mTriBuffer.clear();
	}
}


/*!
	@function		AddTriangle
	@abstract		Add another triangle to the buffer.
*/
void	QORenderer::OpaqueTriBuffer::AddTriangle( const Vertex* inVertices )
{
	// Flush the buffer if the vertex format has changed
	if (inVertices[0].flags != mTriBufferFlags)
	{
		Flush();
		mTriBufferFlags = inVertices[0].flags;
	}
	
	// Flush the buffer if the emissive color has changed
	if ( (! mTriBuffer.empty()) &&
		((mTriBufferFlags & kVertexHaveEmissive) != 0) &&
		(inVertices[0].emissiveColor != mTriBuffer.back().emissiveColor)
	)
	{
		Flush();
	}
	
	// Append the vertices to the buffer
	mTriBuffer.insert( mTriBuffer.end(), inVertices, inVertices+3 );
}

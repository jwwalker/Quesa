/*  NAME:
       QOOpaqueTriBuffer.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://quesa.sourceforge.net/>
        
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
	if (not mTriBuffer.empty())
	{
		// Allow usual lighting
		mRenderer.mLights.SetOnlyAmbient( false );

		// Set up indices, one for each vertex
		const int kNumVertices = mTriBuffer.size();
		mTriBufferIndices.resize( kNumVertices );
		int	i;
		for (i = 0; i < kNumVertices; ++i)
		{
			mTriBufferIndices[i] = i;
		}
		
		glVertexPointer( 3, GL_FLOAT, sizeof(QORenderer::Vertex),
			&mTriBuffer[0].point );
		
		// set up normals, UVs, diffuse colors when available
		if ( (mTriBufferFlags & kVertexHaveNormal) != 0 )
		{
			mRenderer.mGLClientStates.EnableNormalArray();
			glNormalPointer( GL_FLOAT, sizeof(QORenderer::Vertex),
				&mTriBuffer[0].normal );
		}
		else
		{
			mRenderer.mGLClientStates.DisableNormalArray();
		}
		
		if ( (mTriBufferFlags & kVertexHaveUV) != 0 )
		{
			mRenderer.mGLClientStates.EnableTextureArray();
			glTexCoordPointer( 2, GL_FLOAT, sizeof(QORenderer::Vertex),
				&mTriBuffer[0].uv );
		}
		else
		{
			mRenderer.mGLClientStates.DisableTextureArray();
		}
		
		if ( (mTriBufferFlags & kVertexHaveDiffuse) != 0 )
		{
			mRenderer.mGLClientStates.EnableColorArray();
			glColorPointer( 3, GL_FLOAT, sizeof(QORenderer::Vertex),
				&mTriBuffer[0].diffuseColor );
		}
		else
		{
			mRenderer.mGLClientStates.DisableColorArray();
		}
		
		glDrawElements( GL_TRIANGLES, kNumVertices, GL_UNSIGNED_INT,
			&mTriBufferIndices[0] );
		
		
		if ( mRenderer.IsFakeSeparateSpecularColorNeeded() )
		{
			mRenderer.SimulateSeparateSpecularColor( kNumVertices,
				reinterpret_cast<const TQ3Uns32*>(&mTriBufferIndices[0]) );
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
	
	// Append the vertices to the buffer
	mTriBuffer.insert( mTriBuffer.end(), inVertices, inVertices+3 );
}

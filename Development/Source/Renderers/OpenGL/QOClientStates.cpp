/*  NAME:
       QOClientStates.cpp

    DESCRIPTION:
        Header for Quesa OpenGL renderer.
		    
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
#include "QOClientStates.h"

#include "GLPrefix.h"
#include "GLUtils.h"
#include "QOGLShadingLanguage.h"

//=============================================================================
//     Class implementation
//-----------------------------------------------------------------------------

void QORenderer::ClientStates::StartProgram()
{
	mFuncs.glEnableVertexAttribArray( mShader.CurrentProgram()->mVertexAttribLoc );

	if (mShader.CurrentProgram()->mNormalAttribLoc != -1)
	{
		mFuncs.glDisableVertexAttribArray(mShader.CurrentProgram()->mNormalAttribLoc);
	}
	
	if (mShader.CurrentProgram()->mTexCoordAttribLoc != -1)
	{
		mFuncs.glDisableVertexAttribArray(mShader.CurrentProgram()->mTexCoordAttribLoc);
	}
	
	if (mShader.CurrentProgram()->mColorAttribLoc != -1)
	{
		mFuncs.glDisableVertexAttribArray(mShader.CurrentProgram()->mColorAttribLoc);
	}
	
	if (mShader.CurrentProgram()->mLayerShiftAttribLoc != -1)
	{
		mFuncs.glDisableVertexAttribArray(mShader.CurrentProgram()->mLayerShiftAttribLoc);
	}
	
	mGLClientStateNormal = false;
	mGLClientStateUV = false;
	mGLClientStateColor = false;
	mGLClientStateLayers = false;
}

void	QORenderer::ClientStates::EnableNormalArray( bool inEnable )
{
	// If we're trying to disable the normals, this is a no-op if the shader doesn't use normals.
	if (mShader.CurrentProgram()->mNormalAttribLoc == -1)
	{
		Q3_ASSERT(!inEnable);
		return;
	}

	if (inEnable != mGLClientStateNormal)
	{
		mGLClientStateNormal = inEnable;
		if (inEnable)
		{
			mFuncs.glEnableVertexAttribArray( mShader.CurrentProgram()->mNormalAttribLoc );
		}
		else
		{
			mFuncs.glDisableVertexAttribArray( mShader.CurrentProgram()->mNormalAttribLoc );
		}
	}
}

void	QORenderer::ClientStates::DisableNormalArray()
{
	// This is a no-op if the shader doesn't use normals.
	if (mShader.CurrentProgram()->mNormalAttribLoc == -1)
	{
		return;
	}

	if (mGLClientStateNormal)
	{
		mGLClientStateNormal = false;
		mFuncs.glDisableVertexAttribArray( mShader.CurrentProgram()->mNormalAttribLoc );
	}
}

void	QORenderer::ClientStates::EnableTextureArray( bool inEnable )
{
	if (inEnable != mGLClientStateUV)
	{
		mGLClientStateUV = inEnable;
		if (inEnable)
		{
			mFuncs.glEnableVertexAttribArray( mShader.CurrentProgram()->mTexCoordAttribLoc );
		}
		else
		{
			mFuncs.glDisableVertexAttribArray( mShader.CurrentProgram()->mTexCoordAttribLoc );
		}
	}
}

void	QORenderer::ClientStates::DisableTextureArray()
{
	if (mGLClientStateUV)
	{
		mGLClientStateUV = false;
		mFuncs.glDisableVertexAttribArray( mShader.CurrentProgram()->mTexCoordAttribLoc );
	}
}

void	QORenderer::ClientStates::EnableColorArray( bool inEnable )
{
	if (inEnable != mGLClientStateColor)
	{
		mGLClientStateColor = inEnable;
		if (inEnable)
		{
			mFuncs.glEnableVertexAttribArray( mShader.CurrentProgram()->mColorAttribLoc );
		}
		else
		{
			mFuncs.glDisableVertexAttribArray( mShader.CurrentProgram()->mColorAttribLoc );
		}
	}
}

void	QORenderer::ClientStates::DisableColorArray()
{
	if (mGLClientStateColor)
	{
		mGLClientStateColor = false;
		mFuncs.glDisableVertexAttribArray( mShader.CurrentProgram()->mColorAttribLoc );
	}
}

void	QORenderer::ClientStates::EnableLayerShiftArray( bool inEnable )
{
	if (inEnable != mGLClientStateLayers)
	{
		mGLClientStateLayers = inEnable;
		if (inEnable)
		{
			mFuncs.glEnableVertexAttribArray( mShader.CurrentProgram()->mLayerShiftAttribLoc );
		}
		else
		{
			mFuncs.glDisableVertexAttribArray( mShader.CurrentProgram()->mLayerShiftAttribLoc );
		}
		CHECK_GL_ERROR;
	}
}

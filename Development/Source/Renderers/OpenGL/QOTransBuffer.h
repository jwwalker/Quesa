/*  NAME:
       QOTransBuffer.h

    DESCRIPTION:
        Header for Quesa OpenGL renderer class.
		    
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

#include "QOPrefix.h"

#include <vector>


//=============================================================================
//      Class declarations
//-----------------------------------------------------------------------------

namespace QORenderer
{

class Renderer;
class PerPixelLighting;

struct TransparentPrim
{
	TQ3Uns32			mNumVerts;
	Vertex				mVerts[3];	// points and normals in camera coordinates
	float				mFrustumDepth;
	
	GLuint				mTextureName;
	bool				mIsTextureTransparent;
	TQ3ShaderUVBoundary	mShaderUBoundary;
	TQ3ShaderUVBoundary	mShaderVBoundary;
	TQ3Uns32			mUVTransformIndex;
	
	TQ3FillStyle		mFillStyle;
	TQ3OrientationStyle	mOrientationStyle;
	TQ3BackfacingStyle	mBackfacingStyle;
	TQ3ColorRGB			mSpecularColor;
	float				mSpecularControl;
	TQ3ObjectType		mIlluminationType;
	TQ3Uns32			mFogStyleIndex;
	TQ3Uns32			mCameraToFrustumIndex;
};

/*!
	@class		TransBuffer
	
	@abstract	Buffer for transparent (translucent) primitives in the Quesa
				OpenGL renderer.
	
	@discussion	Transparent primitives (triangles, lines, and points) must be
				drawn after all the opaque geometry, in back to front order.
				Therefore we must buffer the primitives, with enough state to
				draw them as they would have been drawn when submitted.
*/
class TransBuffer
{
public:
									TransBuffer(
											Renderer& inRenderer,
											PerPixelLighting& inPPLighting );
									
	void							AddTriangle(
											const Vertex* inVertices );

	void							AddLine(
											const Vertex* inVertices );

	void							AddPoint(
											const Vertex& inVertex );
	
	void							Flush(
											TQ3ViewObject inView );

private:
	void							AddPrim(
											int inNumVerts,
											const Vertex* inVertices );

	void							SortIndices();
	void							InitGLState(
											TQ3ViewObject inView );
	void							Cleanup();
	void							SetEmissiveColor( const TQ3ColorRGB& inColor );
	void							SetDiffuseColor(
											const QORenderer::Vertex& inVert );
	void							SetDiffuseColor(
											const GLfloat* inColor4 );
	
	void							UpdateBlendFunc(
											const TransparentPrim& inPrim );
	void							UpdateCameraToFrustum(
											const TransparentPrim& inPrim,
											TQ3ViewObject inView );
	void							UpdateLightingEnable(
											const TransparentPrim& inPrim );
	void							UpdateTexture(
											const TransparentPrim& inPrim );
	void							UpdateSpecular(
											const TransparentPrim& inPrim );
	void							UpdateFog(
											const TransparentPrim& inPrim );
	void							UpdateFill(
											const TransparentPrim& inPrim );
	void							UpdateOrientation(
											const TransparentPrim& inPrim );
	void							UpdateBackfacing(
											const TransparentPrim& inPrim );

	void							Render(
											const TransparentPrim& inPrim );
	void							RenderSpecular(
											const TransparentPrim& inPrim );
	void							AddSpecularHighlights(
											const TransparentPrim& inPrim );
	
	Renderer&						mRenderer;
	PerPixelLighting&				mPerPixelLighting;
	
	// Buffers used when accumulating primitives
	std::vector<TransparentPrim>	mTransBuffer;
	std::vector<TQ3Matrix4x4>		mCameraToFrustumMatrices;
	std::vector<TQ3Matrix3x3>		mUVTransforms;
	
	// State used when flushing (drawing) primitives
	std::vector<const TransparentPrim*>		mPrimPtrs;
	bool							mIsLightingEnabled;
	TQ3Uns32						mCurCameraToFrustumIndex;
	TQ3CameraTransformData			mCurCameraTransform;
	GLenum							mCurBlendFunc;
	GLuint							mCurTexture;
	TQ3Uns32						mCurUVTransformIndex;
	TQ3ShaderUVBoundary				mCurUBoundary;
	TQ3ShaderUVBoundary				mCurVBoundary;
	GLfloat							mCurSpecularColor[3];
	float							mCurSpecularControl;
	TQ3ColorRGB						mCurEmissiveColor;
	GLfloat							mCurDiffuseColor[4];
};

}

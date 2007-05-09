/*  NAME:
       QOTransBuffer.cpp

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
#include "GLUtils.h"
#include "E3Math.h"
#include "QOGLShadingLanguage.h"

#include <algorithm>

using namespace QORenderer;

//=============================================================================
//      Local constants and types
//-----------------------------------------------------------------------------

// In lieu of glext.h
#ifndef GL_EXT_blend_minmax
	#define GL_FUNC_ADD_EXT                   0x8006
	#define GL_MIN_EXT                        0x8007
	#define GL_MAX_EXT                        0x8008
	#define GL_BLEND_EQUATION_EXT             0x8009
#endif


namespace
{
	const GLfloat				kGLBlackColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	const TQ3ColorRGB			kBlackColor = { 0.0f, 0.0f, 0.0f };

	struct PtrCompare
	{
		inline
		bool	operator()( const TransparentPrim* inOne,
							const TransparentPrim* inTwo ) const
					{
						return inOne->mFrustumDepth < inTwo->mFrustumDepth;
					}
	};
}


//=============================================================================
//      Local Functions
//-----------------------------------------------------------------------------

static inline bool IsNearZero( float x )
{
	return (x < kQ3RealZero) && (x > -kQ3RealZero);
}

static bool IsSame3x3( const TQ3Matrix3x3& inA, const TQ3Matrix3x3& inB )
{
	return
		IsNearZero( inA.value[0][0] - inB.value[0][0] ) &&
		IsNearZero( inA.value[1][0] - inB.value[1][0] ) &&
		IsNearZero( inA.value[2][0] - inB.value[2][0] ) &&
		IsNearZero( inA.value[0][1] - inB.value[0][1] ) &&
		IsNearZero( inA.value[1][1] - inB.value[1][1] ) &&
		IsNearZero( inA.value[2][1] - inB.value[2][1] ) &&
		IsNearZero( inA.value[0][2] - inB.value[0][2] ) &&
		IsNearZero( inA.value[1][2] - inB.value[1][2] ) &&
		IsNearZero( inA.value[2][2] - inB.value[2][2] );
}

static bool IsSame4x4( const TQ3Matrix4x4& inA, const TQ3Matrix4x4& inB )
{
	return
		IsNearZero( inA.value[0][0] - inB.value[0][0] ) &&
		IsNearZero( inA.value[1][0] - inB.value[1][0] ) &&
		IsNearZero( inA.value[2][0] - inB.value[2][0] ) &&
		IsNearZero( inA.value[3][0] - inB.value[3][0] ) &&
		IsNearZero( inA.value[0][1] - inB.value[0][1] ) &&
		IsNearZero( inA.value[1][1] - inB.value[1][1] ) &&
		IsNearZero( inA.value[2][1] - inB.value[2][1] ) &&
		IsNearZero( inA.value[3][1] - inB.value[3][1] ) &&
		IsNearZero( inA.value[0][2] - inB.value[0][2] ) &&
		IsNearZero( inA.value[1][2] - inB.value[1][2] ) &&
		IsNearZero( inA.value[2][2] - inB.value[2][2] ) &&
		IsNearZero( inA.value[3][2] - inB.value[3][2] ) &&
		IsNearZero( inA.value[0][3] - inB.value[0][3] ) &&
		IsNearZero( inA.value[1][3] - inB.value[1][3] ) &&
		IsNearZero( inA.value[2][3] - inB.value[2][3] ) &&
		IsNearZero( inA.value[3][3] - inB.value[3][3] );
}

static float CalcPrimDepth( int inNumVerts, const TQ3Point3D* inFrustumPt )
{
	float	theDepth;
	
	if (inNumVerts == 3)
	{
		theDepth = inFrustumPt[0].z + inFrustumPt[1].z + inFrustumPt[2].z;
	}
	else if (inNumVerts == 2)
	{
		theDepth = inFrustumPt[0].z + inFrustumPt[1].z;
		theDepth *= 1.5f;
	}
	else
	{
		theDepth = inFrustumPt[0].z;
		theDepth *= 3;
	}
	return theDepth;
}


//=============================================================================
//      Method implementations
//-----------------------------------------------------------------------------


TransBuffer::TransBuffer( Renderer& inRenderer,
						PerPixelLighting& inPPLighting )
	: mRenderer( inRenderer )
	, mPerPixelLighting( inPPLighting )
{
}

void	TransBuffer::AddPrim(
										int inNumVerts,
										const Vertex* inVertices )
{
	int	i;
	
	// Copy vertices to new primitive structure
	TransparentPrim		thePrim;
	thePrim.mNumVerts = inNumVerts;
	for (i = 0; i < inNumVerts; ++i)
	{
		thePrim.mVerts[i] = inVertices[i];
	}
	
	// Transform vertex locations to camera space.
	// If all z values are positive (behind the camera), we can bail early.
	const TQ3Matrix4x4&	localToCamera(
		mRenderer.mMatrixState.GetLocalToCamera() );
	bool	isBehindCamera = true;
	for (i = 0; i < inNumVerts; ++i)
	{
		E3Point3D_Transform( &thePrim.mVerts[i].point, &localToCamera,
			&thePrim.mVerts[i].point );
		if (thePrim.mVerts[i].point.z <= 0.0f)
		{
			// I initially thought the comparison above should be <,
			// but then the rasterize test in Geom Test failed.
			isBehindCamera = false;
		}
	}
	
	if (isBehindCamera)
	{
		return;
	}
	
	// Transform vertex normals to camera coordinates, and also compute
	// frustum z coordinate for depth sorting.
	const TQ3Matrix4x4&	localToCameraInverseTranspose(
		mRenderer.mMatrixState.GetLocalToCameraInverseTranspose() );
	const TQ3Matrix4x4&	cameraToFrustum(
		mRenderer.mMatrixState.GetCameraToFrustum() );
	TQ3Point3D	frustumPoint[3];
	for (i = 0; i < inNumVerts; ++i)
	{
		if ( (thePrim.mVerts[i].flags & kVertexHaveNormal) != 0 )
		{
			E3Vector3D_Transform( &thePrim.mVerts[i].normal,
				&localToCameraInverseTranspose,
				&thePrim.mVerts[i].normal );
			
			Q3FastVector3D_Normalize( &thePrim.mVerts[i].normal,
				&thePrim.mVerts[i].normal );
		}
		
		E3Point3D_Transform( &thePrim.mVerts[i].point, &cameraToFrustum,
			&frustumPoint[i] );
	}
	thePrim.mFrustumDepth = CalcPrimDepth( inNumVerts, frustumPoint );
	
	// Record texture state
	const Texture::TextureState&	textureState(
		mRenderer.mTextures.GetTextureState() );
	if (! textureState.mIsTextureActive)
	{
		thePrim.mTextureName = 0;
		thePrim.mIsTextureTransparent = false;
	}
	else
	{
		thePrim.mTextureName = textureState.mGLTextureObject;
		thePrim.mIsTextureTransparent = textureState.mIsTextureTransparent;
		thePrim.mShaderUBoundary = textureState.mShaderUBoundary;
		thePrim.mShaderVBoundary = textureState.mShaderVBoundary;
		if ( mUVTransforms.empty() ||
			(! IsSame3x3( textureState.mUVTransform, mUVTransforms.back() ) ) )
		{
			mUVTransforms.push_back( textureState.mUVTransform );
		}
		thePrim.mUVTransformIndex = mUVTransforms.size() - 1;
	}
	
	// Record camera to frustum matrix
	if ( mCameraToFrustumMatrices.empty() ||
		(! IsSame4x4( mCameraToFrustumMatrices.back(), cameraToFrustum )) )
	{
		mCameraToFrustumMatrices.push_back( cameraToFrustum );
	}
	thePrim.mCameraToFrustumIndex = mCameraToFrustumMatrices.size() - 1;
	
	// Record some style state.
	thePrim.mFillStyle = mRenderer.mStyleFill;
	thePrim.mOrientationStyle = mRenderer.mStyleOrientation;
	thePrim.mBackfacingStyle = mRenderer.mStyleBackfacing;
	thePrim.mSpecularColor = *mRenderer.mGeomState.specularColor;
	thePrim.mSpecularControl = mRenderer.mCurrentSpecularControl;
	thePrim.mIlluminationType = mRenderer.mViewIllumination;
	thePrim.mFogStyleIndex = mRenderer.mCurFogStyleIndex;
	
	// Record the primitive.
	mTransBuffer.push_back( thePrim );
}
				
void	TransBuffer::AddTriangle( const Vertex* inVertices )
{
	if (mRenderer.IsFirstPass())
	{
		AddPrim( 3, inVertices );
	}
}

void	TransBuffer::AddLine( const Vertex* inVertices )
{
	if (mRenderer.IsFirstPass())
	{
		AddPrim( 2, inVertices );
	}
}

void	TransBuffer::AddPoint( const Vertex& inVertex )
{
	if (mRenderer.IsFirstPass())
	{
		AddPrim( 1, &inVertex );
	}
}

void	TransBuffer::SortIndices()
{
	const TQ3Uns32 kNumPrims = mTransBuffer.size();
	mPrimPtrs.resize( kNumPrims );
	TQ3Uns32	i;
	const TransparentPrim**	ptrArray = &mPrimPtrs[0];
	
	for (i = 0; i < kNumPrims; ++i)
	{
		mPrimPtrs[i] = &mTransBuffer[i];
	}
	
	PtrCompare	comparator;
	std::sort( ptrArray, ptrArray + kNumPrims, comparator );
}

void	TransBuffer::Cleanup()
{
	mTransBuffer.clear();
	mPrimPtrs.clear();
	mCameraToFrustumMatrices.clear();
	mUVTransforms.clear();
}

void	TransBuffer::InitGLState( TQ3ViewObject inView )
{
	// We need to clear any transforms which are active when the rendering loop ended,
	// since transparent primitives are already transformed into camera coordinates,
	// and the camera to frustum matrix will be updated later.
	Q3Matrix4x4_SetIdentity( &mCurCameraTransform.localToWorld );
	Q3Matrix4x4_SetIdentity( &mCurCameraTransform.worldToCamera );
	Q3Matrix4x4_SetIdentity( &mCurCameraTransform.cameraToFrustum );
	Q3CameraTransform_Submit (&mCurCameraTransform, inView );
	mCurCameraToFrustumIndex = ULONG_MAX;

	// We also enable blending
	glEnable( GL_BLEND );
	
    // The transparent pass does not need to write to the depth buffer, since it
    // is done after opaque stuff and is depth-sorted, but we will do depth testing.
    // Most likely the current depth comparison function is GL_LESS, and we need not
    // change that.  Note that since we are not writing to the depth buffer, we
    // can continue to use the same depth comparison function when adding
    // specular highlights.
	glDepthMask( GL_FALSE );
	
	glEnable( GL_LIGHTING );
	mIsLightingEnabled = true;
	
	TQ3FillStyle	theFillStyle = kQ3FillStyleFilled;
	mRenderer.UpdateFillStyle( &theFillStyle );
	
	TQ3OrientationStyle	theOrientation = kQ3OrientationStyleCounterClockwise;
	mRenderer.UpdateOrientationStyle( &theOrientation );
	
	TQ3BackfacingStyle	theBackfacing = kQ3BackfacingStyleRemove;
	mRenderer.UpdateBackfacingStyle( &theBackfacing );
	
	glBlendFunc( mSrcBlendFactor, mDstBlendFactor );

	// The first pass will not include specularity, so we set the specular color black.
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, kGLBlackColor );
	
	glDisable( GL_TEXTURE_2D );
	mCurTexture = 0;
	mCurUVTransformIndex = ULONG_MAX;
	mCurUBoundary = kQ3ShaderUVBoundarySize32;
	mCurVBoundary = kQ3ShaderUVBoundarySize32;
	
	// specular: set to illegal values to force initial update
	mCurSpecularControl = -1.0f;
	mCurSpecularColor[0] = -1.0f;
	
	// similar for diffuse color
	mCurDiffuseColor[3] = -1.0f;
	
	mCurEmissiveColor = kBlackColor;
	mRenderer.SetEmissiveMaterial( mCurEmissiveColor );
}

void	TransBuffer::UpdateCameraToFrustum(
											const TransparentPrim& inPrim,
											TQ3ViewObject inView )
{
	if (inPrim.mCameraToFrustumIndex != mCurCameraToFrustumIndex)
	{
		mCurCameraToFrustumIndex = inPrim.mCameraToFrustumIndex;
		mCurCameraTransform.cameraToFrustum =
			mCameraToFrustumMatrices[ mCurCameraToFrustumIndex ];
		
		Q3CameraTransform_Submit( &mCurCameraTransform, inView );
	}
}

void	TransBuffer::UpdateLightingEnable(
											const TransparentPrim& inPrim )
{
	bool	shouldLight = inPrim.mIlluminationType != kQ3IlluminationTypeNULL;
	
	if (shouldLight != mIsLightingEnabled)
	{
		mIsLightingEnabled = shouldLight;
		
		if (mIsLightingEnabled)
		{
			glEnable( GL_LIGHTING );
		}
		else
		{
			glDisable( GL_LIGHTING );
		}
	}
}




void	TransBuffer::UpdateTexture( const TransparentPrim& inPrim )
{
	if (inPrim.mTextureName != mCurTexture)
	{
		mCurTexture = inPrim.mTextureName;
		
		if (mCurTexture == 0)
		{
			glDisable( GL_TEXTURE_2D );
		}
		else
		{
			glEnable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, mCurTexture );
		}
		
		mPerPixelLighting.UpdateTexture();
	}
	
	if ( (inPrim.mUVTransformIndex != mCurUVTransformIndex) &&
		(mCurTexture != 0) )
	{
		mCurUVTransformIndex = inPrim.mUVTransformIndex;
		GLUtils_LoadShaderUVTransform( &mUVTransforms[ mCurUVTransformIndex ] );
	}
	
	if ( (inPrim.mShaderUBoundary != mCurUBoundary) &&
		(mCurTexture != 0) )
	{
		mCurUBoundary = inPrim.mShaderUBoundary;
		GLint	uBoundary;
		GLUtils_ConvertUVBoundary( mCurUBoundary,
			&uBoundary, mRenderer.mGLExtensions.clampToEdge );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, uBoundary );
	}
	
	if ( (inPrim.mShaderVBoundary != mCurVBoundary) &&
		(mCurTexture != 0) )
	{
		mCurVBoundary = inPrim.mShaderVBoundary;
		GLint	vBoundary;
		GLUtils_ConvertUVBoundary( mCurVBoundary,
			&vBoundary, mRenderer.mGLExtensions.clampToEdge );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vBoundary );
	}
}

void	TransBuffer::UpdateFog( const TransparentPrim& inPrim )
{
	if (inPrim.mFogStyleIndex != mRenderer.mCurFogStyleIndex)
	{
		mRenderer.UpdateFogStyle(
			&mRenderer.mFogStyles[ inPrim.mFogStyleIndex ] );
	}
}

void	TransBuffer::UpdateFill( const TransparentPrim& inPrim )
{
	if (inPrim.mFillStyle != mRenderer.mStyleFill)
	{
		mRenderer.UpdateFillStyle( &inPrim.mFillStyle );
	}
}

void	TransBuffer::UpdateOrientation( const TransparentPrim& inPrim )
{
	if (inPrim.mOrientationStyle != mRenderer.mStyleOrientation)
	{
		mRenderer.UpdateOrientationStyle( &inPrim.mOrientationStyle );
	}
}

void	TransBuffer::UpdateBackfacing( const TransparentPrim& inPrim )
{
	if (inPrim.mBackfacingStyle != mRenderer.mStyleBackfacing)
	{
		mRenderer.UpdateBackfacingStyle( &inPrim.mBackfacingStyle );
	}
}


void	TransBuffer::SetEmissiveColor( const TQ3ColorRGB& inColor )
{
	if ( (inColor.r != mCurEmissiveColor.r) ||
		(inColor.g != mCurEmissiveColor.g) ||
		(inColor.b != mCurEmissiveColor.b) )
	{
		mRenderer.SetEmissiveMaterial( inColor );
		mCurEmissiveColor = inColor;
	}
}

void	TransBuffer::SetDiffuseColor( const QORenderer::Vertex& inVert )
{
	GLfloat	color4[4] = {
		inVert.diffuseColor.r,
		inVert.diffuseColor.g,
		inVert.diffuseColor.b,
		inVert.vertAlpha
	};
	
	SetDiffuseColor( color4 );
}

void	TransBuffer::SetDiffuseColor( const GLfloat* inColor4 )
{
	if ( (inColor4[3] != mCurDiffuseColor[3]) ||
		(inColor4[0] != mCurDiffuseColor[0]) ||
		(inColor4[1] != mCurDiffuseColor[1]) ||
		(inColor4[2] != mCurDiffuseColor[2]) )
	{
		mCurDiffuseColor[0] = inColor4[0];
		mCurDiffuseColor[1] = inColor4[1];
		mCurDiffuseColor[2] = inColor4[2];
		mCurDiffuseColor[3] = inColor4[3];

		glColor4fv( mCurDiffuseColor );
	}
}

void	TransBuffer::Render( const TransparentPrim& inPrim )
{
	switch (inPrim.mNumVerts)
	{
		case 3:
			mRenderer.mLights.SetOnlyAmbient( false );
			glBegin( GL_TRIANGLES );
			break;

		case 2:
			mRenderer.mLights.SetOnlyAmbient( true );
			glBegin( GL_LINES );
			break;

		case 1:
			mRenderer.mLights.SetOnlyAmbient( true );
			glBegin( GL_POINTS );
			break;
	}
	
	for (TQ3Uns32 i = 0; i < inPrim.mNumVerts; ++i)
	{
		const Vertex&	theVert( inPrim.mVerts[i] );
		
		if ((theVert.flags & kVertexHaveNormal) != 0)
		{
			glNormal3fv( (const GLfloat *) &theVert.normal );
		}
		
		if ( (mCurTexture != 0) && ((theVert.flags & kVertexHaveUV) != 0) )
		{
			glTexCoord2fv( (const GLfloat *) &theVert.uv );
		}
		
		if ((theVert.flags & kVertexHaveDiffuse) != 0)
		{
			SetDiffuseColor( theVert );
		}
		
		if ((theVert.flags & kVertexHaveEmissive) != 0)
		{
			SetEmissiveColor( theVert.emissiveColor );
		}
		else
		{
			SetEmissiveColor( kBlackColor );
		}
		
		glVertex3fv( (const GLfloat *) &theVert.point );
	}
	
	
	glEnd();
}

void	TransBuffer::RenderSpecular( const TransparentPrim& inPrim )
{
	glBegin( GL_TRIANGLES );
	
	for (TQ3Uns32 i = 0; i < 3; ++i)
	{
		const Vertex&	theVert( inPrim.mVerts[i] );
		
		if ((theVert.flags & kVertexHaveNormal) != 0)
		{
			glNormal3fv( (const GLfloat *) &theVert.normal );
		}
		
		glVertex3fv( (const GLfloat *) &theVert.point );
	}
	
	glEnd();
}

void	TransBuffer::UpdateSpecular( const TransparentPrim& inPrim )
{
	if ( (inPrim.mSpecularColor.r != mCurSpecularColor[0]) ||
		(inPrim.mSpecularColor.g != mCurSpecularColor[1]) ||
		(inPrim.mSpecularColor.b != mCurSpecularColor[2]) )
	{
		mCurSpecularColor[0] = inPrim.mSpecularColor.r;
		mCurSpecularColor[1] = inPrim.mSpecularColor.g;
		mCurSpecularColor[2] = inPrim.mSpecularColor.b;
		
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mCurSpecularColor );
	}
	
	if (inPrim.mSpecularControl != mCurSpecularControl)
	{
		mCurSpecularControl = inPrim.mSpecularControl;
		
		GLfloat		shininess = GLUtils_SpecularControlToGLShininess(
			mCurSpecularControl );
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shininess );
	}
}

void	TransBuffer::AddSpecularHighlights(
										const TransparentPrim& inPrim )
{
	if ( (inPrim.mNumVerts == 3) &&
		(inPrim.mIlluminationType == kQ3IlluminationTypePhong) &&
		(inPrim.mFillStyle == kQ3FillStyleFilled) )
	{
		// Add, not alpha-blend, but use max rather than addition if possible
		// so that color components do not get too big.
		glBlendFunc( GL_ONE, GL_ONE );
		if (mRenderer.mGLBlendEqProc != NULL)
			(*mRenderer.mGLBlendEqProc)( GL_MAX_EXT );

		// black ambient and diffuse so we get only specular
		SetDiffuseColor( kGLBlackColor );
		
		UpdateSpecular( inPrim );
		
		RenderSpecular( inPrim );
		
		// Reset blend func
		glBlendFunc( mSrcBlendFactor, mDstBlendFactor );
		if (mRenderer.mGLBlendEqProc != NULL)
			(*mRenderer.mGLBlendEqProc)( GL_FUNC_ADD_EXT );
	}
}

void	TransBuffer::DrawTransparency( TQ3ViewObject inView,
										GLenum inSrcBlendFactor,
										GLenum inDstBlendFactor )
{
	if (! mTransBuffer.empty())
	{
		mSrcBlendFactor = inSrcBlendFactor;
		mDstBlendFactor = inDstBlendFactor;
		
		SortIndices();

		// Save some OpenGL state
		glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT |
			GL_COLOR_BUFFER_BIT );
		
		InitGLState( inView );
		
		const TQ3Uns32 kNumPrims = mTransBuffer.size();
		
		for (TQ3Uns32 i = 0; i < kNumPrims; ++i)
		{
			const TransparentPrim& thePrim( *mPrimPtrs[i] );
			
			UpdateCameraToFrustum( thePrim, inView );
			UpdateLightingEnable( thePrim );
			UpdateTexture( thePrim );
			UpdateFog( thePrim );
			UpdateFill( thePrim );
			UpdateOrientation( thePrim );
			UpdateBackfacing( thePrim );
			mPerPixelLighting.UpdateIllumination( thePrim.mIlluminationType );
			
			Render( thePrim );
			
			AddSpecularHighlights( thePrim );
		}

		// Reset the OpenGL state
		glPopAttrib();
	}
}

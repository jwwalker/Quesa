/*  NAME:
       QOTransBuffer.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2014, Quesa Developers. All rights reserved.

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
#include "E3Math_Intersect.h"
#include "QOGLShadingLanguage.h"

#include <algorithm>
#include <stdint.h>

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


// Old CodeWarrior headers do not define UINT32_MAX
#ifndef UINT32_MAX
	#define		UINT32_MAX		0xFFFFFFFFU
#endif

namespace
{
	const float					kAlphaThreshold		= 0.01f;

	const GLfloat				kGLBlackColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	const TQ3ColorRGB			kBlackColor = { 0.0f, 0.0f, 0.0f };
	const TQ3ColorRGB			kWhiteColor = { 1.0f, 1.0f, 1.0f };
	
	const TQ3Uns32				kRenderGroupReserve = 10000;
	const TQ3Uns32				kBlockUnionReserve = 1000;

	struct PtrCompare
	{
		inline
		bool	operator()( const TransparentPrim* inOne,
							const TransparentPrim* inTwo ) const
					{
						return inOne->mSortingDepth < inTwo->mSortingDepth;
					}
	};
	
	struct NonNullBlock
	{
		inline
		bool	operator()( const TransparentBlock* x ) const
		{
			return x != NULL;
		}
	};
	
	struct BlockOrder
	{
		inline
		bool	operator()( const TransparentBlock* inOne,
							const TransparentBlock* inTwo ) const
					{
						return inOne->mVisitOrder < inTwo->mVisitOrder;
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

static float CalcPrimDepth( const TransparentPrim& inPrim )
{
	float	theDepth;
	
	if (inPrim.mNumVerts == 3)
	{
		theDepth = inPrim.mVerts[0].point.z + inPrim.mVerts[1].point.z +
			inPrim.mVerts[2].point.z;
	}
	else if (inPrim.mNumVerts == 2)
	{
		theDepth = inPrim.mVerts[0].point.z + inPrim.mVerts[1].point.z;
		theDepth *= 1.5f;
	}
	else
	{
		theDepth = inPrim.mVerts[0].point.z;
		theDepth *= 3;
	}
	return theDepth;
}

static bool IsSameColor( const TQ3ColorRGB& inA, const TQ3ColorRGB& inB )
{
	float absDiff = fabsf( inA.r - inB.r ) + fabsf( inA.g - inB.g ) +
		fabsf( inA.b - inB.b );
	return absDiff < kQ3RealZero * 3.0f;
}

/*!
	@function	IsSameState
	@abstract	Test whether two primitives are of the same type, in the sense
				that they could be grouped together for rendering.
*/
static bool IsSameState( const TransparentPrim& inA, const TransparentPrim& inB )
{
	bool isSame = (inA.mNumVerts == inB.mNumVerts) &&
		(inA.mTextureName == inB.mTextureName) &&
		(inA.mIlluminationType == inB.mIlluminationType) &&
		(inA.mCameraToFrustumIndex == inB.mCameraToFrustumIndex) &&
		(inA.mFillStyle == inB.mFillStyle) &&
		(inA.mBackfacingStyle == inB.mBackfacingStyle) &&
		(inA.mOrientationStyle == inB.mOrientationStyle) &&
		(inA.mInterpolationStyle == inB.mInterpolationStyle) &&
		(inA.mFogStyleIndex == inB.mFogStyleIndex) &&
		(fabsf( inA.mSpecularControl - inB.mSpecularControl ) < kQ3RealZero) &&
		IsSameColor( inA.mSpecularColor, inB.mSpecularColor ) &&
		(fabsf( inA.mLineWidthStyle - inB.mLineWidthStyle ) < kQ3RealZero);
	
	// UV transform and U, V boundary only matter if there is a texture.
	if ( isSame && (inA.mTextureName != 0) )
	{
		isSame = (inA.mShaderUBoundary == inB.mShaderUBoundary) &&
			(inA.mShaderVBoundary == inB.mShaderVBoundary) &&
			(inA.mUVTransformIndex == inB.mUVTransformIndex);
	}
	
	// Primitives can only be consolidated into an array if all the vertices
	// have the same flags.
	if (isSame)
	{
		isSame = (inA.mVerts[0].flags == inB.mVerts[0].flags);
		
		if (isSame)
		{
			for (unsigned int i = 1; i < inA.mNumVerts; ++i)
			{
				if (inA.mVerts[i].flags != inA.mVerts[0].flags)
				{
					isSame = false;
					break;
				}
				if (inB.mVerts[i].flags != inA.mVerts[0].flags)
				{
					isSame = false;
					break;
				}
			}
		}
	}
	
	return isSame;
}

/*!
	@function	IsSameStateForDepth
	@abstract	Test whether two primitives are of the same type, in the sense
				that they could be grouped together for rendering depth.
*/
static bool IsSameStateForDepth( const TransparentPrim& inA, const TransparentPrim& inB )
{
	bool isSame = (inA.mNumVerts == inB.mNumVerts) &&
		(inA.mTextureName == inB.mTextureName) &&
		(inA.mCameraToFrustumIndex == inB.mCameraToFrustumIndex) &&
		(inA.mFillStyle == inB.mFillStyle) &&
		(inA.mBackfacingStyle == inB.mBackfacingStyle) &&
		(inA.mOrientationStyle == inB.mOrientationStyle) &&
		(inA.mInterpolationStyle == inB.mInterpolationStyle) &&
		(fabsf( inA.mLineWidthStyle - inB.mLineWidthStyle ) < kQ3RealZero);
	
	// UV transform and U, V boundary only matter if there is a texture.
	if ( isSame && (inA.mTextureName != 0) )
	{
		isSame = (inA.mShaderUBoundary == inB.mShaderUBoundary) &&
			(inA.mShaderVBoundary == inB.mShaderVBoundary) &&
			(inA.mUVTransformIndex == inB.mUVTransformIndex);
	}
	
	// Primitives can only be consolidated into an array if all the vertices
	// have the same flags.
	if (isSame)
	{
		isSame = (inA.mVerts[0].flags == inB.mVerts[0].flags);
		
		if (isSame)
		{
			for (unsigned int i = 1; i < inA.mNumVerts; ++i)
			{
				if (inA.mVerts[i].flags != inA.mVerts[0].flags)
				{
					isSame = false;
					break;
				}
				if (inB.mVerts[i].flags != inA.mVerts[0].flags)
				{
					isSame = false;
					break;
				}
			}
		}
	}
	
	return isSame;
}

//=============================================================================
//      Method implementations
//-----------------------------------------------------------------------------
#pragma mark -

TransparentBlock::TransparentBlock()
	: mVisitOrder( -1 )
	, mHasUniformVertexFlags( false )
{
	Q3FastBoundingBox_Reset( &mFrustumBounds );
}

bool	TransparentBlock::Intersects( const TransparentBlock& inOther ) const
{
	TQ3BoundingBox commonBox;
	return E3BoundingBox_Intersect( mFrustumBounds, inOther.mFrustumBounds,
		commonBox );
}

void	TransparentBlock::Union( const TransparentBlock& inOther )
{
	E3BoundingBox_Union( &mFrustumBounds, &inOther.mFrustumBounds, &mFrustumBounds );
	
	TQ3Uns32 oldSize = mPrims.size();
	TQ3Uns32 newSize = oldSize + inOther.mPrims.size();
	if (mPrims.capacity() < newSize)
	{
		// If we must grow it, make it worth our while.
		mPrims.reserve( E3Num_Max( 2 * newSize, kBlockUnionReserve ) );
	}
	mPrims.resize( newSize );
	E3Memory_Copy( &inOther.mPrims[0], &mPrims[oldSize],
		inOther.mPrims.size() * sizeof(TransparentPrim) );
	
	mHasUniformVertexFlags = false;
}

/*!
	@function	Occludes
	@abstract	Test whether the block is partly in front of another block.
	@discussion	The block is in front of another if the xy profiles of the
				blocks intersect, and the z coordinate of this block is greater
				(nearer the camera) than the z coordinate of the other.
*/
bool	TransparentBlock::Occludes( const TransparentBlock& inOther ) const
{
	TQ3BoundingBox commonBox;
	E3BoundingBox_Intersect( mFrustumBounds, inOther.mFrustumBounds,
		commonBox );
	Q3_ASSERT( commonBox.isEmpty == kQ3True );
	// Note: I am depending on an implementation detail of
	// E3BoundingBox_Intersect.  If the intersection is empty, then some fields
	// may be undefined, but min.x and max.x will be defined, and if
	// min.x <= max.x, then min.y and max.y will be defined.
	return (commonBox.min.x <= commonBox.max.x) &&
			(commonBox.min.y <= commonBox.max.y) &&
			(mFrustumBounds.min.z > inOther.mFrustumBounds.max.z);
}

/*!
	@function	SortPrimPtrs
	@abstract	Sort pointers to the primitives, in back to front order.
*/
void	TransparentBlock::SortPrimPtrs()
{
	const TQ3Uns32 kNumPrims = static_cast<TQ3Uns32>(mPrims.size());
	mPrimPtrs.resize( kNumPrims );
	
	for (TQ3Uns32 i = 0; i < kNumPrims; ++i)
	{
		mPrimPtrs[i] = &mPrims[i];
	}
	
	PtrCompare	comparator;
	const TransparentPrim**	ptrArray = &mPrimPtrs[0];
	std::sort( ptrArray, ptrArray + kNumPrims, comparator );
}

#pragma mark -

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
	memcpy( thePrim.mVerts, inVertices, inNumVerts * sizeof(Vertex) );
	
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
	// average z coordinate for depth sorting.
	const TQ3Matrix4x4&	localToCameraInverseTranspose(
		mRenderer.mMatrixState.GetLocalToCameraInverseTranspose() );
	const TQ3Matrix4x4&	cameraToFrustum(
		mRenderer.mMatrixState.GetCameraToFrustum() );
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
	}
	thePrim.mSortingDepth = CalcPrimDepth( thePrim );
	
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
		thePrim.mUVTransformIndex = static_cast<TQ3Uns32>(mUVTransforms.size() - 1);
	}
	
	// Record camera to frustum matrix
	if ( mCameraToFrustumMatrices.empty() ||
		(! IsSame4x4( mCameraToFrustumMatrices.back(), cameraToFrustum )) )
	{
		mCameraToFrustumMatrices.push_back( cameraToFrustum );
	}
	thePrim.mCameraToFrustumIndex = static_cast<TQ3Uns32>(mCameraToFrustumMatrices.size() - 1);
	
	// Record some style state.
	thePrim.mFillStyle = mRenderer.mStyleState.mFill;
	thePrim.mOrientationStyle = mRenderer.mStyleState.mOrientation;
	thePrim.mBackfacingStyle = mRenderer.mStyleState.mBackfacing;
	thePrim.mSpecularColor = *mRenderer.mGeomState.specularColor;
	thePrim.mSpecularControl = mRenderer.mCurrentSpecularControl;
	thePrim.mIlluminationType = mRenderer.mViewIllumination;
	thePrim.mFogStyleIndex = mRenderer.mStyleState.mCurFogStyleIndex;
	thePrim.mLineWidthStyle = mRenderer.mLineWidth;
	thePrim.mInterpolationStyle = mRenderer.mStyleState.mInterpolation;
	
	// Make a new block.
	TransparentBlock* theBlock = new TransparentBlock;
	
	// Compute points in frustum space, so we can get frustum bounds.
	TQ3Point3D frustumPts[3];
	for (i = 0; i < inNumVerts; ++i)
	{
		E3Point3D_Transform( &thePrim.mVerts[i].point, &cameraToFrustum,
			&frustumPts[i] );
	}
	E3BoundingBox_SetFromPoints3D( &theBlock->mFrustumBounds, frustumPts,
		inNumVerts, sizeof(TQ3Point3D) );
	
	// Record the primitive.
	theBlock->mPrims.push_back( thePrim );
	mIsSortNeeded = true;
	
	AddBlock( theBlock );
}


/*!
	@function	AddBlock
	
	@abstract	Add a block to our set of blocks, in such a way that we end up
				with disjoint blocks.
	
	@discussion	If the new block intersects any old blocks, it gobbles them up,
				until we are left with no old blocks intersecting the new block.
				Then we add the new block to our list.
*/
void	TransBuffer::AddBlock( TransparentBlock* ioBlock )
{
	bool needAnotherPass = true;
	const TQ3Uns32 kNumOldBlocks = mBlocks.size();
	TQ3Uns32 i;
	TQ3Uns32 remainingOld = kNumOldBlocks;
	//Q3_MESSAGE_FMT("Block %p initial bounds (%f, %f, %f)(%f, %f, %f)",
	//	ioBlock, ioBlock->mFrustumBounds.min.x, ioBlock->mFrustumBounds.min.y,
	//	ioBlock->mFrustumBounds.min.z, ioBlock->mFrustumBounds.max.x,
	//	ioBlock->mFrustumBounds.max.y, ioBlock->mFrustumBounds.max.z );
	
	while (needAnotherPass)
	{
		needAnotherPass = false;
		
		for (i = 0; i < kNumOldBlocks; ++i)
		{
			if ( (mBlocks[i] != NULL) and ioBlock->Intersects( *mBlocks[i] ) )
			{
				TransparentBlock* olderBlock = mBlocks[i];
				mBlocks[i] = NULL;
				
				// Whichever block is bigger will eat the other.
				if (olderBlock->mPrims.size() > ioBlock->mPrims.size())
				{
					olderBlock->Union( *ioBlock );
					delete ioBlock;
					ioBlock = olderBlock;
				}
				else
				{
					ioBlock->Union( *olderBlock );
					delete olderBlock;
				}

				needAnotherPass = true;
				remainingOld -= 1;
			}
		}
	}
	
	//Q3_MESSAGE_FMT("Block %p final bounds (%f, %f, %f)(%f, %f, %f)",
	//	ioBlock, ioBlock->mFrustumBounds.min.x, ioBlock->mFrustumBounds.min.y,
	//	ioBlock->mFrustumBounds.min.z, ioBlock->mFrustumBounds.max.x,
	//	ioBlock->mFrustumBounds.max.y, ioBlock->mFrustumBounds.max.z );

	if (remainingOld < kNumOldBlocks)
	{
		// Rearrange so that non-NULL block pointers precede NULL block pointers.
		std::partition( &mBlocks[0], &mBlocks[kNumOldBlocks], NonNullBlock() );
		mBlocks[ remainingOld ] = ioBlock;
		mBlocks.resize( remainingOld + 1 );
	}
	else
	{
		mBlocks.push_back( ioBlock );
	}
}


/*!
	@function	MakeVertexPrototype
	@abstract	When adding a TriMesh, some of the fields of a Vertex will be
				constant: diffuseColor, vertAlpha, emissiveColor, flags.
				This function fills those fields of a prototype Vertex.
*/
void	TransBuffer::MakeVertexPrototype(
										const MeshArrays& inData,
										Vertex& outVertex ) const
{
	outVertex.flags = kVertexFlagNone;
	outVertex.diffuseColor = kWhiteColor;
	
	TQ3ColorRGB	transparentColor = kWhiteColor;
	bool	haveTransparentColor = false;

	if (inData.vertNormal != NULL)
	{
		outVertex.flags |= kVertexHaveNormal;
	}
	
	if (inData.vertUV != NULL)
	{
		outVertex.flags |= kVertexHaveUV;
	}

	/*
		The legacy behavior is that unless the illumination is NULL, a texture
		replaces the underlying color.
	*/
	if ( mRenderer.mTextures.IsTextureActive() &&
		(mRenderer.mViewIllumination != kQ3IlluminationTypeNULL) &&
		(inData.vertUV != NULL) )
	{
		outVertex.diffuseColor = kWhiteColor;
		outVertex.flags |= kVertexHaveDiffuse;
	}
	// if no color or if highlighting is on, get geom state color
	else
	{
		if (
			(mRenderer.mGeomState.diffuseColor != NULL) &&
			(
				((outVertex.flags & kVertexHaveDiffuse) == 0) ||
				(mRenderer.mGeomState.highlightState == kQ3On) )
			) 
		{
			outVertex.diffuseColor = *mRenderer.mGeomState.diffuseColor;
			outVertex.flags |= kVertexHaveDiffuse;
		}
	}
	
	outVertex.vertAlpha = mRenderer.mGeomState.alpha;
	if (1.0f - outVertex.vertAlpha > kAlphaThreshold)
	{
		outVertex.flags |= kVertexHaveTransparency;
	}

	// Emissive color from geom state?
	if (
		(mRenderer.mGeomState.emissiveColor != NULL) &&
		(mRenderer.mGeomState.emissiveColor->r +
			mRenderer.mGeomState.emissiveColor->g +
			mRenderer.mGeomState.emissiveColor->b > kQ3RealZero) &&
		(
			((outVertex.flags & kVertexHaveEmissive) == 0) ||
			(mRenderer.mGeomState.highlightState == kQ3On) )
		) 
	{
		outVertex.emissiveColor = *mRenderer.mGeomState.emissiveColor;
		outVertex.flags |= kVertexHaveEmissive;
	}
}

void	TransBuffer::AddTriMesh(
											const TQ3TriMeshData& inGeomData,
											const MeshArrays& inData )
{
	if (! mRenderer.IsFirstPass())
	{
		return;
	}

	// Transform points to camera space.
	const TQ3Matrix4x4&	localToCamera(
		mRenderer.mMatrixState.GetLocalToCamera() );
	mWorkCameraPts.resizeNotPreserving( inGeomData.numPoints );
	E3Point3D_To3DTransformArray( inData.vertPosition, &localToCamera,
		&mWorkCameraPts[0], inGeomData.numPoints,
		sizeof(TQ3Point3D), sizeof(TQ3Point3D) );

	// Find which points are in front of the camera (z <= 0).
	// If all z values are positive (behind the camera), we can bail early.
	mWorkIsInFrontOfCamera.resizeNotPreserving( inGeomData.numPoints );
	bool	isBehindCamera = true;
	TQ3Uns32 i;
	for (i = 0; i < inGeomData.numPoints; ++i)
	{
		bool inFront = (mWorkCameraPts[i].z <= 0.0f);
		// I initially thought the comparison above should be <,
		// but then the rasterize test in Geom Test failed.
		mWorkIsInFrontOfCamera[i] = inFront;
		if (inFront)
		{
			isBehindCamera = false;
		}
	}
	if (isBehindCamera)
	{
		return;
	}

	// Transform normals to camera space.
	const TQ3Matrix4x4&	localToCameraInverseTranspose(
		mRenderer.mMatrixState.GetLocalToCameraInverseTranspose() );
	mWorkCameraNormals.resizeNotPreserving( inGeomData.numPoints );
	E3Vector3D_To3DTransformArray( inData.vertNormal,
		&localToCameraInverseTranspose, &mWorkCameraNormals[0],
		inGeomData.numPoints, sizeof(TQ3Vector3D), sizeof(TQ3Vector3D) );
	
	// Normalize the normals.
	for (i = 0; i < inGeomData.numPoints; ++i)
	{
		Q3FastVector3D_Normalize( &mWorkCameraNormals[i], &mWorkCameraNormals[i] );
	}
	
	// Record camera to frustum matrix
	const TQ3Matrix4x4&	cameraToFrustum(
		mRenderer.mMatrixState.GetCameraToFrustum() );
	if ( mCameraToFrustumMatrices.empty() ||
		(! IsSame4x4( mCameraToFrustumMatrices.back(), cameraToFrustum )) )
	{
		mCameraToFrustumMatrices.push_back( cameraToFrustum );
	}
	TQ3Uns32 cameraToFrustumIndex = static_cast<TQ3Uns32>(mCameraToFrustumMatrices.size() - 1);
	
	// Make a new block.
	TransparentBlock* theBlock = new TransparentBlock;
	theBlock->mPrims.reserve( inGeomData.numTriangles );
	
	if ((inData.faceColor == NULL) || (inData.vertColor != NULL))
	{
		theBlock->mHasUniformVertexFlags = true;
	}
	
	// Transform the points to frustum space.
	mWorkFrustumPts.resizeNotPreserving( inGeomData.numPoints );
	E3Point3D_To3DTransformArray( &mWorkCameraPts[0], &cameraToFrustum,
		&mWorkFrustumPts[0], inGeomData.numPoints,
		sizeof(TQ3Point3D), sizeof(TQ3Point3D) );
	
	// Find the bounds in frustum space.
	E3BoundingBox_SetFromPoints3D( &theBlock->mFrustumBounds,
		&mWorkFrustumPts[0], inGeomData.numPoints, sizeof(TQ3Point3D) );
	
	// We need to create a lot of primitives.  First fill in the fields that do
	// not change.
	Vertex protoVert;
	MakeVertexPrototype( inData, protoVert );
	TransparentPrim thePrim;
	thePrim.mNumVerts = 3;
	thePrim.mVerts[0] = protoVert;
	thePrim.mVerts[1] = protoVert;
	thePrim.mVerts[2] = protoVert;
	thePrim.mCameraToFrustumIndex = cameraToFrustumIndex;
	thePrim.mFillStyle = mRenderer.mStyleState.mFill;
	thePrim.mOrientationStyle = mRenderer.mStyleState.mOrientation;
	thePrim.mBackfacingStyle = mRenderer.mStyleState.mBackfacing;
	thePrim.mSpecularColor = *mRenderer.mGeomState.specularColor;
	thePrim.mSpecularControl = mRenderer.mCurrentSpecularControl;
	thePrim.mIlluminationType = mRenderer.mViewIllumination;
	thePrim.mFogStyleIndex = mRenderer.mStyleState.mCurFogStyleIndex;
	thePrim.mLineWidthStyle = mRenderer.mLineWidth;
	thePrim.mInterpolationStyle = mRenderer.mStyleState.mInterpolation;
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
		thePrim.mUVTransformIndex = static_cast<TQ3Uns32>(mUVTransforms.size() - 1);
	}
	
	// Add the primitives, filling in the varying fields mVerts, mSortingDepth.
	for (i = 0; i < inGeomData.numTriangles; ++i)
	{
		const TQ3Uns32* vertIndices = inGeomData.triangles[ i ].pointIndices;
		thePrim.mVerts[0].point = mWorkCameraPts[ vertIndices[0] ];
		thePrim.mVerts[1].point = mWorkCameraPts[ vertIndices[1] ];
		thePrim.mVerts[2].point = mWorkCameraPts[ vertIndices[2] ];
		thePrim.mVerts[0].normal = mWorkCameraNormals[ vertIndices[0] ];
		thePrim.mVerts[1].normal = mWorkCameraNormals[ vertIndices[1] ];
		thePrim.mVerts[2].normal = mWorkCameraNormals[ vertIndices[2] ];
		if (inData.vertUV != NULL)
		{
			thePrim.mVerts[0].uv = inData.vertUV[ vertIndices[0] ];
			thePrim.mVerts[1].uv = inData.vertUV[ vertIndices[1] ];
			thePrim.mVerts[2].uv = inData.vertUV[ vertIndices[2] ];
		}
		thePrim.mSortingDepth = CalcPrimDepth( thePrim );
		theBlock->mPrims.push_back( thePrim );
		mIsSortNeeded = true;
	}
	
	AddBlock( theBlock );
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
	if (mIsSortNeeded)
	{
		SortBlocks();
		SortPrimPtrsInEachBlock();

		mIsSortNeeded = false;
	}
}

void	TransBuffer::SortPrimPtrsInEachBlock()
{
	for (TQ3Uns32 i = 0; i < mBlocks.size(); ++i)
	{
		mBlocks[i]->SortPrimPtrs();
	}
}

void	TransBuffer::SearchBlock( TQ3Uns32 inToVisit,
									TQ3Int32& ioNextID )
{
	for (TQ3Uns32 i = 0; i < mBlocks.size(); ++i)
	{
		if ( (i != inToVisit) &&
			(mBlocks[i]->mVisitOrder < 0) &&
			mBlocks[inToVisit]->Occludes( *mBlocks[i] ) )
		{
			SearchBlock( i, ioNextID );
		}
	}
	
	ioNextID += 1;
	mBlocks[inToVisit]->mVisitOrder = ioNextID;
}


/*!
	@function	SortBlocks
	
	@abstract	Sort the blocks back to front by depth-first search.
	
	@discussion	The Occludes relation on TransparentBlock objects defines a
				directed acyclic graph.  In such a situation, one can use
				depth-first search to do a "topological sort", producing a
				linear ordering consistent with the relation.
*/
void	TransBuffer::SortBlocks()
{
	// Assign the mVisitOrder members of blocks.
	TQ3Int32 nextID = -1;
	
	for (TQ3Uns32 i = 0; i < mBlocks.size(); ++i)
	{
		if (mBlocks[i]->mVisitOrder < 0)
		{
			SearchBlock( i, nextID );
		}
	}
	
	// Sort the blocks.
	std::sort( &mBlocks[0], &mBlocks[mBlocks.size()], BlockOrder() );
}

void	TransBuffer::Cleanup()
{
	mCameraToFrustumMatrices.clear();
	mUVTransforms.clear();
	mRenderGroup.clear();
	
	for (TQ3Uns32 i = 0; i < mBlocks.size(); ++i)
	{
		delete mBlocks[i];
	}
	mBlocks.clear();
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
	mCurCameraToFrustumIndex = UINT32_MAX;

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
	mIsSortNeeded = false;
	
	TQ3FillStyle	theFillStyle = kQ3FillStyleFilled;
	mRenderer.UpdateFillStyle( &theFillStyle );
	
	TQ3OrientationStyle	theOrientation = kQ3OrientationStyleCounterClockwise;
	mRenderer.UpdateOrientationStyle( &theOrientation );
	
	TQ3BackfacingStyle	theBackfacing = kQ3BackfacingStyleRemove;
	mRenderer.UpdateBackfacingStyle( &theBackfacing );
	
	glBlendFunc( mSrcBlendFactor, mDstBlendFactor );

	glDisable( GL_TEXTURE_2D );
	mCurTexture = UINT32_MAX;	// force initial update
	mCurUVTransformIndex = UINT32_MAX;
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
		
		mPerPixelLighting.UpdateTexture( mCurTexture != 0 );
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
	if (inPrim.mFogStyleIndex != mRenderer.mStyleState.mCurFogStyleIndex)
	{
		mRenderer.UpdateFogStyle(
			&mRenderer.mStyleState.mFogStyles[ inPrim.mFogStyleIndex ] );
	}
}

void	TransBuffer::UpdateFill( const TransparentPrim& inPrim )
{
	if (inPrim.mFillStyle != mRenderer.mStyleState.mFill)
	{
		mRenderer.UpdateFillStyle( &inPrim.mFillStyle );
	}
}

void	TransBuffer::UpdateOrientation( const TransparentPrim& inPrim )
{
	if (inPrim.mOrientationStyle != mRenderer.mStyleState.mOrientation)
	{
		mRenderer.UpdateOrientationStyle( &inPrim.mOrientationStyle );
	}
}

void	TransBuffer::UpdateBackfacing( const TransparentPrim& inPrim )
{
	if (inPrim.mBackfacingStyle != mRenderer.mStyleState.mBackfacing)
	{
		mRenderer.UpdateBackfacingStyle( &inPrim.mBackfacingStyle );
	}
}

void	TransBuffer::UpdateInterpolation( const TransparentPrim& inPrim )
{
	if (inPrim.mInterpolationStyle != mRenderer.mStyleState.mInterpolation)
	{
		mRenderer.UpdateInterpolationStyle( &inPrim.mInterpolationStyle );
	}
}

void	TransBuffer::UpdateLineWidth( const TransparentPrim& inPrim )
{
	if (inPrim.mLineWidthStyle != mRenderer.mLineWidth)
	{
		mRenderer.UpdateLineWidthStyle( inPrim.mLineWidthStyle );
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
	// Premultiply alpha, so we can use GL_ONE as the source blend factor
	// for both vertices and textures.
	GLfloat	color4[4] = {
		inVert.diffuseColor.r * inVert.vertAlpha,
		inVert.diffuseColor.g * inVert.vertAlpha,
		inVert.diffuseColor.b * inVert.vertAlpha,
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


void	TransBuffer::UpdateEmission( const TransparentPrim& inPrim )
{
	// Previously, we were setting emissive color inside the loop, but there
	// were problems with emissive color leaking from some transparent triangles
	// to other transparent triangles that should not have had emission, when
	// using per-pixel lighting.
	// Setting the emissive color before glBegin fixes that problem, and I think
	// we can live without per-vertex emissive color.
	if ((inPrim.mVerts[0].flags & kVertexHaveEmissive) != 0)
	{
		SetEmissiveColor( inPrim.mVerts[0].emissiveColor );
	}
	else
	{
		SetEmissiveColor( kBlackColor );
	}
}


void	TransBuffer::Render( const TransparentPrim& inPrim )
{
	// Render geometries of dimension 0 or 1 with only ambient light.
	mRenderer.mLights.SetOnlyAmbient( inPrim.mNumVerts < 3 );
	
	// Maybe update fragment program.
	mPerPixelLighting.PreGeomSubmit( NULL );

	switch (inPrim.mNumVerts)
	{
		case 3:
			glBegin( GL_TRIANGLES );
			break;

		case 2:
			glBegin( GL_LINES );
			break;

		case 1:
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
		
		glVertex3fv( (const GLfloat *) &theVert.point );
	}
	
	glEnd();
}


void	TransBuffer::RenderPrimGroupForDepth(
										TQ3ViewObject inView )
{
	const TransparentPrim& leader( *mRenderGroup[0] );
	
	UpdateCameraToFrustum( leader, inView );
	UpdateTexture( leader );
	UpdateFill( leader );
	UpdateOrientation( leader );
	UpdateBackfacing( leader );
	UpdateLineWidth( leader );

	if (mRenderGroup.size() == 1)
	{
		RenderForDepth( leader );
	}
	else
	{
		VertexFlags flags = leader.mVerts[0].flags;
		TQ3Uns32 vertsPerPrim = leader.mNumVerts;
		TQ3Uns32 pointsExpected = vertsPerPrim * mRenderGroup.size();
		mGroupPts.clear();
		mGroupPts.reserve( pointsExpected );
		
		mRenderer.mLights.SetOnlyAmbient( vertsPerPrim < 3 );
		
		bool haveUV = (leader.mTextureName != 0) && ((flags & kVertexHaveUV) != 0);
		bool haveColor = ((flags & kVertexHaveDiffuse) != 0);
		if (haveUV)
		{
			mGroupUVs.clear();
			mGroupUVs.reserve( pointsExpected );
		}
		if (haveColor)
		{
			mGroupColors.clear();
			mGroupColors.reserve( pointsExpected );
		}

		// Enable or disable client arrays.  (The vertex array is always enabled.)
		mRenderer.mGLClientStates.EnableNormalArray( false );
		mRenderer.mGLClientStates.EnableTextureArray( haveUV );
		mRenderer.mGLClientStates.EnableColorArray( haveColor );

		// Gather data from primitives into arrays.
		TQ3Uns32 i, j;
		for (i = 0; i < mRenderGroup.size(); ++i)
		{
			const TransparentPrim& aPrim( *mRenderGroup[i] );
			
			for (j = 0; j < vertsPerPrim; ++j)
			{
				const Vertex& aVertex( aPrim.mVerts[j] );
				mGroupPts.push_back( aVertex.point );
				if ( haveUV )
				{
					mGroupUVs.push_back( aVertex.uv );
				}
				if ( haveColor )
				{
					TQ3ColorRGBA theColor =
					{
						aVertex.diffuseColor.r * aVertex.vertAlpha,
						aVertex.diffuseColor.g * aVertex.vertAlpha,
						aVertex.diffuseColor.b * aVertex.vertAlpha,
						aVertex.vertAlpha
					};
					mGroupColors.push_back( theColor );
				}
			}
		}

	
		// Pass array pointers to OpenGL.
		glVertexPointer( 3, GL_FLOAT, 0, &mGroupPts[0] );
		if ( haveUV )
		{
			glTexCoordPointer( 2, GL_FLOAT, 0, &mGroupUVs[0] );
		}
		if ( haveColor )
		{
			glColorPointer( 4, GL_FLOAT, 0, &mGroupColors[0] );
		}
		
		// draw an array
		GLenum arrayMode = GL_TRIANGLES;
		if (vertsPerPrim == 2)
		{
			arrayMode = GL_LINES;
		}
		else if (vertsPerPrim == 1)
		{
			arrayMode = GL_POINTS;
		}
		glDrawArrays( arrayMode, 0, mRenderGroup.size() * vertsPerPrim );
	}
}

void	TransBuffer::RenderForDepth( const TransparentPrim& inPrim )
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
		
		if ( (mCurTexture != 0) && ((theVert.flags & kVertexHaveUV) != 0) )
		{
			glTexCoord2fv( (const GLfloat *) &theVert.uv );
		}
		
		if ((theVert.flags & kVertexHaveDiffuse) != 0)
		{
			SetDiffuseColor( theVert );
		}
		
		glVertex3fv( (const GLfloat *) &theVert.point );
	}
	
	glEnd();
}



void	TransBuffer::UpdateSpecularColor( const TQ3ColorRGB& inColor )
{
	if ( (inColor.r != mCurSpecularColor[0]) ||
		(inColor.g != mCurSpecularColor[1]) ||
		(inColor.b != mCurSpecularColor[2]) )
	{
		mCurSpecularColor[0] = inColor.r;
		mCurSpecularColor[1] = inColor.g;
		mCurSpecularColor[2] = inColor.b;
		mCurSpecularColor[3] = 1.0f;
		
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mCurSpecularColor );
	}
}

void	TransBuffer::UpdateSpecular( const TransparentPrim& inPrim )
{
	if (inPrim.mIlluminationType == kQ3IlluminationTypePhong)
	{
		UpdateSpecularColor( inPrim.mSpecularColor );
		
		if (inPrim.mSpecularControl != mCurSpecularControl)
		{
			mCurSpecularControl = inPrim.mSpecularControl;
			
			GLfloat		shininess = GLUtils_SpecularControlToGLShininess(
				mCurSpecularControl );
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shininess );
		}
	}
	else
	{
		UpdateSpecularColor( kBlackColor );
	}
}

void	TransBuffer::RenderPrimGroup(
											TQ3ViewObject inView )
{
	const TransparentPrim& leader( *mRenderGroup[0] );
	
	UpdateCameraToFrustum( leader, inView );
	UpdateLightingEnable( leader );
	UpdateTexture( leader );
	UpdateFog( leader );
	UpdateFill( leader );
	UpdateOrientation( leader );
	UpdateBackfacing( leader );
	UpdateLineWidth( leader );
	UpdateInterpolation( leader );
	mPerPixelLighting.UpdateIllumination( leader.mIlluminationType );
	UpdateSpecular( leader );
	UpdateEmission( leader );

	if (mRenderGroup.size() == 1)
	{
		Render( leader );
	}
	else
	{
		VertexFlags flags = leader.mVerts[0].flags;
		TQ3Uns32 vertsPerPrim = leader.mNumVerts;
		TQ3Uns32 pointsExpected = vertsPerPrim * mRenderGroup.size();
		mGroupPts.clear();
		mGroupPts.reserve( pointsExpected );
		
		mRenderer.mLights.SetOnlyAmbient( vertsPerPrim < 3 );

		// Maybe update fragment program.
		mPerPixelLighting.PreGeomSubmit( NULL );

		bool haveNormal = ((flags & kVertexHaveNormal) != 0);
		bool haveUV = (leader.mTextureName != 0) && ((flags & kVertexHaveUV) != 0);
		bool haveColor = ((flags & kVertexHaveDiffuse) != 0);
		
		if (haveNormal)
		{
			mGroupNormals.clear();
			mGroupNormals.reserve( pointsExpected );
		}
		if (haveUV)
		{
			mGroupUVs.clear();
			mGroupUVs.reserve( pointsExpected );
		}
		if (haveColor)
		{
			mGroupColors.clear();
			mGroupColors.reserve( pointsExpected );
		}
		
		// Enable or disable client arrays.  (The vertex array is always enabled.)
		mRenderer.mGLClientStates.EnableNormalArray( haveNormal );
		mRenderer.mGLClientStates.EnableTextureArray( haveUV );
		mRenderer.mGLClientStates.EnableColorArray( haveColor );
		
		// Gather data from primitives into arrays.
		TQ3Uns32 i, j;
		for (i = 0; i < mRenderGroup.size(); ++i)
		{
			const TransparentPrim& aPrim( *mRenderGroup[i] );
			
			for (j = 0; j < vertsPerPrim; ++j)
			{
				const Vertex& aVertex( aPrim.mVerts[j] );
				mGroupPts.push_back( aVertex.point );
				if (haveNormal)
				{
					mGroupNormals.push_back( aVertex.normal );
				}
				if ( haveUV )
				{
					mGroupUVs.push_back( aVertex.uv );
				}
				if ( haveColor )
				{
					TQ3ColorRGBA theColor =
					{
						aVertex.diffuseColor.r * aVertex.vertAlpha,
						aVertex.diffuseColor.g * aVertex.vertAlpha,
						aVertex.diffuseColor.b * aVertex.vertAlpha,
						aVertex.vertAlpha
					};
					mGroupColors.push_back( theColor );
				}
			}
		}
	
		// Pass array pointers to OpenGL.
		glVertexPointer( 3, GL_FLOAT, 0, &mGroupPts[0] );
		if (haveNormal)
		{
			glNormalPointer( GL_FLOAT, 0, &mGroupNormals[0] );
		}
		if ( haveUV )
		{
			glTexCoordPointer( 2, GL_FLOAT, 0, &mGroupUVs[0] );
		}
		if ( haveColor )
		{
			glColorPointer( 4, GL_FLOAT, 0, &mGroupColors[0] );
		}
		
		// draw an array
		GLenum arrayMode = GL_TRIANGLES;
		if (vertsPerPrim == 2)
		{
			arrayMode = GL_LINES;
		}
		else if (vertsPerPrim == 1)
		{
			arrayMode = GL_POINTS;
		}
		glDrawArrays( arrayMode, 0, mRenderGroup.size() * vertsPerPrim );
	}
}

void	TransBuffer::DrawTransparency( TQ3ViewObject inView,
										GLenum inSrcBlendFactor,
										GLenum inDstBlendFactor )
{
	if (! mBlocks.empty())
	{
		mSrcBlendFactor = inSrcBlendFactor;
		mDstBlendFactor = inDstBlendFactor;
		
		SortIndices();

		// Previously I was calling glPushAttrib here and calling glPopAttrib at
		// the end of the scope, to restore some state changed by InitGLState.
		// However, I do not think it is necessary, and it is preferable to
		// avoid glPushAttrib/glPopAttrib due to buggy drivers.
		
		InitGLState( inView );
		
		mRenderGroup.clear();
		mRenderGroup.reserve( kRenderGroupReserve );
		const TransparentPrim* gpLeader = NULL;

		for (TQ3Uns32 blockNum = 0; blockNum < mBlocks.size(); ++blockNum)
		{
			TransparentBlock& block( *mBlocks[blockNum] );
			for (TQ3Uns32 primNum = 0; primNum < block.mPrims.size(); ++primNum)
			{
				const TransparentPrim& thePrim( *block.mPrimPtrs[primNum] );
				if (gpLeader == NULL)
				{
					gpLeader = &thePrim;
					mRenderGroup.push_back( gpLeader );
				}
				else if ( ((primNum > 0) && block.mHasUniformVertexFlags) ||
					IsSameState( thePrim, *gpLeader ) )
				{
					mRenderGroup.push_back( &thePrim );
				}
				else // finish the group and start another
				{
					RenderPrimGroup( inView );
					mRenderGroup.clear();
					gpLeader = &thePrim;
					mRenderGroup.push_back( gpLeader );
				}
			}
		}

		if (mRenderGroup.size() > 0)
		{
			RenderPrimGroup( inView );
		}
	}
}

void	TransBuffer::InitGLStateForDepth( TQ3ViewObject inView,
											TQ3Float32 inAlphaThreshold )
{
	// We need to clear any transforms which are active when the rendering loop ended,
	// since transparent primitives are already transformed into camera coordinates,
	// and the camera to frustum matrix will be updated later.
	Q3Matrix4x4_SetIdentity( &mCurCameraTransform.localToWorld );
	Q3Matrix4x4_SetIdentity( &mCurCameraTransform.worldToCamera );
	Q3Matrix4x4_SetIdentity( &mCurCameraTransform.cameraToFrustum );
	Q3CameraTransform_Submit (&mCurCameraTransform, inView );
	mCurCameraToFrustumIndex = UINT32_MAX;
	
	TQ3FillStyle	theFillStyle = kQ3FillStyleFilled;
	mRenderer.UpdateFillStyle( &theFillStyle );
	
	TQ3OrientationStyle	theOrientation = kQ3OrientationStyleCounterClockwise;
	mRenderer.UpdateOrientationStyle( &theOrientation );
	
	TQ3BackfacingStyle	theBackfacing = kQ3BackfacingStyleRemove;
	mRenderer.UpdateBackfacingStyle( &theBackfacing );
	
	// Turn off unneeded fragment operations
	glDisable( GL_LIGHTING );
	glDisable( GL_BLEND );
	glDisable( GL_TEXTURE_2D );
	mCurTexture = UINT32_MAX;
	mCurUVTransformIndex = UINT32_MAX;
	mCurUBoundary = kQ3ShaderUVBoundarySize32;
	mCurVBoundary = kQ3ShaderUVBoundarySize32;
	glDisable( GL_DITHER );
	
	// Turn off writing to the color buffer
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	
	// Turn on writing to the depth buffer
	glDepthMask( GL_TRUE );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	
	// Set up alpha test
	glEnable( GL_ALPHA_TEST );
	glAlphaFunc( GL_GREATER, inAlphaThreshold );
	
	// specular: set to illegal values to force initial update
	mCurSpecularControl = -1.0f;
	mCurSpecularColor[0] = -1.0f;
	
	// similar for diffuse color
	mCurDiffuseColor[3] = -1.0f;
	
	mCurEmissiveColor = kBlackColor;
	mRenderer.SetEmissiveMaterial( mCurEmissiveColor );
}

/*!
	@function	DrawDepth
	
	@abstract	If a special renderer property is present, draw the geometry
				again, affecting only the depth buffer.
*/
void	TransBuffer::DrawDepth( TQ3ViewObject inView )
{
	TQ3Float32	alphaThreshold = 1.0f;
	Q3Object_GetProperty( mRenderer.mRendererObject,
		kQ3RendererPropertyDepthAlphaThreshold, sizeof(alphaThreshold), NULL,
		&alphaThreshold );

	if (alphaThreshold < 1.0f)
	{
		InitGLStateForDepth( inView, alphaThreshold );

		SortIndices();
		
		mRenderGroup.clear();
		mRenderGroup.reserve( kRenderGroupReserve );
		const TransparentPrim* gpLeader = NULL;

		for (TQ3Uns32 blockNum = 0; blockNum < mBlocks.size(); ++blockNum)
		{
			TransparentBlock& block( *mBlocks[blockNum] );
			for (TQ3Uns32 primNum = 0; primNum < block.mPrims.size(); ++primNum)
			{
				const TransparentPrim& thePrim( *block.mPrimPtrs[primNum] );
				if (gpLeader == NULL)
				{
					gpLeader = &thePrim;
					mRenderGroup.push_back( gpLeader );
				}
				else if ( ((primNum > 0) && block.mHasUniformVertexFlags) ||
					IsSameStateForDepth( thePrim, *gpLeader ) )
				{
					mRenderGroup.push_back( &thePrim );
				}
				else // finish the group and start another
				{
					RenderPrimGroupForDepth( inView );
					mRenderGroup.clear();
					gpLeader = &thePrim;
					mRenderGroup.push_back( gpLeader );
				}
			}
		}
				
		if (mRenderGroup.size() > 0)
		{
			RenderPrimGroupForDepth( inView );
		}
		
		// Restore GL state
		glDisable( GL_ALPHA_TEST );
		glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	}
}

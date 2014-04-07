/*  NAME:
       QOTransBuffer.h

    DESCRIPTION:
        Header for Quesa OpenGL renderer class.
		    
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

#include "QOPrefix.h"

#include <vector>


//=============================================================================
//      Class declarations
//-----------------------------------------------------------------------------

namespace QORenderer
{

class Renderer;
class PerPixelLighting;
struct MeshArrays;

struct TransparentPrim
{
	TQ3Uns32			mNumVerts;
	Vertex				mVerts[3];	// points and normals in camera coordinates
	float				mSortingDepth;
	
	GLuint				mTextureName;
	bool				mIsTextureTransparent;
	TQ3ShaderUVBoundary	mShaderUBoundary;
	TQ3ShaderUVBoundary	mShaderVBoundary;
	TQ3Uns32			mUVTransformIndex;
	
	TQ3FillStyle		mFillStyle;
	TQ3OrientationStyle	mOrientationStyle;
	TQ3BackfacingStyle	mBackfacingStyle;
	TQ3InterpolationStyle	mInterpolationStyle;
	TQ3ColorRGB			mSpecularColor;
	float				mSpecularControl;
	TQ3ObjectType		mIlluminationType;
	TQ3Uns32			mFogStyleIndex;
	TQ3Uns32			mCameraToFrustumIndex;
	float				mLineWidthStyle;
};

/*!
	@class				TransparentBlock
	
	@abstract			Buffer for a group of transparent primitives, whose
						bounding box in frustum space should be disjoint from
						each other such block.
*/
class TransparentBlock
{
public:
						TransparentBlock();
						~TransparentBlock() {}
	
	bool				Intersects( const TransparentBlock& inOther ) const;
	void				Union( const TransparentBlock& inOther );
	
	bool				Occludes( const TransparentBlock& inOther ) const;
	
	void				SortPrimPtrs();
						
	E3FastArray<TransparentPrim>			mPrims;
	TQ3BoundingBox							mFrustumBounds;
	E3FastArray<const TransparentPrim*>		mPrimPtrs;
	TQ3Int32								mVisitOrder;
	bool									mHasUniformVertexFlags;

private:
						TransparentBlock( const TransparentBlock& inOther );
	TransparentBlock&	operator=( const TransparentBlock& inOther );
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

	void							AddTriMesh(
											const TQ3TriMeshData& inGeomData,
											const MeshArrays& inData );
	
	void							DrawTransparency(
											TQ3ViewObject inView,
											GLenum inSrcBlendFactor,
											GLenum inDstBlendFactor );
	
	void							DrawDepth(
											TQ3ViewObject inView );

	void							Cleanup();
	
	inline bool						HasContent() const { return ! mBlocks.empty(); }

private:
	void							AddPrim(
											int inNumVerts,
											const Vertex* inVertices );

	void							AddBlock( TransparentBlock* ioBlock );

	void							SortPrimPtrsInEachBlock();
	void							SortBlocks();
	void							SearchBlock( TQ3Uns32 inToVisit,
												TQ3Int32& ioNextID );
	void							SortIndices();
	void							MakeVertexPrototype(
											const MeshArrays& inData,
											Vertex& outVertex ) const;
	void							InitGLState(
											TQ3ViewObject inView );
	void							InitGLStateForDepth(
											TQ3ViewObject inView,
											TQ3Float32 inAlphaThreshold );
	void							SetEmissiveColor( const TQ3ColorRGB& inColor );
	void							SetDiffuseColor(
											const QORenderer::Vertex& inVert );
	void							SetDiffuseColor(
											const GLfloat* inColor4 );
	
	void							UpdateCameraToFrustum(
											const TransparentPrim& inPrim,
											TQ3ViewObject inView );
	void							UpdateLightingEnable(
											const TransparentPrim& inPrim );
	void							UpdateTexture(
											const TransparentPrim& inPrim );
	void							UpdateSpecular(
											const TransparentPrim& inPrim );
	void							UpdateSpecularColor(
											const TQ3ColorRGB& inColor );
	void							UpdateFog(
											const TransparentPrim& inPrim );
	void							UpdateFill(
											const TransparentPrim& inPrim );
	void							UpdateOrientation(
											const TransparentPrim& inPrim );
	void							UpdateBackfacing(
											const TransparentPrim& inPrim );
	void							UpdateLineWidth(
											const TransparentPrim& inPrim );
	void							UpdateInterpolation(
											const TransparentPrim& inPrim );
	void							UpdateEmission(
											const TransparentPrim& inPrim );

	void							Render(
											const TransparentPrim& inPrim );
	void							RenderForDepth(
											const TransparentPrim& inPrim );
	void							RenderPrimGroup(
											TQ3ViewObject inView );
	void							RenderPrimGroupForDepth(
											TQ3ViewObject inView );
	
	Renderer&						mRenderer;
	PerPixelLighting&				mPerPixelLighting;
	
	// Buffers used when accumulating primitives
	std::vector<TQ3Matrix4x4>		mCameraToFrustumMatrices;
	std::vector<TQ3Matrix3x3>		mUVTransforms;
	E3FastArray<TQ3Point3D>			mWorkCameraPts;
	E3FastArray<TQ3Point3D>			mWorkFrustumPts;
	E3FastArray<TQ3Vector3D>		mWorkCameraNormals;
	E3FastArray<bool>				mWorkIsInFrontOfCamera;
	E3FastArray<TransparentBlock*>	mBlocks;
	
	// State used when flushing (drawing) primitives
	bool							mIsLightingEnabled;
	bool							mIsSortNeeded;
	TQ3Uns32						mCurCameraToFrustumIndex;
	TQ3CameraTransformData			mCurCameraTransform;
	GLuint							mCurTexture;
	TQ3Uns32						mCurUVTransformIndex;
	TQ3ShaderUVBoundary				mCurUBoundary;
	TQ3ShaderUVBoundary				mCurVBoundary;
	GLfloat							mCurSpecularColor[4];
	float							mCurSpecularControl;
	TQ3ColorRGB						mCurEmissiveColor;
	GLfloat							mCurDiffuseColor[4];
	GLenum							mSrcBlendFactor;
	GLenum							mDstBlendFactor;
	
	E3FastArray<const TransparentPrim*>	mRenderGroup;
	E3FastArray<TQ3Point3D>			mGroupPts;
	E3FastArray<TQ3Vector3D>		mGroupNormals;
	E3FastArray<TQ3Param2D>			mGroupUVs;
	E3FastArray<TQ3ColorRGBA>		mGroupColors;
};

}

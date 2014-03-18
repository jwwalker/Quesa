/*!
	@header		QORenderer.h
	
	This is the header for the main class of the Quesa OpenGL renderer.
*/

/*  NAME:
       QORenderer.h

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
#ifndef QORENDERER_HDR
#define QORENDERER_HDR

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QOPrefix.h"

#include "GLTextureManager.h"
#include "GLVBOManager.h"
#include "QOLights.h"
#include "QOTexture.h"
#include "QOClientStates.h"
#include "QOMatrix.h"
#include "QOOpaqueTriBuffer.h"
#include "QOTransBuffer.h"
#include "QOGLShadingLanguage.h"
#include "QOCalcTriMeshEdges.h"

#include <vector>



namespace QORenderer
{
//=============================================================================
//    Constants
//-----------------------------------------------------------------------------

/*!
	@enum		ESlowPathMask
	@abstract	Bit masks indicating possible reasons why a TriMesh is not on
				the fast path.
*/
enum ESlowPathMask
{
	kSlowPathMask_FastPath				= 0,
	kSlowPathMask_NoVertexNormals		= (1 << 0),
	kSlowPathMask_FaceColors			= (1 << 1),
	kSlowPathMask_Transparency			= (1 << 2),
	kSlowPathMask_EmissiveColor			= (1 << 3),
	kSlowPathMask_FaceTextures			= (1 << 4)
};

//=============================================================================
//     Subsidiary Types
//-----------------------------------------------------------------------------

typedef TQ3Uns32	SlowPathMask;

// glBlendEquation type
typedef void (QO_PROCPTR_TYPE TQ3BlendEquationProcPtr) (GLenum blendType);

/*!
	@struct		ColorState
	@abstract	Structure to hold some color-related values.
*/
struct ColorState
{
	void				Reset();

	const TQ3ColorRGB*	diffuseColor;
	const TQ3ColorRGB*	specularColor;
	const TQ3ColorRGB*	emissiveColor;
	float				specularControl;
	float				alpha;
	TQ3Switch			highlightState;
};

/*!
	@struct		StyleState
	@abstract	Structure to hold current values of styles.
*/
struct StyleState
{
	TQ3InterpolationStyle	mInterpolation;
	TQ3BackfacingStyle		mBackfacing;
	TQ3FillStyle			mFill;
	TQ3OrientationStyle		mOrientation;
	CQ3ObjectRef			mHilite;	
	std::vector<TQ3FogStyleData>	mFogStyles;
	TQ3Uns32				mCurFogStyleIndex;
	bool					mIsCastingShadows;
	bool					mExplicitEdges;
};

/*!
	@class		GLContextCleanup
	@abstract	Class to help ensure that a GL draw context gets destroyed at
				the right time.
*/
class GLContextCleanup
{
public:
					GLContextCleanup( TQ3GLContext& inContext )
						: mContext( inContext ) {}
					
					~GLContextCleanup();

private:
	TQ3GLContext&	mContext;
};

/*!
	@struct		MeshArrays
	@abstract	Data arrays extracted from TQ3TriMeshData.
*/
struct MeshArrays
{
	const TQ3Point3D*	vertPosition;
	const TQ3Vector3D*	vertNormal;
	const TQ3Param2D*	vertUV;
	const TQ3ColorRGB*	vertColor;
	const TQ3ColorRGB*	vertTransparency;
	const TQ3ColorRGB*	vertEmissive;

	const TQ3Vector3D*	faceNormal;
	const TQ3ColorRGB*	faceColor;
	const TQ3ColorRGB*	faceTransparency;
	const TQ3ColorRGB*	faceEmissive;
	TQ3Object*			faceSurfaceShader;
	
	const TQ3ColorRGB*	edgeColor;
};

// Function pointer type for GL_EXT_stencil_two_side
typedef void (QO_PROCPTR_TYPE glActiveStencilFaceEXTProcPtr) (GLenum face);

// Function pointer types for separate stencil, GL 2.0
typedef void (QO_PROCPTR_TYPE glStencilFuncSeparateProcPtr) (GLenum face, GLenum func, GLint ref, GLuint mask);
typedef void (QO_PROCPTR_TYPE glStencilOpSeparateProcPtr) (GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
typedef void (QO_PROCPTR_TYPE glStencilMaskSeparateProcPtr) (GLenum face, GLuint mask);

/*!
	@struct		GLStencilFuncs
	@abstract	OpenGL function pointers for two-sided stencils.
*/
struct GLStencilFuncs
{
								GLStencilFuncs();
								
	void						SetNULL();
	
	/*!
		@function	Initialize
		@abstract	Get the function pointers.  This should be called just
					after the OpenGL context is created.
	*/
	void						Initialize( const TQ3GLExtensions& inExts );
	
	glActiveStencilFaceEXTProcPtr	glActiveStencilFace;
	glStencilFuncSeparateProcPtr	glStencilFuncSeparate;
	glStencilOpSeparateProcPtr		glStencilOpSeparate;
	glStencilMaskSeparateProcPtr	glStencilMaskSeparate;
};

//=============================================================================
//     Main Class
//-----------------------------------------------------------------------------

/*!
	@class		Renderer
	
	@abstract	Class that implements the Quesa OpenGL renderer.
	
	@discussion	All the members are private because they are only called by
				friend classes.  In particular, calls from the plug-in
				functions come through the Statics class.
*/
class Renderer
{
protected:
							Renderer( TQ3RendererObject inRenderer );
							~Renderer();
	
	friend class Statics;
	friend class TransBuffer;
	friend class OpaqueTriBuffer;
	
	//
	//	non-static methods that implement static methods
	//
	TQ3Status				StartFrame(
									TQ3ViewObject inView,
									TQ3DrawContextObject inDrawContext );
	TQ3Status				EndFrame(
									TQ3ViewObject inView );
	void					StartPass(
    								TQ3CameraObject inCamera,
    								TQ3GroupObject inLights );
    TQ3ViewStatus			EndPass(
    								TQ3ViewObject inView );
    
    bool					IsBoundingBoxVisible(
    								TQ3ViewObject inView,
    								const TQ3BoundingBox& inBounds );
	
	bool					SubmitTriMesh(
									TQ3ViewObject inView,
									TQ3GeometryObject inTriMesh,
									const TQ3TriMeshData* inGeomData );
									
	void					SubmitTriangle(
									TQ3ViewObject inView,
									TQ3GeometryObject inTriangle,
									const TQ3TriangleData* inGeomData );
	
	void					SubmitPoint(
									const TQ3PointData* inGeomData );
	
	void					SubmitLine(
									const TQ3LineData* inGeomData );
	void					SubmitPolyLine(
									TQ3ViewObject inView,
									TQ3GeometryObject inPolyLine,
									const TQ3PolyLineData* inGeomData );
	
	TQ3Status				UpdateLocalToCamera(
									TQ3ViewObject inView,
									const TQ3Matrix4x4& inMatrix );
	TQ3Status				UpdateCameraToFrustum(
									TQ3ViewObject inView,
									const TQ3Matrix4x4& inMatrix );
	
	void					UpdateDiffuseColor(
									const TQ3ColorRGB* inAttColor );
	void					UpdateSpecularColor(
									const TQ3ColorRGB* inAttColor );
	void					UpdateTransparencyColor(
									const TQ3ColorRGB* inAttColor );
	void					UpdateEmissiveColor(
									const TQ3ColorRGB* inAttColor );
	void					UpdateSpecularControl(
									const float* inAttValue );
	void					UpdateHiliteState(
									const TQ3Switch* inAttState );
	void					UpdateSurfaceShader(
									TQ3ShaderObject inShader );
	void					UpdateIlluminationShader(
									TQ3ShaderObject inShader );

	void					UpdateInterpolationStyle(
									const TQ3InterpolationStyle* inStyleData );
	void					UpdateBackfacingStyle(
									const TQ3BackfacingStyle* inStyleData );
	void					UpdateFillStyle(
									const TQ3FillStyle* inStyleData );
	void					UpdateOrientationStyle(
									const TQ3OrientationStyle* inStyleData );
	void					UpdateHighlightStyle(
									TQ3AttributeSet* inStyleData );
	void					UpdateAntiAliasStyle(
									TQ3AntiAliasStyleData* inStyleData );
	void					UpdateFogStyle(
									const TQ3FogStyleData* inStyleData );
	void					UpdateCastShadowsStyle(
									TQ3Boolean inStyleData );
	void					UpdateLineWidthStyle(
									float inStyleData );
	

	//
	//	Helper methods
	//
	void					CalcVertexState(
									const TQ3Vertex3D& inSrcVertex,
									Vertex& outVertex );
	void					CalcTriMeshVertState(
									TQ3Uns32 inVertNum,
									TQ3Uns32 inFaceNum,
									const MeshArrays& inData,
									Vertex& outVertex );
	void					HandleGeometryAttributes(
									TQ3AttributeSet inGeomAttSet,
									TQ3ViewObject inView,
									bool inIsTextureRelevant );
	void					UpdateSpecularMaterial();
	void					UpdateEmissiveMaterial();
	void					SetEmissiveMaterial( const TQ3ColorRGB& inColor );
	SlowPathMask			FindTriMeshData(
									const TQ3TriMeshData& inGeomData,
									MeshArrays& outArrays );
	void					RenderFastPathTriMesh(
									TQ3GeometryObject inTriMesh,
									const TQ3TriMeshData& inGeomData,
									const TQ3Vector3D* inVertNormals,
									const TQ3Param2D* inVertUVs,
									const TQ3ColorRGB* inVertColors );
	void					RenderSlowPathTriMesh(
									TQ3ViewObject inView,
									const TQ3TriMeshData& inGeomData,
									const MeshArrays& inData );
	void					RenderExplicitEdges(
									TQ3ViewObject inView,
									const TQ3TriMeshData& inGeomData,
									const TQ3Vector3D* inVertNormals,
									const TQ3ColorRGB* inVertColors,
									const TQ3ColorRGB* inEdgeColors );
	void					RenderCulledEdges(
									TQ3ViewObject inView,
									const TQ3TriMeshData& inGeomData,
									const TQ3ColorRGB* inVertColors,
									const TQ3ColorRGB* inEdgeColors );
	void					RenderFaceEdges(
									TQ3ViewObject inView,
									TQ3GeometryObject inTriMesh,
									const TQ3TriMeshData& inGeomData,
									const TQ3Vector3D* inVertNormals,
									const TQ3ColorRGB* inVertColors );
	void					SimulateSeparateSpecularColor(
									TQ3Uns32 inNumIndices,
									const TQ3Uns32* inIndices );
	bool					IsFakeSeparateSpecularColorNeeded() const;
	bool					IsFirstPass() const { return (mPassIndex == 0) &&
														mLights.IsFirstPass(); }
	void					RenderTransparent( TQ3ViewObject inView );

	
	TQ3RendererObject		mRendererObject;
	TQ3DrawContextObject	mDrawContextObject;
	TQ3GLContext			mGLContext;
	GLContextCleanup		mCleanup;
	GLSLFuncs				mSLFuncs;
	GLBufferFuncs			mBufferFuncs;
	GLStencilFuncs			mStencilFuncs;
	TQ3GLExtensions			mGLExtensions;
	PerPixelLighting		mPPLighting;
	TQ3Uns32				mRendererEditIndex;
	TQ3Uns32				mDrawContextEditIndex;
	GLbitfield				mGLClearFlags;
	TQ3BlendEquationProcPtr	mGLBlendEqProc;
	MatrixState				mMatrixState;
	TQ3Int32				mPassIndex;
	TQ3Int32				mNumPasses;
	bool					mAllowLineSmooth;
	bool					mIsCachingShadows;
	unsigned long long		mNumPrimitivesRenderedInFrame;
	
	// Buffers used temporarily in QOGeometry.cpp, only members to reduce
	// memory allocation
	E3FastArray<char>		mScratchBuffer;
	TQ3EdgeVec				mEdges;
	TQ3TriangleToEdgeVec	mFacesToEdges;
	
	// Color states
	TQ3ObjectType			mViewIllumination;
	ColorState				mViewState;
	ColorState				mGeomState;
	GLfloat					mCurrentSpecularColor[4];
	float					mCurrentSpecularControl;
	TQ3ColorRGB				mCurrentEmissiveColor;
	float 					mLineWidth;
	TQ3XAttributeMask		mAttributesMask;
	bool					mUpdateShader;
	
	// style states
	StyleState				mStyleState;
	
	// OpenGL client state
	ClientStates			mGLClientStates;

	// Light state
	Lights					mLights;
	
	// Buffer for opaque triangles
	OpaqueTriBuffer			mTriBuffer;
	
	// Buffer for transparent stuff
	TransBuffer				mTransBuffer;
	
	// Texture state
	Texture					mTextures;
};

}	// end QORenderer namespace

#endif // QORENDERER_HDR

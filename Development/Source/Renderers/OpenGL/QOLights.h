/*!
	@header		QOLights.h
	
	Lights class for use in Quesa OpenGL renderer.
*/

/*  NAME:
       QOLights.h

    DESCRIPTION:
        Header for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2008, Quesa Developers. All rights reserved.

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
#ifndef QOLIGHTS_HDR
#define QOLIGHTS_HDR

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------

#include "E3Prefix.h"
#include "GLPrefix.h"
#include "QOPrefix.h"
#include "CQ3ObjectRef.h"
#include "QOShadowMarker.h"
#include "QOGLShadingLanguage.h"

#include <cmath>
#include <vector>
#include <limits>


//=============================================================================
//      Class Declaration
//-----------------------------------------------------------------------------

namespace QORenderer
{
typedef std::vector< CQ3ObjectRef >	ObVec;

struct GLStencilFuncs;
class MatrixState;
struct StyleState;

/*!
	@class		Lights

	@abstract	Light handling functions for QORenderer.
*/
class Lights
{
public:
							Lights( const TQ3GLExtensions& inExtensions,
									const GLStencilFuncs& inStencilFuncs,
									const MatrixState& inMatrixState,
									const StyleState& inStyleState,
									PerPixelLighting& ioPerPixelLighting )
								: mGLExtensions( inExtensions )
								, mGLStencilFuncs( inStencilFuncs )
								, mMatrixState( inMatrixState )
								, mStyleState( inStyleState )
								, mPerPixelLighting( ioPerPixelLighting )
								, mLightCount( 0 )
								, mSavedYon( std::numeric_limits<float>::infinity() )
								, mShadowMarker( mMatrixState, mStyleState,
									mGLLightPosition )
								, mIsOnlyAmbient( false ) {}

	void					StartFrame(
									TQ3ViewObject inView,
									bool inIsShadowing );
	
	void					StartPass(
									TQ3CameraObject inCamera,
									TQ3GroupObject inLights );

	bool					EndPass();
	
	void					EndFrame(
									TQ3ViewObject inView );
	
	void					SetOnlyAmbient( bool inOnlyAmbient );
	
	bool					IsEmissionUsed() const;
	inline bool				IsShadowFrame() const {return mIsShadowFrame;}
	inline bool				IsFirstPass() const {return mIsFirstPass;}
	inline bool				IsLastLightingPass() const {return !mIsAnotherPassNeeded;}
	bool					IsShadowMarkingPass() const;
	bool					IsLit( const TQ3BoundingBox& inBounds ) const;
	bool					IsShadowPhase() const;
	
	void					MarkShadowOfTriMesh(
									TQ3GeometryObject inTMObject,
									const TQ3TriMeshData& inTMData,
									const TQ3Vector3D* inFaceNormals );
	
	void					MarkShadowOfTriangle(
									const Vertex* inVertices );

private:
	void					Reset( bool inEnableLighting );
	void					ClassifyLights( TQ3ViewObject inView );
	void					SetUpShadowMarkingPass( const TQ3Matrix4x4& inWorldToView );
	void					SetUpShadowLightingPass();
	void					SetUpNonShadowLightingPass( const TQ3Matrix4x4& inWorldToView );
	void					UseInfiniteYon( TQ3ViewObject inView );
	
	void					AddLight(
									TQ3LightObject inLight,
									const TQ3Matrix4x4& inWorldToView );

	void					AddDirectionalLight(
									TQ3LightObject inLight,
									const TQ3Matrix4x4& inWorldToView );
	void					AddSpotLight(
									TQ3LightObject inLight,
									const TQ3Matrix4x4& inWorldToView );
	void					AddPointLight(
									TQ3LightObject inLight,
									const TQ3Matrix4x4& inWorldToView );

	const TQ3GLExtensions&	mGLExtensions;
	const GLStencilFuncs&	mGLStencilFuncs;
	const MatrixState&		mMatrixState;
	const StyleState&		mStyleState;
	PerPixelLighting&		mPerPixelLighting;
	TQ3Uns32				mLightCount;		// number of GL lights in this pass
	bool					mIsShadowFrame;
	bool					mIsFirstPass;
	bool					mIsShadowPhase;
	bool					mIsNextPassShadowPhase;
	bool					mIsAnotherPassNeeded;
	bool					mIsShadowMarkingPass;
	TQ3Uns32				mStartingLightIndexForPass;
	GLint					mMaxGLLights;
	GLfloat					mGLLightPosition[4];
	float					mSavedYon;
	
	GLfloat					mGlAmbientLight[4];		// color of ambient light
	ObVec					mNonShadowingLights;
	ObVec					mShadowingLights;
	ShadowMarker			mShadowMarker;
	TQ3AttenuationType		mLightAttenuation;
	float					mLightBrightness;
	TQ3Float32				mAttenuatedLightThreshold;
	TQ3ObjectType			mLightType;
	TQ3Vector3D				mSpotLightDirection;
	float					mSpotAngleCosine;
	
	bool					mIsOnlyAmbient;
	GLfloat					mOnlyAmbient[4];
};

}

#endif	// QOLIGHTS_HDR

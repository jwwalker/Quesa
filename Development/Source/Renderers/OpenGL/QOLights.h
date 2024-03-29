/*!
	@header		QOLights.h
	
	Lights class for use in Quesa OpenGL renderer.
*/

/*  NAME:
       QOLights.h

    DESCRIPTION:
        Header for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2022, Quesa Developers. All rights reserved.

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

struct GLStencilFuncs;
class MatrixState;
struct StyleState;
class Renderer;

/*!
	@class		Lights

	@abstract	Light handling functions for QORenderer.
*/
class Lights
{
public:
							Lights( QORenderer::Renderer& inRenderer );

	void					StartFrame(
									TQ3ViewObject inView,
									bool inIsShadowing );
	
	void					StartPass(
									TQ3ViewObject inView,
									TQ3CameraObject inCamera,
									TQ3RendererObject inRenderer );

	bool					EndPass();
	
	void					EndFrame(
									TQ3ViewObject inView );
	
	void					SetLowDimensionalMode( bool inLowD, TQ3ObjectType inIlluminationType );
	
	bool					IsEmissionUsed() const;
	inline bool				IsShadowFrame() const {return mIsShadowFrame;}
	inline bool				IsFirstPass() const {return mIsFirstPass;}
	inline bool				IsLastLightingPass() const {return !mIsAnotherPassNeeded;}
	bool					IsShadowMarkingPass() const;
	bool					IsLit( const TQ3BoundingBox& inBounds ) const;
	bool					IsShadowPhase() const;
	
	/*!
		@function			GetShadowingLightPosition
		@abstract			During a shadow marking or lighting pass (in which
							exactly one light is active), get the position of 
							the light.
		@discussion			If the light is a point or spot light, we return the
							position as a finite point, with w component 1.  If
							the light is directional, we return an infinite point,
							with w = 0 and x, y, z being the direction vector.
	*/
	TQ3RationalPoint4D		GetShadowingLightPosition() const;
	
	void					MarkShadowOfTriMesh(
									TQ3GeometryObject inTMObject,
									const TQ3TriMeshData& inTMData,
									const TQ3Vector3D* inFaceNormals,
									TQ3ViewObject inView );
	
	void					MarkShadowOfTriangle(
									const Vertex* inVertices );

private:
	void					Reset( bool inEnableLighting );
	void					ClassifyLights( TQ3ViewObject inView );
	void					SetUpShadowMarkingPass( TQ3ViewObject inView,
								const TQ3Matrix4x4& inWorldToView );
	void					SetUpShadowLightingPass( TQ3ViewObject inView );
	void					SetUpNonShadowLightingPass( TQ3ViewObject inView,
								const TQ3Matrix4x4& inWorldToView );
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

	QORenderer::Renderer&	mRenderer;
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
	
	TQ3ColorRGB				mGlAmbientLight;		// color of ambient light
	ObVec					mNonShadowingLights;
	ObVec					mShadowingLights;
	ShadowMarker			mShadowMarker;
	TQ3AttenuationType		mLightAttenuation;
	float					mLightBrightness;
	TQ3Float32				mAttenuatedLightThreshold;
	TQ3ObjectType			mLightType;
	TQ3Vector3D				mSpotLightDirection;
	float					mSpotAngleCosine;
	
	bool					mIsLowDMode;
};

}

#endif	// QOLIGHTS_HDR

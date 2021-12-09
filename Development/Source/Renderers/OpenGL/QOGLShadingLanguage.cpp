/*  NAME:
       QOGLShadingLanguage.cpp

    DESCRIPTION:
        Shading language functions for Quesa OpenGL renderer class.
		    
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
#include "QOGLShadingLanguage.h"
#include "GLUtils.h"
#include "E3Prefix.h"
#include "QuesaRenderer.h"
#include "QOTexture.h"
#include "QOShaderProgramCache.h"
#include "CQ3ObjectRef_Gets.h"
#include "QuesaMathOperators.hpp"
#include "QORenderer.h"
#include "QOGLSLShaders.h"

#include <cstring>
#include <string>
#include <algorithm>
#include <sstream>
#include <cstdlib>

#if WIN32
	#undef max // make it possible to use std::max
#endif

#ifndef GL_VERTEX_PROGRAM_TWO_SIDE
	#define GL_VERTEX_PROGRAM_TWO_SIDE        0x8643
#endif

#ifndef GL_SHADER_SOURCE_LENGTH
	#define	GL_SHADER_SOURCE_LENGTH			0x8B88
#endif

#ifndef GL_SHADING_LANGUAGE_VERSION
	#define GL_SHADING_LANGUAGE_VERSION		0x8B8C
#endif

#ifndef GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB
	#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB               0x8B49
#endif

#ifndef GL_GEOMETRY_SHADER
	#define GL_GEOMETRY_SHADER                0x8DD9
#endif

#ifndef GL_VALIDATE_STATUS
	#define GL_VALIDATE_STATUS					0x8B83
#endif

static int sVertexShaderCount = 0;
static int sProgramCount = 0;

using namespace QOGLSLShader;

//=============================================================================
//      Local constants
//-----------------------------------------------------------------------------

namespace
{
	const TQ3Vector3D kZeroVec = { 0.0f, 0.0f, 0.0f };
	
	const TQ3ColorRGB kBlackColor = { 0.0f, 0.0f, 0.0f };
	const TQ3ColorRGB kWhiteColor = { 1.0f, 1.0f, 1.0f };

	const char*	kTextureUnit0UniformName		= "tex0";
	const char*	kTextureUnit1UniformName		= "tex1";
	const char* kSpecularMapFlagUniformName		= "isUsingSpecularMap";
	const char*	kQuantizationUniformName		= "quantization";
	const char*	kLightNearEdgeUniformName		= "lightNearEdge";
	const char* kSpotHotAngleUniformName		= "hotAngle";
	const char* kSpotCutoffAngleUniformName		= "cutoffAngle";
	const char* kLayerShiftFlagUniformName		= "isLayerShifting";
	const char* kFlippingNormalsFlagUniformName	= "isFlippingNormals";
	const char* kClippingPlaneUniformName		= "ECClipPlane";
	const char*	kMaxFogOpacityUniformName		= "maxFogOpacity";
	const char*	kHSFogRateUniformName			= "HSFogRate";
	const char*	kHSFogPlaneUniformName			= "HSFogPlane";
	const char*	kLightPositionUniformName		= "lightPosition";
	const char*	kLightColorUniformName			= "lightColor";
	const char*	kLightAttenuationUniformName	= "lightAttenuation";
	const char*	kSpotLightDirectionUniformName	= "spotLightDirection";
	const char* kModelViewMtxUniformName		= "quesaModelViewMtx";
	const char* kProjectionMtxUniformName		= "quesaProjectionMtx";
	const char* kTextureMtxUniformName			= "quesaTextureMtx";
	const char* kNormalMtxUniformName			= "quesaNormalMtx";
	const char* kCameraRangeUniformName			= "quesaCameraRange";	// vec2: near and far
	const char* kCameraViewportUniformName		= "quesaCameraViewport"; // vec4: origin.x, origin.y, width, height
	const char* kCameraSensorSizeUniformName	= "quesaSensorSize";	// vec2 (mm)
	const char* kCameraFocalLengthUniformName	= "quesaFocalLength";	// float (mm)
	const char* kCameraAngleOfViewUniformName	= "quesaAngleOfView";	// float (radians)
	const char* kFisheyeMappingFuncUniformName	= "quesaFisheyeMappingFunc";	// int (enumeration)
	const char* kFisheyeCroppingUniformName		= "quesaFisheyeCropping";	// int (enumeration)
	
	GLenum	sGLError = 0;
} // end of unnamed namespace


#pragma mark -
//=============================================================================
//      Implementations
//-----------------------------------------------------------------------------

/*!
	@function	CalcLightColor
	
	@abstract	Combine Quesa light color and brightness to make an OpenGL
				light color.
*/
static TQ3ColorRGBA CalcLightColor( const TQ3LightData& inLightData )
{
	TQ3ColorRGBA theColor;
	
	if (inLightData.isOn)
	{
		theColor.r = (inLightData.color.r * inLightData.brightness);
		theColor.g = (inLightData.color.g * inLightData.brightness);
		theColor.b = (inLightData.color.b * inLightData.brightness);
		theColor.a = 1.0f;
	}
	else
	{
		theColor.r = theColor.g = theColor.b = theColor.a = 0.0f;
	}
	return theColor;
}

static TQ3Vector3D ConvertAttenuation( TQ3AttenuationType inType )
{
	TQ3Vector3D atts = { 0.0f, 0.0f, 0.0f };
	
	switch (inType)
	{
		case kQ3AttenuationTypeNone:
			atts.x = 1.0f;
			break;
			
		case kQ3AttenuationTypeInverseDistance:
			atts.y = 1.0f;
			break;
			
		case kQ3AttenuationTypeInverseDistanceSquared:
			atts.z = 1.0f;
			break;
		
		default:
			break;
	}
	
	return atts;
}


QORenderer::ProgramRec::ProgramRec( const ProgramRec& inOther )
	: mProgram( inOther.mProgram )
	, mCharacteristic( inOther.mCharacteristic )
	, mTextureUnit0UniformLoc( inOther.mTextureUnit0UniformLoc )
	, mTextureUnit1UniformLoc( inOther.mTextureUnit1UniformLoc )
	, mQuantizationUniformLoc( inOther.mQuantizationUniformLoc )
	, mLightNearEdgeUniformLoc( inOther.mLightNearEdgeUniformLoc )
	, mSpotHotAngleUniformLoc( inOther.mSpotHotAngleUniformLoc )
	, mSpotCutoffAngleUniformLoc( inOther.mSpotCutoffAngleUniformLoc )
	, mIsSpecularMappingUniformLoc( inOther.mIsSpecularMappingUniformLoc )
	, mIsLayerShiftingUniformLoc( inOther.mIsLayerShiftingUniformLoc )
	, mIsFlippingNormalsUniformLoc( inOther.mIsFlippingNormalsUniformLoc )
	, mClippingPlaneUniformLoc( inOther.mClippingPlaneUniformLoc )
	, mFogColorUniformLoc( inOther.mFogColorUniformLoc )
	, mFogDensityUniformLoc( inOther.mFogDensityUniformLoc )
	, mLinearFogEndUniformLoc( inOther.mLinearFogEndUniformLoc )
	, mLinearFogScaleUniformLoc( inOther.mLinearFogScaleUniformLoc )
	, mMaxFogOpacityUniformLoc( inOther.mMaxFogOpacityUniformLoc )
	, mHalfspaceFogRateUniformLoc( inOther.mHalfspaceFogRateUniformLoc )
	, mHalfspaceFogPlaneUniformLoc( inOther.mHalfspaceFogPlaneUniformLoc )
	, mLightPositionUniformLoc( inOther.mLightPositionUniformLoc )
	, mLightColorUniformLoc( inOther.mLightColorUniformLoc )
	, mLightAttenuationUniformLoc( inOther.mLightAttenuationUniformLoc )
	, mSpotLightDirectionUniformLoc( inOther.mSpotLightDirectionUniformLoc )
	, mModelViewMtxUniformLoc( inOther.mModelViewMtxUniformLoc )
	, mProjectionMtxUniformLoc( inOther.mProjectionMtxUniformLoc )
	, mTextureMtxUniformLoc( inOther.mTextureMtxUniformLoc )
	, mNormalMtxUniformLoc( inOther.mNormalMtxUniformLoc )
	, mSpecularColorUniformLoc( inOther.mSpecularColorUniformLoc )
	, mShininessUniformLoc( inOther.mShininessUniformLoc )
	, mMetallicUniformLoc( inOther.mMetallicUniformLoc )
	, mEmissiveColorUniformLoc( inOther.mEmissiveColorUniformLoc )
	, mAmbientLightUniformLoc( inOther.mAmbientLightUniformLoc )
	, mAlphaThresholdUniformLoc( inOther.mAlphaThresholdUniformLoc )
	, mLineWidthUniformLoc( inOther.mLineWidthUniformLoc )
	, mViewportSizeUniformLoc( inOther.mViewportSizeUniformLoc )
	, mCullFrontFacesUniformLoc( inOther.mCullFrontFacesUniformLoc )
	, mCullBackFacesUniformLoc( inOther.mCullBackFacesUniformLoc )
	, mCameraRangeUniformLoc( inOther.mCameraRangeUniformLoc )
	, mCameraViewportUniformLoc( inOther.mCameraViewportUniformLoc )
	, mVertexAttribLoc( inOther.mVertexAttribLoc )
	, mNormalAttribLoc( inOther.mNormalAttribLoc )
	, mTexCoordAttribLoc( inOther.mTexCoordAttribLoc )
	, mColorAttribLoc( inOther.mColorAttribLoc )
	, mLayerShiftAttribLoc( inOther.mLayerShiftAttribLoc )
	, mSensorSizeAttribLoc( inOther.mSensorSizeAttribLoc )
	, mFocalLengthAttribLoc( inOther.mFocalLengthAttribLoc )
	, mAngleOfViewAttribLoc( inOther.mAngleOfViewAttribLoc )
	, mFisheyeMappingFuncAttribLoc( inOther.mFisheyeMappingFuncAttribLoc )
	, mFisheyeCroppingAttribLoc( inOther.mFisheyeCroppingAttribLoc )
{}

void	QORenderer::ProgramRec::swap( ProgramRec& ioOther )
{
	std::swap( mProgram, ioOther.mProgram );
	mCharacteristic.swap( ioOther.mCharacteristic );
	std::swap( mTextureUnit0UniformLoc, ioOther.mTextureUnit0UniformLoc );
	std::swap( mTextureUnit1UniformLoc, ioOther.mTextureUnit1UniformLoc );
	std::swap( mQuantizationUniformLoc, ioOther.mQuantizationUniformLoc );
	std::swap( mLightNearEdgeUniformLoc, ioOther.mLightNearEdgeUniformLoc );
	std::swap( mSpotHotAngleUniformLoc, ioOther.mSpotHotAngleUniformLoc );
	std::swap( mSpotCutoffAngleUniformLoc, ioOther.mSpotCutoffAngleUniformLoc );
	std::swap( mIsSpecularMappingUniformLoc, ioOther.mIsSpecularMappingUniformLoc );
	std::swap( mIsLayerShiftingUniformLoc, ioOther.mIsLayerShiftingUniformLoc );
	std::swap( mIsFlippingNormalsUniformLoc, ioOther.mIsFlippingNormalsUniformLoc );
	std::swap( mClippingPlaneUniformLoc, ioOther.mClippingPlaneUniformLoc );
	std::swap( mFogColorUniformLoc, ioOther.mFogColorUniformLoc );
	std::swap( mFogDensityUniformLoc, ioOther.mFogDensityUniformLoc );
	std::swap( mLinearFogEndUniformLoc, ioOther.mLinearFogEndUniformLoc );
	std::swap( mLinearFogScaleUniformLoc, ioOther.mLinearFogScaleUniformLoc );
	std::swap( mMaxFogOpacityUniformLoc, ioOther.mMaxFogOpacityUniformLoc );
	std::swap( mHalfspaceFogRateUniformLoc, ioOther.mHalfspaceFogRateUniformLoc );
	std::swap( mHalfspaceFogPlaneUniformLoc, ioOther.mHalfspaceFogPlaneUniformLoc );
	std::swap( mLightPositionUniformLoc, ioOther.mLightPositionUniformLoc );
	std::swap( mLightColorUniformLoc, ioOther.mLightColorUniformLoc );
	std::swap( mLightAttenuationUniformLoc, ioOther.mLightAttenuationUniformLoc );
	std::swap( mSpotLightDirectionUniformLoc, ioOther.mSpotLightDirectionUniformLoc );
	std::swap( mModelViewMtxUniformLoc, ioOther.mModelViewMtxUniformLoc );
	std::swap( mProjectionMtxUniformLoc, ioOther.mProjectionMtxUniformLoc );
	std::swap( mTextureMtxUniformLoc, ioOther.mTextureMtxUniformLoc );
	std::swap( mNormalMtxUniformLoc, ioOther.mNormalMtxUniformLoc );
	std::swap( mSpecularColorUniformLoc, ioOther.mSpecularColorUniformLoc );
	std::swap( mShininessUniformLoc, ioOther.mShininessUniformLoc );
	std::swap( mMetallicUniformLoc, ioOther.mMetallicUniformLoc );
	std::swap( mEmissiveColorUniformLoc, ioOther.mEmissiveColorUniformLoc );
	std::swap( mAmbientLightUniformLoc, ioOther.mAmbientLightUniformLoc );
	std::swap( mAlphaThresholdUniformLoc, ioOther.mAlphaThresholdUniformLoc );
	std::swap( mLineWidthUniformLoc, ioOther.mLineWidthUniformLoc );
	std::swap( mViewportSizeUniformLoc, ioOther.mViewportSizeUniformLoc );
	std::swap( mCullFrontFacesUniformLoc, ioOther.mCullFrontFacesUniformLoc );
	std::swap( mCullBackFacesUniformLoc, ioOther.mCullBackFacesUniformLoc );
	std::swap( mCameraRangeUniformLoc, ioOther.mCameraRangeUniformLoc );
	std::swap( mCameraViewportUniformLoc, ioOther.mCameraViewportUniformLoc );
	std::swap( mVertexAttribLoc, ioOther.mVertexAttribLoc );
	std::swap( mNormalAttribLoc, ioOther.mNormalAttribLoc );
	std::swap( mTexCoordAttribLoc, ioOther.mTexCoordAttribLoc );
	std::swap( mColorAttribLoc, ioOther.mColorAttribLoc );
	std::swap( mLayerShiftAttribLoc, ioOther.mLayerShiftAttribLoc );
	std::swap( mSensorSizeAttribLoc, ioOther.mSensorSizeAttribLoc );
	std::swap( mFocalLengthAttribLoc, ioOther.mFocalLengthAttribLoc );
	std::swap( mAngleOfViewAttribLoc, ioOther.mAngleOfViewAttribLoc );
	std::swap( mFisheyeMappingFuncAttribLoc, ioOther.mFisheyeMappingFuncAttribLoc );
	std::swap( mFisheyeCroppingAttribLoc, ioOther.mFisheyeCroppingAttribLoc );
}

QORenderer::ProgramRec&
QORenderer::ProgramRec::operator=( const QORenderer::ProgramRec& inOther )
{
	ProgramRec	temp( inOther );
	swap( temp );
	return *this;
}


static void ReplaceAllSubstrByInt( std::string& ioString,
								const char* inFindSub,
								GLint inReplacement )
{
	char	intAsStr[11];
	snprintf( intAsStr, sizeof(intAsStr), "%d", inReplacement );
	std::string::size_type	place;
	std::string::size_type	len = std::strlen( inFindSub );
	
	while ( (place = ioString.find( inFindSub )) != std::string::npos )
	{
		ioString.replace( place, len, intAsStr );
	}
}


QORenderer::GLSLFuncs::GLSLFuncs()
{
	SetNULL();
}

void	QORenderer::GLSLFuncs::SetNULL()
{
	glCreateShader = nullptr;
	glShaderSource = nullptr;
	glCompileShader = nullptr;
	glGetShaderiv = nullptr;
	glCreateProgram = nullptr;
	glAttachShader = nullptr;
	glDetachShader = nullptr;
	glLinkProgram = nullptr;
	glValidateProgram = nullptr;
	glGetProgramiv = nullptr;
	glUseProgram = nullptr;
	glGetUniformLocation = nullptr;
	glUniform1i = nullptr;
	glUniform1f = nullptr;
	glUniform4f = nullptr;
	glUniform1fv = nullptr;
	glUniform2fv = nullptr;
	glUniform3fv = nullptr;
	glUniform4fv = nullptr;
	glUniformMatrix4fv = nullptr;
	glUniformMatrix3fv = nullptr;
	glDeleteShader = nullptr;
	glDeleteProgram = nullptr;
	glGetProgramInfoLog = nullptr;
	glGetShaderInfoLog = nullptr;
	glGetShaderSource = nullptr;
	glIsProgram = nullptr;
	glGetAttribLocation = nullptr;
	glEnableVertexAttribArray = nullptr;
	glDisableVertexAttribArray = nullptr;
	glVertexAttrib3fv = nullptr;
	glVertexAttribPointer = nullptr;
	glBindAttribLocation = nullptr;
}

void	QORenderer::GLSLFuncs::Initialize( const TQ3GLExtensions& inExts )
{
	{
		GLGetProcAddress( glCreateShader, "glCreateShader", "glCreateShaderObjectARB" );
		GLGetProcAddress( glShaderSource, "glShaderSource", "glShaderSourceARB" );
		GLGetProcAddress( glCompileShader, "glCompileShader", "glCompileShaderARB" );
		GLGetProcAddress( glGetShaderiv, "glGetShaderiv", "glGetObjectParameterivARB" );
		GLGetProcAddress( glCreateProgram, "glCreateProgram", "glCreateProgramObjectARB" );
		GLGetProcAddress( glAttachShader, "glAttachShader", "glAttachObjectARB" );
		GLGetProcAddress( glDetachShader, "glDetachShader", "glDetachObjectARB" );
		GLGetProcAddress( glLinkProgram, "glLinkProgram", "glLinkProgramARB" );
		GLGetProcAddress( glValidateProgram, "glValidateProgram", "glValidateProgramARB" );
		GLGetProcAddress( glGetProgramiv, "glGetProgramiv", "glGetObjectParameterivARB" );
		GLGetProcAddress( glUseProgram, "glUseProgram", "glUseProgramObjectARB" );
		GLGetProcAddress( glGetUniformLocation, "glGetUniformLocation", "glGetUniformLocationARB" );
		GLGetProcAddress( glUniform1i, "glUniform1i", "glUniform1iARB" );
		GLGetProcAddress( glUniform1f, "glUniform1f", "glUniform1fARB" );
		GLGetProcAddress( glUniform4f, "glUniform4f", "glUniform4fARB" );
		GLGetProcAddress( glUniform1fv, "glUniform1fv", "glUniform1fvARB" );
		GLGetProcAddress( glUniform2fv, "glUniform2fv", "glUniform2fvARB" );
		GLGetProcAddress( glUniform3fv, "glUniform3fv", "glUniform3fvARB" );
		GLGetProcAddress( glUniform4fv, "glUniform4fv", "glUniform4fvARB" );
		GLGetProcAddress( glUniformMatrix4fv, "glUniformMatrix4fv", "glUniformMatrix4fvARB" );
		GLGetProcAddress( glUniformMatrix3fv, "glUniformMatrix3fv", "glUniformMatrix3fvARB" );
		GLGetProcAddress( glDeleteShader, "glDeleteShader", "glDeleteObjectARB" );
		GLGetProcAddress( glDeleteProgram, "glDeleteProgram", "glDeleteObjectARB" );
		GLGetProcAddress( glGetProgramInfoLog, "glGetProgramInfoLog", "glGetInfoLogARB" );
		GLGetProcAddress( glGetShaderInfoLog, "glGetShaderInfoLog", "glGetInfoLogARB" );
		GLGetProcAddress( glGetShaderSource, "glGetShaderSource", "glGetShaderSourceARB" );
		GLGetProcAddress( glIsProgram, "glIsProgram" );

		GLGetProcAddress( glGetAttribLocation, "glGetAttribLocation" );
		GLGetProcAddress( glEnableVertexAttribArray, "glEnableVertexAttribArray" );
		GLGetProcAddress( glDisableVertexAttribArray, "glDisableVertexAttribArray" );
		GLGetProcAddress( glVertexAttrib3fv, "glVertexAttrib3fv" );
		GLGetProcAddress( glVertexAttribPointer, "glVertexAttribPointer" );
		GLGetProcAddress(glBindAttribLocation, "glBindAttribLocation");

		if ( (glCreateShader == nullptr) ||
			(glShaderSource == nullptr) ||
			(glCompileShader == nullptr) ||
			(glGetShaderiv == nullptr) ||
			(glCreateProgram == nullptr) ||
			(glAttachShader == nullptr) ||
			(glDetachShader == nullptr) ||
			(glLinkProgram == nullptr) ||
			(glGetProgramiv == nullptr) ||
			(glUseProgram == nullptr) ||
			(glGetUniformLocation == nullptr) ||
			(glUniform1i == nullptr) ||
			(glUniform1f == nullptr) ||
			(glUniform4f == nullptr) ||
			(glUniform1fv == nullptr) ||
			(glUniform2fv == nullptr) ||
			(glUniform3fv == nullptr) ||
			(glUniform4fv == nullptr) ||
			(glUniformMatrix4fv == nullptr) ||
			(glUniformMatrix3fv == nullptr) ||
			(glDeleteShader == nullptr) ||
			(glDeleteProgram == nullptr) ||
			(glGetProgramInfoLog == nullptr) ||
			(glGetShaderInfoLog == nullptr) ||
			(glGetAttribLocation == nullptr) ||
			(glEnableVertexAttribArray == nullptr) ||
			(glDisableVertexAttribArray == nullptr) ||
			(glVertexAttrib3fv == nullptr) ||
			(glVertexAttribPointer == nullptr) ||
			(glBindAttribLocation == nullptr)
			)
		{
			Q3_MESSAGE( "Shading functions NOT all present.\n" );
			SetNULL();
		}
	}
}

#pragma mark -

QORenderer::PerPixelLighting::PerPixelLighting(
										Renderer& inRenderer,
										const GLSLFuncs& inFuncs,
										TQ3RendererObject inRendererObject,
										TQ3GLContext& inGLContext,
										const TQ3GLExtensions& inExtensions )
	: mRenderer( inRenderer )
	, mFuncs( inFuncs )
	, mRendererObject( inRendererObject )
	, mGLContext( inGLContext )
	, mMayNeedProgramChange( true )
	, mIsSpecularMapped( false )
	, mIsFlippingNormals( true )
	, mCullBackFaces( false )
	, mCullFrontFaces( false )
	, mAmbientLight( kBlackColor )
	, mQuantization( 0.0f )
	, mLightNearEdge( 1.0f )
	, mFogColor( kWhiteColor )
	, mFogDensity( 1.0f )
	, mMaxFogOpacity( 1.0f )
	, mAlphaThreshold( 0.0f )
	, mCurrentProgram( nullptr )
{
	Q3Matrix4x4_SetIdentity( &mModelViewMtx );
	Q3Matrix4x4_SetIdentity( &mProjectionMtx );
	Q3Matrix4x4_SetIdentity( &mTextureMtx );
}

QORenderer::PerPixelLighting::~PerPixelLighting()
{
	Cleanup();
}


static void AddDirectionalShaderSource(	GLint inLightIndex,
										TQ3ObjectType inIlluminationType,
										std::string& ioSource )
{
	std::string		theSource;
	if (inIlluminationType == kQ3IlluminationTypeNondirectional)
	{
		theSource = kDirectionalLightWithNondirIllumFragmentShaderSource;
	}
	else
	{
		theSource = kDirectionalLightFragmentShaderSource;
	}
	ReplaceAllSubstrByInt( theSource, "LIGHT_INDEX", inLightIndex );
	ioSource += theSource;
}

static void AddPointLightShaderSource(	GLint inLightIndex,
										TQ3ObjectType inIlluminationType,
										std::string& ioSource )
{
	std::string		theSource;
	if (inIlluminationType == kQ3IlluminationTypeNondirectional)
	{
		theSource = kPointLightWithNondirIllumFragmentShaderSource;
	}
	else
	{
		theSource = kPointLightFragmentShaderSource;
	}
	ReplaceAllSubstrByInt( theSource, "LIGHT_INDEX", inLightIndex );
	ioSource += theSource;
}

static void AddSpotLightShaderSource(	GLint inLightIndex,
										TQ3ObjectType inIlluminationType,
										std::string& ioSource )
{
	std::string		theSource;
	if (inIlluminationType == kQ3IlluminationTypeNondirectional)
	{
		theSource = kSpotLightWithNondirIllumFragmentShaderSource;
	}
	else
	{
		theSource = kSpotLightFragmentShaderSource;
	}
	ReplaceAllSubstrByInt( theSource, "LIGHT_INDEX", inLightIndex );
	ioSource +=  theSource;
}

static void AddSpotFalloffFuncSource(	GLint inLightIndex,
										QORenderer::ELightType inLightType,
										std::string& ioSource )
{
	std::string		theSource;
	
	switch (inLightType)
	{
		case QORenderer::kLightTypeSpotNone:
			theSource.assign( kSpotFalloffNoneSource );
			break;
			
		case QORenderer::kLightTypeSpotLinear:
			theSource.assign( kSpotFalloffLinearSource );
			break;
			
		case QORenderer::kLightTypeSpotExponential:
			theSource.assign( kSpotFalloffExponentialSource );
			break;
			
		case QORenderer::kLightTypeSpotCosine:
			theSource.assign( kSpotFalloffCosineSource );
			break;
			
		case QORenderer::kLightTypeSpotCubic:
			theSource.assign( kSpotFalloffSmoothCubicSource );
			break;
		
		default:	// just to quiet a warning, should not happen
			break;
	}
	ReplaceAllSubstrByInt( theSource, "LIGHT_INDEX", inLightIndex );
	ioSource += theSource;
}

#if Q3_DEBUG
static void LogShaderCompileError( GLint inShaderID, const QORenderer::GLSLFuncs& inFuncs )
{
	GLint	logSize = 0;
	inFuncs.glGetShaderiv( inShaderID, GL_INFO_LOG_LENGTH, &logSize );
	if (logSize > 0)
	{
		GLbyte*	theLog = (GLbyte*) Q3Memory_Allocate( logSize );
		if (theLog != nullptr)
		{
			inFuncs.glGetShaderInfoLog( inShaderID, logSize, nullptr, theLog );
			Q3_MESSAGE( (char*)theLog );
			Q3_MESSAGE( "\n" );
			Q3Memory_Free( &theLog );
		}
		else
		{
			Q3_MESSAGE_FMT("Failed to allocate %d bytes for compile error log.",
				(int) logSize );
		}
	}
	else
	{
		Q3_MESSAGE( "Compile error log size was 0.\n" );
	}
	Q3_MESSAGE_FMT("GL_VENDOR: %s", (char*)glGetString(GL_VENDOR));
	Q3_MESSAGE_FMT("GL_RENDERER: %s", (char*)glGetString(GL_RENDERER));
	Q3_MESSAGE_FMT("GL_VERSION: %s", (char*)glGetString(GL_VERSION));
	Q3_MESSAGE_FMT("GL_SHADING_LANGUAGE_VERSION: %s", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
#else
	#define		LogShaderCompileError( x, y )
#endif

static char LightTypeLetter( QORenderer::ELightType inType )
{
	char theChar = ' ';
	char typeLetters[] =
	{
		'x',
		'0',
		'D',
		'P',
		'S',
		'L',
		'E',
		'C',
		'3'
	};
	
	theChar = typeLetters[ inType + 1 ];
	
	return theChar;
}

static void DescribeLights( const QORenderer::ProgramCharacteristic& inCharacteristic,
							std::ostream& ioStream )
{
	ioStream << "L(";
	const unsigned long kLightCount = inCharacteristic.mPattern.size();
	unsigned long i;
	for (i = 0; i < kLightCount; ++i)
	{
		if (i > 0)
		{
			ioStream << ',';
		}
		ioStream << LightTypeLetter( inCharacteristic.mPattern[i] );
	}
	ioStream << ')';
}


static void BuildFragmentShaderSource(	const QORenderer::ProgramCharacteristic& inProgramRec,
										bool inHasGeometryShader,
										std::string& outSource )
{
	const GLint kNumLights = static_cast<GLint>(inProgramRec.mPattern.size());
	GLint i;
	
	outSource += kFragmentShaderPrefix;
	
	if (inHasGeometryShader)
	{
		outSource += kFragmentShaderInputWithGeomShader;
	}
	else
	{
		outSource += kFragmentShaderInputWithoutGeomShader;
	}
	
	GLint shaderLightCount = std::max( kNumLights, 1 );
	// If a uniform array size is 0, the shader won't compile.
	
	ReplaceAllSubstrByInt( outSource, "gl_MaxLights", shaderLightCount );
	
	if (inProgramRec.mIlluminationType != kQ3IlluminationTypeNULL)
	{
		if (inProgramRec.mIsCartoonish)
		{
			outSource += kFragmentShaderQuantizeFuncs_Cartoonish;
		}
		else
		{
			outSource += kFragmentShaderQuantizeFuncs_Normal;
		}
		
		for (i = 0; i < kNumLights; ++i)
		{
			switch (inProgramRec.mPattern[i])
			{
				case QORenderer::kLightTypeSpotNone:
				case QORenderer::kLightTypeSpotLinear:
				case QORenderer::kLightTypeSpotExponential:
				case QORenderer::kLightTypeSpotCosine:
				case QORenderer::kLightTypeSpotCubic:
					AddSpotFalloffFuncSource( i, inProgramRec.mPattern[i],
						outSource );
					break;
				
				default:
					break;
			}
		}
	}
	
	outSource += kMainFragmentShaderStart;

	if (inProgramRec.mIsUsingClippingPlane)
	{
		outSource += kFragmentClipping;
	}
	
	if (inProgramRec.mProjectionType == QORenderer::ECameraProjectionType::fisheye)
	{
		outSource += kFragmentFisheyeCropping;
	}
	
	if (inProgramRec.mInterpolationStyle == kQ3InterpolationStyleNone)
	{
		outSource += kMainFragmentShaderStartFlat;
	}
	else
	{
		outSource += kMainFragmentShaderStartSmooth;
	}
	
	if (inProgramRec.mIlluminationType != kQ3IlluminationTypeNULL)
	{
		for (i = 0; i < kNumLights; ++i)
		{
			switch (inProgramRec.mPattern[i])
			{
				case QORenderer::kLightTypeDirectional:
					AddDirectionalShaderSource( i, inProgramRec.mIlluminationType, outSource );
					break;
					
				case QORenderer::kLightTypePoint:
					AddPointLightShaderSource( i, inProgramRec.mIlluminationType, outSource );
					break;
				
				case QORenderer::kLightTypeSpotNone:
				case QORenderer::kLightTypeSpotLinear:
				case QORenderer::kLightTypeSpotExponential:
				case QORenderer::kLightTypeSpotCosine:
				case QORenderer::kLightTypeSpotCubic:
					AddSpotLightShaderSource( i, inProgramRec.mIlluminationType, outSource );
					break;
				
				default:
					break;
			}
		}
	}
	
	if (inProgramRec.mIlluminationType == kQ3IlluminationTypeNULL)
	{
		outSource += kColorCompForNULLIllumination;
	}
	else if (inProgramRec.mIsCartoonish)
	{
		outSource += kColorCompForLambertAndPhong_Cartoonish;
	}
	else
	{
		outSource += kColorCompForLambertAndPhong;
	}
	
	if (inProgramRec.mIsTextured)
	{
		outSource += kTexturedColorComp;
	}
	
	if (inProgramRec.mIlluminationType == kQ3IlluminationTypePhong)
	{
		outSource += kAddSpecularColor;
	}
	
	if (inProgramRec.mFogModeCombined != QORenderer::kFogModeOff)
	{
		switch (inProgramRec.mFogModeCombined)
		{
			case QORenderer::kFogModeLinear:
				outSource += kCalcFogLinear;
				break;
			
			case QORenderer::kFogModeExponential:
				outSource += kCalcFogExp;
				break;
			
			case QORenderer::kFogModeExponentialSquared:
				outSource += kCalcFogExp2;
				break;
			
			case QORenderer::kFogModeHalfspace:
				outSource += kCalcFogHalfspace;
				break;
			
			default:
				break;
		}
		outSource += kMixFog;
	}
	
	if (inProgramRec.mAngleAffectsAlpha)
	{
		outSource += kAngleAffectOnAlpha;
	}
	
	outSource += kMainFragmentShaderEndSource;
}

/*!
	@function	GetLightTypes
	@abstract	Get the pattern of light types.
*/
void QORenderer::PerPixelLighting::GetLightTypes()
{
	mProgramCharacteristic.mPattern.clear();
	
	if ( mProgramCharacteristic.mIlluminationType != kQ3IlluminationTypeNULL )
	{
		// Query number of lights.
		const int kNumLights = static_cast<int>(mLights.size());
		mProgramCharacteristic.mPattern.reserve( kNumLights );
		
		for (int i = 0; i < kNumLights; ++i)
		{
			QORenderer::ELightType	theType = QORenderer::kLightTypeNone;
			TQ3LightObject theLight = mLights[i].get();
			
			TQ3LightData lightData;
			Q3Light_GetData( theLight, &lightData );
			
			if ( lightData.isOn && (lightData.brightness > kQ3RealZero) )
			{
				switch (Q3Light_GetType( theLight ))
				{
					case kQ3LightTypeDirectional:
						theType = QORenderer::kLightTypeDirectional;
						break;
					
					case kQ3LightTypePoint:
						theType = QORenderer::kLightTypePoint;
						break;

					case kQ3LightTypeSpot:
						{
							TQ3FallOffType fallOff = kQ3FallOffTypeNone;
							Q3SpotLight_GetFallOff( theLight, &fallOff );
							
							switch (fallOff)
							{
								default:
								case kQ3FallOffTypeNone:
									theType = QORenderer::kLightTypeSpotNone;
									break;
								
								case kQ3FallOffTypeLinear:
									theType = QORenderer::kLightTypeSpotLinear;
									break;
								
								case kQ3FallOffTypeExponential:
									theType = QORenderer::kLightTypeSpotExponential;
									break;
								
								case kQ3FallOffTypeCosine:
									theType = QORenderer::kLightTypeSpotCosine;
									break;
								
								case kQ3FallOffTypeSmoothCubic:
									theType = QORenderer::kLightTypeSpotCubic;
									break;
							}
						}
						break;
				}
			}
			mProgramCharacteristic.mPattern.push_back( theType );
		}
	}
	
#if 0//Q3_DEBUG
	std::ostringstream desc;
	DescribeLights( mProgramCharacteristic, desc );
	Q3_MESSAGE_FMT("Light types %s", desc.str().c_str() );
#endif
}


/*!
	@function	StartFrame
	@abstract	Begin a rendering frame.
*/
void	QORenderer::PerPixelLighting::StartFrame( TQ3ViewObject inView )
{
	mPassNumber = 0;
	CHECK_GL_ERROR_MSG("PerPixelLighting::StartFrame 1");
	// Save a reference to the view, and initialize the light group edit index
	mView.Assign( inView );
	CQ3ObjectRef lightGroup( CQ3View_GetLightGroup( inView ) );
	mLightGroupEditIndex = Q3Shared_GetEditIndex( (TQ3Object _Nonnull) lightGroup.get() );

	CheckIfShading();
	CHECK_GL_ERROR_MSG("PerPixelLighting::StartFrame 2");
	
	CalcMaxLights();
	CHECK_GL_ERROR_MSG("PerPixelLighting::StartFrame 3");
	
	CQ3ObjectRef theCamera( CQ3View_GetCamera( inView ) );
	switch (Q3Camera_GetType( (TQ3Object _Nonnull) theCamera.get() ))
	{
		case kQ3CameraTypeAllSeeing:
			mProgramCharacteristic.mProjectionType = ECameraProjectionType::allSeeingEquirectangular;
			break;
		
		case kQ3CameraTypeFisheye:
			mProgramCharacteristic.mProjectionType = ECameraProjectionType::fisheye;
			break;
		
		default:
			mProgramCharacteristic.mProjectionType = ECameraProjectionType::standardRectilinear;
			break;
	}
	
	InitVertexShader();
}

void	QORenderer::PerPixelLighting::CalcMaxLights()
{
	// Find the total number of uniform scalars there can be
	GLint maxUniformScalars = 1024;
	glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB, &maxUniformScalars );
	
	// Subtract some for the non-light uniforms.  Right now there are 15, but leave
	// some slack.
	GLint scalarsAvailableForLights = maxUniformScalars - 50;
	
	// For each light, we need 20 or fewer values.
	mMaxLights = scalarsAvailableForLights / 20;
}

/*!
	@function	ClearLights
	@abstract	Forget lights that were previously passed to AddLight.
	@discussion	This will be called by the Lights object when it is starting
				a pass, which happens before the PerPixelLighting gets a
				StartPass call.
*/
void	QORenderer::PerPixelLighting::ClearLights()
{
	mLights.clear();
	
	mLightTypes.clear();
	mLightPositions.clear();
	mLightColors.clear();
	mSpotLightDirections.clear();
	mSpotLightHotAngles.clear();
	mSpotLightCutoffAngles.clear();
	mLightAttenuations.clear();
}

void	QORenderer::PerPixelLighting::AddDirectionalLight( TQ3LightObject inLight )
{
	TQ3DirectionalLightData		lightData;
	Q3DirectionalLight_GetData( inLight, &lightData );
	
	// Transform the direction to view coordinates
	lightData.direction = Q3Normalize3D( lightData.direction * mWorldToView );
	
	// Represent the direction as an infinite rational point
	TQ3RationalPoint4D position = {
		- lightData.direction.x,
		- lightData.direction.y,
		- lightData.direction.z,
		0.0f
	};
	mLightPositions.push_back( position );
	
	mSpotLightDirections.push_back( kZeroVec );
	mSpotLightHotAngles.push_back( 0.0f );
	mSpotLightCutoffAngles.push_back( 0.0f );
	
	mLightAttenuations.push_back( ConvertAttenuation( kQ3AttenuationTypeNone ) );
}

void	QORenderer::PerPixelLighting::AddSpotLight( TQ3LightObject inLight )
{
	// Get light data
	TQ3SpotLightData	lightData;
	Q3SpotLight_GetData( inLight, &lightData );
	
	// Transform the location to view coordinates
	lightData.location *= mWorldToView;
	// Represent location as a finite rational point
	mLightPositions.push_back( Q3ToRational4D( lightData.location ) );
	
	// Transform the direction to view coordinates
	lightData.direction = Q3Normalize3D( lightData.direction * mWorldToView );
	mSpotLightDirections.push_back( lightData.direction );
	
	// Save hot and cutoff angles
	mSpotLightHotAngles.push_back( lightData.hotAngle );
	mSpotLightCutoffAngles.push_back( lightData.outerAngle );

	mLightAttenuations.push_back( ConvertAttenuation( lightData.attenuation ) );
}

void	QORenderer::PerPixelLighting::AddPointLight( TQ3LightObject inLight )
{
	// Get light data
	TQ3PointLightData	lightData;
	Q3PointLight_GetData( inLight, &lightData );
	
	// Transform the location to view coordinates
	lightData.location *= mWorldToView;
	// Represent location as a finite rational point
	mLightPositions.push_back( Q3ToRational4D( lightData.location ) );

	mSpotLightDirections.push_back( kZeroVec );
	mSpotLightHotAngles.push_back( 0.0f );
	mSpotLightCutoffAngles.push_back( 0.0f );

	mLightAttenuations.push_back( ConvertAttenuation( lightData.attenuation ) );
}

/*!
	@function	AddLight
	@abstract	The Lights object uses this to inform the PerPixelLighting
				object about a light being used for this pass.
*/
void	QORenderer::PerPixelLighting::AddLight( TQ3LightObject inLight )
{
	CQ3ObjectRef	lightRef( Q3Shared_GetReference( inLight ) );
#if QUESA_HAS_STDMOVE
	mLights.push_back( std::move( lightRef ) );
#else
	mLights.push_back( lightRef );
#endif
	
	TQ3ObjectType lightType = Q3Light_GetType( inLight );
	mLightTypes.push_back( lightType );
	
	TQ3LightData basicLightData;
	Q3Light_GetData( inLight, &basicLightData );
	TQ3ColorRGBA lightColor( CalcLightColor( basicLightData ) );
	mLightColors.push_back( lightColor );
	
	switch (lightType)
	{
		case kQ3LightTypeDirectional:
			AddDirectionalLight( inLight );
			break;
		
		case kQ3LightTypePoint:
			AddPointLight( inLight );
			break;
		
		case kQ3LightTypeSpot:
			AddSpotLight( inLight );
			break;
	}
}


QORenderer::ProgramCache*	QORenderer::PerPixelLighting::ProgCache()
{
	QORenderer::ProgramCache* cache =
		ProgramCache::GetProgramCache( mGLContext );
	return cache;
}


/*!
	@function	StartPass
	@abstract	Begin a rendering pass.
	@discussion	If per-pixel lighting is desired, we set up the shaders and program.
*/
void	QORenderer::PerPixelLighting::StartPass( TQ3CameraObject inCamera )
{
	++mPassNumber;
	mProgramCharacteristic.mIlluminationType = kQ3IlluminationTypeNULL;
	mProgramCharacteristic.mIsTextured = false;
	mCurrentProgram = nullptr;
	mMayNeedProgramChange = true;
	mProgramCharacteristic.mIsCartoonish = (mQuantization > 0.0f);
	mIsSpecularMapped = false;
	mCullBackFaces = mCullFrontFaces = false;
	
	mProgramCharacteristic.mIsUsingClippingPlane = false;
	mClippingPlane.x = 999999.0f;
	
	mProgramCharacteristic.mFogModeCombined = kFogModeOff;
	mMaxFogOpacity = 1.0f;

	TQ3Boolean angleAffectsAlpha = kQ3True;
	Q3Object_GetProperty( mRendererObject, kQ3RendererPropertyAngleAffectsAlpha,
		sizeof(TQ3Boolean), nullptr, &angleAffectsAlpha );
	mProgramCharacteristic.mAngleAffectsAlpha = (angleAffectsAlpha == kQ3True);
	
	mAlphaThreshold = 0.0f;
	
	Q3Camera_GetWorldToView( inCamera, &mWorldToView );
	Q3Matrix4x4_SetIdentity( &mModelViewMtx );
	Q3Matrix4x4_SetIdentity( &mProjectionMtx );
	Q3Matrix4x4_SetIdentity( &mTextureMtx );
	Q3Matrix3x3_SetIdentity( &mNormalMtx );
	
	if (ProgCache()->VertexShaderID( mProgramCharacteristic.mProjectionType ) != 0)
	{
		GetLightTypes();
	}
}

static std::string DescribeProgram( const QORenderer::ProgramRec& inProgram )
{
	std::ostringstream desc;
	DescribeLights( inProgram.mCharacteristic, desc );
	desc << (inProgram.mCharacteristic.mIsTextured? "T+" : "T-");
	switch (inProgram.mCharacteristic.mIlluminationType)
	{
	case kQ3IlluminationTypePhong:
		desc << "I=P";
		break;
	case kQ3IlluminationTypeLambert:
		desc << "I=L";
		break;
	case kQ3IlluminationTypeNULL:
		desc << "I=0";
		break;
	case kQ3IlluminationTypeNondirectional:
		desc << "I=n";
		break;
	}
	return desc.str();
}


/*!
	@function	ChooseProgram
	
	@abstract	Look for a program that matches the current light pattern,
				illumination, and texturing, creating one if need be, and
				activate it.
*/
void	QORenderer::PerPixelLighting::ChooseProgram()
{
	if ( mMayNeedProgramChange &&
		(ProgCache()->VertexShaderID( mProgramCharacteristic.mProjectionType ) != 0) )
	{
		mMayNeedProgramChange = false;
		/*Q3_MESSAGE_FMT("Shader illumination type: %c%c%c%c",
			(char)(mProgramCharacteristic.mIlluminationType >> 24),
			(char)(mProgramCharacteristic.mIlluminationType >> 16),
			(char)(mProgramCharacteristic.mIlluminationType >> 8),
			(char)(mProgramCharacteristic.mIlluminationType >> 0) );*/

		// Look for a program that meets current needs.
		const ProgramRec* theProgram = ProgCache()->FindProgram( mProgramCharacteristic );
		
		// If there is none, create it.
		if (theProgram == nullptr)
		{
			InitProgram();
			
			theProgram = ProgCache()->FindProgram( mProgramCharacteristic );
		}
		
		// Activate it.
		if (theProgram != nullptr)
		{
			if (theProgram != mCurrentProgram)
			{
				mCurrentProgram = theProgram;
				(void)glGetError();	// discard any previous unknown error
				mFuncs.glUseProgram( theProgram->mProgram );
				CHECK_GL_ERROR_MSG("glUseProgram");
			#if Q3_DEBUG
				std::string desc( DescribeProgram( *theProgram ) );
				Q3_MESSAGE_FMT("Using program ID %u, of type %s",
					(unsigned int)theProgram->mProgram, desc.c_str() );
				if ( (sGLError != GL_NO_ERROR) && (mFuncs.glIsProgram != nullptr) )
				{
					GLboolean isProgram = (*mFuncs.glIsProgram)( theProgram->mProgram );
					Q3_MESSAGE_FMT("Program name %u is%s known to be a program",
						(unsigned int)theProgram->mProgram,
						isProgram? "" : " not" );
				}
			#endif
			
				mRenderer.GetClientStates().StartProgram();
				SetUniformValues();
				mRenderer.RefreshMaterials();
			}
			
			// Even if we didn't change the program, we need to update some
			// uniforms.
			mFuncs.glUniform1i( theProgram->mIsSpecularMappingUniformLoc, mIsSpecularMapped );
			mFuncs.glUniform1i( theProgram->mIsFlippingNormalsUniformLoc, mIsFlippingNormals );
			if (mProgramCharacteristic.mFogModeCombined != kFogModeOff)
			{
				mFuncs.glUniform1f( theProgram->mMaxFogOpacityUniformLoc, mMaxFogOpacity );
				mFuncs.glUniform3fv( theProgram->mFogColorUniformLoc, 1, &mFogColor.r );
				mFuncs.glUniform1f( theProgram->mFogDensityUniformLoc, mFogDensity );
				if (mProgramCharacteristic.mFogModeCombined == kFogModeHalfspace)
				{
					mFuncs.glUniform1f( theProgram->mHalfspaceFogRateUniformLoc,
						mHalfspaceFogRate );
					mFuncs.glUniform4f( theProgram->mHalfspaceFogPlaneUniformLoc,
						mHalfspaceFogPlane.x, mHalfspaceFogPlane.y,
						mHalfspaceFogPlane.z, mHalfspaceFogPlane.w );
				}
				else if (mProgramCharacteristic.mFogModeCombined == kFogModeLinear)
				{
					mFuncs.glUniform1f( theProgram->mLinearFogEndUniformLoc, mLinearFogEnd );
					mFuncs.glUniform1f( theProgram->mLinearFogScaleUniformLoc, mLinearFogScale );
				}
			}
			
			// Alpha threshold.
			mFuncs.glUniform1f( theProgram->mAlphaThresholdUniformLoc, mAlphaThreshold );
			
			// Line width
			if (theProgram->mLineWidthUniformLoc != -1)
			{
				//Q3_MESSAGE_FMT("Setting line width uniform to %f", mRenderer.LineWidth() );
				mFuncs.glUniform1f( theProgram->mLineWidthUniformLoc, mRenderer.LineWidth() );
			}
			
			// Culling, only needed with 2D geometries in line-fill mode
			if (theProgram->mCullBackFacesUniformLoc != -1)
			{
				GLint cullBack =  (GLint)(mCullBackFaces? 1 : 0);
				GLint cullFront = (GLint)(mCullFrontFaces? 1 : 0);
				mFuncs.glUniform1i( theProgram->mCullFrontFacesUniformLoc, cullFront );
				mFuncs.glUniform1i( theProgram->mCullBackFacesUniformLoc, cullBack );
				//Q3_MESSAGE_FMT("Culling back %d, front %d", cullBack, cullFront );
			}
		}
	}
}

/*!
	@function	SetUniformValues
	@abstract	Set values for the uniform variables needed in a newly activated program.
*/
void	QORenderer::PerPixelLighting::SetUniformValues()
{
	// Set texture units.
	mFuncs.glUniform1i( mCurrentProgram->mTextureUnit0UniformLoc, 0 );
	mFuncs.glUniform1i( mCurrentProgram->mTextureUnit1UniformLoc, 1 );

	// Set the quantization uniform variables.
	if (mCurrentProgram->mQuantizationUniformLoc != -1)
	{
		mFuncs.glUniform1f( mCurrentProgram->mQuantizationUniformLoc, mQuantization );
		CHECK_GL_ERROR_MSG("glUniform1f mQuantization");
		mFuncs.glUniform1f( mCurrentProgram->mLightNearEdgeUniformLoc, mLightNearEdge );
		CHECK_GL_ERROR_MSG("glUniform1f mLightNearEdge");
	}
	
	// Set lighting uniform arrays.
	const int kNumLights = static_cast<int>(mLights.size());
	if (kNumLights > 0)
	{
		mFuncs.glUniform1fv( mCurrentProgram->mSpotHotAngleUniformLoc, kNumLights,
			&mSpotLightHotAngles[0] );
		mFuncs.glUniform1fv( mCurrentProgram->mSpotCutoffAngleUniformLoc, kNumLights,
			&mSpotLightCutoffAngles[0] );
		
		mFuncs.glUniform4fv( mCurrentProgram->mLightPositionUniformLoc, kNumLights,
			&mLightPositions[0].x );
		mFuncs.glUniform4fv( mCurrentProgram->mLightColorUniformLoc, kNumLights,
			&mLightColors[0].r );
		mFuncs.glUniform3fv( mCurrentProgram->mSpotLightDirectionUniformLoc, kNumLights,
			&mSpotLightDirections[0].x );
		mFuncs.glUniform3fv( mCurrentProgram->mLightAttenuationUniformLoc, kNumLights,
			&mLightAttenuations[0].x );
	}
	
	// Ambient light.
	mFuncs.glUniform3fv( mCurrentProgram->mAmbientLightUniformLoc, 1, &mAmbientLight.r );
	
	// Set layer shifting flag.
	TQ3Boolean isShifting = kQ3True;
	Q3Object_GetProperty( mRendererObject, kQ3RendererPropertyIsLayerShifting,
		sizeof(TQ3Boolean), nullptr, &isShifting );
	mFuncs.glUniform1i( mCurrentProgram->mIsLayerShiftingUniformLoc, isShifting );
	
	// Set extra fog parameters.
	mFuncs.glUniform1f( mCurrentProgram->mMaxFogOpacityUniformLoc, mMaxFogOpacity );
	
	// Set transform matrices
	mFuncs.glUniformMatrix4fv( mCurrentProgram->mModelViewMtxUniformLoc, 1, GL_FALSE, &mModelViewMtx.value[0][0] );
	SetCameraUniforms();
	mFuncs.glUniformMatrix4fv( mCurrentProgram->mTextureMtxUniformLoc, 1, GL_FALSE, &mTextureMtx.value[0][0] );
	mFuncs.glUniformMatrix3fv( mCurrentProgram->mNormalMtxUniformLoc, 1, GL_FALSE, &mNormalMtx.value[0][0] );
	
	// Initialize specular and emissive color.
	mFuncs.glUniform1f( mCurrentProgram->mShininessUniformLoc, 0.0f );
	mFuncs.glUniform3fv( mCurrentProgram->mSpecularColorUniformLoc, 1, &kZeroVec.x );
	mFuncs.glUniform3fv( mCurrentProgram->mEmissiveColorUniformLoc, 1, &kZeroVec.x );
	mFuncs.glUniform1f( mCurrentProgram->mMetallicUniformLoc, 0.0f );
	
	if ( mCurrentProgram->mViewportSizeUniformLoc != -1 )
	{
		GLfloat port[4];
		glGetFloatv( GL_VIEWPORT, port );
		mFuncs.glUniform2fv( mCurrentProgram->mViewportSizeUniformLoc, 1, &port[2] );
		//Q3_MESSAGE_FMT("Viewport size set to %f %f", port[2], port[3] );
	}
}

/*!
	@function	EndPass
	@abstract	Finish a rendering pass.
*/
void	QORenderer::PerPixelLighting::EndPass()
{
	mFuncs.glUseProgram( 0 );
	mCurrentProgram = nullptr;
}


void	QORenderer::PerPixelLighting::InitUniformLocations( ProgramRec& ioProgram )
{
	ioProgram.mTextureUnit0UniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kTextureUnit0UniformName );
	CHECK_GL_ERROR;
	ioProgram.mTextureUnit1UniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kTextureUnit1UniformName );
	CHECK_GL_ERROR;
	ioProgram.mIsSpecularMappingUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kSpecularMapFlagUniformName );
	CHECK_GL_ERROR;
	ioProgram.mIsLayerShiftingUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kLayerShiftFlagUniformName );
	CHECK_GL_ERROR;
	ioProgram.mQuantizationUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kQuantizationUniformName );
	CHECK_GL_ERROR;
	ioProgram.mLightNearEdgeUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kLightNearEdgeUniformName );
	CHECK_GL_ERROR;
	ioProgram.mSpotHotAngleUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kSpotHotAngleUniformName );
	CHECK_GL_ERROR;
	ioProgram.mSpotCutoffAngleUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kSpotCutoffAngleUniformName );
	CHECK_GL_ERROR;
	ioProgram.mIsFlippingNormalsUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kFlippingNormalsFlagUniformName );
	CHECK_GL_ERROR;
	ioProgram.mClippingPlaneUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kClippingPlaneUniformName );
	CHECK_GL_ERROR;
	ioProgram.mFogColorUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "fogColor" );
	CHECK_GL_ERROR;
	ioProgram.mFogDensityUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "fogDensity" );
	CHECK_GL_ERROR;
	ioProgram.mLinearFogEndUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "linearFogEnd" );
	CHECK_GL_ERROR;
	ioProgram.mLinearFogScaleUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "linearFogScale" );
	CHECK_GL_ERROR;
	ioProgram.mMaxFogOpacityUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kMaxFogOpacityUniformName );
	CHECK_GL_ERROR;
	ioProgram.mHalfspaceFogRateUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kHSFogRateUniformName );
	CHECK_GL_ERROR;
	ioProgram.mHalfspaceFogPlaneUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kHSFogPlaneUniformName );
	CHECK_GL_ERROR;
	ioProgram.mLightPositionUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kLightPositionUniformName );
	CHECK_GL_ERROR;
	ioProgram.mLightColorUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kLightColorUniformName );
	CHECK_GL_ERROR;
	ioProgram.mLightAttenuationUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kLightAttenuationUniformName );
	CHECK_GL_ERROR;
	ioProgram.mSpotLightDirectionUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kSpotLightDirectionUniformName );
	CHECK_GL_ERROR;
	ioProgram.mAmbientLightUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "ambientLight" );
	CHECK_GL_ERROR;
	ioProgram.mAlphaThresholdUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "alphaThreshold" );
	CHECK_GL_ERROR;
	ioProgram.mLineWidthUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "lineWidth" );
	CHECK_GL_ERROR;
	ioProgram.mViewportSizeUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "viewportSize" );
	CHECK_GL_ERROR;
	ioProgram.mCullFrontFacesUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "cullFrontFaces" );
	CHECK_GL_ERROR;
	ioProgram.mCullBackFacesUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "cullBackFaces" );
	CHECK_GL_ERROR;
	ioProgram.mCameraRangeUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kCameraRangeUniformName );
	ioProgram.mCameraViewportUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kCameraViewportUniformName );
	
	ioProgram.mModelViewMtxUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kModelViewMtxUniformName );
	CHECK_GL_ERROR;
	ioProgram.mProjectionMtxUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kProjectionMtxUniformName );
	CHECK_GL_ERROR;
	ioProgram.mTextureMtxUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kTextureMtxUniformName );
	CHECK_GL_ERROR;
	ioProgram.mNormalMtxUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kNormalMtxUniformName );
	CHECK_GL_ERROR;
	
	ioProgram.mSpecularColorUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "specularColor" );
	CHECK_GL_ERROR;
	ioProgram.mShininessUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "shininess" );
	CHECK_GL_ERROR;
	ioProgram.mMetallicUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "metallic" );
	CHECK_GL_ERROR;
	ioProgram.mEmissiveColorUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, "quesaEmissiveColor" );
	
	ioProgram.mVertexAttribLoc = mFuncs.glGetAttribLocation( ioProgram.mProgram, "quesaVertex" );
	CHECK_GL_ERROR;
	ioProgram.mNormalAttribLoc = mFuncs.glGetAttribLocation( ioProgram.mProgram, "quesaNormal" );
	CHECK_GL_ERROR;
	ioProgram.mTexCoordAttribLoc = mFuncs.glGetAttribLocation( ioProgram.mProgram, "quesaTexCoord0" );
	CHECK_GL_ERROR;
	ioProgram.mColorAttribLoc = mFuncs.glGetAttribLocation( ioProgram.mProgram, "quesaColor" );
	CHECK_GL_ERROR;
	ioProgram.mLayerShiftAttribLoc = mFuncs.glGetAttribLocation( ioProgram.mProgram, "quesaLayerShift" );
	CHECK_GL_ERROR;

	ioProgram.mSensorSizeAttribLoc = mFuncs.glGetUniformLocation( ioProgram.mProgram, kCameraSensorSizeUniformName );
	CHECK_GL_ERROR;
	ioProgram.mFocalLengthAttribLoc = mFuncs.glGetUniformLocation( ioProgram.mProgram, kCameraFocalLengthUniformName );
	CHECK_GL_ERROR;
	ioProgram.mAngleOfViewAttribLoc = mFuncs.glGetUniformLocation( ioProgram.mProgram, kCameraAngleOfViewUniformName );
	CHECK_GL_ERROR;
	ioProgram.mFisheyeMappingFuncAttribLoc = mFuncs.glGetUniformLocation( ioProgram.mProgram, kFisheyeMappingFuncUniformName );
	CHECK_GL_ERROR;
	ioProgram.mFisheyeCroppingAttribLoc = mFuncs.glGetUniformLocation( ioProgram.mProgram, kFisheyeCroppingUniformName );
	CHECK_GL_ERROR;
}

/*!
	@function	CheckIfShading
	@abstract	Check whether we are doing per-pixel lighting on this frame.
*/
void	QORenderer::PerPixelLighting::CheckIfShading()
{
	mQuantization = 0.0f;	// default of no quantization
	Q3Object_GetProperty( mRendererObject,
		kQ3RendererPropertyQuantizePerPixelLight, sizeof(mQuantization), nullptr,
		&mQuantization );
	
	mLightNearEdge = 1.0f;	// default, no darkening of edges
	Q3Object_GetProperty( mRendererObject,
		kQ3RendererPropertyCartoonLightNearEdge, sizeof(TQ3Float32), nullptr,
		&mLightNearEdge );
}


/*!
	@function	CreateAndCompileShader
	@abstract	Create a shader, load source code into it, and compile.
	@param		inShaderType		Kind of shader (GL_VERTEX_SHADER,
									GL_FRAGMENT_SHADER)
	@param		inSource			NUL-terminated string.
	@param		inFuncs				OpenGL function pointers.
	@result		A shader ID, or 0 on failure.
*/
static GLuint CreateAndCompileShader( GLenum inShaderType,
									const char* inSource,
									const QORenderer::GLSLFuncs& inFuncs )
{
	GLuint shaderID = inFuncs.glCreateShader( inShaderType );
	CHECK_GL_ERROR_FMT( "glCreateShader(%X)", (unsigned int)inShaderType );
	if (shaderID != 0)
	{
		// Supply source code
		GLint sourceLen = (GLint) std::strlen( inSource );
		inFuncs.glShaderSource( shaderID, 1, &inSource, &sourceLen );
		CHECK_GL_ERROR_MSG("glShaderSource");
		
		// Compile the shader
		inFuncs.glCompileShader( shaderID );
		CHECK_GL_ERROR_MSG("glCompileShader");
		
		// Check for compile success
		GLint	status;
		inFuncs.glGetShaderiv( shaderID, GL_COMPILE_STATUS, &status );
		
		//Q3_MESSAGE_FMT("=== Shader of type %X ===", (int)inShaderType);
		//E3LogMessage(inSource);
		
		if (status == GL_FALSE)
		{
			Q3_MESSAGE( "Failed to compile GLSL shader.\n" );
			LogShaderCompileError( shaderID, inFuncs );
			
			// Log out the source code.
			GLint sourceLength;
			inFuncs.glGetShaderiv( shaderID, GL_SHADER_SOURCE_LENGTH, &sourceLength );
			if (sourceLength <= 0)
			{
				Q3_MESSAGE( "No source read back from shader.\n" );
			}
			else
			{
				Q3_MESSAGE( "Source of shader:\n" );
				char*	theLog = (char*) Q3Memory_Allocate( sourceLength );
				inFuncs.glGetShaderSource( shaderID, sourceLength, nullptr, theLog );
				E3LogMessage( (const char*) theLog );
				Q3Memory_Free( &theLog );
			}
			
			// Try again.
			(void) glGetError();
			inFuncs.glDeleteShader( shaderID );
			shaderID = inFuncs.glCreateShader( inShaderType );
			
			if (shaderID != 0)
			{
				inFuncs.glShaderSource( shaderID, 1, &inSource, &sourceLen );
				GLenum err = glGetError();
				if (err != GL_NO_ERROR)
				{
					Q3_MESSAGE_FMT("glShaderSource returned error %d", (int)err);
				}
				
				inFuncs.glCompileShader( shaderID );
				err = glGetError();
				if (err != GL_NO_ERROR)
				{
					Q3_MESSAGE_FMT("glCompileShader returned error %d", (int)err);
				}
				
				inFuncs.glGetShaderiv( shaderID, GL_COMPILE_STATUS, &status );
				if (status == GL_FALSE)
				{
					Q3_MESSAGE( "Second try failed again.\n" );
					inFuncs.glDeleteShader( shaderID );
					shaderID = 0;
					
					if (inShaderType == GL_VERTEX_SHADER)
					{
						E3ErrorManager_PostWarning( kQ3WarningVertexShaderCompileFailed );
#if Q3_DEBUG
						std::abort();
#endif
					}
					else
					{
						E3ErrorManager_PostWarning( kQ3WarningFragmentShaderCompileFailed );
					}
				}
				else
				{
					Q3_MESSAGE( "Second try succeeded!!\n" );
				}
			}
		}
	}
	else
	{
		Q3_MESSAGE( "Failed to create a shader.\n" );
	}
	return shaderID;
}

/*!
	@function	InitVertexShader
	@abstract	Set up the vertex shader, if it has not already
				been done.
*/
void	QORenderer::PerPixelLighting::InitVertexShader()
{
	if (ProgCache()->VertexShaderID( mProgramCharacteristic.mProjectionType ) == 0)
	{
		std::string shaderSource( kVertexShaderStart );
		switch (mProgramCharacteristic.mProjectionType)
		{
			case ECameraProjectionType::standardRectilinear:
				shaderSource += kVertexShaderStandardProjection;
				break;
			
			case ECameraProjectionType::allSeeingEquirectangular:
				shaderSource += kVertexShaderAllSeeingProjection;
				break;

			case ECameraProjectionType::fisheye:
				shaderSource += kVertexShaderFisheyeProjection;
				break;
		}
		shaderSource += kVertexShaderEnd;
		
		GLuint vertexShader = CreateAndCompileShader( GL_VERTEX_SHADER,
			shaderSource.c_str(), mFuncs );
		
		if (vertexShader == 0)
		{
			Q3_MESSAGE( "Failed to create a vertex shader.\n" );
		}
		else
		{
			++sVertexShaderCount;
			//Q3_MESSAGE_FMT("Created vertex shader number %d, ID %d",
			//	sVertexShaderCount, (int)vertexShader );
			ProgCache()->SetVertexShaderID( mProgramCharacteristic.mProjectionType, vertexShader );
		}
	}
}

/*!
	@function	InitProgram
	@abstract	Set up the main fragment shader and program.
*/
void	QORenderer::PerPixelLighting::InitProgram()
{
	CHECK_GL_ERROR_MSG("InitProgram start");
	ProgramRec	newProgram;
	
	newProgram.mCharacteristic = mProgramCharacteristic;
	
	// If processing lines, build the geometry shader
	GLint geomShaderID = 0;
	if ( (mProgramCharacteristic.mDimension == 1) &&
		(mProgramCharacteristic.mFillStyle != kQ3FillStylePoints) )
	{
		Q3_MESSAGE_FMT("Using fat line geometry shader");
		geomShaderID = CreateAndCompileShader( GL_GEOMETRY_SHADER,
			kLineGeomShader, mFuncs );
	}
	else if ( (mProgramCharacteristic.mDimension == 2) &&
		(mProgramCharacteristic.mFillStyle == kQ3FillStyleEdges) )
	{
		Q3_MESSAGE_FMT("Using triangle edge geometry shader");
		geomShaderID = CreateAndCompileShader( GL_GEOMETRY_SHADER,
			kFaceEdgeGeomShader, mFuncs );
	}
	else if ( (mProgramCharacteristic.mDimension == 2) &&
		(mProgramCharacteristic.mFillStyle == kQ3FillStyleFilled) &&
		(mProgramCharacteristic.mProjectionType == ECameraProjectionType::allSeeingEquirectangular) )
	{
		Q3_MESSAGE_FMT("Using all-seeing geometry shader");
		geomShaderID = CreateAndCompileShader( GL_GEOMETRY_SHADER,
			kAllSeeingGeomShader, mFuncs );
	}
	else if ( (mProgramCharacteristic.mDimension == 2) &&
		(mProgramCharacteristic.mFillStyle == kQ3FillStyleFilled) &&
		(mProgramCharacteristic.mProjectionType == ECameraProjectionType::fisheye) )
	{
		Q3_MESSAGE_FMT("Using fisheye geometry shader");
		geomShaderID = CreateAndCompileShader( GL_GEOMETRY_SHADER,
			kFisheyeGeomShader, mFuncs );
	}
	else
	{
		Q3_MESSAGE_FMT("No geometry shader");
	}

	// Build the source of the fragment shader
	std::string	fragSource;
	BuildFragmentShaderSource( newProgram.mCharacteristic, geomShaderID != 0, fragSource );
	
	// Create the fragment shader
	GLint fragShaderID = CreateAndCompileShader( GL_FRAGMENT_SHADER,
		fragSource.c_str(), mFuncs );
	CHECK_GL_ERROR_MSG("InitProgram after CreateAndCompileShader");

	if (fragShaderID != 0)
	{
		// Create a program.
		newProgram.mProgram = mFuncs.glCreateProgram();
		CHECK_GL_ERROR_MSG("glCreateProgram");
	
		if (newProgram.mProgram != 0)
		{
			GLuint vertexShader = ProgCache()->VertexShaderID( newProgram.mCharacteristic.mProjectionType );
		
			++sProgramCount;
			std::string desc( DescribeProgram( newProgram ) );
			//Q3_MESSAGE_FMT("Created program number %d, ID %u, of type %s",
			//	sProgramCount, (unsigned int)newProgram.mProgram, desc.c_str() );

			// Attach the vertex shader to the program.
			mFuncs.glAttachShader( newProgram.mProgram, vertexShader );
			CHECK_GL_ERROR;
		
			// Attach the fragment shader to the program
			mFuncs.glAttachShader( newProgram.mProgram, fragShaderID );
			CHECK_GL_ERROR;
			
			// Attach the geometry shader, if any
			if (geomShaderID != 0)
			{
				mFuncs.glAttachShader( newProgram.mProgram, geomShaderID );
			}

			// Bind generic attribute 0 to vertex positions.
			// Some buggy drivers render nothing if attribute 0 happens to be
			// a disabled array.
			mFuncs.glBindAttribLocation(newProgram.mProgram, 0, "quesaVertex");
			
			// Link program
			mFuncs.glLinkProgram( newProgram.mProgram );
			CHECK_GL_ERROR;
		
			// Detach shaders from program (whether or not link worked)
			mFuncs.glDetachShader( newProgram.mProgram, fragShaderID );
			mFuncs.glDetachShader( newProgram.mProgram, vertexShader );
			if (geomShaderID != 0)
			{
				mFuncs.glDetachShader( newProgram.mProgram, geomShaderID );
			}
		
			// Delete the fragment shader
			mFuncs.glDeleteShader( fragShaderID );
			CHECK_GL_ERROR;
			if (geomShaderID != 0)
			{
				mFuncs.glDeleteShader( geomShaderID );
			}
		
			// Check for link success
			GLint	linkStatus;
			mFuncs.glGetProgramiv( newProgram.mProgram, GL_LINK_STATUS, &linkStatus );
			CHECK_GL_ERROR;
		
			// Maybe validate the program
		#if Q3_DEBUG
			if ( (linkStatus == GL_TRUE) && (mFuncs.glValidateProgram != nullptr) )
			{
				mFuncs.glValidateProgram( newProgram.mProgram );
				mFuncs.glGetProgramiv( newProgram.mProgram, GL_VALIDATE_STATUS,
					&linkStatus );
			}
		#endif
			
			// Use program
			if (linkStatus == GL_TRUE)
			{
				InitUniformLocations( newProgram );
			
				ProgCache()->AddProgram( newProgram );
			}
			else
			{
			#if Q3_DEBUG
				GLint	logSize = 0;
				mFuncs.glGetProgramiv( newProgram.mProgram, GL_INFO_LOG_LENGTH, &logSize );
				CHECK_GL_ERROR;
				if (logSize > 0)
				{
					GLbyte*	theLog = (GLbyte*) Q3Memory_Allocate( logSize );
					if (theLog != nullptr)
					{
						mFuncs.glGetProgramInfoLog( newProgram.mProgram,
							logSize, nullptr, theLog );
						Q3_MESSAGE( "Failed to link program.  Error log:\n" );
						Q3_MESSAGE( (char*)theLog );
						Q3_MESSAGE( "\n" );
						Q3Memory_Free( &theLog );
					}
				}
				else
				{
					Q3_MESSAGE_FMT("Failed to link program, but log was empty!");
				}
			#endif

				E3ErrorManager_PostWarning( kQ3WarningShaderProgramLinkFailed );
			
				Q3_MESSAGE_FMT("Deleted program number %d", sProgramCount);
				--sProgramCount;
				mFuncs.glDeleteProgram( newProgram.mProgram );
			}
		}
		else
		{
			Q3_MESSAGE( "Failed to create program.\n" );
		}
	}
}




/*!
	@function	Cleanup
	@abstract	Delete program and shaders.
	@discussion	This should be called just before deleting an OpenGL
				draw context.  It will be called automatically by the
				destructor.
*/
void	QORenderer::PerPixelLighting::Cleanup()
{
}


/*!
	@function	UpdateIllumination
	@abstract	Notification that the type of illumination shader may
				have changed.
*/
void	QORenderer::PerPixelLighting::UpdateIllumination( TQ3ObjectType inIlluminationType )
{
	if (mProgramCharacteristic.mIlluminationType != inIlluminationType)
	{
		mProgramCharacteristic.mIlluminationType = inIlluminationType;
		
		GetLightTypes();
		
		mMayNeedProgramChange = true;
	}
}

/*!
	@function	UpdateInterpolationStyle
	@abstract	Notification that the type of illumination style may have
				changed.
*/
void	QORenderer::PerPixelLighting::UpdateInterpolationStyle(
									TQ3InterpolationStyle inInterpolation )
{
	if (mProgramCharacteristic.mInterpolationStyle != inInterpolation)
	{
		mProgramCharacteristic.mInterpolationStyle = inInterpolation;
		
		mMayNeedProgramChange = true;
	}
}

/*!
	@function	UpdateLineWidthStyle
	@abstract	Notification that the line width has changed.
*/
void	QORenderer::PerPixelLighting::UpdateLineWidthStyle()
{
	mMayNeedProgramChange = true;
}


/*!
	@function	UpdateFillStyle
	@abstract	Notification that the fill style may have changed.
*/
void	QORenderer::PerPixelLighting::UpdateFillStyle()
{
	if (mProgramCharacteristic.mFillStyle != mRenderer.GetStyleState().mFill)
	{
		mProgramCharacteristic.mFillStyle = mRenderer.GetStyleState().mFill;
		
		mMayNeedProgramChange = true;
	}
}

/*!
	@function	UpdateBackfacingStyle
	@abstract	Notification that the type of backfacing style may have
				changed.
*/
void	QORenderer::PerPixelLighting::UpdateBackfacingStyle(
									TQ3BackfacingStyle inBackfacing )
{
	bool shouldFlip = (inBackfacing != kQ3BackfacingStyleRemove);
	if (shouldFlip != mIsFlippingNormals)
	{
		mIsFlippingNormals = shouldFlip;
		
		mMayNeedProgramChange = true;
	}
	
	bool shouldCullBack = (inBackfacing == kQ3BackfacingStyleRemove);
	bool shouldCullFront = (inBackfacing == kQ3BackfacingStyleRemoveFront);
	if (shouldCullBack != mCullBackFaces)
	{
		mCullBackFaces = shouldCullBack;
		mMayNeedProgramChange = true;
	}
	if (shouldCullFront != mCullFrontFaces)
	{
		mCullFrontFaces = shouldCullFront;
		mMayNeedProgramChange = true;
	}
}

static QORenderer::EFogModeCombined
CombinedFogMode( const TQ3FogStyleExtendedData& inFog )
{
	QORenderer::EFogModeCombined mode = QORenderer::kFogModeOff;
	if (inFog.state == kQ3On)
	{
		if (inFog.halfspaceFogState == kQ3On)
		{
			mode = QORenderer::kFogModeHalfspace;
		}
		else if ( (inFog.mode >= kQ3FogModeLinear) &&
			(inFog.mode <= kQ3FogModeExponentialSquared) )
		{
			mode = static_cast< QORenderer::EFogModeCombined >( inFog.mode );
		}
	}
	return mode;
}

static bool operator!=( const TQ3RationalPoint4D& a, const TQ3RationalPoint4D& b )
{
	return fabsf( a.x - b.x ) + fabsf( a.y - b.y ) + fabsf( a.z - b.z ) +
		fabsf( a.w - b.w ) > kQ3RealZero;
}

static TQ3RationalPoint4D
ConvertPlaneWorldToEye( TQ3ViewObject inView,
						const TQ3RationalPoint4D& inWorldPlane )
{
	// Normalize the normal of the plane, so that the "rate" will have a more
	// predictable effect.
	TQ3RationalPoint4D normalizedWorldPlane;
	float length = sqrtf( inWorldPlane.x * inWorldPlane.x +
		inWorldPlane.y * inWorldPlane.y +
		inWorldPlane.z * inWorldPlane.z );
	float invLength = 1.0f / (length + kQ3MinFloat);
	normalizedWorldPlane.x = inWorldPlane.x * invLength; 
	normalizedWorldPlane.y = inWorldPlane.y * invLength;
	normalizedWorldPlane.z = inWorldPlane.z * invLength;
	normalizedWorldPlane.w = inWorldPlane.w * invLength;

	// Like a normal vector, a plane must be transformed by the inverse transpose.
	TQ3RationalPoint4D eyePlane;
	TQ3Matrix4x4 worldToView, viewToWorld, viewToWorldTranspose;
	CQ3ObjectRef theCamera( CQ3View_GetCamera( inView ) );
	Q3Camera_GetWorldToView( (TQ3Object _Nonnull) theCamera.get(), &worldToView );
	Q3Matrix4x4_Invert( &worldToView, &viewToWorld );
	Q3Matrix4x4_Transpose( &viewToWorld, &viewToWorldTranspose );
	Q3RationalPoint4D_Transform( &normalizedWorldPlane, &viewToWorldTranspose,
		&eyePlane );
	
	return eyePlane;
}

/*!
	@function	UpdateFogStyle
	@abstract	Notification that fog style has changed.
*/
void	QORenderer::PerPixelLighting::UpdateFogStyle(
									TQ3ViewObject inView,
									const TQ3FogStyleExtendedData& inFog )
{
	QORenderer::EFogModeCombined newMode = CombinedFogMode( inFog );
	TQ3RationalPoint4D newEyePlane = { 0.0f, 1.0f, 0.0f, 0.0f };
	if (newMode == kFogModeHalfspace)
	{
		newEyePlane = ConvertPlaneWorldToEye( inView, inFog.halfspaceFogPlane );
	}

	if ( (newMode != mProgramCharacteristic.mFogModeCombined) ||
		(newMode != kFogModeOff)
	)
	{
		mProgramCharacteristic.mFogModeCombined = newMode;
		mMaxFogOpacity = inFog.maxOpacity;
		mHalfspaceFogRate = inFog.halfspaceFogRate;
		mHalfspaceFogPlane = newEyePlane;
		
		if (mPassNumber > 1)
		{
			// Here's why we need to mess with the fog color:  The contributions of
			// lighting passes are blended additively, which roughly looks like:
			//   pixelColor = geomColor*light1 + geomColor*light2 + ...
			// The computation of fog looks like
			//   fragColor = (1-fog)*fogColor + fog*fragColor.
			// What we would like to get as the fogged result is
			//   pixelColor = (1-fog)*fogColor + fog*(geomColor*light1 + geomColor*light2 + ...)
			// But if we used fog in the normal way in each pass, we would end up with
			//	  pixelColor = (1-fog)*fogColor + fog*geomColor*light1 +
			//		(1-fog)*fogColor + fog*geomColor*light2 + ...
			// To avoid these multiple contributions of the fog color, we set the fog
			// color to black for all but the first pass.
			mFogColor = kBlackColor;
		}
		else
		{
			mFogColor.r = inFog.color.r;
			mFogColor.g = inFog.color.g;
			mFogColor.b = inFog.color.b;
		}
		mFogDensity = inFog.density;
		if (newMode == kFogModeLinear)
		{
			mLinearFogEnd = inFog.fogEnd;
			mLinearFogScale = 1.0f / (inFog.fogEnd - inFog.fogStart);
		}
		
		mMayNeedProgramChange = true;
	}
}


/*!
	@function	UpdateLighting
	@abstract	Notification that there has been a change in which OpenGL
				lights are on.
*/
void	QORenderer::PerPixelLighting::UpdateLighting()
{
	GetLightTypes();
	mMayNeedProgramChange = true;
}


/*!
	@function	UpdateTexture
	@abstract	Notification that there has been a change in whether we
				are texturing.
*/
void	QORenderer::PerPixelLighting::UpdateTexture( bool inTexturing  )
{
	if (inTexturing != mProgramCharacteristic.mIsTextured)
	{
		mProgramCharacteristic.mIsTextured = inTexturing;
		
		mMayNeedProgramChange = true;
	}
}


/*!
	@function	UpdateSpecularMapping
	@abstract	Notification that there has been a change in whether we are using
				a shininess (specular) map.
*/
void	QORenderer::PerPixelLighting::UpdateSpecularMapping( bool inSpecularMapped )
{
	if (inSpecularMapped != mIsSpecularMapped)
	{
		mIsSpecularMapped = inSpecularMapped;
		mMayNeedProgramChange = true;
	}
}


/*!
	@function	PreGeomSubmit
	@abstract	This is called just before a geometry will be rendered, to
				update the fragment shader program if necessary.  The
				geometry is passed, if available, so that cartoon parameters
				may be updated.
	@param		inGeom		Geometry being rendered.  May be nullptr.
	@param		inDimension	Dimension of geometry being rendered.
							Must be 0 (points), 1 (lines or polylines), or
							2 (triangles).
*/
void	QORenderer::PerPixelLighting::PreGeomSubmit( TQ3GeometryObject inGeom,
													int inDimension )
{
	CQ3ObjectRef lightGroup( CQ3View_GetLightGroup( mView.get() ) );
	TQ3Uns32 lightGroupEditIndex = Q3Shared_GetEditIndex( (TQ3Object _Nonnull) lightGroup.get() );
	if (lightGroupEditIndex != mLightGroupEditIndex)
	{
		UpdateLighting();
	}

	bool cartoonUpdate = false;
	CHECK_GL_ERROR_MSG("PreGeomSubmit 1");
	//Q3_MESSAGE_FMT("PreGeomSubmit dimension %d", inDimension );
	
	if ( (inGeom != nullptr) && (mQuantization > 0.0f) )
	{
		TQ3Boolean	isNonCartoon = kQ3False;
		
		Q3Object_GetProperty( inGeom, kQ3GeometryPropertyNonCartoon,
			sizeof(TQ3Boolean), nullptr, &isNonCartoon );
		
		bool	isCartoonish = (isNonCartoon == kQ3False);
		
		if (isCartoonish != mProgramCharacteristic.mIsCartoonish)
		{
			mProgramCharacteristic.mIsCartoonish = isCartoonish;
			mMayNeedProgramChange = true;
			cartoonUpdate = true;
		}
	}
	
	TQ3Status hadProp = Q3Object_GetProperty( mRendererObject,
		kQ3RendererPropertyClippingPlane, sizeof(TQ3RationalPoint4D), nullptr,
		&mClippingPlane );
	bool useClipPlane = (hadProp == kQ3Success);
	if (useClipPlane != mProgramCharacteristic.mIsUsingClippingPlane)
	{
		mProgramCharacteristic.mIsUsingClippingPlane = useClipPlane;
		mMayNeedProgramChange = true;
	}
	CHECK_GL_ERROR_MSG("PreGeomSubmit 2");
	
	if (inDimension != mProgramCharacteristic.mDimension)
	{
		mProgramCharacteristic.mDimension = inDimension;
		mMayNeedProgramChange = true;
	}
	
	ChooseProgram();
	
	if (cartoonUpdate && (mCurrentProgram != nullptr))
	{
		mFuncs.glUniform1f( mCurrentProgram->mQuantizationUniformLoc,
			mProgramCharacteristic.mIsCartoonish? mQuantization : 0.0f );
	}
	
	if ( useClipPlane && (mCurrentProgram != nullptr) )
	{
		mFuncs.glUniform4f( mCurrentProgram->mClippingPlaneUniformLoc,
			mClippingPlane.x, mClippingPlane.y,
			mClippingPlane.z, mClippingPlane.w );
	}
}

/*!
	@function	SetAmbientLight
	@abstract	Set the color/brightness of ambient light.
	@param		inAmbientColor		Ambient light color.
*/
void	QORenderer::PerPixelLighting::SetAmbientLight( const TQ3ColorRGB& inAmbientColor )
{
	mAmbientLight = inAmbientColor;
}

/*!
	@function	SetAlphaThreshold
	@abstract	Set a minimum alpha below which texels will be discarded.
	@param		inThreshold		Alpha threshold.  Default is 0.0.
*/
void	QORenderer::PerPixelLighting::SetAlphaThreshold( float inThreshold )
{
	mAlphaThreshold = inThreshold;
	mMayNeedProgramChange = true;
}



/*!
	@function	SetModelViewMatrix
	@abstract	Supply a new value for the model-view matrix.  It will be passed
				to the current program and to any program that is subsequently
				made current.
	@param		inMtx					A new modelview matrix.
*/
void	QORenderer::PerPixelLighting::SetModelViewMatrix( const TQ3Matrix4x4& inMtx )
{
	mModelViewMtx = inMtx;
	
	// Set mNormalMtx to be the upper left 3x3 of mModelViewMtx
	int i, j;
	for (i = 0; i < 3; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			mNormalMtx.value[i][j] = inMtx.value[i][j];
		}
	}
	// Then invert and transpose.
	Q3Matrix3x3_Invert( &mNormalMtx, &mNormalMtx );
	Q3Matrix3x3_Transpose( &mNormalMtx, &mNormalMtx );
	
	if (mCurrentProgram != nullptr)
	{
		mFuncs.glUniformMatrix4fv( mCurrentProgram->mModelViewMtxUniformLoc, 1, GL_FALSE, &mModelViewMtx.value[0][0] );
		mFuncs.glUniformMatrix3fv( mCurrentProgram->mNormalMtxUniformLoc, 1, GL_FALSE, &mNormalMtx.value[0][0] );
	}
}

/*!
	@function	SetProjectionMatrix
	@abstract	Supply a new value for the projection matrix.  It will be passed
				to the current program and to any program that is subsequently
				made current.
	@param		inMtx					A new projection matrix.
*/
void	QORenderer::PerPixelLighting::SetProjectionMatrix( const TQ3Matrix4x4& inMtx )
{
	mProjectionMtx = inMtx;
	
	if (mCurrentProgram != nullptr)
	{
		SetCameraUniforms();
	}
}

void	QORenderer::PerPixelLighting::SetCameraUniforms()
{
	mFuncs.glUniformMatrix4fv( mCurrentProgram->mProjectionMtxUniformLoc, 1, GL_FALSE, &mProjectionMtx.value[0][0] );
	
	if (mView.get() != nullptr)
	{
		CQ3ObjectRef theCamera( CQ3View_GetCamera( mView.get() ) );
		TQ3CameraData camData;
		Q3Camera_GetData( (TQ3Object _Nonnull) theCamera.get(), &camData );
		
		if (mCurrentProgram->mCameraRangeUniformLoc >= 0)
		{
			GLfloat range[] = {
				camData.range.hither, camData.range.yon
			};
			mFuncs.glUniform2fv( mCurrentProgram->mCameraRangeUniformLoc, 1,
				range );
		}
		
		if (mCurrentProgram->mCameraViewportUniformLoc >= 0)
		{
			GLfloat viewport[] = {
				camData.viewPort.origin.x, camData.viewPort.origin.y,
				camData.viewPort.width, camData.viewPort.height
			};
			mFuncs.glUniform4fv( mCurrentProgram->mCameraViewportUniformLoc, 1,
				viewport );
		}
		
		if (Q3Camera_GetType( (TQ3Object _Nonnull) theCamera.get() ) == kQ3CameraTypeFisheye)
		{
			TQ3FisheyeCameraData fisheyeData;
			Q3FisheyeCamera_GetData( (TQ3Object _Nonnull) theCamera.get(), &fisheyeData );
			mFuncs.glUniform2fv( mCurrentProgram->mSensorSizeAttribLoc, 1,
				&fisheyeData.sensorSize.x );
			CHECK_GL_ERROR;
			mFuncs.glUniform1f( mCurrentProgram->mFocalLengthAttribLoc, fisheyeData.focalLength );
			CHECK_GL_ERROR;
			float angleOfView = Q3FisheyeCamera_CalcAngleOfView(
				&fisheyeData.sensorSize, fisheyeData.mappingFunction,
				fisheyeData.croppingFormat, fisheyeData.focalLength );
			mFuncs.glUniform1f( mCurrentProgram->mAngleOfViewAttribLoc, angleOfView );
			CHECK_GL_ERROR;
			GLint func = (GLint) fisheyeData.mappingFunction;
			mFuncs.glUniform1i( mCurrentProgram->mFisheyeMappingFuncAttribLoc, func );
			CHECK_GL_ERROR;
			mFuncs.glUniform1i( mCurrentProgram->mFisheyeCroppingAttribLoc, (GLint)fisheyeData.croppingFormat );
		}
	}
}

/*!
	@function	SetTextureMatrix
	@abstract	Supply a new value for the texture UV transformation matrix.  It will be passed
				to the current program and to any program that is subsequently
				made current.
	@param		inMtx					A new texture UV transformation matrix.
*/
void	QORenderer::PerPixelLighting::SetTextureMatrix( const TQ3Matrix4x4& inMtx )
{
	mTextureMtx = inMtx;
	
	if (mCurrentProgram != nullptr)
	{
		mFuncs.glUniformMatrix4fv( mCurrentProgram->mTextureMtxUniformLoc, 1, GL_FALSE, &mTextureMtx.value[0][0] );
	}
}

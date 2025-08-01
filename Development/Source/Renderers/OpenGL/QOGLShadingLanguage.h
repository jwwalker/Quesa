/*!
	@header		QOGLShadingLanguage.h
	
	This header holds declarations relating to use of OpenGL Shading Language
	in the Quesa OpenGL renderer.
*/
/*  NAME:
       QOGLShadingLanguage.h

    DESCRIPTION:
        Header for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2025, Quesa Developers. All rights reserved.

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
#ifndef QOSHADINGLANGUAGE_HDR
#define QOSHADINGLANGUAGE_HDR

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QOPrefix.h"
#include "QuesaStyle.h"
#include "QOShaderProgramCache.h"
#include "CQ3WeakObjectRef.h"

#include <vector>


//=============================================================================
//      Macros
//-----------------------------------------------------------------------------

#ifndef GL_VERTEX_SHADER
	#define GL_VERTEX_SHADER                  0x8B31
	#define GL_FRAGMENT_SHADER                0x8B30
#endif

#ifndef	GL_COMPILE_STATUS
	#define GL_COMPILE_STATUS                 0x8B81
	#define GL_LINK_STATUS                    0x8B82
#endif

#ifndef GL_INFO_LOG_LENGTH
	#define		GL_INFO_LOG_LENGTH			0x8B84
#endif


namespace QORenderer
{

//=============================================================================
//      Types
//-----------------------------------------------------------------------------
class Renderer;

/*!
	@typedef	ObVec
	@abstract	A vector of Quesa objects, here used for light objects.
*/
typedef std::vector< CQ3ObjectRef >	ObVec;

typedef	std::vector<ProgramRec>		ProgramVec;

typedef GLuint (QO_PROCPTR_TYPE glCreateShaderProc )(GLenum type);
typedef void (QO_PROCPTR_TYPE glShaderSourceProc )(GLuint shader,
													GLsizei count,
													const char* *string,
													const GLint *length);
typedef void (QO_PROCPTR_TYPE glCompileShaderProc )(GLuint shader);
typedef GLuint (QO_PROCPTR_TYPE glCreateProgramProc )(void);
typedef void (QO_PROCPTR_TYPE glAttachShaderProc )(GLuint program,
													GLuint shader);
typedef void (QO_PROCPTR_TYPE glDetachShaderProc ) (GLuint program,
													GLuint shader);
typedef void (QO_PROCPTR_TYPE glLinkProgramProc )(GLuint program);
typedef void (QO_PROCPTR_TYPE glValidateProgramProc )(GLuint program);
typedef void (QO_PROCPTR_TYPE glUseProgramProc )(GLuint program);
typedef void (QO_PROCPTR_TYPE glDeleteProgramProc )(GLuint program);
typedef void (QO_PROCPTR_TYPE glDeleteShaderProc )(GLuint shader);
typedef void (QO_PROCPTR_TYPE glUniform1iProc )(GLint location, GLint v0);
typedef void (QO_PROCPTR_TYPE glUniform1fProc )(GLint location, GLfloat v0);
typedef void (QO_PROCPTR_TYPE glUniform4fProc )(GLint location, GLfloat v0,
									GLfloat v1, GLfloat v2, GLfloat v3 );
typedef void (QO_PROCPTR_TYPE glUniform1fvProc )(GLint location, GLsizei count,
												const GLfloat* values);
typedef void (QO_PROCPTR_TYPE glUniform2fvProc )(GLint location, GLsizei count,
												const GLfloat* values);
typedef void (QO_PROCPTR_TYPE glUniform3fvProc )(GLint location, GLsizei count,
												const GLfloat* values);
typedef void (QO_PROCPTR_TYPE glUniform4fvProc )(GLint location, GLsizei count,
												const GLfloat* values);
typedef void (QO_PROCPTR_TYPE glGetShaderivProc )(GLuint shader,
													GLenum pname,
													GLint *params);
typedef void (QO_PROCPTR_TYPE glGetProgramivProc )(GLuint program,
													GLenum pname,
													GLint *params);
typedef GLint (QO_PROCPTR_TYPE glGetUniformLocationProc )(GLuint program,
													const char *name);
typedef void (QO_PROCPTR_TYPE glGetProgramInfoLogProc )(GLuint program,
													GLsizei maxlength,
													GLsizei* outLength,
													GLbyte * infoLog );
typedef void (QO_PROCPTR_TYPE glGetShaderInfoLogProc )(GLuint shader,
													GLsizei maxlength,
													GLsizei* outLength,
													GLbyte * infoLog );
typedef void (QO_PROCPTR_TYPE glGetShaderSourceProc)( GLuint shder,
													GLsizei bufSize,
													GLsizei* outLength,
													char* outSource );
typedef GLboolean (QO_PROCPTR_TYPE glIsProgramProc)( GLuint program );
typedef void (QO_PROCPTR_TYPE glUniformMatrix4fvProc)(GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat *value);
typedef void (QO_PROCPTR_TYPE glUniformMatrix3fvProc)(GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat *value);

typedef GLint (QO_PROCPTR_TYPE glGetAttribLocationProc)( GLuint program, const char *name );
typedef void (QO_PROCPTR_TYPE glEnableVertexAttribArrayProc)( GLuint index );
typedef void (QO_PROCPTR_TYPE glDisableVertexAttribArrayProc)( GLuint index );
typedef void (QO_PROCPTR_TYPE glVertexAttrib3fvProc)( GLuint index, const GLfloat *v );
typedef void (QO_PROCPTR_TYPE glVertexAttribPointerProc)(
													GLuint index,
													GLint size,
													GLenum type,
													GLboolean normalized,
													GLsizei stride,
													const GLvoid *pointer );
typedef void (QO_PROCPTR_TYPE glBindAttribLocationProc)(GLuint program, GLuint index, const char* name);


/*!
	@struct		GLSLFuncs
	@abstract	Function pointers for OpenGL Shading Language.
*/
struct GLSLFuncs
{
								GLSLFuncs();
	
	/*!
		@function	Initialize
		@abstract	Get the function pointers.  This should be called just
					after the OpenGL context is created.
	*/
	void						Initialize( const TQ3GLExtensions& inExts );
								
	glCreateShaderProc			glCreateShader;
	glShaderSourceProc			glShaderSource;
	glCompileShaderProc			glCompileShader;
	glGetShaderivProc			glGetShaderiv;
	glCreateProgramProc			glCreateProgram;
	glAttachShaderProc			glAttachShader;
	glDetachShaderProc			glDetachShader;
	glLinkProgramProc			glLinkProgram;
	glValidateProgramProc		glValidateProgram;
	glGetProgramivProc			glGetProgramiv;
	glUseProgramProc			glUseProgram;
	glGetUniformLocationProc	glGetUniformLocation;
	glUniform1iProc				glUniform1i;
	glUniform1fProc				glUniform1f;
	glUniform4fProc				glUniform4f;
	glUniform1fvProc			glUniform1fv;
	glUniform2fvProc			glUniform2fv;
	glUniform3fvProc			glUniform3fv;
	glUniform4fvProc			glUniform4fv;
	glUniformMatrix4fvProc		glUniformMatrix4fv;
	glUniformMatrix3fvProc		glUniformMatrix3fv;
	glDeleteShaderProc			glDeleteShader;
	glDeleteProgramProc			glDeleteProgram;
	glGetProgramInfoLogProc		glGetProgramInfoLog;
	glGetShaderInfoLogProc		glGetShaderInfoLog;
	glGetShaderSourceProc		glGetShaderSource;
	glIsProgramProc				glIsProgram;
	
	glGetAttribLocationProc			glGetAttribLocation;
	glEnableVertexAttribArrayProc	glEnableVertexAttribArray;
	glDisableVertexAttribArrayProc	glDisableVertexAttribArray;
	glVertexAttrib3fvProc			glVertexAttrib3fv;
	glVertexAttribPointerProc		glVertexAttribPointer;
	glBindAttribLocationProc		glBindAttribLocation;

private:
	void						SetNULL();
};

/*!
	@class		PerPixelLighting
	@abstract	Object to handle vertex and fragment shaders that implement
				per-pixel lighting for the OpenGL renderer.
*/
class PerPixelLighting
{
public:
								PerPixelLighting(
										Renderer& inRenderer,
										const GLSLFuncs& inFuncs,
										TQ3RendererObject inRendererObject,
										TQ3GLContext& inGLContext,
										const TQ3GLExtensions& inExtensions );
	
								~PerPixelLighting();
								
	/*!
		@function	Cleanup
		@abstract	Delete program and shaders.
		@discussion	This should be called just before deleting an OpenGL
					draw context.  It will be called automatically by the
					destructor.
	*/
	void						Cleanup();
	
	/*!
		@function	StartFrame
		@abstract	Begin a rendering frame.
	*/
	void						StartFrame( TQ3ViewObject inView );
	
	/*!
		@function	StartPass
		@abstract	Begin a rendering pass.
		@discussion	This is where we check whether per-pixel lighting is
					desired.  If so, we set up the shaders and program.
	*/
	void						StartPass( TQ3CameraObject inCamera );
	
	/*!
		@function	EndPass
		@abstract	Finish a rendering pass.
	*/
	void						EndPass();

	/*!
		@function	GetMaxLightsPerPass
		@abstract	Get the maximum number of lights that can be used in
					one pass.  (Valid after StartFrame has been called.)
	*/
	int							GetMaxLightsPerPass() const
								{
									return mMaxLights;
								}
	
	/*!
		@function	ClearLights
		@abstract	Forget lights that were previously passed to AddLight.
		@discussion	This will be called by the Lights object when it is starting
					a pass, which happens before the PerPixelLighting gets a
					StartPass call.
	*/
	void						ClearLights();
	
	/*!
		@function	AddLight
		@abstract	The Lights object uses this to inform the PerPixelLighting
					object about a light being used for this pass.
	*/
	void						AddLight( TQ3LightObject inLight );
	
	/*!
		@function	UpdateIllumination
		@abstract	Notification that the type of illumination shader may
					have changed.
	*/
	void						UpdateIllumination(
										TQ3ObjectType inIlluminationType );

	/*!
		@function	UpdateInterpolationStyle
		@abstract	Notification that the type of illumination style may have
					changed.
	*/
	void						UpdateInterpolationStyle(
										TQ3InterpolationStyle inInterpolation );
	
	/*!
		@function	UpdateLineWidthStyle
		@abstract	Notification that the line width has changed.
	*/
	void						UpdateLineWidthStyle();

	/*!
		@function	UpdateBackfacingStyle
		@abstract	Notification that the type of backfacing style may have
					changed.
	*/
	void						UpdateBackfacingStyle(
										TQ3BackfacingStyle inBackfacing );
	
	/*!
		@function	UpdateFillStyle
		@abstract	Notification that the fill style may have changed.
	*/
	void						UpdateFillStyle();

	/*!
		@function	UpdateFogStyle
		@abstract	Notification that fog style has changed.
	*/
	void						UpdateFogStyle( TQ3ViewObject inView,
											const TQ3FogStyleExtendedData& inFog );

	/*!
		@function	UpdateTexture
		@abstract	Notification that there has been a change in whether we
					are texturing.
	*/
	void						UpdateTexture( bool inTexturing );
	
	/*!
		@function	UpdateSpecularMapping
		@abstract	Notification that there has been a change in whether we are using
					a shininess (specular) map.
	*/
	void						UpdateSpecularMapping( bool inSpecularMapped );
	
	/*!
		@function	UpdateEmissiveMapping
		@abstract	Notification that there has been a change in whether we are using
					an emissive color map.
	*/
	void						UpdateEmissiveMapping( bool inEmissiveMapped );
	
	/*!
		@function	UpdateLighting
		@abstract	Notification that there has been a change in which OpenGL
					lights are on.
	*/
	void						UpdateLighting();
	
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
	void						PreGeomSubmit( TQ3GeometryObject inGeom,
												int inDimension );
	
	/*!
		@function	SetAmbientLight
		@abstract	Set the color/brightness of ambient light.
		@param		inAmbientColor		Ambient light color.
	*/
	void						SetAmbientLight( const TQ3ColorRGB& inAmbientColor );

	/*!
		@function	SetAlphaThreshold
		@abstract	Set a minimum alpha below which texels will be discarded.
		@param		inThreshold		Alpha threshold.  Default is 0.0.
	*/
	void						SetAlphaThreshold( float inThreshold );

	/*!
		@function	SetModelViewMatrix
		@abstract	Supply a new value for the model-view matrix.  It will be passed
					to the current program and to any program that is subsequently
					made current.
		@param		inMtx					A new modelview matrix.
	*/
	void						SetModelViewMatrix( const TQ3Matrix4x4& inMtx );
	
	const TQ3Matrix4x4&			GetModelViewMatrix() const { return mModelViewMtx; }

	/*!
		@function	SetProjectionMatrix
		@abstract	Supply a new value for the projection matrix.  It will be passed
					to the current program and to any program that is subsequently
					made current.
		@param		inMtx					A new projection matrix.
	*/
	void						SetProjectionMatrix( const TQ3Matrix4x4& inMtx );

	/*!
		@function	SetTextureMatrix
		@abstract	Supply a new value for the texture UV transformation matrix.  It will be passed
					to the current program and to any program that is subsequently
					made current.
		@param		inMtx					A new texture UV transformation matrix.
	*/
	void						SetTextureMatrix( const TQ3Matrix4x4& inMtx );
	
	/*!
		@function	CurrentProgram
		@abstract	Accessor for the current program record.
	*/
	const ProgramRec*			CurrentProgram() const { return mCurrentProgram; }
	
	const GLSLFuncs&			Funcs() const { return mFuncs; }

private:
	void						CheckIfShading();
	void						InitVertexShader();
	void						InitProgram();
	void						InitUniformLocations( ProgramRec& ioProgram );
	void						ChooseProgram();
	void						GetLightTypes();
	void						SetUniformValues();
	void						SetCameraUniforms();
	ProgramCache*				ProgCache();
	void						CalcMaxLights();
	
	void						AddDirectionalLight( TQ3LightObject inLight );
	void						AddSpotLight( TQ3LightObject inLight );
	void						AddPointLight( TQ3LightObject inLight );
	
	Renderer&					mRenderer;
	const GLSLFuncs&			mFuncs;
	TQ3RendererObject			mRendererObject;
	TQ3GLContext&				mGLContext;
	CQ3WeakObjectRef			mView;
	TQ3Uns32					mLightGroupEditIndex;
	int							mPassNumber;
	bool						mMayNeedProgramChange;
	bool						mIsSpecularMapped;
	bool						mIsEmissiveMapped;
	bool						mIsFlippingNormals;
	bool						mCullBackFaces;
	bool						mCullFrontFaces;
	TQ3ColorRGB					mAmbientLight;
	TQ3Float32					mQuantization;
	TQ3Float32					mLightNearEdge;
	TQ3RationalPoint4D			mClippingPlane;
	TQ3ColorRGB					mFogColor;
	float						mFogDensity;
	float						mLinearFogEnd;
	float						mLinearFogScale;
	TQ3Float32					mMaxFogOpacity;
	float						mHalfspaceFogRate;
	TQ3RationalPoint4D			mHalfspaceFogPlane; // in eye (view) coordinates
	TQ3Matrix4x4				mWorldToView;
	int							mMaxLights;
	TQ3Matrix4x4				mModelViewMtx;
	TQ3Matrix4x4				mProjectionMtx;
	TQ3Matrix4x4				mTextureMtx;
	TQ3Matrix3x3				mNormalMtx;
	float						mAlphaThreshold;

	ProgramCharacteristic		mProgramCharacteristic;

	ObVec						mLights;	// Quesa light objects
	
	// Data about lights that will become uniform variables
	std::vector<TQ3ObjectType>			mLightTypes;
	std::vector<TQ3RationalPoint4D>		mLightPositions;
	std::vector<TQ3ColorRGBA>			mLightColors;
	std::vector<TQ3Vector3D>			mSpotLightDirections;
	std::vector<GLfloat>				mSpotLightHotAngles;
	std::vector<GLfloat>				mSpotLightCutoffAngles;
	std::vector<TQ3Vector3D>			mLightAttenuations; // x constant, y linear, z quadratic
	
	const ProgramRec*			mCurrentProgram;
};


}	// end QORenderer namespace

#endif

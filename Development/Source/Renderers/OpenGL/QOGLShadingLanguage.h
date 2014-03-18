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
        Copyright (c) 2007-2013, Quesa Developers. All rights reserved.

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
#ifndef QOSHADINGLANGUAGE_HDR
#define QOSHADINGLANGUAGE_HDR

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QOPrefix.h"
#include "QuesaStyle.h"
#include "QOShaderProgramCache.h"

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
typedef void (QO_PROCPTR_TYPE glUseProgramProc )(GLuint program);
typedef void (QO_PROCPTR_TYPE glDeleteProgramProc )(GLuint program);
typedef void (QO_PROCPTR_TYPE glDeleteShaderProc )(GLuint shader);
typedef void (QO_PROCPTR_TYPE glUniform1iProc )(GLint location, GLint v0);
typedef void (QO_PROCPTR_TYPE glUniform1fProc )(GLint location, GLfloat v0);
typedef void (QO_PROCPTR_TYPE glUniform1fvProc )(GLint location, GLsizei count,
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
	glGetProgramivProc			glGetProgramiv;
	glUseProgramProc			glUseProgram;
	glGetUniformLocationProc	glGetUniformLocation;
	glUniform1iProc				glUniform1i;
	glUniform1fProc				glUniform1f;
	glUniform1fvProc			glUniform1fv;
	glDeleteShaderProc			glDeleteShader;
	glDeleteProgramProc			glDeleteProgram;
	glGetProgramInfoLogProc		glGetProgramInfoLog;
	glGetShaderInfoLogProc		glGetShaderInfoLog;

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
	void						StartFrame();
	
	/*!
		@function	StartPass
		@abstract	Begin a rendering pass.
		@discussion	This is where we check whether per-pixel lighting is
					desired.  If so, we set up the shaders and program.
	*/
	void						StartPass();
	
	/*!
		@function	EndPass
		@abstract	Finish a rendering pass.
	*/
	void						EndPass();
	
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
		@function	UpdateFogStyle
		@abstract	Notification that fog style has changed.
	*/
	void						UpdateFogStyle( const TQ3FogStyleData& inFog );

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
		@param		inGeom		Geometry being rendered.  May be NULL.
	*/
	void						PreGeomSubmit( TQ3GeometryObject inGeom );

private:
	void						CheckIfShading();
	void						InitVertexShader();
	void						InitProgram();
	void						InitUniformLocations( ProgramRec& ioProgram );
	void						ChooseProgram();
	void						GetLightTypes();
	void						SetUniformValues( const ProgramRec& ioProgram );
	ProgramCache*				ProgCache();
	
	const GLSLFuncs&			mFuncs;
	const TQ3GLExtensions&		mGLExtensions;
	TQ3RendererObject			mRendererObject;
	TQ3GLContext&				mGLContext;
	bool						mIsShading;
	bool						mMayNeedProgramChange;
	bool						mIsSpecularMapped;
	TQ3Float32					mQuantization;
	TQ3Float32					mLightNearEdge;

	ProgramCharacteristic		mProgramCharacteristic;

	ObVec						mLights;
	
	const ProgramRec*			mCurrentProgram;
};


}	// end QORenderer namespace

#endif

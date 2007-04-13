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
#ifndef QOSHADINGLANGUAGE_HDR
#define QOSHADINGLANGUAGE_HDR

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QOPrefix.h"

#include <vector>


//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32
	#define		QO_PROCPTR_TYPE		__stdcall
#else
	#define		QO_PROCPTR_TYPE
#endif

#ifndef GL_VERTEX_SHADER
	#define GL_VERTEX_SHADER                  0x8B31
	#define GL_FRAGMENT_SHADER                0x8B30
#endif

#ifndef	GL_COMPILE_STATUS
	#define GL_COMPILE_STATUS                 0x8B81
	#define GL_LINK_STATUS                    0x8B82
#endif

namespace QORenderer
{
//=============================================================================
//      Constants
//-----------------------------------------------------------------------------

enum ELightType
{
	kLightTypeInvalid = -1,
	kLightTypeNone = 0,
	kLightTypeDirectional,
	kLightTypePositional
};

//=============================================================================
//      Types
//-----------------------------------------------------------------------------

typedef GLuint (* QO_PROCPTR_TYPE glCreateShaderProc )(GLenum type);
typedef void (* QO_PROCPTR_TYPE glShaderSourceProc )(GLuint shader,
													GLsizei count,
													const char* *string,
													const GLint *length);
typedef void (* QO_PROCPTR_TYPE glCompileShaderProc )(GLuint shader);
typedef GLuint (* QO_PROCPTR_TYPE glCreateProgramProc )(void);
typedef void (* QO_PROCPTR_TYPE glAttachShaderProc )(GLuint program,
													GLuint shader);
typedef void (* QO_PROCPTR_TYPE glDetachShaderProc ) (GLuint program,
													GLuint shader);
typedef void (* QO_PROCPTR_TYPE glLinkProgramProc )(GLuint program);
typedef void (* QO_PROCPTR_TYPE glUseProgramProc )(GLuint program);
typedef void (* QO_PROCPTR_TYPE glDeleteProgramProc )(GLuint program);
typedef void (* QO_PROCPTR_TYPE glDeleteShaderProc )(GLuint shader);
typedef void (* QO_PROCPTR_TYPE glUniform1iProc )(GLint location, GLint v0);
typedef void (* QO_PROCPTR_TYPE glGetShaderivProc )(GLuint shader,
													GLenum pname,
													GLint *params);
typedef void (* QO_PROCPTR_TYPE glGetProgramivProc )(GLuint program,
													GLenum pname,
													GLint *params);
typedef GLint (* QO_PROCPTR_TYPE glGetUniformLocationProc )(GLuint program,
													const char *name);

/*!
	@struct		GLSLFuncs
	@abstract	Function pointers for OpenGL Shading Language.
*/
struct GLSLFuncs
{
								GLSLFuncs();
								
	void						SetNULL();
	
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
	glDeleteShaderProc			glDeleteShader;
	glDeleteProgramProc			glDeleteProgram;
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
										GLSLFuncs& inFuncs,
										TQ3RendererObject inRendererObject );
	
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
		@function	UpdateIllumination
		@abstract	Notification that the type of illumination shader may
					have changed.
	*/
	void						UpdateIllumination(
										TQ3ObjectType inIlluminationType );

	/*!
		@function	UpdateTexture
		@abstract	Notification that there has been a change in whether we
					are texturing or which texture is active.
	*/
	void						UpdateTexture();

private:
	void						CheckIfShading();
	void						InitProgram();
	void						AttachDirectionalShader( GLint inLightIndex );
	void						AttachPositionalShader( GLint inLightIndex );
	void						InitUniforms();
	void						DetachFragmentShaders();
	
	GLSLFuncs&					mFuncs;
	TQ3RendererObject			mRendererObject;
	bool						mIsShading;
	TQ3ObjectType				mIlluminationType;
	bool						mIsTextured;
	GLuint						mProgram;
	GLint						mIsTexturedUniformLoc;
	GLint						mTextureUnitUniformLoc;
	GLint						mIlluminationTypeUniformLoc;
	
	std::vector<GLuint>			mDirectionalLightShaders;
	std::vector<GLuint>			mPositionalLightShaders;
	std::vector<GLuint>			mAttachedFragmentShaders;
	std::vector<ELightType>		mLightTypes;
};


}	// end QORenderer namespace

#endif

/*  NAME:
       QOGLShadingLanguage.cpp

    DESCRIPTION:
        Shading language functions for Quesa OpenGL renderer class.
		    
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
#include "QOGLShadingLanguage.h"
#include "GLUtils.h"
#include "E3Prefix.h"
#include "QuesaRenderer.h"

#include <cstring>
#include <string>
#include <algorithm>



//=============================================================================
//      Local constants
//-----------------------------------------------------------------------------

namespace
{
	const char* kVertexShaderSource =
				// Normal vector in eye coordinates
				"varying vec3 ECNormal;"
				""
				// Position in eye coordinates
				"varying vec3 ECPos3;"
				""
				"void main()"
				"{"
					// Transform normal to eye coordinates, and let it
					// be interpolated across the primitive.
					// There is no point normalizing yet.
					"ECNormal = gl_NormalMatrix * gl_Normal;"

				// Transform vertex position to eye coordinates.
					"vec4 ECPosition = gl_ModelViewMatrix * gl_Vertex;"

				// Convert to 3 dimensions.
					"ECPos3 = ECPosition.xyz / ECPosition.w;"

					"gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"

					"gl_FrontColor = gl_Color;"

					"gl_Position = ftransform();"
				"}";
	
	const char* kDirectionalLightFragmentShaderSource =
				"void FUNC_NAME("
				"				in vec3 normal,"
				"				inout vec3 diffuse,"
				"				inout vec3 specular )"
				"{"
				"	float nDotVP = max( 0.0, dot( normal,"
				"		gl_LightSource[LIGHT_INDEX].position.xyz ) );"
				"	float nDotHV = max( 0.0, "
				"		dot( normal, gl_LightSource[LIGHT_INDEX].halfVector.xyz ) );"
				"	float pf = 0.0;"
				""
				"	if (nDotVP > 0.0)"
				"	{"
				"		pf = pow( nDotHV, gl_FrontMaterial.shininess );"
				"	}"

				"	diffuse += gl_LightSource[LIGHT_INDEX].diffuse.rgb * nDotVP;"
				"	specular += gl_LightSource[LIGHT_INDEX].specular.rgb * pf;"
				"}";

	const char* kPositionalLightFragmentShaderSource =
				// Position in eye coordinates
				"varying vec3 ECPos3;"

				"void FUNC_NAME("
				"				in vec3 eye,			// geometry to eye direction\n"
				"				in vec3 normal,"
				"				inout vec3 diffuse,"
				"				inout vec3 specular )"
				"{"
					// Compute vector from surface to light position
				"	vec3 geomToLight = gl_LightSource[LIGHT_INDEX].position.xyz - ECPos3;"

					// Compute distance between geometry and light
				"	float d = length(geomToLight);"

					// Normalize direction from geometry to light
				"	geomToLight /= d;"

					// Compute attenuation factor
				"	float attenuation = 1.0 / (gl_LightSource[LIGHT_INDEX].constantAttenuation +"
				"		gl_LightSource[LIGHT_INDEX].linearAttenuation * d +"
				"		gl_LightSource[LIGHT_INDEX].quadraticAttenuation * d * d );"
				
					// See if point on surface is inside cone of illumination,
					// and maybe attenuate by angle from center of spot.
					// Quesa never sets GL_SPOT_EXPONENT, meaning it has the
					// default value of 0, and we need not look at
					// gl_LightSource[LIGHT_INDEX].spotExponent.
				"	float spotDot = dot( -geomToLight, gl_LightSource[LIGHT_INDEX].spotDirection );"
					
					// Set attenuation to 0 if outside the spot light cone.
					// Note that for a point light, spotCosCutoff will be -1,
					// and spotDot will never be less than that.
				"	attenuation *= step( gl_LightSource[LIGHT_INDEX].spotCosCutoff, spotDot );"

					// Compute the direction halfway between the geometry to light vector
					// and the geometry to eye vectors.  This uses the assumption that
					// eye and geomToLight are both normalized.
				"	vec3 halfVector = normalize( geomToLight + eye );"

				"	float nDotGeomToLight = max( 0.0, dot( normal, geomToLight ) );"

				"	diffuse += gl_LightSource[LIGHT_INDEX].diffuse.rgb * nDotGeomToLight * attenuation;"

				"	float nDotHalf = max( 0.0, dot( normal, halfVector ) );"

				"	float pf;"
				"	if (nDotGeomToLight == 0.0)"
				"		pf = 0.0;"
				"	else"
				"		pf = pow( nDotHalf, gl_FrontMaterial.shininess );"

				"	specular += gl_LightSource[LIGHT_INDEX].specular.rgb * pf * attenuation;"
				"}";

	const char* kMainFragmentShaderPart1Source =
				// Normal vector in eye coordinates
				"varying vec3 ECNormal;"

				// Position in eye coordinates
				"varying vec3 ECPos3;"

				// Whether we have a texture on texture unit 0
				"uniform bool isTextured;"

				// Type of illumination shader
				// 0 == NULL (lighting disabled)
				// 1 = Lambert (no specularity)
				// 2 = Phong
				"uniform int IlluminationType;"

				// Sampler for texture unit 0
				"uniform sampler2D tex0;\n"

				"#define	DIRECTIONAL_LIGHT_PROTO(name) 	\\\n"
				"	void name(					\\\n"
				"				in vec3 normal,	\\\n"
				"				inout vec3 diffuse,\\\n"
				"				inout vec3 specular )\n"

				"#define DIRECTIONAL_LIGHT_CALL(name)	name( normal, diff, spec )\n"

				"#define	POSITIONAL_LIGHT_PROTO(name) 	\\\n"
				"	void name(					\\\n"
				"				in vec3 geomToEyeDir,	\\\n"
				"				in vec3 normal,	\\\n"
				"				inout vec3 diffuse,\\\n"
				"				inout vec3 specular )\n"

				"#define	POSITIONAL_LIGHT_CALL(name)	\\\n"
						"name( geomToEyeDir, normal, diff, spec )\n";
		
		// Between part 1 and part 2, we will insert some light shader prototypes.

	const char* kMainFragmentShaderPart2Source =
				"void main()"
				"{"
					// Color components, lights will add to these.
				"	vec3		diff = vec3(0.0);"
				"	vec3		spec = vec3(0.0);"

					// Eye coordinate normal vector.  Even if the vertex normals were normalized
					// and the modelview matrix has no scaling, we would still have to normalize
					// here due to interpolation.
				"	vec3		normal = normalize(ECNormal);"

				"	vec3		geomPos = ECPos3;"
				"	vec3		geomToEyeDir = - normalize( geomPos );"

					// Flip the normal for the back face.  If we are using
					// backfacing style Remove, then back face triangles will
					// not get here, so this does no harm except wasting a
					// little time.
				"	normal = faceforward( normal, geomPos, normal );";
		
		// Between part 2 and part 3, we will insert some light shader calls.

	const char* kMainFragmentShaderPart3Source =
				"	vec3	color;"
				"	float	alpha;"

				"	if (IlluminationType == 0)"
				"	{"
				"		color = gl_Color.rgb + gl_FrontMaterial.emission.rgb;"
				"	}"
				"	else"
				"	{"
						// Start with emissive and global ambient color.
						// I will assume that the only ambient light is global.
				"		color = gl_LightModel.ambient.rgb * gl_Color.rgb + "
							"gl_FrontMaterial.emission.rgb;"

						// Add diffuse color.
				"		color += diff * gl_Color.rgb;"
				"	}"

				"	alpha = gl_Color.a;"
				
					// Texturing, GL_MODULATE mode
				"	if (isTextured)"
				"	{"
				"		vec4 texColor = texture2D( tex0, gl_TexCoord[0].st );"
				"		color *= texColor.rgb;"
				"		alpha *= texColor.a;"
				"	}"

				"	if (IlluminationType == 2)"
				"	{"
						// Add specular color.  This is done after texturing, as with
						// separate specular color.
				"		color += spec * gl_FrontMaterial.specular.rgb;"
				"	}"
				
				" 	gl_FragColor.rgb = color;"
				"	gl_FragColor.a = alpha;"
				"}";
	
	const char* kLightShaderPrefixFormat =
				"#undef LIGHT_INDEX\n"
				"#undef FUNC_NAME\n"
				"#define LIGHT_INDEX %d\n"
				"#define FUNC_NAME Light%d\n";

	const char*	kDirectionalProtoFormat =
					"DIRECTIONAL_LIGHT_PROTO( Light%d );";
	const char*	kPositionalProtoFormat =
					"POSITIONAL_LIGHT_PROTO( Light%d );";

	const char* kDirectionalCallFormat =
					"DIRECTIONAL_LIGHT_CALL( Light%d );";
	const char* kPositionalCallFormat =
					"POSITIONAL_LIGHT_CALL( Light%d );";
					
	const char* kIsTexturedUniformName			= "isTextured";
	const char*	kTextureUnitUniformName			= "tex0";
	const char*	kIlluminationTypeUniformName	= "IlluminationType";
	
	const int	kMaxProgramAge					= 100;
	
	struct AgeIncrementer
	{
		void	operator()( QORenderer::ProgramRec& ioRec ) const
				{
					ioRec.mAgeCounter += 1;
				}
	};
	
	struct IsNotTooOld
	{
		bool	operator()( const QORenderer::ProgramRec& ioRec ) const
				{
					return ioRec.mAgeCounter <= kMaxProgramAge;
				}
	};
	
	struct MatchPattern
	{
					MatchPattern( const QORenderer::LightPattern& inPattern )
						: mPattern( inPattern ) {}
					
					MatchPattern( const MatchPattern& inOther )
						: mPattern( inOther.mPattern ) {}
	
		bool		operator()( const QORenderer::ProgramRec& inProg ) const
					{
						return mPattern == inProg.mPattern;
					}
		
	private:
		const QORenderer::LightPattern&	mPattern;
	};
	
	struct DeleteProgram
	{
								DeleteProgram( QORenderer::GLSLFuncs& inFuncs )
									: mFuncs( inFuncs ) {}
								
								DeleteProgram( const DeleteProgram& inOther )
									: mFuncs( inOther.mFuncs ) {}
								
		void					operator()( QORenderer::ProgramRec& ioProgram ) const
								{
									if (ioProgram.mProgram != 0)
									{
										mFuncs.glDeleteProgram( ioProgram.mProgram );
										ioProgram.mProgram = 0;
									}
								}	
							
		QORenderer::GLSLFuncs&	mFuncs;
	};
	
	struct DeleteShader
	{
								DeleteShader( QORenderer::GLSLFuncs& inFuncs )
									: mFuncs( inFuncs ) {}
								
								DeleteShader( const DeleteProgram& inOther )
									: mFuncs( inOther.mFuncs ) {}
								
		void					operator()( GLuint inShader ) const
								{
									if (inShader != 0)
									{
										mFuncs.glDeleteShader( inShader );
									}
								}	
							
		QORenderer::GLSLFuncs&	mFuncs;
	};
}





//=============================================================================
//      Implementations
//-----------------------------------------------------------------------------
QORenderer::GLSLFuncs::GLSLFuncs()
{
	SetNULL();
}

void	QORenderer::GLSLFuncs::SetNULL()
{
	glCreateShader = NULL;
	glShaderSource = NULL;
	glCompileShader = NULL;
	glGetShaderiv = NULL;
	glCreateProgram = NULL;
	glAttachShader = NULL;
	glDetachShader = NULL;
	glLinkProgram = NULL;
	glGetProgramiv = NULL;
	glUseProgram = NULL;
	glGetUniformLocation = NULL;
	glUniform1i = NULL;
	glDeleteShader = NULL;
	glDeleteProgram = NULL;
	glGetProgramInfoLog = NULL;
	glGetShaderInfoLog = NULL;
}

void	QORenderer::GLSLFuncs::Initialize( const TQ3GLExtensions& inExts )
{
	if (inExts.shadingLanguage == kQ3True)
	{
		GLGetProcAddress( glCreateShader, "glCreateShader", "glCreateShaderObjectARB" );
		GLGetProcAddress( glShaderSource, "glShaderSource", "glShaderSourceARB" );
		GLGetProcAddress( glCompileShader, "glCompileShader", "glCompileShaderARB" );
		GLGetProcAddress( glGetShaderiv, "glGetShaderiv", "glGetObjectParameterivARB" );
		GLGetProcAddress( glCreateProgram, "glCreateProgram", "glCreateProgramObjectARB" );
		GLGetProcAddress( glAttachShader, "glAttachShader", "glAttachObjectARB" );
		GLGetProcAddress( glDetachShader, "glDetachShader", "glDetachObjectARB" );
		GLGetProcAddress( glLinkProgram, "glLinkProgram", "glLinkProgramARB" );
		GLGetProcAddress( glGetProgramiv, "glGetProgramiv", "glGetObjectParameterivARB" );
		GLGetProcAddress( glUseProgram, "glUseProgram", "glUseProgramObjectARB" );
		GLGetProcAddress( glGetUniformLocation, "glGetUniformLocation", "glGetUniformLocationARB" );
		GLGetProcAddress( glUniform1i, "glUniform1i", "glUniform1iARB" );
		GLGetProcAddress( glDeleteShader, "glDeleteShader", "glDeleteObjectARB" );
		GLGetProcAddress( glDeleteProgram, "glDeleteProgram", "glDeleteObjectARB" );
		GLGetProcAddress( glGetProgramInfoLog, "glGetProgramInfoLog", "glGetInfoLogARB" );
		GLGetProcAddress( glGetShaderInfoLog, "glGetShaderInfoLog", "glGetInfoLogARB" );
	#if Q3_DEBUG
		if ( (glCreateShader == NULL) or
			(glShaderSource == NULL) or
			(glCompileShader == NULL) or
			(glGetShaderiv == NULL) or
			(glCreateProgram == NULL) or
			(glAttachShader == NULL) or
			(glDetachShader == NULL) or
			(glLinkProgram == NULL) or
			(glGetProgramiv == NULL) or
			(glUseProgram == NULL) or
			(glGetUniformLocation == NULL) or
			(glUniform1i == NULL) or
			(glDeleteShader == NULL) or
			(glDeleteProgram == NULL) or
			(glGetProgramInfoLog == NULL) or
			(glGetShaderInfoLog == NULL) )
		{
			Q3_MESSAGE( "Shading functions NOT all present.\n" );
		}
	#endif
	}
	else
	{
		SetNULL();
	}
}

#pragma mark -

QORenderer::PerPixelLighting::PerPixelLighting(
										GLSLFuncs& inFuncs,
										TQ3RendererObject inRendererObject )
	: mFuncs( inFuncs )
	, mRendererObject( inRendererObject )
	, mIsShading( false )
	, mIlluminationType( 0 )
	, mIsTextured( false )
	, mVertexShaderID( 0 )
{
}

QORenderer::PerPixelLighting::~PerPixelLighting()
{
	Cleanup();
}

static void AddDirectionalPrototype( GLint inLightIndex,
									std::vector<std::string>& ioProtos )
{
	char	buffer[100];
	snprintf( buffer, sizeof(buffer), kDirectionalProtoFormat,
		(int)inLightIndex );
	ioProtos.push_back( std::string(buffer) );
}

static void AddPositionalPrototype( GLint inLightIndex,
									std::vector<std::string>& ioProtos )
{
	char	buffer[100];
	snprintf( buffer, sizeof(buffer), kPositionalProtoFormat,
		(int)inLightIndex );
	ioProtos.push_back( std::string(buffer) );
}

static void AddDirectionalCall( GLint inLightIndex,
								std::vector<std::string>& ioCalls )
{
	char	buffer[100];
	snprintf( buffer, sizeof(buffer), kDirectionalCallFormat,
		(int)inLightIndex );
	ioCalls.push_back( std::string(buffer) );
}

static void AddPositionalCall( GLint inLightIndex,
								std::vector<std::string>& ioCalls )
{
	char	buffer[100];
	snprintf( buffer, sizeof(buffer), kPositionalCallFormat,
		(int)inLightIndex );
	ioCalls.push_back( std::string(buffer) );
}



#if Q3_DEBUG
static void LogShaderCompileError( GLint inShaderID, QORenderer::GLSLFuncs& inFuncs )
{
	GLint	logSize = 0;
	inFuncs.glGetShaderiv( inShaderID, GL_INFO_LOG_LENGTH, &logSize );
	if (logSize > 0)
	{
		GLbyte*	theLog = (GLbyte*) Q3Memory_Allocate( logSize );
		if (theLog != NULL)
		{
			inFuncs.glGetShaderInfoLog( inShaderID, logSize, NULL, theLog );
			Q3_MESSAGE( (char*)theLog );
			Q3_MESSAGE( "\n" );
			Q3Memory_Free( &theLog );
		}
	}
}
#else
	#define		LogShaderCompileError( x, y )
#endif



static GLint CreateMainFragmentShader( const std::vector<std::string>& inLightProtos,
										const std::vector<std::string>& inLightCalls,
										QORenderer::GLSLFuncs& inFuncs )
{
	GLint shaderID = inFuncs.glCreateShader( GL_FRAGMENT_SHADER );
	if (shaderID != 0)
	{
		std::vector<const char*>	sourceParts;
		
		sourceParts.push_back( kMainFragmentShaderPart1Source );
		
		std::vector<std::string>::const_iterator	j;
		for (j = inLightProtos.begin(); j != inLightProtos.end(); ++j)
		{
			sourceParts.push_back( j->c_str() );
		}
		
		sourceParts.push_back( kMainFragmentShaderPart2Source );
		
		for (j = inLightCalls.begin(); j != inLightCalls.end(); ++j)
		{
			sourceParts.push_back( j->c_str() );
		}
		
		sourceParts.push_back( kMainFragmentShaderPart3Source );
		
		// Supply source code
		inFuncs.glShaderSource( shaderID, sourceParts.size(), &sourceParts[0], NULL );

		// Compile fragment shader
		inFuncs.glCompileShader( shaderID );
		
		// Check for compile success
		GLint	status;
		inFuncs.glGetShaderiv( shaderID, GL_COMPILE_STATUS, &status );
		Q3_ASSERT( status == GL_TRUE );
		
		if (status == GL_FALSE)
		{
			Q3_MESSAGE( "Failed to compile main fragment shader.\n" );
			LogShaderCompileError( shaderID, inFuncs );
			inFuncs.glDeleteShader( shaderID );
			shaderID = 0;
			E3ErrorManager_PostWarning( kQ3WarningFragmentShaderCompileFailed );
		}
	}
	else
	{
		Q3_MESSAGE( "Failed to create fragment shader.\n" );
	}
	
	return shaderID;
}



static void GetLightTypes( QORenderer::LightPattern& outLights )
{
	outLights.clear();
	
	// Query number of lights.
	GLint		maxGLLights;
	glGetIntegerv( GL_MAX_LIGHTS, &maxGLLights );
	outLights.reserve( maxGLLights );
	
	for (GLint i = 0; i < maxGLLights; ++i)
	{
		QORenderer::ELightType	theType = QORenderer::kLightTypeNone;
		
		GLenum	lightID = GL_LIGHT0 + i;
		
		if (glIsEnabled( lightID ))
		{
			// Is this a positional (point/spot) or directional light?
			// We can tell from the w component of the position.
			GLfloat	lightPosition[4];
			glGetLightfv( lightID, GL_POSITION, lightPosition );
			
			if (lightPosition[3] == 0.0f)	// directional
			{
				theType = QORenderer::kLightTypeDirectional;
			}
			else
			{
				theType = QORenderer::kLightTypePositional;
			}
		}
		outLights.push_back( theType );
	}
}


/*!
	@function	IlluminationTypeToCode
	@abstract	Convert a Quesa illumination shader type to an illumination
				code for the fragment shader.
*/
static GLint IlluminationTypeToCode( TQ3ObjectType inIlluminationType )
{
	GLint	illuminationCode = 0;
	switch (inIlluminationType)
	{
		case kQ3IlluminationTypePhong:
			illuminationCode = 2;
			break;
		
		case kQ3IlluminationTypeLambert:
			illuminationCode = 1;
			break;
			
		default:
			illuminationCode = 0;
			break;
	}
	
	return illuminationCode;
}


/*!
	@function	StartFrame
	@abstract	Begin a rendering frame.
*/
void	QORenderer::PerPixelLighting::StartFrame()
{
	CheckIfShading();
	
	if (mIsShading)
	{
		// Increment ages
		std::for_each( mPrograms.begin(), mPrograms.end(), AgeIncrementer() );
		
		// Delete programs that have not been used for a while
		ProgramVec::iterator	newEnd = std::partition( mPrograms.begin(),
			mPrograms.end(), IsNotTooOld() );
		if (newEnd != mPrograms.end())
		{
			std::for_each( newEnd, mPrograms.end(), DeleteProgram( mFuncs ) );
			mPrograms.erase( newEnd, mPrograms.end() );
		}
	}
}

/*!
	@function	StartPass
	@abstract	Begin a rendering pass.
	@discussion	This is where we check whether per-pixel lighting is
				desired.  If so, we set up the shaders and program.
*/
void	QORenderer::PerPixelLighting::StartPass()
{
	CheckIfShading();
	
	if (mIsShading)
	{
		InitVertexShader();
		
		mIlluminationType = 0;
		mIsTextured = false;
		mProgramIndex = -1;
		
		if (mVertexShaderID != 0)
		{
			// See if we have a program matching the current light pattern.
			LightPattern	theLightPattern;
			GetLightTypes( theLightPattern );
			
			MatchPattern	matcher( theLightPattern );
			ProgramVec::iterator	foundProg = std::find_if( mPrograms.begin(),
				mPrograms.end(), matcher );
			
			if (foundProg == mPrograms.end())
			{
				InitProgram( theLightPattern );
				
				foundProg = std::find_if( mPrograms.begin(),
					mPrograms.end(), matcher );
			}
			
			if (foundProg != mPrograms.end())
			{
				mProgramIndex = foundProg - mPrograms.begin();
				
				mFuncs.glUseProgram( foundProg->mProgram );
				foundProg->mAgeCounter = 0;
				
				// Make sure uniform variables start out correct
				mFuncs.glUniform1i( mPrograms[mProgramIndex].mIsTexturedUniformLoc,
					mIsTextured );
				mFuncs.glUniform1i( mPrograms[mProgramIndex].mIlluminationTypeUniformLoc,
					IlluminationTypeToCode( mIlluminationType ) );
			}
		}
	}
}

/*!
	@function	EndPass
	@abstract	Finish a rendering pass.
*/
void	QORenderer::PerPixelLighting::EndPass()
{
	if ( mIsShading )
	{
		mFuncs.glUseProgram( 0 );
	}
}


void	QORenderer::PerPixelLighting::InitUniforms( ProgramRec& ioProgram )
{
	ioProgram.mIsTexturedUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kIsTexturedUniformName );
	ioProgram.mTextureUnitUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kTextureUnitUniformName );
	ioProgram.mIlluminationTypeUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kIlluminationTypeUniformName );
}

/*!
	@function	CheckIfShading
	@abstract	Check whether we are doing per-pixel lighting on this pass.
*/
void	QORenderer::PerPixelLighting::CheckIfShading()
{
	TQ3Boolean	propValue;
	TQ3Status	propStatus = Q3Object_GetProperty( mRendererObject,
		kQ3RendererPropertyPerPixelLighting, sizeof(propValue), NULL,
		&propValue );
	
	mIsShading = (mFuncs.glCreateShader != NULL) &&
		(propStatus == kQ3Success) &&
		(propValue == kQ3True);
}


/*!
	@function	InitVertexShader
	@abstract	Set up the vertex shader, if it has not already
				been done.
*/
void	QORenderer::PerPixelLighting::InitVertexShader()
{
	if (mVertexShaderID == 0)
	{
		mVertexShaderID = mFuncs.glCreateShader( GL_VERTEX_SHADER );
		
		if (mVertexShaderID != 0)
		{
			// Supply source code
			mFuncs.glShaderSource( mVertexShaderID, 1, &kVertexShaderSource, NULL );
			
			// Compile vertex shader
			mFuncs.glCompileShader( mVertexShaderID );
			
			// Check for compile success
			GLint	status;
			mFuncs.glGetShaderiv( mVertexShaderID, GL_COMPILE_STATUS, &status );
			Q3_ASSERT( status == GL_TRUE );
			
			if (status == GL_FALSE)
			{
				Q3_MESSAGE( "Failed to compile a vertex shader.\n" );
				LogShaderCompileError( mVertexShaderID, mFuncs );
				mFuncs.glDeleteShader( mVertexShaderID );
				mVertexShaderID = 0;
				E3ErrorManager_PostWarning( kQ3WarningVertexShaderCompileFailed );
			}
		}
		else
		{
			Q3_MESSAGE( "Failed to create a vertex shader.\n" );
		}
	}
}

/*!
	@function	InitProgram
	@abstract	Set up the main fragment shader and program.
*/
void	QORenderer::PerPixelLighting::InitProgram( const LightPattern& inPattern )
{
	ProgramRec	newProgram;
	newProgram.mPattern = inPattern;
	
	// Create a program.
	newProgram.mProgram = mFuncs.glCreateProgram();
			
	if (newProgram.mProgram != 0)
	{
		// Attach the vertex shader to the program.
		mFuncs.glAttachShader( newProgram.mProgram, mVertexShaderID );
		
		std::vector<std::string>	lightProtos, lightCalls;
		
		const GLint kNumLights = inPattern.size();
	
		for (GLint i = 0; i < kNumLights; ++i)
		{
			GLenum	lightID = GL_LIGHT0 + i;
			
			switch (inPattern[i])
			{
				case kLightTypeDirectional:
					AttachDirectionalShader( i, newProgram.mProgram );
					AddDirectionalPrototype( i, lightProtos );
					AddDirectionalCall( i, lightCalls );
					break;
				
				case kLightTypePositional:
					AttachPositionalShader( i, newProgram.mProgram );
					AddPositionalPrototype( i, lightProtos );
					AddPositionalCall( i, lightCalls );
					break;
			}
		}
		
		// create and attach main fragment shader
		GLint shaderID = CreateMainFragmentShader( lightProtos, lightCalls, mFuncs );
		if (shaderID != 0)
		{
			// Attach
			mFuncs.glAttachShader( newProgram.mProgram, shaderID );
			
			// Delete, so it will go away when detached
			mFuncs.glDeleteShader( shaderID );
		}
		
		// Link program
		mFuncs.glLinkProgram( newProgram.mProgram );
		
		// Check for link success
		GLint	linkStatus;
		mFuncs.glGetProgramiv( newProgram.mProgram, GL_LINK_STATUS, &linkStatus );
		Q3_ASSERT( linkStatus == GL_TRUE );
		
		// Use program
		if (linkStatus == GL_TRUE)
		{
			InitUniforms( newProgram );
			
			mPrograms.push_back( newProgram );
		}
		else
		{
			E3ErrorManager_PostWarning( kQ3WarningShaderProgramLinkFailed );
			
		#if Q3_DEBUG
			GLint	logSize = 0;
			mFuncs.glGetProgramiv( newProgram.mProgram, GL_INFO_LOG_LENGTH, &logSize );
			if (logSize > 0)
			{
				GLbyte*	theLog = (GLbyte*) Q3Memory_Allocate( logSize );
				if (theLog != NULL)
				{
					mFuncs.glGetProgramInfoLog( newProgram.mProgram,
						logSize, NULL, theLog );
					Q3_MESSAGE( "Failed to link program.  Error log:\n" );
					Q3_MESSAGE( (char*)theLog );
					Q3_MESSAGE( "\n" );
					Q3Memory_Free( &theLog );
				}
			}
		#endif

			mFuncs.glDeleteProgram( newProgram.mProgram );
		}
	}
	else
	{
		Q3_MESSAGE( "Failed to create program.\n" );
	}
}


void	QORenderer::PerPixelLighting::AttachDirectionalShader(
								GLint inLightIndex,
								GLuint inProgram )
{
	if (inLightIndex >= mDirectionalLightShaders.size())
	{
		mDirectionalLightShaders.resize( inLightIndex + 1, 0 );
	}
	
	// If we haven't already, create the shader for this light
	GLuint	shaderID = mDirectionalLightShaders[ inLightIndex ];
	if (shaderID == 0)
	{
		char	buffer[100];
		snprintf( buffer, sizeof(buffer), kLightShaderPrefixFormat,
			(int)inLightIndex, (int)inLightIndex );
		
		shaderID = mFuncs.glCreateShader( GL_FRAGMENT_SHADER );
		if (shaderID != 0)
		{
			// Supply source code
			const char*	sourceParts[] =
			{
				buffer,
				kDirectionalLightFragmentShaderSource
			};
			mFuncs.glShaderSource( shaderID, 2, sourceParts, NULL );
			
			// Compile fragment shader
			mFuncs.glCompileShader( shaderID );
			
			// Check for compile success
			GLint	status;
			mFuncs.glGetShaderiv( shaderID, GL_COMPILE_STATUS, &status );
			if (status == GL_TRUE)
			{
				mDirectionalLightShaders[ inLightIndex ] = shaderID;
			}
			else
			{
				Q3_MESSAGE( "Failed to compile directional light fragment shader.\n" );
				LogShaderCompileError( shaderID, mFuncs );
				E3ErrorManager_PostWarning( kQ3WarningFragmentShaderCompileFailed );
				mFuncs.glDeleteShader( shaderID );
				shaderID = 0;
			}
		}
	}
	
	if (shaderID != 0)
	{
		mFuncs.glAttachShader( inProgram, shaderID );
	}
}


void	QORenderer::PerPixelLighting::AttachPositionalShader(
									GLint inLightIndex,
									GLuint inProgram )
{
	if (inLightIndex >= mPositionalLightShaders.size())
	{
		mPositionalLightShaders.resize( inLightIndex + 1, 0 );
	}
	
	// If we haven't already, create the shader for this light
	GLuint	shaderID = mPositionalLightShaders[ inLightIndex ];
	if (shaderID == 0)
	{
		char	buffer[100];
		snprintf( buffer, sizeof(buffer), kLightShaderPrefixFormat,
			(int)inLightIndex, (int)inLightIndex );
		
		shaderID = mFuncs.glCreateShader( GL_FRAGMENT_SHADER );
		if (shaderID != 0)
		{
			// Supply source code
			const char*	sourceParts[] =
			{
				buffer,
				kPositionalLightFragmentShaderSource
			};
			mFuncs.glShaderSource( shaderID, 2, sourceParts, NULL );
			
			// Compile fragment shader
			mFuncs.glCompileShader( shaderID );
			
			// Check for compile success
			GLint	status;
			mFuncs.glGetShaderiv( shaderID, GL_COMPILE_STATUS, &status );
			if (status == GL_TRUE)
			{
				mPositionalLightShaders[ inLightIndex ] = shaderID;
			}
			else
			{
				Q3_MESSAGE( "Failed to compile positional light fragment shader.\n" );
				LogShaderCompileError( shaderID, mFuncs );
				E3ErrorManager_PostWarning( kQ3WarningFragmentShaderCompileFailed );
				mFuncs.glDeleteShader( shaderID );
				shaderID = 0;
			}
		}
	}
	
	if (shaderID != 0)
	{
		mFuncs.glAttachShader( inProgram, shaderID );
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
	if (mIsShading)
	{
		// delete fragment shaders
		DeleteShader	deleter( mFuncs );

		std::for_each( mDirectionalLightShaders.begin(),
			mDirectionalLightShaders.end(), deleter );
		mDirectionalLightShaders.clear();

		std::for_each( mPositionalLightShaders.begin(),
			mPositionalLightShaders.end(), deleter );
		mPositionalLightShaders.clear();
		
		std::for_each( mPrograms.begin(), mPrograms.end(), DeleteProgram( mFuncs ) );
		mPrograms.clear();
		
		mFuncs.glDeleteShader( mVertexShaderID );
		mVertexShaderID = 0;
	}
}


/*!
	@function	UpdateIllumination
	@abstract	Notification that the type of illumination shader may
				have changed.
*/
void	QORenderer::PerPixelLighting::UpdateIllumination( TQ3ObjectType inIlluminationType )
{
	if (mIsShading && (mProgramIndex >= 0))
	{
		if (mIlluminationType != inIlluminationType)
		{
			mIlluminationType = inIlluminationType;
			
			GLint	illuminationCode = IlluminationTypeToCode( mIlluminationType );
			
			mFuncs.glUniform1i( mPrograms[mProgramIndex].mIlluminationTypeUniformLoc,
				illuminationCode );
		}
	}
}

/*!
	@function	UpdateTexture
	@abstract	Notification that there has been a change in whether we
				are texturing or which texture is active.
*/
void	QORenderer::PerPixelLighting::UpdateTexture()
{
	if (mIsShading && (mProgramIndex >= 0))
	{
		bool	isTextured = glIsEnabled( GL_TEXTURE_2D );
		
		if (isTextured != mIsTextured)
		{
			mIsTextured = isTextured;
			
			mFuncs.glUniform1i( mPrograms[mProgramIndex].mIsTexturedUniformLoc,
				mIsTextured );
		}
	}
}


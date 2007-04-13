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

#include <cstring>
#include <string>



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
					"ECPos3 = vec3( ECPosition ) / ECPosition.w;"

					"gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"

					"gl_FrontColor = gl_Color;"
					"gl_BackColor = gl_Color;"
					"gl_FrontSecondaryColor = gl_SecondaryColor;"
					"gl_BackSecondaryColor = gl_SecondaryColor;"

					"gl_Position = ftransform();"
				"}";
	
	const char* kDirectionalLightFragmentShaderSource =
				"void FUNC_NAME("
				"				in vec3 normal,"
				"				inout vec4 diffuse,"
				"				inout vec4 specular )"
				"{"
				"	float nDotVP = max( 0.0, dot( normal,"
				"		normalize(vec3(gl_LightSource[LIGHT_INDEX].position)) ) );"
				"	float nDotHV = max( 0.0, "
				"		dot( normal, vec3(gl_LightSource[LIGHT_INDEX].halfVector) ) );"
				"	float pf = 0.0;"
				""
				"	if (nDotVP != 0.0)"
				"	{"
				"		pf = pow( nDotHV, gl_FrontMaterial.shininess );"
				"	}"

				"	diffuse += gl_LightSource[LIGHT_INDEX].diffuse * nDotVP;"
				"	specular += gl_LightSource[LIGHT_INDEX].specular * pf;"
				"}";

	const char* kPositionalLightFragmentShaderSource =
				"void FUNC_NAME("
				"				in vec3 eye,			// geometry to eye direction\n"
				"				in vec3 ecPosition3,	// geometry position\n"
				"				in vec3 normal,"
				"				inout vec4 diffuse,"
				"				inout vec4 specular )"
				"{"
					// Compute vector from surface to light position
				"	vec3 geomToLight = vec3(gl_LightSource[LIGHT_INDEX].position) - ecPosition3;"

					// Compute distance between geometry and light
				"	float d = length(geomToLight);"

					// Normalize direction from geometry to light
				"	geomToLight /= d;"

					// Compute attenuation factor
				"	float attenuation = 1.0 / (gl_LightSource[LIGHT_INDEX].constantAttenuation +"
				"		gl_LightSource[LIGHT_INDEX].linearAttenuation * d +"
				"		gl_LightSource[LIGHT_INDEX].quadraticAttenuation * d * d );"
				
					// By testing whether this is a spot light, we can make this
					// function work for point lights as well.
				"	if (gl_LightSource[LIGHT_INDEX].spotCosCutoff != -1.0)"
				"	{"
						// See if point on surface is inside cone of illumination,
						// and maybe attenuate by angle from center of spot.
						// Quesa never sets GL_SPOT_EXPONENT, meaning it has the
						// default value of 0, and we need not look at
						// gl_LightSource[LIGHT_INDEX].spotExponent.
				"		float spotDot = dot( -geomToLight,"
				"			normalize(gl_LightSource[LIGHT_INDEX].spotDirection) );"
					
				"		if (spotDot < gl_LightSource[LIGHT_INDEX].spotCosCutoff)"
				"			attenuation = 0.0;		// light contributes nothing\n"
				"	}"

					// Compute the direction halfway between the geometry to light vector
					// and the geometry to eye vectors.  This uses the assumption that
					// eye and geomToLight are both normalized.
				"	vec3 halfVector = normalize( geomToLight + eye );"

				"	float nDotGeomToLight = max( 0.0, dot( normal, geomToLight ) );"

				"	diffuse += gl_LightSource[LIGHT_INDEX].diffuse * nDotGeomToLight * attenuation;"

				"	float nDotHalf = max( 0.0, dot( normal, halfVector ) );"

				"	float pf;"
				"	if (nDotGeomToLight == 0.0)"
				"		pf = 0.0;"
				"	else"
				"		pf = pow( nDotHalf, gl_FrontMaterial.shininess );"

				"	specular += gl_LightSource[LIGHT_INDEX].specular * pf * attenuation;"
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
				"				inout vec4 diffuse,\\\n"
				"				inout vec4 specular )\n"

				"#define DIRECTIONAL_LIGHT_CALL(name)	name( normal, diff, spec )\n"

				"#define	POSITIONAL_LIGHT_PROTO(name) 	\\\n"
				"	void name(					\\\n"
				"				in vec3 geomToEyeDir,	\\\n"
				"				in vec3 geomPos,	\\\n"
				"				in vec3 normal,	\\\n"
				"				inout vec4 diffuse,\\\n"
				"				inout vec4 specular )\n"

				"#define	POSITIONAL_LIGHT_CALL(name)	\\\n"
						"name( geomToEyeDir, geomPos, normal, diff, spec )\n";
		
		// Between part 1 and part 2, we will insert some light shader prototypes.

	const char* kMainFragmentShaderPart2Source =
				"void main()"
				"{"
					// Color components, lights will add to these.
				"	vec4		diff = vec4(0.0);"
				"	vec4		spec = vec4(0.0);"

					// Eye coordinate normal vector.  Even if the vertex normals were normalized
					// and the modelview matrix has no scaling, we would still have to normalize
					// here due to interpolation.
				"	vec3		normal = normalize(ECNormal);"

				"	vec3		geomPos = ECPos3;"
				"	vec3		geomToEyeDir = - normalize( geomPos );"

					// Flip the normal for the back face.  If we are using backfacing style
					// Remove, then back face triangles will not get here, so this does no harm.
				"	normal = faceforward( normal, geomPos, normal );";
		
		// Between part 2 and part 3, we will insert some light shader calls.

	const char* kMainFragmentShaderPart3Source =
				"	vec4	color;"

				"	if (IlluminationType == 0)"
				"	{"
				"		color = gl_Color + gl_FrontMaterial.emission;"
				"	}"
				"	else"
				"	{"
						// Start with emissive and global ambient color.
						// I will assume that the only ambient light is global.
				"		color = gl_FrontLightModelProduct.sceneColor;"

						// Add diffuse color.
				"		color += diff * gl_Color;"
				"	}"
				
					// The trouble with adding colors is that adding alpha is
					// not what we want.
					"color.a = gl_Color.a;"

					// Texturing, GL_MODULATE mode
				"	if (isTextured)"
				"			color *= texture2D( tex0, gl_TexCoord[0].st );"

				"	if (IlluminationType == 2)"
				"	{"
						// Add specular color.  This is done after texturing, as with
						// separate specular color.
				"		color += spec * gl_FrontMaterial.specular;"
				"	}"
				
				" 	gl_FragColor = color;"
				"}";
	
	const char* kLightShaderPrefixFormat =
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
}

void	QORenderer::GLSLFuncs::Initialize( const TQ3GLExtensions& inExts )
{
	if (inExts.shadingLanguage == kQ3True)
	{
		GLGetProcAddress( glCreateShader, "glCreateShader", "glCreateShaderARB" );
		GLGetProcAddress( glShaderSource, "glShaderSource", "glShaderSourceARB" );
		GLGetProcAddress( glCompileShader, "glCompileShader", "glCompileShaderARB" );
		GLGetProcAddress( glGetShaderiv, "glGetShaderiv", "glGetShaderivARB" );
		GLGetProcAddress( glCreateProgram, "glCreateProgram", "glCreateProgramARB" );
		GLGetProcAddress( glAttachShader, "glAttachShader", "glAttachShaderARB" );
		GLGetProcAddress( glDetachShader, "glDetachShader", "glDetachShaderARB" );
		GLGetProcAddress( glLinkProgram, "glLinkProgram", "glLinkProgramARB" );
		GLGetProcAddress( glGetProgramiv, "glGetProgramiv", "glGetProgramivARB" );
		GLGetProcAddress( glUseProgram, "glUseProgram", "glUseProgramARB" );
		GLGetProcAddress( glGetUniformLocation, "glGetUniformLocation", "glGetUniformLocationARB" );
		GLGetProcAddress( glUniform1i, "glUniform1i", "glUniform1iARB" );
		GLGetProcAddress( glDeleteShader, "glDeleteShader", "glDeleteShaderARB" );
		GLGetProcAddress( glDeleteProgram, "glDeleteProgram", "glDeleteProgramARB" );
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
	, mProgram( 0 )
{
	mLightTypes.push_back( kLightTypeInvalid );
}

QORenderer::PerPixelLighting::~PerPixelLighting()
{
	Cleanup();
}

static void AddDirectionalPrototype( GLint inLightIndex,
									std::vector<std::string>& ioProtos )
{
	char	buffer[100];
	std::snprintf( buffer, sizeof(buffer), kDirectionalProtoFormat,
		(int)inLightIndex );
	ioProtos.push_back( std::string(buffer) );
}

static void AddPositionalPrototype( GLint inLightIndex,
									std::vector<std::string>& ioProtos )
{
	char	buffer[100];
	std::snprintf( buffer, sizeof(buffer), kPositionalProtoFormat,
		(int)inLightIndex );
	ioProtos.push_back( std::string(buffer) );
}

static void AddDirectionalCall( GLint inLightIndex,
								std::vector<std::string>& ioCalls )
{
	char	buffer[100];
	std::snprintf( buffer, sizeof(buffer), kDirectionalCallFormat,
		(int)inLightIndex );
	ioCalls.push_back( std::string(buffer) );
}

static void AddPositionalCall( GLint inLightIndex,
								std::vector<std::string>& ioCalls )
{
	char	buffer[100];
	std::snprintf( buffer, sizeof(buffer), kPositionalCallFormat,
		(int)inLightIndex );
	ioCalls.push_back( std::string(buffer) );
}

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
			inFuncs.glDeleteShader( shaderID );
			shaderID = 0;
		}
	}
	
	return shaderID;
}

static void GetLightTypes( std::vector<QORenderer::ELightType>& outLights )
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
		InitProgram();
		
		mIlluminationType = 0;
		mIsTextured = false;
		
		if (mProgram != 0)
		{
			std::vector<ELightType>	theLightTypes;
			GetLightTypes( theLightTypes );
			const GLint kNumLights = theLightTypes.size();
			
			if (theLightTypes != mLightTypes)
			{
				mLightTypes.swap( theLightTypes );
				
				DetachFragmentShaders();
				
				std::vector<std::string>	lightProtos, lightCalls;
				
				for (GLint i = 0; i < kNumLights; ++i)
				{
					GLenum	lightID = GL_LIGHT0 + i;
					
					switch (mLightTypes[i])
					{
						case kLightTypeDirectional:
							AttachDirectionalShader( i );
							AddDirectionalPrototype( i, lightProtos );
							AddDirectionalCall( i, lightCalls );
							break;
						
						case kLightTypePositional:
							AttachPositionalShader( i );
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
					mFuncs.glAttachShader( mProgram, shaderID );
					mAttachedFragmentShaders.push_back( shaderID );
					
					// Delete, so it will go away when detached
					mFuncs.glDeleteShader( shaderID );
				}
				
				// Link program
				mFuncs.glLinkProgram( mProgram );
				
				// Check for link success
				GLint	linkStatus;
				mFuncs.glGetProgramiv( mProgram, GL_LINK_STATUS, &linkStatus );
				Q3_ASSERT( linkStatus == GL_TRUE );
				
				// Use program
				if (linkStatus == GL_TRUE)
				{
					InitUniforms();
				}
			}
			
			mFuncs.glUseProgram( mProgram );
		}
	}
}

/*!
	@function	EndPass
	@abstract	Finish a rendering pass.
*/
void	QORenderer::PerPixelLighting::EndPass()
{
	if ( mIsShading and (mProgram != 0) )
	{
		mFuncs.glUseProgram( 0 );
	}
}

void	QORenderer::PerPixelLighting::DetachFragmentShaders()
{
	for (std::vector<GLuint>::iterator i = mAttachedFragmentShaders.begin();
		i != mAttachedFragmentShaders.end(); ++i)
	{
		mFuncs.glDetachShader( mProgram, *i );
	}
	mAttachedFragmentShaders.clear();
}

void	QORenderer::PerPixelLighting::InitUniforms()
{
	mIsTexturedUniformLoc = mFuncs.glGetUniformLocation( mProgram,
		kIsTexturedUniformName );
	mTextureUnitUniformLoc = mFuncs.glGetUniformLocation( mProgram,
		kTextureUnitUniformName );
	mIlluminationTypeUniformLoc = mFuncs.glGetUniformLocation( mProgram,
		kIlluminationTypeUniformName );
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
	
	mIsShading = (mFuncs.glCreateShader != NULL) and
		(propStatus == kQ3Success) and
		(propValue == kQ3True);
}

/*!
	@function	InitProgram
	@abstract	Set up the vertex shader and program, if it has not already
				been done.
*/
void	QORenderer::PerPixelLighting::InitProgram()
{
	if (mProgram == 0)
	{
		// Make the vertex shader
		GLuint	shaderID = mFuncs.glCreateShader( GL_VERTEX_SHADER );
		if (shaderID != 0)
		{
			// Supply source code
			mFuncs.glShaderSource( shaderID, 1, &kVertexShaderSource, NULL );
			
			// Compile vertex shader
			mFuncs.glCompileShader( shaderID );
			
			// Check for compile success
			GLint	status;
			mFuncs.glGetShaderiv( shaderID, GL_COMPILE_STATUS, &status );
			
			if (status == GL_TRUE)
			{
				// Create a program.
				mProgram = mFuncs.glCreateProgram();
				
				if (mProgram != 0)
				{
					// Attach the vertex shader to the program.
					mFuncs.glAttachShader( mProgram, shaderID );
				}
			}

			// Delete the vertex shader.  If we have attached the shader to a
			// program, the shader will not really be deleted until the program
			// is deleted.
			mFuncs.glDeleteShader( shaderID );
		}
	}
}


void	QORenderer::PerPixelLighting::AttachDirectionalShader( GLint inLightIndex )
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
		std::snprintf( buffer, sizeof(buffer), kLightShaderPrefixFormat,
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
			Q3_ASSERT( status == GL_TRUE );

			mDirectionalLightShaders[ inLightIndex ] = shaderID;
		}
	}
	
	if (shaderID != 0)
	{
		mFuncs.glAttachShader( mProgram, shaderID );
		mAttachedFragmentShaders.push_back( shaderID );
	}
}


void	QORenderer::PerPixelLighting::AttachPositionalShader( GLint inLightIndex )
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
		std::snprintf( buffer, sizeof(buffer), kLightShaderPrefixFormat,
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
			Q3_ASSERT( status == GL_TRUE );

			mPositionalLightShaders[ inLightIndex ] = shaderID;
		}
	}
	
	if (shaderID != 0)
	{
		mFuncs.glAttachShader( mProgram, shaderID );
		mAttachedFragmentShaders.push_back( shaderID );
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
	if (mProgram != 0)
	{
		// delete fragment shaders
		std::vector<GLuint>::iterator i;
		for (i = mDirectionalLightShaders.begin();
			i != mDirectionalLightShaders.end(); ++i)
		{
			if (*i != 0)
			{
				mFuncs.glDeleteShader( *i );
			}
		}
		mDirectionalLightShaders.clear();
		for (i = mPositionalLightShaders.begin();
			i != mPositionalLightShaders.end(); ++i)
		{
			if (*i != 0)
			{
				mFuncs.glDeleteShader( *i );
			}
		}
		
		mPositionalLightShaders.clear();
		mAttachedFragmentShaders.clear();
		mLightTypes.clear();
		mLightTypes.push_back( kLightTypeInvalid );
		
		mFuncs.glDeleteProgram( mProgram );
		mProgram = 0;
	}
}


/*!
	@function	UpdateIllumination
	@abstract	Notification that the type of illumination shader may
				have changed.
*/
void	QORenderer::PerPixelLighting::UpdateIllumination( TQ3ObjectType inIlluminationType )
{
	if (mIsShading)
	{
		if (mIlluminationType != inIlluminationType)
		{
			mIlluminationType = inIlluminationType;
			
			GLint	illuminationCode = 0;
			switch (mIlluminationType)
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
			
			mFuncs.glUniform1i( mIlluminationTypeUniformLoc, illuminationCode );
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
	if (mIsShading)
	{
		bool	isTextured = glIsEnabled( GL_TEXTURE_2D );
		
		if (isTextured != mIsTextured)
		{
			mIsTextured = isTextured;
			
			mFuncs.glUniform1i( mIsTexturedUniformLoc, mIsTextured );
		}
	}
}


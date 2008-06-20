/*  NAME:
       QOGLShadingLanguage.cpp

    DESCRIPTION:
        Shading language functions for Quesa OpenGL renderer class.
		    
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
				"varying vec3 ECNormal;\n"
				""
				// Position in eye coordinates
				"varying vec3 ECPos3;\n"

				"void main()\n"
				"{\n"
					// Transform normal to eye coordinates, and let it
					// be interpolated across the primitive.
					// There is no point normalizing yet.
				"	ECNormal = gl_NormalMatrix * gl_Normal;\n"

				// Transform vertex position to eye coordinates.
				"	vec4 ECPosition = gl_ModelViewMatrix * gl_Vertex;\n"

				// Convert to 3 dimensions.
				"	ECPos3 = ECPosition.xyz / ECPosition.w;\n"

				"	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n"

				"	gl_FrontColor = gl_Color;\n"

				"	gl_Position = ftransform();\n"
				"}\n";
	
	const char*	kFragmentShaderPrefix =
				// Normal vector in eye coordinates
				"varying vec3 ECNormal;\n"

				// Position in eye coordinates
				"varying vec3 ECPos3;\n"
				
				// Light quantization parameter
				"uniform float quantization;\n"
				
				// Cartoon edges parameter
				"uniform float lightNearEdge;\n"

				// Sampler for texture unit 0
				"uniform sampler2D tex0;\n\n"
				;
	
	const char*	kFragmentShaderQuantizeFuncs_Normal =
				"vec3 QuantizeLight( in vec3 light )\n"
				"{\n"
				"	return light;\n"
				"}\n\n"
				
				"float QuantizeDot( in float normalDotGeomToLight )\n"
				"{\n"
				"	return normalDotGeomToLight;\n"
				"}\n\n"
				
				"vec3 QuantizeDiffuse( in vec3 light, in float normalDotGeomToLight )\n"
				"{\n"
				"	return light * normalDotGeomToLight;\n"
				"}\n\n"
				;
	
	const char*	kFragmentShaderQuantizeFuncs_Cartoonish =
				"vec3 QuantizeLight( in vec3 light )\n"
				"{\n"
				"	return floor( light * quantization + vec3(0.5) ) / quantization;\n"
				"}\n\n"
				
				"float QuantizeDot( in float normalDotGeomToLight )\n"
				"{\n"
				"	return floor( 2.0 * normalDotGeomToLight );\n"
				"}\n\n"
				
				"vec3 QuantizeDiffuse( in vec3 light, in float normalDotGeomToLight )\n"
				"{\n"
				"	vec3	result;\n"
				"	result = mix( QuantizeLight( light * normalDotGeomToLight ), light, 0.3 );\n"
				"	return result;\n"
				"}\n\n"
				;
	
	const char* kDirectionalLightFragmentShaderSource =
				// This shader code computes the contribution of one directional
				// light.  Copies of this code, with LIGHT_INDEX replaced by a
				// specific integer, will be included inline.
				// input: vec3 normal
				// inout: vec3 diff, vec3 spec
				"// Directional light, light LIGHT_INDEX\n"
				"{\n"
				"	float nDotVP = max( 0.0, dot( normal,"
				"		gl_LightSource[LIGHT_INDEX].position.xyz ) );\n"
				"	float nDotHV = max( 0.0, \n"
				"		dot( normal, gl_LightSource[LIGHT_INDEX].halfVector.xyz ) );\n"
				"	float pf = 0.0;\n\n"
				""
				"	if (nDotVP > 0.0)\n"
				"	{\n"
				"		pf = pow( nDotHV, gl_FrontMaterial.shininess );\n"
				"	}\n\n"

				"	diff += QuantizeDiffuse( gl_LightSource[LIGHT_INDEX].diffuse.rgb, nDotVP );\n"
				"	spec += QuantizeLight(gl_LightSource[LIGHT_INDEX].diffuse.rgb * pf);\n"
				"}\n\n";

	const char* kPositionalLightFragmentShaderSource =
				// This shader code computes the contribution of one positional
				// light.  Copies of this code, with LIGHT_INDEX replaced by a
				// specific integer, will be included inline.
				// input: vec3 geomToEyeDir, vec3 normal
				// inout: vec3 diff, vec3 spec
				"// Positional light, light LIGHT_INDEX\n"
				"{\n"
					// Compute vector from surface to light position
				"	vec3 geomToLight = gl_LightSource[LIGHT_INDEX].position.xyz - ECPos3;\n"

					// Compute distance between geometry and light
				"	float d = length(geomToLight);\n"

					// Normalize direction from geometry to light
				"	geomToLight /= d;\n"

					// Compute attenuation factor
				"	float attenuation = 1.0 / "
				"		(gl_LightSource[LIGHT_INDEX].constantAttenuation +"
				"		gl_LightSource[LIGHT_INDEX].linearAttenuation * d +"
				"		gl_LightSource[LIGHT_INDEX].quadraticAttenuation * d * d );\n"
				
					// See if point on surface is inside cone of illumination,
					// and maybe attenuate by angle from center of spot.
					// Quesa never sets GL_SPOT_EXPONENT, meaning it has the
					// default value of 0, and we need not look at
					// gl_LightSource[LIGHT_INDEX].spotExponent.
				"	float spotDot = dot( -geomToLight, gl_LightSource[LIGHT_INDEX].spotDirection );\n"
					
					// Set attenuation to 0 if outside the spot light cone.
					// Note that for a point light, spotCosCutoff will be -1,
					// and spotDot will never be less than that.
				"	attenuation *= step( gl_LightSource[LIGHT_INDEX].spotCosCutoff, spotDot );\n"

					// Compute the direction halfway between the geometry to light vector
					// and the geometry to eye vectors.  This uses the assumption that
					// geomToEyeDir and geomToLight are both normalized.
				"	vec3 halfVector = normalize( geomToLight + geomToEyeDir );\n"

				"	float nDotGeomToLight = max( 0.0, dot( normal, geomToLight ) );\n"
				"	nDotGeomToLight = QuantizeDot( nDotGeomToLight )\n;"

				"	diff += QuantizeDiffuse( gl_LightSource[LIGHT_INDEX].diffuse.rgb * \n"
				"				attenuation, nDotGeomToLight );\n"

				"	float nDotHalf = max( 0.0, dot( normal, halfVector ) );\n"

				"	float pf;\n"
				"	if (nDotGeomToLight == 0.0)\n"
				"		pf = 0.0;\n"
				"	else\n"
				"		pf = pow( nDotHalf, gl_FrontMaterial.shininess );\n\n"

				"	spec += QuantizeLight(gl_LightSource[LIGHT_INDEX].diffuse.rgb * pf * attenuation);\n"
				"}\n\n";

	const char* kMainFragmentShaderStartSource =
				"void main()\n"
				"{\n"
					// Color components of light, lights will add to these.
				"	vec3		diff = vec3(0.0);\n"
				"	vec3		spec = vec3(0.0);\n"
				
					// Color of fragment, to be determined later.
				"	vec3	color;\n"
				"	float	alpha;\n"

					// Eye coordinate normal vector.  Even if the vertex normals were normalized
					// and the modelview matrix has no scaling, we would still have to normalize
					// here due to interpolation.
				"	vec3		normal = normalize(ECNormal);\n"

				"	vec3		geomPos = ECPos3;\n"
				"	vec3		geomToEyeDir = - normalize( geomPos );\n"

					// Flip the normal for the back face.  If we are using
					// backfacing style Remove, then back face triangles will
					// not get here, so this does no harm except wasting a
					// little time.
				"	normal = faceforward( normal, geomPos, normal );\n\n";
		
		// Between part 1 and part 2, we will insert some light shader calls.

	const char* kColorCompForNULLIllumination =
				"	color = gl_Color.rgb + gl_FrontMaterial.emission.rgb;\n"
				"	alpha = gl_Color.a;\n"
				;

	const char* kColorCompForLambertAndPhong =
					// Start with emissive and global ambient color.
					// I will assume that the only ambient light is global.
				"	color = gl_LightModel.ambient.rgb * gl_Color.rgb + "
							"gl_FrontMaterial.emission.rgb;\n"

					// Add diffuse color.
				"	color += diff * gl_Color.rgb;\n"
				
				"	alpha = gl_Color.a;\n"
				;

	const char* kColorCompForLambertAndPhong_Cartoonish =
					// Start with emissive and global ambient color.
					// I will assume that the only ambient light is global.
				"	color = gl_LightModel.ambient.rgb * gl_Color.rgb + "
							"gl_FrontMaterial.emission.rgb;\n"

					// Add diffuse color.
					// Ordinarily we just add the diffuse light times the
					// material color, but when doing a cartoonish style, we
					// may darken areas nearly perpendicular to the eye.
				"	if (dot( normal, geomToEyeDir ) > 0.5)\n"
				"	{\n"
				"		color += min( diff, 1.0 ) * gl_Color.rgb;\n"
				"	}\n"
				"	else\n"
				"	{\n"
				"		color += lightNearEdge * min( diff, 1.0 ) * gl_Color.rgb;\n"
				"	}\n"
				
				"	alpha = gl_Color.a;\n"
				;

	const char* kTexturedColorComp =
					// Texturing, GL_MODULATE mode
				"	{\n"
				"		vec4 texColor = texture2D( tex0, gl_TexCoord[0].st );\n"
				"		color *= texColor.rgb;\n"
				"		alpha *= texColor.a;\n"
				"	}\n"
				;

	const char* kAddSpecularColor =
					// Add specular color.  This is done after texturing, as with
					// separate specular color.
				"	color += spec * gl_FrontMaterial.specular.rgb;\n"
				;

	const char* kMainFragmentShaderEndSource =
				" 	gl_FragColor.rgb = color;\n"
				"	gl_FragColor.a = alpha;\n"
				"}\n";
	
					
	const char*	kTextureUnitUniformName			= "tex0";
	const char*	kQuantizationUniformName		= "quantization";
	const char*	kLightNearEdgeUniformName		= "lightNearEdge";
	
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
	
	struct MatchProgram
	{
					MatchProgram(
							const QORenderer::LightPattern& inPattern,
							TQ3ObjectType inIlluminationType,
							bool inIsTextured,
							bool inIsCartoonish )
						: mPattern( inPattern )
						, mIlluminationType( inIlluminationType )
						, mIsTextured( inIsTextured )
						, mIsCartoonish( inIsCartoonish ) {}
					
					MatchProgram( const MatchProgram& inOther )
						: mPattern( inOther.mPattern )
						, mIlluminationType( inOther.mIlluminationType )
						, mIsTextured( inOther.mIsTextured )
						, mIsCartoonish( inOther.mIsCartoonish ) {}
	
		bool		operator()( const QORenderer::ProgramRec& inProg ) const
					{
						return (mIsTextured == inProg.mIsTextured) &&
							(mIlluminationType == inProg.mIlluminationType) &&
							(mIsCartoonish == inProg.mIsCartoonish) &&
							(mPattern == inProg.mPattern);
					}
		
	private:
		const QORenderer::LightPattern&	mPattern;
		TQ3ObjectType	mIlluminationType;
		bool			mIsTextured;
		bool			mIsCartoonish;
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
	
	GLenum	sGLError = 0;
} // end of unnamed namespace

#define		CHECK_GL_ERROR	Q3_ASSERT( (sGLError = glGetError()) == GL_NO_ERROR )



//=============================================================================
//      Implementations
//-----------------------------------------------------------------------------
void	QORenderer::ProgramRec::swap( ProgramRec& ioOther )
{
	std::swap( mProgram, ioOther.mProgram );
	std::swap( mAgeCounter, ioOther.mAgeCounter );
	mPattern.swap( ioOther.mPattern );
	std::swap( mIlluminationType, ioOther.mIlluminationType );
	std::swap( mIsTextured, ioOther.mIsTextured );
	std::swap( mIsCartoonish, ioOther.mIsCartoonish );
	std::swap( mTextureUnitUniformLoc, ioOther.mTextureUnitUniformLoc );
	std::swap( mQuantizationUniformLoc, ioOther.mQuantizationUniformLoc );
	std::swap( mLightNearEdgeUniformLoc, ioOther.mLightNearEdgeUniformLoc );
}

QORenderer::ProgramRec&
QORenderer::ProgramRec::operator=( const QORenderer::ProgramRec& inOther )
{
	ProgramRec	temp( inOther );
	swap( temp );
	return *this;
}

static void ReplaceAllSubstr( std::string& ioString,
								const char* inFindSub,
								const char* inReplacement )
{
	std::string::size_type	place;
	std::string::size_type	len = std::strlen( inFindSub );
	
	while ( (place = ioString.find( inFindSub )) != std::string::npos )
	{
		ioString.replace( place, len, inReplacement );
	}
}


static void ReplaceAllSubstrByInt( std::string& ioString,
								const char* inFindSub,
								unsigned int inReplacement )
{
	char	intAsStr[10];
	std::sprintf( intAsStr, "%d", inReplacement );
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
	glUniform1f = NULL;
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
		GLGetProcAddress( glUniform1f, "glUniform1f", "glUniform1fARB" );
		GLGetProcAddress( glDeleteShader, "glDeleteShader", "glDeleteObjectARB" );
		GLGetProcAddress( glDeleteProgram, "glDeleteProgram", "glDeleteObjectARB" );
		GLGetProcAddress( glGetProgramInfoLog, "glGetProgramInfoLog", "glGetInfoLogARB" );
		GLGetProcAddress( glGetShaderInfoLog, "glGetShaderInfoLog", "glGetInfoLogARB" );
		if ( (glCreateShader == NULL) ||
			(glShaderSource == NULL) ||
			(glCompileShader == NULL) ||
			(glGetShaderiv == NULL) ||
			(glCreateProgram == NULL) ||
			(glAttachShader == NULL) ||
			(glDetachShader == NULL) ||
			(glLinkProgram == NULL) ||
			(glGetProgramiv == NULL) ||
			(glUseProgram == NULL) ||
			(glGetUniformLocation == NULL) ||
			(glUniform1i == NULL) ||
			(glUniform1f == NULL) ||
			(glDeleteShader == NULL) ||
			(glDeleteProgram == NULL) ||
			(glGetProgramInfoLog == NULL) ||
			(glGetShaderInfoLog == NULL) )
		{
			Q3_MESSAGE( "Shading functions NOT all present.\n" );
			SetNULL();
		}
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
	, mQuantization( 0.0f )
	, mLightNearEdge( 1.0f )
	, mIsCartoonish( false )
	, mProgramIndex( -1 )
{
}

QORenderer::PerPixelLighting::~PerPixelLighting()
{
	Cleanup();
}


static void AddDirectionalShaderSource(	GLint inLightIndex,
										std::vector<std::string>& ioSource )
{
	std::string		theSource( kDirectionalLightFragmentShaderSource );
	ReplaceAllSubstrByInt( theSource, "LIGHT_INDEX", inLightIndex );
	ioSource.push_back( theSource );
}

static void AddPositionalShaderSource(	GLint inLightIndex,
										std::vector<std::string>& ioSource )
{
	std::string		theSource( kPositionalLightFragmentShaderSource );
	ReplaceAllSubstrByInt( theSource, "LIGHT_INDEX", inLightIndex );
	ioSource.push_back( theSource );
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



static void BuildFragmentShaderSource(	const QORenderer::LightPattern& inPattern,
										TQ3ObjectType inIlluminationType,
										bool inIsTextured,
										bool inIsCartoonish,
										std::vector<std::string>& outSource )
{
	outSource.push_back( kFragmentShaderPrefix );
	
	if (inIsCartoonish)
	{
		outSource.push_back( kFragmentShaderQuantizeFuncs_Cartoonish );
	}
	else
	{
		outSource.push_back( kFragmentShaderQuantizeFuncs_Normal );
	}
	
	outSource.push_back( kMainFragmentShaderStartSource );

	if (inIlluminationType != kQ3IlluminationTypeNULL)
	{
		const GLint kNumLights = inPattern.size();

		for (GLint i = 0; i < kNumLights; ++i)
		{
			switch (inPattern[i])
			{
				case QORenderer::kLightTypeDirectional:
					AddDirectionalShaderSource( i, outSource );
					break;
					
				case QORenderer::kLightTypePositional:
					AddPositionalShaderSource( i, outSource );
					break;
			}
		}
	}
	
	if (inIlluminationType == kQ3IlluminationTypeNULL)
	{
		outSource.push_back( kColorCompForNULLIllumination );
	}
	else if (inIsCartoonish)
	{
		outSource.push_back( kColorCompForLambertAndPhong_Cartoonish );
	}
	else
	{
		outSource.push_back( kColorCompForLambertAndPhong );
	}
	
	if (inIsTextured)
	{
		outSource.push_back( kTexturedColorComp );
	}
	
	if (inIlluminationType == kQ3IlluminationTypePhong)
	{
		outSource.push_back( kAddSpecularColor );
	}
		
	outSource.push_back( kMainFragmentShaderEndSource );
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
		mIlluminationType = kQ3IlluminationTypeNULL;
		mIsTextured = false;
		mProgramIndex = -1;
		mIsCartoonish = (mQuantization > 0.0f);
		
		InitVertexShader();
		
		if (mVertexShaderID != 0)
		{
			ChooseProgram();
		}
	}
}


/*!
	@function	ChooseProgram
	
	@abstract	Look for a program that matches the current light pattern,
				illumination, and texturing, creating one if need be, and
				activate it.
*/
void	QORenderer::PerPixelLighting::ChooseProgram()
{
	// See if we have a program matching the current light pattern.
	LightPattern	theLightPattern;
	GetLightTypes( theLightPattern );
	
	// Look for a program that meets current needs.
	MatchProgram	matcher( theLightPattern, mIlluminationType, mIsTextured,
		mIsCartoonish );
	ProgramVec::iterator	foundProg = std::find_if( mPrograms.begin(),
		mPrograms.end(), matcher );
	
	// If there is none, create it.
	if (foundProg == mPrograms.end())
	{
		InitProgram( theLightPattern );
		
		foundProg = std::find_if( mPrograms.begin(),
			mPrograms.end(), matcher );
	}
	
	// Activate it.
	if (foundProg != mPrograms.end())
	{
		mProgramIndex = foundProg - mPrograms.begin();
		
		mFuncs.glUseProgram( foundProg->mProgram );
		CHECK_GL_ERROR;
		foundProg->mAgeCounter = 0;
		
		// Set the quantization uniform variables.
		mFuncs.glUniform1f( foundProg->mQuantizationUniformLoc, mQuantization );
		CHECK_GL_ERROR;
		mFuncs.glUniform1f( foundProg->mLightNearEdgeUniformLoc, mLightNearEdge );
		CHECK_GL_ERROR;
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
	ioProgram.mTextureUnitUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kTextureUnitUniformName );
	CHECK_GL_ERROR;
	ioProgram.mQuantizationUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kQuantizationUniformName );
	CHECK_GL_ERROR;
	ioProgram.mLightNearEdgeUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kLightNearEdgeUniformName );
	CHECK_GL_ERROR;
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
	
	if (mIsShading)
	{
		mQuantization = 0.0f;	// default of no quantization
		Q3Object_GetProperty( mRendererObject,
			kQ3RendererPropertyQuantizePerPixelLight, sizeof(mQuantization), NULL,
			&mQuantization );
		
		mLightNearEdge = 1.0f;	// default, no darkening of edges
		Q3Object_GetProperty( mRendererObject,
			kQ3RendererPropertyCartoonLightNearEdge, sizeof(TQ3Float32), NULL,
			&mLightNearEdge );
	}
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
		CHECK_GL_ERROR;
		
		if (mVertexShaderID != 0)
		{
			// Supply source code
			mFuncs.glShaderSource( mVertexShaderID, 1, &kVertexShaderSource, NULL );
			CHECK_GL_ERROR;
			
			// Compile vertex shader
			mFuncs.glCompileShader( mVertexShaderID );
			CHECK_GL_ERROR;
			
			// Check for compile success
			GLint	status;
			mFuncs.glGetShaderiv( mVertexShaderID, GL_COMPILE_STATUS, &status );
			Q3_ASSERT( status == GL_TRUE );
			CHECK_GL_ERROR;
			
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

static void GetSourcePointers(	const std::vector<std::string>& inSrcStrings,
								std::vector<const char*>& outSrcPtrs )
{
	for (std::vector<std::string>::const_iterator i = inSrcStrings.begin();
		i != inSrcStrings.end(); ++i)
	{
		outSrcPtrs.push_back( i->c_str() );
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
	newProgram.mIlluminationType = mIlluminationType;
	newProgram.mIsTextured = mIsTextured;
	newProgram.mIsCartoonish = mIsCartoonish;
	
	// Create a program.
	newProgram.mProgram = mFuncs.glCreateProgram();
	CHECK_GL_ERROR;
	
	if (newProgram.mProgram != 0)
	{
		// Attach the vertex shader to the program.
		mFuncs.glAttachShader( newProgram.mProgram, mVertexShaderID );
		CHECK_GL_ERROR;
		
		// Build the source of the fragment shader
		std::vector<std::string>	fragSource;
		BuildFragmentShaderSource( inPattern, mIlluminationType, mIsTextured,
			mIsCartoonish, fragSource );
		std::vector<const char*>	sourceParts;
		GetSourcePointers( fragSource, sourceParts );
		
		// Create the fragment shader
		GLint shaderID = mFuncs.glCreateShader( GL_FRAGMENT_SHADER );
		CHECK_GL_ERROR;
		if (shaderID != 0)
		{
			// Supply source code
			mFuncs.glShaderSource( shaderID, sourceParts.size(), &sourceParts[0], NULL );
			CHECK_GL_ERROR;

			// Compile fragment shader
			mFuncs.glCompileShader( shaderID );
			CHECK_GL_ERROR;
			
			// Check for compile success
			GLint	status;
			mFuncs.glGetShaderiv( shaderID, GL_COMPILE_STATUS, &status );
			CHECK_GL_ERROR;
			Q3_ASSERT( status == GL_TRUE );
			
			if (status == GL_FALSE)
			{
				Q3_MESSAGE( "Failed to compile fragment shader.\n" );
				LogShaderCompileError( shaderID, mFuncs );
				mFuncs.glDeleteShader( shaderID );
				shaderID = 0;
				E3ErrorManager_PostWarning( kQ3WarningFragmentShaderCompileFailed );
			}
		}
		else
		{
			Q3_MESSAGE( "Failed to create fragment shader.\n" );
		}
		
		if (shaderID != 0)
		{
			// Attach
			mFuncs.glAttachShader( newProgram.mProgram, shaderID );
			CHECK_GL_ERROR;
			
			// Delete, so it will go away when detached
			mFuncs.glDeleteShader( shaderID );
			CHECK_GL_ERROR;
		}
		
		// Link program
		mFuncs.glLinkProgram( newProgram.mProgram );
		CHECK_GL_ERROR;
		
		// Check for link success
		GLint	linkStatus;
		mFuncs.glGetProgramiv( newProgram.mProgram, GL_LINK_STATUS, &linkStatus );
		Q3_ASSERT( linkStatus == GL_TRUE );
		CHECK_GL_ERROR;
		
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
			CHECK_GL_ERROR;
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
	if (mIsShading)
	{
		if (mIlluminationType != inIlluminationType)
		{
			mIlluminationType = inIlluminationType;
			
			ChooseProgram();
		}
	}
}

/*!
	@function	UpdateLighting
	@abstract	Notification that there has been a change in which OpenGL
				lights are on.
*/
void	QORenderer::PerPixelLighting::UpdateLighting()
{
	if (mIsShading)
	{
		ChooseProgram();
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
			
			ChooseProgram();
		}
	}
}

/*!
	@function	PreGeomSubmit
	@abstract	This is called just before a geometry will be rendered, in
				order to check for kQ3GeometryPropertyNonCartoon.
*/
void	QORenderer::PerPixelLighting::PreGeomSubmit( TQ3GeometryObject inGeom )
{
	if ( mIsShading && (inGeom != NULL) && (mQuantization > 0.0f) )
	{
		TQ3Boolean	isNonCartoon = kQ3False;
		
		Q3Object_GetProperty( inGeom, kQ3GeometryPropertyNonCartoon,
			sizeof(TQ3Boolean), NULL, &isNonCartoon );
		
		bool	isCartoonish = (isNonCartoon == kQ3False);
		
		if (isCartoonish != mIsCartoonish)
		{
			mIsCartoonish = isCartoonish;
			
			ChooseProgram();
					
			mFuncs.glUniform1f( mPrograms[ mProgramIndex ].mQuantizationUniformLoc,
				isCartoonish? mQuantization : 0.0f );
		}
	}
}



/*  NAME:
       QOGLShadingLanguage.cpp

    DESCRIPTION:
        Shading language functions for Quesa OpenGL renderer class.
		    
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

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QOGLShadingLanguage.h"
#include "GLUtils.h"
#include "E3Prefix.h"
#include "QuesaRenderer.h"
#include "QOTexture.h"
#include "QOShaderProgramCache.h"

#include <cstring>
#include <string>
#include <algorithm>
#include <cstdlib>

#ifndef GL_VERTEX_PROGRAM_TWO_SIDE
	#define GL_VERTEX_PROGRAM_TWO_SIDE        0x8643
#endif


//=============================================================================
//      Local constants
//-----------------------------------------------------------------------------

namespace
{
	const char* kVertexShaderSource =
				"#version 120\n"

				// Normal vector in eye coordinates
				"varying vec3 ECNormal;\n"
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
				"	gl_BackColor = gl_Color;\n"

				"	gl_Position = ftransform();\n"
				
				// Use secondary color to let the fragment shader tell back from
				// front.  See comments under kMainFragmentShaderStartSmooth.
				"	gl_FrontSecondaryColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
				"	gl_BackSecondaryColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
				"}\n";
	
	#pragma mark kFragmentShaderPrefix
	const char*	kFragmentShaderPrefix =
				"#version 120\n"

				// Normal vector in eye coordinates
				"varying vec3 ECNormal;\n"

				// Position in eye coordinates
				"varying vec3 ECPos3;\n"
				
				// Light quantization parameter
				"uniform float quantization;\n"
				
				// Cartoon edges parameter
				"uniform float lightNearEdge;\n"
				
				// Hot angles, cutoff angles for spot lights (in radians)
				"uniform float hotAngle[gl_MaxLights];\n"
				"uniform float cutoffAngle[gl_MaxLights];\n"
				
				// Specular map flag
				"uniform bool isUsingSpecularMap;\n"

				// Samplers for texture units
				"uniform sampler2D tex0;\n"
				"uniform sampler2D tex1;\n\n"
				;
	
	#pragma mark kFragmentShaderQuantizeFuncs_Normal
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
	
	#pragma mark kFragmentShaderQuantizeFuncs_Cartoonish
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
	
	#pragma mark kDirectionalLightFragmentShaderSource
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
				"	diff += QuantizeDiffuse( gl_LightSource[LIGHT_INDEX].diffuse.rgb, nDotVP );\n\n"
				
				"	float nDotHV = max( 0.0, \n"
				"		dot( normal, gl_LightSource[LIGHT_INDEX].halfVector.xyz ) );\n"
				"	float pf = (nDotVP > 0.0)? pow( nDotHV, specularExp ) : 0.0;\n"
				"	spec += QuantizeLight(gl_LightSource[LIGHT_INDEX].diffuse.rgb * pf);\n"
				"}\n\n";

	#pragma mark kSpotFalloffNoneSource
	const char* kSpotFalloffNoneSource =
				"// Spot light falloff function, none, for light LIGHT_INDEX\n"
				"float SpotFalloff_LIGHT_INDEX( in float x )\n"
				"{\n"
				"	return 1.0;\n"
				"}\n\n"
				;

	#pragma mark kSpotFalloffLinearSource
	const char* kSpotFalloffLinearSource =
				"// Spot light falloff function, linear, for light LIGHT_INDEX\n"
				"float SpotFalloff_LIGHT_INDEX( in float x )\n"
				"{\n"
				"	return 1.0 - x;\n"
				"}\n\n"
				;

	#pragma mark kSpotFalloffExponentialSource
	const char* kSpotFalloffExponentialSource =
				"// Spot light falloff function, exponential, for light LIGHT_INDEX\n"
				"float SpotFalloff_LIGHT_INDEX( in float x )\n"
				"{\n"
				"	return (pow( 10.0, 1.0 - x ) - 1.0) / 9.0;\n"
				"}\n\n"
				;

	#pragma mark kSpotFalloffCosineSource
	const char* kSpotFalloffCosineSource =
				"// Spot light falloff function, cosine, for light LIGHT_INDEX\n"
				"float SpotFalloff_LIGHT_INDEX( in float x )\n"
				"{\n"
				"	return cos( radians( 90.0 * x ) );\n"
				"}\n\n"
				;

	#pragma mark kSpotFalloffSmoothCubicSource
	const char* kSpotFalloffSmoothCubicSource =
				"// Spot light falloff function, smooth cubic, for light LIGHT_INDEX\n"
				"float SpotFalloff_LIGHT_INDEX( in float x )\n"
				"{\n"
				"	return 1.0 - smoothstep( 0.0, 1.0, x );\n"
				"}\n\n"
				;

	#pragma mark kPointLightFragmentShaderSource
	const char* kPointLightFragmentShaderSource =
				// This shader code computes the contribution of one point
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
				"		pf = pow( nDotHalf, specularExp );\n\n"

				"	spec += QuantizeLight(gl_LightSource[LIGHT_INDEX].diffuse.rgb * pf * attenuation);\n"
				"}\n\n";

	#pragma mark kSpotLightFragmentShaderSource
	const char* kSpotLightFragmentShaderSource =
				// This shader code computes the contribution of one spot
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
				
					// Find the angle between the vector from the light to this
					// fragment and the light direction vector.  The vectors are
					// unit vectors, so the dot product is the cosine of the angle.
					// Quesa never sets GL_SPOT_EXPONENT, meaning it has the
					// default value of 0, and we need not look at
					// gl_LightSource[LIGHT_INDEX].spotExponent.
				"	float spotDot = dot( -geomToLight, gl_LightSource[LIGHT_INDEX].spotDirection );\n"
				"	float spotAngle = acos( spotDot );\n"
				
					// Compute a falloff factor.
				"	float fallFrac = (spotAngle - hotAngle[LIGHT_INDEX]) /\n"
				"		(cutoffAngle[LIGHT_INDEX] - hotAngle[LIGHT_INDEX]);\n"
				"	float spotFalloff = (spotAngle < hotAngle[LIGHT_INDEX])?\n"
				"		1.0 :\n"
				"		((spotAngle > cutoffAngle[LIGHT_INDEX])?\n"
				"			0.0 :\n"
				"			SpotFalloff_LIGHT_INDEX( fallFrac ));\n"
					
					// See if point on surface is inside cone of illumination,
					// and maybe attenuate by angle from center of spot.
					// Set attenuation to 0 if outside the spot light cone.
				"	attenuation *= spotFalloff;\n"

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
				"		pf = pow( nDotHalf, specularExp );\n\n"

				"	spec += QuantizeLight(gl_LightSource[LIGHT_INDEX].diffuse.rgb * pf * attenuation);\n"
				"}\n\n";

	#pragma mark kMainFragmentShaderStart
	const char* kMainFragmentShaderStart =
				"void main()\n"
				"{\n"
					// Color components of light, lights will add to these.
				"	vec3		diff = vec3(0.0);\n"
				"	vec3		spec = vec3(0.0);\n"
				
					// Color of fragment, to be determined later.
				"	vec3	color;\n"
				"	float	alpha;\n"
				
					// Specular exponent
				"	float specularExp = isUsingSpecularMap?\n"
				"		texture2D( tex1, gl_TexCoord[0].st ).a * 128.0 :\n"
				"		gl_FrontMaterial.shininess;\n\n";

	#pragma mark kMainFragmentShaderStartSmooth
	const char* kMainFragmentShaderStartSmooth =
					// Eye coordinate normal vector.  Even if the vertex normals were normalized
					// and the modelview matrix has no scaling, we would still have to normalize
					// here due to interpolation.
				"	vec3		normal = normalize(ECNormal);\n"

				"	vec3		geomPos = ECPos3;\n"
				"	vec3		geomToEyeDir = - normalize( geomPos );\n"

					// Flip the normal for the back face.  If we are using
					// backfacing style Remove, then back face triangles will
					// not get here, in which case no harm is done.
					// The most obvious way to flip backfacing normals is
					//   normal = gl_FrontFacing? normal : -normal;
					// but unfortunately gl_FrontFacing is poorly supported on
					// some cards/drivers... it may simply fail, or it may cause
					// a fallback to software rendering.
					// Previously, we did this using
					//   normal = faceforward( normal, geomPos, normal );
					// but that had a problem:  Sometimes, especially in models
					// created by skeletal animation, the vertex normals may
					// point a little away from the camera even though the
					// triangle faces front.
					// Here we use a trick of using two-sided lighting to
					// distinguish back from front.
				"	normal = (gl_SecondaryColor.r > 0.0)? normal : -normal;\n\n"
				;
	
	#pragma mark kMainFragmentShaderStartFlat
	const char* kMainFragmentShaderStartFlat =
					// Face normal vector for flat shading.
					// If we could assume GLSL 1.30 or EXT_gpu_shader4, this
					// could be done in a less tricky way, using the "flat"
					// modifier for a varying variable.
				"	vec3	normal = normalize(cross(dFdx(ECPos3), dFdy(ECPos3)));\n"

				"	vec3		geomPos = ECPos3;\n"
				"	vec3		geomToEyeDir = - normalize( geomPos );\n"

					// Flip the normal for the back face.
				"	normal = (gl_SecondaryColor.r > 0.0)? normal : -normal;\n\n"
				;
		
		// Between part 1 and part 2, we will insert some light shader calls.

	#pragma mark kColorCompForNULLIllumination
	const char* kColorCompForNULLIllumination =
				"	color = gl_Color.rgb + gl_FrontMaterial.emission.rgb;\n"
				"	alpha = gl_Color.a;\n"
				;

	#pragma mark kColorCompForLambertAndPhong
	const char* kColorCompForLambertAndPhong =
					// Start with emissive and global ambient color.
					// I will assume that the only ambient light is global.
				"	color = gl_LightModel.ambient.rgb * gl_Color.rgb + "
				"         gl_FrontMaterial.emission.rgb;\n"

					// Add diffuse color.
				"	color += diff * gl_Color.rgb;\n"
				
				"	alpha = gl_Color.a;\n"
				;

	#pragma mark kColorCompForLambertAndPhong_Cartoonish
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

	#pragma mark kTexturedColorComp
	const char* kTexturedColorComp =
					// Texturing, GL_MODULATE mode
				"	{\n"
				"		vec4 texColor = texture2D( tex0, gl_TexCoord[0].st );\n"
				"		color *= texColor.rgb;\n"
				"		alpha *= texColor.a;\n"
				"	}\n"
				;
	
	#pragma mark kAddSpecularColor
	const char* kAddSpecularColor =
					// Add specular color.  This is done after texturing, as with
					// separate specular color.
				"	vec3 specMat = isUsingSpecularMap?\n"
				"		texture2D( tex1, gl_TexCoord[0].st ).rgb :\n"
				"		gl_FrontMaterial.specular.rgb;\n"
				"	color += spec * specMat;\n"
				;

	const char* kAddFogLinear =
				"	float dist = length( ECPos3 );\n"
				"	float fog = (gl_Fog.end - dist) * gl_Fog.scale;\n"
				"	fog = clamp( fog, 0.0, 1.0 );\n"
				"	color = mix( gl_Fog.color.rgb, color, fog );\n"
				;

	const char* kAddFogExp =
				"	float dist = length( ECPos3 );\n"
				"	float fog = exp( - gl_Fog.density * dist );\n"
				"	fog = clamp( fog, 0.0, 1.0 );\n"
				"	color = mix( gl_Fog.color.rgb, color, fog );\n"
				;

	const char* kAddFogExp2 =
				"	float dist = length( ECPos3 );\n"
				"	float fogProd = gl_Fog.density * dist;\n"
				"	float fog = exp( - fogProd * fogProd );\n"
				"	fog = clamp( fog, 0.0, 1.0 );\n"
				"	color = mix( gl_Fog.color.rgb, color, fog );\n"
				;

	#pragma mark kMainFragmentShaderEndSource
	const char* kMainFragmentShaderEndSource =
				" 	gl_FragColor.rgb = color;\n"
				"	gl_FragColor.a = alpha;\n"
				"}\n";
	
					
	const char*	kTextureUnit0UniformName		= "tex0";
	const char*	kTextureUnit1UniformName		= "tex1";
	const char* kSpecularMapFlagUniformName		= "isUsingSpecularMap";
	const char*	kQuantizationUniformName		= "quantization";
	const char*	kLightNearEdgeUniformName		= "lightNearEdge";
	const char* kSpotHotAngleUniformName		= "hotAngle";
	const char* kSpotCutoffAngleUniformName		= "cutoffAngle";
	
	GLenum	sGLError = 0;
} // end of unnamed namespace

#if Q3_DEBUG && !defined(Q3_DEBUG_GL_ERRORS)
	#define		Q3_DEBUG_GL_ERRORS		0
#endif

#if Q3_DEBUG_GL_ERRORS
	#define		CHECK_GL_ERROR	do {	\
									sGLError = glGetError();	\
									if (sGLError != GL_NO_ERROR)	\
									{	\
										char	xmsg[200];	\
										snprintf( xmsg, sizeof(xmsg),	\
											"glGetError() is %d", \
											(int)sGLError );	\
										E3Assert(__FILE__, __LINE__, xmsg);	\
									} \
								} while (false)
#else
	#define		CHECK_GL_ERROR
#endif

#pragma mark -
//=============================================================================
//      Implementations
//-----------------------------------------------------------------------------
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
								unsigned int inReplacement )
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
	glUniform1fv = NULL;
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
		GLGetProcAddress( glUniform1fv, "glUniform1fv", "glUniform1fvARB" );
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
			(glUniform1fv == NULL) ||
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
										const GLSLFuncs& inFuncs,
										TQ3RendererObject inRendererObject,
										TQ3GLContext& inGLContext,
										const TQ3GLExtensions& inExtensions )
	: mFuncs( inFuncs )
	, mGLExtensions( inExtensions )
	, mRendererObject( inRendererObject )
	, mGLContext( inGLContext )
	, mIsShading( false )
	, mMayNeedProgramChange( true )
	, mIsSpecularMapped( false )
	, mQuantization( 0.0f )
	, mLightNearEdge( 1.0f )
	, mCurrentProgram( NULL )
{
}

QORenderer::PerPixelLighting::~PerPixelLighting()
{
	Cleanup();
}


static void AddDirectionalShaderSource(	GLint inLightIndex,
										std::string& ioSource )
{
	std::string		theSource( kDirectionalLightFragmentShaderSource );
	ReplaceAllSubstrByInt( theSource, "LIGHT_INDEX", inLightIndex );
	ioSource += theSource;
}

static void AddPointLightShaderSource(	GLint inLightIndex,
										std::string& ioSource )
{
	std::string		theSource( kPointLightFragmentShaderSource );
	ReplaceAllSubstrByInt( theSource, "LIGHT_INDEX", inLightIndex );
	ioSource += theSource;
}

static void AddSpotLightShaderSource(	GLint inLightIndex,
										std::string& ioSource )
{
	std::string		theSource( kSpotLightFragmentShaderSource );
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
		if (theLog != NULL)
		{
			inFuncs.glGetShaderInfoLog( inShaderID, logSize, NULL, theLog );
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
}
#else
	#define		LogShaderCompileError( x, y )
#endif



static void BuildFragmentShaderSource(	const QORenderer::ProgramCharacteristic& inProgramRec,
										std::string& outSource )
{
	const GLint kNumLights = static_cast<GLint>(inProgramRec.mPattern.size());
	GLint i;
	
	outSource += kFragmentShaderPrefix;
	
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
					AddDirectionalShaderSource( i, outSource );
					break;
					
				case QORenderer::kLightTypePoint:
					AddPointLightShaderSource( i, outSource );
					break;
				
				case QORenderer::kLightTypeSpotNone:
				case QORenderer::kLightTypeSpotLinear:
				case QORenderer::kLightTypeSpotExponential:
				case QORenderer::kLightTypeSpotCosine:
				case QORenderer::kLightTypeSpotCubic:
					AddSpotLightShaderSource( i, outSource );
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
		outSource +=  kColorCompForLambertAndPhong;
	}
	
	if (inProgramRec.mIsTextured)
	{
		outSource += kTexturedColorComp;
	}
	
	if (inProgramRec.mIlluminationType == kQ3IlluminationTypePhong)
	{
		outSource += kAddSpecularColor;
	}
	
	if (inProgramRec.mFogState == kQ3On)
	{
		switch (inProgramRec.mFogMode)
		{
			case kQ3FogModeLinear:
				outSource += kAddFogLinear;
				break;
			
			case kQ3FogModeExponential:
				outSource += kAddFogExp;
				break;
			
			case kQ3FogModeExponentialSquared:
				outSource += kAddFogExp2;
				break;
			
			default:
				break;
		}
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
	
	if (mProgramCharacteristic.mIlluminationType != kQ3IlluminationTypeNULL)
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
		InitVertexShader();
		
		if (mGLExtensions.vertexProgramTwoSide)
		{
			glEnable( GL_VERTEX_PROGRAM_TWO_SIDE );
		}
	}
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
}

/*!
	@function	AddLight
	@abstract	The Lights object uses this to inform the PerPixelLighting
				object about a light being used for this pass.
*/
void	QORenderer::PerPixelLighting::AddLight( TQ3LightObject inLight )
{
	CQ3ObjectRef	lightRef( Q3Shared_GetReference( inLight ) );
	mLights.push_back( lightRef );
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
	@discussion	This is where we check whether per-pixel lighting is
				desired.  If so, we set up the shaders and program.
*/
void	QORenderer::PerPixelLighting::StartPass()
{
	CheckIfShading();
	
	if (mIsShading)
	{
		mProgramCharacteristic.mIlluminationType = kQ3IlluminationTypeNULL;
		mProgramCharacteristic.mIsTextured = false;
		mCurrentProgram = NULL;
		mMayNeedProgramChange = true;
		mProgramCharacteristic.mIsCartoonish = (mQuantization > 0.0f);
		
		if (ProgCache()->VertexShaderID() != 0)
		{
			GetLightTypes();
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
	if ( mMayNeedProgramChange &&
		(ProgCache()->VertexShaderID() != 0) )
	{
		mMayNeedProgramChange = false;

		// Look for a program that meets current needs.
		const ProgramRec* theProgram = ProgCache()->FindProgram( mProgramCharacteristic );
		
		// If there is none, create it.
		if (theProgram == NULL)
		{
			InitProgram();
			
			theProgram = ProgCache()->FindProgram( mProgramCharacteristic );
		}
		
		// Activate it.
		if (theProgram != NULL)
		{
			if (theProgram != mCurrentProgram)
			{
				mCurrentProgram = theProgram;
				mFuncs.glUseProgram( theProgram->mProgram );
				CHECK_GL_ERROR;
			
				SetUniformValues( *theProgram );
			}
			
			// Even if we didn't change the program, we need to update the
			// specular mapping uniform.
			mFuncs.glUniform1i( theProgram->mIsSpecularMappingUniformLoc, mIsSpecularMapped );
		}
	}
}

/*!
	@function	SetUniformValues
	@abstract	Set values for the uniform variables needed in the program.
*/
void	QORenderer::PerPixelLighting::SetUniformValues( const ProgramRec& ioProgram )
{
	// Set texture units.
	mFuncs.glUniform1i( ioProgram.mTextureUnit0UniformLoc, 0 );
	mFuncs.glUniform1i( ioProgram.mTextureUnit1UniformLoc, 1 );

	// Set the quantization uniform variables.
	mFuncs.glUniform1f( ioProgram.mQuantizationUniformLoc, mQuantization );
	CHECK_GL_ERROR;
	mFuncs.glUniform1f( ioProgram.mLightNearEdgeUniformLoc, mLightNearEdge );
	CHECK_GL_ERROR;
	
	// Retrieve hot angles, cutoff angles for any spot lights.
	const int kNumLights = static_cast<int>(mLights.size());
	std::vector<GLfloat>	hotAngles( kNumLights );
	std::vector<GLfloat>	cutoffAngles( kNumLights );
	for (int i = 0; i < kNumLights; ++i)
	{
		float theHotAngle = 0.0f;
		float theCutoffAngle = 0.0f;
		if (Q3Light_GetType( mLights[i].get() ) == kQ3LightTypeSpot)
		{
			Q3SpotLight_GetHotAngle( mLights[i].get(), &theHotAngle );
			Q3SpotLight_GetOuterAngle( mLights[i].get(), &theCutoffAngle );
		}
		hotAngles[ i ] = theHotAngle;
		cutoffAngles[ i ] = theCutoffAngle;
	}
	
	// Set hot angle, cutoff angle uniform arrays.
	if (kNumLights > 0)
	{
		mFuncs.glUniform1fv( ioProgram.mSpotHotAngleUniformLoc, kNumLights,
			&hotAngles[0] );
		mFuncs.glUniform1fv( ioProgram.mSpotCutoffAngleUniformLoc, kNumLights,
			&cutoffAngles[0] );
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
		mCurrentProgram = NULL;
	}
	mIsShading = false;
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
	if (shaderID != 0)
	{
		// Supply source code
		GLint sourceLen = (GLint) std::strlen( inSource );
		inFuncs.glShaderSource( shaderID, 1, &inSource, &sourceLen );
		
		// Compile the shader
		inFuncs.glCompileShader( shaderID );
		
		// Check for compile success
		GLint	status;
		inFuncs.glGetShaderiv( shaderID, GL_COMPILE_STATUS, &status );
		
		if (status == GL_FALSE)
		{
			Q3_MESSAGE( "Failed to compile GLSL shader.\n" );
			LogShaderCompileError( shaderID, inFuncs );
			
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
	if (ProgCache()->VertexShaderID() == 0)
	{
		GLuint vertexShader = CreateAndCompileShader( GL_VERTEX_SHADER,
			kVertexShaderSource, mFuncs );
		
		if (vertexShader == 0)
		{
			Q3_MESSAGE( "Failed to create a vertex shader.\n" );
		}
		else
		{
			ProgCache()->SetVertexShaderID( vertexShader );
		}
	}
}



/*!
	@function	InitProgram
	@abstract	Set up the main fragment shader and program.
*/
void	QORenderer::PerPixelLighting::InitProgram()
{
	ProgramRec	newProgram;
	
	newProgram.mCharacteristic = mProgramCharacteristic;
	
	// Build the source of the fragment shader
	std::string	fragSource;
	BuildFragmentShaderSource( newProgram.mCharacteristic, fragSource );
		
	// Create the fragment shader
	GLint shaderID = CreateAndCompileShader( GL_FRAGMENT_SHADER,
		fragSource.c_str(), mFuncs );

	if (shaderID != 0)
	{
		// Create a program.
		newProgram.mProgram = mFuncs.glCreateProgram();
		CHECK_GL_ERROR;
	
		if (newProgram.mProgram != 0)
		{
			GLuint vertexShader = ProgCache()->VertexShaderID();

			// Attach the vertex shader to the program.
			mFuncs.glAttachShader( newProgram.mProgram, vertexShader );
			CHECK_GL_ERROR;
		
			// Attach the fragment shader to the program
			mFuncs.glAttachShader( newProgram.mProgram, shaderID );
			CHECK_GL_ERROR;
			
			// Link program
			mFuncs.glLinkProgram( newProgram.mProgram );
			CHECK_GL_ERROR;
		
			// Detach shaders from program (whether or not link worked)
			mFuncs.glDetachShader( newProgram.mProgram, shaderID );
			mFuncs.glDetachShader( newProgram.mProgram, vertexShader );
		
			// Delete the fragment shader
			mFuncs.glDeleteShader( shaderID );
			CHECK_GL_ERROR;
		
			// Check for link success
			GLint	linkStatus;
			mFuncs.glGetProgramiv( newProgram.mProgram, GL_LINK_STATUS, &linkStatus );
			Q3_ASSERT( linkStatus == GL_TRUE );
			CHECK_GL_ERROR;
		
			// Use program
			if (linkStatus == GL_TRUE)
			{
				InitUniformLocations( newProgram );
			
				ProgCache()->AddProgram( newProgram );
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
	if (mIsShading)
	{
		if (mProgramCharacteristic.mIlluminationType != inIlluminationType)
		{
			mProgramCharacteristic.mIlluminationType = inIlluminationType;
			
			GetLightTypes();
			
			// An illumination change does not change the texture, but I want
			// client code to be able to disable texturing at the OpenGL level
			// and get Quesa to notice the change by changing illumination.
			UpdateTexture( glIsEnabled( GL_TEXTURE_2D ) != 0 );
			
			mMayNeedProgramChange = true;
		}
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
	if (mIsShading)
	{
		if (mProgramCharacteristic.mInterpolationStyle != inInterpolation)
		{
			mProgramCharacteristic.mInterpolationStyle = inInterpolation;
			
			mMayNeedProgramChange = true;
		}
	}
}

/*!
	@function	UpdateFogStyle
	@abstract	Notification that fog style has changed.
*/
void	QORenderer::PerPixelLighting::UpdateFogStyle( const TQ3FogStyleData& inFog )
{
	if (mIsShading)
	{
		if ( (inFog.state != mProgramCharacteristic.mFogState) ||
			(
				(mProgramCharacteristic.mFogState == kQ3On) &&
				(inFog.mode != mProgramCharacteristic.mFogMode)
			)
		)
		{
			mProgramCharacteristic.mFogState = inFog.state;
			mProgramCharacteristic.mFogMode = inFog.mode;
			
			mMayNeedProgramChange = true;
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
		GetLightTypes();
		mMayNeedProgramChange = true;
	}
}


/*!
	@function	UpdateTexture
	@abstract	Notification that there has been a change in whether we
				are texturing.
*/
void	QORenderer::PerPixelLighting::UpdateTexture( bool inTexturing  )
{
	if (mIsShading)
	{
		if (inTexturing != mProgramCharacteristic.mIsTextured)
		{
			mProgramCharacteristic.mIsTextured = inTexturing;
			
			mMayNeedProgramChange = true;
		}
	}
}

/*!
	@function	UpdateSpecularMapping
	@abstract	Notification that there has been a change in whether we are using
				a shininess (specular) map.
*/
void	QORenderer::PerPixelLighting::UpdateSpecularMapping( bool inSpecularMapped )
{
	if (mIsShading)
	{
		if (inSpecularMapped != mIsSpecularMapped)
		{
			mIsSpecularMapped = inSpecularMapped;
			mMayNeedProgramChange = true;
		}
	}
}


/*!
	@function	PreGeomSubmit
	@abstract	This is called just before a geometry will be rendered, to
				update the fragment shader program if necessary.  The
				geometry is passed, if available, so that cartoon parameters
				may be updated.
	@param		inGeom		Geometry being rendered.  May be NULL.
*/
void	QORenderer::PerPixelLighting::PreGeomSubmit( TQ3GeometryObject inGeom )
{
	if ( mIsShading )
	{
		bool cartoonUpdate = false;
		
		if ( (inGeom != NULL) && (mQuantization > 0.0f) )
		{
			TQ3Boolean	isNonCartoon = kQ3False;
			
			Q3Object_GetProperty( inGeom, kQ3GeometryPropertyNonCartoon,
				sizeof(TQ3Boolean), NULL, &isNonCartoon );
			
			bool	isCartoonish = (isNonCartoon == kQ3False);
			
			if (isCartoonish != mProgramCharacteristic.mIsCartoonish)
			{
				mProgramCharacteristic.mIsCartoonish = isCartoonish;
				mMayNeedProgramChange = true;
				cartoonUpdate = true;
			}
		}
		
		ChooseProgram();
		
		if (cartoonUpdate && (mCurrentProgram != NULL))
		{
			mFuncs.glUniform1f( mCurrentProgram->mQuantizationUniformLoc,
				mProgramCharacteristic.mIsCartoonish? mQuantization : 0.0f );
		}
	}
}



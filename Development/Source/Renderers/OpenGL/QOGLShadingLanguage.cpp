/*  NAME:
       QOGLShadingLanguage.cpp

    DESCRIPTION:
        Shading language functions for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2019, Quesa Developers. All rights reserved.

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
#include "CQ3ObjectRef_Gets.h"
#include "QuesaMathOperators.hpp"

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

static int sVertexShaderCount = 0;
static int sProgramCount = 0;

//=============================================================================
//      Local constants
//-----------------------------------------------------------------------------

namespace
{
	const TQ3Vector3D kZeroVec = { 0.0f, 0.0f, 0.0f };

	#pragma mark kVertexShaderStart
	const char* kVertexShaderStart =
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
				"	vec4 ECPosition = gl_ModelViewMatrix * gl_Vertex;\n";

	#pragma mark kVertexShaderClipVertex
	const char* kVertexShaderClipVertex =
				// On most video cards, if you want to use clipping planes,
				// you must set gl_ClipVertex.  But on ATI cards, doing so
				// causes a fallback to software rendering.
				"	gl_ClipVertex = ECPosition;\n";

	#pragma mark kVertexShaderEnd
	const char* kVertexShaderEnd =
				// Convert to 3 dimensions.
				"	ECPos3 = ECPosition.xyz / ECPosition.w;\n"

				"	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\n"

				"	gl_FrontColor = gl_Color;\n"
				"	gl_BackColor = gl_Color;\n"

				"	gl_Position = ftransform();\n"
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
				
				// Light data, to replace gl_LightSource
				"uniform vec4 lightPosition[gl_MaxLights];\n"
				"uniform vec4 lightColor[gl_MaxLights];\n"
				"uniform vec3 spotLightDirection[gl_MaxLights];\n"
				"uniform vec3 lightAttenuation[gl_MaxLights];\n"
				
				// Specular map flag
				"uniform bool isUsingSpecularMap;\n"
				
				// Flipping normals flag
				"uniform bool isFlippingNormals;\n"

				// Clipping plane
				"uniform vec4 ECClipPlane;\n"
				
				// Extended fog
				"uniform float maxFogOpacity;\n"
				"uniform float HSFogRate;\n"
				"uniform vec4 HSFogPlane;\n"

				// Samplers for texture units
				"uniform sampler2D tex0;\n"
				"uniform sampler2D tex1;\n\n"
				
				"float FogSmooth( in float x )\n"
				"{\n"
				"	float y = x / maxFogOpacity;\n"
				"	return x * ( y * (1.0 - y) + 1.0 );\n"
				"}\n\n"
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
				"	float nDotVP = max( 0.0, dot( normal,\n"
				"		lightPosition[LIGHT_INDEX].xyz ) );\n"
				"	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb, nDotVP );\n\n"
				
				"	vec3 geomToLight = normalize(lightPosition[LIGHT_INDEX].xyz);\n"
				"	vec3 halfVector = normalize( geomToLight + geomToEyeDir );\n"
				
				"	float nDotHV = max( 0.0, \n"
				"		dot( normal, halfVector ) );\n"
				"	float pf1 = (specularExp <= 0.0)? 1.0 : pow( nDotHV, specularExp );\n"
				"	float pf = (nDotVP > 0.0)? pf1 : 0.0;\n"
				"	spec += QuantizeLight(lightColor[LIGHT_INDEX].rgb * pf);\n"
				"}\n\n";

	#pragma mark kDirectionalLightWithNondirIllumFragmentShaderSource
	const char* kDirectionalLightWithNondirIllumFragmentShaderSource =
				"// Directional light, light LIGHT_INDEX\n"
				"{\n"
					"	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb, 1.0 );\n"
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
				"	vec3 geomToLight = lightPosition[LIGHT_INDEX].xyz - ECPos3;\n"

					// Compute distance between geometry and light
				"	float d = length(geomToLight);\n"

					// Normalize direction from geometry to light
				"	geomToLight /= d;\n"

					// Compute attenuation factor
				"	float attenuation = 1.0 / \n"
				"		(lightAttenuation[LIGHT_INDEX].x +\n"
				"		lightAttenuation[LIGHT_INDEX].y * d +\n"
				"		lightAttenuation[LIGHT_INDEX].z * d * d );\n"

					// Compute the direction halfway between the geometry to light vector
					// and the geometry to eye vectors.  This uses the assumption that
					// geomToEyeDir and geomToLight are both normalized.
				"	vec3 halfVector = normalize( geomToLight + geomToEyeDir );\n"

				"	float nDotGeomToLight = max( 0.0, dot( normal, geomToLight ) );\n"
				"	nDotGeomToLight = QuantizeDot( nDotGeomToLight );\n"

				"	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb * \n"
				"				attenuation, nDotGeomToLight );\n"

				"	float nDotHalf = max( 0.0, dot( normal, halfVector ) );\n"

				"	float pf;\n"
				"	if (nDotGeomToLight == 0.0)\n"
				"		pf = 0.0;\n"
				"	else\n"
				"		pf = (specularExp <= 0.0)? 1.0 : pow( nDotHalf, specularExp );\n\n"

				"	spec += QuantizeLight(lightColor[LIGHT_INDEX].rgb * pf * attenuation);\n"
				"}\n\n";

	#pragma mark kPointLightWithNondirIllumFragmentShaderSource
	const char* kPointLightWithNondirIllumFragmentShaderSource =
				"// Positional light, light LIGHT_INDEX\n"
				"{\n"
					// Compute vector from surface to light position
				"	vec3 geomToLight = lightPosition[LIGHT_INDEX].xyz - ECPos3;\n"

					// Compute distance between geometry and light
				"	float d = length(geomToLight);\n"

					// Compute attenuation factor
				"	float attenuation = 1.0 / \n"
				"		(lightAttenuation[LIGHT_INDEX].x +\n"
				"		lightAttenuation[LIGHT_INDEX].y * d +\n"
				"		lightAttenuation[LIGHT_INDEX].z * d * d );\n"

				"	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb * \n"
				"				attenuation, 1.0 );\n"
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
				"	vec3 geomToLight = lightPosition[LIGHT_INDEX].xyz - ECPos3;\n"

					// Compute distance between geometry and light
				"	float d = length(geomToLight);\n"

					// Normalize direction from geometry to light
				"	geomToLight /= d;\n"

					// Compute attenuation factor
				"	float attenuation = 1.0 / "
				"		(lightAttenuation[LIGHT_INDEX].x +"
				"		lightAttenuation[LIGHT_INDEX].y * d +"
				"		lightAttenuation[LIGHT_INDEX].z * d * d );\n"
				
					// Find the angle between the vector from the light to this
					// fragment and the light direction vector.  The vectors are
					// unit vectors, so the dot product is the cosine of the angle.
					// Quesa never sets GL_SPOT_EXPONENT, meaning it has the
					// default value of 0, and we need not look at
					// gl_LightSource[LIGHT_INDEX].spotExponent.
				"	float spotDot = dot( -geomToLight, spotLightDirection[LIGHT_INDEX] );\n"
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

				"	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb * \n"
				"				attenuation, nDotGeomToLight );\n"

				"	float nDotHalf = max( 0.0, dot( normal, halfVector ) );\n"

				"	float pf;\n"
				"	if (nDotGeomToLight == 0.0)\n"
				"		pf = 0.0;\n"
				"	else\n"
				"		pf = (specularExp <= 0.0)? 1.0 : pow( nDotHalf, specularExp );\n\n"

				"	spec += QuantizeLight(lightColor[LIGHT_INDEX].rgb * pf * attenuation);\n"
				"}\n\n";

	#pragma mark kSpotLightWithNondirIllumFragmentShaderSource
	const char* kSpotLightWithNondirIllumFragmentShaderSource =
				"// Positional light, light LIGHT_INDEX\n"
				"{\n"
					// Compute vector from surface to light position
				"	vec3 geomToLight = lightPosition[LIGHT_INDEX].xyz - ECPos3;\n"

					// Compute distance between geometry and light
				"	float d = length(geomToLight);\n"

					// Normalize direction from geometry to light
				"	geomToLight /= d;\n"

					// Compute attenuation factor
				"	float attenuation = 1.0 / "
				"		(lightAttenuation[LIGHT_INDEX].x +"
				"		lightAttenuation[LIGHT_INDEX].y * d +"
				"		lightAttenuation[LIGHT_INDEX].z * d * d );\n"
				
					// Find the angle between the vector from the light to this
					// fragment and the light direction vector.  The vectors are
					// unit vectors, so the dot product is the cosine of the angle.
					// Quesa never sets GL_SPOT_EXPONENT, meaning it has the
					// default value of 0, and we need not look at
					// gl_LightSource[LIGHT_INDEX].spotExponent.
				"	float spotDot = dot( -geomToLight, spotLightDirection[LIGHT_INDEX] );\n"
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

				"	diff += QuantizeDiffuse( lightColor[LIGHT_INDEX].rgb * \n"
				"				attenuation, 1.0 );\n"
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
				"		gl_FrontMaterial.shininess;\n\n"
				
					// Geometry position and direction to eye
				"	vec3		geomPos = ECPos3;\n"
				"	vec3		geomToEyeDir = - normalize( geomPos );\n"
				
					// Front face normal, used for backface flipping, flat shading
				"	vec3	faceNormal = cross(dFdx(ECPos3), dFdy(ECPos3));\n";

	#pragma mark kFragmentClipping
	const char* kFragmentClipping =
				"	if (dot( vec4( ECPos3, 1.0 ), ECClipPlane ) < 0.0)\n"
				"	{\n"
				"		discard;\n"
				"	}\n"
				;

	#pragma mark kMainFragmentShaderStartSmooth
	const char* kMainFragmentShaderStartSmooth =
					// Eye coordinate normal vector.  Even if the vertex normals were normalized
					// and the modelview matrix has no scaling, we would still have to normalize
					// here due to interpolation.
				"	vec3		normal = normalize(ECNormal);\n"

					// Flip the normal for the back face.  If we are using
					// backfacing style Remove, then back face triangles will
					// not get here, in which case no harm is done.
					// We have tried several methods, each with disadvantages.
					// 1. Obvious way:
					//		normal = gl_FrontFacing? normal : -normal;
					//	  Problem:  gl_FrontFacing is poorly supported on
					//	  some cards/drivers... it may simply fail, or it may cause
					//    a fallback to software rendering.
					// 2. Vertex normal way:
					//		normal = faceforward( normal, geomPos, normal );
					//    Sometimes, especially in models created by skeletal
					//    animation, the vertex normals may
					//    point a little away from the camera even though the
					//    triangle faces front.
					// 3. 2-sided lighting way:
					//		glEnable( GL_VERTEX_PROGRAM_TWO_SIDE );
					//    and in the vertex shader set gl_FrontSecondaryColor
					//    and gl_BackSecondaryColor differently, and in the
					//    fragment shader say
					//      normal = (gl_SecondaryColor.r > 0.0)? normal : -normal;
					//    Problem: on some ATI cards you may get a failure to
					//    compile shaders, intermittently.
					// 4. Calculus way:  Determine the front face normal as we
					//    had previously done for flat shading, and then say
					//      normal = faceforward( normal, normal, -faceNormal );
					//    That is, we flip the normal if its dot with faceNormal
					//    is negative.  This can occasionally produce incorrect
					//    results in weird cases where vertex normals are
					//    somewhat toward the back of the triangle.
				"	normal = isFlippingNormals? faceforward( normal, normal, -faceNormal )\n"
				"				: normal;\n\n"
				;
	
	#pragma mark kMainFragmentShaderStartFlat
	const char* kMainFragmentShaderStartFlat =
					// Face normal vector for flat shading.
					// If we could assume GLSL 1.30 or EXT_gpu_shader4, this
					// could be done in a less tricky way, using the "flat"
					// modifier for a varying variable.
				"	vec3	normal = normalize(faceNormal);\n"

					// In this case there is no need to flip for the backface,
					// because faceNormal always is toward the eye.
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
				"	color = gl_LightModel.ambient.rgb * gl_Color.rgb +\n"
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

	#pragma mark kCalcFogLinear, kCalcFogExp, kCalcFogExp2
	const char* kCalcFogLinear =
				"	float dist = length( ECPos3 );\n"
				"	float fog = (gl_Fog.end - dist) * gl_Fog.scale;\n"
				;

	const char* kCalcFogExp =
				"	float dist = length( ECPos3 );\n"
				"	float fog = exp( - gl_Fog.density * dist );\n"
				;

	const char* kCalcFogExp2 =
				"	float dist = length( ECPos3 );\n"
				"	float fogProd = gl_Fog.density * dist;\n"
				"	float fog = exp( - fogProd * fogProd );\n"
				;
	
	#pragma mark kCalcFogHalfspace
	const char* kCalcFogHalfspace =
				"	vec4 ECPos = vec4(ECPos3, 1.0);\n"
				"	float p = dot( HSFogPlane, ECPos );\n"
				"	float c = HSFogPlane.w;\n"
				"	float D = gl_Fog.density;\n"
				"	float s = HSFogRate;\n"
				"	float cpDiff = c - p;\n"
				"	float adjDiff = sign(cpDiff) * (abs(cpDiff) + 0.000001);\n"
				"	float outerFactor = length(ECPos3) * D / adjDiff;\n"
				"	float densOverRate = D / s;\n"
				"	float rateOverDens = 1.0 / densOverRate;\n"
				"	float c2 = min( c, 0.0 );\n"
				"	float p2 = min( p, 0.0 );\n"
				"	float g = outerFactor * (c2 - p2 - densOverRate *\n"
				"		(exp( c2 * rateOverDens ) - exp( p2 * rateOverDens )));\n"
				"	float fog = exp( - g );\n"
				;
	
	#pragma mark kMixFog
	/*
		Fog works by mixing the fog color with the fragment color.
		This does not work well with premultiplied alpha, where
		resultColor = 1 * premultipliedFragColor + (1 - alpha) * oldColor,
		because if fog brightens the premultipliedFragColor, then the result
		color will be bright no matter how small alpha is.  Therefore we must
		do the fog mixing with unpremultiplied color.
		
		The FogSmooth function is intended to make the place where you hit the
		maxFogOpacity be a less obvious transition.  It was chosen as a cubic
		polynomial satisfying:
		FogSmooth( 0 ) = 0
		FogSmooth( maxFogOpacity ) = maxFogOpacity
		FogSmooth'( 0 ) = 1				(where the prime indicates derivative)
		FogSmooth'( maxFogOpacity ) = 0
	*/
	const char* kMixFog =
				"	float opacity = 1.0 - fog;\n"
				"	opacity = (maxFogOpacity > 0.99)?\n"
				"		clamp( opacity, 0.0, 1.0) :\n"
				"		((opacity < maxFogOpacity)?\n"
				"			 FogSmooth(opacity) :\n"
				"			maxFogOpacity);\n"
				"	float unAlpha = 1.0 / (alpha + 0.0000001);\n"
				"	vec3 unPreColor = clamp( unAlpha * color, 0.0, 1.0 );\n"
				"	unPreColor = mix( unPreColor, gl_Fog.color.rgb, opacity );\n"
				"	color = alpha * unPreColor;\n"
				;

	#pragma mark kAngleAffectOnAlpha
	/*
		If you have a sheet of translucent material at an angle to the eye, then you are
		looking through more of it, so it should block more of the light.  The way the
		math works out, the effective alpha is 1 - (1 - alpha)^(1/cos( angle )) .
		The z coordinate of the eye normal vector is the cosine of the angle between the
		fragment normal vector and the view vector.
		When we change the alpha, we must un-premultiply and re-premultiply the color.
	*/
	const char*	kAngleAffectOnAlpha =
				"	if ( (alpha < 0.999) && (normal.z > 0.0001) )\n"
				"	{\n"
				"		float unAlpha = 1.0 / (alpha + 0.0000001);\n"
				"		vec3 unPreColor = clamp( unAlpha * color, 0.0, 1.0 );\n"
				"		alpha = 1.0 - pow( 1.0 - alpha, 1.0/normal.z );\n"
				"		color = alpha * unPreColor;\n"
				"	}\n"
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
	const char* kFlippingNormalsFlagUniformName	= "isFlippingNormals";
	const char* kClippingPlaneUniformName		= "ECClipPlane";
	const char*	kMaxFogOpacityUniformName		= "maxFogOpacity";
	const char*	kHSFogRateUniformName			= "HSFogRate";
	const char*	kHSFogPlaneUniformName			= "HSFogPlane";
	const char*	kLightPositionUniformName		= "lightPosition";
	const char*	kLightColorUniformName			= "lightColor";
	const char*	kLightAttenuationUniformName	= "lightAttenuation";
	const char*	kSpotLightDirectionUniformName	= "spotLightDirection";
	
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
	#define		CHECK_GL_ERROR_FMT(...) \
								do { \
									sGLError = glGetError();	\
									if (sGLError != GL_NO_ERROR)	\
									{	\
										char customMsg_[400]; char wholeMsg_[800]; \
										snprintf( customMsg_, sizeof(customMsg_), __VA_ARGS__ ); \
										snprintf( wholeMsg_, sizeof(wholeMsg_), "%s %s", GLUtils_GLErrorToString(sGLError), customMsg_ ); \
										E3Assert(__FILE__, __LINE__, wholeMsg_ );	\
									}	\
								} while (false)
#else
	#define		CHECK_GL_ERROR
	#define		CHECK_GL_ERROR_FMT(...)
#endif

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
	, mIsFlippingNormalsUniformLoc( inOther.mIsFlippingNormalsUniformLoc )
	, mClippingPlaneUniformLoc( inOther.mClippingPlaneUniformLoc )
	, mMaxFogOpacityUniformLoc( inOther.mMaxFogOpacityUniformLoc )
	, mHalfspaceFogRateUniformLoc( inOther.mHalfspaceFogRateUniformLoc )
	, mHalfspaceFogPlaneUniformLoc( inOther.mHalfspaceFogPlaneUniformLoc )
	, mLightPositionUniformLoc( inOther.mLightPositionUniformLoc )
	, mLightColorUniformLoc( inOther.mLightColorUniformLoc )
	, mLightAttenuationUniformLoc( inOther.mLightAttenuationUniformLoc )
	, mSpotLightDirectionUniformLoc( inOther.mSpotLightDirectionUniformLoc )
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
	std::swap( mIsFlippingNormalsUniformLoc, ioOther.mIsFlippingNormalsUniformLoc );
	std::swap( mClippingPlaneUniformLoc, ioOther.mClippingPlaneUniformLoc );
	std::swap( mMaxFogOpacityUniformLoc, ioOther.mMaxFogOpacityUniformLoc );
	std::swap( mHalfspaceFogRateUniformLoc, ioOther.mHalfspaceFogRateUniformLoc );
	std::swap( mHalfspaceFogPlaneUniformLoc, ioOther.mHalfspaceFogPlaneUniformLoc );
	std::swap( mLightPositionUniformLoc, ioOther.mLightPositionUniformLoc );
	std::swap( mLightColorUniformLoc, ioOther.mLightColorUniformLoc );
	std::swap( mLightAttenuationUniformLoc, ioOther.mLightAttenuationUniformLoc );
	std::swap( mSpotLightDirectionUniformLoc, ioOther.mSpotLightDirectionUniformLoc );
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
	glGetProgramiv = nullptr;
	glUseProgram = nullptr;
	glGetUniformLocation = nullptr;
	glUniform1i = nullptr;
	glUniform1f = nullptr;
	glUniform1fv = nullptr;
	glDeleteShader = nullptr;
	glDeleteProgram = nullptr;
	glGetProgramInfoLog = nullptr;
	glGetShaderInfoLog = nullptr;
	glGetShaderSource = nullptr;
	glIsProgram = nullptr;
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
		GLGetProcAddress( glUniform4f, "glUniform4f", "glUniform4fARB" );
		GLGetProcAddress( glUniform1fv, "glUniform1fv", "glUniform1fvARB" );
		GLGetProcAddress( glUniform3fv, "glUniform3fv", "glUniform3fvARB" );
		GLGetProcAddress( glUniform4fv, "glUniform4fv", "glUniform4fvARB" );
		GLGetProcAddress( glDeleteShader, "glDeleteShader", "glDeleteObjectARB" );
		GLGetProcAddress( glDeleteProgram, "glDeleteProgram", "glDeleteObjectARB" );
		GLGetProcAddress( glGetProgramInfoLog, "glGetProgramInfoLog", "glGetInfoLogARB" );
		GLGetProcAddress( glGetShaderInfoLog, "glGetShaderInfoLog", "glGetInfoLogARB" );
		GLGetProcAddress( glGetShaderSource, "glGetShaderSource", "glGetShaderSourceARB" );
		GLGetProcAddress( glIsProgram, "glIsProgram" );
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
			(glUniform3fv == nullptr) ||
			(glUniform4fv == nullptr) ||
			(glDeleteShader == nullptr) ||
			(glDeleteProgram == nullptr) ||
			(glGetProgramInfoLog == nullptr) ||
			(glGetShaderInfoLog == nullptr) )
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
	, mExtensions( inExtensions )
	, mRendererObject( inRendererObject )
	, mGLContext( inGLContext )
	, mIsShading( false )
	, mMayNeedProgramChange( true )
	, mIsSpecularMapped( false )
	, mIsFlippingNormals( true )
	, mQuantization( 0.0f )
	, mLightNearEdge( 1.0f )
	, mMaxFogOpacity( 1.0f )
	, mCurrentProgram( nullptr )
{
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



static void BuildFragmentShaderSource(	const QORenderer::ProgramCharacteristic& inProgramRec,
										std::string& outSource )
{
	const GLint kNumLights = static_cast<GLint>(inProgramRec.mPattern.size());
	GLint i;
	
	outSource += kFragmentShaderPrefix;
	
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
}


/*!
	@function	StartFrame
	@abstract	Begin a rendering frame.
*/
void	QORenderer::PerPixelLighting::StartFrame( TQ3ViewObject inView )
{
	// Save a reference to the view, and initialize the light group edit index
	mView.Assign( inView );
	CQ3ObjectRef lightGroup( CQ3View_GetLightGroup( inView ) );
	mLightGroupEditIndex = Q3Shared_GetEditIndex( lightGroup.get() );

	CheckIfShading();
	
	if (mIsShading)
	{
		CalcMaxLights();
		InitVertexShader();
	}
}

void	QORenderer::PerPixelLighting::CalcMaxLights()
{
#if 0
	mMaxLights = mExtensions.maxLights;
#else
	// Find the total number of uniform scalars there can be
	GLint maxUniformScalars = 1024;
	glGetIntegerv( GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB, &maxUniformScalars );
	
	// Subtract some for the non-light uniforms.  Right now there are 15, but leave
	// some slack.
	GLint scalarsAvailableForLights = maxUniformScalars - 50;
	
	// For each light, we need 20 or fewer values.
	mMaxLights = scalarsAvailableForLights / 20;
#endif
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
	if (mIsShading)
	{
		mProgramCharacteristic.mIlluminationType = kQ3IlluminationTypeNULL;
		mProgramCharacteristic.mIsTextured = false;
		mCurrentProgram = nullptr;
		mMayNeedProgramChange = true;
		mProgramCharacteristic.mIsCartoonish = (mQuantization > 0.0f);
		mIsSpecularMapped = false;
		
		mProgramCharacteristic.mIsUsingClippingPlane = false;
		mClippingPlane.x = 999999.0f;
		
		mProgramCharacteristic.mFogModeCombined = kFogModeOff;
		mMaxFogOpacity = 1.0f;

		TQ3Boolean angleAffectsAlpha = kQ3True;
		Q3Object_GetProperty( mRendererObject, kQ3RendererPropertyAngleAffectsAlpha,
			sizeof(TQ3Boolean), nullptr, &angleAffectsAlpha );
		mProgramCharacteristic.mAngleAffectsAlpha = (angleAffectsAlpha == kQ3True);
		
		Q3Camera_GetWorldToView( inCamera, &mWorldToView );
		
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
				CHECK_GL_ERROR_FMT("glUseProgram");
			#if Q3_DEBUG
				if ( (sGLError != GL_NO_ERROR) && (mFuncs.glIsProgram != nullptr) )
				{
					GLboolean isProgram = (*mFuncs.glIsProgram)( theProgram->mProgram );
					Q3_MESSAGE_FMT("Program name %u is%s known to be a program",
						(unsigned int)theProgram->mProgram,
						isProgram? "" : " not" );
				}
			#endif
			
				SetUniformValues( *theProgram );
			}
			
			// Even if we didn't change the program, we need to update some
			// uniforms.
			mFuncs.glUniform1i( theProgram->mIsSpecularMappingUniformLoc, mIsSpecularMapped );
			mFuncs.glUniform1i( theProgram->mIsFlippingNormalsUniformLoc, mIsFlippingNormals );
			if (mProgramCharacteristic.mFogModeCombined != kFogModeOff)
			{
				mFuncs.glUniform1f( theProgram->mMaxFogOpacityUniformLoc, mMaxFogOpacity );
				if (mProgramCharacteristic.mFogModeCombined == kFogModeHalfspace)
				{
					mFuncs.glUniform1f( theProgram->mHalfspaceFogRateUniformLoc,
						mHalfspaceFogRate );
					mFuncs.glUniform4f( theProgram->mHalfspaceFogPlaneUniformLoc,
						mHalfspaceFogPlane.x, mHalfspaceFogPlane.y,
						mHalfspaceFogPlane.z, mHalfspaceFogPlane.w );
				}
			}
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
	CHECK_GL_ERROR_FMT("glUniform1f mQuantization");
	mFuncs.glUniform1f( ioProgram.mLightNearEdgeUniformLoc, mLightNearEdge );
	CHECK_GL_ERROR_FMT("glUniform1f mLightNearEdge");
	
	// Set lighting uniform arrays.
	const int kNumLights = static_cast<int>(mLights.size());
	if (kNumLights > 0)
	{
		mFuncs.glUniform1fv( ioProgram.mSpotHotAngleUniformLoc, kNumLights,
			&mSpotLightHotAngles[0] );
		mFuncs.glUniform1fv( ioProgram.mSpotCutoffAngleUniformLoc, kNumLights,
			&mSpotLightCutoffAngles[0] );
		
		mFuncs.glUniform4fv( ioProgram.mLightPositionUniformLoc, kNumLights,
			&mLightPositions[0].x );
		mFuncs.glUniform4fv( ioProgram.mLightColorUniformLoc, kNumLights,
			&mLightColors[0].r );
		mFuncs.glUniform3fv( ioProgram.mSpotLightDirectionUniformLoc, kNumLights,
			&mSpotLightDirections[0].x );
		mFuncs.glUniform3fv( ioProgram.mLightAttenuationUniformLoc, kNumLights,
			&mLightAttenuations[0].x );
	}
	
	// Set extra fog parameters.
	mFuncs.glUniform1f( ioProgram.mMaxFogOpacityUniformLoc, mMaxFogOpacity );
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
		mCurrentProgram = nullptr;
	}
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
	ioProgram.mIsFlippingNormalsUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kFlippingNormalsFlagUniformName );
	CHECK_GL_ERROR;
	ioProgram.mClippingPlaneUniformLoc = mFuncs.glGetUniformLocation(
		ioProgram.mProgram, kClippingPlaneUniformName );
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
	
}

/*!
	@function	CheckIfShading
	@abstract	Check whether we are doing per-pixel lighting on this frame.
*/
void	QORenderer::PerPixelLighting::CheckIfShading()
{
	TQ3Boolean	propValue;
	TQ3Status	propStatus = Q3Object_GetProperty( mRendererObject,
		kQ3RendererPropertyPerPixelLighting, sizeof(propValue), nullptr,
		&propValue );
	
	mIsShading = (mFuncs.glCreateShader != nullptr) &&
		(propStatus == kQ3Success) &&
		(propValue == kQ3True);
	
	if (mIsShading)
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
		CHECK_GL_ERROR_FMT("glShaderSource");
		
		// Compile the shader
		inFuncs.glCompileShader( shaderID );
		CHECK_GL_ERROR_FMT("glCompileShader");
		
		// Check for compile success
		GLint	status;
		inFuncs.glGetShaderiv( shaderID, GL_COMPILE_STATUS, &status );
		
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
	if (ProgCache()->VertexShaderID() == 0)
	{
		std::string shaderSource( kVertexShaderStart );
		if ( ! mExtensions.ATICard )
		{
			shaderSource += kVertexShaderClipVertex;
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
			ProgCache()->SetVertexShaderID( vertexShader );
		}
	}
}


static std::string DescribeProgram( const QORenderer::ProgramRec& inProgram )
{
	std::ostringstream desc;
	desc << "L(";
	const unsigned long kLightCount = inProgram.mCharacteristic.mPattern.size();
	unsigned long i;
	for (i = 0; i < kLightCount; ++i)
	{
		if (i > 0)
		{
			desc << ',';
		}
		desc << inProgram.mCharacteristic.mPattern[i];
	}
	desc << ')';
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
	@function	InitProgram
	@abstract	Set up the main fragment shader and program.
*/
void	QORenderer::PerPixelLighting::InitProgram()
{
	CHECK_GL_ERROR_FMT("InitProgram start");
	ProgramRec	newProgram;
	
	newProgram.mCharacteristic = mProgramCharacteristic;
	
	// Build the source of the fragment shader
	std::string	fragSource;
	BuildFragmentShaderSource( newProgram.mCharacteristic, fragSource );
		
	// Create the fragment shader
	GLint shaderID = CreateAndCompileShader( GL_FRAGMENT_SHADER,
		fragSource.c_str(), mFuncs );
	CHECK_GL_ERROR_FMT("InitProgram after CreateAndCompileShader");

	if (shaderID != 0)
	{
		// Create a program.
		newProgram.mProgram = mFuncs.glCreateProgram();
		CHECK_GL_ERROR_FMT("glCreateProgram");
	
		if (newProgram.mProgram != 0)
		{
			GLuint vertexShader = ProgCache()->VertexShaderID();

			++sProgramCount;
			std::string desc( DescribeProgram( newProgram ) );
			Q3_MESSAGE_FMT("Created program number %d, ID %u, of type %s",
				sProgramCount, (unsigned int)newProgram.mProgram, desc.c_str() );

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
			CHECK_GL_ERROR;
		
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
	@function	UpdateBackfacingStyle
	@abstract	Notification that the type of backfacing style may have
				changed.
*/
void	QORenderer::PerPixelLighting::UpdateBackfacingStyle(
									TQ3BackfacingStyle inBackfacing )
{
	if (mIsShading)
	{
		bool shouldFlip = (inBackfacing != kQ3BackfacingStyleRemove);
		if (shouldFlip != mIsFlippingNormals)
		{
			mIsFlippingNormals = shouldFlip;
			mMayNeedProgramChange = true;
		}
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
	Q3Camera_GetWorldToView( theCamera.get(), &worldToView );
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
	if (mIsShading)
	{
		QORenderer::EFogModeCombined newMode = CombinedFogMode( inFog );
		TQ3RationalPoint4D newEyePlane = { 0.0f, 1.0f, 0.0f, 0.0f };
		if (newMode == kFogModeHalfspace)
		{
			newEyePlane = ConvertPlaneWorldToEye( inView, inFog.halfspaceFogPlane );
		}
	
		if ( (newMode != mProgramCharacteristic.mFogModeCombined) ||
			( (newMode != kFogModeOff) && (inFog.maxOpacity != mMaxFogOpacity) ) ||
			(
				(newMode == kFogModeHalfspace) &&
				(
					(inFog.halfspaceFogRate != mHalfspaceFogRate) ||
					(newEyePlane != mHalfspaceFogPlane)
				)
			)
		)
		{
			mProgramCharacteristic.mFogModeCombined = newMode;
			mMaxFogOpacity = inFog.maxOpacity;
			mHalfspaceFogRate = inFog.halfspaceFogRate;
			mHalfspaceFogPlane = newEyePlane;
			
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
	@param		inGeom		Geometry being rendered.  May be nullptr.
*/
void	QORenderer::PerPixelLighting::PreGeomSubmit( TQ3GeometryObject inGeom )
{
	if ( mIsShading )
	{
		CQ3ObjectRef lightGroup( CQ3View_GetLightGroup( mView.get() ) );
		TQ3Uns32 lightGroupEditIndex = Q3Shared_GetEditIndex( lightGroup.get() );
		if (lightGroupEditIndex != mLightGroupEditIndex)
		{
			UpdateLighting();
		}

		bool cartoonUpdate = false;
		CHECK_GL_ERROR_FMT("PreGeomSubmit 1");
		
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
		CHECK_GL_ERROR_FMT("PreGeomSubmit 2");
		
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
}

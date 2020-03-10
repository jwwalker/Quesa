/*!
	@header		QOGLSLShaders.h
	
	This header holds declarations relating to use of OpenGL Shading Language
	in the Quesa OpenGL renderer.
*/
/*  NAME:
       QOGLSLShaders.h

    DESCRIPTION:
        Header for Quesa OpenGL shaders.
		    
    COPYRIGHT:
        Copyright (c) 2020, Quesa Developers. All rights reserved.

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
#ifndef QOGLSLShaders_h
#define QOGLSLShaders_h

namespace QOGLSLShader
{
	// Vertex Shader
	extern const char* kVertexShaderStart;
	extern const char* kVertexShaderStandardProjection;
	extern const char* kVertexShaderAllSeeingProjection;
	extern const char* kVertexShaderFisheyeProjection;
	extern const char* kVertexShaderEnd;
	
	
	// Geometry Shaders
	extern const char* kFisheyeGeomShader;
	extern const char* kAllSeeingGeomShader;
	extern const char* kFaceEdgeGeomShader;
	extern const char* kLineGeomShader;
	
	
	// Fragment Shaders
	extern const char* kFragmentShaderPrefix;
	extern const char* kFragmentShaderInputWithoutGeomShader;
	extern const char* kFragmentShaderInputWithGeomShader;
	extern const char* kFragmentShaderQuantizeFuncs_Normal;
	extern const char* kFragmentShaderQuantizeFuncs_Cartoonish;
	extern const char* kDirectionalLightFragmentShaderSource;
	extern const char* kDirectionalLightWithNondirIllumFragmentShaderSource;
	extern const char* kSpotFalloffNoneSource;
	extern const char* kSpotFalloffLinearSource;
	extern const char* kSpotFalloffExponentialSource;
	extern const char* kSpotFalloffCosineSource;
	extern const char* kSpotFalloffSmoothCubicSource;
	extern const char* kPointLightFragmentShaderSource;
	extern const char* kPointLightWithNondirIllumFragmentShaderSource;
	extern const char* kSpotLightFragmentShaderSource;
	extern const char* kSpotLightWithNondirIllumFragmentShaderSource;
	extern const char* kMainFragmentShaderStart;
	extern const char* kFragmentClipping;
	extern const char* kFragmentFisheyeCropping;
	extern const char* kMainFragmentShaderStartSmooth;
	extern const char* kMainFragmentShaderStartFlat;
	extern const char* kColorCompForNULLIllumination;
	extern const char* kColorCompForLambertAndPhong;
	extern const char* kColorCompForLambertAndPhong_Cartoonish;
	extern const char* kTexturedColorComp;
	extern const char* kAddSpecularColor;
	extern const char* kCalcFogLinear;
	extern const char* kCalcFogExp;
	extern const char* kCalcFogExp2;
	extern const char* kCalcFogHalfspace;
	extern const char* kMixFog;
	extern const char*	kAngleAffectOnAlpha;
	extern const char* kMainFragmentShaderEndSource;
	
}

#endif /* QOGLSLShaders_h */

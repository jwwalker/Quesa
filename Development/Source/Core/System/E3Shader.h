/*  NAME:
        E3Shader.h

    DESCRIPTION:
        Header file for E3Shader.c.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
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
#ifndef E3SHADER_HDR
#define E3SHADER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
	


// Shader state data
typedef struct TQ3ShaderData {
	TQ3ShaderUVBoundary		uBoundary;
	TQ3ShaderUVBoundary		vBoundary;
	TQ3Matrix3x3			uvTransform;
} TQ3ShaderData;





class E3Shader : public E3ShapeData
	{
Q3_CLASS_ENUMS ( kQ3ShapeTypeShader, E3Shader, E3ShapeData )

	TQ3ShaderData		shaderData ;


public :

	TQ3Status			SetUVTransform ( const TQ3Matrix3x3* uvTransform ) ;
	TQ3Status			GetUVTransform ( TQ3Matrix3x3* uvTransform ) ;
	TQ3Status			SetUBoundary ( TQ3ShaderUVBoundary uBoundary ) ;
	TQ3Status			SetVBoundary ( TQ3ShaderUVBoundary vBoundary ) ;
	TQ3Status			GetUBoundary ( TQ3ShaderUVBoundary* uBoundary )	;
	TQ3Status			GetVBoundary ( TQ3ShaderUVBoundary* vBoundary ) ;

	} ;



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3ColorARGB *			E3ColorARGB_Set(TQ3ColorARGB *color, float a, float r, float g, float b);
TQ3ColorARGB *			E3ColorARGB_Add(const TQ3ColorARGB *c1, const TQ3ColorARGB *c2, TQ3ColorARGB *result);
TQ3ColorARGB *			E3ColorARGB_Subtract(const TQ3ColorARGB *c1, const TQ3ColorARGB *c2, TQ3ColorARGB *result);
TQ3ColorARGB *			E3ColorARGB_Scale(const TQ3ColorARGB *color, float scale, TQ3ColorARGB *result);
TQ3ColorARGB *			E3ColorARGB_Clamp(const TQ3ColorARGB *color, TQ3ColorARGB *result);
TQ3ColorARGB *			E3ColorARGB_Lerp(const TQ3ColorARGB *first, const TQ3ColorARGB *last, float alpha, TQ3ColorARGB *result);
TQ3ColorARGB *			E3ColorARGB_Accumulate(const TQ3ColorARGB *src, TQ3ColorARGB *result);

TQ3ColorRGB *			E3ColorRGB_Set(TQ3ColorRGB *color, float r, float g, float b);
TQ3ColorRGB *			E3ColorRGB_Add(const TQ3ColorRGB *c1, const TQ3ColorRGB *c2, TQ3ColorRGB *result);
TQ3ColorRGB *			E3ColorRGB_Subtract(const TQ3ColorRGB *c1, const TQ3ColorRGB *c2, TQ3ColorRGB *result);
TQ3ColorRGB *			E3ColorRGB_Scale(const TQ3ColorRGB *color, float scale, TQ3ColorRGB *result);
TQ3ColorRGB *			E3ColorRGB_Clamp(const TQ3ColorRGB *color, TQ3ColorRGB *result);
TQ3ColorRGB *			E3ColorRGB_Lerp(const TQ3ColorRGB *first, const TQ3ColorRGB *last, float alpha, TQ3ColorRGB *result);
TQ3ColorRGB *			E3ColorRGB_Accumulate(const TQ3ColorRGB *src, TQ3ColorRGB *result);
float *					E3ColorRGB_Luminance(const TQ3ColorRGB *color, float *luminance);

TQ3Status				E3Shader_RegisterClass(void);
TQ3Status				E3Shader_UnregisterClass(void);

TQ3ObjectType			E3Shader_GetType(TQ3ShaderObject shader);
TQ3Status				E3Shader_Submit(TQ3ShaderObject shader, TQ3ViewObject view);
//TQ3Status				E3Shader_SetUVTransform(TQ3ShaderObject shader, const TQ3Matrix3x3 *uvTransform);
//TQ3Status				E3Shader_GetUVTransform(TQ3ShaderObject shader, TQ3Matrix3x3 *uvTransform);
//TQ3Status				E3Shader_SetUBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary uBoundary);
//TQ3Status				E3Shader_SetVBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary vBoundary);
//TQ3Status				E3Shader_GetUBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary *uBoundary);
//TQ3Status				E3Shader_GetVBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary *vBoundary);

TQ3ObjectType			E3IlluminationShader_GetType(TQ3ShaderObject shader);
TQ3ShaderObject			E3PhongIllumination_New(void);
TQ3ShaderObject			E3LambertIllumination_New(void);
TQ3ShaderObject			E3NULLIllumination_New(void);

TQ3ObjectType			E3SurfaceShader_GetType(TQ3SurfaceShaderObject shader);
TQ3ShaderObject			E3TextureShader_New(TQ3TextureObject texture);
TQ3Status				E3TextureShader_GetTexture(TQ3ShaderObject shader, TQ3TextureObject *texture);
TQ3Status				E3TextureShader_SetTexture(TQ3ShaderObject shader, TQ3TextureObject texture);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


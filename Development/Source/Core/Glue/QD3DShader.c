/*  NAME:
        QD3DShader.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Shader.h"
#include "E3Texture.h"





//=============================================================================
//      Inline APIs
//-----------------------------------------------------------------------------
#undef Q3ColorARGB_Set
#undef Q3ColorARGB_Add
#undef Q3ColorARGB_Subtract
#undef Q3ColorARGB_Scale
#undef Q3ColorARGB_Clamp
#undef Q3ColorARGB_Lerp
#undef Q3ColorARGB_Accumulate
#undef Q3ColorRGB_Set
#undef Q3ColorRGB_Add
#undef Q3ColorRGB_Subtract
#undef Q3ColorRGB_Scale
#undef Q3ColorRGB_Clamp
#undef Q3ColorRGB_Lerp
#undef Q3ColorRGB_Accumulate
#undef Q3ColorRGB_Luminance





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3ColorARGB_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
Q3ColorARGB_Set(TQ3ColorARGB *color, float a, float r, float g, float b)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on color
		return(NULL);

	if (0) // Further checks on a
		return(NULL);

	if (0) // Further checks on r
		return(NULL);

	if (0) // Further checks on g
		return(NULL);

	if (0) // Further checks on b
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorARGB_Set(color, a, r, g, b));
}





//=============================================================================
//      Q3ColorARGB_Add : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
Q3ColorARGB_Add(const TQ3ColorARGB *c1, const TQ3ColorARGB *c2, TQ3ColorARGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(c1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(c2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on c1
		return(NULL);

	if (0) // Further checks on c2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorARGB_Add(c1, c2, result));
}





//=============================================================================
//      Q3ColorARGB_Subtract : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
Q3ColorARGB_Subtract(const TQ3ColorARGB *c1, const TQ3ColorARGB *c2, TQ3ColorARGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(c1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(c2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on c1
		return(NULL);

	if (0) // Further checks on c2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorARGB_Subtract(c1, c2, result));
}





//=============================================================================
//      Q3ColorARGB_Scale : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
Q3ColorARGB_Scale(const TQ3ColorARGB *color, float scale, TQ3ColorARGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on color
		return(NULL);

	if (0) // Further checks on scale
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorARGB_Scale(color, scale, result));
}





//=============================================================================
//      Q3ColorARGB_Clamp : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
Q3ColorARGB_Clamp(const TQ3ColorARGB *color, TQ3ColorARGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on color
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorARGB_Clamp(color, result));
}





//=============================================================================
//      Q3ColorARGB_Lerp : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
Q3ColorARGB_Lerp(const TQ3ColorARGB *first, const TQ3ColorARGB *last, float alpha, TQ3ColorARGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(first), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(last), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on first
		return(NULL);

	if (0) // Further checks on last
		return(NULL);

	if (0) // Further checks on alpha
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorARGB_Lerp(first, last, alpha, result));
}





//=============================================================================
//      Q3ColorARGB_Accumulate : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorARGB *
Q3ColorARGB_Accumulate(const TQ3ColorARGB *src, TQ3ColorARGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(src), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on src
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorARGB_Accumulate(src, result));
}





//=============================================================================
//      Q3ColorRGB_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
Q3ColorRGB_Set(TQ3ColorRGB *color, float r, float g, float b)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on color
		return(NULL);

	if (0) // Further checks on r
		return(NULL);

	if (0) // Further checks on g
		return(NULL);

	if (0) // Further checks on b
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorRGB_Set(color, r, g, b));
}





//=============================================================================
//      Q3ColorRGB_Add : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
Q3ColorRGB_Add(const TQ3ColorRGB *c1, const TQ3ColorRGB *c2, TQ3ColorRGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(c1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(c2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on c1
		return(NULL);

	if (0) // Further checks on c2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorRGB_Add(c1, c2, result));
}





//=============================================================================
//      Q3ColorRGB_Subtract : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
Q3ColorRGB_Subtract(const TQ3ColorRGB *c1, const TQ3ColorRGB *c2, TQ3ColorRGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(c1), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(c2), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on c1
		return(NULL);

	if (0) // Further checks on c2
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorRGB_Subtract(c1, c2, result));
}





//=============================================================================
//      Q3ColorRGB_Scale : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
Q3ColorRGB_Scale(const TQ3ColorRGB *color, float scale, TQ3ColorRGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on color
		return(NULL);

	if (0) // Further checks on scale
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorRGB_Scale(color, scale, result));
}





//=============================================================================
//      Q3ColorRGB_Clamp : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
Q3ColorRGB_Clamp(const TQ3ColorRGB *color, TQ3ColorRGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on color
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorRGB_Clamp(color, result));
}





//=============================================================================
//      Q3ColorRGB_Lerp : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
Q3ColorRGB_Lerp(const TQ3ColorRGB *first, const TQ3ColorRGB *last, float alpha, TQ3ColorRGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(first), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(last), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on first
		return(NULL);

	if (0) // Further checks on last
		return(NULL);

	if (0) // Further checks on alpha
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorRGB_Lerp(first, last, alpha, result));
}





//=============================================================================
//      Q3ColorRGB_Accumulate : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
Q3ColorRGB_Accumulate(const TQ3ColorRGB *src, TQ3ColorRGB *result)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(src), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(result), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on src
		return(NULL);

	if (0) // Further checks on result
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorRGB_Accumulate(src, result));
}





//=============================================================================
//      Q3ColorRGB_Luminance : Quesa API entry point.
//-----------------------------------------------------------------------------
float *
Q3ColorRGB_Luminance(const TQ3ColorRGB *color, float *luminance)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(luminance), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on color
		return(NULL);

	if (0) // Further checks on luminance
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ColorRGB_Luminance(color, luminance));
}



#pragma mark -

//=============================================================================
//      Q3Shader_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3Shader_GetType(TQ3ShaderObject shader)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shader, kQ3ShapeTypeShader), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Shader_GetType(shader));
}





//=============================================================================
//      Q3Shader_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shader_Submit(TQ3ShaderObject shader, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shader, (kQ3ShapeTypeShader)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, (kQ3ObjectTypeView)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Shader_Submit(shader, view));
}





//=============================================================================
//      Q3Shader_SetUVTransform : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shader_SetUVTransform(TQ3ShaderObject shader, const TQ3Matrix3x3 *uvTransform)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shader, (kQ3ShapeTypeShader)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(uvTransform), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3Failure);

	if (0) // Further checks on uvTransform
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shader*) shader )->SetUVTransform ( uvTransform ) ;
}





//=============================================================================
//      Q3Shader_GetUVTransform : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shader_GetUVTransform(TQ3ShaderObject shader, TQ3Matrix3x3 *uvTransform)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shader, (kQ3ShapeTypeShader)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(uvTransform), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3Failure);

	if (0) // Further checks on uvTransform
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shader*) shader )->GetUVTransform ( uvTransform ) ;
}





//=============================================================================
//      Q3Shader_SetUBoundary : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shader_SetUBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary uBoundary)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shader, (kQ3ShapeTypeShader)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3Failure);

	if (0) // Further checks on uBoundary
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shader*) shader )->SetUBoundary ( uBoundary ) ;
}





//=============================================================================
//      Q3Shader_SetVBoundary : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shader_SetVBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary vBoundary)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shader, (kQ3ShapeTypeShader)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3Failure);

	if (0) // Further checks on vBoundary
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shader*) shader )->SetVBoundary ( vBoundary ) ;
}





//=============================================================================
//      Q3Shader_GetUBoundary : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shader_GetUBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary *uBoundary)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(shader != NULL, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(shader->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_OBJECT_IS_CLASS(shader, E3Shader), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(uBoundary), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3Failure);

	if (0) // Further checks on uBoundary
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shader*) shader )->GetUBoundary ( uBoundary ) ;
}





//=============================================================================
//      Q3Shader_GetVBoundary : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shader_GetVBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary *vBoundary)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(shader != NULL, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(shader->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_OBJECT_IS_CLASS(shader, E3Shader), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vBoundary), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3Failure);

	if (0) // Further checks on vBoundary
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shader*) shader )->GetVBoundary ( vBoundary ) ;
}



#pragma mark -

//=============================================================================
//      Q3IlluminationShader_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3IlluminationShader_GetType(TQ3ShaderObject shader)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shader, kQ3ShapeTypeShader), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3IlluminationShader_GetType(shader));
}





//=============================================================================
//      Q3PhongIllumination_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ShaderObject
Q3PhongIllumination_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PhongIllumination_New());
}





//=============================================================================
//      Q3LambertIllumination_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ShaderObject
Q3LambertIllumination_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3LambertIllumination_New());
}





//=============================================================================
//      Q3NULLIllumination_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ShaderObject
Q3NULLIllumination_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NULLIllumination_New());
}





//=============================================================================
//      Q3SurfaceShader_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3SurfaceShader_GetType(TQ3SurfaceShaderObject shader)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shader, kQ3ShaderTypeSurface), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SurfaceShader_GetType(shader));
}



#pragma mark -

//=============================================================================
//      Q3TextureShader_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ShaderObject
Q3TextureShader_New(TQ3TextureObject texture)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(texture, kQ3SharedTypeTexture), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on texture
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TextureShader_New(texture));
}





//=============================================================================
//      Q3TextureShader_GetTexture : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TextureShader_GetTexture(TQ3ShaderObject shader, TQ3TextureObject *texture)
{


	// Release build checks

	Q3_REQUIRE_OR_RESULT(shader != NULL, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(shader->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_OBJECT_IS_CLASS(shader, E3Shader), kQ3Failure);

	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(texture), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3Failure);

	if (0) // Further checks on texture
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TextureShader_GetTexture(shader, texture));
}





//=============================================================================
//      Q3TextureShader_SetTexture : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TextureShader_SetTexture(TQ3ShaderObject shader, TQ3TextureObject texture)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shader, (kQ3ShapeTypeShader)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(texture, (kQ3SharedTypeTexture)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shader
		return(kQ3Failure);

	if (0) // Further checks on texture
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TextureShader_SetTexture(shader, texture));
}





//=============================================================================
//      Q3Texture_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3Texture_GetType(TQ3TextureObject texture)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(texture, kQ3SharedTypeTexture), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on texture
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Texture_GetType(texture));
}





//=============================================================================
//      Q3Texture_GetWidth : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Texture_GetWidth(TQ3TextureObject texture, TQ3Uns32 *width)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(texture, (kQ3SharedTypeTexture)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(width), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on texture
		return(kQ3Failure);

	if (0) // Further checks on width
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Texture_GetWidth(texture, width));
}





//=============================================================================
//      Q3Texture_GetHeight : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Texture_GetHeight(TQ3TextureObject texture, TQ3Uns32 *height)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(texture, (kQ3SharedTypeTexture)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(height), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on texture
		return(kQ3Failure);

	if (0) // Further checks on height
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Texture_GetHeight(texture, height));
}



#pragma mark -

//=============================================================================
//      Q3PixmapTexture_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TextureObject
Q3PixmapTexture_New(const TQ3StoragePixmap *pixmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmap), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmap
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapTexture_New(pixmap));
}





//=============================================================================
//      Q3PixmapTexture_GetPixmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapTexture_GetPixmap(TQ3TextureObject texture, TQ3StoragePixmap *pixmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(texture, (kQ3SharedTypeTexture)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on texture
		return(kQ3Failure);

	if (0) // Further checks on pixmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapTexture_GetPixmap(texture, pixmap));
}





//=============================================================================
//      Q3PixmapTexture_SetPixmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapTexture_SetPixmap(TQ3TextureObject texture, const TQ3StoragePixmap *pixmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(texture, (kQ3SharedTypeTexture)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on texture
		return(kQ3Failure);

	if (0) // Further checks on pixmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapTexture_SetPixmap(texture, pixmap));
}



#pragma mark -

//=============================================================================
//      Q3MipmapTexture_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TextureObject
Q3MipmapTexture_New(const TQ3Mipmap *mipmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(mipmap), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mipmap
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MipmapTexture_New(mipmap));
}





//=============================================================================
//      Q3MipmapTexture_GetMipmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MipmapTexture_GetMipmap(TQ3TextureObject texture, TQ3Mipmap *mipmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(texture, (kQ3SharedTypeTexture)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(mipmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on texture
		return(kQ3Failure);

	if (0) // Further checks on mipmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MipmapTexture_GetMipmap(texture, mipmap));
}





//=============================================================================
//      Q3MipmapTexture_SetMipmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MipmapTexture_SetMipmap(TQ3TextureObject texture, const TQ3Mipmap *mipmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(texture, (kQ3SharedTypeTexture)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(mipmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on texture
		return(kQ3Failure);

	if (0) // Further checks on mipmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MipmapTexture_SetMipmap(texture, mipmap));
}



#pragma mark -

//=============================================================================
//      Q3CompressedPixmapTexture_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_SUPPORT_QUICKTIME

TQ3TextureObject
Q3CompressedPixmapTexture_New(const TQ3CompressedPixmap *compressedPixmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(compressedPixmap), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on compressedPixmap
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CompressedPixmapTexture_New(compressedPixmap));
}





//=============================================================================
//      Q3CompressedPixmapTexture_GetCompressedPixmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CompressedPixmapTexture_GetCompressedPixmap(TQ3TextureObject texture, TQ3CompressedPixmap *compressedPixmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(texture, (kQ3SharedTypeTexture)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(compressedPixmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on texture
		return(kQ3Failure);

	if (0) // Further checks on compressedPixmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CompressedPixmapTexture_GetCompressedPixmap(texture, compressedPixmap));
}





//=============================================================================
//      Q3CompressedPixmapTexture_SetCompressedPixmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CompressedPixmapTexture_SetCompressedPixmap(TQ3TextureObject texture, const TQ3CompressedPixmap *compressedPixmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(texture, (kQ3SharedTypeTexture)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(compressedPixmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on texture
		return(kQ3Failure);

	if (0) // Further checks on compressedPixmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CompressedPixmapTexture_SetCompressedPixmap(texture, compressedPixmap));
}





//=============================================================================
//      Q3CompressedPixmapTexture_CompressImage : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CompressedPixmapTexture_CompressImage(TQ3CompressedPixmap *compressedPixmap, PixMapHandle sourcePixMap, CodecType codecType, CodecComponent codecComponent, TQ3Int16 codedDepth, CodecQ codecQuality)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(compressedPixmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on compressedPixmap
		return(kQ3Failure);

	if (0) // Further checks on sourcePixMap
		return(kQ3Failure);

	if (0) // Further checks on codecType
		return(kQ3Failure);

	if (0) // Further checks on codecComponent
		return(kQ3Failure);

	if (0) // Further checks on codedDepth
		return(kQ3Failure);

	if (0) // Further checks on codecQuality
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CompressedPixmapTexture_CompressImage(compressedPixmap, sourcePixMap, codecType, codecComponent, codedDepth, codecQuality));
}

#endif // QUESA_SUPPORT_QUICKTIME





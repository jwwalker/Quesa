/*  NAME:
        E3Shader.h

    DESCRIPTION:
        Header file for E3Shader.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
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
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3ColorARGB *			E3ColorARGB_Set(TQ3ColorARGB *color, float a, float r, float g, float b);
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
TQ3Status				E3Shader_SetUVTransform(TQ3ShaderObject shader, const TQ3Matrix3x3 *uvTransform);
TQ3Status				E3Shader_GetUVTransform(TQ3ShaderObject shader, TQ3Matrix3x3 *uvTransform);
TQ3Status				E3Shader_SetUBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary uBoundary);
TQ3Status				E3Shader_SetVBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary vBoundary);
TQ3Status				E3Shader_GetUBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary *uBoundary);
TQ3Status				E3Shader_GetVBoundary(TQ3ShaderObject shader, TQ3ShaderUVBoundary *vBoundary);

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


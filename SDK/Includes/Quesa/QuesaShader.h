/*! @header QuesaShader.h
        Declares the Quesa shader objects.
 */
/*  NAME:
        QuesaShader.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

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
#ifndef QUESA_SHADER_HDR
#define QUESA_SHADER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#ifdef __QD3DSHADER__
#error
#endif

#define __QD3DSHADER__

#if QUESA_SUPPORT_QUICKTIME
	#include <QuickTime/QuickTime.h>
#endif




//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
 *  @enum
 *      TQ3ShaderUVBoundary
 *  @discussion
 *      Shader UV boundary behaviour.
 *
 *  @constant kQ3ShaderUVBoundaryWrap    Wrap UV values outside of 0..1.
 *  @constant kQ3ShaderUVBoundaryClamp   Clamp UV values to 0..1.
 */
typedef enum TQ3ShaderUVBoundary {
    kQ3ShaderUVBoundaryWrap                     = 0,
    kQ3ShaderUVBoundaryClamp                    = 1,
    kQ3ShaderUVBoundarySize32                   = 0xFFFFFFFF
} TQ3ShaderUVBoundary;


/*!
	@enum	Texture&nbsp;shader&nbsp;element&nbsp;types
	
	@abstract
			Custom elements that may be set on texture shader objects.
	
	@constant	kQ3ElementTypeTextureShaderAlphaTest
					Normally, geometry using a texture with an alpha
					channel must be broken into triangles to be sorted by depth
					and rendered back to front.  With this element, you can
					instead treat each texel as either completely opaque or
					completely transparent, avoiding depth sorting.  The
					element value is a depth threshold, i.e., alpha less than
					the threshold is considered completely transparent.
					Data type: TQ3Float32.
*/
enum
{
	kQ3ElementTypeTextureShaderAlphaTest	= Q3_OBJECT_TYPE(0xF0, 's', 'a', 't')
};



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
	@functiongroup ARGB Color
*/

/*!
 *  @function
 *      Q3ColorARGB_Set
 *  @discussion
 *      Set an ARGB color.
 *
 *      Available in inline form as Q3FastColorARGB_Set.
 *
 *  @param color            The color to set.
 *  @param a                The alpha component for the color.
 *  @param r                The red component for the color.
 *  @param g                The green component for the color.
 *  @param b                The blue component for the color.
 *  @result                 Convenience pointer to the result.
 */
Q3_EXTERN_API_C ( TQ3ColorARGB * )
Q3ColorARGB_Set (
    TQ3ColorARGB                  *color,
    float                         a,
    float                         r,
    float                         g,
    float                         b
);



/*!
 *  @function
 *      Q3ColorARGB_Add
 *  @discussion
 *      Add c1 to c2.
 *
 *      Available in inline form as Q3FastColorARGB_Add.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param c1               The first color.
 *  @param c2               The second color.
 *  @param result           Receives the sum of c1 and c2.
 *  @result                 Convenience pointer to the result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ColorARGB * )
Q3ColorARGB_Add (
    const TQ3ColorARGB            *c1,
    const TQ3ColorARGB            *c2,
    TQ3ColorARGB                  *result
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3ColorARGB_Subtract
 *  @discussion
 *      Subtract c1 from c2.
 *
 *      Available in inline form as Q3FastColorARGB_Subtract.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param c1               The first color.
 *  @param c2               The second color.
 *  @param result           Receives c2 minus c1.
 *  @result                 Convenience pointer to the result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ColorARGB * )
Q3ColorARGB_Subtract (
    const TQ3ColorARGB            *c1,
    const TQ3ColorARGB            *c2,
    TQ3ColorARGB                  *result
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3ColorARGB_Scale
 *  @discussion
 *      Scale a color.
 *
 *      Available in inline form as Q3FastColorARGB_Scale.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param color            The color to scale.
 *  @param scale            The factor to scale by.
 *  @param result           Receives the scaled color.
 *  @result                 Convenience pointer to the result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ColorARGB * )
Q3ColorARGB_Scale (
    const TQ3ColorARGB            *color,
    float                         scale,
    TQ3ColorARGB                  *result
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3ColorARGB_Clamp
 *  @discussion
 *      Clamp the components of a color to within 0.0 to 1.0.
 *
 *      Available in inline form as Q3FastColorARGB_Clamp.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param color            The color to clamp.
 *  @param result           Receives the clamped color.
 *  @result                 Convenience pointer to the result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ColorARGB * )
Q3ColorARGB_Clamp (
    const TQ3ColorARGB            *color,
    TQ3ColorARGB                  *result
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3ColorARGB_Lerp
 *  @discussion
 *      Linearly interpolate between two colors.
 *
 *      Available in inline form as Q3FastColorARGB_Lerp.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param first            The first color.
 *  @param last             The second color.
 *  @param alpha            The blend factor to interpolate by.
 *  @param result           Receives the interpolated color.
 *  @result                 Convenience pointer to the result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ColorARGB * )
Q3ColorARGB_Lerp (
    const TQ3ColorARGB            *first,
    const TQ3ColorARGB            *last,
    float                         alpha,
    TQ3ColorARGB                  *result
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3ColorARGB_Accumulate
 *  @discussion
 *      Add a colour to an existing color.
 *
 *      The result is not clamped, and its components may increase past 1.0.
 *
 *      Available in inline form as Q3FastColorARGB_Accumulate.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param src              The color to accumulate.
 *  @param result           The color to accumulate into.
 *  @result                 Convenience pointer to the result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ColorARGB * )
Q3ColorARGB_Accumulate (
    const TQ3ColorARGB            *src,
    TQ3ColorARGB                  *result
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS


/*!
	@functiongroup RGB Color
*/

/*!
 *  @function
 *      Q3ColorRGB_Set
 *  @discussion
 *      Set an RGB color.
 *
 *      Available in inline form as Q3FastColorRGB_Set.
 *
 *  @param color            The color to set.
 *  @param r                The red component for the color.
 *  @param g                The green component for the color.
 *  @param b                The blue component for the color.
 *  @result                 Convenience pointer to the result.
 */
Q3_EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Set (
    TQ3ColorRGB                   *color,
    float                         r,
    float                         g,
    float                         b
);



/*!
 *  @function
 *      Q3ColorRGB_Add
 *  @discussion
 *      Add c1 to c2.
 *
 *      Available in inline form as Q3FastColorRGB_Add.
 *
 *  @param c1               The first color.
 *  @param c2               The second color.
 *  @param result           Receives the sum of c1 and c2.
 *  @result                 Convenience pointer to the result.
 */
Q3_EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Add (
    const TQ3ColorRGB             *c1,
    const TQ3ColorRGB             *c2,
    TQ3ColorRGB                   *result
);



/*!
 *  @function
 *      Q3ColorRGB_Subtract
 *  @discussion
 *      Subtract c1 from c2.
 *
 *      Available in inline form as Q3FastColorRGB_Subtract.
 *
 *  @param c1               The first color.
 *  @param c2               The second color.
 *  @param result           Receives c2 minus c1.
 *  @result                 Convenience pointer to the result.
 */
Q3_EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Subtract (
    const TQ3ColorRGB             *c1,
    const TQ3ColorRGB             *c2,
    TQ3ColorRGB                   *result
);



/*!
 *  @function
 *      Q3ColorRGB_Scale
 *  @discussion
 *      Scale a color.
 *
 *      Available in inline form as Q3FastColorRGB_Scale.
 *
 *  @param color            The color to scale.
 *  @param scale            The factor to scale by.
 *  @param result           Receives the scaled color.
 *  @result                 Convenience pointer to the result.
 */
Q3_EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Scale (
    const TQ3ColorRGB             *color,
    float                         scale,
    TQ3ColorRGB                   *result
);



/*!
 *  @function
 *      Q3ColorRGB_Clamp
 *  @discussion
 *      Clamp the components of a color to within 0.0 to 1.0.
 *
 *      Available in inline form as Q3FastColorRGB_Clamp.
 *
 *  @param color            The color to clamp.
 *  @param result           Receives the clamped color.
 *  @result                 Convenience pointer to the result.
 */
Q3_EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Clamp (
    const TQ3ColorRGB             *color,
    TQ3ColorRGB                   *result
);



/*!
 *  @function
 *      Q3ColorRGB_Lerp
 *  @discussion
 *      Linearly interpolate between two colors.
 *
 *      Available in inline form as Q3FastColorRGB_Lerp.
 *
 *  @param first            The first color.
 *  @param last             The second color.
 *  @param alpha            The blend factor to interpolate by.
 *  @param result           Receives the interpolated color.
 *  @result                 Convenience pointer to the result.
 */
Q3_EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Lerp (
    const TQ3ColorRGB             *first,
    const TQ3ColorRGB             *last,
    float                         alpha,
    TQ3ColorRGB                   *result
);



/*!
 *  @function
 *      Q3ColorRGB_Accumulate
 *  @discussion
 *      Add a colour to an existing color.
 *
 *      The result is not clamped, and its components may increase past 1.0.
 *
 *      Available in inline form as Q3FastColorRGB_Accumulate.
 *
 *  @param src              The color to accumulate.
 *  @param result           The color to accumulate into.
 *  @result                 Convenience pointer to the result.
 */
Q3_EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Accumulate (
    const TQ3ColorRGB             *src,
    TQ3ColorRGB                   *result
);



/*!
 *  @function
 *      Q3ColorRGB_Luminance
 *  @discussion
 *      Calculate the luminance of a color.
 *
 *      The luminance is calculated with the factors used by QD3D, i.e.,:
 *
 *      <tt>l = (r * 0.30078125f) +</tt>
 *      <tt>    (g * 0.58984375f) +</tt>
 *      <tt>    (b * 0.109375f)</tt>
 *
 *      Available in inline form as Q3FastColorRGB_Luminance.
 *
 *  @param color            The color whose luminance is to be calculated.
 *  @param luminance        Receives the luminance value for the color.
 *  @result                 Convenience pointer to the result.
 */
Q3_EXTERN_API_C ( float * )
Q3ColorRGB_Luminance (
    const TQ3ColorRGB             *color,
    float                         *luminance
);


/*!
	@functiongroup Shaders
*/

/*!
 *  @function
 *      Q3Shader_GetType
 *  @discussion
 *      Get the type of a shader object.
 *
 *  @param shader           The shader to query.
 *  @result                 The type of the shader.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Shader_GetType (
    TQ3ShaderObject               shader
);



/*!
 *  @function
 *      Q3Shader_Submit
 *  @discussion
 *      Submit a shader to a view.
 *
 *  @param shader           The shader to submit.
 *  @param view             The view to submit the shader to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shader_Submit (
    TQ3ShaderObject               shader,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Shader_SetUVTransform
 *  @discussion
 *      Set the UV transform for a shader.
 *
 *  @param shader           The shader to update.
 *  @param uvTransform      The new UV transform for the shader.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shader_SetUVTransform (
    TQ3ShaderObject               shader,
    const TQ3Matrix3x3            *uvTransform
);



/*!
 *  @function
 *      Q3Shader_GetUVTransform
 *  @discussion
 *      Get the UV transform for a shader.
 *
 *  @param shader           The shader to query.
 *  @param uvTransform      Receives the UV transform of the shader.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shader_GetUVTransform (
    TQ3ShaderObject               shader,
    TQ3Matrix3x3                  *uvTransform
);



/*!
 *  @function
 *      Q3Shader_SetUBoundary
 *  @discussion
 *      Set the U boundary behaviour for a shader.
 *
 *      The initial U boundary behaviour for a shader is kQ3ShaderUVBoundaryWrap.
 *
 *  @param shader           The shader to update.
 *  @param uBoundary        The U boundary behaviour for the shader.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shader_SetUBoundary (
    TQ3ShaderObject               shader,
    TQ3ShaderUVBoundary           uBoundary
);



/*!
 *  @function
 *      Q3Shader_SetVBoundary
 *  @discussion
 *      Set the V boundary behaviour for a shader.
 *
 *      The initial V boundary behaviour for a shader is kQ3ShaderUVBoundaryWrap.
 *
 *  @param shader           The shader to update.
 *  @param vBoundary        The V boundary behaviour for the shader.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shader_SetVBoundary (
    TQ3ShaderObject               shader,
    TQ3ShaderUVBoundary           vBoundary
);



/*!
 *  @function
 *      Q3Shader_GetUBoundary
 *  @discussion
 *      Get the U boundary behaviour of a shader.
 *
 *  @param shader           The shader to query.
 *  @param uBoundary        Receives the U boundary behaviour of the shader.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shader_GetUBoundary (
    TQ3ShaderObject               shader,
    TQ3ShaderUVBoundary           *uBoundary
);



/*!
 *  @function
 *      Q3Shader_GetVBoundary
 *  @discussion
 *      Get the V boundary behaviour of a shader.
 *
 *  @param shader           The shader to query.
 *  @param vBoundary        Receives the V boundary behaviour of the shader.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shader_GetVBoundary (
    TQ3ShaderObject               shader,
    TQ3ShaderUVBoundary           *vBoundary
);


/*!
	@functiongroup Illumination Shaders
*/

/*!
 *  @function
 *      Q3IlluminationShader_GetType
 *  @discussion
 *      Get the type of an illumination shader.
 *
 *  @param shader           The shader to query.
 *  @result                 The type of the illumination shader.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3IlluminationShader_GetType (
    TQ3ShaderObject               shader
);



/*!
 *  @function
 *      Q3PhongIllumination_New
 *  @discussion
 *      Create a Phong illumination shader.
 *
 *  @result                 The new illumination shader.
 */
Q3_EXTERN_API_C ( TQ3ShaderObject  )
Q3PhongIllumination_New (
    void
);



/*!
 *  @function
 *      Q3LambertIllumination_New
 *  @discussion
 *      Create a Lambert illumination shader.
 *
 *  @result                 The new illumination shader.
 */
Q3_EXTERN_API_C ( TQ3ShaderObject  )
Q3LambertIllumination_New (
    void
);



/*!
 *  @function
 *      Q3NULLIllumination_New
 *  @discussion
 *      Create a NULL illumination shader.
 *
 *  @result                 The new illumination shader.
 */
Q3_EXTERN_API_C ( TQ3ShaderObject  )
Q3NULLIllumination_New (
    void
);


/*!
	@functiongroup Surface Shaders
*/

/*!
 *  @function
 *      Q3SurfaceShader_GetType
 *  @discussion
 *      Get the type of a surface shader.
 *
 *  @param shader           The shader to query.
 *  @result                 The type of the surface shader.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3SurfaceShader_GetType (
    TQ3SurfaceShaderObject        shader
);



/*!
 *  @function
 *      Q3TextureShader_New
 *  @discussion
 *      Create a texture shader.
 *
 *		A texture shader with a NULL texture serves to override a texture shader
 *		at a higher level of the hierarchy, causing geometries that it governs
 *		to be untextured.
 *
 *  @param texture          The texture object for the shader.
 *  @result                 The new texture shader, or NULL.
 */
Q3_EXTERN_API_C ( TQ3ShaderObject  )
Q3TextureShader_New (
    TQ3TextureObject              texture
);



/*!
 *  @function
 *      Q3TextureShader_GetTexture
 *  @discussion
 *      Get the texture from a texture shader.
 *
 *      The reference count of the returned shader is incremented, and the
 *      caller must release their reference when done.
 *
 *  @param shader           The shader to query.
 *  @param texture          Receives the texture from the texture shader.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TextureShader_GetTexture (
    TQ3ShaderObject               shader,
    TQ3TextureObject              *texture
);



/*!
 *  @function
 *      Q3TextureShader_SetTexture
 *  @discussion
 *      Set the texture for a texture shader.
 *
 *  @param shader           The shader to update.
 *  @param texture          The texture for the shader, or NULL.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TextureShader_SetTexture (
    TQ3ShaderObject               shader,
    TQ3TextureObject              texture
);

/*!
	@functiongroup Textures
*/


/*!
 *  @function
 *      Q3Texture_GetType
 *  @discussion
 *      Get the type of a texture object.
 *
 *  @param texture          The texture to query.
 *  @result                 The type of the texture object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Texture_GetType (
    TQ3TextureObject              texture
);



/*!
 *  @function
 *      Q3Texture_GetWidth
 *  @discussion
 *      Get the width of a texture object.
 *
 *  @param texture          The texture to query.
 *  @param width            Receives the width of the texture object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Texture_GetWidth (
    TQ3TextureObject              texture,
    TQ3Uns32                      *width
);



/*!
 *  @function
 *      Q3Texture_GetHeight
 *  @discussion
 *      Get the height of a texture object.
 *
 *  @param texture          The texture to query.
 *  @param height           Receives the height of the texture object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Texture_GetHeight (
    TQ3TextureObject              texture,
    TQ3Uns32                      *height
);

/*!
	@functiongroup Pixmap Textures
*/


/*!
 *  @function
 *      Q3PixmapTexture_New
 *  @discussion
 *      Create a pixmap texture.
 *
 *  @param pixmap           The pixmap to be used by the texture.
 *  @result                 The new pixmap texture.
 */
Q3_EXTERN_API_C ( TQ3TextureObject  )
Q3PixmapTexture_New (
    const TQ3StoragePixmap        *pixmap
);



/*!
 *  @function
 *      Q3PixmapTexture_GetPixmap
 *  @discussion
 *      Get the pixmap from a pixmap texture.
 *
 *      The reference count of the returned image storage is incremented, and the
 *      caller must release their reference when done.
 *
 *  @param texture          The texture to query.
 *  @param pixmap           Receives the pixmap of the texture.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapTexture_GetPixmap (
    TQ3TextureObject              texture,
    TQ3StoragePixmap              *pixmap
);



/*!
 *  @function
 *      Q3PixmapTexture_SetPixmap
 *  @discussion
 *      Set the pixmap for a pixmap texture.
 *
 *  @param texture          The texture to update.
 *  @param pixmap           The new pixmap for the texture.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapTexture_SetPixmap (
    TQ3TextureObject              texture,
    const TQ3StoragePixmap        *pixmap
);

/*!
	@functiongroup Mipmap Textures
*/


/*!
 *  @function
 *      Q3MipmapTexture_New
 *  @discussion
 *      Create a mipmap texture.
 *
 *  @param mipmap           The mipmap to be used by the texture.
 *  @result                 The new mipmap texture.
 */
Q3_EXTERN_API_C ( TQ3TextureObject  )
Q3MipmapTexture_New (
    const TQ3Mipmap               *mipmap
);



/*!
 *  @function
 *      Q3MipmapTexture_GetMipmap
 *  @discussion
 *      Get the mipmap from a mipmap texture.
 *
 *      The reference count of the returned image storage is incremented, and the
 *      caller must release their reference when done.
 *
 *  @param texture          The texture to query.
 *  @param mipmap           Receives the mipmap of the texture.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MipmapTexture_GetMipmap (
    TQ3TextureObject              texture,
    TQ3Mipmap                     *mipmap
);



/*!
 *  @function
 *      Q3MipmapTexture_SetMipmap
 *  @discussion
 *      Set the mipmap for a mipmap texture.
 *
 *  @param texture          The texture to update.
 *  @param mipmap           The new mipmap for the texture.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MipmapTexture_SetMipmap (
    TQ3TextureObject              texture,
    const TQ3Mipmap               *mipmap
);

/*!
	@functiongroup Compressed Pixmap Textures
*/


#if QUESA_SUPPORT_QUICKTIME
/*!
 *  @function
 *      Q3CompressedPixmapTexture_New
 *  @discussion
 *      Create a compressed pixmap texture.
 *
 *      This function is only available on platforms that support QuickTime.
 *
 *  @param compressedPixmap The compressed pixmap for the texture.
 *  @result                 The new compressed pixmap texture.
 */
Q3_EXTERN_API_C ( TQ3TextureObject  )
Q3CompressedPixmapTexture_New (
    const TQ3CompressedPixmap     *compressedPixmap
);



/*!
 *  @function
 *      Q3CompressedPixmapTexture_GetCompressedPixmap
 *  @discussion
 *      Get the compressed pixmap from a compressed pixmap texture.
 *
 *      This function is only available on platforms that support QuickTime.
 *
 *      The reference counts of the returned compressedImage and imageDesc
 *		fields are incremented, and the  caller must release their references
 *		when done.
 *
 *  @param texture          The texture to query.
 *  @param compressedPixmap Receives the compressed pixmap of the texture.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CompressedPixmapTexture_GetCompressedPixmap (
    TQ3TextureObject              texture,
    TQ3CompressedPixmap           *compressedPixmap
);



/*!
 *  @function
 *      Q3CompressedPixmapTexture_SetCompressedPixmap
 *  @discussion
 *      Set the compressed pixmap for a compressed pixmap texture.
 *
 *      This function is only available on platforms that support QuickTime.
 *
 *  @param texture          The texture to update.
 *  @param compressedPixmap The new compressed pixmap for the texture.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CompressedPixmapTexture_SetCompressedPixmap (
    TQ3TextureObject              texture,
    const TQ3CompressedPixmap     *compressedPixmap
);



/*!
 *  @function
 *      Q3CompressedPixmapTexture_CompressImage
 *  @discussion
 *      Create a compressed pixmap from a PixMapHandle.
 *
 *      This function is only available on platforms that support QuickTime.
 *
 *  @param compressedPixmap Receives the compressed pixmap.
 *  @param sourcePixMap     The uncompressed PixMapHandle.
 *  @param codecType        The QuickTime codec to use for compression.
 *  @param codecComponent   The QuickTime component to use for compression.
 *  @param codedDepth       The depth the codec should compress to.
 *  @param codecQuality     The quality the codec should compress to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CompressedPixmapTexture_CompressImage (
    TQ3CompressedPixmap           *compressedPixmap,
    PixMapHandle                  sourcePixMap,
    CodecType                     codecType,
    CodecComponent                codecComponent,
    TQ3Int16                      codedDepth,
    CodecQ                        codecQuality
);

#endif // QUESA_SUPPORT_QUICKTIME





//=============================================================================
//      Inline APIs
//-----------------------------------------------------------------------------
//		Note : Preserve the order of functions when adding new inline APIs.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS

// Implementation
#define __Q3FastColorARGB_Set(_c, _a, _r, _g, _b)							\
	do																		\
		{																	\
		(_c)->a = (_a);														\
		(_c)->r = (_r);														\
		(_c)->g = (_g);														\
		(_c)->b = (_b);														\
		}																	\
	while (0)

#define __Q3FastColorARGB_Add(_c1, _c2, _r)									\
	do																		\
		{																	\
		(_r)->a = (_c1)->a + (_c2)->a;										\
		(_r)->r = (_c1)->r + (_c2)->r;										\
		(_r)->g = (_c1)->g + (_c2)->g;										\
		(_r)->b = (_c1)->b + (_c2)->b;										\
		}																	\
	while (0)

#define __Q3FastColorARGB_Subtract(_c1, _c2, _r)							\
	do																		\
		{																	\
		(_r)->a = (_c2)->a - (_c1)->a;										\
		(_r)->r = (_c2)->r - (_c1)->r;										\
		(_r)->g = (_c2)->g - (_c1)->g;										\
		(_r)->b = (_c2)->b - (_c1)->b;										\
		}																	\
	while (0)

#define __Q3FastColorARGB_Scale(_c, _s, _r)									\
	do																		\
		{																	\
		(_r)->a = (_c)->a * (_s);											\
		(_r)->r = (_c)->r * (_s);											\
		(_r)->g = (_c)->g * (_s);											\
		(_r)->b = (_c)->b * (_s);											\
		}																	\
	while (0)

#define __Q3FastColorARGB_Clamp(_c, _r)										\
	do																		\
		{																	\
		float ca = (_c)->a;													\
		float cr = (_c)->r;													\
		float cg = (_c)->g;													\
		float cb = (_c)->b;													\
																			\
		(_r)->a = (ca < 0.0f ? 0.0f : (ca > 1.0f ? 1.0f : ca));				\
		(_r)->r = (cr < 0.0f ? 0.0f : (cr > 1.0f ? 1.0f : cr));				\
		(_r)->g = (cg < 0.0f ? 0.0f : (cg > 1.0f ? 1.0f : cg));				\
		(_r)->b = (cb < 0.0f ? 0.0f : (cb > 1.0f ? 1.0f : cb));				\
		}																	\
	while (0)

#define __Q3FastColorARGB_Lerp(_f, _l, _a, _r)								\
	do																		\
		{																	\
		(_r)->a = (_f)->a + ( ((_l)->a - (_f)->a) * (_a) );					\
		(_r)->r = (_f)->r + ( ((_l)->r - (_f)->r) * (_a) );					\
		(_r)->g = (_f)->g + ( ((_l)->g - (_f)->g) * (_a) );					\
		(_r)->b = (_f)->b + ( ((_l)->b - (_f)->b) * (_a) );					\
		}																	\
	while (0)

#define __Q3FastColorARGB_Accumulate(_c, _r)								\
	do																		\
		{																	\
		(_r)->a += (_c)->a;													\
		(_r)->r += (_c)->r;													\
		(_r)->g += (_c)->g;													\
		(_r)->b += (_c)->b;													\
		}																	\
	while (0)

#define __Q3FastColorRGB_Set(_c, _r, _g, _b)								\
	do																		\
		{																	\
		(_c)->r = (_r);														\
		(_c)->g = (_g);														\
		(_c)->b = (_b);														\
		}																	\
	while (0)

#define __Q3FastColorRGB_Add(_c1, _c2, _r)									\
	do																		\
		{																	\
		(_r)->r = (_c1)->r + (_c2)->r;										\
		(_r)->g = (_c1)->g + (_c2)->g;										\
		(_r)->b = (_c1)->b + (_c2)->b;										\
		}																	\
	while (0)

#define __Q3FastColorRGB_Subtract(_c1, _c2, _r)								\
	do																		\
		{																	\
		(_r)->r = (_c2)->r - (_c1)->r;										\
		(_r)->g = (_c2)->g - (_c1)->g;										\
		(_r)->b = (_c2)->b - (_c1)->b;										\
		}																	\
	while (0)

#define __Q3FastColorRGB_Scale(_c, _s, _r)									\
	do																		\
		{																	\
		(_r)->r = (_c)->r * (_s);											\
		(_r)->g = (_c)->g * (_s);											\
		(_r)->b = (_c)->b * (_s);											\
		}																	\
	while (0)

#define __Q3FastColorRGB_Clamp(_c, _r)										\
	do																		\
		{																	\
		float cr = (_c)->r;													\
		float cg = (_c)->g;													\
		float cb = (_c)->b;													\
																			\
		(_r)->r = (cr < 0.0f ? 0.0f : (cr > 1.0f ? 1.0f : cr));				\
		(_r)->g = (cg < 0.0f ? 0.0f : (cg > 1.0f ? 1.0f : cg));				\
		(_r)->b = (cb < 0.0f ? 0.0f : (cb > 1.0f ? 1.0f : cb));				\
		}																	\
	while (0)

#define __Q3FastColorRGB_Lerp(_f, _l, _a, _r)								\
	do																		\
		{																	\
		(_r)->r = (_f)->r + ( ((_l)->r - (_f)->r) * (_a) );					\
		(_r)->g = (_f)->g + ( ((_l)->g - (_f)->g) * (_a) );					\
		(_r)->b = (_f)->b + ( ((_l)->b - (_f)->b) * (_a) );					\
		}																	\
	while (0)

#define __Q3FastColorRGB_Accumulate(_c, _r)									\
	do																		\
		{																	\
		(_r)->r += (_c)->r;													\
		(_r)->g += (_c)->g;													\
		(_r)->b += (_c)->b;													\
		}																	\
	while (0)

#define __Q3FastColorRGB_Luminance(_c, _l)									\
	do																		\
		{																	\
		*(_l) = ((_c)->r * 0.30078125f) +									\
				((_c)->g * 0.58984375f) +									\
				((_c)->b * 0.109375f);										\
		}																	\
	while (0)



// Wrappers
#ifdef __cplusplus

	inline TQ3ColorARGB *Q3FastColorARGB_Set(TQ3ColorARGB *color, float a, float r, float g, float b)
	{
		__Q3FastColorARGB_Set(color, a, r, g, b);
		return(color);
	}
	
	inline TQ3ColorARGB *Q3FastColorARGB_Add(const TQ3ColorARGB *c1, const TQ3ColorARGB *c2, TQ3ColorARGB *result)
	{
		__Q3FastColorARGB_Add(c1, c2, result);
		return(result);
	}
	
	inline TQ3ColorARGB *Q3FastColorARGB_Subtract(const TQ3ColorARGB *c1, const TQ3ColorARGB *c2, TQ3ColorARGB *result)
	{
		__Q3FastColorARGB_Subtract(c1, c2, result);
		return(result);
	}
	
	inline TQ3ColorARGB *Q3FastColorARGB_Scale(const TQ3ColorARGB *color, float scale, TQ3ColorARGB *result)
	{
		__Q3FastColorARGB_Scale(color, scale, result);
		return(result);
	}
	
	inline TQ3ColorARGB *Q3FastColorARGB_Clamp(const TQ3ColorARGB *color, TQ3ColorARGB *result)
	{
		__Q3FastColorARGB_Clamp(color, result);
		return(result);
	}
	
	inline TQ3ColorARGB *Q3FastColorARGB_Lerp(const TQ3ColorARGB *first, const TQ3ColorARGB *last, float alpha, TQ3ColorARGB *result)
	{
		__Q3FastColorARGB_Lerp(first, last, alpha, result);
		return(result);
	}
	
	inline TQ3ColorARGB *Q3FastColorARGB_Accumulate(const TQ3ColorARGB *src, TQ3ColorARGB *result)
	{
		__Q3FastColorARGB_Accumulate(src, result);
		return(result);
	}

	inline TQ3ColorRGB *Q3FastColorRGB_Set(TQ3ColorRGB *color, float r, float g, float b)
	{
		__Q3FastColorRGB_Set(color, r, g, b);
		return(color);
	}
	
	inline TQ3ColorRGB *Q3FastColorRGB_Add(const TQ3ColorRGB *c1, const TQ3ColorRGB *c2, TQ3ColorRGB *result)
	{
		__Q3FastColorRGB_Add(c1, c2, result);
		return(result);
	}
	
	inline TQ3ColorRGB *Q3FastColorRGB_Subtract(const TQ3ColorRGB *c1, const TQ3ColorRGB *c2, TQ3ColorRGB *result)
	{
		__Q3FastColorRGB_Subtract(c1, c2, result);
		return(result);
	}
	
	inline TQ3ColorRGB *Q3FastColorRGB_Scale(const TQ3ColorRGB *color, float scale, TQ3ColorRGB *result)
	{
		__Q3FastColorRGB_Scale(color, scale, result);
		return(result);
	}
	
	inline TQ3ColorRGB *Q3FastColorRGB_Clamp(const TQ3ColorRGB *color, TQ3ColorRGB *result)
	{
		__Q3FastColorRGB_Clamp(color, result);
		return(result);
	}
	
	inline TQ3ColorRGB *Q3FastColorRGB_Lerp(const TQ3ColorRGB *first, const TQ3ColorRGB *last, float alpha, TQ3ColorRGB *result)
	{
		__Q3FastColorRGB_Lerp(first, last, alpha, result);
		return(result);
	}
	
	inline TQ3ColorRGB *Q3FastColorRGB_Accumulate(const TQ3ColorRGB *src, TQ3ColorRGB *result)
	{
		__Q3FastColorRGB_Accumulate(src, result);
		return(result);
	}
	
	inline float *Q3FastColorRGB_Luminance(const TQ3ColorRGB *color, float *luminance)
	{
		__Q3FastColorRGB_Luminance(color, luminance);
		return(luminance);
	}

#else
	#define Q3FastColorARGB_Set							__Q3FastColorARGB_Set
	#define Q3FastColorARGB_Add							__Q3FastColorARGB_Add
	#define Q3FastColorARGB_Subtract					__Q3FastColorARGB_Subtract
	#define Q3FastColorARGB_Scale						__Q3FastColorARGB_Scale
	#define Q3FastColorARGB_Clamp						__Q3FastColorARGB_Clamp
	#define Q3FastColorARGB_Lerp						__Q3FastColorARGB_Lerp
	#define Q3FastColorARGB_Accumulate					__Q3FastColorARGB_Accumulate
	#define Q3FastColorRGB_Set							__Q3FastColorRGB_Set
	#define Q3FastColorRGB_Add							__Q3FastColorRGB_Add
	#define Q3FastColorRGB_Subtract						__Q3FastColorRGB_Subtract
	#define Q3FastColorRGB_Scale						__Q3FastColorRGB_Scale
	#define Q3FastColorRGB_Clamp						__Q3FastColorRGB_Clamp
	#define Q3FastColorRGB_Lerp							__Q3FastColorRGB_Lerp
	#define Q3FastColorRGB_Accumulate					__Q3FastColorRGB_Accumulate
	#define Q3FastColorRGB_Luminance					__Q3FastColorRGB_Luminance
#endif



// Redirection
//
// Unfortunately the Q3Color[A]RGB_Set functions are allowed to be called with either
// individual arguments or with a macro such as kQ3ViewDefaultDiffuseColor. We can
// support this using __VA_ARGS__, however this is not supported by all compilers.
//
// For gcc we have a workaround, for Visual C++ we do not - so for now only CW/gcc can
// automatically remap these two Q3 calls to their Q3Fast equivalents.
#if QUESA_ALLOW_INLINE_APIS
	#ifdef __MWERKS__
		#define Q3ColorARGB_Set(...)					Q3FastColorARGB_Set(__VA_ARGS__)
		#define Q3ColorRGB_Set(...)						Q3FastColorRGB_Set(__VA_ARGS__)
	#endif

	#ifdef __GNUC__
		#define Q3ColorARGB_Set(args...)				Q3FastColorARGB_Set(args)
		#define Q3ColorRGB_Set(args...)					Q3FastColorRGB_Set(args)
	#endif

	#define Q3ColorARGB_Add								Q3FastColorARGB_Add
	#define Q3ColorARGB_Subtract						Q3FastColorARGB_Subtract
	#define Q3ColorARGB_Scale							Q3FastColorARGB_Scale
	#define Q3ColorARGB_Clamp							Q3FastColorARGB_Clamp
	#define Q3ColorARGB_Lerp							Q3FastColorARGB_Lerp
	#define Q3ColorARGB_Accumulate						Q3FastColorARGB_Accumulate
	#define Q3ColorRGB_Add								Q3FastColorRGB_Add
	#define Q3ColorRGB_Subtract							Q3FastColorRGB_Subtract
	#define Q3ColorRGB_Scale							Q3FastColorRGB_Scale
	#define Q3ColorRGB_Clamp							Q3FastColorRGB_Clamp
	#define Q3ColorRGB_Lerp								Q3FastColorRGB_Lerp
	#define Q3ColorRGB_Accumulate						Q3FastColorRGB_Accumulate
	#define Q3ColorRGB_Luminance						Q3FastColorRGB_Luminance
#endif

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



// Work around a HeaderDoc bug
/*!
	@functiongroup
*/



//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



/*! @header QuesaShader.h
        Declares the Quesa shader objects.
 */
/*  NAME:
        QuesaShader.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif


#pragma enumsalwaysint on



//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Shader boundary types
typedef enum {
    kQ3ShaderUVBoundaryWrap                     = 0,
    kQ3ShaderUVBoundaryClamp                    = 1
} TQ3ShaderUVBoundary;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3ColorRGB_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param color            Description of the parameter.
 *  @param r                Description of the parameter.
 *  @param g                Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Set (
    TQ3ColorRGB                   *color,
    float                         r,
    float                         g,
    float                         b
);



/*!
 *  @function
 *      Q3ColorARGB_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param color            Description of the parameter.
 *  @param a                Description of the parameter.
 *  @param r                Description of the parameter.
 *  @param g                Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ColorARGB * )
Q3ColorARGB_Set (
    TQ3ColorARGB                  *color,
    float                         a,
    float                         r,
    float                         g,
    float                         b
);



/*!
 *  @function
 *      Q3ColorRGB_Add
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param c1               Description of the parameter.
 *  @param c2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Add (
    const TQ3ColorRGB             *c1,
    const TQ3ColorRGB             *c2,
    TQ3ColorRGB                   *result
);



/*!
 *  @function
 *      Q3ColorRGB_Subtract
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param c1               Description of the parameter.
 *  @param c2               Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Subtract (
    const TQ3ColorRGB             *c1,
    const TQ3ColorRGB             *c2,
    TQ3ColorRGB                   *result
);



/*!
 *  @function
 *      Q3ColorRGB_Scale
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param color            Description of the parameter.
 *  @param scale            Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Scale (
    const TQ3ColorRGB             *color,
    float                         scale,
    TQ3ColorRGB                   *result
);



/*!
 *  @function
 *      Q3ColorRGB_Clamp
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param color            Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Clamp (
    const TQ3ColorRGB             *color,
    TQ3ColorRGB                   *result
);



/*!
 *  @function
 *      Q3ColorRGB_Lerp
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param first            Description of the parameter.
 *  @param last             Description of the parameter.
 *  @param alpha            Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ColorRGB * )
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param src              Description of the parameter.
 *  @param result           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Accumulate (
    const TQ3ColorRGB             *src,
    TQ3ColorRGB                   *result
);



/*!
 *  @function
 *      Q3ColorRGB_Luminance
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param color            Description of the parameter.
 *  @param luminance        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( float * )
Q3ColorRGB_Luminance (
    const TQ3ColorRGB             *color,
    float                         *luminance
);



/*!
 *  @function
 *      Q3Shader_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Shader_GetType (
    TQ3ShaderObject               shader
);



/*!
 *  @function
 *      Q3Shader_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_Submit (
    TQ3ShaderObject               shader,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Shader_SetUVTransform
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @param uvTransform      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_SetUVTransform (
    TQ3ShaderObject               shader,
    const TQ3Matrix3x3            *uvTransform
);



/*!
 *  @function
 *      Q3Shader_GetUVTransform
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @param uvTransform      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_GetUVTransform (
    TQ3ShaderObject               shader,
    TQ3Matrix3x3                  *uvTransform
);



/*!
 *  @function
 *      Q3Shader_SetUBoundary
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @param uBoundary        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_SetUBoundary (
    TQ3ShaderObject               shader,
    TQ3ShaderUVBoundary           uBoundary
);



/*!
 *  @function
 *      Q3Shader_SetVBoundary
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @param vBoundary        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_SetVBoundary (
    TQ3ShaderObject               shader,
    TQ3ShaderUVBoundary           vBoundary
);



/*!
 *  @function
 *      Q3Shader_GetUBoundary
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @param uBoundary        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_GetUBoundary (
    TQ3ShaderObject               shader,
    TQ3ShaderUVBoundary           *uBoundary
);



/*!
 *  @function
 *      Q3Shader_GetVBoundary
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @param vBoundary        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_GetVBoundary (
    TQ3ShaderObject               shader,
    TQ3ShaderUVBoundary           *vBoundary
);



/*!
 *  @function
 *      Q3IlluminationShader_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3IlluminationShader_GetType (
    TQ3ShaderObject               shader
);



/*!
 *  @function
 *      Q3PhongIllumination_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ShaderObject  )
Q3PhongIllumination_New (
    void
);



/*!
 *  @function
 *      Q3LambertIllumination_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ShaderObject  )
Q3LambertIllumination_New (
    void
);



/*!
 *  @function
 *      Q3NULLIllumination_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ShaderObject  )
Q3NULLIllumination_New (
    void
);



/*!
 *  @function
 *      Q3SurfaceShader_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3SurfaceShader_GetType (
    TQ3SurfaceShaderObject        shader
);



/*!
 *  @function
 *      Q3TextureShader_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param texture          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ShaderObject  )
Q3TextureShader_New (
    TQ3TextureObject              texture
);



/*!
 *  @function
 *      Q3TextureShader_GetTexture
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @param texture          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TextureShader_GetTexture (
    TQ3ShaderObject               shader,
    TQ3TextureObject              *texture
);



/*!
 *  @function
 *      Q3TextureShader_SetTexture
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shader           Description of the parameter.
 *  @param texture          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TextureShader_SetTexture (
    TQ3ShaderObject               shader,
    TQ3TextureObject              texture
);



/*!
 *  @function
 *      Q3Texture_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param texture          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Texture_GetType (
    TQ3TextureObject              texture
);



/*!
 *  @function
 *      Q3Texture_GetWidth
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param texture          Description of the parameter.
 *  @param width            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Texture_GetWidth (
    TQ3TextureObject              texture,
    TQ3Uns32                      *width
);



/*!
 *  @function
 *      Q3Texture_GetHeight
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param texture          Description of the parameter.
 *  @param height           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Texture_GetHeight (
    TQ3TextureObject              texture,
    TQ3Uns32                      *height
);



/*!
 *  @function
 *      Q3PixmapTexture_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pixmap           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TextureObject  )
Q3PixmapTexture_New (
    const TQ3StoragePixmap        *pixmap
);



/*!
 *  @function
 *      Q3PixmapTexture_GetPixmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param texture          Description of the parameter.
 *  @param pixmap           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapTexture_GetPixmap (
    TQ3TextureObject              texture,
    TQ3StoragePixmap              *pixmap
);



/*!
 *  @function
 *      Q3PixmapTexture_SetPixmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param texture          Description of the parameter.
 *  @param pixmap           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapTexture_SetPixmap (
    TQ3TextureObject              texture,
    const TQ3StoragePixmap        *pixmap
);



/*!
 *  @function
 *      Q3MipmapTexture_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param mipmap           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TextureObject  )
Q3MipmapTexture_New (
    const TQ3Mipmap               *mipmap
);



/*!
 *  @function
 *      Q3MipmapTexture_GetMipmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param texture          Description of the parameter.
 *  @param mipmap           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MipmapTexture_GetMipmap (
    TQ3TextureObject              texture,
    TQ3Mipmap                     *mipmap
);



/*!
 *  @function
 *      Q3MipmapTexture_SetMipmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param texture          Description of the parameter.
 *  @param mipmap           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MipmapTexture_SetMipmap (
    TQ3TextureObject              texture,
    const TQ3Mipmap               *mipmap
);


#if QUESA_SUPPORT_QUICKTIME
/*!
 *  @function
 *      Q3CompressedPixmapTexture_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param compressedPixmap Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TextureObject  )
Q3CompressedPixmapTexture_New (
    const TQ3CompressedPixmap     *compressedPixmap
);



/*!
 *  @function
 *      Q3CompressedPixmapTexture_GetCompressedPixmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param texture          Description of the parameter.
 *  @param compressedPixmap Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3CompressedPixmapTexture_GetCompressedPixmap (
    TQ3TextureObject              texture,
    TQ3CompressedPixmap           *compressedPixmap
);



/*!
 *  @function
 *      Q3CompressedPixmapTexture_SetCompressedPixmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param texture          Description of the parameter.
 *  @param compressedPixmap Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3CompressedPixmapTexture_SetCompressedPixmap (
    TQ3TextureObject              texture,
    const TQ3CompressedPixmap     *compressedPixmap
);



/*!
 *  @function
 *      Q3CompressedPixmapTexture_CompressImage
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param compressedPixmap Description of the parameter.
 *  @param sourcePixMap     Description of the parameter.
 *  @param codecType        Description of the parameter.
 *  @param codecComponent   Description of the parameter.
 *  @param codedDepth       Description of the parameter.
 *  @param codecQuality     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3CompressedPixmapTexture_CompressImage (
    TQ3CompressedPixmap           *compressedPixmap,
    PixMapHandle                  sourcePixMap,
    CodecType                     codecType,
    CodecComponent                codecComponent,
    TQ3Int16                      codedDepth,
    CodecQ                        codecQuality
);

#endif // QUESA_SUPPORT_QUICKTIME


#pragma enumsalwaysint reset



//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



/*! @header QuesaShader.h
        Declares the Quesa shader objects.
 */
/*  NAME:
        QuesaShader.h

    DESCRIPTION:
        Quesa public header.

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
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Shader boundary types
typedef enum {
	kQ3ShaderUVBoundaryWrap						= 0,
	kQ3ShaderUVBoundaryClamp					= 1
} TQ3ShaderUVBoundary;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*
 *	Q3ColorRGB_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Set (
	TQ3ColorRGB                   *color,
	float                         r,
	float                         g,
	float                         b
);



/*
 *	Q3ColorARGB_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3ColorARGB * )
Q3ColorARGB_Set (
	TQ3ColorARGB                  *color,
	float                         a,
	float                         r,
	float                         g,
	float                         b
);



/*
 *	Q3ColorRGB_Add
 *		Description of function
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Add (
	const TQ3ColorRGB             *c1,
	const TQ3ColorRGB             *c2,
	TQ3ColorRGB                   *result
);



/*
 *	Q3ColorRGB_Subtract
 *		Description of function
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Subtract (
	const TQ3ColorRGB             *c1,
	const TQ3ColorRGB             *c2,
	TQ3ColorRGB                   *result
);



/*
 *	Q3ColorRGB_Scale
 *		Description of function
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Scale (
	const TQ3ColorRGB             *color,
	float                         scale,
	TQ3ColorRGB                   *result
);



/*
 *	Q3ColorRGB_Clamp
 *		Description of function
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Clamp (
	const TQ3ColorRGB             *color,
	TQ3ColorRGB                   *result
);



/*
 *	Q3ColorRGB_Lerp
 *		Description of function
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Lerp (
	const TQ3ColorRGB             *first,
	const TQ3ColorRGB             *last,
	float                         alpha,
	TQ3ColorRGB                   *result
);



/*
 *	Q3ColorRGB_Accumulate
 *		Description of function
 */
EXTERN_API_C ( TQ3ColorRGB * )
Q3ColorRGB_Accumulate (
	const TQ3ColorRGB             *src,
	TQ3ColorRGB                   *result
);



/*
 *	Q3ColorRGB_Luminance
 *		Description of function
 */
EXTERN_API_C ( float * )
Q3ColorRGB_Luminance (
	const TQ3ColorRGB             *color,
	float                         *luminance
);



/*
 *	Q3Shader_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Shader_GetType (
	TQ3ShaderObject               shader
);



/*
 *	Q3Shader_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_Submit (
	TQ3ShaderObject               shader,
	TQ3ViewObject                 view
);



/*
 *	Q3Shader_SetUVTransform
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_SetUVTransform (
	TQ3ShaderObject               shader,
	const TQ3Matrix3x3            *uvTransform
);



/*
 *	Q3Shader_GetUVTransform
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_GetUVTransform (
	TQ3ShaderObject               shader,
	TQ3Matrix3x3                  *uvTransform
);



/*
 *	Q3Shader_SetUBoundary
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_SetUBoundary (
	TQ3ShaderObject               shader,
	TQ3ShaderUVBoundary           uBoundary
);



/*
 *	Q3Shader_SetVBoundary
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_SetVBoundary (
	TQ3ShaderObject               shader,
	TQ3ShaderUVBoundary           vBoundary
);



/*
 *	Q3Shader_GetUBoundary
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_GetUBoundary (
	TQ3ShaderObject               shader,
	TQ3ShaderUVBoundary           *uBoundary
);



/*
 *	Q3Shader_GetVBoundary
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shader_GetVBoundary (
	TQ3ShaderObject               shader,
	TQ3ShaderUVBoundary           *vBoundary
);



/*
 *	Q3IlluminationShader_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3IlluminationShader_GetType (
	TQ3ShaderObject               shader
);



/*
 *	Q3PhongIllumination_New
 *		Description of function
 */
EXTERN_API_C ( TQ3ShaderObject  )
Q3PhongIllumination_New (
	void
);



/*
 *	Q3LambertIllumination_New
 *		Description of function
 */
EXTERN_API_C ( TQ3ShaderObject  )
Q3LambertIllumination_New (
	void
);



/*
 *	Q3NULLIllumination_New
 *		Description of function
 */
EXTERN_API_C ( TQ3ShaderObject  )
Q3NULLIllumination_New (
	void
);



/*
 *	Q3SurfaceShader_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3SurfaceShader_GetType (
	TQ3SurfaceShaderObject        shader
);



/*
 *	Q3TextureShader_New
 *		Description of function
 */
EXTERN_API_C ( TQ3ShaderObject  )
Q3TextureShader_New (
	TQ3TextureObject              texture
);



/*
 *	Q3TextureShader_GetTexture
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TextureShader_GetTexture (
	TQ3ShaderObject               shader,
	TQ3TextureObject              *texture
);



/*
 *	Q3TextureShader_SetTexture
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TextureShader_SetTexture (
	TQ3ShaderObject               shader,
	TQ3TextureObject              texture
);



/*
 *	Q3Texture_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Texture_GetType (
	TQ3TextureObject              texture
);



/*
 *	Q3Texture_GetWidth
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Texture_GetWidth (
	TQ3TextureObject              texture,
	TQ3Uns32                      *width
);



/*
 *	Q3Texture_GetHeight
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Texture_GetHeight (
	TQ3TextureObject              texture,
	TQ3Uns32                      *height
);



/*
 *	Q3PixmapTexture_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TextureObject  )
Q3PixmapTexture_New (
	const TQ3StoragePixmap        *pixmap
);



/*
 *	Q3PixmapTexture_GetPixmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapTexture_GetPixmap (
	TQ3TextureObject              texture,
	TQ3StoragePixmap              *pixmap
);



/*
 *	Q3PixmapTexture_SetPixmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapTexture_SetPixmap (
	TQ3TextureObject              texture,
	const TQ3StoragePixmap        *pixmap
);



/*
 *	Q3MipmapTexture_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TextureObject  )
Q3MipmapTexture_New (
	const TQ3Mipmap               *mipmap
);



/*
 *	Q3MipmapTexture_GetMipmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MipmapTexture_GetMipmap (
	TQ3TextureObject              texture,
	TQ3Mipmap                     *mipmap
);



/*
 *	Q3MipmapTexture_SetMipmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MipmapTexture_SetMipmap (
	TQ3TextureObject              texture,
	const TQ3Mipmap               *mipmap
);


#if QUESA_SUPPORT_QUICKTIME
/*
 *	Q3CompressedPixmapTexture_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TextureObject  )
Q3CompressedPixmapTexture_New (
	const TQ3CompressedPixmap     *compressedPixmap
);



/*
 *	Q3CompressedPixmapTexture_GetCompressedPixmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CompressedPixmapTexture_GetCompressedPixmap (
	TQ3TextureObject              texture,
	TQ3CompressedPixmap           *compressedPixmap
);



/*
 *	Q3CompressedPixmapTexture_SetCompressedPixmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CompressedPixmapTexture_SetCompressedPixmap (
	TQ3TextureObject              texture,
	const TQ3CompressedPixmap     *compressedPixmap
);



/*
 *	Q3CompressedPixmapTexture_CompressImage
 *		Description of function
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





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


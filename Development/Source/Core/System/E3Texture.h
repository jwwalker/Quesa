/*  NAME:
        E3Texture.h

    DESCRIPTION:
        Header file for E3Texture.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#ifndef __E3TEXTURE__
#define __E3TEXTURE__
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
TQ3Status				E3Texture_RegisterClass(void);
TQ3Status				E3Texture_UnregisterClass(void);

TQ3ObjectType			E3Texture_GetType(	TQ3TextureObject texture);
TQ3Status				E3Texture_GetWidth(	TQ3TextureObject texture, unsigned long *width);
TQ3Status				E3Texture_GetHeight(TQ3TextureObject texture, unsigned long *height);

TQ3TextureObject		E3PixmapTexture_New(const TQ3StoragePixmap *pixmap);
TQ3Status				E3PixmapTexture_GetPixmap(	TQ3TextureObject texture, TQ3StoragePixmap *pixmap);
TQ3Status				E3PixmapTexture_SetPixmap(	TQ3TextureObject texture, const TQ3StoragePixmap *pixmap);

TQ3TextureObject		E3MipmapTexture_New(const TQ3Mipmap *mipmap);
TQ3Status				E3MipmapTexture_GetMipmap(TQ3TextureObject texture, TQ3Mipmap *mipmap);
TQ3Status				E3MipmapTexture_SetMipmap(TQ3TextureObject texture, const TQ3Mipmap *mipmap);

TQ3TextureObject		E3CompressedPixmapTexture_New(const TQ3CompressedPixmap *compressedPixmap);
TQ3Status				E3CompressedPixmapTexture_GetCompressedPixmap(TQ3TextureObject texture, TQ3CompressedPixmap *compressedPixmap);
TQ3Status				E3CompressedPixmapTexture_SetCompressedPixmap(TQ3TextureObject texture, const TQ3CompressedPixmap *compressedPixmap);
TQ3Status				E3CompressedPixmapTexture_CompressImage(TQ3CompressedPixmap *compressedPixmap, PixMapHandle sourcePixMap, CodecType codecType, CodecComponent codecComponent, short codedDepth, CodecQ codecQuality);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


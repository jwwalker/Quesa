/*  NAME:
        IRTexture.h

    DESCRIPTION:
        Header file for IRTexture.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#ifndef IRTEXTURE_HDR
#define IRTEXTURE_HDR
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
void				IRRenderer_Texture_EndPass(
								TQ3InteractiveData		*instanceData);

void				IRRenderer_Texture_Terminate(
								TQ3InteractiveData		*instanceData);

TQ3Status			IRRenderer_Texture_Set(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3ShaderObject			theShader,
								TQ3TextureObject		theTexture);

void				IRRenderer_Texture_Rebuild(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData);

TQ3Boolean			IRRenderer_Texture_Preamble(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3AttributeSet			theAttributes);

void				IRRenderer_Texture_Postamble(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3Boolean				hadAttributeTexture,
								TQ3Boolean				couldTexture);

TQ3Uns8				*IRRenderer_Texture_GetData(
								TQ3StorageObject		theStorage,
								TQ3Boolean				*wasCopied);

void				IRRenderer_Texture_ReleaseData(
								TQ3StorageObject		theStorage,
								TQ3Uns8					*basePtr,
								TQ3Boolean				wasCopied);

TQ3Uns8				*IRRenderer_Texture_ConvertDepthAndFlip(
								TQ3Uns32				theWidth,
								TQ3Uns32				theHeight,
								TQ3Uns32				srcRowBytes,
								TQ3Uns8					*srcBasePtr,
								TQ3PixelType			srcPixelType,
								TQ3Endian 				srcByteOrder,
								TQ3Boolean				doVerticalFlip,
								GLint					*glPixelType);

TQ3Uns8				*IRRenderer_Texture_ConvertSize(
								TQ3Uns32				srcWidth,
								TQ3Uns32				srcHeight,
								TQ3Uns32				srcRowBytes,
								TQ3Uns8					*srcBasePtr,
								TQ3Uns32				*dstWidth,
								TQ3Uns32				*dstHeight,
								TQ3Uns32				*dstRowBytes);

TQ3Uns8				*IRRenderer_Texture_ConvertImage(
								TQ3StorageObject		theStorage,
								TQ3PixelType			srcPixelType,
								TQ3Uns32				srcWidth,
								TQ3Uns32				srcHeight,
								TQ3Uns32				srcRowBytes,
								TQ3Endian 				srcByteOrder,
								TQ3Uns32				*dstWidth,
								TQ3Uns32				*dstHeight,
								TQ3Uns32				*dstRowBytes,
								GLint					*glPixelType);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


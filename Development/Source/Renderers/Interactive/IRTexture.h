/*  NAME:
        IRTexture.h

    DESCRIPTION:
        Header file for IRTexture.c.

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
								TQ3Uns32				storageOffset,
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


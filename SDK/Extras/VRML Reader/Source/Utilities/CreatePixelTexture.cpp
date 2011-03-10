/*  NAME:
        CreatePixelTexture.cp

    DESCRIPTION:
       Utility function.

    COPYRIGHT:
        Copyright (c) 2005, Quesa Developers. All rights reserved.

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
#include "CreatePixelTexture.h"

#include "IsKeyPresent.h"
#include "VRML-reader-prefix.h"

#if __MACH__
	#include <Quesa/CQ3ObjectRef.h>
	#include <Quesa/QuesaShader.h>
	#include <Quesa/QuesaStorage.h>
#else
	#include <CQ3ObjectRef.h>
	#include <QuesaShader.h>
	#include <QuesaStorage.h>
#endif

/*!
	@function	CreatePixelTexture
	
	@abstract	Given the dictionary of a VRML 2 PixelTexture node or a
				VRML 1 Texture2 node, attempt to create a Quesa texture.
	
	@discussion	Currently we only implement RGB and RGBA textures, not
				intensity maps.
	
	@param		textureDict		The dictionary of a texture node.
	
	@result		A texture object, or NULL.
*/
CQ3ObjectRef	CreatePixelTexture( PolyValue::Dictionary& textureDict )
{
	CQ3ObjectRef	theTexture;
	
	if (IsKeyPresent( textureDict, "image" ))
	{
		PolyValue&	imageValue( textureDict["image"] );
		if (imageValue.GetType() == PolyValue::kDataTypeArrayOfInt)
		{
			const PolyValue::IntVec&	imageVec( imageValue.GetIntVec() );
			if (imageVec.size() >= 3)
			{
				int	imageWidth = imageVec[0];
				int	imageHeight = imageVec[1];
				int	componentCount = imageVec[2];
				
				if ( (imageVec.size() == imageWidth * imageHeight + 3) and
					((componentCount == 3) or (componentCount == 4))
				)
				{
					int	rowBytes = imageWidth * 4;
					std::vector<unsigned char>	theImage(imageHeight * rowBytes);
					int	srcRow, dstRow, col, alpha, red, green, blue;
					unsigned long srcPixel;
					
					// VRML stores the rows of a texture from bottom to top, but
					// Quesa does it top to bottom.
					if (componentCount == 3)
					{
						for (srcRow = 0; srcRow < imageHeight; ++srcRow)
						{
							dstRow = imageHeight - 1 - srcRow;
							for (col = 0; col < imageWidth; ++col)
							{
								srcPixel = imageVec[ 3 + col + srcRow * imageWidth ];
								red = (srcPixel & 0x00FF0000UL) >> 16;
								green = (srcPixel & 0x0000FF00) >> 8;
								blue = (srcPixel & 0x000000FF);
								
								theImage[ dstRow * rowBytes + col * 4 ] = 0xFF;
								theImage[ dstRow * rowBytes + col * 4 + 1 ] = red;
								theImage[ dstRow * rowBytes + col * 4 + 2 ] = green;
								theImage[ dstRow * rowBytes + col * 4 + 3 ] = blue;
							}
						}
					}
					else // (componentCount == 4)
					{
						for (srcRow = 0; srcRow < imageHeight; ++srcRow)
						{
							dstRow = imageHeight - 1 - srcRow;
							for (col = 0; col < imageWidth; ++col)
							{
								srcPixel = imageVec[ 3 + col + srcRow * imageWidth ];
								alpha = srcPixel & 0x0FF;
								red = (srcPixel & 0xFF000000UL) >> 24;
								green = (srcPixel & 0x00FF0000) >> 16;
								blue = (srcPixel & 0x0000FF00) >> 8;
								
								theImage[ dstRow * rowBytes + col * 4 ] = alpha;
								theImage[ dstRow * rowBytes + col * 4 + 1 ] = red;
								theImage[ dstRow * rowBytes + col * 4 + 2 ] = green;
								theImage[ dstRow * rowBytes + col * 4 + 3 ] = blue;
							}
						}
					}
					
					CQ3ObjectRef	imageStorage( Q3MemoryStorage_New( &theImage[0],
						theImage.size() ) );
					ThrowIfNullQuesaOb_( imageStorage );
					
					TQ3StoragePixmap pixmap = {
						imageStorage.get(),
						imageWidth,
						imageHeight,
						rowBytes,
						32,
						(componentCount == 3)? kQ3PixelTypeRGB32 : kQ3PixelTypeARGB32,
						kQ3EndianBig,
						kQ3EndianBig
					};
					
					CQ3ObjectRef	pixmapTexture( Q3PixmapTexture_New( &pixmap ) );
					theTexture = pixmapTexture;
				}
			}
		}
	}
	
	return theTexture;
}


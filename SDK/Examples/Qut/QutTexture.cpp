/*  NAME:
        QutTexture.c

    DESCRIPTION:
        Quesa Utility Toolkit - texture utilities.

    COPYRIGHT:
        Copyright (c) 1999-2019, Quesa Developers. All rights reserved.

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
#include "QutTexture.h"

#include <stdio.h>



//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#define	kTGATypeColor							2
#define	kTGATypeColorRLE						10
#define	kTGADescTopToBottom						0x20


//=============================================================================
//      Types
//-----------------------------------------------------------------------------
typedef struct TGAHeader {
	char  IDLength;
	char  colorMapType;
	char  dataTypeCode;
	short int colorMapOrigin;
	short int colorMapLength;
	char  colorMapDepth;
	short int x_origin;
	short int y_origin;
	short width;
	short height;
	char  bitsPerPixel;
	char  imageDescriptor;
} TGAHeader;



//=============================================================================
//		Private functions.
//-----------------------------------------------------------------------------

static short
qutTexture_read_littleendian_short( FILE* inFile )
{
	int	lowByte, hiByte;
	
	lowByte = fgetc( inFile );
	hiByte = fgetc( inFile );
	
	return lowByte | (hiByte << 8);
}

//=============================================================================
//		Public functions.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//		QutTexture_CreateTextureObjectFromTGAFile : Create texture object from a TGA file.
//-----------------------------------------------------------------------------
TQ3TextureObject
QutTexture_CreateTextureObjectFromTGAFile( const char* inFilePath )
{
	TQ3TextureObject	theTexture = NULL;
	FILE*				theFile = NULL;
	TGAHeader			theHeader;
	int					numPixels, bytesPerPixel, n, i, j;
	int					oppositeRow, packetHeader, chunkSize, rowBytes;
	TQ3Object			memStorage = NULL;
	unsigned char		p[5];
	unsigned char*		theBuffer;
	TQ3Uns32			bufferSize;
	TQ3StoragePixmap	thePixMap;
	TQ3PixelType		pixelType;
	
	theFile = fopen( inFilePath, "rb" );
	if (theFile != NULL)
	{
		// Read the file header.
		theHeader.IDLength = fgetc( theFile );
		theHeader.colorMapType = fgetc( theFile );
		theHeader.dataTypeCode = fgetc( theFile );
		theHeader.colorMapOrigin = qutTexture_read_littleendian_short( theFile );
		theHeader.colorMapLength = qutTexture_read_littleendian_short( theFile );
		theHeader.colorMapDepth = fgetc( theFile );
		theHeader.x_origin = qutTexture_read_littleendian_short( theFile );
		theHeader.y_origin = qutTexture_read_littleendian_short( theFile );
		theHeader.width = qutTexture_read_littleendian_short( theFile );
		theHeader.height = qutTexture_read_littleendian_short( theFile );
		theHeader.bitsPerPixel = fgetc( theFile );
		theHeader.imageDescriptor = fgetc( theFile );
		
		// Check that the image format is one we can handle.
		if (
			(
				(theHeader.dataTypeCode == kTGATypeColor) ||
				(theHeader.dataTypeCode == kTGATypeColorRLE)
			) &&
			(
				(theHeader.bitsPerPixel == 16) ||
				(theHeader.bitsPerPixel == 24) ||
				(theHeader.bitsPerPixel == 32)
			) &&
			(
				(theHeader.colorMapType == 0) ||
				(theHeader.colorMapType == 1)
			) &&
			! (feof(theFile) || ferror(theFile))
		)
		{
			// Skip identification field, if any.
			fseek( theFile, theHeader.IDLength, SEEK_CUR );
			// Skip color map, if any.
			fseek( theFile, theHeader.colorMapType * theHeader.colorMapLength, SEEK_CUR );
			
			// Make a storage object to hold the pixels.
			bytesPerPixel = theHeader.bitsPerPixel / 8;
			numPixels = theHeader.width * theHeader.height;
			if (numPixels <= 0)
			{
				fclose( theFile );
				return theTexture;
			}
			bufferSize = numPixels * bytesPerPixel;
			theBuffer = (unsigned char*) calloc( 1, bufferSize );
			if (theBuffer != NULL)
			{
				// Read the image.
				n = 0;
				while ( (n < numPixels) && (!feof(theFile)) && (!ferror(theFile)) )
				{
					if (theHeader.dataTypeCode == kTGATypeColor)	// uncompressed
					{
						fread( theBuffer + n * bytesPerPixel, bytesPerPixel, 1, theFile );
						++n;
					}
					else	// RLE compressed
					{
						packetHeader = fgetc( theFile );
						chunkSize = (packetHeader & 0x7F) + 1;
						
						if (packetHeader & 0x80)	// RLE chunk
						{
							fread( p, bytesPerPixel, 1, theFile );
							
							for (i=0; i < chunkSize; ++i)
							{
								memcpy( theBuffer + n * bytesPerPixel, p, bytesPerPixel );
								++n;
							}
						}
						else	// uncompressed chunk
						{
							fread( theBuffer + n * bytesPerPixel, bytesPerPixel, chunkSize, theFile );
							n += chunkSize;
						}
					}
				}
				
				// We may need to flip the image into the usual top to bottom row order.
				rowBytes = theHeader.width * bytesPerPixel;
				if ( (theHeader.imageDescriptor & kTGADescTopToBottom) == 0)
				{
					for (i = 0; i < theHeader.height / 2; ++i)
					{
						oppositeRow = theHeader.height - i - 1;
						for (j = 0; j < rowBytes; ++j)
						{
							unsigned char temp = theBuffer[ i * rowBytes + j ];
							theBuffer[ i * rowBytes + j ] = theBuffer[ oppositeRow * rowBytes + j ];
							theBuffer[ oppositeRow * rowBytes + j ] = temp;
						}
					}
				}
				
				// Set the pixel type
				switch (bytesPerPixel)
				{
					case 2:
						pixelType = kQ3PixelTypeRGB16;
						break;
						
					case 3:
						pixelType = kQ3PixelTypeRGB24;
						break;
					
					default:	// quiet an uninitialized-variable warning
					case 4:
						pixelType = kQ3PixelTypeARGB32;
						// Typically TGA format uses non-premultiplied alpha,
						// whereas Quesa usually expects premultiplied alpha, so
						// we multiply here.
						for (i = 0; i < theHeader.width * theHeader.height; ++i)
						{
							unsigned short a = theBuffer[i*4+3];

							theBuffer[i*4] = (theBuffer[i*4]*a) / 255;
							theBuffer[i*4+1] = (theBuffer[i*4+1]*a) / 255;
							theBuffer[i*4+2] = (theBuffer[i*4+2]*a) / 255;							
						}
						break;
				}
				
			#if TARGET_API_MAC_OS8
				// QD3D on Mac does not support kQ3PixelTypeRGB24, and also appears to
				// ignore the byte order, so we must convert to kQ3PixelTypeRGB32 in
				// big-endian order.
				if (pixelType == kQ3PixelTypeRGB24)
				{
					unsigned char*	bigBuffer = malloc( numPixels * 4 );
					if (bigBuffer != NULL)
					{
						pixelType = kQ3PixelTypeRGB32;
						
						for (i = 0; i < numPixels; ++i)
						{
							bigBuffer[4*i + 3] = theBuffer[3*i];
							bigBuffer[4*i + 2] = theBuffer[3*i + 1];
							bigBuffer[4*i + 1] = theBuffer[3*i + 2];
						}
						
						bytesPerPixel = 4;
						rowBytes = theHeader.width * bytesPerPixel;
						bufferSize = numPixels * bytesPerPixel;
						free( theBuffer );
						theBuffer = bigBuffer;
					}
				}
			#endif
				
				// Create a memory storage object holding a copy of the buffer
				memStorage = Q3MemoryStorage_New( theBuffer, bufferSize );
				
				if (memStorage != NULL)
				{
					// Create the texture object
					thePixMap.image = memStorage;
					thePixMap.width = theHeader.width;
					thePixMap.height = theHeader.height;
					thePixMap.rowBytes = rowBytes;
					thePixMap.pixelSize = bytesPerPixel * 8;
					thePixMap.bitOrder = kQ3EndianLittle;
					thePixMap.byteOrder = kQ3EndianLittle;
					thePixMap.pixelType = pixelType;
					
					theTexture = Q3PixmapTexture_New( &thePixMap );
					
					Q3Object_Dispose( memStorage );
				}
				free( theBuffer );
			}
		}
		
		fclose( theFile );
	}
	
	return theTexture;
}





//-----------------------------------------------------------------------------
//		QutTexture_CreateTextureFromTGAFile : Create texture shader from a TGA file.
//-----------------------------------------------------------------------------
TQ3ShaderObject
QutTexture_CreateTextureFromTGAFile( const char* inFilePath )
{
	TQ3TextureObject		qd3dTextureObject	= NULL ;
	TQ3ShaderObject			qd3dTextureShader	= NULL ;



	// Create the texture
	qd3dTextureObject = QutTexture_CreateTextureObjectFromTGAFile( inFilePath );



	// Create the texture shader
	if (qd3dTextureObject != NULL)
	{
		qd3dTextureShader = Q3TextureShader_New(qd3dTextureObject);
		Q3Object_Dispose(qd3dTextureObject);
	}

	return(qd3dTextureShader);
}


#if QUESA_OS_MACINTOSH
TQ3TextureObject	QutTexture_CreateTextureObjectFromFile(	CFURLRef		fileURL,
														TQ3Boolean		wantMipMaps)
{
	TQ3TextureObject	texture = NULL;
	
	CGImageSourceRef imSrc = CGImageSourceCreateWithURL( fileURL, NULL );
	if (imSrc != NULL)
	{
		CGImageRef imRef = CGImageSourceCreateImageAtIndex( imSrc, 0, NULL );
		CFRelease( imSrc );
		if (imRef != NULL)
		{
			CGBitmapInfo bitInfo = CGImageGetBitmapInfo( imRef );
			bitInfo = bitInfo & kCGBitmapAlphaInfoMask;
			bool hasAlpha = (bitInfo != kCGImageAlphaNone) &&
				(bitInfo != kCGImageAlphaNoneSkipFirst) &&
				(bitInfo != kCGImageAlphaNoneSkipLast);
			bitInfo = kCGBitmapByteOrder32Little |
				(hasAlpha? kCGImageAlphaPremultipliedFirst : kCGImageAlphaNoneSkipFirst);
			
			size_t theWidth = CGImageGetWidth( imRef );
			size_t theHeight = CGImageGetHeight( imRef );
			size_t rowBytes = theWidth * 4;
			
			CGContextRef imDst = CGBitmapContextCreate( NULL,
				theWidth, theHeight, 8,
				rowBytes, CGImageGetColorSpace( imRef ),
				bitInfo );
			if (imDst != NULL)
			{
				CGRect bounds = CGRectMake( 0.0, 0.0, theWidth, theHeight );
				CGContextClearRect( imDst, bounds );
				CGContextDrawImage( imDst, bounds, imRef );
				
				TQ3PixelType pixelType = hasAlpha?
					kQ3PixelTypeARGB32 : kQ3PixelTypeRGB32;
				const unsigned char* pixelData = (const unsigned char*)
					CGBitmapContextGetData( imDst );
				
				TQ3StorageObject qd3dMemoryStorage =
					Q3MemoryStorage_New( pixelData,
						(TQ3Uns32)(theHeight * rowBytes) );
				if (qd3dMemoryStorage)
				{
					if (wantMipMaps)
					{
						TQ3StoragePixmap	qd3dPixMap;
						
						qd3dPixMap.image    = qd3dMemoryStorage;
						qd3dPixMap.width	= (TQ3Uns32) theWidth;
						qd3dPixMap.height	= (TQ3Uns32) theHeight;
						qd3dPixMap.rowBytes	= (TQ3Uns32) rowBytes;
						qd3dPixMap.pixelSize	= 32;
						qd3dPixMap.pixelType	= pixelType;
						qd3dPixMap.bitOrder	= kQ3EndianLittle;
						qd3dPixMap.byteOrder	= kQ3EndianLittle;
						texture	= Q3PixmapTexture_New(&qd3dPixMap);
					}
					else
					{
						TQ3Mipmap				qd3dMipMap;
						qd3dMipMap.image         = qd3dMemoryStorage;
						qd3dMipMap.useMipmapping = kQ3False;
						qd3dMipMap.pixelType	 = pixelType;
						qd3dMipMap.bitOrder		 = kQ3EndianLittle;
						qd3dMipMap.byteOrder	 = kQ3EndianLittle;
						qd3dMipMap.reserved		 = 0;

						qd3dMipMap.mipmaps[0].width    = (TQ3Uns32) theWidth;
						qd3dMipMap.mipmaps[0].height   = (TQ3Uns32) theHeight;
						qd3dMipMap.mipmaps[0].rowBytes = (TQ3Uns32) rowBytes;
						qd3dMipMap.mipmaps[0].offset   = 0;
					
						texture = Q3MipmapTexture_New(&qd3dMipMap);
					}
					
					Q3Object_Dispose(qd3dMemoryStorage);
				}

				CGContextRelease( imDst );
			}
		}
		CGImageRelease( imRef );
	}

	return texture;
}
#endif




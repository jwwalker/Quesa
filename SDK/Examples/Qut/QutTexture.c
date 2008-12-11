/*  NAME:
        QutTexture.c

    DESCRIPTION:
        Quesa Utility Toolkit - texture utilities.

    COPYRIGHT:
        Copyright (c) 1999-2008, Quesa Developers. All rights reserved.

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

#if QUESA_OS_MACINTOSH
// The constant kNativeEndianPixMap is defined in the framework versions of
// QDOffscreen.h but not in the CFM (universal interfaces) versions.  We only
// need this flag when building natively for Intel, in which case we will be
// using framework headers.  Hence we can use 0 in other cases.
	#if TARGET_RT_MAC_CFM
		enum
		{
			kQ3NativeEndianPixMap           = 0
		};
	#else
		enum
		{
			kQ3NativeEndianPixMap           = kNativeEndianPixMap
		};
	#endif
#endif


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
			bufferSize = numPixels * bytesPerPixel;
			theBuffer = (unsigned char*) malloc( bufferSize );
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

//-----------------------------------------------------------------------------
//		QutTexture_CreateGWorldFromPICT : Create a GWorld from a PICT.
//-----------------------------------------------------------------------------
GWorldPtr
QutTexture_CreateGWorldFromPICT(PicHandle thePicture, TQ3PixelType pixelType)
{	GWorldPtr			deepGWorld, theGWorld;
 	Boolean				shouldDither;
 	GDHandle			saveDevice;
	CGrafPtr			savePort;
	TQ3Uns32			theDepth;
	OSErr				theErr;



	// Work out the depth of GWorld, and if it should be dithered
	if (pixelType == kQ3PixelTypeARGB16 || pixelType == kQ3PixelTypeRGB16)
		theDepth = 16;
	else
		theDepth = 32;

	shouldDither = (theDepth == 16);



	// Create a GWorld to hold the image data
	theErr = NewGWorld(&theGWorld, theDepth, &(*thePicture)->picFrame, NULL, NULL,
		useTempMem | kQ3NativeEndianPixMap );
	if (theErr != noErr || theGWorld == NULL)
		return(NULL);



	// If we should dither, create a deep GWorld
	if (theDepth == 16 && shouldDither)
		theErr = NewGWorld(&deepGWorld, 32, &(*thePicture)->picFrame, NULL, NULL,
			useTempMem | kQ3NativeEndianPixMap );
	else
		deepGWorld = NULL;
		


	// If we have a deep GWorld, draw the image into that and then
	// copy it into the output GWorld. If we don't have a deep GWorld,
	// just draw it directly into the output GWorld.
	if (deepGWorld != NULL)
		{
		// Draw the picture into the deep GWorld
		GetGWorld(&savePort, &saveDevice);
		SetGWorld(deepGWorld, NULL);
		LockPixels(GetGWorldPixMap(deepGWorld));

		DrawPicture(thePicture, &(*thePicture)->picFrame);

		UnlockPixels(GetGWorldPixMap(deepGWorld));
		SetGWorld(savePort, saveDevice);



		// Copy this into the output GWorld
		GetGWorld(&savePort, &saveDevice);
		SetGWorld(theGWorld, NULL);
		LockPixels(GetGWorldPixMap(theGWorld));

		CopyBits(GetPortBitMapForCopyBits(deepGWorld),
				 GetPortBitMapForCopyBits(theGWorld),
				 &(*thePicture)->picFrame,
				 &(*thePicture)->picFrame,
				 ditherCopy, NULL);

		UnlockPixels(GetGWorldPixMap(theGWorld));
		SetGWorld(savePort, saveDevice);


		// Clean up
		DisposeGWorld(deepGWorld);
		deepGWorld = NULL;
		}
	else
		{
		// Draw the picture into the output GWorld
		GetGWorld(&savePort, &saveDevice);
		SetGWorld(theGWorld, NULL);
		LockPixels(GetGWorldPixMap(theGWorld));

		DrawPicture(thePicture, &(*thePicture)->picFrame);

		UnlockPixels(GetGWorldPixMap(theGWorld));
		SetGWorld(savePort, saveDevice);
		}
		
	return(theGWorld);
}





//=============================================================================
//		QutTexture_CreateGWorldFromFile : Create a GWorld from a file.
//-----------------------------------------------------------------------------
GWorldPtr
QutTexture_CreateGWorldFromFile(const FSSpec *theFSSpec, TQ3PixelType pixelType)
{	CTabHandle					colourTableHnd;
	ImageDescriptionHandle		imageDescHnd;
	ComponentInstance			theImporter;
	GWorldPtr					theGWorld;
	TQ3Uns32					theDepth;
	Rect						theRect;
	OSErr						theErr;



	// Initialise ourselves
	theImporter    = NULL;
	theGWorld      = NULL;
	imageDescHnd   = NULL;
	colourTableHnd = NULL;
	theDepth       = (pixelType == kQ3PixelTypeARGB16 || pixelType == kQ3PixelTypeRGB16) ? 16 : 32;



	// Create the importer
	theErr = GetGraphicsImporterForFile(theFSSpec, &theImporter);



	// Query the importer for the information we need
	if (theErr == noErr)
		{
		GraphicsImportGetBoundsRect(theImporter, &theRect);
	
		theErr = GraphicsImportGetImageDescription(theImporter, &imageDescHnd);
		if (theErr == noErr)
			theErr = GetImageDescriptionCTable(imageDescHnd, &colourTableHnd);
		}



	// Create the GWorld
	if (theErr == noErr)
		theErr = NewGWorld(&theGWorld, theDepth, &theRect, colourTableHnd, NULL,
			useTempMem | kQ3NativeEndianPixMap );



	// Draw the image into the GWorld
	if (theErr == noErr)
		{
		GraphicsImportSetGWorld(theImporter, theGWorld, NULL);
		GraphicsImportDraw(theImporter);
		}



	// Clean up
	if (theImporter != NULL)
		CloseComponent(theImporter);
		
	if (imageDescHnd != NULL)
		DisposeHandle((Handle) imageDescHnd);
		
	if (colourTableHnd != NULL)
		DisposeCTable(colourTableHnd);

	return(theGWorld);
}





//=============================================================================
//		QutTexture_CreateTextureFromPixmap : Create a QD3D texture.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ShaderObject
QutTexture_CreateTextureFromPixmap(	PixMapHandle	thePixMap,
									TQ3PixelType	pixelType,
									TQ3Boolean		wantMipMaps)
{
	TQ3TextureObject		qd3dTextureObject	= NULL ;
	TQ3ShaderObject			qd3dTextureShader	= NULL ;



	// Create the texture
	qd3dTextureObject = QutTexture_CreateTextureObjectFromPixmap(	thePixMap, 
																	pixelType,
																	wantMipMaps) ;



	// Create the texture shader
	if (qd3dTextureObject != NULL)
	{
		qd3dTextureShader = Q3TextureShader_New(qd3dTextureObject);
		Q3Object_Dispose(qd3dTextureObject);
	}

	return(qd3dTextureShader);
}





//=============================================================================
//		QutTexture_CreateCompressedTextureFromPixmap : Create a QD3D compressed
//													   texture from a Pixmap.
//-----------------------------------------------------------------------------
TQ3ShaderObject
QutTexture_CreateCompressedTextureFromPixmap(	PixMapHandle	thePixMap,
												TQ3PixelType	pixelType,
												TQ3Boolean		wantMipMaps)
{
	TQ3ShaderObject		theShader	= NULL;
	TQ3TextureObject	theTexture	= NULL;



	theTexture	= QutTexture_CreateCompressedTextureObjectFromPixmap( thePixMap, pixelType, wantMipMaps ) ;
	if ( theTexture != NULL)
		{
		theShader	= Q3TextureShader_New( theTexture );
		Q3Object_Dispose( theTexture );
		}
	
	return(theShader) ;
}





//=============================================================================
//		QutTexture_CreateTextureFromGWorld : Create a QD3D texture from a GWorld.
//-----------------------------------------------------------------------------
TQ3ShaderObject
QutTexture_CreateTextureFromGWorld(GWorldPtr		theGWorld,
									TQ3PixelType	pixelType,
							 		TQ3Boolean		wantMipMaps)
{	TQ3ShaderObject		theTexture;
	PixMapHandle		thePixMap;



	// Grab the GWorld PixMap
	thePixMap = GetGWorldPixMap(theGWorld);
	if (thePixMap == NULL)
		return(NULL);



	// Create a texture from the PixMap
	theTexture = QutTexture_CreateTextureFromPixmap(thePixMap, pixelType, wantMipMaps);

	return(theTexture);
}





//=============================================================================
//		QutTexture_CreateTextureFromFile : Create a QD3D texture from a file.
//-----------------------------------------------------------------------------
TQ3ShaderObject
QutTexture_CreateTextureFromFile(const FSSpec		*theFSSpec,
									TQ3PixelType	pixelType,
									TQ3Boolean		wantMipMaps)
{	
	TQ3ShaderObject		theShader	= NULL;
	GWorldPtr			theGWorld	= NULL;



	// Load the image, then create a texture from it
	theShader = NULL;
	theGWorld = QutTexture_CreateGWorldFromFile(theFSSpec, pixelType);
	if (theGWorld != NULL)
		{
		theShader = QutTexture_CreateTextureFromGWorld(theGWorld, pixelType, wantMipMaps);
		DisposeGWorld(theGWorld);
		}
	
	return(theShader);
}





//=============================================================================
//		QutTexture_CreateCompressedTextureFromFile : Create a QD3D compressed
//													 texture from a file.
//-----------------------------------------------------------------------------														
TQ3ShaderObject		
QutTexture_CreateCompressedTextureFromFile(	const FSSpec *	theFSSpec,
											TQ3PixelType	pixelType,
											TQ3Boolean		wantMipMaps)
{
	TQ3TextureObject		theTexture	= NULL;
	TQ3ShaderObject			theShader	= NULL;
	
	
	theTexture = QutTexture_CreateCompressedTextureObjectFromFile( theFSSpec, pixelType, wantMipMaps ) ;
	
	if ( theTexture != NULL)
		{
		theShader = Q3TextureShader_New( theTexture );
		Q3Object_Dispose( theTexture );
		}
	
	return(theShader);
}





//=============================================================================
//		QutTexture_CreateTextureObjectFromPixmap : Create a QD3D texture object
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TextureObject
QutTexture_CreateTextureObjectFromPixmap(PixMapHandle		thePixMap,
									TQ3PixelType	pixelType,
									TQ3Boolean		wantMipMaps)
{	TQ3Uns32				x, y, theWidth, theHeight, rowBytes, pixelBytes;
	TQ3TextureObject		qd3dTextureObject = NULL;
	TQ3StorageObject		qd3dMemoryStorage;
	TQ3StoragePixmap		qd3dPixMap;
	TQ3Mipmap				qd3dMipMap;
	UInt16					*pixelPtr;
	UInt8					*baseAddr;
	OSType					pixelFormat;
	TQ3Endian				byteOrder;


	// Get the details we need from the PixMap
	NoPurgePixels(thePixMap);
	LockPixels(thePixMap);

	theWidth   = (*thePixMap)->bounds.right  - (*thePixMap)->bounds.left;
	theHeight  = (*thePixMap)->bounds.bottom - (*thePixMap)->bounds.top;
	rowBytes   = (*thePixMap)->rowBytes & 0x7FFF;
	pixelBytes = (*thePixMap)->pixelSize / 8;
	baseAddr   = (UInt8 *) GetPixBaseAddr(thePixMap);
#if OLDPIXMAPSTRUCT
	pixelFormat = 0;
#else
	pixelFormat = (*thePixMap)->pixelFormat;
#endif


	// If this is a 16 bit alpha channel texture, set the alpha bits.
	// We assume that black is transparent.
	if (pixelType == kQ3PixelTypeARGB16)
		{
		for (y = 0; y < theHeight; y++)
			{
			for (x = 0; x < theWidth; x++)
				{
				pixelPtr = (UInt16 *) (baseAddr + (y * rowBytes) + (x * 2));
				if (*pixelPtr != 0x0000)
					*pixelPtr |= (1 << 15);
				}
			}
		}


	// Set the byte order
	if ( (pixelFormat == k32BGRAPixelFormat) ||
		(pixelFormat == k16LE555PixelFormat) )
	{
		byteOrder = kQ3EndianLittle;
	}
	else
	{
		byteOrder = kQ3EndianBig;
	}


	// Create a storage object based on the GWorld
	qd3dMemoryStorage = Q3MemoryStorage_New(baseAddr, theHeight * rowBytes);
	if (qd3dMemoryStorage != NULL)
		{
		// Create the appropriate type of texture. Note that if mip-maps are
		// required, we create a QD3D PixMap. This might seem back to front,
		// but QD3D automatically creates mip-maps for PixMaps.
		//
		// If mip-maps are not required, we create a QD3D MipMap by hand and
		// signal that we don't want any other mip-maps to be created.
		//
		// We need to work around a bug in Q3MipmapTexture_New: if the rowByte
		// for the pixel exactly matches the size of each row, the texture is
		// distorted - we can fix this by using Q3PixmapTexture_New instead. 
		if (wantMipMaps || (rowBytes == theWidth * pixelBytes))
			{
			// Create a PixMap from the GWorld data
			qd3dPixMap.image        = qd3dMemoryStorage;
			qd3dPixMap.width		= theWidth;
			qd3dPixMap.height		= theHeight;
			qd3dPixMap.rowBytes		= rowBytes;
			qd3dPixMap.pixelSize	= (pixelType == kQ3PixelTypeARGB32 || pixelType == kQ3PixelTypeRGB32) ? 32 : 16;
			qd3dPixMap.pixelType	= pixelType;
			qd3dPixMap.bitOrder		= byteOrder;
			qd3dPixMap.byteOrder	= byteOrder;

			qd3dTextureObject = Q3PixmapTexture_New(&qd3dPixMap);
			}
	
		else
			{
			// Create a MipMap from the GWorld data
			qd3dMipMap.image         = qd3dMemoryStorage;
			qd3dMipMap.useMipmapping = kQ3False;
			qd3dMipMap.pixelType	 = pixelType;
			qd3dMipMap.bitOrder		 = byteOrder;
			qd3dMipMap.byteOrder	 = byteOrder;
			qd3dMipMap.reserved		 = 0;

			qd3dMipMap.mipmaps[0].width    = theWidth;
			qd3dMipMap.mipmaps[0].height   = theHeight;
			qd3dMipMap.mipmaps[0].rowBytes = rowBytes;
			qd3dMipMap.mipmaps[0].offset   = 0;
		
			qd3dTextureObject = Q3MipmapTexture_New(&qd3dMipMap);
			}
		Q3Object_Dispose(qd3dMemoryStorage);
		
		}



	// Clean up and return
	UnlockPixels(thePixMap);
	return(qd3dTextureObject);
}





//=============================================================================
//		QutTexture_CreateTextureObjectFromGWorld :	Create a QD3D texture oject
//													from a GWorld.
//-----------------------------------------------------------------------------
TQ3TextureObject
QutTexture_CreateTextureObjectFromGWorld(GWorldPtr		theGWorld,
									TQ3PixelType	pixelType,
							 		TQ3Boolean		wantMipMaps)
{	TQ3TextureObject	theTexture;
	PixMapHandle		thePixMap;



	// Grab the GWorld PixMap
	thePixMap = GetGWorldPixMap(theGWorld);
	if (thePixMap == NULL)
		return(NULL);



	// Create a texture from the PixMap
	theTexture = QutTexture_CreateTextureObjectFromPixmap(thePixMap, pixelType, wantMipMaps);

	return(theTexture);
}





//=============================================================================
//		QutTexture_CreateCompressedTextureObjectFromPixmap : Create a QD3D
//															 texture.
//-----------------------------------------------------------------------------
TQ3TextureObject
QutTexture_CreateCompressedTextureObjectFromPixmap( PixMapHandle sourcePixmap, 
													TQ3PixelType pixelType,
													TQ3Boolean wantMipMaps )
{

	ComponentInstance	theCompressor	  = OpenDefaultComponent(StandardCompressionType,
																 StandardCompressionSubType ) ;
	Rect				bounds		 	  = (** sourcePixmap).bounds;
	TQ3TextureObject	compressedTexture = NULL;
	TQ3Status			qd3dStatus		  = kQ3Success;
	TQ3CompressedPixmap	compressedPixmap;
	

	
	// set the fields I must
	compressedPixmap.imageDescByteOrder	= kQ3EndianBig;
	compressedPixmap.makeMipmaps		= wantMipMaps;
	compressedPixmap.width				= bounds.right - bounds.left;
	compressedPixmap.height				= bounds.top - bounds.bottom;
	compressedPixmap.pixelSize			= 32;
	compressedPixmap.pixelType			= pixelType ;



	// use QD3D for the hard work, use the minimim quality to see the
	// results of compression
	qd3dStatus = Q3CompressedPixmapTexture_CompressImage(	&compressedPixmap,
															sourcePixmap,
															compressorComponentType,
															bestCompressionCodec,
															0,
															codecMinQuality);
	if( qd3dStatus == kQ3Success )														
		compressedTexture	= Q3CompressedPixmapTexture_New( &compressedPixmap ) ;
	
	return(compressedTexture);
}





//=============================================================================
//		QutTexture_CreateTextureObjectFromFile : Create a QD3D texture.
//-----------------------------------------------------------------------------
TQ3TextureObject
QutTexture_CreateTextureObjectFromFile(const FSSpec		*theFSSpec,
									TQ3PixelType	pixelType,
									TQ3Boolean		wantMipMaps)
{	TQ3TextureObject	theTexture;
	GWorldPtr			theGWorld;



	// Load the image, then create a texture from it
	theTexture = NULL;
	theGWorld = QutTexture_CreateGWorldFromFile(theFSSpec, pixelType);
	if (theGWorld != NULL)
	{
		theTexture = QutTexture_CreateTextureObjectFromGWorld(theGWorld, pixelType, wantMipMaps);
		DisposeGWorld(theGWorld);
	}
	
	return(theTexture);
}





//=============================================================================
//		QutTexture_CreateCompressedTextureObjectFromFile :	Create a QD3D 
//															compressed texture.
//-----------------------------------------------------------------------------
TQ3TextureObject		
QutTexture_CreateCompressedTextureObjectFromFile(
									const FSSpec *	theFSSpec,
									TQ3PixelType	pixelType,
									TQ3Boolean		wantMipMaps)
{
	TQ3TextureObject	theTexture	= NULL;
	GWorldPtr			theGWorld	= NULL;
	PixMapHandle		thePixmap	= NULL;



	// Load the image, then create a texture from it
	theGWorld = QutTexture_CreateGWorldFromFile(theFSSpec, pixelType);
	if (theGWorld != NULL)
	{
		thePixmap	= GetGWorldPixMap(theGWorld);
		
		if( thePixmap != NULL)
		{
			theTexture = QutTexture_CreateCompressedTextureObjectFromPixmap(thePixmap, pixelType, wantMipMaps);
			DisposeGWorld(theGWorld);
		}
	}
	
	return(theTexture);
}

#endif


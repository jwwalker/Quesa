/*  NAME:
        QutTexture.c

    DESCRIPTION:
        Quesa Utility Toolkit - texture utilities.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QutTexture.h"





//=============================================================================
//		Public functions.
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
	theErr = NewGWorld(&theGWorld, theDepth, &(*thePicture)->picFrame, NULL, NULL, useTempMem);
	if (theErr != noErr || theGWorld == NULL)
		return(NULL);



	// If we should dither, create a deep GWorld
	if (theDepth == 16 && shouldDither)
		theErr = NewGWorld(&deepGWorld, 32, &(*thePicture)->picFrame, NULL, NULL, useTempMem);
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
		theErr = NewGWorld(&theGWorld, theDepth, &theRect, colourTableHnd, NULL, useTempMem);



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
	TQ3TextureObject		qd3dTextureObject;
	TQ3StorageObject		qd3dMemoryStorage;
	TQ3StoragePixmap		qd3dPixMap;
	TQ3Mipmap				qd3dMipMap;
	UInt16					*pixelPtr;
	UInt8					*baseAddr;



	// Get the details we need from the PixMap
	NoPurgePixels(thePixMap);
	LockPixels(thePixMap);

	theWidth   = (*thePixMap)->bounds.right  - (*thePixMap)->bounds.left;
	theHeight  = (*thePixMap)->bounds.bottom - (*thePixMap)->bounds.top;
	rowBytes   = (*thePixMap)->rowBytes & 0x7FFF;
	pixelBytes = (*thePixMap)->pixelSize / 8;
	baseAddr   = (UInt8 *) GetPixBaseAddr(thePixMap);



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
			qd3dPixMap.bitOrder		= kQ3EndianBig;
			qd3dPixMap.byteOrder	= kQ3EndianBig;

			qd3dTextureObject = Q3PixmapTexture_New(&qd3dPixMap);
			}
	
		else
			{
			// Create a MipMap from the GWorld data
			qd3dMipMap.image         = qd3dMemoryStorage;
			qd3dMipMap.useMipmapping = kQ3False;
			qd3dMipMap.pixelType	 = pixelType;
			qd3dMipMap.bitOrder		 = kQ3EndianBig;
			qd3dMipMap.byteOrder	 = kQ3EndianBig;
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

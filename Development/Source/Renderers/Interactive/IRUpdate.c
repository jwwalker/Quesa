/*  NAME:
        IRUpdate.c

    DESCRIPTION:
        Quesa interactive renderer update methods.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "IRPrefix.h"
#include "IRUpdate.h"

#include "GLPrefix.h"
#include "GLCamera.h"
#include "GLUtils.h"
#include "GLDrawContext.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kAAPointSize									0.5f
#define kAALineSize										0.5f





//=============================================================================
//      ir_state_is_power_of_2 : Is a number a power of 2?
//-----------------------------------------------------------------------------
//		Note : Cute trick found in:
//			   http://web2.airmail.net/sjbaker1/software/cute_code.html
//-----------------------------------------------------------------------------
#define ir_state_is_power_of_2(_x)			(((_x) & ((_x) - 1)) == 0)





//=============================================================================
//      ir_state_texture_get_data : Get the data for an image.
//-----------------------------------------------------------------------------
//		Note : If possible, we try and avoid copying the image data.
//-----------------------------------------------------------------------------
static TQ3Uns8 *
ir_state_texture_get_data(TQ3StorageObject theStorage, TQ3Boolean *wasCopied)
{	TQ3Uns32			validSize, bufferSize;
	TQ3Status			qd3dStatus;
	TQ3Uns8				*basePtr;
	TQ3ObjectType		theType;
	


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theStorage), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(wasCopied), NULL);



	// Get a pointer to the data for the image, ideally without copying
	*wasCopied = kQ3False;
	theType    = Q3Storage_GetType(theStorage);
	switch (theType) {
		case kQ3StorageTypeMemory:
			qd3dStatus = Q3MemoryStorage_GetBuffer(theStorage, &basePtr, &validSize, &bufferSize);
			break;

#if QUESA_OS_MACINTOSH
		case kQ3MemoryStorageTypeHandle:
			{	Handle	theHnd;
			
			qd3dStatus = Q3HandleStorage_Get(theStorage, &theHnd, &validSize);
			if (qd3dStatus == kQ3Success && theHnd != NULL)
				{
			    MoveHHi(theHnd);
    			HLock(theHnd);
    			basePtr = (TQ3Uns8 *) *theHnd;
    			}
    		}
    		break;
#endif

		default:
			qd3dStatus = Q3Storage_GetSize(theStorage, &bufferSize);
			if (qd3dStatus == kQ3Success)
				basePtr = E3Memory_Allocate(bufferSize);
			
			if (basePtr != NULL)
				{
				qd3dStatus = Q3Storage_GetData(theStorage, 0, bufferSize, basePtr, &validSize);
				*wasCopied = (TQ3Boolean) (qd3dStatus == kQ3Success);
				
				if (qd3dStatus != kQ3Success)
					E3Memory_Free(&basePtr);
				}
			break;
		}



	// Return the image
	if (qd3dStatus != kQ3Success)
		return(NULL);
	
	return(basePtr);
}





//=============================================================================
//      ir_state_texture_release_data : Release the data for an image.
//-----------------------------------------------------------------------------
//		Note :	If we copied the data for the image, we dispose of it here.
//
//				We also need to check for Mac Handle storage objects, and
//				unlock the handle - we left it locked when the image data was
//				acquired, since it may have saved us from having to copy the
//				data.
//-----------------------------------------------------------------------------
static void
ir_state_texture_release_data(TQ3StorageObject	theStorage,
								TQ3Uns8			*basePtr,
								TQ3Boolean		wasCopied)
{
#if QUESA_OS_MACINTOSH
	TQ3Status			qd3dStatus;
	TQ3Uns32			validSize;
	TQ3ObjectType		theType;
	Handle				theHnd;



	// If this is a Mac handle object, unlock the handle
	theType = Q3Storage_GetType(theStorage);
	if (theType == kQ3MemoryStorageTypeHandle)
		{
		qd3dStatus = Q3HandleStorage_Get(theStorage, &theHnd, &validSize);
		if (qd3dStatus == kQ3Success && theHnd != NULL)
			HUnlock(theHnd);
		}
#endif



	// If the data was copied, dispose of it
	if (wasCopied)
		E3Memory_Free(&basePtr);
}





//=============================================================================
//      ir_state_texture_convert_depth : Convert an image to a suitable depth.
//-----------------------------------------------------------------------------
//		Note :	OpenGL textures currently have to be 24 or 32bpp deep.
//
//				QD3D allows various depths, so we need to resample the QD3D
//				texture to obtain a new image we can pass to OpenGL.
//
//				We always return 32bpp RGBA images, although we also set up
//				outPixelType to act as a hint for the final desired depth (so
//				shallower images will hopefully end up smaller when loaded into
//				VRAM).
//
//				Note that we always have to create a new image, since even
//				32bpp data from QD3D has to be turned from ARGB into RGBA.
//
//				We never return 24bpp images, since the missing alpha bytes
//				will misalign the texture data. Even if no alpha is present in
//				the QD3D texture, a dummy value is created.
//-----------------------------------------------------------------------------
static TQ3Uns8 *
ir_state_texture_convert_depth(TQ3Uns32			theWidth,
								TQ3Uns32		theHeight,
								TQ3Uns32		srcRowBytes,
								TQ3Uns8			*srcBasePtr,
								TQ3PixelType	srcPixelType,
								TQ3Endian 		srcByteOrder,
								GLint			*glPixelType)
{	TQ3Uns8				*dstBasePtr, *dstRow, *dstPixel, *srcRow, *srcPixel;
	TQ3Uns32			redBits, greenBits, blueBits, alphaBits;
	TQ3Uns32			x, y, n, dstRowBytes, srcDepth;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(theWidth    != 0,          NULL);
	Q3_REQUIRE_OR_RESULT(theHeight   != 0,          NULL);
	Q3_REQUIRE_OR_RESULT(srcRowBytes != 0,          NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(srcBasePtr),  NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glPixelType), NULL);



	// Allocate the buffer for the image data
	dstBasePtr = E3Memory_Allocate(theWidth * theHeight * 4);
	if (dstBasePtr == NULL)
		return(NULL);



	// Work out the information we need to convert the image
	*glPixelType = GLUtils_ConvertPixelType(srcPixelType);
	srcDepth     = GLUtils_SizeOfPixelType(srcPixelType)/8;
	dstRowBytes  = theWidth * 4;

	srcRow = srcBasePtr;
	dstRow = dstBasePtr;



	// Big endian conversion
	if(srcByteOrder == kQ3EndianBig)
		{
		switch(srcPixelType) {
			case kQ3PixelTypeARGB32:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[1];
						dstPixel[1] = srcPixel[2];
						dstPixel[2] = srcPixel[3];
						dstPixel[3] = srcPixel[0];
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeARGB32
			case kQ3PixelTypeRGB32:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[1];
						dstPixel[1] = srcPixel[2];
						dstPixel[2] = srcPixel[3];
						dstPixel[3] = 0xFF;
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeRGB32
			case kQ3PixelTypeRGB24:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[0];
						dstPixel[1] = srcPixel[1];
						dstPixel[2] = srcPixel[2];
						dstPixel[3] = 0xFF;
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeRGB24
			case kQ3PixelTypeARGB16:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
				    n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
						alphaBits = (n >> 15) & 0x0001;
						redBits   = (n >> 10) & 0x001F;
						greenBits = (n >>  5) & 0x001F;
						blueBits  = (n >>  0) & 0x001F;
								
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 8);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = (TQ3Uns8) (alphaBits * 0xFF);
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeARGB16
			case kQ3PixelTypeRGB16:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
						n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
						redBits   = (n >> 10) & 0x001F;
						greenBits = (n >>  5) & 0x001F;
						blueBits  = (n >>  0) & 0x001F;
	
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 8);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = 0xFF;
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeRGB16
			case kQ3PixelTypeRGB16_565:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
						n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
						redBits   = (n >> 11) & 0x001F;
						greenBits = (n >>  5) & 0x003F;
						blueBits  = (n >>  0) & 0x001F;
	
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 4);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = 0xFF;
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeRGB16_565
			}
		}


	// Little endian conversion
	else
		{
		switch(srcPixelType) {
			case kQ3PixelTypeARGB32:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[2];
						dstPixel[1] = srcPixel[1];
						dstPixel[2] = srcPixel[0];
						dstPixel[3] = srcPixel[3];
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeARGB32
			case kQ3PixelTypeRGB32:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[2];
						dstPixel[1] = srcPixel[1];
						dstPixel[2] = srcPixel[0];
						dstPixel[3] = 0xFF;
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeRGB32
			case kQ3PixelTypeRGB24:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[3];
						dstPixel[1] = srcPixel[2];
						dstPixel[2] = srcPixel[1];
						dstPixel[3] = 0xFF;
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeRGB24
			case kQ3PixelTypeARGB16:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
				    n         = (TQ3Uns32) *((TQ3Uns16 *) E3EndianSwap16(srcPixel));
						alphaBits = (n >> 15) & 0x0001;
						redBits   = (n >> 10) & 0x001F;
						greenBits = (n >>  5) & 0x001F;
						blueBits  = (n >>  0) & 0x001F;
								
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 8);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = (TQ3Uns8) (alphaBits * 0xFF);
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeARGB16
			case kQ3PixelTypeRGB16:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
						n         = (TQ3Uns32) *((TQ3Uns16 *) E3EndianSwap16(srcPixel));
						redBits   = (n >> 10) & 0x001F;
						greenBits = (n >>  5) & 0x001F;
						blueBits  = (n >>  0) & 0x001F;
	
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 8);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = 0xFF;
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeRGB16
			case kQ3PixelTypeRGB16_565:
				{
				for (y = 0; y < theHeight; y++)
					{
					// Reset the pixel to the start of the row
					srcPixel = srcRow;
					dstPixel = dstRow;
					// Convert the row
					for (x = 0; x < theWidth; x++)
						{
						n         = (TQ3Uns32) *((TQ3Uns16 *) E3EndianSwap16(srcPixel));
						redBits   = (n >> 11) & 0x001F;
						greenBits = (n >>  5) & 0x003F;
						blueBits  = (n >>  0) & 0x001F;
	
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 4);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = 0xFF;
						// Move on to the next pixel
						srcPixel += srcDepth;
						dstPixel += 4;
						}
					// Move on to the next row
					srcRow += srcRowBytes;
					dstRow += dstRowBytes;
					}
				}
				break;	//kQ3PixelTypeRGB16_565
			}
		}



	// Return the new image
	return(dstBasePtr);
}





//=============================================================================
//      ir_state_texture_convert_size : Convert an image to a suitable size.
//-----------------------------------------------------------------------------
//		Note :	OpenGL textures currently have to be powers of 2 in size.
//
//				QD3D allows various sizes, so we need to resample the QD3D
//				texture to obtain a new image we can pass to OpenGL.
//
//				We assume the input image is 32bpp deep.
//-----------------------------------------------------------------------------
static TQ3Uns8 *
ir_state_texture_convert_size(TQ3Uns32			srcWidth,
								TQ3Uns32		srcHeight,
								TQ3Uns32		srcRowBytes,
								TQ3Uns8			*srcBasePtr,
								TQ3Uns32		*dstWidth,
								TQ3Uns32		*dstHeight,
								TQ3Uns32		*dstRowBytes)
{	TQ3Uns32			theWidth, theHeight;
	TQ3Uns8				*dstBasePtr;
	GLint				maxGLSize;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(srcWidth    != 0,                   NULL);
	Q3_REQUIRE_OR_RESULT(srcHeight   != 0,                   NULL);
	Q3_REQUIRE_OR_RESULT(srcRowBytes != 0,                   NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(srcBasePtr),           NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstWidth),             NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstHeight),            NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstRowBytes),          NULL);
	Q3_REQUIRE_OR_RESULT(!ir_state_is_power_of_2(srcWidth),  NULL);
	Q3_REQUIRE_OR_RESULT(!ir_state_is_power_of_2(srcHeight), NULL);



	// Assign some return values
	*dstWidth    = 0;
	*dstHeight   = 0;
	*dstRowBytes = 0;



	// Work out how large the image should be. We scale up rather
	// than down, resizing the image to the next largest power of 2.
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxGLSize);
	theWidth  = 1;
	theHeight = 1;

	while (theWidth < srcWidth && theWidth <= (TQ3Uns32) maxGLSize)
		theWidth *= 2;
		
	while (theHeight < srcHeight && theHeight <= (TQ3Uns32) maxGLSize)
		theHeight *= 2;



	// Allocate the buffer for the image data
	dstBasePtr = E3Memory_Allocate(theWidth * theHeight * 4);
	if (dstBasePtr == NULL)
		return(NULL);



	// Resize the image
	gluScaleImage(GL_RGBA, srcWidth, srcHeight, GL_UNSIGNED_BYTE, srcBasePtr,
						   theWidth, theHeight, GL_UNSIGNED_BYTE, dstBasePtr);



	// Return the scaled image
	*dstWidth    = theWidth;
	*dstHeight   = theHeight;
	*dstRowBytes = theWidth * 4;	

	return(dstBasePtr);
}





//=============================================================================
//      ir_state_texture_convert_image : Convert a QD3D texture.
//-----------------------------------------------------------------------------
//		Note :	Given a QD3D texture return a block of memory suitable for use
//				as an OpenGL texture, as well as the appropriate size/depth
//				description of the new texture.
//
//				QD3D textures are always converted to a 32bpp RGBA image for
//				OpenGL, and our caller must dispose of this image after use.
//-----------------------------------------------------------------------------
static TQ3Uns8 *
ir_state_texture_convert_image(TQ3StorageObject		theStorage,
								TQ3PixelType		srcPixelType,
								TQ3Uns32			srcWidth,
								TQ3Uns32			srcHeight,
								TQ3Uns32			srcRowBytes,
								TQ3Endian 		srcByteOrder,
								TQ3Uns32			*dstWidth,
								TQ3Uns32			*dstHeight,
								TQ3Uns32			*dstRowBytes,
								GLint				*glPixelType)
{	TQ3Uns8		*qd3dBasePtr, *depthBasePtr, *sizeBasePtr;
	TQ3Boolean		wasCopied;


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theStorage),    NULL);
	Q3_REQUIRE_OR_RESULT(srcWidth    != 0,            NULL);
	Q3_REQUIRE_OR_RESULT(srcHeight   != 0,            NULL);
	Q3_REQUIRE_OR_RESULT(srcRowBytes != 0,            NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstWidth),      NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstHeight),     NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstRowBytes),   NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glPixelType),   NULL);



	// Set up some return values
	*dstWidth    = 0;
	*dstHeight   = 0;
	*dstRowBytes = 0;
	*glPixelType = GL_RGB;



	// Get the QD3D texture data
	qd3dBasePtr = ir_state_texture_get_data(theStorage, &wasCopied);
	if (qd3dBasePtr == NULL)
		return(NULL);



	// Copy the texture to a temporary copy of the correct depth
	depthBasePtr = ir_state_texture_convert_depth(srcWidth,       srcHeight,
													srcRowBytes,  qd3dBasePtr,
													srcPixelType, srcByteOrder, glPixelType);



	// Release the QD3D texture data
	ir_state_texture_release_data(theStorage, qd3dBasePtr, wasCopied);



	// Bail if the depth copy failed
	if (depthBasePtr == NULL)
		return(NULL);



	// If the image isn't a power of 2 in size, we also need to resize it		
	if (!ir_state_is_power_of_2(srcWidth) || !ir_state_is_power_of_2(srcHeight))
		{
		// Resize the image
		sizeBasePtr = ir_state_texture_convert_size(srcWidth, srcHeight, srcRowBytes, depthBasePtr,
													dstWidth, dstHeight, dstRowBytes);


		// Release the depth copy of the texture
		E3Memory_Free(&depthBasePtr);


		// Replace the depth copy with the size copy
		depthBasePtr = sizeBasePtr;
		}


	// Otherwise, use the size as-is (setting rowBytes for the 32bpp output image)
	else
		{
		*dstWidth    = srcWidth;
		*dstHeight   = srcHeight;
		*dstRowBytes = (srcWidth * 4);
		}



	// We're done - the final image is 32bpp RGBA, and a power of 2 in size
	return(depthBasePtr);
}





//=============================================================================
//      ir_state_texture_convert_pixmap : Convert a QD3D Pixmap texture.
//-----------------------------------------------------------------------------
//		Note :	The texture state has already been set - we just need to
//				extract the image data and pass it to OpenGL.
//-----------------------------------------------------------------------------
static TQ3Status
ir_state_texture_convert_pixmap(TQ3TextureObject theTexture)
{	TQ3Uns32				theWidth, theHeight, rowBytes;
	GLint					glPixelType;
	TQ3Status				qd3dStatus;
	TQ3StoragePixmap		thePixmap;
	TQ3Uns8					*basePtr;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3Texture_GetType(theTexture) == kQ3TextureTypePixmap, kQ3Failure);



	// Get the pixmap data
	qd3dStatus = Q3PixmapTexture_GetPixmap(theTexture, &thePixmap);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Get the image data for the pixmap in a form that OpenGL can accept
	basePtr = ir_state_texture_convert_image(thePixmap.image, thePixmap.pixelType,
												thePixmap.width,
												thePixmap.height,
												thePixmap.rowBytes,
												thePixmap.byteOrder,
												&theWidth,
												&theHeight,
												&rowBytes,
												&glPixelType);



	// Pass the image to OpenGL
	if (basePtr != NULL)
		glTexImage2D(GL_TEXTURE_2D, 0,						// Mip map level
						glPixelType,						// Requested final size of texture
						theWidth, theHeight, 0,				// Size of texture and border
						GL_RGBA, GL_UNSIGNED_BYTE,			// Format of input texture
						basePtr);							// Input texture



	// Clean up and return
	E3Object_DisposeAndForget(thePixmap.image);
	E3Memory_Free(&basePtr);
	
	return(kQ3Success);
}





//=============================================================================
//      ir_state_texture_convert_mipmap : Convert a QD3D Mipmap texture.
//-----------------------------------------------------------------------------
//		Note :	The texture state has already been set - we just need to
//				extract the image data and pass it to OpenGL.
//-----------------------------------------------------------------------------
static TQ3Status
ir_state_texture_convert_mipmap(TQ3TextureObject theTexture)
{	TQ3Uns32				theWidth, theHeight, rowBytes;
	TQ3Uns32				skipPixels, skipRows;
	TQ3Uns32				n, numImages;
	GLint					glPixelType;
	TQ3Status				qd3dStatus;
	TQ3Mipmap				theMipmap;
	TQ3Uns8					*basePtr;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3Texture_GetType(theTexture) == kQ3TextureTypeMipmap, kQ3Failure);



	// Get the mipmap data
	qd3dStatus = Q3MipmapTexture_GetMipmap(theTexture, &theMipmap);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Work out how many images are contained in the mipmap
	if (theMipmap.useMipmapping)
		{
		numImages = 0;
		theWidth  = theMipmap.mipmaps[0].width;
		while (theWidth > 1)
			{
			theWidth /= 2;
			numImages++;
			}
		}
	else
		numImages = 1;



	// Get the image data for the mipmap in a form that OpenGL can accept
	basePtr = ir_state_texture_convert_image(theMipmap.image, theMipmap.pixelType,
												theMipmap.mipmaps[0].width,
												theMipmap.mipmaps[0].height,
												theMipmap.mipmaps[0].rowBytes,
												theMipmap.byteOrder,
												&theWidth,
												&theHeight,
												&rowBytes,
												&glPixelType);



	// Pass the mipmap images to OpenGL
	if (basePtr != NULL)
		{
		skipPixels    = 0;
		skipRows      = 0;

		for (n = 0; n < numImages; n++)
			{
			// Locate the image within the mipmap
			glPixelStorei(GL_PACK_ROW_LENGTH,  theWidth);
			glPixelStorei(GL_PACK_SKIP_ROWS,   skipPixels);
			glPixelStorei(GL_PACK_SKIP_PIXELS, skipRows);



			// Pass this image to OpenGL
			glTexImage2D(GL_TEXTURE_2D, n,						// Mip map level
							glPixelType,						// Requested final size of texture
							theWidth, theHeight, 0,				// Size of texture and border
							GL_RGBA, GL_UNSIGNED_BYTE,			// Format of input texture
							basePtr);							// Input texture


			// Adjust our offsets for the next image
			skipPixels +=  theMipmap.mipmaps[n].width;
			skipRows   += (theMipmap.mipmaps[n].height / 2);
			}
		}



	// Clean up and return
	E3Object_DisposeAndForget(theMipmap.image);
	E3Memory_Free(&basePtr);
	
	return(kQ3Success);
}





//=============================================================================
//      ir_state_texture_convert_compressed_pixmap : Convert a QD3D compressed
//													 pixmap texture.
//-----------------------------------------------------------------------------
//		Note :	The texture state has already been set - we just need to
//				extract the image data and pass it to OpenGL.
//
//				Only available on platforms with QuickTime.
//-----------------------------------------------------------------------------
static TQ3Status
ir_state_texture_convert_compressed_pixmap(TQ3TextureObject theTexture)
{
#if QUESA_SUPPORT_QUICKTIME


	// To be implemented...
	return(kQ3Failure);
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      ir_state_texture_convert : Convert a QD3D texture to OpenGL.
//-----------------------------------------------------------------------------
static TQ3Status
ir_state_texture_convert(TQ3ViewObject			theView,
						 TQ3InteractiveData		*instanceData,
						 TQ3ShaderObject		theShader,
						 TQ3TextureObject		theTexture)
{	TQ3ShaderUVBoundary		qd3dBoundsU, qd3dBoundsV;
	GLint					glBoundsU, glBoundsV;
	GLfloat					glMatrix[16];
	TQ3RendererObject		theRenderer;
	TQ3Matrix3x3			qd3dMatrix;
	TQ3Status				qd3dStatus;
	TQ3Uns32				raveFilter;
	TQ3ObjectType			theType;



	// Create an OpenGL texture object for the texture
	Q3_ASSERT(!glIsTexture((GLuint) theTexture));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, (GLuint) theTexture);
	Q3_ASSERT(glIsTexture( (GLuint) theTexture));



	// Set up the texture state
	raveFilter = 0;
	Q3View_GetRenderer(theView, &theRenderer);
	if (theRenderer != NULL)
		{
		Q3InteractiveRenderer_GetRAVETextureFilter(theRenderer, &raveFilter);
		Q3Object_Dispose(theRenderer);
		}

	Q3Shader_GetUBoundary(theShader,   &qd3dBoundsU);
	Q3Shader_GetVBoundary(theShader,   &qd3dBoundsV);
	Q3Shader_GetUVTransform(theShader, &qd3dMatrix);

	GLUtils_ConvertUVBoundary(qd3dBoundsU, &glBoundsU);
	GLUtils_ConvertUVBoundary(qd3dBoundsV, &glBoundsV);

	glMatrix[0]  = qd3dMatrix.value[0][0];
	glMatrix[1]  = qd3dMatrix.value[0][1];
	glMatrix[2]  = qd3dMatrix.value[0][2];
	glMatrix[3]  = 0.0f;

	glMatrix[4]  = qd3dMatrix.value[1][0];
	glMatrix[5]  = qd3dMatrix.value[1][1] * -1.0f; // Flip QD3D UVs for OpenGL
	glMatrix[6]  = qd3dMatrix.value[1][2];
	glMatrix[7]  = 0.0f;

	glMatrix[8]  = qd3dMatrix.value[2][0];
	glMatrix[9]  = qd3dMatrix.value[2][1];
	glMatrix[10] = qd3dMatrix.value[2][2];
	glMatrix[11] = 0.0f;

	glMatrix[12] = qd3dMatrix.value[3][0];
	glMatrix[13] = qd3dMatrix.value[3][1];
	glMatrix[14] = qd3dMatrix.value[3][2];
	glMatrix[15] = 1.0f;

	glTexEnvi(      GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,   GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S,     glBoundsU);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T,     glBoundsV);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, raveFilter ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, raveFilter ? GL_NEAREST : GL_LINEAR);

	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(glMatrix);



	// Set up the texture data
	theType = Q3Texture_GetType(theTexture);
	switch (theType) {
		case kQ3TextureTypePixmap:
			qd3dStatus = ir_state_texture_convert_pixmap(theTexture);
			break;
	
		case kQ3TextureTypeMipmap:
			qd3dStatus = ir_state_texture_convert_mipmap(theTexture);
			break;

		case kQ3TextureTypeCompressedPixmap:
			qd3dStatus = ir_state_texture_convert_compressed_pixmap(theTexture);
			break;

		default:
			qd3dStatus = kQ3Success;
			break;
		}



	// Save a reference to the QD3D texture so that we can tell when it's time to dispose of the
	// OpenGL texture. If we could register a callback with Quesa to find out when an object was
	// disposed of, we could do away with this caching system - its only purpose is to let us
	// know when it's time to release the OpenGL texture.
	if (qd3dStatus == kQ3Success)
		{
		qd3dStatus = E3Memory_Reallocate(&instanceData->textureCache,
										 sizeof(TQ3TextureObject) * (instanceData->textureCount+1));
		if (qd3dStatus == kQ3Success)
			{
			instanceData->textureCache[instanceData->textureCount] = Q3Shared_GetReference(theTexture);
			instanceData->textureCount++;
			}
		}



	// Handle failure and return
	if (qd3dStatus != kQ3Success)
		glDeleteTextures(1, (GLuint *) &theTexture);

	return(qd3dStatus);
}





//=============================================================================
//      ir_state_adjust_texture_lighting : Adjust our state for a geometry.
//-----------------------------------------------------------------------------
static void
ir_state_adjust_texture_lighting(TQ3InteractiveData *instanceData)
{


	// Check to see if we need to force vertex colours to white.
	//
	// To produce the same effect as QD3D (i.e., textured geometry is still
	// illuminated by lights), we need to force the vertex colour to white
	// under some circumstances to get the correct effect. This seems slightly
	// odd, so if you know of a better way please let us know!
	//
	// If we're using anything than a NULL illumination, and we're rendering
	// in filled mode, we need to force the vertex colours to white.
	instanceData->stateTextureForceWhite = (TQ3Boolean)
		(instanceData->stateTextureActive    &&
		 instanceData->stateViewIllumination != kQ3IlluminationTypeNULL &&
		 instanceData->stateFill             == kQ3FillStyleFilled);
}





//=============================================================================
//      ir_state_adjust_geom : Adjust our state for a geometry.
//-----------------------------------------------------------------------------
static void
ir_state_adjust_geom(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes)
{	TQ3XAttributeMask		theMask;



	// Get the attribute set mask
    if (theAttributes != NULL)
        theMask = Q3XAttributeSet_GetMask(theAttributes);
	else
		theMask = kQ3XAttributeMaskNone;



	// Update our state to reflect the attribute set
	if (theMask & kQ3XAttributeMaskDiffuseColor)
		instanceData->stateGeomDiffuseColour = (TQ3ColorRGB *) 
                                          			Q3XAttributeSet_GetPointer(theAttributes,
                                        			kQ3AttributeTypeDiffuseColor);
        
    if (theMask & kQ3XAttributeMaskSpecularColor)
        instanceData->stateGeomSpecularColour = (TQ3ColorRGB *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeSpecularColor);

    if (theMask & kQ3XAttributeMaskTransparencyColor)
        instanceData->stateGeomTransparencyColour = (TQ3ColorRGB *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeTransparencyColor);

    if (theMask & kQ3XAttributeMaskSpecularControl)
        instanceData->stateGeomSpecularControl = * ((float *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeSpecularControl));

    if (theMask & kQ3XAttributeMaskHighlightState)
        instanceData->stateGeomHilightState = * ((TQ3Switch *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeHighlightState));

	Q3_ASSERT(instanceData->stateGeomDiffuseColour      != NULL);
	Q3_ASSERT(instanceData->stateGeomSpecularColour     != NULL);
	Q3_ASSERT(instanceData->stateGeomTransparencyColour != NULL);
}





//=============================================================================
//      ir_state_adjust_gl : Adjust the OpenGL state for a geometry.
//-----------------------------------------------------------------------------
//		Note :	Although QD3D allows us to specify some attributes at a
//				per-vertex level (e.g., specular colour), we currently support
//				these attributes at the geometry level with OpenGL (to minimise
//				the number of material changes we need to make).
//
//				May be possible to improve this if we keep track of what the
//				state was at the previous vertex and only update the OpenGL
//				state when it changes? 
//-----------------------------------------------------------------------------
static void
ir_state_adjust_gl(TQ3InteractiveData *instanceData)
{	GLfloat		specularControl;



	// If we're using Phong illumination, update the specular colour and control
	if (instanceData->stateViewIllumination == kQ3IlluminationTypePhong)
		{
		// Update the specular colour if it's changed
		if (instanceData->stateGeomSpecularColour->r != instanceData->stateCurrentSpecularColour[0] ||
			instanceData->stateGeomSpecularColour->g != instanceData->stateCurrentSpecularColour[1] ||
			instanceData->stateGeomSpecularColour->b != instanceData->stateCurrentSpecularColour[2])
			{
			instanceData->stateCurrentSpecularColour[0] = instanceData->stateGeomSpecularColour->r;
			instanceData->stateCurrentSpecularColour[1] = instanceData->stateGeomSpecularColour->g;
			instanceData->stateCurrentSpecularColour[2] = instanceData->stateGeomSpecularColour->b;
			instanceData->stateCurrentSpecularColour[3] = 1.0f;

			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, instanceData->stateCurrentSpecularColour);
			}
			


		// Update the specular control if it's changed. There doesn't seem to be an
		// exact mapping between the lighting controls in QD3D and in OpenGL, so we
		// use this hack for now.
		if (instanceData->stateGeomSpecularControl != instanceData->stateCurrentSpecularControl)
			{
			instanceData->stateCurrentSpecularControl = instanceData->stateGeomSpecularControl;
			specularControl                           = instanceData->stateCurrentSpecularControl;
			
			if (specularControl < 10.0f)
				specularControl *= 10.0f;

			else if (specularControl < 20.0f)
				specularControl *= 3.5f;

			else if (specularControl < 40.0f)
				specularControl *= 3.0f;

			else if (specularControl < 60.0f)
				specularControl *= 2.25f;

			else if (specularControl < 100.0f)
				specularControl *= 1.8f;

			else if (specularControl < 128.0f)
				specularControl *= 1.24f;

			else
				specularControl = 128.0f;

			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &specularControl);
			}
		}



	// If we're to force vertex colours to white (for texturing), set the current
	// colour to white so we don't have to explicitly set it for each geometry.
	if (instanceData->stateTextureForceWhite)
		glColor3f(1.0f, 1.0f, 1.0f);
}





//=============================================================================
//      ir_state_reset : Reset our state to the defaults.
//-----------------------------------------------------------------------------
static void
ir_state_reset(TQ3InteractiveData *instanceData)
{	TQ3Uns32		n;



    // Reset our state
    instanceData->stateFill    = kQ3FillStyleFilled;
    instanceData->stateHilight = NULL;
    Q3ColorRGB_Set(&instanceData->stateDefaultDiffuseColour,      kQ3ViewDefaultDiffuseColor);
    Q3ColorRGB_Set(&instanceData->stateDefaultSpecularColour,     kQ3ViewDefaultSpecularColor);
    Q3ColorRGB_Set(&instanceData->stateDefaultTransparencyColour, kQ3ViewDefaultTransparency);

	for (n = 0; n < 4; n++)
		instanceData->stateCurrentSpecularColour[n] = -1.0f;

	instanceData->stateCurrentSpecularControl = -1.0f;
	instanceData->stateTextureActive          = kQ3False;
	instanceData->stateTextureForceWhite      = kQ3False;

    instanceData->stateGeomDiffuseColour      = &instanceData->stateDefaultDiffuseColour;
    instanceData->stateGeomSpecularColour     = &instanceData->stateDefaultSpecularColour;
    instanceData->stateGeomTransparencyColour = &instanceData->stateDefaultTransparencyColour;
	instanceData->stateGeomSpecularControl    = kQ3ViewDefaultSpecularControl;
	instanceData->stateGeomHilightState       = kQ3Off;


    instanceData->stateViewDiffuseColour      = &instanceData->stateDefaultDiffuseColour;
    instanceData->stateViewSpecularColour     = &instanceData->stateDefaultSpecularColour;
    instanceData->stateViewTransparencyColour = &instanceData->stateDefaultTransparencyColour;
	instanceData->stateViewSpecularControl    = kQ3ViewDefaultSpecularControl;
	instanceData->stateViewHilightState       = kQ3Off;
	instanceData->stateViewIllumination       = kQ3ObjectTypeInvalid;
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRRenderer_State_Initialise : Update our state for a geometry.
//-----------------------------------------------------------------------------
#pragma mark -
void
IRRenderer_State_Initialise(TQ3InteractiveData *instanceData, TQ3ViewObject theView)
{


	// Reset the state
	ir_state_reset(instanceData);
}





//=============================================================================
//      IRRenderer_State_Terminate : Clean up our state.
//-----------------------------------------------------------------------------
void
IRRenderer_State_Terminate(TQ3InteractiveData *instanceData)
{


	// Dispose of our state
	if (instanceData->stateHilight != NULL)
		Q3Object_Dispose(instanceData->stateHilight);



	// Flush the texture cache (perhaps only do if enough time has
	// passed, rather than on each frame?)
	IRRenderer_State_FlushTextureCache(instanceData, kQ3False);



	// Reset the state
	ir_state_reset(instanceData);
}





//=============================================================================
//      IRRenderer_State_FlushTextureCache : Flush the texture cache.
//-----------------------------------------------------------------------------
void
IRRenderer_State_FlushTextureCache(TQ3InteractiveData *instanceData, TQ3Boolean forceFlush)
{	TQ3Uns32		n;



	// If we're to flush everything, release the textures and reset
	// the texture cache to be completely empty.
	if (forceFlush)
		{
		// Dispose of the objects in the cache
		for (n = 0; n < instanceData->textureCount; n++)
			{
			Q3_ASSERT(glIsTexture((GLuint) instanceData->textureCache[n]));
			glDeleteTextures(1, (GLuint *) &instanceData->textureCache[n]);
			E3Object_DisposeAndForget(instanceData->textureCache[n]);
			}


		// Update the table
		instanceData->textureCount = 0;
		E3Memory_Free(&instanceData->textureCache);
		}
	
	
	
	// Otherwise we just dispose of any textures of which we're now the
	// last reference, and shuffle the rest of the cache down.
	//
	// Note that we don't bother resizing the cache table itself: this
	// will happen automatically when the next texture is created or
	// when the renderer is disposed of.
	else
		{
		// Dispose of any objects that we hold the last reference to
		for (n = 0; n < instanceData->textureCount; n++)
			{
			if (instanceData->textureCache[n] != NULL &&
				!Q3Shared_IsReferenced(instanceData->textureCache[n]))
				{
				// Release the texture
				Q3_ASSERT(glIsTexture((GLuint) instanceData->textureCache[n]));
				glDeleteTextures(1, (GLuint *) &instanceData->textureCache[n]);
				E3Object_DisposeAndForget(instanceData->textureCache[n]);



				// Shuffle the remaining slots down by 1 (using memmove
				// since we're copying data on top of itself)
				if (n < (instanceData->textureCount-1))
					memmove(&instanceData->textureCache[n],
							&instanceData->textureCache[n+1],
							instanceData->textureCount - 1 - n);



				// Update the table
				instanceData->textureCount--;
				}
			}
		}
}





//=============================================================================
//      IRRenderer_State_Update : Update our state for a geometry.
//-----------------------------------------------------------------------------
//		Note :	Called by every geometry before they submit themselves in order
//				to update the current QD3D state with the state of the geom.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_State_Update(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes)
{


	// Reset the geometry state to the current view state
	instanceData->stateGeomDiffuseColour      = instanceData->stateViewDiffuseColour;
    instanceData->stateGeomSpecularColour     = instanceData->stateViewSpecularColour;
    instanceData->stateGeomTransparencyColour = instanceData->stateViewTransparencyColour;
    instanceData->stateGeomSpecularControl    = instanceData->stateViewSpecularControl;
    instanceData->stateGeomHilightState       = instanceData->stateViewHilightState;



	// Update the geometry state with its attribute set and any hilights
	ir_state_adjust_geom(instanceData, theAttributes);

    if (instanceData->stateGeomHilightState == kQ3On && instanceData->stateHilight != NULL)
        ir_state_adjust_geom(instanceData, instanceData->stateHilight);



	// Update the GL state for this geometry
	ir_state_adjust_gl(instanceData);

    return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Texture_Preamble : Update the texture mapping state.
//-----------------------------------------------------------------------------
//		Note :	Called by geometries which can be textured mapped, to allow us
//				to update the OpenGL texture state to produce the correct
//				effect.
//
//				If the attribute set contains a texture map shader, we need to
//				submit it by hand, to apply the texture map to this geometry in
//				the same way that Apple's Interactive Renderer does.
//-----------------------------------------------------------------------------
void
IRRenderer_Texture_Preamble(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3AttributeSet			theAttributes)
{	TQ3ShaderObject		*theShader;
	TQ3XAttributeMask	theMask;



	// Check to see if the attribute set contains a texture
	theMask = Q3XAttributeSet_GetMask(theAttributes);
	if ((theMask & kQ3XAttributeMaskSurfaceShader) != 0)
		{
		// Get the texture
		theShader = (TQ3ShaderObject *) Q3XAttributeSet_GetPointer(theAttributes, kQ3AttributeTypeSurfaceShader);
		if (theShader != NULL && *theShader != NULL)
			{
			// Apply it, and update the GL state
			IRRenderer_Update_Shader_Surface(theView, instanceData, theShader);
			ir_state_adjust_gl(instanceData);
			}
		}
}





//=============================================================================
//      IRRenderer_Texture_Postamble : Update the texture mapping state.
//-----------------------------------------------------------------------------
//		Note :	Called by geometries which can be textured mapped, to allow us
//				to update the OpenGL texture state to produce the correct
//				effect.
//
//				We undo any temporary changes that were applied by the previous
//				call to IRRenderer_Texture_Preamble, and re-enable textures
//				which were turned off by geometries which found themselves
//				without UVs.
//
//				Note that the second step reverses a glDisable carried out by
//				the geometry callbacks themselves - IRRenderer_Texture_Preamble
//				wasn't able to do it for them, since at the time at which it
//				was called the geometries hadn't checked their UVs (they don't
//				bother searching for UVs if there isn't a texture active).
//-----------------------------------------------------------------------------
void
IRRenderer_Texture_Postamble(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3AttributeSet			theAttributes,
							TQ3Boolean				canTexture)
{	TQ3XAttributeMask	theMask;



	// If we can texture map, remove any texture applied by the attribute set.
	if (canTexture)
		{
		theMask = Q3XAttributeSet_GetMask(theAttributes);
		if ((theMask & kQ3XAttributeMaskSurfaceShader) != 0)
			IRRenderer_Update_Shader_Surface(theView, instanceData, NULL);
		}
	
	
	// Otherwise, check to see if a texture is currently active. If it is, we
	// need to turn it back on again for future objects.
	else if (instanceData->stateTextureForceWhite)
		glEnable(GL_TEXTURE_2D);
}





//=============================================================================
//      IRRenderer_Update_Matrix_LocalToCamera : Update our state.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
IRRenderer_Update_Matrix_LocalToCamera(TQ3ViewObject			theView,
											TQ3InteractiveData	*instanceData,
											TQ3Matrix4x4		*theMatrix)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Set up the model-view transform
	GLCamera_SetModelView(theMatrix);

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Interpolation : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Interpolation(TQ3ViewObject			theView,
									TQ3InteractiveData		*instanceData,
									TQ3InterpolationStyle	*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Set the interpolation style
	//
	// OpenGL has two shading model, flat and smooth (Gouraud shading).
	// Like QD3D's IR, we treat kQ3InterpolationStyleVertex as being
	// equivalent to kQ3InterpolationStylePixel.
	switch (*styleData) {
		case kQ3InterpolationStyleNone:
			glShadeModel(GL_FLAT);
			break;

		case kQ3InterpolationStyleVertex:
		case kQ3InterpolationStylePixel:
			glShadeModel(GL_SMOOTH);
		default:
			break;
		}	

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Backfacing : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Backfacing(TQ3ViewObject		theView,
									TQ3InteractiveData	*instanceData,
									TQ3BackfacingStyle	*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Set the backfacing style
	//
	// OpenGL doesn't do automatic flipping, so we have to just treat
	// that style as being equivalent to kQ3BackfacingStyleBoth.
	switch (*styleData) {
		case kQ3BackfacingStyleRemove:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;

		case kQ3BackfacingStyleFlip:
		case kQ3BackfacingStyleBoth:
		default:
			glDisable(GL_CULL_FACE);
			break;
		}	

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Fill : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Fill(TQ3ViewObject		theView,
							 TQ3InteractiveData	*instanceData,
							 TQ3FillStyle		*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Save and set the fill style
	instanceData->stateFill = *styleData;

	switch (instanceData->stateFill) {
		case kQ3FillStylePoints:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;

		case kQ3FillStyleEdges:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;

		case kQ3FillStyleFilled:
		default:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}	



	// If texture mapping is on, adjust the texture state
	if (instanceData->stateTextureActive)
		{
		if (instanceData->stateFill == kQ3FillStyleFilled)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		}



	// Update the texture lighting state
	ir_state_adjust_texture_lighting(instanceData);

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Orientation : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Orientation(TQ3ViewObject			theView,
									 TQ3InteractiveData		*instanceData,
									 TQ3OrientationStyle	*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Set the front facing direction
	switch (*styleData) {
		case kQ3OrientationStyleClockwise:
			glFrontFace(GL_CW);
			break;

		case kQ3OrientationStyleCounterClockwise:
		default:
			glFrontFace(GL_CCW);
			break;
		}

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Hilight : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Hilight(TQ3ViewObject			theView,
								 TQ3InteractiveData		*instanceData,
								 TQ3AttributeSet		*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Save the hilight attribute set
	E3Shared_Replace(&instanceData->stateHilight, *styleData);

    return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_AntiAlias : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_AntiAlias(TQ3ViewObject					theView,
									TQ3InteractiveData			*instanceData,
									TQ3AntiAliasStyleData		*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Turn everything off
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);



	// Turn things back on as required
	if (styleData->state == kQ3On)
		{
		// Set up our aliasing thresholds
		glPointSize(kAAPointSize);
		glLineWidth(kAALineSize);


		// Always do points
		glEnable(GL_POINT_SMOOTH);


		// Do lines or edges as required
		if (styleData->mode == kQ3AntiAliasModeMaskEdges)
			glEnable(GL_LINE_SMOOTH);
		else
			glEnable(GL_POLYGON_SMOOTH);
		}

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Fog : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Fog(TQ3ViewObject		theView,
							 TQ3InteractiveData	*instanceData,
							 TQ3FogStyleData	*styleData)
{	GLfloat		fogColour[4];
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Set up the fog state
	if (styleData->state == kQ3On)
		{
		// Set up the colour
		fogColour[0] = (GLfloat) styleData->color.r;
		fogColour[1] = (GLfloat) styleData->color.g;
		fogColour[2] = (GLfloat) styleData->color.b;
		fogColour[3] = (GLfloat) styleData->color.a;



		// Set up the fog state
		glEnable(GL_FOG);
		glFogf(GL_FOG_DENSITY, styleData->density);
		glFogf(GL_FOG_START,   styleData->fogStart);
		glFogf(GL_FOG_END,     styleData->fogEnd);
		glFogfv(GL_FOG_COLOR,  fogColour);



		// Set the fog mode (alpha fog is not supported in OpenGL)
		switch (styleData->mode) {
			case kQ3FogModeLinear:
				glFogi(GL_FOG_MODE, GL_LINEAR);
				break;
				
			case kQ3FogModeExponential:
				glFogi(GL_FOG_MODE, GL_EXP);
				break;
				
			case kQ3FogModeExponentialSquared:
				glFogi(GL_FOG_MODE, GL_EXP2);
				break;
				
			case kQ3FogModeAlpha:
			default:
				glFogi(GL_FOG_MODE, GL_LINEAR);
				break;
			}
		}


	// Or turn the fog off
	else
		glDisable(GL_FOG);

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_DiffuseColour : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_DiffuseColour(TQ3ViewObject			theView,
							 			  TQ3InteractiveData	*instanceData,
							 			  TQ3ColorRGB			*attributeData)
{


	// Update our state
	instanceData->stateViewDiffuseColour = attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_SpecularColour : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_SpecularColour(TQ3ViewObject		theView,
							 			  	TQ3InteractiveData	*instanceData,
							 			  	TQ3ColorRGB			*attributeData)
{


	// Update our state
	instanceData->stateViewSpecularColour = attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_SpecularControl : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_SpecularControl(TQ3ViewObject		theView,
							 			  	TQ3InteractiveData	*instanceData,
							 			  	float				*attributeData)
{


	// Update our state
	instanceData->stateViewSpecularControl = *attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_HilightState : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_HilightState(TQ3ViewObject			theView,
							 			 TQ3InteractiveData		*instanceData,
							 			 TQ3Switch				*attributeData)
{


	// Update our state
	instanceData->stateViewHilightState = *attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_TransparencyColour : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_TransparencyColour(TQ3ViewObject		theView,
							 					TQ3InteractiveData	*instanceData,
							 					TQ3ColorRGB			*attributeData)
{


	// Update our state
	instanceData->stateViewTransparencyColour = attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Shader_Illumination : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Shader_Illumination(TQ3ViewObject			theView,
							 			TQ3InteractiveData	*instanceData,
							 			TQ3ShaderObject		*shaderData)
{	GLfloat		specularColour[4]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat		specularControl[1] = { 0.0f };



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Update our state
    if (shaderData != NULL && *shaderData != NULL)
        instanceData->stateViewIllumination = Q3IlluminationShader_GetType(*shaderData);
    else
        instanceData->stateViewIllumination = kQ3ObjectTypeInvalid;



	// Update the OpenGL state
	switch (instanceData->stateViewIllumination) {
		case kQ3IlluminationTypeNULL:
			glDisable(GL_LIGHTING);
			break;
		
		case kQ3IlluminationTypeLambert:
			glEnable(GL_LIGHTING);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specularColour);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, specularControl);
			break;
		
		case kQ3IlluminationTypePhong:
			glEnable(GL_LIGHTING);
			break;
		}



	// Update the texture lighting state
	ir_state_adjust_texture_lighting(instanceData);

    return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Shader_Surface : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Shader_Surface(TQ3ViewObject			theView,
							 		TQ3InteractiveData	*instanceData,
							 		TQ3ShaderObject		*shaderData)
{	TQ3TextureObject	theTexture;
	TQ3Status			qd3dStatus;
	TQ3ObjectType		theType;



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Get the texture from the shader
	theTexture = NULL;

	if (shaderData != NULL && *shaderData != NULL)
		{
		theType = Q3SurfaceShader_GetType(*shaderData);
		if (theType == kQ3SurfaceShaderTypeTexture)
			Q3TextureShader_GetTexture(*shaderData, &theTexture);
		}



	// If we don't have a texture, disable texture mapping
	instanceData->stateTextureActive = (TQ3Boolean) (theTexture != NULL);
	if (!instanceData->stateTextureActive)
		{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		}



	// Otherwise we need to update the texture state
	else
		{
		// If we don't have a texture object for this texture, create it
		qd3dStatus = kQ3Success;
		if (!glIsTexture((GLuint) theTexture))
			qd3dStatus = ir_state_texture_convert(theView, instanceData, *shaderData, theTexture);


		// Enable the texture object
		if (qd3dStatus == kQ3Success)
			{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, (GLuint) theTexture);
			}
		}



	// Update the texture lighting state
	ir_state_adjust_texture_lighting(instanceData);

    return(kQ3Success);
}



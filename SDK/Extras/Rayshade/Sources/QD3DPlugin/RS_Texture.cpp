/*  NAME:
        RS_Texture.c

    DESCRIPTION:
        RayShade geometry handlers

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "RSPrefix.h"

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
	#include <QuesaIO.h>
	#include <QuesaShader.h>
	#include <QuesaStorage.h>
#else
	#include <QD3D.h>
	#include <QD3DIO.h>
	#include <QD3DShader.h>
	#include <QD3DStorage.h>
#endif

#include <stdlib.h>

#include "RS_Texture.h"

#include "RT_Texture.h"
/*===========================================================================*\
 *
 *	Routine:	rs_GetStorageData()
 *
 *	Comments:	Gets the storage data of the object
 *
\*===========================================================================*/
static
TQ3Uns8 *
rs_GetStorageData(
		TQ3StorageObject	theStorage,
		TQ3Boolean			*outWasCopied)
{
	TQ3Status theStatus;
	TQ3Uns8	*result;
	TQ3Uns32 validSize,bufferSize;
	TQ3ObjectType theType;
	
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theStorage), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outWasCopied), NULL);	
	
	result = NULL;
	*outWasCopied = kQ3False;
	theType = Q3Storage_GetType(theStorage);
	switch(theType)
	{
		case kQ3StorageTypeMemory:
			theStatus = Q3MemoryStorage_GetBuffer(
								theStorage,
								&result,
								&validSize,
								&bufferSize);
			break;
#if defined(OS_MACINTOSH) && OS_MACINTOSH
		case kQ3MemoryStorageTypeHandle:
		{
			Handle	theHandle;
			
			theStatus = Q3HandleStorage_Get(theStorage, &theHandle, &validSize);
			if (theStatus == kQ3Success && theHandle != NULL)
			{
				MoveHHi(theHandle);
				HLock(theHandle);
				result = (TQ3Uns8*) *theHandle;
			}
			break;
		}
#endif
		default:
			theStatus = Q3Storage_GetSize(theStorage, &bufferSize);
			if (theStatus == kQ3Success)
				result = (TQ3Uns8*)malloc(bufferSize);
			
			if (result != NULL)
			{
				theStatus = Q3Storage_GetData(theStorage, 0, bufferSize, result, &validSize);
				*outWasCopied = (TQ3Boolean) (theStatus == kQ3Success);
				if (theStatus != kQ3Success)
				{
					result = NULL;
					free(result);
				}
			}			
			break;				
	}
	
	if (theStatus != kQ3Success)
		return NULL;
		
	return result;
}
/*===========================================================================*\
 *
 *	Routine:	rs_ReleasetStorageData()
 *
 *	Comments:	Releases the storage data of the object
 *
\*===========================================================================*/
static
void
rs_ReleaseStorageData(
		TQ3StorageObject	theStorage,
		TQ3Uns8				*basePtr,
		TQ3Boolean			wasCopied)
{
#if defined(macintosh)
	TQ3Status			theStatus;
	TQ3Uns32			validSize;
	TQ3ObjectType		theType;
	Handle				theHandle;
	
	theType = Q3Storage_GetType(theStorage);
	if (theType == kQ3MemoryStorageTypeHandle)
	{
		theStatus = Q3HandleStorage_Get(theStorage, &theHandle, &validSize);
		if (theStatus == kQ3Success && theHandle != NULL)
			HUnlock(theHandle);
	}
#endif	
	
	if (wasCopied)
		free(basePtr);
}
/*===========================================================================*\
 *
 *	Routine:	rs_TextureConvertMemory()
 *
 *	Comments:	Converts the texture image to the closest texture format
 *
\*===========================================================================*/
static
TQ3Uns8*
rs_TextureConvertMemory(
		TRSRendererPrivate		* /*rsPrivate*/,
		TQ3PixelType			srcPixelType,
		TQ3Uns32				srcWidth,
		TQ3Uns32				srcHeight,
		TQ3Uns32				srcRowBytes,
		TQ3Uns8					*srcBaseAddr,
		int						*dstWidth,
		int						*dstHeight,
		TQ3Uns32				*dstRowBytes,
		TRTPixelType			*dstPixelType)
{
	TQ3Uns8 	*dstBasePtr;
	int			y,x;
	int			pixelSize;
	int			srcPixelSize;
	TQ3Uns8		*srcRow,*dstRow;
	/*
	 * Convert to RBG32 format:
	 */
	pixelSize = 4;
	dstBasePtr = (TQ3Uns8*)malloc(srcWidth*srcHeight*pixelSize);
	if (dstBasePtr == NULL)
		return NULL;
	
	*dstRowBytes = srcWidth*pixelSize;
	
	srcRow = srcBaseAddr;
	dstRow = dstBasePtr;
	
	switch(srcPixelType)
	{
		case kQ3PixelTypeARGB32:
		case kQ3PixelTypeRGB32:
			srcPixelSize = 4;
			break;
		case kQ3PixelTypeRGB24:
			srcPixelSize = 3;
			break;
		case kQ3PixelTypeARGB16:
		case kQ3PixelTypeRGB16:
		case kQ3PixelTypeRGB16_565:
			srcPixelSize = 2;
			break;
	}
	
	
	for (y = 0; y < (int)srcHeight; y++)
	{
		TQ3Uns8 *srcPixel,*dstPixel;
		TQ3Uns32 alphaBits,redBits,greenBits,blueBits;
		srcPixel = srcRow;
		dstPixel = dstRow;
		for (x = 0; x < (int)srcWidth; x++)
		{
			TQ3Uns32 n;
			switch (srcPixelType) {
				case kQ3PixelTypeARGB32:
					dstPixel[1] = srcPixel[1];
					dstPixel[2] = srcPixel[2];
					dstPixel[3] = srcPixel[3];
					dstPixel[0] = srcPixel[0];
					break;

				case kQ3PixelTypeRGB32:
					dstPixel[1] = srcPixel[1];
					dstPixel[2] = srcPixel[2];
					dstPixel[3] = srcPixel[3];
					dstPixel[0] = 0xFF;
					break;

				case kQ3PixelTypeRGB24:
					dstPixel[1] = srcPixel[0];
					dstPixel[2] = srcPixel[1];
					dstPixel[3] = srcPixel[2];
					dstPixel[0] = 0xFF;
					break;

				case kQ3PixelTypeARGB16:
				    n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
					alphaBits = (n >> 15) & 0x0001;
					redBits   = (n >> 10) & 0x001F;
					greenBits = (n >>  5) & 0x001F;
					blueBits  = (n >>  0) & 0x001F;
							
					dstPixel[1] = (TQ3Uns8) (redBits   * 8);
					dstPixel[2] = (TQ3Uns8) (greenBits * 8);
					dstPixel[3] = (TQ3Uns8) (blueBits  * 8);
					dstPixel[0] = (TQ3Uns8) (alphaBits * 0xFF);
					break;

				case kQ3PixelTypeRGB16:
					n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
					redBits   = (n >> 10) & 0x001F;
					greenBits = (n >>  5) & 0x001F;
					blueBits  = (n >>  0) & 0x001F;

					dstPixel[1] = (TQ3Uns8) (redBits   * 8);
					dstPixel[2] = (TQ3Uns8) (greenBits * 8);
					dstPixel[3] = (TQ3Uns8) (blueBits  * 8);
					dstPixel[0] = 0xFF;
					break;

				case kQ3PixelTypeRGB16_565:
					n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
					redBits   = (n >> 11) & 0x001F;
					greenBits = (n >>  5) & 0x003F;
					blueBits  = (n >>  0) & 0x001F;

					dstPixel[1] = (TQ3Uns8) (redBits   * 8);
					dstPixel[2] = (TQ3Uns8) (greenBits * 4);
					dstPixel[3] = (TQ3Uns8) (blueBits  * 8);
					dstPixel[0] = 0xFF;
					break;
				default:
					Q3_ASSERT(false);
					break;
				}
				srcPixel += srcPixelSize;
				dstPixel += pixelSize;
		}
				
		srcRow += srcRowBytes;
		dstRow += *dstRowBytes;
	}
	*dstWidth = (int)srcWidth;
	*dstHeight = (int)srcHeight;
	*dstPixelType = kRTPixelType_RGB32;
	return dstBasePtr;
}
/*===========================================================================*\
 *
 *	Routine:	rs_TextureContevert_Image()
 *
 *	Comments:	Converts the texture image to the closest texture format
 *
\*===========================================================================*/
static 
TQ3Uns8*
rt_TextureConvertImage(
		TRSRendererPrivate		*rsPrivate,
		TQ3StorageObject		theStorage,
		TQ3PixelType			srcPixelType,
		TQ3Uns32				srcWidth,
		TQ3Uns32				srcHeight,
		TQ3Uns32				srcRowBytes,
		int						*dstWidth,
		int						*dstHeight,
		TQ3Uns32				*dstRowBytes,
		TRTPixelType			*dstPixelType)
{
	TQ3Boolean	wasCopied;
	TQ3Uns8		*srcBasePtr;
	TQ3Uns8		*dstBasePtr;
	
	dstBasePtr = NULL;
	
	srcBasePtr = NULL;
	wasCopied = kQ3False;
	
	*dstWidth    = 0;
	*dstHeight   = 0;
	*dstRowBytes = 0;
	*dstPixelType = kRTPixelType_RGB32;
	
	
	srcBasePtr = rs_GetStorageData(theStorage,&wasCopied);
	if (!srcBasePtr)
		goto cleanup;
	

	dstBasePtr = rs_TextureConvertMemory(
					rsPrivate,srcPixelType,srcWidth,srcHeight,srcRowBytes,srcBasePtr,
					dstWidth,dstHeight,dstRowBytes,dstPixelType);
	
	
	if (srcBasePtr) rs_ReleaseStorageData(theStorage,srcBasePtr,wasCopied);
	return dstBasePtr;
cleanup:
	if (srcBasePtr) rs_ReleaseStorageData(theStorage,srcBasePtr,wasCopied);
	if (dstBasePtr) free(dstBasePtr);
	return NULL;	
}
		
	
/*===========================================================================*\
 *
 *	Routine:	RS_DefineTexture_PixMap()
 *
 *	Comments:	Defines the texture number.
 *
\*===========================================================================*/
static
TQ3Status
RS_DefineTexture_PixMap(
		TRSRendererPrivate		*rsPrivate,
		int						inTextureId,
		TQ3TextureObject		inTextureObject)
{	
	int			theWidth,theHeight;
	TQ3Uns32			rowBytes;
	TQ3Status			result;
	TQ3StoragePixmap	thePixmap;
	TQ3Uns8				*basePtr;
	TRTPixelType		thePixelType;
	
	basePtr = NULL;
	
	result = Q3PixmapTexture_GetPixmap(inTextureObject, &thePixmap);
	if (result != kQ3Success)
		goto cleanup;
	
	basePtr = rt_TextureConvertImage(
							rsPrivate,
							thePixmap.image,
							thePixmap.pixelType,
							thePixmap.width,
							thePixmap.height,
							thePixmap.rowBytes,
							&theWidth,
							&theHeight,
							&rowBytes,
							&thePixelType);
	if (!basePtr)
	{
		result = kQ3Failure;
		goto cleanup;
	}
	
	result = RT_DefineTexture(
					rsPrivate->raytracer,
					inTextureId,
					theWidth,
					theHeight,
					(int)rowBytes,
					(void*)basePtr,
					thePixelType);
	if (result != kQ3Success)
		goto cleanup;
	
	if (basePtr) free(basePtr);
	return kQ3Success;
cleanup:
	if (basePtr) free(basePtr);
	return result;
}
/*===========================================================================*\
 *
 *	Routine:	RS_DefineTexture_PixMap()
 *
 *	Comments:	Defines the texture number.
 *
\*===========================================================================*/
static
TQ3Status
RS_DefineTexture_MipMap(
		TRSRendererPrivate		*rsPrivate,
		int						inTextureId,
		TQ3TextureObject		inTextureObject)
{	
	int			theWidth,theHeight;
	TQ3Uns32			rowBytes;
	TQ3Status			result;
	TQ3Mipmap			theMipmap;
	TQ3Uns8				*basePtr;
	TRTPixelType		thePixelType;
	
	basePtr = NULL;
	
	result = Q3MipmapTexture_GetMipmap(inTextureObject, &theMipmap);
	if (result != kQ3Success)
		goto cleanup;
	
	basePtr = rt_TextureConvertImage(
							rsPrivate,
							theMipmap.image,
							theMipmap.pixelType,
							theMipmap.mipmaps[0].width,
							theMipmap.mipmaps[0].height,
							theMipmap.mipmaps[0].rowBytes,
							&theWidth,
							&theHeight,
							&rowBytes,
							&thePixelType);
	if (!basePtr)
	{
		result = kQ3Failure;
		goto cleanup;
	}
	
	result = RT_DefineTexture(
					rsPrivate->raytracer,
					inTextureId,
					theWidth,
					theHeight,
					(int)rowBytes,
					(void*)basePtr,
					(TRTPixelType)thePixelType);
	if (result != kQ3Success)
		goto cleanup;
	
	if (basePtr) free(basePtr);
	return kQ3Success;
cleanup:
	if (basePtr) free(basePtr);
	return result;
}
/*===========================================================================*\
 *
 *	Routine:	RS_DefineTexture()
 *
 *	Comments:	Defines the texture number.
 *
\*===========================================================================*/
static TQ3Status
RS_DefineTexture(
		TRSRendererPrivate		*rsPrivate,
		int						inTextureId,
		TQ3TextureObject		inTextureObject)
{	
	TQ3ObjectType	theType;
	TQ3Status 		result;
	
	theType = Q3Texture_GetType(inTextureObject);
	switch(theType)
	{
		case kQ3TextureTypePixmap:
			result = RS_DefineTexture_PixMap(rsPrivate,inTextureId,inTextureObject);
			if (result != kQ3Success) goto cleanup;
			break;
		case kQ3TextureTypeMipmap:
			result = RS_DefineTexture_MipMap(rsPrivate,inTextureId,inTextureObject);
			if (result != kQ3Success) goto cleanup;
			break;
		default:
			return kQ3Failure;
			break;
	}
	return kQ3Success;
cleanup:
	return result;
}
/*===========================================================================*\
 *
 *	Routine:	RS_SetShaderSurface()
 *
 *	Comments:	Updates the attributes if neccesary.
 *
 *  ToDo: Texture claming/repeat is ignored.
 *
\*===========================================================================*/
TQ3Status
RS_SetShaderSurface(
		TRSRendererPrivate			*rsPrivate,
		TQ3ShaderObject				*inShader)
{
	TQ3TextureObject	theTexture;
	TQ3Status			result;
	
	theTexture = NULL;
	/*
	 * Get the texture from shader
	 */
	if (inShader != NULL && *inShader != NULL)
	{
		TQ3ObjectType	theType;
		
		theType = Q3SurfaceShader_GetType(*inShader);
		if (theType == kQ3SurfaceShaderTypeTexture)
		{
			Q3TextureShader_GetTexture(*inShader, &theTexture);
		}
	}
	
	/*
	 * If we don't have texture mapping disable it
	 */
	if (theTexture == NULL)
	{
		RT_SetCurrentTexture(rsPrivate->raytracer,kRTTexture_None);
		return kQ3Success;
	}
	
	
	/*
	 * Update the texture state otherwise:
	 */
	if (!RT_IsTextureDefined(rsPrivate->raytracer,(int)theTexture))
	{
		result = RS_DefineTexture(rsPrivate,(int)theTexture,theTexture);
		if (result != kQ3Success)
		{
			RT_SetCurrentTexture(rsPrivate->raytracer,kRTTexture_None);
			goto cleanup;
		}
	}
	/*
	 * Set the texture to the current one
	 */
	result = RT_SetCurrentTexture(rsPrivate->raytracer,(int)theTexture);
	/*
	 * Set UV transformation
	 */
	Q3Shader_GetUVTransform(*inShader, &rsPrivate->uvTransform);

	rsPrivate->uvTransform.value[1][1] *= -1.0f;
	
	if (theTexture) Q3Object_Dispose(theTexture);

	return kQ3Success;
cleanup:
	if (theTexture) Q3Object_Dispose(theTexture);
	return result;
}		
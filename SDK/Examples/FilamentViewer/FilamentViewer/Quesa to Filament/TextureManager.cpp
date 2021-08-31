//
//  TextureManager.cpp
//  FilamentViewer
//
//  Created by James Walker on 2/5/21.
//
/* ___________________________________________________________________________
   COPYRIGHT:
        Copyright (c) 2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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

#include "TextureManager.h"

#include "LogToConsole.h"
#include "RenderableGeom.h"

#include <filament/Engine.h>
#include <filament/Texture.h>
#include <backend/PixelBufferDescriptor.h>

#include <Quesa/CQ3ObjectRef_Gets.h>
#include <Quesa/CQ3WeakObjectRef.h>
#include <Quesa/QuesaCustomElements.h>
#include <Quesa/QuesaShader.h>
#include <Quesa/QuesaStorage.h>

#include <map>
#include <vector>
#include <stdint.h>

using namespace filament;

namespace
{
	struct CachedTex
	{
		CQ3WeakObjectRef	weakTexture;
		sharedTexture		filamentTexture;
		TQ3Uns32			editIndexTexture;
		TQ3Uns32			editIndexStorage;
		bool				hasAlpha;
	};
	
	typedef std::shared_ptr< CachedTex >	CachedTexPtr;

	typedef std::map< TQ3TextureObject, CachedTexPtr >	CachedTextureMap;
	
	typedef std::shared_ptr< std::vector<uint8_t> >	BufPtr;

	typedef	void (*PixelConverter)( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel );
}

class TextureManagerImp
{
public:
						TextureManagerImp( filament::Engine& inEngine );
						~TextureManagerImp();

	/*!
		@function		GetTexture
		
		@abstract		Create, or retrieve from a cache, a Filament texture equivalent to a Quesa
						texture.
		@param			inTexture		A pixmap or mipmap texture object.
		@result			A texture, or nullptr if the conversion failed.
	*/
	Texture*			GetTexture( TQ3TextureObject inTexture,
									bool& outHasAlpha );
	
	/*!
		@function		FlushDeadTextures
		@abstract		Flush any cached textures for which the weak Quesa reference to the
						original texture object has become zero.
	*/
	void				FlushDeadTextures( void );

private:
	Engine&				_engine;
	CachedTextureMap	_textures;
};

///MARK:-

/*
	Each of the following functions converts a pixel in a particular Quesa
	format to a Filament pixel in
	PixelDataFormat::RGBA_INTEGER/PixelDataType::UBYTE or
	PixelDataFormat::RGB_INTEGER/PixelDataType::UBYTE format.
*/

static void ConvertPixel_ARGB32_Big( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = inSrcPixel[1];	// R
	ioDstPixel[1] = inSrcPixel[2];	// G
	ioDstPixel[2] = inSrcPixel[3];	// B
	ioDstPixel[3] = inSrcPixel[0];	// A
}

static void ConvertPixel_ARGB32_Little( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = inSrcPixel[2];	// R
	ioDstPixel[1] = inSrcPixel[1];	// G
	ioDstPixel[2] = inSrcPixel[0];	// B
	ioDstPixel[3] = inSrcPixel[3];	// A
}

static void ConvertPixel_xRGB32_Big( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = inSrcPixel[1];	// R
	ioDstPixel[1] = inSrcPixel[2];	// G
	ioDstPixel[2] = inSrcPixel[3];	// B
	ioDstPixel[3] = 0xFF;			// A
}

static void ConvertPixel_xRGB32_Little( const TQ3Uns8* inSrcPixel,
										TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = inSrcPixel[2];	// R
	ioDstPixel[1] = inSrcPixel[1];	// G
	ioDstPixel[2] = inSrcPixel[0];	// B
	ioDstPixel[3] = 0xFF;			// A
}

static void ConvertPixel_RGB24_Big( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = inSrcPixel[0];	// R
	ioDstPixel[1] = inSrcPixel[1];	// G
	ioDstPixel[2] = inSrcPixel[2];	// B
	ioDstPixel[3] = 0xFF;			// A
}

static void ConvertPixel_RGB24_Little( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = inSrcPixel[2];	// R
	ioDstPixel[1] = inSrcPixel[1];	// G
	ioDstPixel[2] = inSrcPixel[0];	// B
	ioDstPixel[3] = 0xFF;			// A
}

static void ConvertPixel_RGB16_Big( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	TQ3Uns16	pixelValue = (((TQ3Uns16)inSrcPixel[0]) << 8) | inSrcPixel[1];
	TQ3Uns8		redBits = (pixelValue >> 10) & 0x1F;
	TQ3Uns8		greenBits = (pixelValue >> 5) & 0x1F;
	TQ3Uns8		blueBits = (pixelValue >> 0) & 0x1F;
	ioDstPixel[0] = redBits << 3;	// R
	ioDstPixel[1] = greenBits << 3;	// G
	ioDstPixel[2] = blueBits << 3;	// B
	ioDstPixel[3] = 0xFF;			// A
}

static void ConvertPixel_RGB16_Little( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	TQ3Uns16	pixelValue = (((TQ3Uns16)inSrcPixel[1]) << 8) | inSrcPixel[0];
	TQ3Uns8		redBits = (pixelValue >> 10) & 0x1F;
	TQ3Uns8		greenBits = (pixelValue >> 5) & 0x1F;
	TQ3Uns8		blueBits = (pixelValue >> 0) & 0x1F;
	ioDstPixel[0] = redBits << 3;	// R
	ioDstPixel[1] = greenBits << 3;	// G
	ioDstPixel[2] = blueBits << 3;	// B
	ioDstPixel[3] = 0xFF;			// A
}

static void ConvertPixel_RGB16_565_Big( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	TQ3Uns16	pixelValue = (((TQ3Uns16)inSrcPixel[0]) << 8) | inSrcPixel[1];
	TQ3Uns8		redBits = (pixelValue >> 11) & 0x1F;
	TQ3Uns8		greenBits = (pixelValue >> 5) & 0x3F;
	TQ3Uns8		blueBits = (pixelValue >> 0) & 0x1F;
	ioDstPixel[0] = redBits << 3;	// R
	ioDstPixel[1] = greenBits << 2;	// G
	ioDstPixel[2] = blueBits << 3;	// B
	ioDstPixel[3] = 0xFF;			// A
}

static void ConvertPixel_RGB16_565_Little( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	TQ3Uns16	pixelValue = (((TQ3Uns16)inSrcPixel[1]) << 8) | inSrcPixel[0];
	TQ3Uns8		redBits = (pixelValue >> 11) & 0x1F;
	TQ3Uns8		greenBits = (pixelValue >> 5) & 0x3F;
	TQ3Uns8		blueBits = (pixelValue >> 0) & 0x1F;
	ioDstPixel[0] = redBits << 3;	// R
	ioDstPixel[1] = greenBits << 2;	// G
	ioDstPixel[2] = blueBits << 3;	// B
	ioDstPixel[3] = 0xFF;			// A
}

static void ConvertPixel_ARGB16_Big( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	TQ3Uns16	pixelValue = (((TQ3Uns16)inSrcPixel[0]) << 8) | inSrcPixel[1];
	TQ3Uns8		alphaBits = pixelValue >> 15;
	TQ3Uns8		redBits = (pixelValue >> 10) & 0x1F;
	TQ3Uns8		greenBits = (pixelValue >> 5) & 0x1F;
	TQ3Uns8		blueBits = (pixelValue >> 0) & 0x1F;
	ioDstPixel[0] = redBits << 3;	// R
	ioDstPixel[1] = greenBits << 3;	// G
	ioDstPixel[2] = blueBits << 3;	// B
	ioDstPixel[3] = alphaBits ? 0xFF : 0;	// A
}

static void ConvertPixel_ARGB16_Little( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	TQ3Uns16	pixelValue = (((TQ3Uns16)inSrcPixel[1]) << 8) | inSrcPixel[0];
	TQ3Uns8		alphaBits = pixelValue >> 15;
	TQ3Uns8		redBits = (pixelValue >> 10) & 0x1F;
	TQ3Uns8		greenBits = (pixelValue >> 5) & 0x1F;
	TQ3Uns8		blueBits = (pixelValue >> 0) & 0x1F;
	ioDstPixel[0] = redBits << 3;	// R
	ioDstPixel[1] = greenBits << 3;	// G
	ioDstPixel[2] = blueBits << 3;	// B
	ioDstPixel[3] = alphaBits ? 0xFF : 0;	// A
}


///MARK:-

static PixelConverter ChoosePixelConverter(
								TQ3PixelType inSrcPixelType,
								TQ3Endian inSrcByteOrder )
{
	PixelConverter	theConverter = nullptr;
	
	if (inSrcByteOrder == kQ3EndianBig)
	{
		switch (inSrcPixelType)
		{
			default:
			case kQ3PixelTypeRGB32:
				theConverter = ConvertPixel_xRGB32_Big;
				break;
			
			case kQ3PixelTypeARGB32:
				theConverter = ConvertPixel_ARGB32_Big;
				break;
			
			case kQ3PixelTypeRGB16:
				theConverter = ConvertPixel_RGB16_Big;
				break;
			
			case kQ3PixelTypeARGB16:
				theConverter = ConvertPixel_ARGB16_Big;
				break;
			
			case kQ3PixelTypeRGB16_565:
				theConverter = ConvertPixel_RGB16_565_Big;
				break;
			
			case kQ3PixelTypeRGB24:
				theConverter = ConvertPixel_RGB24_Big;
				break;
		}
	}
	else	// little-endian
	{
		switch (inSrcPixelType)
		{
			default:
			case kQ3PixelTypeRGB32:
				theConverter = ConvertPixel_xRGB32_Little;
				break;
			
			case kQ3PixelTypeARGB32:
				theConverter = ConvertPixel_ARGB32_Little;
				break;
			
			case kQ3PixelTypeRGB16:
				theConverter = ConvertPixel_RGB16_Little;
				break;
			
			case kQ3PixelTypeARGB16:
				theConverter = ConvertPixel_ARGB16_Little;
				break;
			
			case kQ3PixelTypeRGB16_565:
				theConverter = ConvertPixel_RGB16_565_Little;
				break;
			
			case kQ3PixelTypeRGB24:
				theConverter = ConvertPixel_RGB24_Little;
				break;
		}
	}
	
	return theConverter;
}


static CQ3ObjectRef	GetPixmap( TQ3TextureObject inTexture,
								TQ3StoragePixmap& outInfo )
{
	CQ3ObjectRef result;
	if (kQ3Success == Q3PixmapTexture_GetPixmap( inTexture, &outInfo ))
	{
		result = CQ3ObjectRef( outInfo.image );
	}
	return result;
}

static CQ3ObjectRef	GetMipmap( TQ3TextureObject inTexture,
								TQ3Mipmap& outInfo )
{
	CQ3ObjectRef result;
	if (kQ3Success == Q3MipmapTexture_GetMipmap( inTexture, &outInfo ))
	{
		result = CQ3ObjectRef( outInfo.image );
	}
	
	return result;
}

static BufPtr GetDataOfStorage( TQ3StorageObject inStorage )
{
	BufPtr result;
	if (kQ3Success == Q3Storage_Open( inStorage, kQ3False ))
	{
		TQ3Uns32 dataSize = 0;
		Q3Storage_GetSize( inStorage, &dataSize );
		if (dataSize > 0)
		{
			result = BufPtr( new std::vector<uint8_t>( dataSize ) );
			TQ3Uns32 sizeRead;
			Q3Storage_GetData( inStorage, 0, dataSize, result->data(), &sizeRead );
		}
		
		Q3Storage_Close( inStorage );
	}
	
	return result;
}

static void ReleaseBufferCallback( void* buffer, size_t size, void* user )
{
	std::vector<uint8_t>* bufVec = reinterpret_cast<std::vector<uint8_t>*>( user );
	LogToConsole("Released texture buffer of size %zu", size );
	delete bufVec;
}

static TQ3Uns32 CalcMipmapLevels( TQ3Uns32 inWidth, TQ3Uns32 inHeight )
{
	TQ3Uns32 numLevels = 1;
	TQ3Uns32 n = std::max( inWidth, inHeight );
	while (n > 1)
	{
		n /= 2;
		numLevels += 1;
	}
	
	return numLevels;
}

static bool GetMipmapPixels( const TQ3Mipmap& inMipRec,
							TQ3Boolean inFlipRows,
							TQ3Uns32 inLevel,
							backend::PixelBufferDescriptor& outBufDesc,
							bool& outHasAlpha )
{
	bool didGet = false;
	outHasAlpha = false;
	TQ3Uns32 srcBytesPerPixel = 0;
	const TQ3Uns32 dstBytesPerPixel = 4;
	const backend::PixelDataFormat pixFmt = backend::PixelDataFormat::RGBA;
	const backend::PixelDataType pixType = backend::PixelDataType::UBYTE;
	switch (inMipRec.pixelType)
	{
		default:
		case kQ3PixelTypeRGB32:
			srcBytesPerPixel = 4;
			break;

		case kQ3PixelTypeRGB24:
			srcBytesPerPixel = 3;
			break;
			
		case kQ3PixelTypeRGB16:
		case kQ3PixelTypeRGB16_565:
			srcBytesPerPixel = 2;
			break;
			
		case kQ3PixelTypeARGB32:
			srcBytesPerPixel = 4;
			outHasAlpha = true;
			break;
		
		case kQ3PixelTypeARGB16:
			srcBytesPerPixel = 2;
			outHasAlpha = true;
			break;
	}
	PixelConverter converter = ChoosePixelConverter( inMipRec.pixelType, inMipRec.byteOrder );
	if (converter != nullptr)
	{
		BufPtr rawSourceData( GetDataOfStorage( inMipRec.image ) );
		if (rawSourceData.get() != nullptr)
		{
			const TQ3MipmapImage& theLevel( inMipRec.mipmaps[ inLevel ] );
			TQ3Uns32 dstRowBytes = theLevel.width * dstBytesPerPixel;
			TQ3Uns32 dstDataSize = dstRowBytes * theLevel.height;
			std::unique_ptr< std::vector<uint8_t> > dstData(
				new std::vector<uint8_t>( dstDataSize ) );
			uint8_t* srcRow = rawSourceData->data() + theLevel.offset;
			uint8_t* srcPixel;
			uint8_t* dstPixel = dstData->data();
			TQ3Uns32 rowNum, colNum;
			if (inFlipRows)
			{
				srcRow = rawSourceData->data() + theLevel.rowBytes * theLevel.height;
				
				for (rowNum = 0; rowNum < theLevel.height; ++rowNum)
				{
					srcRow -= theLevel.rowBytes;
					srcPixel = srcRow;
					
					for (colNum = 0; colNum < theLevel.width; ++colNum)
					{
						converter( srcPixel, dstPixel );
						srcPixel += srcBytesPerPixel;
						dstPixel += dstBytesPerPixel;
					}
				}
			}
			else
			{
				for (rowNum = 0; rowNum < theLevel.height; ++rowNum)
				{
					srcPixel = srcRow;
					
					for (colNum = 0; colNum < theLevel.width; ++colNum)
					{
						converter( srcPixel, dstPixel );
						srcPixel += srcBytesPerPixel;
						dstPixel += dstBytesPerPixel;
					}
					
					srcRow += theLevel.rowBytes;
				}
			}
			backend::PixelBufferDescriptor desc( dstData->data(), dstData->size(),
				pixFmt, pixType, ReleaseBufferCallback, dstData.release() );
			
			outBufDesc = std::move( desc );
			
			didGet = true;
		}
	}
	
	return didGet;
}

static bool GetPixMapPixels( const TQ3StoragePixmap& inPixRec,
							TQ3Boolean inFlipRows,
							backend::PixelBufferDescriptor& outBufDesc,
							bool& outHasAlpha )
{
	bool didGet = false;
	outHasAlpha = false;
	TQ3Uns32 srcBytesPerPixel = inPixRec.pixelSize / 8;
	const TQ3Uns32 dstBytesPerPixel = 4;
	const backend::PixelDataFormat pixFmt = backend::PixelDataFormat::RGBA;
	const backend::PixelDataType pixType = backend::PixelDataType::UBYTE;
	switch (inPixRec.pixelType)
	{
		case kQ3PixelTypeRGB32:
		case kQ3PixelTypeRGB16:
		case kQ3PixelTypeRGB24:
		case kQ3PixelTypeRGB16_565:
		default:
			break;
			
		case kQ3PixelTypeARGB32:
		case kQ3PixelTypeARGB16:
			outHasAlpha = true;
			break;
	}
	PixelConverter converter = ChoosePixelConverter( inPixRec.pixelType, inPixRec.byteOrder );
	
	if (converter != nullptr)
	{
		BufPtr rawSourceData( GetDataOfStorage( inPixRec.image ) );
		if (rawSourceData.get() != nullptr)
		{
			TQ3Uns32 dstRowBytes = inPixRec.width * dstBytesPerPixel;
			TQ3Uns32 dstDataSize = dstRowBytes * inPixRec.height;
			std::unique_ptr< std::vector<uint8_t> > dstData(
				new std::vector<uint8_t>( dstDataSize ) );
			uint8_t* srcRow;
			uint8_t* srcPixel;
			uint8_t* dstPixel = dstData->data();
			TQ3Uns32 rowNum, colNum;
			if (inFlipRows)
			{
				srcRow = rawSourceData->data() + inPixRec.rowBytes * inPixRec.height;
				
				for (rowNum = 0; rowNum < inPixRec.height; ++rowNum)
				{
					srcRow -= inPixRec.rowBytes;
					srcPixel = srcRow;
					
					for (colNum = 0; colNum < inPixRec.width; ++colNum)
					{
						converter( srcPixel, dstPixel );
						srcPixel += srcBytesPerPixel;
						dstPixel += dstBytesPerPixel;
					}
				}
			}
			else
			{
				srcRow = rawSourceData->data();
				
				for (rowNum = 0; rowNum < inPixRec.height; ++rowNum)
				{
					srcPixel = srcRow;
					
					for (colNum = 0; colNum < inPixRec.width; ++colNum)
					{
						converter( srcPixel, dstPixel );
						srcPixel += srcBytesPerPixel;
						dstPixel += dstBytesPerPixel;
					}
					
					srcRow += inPixRec.rowBytes;
				}
			}
			
			backend::PixelBufferDescriptor desc( dstData->data(), dstData->size(),
				pixFmt, pixType, ReleaseBufferCallback, dstData.release() );
			
			outBufDesc = std::move( desc );
			
			didGet = true;
		}
	}
	
	return didGet;
}

///MARK:-


TextureManagerImp::TextureManagerImp( filament::Engine& inEngine )
	: _engine( inEngine )
{
}

TextureManagerImp::~TextureManagerImp()
{
	// Note:  Since the cached Textures are stored using sharedTexture,
	// they should automatically get destroyed properly. 
}

/*!
	@function		GetTexture
	
	@abstract		Create, or retrieve from a cache, a Filament texture equivalent to a Quesa
					texture.
	@param			inTexture		A pixmap or mipmap texture object.
	@param			outHasAlpha		Whether the texture should be considered translucent.
	@result			A texture, or nullptr if the conversion failed.
*/
Texture*	TextureManagerImp::GetTexture( TQ3TextureObject inTexture,
											bool& outHasAlpha )
{
	Texture* result = nullptr;
	
	// Get the edit indexes
	TQ3Uns32 textureEditIndex = Q3Shared_GetEditIndex( inTexture );
	TQ3StoragePixmap pixInfo;
	TQ3Mipmap mipInfo;
	bool isPixmap = Q3Object_IsType( inTexture, kQ3TextureTypePixmap );
	bool isMipmap = Q3Object_IsType( inTexture, kQ3TextureTypeMipmap );
	
	CQ3ObjectRef storage;
	if (isPixmap)
	{
		storage = GetPixmap( inTexture, pixInfo );
	}
	else if (isMipmap)
	{
		storage = GetMipmap( inTexture, mipInfo );
	}
	
	TQ3Uns32 storageEditIndex = storage.isvalid()?
		Q3Shared_GetEditIndex( storage.get() ) : 0;
	
	// See if we find that texture in the cache
	auto foundIt = _textures.find( inTexture );
	if (foundIt != _textures.end())
	{
		// Check whether the cached version is stale.
		bool isStale = (textureEditIndex !=
				foundIt->second->editIndexTexture) or
			(storageEditIndex !=
					foundIt->second->editIndexStorage);
		
		// If this record is not stale, we have a result; if it is, flush it.
		if (isStale)
		{
			_textures.erase( foundIt );
		}
		else
		{
			result = foundIt->second->filamentTexture.get();
			outHasAlpha = foundIt->second->hasAlpha;
		}
	}
	
	// If we did not get a good result from the cache, we must do real work.
	if ( (result == nullptr) and storage.isvalid() )
	{
		TQ3Boolean flipRows = CETextureFlippedRowsElement_IsPresent( inTexture );
		LogToConsole("Texture is%s flipped", flipRows? "" : " not");
	
		backend::PixelBufferDescriptor bufDesc;
		outHasAlpha = false;
		Texture* aTexture = nullptr;

		if (isMipmap)
		{
			if (mipInfo.useMipmapping)
			{
				TQ3Uns32 levels = CalcMipmapLevels( mipInfo.mipmaps[0].width,
					mipInfo.mipmaps[0].height );
				aTexture = Texture::Builder()
					.width( mipInfo.mipmaps[0].width )
					.height( mipInfo.mipmaps[0].height )
					.levels( levels )
					.sampler( backend::SamplerType::SAMPLER_2D )
					.format( filament::backend::TextureFormat::RGBA8 )
					.build( _engine );
				
				for (TQ3Uns32 level = 0; level < levels; ++level)
				{
					if (GetMipmapPixels( mipInfo, flipRows, level, bufDesc, outHasAlpha ))
					{
						aTexture->setImage( _engine, level, std::move(bufDesc) );
					}
				}
			}
			else // only one level
			{
				if (GetMipmapPixels( mipInfo, flipRows, 0, bufDesc, outHasAlpha ))
				{
					aTexture = Texture::Builder()
						.width( mipInfo.mipmaps[0].width )
						.height( mipInfo.mipmaps[0].height )
						.levels( 1 )
						.sampler( backend::SamplerType::SAMPLER_2D )
						.format( filament::backend::TextureFormat::RGBA8 )
						.build( _engine );
					aTexture->setImage( _engine, 0, std::move(bufDesc) );
				}
			}
		}
		else if (isPixmap)
		{
			if ( GetPixMapPixels( pixInfo, flipRows, bufDesc, outHasAlpha ) )
			{
				TQ3Uns32 levels = CalcMipmapLevels( pixInfo.width, pixInfo.height );
				
				aTexture = Texture::Builder()
					.width( pixInfo.width )
					.height( pixInfo.height )
					.levels( levels )
					.sampler( backend::SamplerType::SAMPLER_2D )
					.format( filament::backend::TextureFormat::RGBA8 )
					.build( _engine );
				aTexture->setImage( _engine, 0, std::move(bufDesc) );
				aTexture->generateMipmaps( _engine );
			}
		}
		
		if (aTexture != nullptr)
		{
			CachedTexPtr cachedTex( new CachedTex );
			cachedTex->filamentTexture.reset( aTexture, &_engine );
			cachedTex->weakTexture = CQ3WeakObjectRef( inTexture );
			cachedTex->editIndexTexture = textureEditIndex;
			cachedTex->editIndexStorage = storageEditIndex;
			cachedTex->hasAlpha = outHasAlpha;
			
			// Add to cache
			_textures[ inTexture ] = cachedTex;
			
			result = aTexture;
		}
	}
	
	return result;
}

/*!
	@function		FlushDeadTextures
	@abstract		Flush any cached textures for which the weak Quesa reference to the
					original texture object has become zero.
*/
void	TextureManagerImp::FlushDeadTextures( void )
{
	auto endIt = _textures.end();
	auto it = _textures.begin();
	while (it != endIt)
	{
		auto nextIt = it;
		++nextIt;
		if (not it->second->weakTexture.isvalid())
		{
			_textures.erase( it );
			LogToConsole("Flushed a dead texture");
		}
		it = nextIt;
	}
}


///MARK:-

TextureManager::TextureManager( filament::Engine& inEngine )
	: _imp( new TextureManagerImp( inEngine ) )
{
	
}

TextureManager::~TextureManager()
{
	
}

/*!
	@function		GetTexture
	
	@abstract		Create, or retrieve from a cache, a Filament texture equivalent to a Quesa
					texture.
	@param			inTexture		A pixmap or mipmap texture object.
	@param			outHasAlpha		Whether the texture should be considered translucent.
	@result			A texture, or nullptr if the conversion failed.
*/
Texture*	TextureManager::GetTexture( TQ3TextureObject inTexture,
										bool& outHasAlpha )
{
	return _imp->GetTexture( inTexture, outHasAlpha );
}

/*!
	@function		FlushDeadTextures
	@abstract		Flush any cached textures for which the original Quesa texture object
					no longer exists.
*/
void	TextureManager::FlushDeadTextures( void )
{
	_imp->FlushDeadTextures();
}

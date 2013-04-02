/*  NAME:
        GLTextureLoader.cpp

    DESCRIPTION:
        Source code to load a texture into OpenGL texture memory.

    COPYRIGHT:
        Copyright (c) 1999-2013, Quesa Developers. All rights reserved.

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

#include "GLTextureLoader.h"
#include "QuesaErrors.h"
#include "QuesaMemory.h"
#include "QuesaStorage.h"
#include "CQ3ObjectRef.h"
#include "GLUtils.h"
#include "E3Debug.h"
#include "E3ErrorManager.h"
#include "E3Utils.h"

#include <algorithm>
#include <new>



//=============================================================================
//      Local types
//-----------------------------------------------------------------------------

namespace
{
	typedef	void (*PixelConverter)( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel );


	/*!
		@class		ByteBuffer
		
		@abstract	Simple growable array of bytes.
		
		@discussion	Previously we were using std::vector<TQ3Uns8> and
					std::vector<GLubyte>, but resizing a std::vector default-
					constructs new elements, which is unnecessary overhead
					in our case.
	*/
	class ByteBuffer
	{
	public:
						ByteBuffer( unsigned long inInitialSize );
						~ByteBuffer();
	
						/*!
							@function	Address
							@abstract	Get the address of the start of the
										buffer.
							@result		Address of buffer.
						*/
		unsigned char*	Address()
						{
							return mBuffer;
						}
		
						/*!
							@function	Grow
							@abstract	Make the buffer have at least this many
										bytes.
							@discussion	If the new size is greater than the
										current size, the buffer is grown, with
										new elements being uninitialized.  If
										the new size is less or equal to the
										current size, nothing happens.
							@param		inSize		Desired size in bytes.
						*/
		void			Grow( unsigned long inSize );
		
						/*!
							@function	Swap
							@abstract	Exchange storage with another buffer.
							@param		ioOther		Another buffer.
						*/
		void			Swap( ByteBuffer& ioOther );
	
	private:
		unsigned char*	mBuffer;
		unsigned long	mSize;
		
		// Unimplemented
						ByteBuffer( const ByteBuffer& inOther );
		ByteBuffer&		operator=( const ByteBuffer& inOther );
	};
}


//=============================================================================
//      Constants
//-----------------------------------------------------------------------------

const unsigned long		kInitialBufferSize = 65536;



//=============================================================================
//      Local data
//-----------------------------------------------------------------------------

static ByteBuffer	sSrcImageData( kInitialBufferSize );

static ByteBuffer	sGLFormatWork( kInitialBufferSize );

static ByteBuffer	sGLImageData( kInitialBufferSize );




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------

ByteBuffer::ByteBuffer( unsigned long inInitialSize )
	: mBuffer( static_cast<unsigned char*>( Q3Memory_Allocate( static_cast<TQ3Uns32>(inInitialSize) ) ) )
	, mSize( inInitialSize )
{
	if (mBuffer == NULL)
	{
		mSize = 0;
	}
}


ByteBuffer::~ByteBuffer()
{
	if (mBuffer != NULL)
	{
		Q3Memory_Free( &mBuffer );
	}
}

/*!
	@function	Grow
	@abstract	Make the buffer have at least this many
				bytes.
	@discussion	If the new size is greater than the
				current size, the buffer is grown, with
				new elements being uninitialized.  If
				the new size is less or equal to the
				current size, nothing happens.
	@param		inSize		Desired size in bytes.
*/
void	ByteBuffer::Grow( unsigned long inSize )
{
	if (inSize > mSize)
	{
		TQ3Status theStatus = Q3Memory_Reallocate( &mBuffer, static_cast<TQ3Uns32>(inSize) );
		if (theStatus == kQ3Failure)
		{
			E3ErrorManager_PostError( kQ3ErrorOutOfMemory, kQ3False );
			throw std::bad_alloc();
		}
		mSize = inSize;
	}
}

/*!
	@function	Swap
	@abstract	Exchange storage with another buffer.
	@param		ioOther		Another buffer.
*/
void	ByteBuffer::Swap( ByteBuffer& ioOther )
{
	if (&ioOther != this)
	{
		std::swap( mBuffer, ioOther.mBuffer );
		std::swap( mSize, ioOther.mSize );
	}
}


static bool IsPowerOf2( TQ3Uns32 n )
{
	return (n & (n - 1)) == 0;
}

static TQ3Uns32 NextPowerOf2( TQ3Uns32 n )
{
	// smear out the 1 bits...
	n |= (n >> 16);
	n |= (n >> 8);
	n |= (n >> 4);
	n |= (n >> 2);
	n |= (n >> 1);
	
	n += 1;
	
	return n;
}

static void ConstrainTextureSize(
									TQ3Uns32 inSrcWidth,
									TQ3Uns32 inSrcHeight,
									TQ3Uns32& outDstWidth,
									TQ3Uns32& outDstHeight )
{
	outDstWidth = inSrcWidth;
	outDstHeight = inSrcHeight;
	
	GLint	maxGLSize;
	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxGLSize );
	
	if (!IsPowerOf2( outDstWidth ))
	{
		outDstWidth = NextPowerOf2( outDstWidth );
	}
	
	if (!IsPowerOf2( outDstHeight ))
	{
		outDstHeight = NextPowerOf2( outDstHeight );
	}
	
	while (outDstWidth > (TQ3Uns32)maxGLSize)
	{
		outDstWidth /= 2;
	}
	
	while (outDstHeight > (TQ3Uns32)maxGLSize)
	{
		outDstHeight /= 2;
	}
}

static bool GetPixmapTextureData( TQ3TextureObject inTexture,
								TQ3StoragePixmap& outData,
								CQ3ObjectRef& outStorageHolder )
{
	TQ3Status	theStatus = Q3PixmapTexture_GetPixmap( inTexture, &outData );
	if (theStatus == kQ3Success)
	{
		outStorageHolder = CQ3ObjectRef( outData.image );
	}
	return (theStatus == kQ3Success);
}

static bool GetMipmapTextureData( TQ3TextureObject inTexture,
								TQ3Mipmap& outData,
								CQ3ObjectRef& outStorageHolder )
{
	TQ3Status	theStatus = Q3MipmapTexture_GetMipmap( inTexture, &outData );
	if (theStatus == kQ3Success)
	{
		outStorageHolder = CQ3ObjectRef( outData.image );
	}
	return (theStatus == kQ3Success);
}


static TQ3PixelType	GetTexturePixelType( TQ3TextureObject inTexture )
{
	TQ3PixelType	thePixelType = kQ3PixelTypeUnknown;
	CQ3ObjectRef	storageHolder;
	
	switch (Q3Texture_GetType( inTexture ))
	{
		case kQ3TextureTypePixmap:
			{
				TQ3StoragePixmap	thePixmap;
				if (GetPixmapTextureData( inTexture, thePixmap,
					storageHolder ))
				{
					thePixelType = thePixmap.pixelType;
				}
			}
			break;
		
		case kQ3TextureTypeMipmap:
			{
				TQ3Mipmap			theMipmap;
				if (GetMipmapTextureData( inTexture, theMipmap,
					storageHolder ))
				{
					thePixelType = theMipmap.pixelType;
				}
			}
			break;
		
		default:
			thePixelType = kQ3PixelTypeUnknown;
			break;
	}
	
	return thePixelType;
}

/*!
	@function		IsTextureMipmapped
	
	@abstract		Test whether a texture will use mipmapping.
	
	@discussion		In Quesa, pixmap textures automatically use mipmapping.
					However, a mipmap texture can either provide a full set
					of mipmap textures, or can provide one and say that
					mipmapping is not to be used.
*/
static bool IsTextureMipmapped( TQ3TextureObject inTexture )
{
	bool	isMipmapped = false;
	
	switch (Q3Texture_GetType( inTexture ))
	{
		case kQ3TextureTypePixmap:
			isMipmapped = true;	// automatic
			break;
			
		case kQ3TextureTypeMipmap:
			{
				TQ3Mipmap			theMipmap;
				CQ3ObjectRef	storageHolder;
				if (GetMipmapTextureData( inTexture, theMipmap,
					storageHolder ))
				{
					isMipmapped = (theMipmap.useMipmapping == kQ3True);
				}
			}
			break;
	}
	
	return isMipmapped;
}

static int	CountImagesInMipmap( const TQ3Mipmap& inMipmapData )
{
	int	numImages = 1;
	
	if (inMipmapData.useMipmapping)
	{
		// The mipmap images have their dimensions repeatedly halved, until
		// it is down to size (1, 1).
		int n = E3Num_Max( inMipmapData.mipmaps[0].width,
			inMipmapData.mipmaps[0].height );
		
		while (n > 1)
		{
			n /= 2;
			numImages += 1;
		}
	}
	
	return numImages;
}

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

static void ConvertPixel_ARGB32_Big_Premultiply( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = (inSrcPixel[1] * inSrcPixel[0]) / 255;	// R
	ioDstPixel[1] = (inSrcPixel[2] * inSrcPixel[0]) / 255;	// G
	ioDstPixel[2] = (inSrcPixel[3] * inSrcPixel[0]) / 255;	// B
	ioDstPixel[3] = inSrcPixel[0];	// A
}

static void ConvertPixel_ARGB32_Little_Premultiply( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = (inSrcPixel[2] * inSrcPixel[3]) / 255;	// R
	ioDstPixel[1] = (inSrcPixel[1] * inSrcPixel[3]) / 255;	// G
	ioDstPixel[2] = (inSrcPixel[0] * inSrcPixel[3]) / 255;	// B
	ioDstPixel[3] = inSrcPixel[3];	// A
}

static void ConvertPixel_xRGB32_Big( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = inSrcPixel[1];	// R
	ioDstPixel[1] = inSrcPixel[2];	// G
	ioDstPixel[2] = inSrcPixel[3];	// B
}

static void ConvertPixel_xRGB32_Little( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = inSrcPixel[2];	// R
	ioDstPixel[1] = inSrcPixel[1];	// G
	ioDstPixel[2] = inSrcPixel[0];	// B
}


static void ConvertPixel_RGB24_Big( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = inSrcPixel[0];	// R
	ioDstPixel[1] = inSrcPixel[1];	// G
	ioDstPixel[2] = inSrcPixel[2];	// B
}

static void ConvertPixel_RGB24_Little( const TQ3Uns8* inSrcPixel,
									TQ3Uns8* ioDstPixel )
{
	ioDstPixel[0] = inSrcPixel[2];	// R
	ioDstPixel[1] = inSrcPixel[1];	// G
	ioDstPixel[2] = inSrcPixel[0];	// B
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

static void ConvertPixel_ARGB16_Big_Premultiply( const TQ3Uns8* inSrcPixel,
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
	if (alphaBits == 0)
	{
		ioDstPixel[0] = ioDstPixel[1] = ioDstPixel[2] = 0;
	}
}

static void ConvertPixel_ARGB16_Little_Premultiply( const TQ3Uns8* inSrcPixel,
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
	if (alphaBits == 0)
	{
		ioDstPixel[0] = ioDstPixel[1] = ioDstPixel[2] = 0;
	}
}

static PixelConverter ChoosePixelConverter(
								TQ3PixelType inSrcPixelType,
								TQ3Endian inSrcByteOrder,
								bool inPremultiplyAlpha )
{
	PixelConverter	theConverter = NULL;
	
	if (inSrcByteOrder == kQ3EndianBig)
	{
		switch (inSrcPixelType)
		{
			default:
			case kQ3PixelTypeRGB32:
				theConverter = ConvertPixel_xRGB32_Big;
				break;
			
			case kQ3PixelTypeARGB32:
				theConverter = inPremultiplyAlpha?
					ConvertPixel_ARGB32_Big_Premultiply :
					ConvertPixel_ARGB32_Big;
				break;
			
			case kQ3PixelTypeRGB16:
				theConverter = ConvertPixel_RGB16_Big;
				break;
			
			case kQ3PixelTypeARGB16:
				theConverter = inPremultiplyAlpha?
					ConvertPixel_ARGB16_Big_Premultiply :
					ConvertPixel_ARGB16_Big;
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
				theConverter = inPremultiplyAlpha?
					ConvertPixel_ARGB32_Little_Premultiply :
					ConvertPixel_ARGB32_Little;
				break;
			
			case kQ3PixelTypeRGB16:
				theConverter = ConvertPixel_RGB16_Little;
				break;
			
			case kQ3PixelTypeARGB16:
				theConverter = inPremultiplyAlpha?
					ConvertPixel_ARGB16_Little_Premultiply :
					ConvertPixel_ARGB16_Little;
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

/*!
	@function	GetImageData
	@abstract	Get a pointer to the original image data from the storage
				object, if possible without copying.
*/
static const TQ3Uns8*	GetImageData(
									TQ3StorageObject inStorage,
									TQ3Uns32 inStorageOffset,
									TQ3Uns32 inDataSize )
{
	const TQ3Uns8*	theData = NULL;
	TQ3Uns32			sizeRead, bufferSize;
	TQ3Uns8*	dataAddr = NULL;
	
	TQ3ObjectType	theType = Q3Object_GetLeafType( inStorage );
	
	switch (theType)
	{
		case kQ3StorageTypeMemory:
			Q3MemoryStorage_GetBuffer( inStorage, &dataAddr, NULL, NULL );
			if (dataAddr != NULL)
			{
				theData = dataAddr + inStorageOffset;
			}
			break;
		
		default:	// any type of storage
			if ( (kQ3Success == Q3Storage_GetSize( inStorage, &bufferSize )) &&
				(bufferSize > inStorageOffset + inDataSize) )
			{
				sSrcImageData.Grow( inDataSize );
				
				if (kQ3Success == Q3Storage_GetData( inStorage,
					inStorageOffset, inDataSize, sSrcImageData.Address(),
					&sizeRead ))
				{
					theData = sSrcImageData.Address();
				}
			}
			break;
	}
	
	return theData;
}


/*!
	@function	ConvertImageFormat
	@abstract	Convert the Quesa texture image data to a format OpenGL likes,
				including making the image rows go bottom to top instead of top
				to bottom.
*/
static bool	ConvertImageFormat(
								const TQ3Uns8* inSrcImageData,
								TQ3PixelType inSrcPixelType,
								TQ3Uns32 inSrcWidth,
								TQ3Uns32 inSrcHeight,
								TQ3Uns32 inSrcRowBytes,
								TQ3Endian inSrcByteOrder,
								bool inPremultiplyAlpha,
								ByteBuffer& outImage,
								GLint& outGLInternalFormat,
								GLenum& outGLFormat )
{
	bool	didConvert = false;
	
	bool	hasAlpha = (inSrcPixelType == kQ3PixelTypeARGB32) ||
		(inSrcPixelType == kQ3PixelTypeARGB16);
	
	outGLFormat = hasAlpha? GL_RGBA : GL_RGB;
	outGLInternalFormat = GLUtils_ConvertPixelType( inSrcPixelType );
	TQ3Uns32 srcBytesPerPixel = GLUtils_SizeOfPixelType( inSrcPixelType ) / 8;
	TQ3Uns32 dstBytesPerPixel = hasAlpha? 4 : 3;
	// Assume 4-byte alignment, so dstRowBytes must be rounded up to next
	// multiple of 4.
	TQ3Uns32 dstRowBytes = 4 * ((dstBytesPerPixel * inSrcWidth + 3) / 4);
	
	PixelConverter	theConverter = ChoosePixelConverter( inSrcPixelType,
		inSrcByteOrder, inPremultiplyAlpha );
	
	if (theConverter != NULL)
	{
		// Allocate memory
		outImage.Grow( dstRowBytes * inSrcHeight );
		TQ3Uns8*	dstData = outImage.Address();
		
		
		for (TQ3Uns32 rowNum = 0; rowNum < inSrcHeight; ++rowNum)
		{
			const TQ3Uns8* srcRowData = inSrcImageData +
				(inSrcHeight - rowNum - 1) * inSrcRowBytes;
			TQ3Uns8* dstRowData = &dstData[ dstRowBytes * rowNum ];
			
			for (TQ3Uns32 colNum = 0; colNum < inSrcWidth; ++colNum)
			{
				(*theConverter)( srcRowData, dstRowData );
				
				srcRowData += srcBytesPerPixel;
				dstRowData += dstBytesPerPixel;
			}
		}
		
		didConvert = true;
	}
	
	return didConvert;
}

static void	ResizeImage(
								GLenum inGLFormat,
								TQ3Uns32 inSrcWidth,
								TQ3Uns32 inSrcHeight,
								ByteBuffer& inSrcImage,
								TQ3Uns32 inDstWidth,
								TQ3Uns32 inDstHeight,
								ByteBuffer& outDstImage )
{
	TQ3Uns32 dstBytesPerPixel = (inGLFormat == GL_RGBA)? 4 : 3;
	// Assume 4-byte alignment, so dstRowBytes must be rounded up to next
	// multiple of 4.
	TQ3Uns32 dstRowBytes = 4 * ((dstBytesPerPixel * inDstWidth + 3) / 4);
	
	// Allocate memory
	outDstImage.Grow( dstRowBytes * inDstHeight );
	
	
	// Resize the image
	gluScaleImage( inGLFormat,
		inSrcWidth, inSrcHeight, GL_UNSIGNED_BYTE, inSrcImage.Address(),
		inDstWidth, inDstHeight, GL_UNSIGNED_BYTE, outDstImage.Address() );
}

/*!
	@function	ConvertImageForOpenGL
	@abstract	Convert the Quesa texture image data to a format OpenGL likes,
				including obeying the texture size upper bound and power of 2
				requirements.
*/
static bool	ConvertImageForOpenGL(
								TQ3StorageObject inStorage,
								TQ3Uns32 inStorageOffset,
								TQ3PixelType inSrcPixelType,
								TQ3Uns32 inSrcWidth,
								TQ3Uns32 inSrcHeight,
								TQ3Uns32 inSrcRowBytes,
								TQ3Endian inSrcByteOrder,
								bool inPremultiplyAlpha,
								TQ3Uns32& outDstWidth,
								TQ3Uns32& outDstHeight,
								ByteBuffer& outImage,
								GLint& outGLInternalFormat,
								GLenum& outGLFormat )
{
	bool	didConvert = false;
	TQ3Uns32	srcDataSize = inSrcRowBytes * inSrcHeight;
	const TQ3Uns8*	srcData = GetImageData( inStorage, inStorageOffset,
		srcDataSize );
	
	if ( (srcData != NULL) &&
		ConvertImageFormat( srcData, inSrcPixelType, inSrcWidth, inSrcHeight,
			inSrcRowBytes, inSrcByteOrder, inPremultiplyAlpha, sGLFormatWork,
			outGLInternalFormat, outGLFormat ) )
	{
		ConstrainTextureSize( inSrcWidth, inSrcHeight, outDstWidth, outDstHeight );
		
		if ( (outDstWidth == inSrcWidth) && (outDstHeight == inSrcHeight) )
		{
			// Image can be used as is
			outImage.Swap( sGLFormatWork );
		}
		else
		{
			ResizeImage( outGLFormat,
				inSrcWidth, inSrcHeight, sGLFormatWork,
				outDstWidth, outDstHeight, outImage );
		}
		
		didConvert = true;
	}
	
	return didConvert;
}


static bool	LoadOpenGLWithPixmapTexture(
								TQ3TextureObject inTexture,
								bool inPremultiplyAlpha )
{
	bool	didLoad = false;
	TQ3StoragePixmap	thePixmap;
	CQ3ObjectRef		storageHolder;
	
	
	if (GetPixmapTextureData( inTexture, thePixmap, storageHolder ))
	{
		TQ3Uns32	theWidth, theHeight;
		GLint	glInternalFormat;
		GLenum	glFormat;
		
		bool didConvert = ConvertImageForOpenGL( thePixmap.image, 0,
			thePixmap.pixelType, thePixmap.width, thePixmap.height,
			thePixmap.rowBytes, thePixmap.byteOrder,
			inPremultiplyAlpha,
			theWidth, theHeight,
			sGLImageData, glInternalFormat, glFormat );
		
		if (didConvert)
		{
			gluBuild2DMipmaps( GL_TEXTURE_2D, glInternalFormat,
				theWidth, theHeight,
				glFormat, GL_UNSIGNED_BYTE,
				sGLImageData.Address() );

			didLoad = true;
		}
	}
	
	return didLoad;
}

static bool	LoadOpenGLWithMipmapTexture(
								TQ3TextureObject inTexture,
								bool inPremultiplyAlpha )
{
	bool	didLoad = false;
	TQ3Mipmap		theMipmap;
	CQ3ObjectRef	storageHolder;
	
	if (GetMipmapTextureData( inTexture, theMipmap, storageHolder ))
	{
		didLoad = true;
		int	numImages = CountImagesInMipmap( theMipmap );
		
		for (int i = 0; i < numImages; ++i)
		{
			TQ3Uns32	theWidth, theHeight;
			GLint	glInternalFormat;
			GLenum	glFormat;
			
			bool didConvert = ConvertImageForOpenGL( theMipmap.image,
				theMipmap.mipmaps[i].offset, theMipmap.pixelType,
				theMipmap.mipmaps[i].width, theMipmap.mipmaps[i].height,
				theMipmap.mipmaps[i].rowBytes,
				theMipmap.byteOrder, inPremultiplyAlpha,
				theWidth, theHeight,
				sGLImageData, glInternalFormat, glFormat );
			
			if (didConvert)
			{
				glTexImage2D( GL_TEXTURE_2D, i, glInternalFormat,
					theWidth, theHeight, 0, glFormat, GL_UNSIGNED_BYTE,
					sGLImageData.Address() );
			}
			else
			{
				didLoad = false;
			}
		}
	}


	return didLoad;
}

//=============================================================================
//      External functions
//-----------------------------------------------------------------------------

/*!
	@function	GLTextureLoader
	
	@abstract	Load a Quesa texture object as an OpenGL texture object.
	@param		inTexture		A texture object.
	@param		inPremultiplyAlpha	If true, the loader will multiply each color
									value by its alpha value.  Use this if your
									texture data has an alpha channel and is NOT
									set up with premultiplied alpha.
	@result		An OpenGL texture "name", or 0 on failure.
*/
GLuint	GLTextureLoader( TQ3TextureObject inTexture,
							TQ3Boolean inPremultiplyAlpha )
{
	GLuint	resultTextureName = 0;
	Q3_ASSERT( inTexture != NULL );
	
	try
	{
		glEnable( GL_TEXTURE_2D );
		GLuint	textureName;
		glGenTextures( 1, &textureName );
		glBindTexture( GL_TEXTURE_2D, textureName );
		Q3_ASSERT( glIsTexture( textureName ) );

		glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
		glPixelStorei( GL_PACK_ROW_LENGTH, 0 );
		glPixelStorei( GL_PACK_ALIGNMENT, 4 );

		TQ3ObjectType	theType = Q3Texture_GetType( inTexture );
		bool	didLoad = false;
		
		switch (theType)
		{
			case kQ3TextureTypePixmap:
				didLoad = LoadOpenGLWithPixmapTexture( inTexture,
					inPremultiplyAlpha == kQ3True );
				break;
			
			case kQ3TextureTypeMipmap:
				didLoad = LoadOpenGLWithMipmapTexture( inTexture,
					inPremultiplyAlpha == kQ3True );
				break;
		}
		
		if (didLoad)
		{
			resultTextureName = textureName;
		}
		else
		{
			glDeleteTextures( 1, &textureName );
		}
	}
	catch (...)
	{
	}
	
	return resultTextureName;
}

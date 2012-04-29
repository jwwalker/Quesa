/*
 *  PixmapTexture.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/8/12.
 *  Copyright (c) 2012 James W. Walker.
 *
 *  This software is provided 'as-is', without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from the
 *  use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source distribution.
 *
 */

#include "PixmapTexture.h"

#include <iomanip>

namespace
{
	const char* kEndianName[] =
	{
		"BigEndian",
		"LittleEndian"
	};
	
	const char* kPixelTypeName[] =
	{
		"RGB32",
		"ARGB32",
		"RGB16",
		"ARGB16",
		"RGB16_565",
		"RGB24"
	};
}

PixmapTexture::PixmapTexture()
	: TypeHandler( 'txpm', "PixmapTexture" )
{
}

void	PixmapTexture::Process( uint32_t inStartOffset,
									uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 28)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 28 );
	}
	
	uint32_t width = FetchUInt32( inStartOffset );
	uint32_t height = FetchUInt32( inStartOffset+4 );

	if ( (width == 0) or (height == 0) )
	{
		throw FormatException( Name(), inStartOffset );
	}
	
	uint32_t rowBytes = FetchUInt32( inStartOffset+8 );
	uint32_t pixelSize = FetchUInt32( inStartOffset+12 );
	
	if ( (pixelSize > 32) or ((pixelSize % 8) != 0) or
		(rowBytes < (pixelSize/8) * width) )
	{
		throw FormatException( Name(), inStartOffset+8 );
	}
	
	uint32_t pixelType = FetchUInt32( inStartOffset+16 );
	
	if (pixelType > 5)
	{
		throw FormatException( Name(), inStartOffset+16 );
	}
	
	uint32_t bitOrder = FetchUInt32( inStartOffset+20 );
	uint32_t byteOrder = FetchUInt32( inStartOffset+24 );
	
	if ( (bitOrder > 1) or (byteOrder > 1) )
	{
		throw FormatException( Name(), inStartOffset+20 );
	}
	
	uint32_t imageDataLen = rowBytes * height;
	uint32_t padBytes = 0;
	if ( (imageDataLen % 4) != 0 )
	{
		padBytes = 4 - (imageDataLen % 4);
	}
	uint32_t expectedLen = 28 + imageDataLen + padBytes;
	if (dataLen != expectedLen)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset,
			expectedLen );
	}
	
	Out() << Indent() << Name() << "(\n" <<
		Indent(1) << width << ' ' << height << ' ' <<
		rowBytes << ' ' << pixelSize << ' ' <<
		kPixelTypeName[pixelType] << ' ' <<
		kEndianName[bitOrder] << ' ' << kEndianName[byteOrder];
	
	Boss()->ReadAndWriteHexData( inStartOffset + 28, imageDataLen );
	
	Out() << Indent() << ")\n";
}

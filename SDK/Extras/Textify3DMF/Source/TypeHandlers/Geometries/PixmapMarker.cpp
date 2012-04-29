/*
 *  PixmapMarker.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/15/12.
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

#include "PixmapMarker.h"

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


PixmapMarker::PixmapMarker()
	: TypeHandler( 'mrkp', "PixmapMarker" )
{
}

void	PixmapMarker::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 48)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 48 );
	}
	
	float loc_x = FetchFloat32( inStartOffset );
	float loc_y = FetchFloat32( inStartOffset+4 );
	float loc_z = FetchFloat32( inStartOffset+8 );
	int32_t off_x = (int32_t)FetchUInt32( inStartOffset+12 );
	int32_t off_y = (int32_t)FetchUInt32( inStartOffset+16 );
	uint32_t width = FetchUInt32( inStartOffset+20 );
	uint32_t height = FetchUInt32( inStartOffset+24 );
	uint32_t rowBytes = FetchUInt32( inStartOffset+28 );
	uint32_t pixelSize = FetchUInt32( inStartOffset+32 );
	uint32_t pixelType = FetchUInt32( inStartOffset+36 );
	uint32_t bitOrder = FetchUInt32( inStartOffset+40 );
	uint32_t byteOrder = FetchUInt32( inStartOffset+44 );
	
	if ( (pixelSize != 16) and (pixelSize != 24) and (pixelSize != 32) )
	{
		throw FormatException( Name(), inStartOffset+32 );
	}
	if (pixelType > 5)
	{
		throw FormatException( Name(), inStartOffset+36 );
	}
	if (bitOrder > 1)
	{
		throw FormatException( Name(), inStartOffset+40 );
	}
	if (byteOrder > 1)
	{
		throw FormatException( Name(), inStartOffset+44 );
	}
	
	uint32_t imageDataLen = rowBytes * height;
	uint32_t expectedLen = 48 + imageDataLen;
	if ( (expectedLen % 4) != 0 )
	{
		expectedLen += 4 - (expectedLen % 4);
	}
	if (dataLen != expectedLen)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, expectedLen );
	}
	
	Out() << Indent() << Name() << " (\n" <<
		Indent(1) << loc_x << ' ' << loc_y << ' ' << loc_z << "\t# origin\n" <<
		Indent(1) << off_x << "\t# xOffset\n" <<
		Indent(1) << off_y << "\t# yOffset\n" <<
		Indent(1) << width << "\t# width\n" <<
		Indent(1) << height << "\t# height\n" <<
		Indent(1) << rowBytes << "\t# rowBytes\n" <<
		Indent(1) << pixelSize << "\t# pixelSize\n" <<
		Indent(1) << kPixelTypeName[pixelType] << "\t# pixelType\n" <<
		Indent(1) << kEndianName[bitOrder] << "\t# bitOrder\n" <<
		Indent(1) << kEndianName[byteOrder] << "\t# byteOrder";
	
	Boss()->ReadAndWriteHexData( inStartOffset+48, imageDataLen );
	
	Out() << Indent() << ")\n";
}

/*
 *  GeneralPolygon.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/13/12.
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

#include "GeneralPolygon.h"


GeneralPolygon::GeneralPolygon()
	: TypeHandler( 'gpgn', "GeneralPolygon" )
{
}

void	GeneralPolygon::Process( uint32_t inStartOffset, uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 4)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 4 );
	}
	
	uint32_t numContours = FetchUInt32( inStartOffset );
	
	Out() << Indent() << Name() << " (\n" <<
		Indent(1) << numContours << "\t# numContours\n";
	
	uint32_t offset = inStartOffset + 4;
	
	for (uint32_t i = 0; i < numContours; ++i)
	{
		if (offset + 4 > inEndOffset)
		{
			throw FormatException( Name(), offset );
		}
		uint32_t nVertices = FetchUInt32( offset );
		offset += 4;
		if (offset + 12 * nVertices > inEndOffset)
		{
			throw FormatException( Name(), offset );
		}
		Out() << Indent(1) << nVertices <<
			"\t# nVertices, contour " << i << '\n';
		
		for (uint32_t j = 0; j < nVertices; ++j)
		{
			Out() << Indent(1) <<
				FetchFloat32( offset ) << ' ' <<
				FetchFloat32( offset+4 ) << ' ' <<
				FetchFloat32( offset+8 ) << '\n';
			offset += 12;
		}
	}
	
	if (offset != inEndOffset)
	{
		throw FormatException( Name(), offset );
	}
	
	Out() << Indent() << ")\n";
}

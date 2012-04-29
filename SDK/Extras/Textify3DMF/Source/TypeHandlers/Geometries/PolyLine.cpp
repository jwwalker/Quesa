/*
 *  PolyLine.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/11/12.
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

#include "PolyLine.h"

PolyLine::PolyLine()
	: TypeHandler( 'plyl', "PolyLine" )
{
}

void	PolyLine::Process( uint32_t inStartOffset, uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 4)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 4 );
	}
	
	uint32_t numVerts = FetchUInt32( inStartOffset );
	uint32_t expectedLen = 4 + numVerts * 12;
	
	if (dataLen != expectedLen)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, expectedLen );
	}
	
	Out() << Indent() << Name() << " (\n" <<
		Indent(1) << numVerts << '\n';
	
	for (uint32_t i = 0; i < numVerts; ++i)
	{
		uint32_t vertOff = inStartOffset + 4 + i * 12;
		Out() << Indent(1) <<
			FetchFloat32( vertOff ) << ' ' <<
			FetchFloat32( vertOff+4 ) << ' ' <<
			FetchFloat32( vertOff+8 ) << '\n';
	}
	
	Out() << Indent() << ")\n";
}

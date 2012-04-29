/*
 *  NURBPatch.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/20/12.
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

#include "NURBPatch.h"

NURBPatch::NURBPatch()
	: TypeHandler( 'nrbp', "NURBPatch" )
{
}

void	NURBPatch::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 16)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 16 );
	}
	
	uint32_t uOrder = FetchUInt32( inStartOffset );
	uint32_t vOrder = FetchUInt32( inStartOffset+4 );
	uint32_t numUPoints = FetchUInt32( inStartOffset+8 );
	uint32_t numVPoints = FetchUInt32( inStartOffset+12 );
	
	// Note: the 3DMF specification disagrees with the main QD3D manual about
	// how the knot counts relate to the other numbers, and the one given
	// example does not clarify it.
	uint32_t numUKnots = uOrder + numVPoints;
	uint32_t numVKnots = vOrder + numUPoints;
	
	uint32_t expectedLen = 16 + 16*numUPoints*numVPoints +
		4*(numUKnots + numVKnots);

	if (dataLen != expectedLen)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, expectedLen );
	}
	
	Out() << Indent() << Name() << " (\n" <<
		Indent(1) << uOrder << ' ' << vOrder << "\t# uOrder, vOrder\n" <<
		Indent(1) << numUPoints << ' ' << numVPoints <<
		"\t# numRows, numColumns\n\n" <<
		Indent(1) << "# points\n";
	
	uint32_t i, j;
	uint32_t offset = inStartOffset + 16;
	
	for (i = 0; i < numUPoints; ++i)
	{
		for (j = 0; j < numVPoints; ++j)
		{
			Out() << Indent(1) << FetchFloat32( offset ) << ' ' <<
				FetchFloat32( offset+4 ) << ' ' << 
				FetchFloat32( offset+8 ) << ' ' << 
				FetchFloat32( offset+12 ) << "\t# row " << i << ", col " <<
				j << '\n';
			offset += 16;
		}
	}
	
	Out() << '\n' << Indent(1) << "# uKnots";
	for (i = 0; i < numUKnots; ++i)
	{
		if ( (i % 10) == 0 )
		{
			Out() << '\n' << Indent(1);;
		}
		Out() << FetchFloat32( offset ) << ' ';
		offset += 4;
	}
	
	Out() << "\n\n" << Indent(1) << "# vKnots";
	for (i = 0; i < numVKnots; ++i)
	{
		if ( (i % 10) == 0 )
		{
			Out() << '\n' << Indent(1);;
		}
		Out() << FetchFloat32( offset ) << ' ';
		offset += 4;
	}
		
	Out() << '\n' << Indent() << ")\n";
}

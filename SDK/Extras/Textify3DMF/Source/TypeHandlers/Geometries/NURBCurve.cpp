/*
 *  NURBCurve.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/19/12.
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

#include "NURBCurve.h"

NURBCurve::NURBCurve()
	: TypeHandler( 'nrbc', "NURBCurve" )
{
}

void	NURBCurve::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 8)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 8 );
	}
	
	uint32_t order = FetchUInt32( inStartOffset );
	uint32_t nPoints = FetchUInt32( inStartOffset+4 );
	uint32_t numKnots = nPoints+order;
	
	uint32_t expectedLen = 8 + nPoints*16 + numKnots*4;
	
	if (dataLen != expectedLen)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, expectedLen );
	}
	
	Out() << Indent() << Name() << " (\n" <<
		Indent(1) << order << "\t# order\n" <<
		Indent(1) << nPoints << "\t# numPoints\n" <<
		Indent(1) << "# points\n";
	
	uint32_t i, offset;
	offset = inStartOffset + 8;
	
	for (i = 0; i < nPoints; ++i)
	{
		Out() << Indent(1) <<
			FetchFloat32( offset ) << ' ' <<
			FetchFloat32( offset+4 ) << ' ' <<
			FetchFloat32( offset+8 ) << ' ' <<
			FetchFloat32( offset+12 ) << '\n';
		offset += 16;
	}
	
	Out() << Indent(1) << "# knots";
	
	for (i = 0; i < numKnots; ++i)
	{
		if ( (i % 10) == 0 )
		{
			Out() << '\n' << Indent(1);
		}
		Out() << FetchFloat32( offset ) << ' ';
		offset += 4;
	}
	
	Out() << '\n' << Indent() << ")\n";
}

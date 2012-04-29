/*
 *  SubdivisionStyle.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/1/12.
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

#include "SubdivisionStyle.h"

namespace
{
	const char* methodName[] = { "Constant", "WorldSpace", "ScreenSpace" };
}

SubdivisionStyle::SubdivisionStyle()
	: TypeHandler( 'sbdv', "SubdivisionStyle" )
{
}

void	SubdivisionStyle::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	if (inEndOffset - inStartOffset < 8)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 8 );
	}
	
	uint32_t methodEnum = FetchUInt32( inStartOffset );
	if (methodEnum == 0)
	{
		if (inEndOffset - inStartOffset != 12)
		{
			throw DataLengthException( Name(), inStartOffset, inEndOffset, 12 );
		}
	}
	else if (methodEnum > 2)
	{
		throw FormatException( Name(), inStartOffset );
	}
	else	// methodEnum is 1 or 2
	{
		if (inEndOffset - inStartOffset != 8)
		{
			throw DataLengthException( Name(), inStartOffset, inEndOffset, 8 );
		}
	}
	
	float value1;
	uint32_t const1, const2;
	if (methodEnum == 0)
	{
		const1 = FetchUInt32( inStartOffset+4 );
		const2 = FetchUInt32( inStartOffset+8 );
	}
	else
	{
		value1 = FetchFloat32( inStartOffset+4 );
	}
	
	Out() << Indent() << Name() << " ( " <<
		methodName[methodEnum] << " ";
	
	if (methodEnum == 0)
	{
		Out() << const1 << " " << const2 << " )\n";
	}
	else
	{
		Out() << value1 << " )\n";
	}
}

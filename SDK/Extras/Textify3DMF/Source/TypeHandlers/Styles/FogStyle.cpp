/*
 *  FogStyle.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/14/12.
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

#include "FogStyle.h"

namespace
{
	const char* kMode[] =
	{
		"Linear",
		"Exponential",
		"ExponentialSquared",
		"Alpha"
	};
}

FogStyle::FogStyle()
	: TypeHandler( 'fogg', "FogStyle" )
{
}

void	FogStyle::Process( uint32_t inStartOffset,
									uint32_t inEndOffset )
{
	if (inEndOffset != inStartOffset + 36)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 36 );
	}
	
	uint32_t switchVal = FetchUInt32( inStartOffset );
	if (switchVal > 1)
	{
		throw FormatException( Name(), inStartOffset );
	}

	uint32_t modeVal = FetchUInt32( inStartOffset+4 );
	if (modeVal > 3)
	{
		throw FormatException( Name(), inStartOffset+4 );
	}
	
	float fogStart = FetchFloat32( inStartOffset+8 );
	float fogEnd = FetchFloat32( inStartOffset+12 );
	float fogDensity = FetchFloat32( inStartOffset+16 );
	float color_a = FetchFloat32( inStartOffset+20 );
	float color_r = FetchFloat32( inStartOffset+24 );
	float color_g = FetchFloat32( inStartOffset+28 );
	float color_b = FetchFloat32( inStartOffset+32 );
	
	Out() << Indent() << Name() << " (\n" <<
		Indent(1) << ((switchVal == 0)? "Off" : "On") << ' ' <<
		kMode[ modeVal ] << '\n' <<
		Indent(1) << fogStart << ' ' << fogEnd << ' ' <<
		fogDensity << "\t# start, end, density\n" <<
		Indent(1) << color_a << ' ' << color_r << ' ' <<
		color_g << ' ' << color_b << "\t# color ARGB\n" <<
		Indent() << ")\n";
}

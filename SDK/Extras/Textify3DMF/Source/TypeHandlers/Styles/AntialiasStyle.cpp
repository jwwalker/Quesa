/*
 *  AntialiasStyle.cpp
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

#include "AntialiasStyle.h"

AntialiasStyle::AntialiasStyle()
	: TypeHandler( 'anti', "AntialiasStyle" )
{
}

void	AntialiasStyle::Process( uint32_t inStartOffset,
									uint32_t inEndOffset )
{
	if (inEndOffset != inStartOffset + 12)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 12 );
	}
	
	uint32_t switchVal = FetchUInt32( inStartOffset );
	uint32_t modeVal = FetchUInt32( inStartOffset+4 );
	float quality = FetchFloat32( inStartOffset+8 );
	
	if (switchVal > 1)
	{
		throw FormatException( Name(), inStartOffset );
	}
	if (modeVal > 7)
	{
		throw FormatException( Name(), inStartOffset+4 );
	}
	
	Out() << Indent() << Name() << " ( " <<
		((switchVal == 0)? "Off" : "On") << ' ';
	
	if (modeVal == 0)
	{
		Out() << "None ";
	}
	else
	{
		bool wroteFlag = false;
		if ( (modeVal & 1) != 0 )
		{
			Out() << "Edges ";
			wroteFlag = true;
		}
		if ( (modeVal & 2) != 0 )
		{
			if (wroteFlag)
			{
				Out() << "| ";
			}
			Out() << "Filled ";
			wroteFlag = true;
		}
		if ( (modeVal & 4) != 0 )
		{
			if (wroteFlag)
			{
				Out() << "| ";
			}
			Out() << "FullScreen ";
		}
	}
	
	Out() << quality << " ) \n";
}

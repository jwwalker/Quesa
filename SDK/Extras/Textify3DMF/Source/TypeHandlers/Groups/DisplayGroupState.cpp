/*
 *  DisplayGroupState.cpp
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

#include "DisplayGroupState.h"

namespace
{
	const char* kNoFlags = "None";

	const char* kFlagBits[] =
	{
		"Inline",
		"DoNotDraw",
		"NoBoundingBox",
		"NoBoundingSphere",
		"DoNotPick",
		"DoNotBound"
	};
	
	const int kNumBits = sizeof(kFlagBits) / sizeof(kFlagBits[0]);
}

DisplayGroupState::DisplayGroupState()
	: TypeHandler( 'dgst', "DisplayGroupState" )
{
}

void	DisplayGroupState::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen != 4)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 4 );
	}
	
	uint32_t flags = FetchUInt32( inStartOffset );
	
	if (flags == 0)
	{
		Out() << Indent() << Name() << " ( " <<
			kNoFlags << " )\n";
	}
	else
	{
		Out() << Indent() << Name() << " (\n";
		
		Out() << Indent(1);
		bool wroteSomething = false;
		
		for (int i = 0; i < kNumBits; ++i)
		{
			if ( (flags & (1L << i)) != 0 )
			{
				if (wroteSomething)
				{
					Out() << " | ";
				}
				Out() << kFlagBits[i];
				wroteSomething = true;
			}
		}
		
		Out() << "\n" << Indent() << " )\n";
	}
}

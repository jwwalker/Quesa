/*
 *  TextureShaderBoundary.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 12/14/2021.
 *  Copyright (c) 2021 James W. Walker.
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

#include "TextureShaderBoundary.h"

static const char* kFlagNames[] =
{
	"Wrap",
	"Clamp",
	"Mirror"
};

TextureShaderBoundary::TextureShaderBoundary()
	: TypeHandler( 'shdr', "Shader" )
{
}

void	TextureShaderBoundary::Process( size_t inStartOffset,
									size_t inEndOffset )
{
	size_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen != 8)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 8 );
	}
	
	uint32_t uBoundary = FetchUInt32( inStartOffset );
	uint32_t vBoundary = FetchUInt32( inStartOffset+4 );
	
	if (uBoundary > 2)
	{
		throw FormatException( Name(), inStartOffset+4 );
	}
	if (vBoundary > 2)
	{
		throw FormatException( Name(), inStartOffset+8 );
	}
	
	Out() << Indent() << Name() << " ( " <<
		kFlagNames[ uBoundary ] << ' ' << kFlagNames[ vBoundary ] << " )\n";
}

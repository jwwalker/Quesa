/*
 *  Matrix.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 3/30/12.
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

#include "Matrix.h"

#include "Controller.h"

Matrix::Matrix()
	: TypeHandler( 'mtrx', "Matrix" )
{
}

void	Matrix::Process( uint32_t inStartOffset, uint32_t inEndOffset )
{
	
	if (inEndOffset != inStartOffset + 64)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 64 );
	}

	Out() << Indent() << Name() << " (\n";
	Out() << Indent(1) <<
		FetchFloat32( inStartOffset ) << " " <<
		FetchFloat32( inStartOffset+4 ) << " " <<
		FetchFloat32( inStartOffset+8 ) << " " <<
		FetchFloat32( inStartOffset+12 ) << "\n";
	Out() << Indent(1) <<
		FetchFloat32( inStartOffset+16 ) << " " <<
		FetchFloat32( inStartOffset+20 ) << " " <<
		FetchFloat32( inStartOffset+24 ) << " " <<
		FetchFloat32( inStartOffset+28 ) << "\n";
	Out() << Indent(1) <<
		FetchFloat32( inStartOffset+32 ) << " " <<
		FetchFloat32( inStartOffset+36 ) << " " <<
		FetchFloat32( inStartOffset+40 ) << " " <<
		FetchFloat32( inStartOffset+44 ) << "\n";
	Out() << Indent(1) <<
		FetchFloat32( inStartOffset+48 ) << " " <<
		FetchFloat32( inStartOffset+52 ) << " " <<
		FetchFloat32( inStartOffset+56 ) << " " <<
		FetchFloat32( inStartOffset+60 ) << "\n";
	Out() << Indent() << ")\n";
}

/*
 *  Cylinder.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/15/12.
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

#include "Cylinder.h"

Cylinder::Cylinder()
	: TypeHandler( 'cyln', "Cylinder" )
{
}

void	Cylinder::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen == 0)
	{
		Out() << Indent() << Name() <<
			" ( )\n";
	}
	else
	{
		if (dataLen != 64)
		{
			throw DataLengthException( Name(), inStartOffset, inEndOffset, 64 );
		}
		
		Out() << Indent() << Name() << " (\n" <<
		
			Indent(1) << FetchFloat32( inStartOffset ) << " " <<
			FetchFloat32( inStartOffset+4 ) << " " <<
			FetchFloat32( inStartOffset+8 ) << "\t# orientation\n" <<
		
			Indent(1) << FetchFloat32( inStartOffset+12 ) << " " <<
			FetchFloat32( inStartOffset+16 ) << " " <<
			FetchFloat32( inStartOffset+20 ) << "\t# majorRadius\n" <<
		
			Indent(1) << FetchFloat32( inStartOffset+24 ) << " " <<
			FetchFloat32( inStartOffset+28 ) << " " <<
			FetchFloat32( inStartOffset+32 ) << "\t# minorRadius\n" <<
		
			Indent(1) << FetchFloat32( inStartOffset+36 ) << " " <<
			FetchFloat32( inStartOffset+40 ) << " " <<
			FetchFloat32( inStartOffset+44 ) << "\t# origin\n" <<
			
			Indent(1) << FetchFloat32( inStartOffset+48 ) <<
				"\t# uMin\n" <<
			Indent(1) << FetchFloat32( inStartOffset+52 ) <<
				"\t# uMax\n" <<
			Indent(1) << FetchFloat32( inStartOffset+56 ) <<
				"\t# vMin\n" <<
			Indent(1) << FetchFloat32( inStartOffset+60 ) <<
				"\t# vMax\n" <<
			
			Indent() << ")\n";
	}
}

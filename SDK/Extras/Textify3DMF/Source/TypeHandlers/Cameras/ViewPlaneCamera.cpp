/*
 *  ViewPlaneCamera.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/21/12.
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

#include "ViewPlaneCamera.h"

ViewPlaneCamera::ViewPlaneCamera()
	: TypeHandler( 'vwpl', "ViewPlaneCamera" )
{
}


void	ViewPlaneCamera::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	if (inEndOffset != inStartOffset + 20)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 20 );
	}
	
	Out() << Indent() << Name() << " (\n" <<
	
		Indent(1) << FetchFloat32( inStartOffset ) << "\t# viewPlane\n" <<
		Indent(1) << FetchFloat32( inStartOffset+4 ) << "\t# halfWidthAtViewPlane\n" <<
		Indent(1) << FetchFloat32( inStartOffset+8 ) << "\t# halfHeightatViewPlane\n" <<
		Indent(1) << FetchFloat32( inStartOffset+12 ) << "\t# centerXOnViewPlane\n" <<
		Indent(1) << FetchFloat32( inStartOffset+16 ) << "\t# centerYOnViewPlane\n" <<
	
		Indent() << ")\n";
}

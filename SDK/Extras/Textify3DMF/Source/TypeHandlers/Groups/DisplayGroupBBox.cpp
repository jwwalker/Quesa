/*
 *  DisplayGroupBBox.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 3/26/2013.
 *  Copyright 2013 ScriptPerfection Enterprises, Inc. All rights reserved.
 *
 */

#include "DisplayGroupBBox.h"

DisplayGroupBBox::DisplayGroupBBox()
	: TypeHandler( 'dgbb', "DisplayGroupBBox" )
{
}

void	DisplayGroupBBox::Process( uint32_t inStartOffset,
									uint32_t inEndOffset )
{
	if (inEndOffset != inStartOffset + 24)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 24 );
	}

	Out() << Indent() << Name() << " ( " <<
		FetchFloat32( inStartOffset ) << " " <<
		FetchFloat32( inStartOffset+4 ) << " " <<
		FetchFloat32( inStartOffset+8 ) << " " <<
		FetchFloat32( inStartOffset+12 ) << " " <<
		FetchFloat32( inStartOffset+16 ) << " " <<
		FetchFloat32( inStartOffset+20 ) << " )\n";
}

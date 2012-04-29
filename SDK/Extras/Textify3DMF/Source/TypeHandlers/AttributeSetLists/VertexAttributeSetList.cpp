/*
 *  VertexAttributeSetList.cpp
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

#include "VertexAttributeSetList.h"

#include "Controller.h"

AttributeSetList::AttributeSetList(	TypeCode inType,
									const char* inName )
	: TypeHandler( inType, inName )
{
}


void	AttributeSetList::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	if ( inEndOffset - inStartOffset < 12 )
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 12 );
	}
	
	uint32_t nObjects = FetchUInt32( inStartOffset );
	
	uint32_t packingEnum = FetchUInt32( inStartOffset+4 );
	
	// legal packing values are 0 or 1
	if (packingEnum >= 2)
	{
		throw FormatException( Name(), inStartOffset+4 );
	}
	const char* packingWord = (packingEnum == 0)? "include" : "exclude";
	
	uint32_t nIndices = FetchUInt32( inStartOffset+8 );
	uint32_t expectedSize = 12 + nIndices * 4;
	
	if (inEndOffset - inStartOffset < expectedSize)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset,
			expectedSize );
	}
	
	if (nIndices == 0)
	{
		Out() << Indent() << Name() << " ( " <<
			nObjects << " " << packingWord << " 0 )\n";
	}
	else
	{
		Out() << Indent() << Name() << " (\n";
		Out() << Indent(1) <<
			nObjects << " " << packingWord << " " << nIndices;
		
		uint32_t indexOffset = inStartOffset + 12;
		
		for (uint32_t i = 0; i < nIndices; ++i)
		{
			if ( (i % 10) == 0 )
			{
				Out() << "\n" << Indent(1);
			}
			else
			{
				Out() << " ";
			}
			uint32_t anIndex = FetchUInt32( indexOffset );
			Out() << anIndex;
			indexOffset += 4;
		}
		Out() << "\n";
		Out() << Indent() << ")\n";
	}
}


VertexAttributeSetList::VertexAttributeSetList()
	: AttributeSetList( 'vasl', "VertexAttributeSetList" )
{
}



FaceAttributeSetList::FaceAttributeSetList()
	: AttributeSetList( 'fasl', "FaceAttributeSetList" )
{
}



GeometryAttributeSetList::GeometryAttributeSetList()
	: AttributeSetList( 'gasl', "GeometryAttributeSetList" )
{
}



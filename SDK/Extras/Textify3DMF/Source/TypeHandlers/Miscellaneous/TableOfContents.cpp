/*
 *  TableOfContents.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/7/12.
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

#include "TableOfContents.h"

TableOfContents::TableOfContents()
	: TypeHandler( 'toc ', "TableOfContents" )
{
}

void	TableOfContents::PreParse( uint32_t inStartOffset,
									uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 28)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 28 );
	}
	
	uint32_t tocEntryType = FetchUInt32( inStartOffset+16 );
	uint32_t tocEntrySize = FetchUInt32( inStartOffset+20 );
	uint32_t nEntries = FetchUInt32( inStartOffset+24 );
	
	if (tocEntryType > 1)
	{
		throw FormatException( Name(), inStartOffset+16 );
	}
	if ( ((tocEntryType == 0) and (tocEntrySize != 12)) or
		((tocEntryType == 1) and (tocEntrySize != 16)) )
	{
		throw FormatException( Name(), inStartOffset+16 );
	}

	uint32_t expectedLen = 28 + nEntries * tocEntrySize;
	if (dataLen < expectedLen)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset,
			expectedLen );
	}
	
	for (uint32_t i = 0; i < nEntries; ++i)
	{
		uint32_t refID = FetchUInt32( inStartOffset+28 + i*tocEntrySize );
		uint64_t obOffset = Boss()->FetchUInt64( inStartOffset+28 +
			i*tocEntrySize + 4 );
		TypeCode obType = 0;
		if (tocEntrySize == 16)
		{
			obType = FetchUInt32( inStartOffset+28 + i*tocEntrySize + 12 );
		}
		Boss()->RecordTOCEntry( refID, obOffset, obType );
	}
}

void	TableOfContents::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 28)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 28 );
	}
	
	//uint64_t nextToc = Boss()->FetchUInt64( inStartOffset );
	uint32_t refSeed = FetchUInt32( inStartOffset+8 );
	int32_t typeSeed = (int32_t)FetchUInt32( inStartOffset+12 );
	uint32_t tocEntryType = FetchUInt32( inStartOffset+16 );
	uint32_t tocEntrySize = FetchUInt32( inStartOffset+20 );
	uint32_t nEntries = FetchUInt32( inStartOffset+24 );
	
	uint32_t expectedLen = 28 + nEntries * tocEntrySize;
	if (dataLen != expectedLen)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset,
			expectedLen );
	}
	
	if (tocEntryType > 1)
	{
		throw FormatException( Name(), inStartOffset+16 );
	}
	if ( ((tocEntryType == 0) and (tocEntrySize != 12)) or
		((tocEntryType == 1) and (tocEntrySize != 16)) )
	{
		throw FormatException( Name(), inStartOffset+16 );
	}
	
	int whichTOC = Boss()->GetTOCIndex( inStartOffset - 8 );
	
	Out() <<
		Indent() << "tableofcontents" << whichTOC << ":\n" <<
	
		Indent() << Name() << " (\n" <<
	
		Indent(1) << "tableofcontents" << (whichTOC + 1) <<
			">\t# next TOC\n" <<
		Indent(1) << refSeed << "\t# reference seed\n" <<
		Indent(1) << typeSeed << "\t# type seed\n" <<
		Indent(1) << tocEntryType << "\t# tocEntryType\n" <<
		Indent(1) << tocEntrySize << "\t# tocEntrySize\n" <<
		Indent(1) << nEntries << "\t# nEntries\n";
	
	std::string className, label;
	uint32_t refID;
	
	for (uint32_t i = 0; i < nEntries; ++i)
	{
		uint64_t obOffset = Boss()->FetchUInt64( inStartOffset+28 +
			i*tocEntrySize + 4 );
		
		if (Boss()->FindTOCEntry( obOffset, refID, className, label ))
		{
			Out() << Indent(1) << refID << ' ' <<
				label << ">\n";
			if (tocEntryType == 1)
			{
				Out() << Indent(1) << className << '\n';
			}
		}
	}
	
	Out() << Indent() << ")\n";
}

/*
 *  Polyhedron.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/17/12.
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

#include "Polyhedron.h"

namespace
{
	const char* kFlags[] =
	{
		"Edge01",
		"Edge12",
		"Edge20"
	};
}

Polyhedron::Polyhedron()
	: TypeHandler( 'plhd', "Polyhedron" )
{
}

void	Polyhedron::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 12)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 12 );
	}
	
	uint32_t numVertices = FetchUInt32( inStartOffset );
	uint32_t numEdges = FetchUInt32( inStartOffset+4 );
	uint32_t numTriangles = FetchUInt32( inStartOffset+8 );
	
	uint32_t expectedLen = 12 + numVertices*12 + numEdges*16 + numTriangles*16;
	
	if (dataLen != expectedLen)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, expectedLen );
	}
	
	Out() << Indent() << Name() << " (\n" <<
		Indent(1) << numVertices << ' ' << numEdges << ' ' <<
		numTriangles << "\t# numVertices, numEdges, numTriangles\n";
	
	uint32_t i;
	uint32_t offset = inStartOffset + 12;
	
	// Vertices
	for (i = 0; i < numVertices; ++i)
	{
		Out() << Indent(1) << FetchFloat32(offset) <<
			' ' << FetchFloat32(offset+4) << ' ' <<
			FetchFloat32(offset+8);
		offset += 12;
		if (i == 0)
		{
			Out() << "\t# vertices\n";
		}
		else
		{
			Out() << '\n';
		}
	}
	
	// Edges
	for (i = 0; i < numEdges; ++i)
	{
		Out() << Indent(1) << FetchUInt32(offset) <<
			' ' << FetchUInt32(offset+4) << ' ' <<
			FetchUInt32(offset+8) << ' ' <<
			FetchUInt32(offset+12);
		offset += 16;
		if (i == 0)
		{
			Out() << "\t# edges\n";
		}
		else
		{
			Out() << '\n';
		}
	}
	
	// Triangles
	for (i = 0; i < numTriangles; ++i)
	{
		Out() << Indent(1) << FetchUInt32(offset) <<
			' ' << FetchUInt32(offset+4) << ' ' <<
			FetchUInt32(offset+8) << ' ';
		uint32_t flags = FetchUInt32(offset+12);
		offset += 16;
		if ((flags & 7) == 0)
		{
			Out() << "None";
		}
		else
		{
			bool wroteSomething = false;
			for (int j = 0; j < 3; ++j)
			{
				if ( (flags & (1L << j)) != 0 )
				{
					if (wroteSomething)
					{
						Out() << " | ";
					}
					Out() << kFlags[j];
					wroteSomething = true;
				}
			}
		}
		if (i == 0)
		{
			Out() << "\t# triangles\n";
		}
		else
		{
			Out() << '\n';
		}
	}
	
	Out() << Indent() << ")\n";
}

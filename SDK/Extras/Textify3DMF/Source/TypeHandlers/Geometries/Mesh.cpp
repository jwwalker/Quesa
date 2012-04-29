/*
 *  Mesh.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/18/12.
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

#include "Mesh.h"

Mesh::Mesh()
	: TypeHandler( 'mesh', "Mesh" )
{
}

void	Mesh::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 4)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 4 );
	}
	
	uint32_t numVertices = FetchUInt32( inStartOffset );
	
	// length of data through numContours
	uint32_t expectedLen = 4 + numVertices*12 + 8;
	
	if (dataLen < expectedLen)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, expectedLen );
	}
	
	Out() << Indent() << Name() << " (\n" <<
		Indent(1) << numVertices << "\t# numVertices\n";
	
	uint32_t offset = inStartOffset + 4;
	uint32_t i, j;
	
	for (i = 0; i < numVertices; ++i)
	{
		Out() << Indent(1) <<
			FetchFloat32( offset ) << ' ' <<
			FetchFloat32( offset+4 ) << ' ' <<
			FetchFloat32( offset+8 ) << "\t# vertex " << i << '\n';
		offset += 12;
	}
	
	uint32_t nFaces = FetchUInt32( offset );
	uint32_t nContours = FetchUInt32( offset+4 );
	offset += 8;
	
	Out() << '\n' << Indent(1) << nFaces <<
		"\t# numFaces\n" << Indent(1) << nContours <<
		"\t# numContours\n\n";
	
	uint32_t nFaceObs = nFaces + nContours;
	uint32_t facesSeen = 0;
	uint32_t contoursSeen = 0;
	
	for (i = 0; i < nFaceObs; ++i)
	{
		expectedLen += 4;
		if (dataLen < expectedLen)
		{
			throw DataLengthException( Name(), inStartOffset, inEndOffset, expectedLen );
		}
		int32_t indCount = (int32_t)FetchUInt32( offset );
		uint32_t numIndices;
		offset += 4;
		if (indCount < 0)
		{
			contoursSeen += 1;
			numIndices = - indCount;
		}
		else
		{
			facesSeen += 1;
			numIndices = indCount;
		}
		expectedLen += 4 * numIndices;
		if (dataLen < expectedLen)
		{
			throw DataLengthException( Name(), inStartOffset, inEndOffset, expectedLen );
		}
		Out() << Indent(1) << indCount;
		for (j = 0; j < numIndices; ++j)
		{
			Out() << ' ' << FetchUInt32( offset );
			offset += 4;
		}
		Out() << '\n';
	}
	
	if (facesSeen != nFaces)
	{
		Boss()->ErrorStream() << nFaces << " faces declared, " << facesSeen <<
			" encountered in Mesh at offset " << inStartOffset << '\n';
	}

	if (dataLen != expectedLen)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, expectedLen );
	}
	
	Out() << Indent() << ")\n";
}

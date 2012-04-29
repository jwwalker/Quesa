/*
 *  TriMesh.cpp
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

#include "TriMesh.h"


TriMesh::TriMesh()
	: TypeHandler( 'tmsh', "TriMesh" )
	, mFaces( 0 )
	, mEdges( 0 )
	, mPoints( 0 )
{
}

void	TriMesh::GetCounts( uint32_t& outFaces,
							uint32_t& outEdges,
							uint32_t& outPoints ) const
{
	outFaces = mFaces;
	outEdges = mEdges;
	outPoints = mPoints;
}

void	TriMesh::Process( uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 52)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 52 );
	}
	
	mFaces = FetchUInt32( inStartOffset );
	uint32_t numFaceAtts = FetchUInt32( inStartOffset+4 );
	mEdges = FetchUInt32( inStartOffset+8 );
	uint32_t numEdgeAtts = FetchUInt32( inStartOffset+12 );
	mPoints = FetchUInt32( inStartOffset+16 );
	uint32_t numPointAtts = FetchUInt32( inStartOffset+20 );
	
	int bytesPointIndex, bytesPerFaceIndex;
	if (mPoints - 1 <= 0xFE)
	{
		bytesPointIndex = 1;
	}
	else if (mPoints - 1 <= 0xFFFE)
	{
		bytesPointIndex = 2;
	}
	else
	{
		bytesPointIndex = 4;
	}

	if (mFaces - 1 <= 0xFE)
	{
		bytesPerFaceIndex = 1;
	}
	else if (mFaces - 1 <= 0xFFFE)
	{
		bytesPerFaceIndex = 2;
	}
	else
	{
		bytesPerFaceIndex = 4;
	}
	
	uint32_t trianglesSize = mFaces * 3 * bytesPointIndex;
	uint32_t edgesSize = mEdges * 2 * (bytesPointIndex + bytesPerFaceIndex);
	uint32_t pointsSize = mPoints * 12;
	uint32_t expectedLength = 52 + trianglesSize + edgesSize + pointsSize;
	if (dataLen != expectedLength)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset,
			expectedLength );
	}
	
	Out() << Indent() << Name() << " (\n" <<
	
		Indent(1) << mFaces << " " << numFaceAtts << " " <<
			mEdges << " " << numEdgeAtts << " " <<
			mPoints << " " << numPointAtts << "\t" <<
			"# faces faceAtts edges edgeAtts pts ptAtts\n";

	WriteTriangles( inStartOffset+24, mFaces, bytesPointIndex );
	WriteEdges( inStartOffset+24+trianglesSize, mEdges, bytesPointIndex,
		bytesPerFaceIndex );
	WritePoints( inStartOffset+24+trianglesSize+edgesSize, mPoints );
	WriteBoundingBox( inStartOffset+24+trianglesSize+edgesSize+pointsSize );
	
	Out() << Indent() << ")\n";
}


uint32_t	TriMesh::FetchIndex( uint32_t inStartOffset, int inBytes )
{
	uint32_t result = 0;
	
	if (inBytes == 1)
	{
		result = Boss()->FetchUInt8( inStartOffset );
		if (result == 0xFF)
		{
			result = 0xFFFFFFFFUL;
		}
	}
	else if (inBytes == 2)
	{
		if (result == 0xFFFF)
		{
			result = 0xFFFFFFFFUL;
		}
		result = Boss()->FetchUInt16( inStartOffset );
	}
	else
	{
		result = FetchUInt32( inStartOffset );
	}
	
	return result;
}

void	TriMesh::WriteTriangles( uint32_t inStartOffset,
										uint32_t inNumFaces,
										int inBytesPerPointIndex )
{
	for (uint32_t i = 0; i < inNumFaces; ++i)
	{
		uint32_t ind1 = FetchIndex( inStartOffset + i*3*inBytesPerPointIndex,
			inBytesPerPointIndex );
		uint32_t ind2 = FetchIndex( inStartOffset + i*3*inBytesPerPointIndex +
			inBytesPerPointIndex, inBytesPerPointIndex );
		uint32_t ind3 = FetchIndex( inStartOffset + i*3*inBytesPerPointIndex +
			2 * inBytesPerPointIndex, inBytesPerPointIndex );
		
		Out() << Indent(1) << ind1 << ' ' << ind2 <<
			' ' << ind3;
		if (i == 0)
		{
			Out() << "\t# triangles\n";
		}
		else
		{
			Out() << '\n';
		}
	}
}

void	TriMesh::WriteEdges( uint32_t inStartOffset,
										uint32_t inNumEdges,
										int inBytesPerPointIndex,
										int inBytesPerFaceIndex )
{
	uint32_t edgeDataLen = 2 * (inBytesPerPointIndex + inBytesPerFaceIndex);
	
	for (uint32_t i = 0; i < inNumEdges; ++i)
	{
		uint32_t ptInd1 = FetchIndex( inStartOffset + i*edgeDataLen,
			inBytesPerPointIndex );
		uint32_t ptInd2 = FetchIndex( inStartOffset + i*edgeDataLen +
			inBytesPerPointIndex, inBytesPerPointIndex );
		uint32_t faceInd1 = FetchIndex( inStartOffset + i*edgeDataLen +
			2*inBytesPerPointIndex, inBytesPerFaceIndex );
		uint32_t faceInd2 = FetchIndex( inStartOffset + i*edgeDataLen +
			2*inBytesPerPointIndex + inBytesPerFaceIndex, inBytesPerFaceIndex );
		Out() << Indent(1) << ptInd1 << ' ' << ptInd2 <<
			' ' << faceInd1 << ' ' << faceInd2;
		if (i == 0)
		{
			Out() << "\t# edges\n";
		}
		else
		{
			Out() << '\n';
		}
	}
}

void	TriMesh::WritePoints( uint32_t inStartOffset, uint32_t inNumPoints )
{
	for (uint32_t i = 0; i < inNumPoints; ++i)
	{
		float x = FetchFloat32( inStartOffset + i*12 );
		float y = FetchFloat32( inStartOffset + i*12 + 4 );
		float z = FetchFloat32( inStartOffset + i*12 + 8 );
		Out() << Indent(1) << x << ' ' << y << ' ' << z;
		if (i == 0)
		{
			Out() << "\t# points\n";
		}
		else
		{
			Out() << '\n';
		}
	}
}

void	TriMesh::WriteBoundingBox( uint32_t inStartOffset )
{
	float xMin = FetchFloat32( inStartOffset );
	float yMin = FetchFloat32( inStartOffset+4 );
	float zMin = FetchFloat32( inStartOffset+8 );
	float xMax = FetchFloat32( inStartOffset+12 );
	float yMax = FetchFloat32( inStartOffset+16 );
	float zMax = FetchFloat32( inStartOffset+20 );
	uint32_t emptyFlag = FetchUInt32( inStartOffset+24 );
	Out() << Indent(1) <<
		xMin << ' ' << yMin << ' ' << zMin << ' ' <<
		xMax << ' ' << yMax << ' ' << zMax << ' ' <<
		((emptyFlag == 0)? "False" : "True") << "\t# bounding box\n";
}

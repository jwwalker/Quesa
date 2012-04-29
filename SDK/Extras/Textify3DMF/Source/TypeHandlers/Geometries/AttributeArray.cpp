/*
 *  AttributeArray.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 4/8/12.
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

#include "AttributeArray.h"

namespace
{
enum TQ3AttributeTypes {
    kQ3AttributeTypeNone                        = 0,            // N/A
    kQ3AttributeTypeSurfaceUV                   = 1,            // TQ3Param2D
    kQ3AttributeTypeShadingUV                   = 2,            // TQ3Param2D
    kQ3AttributeTypeNormal                      = 3,            // TQ3Vector3D
    kQ3AttributeTypeAmbientCoefficient          = 4,            // float
    kQ3AttributeTypeDiffuseColor                = 5,            // TQ3ColorRGB
    kQ3AttributeTypeSpecularColor               = 6,            // TQ3ColorRGB
    kQ3AttributeTypeSpecularControl             = 7,            // float
    kQ3AttributeTypeTransparencyColor           = 8,            // TQ3ColorRGB
    kQ3AttributeTypeSurfaceTangent              = 9,            // TQ3Tangent2D
    kQ3AttributeTypeHighlightState              = 10,           // TQ3Switch
    kQ3AttributeTypeSurfaceShader               = 11,           // TQ3SurfaceShaderObject
    kQ3AttributeTypeEmissiveColor               = 12,           // TQ3ColorRGB
    kQ3AttributeTypeNumTypes                    = 13,           // N/A
    kQ3AttributeTypeSize32                      = 0xFFFFFFFF
};

}

AttributeArray::AttributeArray()
	: TypeHandler( 'atar', "AttributeArray" )
{
}

void	AttributeArray::Process( uint32_t inStartOffset, uint32_t inEndOffset )
{
	uint32_t dataLen = inEndOffset - inStartOffset;
	
	if (dataLen < 20)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, 20 );
	}

	int32_t attType = (int32_t) FetchUInt32( inStartOffset );
	uint32_t reserved = FetchUInt32( inStartOffset+4 );
	uint32_t positionOfArray = FetchUInt32( inStartOffset+8 );
	uint32_t positionInArray = FetchUInt32( inStartOffset+12 );
	uint32_t attUseFlag = FetchUInt32( inStartOffset+16 );
	
	if ( (attType >= kQ3AttributeTypeNumTypes) or (attType < 1) )
	{
		throw FormatException( Name(), inStartOffset );
	}
	if (reserved != 0)
	{
		throw FormatException( Name(), inStartOffset+4 );
	}
	if (positionOfArray > 2)
	{
		throw FormatException( Name(), inStartOffset+8 );
	}
	if (attUseFlag > 1)
	{
		throw FormatException( Name(), inStartOffset+16 );
	}
	
	uint32_t faceCount, edgeCount, pointCount;
	Boss()->LastTriMeshCounts( faceCount, edgeCount, pointCount );
	
	uint32_t elementCount;
	const char* elementKindName = NULL;
	switch (positionOfArray)
	{
		case 0:
			elementCount = faceCount;
			elementKindName = "face";
			break;
			
		case 1:
			elementCount = edgeCount;
			elementKindName = "edge";
			break;
			
		case 2:
			elementCount = pointCount;
			elementKindName = "point";
			break;
	}
	
	uint32_t sizePerElement = 0;
	const char* attKindName = "";
	switch (attType)
	{
		case kQ3AttributeTypeSurfaceUV:
		case kQ3AttributeTypeShadingUV:
			sizePerElement = 2 * sizeof(float);
			attKindName = "UV";
			break;
		
		case kQ3AttributeTypeNormal:
			sizePerElement = 3 * sizeof(float);
			attKindName = "normal";
			break;

		case kQ3AttributeTypeDiffuseColor:
			sizePerElement = 3 * sizeof(float);
			attKindName = "diffuse color";
			break;

		case kQ3AttributeTypeSpecularColor:
			sizePerElement = 3 * sizeof(float);
			attKindName = "specular color";
			break;

		case kQ3AttributeTypeTransparencyColor:
			sizePerElement = 3 * sizeof(float);
			attKindName = "transparency color";
			break;

		case kQ3AttributeTypeEmissiveColor:
			sizePerElement = 3 * sizeof(float);
			attKindName = "emissive color";
			break;
		
		case kQ3AttributeTypeAmbientCoefficient:
			sizePerElement = 1 * sizeof(float);
			attKindName = "ambient coefficient";
			break;

		case kQ3AttributeTypeSpecularControl:
			sizePerElement = 1 * sizeof(float);
			attKindName = "specular control";
			break;
		
		case kQ3AttributeTypeSurfaceTangent:
			sizePerElement = 6 * sizeof(float);
			attKindName = "tangent";
			break;
		
		case kQ3AttributeTypeHighlightState:
			sizePerElement = 1 * sizeof(uint32_t);
			attKindName = "highlight state";
			break;
		
		case kQ3AttributeTypeSurfaceShader:
			sizePerElement = 0;
			attKindName = "surface shader";
	}
	uint32_t arraySize = elementCount * sizePerElement;
	uint32_t useArraySize = elementCount * attUseFlag;
	uint32_t expectedLen = 20 + arraySize + useArraySize;
	
	if (dataLen != expectedLen)
	{
		throw DataLengthException( Name(), inStartOffset, inEndOffset, expectedLen );
	}
	
	Out() << Indent() << Name() << "(\n" <<
		Indent(1) << attType << " 0 " << positionOfArray << ' ' <<
		positionInArray << ' ' << attUseFlag <<
		"\t# " << elementKindName << ' ' << attKindName << '\n';
	
	switch (attType)
	{
		case kQ3AttributeTypeAmbientCoefficient:
			WriteFloatArray( inStartOffset+20, elementCount, 1 );
			break;

		case kQ3AttributeTypeSurfaceUV:
		case kQ3AttributeTypeShadingUV:
			WriteFloatArray( inStartOffset+20, elementCount, 2 );
			break;
		
		case kQ3AttributeTypeNormal:
		case kQ3AttributeTypeDiffuseColor:
		case kQ3AttributeTypeSpecularColor:
		case kQ3AttributeTypeTransparencyColor:
		case kQ3AttributeTypeEmissiveColor:
			WriteFloatArray( inStartOffset+20, elementCount, 3 );
			break;
		
		case kQ3AttributeTypeSurfaceTangent:
			WriteFloatArray( inStartOffset+20, elementCount, 6 );
			break;
		
		case kQ3AttributeTypeHighlightState:
			WriteSwitchArray( inStartOffset+20, elementCount );
			break;
	}
	
	if (attUseFlag)
	{
		WriteUseArray( inStartOffset + 20 + arraySize, elementCount );
	}
	
	Out() << Indent() << ")\n";
}

void	AttributeArray::WriteUseArray( uint32_t inStartOffset,
									uint32_t inElementCount )
{
	uint32_t offset = inStartOffset;
	
	for (uint32_t element = 0; element < inElementCount; ++element)
	{
		Out() << Indent(1) <<
			(int)Boss()->FetchUInt8( offset ) << '\n';
		offset += sizeof(uint8_t);
	}
}

void	AttributeArray::WriteSwitchArray( uint32_t inStartOffset,
									uint32_t inElementCount )
{
	uint32_t offset = inStartOffset;
	
	for (uint32_t element = 0; element < inElementCount; ++element)
	{
		Out() << Indent(1) <<
			(FetchUInt32( offset )? "On" : "Off") << '\n';
		offset += sizeof(uint32_t);
	}
}

void	AttributeArray::WriteFloatArray( uint32_t inStartOffset,
										uint32_t inElementCount,
										uint32_t inFloatsPerElement )
{
	uint32_t offset = inStartOffset;
	
	for (uint32_t element = 0; element < inElementCount; ++element)
	{
		Out() << Indent(1);
		
		for (uint32_t i = 0; i < inFloatsPerElement; ++i)
		{
			if (i > 0)
			{
				Out() << ' ';
			}
			Out() << FetchFloat32( offset );
			offset += sizeof(float);
		}
		
		Out() << '\n';
	}
}

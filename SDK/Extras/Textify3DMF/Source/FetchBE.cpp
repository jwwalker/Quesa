#include "FetchBE.h"

uint32_t FetchBEULong( const uint8_t* inData, int32_t inOffset )
{
	uint32_t theNum =
		((uint32_t)inData[inOffset] << 24) |
		((uint32_t)inData[inOffset + 1] << 16) |
		((uint32_t)inData[inOffset + 2] << 8) |
		((uint32_t)inData[inOffset + 3] << 0);
	
	return theNum;
}

uint16_t FetchBEUShort( const uint8_t* inData, int32_t inOffset )
{
	uint16_t theNum =
		((uint32_t)inData[inOffset] << 8) |
		((uint32_t)inData[inOffset + 1] << 0);
	
	return theNum;
}

uint64_t FetchBEU64( const uint8_t* inData, int32_t inOffset )
{
	uint64_t theNum =
		((uint64_t)inData[inOffset] << 56) |
		((uint64_t)inData[inOffset + 1] << 48) |
		((uint64_t)inData[inOffset + 2] << 40) |
		((uint64_t)inData[inOffset + 3] << 32) |
		((uint64_t)inData[inOffset + 4] << 24) |
		((uint64_t)inData[inOffset + 5] << 16) |
		((uint64_t)inData[inOffset + 6] << 8) |
		((uint64_t)inData[inOffset + 7] << 0);
	
	return theNum;
}

float	FetchBEFloat( const uint8_t* inData, int32_t inOffset )
{
	uint32_t	theLong = FetchBEULong( inData, inOffset );
	float	theFloat = *(float*)&theLong;
	return theFloat;
}

int32_t FetchBELong( const uint8_t* inData, int32_t inOffset )
{
	uint32_t	theLong = FetchBEULong( inData, inOffset );
	return *(int32_t*)&theLong;
}

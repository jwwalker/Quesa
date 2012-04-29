#include "FetchLE.h"


uint32_t FetchLEULong( const uint8_t* inData, int32_t inOffset )
{
	uint32_t theNum =
		((uint32_t)inData[inOffset] << 0) |
		((uint32_t)inData[inOffset + 1] << 8) |
		((uint32_t)inData[inOffset + 2] << 16) |
		((uint32_t)inData[inOffset + 3] << 24);
	
	return theNum;
}

uint16_t FetchLEUShort( const uint8_t* inData, int32_t inOffset )
{
	uint16_t theNum =
		((uint32_t)inData[inOffset] << 0) |
		((uint32_t)inData[inOffset + 1] << 8);
	
	return theNum;
}

uint64_t FetchLEU64( const uint8_t* inData, int32_t inOffset )
{
	uint64_t theNum =
		((uint64_t)inData[inOffset] << 0) |
		((uint64_t)inData[inOffset + 1] << 8) |
		((uint64_t)inData[inOffset + 2] << 16) |
		((uint64_t)inData[inOffset + 3] << 24) |
		((uint64_t)inData[inOffset + 4] << 32) |
		((uint64_t)inData[inOffset + 5] << 40) |
		((uint64_t)inData[inOffset + 6] << 48) |
		((uint64_t)inData[inOffset + 7] << 56);
	
	return theNum;
}

float	FetchLEFloat( const uint8_t* inData, int32_t inOffset )
{
	uint32_t	theLong = FetchLEULong( inData, inOffset );
	float	theFloat = *(float*)&theLong;
	return theFloat;
}

int32_t FetchLELong( const uint8_t* inData, int32_t inOffset )
{
	uint32_t	theLong = FetchLEULong( inData, inOffset );
	return *(int32_t*)&theLong;
}

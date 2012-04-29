#include "Fetch.h"

#include "FetchBE.h"
#include "FetchLE.h"


static uint16_t (*sFetchUShort)( const uint8_t* inData, int32_t inOffset ) =
	FetchBEUShort;

static uint32_t (*sFetchULong)( const uint8_t* inData, int32_t inOffset ) =
	FetchBEULong;

static uint64_t (*sFetchU64)( const uint8_t* inData, int32_t inOffset ) =
	FetchBEU64;

static float	(*sFetchFloat)( const uint8_t* inData, int32_t inOffset ) =
	FetchBEFloat;

static int32_t (*sFetchLong)( const uint8_t* inData, int32_t inOffset ) =
	FetchBELong;

void	SetBigEndian( bool inBigEndian )
{
	if (inBigEndian)
	{
		sFetchUShort = FetchBEUShort;
		sFetchULong = FetchBEULong;
		sFetchU64 = FetchBEU64;
		sFetchFloat = FetchBEFloat;
		sFetchLong = FetchBELong;
	}
	else
	{
		sFetchUShort = FetchLEUShort;
		sFetchULong = FetchLEULong;
		sFetchU64 = FetchLEU64;
		sFetchFloat = FetchLEFloat;
		sFetchLong = FetchLELong;
	}
}

uint16_t FetchUShort( const uint8_t* inData, int32_t inOffset )
{
	return sFetchUShort( inData, inOffset );
}

uint32_t FetchULong( const uint8_t* inData, int32_t inOffset )
{
	return sFetchULong( inData, inOffset );
}

uint64_t FetchU64( const uint8_t* inData, int32_t inOffset )
{
	return sFetchU64( inData, inOffset );
}

float	FetchFloat( const uint8_t* inData, int32_t inOffset )
{
	return sFetchFloat( inData, inOffset );
}

int32_t FetchLong( const uint8_t* inData, int32_t inOffset )
{
	return sFetchLong( inData, inOffset );
}


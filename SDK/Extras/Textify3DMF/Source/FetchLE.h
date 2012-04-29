#pragma once

/*	---------------------------------------------------------------------------------------------
	The 3DMF file format has numbers of various sizes stored in
	bigendian format.  These functions read such numbers from an
	array of bytes into an integer variable.
	---------------------------------------------------------------------------------------------
*/

uint16_t FetchLEUShort( const uint8_t* inData, int32_t inOffset );

uint32_t FetchLEULong( const uint8_t* inData, int32_t inOffset );

uint64_t FetchLEU64( const uint8_t* inData, int32_t inOffset );

float	FetchLEFloat( const uint8_t* inData, int32_t inOffset );

int32_t FetchLELong( const uint8_t* inData, int32_t inOffset );


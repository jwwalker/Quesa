#pragma once

/*	---------------------------------------------------------------------------------------------
	The 3DMF file format has numbers of various sizes stored in
	bigendian format.  These functions read such numbers from an
	array of bytes into an integer variable.
	---------------------------------------------------------------------------------------------
*/

uint16_t FetchBEUShort( const uint8_t* inData, int32_t inOffset );

uint32_t FetchBEULong( const uint8_t* inData, int32_t inOffset );

uint64_t FetchBEU64( const uint8_t* inData, int32_t inOffset );

float	FetchBEFloat( const uint8_t* inData, int32_t inOffset );

int32_t FetchBELong( const uint8_t* inData, int32_t inOffset );


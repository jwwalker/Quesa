#pragma once

/*	---------------------------------------------------------------------------------------------
	The 3DMF file format has numbers of various sizes stored in
	bigendian format.  These functions read such numbers from an
	array of bytes into an integer variable.
	---------------------------------------------------------------------------------------------
*/

void	SetBigEndian( bool inBigEndian );

uint16_t FetchUShort( const uint8_t* inData, int32_t inOffset );

uint32_t FetchULong( const uint8_t* inData, int32_t inOffset );

uint64_t FetchU64( const uint8_t* inData, int32_t inOffset );

float	FetchFloat( const uint8_t* inData, int32_t inOffset );

int32_t FetchLong( const uint8_t* inData, int32_t inOffset );


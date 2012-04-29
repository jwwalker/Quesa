/*
 *  main.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 3/26/12.
 *
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
#include "Textify3DMF.h"

#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <vector>

using namespace std;

int main (int argc, char * const argv[])
{
	if ( argc < 2 )
	{
		cerr << "Too few arguments.\n" <<
					"Textify3DMF path.3dmf\n";
		return 1;
	}
	if ( argc > 2 )
	{
		cerr << "Too many arguments.\n" <<
					"Textify3DMF path.3dmf\n";
		return 1;
	}

	FILE* inFile = fopen( argv[1], "rb" );
	if (inFile == NULL)
	{
		cerr << "Cannot open input file '" << argv[1] << "'\n";
		return 2;
	}

	fseek( inFile, 0, SEEK_END );
	int32_t fileLen = ftell( inFile );
	fseek( inFile, 0, SEEK_SET );

	// Read the whole file into a buffer.
	vector<uint8_t>	dataBuf( fileLen );
	uint8_t*	bufStart = &dataBuf[0];
	int32_t	numRead = fread( bufStart, 1, fileLen, inFile );

	if (numRead < fileLen)
	{
		cerr << "Only read " << numRead << " bytes out of " << fileLen <<
			"expected." << endl;
		return 3;
	}
	else
	{
		try
		{
			Textify3DMF( bufStart, fileLen );
		}
		catch (...)
		{
			cerr << "Exception thrown." << endl;
			return 4;
		}
	}

    return 0;
}

/*
 *  Read3DMF.cpp
 *  Binify3DMF
 *
 *  Created by James Walker on 4/22/12.
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

#include "Read3DMF.h"

#include <QuesaIO.h>
#include <QuesaStorage.h>

#include <iostream>

/*!
	@function	Read3DMF
	
	@abstract	Read a 3DMF file, producing one or more Quesa objects.
	
	@param		inPath		File system path to a 3DMF file
	@param		outObjects	Receives objects.
	@param		outText		Receives a flag indicating whether the 3DMF data was
							in text form.
	@result		True if the reading was successful.
*/
bool Read3DMF( const char* inPath,
				std::vector<CQ3ObjectRef>& outObjects,
				bool& outText )
{
	bool didRead = false;
	outObjects.clear();
	outText = false;
	
	CQ3ObjectRef theStorage( Q3PathStorage_New( inPath ) );
	CQ3ObjectRef theFile( Q3File_New() );
	
	if ( theStorage.isvalid() and theFile.isvalid() )
	{
		Q3File_SetStorage( theFile.get(), theStorage.get() );
		
		TQ3FileMode         fileMode;
		if (Q3File_OpenRead( theFile.get(), &fileMode ) == kQ3Success)
		{
			outText = ((fileMode & kQ3FileModeText) != 0);
			
			while (Q3File_IsEndOfFile( theFile.get() ) == kQ3False)
			{
				CQ3ObjectRef theObject( Q3File_ReadObject( theFile.get() ) );
				if (theObject.isvalid())
				{
					outObjects.push_back( theObject );
					didRead = true;
				}
			}
			
			Q3File_Close( theFile.get() );
		}
		else
		{
			std::cerr << "Failed to open for reading.\n";
		}
	}
	
	return didRead;
}

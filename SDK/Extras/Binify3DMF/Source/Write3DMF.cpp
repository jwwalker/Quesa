/*
 *  Write3DMF.cpp
 *  Binify3DMF
 *
 *  Created by James Walker on 4/24/12.
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

#include "Write3DMF.h"

#include <QuesaIO.h>
#include <QuesaStorage.h>
#include <QuesaView.h>

/*!
	@function	Write3DMF
	
	@abstract	Write Quesa objects to a binary 3DMF stream.
	
	@param		inObjects		Objects to write.
	@param		inBinStream		A stream to write to.
	@result		Success or failure of the operation.
*/
bool	Write3DMF( const std::vector<CQ3ObjectRef>& inObjects,
					FILE* inBinStream )
{
	bool didWrite = false;
	
	// Set up a dummy pixmap on which to base a draw context.
	int width = 32;
	int height = 32;
	int rowBytes = width * 4;
	int dataSize = height * rowBytes;
	std::vector<char> dataBuf( dataSize );
	TQ3Pixmap pixmap =
	{
		&dataBuf[0],
		width,
		height,
		rowBytes,
		32,
		kQ3PixelTypeRGB32,
		kQ3EndianLittle,
		kQ3EndianLittle
	};
	
	// Create a view.
	CQ3ObjectRef theView( Q3View_NewWithDefaults( kQ3DrawContextTypePixmap,
		&pixmap ) );
	
	// Create a storage object and a file.
	CQ3ObjectRef theStorage( Q3FileStreamStorage_New( inBinStream ) );
	CQ3ObjectRef theFile( Q3File_New() );
	
	if ( theView.isvalid() and theStorage.isvalid() and theFile.isvalid() )
	{
		Q3File_SetStorage( theFile.get(), theStorage.get() );
		
		Q3File_OpenWrite( theFile.get(), kQ3FileModeNormal );
		
		if (kQ3Success == Q3View_StartWriting( theView.get(),theFile.get() ))
		{
			for (std::vector<CQ3ObjectRef>::const_iterator i = inObjects.begin();
				i != inObjects.end(); ++i)
			{
				if (kQ3Success == Q3Object_Submit( i->get(), theView.get() ))
				{
					didWrite = true;
				}
			}
				
			Q3View_EndWriting( theView.get() );
		}
		
		Q3File_Close( theFile.get() );
	}
	
	return didWrite;
}

/*  NAME:
        E3IOFileFormat.c

    DESCRIPTION:
        Implementation of Quesa Abstract FileFormat object.
        
    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3IO.h"
#include "E3IOFileFormat.h"
#include "E3FFormat_3DMF.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FileFormat_RegisterClasses : Register the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_RegisterClass()
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
											kQ3ObjectTypeFileFormat,
											kQ3ClassNameFileFormat,
											NULL,
											0);


	// Register the built in readers
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeFileFormat,
											kQ3FileFormatTypeReader,
											kQ3ClassNameFileFormatReader,
											NULL,
											0);



	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3FFormat_3DMF_Reader_RegisterClass();




	// Register the built in writers
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeFileFormat,
											kQ3FileFormatTypeWriter,
											kQ3ClassNameFileFormatWriter,
											NULL,
											0);

	return(qd3dStatus);
}





//=============================================================================
//      E3FileFormat_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_UnregisterClass()
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3FFormat_3DMF_Reader_UnregisterClass();
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3FFormatReaderType3DMFBin, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3FFormatReaderType3DMFBinSwapped, kQ3True);
	
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3FileFormatTypeReader, kQ3True);

	qd3dStatus = E3FFormat_3DMF_Writer_UnregisterClass();
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3FileFormatTypeWriter, kQ3True);

	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeFileFormat, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3FileFormat_Init : Initializes the format.
//-----------------------------------------------------------------------------
TQ3Status 
E3FileFormat_Init(TQ3FileFormatObject theFileFormat, TQ3StorageObject storage)
{
	TQ3FFormatBaseData		*instanceData = (TQ3FFormatBaseData *) theFileFormat->instanceData;
	E3ClassInfoPtr storageClass;
	E3ClassInfoPtr fformatClass;
	TQ3XStorageGetSizeMethod	 	getSize;
	
	fformatClass = theFileFormat->theClass;

	E3Shared_Replace(&instanceData->storage, storage);

	if( instanceData->storage != NULL)
	{
	storageClass = storage->theClass;
	
	instanceData->currentStoragePosition = 0;
	instanceData->readInGroup = kQ3True;
	
	getSize   = (TQ3XStorageGetSizeMethod)
					E3ClassTree_GetMethod(storageClass, kQ3XMethodTypeStorageGetSize);

	
	if(getSize(storage, &instanceData->logicalEOF) == kQ3Failure)
		return kQ3Failure;
	}
	
	return(kQ3Success);
}





//=============================================================================
//      E3File_GetFileFormat : Get the file format for a file.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3FileFormatObject 
E3File_GetFileFormat(TQ3FileObject theFile)
{
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;

	
	return(instanceData->format);
}




//=============================================================================
//      E3FileFormat_NewFromType : Create a new fileFormat object.
//-----------------------------------------------------------------------------
TQ3FileFormatObject
E3FileFormat_NewFromType(TQ3ObjectType fformatObjectType)
{
	TQ3Object		theObject;

	// Create the object
	theObject = E3ClassTree_CreateInstance(fformatObjectType, kQ3False, NULL);
	

	return(theObject);
}





//=============================================================================
//      E3FileFormat_GenericReadBinary_8 : Reads 8 bits from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_8(TQ3FileFormatObject format, TQ3Int8* data)
{
	return Q3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, 1);
}





//=============================================================================
//      E3FileFormat_GenericReadBinary_16 : Reads 16 bits from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_16(TQ3FileFormatObject format, TQ3Int16* data)
{
	return Q3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, 2);
}





//=============================================================================
//      E3FileFormat_GenericReadBinary_32 : Reads 32 bits from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_32(TQ3FileFormatObject format, TQ3Int32* data)
{
	return Q3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, 4);
}





//=============================================================================
//      E3FileFormat_GenericReadBinary_64 : Reads 64 bits from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_64(TQ3FileFormatObject format, TQ3Int64* data)
{
	return Q3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, 8);
}





//=============================================================================
//      E3FileFormat_GenericReadBinary_String : Reads a zero terminated padded
//												string from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_String(TQ3FileFormatObject format, char* data, unsigned long *length)
{
	TQ3Uns32 					sizeRead = 0;
	TQ3Status 					result = kQ3Failure;
	TQ3FFormatBaseData			*instanceData = (TQ3FFormatBaseData *) format->instanceData;
	TQ3XStorageReadDataMethod	dataRead;
	
	char* 						dataPtr = data;
	char 						lastChar;

	dataRead = (TQ3XStorageReadDataMethod)
					E3ClassTree_GetMethod(instanceData->storage->theClass, kQ3XMethodTypeStorageReadData);

	length = 0;
	if( dataRead != NULL)
	
		do{
		result = dataRead(instanceData->storage,
							instanceData->currentStoragePosition,
							1, (unsigned char *)&lastChar, &sizeRead);
							
		instanceData->currentStoragePosition++;
		length++;
		*dataPtr = lastChar;
		dataPtr++;
		
		} 
		while ((lastChar != 0) && (result == kQ3Success));
		
		length--;// don't count trailing zero


	return result;							 
}





//=============================================================================
//      E3FileFormat_GenericReadBinary_Raw : Reads length bytes from storage.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_Raw(TQ3FileFormatObject format, unsigned char* data, unsigned long length)
{
	TQ3Uns32 sizeRead = 0;
	TQ3Status result = kQ3Failure;
	TQ3FFormatBaseData		*instanceData = (TQ3FFormatBaseData *) format->instanceData;
	TQ3XStorageReadDataMethod	dataRead;

	dataRead = (TQ3XStorageReadDataMethod)
					E3ClassTree_GetMethod(instanceData->storage->theClass, kQ3XMethodTypeStorageReadData);

	if( dataRead != NULL)
		result = dataRead(instanceData->storage,
							instanceData->currentStoragePosition,
							length, data, &sizeRead);

	Q3_ASSERT(sizeRead == length);
	instanceData->currentStoragePosition += length;

	return result;							 
}





//=============================================================================
//      E3FileFormat_GenericReadBinSwap_16 : Reads 16 bits from storage,
//											 swapping the byte order.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinSwap_16(TQ3FileFormatObject format, TQ3Int16* data)
{
	TQ3Status result;
	result = Q3FileFormat_GenericReadBinary_16 (format, data);
	if(result == kQ3Success)
		*data = E3EndianSwap16(*data);
	
	return result;
}





//=============================================================================
//      E3FileFormat_GenericReadBinSwap_32 : Reads 32 bits from storage,
//											 swapping the byte order.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinSwap_32(TQ3FileFormatObject format, TQ3Int32* data)
{
	TQ3Status result;
	result = Q3FileFormat_GenericReadBinary_32 (format, data);
	if(result == kQ3Success)
		*data = E3EndianSwap32(*data);
	
	return result;
}





//=============================================================================
//      E3FileFormat_GenericReadBinSwap_64 : Reads 64 bits from storage,
//											 swapping the byte order.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinSwap_64(TQ3FileFormatObject format, TQ3Int64* data)
{
	TQ3Status result;
	result = Q3FileFormat_GenericReadBinary_64 (format, data);
	if(result == kQ3Success){
		TQ3Int64 temp;
		temp.lo = E3EndianSwap32(data->hi);
		temp.hi = E3EndianSwap32(data->lo);
		*data = temp;
		}
	
	return result;
}





//=============================================================================
//      E3FileFormat_GenericReadText_SkipBlanks : positions the currentPosition
//							pointing to the first non blank char.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadText_SkipBlanks(TQ3FileFormatObject format)
{
	char buffer;

	TQ3Uns32 sizeRead = 0;
	TQ3Status result = kQ3Failure;
	TQ3FFormatBaseData		*instanceData = (TQ3FFormatBaseData *) format->instanceData;
	TQ3XStorageReadDataMethod	dataRead;

	dataRead = (TQ3XStorageReadDataMethod)
					E3ClassTree_GetMethod(instanceData->storage->theClass, kQ3XMethodTypeStorageReadData);

	if( dataRead != NULL)
		do{
			result = dataRead(instanceData->storage,
							instanceData->currentStoragePosition,
							1, (TQ3Uns8 *)&buffer, &sizeRead);
			
			if( (buffer <= 0x20) ||
				(buffer == 0x7F) 
				)
				instanceData->currentStoragePosition++;
			else
				break;
			
			}
		while((result == kQ3Success) && (instanceData->currentStoragePosition <= instanceData->logicalEOF));


	return result;							 
}





//=============================================================================
//      E3FileFormat_GenericReadText_ReadUntilChars : reads a string until:
//						-one of the numChars first chars in *chars is reached.
//							in this case the found char index is returned in the foundChar parameter
//						-maxLen of chars has been read without founding any of the chars in *chars
//							in this case foundChar parameter returns -1
//					charsRead returns the number of chars read not counting the flag char if found
//					currentStoragePosition is set one past the flag char
//					if the blanks flag is set, all chars< 0x20 are considered as stop chars
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadText_ReadUntilChars(TQ3FileFormatObject format,char* buffer,
											char* chars, TQ3Uns32 numChars, TQ3Boolean blanks,
											TQ3Int32* foundChar,TQ3Uns32 maxLen,
											TQ3Uns32* charsRead)
{
	TQ3Boolean found;

	TQ3Uns32 sizeRead = 0;
	TQ3Uns32 index = 0;
	TQ3Uns32 i;
	TQ3Status result = kQ3Failure;
	TQ3FFormatBaseData		*instanceData = (TQ3FFormatBaseData *) format->instanceData;
	TQ3XStorageReadDataMethod	dataRead;

	if(foundChar)
		*foundChar = -1;

	dataRead = (TQ3XStorageReadDataMethod)
					E3ClassTree_GetMethod(instanceData->storage->theClass, kQ3XMethodTypeStorageReadData);

	if( dataRead != NULL)
			found = kQ3False;
			result = dataRead(instanceData->storage,
							instanceData->currentStoragePosition,
							maxLen, (TQ3Uns8*)buffer, &sizeRead); // read all the data at once
			
		while((result == kQ3Success)
				&& (instanceData->currentStoragePosition < instanceData->logicalEOF) 
				&& (index < sizeRead) 
				 ){
			instanceData->currentStoragePosition ++;
			
			for(i = 0; i< numChars; i++)
				if((chars[i] == buffer[index]) || ((blanks == kQ3True) && (buffer[index] <= 0x20))
					|| ((chars[i] == 0x0D) && (buffer[index] == 0x0A)))// unix file
					{
					if(foundChar)
						*foundChar = buffer[index];
					if((chars[i] == 0x0D) && (buffer[index+1] == 0x0A))// windows file
						instanceData->currentStoragePosition ++;
					buffer[index] = 0;
					found = kQ3True;
					break;
					}
			
			if(found == kQ3True)
				break;
			else
				{
				index++;
				}
			
			};

	if(charsRead)
		*charsRead = index;
	
	return result;							 
}




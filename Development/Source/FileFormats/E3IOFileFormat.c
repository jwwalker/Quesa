/*  NAME:
        E3IOFileFormat.c

    DESCRIPTION:
        Implementation of Quesa Abstract FileFormat object.
        
    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3IO.h"
#include "E3IOFileFormat.h"
#include "E3FFR_3DMF.h"
#include "E3View.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FileFormat_RegisterClasses : Register the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_RegisterClass()
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = Q3_REGISTER_CLASS_NO_DATA	(	kQ3ClassNameFileFormat,
										NULL,
										E3FileFormat ) ;


	// Register the built in readers
	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_NO_DATA	(	kQ3ClassNameFileFormatReader,
											NULL,
											E3FileFormatReader ) ;



	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3FFormat_3DMF_Reader_RegisterClass();




	// Register the built in writers
	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_NO_DATA	(	kQ3ClassNameFileFormatWriter,
											NULL,
											E3FileFormatWriter ) ;

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3FFW_3DMF_Register();

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
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatReaderType3DMFBin, kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatReaderType3DMFBinSwapped, kQ3True);
	
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3FileFormatTypeReader, kQ3True);

	qd3dStatus = E3FFW_3DMF_Unregister();
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3FileFormatTypeWriter, kQ3True);

	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeFileFormat, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3FileFormat_Init : Initializes the format.
//-----------------------------------------------------------------------------
TQ3Status 
E3FileFormat_Init(TQ3FileFormatObject theFileFormat, TQ3StorageObject storage)
{
	TQ3FFormatBaseData		*instanceData = (TQ3FFormatBaseData *) theFileFormat->FindLeafInstanceData () ;

	E3Shared_Replace(&instanceData->storage, storage);

	if( instanceData->storage != NULL)
	{
	
	instanceData->currentStoragePosition = 0;
	instanceData->readInGroup = kQ3True;

	
	if(Q3Storage_GetSize(storage, &instanceData->logicalEOF) == kQ3Failure)
		return kQ3Failure;
	}
	
	return(kQ3Success);
}





//=============================================================================
//      E3FileFormat_Terminate : Revert the job done by Init.
//-----------------------------------------------------------------------------
TQ3Status 
E3FileFormat_Terminate(TQ3FileFormatObject theFileFormat)
{
	TQ3FFormatBaseData		*instanceData = (TQ3FFormatBaseData *) theFileFormat->FindLeafInstanceData () ;

	E3Shared_Replace(&instanceData->storage, NULL);

	
	instanceData->currentStoragePosition = 0;
	instanceData->readInGroup = kQ3True;

	
	instanceData->logicalEOF= 0;
	
	return(kQ3Success);
}





//=============================================================================
//      E3FileFormat_NewFromType : Create a new fileFormat object.
//-----------------------------------------------------------------------------
TQ3FileFormatObject
E3FileFormat_NewFromType(TQ3ObjectType fformatObjectType)
{
	TQ3Object		theObject;

	// Create the object
	theObject = E3ClassTree::CreateInstance(fformatObjectType, kQ3False, NULL);
	

	return(theObject);
}





//=============================================================================
//      E3FileFormat_GenericReadBinary_8 : Reads 8 bits from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_8(TQ3FileFormatObject format, TQ3Int8* data)
{
	return E3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, 1);
}





//=============================================================================
//      E3FileFormat_GenericReadBinary_16 : Reads 16 bits from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_16(TQ3FileFormatObject format, TQ3Int16* data)
{
	return E3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, 2);
}





//=============================================================================
//      E3FileFormat_GenericReadBinary_32 : Reads 32 bits from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_32(TQ3FileFormatObject format, TQ3Int32* data)
{
	return E3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, 4);
}





//=============================================================================
//      E3FileFormat_GenericReadBinaryArray_8 : Reads array of 8 bit values from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinaryArray_8(TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int8* data)
{
	return E3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, numNums);
}





//=============================================================================
//      E3FileFormat_GenericReadBinaryArray_16 : Reads array of 16 bit values from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinaryArray_16(TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int16* data)
{
	return E3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, numNums * 2);
}





//=============================================================================
//      E3FileFormat_GenericReadBinaryArray_32 : Reads array of 32 bit values from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinaryArray_32(TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int32* data)
{
	return E3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, numNums * 4);
}





//=============================================================================
//      E3FileFormat_GenericReadBinary_64 : Reads 64 bits from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_64(TQ3FileFormatObject format, TQ3Int64* data)
{
	return E3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, 8);
}





//=============================================================================
//      E3FileFormat_GenericReadBinary_String : Reads a zero terminated
//												string from stream.
//-----------------------------------------------------------------------------
//		Note: see notes for E3FileFormat_GenericReadBinary_StringPadded
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_String(TQ3FileFormatObject format, char* data,
	TQ3Uns32 *ioLength)
{
	return E3FileFormat_GenericReadBinary_StringPadded(format, data, ioLength, kQ3False);
}







//=============================================================================
//      E3FileFormat_GenericReadBinary_StringPadded : Reads a zero terminated, possibly
//												padded string from stream.
//-----------------------------------------------------------------------------
//		Note: If the data parameter is NULL, this still finds the length of the
//		string, but leaves the file offset at the beginning of the string.
//		This makes it possible to read an unlimited-size string in a two-pass
//		manner: First find the length, then allocate a large-enough buffer,
//		then read the data.
//
//		On input, the value of *ioLength should be the number of bytes allocated
//		to receive data.  On output, *ioLength is the length of the string
//		(excluding the terminating NUL), even if not all of the string was able
//		to fit in the buffer.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_StringPadded(TQ3FileFormatObject format, char* data,
	TQ3Uns32 *ioLength, TQ3Boolean padTo4)
{
	TQ3Uns32 					sizeRead = 0;
	TQ3Status 					result = kQ3Failure;
	TQ3FFormatBaseData			*instanceData = (TQ3FFormatBaseData *) format->FindLeafInstanceData () ;
	TQ3Uns32					startOffset;
	TQ3Uns32					bufferSize = *ioLength;
	
	char* 						dataPtr = data;
	char 						lastChar;

	TQ3XStorageReadDataMethod dataRead = (TQ3XStorageReadDataMethod) instanceData->storage->GetMethod ( kQ3XMethodTypeStorageReadData ) ;

	*ioLength = 0;
	
	if ( dataRead != NULL)
	{
		startOffset = instanceData->currentStoragePosition;
		
		// Read bytes one at a time, until we fail to read or read a zero byte.
		do{
			result = dataRead( instanceData->storage,
								instanceData->currentStoragePosition,
								1, (TQ3Uns8 *)&lastChar, &sizeRead );
								
			instanceData->currentStoragePosition++;
			*ioLength += 1;
			
			if (data != NULL)
			{
				if (*ioLength < bufferSize)
				{
					*dataPtr = lastChar;
					dataPtr++;
				}
				else if (*ioLength == bufferSize)
				{
					*dataPtr = '\0';
				}
			}
		} 
		while ((lastChar != 0) && (result == kQ3Success));
		
		if (data == NULL)
		{
			// back to the beginning of the string
			instanceData->currentStoragePosition = startOffset;
		}
		else  if (padTo4 == kQ3True){
			// skip pad bytes
			instanceData->currentStoragePosition = startOffset +
				Q3Size_Pad( instanceData->currentStoragePosition - startOffset );
		}
		
		if (lastChar == 0)
			*ioLength -= 1;// don't count trailing zero
	}

	return result;							 
}






//=============================================================================
//      E3FileFormat_GenericReadBinary_Raw : Reads length bytes from storage.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinary_Raw(TQ3FileFormatObject format, unsigned char* data, TQ3Uns32 length)
{
	TQ3Uns32 sizeRead = 0;
	TQ3Status result = kQ3Failure;
	TQ3FFormatBaseData		*instanceData = (TQ3FFormatBaseData *) format->FindLeafInstanceData ();

	TQ3XStorageReadDataMethod dataRead = (TQ3XStorageReadDataMethod) instanceData->storage->GetMethod ( kQ3XMethodTypeStorageReadData ) ;

	if( dataRead != NULL)
		result = dataRead(instanceData->storage,
							instanceData->currentStoragePosition,
							length, (TQ3Uns8*)data, &sizeRead);

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
	result = E3FileFormat_GenericReadBinary_16 (format, data);
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
	result = E3FileFormat_GenericReadBinary_32 (format, data);
	if(result == kQ3Success)
		*data = E3EndianSwap32(*data);
	
	return result;
}





//=============================================================================
//      E3FileFormat_GenericReadBinSwapArray_16 : Reads array of 16 bit values from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinSwapArray_16(TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int16* data)
{
	TQ3Status result;
	TQ3Uns32	n;
	result = E3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, numNums * 2);
	
	if (result == kQ3Success)
	{
		for (n = 0; n < numNums; ++n)
		{
			data[n] = E3EndianSwap16( data[n] );
		}
	}
	return result;
}





//=============================================================================
//      E3FileFormat_GenericReadBinSwapArray_32 : Reads array of 32 bit values from stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericReadBinSwapArray_32(TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int32* data)
{
	TQ3Status result;
	TQ3Uns32	n;
	result = E3FileFormat_GenericReadBinary_Raw (format, (unsigned char*)data, numNums * 4);
	
	if (result == kQ3Success)
	{
		for (n = 0; n < numNums; ++n)
		{
			data[n] = E3EndianSwap32( data[n] );
		}
	}
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
	result = E3FileFormat_GenericReadBinary_64 (format, data);
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
{	TQ3FFormatBaseData			*instanceData = (TQ3FFormatBaseData *) format->FindLeafInstanceData ();
	TQ3Status					result        = kQ3Success;
	TQ3Uns32					sizeRead      = 0;
	char						buffer;



	// Get the read method
	TQ3XStorageReadDataMethod dataRead = (TQ3XStorageReadDataMethod) instanceData->storage->GetMethod ( kQ3XMethodTypeStorageReadData ) ;
	if (dataRead == NULL)
		return(kQ3Failure);



	// Skip until we find the end of the file or a non-blank character
	while (result == kQ3Success && instanceData->currentStoragePosition < instanceData->logicalEOF)
		{
		result = dataRead(instanceData->storage, instanceData->currentStoragePosition, 1, (TQ3Uns8 *)&buffer, &sizeRead);
		if (buffer <= 0x20 || buffer == 0x7F) 
			instanceData->currentStoragePosition++;
		else
			break;
		}

	return(result);							 
}





/*!
	@function	E3FileFormat_GenericReadText_ReadUntilChars
	
	@abstract	Read text from the file until we find one of a list of
				designated "stop" characters, or until we reach the end file.
				The current storage position is set one past the stop character.
	
	@param		format		File format object.
	@param		buffer		Buffer to receive text.  The stop character, if
							found, will be replaced by a NUL character.
	@param		chars		Characters to stop at.
	@param		numChars	Number of stop characters.
	@param		blanks		If true, stop on all control characters (<= 0x20).
	@param		foundChar	Receives stop character that we found, or -1 if we
							filled the buffer or reached the end of file.  You
							may pass NULL if you do not need this information.
	@param		maxLen		Maximum number of characters to read.
	@param		charsRead	Receives number of characters read, not including
							the stop character if any.  You may pass NULL if you
							do not need this information.

	@result		kQ3Success unless we failed to read from the file.
*/
TQ3Status
E3FileFormat_GenericReadText_ReadUntilChars(TQ3FileFormatObject format, char* buffer,
											const char* chars, TQ3Uns32 numChars, TQ3Boolean blanks,
											TQ3Int32* foundChar,TQ3Uns32 maxLen,
											TQ3Uns32* charsRead)
{
	TQ3Boolean found;

	TQ3Uns32 sizeRead = 0;
	TQ3Uns32 index = 0;
	TQ3Uns32 i;
	TQ3Status result = kQ3Failure;
	TQ3FFormatBaseData		*instanceData = (TQ3FFormatBaseData *) format->FindLeafInstanceData ();

	if(foundChar)
		*foundChar = -1;

	TQ3XStorageReadDataMethod dataRead = (TQ3XStorageReadDataMethod) instanceData->storage->GetMethod ( kQ3XMethodTypeStorageReadData ) ;
	
	// The read method may post an error if we try to read beyond the end of file
	maxLen = E3Num_Min( maxLen, instanceData->logicalEOF - instanceData->currentStoragePosition );

	if( (dataRead != NULL) && (maxLen > 0) )
		{
		found = kQ3False;
		result = dataRead(instanceData->storage,
						instanceData->currentStoragePosition,
						maxLen, (TQ3Uns8*)buffer, &sizeRead); // read all the data at once
			
		while((result == kQ3Success)
				&& (instanceData->currentStoragePosition < instanceData->logicalEOF) 
				&& (index < sizeRead) 
				 )
			{
			instanceData->currentStoragePosition ++;
			
			if ((blanks == kQ3True) && (((TQ3Uns8)buffer[index]) <= 0x20))
				{
				if (foundChar)
					*foundChar = ((TQ3Uns8)buffer[index]);
				buffer[index] = 0;
				found = kQ3True;
				break;
				}
			
			for(i = 0; i< numChars; i++)
				if((chars[i] == buffer[index]) ||
					((chars[i] == 0x0D) && (buffer[index] == 0x0A)))// unix file
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
		}

	if(charsRead)
		*charsRead = index;
	
	return result;							 
}



//=============================================================================
//      E3FileFormat_GenericWriteBinary_8 : Writes 8 bits to a stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericWriteBinary_8(TQ3FileFormatObject format, const TQ3Int8 *data)
{
	return E3FileFormat_GenericWriteBinary_Raw (format, (const unsigned char*)data, 1);
}





//=============================================================================
//      E3FileFormat_GenericWriteBinary_16 : Writes 16 bits to a stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericWriteBinary_16(TQ3FileFormatObject format, const TQ3Int16 *data)
{
	return E3FileFormat_GenericWriteBinary_Raw (format, (const unsigned char*)data, 2);
}





//=============================================================================
//      E3FileFormat_GenericWriteBinary_32 : Writes 32 bits to a stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericWriteBinary_32(TQ3FileFormatObject format, const TQ3Int32* data)
{
	return E3FileFormat_GenericWriteBinary_Raw (format, (const unsigned char*)data, 4);
}





//=============================================================================
//      E3FileFormat_GenericWriteBinary_64 : Writes 64 bits to a stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericWriteBinary_64(TQ3FileFormatObject format, const TQ3Int64 *data)
{
	return E3FileFormat_GenericWriteBinary_Raw (format, (const unsigned char*)data, 8);
}





//=============================================================================
//      E3FileFormat_GenericWriteBinary_String : Writes a zero terminated padded
//												string to a stream.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericWriteBinary_String(TQ3FileFormatObject format, const char* data, TQ3Uns32 *length)
{
	TQ3Status	result;
	TQ3Size		theLength, paddedLength;
	

	theLength = strlen( data ) + 1;	// 1 for the trailing NUL byte
	paddedLength = Q3Size_Pad( theLength );
	
	
	result = E3FileFormat_GenericWriteBinary_Raw( format, (const unsigned char *)data,
		theLength );
	
	
	if ( (result == kQ3Success) && (paddedLength > theLength) )
	{
		// There are at most 3 pad bytes added, since Q3Size_Pad aligns to
		// longword sizes.
		TQ3Uns32	pad = 0;
		result = E3FileFormat_GenericWriteBinary_Raw( format, (const unsigned char *)&pad,
			paddedLength - theLength );
	}
	
	return (result);
}





//=============================================================================
//      E3FileFormat_GenericWriteBinary_Raw : Writes length bytes to a storage.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericWriteBinary_Raw(TQ3FileFormatObject format,const unsigned char* data, TQ3Uns32 length)
{
	TQ3Uns32 sizeWrite = 0;
	TQ3Status result = kQ3Failure;
	TQ3FFormatBaseData		*instanceData = (TQ3FFormatBaseData *) format->FindLeafInstanceData ();

	TQ3XStorageWriteDataMethod dataWrite = (TQ3XStorageWriteDataMethod) instanceData->storage->GetMethod ( kQ3XMethodTypeStorageWriteData ) ;

	if( dataWrite != NULL)
		result = dataWrite(instanceData->storage,
							instanceData->currentStoragePosition,
							length, (TQ3Uns8*)data, &sizeWrite);

	Q3_ASSERT(sizeWrite == length);
	instanceData->currentStoragePosition += length;

	return result;							 
}





//=============================================================================
//      E3FileFormat_GenericWriteBinSwap_16 : Writes 16 bits to a storage,
//											 swapping the byte order.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericWriteBinSwap_16(TQ3FileFormatObject format, const TQ3Int16 *data)
{
	TQ3Status result;
	TQ3Int16 swappedData = E3EndianSwap16(*data);
	result = E3FileFormat_GenericWriteBinary_16 (format, &swappedData);
	
	return result;
}





//=============================================================================
//      E3FileFormat_GenericWriteBinSwap_32 : Writes 32 bits to a storage,
//											 swapping the byte order.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericWriteBinSwap_32(TQ3FileFormatObject format, const TQ3Int32 *data)
{
	TQ3Status result;
	TQ3Int32 swappedData = E3EndianSwap32(*data);
	result = E3FileFormat_GenericWriteBinary_32 (format, &swappedData);
	
	return result;
}





//=============================================================================
//      E3FileFormat_GenericWriteBinSwap_64 : Writes 64 bits to a storage,
//											 swapping the byte order.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GenericWriteBinSwap_64(TQ3FileFormatObject format, const TQ3Int64 *data)
{
	TQ3Status result;
	TQ3Int64 temp;
	temp.lo = E3EndianSwap32(data->hi);
	temp.hi = E3EndianSwap32(data->lo);
	
	result = E3FileFormat_GenericWriteBinary_64 (format, &temp);
	
	return result;
}



//=============================================================================
//      E3FileFormat_GetType : Return the type of a fileFormat.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3FileFormat_GetType(TQ3FileFormatObject theFormat)
	{
	// Return the type
	return theFormat->GetObjectType ( kQ3ObjectTypeFileFormat ) ;
	}




//=============================================================================
//      E3FileFormat_HasModalConfigure : Does a fileFormat have a configure dialog?
//-----------------------------------------------------------------------------
TQ3Boolean
E3FileFormat_HasModalConfigure(TQ3FileFormatObject theFormat)
	{
	// Return as the method is defined or not
	return (TQ3Boolean) ( theFormat->GetMethod ( kQ3XMethodTypeRendererModalConfigure) != NULL ) ;
	}





//=============================================================================
//      E3FileFormat_ModalConfigure : Invoke a fileFormat's modal configure dialog.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_ModalConfigure(TQ3FileFormatObject theFormat, TQ3DialogAnchor dialogAnchor, TQ3Boolean *canceled)
	{		
	// Find the method
	TQ3XRendererModalConfigureMethod modalConfigure = (TQ3XRendererModalConfigureMethod) theFormat->GetMethod ( kQ3XMethodTypeRendererModalConfigure ) ;
	if ( modalConfigure == NULL )
		return kQ3Failure ;



	// Call the method
	return modalConfigure ( theFormat, dialogAnchor, canceled, theFormat->FindLeafInstanceData () ) ;
	}





//=============================================================================
//      E3FileFormatClass_GetFormatNameString : Get the fileFormat nick name.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormatClass_GetFormatNameString(TQ3ObjectType fileFormatClassType, TQ3ObjectClassNameString fileFormatClassString)
	{		
	// Initialise a return value
	fileFormatClassString[0] = 0x00 ;



	// Find the fileFormat class, and get the method
	E3ClassInfoPtr fileFormatClass = E3ClassTree::GetClass ( fileFormatClassType ) ;
	if ( fileFormatClass == NULL )
		return kQ3Failure ;

	TQ3XRendererGetNickNameStringMethod nickNameMethod = (TQ3XRendererGetNickNameStringMethod)
							fileFormatClass->GetMethod ( kQ3XMethodTypeRendererGetNickNameString);
	if ( nickNameMethod == NULL )
		return kQ3Failure ;



	TQ3Uns32 numWritten ;
	// Call the method
	return nickNameMethod ( (unsigned char *) fileFormatClassString,
									sizeof ( TQ3ObjectClassNameString ),
									&numWritten ) ;
	}





//=============================================================================
//      E3FileFormat_GetConfigurationData : Get a fileFormat's config data.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_GetConfigurationData(TQ3FileFormatObject theFormat, unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
	{
	// Initialise a return value
	*actualDataSize = 0;



	// Find the method
	TQ3XRendererGetConfigurationDataMethod getConfigData = (TQ3XRendererGetConfigurationDataMethod) theFormat->GetMethod ( kQ3XMethodTypeRendererGetConfigurationData ) ;
	if ( getConfigData == NULL )
		return kQ3Failure ;



	// Call the method
	return getConfigData ( theFormat,
								dataBuffer,
								bufferSize,
								actualDataSize,
								theFormat->FindLeafInstanceData () ) ;

	}





//=============================================================================
//      E3FileFormat_SetConfigurationData : Set a fileFormat's config data.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_SetConfigurationData(TQ3FileFormatObject theFormat, unsigned char *dataBuffer, TQ3Uns32 bufferSize)
	{
	// Find the method
	TQ3XRendererSetConfigurationDataMethod setConfigData = (TQ3XRendererSetConfigurationDataMethod) theFormat->GetMethod ( kQ3XMethodTypeRendererSetConfigurationData ) ;
	if ( setConfigData == NULL )
		return kQ3Failure ;



	// Call the method
	TQ3Status qd3dStatus = setConfigData ( theFormat,
								dataBuffer,
								bufferSize,
								theFormat->FindLeafInstanceData ());

	Q3Shared_Edited ( theFormat ) ;

	return qd3dStatus ;
	}



//=============================================================================
//      E3FileFormat_Method_StartFile : Call the start file method.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3FileFormat_Method_StartFile(TQ3ViewObject theView)
	{
	TQ3FileFormatObject	theFormat = E3View_AccessFileFormat ( theView ) ;



	// No-op if no format set
	if ( theFormat == NULL )
		return kQ3Success ;



	// Find the method
	TQ3XRendererStartFrameMethod startFile = (TQ3XRendererStartFrameMethod) theFormat->GetMethod ( kQ3XMethodTypeRendererStartFrame ) ;
	if ( startFile == NULL )
		return kQ3Success ;



	// Call the method
	return startFile ( theView, theFormat->FindLeafInstanceData (), NULL ) ;
	}


//=============================================================================
//      E3FileFormat_Method_EndFile : Call the end file method.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3FileFormat_Method_EndFile(TQ3ViewObject theView)
	{
	TQ3FileFormatObject theFormat = E3View_AccessFileFormat ( theView ) ;



	// No-op if no format set
	if ( theFormat == NULL )
		return kQ3Success ;



	// Find the method
	TQ3XRendererStartFrameMethod endFrame = (TQ3XRendererEndFrameMethod) theFormat->GetMethod ( kQ3XMethodTypeRendererEndFrame ) ;
	if ( endFrame == NULL ) 
		return kQ3Success ;



	// Call the method
	return endFrame ( theView, theFormat->FindLeafInstanceData (), NULL ) ;
	}





//=============================================================================
//      E3FileFormat_Method_StartPass : Call the start pass method.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_Method_StartPass(TQ3ViewObject theView)
	{
	TQ3FileFormatObject theFormat = E3View_AccessFileFormat ( theView ) ;



	// No-op if no file FormatSet set
	if ( theFormat == NULL )
		return kQ3Success ;



	// Find the method
	TQ3XRendererStartPassMethod startPass = (TQ3XRendererStartPassMethod) theFormat->GetMethod ( kQ3XMethodTypeRendererStartPass ) ;
	if ( startPass == NULL )
		return kQ3Success ;



	// Call the method
	return startPass ( theView, theFormat->FindLeafInstanceData (), NULL, NULL ) ;
	}





//=============================================================================
//      E3FileFormat_Method_EndPass : Call the end pass method.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3FileFormat_Method_EndPass(TQ3ViewObject theView)
	{
	TQ3FileFormatObject theFormat = E3View_AccessFileFormat ( theView ) ;



	// No-op if no format set
	if ( theFormat == NULL )
		return kQ3ViewStatusDone ;



	// Find the method
	TQ3XRendererEndPassMethod endPass = (TQ3XRendererEndPassMethod) theFormat->GetMethod ( kQ3XMethodTypeRendererEndPass ) ;
	if ( endPass == NULL )
		return kQ3ViewStatusDone ;



	// Call the method
	return endPass ( theView, theFormat->FindLeafInstanceData () ) ;
	}



//=============================================================================
//      E3FileFormatr_Method_SubmitObject : Submit an Object to a writer.
//-----------------------------------------------------------------------------
//		This is the default entry point to a FileFormat.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_Method_SubmitObject(TQ3ViewObject	theView,
								 TQ3Object		object,
								 TQ3ObjectType	objectType,
								 const void			*objectData)
								 
	{
	TQ3FileFormatObject theFormat = E3View_AccessFileFormat ( theView ) ;



	// No-op if no formatter set or object not supported
	if ( theFormat == NULL )
		return kQ3Success ;


	
	// Find the method 
	TQ3XFileFormatSubmitObjectMethod submitObject = (TQ3XFileFormatSubmitObjectMethod) theFormat->GetMethod ( kQ3XMethodTypeFFormatSubmitObject ) ;


	// Call the method
	if ( submitObject != NULL )
		return submitObject ( theView, theFormat->FindLeafInstanceData (), object, objectType, objectData ) ;

	return kQ3Success ;
	}

//=============================================================================
//      E3FileFormatr_Method_SubmitGeometry : Submit a geometry to a writer.
//-----------------------------------------------------------------------------
//		Note :	We update geomSupported to indicate if the writer could accept
//				the geometry or not.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_Method_SubmitGeometry(TQ3ViewObject		theView,
								 TQ3ObjectType		geomType,
								 TQ3Boolean			*geomSupported,
								 TQ3GeometryObject	theGeom,
								 const void			*geomData)
	{
	TQ3FileFormatObject	 theFormat = E3View_AccessFileFormat ( theView ) ;



	// No-op if no formatter set
	if ( theFormat == NULL )
		return kQ3Success ;



	// Find the method
	TQ3XRendererSubmitGeometryMethod submitGeom = (TQ3XRendererSubmitGeometryMethod) theFormat->GetMethod ( geomType ) ;



	// Indicate if the geometry is supported or not
	*geomSupported = (TQ3Boolean) ( submitGeom != NULL ) ;



	// Call the method
	if ( submitGeom != NULL )
		return submitGeom ( theView, theFormat->FindLeafInstanceData (), theGeom, geomData ) ;

	return kQ3Failure ;
	}

//=============================================================================
//      E3FileFormatr_Method_SubmitGroup : Submit a group to a writer.
//-----------------------------------------------------------------------------
//		Note :	if there's no TQ3XFileFormatSubmitGroupMethod for the format,
//              defaults to submit the contents.
//-----------------------------------------------------------------------------
TQ3Status
E3FileFormat_Method_SubmitGroup(TQ3ViewObject	theView,
								TQ3Object		group,
								TQ3ObjectType	groupType,
								const void		*groupData)
								
	{
	TQ3GroupPosition					position ;
	TQ3Object							subObject ;
	TQ3FileFormatObject theFormat = E3View_AccessFileFormat ( theView ) ;


	// No-op if no formatter set
	if ( theFormat == NULL )
		return kQ3Success ;



	// Find the method
	TQ3XFileFormatSubmitObjectMethod submitGroup = (TQ3XFileFormatSubmitObjectMethod) theFormat->GetMethod ( kQ3XMethodTypeFFormatSubmitGroup ) ;





	// Call the method
	if ( submitGroup != NULL )
		return submitGroup ( theView, theFormat->FindLeafInstanceData (), group, groupType , groupData ) ;
	
	// submit the group contents
	TQ3Status qd3dStatus = kQ3Success ;
	for ( Q3Group_GetFirstPosition ( group, &position ) ;
		position != NULL ;
		Q3Group_GetNextPosition ( group, &position ) )
		{
		qd3dStatus = Q3Group_GetPositionObject ( group, position, &subObject ) ;
		if ( qd3dStatus != kQ3Success )
			break ;
		qd3dStatus = Q3Object_Submit ( subObject, theView ) ;
		
		Q3Object_Dispose ( subObject ) ;
		
		if ( qd3dStatus != kQ3Success )
			break ;
		}

	return qd3dStatus ;
	}


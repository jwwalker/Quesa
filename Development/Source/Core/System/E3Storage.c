/*  NAME:
        E3Storage.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#include "E3Storage.h"

#include <stdio.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kE3MemoryStorageDefaultGrowSize					1024
#define kE3MemoryStorageMinimumGrowSize					32





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Memory storage
typedef struct TE3_MemoryStorageData {
	TQ3Uns8			*buffer;
	TQ3Boolean		ownBuffer;
	TQ3Uns32		bufferSize;
	TQ3Uns32		validSize;
	TQ3Uns32		growSize;
} TE3_MemoryStorageData;


// Path storage
typedef struct TQ3PathStorageData {
	char		*thePath;
	FILE		*theFile;
} TQ3PathStorageData;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3storage_memory_read : Read data from the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_memory_read(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead)
{	TE3_MemoryStorageData	*instanceData  = (TE3_MemoryStorageData *) storage->instanceData;
	TQ3Uns32 		bytesToRead = dataSize;
	
	*sizeRead = 0;
	
	if(offset >= instanceData->validSize)
		return(kQ3Failure);
		
	if(offset + bytesToRead > instanceData->validSize)
		bytesToRead = instanceData->validSize - offset;
		
	memcpy(data,&instanceData->buffer[offset],bytesToRead);
	
	*sizeRead = bytesToRead;
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_memory_write : Write data to the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_memory_write(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten)
{	TE3_MemoryStorageData	*instanceData = (TE3_MemoryStorageData *) storage->instanceData;
	TQ3Uns32 				bytesToWrite;



	// Initialise a return value
	*sizeWritten = 0;



	// Make sure we have enough space
	if (offset  >= instanceData->validSize)
		return(kQ3Failure);



	// Work out how much we should copy
	bytesToWrite = dataSize;
	if (offset + bytesToWrite > instanceData->validSize)
		bytesToWrite = instanceData->validSize - offset;



	// Copy the block
	memcpy(&instanceData->buffer[offset], data, bytesToWrite);
	
	*sizeWritten = bytesToWrite;
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_memory_getsize : Get the size of the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_memory_getsize(TQ3StorageObject storage, TQ3Uns32 *size)
{	TE3_MemoryStorageData	*instanceData  = (TE3_MemoryStorageData *) storage->instanceData;

	*size = instanceData->validSize;
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_memory_new : New method.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_memory_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TE3_MemoryStorageData	*instanceData  = (TE3_MemoryStorageData *) privateData;
	TQ3Uns8					*passedBuffer;
	
	*instanceData = *((const TE3_MemoryStorageData *)paramData);

	if(instanceData->ownBuffer == kQ3True){
		// called from _New
		if ( instanceData->buffer != NULL ){
			// copy the buffer
			Q3_ASSERT(instanceData->bufferSize != 0);
			passedBuffer = instanceData->buffer;
			instanceData->buffer = (TQ3Uns8*)Q3Memory_Allocate( instanceData->bufferSize ) ;

			if (instanceData->buffer == NULL){
				return(kQ3Failure);						
				}
			memcpy( instanceData->buffer, passedBuffer, instanceData->bufferSize );
			instanceData->validSize = instanceData->bufferSize;
			instanceData->growSize = kE3MemoryStorageDefaultGrowSize;
			}
		else{
			// called with buffer == NULL, allocate our own
			// check validSize parameter
			instanceData->bufferSize = 0L;
			if(instanceData->validSize < kE3MemoryStorageMinimumGrowSize){
				instanceData->validSize = 0L;
				instanceData->growSize = kE3MemoryStorageDefaultGrowSize;
				}
			else{
				instanceData->growSize = instanceData->validSize;
				instanceData->validSize = 0L;
				}
				
			instanceData->buffer = (TQ3Uns8*)Q3Memory_Allocate( instanceData->growSize ) ;

			if (instanceData->buffer == NULL){
				return(kQ3Failure);						
				}
			instanceData->bufferSize = instanceData->growSize;
			instanceData->validSize = instanceData->growSize;
			}
		}
	else{
		// called from _NewBuffer
		if ( instanceData->buffer == NULL ){
			// allocate our own
			// check validSize parameter
			instanceData->ownBuffer = kQ3True;
			instanceData->bufferSize = 0L;
			if(instanceData->validSize < kE3MemoryStorageMinimumGrowSize){
				instanceData->validSize = 0L;
				instanceData->growSize = kE3MemoryStorageDefaultGrowSize;
				}
			else{
				instanceData->growSize = instanceData->validSize;
				instanceData->validSize = 0L;
				}
				
			instanceData->buffer = (TQ3Uns8*)Q3Memory_Allocate( instanceData->growSize ) ;

			if (instanceData->buffer == NULL){
				return(kQ3Failure);						
				}
			instanceData->bufferSize = instanceData->growSize;
			instanceData->validSize = instanceData->growSize;
			}
		}	

	return(kQ3Success);
}





//=============================================================================
//      e3storage_memory_delete : Delete method.
//-----------------------------------------------------------------------------
static void
e3storage_memory_delete(TQ3Object storage, void *privateData)
{	TE3_MemoryStorageData	*instanceData  = (TE3_MemoryStorageData *) privateData;
#pragma unused(storage)



	// Dispose of our instance data
	if (instanceData->ownBuffer == kQ3True)
		Q3Memory_Free(&instanceData->buffer);
}





//=============================================================================
//      e3storage_memory_metahandler : Storage metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3storage_memory_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3storage_memory_delete;
			break;

		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3storage_memory_new;
			break;

		case kQ3XMethodTypeStorageGetSize:
			theMethod = (TQ3XFunctionPointer) e3storage_memory_getsize;
			break;

		case kQ3XMethodTypeStorageReadData:
			theMethod = (TQ3XFunctionPointer) e3storage_memory_read;
			break;

		case kQ3XMethodTypeStorageWriteData:
			theMethod = (TQ3XFunctionPointer) e3storage_memory_write;
			break;

		}
	
	return(theMethod);
}





//=============================================================================
//      e3storage_path_new : Path storage new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_path_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PathStorageData		*instanceData = (TQ3PathStorageData *) privateData;
	const char				*thePath      = (const char *) paramData;
	TQ3Uns32				pathLen;



	// Initialise our instance data
	pathLen = strlen(thePath);
	instanceData->thePath = (char *) Q3Memory_Allocate(pathLen + 1);
	if (instanceData->thePath == NULL)
		return(kQ3Failure);

	strcpy(instanceData->thePath, thePath);
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_path_delete : Path storage delete method.
//-----------------------------------------------------------------------------
static void
e3storage_path_delete(TQ3Object storage, void *privateData)
{	TQ3PathStorageData		*instanceData = (TQ3PathStorageData *) privateData;
#pragma unused(storage)



	// Make sure the file isn't open
	if (instanceData->theFile != NULL)
		E3ErrorManager_PostError(kQ3ErrorFileIsOpen, kQ3False);



	// Dispose of our instance data
	if (instanceData->thePath != NULL)
		Q3Memory_Free(&instanceData->thePath);
}





//=============================================================================
//      e3storage_path_open : Open the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_path_open(TQ3StorageObject storage, TQ3Boolean forWriting)
{	TQ3PathStorageData		*instanceData = (TQ3PathStorageData *) storage->instanceData;



	// Make sure the file isn't already open
	if (instanceData->theFile != NULL)
		{
		E3ErrorManager_PostError(kQ3ErrorFileAlreadyOpen, kQ3False);
		return(kQ3Failure);
		}



	// Open the file		
	instanceData->theFile = fopen(instanceData->thePath, forWriting ? "wb+" : "rb");
	if (instanceData->theFile == NULL)
		return(kQ3Failure);
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_path_close : Close the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_path_close(TQ3StorageObject storage)
{	TQ3PathStorageData		*instanceData = (TQ3PathStorageData *) storage->instanceData;



	// Make sure the file is open
	if (instanceData->theFile == NULL)
		{
		E3ErrorManager_PostError(kQ3ErrorFileNotOpen, kQ3False);
		return(kQ3Failure);
		}



	// Close the file		
	fclose(instanceData->theFile);
	instanceData->theFile = NULL;

	return(kQ3Success);
}





//=============================================================================
//      e3storage_path_getsize : Get the size of the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_path_getsize(TQ3StorageObject storage, TQ3Uns32 *size)
{	TQ3PathStorageData		*instanceData = (TQ3PathStorageData *) storage->instanceData;
	fpos_t					oldPos;



	// Make sure the file is open
	if (instanceData->theFile == NULL)
		{
		E3ErrorManager_PostError(kQ3ErrorFileNotOpen, kQ3False);
		return(kQ3Failure);
		}



	// Get the current position in the file
	if (fgetpos(instanceData->theFile, &oldPos))
		return(kQ3Failure);



	// Seek to the end and get the position there. Not that using ftell rather
	// than fgetpos limits us to 2147483647 byte files, but casting an fpos_t
	// to a 32-bit integer is not valid on some Unix systems.
	if (fseek(instanceData->theFile, 0, SEEK_END))
		return(kQ3Failure);

	*size = (TQ3Uns32) ftell(instanceData->theFile);

	if (fseek(instanceData->theFile, 0, SEEK_SET))
		return(kQ3Failure);



	// Restore the previous position in the file
	if (fsetpos(instanceData->theFile, &oldPos))
		return(kQ3Failure);

	return(kQ3Success);
}





//=============================================================================
//      e3storage_path_read : Read data from the storage object.
//-----------------------------------------------------------------------------
//		Note : Currently unbuffered - may cause performance problems.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_path_read(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead)
{	TQ3PathStorageData		*instanceData = (TQ3PathStorageData *) storage->instanceData;



	// Make sure the file is open
	if (instanceData->theFile == NULL)
		{
		E3ErrorManager_PostError(kQ3ErrorFileNotOpen, kQ3False);
		return(kQ3Failure);
		}



	// Seek to the offset, and read the data
	// (The ftell is needed because in the Windows version of
	// CodeWarrior's standard library, fseek always flushes the buffer.)
	if (offset != ftell( instanceData->theFile ))
		{
		if (fseek(instanceData->theFile, offset, SEEK_SET))
			return(kQ3Failure);
		}

	*sizeRead = fread(data, 1, dataSize, instanceData->theFile);

	return(kQ3Success);
}





//=============================================================================
//      e3storage_path_write : Write data to the storage object.
//-----------------------------------------------------------------------------
//		Note : Currently unbuffered - may cause performance problems.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_path_write(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten)
{	TQ3PathStorageData		*instanceData = (TQ3PathStorageData *) storage->instanceData;



	// Make sure the file is open
	if (instanceData->theFile == NULL)
		{
		E3ErrorManager_PostError(kQ3ErrorFileNotOpen, kQ3False);
		return(kQ3Failure);
		}



	// Seek to the offset, and write the data
	if (fseek(instanceData->theFile, offset, SEEK_SET))
		return(kQ3Failure);

	*sizeWritten = fwrite(data, 1, dataSize, instanceData->theFile);

	return(kQ3Success);
}





//=============================================================================
//      e3storage_path_metahandler : Path storage metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3storage_path_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3storage_path_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3storage_path_delete;
			break;

		case kQ3XMethodTypeStorageOpen:
			theMethod = (TQ3XFunctionPointer) e3storage_path_open;
			break;

		case kQ3XMethodTypeStorageClose:
			theMethod = (TQ3XFunctionPointer) e3storage_path_close;
			break;

		case kQ3XMethodTypeStorageGetSize:
			theMethod = (TQ3XFunctionPointer) e3storage_path_getsize;
			break;

		case kQ3XMethodTypeStorageReadData:
			theMethod = (TQ3XFunctionPointer) e3storage_path_read;
			break;

		case kQ3XMethodTypeStorageWriteData:
			theMethod = (TQ3XFunctionPointer) e3storage_path_write;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Storage_RegisterClass : Register the classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Storage_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the storage classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
											kQ3SharedTypeStorage,
											kQ3ClassNameStorage,
											NULL,
											0);


	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeStorage,
												kQ3StorageTypeMemory,
												kQ3ClassNameStorageMemory,
												e3storage_memory_metahandler,
												sizeof(TE3_MemoryStorageData));


	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeStorage,
												kQ3StorageTypePath,
												kQ3ClassNameStoragePath,
												e3storage_path_metahandler,
												sizeof(TQ3PathStorageData));



	// Register the platform specific classes
	if (qd3dStatus == kQ3Success)
		{
#if QUESA_OS_MACINTOSH
		qd3dStatus = E3MacStorage_RegisterClass();
#elif QUESA_OS_UNIX
		qd3dStatus = E3UnixStorage_RegisterClass();
#elif QUESA_OS_WIN32
		qd3dStatus = E3Win32Storage_RegisterClass();
#elif QUESA_OS_BE
		qd3dStatus = E3BeStorage_RegisterClass();
#endif
		}

	return(qd3dStatus);
}





//=============================================================================
//      E3Storage_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Storage_UnregisterClass(void)
{	TQ3Status		qd3dStatus;


	qd3dStatus = E3ClassTree_UnregisterClass(kQ3SharedTypeStorage, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StorageTypeMemory, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StorageTypePath,   kQ3True);


#if QUESA_OS_MACINTOSH
	qd3dStatus = E3MacStorage_UnregisterClass();
#elif QUESA_OS_UNIX
	qd3dStatus = E3UnixStorage_UnregisterClass();
#elif QUESA_OS_WIN32
	qd3dStatus = E3Win32Storage_UnregisterClass();
#elif QUESA_OS_BE
	qd3dStatus = E3BeStorage_UnregisterClass();
#endif
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Storage_GetType : Return the type of a storage object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Storage_GetType(TQ3StorageObject storage)
{


	// Return the type
	return(E3ClassTree_GetObjectType(storage, kQ3SharedTypeStorage));
}





//=============================================================================
//      E3Storage_GetSize : Return the size of data in a storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3Storage_GetSize(TQ3StorageObject storage, TQ3Uns32 *size)
{
	TQ3Status result = kQ3Failure;
	// get the subclass method;
	TQ3XStorageGetSizeMethod getEOF_Method;
	getEOF_Method = (TQ3XStorageGetSizeMethod) E3ClassTree_GetMethod(storage->theClass,
														  kQ3XMethodTypeStorageGetSize);
	if (getEOF_Method != NULL)
		result = getEOF_Method(storage, size);
	
	return (result);
}





//=============================================================================
//      E3Storage_GetData : Return the data in a storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3Storage_GetData(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead)
{
	TQ3Status result = kQ3Failure;
	// get the subclass method;
	TQ3XStorageReadDataMethod getData_Method;
	getData_Method = (TQ3XStorageReadDataMethod) E3ClassTree_GetMethod(storage->theClass,
														  kQ3XMethodTypeStorageReadData);
	if (getData_Method != NULL)
		result = getData_Method(storage, offset, dataSize, data, sizeRead);
	
	return (result);
}





//=============================================================================
//      E3Storage_SetData : Set the data for a storage object.
//-----------------------------------------------------------------------------
//		Note : Under normal conditions SetData always returns dataSize ==
//			   sizeWritten, the storage grows to accept new data
//-----------------------------------------------------------------------------
TQ3Status
E3Storage_SetData(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten)
{
	
	TQ3Status result = kQ3Failure;
	// get the subclass method;
	TQ3XStorageWriteDataMethod setData_Method;
	setData_Method = (TQ3XStorageWriteDataMethod) E3ClassTree_GetMethod(storage->theClass,
														  kQ3XMethodTypeStorageWriteData);
	if (setData_Method != NULL)
		result = setData_Method(storage, offset, dataSize, data, sizeWritten);

	Q3Shared_Edited(storage);
	
	return (result);
}





//=============================================================================
//      E3MemoryStorage_GetType : Return the type of a memory storage object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
E3MemoryStorage_GetType(TQ3StorageObject storage)
{


	// Return the type
	return(E3ClassTree_GetObjectType(storage, kQ3StorageTypeMemory));
}





//=============================================================================
//      E3MemoryStorage_New : Creates a new storage copying the buffer.
//-----------------------------------------------------------------------------
TQ3StorageObject
E3MemoryStorage_New(const unsigned char *buffer, TQ3Uns32 validSize)
{	TQ3Object				theObject;
	TE3_MemoryStorageData	objectData;
	
	objectData.buffer = (TQ3Uns8 *)buffer;
	objectData.ownBuffer = kQ3True;
	objectData.bufferSize = validSize;
	objectData.validSize = validSize;
	objectData.growSize = kE3MemoryStorageDefaultGrowSize;


	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StorageTypeMemory, kQ3False, &objectData);

	return(theObject);
}





//=============================================================================
//      E3MemoryStorage_Set : Sets the buffer for a storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3MemoryStorage_Set(TQ3StorageObject storage, const unsigned char *buffer, TQ3Uns32 validSize)
{	TE3_MemoryStorageData	*instanceData  = (TE3_MemoryStorageData *) storage->instanceData;
	TQ3Status				qd3dStatus;
	


	// If we don't own our buffer, reset our state
	if (!instanceData->ownBuffer)
		{
		Q3Memory_Clear(instanceData, sizeof(TE3_MemoryStorageData));
		instanceData->ownBuffer = kQ3True;
		}



	// If we need to grow our buffer, do so
	if (instanceData->bufferSize < validSize)
		{
		qd3dStatus = Q3Memory_Reallocate(&instanceData->buffer, validSize);
		if (qd3dStatus != kQ3Success)
			return(kQ3Failure);
		
		instanceData->bufferSize = validSize;
		}



	// Verify that we do have a buffer, we own it, and it's large enough
	Q3_ASSERT(Q3_VALID_PTR(instanceData->buffer));
	Q3_ASSERT(instanceData->ownBuffer);
	Q3_ASSERT(instanceData->bufferSize >= validSize);



	// Copy the data into the buffer
	if (buffer != NULL)
		memcpy(instanceData->buffer, buffer, validSize);

	instanceData->validSize = validSize;
	instanceData->growSize  = kE3MemoryStorageDefaultGrowSize;

	Q3Shared_Edited(storage);

	return(kQ3Success);
}





//=============================================================================
//      E3MemoryStorage_NewBuffer : Creates a new storage using the buffer.
//-----------------------------------------------------------------------------
TQ3StorageObject
E3MemoryStorage_NewBuffer(unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize)
{	TQ3Object				theObject;
	TE3_MemoryStorageData	objectData;
	
	objectData.buffer = buffer;
	objectData.ownBuffer = kQ3False;
	objectData.bufferSize = bufferSize;
	objectData.validSize = validSize;
	objectData.growSize = validSize;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StorageTypeMemory, kQ3False, &objectData);

	return(theObject);
}





//=============================================================================
//      E3MemoryStorage_SetBuffer : Set the data for a storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3MemoryStorage_SetBuffer(TQ3StorageObject storage, unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize)
{	TE3_MemoryStorageData	*instanceData  = (TE3_MemoryStorageData *) storage->instanceData;
	TQ3Status				qd3dStatus;



	// If no buffer has been supplied, use our own
	if (buffer == NULL)
		qd3dStatus = Q3MemoryStorage_Set(storage, buffer, validSize);



	// Otherwise, use the supplied buffer
	else
		{
		// First check to see if we currently own the buffer...
		if (instanceData->ownBuffer == kQ3True)
			{
			// If the buffer has changed, we're no longer going to own it so we need to
			// release it. If the buffer is the same, we don't want to do anything - we're
			// just being called because the app has touched the contents and is calling
			// us for the side effect of calling Q3Shared_Edited).
			if (instanceData->buffer != buffer)
				{
				// Make sure the app isn't trying to change anything they shouldn't: since
				// we own the buffer, the app shouldn't be trying to resize it on us.
				Q3_ASSERT(instanceData->bufferSize == bufferSize);
				Q3_ASSERT(instanceData->validSize  == validSize);


				// Release the buffer
				instanceData->ownBuffer = kQ3False;
				Q3Memory_Free(&instanceData->buffer);
				}
			}



		// Update our state. Note that we leave ownBuffer unchanged: we either reset
		// it to false above (in which case the new buffer is owned by the app), or
		// the previous owner still owns it.
		instanceData->buffer     = buffer;
		instanceData->bufferSize = bufferSize;
		instanceData->validSize  = validSize;
		instanceData->growSize   = kE3MemoryStorageDefaultGrowSize;
		
		qd3dStatus = kQ3Success;
		}

	Q3Shared_Edited(storage);

	return(qd3dStatus);
}





//=============================================================================
//      E3MemoryStorage_GetBuffer : Return info about the buffer.
//-----------------------------------------------------------------------------
//		Note :	We need to handle NULL parameters, since the QD3D Interactive
//				Renderer calls us with some parameters as NULL.
//-----------------------------------------------------------------------------
TQ3Status
E3MemoryStorage_GetBuffer(TQ3StorageObject storage, unsigned char **buffer, TQ3Uns32 *validSize, TQ3Uns32 *bufferSize)
{	TE3_MemoryStorageData	*instanceData  = (TE3_MemoryStorageData *) storage->instanceData;

	if(buffer != NULL)
		*buffer = instanceData->buffer;
	if(validSize != NULL)
		*validSize = instanceData->validSize;
	if(bufferSize != NULL)
		*bufferSize = instanceData->bufferSize;
	
	return(kQ3Success);
}





//=============================================================================
//      E3PathStorage_New : Create a path storage object.
//-----------------------------------------------------------------------------
TQ3StorageObject
E3PathStorage_New(const char *pathName)
{	TQ3Object			theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StorageTypePath, kQ3False, pathName);

	return(theObject);
}





//=============================================================================
//      E3PathStorage_Set : Set the path for a path storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3PathStorage_Set(TQ3StorageObject theStorage, const char *pathName)
{	TQ3PathStorageData		*instanceData = (TQ3PathStorageData *) theStorage->instanceData;
	char					*newPath;
	TQ3Uns32				pathLen;



	// Take a copy of the new path
	pathLen = strlen(pathName);
	newPath = (char *) Q3Memory_Allocate(pathLen + 1);
	if (newPath == NULL)
		return(kQ3Failure);



	// Clean up the instance data
	if (instanceData->thePath != NULL)
		Q3Memory_Free(&instanceData->thePath);

	if (instanceData->theFile != NULL)
		fclose(instanceData->theFile);



	// Update the instance data
	instanceData->thePath = newPath;
	instanceData->theFile = NULL;

	return(kQ3Success);	
}





//=============================================================================
//      E3PathStorage_Get : Get the path for a path storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3PathStorage_Get(TQ3StorageObject theStorage, char *pathName)
{	TQ3PathStorageData		*instanceData = (TQ3PathStorageData *) theStorage->instanceData;



	// Return the path
	strncpy(pathName, instanceData->thePath, kQ3StringMaximumLength);

	return(kQ3Success);
}


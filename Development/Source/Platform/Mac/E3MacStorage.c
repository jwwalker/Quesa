/*  NAME:
        E3MacStorage.c

    DESCRIPTION:
        Macintosh specific Storage calls.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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

#include "E3Storage.h"

#if QUESA_USES_MOREFILES
	#include "MoreFiles.h"
	#include "MoreFilesExtras.h"
#endif





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kQ3MacStorage_IsOpenFlag							0x0001
#define kQ3MacStorage_CloseOnExitFlag						0x0002
#define kQ3MacStorage_ReadOnlyFlag							0x0004
#define kQ3MacStorage_BufferDirtyFlag						0x0100

#define kQ3MacStorage_BufferSize							4096
#define kQ3MacStorage_MaxSizeBuffered						3072
#define kQ3MacStorage_BufferInvalid							0xFFFFFFFF





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Mac storage
typedef struct TE3_MacStorageData {
	TQ3Uns8		*buffer;
	TQ3Uns32	bufferStart; // position of first byte in buffer in disk file
	TQ3Uns32	validBufferSize; 
	TQ3Uns32	fileEOF;
	
	TQ3Int16 	fsRefNum; 
	TQ3Int16	flags;
} TE3_MacStorageData;

typedef TE3_MacStorageData *TE3_MacStorageDataPtr;


// Handle storage
typedef struct TQ3HandleStorageData {
	TQ3Boolean		ownHandle;
	TQ3Uns32		theSize;
	Handle			theHnd;
} TQ3HandleStorageData;





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
#define e3storage_mac_setFlag(instanceData,   theFlag) (((TE3_MacStorageDataPtr)instanceData)->flags |= (theFlag))
#define e3storage_mac_clearFlag(instanceData, theFlag) (((TE3_MacStorageDataPtr)instanceData)->flags &= ~(theFlag))
#define e3storage_mac_hasFlag(instanceData,   theFlag) ((((TE3_MacStorageDataPtr)instanceData)->flags & (theFlag)) == (theFlag))





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3storage_mac_getsize : Get the size of the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_getsize(TQ3StorageObject storage, TQ3Uns32 *size)
{
	SInt32	theLength;
	OSErr	err;
	
	TE3_MacStorageDataPtr instanceData =(TE3_MacStorageDataPtr)E3ClassTree_FindInstanceData (storage, kQ3StorageTypeMacintosh);
	
	if(instanceData == NULL)
		{
		E3ErrorManager_PostError (kQ3ErrorInvalidObjectClass, kQ3False);
		return(kQ3Failure);
		}


	Q3_REQUIRE_OR_RESULT((instanceData->fsRefNum != -1),kQ3Failure);


	err = GetEOF(instanceData->fsRefNum, &theLength);

	if (err != noErr) {
		E3ErrorManager_PostPlatformError (err);
		return(kQ3Failure);
	}
	
	*size = (TQ3Uns32)theLength;
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_mac_fillbuffer : Fill our buffer.  (Used only when reading.)
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_fillbuffer(TQ3StorageObject storage,TE3_MacStorageDataPtr instanceData, TQ3Uns32 offset)
{
	OSErr	err;

	SInt32	ioByteCount;
	
	ioByteCount = kQ3MacStorage_BufferSize;
	
	if(instanceData->fileEOF == kQ3MacStorage_BufferInvalid)
		if(e3storage_mac_getsize (storage, &instanceData->fileEOF) != kQ3Success)
			return (kQ3Failure);
	
	if(offset >= instanceData->fileEOF)
			return (kQ3Failure);

	if(offset + ioByteCount > instanceData->fileEOF)
			ioByteCount = (TQ3Int32) instanceData->fileEOF - (TQ3Int32) offset;

	err = noErr;
	
	
	Q3_REQUIRE_OR_RESULT(instanceData->fsRefNum != -1,kQ3Failure);
	Q3_REQUIRE_OR_RESULT(instanceData->buffer != NULL,kQ3Failure);
	

	err = SetFPos(instanceData->fsRefNum, fsFromStart, (TQ3Int32) offset);
	if (err != noErr) {
		E3ErrorManager_PostPlatformError (err);
		return(kQ3Failure);
	}
	
	
	err = FSRead(instanceData->fsRefNum, &ioByteCount, instanceData->buffer);
	if (err != noErr) {
		E3ErrorManager_PostPlatformError (err);
		return(kQ3Failure);
	}
		
	instanceData->bufferStart = offset;
	instanceData->validBufferSize = (TQ3Uns32) ioByteCount;
	
	e3storage_mac_clearFlag(instanceData, kQ3MacStorage_BufferDirtyFlag);

	return(kQ3Success);
}





//=============================================================================
//      e3storage_mac_flushbuffer : write buffer to disk.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_flushbuffer(TQ3StorageObject storage,TE3_MacStorageDataPtr instanceData)
{
	OSErr	err;

	SInt32	theLength;

	SInt32	ioByteCount = (SInt32) instanceData->validBufferSize;
	

	if(!e3storage_mac_hasFlag(instanceData, kQ3MacStorage_BufferDirtyFlag)){
		instanceData->bufferStart = kQ3MacStorage_BufferInvalid;
		instanceData->validBufferSize = 0;
		return(kQ3Success);
		}
		
	Q3_REQUIRE_OR_RESULT(instanceData->fsRefNum != -1,kQ3Failure);
	Q3_REQUIRE_OR_RESULT(instanceData->buffer != NULL,kQ3Failure);
	
	err = GetEOF(instanceData->fsRefNum, &theLength);
	if (err != noErr) {
		E3ErrorManager_PostPlatformError (err);
		return(kQ3Failure);
	}
	// Grow storage 
	if ((instanceData->bufferStart + ioByteCount) > theLength){
		err = SetEOF(instanceData->fsRefNum, ((SInt32) instanceData->bufferStart + ioByteCount));
		if (err != noErr) {
			E3ErrorManager_PostPlatformError (err);
			return(kQ3Failure);
			}
		}

	err = SetFPos(instanceData->fsRefNum, fsFromStart, (SInt32) instanceData->bufferStart);
	if (err != noErr) {
		E3ErrorManager_PostPlatformError (err);
		return(kQ3Failure);
	}
	
	
	err = FSWrite(instanceData->fsRefNum, &ioByteCount, instanceData->buffer);
	if (err != noErr) {
		E3ErrorManager_PostPlatformError (err);
		return(kQ3Failure);
	}
	Q3_ASSERT(ioByteCount == instanceData->validBufferSize);
	
	instanceData->bufferStart = kQ3MacStorage_BufferInvalid;
	instanceData->validBufferSize = 0;
	
	e3storage_mac_clearFlag(instanceData, kQ3MacStorage_BufferDirtyFlag);
	
	
	
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_mac_read : Read data from the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_read(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead)
{
	OSErr					err;
	TE3_MacStorageDataPtr	instanceData;
	
	TQ3Uns8					*src,*dest;

	SInt32	ioByteCount;
	
	ioByteCount = (SInt32) dataSize;
	*sizeRead = 0UL;
	err = noErr;
	
	instanceData =(TE3_MacStorageDataPtr)E3ClassTree_FindInstanceData (storage, kQ3StorageTypeMacintosh);
	
	if(instanceData == NULL)
		{
		E3ErrorManager_PostError (kQ3ErrorInvalidObjectClass, kQ3False);
		return(kQ3Failure);
		}
	
	
	Q3_REQUIRE_OR_RESULT((instanceData->fsRefNum != -1),kQ3Failure);
	
	if(instanceData->fileEOF == kQ3MacStorage_BufferInvalid)
		if(e3storage_mac_getsize (storage, &instanceData->fileEOF) != kQ3Success)
			return (kQ3Failure);
	
	if(offset >= instanceData->fileEOF){
		E3ErrorManager_PostPlatformError (eofErr);
		return (kQ3Failure);
		}

		if(e3storage_mac_hasFlag(instanceData, kQ3MacStorage_BufferDirtyFlag))		
			if(e3storage_mac_flushbuffer (storage, instanceData) != kQ3Success)
				return kQ3Failure;


	if((offset + ioByteCount) > instanceData->fileEOF){
		err = eofErr;
		ioByteCount = (SInt32) instanceData->fileEOF - (SInt32) offset;
		}

	if(ioByteCount > kQ3MacStorage_MaxSizeBuffered){ // read it directly

		err = SetFPos(instanceData->fsRefNum, fsFromStart, (SInt32) offset);
		if (err != noErr) {
			E3ErrorManager_PostPlatformError (err);
			return(kQ3Failure);
		}
		
		
		err = FSRead(instanceData->fsRefNum, &ioByteCount, data);
		if (err != noErr) {
			E3ErrorManager_PostPlatformError (err);
			return(kQ3Failure);
		}
		
	
		}
	else{ // read via buffer
	
		//reload buffer if invalid
		if((instanceData->bufferStart >= offset) ||
			 (instanceData->bufferStart + kQ3MacStorage_BufferSize <= offset + ioByteCount) ||
			 (instanceData->bufferStart == kQ3MacStorage_BufferInvalid))
			if(e3storage_mac_fillbuffer (storage, instanceData, offset) != kQ3Success)
				return (kQ3Failure);
				
		src = ((TQ3Uns8*)instanceData->buffer) + (offset - instanceData->bufferStart);
		dest = (TQ3Uns8*)data;
		Q3Memory_Copy(src, dest, (TQ3Uns32) ioByteCount);
		}
		
	*sizeRead = (TQ3Uns32) ioByteCount;
	return(kQ3Success);
}





//=============================================================================
//      e3storage_mac_write : Write data to the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_write(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten)
{
	// TODO for now the writing methods remain unbuffered

	OSErr	err;
	TE3_MacStorageDataPtr	instanceData;
	SInt32	theLength;
	TQ3Uns8					*src,*dest;

	SInt32	ioByteCount;
		
	ioByteCount = (SInt32) dataSize;
	*sizeWritten = 0UL;
	err = noErr;
	instanceData =(TE3_MacStorageDataPtr)E3ClassTree_FindInstanceData (storage, kQ3StorageTypeMacintosh);
	
	if(instanceData == NULL)
		{
		E3ErrorManager_PostError (kQ3ErrorInvalidObjectClass, kQ3False);
		return(kQ3Failure);
		}

	Q3_REQUIRE_OR_RESULT((instanceData->fsRefNum != -1),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(instanceData->buffer != NULL,kQ3Failure);
	Q3_REQUIRE_OR_RESULT(!e3storage_mac_hasFlag(instanceData, kQ3MacStorage_ReadOnlyFlag),kQ3Failure);


	// check buffer out of range
	if((offset < (instanceData->bufferStart + instanceData->validBufferSize)) ||
		(offset > (instanceData->bufferStart + kQ3MacStorage_BufferSize)) ||
		(offset + dataSize > (instanceData->bufferStart + kQ3MacStorage_BufferSize)))
		{
		if(e3storage_mac_flushbuffer (storage, instanceData) != kQ3Success)
			return kQ3Failure;
		
		instanceData->bufferStart = offset;
		}
	
	Q3_ASSERT( (instanceData->bufferStart == kQ3MacStorage_BufferInvalid) ||
		(instanceData->bufferStart + instanceData->validBufferSize == offset) );

	// we have a valid buffer
	if( dataSize >= kQ3MacStorage_MaxSizeBuffered)
		{// write directly
		if(e3storage_mac_hasFlag(instanceData, kQ3MacStorage_BufferDirtyFlag))		
			if(e3storage_mac_flushbuffer (storage, instanceData) != kQ3Success)
				return kQ3Failure;
		
		// Make sure that the next write doesn't think there is anything in the buffer
		instanceData->bufferStart = kQ3MacStorage_BufferInvalid;
		
		err = GetEOF(instanceData->fsRefNum, &theLength);
		if (err != noErr)
			{
			E3ErrorManager_PostPlatformError (err);
			return(kQ3Failure);
			}
		// Grow storage 
		if ((offset + dataSize) > theLength)
			{
			err = SetEOF(instanceData->fsRefNum, (SInt32) (offset + dataSize));
			if (err != noErr)
				{
				E3ErrorManager_PostPlatformError (err);
				return(kQ3Failure);
				}
			}
	
		err = SetFPos(instanceData->fsRefNum, fsFromStart, (SInt32) offset);
		if (err != noErr)
			{
			E3ErrorManager_PostPlatformError (err);
			return(kQ3Failure);
			}
		
		
		err = FSWrite(instanceData->fsRefNum, &ioByteCount, data);
		if (err != noErr)
			{
			E3ErrorManager_PostPlatformError (err);
			return(kQ3Failure);
			}
		}
	else
		{	 // copy it to buffer
		dest = ((TQ3Uns8*)instanceData->buffer) + (offset - instanceData->bufferStart);
		src = (TQ3Uns8*)data;
		Q3Memory_Copy(src, dest, dataSize);

		instanceData->validBufferSize += dataSize;
		e3storage_mac_setFlag(instanceData, kQ3MacStorage_BufferDirtyFlag);
		}
	*sizeWritten = (TQ3Uns32) ioByteCount;
	
	Q3_ASSERT( (instanceData->bufferStart == kQ3MacStorage_BufferInvalid) ||
		(instanceData->bufferStart + instanceData->validBufferSize == offset + dataSize) );


	return(kQ3Success);
	
}





//=============================================================================
//      e3storage_mac_new : New method.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TE3_MacStorageData			*instanceData  = (TE3_MacStorageData *) privateData;
	const TQ3Int16	*fsRefNumPtr  = (const TQ3Int16 *) paramData;

	instanceData->buffer = (TQ3Uns8*)Q3Memory_Allocate(kQ3MacStorage_BufferSize);		// Allocate buffer
	if(instanceData->buffer == NULL)
		return (kQ3Failure);
		
	instanceData->bufferStart = kQ3MacStorage_BufferInvalid;		// this will force the refill of the buffer at first read
	instanceData->fileEOF = kQ3MacStorage_BufferInvalid;			// this marks the value as invalid
	
	
	
	instanceData->flags = 0;		// initialize flags
	
	if(fsRefNumPtr != NULL){ 		// I'm the leaf type
		Q3_ASSERT(Q3Object_GetLeafType(theObject) == kQ3StorageTypeMacintosh);
		
		instanceData->fsRefNum = *fsRefNumPtr;
		// TODO
		// test if read only and then set flag
		
		}
	else{							// I'm an FSSpecStorage
		instanceData->fsRefNum = -1;
		e3storage_mac_setFlag(instanceData, kQ3MacStorage_CloseOnExitFlag);
		}

	return(kQ3Success);
}





//=============================================================================
//      e3storage_mac_delete : Delete method.
//-----------------------------------------------------------------------------
static void
e3storage_mac_delete(TQ3Object storage, void *privateData)
{	TE3_MacStorageData			*instanceData  = (TE3_MacStorageData *) privateData;

	Q3Memory_Free(&instanceData->buffer);

}





//=============================================================================
//      e3storage_mac_metahandler : Storage metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3storage_mac_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_delete;
			break;

		/*case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_fsspec_duplicate;
			break;*/
// Jose, what's the correnct behavior for a double referenced disk file?

		case kQ3XMethodTypeStorageGetSize:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_getsize;
			break;

		case kQ3XMethodTypeStorageReadData:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_read;
			break;

		case kQ3XMethodTypeStorageWriteData:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_write;
			break;

		}
	
	return(theMethod);
}





//=============================================================================
//      e3storage_mac_fsspec_open : Open the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_fsspec_open(TQ3StorageObject theStorage, TQ3Boolean forWriting)
{	TE3_MacStorageDataPtr	 instanceDataMac    = (TE3_MacStorageDataPtr) E3ClassTree_FindInstanceData(theStorage, kQ3StorageTypeMacintosh);
	FSSpec					*instanceDataFSSpec = (FSSpec              *) E3ClassTree_FindInstanceData(theStorage, kQ3MacintoshStorageTypeFSSpec);
	TQ3Int16				filePerm;
	OSStatus				theErr;



	// Validate our state
	Q3_REQUIRE_OR_RESULT(instanceDataMac->fsRefNum == -1, kQ3Failure);



	// Open the file
#if QUESA_USES_MOREFILES
	filePerm = forWriting ? dmRdWrDenyRdWr : dmRdDenyWr;
	theErr   = FSpOpenAware(instanceDataFSSpec, filePerm, &instanceDataMac->fsRefNum);
#else
	filePerm = forWriting ? fsRdWrPerm : fsRdPerm;
	theErr   = FSpOpenDF(   instanceDataFSSpec, filePerm, &instanceDataMac->fsRefNum);
#endif



	// Handle failure
	if (theErr != noErr)
		{
		E3ErrorManager_PostPlatformError(theErr);
		instanceDataMac->fsRefNum = -1;
		}
	
	return(theErr == noErr ? kQ3Success : kQ3Failure);
}





//=============================================================================
//      e3storage_mac_fsspec_close : Close the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_fsspec_close(TQ3StorageObject theStorage)
{	TE3_MacStorageDataPtr	 instanceDataMac    = (TE3_MacStorageDataPtr) E3ClassTree_FindInstanceData(theStorage, kQ3StorageTypeMacintosh);
	FSSpec					*instanceDataFSSpec = (FSSpec              *) E3ClassTree_FindInstanceData(theStorage, kQ3MacintoshStorageTypeFSSpec);
	OSStatus				theErr;



	// Close the file
	if (instanceDataMac->fsRefNum != -1)
		{
		// Flush the file
		if(e3storage_mac_hasFlag(instanceDataMac, kQ3MacStorage_BufferDirtyFlag))
			e3storage_mac_flushbuffer(theStorage, instanceDataMac);
		

		
		// Close it
		theErr = FSClose(instanceDataMac->fsRefNum);
		instanceDataMac->fsRefNum = -1;
		
		if (theErr != noErr)
			{
			E3ErrorManager_PostPlatformError(theErr);
			return(kQ3Failure);
			}

		
		
		// And flush the volume
		e3storage_mac_clearFlag(instanceDataMac, kQ3MacStorage_IsOpenFlag);
		if(!e3storage_mac_hasFlag(instanceDataMac, kQ3MacStorage_ReadOnlyFlag))
			FlushVol(NULL, instanceDataFSSpec->vRefNum);
		}

	return(kQ3Success);
}





//=============================================================================
//      e3storage_mac_fsspec_delete : Delete the storage object.
//-----------------------------------------------------------------------------
static void
e3storage_mac_fsspec_delete(TQ3Object storage, void *privateData)
{
#pragma unused(privateData)
	// storage MUST be closed
	TE3_MacStorageDataPtr instanceData =(TE3_MacStorageDataPtr)E3ClassTree_FindInstanceData (storage, kQ3StorageTypeMacintosh);
	
	if(instanceData == NULL)
		{
		E3ErrorManager_PostError (kQ3ErrorInvalidObjectClass, kQ3False);
		return;
		}

	
	if(e3storage_mac_hasFlag(instanceData, kQ3MacStorage_IsOpenFlag))
		{
		E3ErrorManager_PostError (kQ3ErrorStorageIsOpen, kQ3True);
		}
}





//=============================================================================
//      e3storage_mac_fsspec_metahandler : Storage metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3storage_mac_fsspec_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_fsspec_delete;
			break;

		/*case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_fsspec_duplicate;
			break;*/
//Jose, what's the correnct behavior for a double referenced disk file?

		case kQ3XMethodTypeStorageOpen:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_fsspec_open;
			break;

		case kQ3XMethodTypeStorageClose:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_fsspec_close;
			break;

		}
	
	return(theMethod);
}





//=============================================================================
//      e3storage_mac_handle_new : Handle storage new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_handle_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3HandleStorageData		*instanceData = (TQ3HandleStorageData *) privateData;
	const TQ3HandleStorageData	*storageData  = (const TQ3HandleStorageData *) paramData;
	OSErr						theErr;



	// Initialise our instance data
	*instanceData = *storageData;



	// If we're expected to create our own handle, do so	
	if (instanceData->theHnd == NULL)
		{
		instanceData->theHnd = TempNewHandle((SInt32) instanceData->theSize, &theErr);
		if (instanceData->theHnd == NULL || theErr != noErr)
			return(kQ3Failure);

		instanceData->ownHandle = kQ3True;
		}

	return(kQ3Success);
}





//=============================================================================
//      e3storage_mac_handle_delete : Handle storage delete method.
//-----------------------------------------------------------------------------
static void
e3storage_mac_handle_delete(TQ3Object storage, void *privateData)
{	TQ3HandleStorageData	*instanceData = (TQ3HandleStorageData *) privateData;
#pragma unused(storage)



	// Dispose of our instance data
	if (instanceData->ownHandle && instanceData->theHnd != NULL)
		DisposeHandle(instanceData->theHnd);
}





//=============================================================================
//      e3storage_mac_handle_getsize : Get the size of the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_handle_getsize(TQ3StorageObject storage, TQ3Uns32 *size)
{	TQ3HandleStorageData	*instanceData = (TQ3HandleStorageData *) E3ClassTree_FindInstanceData(storage, kQ3MemoryStorageTypeHandle);



	// Return our size
	*size = instanceData->theSize;
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_mac_handle_read : Read data from the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_handle_read(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead)
{	TQ3HandleStorageData	*instanceData = (TQ3HandleStorageData *) E3ClassTree_FindInstanceData(storage, kQ3MemoryStorageTypeHandle);
	TQ3Uns32 				bytesToRead;



	// Initialise a return value
	*sizeRead = 0;



	// Make sure we have enough space
	if (offset >= instanceData->theSize)
		return(kQ3Failure);



	// Work out how much we should copy
	bytesToRead = dataSize;
	if (offset + bytesToRead > instanceData->theSize)
		bytesToRead = instanceData->theSize - offset;



	// Copy the block (using BlockMoveData since this is Mac specific,
	// while Q3Memory_Copy would require us to lock the handle).
	BlockMoveData((*instanceData->theHnd) + offset, data, (SInt32) bytesToRead);
	
	*sizeRead = bytesToRead;
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_mac_handle_write : Write data to the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_handle_write(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten)
{	TQ3HandleStorageData	*instanceData = (TQ3HandleStorageData *) E3ClassTree_FindInstanceData(storage, kQ3MemoryStorageTypeHandle);
	TQ3Uns32 				newSize, bytesToWrite;



	// Initialise a return value
	*sizeWritten = 0;
	bytesToWrite = dataSize;



	// Make sure we have enough space
	// Grow the handle if we need to
	newSize = offset + bytesToWrite;
	if (newSize > instanceData->theSize)
		{
		SetHandleSize(instanceData->theHnd, (SInt32) newSize);
		if (MemError() != noErr)
			return(kQ3Failure);

		instanceData->theSize = newSize;
		}


	// Copy the block (using BlockMoveData since this is Mac specific,
	// while Q3Memory_Copy would require us to lock the handle).
	BlockMoveData(data, (*instanceData->theHnd) + offset, (SInt32) bytesToWrite);
	
	*sizeWritten = bytesToWrite;
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_mac_handle_metahandler : Handle storage metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3storage_mac_handle_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_handle_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_handle_delete;
			break;

		case kQ3XMethodTypeStorageGetSize:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_handle_getsize;
			break;

		case kQ3XMethodTypeStorageReadData:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_handle_read;
			break;

		case kQ3XMethodTypeStorageWriteData:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_handle_write;
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
E3MacStorage_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the classes
	//
	// N.B. - Handle storage is a sub-class of memory storage, not a
	// subclass of storage (the indentation in QD3D.h is incorrect).
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeStorage,
											kQ3StorageTypeMacintosh,
											kQ3ClassNameStorageMacintosh,
											e3storage_mac_metahandler,
											sizeof(TE3_MacStorageData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3StorageTypeMacintosh,
											kQ3MacintoshStorageTypeFSSpec,
											kQ3ClassNameStorageFSSpec,
											e3storage_mac_fsspec_metahandler,
											sizeof(FSSpec));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3StorageTypeMemory,
											kQ3MemoryStorageTypeHandle,
											kQ3ClassNameStorageHandle,
											e3storage_mac_handle_metahandler,
											sizeof(TQ3HandleStorageData));

	return(qd3dStatus);
}





//=============================================================================
//      E3Storage_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3MacStorage_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3MemoryStorageTypeHandle,    kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3MacintoshStorageTypeFSSpec, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StorageTypeMacintosh,       kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3HandleStorage_New : Create a Handle storage object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StorageObject
E3HandleStorage_New(Handle handle, TQ3Uns32 validSize)
{	TQ3HandleStorageData		storageData;
	TQ3Object					theObject;



	// Create the object
	storageData.ownHandle = kQ3False;
	storageData.theSize   = validSize;
	storageData.theHnd    = handle;

	theObject = E3ClassTree_CreateInstance(kQ3MemoryStorageTypeHandle, kQ3False, &storageData);

	return(theObject);
}





//=============================================================================
//      E3HandleStorage_Set : Set the Handle for a Handle storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3HandleStorage_Set(TQ3StorageObject storage, Handle handle, TQ3Uns32 validSize)
{	TQ3HandleStorageData	*instanceData  = (TQ3HandleStorageData *) E3ClassTree_FindInstanceData(storage, kQ3MemoryStorageTypeHandle);
	OSErr					theErr;



	// If we own our buffer, get rid of it. Either way, reset our state
	if (instanceData->ownHandle)
		DisposeHandle(instanceData->theHnd);

	Q3Memory_Clear(instanceData, sizeof(TQ3HandleStorageData));



	// If we're expected to create a new handle, create one
	if (handle == NULL)
		{
		instanceData->theHnd = TempNewHandle((SInt32) validSize, &theErr);
		if (instanceData->theHnd == NULL || theErr != noErr)
			return(kQ3Failure);

		instanceData->ownHandle = kQ3True;
		instanceData->theSize   = validSize;
		}
	
	
	
	// Otherwise, save the supplied handle
	else
		{
		instanceData->ownHandle = kQ3False;
		instanceData->theSize   = validSize;
		instanceData->theHnd    = handle;
		}

	Q3Shared_Edited(storage);

	return(kQ3Success);
}





//=============================================================================
//      E3HandleStorage_Get : Get info about a Handle storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3HandleStorage_Get(TQ3StorageObject storage, Handle *handle, TQ3Uns32 *validSize)
{	TQ3HandleStorageData	*instanceData = (TQ3HandleStorageData *) E3ClassTree_FindInstanceData(storage, kQ3MemoryStorageTypeHandle);



	// Return the information
	*handle    = instanceData->theHnd;
	*validSize = instanceData->theSize;

	return(kQ3Success);
}





//=============================================================================
//      E3MacintoshStorage_New : Create a new Macintosh storage object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StorageObject
E3MacintoshStorage_New(TQ3Int16 fsRefNum)
{	TQ3Object			theObject;


	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StorageTypeMacintosh, kQ3False, &fsRefNum);
	return(theObject);
}





//=============================================================================
//      E3MacintoshStorage_Set : Set the information for a Mac storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3MacintoshStorage_Set(TQ3StorageObject storage, TQ3Int16 fsRefNum)
{
	//parameters has been tested by Q3MacintoshStorage_Set
	
	// storage MUST be closed
	TE3_MacStorageDataPtr instanceData =(TE3_MacStorageDataPtr)E3ClassTree_FindInstanceData (storage, kQ3StorageTypeMacintosh);
	
	if(instanceData == NULL)
		{
		E3ErrorManager_PostError (kQ3ErrorInvalidObjectClass, kQ3False);
		return(kQ3Failure);
		}

	
	if(e3storage_mac_hasFlag(instanceData, kQ3MacStorage_IsOpenFlag))
		{
		E3ErrorManager_PostError (kQ3ErrorStorageIsOpen, kQ3False);
		return(kQ3Failure);
		}
	

	((TE3_MacStorageDataPtr)instanceData)->fsRefNum = fsRefNum;

	Q3Shared_Edited(storage);
	
	return(kQ3Success);
}





//=============================================================================
//      E3MacintoshStorage_Get : Get the information for a Mac storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3MacintoshStorage_Get(TQ3StorageObject storage, TQ3Int16 *fsRefNum)
{
	//parameters has been tested by Q3MacintoshStorage_Get

	TE3_MacStorageDataPtr instanceData =(TE3_MacStorageDataPtr)E3ClassTree_FindInstanceData (storage, kQ3StorageTypeMacintosh);
	
	if(instanceData == NULL)
		{
		E3ErrorManager_PostError (kQ3ErrorInvalidObjectClass, kQ3False);
		return(kQ3Failure);
		}


	*fsRefNum = instanceData->fsRefNum;
	
	return(kQ3Success);
}





//=============================================================================
//      E3MacintoshStorage_GetType : Return the type of a Mac storage object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3MacintoshStorage_GetType(TQ3StorageObject storage)
{


	// Return the type
	return(E3ClassTree_GetObjectType(storage, kQ3StorageTypeMacintosh));
}





//=============================================================================
//      E3FSSpecStorage_New : Create a new FSSpec storage object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StorageObject
E3FSSpecStorage_New(const FSSpec *fs)
{	TQ3Object			theObject;


	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3MacintoshStorageTypeFSSpec, kQ3False, fs);
	return(theObject);
}





//=============================================================================
//      E3FSSpecStorage_Set : Set the information for an FSSpec storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3FSSpecStorage_Set(TQ3StorageObject theStorage, const FSSpec *theFSSpec)
{	TE3_MacStorageDataPtr	 instanceDataMac    = (TE3_MacStorageDataPtr) E3ClassTree_FindInstanceData(theStorage, kQ3StorageTypeMacintosh);
	FSSpec					*instanceDataFSSpec = (FSSpec              *) E3ClassTree_FindInstanceData(theStorage, kQ3MacintoshStorageTypeFSSpec);



	// Make sure we're not open
	if (e3storage_mac_hasFlag(instanceDataMac, kQ3MacStorage_IsOpenFlag))
		{
		E3ErrorManager_PostError(kQ3ErrorStorageIsOpen, kQ3False);
		return(kQ3Failure);
		}



	// Set the data
	Q3Memory_Copy(theFSSpec, instanceDataFSSpec, sizeof(FSSpec));
	Q3Shared_Edited(theStorage);
	
	return(kQ3Success);
}





//=============================================================================
//      E3FSSpecStorage_Get : Get the information for an FSSpec storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3FSSpecStorage_Get(TQ3StorageObject theStorage, FSSpec *theFSSpec)
{	FSSpec	*instanceData = E3ClassTree_FindInstanceData(theStorage, kQ3MacintoshStorageTypeFSSpec);



	// Get the data
	Q3Memory_Copy(theFSSpec, instanceData, sizeof(FSSpec));
	
	return(kQ3Success);
}



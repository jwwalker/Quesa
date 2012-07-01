/*  NAME:
        E3MacStorage.c

    DESCRIPTION:
        Macintosh specific Storage calls.

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

#include "E3Storage.h"





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
	
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1050
	FSIORefNum 	fsRefNum; 
#else
	TQ3Int16 	fsRefNum; 
#endif
	TQ3Int16	flags;
} TE3_MacStorageData;

typedef TE3_MacStorageData *TE3_MacStorageDataPtr;


// Handle storage
typedef struct TQ3HandleStorageData {
	TQ3Boolean		ownHandle;
	TQ3Uns32		theSize;
	Handle			theHnd;
} TQ3HandleStorageData;





class E3MacintoshStorage : public E3Storage // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
	{
Q3_CLASS_ENUMS ( kQ3StorageTypeMacintosh, E3MacintoshStorage, E3Storage )
public :

	TE3_MacStorageData					macStorageData ;
	} ;
	


class E3FSSpecStorage : public E3MacintoshStorage  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3MacintoshStorageTypeFSSpec, E3FSSpecStorage, E3MacintoshStorage )
public :

	FSRef								theFSRef;
	} ;
	


class E3HandleStorage : public E3MemoryStorage  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3MemoryStorageTypeHandle, E3HandleStorage, E3MemoryStorage )
public :

	TQ3HandleStorageData				instanceData ;
	} ;
	


//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
#define e3storage_mac_setFlag(storage,   theFlag) ((storage)->macStorageData.flags |= (theFlag))
#define e3storage_mac_clearFlag(storage, theFlag) ((storage)->macStorageData.flags &= ~(theFlag))
#define e3storage_mac_hasFlag(storage,   theFlag) (((storage)->macStorageData.flags & (theFlag)) == (theFlag))





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3storage_mac_getsize : Get the size of the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_getsize ( E3MacintoshStorage* storage, TQ3Uns32 *size )
	{
	Q3_REQUIRE_OR_RESULT( ( storage->macStorageData.fsRefNum != -1 ), kQ3Failure ) ;


	SInt64 theLength ;
	OSErr err = ::FSGetForkSize ( storage->macStorageData.fsRefNum, & theLength ) ;

	if ( err != noErr )
		{
		E3ErrorManager_PostPlatformError ( err ) ;
		return kQ3Failure ;
		}
	
	*size = (TQ3Uns32) theLength ;
	
	return kQ3Success ;
	}





//=============================================================================
//      e3storage_mac_fillbuffer : Fill our buffer.  (Used only when reading.)
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_fillbuffer ( E3MacintoshStorage* storage, TQ3Uns32 offset )
	{
	ByteCount ioByteCount = kQ3MacStorage_BufferSize ;
	
	if ( storage->macStorageData.fileEOF == kQ3MacStorage_BufferInvalid )
		if ( e3storage_mac_getsize ( storage, & storage->macStorageData.fileEOF ) != kQ3Success )
			return kQ3Failure ;
	
	if ( offset >= storage->macStorageData.fileEOF )
			return kQ3Failure ;

	if ( offset + ioByteCount > storage->macStorageData.fileEOF )
			ioByteCount = (TQ3Int32) storage->macStorageData.fileEOF - (TQ3Int32) offset ;

	OSErr err = noErr ;
	
	
	Q3_REQUIRE_OR_RESULT( storage->macStorageData.fsRefNum != -1, kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( storage->macStorageData.buffer != NULL, kQ3Failure ) ;
	

	err = ::FSSetForkPosition ( storage->macStorageData.fsRefNum, fsFromStart, (TQ3Int32) offset ) ;
	if ( err != noErr )
		{
		E3ErrorManager_PostPlatformError ( err ) ;
		return kQ3Failure ;
		}
	
	
	err = ::FSReadFork ( storage->macStorageData.fsRefNum, fsFromMark, 0,
		ioByteCount, storage->macStorageData.buffer, &ioByteCount ) ;
	if ( err != noErr )
		{
		E3ErrorManager_PostPlatformError ( err ) ;
		return kQ3Failure ;
		}
		
	storage->macStorageData.bufferStart = offset ;
	storage->macStorageData.validBufferSize = (TQ3Uns32) ioByteCount ;
	
	e3storage_mac_clearFlag( storage, kQ3MacStorage_BufferDirtyFlag ) ;

	return kQ3Success ;
	}





//=============================================================================
//      e3storage_mac_flushbuffer : write buffer to disk.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_flushbuffer ( E3MacintoshStorage* storage )
	{
	ByteCount	ioByteCount = storage->macStorageData.validBufferSize ;
	

	if ( ! e3storage_mac_hasFlag( storage, kQ3MacStorage_BufferDirtyFlag ) )
		{
		storage->macStorageData.bufferStart = kQ3MacStorage_BufferInvalid ;
		storage->macStorageData.validBufferSize = 0 ;
		return kQ3Success ;
		}
		
	Q3_REQUIRE_OR_RESULT( storage->macStorageData.fsRefNum != -1, kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( storage->macStorageData.buffer != NULL, kQ3Failure ) ;
	
	SInt64 theLength ;
	OSErr err = ::FSGetForkSize ( storage->macStorageData.fsRefNum, &theLength ) ;
	if ( err != noErr )
		{
		E3ErrorManager_PostPlatformError ( err ) ;
		return kQ3Failure ;
		}
	// Grow storage 
	if ( ( storage->macStorageData.bufferStart + ioByteCount ) > (TQ3Uns32)theLength )
		{
		err = ::FSSetForkSize ( storage->macStorageData.fsRefNum, fsFromStart,
			((SInt32) storage->macStorageData.bufferStart + ioByteCount ) ) ;
		if ( err != noErr )
			{
			E3ErrorManager_PostPlatformError ( err ) ;
			return kQ3Failure ;
			}
		}

	err = ::FSSetForkPosition ( storage->macStorageData.fsRefNum, fsFromStart,
		(SInt32) storage->macStorageData.bufferStart ) ;
	if ( err != noErr )
		{
		E3ErrorManager_PostPlatformError ( err ) ;
		return kQ3Failure ;
		}
	
	
	err = ::FSWriteFork ( storage->macStorageData.fsRefNum, fsFromMark, 0,
		ioByteCount, storage->macStorageData.buffer, &ioByteCount ) ;
	if ( err != noErr )
		{
		E3ErrorManager_PostPlatformError ( err ) ;
		return kQ3Failure ;
		}
	Q3_ASSERT ( ioByteCount == storage->macStorageData.validBufferSize ) ;
	
	storage->macStorageData.bufferStart = kQ3MacStorage_BufferInvalid ;
	storage->macStorageData.validBufferSize = 0; 
	
	e3storage_mac_clearFlag( storage, kQ3MacStorage_BufferDirtyFlag ) ;
	
	
	
	
	return kQ3Success ;
	}





//=============================================================================
//      e3storage_mac_read : Read data from the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_read ( E3MacintoshStorage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead )
	{
	ByteCount ioByteCount = dataSize ;
	*sizeRead = 0U;
	OSErr err = noErr;
	
	Q3_REQUIRE_OR_RESULT( ( storage->macStorageData.fsRefNum != -1 ), kQ3Failure ) ;
	
	if ( storage->macStorageData.fileEOF == kQ3MacStorage_BufferInvalid )
		if ( e3storage_mac_getsize ( storage, & storage->macStorageData.fileEOF ) != kQ3Success )
			return kQ3Failure ;
	
	if ( offset >= storage->macStorageData.fileEOF )
		{
		E3ErrorManager_PostPlatformError ( (TQ3Uns32) eofErr ) ;
		return kQ3Failure ;
		}

	if ( e3storage_mac_hasFlag( storage, kQ3MacStorage_BufferDirtyFlag ) )		
		if ( e3storage_mac_flushbuffer ( storage ) != kQ3Success )
			return kQ3Failure ;


	if ( ( offset + ioByteCount) > storage->macStorageData.fileEOF )
		{
		//err = eofErr ;
		ioByteCount = (SInt32) storage->macStorageData.fileEOF - (SInt32) offset ;
		}

	if ( ioByteCount > kQ3MacStorage_MaxSizeBuffered )
		{ // read it directly
		err = ::FSSetForkPosition ( storage->macStorageData.fsRefNum, fsFromStart, (SInt32) offset ) ;
		if (err != noErr)
			{
			E3ErrorManager_PostPlatformError ( err ) ;
			return kQ3Failure ;
			}
		
		
		err = ::FSReadFork ( storage->macStorageData.fsRefNum, fsFromMark, 0,
			ioByteCount, data, &ioByteCount ) ;
		if ( err != noErr )
			{
			E3ErrorManager_PostPlatformError ( err ) ;
			return kQ3Failure ;
			}
		
	
		}
	else
		{ // read via buffer
		//reload buffer if invalid
		if ( ( storage->macStorageData.bufferStart >= offset )
		|| ( storage->macStorageData.bufferStart + kQ3MacStorage_BufferSize <= offset + ioByteCount )
		|| ( storage->macStorageData.bufferStart == kQ3MacStorage_BufferInvalid ) )
			if ( e3storage_mac_fillbuffer ( storage, offset ) != kQ3Success )
				return kQ3Failure ;
				
		TQ3Uns8* src = ( (TQ3Uns8*) storage->macStorageData.buffer ) + ( offset - storage->macStorageData.bufferStart ) ;
		TQ3Uns8* dest = (TQ3Uns8*) data ;
		Q3Memory_Copy ( src, dest, (TQ3Uns32) ioByteCount ) ;
		}
		
	*sizeRead = (TQ3Uns32) ioByteCount ;
	return kQ3Success ;
	}





//=============================================================================
//      e3storage_mac_write : Write data to the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_write ( E3MacintoshStorage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten )
	{

	ByteCount ioByteCount = dataSize ;
	*sizeWritten = 0U;
	OSErr err = noErr ;
	
	Q3_REQUIRE_OR_RESULT( ( storage->macStorageData.fsRefNum != -1 ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( storage->macStorageData.buffer != NULL, kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( ! e3storage_mac_hasFlag( storage, kQ3MacStorage_ReadOnlyFlag ), kQ3Failure ) ;



	// we have a valid buffer
	if ( dataSize >= kQ3MacStorage_MaxSizeBuffered )
		{// write directly
		if ( e3storage_mac_hasFlag( storage, kQ3MacStorage_BufferDirtyFlag ) )		
			if ( e3storage_mac_flushbuffer ( storage ) != kQ3Success )
				return kQ3Failure ;
		
		// Make sure that the next write doesn't think there is anything in the buffer
		storage->macStorageData.bufferStart = kQ3MacStorage_BufferInvalid ;
		
		SInt64 theLength ;
		err = ::FSGetForkSize ( storage->macStorageData.fsRefNum, &theLength ) ;
		if ( err != noErr )
			{
			E3ErrorManager_PostPlatformError ( err ) ;
			return kQ3Failure ;
			}
		// Grow storage 
		if ( (offset + dataSize ) > theLength )
			{
			err = ::FSSetForkSize ( storage->macStorageData.fsRefNum, fsFromStart,
				(SInt32) ( offset + dataSize ) ) ;
			if ( err != noErr )
				{
				E3ErrorManager_PostPlatformError ( err ) ;
				return kQ3Failure ;
				}
			}
	
		err = ::FSSetForkPosition ( storage->macStorageData.fsRefNum, fsFromStart, (SInt32) offset ) ;
		if (err != noErr)
			{
			E3ErrorManager_PostPlatformError ( err ) ;
			return kQ3Failure ;
			}
		
		
		err = ::FSWriteFork ( storage->macStorageData.fsRefNum, fsFromMark, 0,
			ioByteCount, data, &ioByteCount ) ;
		if (err != noErr)
			{
			E3ErrorManager_PostPlatformError ( err ) ;
			return kQ3Failure ;
			}
		}
	else
		{	 // copy it to buffer
		// check buffer out of range
		if ( e3storage_mac_hasFlag( storage, kQ3MacStorage_BufferDirtyFlag ) )	{	
			if ( ( offset < ( storage->macStorageData.bufferStart/*+ storage->macStorageData.validBufferSize */) )
			||   ( offset > ( storage->macStorageData.bufferStart + storage->macStorageData.validBufferSize ) )
			||   ( offset + dataSize > ( storage->macStorageData.bufferStart + kQ3MacStorage_BufferSize ) ) )
				{
				if ( e3storage_mac_flushbuffer ( storage ) != kQ3Success )
					return kQ3Failure ;
				
				}
			}
			
		if(storage->macStorageData.bufferStart == kQ3MacStorage_BufferInvalid){
			storage->macStorageData.bufferStart = offset ;
			}
	
		TQ3Uns8* dest = ( (TQ3Uns8*) storage->macStorageData.buffer ) + ( offset - storage->macStorageData.bufferStart ) ;
		TQ3Uns8* src = (TQ3Uns8*) data ;
		Q3Memory_Copy ( src, dest, dataSize ) ;
		
		if(offset + dataSize > storage->macStorageData.bufferStart+storage->macStorageData.validBufferSize){
			storage->macStorageData.validBufferSize = (offset + dataSize) - storage->macStorageData.bufferStart ;
			}
			
		e3storage_mac_setFlag ( storage, kQ3MacStorage_BufferDirtyFlag ) ;
		}
		
	*sizeWritten = (TQ3Uns32) ioByteCount ;
	


	return kQ3Success ;
	}





//=============================================================================
//      e3storage_mac_new : New method.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_new(TQ3Object theObject, void *privateData, const void *paramData)
	{
	TE3_MacStorageData* instanceData  = (TE3_MacStorageData *) privateData ;
	const TQ3Int16	*fsRefNumPtr  = (const TQ3Int16 *) paramData ;

	instanceData->buffer = (TQ3Uns8*) Q3Memory_Allocate ( kQ3MacStorage_BufferSize ) ;		// Allocate buffer
	if ( instanceData->buffer == NULL )
		return kQ3Failure ;
		
	instanceData->bufferStart = kQ3MacStorage_BufferInvalid ;		// this will force the refill of the buffer at first read
	instanceData->fileEOF = kQ3MacStorage_BufferInvalid ;			// this marks the value as invalid
	
	
	
	instanceData->flags = 0 ;		// initialize flags
	
	if ( fsRefNumPtr != NULL )
		{ 		// I'm the leaf type
		Q3_ASSERT ( Q3Object_GetLeafType ( theObject ) == kQ3StorageTypeMacintosh ) ;
		
		instanceData->fsRefNum = *fsRefNumPtr ;
		// TODO
		// test if read only and then set flag
		
		}
	else{							// I'm an FSSpecStorage
		instanceData->fsRefNum = -1 ;
		instanceData->flags |= kQ3MacStorage_CloseOnExitFlag ;
		}

	return kQ3Success ;
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
e3storage_mac_fsspec_open ( E3FSSpecStorage* theStorage, TQ3Boolean forWriting )
	{
	// Validate our state
	Q3_REQUIRE_OR_RESULT( theStorage->macStorageData.fsRefNum == -1, kQ3Failure ) ;
	
	
	// Open the file
	TQ3Int16 filePerm = forWriting ? fsRdWrPerm : fsRdPerm ;
	OSStatus	theErr   = ::FSOpenFork( &theStorage->theFSRef, 0, NULL, filePerm,
			& theStorage->macStorageData.fsRefNum );



	// Handle failure
	if ( theErr != noErr )
		{
		E3ErrorManager_PostPlatformError ( theErr ) ;
		theStorage->macStorageData.fsRefNum = -1 ;
		}
	
	return ( theErr == noErr ? kQ3Success : kQ3Failure ) ;
	}





//=============================================================================
//      e3storage_mac_fsspec_close : Close the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_fsspec_close ( E3FSSpecStorage* theStorage )
	{
	// Close the file
	if ( theStorage->macStorageData.fsRefNum != -1 )
		{
		// Flush the file
		if ( e3storage_mac_hasFlag( theStorage, kQ3MacStorage_BufferDirtyFlag ) )
			e3storage_mac_flushbuffer ( theStorage ) ;
		

		
		// Close it
		OSStatus theErr = ::FSCloseFork ( theStorage->macStorageData.fsRefNum ) ;
		theStorage->macStorageData.fsRefNum = -1 ;
		
		if ( theErr != noErr )
			{
			E3ErrorManager_PostPlatformError ( theErr ) ;
			return kQ3Failure ;
			}

		
		
		e3storage_mac_clearFlag ( theStorage, kQ3MacStorage_IsOpenFlag ) ;
		}

	return kQ3Success ;
	}





//=============================================================================
//      e3storage_mac_fsspec_delete : Delete the storage object.
//-----------------------------------------------------------------------------
static void
e3storage_mac_fsspec_delete ( E3FSSpecStorage* storage, void *privateData )
	{
#pragma unused(privateData)
	// storage MUST be closed
	
	if ( ( storage->macStorageData.flags & kQ3MacStorage_IsOpenFlag ) != 0 )
		E3ErrorManager_PostError ( kQ3ErrorStorageIsOpen, kQ3True ) ;
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
//      e3storage_mac_handle_duplicate : Handle storage duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_handle_duplicate(	TQ3Object fromObject, const void *fromPrivateData,
							TQ3Object toObject,   void       *toPrivateData)
{
	TQ3Status	theStatus = kQ3Success;
	const TQ3HandleStorageData*	fromInstanceData = (const TQ3HandleStorageData *) fromPrivateData;
	TQ3HandleStorageData *		toInstanceData   = (TQ3HandleStorageData *)       toPrivateData;
#pragma unused(fromObject)
#pragma unused(toObject)
	
	toInstanceData->ownHandle = fromInstanceData->ownHandle;
	toInstanceData->theSize = fromInstanceData->theSize;
	
	if (toInstanceData->ownHandle)
	{
		OSErr						theErr;
		toInstanceData->theHnd = TempNewHandle( toInstanceData->theSize, &theErr );
		if (toInstanceData->theHnd == NULL)
		{
			theStatus = kQ3Failure;
		}
		else
		{
			Q3Memory_Copy( *fromInstanceData->theHnd, *toInstanceData->theHnd,
				toInstanceData->theSize );
		}
	}
	else
	{
		toInstanceData->theHnd = fromInstanceData->theHnd;
	}
	return theStatus;
}





//=============================================================================
//      e3storage_mac_handle_getsize : Get the size of the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_handle_getsize ( E3HandleStorage* storage, TQ3Uns32 *size )
	{
	// Return our size
	*size = storage->instanceData.theSize ;
	
	return kQ3Success ;
	}





//=============================================================================
//      e3storage_mac_handle_read : Read data from the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_handle_read ( E3HandleStorage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead )
	{
	// Initialise a return value
	*sizeRead = 0 ;



	// Make sure we have enough space
	if ( offset >= storage->instanceData.theSize )
		return kQ3Failure ;



	// Work out how much we should copy
	TQ3Uns32 bytesToRead = dataSize ;
	if ( offset + bytesToRead > storage->instanceData.theSize )
		bytesToRead = storage->instanceData.theSize - offset ;



	// Copy the block.
	::memmove ( data, ( *storage->instanceData.theHnd ) + offset, bytesToRead ) ;
	
	*sizeRead = bytesToRead ;
	
	return kQ3Success ;
	}





//=============================================================================
//      e3storage_mac_handle_write : Write data to the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_mac_handle_write ( E3HandleStorage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten )
	{
	// Initialise a return value
	*sizeWritten = 0 ;
	TQ3Uns32 bytesToWrite = dataSize ;



	// Make sure we have enough space
	// Grow the handle if we need to
	TQ3Uns32 newSize = offset + bytesToWrite ;
	if ( newSize > storage->instanceData.theSize )
		{
		::SetHandleSize ( storage->instanceData.theHnd, (SInt32) newSize ) ;
		if ( MemError () != noErr )
			return kQ3Failure ;

		storage->instanceData.theSize = newSize ;
		}


	// Copy the block.
	::memmove ( (*storage->instanceData.theHnd) + offset, data, bytesToWrite ) ;
	
	*sizeWritten = bytesToWrite ;
	
	return kQ3Success ;
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

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3storage_mac_handle_duplicate;
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
	qd3dStatus = Q3_REGISTER_CLASS_WITH_DATA (	kQ3ClassNameStorageMacintosh,
										e3storage_mac_metahandler,
										E3MacintoshStorage,
										sizeof(TE3_MacStorageData) ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_WITH_DATA (	kQ3ClassNameStorageFSSpec,
											e3storage_mac_fsspec_metahandler,
											E3FSSpecStorage,
											sizeof(FSRef) ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameStorageHandle,
											e3storage_mac_handle_metahandler,
											E3HandleStorage ) ;

	return(qd3dStatus);
}





//=============================================================================
//      E3Storage_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3MacStorage_UnregisterClass(void)
{



	// Unregister the classes
	E3ClassTree::UnregisterClass(kQ3MemoryStorageTypeHandle,    kQ3True);
	E3ClassTree::UnregisterClass(kQ3MacintoshStorageTypeFSSpec, kQ3True);
	E3ClassTree::UnregisterClass(kQ3StorageTypeMacintosh,       kQ3True);

	return(kQ3Success);
}





//=============================================================================
//      E3HandleStorage_New : Create a Handle storage object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StorageObject
E3HandleStorage_New(Handle handle, TQ3Uns32 validSize)
	{
	TQ3HandleStorageData		storageData ;



	// Create the object
	storageData.ownHandle = kQ3False;
	storageData.theSize   = validSize;
	storageData.theHnd    = handle;

	return E3ClassTree::CreateInstance(kQ3MemoryStorageTypeHandle, kQ3False, &storageData);
	}





//=============================================================================
//      E3HandleStorage_Set : Set the Handle for a Handle storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3HandleStorage_Set(TQ3StorageObject storage, Handle handle, TQ3Uns32 validSize)
	{
	// If we own our buffer, get rid of it. Either way, reset our state
	if ( ( (E3HandleStorage*) storage )->instanceData.ownHandle )
		::DisposeHandle ( ( (E3HandleStorage*) storage )->instanceData.theHnd ) ;

	Q3Memory_Clear ( & ( (E3HandleStorage*) storage )->instanceData, sizeof( TQ3HandleStorageData ) ) ;



	// If we're expected to create a new handle, create one
	if ( handle == NULL )
		{
		OSErr theErr ;
		( (E3HandleStorage*) storage )->instanceData.theHnd = ::TempNewHandle ( (SInt32) validSize, &theErr ) ;
		if ( ( (E3HandleStorage*) storage )->instanceData.theHnd == NULL || theErr != noErr )
			return kQ3Failure ;

		( (E3HandleStorage*) storage )->instanceData.ownHandle = kQ3True ;
		( (E3HandleStorage*) storage )->instanceData.theSize   = validSize ;
		}
	
	
	
	// Otherwise, save the supplied handle
	else
		{
		( (E3HandleStorage*) storage )->instanceData.ownHandle = kQ3False ;
		( (E3HandleStorage*) storage )->instanceData.theSize   = validSize ;
		( (E3HandleStorage*) storage )->instanceData.theHnd    = handle ;
		}

	Q3Shared_Edited ( storage ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3HandleStorage_Get : Get info about a Handle storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3HandleStorage_Get(TQ3StorageObject storage, Handle *handle, TQ3Uns32 *validSize)
	{
	// Return the information
	*handle    = ( (E3HandleStorage*) storage )->instanceData.theHnd ;
	*validSize = ( (E3HandleStorage*) storage )->instanceData.theSize ;

	return kQ3Success ;
	}





//=============================================================================
//      E3MacintoshStorage_New : Create a new Macintosh storage object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StorageObject
E3MacintoshStorage_New(TQ3Int16 fsRefNum)
	{
	// Create the object
	return E3ClassTree::CreateInstance(kQ3StorageTypeMacintosh, kQ3False, &fsRefNum);
	}





//=============================================================================
//      E3MacintoshStorage_Set : Set the information for a Mac storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3MacintoshStorage_Set(TQ3StorageObject storage, TQ3Int16 fsRefNum)
	{
	//parameters has been tested by Q3MacintoshStorage_Set
	
	// storage MUST be closed
	if ( e3storage_mac_hasFlag ( (E3MacintoshStorage*) storage , kQ3MacStorage_IsOpenFlag ) )
		{
		E3ErrorManager_PostError  (kQ3ErrorStorageIsOpen, kQ3False ) ;
		return kQ3Failure ;
		}
	

	( (E3MacintoshStorage*) storage )->macStorageData.fsRefNum = fsRefNum ;

	Q3Shared_Edited ( storage ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3MacintoshStorage_Get : Get the information for a Mac storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3MacintoshStorage_Get(TQ3StorageObject storage, TQ3Int16 *fsRefNum)
	{
	//parameters has been tested by Q3MacintoshStorage_Get

	*fsRefNum = ( (E3MacintoshStorage*) storage )->macStorageData.fsRefNum ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3MacintoshStorage_GetType : Return the type of a Mac storage object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3MacintoshStorage_GetType(TQ3StorageObject storage)
	{
	// Return the type
	return storage->GetObjectType ( kQ3StorageTypeMacintosh ) ;
	}





//=============================================================================
//      E3FSSpecStorage_New : Create a new FSSpec storage object.
//-----------------------------------------------------------------------------
#pragma mark -

// FSSpec is not available in 64bit.
#if QUESA_SUPPORT_HITOOLBOX

TQ3StorageObject
E3FSSpecStorage_New(const FSSpec *fs)
	{
	// Create the object
	FSRef	theFSRef;
	FSpMakeFSRef( fs, &theFSRef );
	return E3ClassTree::CreateInstance(kQ3MacintoshStorageTypeFSSpec, kQ3False, &theFSRef );
	}





//=============================================================================
//      E3FSSpecStorage_Set : Set the information for an FSSpec storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3FSSpecStorage_Set(TQ3StorageObject theStorage, const FSSpec *theFSSpec)
	{
	// Make sure we're not open
	if ( e3storage_mac_hasFlag( (E3MacintoshStorage*) theStorage, kQ3MacStorage_IsOpenFlag ) )
		{
		E3ErrorManager_PostError ( kQ3ErrorStorageIsOpen, kQ3False ) ;
		return kQ3Failure ;
		}



	// Set the data
	FSpMakeFSRef( theFSSpec, & ( (E3FSSpecStorage*) theStorage )->theFSRef );
	Q3Shared_Edited ( theStorage ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3FSSpecStorage_Get : Get the information for an FSSpec storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3FSSpecStorage_Get(TQ3StorageObject theStorage, FSSpec *theFSSpec)
	{
	// Get the data
	FSGetCatalogInfo( & ( (E3FSSpecStorage*) theStorage )->theFSRef,
		kFSCatInfoNone, NULL, NULL, theFSSpec, NULL );
	
	return kQ3Success ;
	}

#endif // QUESA_SUPPORT_HITOOLBOX



//=============================================================================
//      E3FSRefStorage_New : Create a new FSRef storage object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3StorageObject
E3FSRefStorage_New(const FSRef *fs)
{
	return E3ClassTree::CreateInstance(kQ3MacintoshStorageTypeFSSpec, kQ3False, fs );
}





//=============================================================================
//      E3FSSpecStorage_Set : Set the information for an FSRef storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3FSRefStorage_Set(TQ3StorageObject storage, const FSRef *fs)
{
	// Make sure we're not open
	if ( e3storage_mac_hasFlag( (E3MacintoshStorage*) storage, kQ3MacStorage_IsOpenFlag ) )
	{
		E3ErrorManager_PostError ( kQ3ErrorStorageIsOpen, kQ3False ) ;
		return kQ3Failure ;
	}



	// Set the data
	( (E3FSSpecStorage*) storage )->theFSRef = *fs;
	Q3Shared_Edited ( storage ) ;
	
	return kQ3Success ;
}





//=============================================================================
//      E3FSRefStorage_Get : Get the information for an FSRef storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3FSRefStorage_Get(TQ3StorageObject storage, FSRef *fs)
{
	// Get the data
	*fs = ( (E3FSSpecStorage*) storage )->theFSRef;
	
	return kQ3Success ;
}


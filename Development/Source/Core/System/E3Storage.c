/*  NAME:
        E3Storage.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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

#include <stdio.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kE3MemoryStorageDefaultGrowSize					1024
#define kE3MemoryStorageMinimumGrowSize					32





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      E3StorageInfo::E3StorageInfo : Constructor for class info of the class.
//-----------------------------------------------------------------------------

E3StorageInfo::E3StorageInfo	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
		: E3SharedInfo ( newClassMetaHandler, newParent ) ,
		getData_Method		( (TQ3XStorageReadDataMethod)		Find_Method ( kQ3XMethodTypeStorageReadData ) ) ,
		setData_Method		( (TQ3XStorageWriteDataMethod)		Find_Method ( kQ3XMethodTypeStorageWriteData ) ) ,
		getEOF_Method		( (TQ3XStorageGetSizeMethod)		Find_Method ( kQ3XMethodTypeStorageGetSize ) )	
		 	 
	{
	if ( getData_Method == NULL
	|| setData_Method == NULL
	|| getEOF_Method == NULL )
		SetAbstract () ;
	} ; 


//=============================================================================
//      e3transform_new_class_info : Method to construct a class info record.
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3storage_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent
			 	)
	{
	return new ( std::nothrow ) E3StorageInfo ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3storage_memory_read : Read data from the storage object.
//-----------------------------------------------------------------------------
TQ3Status
e3storage_memory_read ( E3MemoryStorage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead )
	{	
	*sizeRead = 0 ;
	
	if ( offset >= storage->memoryDetails.validSize )
		return kQ3Failure ;
		
	TQ3Uns32 bytesToRead = dataSize ;
	
	if ( offset + bytesToRead > storage->memoryDetails.validSize )
		bytesToRead = storage->memoryDetails.validSize - offset ;
		
	Q3Memory_Copy ( & storage->memoryDetails.buffer [ offset ], data, bytesToRead ) ;
	
	*sizeRead = bytesToRead ;
	
	return kQ3Success ;
	}



//=============================================================================
//      e3storage_memory_grow : Try to grow the storage 
//-----------------------------------------------------------------------------
//	it does it if it owns the storage,
//  tries to allocate at least the requestedSize and do it in instanceData->growSize chunks
//-----------------------------------------------------------------------------
TQ3Status
e3storage_memory_grow ( E3MemoryStorage* storage, TQ3Uns32 requestedSize )
	{	
	if ( ( storage->memoryDetails.ownBuffer != kQ3False )
	&& ( requestedSize > storage->memoryDetails.bufferSize ) )
		{
		// Grow at least to twice the previous size.
		TQ3Uns32 expSize = storage->memoryDetails.bufferSize * 2 ;
		TQ3Uns32 newSize = E3Num_Max( requestedSize, expSize ) ;

		// Round up to next multiple of growSize.
		newSize = ( ( newSize / storage->memoryDetails.growSize ) + 1 ) * storage->memoryDetails.growSize ;
		
		TQ3Status qd3dStatus = Q3Memory_Reallocate( & storage->memoryDetails.buffer, newSize ) ;
		if ( qd3dStatus == kQ3Failure )
			return kQ3Failure ;
		
		storage->memoryDetails.bufferSize = newSize ;
		}
	
	return kQ3Success ;
	}




//=============================================================================
//      e3storage_memory_write : Write data to the storage object.
//-----------------------------------------------------------------------------
TQ3Status
e3storage_memory_write ( E3MemoryStorage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten )
	{
	// Initialise a return value
	*sizeWritten = 0 ;
	TQ3Uns32 bytesToWrite = dataSize ;

  
	// Try to grow the buffer
	if ( offset + bytesToWrite > storage->memoryDetails.bufferSize )
		if ( e3storage_memory_grow ( storage, offset + bytesToWrite ) == kQ3Failure )
			return kQ3Failure ;



	// Make sure we have enough space to write something
	if ( ( bytesToWrite > 0 ) && ( offset  >= storage->memoryDetails.bufferSize ) )
		return kQ3Failure ;



	// Work out how much we should copy
	if ( offset + bytesToWrite > storage->memoryDetails.bufferSize )
		bytesToWrite = storage->memoryDetails.bufferSize - offset ;



	// Copy the block
	if ( bytesToWrite > 0 )
		Q3Memory_Copy ( data, & storage->memoryDetails.buffer [ offset ], bytesToWrite ) ;
	
	
	if ( storage->memoryDetails.validSize < offset + bytesToWrite ) // shift EOF
		storage->memoryDetails.validSize = offset + bytesToWrite ;
	
	*sizeWritten = bytesToWrite ;
	
	return kQ3Success ;
	}





//=============================================================================
//      e3storage_memory_getsize : Get the size of the storage object.
//-----------------------------------------------------------------------------
TQ3Status
e3storage_memory_getsize ( E3MemoryStorage* storage, TQ3Uns32 *size )
	{
	*size = storage->memoryDetails.validSize ;
	
	return kQ3Success ;
	}





//=============================================================================
//      e3storage_memory_new : New method.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_memory_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TE3_MemoryStorageData	*instanceData  = (TE3_MemoryStorageData *) privateData;
	TQ3Uns8					*passedBuffer;
	
	// The only case in which we will be called with NULL for paramData is when
	// an object of type kQ3MemoryStorageTypeHandle is being constructed.
	// Handle storage is a subclass of memory storage only superfically, i.e.,
	// it has a completely separate implementation.  Therefore we may as well
	// leave the initial zero values in the memory storage instance data.
	if (paramData == NULL)
		return kQ3Success;
	
	*instanceData = *((const TE3_MemoryStorageData *)paramData);

	if (instanceData->ownBuffer == kQ3True){
		// called from _New
		if ( instanceData->buffer != NULL ){
			// copy the buffer
			Q3_ASSERT(instanceData->bufferSize != 0);
			passedBuffer = instanceData->buffer;
			instanceData->buffer = (TQ3Uns8*)Q3Memory_Allocate( instanceData->bufferSize ) ;

			if (instanceData->buffer == NULL){
				return(kQ3Failure);						
				}
			Q3Memory_Copy( passedBuffer, instanceData->buffer, instanceData->bufferSize );
			instanceData->validSize = instanceData->bufferSize;
			instanceData->growSize = kE3MemoryStorageDefaultGrowSize;
			}
		else{
			// called with buffer == NULL, allocate our own
			// check validSize parameter
			if(instanceData->validSize < kE3MemoryStorageMinimumGrowSize){
				instanceData->growSize = kE3MemoryStorageDefaultGrowSize;
				}
			else{
				instanceData->growSize = instanceData->validSize;
				}
			
			instanceData->validSize = 0L;
				
			instanceData->buffer = (TQ3Uns8*)Q3Memory_Allocate( instanceData->growSize ) ;

			if (instanceData->buffer == NULL){
				instanceData->bufferSize = 0L;
				return(kQ3Failure);						
				}
			instanceData->bufferSize = instanceData->growSize;
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
//      e3storage_memory_duplicate : Memory storage duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_memory_duplicate(	TQ3Object fromObject, const void *fromPrivateData,
							TQ3Object toObject,   void       *toPrivateData)
{
	TQ3Status	theStatus = kQ3Success;
	const TE3_MemoryStorageData*	fromInstanceData = (const TE3_MemoryStorageData *) fromPrivateData;
	TE3_MemoryStorageData *			toInstanceData   = (TE3_MemoryStorageData *)       toPrivateData;
#pragma unused(fromObject)
#pragma unused(toObject)
	
	toInstanceData->ownBuffer = fromInstanceData->ownBuffer;
	toInstanceData->bufferSize = fromInstanceData->bufferSize;
	toInstanceData->validSize = fromInstanceData->validSize;
	toInstanceData->growSize = fromInstanceData->growSize;
	
	if (toInstanceData->ownBuffer)
	{
		toInstanceData->buffer = (TQ3Uns8*) Q3Memory_Allocate( toInstanceData->bufferSize );
		if (toInstanceData->buffer == NULL)
		{
			theStatus = kQ3Failure;
		}
		else
		{
			Q3Memory_Copy( fromInstanceData->buffer, toInstanceData->buffer,
				toInstanceData->bufferSize );
		}
	}
	else
	{
		toInstanceData->buffer = fromInstanceData->buffer;
	}
	return theStatus;
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
		
		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3storage_memory_duplicate;
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
	pathLen = static_cast<TQ3Uns32>(strlen(thePath));
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
TQ3Status
e3storage_path_open ( E3PathStorage* storage, TQ3Boolean forWriting )
	{
	// Make sure the file isn't already open
	if ( storage->pathDetails.theFile != NULL )
		{
		E3ErrorManager_PostError ( kQ3ErrorFileAlreadyOpen, kQ3False ) ;
		return kQ3Failure ;
		}



	// Open the file		
	storage->pathDetails.theFile = fopen ( storage->pathDetails.thePath, forWriting ? "wb+" : "rb" ) ;
	if ( storage->pathDetails.theFile == NULL )
		return kQ3Failure ;
	
	return kQ3Success ;
	}





//=============================================================================
//      e3storage_path_close : Close the storage object.
//-----------------------------------------------------------------------------
TQ3Status
e3storage_path_close ( E3PathStorage* storage )
	{
	// Make sure the file is open
	if ( storage->pathDetails.theFile == NULL )
		{
		E3ErrorManager_PostError ( kQ3ErrorFileNotOpen, kQ3False ) ;
		return kQ3Failure ;
		}



	// Close the file		
	fclose ( storage->pathDetails.theFile ) ;
	storage->pathDetails.theFile = NULL ;

	return kQ3Success ;
	}





//=============================================================================
//      e3storage_path_getsize : Get the size of the storage object.
//-----------------------------------------------------------------------------
TQ3Status
e3storage_path_getsize ( E3PathStorage* storage, TQ3Uns32 *size )
	{
	fpos_t					oldPos;



	// Make sure the file is open
	if ( storage->pathDetails.theFile == NULL )
		{
		E3ErrorManager_PostError ( kQ3ErrorFileNotOpen, kQ3False ) ;
		return kQ3Failure ;
		}



	// Get the current position in the file
	if ( fgetpos ( storage->pathDetails.theFile, &oldPos ) )
		return kQ3Failure ;



	// Seek to the end and get the position there. Not that using ftell rather
	// than fgetpos limits us to 2147483647 byte files, but casting an fpos_t
	// to a 32-bit integer is not valid on some Unix systems.
	if ( fseek ( storage->pathDetails.theFile, 0, SEEK_END ) )
		return kQ3Failure ;

	*size = (TQ3Uns32) ftell ( storage->pathDetails.theFile ) ;

	if ( fseek ( storage->pathDetails.theFile, 0, SEEK_SET ) )
		return kQ3Failure ;



	// Restore the previous position in the file
	if ( fsetpos ( storage->pathDetails.theFile, &oldPos ) )
		return kQ3Failure ;

	return kQ3Success ;
	}





//=============================================================================
//      e3storage_path_read : Read data from the storage object.
//-----------------------------------------------------------------------------
//		Note : Currently unbuffered - may cause performance problems.
//-----------------------------------------------------------------------------
TQ3Status
e3storage_path_read ( E3PathStorage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead )
	{
	// Make sure the file is open
	if ( storage->pathDetails.theFile == NULL )
		{
		E3ErrorManager_PostError ( kQ3ErrorFileNotOpen, kQ3False ) ;
		return kQ3Failure ;
		}



	// Seek to the offset, and read the data
	// (The ftell is needed because in the Windows version of
	// CodeWarrior's standard library, fseek always flushes the buffer.)
	if ( (TQ3Int32) offset != ftell ( storage->pathDetails.theFile ) )
		{
		if ( fseek ( storage->pathDetails.theFile, (long) offset, SEEK_SET ) )
			return kQ3Failure ;
		}

	*sizeRead = static_cast<TQ3Uns32>(fread ( data, 1, dataSize, storage->pathDetails.theFile ));

	return kQ3Success ;
	}





//=============================================================================
//      e3storage_path_write : Write data to the storage object.
//-----------------------------------------------------------------------------
//		Note : Currently unbuffered - may cause performance problems.
//-----------------------------------------------------------------------------
TQ3Status
e3storage_path_write ( E3PathStorage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten )
	{
	// Make sure the file is open
	if ( storage->pathDetails.theFile == NULL )
		{
		E3ErrorManager_PostError ( kQ3ErrorFileNotOpen, kQ3False ) ;
		return kQ3Failure ;
		}



	// Seek to the offset, and write the data
	if ( fseek ( storage->pathDetails.theFile, (long)offset, SEEK_SET ) )
		return kQ3Failure ;

	*sizeWritten = static_cast<TQ3Uns32>(fwrite( data, 1, dataSize, storage->pathDetails.theFile ));

	return kQ3Success ;
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
//      e3storage_stream_new : Stream storage new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_stream_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	FILE** instanceData = (FILE**) privateData;
	FILE				*theStream      = (FILE *) paramData;

	// Initialise our instance data
	*instanceData = theStream;
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_stream_getsize : Get the size of the storage object.
//-----------------------------------------------------------------------------
TQ3Status
e3storage_stream_getsize ( E3FileStreamStorage* storage, TQ3Uns32 *size )
{
	fpos_t					oldPos;



	// Make sure the file is open
	if ( storage->mStream == NULL )
	{
		E3ErrorManager_PostError( kQ3ErrorFileNotOpen, kQ3False );
		return kQ3Failure;
	}



	// Get the current position in the file
	if ( fgetpos( storage->mStream, &oldPos ) )
		return kQ3Failure;



	// Seek to the end and get the position there. Note that using ftell rather
	// than fgetpos limits us to 2147483647 byte files, but casting an fpos_t
	// to a 32-bit integer is not valid on some Unix systems.
	if ( fseek( storage->mStream, 0, SEEK_END ) )
		return kQ3Failure;

	*size = (TQ3Uns32) ftell( storage->mStream ) ;

	if ( fseek( storage->mStream, 0, SEEK_SET ) )
		return kQ3Failure ;



	// Restore the previous position in the file
	if ( fsetpos( storage->mStream, &oldPos ) )
		return kQ3Failure ;

	return kQ3Success;
}





//=============================================================================
//      e3storage_stream_read : Read data from the storage object.
//-----------------------------------------------------------------------------
//		Note : Currently unbuffered - may cause performance problems.
//-----------------------------------------------------------------------------
TQ3Status
e3storage_stream_read( E3FileStreamStorage* storage, TQ3Uns32 offset,
						TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead )
{
	// Make sure the file is open
	if ( storage->mStream == NULL )
	{
		E3ErrorManager_PostError( kQ3ErrorFileNotOpen, kQ3False );
		return kQ3Failure;
	}



	// Seek to the offset, and read the data
	// (The ftell is needed because in the Windows version of
	// CodeWarrior's standard library, fseek always flushes the buffer.)
	if ( (TQ3Int32) offset != ftell( storage->mStream ) )
	{
		if ( fseek( storage->mStream, (long) offset, SEEK_SET ) )
			return kQ3Failure;
	}

	*sizeRead = static_cast<TQ3Uns32>(fread( data, 1, dataSize, storage->mStream ));

	return kQ3Success;
}





//=============================================================================
//      e3storage_stream_write : Write data to the storage object.
//-----------------------------------------------------------------------------
//		Note : Currently unbuffered - may cause performance problems.
//-----------------------------------------------------------------------------
TQ3Status
e3storage_stream_write( E3FileStreamStorage* storage, TQ3Uns32 offset,
						TQ3Uns32 dataSize, const unsigned char *data,
						TQ3Uns32 *sizeWritten )
{
	// Make sure the file is open
	if ( storage->mStream == NULL )
	{
		E3ErrorManager_PostError( kQ3ErrorFileNotOpen, kQ3False );
		return kQ3Failure;
	}



	// Seek to the offset, and write the data
	if ( fseek( storage->mStream, (long)offset, SEEK_SET ) )
		return kQ3Failure;

	*sizeWritten = static_cast<TQ3Uns32>(fwrite( data, 1, dataSize, storage->mStream ));

	return kQ3Success ;
}





//=============================================================================
//      e3storage_stream_metahandler : Stream storage metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3storage_stream_metahandler(TQ3XMethodType methodType)
{
	TQ3XFunctionPointer		theMethod = NULL;
	
	switch (methodType)
	{
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3storage_stream_new;
			break;

		case kQ3XMethodTypeStorageGetSize:
			theMethod = (TQ3XFunctionPointer) e3storage_stream_getsize;
			break;

		case kQ3XMethodTypeStorageReadData:
			theMethod = (TQ3XFunctionPointer) e3storage_stream_read;
			break;

		case kQ3XMethodTypeStorageWriteData:
			theMethod = (TQ3XFunctionPointer) e3storage_stream_write;
			break;
	}
	
	return theMethod;
}



//=============================================================================
//      e3storage_metahandler : base metahandler for storage classes.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3storage_metahandler(TQ3XMethodType methodType)
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeNewObjectClass :
			return (TQ3XFunctionPointer) e3storage_new_class_info ;

		}
	
	return NULL ;
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
	qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (	kQ3ClassNameStorage,
										e3storage_metahandler,
										E3Storage ) ;


	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_WITH_DATA (	kQ3ClassNameStorageMemory,
											e3storage_memory_metahandler,
											E3MemoryStorage,
											sizeof(TE3_MemoryStorageData) ) ;


	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_WITH_DATA (	kQ3ClassNameStoragePath,
											e3storage_path_metahandler,
											E3PathStorage,
											sizeof(TQ3PathStorageData) ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_WITH_DATA (	kQ3ClassNameStorageStream,
											e3storage_stream_metahandler,
											E3FileStreamStorage,
											sizeof(FILE*) ) ;



	// Register the platform specific classes
	if (qd3dStatus == kQ3Success)
		{
#if QUESA_OS_MACINTOSH
		qd3dStatus = E3MacStorage_RegisterClass();
#elif QUESA_OS_UNIX
		qd3dStatus = E3UnixStorage_RegisterClass();
#elif QUESA_OS_WIN32
		qd3dStatus = E3Win32Storage_RegisterClass();
#endif
		}

	return(qd3dStatus);
}





//=============================================================================
//      E3Storage_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Storage_UnregisterClass(void)
{


	E3ClassTree::UnregisterClass(kQ3SharedTypeStorage, kQ3True);
	E3ClassTree::UnregisterClass(kQ3StorageTypeMemory, kQ3True);
	E3ClassTree::UnregisterClass(kQ3StorageTypePath,   kQ3True);
	E3ClassTree::UnregisterClass(kQ3StorageTypeFileStream,   kQ3True);


#if QUESA_OS_MACINTOSH
	E3MacStorage_UnregisterClass();
#elif QUESA_OS_UNIX
	E3UnixStorage_UnregisterClass();
#elif QUESA_OS_WIN32
	E3Win32Storage_UnregisterClass();
#endif
	
	return(kQ3Success);
}





//=============================================================================
//      E3Storage::IsOfMyClass : Check if object pointer is valid and of type Storage
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3SharedTypeStorage )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3Storage::IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3Storage ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3Storage_GetType : Return the type of a storage object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Storage::GetType ( void )
	{
	// Return the type
	return GetObjectType ( kQ3SharedTypeStorage ) ;
	}





//=============================================================================
//      E3Storage_GetSize : Return the size of data in a storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3Storage::GetSize ( TQ3Uns32* size )
	{
	return GetClass ()->getEOF_Method ( this, size ) ;
	}





//=============================================================================
//      E3Storage_GetData : Return the data in a storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3Storage::GetData ( TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char* data, TQ3Uns32* sizeRead )
	{
	return GetClass ()->getData_Method ( this, offset, dataSize, (TQ3Uns8*) data, sizeRead ) ;
	}





//=============================================================================
//      E3Storage_SetData : Set the data for a storage object.
//-----------------------------------------------------------------------------
//		Note : Under normal conditions SetData always returns dataSize ==
//			   sizeWritten, the storage grows to accept new data
//-----------------------------------------------------------------------------
TQ3Status
E3Storage::SetData ( TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char* data, TQ3Uns32* sizeWritten )
	{
	TQ3Status result = GetClass ()->setData_Method ( this, offset, dataSize, (TQ3Uns8*) data, sizeWritten ) ;

	Edited () ;
	
	return result ;
	}





//=============================================================================
//      E3MemoryStorage_GetType : Return the type of a memory storage object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
E3MemoryStorage_GetType(TQ3StorageObject storage)
	{
	// Return the type
	return storage->GetObjectType ( kQ3StorageTypeMemory ) ;
	}





//=============================================================================
//      E3MemoryStorage_New : Creates a new storage copying the buffer.
//-----------------------------------------------------------------------------
TQ3StorageObject
E3MemoryStorage_New(const unsigned char *buffer, TQ3Uns32 validSize)
	{
	TE3_MemoryStorageData	objectData ;
	
	objectData.buffer = (TQ3Uns8 *)buffer ;
	objectData.ownBuffer = kQ3True ;
	objectData.bufferSize = validSize ;
	objectData.validSize = validSize ;
	objectData.growSize = kE3MemoryStorageDefaultGrowSize ;


	// Create the object
	return E3ClassTree::CreateInstance ( kQ3StorageTypeMemory, kQ3False, &objectData ) ;
	}





//=============================================================================
//      E3MemoryStorage_Set : Sets the buffer for a storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3MemoryStorage::Set ( const unsigned char *buffer, TQ3Uns32 validSize )
	{
	// If we don't own our buffer, reset our state
	if ( ! memoryDetails.ownBuffer )
		{
		Q3Memory_Clear ( &memoryDetails, sizeof ( TE3_MemoryStorageData ) ) ;
		memoryDetails.ownBuffer = kQ3True ;
		}



	// If we need to grow our buffer, do so
	if ( memoryDetails.bufferSize < validSize )
		{
		TQ3Status qd3dStatus = Q3Memory_Reallocate ( & memoryDetails.buffer, validSize ) ;
		if ( qd3dStatus == kQ3Failure )
			return kQ3Failure ;
		
		memoryDetails.bufferSize = validSize ;
		}



	// Verify that we do have a buffer, we own it, and it's large enough
	Q3_ASSERT(Q3_VALID_PTR( memoryDetails.buffer) ) ;
	Q3_ASSERT( memoryDetails.ownBuffer ) ;
	Q3_ASSERT( memoryDetails.bufferSize >= validSize ) ;



	// Copy the data into the buffer
	if ( buffer != NULL) 
		Q3Memory_Copy ( buffer, memoryDetails.buffer, validSize ); 

	memoryDetails.validSize = validSize ;
	memoryDetails.growSize  = kE3MemoryStorageDefaultGrowSize ;

	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3MemoryStorage_NewBuffer : Creates a new storage using the buffer.
//-----------------------------------------------------------------------------
TQ3StorageObject
E3MemoryStorage_NewBuffer(unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize)
	{
	TE3_MemoryStorageData	objectData ;
	
	objectData.buffer = (TQ3Uns8*) buffer ;
	objectData.ownBuffer = kQ3False ;
	objectData.bufferSize = bufferSize ;
	objectData.validSize = validSize ;
	objectData.growSize = validSize ;



	// Create the object
	return E3ClassTree::CreateInstance ( kQ3StorageTypeMemory, kQ3False, &objectData ) ;
	}





//=============================================================================
//      E3MemoryStorage_SetBuffer : Set the data for a storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3MemoryStorage::SetBuffer ( unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize )
	{
	TQ3Status				qd3dStatus;



	// If no buffer has been supplied, use our own
	if ( buffer == NULL )
		qd3dStatus = Q3MemoryStorage_Set ( this, buffer, validSize ) ;



	// Otherwise, use the supplied buffer
	else
		{
		// First check to see if we currently own the buffer...
		if ( memoryDetails.ownBuffer == kQ3True )
			{
			// If the buffer has changed, we're no longer going to own it so we need to
			// release it. If the buffer is the same, we don't want to do anything - we're
			// just being called because the app has touched the contents and is calling
			// us for the side effect of calling Q3Shared_Edited).
			if ( memoryDetails.buffer != (TQ3Uns8*) buffer )
				{
				// Make sure the app isn't trying to change anything they shouldn't: since
				// we own the buffer, the app shouldn't be trying to resize it on us.
				Q3_ASSERT( memoryDetails.bufferSize == bufferSize ) ;
				Q3_ASSERT( memoryDetails.validSize  == validSize ) ;


				// Release the buffer
				memoryDetails.ownBuffer = kQ3False;
				Q3Memory_Free(  & memoryDetails.buffer ) ;
				}
			}



		// Update our state. Note that we leave ownBuffer unchanged: we either reset
		// it to false above (in which case the new buffer is owned by the app), or
		// the previous owner still owns it.
		memoryDetails.buffer     = (TQ3Uns8*) buffer ;
		memoryDetails.bufferSize = bufferSize ;
		memoryDetails.validSize  = validSize ;
		memoryDetails.growSize   = kE3MemoryStorageDefaultGrowSize ;
		
		qd3dStatus = kQ3Success;
		}

	Q3Shared_Edited ( this ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3MemoryStorage_GetBuffer : Return info about the buffer.
//-----------------------------------------------------------------------------
//		Note :	We need to handle NULL parameters, since the QD3D Interactive
//				Renderer calls us with some parameters as NULL.
//-----------------------------------------------------------------------------
TQ3Status
E3MemoryStorage::GetBuffer ( unsigned char **buffer, TQ3Uns32 *validSize, TQ3Uns32 *bufferSize )
	{
	if ( buffer != NULL )
		*buffer = (unsigned char *) memoryDetails.buffer ;
	if ( validSize != NULL )
		*validSize = memoryDetails.validSize ;
	if ( bufferSize != NULL )
		*bufferSize = memoryDetails.bufferSize ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3PathStorage_New : Create a path storage object.
//-----------------------------------------------------------------------------
TQ3StorageObject
E3PathStorage_New(const char *pathName)
	{
	// Create the object
	return E3ClassTree::CreateInstance ( kQ3StorageTypePath, kQ3False, pathName ) ;
	}





//=============================================================================
//      E3PathStorage_Set : Set the path for a path storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3PathStorage:: Set( const char *pathName )
	{
	// Take a copy of the new path
	TQ3Uns32 pathLen = static_cast<TQ3Uns32>( strlen( pathName) );
	char* newPath = (char*) Q3Memory_Allocate ( pathLen + 1 ) ;
	if ( newPath == NULL )
		return kQ3Failure ;



	// Clean up the instance data
	if ( pathDetails.thePath != NULL )
		Q3Memory_Free( & pathDetails.thePath ) ;

	if ( pathDetails.theFile != NULL )
		fclose ( pathDetails.theFile ) ;



	// Update the instance data
	pathDetails.thePath = newPath ;
	pathDetails.theFile = NULL ;

	return kQ3Success ;	
	}





//=============================================================================
//      E3PathStorage_Get : Get the path for a path storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3PathStorage::Get ( char *pathName )
	{
	// Return the path
	strncpy ( pathName, pathDetails.thePath, kQ3StringMaximumLength ) ;

	return kQ3Success ;
	}



//=============================================================================
//      E3FileStreamStorage_New : Create a stream storage object.
//-----------------------------------------------------------------------------
TQ3StorageObject
E3FileStreamStorage_New(FILE *stream)
{
	return E3ClassTree::CreateInstance ( kQ3StorageTypeFileStream, kQ3False, stream ) ;
}


//=============================================================================
//      E3FileStreamStorage::Set : Set the stream for a stream storage object.
//-----------------------------------------------------------------------------
void
E3FileStreamStorage::Set( FILE* stream )
{
	mStream = stream;
}


//=============================================================================
//      E3PathStorage_Get : Get the path for a path storage object.
//-----------------------------------------------------------------------------
FILE*
E3FileStreamStorage::Get()
{
	return mStream;
}

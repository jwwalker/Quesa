/*  NAME:
        E3WindowsStorage.c

    DESCRIPTION:
        Windows specific Storage calls.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
//      Internal types
//-----------------------------------------------------------------------------
// Windows storage
typedef struct TQ3Win32StorageData {
	HANDLE		theFile;
} TQ3Win32StorageData;





class E3Win32Storage : public E3Storage  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TQ3Win32StorageData						instanceData ;
	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3storage_win32_new : Win32 storage new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_win32_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3Win32StorageData		*instanceData = (TQ3Win32StorageData *) privateData;
	HANDLE					theFile       = (HANDLE) paramData;



	// Initialise our instance data
	instanceData->theFile = theFile;
	
	return(kQ3Success);
}





//=============================================================================
//      e3storage_win32_getsize : Get the size of the storage object.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_win32_getsize ( E3Win32Storage* storage, TQ3Uns32 *size )
	{
	// Make sure the file is open
	if (storage->instanceData.theFile == NULL)
		{
		E3ErrorManager_PostError(kQ3ErrorFileNotOpen, kQ3False);
		return(kQ3Failure);
		}



	// Get the file size
	DWORD fileSize = GetFileSize(storage->instanceData.theFile, NULL);
	if (fileSize == 0xFFFFFFFF)
		{
		*size = 0;
		return(kQ3Failure);
		}

	*size = (TQ3Uns32) fileSize;
	
	return(kQ3Success);
	}





//=============================================================================
//      e3storage_win32_read : Read data from the storage object.
//-----------------------------------------------------------------------------
//		Note : Currently unbuffered - may cause performance problems.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_win32_read ( E3Win32Storage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead )
	{
	// Make sure the file is open
	if (storage->instanceData.theFile == NULL)
		{
		E3ErrorManager_PostError(kQ3ErrorFileNotOpen, kQ3False);
		return(kQ3Failure);
		}



	// Seek to the offset
	DWORD newPos = SetFilePointer(storage->instanceData.theFile, offset, NULL, FILE_BEGIN);
	if (newPos == 0xFFFFFFFF)
		return(kQ3Failure);



	// Read the data	
	BOOL theResult = ReadFile(storage->instanceData.theFile, data, (DWORD) dataSize, (LPDWORD) sizeRead, NULL);
	if (!theResult)
		return(kQ3Failure);
	
	return(kQ3Success);
	}





//=============================================================================
//      e3storage_win32_write : Write data to the storage object.
//-----------------------------------------------------------------------------
//		Note : Currently unbuffered - may cause performance problems.
//-----------------------------------------------------------------------------
static TQ3Status
e3storage_win32_write ( E3Win32Storage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten )
	{
	// Make sure the file is open
	if (storage->instanceData.theFile == NULL)
		{
		E3ErrorManager_PostError(kQ3ErrorFileNotOpen, kQ3False);
		return(kQ3Failure);
		}



	// Seek to the offset
	DWORD newPos = SetFilePointer(storage->instanceData.theFile, offset, NULL, FILE_BEGIN);
	if (newPos == 0xFFFFFFFF)
		return(kQ3Failure);



	// Write the data	
	BOOL theResult = WriteFile(storage->instanceData.theFile, data, (DWORD) dataSize, (LPDWORD) sizeWritten, NULL);
	if (!theResult)
		return(kQ3Failure);
	
	return(kQ3Success);
	}





//=============================================================================
//      e3storage_win32_metahandler : Win32 storage metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3storage_win32_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3storage_win32_new;
			break;

		case kQ3XMethodTypeStorageGetSize:
			theMethod = (TQ3XFunctionPointer) e3storage_win32_getsize;
			break;

		case kQ3XMethodTypeStorageReadData:
			theMethod = (TQ3XFunctionPointer) e3storage_win32_read;
			break;

		case kQ3XMethodTypeStorageWriteData:
			theMethod = (TQ3XFunctionPointer) e3storage_win32_write;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Win32Storage_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Win32Storage_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree::RegisterClass(kQ3SharedTypeStorage,
											kQ3StorageTypeWin32,
											kQ3ClassNameStorageWin32,
											e3storage_win32_metahandler,
											~sizeof(E3Win32Storage));

	return(qd3dStatus);
}





//=============================================================================
//      E3Win32Storage_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Win32Storage_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3StorageTypeWin32, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Win32Storage_New : Create a Win32 storage object.
//-----------------------------------------------------------------------------
TQ3StorageObject
E3Win32Storage_New(HANDLE hFile)
	{
	// Create the object
	return E3ClassTree::CreateInstance ( kQ3StorageTypeWin32, kQ3False, hFile ) ;
	}





//=============================================================================
//      E3Win32Storage_Set : Set the handle for a Win32 storage.
//-----------------------------------------------------------------------------
TQ3Status
E3Win32Storage_Set(TQ3StorageObject storage, HANDLE hFile)
	{
	// Clean up the instance data
	if ( ( (E3Win32Storage*) storage )->instanceData.theFile != NULL )
		CloseHandle ( ( (E3Win32Storage*) storage )->instanceData.theFile ) ;



	// Update the instance data
	( (E3Win32Storage*) storage )->instanceData.theFile = hFile ;

	return kQ3Success ;	
	}





//=============================================================================
//      E3Win32Storage_Get : Get the handle for a Win32 storage.
//-----------------------------------------------------------------------------
TQ3Status
E3Win32Storage_Get(TQ3StorageObject storage, HANDLE *hFile)
	{
	// Return the handle
	*hFile = ( (E3Win32Storage*) storage )->instanceData.theFile ;
	
	return kQ3Success ;
	}




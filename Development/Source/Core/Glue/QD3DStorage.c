/*  NAME:
        QD3DStorage.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3Storage_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3Storage_GetType(TQ3StorageObject storage)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, kQ3SharedTypeStorage), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Storage_GetType(storage));
}





//=============================================================================
//      Q3Storage_GetSize : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Storage_GetSize(TQ3StorageObject storage, TQ3Uns32 *size)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, (kQ3SharedTypeStorage)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(size), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on size
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Storage_GetSize(storage, size));
}





//=============================================================================
//      Q3Storage_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Storage_GetData(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, (kQ3SharedTypeStorage)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(sizeRead), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on offset
		return(kQ3Failure);

	if (0) // Further checks on dataSize
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on sizeRead
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Storage_GetData(storage, offset, dataSize, data, sizeRead));
}





//=============================================================================
//      Q3Storage_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Storage_SetData(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, (kQ3SharedTypeStorage)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(sizeWritten), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on offset
		return(kQ3Failure);

	if (0) // Further checks on dataSize
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on sizeWritten
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Storage_SetData(storage, offset, dataSize, data, sizeWritten));
}





//=============================================================================
//      Q3MemoryStorage_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3MemoryStorage_GetType(TQ3StorageObject storage)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, kQ3SharedTypeStorage), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MemoryStorage_GetType(storage));
}





//=============================================================================
//      Q3MemoryStorage_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StorageObject
Q3MemoryStorage_New(const unsigned char *buffer, TQ3Uns32 validSize)
{


	// Release build checks


	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on buffer
		return(NULL);

	if (0) // Further checks on validSize
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MemoryStorage_New(buffer, validSize));
}





//=============================================================================
//      Q3MemoryStorage_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MemoryStorage_Set(TQ3StorageObject storage, const unsigned char *buffer, TQ3Uns32 validSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, kQ3SharedTypeStorage), kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(storage) == kQ3StorageTypeMemory, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(buffer), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on buffer
		return(kQ3Failure);

	if (0) // Further checks on validSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MemoryStorage_Set(storage, buffer, validSize));
}





//=============================================================================
//      Q3MemoryStorage_NewBuffer : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StorageObject
Q3MemoryStorage_NewBuffer(unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(buffer), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on buffer
		return(NULL);

	if (0) // Further checks on validSize
		return(NULL);

	if (0) // Further checks on bufferSize
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MemoryStorage_NewBuffer(buffer, validSize, bufferSize));
}





//=============================================================================
//      Q3MemoryStorage_SetBuffer : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MemoryStorage_SetBuffer(TQ3StorageObject storage, unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, kQ3SharedTypeStorage), kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(storage) == kQ3StorageTypeMemory, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(buffer), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on buffer
		return(kQ3Failure);

	if (0) // Further checks on validSize
		return(kQ3Failure);

	if (0) // Further checks on bufferSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MemoryStorage_SetBuffer(storage, buffer, validSize, bufferSize));
}





//=============================================================================
//      Q3MemoryStorage_GetBuffer : Quesa API entry point.
//-----------------------------------------------------------------------------
//		Note :	It's not correct going by the documentation, but the QD3D
//				Interactive Renderer calls this with several NULL parameters.
//-----------------------------------------------------------------------------
TQ3Status
Q3MemoryStorage_GetBuffer(TQ3StorageObject storage, unsigned char **buffer, TQ3Uns32 *validSize, TQ3Uns32 *bufferSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, kQ3SharedTypeStorage), kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(storage) == kQ3StorageTypeMemory, kQ3Failure);

	if (buffer != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(buffer), kQ3Failure);

	if (validSize != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(validSize), kQ3Failure);

	if (bufferSize != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bufferSize), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on buffer
		return(kQ3Failure);

	if (0) // Further checks on validSize
		return(kQ3Failure);

	if (0) // Further checks on bufferSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MemoryStorage_GetBuffer(storage, buffer, validSize, bufferSize));
}





//=============================================================================
//      Q3PathStorage_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3StorageObject
Q3PathStorage_New(const char *pathName)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pathName), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pathName
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PathStorage_New(pathName));
}
#endif





//=============================================================================
//      Q3PathStorage_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Status
Q3PathStorage_Set(TQ3StorageObject theStorage, const char *pathName)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theStorage, kQ3SharedTypeStorage), kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(theStorage) == kQ3StorageTypePath, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pathName), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theStorage
		return(kQ3Failure);

	if (0) // Further checks on pathName
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PathStorage_Set(theStorage, pathName));
}
#endif





//=============================================================================
//      Q3PathStorage_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Status
Q3PathStorage_Get(TQ3StorageObject theStorage, char *pathName)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theStorage, kQ3SharedTypeStorage), kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(theStorage) == kQ3StorageTypePath, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pathName), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theStorage
		return(kQ3Failure);

	if (0) // Further checks on pathName
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PathStorage_Get(theStorage, pathName));
}
#endif





//=============================================================================
//      Q3HandleStorage_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_MACINTOSH
TQ3StorageObject
Q3HandleStorage_New(Handle handle, TQ3Uns32 validSize)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on handle
		return(NULL);

	if (0) // Further checks on validSize
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3HandleStorage_New(handle, validSize));
}





//=============================================================================
//      Q3HandleStorage_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3HandleStorage_Set(TQ3StorageObject storage, Handle handle, TQ3Uns32 validSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, kQ3SharedTypeStorage), kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(storage) == kQ3MemoryStorageTypeHandle, kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on handle
		return(kQ3Failure);

	if (0) // Further checks on validSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3HandleStorage_Set(storage, handle, validSize));
}





//=============================================================================
//      Q3HandleStorage_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3HandleStorage_Get(TQ3StorageObject storage, Handle *handle, TQ3Uns32 *validSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, kQ3SharedTypeStorage), kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(storage) == kQ3MemoryStorageTypeHandle, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(handle), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(validSize), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on handle
		return(kQ3Failure);

	if (0) // Further checks on validSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3HandleStorage_Get(storage, handle, validSize));
}





//=============================================================================
//      Q3MacintoshStorage_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StorageObject
Q3MacintoshStorage_New(TQ3Int16 fsRefNum)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on fsRefNum
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacintoshStorage_New(fsRefNum));
}





//=============================================================================
//      Q3MacintoshStorage_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MacintoshStorage_Set(TQ3StorageObject storage, TQ3Int16 fsRefNum)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, kQ3SharedTypeStorage), kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT((Q3Object_GetLeafType(storage) == kQ3StorageTypeMacintosh), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on fsRefNum
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacintoshStorage_Set(storage, fsRefNum));
}





//=============================================================================
//      Q3MacintoshStorage_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MacintoshStorage_Get(TQ3StorageObject storage, TQ3Int16 *fsRefNum)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, kQ3SharedTypeStorage), kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT((Q3Object_GetLeafType(storage) == kQ3StorageTypeMacintosh), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fsRefNum), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on fsRefNum
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacintoshStorage_Get(storage, fsRefNum));
}





//=============================================================================
//      Q3MacintoshStorage_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3MacintoshStorage_GetType(TQ3StorageObject storage)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, kQ3StorageTypeMacintosh), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacintoshStorage_GetType(storage));
}





//=============================================================================
//      Q3FSSpecStorage_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StorageObject
Q3FSSpecStorage_New(const FSSpec *fs)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fs), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on fs
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FSSpecStorage_New(fs));
}





//=============================================================================
//      Q3FSSpecStorage_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3FSSpecStorage_Set(TQ3StorageObject storage, const FSSpec *fs)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, (kQ3MacintoshStorageTypeFSSpec)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fs), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on fs
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FSSpecStorage_Set(storage, fs));
}





//=============================================================================
//      Q3FSSpecStorage_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3FSSpecStorage_Get(TQ3StorageObject storage, FSSpec *fs)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, (kQ3MacintoshStorageTypeFSSpec)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fs), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on fs
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FSSpecStorage_Get(storage, fs));
}
#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Q3Win32Storage_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_WIN32
TQ3StorageObject
Q3Win32Storage_New(HANDLE hFile)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on hFile
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Win32Storage_New(hFile));
}





//=============================================================================
//      Q3Win32Storage_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Win32Storage_Set(TQ3StorageObject storage, HANDLE hFile)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, (kQ3StorageTypeWin32)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on hFile
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Win32Storage_Set(storage, hFile));
}





//=============================================================================
//      Q3Win32Storage_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Win32Storage_Get(TQ3StorageObject storage, HANDLE *hFile)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, (kQ3StorageTypeWin32)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(hFile), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on hFile
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Win32Storage_Get(storage, hFile));
}
#endif // QUESA_OS_WIN32





//=============================================================================
//      Q3UnixPathStorage_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_UNIX
TQ3StorageObject
Q3UnixPathStorage_New(const char *pathName)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pathName), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pathName
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3UnixPathStorage_New(pathName));
}





//=============================================================================
//      Q3UnixPathStorage_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3UnixPathStorage_Set(TQ3StorageObject storage, const char *pathName)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, (kQ3UnixStorageTypePath)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pathName), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on pathName
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3UnixPathStorage_Set(storage, pathName));
}





//=============================================================================
//      Q3UnixPathStorage_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3UnixPathStorage_Get(TQ3StorageObject storage, char *pathName)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(storage, (kQ3UnixStorageTypePath)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pathName), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on storage
		return(kQ3Failure);

	if (0) // Further checks on pathName
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3UnixPathStorage_Get(storage, pathName));
}
#endif // QUESA_OS_UNIX





//=============================================================================
//      Q3BeStorage_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_BE
TQ3StorageObject
Q3BeStorage_New(BFile *theFile)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theFile), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theFile
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BeStorage_New(theFile));
}





//=============================================================================
//      Q3BeStorage_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3BeStorage_Set(TQ3StorageObject theStorage, BFile *theFile)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theStorage, (kQ3StorageTypeBe)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theFile), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theStorage
		return(kQ3Failure);

	if (0) // Further checks on theFile
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BeStorage_Set(theStorage, theFile));
}





//=============================================================================
//      Q3BeStorage_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3BeStorage_Get(TQ3StorageObject theStorage, BFile **theFile)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theStorage, (kQ3StorageTypeBe)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theFile), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theStorage
		return(kQ3Failure);

	if (0) // Further checks on theFile
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3BeStorage_Get(theStorage, theFile));
}
#endif // OS_BE





/*  NAME:
        QD3DStorage.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Copyright (c) 1999-2019, Quesa Developers. All rights reserved.

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
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( storage ), kQ3ObjectTypeInvalid);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Storage*) storage )->GetType () ;
}





//=============================================================================
//      Q3Storage_GetSize : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Storage_GetSize(TQ3StorageObject storage, TQ3Uns32 *size)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( storage ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(size), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Storage*) storage )->GetSize ( size ) ;
}





//=============================================================================
//      Q3Storage_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Storage_GetData(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( storage ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(sizeRead), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Storage*) storage )->GetData ( offset, dataSize, data, sizeRead ) ;
}





//=============================================================================
//      Q3Storage_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Storage_SetData(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( storage ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(sizeWritten), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Storage*) storage )->SetData ( offset, dataSize, data, sizeWritten ) ;
}



/*!
	@function			Q3Storage_Open
	@abstract			Open a storage for reading or writing of raw data.
	@discussion			A file storage object must be opened before you get,
						set, or find the size of its data.  If you are reading
						or writing object data (e.g., 3DMF or VRML), use a File
						object attached to the storage and open the File.  But
						if the storage contains other binary data, such as pixel
						data, it is more appropriate to open the storage
						directly.
						
						Of course, you should close the storage after you are
						finished reading or writing its contents.
						
						If you open a storage that does not need to be opened,
						such as a memory storage or file stream storage, then
						this function will succeed without really doing anything.
	@param				storage		The storage object.
	@param				forWriting	True to open for writing, false to open for
									reading.
	@result				Success or failure of the operation.
*/
TQ3Status
Q3Storage_Open(
	TQ3StorageObject              storage,
	TQ3Boolean                    forWriting )
{
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( storage ), kQ3Failure);



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Storage*) storage )->Open( forWriting );
}



/*!
	@function			Q3Storage_Close
	@abstract			Close a storage object.
	@discussion			See Q3Storage_Open for a discussion of opening a storage
						versus opening a file.
	@param				storage		The storage object.
	@result				Success or failure of the operation.
*/
TQ3Status
Q3Storage_Close(
	TQ3StorageObject              storage )
{
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( storage ), kQ3Failure);



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Storage*) storage )->Close();
}



/*!
	@function			Q3Storage_GetOpenness
	@abstract			Get the openness state of a storage object.
	@discussion			Note that a storage object may be opened using
						Q3Storage_Open or by associating a File object and
						opening that.
	@param				inStorage		A storage object.
	@param				outOpenness		Receives an enumerated value indicating
										the openness state.
	@result				Success or failure of the operation.
*/
TQ3Status
Q3Storage_GetOpenness(
	TQ3StorageObject			inStorage,
	TQ3StorageOpenness*			outOpenness )
{
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( inStorage ), kQ3Failure );
	Q3_REQUIRE_OR_RESULT( Q3_VALID_PTR(outOpenness), kQ3Failure );
	
	// Call the bottleneck
	E3System_Bottleneck();

	// Call our implementation
	return ( (E3Storage*) inStorage )->GetOpenness( outOpenness );
}



//=============================================================================
//      Q3MemoryStorage_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3MemoryStorage_GetType(TQ3StorageObject storage)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( storage ), kQ3ObjectTypeInvalid);



	// Debug build checks



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



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MemoryStorage_New(buffer, validSize));
}



/*!
	@function	Q3MemoryStorage_NewNoCopy
	
	@abstract	Create a memory storage object, taking over ownership of
				previously allocated memory.
	
	@discussion	The buffer you pass in must have been allocated using
				Q3Memory_Allocate or Q3Memory_AllocateClear.  The storage
				object will take over responsibility for releasing the
				buffer when the storage object is disposed or grown,
				as appropriate.

	@param		buffer		Pointer to a buffer in memory, previously allocated
							using Q3Memory_Allocate or Q3Memory_AllocateClear.
	@param		validSize	Number of bytes of data in the buffer.
	@param		bufferSize	Allocated size of the buffer.
	@result		The new storage object.
*/
TQ3StorageObject
Q3MemoryStorage_NewNoCopy(
    unsigned char                 *buffer,
    TQ3Uns32                      validSize,
    TQ3Uns32                      bufferSize
)
{
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(buffer), nullptr);


	// Call the bottleneck
	E3System_Bottleneck();


	// Call our implementation
	return E3MemoryStorage_NewNoCopy( buffer, validSize, bufferSize );
}

//=============================================================================
//      Q3MemoryStorage_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MemoryStorage_Set(TQ3StorageObject storage, const unsigned char *buffer, TQ3Uns32 validSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( storage ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(storage) == kQ3StorageTypeMemory, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(buffer), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3MemoryStorage*) storage )->Set ( buffer, validSize ) ;
}





//=============================================================================
//      Q3MemoryStorage_NewBuffer : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3StorageObject
Q3MemoryStorage_NewBuffer(unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize)
{


	// Release build checks
	// Note, buffer is allowed to be nullptr.
	Q3_REQUIRE_OR_RESULT( validSize <= bufferSize, nullptr );



	// Debug build checks



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
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( storage ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(storage) == kQ3StorageTypeMemory, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(buffer), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3MemoryStorage*) storage )->SetBuffer ( buffer, validSize, bufferSize ) ;
}





//=============================================================================
//      Q3MemoryStorage_GetBuffer : Quesa API entry point.
//-----------------------------------------------------------------------------
//		Note :	It's not correct going by the documentation, but the QD3D
//				Interactive Renderer calls this with several nullptr parameters.
//-----------------------------------------------------------------------------
TQ3Status
Q3MemoryStorage_GetBuffer(TQ3StorageObject storage, unsigned char **buffer, TQ3Uns32 *validSize, TQ3Uns32 *bufferSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(storage) == kQ3StorageTypeMemory, kQ3Failure);

	if (buffer != nullptr)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(buffer), kQ3Failure);

	if (validSize != nullptr)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(validSize), kQ3Failure);

	if (bufferSize != nullptr)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bufferSize), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3MemoryStorage*) storage )->GetBuffer ( buffer, validSize, bufferSize ) ;
}





//=============================================================================
//      Q3PathStorage_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3StorageObject
Q3PathStorage_New(const char *pathName)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pathName), nullptr);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return (E3PathStorage_New( pathName, kQ3False ));
}
#endif





#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3StorageObject
Q3PathStorage_New_Owned( const char* pathName )
{
	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pathName), nullptr);


	// Call the bottleneck
	E3System_Bottleneck();
	


	// Call our implementation
	return (E3PathStorage_New( pathName, kQ3True ));
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
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( theStorage ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(theStorage) == kQ3StorageTypePath, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pathName), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3PathStorage*) theStorage )->Set ( pathName ) ;
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
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( theStorage ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(theStorage) == kQ3StorageTypePath, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pathName), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3PathStorage*) theStorage )->Get ( pathName ) ;
}
#endif





//=============================================================================
//      Q3FileStreamStorage_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3StorageObject
Q3FileStreamStorage_New( FILE* theStream )
{
	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3FileStreamStorage_New(theStream));
}
#endif





//=============================================================================
//      Q3FileStreamStorage_Set : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Status
Q3FileStreamStorage_Set( TQ3StorageObject theStorage, FILE* theStream )
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( theStorage ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(theStorage) == kQ3StorageTypeFileStream, kQ3Failure);



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	( (E3FileStreamStorage*) theStorage )->Set( theStream );
	
	return kQ3Success;
}
#endif





//=============================================================================
//      Q3FileStreamStorage_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Status
Q3FileStreamStorage_Get(TQ3StorageObject theStorage, FILE** theStream )
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Storage::IsOfMyClass ( theStorage ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_GetLeafType(theStorage) == kQ3StorageTypeFileStream, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theStream), kQ3Failure);



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	*theStream = ( (E3FileStreamStorage*) theStorage )->Get();
	return kQ3Success;
}
#endif









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



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Win32Storage_Get(storage, hFile));
}
#endif // QUESA_OS_WIN32

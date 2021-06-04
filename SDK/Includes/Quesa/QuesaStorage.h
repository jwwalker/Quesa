/*! @header QuesaStorage.h
        Declares the Quesa storage objects.
          
	@ignore	_Nullable
	@ignore _Nonnull
	@ignore	_Null_unspecified
 */
/*  NAME:
        QuesaStorage.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Copyright (c) 1999-2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#ifndef QUESA_STORAGE_HDR
#define QUESA_STORAGE_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"
#include <stdio.h>

// Disable QD3D header
#ifdef __QD3DSTORAGE__
#error
#endif

#define __QD3DSTORAGE__

#if QUESA_OS_WIN32
    #include <Windows.h>
#endif

#include <stdio.h>





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------

/*!
	@enum		TQ3StorageOpenness
	@discussion	Open or closed state of a storage object.  Be aware that some
				types of storage object do not have separate open and closed
				states, so this enumeration has 3 values.
	@constant	kQ3StorageOpenness_Ignored
				The storage ignores open and close commands, as in for example
				memory storage.
	@constant	kQ3StorageOpenness_Closed
				The storage is currently closed.
	@constant	kQ3StorageOpenness_Open
				The storage is currently opened.
*/
typedef enum TQ3StorageOpenness QUESA_ENUM_BASE(TQ3Uns32)
{
	kQ3StorageOpenness_Ignored = 0,
	kQ3StorageOpenness_Closed,
	kQ3StorageOpenness_Open
} TQ3StorageOpenness;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Storage_GetType
 *  @discussion
 *      Returns the type of a storage object.
 *
 *      Returns kQ3StorageTypeMemory, kQ3StorageTypeUnix, kQ3StorageTypeMacintosh,
 *		or kQ3StorageTypeWin32, provided that the object is a storage object.
 *
 *  @param storage          The storage object to test.
 *  @result                 The type of storage, or kQ3ObjectTypeInvalid.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Storage_GetType (
    TQ3StorageObject _Nonnull             storage
);



/*!
 *  @function
 *      Q3Storage_GetSize
 *  @discussion
 *      Get the size of the data in a storage object.
 *
 *      Returns the number of bytes of data stored in the object.
 *      If the storage is a type associated with a file, the file
 *		must be open.
 *
 *  @param storage          The storage object.
 *  @param size             On output, receives the size.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Storage_GetSize (
    TQ3StorageObject _Nonnull             storage,
    TQ3Uns32                      * _Nonnull size
);



/*!
 *  @function
 *      Q3Storage_GetData
 *  @discussion
 *      Read some data from a storage object.
 *
 *      Returns some or all of the private data associated with a storage
 *      object.  If the storage is associated with a file object, then
 *		the file must be open.
 *
 *  @param storage          The storage object.
 *  @param offset           Starting offset of the data to be retrieved.
 *  @param dataSize         Number of bytes of data to get.
 *  @param data             Buffer to receive the data.
 *  @param sizeRead         On output, number of bytes actually received.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Storage_GetData (
    TQ3StorageObject _Nonnull             storage,
    TQ3Uns32                      offset,
    TQ3Uns32                      dataSize,
    unsigned char                 * _Nonnull data,
    TQ3Uns32                      * _Nonnull sizeRead
);



/*!
 *  @function
 *      Q3Storage_SetData
 *  @discussion
 *      Write some data to a storage object.
 *
 *		Write some data to a storage object, possibly extending the storage
 *		to accommodate the new data.  If the storage is associated with a file object, then
 *		the file must be open.
 *
 *  @param storage          The storage object.
 *  @param offset           The offset at which to begin writing new data.
 *  @param dataSize         Number of bytes of data to be written.
 *  @param data             Data to be written.
 *  @param sizeWritten      On output, number of bytes actually written,
 *							normally the same as dataSize.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Storage_SetData (
    TQ3StorageObject _Nonnull             storage,
    TQ3Uns32                      offset,
    TQ3Uns32                      dataSize,
    const unsigned char           * _Nonnull data,
    TQ3Uns32                      * _Nonnull sizeWritten
);



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
Q3_EXTERN_API_C ( TQ3Status )
Q3Storage_Open(
	TQ3StorageObject _Nonnull             storage,
	TQ3Boolean                    forWriting );



/*!
	@function			Q3Storage_Close
	@abstract			Close a storage object.
	@discussion			See Q3Storage_Open for a discussion of opening a storage
						versus opening a file.
	@param				storage		The storage object.
	@result				Success or failure of the operation.
*/
Q3_EXTERN_API_C ( TQ3Status )
Q3Storage_Close(
	TQ3StorageObject _Nonnull             storage );



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
Q3_EXTERN_API_C ( TQ3Status )
Q3Storage_GetOpenness(
	TQ3StorageObject _Nonnull			inStorage,
	TQ3StorageOpenness*	_Nonnull		outOpenness );


/*!
	@functiongroup Memory Storage
*/


/*!
 *  @function
 *      Q3MemoryStorage_GetType
 *  @discussion
 *      Get the type of a memory storage object.
 *
 *      Returns the sub-type of a memory storage object,
 *      currently either kQ3MemoryStorageTypeHandle or kQ3ObjectTypeInvalid.
 *
 *  @param storage          The storage object.
 *  @result                 The type of memory storage, or kQ3ObjectTypeInvalid.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3MemoryStorage_GetType (
    TQ3StorageObject _Nonnull             storage
);



/*!
 *  @function
 *      Q3MemoryStorage_New
 *  @discussion
 *      Create a memory storage object.
 *
 *      Creates a memory storage object using memory that is maintained
 *		internally by Quesa.  If you pass a buffer of data, that data will be
 *		copied to the storage object's private memory
 *
 *  @param buffer           Pointer to a buffer in memory, or nullptr.
 *  @param validSize        Number of bytes of data in the buffer.
 *							If you passed nullptr for the buffer, this number serves
 *							as the initial size and grow size of the internally
 *							allocated buffer.  If you passed nullptr for buffer and
 *							0 for validSize, then Quesa uses a default grow size.
 *  @result                 The new storage object.
 */
Q3_EXTERN_API_C ( TQ3StorageObject _Nonnull )
Q3MemoryStorage_New (
    const unsigned char           * _Nullable buffer,
    TQ3Uns32                      validSize
);



/*!
 *	@function
 *		Q3MemoryStorage_NewNoCopy
 *	@abstract	Create a memory storage object, taking over ownership of
 *				previously allocated memory.
 *	@discussion	The buffer you pass in must have been allocated using
 *				Q3Memory_Allocate or Q3Memory_AllocateClear.  The storage
 *				object will take over responsibility for releasing the
 *				buffer when the storage object is disposed or grown,
 *				as appropriate.
 *
 *	@param		buffer		Pointer to a buffer in memory, previously allocated
 *							using Q3Memory_Allocate or Q3Memory_AllocateClear.
 *	@param		validSize	Number of bytes of data in the buffer.
 *	@param		bufferSize	Allocated size of the buffer.
 *	@result		The new storage object.
*/
Q3_EXTERN_API_C ( TQ3StorageObject _Nonnull )
Q3MemoryStorage_NewNoCopy(
    unsigned char                 * _Nonnull buffer,
    TQ3Uns32                      validSize,
    TQ3Uns32                      bufferSize
);



/*!
 *  @function
 *      Q3MemoryStorage_Set
 *  @discussion
 *      Sets the data of a memory storage object.
 *
 *      Writes data to a storage object by copying data from a buffer.
 *		Whether or not the storage object owned its own memory before
 *		this call (i.e., whether it was created with Q3MemoryStorage_New
 *		or Q3MemoryStorage_NewBuffer), it will own its own memory afterward.
 *
 *  @param storage          The storage object.
 *  @param buffer           Pointer to a buffer of data, or nullptr.
 *  @param validSize        Number of bytes to be copied from the buffer to the
 *							storage, or initial memory size if buffer was nullptr.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MemoryStorage_Set (
    TQ3StorageObject _Nonnull     storage,
    const unsigned char           * _Nullable buffer,
    TQ3Uns32                      validSize
);



/*!
 *  @function
 *      Q3MemoryStorage_NewBuffer
 *  @discussion
 *      Create a memory storage object that uses a provided buffer.
 *
 *      Creates a memory storage object using a given buffer rather than by
 *		allocating Quesa memory.
 *
 *  @param buffer           Pointer to a buffer of data, or nullptr.
 *  @param validSize        Number of bytes of valid data in the provided buffer,
 *							or if buffer was nullptr, the initial size and grow
 *							size of an internally-allocated buffer.
 *  @param bufferSize       Size in bytes of the buffer.
 *  @result                 The new storage object.
 */
Q3_EXTERN_API_C ( TQ3StorageObject _Nonnull )
Q3MemoryStorage_NewBuffer (
    unsigned char                 * _Nullable buffer,
    TQ3Uns32                      validSize,
    TQ3Uns32                      bufferSize
);



/*!
 *  @function
 *      Q3MemoryStorage_SetBuffer
 *  @discussion
 *      Sets the data of a memory storage object.
 *
 *      Sets the data of a memory storage object to be stored in a
 *		given buffer.
 *
 *  @param storage          The storage object.
 *  @param buffer           Pointer to a buffer of data, or nullptr.
 *  @param validSize        Number of valid bytes in the buffer, or
 *							if the buffer is nullptr, the initial and grow size
 *							of an internally allocated buffer.
 *  @param bufferSize       Size in bytes of the buffer.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MemoryStorage_SetBuffer (
    TQ3StorageObject _Nonnull             storage,
    unsigned char                 * _Nullable buffer,
    TQ3Uns32                      validSize,
    TQ3Uns32                      bufferSize
);



/*!
 *  @function
 *      Q3MemoryStorage_GetBuffer
 *  @discussion
 *      Get the data of a memory storage object.
 *
 *      Gets a pointer to the data of a memory storage object without copying.
 *      The pointer may become invalid if later storage operations cause the
 *		buffer to be reallocated.
 *
 *		Each of the output parameters may be nullptr if you don't need that information.
 *
 *  @param storage          The storage object.
 *  @param buffer           On output, receives a pointer to the actual data
 *							of the storage.
 *  @param validSize        On output, the number of valid metafile data bytes
 *							in the storage object.
 *  @param bufferSize       On output, the size of the buffer in bytes.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MemoryStorage_GetBuffer (
    TQ3StorageObject _Nonnull             storage,
    unsigned char                 * _Nonnull * _Nullable buffer,
    TQ3Uns32                      * _Nullable validSize,
    TQ3Uns32                      * _Nullable bufferSize
);


/*!
	@functiongroup Multiplatform Path Storage
*/


/*!
 *  @function
 *      Q3PathStorage_New
 *  @discussion
 *      Creates a storage object of type kQ3StorageTypePath.
 *		This type of storage is associated with a file specified by a path name,
 *		and is intended to replace the poorly-named "Unix path" storage.
 *
 *		The exact format of permissible paths is platform-dependent.  For example,
 *		on Mac OS X, the path is slash-separated and assumed to be in the UTF-8
 *		character encoding.
 *
 *		If the file does not already exist, opening the storage for writing will
 *		create it.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param pathName         A NUL-terminated pathname, as might be passed to fopen.
 *  @result                 The new storage object.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3StorageObject _Nonnull )
Q3PathStorage_New (
    const char                    * _Nonnull pathName
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3PathStorage_New_Owned
 *  @discussion
 *  	Creates a storage object of type kQ3StorageTypePath that
 *  	will delete the associated file when the object is deleted.
 *  
 *  	This function is like Q3PathStorage_New, except that if you
 *  	create a path storage with this function, it "owns" the file.
 *  	That is, when the storage is deleted (i.e., the last
 *  	reference is disposed), the file will be unlinked.
 *  	
 *  	If you duplicate an owned path storage, then the file will
 *  	be unlinked when the last storage that owns it is deleted.
 *  	
 *  	<em>This function is not available in QD3D.</em>
 *
 *  @param pathName         A NUL-terminated pathname, as might be passed to fopen.
 *  @result                 The new storage object.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3StorageObject _Nonnull )
Q3PathStorage_New_Owned (
    const char                    * _Nonnull pathName
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3PathStorage_Set
 *  @discussion
 *      Changes the path associated with a path storage object.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theStorage       A path storage object.
 *  @param pathName         A NUL-terminated pathname.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3PathStorage_Set (
    TQ3StorageObject _Nonnull             theStorage,
    const char                    * _Nonnull pathName
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3PathStorage_Get
 *  @discussion
 *      Finds the path associated with a path storage object.
 *
 *		The path returned by this function could be up to 1024 bytes long.
 *		Be sure to allocate enough space for it.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theStorage       A path storage object.
 *  @param pathName         On output, the path as a NUL-terminated string.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3PathStorage_Get (
    TQ3StorageObject _Nonnull             theStorage,
    char                          * _Nonnull pathName
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS


/*!
	@functiongroup Multiplatform File Stream Storage
*/


/*!
	@function	Q3FileStreamStorage_New
	@abstract	Create a storage object from a Unix-style file stream (FILE*).
	
	@discussion	The specified stream must remain open as long as you use the
				storage object, and it is your responsibility to close it later.
				Calling Q3File_OpenRead, Q3File_OpenWrite, or Q3File_Close on an
				associated TQ3FileObject do not actually open or close the
				stream, though they are still necessary steps when doing I/O
				with a file stream storage object.
				
				<em>This function is not available in QD3D.</em>
	
	@param		theStream An open stream.
	@result		The new storage object.
*/
Q3_EXTERN_API_C ( TQ3StorageObject _Nonnull )
Q3FileStreamStorage_New (
    FILE* _Nonnull theStream
);



/*!
	@function	Q3FileStreamStorage_Get
	@discussion	Get the stream associated with the storage.
	
				<em>This function is not available in QD3D.</em>
	
	@param		theStorage		A file stream storage object.
	@param		theStream		Receives a FILE pointer.
	@result		Success or failure of the operation.
*/
Q3_EXTERN_API_C ( TQ3Status  )
Q3FileStreamStorage_Get (
    TQ3StorageObject _Nonnull             theStorage,
    FILE* _Nullable *  _Nonnull                      theStream
);



/*!
	@function	Q3FileStreamStorage_Set
	@discussion	Set the stream associated with the storage.
	
				<em>This function is not available in QD3D.</em>
	
	@param		theStorage		A file stream storage object.
	@param		theStream		A stream to use with the storage.
	@result		Success or failure of the operation.
*/
Q3_EXTERN_API_C ( TQ3Status  )
Q3FileStreamStorage_Set (
    TQ3StorageObject _Nonnull             theStorage,
    FILE* _Nonnull                        theStream
);









//=============================================================================
//      Windows function prototypes
//-----------------------------------------------------------------------------
/*!
	@functiongroup Windows Storage
*/

#if QUESA_OS_WIN32

/*!
 *  @function
 *      Q3Win32Storage_New
 *  @discussion
 *      This creates a storage object whose data is stored in a file,
 *		specified by a file HANDLE for an open file.
 *		The specified file must remain open as long as you use the storage
 *		object, and it is your responsibility to close the file later.
 *
 *  @param hFile            A valid file handle.
 *  @result                 The new storage object.
 */
Q3_EXTERN_API_C ( TQ3StorageObject _Nonnull )
Q3Win32Storage_New (
    HANDLE _Nonnull                        hFile
);



/*!
 *  @function
 *      Q3Win32Storage_Set
 *  @discussion
 *      Set the file handle for a Win32 storage object.
 *
 *  @param storage          The storage object to update.
 *  @param hFile            The new file handle.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Win32Storage_Set (
    TQ3StorageObject _Nonnull             storage,
    HANDLE _Nonnull                        hFile
);



/*!
 *  @function
 *      Q3Win32Storage_Get
 *  @discussion
 *      Get the file handle for a Win32 storage object.
 *
 *  @param storage          The storage object to query.
 *  @param hFile            Receives the file handle.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Win32Storage_Get (
    TQ3StorageObject _Nonnull             storage,
    HANDLE _Nonnull                       * _Nonnull hFile
);

#endif // QUESA_OS_WIN32








// Work around a HeaderDoc bug
/*!
	@functiongroup
*/




//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



/*! @header QuesaStorage.h
        Declares the Quesa storage objects.
 */
/*  NAME:
        QuesaStorage.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_STORAGE_HDR
#define QUESA_STORAGE_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#ifdef __QD3DSTORAGE__
#error
#endif

#define __QD3DSTORAGE__

#if QUESA_OS_MACINTOSH
    #if QUESA_UH_IN_FRAMEWORKS
		#include <Carbon/Carbon.h>
    #else
        #include <MacTypes.h>
        #include <Files.h>
    #endif

#elif QUESA_OS_WIN32
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
    TQ3StorageObject              storage
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
    TQ3StorageObject              storage,
    TQ3Uns32                      *size
);



/*!
 *  @function
 *      Q3Storage_GetData
 *  @discussion
 *      Get private data from a storage object.
 *
 *      Returns some or all of the private data associated with a storage
 *      object.  If the storage is associated with a file object, then
 *		the file must be closed.
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
    TQ3StorageObject              storage,
    TQ3Uns32                      offset,
    TQ3Uns32                      dataSize,
    unsigned char                 *data,
    TQ3Uns32                      *sizeRead
);



/*!
 *  @function
 *      Q3Storage_SetData
 *  @discussion
 *      Write some data to a storage object.
 *
 *		Write some data to a storage object, possibly extending the storage
 *		to accommodate the new data.
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
    TQ3StorageObject              storage,
    TQ3Uns32                      offset,
    TQ3Uns32                      dataSize,
    const unsigned char           *data,
    TQ3Uns32                      *sizeWritten
);



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
    TQ3StorageObject              storage
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
 *  @param buffer           Pointer to a buffer in memory, or NULL.
 *  @param validSize        Number of bytes of data in the buffer.
 *							If you passed NULL for the buffer, this number serves
 *							as the initial size and grow size of the internally
 *							allocated buffer.  If you passed NULL for buffer and
 *							0 for validSize, then Quesa uses a default grow size.
 *  @result                 The new storage object.
 */
Q3_EXTERN_API_C ( TQ3StorageObject  )
Q3MemoryStorage_New (
    const unsigned char           *buffer,
    TQ3Uns32                      validSize
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
 *  @param buffer           Pointer to a buffer of data, or NULL.
 *  @param validSize        Number of bytes to be copied from the buffer to the
 *							storage, or initial memory size if buffer was NULL.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MemoryStorage_Set (
    TQ3StorageObject              storage,
    const unsigned char           *buffer,
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
 *  @param buffer           Pointer to a buffer of data, or NULL.
 *  @param validSize        Number of bytes of valid data in the provided buffer,
 *							or if buffer was NULL, the initial size and grow
 *							size of an internally-allocated buffer.
 *  @param bufferSize       Size in bytes of the buffer.
 *  @result                 The new storage object.
 */
Q3_EXTERN_API_C ( TQ3StorageObject  )
Q3MemoryStorage_NewBuffer (
    unsigned char                 *buffer,
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
 *  @param buffer           Pointer to a buffer of data, or NULL.
 *  @param validSize        Number of valid bytes in the buffer, or
 *							if the buffer is NULL, the initial and grow size
 *							of an internally allocated buffer.
 *  @param bufferSize       Size in bytes of the buffer.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MemoryStorage_SetBuffer (
    TQ3StorageObject              storage,
    unsigned char                 *buffer,
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
 *		Each of the output parameters may be NULL if you don't need that information.
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
    TQ3StorageObject              storage,
    unsigned char                 **buffer,
    TQ3Uns32                      *validSize,
    TQ3Uns32                      *bufferSize
);



/*!
 *  @function
 *      Q3PathStorage_New
 *  @discussion
 *      Creates a storage object of type kQ3StorageTypePath.
 *		This type of storage is associated with a file specified by a path name,
 *		and is intended to replace the poorly-named "Unix path" storage.
 *
 *		The exact format of permissible paths is platform-dependent.  For example,
 *		in a Mac version of Quesa built with the Metrowerks Standard Library,
 *		the path is colon-separated and assumed to be in the system encoding.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param pathName         A NULL-terminated pathname, as might be passed to fopen.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3StorageObject  )
Q3PathStorage_New (
    const char                    *pathName
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
 *  @param pathName         A NULL-terminated pathname.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3PathStorage_Set (
    TQ3StorageObject              theStorage,
    const char                    *pathName
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
 *  @param pathName         On output, the path as a NULL-terminated string.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3PathStorage_Get (
    TQ3StorageObject              theStorage,
    char                          *pathName
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      Mac OS function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

/*!
 *  @function
 *      Q3HandleStorage_New
 *  @discussion
 *      Create a Handle storage object.
 *
 *      Create a storage object that stores data in a Macintosh memory Handle.
 *		If you pass NULL for the Handle, Quesa internally allocates a
 *		Handle of the given size, and disposes the Handle when the storage
 *		object is disposed.
 *
 *  @param handle           An existing Handle, or NULL.
 *  @param validSize        The size of the valid date in the given Handle,
 *							or the desired size if the Handle is NULL.
 *  @result                 The new storage object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3StorageObject  )
Q3HandleStorage_New (
    Handle                        handle,
    TQ3Uns32                      validSize
);



/*!
 *  @function
 *      Q3HandleStorage_Set
 *  @discussion
 *      Set information about a Handle storage object.
 *
 *      Sets the buffer location and size of a Handle storage object.
 *		If you pass NULL for the Handle, Quesa internally allocates a
 *		Handle of the given size, and disposes the Handle when the storage
 *		object is disposed.
 *
 *  @param storage          The storage object.
 *  @param handle           A Mac Handle to be associated with the storage, or NULL.
 *  @param validSize        Size in bytes of the specified buffer.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3HandleStorage_Set (
    TQ3StorageObject              storage,
    Handle                        handle,
    TQ3Uns32                      validSize
);



/*!
 *  @function
 *      Q3HandleStorage_Get
 *  @discussion
 *      Get information about a Handle storage object.
 *
 *
 *  @param storage          The storage object.
 *  @param handle           On output, the Handle associated with the storage.
 *  @param validSize        On output, the valid size in bytes of the storage.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3HandleStorage_Get (
    TQ3StorageObject              storage,
    Handle                        *handle,
    TQ3Uns32                      *validSize
);



/*!
 *  @function
 *      Q3MacintoshStorage_New
 *  @discussion
 *      Create a Macintosh storage object.
 *
 *      This creates a storage object whose data is stored in a file,
 *		specified by a Mac file reference number for an open file.
 *		The specified file must remain open as long as you use the storage
 *		object, and it is your responsibility to close the file later.
 *
 *  @param fsRefNum         A file reference number for an open data fork of a
 *							Mac file.
 *  @result                 The new storage object.
 */
Q3_EXTERN_API_C ( TQ3StorageObject  )
Q3MacintoshStorage_New (
    TQ3Int16                      fsRefNum
);



/*!
 *  @function
 *      Q3MacintoshStorage_Set
 *  @discussion
 *      Change the file reference number associated with a Mac storage.
 *
 *      This cannot be used when the storage is open.
 *
 *  @param storage          The storage object.
 *  @param fsRefNum       	A file reference number for an open data fork of a
 *							Mac file.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MacintoshStorage_Set (
    TQ3StorageObject              storage,
    TQ3Int16                      fsRefNum
);



/*!
 *  @function
 *      Q3MacintoshStorage_Get
 *  @discussion
 *      Get the file reference number associated with the storage.
 *
 *
 *  @param storage          The storage object.
 *  @param fsRefNum         On output, the file reference number of the file
 *							associated with the storage.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MacintoshStorage_Get (
    TQ3StorageObject              storage,
    TQ3Int16                      *fsRefNum
);



/*!
 *  @function
 *      Q3MacintoshStorage_GetType
 *  @discussion
 *      Get a derived type of a Mac storage object, currently either
 *		kQ3MacintoshStorageTypeFSSpec or kQ3ObjectTypeInvalid.
 *
 *
 *  @param storage          The storage object.
 *  @result                 The object type.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3MacintoshStorage_GetType (
    TQ3StorageObject              storage
);



/*!
 *  @function
 *      Q3FSSpecStorage_New
 *  @discussion
 *      Create a storage object associated with a Mac file specification.
 *
 *      Given a valid FSSpec structure, Quesa creates a storage object.
 *      The file should exist, but should not be open.
 *
 *  @param fs               A valid file system specification.
 *  @result                 The new storage object.
 */
Q3_EXTERN_API_C ( TQ3StorageObject  )
Q3FSSpecStorage_New (
    const FSSpec                  *fs
);



/*!
 *  @function
 *      Q3FSSpecStorage_Set
 *  @discussion
 *      Change the file specification for an FSSpec storage object.
 *
 *      The storage must be closed at the time you call this.
 *
 *  @param storage          The FSSpec storage object to modify.
 *  @param fs               The new file system specification.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3FSSpecStorage_Set (
    TQ3StorageObject              storage,
    const FSSpec                  *fs
);



/*!
 *  @function
 *      Q3FSSpecStorage_Get
 *  @discussion
 *      Find the FSSpec associated with an FSSpec storage object.
 *
 *  @param storage          The FSSpec storage object to examine.
 *  @param fs               On output, a file system specification.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3FSSpecStorage_Get (
    TQ3StorageObject              storage,
    FSSpec                        *fs
);

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Windows function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32

/*!
 *  @function
 *      Q3Win32Storage_New
 *  @discussion
 *      Create a new Win32 storage object.
 *
 *  @param hFile            A valid file handle.
 *  @result                 The new storage object.
 */
Q3_EXTERN_API_C ( TQ3StorageObject  )
Q3Win32Storage_New (
    HANDLE                        hFile
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
    TQ3StorageObject              storage,
    HANDLE                        hFile
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
    TQ3StorageObject              storage,
    HANDLE                        *hFile
);

#endif // QUESA_OS_WIN32





//=============================================================================
//      Unix function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_UNIX

/*!
 *  @function
 *      Q3UnixPathStorage_New
 *  @discussion
 *      Create a storage object associated with a file system path.
 *
 *      The Quesa extension Q3PathStorage_New may be used in place of this.
 *
 *  @param pathName        A NULL-terminated file system path.
 *  @result                The new storage object.
 */
Q3_EXTERN_API_C ( TQ3StorageObject  )
Q3UnixPathStorage_New (
    const char                    *pathName
);



/*!
 *  @function
 *      Q3UnixPathStorage_Set
 *  @discussion
 *      Change the path associated with a Unix path storage object.
 *
 *  @param storage          A Unix path storage object.
 *  @param pathName         A NULL-terminated file system path.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3UnixPathStorage_Set (
    TQ3StorageObject              storage,
    const char                    *pathName
);



/*!
 *  @function
 *      Q3UnixPathStorage_Get
 *  @discussion
 *      Get the path associated with a Unix path storage object
 *
 *		The path returned by this function could be up to 1024 bytes long.
 *		Be sure to allocate enough space for it.
 *
 *  @param storage          A Unix path storage object.
 *  @param pathName         On output, a NULL-terminated path.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3UnixPathStorage_Get (
    TQ3StorageObject              storage,
    char                          *pathName
);

#endif // QUESA_OS_UNIX




//=============================================================================
//      Be function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_BE

/*!
 *  @function
 *      Q3BeStorage_New
 *  @discussion
 *      Create a new Be BFile storage object.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theFile          A valid BFile.
 *  @result                 The new storage object.
 */
Q3_EXTERN_API_C ( TQ3StorageObject  )
Q3BeStorage_New (
    BFile                         *theFile
);



/*!
 *  @function
 *      Q3BeStorage_Set
 *  @discussion
 *      Set the BFile for a Be storage object.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theStorage       The storage object to update.
 *  @param theFile          The new BFile for the storage object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3BeStorage_Set (
    TQ3StorageObject              theStorage,
    BFile                         *theFile
);



/*!
 *  @function
 *      Q3BeStorage_Get
 *  @discussion
 *      Get the BFile for a Be storage object.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theStorage       The storage object to query.
 *  @param theFile          Receives the BFile.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3BeStorage_Get (
    TQ3StorageObject              theStorage,
    BFile                         **theFile
);

#endif // QUESA_OS_BE





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



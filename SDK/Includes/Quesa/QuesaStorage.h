/*! @header QuesaStorage.h
        Declares the Quesa storage objects.
 */
/*  NAME:
        QuesaStorage.h

    DESCRIPTION:
        Quesa public header.

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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Storage_GetType (
    TQ3StorageObject              storage
);



/*!
 *  @function
 *      Q3Storage_GetSize
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param size             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Storage_GetSize (
    TQ3StorageObject              storage,
    TQ3Uns32                      *size
);



/*!
 *  @function
 *      Q3Storage_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param offset           Description of the parameter.
 *  @param dataSize         Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param sizeRead         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param offset           Description of the parameter.
 *  @param dataSize         Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param sizeWritten      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3MemoryStorage_GetType (
    TQ3StorageObject              storage
);



/*!
 *  @function
 *      Q3MemoryStorage_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param buffer           Description of the parameter.
 *  @param validSize        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3MemoryStorage_New (
    const unsigned char           *buffer,
    TQ3Uns32                      validSize
);



/*!
 *  @function
 *      Q3MemoryStorage_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param buffer           Description of the parameter.
 *  @param validSize        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MemoryStorage_Set (
    TQ3StorageObject              storage,
    const unsigned char           *buffer,
    TQ3Uns32                      validSize
);



/*!
 *  @function
 *      Q3MemoryStorage_NewBuffer
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param buffer           Description of the parameter.
 *  @param validSize        Description of the parameter.
 *  @param bufferSize       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3MemoryStorage_NewBuffer (
    unsigned char                 *buffer,
    TQ3Uns32                      validSize,
    TQ3Uns32                      bufferSize
);



/*!
 *  @function
 *      Q3MemoryStorage_SetBuffer
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param buffer           Description of the parameter.
 *  @param validSize        Description of the parameter.
 *  @param bufferSize       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param buffer           Description of the parameter.
 *  @param validSize        Description of the parameter.
 *  @param bufferSize       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
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
 *      Not supported by QD3D.
 *
 *  @param pathName         Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3StorageObject  )
Q3PathStorage_New (
    const char                    *pathName
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3PathStorage_Set
 *  @discussion
 *      Not supported by QD3D.
 *
 *  @param theStorage       Description of the parameter.
 *  @param pathName         Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3PathStorage_Set (
    TQ3StorageObject              theStorage,
    const char                    *pathName
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3PathStorage_Get
 *  @discussion
 *      Not supported by QD3D.
 *
 *  @param theStorage       Description of the parameter.
 *  @param pathName         Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param handle           Description of the parameter.
 *  @param validSize        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3HandleStorage_New (
    Handle                        handle,
    TQ3Uns32                      validSize
);



/*!
 *  @function
 *      Q3HandleStorage_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param handle           Description of the parameter.
 *  @param validSize        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3HandleStorage_Set (
    TQ3StorageObject              storage,
    Handle                        handle,
    TQ3Uns32                      validSize
);



/*!
 *  @function
 *      Q3HandleStorage_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param handle           Description of the parameter.
 *  @param validSize        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3HandleStorage_Get (
    TQ3StorageObject              storage,
    Handle                        *handle,
    TQ3Uns32                      *validSize
);



/*!
 *  @function
 *      Q3MacintoshStorage_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param fsRefNum         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3MacintoshStorage_New (
    TQ3Int16                      fsRefNum
);



/*!
 *  @function
 *      Q3MacintoshStorage_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param fsRefNum         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MacintoshStorage_Set (
    TQ3StorageObject              storage,
    TQ3Int16                      fsRefNum
);



/*!
 *  @function
 *      Q3MacintoshStorage_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param fsRefNum         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MacintoshStorage_Get (
    TQ3StorageObject              storage,
    TQ3Int16                      *fsRefNum
);



/*!
 *  @function
 *      Q3MacintoshStorage_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3MacintoshStorage_GetType (
    TQ3StorageObject              storage
);



/*!
 *  @function
 *      Q3FSSpecStorage_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param fs               Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3FSSpecStorage_New (
    const FSSpec                  *fs
);



/*!
 *  @function
 *      Q3FSSpecStorage_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param fs               Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3FSSpecStorage_Set (
    TQ3StorageObject              storage,
    const FSSpec                  *fs
);



/*!
 *  @function
 *      Q3FSSpecStorage_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param fs               Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param hFile            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3Win32Storage_New (
    HANDLE                        hFile
);



/*!
 *  @function
 *      Q3Win32Storage_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param hFile            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Win32Storage_Set (
    TQ3StorageObject              storage,
    HANDLE                        hFile
);



/*!
 *  @function
 *      Q3Win32Storage_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param hFile            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pathName         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3UnixPathStorage_New (
    const char                    *pathName
);



/*!
 *  @function
 *      Q3UnixPathStorage_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param pathName         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3UnixPathStorage_Set (
    TQ3StorageObject              storage,
    const char                    *pathName
);



/*!
 *  @function
 *      Q3UnixPathStorage_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param storage          Description of the parameter.
 *  @param pathName         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
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
 *      Not supported by QD3D.
 *
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3BeStorage_New (
    BFile                         *theFile
);



/*!
 *  @function
 *      Q3BeStorage_Set
 *  @discussion
 *      Not supported by QD3D.
 *
 *  @param theStorage       Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3BeStorage_Set (
    TQ3StorageObject              theStorage,
    BFile                         *theFile
);



/*!
 *  @function
 *      Q3BeStorage_Get
 *  @discussion
 *      Not supported by QD3D.
 *
 *  @param theStorage       Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
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



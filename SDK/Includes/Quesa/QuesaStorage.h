/*! @header QuesaStorage.h
        Declares the Quesa storage objects.
 */
/*  NAME:
        QuesaStorage.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
		#include <CarbonCore/MacTypes.h>
		#include <CarbonCore/Files.h>
  	#else
		#include <MacTypes.h>
		#include <Files.h>
	#endif

#elif QUESA_OS_WIN32
	#include <Windows.h>
#endif

#include <stdio.h>





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*
 *	Q3Storage_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Storage_GetType (
	TQ3StorageObject              storage
);



/*
 *	Q3Storage_GetSize
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Storage_GetSize (
	TQ3StorageObject              storage,
	TQ3Uns32                      *size
);



/*
 *	Q3Storage_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Storage_GetData (
	TQ3StorageObject              storage,
	TQ3Uns32                      offset,
	TQ3Uns32                      dataSize,
	unsigned char                 *data,
	TQ3Uns32                      *sizeRead
);



/*
 *	Q3Storage_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Storage_SetData (
	TQ3StorageObject              storage,
	TQ3Uns32                      offset,
	TQ3Uns32                      dataSize,
	const unsigned char           *data,
	TQ3Uns32                      *sizeWritten
);



/*
 *	Q3MemoryStorage_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3MemoryStorage_GetType (
	TQ3StorageObject              storage
);



/*
 *	Q3MemoryStorage_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3MemoryStorage_New (
	const unsigned char           *buffer,
	TQ3Uns32                      validSize
);



/*
 *	Q3MemoryStorage_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MemoryStorage_Set (
	TQ3StorageObject              storage,
	const unsigned char           *buffer,
	TQ3Uns32                      validSize
);



/*
 *	Q3MemoryStorage_NewBuffer
 *		Description of function
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3MemoryStorage_NewBuffer (
	unsigned char                 *buffer,
	TQ3Uns32                      validSize,
	TQ3Uns32                      bufferSize
);



/*
 *	Q3MemoryStorage_SetBuffer
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MemoryStorage_SetBuffer (
	TQ3StorageObject              storage,
	unsigned char                 *buffer,
	TQ3Uns32                      validSize,
	TQ3Uns32                      bufferSize
);



/*
 *	Q3MemoryStorage_GetBuffer
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MemoryStorage_GetBuffer (
	TQ3StorageObject              storage,
	unsigned char                 **buffer,
	TQ3Uns32                      *validSize,
	TQ3Uns32                      *bufferSize
);



/*
 *	Q3PathStorage_New
 *		Description of function
 *
 *		Note : Not supported by QD3D.
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3PathStorage_New (
	const char                    *pathName
);



/*
 *	Q3PathStorage_Set
 *		Description of function
 *
 *		Note : Not supported by QD3D.
 */
EXTERN_API_C ( TQ3Status  )
Q3PathStorage_Set (
	TQ3StorageObject              theStorage,
	const char                    *pathName
);



/*
 *	Q3PathStorage_Get
 *		Description of function
 *
 *		Note : Not supported by QD3D.
 */
EXTERN_API_C ( TQ3Status  )
Q3PathStorage_Get (
	TQ3StorageObject              theStorage,
	char                          *pathName
);





//=============================================================================
//      Mac OS function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

/*
 *	Q3HandleStorage_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3HandleStorage_New (
	Handle                        handle,
	TQ3Uns32                      validSize
);



/*
 *	Q3HandleStorage_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3HandleStorage_Set (
	TQ3StorageObject              storage,
	Handle                        handle,
	TQ3Uns32                      validSize
);



/*
 *	Q3HandleStorage_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3HandleStorage_Get (
	TQ3StorageObject              storage,
	Handle                        *handle,
	TQ3Uns32                      *validSize
);



/*
 *	Q3MacintoshStorage_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3MacintoshStorage_New (
	TQ3Int16                      fsRefNum
);



/*
 *	Q3MacintoshStorage_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MacintoshStorage_Set (
	TQ3StorageObject              storage,
	TQ3Int16                      fsRefNum
);



/*
 *	Q3MacintoshStorage_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MacintoshStorage_Get (
	TQ3StorageObject              storage,
	TQ3Int16                      *fsRefNum
);



/*
 *	Q3MacintoshStorage_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3MacintoshStorage_GetType (
	TQ3StorageObject              storage
);



/*
 *	Q3FSSpecStorage_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3FSSpecStorage_New (
	const FSSpec                  *fs
);



/*
 *	Q3FSSpecStorage_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FSSpecStorage_Set (
	TQ3StorageObject              storage,
	const FSSpec                  *fs
);



/*
 *	Q3FSSpecStorage_Get
 *		Description of function
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

/*
 *	Q3Win32Storage_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3Win32Storage_New (
	HANDLE                        hFile
);



/*
 *	Q3Win32Storage_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Win32Storage_Set (
	TQ3StorageObject              storage,
	HANDLE                        hFile
);



/*
 *	Q3Win32Storage_Get
 *		Description of function
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

/*
 *	Q3UnixPathStorage_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3UnixPathStorage_New (
	const char                    *pathName
);



/*
 *	Q3UnixPathStorage_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3UnixPathStorage_Set (
	TQ3StorageObject              storage,
	const char                    *pathName
);



/*
 *	Q3UnixPathStorage_Get
 *		Description of function
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

/*
 *	Q3BeStorage_New
 *		Description of function
 *
 *		Note : Not supported by QD3D.
 */
EXTERN_API_C ( TQ3StorageObject  )
Q3BeStorage_New (
	BFile                         *theFile
);



/*
 *	Q3BeStorage_Set
 *		Description of function
 *
 *		Note : Not supported by QD3D.
 */
EXTERN_API_C ( TQ3Status  )
Q3BeStorage_Set (
	TQ3StorageObject              theStorage,
	BFile                         *theFile
);



/*
 *	Q3BeStorage_Get
 *		Description of function
 *
 *		Note : Not supported by QD3D.
 */
EXTERN_API_C ( TQ3Status  )
Q3BeStorage_Get (
	TQ3StorageObject              theStorage,
	BFile                         **theFile
);

#endif // QUESA_OS_BE





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


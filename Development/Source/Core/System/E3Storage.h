/*  NAME:
        E3Storage.h

    DESCRIPTION:
        Header file for E3Storage.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#ifndef __E3STORAGE__
#define __E3STORAGE__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Cross platform
TQ3Status			E3Storage_RegisterClass(void);
TQ3Status			E3Storage_UnregisterClass(void);

TQ3ObjectType		E3Storage_GetType(TQ3StorageObject storage);
TQ3Status			E3Storage_GetSize(TQ3StorageObject storage, unsigned long *size);
TQ3Status			E3Storage_GetData(TQ3StorageObject storage, unsigned long offset, unsigned long dataSize, unsigned char *data, unsigned long *sizeRead);
TQ3Status			E3Storage_SetData(TQ3StorageObject storage, unsigned long offset, unsigned long dataSize, const unsigned char *data, unsigned long *sizeWritten);
TQ3ObjectType		E3MemoryStorage_GetType(TQ3StorageObject storage);
TQ3StorageObject	E3MemoryStorage_New(const unsigned char *buffer, unsigned long validSize);
TQ3Status			E3MemoryStorage_Set(TQ3StorageObject storage, const unsigned char *buffer, unsigned long validSize);
TQ3StorageObject	E3MemoryStorage_NewBuffer(unsigned char *buffer, unsigned long validSize, unsigned long bufferSize);
TQ3Status			E3MemoryStorage_SetBuffer(TQ3StorageObject storage, unsigned char *buffer, unsigned long validSize, unsigned long bufferSize);
TQ3Status			E3MemoryStorage_GetBuffer(TQ3StorageObject storage, unsigned char **buffer, unsigned long *validSize, unsigned long *bufferSize);
TQ3StorageObject	E3PathStorage_New(const char *pathName);
TQ3Status			E3PathStorage_Set(TQ3StorageObject theStorage, const char *pathName);
TQ3Status			E3PathStorage_Get(TQ3StorageObject theStorage, char *pathName);


// Mac specific
#if OS_MACINTOSH
TQ3Status			E3MacStorage_RegisterClass(void);
TQ3Status			E3MacStorage_UnregisterClass(void);

TQ3StorageObject	E3HandleStorage_New(Handle handle, unsigned long validSize);
TQ3Status			E3HandleStorage_Set(TQ3StorageObject storage, Handle handle, unsigned long validSize);
TQ3Status			E3HandleStorage_Get(TQ3StorageObject storage, Handle *handle, unsigned long *validSize);
TQ3StorageObject	E3MacintoshStorage_New(TQ3Int16 fsRefNum);
TQ3Status			E3MacintoshStorage_Set(TQ3StorageObject storage, TQ3Int16 fsRefNum);
TQ3Status			E3MacintoshStorage_Get(TQ3StorageObject storage, TQ3Int16 *fsRefNum);
TQ3ObjectType		E3MacintoshStorage_GetType(TQ3StorageObject storage);
TQ3StorageObject	E3FSSpecStorage_New(const FSSpec *fs);
TQ3Status			E3FSSpecStorage_Set(TQ3StorageObject storage, const FSSpec *fs);
TQ3Status			E3FSSpecStorage_Get(TQ3StorageObject storage, FSSpec *fs);
#endif


// Windows specific
#if OS_WIN32
TQ3Status			E3Win32Storage_RegisterClass(void);
TQ3Status			E3Win32Storage_UnregisterClass(void);

TQ3StorageObject	E3Win32Storage_New(HANDLE hFile);
TQ3Status			E3Win32Storage_Set(TQ3StorageObject storage, HANDLE hFile);
TQ3Status			E3Win32Storage_Get(TQ3StorageObject storage, HANDLE *hFile);
#endif


// Unix specific
#if OS_UNIX
TQ3Status			E3UnixStorage_RegisterClass(void);
TQ3Status			E3UnixStorage_UnregisterClass(void);

TQ3StorageObject	E3UnixPathStorage_New(const char *pathName);
TQ3Status			E3UnixPathStorage_Set(TQ3StorageObject storage, const char *pathName);
TQ3Status			E3UnixPathStorage_Get(TQ3StorageObject storage, char *pathName);
#endif


// Be specific
#if OS_BE
TQ3Status			E3BeStorage_RegisterClass(void);
TQ3Status			E3BeStorage_UnregisterClass(void);

TQ3StorageObject	E3BeStorage_New(BFile *theFile);
TQ3Status			E3BeStorage_Set(TQ3StorageObject theStorage, BFile *theFile);
TQ3Status			E3BeStorage_Get(TQ3StorageObject theStorage, BFile **theFile);
#endif





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


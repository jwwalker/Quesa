/*  NAME:
        E3Storage.h

    DESCRIPTION:
        Header file for E3Storage.c.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
#ifndef E3STORAGE_HDR
#define E3STORAGE_HDR
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
TQ3Status			E3Storage_GetSize(TQ3StorageObject storage, TQ3Uns32 *size);
TQ3Status			E3Storage_GetData(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead);
TQ3Status			E3Storage_SetData(TQ3StorageObject storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten);
TQ3ObjectType		E3MemoryStorage_GetType(TQ3StorageObject storage);
TQ3StorageObject	E3MemoryStorage_New(const unsigned char *buffer, TQ3Uns32 validSize);
TQ3Status			E3MemoryStorage_Set(TQ3StorageObject storage, const unsigned char *buffer, TQ3Uns32 validSize);
TQ3StorageObject	E3MemoryStorage_NewBuffer(unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize);
TQ3Status			E3MemoryStorage_SetBuffer(TQ3StorageObject storage, unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize);
TQ3Status			E3MemoryStorage_GetBuffer(TQ3StorageObject storage, unsigned char **buffer, TQ3Uns32 *validSize, TQ3Uns32 *bufferSize);
TQ3StorageObject	E3PathStorage_New(const char *pathName);
TQ3Status			E3PathStorage_Set(TQ3StorageObject theStorage, const char *pathName);
TQ3Status			E3PathStorage_Get(TQ3StorageObject theStorage, char *pathName);


// Mac specific
#if QUESA_OS_MACINTOSH
TQ3Status			E3MacStorage_RegisterClass(void);
TQ3Status			E3MacStorage_UnregisterClass(void);

TQ3StorageObject	E3HandleStorage_New(Handle handle, TQ3Uns32 validSize);
TQ3Status			E3HandleStorage_Set(TQ3StorageObject storage, Handle handle, TQ3Uns32 validSize);
TQ3Status			E3HandleStorage_Get(TQ3StorageObject storage, Handle *handle, TQ3Uns32 *validSize);
TQ3StorageObject	E3MacintoshStorage_New(TQ3Int16 fsRefNum);
TQ3Status			E3MacintoshStorage_Set(TQ3StorageObject storage, TQ3Int16 fsRefNum);
TQ3Status			E3MacintoshStorage_Get(TQ3StorageObject storage, TQ3Int16 *fsRefNum);
TQ3ObjectType		E3MacintoshStorage_GetType(TQ3StorageObject storage);
TQ3StorageObject	E3FSSpecStorage_New(const FSSpec *fs);
TQ3Status			E3FSSpecStorage_Set(TQ3StorageObject storage, const FSSpec *fs);
TQ3Status			E3FSSpecStorage_Get(TQ3StorageObject storage, FSSpec *fs);
#endif


// Windows specific
#if QUESA_OS_WIN32
TQ3Status			E3Win32Storage_RegisterClass(void);
TQ3Status			E3Win32Storage_UnregisterClass(void);

TQ3StorageObject	E3Win32Storage_New(HANDLE hFile);
TQ3Status			E3Win32Storage_Set(TQ3StorageObject storage, HANDLE hFile);
TQ3Status			E3Win32Storage_Get(TQ3StorageObject storage, HANDLE *hFile);
#endif


// Unix specific
#if QUESA_OS_UNIX
TQ3Status			E3UnixStorage_RegisterClass(void);
TQ3Status			E3UnixStorage_UnregisterClass(void);

TQ3StorageObject	E3UnixPathStorage_New(const char *pathName);
TQ3Status			E3UnixPathStorage_Set(TQ3StorageObject storage, const char *pathName);
TQ3Status			E3UnixPathStorage_Get(TQ3StorageObject storage, char *pathName);
#endif


// Be specific
#if QUESA_OS_BE
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


/*  NAME:
        E3Storage.h

    DESCRIPTION:
        Header file for E3Storage.cpp.

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
#ifndef E3STORAGE_HDR
#define E3STORAGE_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here


#include "E3Main.h"


//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Memory storage
typedef struct TE3_MemoryStorageData {
	TQ3Uns8			*buffer;
	TQ3Boolean		ownBuffer;
	TQ3Boolean		noCopy;
	TQ3Uns32		bufferSize;
	TQ3Uns32		validSize;
	TQ3Uns32		growSize;
} TE3_MemoryStorageData;


// Path storage
typedef struct TQ3PathStorageData {
	char		*thePath;
	FILE		*theFile;
	TQ3Uns32*	ownerCount;
} TQ3PathStorageData;




class E3StorageInfo : public E3SharedInfo
	{
	const TQ3XStorageReadDataMethod		getData_Method ;
	const TQ3XStorageWriteDataMethod	setData_Method ;
	const TQ3XStorageGetSizeMethod		getEOF_Method ;
	
public :

									E3StorageInfo	(
													TQ3XMetaHandler	newClassMetaHandler,
													E3ClassInfo*	newParent
					 								) ; // constructor	
	friend class E3Storage ;
	} ;




class E3Storage : public E3Shared
	{
Q3_CLASS_ENUMS ( kQ3SharedTypeStorage, E3Storage, E3Shared )

	// No new instance data for this class
	
public :

	static TQ3Boolean				IsOfMyClass ( TQ3Object object ) ;
	
	E3StorageInfo*					GetClass ( void ) { return (E3StorageInfo*) OpaqueTQ3Object::GetClass () ; }
	
	TQ3ObjectType					GetType ( void ) ;
	TQ3Status						GetSize ( TQ3Uns32* size ) ;
	TQ3Status						GetData ( TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char* data, TQ3Uns32* sizeRead ) ;
	TQ3Status						SetData ( TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char* data, TQ3Uns32* sizeWritten ) ;
	
	TQ3Status						Open( TQ3Boolean forWriting );
	TQ3Status						Close();
	TQ3Status						GetOpenness( TQ3StorageOpenness* outOpenness ); 
	} ;



class E3MemoryStorage : public E3Storage
	{
Q3_CLASS_ENUMS ( kQ3StorageTypeMemory, E3MemoryStorage, E3Storage )

public :
	TE3_MemoryStorageData		memoryDetails ;
	
	TQ3Status					Set ( const unsigned char *buffer, TQ3Uns32 validSize ) ;
	TQ3Status					SetBuffer ( unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize ) ;
	TQ3Status					GetBuffer ( unsigned char **buffer, TQ3Uns32 *validSize, TQ3Uns32 *bufferSize ) ;
	

	friend TQ3Status			e3storage_memory_read ( TQ3StorageObject inStorage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead ) ;
	friend TQ3Status			e3storage_memory_grow ( E3MemoryStorage* storage, TQ3Uns32 requestedSize ) ;
	friend TQ3Status			e3storage_memory_write ( E3MemoryStorage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten ) ;
	friend TQ3Status			e3storage_memory_getsize ( TQ3StorageObject inStorage, TQ3Uns32 *size ) ;
	} ;



class E3PathStorage : public E3Storage
	{
Q3_CLASS_ENUMS ( kQ3StorageTypePath, E3PathStorage, E3Storage )

public :
	TQ3PathStorageData		pathDetails ;
	
	TQ3Status					Set ( const char* pathName ) ;
	TQ3Status					Get ( char* pathName)  ;


	friend TQ3Status			e3storage_path_open ( TQ3StorageObject inStorage, TQ3Boolean forWriting ) ;
	friend TQ3Status			e3storage_path_close ( TQ3StorageObject inStorage ) ;
	friend TQ3Status			e3storage_path_getsize ( TQ3StorageObject inStorage, TQ3Uns32 *size ) ;
	friend TQ3Status			e3storage_path_read ( TQ3StorageObject inStorage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead ) ;
	friend TQ3Status			e3storage_path_write ( E3PathStorage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten ) ;
	friend TQ3Status			e3storage_path_getopenness( E3PathStorage* storage,
									TQ3StorageOpenness* outOpenness );
	} ;



class E3FileStreamStorage : public E3Storage
{
Q3_CLASS_ENUMS ( kQ3StorageTypeFileStream, E3FileStreamStorage, E3Storage )
	
public:
	FILE*		mStream;

	void						Set( FILE* stream );
	FILE*						Get();

	friend TQ3Status			e3storage_stream_getsize ( TQ3StorageObject inStorage, TQ3Uns32 *size ) ;
	friend TQ3Status			e3storage_stream_read ( TQ3StorageObject inStorage, TQ3Uns32 offset, TQ3Uns32 dataSize, unsigned char *data, TQ3Uns32 *sizeRead ) ;
	friend TQ3Status			e3storage_stream_write ( E3FileStreamStorage* storage, TQ3Uns32 offset, TQ3Uns32 dataSize, const unsigned char *data, TQ3Uns32 *sizeWritten ) ;
};


//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Cross platform
TQ3Status			E3Storage_RegisterClass(void);
TQ3Status			E3Storage_UnregisterClass(void);

TQ3ObjectType		E3MemoryStorage_GetType(TQ3StorageObject storage);
TQ3StorageObject	E3MemoryStorage_New(const unsigned char *buffer, TQ3Uns32 validSize);
TQ3StorageObject	E3MemoryStorage_NewNoCopy(unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize);
TQ3StorageObject	E3MemoryStorage_NewBuffer(unsigned char *buffer, TQ3Uns32 validSize, TQ3Uns32 bufferSize);
TQ3StorageObject	E3PathStorage_New(const char *pathName, TQ3Boolean owned);
TQ3StorageObject	E3FileStreamStorage_New(FILE *stream);



// Windows specific
#if QUESA_OS_WIN32
TQ3Status			E3Win32Storage_RegisterClass(void);
TQ3Status			E3Win32Storage_UnregisterClass(void);

TQ3StorageObject	E3Win32Storage_New(HANDLE hFile);
TQ3Status			E3Win32Storage_Set(TQ3StorageObject storage, HANDLE hFile);
TQ3Status			E3Win32Storage_Get(TQ3StorageObject storage, HANDLE *hFile);
#endif




//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


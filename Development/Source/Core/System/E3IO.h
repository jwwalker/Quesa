/*  NAME:
        E3IO.h

    DESCRIPTION:
        Header file for E3IO.c.

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
#ifndef E3IO_HDR
#define E3IO_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QuesaIO.h"
#include "E3Main.h"




//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
enum TE3FileStatus {
	kE3_File_Status_Closed     = 0L,
	kE3_File_Status_Reading,
	kE3_File_Status_Writing
};

typedef enum TE3FileStatus TE3FileStatus;

enum TE3FileReason {
	kE3_File_Reason_OK         = 0L,
	kE3_File_Reason_Cancelled
};

typedef enum TE3FileReason TE3FileReason;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// NB - the mode field is really the format type. For the 3DMF format
// the type is hard-coded.
typedef struct TE3FileData {
	TQ3StorageObject		storage;
	TQ3FileFormatObject		format;
	TQ3FileMode				mode;

	TE3FileStatus			status;
	TE3FileReason			reason;
	
	TQ3FileIdleMethod		idleMethod;
	const void*				idleData;
} TE3FileData;




class E3File : public E3Shared
	{
	TE3FileData				instanceData ;
	
public :
	void					CallIdle ( void ) ;
	
	TQ3Status				GetStorage ( TQ3StorageObject* storage ) ;
	TQ3Status				SetStorage ( TQ3StorageObject storage ) ;
	TQ3Status				OpenRead ( TQ3FileMode* mode ) ;
	TQ3Status				OpenWrite ( TQ3FileMode mode ) ;
	TQ3Status				IsOpen ( TQ3Boolean* isOpen ) ;
	TQ3Status				GetMode ( TQ3FileMode* mode ) ;
	TQ3Status				GetVersion ( TQ3FileVersion *version ) ;
	TQ3Status				Close ( void ) ;
	TQ3Status				Cancel ( void ) ;
	TQ3ObjectType			GetNextObjectType ( void ) ;
	TQ3Boolean				IsNextObjectOfType ( TQ3ObjectType ofType ) ;
	TQ3Object				ReadObject ( void ) ;
	TQ3Status				SkipObject ( void ) ;
	TQ3Boolean				IsEndOfData ( void ) ;
	TQ3Boolean				IsEndOfContainer ( TQ3Object rootObject ) ;
	TQ3Boolean				IsEndOfFile ( void ) ;
	TQ3Status				SetReadInGroup ( TQ3FileReadGroupState readGroupState ) ;
	TQ3Status				GetReadInGroup ( TQ3FileReadGroupState* readGroupState ) ;
	TQ3Status				SetIdleMethod ( TQ3FileIdleMethod idle, const void* idleData ) ;
	TQ3FileFormatObject		GetFileFormat ( void ) ;
	TE3FileStatus			GetFileStatus ( void ) ;

	friend TQ3Status		e3file_format_attach ( E3File* theFile,TQ3FileFormatObject theFileFormat ) ;
	} ;




//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3File_RegisterClass(void);
TQ3Status			E3File_UnregisterClass(void);

TQ3FileObject		E3File_New(void);

TQ3Status			E3File_MarkAsExternalReference(TQ3FileObject theFile, TQ3SharedObject sharedObject);
TQ3GroupObject		E3File_GetExternalReferences(TQ3FileObject theFile);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


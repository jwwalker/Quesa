/*  NAME:
        E3IO.h

    DESCRIPTION:
        Header file for E3IO.c.

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
#ifndef E3IO_HDR
#define E3IO_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QuesaIO.h"




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





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3File_RegisterClass(void);
TQ3Status			E3File_UnregisterClass(void);
void				E3File_CallIdle(TQ3FileObject theFile);

TQ3FileObject		E3File_New(void);
TQ3Status			E3File_GetStorage(TQ3FileObject theFile, TQ3StorageObject *storage);
TQ3Status			E3File_SetStorage(TQ3FileObject theFile, TQ3StorageObject storage);
TQ3Status			E3File_OpenRead(TQ3FileObject theFile, TQ3FileMode *mode);
TQ3Status			E3File_OpenWrite(TQ3FileObject theFile, TQ3FileMode mode);
TQ3Status			E3File_IsOpen(TQ3FileObject theFile, TQ3Boolean *isOpen);
TQ3Status			E3File_GetMode(TQ3FileObject theFile, TQ3FileMode *mode);
TQ3Status			E3File_GetVersion(TQ3FileObject theFile, TQ3FileVersion *version);
TQ3Status			E3File_Close(TQ3FileObject theFile);
TQ3Status			E3File_Cancel(TQ3FileObject theFile);
TQ3ObjectType		E3File_GetNextObjectType(TQ3FileObject theFile);
TQ3Boolean			E3File_IsNextObjectOfType(TQ3FileObject theFile, TQ3ObjectType ofType);
TQ3Object			E3File_ReadObject(TQ3FileObject theFile);
TQ3Status			E3File_SkipObject(TQ3FileObject theFile);
TQ3Boolean			E3File_IsEndOfData(TQ3FileObject theFile);
TQ3Boolean			E3File_IsEndOfContainer(TQ3FileObject theFile, TQ3Object rootObject);
TQ3Boolean			E3File_IsEndOfFile(TQ3FileObject theFile);
TQ3Status			E3File_MarkAsExternalReference(TQ3FileObject theFile, TQ3SharedObject sharedObject);
TQ3GroupObject		E3File_GetExternalReferences(TQ3FileObject theFile);
TQ3Status			E3File_SetReadInGroup(TQ3FileObject theFile, TQ3FileReadGroupState readGroupState);
TQ3Status			E3File_GetReadInGroup(TQ3FileObject theFile, TQ3FileReadGroupState *readGroupState);
TQ3Status			E3File_SetIdleMethod(TQ3FileObject theFile, TQ3FileIdleMethod idle, const void *idleData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


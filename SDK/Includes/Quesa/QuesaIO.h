/*  NAME:
        QuesaIO.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_IO_HDR
#define QUESA_IO_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

#include "QD3DDrawContext.h"
#include "QD3DView.h"
#include "QuesaRenderer.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif


//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// File format types
enum{
	kQ3ObjectTypeFileFormat									= Q3_OBJECT_TYPE('F', 'F', 'm', 't'),
		kQ3FileFormatTypeReader								= Q3_OBJECT_TYPE('F', 'm', 't', 'R'),
			kQ3FFormatReaderType3DMFBin						= Q3_OBJECT_TYPE('F', 'r', 'b', 'i'),
			kQ3FFormatReaderType3DMFBinSwapped				= Q3_OBJECT_TYPE('F', 'r', 'b', 's'),
			kQ3FFormatReaderType3DMFText					= Q3_OBJECT_TYPE('F', 'r', 't', 'x'),

		kQ3FileFormatTypeWriter								= Q3_OBJECT_TYPE('F', 'm', 't', 'W'),
			kQ3FFormatWriterType3DMFStreamBin				= Q3_OBJECT_TYPE('F', 'w', 's', 'b'),
			kQ3FFormatWriterType3DMFStreamText				= Q3_OBJECT_TYPE('F', 'w', 's', 't'),
			kQ3FFormatWriterType3DMFNormalBin				= Q3_OBJECT_TYPE('F', 'w', 'n', 'b'),
			kQ3FFormatWriterType3DMFNormalText				= Q3_OBJECT_TYPE('F', 'w', 'n', 't'),
			kQ3FFormatWriterType3DMFDatabaseBin				= Q3_OBJECT_TYPE('F', 'w', 'd', 'b'),
			kQ3FFormatWriterType3DMFDatabaseText			= Q3_OBJECT_TYPE('F', 'w', 'd', 't'),
			kQ3FFormatWriterType3DMFDatabaseStreamBin		= Q3_OBJECT_TYPE('F', 'd', 's', 'b'),
			kQ3FFormatWriterType3DMFDatabaseStreamText		= Q3_OBJECT_TYPE('F', 'd', 's', 't')
	};
	
// the old QD3D File modes
enum {
	kQ3FileModeNormal	= 0,
	kQ3FileModeStream	= 1,
	kQ3FileModeDatabase	= 2,
	kQ3FileModeText		= 4
	};



// File format methods
enum {
	// Common
	kQ3XMethodTypeFFormatClose									= Q3_METHOD_TYPE('F', 'c', 'l', 's'),
	kQ3XMethodTypeFFormatGetFormatType							= Q3_METHOD_TYPE('F', 'g', 'f', 't'),

	// Read
	kQ3XMethodTypeFFormatCanRead								= Q3_METHOD_TYPE('F', 'i', 'l', 'F'),
	kQ3XMethodTypeFFormatReadHeader								= Q3_METHOD_TYPE('F', 'r', 'h', 'd'),
	kQ3XMethodTypeFFormatReadObject								= Q3_METHOD_TYPE('F', 'r', 'o', 'b'),
	kQ3XMethodTypeFFormatSkipObject								= Q3_METHOD_TYPE('F', 's', 'o', 'b'),
	kQ3XMethodTypeFFormatGetNextType							= Q3_METHOD_TYPE('F', 'g', 'n', 't'),

	// Used for Q3XXX_ReadMethods, no strict need to override to implement a new format
	kQ3XMethodTypeFFormatFloat32Read							= Q3_METHOD_TYPE('F', 'f', '3', 'r'),
	kQ3XMethodTypeFFormatFloat64Read							= Q3_METHOD_TYPE('F', 'f', '6', 'r'),
	kQ3XMethodTypeFFormatInt8Read								= Q3_METHOD_TYPE('F', 'i', '8', 'r'),
	kQ3XMethodTypeFFormatInt16Read								= Q3_METHOD_TYPE('F', 'i', '1', 'r'),
	kQ3XMethodTypeFFormatInt32Read								= Q3_METHOD_TYPE('F', 'i', '3', 'r'),
	kQ3XMethodTypeFFormatInt64Read								= Q3_METHOD_TYPE('F', 'i', '6', 'r'),
	kQ3XMethodTypeFFormatStringRead								= Q3_METHOD_TYPE('F', 's', 't', 'r'),
	kQ3XMethodTypeFFormatRawRead								= Q3_METHOD_TYPE('F', 'r', 'w', 'r'),

	// Write
	kQ3XMethodTypeFFormatWriteHeader							= Q3_METHOD_TYPE('F', 'w', 'h', 'd'),

	// Used for Q3XXX_WriteMethods, no strict need to override to implement a new format
	kQ3XMethodTypeFFormatFloat32Write							= Q3_METHOD_TYPE('F', 'f', '3', 'w'),
	kQ3XMethodTypeFFormatFloat64Write							= Q3_METHOD_TYPE('F', 'f', '6', 'w'),
	kQ3XMethodTypeFFormatInt8Write								= Q3_METHOD_TYPE('F', 'i', '8', 'w'),
	kQ3XMethodTypeFFormatInt16Write								= Q3_METHOD_TYPE('F', 'i', '1', 'w'),
	kQ3XMethodTypeFFormatInt32Write								= Q3_METHOD_TYPE('F', 'i', '3', 'w'),
	kQ3XMethodTypeFFormatInt64Write								= Q3_METHOD_TYPE('F', 'i', '6', 'w'),
	kQ3XMethodTypeFFormatStringWrite							= Q3_METHOD_TYPE('F', 's', 't', 'w'),
	kQ3XMethodTypeFFormatRawWrite								= Q3_METHOD_TYPE('F', 'r', 'w', 'w')
};

// Object 3DMF I/O methods
enum {
	kQ3XMethodTypeObjectFileVersion								= Q3_METHOD_TYPE('v', 'e', 'r', 's'),
	// write
	kQ3XMethodTypeObjectTraverse								= Q3_METHOD_TYPE('t', 'r', 'v', 's'),
	kQ3XMethodTypeObjectTraverseData							= Q3_METHOD_TYPE('t', 'r', 'v', 'd'),
	kQ3XMethodTypeObjectWrite									= Q3_METHOD_TYPE('w', 'r', 'i', 't'),
	// read
	kQ3XMethodTypeObjectReadData								= Q3_METHOD_TYPE('r', 'd', 'd', 't'),
	kQ3XMethodTypeObjectRead									= Q3_METHOD_TYPE('r', 'e', 'a', 'd'),
	kQ3XMethodTypeObjectAttach									= Q3_METHOD_TYPE('a', 't', 't', 'c')
};


enum {
	kQ3FileReadWholeGroup			= 0,
	kQ3FileReadObjectsInGroup		= 1,
	kQ3FileCurrentlyInsideGroup		= 2
	};


//=============================================================================
//      Types
//-----------------------------------------------------------------------------

typedef TQ3Uns32						TQ3FileMode;
typedef TQ3Uns32						TQ3FileVersion;
typedef TQ3Uns32						TQ3FileReadGroupState;



// A FileFormat object derives from TQ3Object
typedef TQ3Object 						TQ3FileFormatObject;


// Method types

// 3DMF Write
typedef CALLBACK_API_C(TQ3Status,		TQ3XObjectTraverseMethod)		(TQ3Object object, void *data, TQ3ViewObject view);
typedef CALLBACK_API_C(TQ3Status,		TQ3XObjectTraverseDataMethod)	(TQ3Object object, void *data, TQ3ViewObject view);
typedef CALLBACK_API_C(TQ3Status,		TQ3XObjectWriteMethod)			(const void *object, TQ3FileObject theFile);
typedef CALLBACK_API_C(void,			TQ3XDataDeleteMethod)			(void *data);

// 3DMF Read
typedef CALLBACK_API_C(TQ3Object,		TQ3XObjectReadMethod)			(TQ3FileObject theFile);
typedef CALLBACK_API_C(TQ3Status,		TQ3XObjectReadDataMethod)		(TQ3Object parentObject, TQ3FileObject theFile);
typedef CALLBACK_API_C(TQ3Status,		TQ3XObjectAttachMethod)			(TQ3Object childObject, TQ3Object parentObject);



// Common
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatCloseMethod)			(TQ3FileFormatObject format, TQ3Boolean abort);
typedef CALLBACK_API_C(TQ3FileMode,		TQ3XFFormatGetFormatTypeMethod)	(TQ3FileObject theFile);

// Read
typedef CALLBACK_API_C(TQ3Boolean,		TQ3XFFormatCanReadMethod)		(TQ3StorageObject storage, TQ3ObjectType* theFileFormatFound);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatReadHeaderMethod)	(TQ3FileObject theFile);
typedef CALLBACK_API_C(TQ3Object,		TQ3XFFormatReadObjectMethod)	(TQ3FileObject theFile);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatSkipObjectMethod)	(TQ3FileObject theFile);
typedef CALLBACK_API_C(TQ3ObjectType,	TQ3XFFormatGetNextTypeMethod)	(TQ3FileObject theFile);

// Used for Q3XXX_ReadMethods, no strict need to override to implement a new Format
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatFloat32ReadMethod)	(TQ3FileFormatObject format, TQ3Float32* data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatFloat64ReadMethod)	(TQ3FileFormatObject format, TQ3Float64* data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatInt8ReadMethod)		(TQ3FileFormatObject format, TQ3Int8* data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatInt16ReadMethod)		(TQ3FileFormatObject format, TQ3Int16* data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatInt32ReadMethod)		(TQ3FileFormatObject format, TQ3Int32* data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatInt64ReadMethod)		(TQ3FileFormatObject format, TQ3Int64* data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatStringReadMethod)	(TQ3FileFormatObject format, char* data, TQ3Uns32 *length);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatRawReadMethod)		(TQ3FileFormatObject format, unsigned char* data, TQ3Uns32 length);

// Write
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatWriteHeaderMethod)	(TQ3FileObject theFile, TQ3FileMode mode);

// Used for Q3XXX_WriteMethods
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatFloat32WriteMethod)	(TQ3FileFormatObject format, TQ3Float32 data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatFloat64WriteMethod)	(TQ3FileFormatObject format, TQ3Float64 data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatInt8WriteMethod)		(TQ3FileFormatObject format, TQ3Int8 data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatInt16WriteMethod)	(TQ3FileFormatObject format, TQ3Int16 data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatInt32WriteMethod)	(TQ3FileFormatObject format, TQ3Int32 data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatInt64WriteMethod)	(TQ3FileFormatObject format, TQ3Int64 data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatStringWriteMethod)	(TQ3FileFormatObject format, const char* data);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatRawWriteMethod)		(TQ3FileFormatObject format, const unsigned char* data, TQ3Uns32 length);

// idling
typedef CALLBACK_API_C(TQ3Status,		TQ3FileIdleMethod)				(TQ3FileObject theFile, const void *idlerData);

// FileFormat common data (must be first field in struct)			
typedef struct {
	// Initialised by Quesa
	TQ3Uns32					baseDataVersion;
	TQ3StorageObject			storage;
	TQ3Uns32					currentStoragePosition;
	TQ3Uns32					logicalEOF;


	// Initialised by the importer
	TQ3FileVersion				fileVersion;
	TQ3Boolean					noMoreObjects;
	TQ3Endian					byteOrder;
	TQ3Boolean					readInGroup;
	TQ3Int32					groupDeepCounter;
	TQ3Uns32					*reserved1;
	TQ3Uns32					*reserved2;
	TQ3Uns32					*reserved3;
} TQ3FFormatBaseData;

// Unknown text
typedef struct {
	char						*objectName;
	char						*contents;
}TQ3UnknownTextData;

// Unknown text
typedef struct {
	TQ3ObjectType 					objectType;
	TQ3Size		 					size;
	TQ3Endian 						byteOrder;
	char							*contents;
} TQ3UnknownBinaryData;


//=============================================================================
//      Macros
//-----------------------------------------------------------------------------

// version
#define Q3FileVersion(_major, _minor)	(TQ3FileVersion) ((((TQ3Uns32) _major & 0xFFFF) << 16) | ((TQ3Uns32) _major & 0xFFFF))
#define kQ3FileVersionCurrent	Q3FileVersion(1,6)





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------

/*
 *	Q3XView_SubmitWriteData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XView_SubmitWriteData (
	TQ3ViewObject                 view,
	TQ3Size                       size,
	void                          *data,
	TQ3XDataDeleteMethod          deleteData
);



/*
 *	Q3XView_SubmitSubObjectData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XView_SubmitSubObjectData (
	TQ3ViewObject                 view,
	TQ3XObjectClass               objectClass,
	TQ3Uns32                      size,
	void                          *data,
	TQ3XDataDeleteMethod          deleteData
);



/*
 *	Q3File_New
 *		Description of function
 */
EXTERN_API_C ( TQ3FileObject  )
Q3File_New (
	void
);



/*
 *	Q3File_GetStorage
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_GetStorage (
	TQ3FileObject                 theFile,
	TQ3StorageObject              *storage
);



/*
 *	Q3File_SetStorage
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_SetStorage (
	TQ3FileObject                 theFile,
	TQ3StorageObject              storage
);



/*
 *	Q3File_OpenRead
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_OpenRead (
	TQ3FileObject                 theFile,
	TQ3FileMode                   *mode
);



/*
 *	Q3File_OpenWrite
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_OpenWrite (
	TQ3FileObject                 theFile,
	TQ3FileMode                   mode
);



/*
 *	Q3File_IsOpen
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_IsOpen (
	TQ3FileObject                 theFile,
	TQ3Boolean                    *isOpen
);



/*
 *	Q3File_GetMode
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_GetMode (
	TQ3FileObject                 theFile,
	TQ3FileMode                   *mode
);



/*
 *	Q3File_GetVersion
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_GetVersion (
	TQ3FileObject                 theFile,
	TQ3FileVersion                *version
);



/*
 *	Q3File_Close
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_Close (
	TQ3FileObject                 theFile
);



/*
 *	Q3File_Cancel
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_Cancel (
	TQ3FileObject                 theFile
);



/*
 *	Q3View_StartWriting
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_StartWriting (
	TQ3ViewObject                 view,
	TQ3FileObject                 theFile
);



/*
 *	Q3View_EndWriting
 *		Description of function
 */
EXTERN_API_C ( TQ3ViewStatus  )
Q3View_EndWriting (
	TQ3ViewObject                 view
);



/*
 *	Q3File_GetNextObjectType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3File_GetNextObjectType (
	TQ3FileObject                 theFile
);



/*
 *	Q3File_IsNextObjectOfType
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3File_IsNextObjectOfType (
	TQ3FileObject                 theFile,
	TQ3ObjectType                 ofType
);



/*
 *	Q3File_ReadObject
 *		Description of function
 */
EXTERN_API_C ( TQ3Object  )
Q3File_ReadObject (
	TQ3FileObject                 theFile
);



/*
 *	Q3File_SkipObject
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_SkipObject (
	TQ3FileObject                 theFile
);



/*
 *	Q3File_IsEndOfData
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3File_IsEndOfData (
	TQ3FileObject                 theFile
);



/*
 *	Q3File_IsEndOfContainer
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3File_IsEndOfContainer (
	TQ3FileObject                 theFile,
	TQ3Object                     rootObject
);



/*
 *	Q3File_IsEndOfFile
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3File_IsEndOfFile (
	TQ3FileObject                 theFile
);



/*
 *	Q3File_MarkAsExternalReference
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_MarkAsExternalReference (
	TQ3FileObject                 theFile,
	TQ3SharedObject               sharedObject
);



/*
 *	Q3File_GetExternalReferences
 *		Description of function
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3File_GetExternalReferences (
	TQ3FileObject                 theFile
);



/*
 *	Q3Shared_ClearEditTracking
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shared_ClearEditTracking (
	TQ3SharedObject               sharedObject
);



/*
 *	Q3Shared_GetEditTrackingState
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Shared_GetEditTrackingState (
	TQ3SharedObject               sharedObject
);



/*
 *	Q3File_SetReadInGroup
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_SetReadInGroup (
	TQ3FileObject                 theFile,
	TQ3FileReadGroupState         readGroupState
);



/*
 *	Q3File_GetReadInGroup
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_GetReadInGroup (
	TQ3FileObject                 theFile,
	TQ3FileReadGroupState         *readGroupState
);



/*
 *	Q3File_SetIdleMethod
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3File_SetIdleMethod (
	TQ3FileObject                 theFile,
	TQ3FileIdleMethod             idle,
	const void                    *idleData
);



/*
 *	Q3NewLine_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3NewLine_Write (
	TQ3FileObject                 theFile
);



/*
 *	Q3Uns8_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns8_Read (
	TQ3Uns8                       *data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Uns8_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns8_Write (
	TQ3Uns8                       data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Uns16_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns16_Read (
	TQ3Uns16                      *data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Uns16_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns16_Write (
	TQ3Uns16                      data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Uns32_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns32_Read (
	TQ3Uns32                      *data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Uns32_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns32_Write (
	TQ3Uns32                      data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Int8_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Int8_Read (
	TQ3Int8                       *data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Int8_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Int8_Write (
	TQ3Int8                       data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Int16_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Int16_Read (
	TQ3Int16                      *data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Int16_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Int16_Write (
	TQ3Int16                      data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Int32_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Int32_Read (
	TQ3Int32                      *data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Int32_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Int32_Write (
	TQ3Int32                      data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Uns64_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns64_Read (
	TQ3Uns64                      *data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Uns64_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns64_Write (
	TQ3Uns64                      data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Int64_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Int64_Read (
	TQ3Int64                      *data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Int64_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Int64_Write (
	TQ3Int64                      data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Float32_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Float32_Read (
	TQ3Float32                    *data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Float32_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Float32_Write (
	TQ3Float32                    data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Float64_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Float64_Read (
	TQ3Float64                    *data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Float64_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Float64_Write (
	TQ3Float64                    data,
	TQ3FileObject                 theFile
);



/*
 *	Q3Size_Pad
 *		Description of function
 */
EXTERN_API_C ( TQ3Size  )
Q3Size_Pad (
	TQ3Size                       size
);



/*
 *	Q3String_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3String_Read (
	char                          *data,
	TQ3Uns32                      *length,
	TQ3FileObject                 theFile
);



/*
 *	Q3String_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3String_Write (
	const char                    *data,
	TQ3FileObject                 theFile
);



/*
 *	Q3RawData_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RawData_Read (
	unsigned char                 *data,
	TQ3Uns32                      size,
	TQ3FileObject                 theFile
);



/*
 *	Q3RawData_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RawData_Write (
	const unsigned char           *data,
	TQ3Uns32                      size,
	TQ3FileObject                 theFile
);



/*
 *	Q3Point2D_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point2D_Read (
	TQ3Point2D                    *point2D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Point2D_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point2D_Write (
	const TQ3Point2D              *point2D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Point3D_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point3D_Read (
	TQ3Point3D                    *point3D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Point3D_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Point3D_Write (
	const TQ3Point3D              *point3D,
	TQ3FileObject                 theFile
);



/*
 *	Q3RationalPoint3D_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RationalPoint3D_Read (
	TQ3RationalPoint3D            *point3D,
	TQ3FileObject                 theFile
);



/*
 *	Q3RationalPoint3D_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RationalPoint3D_Write (
	const TQ3RationalPoint3D      *point3D,
	TQ3FileObject                 theFile
);



/*
 *	Q3RationalPoint4D_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RationalPoint4D_Read (
	TQ3RationalPoint4D            *point4D,
	TQ3FileObject                 theFile
);



/*
 *	Q3RationalPoint4D_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RationalPoint4D_Write (
	const TQ3RationalPoint4D      *point4D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Vector2D_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Vector2D_Read (
	TQ3Vector2D                   *vector2D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Vector2D_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Vector2D_Write (
	const TQ3Vector2D             *vector2D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Vector3D_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Vector3D_Read (
	TQ3Vector3D                   *vector3D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Vector3D_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Vector3D_Write (
	const TQ3Vector3D             *vector3D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Matrix4x4_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Matrix4x4_Read (
	TQ3Matrix4x4                  *matrix4x4,
	TQ3FileObject                 theFile
);



/*
 *	Q3Matrix4x4_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Matrix4x4_Write (
	const TQ3Matrix4x4            *matrix4x4,
	TQ3FileObject                 theFile
);



/*
 *	Q3Tangent2D_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tangent2D_Read (
	TQ3Tangent2D                  *tangent2D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Tangent2D_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tangent2D_Write (
	const TQ3Tangent2D            *tangent2D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Tangent3D_Read
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tangent3D_Read (
	TQ3Tangent3D                  *tangent3D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Tangent3D_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tangent3D_Write (
	const TQ3Tangent3D            *tangent3D,
	TQ3FileObject                 theFile
);



/*
 *	Q3Comment_Write
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Comment_Write (
	char                          *comment,
	TQ3FileObject                 theFile
);



/*
 *	Q3Unknown_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Unknown_GetType (
	TQ3UnknownObject              unknownObject
);



/*
 *	Q3Unknown_GetDirtyState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Unknown_GetDirtyState (
	TQ3UnknownObject              unknownObject,
	TQ3Boolean                    *isDirty
);



/*
 *	Q3Unknown_SetDirtyState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Unknown_SetDirtyState (
	TQ3UnknownObject              unknownObject,
	TQ3Boolean                    isDirty
);



/*
 *	Q3UnknownText_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownText_GetData (
	TQ3UnknownObject              unknownObject,
	TQ3UnknownTextData            *unknownTextData
);



/*
 *	Q3UnknownText_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownText_EmptyData (
	TQ3UnknownTextData            *unknownTextData
);



/*
 *	Q3UnknownBinary_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_GetData (
	TQ3UnknownObject              unknownObject,
	TQ3UnknownBinaryData          *unknownBinaryData
);



/*
 *	Q3UnknownBinary_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_EmptyData (
	TQ3UnknownBinaryData          *unknownBinaryData
);



/*
 *	Q3UnknownBinary_GetTypeString
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_GetTypeString (
	TQ3UnknownObject              unknownObject,
	char                          **typeString
);



/*
 *	Q3UnknownBinary_EmptyTypeString
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_EmptyTypeString (
	char                          **typeString
);



/*
 *	Q3ViewHints_New
 *		Description of function
 */
EXTERN_API_C ( TQ3ViewHintsObject  )
Q3ViewHints_New (
	TQ3ViewObject                 view
);



/*
 *	Q3ViewHints_SetRenderer
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetRenderer (
	TQ3ViewHintsObject            viewHints,
	TQ3RendererObject             renderer
);



/*
 *	Q3ViewHints_GetRenderer
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetRenderer (
	TQ3ViewHintsObject            viewHints,
	TQ3RendererObject             *renderer
);



/*
 *	Q3ViewHints_SetCamera
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetCamera (
	TQ3ViewHintsObject            viewHints,
	TQ3CameraObject               camera
);



/*
 *	Q3ViewHints_GetCamera
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetCamera (
	TQ3ViewHintsObject            viewHints,
	TQ3CameraObject               *camera
);



/*
 *	Q3ViewHints_SetLightGroup
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetLightGroup (
	TQ3ViewHintsObject            viewHints,
	TQ3GroupObject                lightGroup
);



/*
 *	Q3ViewHints_GetLightGroup
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetLightGroup (
	TQ3ViewHintsObject            viewHints,
	TQ3GroupObject                *lightGroup
);



/*
 *	Q3ViewHints_SetAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetAttributeSet (
	TQ3ViewHintsObject            viewHints,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3ViewHints_GetAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetAttributeSet (
	TQ3ViewHintsObject            viewHints,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3ViewHints_SetDimensionsState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetDimensionsState (
	TQ3ViewHintsObject            viewHints,
	TQ3Boolean                    isValid
);



/*
 *	Q3ViewHints_GetDimensionsState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetDimensionsState (
	TQ3ViewHintsObject            viewHints,
	TQ3Boolean                    *isValid
);



/*
 *	Q3ViewHints_SetDimensions
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetDimensions (
	TQ3ViewHintsObject            viewHints,
	TQ3Uns32                      width,
	TQ3Uns32                      height
);



/*
 *	Q3ViewHints_GetDimensions
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetDimensions (
	TQ3ViewHintsObject            viewHints,
	TQ3Uns32                      *width,
	TQ3Uns32                      *height
);



/*
 *	Q3ViewHints_SetMaskState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetMaskState (
	TQ3ViewHintsObject            viewHints,
	TQ3Boolean                    isValid
);



/*
 *	Q3ViewHints_GetMaskState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetMaskState (
	TQ3ViewHintsObject            viewHints,
	TQ3Boolean                    *isValid
);



/*
 *	Q3ViewHints_SetMask
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetMask (
	TQ3ViewHintsObject            viewHints,
	const TQ3Bitmap               *mask
);



/*
 *	Q3ViewHints_GetMask
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetMask (
	TQ3ViewHintsObject            viewHints,
	TQ3Bitmap                     *mask
);



/*
 *	Q3ViewHints_SetClearImageMethod
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetClearImageMethod (
	TQ3ViewHintsObject            viewHints,
	TQ3DrawContextClearImageMethod clearMethod
);



/*
 *	Q3ViewHints_GetClearImageMethod
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetClearImageMethod (
	TQ3ViewHintsObject            viewHints,
	TQ3DrawContextClearImageMethod *clearMethod
);



/*
 *	Q3ViewHints_SetClearImageColor
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetClearImageColor (
	TQ3ViewHintsObject            viewHints,
	const TQ3ColorARGB            *color
);



/*
 *	Q3ViewHints_GetClearImageColor
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetClearImageColor (
	TQ3ViewHintsObject            viewHints,
	TQ3ColorARGB                  *color
);



/*
 *	Q3File_GetFileFormat
 *		Description of function
 */
EXTERN_API_C ( TQ3FileFormatObject  )
Q3File_GetFileFormat (
	TQ3FileObject                 theFile
);



/*
 *	Q3FileFormat_NewFromType
 *		Description of function
 */
EXTERN_API_C ( TQ3FileFormatObject  )
Q3FileFormat_NewFromType (
	TQ3ObjectType                 fformatObjectType
);



/*
 *	Q3FileFormat_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3FileFormat_GetType (
	TQ3FileFormatObject           format
);



/*
 *	Q3FileFormat_HasModalConfigure
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3FileFormat_HasModalConfigure (
	TQ3FileFormatObject           format
);



/*
 *	Q3FileFormat_ModalConfigure
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_ModalConfigure (
	TQ3FileFormatObject           format,
	TQ3DialogAnchor               dialogAnchor,
	TQ3Boolean                    *canceled
);



/*
 *	Q3FileFormatClass_GetFormatNameString
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormatClass_GetFormatNameString (
	TQ3ObjectType                 formatClassType,
	TQ3ObjectClassNameString      formatClassString
);



/*
 *	Q3FileFormat_GetConfigurationData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GetConfigurationData (
	TQ3FileFormatObject           format,
	unsigned char                 *dataBuffer,
	TQ3Uns32                      bufferSize,
	TQ3Uns32                      *actualDataSize
);



/*
 *	Q3FileFormat_SetConfigurationData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_SetConfigurationData (
	TQ3RendererObject             format,
	unsigned char                 *dataBuffer,
	TQ3Uns32                      bufferSize
);





//=============================================================================
//      Binary reading utilities
//-----------------------------------------------------------------------------
/*
 *	Q3FileFormat_GenericReadBinary_8
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_8 (
	TQ3FileFormatObject           format,
	TQ3Int8                       *data
);



/*
 *	Q3FileFormat_GenericReadBinary_16
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_16 (
	TQ3FileFormatObject           format,
	TQ3Int16                      *data
);



/*
 *	Q3FileFormat_GenericReadBinary_32
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_32 (
	TQ3FileFormatObject           format,
	TQ3Int32                      *data
);



/*
 *	Q3FileFormat_GenericReadBinary_64
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_64 (
	TQ3FileFormatObject           format,
	TQ3Int64                      *data
);



/*
 *	Q3FileFormat_GenericReadBinary_String
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_String (
	TQ3FileFormatObject           format,
	char                          *data,
	TQ3Uns32                      *length
);



/*
 *	Q3FileFormat_GenericReadBinary_Raw
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_Raw (
	TQ3FileFormatObject           format,
	unsigned char                 *data,
	TQ3Uns32                      length
);





//=============================================================================
//      Swapped binary reading utilities
//-----------------------------------------------------------------------------
/*
 *	Q3FileFormat_GenericReadBinSwap_16
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinSwap_16 (
	TQ3FileFormatObject           format,
	TQ3Int16                      *data
);



/*
 *	Q3FileFormat_GenericReadBinSwap_32
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinSwap_32 (
	TQ3FileFormatObject           format,
	TQ3Int32                      *data
);



/*
 *	Q3FileFormat_GenericReadBinSwap_64
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinSwap_64 (
	TQ3FileFormatObject           format,
	TQ3Int64                      *data
);





//=============================================================================
//      Text reading utilities
//-----------------------------------------------------------------------------
/*
 *	Q3FileFormat_GenericReadText_SkipBlanks
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadText_SkipBlanks (
	TQ3FileFormatObject           format
);



/*
 *	Q3FileFormat_GenericReadText_ReadUntilChars
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadText_ReadUntilChars (
	TQ3FileFormatObject           format,
	char                          *buffer,
	char                          *chars,
	TQ3Uns32                      numChars,
	TQ3Boolean                    blanks,
	TQ3Int32                      *foundChar,
	TQ3Uns32                      maxLen,
	TQ3Uns32                      *charsRead
);





//=============================================================================
//      Binary writing utilities
//-----------------------------------------------------------------------------
/*
 *	Q3FileFormat_GenericWriteBinary_8
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_8 (
	TQ3FileFormatObject           format,
	TQ3Int8                       data
);



/*
 *	Q3FileFormat_GenericWriteBinary_16
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_16 (
	TQ3FileFormatObject           format,
	TQ3Int16                      data
);



/*
 *	Q3FileFormat_GenericWriteBinary_32
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_32 (
	TQ3FileFormatObject           format,
	TQ3Int32                      data
);



/*
 *	Q3FileFormat_GenericWriteBinary_64
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_64 (
	TQ3FileFormatObject           format,
	TQ3Int64                      data
);



/*
 *	Q3FileFormat_GenericWriteBinary_String
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_String (
	TQ3FileFormatObject           format,
	const char                    *data,
	TQ3Uns32                      *length
);



/*
 *	Q3FileFormat_GenericWriteBinary_Raw
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_Raw (
	TQ3FileFormatObject           format,
	const unsigned char           *data,
	TQ3Uns32                      length
);





//=============================================================================
//      Swapped binary writing utilities
//-----------------------------------------------------------------------------
/*
 *	Q3FileFormat_GenericWriteBinSwap_16
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinSwap_16 (
	TQ3FileFormatObject           format,
	TQ3Int16                      data
);



/*
 *	Q3FileFormat_GenericWriteBinSwap_32
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinSwap_32 (
	TQ3FileFormatObject           format,
	TQ3Int32                      data
);



/*
 *	Q3FileFormat_GenericWriteBinSwap_64
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinSwap_64 (
	TQ3FileFormatObject           format,
	TQ3Int64                      data
);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


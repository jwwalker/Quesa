/*  NAME:
        QuesaIO.h

    DESCRIPTION:
        Defines extensions over and above the normal QD3D I/O API.

		The QD3D Q3File class has been divided in two objects:
		
			- E3File, acts as interface for QD3D
			- E3FileFormat, dynamically loaded act as the real data IO object
		
		Each subclass of E3FileFormat manages a file format. The currently
		planned E3FileFormat hierarchy is similar to the hierarchy planned
		for kE3ObjectTypeFileFormat.

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
#ifndef __QUESAIO_
#define __QUESAIO_
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include <QD3DIO.h>
#include <QD3DRenderer.h>

// we sould move test like this to a Quesa.h file
#ifndef QUESA_HOST_IS_BIG_ENDIAN
	#define QUESA_HOST_IS_BIG_ENDIAN TARGET_RT_BIG_ENDIAN
#endif

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
#define kQ3ObjectTypeFileFormat									((TQ3ObjectType) FOUR_CHAR_CODE('FFmt'))
	#define kQ3FileFormatTypeReader								((TQ3ObjectType) FOUR_CHAR_CODE('FmtR'))
		#define kQ3FFormatReaderType3DMFBin						((TQ3ObjectType) FOUR_CHAR_CODE('Frbi'))
		#define kQ3FFormatReaderType3DMFBinSwapped				((TQ3ObjectType) FOUR_CHAR_CODE('Frbs'))
		#define kQ3FFormatReaderType3DMFText					((TQ3ObjectType) FOUR_CHAR_CODE('Frtx'))

	#define kQ3FileFormatTypeWriter								((TQ3ObjectType) FOUR_CHAR_CODE('FmtW'))
		#define kQ3FFormatWriterType3DMFStreamBin				((TQ3ObjectType) FOUR_CHAR_CODE('Fwsb'))
		#define kQ3FFormatWriterType3DMFStreamText				((TQ3ObjectType) FOUR_CHAR_CODE('Fwst'))
		#define kQ3FFormatWriterType3DMFNormalBin				((TQ3ObjectType) FOUR_CHAR_CODE('Fwnb'))
		#define kQ3FFormatWriterType3DMFNormalText				((TQ3ObjectType) FOUR_CHAR_CODE('Fwnt'))
		#define kQ3FFormatWriterType3DMFDatabaseBin				((TQ3ObjectType) FOUR_CHAR_CODE('Fwdb'))
		#define kQ3FFormatWriterType3DMFDatabaseText			((TQ3ObjectType) FOUR_CHAR_CODE('Fwdt'))
		#define kQ3FFormatWriterType3DMFDatabaseStreamBin		((TQ3ObjectType) FOUR_CHAR_CODE('Fdsb'))
		#define kQ3FFormatWriterType3DMFDatabaseStreamText		((TQ3ObjectType) FOUR_CHAR_CODE('Fdst'))


// File format methods
enum {
	// Common
	kQ3XMethodTypeFFormatClose									= FOUR_CHAR_CODE('Fcls'),
	kQ3XMethodTypeFFormatGetFormatType							= FOUR_CHAR_CODE('Fgft'),

	// Read
	kQ3XMethodTypeFFormatCanRead								= FOUR_CHAR_CODE('FilF'),
	kQ3XMethodTypeFFormatReadHeader								= FOUR_CHAR_CODE('Frhd'),
	kQ3XMethodTypeFFormatReadObject								= FOUR_CHAR_CODE('Frob'),
	kQ3XMethodTypeFFormatSkipObject								= FOUR_CHAR_CODE('Fsob'),
	kQ3XMethodTypeFFormatGetNextType							= FOUR_CHAR_CODE('Fgnt'),

	// Used for Q3XXX_ReadMethods, no strict need to override to implement a new format
	kQ3XMethodTypeFFormatFloat32Read							= FOUR_CHAR_CODE('Ff3r'),
	kQ3XMethodTypeFFormatFloat64Read							= FOUR_CHAR_CODE('Ff6r'),
	kQ3XMethodTypeFFormatInt8Read								= FOUR_CHAR_CODE('Fi8r'),
	kQ3XMethodTypeFFormatInt16Read								= FOUR_CHAR_CODE('Fi1r'),
	kQ3XMethodTypeFFormatInt32Read								= FOUR_CHAR_CODE('Fi3r'),
	kQ3XMethodTypeFFormatInt64Read								= FOUR_CHAR_CODE('Fi6r'),
	kQ3XMethodTypeFFormatStringRead								= FOUR_CHAR_CODE('Fstr'),
	kQ3XMethodTypeFFormatRawRead								= FOUR_CHAR_CODE('Frwr'),

	// Write
	kQ3XMethodTypeFFormatWriteHeader							= FOUR_CHAR_CODE('Fwhd'),

	// Used for Q3XXX_WriteMethods, no strict need to override to implement a new format
	kQ3XMethodTypeFFormatFloat32Write							= FOUR_CHAR_CODE('Ff3w'),
	kQ3XMethodTypeFFormatFloat64Write							= FOUR_CHAR_CODE('Ff6w'),
	kQ3XMethodTypeFFormatInt8Write								= FOUR_CHAR_CODE('Fi8w'),
	kQ3XMethodTypeFFormatInt16Write								= FOUR_CHAR_CODE('Fi1w'),
	kQ3XMethodTypeFFormatInt32Write								= FOUR_CHAR_CODE('Fi3w'),
	kQ3XMethodTypeFFormatInt64Write								= FOUR_CHAR_CODE('Fi6w'),
	kQ3XMethodTypeFFormatStringWrite							= FOUR_CHAR_CODE('Fstw'),
	kQ3XMethodTypeFFormatRawWrite								= FOUR_CHAR_CODE('Frww')
};





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// A FileFormat object derives from TQ3Object
typedef TQ3Object TQ3FileFormatObject;


// Method types

//Common
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
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatStringReadMethod)	(TQ3FileFormatObject format, char* data, unsigned long *length);
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatRawReadMethod)		(TQ3FileFormatObject format, unsigned char* data, unsigned long length);

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
typedef CALLBACK_API_C(TQ3Status,		TQ3XFFormatRawWriteMethod)		(TQ3FileFormatObject format, const unsigned char* data, unsigned long length);


// FileFormat common data (must be first field in struct)			
typedef struct {
	// Initialised by Quesa
	TQ3Uns32						baseDataVersion;
	TQ3StorageObject				storage;
	TQ3Uns32						currentStoragePosition;
	TQ3Uns32						logicalEOF;


	// Initialised by the importer
	TQ3FileVersion					fileVersion;
	TQ3Boolean						noMoreObjects;
	TQ3Endian						byteOrder;
	TQ3Boolean						readInGroup;
	TQ3Int32						groupDeepCounter;
	TQ3Uns32						*reserved1;
	TQ3Uns32						*reserved2;
	TQ3Uns32						*reserved3;
} TQ3FFormatBaseData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
EXTERN_API_C( TQ3FileFormatObject )
Q3File_GetFileFormat			(TQ3FileObject theFile);

EXTERN_API_C( TQ3FileFormatObject )
Q3FileFormat_NewFromType		(TQ3ObjectType fformatObjectType);

EXTERN_API_C( TQ3ObjectType )
Q3FileFormat_GetType			(TQ3FileFormatObject 	format);


// Binary reading utilities
EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericReadBinary_8		(TQ3FileFormatObject	format,
										 TQ3Int8				*data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericReadBinary_16		(TQ3FileFormatObject	format,
										 TQ3Int16				*data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericReadBinary_32		(TQ3FileFormatObject	format,
										 TQ3Int32				*data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericReadBinary_64		(TQ3FileFormatObject	format,
										 TQ3Int64				*data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericReadBinary_String	(TQ3FileFormatObject	format,
										 char					*data,
										 unsigned long			*length);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericReadBinary_Raw		(TQ3FileFormatObject	format,
										 unsigned char			*data,
										 unsigned long			length);


// Swapping binary reading utilities
EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericReadBinSwap_16		(TQ3FileFormatObject	format,
										 TQ3Int16				*data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericReadBinSwap_32		(TQ3FileFormatObject	format,
										 TQ3Int32				*data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericReadBinSwap_64		(TQ3FileFormatObject	format,
										 TQ3Int64				*data);


// Text reading utilities
EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericReadText_SkipBlanks		(TQ3FileFormatObject	format);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericReadText_ReadUntilChars	(TQ3FileFormatObject	format,
											 char					*buffer,
											 char					*chars,
											 TQ3Uns32				numChars,
											 TQ3Boolean				blanks,
											 TQ3Int32				*foundChar,
											 TQ3Uns32				maxLen,
											 TQ3Uns32				*charsRead);


// Binary writing utilities
EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericWriteBinary_8		(TQ3FileFormatObject	format,
										 TQ3Int8				data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericWriteBinary_16		(TQ3FileFormatObject	format,
										 TQ3Int16				data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericWriteBinary_32		(TQ3FileFormatObject	format,
										 TQ3Int32				data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericWriteBinary_64		(TQ3FileFormatObject	format,
										 TQ3Int64				data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericWriteBinary_String	(TQ3FileFormatObject	format,
										 const char				*data,
										 unsigned long			*length);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericWriteBinary_Raw		(TQ3FileFormatObject	format,
										 const unsigned char	*data,
										 unsigned long			length);


// Swapping binary writing utilities
EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericWriteBinSwap_16		(TQ3FileFormatObject	format,
										 TQ3Int16				data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericWriteBinSwap_32		(TQ3FileFormatObject	format,
										 TQ3Int32				data);

EXTERN_API_C( TQ3Status )
Q3FileFormat_GenericWriteBinSwap_64		(TQ3FileFormatObject	format,
										 TQ3Int64				data);



/*
	Writers API
	For Documentation about use refer to the Renderer counterparts in QD3DRenderer.h
*/

EXTERN_API_C( TQ3Boolean )
Q3FileFormat_HasModalConfigure	(TQ3FileFormatObject 	format);

EXTERN_API_C( TQ3Status )
Q3FileFormat_ModalConfigure		(TQ3FileFormatObject 	format,
								 TQ3DialogAnchor 		dialogAnchor,
								 TQ3Boolean *			canceled);

EXTERN_API_C( TQ3Status )
Q3FileFormatClass_GetFormatNameString (TQ3ObjectType 		formatClassType,
								 TQ3ObjectClassNameString  formatClassString);


EXTERN_API_C( TQ3Status )
Q3FileFormat_GetConfigurationData	(TQ3FileFormatObject 	format,
								 unsigned char *		dataBuffer,
								 unsigned long 			bufferSize,
								 unsigned long *		actualDataSize);

EXTERN_API_C( TQ3Status )
Q3FileFormat_SetConfigurationData	(TQ3FileFormatObject 	format,
								 unsigned char *		dataBuffer,
								 unsigned long 			bufferSize);



//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


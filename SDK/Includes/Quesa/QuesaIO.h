/*! @header QuesaIO.h
        Declares the Quesa IO objects.
 */
/*  NAME:
        QuesaIO.h

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
#ifndef QUESA_IO_HDR
#define QUESA_IO_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

#include "QuesaDrawContext.h"
#include "QuesaView.h"
#include "QuesaRenderer.h"

// Disable QD3D header
#ifdef __QD3DIO__
#error
#endif

#define __QD3DIO__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint on

#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
 *	@enum	TQ3FileModeMasks
 *	@discussion
 *		Mode masks for file operations.
 *	@constant	kQ3FileModeNormal		Normal file mode.
 *	@constant	kQ3FileModeStream		Stream mode.
 *	@constant	kQ3FileModeDatabase		Database mode.
 *	@constant	kQ3FileModeText			Text mode.
 */
typedef enum TQ3FileModeMasks {
    kQ3FileModeNormal                           = 0,
    kQ3FileModeStream                           = (1 << 0),
    kQ3FileModeDatabase                         = (1 << 1),
    kQ3FileModeText                             = (1 << 2)
} TQ3FileModeMasks;


// Read states
typedef enum TQ3FileReadGroupStateMasks {
    kQ3FileReadWholeGroup                       = 0,
    kQ3FileReadObjectsInGroup                   = (1 << 0),
    kQ3FileCurrentlyInsideGroup                 = (1 << 1)
} TQ3FileReadGroupStateMasks;


// File format types
enum {
    kQ3ObjectTypeFileFormat                                 = Q3_OBJECT_TYPE('F', 'F', 'm', 't'),
        kQ3FileFormatTypeReader                             = Q3_OBJECT_TYPE('F', 'm', 't', 'R'),
            kQ3FFormatReaderType3DMFBin                     = Q3_OBJECT_TYPE('F', 'r', 'b', 'i'),
            kQ3FFormatReaderType3DMFBinSwapped              = Q3_OBJECT_TYPE('F', 'r', 'b', 's'),
            kQ3FFormatReaderType3DMFText                    = Q3_OBJECT_TYPE('F', 'r', 't', 'x'),

        kQ3FileFormatTypeWriter                             = Q3_OBJECT_TYPE('F', 'm', 't', 'W'),
            kQ3FFormatWriterType3DMFStreamBin               = Q3_OBJECT_TYPE('F', 'w', 's', 'b'),
            kQ3FFormatWriterType3DMFStreamText              = Q3_OBJECT_TYPE('F', 'w', 's', 't'),
            kQ3FFormatWriterType3DMFNormalBin               = Q3_OBJECT_TYPE('F', 'w', 'n', 'b'),
            kQ3FFormatWriterType3DMFNormalText              = Q3_OBJECT_TYPE('F', 'w', 'n', 't'),
            kQ3FFormatWriterType3DMFDatabaseBin             = Q3_OBJECT_TYPE('F', 'w', 'd', 'b'),
            kQ3FFormatWriterType3DMFDatabaseText            = Q3_OBJECT_TYPE('F', 'w', 'd', 't'),
            kQ3FFormatWriterType3DMFDatabaseStreamBin       = Q3_OBJECT_TYPE('F', 'd', 's', 'b'),
            kQ3FFormatWriterType3DMFDatabaseStreamText      = Q3_OBJECT_TYPE('F', 'd', 's', 't')
};


// File format methods
enum {
    // Common
    kQ3XMethodTypeFFormatClose                  = Q3_METHOD_TYPE('F', 'c', 'l', 's'),
    kQ3XMethodTypeFFormatGetFormatType          = Q3_METHOD_TYPE('F', 'g', 'f', 't'),

    // Read
    kQ3XMethodTypeFFormatCanRead                = Q3_METHOD_TYPE('F', 'i', 'l', 'F'),
    kQ3XMethodTypeFFormatReadHeader             = Q3_METHOD_TYPE('F', 'r', 'h', 'd'),
    kQ3XMethodTypeFFormatReadObject             = Q3_METHOD_TYPE('F', 'r', 'o', 'b'),
    kQ3XMethodTypeFFormatSkipObject             = Q3_METHOD_TYPE('F', 's', 'o', 'b'),
    kQ3XMethodTypeFFormatGetNextType            = Q3_METHOD_TYPE('F', 'g', 'n', 't'),

    // Used for Q3XXX_ReadMethods, no strict need to override to implement a new format
    kQ3XMethodTypeFFormatFloat32Read            = Q3_METHOD_TYPE('F', 'f', '3', 'r'),
    kQ3XMethodTypeFFormatFloat32ReadArray       = Q3_METHOD_TYPE('F', 'f', '3', 'A'),
    kQ3XMethodTypeFFormatFloat64Read            = Q3_METHOD_TYPE('F', 'f', '6', 'r'),
    kQ3XMethodTypeFFormatInt8Read               = Q3_METHOD_TYPE('F', 'i', '8', 'r'),
    kQ3XMethodTypeFFormatInt8ReadArray     		= Q3_METHOD_TYPE('F', 'i', '8', 'A'),
    kQ3XMethodTypeFFormatInt16Read              = Q3_METHOD_TYPE('F', 'i', '1', 'r'),
    kQ3XMethodTypeFFormatInt16ReadArray     	= Q3_METHOD_TYPE('F', 'i', '1', 'A'),
    kQ3XMethodTypeFFormatInt32Read              = Q3_METHOD_TYPE('F', 'i', '3', 'r'),
    kQ3XMethodTypeFFormatInt32ReadArray     	= Q3_METHOD_TYPE('F', 'i', '3', 'A'),
    kQ3XMethodTypeFFormatInt64Read              = Q3_METHOD_TYPE('F', 'i', '6', 'r'),
    kQ3XMethodTypeFFormatStringRead             = Q3_METHOD_TYPE('F', 's', 't', 'r'),
    kQ3XMethodTypeFFormatRawRead                = Q3_METHOD_TYPE('F', 'r', 'w', 'r'),

    // Write
    kQ3XMethodTypeFFormatSubmitGroup            = Q3_METHOD_TYPE('F', 'w', 'g', 'r'),
    kQ3XMethodTypeFFormatSubmitObject           = Q3_METHOD_TYPE('F', 'w', 'o', 'b'),

    // Used for Q3XXX_WriteMethods, no strict need to override to implement a new format
    kQ3XMethodTypeFFormatFloat32Write           = Q3_METHOD_TYPE('F', 'f', '3', 'w'),
    kQ3XMethodTypeFFormatFloat64Write           = Q3_METHOD_TYPE('F', 'f', '6', 'w'),
    kQ3XMethodTypeFFormatInt8Write              = Q3_METHOD_TYPE('F', 'i', '8', 'w'),
    kQ3XMethodTypeFFormatInt16Write             = Q3_METHOD_TYPE('F', 'i', '1', 'w'),
    kQ3XMethodTypeFFormatInt32Write             = Q3_METHOD_TYPE('F', 'i', '3', 'w'),
    kQ3XMethodTypeFFormatInt64Write             = Q3_METHOD_TYPE('F', 'i', '6', 'w'),
    kQ3XMethodTypeFFormatStringWrite            = Q3_METHOD_TYPE('F', 's', 't', 'w'),
    kQ3XMethodTypeFFormatRawWrite               = Q3_METHOD_TYPE('F', 'r', 'w', 'w')
};


// Object 3DMF I/O methods
enum {
    // Version
    kQ3XMethodTypeObjectFileVersion             = Q3_METHOD_TYPE('v', 'e', 'r', 's'),

    // Write
    kQ3XMethodTypeObjectTraverse                = Q3_METHOD_TYPE('t', 'r', 'v', 's'),
    kQ3XMethodTypeObjectTraverseData            = Q3_METHOD_TYPE('t', 'r', 'v', 'd'),
    kQ3XMethodTypeObjectWrite                   = Q3_METHOD_TYPE('w', 'r', 'i', 't'),

    // Read
    kQ3XMethodTypeObjectReadData                = Q3_METHOD_TYPE('r', 'd', 'd', 't'),
    kQ3XMethodTypeObjectRead                    = Q3_METHOD_TYPE('r', 'e', 'a', 'd'),
    kQ3XMethodTypeObjectAttach                  = Q3_METHOD_TYPE('a', 't', 't', 'c')
};





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// File types
typedef TQ3Uns32                                TQ3FileMode;
typedef TQ3Uns32                                TQ3FileVersion;
typedef TQ3Uns32                                TQ3FileReadGroupState;
typedef TQ3Object                               TQ3FileFormatObject;


// Method types - 3DMF write
/*!
 *	@typedef	TQ3XObjectTraverseMethod
 *	@abstract	Traversal method (<code>kQ3XMethodTypeObjectTraverse</code>) for a
 *				custom element or attribute.
 *	@discussion
 *		Writing of 3DMF data is done in two phases:  traversal, which figures out which
 *		objects are to be written and how much space each will use, and the actual
 *		writing.  Two phases are necessary because the start of a container includes
 *		the total length of the container, but this cannot be known without asking
 *		each member of the container.
 *
 *		Typically, this method will call <code>Q3XView_SubmitWriteData</code> to
 *		specify the immediate object data, and then call <code>Q3Object_Submit</code>
 *		for each child object.
 *
 *	@param		object		The object to traverse.
 *	@param		data		The object's instance data.  In the case of a custom element,
 *							this is the internal form of the element data.
 *	@param		view		A view object.
 *	result		Success or failure of the operation.
*/
typedef CALLBACK_API_C(TQ3Status,               TQ3XObjectTraverseMethod)       (TQ3Object object, void *data, TQ3ViewObject view);

/*!
 *	@typedef	TQ3XObjectTraverseDataMethod
 *	@discussion
 *		This is the custom object method of type <code>kQ3XMethodTypeObjectTraverseData</code>.
 *		The QuickDraw 3D documention mentions this method, but does not explain how it
 *		differs from <code>TQ3XObjectTraverseMethod</code>.  Quesa does not use it.
 */
typedef CALLBACK_API_C(TQ3Status,               TQ3XObjectTraverseDataMethod)   (TQ3Object object, void *data, TQ3ViewObject view);

/*!
 *	@typedef	TQ3XObjectWriteMethod
 *	@abstract	Object method of type <code>kQ3XMethodTypeObjectWrite</code> for a
 *				custom element or attribute.
 *	@discussion
 *		Writing of 3DMF data is done in two phases:  traversal, which figures out which
 *		objects are to be written and how much space each will use, and the actual
 *		writing.  Two phases are necessary because the start of a container includes
 *		the total length of the container, but this cannot be known without asking
 *		each member of the container.
 *
 *		This method will write the object's instance data using functions such as
 *		<code>Q3Float32_Write</code>.  An class with no instance data other than
 *		child objects does not need to provide this method.
 *
 *	@param		data		Instance data to write.  This will be the data passed to
 *							<code>Q3XView_SubmitWriteData</code> by the traversal method.
 *	@param		theFile		A file object.
 *	@result		Success or failure of the operation.
 */
typedef CALLBACK_API_C(TQ3Status,               TQ3XObjectWriteMethod)          (const void *data, TQ3FileObject theFile);
typedef CALLBACK_API_C(void,                    TQ3XDataDeleteMethod)           (void *data);


// Method types - 3DMF read
typedef CALLBACK_API_C(TQ3Object,               TQ3XObjectReadMethod)           (TQ3FileObject theFile);

/*!
 *	@typedef	TQ3XObjectReadDataMethod
 *	@abstract	Object method of type <code>kQ3XMethodTypeObjectReadData</code> for a
 *				custom element or attribute.
 *	@discussion
 *		The method normally reads the element's data using functions such as
 *		<code>Q3Float32_Read</code> and <code>Q3File_ReadObject</code> (for child objects),
 *		then adds the element data to its parent set.
 *	@param		parentObject		Set object to receive the new element or attribute.
 *	@param		theFile				A file object.	
 *	@result		Success or failure of the operation.
 */
typedef CALLBACK_API_C(TQ3Status,               TQ3XObjectReadDataMethod)       (TQ3Object parentObject, TQ3FileObject theFile);
typedef CALLBACK_API_C(TQ3Status,               TQ3XObjectAttachMethod)         (TQ3Object childObject, TQ3Object parentObject);


// Method types - file format common
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatCloseMethod)         (TQ3FileFormatObject format, TQ3Boolean abort);
typedef CALLBACK_API_C(TQ3FileMode,             TQ3XFFormatGetFormatTypeMethod) (TQ3FileObject theFile);


// Method types - file format read



/*!
 *	@typedef	TQ3XFFormatCanReadMethod
 *	@abstract	Returns whether the called FileFormat is able to read the given storage.
 *	@discussion
 *				This method is called for every subclass of kQ3FileFormatTypeReader from Q3File_OpenRead.
 *              Once found a FileFormat that returns something other than kQ3ObjectTypeInvalid in the theFileFormatFound
 *				parameter the search stops.
 *
 *	@param		storage					Storage Object - positioned at zero - that contains the model to read.
 *	@param		theFileFormatFound		returns the signature of the FileFormat able to read this file 
 *										or kQ3ObjectTypeInvalid.	
 *	@result		Success or failure of the operation.
 */
typedef CALLBACK_API_C(TQ3Boolean,              TQ3XFFormatCanReadMethod)       (TQ3StorageObject storage, TQ3ObjectType* theFileFormatFound);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatReadHeaderMethod)    (TQ3FileObject theFile);
typedef CALLBACK_API_C(TQ3Object,               TQ3XFFormatReadObjectMethod)    (TQ3FileObject theFile);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatSkipObjectMethod)    (TQ3FileObject theFile);
typedef CALLBACK_API_C(TQ3ObjectType,           TQ3XFFormatGetNextTypeMethod)   (TQ3FileObject theFile);


// Method types - file format write

typedef CALLBACK_API_C(TQ3Status,           TQ3XFileFormatSubmitObjectMethod)(
                            TQ3ViewObject       theView,
                            void                *fileFormatPrivate,
                            TQ3Object			theObject,
							TQ3ObjectType		objectType,
							const void			*objectData);

// Method types - used for Q3XXX_ReadMethods (not required when implementing a new format)
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatFloat32ReadMethod)   (TQ3FileFormatObject format, TQ3Float32* data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatFloat32ReadArrayMethod)   (TQ3FileFormatObject format, TQ3Uns32 numFloats, TQ3Float32* data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatFloat64ReadMethod)   (TQ3FileFormatObject format, TQ3Float64* data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatInt8ReadMethod)      (TQ3FileFormatObject format, TQ3Int8* data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatInt8ReadArrayMethod) (TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int8* data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatInt16ReadMethod)     (TQ3FileFormatObject format, TQ3Int16* data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatInt16ReadArrayMethod) (TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int16* data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatInt32ReadMethod)     (TQ3FileFormatObject format, TQ3Int32* data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatInt32ReadArrayMethod) (TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int32* data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatInt64ReadMethod)     (TQ3FileFormatObject format, TQ3Int64* data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatStringReadMethod)    (TQ3FileFormatObject format, char* data, TQ3Uns32 *length);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatRawReadMethod)       (TQ3FileFormatObject format, unsigned char* data, TQ3Uns32 length);


// Method types - used for Q3XXX_WriteMethods
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatFloat32WriteMethod)  (TQ3FileFormatObject format, const TQ3Float32 *data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatFloat64WriteMethod)  (TQ3FileFormatObject format, const TQ3Float64 *data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatInt8WriteMethod)     (TQ3FileFormatObject format, const TQ3Int8 *data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatInt16WriteMethod)    (TQ3FileFormatObject format, const TQ3Int16 *data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatInt32WriteMethod)    (TQ3FileFormatObject format, const TQ3Int32 *data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatInt64WriteMethod)    (TQ3FileFormatObject format, const TQ3Int64 *data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatStringWriteMethod)   (TQ3FileFormatObject format, const char* data);
typedef CALLBACK_API_C(TQ3Status,               TQ3XFFormatRawWriteMethod)      (TQ3FileFormatObject format, const unsigned char* data, TQ3Uns32 length);


// Method types - misc
typedef CALLBACK_API_C(TQ3Status,               TQ3FileIdleMethod)              (TQ3FileObject theFile, const void *idlerData);


// FileFormat common data (must be first field in struct)           
typedef struct TQ3FFormatBaseData {
    // Initialised by Quesa
    TQ3Uns32                                    baseDataVersion;
    TQ3StorageObject                            storage;
    TQ3Uns32                                    currentStoragePosition;
    TQ3Uns32                                    logicalEOF;


    // Initialised by the importer
    TQ3FileVersion                              fileVersion;
    TQ3Boolean                                  noMoreObjects;
    TQ3Endian                                   byteOrder;
    TQ3Boolean                                  readInGroup;
    TQ3Int32                                    groupDeepCounter;
    TQ3Uns32                                    *reserved1;
    TQ3Uns32                                    *reserved2;
    TQ3Uns32                                    *reserved3;
} TQ3FFormatBaseData;


// Unknown text
typedef struct TQ3UnknownTextData {
    char                                        *objectName;
    char                                        *contents;
}TQ3UnknownTextData;


// Unknown text
typedef struct TQ3UnknownBinaryData {
    TQ3ObjectType                               objectType;
    TQ3Size                                     size;
    TQ3Endian                                   byteOrder;
    char                                        *contents;
} TQ3UnknownBinaryData;





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Version
#define Q3FileVersion(_major, _minor)                   \
        (TQ3FileVersion) ((((TQ3Uns32) _major & 0xFFFF) << 16) | ((TQ3Uns32) _major & 0xFFFF))

#define kQ3FileVersionCurrent                           Q3FileVersion(1, 6)





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3XView_SubmitWriteData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param size             Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param deleteData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XView_SubmitWriteData (
    TQ3ViewObject                 view,
    TQ3Size                       size,
    void                          *data,
    TQ3XDataDeleteMethod          deleteData
);



/*!
 *  @function
 *      Q3XView_SubmitSubObjectData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param objectClass      Description of the parameter.
 *  @param size             Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param deleteData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XView_SubmitSubObjectData (
    TQ3ViewObject                 view,
    TQ3XObjectClass               objectClass,
    TQ3Uns32                      size,
    void                          *data,
    TQ3XDataDeleteMethod          deleteData
);



/*!
 *  @function
 *      Q3File_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3FileObject  )
Q3File_New (
    void
);



/*!
 *  @function
 *      Q3File_GetStorage
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @param storage          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_GetStorage (
    TQ3FileObject                 theFile,
    TQ3StorageObject              *storage
);



/*!
 *  @function
 *      Q3File_SetStorage
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @param storage          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_SetStorage (
    TQ3FileObject                 theFile,
    TQ3StorageObject              storage
);



/*!
 *  @function
 *      Q3File_OpenRead
 *  @discussion
 *      Open a file object for reading.
 *
 *		Associate a FileFormat object with the File
 *
 *      The file object must already have a storage object associated
 *      with it, and must not already be open.
 *
 *  @param theFile          The file object.
 *  @param mode             Receives a combination of TQ3FileModeMasks values if the file is a 3DMF file,
 *							otherway it receives the choosed file format signature.
 *                          Pass NULL if you don't need this information.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_OpenRead (
    TQ3FileObject                 theFile,
    TQ3FileMode                   *mode
);



/*!
 *  @function
 *      Q3File_OpenWrite
 *  @discussion
 *      Open a file object for writing.
 *
 *      The file object must have a storage object associated with it,
 *      and must not already be open.
 *
 *		Currently, Quesa ignores the mode parameter and writes in binary stream mode.
 *
 *  @param theFile          The file object.
 *  @param mode             A combination of TQ3FileModeMasks values.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_OpenWrite (
    TQ3FileObject                 theFile,
    TQ3FileMode                   mode
);



/*!
 *  @function
 *      Q3File_IsOpen
 *  @discussion
 *      Determines whether a file object is open.
 *
 *  @param theFile          The file object.
 *  @param isOpen           Receives kQ3True if the file is open.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_IsOpen (
    TQ3FileObject                 theFile,
    TQ3Boolean                    *isOpen
);



/*!
 *  @function
 *      Q3File_GetMode
 *  @discussion
 *      Determine the mode of an open file object.
 *
 *  @param theFile          The file object.
 *  @param mode             Receives the current mode mask.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_GetMode (
    TQ3FileObject                 theFile,
    TQ3FileMode                   *mode
);



/*!
 *  @function
 *      Q3File_GetVersion
 *  @discussion
 *      Get the file format version of an open file.
 *
 *  @param theFile          The file object.
 *  @param version          Receives the file format version.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_GetVersion (
    TQ3FileObject                 theFile,
    TQ3FileVersion                *version
);



/*!
 *  @function
 *      Q3File_Close
 *  @discussion
 *      Close a file object.
 *
 *      When a file object is deleted (i.e., its last reference is disposed),
 *      it is automatically closed, therefore you may not need to call
 *		Q3File_Close.
 *
 *  @param theFile          The file object.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_Close (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_Cancel
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_Cancel (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3View_StartWriting
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3View_StartWriting (
    TQ3ViewObject                 view,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3View_EndWriting
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ViewStatus  )
Q3View_EndWriting (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3File_GetNextObjectType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3File_GetNextObjectType (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_IsNextObjectOfType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @param ofType           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Boolean  )
Q3File_IsNextObjectOfType (
    TQ3FileObject                 theFile,
    TQ3ObjectType                 ofType
);



/*!
 *  @function
 *      Q3File_ReadObject
 *  @discussion
 *      Read an object from a 3DMF file.
 *
 *      The file must already be open for reading.
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          The file object.
 *  @result                 The new object, or NULL on failure.
 */
EXTERN_API_C ( TQ3Object  )
Q3File_ReadObject (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_SkipObject
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_SkipObject (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_IsEndOfData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Boolean  )
Q3File_IsEndOfData (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_IsEndOfContainer
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @param rootObject       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Boolean  )
Q3File_IsEndOfContainer (
    TQ3FileObject                 theFile,
    TQ3Object                     rootObject
);



/*!
 *  @function
 *      Q3File_IsEndOfFile
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Boolean  )
Q3File_IsEndOfFile (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_MarkAsExternalReference
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @param sharedObject     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_MarkAsExternalReference (
    TQ3FileObject                 theFile,
    TQ3SharedObject               sharedObject
);



/*!
 *  @function
 *      Q3File_GetExternalReferences
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3File_GetExternalReferences (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Shared_ClearEditTracking
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param sharedObject     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Shared_ClearEditTracking (
    TQ3SharedObject               sharedObject
);



/*!
 *  @function
 *      Q3Shared_GetEditTrackingState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param sharedObject     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Shared_GetEditTrackingState (
    TQ3SharedObject               sharedObject
);



/*!
 *  @function
 *      Q3File_SetReadInGroup
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @param readGroupState   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_SetReadInGroup (
    TQ3FileObject                 theFile,
    TQ3FileReadGroupState         readGroupState
);



/*!
 *  @function
 *      Q3File_GetReadInGroup
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @param readGroupState   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_GetReadInGroup (
    TQ3FileObject                 theFile,
    TQ3FileReadGroupState         *readGroupState
);



/*!
 *  @function
 *      Q3File_SetIdleMethod
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @param idle             Description of the parameter.
 *  @param idleData         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3File_SetIdleMethod (
    TQ3FileObject                 theFile,
    TQ3FileIdleMethod             idle,
    const void                    *idleData
);



/*!
 *  @function
 *      Q3NewLine_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3NewLine_Write (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns8_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns8_Read (
    TQ3Uns8                       *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns8_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns8_Write (
    TQ3Uns8                       data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns16_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns16_Read (
    TQ3Uns16                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns16_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns16_Write (
    TQ3Uns16                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns32_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns32_Read (
    TQ3Uns32                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns32_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns32_Write (
    TQ3Uns32                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int8_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Int8_Read (
    TQ3Int8                       *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int8_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Int8_Write (
    TQ3Int8                       data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int16_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Int16_Read (
    TQ3Int16                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int16_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Int16_Write (
    TQ3Int16                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int32_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Int32_Read (
    TQ3Int32                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int32_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Int32_Write (
    TQ3Int32                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns64_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns64_Read (
    TQ3Uns64                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns64_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Uns64_Write (
    TQ3Uns64                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int64_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Int64_Read (
    TQ3Int64                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int64_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Int64_Write (
    TQ3Int64                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Float32_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Float32_Read (
    TQ3Float32                    *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Float32_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Float32_Write (
    TQ3Float32                    data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Float64_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Float64_Read (
    TQ3Float64                    *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Float64_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Float64_Write (
    TQ3Float64                    data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Size_Pad
 *  @discussion
 *      Pad a number of bytes to be long-word aligned.
 *
 *      In other words, this rounds a number up to a multiple of 4.
 *
 *  @param size             An unsigned integer.
 *  @result                 Next multiple of 4.
 */
EXTERN_API_C ( TQ3Size  )
Q3Size_Pad (
    TQ3Size                       size
);



/*!
 *  @function
 *      Q3String_Read
 *  @discussion
 *      Read a C string from a file.
 *
 *		Note that the length parameter is an output-only parameter.  That is,
 *		this function does not know the size of your buffer, it simply
 *		assumes that you have allocated <code>kQ3StringMaximumLength</code> bytes.
 *
 *      If you pass NULL for the buffer, <code>Q3String_Read</code> will still
 *		find the length of the string, but will leave the file object's position
 *		at the beginning of the string.
 *
 *		If it reads a string of length at least <code>kQ3StringMaximumLength</code>,
 *		and you have passed a non-NULL buffer, then Quesa will post the warning
 *		<code>kQ3WarningStringExceedsMaximumLength</code>.
 *
 *  @param data             Buffer to receive string data, of size
 *							<code>kQ3StringMaximumLength</code>, or NULL.
 *  @param length           Receives length in bytes of string data that was read,
 *							not including the terminating NUL character.
 *  @param theFile          A file object.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3String_Read (
    char                          *data,
    TQ3Uns32                      *length,
    TQ3FileObject                 theFile
);



/*!
 *	@function
 *		Q3String_ReadUnlimited
 *	@discussion
 *		Read a NUL-terminated string (C string) from a file.
 *
 *		Unlike the QD3D function <code>Q3String_Read</code>, this function is not
 *      limited to reading at most <code>kQ3StringMaximumLength</code> bytes. Note that
 *		the length parameter is an input-output parameter.
 *
 *      If you pass NULL for the buffer, <code>Q3String_ReadUnlimited</code> will still
 *		find the length of the string, but will leave the file object's position
 *		at the beginning of the string.  Therefore, you can read a string of
 *		unknown size as follows:
 *      
 *		<blockquote><pre><code>
 *		Q3String_Read( NULL, &stringSize, theFile );
 *		stringSize += 1;	// make room for terminal NUL byte
 *		buffer = Q3Memory_Allocate( stringSize );
 *		Q3String_Read( buffer, &stringSize, theFile );
 *		</code></pre></blockquote>
 *
 *		If you passed a non-NULL buffer that was not big enough for the whole string,
 *		Quesa posts <code>kQ3WarningStringExceedsMaximumLength</code>.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *	@param	data			Buffer to receive string data, or NULL.
 *	@param	ioLength		Pass the size of your buffer.  Receives the number of
 *							bytes of string data found in the file, not including
 *							the terminal NUL byte, even if it did not all fit in the
 *							buffer.
 *	@param	theFile         A file object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS
EXTERN_API_C ( TQ3Status  )
Q3String_ReadUnlimited(
    char                          *data,
    TQ3Uns32                      *ioLength,
    TQ3FileObject                 theFile
);
#endif



/*!
 *  @function
 *      Q3String_Write
 *  @discussion
 *      Write a NUL-terminated string (C string) to a file.
 *
 *      This writes a NUL-terminated string to a file, and if necessary writes
 *      up to 3 more pad bytes.  If the string's length, including the terminal
 *		NUL byte, exceeds <code>kQ3StringMaximumLength</code>, then the error
 *		<code>kQ3ErrorStringExceedsMaximumLength</code> will be posted and
 *		<code>kQ3StringMaximumLength</code> bytes will be written.
 *
 *  @param data             A NUL-terminated string.
 *  @param theFile          A file object.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3String_Write (
    const char                    *data,
    TQ3FileObject                 theFile
);



/*!
 *	@function
 *		Q3String_WriteUnlimited
 *	@discussion
 *		Write a NUL-terminated string (C string) to a file.  If necessary it writes
 *		up to 3 more pad bytes to make the total number of bytes a multiple of 4.
 *
 *		<em>This function is not available in QuickDraw 3D</em>.
 *
 *  @param data             A NUL-terminated string.
 *  @param theFile          A file object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS
EXTERN_API_C ( TQ3Status  )
Q3String_WriteUnlimited (
    const char                    *data,
    TQ3FileObject                 theFile
);
#endif



/*!
 *  @function
 *      Q3RawData_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param size             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RawData_Read (
    unsigned char                 *data,
    TQ3Uns32                      size,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3RawData_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param size             Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RawData_Write (
    const unsigned char           *data,
    TQ3Uns32                      size,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Point2D_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point2D          Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point2D_Read (
    TQ3Point2D                    *point2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Point2D_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point2D          Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point2D_Write (
    const TQ3Point2D              *point2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Point3D_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point3D          Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point3D_Read (
    TQ3Point3D                    *point3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Point3D_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point3D          Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Point3D_Write (
    const TQ3Point3D              *point3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3RationalPoint3D_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point3D          Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RationalPoint3D_Read (
    TQ3RationalPoint3D            *point3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3RationalPoint3D_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point3D          Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RationalPoint3D_Write (
    const TQ3RationalPoint3D      *point3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3RationalPoint4D_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point4D          Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RationalPoint4D_Read (
    TQ3RationalPoint4D            *point4D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3RationalPoint4D_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param point4D          Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RationalPoint4D_Write (
    const TQ3RationalPoint4D      *point4D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Vector2D_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector2D         Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Vector2D_Read (
    TQ3Vector2D                   *vector2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Vector2D_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector2D         Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Vector2D_Write (
    const TQ3Vector2D             *vector2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Vector3D_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector3D         Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Vector3D_Read (
    TQ3Vector3D                   *vector3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Vector3D_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param vector3D         Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Vector3D_Write (
    const TQ3Vector3D             *vector3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Matrix4x4_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Matrix4x4_Read (
    TQ3Matrix4x4                  *matrix4x4,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Matrix4x4_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix4x4        Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Matrix4x4_Write (
    const TQ3Matrix4x4            *matrix4x4,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Tangent2D_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param tangent2D        Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tangent2D_Read (
    TQ3Tangent2D                  *tangent2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Tangent2D_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param tangent2D        Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tangent2D_Write (
    const TQ3Tangent2D            *tangent2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Tangent3D_Read
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param tangent3D        Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tangent3D_Read (
    TQ3Tangent3D                  *tangent3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Tangent3D_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param tangent3D        Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tangent3D_Write (
    const TQ3Tangent3D            *tangent3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Comment_Write
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param comment          Description of the parameter.
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Comment_Write (
    char                          *comment,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Unknown_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param unknownObject    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Unknown_GetType (
    TQ3UnknownObject              unknownObject
);



/*!
 *  @function
 *      Q3Unknown_GetDirtyState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param unknownObject    Description of the parameter.
 *  @param isDirty          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Unknown_GetDirtyState (
    TQ3UnknownObject              unknownObject,
    TQ3Boolean                    *isDirty
);



/*!
 *  @function
 *      Q3Unknown_SetDirtyState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param unknownObject    Description of the parameter.
 *  @param isDirty          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Unknown_SetDirtyState (
    TQ3UnknownObject              unknownObject,
    TQ3Boolean                    isDirty
);



/*!
 *  @function
 *      Q3UnknownText_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param unknownObject    Description of the parameter.
 *  @param unknownTextData  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownText_GetData (
    TQ3UnknownObject              unknownObject,
    TQ3UnknownTextData            *unknownTextData
);



/*!
 *  @function
 *      Q3UnknownText_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param unknownTextData  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownText_EmptyData (
    TQ3UnknownTextData            *unknownTextData
);



/*!
 *  @function
 *      Q3UnknownBinary_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param unknownObject    Description of the parameter.
 *  @param unknownBinaryData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_GetData (
    TQ3UnknownObject              unknownObject,
    TQ3UnknownBinaryData          *unknownBinaryData
);



/*!
 *  @function
 *      Q3UnknownBinary_EmptyData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param unknownBinaryData Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_EmptyData (
    TQ3UnknownBinaryData          *unknownBinaryData
);



/*!
 *  @function
 *      Q3UnknownBinary_GetTypeString
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param unknownObject    Description of the parameter.
 *  @param typeString       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_GetTypeString (
    TQ3UnknownObject              unknownObject,
    char                          **typeString
);



/*!
 *  @function
 *      Q3UnknownBinary_EmptyTypeString
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param typeString       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_EmptyTypeString (
    char                          **typeString
);



/*!
 *  @function
 *      Q3ViewHints_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ViewHintsObject  )
Q3ViewHints_New (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3ViewHints_SetRenderer
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param renderer         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetRenderer (
    TQ3ViewHintsObject            viewHints,
    TQ3RendererObject             renderer
);



/*!
 *  @function
 *      Q3ViewHints_GetRenderer
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param renderer         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetRenderer (
    TQ3ViewHintsObject            viewHints,
    TQ3RendererObject             *renderer
);



/*!
 *  @function
 *      Q3ViewHints_SetCamera
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param camera           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetCamera (
    TQ3ViewHintsObject            viewHints,
    TQ3CameraObject               camera
);



/*!
 *  @function
 *      Q3ViewHints_GetCamera
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param camera           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetCamera (
    TQ3ViewHintsObject            viewHints,
    TQ3CameraObject               *camera
);



/*!
 *  @function
 *      Q3ViewHints_SetLightGroup
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param lightGroup       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetLightGroup (
    TQ3ViewHintsObject            viewHints,
    TQ3GroupObject                lightGroup
);



/*!
 *  @function
 *      Q3ViewHints_GetLightGroup
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param lightGroup       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetLightGroup (
    TQ3ViewHintsObject            viewHints,
    TQ3GroupObject                *lightGroup
);



/*!
 *  @function
 *      Q3ViewHints_SetAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetAttributeSet (
    TQ3ViewHintsObject            viewHints,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3ViewHints_GetAttributeSet
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param attributeSet     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetAttributeSet (
    TQ3ViewHintsObject            viewHints,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3ViewHints_SetDimensionsState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param isValid          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetDimensionsState (
    TQ3ViewHintsObject            viewHints,
    TQ3Boolean                    isValid
);



/*!
 *  @function
 *      Q3ViewHints_GetDimensionsState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param isValid          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetDimensionsState (
    TQ3ViewHintsObject            viewHints,
    TQ3Boolean                    *isValid
);



/*!
 *  @function
 *      Q3ViewHints_SetDimensions
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param width            Description of the parameter.
 *  @param height           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetDimensions (
    TQ3ViewHintsObject            viewHints,
    TQ3Uns32                      width,
    TQ3Uns32                      height
);



/*!
 *  @function
 *      Q3ViewHints_GetDimensions
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param width            Description of the parameter.
 *  @param height           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetDimensions (
    TQ3ViewHintsObject            viewHints,
    TQ3Uns32                      *width,
    TQ3Uns32                      *height
);



/*!
 *  @function
 *      Q3ViewHints_SetMaskState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param isValid          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetMaskState (
    TQ3ViewHintsObject            viewHints,
    TQ3Boolean                    isValid
);



/*!
 *  @function
 *      Q3ViewHints_GetMaskState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param isValid          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetMaskState (
    TQ3ViewHintsObject            viewHints,
    TQ3Boolean                    *isValid
);



/*!
 *  @function
 *      Q3ViewHints_SetMask
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param mask             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetMask (
    TQ3ViewHintsObject            viewHints,
    const TQ3Bitmap               *mask
);



/*!
 *  @function
 *      Q3ViewHints_GetMask
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param mask             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetMask (
    TQ3ViewHintsObject            viewHints,
    TQ3Bitmap                     *mask
);



/*!
 *  @function
 *      Q3ViewHints_SetClearImageMethod
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param clearMethod      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetClearImageMethod (
    TQ3ViewHintsObject            viewHints,
    TQ3DrawContextClearImageMethod clearMethod
);



/*!
 *  @function
 *      Q3ViewHints_GetClearImageMethod
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param clearMethod      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetClearImageMethod (
    TQ3ViewHintsObject            viewHints,
    TQ3DrawContextClearImageMethod *clearMethod
);



/*!
 *  @function
 *      Q3ViewHints_SetClearImageColor
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param color            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetClearImageColor (
    TQ3ViewHintsObject            viewHints,
    const TQ3ColorARGB            *color
);



/*!
 *  @function
 *      Q3ViewHints_GetClearImageColor
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewHints        Description of the parameter.
 *  @param color            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetClearImageColor (
    TQ3ViewHintsObject            viewHints,
    TQ3ColorARGB                  *color
);



/*!
 *  @function
 *      Q3File_GetFileFormat  
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theFile          Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3FileFormatObject  )
Q3File_GetFileFormat (
    TQ3FileObject                 theFile
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_NewFromType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param fformatObjectType Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3FileFormatObject  )
Q3FileFormat_NewFromType (
    TQ3ObjectType                 fformatObjectType
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3ObjectType  )
Q3FileFormat_GetType (
    TQ3FileFormatObject           format
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_HasModalConfigure
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Boolean  )
Q3FileFormat_HasModalConfigure (
    TQ3FileFormatObject           format
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_ModalConfigure
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param dialogAnchor     Description of the parameter.
 *  @param canceled         Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_ModalConfigure (
    TQ3FileFormatObject           format,
    TQ3DialogAnchor               dialogAnchor,
    TQ3Boolean                    *canceled
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormatClass_GetFormatNameString
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param formatClassType  Description of the parameter.
 *  @param formatClassString Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormatClass_GetFormatNameString (
    TQ3ObjectType                 formatClassType,
    TQ3ObjectClassNameString      formatClassString
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GetConfigurationData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param dataBuffer       Description of the parameter.
 *  @param bufferSize       Description of the parameter.
 *  @param actualDataSize   Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GetConfigurationData (
    TQ3FileFormatObject           format,
    unsigned char                 *dataBuffer,
    TQ3Uns32                      bufferSize,
    TQ3Uns32                      *actualDataSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_SetConfigurationData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param dataBuffer       Description of the parameter.
 *  @param bufferSize       Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_SetConfigurationData (
    TQ3RendererObject             format,
    unsigned char                 *dataBuffer,
    TQ3Uns32                      bufferSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      Binary reading utilities
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3FileFormat_GenericReadBinary_8
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_8 (
    TQ3FileFormatObject           format,
    TQ3Int8                       *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinary_16
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_16 (
    TQ3FileFormatObject           format,
    TQ3Int16                      *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinary_32
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_32 (
    TQ3FileFormatObject           format,
    TQ3Int32                      *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinary_64
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_64 (
    TQ3FileFormatObject           format,
    TQ3Int64                      *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinary_String
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param length           Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_String (
    TQ3FileFormatObject           format,
    char                          *data,
    TQ3Uns32                      *length
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinary_Raw
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param length           Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_Raw (
    TQ3FileFormatObject           format,
    unsigned char                 *data,
    TQ3Uns32                      length
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      Swapped binary reading utilities
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3FileFormat_GenericReadBinSwap_16
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinSwap_16 (
    TQ3FileFormatObject           format,
    TQ3Int16                      *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinSwap_32
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinSwap_32 (
    TQ3FileFormatObject           format,
    TQ3Int32                      *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinSwap_64
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinSwap_64 (
    TQ3FileFormatObject           format,
    TQ3Int64                      *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      Text reading utilities
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3FileFormat_GenericReadText_SkipBlanks
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadText_SkipBlanks (
    TQ3FileFormatObject           format
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadText_ReadUntilChars
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param buffer           Description of the parameter.
 *  @param chars            Description of the parameter.
 *  @param numChars         Description of the parameter.
 *  @param blanks           Description of the parameter.
 *  @param foundChar        Description of the parameter.
 *  @param maxLen           Description of the parameter.
 *  @param charsRead        Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

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

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      Binary writing utilities
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinary_8
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_8 (
    TQ3FileFormatObject           format,
    const TQ3Int8                 *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinary_16
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_16 (
    TQ3FileFormatObject           format,
    const TQ3Int16                *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinary_32
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_32 (
    TQ3FileFormatObject           format,
    const TQ3Int32                *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinary_64
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_64 (
    TQ3FileFormatObject           format,
    const TQ3Int64                *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinary_String
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param length           Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_String (
    TQ3FileFormatObject           format,
    const char                    *data,
    TQ3Uns32                      *length
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinary_Raw 
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param length           Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_Raw (
    TQ3FileFormatObject           format,
    const unsigned char           *data,
    TQ3Uns32                      length
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      Swapped binary writing utilities
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinSwap_16
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinSwap_16 (
    TQ3FileFormatObject           format,
    const TQ3Int16                *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinSwap_32
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinSwap_32 (
    TQ3FileFormatObject           format,
    const TQ3Int32                *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinSwap_64
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinSwap_64 (
    TQ3FileFormatObject           format,
    const TQ3Int64                *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS




/*!
 *  @function
 *      Q3Float32_ReadArray
 *  @discussion
 *      Read an array of 32-bit floating point numbers from a file object.
 *
 *      Calling this function has somewhat less overhead than calling
 *      Q3Float32_Read repeatedly.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param	numFloats		Number of numbers to read.
 *  @param	floatArray		Address of array to receive the numbers.
 *	@param	theFile			A file object.
 *  @result    Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3Float32_ReadArray (
	TQ3Uns32					numFloats,
	TQ3Float32*					floatArray,
	TQ3FileObject            	theFile
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS







/*!
 *  @function
 *      Q3Uns32_ReadArray
 *  @discussion
 *      Read an array of 32-bit unsigned integers from a file object.
 *
 *      Calling this function has somewhat less overhead than calling
 *      Q3Uns32_Read repeatedly.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param	numNums			Number of numbers to read.
 *  @param	intArray		Address of array to receive the numbers.
 *	@param	theFile			A file object.
 *  @result    Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3Uns32_ReadArray (
	TQ3Uns32					numNums,
	TQ3Uns32*					intArray,
	TQ3FileObject            	theFile
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS






/*!
 *  @function
 *      Q3Uns16_ReadArray
 *  @discussion
 *      Read an array of 16-bit unsigned integers from a file object.
 *
 *      Calling this function has somewhat less overhead than calling
 *      Q3Uns16_Read repeatedly.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param	numNums			Number of numbers to read.
 *  @param	intArray		Address of array to receive the numbers.
 *	@param	theFile			A file object.
 *  @result    Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3Uns16_ReadArray (
	TQ3Uns32					numNums,
	TQ3Uns16*					intArray,
	TQ3FileObject            	theFile
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS






/*!
 *  @function
 *      Q3Uns8_ReadArray
 *  @discussion
 *      Read an array of 8-bit unsigned integers from a file object.
 *
 *      Calling this function has somewhat less overhead than calling
 *      Q3Uns8_Read repeatedly.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param	numNums			Number of numbers to read.
 *  @param	intArray		Address of array to receive the numbers.
 *	@param	theFile			A file object.
 *  @result    Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Status  )
Q3Uns8_ReadArray (
	TQ3Uns32					numNums,
	TQ3Uns8*					intArray,
	TQ3FileObject            	theFile
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS






//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif



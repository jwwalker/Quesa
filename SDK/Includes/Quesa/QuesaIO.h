/*! @header QuesaIO.h
        Declares the Quesa IO objects.
 */
/*  NAME:
        QuesaIO.h

    DESCRIPTION:
        Quesa public header.

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
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
 *  @enum
 *      TQ3FileModeMasks
 *  @discussion
 *      Mode masks for file operations.
 *
 *  @constant kQ3FileModeNormal      Normal file mode.
 *  @constant kQ3FileModeStream      Stream mode.
 *  @constant kQ3FileModeDatabase    Database mode.
 *  @constant kQ3FileModeText        Text mode.
 *  @constant kQ3FileModeSwap        Binary mode with byte order swapped (not present in QD3D).
 */
typedef enum TQ3FileModeMasks {
    kQ3FileModeNormal                           = 0,
    kQ3FileModeStream                           = (1 << 0),
    kQ3FileModeDatabase                         = (1 << 1),
    kQ3FileModeText                             = (1 << 2),
#if QUESA_ALLOW_QD3D_EXTENSIONS
    kQ3FileModeSwap                             = (1 << 3),
#endif
    kQ3FileModeSize32                           = 0xFFFFFFFF
} TQ3FileModeMasks;


/*!
 *  @enum
 *      TQ3FileReadGroupStateMasks
 *  @discussion
 *      Read group state masks.
 *
 *  @constant kQ3FileReadWholeGroup          Read the whole group.
 *  @constant kQ3FileReadObjectsInGroup      Read objects in the group.
 *  @constant kQ3FileCurrentlyInsideGroup    Currently inside a group.
 */
typedef enum TQ3FileReadGroupStateMasks {
    kQ3FileReadWholeGroup                       = 0,
    kQ3FileReadObjectsInGroup                   = (1 << 0),
    kQ3FileCurrentlyInsideGroup                 = (1 << 1),
    kQ3FileReadGroupStateSize32                 = 0xFFFFFFFF
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
            kQ3FFormatWriterType3DMFStreamBinSwap           = Q3_OBJECT_TYPE('F', 'w', 's', 'w'),
            kQ3FFormatWriterType3DMFStreamText              = Q3_OBJECT_TYPE('F', 'w', 's', 't'),
            kQ3FFormatWriterType3DMFNormalBin               = Q3_OBJECT_TYPE('F', 'w', 'n', 'b'),
            kQ3FFormatWriterType3DMFNormalBinSwap           = Q3_OBJECT_TYPE('F', 'w', 'n', 'w'),
            kQ3FFormatWriterType3DMFNormalText              = Q3_OBJECT_TYPE('F', 'w', 'n', 't'),
            kQ3FFormatWriterType3DMFDatabaseBin             = Q3_OBJECT_TYPE('F', 'w', 'd', 'b'),
            kQ3FFormatWriterType3DMFDatabaseBinSwap         = Q3_OBJECT_TYPE('F', 'w', 'd', 'w'),
            kQ3FFormatWriterType3DMFDatabaseText            = Q3_OBJECT_TYPE('F', 'w', 'd', 't'),
            kQ3FFormatWriterType3DMFDatabaseStreamBin       = Q3_OBJECT_TYPE('F', 'd', 's', 'b'),
            kQ3FFormatWriterType3DMFDatabaseStreamBinSwap   = Q3_OBJECT_TYPE('F', 'd', 's', 'w'),
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
 #if QUESA_ALLOW_QD3D_EXTENSIONS
    kQ3XMethodTypeObjectReadDefault             = Q3_METHOD_TYPE('r', 'd', 'd', 'f'),
 #endif
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
typedef Q3_CALLBACK_API_C(TQ3Status,               TQ3XObjectTraverseMethod)       (TQ3Object object, void *data, TQ3ViewObject view);


/*!
 *	@typedef	TQ3XObjectTraverseDataMethod
 *	@discussion
 *		This is the custom object method of type <code>kQ3XMethodTypeObjectTraverseData</code>.
 *		The QuickDraw 3D documention mentions this method, but does not explain how it
 *		differs from <code>TQ3XObjectTraverseMethod</code>.  Quesa does not use it.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,               TQ3XObjectTraverseDataMethod)   (TQ3Object object, void *data, TQ3ViewObject view);


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
typedef Q3_CALLBACK_API_C(TQ3Status,               TQ3XObjectWriteMethod)          (const void *data, TQ3FileObject theFile);


/*!
 *  @typedef
 *      TQ3XDataDeleteMethod
 *  @discussion
 *      Delete method.
 *
 *  @param delete           The data to delete.
 */
typedef Q3_CALLBACK_API_C(void,                    TQ3XDataDeleteMethod)           (void *data);


/*!
 *  @typedef
 *      TQ3XObjectReadMethod
 *  @discussion
 *      Read method.
 *
 *  @param theFile          The file to read.
 *	@result                 The object(s) read from the file.
 */
typedef Q3_CALLBACK_API_C(TQ3Object,               TQ3XObjectReadMethod)           (TQ3FileObject theFile);


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
typedef Q3_CALLBACK_API_C(TQ3Status,               TQ3XObjectReadDataMethod)       (TQ3Object parentObject, TQ3FileObject theFile);


#if QUESA_ALLOW_QD3D_EXTENSIONS
/*!
 *  @typedef
 *      TQ3XObjectReadDefaultMethod
 *  @discussion
 *      Read method for default geometries (geometries represented with no data in 3DMF files).
 *
 *      <em>This method is not available in QD3D.</em>
 *
 *	@param		theFile				A file object.	
 *	@result                 The new default object.
 */
typedef Q3_CALLBACK_API_C(TQ3Object,               TQ3XObjectReadDefaultMethod)    (TQ3FileObject theFile);
#endif

/*!
 *  @typedef
 *      TQ3XObjectAttachMethod
 *  @discussion
 *      Attach method.
 *
 *  @param childObject      The child object.
 *  @param parentObject     The parent object.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,               TQ3XObjectAttachMethod)         (TQ3Object childObject, TQ3Object parentObject);


/*!
 *  @typedef
 *      TQ3XFFormatCloseMethod
 *  @discussion
 *      Close method.
 *
 *  @param format           The format to close.
 *  @param abort            Closing due to an abort.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,               TQ3XFFormatCloseMethod)         (TQ3FileFormatObject format, TQ3Boolean abort);


/*!
 *  @typedef
 *      TQ3XFFormatGetFormatTypeMethod
 *  @discussion
 *      Get the format type for a file.
 *
 *  @param theFile          The file to query.
 *  @result                 The file format.
 */
typedef Q3_CALLBACK_API_C(TQ3FileMode,             TQ3XFFormatGetFormatTypeMethod) (TQ3FileObject theFile);


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
typedef Q3_CALLBACK_API_C(TQ3Boolean,              TQ3XFFormatCanReadMethod)       (TQ3StorageObject storage, TQ3ObjectType* theFileFormatFound);


/*!
 *  @typedef
 *      TQ3XFFormatReadHeaderMethod
 *  @discussion
 *      Read the header from a file.
 *
 *  @param theFile          The file to process.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,               TQ3XFFormatReadHeaderMethod)    (TQ3FileObject theFile);


/*!
 *  @typedef
 *      TQ3XFFormatReadObjectMethod
 *  @discussion
 *      Read an object from a file.
 *
 *  @param theFile          The file to process.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Object,               TQ3XFFormatReadObjectMethod)    (TQ3FileObject theFile);


/*!
 *  @typedef
 *      TQ3XFFormatSkipObjectMethod
 *  @discussion
 *      Skip an object in a file.
 *
 *  @param theFile          The file to process.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,               TQ3XFFormatSkipObjectMethod)    (TQ3FileObject theFile);


/*!
 *  @typedef
 *      TQ3XFFormatGetNextTypeMethod
 *  @discussion
 *      Get the next type from a file.
 *
 *  @param theFile          The file to process.
 *  @result                 The next type in the file.
 */
typedef Q3_CALLBACK_API_C(TQ3ObjectType,           TQ3XFFormatGetNextTypeMethod)   (TQ3FileObject theFile);


/*!
 *  @typedef
 *      TQ3XFileFormatSubmitObjectMethod
 *  @discussion
 *      Submit an object to a file.
 *
 *  @param theView              The view being submitted to.
 *  @param fileFormatPrivate    File-format specific data.
 *  @param theObject            The object to submit.
 *  @param objectType           The type of theObject.
 *  @param objectData           The data for theObject.
 *  @result                     Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XFileFormatSubmitObjectMethod)(
                            TQ3ViewObject       theView,
                            void                *fileFormatPrivate,
                            TQ3Object			theObject,
							TQ3ObjectType		objectType,
							const void			*objectData);


/*!
 *  @typedef
 *      TQ3XFFormatFloat32ReadMethod
 *  @discussion
 *      Read a 32-bit float from a file.
 *
 *  @param format           The file format.
 *  @param data             Receives the data being read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatFloat32ReadMethod)      (TQ3FileFormatObject format, TQ3Float32* data);


/*!
 *  @typedef
 *      TQ3XFFormatFloat32ReadArrayMethod
 *  @discussion
 *      Read an array of 32-bit floats from a file.
 *
 *  @param format           The file format.
 *  @param numFloats        The number of floats to read.
 *  @param data             Receives the data being read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatFloat32ReadArrayMethod) (TQ3FileFormatObject format, TQ3Uns32 numFloats, TQ3Float32* data);


/*!
 *  @typedef
 *      TQ3XFFormatFloat64ReadMethod
 *  @discussion
 *      Read a 64-bit float from a file.
 *
 *  @param format           The file format.
 *  @param data             Receives the data being read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatFloat64ReadMethod)      (TQ3FileFormatObject format, TQ3Float64* data);


/*!
 *  @typedef
 *      TQ3XFFormatInt8ReadMethod
 *  @discussion
 *      Read an 8-bit integer from a file.
 *
 *  @param format           The file format.
 *  @param data             Receives the data being read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatInt8ReadMethod)         (TQ3FileFormatObject format, TQ3Int8* data);


/*!
 *  @typedef
 *      TQ3XFFormatInt8ReadArrayMethod
 *  @discussion
 *      Read an array of 8-bit integers from a file.
 *
 *  @param format           The file format.
 *  @param numNums          The number of numbers to read.
 *  @param data             Receives the data being read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatInt8ReadArrayMethod)    (TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int8* data);


/*!
 *  @typedef
 *      TQ3XFFormatInt16ReadMethod
 *  @discussion
 *      Read a 16-bit integer from a file.
 *
 *  @param format           The file format.
 *  @param data             Receives the data being read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatInt16ReadMethod)        (TQ3FileFormatObject format, TQ3Int16* data);


/*!
 *  @typedef
 *      TQ3XFFormatInt16ReadArrayMethod
 *  @discussion
 *      Read an array of 16-bit integers from a file.
 *
 *  @param format           The file format.
 *  @param numNums          The number of numbers to read.
 *  @param data             Receives the data being read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatInt16ReadArrayMethod)   (TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int16* data);


/*!
 *  @typedef
 *      TQ3XFFormatInt32ReadMethod
 *  @discussion
 *      Read a 32-bit integer from a file.
 *
 *  @param format           The file format.
 *  @param data             Receives the data being read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatInt32ReadMethod)        (TQ3FileFormatObject format, TQ3Int32* data);


/*!
 *  @typedef
 *      TQ3XFFormatInt32ReadArrayMethod
 *  @discussion
 *      Read an array of 32-bit integers from a file.
 *
 *  @param format           The file format.
 *  @param numNums          The number of numbers to read.
 *  @param data             Receives the data being read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatInt32ReadArrayMethod)   (TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int32* data);


/*!
 *  @typedef
 *      TQ3XFFormatInt64ReadMethod
 *  @discussion
 *      Read a 64-bit integer from a file.
 *
 *  @param format           The file format.
 *  @param data             Receives the data being read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatInt64ReadMethod)        (TQ3FileFormatObject format, TQ3Int64* data);


/*!
 *  @typedef
 *      TQ3XFFormatStringReadMethod
 *  @discussion
 *      Read a string from a file.
 *
 *  @param format           The file format.
 *  @param data             Receives the data being read.
 *  @param length           Receives the number of bytes read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatStringReadMethod)       (TQ3FileFormatObject format, char* data, TQ3Uns32 *length);


/*!
 *  @typedef
 *      TQ3XFFormatRawReadMethod
 *  @discussion
 *      Read a block of data from a file.
 *
 *  @param format           The file format.
 *  @param data             Receives the data being read.
 *  @param length           Receives the number of bytes read.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatRawReadMethod)          (TQ3FileFormatObject format, unsigned char* data, TQ3Uns32 length);


/*!
 *  @typedef
 *      TQ3XFFormatFloat32WriteMethod
 *  @discussion
 *      Write a 32-bit float to a file.
 *
 *  @param format           The file format.
 *  @param data             The data to write.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatFloat32WriteMethod)(TQ3FileFormatObject format, const TQ3Float32 *data);


/*!
 *  @typedef
 *      TQ3XFFormatFloat64WriteMethod
 *  @discussion
 *      Write a 64-bit float to a file.
 *
 *  @param format           The file format.
 *  @param data             The data to write.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatFloat64WriteMethod)(TQ3FileFormatObject format, const TQ3Float64 *data);


/*!
 *  @typedef
 *      TQ3XFFormatInt8WriteMethod
 *  @discussion
 *      Write a 8-bit integer to a file.
 *
 *  @param format           The file format.
 *  @param data             The data to write.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatInt8WriteMethod)   (TQ3FileFormatObject format, const TQ3Int8 *data);


/*!
 *  @typedef
 *      TQ3XFFormatInt16WriteMethod
 *  @discussion
 *      Write a 16-bit integer to a file.
 *
 *  @param format           The file format.
 *  @param data             The data to write.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatInt16WriteMethod)  (TQ3FileFormatObject format, const TQ3Int16 *data);


/*!
 *  @typedef
 *      TQ3XFFormatInt32WriteMethod
 *  @discussion
 *      Write a 32-bit integer to a file.
 *
 *  @param format           The file format.
 *  @param data             The data to write.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatInt32WriteMethod)  (TQ3FileFormatObject format, const TQ3Int32 *data);


/*!
 *  @typedef
 *      TQ3XFFormatInt64WriteMethod
 *  @discussion
 *      Write a 64-bit integer to a file.
 *
 *  @param format           The file format.
 *  @param data             The data to write.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatInt64WriteMethod)  (TQ3FileFormatObject format, const TQ3Int64 *data);


/*!
 *  @typedef
 *      TQ3XFFormatStringWriteMethod
 *  @discussion
 *      Write a C-string to a file.
 *
 *  @param format           The file format.
 *  @param data             The data to write.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatStringWriteMethod) (TQ3FileFormatObject format, const char* data);


/*!
 *  @typedef
 *      TQ3XFFormatRawWriteMethod
 *  @discussion
 *      Write a block of data to a file.
 *
 *  @param format           The file format.
 *  @param data             The data to write.
 *  @param length           The number of bytes to write.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3XFFormatRawWriteMethod)    (TQ3FileFormatObject format, const unsigned char* data, TQ3Uns32 length);


/*!
 *  @typedef
 *      TQ3FileIdleMethod
 *  @discussion
 *      Idle callback for reading/writing.
 *
 *  @param theFile          The file being process.
 *  @param idlerData        Application-specific data.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3FileIdleMethod) (TQ3FileObject theFile, const void *idlerData);


/*!
 *  @struct
 *      TQ3FFormatBaseData
 *  @discussion
 *      Common data for FileFormat objects.
 *
 *      This must be the first field in any derived structures.
 *
 *      The baseDataVersion, storage, currentStoragePosition, and logicalEOF fields
 *      are initialised automatically by Quesa. Remaining fields must be initialised
 *      by the importer.
 *
 *  @field baseDataVersion           The base data version.
 *  @field storage                   The storage object.
 *  @field currentStoragePosition    The current position within the storage object.
 *  @field logicalEOF                The number of bytes in the storage object.
 */
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


/*!
 *  @struct
 *      TQ3UnknownTextData
 *  @discussion
 *      Unknown text data.
 *
 *  @field objectName        The name of the object.
 *  @field contents          The data for the object.
 */
typedef struct TQ3UnknownTextData {
    char                                        *objectName;
    char                                        *contents;
} TQ3UnknownTextData;


/*!
 *  @struct
 *      TQ3UnknownBinaryData
 *  @discussion
 *      Unknown binary data.
 *
 *  @field objectType        The type of the object.
 *  @field size              The size of the object data.
 *  @field byteOrder         The byte order of the object data.
 *  @field contents          The data for the object.
 */
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
 *      Submit data to a view for writing.
 *
 *  @param view             The view being submitted to.
 *  @param size             The number of bytes pointed to by data.
 *  @param data             The data to submit for writing.
 *  @param deleteData       Delete callback to dispose of the data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Submit a sub-object to a view for writing.
 *
 *  @param view             The view being submitted to.
 *  @param objectClass      The class of the object being submitted.
 *  @param size             The number of bytes pointed to by data.
 *  @param data             The data for the object.
 *  @param deleteData       Delete callback to dispose of the data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Create a new file object.
 *
 *  @result                 The new file object.
 */
Q3_EXTERN_API_C ( TQ3FileObject  )
Q3File_New (
    void
);



/*!
 *  @function
 *      Q3File_GetStorage
 *  @discussion
 *      Get the storage object for a file.
 *
 *  @param theFile          The file to query.
 *  @param storage          Receives the storage object for the file.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3File_GetStorage (
    TQ3FileObject                 theFile,
    TQ3StorageObject              *storage
);



/*!
 *  @function
 *      Q3File_SetStorage
 *  @discussion
 *      Set the storage objectfor a file.
 *
 *  @param theFile          The file to query.
 *  @param storage          The new storage object for the file.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Status  )
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
 *  @param mode             A combination of TQ3FileModeMasks values, or the file format class signature.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Status  )
Q3File_Close (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_Cancel
 *  @discussion
 *      Cancel a read/write operation on a file.
 *
 *  @param theFile          The file to cancel the current read/write operation for.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3File_Cancel (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3View_StartWriting
 *  @discussion
 *      Start a writing loop.
 *
 *  @param view             The view to start writing to.
 *  @param theFile          The file to start writing to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3View_StartWriting (
    TQ3ViewObject                 view,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3View_EndWriting
 *  @discussion
 *      End a writing loop.
 *
 *  @param view             The view to end writing to.
 *  @result                 Success or failure of the writing loop.
 *                          Note that the result is a TQ3ViewStatus, not a TQ3Status.
 */
Q3_EXTERN_API_C ( TQ3ViewStatus  )
Q3View_EndWriting (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3File_GetNextObjectType
 *  @discussion
 *      Get the type of the next object in a file.
 *
 *  @param theFile          The file to query.
 *  @result                 The type of the next object in the file.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3File_GetNextObjectType (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_IsNextObjectOfType
 *  @discussion
 *      Test the type of the next object in a file.
 *
 *  @param theFile          The file to query.
 *  @param ofType           The type to compare against.
 *  @result                 Does the next object match the specified type?
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
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
Q3_EXTERN_API_C ( TQ3Object  )
Q3File_ReadObject (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_SkipObject
 *  @discussion
 *      Skip the next object in a file.
 *
 *  @param theFile          The file to process.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3File_SkipObject (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_IsEndOfData
 *  @discussion
 *      Has the end of the data within a file been reached?
 *
 *  @param theFile          The file to query.
 *  @result                 Has the end of the data been reached?
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3File_IsEndOfData (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_IsEndOfContainer
 *  @discussion
 *      Has the end of the current container in a file been reached?
 *
 *  @param theFile          The file to query.
 *  @param rootObject       The root object in the container.
 *  @result                 Has the end of the container been reached?
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3File_IsEndOfContainer (
    TQ3FileObject                 theFile,
    TQ3Object                     rootObject
);



/*!
 *  @function
 *      Q3File_IsEndOfFile
 *  @discussion
 *      Has the end of a file been reached?
 *
 *  @param theFile          The file to query.
 *  @result                 Has the end of the file been reached?
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3File_IsEndOfFile (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3File_MarkAsExternalReference
 *  @discussion
 *      Mark an object within a file as being an external reference.
 *
 *  @param theFile          The file to update.
 *  @param sharedObject     The object to mark as external.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3File_MarkAsExternalReference (
    TQ3FileObject                 theFile,
    TQ3SharedObject               sharedObject
);



/*!
 *  @function
 *      Q3File_GetExternalReferences
 *  @discussion
 *      Get the external references of a file.
 *
 *  @param theFile          The file to query.
 *  @result                 The external objects within the file.
 */
Q3_EXTERN_API_C ( TQ3GroupObject  )
Q3File_GetExternalReferences (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Shared_ClearEditTracking
 *  @discussion
 *      Clear the edit tracking state of a shared object.
 *
 *  @param sharedObject     The shared object to update.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shared_ClearEditTracking (
    TQ3SharedObject               sharedObject
);



/*!
 *  @function
 *      Q3Shared_GetEditTrackingState
 *  @discussion
 *      Get the edit tracking state of a shared object.
 *
 *  @param sharedObject     The shared object to update.
 *  @result                 the current edit tracking state of the object.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Shared_GetEditTrackingState (
    TQ3SharedObject               sharedObject
);



/*!
 *  @function
 *      Q3File_SetReadInGroup
 *  @discussion
 *      Set the group reading state for a file.
 *
 *  @param theFile          The file to update.
 *  @param readGroupState   The new group reading state for the file.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3File_SetReadInGroup (
    TQ3FileObject                 theFile,
    TQ3FileReadGroupState         readGroupState
);



/*!
 *  @function
 *      Q3File_GetReadInGroup
 *  @discussion
 *      Get the group reading state for a file.
 *
 *  @param theFile          The file to query.
 *  @param readGroupState   Receives the group reading state for the file.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3File_GetReadInGroup (
    TQ3FileObject                 theFile,
    TQ3FileReadGroupState         *readGroupState
);



/*!
 *  @function
 *      Q3File_SetIdleMethod
 *  @discussion
 *      Set the idle method for a file.
 *
 *  @param theFile          The file to update.
 *  @param idle             The idle callback.
 *  @param idleData         Application-specific data for the idle callback.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3File_SetIdleMethod (
    TQ3FileObject                 theFile,
    TQ3FileIdleMethod             idle,
    const void                    *idleData
);



/*!
 *  @function
 *      Q3NewLine_Write
 *  @discussion
 *      Write a newline to a file.
 *
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NewLine_Write (
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns8_Read
 *  @discussion
 *      Read a TQ3Uns8 from a file.
 *
 *  @param data             Receives the data read from the file.
 *  @param theFile          The file to read from.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Uns8_Read (
    TQ3Uns8                       *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns8_Write
 *  @discussion
 *      Write a TQ3Uns8 to a file.
 *
 *  @param data             The data to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Uns8_Write (
    TQ3Uns8                       data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns16_Read
 *  @discussion
 *      Read a TQ3Uns16 from a file.
 *
 *  @param data             Receives the data read from the file.
 *  @param theFile          The file to read from.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Uns16_Read (
    TQ3Uns16                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns16_Write
 *  @discussion
 *      Write a TQ3Uns16 to a file.
 *
 *  @param data             The data to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Uns16_Write (
    TQ3Uns16                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns32_Read
 *  @discussion
 *      Read a TQ3Uns32 from a file.
 *
 *  @param data             Receives the data read from the file.
 *  @param theFile          The file to read from.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Uns32_Read (
    TQ3Uns32                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns32_Write
 *  @discussion
 *      Write a TQ3Uns32 to a file.
 *
 *  @param data             The data to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Uns32_Write (
    TQ3Uns32                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int8_Read
 *  @discussion
 *      Read a TQ3Int8 from a file.
 *
 *  @param data             Receives the data read from the file.
 *  @param theFile          The file to read from.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Int8_Read (
    TQ3Int8                       *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int8_Write
 *  @discussion
 *      Write a TQ3Int8 to a file.
 *
 *  @param data             The data to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Int8_Write (
    TQ3Int8                       data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int16_Read
 *  @discussion
 *      Read a TQ3Int16 from a file.
 *
 *  @param data             Receives the data read from the file.
 *  @param theFile          The file to read from.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Int16_Read (
    TQ3Int16                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int16_Write
 *  @discussion
 *      Write a TQ3Int16 to a file.
 *
 *  @param data             The data to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Int16_Write (
    TQ3Int16                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int32_Read
 *  @discussion
 *      Read a TQ3Int32 from a file.
 *
 *  @param data             Receives the data read from the file.
 *  @param theFile          The file to read from.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Int32_Read (
    TQ3Int32                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int32_Write
 *  @discussion
 *      Write a TQ3Int32 to a file.
 *
 *  @param data             The data to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Int32_Write (
    TQ3Int32                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns64_Read
 *  @discussion
 *      Read a TQ3Uns64 from a file.
 *
 *  @param data             Receives the data read from the file.
 *  @param theFile          The file to read from.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Uns64_Read (
    TQ3Uns64                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Uns64_Write
 *  @discussion
 *      Write a TQ3Uns64 to a file.
 *
 *  @param data             The data to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Uns64_Write (
    TQ3Uns64                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int64_Read
 *  @discussion
 *      Read a TQ3Int64 from a file.
 *
 *  @param data             Receives the data read from the file.
 *  @param theFile          The file to read from.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Int64_Read (
    TQ3Int64                      *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Int64_Write
 *  @discussion
 *      Write a TQ3Int64 to a file.
 *
 *  @param data             The data to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Int64_Write (
    TQ3Int64                      data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Float32_Read
 *  @discussion
 *      Read a TQ3Float32 from a file.
 *
 *  @param data             Receives the data read from the file.
 *  @param theFile          The file to read from.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Float32_Read (
    TQ3Float32                    *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Float32_Write
 *  @discussion
 *      Write a TQ3Float32 to a file.
 *
 *  @param data             The data to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Float32_Write (
    TQ3Float32                    data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Float64_Read
 *  @discussion
 *      Read a TQ3Float64 from a file.
 *
 *  @param data             Receives the data read from the file.
 *  @param theFile          The file to read from.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Float64_Read (
    TQ3Float64                    *data,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Float64_Write
 *  @discussion
 *      Write a TQ3Float64 to a file.
 *
 *  @param data             The data to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Size  )
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
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Status  )
Q3String_WriteUnlimited (
    const char                    *data,
    TQ3FileObject                 theFile
);
#endif



/*!
 *  @function
 *      Q3RawData_Read
 *  @discussion
 *      Read a block of data from a file.
 *
 *  @param data             Receives the data read from the file.
 *  @param size             The number of bytes to read.
 *  @param theFile          The file to read from.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RawData_Read (
    unsigned char                 *data,
    TQ3Uns32                      size,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3RawData_Write
 *  @discussion
 *      Write a block of data to a file.
 *
 *  @param data             The data to write to the file.
 *  @param size             The number of bytes to write.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RawData_Write (
    const unsigned char           *data,
    TQ3Uns32                      size,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Point2D_Read
 *  @discussion
 *      Read a TQ3Point2D from a file.
 *
 *  @param point2D          Receives the point read from the file.
 *  @param theFile          The file to read from.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Point2D_Read (
    TQ3Point2D                    *point2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Point2D_Write
 *  @discussion
 *      Write a TQ3Point2D to a file.
 *
 *  @param point2D          The point to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Point2D_Write (
    const TQ3Point2D              *point2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Point3D_Read
 *  @discussion
 *      Read a TQ3Point3D from a file.
 *
 *  @param point3D          Receives the point read from the file.
 *  @param theFile          The file to read from.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Point3D_Read (
    TQ3Point3D                    *point3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Point3D_Write
 *  @discussion
 *      Write a TQ3Point3D to a file.
 *
 *  @param point3D          The point to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Point3D_Write (
    const TQ3Point3D              *point3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3RationalPoint3D_Read
 *  @discussion
 *      Read a TQ3RationalPoint3D from a file.
 *
 *  @param point3D          Receives the point read from the file.
 *  @param theFile          The file to read from.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RationalPoint3D_Read (
    TQ3RationalPoint3D            *point3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3RationalPoint3D_Write
 *  @discussion
 *      Write a TQ3RationalPoint3D to a file.
 *
 *  @param point3D          The point to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RationalPoint3D_Write (
    const TQ3RationalPoint3D      *point3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3RationalPoint4D_Read
 *  @discussion
 *      Read a TQ3RationalPoint4D from a file.
 *
 *  @param point4D          Receives the point read from the file.
 *  @param theFile          The file to read from.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RationalPoint4D_Read (
    TQ3RationalPoint4D            *point4D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3RationalPoint4D_Write
 *  @discussion
 *      Write a TQ3RationalPoint4D to a file.
 *
 *  @param point4D          The point to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RationalPoint4D_Write (
    const TQ3RationalPoint4D      *point4D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Vector2D_Read
 *  @discussion
 *      Read a TQ3Vector2D from a file.
 *
 *  @param vector2D         Receives the vector read from the file.
 *  @param theFile          The file to read from.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Vector2D_Read (
    TQ3Vector2D                   *vector2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Vector2D_Write
 *  @discussion
 *      Write a TQ3Vector2D to a file.
 *
 *  @param vector2D         The vector to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Vector2D_Write (
    const TQ3Vector2D             *vector2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Vector3D_Read
 *  @discussion
 *      Read a TQ3Vector3D from a file.
 *
 *  @param vector3D         Receives the vector read from the file.
 *  @param theFile          The file to read from.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Vector3D_Read (
    TQ3Vector3D                   *vector3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Vector3D_Write
 *  @discussion
 *      Write a TQ3Vector3D to a file.
 *
 *  @param vector3D         The vector to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Vector3D_Write (
    const TQ3Vector3D             *vector3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Matrix4x4_Read
 *  @discussion
 *      Read a TQ3Matrix4x4 from a file.
 *
 *  @param matrix4x4        Receives the matrix read from the file.
 *  @param theFile          The file to read from.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Matrix4x4_Read (
    TQ3Matrix4x4                  *matrix4x4,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Matrix4x4_Write
 *  @discussion
 *      Write a TQ3Matrix4x4 to a file.
 *
 *  @param matrix4x4        The matrix to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Matrix4x4_Write (
    const TQ3Matrix4x4            *matrix4x4,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Tangent2D_Read
 *  @discussion
 *      Read a TQ3Tangent2D from a file.
 *
 *  @param tangent2D        Receives the tangent read from the file.
 *  @param theFile          The file to read from.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tangent2D_Read (
    TQ3Tangent2D                  *tangent2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Tangent2D_Write
 *  @discussion
 *      Write a TQ3Tangent2D to a file.
 *
 *  @param tangent2D        The tangent to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tangent2D_Write (
    const TQ3Tangent2D            *tangent2D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Tangent3D_Read
 *  @discussion
 *      Read a TQ3Tangent3D from a file.
 *
 *  @param tangent3D        Receives the tangent read from the file.
 *  @param theFile          The file to read from.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tangent3D_Read (
    TQ3Tangent3D                  *tangent3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Tangent3D_Write
 *  @discussion
 *      Write a TQ3Tangent3D to a file.
 *
 *  @param tangent3D        The tangent to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tangent3D_Write (
    const TQ3Tangent3D            *tangent3D,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Comment_Write
 *  @discussion
 *      Write a comment to a file.
 *
 *  @param comment          The comment to write to the file.
 *  @param theFile          The file to write to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Comment_Write (
    char                          *comment,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Unknown_GetType
 *  @discussion
 *      Get the type of an unknown object.
 *
 *  @param unknownObject    The object to query.
 *  @result                 The type of the object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Unknown_GetType (
    TQ3UnknownObject              unknownObject
);



/*!
 *  @function
 *      Q3Unknown_GetDirtyState
 *  @discussion
 *      Get the dirty state of an unknown object.
 *
 *  @param unknownObject    The object to query.
 *  @param isDirty          Receives the dirty state of the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Unknown_GetDirtyState (
    TQ3UnknownObject              unknownObject,
    TQ3Boolean                    *isDirty
);



/*!
 *  @function
 *      Q3Unknown_SetDirtyState
 *  @discussion
 *      Set the dirty state for an unknown object.
 *
 *  @param unknownObject    The object to update.
 *  @param isDirty          The new dirty state of the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Unknown_SetDirtyState (
    TQ3UnknownObject              unknownObject,
    TQ3Boolean                    isDirty
);



/*!
 *  @function
 *      Q3UnknownText_GetData
 *  @discussion
 *      Get the data from an unknown text object.
 *
 *  @param unknownObject    The object to query.
 *  @param unknownTextData  Receives the text data from the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3UnknownText_GetData (
    TQ3UnknownObject              unknownObject,
    TQ3UnknownTextData            *unknownTextData
);



/*!
 *  @function
 *      Q3UnknownText_EmptyData
 *  @discussion
 *      Release the data of an unknown text object.
 *
 *  @param unknownTextData  The text data to release.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3UnknownText_EmptyData (
    TQ3UnknownTextData            *unknownTextData
);



/*!
 *  @function
 *      Q3UnknownBinary_GetData
 *  @discussion
 *      Get the data from an unknown binary object.
 *
 *  @param unknownObject        The object to query.
 *  @param unknownBinaryData    Receives the binary data from the object.
 *  @result                     Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_GetData (
    TQ3UnknownObject              unknownObject,
    TQ3UnknownBinaryData          *unknownBinaryData
);



/*!
 *  @function
 *      Q3UnknownBinary_EmptyData
 *  @discussion
 *      Release the data of an unknown binary object.
 *
 *  @param unknownBinaryData    The binary data to release.
 *  @result                     Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_EmptyData (
    TQ3UnknownBinaryData          *unknownBinaryData
);



/*!
 *  @function
 *      Q3UnknownBinary_GetTypeString
 *  @discussion
 *      Get the type string of an unknown binary object.
 *
 *  @param unknownObject    The object to query.
 *  @param typeString       Receives the type string from the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_GetTypeString (
    TQ3UnknownObject              unknownObject,
    char                          **typeString
);



/*!
 *  @function
 *      Q3UnknownBinary_EmptyTypeString
 *  @discussion
 *      Release the type string of an unknown binary object.
 *
 *  @param typeString       The type string to release.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3UnknownBinary_EmptyTypeString (
    char                          **typeString
);



/*!
 *  @function
 *      Q3ViewHints_New
 *  @discussion
 *      Create a new view hints object.
 *
 *  @param view             The view the view hints should be based on.
 *  @result                 The new view hints object.
 */
Q3_EXTERN_API_C ( TQ3ViewHintsObject  )
Q3ViewHints_New (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3ViewHints_SetRenderer
 *  @discussion
 *      Set the renderer for a view hints object.
 *
 *  @param viewHints        The view hints to update.
 *  @param renderer         The new renderer for the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetRenderer (
    TQ3ViewHintsObject            viewHints,
    TQ3RendererObject             renderer
);



/*!
 *  @function
 *      Q3ViewHints_GetRenderer
 *  @discussion
 *      Get the renderer from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @param renderer         Receives the renderer from the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetRenderer (
    TQ3ViewHintsObject            viewHints,
    TQ3RendererObject             *renderer
);



/*!
 *  @function
 *      Q3ViewHints_SetCamera
 *  @discussion
 *      Set the camera for a view hints object.
 *
 *  @param viewHints        The view hints to update.
 *  @param camera           The new camera for the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetCamera (
    TQ3ViewHintsObject            viewHints,
    TQ3CameraObject               camera
);



/*!
 *  @function
 *      Q3ViewHints_GetCamera
 *  @discussion
 *      Get the camera from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @param camera           Receives the camera from the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetCamera (
    TQ3ViewHintsObject            viewHints,
    TQ3CameraObject               *camera
);



/*!
 *  @function
 *      Q3ViewHints_SetLightGroup
 *  @discussion
 *      Set the light group for a view hints object.
 *
 *  @param viewHints        The view hints to update.
 *  @param lightGroup       The new light group for the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetLightGroup (
    TQ3ViewHintsObject            viewHints,
    TQ3GroupObject                lightGroup
);



/*!
 *  @function
 *      Q3ViewHints_GetLightGroup
 *  @discussion
 *      Get the light group from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @param lightGroup       Receives the light group from the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetLightGroup (
    TQ3ViewHintsObject            viewHints,
    TQ3GroupObject                *lightGroup
);



/*!
 *  @function
 *      Q3ViewHints_SetAttributeSet
 *  @discussion
 *      Set the attribute set for a view hints object.
 *
 *  @param viewHints        The view hints to update.
 *  @param attributeSet     The new attribute set for the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetAttributeSet (
    TQ3ViewHintsObject            viewHints,
    TQ3AttributeSet               attributeSet
);



/*!
 *  @function
 *      Q3ViewHints_GetAttributeSet
 *  @discussion
 *      Get the attribute set from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @param attributeSet     Receives the attribute set from the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetAttributeSet (
    TQ3ViewHintsObject            viewHints,
    TQ3AttributeSet               *attributeSet
);



/*!
 *  @function
 *      Q3ViewHints_SetDimensionsState
 *  @discussion
 *      Set the dimension state of a view hints object.
 *
 *  @param viewHints        The view hints to update.
 *  @param isValid          The new dimension state for the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetDimensionsState (
    TQ3ViewHintsObject            viewHints,
    TQ3Boolean                    isValid
);



/*!
 *  @function
 *      Q3ViewHints_GetDimensionsState
 *  @discussion
 *      Get the dimension state of a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @param isValid          Receives the dimension state from the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetDimensionsState (
    TQ3ViewHintsObject            viewHints,
    TQ3Boolean                    *isValid
);



/*!
 *  @function
 *      Q3ViewHints_SetDimensions
 *  @discussion
 *      Set the dimensions of a view hints object.
 *
 *  @param viewHints        The view hints to update.
 *  @param width            The new width for the view hints.
 *  @param height           The new height for the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetDimensions (
    TQ3ViewHintsObject            viewHints,
    TQ3Uns32                      width,
    TQ3Uns32                      height
);



/*!
 *  @function
 *      Q3ViewHints_GetDimensions
 *  @discussion
 *      Get the dimensions of a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @param width            Receives the width from the view hints.
 *  @param height           Receives the height from the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetDimensions (
    TQ3ViewHintsObject            viewHints,
    TQ3Uns32                      *width,
    TQ3Uns32                      *height
);



/*!
 *  @function
 *      Q3ViewHints_SetMaskState
 *  @discussion
 *      Set the mask state of a view hints object.
 *
 *  @param viewHints        The view hints to update.
 *  @param isValid          The new mask state for the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetMaskState (
    TQ3ViewHintsObject            viewHints,
    TQ3Boolean                    isValid
);



/*!
 *  @function
 *      Q3ViewHints_GetMaskState
 *  @discussion
 *      Get the mask state from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @param isValid          Receives the mask state from the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetMaskState (
    TQ3ViewHintsObject            viewHints,
    TQ3Boolean                    *isValid
);



/*!
 *  @function
 *      Q3ViewHints_SetMask
 *  @discussion
 *      Set the mask of a view hints object.
 *
 *  @param viewHints        The view hints to update.
 *  @param mask             The new mask for the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetMask (
    TQ3ViewHintsObject            viewHints,
    const TQ3Bitmap               *mask
);



/*!
 *  @function
 *      Q3ViewHints_GetMask
 *  @discussion
 *      Get the mask from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @param mask             Receives the mask from the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetMask (
    TQ3ViewHintsObject            viewHints,
    TQ3Bitmap                     *mask
);



/*!
 *  @function
 *      Q3ViewHints_SetClearImageMethod
 *  @discussion
 *      Set the clear image method of a view hints object.
 *
 *  @param viewHints        The view hints to update.
 *  @param clearMethod      The new clear image method for the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetClearImageMethod (
    TQ3ViewHintsObject            viewHints,
    TQ3DrawContextClearImageMethod clearMethod
);



/*!
 *  @function
 *      Q3ViewHints_GetClearImageMethod
 *  @discussion
 *      Get the clear image method from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @param clearMethod      Receives the clear image method from the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetClearImageMethod (
    TQ3ViewHintsObject            viewHints,
    TQ3DrawContextClearImageMethod *clearMethod
);



/*!
 *  @function
 *      Q3ViewHints_SetClearImageColor
 *  @discussion
 *      Set the clear image color of a view hints object.
 *
 *  @param viewHints        The view hints to update.
 *  @param color            The new clear image color for the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_SetClearImageColor (
    TQ3ViewHintsObject            viewHints,
    const TQ3ColorARGB            *color
);



/*!
 *  @function
 *      Q3ViewHints_GetClearImageColor
 *  @discussion
 *      Get the clear image color from a view hints object.
 *
 *  @param viewHints        The view hints to query.
 *  @param color            Receives the clear image color from the view hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewHints_GetClearImageColor (
    TQ3ViewHintsObject            viewHints,
    TQ3ColorARGB                  *color
);



/*!
 *  @function
 *      Q3File_GetFileFormat  
 *  @discussion
 *      Get the file format used to process a file.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theFile          The file to query.
 *  @result                 The file format used to process the file.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3FileFormatObject  )
Q3File_GetFileFormat (
    TQ3FileObject                 theFile
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_NewFromType
 *  @discussion
 *      Create a new file format object.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param fformatObjectType    The type of the file format.
 *  @result                     The new file format object.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3FileFormatObject  )
Q3FileFormat_NewFromType (
    TQ3ObjectType                 fformatObjectType
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GetType
 *  @discussion
 *      Get the type of a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to query.
 *  @result                 The type of the file format.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3FileFormat_GetType (
    TQ3FileFormatObject           format
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_HasModalConfigure
 *  @discussion
 *      Does a file format have a modal configure dialog?
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to query.
 *  @result                 Does the file format have a modal configure dialog?
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Boolean  )
Q3FileFormat_HasModalConfigure (
    TQ3FileFormatObject           format
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_ModalConfigure
 *  @discussion
 *      Invoke a file format's modal configure dialog.
 *
 *      If the file format provides a user interface for adjusting its preferences,
 *      a modal configure dialog can be displayed by this function.
 *
 *      The cancel/accept state of the dialog is returned through the cancelled
 *      parameter.
 *
 *      After a configure dialog has been accepted, the current preferences
 *      should be retrieved with Q3FileFormat_GetConfigurationData and stored by
 *      the application for later recall.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format whose configure dialog is to be displayed.
 *  @param dialogAnchor     Platform-specific dialog data.
 *  @param cancelled        Receives the OK/Cancel state of the dialog.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Get the user-visible name of a file format.
 *
 *      The user visible name is suitable for display in a menu or window.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param formatClassType      The class type of the file format to query.
 *  @param formatClassString    Receives the name of the file format.
 *  @result                     Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormatClass_GetFormatNameString (
    TQ3ObjectType                 formatClassType,
    TQ3ObjectClassNameString      formatClassString
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GetConfigurationData
 *  @discussion
 *      Get the configuration data for a file format.
 *
 *      Configuration data should be saved by the application in a manner appropriate
 *      for the current platform (e.g., the Registry on Windows or a .plist file on
 *      the Mac), tagging it with the file format's type for later identification.
 *
 *      If dataBuffer is NULL, the size of data required to store the configuration
 *      data will be returned in actualDataSize.
 *
 *      Otherwise bufferSize should be set to the number of bytes pointed to by
 *      dataBuffer, and actualDataSize will receive the number of bytes written to
 *      dataBuffer.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to query.
 *  @param dataBuffer       Receives the file format configuration data. May be NULL.
 *  @param bufferSize       The number of bytes pointed to by dataBuffer. May be 0.
 *  @param actualDataSize   Receives the number of bytes required for, or written to, dataBuffer.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Set the configuration data for a file format.
 *
 *      The configuration must have been obtained with a previous call to
 *      Q3FileFormat_GetConfigurationData.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to update.
 *  @param dataBuffer       The configuration data for the file format.
 *  @param bufferSize       The number of bytes pointed to by dataBuffer.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Read a TQ3Int8 from a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to read with.
 *  @param data             Receives the data read from the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_8 (
    TQ3FileFormatObject           format,
    TQ3Int8                       *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinary_16
 *  @discussion
 *      Read a TQ3Int16 from a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to read with.
 *  @param data             Receives the data read from the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_16 (
    TQ3FileFormatObject           format,
    TQ3Int16                      *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinary_32
 *  @discussion
 *      Read a TQ3Int32 from a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to read with.
 *  @param data             Receives the data read from the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_32 (
    TQ3FileFormatObject           format,
    TQ3Int32                      *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinary_64
 *  @discussion
 *      Read a TQ3Int64 from a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to read with.
 *  @param data             Receives the data read from the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinary_64 (
    TQ3FileFormatObject           format,
    TQ3Int64                      *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinary_String
 *  @discussion
 *      Read a string from a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to read with.
 *  @param data             Receives the data read from the file format.
 *  @param length           Receives the number of bytes written to data.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Read a block of data from a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to read with.
 *  @param data             Receives the data read from the file format.
 *  @param length           The number of bytes to read from the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Read and endian swap a TQ3Int16 from a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to read with.
 *  @param data             Receives the data read from the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinSwap_16 (
    TQ3FileFormatObject           format,
    TQ3Int16                      *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinSwap_32
 *  @discussion
 *      Read and endian swap a TQ3Int32 from a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to read with.
 *  @param data             Receives the data read from the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadBinSwap_32 (
    TQ3FileFormatObject           format,
    TQ3Int32                      *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadBinSwap_64
 *  @discussion
 *      Read and endian swap a TQ3Int64 from a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to read with.
 *  @param data             Receives the data read from the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Read and discard whitespace from a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to read with.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericReadText_SkipBlanks (
    TQ3FileFormatObject           format
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericReadText_ReadUntilChars
 *  @discussion
 *      Read until characters have been found from a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to read with.
 *  @param buffer           The buffer to read to.
 *  @param chars            The list of "stop" characters.
 *  @param numChars         The number of characters in chars.
 *  @param blanks           Should reading stop on any characters < 0x20.
 *  @param foundChar        The character which caused reading to stop.
 *  @param maxLen           The maximum number of characters to read.
 *  @param charsRead        Receives the number of characters read.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Write a TQ3Int8 to a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to write with.
 *  @param data             The data to write to the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_8 (
    TQ3FileFormatObject           format,
    const TQ3Int8                 *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinary_16
 *  @discussion
 *      Write a TQ3Int16 to a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to write with.
 *  @param data             The data to write to the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_16 (
    TQ3FileFormatObject           format,
    const TQ3Int16                *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinary_32
 *  @discussion
 *      Write a TQ3Int32 to a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to write with.
 *  @param data             The data to write to the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_32 (
    TQ3FileFormatObject           format,
    const TQ3Int32                *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinary_64
 *  @discussion
 *      Write a TQ3Int64 to a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to write with.
 *  @param data             The data to write to the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinary_64 (
    TQ3FileFormatObject           format,
    const TQ3Int64                *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinary_String
 *  @discussion
 *      Write a string to a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to write with.
 *  @param data             The data to write to the file format.
 *  @param length           The number of bytes to write.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Write a block of data to a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to write with.
 *  @param data             The data to write to the file format.
 *  @param length           The number of bytes to write.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Endian swap and write a TQ3Int16 to a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to write with.
 *  @param data             The data to write to the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinSwap_16 (
    TQ3FileFormatObject           format,
    const TQ3Int16                *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinSwap_32
 *  @discussion
 *      Endian swap and write a TQ3Int32 to a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to write with.
 *  @param data             The data to write to the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3FileFormat_GenericWriteBinSwap_32 (
    TQ3FileFormatObject           format,
    const TQ3Int32                *data
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3FileFormat_GenericWriteBinSwap_64
 *  @discussion
 *      Endian swap and write a TQ3Int64 to a file format.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param format           The file format to write with.
 *  @param data             The data to write to the file format.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
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

Q3_EXTERN_API_C ( TQ3Status  )
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

Q3_EXTERN_API_C ( TQ3Status  )
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

Q3_EXTERN_API_C ( TQ3Status  )
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
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Uns8_ReadArray (
	TQ3Uns32					numNums,
	TQ3Uns8*					intArray,
	TQ3FileObject            	theFile
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif



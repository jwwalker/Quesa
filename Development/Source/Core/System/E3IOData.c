/*  NAME:
        E3IO.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3IOData.h"
#include "E3IO.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct TE3UnknownBinary_Data {
	char					*typeString;
	TQ3UnknownBinaryData	data;
} TE3UnknownBinary_Data;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3unknown_binary_delete : UnknownBinary delete method.
//-----------------------------------------------------------------------------
static void
e3unknown_binary_delete(TQ3Object theObject, void *privateData)
{	
	TE3UnknownBinary_Data		*instanceData = (TE3UnknownBinary_Data *) privateData;
	
#pragma unused(theObject)

	// Dispose of our instance data
	E3Memory_Free(&instanceData->typeString);
	E3UnknownBinary_EmptyData (&instanceData->data);
}





//=============================================================================
//      e3unknown_binary_duplicateData : UnknownBinary duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3unknown_binary_duplicateData(const TQ3UnknownBinaryData *fromData, TQ3UnknownBinaryData *toData)
{	
	TQ3Status				qd3dStatus			= kQ3Success ;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toData),   kQ3Failure);



	
	// Copy the data members
	toData->objectType = fromData->objectType;
	toData->size = fromData->size;
	toData->byteOrder = fromData->byteOrder;

	// Copy the contents
	if (fromData->size != 0)
		{
		toData->contents = (char *) E3Memory_Allocate(fromData->size);
		if (toData->contents != NULL)
			memcpy(toData->contents, fromData->contents,fromData->size);
		else
			qd3dStatus = kQ3Failure;
		}
	else
		toData->contents = NULL;
	// Handle failure
	if (qd3dStatus != kQ3Success)
		{
		E3UnknownBinary_EmptyData(toData);
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3unknown_binary_duplicate : UnknownBinary duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3unknown_binary_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						   TQ3Object toObject,         void *toPrivateData)
{	
	const TE3UnknownBinary_Data	*fromInstanceData	= (const TE3UnknownBinary_Data*) fromPrivateData;
	TE3UnknownBinary_Data		*toInstanceData		= (TE3UnknownBinary_Data*) toPrivateData;
	TQ3Status					qd3dStatus			= kQ3Success ;

#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	
	// Copy the string
	if (fromInstanceData->typeString != NULL)
		{
		toInstanceData->typeString = (char *) E3Memory_Allocate(strlen(fromInstanceData->typeString) + 1);
		if (toInstanceData->typeString == NULL)
			return(kQ3Failure);
		strcpy(toInstanceData->typeString, fromInstanceData->typeString);	
		}
	else
		toInstanceData->typeString = NULL;
	
	// Copy the data members
	qd3dStatus = e3unknown_binary_duplicateData (&fromInstanceData->data, &toInstanceData->data);

	return(qd3dStatus);
}





//=============================================================================
//      e3unknown_binary_new : UnknownBinary new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3unknown_binary_new(TQ3Object theObject, void *privateData, const void *paramData)
{	

	return e3unknown_binary_duplicate (NULL, paramData, theObject, privateData);
}





//=============================================================================
//      e3unknown_binary_metahandler : UnknownBinary metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3unknown_binary_metahandler(TQ3XMethodType methodType)
{	
	TQ3XFunctionPointer		theMethod = NULL;

	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3unknown_binary_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3unknown_binary_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3unknown_binary_duplicate;
			break;

		}
	
	return(theMethod);
}





//=============================================================================
//      e3unknown_text_delete : UnknownText delete method.
//-----------------------------------------------------------------------------
static void
e3unknown_text_delete(TQ3Object theObject, void *privateData)
{	
	TQ3UnknownTextData		*instanceData = (TQ3UnknownTextData *) privateData;
	
#pragma unused(theObject)

	// Dispose of our instance data
	E3UnknownText_EmptyData (instanceData);
}





//=============================================================================
//      e3unknown_text_duplicateData : UnknownText duplicate Data method.
//-----------------------------------------------------------------------------
static TQ3Status
e3unknown_text_duplicateData(const TQ3UnknownTextData *fromData, TQ3UnknownTextData *toData)
{	
	TQ3Status				qd3dStatus			= kQ3Success ;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toData),   kQ3Failure);



	
	// Copy the name
	if (fromData->objectName != NULL)
		toData->objectName = (char *) E3Memory_Allocate(strlen(fromData->objectName) + 1);
	if (toData->objectName == NULL)
		return(kQ3Failure);
	strcpy(toData->objectName, fromData->objectName);	
	
	// Copy the contents
	if (fromData->contents != 0)
		toData->contents = (char *) E3Memory_Allocate(strlen(fromData->contents) + 1);
	if (toData->contents == NULL)
		strcpy(toData->contents, fromData->contents);	
	else
		qd3dStatus = kQ3Failure;

	// Handle failure
	if (qd3dStatus != kQ3Success)
		{
		e3unknown_text_delete(NULL, toData);
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3unknown_text_duplicate : UnknownText duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3unknown_text_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						 TQ3Object toObject,         void *toPrivateData)
{	
	const TQ3UnknownTextData	*fromInstanceData	= (const TQ3UnknownTextData*) fromPrivateData;
	TQ3UnknownTextData			*toInstanceData		= (TQ3UnknownTextData*) toPrivateData;
	TQ3Status					qd3dStatus			= kQ3Success ;

#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);


	qd3dStatus = e3unknown_text_duplicateData (fromInstanceData, toInstanceData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3unknown_text_new : UnknownBinary new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3unknown_text_new(TQ3Object theObject, void *privateData, const void *paramData)
{	

	return e3unknown_text_duplicate (NULL, paramData, theObject, privateData);
}





//=============================================================================
//      e3unknown_text_metahandler : UnknownText metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3unknown_text_metahandler(TQ3XMethodType methodType)
{	
	TQ3XFunctionPointer		theMethod = NULL;

	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3unknown_text_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3unknown_text_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3unknown_text_duplicate;
			break;

		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Shared_ClearEditTracking : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Shared_ClearEditTracking(TQ3SharedObject sharedObject)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Shared_GetEditTrackingState : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Shared_GetEditTrackingState(TQ3SharedObject sharedObject)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3NewLine_Write : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3NewLine_Write(TQ3FileObject theFile)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Uns8_Read : Read a TQ3Uns8.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Uns8_Read(TQ3Uns8 *data, TQ3FileObject theFile)
{
	return Q3Int8_Read ((TQ3Int8*)data, theFile);
}





//=============================================================================
//      E3Uns8_Write : Write a TQ3Uns8.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns8_Write(TQ3Uns8 data, TQ3FileObject theFile)
{
	return Q3Int8_Write ((TQ3Int8)data, theFile);
}





//=============================================================================
//      E3Uns16_Read : Read a TQ3Uns16.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns16_Read(TQ3Uns16 *data, TQ3FileObject theFile)
{
	return Q3Int16_Read ((TQ3Int16*)data, theFile);
}





//=============================================================================
//      E3Uns16_Write : Write a TQ3Uns16.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns16_Write(TQ3Uns16 data, TQ3FileObject theFile)
{
	return Q3Int16_Write ((TQ3Int16)data, theFile);
}





//=============================================================================
//      E3Uns32_Read : Read a TQ3Uns32.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns32_Read(TQ3Uns32 *data, TQ3FileObject theFile)
{
	return Q3Int32_Read ((TQ3Int32*)data, theFile);
}





//=============================================================================
//      E3Uns32_Write : Write a TQ3Uns32.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns32_Write(TQ3Uns32 data, TQ3FileObject theFile)
{
	return Q3Int32_Write ((TQ3Int32)data, theFile);
}





//=============================================================================
//      E3Int8_Read : Read a TQ3Int8.
//-----------------------------------------------------------------------------
TQ3Status
E3Int8_Read(TQ3Int8 *data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatInt8ReadMethod 		int8Read;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	int8Read = (TQ3XFFormatInt8ReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatInt8Read);

	if (int8Read != NULL)
		return int8Read(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Int8_Write : Write a TQ3Int8.
//-----------------------------------------------------------------------------
TQ3Status
E3Int8_Write(TQ3Int8 data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatInt8WriteMethod 		int8Write;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	int8Write = (TQ3XFFormatInt8WriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatInt8Write);

	if (int8Write != NULL)
		return int8Write(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Int16_Read : Read a TQ3Int16.
//-----------------------------------------------------------------------------
TQ3Status
E3Int16_Read(TQ3Int16 *data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatInt16ReadMethod 		int16Read;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	int16Read = (TQ3XFFormatInt16ReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatInt16Read);

	if(int16Read != NULL)
		return int16Read(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Int16_Write : Write a TQ3Int16.
//-----------------------------------------------------------------------------
TQ3Status
E3Int16_Write(TQ3Int16 data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatInt16WriteMethod 		int16Write;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	int16Write = (TQ3XFFormatInt16WriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatInt16Write);

	if(int16Write != NULL)
		return int16Write(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Int32_Read : Read a TQ3Int32.
//-----------------------------------------------------------------------------
TQ3Status
E3Int32_Read(TQ3Int32 *data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatInt32ReadMethod 		int32Read;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	int32Read = (TQ3XFFormatInt32ReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatInt32Read);

	if(int32Read != NULL)
		return int32Read(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Int32_Write : Write a TQ3Int32.
//-----------------------------------------------------------------------------
TQ3Status
E3Int32_Write(TQ3Int32 data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatInt32WriteMethod 		int32Write;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	int32Write = (TQ3XFFormatInt32WriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatInt32Write);

	if(int32Write != NULL)
		return int32Write(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Uns64_Read : Read a TQ3Uns64.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns64_Read(TQ3Uns64 *data, TQ3FileObject theFile)
{
	return Q3Int64_Read ((TQ3Int64*)data, theFile);
}





//=============================================================================
//      E3Uns64_Write : Write a TQ3Uns64.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns64_Write(TQ3Uns64 data, TQ3FileObject theFile)
{
	return Q3Int64_Write (*((TQ3Int64*)&data), theFile);
}





//=============================================================================
//      E3Int64_Read : Read a TQ3Int64.
//-----------------------------------------------------------------------------
TQ3Status
E3Int64_Read(TQ3Int64 *data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatInt64ReadMethod 		int64Read;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	int64Read = (TQ3XFFormatInt64ReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatInt64Read);

	if(int64Read != NULL)
		return int64Read(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Int64_Write : Write a TQ3Int64.
//-----------------------------------------------------------------------------
TQ3Status
E3Int64_Write(TQ3Int64 data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatInt64WriteMethod 		int64Write;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	int64Write = (TQ3XFFormatInt64WriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatInt64Write);

	if(int64Write != NULL)
		return int64Write(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Float32_Read : Read a TQ3Float32.
//-----------------------------------------------------------------------------
TQ3Status
E3Float32_Read(TQ3Float32 *data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatFloat32ReadMethod 	float32Read;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Read);

	if(float32Read != NULL)
		return float32Read(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Float32_Write : Write a TQ3Float32.
//-----------------------------------------------------------------------------
TQ3Status
E3Float32_Write(TQ3Float32 data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatFloat32WriteMethod 	float32Write;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL)
		return float32Write(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Float64_Read : Read a TQ3Float64.
//-----------------------------------------------------------------------------
TQ3Status
E3Float64_Read(TQ3Float64 *data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatFloat64ReadMethod 	float64Read;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float64Read = (TQ3XFFormatFloat64ReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat64Read);

	if(float64Read != NULL)
		return float64Read(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Float64_Write : Write a TQ3Float64.
//-----------------------------------------------------------------------------
TQ3Status
E3Float64_Write(TQ3Float64 data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatFloat64WriteMethod 	float64Write;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float64Write = (TQ3XFFormatFloat64WriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat64Write);

	if(float64Write != NULL)
		return float64Write(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Size_Pad : Pad a number ofbytes to be long-word aligned.
//-----------------------------------------------------------------------------
TQ3Size
E3Size_Pad(TQ3Size size)
{
	
	if((size & 0x03) != 0)
		size = (size & 0xFFFFFFF8) + 4;
		
	return size;
}





//=============================================================================
//      E3String_Read : Read a string.
//-----------------------------------------------------------------------------
TQ3Status
E3String_Read(char *data, TQ3Uns32 *length, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatStringReadMethod 	stringRead;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	stringRead = (TQ3XFFormatStringReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatStringRead);

	if(stringRead != NULL)
		return stringRead(instanceData->format,data,length);

	return(kQ3Failure);
}





//=============================================================================
//      E3String_Write : Write a string.
//-----------------------------------------------------------------------------
TQ3Status
E3String_Write(const char *data, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatStringWriteMethod 	stringWrite;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	stringWrite = (TQ3XFFormatStringWriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatStringWrite);

	if(stringWrite != NULL)
		return stringWrite(instanceData->format,data);

	return(kQ3Failure);
}





//=============================================================================
//      E3RawData_Read : Read a block of raw data.
//-----------------------------------------------------------------------------
TQ3Status
E3RawData_Read(unsigned char *data, TQ3Uns32 size, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatRawReadMethod	 	rawRead;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	rawRead = (TQ3XFFormatRawReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatRawRead);

	if(rawRead != NULL)
		return rawRead(instanceData->format,data,size);

	return(kQ3Failure);
}





//=============================================================================
//      E3RawData_Write : Write a block of raw data.
//-----------------------------------------------------------------------------
TQ3Status
E3RawData_Write(const unsigned char *data, TQ3Uns32 size, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatRawWriteMethod	 	rawWrite;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	rawWrite = (TQ3XFFormatRawWriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatRawWrite);

	if(rawWrite != NULL)
		return rawWrite(instanceData->format,data,size);

	return(kQ3Failure);
}





//=============================================================================
//      E3Point2D_Read : Read a TQ3Point2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Point2D_Read(TQ3Point2D *point2D, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3Status						result        = kQ3Failure;
	TQ3XFFormatFloat32ReadMethod 	float32Read;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Read);

	if(float32Read != NULL){
		result = float32Read(instanceData->format,&point2D->x);
		if(result == kQ3Success)
			result = float32Read(instanceData->format,&point2D->y);
		}

	return(result);
}





//=============================================================================
//      E3Point2D_Write : Write a TQ3Point2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Point2D_Write(const TQ3Point2D *point2D, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3Status						result        = kQ3Failure;
	TQ3XFFormatFloat32WriteMethod 	float32Write;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL){
		result = float32Write(instanceData->format,point2D->x);
		if(result == kQ3Success)
			result = float32Write(instanceData->format,point2D->y);
		}

	return(result);
}





//=============================================================================
//      E3Point3D_Read : Read a TQ3Point3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Point3D_Read(TQ3Point3D *point3D, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32ReadMethod 	float32Read;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Read);

	if(float32Read != NULL){
		result = float32Read(instanceData->format,&point3D->x);
		if(result == kQ3Success)
			result = float32Read(instanceData->format,&point3D->y);
		if(result == kQ3Success)
			result = float32Read(instanceData->format,&point3D->z);
		}

	return(result);
}





//=============================================================================
//      E3Point3D_Write : Write a TQ3Point3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Point3D_Write(const TQ3Point3D *point3D, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32WriteMethod 	float32Write;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL){
		result = float32Write(instanceData->format,point3D->x);
		if(result == kQ3Success)
			result = float32Write(instanceData->format,point3D->y);
		if(result == kQ3Success)
			result = float32Write(instanceData->format,point3D->z);
		}

	return(result);
}





//=============================================================================
//      E3RationalPoint3D_Read : Read a TQ3RationalPoint3D.
//-----------------------------------------------------------------------------
TQ3Status
E3RationalPoint3D_Read(TQ3RationalPoint3D *point3D, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32ReadMethod 	float32Read;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Read);

	if(float32Read != NULL){
		result = float32Read(instanceData->format,&point3D->x);
		if(result == kQ3Success)
			result = float32Read(instanceData->format,&point3D->y);
		if(result == kQ3Success)
			result = float32Read(instanceData->format,&point3D->w);
		}

	return(result);
}





//=============================================================================
//      E3RationalPoint3D_Write : Write a TQ3RationalPoint3D.
//-----------------------------------------------------------------------------
TQ3Status
E3RationalPoint3D_Write(const TQ3RationalPoint3D *point3D, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32WriteMethod 	float32Write;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL){
		result = float32Write(instanceData->format,point3D->x);
		if(result == kQ3Success)
			result = float32Write(instanceData->format,point3D->y);
		if(result == kQ3Success)
			result = float32Write(instanceData->format,point3D->w);
		}

	return(result);
}





//=============================================================================
//      E3RationalPoint4D_Read : Read a TQ3RationalPoint4D.
//-----------------------------------------------------------------------------
TQ3Status
E3RationalPoint4D_Read(TQ3RationalPoint4D *point4D, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32ReadMethod 	float32Read;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Read);

	if(float32Read != NULL){
		result = float32Read(instanceData->format,&point4D->x);
		if(result == kQ3Success)
			result = float32Read(instanceData->format,&point4D->y);
		if(result == kQ3Success)
			result = float32Read(instanceData->format,&point4D->z);
		if(result == kQ3Success)
			result = float32Read(instanceData->format,&point4D->w);
		}

	return(result);
}





//=============================================================================
//      E3RationalPoint4D_Write : Write a TQ3RationalPoint4D.
//-----------------------------------------------------------------------------
TQ3Status
E3RationalPoint4D_Write(const TQ3RationalPoint4D *point4D, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32WriteMethod 	float32Write;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL){
		result = float32Write(instanceData->format,point4D->x);
		if(result == kQ3Success)
			result = float32Write(instanceData->format,point4D->y);
		if(result == kQ3Success)
			result = float32Write(instanceData->format,point4D->z);
		if(result == kQ3Success)
			result = float32Write(instanceData->format,point4D->w);
		}

	return(result);
}





//=============================================================================
//      E3Vector2D_Read : Read a TQ3Vector2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Vector2D_Read(TQ3Vector2D *vector2D, TQ3FileObject theFile)
{
	return Q3Point2D_Read ((TQ3Point2D*)vector2D, theFile);
}





//=============================================================================
//      E3Vector2D_Write : Write a TQ3Vector2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Vector2D_Write(const TQ3Vector2D *vector2D, TQ3FileObject theFile)
{
	return Q3Point2D_Write ((const TQ3Point2D*)vector2D, theFile);
}





//=============================================================================
//      E3Vector3D_Read : Read a TQ3Vector3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Vector3D_Read(TQ3Vector3D *vector3D, TQ3FileObject theFile)
{
	return Q3Point3D_Read ((TQ3Point3D*)vector3D, theFile);
}





//=============================================================================
//      E3Vector3D_Write : Write a TQ3Vector3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Vector3D_Write(const TQ3Vector3D *vector3D, TQ3FileObject theFile)
{
	return Q3Point3D_Write ((const TQ3Point3D*)vector3D, theFile);
}





//=============================================================================
//      E3Matrix4x4_Read : Read a TQ3Matrix4x4.
//-----------------------------------------------------------------------------
TQ3Status
E3Matrix4x4_Read(TQ3Matrix4x4 *matrix4x4, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32ReadMethod 	float32Read;
	TQ3Uns32						i,j;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Read);

	if(float32Read != NULL){
		result = kQ3Success;
		
		for( i = 0; ((i< 4) && (result == kQ3Success)); i++)
			for( j = 0; ((j< 4) && (result == kQ3Success)); j++)
				result = float32Read(instanceData->format,&matrix4x4->value[i][j]);
		}

	return(result);
}





//=============================================================================
//      E3Matrix4x4_Write : Write a TQ3Matrix4x4.
//-----------------------------------------------------------------------------
TQ3Status
E3Matrix4x4_Write(const TQ3Matrix4x4 *matrix4x4, TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32WriteMethod 	float32Write;
	TQ3Uns32						i,j;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod)
					E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL){
		result = kQ3Success;
		
		for( i = 0; ((i< 4) && (result == kQ3Success)); i++)
			for( j = 0; ((j< 4) && (result == kQ3Success)); j++)
				result = float32Write(instanceData->format,matrix4x4->value[i][j]);
		}

	return(result);
}





//=============================================================================
//      E3Tangent2D_Read : Read a TQ3Tangent2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Tangent2D_Read(TQ3Tangent2D *tangent2D, TQ3FileObject theFile)
{
	TQ3Status result = kQ3Failure;
	
	result = Q3Vector3D_Read (&tangent2D->uTangent, theFile);
	if(result == kQ3Success)
		result = Q3Vector3D_Read (&tangent2D->vTangent, theFile);

	return(result);
}





//=============================================================================
//      E3Tangent2D_Write : Write a TQ3Tangent2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Tangent2D_Write(const TQ3Tangent2D *tangent2D, TQ3FileObject theFile)
{
	TQ3Status result = kQ3Failure;
	
	result = Q3Vector3D_Write (&tangent2D->uTangent, theFile);
	if(result == kQ3Success)
		result = Q3Vector3D_Write (&tangent2D->vTangent, theFile);

	return(result);
}





//=============================================================================
//      E3Tangent3D_Read : Read a TQ3Tangent3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Tangent3D_Read(TQ3Tangent3D *tangent3D, TQ3FileObject theFile)
{
	TQ3Status result = kQ3Failure;
	
	result = Q3Vector3D_Read (&tangent3D->uTangent, theFile);
	if(result == kQ3Success)
		result = Q3Vector3D_Read (&tangent3D->vTangent, theFile);
	if(result == kQ3Success)
		result = Q3Vector3D_Read (&tangent3D->wTangent, theFile);

	return(result);
}





//=============================================================================
//      E3Tangent3D_Write : Write a TQ3Tangent3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Tangent3D_Write(const TQ3Tangent3D *tangent3D, TQ3FileObject theFile)
{
	TQ3Status result = kQ3Failure;
	
	result = Q3Vector3D_Write (&tangent3D->uTangent, theFile);
	if(result == kQ3Success)
		result = Q3Vector3D_Write (&tangent3D->vTangent, theFile);
	if(result == kQ3Success)
		result = Q3Vector3D_Write (&tangent3D->wTangent, theFile);

	return(result);
}





//=============================================================================
//      E3Comment_Write : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Comment_Write(char *comment, TQ3FileObject theFile)
{


	// To be implemented...
	return(kQ3Failure);
}






//=============================================================================
//      E3Unknown_RegisterClass : Unknown register.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Unknown_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeShape,
											kQ3ShapeTypeUnknown,
											kQ3ClassNameUnknown,
											NULL,
											sizeof(TQ3Boolean));
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeUnknown,
											kQ3UnknownTypeBinary,
											kQ3ClassNameUnknownBinary,
											e3unknown_binary_metahandler,
											sizeof(TE3FileData));
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeUnknown,
											kQ3UnknownTypeText,
											kQ3ClassNameUnknownText,
											e3unknown_text_metahandler,
											sizeof(TE3FileData));
	return(qd3dStatus);

}





//=============================================================================
//      E3Unknown_UnregisterClass : Unknown unregister.
//-----------------------------------------------------------------------------
TQ3Status
E3Unknown_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3UnknownTypeBinary, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3UnknownTypeText, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShapeTypeUnknown, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Unknown_GetType : Get the type of an unknown object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Unknown_GetType(TQ3UnknownObject unknownObject)
{


	// Return the type
	return(E3ClassTree_GetObjectType(unknownObject, kQ3ShapeTypeUnknown));
}





//=============================================================================
//      E3Unknown_GetDirtyState : Get the dirty state of an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3Unknown_GetDirtyState(TQ3UnknownObject unknownObject, TQ3Boolean *isDirty)
{
	TQ3Boolean	*parentData;
	TQ3Object	parent = E3ClassTree_FindParentInstance (unknownObject, kQ3ShapeTypeUnknown);
	
	if(parent == NULL)
		return(kQ3Failure);
	
	parentData = (TQ3Boolean*)parent->instanceData;
	
	*isDirty = *parentData;

	return(kQ3Success);
}





//=============================================================================
//      E3Unknown_SetDirtyState : Set the dirty state of an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3Unknown_SetDirtyState(TQ3UnknownObject unknownObject, TQ3Boolean isDirty)
{
	TQ3Boolean	*parentData;
	TQ3Object	parent = E3ClassTree_FindParentInstance (unknownObject, kQ3ShapeTypeUnknown);
	
	if(parent == NULL)
		return(kQ3Failure);
	
	parentData = (TQ3Boolean*)parent->instanceData;
	
	*parentData = isDirty;

	return(kQ3Success);
}





//=============================================================================
//      E3UnknownText_New : Create a Unknown Text object.
//-----------------------------------------------------------------------------
TQ3UnknownObject
E3UnknownText_New(TQ3UnknownTextData* data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3UnknownTypeText, kQ3False, data);
	return(theObject);
}





//=============================================================================
//      E3UnknownText_GetData : Get the data for an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownText_GetData(TQ3UnknownObject unknownObject, TQ3UnknownTextData *unknownTextData)
{
	TQ3UnknownTextData		*instanceData = (TQ3UnknownTextData *) unknownObject->instanceData;

	return e3unknown_text_duplicateData (instanceData, unknownTextData);
}





//=============================================================================
//      E3UnknownText_EmptyData : Release the data for an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownText_EmptyData(TQ3UnknownTextData *unknownTextData)
{
	E3Memory_Free(&unknownTextData->objectName);
	E3Memory_Free(&unknownTextData->contents);
	
	return (kQ3Success);
}





//=============================================================================
//      E3UnknownBinary_New : Create a Unknown Binary object.
//-----------------------------------------------------------------------------
TQ3UnknownObject
E3UnknownBinary_New(TQ3UnknownBinaryData* data,const char *typeString)
{	TQ3Object		theObject;

	TE3UnknownBinary_Data	unknownBinaryData;
	
	unknownBinaryData.typeString = (char*)typeString;
	unknownBinaryData.data = *data;

	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3UnknownTypeBinary, kQ3False, &unknownBinaryData);
	return(theObject);
}





//=============================================================================
//      E3UnknownBinary_GetData : Get the data for an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownBinary_GetData(TQ3UnknownObject unknownObject, TQ3UnknownBinaryData *unknownBinaryData)
{
	TE3UnknownBinary_Data		*instanceData = (TE3UnknownBinary_Data *) unknownObject->instanceData;

	return e3unknown_binary_duplicateData (&instanceData->data, unknownBinaryData);
}





//=============================================================================
//      E3UnknownBinary_EmptyData : Release the data for an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownBinary_EmptyData(TQ3UnknownBinaryData *unknownBinaryData)
{
	E3Memory_Free(&unknownBinaryData->contents);
	
	return (kQ3Success);
}





//=============================================================================
//      E3UnknownBinary_GetTypeString : Get the type string of unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownBinary_GetTypeString(TQ3UnknownObject unknownObject, char **typeString)
{
	TE3UnknownBinary_Data		*instanceData = (TE3UnknownBinary_Data *) unknownObject->instanceData;

	if (instanceData->typeString != NULL)
		{
		*typeString = (char *) E3Memory_Allocate(strlen(instanceData->typeString) + 1);
		if (*typeString == NULL)
			return(kQ3Failure);
		strcpy(*typeString, instanceData->typeString);
		}
	else
		**typeString = 0;

	return(kQ3Success);
}





//=============================================================================
//      E3UnknownBinary_EmptyTypeString : Release a type string.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownBinary_EmptyTypeString(char **typeString)
{
	E3Memory_Free(typeString);
	
	return (kQ3Success);
}





//=============================================================================
//      E3ViewHints_New : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ViewHintsObject
E3ViewHints_New(TQ3ViewObject view)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3ViewHints_SetRenderer : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_SetRenderer(TQ3ViewHintsObject viewHints, TQ3RendererObject renderer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_GetRenderer : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_GetRenderer(TQ3ViewHintsObject viewHints, TQ3RendererObject *renderer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_SetCamera : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_SetCamera(TQ3ViewHintsObject viewHints, TQ3CameraObject camera)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_GetCamera : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_GetCamera(TQ3ViewHintsObject viewHints, TQ3CameraObject *camera)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_SetLightGroup : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_SetLightGroup(TQ3ViewHintsObject viewHints, TQ3GroupObject lightGroup)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_GetLightGroup : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_GetLightGroup(TQ3ViewHintsObject viewHints, TQ3GroupObject *lightGroup)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_SetAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_SetAttributeSet(TQ3ViewHintsObject viewHints, TQ3AttributeSet attributeSet)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_GetAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_GetAttributeSet(TQ3ViewHintsObject viewHints, TQ3AttributeSet *attributeSet)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_SetDimensionsState : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_SetDimensionsState(TQ3ViewHintsObject viewHints, TQ3Boolean isValid)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_GetDimensionsState : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_GetDimensionsState(TQ3ViewHintsObject viewHints, TQ3Boolean *isValid)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_SetDimensions : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_SetDimensions(TQ3ViewHintsObject viewHints, TQ3Uns32 width, TQ3Uns32 height)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_GetDimensions : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_GetDimensions(TQ3ViewHintsObject viewHints, TQ3Uns32 *width, TQ3Uns32 *height)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_SetMaskState : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_SetMaskState(TQ3ViewHintsObject viewHints, TQ3Boolean isValid)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_GetMaskState : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_GetMaskState(TQ3ViewHintsObject viewHints, TQ3Boolean *isValid)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_SetMask : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_SetMask(TQ3ViewHintsObject viewHints, const TQ3Bitmap *mask)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_GetMask : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_GetMask(TQ3ViewHintsObject viewHints, TQ3Bitmap *mask)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_SetClearImageMethod : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_SetClearImageMethod(TQ3ViewHintsObject viewHints, TQ3DrawContextClearImageMethod clearMethod)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_GetClearImageMethod : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_GetClearImageMethod(TQ3ViewHintsObject viewHints, TQ3DrawContextClearImageMethod *clearMethod)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_SetClearImageColor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_SetClearImageColor(TQ3ViewHintsObject viewHints, const TQ3ColorARGB *color)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ViewHints_GetClearImageColor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_GetClearImageColor(TQ3ViewHintsObject viewHints, TQ3ColorARGB *color)
{


	// To be implemented...
	return(kQ3Failure);
}






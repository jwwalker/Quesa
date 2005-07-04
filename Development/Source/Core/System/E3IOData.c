/*  NAME:
        E3IO.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3IOData.h"
#include "E3IO.h"
#include "E3Main.h"
#include "E3View.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------



class E3Unknown : public E3Shape // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
	{
Q3_CLASS_ENUMS ( kQ3ShapeTypeUnknown, E3Unknown, E3Shape )
public :

	TQ3Boolean					dirtyFlag ;
	} ;
	


class E3UnknownBinary : public E3Unknown  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3UnknownTypeBinary, E3UnknownBinary, E3Unknown )
public :

	TE3UnknownBinary_Data		instanceData ;
	} ;
	


class E3UnknownText : public E3Unknown  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3UnknownTypeText, E3UnknownText, E3Unknown )
public :

	TQ3UnknownTextData			instanceData ;
	} ;
	


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
	Q3Memory_Free(&instanceData->typeString);
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
		toData->contents = (char *) Q3Memory_Allocate(fromData->size);
		if (toData->contents != NULL)
			Q3Memory_Copy(fromData->contents, toData->contents,fromData->size);
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
		toInstanceData->typeString = (char *) Q3Memory_Allocate(strlen(fromInstanceData->typeString) + 1);
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
		toData->objectName = (char *) Q3Memory_Allocate(strlen(fromData->objectName) + 1);
	if (toData->objectName == NULL)
		return(kQ3Failure);
	strcpy(toData->objectName, fromData->objectName);	
	
	// Copy the contents
	if (fromData->contents != 0)
		toData->contents = (char *) Q3Memory_Allocate(strlen(fromData->contents) + 1);
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
//      e3float32_validate :	Check a floating-point number we just read.
//								If it is infinite, post a warning and replace it
//								by 1.0.
//-----------------------------------------------------------------------------
static void
e3float32_validate( TQ3Float32* theFloat )
{
	if (!isfinite( *theFloat ))
	{
		E3ErrorManager_PostWarning( kQ3WarningReadInfiniteFloatingPointNumber );
		*theFloat = 1.0f;
	}
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
E3NewLine_Write(E3File* theFile)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Uns8_Read : Read a TQ3Uns8.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Uns8_Read(TQ3Uns8 *data, E3File* theFile)
{
	return E3Int8_Read ((TQ3Int8*)data, theFile);
}





//=============================================================================
//      E3Uns8_ReadArray : Read an array of TQ3Uns8.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns8_ReadArray(TQ3Uns32 numNums, TQ3Uns8 *data, E3File* theFile)
{
	TQ3FileFormatObject				format = theFile->GetFileFormat () ;
	TQ3XFFormatInt8ReadMethod 		int8Read;
	TQ3XFFormatInt8ReadArrayMethod	int8ArrayRead;
	TQ3Uns32						n;
	TQ3Status						status = kQ3Success;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((format != NULL),kQ3Failure);

	
	int8ArrayRead = (TQ3XFFormatInt8ReadArrayMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt8ReadArray);

	if (int8ArrayRead == NULL)
	{
		int8Read = (TQ3XFFormatInt8ReadMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt8Read);
		Q3_REQUIRE_OR_RESULT((int8Read != NULL),kQ3Failure);
		
		
		for (n = 0; n < numNums; ++n)
		{
			status = int8Read( format, (TQ3Int8*)&data[n] );
			if (status == kQ3Failure)
			{
				break;
			}
		}
	}
	else
	{
		status = int8ArrayRead( format, numNums, (TQ3Int8*)data );
	}

	
	return status;
}





//=============================================================================
//      E3Uns8_Write : Write a TQ3Uns8.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns8_Write(TQ3Uns8 data, E3File* theFile)
{
	return E3Int8_Write ((TQ3Int8)data, theFile);
}





//=============================================================================
//      E3Uns16_Read : Read a TQ3Uns16.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns16_Read(TQ3Uns16 *data, E3File* theFile)
{
	return E3Int16_Read ((TQ3Int16*)data, theFile);
}





//=============================================================================
//      E3Uns16_ReadArray : Read an array of TQ3Uns16.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns16_ReadArray(TQ3Uns32 numNums, TQ3Uns16 *data, E3File* theFile)
{
	TQ3FileFormatObject				format = theFile->GetFileFormat () ;
	TQ3XFFormatInt16ReadMethod 		int16Read;
	TQ3XFFormatInt16ReadArrayMethod	int16ArrayRead;
	TQ3Uns32						n;
	TQ3Status						status = kQ3Success;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((format != NULL),kQ3Failure);

	
	int16ArrayRead = (TQ3XFFormatInt16ReadArrayMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt16ReadArray);

	if (int16ArrayRead == NULL)
	{
		int16Read = (TQ3XFFormatInt16ReadMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt16Read);
		Q3_REQUIRE_OR_RESULT((int16Read != NULL),kQ3Failure);
		
		
		for (n = 0; n < numNums; ++n)
		{
			status = int16Read( format, (TQ3Int16*)&data[n] );
			if (status == kQ3Failure)
			{
				break;
			}
		}
	}
	else
	{
		status = int16ArrayRead( format, numNums, (TQ3Int16*)data );
	}

	
	return status;
}





//=============================================================================
//      E3Uns16_Write : Write a TQ3Uns16.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns16_Write(TQ3Uns16 data, E3File* theFile)
{
	return E3Int16_Write ((TQ3Int16)data, theFile);
}





//=============================================================================
//      E3Uns32_Read : Read a TQ3Uns32.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns32_Read(TQ3Uns32 *data, E3File* theFile)
{
	return E3Int32_Read ((TQ3Int32*)data, theFile);
}





//=============================================================================
//      E3Uns32_Write : Write a TQ3Uns32.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns32_Write(TQ3Uns32 data, E3File* theFile)
{
	return E3Int32_Write ((TQ3Int32)data, theFile);
}





//=============================================================================
//      E3Int8_Read : Read a TQ3Int8.
//-----------------------------------------------------------------------------
TQ3Status
E3Int8_Read(TQ3Int8 *data, E3File* theFile)
{
	TQ3XFFormatInt8ReadMethod 		int8Read;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	int8Read = (TQ3XFFormatInt8ReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatInt8Read);

	if (int8Read != NULL)
		return int8Read( theFile->GetFileFormat (),data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Int8_Write : Write a TQ3Int8.
//-----------------------------------------------------------------------------
TQ3Status
E3Int8_Write(TQ3Int8 data, E3File* theFile)
{
	TQ3XFFormatInt8WriteMethod 		int8Write;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	int8Write = (TQ3XFFormatInt8WriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatInt8Write);

	if (int8Write != NULL)
		return int8Write( theFile->GetFileFormat (),&data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Int16_Read : Read a TQ3Int16.
//-----------------------------------------------------------------------------
TQ3Status
E3Int16_Read(TQ3Int16 *data, E3File* theFile)
{
	TQ3XFFormatInt16ReadMethod 		int16Read;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	int16Read = (TQ3XFFormatInt16ReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatInt16Read);

	if(int16Read != NULL)
		return int16Read( theFile->GetFileFormat (),data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Int16_Write : Write a TQ3Int16.
//-----------------------------------------------------------------------------
TQ3Status
E3Int16_Write(TQ3Int16 data, E3File* theFile)
{
	TQ3XFFormatInt16WriteMethod 		int16Write;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	int16Write = (TQ3XFFormatInt16WriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatInt16Write);

	if(int16Write != NULL)
		return int16Write( theFile->GetFileFormat (),&data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Int32_Read : Read a TQ3Int32.
//-----------------------------------------------------------------------------
TQ3Status
E3Int32_Read(TQ3Int32 *data, E3File* theFile)
{
	TQ3XFFormatInt32ReadMethod 		int32Read;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	int32Read = (TQ3XFFormatInt32ReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatInt32Read);

	if(int32Read != NULL)
		return int32Read( theFile->GetFileFormat (),data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Uns32_ReadArray : Read an array of TQ3Uns32.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns32_ReadArray(TQ3Uns32 numNums, TQ3Uns32 *data, E3File* theFile)
{
	TQ3FileFormatObject				format = theFile->GetFileFormat () ;
	TQ3XFFormatInt32ReadMethod 		int32Read;
	TQ3XFFormatInt32ReadArrayMethod	int32ArrayRead;
	TQ3Uns32						n;
	TQ3Status						status = kQ3Success;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((format != NULL),kQ3Failure);

	
	int32ArrayRead = (TQ3XFFormatInt32ReadArrayMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt32ReadArray);

	if (int32ArrayRead == NULL)
	{
		int32Read = (TQ3XFFormatInt32ReadMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt32Read);
		Q3_REQUIRE_OR_RESULT((int32Read != NULL),kQ3Failure);
		
		
		for (n = 0; n < numNums; ++n)
		{
			status = int32Read( format, (TQ3Int32*)&data[n] );
			if (status == kQ3Failure)
			{
				break;
			}
		}
	}
	else
	{
		status = int32ArrayRead( format, numNums, (TQ3Int32*)data );
	}

	
	return status;
}





//=============================================================================
//      E3Int32_Write : Write a TQ3Int32.
//-----------------------------------------------------------------------------
TQ3Status
E3Int32_Write(TQ3Int32 data, E3File* theFile)
{
	TQ3XFFormatInt32WriteMethod 		int32Write;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	int32Write = (TQ3XFFormatInt32WriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatInt32Write);

	if(int32Write != NULL)
		return int32Write( theFile->GetFileFormat (),&data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Uns64_Read : Read a TQ3Uns64.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns64_Read(TQ3Uns64 *data, E3File* theFile)
{
	return E3Int64_Read ((TQ3Int64*)data, theFile);
}





//=============================================================================
//      E3Uns64_Write : Write a TQ3Uns64.
//-----------------------------------------------------------------------------
TQ3Status
E3Uns64_Write(TQ3Uns64 data, E3File* theFile)
{
	return E3Int64_Write (*((TQ3Int64*)&data), theFile);
}





//=============================================================================
//      E3Int64_Read : Read a TQ3Int64.
//-----------------------------------------------------------------------------
TQ3Status
E3Int64_Read(TQ3Int64 *data, E3File* theFile)
{
	TQ3XFFormatInt64ReadMethod 		int64Read;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	int64Read = (TQ3XFFormatInt64ReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatInt64Read);

	if(int64Read != NULL)
		return int64Read( theFile->GetFileFormat (),data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Int64_Write : Write a TQ3Int64.
//-----------------------------------------------------------------------------
TQ3Status
E3Int64_Write(TQ3Int64 data, E3File* theFile)
{
	TQ3XFFormatInt64WriteMethod 		int64Write;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	int64Write = (TQ3XFFormatInt64WriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatInt64Write);

	if(int64Write != NULL)
		return int64Write( theFile->GetFileFormat (),&data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Float32_Read : Read a TQ3Float32.
//-----------------------------------------------------------------------------
TQ3Status
E3Float32_Read(TQ3Float32 *data, E3File* theFile)
{
	TQ3XFFormatFloat32ReadMethod 	float32Read;
	TQ3Status	status = kQ3Failure;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Read);

	if (float32Read != NULL)
	{
		status = float32Read( theFile->GetFileFormat (), data );
		if (status == kQ3Success)
		{
			e3float32_validate( data );
		}
	}

	return status;
}





//=============================================================================
//      E3Float32_ReadArray : Read an array of TQ3Float32.
//-----------------------------------------------------------------------------
TQ3Status
E3Float32_ReadArray( TQ3Uns32 numFloats, TQ3Float32* theFloats, E3File* theFile)
{
	TQ3FileFormatObject				format = theFile->GetFileFormat () ;
	TQ3XFFormatFloat32ReadMethod 	float32Read;
	TQ3XFFormatFloat32ReadArrayMethod	floatArrayRead;
	TQ3Uns32						n;
	TQ3Status						status = kQ3Success;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((format != NULL),kQ3Failure);

	
	floatArrayRead = (TQ3XFFormatFloat32ReadArrayMethod) format->GetMethod ( kQ3XMethodTypeFFormatFloat32ReadArray);

	if (floatArrayRead == NULL)
	{
		float32Read = (TQ3XFFormatFloat32ReadMethod) format->GetMethod ( kQ3XMethodTypeFFormatFloat32Read);
		Q3_REQUIRE_OR_RESULT((float32Read != NULL),kQ3Failure);
		
		
		for (n = 0; n < numFloats; ++n)
		{
			status = float32Read( format, &theFloats[n] );
			if (status == kQ3Success)
				e3float32_validate( &theFloats[n] );
			else
				break;
		}
	}
	else
	{
		status = floatArrayRead( format, numFloats, theFloats );
		
		if (status == kQ3Success)
		{
			for (n = 0; n < numFloats; ++n)
			{
				e3float32_validate( &theFloats[n] );
			}
		}
	}

	
	return status;
}





//=============================================================================
//      E3Float32_Write : Write a TQ3Float32.
//-----------------------------------------------------------------------------
TQ3Status
E3Float32_Write(TQ3Float32 data, E3File* theFile)
{
	TQ3XFFormatFloat32WriteMethod 	float32Write;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL)
		return float32Write( theFile->GetFileFormat (),&data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Float64_Read : Read a TQ3Float64.
//-----------------------------------------------------------------------------
TQ3Status
E3Float64_Read(TQ3Float64 *data, E3File* theFile)
{
	TQ3XFFormatFloat64ReadMethod 	float64Read;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float64Read = (TQ3XFFormatFloat64ReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat64Read);

	if(float64Read != NULL)
		return float64Read( theFile->GetFileFormat (),data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Float64_Write : Write a TQ3Float64.
//-----------------------------------------------------------------------------
TQ3Status
E3Float64_Write(TQ3Float64 data, E3File* theFile)
{
	TQ3XFFormatFloat64WriteMethod 	float64Write;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float64Write = (TQ3XFFormatFloat64WriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat64Write);

	if(float64Write != NULL)
		return float64Write( theFile->GetFileFormat (),&data);

	return(kQ3Failure);
}





//=============================================================================
//      E3Size_Pad : Pad a number of bytes to be long-word aligned.
//-----------------------------------------------------------------------------
TQ3Size
E3Size_Pad(TQ3Size size)
{
	
	if ((size & 0x03) != 0)
		size = (size & 0xFFFFFFFC) + 4;
		
	return size;
}





//=============================================================================
//      E3String_ReadUnlimited : Read a NUL-termined string.
//-----------------------------------------------------------------------------
TQ3Status
E3String_ReadUnlimited(char *data, TQ3Uns32 *ioLength, E3File* theFile)
{
	TQ3Status		status;
	TQ3XFFormatStringReadMethod 	stringRead;
	TQ3Uns32		bufferSize;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);
	
	stringRead = (TQ3XFFormatStringReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatStringRead);

	if (stringRead != NULL)
	{
		bufferSize = *ioLength;
		
		status = stringRead( theFile->GetFileFormat (), data, ioLength );
		
		if ( (status == kQ3Success) && (data != NULL) &&
			(*ioLength >= bufferSize) )
		{
			E3ErrorManager_PostWarning( kQ3WarningStringExceedsMaximumLength );
		}
	}

	return (status);
}





//=============================================================================
//      E3String_Read : Read a string.
//-----------------------------------------------------------------------------
TQ3Status
E3String_Read(char *data, TQ3Uns32 *length, E3File* theFile)
{
	TQ3Status	status;
	
	// The length parameter of Q3String_Read is output only.  The buffer is assumed
	// to have size kQ3StringMaximumLength.
	*length = kQ3StringMaximumLength;


	status = E3String_ReadUnlimited( data, length, theFile );
	
	
	return status;
}





//=============================================================================
//      E3String_WriteUnlimited : Write a NUL-terminated string.
//-----------------------------------------------------------------------------
TQ3Status
E3String_WriteUnlimited(const char *data, E3File* theFile)
{
	TQ3XFFormatStringWriteMethod 	stringWrite;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	stringWrite = (TQ3XFFormatStringWriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatStringWrite);
	Q3_REQUIRE_OR_RESULT( stringWrite != NULL, kQ3Failure );
	
	return stringWrite( theFile->GetFileFormat (), data );
}





//=============================================================================
//      E3String_Write : Write a NUL-terminated string, limited length.
//-----------------------------------------------------------------------------
TQ3Status
E3String_Write(const char *data, E3File* theFile)
{
	TQ3Uns32		theLength;
	TQ3Status		status;
	char			strCopy[ kQ3StringMaximumLength ];

	
	theLength = strlen( data );
	
	if (theLength >= kQ3StringMaximumLength)
	{
		E3ErrorManager_PostError( kQ3ErrorStringExceedsMaximumLength, kQ3False );
		
		// In this situation, QD3D writes kQ3StringMaximumLength bytes.  But since the
		// data parameter is const, we must make a copy.
		Q3Memory_Copy(data, strCopy, kQ3StringMaximumLength - 1 );
		strCopy[ kQ3StringMaximumLength - 1 ] = '\0';
		
		status = E3String_WriteUnlimited( strCopy, theFile );
	}
	else
	{
		status = E3String_WriteUnlimited( data, theFile );
	}
	
	return status;
}





//=============================================================================
//      E3RawData_Read : Read a block of raw data.
//-----------------------------------------------------------------------------
TQ3Status
E3RawData_Read(unsigned char *data, TQ3Uns32 size, E3File* theFile)
{
	TQ3XFFormatRawReadMethod	 	rawRead;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	rawRead = (TQ3XFFormatRawReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatRawRead);

	if(rawRead != NULL)
		return rawRead( theFile->GetFileFormat (),data,size);

	return(kQ3Failure);
}





//=============================================================================
//      E3RawData_Write : Write a block of raw data.
//-----------------------------------------------------------------------------
TQ3Status
E3RawData_Write(const unsigned char *data, TQ3Uns32 size, E3File* theFile)
{
	TQ3XFFormatRawWriteMethod	 	rawWrite;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	rawWrite = (TQ3XFFormatRawWriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatRawWrite);

	if(rawWrite != NULL)
		return rawWrite( theFile->GetFileFormat (),data,size);

	return(kQ3Failure);
}





//=============================================================================
//      E3Point2D_Read : Read a TQ3Point2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Point2D_Read(TQ3Point2D *point2D, E3File* theFile)
{
	TQ3Status						result        = kQ3Failure;
	TQ3XFFormatFloat32ReadMethod 	float32Read;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Read);

	if(float32Read != NULL){
		result = float32Read( theFile->GetFileFormat (),&point2D->x);
		if(result == kQ3Success)
			result = float32Read( theFile->GetFileFormat (),&point2D->y);
		}
		
	if (result == kQ3Success)
		{
		e3float32_validate( &point2D->x );
		e3float32_validate( &point2D->y );
		}

	return(result);
}





//=============================================================================
//      E3Point2D_Write : Write a TQ3Point2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Point2D_Write(const TQ3Point2D *point2D, E3File* theFile)
{
	TQ3Status						result        = kQ3Failure;
	TQ3XFFormatFloat32WriteMethod 	float32Write;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL){
		result = float32Write( theFile->GetFileFormat (),&point2D->x);
		if(result == kQ3Success)
			result = float32Write( theFile->GetFileFormat (),&point2D->y);
		}

	return(result);
}





//=============================================================================
//      E3Point3D_Read : Read a TQ3Point3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Point3D_Read(TQ3Point3D *point3D, E3File* theFile)
{
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32ReadMethod 	float32Read;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Read);

	if(float32Read != NULL){
		result = float32Read( theFile->GetFileFormat (),&point3D->x);
		if(result == kQ3Success)
			result = float32Read( theFile->GetFileFormat (),&point3D->y);
		if(result == kQ3Success)
			result = float32Read( theFile->GetFileFormat (),&point3D->z);
			
		if (result == kQ3Success)
			{
			e3float32_validate( &point3D->x );
			e3float32_validate( &point3D->y );
			e3float32_validate( &point3D->z );
			}
		}
		

	return(result);
}





//=============================================================================
//      E3Point3D_Write : Write a TQ3Point3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Point3D_Write(const TQ3Point3D *point3D, E3File* theFile)
{
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32WriteMethod 	float32Write;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL){
		result = float32Write( theFile->GetFileFormat (),&point3D->x);
		if(result == kQ3Success)
			result = float32Write( theFile->GetFileFormat (),&point3D->y);
		if(result == kQ3Success)
			result = float32Write( theFile->GetFileFormat (),&point3D->z);
		}

	return(result);
}





//=============================================================================
//      E3RationalPoint3D_Read : Read a TQ3RationalPoint3D.
//-----------------------------------------------------------------------------
TQ3Status
E3RationalPoint3D_Read(TQ3RationalPoint3D *point3D, E3File* theFile)
{
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32ReadMethod 	float32Read;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Read);

	if(float32Read != NULL){
		result = float32Read( theFile->GetFileFormat (),&point3D->x);
		if(result == kQ3Success)
			result = float32Read( theFile->GetFileFormat (),&point3D->y);
		if(result == kQ3Success)
			result = float32Read( theFile->GetFileFormat (),&point3D->w);

		if (result == kQ3Success)
			{
			e3float32_validate( &point3D->x );
			e3float32_validate( &point3D->y );
			e3float32_validate( &point3D->w );
			}
		}

	return(result);
}





//=============================================================================
//      E3RationalPoint3D_Write : Write a TQ3RationalPoint3D.
//-----------------------------------------------------------------------------
TQ3Status
E3RationalPoint3D_Write(const TQ3RationalPoint3D *point3D, E3File* theFile)
{
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32WriteMethod 	float32Write;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL){
		result = float32Write( theFile->GetFileFormat (),&point3D->x);
		if(result == kQ3Success)
			result = float32Write( theFile->GetFileFormat (),&point3D->y);
		if(result == kQ3Success)
			result = float32Write( theFile->GetFileFormat (),&point3D->w);
		}

	return(result);
}





//=============================================================================
//      E3RationalPoint4D_Read : Read a TQ3RationalPoint4D.
//-----------------------------------------------------------------------------
TQ3Status
E3RationalPoint4D_Read(TQ3RationalPoint4D *point4D, E3File* theFile)
{
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32ReadMethod 	float32Read;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Read);

	if(float32Read != NULL){
		result = float32Read( theFile->GetFileFormat (),&point4D->x);
		if(result == kQ3Success)
			result = float32Read( theFile->GetFileFormat (),&point4D->y);
		if(result == kQ3Success)
			result = float32Read( theFile->GetFileFormat (),&point4D->z);
		if(result == kQ3Success)
			result = float32Read( theFile->GetFileFormat (),&point4D->w);

		if (result == kQ3Success)
			{
			e3float32_validate( &point4D->x );
			e3float32_validate( &point4D->y );
			e3float32_validate( &point4D->z );
			e3float32_validate( &point4D->w );
			}
		}

	return(result);
}





//=============================================================================
//      E3RationalPoint4D_Write : Write a TQ3RationalPoint4D.
//-----------------------------------------------------------------------------
TQ3Status
E3RationalPoint4D_Write(const TQ3RationalPoint4D *point4D, E3File* theFile)
{
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32WriteMethod 	float32Write;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL){
		result = float32Write( theFile->GetFileFormat (),&point4D->x);
		if(result == kQ3Success)
			result = float32Write( theFile->GetFileFormat (),&point4D->y);
		if(result == kQ3Success)
			result = float32Write( theFile->GetFileFormat (),&point4D->z);
		if(result == kQ3Success)
			result = float32Write( theFile->GetFileFormat (),&point4D->w);
		}

	return(result);
}





//=============================================================================
//      E3Vector2D_Read : Read a TQ3Vector2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Vector2D_Read(TQ3Vector2D *vector2D, E3File* theFile)
{
	return Q3Point2D_Read ((TQ3Point2D*)vector2D, theFile);
}





//=============================================================================
//      E3Vector2D_Write : Write a TQ3Vector2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Vector2D_Write(const TQ3Vector2D *vector2D, E3File* theFile)
{
	return E3Point2D_Write ((const TQ3Point2D*)vector2D, theFile);
}





//=============================================================================
//      E3Vector3D_Read : Read a TQ3Vector3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Vector3D_Read(TQ3Vector3D *vector3D, E3File* theFile)
{
	return E3Point3D_Read ((TQ3Point3D*)vector3D, theFile);
}





//=============================================================================
//      E3Vector3D_Write : Write a TQ3Vector3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Vector3D_Write(const TQ3Vector3D *vector3D, E3File* theFile)
{
	return E3Point3D_Write ((const TQ3Point3D*)vector3D, theFile);
}





//=============================================================================
//      E3Matrix4x4_Read : Read a TQ3Matrix4x4.
//-----------------------------------------------------------------------------
TQ3Status
E3Matrix4x4_Read(TQ3Matrix4x4 *matrix4x4, E3File* theFile)
{
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32ReadMethod 	float32Read;
	TQ3Uns32						i,j;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Read = (TQ3XFFormatFloat32ReadMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Read);

	if(float32Read != NULL){
		result = kQ3Success;
		
		for( i = 0; ((i< 4) && (result == kQ3Success)); i++)
			for( j = 0; ((j< 4) && (result == kQ3Success)); j++)
				{
				result = float32Read( theFile->GetFileFormat (),&matrix4x4->value[i][j]);
				if (result == kQ3Success)
					e3float32_validate( &matrix4x4->value[i][j] );
				}
		}

	return(result);
}





//=============================================================================
//      E3Matrix4x4_Write : Write a TQ3Matrix4x4.
//-----------------------------------------------------------------------------
TQ3Status
E3Matrix4x4_Write(const TQ3Matrix4x4 *matrix4x4, E3File* theFile)
{
	TQ3Status						result = kQ3Failure;
	TQ3XFFormatFloat32WriteMethod 	float32Write;
	TQ3Uns32						i,j;

	Q3_REQUIRE_OR_RESULT(( theFile->GetFileStatus () == kE3_File_Status_Writing),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( theFile->GetFileFormat () != NULL),kQ3Failure);

	float32Write = (TQ3XFFormatFloat32WriteMethod) theFile->GetFileFormat ()->GetMethod ( kQ3XMethodTypeFFormatFloat32Write);

	if(float32Write != NULL){
		result = kQ3Success;
		
		for( i = 0; ((i< 4) && (result == kQ3Success)); i++)
			for( j = 0; ((j< 4) && (result == kQ3Success)); j++)
				result = float32Write( theFile->GetFileFormat (),&matrix4x4->value[i][j]);
		}

	return(result);
}





//=============================================================================
//      E3Tangent2D_Read : Read a TQ3Tangent2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Tangent2D_Read(TQ3Tangent2D *tangent2D, E3File* theFile)
{
	TQ3Status result = kQ3Failure;
	
	result = E3Vector3D_Read (&tangent2D->uTangent, theFile);
	if(result == kQ3Success)
		result = E3Vector3D_Read (&tangent2D->vTangent, theFile);

	return(result);
}





//=============================================================================
//      E3Tangent2D_Write : Write a TQ3Tangent2D.
//-----------------------------------------------------------------------------
TQ3Status
E3Tangent2D_Write(const TQ3Tangent2D *tangent2D, E3File* theFile)
{
	TQ3Status result = kQ3Failure;
	
	result = E3Vector3D_Write (&tangent2D->uTangent, theFile);
	if(result == kQ3Success)
		result = E3Vector3D_Write (&tangent2D->vTangent, theFile);

	return(result);
}





//=============================================================================
//      E3Tangent3D_Read : Read a TQ3Tangent3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Tangent3D_Read(TQ3Tangent3D *tangent3D, E3File* theFile)
{
	TQ3Status result = kQ3Failure;
	
	result = E3Vector3D_Read (&tangent3D->uTangent, theFile);
	if(result == kQ3Success)
		result = E3Vector3D_Read (&tangent3D->vTangent, theFile);
	if(result == kQ3Success)
		result = E3Vector3D_Read (&tangent3D->wTangent, theFile);

	return(result);
}





//=============================================================================
//      E3Tangent3D_Write : Write a TQ3Tangent3D.
//-----------------------------------------------------------------------------
TQ3Status
E3Tangent3D_Write(const TQ3Tangent3D *tangent3D, E3File* theFile)
{
	TQ3Status result = kQ3Failure;
	
	result = E3Vector3D_Write (&tangent3D->uTangent, theFile);
	if(result == kQ3Success)
		result = E3Vector3D_Write (&tangent3D->vTangent, theFile);
	if(result == kQ3Success)
		result = E3Vector3D_Write (&tangent3D->wTangent, theFile);

	return(result);
}





//=============================================================================
//      E3Comment_Write : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Comment_Write(char *comment, E3File* theFile)
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
	qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameUnknown,
										NULL,
										E3Unknown ) ;
	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameUnknownBinary,
											e3unknown_binary_metahandler,
											E3UnknownBinary ) ;
	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameUnknownText,
											e3unknown_text_metahandler,
											E3UnknownText ) ;
	return(qd3dStatus);

}





//=============================================================================
//      E3Unknown_UnregisterClass : Unknown unregister.
//-----------------------------------------------------------------------------
TQ3Status
E3Unknown_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3UnknownTypeBinary, kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3UnknownTypeText, kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ShapeTypeUnknown, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Unknown_GetType : Get the type of an unknown object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Unknown_GetType(TQ3UnknownObject unknownObject)
	{
	// Return the type
	return unknownObject->GetObjectType ( kQ3ShapeTypeUnknown ) ;
	}





//=============================================================================
//      E3Unknown_GetDirtyState : Get the dirty state of an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3Unknown_GetDirtyState(TQ3UnknownObject unknownObject, TQ3Boolean *isDirty)
	{
	*isDirty = ( (E3Unknown*) unknownObject )->dirtyFlag ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Unknown_SetDirtyState : Set the dirty state of an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3Unknown_SetDirtyState(TQ3UnknownObject unknownObject, TQ3Boolean isDirty)
	{
	( (E3Unknown*) unknownObject )->dirtyFlag = isDirty ;

	return kQ3Success ;
	}





//=============================================================================
//      E3UnknownText_New : Create a Unknown Text object.
//-----------------------------------------------------------------------------
TQ3UnknownObject
E3UnknownText_New(TQ3UnknownTextData* data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3UnknownTypeText, kQ3False, data);
	return(theObject);
}





//=============================================================================
//      E3UnknownText_GetData : Get the data for an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownText_GetData(TQ3UnknownObject unknownObject, TQ3UnknownTextData *unknownTextData)
	{
	return e3unknown_text_duplicateData ( & ( (E3UnknownText*) unknownObject )->instanceData, unknownTextData ) ;
	}





//=============================================================================
//      E3UnknownText_EmptyData : Release the data for an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownText_EmptyData(TQ3UnknownTextData *unknownTextData)
{
	Q3Memory_Free(&unknownTextData->objectName);
	Q3Memory_Free(&unknownTextData->contents);
	
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
	theObject = E3ClassTree::CreateInstance ( kQ3UnknownTypeBinary, kQ3False, &unknownBinaryData);
	return(theObject);
}





//=============================================================================
//      E3UnknownBinary_GetData : Get the data for an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownBinary_GetData(TQ3UnknownObject unknownObject, TQ3UnknownBinaryData *unknownBinaryData)
	{
	return e3unknown_binary_duplicateData ( & ( (E3UnknownBinary*) unknownObject )->instanceData.data, unknownBinaryData ) ;
	}





//=============================================================================
//      E3UnknownBinary_EmptyData : Release the data for an unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownBinary_EmptyData(TQ3UnknownBinaryData *unknownBinaryData)
{
	Q3Memory_Free(&unknownBinaryData->contents);
	
	return (kQ3Success);
}





//=============================================================================
//      E3UnknownBinary_GetTypeString : Get the type string of unknown object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownBinary_GetTypeString(TQ3UnknownObject unknownObject, char **typeString)
	{
	E3UnknownBinary* unknownBinary = (E3UnknownBinary*) unknownObject ;
	
	if ( unknownBinary->instanceData.typeString != NULL)
		{
		*typeString = (char *) Q3Memory_Allocate ( strlen ( unknownBinary->instanceData.typeString ) + 1 ) ;
		if ( *typeString == NULL )
			return kQ3Failure ;
		strcpy ( * typeString, unknownBinary->instanceData.typeString ) ;
		}
	else
		**typeString = 0 ;

	return kQ3Success ;
	}





//=============================================================================
//      E3UnknownBinary_EmptyTypeString : Release a type string.
//-----------------------------------------------------------------------------
TQ3Status
E3UnknownBinary_EmptyTypeString(char **typeString)
{
	Q3Memory_Free(typeString);
	
	return (kQ3Success);
}





#pragma mark -
//=============================================================================
//      E3ViewHints
//=============================================================================


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3viewhints_new : View hints new method.
//-----------------------------------------------------------------------------
TQ3Status
e3viewhints_new ( E3ViewHints* theObject, void *privateData, const void *paramData )
	{
	// Initialise our instance data
	
#pragma unused(privateData)
	if ( paramData )
		{
		const TQ3ViewObject theView = (const TQ3ViewObject) paramData ;

		Q3View_GetRenderer ( theView , &theObject->renderer ) ;
		Q3View_GetCamera ( theView , &theObject->camera ) ;
		Q3View_GetLightGroup ( theView , &theObject->lightGroup ) ;
		Q3View_GetAttributeSetState ( theView , &theObject->attributeSet ) ;
		
		TQ3DrawContextObject drawContext ;
		if ( Q3View_GetDrawContext ( theView , &drawContext ) != kQ3Failure )
			{
			Q3DrawContext_GetClearImageMethod ( drawContext , &theObject->clearMethod ) ;
			Q3DrawContext_GetClearImageColor ( drawContext , &theObject->clearImageColor ) ;
			theObject->isValidSetMaskState = ( Q3DrawContext_GetMask ( drawContext , &theObject->mask ) != kQ3Failure ) ? kQ3True : kQ3False ;
			}
		}
	else
		{
		theObject->renderer = Q3Renderer_NewFromType ( kQ3RendererTypeInteractive ) ;
		theObject->camera = NULL ;
		theObject->lightGroup = NULL ;
		theObject->attributeSet = NULL ;
		theObject->clearMethod = kQ3ClearMethodNone  ;
		Q3ColorARGB_Set ( &theObject->clearImageColor, 1.0f, 1.0f, 1.0f, 1.0f ) ;
		theObject->mask.image = NULL ;
		theObject->mask.width = 0 ;
		theObject->mask.height = 0 ;
		theObject->mask.rowBytes = 0 ;
		theObject->mask.bitOrder = kQ3EndianBig ;
		}

	
	theObject->isValidSetDimensions = kQ3False ;
	theObject->widthDimensions = 0 ; // These don't seem to be in the View or the Draw Context, unless they should be max - min.
	theObject->heightDimensions = 0 ;

	return kQ3Success ;
	}





//=============================================================================
//      e3viewhints_delete : View hints delete method.
//-----------------------------------------------------------------------------
void
e3viewhints_delete ( E3ViewHints* theObject, void *privateData )
	{
#pragma unused(privateData)

	// Dispose of our instance data
	if ( theObject->renderer )
		Q3Object_Dispose ( theObject->renderer ) ;
	if ( theObject->camera )
		Q3Object_Dispose ( theObject->camera ) ;
	if ( theObject->attributeSet )
		Q3Object_Dispose ( theObject->attributeSet ) ;
	if ( theObject->lightGroup )
		Q3Object_Dispose ( theObject->lightGroup ) ;
	}




//=============================================================================
//      e3viewhints_read : View hints read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3viewhints_read ( TQ3FileObject theFile )
	{
	TQ3Status result = kQ3Success ;

	// Create the object
	E3ViewHints* theObject = (E3ViewHints*) E3ClassTree::CreateInstance ( kQ3SharedTypeViewHints, kQ3False, NULL ) ;
	
	if ( theObject )
		{
		while ( ! Q3File_IsEndOfContainer ( theFile, NULL ) )
			{
			if ( TQ3Object childObject = Q3File_ReadObject ( theFile ) )
				{
				if ( Q3Object_IsType ( childObject , kQ3SharedTypeRenderer ) )
					theObject->SetRenderer ( childObject ) ;
				else
				if ( Q3Object_IsType ( childObject , kQ3ShapeTypeCamera ) )
					theObject->SetCamera ( childObject ) ;
				else
				if ( Q3Object_IsType ( childObject , kQ3GroupTypeLight ) ) // Allow reading of files with lights in a group
					theObject->SetLightGroup ( childObject ) ;
				else
 				if ( Q3Object_IsType ( childObject , kQ3ShapeTypeLight ) ) // or the lights loose, not in a group
 					{
 					TQ3GroupObject lightGroup ;
 					theObject->GetLightGroup ( &lightGroup ) ;
					if ( lightGroup == NULL )
 						{
 						lightGroup = Q3LightGroup_New () ;
  						theObject->SetLightGroup ( lightGroup ) ;
  						}
					Q3Group_AddObject ( lightGroup , childObject ) ;
					Q3Object_Dispose ( lightGroup ) ;
 					}
 				else
				if ( Q3Object_IsType ( childObject , kQ3SetTypeAttribute ) )
					theObject->SetAttributeSet ( childObject ) ;
				else
				if ( Q3Object_IsType ( childObject , kQ3ImageClearColour ) )
					{
					theObject->SetClearImageMethod ( kQ3ClearMethodWithColor ) ;
					theObject->SetClearImageColor ( (TQ3ColorARGB*) childObject->FindLeafInstanceData () ) ;
					}
				else
				if ( Q3Object_IsType ( childObject , kQ3ImageDimensions ) )
					{		
					theObject->SetDimensionsState ( kQ3True ) ;
					TQ3Uns32* p = (TQ3Uns32*) childObject->FindLeafInstanceData () ;
					theObject->SetDimensions ( p [ 0 ] , p [ 1 ] ) ;
					}
				else
/*
To be written:
				if ( Q3Object_IsType ( childObject , ? ) )
					{
					theObject->SetMaskState ( kQ3True ) ;
					theObject->SetMask ( TQ3Bitmap* mask ) ;
					}
*/				
				Q3Object_Dispose ( childObject ) ; // decrement reference count
				}
			}

		if ( result != kQ3Success )
			{
			Q3Object_Dispose ( theObject ) ;
			return NULL ;
			}
		}
	
	return theObject ;
	}




//=============================================================================
//      e3viewhints_traverse : View Hints traverse method.
//-----------------------------------------------------------------------------

static TQ3Status
e3viewhints_traverse ( E3ViewHints* theObject, void *data, TQ3ViewObject theView )
	{
	TQ3Uns32 result = Q3XView_SubmitWriteData ( theView, 0, NULL, NULL ) ;

	TQ3RendererObject renderer ;
	if ( theObject->GetRenderer ( &renderer ) == kQ3Success )
		{
		result &= Q3Object_Submit ( renderer , theView ) ;
		Q3Object_Dispose ( renderer ) ;
		}
		
	TQ3CameraObject camera ;
	if ( theObject->GetCamera ( &camera ) == kQ3Success )
		{
		result &= Q3Object_Submit ( camera , theView ) ;
		Q3Object_Dispose ( camera ) ;
		}
	
	TQ3GroupObject lightGroup ;
	if ( theObject->GetLightGroup ( &lightGroup ) == kQ3Success )
		{
#if 1
		TQ3GroupPosition pos ;
		for ( Q3Group_GetFirstPosition ( lightGroup, &pos ) ; pos ; Q3Group_GetNextPosition ( lightGroup, &pos ) )
			{
			TQ3Object object ;
			Q3Group_GetPositionObject ( lightGroup, pos, &object ) ;
			result &= Q3Object_Submit ( object, theView ) ;
			Q3Object_Dispose ( object ) ;
			}
#else
		result &= Q3Object_Submit ( lightGroup, theView ) ;
#endif
		Q3Object_Dispose ( lightGroup ) ;
		}
		
	TQ3AttributeSet attributeSet ;
	if ( theObject->GetAttributeSet ( &attributeSet ) == kQ3Success )
		{
		result &= Q3Object_Submit ( attributeSet , theView ) ;
		Q3Object_Dispose ( attributeSet ) ;
		}
		
	TQ3DrawContextClearImageMethod clearMethod ;
	if ( theObject->GetClearImageMethod ( &clearMethod ) == kQ3Success
	&& clearMethod == kQ3ClearMethodWithColor )
		{
		result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3ImageClearColour ), sizeof ( TQ3ColorARGB ), theObject, NULL ) ;
		}
		
	TQ3Boolean isValidSetDimensions ;
	if ( theObject->GetDimensionsState ( &isValidSetDimensions ) == kQ3Success
	&& isValidSetDimensions == kQ3True )
		{
		result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3ImageDimensions ), sizeof ( TQ3Uns32 ) * 2, theObject, NULL ) ;
		}
		
	return (TQ3Status) result ;
}



//=============================================================================
//      viewhints_metahandler : viewhints metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
viewhints_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectNew:
			return (TQ3XFunctionPointer) e3viewhints_new ;

		case kQ3XMethodTypeObjectDelete:
			return (TQ3XFunctionPointer) e3viewhints_delete ;

		case kQ3XMethodTypeObjectRead:
			return (TQ3XFunctionPointer) e3viewhints_read ;
		
		case kQ3XMethodTypeObjectTraverse :
			return (TQ3XFunctionPointer) e3viewhints_traverse ;
				
		}

	return NULL ;
	}



//=============================================================================
//      E3ViewHints_RegisterClass : ViewHints register.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3ViewHints_RegisterClass ( void )
	{
	// Register the View Hints group class
	return Q3_REGISTER_CLASS	(	kQ3ClassNameViewHint,
									viewhints_metahandler,
									E3ViewHints ) ;
	}





//=============================================================================
//      E3ViewHints_UnregisterClass : ViewHints unregister.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3SharedTypeViewHints, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3ViewHints_New : Create a new View Hints object.
//-----------------------------------------------------------------------------
TQ3ViewHintsObject
E3ViewHints_New ( TQ3ViewObject view )
	{
	return E3ClassTree::CreateInstance ( kQ3SharedTypeViewHints, kQ3False, view ) ;
	}





//=============================================================================
//      E3ViewHints::SetRenderer : Set the view hint's renderer.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::SetRenderer ( TQ3RendererObject theRenderer )
	{
	E3Shared_Replace ( &renderer, theRenderer ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::GetRenderer : Get the view hint's renderer.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::GetRenderer ( TQ3RendererObject* theRenderer )
	{
	// Make sure we have a renderer
	if ( renderer == NULL )
		{
		*theRenderer = NULL ; // Assign a return value
		return kQ3Failure ;
		}



	// Create a new reference to our renderer
	*theRenderer = Q3Shared_GetReference ( renderer ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::SetCamera : Set the view hint's camera.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::SetCamera ( TQ3CameraObject theCamera )
	{
	// Replace the existing renderer reference
	E3Shared_Replace ( &camera, theCamera ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::GetCamera : Get the view hint's camera.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::GetCamera ( TQ3CameraObject* theCamera )
	{
	// Make sure we have a camera
	if ( camera == NULL )
		{
		*theCamera = NULL ; // Assign a return value
		return kQ3Failure ;
		}



	// Create a new reference to our camera
	*theCamera = Q3Shared_GetReference ( camera ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::SetLightGroup : Set the view hint's light group.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::SetLightGroup ( TQ3GroupObject theLightGroup )
	{
	// Replace the existing light group reference
	E3Shared_Replace ( &lightGroup, theLightGroup ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::GetLightGroup : Get the view hint's light group.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::GetLightGroup ( TQ3GroupObject* theLightGroup )
	{
	// Make sure we have a light group
	if ( lightGroup == NULL )
		{
		*theLightGroup = NULL ; // Assign a return value
		return kQ3Failure ;
		}



	// Create a new reference to our light group
	*theLightGroup = Q3Shared_GetReference ( lightGroup ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::SetAttributeSet : Set the view hint's attribute set.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::SetAttributeSet ( TQ3AttributeSet theAttributeSet )
	{
	// Replace the existing attribute set reference
	E3Shared_Replace ( &attributeSet, theAttributeSet ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::GetAttributeSet : Get the view hint's attribute set.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::GetAttributeSet ( TQ3AttributeSet* theAttributeSet )
	{
	// Make sure we have an attribute set
	if ( attributeSet == NULL )
		{
		*theAttributeSet = NULL ; // Assign a return value
		return kQ3Failure ;
		}



	// Create a new reference to our attribute set
	*theAttributeSet = Q3Shared_GetReference ( attributeSet ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::SetDimensionsState : Set the view hint's dimension state.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::SetDimensionsState ( TQ3Boolean isValid )
	{
	isValidSetDimensions = isValid ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::GetDimensionsState : Get the view hint's dimension state.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::GetDimensionsState ( TQ3Boolean* isValid )
	{
	*isValid = isValidSetDimensions ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::SetDimensions : Set the view hint's dimensions.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::SetDimensions ( TQ3Uns32 width, TQ3Uns32 height )
	{
	widthDimensions = width ;
	heightDimensions = height ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::GetDimensions : Get the view hint's dimensions.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::GetDimensions ( TQ3Uns32* width, TQ3Uns32* height )
	{
	*width = widthDimensions ;
	*height = heightDimensions ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::SetMaskState : Set the view hint's set mask state.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::SetMaskState ( TQ3Boolean isValid )
	{
	isValidSetMaskState = isValid ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::GetMaskState : Get the view hint's set mask state.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::GetMaskState ( TQ3Boolean* isValid )
	{
	*isValid = isValidSetMaskState ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::SetMask : Set the view hint's set mask.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::SetMask ( const TQ3Bitmap* theMask )
	{
	// Copy the mask image. We don't compare the current
	// state, and assume that setting a mask may cause a rebuild.
	return E3Bitmap_Replace ( theMask, &mask, kQ3True ) ;
	}





//=============================================================================
//      E3ViewHints::GetMask : Get the view hint's set mask.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::GetMask ( TQ3Bitmap* theMask )
	{
	// Copy the mask image and update our flags
	return E3Bitmap_Replace ( &mask, theMask, kQ3False ) ;
	}





//=============================================================================
//      E3ViewHints::SetClearImageMethod : Set the view hint's clear image method.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::SetClearImageMethod ( TQ3DrawContextClearImageMethod theClearMethod )
	{
	// Set the clear image method [ and update our flags? ]
	if ( clearMethod != theClearMethod )
		 clearMethod = theClearMethod ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::GetClearImageMethod : Get the view hint's clear image method.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::GetClearImageMethod ( TQ3DrawContextClearImageMethod* theClearMethod )
	{
	// Return the clear image method
	*theClearMethod = clearMethod ;
	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::SetClearImageColor : Set the view hint's clear image colour.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::SetClearImageColor ( const TQ3ColorARGB* color )
	{
	// Set the clear colour and update our flags
	if ( memcmp ( &clearImageColor, color, sizeof ( TQ3ColorARGB ) ) != 0 )
		clearImageColor = *color ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewHints::GetClearImageColor : Get the view hint's clear image colour.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewHints::GetClearImageColor ( TQ3ColorARGB* color )
	{
	// Return the clear colour
	*color = clearImageColor ;
	return kQ3Success ;
	}






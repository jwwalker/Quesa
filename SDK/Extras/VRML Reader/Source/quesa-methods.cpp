/*  NAME:
        quesa-methods.cp

    DESCRIPTION:
        File format reader class registration.

    COPYRIGHT:
        Copyright (c) 2005, Quesa Developers. All rights reserved.

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
#include "quesa-methods.h"

#include "CVRMLReader.h"

#if __MACH__
	#include <Quesa/QuesaIO.h>
	#include <Quesa/QuesaExtension.h>
	#include <Quesa/QuesaMemory.h>
	#include <Quesa/QuesaStorage.h>
#else
	#include <QuesaIO.h>
	#include <QuesaExtension.h>
	#include <QuesaMemory.h>
	#include <QuesaStorage.h>
#endif

#include <ostream>
#include <string.h>

namespace
{
	TQ3ObjectType	sRegisteredType = 0;
	
	TQ3XObjectClass	sRegisteredClass = 0;
	
	const char*		kClassNameVRMLReader	= "FileFormat:Reader:VRML";
	
	const char*		kFormatNickname			= "VRML";
}

/*!
	@function	Create
	@abstract	Create a reader instance.
	@discussion	Method type: kQ3XMethodTypeObjectNew, TQ3XObjectNewMethod
*/
static TQ3Status	Create(
							TQ3Object object,
							TQ3FFormatBaseData* privateData,
							void* initData )
{
#pragma unused( object, initData )
	TQ3Status	success = kQ3Success;
	
	try
	{
		// Create the C++ object and store its address in the
		// private data.
		privateData->reserved1 = reinterpret_cast<TQ3Uns32*>(
			new CVRMLReader( privateData ) );
	}
	catch (...)
	{
		success = kQ3Failure;
	}
	
	return success;
}

/*!
	@function	Delete
	@abstract	Delete a reader instance.
	@discussion	Method type: kQ3XMethodTypeObjectDelete, TQ3XObjectDeleteMethod
*/
static void	Delete(
							TQ3Object object,
							TQ3FFormatBaseData* privateData )
{
#pragma unused( object )
	CVRMLReader*	reader = reinterpret_cast<CVRMLReader*>(
		privateData->reserved1 );
	delete reader;
	privateData->reserved1 = NULL;
}

/*!
	@function	CanRead
	@abstract	Test whether we recognize data in a storage object as VRML
				data.
	@discussion	Note that this function will be called before an instance of
				the VRML reader class has been created.
				
				Method type: kQ3XMethodTypeFFormatCanRead, TQ3XFFormatCanReadMethod
*/
static TQ3Boolean	CanRead(
							TQ3StorageObject storage,
							TQ3ObjectType* theFileFormatFound )
{
	TQ3Boolean	didTest = kQ3False;
	*theFileFormatFound = kQ3ObjectTypeInvalid;
	
	TQ3FileVersion	theVersion = CVRMLReader::ReadVersion( storage );
	
	if (theVersion != 0)
	{
		*theFileFormatFound = sRegisteredType;
		didTest = kQ3True;
	}
	
	return didTest;
}

/*!
	@function	Close
	
	@abstract	Close the reader.
	
	@discussion	Method type: kQ3XMethodTypeFFormatClose, TQ3XFFormatCloseMethod
*/
static TQ3Status	Close(
							TQ3FileFormatObject format,
							TQ3Boolean abort )
{
	TQ3Status	status = kQ3Success;
	CVRMLReader*	reader = CVRMLReader::FromFileFormat( format );
	
	try
	{
		reader->Close( abort );
	}
	catch (...)
	{
		status = kQ3Failure;
	}
	
	return kQ3Success;
}

/*!
	@function	ReadHeader
	
	@abstract	Begin reading the data.
	
	@discussion	Method type: kQ3XMethodTypeFFormatReadHeader,
				TQ3XFFormatReadHeaderMethod
*/
static TQ3Status	ReadHeader(
							TQ3FileObject theFile )
{
	TQ3Status	status = kQ3Success;
	try
	{
		CVRMLReader*	reader = CVRMLReader::FromFile( theFile );
		status = reader->ReadHeader()? kQ3Success : kQ3Failure;
	}
	catch (...)
	{
		status = kQ3Failure;
	}
	return status;
}

/*!
	@function	ReadObject
	
	@abstract	Read the next object from the file.
	
	@discussion	Method type: kQ3XMethodTypeFFormatReadObject,
				TQ3XFFormatReadObjectMethod
*/
static TQ3Object	ReadObject(
						TQ3FileObject theFile )
{
	TQ3Object	theObject = NULL;
	CVRMLReader*	reader = CVRMLReader::FromFile( theFile );
	try
	{
		theObject = reader->ReadObject();
	}
	catch (...)
	{
		if (reader->GetDebugStream() != NULL)
		{
			*(reader->GetDebugStream()) << "Exception caught in ReadObject." << std::endl;
		}
	}
	return theObject;
}

/*!
	@function	SkipObject
	
	@abstract	Skip over the next object in the file.
	
	@discussion	Method type: kQ3XMethodTypeFFormatSkipObject,
				TQ3XFFormatSkipObjectMethod
*/
static TQ3Status	SkipObject(
						TQ3FileObject theFile )
{
	TQ3Status	theStatus = kQ3Failure;
	try
	{
		CVRMLReader*	reader = CVRMLReader::FromFile( theFile );
		theStatus = reader->SkipObject()? kQ3Success : kQ3Failure;
	}
	catch (...)
	{
	}
	return theStatus;
}

/*!
	@function	GetNextType
	
	@abstract	Get the type of the next object to be read.
	
	@discussion	Method type: kQ3XMethodTypeFFormatGetNextType,
				TQ3XFFormatGetNextTypeMethod
*/
static TQ3ObjectType	GetNextType(
						TQ3FileObject theFile )
{
	TQ3ObjectType	theType = kQ3ObjectTypeInvalid;
	
	try
	{
		CVRMLReader*	reader = CVRMLReader::FromFile( theFile );
		theType = reader->GetNextType();
	}
	catch (...)
	{
	}
	
	return theType;
}

/*!
	@function	GetFormatType
	
	@abstract	Get the format type for a file.
	
	@discussion	This method is invoked by Q3File_OpenRead in order to
				determine its TQ3FileMode output parameter.  This result
				is assumed to be a combination of flag bits, which do not
				really make sense for VRML.
*/
static TQ3FileMode	GetFormatType( TQ3FileObject inFile )
{
#pragma unused( inFile )
	return 0;
}

/*!
	@function	GetNickname
	
	@abstract	Get the nickname of the format, which is a more "user friendly"
				name than the class name.  It will be a NUL-terminated string.
	
	@param		dataBuffer		Buffer to receive the data, or NULL.
	@param		bufferSize		Size in bytes of the buffer.
	@param		outActualSize	Receives number of bytes returned, or if
								dataBuffer is NULL, receives full length of
								the nickname.
	@result		Success or failure of operation.
*/
static TQ3Status GetNickname( char* dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32* outActualSize )
{
	*outActualSize = strlen( kFormatNickname ) + 1;
	
	if (dataBuffer != NULL)
	{
		if (bufferSize < *outActualSize)
		{
			*outActualSize = bufferSize;
		}
		
		Q3Memory_Copy( kFormatNickname, dataBuffer, *outActualSize );
	}
	
	return kQ3Success;
}

/*!
	@function	vrml_metahandler
	
	@abstract	Metahandler that provides Quesa methods for the VRML reader.
*/
static TQ3XFunctionPointer
vrml_metahandler( TQ3XMethodType methodType )
{
	TQ3XFunctionPointer		theMethod = NULL;

	switch (methodType)
	{
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) Create;
			break;
			
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) Delete;
			break;
			
		case kQ3XMethodTypeFFormatCanRead:
			theMethod = (TQ3XFunctionPointer) CanRead;
			break;
			
		case kQ3XMethodTypeFFormatReadHeader:
			theMethod = (TQ3XFunctionPointer) ReadHeader;
			break;

		case kQ3XMethodTypeFFormatReadObject:
			theMethod = (TQ3XFunctionPointer) ReadObject;
			break;

		case kQ3XMethodTypeFFormatSkipObject:
			theMethod = (TQ3XFunctionPointer) SkipObject;
			break;

		case kQ3XMethodTypeFFormatGetNextType:
			theMethod = (TQ3XFunctionPointer) GetNextType;
			break;
		
		case kQ3XMethodTypeFFormatGetFormatType:
			theMethod = (TQ3XFunctionPointer) GetFormatType;
			break;
		
		// In spite of the name, this method is not just for renderers.
		// It is also used by Q3FileFormatClass_GetFormatNameString.
		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) GetNickname;
			break;
		
		case kQ3XMethodTypeFFormatClose:
			theMethod = (TQ3XFunctionPointer) Close;
			break;
	}
	
	return theMethod;
}

#pragma mark -

/*!
	@function	RegisterVRMLReaderClass
	
	@abstract	Register the VRML reader class with Quesa.
	
	@result		Success or failure of the operation.
*/
TQ3Status	RegisterVRMLReaderClass()
{
	sRegisteredClass = Q3XObjectHierarchy_RegisterClass(
										kQ3FileFormatTypeReader,
										&sRegisteredType, 
										kClassNameVRMLReader,
										vrml_metahandler,
										NULL,
										0,
										sizeof(TQ3FFormatBaseData));
	
	TQ3Status	theStatus = (sRegisteredClass == NULL)? kQ3Failure : kQ3Success;
	
	return theStatus;
}

/*!
	@function	UnregisterVRMLReaderClass
	
	@abstract	Unregister the VRML reader class from Quesa.
	
	@discussion	Typically, one will use Quesa and its plugins until the
				process quits, so it will not be important to unregister
				classes.
	
	@result		Success or failure of the operation.
*/
TQ3Status	UnregisterVRMLReaderClass()
{
	TQ3Status	status = kQ3Success;
	
	if (sRegisteredClass != NULL)
	{
		status = Q3XObjectHierarchy_UnregisterClass(sRegisteredClass);
		sRegisteredClass = NULL;
		sRegisteredType = 0;
	}
	return status;
}


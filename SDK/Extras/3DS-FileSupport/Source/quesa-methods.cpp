/*  NAME:
        quesa-methods.cp

    DESCRIPTION:
        File format reader class registration.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
	  This program is  free  software;  you can redistribute it and/or modify it
	  under the terms of the  GNU Lesser General Public License  as published by 
	  the  Free Software Foundation;  either version 2.1 of the License,  or (at 
	  your option) any later version.
	 
	  This  program  is  distributed in  the  hope that it will  be useful,  but
	  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
	  or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public  
	  License for more details.
	 
	  You should  have received  a copy of the GNU Lesser General Public License
	  along with  this program;  if not, write to the  Free Software Foundation,
	  Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
    ___________________________________________________________________________
*/
#include "quesa-methods.h"

#include "C3DSReader.h"
#include "C3DSWriter.h"

#if __MACH__
	#include <Quesa/QuesaIO.h>
	#include <Quesa/QuesaExtension.h>
	#include <Quesa/QuesaMemory.h>
	#include <Quesa/QuesaStorage.h>
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaTransform.h>
#else
	#include <QuesaIO.h>
	#include <QuesaExtension.h>
	#include <QuesaMemory.h>
	#include <QuesaStorage.h>
	#include <QuesaGeometry.h>
	#include <QuesaTransform.h>
#endif

#include <ostream>
#include <string.h>

namespace
{
	TQ3ObjectType	sRegisteredReaderType= 0;
	TQ3ObjectType	sRegisteredWriterType= 0;
	
	TQ3XObjectClass	sRegisteredReaderClass = 0;
	TQ3XObjectClass	sRegisteredWriterClass = 0;
	
	const char*		kClassName3DSReader	= "FileFormat:Reader:3DS";
	const char*		kClassName3DSWriter	= "FileFormat:Writer:3DS";
	
	const char*		kFormatNickname			= "3DS";
}

#pragma mark --
#pragma mark Reader
#pragma mark --
/*!
	@function	CreateReader
	@abstract	Create a reader instance.
	@discussion	Method type: kQ3XMethodTypeObjectNew, TQ3XObjectNewMethod
*/
static TQ3Status	CreateReader(
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
			new C3DSReader( privateData ) );
	}
	catch (...)
	{
		success = kQ3Failure;
	}
	
	return success;
}

/*!
	@function	DeleteReader
	@abstract	Delete a reader instance.
	@discussion	Method type: kQ3XMethodTypeObjectDelete, TQ3XObjectDeleteMethod
*/
static void	DeleteReader(
							TQ3Object object,
							TQ3FFormatBaseData* privateData )
{
#pragma unused( object )
	C3DSReader*	reader = reinterpret_cast<C3DSReader*>(
		privateData->reserved1 );
	delete reader;
	privateData->reserved1 = NULL;
}

/*!
	@function	CanRead
	@abstract	Test whether we recognize data in a storage object as 3DS
				data.
	@discussion	Note that this function will be called before an instance of
				the 3DS reader class has been created.
				
				Method type: kQ3XMethodTypeFFormatCanRead, TQ3XFFormatCanReadMethod

				read 2 bytes, search for 0x4d4d

				Unfortunately we cant check here for endianness
				since 0x4D4D is symetric.
*/
static TQ3Boolean	CanRead(
							TQ3StorageObject storage,
							TQ3ObjectType* theFileFormatFound )
{
	TQ3Boolean	didTest = kQ3False;
	*theFileFormatFound = kQ3ObjectTypeInvalid;

	TQ3Int16 label;
	TQ3Uns32 sizeRead;
	
	// Check the signature
	//
		
	if(Q3Storage_GetData (storage,0, 2,(unsigned char*)&label, &sizeRead) == kQ3Success){

		if ((sizeRead == 2) && (label == 0x4d4d)){
			*theFileFormatFound = sRegisteredReaderType;
			didTest = kQ3True;
			}
		}
	
	return didTest;
}

/*!
	@function	CloseReader
	
	@abstract	Close the reader.
	
	@discussion	Method type: kQ3XMethodTypeFFormatClose, TQ3XFFormatCloseMethod
*/
static TQ3Status	CloseReader(
							TQ3FileFormatObject format,
							TQ3Boolean abort )
{
	TQ3Status	status = kQ3Success;
	C3DSReader*	reader = C3DSReader::FromFileFormat( format );
	
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
				Despite the method namber the entire file is parsed in this call
*/
static TQ3Status	ReadHeader(
							TQ3FileObject theFile )
{
	TQ3Status	status = kQ3Success;
	try
	{
		C3DSReader*	reader = C3DSReader::FromFile( theFile );
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
	C3DSReader*	reader = C3DSReader::FromFile( theFile );
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
				Does nothing
*/
static TQ3Status	SkipObject(
						TQ3FileObject /*theFile*/ )
{
	TQ3Status	theStatus = kQ3Success;
	return theStatus;
}

/*!
	@function	GetNextType
	
	@abstract	Get the type of the next object to be read.
	
	@discussion	Method type: kQ3XMethodTypeFFormatGetNextType,
				TQ3XFFormatGetNextTypeMethod
				Does nothing
*/
static TQ3ObjectType	GetNextType(
						TQ3FileObject /*theFile*/ )
{
	TQ3ObjectType	theType = kQ3ObjectTypeInvalid;
	
	
	return theType;
}

/*!
	@function	GetFormatType
	
	@abstract	Get the format type for a file.
	
	@discussion	This method is invoked by Q3File_OpenRead in order to
				determine its TQ3FileMode output parameter.  This result
				is assumed to be a combination of flag bits, which do not
				really make sense for 3DS.
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
	@function	_3ds_reader_metahandler
	
	@abstract	Metahandler that provides Quesa methods for the 3DS reader.
*/
static TQ3XFunctionPointer
_3ds_reader_metahandler( TQ3XMethodType methodType )
{
	TQ3XFunctionPointer		theMethod = NULL;

	switch (methodType)
	{
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) CreateReader;
			break;
			
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) DeleteReader;
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
			theMethod = (TQ3XFunctionPointer) CloseReader;
			break;
	}
	
	return theMethod;
}

#pragma mark --
#pragma mark Writer
#pragma mark --

/*!
	@function	CreateWriter
	@abstract	Create a writer instance.
	@discussion	Method type: kQ3XMethodTypeObjectNew, TQ3XObjectNewMethod
*/
static TQ3Status	CreateWriter(
							TQ3Object object,
							TQ3FFormatBaseData* privateData,
							void* initData )
{
#pragma unused( object, initData )
	TQ3Status	success = kQ3Failure;
	
	try
	{
		// Create the C++ object and store its address in the
		// private data.
		C3DSWriter* writer = new C3DSWriter( privateData );
		
			
		if (writer){
			privateData->reserved1 = reinterpret_cast<TQ3Uns32*>(writer);
			success = kQ3Success;
			}
	}
	catch (...)
	{
	}
	
	return success;
}

/*!
	@function	DeleteWriter
	@abstract	Delete a writer instance.
	@discussion	Method type: kQ3XMethodTypeObjectDelete, TQ3XObjectDeleteMethod
*/
static void	DeleteWriter(
							TQ3Object object,
							TQ3FFormatBaseData* privateData )
{
#pragma unused( object )
	C3DSWriter*	writer = reinterpret_cast<C3DSWriter*>(
		privateData->reserved1 );
	delete writer;
	privateData->reserved1 = NULL;
}

/*!
	@function	Writer_StartFile
	@abstract	.
	@discussion	
*/
static TQ3Status	Writer_StartFile(
						TQ3ViewObject			theView,
						TQ3FFormatBaseData		*baseData,
						TQ3DrawContextObject	theDrawContext)
{
#pragma unused( theView, theDrawContext )
	TQ3Status	success = kQ3Failure;
	C3DSWriter*	writer = reinterpret_cast<C3DSWriter*>(baseData->reserved1 );
	
	if(!writer){
		return success;
		}
	try
	{
		success = writer->StartFile(theView, baseData, theDrawContext);
	}
	catch (...)
	{
	}
	
	return success;
}



/*!
	@function	Writer_EndPass
	@abstract	.
	@discussion	
*/
static TQ3ViewStatus	Writer_EndPass(
							TQ3ViewObject			theView,
							TQ3FFormatBaseData		*baseData)
{
#pragma unused( theView)
	TQ3ViewStatus	status = kQ3ViewStatusError;
	C3DSWriter*	writer = reinterpret_cast<C3DSWriter*>(baseData->reserved1 );
	
	if(!writer){
		return status;
		}
	try
	{
		status = writer->EndPass(theView, baseData);
	}
	catch (...)
	{
	}
	
	return status;
}





/*!
	@function	Writer_Cancel
	@abstract	.
	@discussion	
*/
static void			Writer_Cancel(
								TQ3ViewObject			theView,
								TQ3FFormatBaseData		*baseData)
{
#pragma unused( theView)
	C3DSWriter*	writer = reinterpret_cast<C3DSWriter*>(baseData->reserved1 );

	if(!writer){
		return;
		}
	try
	{
		writer->Close(kQ3True);
	}
	catch (...)
	{
	}
	
}



/*!
	@function	Writer_Close
	@abstract	.
	@discussion	
*/
static TQ3Status			Writer_Close(
								TQ3FileFormatObject format,
								TQ3Boolean abort)
{
#pragma unused( theView)
	TQ3Status	success = kQ3Success;
	C3DSWriter*	writer = C3DSWriter::FromFileFormat( format );
	
	if(!writer){
		return success;
		}

	try
	{
		success = writer->Close(abort);
	}
	catch (...)
	{
	}
	
	return success;
}



/*!
	@function	Writer_Submit
	@abstract	.
	@discussion	
*/
static TQ3Status			Writer_Submit(
						TQ3ViewObject		theView,
						TQ3FFormatBaseData	*baseData,
						TQ3Object			theObject,
						TQ3ObjectType		objectType,
						const void			*objectData)
{
#pragma unused( theView)
	TQ3Status	success = kQ3Success;
	C3DSWriter*	writer = reinterpret_cast<C3DSWriter*>(baseData->reserved1 );

	if(!writer){
		return success;
		}

	try
	{
		if( objectType == kQ3SetTypeAttribute){
			writer->UpdateAttributes((TQ3AttributeSet)theObject);
			}
		if( objectType == kQ3StateOperatorTypePush){
			writer->PushGroupLevel();
			}
		else if( objectType == kQ3StateOperatorTypePop){
			writer->PopGroupLevel();
			}
		else if(theObject){
			if(Q3Object_IsType(theObject, (kQ3ShapeTypeTransform))){
				TQ3Matrix4x4 theMatrix;
				Q3Transform_GetMatrix(theObject, &theMatrix);
				writer->UpdateMatrix(&theMatrix);
			}
		}
	}
	catch (...)
	{
	}
	
	return success;
}




/*!
	@function	Writer_Triangle
	@abstract	.
	@discussion	
*/
static TQ3Status			Writer_Triangle(TQ3ViewObject	theView,
										TQ3FFormatBaseData	*baseData,
										TQ3GeometryObject	theGeom,
										TQ3TriangleData		*geomData)
{
#pragma unused( theView)
	TQ3Status	success = kQ3Success;
	C3DSWriter*	writer = reinterpret_cast<C3DSWriter*>(baseData->reserved1 );
	
	try
	{
		writer->Write_Triangle(geomData);
	}
	catch (...)
	{
	}
	
	return success;
}





/*!
	@function	Writer_TriMesh
	@abstract	.
	@discussion	
*/
static TQ3Status			Writer_TriMesh(TQ3ViewObject	theView,
										TQ3FFormatBaseData	*baseData,
										TQ3GeometryObject	theGeom,
										TQ3TriMeshData		*geomData)
{
#pragma unused( theView)
	TQ3Status	success = kQ3Success;
	C3DSWriter*	writer = reinterpret_cast<C3DSWriter*>(baseData->reserved1 );
	
	try
	{
		writer->Write_TriMesh(geomData);
	}
	catch (...)
	{
	}
	
	return success;
}



/*!
	@function	Writer_NULL
	@abstract	Does Nothing.
	@discussion	Used to handle the required geometryes
*/
static TQ3Status			Writer_NULL(TQ3ViewObject	,
										TQ3FFormatBaseData	*,
										TQ3GeometryObject	,
										void		*)
{	
	return kQ3Success;
}




/*!
	@function	_3ds_writer_metahandler
	
	@abstract	Metahandler that provides Quesa methods for the VRML reader.
*/
static TQ3XFunctionPointer
_3ds_writer_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) CreateWriter;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) DeleteWriter;
			break;
			
		case kQ3XMethodTypeRendererStartFrame:
			theMethod = (TQ3XFunctionPointer) Writer_StartFile;
			break;

		case kQ3XMethodTypeRendererEndPass:
			theMethod = (TQ3XFunctionPointer) Writer_EndPass;
			break;

		case kQ3XMethodTypeRendererCancel:
			theMethod = (TQ3XFunctionPointer) Writer_Cancel;
			break;

		// In spite of the name, this method is not just for renderers.
		// It is also used by Q3FileFormatClass_GetFormatNameString.
		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) GetNickname;
			break;
		
		case kQ3XMethodTypeFFormatClose:
			theMethod = (TQ3XFunctionPointer) Writer_Close;
			break;

		// object submit
		case kQ3XMethodTypeFFormatSubmitObject:
			theMethod = (TQ3XFunctionPointer) Writer_Submit;
			break;

		// Required
		case kQ3GeometryTypeTriangle:
			theMethod = (TQ3XFunctionPointer) Writer_Triangle;
			break;

		case kQ3GeometryTypeTriMesh:
			theMethod = (TQ3XFunctionPointer) Writer_TriMesh;
			break;

		case kQ3GeometryTypePoint:
		case kQ3GeometryTypeLine:
		case kQ3GeometryTypeMarker:
		case kQ3GeometryTypePixmapMarker:
			theMethod = (TQ3XFunctionPointer) Writer_NULL;
			break;

		
		}

	return(theMethod);
}





#pragma mark -

/*!
	@function	Register_3DS_Class
	
	@abstract	Register the 3DS reader and writer classes with Quesa.
	
	@result		Success or failure of the operation.
*/
TQ3Status	Register_3DS_Class()
{
	sRegisteredReaderClass = Q3XObjectHierarchy_RegisterClass(
										kQ3FileFormatTypeReader,
										&sRegisteredReaderType, 
										kClassName3DSReader,
										_3ds_reader_metahandler,
										NULL,
										0,
										sizeof(TQ3FFormatBaseData));
	
	sRegisteredWriterClass = Q3XObjectHierarchy_RegisterClass(
										kQ3FileFormatTypeWriter,
										&sRegisteredWriterType, 
										kClassName3DSWriter,
										_3ds_writer_metahandler,
										NULL,
										0,
										sizeof(TQ3FFormatBaseData));
	
	TQ3Status	theStatus = (sRegisteredReaderClass == NULL)? kQ3Failure : kQ3Success;
	
	return theStatus;
}

/*!
	@function	Unregister_3DS_Class
	
	@abstract	Unregister the 3DS reader and writer classes from Quesa.
	
	@discussion	Typically, one will use Quesa and its plugins until the
				process quits, so it will not be important to unregister
				classes.
	
	@result		Success or failure of the operation.
*/
TQ3Status	Unregister_3DS_Class()
{
	TQ3Status	status = kQ3Success;
	TQ3Status	status2 = kQ3Success;
	
	if (sRegisteredReaderClass != NULL)
	{
		status = Q3XObjectHierarchy_UnregisterClass(sRegisteredReaderClass);
		sRegisteredReaderClass = NULL;
		sRegisteredReaderType = 0;
	}
	if (sRegisteredWriterClass != NULL)
	{
		status2 = Q3XObjectHierarchy_UnregisterClass(sRegisteredWriterClass);
		sRegisteredWriterClass = NULL;
		sRegisteredWriterType = 0;
	}
	return (status == kQ3Success && status2 == kQ3Success)?  kQ3Success : kQ3Failure;
}


/*  NAME:
        E3IOFile.c

    DESCRIPTION:
        Implementation of Quesa Q3File related methods.
        
    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#include "E3IO.h"
#include "E3IOData.h"
#include "E3FFR_3DMF.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3file_format_attach .
//-----------------------------------------------------------------------------
static TQ3Status 
e3file_format_attach(TQ3FileObject theFile,TQ3FileFormatObject theFileFormat)
{	TE3FileData				*instanceData = (TE3FileData *) theFile->instanceData;

	E3Shared_Replace(&instanceData->format, theFileFormat);

	if( theFileFormat != NULL)
		{
		E3FileFormat_Init (theFileFormat, instanceData->storage);
		}
		
	return(kQ3Success);
}





//=============================================================================
//      e3file_format_read_test : Recursive test for a child that can read
//									the format.
//-----------------------------------------------------------------------------
static void 
e3file_format_read_test(E3ClassInfoPtr theParent, TQ3StorageObject storage,TQ3ObjectType* theFileFormatFound)
{	E3ClassInfoPtr		theChild;
	TQ3Uns32			n, numChildren;
	TQ3XFFormatCanReadMethod canRead;

	
	*theFileFormatFound = kQ3ObjectTypeInvalid;


	numChildren = E3ClassTree_GetNumChildren(theParent);


	// test the child classes	
	for (n = 0; n < numChildren; n++)
		{
		// Get the child
		theChild = E3ClassTree_GetChild(theParent, n);
		Q3_ASSERT_VALID_PTR(theChild);
		
		
		// test the child
		canRead = (TQ3XFFormatCanReadMethod)E3ClassTree_GetMethod (theChild, kQ3XMethodTypeFFormatCanRead);
		if(canRead != NULL)
			canRead(storage, theFileFormatFound);
		if(*theFileFormatFound != kQ3ObjectTypeInvalid)
			break;
		
		//test subclasses
		e3file_format_read_test (theChild, storage,theFileFormatFound);
		if(*theFileFormatFound != kQ3ObjectTypeInvalid)
			break;
		}

}





//=============================================================================
//      e3file_format_read_find : Finds a format able to read this storage.
//-----------------------------------------------------------------------------
static TQ3Status 
e3file_format_read_find(TQ3FileObject theFile, TQ3StorageObject storage,TQ3ObjectType* theFileFormatFound)
{	E3ClassInfoPtr		theClass;
	TQ3Uns32			numChildren;

	
	*theFileFormatFound = kQ3ObjectTypeInvalid;



	// Find the class
	theClass = E3ClassTree_GetClassByType(kQ3FileFormatTypeReader);
	if (theClass == NULL)
		return(kQ3Failure);

	numChildren = E3ClassTree_GetNumChildren(theClass);


	// test the child classes	
	e3file_format_read_test (theClass, storage, theFileFormatFound);

	if(*theFileFormatFound != kQ3ObjectTypeInvalid)
		return(kQ3Success);

	return(kQ3Failure);
}





//=============================================================================
//      e3file_delete : File delete method.
//-----------------------------------------------------------------------------
static void
e3file_delete(TQ3Object file, void *privateData)
{
	TE3FileData		*instanceData = (TE3FileData *) privateData;
	if(instanceData->status != kE3_File_Status_Closed)
		Q3File_Close((TQ3FileObject)file);

	Q3File_SetStorage((TQ3FileObject)file,NULL);
		
	e3file_format_attach((TQ3FileObject)file,NULL);
		

}





//=============================================================================
//      e3file_metahandler : File metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3file_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3file_delete;
			break;

		/*case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3macStorage_FSSpec_duplicate;
			break;*/
		// Jose, what's the correnct behavior for a double referenced disk file?
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3File_RegisterClasses : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3File_RegisterClass()
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
											kQ3SharedTypeFile,
											kQ3ClassNameFile,
											e3file_metahandler,
											sizeof(TE3FileData));
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3Unknown_RegisterClass ();

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3FileFormat_RegisterClass ();

	return(qd3dStatus);
}





//=============================================================================
//      E3File_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3File_UnregisterClass()
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3SharedTypeFile, kQ3True);

	qd3dStatus = E3FileFormat_UnregisterClass ();
	
	qd3dStatus = E3Unknown_UnregisterClass ();
	
	return(qd3dStatus);
}





//=============================================================================
//      E3File_CallIdle : Calls the idle application method.
//-----------------------------------------------------------------------------
void
E3File_CallIdle(TQ3FileObject theFile)
{	
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
	if(instanceData->idleMethod != NULL)
		instanceData->idleMethod (theFile,instanceData->idleData);
}



//=============================================================================
//      E3File_New : Create a new file object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3FileObject
E3File_New(void)
{	TQ3Object			theObject;


	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3SharedTypeFile, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3File_GetStorage : Get the storage for a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File_GetStorage(TQ3FileObject theFile, TQ3StorageObject *storage)
{	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;



	// Assign a return value
	*storage = NULL;



	// Make sure we have a storage
	if (instanceData->storage == NULL)
		return(kQ3Failure);



	// Create a new reference to our storage
	*storage = Q3Shared_GetReference(instanceData->storage);

	return(kQ3Success);
}





//=============================================================================
//      E3File_SetStorage : Set the storage for a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File_SetStorage(TQ3FileObject theFile, TQ3StorageObject storage)
{
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Closed),kQ3Failure);


	// Replace the existing storage reference
	E3Shared_Replace(&instanceData->storage, storage);
	
	return kQ3Success;
	
}





//=============================================================================
//      E3File_OpenRead : Open a file object for reading.
//-----------------------------------------------------------------------------
TQ3Status
E3File_OpenRead(TQ3FileObject theFile, TQ3FileMode *mode)
{
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3FileFormatObject format;
	TQ3ObjectType formatType;
	TQ3XFFormatReadHeaderMethod readHeader;
	TQ3XFFormatGetFormatTypeMethod formatTypeMethod;
	TQ3XStorageOpenMethod open;
	TQ3XStorageCloseMethod close;
	
	TQ3Status openStatus = kQ3Success;
	TQ3Status readHeaderStatus = kQ3Success;
	
	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Closed),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->storage != NULL),kQ3Failure);
	
	// Open the storage for reading
	open = (TQ3XStorageOpenMethod)E3ClassTree_GetMethod (instanceData->storage->theClass, kQ3XMethodTypeStorageOpen);
	if(open != NULL)
		openStatus = open(instanceData->storage,kQ3False);
	// Find a FileFormat Capable of read this file
	if((openStatus == kQ3Success) && (e3file_format_read_find(theFile,instanceData->storage,&formatType) == kQ3Success))
		{
		// Instantiate it 
		format = Q3FileFormat_NewFromType(formatType);
		
		
		if(format != NULL){

			e3file_format_attach(theFile,format);
			
			instanceData->status = kE3_File_Status_Reading;
			instanceData->reason = kE3_File_Reason_OK;
			// lets the fileFormat orient itself;
			readHeader = (TQ3XFFormatReadHeaderMethod)E3ClassTree_GetMethod (format->theClass, kQ3XMethodTypeFFormatReadHeader);
			if(readHeader != NULL)
				readHeaderStatus = readHeader(theFile);
			
			
			if(mode != NULL)
				{
				// get the custom format ID;
				formatTypeMethod = (TQ3XFFormatGetFormatTypeMethod)E3ClassTree_GetMethod (format->theClass, kQ3XMethodTypeFFormatGetFormatType);
				if(formatTypeMethod != NULL)
					*mode = formatTypeMethod(theFile);
				else
					*mode = (TQ3FileMode)formatType;
				}
			
			Q3Object_Dispose(format);
			
			
			if (readHeaderStatus == kQ3Success)
			// we're now able to read the first object
			return(kQ3Success);
				// else drop down
			}
		// else drop down
		}
	if(openStatus == kQ3Success){
		// close the storage
		close = (TQ3XStorageCloseMethod)E3ClassTree_GetMethod (instanceData->storage->theClass, kQ3XMethodTypeStorageClose);
		if(close != NULL)
			(void)close(instanceData->storage);
		}

	return(kQ3Failure);
}





//=============================================================================
//      E3File_OpenWrite : Open a file for writing.
//-----------------------------------------------------------------------------
TQ3Status
E3File_OpenWrite(TQ3FileObject theFile, TQ3FileMode mode)
{
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3FileFormatObject format;

	TQ3XStorageOpenMethod open;
	TQ3XStorageCloseMethod close;
	
	TQ3Status openStatus = kQ3Success;
	
	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Closed),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->storage != NULL),kQ3Failure);
	
	
	//Convert QD3D modes to Quesa format codes
	switch(mode){
		case kQ3FileModeNormal:
			mode = kQ3FFormatWriterType3DMFNormalBin;
			break;
		case kQ3FileModeStream:
			mode = kQ3FFormatWriterType3DMFStreamBin;
			break;
		case kQ3FileModeDatabase:
			mode = kQ3FFormatWriterType3DMFDatabaseBin;
			break;
		case (kQ3FileModeNormal + kQ3FileModeText):
			mode = kQ3FFormatWriterType3DMFNormalText;
			break;
		case (kQ3FileModeStream + kQ3FileModeText):
			mode = kQ3FFormatWriterType3DMFStreamText;
			break;
		case (kQ3FileModeDatabase + kQ3FileModeText):
			mode = kQ3FFormatWriterType3DMFDatabaseText;
			break;
		case (kQ3FileModeStream + kQ3FileModeDatabase):
			mode = kQ3FFormatWriterType3DMFDatabaseStreamBin;
			break;
		case (kQ3FileModeDatabase + kQ3FileModeStream + kQ3FileModeText):
			mode = kQ3FFormatWriterType3DMFDatabaseStreamText;
			break;
		}
	
	// Instantiate the fileFormat 
	format = Q3FileFormat_NewFromType(mode);
		
	if(format != NULL){
		if(Q3Object_IsType(format,kQ3FileFormatTypeWriter) == kQ3True){
			// Open the storage for writing
			open = (TQ3XStorageOpenMethod)E3ClassTree_GetMethod (instanceData->storage->theClass, kQ3XMethodTypeStorageOpen);
			if(open != NULL)
				openStatus = open(instanceData->storage,kQ3True);
			if(openStatus == kQ3Success){
			
				if(e3file_format_attach(theFile,format) == kQ3Success){
				
					instanceData->status = kE3_File_Status_Writing;
					instanceData->reason = kE3_File_Reason_OK;
					
					
					Q3Object_Dispose(format);
					
					return kQ3Success;
					}
				else{
					// close the storage
					close = (TQ3XStorageCloseMethod)E3ClassTree_GetMethod (instanceData->storage->theClass, kQ3XMethodTypeStorageClose);
					if(close != NULL)
						(void)close(instanceData->storage);
					}
				}
			}
		
		/* catch */
		Q3Object_Dispose(format);
		return kQ3Failure;
		}
	else // format == NULL
		return kQ3Failure;
		
}





//=============================================================================
//      E3File_IsOpen : Test to see if a file object is open.
//-----------------------------------------------------------------------------
TQ3Status
E3File_IsOpen(TQ3FileObject theFile, TQ3Boolean *isOpen)
{
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
	*isOpen = kQ3True;
	if(instanceData->status == kE3_File_Status_Closed)
		*isOpen = kQ3False;

	return(kQ3Success);
}





//=============================================================================
//      E3File_GetMode : Get the mode for a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File_GetMode(TQ3FileObject theFile, TQ3FileMode *mode)
{
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
	
	Q3_REQUIRE_OR_RESULT((instanceData->status != kE3_File_Status_Closed),kQ3Failure);
	
	*mode = instanceData->mode;
	return(kQ3Success);
}





//=============================================================================
//      E3File_GetVersion : Get the version for a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File_GetVersion(TQ3FileObject theFile, TQ3FileVersion *version)
{	
	TQ3FFormatBaseData* fformatData;
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
	
	
	
	Q3_REQUIRE_OR_RESULT((instanceData->status != kE3_File_Status_Closed),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);
	
	fformatData = (TQ3FFormatBaseData*)instanceData->format->instanceData;

	*version = fformatData->fileVersion;
	return(kQ3Success);
}





//=============================================================================
//      E3File_Close : Close a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File_Close(TQ3FileObject theFile)
{	TE3FileData				*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XStorageCloseMethod	closeStorage;
	TQ3XFFormatCloseMethod	closeFormat;



	// Get our methods
	closeStorage = NULL;
	closeFormat  = NULL;

	if (instanceData->storage != NULL)
		closeStorage = (TQ3XStorageCloseMethod)
						E3ClassTree_GetMethod(instanceData->storage->theClass, kQ3XMethodTypeStorageClose);

	if (instanceData->format != NULL)
		closeFormat = (TQ3XFFormatCloseMethod)
						E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatClose);



	// close the FileFormat _gently_
	if(closeFormat != NULL)
		closeFormat(instanceData->format,kQ3False);


	// close the storage
	if(closeStorage != NULL)
		closeStorage(instanceData->storage);



	// delete the FileFormat
	e3file_format_attach(theFile,NULL);

	instanceData->status = kE3_File_Status_Closed;
	instanceData->reason = kE3_File_Reason_OK;

	return(kQ3Success);
}





//=============================================================================
//      E3File_Cancel : Cancel a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File_Cancel(TQ3FileObject theFile)
{	TE3FileData				*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XStorageCloseMethod	closeStorage;
	TQ3XFFormatCloseMethod	closeFormat;



	// Get our methods
	closeStorage = NULL;
	closeFormat  = NULL;

	if (instanceData->storage != NULL)
		closeStorage = (TQ3XStorageCloseMethod)
						E3ClassTree_GetMethod(instanceData->storage->theClass, kQ3XMethodTypeStorageClose);

	if (instanceData->format != NULL)
		closeFormat = (TQ3XFFormatCloseMethod)
						E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatClose);



	// close the FileFormat
	if(closeFormat != NULL)
		closeFormat(instanceData->format,kQ3True);


	// close the storage
	if(closeStorage != NULL)
		closeStorage(instanceData->storage);



	// delete the FileFormat
	e3file_format_attach(theFile,NULL);

	instanceData->status = kE3_File_Status_Closed;
	instanceData->reason = kE3_File_Reason_Cancelled;

	return(kQ3Success);
}





//=============================================================================
//      E3File_GetNextObjectType : Get the type of the next object in a file.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3File_GetNextObjectType(TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatGetNextTypeMethod	getNextObjectType;


	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);


	getNextObjectType = (TQ3XFFormatGetNextTypeMethod)
							E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatGetNextType);



	E3File_CallIdle(theFile);

	if(getNextObjectType != NULL)
		return getNextObjectType(theFile);
	
	return(kQ3ObjectTypeInvalid);
}





//=============================================================================
//      E3File_IsNextObjectOfType : Test the type of the next object.
//-----------------------------------------------------------------------------
TQ3Boolean
E3File_IsNextObjectOfType(TQ3FileObject theFile, TQ3ObjectType ofType)
{
	TQ3ObjectType nextObjectType = Q3File_GetNextObjectType (theFile);
	
	if(ofType == nextObjectType)
		return(kQ3True);
	
	if((nextObjectType != kQ3ObjectTypeInvalid) &&
	  (E3ClassTree_IsType (E3ClassTree_GetClassByType (nextObjectType), ofType)))
		return(kQ3True);
		
	return(kQ3False);
}





//=============================================================================
//      E3File_ReadObject : Read the next object.
//-----------------------------------------------------------------------------
TQ3Object
E3File_ReadObject(TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatReadObjectMethod		readObject;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),NULL);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),NULL);

	readObject = (TQ3XFFormatReadObjectMethod)
						E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatReadObject);
						
	E3File_CallIdle(theFile);

	if(readObject != NULL)
		return readObject(theFile);
	
	return(NULL);
}





//=============================================================================
//      E3File_SkipObject : Skip the next object.
//-----------------------------------------------------------------------------
TQ3Status
E3File_SkipObject(TQ3FileObject theFile)
{
	TE3FileData						*instanceData = (TE3FileData *) theFile->instanceData;
	TQ3XFFormatSkipObjectMethod		skipObject;

	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);

	skipObject = (TQ3XFFormatSkipObjectMethod)
							E3ClassTree_GetMethod(instanceData->format->theClass, kQ3XMethodTypeFFormatSkipObject);

	E3File_CallIdle(theFile);

	if(skipObject != NULL)
		return skipObject(theFile);
	
	return(kQ3Failure);
}





//=============================================================================
//      E3File_IsEndOfData : Test for more data to read.
//-----------------------------------------------------------------------------
TQ3Boolean
E3File_IsEndOfData(TQ3FileObject theFile)
{
	TE3FFormat3DMF_Data* fformatData;
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
	
	
	
	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3True);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3True);
	Q3_REQUIRE_OR_RESULT((instanceData->mode <= 3),kQ3True); // only for 3DMF
	
	fformatData = (TE3FFormat3DMF_Data*)instanceData->format->instanceData;

	return(fformatData->noMoreObjectData);
}





//=============================================================================
//      E3File_IsEndOfContainer : Test for more sub-objects.
//-----------------------------------------------------------------------------
TQ3Boolean
E3File_IsEndOfContainer(TQ3FileObject theFile, TQ3Object rootObject)
{
	TE3FFormat3DMF_Data* fformatData;
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
	
	
	
	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3True);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3True);
	Q3_REQUIRE_OR_RESULT((instanceData->mode <= 3),kQ3True); // only for 3DMF
	
	fformatData = (TE3FFormat3DMF_Data*)instanceData->format->instanceData;

	return((TQ3Boolean)((!fformatData->inContainer) || (fformatData->baseData.noMoreObjects)) );
}





//=============================================================================
//      E3File_IsEndOfFile : Test for the end of the file.
//-----------------------------------------------------------------------------
TQ3Boolean
E3File_IsEndOfFile(TQ3FileObject theFile)
{
	TQ3FFormatBaseData* fformatData;
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
	
	
	
	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3True);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3True);
	
	fformatData = (TQ3FFormatBaseData*)instanceData->format->instanceData;

	return(fformatData->noMoreObjects);
}





//=============================================================================
//      E3File_MarkAsExternalReference : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3File_MarkAsExternalReference(TQ3FileObject theFile, TQ3SharedObject sharedObject)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3File_GetExternalReferences : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3GroupObject
E3File_GetExternalReferences(TQ3FileObject theFile)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3File_SetReadInGroup : Set the mode by which group objects are read.
//-----------------------------------------------------------------------------
TQ3Status
E3File_SetReadInGroup(TQ3FileObject theFile, TQ3FileReadGroupState readGroupState)
{
	TQ3FFormatBaseData* fformatData;
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
		
	
	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->mode <= 3),kQ3Failure); // only for 3DMF
	Q3_REQUIRE_OR_RESULT((readGroupState == kQ3FileReadWholeGroup) ||
						(readGroupState == kQ3FileReadObjectsInGroup),kQ3Failure);
	
	fformatData = (TQ3FFormatBaseData*)instanceData->format->instanceData;

	fformatData->readInGroup = (TQ3Boolean) (readGroupState == kQ3FileReadWholeGroup);
		
	return (kQ3Success);
		
}





//=============================================================================
//      E3File_GetReadInGroup : Get the mode by which group objects are read.
//-----------------------------------------------------------------------------
TQ3Status
E3File_GetReadInGroup(TQ3FileObject theFile, TQ3FileReadGroupState *readGroupState)
{
	TQ3FFormatBaseData* fformatData;
	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;
	
	*readGroupState = kQ3FileReadWholeGroup;
	
	
	Q3_REQUIRE_OR_RESULT((instanceData->status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->format != NULL),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData->mode <= 3),kQ3Failure); // only for 3DMF
	
	fformatData = (TQ3FFormatBaseData*)instanceData->format->instanceData;

	if (fformatData->readInGroup == kQ3False)
		*readGroupState = kQ3FileReadObjectsInGroup;
		
	if (fformatData->groupDeepCounter > 0)
		*readGroupState |= kQ3FileCurrentlyInsideGroup;
		
	return (kQ3Success);
		
}





//=============================================================================
//      E3File_SetIdleMethod : Set the idle method for a file.
//-----------------------------------------------------------------------------
TQ3Status
E3File_SetIdleMethod(TQ3FileObject theFile, TQ3FileIdleMethod idle, const void *idleData)
{	TE3FileData		*instanceData = (TE3FileData *) theFile->instanceData;



	// Replace the existing camera reference
	instanceData->idleMethod = idle;
	instanceData->idleData = idleData;
	return (kQ3Success);

}




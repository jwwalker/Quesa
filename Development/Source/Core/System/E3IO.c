/*  NAME:
        E3IOFile.c

    DESCRIPTION:
        Implementation of Quesa Q3File related methods.
        
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
#include "E3IO.h"
#include "E3IOData.h"
#include "E3FFR_3DMF.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3file_format_attach .
//-----------------------------------------------------------------------------
TQ3Status 
e3file_format_attach ( E3File* theFile,TQ3FileFormatObject theFileFormat )
	{
	if ( ( theFileFormat != theFile->instanceData.format ) && ( theFile->instanceData.format != NULL ) )
		{
		E3FileFormat_Terminate ( theFile->instanceData.format ) ;
		}
		
	E3Shared_Replace ( & theFile->instanceData.format, theFileFormat ) ;

	if ( theFileFormat != NULL)
		{
		E3FileFormat_Init ( theFileFormat, theFile->instanceData.storage ) ;
		}
		
	return kQ3Success ;
	}





//=============================================================================
//      e3file_format_read_test : Recursive test for a child that can read
//									the format.
//-----------------------------------------------------------------------------
static void 
e3file_format_read_test(E3ClassInfoPtr theParent, TQ3StorageObject storage,TQ3ObjectType* theFileFormatFound)
	{
	*theFileFormatFound = kQ3ObjectTypeInvalid ;


	TQ3Uns32 numChildren = theParent->GetNumChildren () ;


	// test the child classes	
	for ( TQ3Uns32 n = 0 ; n < numChildren ; ++n )
		{
		// Get the child
		E3ClassInfoPtr theChild = theParent->GetChild ( n ) ;
		Q3_ASSERT_VALID_PTR(theChild);
		
		
		// test the child
		TQ3XFFormatCanReadMethod canRead = (TQ3XFFormatCanReadMethod) theChild->GetMethod ( kQ3XMethodTypeFFormatCanRead ) ;
		if ( canRead != NULL )
			canRead ( storage, theFileFormatFound ) ;
		if ( *theFileFormatFound != kQ3ObjectTypeInvalid )
			break;
		
		//test subclasses
		e3file_format_read_test ( theChild, storage,theFileFormatFound ) ;
		if ( *theFileFormatFound != kQ3ObjectTypeInvalid )
			break ;
		}
	}





//=============================================================================
//      e3file_format_read_find : Finds a format able to read this storage.
//-----------------------------------------------------------------------------
static TQ3Status 
e3file_format_read_find(TQ3FileObject theFile, TQ3StorageObject storage,TQ3ObjectType* theFileFormatFound)
	{
	*theFileFormatFound = kQ3ObjectTypeInvalid ;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( kQ3FileFormatTypeReader ) ;
	if ( theClass == NULL )
		return kQ3Failure ;

	TQ3Uns32 numChildren = theClass->GetNumChildren () ;


	// test the child classes	
	e3file_format_read_test ( theClass, storage, theFileFormatFound ) ;

	if ( *theFileFormatFound != kQ3ObjectTypeInvalid )
		return kQ3Success ;

	return kQ3Failure ;
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
		
	e3file_format_attach ( (E3File*)file, NULL ) ;
		

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
	qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameFile,
										e3file_metahandler,
										E3File ) ;
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3Unknown_RegisterClass ();

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ViewHints_RegisterClass ();

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
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3SharedTypeFile, kQ3True);

	qd3dStatus = E3FileFormat_UnregisterClass ();
	
	qd3dStatus = E3Unknown_UnregisterClass ();
	
	return(qd3dStatus);
}





//=============================================================================
//      E3File_CallIdle : Calls the idle application method.
//-----------------------------------------------------------------------------
void
E3File::CallIdle ( void)
	{	
	if ( instanceData.idleMethod != NULL )
		instanceData.idleMethod ( this, instanceData.idleData ) ;
	}



//=============================================================================
//      E3File_New : Create a new file object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3FileObject
E3File_New(void)
	{
	// Create the object
	return E3ClassTree::CreateInstance ( kQ3SharedTypeFile, kQ3False, NULL ) ;
	}




//=============================================================================
//      E3File_GetStorage : Get the storage for a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File::GetStorage ( TQ3StorageObject *storage )
	{
	// Assign a return value
	*storage = NULL;



	// Make sure we have a storage
	if ( instanceData.storage == NULL )
		return kQ3Failure ;



	// Create a new reference to our storage
	*storage = Q3Shared_GetReference ( instanceData.storage ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3File_SetStorage : Set the storage for a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File::SetStorage ( TQ3StorageObject storage )
	{
	Q3_REQUIRE_OR_RESULT ( ( instanceData.status == kE3_File_Status_Closed ), kQ3Failure ) ;


	// Replace the existing storage reference
	E3Shared_Replace ( & instanceData.storage, storage ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3File_OpenRead : Open a file object for reading.
//-----------------------------------------------------------------------------
TQ3Status
E3File::OpenRead ( TQ3FileMode* mode )
	{
	TQ3ObjectType formatType;
	
	TQ3Status openStatus = kQ3Success ;
	TQ3Status readHeaderStatus = kQ3Success ;
	
	Q3_REQUIRE_OR_RESULT((instanceData.status == kE3_File_Status_Closed),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData.storage != NULL),kQ3Failure);
	
	// Open the storage for reading
	TQ3XStorageOpenMethod open = (TQ3XStorageOpenMethod) instanceData.storage->GetMethod ( kQ3XMethodTypeStorageOpen ) ;
	if ( open != NULL )
		openStatus = open ( instanceData.storage, kQ3False ) ;
	// Find a FileFormat Capable of read this file
	if ( ( openStatus != kQ3Failure )
	&& ( e3file_format_read_find ( this, instanceData.storage, & formatType ) != kQ3Failure ) )
		{
		// Instantiate it 
		TQ3FileFormatObject format = Q3FileFormat_NewFromType ( formatType ) ;
		
		
		if ( format != NULL )
			{
			e3file_format_attach ( this, format ) ;
			
			instanceData.status = kE3_File_Status_Reading ;
			instanceData.reason = kE3_File_Reason_OK ;
			// lets the fileFormat orient itself;
			TQ3XFFormatReadHeaderMethod readHeader = (TQ3XFFormatReadHeaderMethod) format->GetMethod ( kQ3XMethodTypeFFormatReadHeader ) ;
			if ( readHeader != NULL )
				readHeaderStatus = readHeader ( this ) ;
			
			
			// get the custom format ID;
			TQ3XFFormatGetFormatTypeMethod formatTypeMethod = (TQ3XFFormatGetFormatTypeMethod) format->GetMethod ( kQ3XMethodTypeFFormatGetFormatType ) ;
			if ( formatTypeMethod != NULL )
				instanceData.mode = formatTypeMethod ( this ) ;
			else
				instanceData.mode = (TQ3FileMode) formatType ;

			if( mode != NULL )
				*mode = instanceData.mode ;
			
			Q3Object_Dispose ( format ) ;
			
			
			if ( readHeaderStatus != kQ3Failure )
			// we're now able to read the first object
				return kQ3Success ;
				// else drop down
			}
		// else drop down
		}
	if ( openStatus != kQ3Failure )
		{
		// close the storage
		TQ3XStorageCloseMethod close = (TQ3XStorageCloseMethod)instanceData.storage->GetMethod ( kQ3XMethodTypeStorageClose ) ;
		if ( close != NULL )
			(void) close ( instanceData.storage ) ;
		}

	return kQ3Failure ;
	}





//=============================================================================
//      E3File_OpenWrite : Open a file for writing.
//-----------------------------------------------------------------------------
TQ3Status
E3File::OpenWrite ( TQ3FileMode mode )
	{
	TQ3ObjectType formatType = 0 ;

	TQ3Status openStatus = kQ3Success ;
	
	Q3_REQUIRE_OR_RESULT((instanceData.status == kE3_File_Status_Closed),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData.storage != NULL),kQ3Failure);
	
	instanceData.mode = mode ;
	
	//Convert QD3D modes to Quesa format codes
	switch ( mode )
		{
		case kQ3FileModeNormal:
			formatType = kQ3FFormatWriterType3DMFNormalBin ;
			break;
		case kQ3FileModeStream:
			formatType = kQ3FFormatWriterType3DMFStreamBin ;
			break;
		case kQ3FileModeDatabase:
			formatType = kQ3FFormatWriterType3DMFDatabaseBin ;
			break;
		case (kQ3FileModeNormal + kQ3FileModeText):
			formatType = kQ3FFormatWriterType3DMFNormalText ;
			break;
		case (kQ3FileModeStream + kQ3FileModeText):
			formatType = kQ3FFormatWriterType3DMFStreamText ;
			break;
		case (kQ3FileModeDatabase + kQ3FileModeText):
			formatType = kQ3FFormatWriterType3DMFDatabaseText ;
			break;
		case (kQ3FileModeStream + kQ3FileModeDatabase):
			formatType = kQ3FFormatWriterType3DMFDatabaseStreamBin ;
			break;
		case (kQ3FileModeDatabase + kQ3FileModeStream + kQ3FileModeText):
			formatType = kQ3FFormatWriterType3DMFDatabaseStreamText ;
			break;
		}
	
	// Instantiate the fileFormat 
	TQ3FileFormatObject format = Q3FileFormat_NewFromType ( formatType ) ;
		
	if ( format != NULL )
		{
		if ( Q3Object_IsType ( format, kQ3FileFormatTypeWriter) == kQ3True )
			{
			// Open the storage for writing
			TQ3XStorageOpenMethod open = (TQ3XStorageOpenMethod) instanceData.storage->GetMethod ( kQ3XMethodTypeStorageOpen ) ;
			if ( open != NULL )
				openStatus = open ( instanceData.storage, kQ3True ) ;
			if ( openStatus == kQ3Success )
				{
				if ( e3file_format_attach ( this, format ) == kQ3Success )
					{
					instanceData.status = kE3_File_Status_Writing ;
					instanceData.reason = kE3_File_Reason_OK ;
					
					
					Q3Object_Dispose ( format ) ;
					
					return kQ3Success;
					}
				else
					{
					// close the storage
					TQ3XStorageCloseMethod close = (TQ3XStorageCloseMethod) instanceData.storage->GetMethod ( kQ3XMethodTypeStorageClose ) ;
					if ( close != NULL )
						(void) close ( instanceData.storage ) ;
					}
				}
			}
		
		/* catch */
		Q3Object_Dispose ( format ) ;
		}
	
	return kQ3Failure ;
	}





//=============================================================================
//      E3File_IsOpen : Test to see if a file object is open.
//-----------------------------------------------------------------------------
TQ3Status
E3File::IsOpen ( TQ3Boolean* isOpen )
	{
	*isOpen = kQ3True ;
	if ( instanceData.status == kE3_File_Status_Closed )
		*isOpen = kQ3False ;

	return kQ3Success ;
	}





//=============================================================================
//      E3File_GetMode : Get the mode for a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File::GetMode ( TQ3FileMode* mode )
	{
	Q3_REQUIRE_OR_RESULT( ( instanceData.status != kE3_File_Status_Closed ), kQ3Failure ) ;
	
	*mode = instanceData.mode ;
	return kQ3Success ;
	}





//=============================================================================
//      E3File_GetVersion : Get the version for a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File::GetVersion ( TQ3FileVersion *version )
	{	
	Q3_REQUIRE_OR_RESULT(( instanceData.status != kE3_File_Status_Closed),kQ3Failure);
	Q3_REQUIRE_OR_RESULT(( instanceData.format != NULL),kQ3Failure);
	
	TQ3FFormatBaseData* fformatData = (TQ3FFormatBaseData*) instanceData.format->FindLeafInstanceData () ;

	*version = fformatData->fileVersion ;
	return kQ3Success ;
	}





//=============================================================================
//      E3File_Close : Close a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File::Close ( void )
	{
	// Get our methods
	TQ3XStorageCloseMethod closeStorage = NULL;
	TQ3XFFormatCloseMethod closeFormat  = NULL;

	if ( instanceData.storage != NULL )
		closeStorage = (TQ3XStorageCloseMethod) instanceData.storage->GetMethod ( kQ3XMethodTypeStorageClose ) ;

	if ( instanceData.format != NULL )
		closeFormat = (TQ3XFFormatCloseMethod) instanceData.format->GetMethod ( kQ3XMethodTypeFFormatClose ) ;



	// close the FileFormat _gently_
	if ( closeFormat != NULL )
		closeFormat ( instanceData.format, kQ3False ) ;


	// close the storage
	if ( closeStorage != NULL )
		closeStorage ( instanceData.storage ) ;



	// delete the FileFormat
	e3file_format_attach ( this, NULL ) ;

	instanceData.status = kE3_File_Status_Closed ;
	instanceData.reason = kE3_File_Reason_OK ;

	return kQ3Success ;
	}





//=============================================================================
//      E3File_Cancel : Cancel a file object.
//-----------------------------------------------------------------------------
TQ3Status
E3File::Cancel ( void )
	{
	// Get our methods
	TQ3XStorageCloseMethod closeStorage = NULL;
	TQ3XFFormatCloseMethod closeFormat  = NULL;

	if ( instanceData.storage != NULL )
		closeStorage = (TQ3XStorageCloseMethod) instanceData.storage->GetMethod ( kQ3XMethodTypeStorageClose ) ;

	if ( instanceData.format != NULL )
		closeFormat = (TQ3XFFormatCloseMethod) instanceData.format->GetMethod ( kQ3XMethodTypeFFormatClose ) ;



	// close the FileFormat
	if ( closeFormat != NULL )
		closeFormat ( instanceData.format, kQ3True ) ;


	// close the storage
	if ( closeStorage != NULL )
		closeStorage ( instanceData.storage ) ;



	// delete the FileFormat
	e3file_format_attach ( this, NULL ) ;

	instanceData.status = kE3_File_Status_Closed ;
	instanceData.reason = kE3_File_Reason_Cancelled ;

	return kQ3Success ;
	}





//=============================================================================
//      E3File_GetNextObjectType : Get the type of the next object in a file.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3File::GetNextObjectType ( void )
	{
	Q3_REQUIRE_OR_RESULT(( instanceData.status == kE3_File_Status_Reading),kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(( instanceData.format != NULL),kQ3Failure);


	TQ3XFFormatGetNextTypeMethod getNextObjectType =
		(TQ3XFFormatGetNextTypeMethod) instanceData.format->GetMethod ( kQ3XMethodTypeFFormatGetNextType ) ;



	CallIdle () ;

	if ( getNextObjectType != NULL )
		return getNextObjectType ( this ) ;
	
	return kQ3ObjectTypeInvalid ;
	}





//=============================================================================
//      E3File_IsNextObjectOfType : Test the type of the next object.
//-----------------------------------------------------------------------------
TQ3Boolean
E3File::IsNextObjectOfType ( TQ3ObjectType ofType )
	{
	TQ3ObjectType nextObjectType = Q3File_GetNextObjectType ( this ) ;
	
	if ( ofType == nextObjectType )
		return kQ3True ;
	
	if ( ( nextObjectType != kQ3ObjectTypeInvalid )
	&& ( E3ClassTree::GetClass ( nextObjectType )->IsType ( ofType ) ) )
		return kQ3True ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3File_ReadObject : Read the next object.
//-----------------------------------------------------------------------------
TQ3Object
E3File::ReadObject ()
	{
	Q3_REQUIRE_OR_RESULT((instanceData.status == kE3_File_Status_Reading),NULL);
	Q3_REQUIRE_OR_RESULT((instanceData.format != NULL),NULL);

	TQ3XFFormatReadObjectMethod readObject = (TQ3XFFormatReadObjectMethod) instanceData.format->GetMethod ( kQ3XMethodTypeFFormatReadObject ) ;
						
	CallIdle () ;

	if ( readObject != NULL )
		return readObject ( this ) ;
	
	return NULL ;
	}





//=============================================================================
//      E3File_SkipObject : Skip the next object.
//-----------------------------------------------------------------------------
TQ3Status
E3File::SkipObject ( void )
	{
	Q3_REQUIRE_OR_RESULT((instanceData.status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData.format != NULL),kQ3Failure);

	TQ3XFFormatSkipObjectMethod skipObject = (TQ3XFFormatSkipObjectMethod) instanceData.format->GetMethod ( kQ3XMethodTypeFFormatSkipObject ) ;

	CallIdle () ;

	if ( skipObject != NULL )
		return skipObject ( this ) ;
	
	return kQ3Failure ;
	}





//=============================================================================
//      E3File_IsEndOfData : Test for more data to read.
//-----------------------------------------------------------------------------
TQ3Boolean
E3File::IsEndOfData ( void )
	{
	Q3_REQUIRE_OR_RESULT((instanceData.status == kE3_File_Status_Reading),kQ3True);
	Q3_REQUIRE_OR_RESULT((instanceData.format != NULL),kQ3True);
	Q3_REQUIRE_OR_RESULT((instanceData.mode <= (kQ3FileModeSwap|kQ3FileModeDatabase|kQ3FileModeStream)),kQ3True); // only for 3DMF
	
	TE3FFormat3DMF_Data* fformatData = (TE3FFormat3DMF_Data*) instanceData.format->FindLeafInstanceData () ;

	return fformatData->noMoreObjectData ;
	}





//=============================================================================
//      E3File_IsEndOfContainer : Test for more sub-objects.
//-----------------------------------------------------------------------------
TQ3Boolean
E3File::IsEndOfContainer ( TQ3Object rootObject )
	{
	Q3_REQUIRE_OR_RESULT((instanceData.status == kE3_File_Status_Reading),kQ3True);
	Q3_REQUIRE_OR_RESULT((instanceData.format != NULL),kQ3True);
	Q3_REQUIRE_OR_RESULT((instanceData.mode <= (kQ3FileModeSwap|kQ3FileModeDatabase|kQ3FileModeStream)),kQ3True); // only for 3DMF
	
	TE3FFormat3DMF_Data* fformatData = (TE3FFormat3DMF_Data*) instanceData.format->FindLeafInstanceData () ;

	return (TQ3Boolean) ( ( ! fformatData->inContainer ) || ( fformatData->baseData.noMoreObjects ) ) ;
	}





//=============================================================================
//      E3File_IsEndOfFile : Test for the end of the file.
//-----------------------------------------------------------------------------
TQ3Boolean
E3File::IsEndOfFile ( void )
	{
	Q3_REQUIRE_OR_RESULT((instanceData.status == kE3_File_Status_Reading),kQ3True);
	Q3_REQUIRE_OR_RESULT((instanceData.format != NULL),kQ3True);
	
	TQ3FFormatBaseData* fformatData = (TQ3FFormatBaseData*) instanceData.format->FindLeafInstanceData () ;

	return fformatData->noMoreObjects ;
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
E3File::SetReadInGroup ( TQ3FileReadGroupState readGroupState )
	{
	Q3_REQUIRE_OR_RESULT((instanceData.status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData.format != NULL),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData.mode <= (kQ3FileModeSwap|kQ3FileModeDatabase|kQ3FileModeStream)),kQ3Failure); // only for 3DMF
	Q3_REQUIRE_OR_RESULT((readGroupState == kQ3FileReadWholeGroup) ||
						(readGroupState == kQ3FileReadObjectsInGroup),kQ3Failure);
	
	TQ3FFormatBaseData* fformatData = (TQ3FFormatBaseData*) instanceData.format->FindLeafInstanceData () ;

	fformatData->readInGroup = (TQ3Boolean) ( readGroupState == kQ3FileReadWholeGroup ) ;
		
	return kQ3Success ;
	}





//=============================================================================
//      E3File_GetReadInGroup : Get the mode by which group objects are read.
//-----------------------------------------------------------------------------
TQ3Status
E3File::GetReadInGroup ( TQ3FileReadGroupState* readGroupState )
	{
	*readGroupState = kQ3FileReadWholeGroup ;
	
	
	Q3_REQUIRE_OR_RESULT((instanceData.status == kE3_File_Status_Reading),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData.format != NULL),kQ3Failure);
	Q3_REQUIRE_OR_RESULT((instanceData.mode <= (kQ3FileModeSwap|kQ3FileModeDatabase|kQ3FileModeStream)),kQ3Failure); // only for 3DMF
	
	TQ3FFormatBaseData* fformatData = (TQ3FFormatBaseData*) instanceData.format->FindLeafInstanceData () ;

	if ( fformatData->readInGroup == kQ3False )
		*readGroupState = kQ3FileReadObjectsInGroup ;
		
	if ( fformatData->groupDeepCounter > 0 )
		*readGroupState |= kQ3FileCurrentlyInsideGroup ;
		
	return kQ3Success ;
	}





//=============================================================================
//      E3File_SetIdleMethod : Set the idle method for a file.
//-----------------------------------------------------------------------------
TQ3Status
E3File::SetIdleMethod ( TQ3FileIdleMethod idle, const void* idleData )
	{
	// Replace the existing camera reference
	instanceData.idleMethod = idle ;
	instanceData.idleData = idleData ;
	return kQ3Success ;
	}




//=============================================================================
//      E3File_GetFileFormat : Get the file format for a file.
//-----------------------------------------------------------------------------
TQ3FileFormatObject 
E3File::GetFileFormat ( void )
	{
	return instanceData.format ;
	}




//=============================================================================
//      E3File::GetFileStatus : Get the file status for a file.
//-----------------------------------------------------------------------------
TE3FileStatus 
E3File::GetFileStatus ( void )
	{
	return instanceData.status ;
	}





/*  NAME:
        E3FFormat_Sample.c

    DESCRIPTION:
        Sample file to create an Importer Plug-In for Quesa.
        
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
#include <QD3D.h>
#include <QD3DExtension.h>
#include <QD3DIO.h>
#include <QD3DStorage.h>

#include "QuesaIO.h"


#include "E3FFormat_Sample.h"





//=============================================================================
//      Platform specific #includes
//-----------------------------------------------------------------------------
#if OS_MACINTOSH
	#include <Errors.h>
#endif





//=============================================================================
//      Internal yypes
//-----------------------------------------------------------------------------
typedef struct {
	TQ3FFormatBaseData				baseData;
		// add here the data for your importer
} TE3FFormat_Sample_Data;





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
// Globals to hold the type and class assigned by Quesa when registered
TQ3ObjectType	gMyTypeAsRegistered;
TQ3XObjectClass gMyRegisteredClass;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3fformat_sample_canread : Determines the if we can read this data.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_sample_canread(TQ3StorageObject storage, TQ3ObjectType* theFileFormatFound)
{
	TQ3Boolean						result = kQ3False;
	
	// here you have to scan the storage trying to determine if you can read this file
	// We are studying a system to pass some more hints, like the FSSpec or type or extension
	
	
	if (theFileFormatFound == NULL) {
		return (kQ3False);
	}

	*theFileFormatFound = kQ3ObjectTypeInvalid;
	
	/*
		
	if (found magic numbers){
		result = kQ3True;
		*theFileFormatFound = myTypeAsRegistered;
		}

excerpt from the e3fformat_3ds_canread 
	if(Q3Storage_GetData (storage,0, 2,(unsigned char*)&label, &sizeRead) == kQ3Success){

		if (sizeRead != 2)
			return kQ3False;
		if (label == k3dsChunkMain)
			*theFileFormatFound = sRegisteredType;
			result = kQ3True;
		}
		
	*/
	return (result);
}





//=============================================================================
//      e3fformat_sample_read_header : Initialize the reader.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_sample_read_header(TQ3FileObject theFile)
{
	TQ3Boolean						result = kQ3False;
	
	/*
	this is the first FileFormat Object Method Called after instatiating tha Object
	(other that the _new, of course)
	
	it's called by the E3File_OpenRead method, at this time the storage has been attached
	 and the logicalEOF field has beed inited
		
	
	this method is called just after the file has been open for reading,
	its purpose is allow the format to populate internal data, preparing itself
	to respond to a next readObject method call.
	
	You could even read the whole file inside this method, converting it to 
	Quesa objects, that will be returned in the read loop of readObject Calls
	
	Other uses could be doing a first pass in the file to read and build materials 
	that will be attached to objects read in the read object Method.
	
	remember that you could use the GenericRead routines from QuesaIO.h
	
	*/
	
	return (result);
}





//=============================================================================
//      e3fformat_sample_skipobject : Skips the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_sample_skipobject(TQ3FileObject theFile)
{
	/*
	
	in this method you have to discard the next object in file (or in the cached list
	created in the readHeader method)
	
	pay attention at the readInGroup Flag of TQ3FFormatBaseData to know if you have to
	discard just one element or the entire group (the meaning of element or group is up to you)
	
	*/


	return (kQ3Failure);
}





//=============================================================================
//      e3fformat_sample_readobject : Reads the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_sample_readobject(TQ3FileFormatObject format, TQ3FileObject theFile)
{
	/*
	
	in this method you have to return the next object in file (or in the cached list
	created in the readHeader method)
	
	the object has to be completed of attributes,and we have to add a reference to it prior to return
	
	pay attention at the readInGroup Flag of TQ3FFormatBaseData to know if you have to
	discard just one element or the entire group (the meaning of element or group is up to you)
	
	remember that you could use the GenericRead routines from QuesaIO.h
	*/

	return (NULL);
}





//=============================================================================
//      e3fformat_sample_get_nexttype : Returns the type of the next object.
//-----------------------------------------------------------------------------
// 		Note : Doesn't move the storage position.
//-----------------------------------------------------------------------------
static TQ3ObjectType
e3fformat_sample_get_nexttype(TQ3FileObject theFile)
{
 
 /*
 	
	in this method you have to return the Quesa type of the object that will be returned
	by a next call to readObject
	
	pay attention at the readInGroup Flag of TQ3FFormatBaseData to know if you have to
	discard just one element or the entire group (the meaning of element or group is up to you)
	
 */

	return (kQ3ObjectTypeInvalid);
}





//=============================================================================
//      e3fformat_sample_close : frees memory.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_sample_close(TQ3FileFormatObject format, TQ3Boolean abort)
{

/*

	this method is the counterpart of the readHeader one,
	free here the memory allocated by the calls above
	
	this method is the last method called prior deallocating the reader

*/
	return (kQ3Success);

}





//=============================================================================
//      e3fformat_sample_metahandler : Metahandler for 3DMF Text.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_sample_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;

	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeFFormatCanRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_sample_canread;
			break;

		case kQ3XMethodTypeFFormatReadHeader:
			theMethod = (TQ3XFunctionPointer) e3fformat_sample_read_header;
			break;

/*
		
		the use of this method out of the 3DMF readers is still to be determined
		
		case kQ3XMethodTypeFFormatGetFormatType:
			theMethod = (TQ3XFunctionPointer) e3fformat_sample_get_formattype;
			break;
*/

		case kQ3XMethodTypeFFormatReadObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_sample_readobject;
			break;

		case kQ3XMethodTypeFFormatSkipObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_sample_skipobject;
			break;

		case kQ3XMethodTypeFFormatGetNextType:
			theMethod = (TQ3XFunctionPointer) e3fformat_sample_get_nexttype;
			break;

		case kQ3XMethodTypeFFormatClose:
			theMethod = (TQ3XFunctionPointer) e3fformat_sample_close;
			break;

/*

	You don't have to implement these methods unless you would use the Q3XXX_Read calls
	inside the implementation of the methods above


		case kQ3XMethodTypeFFormatFloat32Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_64;
			break;

		case kQ3XMethodTypeFFormatInt8Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_8;
			break;

		case kQ3XMethodTypeFFormatInt16Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_16;
			break;

		case kQ3XMethodTypeFFormatInt32Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_32;
			break;

		case kQ3XMethodTypeFFormatInt64Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_64;
			break;

		case kQ3XMethodTypeFFormatStringRead:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_String;
			break;

		case kQ3XMethodTypeFFormatRawRead:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_Raw;
			break;
*/
		}
	
	return(theMethod);
}





//=============================================================================
//      e3fformat_sample_runtime_registerclass : Register the class.
//-----------------------------------------------------------------------------
static void
e3fformat_sample_runtime_registerclass(void)
{


	// Register the class
	gMyRegisteredClass = Q3XObjectHierarchy_RegisterClass (kQ3FileFormatTypeReader,
										 &gMyTypeAsRegistered, 
										 kQ3ClassNameFileFormatReader_Sample,
										 e3fformat_sample_metahandler,
										 NULL,
										 0,
										 sizeof(TE3FFormat_Sample_Data));
}





//=============================================================================
//      e3fformat_sample_runtime_unregisterclass : Unregister the class.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_sample_runtime_unregisterclass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = Q3XObjectHierarchy_UnregisterClass(gMyRegisteredClass);

	return(qd3dStatus);
}





//=============================================================================
//      Static Registration functions
//-----------------------------------------------------------------------------
//      E3FFormat_Sample_Reader_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#if ((defined QUESA_REGISTER_BUILTIN_PLUGINS) && (QUESA_REGISTER_BUILTIN_PLUGINS == 0)) 

TQ3Status
E3FFormat_Sample_Reader_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kE3FileFormatTypeReader,
											kE3FFormatReaderTypeSample,
											kQ3ClassNameFileFormatReader_Sample,
											e3fformat_sample_metahandler,
											sizeof(TE3FFormat_Sample_Data));

	return (qd3dStatus);
}





//=============================================================================
//      E3FFormat_Sample_Reader_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3FFormat_Sample_Reader_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kE3FFormatReaderTypeSample, kQ3True);

	return(qd3dStatus);
}

#endif





//=============================================================================
//      SampleImporterLoad : CFM entry point for Mac.
//-----------------------------------------------------------------------------
#if OS_MACINTOSH
OSErr
SampleImporterLoad(void)
{


	// Register ourselves	
	e3fformat_sample_runtime_registerclass();

	if (gMyRegisteredClass == NULL)
		return(memFullErr);
	
	return(noErr);
}
#endif





//=============================================================================
//      SampleImporterExit : CFM exit point for Mac.
//-----------------------------------------------------------------------------
#if OS_MACINTOSH
TQ3Int32
SampleImporterExit(void)
{


	// Unregister ourselves
	e3fformat_sample_runtime_unregisterclass();
	return(0);
}
#endif

//=============================================================================
//      DllMain : DLL entry point for Windows.
//-----------------------------------------------------------------------------
#if OS_WIN32
BOOL APIENTRY
DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			e3fformat_sample_runtime_registerclass();
			
			if(gMyRegisteredClass == NULL)
			{
				return FALSE;
			}
			break;
		case DLL_PROCESS_DETACH:
			e3fformat_sample_runtime_unregisterclass();
			break;
    }
    return TRUE;
}

#endif


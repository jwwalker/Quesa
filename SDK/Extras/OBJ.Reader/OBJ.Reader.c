/*  NAME:
        OBJ.Reader.c

    DESCRIPTION:
        Glue for an WaveFront .OBJ Importer Plug-In for Quesa.
        
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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"
#include "QuesaExtension.h"
#include "QuesaGeometry.h"
#include "QuesaMath.h"
#include "QuesaGroup.h"
#include "QuesaIO.h"
#include "QuesaStorage.h"

#include "OBJ.Reader.h"
#include "wavefrontobj.h"

#include <stdlib.h>
#include <string.h>

#if QUESA_OS_WIN32
#endif
//=============================================================================
//      Internal Types
//-----------------------------------------------------------------------------




//=============================================================================
//      Debug Macros
//-----------------------------------------------------------------------------

//=============================================================================
//      Global Variables
//-----------------------------------------------------------------------------


static TQ3ObjectType sRegisteredType;	// global variable to hold the
									// signature assigned by Quesa at register time
static TQ3XObjectClass sRegisteredClass;



//=============================================================================
//      Internal functions (Parser Specific)
//-----------------------------------------------------------------------------




//=============================================================================
//      Internal functions (Quesa FileFormat Glue)
//-----------------------------------------------------------------------------
//      e3fformat_OBJ_canread : Determines the if we can read this data.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_OBJ_canread(TQ3StorageObject storage, TQ3ObjectType* theFileFormatFound)
{

//we have by now no way to determine this just return true

	if (theFileFormatFound == NULL) {
		return (kQ3False);
	}

	*theFileFormatFound = sRegisteredType;

	return kQ3True;
}




//-----------------------------------------------------------------------------
//      e3fformat_OBJ_read_header : Initialize the reader.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_OBJ_read_header(TQ3FileObject theFile)
{

	TQ3FileFormatObject format = Q3File_GetFileFormat (theFile);
	TQ3XObjectClass theFormatClass = Q3XObject_GetClass (format);
	TE3FFormat_OBJ_Data		*instanceData = (TE3FFormat_OBJ_Data *) Q3XObjectClass_GetPrivate(theFormatClass, format);


	instanceData->model = NULL;

	instanceData->gNextVertIsNewGroup = kQ3True;

	instanceData->gNumGroups = 0;
	instanceData->gCurrentGroup = -1;
	
	instanceData->gNumPointsInCurrentGroup = 0;
	instanceData->gPointIndexBase = 1;

	instanceData->gNumUVsInCurrentGroup = 0;
	instanceData->gUVIndexBase = 1;

	instanceData->gNumNormalsInCurrentGroup = 0;
	instanceData->gNormalIndexBase = 1;
	
	instanceData->gTheOBJData = NULL;

		ImportObjFile (instanceData);


	return (kQ3True);
}


//=============================================================================
//      e3fformat_OBJ_skipobject : Skips the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_OBJ_skipobject(TQ3FileObject theFile)
{


	return (kQ3Failure);
}





//=============================================================================
//      e3fformat_OBJ_readobject : Reads the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_OBJ_readobject (TQ3FileObject theFile)
{
	TQ3FileFormatObject format = Q3File_GetFileFormat (theFile);
	TQ3XObjectClass theFormatClass = Q3XObject_GetClass (format);
	TE3FFormat_OBJ_Data		*instanceData = (TE3FFormat_OBJ_Data *) Q3XObjectClass_GetPrivate(theFormatClass, format);
	
	if(instanceData){
		instanceData->baseData.noMoreObjects = kQ3True;

		return (instanceData->model);
		}
	return NULL;
}





//=============================================================================
//      e3fformat_OBJ_get_nexttype : Returns the type of the next object.
//-----------------------------------------------------------------------------
// 		Note : Doesn't move the storage position.
//-----------------------------------------------------------------------------
static TQ3ObjectType
e3fformat_OBJ_get_nexttype(TQ3FileObject theFile)
{
 

	return (kQ3ObjectTypeInvalid);
}





//=============================================================================
//      e3fformat_OBJ_close : frees memory.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_OBJ_close(TQ3FileFormatObject format, TQ3Boolean abort)
{


	return (kQ3Success);

}





//=============================================================================
//      e3fformat_OBJ_metahandler : Metahandler for 3DMF Text.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_OBJ_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;

	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeFFormatCanRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_OBJ_canread;
			break;

		case kQ3XMethodTypeFFormatReadHeader:
			theMethod = (TQ3XFunctionPointer) e3fformat_OBJ_read_header;
			break;


		case kQ3XMethodTypeFFormatReadObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_OBJ_readobject;
			break;

		case kQ3XMethodTypeFFormatSkipObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_OBJ_skipobject;
			break;

		case kQ3XMethodTypeFFormatGetNextType:
			theMethod = (TQ3XFunctionPointer) e3fformat_OBJ_get_nexttype;
			break;

		case kQ3XMethodTypeFFormatClose:
			theMethod = (TQ3XFunctionPointer) e3fformat_OBJ_close;
			break;


#if QUESA_HOST_IS_BIG_ENDIAN


		case kQ3XMethodTypeFFormatFloat32Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinSwap_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinSwap_64;
			break;

		case kQ3XMethodTypeFFormatInt16Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinSwap_16;
			break;

		case kQ3XMethodTypeFFormatInt32Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinSwap_32;
			break;

		case kQ3XMethodTypeFFormatInt64Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinSwap_64;
			break;

#else

		case kQ3XMethodTypeFFormatFloat32Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_64;
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


#endif
		case kQ3XMethodTypeFFormatInt8Read:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_8;
			break;

		case kQ3XMethodTypeFFormatStringRead:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_String;
			break;

		case kQ3XMethodTypeFFormatRawRead:
			theMethod = (TQ3XFunctionPointer) Q3FileFormat_GenericReadBinary_Raw;
			break;


		}
	
	return(theMethod);
}


//-----------------------------------------------------------------------------
//      e3fformat_OBJ_runtime_registerclass : Register the classes.
//-----------------------------------------------------------------------------
static void
e3fformat_OBJ_runtime_registerclass(void)
{
	

	sRegisteredClass = Q3XObjectHierarchy_RegisterClass (kQ3FileFormatTypeReader,
										 &sRegisteredType, 
										 kQ3ClassNameFileFormatReader_OBJ,
										 e3fformat_OBJ_metahandler,
										 NULL,
										 0,
										 sizeof(TE3FFormat_OBJ_Data));
										 		

}





//=============================================================================
//      e3fformat_OBJ_runtime_unregisterclass : Unregister the classes.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_OBJ_runtime_unregisterclass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes


	qd3dStatus = Q3XObjectHierarchy_UnregisterClass(sRegisteredClass);

	return(qd3dStatus);
}






//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFormat_Sample_Reader_RegisterClass : Register the classes.
//-----------------------------------------------------------------------------
#if ((defined QUESA_REGISTER_BUILTIN_PLUGINS) && (QUESA_REGISTER_BUILTIN_PLUGINS == 0)) 
TQ3Status
E3FFormat_OBJ_Reader_RegisterClass(void)
{	TQ3Status		qd3dStatus;
	

	qd3dStatus = E3ClassTree_RegisterClass(kQ3FileFormatTypeReader,
											kE3FFormatReaderTypeOBJ,
											kQ3ClassNameFileFormatReader_OBJ,
											e3fformat_OBJ_metahandler,
											sizeof(TE3FFormat_OBJ_Data));

	return (qd3dStatus);
}





//=============================================================================
//      E3FFormat_OBJ_Reader_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FFormat_OBJ_Reader_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes


	qd3dStatus = E3ClassTree_UnregisterClass(kE3FFormatReaderTypeOBJ, kQ3True);


	return(qd3dStatus);
}

#endif

//=============================================================================
//      E3FFormat_OBJ_Reader_Entry : CFM entry point for Mac.
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
OSErr
E3FFormat_OBJ_Reader_Entry(void)
{	
	e3fformat_OBJ_runtime_registerclass();
	
	if(sRegisteredClass == NULL)
	{
		return kQ3Failure;
	}
	
	return noErr;

}
#endif



//=============================================================================
//      E3FFormat_OBJ_Reader_Exit : CFM exit point for Mac.
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
TQ3Int32
E3FFormat_OBJ_Reader_Exit(void)
{

	e3fformat_OBJ_runtime_unregisterclass();

	return 0;
}

#endif

//=============================================================================
//      DllMain : DLL entry point for Windows.
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32
BOOL APIENTRY
DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			e3fformat_OBJ_runtime_registerclass();
			
			if(sRegisteredClass == NULL)
			{
				return FALSE;
			}
			break;
		case DLL_PROCESS_DETACH:
			e3fformat_OBJ_runtime_unregisterclass();
			break;
    }
    return TRUE;
}

#endif

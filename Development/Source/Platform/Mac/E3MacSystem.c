/*  NAME:
        E3MacSystem.c

    DESCRIPTION:
        Mac specific routines.

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
#include "E3Prefix.h"
#include "E3System.h"

#if QUESA_UH_IN_FRAMEWORKS
	#include <Carbon/Carbon.h>
#else
	#include <Aliases.h>
	#include <CodeFragments.h>
	#include <Folders.h>
	#include <Processes.h>
	#include <Resources.h>
#endif

#if TARGET_RT_MAC_MACHO
	#include <AGL/agl.h>
#else
	#include <agl.h>
#endif

#if Q3_PROFILE
	#include <Profiler.h>
#endif





//=============================================================================
//      Function prototyes
//-----------------------------------------------------------------------------
pascal OSErr E3MacCFM_Initialise(const CFragInitBlock *initBlock);
pascal OSErr E3MacCFM_Terminate(void);





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct E3MacSystem_CFMSlot *E3MacSystem_CFMSlotPtr;

typedef struct E3MacSystem_CFMSlot {
	CFragConnectionID			CFM_ID;
	E3MacSystem_CFMSlotPtr		nextSlot;
} E3MacSystem_CFMSlot;




//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
static AliasHandle gQuesaLib = NULL;
short gShlbResFile = 0;
static E3MacSystem_CFMSlotPtr e3macsystem_cfmSlotHead = NULL;




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3mac_load_plugin : Load a plug-in.
//-----------------------------------------------------------------------------
static void
e3mac_load_plugin(const FSSpec *theFSSpec)
{	Ptr					mainAddr;
	Str255				theStr;
	OSErr				theErr;
	short				oldResFile;
	E3MacSystem_CFMSlotPtr newSlot = NULL;


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theFSSpec));





	newSlot = (E3MacSystem_CFMSlotPtr)Q3Memory_Allocate(sizeof(E3MacSystem_CFMSlot));
	if(newSlot != NULL){
		oldResFile = CurResFile();


		// Load the plug-in. Note that we just need to open a connection
		// to the fragment, and the plug-ins CFM initialisation routine
		// will register any Quesa objects with the system.
		theErr = GetDiskFragment(theFSSpec, 0, kCFragGoesToEOF, "\p",
								 kPrivateCFragCopy, &newSlot->CFM_ID,
								 &mainAddr, theStr);
		if((theErr == noErr) && (newSlot->CFM_ID != NULL)){
			newSlot->nextSlot = e3macsystem_cfmSlotHead;
			e3macsystem_cfmSlotHead = newSlot;
		}
		else{//GetDiskFragment failed
			Q3Memory_Free(&newSlot);
		}
		
		// In case the plug-in's initialization routine changed the resource file
		UseResFile( oldResFile );
	}
}





//=============================================================================
//      e3mac_load_plugins : Scan a directory for plug-ins and load them.
//-----------------------------------------------------------------------------
//		Note :	Although it makes the interface a little unusual, the FSSpec
//				that is supplied is to a file contained in the directory to
//				be scanned (e.g., passing an FSSpec to the application would
//				cause the application directory to be scanned).
//
//				This simplifies the code which invokes us (since it starts with
//				an FSSpec to a file in a directory to be scanned), and our own
//				implementation (since we need the directory ID of the directory
//				to scan, which we'd have to recalculate ourselves if we were
//				passed an FSSpec to the directory to scan).
//-----------------------------------------------------------------------------
static void
e3mac_load_plugins(const FSSpec *fileInDirToScan)
{	Boolean			targetIsFolder;
	Boolean			wasAliased;
	FSSpec			theFSSpec;
	TQ3Int16		theIndex;
	OSErr			theErr;
	CInfoPBRec		thePB;



	// Iterate through the directory, looking for plug-ins
	theIndex = 1;
	do
		{
		// Get the next file
		thePB.dirInfo.ioFDirIndex = theIndex;
		thePB.dirInfo.ioVRefNum   = fileInDirToScan->vRefNum;
		thePB.dirInfo.ioDrDirID   = fileInDirToScan->parID;
		thePB.dirInfo.ioNamePtr   = theFSSpec.name;
		
		theErr = PBGetCatInfoSync(&thePB);

		if (theErr == noErr)
			{
			// Grab the file
			theErr = FSMakeFSSpec(fileInDirToScan->vRefNum, fileInDirToScan->parID, theFSSpec.name, &theFSSpec);
			if (theErr == noErr)
				{
				// Resolve any aliases
				targetIsFolder = true;
				ResolveAliasFile(&theFSSpec, true, &targetIsFolder, &wasAliased);


				// If this isn't a directory, check the type
				if (!targetIsFolder)
					{
					// If this is a plug-in, load it
					if (thePB.hFileInfo.ioFlFndrInfo.fdType    == kQ3XExtensionMacFileType &&
						thePB.hFileInfo.ioFlFndrInfo.fdCreator == kQ3XExtensionMacCreatorType)
						e3mac_load_plugin(&theFSSpec);
					}
				}
			
			
			// Clear any error - don't stop if we hit an error at this point
			theErr = noErr;
			}
		
		theIndex++;
		}
	while (theErr == noErr);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3MacCFM_Initialise : CFM initialise routine.
//-----------------------------------------------------------------------------
#pragma mark -
pascal OSErr
E3MacCFM_Initialise(const CFragInitBlock *initBlock)
{	OSErr	theErr;



	// Save an alias to our FSSpec for later
	if (initBlock->fragLocator.where == kDataForkCFragLocator)
		theErr = NewAlias(NULL, initBlock->fragLocator.u.onDisk.fileSpec, &gQuesaLib);
	else	
		theErr = noErr;

	// Get the resource fork reference, save that for later too
	if (initBlock->fragLocator.where == kDataForkCFragLocator)
		{
		short oldResFile = CurResFile();
		gShlbResFile = FSpOpenResFile(initBlock->fragLocator.u.onDisk.fileSpec, fsRdPerm);
		UseResFile(oldResFile);
		}

	return(theErr);
}





//=============================================================================
//      E3MacCFM_Terminate : CFM termination routine.
//-----------------------------------------------------------------------------
pascal OSErr
E3MacCFM_Terminate(void)
{


	// If Quesa has not been shut down yet, it probably means that the
	// application crashed.  In that case, any remaining objects probably
	// don't constitute true leaks.
	#if QUESA_ALLOW_QD3D_EXTENSIONS && Q3_DEBUG
	if ( Q3IsInitialized() )
	{
		Q3Memory_StopRecording();
	}
	#endif



	// Make sure Quesa has been shut down by the app. We keep polling
	// Q3IsInitialized until it returns false, to make sure we decrement
	// the system reference count down to 0.
	while (Q3IsInitialized())
		Q3Exit();



	// Dispose of our alias
	if (gQuesaLib != NULL)
		{
		DisposeHandle((Handle) gQuesaLib);
		gQuesaLib = NULL;
		}

	return(noErr);
}





//=============================================================================
//      E3MacSystem_Initialise : Initialise the system.
//-----------------------------------------------------------------------------
TQ3Status
E3MacSystem_Initialise(void)
{


	// If we're profiling, initialise the profiler
#if Q3_PROFILE
	OSErr theErr = ProfilerInit(collectDetailed, bestTimeBase, 30000, 2000);
	if (theErr != noErr)
		return(kQ3Failure);
#endif

	return(kQ3Success);
}





//=============================================================================
//      E3MacSystem_Terminate : Terminate the system.
//-----------------------------------------------------------------------------
void
E3MacSystem_Terminate(void)
{



	// If we're profiling, dump the results and terminate the profiler
#if Q3_PROFILE
	ProfilerDump("\pQuesa Profile Log");
	ProfilerTerm();
#endif



	// Shut down OpenGL. This fixes a crash on exit on some apps on Mac OS 9
	// if they exit without destroying renderer objects that use OpenGL.
	//
	// Since we may not have any renderers which use OpenGL, we assume that
	// we've been weak linked and test for the symbol first.
	if ((TQ3Uns32) aglResetLibrary != (TQ3Uns32) kUnresolvedCFragSymbolAddress)
		aglResetLibrary();
}





//=============================================================================
//      E3MacSystem_LoadPlugins : Scan for and load plug-ins.
//-----------------------------------------------------------------------------
//		Note :	Plug-ins are loaded by scanning the application directory,
//				the Quesa library directory, and the Extensions folder.
//
//				Note that we deliberately pass e3mac_load_plugins an FSSpec to
//				a file within the directory to scan. This is deliberate.
//-----------------------------------------------------------------------------
void
E3MacSystem_LoadPlugins(void)
{	FSSpec					appFSSpec, quesaFSSpec, extensionsFSSpec;
	ProcessInfoRec			processInfo;
	Boolean					wasChanged;
	ProcessSerialNumber		thePSN = { kNoProcess, kCurrentProcess };
	OSErr					theErr;



	// Initialise ourselves
	memset(&appFSSpec,        0x00, sizeof(appFSSpec));
	memset(&quesaFSSpec,      0x00, sizeof(quesaFSSpec));
	memset(&extensionsFSSpec, 0x00, sizeof(extensionsFSSpec));



	// Find the application file
	processInfo.processInfoLength = sizeof(ProcessInfoRec);
	processInfo.processName       = (StringPtr) &appFSSpec.name;
	processInfo.processAppSpec    = &appFSSpec;
	
	theErr = GetProcessInformation(&thePSN, &processInfo);



	// Find the Quesa shared library file
	if (gQuesaLib != NULL)
		theErr = ResolveAlias(NULL, gQuesaLib, &quesaFSSpec, &wasChanged);



	// Find a file (any file) within the Extensions folder
	theErr = FindFolder(kOnSystemDisk, kExtensionFolderType, true,
						&extensionsFSSpec.vRefNum, &extensionsFSSpec.parID);
	if (theErr == noErr)
		{
		extensionsFSSpec.name[0] = 1;
		extensionsFSSpec.name[1] = 'Q';
		}



	// Reset any duplicate FSSpecs to avoid multiple re-scans. Note that
	// we only care about the vRefNum and parID fields of the FSSpec.
	if (memcmp(&appFSSpec, &quesaFSSpec, sizeof(short) + sizeof(long)) == 0)
		memset(&quesaFSSpec, 0x00, sizeof(quesaFSSpec));

	if (memcmp(&appFSSpec, &extensionsFSSpec, sizeof(short) + sizeof(long)) == 0)
		memset(&extensionsFSSpec, 0x00, sizeof(extensionsFSSpec));

	if (memcmp(&quesaFSSpec, &extensionsFSSpec, sizeof(short) + sizeof(long)) == 0)
		memset(&extensionsFSSpec, 0x00, sizeof(extensionsFSSpec));



	// Scan for and load our plug-ins
	if (appFSSpec.name[0] != 0x00)
		e3mac_load_plugins(&appFSSpec);

	if (quesaFSSpec.name[0] != 0x00)
		e3mac_load_plugins(&quesaFSSpec);

	if (extensionsFSSpec.name[0] != 0x00)
		e3mac_load_plugins(&extensionsFSSpec);
}





//=============================================================================
//      E3MacSystem_UnloadPlugins : unload plug-ins.
//-----------------------------------------------------------------------------
void
E3MacSystem_UnloadPlugins(void)
{
		E3MacSystem_CFMSlotPtr nextSlot;
		E3MacSystem_CFMSlotPtr currentSlot;

		nextSlot = e3macsystem_cfmSlotHead;

		while( nextSlot != NULL){
			currentSlot = nextSlot;
			nextSlot = currentSlot->nextSlot;
			CloseConnection(&currentSlot->CFM_ID);
			Q3Memory_Free(&currentSlot);
		}
		
	e3macsystem_cfmSlotHead = NULL;
}

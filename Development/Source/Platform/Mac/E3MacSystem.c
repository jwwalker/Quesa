/*  NAME:
        E3MacSystem.c

    DESCRIPTION:
        Mac specific routines.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
//      Internal globals
//-----------------------------------------------------------------------------
static AliasHandle gQuesaLib = NULL;
short gShlbResFile = 0;




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3mac_load_plugin : Load a plug-in.
//-----------------------------------------------------------------------------
static void
e3mac_load_plugin(const FSSpec *theFSSpec)
{	CFragConnectionID	theConnection;
	Ptr					mainAddr;
	Str255				theStr;
	OSErr				theErr;
	


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theFSSpec));



	// Load the plug-in. Note that we just need to open a connection
	// to the fragment, and the plug-ins CFM initialisation routine
	// will register any Quesa objects with the system.
	theErr = GetDiskFragment(theFSSpec, 0, kCFragGoesToEOF, "\p",
							 kPrivateCFragCopy, &theConnection,
							 &mainAddr, theStr);
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
	TQ3Uns32		theIndex;
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
	ProcessSerialNumber		thePSN;
	OSErr					theErr;



	// Initialise ourselves
	memset(&appFSSpec,        0x00, sizeof(appFSSpec));
	memset(&quesaFSSpec,      0x00, sizeof(quesaFSSpec));
	memset(&extensionsFSSpec, 0x00, sizeof(extensionsFSSpec));



	// Find the application file
	processInfo.processInfoLength = sizeof(ProcessInfoRec);
	processInfo.processName       = (StringPtr) &appFSSpec.name;
	processInfo.processAppSpec    = &appFSSpec;
	
	theErr = GetCurrentProcess(&thePSN);
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
	// using memcmp on Pascal strings would not normally be valid, but
	// we initialised our FSSpecs with 0s so unused bytes will be equal.
	if (memcmp(&appFSSpec, &quesaFSSpec, sizeof(FSSpec)) == 0)
		memset(&quesaFSSpec, 0x00, sizeof(quesaFSSpec));

	if (memcmp(&appFSSpec, &extensionsFSSpec, sizeof(FSSpec)) == 0)
		memset(&extensionsFSSpec, 0x00, sizeof(extensionsFSSpec));

	if (memcmp(&quesaFSSpec, &extensionsFSSpec, sizeof(FSSpec)) == 0)
		memset(&extensionsFSSpec, 0x00, sizeof(extensionsFSSpec));



	// Scan for and load our plug-ins
	if (appFSSpec.name[0] != 0x00)
		e3mac_load_plugins(&appFSSpec);

	if (quesaFSSpec.name[0] != 0x00)
		e3mac_load_plugins(&quesaFSSpec);

	if (extensionsFSSpec.name[0] != 0x00)
		e3mac_load_plugins(&extensionsFSSpec);
}

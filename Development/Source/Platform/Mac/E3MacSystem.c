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
#include "E3System.h"

#if QUESA_UH_IN_FRAMEWORKS
	#include <Carbon/Carbon.h>
#else
	#include <Aliases.h>
	#include <CFBundle.h>
	#include <CFURL.h>
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
Q3_EXTERN_API_C(void) E3MacMachoFrameworkInit();
Q3_EXTERN_API_C(void) E3MacMachoFrameworkTerminate();





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct E3MacSystem_PluginSlot *E3MacSystem_PluginSlotPtr;

typedef struct E3MacSystem_PluginSlot {
#if TARGET_RT_MAC_MACHO
	CFBundleRef					pluginBundle;
#else
	CFragConnectionID			CFM_ID;
#endif
	E3MacSystem_PluginSlotPtr	nextSlot;
} E3MacSystem_PluginSlot;




//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
static AliasHandle gQuesaLib = NULL;
short gShlbResFile = 0;
static E3MacSystem_PluginSlotPtr e3macsystem_pluginSlotHead = NULL;




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------

#if TARGET_RT_MAC_MACHO
//-----------------------------------------------------------------------------
//      e3macho_load_plugin : Load a CFM plug-in.
//-----------------------------------------------------------------------------
static void e3macho_load_plugin( CFBundleRef theBundle )
{
	E3MacSystem_PluginSlotPtr newSlot = NULL;
	short oldResFile = CurResFile();
	
	// Load the plugin, which causes the function marked as CALL_ON_LOAD
	// to be called.
	if (CFBundleLoadExecutable( theBundle ))
	{
		newSlot = (E3MacSystem_PluginSlotPtr)Q3Memory_Allocate(sizeof(E3MacSystem_PluginSlot));
		
		if (newSlot != NULL)
		{
			newSlot->pluginBundle = theBundle;
			CFRetain( theBundle );
			newSlot->nextSlot = e3macsystem_pluginSlotHead;
			e3macsystem_pluginSlotHead = newSlot;
		}
	}
	
	// In case the plug-in's initialization routine changed the resource file
	UseResFile( oldResFile );
}

#else	// CFM
//-----------------------------------------------------------------------------
//      e3mac_load_plugin : Load a CFM plug-in.
//-----------------------------------------------------------------------------
static void
e3mac_load_plugin(const FSSpec *theFSSpec)
{	Ptr					mainAddr;
	Str255				theStr;
	OSErr				theErr;
	short				oldResFile;
	E3MacSystem_PluginSlotPtr newSlot = NULL;


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theFSSpec));



	newSlot = (E3MacSystem_PluginSlotPtr)Q3Memory_Allocate(sizeof(E3MacSystem_PluginSlot));
	if(newSlot != NULL){
		oldResFile = CurResFile();


		// Load the plug-in. Note that we just need to open a connection
		// to the fragment, and the plug-ins CFM initialisation routine
		// will register any Quesa objects with the system.
		theErr = GetDiskFragment(theFSSpec, 0, kCFragGoesToEOF, "\p",
								 kPrivateCFragCopy, &newSlot->CFM_ID,
								 &mainAddr, theStr);
		if((theErr == noErr) && (newSlot->CFM_ID != NULL)){
			newSlot->nextSlot = e3macsystem_pluginSlotHead;
			e3macsystem_pluginSlotHead = newSlot;
		}
		else{//GetDiskFragment failed
			Q3Memory_Free(&newSlot);
		}
		
		// In case the plug-in's initialization routine changed the resource file
		UseResFile( oldResFile );
	}
}
#endif




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
{
	OSErr			theErr;
#if TARGET_RT_MAC_MACHO
	FSSpec			dirSpec;
	FSRef			dirRef;
	CFURLRef		dirURL = NULL;
	CFArrayRef		pluginsArray = NULL;
	CFIndex			numPlugins;
	CFIndex			i;
	CFBundleRef		plugBundle = NULL;
	
	// Convert vRefNum and dirID to a URL.
	theErr = FSMakeFSSpec( fileInDirToScan->vRefNum, fileInDirToScan->parID, "\p", &dirSpec );
	if (theErr == noErr)
		theErr = FSpMakeFSRef( &dirSpec, &dirRef );
	if (theErr == noErr)
		dirURL = CFURLCreateFromFSRef( NULL, &dirRef );
	
	// Look for plugins in the directory.
	if (dirURL != NULL)
	{
		pluginsArray = CFBundleCreateBundlesFromDirectory( NULL, dirURL, CFSTR("quesaplug") );
		
		if (pluginsArray != NULL)
		{
			numPlugins = CFArrayGetCount( pluginsArray );
			
			for (i = 0; i < numPlugins; ++i)
			{
				plugBundle = (CFBundleRef) CFArrayGetValueAtIndex( pluginsArray, i );
				e3macho_load_plugin( plugBundle );
			}
			
			CFRelease( pluginsArray );
		}
		
		CFRelease( dirURL );
	}
	
#else	// CFM
	Boolean			targetIsFolder;
	Boolean			wasAliased;
	FSSpec			theFSSpec;
	TQ3Int16		theIndex;
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
				ResolveAliasFileWithMountFlags( &theFSSpec, true,
					&targetIsFolder, &wasAliased, kResolveAliasFileNoUI );


				// If this isn't a directory, check the type
				if (!targetIsFolder)
					{
					if (wasAliased)
						{
						// Don't rely on the alias file having the right file type and creator.
						FInfo	finderInfo;
						FSpGetFInfo( &theFSSpec, &finderInfo );
						thePB.hFileInfo.ioFlFndrInfo.fdType = finderInfo.fdType;
						thePB.hFileInfo.ioFlFndrInfo.fdCreator = finderInfo.fdCreator;
						}
					
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
#endif
}





//=============================================================================
//      e3mac_find_folder_spec : Find a standard directory.
//-----------------------------------------------------------------------------
//	We are using an FSSpec as a convenient holder for a vRefNum and dirID.
//	The name field is irrelevant.
static OSErr
e3mac_find_folder_spec( short vRefNum, OSType folderType, FSSpec* outSpec )
{
	OSErr	theErr = FindFolder( vRefNum, folderType, kDontCreateFolder,
						&outSpec->vRefNum, &outSpec->parID );
	
	return theErr;
}





#pragma mark -
//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------

#if TARGET_RT_MAC_MACHO
//-----------------------------------------------------------------------------
//      E3MacMachoFrameworkInit : Mach-o initialise routine.
//-----------------------------------------------------------------------------
void E3MacMachoFrameworkInit()
{
	CFBundleRef		myBundle = CFBundleGetBundleWithIdentifier( CFSTR("org.Quesa.Quesa") );
	
	if (myBundle != NULL)
	{
		// Note: leave the gQuesaLib AliasHandle as NULL.  The only reason we were
		// getting the alias to the shared library was so that we could look for
		// plug-ins in the same directory.  However, we do not expect to find plug-ins
		// in the same location as a framework.
		
		short oldResFile = CurResFile();
		gShlbResFile = CFBundleOpenBundleResourceMap( myBundle );
		UseResFile(oldResFile);
	}
}





//-----------------------------------------------------------------------------
//      E3MacMachoFrameworkTerminate : Mach-o terminate routine.
//-----------------------------------------------------------------------------
#pragma CALL_ON_UNLOAD	E3MacMachoFrameworkTerminate
void E3MacMachoFrameworkTerminate()
{
	// Close the resource file.
	CFBundleRef		myBundle = CFBundleGetBundleWithIdentifier( CFSTR("org.Quesa.Quesa") );
	
	if ( (myBundle != NULL) && (gShlbResFile != 0) && (gShlbResFile != -1) )
	{
		CFBundleCloseBundleResourceMap( myBundle, gShlbResFile );
	}



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
	
}




#else	// CFM
//-----------------------------------------------------------------------------
//      E3MacCFM_Initialise : CFM initialise routine.
//-----------------------------------------------------------------------------
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
#endif





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
//		Note :	Plug-ins are loaded by scanning certain directories.
//
//				In every case, we scan the directory containing the Quesa library
//				and the directory containing the host application.
//
//				On OS 9, we also scan the Extensions folder.
//
//				On OS X, we also scan the /Library/CFMSupport and ~/Library/CFMSupport
//				directories, and the bundle Plugins folder if the host application
//				is bundled.
//
//				Note that we deliberately pass e3mac_load_plugins an FSSpec to
//				a file within the directory to scan. This is deliberate.
//-----------------------------------------------------------------------------
void
E3MacSystem_LoadPlugins(void)
{
	FSSpec					fileSpec[6];
	int						dirCount = 0;
	ProcessInfoRec			processInfo;
	Boolean					wasChanged, isOnOSX;
	ProcessSerialNumber		thePSN = { kNoProcess, kCurrentProcess };
	OSStatus				theErr;
	long					sysVersion;
	int						i, j;
#if TARGET_API_MAC_CARBON
	CFBundleRef				myBundle;
	FSCatalogInfo			catInfo;
	FSRef					bundleFSRef;
#endif



	// Initialise ourselves
	isOnOSX = (Gestalt(gestaltSystemVersion, &sysVersion) == noErr) &&
		(sysVersion >= 0x00001000);



	// Find the Quesa shared library file
	if (gQuesaLib != NULL)
		{
		theErr = ResolveAlias(NULL, gQuesaLib, &fileSpec[ dirCount ], &wasChanged);
		if (theErr == noErr)
			++dirCount;
		}



	// Find the application file
#if TARGET_API_MAC_CARBON
	if (isOnOSX)
		{
		theErr = GetProcessBundleLocation( &thePSN, &bundleFSRef );
		
		if (theErr == noErr)
			{
			theErr = FSGetCatalogInfo( &bundleFSRef, 0, NULL, NULL, &fileSpec[ dirCount ], NULL );
			}
		}
	else
#endif
		{
		processInfo.processInfoLength = sizeof(ProcessInfoRec);
		processInfo.processName       = NULL;
		processInfo.processAppSpec    = &fileSpec[ dirCount ];
		
		theErr = GetProcessInformation(&thePSN, &processInfo);
		}
	if (theErr == noErr)
		++dirCount;



#if TARGET_API_MAC_CARBON
	if (isOnOSX)
		{
		// ~/Library/CFMSupport
		theErr = e3mac_find_folder_spec( kUserDomain, kSharedLibrariesFolderType,
			&fileSpec[ dirCount ] );
		if (theErr == noErr)
			++dirCount;
		
		// /Library/CFMSupport
		theErr = e3mac_find_folder_spec( kLocalDomain, kSharedLibrariesFolderType,
			&fileSpec[ dirCount ] );
		if (theErr == noErr)
			++dirCount;
		
		// Plugins folder of bundle
		myBundle = CFBundleGetMainBundle();
		if (myBundle != NULL)
			{
			CFURLRef	pluginsURL = CFBundleCopyBuiltInPlugInsURL( myBundle );
			if (pluginsURL != NULL)
				{
				FSRef	dirRef;
				if (CFURLGetFSRef( pluginsURL, &dirRef ))
					{
					theErr = FSGetCatalogInfo( &dirRef, kFSCatInfoVolume | kFSCatInfoNodeID,
						&catInfo, NULL, NULL, NULL );
					if (theErr == noErr)
						{
						fileSpec[ dirCount ].vRefNum = catInfo.volume;
						fileSpec[ dirCount ].parID = catInfo.nodeID;
						++dirCount;
						}
					}
				CFRelease( pluginsURL );
				}
			}
		}
	else
#endif
		{	// OS 9 Extensions folder
		theErr = e3mac_find_folder_spec( kOnSystemDisk, kExtensionFolderType,
			&fileSpec[ dirCount ] );
		if (theErr == noErr)
			++dirCount;
		}



	// Reset any duplicate FSSpecs to avoid multiple re-scans. Note that
	// we only care about the vRefNum and parID fields of the FSSpec.
	for (i = 0; i < dirCount; ++i)
		{
		for (j = i + 1; j < dirCount; ++j)
			{
			if ( (fileSpec[i].parID == fileSpec[j].parID) &&
				(fileSpec[i].vRefNum == fileSpec[j].vRefNum) )
				{
				fileSpec[j].parID = 0;
				}
			}
		}



	// Scan for and load our plug-ins
	for (i = 0; i < dirCount; ++i)
		{
		if (fileSpec[i].parID != 0)
			{
			e3mac_load_plugins( &fileSpec[i] );
			}
		}
}





//=============================================================================
//      E3MacSystem_UnloadPlugins : unload plug-ins.
//-----------------------------------------------------------------------------
void
E3MacSystem_UnloadPlugins(void)
{
		E3MacSystem_PluginSlotPtr nextSlot;
		E3MacSystem_PluginSlotPtr currentSlot;

		nextSlot = e3macsystem_pluginSlotHead;

		while( nextSlot != NULL){
			currentSlot = nextSlot;
			nextSlot = currentSlot->nextSlot;
		#if TARGET_RT_MAC_MACHO
			CFBundleUnloadExecutable( currentSlot->pluginBundle );
			CFRelease( currentSlot->pluginBundle );
		#else
			CloseConnection(&currentSlot->CFM_ID);
		#endif
			Q3Memory_Free(&currentSlot);
		}
		
	e3macsystem_pluginSlotHead = NULL;
}

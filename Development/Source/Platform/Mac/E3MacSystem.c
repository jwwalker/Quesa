/*  NAME:
        E3MacSystem.c

    DESCRIPTION:
        Mac specific routines.

    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

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
	#include <Finder.h>
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
//      Constants
//-----------------------------------------------------------------------------

const int kMaxPluginLocations = 6;

const ItemCount	kPluginSearchBatchSize	= 10;




//=============================================================================
//      Function prototyes
//-----------------------------------------------------------------------------
#if TARGET_RT_MAC_CFM
pascal OSErr E3MacCFM_Initialise(const CFragInitBlock *initBlock);
pascal OSErr E3MacCFM_Terminate(void);

#elif TARGET_RT_MAC_MACHO

#ifdef __cplusplus
extern "C" {
#endif

Q3_EXTERN_API_C(void) E3MacMachoFrameworkInit();

#if __MWERKS__
	Q3_EXTERN_API_C(void) E3MacMachoFrameworkTerminate();
#else
	Q3_EXTERN_API_C(void) E3MacMachoFrameworkTerminate() __attribute__((destructor));
#endif

#ifdef __cplusplus
}
#endif

#endif




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
#if TARGET_RT_MAC_CFM
static AliasHandle gQuesaLib = NULL;
#endif

short gShlbResFile = 0;

static E3MacSystem_PluginSlotPtr e3macsystem_pluginSlotHead = NULL;




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------

#if TARGET_RT_MAC_MACHO
//-----------------------------------------------------------------------------
//      e3macho_load_plugin : Load a Mach-o plug-in.
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
//      e3mac_load_cfm_plugin : Load a CFM plug-in.
//-----------------------------------------------------------------------------
static void
e3mac_load_cfm_plugin(const FSRef& theFileRef )
{	Ptr					mainAddr;
	Str255				theStr;
	OSErr				theErr;
	short				oldResFile;
	E3MacSystem_PluginSlotPtr newSlot = NULL;
	FSSpec				fileSpec;


	FSGetCatalogInfo( &theFileRef, kFSCatInfoNone, NULL, NULL, &fileSpec, NULL );
	

	newSlot = (E3MacSystem_PluginSlotPtr)Q3Memory_Allocate(sizeof(E3MacSystem_PluginSlot));
	if (newSlot != NULL)
	{
		oldResFile = CurResFile();


		// Load the plug-in. Note that we just need to open a connection
		// to the fragment, and the plug-ins CFM initialisation routine
		// will register any Quesa objects with the system.
		theErr = GetDiskFragment( &fileSpec, 0, kCFragGoesToEOF, "\p",
								 kPrivateCFragCopy, &newSlot->CFM_ID,
								 &mainAddr, theStr);
		if ((theErr == noErr) && (newSlot->CFM_ID != NULL))
		{
			newSlot->nextSlot = e3macsystem_pluginSlotHead;
			e3macsystem_pluginSlotHead = newSlot;
		}
		else
		{//GetDiskFragment failed
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
static void
e3mac_load_plugins(const FSRef& dirToScan )
{
	OSErr			theErr;

#if TARGET_RT_MAC_MACHO
	// Convert FSRef to a URL.
	CFURLRef dirURL = CFURLCreateFromFSRef( NULL, &dirToScan );
	
	// Look for plugins in the directory.
	if (dirURL != NULL)
	{
		CFArrayRef pluginsArray = CFBundleCreateBundlesFromDirectory( NULL,
			dirURL, CFSTR("quesaplug") );
		
		if (pluginsArray != NULL)
		{
			CFIndex	numPlugins = CFArrayGetCount( pluginsArray );
			
			for (CFIndex i = 0; i < numPlugins; ++i)
			{
				CFBundleRef plugBundle = (CFBundleRef) CFArrayGetValueAtIndex(
					pluginsArray, i );
				e3macho_load_plugin( plugBundle );
			}
			
			CFRelease( pluginsArray );
		}
		
		CFRelease( dirURL );
	}
	
#else	// CFM

	// Create an iterator
	FSIterator		dirIterator = NULL;
	theErr = FSOpenIterator( &dirToScan, kFSIterateFlat, &dirIterator );
	
	if (theErr == noErr)
	{
		FSRef	fileRefs[ kPluginSearchBatchSize ];
		FSCatalogInfo	catInfos[ kPluginSearchBatchSize ];
		ItemCount	batchSizeReturned;
		Boolean			targetIsFolder;
		Boolean			wasAliased;
		
		do
		{
			batchSizeReturned = 0;
			theErr = FSGetCatalogInfoBulk( dirIterator, kPluginSearchBatchSize,
				&batchSizeReturned, NULL, kFSCatInfoFinderInfo, catInfos,
				fileRefs, NULL, NULL );
			for (ItemCount i = 0; i < batchSizeReturned; ++i)
			{
				// Resolve an alias, if it is an alias
				// This requires OS 9.1 or better
				targetIsFolder = false;
				wasAliased = false;
				if (FSResolveAliasFileWithMountFlags != NULL)
				{
					FSResolveAliasFileWithMountFlags( &fileRefs[ i ], true,
						&targetIsFolder, &wasAliased, kResolveAliasFileNoUI );
				}
				if (!targetIsFolder)
				{
					if (wasAliased)
					{
						// Get the Finder info of the target of the alias
						FSGetCatalogInfo( &fileRefs[ i ], kFSCatInfoFinderInfo,
							&catInfos[ i ], NULL, NULL, NULL );
					}
					const FileInfo&	finderInfo( *reinterpret_cast<FileInfo*>(
						catInfos[i].finderInfo ) );
					
					if ( (finderInfo.fileType == kQ3XExtensionMacFileType) &&
						(finderInfo.fileCreator == kQ3XExtensionMacCreatorType) )
					{
						e3mac_load_cfm_plugin( fileRefs[i] );
					}
				}
			}
		} while (theErr == noErr);
		
		FSCloseIterator( dirIterator );
	}
#endif
}





#pragma mark -
//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------

#if TARGET_RT_MAC_MACHO
//-----------------------------------------------------------------------------
//      E3MacMachoFrameworkInit : Mach-o initialise routine.
//
//	This is designated at the initiailization function in the Xcode build settings
//	(INIT_ROUTINE).
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
#if __MWERKS__
#pragma CALL_ON_UNLOAD	E3MacMachoFrameworkTerminate
#endif
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




#elif TARGET_RT_MAC_CFM // CFM
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


#if QUESA_SUPPORT_HITOOLBOX
	// Shut down OpenGL. This fixes a crash on exit on some apps on Mac OS 9
	// if they exit without destroying renderer objects that use OpenGL.
	//
	// Since we may not have any renderers which use OpenGL, we assume that
	// we've been weak linked and test for the symbol first.
	if ( aglResetLibrary != NULL )
		aglResetLibrary();
#endif
}





//=============================================================================
//      E3MacSystem_LoadPlugins : Scan for and load plug-ins.
//-----------------------------------------------------------------------------
//		Note :	Plug-ins are loaded by scanning certain directories.
//				Which directories we scan depends on whether we are running
//				under OS 9 or OS X, and whether we are built as CFM or Mach-O.
//-----------------------------------------------------------------------------
void
E3MacSystem_LoadPlugins(void)
{
	FSRef					dirRef[ kMaxPluginLocations ];
	bool					isUnique[ kMaxPluginLocations ];
	FSRef					fileRef;
	int						dirCount = 0;
	Boolean					wasChanged, isOnOSX;
	OSStatus				theErr = noErr;
	SInt32					sysVersion;
	int						i, j;



	// Initialise ourselves
	isOnOSX = (Gestalt(gestaltSystemVersion, &sysVersion) == noErr) &&
		(sysVersion >= 0x00001000);



#if TARGET_RT_MAC_CFM
	// Find the Quesa shared library file (CFM only)
	if (gQuesaLib != NULL)
	{
		theErr = FSResolveAlias( NULL, gQuesaLib, &fileRef, &wasChanged);
		if (theErr == noErr)
		{
			// Get the parent directory of the application
			theErr = FSGetCatalogInfo( &fileRef, 0, NULL, NULL, NULL, &dirRef[ dirCount ] );
		}
		if (theErr == noErr)
			++dirCount;
	}
#endif



	// Find the application file
	ProcessSerialNumber		thePSN = { kNoProcess, kCurrentProcess };
	if (isOnOSX)
	{
		theErr = GetProcessBundleLocation( &thePSN, &fileRef );
	}
	else
	{
#if QUESA_SUPPORT_HITOOLBOX
		// FSSpec is not supported in 64 bit, and this else is for Mac OS 9 only anyway.
		FSSpec	appSpec;
		ProcessInfoRec			processInfo;
		processInfo.processInfoLength = sizeof(ProcessInfoRec);
		processInfo.processName       = NULL;
		processInfo.processAppSpec    = &appSpec;
		
		theErr = GetProcessInformation( &thePSN, &processInfo );
		if (theErr == noErr)
		{
			FSpMakeFSRef( &appSpec, &fileRef );
		}
#endif
	}	
	if (theErr == noErr)
	{
		// Get the parent directory of the application
		theErr = FSGetCatalogInfo( &fileRef, 0, NULL, NULL, NULL, &dirRef[ dirCount ] );
	}
	if (theErr == noErr)
		++dirCount;



	if (isOnOSX)
	{
	#if TARGET_RT_MAC_CFM
		// ~/Library/CFMSupport
		theErr = FSFindFolder( kUserDomain, kSharedLibrariesFolderType,
			kDontCreateFolder, &dirRef[ dirCount ] );
		if (theErr == noErr)
			++dirCount;
		
		// /Library/CFMSupport
		theErr = FSFindFolder( kLocalDomain, kSharedLibrariesFolderType,
			kDontCreateFolder, &dirRef[ dirCount ] );
		if (theErr == noErr)
			++dirCount;
	#endif
		
		// Plugins folder of bundle
		CFBundleRef myBundle = CFBundleGetMainBundle();
		if (myBundle != NULL)
		{
			CFURLRef	pluginsURL = CFBundleCopyBuiltInPlugInsURL( myBundle );
			if (pluginsURL != NULL)
			{
				if (CFURLGetFSRef( pluginsURL, &dirRef[ dirCount ] ))
				{
					++dirCount;
				}
				CFRelease( pluginsURL );
			}
		}
	}
	else
	{	// OS 9 Extensions folder
		theErr = FSFindFolder( kOnSystemDisk, kExtensionFolderType,
			kDontCreateFolder, &dirRef[ dirCount ] );
		if (theErr == noErr)
			++dirCount;
	}



	// Look for duplicates among the directory references.
	for (i = 0; i < dirCount; ++i)
	{
		isUnique[i] = true;
	}
	for (i = 0; i < dirCount; ++i)
	{
		for (j = i + 1; j < dirCount; ++j)
		{
			if (noErr == FSCompareFSRefs( &dirRef[i], &dirRef[j] ))
			{
				isUnique[j] = false;
			}
		}
	}



	// Scan for and load our plug-ins
	for (i = 0; i < dirCount; ++i)
		{
		if (isUnique[i])
			{
			e3mac_load_plugins( dirRef[i] );
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

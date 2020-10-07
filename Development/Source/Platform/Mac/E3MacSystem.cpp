/*  NAME:
        E3MacSystem.c

    DESCRIPTION:
        Mac specific routines.

    COPYRIGHT:
        Copyright (c) 1999-2019, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#include "E3MacLog.h"



//=============================================================================
//      Constants
//-----------------------------------------------------------------------------

const int kMaxPluginLocations = 6;

const ItemCount	kPluginSearchBatchSize	= 10;




//=============================================================================
//      Function prototyes
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

Q3_EXTERN_API_C(void) E3MacMachoFrameworkTerminate() __attribute__((destructor));

#ifdef __cplusplus
}
#endif




//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct E3MacSystem_PluginSlot *E3MacSystem_PluginSlotPtr;

typedef struct E3MacSystem_PluginSlot {
	CFBundleRef					pluginBundle;
	E3MacSystem_PluginSlotPtr	nextSlot;
} E3MacSystem_PluginSlot;




//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------

extern short gShlbResFile;
short gShlbResFile = 0;

static E3MacSystem_PluginSlotPtr e3macsystem_pluginSlotHead = nullptr;




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//      e3macho_load_plugin : Load a Mach-o plug-in.
//-----------------------------------------------------------------------------
static void e3macho_load_plugin( CFBundleRef theBundle )
{
	E3MacSystem_PluginSlotPtr newSlot = nullptr;
	
	// Load the plugin, which causes the function marked as CALL_ON_LOAD
	// to be called.
	if (CFBundleLoadExecutable( theBundle ))
	{
		newSlot = (E3MacSystem_PluginSlotPtr)Q3Memory_Allocate(sizeof(E3MacSystem_PluginSlot));
		
		if (newSlot != nullptr)
		{
			newSlot->pluginBundle = theBundle;
			CFRetain( theBundle );
			newSlot->nextSlot = e3macsystem_pluginSlotHead;
			e3macsystem_pluginSlotHead = newSlot;
		}
	}
}





//=============================================================================
//      e3mac_load_plugins : Scan a directory for plug-ins and load them.
//-----------------------------------------------------------------------------
static void
e3mac_load_plugins(const FSRef& dirToScan )
{
	OSErr			theErr;

	// Convert FSRef to a URL.
	CFURLRef dirURL = CFURLCreateFromFSRef( nullptr, &dirToScan );
	
	// Look for plugins in the directory.
	if (dirURL != nullptr)
	{
		CFArrayRef pluginsArray = CFBundleCreateBundlesFromDirectory( nullptr,
			dirURL, CFSTR("quesaplug") );
		
		if (pluginsArray != nullptr)
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
	
}





#pragma mark -
//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------






//-----------------------------------------------------------------------------
//      E3MacMachoFrameworkTerminate : Mach-o terminate routine.
//-----------------------------------------------------------------------------
void E3MacMachoFrameworkTerminate()
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
	
}








//=============================================================================
//      E3MacSystem_Initialise : Initialise the system.
//-----------------------------------------------------------------------------
TQ3Status
E3MacSystem_Initialise(void)
{
#warning start device server
// ToDo: start the device server from inside first library instance!


	return(kQ3Success);
}





//=============================================================================
//      E3MacSystem_Terminate : Terminate the system.
//-----------------------------------------------------------------------------
void
E3MacSystem_Terminate(void)
{
	FILE* logStream = E3GetLogStream( false );
	if (logStream != nullptr)
	{
		fclose( logStream );
	}
}





//=============================================================================
//      E3MacSystem_LoadPlugins : Scan for and load plug-ins.
//-----------------------------------------------------------------------------
//		Note :	Plug-ins are loaded by scanning certain directories.
//-----------------------------------------------------------------------------
void
E3MacSystem_LoadPlugins(void)
{
	FSRef					dirRef[ kMaxPluginLocations ];
	bool					isUnique[ kMaxPluginLocations ];
	FSRef					fileRef;
	int						dirCount = 0;
	Boolean					wasChanged;
	OSStatus				theErr = noErr;
	SInt32					sysVersion;
	int						i, j;



	// Find the application file
	ProcessSerialNumber		thePSN = { kNoProcess, kCurrentProcess };
	theErr = GetProcessBundleLocation( &thePSN, &fileRef );
	
	if (theErr == noErr)
	{
		// Get the parent directory of the application
		theErr = FSGetCatalogInfo( &fileRef, 0, nullptr, nullptr, nullptr, &dirRef[ dirCount ] );
	}
	if (theErr == noErr)
		++dirCount;



	// Plugins folder of bundle
	CFBundleRef myBundle = CFBundleGetMainBundle();
	if (myBundle != nullptr)
	{
		CFURLRef	pluginsURL = CFBundleCopyBuiltInPlugInsURL( myBundle );
		if (pluginsURL != nullptr)
		{
			if (CFURLGetFSRef( pluginsURL, &dirRef[ dirCount ] ))
			{
				++dirCount;
			}
			CFRelease( pluginsURL );
		}
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
		E3LogToConsole("E3MacSystem_UnloadPlugins 1");

		nextSlot = e3macsystem_pluginSlotHead;

		while( nextSlot != nullptr){
			currentSlot = nextSlot;
			nextSlot = currentSlot->nextSlot;
			E3LogToConsole("E3MacSystem_UnloadPlugins 2");
			CFBundleUnloadExecutable( currentSlot->pluginBundle );
			E3LogToConsole("E3MacSystem_UnloadPlugins 3");
			CFRelease( currentSlot->pluginBundle );
			E3LogToConsole("E3MacSystem_UnloadPlugins 4");
			Q3Memory_Free(&currentSlot);
			E3LogToConsole("E3MacSystem_UnloadPlugins 5");
		}
		
	e3macsystem_pluginSlotHead = nullptr;
}

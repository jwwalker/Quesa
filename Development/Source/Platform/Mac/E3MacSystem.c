/*  NAME:
        E3MacSystem.c

    DESCRIPTION:
        Mac specific routines.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
    #include <CarbonCore/CodeFragments.h>
    #include <CarbonCore/Folders.h>
#else
	#include <CodeFragments.h>
	#include <Folders.h>
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



	// Load the plug-in
	theErr = GetDiskFragment(theFSSpec, 0, kCFragGoesToEOF,
								"\p", kPrivateCFragCopy,
								&theConnection, &mainAddr, theStr);

	if (theErr != noErr || theStr[0] != 0x00)
		{
		// Handle failure
		}



	// Should call CloseConnection at this point
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3MacSystem_Initialise : Initialise the system.
//-----------------------------------------------------------------------------
#pragma mark -
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
#pragma mark -
void
E3MacSystem_Terminate(void)
{	AGLContext		glContext;


	// If we're profiling, dump the results and terminate the profiler
#if Q3_PROFILE
	ProfilerDump("\pQuesa Profile Log");
	ProfilerTerm();
#endif



	// If a renderer left an OpenGL context active, shut it down. This fixes
	// a crash on exit on some apps if they exit without destroying renderers
	// that use OpenGL.
	//
	// Note that we may not actually have any renderers which use OpenGL, and
	// so we need to test to see if OpenGL is present first.
	if ((TQ3Uns32) aglGetCurrentContext != (TQ3Uns32) kUnresolvedCFragSymbolAddress)
		{
		glContext = aglGetCurrentContext();
		if (glContext != NULL)
			{
			aglSetCurrentContext(NULL);
			aglSetDrawable(glContext, NULL);
			}
		}
}





//=============================================================================
//      E3MacSystem_LoadPlugins : Scan for and load plug-ins.
//-----------------------------------------------------------------------------
void
E3MacSystem_LoadPlugins(void)
{	SInt16			extensionFolderVolRefNum;
	SInt32			extensionFolderDirID;
	Boolean			targetIsFolder;
	Boolean			wasAliased;
	FSSpec			theFSSpec;
	TQ3Uns32		theIndex;
	OSErr			theErr;
	CInfoPBRec		thePB;



	// Find	the extensions folder
	theErr = FindFolder(kOnSystemDisk, kExtensionFolderType, true,
						&extensionFolderVolRefNum, &extensionFolderDirID);
	if (theErr != noErr)
		return;



	// Iterate through the directory, looking for plug-ins
	theIndex = 1;
	do
		{
		// Get the next file
		thePB.dirInfo.ioFDirIndex = theIndex;
		thePB.dirInfo.ioVRefNum   = extensionFolderVolRefNum;
		thePB.dirInfo.ioDrDirID   = extensionFolderDirID;
		thePB.dirInfo.ioNamePtr   = theFSSpec.name;
		
		theErr = PBGetCatInfoSync(&thePB);

		if (theErr == noErr)
			theErr = FSMakeFSSpec(extensionFolderVolRefNum,
									extensionFolderDirID,
									theFSSpec.name, &theFSSpec);

		if (theErr == noErr)
			theErr = ResolveAliasFile(&theFSSpec, kQ3True, &targetIsFolder, &wasAliased);


		// If this isn't a directory, check the type
		if (!targetIsFolder)
			{
			// If this is a plug-in, load it
			if (thePB.hFileInfo.ioFlFndrInfo.fdType    == kQ3XExtensionMacFileType &&
				thePB.hFileInfo.ioFlFndrInfo.fdCreator == kQ3XExtensionMacCreatorType)
				e3mac_load_plugin(&theFSSpec);
			}
		
		theIndex++;
		}
	while (theErr == noErr);
}

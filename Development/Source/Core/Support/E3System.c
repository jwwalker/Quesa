/*  NAME:
        E3System.c

    DESCRIPTION:
        Quesa platform specific routines.

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
#include "GNRegister.h"
#include "WFRegister.h"
#include "IRRegister.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3System_Initialise : Initialise the system.
//-----------------------------------------------------------------------------
TQ3Status
E3System_Initialise(void)
{	TQ3Status		qd3dStatus;



	// Initialise the system
#if QUESA_OS_MACINTOSH
	qd3dStatus = E3MacSystem_Initialise();

#elif QUESA_OS_UNIX
	qd3dStatus = E3UnixSystem_Initialise();

#elif QUESA_OS_WIN32
	qd3dStatus = E3WindowsSystem_Initialise();

#elif QUESA_OS_BE
	qd3dStatus = E3BeSystem_Initialise();

#elif QUESA_OS_COCOA
	qd3dStatus = E3CocoaSystem_Initialise();
#endif

	return(qd3dStatus);
}





//=============================================================================
//      E3System_Terminate : Terminate the system.
//-----------------------------------------------------------------------------
void
E3System_Terminate(void)
{


	// Terminate the system
#if QUESA_OS_MACINTOSH
	E3MacSystem_Terminate();

#elif QUESA_OS_UNIX
	E3UnixSystem_Terminate();

#elif QUESA_OS_WIN32
	E3WindowsSystem_Terminate();

#elif QUESA_OS_BE
	E3BeSystem_Terminate();

#elif QUESA_OS_COCOA
	E3CocoaSystem_Terminate();
#endif
}





//=============================================================================
//      E3System_LoadPlugins : Scan for and load plug-ins.
//-----------------------------------------------------------------------------
//		Note :	We use a platform specific routine to scan for and load any
//				plug-in libraries, then call the registration functions for any
//				shared libraries that the plug-ins registered with Quesa.
//-----------------------------------------------------------------------------
void
E3System_LoadPlugins(void)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();
	TQ3Status		qd3dStatus;
	TQ3Uns32		n;



	// Load the plug-ins on a per-platform basis
#if QUESA_OS_MACINTOSH
	E3MacSystem_LoadPlugins();

#elif QUESA_OS_UNIX
	E3UnixSystem_LoadPlugins();

#elif QUESA_OS_WIN32
	E3WindowsSystem_LoadPlugins();

#elif QUESA_OS_BE
	E3BeSystem_LoadPlugins();

#elif QUESA_OS_COCOA
	E3CocoaSystem_LoadPlugins();
#endif



	// Register any shared libraries contained in the plug-ins
	for (n = 0; n < theGlobals->sharedLibraryCount; n++)
		{
		if (theGlobals->sharedLibraryInfo[n].registerFunction != NULL)
			qd3dStatus = theGlobals->sharedLibraryInfo[n].registerFunction();
		}



	// Register the built-in plug-ins
#if QUESA_REGISTER_BUILTIN_PLUGINS
	qd3dStatus = GNRenderer_Register();
	qd3dStatus = WFRenderer_Register();
	qd3dStatus = IRRenderer_Register();
#endif
}





//=============================================================================
//      E3System_UnloadPlugins : Unload any plug-ins.
//-----------------------------------------------------------------------------
//		Note : We unregister any shared libraries contained in plug-ins.
//-----------------------------------------------------------------------------
void
E3System_UnloadPlugins(void)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();



	// Dispose of the global state
	theGlobals->sharedLibraryCount = 0;
	Q3Memory_Free(&theGlobals->sharedLibraryInfo);



	// Unregister the built-in plug-ins
#if QUESA_REGISTER_BUILTIN_PLUGINS
	GNRenderer_Unregister();
	WFRenderer_Unregister();
	IRRenderer_Unregister();
#endif


	// Unload the plug-ins on a per-platform basis
#if QUESA_OS_MACINTOSH
	E3MacSystem_UnloadPlugins();

#elif QUESA_OS_UNIX
	E3UnixSystem_UnloadPlugins();

#elif QUESA_OS_WIN32
	E3WindowsSystem_UnloadPlugins();

#elif QUESA_OS_BE
	E3BeSystem_UnloadPlugins();

#elif QUESA_OS_COCOA
	E3CocoaSystem_UnloadPlugins();
#endif

}





//=============================================================================
//      E3System_ClearBottleneck : System bottlebeck point.
//-----------------------------------------------------------------------------
//		Note : Performs any housekeeping required after API entry points.
//-----------------------------------------------------------------------------
void
E3System_ClearBottleneck(void)
{


	// Validate our state
	Q3_ASSERT(gE3Globals.systemDoBottleneck);



	// Clear the Error Manager state
	if (gE3Globals.errMgrClearError)
		E3ErrorManager_ClearError();

	if (gE3Globals.errMgrClearWarning)
		E3ErrorManager_ClearWarning();

	if (gE3Globals.errMgrClearNotice)
		E3ErrorManager_ClearNotice();

	if (gE3Globals.errMgrClearPlatform)
		E3ErrorManager_ClearPlatformError();



	// Reset our state
	gE3Globals.systemDoBottleneck = kQ3False;
}


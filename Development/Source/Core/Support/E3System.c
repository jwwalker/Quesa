/*  NAME:
        E3System.c

    DESCRIPTION:
        Quesa platform specific routines.

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
	E3Memory_Free(&theGlobals->sharedLibraryInfo);



	// Unregister the built-in plug-ins
#if QUESA_REGISTER_BUILTIN_PLUGINS
	GNRenderer_Unregister();
	WFRenderer_Unregister();
	IRRenderer_Unregister();
#endif
}





//=============================================================================
//      E3System_Bottleneck : System bottlebeck point.
//-----------------------------------------------------------------------------
//		Note :	This function is called on virtually every API call, in order
//				to perform any bookkeeping that's required.
//
//				At the present, this simply involves clearing the global error
//				state. For speed, we check the state from outside to see if
//				something has been posted.
//
//				Please profile before making any changes to this routine, as
//				this will be called for every API entry point (so must have
//				minimal overhead).
//-----------------------------------------------------------------------------
void
E3System_Bottleneck(void)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();



	// Clear the global error state
	if (theGlobals->errMgrSomethingPosted)
		E3ErrorManager_ClearAll(theGlobals);
}

/*  NAME:
        E3Globals.c

    DESCRIPTION:
        Quesa global state.

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
#include "E3Globals.h"





//=============================================================================
//      Global variables
//-----------------------------------------------------------------------------
//		Note : Code should access these via E3Globals_Get(); do not access
//				them directly (except in E3System.c).
//-----------------------------------------------------------------------------
E3Globals gE3Globals = {
	kQ3False,				// systemInitialised;
	0,						// systemRefCount;
	0,						// classTree;
	0,						// classTreeRoot;
	0,						// nextType;
	0,						// sharedLibraryCount;
	0,						// sharedLibraryInfo;
	kQ3False,				// errMgrClearError;
	kQ3False,				// errMgrClearWarning;
	kQ3False,				// errMgrClearNotice;
	kQ3False,				// errMgrClearPlatform;
	kQ3False,				// errMgrNeedsClearing;
	kQ3False,				// errMgrIsFatalError;
	kQ3ErrorNone,			// errMgrOldestError;
	kQ3WarningNone,			// errMgrOldestWarning;
	kQ3NoticeNone,			// errMgrOldestNotice;
	0,						// errMgrOldestPlatform;
	kQ3ErrorNone,			// errMgrLatestError;
	kQ3WarningNone,			// errMgrLatestWarning;
	kQ3NoticeNone,			// errMgrLatestNotice;
	0,						// errMgrLatestPlatform;
	0,						// errMgrHandlerFuncError;
	0,						// errMgrHandlerFuncWarning;
	0,						// errMgrHandlerFuncNotice;
	0,						// errMgrHandlerFuncPlatform;
	0,						// errMgrHandlerDataError;
	0,						// errMgrHandlerDataWarning;
	0,						// errMgrHandlerDataNotice;
	0,						// errMgrHandlerDataPlatform;

#if QUESA_OS_MACINTOSH
	0,						// dmNotifyUPP;
	kQ3False,				// dmNotifiedChanges;
#endif

#if Q3_DEBUG
	NULL,					// listHead
	kQ3False				// isLeakChecking
#endif
};





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Globals_Get : Get access to the Quesa global state.
//-----------------------------------------------------------------------------
//		Note : The global state is currently stored in a static variable, and
//				its contents are not shared between instance of Quesa.
//-----------------------------------------------------------------------------
E3GlobalsPtr
E3Globals_Get(void)
{


	// Return the globals
	return(&gE3Globals);
}

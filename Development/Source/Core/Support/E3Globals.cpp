/*  NAME:
        E3Globals.c

    DESCRIPTION:
        Quesa global state.

    COPYRIGHT:
        Copyright (c) 1999-2021, Quesa Developers. All rights reserved.

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
#include "E3Globals.h"





//=============================================================================
//      Global variables
//-----------------------------------------------------------------------------
//		Note : Code should access these via E3Globals_Get(); do not access
//			   them directly (except in E3System.c).
//-----------------------------------------------------------------------------
E3Globals gE3Globals = {
	kQ3False,				// systemInitialised
	kQ3False,				// systemDoBottleneck
	0,						// systemRefCount
	nullptr,				// classTree
	nullptr,				// classTreeRoot
	0,						// classNextType
	0,						// sharedLibraryCount
	nullptr,				// sharedLibraryInfo
	kQ3False,				// errMgrClearError
	kQ3False,				// errMgrClearWarning
	kQ3False,				// errMgrClearNotice
	kQ3False,				// errMgrClearPlatform
	kQ3False,				// errMgrIsFatalError
	kQ3ErrorNone,			// errMgrOldestError
	kQ3WarningNone,			// errMgrOldestWarning
	kQ3NoticeNone,			// errMgrOldestNotice
	0,						// errMgrOldestPlatform
	kQ3ErrorNone,			// errMgrLatestError
	kQ3WarningNone,			// errMgrLatestWarning
	kQ3NoticeNone,			// errMgrLatestNotice
	0,						// errMgrLatestPlatform
	nullptr,				// errMgrHandlerFuncError
	nullptr,				// errMgrHandlerFuncWarning
	nullptr,				// errMgrHandlerFuncNotice
	nullptr,				// errMgrHandlerFuncPlatform
	0,						// errMgrHandlerDataError
	0,						// errMgrHandlerDataWarning
	0,						// errMgrHandlerDataNotice
	0,						// errMgrHandlerDataPlatform

#if Q3_DEBUG
	nullptr,					// listHead
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

/*  NAME:
        QD3DErrors.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Copyright (c) 1999-2014, Quesa Developers. All rights reserved.

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
#include "E3Errors.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3Error_Register : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Error_Register(TQ3ErrorMethod errorPost, TQ3Int32 reference)
{


	// Release build checks



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Error_Register(errorPost, reference));
}





//=============================================================================
//      Q3Warning_Register : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Warning_Register(TQ3WarningMethod warningPost, TQ3Int32 reference)
{


	// Release build checks



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Warning_Register(warningPost, reference));
}





//=============================================================================
//      Q3Notice_Register : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Notice_Register(TQ3NoticeMethod noticePost, TQ3Int32 reference)
{


	// Release build checks



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Notice_Register(noticePost, reference));
}





//=============================================================================
//      Q3Error_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
//		Note :	Must preserve the Error Manager state around the bottleneck,
//				since calling this routine should not clear existing state.
//-----------------------------------------------------------------------------
TQ3Error
Q3Error_Get(TQ3Error *firstError)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();
	TQ3Boolean		saveState;



	// Release build checks
	if (firstError != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(firstError), kQ3ErrorNone);



	// Debug build checks



	// Call the bottleneck, saving the state around it
	saveState                      = theGlobals->systemDoBottleneck;
	theGlobals->systemDoBottleneck = kQ3False;

	E3System_Bottleneck();
	
	theGlobals->systemDoBottleneck = saveState;



	// Call our implementation
	return(E3Error_Get(firstError));
}





//=============================================================================
//      Q3Error_IsFatalError : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Error_IsFatalError(TQ3Error error)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Error_IsFatalError(error));
}





//=============================================================================
//      Q3Warning_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
//		Note :	Must preserve the Error Manager state around the bottleneck,
//				since calling this routine should not clear existing state.
//-----------------------------------------------------------------------------
TQ3Warning
Q3Warning_Get(TQ3Warning *firstWarning)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();
	TQ3Boolean		saveState;



	// Release build checks
	if (firstWarning != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(firstWarning), kQ3WarningNone);



	// Debug build checks




	// Call the bottleneck, saving the state around it
	saveState                      = theGlobals->errMgrClearWarning;
	theGlobals->errMgrClearWarning = kQ3False;

	E3System_Bottleneck();
	
	theGlobals->errMgrClearWarning = saveState;



	// Call our implementation
	return(E3Warning_Get(firstWarning));
}





//=============================================================================
//      Q3Notice_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
//		Note :	Must preserve the Error Manager state around the bottleneck,
//				since calling this routine should not clear existing state.
//-----------------------------------------------------------------------------
TQ3Notice
Q3Notice_Get(TQ3Notice *firstNotice)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();
	TQ3Boolean		saveState;



	// Release build checks
	if (firstNotice != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(firstNotice), kQ3NoticeNone);



	// Debug build checks




	// Call the bottleneck, saving the state around it
	saveState                     = theGlobals->errMgrClearNotice;
	theGlobals->errMgrClearNotice = kQ3False;

	E3System_Bottleneck();
	
	theGlobals->errMgrClearNotice = saveState;



	// Call our implementation
	return(E3Notice_Get(firstNotice));
}





//=============================================================================
//      Q3Error_PlatformGet : Quesa API entry point.
//-----------------------------------------------------------------------------
//		Note :	Must preserve the Error Manager state around the bottleneck,
//				since calling this routine should not clear existing state.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Uns32
Q3Error_PlatformGet(TQ3Uns32 *firstErr)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();
	TQ3Boolean		saveState;



	// Release build checks
	if (firstErr != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(firstErr), 0);



	// Debug build checks




	// Call the bottleneck, saving the state around it
	saveState                       = theGlobals->errMgrClearPlatform;
	theGlobals->errMgrClearPlatform = kQ3False;

	E3System_Bottleneck();
	
	theGlobals->errMgrClearPlatform = saveState;



	// Call our implementation
	return(E3Error_PlatformGet(firstErr));
}
#endif





//=============================================================================
//      Q3Error_PlatformPost : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
void
Q3Error_PlatformPost(TQ3Uns32 theErr)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	E3Error_PlatformPost(theErr);
}
#endif





//=============================================================================
//      Q3Error_ToString : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
const char *
Q3Error_ToString(TQ3Language theLanguage, TQ3Error theError)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Error_ToString(theLanguage, theError));
}
#endif





//=============================================================================
//      Q3Warning_ToString : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
const char *
Q3Warning_ToString(TQ3Language theLanguage, TQ3Warning theWarning)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Warning_ToString(theLanguage, theWarning));
}
#endif





//=============================================================================
//      Q3Notice_ToString : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
const char *
Q3Notice_ToString(TQ3Language theLanguage, TQ3Notice theNotice)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Notice_ToString(theLanguage, theNotice));
}
#endif





//=============================================================================
//      Q3MacintoshError_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_MACINTOSH
OSErr
Q3MacintoshError_Get(OSErr *firstMacErr)
{


	// Release build checks
	if (firstMacErr != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(firstMacErr), noErr);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacintoshError_Get(firstMacErr));
}
#endif





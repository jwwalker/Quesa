/*  NAME:
        QD3DErrors.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#if Q3_DEBUG
	if (0) // Further checks on errorPost
		return(kQ3Failure);

	if (0) // Further checks on reference
		return(kQ3Failure);
#endif



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
#if Q3_DEBUG
	if (0) // Further checks on warningPost
		return(kQ3Failure);

	if (0) // Further checks on reference
		return(kQ3Failure);
#endif



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
#if Q3_DEBUG
	if (0) // Further checks on noticePost
		return(kQ3Failure);

	if (0) // Further checks on reference
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Notice_Register(noticePost, reference));
}





//=============================================================================
//      Q3Error_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Error
Q3Error_Get(TQ3Error *firstError)
{


	// Release build checks
	if (firstError != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(firstError), kQ3ErrorNone);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on firstError
		return(kQ3ErrorNone);
#endif



	// Call our implementation
	return(E3Error_Get(firstError));
}





//=============================================================================
//      Q3Error_IsFatalError : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Error_IsFatalError(TQ3Error error)
{


	// Call our implementation
	return(E3Error_IsFatalError(error));
}





//=============================================================================
//      Q3Warning_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Warning
Q3Warning_Get(TQ3Warning *firstWarning)
{


	// Release build checks
	if (firstWarning != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(firstWarning), kQ3WarningNone);



	// Call our implementation
	return(E3Warning_Get(firstWarning));
}





//=============================================================================
//      Q3Notice_Get : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Notice
Q3Notice_Get(TQ3Notice *firstNotice)
{


	// Release build checks
	if (firstNotice != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(firstNotice), kQ3NoticeNone);



	// Call our implementation
	return(E3Notice_Get(firstNotice));
}





//=============================================================================
//      Q3Error_PlatformGet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3Error_PlatformGet(TQ3Uns32 *firstErr)
{


	// Release build checks
	if (firstErr != NULL)
		Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(firstErr), 0);



	// Call our implementation
	return(E3Error_PlatformGet(firstErr));
}





//=============================================================================
//      Q3Error_PlatformPost : Quesa API entry point.
//-----------------------------------------------------------------------------
void
Q3Error_PlatformPost(TQ3Uns32 theErr)
{


	// Call our implementation
	E3Error_PlatformPost(theErr);
}





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



	// Call our implementation
	return(E3MacintoshError_Get(firstMacErr));
}
#endif





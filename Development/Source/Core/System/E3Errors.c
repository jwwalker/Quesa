/*  NAME:
        E3Errors.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
//      Public functions
//-----------------------------------------------------------------------------
//      E3Error_Register : Register a callback.
//-----------------------------------------------------------------------------
TQ3Status
E3Error_Register(TQ3ErrorMethod errorPost, TQ3Int32 reference)
{


	// Set our callback
	E3ErrorManager_SetCallback_Error(errorPost, reference);
	return(kQ3Success);
}





//=============================================================================
//      E3Warning_Register : Register a callback.
//-----------------------------------------------------------------------------
TQ3Status
E3Warning_Register(TQ3WarningMethod warningPost, TQ3Int32 reference)
{


	// Set our callback
	E3ErrorManager_SetCallback_Warning(warningPost, reference);
	return(kQ3Success);
}





//=============================================================================
//      E3Notice_Register : Register a callback.
//-----------------------------------------------------------------------------
TQ3Status
E3Notice_Register(TQ3NoticeMethod noticePost, TQ3Int32 reference)
{


	// Set our callback
	E3ErrorManager_SetCallback_Notice(noticePost, reference);
	return(kQ3Success);
}





//=============================================================================
//      E3Error_IsFatalError : Return as we've received a fatal error.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Error_IsFatalError(TQ3Error theError)
{	TQ3Boolean		isFatal;



	// Find out if we've hit a fatal error
	isFatal = E3ErrorManager_GetIsFatalError(theError);
	return(isFatal);
}





//=============================================================================
//      E3Error_Get : Get the oldest and latest error.
//-----------------------------------------------------------------------------
TQ3Error
E3Error_Get(TQ3Error *firstError)
{	TQ3Error		latestError;



	// Get the state we need, and clear it
	E3ErrorManager_GetError(firstError, &latestError);
	E3ErrorManager_ClearError();
	
	return(latestError);
}





//=============================================================================
//      E3Warning_Get : Get the oldest and latest warning.
//-----------------------------------------------------------------------------
TQ3Warning
E3Warning_Get(TQ3Warning *firstWarning)
{	TQ3Warning		latestWarning;



	// Get the state we need, and clear it
	E3ErrorManager_GetWarning(firstWarning, &latestWarning);
	E3ErrorManager_ClearWarning();
	
	return(latestWarning);
}





//=============================================================================
//      E3Notice_Get : Get the oldest and latest notice.
//-----------------------------------------------------------------------------
TQ3Notice
E3Notice_Get(TQ3Notice *firstNotice)
{	TQ3Notice		latestNotice;



	// Get the state we need, and clear it
	E3ErrorManager_GetNotice(firstNotice, &latestNotice);
	E3ErrorManager_ClearNotice();
	
	return(latestNotice);
}





//=============================================================================
//      E3Error_PlatformGet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Error_PlatformGet(TQ3Uns32 *firstErr)
{	TQ3Uns32		firstPlatform, latestPlatform;



	// Get the state we need, and clear it
	E3ErrorManager_GetPlatformError(&firstPlatform, &latestPlatform);
	E3ErrorManager_ClearPlatformError();
	
	if (firstErr != NULL)
		*firstErr = firstPlatform;
		
	return(latestPlatform);
}





//=============================================================================
//      E3Error_PlatformPost : Quesa API entry point.
//-----------------------------------------------------------------------------
void
E3Error_PlatformPost(TQ3Uns32 theErr)
{


	// Post the error
	E3ErrorManager_PostPlatformError(theErr);
}


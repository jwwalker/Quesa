/*  NAME:
        QuesaErrors.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_ERRORS_HDR
#define QUESA_ERRORS_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

#include "QD3DErrors.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Constants go here





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Types go here





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Macros go here





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
#if defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON

/*
 *	Q3Error_Register
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Error_Register (
	TQ3ErrorMethod                errorPost,
	TQ3Int32                      reference
);



/*
 *	Q3Warning_Register
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Warning_Register (
	TQ3WarningMethod              warningPost,
	TQ3Int32                      reference
);



/*
 *	Q3Notice_Register
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Notice_Register (
	TQ3NoticeMethod               noticePost,
	TQ3Int32                      reference
);



/*
 *	Q3Error_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Error  )
Q3Error_Get (
	TQ3Error                      *firstError
);



/*
 *	Q3Error_IsFatalError
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Error_IsFatalError (
	TQ3Error                      error
);



/*
 *	Q3Warning_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Warning  )
Q3Warning_Get (
	TQ3Warning                    *firstWarning
);



/*
 *	Q3Notice_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Notice  )
Q3Notice_Get (
	TQ3Notice                     *firstNotice
);

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON


/*
 *	Q3Error_PlatformGet
 *		Description of function
 */
EXTERN_API_C ( TQ3Uns32  )
Q3Error_PlatformGet (
	TQ3Uns32                      *firstErr
);



/*
 *	Q3Error_PlatformPost
 *		Description of function
 */
EXTERN_API_C ( void  )
Q3Error_PlatformPost (
	TQ3Uns32                      theErr
);





//=============================================================================
//      Mac OS function prototypes
//-----------------------------------------------------------------------------
#if defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON

#if OS_MACINTOSH

/*
 *	Q3MacintoshError_Get
 *		Description of function
 */
EXTERN_API_C ( OSErr  )
Q3MacintoshError_Get (
	OSErr                         *firstMacErr
);

#endif // OS_MACINTOSH

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        E3Errors.h

    DESCRIPTION:
        Header file for E3Errors.c.

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
#ifndef __E3ERRORS__
#define __E3ERRORS__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Cross platform
TQ3Status			E3Error_Register(TQ3ErrorMethod errorPost,       TQ3Int32 reference);
TQ3Status			E3Warning_Register(TQ3WarningMethod warningPost, TQ3Int32 reference);
TQ3Status			E3Notice_Register(TQ3NoticeMethod noticePost,    TQ3Int32 reference);
TQ3Boolean			E3Error_IsFatalError(TQ3Error theError);
TQ3Error			E3Error_Get(TQ3Error *firstError);
TQ3Warning			E3Warning_Get(TQ3Warning *firstWarning);
TQ3Notice			E3Notice_Get(TQ3Notice *firstNotice);
TQ3Uns32			E3Error_PlatformGet(TQ3Uns32 *firstErr);
void				E3Error_PlatformPost(TQ3Uns32 theErr);


// Platform specific
#if OS_MACINTOSH
OSErr				E3MacintoshError_Get(OSErr *firstErr);
#endif





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


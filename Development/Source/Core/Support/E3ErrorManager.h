/*  NAME:
        E3ErrorManager.h

    DESCRIPTION:
        Header file for E3ErrorManager.c.

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
#ifndef E3ERRORMANAGER_HDR
#define E3ERRORMANAGER_HDR
//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Post an error, warning, notice, or platform error
void E3ErrorManager_PostError(TQ3Error         theError, TQ3Boolean isFatal);
void E3ErrorManager_PostWarning(TQ3Warning     theWarning);
void E3ErrorManager_PostNotice(TQ3Notice       theNotice);
void E3ErrorManager_PostPlatformError(TQ3Uns32 theError);


// Get the oldest and latest error, warning, notice, or platform error
TQ3Boolean E3ErrorManager_GetIsFatalError(TQ3Error theError);
void       E3ErrorManager_GetError(TQ3Error         *oldestError,    TQ3Error   *latestError);
void       E3ErrorManager_GetWarning(TQ3Warning     *oldestWarning,  TQ3Warning *latestWarning);
void       E3ErrorManager_GetNotice(TQ3Notice       *oldestNotice,   TQ3Notice  *latestNotice);
void       E3ErrorManager_GetPlatformError(TQ3Uns32 *oldestPlatform, TQ3Uns32   *latestPlatform);


// Clear the current error, warning, notice, or platform error
void E3ErrorManager_ClearAll(E3GlobalsPtr theGlobals);
void E3ErrorManager_ClearError(void);
void E3ErrorManager_ClearWarning(void);
void E3ErrorManager_ClearNotice(void);
void E3ErrorManager_ClearPlatformError(void);


// Get/set error handler callbacks
TQ3ErrorMethod   E3ErrorManager_GetCallback_Error(void);
TQ3WarningMethod E3ErrorManager_GetCallback_Warning(void);
TQ3NoticeMethod  E3ErrorManager_GetCallback_Notice(void);
TQ3ErrorMethod   E3ErrorManager_GetCallback_PlatformError(void);

void E3ErrorManager_SetCallback_Error(TQ3ErrorMethod         theCallback, TQ3Uns32 theData);
void E3ErrorManager_SetCallback_Warning(TQ3WarningMethod     theCallback, TQ3Uns32 theData);
void E3ErrorManager_SetCallback_Notice(TQ3NoticeMethod       theCallback, TQ3Uns32 theData);
void E3ErrorManager_SetCallback_PlatformError(TQ3ErrorMethod theCallback, TQ3Uns32 theData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


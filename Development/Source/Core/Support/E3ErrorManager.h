/*  NAME:
        E3ErrorManager.h

    DESCRIPTION:
        Header file for E3ErrorManager.c.

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


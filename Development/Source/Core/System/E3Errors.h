/*  NAME:
        E3Errors.h

    DESCRIPTION:
        Header file for E3Errors.c.

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
#ifndef E3ERRORS_HDR
#define E3ERRORS_HDR
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
TQ3Uns32			E3Error_PlatformGet(TQ3Uns32 *firstPlatform);
void				E3Error_PlatformPost(TQ3Uns32 theErr);
const char			*E3Error_ToString(TQ3Language theLanguage,   TQ3Error theError);
const char			*E3Warning_ToString(TQ3Language theLanguage, TQ3Warning theWarning);
const char			*E3Notice_ToString(TQ3Language theLanguage,  TQ3Notice theNotice);


// Platform specific
#if QUESA_OS_MACINTOSH
OSErr				E3MacintoshError_Get(OSErr *firstErr);
#endif





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


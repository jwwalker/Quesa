/*  NAME:
        GLDrawContext.h

    DESCRIPTION:
        Header file for GLDrawContext.c.

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
#ifndef GLDRAWCONTEXT_HDR
#define GLDRAWCONTEXT_HDR
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
void				*GLDrawContext_New(
								TQ3ViewObject			theView,
								TQ3DrawContextObject	theDrawContext,
								GLbitfield				*clearFlags);

void				GLDrawContext_Destroy(
								void					**glContext);

void				GLDrawContext_SwapBuffers(
								void					*glContext);

void				GLDrawContext_SetCurrent(
								void					*glContext,
								TQ3Boolean				forceSet);

void				GLDrawContext_SetClearFlags(
								TQ3DrawContextObject	theDrawContext,
								GLbitfield				*clearFlags);

void				GLDrawContext_SetBackgroundColour(
								TQ3DrawContextObject	theDrawContext);

void				GLDrawContext_SetDepthState(
								TQ3DrawContextObject	theDrawContext);

TQ3Boolean			GLDrawContext_UpdateWindowClip(
								void					*glContext);

TQ3Boolean			GLDrawContext_UpdateWindowPosition(
								void					*glContext);

GLfloat				GLDrawContext_GetMinLineWidth(
								void					*glContext);

TQ3Status			GLDrawContext_UpdateSize(
								TQ3DrawContextObject	theDrawContext,
								void					*glContext );



//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        E3System.h

    DESCRIPTION:
        Header file for E3System.c.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
#ifndef E3SYSTEM_HDR
#define E3SYSTEM_HDR
//=============================================================================
//		Include files
//-----------------------------------------------------------------------------





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// E3System_Bottleneck
//
// Invoked on every API entry point to allow us to perform system housekeeping.
// To minimise the performance impact, the bottleneck is implemented as a macro
// which polls a global then invokes a real function if there is any work to do.
#define E3System_Bottleneck()													\
				do																\
					{															\
					if (gE3Globals.systemDoBottleneck)							\
						E3System_ClearBottleneck();								\
					}															\
				while (0)





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Cross platform
TQ3Status	E3System_Initialise(void);
void		E3System_Terminate(void);
void		E3System_LoadPlugins(void);
void		E3System_UnloadPlugins(void);
void		E3System_ClearBottleneck(void);


// Mac specific
#if QUESA_OS_MACINTOSH
TQ3Status	E3MacSystem_Initialise(void);
void		E3MacSystem_Terminate();
void		E3MacSystem_LoadPlugins(void);
void		E3MacSystem_UnloadPlugins(void);
#endif


// Unix specific
#if QUESA_OS_UNIX
TQ3Status	E3UnixSystem_Initialise(void);
void		E3UnixSystem_Terminate(void);
void		E3UnixSystem_LoadPlugins(void);
void		E3UnixSystem_UnloadPlugins(void);
#endif


// Windows specific
#if QUESA_OS_WIN32
TQ3Status	E3WindowsSystem_Initialise(void);
void		E3WindowsSystem_Terminate(void);
void		E3WindowsSystem_LoadPlugins(void);
void		E3WindowsSystem_UnloadPlugins(void);
#endif


// Be specific
#if QUESA_OS_BE
TQ3Status	E3BeSystem_Initialise(void);
void		E3BeSystem_Terminate(void);
void		E3BeSystem_LoadPlugins(void);
void		E3BeSystem_UnloadPlugins(void);
#endif


// Cocoa specific
#if QUESA_OS_COCOA
TQ3Status	E3CocoaSystem_Initialise(void);
void		E3CocoaSystem_Terminate(void);
void		E3CocoaSystem_LoadPlugins(void);
void		E3CocoaSystem_UnloadPlugins(void);
#endif





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        E3System.h

    DESCRIPTION:
        Header file for E3System.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
//      Function prototypes
//-----------------------------------------------------------------------------
// Cross platform
TQ3Status	E3System_Initialise(void);
void		E3System_Terminate(void);
void		E3System_LoadPlugins(void);
void		E3System_UnloadPlugins(void);
void		E3System_Bottleneck(void);


// Mac specific
#if QUESA_OS_MACINTOSH
TQ3Status	E3MacSystem_Initialise(void);
void		E3MacSystem_Terminate();
void		E3MacSystem_LoadPlugins(void);
#endif


// Unix specific
#if QUESA_OS_UNIX
TQ3Status	E3UnixSystem_Initialise(void);
void		E3UnixSystem_Terminate(void);
void		E3UnixSystem_LoadPlugins(void);
#endif


// Windows specific
#if QUESA_OS_WIN32
TQ3Status	E3WindowsSystem_Initialise(void);
void		E3WindowsSystem_Terminate(void);
void		E3WindowsSystem_LoadPlugins(void);
#endif


// Be specific
#if QUESA_OS_BE
TQ3Status	E3BeSystem_Initialise(void);
void		E3BeSystem_Terminate(void);
void		E3BeSystem_LoadPlugins(void);
#endif


// Cocoa specific
#if QUESA_OS_COCOA
TQ3Status	E3CocoaSystem_Initialise(void);
void		E3CocoaSystem_Terminate(void);
void		E3CocoaSystem_LoadPlugins(void);
#endif





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        GLCocoaContext.h

    DESCRIPTION:
        Header file for GLCocoaContext.m.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#ifndef GLCOCOACONTEXT_HDR
#define GLCOCOACONTEXT_HDR
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
//      Types
//-----------------------------------------------------------------------------
// Cocoa GL context type
#if QUESA_OS_COCOA

typedef struct CocoaGLContext {
	void			*glContext;
	void 			*nsView;
} CocoaGLContext;

#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Cocoa GL context operations
#if QUESA_OS_COCOA

void		*gldrawcontext_cocoa_new(TQ3DrawContextObject theDrawContext);
void		gldrawcontext_cocoa_destroy(void *glContext);
void		gldrawcontext_cocoa_swapbuffers(void *glContext);
void		gldrawcontext_cocoa_setcurrent(void *glContext);
TQ3Boolean	gldrawcontext_cocoa_updateclip(void *glContext);
TQ3Boolean	gldrawcontext_cocoa_updatepos(void *glContext);

#endif





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


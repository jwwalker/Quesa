/*  NAME:
        GLDrawContext.h

    DESCRIPTION:
        Header file for GLDrawContext.c.

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
// Create/Destroy a GL context
void		*GLDrawContext_New(TQ3DrawContextObject theDrawContext, GLbitfield *clearFlags);
void		GLDrawContext_Destroy(void **glContext);


// Swap the back and front buffers
void		GLDrawContext_SwapBuffers(void *glContext);


// Make a context active
void		GLDrawContext_SetCurrent(void *glContext, TQ3Boolean forceSet);


// Update the state
void		GLDrawContext_SetClearFlags(TQ3DrawContextObject theDrawContext, GLbitfield *clearFlags);
void		GLDrawContext_SetBackgroundColour(TQ3DrawContextObject theDrawContext);
TQ3Boolean	GLDrawContext_UpdateWindowClip(void *glContext);
TQ3Boolean	GLDrawContext_UpdateWindowPosition(void *glContext);




//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        GLPrefix.h

    DESCRIPTION:
        Global prefix file for OpenGL.

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
#ifndef GLPREFIX_HDR
#define GLPREFIX_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Quesa
#include "E3Prefix.h"


// OpenGL (everywhere but Mac OS X)
#if !TARGET_RT_MAC_MACHO && !QUESA_OS_COCOA
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif


// OpenGL (platform specific)
#if QUESA_OS_MACINTOSH
	#if TARGET_RT_MAC_MACHO
            #include <OpenGL/gl.h>
            #include <OpenGL/glu.h>
            #include <AGL/agl.h>
            #include <AGL/aglRenderers.h>
        #else
            #include <agl.h>
            #include <aglRenderers.h>
        #endif

#elif QUESA_OS_UNIX
	#include <GL/glx.h>

#elif QUESA_OS_BE
	#include <be/opengl/GLView.h>

#elif QUESA_OS_COCOA
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#endif





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// For Mac specific weak linking
#if QUESA_OS_MACINTOSH
	#define QUESA_FOUND_OPENGL		((TQ3Uns32) aglGetVersion != (TQ3Uns32) kUnresolvedCFragSymbolAddress)
#else
	#define QUESA_FOUND_OPENGL		(1)
#endif



#endif

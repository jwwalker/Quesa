/*  NAME:
        GLPrefix.h

    DESCRIPTION:
        Global prefix file for OpenGL.

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
#ifndef GLPREFIX_HDR
#define GLPREFIX_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Quesa
#include "E3Prefix.h"


// OpenGL
#if QUESA_OS_MACINTOSH

	#if TARGET_RT_MAC_MACHO
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
		#include <AGL/agl.h>
		#include <AGL/aglRenderers.h>
	#else
		#include <gl.h>
		#include <glu.h>
		#include <agl.h>
		#include <aglRenderers.h>
	#endif

#elif QUESA_OS_WIN32
    #include <GL/gl.h>
    #include <GL/glu.h>

#elif QUESA_OS_UNIX
	#include <GL/glx.h>
    #include <GL/glu.h>

#elif QUESA_OS_BE
    #include <GL/gl.h>
    #include <GL/glu.h>
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


//=============================================================================
//      Types
//-----------------------------------------------------------------------------

// OpenGL extension availability flags
typedef struct TQ3GLExtensions
{
	TQ3Boolean				separateSpecularColor;
} TQ3GLExtensions;


#endif

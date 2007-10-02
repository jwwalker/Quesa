/*  NAME:
        GLPrefix.h

    DESCRIPTION:
        Global prefix file for OpenGL.

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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
#include "Quesa.h"


// OpenGL
#if QUESA_OS_MACINTOSH

	#if QUESA_UH_IN_FRAMEWORKS
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
// for glXGetProcAddressARB
    #define GLX_GLXEXT_PROTOTYPES 1

    #include <GL/glx.h>
    #include <GL/glu.h>

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
struct TQ3GLExtensions
{
	TQ3Boolean				separateSpecularColor;
	TQ3Boolean				clampToEdge;
	TQ3Boolean				multitexture;			// GL_ARB_multitexture
	TQ3Boolean				blendMinMax;
	TQ3Boolean				vertexBufferObjects;
	TQ3Boolean				frameBufferObjects;		// GL_EXT_framebuffer_object
	TQ3Boolean				shadingLanguage;
	TQ3Boolean				packedPixels;			// GL 1.2
	bool					depthClamp;				// GL_NV_depth_clamp
	bool					stencilTwoSide;			// GL_EXT_stencil_two_side
	bool					separateStencil;
	bool					stencilWrap;			// GL_EXT_stencil_wrap
	bool					packedDepthStencil;		// GL_EXT_packed_depth_stencil
};


/*!
	@class		CQ3GLContext
	@abstract	Abstract base class for an OpenGL context.
	@discussion	The concrete subclasses will be platform-dependent.
	
				A subclass constructor should get the address for
				bindFrameBufferFunc.  It is usually necessary to make the GL
				context current and perhaps check for the FBO extension before
				doing so, hence the base class constructor cannot do it.
*/
class CQ3GLContext
{
public:
						CQ3GLContext( TQ3DrawContextObject inDC )
							: quesaDrawContext( inDC )
							, currentFrameBufferID( 0 )
							, bindFrameBufferFunc( NULL ) {}
				
	virtual				~CQ3GLContext() {}
	
	virtual void		SwapBuffers() = 0;
	
	virtual void		StartFrame() {}

						// Make the platform OpenGL context current, but
						// do not alter the framebuffer binding.
	virtual void		SetCurrentBase( TQ3Boolean inForceSet ) = 0;
	
						// Set the active framebuffer (FBO), or use 0 to select
						// no FBO.
	void				BindFrameBuffer( GLuint inFrameBufferID );
	
						// Make the context current.  Normally this will use
						// SetCurrentBase and BindFrameBuffer.
	virtual void		SetCurrent( TQ3Boolean inForceSet ) = 0;
	
	virtual bool		UpdateWindowPosition() { return false; }
	
	virtual bool		UpdateWindowSize() { return false; }

protected:
	TQ3Object			quesaDrawContext;
	GLuint				currentFrameBufferID;
	void*				bindFrameBufferFunc;
};


/*!
	@typedef		TQ3GLContext
	@abstract		Opaque pointer to a GL context.  What it points to is
					platform-dependent, and may be a private Quesa structure
					rather than simply a platform GL context.
*/
typedef	void*		TQ3GLContext;


// GLU callback
#ifndef CALLBACK
	#define CALLBACK
#endif

#if defined(__GNUC__) && (__GNUC__ >= 4) && defined(__APPLE_CC__) && (__APPLE_CC__ < 5450)

	typedef GLvoid (CALLBACK *GLcallback)(...);
#else

	typedef void (CALLBACK *GLcallback)();

#endif

#endif

/*  NAME:
        GLCocoaContext.m

    DESCRIPTION:
        Interface to NSOpenGLContext for Cocoa support.
        
        Note that this code should really live as another branch inside
        GLDrawContext.c, however since we need to use Objective-C to
        access the Cocoa OpenGL API then this is handled as a special case.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <AppKit/NSOpenGL.h>
#import <AppKit/NSOpenGLView.h>
#import <OpenGL/gl.h>

#import "GLPrefix.h"
#import "GLCocoaContext.h"
#include "GLUtils.h"





//=============================================================================
//		Platform check
//-----------------------------------------------------------------------------
#if !QUESA_OS_COCOA
	#error This should only be built for Cocoa!
#endif



CocoaGLContext::CocoaGLContext(
					TQ3DrawContextObject theDrawContext )
	: CQ3GLContext( theDrawContext )
	, glContext( NULL )
	, nsView( NULL )
{
   TQ3Int32						glAttributes[] =
	{
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 24,
		NSOpenGLPFAStencilSize, 8,
		0
	};
	TQ3ObjectType					drawContextType;
	TQ3DrawContextData				drawContextData;
    NSOpenGLPixelFormat				*pixelFormat = NULL;
    CocoaGLContext					*theContext;
	TQ3Status						qd3dStatus;
    NSRect							viewFrame;
    GLint							glRect[4];
    GLint							enable;
	TQ3GLExtensions					extFlags;



	// Get the type specific draw context data
	drawContextType = Q3DrawContext_GetType(theDrawContext);
    switch (drawContextType) {
		// NSView
		case kQ3DrawContextTypeCocoa:
			// Get the NSView
			Q3CocoaDrawContext_GetNSView(theDrawContext, &nsView);


			// Check whether a pixel format was provided by the client
			Q3Object_GetProperty( theDrawContext, kQ3DrawContextPropertyGLPixelFormat,
				sizeof(pixelFormat), NULL, &pixelFormat );


			// Set up and create the NSOpenGLContext
			if (pixelFormat == NULL)
			{
				pixelFormat           = [[NSOpenGLPixelFormat alloc]
					initWithAttributes:(NSOpenGLPixelFormatAttribute*)glAttributes];
				glContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
				[pixelFormat release];
			}
			else
			{
				glContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
			}


			// Set the NSView as the NSOpenGLContext's drawable
			[(id)glContext setView:(id)nsView];
			[(id)glContext makeCurrentContext];


			// Get the view bounds from the NSView for the initial gl viewport
			// and the default draw context pane if it's needed.
			viewFrame = [[(id)glContext view]bounds];
          break;

		
		// Unsupported
		default:
			throw std::exception();
			break;
		}



	// Get the common draw context data
	qd3dStatus = Q3DrawContext_GetData(theDrawContext, &drawContextData);
	if (qd3dStatus != kQ3Success)
		throw std::exception();

	if (!drawContextData.paneState)
    {
      drawContextData.pane.min.x = viewFrame.origin.x;
      drawContextData.pane.min.y = viewFrame.origin.y;
      drawContextData.pane.max.x = viewFrame.origin.x+viewFrame.size.width;
      drawContextData.pane.max.y = viewFrame.origin.y+viewFrame.size.height;
    }



	// Activate the context
    [(id)glContext makeCurrentContext];



	// Set the viewport
	glRect[0] = (GLint) drawContextData.pane.min.x;
	glRect[1] = (GLint) ((viewFrame.origin.y+viewFrame.size.height)            
                                      - drawContextData.pane.max.y);
	glRect[2] = (GLint) (drawContextData.pane.max.x - drawContextData.pane.min.x);
	glRect[3] = (GLint) (drawContextData.pane.max.y - drawContextData.pane.min.y);

	glViewport(0, 0, glRect[2], glRect[3]);



	// Set the swap buffer rect
	enable = 1;
	[(id)glContext setValues:&enable forParameter:NSOpenGLCPSwapRectangleEnable];
	[(id)glContext setValues:glRect forParameter:NSOpenGLCPSwapRectangle];



	// Get the function pointer to bind an FBO, if possible.  This cannot be done until
	// after the context has been made current.
	GLUtils_CheckExtensions( &extFlags );
	if (extFlags.frameBufferObjects)
	{
		// The extension check is necessary; the function pointer may be
		// available even if the extension is not.
		bindFrameBufferFunc = GLGetProcAddress( "glBindFramebufferEXT" );
 	}



	// Sync to monitor refresh rate?
	TQ3Boolean	doSync;
	if ( (kQ3Success == Q3Object_GetProperty( theDrawContext,
		kQ3DrawContextPropertySyncToRefresh, sizeof(doSync), NULL, &doSync ))
		&&
		doSync
	)
	{
		[(id)glContext	setValues:&enable
									forParameter:NSOpenGLCPSwapInterval];
	}
}

CocoaGLContext::~CocoaGLContext()
{
	// Close down the context
	[NSOpenGLContext clearCurrentContext];
	// BH, this seems right, but complains 'invalid drawable' when it runs
	// [(id)glContext setView:nil];



	// Destroy the context
    [(id)glContext release];
}
	
void	CocoaGLContext::SwapBuffers()
{
	// Previously there was a glFlush call here, with a comment that it doesn't
	// seem to draw without it.  But now it does seem fine without glFlush, and
	// Apple docs explicitly say that one should not call glFlush before
	// flushBuffer.


	// Swap the buffers
	[(id)glContext flushBuffer];
}

void	CocoaGLContext::SetCurrentBase( TQ3Boolean inForceSet )
{
	// Activate the context
	if(inForceSet || ![[NSOpenGLContext currentContext]isEqual:(id)glContext])
		[(id)glContext makeCurrentContext];
}

void	CocoaGLContext::SetCurrent( TQ3Boolean inForceSet )
{
	// Activate the context
	SetCurrentBase( inForceSet );
	
	// Make sure that no FBO is active
	BindFrameBuffer( 0 );
}


bool	CocoaGLContext::UpdateWindowSize()
{
	[(id)glContext update];
	
	NSRect	viewFrame = [(id)nsView bounds];
	TQ3DrawContextData				drawContextData;
	Q3DrawContext_GetData( quesaDrawContext, &drawContextData );
	
	GLint	glRect[4] =
	{
		drawContextData.pane.min.x,
		((viewFrame.origin.y+viewFrame.size.height)            
                                      - drawContextData.pane.max.y),
		drawContextData.pane.max.x - drawContextData.pane.min.x,
		drawContextData.pane.max.y - drawContextData.pane.min.y
	};
	glViewport(0, 0, glRect[2], glRect[3]);
	
	[(id)glContext setValues:glRect
					forParameter:NSOpenGLCPSwapRectangle];
	
	return true;
}




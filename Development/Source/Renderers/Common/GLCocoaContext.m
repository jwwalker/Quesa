/*  NAME:
        GLCocoaContext.m

    DESCRIPTION:
        Interface to NSOpenGLContext for Cocoa support.
        
        Note that this code should really live as another branch inside
        GLDrawContext.c, however since we need to use Objective-C to
        access the Cocoa OpenGL API then this is handled as a special case.

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





//=============================================================================
//		Platform check
//-----------------------------------------------------------------------------
#if !QUESA_OS_COCOA
	#error This should only be built for Cocoa!
#endif





//=============================================================================
//		gldrawcontext_cocoa_new : Create an OpenGL context for a draw context.
//-----------------------------------------------------------------------------
//		Note :	BH, use glAttributes={5,12,16,0} for double-buffer and 16 bits
//				depth buffer
//-----------------------------------------------------------------------------
void * 
gldrawcontext_cocoa_new(TQ3DrawContextObject theDrawContext)
{    NSOpenGLPixelFormatAttribute	glAttributes[] = { NSOpenGLPFADepthSize, 24, 0 };
	TQ3ObjectType					drawContextType;
	TQ3DrawContextData				drawContextData;
    NSOpenGLPixelFormat				*pixelFormat;
    CocoaGLContext					*theContext;
	TQ3Status						qd3dStatus;
    NSRect							viewFrame;
    TQ3Uns32						glRect[4];
    TQ3Uns32						enable;



	// Allocate the context structure
	theContext = (CocoaGLContext *) Q3Memory_AllocateClear(sizeof(CocoaGLContext));
	if (theContext == NULL)
		return(NULL);



	// Get the type specific draw context data
	drawContextType = Q3DrawContext_GetType(theDrawContext);
    switch (drawContextType) {
		// NSView
		case kQ3DrawContextTypeCocoa:
			// Get the NSView
			Q3CocoaDrawContext_GetNSView(theDrawContext, &theContext->nsView);


			// Set up and create the NSOpenGLContext
			pixelFormat           = [[NSOpenGLPixelFormat alloc] initWithAttributes:glAttributes];
			theContext->glContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
			[pixelFormat release];


			// Set the NSView as the NSOpenGLContext's drawable
			[(id)theContext->glContext setView:(id)theContext->nsView];
			[(id)theContext->glContext makeCurrentContext];


			// Get the view bounds from the NSView for the initial gl viewport
			// and the default draw context pane if it's needed.
			viewFrame = [[(id)theContext->glContext view]bounds];
          break;

		
		// Unsupported
		default:
			goto fail;
			break;
		}



	// Get the common draw context data
	qd3dStatus = Q3DrawContext_GetData(theDrawContext, &drawContextData);
	if (qd3dStatus != kQ3Success)
		goto fail;

	if (!drawContextData.paneState)
    {
      drawContextData.pane.min.x = viewFrame.origin.x;
      drawContextData.pane.min.y = viewFrame.origin.y;
      drawContextData.pane.max.x = viewFrame.origin.x+viewFrame.size.width;
      drawContextData.pane.max.y = viewFrame.origin.y+viewFrame.size.height;
    }



	// Activate the context
    [(id)theContext->glContext makeCurrentContext];



	// Set the viewport
	glRect[0] = drawContextData.pane.min.x;
	glRect[1] = (viewFrame.origin.y+viewFrame.size.height)            
                                      - drawContextData.pane.max.y;
	glRect[2] = drawContextData.pane.max.x - drawContextData.pane.min.x;
	glRect[3] = drawContextData.pane.max.y - drawContextData.pane.min.y;

	glViewport(0, 0, glRect[2], glRect[3]);



	// Set the swap buffer rect
	enable = 1;
	[(id)theContext->glContext setValues:&enable forParameter:NSOpenGLCPSwapRectangleEnable];
	[(id)theContext->glContext setValues:(const long *) glRect forParameter:NSOpenGLCPSwapRectangle];



	// Return the CocoaGLContext
	return(theContext);

fail:
	Q3Memory_Free(&theContext);
	return(NULL);
}





//=============================================================================
//		gldrawcontext_cocoa_destroy : Destroy an OpenGL context.
//-----------------------------------------------------------------------------
void
gldrawcontext_cocoa_destroy(void *glContext)
{	CocoaGLContext		*theContext = (CocoaGLContext *) glContext;



	// Close down the context
	[NSOpenGLContext clearCurrentContext];
	// BH, this seems right, but complains 'invalid drawable' when it runs
	// [(id)theContext->glContext setView:nil];



	// Destroy the context
    [(id)theContext->glContext release];



	// Dispose of the GL state
	Q3Memory_Free(&theContext);
}





//=============================================================================
//		gldrawcontext_cocoa_swapbuffers : Swap the buffers.
//-----------------------------------------------------------------------------
void
gldrawcontext_cocoa_swapbuffers(void *glContext)
{	CocoaGLContext		*theContext = (CocoaGLContext *) glContext;



	// BH, I didn't expect to have to call this, but it doesn't seem to
    // draw without it here (I expected the gl renderer to do it).
	glFlush();



	// Swap the buffers
	[(id)theContext->glContext flushBuffer];
}





//=============================================================================
//		gldrawcontext_cocoa_setcurrent : Make an OpenGL context current.
//-----------------------------------------------------------------------------
void
gldrawcontext_cocoa_setcurrent(void *glContext, TQ3Boolean forceSet)
{	CocoaGLContext		*theContext = (CocoaGLContext *) glContext;



	// Activate the context
	if(forceSet || ![[NSOpenGLContext currentContext]isEqual:(id)theContext->glContext])
		[(id)theContext->glContext makeCurrentContext];
}





//=============================================================================
//		gldrawcontext_cocoa_updateclip : Update OpenGL context clipping.
//-----------------------------------------------------------------------------
TQ3Boolean
gldrawcontext_cocoa_updateclip(void *glContext)
{


	// Not required
	return(kQ3False);
}





//=============================================================================
//		gldrawcontext_cocoa_updatepos : Update OpenGL context position.
//-----------------------------------------------------------------------------
TQ3Boolean
gldrawcontext_cocoa_updatepos(void *glContext)
{


	// Not required
	return(kQ3False);
}

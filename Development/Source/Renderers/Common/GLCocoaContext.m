/*  NAME:
        GLCocoaContext.m

    DESCRIPTION:
        Interface to NSOpenGLContext for Cocoa support.
        
        Note that this code should really live as another branch inside
        GLDrawContext.c, however since we need to use Objective-C to
        access the Cocoa OpenGL API then this is handled as a special case.

    COPYRIGHT:
        Quesa Copyright � 1999-2002, Quesa Developers.
        
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
gldrawcontext_cocoa_setcurrent(void *glContext)
{	CocoaGLContext		*theContext = (CocoaGLContext *) glContext;



	// Activate the context
	if(![[NSOpenGLContext currentContext]isEqual:(id)theContext->glContext])
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

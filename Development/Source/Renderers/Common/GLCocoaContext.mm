/*  NAME:
        GLCocoaContext.mm

    DESCRIPTION:
        Interface to NSOpenGLContext for Cocoa support.
        
        Note that this code should really live as another branch inside
        GLDrawContext.c, however since we need to use Objective-C to
        access the Cocoa OpenGL API then this is handled as a special case.

    COPYRIGHT:
        Copyright (c) 1999-2022, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#if QUESA_OS_COCOA

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <AppKit/NSOpenGL.h>
#import <AppKit/NSOpenGLView.h>
#import <cstring>

#import "GLPrefix.h"
#import "GLCocoaContext.h"
#import "GLUtils.h"
#import "GLGPUSharing.h"
#import "GLDrawContext.h"
#import "E3DrawContext.h"


#ifndef GL_FRAMEBUFFER
	#define GL_FRAMEBUFFER                    0x8D40
#endif

//=============================================================================
//		Platform check
//-----------------------------------------------------------------------------
#if !QUESA_OS_COCOA
	#error This should only be built for Cocoa!
#endif

@interface NSOpenGLContext ()

- (NSArray<NSOpenGLContext*>*) sharingPartners;
/*
	If instances of NSOpenGLContext are allocated by the client rather than Quesa,
	and the client does texture sharing by passing a non-nil value to the second
	parameter of -[NSOpenGLContext initWithFormat: shareContext:], then the client
	needs to let Quesa know about the sharing.  It can do so by allocating a
	subclass of NSOpenGLContext with this sharingPartners method, providing all
	the other contexts with which this one shares data.  It is not sufficient to
	just tell Quesa about the one sharing context passed to the initializer,
	because creation of CocoaGLContext instances may not happen in the same order
	as creation of NSOpenGLContext instances.
*/

@end


#if Q3_DEBUG
#undef Q3_DEBUG_GL_ERRORS
#define Q3_DEBUG_GL_ERRORS 	1
#endif


CocoaGLContext::CocoaGLContext(
					TQ3DrawContextObject theDrawContext,
					TQ3Boolean shareTextures )
	: CQ3GLContext( theDrawContext )
	, glContext( nil )
	, createdContext( NO )
	, nsView( nullptr )
{
   TQ3Int32						glAttributes[] =
	{
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 24,
		NSOpenGLPFAStencilSize, 8,
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
		0
	};
	TQ3ObjectType					drawContextType;
	TQ3DrawContextData				drawContextData;
    NSOpenGLPixelFormat				*pixelFormat = nil;
 	TQ3Status						qd3dStatus;
    NSRect							viewFrame;
    GLint							enable;
	TQ3GLExtensions					extFlags;
	TQ3GLContext					sharingContext = nullptr;
	NSView*							theView = nil;
	GLint							order;

	@autoreleasepool
	{
		// Get the type specific draw context data
		drawContextType = Q3DrawContext_GetType(theDrawContext);
		switch (drawContextType) {
			// NSView
			case kQ3DrawContextTypeCocoa:
				// Get the NSView
				Q3CocoaDrawContext_GetNSView(theDrawContext, &nsView);
				theView = (NSView*) nsView;
				// Previously I checked here that theView has a window, now I do that
				// earlier when the TQ3DrawContextObject is created.
				
				// Check whether an NSOpenGLContext was provided by the client
				if ([theView isKindOfClass: [NSOpenGLView class]])
				{
					glContext = [(NSOpenGLView*) theView openGLContext];
				}
				if (glContext == nil)
				{
					Q3Object_GetProperty( theDrawContext, kQ3DrawContextPropertyNSOpenGLContext,
						sizeof(glContext), nullptr, &glContext );
				}
				
				if (glContext == nil)
				{
					// Check whether a pixel format was provided by the client
					Q3Object_GetProperty( theDrawContext, kQ3DrawContextPropertyGLPixelFormat,
						sizeof(pixelFormat), nullptr, &pixelFormat );


					// If not, create a pixel format here
					if (pixelFormat == nullptr)
					{
						pixelFormat = [[[NSOpenGLPixelFormat alloc]
							initWithAttributes: (NSOpenGLPixelFormatAttribute *)glAttributes]
							autorelease];
					}
					
					// Set up and create the NSOpenGLContext
					if (shareTextures)
					{
						// Attempt to share textures with a previously created context.
						while ( (sharingContext = GLGPUSharing_GetNextSharingBase( sharingContext )) != nullptr )
						{
							CQ3GLContext*	theOldContext = static_cast<CQ3GLContext*>( sharingContext );
							CocoaGLContext* oldCocoaContext = reinterpret_cast<CocoaGLContext*>( theOldContext );
							if (oldCocoaContext != nil)
							{
								NSOpenGLContext* prevGLContext = oldCocoaContext->glContext;
								glContext = [[NSOpenGLContext alloc] initWithFormat: pixelFormat
																	shareContext: prevGLContext];
								if (glContext != nil)
								{
									//Q3_LOG_FMT("Quesa created shared context %p", glContext);
									break;
								}
							}
						}
					}
					
					if (glContext == nil)
					{
						glContext = [[NSOpenGLContext alloc] initWithFormat: pixelFormat
															shareContext: nil];
						//Q3_LOG_FMT("Quesa created unshared context %p", glContext);
					}
					
					GLGPUSharing_AddContext( this, sharingContext );
					
					createdContext = YES;
				} // end of case that the client did not provide an NSOpenGLContext
				else // the client did provide an NSOpenGLContext
				{
					// Retain the context, so that whether or not we created it,
					// we will have a reference to it.
					[glContext retain];
				
					sharingContext = nullptr;
					
					NSArray<NSOpenGLContext*>* sharingPartners = nil;
					if ([glContext respondsToSelector: @selector(sharingPartners)])
					{
						sharingPartners = [glContext sharingPartners];
					}
					if ( (sharingPartners != nil) && (sharingPartners.count > 0) )
					{
						std::vector<TQ3GLContext> allContexts;
						GLGPUSharing_GetAllContexts( allContexts );
						
						for (NSOpenGLContext* aPartner in sharingPartners)
						{
							for (TQ3GLContext oldContext : allContexts)
							{
								CQ3GLContext*	theOldContext = static_cast<CQ3GLContext*>( oldContext );
								CocoaGLContext* oldCocoaContext = reinterpret_cast<CocoaGLContext*>( theOldContext );
								if (oldCocoaContext != nil)
								{
									NSOpenGLContext* prevGLContext = oldCocoaContext->glContext;
									if (prevGLContext == aPartner)
									{
										// Found the context to share with.
										sharingContext = oldContext;
										break;
									}
								}
							}
							if (sharingContext != nullptr)
							{
								break;
							}
						}
					}
					
					Q3_MESSAGE_FMT("New context %p shares with old %p", this, sharingContext );
					GLGPUSharing_AddContext( this, sharingContext );
				}

				// Associate the NSOpenGLContext with the Quesa context in case the client app needs it
				Q3Object_SetProperty( theDrawContext, kQ3DrawContextPropertyNSOpenGLContext, sizeof(glContext), &glContext );
	
				// Set the NSView as the NSOpenGLContext's drawable
				[glContext setView: theView];
				[glContext makeCurrentContext];


				// Get the view bounds from the NSView for the initial gl viewport
				// and the default draw context pane if it's needed.
				viewFrame = theView.bounds;
				if (E3CocoaDrawContext_IsConvertToBackingNeeded())
				{
					viewFrame = [theView convertRectToBacking: viewFrame];
				}
				// Previously I was getting a cached version of the view frame because
				// -[NSView bounds] can't be called from a thread other than the main
				// thread.  But as of Catalina, more methods must be called from the
				// main thread, such as -[NSOpenGLContext setView:] and
				// -[NSOpenGLContext update], so I have given up on using NSView-based
				// Quesa rendering off the main thread.
			  break;

			
			// Unsupported
			default:
				throw std::exception();
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
		[glContext makeCurrentContext];



		// Set the viewport
		viewPort[0] = (GLint) drawContextData.pane.min.x;
		viewPort[1] = (GLint) ((viewFrame.origin.y+viewFrame.size.height)            
										  - drawContextData.pane.max.y);
		viewPort[2] = (GLint) (drawContextData.pane.max.x - drawContextData.pane.min.x);
		viewPort[3] = (GLint) (drawContextData.pane.max.y - drawContextData.pane.min.y);

		glViewport( viewPort[0], viewPort[1], viewPort[2], viewPort[3] );
		Q3_MESSAGE_FMT("CocoaGLContext::CocoaGLContext %p port %dx%d", this, viewPort[2], viewPort[3] );


		// Set the swap buffer rect
		enable = 1;
		[glContext setValues:&enable forParameter:NSOpenGLCPSwapRectangleEnable];
		[glContext setValues:viewPort forParameter:NSOpenGLCPSwapRectangle];



		// Get the function pointer to bind an FBO, if possible.  This cannot be done until
		// after the context has been made current.
		GLUtils_CheckExtensions( &extFlags );
		GLGetProcAddress( bindFrameBufferFunc, "glBindFramebuffer", "glBindFramebufferEXT" );



		// Sync to monitor refresh rate?
		TQ3Boolean	doSync;
		if (kQ3Success == Q3Object_GetProperty( theDrawContext,
			kQ3DrawContextPropertySyncToRefresh, sizeof(doSync), nullptr, &doSync ))
		{
			if (doSync)
			{
				enable = 1;
			}
			else
			{
				enable = 0;
			}
			
			[glContext	setValues:&enable forParameter:NSOpenGLCPSwapInterval];
		}
	} // end autoreleasepool
}

CocoaGLContext::~CocoaGLContext()
{
	@autoreleasepool
	{
		// Workaround for a weird problem: In some cases I destroyed the GL context for a
		// certain NSView, and later created a new GL context for that same view.
		// At the point where the CocoaGLContext constructor calls -[NSOpenGLContext setView:],
		// the last image of the deceased GL context shows up.  So I move the context behind the
		// window, where it will likely be hidden.
		GLint order = -1;
		[glContext setValues: &order forParameter: NSOpenGLCPSurfaceOrder];
		

		// Close down the context
		if (createdContext)
		{
			[glContext clearDrawable];
		}
		[NSOpenGLContext clearCurrentContext];


		// Release the context.  We have a reference to it, whether or not
		// we created it.
		Q3_LOG_FMT("NSOpenGLContext release %p by ~CocoaGLContext", glContext);
		[glContext release];
	}
}
	
void	CocoaGLContext::SwapBuffers()
{
	// Previously there was a glFlush call here, with a comment that it doesn't
	// seem to draw without it.  But now it does seem fine without glFlush, and
	// Apple docs explicitly say that one should not call glFlush before
	// flushBuffer.


	// Swap the buffers
	[glContext flushBuffer];
}

void	CocoaGLContext::SetCurrentBase( TQ3Boolean inForceSet )
{
	// The autorelease pool fixes a strange bug seen on OS 10.10 and later.
	// When one CocoaGLContext was destroyed and another was created for the
	// same NSView, the new one disappeared after the first render.
	@autoreleasepool
	{
		// Activate the context
		if (inForceSet || ![[NSOpenGLContext currentContext] isEqual:glContext])
		{
			//Q3_LOG_FMT("Quesa making context %p current", glContext);
			[glContext makeCurrentContext];
			// It's not clear what it could mean for there to be an OpenGL error
			// right after the context has been made current...
			GLenum err;
			while ( (err = glGetError()) != GL_NO_ERROR )
			{
				Q3_MESSAGE_FMT("%s after makeCurrentContext", GLUtils_GLErrorToString(err));
			}
		}
	}
}

void	CocoaGLContext::SetCurrent( TQ3Boolean inForceSet )
{
	// Activate the context
	SetCurrentBase( inForceSet );
	CHECK_GL_ERROR;
	
	// Make sure that no FBO is active
	if (BindFrameBuffer( GL_FRAMEBUFFER, 0 ))
	{
		CHECK_GL_ERROR;
		glViewport( viewPort[0], viewPort[1], viewPort[2], viewPort[3] );
		CHECK_GL_ERROR;
	}
}


bool	CocoaGLContext::UpdateWindowSize()
{
	[glContext update];
	
	NSView* theView = (NSView*) nsView;
	NSRect viewFrame = theView.bounds;
	if (E3CocoaDrawContext_IsConvertToBackingNeeded())
	{
		viewFrame = [theView convertRectToBacking: viewFrame];
	}
	
	TQ3DrawContextData				drawContextData;
	Q3DrawContext_GetData( quesaDrawContext, &drawContextData );
	
	viewPort[0] = (GLint) drawContextData.pane.min.x;
	viewPort[1] = (GLint) ((viewFrame.origin.y+viewFrame.size.height)            
                                      - drawContextData.pane.max.y);
	viewPort[2] = (GLint) (drawContextData.pane.max.x - drawContextData.pane.min.x);
	viewPort[3] = (GLint) (drawContextData.pane.max.y - drawContextData.pane.min.y);

	glViewport( viewPort[0], viewPort[1], viewPort[2], viewPort[3] );
	
	[glContext setValues: viewPort
					forParameter:NSOpenGLCPSwapRectangle];
	
	//NSLog(@"CocoaGLContext::UpdateWindowSize %p view bounds %@, glViewport (%d, %d, %d, %d)",
	//	this, NSStringFromRect(viewFrame),
	//	viewPort[0], viewPort[1], viewPort[2], viewPort[3] );
	
	return true;
}

#endif // QUESA_OS_COCOA



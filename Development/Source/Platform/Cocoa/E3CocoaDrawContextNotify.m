/*  NAME:
        E3CocoaDrawContextNotify.m

    DESCRIPTION:
        Notification callbacks for Cocoa support.
        
        Note that this code should really live inside E3CocoaDrawContext.c,
        however since we need to use Objective-C to access the Cocoa API
        then this is handled as a special case.
        
        By registering a notification callback for the NSView associated
        with a Cocoa draw context, we can automatically handle resize
        events for the application.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#import "E3Prefix.h"
#import "E3CocoaDrawContextNotify.h"
#import "E3DrawContext.h"

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define CocoaDrawContextWillCloseNotification @"CocoaDrawContextWillCloseNotification"





//=============================================================================
//      Interface
//-----------------------------------------------------------------------------
@interface QuesaViewWatcher : NSObject
{
	TQ3DrawContextObject		drawContext;
}
- (void) setDrawContext:(TQ3DrawContextObject)theContext;

@end





//=============================================================================
//      Implementation
//-----------------------------------------------------------------------------
@implementation QuesaViewWatcher
- (void) viewDidResize:(NSNotification*)note
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;
    NSRect						viewFrame;



	// Grab our bounds    
	viewFrame = [[note object]bounds];



	// Reset our state, and update the size of the draw context pane    
	instanceData->theState = kQ3XDrawContextValidationAll;
	if (!instanceData->data.common.paneState)
		{
		instanceData->data.common.pane.min.x = viewFrame.origin.x;
		instanceData->data.common.pane.min.y = viewFrame.origin.y;
		instanceData->data.common.pane.max.x = viewFrame.origin.x + viewFrame.size.width;
		instanceData->data.common.pane.max.y = viewFrame.origin.y + viewFrame.size.height;
		}
}
- (void)drawContextWillClose:(NSNotification*)note
{
	[[NSNotificationCenter defaultCenter]removeObserver:self];
	[self release]; // Retained from e3cocoa_nsview_register
}
- (void)setDrawContext:(TQ3DrawContextObject)theContext
{
	drawContext = theContext;
}
@end





//=============================================================================
//      e3cocoa_nsview_register : Register our notification callback.
//-----------------------------------------------------------------------------
void e3cocoa_nsview_register(void *nsView, TQ3DrawContextObject theDrawContext)
{


	// Create our watcher - released in drawContextWillClose
	QuesaViewWatcher* watcher = [QuesaViewWatcher new];
	[watcher setDrawContext:theDrawContext];



	// Hook it into the view
	[[NSNotificationCenter defaultCenter]addObserver:watcher
											selector:@selector(viewDidResize:)
											name:NSViewFrameDidChangeNotification
											object:nsView];

	[[NSNotificationCenter defaultCenter]addObserver:watcher
											selector:@selector(drawContextWillClose:)
											name:CocoaDrawContextWillCloseNotification
											object:nsView];
}





//=============================================================================
//      e3cocoa_nsview_unregister : Unregister our notification callback.
//-----------------------------------------------------------------------------
void e3cocoa_nsview_unregister(void *nsView)
{
	[[NSNotificationCenter defaultCenter]postNotificationName:CocoaDrawContextWillCloseNotification
											object:nsView];
}

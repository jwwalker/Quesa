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
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;
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

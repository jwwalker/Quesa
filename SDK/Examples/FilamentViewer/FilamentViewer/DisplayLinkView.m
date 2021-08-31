//
//  DisplayLinkView.m
//  MiniShadow
//
//  Created by James Walker on 5/6/21.
//

#import "DisplayLinkView.h"

#import <CoreVideo/CoreVideo.h>

@implementation DisplayLinkView
{
    BOOL                _didWake;
	CVDisplayLinkRef	_displayLink;
	CGDirectDisplayID	_curDisplayID;
}

- (instancetype) initWithCoder: (NSCoder*) coder
{
	self = [super initWithCoder:coder];
	
	if (self != nil)
	{
        CVDisplayLinkCreateWithActiveCGDisplays( &_displayLink );
	}
	
	return self;
}

- (void) dealloc
{
	[NSNotificationCenter.defaultCenter removeObserver: self];

	CVDisplayLinkStop( _displayLink );
	CVDisplayLinkRelease( _displayLink );
}

- (void) updateDisplayLinkForCurrentScreen
{
	NSScreen* myScreen = self.window.screen;
	NSDictionary<NSDeviceDescriptionKey, id>* screenInfo = myScreen.deviceDescription;
	NSNumber* screenIDNum = screenInfo[ @"NSScreenNumber" ];
	CGDirectDisplayID displayID = (CGDirectDisplayID) screenIDNum.integerValue;
	
	if ( (displayID != kCGNullDirectDisplay) && (displayID != _curDisplayID) )
	{
		CVDisplayLinkSetCurrentCGDisplay( _displayLink, displayID );
		_curDisplayID = displayID;
	}
}

- (void) awakeFromNib
{
	[super awakeFromNib];
	
	if (! _didWake)
	{
		_didWake = YES;
		
		__weak DisplayLinkView* weakSelf = self;

		[NSNotificationCenter.defaultCenter
			addObserverForName: NSWindowDidChangeScreenNotification
			object: self.window
			queue: nil
			usingBlock:
				^(NSNotification * _Nonnull note)
				{
					if (weakSelf != nil)
					{
						__strong DisplayLinkView* strongSelf = weakSelf;
						[strongSelf updateDisplayLinkForCurrentScreen];
					}
				}];

		// CVDisplayLink notifies when the display is refreshed, but we need
		// to run the render loop on the main thread.  This is accomplished
		// more easily on iOS using CADisplayLink.
		CVDisplayLinkSetOutputHandler( _displayLink,
			^CVReturn(
				CVDisplayLinkRef _Nonnull displayLink,
				const CVTimeStamp * _Nonnull inNow,
				const CVTimeStamp * _Nonnull inOutputTime,
				CVOptionFlags flagsIn,
				CVOptionFlags * _Nonnull flagsOut)
			{
				if (weakSelf != nil)
				{
					__strong DisplayLinkView* strongSelf = weakSelf;
					[strongSelf performSelectorOnMainThread: @selector(refreshOnMainThread)
								withObject: nil
								waitUntilDone: NO];
				}
				return kCVReturnSuccess;
			});
		
		[self updateDisplayLinkForCurrentScreen];
		CVDisplayLinkStart( _displayLink );
	}
}


- (void) refreshOnMainThread
{
	
}

@end

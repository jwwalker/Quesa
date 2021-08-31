//
//  DisplayRefreshNotifier.m
//  FilamentViewer
//
//  Created by James Walker on 6/1/21.
//
/* ___________________________________________________________________________
   COPYRIGHT:
        Copyright (c) 2021, Quesa Developers. All rights reserved.

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

#import "DisplayRefreshNotifier.h"

#import <CoreVideo/CoreVideo.h>


@implementation DisplayRefreshNotifier
{
	NSWindow* __weak	_window;
	BOOL				_onMainThread;
	CVDisplayLinkRef	_displayLink;
	CGDirectDisplayID	_curDisplayID;
}

- (instancetype) initForWindow: (NSWindow*) window
				notifyingOnMainThread: (BOOL) mainThread
{
	self = [super init];
	if (self != nil)
	{
		_window = window;
		_onMainThread = mainThread;
		CVDisplayLinkCreateWithActiveCGDisplays( &_displayLink );
		
		// Get notified if the window moves to a different screen
		DisplayRefreshNotifier* __weak weakSelf = self;
		[NSNotificationCenter.defaultCenter
			addObserverForName: NSWindowDidChangeScreenNotification
			object: _window
			queue: nil
			usingBlock:
				^(NSNotification * _Nonnull note)
				{
					if (weakSelf != nil)
					{
						DisplayRefreshNotifier* strongSelf = weakSelf;
						[strongSelf updateDisplayLinkForCurrentScreen];
					}
				}];
		
		// Get notified when the display refreshes
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
					DisplayRefreshNotifier* strongSelf = weakSelf;
					
					if (strongSelf->_onMainThread)
					{
						[strongSelf performSelectorOnMainThread: @selector(sendNotification)
									withObject: nil
									waitUntilDone: NO];
					}
					else
					{
						[strongSelf sendNotification];
					}
				}
				return kCVReturnSuccess;
			});

		[self updateDisplayLinkForCurrentScreen];
		CVDisplayLinkStart( _displayLink );
	}
	return self;
}

- (void) sendNotification
{
	if (_window != nil)
	{
		[NSNotificationCenter.defaultCenter
			postNotificationName: @"JWDisplayRefresh"
			object: _window];
	}
}

- (void) dealloc
{
	[NSNotificationCenter.defaultCenter removeObserver: self];

	CVDisplayLinkStop( _displayLink );
	CVDisplayLinkRelease( _displayLink );
}

- (void) updateDisplayLinkForCurrentScreen
{
	NSScreen* myScreen = _window.screen;
	NSDictionary<NSDeviceDescriptionKey, id>* screenInfo = myScreen.deviceDescription;
	NSNumber* screenIDNum = screenInfo[ @"NSScreenNumber" ];
	CGDirectDisplayID displayID = (CGDirectDisplayID) screenIDNum.integerValue;
	
	if ( (displayID != kCGNullDirectDisplay) && (displayID != _curDisplayID) )
	{
		CVDisplayLinkSetCurrentCGDisplay( _displayLink, displayID );
		_curDisplayID = displayID;
	}
}


@end

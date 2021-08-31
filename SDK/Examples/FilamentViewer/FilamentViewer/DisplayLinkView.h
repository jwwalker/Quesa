//
//  DisplayLinkView.h
//  MiniShadow
//
//  Created by James Walker on 5/6/21.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

/*!
	@class		DisplayLinkView
	
	@abstract	This view class uses CVDisplayLink to synchronize with the refresh rate of a display.
*/
@interface DisplayLinkView : NSView

- (void) refreshOnMainThread;	// to be overridden

@end

NS_ASSUME_NONNULL_END

/*  NAME:
        Quesa3DView.h

    DESCRIPTION:
        NSView subclass to display a quesa draw context.

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
#import <AppKit/AppKit.h>
#include <QuesaCocoa/Quesa.h>

@interface Quesa3DView : NSView
{
    id 						qd3dDelegate;
    TQ3DrawContextObject	drawContext;
    TQ3ViewObject			qd3dView;
}

- (id)qd3dDelegate;
- (void)setQD3DDelegate:(id)inDelegate;

- (TQ3DrawContextObject)drawContext;
- (TQ3ViewObject)qd3dView;

- (void) createLight:(TQ3ObjectType) lightType withData:(void *)lightData;
//this shouldn't be called directly, but is here for subclasses to override
//(although they shouldn't need to...).
//if you want to 'force' a Quesa3DView to draw a frame, use setNeedsDisplay:YES
// or display
- (void)drawQD3D;

@end


//===========================================================================
//	The Quesa3DViewDelegate is called upon to actually submit the application
//	specific QD3D objects to the TQ3ViewObject.
//
//	The first time through the draw loop, after the Quesa3DView is visible, the 
//	view initializes the draw context and calls qd3dViewDidInit on it's qd3dDelegate.
//	This is a good time to setup lights (default lights are created).
//
//	Each frame, qd3dViewWillRender: is called then starts the
//	Q3View rendering loop. Inside, it qd3dViewRenderFrame is called on the qd3dDelegate
//	on each pass through the frame. This is where the qd3dDelegate should submit the
//	objects to the views TQ3ViewObject.
//
//	After rendering is finished, qd3dViewDidRender is called.
//	At the moment, KeyUp and KeyDown events are also passed to the qd3dView. I'm not sure
//	if this is a good idea or not, but they seem useful there.
//===========================================================================
@interface NSObject(Quesa3DViewDelegate)
-(void)qd3dViewDidInit:(Quesa3DView*)inView;
-(void)qd3dViewWillRender:(Quesa3DView*)inView;
-(void)qd3dViewRenderFrame:(Quesa3DView*)inView;
-(void)qd3dViewDidRender:(Quesa3DView*)inView;
-(void)qd3dView:(Quesa3DView*)inView eventOccurred:(NSEvent*)inEvent;
@end

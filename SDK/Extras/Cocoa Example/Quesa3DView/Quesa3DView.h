/*  NAME:
        Quesa3DView.h

    DESCRIPTION:
        NSView subclass to display a quesa draw context.

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

/*  NAME:
        HIQuesaView.h

    DESCRIPTION:
        Header for HIQuesaView sample code.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

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

#include <Carbon/Carbon.h>
#include <Quesa/Quesa.h>
#include "TView.h"
#include <memory>
class XHIQuesaViewImp;

#define		kHIQuesaViewClassID	CFSTR("org.quesa.hiquesaview")

const ControlKind	kHIQuesaViewControlKind  = { 'Qesa', 'HIQV' };

/*
	kEventClassControl / kEventControlQuesaRender
	
	Summary:
		Quesa rendering should be performed.

	Parameters:
	
	--> kEventParamDirectObject (in, typeControlRef)
			The HIViewRef for the HIQuesaView to render.
*/
enum
{
	kEventControlQuesaRender	= 'QsRn'
};

/*!
	@class		HIQuesaView
	
	@abstract	View that renders using Quesa within a Carbon compositing
				window.
	
	@discussion	When this view is called to draw itself, it draws a border, and
				then sends a kEventControlQuesaRender Carbon event to itself.
				The handler for that event should do the actual rendering.  You
				can install the handler on this view, or a superview, or on the
				owning window.
				
				You can also opt to render at other times, so long as it is in
				the main thread.  Do not be confused by the rule that one should
				draw in a Carbon compositing window only when a Carbon event
				asks you to draw; Quesa does not really draw in the window, but
				rather in an OpenGL drawing surface behind the window.
				
				An HIQuesaView owns a Quesa view object, and assigns it a draw
				context, a renderer, a camera, and a light group.  You are free
				to replace or modify the renderer, camera, or light group.  You
				should not replace the draw context or modify its pane, but you
				may change its clear color.
				
				When the view is moved or resized, it will automatically update
				its draw context.  Unless you have replaced the default camera
				with an orthographic camera, it will also automatically update
				the aspect ratio for the camera.
				
				Initialization parameters, settable in Interface Builder:
				
				'Brdr'	typeFloat	Thickness in pixels.  Default: 1.0.
*/
class HIQuesaView : public TView
{
public:
	/*!
		@function				RegisterClass
		@abstract				Register the class.  This must be called once
								during application initialization.  Calling it
								more than once does no harm.
	*/
	static void					RegisterClass();
	
	/*!
		@function				GetQuesaView
		@abstract				Accessor for Quesa view.  Do not dispose.
	*/
	TQ3ViewObject				GetQuesaView();
	
protected:
								HIQuesaView( HIViewRef inControl );
	virtual						~HIQuesaView();
	
	virtual OSStatus			Initialize(
										TCarbonEvent&		inEvent );

	virtual ControlKind			GetKind();

	virtual void				OwningWindowChanged(
										WindowRef			oldWindow,
										WindowRef			newWindow );

	virtual void				BoundsChanged(
										UInt32				inOptions,
										const HIRect&		inOriginalBounds,
										const HIRect&		inCurrentBounds );

	virtual OSStatus			GetRegion(
										ControlPartCode		inPart,
										RgnHandle			outRgn );

	virtual void				Draw(
										RgnHandle			inLimitRgn,
										CGContextRef		inContext );

private:
	static OSStatus				Construct(
										HIObjectRef			inObjectRef,
										TObject**			outObject );

	std::auto_ptr<XHIQuesaViewImp>	mImp;

	// Unimplemented methods
								HIQuesaView( const HIQuesaView& );
	HIQuesaView&				operator=( const HIQuesaView& );
	
};


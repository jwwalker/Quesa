/*  NAME:
        E3CocoaDrawContext.mm

    DESCRIPTION:
        Cocoa specific draw context implementation.

    COPYRIGHT:
        Copyright (c) 1999-2021, Quesa Developers. All rights reserved.

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
#include "E3Prefix.h"

#if QUESA_OS_COCOA

#import <Cocoa/Cocoa.h>
#include "E3DrawContext.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define CocoaDrawContextWillCloseNotification @"CocoaDrawContextWillCloseNotification"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
@interface QuesaViewWatcher : NSObject

- (instancetype) initWithDrawContext:(TQ3DrawContextObject)theContext
				view: (NSView*) view;

@end



class E3CocoaDrawContext : public E3DrawContext  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
	Q3_CLASS_ENUMS ( kQ3DrawContextTypeCocoa, E3CocoaDrawContext, E3DrawContext )
public :

	TQ3DrawContextUnionData				instanceData ;
	} ;
	


///=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
@implementation QuesaViewWatcher
{
	TQ3DrawContextObject		_drawContext;
	NSView*						_view;
}

- (instancetype) initWithDrawContext:(TQ3DrawContextObject)theContext
				view: (NSView*) view
{
	self = [super init];
	if ( self != nil )
	{
		_drawContext = theContext;
		_view = view;
	}
	return self;
}

- (void) refreshCachedBounds
{
	// Grab our bounds
	NSRect viewBounds = _view.bounds;
	if (E3CocoaDrawContext_IsConvertToBackingNeeded())
	{
		viewBounds = [_view convertRectToBacking: viewBounds];
	}

	// Reset our state, and update the size of the draw context pane    
	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *)
		_drawContext->FindLeafInstanceData();
	instanceData->theState |= kQ3XDrawContextValidationWindowSize;
	if (!instanceData->data.common.paneState)
	{
		instanceData->data.common.pane.min.x = viewBounds.origin.x;
		instanceData->data.common.pane.min.y = viewBounds.origin.y;
		instanceData->data.common.pane.max.x = viewBounds.origin.x + viewBounds.size.width;
		instanceData->data.common.pane.max.y = viewBounds.origin.y + viewBounds.size.height;
	}
}

- (void) viewDidResize: (NSNotification*)note
{
	[self refreshCachedBounds];
}


- (void) drawContextWillClose: (NSNotification*)note
{
	[[NSNotificationCenter defaultCenter] removeObserver: self];
	
	[self release];
}

@end

#pragma mark -

//-----------------------------------------------------------------------------
//      e3drawcontext_cocoa_new : Cocoa draw context new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_cocoa_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DrawContextUnionData			*instanceData = (TQ3DrawContextUnionData *) privateData;
	const TQ3CocoaDrawContextData	*cocoaData    = (const TQ3CocoaDrawContextData *) paramData;



	// Initialise our instance data
	instanceData->data.cocoaData.theData = *cocoaData;

	E3DrawContext_InitialiseData(&instanceData->data.cocoaData.theData.drawContextData);



	// Register our notification callback
	NSView* viewToWatch = (NSView*)cocoaData->nsView;
	QuesaViewWatcher* watcher = [[QuesaViewWatcher alloc]
		initWithDrawContext: theObject
		view: viewToWatch];

	NSNotificationCenter* defaultCenter = [NSNotificationCenter defaultCenter];
	
	[defaultCenter addObserver: watcher
						selector: @selector(viewDidResize:)
						name: NSViewFrameDidChangeNotification
						object: viewToWatch];

	[defaultCenter addObserver: watcher
						selector: @selector(viewDidResize:)
						name: NSWindowDidChangeScreenNotification
						object: viewToWatch.window];

	[defaultCenter addObserver: watcher
						selector: @selector(drawContextWillClose:)
						name: CocoaDrawContextWillCloseNotification
						object: (NSView*)cocoaData->nsView];

	return(kQ3Success);
}





//=============================================================================
//      e3drawcontext_cocoa_delete : Cocoa draw context delete method.
//-----------------------------------------------------------------------------
static void
e3drawcontext_cocoa_delete(TQ3Object theObject, void *privateData)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) privateData;


	// Unregister our notification callback
	@autoreleasepool
	{
		NSNotificationCenter* defaultCenter = [NSNotificationCenter defaultCenter];
		[defaultCenter postNotificationName: CocoaDrawContextWillCloseNotification
					object: (NSView*) instanceData->data.cocoaData.theData.nsView];
	}



	// Dispose of the common instance data
	if (instanceData->data.common.maskState)
		Q3Bitmap_Empty(&instanceData->data.common.mask);
}





//=============================================================================
//      e3drawcontext_cocoa_update : Cocoa draw context update method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_cocoa_update(TQ3DrawContextObject theDrawContext)
{	//TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->FindLeafInstanceData () ;

	// We use a notification to update the view size, so there is no need to
	// check it here.  If we wanted to check for other changes, such as a
	// display device going offline, this would be the place to update the draw
	// context validation state.
	
	return kQ3Success;
}





//=============================================================================
//      e3drawcontext_cocoa_get_dimensions : Cocoa draw context dimensions.
//-----------------------------------------------------------------------------
static void
e3drawcontext_cocoa_get_dimensions(TQ3DrawContextObject theDrawContext, TQ3Area *thePane)
{
	// Return our dimensions
	TQ3DrawContextUnionData* instanceData = (TQ3DrawContextUnionData *) theDrawContext->FindLeafInstanceData();
	TQ3CocoaDrawContextData& cocoaData( instanceData->data.cocoaData.theData );
	NSView* theView = (NSView*) cocoaData.nsView;
	NSRect	viewBounds = theView.bounds;
	if (E3CocoaDrawContext_IsConvertToBackingNeeded())
	{
		viewBounds = [theView convertRectToBacking: viewBounds];
	}
	thePane->min.x = (float) viewBounds.origin.x;
	thePane->min.y = (float) viewBounds.origin.y;
	thePane->max.x = (float) (viewBounds.origin.x + viewBounds.size.width);
	thePane->max.y = (float) (viewBounds.origin.y + viewBounds.size.height);
}





//=============================================================================
//      e3drawcontext_cocoa_metahandler : Cocoa draw context metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3drawcontext_cocoa_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = nullptr;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_cocoa_new;
			break;
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_cocoa_delete;
			break;

		case kQ3XMethodTypeDrawContextUpdate:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_cocoa_update;
			break;

		case kQ3XMethodTypeDrawContextGetDimensions:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_cocoa_get_dimensions;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3CocoaDrawContext_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
TQ3Status
E3CocoaDrawContext_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = Q3_REGISTER_CLASS( kQ3ClassNameDrawContextCocoa,
									e3drawcontext_cocoa_metahandler,
									E3CocoaDrawContext );

	return(qd3dStatus);
}





//=============================================================================
//      E3CocoaDrawContext_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3CocoaDrawContext_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3DrawContextTypeCocoa, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3MacDrawContext_New : Create a new Cocoa draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3CocoaDrawContext_New(const TQ3CocoaDrawContextData *drawContextData)
{
	TQ3DrawContextObject theDC = nullptr;
	NSView* theView = (NSView*) drawContextData->nsView;
	if ( (theView != nil) && ([theView window] != nil) && ([[theView window] windowNumber] > 0) )
	{
		theDC = E3ClassTree::CreateInstance(kQ3DrawContextTypeCocoa, 
											   kQ3False, drawContextData);
	}
	else
	{
		E3ErrorManager_PostError( kQ3ErrorInvalidParameter, kQ3False );
	}
	return theDC;
}





//=============================================================================
//      E3CocoaDrawContext_NewWithWindow : Create a new Cocoa Draw Context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3CocoaDrawContext_NewWithWindow(TQ3ObjectType drawContextType, void *drawContextTarget)
{	TQ3CocoaDrawContextData		cocoaDrawContextData;
	TQ3DrawContextData			drawContextData;
	TQ3DrawContextObject		drawContext;



	// Check we have a suitable target for the draw context
	if (drawContextType != kQ3DrawContextTypeCocoa)
		return(nullptr);


	// Check if the view is valid
	NSView * theView = (NSView *) drawContextTarget;
	if ( (theView == nil) && ([theView window] == nil) && ([[theView window] windowNumber] <= 0) )
	{
		E3ErrorManager_PostError( kQ3ErrorInvalidParameter, kQ3False );
		return(nullptr);
	}


	// Prepare the draw context
	Q3ColorARGB_Set(&drawContextData.clearImageColor, kQ3DrawContextDefaultBackgroundColour);

	drawContextData.clearImageMethod  = kQ3ClearMethodWithColor;
	drawContextData.paneState         = kQ3False;
	drawContextData.maskState		  = kQ3False;
	drawContextData.doubleBufferState = kQ3True;
	drawContextData.pane.min.x        = 0.0f;
	drawContextData.pane.min.y        = 0.0f;
	drawContextData.pane.max.x        = 0.0f;
	drawContextData.pane.max.y        = 0.0f;
	drawContextData.mask.image        = nullptr;
	drawContextData.mask.width        = 0;
	drawContextData.mask.height       = 0;
	drawContextData.mask.rowBytes     = 0;
	drawContextData.mask.bitOrder     = kQ3EndianBig;

	cocoaDrawContextData.drawContextData = drawContextData;
	cocoaDrawContextData.nsView          = theView;



	// Create the draw context
	drawContext = Q3CocoaDrawContext_New(&cocoaDrawContextData);

	return(drawContext);
}





//=============================================================================
//      E3CocoaDrawContext_SetNSView : Set the view for an Cocoa draw context.
//-----------------------------------------------------------------------------
//		Note :	Quesa is not involved in any OS-specific reference counting,
//				and so our caller must ensure that the old NSView is released
//				as appropriate.
//-----------------------------------------------------------------------------
TQ3Status
E3CocoaDrawContext_SetNSView(TQ3DrawContextObject drawContext, void *nsView)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Set the field and reset our flag
	if (instanceData->data.cocoaData.theData.nsView != nsView)
	{
		instanceData->data.cocoaData.theData.nsView = nsView;
		instanceData->theState                     |= kQ3XDrawContextValidationAll;
		Q3Shared_Edited(drawContext);
	}

	return(kQ3Success);
}





//=============================================================================
//      E3CocoaDrawContext_GetDisplay : Get the display for an Cocoa draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3CocoaDrawContext_GetNSView(TQ3DrawContextObject drawContext, void **nsView)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Get the field
	*nsView = instanceData->data.cocoaData.theData.nsView;

	return(kQ3Success);
}


/*!
	@function	E3CocoaDrawContext_IsConvertToBackingNeeded
	@abstract	Test whether we need to use convertRectToBacking: when getting the bounds of
				the Cocoa view.
	@discussion	In macOS 10.15 (Catalina), Apple made a change in the way that NSOpenGLView
				behaves on a Retina display that made this step necessary.
				
				The reason for not using @available( macOS 10.15, * ) is that it apparently creates
				a compatibility problem when using a library built with a newer Xcode within an older
				version of Xcode.  <https://developer.apple.com/forums/thread/123003>
*/
bool	E3CocoaDrawContext_IsConvertToBackingNeeded( void )
{
	return NSAppKitVersionNumber >= 1894.0; // NSAppKitVersionNumber10_15 = 1894
}

#endif // QUESA_OS_COCOA

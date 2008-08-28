/*  NAME:
        E3MacDrawContext.c

    DESCRIPTION:
        Macintosh specific Draw Context calls.
        
        Only WindowPtr based Mac draw contexts are supported at present.

    COPYRIGHT:
        Copyright (c) 1999-2008, Quesa Developers. All rights reserved.

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
#include "E3Prefix.h"
#include "E3DrawContext.h"

#if QUESA_SUPPORT_HITOOLBOX

#if !QUESA_UH_IN_FRAMEWORKS
	#include <QuickDraw.h>
	#include <MacWindows.h>
#endif




//=============================================================================
//      Constants
//-----------------------------------------------------------------------------


const UInt32 kEventQuesaDisplayChange	= 'QDsp';




//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------



class E3MacDrawContext : public E3DrawContext  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3DrawContextTypeMacintosh, E3MacDrawContext, E3DrawContext )
public :

	TQ3DrawContextUnionData				instanceData ;
	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------


//=============================================================================
//      e3drawcontext_mac_checkregions : Check for changes to the draw regions.
//-----------------------------------------------------------------------------
//		Note :	We test our window for changes which could affect the draw
//				regions, and return the flags which have changed.
//
//				If we've received a Display Manager notification (e.g., depth,
//				resolution, or number of monitors has changed), we reset
//				everything.
//
//				This is called once per frame, so must have minimal overhead.
//-----------------------------------------------------------------------------
static TQ3XDrawContextValidation
e3drawcontext_mac_checkregions(TQ3DrawContextObject theDrawContext)
{
	TQ3DrawContextUnionData			*instanceData =
		(TQ3DrawContextUnionData *) theDrawContext->FindLeafInstanceData () ;
	E3GlobalsPtr					theGlobals    = E3Globals_Get();
	TQ3XDrawContextValidation		stateChanges;
	TQ3MacDrawContext2DLibrary		theLibrary;
	Rect							windowRect;
	WindowRef						theWindow;
	TQ3MacDrawContextState			*macState;
	


	// If a Display Manager notification has occurred, reset everything
	if (theGlobals->dmNotifiedChanges)
		{
		theGlobals->dmNotifiedChanges = kQ3False;
		return(kQ3XDrawContextValidationAll);
		}



	// Initialise ourselves
	stateChanges = kQ3XDrawContextValidationClearFlags;
	macState     = &instanceData->data.macData;
	theWindow    = (WindowRef) macState->theData.window;



	// Work out what type of draw context we're dealing with, watching out
	// for people who specified QuickDraw when they really wanted a window.
	theLibrary = macState->theData.library;
	if (theLibrary == kQ3Mac2DLibraryQuickDraw &&
		theWindow  != NULL                     &&
		theWindow  == (WindowRef) macState->theData.grafPort)
		theLibrary = kQ3Mac2DLibraryNone;



	// Handle each draw context type in turn
	switch (theLibrary) {
		case kQ3Mac2DLibraryNone:
			// Get the window bounds
			GetWindowBounds(theWindow, kWindowContentRgn, &windowRect);



			// Check to see if the pane state has changed
			if (macState->theData.drawContextData.paneState != macState->paneState)
				stateChanges |= kQ3XDrawContextValidationPane;

			if (macState->paneState)
				{
				if (macState->theData.drawContextData.pane.min.x != macState->thePane.min.x ||
					macState->theData.drawContextData.pane.min.y != macState->thePane.min.y ||
					macState->theData.drawContextData.pane.max.x != macState->thePane.max.x ||
					macState->theData.drawContextData.pane.max.y != macState->thePane.max.y)
					stateChanges |= kQ3XDrawContextValidationPane;
				}



			// Check to see if the window has changed position.
			if (windowRect.top  != macState->windowRect.top ||
				windowRect.left != macState->windowRect.left)
				{
				stateChanges |= kQ3XDrawContextValidationWindowPosition;
				}



			// Check to see if the window has changed size.
			if (E3Rect_GetWidth(&windowRect)  != E3Rect_GetWidth(&macState->windowRect) ||
				E3Rect_GetHeight(&windowRect) != E3Rect_GetHeight(&macState->windowRect))
				{
				stateChanges |= kQ3XDrawContextValidationWindowSize;
				}



			// Save the details for next time
			macState->paneState  = macState->theData.drawContextData.paneState;
			macState->thePane    = macState->theData.drawContextData.pane;
			macState->windowRect = windowRect;
			break;

		
		case kQ3Mac2DLibraryQuickDraw:
		case kQ3Mac2DLibraryQuickDrawGX:
			// Not supported
			break;

			
		case kQ3Mac2DLibrarySize32:
		default:
			Q3_ASSERT(!"Unknown library");
			break;
		}



	// Return the state changes
	return(stateChanges);
}





#if !TARGET_API_MAC_OS8
//=============================================================================
//      e3drawcontext_mac_display_change_handler : Handle a Carbon event notifying
//				us that the display configuration changed.
//-----------------------------------------------------------------------------
static pascal OSStatus
e3drawcontext_mac_display_change_handler( EventHandlerCallRef inHandlerCallRef,
										EventRef inEvent, void *inUserData )
{
#pragma unused( inHandlerCallRef, inEvent )
	TQ3XDrawContextValidation*	theState = (TQ3XDrawContextValidation*)inUserData;
	*theState = kQ3XDrawContextValidationAll;
	
	// Let other handlers see the event too
	return eventNotHandledErr;
}
#endif





//=============================================================================
//      e3drawcontext_mac_new : Mac draw context new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_mac_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DrawContextUnionData			*instanceData = (TQ3DrawContextUnionData *) privateData;
	const TQ3MacDrawContextData		*macData      = (const TQ3MacDrawContextData *) paramData;
	TQ3MacDrawContext2DLibrary		theLibrary;
	WindowRef						theWindow;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->data.macData.theData   = *macData;
	instanceData->data.macData.paneState = macData->drawContextData.paneState;
	instanceData->data.macData.thePane   = macData->drawContextData.pane;

#if !TARGET_API_MAC_OS8
	instanceData->data.macData.displayNotificationHandler = NULL;
	static EventHandlerUPP	sHandlerUPP = NewEventHandlerUPP(
		e3drawcontext_mac_display_change_handler );
	EventTypeSpec	evtSpec = {
		kEventClassApplication, kEventQuesaDisplayChange
	};
	InstallEventHandler( GetApplicationEventTarget(), sHandlerUPP, 1, &evtSpec,
		&instanceData->theState, &instanceData->data.macData.displayNotificationHandler );
#endif

	E3DrawContext_InitaliseData(&instanceData->data.macData.theData.drawContextData);



	// Save the initial window bounds and visible region, watching out for
	// people who specified QuickDraw when they really wanted a window.
	theLibrary = instanceData->data.macData.theData.library;
	if (theLibrary == kQ3Mac2DLibraryQuickDraw            &&
		instanceData->data.macData.theData.window != NULL &&
		instanceData->data.macData.theData.window == (CWindowPtr)instanceData->data.macData.theData.grafPort)
		theLibrary = kQ3Mac2DLibraryNone;

	if (theLibrary == kQ3Mac2DLibraryNone)
		{
		// Get the window
		theWindow = (WindowRef) instanceData->data.macData.theData.window;


		// Grab its bounds and visible region
		GetWindowBounds(theWindow, kWindowContentRgn, &instanceData->data.macData.windowRect);
		}



	return(kQ3Success);
}





//=============================================================================
//      e3drawcontext_mac_delete : Mac draw context delete method.
//-----------------------------------------------------------------------------
static void
e3drawcontext_mac_delete(TQ3Object theObject, void *privateData)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) privateData;
	TQ3Status					qd3dStatus = kQ3Success;
#pragma unused(theObject)


#if !TARGET_API_MAC_OS8
	if (instanceData->data.macData.displayNotificationHandler != NULL)
	{
		RemoveEventHandler( instanceData->data.macData.displayNotificationHandler );
	}
#endif

	// Dispose of the common instance data
	if (instanceData->data.common.maskState)
		qd3dStatus = Q3Bitmap_Empty(&instanceData->data.common.mask);
}





//=============================================================================
//      e3drawcontext_mac_update : Mac draw context update method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_mac_update(TQ3DrawContextObject theDrawContext)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->FindLeafInstanceData () ;
	TQ3XDrawContextValidation	stateChanges;
	TQ3Status					qd3dStatus = kQ3Success;



	// If we don't have any draw regions, or everything has changed, rebuild them
	if (instanceData->theState == kQ3XDrawContextValidationAll)
		{
		// Rebuild the draw regions
		// update paneState, thePane, windowRect fields of TQ3MacDrawContextState
		e3drawcontext_mac_checkregions(theDrawContext);
		}



	// Otherwise, check to see if they need to be updated
	else
		{
		// Test to see if the draw regions need updating
		stateChanges = e3drawcontext_mac_checkregions(theDrawContext);
		if (stateChanges != kQ3XDrawContextValidationClearFlags)
			{
			// If something changed, update the state flag
			instanceData->theState |= stateChanges;
			}
		}
	
	return(qd3dStatus);		
}





//=============================================================================
//      e3drawcontext_mac_get_dimensions : Mac draw context dimensions.
//-----------------------------------------------------------------------------
static void
e3drawcontext_mac_get_dimensions(TQ3DrawContextObject theDrawContext, TQ3Area *thePane)
{
	TQ3DrawContextUnionData		*instanceData =
		(TQ3DrawContextUnionData *) theDrawContext->FindLeafInstanceData () ;
	WindowRef					theWindow;
	Rect						theRect;



	// Get our window and its bounds
	theWindow = (WindowRef) instanceData->data.macData.theData.window;
	Q3_ASSERT_VALID_PTR(theWindow);
	GetWindowPortBounds( theWindow, &theRect);



	// Return our dimensions
	thePane->min.x = (float) theRect.left;
	thePane->min.y = (float) theRect.top;
	thePane->max.x = (float) theRect.right;
	thePane->max.y = (float) theRect.bottom;
}





//=============================================================================
//      e3drawcontext_mac_metahandler : Mac draw context metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3drawcontext_mac_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_mac_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_mac_delete;
			break;

		case kQ3XMethodTypeDrawContextUpdate:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_mac_update;
			break;

		case kQ3XMethodTypeDrawContextGetDimensions:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_mac_get_dimensions;
			break;
		}
	
	return(theMethod);
}


//=============================================================================
//      e3drawcontext_mac_notify_display_change :	Notify draw contexts of a
//													display configuration change.
//-----------------------------------------------------------------------------
static void
e3drawcontext_mac_notify_display_change()
{
#if TARGET_API_MAC_OS8
	// This older technique will only work for one draw context
	E3GlobalsPtr	theGlobals = E3Globals_Get();

	// Set our flag
	Q3_ASSERT_VALID_PTR( theGlobals );
	theGlobals->dmNotifiedChanges = kQ3True;
#else
	EventRef	theEvent = NULL;
	CreateEvent( NULL, kEventClassApplication, kEventQuesaDisplayChange,
		0.0, kEventAttributeNone, &theEvent );
	if (theEvent)
	{
		SendEventToEventTarget( theEvent, GetApplicationEventTarget() );
	}
#endif
	
}





#if TARGET_RT_MAC_MACHO && (MAC_OS_X_VERSION_MIN_REQUIRED >= 1030)
//=============================================================================
//      e3drawcontext_mac_cg_notify : Core Graphics display callback.
//-----------------------------------------------------------------------------
static void
e3drawcontext_mac_cg_notify( CGDirectDisplayID display,
                             CGDisplayChangeSummaryFlags flags,
                             void *userInfo )
{
#pragma unused( display, userInfo )
	if ( (flags & kCGDisplayBeginConfigurationFlag) != 0 )
	{
		e3drawcontext_mac_notify_display_change();
	}
}


#else
//=============================================================================
//      e3drawcontext_mac_dm_notify : Display Manager callback.
//-----------------------------------------------------------------------------
static pascal void
e3drawcontext_mac_dm_notify(AppleEvent *theEvent)
{
#pragma unused(theEvent)
	e3drawcontext_mac_notify_display_change();
}
#endif





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3MacDrawContext_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3MacDrawContext_RegisterClass(void)
{	E3GlobalsPtr			theGlobals = E3Globals_Get();
	TQ3Status				qd3dStatus;




#if TARGET_RT_MAC_MACHO && (MAC_OS_X_VERSION_MIN_REQUIRED >= 1030)
	// Register to be notified of display configuration changes
	CGDisplayRegisterReconfigurationCallback( e3drawcontext_mac_cg_notify, theGlobals );

#else

	// Register our Display Manager notification callback
	theGlobals->dmNotifyUPP = NewDMNotificationUPP(e3drawcontext_mac_dm_notify);
	if (theGlobals->dmNotifyUPP != NULL)
		{
		ProcessSerialNumber		thePSN;
		GetCurrentProcess(&thePSN);
		DMRegisterNotifyProc(theGlobals->dmNotifyUPP, &thePSN);
		}
#endif


	// Register the class
	qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameDrawContextMac,
										e3drawcontext_mac_metahandler,
										E3MacDrawContext ) ;

	return(qd3dStatus);
}





//=============================================================================
//      E3MacDrawContext_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_UnregisterClass(void)
{	E3GlobalsPtr			theGlobals = E3Globals_Get();
	TQ3Status				qd3dStatus;




#if TARGET_RT_MAC_MACHO && (MAC_OS_X_VERSION_MIN_REQUIRED >= 1030)
	CGDisplayRemoveReconfigurationCallback( e3drawcontext_mac_cg_notify, theGlobals );
#else
	// Unregister our Display Manager notification callback
	if (theGlobals->dmNotifyUPP != NULL)
		{
		ProcessSerialNumber		thePSN;
		GetCurrentProcess(&thePSN);
		DMRemoveNotifyProc(theGlobals->dmNotifyUPP, &thePSN);

		DisposeDMNotificationUPP(theGlobals->dmNotifyUPP);
		theGlobals->dmNotifyUPP = NULL;
		}
#endif


	// Unregister the classes
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3DrawContextTypeMacintosh, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3MacDrawContext_New : Create a new Mac draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3MacDrawContext_New(const TQ3MacDrawContextData *drawContextData)
	{
	// Create the object
	return E3ClassTree::CreateInstance(kQ3DrawContextTypeMacintosh, kQ3False, drawContextData);
	}





//=============================================================================
//      E3MacDrawContext_NewWithWindow : Create a new Mac Draw Context object.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3MacDrawContext_NewWithWindow(TQ3ObjectType drawContextType, void *drawContextTarget)
{	TQ3MacDrawContextData	macDrawContextData;
	TQ3DrawContextData		drawContextData;
	TQ3DrawContextObject	drawContext;



	// Check we have a suitable target for the draw context
	if (drawContextType != kQ3DrawContextTypeMacintosh)
		return(NULL);



	// Prepare the draw context
	Q3ColorARGB_Set(&drawContextData.clearImageColor, kQ3DrawContextDefaultBackgroundColour);

	drawContextData.clearImageMethod  = kQ3ClearMethodWithColor;
	drawContextData.paneState         = kQ3False;
	drawContextData.maskState		  = kQ3False;
	E3Memory_Clear( &drawContextData.mask, sizeof(drawContextData.mask) );
	drawContextData.doubleBufferState = kQ3True;
	drawContextData.pane.min.x        = 0.0f;
	drawContextData.pane.min.y        = 0.0f;
	drawContextData.pane.max.x        = 0.0f;
	drawContextData.pane.max.y        = 0.0f;

	macDrawContextData.drawContextData = drawContextData;
	macDrawContextData.window          = (CWindowPtr) drawContextTarget;
	macDrawContextData.library         = kQ3Mac2DLibraryNone;
	macDrawContextData.viewPort        = NULL;
	macDrawContextData.grafPort        = NULL;
	


	// Create the draw context
	drawContext = Q3MacDrawContext_New(&macDrawContextData);

	return(drawContext);
}





//=============================================================================
//      E3MacDrawContext_SetWindow : Set the window.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_SetWindow(TQ3DrawContextObject drawContext, CWindowPtr window)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Set the window and reset our flag
	if (instanceData->data.macData.theData.window != window)
		{
		instanceData->data.macData.theData.window = window;
		instanceData->theState                   |= kQ3XDrawContextValidationAll;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_GetWindow : Return the window.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_GetWindow(TQ3DrawContextObject drawContext, CWindowPtr *window)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the window
	*window = instanceData->data.macData.theData.window;
	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_SetGXViewPort : Set the GX viewport.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_SetGXViewPort(TQ3DrawContextObject drawContext, gxViewPort viewPort)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Set the GX viewport and reset our flag
	if (instanceData->data.macData.theData.viewPort != viewPort)
		{
		instanceData->data.macData.theData.viewPort = viewPort;
		instanceData->theState                     |= kQ3XDrawContextValidationAll;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_GetGXViewPort : Return the GX viewport.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_GetGXViewPort(TQ3DrawContextObject drawContext, gxViewPort *viewPort)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the GX viewport
	*viewPort = instanceData->data.macData.theData.viewPort;
	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_SetGrafPort : Set the GrafPort.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_SetGrafPort(TQ3DrawContextObject drawContext, CGrafPtr grafPort)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Set the GrafPort and reset the flag
	if (instanceData->data.macData.theData.grafPort != grafPort)
		{
		instanceData->data.macData.theData.grafPort = grafPort;
		instanceData->theState                     |= kQ3XDrawContextValidationAll;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_GetGrafPort : Return the GrafPort.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_GetGrafPort(TQ3DrawContextObject drawContext, CGrafPtr *grafPort)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the GrafPort
	*grafPort = instanceData->data.macData.theData.grafPort;
	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_Set2DLibrary : Set the 2D library.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_Set2DLibrary(TQ3DrawContextObject drawContext, TQ3MacDrawContext2DLibrary library)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Set the 2D library and reset our flag
	if (instanceData->data.macData.theData.library != library)
		{
		instanceData->data.macData.theData.library = library;
		instanceData->theState                    |= kQ3XDrawContextValidationAll;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_Get2DLibrary : Return the 2D library.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_Get2DLibrary(TQ3DrawContextObject drawContext, TQ3MacDrawContext2DLibrary *library)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the 2D library
	*library = instanceData->data.macData.theData.library;
	return(kQ3Success);
}



#endif	// QUESA_SUPPORT_HITOOLBOX


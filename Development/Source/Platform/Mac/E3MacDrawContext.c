/*  NAME:
        E3MacDrawContext.c

    DESCRIPTION:
        Macintosh specific Draw Context calls.
        
        Only WindowPtr based Mac draw contexts are supported at present.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#include "E3Prefix.h"
#include "E3DrawContext.h"





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#define kMaxDevices									50





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3drawcontext_mac_isactivedevice : Is a device active?
//-----------------------------------------------------------------------------
static TQ3Boolean
e3drawcontext_mac_isactivedevice(GDHandle theDevice)
{	TQ3Boolean		isActive;



	// Test for an active screen device
	isActive = (TQ3Boolean) (TestDeviceAttribute(theDevice, screenDevice) &&
							 TestDeviceAttribute(theDevice, screenActive));

	return(isActive);
}





//=============================================================================
//      e3drawcontext_mac_isactiveregion : Is a draw region active?
//-----------------------------------------------------------------------------
//		Note :	Given a particular draw region, we return as it is active for
//				the associated draw context.
//
//				We currently only support WindowPtr based draw contexts.
//
//				A draw region is active when some portion of the draw context
//				falls within the draw region. We return the area of the draw
//				region that is active, and the area of the window.
//
//				We adjust clipRgn to reflect the visible portion of the window,
//				and indicate if the window is completely or partially visible
//				by updating clipMaskState.
//
//				All coordinates are relative to the draw region's device.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3drawcontext_mac_isactiveregion(TQ3DrawContextObject	theDrawContext,
									TQ3Uns32			theRegion,
									TQ3Area				*deviceArea,
									TQ3Area				*windowArea,
									TQ3XClipMaskState	*clipMaskState,
									RgnHandle			clipRgn)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->instanceData;
	Rect						windowRect, deviceRect, overlapRect, paneRect;
	Rect						portRect, visRect, contentRect;
	TQ3MacDrawContext2DLibrary	theLibrary;
	GDHandle					theDevice;
	TQ3Boolean					isActive;
	CWindowPtr					theWindow;
	RgnHandle					visRgn;				// visible region of window



	// Work out what type of draw context we're dealing with, watching out
	// for people who specified QuickDraw when they really wanted a window.
	theLibrary = instanceData->data.macData.theData.library;
	if (theLibrary == kQ3Mac2DLibraryQuickDraw            &&
		instanceData->data.macData.theData.window != NULL &&
		instanceData->data.macData.theData.window == (CWindowPtr)instanceData->data.macData.theData.grafPort)
		theLibrary = kQ3Mac2DLibraryNone;
		


	// Handle each draw context type in turn
	switch (theLibrary) {
		case kQ3Mac2DLibraryNone:
			// Get the window and the draw region's device
			theWindow = instanceData->data.macData.theData.window;
			theDevice = (GDHandle)   instanceData->drawRegions[theRegion].platformHandle;



			// Work out rectangles. If a pane has been specified, we adjust the window
			// rect accordingly (i.e., as if the window was the size of the pane).
			// Work out regions, too, as we'll need them shortly.
			visRgn = NewRgn();
			if (visRgn == NULL)
				return(kQ3False);

			GetWindowContentRect( theWindow, &windowRect );
			GetPortVisibleRegion( GetWindowPort(theWindow), visRgn );
			GetPortBounds( GetWindowPort(theWindow), &portRect );
			contentRect = windowRect;
			GetRegionBounds( visRgn, &visRect );

			deviceRect = (*theDevice)->gdRect;

			if (instanceData->data.common.paneState)
				{
				E3Area_ToRect(&instanceData->data.common.pane, &paneRect);
				OffsetRect(&paneRect, windowRect.left, windowRect.top);
				windowRect = paneRect;
				}



			// Check to see if the draw region is active (i.e., it intersects the window)
			isActive = (TQ3Boolean) SectRect(&windowRect, &deviceRect, &overlapRect);
			if (isActive)
				{
				// Return the active area of the draw region and the window area
				OffsetRect(&overlapRect, -deviceRect.left, -deviceRect.top);
				OffsetRect(&windowRect,  -deviceRect.left, -deviceRect.top);

				E3Area_FromRect(deviceArea, &overlapRect);
				E3Area_FromRect(windowArea, &windowRect);



				// If the window is completely visible, we return an empty region.
				// Otherwise, we return the visible region in device coordinates.
				if (IsRegionRectangular(visRgn) && EqualRect( &visRect, &portRect ))
					{
					*clipMaskState = kQ3XClipMaskFullyExposed;
					RectRgn(clipRgn, &portRect);
					}
				else
					{
					*clipMaskState = kQ3XClipMaskPartiallyExposed;
					CopyRgn(visRgn, clipRgn);
					}

				OffsetRgn(clipRgn, contentRect.left, contentRect.top);
				}
			
			DisposeRgn(visRgn);
			break;
		
		
		case kQ3Mac2DLibraryQuickDraw:
		case kQ3Mac2DLibraryQuickDrawGX:
			// Not supported
			isActive = kQ3False;
			break;
		}



	// Return as the draw region is active
	return(isActive);
}





//=============================================================================
//      e3drawcontext_mac_createregions : Create some draw regions.
//-----------------------------------------------------------------------------
//		Note :	We create the specified number of draw regions, disposing of
//				any existing Mac clipping RgnHandles before rebuilding the
//				draw regions.
//
//				We need to do this ourselves, since E3DrawContext_CreateRegions
//				doesn't know that these are RgnHandle structures.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_mac_createregions(TQ3DrawContextObject theDrawContext, TQ3Uns32 numRegions)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->instanceData;
	TQ3Status					qd3dStatus;
	TQ3Uns32					n;



	// Dispose of any clipping regions that already exist
	for (n = 0; n < instanceData->numDrawRegions; n++)
		{
		if (instanceData->drawRegions[n].platformClip != NULL)
			{
			DisposeRgn((RgnHandle) instanceData->drawRegions[n].platformClip);
			instanceData->drawRegions[n].platformClip = NULL;
			}
		}



	// Create the draw regions we need
	qd3dStatus = E3DrawContext_CreateRegions(theDrawContext, numRegions);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);

	return(qd3dStatus);
}





//=============================================================================
//      e3drawcontext_mac_buildregions : Build the draw regions we need.
//-----------------------------------------------------------------------------
//		Note :	We create a single draw region for each monitor connected to
//				the system, and leave them all in a disabled state.
//
//				We fill in the generic fields (pixel format, etc), but leave
//				the rendering specific fields (position, size, etc) blank.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_mac_buildregions(TQ3DrawContextObject theDrawContext)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->instanceData;
	TQ3Uns32					n, numRegions, rowBytes, pixelBytes;
	GDHandle					activeDevices[kMaxDevices];
	TQ3Status					qd3dStatus;
	PixMapHandle				thePixMap;
	GDHandle					theDevice;
	TQ3XDevicePixelType			pixelType;



	// Work out how many draw regions we need
	numRegions = 0;
	theDevice  = GetDeviceList();
	while (theDevice != NULL && numRegions < kMaxDevices)
		{
		// Check this device, and save it if it's active
		if (e3drawcontext_mac_isactivedevice(theDevice))
			{
			activeDevices[numRegions] = theDevice;
			numRegions++;
			}


		// And move on to the next device
		theDevice = GetNextDevice(theDevice);
		}

	if (numRegions == 0)
		return(kQ3Failure);



	// Create the draw regions we need
	qd3dStatus = e3drawcontext_mac_createregions(theDrawContext, numRegions);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Configure them		
	for (n = 0; n < instanceData->numDrawRegions; n++)
		{
		// Grab the info we need for the device
		thePixMap = (*activeDevices[n])->gdPMap;
		Q3_ASSERT_VALID_PTR(thePixMap);
		
		rowBytes   = (*thePixMap)->rowBytes & 0x7FFF;
		pixelBytes = (*thePixMap)->pixelSize / 8;
		pixelType  = E3DrawContext_GetDevicePixelTypeFromBPP((*thePixMap)->pixelSize);



		// Configure the draw region. Note that we don't bother setting up
		// the colorDescriptor field of the draw region descriptor since
		// these fields aren't currently used by the Interactive Renderer.
		//
		// Since we want to move away from draw regions, this is OK for now.
		instanceData->drawRegions[n].platformHandle          = (TQ3Uns32) activeDevices[n];
		instanceData->drawRegions[n].theDescriptor.width	 = (*thePixMap)->bounds.right - (*thePixMap)->bounds.left;
		instanceData->drawRegions[n].theDescriptor.height	 = (*thePixMap)->bounds.bottom - (*thePixMap)->bounds.top;
		instanceData->drawRegions[n].theDescriptor.rowBytes	 = rowBytes;
		instanceData->drawRegions[n].theDescriptor.pixelSize = pixelBytes;
		instanceData->drawRegions[n].theDescriptor.pixelType = pixelType;
		//instanceData->drawRegions[n].theDescriptor.colorDescriptor.redShift	 = ???;
		//instanceData->drawRegions[n].theDescriptor.colorDescriptor.redMask	 = ???;
		//instanceData->drawRegions[n].theDescriptor.colorDescriptor.greenShift	 = ???;
		//instanceData->drawRegions[n].theDescriptor.colorDescriptor.greenMask	 = ???;
		//instanceData->drawRegions[n].theDescriptor.colorDescriptor.blueShift	 = ???;
		//instanceData->drawRegions[n].theDescriptor.colorDescriptor.blueMask	 = ???;
		//instanceData->drawRegions[n].theDescriptor.colorDescriptor.alphaShift	 = ???;
		//instanceData->drawRegions[n].theDescriptor.colorDescriptor.alphaMask	 = ???;
		instanceData->drawRegions[n].theDescriptor.bitOrder	 = kQ3EndianBig;
		instanceData->drawRegions[n].theDescriptor.byteOrder = kQ3EndianBig;
		instanceData->drawRegions[n].theDescriptor.clipMask = NULL;

		// better implement the following line by calling LockPixels//Unlock in
		// E3XDrawRegion_StartAccessToImageBuffer and E3XDrawRegion_End
		//instanceData->drawRegions[n].imageBuffer = GetPixBaseAddr(thePixMap);


		// Create a clipping region
		instanceData->drawRegions[n].platformClip = (TQ3Uns32) NewRgn();
		}

	return(qd3dStatus);
}





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
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->instanceData;
	E3GlobalsPtr				theGlobals = E3Globals_Get();
	TQ3MacDrawContext2DLibrary	theLibrary;
	Rect						windowRect, paneRect;
	TQ3XDrawContextValidation	stateChanges;
	CWindowPtr					theWindow;
	RgnHandle					visRgn;				// visible region of window



	// If a Display Manager notification has occurred, reset everything
	if (theGlobals->dmNotifiedChanges)
		{
		theGlobals->dmNotifiedChanges = kQ3False;
		return(kQ3XDrawContextValidationAll);
		}



	// Otherwise, start off with nothing
	stateChanges = kQ3XDrawContextValidationClearFlags;



	// Work out what type of draw context we're dealing with, watching out
	// for people who specified QuickDraw when they really wanted a window.
	theLibrary = instanceData->data.macData.theData.library;
	if (theLibrary == kQ3Mac2DLibraryQuickDraw            &&
		instanceData->data.macData.theData.window != NULL &&
		instanceData->data.macData.theData.window == (CWindowPtr)instanceData->data.macData.theData.grafPort)
		theLibrary = kQ3Mac2DLibraryNone;



	// Handle each draw context type in turn
	switch (theLibrary) {
		case kQ3Mac2DLibraryNone:
			// Get the window, and work out its rectangles.
			//
			// If a pane has been specified, we adjust the window rect
			// accordingly (i.e., as if the window was the size of the pane).
			theWindow  = (CWindowPtr) instanceData->data.macData.theData.window;
			GetWindowContentRect( theWindow, &windowRect );

			if (instanceData->data.common.paneState)
				{
				E3Area_ToRect(&instanceData->data.common.pane, &paneRect);
				OffsetRect(&paneRect, windowRect.left, windowRect.top);
				windowRect = paneRect;
				}



			// Check to see if the window has changed position. If it has, we set the
			// kQ3XDrawContextValidationDevice bit as well in case it's been moved to
			// a different device.
			if (windowRect.top  != instanceData->data.macData.windowRect.top ||
				windowRect.left != instanceData->data.macData.windowRect.left)
				{
				stateChanges |= kQ3XDrawContextValidationWindowPosition;
				stateChanges |= kQ3XDrawContextValidationDevice;
				}



			// Check to see if the window has changed size. If it has, we set the
			// kQ3XDrawContextValidationDevice bit as well in case it's been grown
			// to a different device.
			if (E3Rect_GetWidth(&windowRect)  != E3Rect_GetWidth(&instanceData->data.macData.windowRect) ||
				E3Rect_GetHeight(&windowRect) != E3Rect_GetHeight(&instanceData->data.macData.windowRect))
				{
				stateChanges |= kQ3XDrawContextValidationWindowSize;
				stateChanges |= kQ3XDrawContextValidationDevice;
				}



			// Check to see if the window clipping has changed
			visRgn = NewRgn();
			if (visRgn != NULL)
				{
				GetPortVisibleRegion( GetWindowPort(theWindow), visRgn );
				if (!EqualRgn(visRgn, instanceData->data.macData.visRgn))
					stateChanges |= kQ3XDrawContextValidationWindowClip;
				}



			// Save the winow details for next time
			instanceData->data.macData.windowRect = windowRect;
			if (visRgn != NULL)
				CopyRgn(visRgn, instanceData->data.macData.visRgn);
			break;

		
		
		case kQ3Mac2DLibraryQuickDraw:
		case kQ3Mac2DLibraryQuickDrawGX:
			// Not supported
			break;
		}



	// Return the state changes
	return(stateChanges);
}





//=============================================================================
//      e3drawcontext_mac_updateregions : Update the draw regions.
//-----------------------------------------------------------------------------
//		Note :	We update the rendering specific field of the draw regions to
//				reflect the current state of the window.
//
//				The characteristics which have changed are indicated by
//				stateChanges.
//
//				This is called once per frame, so must have minimal overhead.
//-----------------------------------------------------------------------------
static void
e3drawcontext_mac_updateregions(TQ3DrawContextObject theDrawContext, TQ3XDrawContextValidation stateChanges)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->instanceData;
	TQ3Area						deviceArea, windowArea;
	TQ3XClipMaskState			clipMaskState;
	TQ3Boolean					isActive;
	TQ3Uns32					n;
	


	// Update the draw regions
	for (n = 0; n < instanceData->numDrawRegions; n++)
		{
		// If this draw region intersects the window, update it
		isActive = e3drawcontext_mac_isactiveregion(theDrawContext,
													n,
													&deviceArea,
													&windowArea,
													&clipMaskState,
													(RgnHandle) instanceData->drawRegions[n].platformClip);
		if (isActive)
			{
			// If this draw region has just become active, update everything. We need to do this
			// since the device/window fields may still be in their initial 0'd state - e.g., the
			// window may not have changed size, but dragging it on to a previously inactive
			// draw region will require that those fields have the correct values.
			if (!instanceData->drawRegions[n].isActive)
				stateChanges |= kQ3XDrawContextValidationWindowPosition |
								kQ3XDrawContextValidationWindowSize     |
								kQ3XDrawContextValidationWindowClip;



			// Update the draw region for position changes
			if (stateChanges & kQ3XDrawContextValidationWindowPosition)
				{
				instanceData->drawRegions[n].deviceOffsetX = deviceArea.min.x;
				instanceData->drawRegions[n].deviceOffsetY = deviceArea.min.y;
				instanceData->drawRegions[n].windowOffsetX = windowArea.min.x;
				instanceData->drawRegions[n].windowOffsetY = windowArea.min.y;
				}



			// Update the draw region for size changes
			if (stateChanges & kQ3XDrawContextValidationWindowSize)
				{
				instanceData->drawRegions[n].deviceScaleX = (deviceArea.max.x - deviceArea.min.x);
				instanceData->drawRegions[n].deviceScaleY = (deviceArea.max.y - deviceArea.min.y);
				instanceData->drawRegions[n].windowScaleX = (windowArea.max.x - windowArea.min.x);
				instanceData->drawRegions[n].windowScaleY = (windowArea.max.y - windowArea.min.y);
				}



			// Adjust the draw region for visibility changes
			if (stateChanges & kQ3XDrawContextValidationWindowClip)
				{
				// Update the clipping mask state
				instanceData->drawRegions[n].clipMaskState = clipMaskState;



				// Clipping masks aren't currently supported. It's a little unclear what
				// the behaviour for draw context clipping masks should be at the draw
				// region level: presumably the mask would need to be split up for each
				// draw region, with the mask for each draw region being the intersection
				// between the draw context mask and the draw region active area.
				//
				// Would need to compare against QD3D to see what the behaviour there
				// is, so for now these masks aren't supported.
				instanceData->drawRegions[n].theDescriptor.clipMask = NULL;
				}
			}



		// Adjust the draw region state
		instanceData->drawRegions[n].isActive = isActive;
		}
}





//=============================================================================
//      e3drawcontext_mac_new : Mac draw context new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_mac_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DrawContextUnionData			*instanceData = (TQ3DrawContextUnionData *) privateData;
	const TQ3MacDrawContextData		*macData      = (const TQ3MacDrawContextData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->data.macData.theData = *macData;
	instanceData->data.macData.visRgn  = NewRgn();



	// Handle failure
	if (instanceData->data.macData.visRgn == NULL)
		return(kQ3Failure);
	
	return(kQ3Success);
}





//=============================================================================
//      e3drawcontext_mac_delete : Mac draw context delete method.
//-----------------------------------------------------------------------------
static void
e3drawcontext_mac_delete(TQ3Object theObject, void *privateData)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) privateData;
	TQ3Status					qd3dStatus;
#pragma unused(privateData)



	// Dispose of the common instance data
	qd3dStatus = e3drawcontext_mac_createregions(theObject, 0);

	if (instanceData->data.common.maskState)
		qd3dStatus = Q3Bitmap_Empty(&instanceData->data.common.mask);



	// Dispose of the Mac specific instance data
	if (instanceData->data.macData.visRgn != NULL)
		DisposeRgn(instanceData->data.macData.visRgn);
}





//=============================================================================
//      e3drawcontext_mac_update : Mac draw context update method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_mac_update(TQ3DrawContextObject theDrawContext)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->instanceData;
	TQ3XDrawContextValidation	stateChanges;
	TQ3Status					qd3dStatus;



	// If we don't have any draw regions, or everything has changed, rebuild them
	if (instanceData->numDrawRegions == 0 || instanceData->theState == kQ3XDrawContextValidationAll)
		{
		// Rebuild the draw regions
		qd3dStatus = e3drawcontext_mac_buildregions(theDrawContext);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);


		// Update the regions and the state flag
		e3drawcontext_mac_updateregions(theDrawContext, kQ3XDrawContextValidationAll);
		instanceData->theState = kQ3XDrawContextValidationAll;
		}



	// Otherwise, check to see if they need to be updated
	else
		{
		// Test to see if the draw regions need updating
		qd3dStatus   = kQ3Success;
		stateChanges = e3drawcontext_mac_checkregions(theDrawContext);
		if (stateChanges != kQ3XDrawContextValidationClearFlags)
			{
			// If something changed, update the state flag and the draw regions
			instanceData->theState |= stateChanges;
			e3drawcontext_mac_updateregions(theDrawContext, instanceData->theState);
			}
		}

	// clear the DrawContext
	if(instanceData->data.common.clearImageMethod == kQ3ClearMethodWithColor){
		NULL;
		}
		
	
	return(qd3dStatus);		
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
		}
	
	return(theMethod);
}





//=============================================================================
//      e3drawcontext_mac_dm_notify : Display Manager callback.
//-----------------------------------------------------------------------------
static pascal void
e3drawcontext_mac_dm_notify(AppleEvent *theEvent)
{	E3GlobalsPtr			theGlobals = E3Globals_Get();
#pragma unused(theEvent)



	// Set our flag
	Q3_ASSERT_VALID_PTR(theGlobals);
	theGlobals->dmNotifiedChanges = kQ3True;
}





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
	ProcessSerialNumber		thePSN;



	// Register our Display Manager notification callback
	theGlobals->dmNotifyUPP = NewDMNotificationProc(e3drawcontext_mac_dm_notify);
	if (theGlobals->dmNotifyUPP != NULL)
		{
		GetCurrentProcess(&thePSN);
		DMRegisterNotifyProc(theGlobals->dmNotifyUPP, &thePSN);
		}



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeDrawContext,
											kQ3DrawContextTypeMacintosh,
											kQ3ClassNameDrawContextMac,
											e3drawcontext_mac_metahandler,
											sizeof(TQ3DrawContextUnionData));

	return(qd3dStatus);
}





//=============================================================================
//      E3MacDrawContext_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_UnregisterClass(void)
{	E3GlobalsPtr			theGlobals = E3Globals_Get();
	TQ3Status				qd3dStatus;
	ProcessSerialNumber		thePSN;
	
	
// let's compile with Universal Interfaces 3.2
#ifndef DisposeDMNotificationUPP
#define DisposeDMNotificationUPP DisposeRoutineDescriptor
#endif


	// Unregister our Display Manager notification callback
	if (theGlobals->dmNotifyUPP != NULL)
		{
		GetCurrentProcess(&thePSN);
		DMRemoveNotifyProc(theGlobals->dmNotifyUPP, &thePSN);

		DisposeDMNotificationUPP(theGlobals->dmNotifyUPP);
		theGlobals->dmNotifyUPP = NULL;
		}



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3DrawContextTypeMacintosh, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3MacDrawContext_New : Create a new Mac draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3MacDrawContext_New(const TQ3MacDrawContextData *drawContextData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3DrawContextTypeMacintosh, kQ3False, drawContextData);

	return(theObject);
}





//=============================================================================
//      E3MacDrawContext_SetWindow : Set the window.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_SetWindow(TQ3DrawContextObject drawContext, CWindowPtr window)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the window and reset our flag
	instanceData->data.macData.theData.window = window;
	instanceData->theState                   |= kQ3XDrawContextValidationAll;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_GetWindow : Return the window.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_GetWindow(TQ3DrawContextObject drawContext, CWindowPtr *window)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the window
	*window = instanceData->data.macData.theData.window;
	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_SetGXViewPort : Set the GX viewport.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_SetGXViewPort(TQ3DrawContextObject drawContext, gxViewPort viewPort)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the GX viewport and reset our flag
	instanceData->data.macData.theData.viewPort = viewPort;
	instanceData->theState                     |= kQ3XDrawContextValidationAll;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_GetGXViewPort : Return the GX viewport.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_GetGXViewPort(TQ3DrawContextObject drawContext, gxViewPort *viewPort)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the GX viewport
	*viewPort = instanceData->data.macData.theData.viewPort;
	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_SetGrafPort : Set the GrafPort.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_SetGrafPort(TQ3DrawContextObject drawContext, CGrafPtr grafPort)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the GrafPort and reset the flag
	instanceData->data.macData.theData.grafPort = grafPort;
	instanceData->theState                     |= kQ3XDrawContextValidationAll;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_GetGrafPort : Return the GrafPort.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_GetGrafPort(TQ3DrawContextObject drawContext, CGrafPtr *grafPort)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the GrafPort
	*grafPort = instanceData->data.macData.theData.grafPort;
	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_Set2DLibrary : Set the 2D library.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_Set2DLibrary(TQ3DrawContextObject drawContext, TQ3MacDrawContext2DLibrary library)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the 2D library and reset our flag
	instanceData->data.macData.theData.library = library;
	instanceData->theState                    |= kQ3XDrawContextValidationAll;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3MacDrawContext_Get2DLibrary : Return the 2D library.
//-----------------------------------------------------------------------------
TQ3Status
E3MacDrawContext_Get2DLibrary(TQ3DrawContextObject drawContext, TQ3MacDrawContext2DLibrary *library)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the 2D library
	*library = instanceData->data.macData.theData.library;
	return(kQ3Success);
}





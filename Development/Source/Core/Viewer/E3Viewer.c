/*  NAME:
        E3Viewer.c

    DESCRIPTION:
        Implementation the Quesa viewer library. Search for '???' for things
        to do.

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

#include "E3Viewer.h"
#include "E3ViewerTools.h"
#if QUESA_OS_MACINTOSH
	#include "E3CarbonCoating.h"
#endif

//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kQ3MajorRevision	1L
#define kQ3MinorRevision	6L
#define kQ3FixRevision		0L

#define kQ3ValidViewer		0xFEEDD0D0
#define kQ3InvalidViewer	0xDEADD0D0

#define kQ3ControllerWidth	 0L
#define kQ3ControllerHeight	30L
#define kQ3MinimumWidth		20L
#define kQ3MinimumHeight	20L
#define kQ3ButtonHeight		24L
#define kQ3ButtonWidth		24L
#define kQ3ButtonGap		 4L
#define kQ3BottomOffset		 2L
#define kQ3DragBorderWidth	 4L
#define kQ3ButtonSpacing	kQ3ButtonWidth + kQ3ButtonGap

#define kHandCursor			-128
#define kClosedHandCursor	-129

#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
const TQ3Uns32 kQ3ViewerCanDrawDragBorders	=		kQ3ViewerDragMode			|
													kQ3ViewerDrawDragBorder		;

const TQ3Uns32 kQ3ViewerInternalDefault 	=		kQ3ViewerActive				|
													kQ3ViewerControllerVisible	|
													kQ3ViewerDrawFrame			|
													kQ3ViewerButtonCamera		|
													kQ3ViewerButtonTruck		|
													kQ3ViewerButtonOrbit		|
													kQ3ViewerButtonZoom			|
													kQ3ViewerButtonDolly		|
													kQ3ViewerButtonReset		|
													kQ3ViewerDragMode			|
													kQ3ViewerDrawDragBorder 	|
													kQ3ViewerButtonOptions		;
#else
const TQ3Uns32 kQ3ViewerCanDrawDragBorders	=		0;

const TQ3Uns32 kQ3ViewerInternalDefault 	=		kQ3ViewerActive				|
													kQ3ViewerControllerVisible	|
													kQ3ViewerButtonCamera		|
													kQ3ViewerButtonTruck		|
													kQ3ViewerButtonOrbit		|
													kQ3ViewerButtonZoom			|
													kQ3ViewerButtonDolly		|
													kQ3ViewerButtonReset		|
													kQ3ViewerButtonOptions		;
#endif

#if __REALQD3D__
	#ifndef Q3_ASSERT
		#define Q3_ASSERT(x)
	#endif
	TQ3ViewMode gViewMode = kQ3ViewModeInactive;
	
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	#define kAppHeapThreshold                   (50 * 1024)
	
	static void* E3Memory_Allocate (TQ3Uns32 theSize)
		{
		TQ3Uns32    totalMem, memAvail, requiredSize;
	    Handle      theHnd, *hndPtr;
	    OSErr       theErr;



	    // Check our parameters
	    if (theSize == 0)
	        return(NULL);



	    // Work out how much memory we need.
	    requiredSize = theSize + sizeof(Handle);



	    // Try and allocate some memory from temp mem first.
	    // If that fails, we compact the app heap and try there.
	    theHnd = TempNewHandle(requiredSize, &theErr);
	    if (theHnd == NULL)
	        {
	        PurgeSpace((SInt32 *) &totalMem, (SInt32 *) &memAvail);
	        if (requiredSize + kAppHeapThreshold <= memAvail)
	            {
	            PurgeMem(requiredSize);       
	            theHnd = NewHandle(requiredSize);
	            }
	        }



	    // If we didn't get the handle, we're done.
	    if (theHnd == NULL)
	        return(NULL);



	    // Write the handle into the first four bytes, and return the
	    // rest of the block as the requested memory.
	    MoveHHi(theHnd);
	    HLock(theHnd);
	    hndPtr    = (Handle *) *theHnd;
	    *hndPtr++ = theHnd;

	    return((void *) hndPtr);
		}


	static void E3Memory_Free (void** thePtr)
		{   
		Handle      theHnd, *hndPtr;



	    // Check our parameters
	    if (thePtr == NULL)
	        return;
	    if (*thePtr == NULL)
	        return;



	    // Recover the handle from the pointer and free the block.
	    hndPtr = (Handle *) *thePtr;
	    theHnd = *(hndPtr - 1);
	    
	    DisposeHandle(theHnd);
	    *thePtr = NULL;
		}

#endif
#endif





//=============================================================================
//      Internal #defines
//-----------------------------------------------------------------------------

#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	#define TQ3Rect Rect
	#define ConstTQ3Rect Rect
	#define TQ3Port CGrafPtr
	#define TQ3Result OSErr
	#define TQ3EventRecord EventRecord
	#define TQ3BoolResult Boolean
	#define kQ3GoodResult noErr
	#define kQ3BadResult paramErr
#else
	#if defined(QUESA_OS_WIN32) && QUESA_OS_WIN32)
		#define TQ3Rect RECT
		#define ConstTQ3Rect const RECT
		#define TQ3Port HWND
		#define TQ3BoolResult BOOL
	#else
		#define TQ3Rect TQ3Area
		#define ConstTQ3Rect const TQ3Area
		#define TQ3Port void*
		#define TQ3BoolResult TQ3Boolean
	#endif
	#define TQ3EventRecord void
	#define TQ3Result TQ3Status
	#define kQ3GoodResult kQ3Success
	#define kQ3BadResult kQ3Failure
#endif

#define CheckViewerFalse(_viewer)	if (!_viewer || ((TQ3ViewerData*)(_viewer))->validViewer != kQ3ValidViewer) return 0
#define CheckViewerNULL(_viewer)	if (!_viewer || ((TQ3ViewerData*)(_viewer))->validViewer != kQ3ValidViewer) return NULL
#define CheckViewerFailure(_viewer)	if (!_viewer || ((TQ3ViewerData*)(_viewer))->validViewer != kQ3ValidViewer) return kQ3BadResult





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
#pragma mark --- Types ---

enum TModalButtons
	{
	eFirstButton,
	eCameraButton = 0,
	eTruckButton,
	eOrbitButton,
	eZoomButton,
	eDollyButton,
	eResetButton,
	eOptionsButton,
	eAboutBoxButton,
	ePluginsButton,
	eLastButton
	} TModalButtons;


typedef struct TQ3ViewerData
	{
	TQ3Uns32								validViewer;
	TQ3Uns32								flags;
	TQ3Uns32								currentButton;
	TQ3ViewObject							theView;
	TQ3GroupObject							theObjects;
	TQ3GroupObject							theViewHints;
	TQ3GroupObject							otherObjects;
	TQ3StyleObject							backfaceObject;
	TQ3ShaderObject							phongObject;
	TQ3XMetaHandler							metaHandlers [eLastButton];
	TQ3ViewerDrawingCallbackMethod			callbackMethod;
	void* 									callbackData;
	TQ3ViewerWindowResizeCallbackMethod		windowResizeCallbackMethod;
	void* 									windowResizeCallbackData;
	TQ3ViewerPaneResizeNotifyCallbackMethod	paneResizeNotifyCallbackMethod;
	void*									paneResizeNotifyCallbackData;
	TQ3Rect									theRect;
	TQ3Rect									drawRect;
	TQ3Port								thePort;
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	ControlHandle							statusBar;
	DragReference							theDrag;
	DragReceiveHandlerUPP					receiveHandler;
	DragTrackingHandlerUPP					trackingHandler;
#elif defined(QUESA_OS_WIN32) && QUESA_OS_WIN32
	TQ3Port								the3DMFWindow;
	TQ3Port								theControllerWindow;
#else
	// other platforms
#endif
	} TQ3ViewerData;


static char gContinueTracking = 0;
static TQ3EventRecord* gEventPtr = NULL;
static TQ3ViewerData* gDragViewer = NULL;
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	static AliasHandle gAliasHandle = NULL;
	static TQ3Int16 gResFileRefNum = 0;
	static TQ3Uns32 gResFileCount = 0;
#endif





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3_viewer_GetPortWindow :	Convert a TQ3Port into the associated Window.
//									MacOS only.
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
static
WindowRef e3_viewer_GetPortWindow(TQ3Port port)
{
	#if TARGET_API_MAC_CARBON
		// Under Carbon, use a function:
		return GetWindowFromPort(port);
	#else
		// under Classic MacOS, you can just typecast:
		return (WindowRef)port;
	#endif
}
#endif


//-----------------------------------------------------------------------------
//      e3_viewer_button_to_external :	Convert an internal button index to the
//										external index.
//-----------------------------------------------------------------------------
//		Note :	We currently use a hard-coded array of buttons, so we need this
//				translation routine to go between indices into this array and
//				the 'array' of buttons that the app thinks exist (i.e., the
//				buttons it turned on via its flags).
//-----------------------------------------------------------------------------
static TQ3Uns32
e3_viewer_button_to_external(TQ3ViewerObject theViewer, TQ3Uns32 theButton)
{	TQ3ViewerData		*viewerData = (TQ3ViewerData *) theViewer;
	TQ3Uns32			n, m;



	// Look up the button
	n = 0;
	m = 0;
	
	if (theButton == n)
		return(m);
	if (viewerData->flags & kQ3ViewerButtonCamera)
		m++;
	n++;

	if (theButton == n)
		return(m);
	if (viewerData->flags & kQ3ViewerButtonTruck)
		m++;
	n++;

	if (theButton == n)
		return(m);
	if (viewerData->flags & kQ3ViewerButtonOrbit)
		m++;
	n++;

	if (theButton == n)
		return(m);
	if (viewerData->flags & kQ3ViewerButtonZoom)
		m++;
	n++;

	if (theButton == n)
		return(m);
	if (viewerData->flags & kQ3ViewerButtonDolly)
		m++;
	n++;

	if (theButton == n)
		return(m);
	if (viewerData->flags & kQ3ViewerButtonReset)
		m++;
	n++;

	if (theButton == n)
		return(m);
	if (viewerData->flags & kQ3ViewerButtonOptions)
		m++;
	n++;

	Q3_ASSERT(theButton == n);
	return(theButton);
}





static TQ3Status
e3callallplugins (TQ3ViewerData* theViewer, TQ3Int32 handler)
	{
	void* plugin;
	TQ3Status result;
	TQ3CameraObject theCamera = NULL;
	TQ3RendererObject theRenderer = NULL;
	TQ3GroupObject theLights = NULL;
	TQ3DrawContextObject theContext = NULL;
	
	switch (handler)
		{
		case kQ3XMethodType_PluginBeforeDraw:
		case kQ3XMethodType_PluginAfterDraw:
		case kQ3XMethodType_PluginAfterStartDraw:
		case kQ3XMethodType_PluginAfterStartWrite:
		case kQ3XMethodType_PluginAfterStartBounds:
		case kQ3XMethodType_PluginAfterStartPick:
		case kQ3XMethodType_ViewerPluginNewViewer:
		case kQ3XMethodType_ViewerPluginDeleteViewer:
		case kQ3XMethodType_ViewerPluginActivate:
		case kQ3XMethodType_ViewerPluginDeactivate:
		case kQ3XMethodType_ViewerPluginGroupChanged:
		case kQ3XMethodType_ViewerPluginAddDragFlavors:
			result = kQ3Success;
			break;
			
		case kQ3XMethodType_ViewerPluginCameraChanged:
			result = Q3View_GetCamera (theViewer->theView, &theCamera);
			break;

		case kQ3XMethodType_ViewerPluginRendererChanged:
			result = Q3View_GetRenderer (theViewer->theView, &theRenderer);
			break;

		case kQ3XMethodType_ViewerPluginLightsChanged:
			result = Q3View_GetLightGroup (theViewer->theView, &theLights);
			break;

		case kQ3XMethodType_ViewerPluginDrawContextChanged:
			result = Q3View_GetDrawContext (theViewer->theView, &theContext);
			break;
		
		default:
			Q3_ASSERT (0);
			return kQ3Success; // not a suitable method
		}
	
	if (result == kQ3Failure)
		return kQ3Success; // could not get an object but do not stop anything else from functioning
	
	plugin = NULL;
	while (plugin) // for each plugin that registers a message
		{
		TQ3XFunctionPointer method = NULL;
		
		if (method)
			{
			switch (handler)
				{
				case kQ3XMethodType_ViewerPluginGroupChanged:
					result = ((TQ3XViewerPluginGroupChangedMethod)(method)) (theViewer, plugin, theViewer->theObjects);
					break;

				case kQ3XMethodType_ViewerPluginCameraChanged:
					result = ((TQ3XViewerPluginCameraChangedMethod)(method)) (theViewer, plugin, theCamera);
					break;

				case kQ3XMethodType_ViewerPluginRendererChanged:
					result = ((TQ3XViewerPluginRendererChangedMethod)(method)) (theViewer, plugin, theRenderer);
					break;

				case kQ3XMethodType_ViewerPluginLightsChanged:
					result = ((TQ3XViewerPluginLightsChangedMethod)(method)) (theViewer, plugin, theLights);
					break;

				case kQ3XMethodType_ViewerPluginDrawContextChanged:
					result = ((TQ3XViewerPluginDrawContextChangedMethod)(method)) (theViewer, plugin, theContext);
					break;
				
				case kQ3XMethodType_ViewerPluginDeactivate:
				case kQ3XMethodType_ViewerPluginActivate:
				case kQ3XMethodType_ViewerPluginNewViewer:
				case kQ3XMethodType_ViewerPluginDeleteViewer:
					result = ((TQ3XPluginViewerParamMethod)(method)) (theViewer, plugin);
					break;

				case kQ3XMethodType_PluginAfterStartWrite:
				case kQ3XMethodType_PluginAfterStartBounds:
				case kQ3XMethodType_PluginAfterStartPick:
				case kQ3XMethodType_PluginBeforeDraw:
				case kQ3XMethodType_PluginAfterDraw:
				case kQ3XMethodType_PluginAfterStartDraw:
					result = ((TQ3XPluginViewParamMethod)(method)) (theViewer->theView);
					break;
				
				case kQ3XMethodType_ViewerPluginAddDragFlavors:
					{
					TQ3Uns32 itemRef = 0;
					result = ((TQ3XViewerPluginAddDragFlavorsMethod)(method)) (theViewer->theView, plugin, theViewer->theDrag, itemRef);
					break;
					}
				}
			}
		}
	if (theCamera)
		Q3Object_Dispose (theCamera);
	if (theRenderer)
		Q3Object_Dispose (theRenderer);
	if (theLights)
		Q3Object_Dispose (theLights);
	if (theContext)
		Q3Object_Dispose (theContext);
	return result;
	}


static TQ3ViewStatus
e3callallviewstatusplugins (TQ3ViewerData* theViewer, TQ3Int32 handler)
	{
	void* plugin;
	TQ3ViewStatus result = kQ3ViewStatusDone;
	
	switch (handler)
		{
		case kQ3XMethodType_PluginBeforeEndWrite:
		case kQ3XMethodType_PluginBeforeEndBounds:
		case kQ3XMethodType_PluginBeforeEndDraw:
		case kQ3XMethodType_PluginBeforeEndPick:
			break;

		default:
			Q3_ASSERT (0);
			return kQ3ViewStatusDone; // not a suitable method
		}

	plugin = NULL;
	while (plugin) // for each plugin that registers a message
		{
		TQ3XFunctionPointer method = NULL;
		
		if (method)
			result = ((TQ3XPluginViewStatusMethod)(method)) (theViewer->theView);
		}
	return result;
	}


static TQ3Status
e3callallpluginswithparams (TQ3ViewerData* theViewer, TQ3Int32 handler, void* evt, TQ3Uns32 param1, TQ3Uns32 param2)
	{
	void* plugin;
	TQ3Status result = kQ3Success;
	
	switch (handler)
		{
		case kQ3XMethodType_PluginDuringSubmit:
		case kQ3XMethodType_ViewerPluginBeforeEvent:
		case kQ3XMethodType_ViewerPluginAfterEvent:
		case kQ3XMethodType_ViewerPluginDragAcceptable:
		case kQ3XMethodType_ViewerPluginDoDrop:
			break;

		default:
			Q3_ASSERT (0);
			return kQ3Success; // not a suitable method
		}
	
	plugin = NULL;
	while (plugin) // for each plugin that registers a message
		{
		TQ3XFunctionPointer method = NULL;
		
		if (method)
			{
			switch (handler)
				{
				case kQ3XMethodType_PluginDuringSubmit:
					result = ((TQ3XPluginDuringSubmitMethod)(method)) (theViewer->theView, plugin, param1, param2, (TQ3ViewMode)(evt));
					break;

				case kQ3XMethodType_ViewerPluginBeforeEvent:
					result = ((TQ3XViewerPluginBeforeEventMethod)(method)) (theViewer->theView, plugin, evt, param1, param2);
					break;

				case kQ3XMethodType_ViewerPluginAfterEvent:
					result = ((TQ3XViewerPluginAfterEventMethod)(method)) (theViewer->theView, plugin, evt, param1, param2);
					break;
				
				case kQ3XMethodType_ViewerPluginDragAcceptable:
					{
					if (((TQ3XViewerPluginDragAcceptableMethod)(method)) (theViewer, plugin, (DragReference)(param1), param2, evt) == kQ3True)
						result = kQ3Success;
					else
						result = kQ3Failure;
					break;
					}
				
				case kQ3XMethodType_ViewerPluginDoDrop:
					result = ((TQ3XViewerPluginDoDropMethod)(method)) (theViewer, plugin, (DragReference)(param1), param2, *(TQ3Int32*)(evt));
					break;
				}
			}
		}
	return result;
	}


static TQ3Status
e3checkcancel (void)
	{
	static UInt32 sNextCheck = 0 ;
	UInt32 ticks;
	return kQ3Success; // ???
	
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	ticks = TickCount ();
#else
	ticks = 0;
#endif
	if (ticks >= sNextCheck)
		{
	#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
		#define escKeyCode 		0x35 // See IM V-191.
		#define commandKeyCode	0x37
		#define periodKeyCode 	0x2F
		
		KeyMap theKeys;
		sNextCheck = ticks + 20; // check every 1/3 of a second
	
		GetKeys (theKeys);
		if (((theKeys [escKeyCode >> 3] >> (escKeyCode & 7)) & 1) != 0)
			return kQ3Failure;

		if ((((theKeys [commandKeyCode >> 3] >> (commandKeyCode & 7)) & 1) != 0) &&
			((((theKeys [periodKeyCode >> 3] >> (periodKeyCode & 7)) & 1)) != 0))
			return kQ3Failure;
	
	#endif
		}
	return kQ3Success;
	}


#pragma mark --- end unimplemented ---
#pragma mark -


static TQ3Status
e3callindexplugin (TQ3ViewerData* theViewer, TQ3Uns32 index, TQ3Int32 handler, void* param)
	{
	TQ3XMetaHandler metaHandler;
	if ((index >= eLastButton) || (index < 0))
		return kQ3Failure;
	metaHandler = theViewer->metaHandlers [index];
	if (metaHandler)
		{
		TQ3XFunctionPointer method = metaHandler (handler);
		if (method)
			{
			TQ3Status result = kQ3Success;
		#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
			TQ3Int16 oldResFile = CurResFile ();
			UseResFile (gResFileRefNum);
		#endif
			switch (handler)
				{
				case kQ3XMethodType_ViewerPluginUnclickTool: // param is NULL
				case kQ3XMethodType_ViewerPluginClickTool:
				case kQ3XMethodType_ViewerPluginDoubleClickTool:
					result = ((TQ3XPluginViewerParamMethod)(method)) (theViewer, NULL);
					break;
					
				case kQ3XMethodType_ViewerPluginDoToolEvent: // param is EventRecord*
				case kQ3XMethodType_ViewerPluginDoKeyDown:
					{
					TQ3EventRecord* evt = (TQ3EventRecord*)param;
					result = ((TQ3XPluginViewerParamEventMethod)(method)) (theViewer, NULL, evt, 0, 0);
					break;
					}
				case kQ3XMethodType_ViewerPluginDoToolStart: // param is TQ3Point2D
				case kQ3XMethodType_ViewerPluginDoToolTracking:
				case kQ3XMethodType_ViewerPluginDoToolEnd:
				case kQ3XMethodType_ViewerPluginCursorAdjust:
					{
					TQ3Point2D* pt = (TQ3Point2D*)param;
					result = ((TQ3XPluginViewerParamPointMethod)(method)) (theViewer, NULL, pt);
					break;
					}
				
				case kQ3XMethodType_ViewerPluginGetHelpString:
					{
					result = ((TQ3XViewerPluginGetHelpStringMethod)(method)) (theViewer, NULL, param);
					break;
					}
				
				default:
					result = kQ3Failure;
					break;
				}
		#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
			UseResFile (oldResFile);
		#endif
			return result;
			}
		}
	return kQ3Failure;
	}


static TQ3Status
e3idlemethod (TQ3ViewObject theView, const void* viewerData) // only called for interactive renderers
	{
	TQ3ViewMode mode;
#if __REALQD3D__
#pragma unused (theView)
	mode = gViewMode;
#else
	mode = E3View_GetViewMode (theView);
#endif
	if (e3callallpluginswithparams ((TQ3ViewerData*)(viewerData), kQ3XMethodType_PluginDuringSubmit, (void*)(mode), 0, 0) == kQ3Success)
		return e3checkcancel (); // just check whether the user has canceled rendering
	return kQ3Failure;
	}


static TQ3Status
e3idleprogressmethod ( // only called for non-interactive renderers
	TQ3ViewObject		theView,
	const void*			data,
	TQ3Uns32			current,
	TQ3Uns32			completed)
	{
	static UInt32 sNextFlush = 0;
	TQ3ViewMode mode;
	UInt32 ticks;
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	ticks = TickCount ();
#else
	ticks = 0;
#endif
	if (ticks >= sNextFlush)
		{
	#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
		sNextFlush = ticks + 60; // every second
	#endif
		Q3View_Flush (theView);
		}
	if (data)
		{
		TQ3ViewerData* viewerData = (TQ3ViewerData*)(data);
		TQ3Uns32 percentage = (float)(current) / (float)(completed) * 100.0f;
	#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
		if (viewerData->statusBar)
			SetControlValue (viewerData->statusBar, percentage);
	#endif
		}
#if __REALQD3D__
	mode = gViewMode;
#else
	mode = E3View_GetViewMode (theView);
#endif
	if (e3callallpluginswithparams ((TQ3ViewerData*)(data), kQ3XMethodType_PluginDuringSubmit, (void*)(mode) , current , completed) == kQ3Success)
		return e3checkcancel ();
	return kQ3Failure;
	}


static void
e3setdefaultdrawcontext (TQ3Rect* rect, TQ3DrawContextData* drawContextData)
	{
	drawContextData->clearImageMethod = kQ3ClearMethodWithColor;
	drawContextData->clearImageColor.a = 0.0f;
	drawContextData->clearImageColor.r = 1.0f;
	drawContextData->clearImageColor.g = 1.0f;
	drawContextData->clearImageColor.b = 1.0f;
	drawContextData->pane.min.x = rect->left;
	drawContextData->pane.min.y = rect->top;
	drawContextData->pane.max.x = rect->right;
	drawContextData->pane.max.y = rect->bottom;
	drawContextData->paneState = kQ3True;
	drawContextData->maskState = kQ3False;
	drawContextData->doubleBufferState = kQ3True;
	}


static TQ3Status
e3submitloop (TQ3ViewerData* viewerData)
	{
	TQ3Status status;
	if (viewerData->backfaceObject)
		status = Q3Style_Submit (viewerData->backfaceObject, viewerData->theView);
	else
		status = kQ3Success;
	
	if (viewerData->phongObject && (status == kQ3Success))
		status = Q3Shader_Submit (viewerData->phongObject, viewerData->theView);
	
	if (viewerData->theObjects && (status == kQ3Success))
		status = Q3DisplayGroup_Submit (viewerData->theObjects, viewerData->theView);
	
#if ! (defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH)
	if (viewerData->callbackMethod && (status == kQ3Success))
		status = viewerData->callbackMethod (viewerData, viewerData->callbackData);
#endif
	return status;
	}


#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
static TQ3Status
e3submitloopwitherr (TQ3ViewerData* viewerData, OSErr* err)
	{
	TQ3Status status = e3submitloop (viewerData);
	if ((viewerData->callbackMethod) && (status == kQ3Success))
		{
		*err = viewerData->callbackMethod (viewerData, viewerData->callbackData);
		if (*err)
			status = kQ3Failure;
		}
	return status;
	}
#endif


static TQ3Status
e3writefile (TQ3ViewerData* viewerData, TQ3StorageObject store)
	{
	TQ3FileObject theFile = Q3File_New ();
	if (theFile)
		{
		TQ3Status status = Q3File_SetStorage (theFile, store);
		if (status == kQ3Success)
			{
			TQ3FileMode mode = kQ3FileModeNormal;
			if (viewerData->flags & kQ3ViewerOutputTextMode)
				mode |= kQ3FileModeText;
			status = Q3File_OpenWrite (theFile, mode);
			if (status == kQ3Success)
				{
			#if __REALQD3D__
				gViewMode = kQ3ViewModeWriting;
			#endif
				status = Q3View_StartWriting (viewerData->theView, theFile);
				if (status == kQ3Success)
					{
					TQ3ViewStatus writeErr;
					do
						{
						TQ3ViewHintsObject theViewHints;
						status = e3callallplugins (viewerData, kQ3XMethodType_PluginAfterStartWrite);
						// write out the main viewHints
						theViewHints = Q3ViewHints_New (viewerData->theView);
						if (theViewHints)
							{
							Q3Object_Submit (theViewHints, viewerData->theView);
							Q3Object_Dispose (theViewHints);
							}
						if (viewerData->theViewHints)
							{
							// write out all the stored view hints
							TQ3GroupPosition pos;
							TQ3Status locStatus = Q3Group_GetFirstPosition (viewerData->theViewHints, &pos);
							if ((locStatus == kQ3Success) && pos)
								{
								do
									{
									TQ3Object object;
									locStatus = Q3Group_GetPositionObject (viewerData->theViewHints, pos, &object);
									if ((locStatus == kQ3Success) && object)
										{
										Q3Object_Submit (object, viewerData->theView);
										Q3Object_Dispose (object);
										locStatus = Q3Group_GetNextPosition (viewerData->theViewHints , &pos);
										if (locStatus == kQ3Failure)
											break;
										}
									else
										break;
									}
								while (pos);
								}
							}
						if (status == kQ3Success)
							status = e3submitloop (viewerData);
						if ((viewerData->otherObjects) && (status == kQ3Success))
							status = Q3Object_Submit (viewerData->otherObjects, viewerData->theView);
						if (status == kQ3Success)
							writeErr = e3callallviewstatusplugins (viewerData, kQ3XMethodType_PluginBeforeEndWrite);
						else
							e3callallviewstatusplugins (viewerData, kQ3XMethodType_PluginBeforeEndWrite);
						writeErr = Q3View_EndWriting (viewerData->theView);
						if (status == kQ3Failure)
							break;
						}
					while (writeErr == kQ3ViewStatusRetraverse);
					if (writeErr == kQ3ViewStatusError)
						status = kQ3Failure;
					}
				Q3File_Close (theFile);
			#if __REALQD3D__
				gViewMode = kQ3ViewModeInactive;
			#endif
				}
			}
		Q3Object_Dispose (theFile);
		return status;
		}
	return kQ3Failure;
	}


static TQ3Status
e3readfile (TQ3ViewerData* viewerData, TQ3StorageObject store)
	{
	TQ3FileObject theFile = Q3File_New ();
	if (theFile)
		{
		TQ3Status status = Q3File_SetStorage (theFile, store);
		if (status == kQ3Success)
			{
			TQ3FileMode mode = kQ3FileModeNormal;
			status = Q3File_OpenRead (theFile, &mode);
			if (status == kQ3Success)
				{
				if (!viewerData->theObjects)
					{
					viewerData->theObjects = Q3DisplayGroup_New ();
					if (!viewerData->theObjects)
						goto bail;
					}
				else
					Q3Group_EmptyObjects (viewerData->theObjects);
				if (viewerData->otherObjects)
					Q3Group_EmptyObjects (viewerData->otherObjects);
				if (viewerData->theViewHints)
					Q3Group_EmptyObjects (viewerData->theViewHints);
				
				Q3Error_Get (NULL); // clears any pending errors
				while (Q3File_IsEndOfFile (theFile) == kQ3False)
					{
					TQ3Error readError;
					TQ3Object object = Q3File_ReadObject (theFile);
					readError = Q3Error_Get (NULL);
					if (readError)
						{
						if (Q3Error_IsFatalError (readError))
							{
							if (object)
								Q3Object_Dispose (object);
							status = kQ3Failure;
							break;
							}
						}
					if (object)
						{
						TQ3ObjectType theType = Q3Object_GetLeafType (object);
						if (theType == kQ3SharedTypeViewHints)
							{
							if (viewerData->theViewHints) // add this view hint to my list of cameras
								Q3Group_AddObject (viewerData->theViewHints, object);
							}
						else
						if (theType == 'push') // get these when the app writes in immediate mode.
							{
// ???
							// need to create a new group
							}
						else
						if (theType == 'pop ')
							{
// ???
							}
						else
						if (Q3Object_IsDrawable (object))
							Q3Group_AddObject (viewerData->theObjects, object); // no need to check viewerData->theObjects != NULL, done above
						else
						if (viewerData->otherObjects)
							Q3Group_AddObject (viewerData->otherObjects, object);
						Q3Object_Dispose (object);
						}
					else // no object
						{
						status = kQ3Failure;
						break;
						}
					}
			bail:
				Q3File_Close (theFile);
				}
			}
		Q3Object_Dispose (theFile);
		if (status == kQ3Failure)
			{
			// clear all the geometries out, or leave what was read in so far ??
			if (viewerData->otherObjects)
				Q3Group_EmptyObjects (viewerData->otherObjects);
			if (viewerData->otherObjects)
				Q3Group_EmptyObjects (viewerData->otherObjects);
			}
		else
			e3callallplugins (viewerData, kQ3XMethodType_ViewerPluginGroupChanged);
		return status;
		}
	return kQ3Failure;
	}


static TQ3Status
e3getgroupbounds (TQ3ViewerData* viewerData, TQ3BoundingBox* bounds)
	{
	TQ3Status status;
#if __REALQD3D__
	gViewMode = kQ3ViewModeCalcBounds;
#endif
	status = Q3View_StartBoundingBox (viewerData->theView, kQ3ComputeBoundsExact);
	if (status == kQ3Success)
		{
		TQ3ViewStatus boundsErr;
		do
			{
			status = e3callallplugins (viewerData, kQ3XMethodType_PluginAfterStartBounds);
			if (status == kQ3Success)
				status = e3submitloop (viewerData);
			if (status == kQ3Success)
				boundsErr = e3callallviewstatusplugins (viewerData, kQ3XMethodType_PluginBeforeEndBounds);
			else
				e3callallviewstatusplugins (viewerData, kQ3XMethodType_PluginBeforeEndBounds);
			boundsErr = Q3View_EndBoundingBox (viewerData->theView, bounds);
			if (status == kQ3Failure)
				break;
			}
		while (boundsErr == kQ3ViewStatusRetraverse);
		if (boundsErr == kQ3ViewStatusError)
			status = kQ3Failure;
		}
#if __REALQD3D__
	gViewMode = kQ3ViewModeInactive;
#endif
	return status;
	}


static TQ3Status
e3resetcamerabounds (TQ3ViewerData* viewerData)
	{
	TQ3CameraObject theCamera;
	TQ3Status err = Q3View_GetCamera (viewerData->theView , &theCamera);
	if ((err == kQ3Success) && theCamera)
		{
		TQ3ObjectType theType = Q3Object_GetLeafType (theCamera);
		if (theType == kQ3CameraTypeViewAngleAspect)
			err = Q3ViewAngleAspectCamera_SetAspectRatio (theCamera,
				(float)(viewerData->drawRect.right - viewerData->drawRect.left) / (float)(viewerData->drawRect.bottom - viewerData->drawRect.top));
		else
		if (theType == kQ3CameraTypeOrthographic)
			{
// ???
			//Q3OrthographicCamera_SetLeft
			//Q3OrthographicCamera_SetRight
			//Q3OrthographicCamera_SetTop
			//Q3OrthographicCamera_SetBottom
			err = kQ3Failure;
			}
		else
		if (theType == kQ3CameraTypeViewPlane)
			{
// ???
			//Q3ViewPlaneCamera_SetHalfWidth
			//Q3ViewPlaneCamera_SetHalfHeight
			err = kQ3Failure;
			}
		else
			err = kQ3Failure;
		Q3Object_Dispose (theCamera);
		}
	if (err == kQ3Success)
		{
		if (viewerData->paneResizeNotifyCallbackMethod)
			err = viewerData->paneResizeNotifyCallbackMethod ((TQ3ViewerObject)(viewerData), viewerData->paneResizeNotifyCallbackData);
		}
	return err;
	}


static TQ3Status
e3drawtool (TQ3ViewerData* viewerData, TQ3Uns32 buttonMask, TQ3Uns32 buttonNumber)
	{
	if (viewerData->thePort == NULL)
		return kQ3Success;
	if (viewerData->flags & buttonMask)
		{
		TQ3Status status = kQ3Failure;
		TQ3XMetaHandler metaHandler = viewerData->metaHandlers [buttonNumber];
		TQ3Area area;
	#if (defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH) || (defined(QUESA_OS_WIN32) && QUESA_OS_WIN32)
		TQ3Int16 oldResFile = CurResFile ();
		TQ3Rect r;
		TQ3Rect iconRect;
		TQ3Result err = Q3ViewerGetButtonRect (viewerData, e3_viewer_button_to_external((TQ3ViewerObject) viewerData, buttonNumber), &r);
		if (err == noErr)
			{
			area.min.x = r.left;
			area.min.y = r.top;
			area.max.x = r.right;
			area.max.y = r.bottom;
			iconRect = r;
			iconRect.bottom = iconRect.top + 32;
			iconRect.right = iconRect.left + 32;
			}
		else
			return kQ3Failure;
		UseResFile (gResFileRefNum);
		if (buttonNumber == viewerData->currentButton)
			PlotIconID (&iconRect, kAlignNone, kTransformNone, -130);
		else
			PlotIconID (&iconRect, kAlignNone, kTransformNone, -129);
	#else
		status = Q3ViewerGetButtonRect (viewerData, e3_viewer_button_to_external((TQ3ViewerObject) viewerData, buttonNumber), &area);
	#endif
				
		if (metaHandler)
			{
			TQ3XViewerPluginDrawToolMethod method = (TQ3XViewerPluginDrawToolMethod)metaHandler (kQ3XMethodType_ViewerPluginDrawTool);
			if (method)
				status = method (viewerData, NULL, &area);
			}
		if (status == kQ3Failure)
			{
			// draw default tool icon
		#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
			PlotIconID (&iconRect, kAlignNone, kTransformNone, -128);
		#endif
			}
	#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
		UseResFile (oldResFile);
	#endif
		}
	return kQ3Success;
	}


static TQ3Status
e3clickbutton (TQ3ViewerData* theViewer, TQ3Uns32 button)
	{
	TQ3Uns32 oldButton = theViewer->currentButton;
	TQ3Status status = e3callindexplugin (theViewer, theViewer->currentButton, kQ3XMethodType_ViewerPluginUnclickTool, NULL);
	if (status == kQ3Failure) // plug-in does not want to be unclicked
		{
	#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
		SysBeep (30);
	#endif
		return status;
		}

	theViewer->currentButton = button; // change tool
	e3drawtool (theViewer, 0xFFFFFFFF, oldButton); // called afterwards so hilight is correct
	e3drawtool (theViewer, 0xFFFFFFFF, button);
	status = e3callindexplugin (theViewer, theViewer->currentButton, kQ3XMethodType_ViewerPluginClickTool, NULL);
	if (status == kQ3Failure)
		status = e3callindexplugin (theViewer, theViewer->currentButton, kQ3XMethodType_ViewerPluginDoubleClickTool, NULL);
	if (status == kQ3Failure)
		{
		theViewer->currentButton = oldButton; // change tool back to old tool
		e3drawtool (theViewer, 0xFFFFFFFF, button); // undraw the tool
		button = oldButton;
		status = e3callindexplugin (theViewer, theViewer->currentButton, kQ3XMethodType_ViewerPluginClickTool, NULL);
		if (status == kQ3Failure)
			status = e3callindexplugin (theViewer, theViewer->currentButton, kQ3XMethodType_ViewerPluginDoubleClickTool, NULL);
		e3drawtool (theViewer, 0xFFFFFFFF, button);
		}
	return status;
	}


static TQ3BoolResult
e3pointinrect (TQ3Point2D* pt, TQ3Rect* r)
	{
	if (pt->x >= r->left)
		{
		if (pt->x <= r->right)
			{
			if (pt->y <= r->bottom)
				{
				if (pt->y >= r->top)
					return 1;
				}
			}
		}
	return 0;
	}


#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH

static void
e3balloonhelp (TQ3ViewerData* theViewer, Point pt)
	{
#if TARGET_API_MAC_CARBON
	// Balloon Help has been replaced with MacHelp in Carbon.
	// But it's different enough to be a real pain in the tuckus.
	// Let's defer it for now, and come back to it later.
	#pragma unused(theViewer, pt)
#else
	if ((UInt32)(HMGetBalloons) != kUnresolvedCFragSymbolAddress)
		{
		if (HMGetBalloons()) // Balloon help is on
			{
			if (!HMIsBalloon ()) // there is already a balloon showing
				{
				GrafPtr oldPort;
				TQ3Int16 oldResFile = CurResFile ();
				TQ3Int16 strId = kDragAreaStrID;
				Rect alternateRect = theViewer->theRect;
				HMMessageRecord aHelpMsg;
				aHelpMsg.hmmHelpType = khmmString;
				aHelpMsg.u.hmmString [0] = 0;
				UseResFile (gResFileRefNum);

				if (PtInRect (pt, &theViewer->drawRect))
					{
					strId = kDrawAreaStrID;
					alternateRect = theViewer->drawRect;
					}
				else
					{
					UInt32 button = 0;
					Rect r;
					do
						{
						if (Q3ViewerGetButtonRect (theViewer, e3_viewer_button_to_external((TQ3ViewerObject) theViewer, button), &r) == kQ3GoodResult)
							{
							if (PtInRect (pt, &r))
								{
								alternateRect = r;
								if (e3callindexplugin (theViewer, button, kQ3XMethodType_ViewerPluginGetHelpString , (void*)aHelpMsg.u.hmmString) == kQ3Failure)
									{
									if (button == ePluginsButton)
										strId = kPluginsStrID;
									else
										strId = kDefaultStrID;
									}
								break; // while loop
								}
							}
						else
							{
							// not in a tool button
							if (theViewer->flags & kQ3ViewerControllerVisible)
								{
								r = theViewer->theRect;
								r.top = r.bottom - kQ3ControllerHeight;
								if (PtInRect (pt, &r))
									return; // in the tool area over nothing, so no help
								}
							// if get here, might be in the drag rgn
							if (theViewer->flags & kQ3ViewerCanDrawDragBorders)
								break; // while loop
							return;
							}
						++button;
						}
					while (true);
					}
				if (aHelpMsg.u.hmmString [0] == 0)
					GetIndString (aHelpMsg.u.hmmString, kHelpStrings, strId);
				UseResFile (oldResFile);
				if (aHelpMsg.u.hmmString [0])
					{
					GetPort (&oldPort);
					if (theViewer->thePort)
						SetPort ((GrafPtr)(theViewer->thePort));
					LocalToGlobal ((Point*)(&alternateRect.top));
					LocalToGlobal ((Point*)(&alternateRect.bottom));
					LocalToGlobal (&pt);
					SetPort (oldPort);
					HMShowBalloon (&aHelpMsg, pt, &alternateRect, NULL, 0, kHMRegularWindow | kHMSaveBitsNoWindow, 0);
					}
				}
			}
		}
#endif
	}


static pascal OSErr
e3sendproc (FlavorType theType, void *dragSendRefCon, ItemReference theItemRef, DragReference theDragRef)
	{
	OSErr err;
	CursHandle watch;
	TQ3ViewerData* theViewer = (TQ3ViewerData*)dragSendRefCon;
	CheckViewerFailure (theViewer);
	if (theType == 'PICT')
		{
		PicHandle picture;
		watch = GetCursor (watchCursor);
		if (watch)
			SetCursor (*watch);
		picture = Q3ViewerGetPict (theViewer);
		if (picture)
			{
			err = SetDragItemFlavorData (theDragRef, theItemRef, 'PICT', (Ptr)(*picture), GetHandleSize ((Handle)(picture)), 0);
			KillPicture (picture);
			return err;
			}
		}
	else
	if (theType == '3DMF')
		{
		Handle data = TempNewHandle (0, &err); // use temp memory as app may not have enough RAM
		if (err)
			{
			data = NewHandle (0);
			if (data)
				err = noErr;
			}
		if ((err == noErr) && data)
			{
			TQ3Uns32 size;
			watch = GetCursor (watchCursor);
			if (watch)
				SetCursor (*watch);
			size = Q3ViewerWriteData (theViewer, data);
			if (size > 0)
				{
				err = SetDragItemFlavorData (theDragRef, theItemRef, '3DMF', *data, size, 0);
				DisposeHandle (data);
				return err;
				}
			DisposeHandle (data);
			return memFullErr;
			}
		}
	else
	if (e3callallpluginswithparams (theViewer, kQ3XMethodType_ViewerPluginDoDrop, &theType, (TQ3Uns32)theDragRef, theItemRef) == kQ3Success)
		return noErr;
	else
		return badDragFlavorErr;
	return memFullErr;
	}


static Boolean
e3acceptabledrag (TQ3ViewerData* theViewer, DragReference theDragRef)
	{
	ItemReference theItemRef;
	OSErr err;
	err = GetDragItemReferenceNumber (theDragRef, 1, &theItemRef); // only look at the first item
	if (err == noErr)
		{
		UInt32 index = 1;
		do
			{
			FlavorType theType;
			err = GetFlavorType (theDragRef, theItemRef, index, &theType);
			if (err == noErr)
				{
				if (theType == '3DMF')
					return true;
				else
				if (theType == flavorTypeHFS)
					{
					Size actualSize = sizeof (HFSFlavor);
					HFSFlavor theSpec;
					err = GetFlavorData (theDragRef, theItemRef, theType, &theSpec, &actualSize, 0);
					if ((err == noErr) && (actualSize == sizeof (HFSFlavor)))
						return theSpec.fileType = '3DMF';
					}
				else
				if (e3callallpluginswithparams (theViewer, kQ3XMethodType_ViewerPluginDragAcceptable, &theType, (TQ3Uns32)theDragRef, theItemRef) == kQ3Success)
					return true;
				// continues with next flavor from here
				}
			else
				return false;
			}
		while (true);
		}
	return false;
	}


static pascal OSErr
e3trackinghandler (DragTrackingMessage message, WindowPtr theWindow, void *handlerRefCon, DragReference theDragRef)
	{
	static Boolean sHilited = false;
	TQ3ViewerData* theViewer = (TQ3ViewerData*)handlerRefCon;
	CheckViewerFailure (theViewer);
	switch (message)
		{
		case kDragTrackingEnterHandler:
		case kDragTrackingLeaveHandler:
		case kDragTrackingEnterWindow:
			return noErr;
		case kDragTrackingInWindow:
			{
			Point mouse;
			GrafPtr oldPort;
			GetPort (&oldPort);
			SetPortWindowPort(theWindow);
			GetMouse (&mouse);
			if (PtInRect (mouse, &theViewer->drawRect))
				{
				if (!sHilited)
					{
					if (theViewer != gDragViewer) // I do not accept my own drags
						{
						if (e3acceptabledrag (theViewer, theDragRef))
							{
							RgnHandle hiliteFrame = NewRgn ();
							if (hiliteFrame)
								{
								OSErr err;
								Boolean frameInside = (theViewer->flags & kQ3ViewerDrawDragBorder) == 0;
								RectRgn (hiliteFrame, &theViewer->drawRect);
								sHilited = true;
								err = ShowDragHilite (theDragRef, hiliteFrame, frameInside);
								DisposeRgn (hiliteFrame);
								SetPort (oldPort);
								return err;
								}
							}
						}
					}
				else // it is correctly hilited
					{
					SetPort (oldPort);
					return noErr;
					}
				}
			sHilited = false;
			HideDragHilite (theDragRef);
			SetPort (oldPort);
			return noErr;
			}
		case kDragTrackingLeaveWindow:
			if (sHilited)
				{
				sHilited = false;
				return HideDragHilite (theDragRef);
				}
			return noErr;
		default:
			return noErr;
		}
	}


static pascal OSErr
e3receivehandler (WindowPtr, void *handlerRefCon, DragReference theDragRef)
	{
	OSErr err = dragNotAcceptedErr;
	TQ3ViewerData* theViewer = (TQ3ViewerData*)handlerRefCon;
	CheckViewerFailure (theViewer);
	// how do I check that the drag did not drop in my window??
	// I can not look at GetDragAttributes and see if dragInsideSenderWindow bit is set
	// because there may be two viewers in the same window
	// so I will test against a global
	if (theViewer == gDragViewer) // I do not accept my own drags
		return dragNotAcceptedErr;
	if ((theViewer->flags & kQ3ViewerDragMode) && ((theViewer->flags & kQ3ViewerDraggingInOff) == 0)) // can allow dragging in
		{
		Point mouse;
		Point pinnedMouse;
		err = GetDragMouse (theDragRef, &mouse, &pinnedMouse);
		if (err == noErr)
			{
// ???
			// could test to see if you dropped a plug-in into the toolbar
			
			if (PtInRect (mouse, &theViewer->drawRect))
				{
				ItemReference theItemRef;
				err = GetDragItemReferenceNumber (theDragRef, 1, &theItemRef); // only look at the first item
				if (err == noErr)
					{
					UInt32 index = 1;
					do
						{
						FlavorType theType;
						err = GetFlavorType (theDragRef, theItemRef, index, &theType);
						if (err == noErr)
							{
							if (theType == '3DMF')
								{
								Size dataSize;
								err = GetFlavorDataSize (theDragRef, theItemRef, theType, &dataSize);
								if (err == noErr)
									{
									Handle h = NewHandle (dataSize);
									if (h)
										{
										Size actualSize = dataSize;
										HLock (h);
										err = GetFlavorData (theDragRef, theItemRef, theType, *h, &actualSize, 0);
										if ((err == noErr) && (actualSize == dataSize))
											err = Q3ViewerUseData (theViewer, *h, actualSize);
										DisposeHandle (h);
										return err;
										}
									else
										err = memFullErr;
									}
								}
							else
							if (theType == flavorTypeHFS)
								{
								Size actualSize = sizeof (HFSFlavor);
								HFSFlavor theSpec;
								err = GetFlavorData (theDragRef, theItemRef, theType, &theSpec, &actualSize, 0);
								if ((err == noErr) && (actualSize == sizeof (HFSFlavor)) && (theSpec.fileType == '3DMF'))
									{
									TQ3Int16 refnum;
									err = FSpOpenDF (&theSpec.fileSpec, fsRdPerm, &refnum);
									if (err == noErr)
										{
										err = Q3ViewerUseFile (theViewer, refnum);
										FSClose (refnum);
										return err;
										}
									}
								}
							}
						++index;
						}
					while (err == noErr);
					}
				}
			}
		}
	return err;
	}


static void
e3dodrag (TQ3ViewerData* theViewer, TQ3EventRecord* theEvent)
	{
	if ((UInt32)(NewDrag) != kUnresolvedCFragSymbolAddress)
		{
		DragReference theDrag;
		OSErr err = NewDrag (&theDrag);
		if ((err == noErr) && theDrag)
			{
			CursHandle h;
			DragSendDataUPP sendProcUPP;
			// create a drag region
			RgnHandle theRegion = NewRgn ();
			TQ3Int16 oldResFile = CurResFile ();
			UseResFile (gResFileRefNum);
			if (theRegion)
				{
				GrafPtr oldPort;
				RgnHandle innerRgn;
				Rect r = theViewer->drawRect;
				GetPort (&oldPort);
				if (theViewer->thePort)
					SetPort ((GrafPtr)(theViewer->thePort));
				LocalToGlobal ((Point*)(&r.top));
				LocalToGlobal ((Point*)(&r.bottom));
				SetPort (oldPort);
				RectRgn (theRegion, &r);
				innerRgn = NewRgn ();
				if (innerRgn)
					{
					// Carve out interior of region sothat it's just a one-pixel thick outline of the item rectangle
					CopyRgn (theRegion, innerRgn);		
					InsetRgn (innerRgn, 1, 1);  
					DiffRgn (theRegion, innerRgn, theRegion);
					DisposeRgn (innerRgn);
					}
				}
			
			theViewer->theDrag = theDrag; // used in e3callallplugins
			
			// add a send data handler
			#if TARGET_API_MAC_CARBON
				sendProcUPP = NewDragSendDataUPP (e3sendproc);
			#else
				sendProcUPP = NewDragSendDataProc (e3sendproc);
			#endif
			if (sendProcUPP)
				SetDragSendProc (theDrag, sendProcUPP, theViewer);
			
			// add your flavors
			AddDragItemFlavor (theDrag, 0, '3DMF', NULL, 0, 0);
			AddDragItemFlavor (theDrag, 0, 'PICT', NULL, 0, 0);
			e3callallplugins (theViewer, kQ3XMethodType_ViewerPluginAddDragFlavors);
			
			// set the bounds
			SetDragItemBounds (theDrag, 0, &theViewer->drawRect);
			
			// do translucent dragging if present
// ???
			
			// do the drag
			gDragViewer = theViewer;
			h = GetCursor (kClosedHandCursor);
			if (h)
				SetCursor (*h);
			TrackDrag (theDrag, theEvent, theRegion);
			gDragViewer = NULL;
			
			// if drag dropped in trash, then clear the model
// ???
			
			// clean up
			theViewer->theDrag = NULL;
			DisposeDrag (theDrag);
			if (theRegion)
				DisposeRgn (theRegion);
			if (sendProcUPP)
				{
				#if TARGET_API_MAC_CARBON
					DisposeDragSendDataUPP (sendProcUPP);
				#else
					DisposeRoutineDescriptor (sendProcUPP);
				#endif
				}
			UseResFile (oldResFile);
			}
		}
	}

#endif





//=============================================================================
//      e3_viewer_button_to_internal :	Convert an external button index to the
//										internal index.
//-----------------------------------------------------------------------------
//		Note :	We currently use a hard-coded array of buttons, so we need this
//				translation routine to go between indices into this array and
//				the 'array' of buttons that the app thinks exist (i.e., the
//				buttons it turned on via its flags).
//-----------------------------------------------------------------------------
static TQ3Uns32
e3_viewer_button_to_internal(TQ3ViewerObject theViewer, TQ3Uns32 theButton)
{	TQ3ViewerData		*viewerData = (TQ3ViewerData *) theViewer;
	TQ3Uns32			externalButtons[eLastButton];
	TQ3Uns32			n;



	// Build the list of external buttons
	n = 0;
	
	if (viewerData->flags & kQ3ViewerButtonCamera)
		externalButtons[n++] = eCameraButton;
	
	if (viewerData->flags & kQ3ViewerButtonTruck)
		externalButtons[n++] = eTruckButton;
	
	if (viewerData->flags & kQ3ViewerButtonOrbit)
		externalButtons[n++] = eOrbitButton;
	
	if (viewerData->flags & kQ3ViewerButtonZoom)
		externalButtons[n++] = eZoomButton;
	
	if (viewerData->flags & kQ3ViewerButtonDolly)
		externalButtons[n++] = eDollyButton;
	
	if (viewerData->flags & kQ3ViewerButtonReset)
		externalButtons[n++] = eResetButton;
	
	if (viewerData->flags & kQ3ViewerButtonOptions)
		externalButtons[n++] = eOptionsButton;



	// Look up the button
	if (theButton > eFirstButton && theButton < eLastButton)
		theButton = externalButtons[theButton];
	
	return(theButton);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3ViewerGetVersion : Gets the viewer version.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Result
Q3ViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
	{
	if (majorRevision)
		*majorRevision = kQ3MajorRevision;
	if (minorRevision)
		*minorRevision = kQ3MinorRevision;
	return kQ3GoodResult;
	}



//=============================================================================
//      Q3ViewerGetReleaseVersion : Gets the viewer release version.
//-----------------------------------------------------------------------------

TQ3Result
Q3ViewerGetReleaseVersion(TQ3Uns32 *releaseRevision)
	{
	if (releaseRevision)
		*releaseRevision = (kQ3MajorRevision << 16) | (kQ3MinorRevision << 8) | kQ3FixRevision;
	return kQ3GoodResult;
	}




//=============================================================================
//      Q3ViewerNew : Creates a new viewer object.
//-----------------------------------------------------------------------------
TQ3ViewerObject
Q3ViewerNew(TQ3Port port, ConstTQ3Rect *rect, TQ3Uns32 flags)
	{
	TQ3Uns32 index;
	TQ3ViewerData* viewerData;
	if (rect == NULL)
		return NULL;
	if (Q3Initialize () == kQ3Failure)
		return NULL;
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	if (!gResFileRefNum)
		{
		Boolean wasChanged;
		FSSpec fileSpec;
		OSErr err;
		if (!gAliasHandle)
			return NULL;
		err = ResolveAlias (NULL, gAliasHandle, &fileSpec, &wasChanged);
		if (err)
			return NULL;
		gResFileRefNum = FSpOpenResFile (&fileSpec, fsRdPerm);
		err = ResError ();
		if (err)
			return NULL;
		}
	++gResFileCount;
#endif
	viewerData = (TQ3ViewerData*)E3Memory_Allocate (sizeof (TQ3ViewerData));
	if (viewerData)
		{
		TQ3Status status = kQ3Failure;
		viewerData->validViewer = kQ3ValidViewer;
		for (index = eFirstButton; index < eLastButton; ++index)
			viewerData->metaHandlers [index]		= BaseToolMetaHandler;
		viewerData->metaHandlers [eCameraButton]	= CameraToolMetaHandler;
		viewerData->metaHandlers [eTruckButton]		= TruckToolMetaHandler;
		viewerData->metaHandlers [eOrbitButton]		= OrbitToolMetaHandler;
		viewerData->metaHandlers [eZoomButton]		= ZoomToolMetaHandler;
		viewerData->metaHandlers [eDollyButton]		= DollyToolMetaHandler;
		viewerData->metaHandlers [eResetButton]		= ResetToolMetaHandler;
		viewerData->metaHandlers [eOptionsButton]	= OptionsToolMetaHandler;
		viewerData->metaHandlers [eAboutBoxButton]	= AboutToolMetaHandler;

		viewerData->backfaceObject = NULL;
		if (flags == 0) // the scrapbook sends through flags of 0 and never calls Q3ViewerSetFlags
			viewerData->flags = kQ3ViewerInternalDefault;
		else
		if (flags & kQ3ViewerDefault)
			{
			flags &= ~kQ3ViewerDefault; // knock off default bit
			viewerData->flags = kQ3ViewerInternalDefault | flags;
			}
		else
			viewerData->flags = flags;

	#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
		viewerData->statusBar = NULL;
		viewerData->theDrag = NULL;
		if ((UInt32)(NewDrag) == kUnresolvedCFragSymbolAddress)
			{
			viewerData->flags &= ~kQ3ViewerDragMode;
			viewerData->flags &= ~kQ3ViewerDraggingInOff;
			viewerData->flags &= ~kQ3ViewerDraggingOutOff;
			}

		if (viewerData->flags & kQ3ViewerDraggingInOff)
			{
			viewerData->receiveHandler = NULL;
			viewerData->trackingHandler = NULL;
			}
		else
		if (port && (viewerData->flags & kQ3ViewerDragMode))
			{
			#if TARGET_API_MAC_CARBON
				viewerData->receiveHandler = NewDragReceiveHandlerUPP (e3receivehandler);
			#else
				viewerData->receiveHandler = NewDragReceiveHandlerProc (e3receivehandler);
			#endif
			if (viewerData->receiveHandler)
				InstallReceiveHandler (viewerData->receiveHandler, (WindowRef)(port), viewerData);
			#if TARGET_API_MAC_CARBON
				viewerData->trackingHandler = NewDragTrackingHandlerUPP (e3trackinghandler);
			#else
				viewerData->trackingHandler = NewDragTrackingHandlerProc (e3trackinghandler);
			#endif
			if (viewerData->trackingHandler)
				InstallTrackingHandler (viewerData->trackingHandler, (WindowRef)(port), viewerData);
			}
		else
			{
			viewerData->receiveHandler = NULL;
			viewerData->trackingHandler = NULL;
			}
	#endif
		viewerData->thePort = port;
		viewerData->theRect = *rect;
		viewerData->drawRect = *rect;
		if (viewerData->flags & kQ3ViewerControllerVisible)
			{
			viewerData->drawRect.bottom -= kQ3ControllerHeight;
			viewerData->drawRect.left += kQ3ControllerWidth;
			}
		if (viewerData->flags & kQ3ViewerCanDrawDragBorders)
			{
			viewerData->drawRect.right -= kQ3DragBorderWidth;
			viewerData->drawRect.left += kQ3DragBorderWidth;
			viewerData->drawRect.top += kQ3DragBorderWidth;
			viewerData->drawRect.bottom -= kQ3DragBorderWidth;
			}
		viewerData->callbackMethod = NULL;
		viewerData->callbackData = NULL;
		viewerData->windowResizeCallbackMethod = NULL;
		viewerData->windowResizeCallbackData = NULL;
		viewerData->paneResizeNotifyCallbackMethod = NULL;
		viewerData->paneResizeNotifyCallbackData = NULL;
		viewerData->currentButton = eOrbitButton;
		viewerData->phongObject = Q3PhongIllumination_New ();
		viewerData->theObjects = Q3DisplayGroup_New ();
		viewerData->theViewHints = Q3Group_New ();
		viewerData->otherObjects = Q3Group_New ();
		viewerData->theView = Q3View_New ();
		if (viewerData->theView)
			{
			TQ3CameraObject theCamera;
			TQ3ViewAngleAspectCameraData theCameraData;
			
			theCameraData.cameraData.placement.cameraLocation.x = 0.0f;
			theCameraData.cameraData.placement.cameraLocation.y = 0.0f;
			theCameraData.cameraData.placement.cameraLocation.z = 100.0f;
			theCameraData.cameraData.placement.pointOfInterest.x = 0.0f;
			theCameraData.cameraData.placement.pointOfInterest.y = 0.0f;
			theCameraData.cameraData.placement.pointOfInterest.z = 0.0f;
			theCameraData.cameraData.placement.upVector.x = 0.0f;
			theCameraData.cameraData.placement.upVector.y = 1.0f;
			theCameraData.cameraData.placement.upVector.z = 0.0f;
			theCameraData.cameraData.range.hither = 1.0f;
			theCameraData.cameraData.range.yon = 1000.0f;
			theCameraData.cameraData.viewPort.origin.x = -1.0f;
			theCameraData.cameraData.viewPort.origin.y = 1.0f;
			theCameraData.cameraData.viewPort.width = 2.0f;
			theCameraData.cameraData.viewPort.height = 2.0f;
			theCameraData.fov = 0.3f;
			theCameraData.aspectRatioXToY = (float)(viewerData->drawRect.right - viewerData->drawRect.left)
											/ (float)(viewerData->drawRect.bottom - viewerData->drawRect.top);
			theCamera = Q3ViewAngleAspectCamera_New (&theCameraData);
			if (theCamera)
				{
				status = Q3View_SetCamera (viewerData->theView, theCamera);
				if (status == kQ3Success)
					{
					TQ3DrawContextObject theContext;
				#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
					TQ3MacDrawContextData drawContextData;
					e3setdefaultdrawcontext (&viewerData->drawRect, &drawContextData.drawContextData);
					drawContextData.window = e3_viewer_GetPortWindow(port);
					drawContextData.grafPort = port;
					drawContextData.library = kQ3Mac2DLibraryNone;
					drawContextData.viewPort = NULL; // do not support GX
					theContext = Q3MacDrawContext_New (&drawContextData);
				#elif defined(QUESA_OS_WIN32) && QUESA_OS_WIN32
					TQ3Win32DCDrawContextData drawContextData;
					e3setdefaultdrawcontext (&viewerData->drawRect, &drawContextData.drawContextData);
					drawContextData.hdc = (HDC)port; // (temp, need function call here). Be warned: port may be NULL
					theContext = Q3Win32DCDrawContext_New (&drawContextData);
				#else
					theContext = NULL; // be warned: port may be NULL
				#endif
					if (theContext)
						{
						status = Q3View_SetDrawContext (viewerData->theView, theContext);
						Q3Object_Dispose (theContext);
						if (status == kQ3Success)
							{
							status = Q3View_SetRendererByType (viewerData->theView, kQ3RendererTypeInteractive);
							if (status == kQ3Success)
								{
								// set up a light group
								TQ3GroupObject lightGroup = Q3LightGroup_New ();
								if (lightGroup) // does not matter if do not have any lights
									{
									TQ3LightObject light;
									TQ3DirectionalLightData lightData;
									lightData.lightData.isOn = kQ3True;
									lightData.lightData.brightness = 0.65f;
									lightData.lightData.color.r = 1.0f;
									lightData.lightData.color.g = 1.0f;
									lightData.lightData.color.b = 1.0f;
									light = Q3AmbientLight_New (&lightData.lightData);
									if (light)
										{
										Q3Group_AddObject (lightGroup, light);
										Q3Object_Dispose (light);
										}
									lightData.castsShadows = kQ3True;
									lightData.direction.x = -0.5f;
									lightData.direction.y = -0.5f;
									lightData.direction.z = 0.5f;
									light = Q3DirectionalLight_New (&lightData);
									if (light)
										{
										Q3Group_AddObject (lightGroup, light);
										Q3Object_Dispose (light);
										}
									lightData.direction.x = 0.5f;
									light = Q3DirectionalLight_New (&lightData);
									if (light)
										{
										Q3Group_AddObject (lightGroup, light);
										Q3Object_Dispose (light);
										}
									Q3View_SetLightGroup (viewerData->theView, lightGroup); // does not matter if fails
									Q3Object_Dispose (lightGroup);
									}
								}
							}
						}
					else
						status = kQ3Failure;
					}
				Q3Object_Dispose (theCamera);
				}
			}
		
		if (status == kQ3Failure)
			{
			Q3ViewerDispose (viewerData);
			viewerData = NULL;
			}
		}
	if (viewerData)
		{
		e3clickbutton (viewerData, viewerData->currentButton);
		e3callallplugins (viewerData, kQ3XMethodType_ViewerPluginNewViewer);
		e3callallplugins (viewerData, kQ3XMethodType_ViewerPluginGroupChanged);
		e3callallplugins (viewerData, kQ3XMethodType_ViewerPluginCameraChanged);
		e3callallplugins (viewerData, kQ3XMethodType_ViewerPluginRendererChanged);
		e3callallplugins (viewerData, kQ3XMethodType_ViewerPluginLightsChanged);
		e3callallplugins (viewerData, kQ3XMethodType_ViewerPluginDrawContextChanged);
		if (port)
			Q3ViewerSetPort ((TQ3ViewerObject)viewerData, port); // need to do this as it sets up the drag and drop stuff
		}
	else
		Q3Exit ();
	return viewerData;
	}





//=============================================================================
//      Q3ViewerDispose : Disposes of the viewer object.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerDispose(TQ3ViewerObject theViewer)
	{
	TQ3ViewerData* viewerData = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	viewerData->validViewer = kQ3InvalidViewer;
	if (viewerData->receiveHandler)
		{
		RemoveReceiveHandler (viewerData->receiveHandler, (WindowRef)(viewerData->thePort));
		#if TARGET_API_MAC_CARBON
			DisposeDragReceiveHandlerUPP (viewerData->receiveHandler);
		#else
			DisposeRoutineDescriptor (viewerData->receiveHandler);
		#endif
		viewerData->receiveHandler = NULL;
		}
	if (viewerData->trackingHandler)
		{
		RemoveTrackingHandler (viewerData->trackingHandler, (WindowRef)(viewerData->thePort));
		#if TARGET_API_MAC_CARBON
			DisposeDragTrackingHandlerUPP (viewerData->trackingHandler);
		#else
			DisposeRoutineDescriptor (viewerData->trackingHandler);
		#endif
		viewerData->trackingHandler = NULL;
		}
	if (viewerData->theView)
		{
		Q3Object_Dispose (viewerData->theView);
		viewerData->theView = NULL;
		}
	if (viewerData->theObjects)
		{
		Q3Object_Dispose (viewerData->theObjects);
		viewerData->theObjects = NULL;
		}
	if (viewerData->theViewHints)
		{
		Q3Object_Dispose (viewerData->theViewHints);
		viewerData->theViewHints = NULL;
		}
	if (viewerData->otherObjects)
		{
		Q3Object_Dispose (viewerData->otherObjects);
		viewerData->otherObjects = NULL;
		}
	if (viewerData->backfaceObject)
		{
		Q3Object_Dispose (viewerData->backfaceObject);
		viewerData->backfaceObject = NULL;
		}
	if (viewerData->phongObject)
		{
		Q3Object_Dispose (viewerData->phongObject);
		viewerData->phongObject = NULL;
		}
	e3callallplugins (viewerData, kQ3XMethodType_ViewerPluginDeleteViewer);
	E3Memory_Free (&((void *) viewerData));
	if (gResFileCount)
		{
		--gResFileCount;
		if (gResFileCount == 0)
			{
			CloseResFile (gResFileRefNum);
			gResFileRefNum = 0;
			}
		}
	// do not recode this as kQ3GoodResult is noErr on MacOS
	if (Q3Exit () == kQ3Success)
		return kQ3GoodResult;
	else
		return kQ3BadResult;
	}





//=============================================================================
//      Q3ViewerUseFile : Reads in the objects from a file.
//-----------------------------------------------------------------------------
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
OSErr
Q3ViewerUseFile(TQ3ViewerObject theViewer, SInt32 refNum)
	{
	TQ3StorageObject store;
	CheckViewerFailure (theViewer);
	store = Q3MacintoshStorage_New (refNum);
	if (store)
		{
		TQ3ViewerData* viewerData = (TQ3ViewerData*)theViewer;
		TQ3Status err = e3readfile (viewerData, store);
		Q3Object_Dispose (store);
		if (err == kQ3Success)
			{
			Q3ViewerSetCameraByView (theViewer, kQ3ViewerCameraRestore);
			return noErr;
			}
		}
	return paramErr;
	}
#endif

#if defined(QUESA_OS_WIN32) && QUESA_OS_WIN32
TQ3Status
Q3ViewerUseFile(TQ3ViewerObject theViewer, HANDLE fileHandle)
	{
	TQ3Status err = kQ3Failure;
	TQ3StorageObject store;
	CheckViewerFailure (theViewer);
	store = Q3Win32Storage_New (fileHandle);
	if (store)
		{
		TQ3ViewerData* viewerData = (TQ3ViewerData*)theViewer;
		err = e3readfile (viewerData, store);
		Q3Object_Dispose (store);
		if (err == kQ3Success)
			Q3ViewerSetCameraByView (theViewer, kQ3ViewerCameraRestore);
		}
	return err;
	}
#endif




//=============================================================================
//      Q3ViewerUseData : Reads in the objects from a 3DMF stream.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerUseData(TQ3ViewerObject theViewer, void *data, TQ3Int32 size)
	{
	TQ3StorageObject store;
	if (data == NULL)
		return kQ3BadResult;
	CheckViewerFailure (theViewer);
	store = Q3MemoryStorage_New ((unsigned char*)data, size);
	if (store)
		{
		TQ3ViewerData* viewerData = (TQ3ViewerData*)theViewer;
		TQ3Status status = e3readfile (viewerData, store);
		Q3Object_Dispose (store);
		if (status == kQ3Success)
			{
			Q3ViewerSetCameraByView (theViewer, kQ3ViewerCameraRestore);
			return kQ3GoodResult;
			}
		}
	return kQ3BadResult;
	}



//=============================================================================
//      Q3ViewerWriteFile : Writes the objects to a file.
//-----------------------------------------------------------------------------
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
OSErr
Q3ViewerWriteFile(TQ3ViewerObject theViewer, TQ3Int32 refNum)
	{
	TQ3StorageObject store;
	CheckViewerFailure (theViewer);
	store = Q3MacintoshStorage_New (refNum);
	if (store)
		{
		TQ3ViewerData* viewerData = (TQ3ViewerData*)theViewer;
		TQ3Status status = e3writefile (viewerData, store);
		Q3Object_Dispose (store);
		if (status == kQ3Success)
			return noErr;
		}
	return paramErr;
	}
#endif

#if defined(QUESA_OS_WIN32) && QUESA_OS_WIN32
TQ3Status
Q3ViewerWriteFile(TQ3ViewerObject theViewer, HANDLE fileHandle)
	{
	TQ3Status err = kQ3Failure;
	TQ3StorageObject store;
	CheckViewerFailure (theViewer);
	store = Q3Win32Storage_New (fileHandle);
	if (store)
		{
		TQ3ViewerData* viewerData = (TQ3ViewerData*)theViewer;
		err = e3writefile (viewerData, store);
		Q3Object_Dispose (store);
		}
	return err;
	}
#endif




//=============================================================================
//      Q3ViewerWriteData : Writes the objects to a Handle.
//-----------------------------------------------------------------------------
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH

TQ3Uns32
Q3ViewerWriteData(TQ3ViewerObject theViewer, Handle data)
	{
	TQ3StorageObject store;
	CheckViewerFailure (theViewer);
	if (data == NULL)
		return 0;
	store = Q3HandleStorage_New (data, GetHandleSize (data));
	if (store)
		{
		TQ3ViewerData* viewerData = (TQ3ViewerData*)theViewer;
		TQ3Status err = e3writefile (viewerData, store);
		TQ3Uns32 result = GetHandleSize (data);
		Q3Object_Dispose (store);
		if (err == kQ3Success)
			return result; // return the actual size of the data written
		}
	return 0;
	}
	
#else

TQ3Status
Q3ViewerWriteData(
	TQ3ViewerObject		theViewer,
	void				*data,
	TQ3Uns32			dataSize,
	TQ3Uns32			*actualDataSize)
	{
	TQ3StorageObject store;
	TQ3Uns32 actualSize;
	if (data == NULL)
		return kQ3Failure;
	CheckViewerFailure (theViewer);
	store = Q3MemoryStorage_NewBuffer (data, dataSize, &actualSize);
	if (store)
		{
		TQ3ViewerData* viewerData = (TQ3ViewerData*)theViewer;
		TQ3Status err = e3writefile (viewerData, store);
		Q3Object_Dispose (store);
		if (actualDataSize)
			*actualDataSize = actualSize;
		return err;
		}
	return kQ3Failure;
	}
	
#endif


//=============================================================================
//      Q3ViewerDraw : Draws the objects and the control strip.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerDraw(TQ3ViewerObject theViewer)
	{
	TQ3Result err;
	CheckViewerFailure (theViewer);
	err = Q3ViewerDrawControlStrip (theViewer);
	if (!err)
		err = Q3ViewerDrawContent (theViewer);
	return err;
	}



//=============================================================================
//      Q3ViewerDrawContent : Draws the objects.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerDrawContent(TQ3ViewerObject theViewer)
	{
	TQ3DrawContextObject theContext;
	TQ3ViewObject theView;
	TQ3Status status;
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	GrafPtr oldPort;
	OSErr err = noErr;
#endif
	TQ3ViewerData* viewerData = (TQ3ViewerData*)theViewer;
return kQ3Success;
	CheckViewerFailure (theViewer);
	if (!viewerData->thePort)
		return kQ3BadResult;
	theView = viewerData->theView;
	if (theView == NULL)
		return kQ3BadResult;
	
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	GetPort (&oldPort);
	SetPort ((GrafPtr)(viewerData->thePort));
#endif
	if (viewerData->flags & kQ3ViewerCanDrawDragBorders)
		{
		TQ3Rect r = viewerData->drawRect;
		r.right += kQ3DragBorderWidth;
		r.left -= kQ3DragBorderWidth;
		r.top -= kQ3DragBorderWidth;
		r.bottom += kQ3DragBorderWidth;
	#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
		{{
		Pattern myGray = {0xAA, 0x00, 0xAA, 0x00, 0xAA, 0x00, 0xAA, 0x00};
		PenNormal ();
		PenSize (kQ3DragBorderWidth, kQ3DragBorderWidth);
		PenPat (&myGray);
		FrameRect (&r);
		PenNormal ();
		}}
	#endif
		}
	// set an idleProc
	if ((Q3View_GetDrawContext (theView, &theContext) == kQ3Success) && theContext)
		{
		TQ3RendererObject theRenderer;
		if (Q3Object_GetLeafType (theContext) == kQ3DrawContextTypePixmap) // only should happen if writing a picture
			{
			Q3View_SetIdleMethod (theView, NULL, NULL);
			Q3View_SetIdleProgressMethod (theView, NULL, NULL);
			}
		else
		if ((Q3View_GetRenderer (theView, &theRenderer) == kQ3Success) && theRenderer)
			{
			Q3View_SetIdleMethod (theView, e3idlemethod, (void*)(theViewer));
			if (!Q3Renderer_IsInteractive (theRenderer))
				{
				Q3View_SetIdleProgressMethod (theView, e3idleprogressmethod, (void*)(theViewer));
			#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
				if (!viewerData->statusBar)
					{
					Rect r = {0, 0, 0, 0}; // get correct rectangle
					SInt16 procID = scrollBarProc; // change to slider control if running Mac OS 8 or later
					viewerData->statusBar = NewControl ((WindowRef)(viewerData->thePort), &r, "\p", true, 0, 0, 100, procID, 0);
					}
			#endif
				}
			Q3Object_Dispose (theRenderer);
			}
		Q3Object_Dispose (theContext);
		}
	e3callallplugins (viewerData, kQ3XMethodType_PluginBeforeDraw);
#if __REALQD3D__
	gViewMode = kQ3ViewModeDrawing;
#endif

	status = Q3View_StartRendering (theView);
	if (status == kQ3Success)
		{
		TQ3ViewStatus drawStatus;
		do
			{
			status = e3callallplugins (viewerData, kQ3XMethodType_PluginAfterStartDraw);
			if (status == kQ3Success)
		#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
				status = e3submitloopwitherr (viewerData, &err);
		#else
				status = e3submitloop (viewerData);
		#endif
			if (status == kQ3Success)
				drawStatus = e3callallviewstatusplugins (viewerData, kQ3XMethodType_PluginBeforeEndDraw);
			else
				e3callallviewstatusplugins (viewerData, kQ3XMethodType_PluginBeforeEndDraw);
			drawStatus = Q3View_EndRendering (theView);
			if (status == kQ3Failure)
				break;
			}
		while (drawStatus == kQ3ViewStatusRetraverse);
		if (drawStatus == kQ3ViewStatusError)
			status = kQ3Failure;
		}
#if __REALQD3D__
	gViewMode = kQ3ViewModeInactive;
#endif
	e3callallplugins (viewerData, kQ3XMethodType_PluginAfterDraw);
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	SetPort (oldPort);
	if (viewerData->statusBar)
		{
		DisposeControl (viewerData->statusBar);
		viewerData->statusBar = NULL;
		}
	if (!err)
		{
		if (status == kQ3Failure)
			err = kQ3BadResult;
		}
	return err;
#else
	return status;
#endif
	}




//=============================================================================
//      Q3ViewerDrawControlStrip : Draws just the control strip.
//-----------------------------------------------------------------------------
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH

OSErr
Q3ViewerDrawControlStrip(TQ3ViewerObject theViewer)
	{
	TQ3ViewerData* viewerData = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	if (viewerData->flags & kQ3ViewerControllerVisible)
		{
		RGBColor myGray = {0xCFFF, 0xCFFF, 0xCFFF};
		Rect r;
		GrafPtr oldPort;
		if (!viewerData->thePort)
			return paramErr;
		r = viewerData->theRect;
		r.top = viewerData->drawRect.bottom;
		if (viewerData->flags & kQ3ViewerCanDrawDragBorders)
			r.top += kQ3DragBorderWidth;
		GetPort (&oldPort);
		SetPort ((GrafPtr)viewerData->thePort);
		RGBForeColor (&myGray);
		PaintRect (&r);
		ForeColor (blackColor);
		PenNormal();
		MoveTo (r.left, r.top);
		LineTo (r.right - 1, r.top); // -1 because the pen hangs 1 pixel over to the right
		e3drawtool (viewerData, kQ3ViewerButtonCamera,	eCameraButton);
		e3drawtool (viewerData, kQ3ViewerButtonTruck,	eTruckButton);
		e3drawtool (viewerData, kQ3ViewerButtonOrbit,	eOrbitButton);
		e3drawtool (viewerData, kQ3ViewerButtonZoom,	eZoomButton);
		e3drawtool (viewerData, kQ3ViewerButtonDolly,	eDollyButton);
		e3drawtool (viewerData, kQ3ViewerButtonReset,	eResetButton);
		e3drawtool (viewerData, kQ3ViewerButtonOptions,	eOptionsButton);
		//e3drawtool (viewerData, kQ3ViewerButtonOptions,	ePluginsButton);
// ???
		e3drawtool (viewerData, 0xFFFFFFFF,	eAboutBoxButton);
		SetPort (oldPort);
		}
	return noErr;
	}

#else

#pragma mark -
#pragma mark --- unimplemented Windows OS ---

TQ3Status
Q3ViewerDrawControlStrip(TQ3ViewerObject theViewer)
	{
	TQ3ViewerData* viewerData = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	if (viewerData->flags & kQ3ViewerControllerVisible)
		{
		if (!viewerData->thePort)
			return kQ3Failure;
		e3drawtool (viewerData, kQ3ViewerButtonCamera,	eCameraButton);
		e3drawtool (viewerData, kQ3ViewerButtonTruck,	eTruckButton);
		e3drawtool (viewerData, kQ3ViewerButtonOrbit,	eOrbitButton);
		e3drawtool (viewerData, kQ3ViewerButtonZoom,	eZoomButton);
		e3drawtool (viewerData, kQ3ViewerButtonDolly,	eDollyButton);
		e3drawtool (viewerData, kQ3ViewerButtonReset,	eResetButton);
		e3drawtool (viewerData, kQ3ViewerButtonOptions,	eOptionsButton);
		//e3drawtool (viewerData, kQ3ViewerButtonOptions,	ePluginsButton);
// ???
		e3drawtool (viewerData, 0xFFFFFFFF,	eAboutBoxButton);
		}
	// To be implemented...
	return kQ3Success;
	}


#pragma mark --- end unimplemented ---
#pragma mark -

#endif


//=============================================================================
//      Q3ViewerGetButtonRect : Returns the rect of a specified button.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, TQ3Rect *rect)
	{
	TQ3Uns32 flags;
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	if (rect == NULL)
		return kQ3BadResult;
	CheckViewerFailure (theViewer);



	// Translate the button index
	button = e3_viewer_button_to_internal(theViewer, button);


	rect->left = rect->right = viewer->theRect.left;
	rect->top = rect->bottom = viewer->theRect.bottom - kQ3BottomOffset;
	flags = viewer->flags;
	if ((flags & kQ3ViewerControllerVisible) == 0) // no controller
		return kQ3BadResult;
	rect->top -= kQ3ButtonHeight;
	switch (button)
		{
		// no breaks in the case statements - order is vital
		case eAboutBoxButton: // About box must always be first as is used in Q3ViewerGetMinimumDimension
			rect->left += kQ3ButtonSpacing;
		case eOptionsButton:
			if (flags & kQ3ViewerButtonOptions)
				rect->left += kQ3ButtonSpacing;
		case eResetButton:
			if (flags & kQ3ViewerButtonReset)
				rect->left += kQ3ButtonSpacing + kQ3ButtonGap;
// ???
		/*
		case ePluginsButton:
			if (flags & kQ3ViewerButtonOptions) // do not have a special flag yet
				rect->left += kQ3ButtonSpacing;
		*/
		case eZoomButton:
			if (flags & kQ3ViewerButtonZoom)
				rect->left += kQ3ButtonSpacing;
		case eDollyButton:
			if (flags & kQ3ViewerButtonDolly)
				rect->left += kQ3ButtonSpacing;
		case eOrbitButton:
			if (flags & kQ3ViewerButtonOrbit)
				rect->left += kQ3ButtonSpacing;
		case eTruckButton:
			if (flags & kQ3ViewerButtonTruck)
				rect->left += kQ3ButtonSpacing + kQ3ButtonGap;
		case eCameraButton:
			if (flags & kQ3ViewerButtonCamera)
				rect->left += kQ3ButtonGap;
			break;
		default:
			return kQ3BadResult;
		}
	rect->right = rect->left + kQ3ButtonWidth;
	return kQ3GoodResult;
	}




//=============================================================================
//      Q3ViewerGetCurrentButton : Returns the currently selected button.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3ViewerGetCurrentButton(TQ3ViewerObject theViewer)
	{	TQ3Uns32		theButton;


	CheckViewerFalse (theViewer);


	// Translate the button index
	theButton = e3_viewer_button_to_external(theViewer, ((TQ3ViewerData*)theViewer)->currentButton);
	return(theButton);
	}




//=============================================================================
//      Q3ViewerSetCurrentButton : Sets the current button.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 button)
	{
	CheckViewerFailure (theViewer);



	// Translate the button index
	button = e3_viewer_button_to_internal(theViewer, button);



	if (button < eLastButton)
		{
		if (e3clickbutton ((TQ3ViewerData*)theViewer, button) == kQ3Success)
			return kQ3GoodResult;
		}
	return kQ3BadResult;
	}





//=============================================================================
//      Q3ViewerUseGroup : Replaces the viewers group with a new group.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerUseGroup(TQ3ViewerObject theViewer, TQ3GroupObject group)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	if (group == NULL)
		return kQ3BadResult;
	CheckViewerFailure (theViewer);
	if (viewer->otherObjects)
		Q3Group_EmptyObjects (viewer->otherObjects);
	if (viewer->theViewHints)
		Q3Group_EmptyObjects (viewer->theViewHints);
	if (viewer->theObjects)
		Q3Object_Dispose (viewer->theObjects);
	viewer->theObjects = Q3Shared_GetReference (group);
	e3callallplugins (viewer, kQ3XMethodType_ViewerPluginGroupChanged);
	Q3ViewerSetCameraByView (theViewer, kQ3ViewerCameraRestore);
	return kQ3GoodResult;
	}





//=============================================================================
//      Q3ViewerGetGroup : Returns a reference to the current group.
//-----------------------------------------------------------------------------
TQ3GroupObject
Q3ViewerGetGroup(TQ3ViewerObject theViewer)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerNULL (theViewer);
	if (viewer->theObjects)
		return Q3Shared_GetReference (viewer->theObjects);
	return NULL;
	}





//=============================================================================
//      Q3ViewerSetBackgroundColor : Sets the background color.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
	{
	TQ3Result err = kQ3BadResult;
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	if (color == NULL)
		return err;
	CheckViewerFailure (theViewer);
	if (viewer->theView)
		{
		TQ3DrawContextObject theContext;
		if ((Q3View_GetDrawContext (viewer->theView, &theContext) == kQ3Success) && theContext)
			{
			if (Q3DrawContext_SetClearImageColor (theContext, color) == kQ3Success)
				{
				e3callallplugins (viewer, kQ3XMethodType_ViewerPluginDrawContextChanged);
				err = kQ3GoodResult;
				}
			Q3Object_Dispose (theContext);
			}
		}
	return err;
	}





//=============================================================================
//      Q3ViewerGetBackgroundColor : Gets the background color.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerGetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
	{
	TQ3Result err = kQ3BadResult;
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	if (color == NULL)
		return err;
	CheckViewerFailure (theViewer);
	if (viewer->theView)
		{
		TQ3DrawContextObject theContext;
		if ((Q3View_GetDrawContext (viewer->theView, &theContext) == kQ3Success) && theContext)
			{
			if (Q3DrawContext_GetClearImageColor (theContext, color) == kQ3Success)
				{
				e3callallplugins (viewer, kQ3XMethodType_ViewerPluginDrawContextChanged);
				err = kQ3GoodResult;
				}
			Q3Object_Dispose (theContext);
			}
		}
	return err;
	}





//=============================================================================
//      Q3ViewerGetView : Returns the viewers view object.
//-----------------------------------------------------------------------------
TQ3ViewObject
Q3ViewerGetView(TQ3ViewerObject theViewer)
	{
	CheckViewerNULL (theViewer);
	return ((TQ3ViewerData*)theViewer)->theView;
	}





//=============================================================================
//      Q3ViewerRestoreView : Don't know what this is supposed to do.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerRestoreView(TQ3ViewerObject theViewer)
	{
	return Q3ViewerSetCameraByView (theViewer, kQ3ViewerCameraRestore);
	}




//=============================================================================
//      Q3ViewerSetFlags : Sets the viewers flags.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetFlags(TQ3ViewerObject theViewer, TQ3Uns32 flags)
	{
	TQ3Uns32 oldFlags;
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	
	oldFlags = viewer->flags;
	
	if (flags & kQ3ViewerDefault)
		{
		flags &= ~kQ3ViewerDefault; // knock off default bit
		flags |= kQ3ViewerInternalDefault; // add on my flags, leaving any other bits the app may have set
		}
	
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	if ((UInt32)(NewDrag) == kUnresolvedCFragSymbolAddress)
		{
		flags &= ~kQ3ViewerDragMode;
		flags &= ~kQ3ViewerDraggingInOff;
		flags &= ~kQ3ViewerDraggingOutOff;
		}
#endif

	viewer->flags = flags;
	
	oldFlags &= flags; // old flags now holds the bits that were NOT changed
	if ((oldFlags & kQ3ViewerControllerVisible) == 0) // kQ3ViewerControllerVisible bit WAS changed
		Q3ViewerSetBounds (viewer, &viewer->theRect);

#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	if ((oldFlags & kQ3ViewerDraggingInOff) == 0) // kQ3ViewerDraggingInOff bit WAS changed
		{
		WindowRef window = e3_viewer_GetPortWindow(viewer->thePort);
		if (flags & kQ3ViewerDraggingInOff)
			{
			if (viewer->receiveHandler) // was on before
				{
				RemoveReceiveHandler (viewer->receiveHandler, window);
				#if TARGET_API_MAC_CARBON
					DisposeDragReceiveHandlerUPP (viewer->receiveHandler);
				#else
					DisposeRoutineDescriptor (viewer->receiveHandler);
				#endif
				viewer->receiveHandler = NULL;
				}
			if (viewer->trackingHandler) // was on before
				{
				RemoveTrackingHandler (viewer->trackingHandler, window);
				#if TARGET_API_MAC_CARBON
					DisposeDragTrackingHandlerUPP (viewer->trackingHandler);
				#else
					DisposeRoutineDescriptor (viewer->trackingHandler);
				#endif
				viewer->trackingHandler = NULL;
				}
			}
		else
			{
			if (viewer->receiveHandler == NULL) // was not on before
				{
				#if TARGET_API_MAC_CARBON
					viewer->receiveHandler = NewDragReceiveHandlerUPP (e3receivehandler);
				#else
					viewer->receiveHandler = NewDragReceiveHandlerProc (e3receivehandler);
				#endif
				if (viewer->receiveHandler)
					InstallReceiveHandler (viewer->receiveHandler, window, theViewer);
				}
			if (viewer->trackingHandler == NULL) // was not on before
				{
				#if TARGET_API_MAC_CARBON
					viewer->trackingHandler = NewDragTrackingHandlerUPP (e3trackinghandler);
				#else
					viewer->trackingHandler = NewDragTrackingHandlerProc (e3trackinghandler);
				#endif
				if (viewer->trackingHandler)
					InstallTrackingHandler (viewer->trackingHandler, window, theViewer);
				}
			}
		}
#endif
	// possibly could have hidden the currently selected tool. Do we leave it still active??
// ???
	Q3ViewerDraw (theViewer);
	return kQ3GoodResult;
	}





//=============================================================================
//      Q3ViewerGetFlags : Returns the viewers flags.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3ViewerGetFlags(TQ3ViewerObject theViewer)
	{
	CheckViewerFalse (theViewer);
	return((TQ3ViewerData*)theViewer)->flags;
	}




//=============================================================================
//      Q3ViewerSetBounds : Sets the viewers bounds.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetBounds(TQ3ViewerObject theViewer, TQ3Rect *bounds)
	{
	// do not test to see if bounds is the same as viewer->theRect as it will be when calling Q3ViewerSetFlags
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	if (bounds == NULL)
		return kQ3BadResult;
	CheckViewerFailure (theViewer);
	if (viewer->theView)
		{
		TQ3DrawContextObject theContext;
		if ((Q3View_GetDrawContext (viewer->theView, &theContext) == kQ3Success) && theContext)
			{
			TQ3Status err;
			TQ3Area area;
		#if (defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH) || (defined(QUESA_OS_WIN32) && QUESA_OS_WIN32)
			area.min.x = bounds->left;
			area.min.y = bounds->top;
			area.max.x = bounds->right;
			area.max.y = bounds->bottom;
		#else
			area = bounds;
		#endif
			err = Q3DrawContext_SetPane (theContext, &area);
			Q3Object_Dispose (theContext);
			if (err == kQ3Success)
				{
				e3callallplugins (viewer, kQ3XMethodType_ViewerPluginDrawContextChanged);
				viewer->theRect = *bounds;
				viewer->drawRect = *bounds;
				if (viewer->flags & kQ3ViewerControllerVisible)
					{
					viewer->drawRect.bottom -= kQ3ControllerHeight;
					viewer->drawRect.left += kQ3ControllerWidth;
					}
				if (viewer->flags & kQ3ViewerCanDrawDragBorders)
					{
					viewer->drawRect.right -= kQ3DragBorderWidth;
					viewer->drawRect.left += kQ3DragBorderWidth;
					viewer->drawRect.top += kQ3DragBorderWidth;
					viewer->drawRect.bottom -= kQ3DragBorderWidth;
					}
				e3resetcamerabounds (viewer);
				Q3ViewerDraw (theViewer);
				return kQ3GoodResult;
				}
			}
		}
	return kQ3BadResult;
	}




//=============================================================================
//      Q3ViewerGetBounds : Gets the viewers bounds.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerGetBounds(TQ3ViewerObject theViewer, TQ3Rect *bounds)
	{
	CheckViewerFailure (theViewer);
	if (bounds == NULL)
		return kQ3BadResult;
	*bounds = ((TQ3ViewerData*)theViewer)->theRect;
	return kQ3GoodResult ;
	}





//=============================================================================
//      Q3ViewerSetDimension : Sets the width and height of the viewer.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetDimension(TQ3ViewerObject theViewer, TQ3Uns32 width, TQ3Uns32 height)
	{
	TQ3Rect r;
	CheckViewerFailure (theViewer);
	r = ((TQ3ViewerData*)theViewer)->theRect;
	r.right = r.left + width;
	r.bottom = r.top + height;
	return Q3ViewerSetBounds (theViewer, &r);
	}





//=============================================================================
//      Q3ViewerGetDimension : Gets the width and height of the viewer.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerGetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
#if (defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH) || (defined(QUESA_OS_WIN32) && QUESA_OS_WIN32)
	if (width)
		*width = viewer->theRect.right - viewer->theRect.left;
	if (height)
		*height = viewer->theRect.bottom - viewer->theRect.top;
#else
	if (width)
		*width = viewer->theRect.max.x - viewer->theRect.min.x;
	if (height)
		*height = viewer->theRect.max.y - viewer->theRect.min.y;
#endif
	return kQ3GoodResult;
	}





//=============================================================================
//      Q3ViewerGetMinimumDimension : Gets the minimum dimensions of the viewer.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerGetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height)
	{
	TQ3Rect r;
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	TQ3Int16 theWidth = kQ3MinimumWidth;
	TQ3Int16 theHeight = kQ3MinimumHeight;
	CheckViewerFailure (theViewer);
	if (viewer->flags & kQ3ViewerControllerVisible)
		theHeight += kQ3ControllerHeight;
	if (Q3ViewerGetButtonRect (theViewer, e3_viewer_button_to_external(theViewer, eAboutBoxButton), &r) == kQ3GoodResult)
		theWidth = r.right + kQ3ButtonGap;
	if (height)
		*height = theHeight;
	if (width)
		*width = theWidth;
	return(noErr);
	}




//=============================================================================
//      Q3ViewerAdjustCursor : Adjusts the cursor if in the viewers rect.
//-----------------------------------------------------------------------------
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH

Boolean
Q3ViewerAdjustCursor(TQ3ViewerObject theViewer, Point *pt)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	if (pt == NULL)
		return false;
	CheckViewerFalse (theViewer);
	if (PtInRect (*pt, &viewer->theRect))
		{
		Rect r;
		// call plug-in balloon help if on
		e3balloonhelp (viewer, *pt);

		if (PtInRect (*pt, &viewer->drawRect))
			{
			if (e3callindexplugin (viewer, viewer->currentButton, kQ3XMethodType_ViewerPluginCursorAdjust, (void*)pt) == kQ3Success)
				return true;
			}
		
		r = viewer->theRect;
		r.top = r.bottom - kQ3ControllerHeight;
		if (PtInRect (*pt, &r)) // in tool rect
			InitCursor ();
		else
			{
			CursHandle h;
			TQ3Int16 oldResFile = CurResFile ();
			UseResFile (gResFileRefNum);
			h = GetCursor (kHandCursor);
			if (h)
				SetCursor (*h);
			else
				InitCursor ();
			UseResFile (oldResFile);
			}
		return true;
		}
	return false;
	}

#else

#pragma mark -
#pragma mark --- unimplemented Windows OS ---

TQ3Boolean
Q3ViewerAdjustCursor(TQ3ViewerObject theViewer, TQ3Int32 x , TQ3Int32 y)
	{
	CheckViewerFalse (theViewer);
	// To be implemented...
	return kQ3False;
	}

#pragma mark --- end unimplemented ---
#pragma mark -

#endif




//=============================================================================
//      Q3ViewerCursorChanged : Don't know what this is supposed to do.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerCursorChanged(TQ3ViewerObject theViewer)
	{
	CheckViewerFailure (theViewer);
	InitCursor (); // ???
	return kQ3GoodResult;
	}



//=============================================================================
//      Q3ViewerGetState : Returns the state of the viewer.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3ViewerGetState(TQ3ViewerObject theViewer)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	TQ3Uns32 result = kQ3ViewerEmpty;
	CheckViewerFalse (theViewer);
	if (viewer->theObjects)
		{
		TQ3GroupPosition pos;
		if ((Q3Group_GetFirstPosition (viewer->theObjects, &pos) == kQ3Success) && pos) // there is at least one object
			result |= kQ3ViewerHasModel;
		}
	// kQ3ViewerHasUndo
	return result;
	}





//=============================================================================
//      Q3ViewerGetUndoString : Returns the string for undo.
//-----------------------------------------------------------------------------
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
Boolean
Q3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, TQ3Uns32 *actualSize)
#else
TQ3Boolean
Q3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, TQ3Uns32 stringSize, TQ3Uns32 *actualSize)
#endif
	{
	CheckViewerFalse (theViewer);
#if ! (defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH)
	if (stringSize > 0)
#endif
	if (str)
		*str = 0;
	if (actualSize)
		*actualSize = 1;
	return 0;
	}





//=============================================================================
//      Q3ViewerUndo : Undoes the last action.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerUndo(TQ3ViewerObject theViewer)
	{
	CheckViewerFailure (theViewer);
	return noErr; // we do not support Undo for now
	}





//=============================================================================
//      Q3ViewerClear : Clears the objects in the viewer.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerClear(TQ3ViewerObject theViewer)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	if (viewer->theObjects)
		{
		Q3Group_EmptyObjects (viewer->theObjects);
		Q3ViewerDraw (theViewer);
		}
	return kQ3GoodResult;
	}





//=============================================================================
//      Q3ViewerCut : Cuts the objects and puts them on the clipboard.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerCut(TQ3ViewerObject theViewer)
	{
	TQ3Result err = Q3ViewerCopy (theViewer);
	if (err == kQ3GoodResult)
		return Q3ViewerClear (theViewer);
	return err;
	}




//=============================================================================
//      Q3ViewerCopy : Copies the objects onto the clipboard.
//-----------------------------------------------------------------------------
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH

OSErr
Q3ViewerCopy(TQ3ViewerObject theViewer)
	{
	TQ3StorageObject theStore;
	Handle h;
	
	CheckViewerFailure (theViewer);
	h = NewHandle (0);
	if (!h) // try temp mem
		{
		OSErr err;
		h = TempNewHandle (0, &err);
		}
	#if !TARGET_API_MAC_CARBON
		if (!h) // try system mem
			h = NewHandleSys (0);
	#endif
	if (!h)
		return memFullErr;

	theStore = Q3HandleStorage_New (h, 0);
	DisposeHandle (h); // dispose of my copy
	if (theStore)
		{
		TQ3Status status = e3writefile ((TQ3ViewerData*)theViewer, theStore);
		if (status == kQ3Success)
			{
			PicHandle picH;
			UInt32 result;
			TQ3Uns32 validSize;
			ZeroScrap (); // clear out the old scrap
			if (Q3HandleStorage_Get (theStore, &h, &validSize) == kQ3Success)
				result = PutScrap (validSize, '3DMF', *h);
			else
				result = memFullErr;
			Q3Object_Dispose (theStore);
			
			// put a picture on the scrap too
			picH = Q3ViewerGetPict (theViewer);
			if (picH)
				{
				PutScrap (GetHandleSize ((Handle)(picH)), 'PICT', *picH); // don't care if could not get PICT onto scrap
				KillPicture (picH);
				}
			UnloadScrap ();
			if (result < 0)
				return result;
			return noErr;
			}
		else
			Q3Object_Dispose (theStore);
		}
	else
		return memFullErr;
	return paramErr;
	}
	
#else

TQ3Status
Q3ViewerCopy(TQ3ViewerObject theViewer)
	{
	CheckViewerFailure (theViewer);

	// To be implemented...
	return kQ3Failure;
	}

#endif




//=============================================================================
//      Q3ViewerPaste : Copies the objects from the clipboard.
//-----------------------------------------------------------------------------
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH

OSErr
Q3ViewerPaste(TQ3ViewerObject theViewer)
	{
	TQ3Int32 size;
	TQ3Int32 offset = 0; // must zero out offset
	CheckViewerFailure (theViewer);
	size = LoadScrap ();
	if (size < 0)
		return size;
	size = GetScrap (NULL, '3DMF', &offset); // read the length of the scrap if it exists
	if (size > 0)
		{
		TQ3StorageObject theStore;
		Handle h = NewHandle (size);
		if (!h) // try temp mem
			{
			OSErr err;
			h = TempNewHandle (size, &err);
			}
		#if !TARGET_API_MAC_CARBON
			if (!h) // try system mem
				h = NewHandleSys (size);
		#endif
		if (!h)
			return memFullErr;
		size = GetScrap (h, '3DMF', &offset); // read the scrap into the handle
		if (size < 0)
			{
			DisposeHandle (h);
			return size;
			}
		theStore = Q3HandleStorage_New (h, size);
		DisposeHandle (h); // dispose of my copy
		if (theStore)
			{
			TQ3Status status = e3readfile ((TQ3ViewerData*)(theViewer), theStore);
			Q3Object_Dispose (theStore);
			if (status == kQ3Success)
				{
				Q3ViewerSetCameraByView (theViewer, kQ3ViewerCameraRestore); // this will draw the view too
				return noErr;
				}
			}
		else
			return memFullErr;
		}
	if (size < 0)
		return size;
	return paramErr;
	}

#else

TQ3Status
Q3ViewerPaste(TQ3ViewerObject theViewer)
	{
	CheckViewerFailure (theViewer);
	// To be implemented...
	return kQ3Failure;
	}

#endif



//=============================================================================
//      Q3ViewerMouseDown : Called when the mouse if first clicked.
//-----------------------------------------------------------------------------
TQ3BoolResult
Q3ViewerMouseDown(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
	{
	TQ3Point2D pt;
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFalse (theViewer);
	gContinueTracking = 0;
	pt.x = x;
	pt.y = y;
	if (e3pointinrect (&pt, &viewer->theRect))
		{
		TQ3Uns32 button;
		TQ3Rect r;
		
		// see if mouse hit main area
		if (e3pointinrect (&pt, &viewer->drawRect))
			{
			e3callindexplugin (viewer, viewer->currentButton, kQ3XMethodType_ViewerPluginDoToolStart, (void*)(&pt));
			return 1;
			}

		// see if mouse hit a button
		button = 0;
		while (Q3ViewerGetButtonRect (theViewer, e3_viewer_button_to_external(theViewer, button), &r) == kQ3GoodResult)
			{
			if (e3pointinrect (&pt, &r))
				{
				Q3ViewerSetCurrentButton (theViewer, e3_viewer_button_to_external(theViewer, button));
				return 1;
				}
			++button;
			}
		
		r = viewer->theRect;
		r.top = r.bottom - kQ3ControllerHeight;
		if (e3pointinrect (&pt, &r)) // in tool rect
			return 1;
		
	#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
		// mouse must be in drag border
		if ((viewer->flags & kQ3ViewerDragMode) && ((viewer->flags & kQ3ViewerDraggingOutOff) == 0))
			e3dodrag (viewer, gEventPtr);
	#endif
		return 1; // It was in my area so I handled it even if I did nothing
		}
	return 0;
	}


//=============================================================================
//      Q3ViewerContinueTracking : Tracks the mouse while it is down.
//-----------------------------------------------------------------------------
TQ3BoolResult
Q3ViewerContinueTracking(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	TQ3Point2D pt;
	CheckViewerFalse (theViewer);
	pt.x = x;
	pt.y = y;
	if (e3pointinrect (&pt, &viewer->drawRect) || gContinueTracking)
		{
		gContinueTracking = 1; // once you are in you can stay in while the mouse is down
		if (e3callindexplugin (viewer, viewer->currentButton, kQ3XMethodType_ViewerPluginDoToolTracking, (void*)(&pt)) == kQ3Failure)
			return 0;
	#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
		return StillDown ();
	#else
// To be implemented...
		return 0; // some sort of mouse is still down check
	#endif
		}
	return 0;
	}




//=============================================================================
//      Q3ViewerMouseUp : Does the stuff when the mouse is released.
//-----------------------------------------------------------------------------
TQ3BoolResult
Q3ViewerMouseUp(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	TQ3Point2D pt;
	gContinueTracking = 0;
	CheckViewerFalse (theViewer);
	pt.x = x;
	pt.y = y;
	e3callindexplugin (viewer, viewer->currentButton, kQ3XMethodType_ViewerPluginDoToolEnd, (void*)(&pt));
	return 1;
	}




//=============================================================================
//      Q3ViewerSetDrawingCallbackMethod : Sets the drawing callback method.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetDrawingCallbackMethod(TQ3ViewerObject theViewer, TQ3ViewerDrawingCallbackMethod callbackMethod, const void *data)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	viewer->callbackMethod = callbackMethod;
	viewer->callbackData = (void*)data;
	return kQ3GoodResult;
	}





//=============================================================================
//      Q3ViewerSetWindowResizeCallback : Sets the window resize callback method.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetWindowResizeCallback(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod windowResizeCallbackMethod, const void *data)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	viewer->windowResizeCallbackMethod = windowResizeCallbackMethod;
	viewer->windowResizeCallbackData = (void*)data;
	return kQ3GoodResult;
	}






//=============================================================================
//      Q3ViewerSetPaneResizeNotifyCallback : Sets the pane resize callback method.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetPaneResizeNotifyCallback(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod paneResizeNotifyCallbackMethod, const void *data)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	viewer->paneResizeNotifyCallbackMethod = paneResizeNotifyCallbackMethod;
	viewer->paneResizeNotifyCallbackData = (void*)data;
	return kQ3GoodResult;
	}




//=============================================================================
//      Q3ViewerGetCameraCount : Returns the number of custom views.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerGetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cnt)
	{
	if (cnt)
		{
		CheckViewerFailure (theViewer);
		if (Q3Group_CountObjects (((TQ3ViewerData*)(theViewer))->theViewHints, cnt) == kQ3Success)
			return kQ3GoodResult;
		}
	return kQ3BadResult;
	}





//=============================================================================
//      Q3ViewerSetCameraByNumber : Sets the view to user camera n.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraNo)
	{
	TQ3ViewerData* viewer;
	TQ3GroupPosition pos;
	TQ3Status status;
	CheckViewerFailure (theViewer);
	
	viewer = (TQ3ViewerData*)theViewer;
	status = Q3Group_GetFirstPosition (viewer->theViewHints, &pos);
	if (status == kQ3Success)
		{
		TQ3Uns32 count = 1;
		while (cameraNo < count)
			{
			status = Q3Group_GetNextPosition (viewer->theViewHints, &pos);
			if (status == kQ3Failure)
				break;
			++count;
			}
		if (status == kQ3Success)
			{
			TQ3Object object;
			status = Q3Group_GetPositionObject (viewer->theViewHints, pos, &object);
			if (status == kQ3Success)
				{
				TQ3CameraObject camera;
				status = Q3ViewHints_GetCamera (object, &camera);
				if ((status == kQ3Success) && camera)
					{
					status = Q3View_SetCamera (viewer->theView, camera);
					Q3Object_Dispose (camera);
					e3callallplugins (viewer, kQ3XMethodType_ViewerPluginCameraChanged);
					}
				}
			}
		}
	if (status == kQ3Success) // do not recode this as kQ3BadResult is paramErr on MacOS
		return kQ3GoodResult;
	else
		return kQ3BadResult;
	}




//=============================================================================
//      Q3ViewerSetCameraByView : Sets the view to a specific view.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType)
	{
	TQ3Status err = kQ3Failure;
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	if (viewer->theView)
		{
		TQ3CameraObject theCamera;
		if ((Q3View_GetCamera (viewer->theView, &theCamera) == kQ3Success) && theCamera)
			{
			TQ3BoundingBox bounds;
			TQ3CameraData cameraData;
			
			err = Q3Camera_GetData (theCamera, &cameraData);
			if (err == kQ3Success)
				err = e3getgroupbounds (viewer, &bounds);
			
			if ((err == kQ3Success) && (bounds.isEmpty == kQ3False))
				{
				TQ3Vector3D viewVector;
				TQ3ObjectType cameraType = Q3Object_GetLeafType (theCamera);
				float width = bounds.max.x - bounds.min.x;
				float height = bounds.max.y - bounds.min.y;
				float depth = bounds.max.z - bounds.min.z;
				float offset = width; // take the maximum of the bounds
				if (height > offset)
					offset = height;
				if (depth > offset)
					offset = depth;
				offset *= 4; // move the camera 4 times the bounds away
				
				if (viewType == kQ3ViewerCameraFit)
					{
					viewVector.x = cameraData.placement.pointOfInterest.x - cameraData.placement.cameraLocation.x;
					viewVector.y = cameraData.placement.pointOfInterest.y - cameraData.placement.cameraLocation.y;
					viewVector.z = cameraData.placement.pointOfInterest.z - cameraData.placement.cameraLocation.z;
					Q3Vector3D_Normalize (&viewVector, &viewVector);
					viewVector.x *= offset;
					viewVector.y *= offset;
					viewVector.z *= offset;
					}
				else
					{
					cameraData.placement.upVector.x = 0.0f;
					cameraData.placement.upVector.y = 1.0f;
					cameraData.placement.upVector.z = 0.0f;
					}
				
				// place the point of interest at the middle of the bounding box
				cameraData.placement.pointOfInterest.x = bounds.min.x + (width / 2.0f);
				cameraData.placement.pointOfInterest.y = bounds.min.y + (height / 2.0f);
				cameraData.placement.pointOfInterest.z = bounds.min.z + (depth / 2.0f);
				
				cameraData.placement.cameraLocation = cameraData.placement.pointOfInterest;
				
				switch (viewType)
					{
					case kQ3ViewerCameraRestore: // same as front view for now
						{
						cameraData.placement.cameraLocation.z += offset;
						break;
						}
					case kQ3ViewerCameraFit:
						{
						cameraData.placement.cameraLocation.x = cameraData.placement.pointOfInterest.x - viewVector.x;
						cameraData.placement.cameraLocation.y = cameraData.placement.pointOfInterest.y - viewVector.y;
						cameraData.placement.cameraLocation.z = cameraData.placement.pointOfInterest.z - viewVector.z;
						break;
						}
					case kQ3ViewerCameraFront:
						{
						cameraData.placement.cameraLocation.z += offset;
						break;
						}
					case kQ3ViewerCameraBack:
						{
						cameraData.placement.cameraLocation.z -= offset;
						break;
						}
					case kQ3ViewerCameraLeft:
						{
						cameraData.placement.cameraLocation.x -= offset;
						break;
						}
					case kQ3ViewerCameraRight:
						{
						cameraData.placement.cameraLocation.x += offset;
						break;
						}
					case kQ3ViewerCameraTop:
						{
						cameraData.placement.upVector.z        = -1.0f;
						cameraData.placement.upVector.y        =  0.0f;
						cameraData.placement.cameraLocation.y += offset;
						break;
						}
					case kQ3ViewerCameraBottom:
						{
						cameraData.placement.upVector.z        = 1.0f;
						cameraData.placement.upVector.y        = 0.0f;
						cameraData.placement.cameraLocation.y -= offset;
						break;
						}
					default:
						{
						Q3Object_Dispose (theCamera);
						return kQ3BadResult;
						}
					}
				
				if (cameraType == kQ3CameraTypeViewAngleAspect)
					err = Q3ViewAngleAspectCamera_SetFOV (theCamera, 0.3f);
				else
				if (cameraType == kQ3CameraTypeOrthographic)
					{
// ???
					//Q3OrthographicCamera_SetLeft
					//Q3OrthographicCamera_SetRight
					//Q3OrthographicCamera_SetTop
					//Q3OrthographicCamera_SetBottom
					err = kQ3Failure;
					}
				else
				if (cameraType == kQ3CameraTypeViewPlane)
					{
// ???
					//Q3ViewPlaneCamera_SetHalfWidth
					//Q3ViewPlaneCamera_SetHalfHeight
					err = kQ3Failure;
					}
				else
					err = kQ3Failure;
				
				if (err == kQ3Success)
					{
					viewVector.x = cameraData.placement.cameraLocation.x - cameraData.placement.pointOfInterest.x;
					viewVector.y = cameraData.placement.cameraLocation.y - cameraData.placement.pointOfInterest.y;
					viewVector.z = cameraData.placement.cameraLocation.z - cameraData.placement.pointOfInterest.z;
					cameraData.range.yon = 4.0f * sqrt (viewVector.x * viewVector.x + viewVector.y * viewVector.y + viewVector.z * viewVector.z); // Pythag
					cameraData.range.hither = cameraData.range.yon / 10000.0f;
					err = Q3Camera_SetData (theCamera, &cameraData);
					}
				
				}
			Q3Object_Dispose (theCamera);
			e3callallplugins (viewer, kQ3XMethodType_ViewerPluginCameraChanged);
			Q3ViewerDrawContent (theViewer);
			return kQ3GoodResult;
			}
		}
	if (err == kQ3Success) // do not recode this as kQ3BadResult is paramErr on MacOS
		return kQ3GoodResult;
	else
		return kQ3BadResult;
	}




//=============================================================================
//      Q3ViewerSetRendererType : Sets the viewers renderer to this type.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	if (viewer->theView)
		{
		if (Q3View_SetRendererByType (viewer->theView, rendererType) == kQ3Success)
			{
			e3callallplugins (viewer, kQ3XMethodType_ViewerPluginRendererChanged);
			Q3ViewerDrawContent (theViewer);
			return kQ3GoodResult;
			}
		}
	return kQ3BadResult;
	}





//=============================================================================
//      Q3ViewerGetRendererType : Gets the current renderer type.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerGetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	if (rendererType == NULL)
		return kQ3BadResult;
	*rendererType = kQ3ObjectTypeInvalid;
	CheckViewerFailure (theViewer);
	if (viewer->theView)
		{
		TQ3RendererObject renderer;
		if ((Q3View_GetRenderer (viewer->theView, &renderer) == kQ3Success) && renderer)
			{
			*rendererType = Q3Object_GetLeafType (renderer);
			Q3Object_Dispose (renderer);
			return kQ3GoodResult;
			}
		}
	return kQ3BadResult;
	}





//=============================================================================
//      Q3ViewerChangeBrightness : Changes the brightness of all the light to brightness.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerChangeBrightness(TQ3ViewerObject theViewer, float brightness)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	if (viewer->theView)
		{
		TQ3GroupObject lights;
		TQ3Status err = Q3View_GetLightGroup (viewer->theView, &lights);
		if (err == kQ3Success)
			{
			TQ3GroupPosition pos;
			err = Q3Group_GetFirstPosition (lights, &pos);
			if (err == kQ3Success)
				{
				do
					{
					TQ3Object object;
					err = Q3Group_GetPositionObject (lights, pos, &object);
					if (err == kQ3Success)
						{
						err = Q3Light_SetBrightness (object, brightness);
						if (err == kQ3Success)
							err = Q3Group_GetNextPosition (lights , &pos);
						}
					if (err == kQ3Failure)
						break;
					}
				while (pos);
				}
			Q3ViewerDrawContent (theViewer);
			}
		if (err == kQ3Failure) // do not recode this as kQ3BadResult is paramErr on MacOS
			return kQ3BadResult;
		return kQ3GoodResult;
		}
	return kQ3BadResult;
	}





//=============================================================================
//      Q3ViewerSetRemoveBackfaces : Adds a backface removal style.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean remove)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	if (remove)
		{
		if (!viewer->backfaceObject)
			viewer->backfaceObject = Q3BackfacingStyle_New (kQ3BackfacingStyleRemove);
		}
	else
	if (viewer->backfaceObject)
		{
		Q3Object_Dispose (viewer->backfaceObject);
		viewer->backfaceObject = NULL;
		}
	return kQ3GoodResult;
	}





//=============================================================================
//      Q3ViewerGetRemoveBackfaces : Gets whether backface removal is on.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerGetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *remove)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	if (remove == NULL)
		return kQ3BadResult;
	CheckViewerFailure (theViewer);
	if (viewer->backfaceObject)
		*remove = kQ3True;
	else
		*remove = kQ3False;
	return kQ3GoodResult;
	}





//=============================================================================
//      Q3ViewerSetPhongShading : Sets whether phong shading is on.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerSetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phong)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	if (phong)
		{
		if (!viewer->phongObject)
			viewer->phongObject = Q3PhongIllumination_New ();
		}
	else
	if (viewer->phongObject)
		{
		Q3Object_Dispose (viewer->phongObject);
		viewer->phongObject = NULL;
		}
	return kQ3GoodResult;
	}





//=============================================================================
//      Q3ViewerGetPhongShading : Gets whether phong shading is on.
//-----------------------------------------------------------------------------
TQ3Result
Q3ViewerGetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phong)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	if (phong == NULL)
		return kQ3BadResult;
	CheckViewerFailure (theViewer);
	if (viewer->phongObject)
		*phong = kQ3True;
	else
		*phong = kQ3False;
	return kQ3GoodResult;
	}


//=============================================================================
//      Q3ViewerGetPhongShading : Gets whether phong shading is on.
//-----------------------------------------------------------------------------


TQ3Status
Q3ViewerGetGroupBounds(TQ3ViewerObject theViewer, TQ3BoundingBox* bounds)
	{
	if (!theViewer || ((TQ3ViewerData*)(theViewer))->validViewer != kQ3ValidViewer)
		return kQ3Failure;
	if (bounds)
		return e3getgroupbounds ((TQ3ViewerData*) theViewer, bounds);
	return kQ3Failure;
	}



#pragma mark -
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
#pragma mark ----- Mac OS -----
//=============================================================================
//      Q3ViewerEvent : Returns whether this is a viewer event.
//-----------------------------------------------------------------------------
Boolean
Q3ViewerEvent(TQ3ViewerObject theViewer, EventRecord *evt)
	{
	Boolean result = false;
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	gEventPtr = evt;
	if (evt == NULL)
		return false;
	CheckViewerFalse (theViewer);
	e3callallpluginswithparams (viewer, kQ3XMethodType_ViewerPluginBeforeEvent, evt , 0 , 0);
	// these may alter evt
	switch (evt->what)
		{
		case mouseDown:
			{
			Point pt = evt->where;
			WindowPtr theWindow;
			TQ3Int16 part = FindWindow (pt, &theWindow);
			if (part == inGrow)
				{
				if (viewer->windowResizeCallbackMethod)
					{
					if (viewer->windowResizeCallbackMethod (theViewer, viewer->windowResizeCallbackData) == kQ3Success)
						result = true;
					}
				}
			else
			if (part == inContent)
				{
				GrafPtr thePort = (GrafPtr)(viewer->thePort);
				GrafPtr oldPort;
				GetPort (&oldPort);
				SetPort (thePort);
				GlobalToLocal (&pt);
				if (e3callindexplugin (viewer, viewer->currentButton, kQ3XMethodType_ViewerPluginDoToolEvent, (void*)(evt)) == kQ3Success)
					{
					if (Q3ViewerMouseDown (theViewer, pt.h, pt.v)) // this checks if the mouse down is in the viewer rect
						{
						while (Q3ViewerContinueTracking (theViewer, pt.h, pt.v))
							GetMouse (&pt);
						Q3ViewerMouseUp (theViewer, pt.h, pt.v);
						result = true;
						}
					}
				SetPort (oldPort);
				}
			break;
			}
		/*
		// Do not assume that the key is for the viewer.
		// Let the host application decide to call Q3ViewerHandleKeyEvent
		case keyDown:
		case autoKey:
			{
			result = Q3ViewerHandleKeyEvent (theViewer, evt);
			break;
			}
		case updateEvt:
		// Leave these to the Host application
			{
			GrafPtr thePort = (GrafPtr)(viewer->thePort);
			WindowPtr theWindow = (WindowPtr)evt->message ;
			if (theWindow == thePort)
				{
				BeginUpdate (thePort);
				Q3ViewerDraw (theViewer);
				EndUpdate(thePort);
				result = true;
				}
			break;
			}
		*/
		case activateEvt:
			{
			if ((GrafPtr)evt->message == (GrafPtr)(viewer->thePort))
				{
				Boolean becomingActive = ((evt->modifiers & activeFlag) == activeFlag);
				if (becomingActive)
					e3callallplugins (viewer, kQ3XMethodType_ViewerPluginActivate);
				else
					e3callallplugins (viewer, kQ3XMethodType_ViewerPluginDeactivate);
				}
			break;
			}
		}
	e3callallpluginswithparams (viewer, kQ3XMethodType_ViewerPluginAfterEvent, evt, 0 , 0);
	return result ;
	}



//=============================================================================
//      Q3ViewerHandleKeyEvent : Handles key down events.
//-----------------------------------------------------------------------------
//		Note : Could handle arrow keys ??
//-----------------------------------------------------------------------------
Boolean
Q3ViewerHandleKeyEvent(TQ3ViewerObject theViewer, EventRecord *evt)
	{
	if (evt == NULL)
		return false;
	CheckViewerFalse (theViewer);
	gEventPtr = evt;
	if (e3callindexplugin ((TQ3ViewerData*)theViewer, ((TQ3ViewerData*)theViewer)->currentButton, kQ3XMethodType_ViewerPluginDoKeyDown , (void*)evt) == kQ3Success)
		return true;
	return false;
	}



//=============================================================================
//      Q3ViewerGetPict : Converts the objects into a picture.
//-----------------------------------------------------------------------------
PicHandle
Q3ViewerGetPict(TQ3ViewerObject theViewer)
	{
	GrafPtr oldPort;
	OSErr err;
	Rect r;
	GWorldPtr theGWorld;
	PixMapHandle thePixMap;
	PicHandle thePicture = NULL;
	TQ3Int16 pixelDepth = 32;
	TQ3ViewObject theView;
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	
	CheckViewerNULL (theViewer);
	theView = viewer->theView;
	if (theView == NULL)
		return NULL;
	if (viewer->theObjects == NULL)
		return NULL;
	
	GetPort (&oldPort);
	r = viewer->drawRect;
	err = NewGWorld (&theGWorld, pixelDepth, &r, NULL, NULL, 0); // try app first as QD3D uses temp for its stuff
	if (err)
		err = NewGWorld (&theGWorld, pixelDepth, &r, NULL, NULL, useTempMem);
	if (err)
		{
		pixelDepth = 16; // try 16 bit
		err = NewGWorld (&theGWorld, pixelDepth, &r, NULL, NULL, 0);
		}
	if (err)
		err = NewGWorld (&theGWorld, pixelDepth, &r, NULL, NULL, useTempMem);
	if (err)
		return NULL;
	if (theGWorld == NULL)
		return NULL;
	
	thePixMap = GetGWorldPixMap (theGWorld);
	if (thePixMap && LockPixels (thePixMap))
		{
		Ptr baseAddr = GetPixBaseAddr (thePixMap);
		if (baseAddr)
			{
			TQ3DrawContextObject newDrawContext;
			TQ3DrawContextObject theContext;
			TQ3PixmapDrawContextData drawContextData;
			
			e3setdefaultdrawcontext (&r, &drawContextData.drawContextData);
			if ((Q3View_GetDrawContext (theView, &theContext) == kQ3Success) && theContext)
				Q3DrawContext_GetData (theContext, &drawContextData.drawContextData);
			drawContextData.pixmap.image = baseAddr;
			drawContextData.pixmap.width = r.right - r.left;
			drawContextData.pixmap.height = r.bottom - r.top;
			drawContextData.pixmap.rowBytes = (*thePixMap)->rowBytes & 0x3FFF; // mask off top two bits
			drawContextData.pixmap.pixelSize = pixelDepth;
			if (pixelDepth == 32)
				drawContextData.pixmap.pixelType = kQ3PixelTypeARGB32;
			else
				drawContextData.pixmap.pixelType = kQ3PixelTypeARGB16;
			drawContextData.pixmap.bitOrder = drawContextData.pixmap.byteOrder = kQ3EndianBig;
			
			drawContextData.drawContextData.pane.min.x = drawContextData.drawContextData.pane.min.y = 0.0f;
			drawContextData.drawContextData.pane.max.x = drawContextData.pixmap.width;
			drawContextData.drawContextData.pane.max.y = drawContextData.pixmap.height;
			
			newDrawContext = Q3PixmapDrawContext_New (&drawContextData);
			if (newDrawContext)
				{
				if (Q3View_SetDrawContext (theView, newDrawContext) == kQ3Success)
					{
					GWorldPtr oldGWorld;
					GDHandle oldDevice;
					TQ3Status status;
					TQ3Uns32 oldFlags = viewer->flags;
				
				#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
					// turn off the draw border flag
					viewer->flags &= ~kQ3ViewerDrawDragBorder;
				#endif
					
					e3callallplugins (viewer, kQ3XMethodType_ViewerPluginDrawContextChanged);
					
					GetGWorld (&oldGWorld, &oldDevice);
					SetGWorld (theGWorld, NULL);
					
					// Draw the objects
					err = Q3ViewerDrawContent (theViewer);
					if (err)
						status = kQ3Failure;
					else
						status = kQ3Success;
					
					// remember to call Sync
					if (status == kQ3Success)
						status = Q3View_Sync (theView);
					
					viewer->flags = oldFlags;
					SetGWorld (oldGWorld, oldDevice);
					
					// reset back the old draw context
					Q3View_SetDrawContext (theView, theContext);
					e3callallplugins (viewer, kQ3XMethodType_ViewerPluginDrawContextChanged);
					
					if (status == kQ3Success)
						{
						// now make a Picture from the GWorld
						RgnHandle oldClip;
						OpenCPicParams pictureHeader;
						CGrafPtr thePort = viewer->thePort;
						SetPort(thePort);
						oldClip = NewRgn ();
						if (oldClip)
							GetClip (oldClip); // for some reason I need to get and reset the clip region
						pictureHeader.srcRect = r;
						pictureHeader.hRes = pictureHeader.vRes = 0x00480000; //specifies 72 dpi
						pictureHeader.version = -2;
						pictureHeader.reserved1 = pictureHeader.reserved2 = 0;
						thePicture = OpenCPicture (&pictureHeader);
						if (thePicture)
							{
							ClipRect (&r); // this cliprect should go into the picture but it seems to effect the ports cliprect
							#if TARGET_API_MAC_CARBON
								CopyBits ((BitMapPtr)(*thePixMap), GetPortBitMapForCopyBits(thePort), &r, &r, srcCopy, NULL);
							#else
								CopyBits ((BitMapPtr)(*thePixMap), &((GrafPtr)thePort)->portBits, &r, &r, srcCopy, NULL);
							#endif
							ClosePicture ();
							}
						if (oldClip)
							{
							SetClip (oldClip);
							DisposeRgn (oldClip);
							}
						}
					}
				Q3Object_Dispose (newDrawContext);
				}
			Q3Object_Dispose (theContext);
			}		
		UnlockPixels (thePixMap);
		}
	
	DisposeGWorld (theGWorld);
	SetPort (oldPort);
	return thePicture;
	}




//=============================================================================
//      Q3ViewerSetPort : Sets the viewers port.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetPort(TQ3ViewerObject theViewer, CGrafPtr port)
	{
	TQ3ViewerData* viewer = (TQ3ViewerData*)theViewer;
	CheckViewerFailure (theViewer);
	if (viewer->theView && port)
		{
		TQ3DrawContextObject theContext;
		if ((Q3View_GetDrawContext (viewer->theView, &theContext) == kQ3Success) && theContext)
			{
			WindowRef window = e3_viewer_GetPortWindow(port);
			TQ3Status err = Q3MacDrawContext_SetWindow (theContext, window);
			if (err == kQ3Success)
				err = Q3MacDrawContext_SetGrafPort (theContext, port);
			Q3Object_Dispose (theContext);
			if (err == kQ3Success)
				{
				e3callallplugins (viewer, kQ3XMethodType_ViewerPluginDrawContextChanged);
				if (viewer->thePort)
					{
					// need to unset up a drag handler for viewer->thePort
					if (viewer->receiveHandler)
						{
						RemoveReceiveHandler (viewer->receiveHandler, window);
						#if TARGET_API_MAC_CARBON
							DisposeDragReceiveHandlerUPP (viewer->receiveHandler);
						#else
							DisposeRoutineDescriptor (viewer->receiveHandler);
						#endif
						viewer->receiveHandler = NULL;
						}
					if (viewer->trackingHandler)
						{
						RemoveTrackingHandler (viewer->trackingHandler, window);
						#if TARGET_API_MAC_CARBON
							DisposeDragTrackingHandlerUPP (viewer->trackingHandler);
						#else
							DisposeRoutineDescriptor (viewer->trackingHandler);
						#endif
						viewer->trackingHandler = NULL;
						}
					}
				viewer->thePort = port;
				Q3ViewerDraw (theViewer);
				// need to set up a drag handler for this port if flags specify it
				if ((viewer->flags & kQ3ViewerDragMode) && ((viewer->flags & kQ3ViewerDraggingInOff) == 0))
					{
					OSErr err;
					viewer->receiveHandler = NewDragReceiveHandlerUPP (e3receivehandler);
					if (viewer->receiveHandler)
						err = InstallReceiveHandler (viewer->receiveHandler, window, theViewer);
					viewer->trackingHandler = NewDragTrackingHandlerUPP (e3trackinghandler);
					if (viewer->trackingHandler)
						err = InstallTrackingHandler (viewer->trackingHandler, window, theViewer);
					}
				return noErr;
				}
			}
		}
	return paramErr;
	}


//=============================================================================
//      Q3ViewerGetPort : Returns the viewers GrafPort.
//-----------------------------------------------------------------------------
CGrafPtr
Q3ViewerGetPort(TQ3ViewerObject theViewer)
	{
	CheckViewerNULL (theViewer);
	return ((TQ3ViewerData*)theViewer)->thePort;
	}


#pragma mark ----- Mac OS -----
#endif

#pragma mark -

#if defined(QUESA_OS_WIN32) && QUESA_OS_WIN32
#pragma mark ---- Windows OS ----
#pragma mark --- unimplemented ---
//=============================================================================
//      Q3ViewerGetBitmap : Converts the objects into an HBITMAP.
//-----------------------------------------------------------------------------
HBITMAP
Q3ViewerGetBitmap(TQ3ViewerObject theViewer)
	{
	CheckViewerNULL (theViewer);
	// To be implemented...
	return NULL;
	}


//=============================================================================
//      Q3ViewerSetWindow : Sets the viewers window.
//-----------------------------------------------------------------------------
TQ3Status
Q3ViewerSetWindow(TQ3ViewerObject theViewer, HWND window)
	{
	CheckViewerFailure (theViewer);
	// To be implemented...
	return kQ3Failure;
	}


//=============================================================================
//      Q3ViewerGetWindow : Gets the viewers window.
//-----------------------------------------------------------------------------
HWND
Q3ViewerGetWindow(TQ3ViewerObject theViewer)
	{
	CheckViewerNULL (theViewer);
	return ((TQ3ViewerData*)theViewer)->thePort;
	}


//=============================================================================
//      Q3ViewerGetViewer : Gets the windows viewer.
//-----------------------------------------------------------------------------
TQ3ViewerObject
Q3ViewerGetViewer(HWND theWindow)
	{
	if (theWindow == NULL)
		return NULL:
	// To be implemented...
	return NULL;
	}


//=============================================================================
//      Q3ViewerGetControlStrip : Gets the viewers control strip.
//-----------------------------------------------------------------------------
HWND
Q3ViewerGetControlStrip(TQ3ViewerObject theViewer)
	{
	CheckViewerNULL (theViewer);
	return ((TQ3ViewerData*)theViewer)->theControllerWindow;
	}


#pragma mark --- end unimplemented ---
#pragma mark ---- Windows OS ----
#endif

#pragma mark -

#pragma mark ---- Other OS's ----


#pragma mark -

#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH


OSErr __ViewerInitialize (const CFragInitBlock* initBlock);
OSErr __ViewerTerminate (void);

#if defined(__MWERKS__)
extern OSErr __initialize(const CFragInitBlock *theInitBlock);
extern void __terminate(void);
#endif


OSErr __ViewerInitialize (const CFragInitBlock* initBlock)
	{
	OSErr err = __initialize (initBlock);
	if (err == noErr)
		{
		if (initBlock->fragLocator.where == kDataForkCFragLocator)
			err = NewAlias (NULL, initBlock->fragLocator.u.onDisk.fileSpec, &gAliasHandle);
		else
			err = paramErr;
		}
	return err;
	}



OSErr __ViewerTerminate (void)
	{
	if (gResFileRefNum)
		{
		CloseResFile (gResFileRefNum);
		gResFileRefNum = 0;
		}
	if (gAliasHandle)
		{
		DisposeHandle ((Handle)gAliasHandle);
		gAliasHandle = NULL;
		}
	gResFileCount = 0;
	__terminate();
	return noErr;
	}


#endif

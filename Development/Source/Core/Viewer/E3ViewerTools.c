/*  NAME:
        E3ViewerTools.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#include "Quesa.h"
#include "QuesaExtension.h"
#include "QuesaCamera.h"
#include "QuesaRenderer.h"
#include "QuesaMath.h"
#include "QuesaLight.h"
#include "QuesaDrawContext.h"
#include "QuesaViewer.h"

#include "E3Viewer.h"
#include "E3ViewerTools.h"

#include <math.h>
#include <string.h>

#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	#include <ToolUtils.h>
	#include <ColorPicker.h>
#endif

// ANSI C does not allow anonymous function parameters.
// However the Metrowerks compiler uses these to identify
// unused parameters, to avoid warnings.  As a compromise,
// we define these macros so we can have it both ways.
#if defined(__MWERKS__)
#define _unused1
#define _unused2
#define _unused3
#define _unused4
#define _unused5
#else
#define _unused1 unused1
#define _unused2 unused2
#define _unused3 unused3
#define _unused4 unused4
#define _unused5 unused5
#endif



//=============================================================================
//      Internal #defines
//-----------------------------------------------------------------------------
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	#ifndef TQ3Rect
		#define TQ3Rect			Rect
	#endif
	#ifndef TQ3Result
		#define TQ3Result		OSErr
	#endif
	#ifndef kQ3GoodResult
		#define kQ3GoodResult	noErr
	#endif
	#ifndef TQ3EventRecord
		#define TQ3EventRecord	EventRecord
	#endif
#else
	#ifndef TQ3Rect
		#if defined(QUESA_OS_WIN32) && QUESA_OS_WIN32
			#define TQ3Rect		RECT
		#else
			#define TQ3Rect		TQ3Area
		#endif
	#endif
	#ifndef TQ3Result
		#define TQ3Result		TQ3Status
	#endif
	#ifndef kQ3GoodResult
		#define kQ3GoodResult	kQ3Success
	#endif
	#ifndef TQ3EventRecord
		#define TQ3EventRecord	void
	#endif
#endif





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kArrowCursor	0L
#define kTruckCursor	-133L
#define kOrbitCursor	-134L
#define kDollyCursor	-135L
#define kZoomCursor		-136L

#define kDefaultIcon	-128L
#define kAboutIcon		-128L
#define kCameraIcon		-132L
#define kTruckIcon		-133L
#define kOrbitIcon		-134L
#define kDollyIcon		-135L
#define kZoomIcon		-136L
#define kResetIcon		-137L
#define kOptionsIcon	-138L

#define kAboutAlertID	-128L
#define kURLItem		 2

#define kCameraMenuID	-128L
#define kOptionsMenuID	-129L
#define kRendererItem	 1
#define kLightsItem		 2
#define kChangeColorItem 3
#define kBackfaceItem	 4
#define kPhongItem		 5
#define	kRendererMenuID	100L
#define kLightsMenuID	101L


#define kGeneralStrings	-129L
#define kGetColorStrID		1L
#define kConfigureStrID		2L

#define kHelpStrings  -128
#define kDragAreaStrID	 1
#define kDrawAreaStrID	 2
#define kCameraStrID	 3
#define kTruckStrID		 4
#define kOrbitStrID		 5
#define kZoomStrID		 6
#define kDollyStrID		 7
#define kResetStrID		 8
#define kOptionsStrID	 9
#define kPluginsStrID	10
#define kAboutStrID		11
#define kDefaultStrID	12

//---- TQ3PluginTypeViewer methods ----
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XPluginViewerParamMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XPluginViewerParamEventMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, void* evt, TQ3Int32 param1, TQ3Int32 param2);
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XPluginViewerParamPointMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Point2D* pt);

#define kQ3XMethodType_ViewerPluginNewViewer			Q3_METHOD_TYPE('v','n','e','w')
// Called when a new viewer is created
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginNewViewerMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginDeleteViewer			Q3_METHOD_TYPE('v','d','e','l')
// Called when a viewer is destroyed
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDeleteViewerMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginDrawTool				Q3_METHOD_TYPE('v','d','r','t')
// Called when the tool needs to be drawn. Bounds is the area to draw your tool into and the port will be correctly set.
// If you return kQ3Failure a generic icon will be drawn.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDrawToolMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Area* bounds);

#define kQ3XMethodType_ViewerPluginClickTool			Q3_METHOD_TYPE('v','c','l','t')
// Called when the tool has been clicked.
// If you return kQ3Failure you will get your double click method called.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginClickToolMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginDoubleClickTool		Q3_METHOD_TYPE('v','d','c','t')
// Called when the tool has been double clicked.
// If you return kQ3Failure you will get your click method called.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoubleClickToolMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginUnclickTool			Q3_METHOD_TYPE('v','u','c','t')
// Called when another tool has been clicked
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginUnclickToolMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginCursorAdjust			Q3_METHOD_TYPE('v','c','a','d')
// Called when the cursor is over the viewer area. MousePt is in local coordinates.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginCursorAdjustMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Point2D* mousePt);

#define kQ3XMethodType_ViewerPluginGetHelpString		Q3_METHOD_TYPE('v','s','h','e')
// Called when the tool needs to display some help (Balloon help on Mac).
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginGetHelpStringMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, Str255 helpString);

#define kQ3XMethodType_ViewerPluginToolOptions			Q3_METHOD_TYPE('v','t','o','p')
// Called when the user wants to display some tool options (Perhaps clicked an options button)
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginToolOptionsMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginActivate				Q3_METHOD_TYPE('v','a','c','t')
// Called when the viewer is activated.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginActivateMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginDeactivate			Q3_METHOD_TYPE('v','d','a','c')
// Called when the viewer is deactivated.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDeactivateMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginGroupChanged			Q3_METHOD_TYPE('v','g','r','c')
// Called when the viewer changes the contents of its group.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginGroupChangedMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3GroupObject group);

#define kQ3XMethodType_ViewerPluginCameraChanged		Q3_METHOD_TYPE('v','c','a','c')
// Called when the viewer changes its camera object.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginCameraChangedMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3CameraObject camera);

#define kQ3XMethodType_ViewerPluginRendererChanged		Q3_METHOD_TYPE('v','r','e','c')
// Called when the viewer changes its renderer object.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginRendererChangedMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3RendererObject renderer);

#define kQ3XMethodType_ViewerPluginLightsChanged		Q3_METHOD_TYPE('v','l','i','c')
// Called when the viewer changes the contents of its lights group.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginLightsChangedMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3GroupObject lightGroup);

#define kQ3XMethodType_ViewerPluginDrawContextChanged	Q3_METHOD_TYPE('v','d','c','c')
// Called when the viewer changes its draw context object.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDrawContextChangedMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3DrawContextObject drawContext);


// --- Platform Specific events ---

#define kQ3XMethodType_ViewerPluginDoToolEvent			Q3_METHOD_TYPE('v','d','o','t')
// Called when the selected tool has been clicked in the main viewer area. Do your stuff.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoToolEventMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, void* evt, TQ3Int32 param1, TQ3Int32 param2);
// On Mac evt is an EventRecord* and param1 and param2 are not used
// On Windows, ???

#define kQ3XMethodType_ViewerPluginDoToolStart			Q3_METHOD_TYPE('v','d','t','s')
// Called when the selected tool has been clicked in the main viewer area. Do your stuff.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoToolMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Int32 x, TQ3Int32 y);

#define kQ3XMethodType_ViewerPluginDoToolTracking		Q3_METHOD_TYPE('v','d','t','t')
// Called when the selected tool is tracking in the main viewer area. Do your stuff.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoToolTrackingMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Int32 x, TQ3Int32 y);

#define kQ3XMethodType_ViewerPluginDoToolEnd			Q3_METHOD_TYPE('v','d','t','e')
// Called when the selected tool is finished in the main viewer area. Do your stuff.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoToolEndMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Int32 x, TQ3Int32 y);

#define kQ3XMethodType_ViewerPluginDoKeyDown			Q3_METHOD_TYPE('v','d','c','d')
// Called when the selected tool has been told that the key is for it. Do your stuff.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoKeyDownMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, void* evt, TQ3Int32 param1, TQ3Int32 param2);
// On Mac evt is an EventRecord* and param1 and param2 are not used
// On Windows, ???

#define kQ3XMethodType_ViewerPluginBeforeEvent			Q3_METHOD_TYPE('v','b','e','v')
// Called when your tool is active and an event has come in.
// If you return kQ3Success the event will not be passed on to the viewer as the plug-in will have dealt with it.
// You can do some processing depending on the event and still return kQ3Failure to allow the event to be passed on.
// Should be called for each registered Viewer plug-in.
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginBeforeEventMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, void* evt, TQ3Int32 param1, TQ3Int32 param2);
// On Mac evt is an EventRecord* and param1 and param2 are not used
// On Windows, ???

#define kQ3XMethodType_ViewerPluginAfterEvent			Q3_METHOD_TYPE('v','a','e','v')
// Called when your tool is active and an event has been processed. Should be called for each YPE('p','a','s','p')
// All registered plug-ins must be called
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginAfterEventMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, void* evt, TQ3Int32 param1, TQ3Int32 param2);

//---- TQ3PluginTypeViewer methods ----

#define	kQ3XMethodType_ViewerPluginDragAcceptable		Q3_METHOD_TYPE('v','d','r','a')
// Called when the viewer is about to accept a drop. Return the acceptable flavor.
typedef Q3_CALLBACK_API_C( TQ3Boolean , TQ3XViewerPluginDragAcceptableMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, DragReference drag, TQ3Uns32 item, TQ3Int32* myFlavor);
// On Mac, drag is a DragReference, item is an ItemReference, myFlavor is an OSType
// On Windows, drag is a Handle, item is unused, myFlavor is unused

#define kQ3XMethodType_ViewerPluginDoDrop				Q3_METHOD_TYPE('v','d','d','r')
// Called when the viewer has dropped your flavor type. Return kQ3Success if you succeeded
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoDropMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, DragReference drag, TQ3Uns32 item, TQ3Int32 yourFlavor);
// On Mac, drag is a DragReference, item is an ItemReference, yourFlavor is an OSType
// On Windows, drag is a Handle, item is unused, yourFlavor is unused

#define kQ3XMethodType_ViewerPluginAddDragFlavors		Q3_METHOD_TYPE('v','a','d','f')
// Called when the viewer is about to start a drag.
// Mac only ??
typedef Q3_CALLBACK_API_C( TQ3Status , TQ3XViewerPluginAddDragFlavorsMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, DragReference drag, TQ3Uns32 item);
// On Mac, drag is a DragReference, item is an ItemReference




//=============================================================================
//      Internal variables
//-----------------------------------------------------------------------------
static TQ3Point2D gStartPt; // this variable can be shared amongst all tools as it is only used during tracking



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// dair, temporary hack - will be cleaned up post d14
TQ3Status	
E3ViewerGetGroupBounds(TQ3ViewerObject theViewer, TQ3BoundingBox* bounds);



//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
/*
TQ3Status E3ViewerPlugins_RegisterClass (void)
	{
	// Register the classes
	TQ3Status qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
											kQ3SharedTypePlugin,
											kQ3ClassNamePlugin,
											BaseToolMetaHandler,
											0);
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypePlugin,
											kQ3PluginTypeDollyTool,
											kQ3ClassDollyToolPlugin,
											DollyToolMetaHandler,
											0);
	}
*/

//=============================================================================
//      Private functions
//-----------------------------------------------------------------------------
#pragma mark { OS dependent methods }


static TQ3Status GetOSHelpString (TQ3Int32 id, Str255 helpString)
	{
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	GetIndString (helpString, kHelpStrings, id);
	return kQ3Success;
#else
	#pragma unused (id, helpString)
	return kQ3Failure;
#endif
	}


static TQ3Status DrawOSTool (TQ3Area* area, TQ3Int32 id)
	{
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	TQ3Rect r;
	r.left = area->min.x;
	r.right = r.left + 32;
	r.top = area->min.y;
	r.bottom = r.top + 32;
	if (PlotIconID (&r, kAlignNone, kTransformNone, id) != noErr)
		return kQ3Failure;
#else
	#pragma unused (id, area)
#endif
	return kQ3Success;
	}


static void SetOSCursor (TQ3Int32 id)
	{
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	if (id)
		{
		CursHandle h = GetCursor (id);
		if (h)
			SetCursor (*h);
		else
			InitCursor ();
		}
	else
		InitCursor ();
#else
	#pragma unused (id)
#endif
	}


static TQ3Int32
OSPopupMenuSelect (TQ3Rect* r, TQ3Int32 id)
	{
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	MenuHandle menu = GetMenu (id);
	if (menu && r)
		{
		TQ3Int32 result;
		Point pt;
		pt.h = r->right;
		pt.v = r->top + 1;
		LocalToGlobal (&pt);
		InsertMenu (menu, -1);
		result = PopUpMenuSelect (menu, pt.v, pt.h, 0);
		#if TARGET_API_MAC_CARBON
			DeleteMenu (GetMenuID(menu));
		#else
			DeleteMenu ((**menu).menuID);
		#endif
		DisposeMenu (menu);
		return result;
		}
	else
		{
		// unable to locate the menu, or no rect given,
		// so we can't do the menu select.
		//SysBeep (30);
		}
#else
	#pragma unused (id, r)
#endif
	return 0;
	}


static void
OSGetDelta (float* x, float*y, TQ3EventRecord* evt, TQ3Int32 param1, TQ3Int32 param2)
	{
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	#pragma unused (param1, param2)
	//const float kSlowSpeed = 1.0f;
	const float kMediumSpeed = 2.5f;
	//const float kFastSpeed = 5.0f;
	if (x)
		*x = 0;
	if (y)
		*y = 0;

	if (evt && ((evt->what == keyDown) || (evt->what == autoKey)))
		{
		char c = evt->message & charCodeMask;
		if (c == kLeftArrowCharCode)
			{
				if (x)
					*x = (1.0f * kMediumSpeed);
			}
		else if (c == kRightArrowCharCode)
			{
				if (x)
					*x = (-1.0f * kMediumSpeed);
			}
		else if (c == kUpArrowCharCode)
			{
				if (y)
					*y = (1.0f * kMediumSpeed);
			}
		else if (c == kDownArrowCharCode)
			{
				if (y)
					*y = (-1.0f * kMediumSpeed);
			}
		}
#else
#pragma unused (evt, param1, param2)
	if (x)
		*x = 0;
	if (y)
		*y = 0;
#endif
	}


#pragma mark -
#pragma mark { Default methods }


static TQ3Status NewTool (TQ3Object _unused1, void* _unused2, const void* _unused3)
	{
	return kQ3Success;
	}


static TQ3Status DeleteTool (TQ3Object _unused1, void* _unused2)
	{
	return kQ3Success;
	}


static TQ3Status DrawTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Area* area)
	{
	return DrawOSTool (area, kDefaultIcon);
	}


static TQ3Status DoToolStart (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Point2D* pt)
	{
	gStartPt = *pt;
	return kQ3Success;
	}


static TQ3Status DoToolTracking (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Point2D* _unused3) // all tools should override this method
	{
	return kQ3Failure; // stops tracking
	}


static TQ3Status DoToolEnd (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Point2D* _unused3)
	{
	return kQ3Success;
	}


static TQ3Status ToolAdjustCursor (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Point2D* _unused3)
	{
	SetOSCursor (kArrowCursor);
	return kQ3Success;
	}


static TQ3Status UnclickTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2)
	{
	return kQ3Success;
	}


static TQ3Status ClickTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2)
	{
	return kQ3Success;
	}


static TQ3Status DoubleClickTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2)
	{
	return kQ3Failure; // do not change this as tools that popup menus rely on this to return failure
	}


static TQ3Status DoToolEvent (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3EventRecord* _unused3, TQ3Int32 _unused4, TQ3Int32 _unused5)
	{
	return kQ3Success; // allow the event to pass on
	}


static TQ3Status DoToolKeyDown (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3EventRecord* _unused3, TQ3Int32 _unused4, TQ3Int32 _unused5)
	{
	return kQ3Failure; // did not handle it
	}


static TQ3Status GetHelpString (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, Str255 helpString)
	{
	return GetOSHelpString (kDefaultStrID, helpString);
	}


TQ3XFunctionPointer BaseToolMetaHandler (TQ3XMethodType methodType)
	{
	switch (methodType)
		{
		case kQ3XMethodTypeObjectNew:
			return (TQ3XFunctionPointer)NewTool;
			
		case kQ3XMethodTypeObjectDelete:
			return (TQ3XFunctionPointer)DeleteTool;
		
		case kQ3XMethodType_ViewerPluginDrawTool:
			return (TQ3XFunctionPointer)DrawTool;
		
		case kQ3XMethodType_ViewerPluginUnclickTool:
			return (TQ3XFunctionPointer)UnclickTool;
			
		case kQ3XMethodType_ViewerPluginClickTool:
			return (TQ3XFunctionPointer)ClickTool;

		case kQ3XMethodType_ViewerPluginDoubleClickTool:
			return (TQ3XFunctionPointer)DoubleClickTool;
			
		case kQ3XMethodType_ViewerPluginDoToolEvent:
			return (TQ3XFunctionPointer)DoToolEvent;

		case kQ3XMethodType_ViewerPluginDoKeyDown:
			return (TQ3XFunctionPointer)DoToolKeyDown;

		case kQ3XMethodType_ViewerPluginDoToolStart:
			return (TQ3XFunctionPointer)DoToolStart;

		case kQ3XMethodType_ViewerPluginDoToolTracking:
			return (TQ3XFunctionPointer)DoToolTracking;
			
		case kQ3XMethodType_ViewerPluginDoToolEnd:
			return (TQ3XFunctionPointer)DoToolEnd;
			
		case kQ3XMethodType_ViewerPluginCursorAdjust:
			return (TQ3XFunctionPointer)ToolAdjustCursor;
		
		case kQ3XMethodType_ViewerPluginGetHelpString:
			return (TQ3XFunctionPointer)GetHelpString;
		}
	return NULL;
	}


#pragma mark -
#pragma mark { Camera Tool }


static TQ3Status CameraClickTool (TQ3ViewerObject theViewer, TQ3SharedObject _unused1)
	{
	TQ3Rect r;
	if (Q3ViewerGetButtonRect (theViewer, Q3ViewerGetCurrentButton (theViewer), &r) == kQ3GoodResult)
		{
		TQ3Int16 menuItem = OSPopupMenuSelect (&r, kCameraMenuID) & 0x0000FFFF;
		if (menuItem)
			Q3ViewerSetCameraByView (theViewer, (TQ3ViewerCameraView)(menuItem - 1)); // cos menu items are 1 based but TQ3ViewerCameraView is zero based
		}
	return kQ3Failure; // so does not click me
	}


static TQ3Status CameraDrawTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Area* area)
	{
	return DrawOSTool (area, kCameraIcon);
	}


static TQ3Status CameraGetHelpString (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, Str255 helpString)
	{
	return GetOSHelpString (kCameraStrID, helpString);
	}


TQ3XFunctionPointer CameraToolMetaHandler (TQ3XMethodType methodType)
	{
	switch (methodType)
		{
		case kQ3XMethodType_ViewerPluginDrawTool:
			return (TQ3XFunctionPointer)CameraDrawTool;
		
		case kQ3XMethodType_ViewerPluginClickTool:
			return (TQ3XFunctionPointer)CameraClickTool;
		
		case kQ3XMethodType_ViewerPluginGetHelpString:
			return (TQ3XFunctionPointer)CameraGetHelpString;
		}
	return BaseToolMetaHandler (methodType);
	}


#pragma mark -
#pragma mark { Dolly Tool }


static TQ3Status DoDollyToolMove (TQ3ViewerObject theViewer, TQ3Point2D* delta)
	{
	if (delta->x || delta->y)
		{
		TQ3ViewObject theView = Q3ViewerGetView (theViewer);
		if (theView)
			{
			TQ3CameraObject theCamera;
			if ((Q3View_GetCamera (theView, &theCamera) == kQ3Success) && theCamera)
				{
				TQ3CameraData theData;
				if (Q3Camera_GetData (theCamera, &theData) == kQ3Success)
					{
					TQ3BoundingBox bounds;
					if (E3ViewerGetGroupBounds (theViewer, &bounds) == kQ3Success)
						{
						TQ3Vector3D v;
						TQ3Matrix4x4 mat;
						v.x = delta->x * (bounds.max.x - bounds.min.x) * 0.01f;
						v.y = -delta->y * (bounds.max.y - bounds.min.y) * 0.01f;
						v.z = 0.0f;
						if (Q3Camera_GetWorldToView (theCamera, &mat) == kQ3Failure)
							Q3Matrix4x4_SetIdentity (&mat);
						Q3Matrix4x4_Invert (&mat, &mat);
						Q3Vector3D_Transform (&v, &mat, &v);
						theData.placement.cameraLocation.x += v.x;
						theData.placement.cameraLocation.y += v.y;
						theData.placement.cameraLocation.z += v.z;
						theData.placement.pointOfInterest.x += v.x;
						theData.placement.pointOfInterest.y += v.y;
						theData.placement.pointOfInterest.y += v.z;
						if (Q3Camera_SetData (theCamera, &theData) == kQ3Success)
							{
							Q3ViewerDrawContent (theViewer);
							Q3Object_Dispose (theCamera);
							return kQ3Success;
							}
						}
					}
				Q3Object_Dispose (theCamera);
				}
			}
		return kQ3Failure;
		}
	return kQ3Success; // continue tracking
	}


static TQ3Status DoDollyToolKeyDown (TQ3ViewerObject theViewer, TQ3SharedObject _unused1, TQ3EventRecord* evt, TQ3Int32 param1, TQ3Int32 param2)
	{
	TQ3Point2D delta;
	OSGetDelta (&delta.x, &delta.y, evt, param1, param2);
	if ((0 == delta.x) && (0 == delta.y))
		return kQ3Failure;
	else
		return DoDollyToolMove (theViewer, &delta);
	}


static TQ3Status DoDollyToolTracking (TQ3ViewerObject theViewer, TQ3SharedObject _unused1, TQ3Point2D* pt)
	{
	TQ3Point2D delta;
	delta.x = gStartPt.x - pt->x;
	delta.y = gStartPt.y - pt->y;
	if (DoDollyToolMove (theViewer, &delta) == kQ3Success)
		{
		gStartPt = *pt;
		return kQ3Success;
		}
	return kQ3Failure;
	}


static TQ3Status DollyToolAdjustCursor (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Point2D* _unused3)
	{
	SetOSCursor (kDollyCursor);
	return kQ3Success;
	}


static TQ3Status DollyDrawTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Area* area)
	{
	return DrawOSTool (area, kDollyIcon);
	}


static TQ3Status DollyGetHelpString (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, Str255 helpString)
	{
	return GetOSHelpString (kDollyStrID, helpString);
	}


TQ3XFunctionPointer DollyToolMetaHandler (TQ3XMethodType methodType)
	{
	switch (methodType)
		{
		case kQ3XMethodType_ViewerPluginDrawTool:
			return (TQ3XFunctionPointer)DollyDrawTool;

		case kQ3XMethodType_ViewerPluginDoKeyDown:
			return (TQ3XFunctionPointer)DoDollyToolKeyDown;

		case kQ3XMethodType_ViewerPluginDoToolTracking:
			return (TQ3XFunctionPointer)DoDollyToolTracking;
			
		case kQ3XMethodType_ViewerPluginCursorAdjust:
			return (TQ3XFunctionPointer)DollyToolAdjustCursor;
			
		case kQ3XMethodType_ViewerPluginGetHelpString:
			return (TQ3XFunctionPointer)DollyGetHelpString;
		}
	return BaseToolMetaHandler (methodType);
	}


#pragma mark -
#pragma mark { Truck Tool }


static TQ3Status DoTruckToolMove (TQ3ViewerObject theViewer, float delta)
	{
	if (delta)
		{
		TQ3ViewObject theView = Q3ViewerGetView (theViewer);
		if (theView)
			{
			TQ3CameraObject theCamera;
			if ((Q3View_GetCamera (theView, &theCamera) == kQ3Success) && theCamera)
				{
				TQ3CameraData theData;
				if (Q3Camera_GetData (theCamera, &theData) == kQ3Success)
					{
					TQ3BoundingBox bounds;
					if (E3ViewerGetGroupBounds (theViewer, &bounds) == kQ3Success)
						{
						TQ3Vector3D v;
						TQ3Matrix4x4 mat;
						v.x = 0.0f;
						v.y = 0.0f;
						v.z = delta * (bounds.max.y - bounds.min.y) * 0.01f;
						if (Q3Camera_GetWorldToView (theCamera, &mat) == kQ3Failure)
							Q3Matrix4x4_SetIdentity (&mat);
						Q3Matrix4x4_Invert (&mat, &mat);
						Q3Vector3D_Transform (&v, &mat, &v);
						theData.placement.cameraLocation.x += v.x;
						theData.placement.cameraLocation.y += v.y;
						theData.placement.cameraLocation.z += v.z;
						theData.placement.pointOfInterest.x += v.x;
						theData.placement.pointOfInterest.y += v.y;
						theData.placement.pointOfInterest.z += v.z;
						if (Q3Camera_SetData (theCamera, &theData) == kQ3Success)
							{
							Q3ViewerDrawContent (theViewer);
							Q3Object_Dispose (theCamera);
							return kQ3Success;
							}
						}
					}
				Q3Object_Dispose (theCamera);
				}
			}
		return kQ3Failure;
		}
	return kQ3Success; // continue tracking
	}


static TQ3Status DoTruckToolKeyDown (TQ3ViewerObject theViewer, TQ3SharedObject _unused1, TQ3EventRecord* evt, TQ3Int32 param1, TQ3Int32 param2)
	{
	float delta;
	OSGetDelta (NULL, &delta, evt, param1, param2);
	if (0 == delta)
		return kQ3Failure;
	else
	return DoTruckToolMove (theViewer, delta);
	}


static TQ3Status DoTruckToolTracking (TQ3ViewerObject theViewer, TQ3SharedObject _unused1, TQ3Point2D* pt)
	{
	float delta = gStartPt.y - pt->y;
	if (DoTruckToolMove (theViewer, delta) == kQ3Success)
		{
		gStartPt = *pt;
		return kQ3Success;
		}
	return kQ3Failure;
	}


static TQ3Status TruckToolAdjustCursor (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Point2D* _unused3)
	{
	SetOSCursor (kTruckCursor);
	return kQ3Success;
	}


static TQ3Status TruckDrawTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Area* area)
	{
	return DrawOSTool (area, kTruckIcon);
	}


static TQ3Status TruckGetHelpString (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, Str255 helpString)
	{
	return GetOSHelpString (kTruckStrID, helpString);
	}


TQ3XFunctionPointer TruckToolMetaHandler (TQ3XMethodType methodType)
	{
	switch (methodType)
		{
		case kQ3XMethodType_ViewerPluginDrawTool:
			return (TQ3XFunctionPointer)TruckDrawTool;

		case kQ3XMethodType_ViewerPluginDoKeyDown:
			return (TQ3XFunctionPointer)DoTruckToolKeyDown;

		case kQ3XMethodType_ViewerPluginDoToolTracking:
			return (TQ3XFunctionPointer)DoTruckToolTracking;
			
		case kQ3XMethodType_ViewerPluginCursorAdjust:
			return (TQ3XFunctionPointer)TruckToolAdjustCursor;
			
		case kQ3XMethodType_ViewerPluginGetHelpString:
			return (TQ3XFunctionPointer)TruckGetHelpString;
		}
	return BaseToolMetaHandler (methodType);
	}


#pragma mark -
#pragma mark { Orbit Tool }



//=============================================================================
//      DoOrbitToolMove : Orbit the camera around its point of interest.
//-----------------------------------------------------------------------------
static TQ3Status
DoOrbitToolMove(TQ3ViewerObject theViewer, TQ3Point2D *delta)
{	float					cosAngle, sinAngle, cosTheta, sinTheta;
	float					viewDist, longAngle, latAngle;
	TQ3Vector3D				viewDir, viewFront, viewLeft;
	TQ3CameraObject			theCamera;
	TQ3Status				qd3dStatus;
	TQ3ViewObject			theView;
	TQ3CameraPlacement		camPos;



	// If we don't have anything to do, just keep tracking
	if (delta->x == 0.0f && delta->y == 0.0f)
		return(kQ3Success);



	// Get the camera and its placement		
	theView = Q3ViewerGetView(theViewer);
	if (theView == NULL)
		return(kQ3Failure);
	
	qd3dStatus = Q3View_GetCamera(theView, &theCamera);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);
	
	Q3Camera_GetPlacement(theCamera, &camPos);



	// Initialise ourselves
	Q3Point3D_Subtract(&camPos.cameraLocation, &camPos.pointOfInterest, &viewDir);
	viewDist = Q3Vector3D_Length(&viewDir);

	Q3Vector3D_Scale(&viewDir,         (1.0f / viewDist), &viewDir);
	Q3Vector3D_Cross(&camPos.upVector, &viewDir,          &viewLeft);
	Q3Vector3D_Cross(&viewLeft,        &camPos.upVector,  &viewFront);

    longAngle =  Q3Math_DegreesToRadians(delta->x);
    latAngle  = -Q3Math_DegreesToRadians(delta->y);



	// Orbit the camera horizontally
	if (longAngle != 0.0f)
		{
		cosAngle = (float) cos(longAngle);
		sinAngle = (float) sin(longAngle);
		cosTheta = Q3Vector3D_Dot(&viewDir, &camPos.upVector);
		sinTheta = Q3Vector3D_Dot(&viewDir, &viewFront);

		viewFront.x = (viewFront.x * cosAngle) + (viewLeft.x * sinAngle);
		viewFront.y = (viewFront.y * cosAngle) + (viewLeft.y * sinAngle);
		viewFront.z = (viewFront.z * cosAngle) + (viewLeft.z * sinAngle);

		viewDir.x = (viewFront.x * sinTheta) + (camPos.upVector.x * cosTheta);
		viewDir.y = (viewFront.y * sinTheta) + (camPos.upVector.y * cosTheta);
		viewDir.z = (viewFront.z * sinTheta) + (camPos.upVector.z * cosTheta);

		if (latAngle != 0.0f)
			Q3Vector3D_Cross(&camPos.upVector, &viewFront, &viewLeft);
		}



	// Orbit the camera vertically
	if (latAngle != 0.0f)
		{
		cosAngle = (float) cos(latAngle);
		sinAngle = (float) sin(latAngle);
		Q3Vector3D_Cross(&viewDir, &viewLeft, &camPos.upVector);

		viewDir.x = (viewDir.x * cosAngle) + (camPos.upVector.x * sinAngle);
		viewDir.y = (viewDir.y * cosAngle) + (camPos.upVector.y * sinAngle);
		viewDir.z = (viewDir.z * cosAngle) + (camPos.upVector.z * sinAngle);
		}



	// Recalculate the camera details
	camPos.cameraLocation.x = (camPos.pointOfInterest.x * 1.0f) + (viewDir.x * viewDist);
	camPos.cameraLocation.y = (camPos.pointOfInterest.y * 1.0f) + (viewDir.y * viewDist);
	camPos.cameraLocation.z = (camPos.pointOfInterest.z * 1.0f) + (viewDir.z * viewDist);

	Q3Vector3D_Cross(&viewDir,             &viewLeft, &camPos.upVector);
	Q3Vector3D_Normalize(&camPos.upVector, &camPos.upVector);



	// Update the camera and redraw
	Q3Camera_SetPlacement(theCamera, &camPos);
	Q3Object_Dispose(theCamera);

	Q3ViewerDrawContent(theViewer);
	return kQ3Success;
}


static TQ3Status DoOrbitToolKeyDown (TQ3ViewerObject theViewer, TQ3SharedObject _unused1, TQ3EventRecord* evt, TQ3Int32 param1, TQ3Int32 param2)
	{
	TQ3Point2D delta;
	OSGetDelta (&delta.x, &delta.y, evt, param1, param2);
	if ((0 == delta.x) && (0 == delta.y))
		return kQ3Failure;
	else
	return DoOrbitToolMove (theViewer, &delta);
	}


static TQ3Status DoOrbitToolTracking (TQ3ViewerObject theViewer, TQ3SharedObject _unused1, TQ3Point2D* pt)
	{
	TQ3Point2D delta;
	delta.x = gStartPt.x - pt->x;
	delta.y = gStartPt.y - pt->y;
	if (DoOrbitToolMove (theViewer, &delta) == kQ3Success)
		{
		gStartPt = *pt;
		return kQ3Success;
		}
	return kQ3Failure;
	}


static TQ3Status OrbitToolAdjustCursor (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Point2D* _unused3)
	{
	SetOSCursor (kOrbitCursor);
	return kQ3Success;
	}


static TQ3Status OrbitDrawTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Area* area)
	{
	return DrawOSTool (area, kOrbitIcon);
	}


static TQ3Status OrbitGetHelpString (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, Str255 helpString)
	{
	return GetOSHelpString (kOrbitStrID, helpString);
	}


TQ3XFunctionPointer OrbitToolMetaHandler (TQ3XMethodType methodType)
	{
	switch (methodType)
		{
		case kQ3XMethodType_ViewerPluginDrawTool:
			return (TQ3XFunctionPointer)OrbitDrawTool;

		case kQ3XMethodType_ViewerPluginDoKeyDown:
			return (TQ3XFunctionPointer)DoOrbitToolKeyDown;

		case kQ3XMethodType_ViewerPluginDoToolTracking:
			return (TQ3XFunctionPointer)DoOrbitToolTracking;
			
		case kQ3XMethodType_ViewerPluginCursorAdjust:
			return (TQ3XFunctionPointer)OrbitToolAdjustCursor;
		
		case kQ3XMethodType_ViewerPluginGetHelpString:
			return (TQ3XFunctionPointer)OrbitGetHelpString;
		}
	return BaseToolMetaHandler (methodType);
	}


#pragma mark -
#pragma mark { Zoom Tool }


static TQ3Status DoZoomToolMove (TQ3ViewerObject theViewer, float delta)
	{
	TQ3Status err = kQ3Success; // continue tracking
	if (delta)
		{
		TQ3ViewObject theView = Q3ViewerGetView (theViewer);
		if (theView)
			{
			TQ3CameraObject theCamera;
			err = Q3View_GetCamera (theView, &theCamera);
			if ((err == kQ3Success) && theCamera)
				{
				TQ3ObjectType theType = Q3Camera_GetType (theCamera);
				if (theType == kQ3CameraTypeViewAngleAspect)
					{
					float FOV;
					err = Q3ViewAngleAspectCamera_GetFOV (theCamera, &FOV);
					if (err == kQ3Success)
						{
						const float kQ3MinAngle = 0.01f;
						const float kQ3MaxAngle = 3.0f;
						FOV += delta * 0.001f;
						if (FOV < kQ3MinAngle)
							FOV = kQ3MinAngle;
						else
						if (FOV > kQ3MaxAngle)
							FOV = kQ3MaxAngle;
						err = Q3ViewAngleAspectCamera_SetFOV (theCamera, FOV);
						}
					}
				else
				if (theType == kQ3CameraTypeOrthographic)
					{
					//Q3OrthographicCamera_SetLeft ???
					//Q3OrthographicCamera_SetRight
					//Q3OrthographicCamera_SetTop
					//Q3OrthographicCamera_SetBottom
					err = kQ3Failure;
					}
				else
				if (theType == kQ3CameraTypeViewPlane)
					{
					//Q3ViewPlaneCamera_SetHalfWidth ???
					//Q3ViewPlaneCamera_SetHalfHeight
					err = kQ3Failure;
					}
				else
					err = kQ3Failure;
				if (err == kQ3Success)
					Q3ViewerDrawContent (theViewer);
				Q3Object_Dispose (theCamera);
				}
			}
		else
			err = kQ3Failure;
		}
	return err;
	}


static TQ3Status DoZoomToolKeyDown (TQ3ViewerObject theViewer, TQ3SharedObject _unused1, TQ3EventRecord* evt, TQ3Int32 param1, TQ3Int32 param2)
	{
	float delta;
	OSGetDelta (NULL, &delta, evt, param1, param2);
	if (0 == delta)
		return kQ3Failure;
	else
	return DoZoomToolMove (theViewer, delta);
	}


static TQ3Status DoZoomToolTracking (TQ3ViewerObject theViewer, TQ3SharedObject _unused1, TQ3Point2D* pt)
	{
	float delta = gStartPt.y - pt->y;
	if (DoZoomToolMove (theViewer, delta) == kQ3Success)
		{
		gStartPt = *pt;
		return kQ3Success;
		}
	return kQ3Failure;
	}


static TQ3Status ZoomToolAdjustCursor (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Point2D* _unused3)
	{
	SetOSCursor (kZoomCursor);
	return kQ3Success;
	}


static TQ3Status ZoomDrawTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Area* area)
	{
	return DrawOSTool (area, kZoomIcon);
	}


static TQ3Status ZoomGetHelpString (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, Str255 helpString)
	{
	return GetOSHelpString (kZoomStrID, helpString);
	}


TQ3XFunctionPointer ZoomToolMetaHandler (TQ3XMethodType methodType)
	{
	switch (methodType)
		{
		case kQ3XMethodType_ViewerPluginDrawTool:
			return (TQ3XFunctionPointer)ZoomDrawTool;

		case kQ3XMethodType_ViewerPluginDoKeyDown:
			return (TQ3XFunctionPointer)DoZoomToolKeyDown;

		case kQ3XMethodType_ViewerPluginDoToolTracking:
			return (TQ3XFunctionPointer)DoZoomToolTracking;
			
		case kQ3XMethodType_ViewerPluginCursorAdjust:
			return (TQ3XFunctionPointer)ZoomToolAdjustCursor;
			
		case kQ3XMethodType_ViewerPluginGetHelpString:
			return (TQ3XFunctionPointer)ZoomGetHelpString;
		}
	return BaseToolMetaHandler (methodType);
	}


#pragma mark -
#pragma mark { Reset Tool }


static TQ3Status ResetClickTool (TQ3ViewerObject theViewer, TQ3SharedObject _unused1)
	{
	Q3ViewerSetCameraByView (theViewer, kQ3ViewerCameraRestore);
	return kQ3Failure; // so does not click me
	}


static TQ3Status ResetDrawTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Area* area)
	{
	return DrawOSTool (area, kResetIcon);
	}


static TQ3Status ResetGetHelpString (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, Str255 helpString)
	{
	return GetOSHelpString (kResetStrID, helpString);
	}


TQ3XFunctionPointer ResetToolMetaHandler (TQ3XMethodType methodType)
	{
	switch (methodType)
		{
		case kQ3XMethodType_ViewerPluginDrawTool:
			return (TQ3XFunctionPointer)ResetDrawTool;

		case kQ3XMethodType_ViewerPluginClickTool:
			return (TQ3XFunctionPointer)ResetClickTool;
			
		case kQ3XMethodType_ViewerPluginGetHelpString:
			return (TQ3XFunctionPointer)ResetGetHelpString;
		}
	return BaseToolMetaHandler (methodType);
	}


#pragma mark -
#pragma mark { Options Tool }


#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH

static TQ3Status OptionsClickTool (TQ3ViewerObject theViewer, TQ3SharedObject)
	{
	TQ3Rect r;
	MenuHandle theMainMenu = GetMenu (kOptionsMenuID);
	if (!theMainMenu)
		return kQ3Failure;
	if (Q3ViewerGetButtonRect (theViewer, Q3ViewerGetCurrentButton (theViewer), &r) == kQ3GoodResult)
		{
		#define kMaxRenderers 20
		TQ3Status err;
		TQ3ObjectClassNameString rendererClassString;
		TQ3ObjectType rendererClassType = kQ3ObjectTypeInvalid;
		TQ3Result result = Q3ViewerGetRendererType (theViewer, &rendererClassType);
		TQ3RendererObject theRenderer = (result == kQ3GoodResult) ? Q3Renderer_NewFromType (rendererClassType) : NULL;
		
		TQ3ObjectType rendererTypes [kMaxRenderers];
		Str255 name;
		TQ3Int32 menuResult;
		TQ3Boolean isOn;
		Boolean hasConfigure = false;
		MenuHandle theRendererMenu = GetMenu (kRendererMenuID);
		MenuHandle theLightsMenu = GetMenu (kLightsMenuID);
		if (theRendererMenu)
			{
			TQ3SubClassData subClassData;
			InsertMenu (theRendererMenu, -1);
			if (theRenderer)
				hasConfigure = Q3Renderer_HasModalConfigure (theRenderer) == kQ3True;
			err = Q3ObjectHierarchy_GetSubClassData (kQ3SharedTypeRenderer, &subClassData);
			if (err == kQ3Success)
				{
				TQ3Uns32 rendererCount = 0;
				TQ3Uns32 i;
				for (i = 0; i < subClassData.numClasses; ++i)
					{
					TQ3ObjectType theType = subClassData.classTypes [i];

					if (theType != kQ3RendererTypeGeneric)
						{
						err = Q3RendererClass_GetNickNameString (theType, rendererClassString);
						if ( err == kQ3Failure)
							err = Q3ObjectHierarchy_GetStringFromType (theType, rendererClassString);
						if ((err == kQ3Success) && (rendererCount < kMaxRenderers))
							{
							TQ3Int32 j = 0;
							while (rendererClassString [j] && (j < 255))
								{
								name [j + 1] = rendererClassString [j];
								++j;
								}
							name [0] = j;
							InsertMenuItem (theRendererMenu, name, CountMenuItems (theRendererMenu));
							if (rendererClassType == theType)
								SetItemMark (theRendererMenu, CountMenuItems (theRendererMenu), checkMark);
							rendererTypes [rendererCount] = theType;
							++rendererCount;
							}
						}
					}
				Q3ObjectHierarchy_EmptySubClassData (&subClassData);
				}
			
			if (hasConfigure)
				{
				InsertMenuItem (theRendererMenu, "\p-", CountMenuItems (theRendererMenu));
				GetIndString (name, kGeneralStrings, kConfigureStrID);
				InsertMenuItem (theRendererMenu, name, CountMenuItems (theRendererMenu));
				}
			}
		if (theLightsMenu)
			{
			TQ3ViewObject theView = Q3ViewerGetView (theViewer);
			InsertMenu (theLightsMenu, -1);
			if (theView)
				{
				TQ3GroupObject lights;
				if (Q3View_GetLightGroup (theView, &lights) == kQ3Success)
					{
					TQ3GroupPosition pos;
					if (Q3Group_GetFirstPosition (lights, &pos) == kQ3Success)
						{
						TQ3LightObject object;
						if (Q3Group_GetPositionObject (lights, pos, &object) == kQ3Success)
							{
							float brightness;
							if (Q3Light_GetBrightness (object, &brightness) == kQ3Success)
								{
								TQ3Int32 item = brightness * 10.0f;
								if (item <= 0)
									item = 1;
								else
								if (item > 10)
									item = 10;
								SetItemMark (theLightsMenu, item, checkMark);
								}
							Q3Object_Dispose (object);
							}
						}
					Q3Object_Dispose (lights);
					}
				}
			}
		// tick items
		if (Q3ViewerGetRemoveBackfaces (theViewer, &isOn) == kQ3GoodResult)
			{
			if (isOn == kQ3True)
				SetItemMark (theMainMenu, kBackfaceItem, checkMark);
			}
		if (Q3ViewerGetPhongShading (theViewer, &isOn) == kQ3GoodResult)
			{
			if (isOn == kQ3True)
				SetItemMark (theMainMenu, kPhongItem, checkMark);
			}
	
		menuResult = OSPopupMenuSelect (&r, kOptionsMenuID);
		
		if (menuResult)
			{
			TQ3Int16 menuItem = menuResult & 0x0000FFFF;
			TQ3Int16 menu = menuResult >> 16;
			if (menu == kRendererMenuID)
				{
				if (hasConfigure && (menuItem == CountMenuItems (theRendererMenu))) // last menu item
					{
					TQ3DialogAnchor dialogAnchor = {NULL};
					TQ3Boolean canceled;
					Q3Renderer_ModalConfigure (theRenderer, dialogAnchor, &canceled);
					}
				else
					Q3ViewerSetRendererType(theViewer, rendererTypes[menuItem - 1]);
				}
			else
			if (menu == kLightsMenuID)
				Q3ViewerChangeBrightness (theViewer, (float)(menuItem) * 0.1);
			else // normal menu
				{
				if (menuItem == kChangeColorItem) // change color
					{
					TQ3ViewObject theView = Q3ViewerGetView (theViewer);
					if (theView)
						{
						TQ3DrawContextObject theDrawContext;
						err = Q3View_GetDrawContext (theView, &theDrawContext);
						if ((err == kQ3Success) && theDrawContext)
							{
							Point where = {0, 0}; // centers the dialog
							TQ3ColorARGB color;
							RGBColor inColor = {65535, 65535, 65535};
							RGBColor outColor;
							if (Q3DrawContext_GetClearImageColor (theDrawContext, &color) == kQ3Success)
								{
								inColor.red = color.r * 65535L;
								inColor.green = color.g * 65535L;
								inColor.blue = color.b * 65535L;
								}
							GetIndString (name, kGeneralStrings, kGetColorStrID);
							if (GetColor (where, name, &inColor, &outColor))
								{
								color.r = (float)outColor.red / 65535.0f;
								color.g = (float)outColor.green / 65535.0f;
								color.b = (float)outColor.blue / 65535.0f;
								Q3DrawContext_SetClearImageColor (theDrawContext, &color);
								}
							Q3Object_Dispose (theDrawContext);
							}
						}
					}
				else
				if (menuItem == kBackfaceItem)
					{
					if (Q3ViewerGetRemoveBackfaces (theViewer, &isOn) == kQ3GoodResult)
						{
						if (isOn == kQ3True)
							isOn = kQ3False;
						else
							isOn = kQ3True;
						Q3ViewerSetRemoveBackfaces (theViewer, isOn);
						}
					}
				else
				if (menuItem == kPhongItem)
					{
					if (Q3ViewerGetPhongShading (theViewer, &isOn) == kQ3GoodResult)
						{
						if (isOn == kQ3True)
							isOn = kQ3False;
						else
							isOn = kQ3True;
						Q3ViewerSetPhongShading (theViewer, isOn);
						}
					}
				}
			}
		if (theRendererMenu)
			{
			#if TARGET_API_MAC_CARBON
				DeleteMenu (GetMenuID(theRendererMenu));
			#else
				DeleteMenu ((**theRendererMenu).menuID);
			#endif
			DisposeMenu (theRendererMenu);
			}
		if (theLightsMenu)
			{
			#if TARGET_API_MAC_CARBON
				DeleteMenu (GetMenuID(theLightsMenu));
			#else
				DeleteMenu ((**theLightsMenu).menuID);
			#endif
			DisposeMenu (theLightsMenu);
			}
		if (theRenderer)
			Q3Object_Dispose (theRenderer);
		}
	return kQ3Failure; // so does not click me
	}

#else


static TQ3Status OptionsClickTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2)
	{
	return kQ3Failure; // so does not click me
	}

#endif


static TQ3Status OptionsDrawTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Area* area)
	{
	return DrawOSTool (area, kOptionsIcon);
	}


static TQ3Status OptionsGetHelpString (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, Str255 helpString)
	{
	return GetOSHelpString (kOptionsStrID, helpString);
	}


TQ3XFunctionPointer OptionsToolMetaHandler (TQ3XMethodType methodType)
	{
	switch (methodType)
		{
		case kQ3XMethodType_ViewerPluginDrawTool:
			return (TQ3XFunctionPointer)OptionsDrawTool;

		case kQ3XMethodType_ViewerPluginClickTool:
			return (TQ3XFunctionPointer)OptionsClickTool;
			
		case kQ3XMethodType_ViewerPluginGetHelpString:
			return (TQ3XFunctionPointer)OptionsGetHelpString;
		}
	return BaseToolMetaHandler (methodType);
	}


#pragma mark -
#pragma mark { About Tool }


static TQ3Status AboutClickTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2)
	{
/* JJS: I'm not sure we're going to support this tool anyway.
	char url[] = "http://www.quesa.org/";
#if defined(QUESA_OS_MACINTOSH) && QUESA_OS_MACINTOSH
	if (Alert (kAboutAlertID, NULL) == kURLItem)
		{
		ICInstance inst = nil;
		OSStatus err = ICStart (&inst, '????');
		if (!err)
			{
			// If we're not on Carbon, find the config file
			#if !TARGET_API_MAC_CARBON
				err = ICFindConfigFile (inst, 0, nil);
			#endif

			// Launch the URL
			if (!err)
				{
				TQ3Int32 selStart, selEnd;
				selStart = selEnd = 0;
				err = ICLaunchURL (inst, "\p", url, strlen(url), &selStart, &selEnd);
				}
			ICStop (inst);
			}
		if ( err )
			{
			// put up alert showing web page address
			}
		}
#endif
#if WINVER
	if (ShellExecute (NULL, "open", "IEXPLORE", url, "", SW_SHOWNORMAL) <= (HINSTANCE)(32))
		{
		// failed
		}
#endif
*/
	return kQ3Failure; // so does not click me
	}


static TQ3Status AboutDrawTool (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, TQ3Area* area)
	{
	return DrawOSTool (area, kAboutIcon);
	}


static TQ3Status AboutGetHelpString (TQ3ViewerObject _unused1, TQ3SharedObject _unused2, Str255 helpString)
	{
	return GetOSHelpString (kAboutStrID, helpString);
	}


TQ3XFunctionPointer AboutToolMetaHandler (TQ3XMethodType methodType)
	{
	switch (methodType)
		{
		case kQ3XMethodType_ViewerPluginDrawTool:
			return (TQ3XFunctionPointer)AboutDrawTool;

		case kQ3XMethodType_ViewerPluginClickTool:
			return (TQ3XFunctionPointer)AboutClickTool;
			
		case kQ3XMethodType_ViewerPluginGetHelpString:
			return (TQ3XFunctionPointer)AboutGetHelpString;
		}
	return BaseToolMetaHandler (methodType);
	}

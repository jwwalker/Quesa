/*  NAME:
        E3Viewer.h

    DESCRIPTION:
        Header file for E3Viewer.c.

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
#ifndef E3VIEWER_HDR
#define E3VIEWER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Quesa
#include "Quesa.h"
#include "QuesaCamera.h"
#include "QuesaController.h"
#include "QuesaCustomElements.h"
#include "QuesaDrawContext.h"
#include "QuesaErrors.h"
#include "QuesaExtension.h"
#include "QuesaGeometry.h"
#include "QuesaGroup.h"
#include "QuesaIO.h"
#include "QuesaLight.h"
#include "QuesaMath.h"
#include "QuesaPick.h"
#include "QuesaRenderer.h"
#include "QuesaSet.h"
#include "QuesaShader.h"
#include "QuesaStorage.h"
#include "QuesaString.h"
#include "QuesaStyle.h"
#include "QuesaTransform.h"
#include "QuesaView.h"
#include "QuesaViewer.h"


// Mac OS
#if OS_MACINTOSH
	#include <Balloons.h>
	#include <CodeFragments.h>
	#include <ConditionalMacros.h>
	#include <ColorPicker.h>
	#include <Dialogs.h>
	#include <Drag.h>
	#include <Events.h>
	#include <QuickDraw.h>
	#include <Resources.h>
	#include <Scrap.h>
	#include <Sound.h>
	#include <TextUtils.h>

	#if defined(UNIVERSAL_INTERFACES_VERSION) && (UNIVERSAL_INTERFACES_VERSION > 0x0320)
		#include <ControlDefinitions.h>
	#endif
#endif


// ANSI
#include <string.h>
#include <math.h>





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//		Constants
//-----------------------------------------------------------------------------
#ifndef __QUESAQD3D__
	#define __QUESAQD3D__ 0
	#define __REALQD3D__  1
#endif

#ifndef __REALQD3D__
	#define __REALQD3D__ !__QUESAQD3D__
#endif

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

#if __REALQD3D__
enum TQ3ViewMode {
	kQ3ViewModeInactive	             		= 0,	// View not doing anythig
	kQ3ViewModeDrawing		         		= 1,	// Performing drawing
	kQ3ViewModePicking		         		= 2,	// Performing picking
	kQ3ViewModeWriting		    			= 3,	// Performing I/O
	kQ3ViewModeCalcBounds			 		= 4		// Calculating bounds
};
typedef enum TQ3ViewMode TQ3ViewMode;
#endif

#if !OS_MACINTOSH
	typedef unsigned char Str255 [256];
#endif

// kQ3XMethodTypeObjectNew
// Called after initialisation of your object. Initialise your plug-in instance
// kQ3XMethodTypeObjectDelete
// Called before deletion of your object. Dispose of your instance 

//---- TQ3ObjectTypePlugin methods ----
typedef CALLBACK_API_C( TQ3Status , TQ3XPluginViewParamMethod )(TQ3ViewObject theView);
typedef CALLBACK_API_C( TQ3ViewStatus , TQ3XPluginViewStatusMethod )(TQ3ViewObject theView);

#define kQ3XMethodType_PluginBeforeDraw					Q3_METHOD_TYPE('p','b','d','r')
// Called just before Q3View_StartDrawing.
// All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3Status , TQ3XPluginBeforeDrawMethod )(TQ3ViewObject theView, TQ3SharedObject thePlugin);

#define kQ3XMethodType_PluginAfterDraw					Q3_METHOD_TYPE('p','a','d','r')
// Called just after Q3View_EndDrawing.
// All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3Status , TQ3XPluginAfterDrawMethod )(TQ3ViewObject theView, TQ3SharedObject thePlugin);

#define kQ3XMethodType_PluginDuringSubmit				Q3_METHOD_TYPE('p','d','d','r')
// Called periodically during submission of objects during drawing, picking, writing and bounding box/sphere.
// All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3Status , TQ3XPluginDuringSubmitMethod )
	(TQ3ViewObject theView, TQ3SharedObject thePlugin, TQ3Uns32 current, TQ3Uns32 max, TQ3ViewMode mode); // mode is either draw, pick, writing, calc etc.

#define kQ3XMethodType_PluginAfterStartDraw				Q3_METHOD_TYPE('p','a','s','d')
// Called just after Q3View_StartDrawing. Submit any of your objects.
// All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3Status , TQ3XPluginAfterStartDrawMethod )(TQ3ViewObject theView, TQ3SharedObject thePlugin);

#define kQ3XMethodType_PluginBeforeEndDraw				Q3_METHOD_TYPE('p','b','e','d')
// Called just before Q3View_EndDrawing. Submit any of your objects. Plug-in can force retraverse?
// All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3ViewStatus , TQ3XPluginBeforeEndDrawMethod )(TQ3ViewObject theView, TQ3SharedObject thePlugin);

#define kQ3XMethodType_PluginAfterStartWrite			Q3_METHOD_TYPE('p','a','s','w')
// Called just after Q3View_StartWriting.
// All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3Status , TQ3XPluginAfterStartWritingMethod )(TQ3ViewObject theView, TQ3SharedObject thePlugin);

#define kQ3XMethodType_PluginBeforeEndWrite				Q3_METHOD_TYPE('p','b','e','w')
// Called just before Q3View_EndWriting.
// All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3ViewStatus , TQ3XPluginBeforeEndWritingMethod )(TQ3ViewObject theView, TQ3SharedObject thePlugin);

#define kQ3XMethodType_PluginAfterStartBounds			Q3_METHOD_TYPE('p','a','s','b')
// Called just after Q3View_StartBoundingBox or Q3View_StartBoundingSphere.
// All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3Status , TQ3XPluginAfterStartBoundsMethod )(TQ3ViewObject theView, TQ3SharedObject thePlugin);

#define kQ3XMethodType_PluginBeforeEndBounds			Q3_METHOD_TYPE('p','b','e','b')
// Called just before Q3View_EndBoundingBox or Q3View_EndBoundingSphere.
// All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3ViewStatus , TQ3XPluginBeforeEndBoundsMethod )(TQ3ViewObject theView, TQ3SharedObject thePlugin);

#define kQ3XMethodType_PluginAfterStartPick				Q3_METHOD_TYPE('p','a','s','p')
// Called just after Q3View_StartPicking.
// All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3Status , TQ3XPluginAfterStartPickingMethod )(TQ3ViewObject theView, TQ3SharedObject thePlugin);

#define kQ3XMethodType_PluginBeforeEndPick				Q3_METHOD_TYPE('p','b','e','p')
// Called just before Q3View_EndPicking. All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3ViewStatus , TQ3XPluginBeforeEndPickingMethod )(TQ3ViewObject theViewer, TQ3SharedObject thePlugin);


//---- TQ3PluginTypeViewer methods ----
typedef CALLBACK_API_C( TQ3Status , TQ3XPluginViewerParamMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);
typedef CALLBACK_API_C( TQ3Status , TQ3XPluginViewerParamEventMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, void* evt, TQ3Int32 param1, TQ3Int32 param2);
typedef CALLBACK_API_C( TQ3Status , TQ3XPluginViewerParamPointMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Point2D* pt);

#define kQ3XMethodType_ViewerPluginNewViewer			Q3_METHOD_TYPE('v','n','e','w')
// Called when a new viewer is created
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginNewViewerMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginDeleteViewer			Q3_METHOD_TYPE('v','d','e','l')
// Called when a viewer is destroyed
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDeleteViewerMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginDrawTool				Q3_METHOD_TYPE('v','d','r','t')
// Called when the tool needs to be drawn. Bounds is the area to draw your tool into and the port will be correctly set.
// If you return kQ3Failure a generic icon will be drawn.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDrawToolMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Area* bounds);

#define kQ3XMethodType_ViewerPluginClickTool			Q3_METHOD_TYPE('v','c','l','t')
// Called when the tool has been clicked.
// If you return kQ3Failure you will get your double click method called.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginClickToolMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginDoubleClickTool		Q3_METHOD_TYPE('v','d','c','t')
// Called when the tool has been double clicked.
// If you return kQ3Failure you will get your click method called.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoubleClickToolMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginUnclickTool			Q3_METHOD_TYPE('v','u','c','t')
// Called when another tool has been clicked
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginUnclickToolMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginCursorAdjust			Q3_METHOD_TYPE('v','c','a','d')
// Called when the cursor is over the viewer area. MousePt is in local coordinates.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginCursorAdjustMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Point2D* mousePt);

#define kQ3XMethodType_ViewerPluginGetHelpString		Q3_METHOD_TYPE('v','s','h','e')
// Called when the tool needs to display some help (Balloon help on Mac).
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginGetHelpStringMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, Str255 helpString);

#define kQ3XMethodType_ViewerPluginToolOptions			Q3_METHOD_TYPE('v','t','o','p')
// Called when the user wants to display some tool options (Perhaps clicked an options button)
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginToolOptionsMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginActivate				Q3_METHOD_TYPE('v','a','c','t')
// Called when the viewer is activated.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginActivateMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginDeactivate			Q3_METHOD_TYPE('v','d','a','c')
// Called when the viewer is deactivated.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDeactivateMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin);

#define kQ3XMethodType_ViewerPluginGroupChanged			Q3_METHOD_TYPE('v','g','r','c')
// Called when the viewer changes the contents of its group.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginGroupChangedMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3GroupObject group);

#define kQ3XMethodType_ViewerPluginCameraChanged		Q3_METHOD_TYPE('v','c','a','c')
// Called when the viewer changes its camera object.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginCameraChangedMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3CameraObject camera);

#define kQ3XMethodType_ViewerPluginRendererChanged		Q3_METHOD_TYPE('v','r','e','c')
// Called when the viewer changes its renderer object.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginRendererChangedMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3RendererObject renderer);

#define kQ3XMethodType_ViewerPluginLightsChanged		Q3_METHOD_TYPE('v','l','i','c')
// Called when the viewer changes the contents of its lights group.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginLightsChangedMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3GroupObject lightGroup);

#define kQ3XMethodType_ViewerPluginDrawContextChanged	Q3_METHOD_TYPE('v','d','c','c')
// Called when the viewer changes its draw context object.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDrawContextChangedMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3DrawContextObject drawContext);


// --- Platform Specific events ---

#define kQ3XMethodType_ViewerPluginDoToolEvent			Q3_METHOD_TYPE('v','d','o','t')
// Called when the selected tool has been clicked in the main viewer area. Do your stuff.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoToolEventMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, void* evt, TQ3Int32 param1, TQ3Int32 param2);
// On Mac evt is an EventRecord* and param1 and param2 are not used
// On Windows, ???

#define kQ3XMethodType_ViewerPluginDoToolStart			Q3_METHOD_TYPE('v','d','t','s')
// Called when the selected tool has been clicked in the main viewer area. Do your stuff.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoToolMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Int32 x, TQ3Int32 y);

#define kQ3XMethodType_ViewerPluginDoToolTracking		Q3_METHOD_TYPE('v','d','t','t')
// Called when the selected tool is tracking in the main viewer area. Do your stuff.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoToolTrackingMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Int32 x, TQ3Int32 y);

#define kQ3XMethodType_ViewerPluginDoToolEnd			Q3_METHOD_TYPE('v','d','t','e')
// Called when the selected tool is finished in the main viewer area. Do your stuff.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoToolEndMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, TQ3Int32 x, TQ3Int32 y);

#define kQ3XMethodType_ViewerPluginDoKeyDown			Q3_METHOD_TYPE('v','d','c','d')
// Called when the selected tool has been told that the key is for it. Do your stuff.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoKeyDownMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, void* evt, TQ3Int32 param1, TQ3Int32 param2);
// On Mac evt is an EventRecord* and param1 and param2 are not used
// On Windows, ???

#define kQ3XMethodType_ViewerPluginBeforeEvent			Q3_METHOD_TYPE('v','b','e','v')
// Called when your tool is active and an event has come in.
// If you return kQ3Success the event will not be passed on to the viewer as the plug-in will have dealt with it.
// You can do some processing depending on the event and still return kQ3Failure to allow the event to be passed on.
// Should be called for each registered Viewer plug-in.
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginBeforeEventMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, void* evt, TQ3Int32 param1, TQ3Int32 param2);
// On Mac evt is an EventRecord* and param1 and param2 are not used
// On Windows, ???

#define kQ3XMethodType_ViewerPluginAfterEvent			Q3_METHOD_TYPE('v','a','e','v')
// Called when your tool is active and an event has been processed. Should be called for each YPE('p','a','s','p')
// All registered plug-ins must be called
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginAfterEventMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, void* evt, TQ3Int32 param1, TQ3Int32 param2);

//---- TQ3PluginTypeViewer methods ----

#define	kQ3XMethodType_ViewerPluginDragAcceptable		Q3_METHOD_TYPE('v','d','r','a')
// Called when the viewer is about to accept a drop. Return the acceptable flavor.
typedef CALLBACK_API_C( TQ3Boolean , TQ3XViewerPluginDragAcceptableMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, DragReference drag, TQ3Uns32 item, TQ3Int32* myFlavor);
// On Mac, drag is a DragReference, item is an ItemReference, myFlavor is an OSType
// On Windows, drag is a Handle, item is unused, myFlavor is unused

#define kQ3XMethodType_ViewerPluginDoDrop				Q3_METHOD_TYPE('v','d','d','r')
// Called when the viewer has dropped your flavor type. Return kQ3Success if you succeeded
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginDoDropMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, DragReference drag, TQ3Uns32 item, TQ3Int32 yourFlavor);
// On Mac, drag is a DragReference, item is an ItemReference, yourFlavor is an OSType
// On Windows, drag is a Handle, item is unused, yourFlavor is unused

#define kQ3XMethodType_ViewerPluginAddDragFlavors		Q3_METHOD_TYPE('v','a','d','f')
// Called when the viewer is about to start a drag.
// Mac only ??
typedef CALLBACK_API_C( TQ3Status , TQ3XViewerPluginAddDragFlavorsMethod )(TQ3ViewerObject theViewer, TQ3SharedObject thePlugin, DragReference drag, TQ3Uns32 item);
// On Mac, drag is a DragReference, item is an ItemReference


//---- TQ3Object method

#define kQ3XMethodType_InterObjectMessage				Q3_METHOD_TYPE('i','o','m','e')
// Useful means of allowing objects to communicate with each other.
// We will need a call like Q3Object_SendMessage (TQ3Object fromObject, TQ3Object toObject, TQ3Int32 message, void* data, TQ3Uns32 length);
// message is any value you like and depends on the object you are communicating with.
// Define your own messaging system between your own plug-in suites.
typedef CALLBACK_API_C( TQ3Status , TQ3XObjectMessageMethod )(TQ3Object fromObject, TQ3Int32 message, void* messageData, TQ3Uns32 messageDataLength);



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
#if defined(OS_MACINTOSH) && OS_MACINTOSH

OSErr			Q3ViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision);
OSErr			Q3ViewerGetReleaseVersion(TQ3Uns32 *releaseRevision);
TQ3ViewerObject	Q3ViewerNew(CGrafPtr port, Rect *rect, TQ3Uns32 flags);
OSErr			Q3ViewerDispose(TQ3ViewerObject theViewer);
OSErr			Q3ViewerUseFile(TQ3ViewerObject theViewer, TQ3Int32 refNum);
OSErr			Q3ViewerUseData(TQ3ViewerObject theViewer, void *data, TQ3Int32 size);
OSErr			Q3ViewerWriteFile(TQ3ViewerObject theViewer, TQ3Int32 refNum);
TQ3Uns32		Q3ViewerWriteData(TQ3ViewerObject theViewer, Handle data);
OSErr			Q3ViewerDraw(TQ3ViewerObject theViewer);
OSErr			Q3ViewerDrawContent(TQ3ViewerObject theViewer);
OSErr			Q3ViewerDrawControlStrip(TQ3ViewerObject theViewer);
Boolean			Q3ViewerEvent(TQ3ViewerObject theViewer, EventRecord *evt);
PicHandle		Q3ViewerGetPict(TQ3ViewerObject theViewer);
OSErr			Q3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, Rect *rect);
TQ3Uns32		Q3ViewerGetCurrentButton(TQ3ViewerObject theViewer);
OSErr			Q3ViewerSetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 button);
OSErr			Q3ViewerUseGroup(TQ3ViewerObject theViewer, TQ3GroupObject group);
TQ3GroupObject	Q3ViewerGetGroup(TQ3ViewerObject theViewer);
OSErr			Q3ViewerSetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color);
OSErr			Q3ViewerGetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color);
TQ3ViewObject	Q3ViewerGetView(TQ3ViewerObject theViewer);
OSErr			Q3ViewerRestoreView(TQ3ViewerObject theViewer);
OSErr			Q3ViewerSetFlags(TQ3ViewerObject theViewer, TQ3Uns32 flags);
TQ3Uns32		Q3ViewerGetFlags(TQ3ViewerObject theViewer);
OSErr			Q3ViewerSetBounds(TQ3ViewerObject theViewer, Rect *bounds);
OSErr			Q3ViewerGetBounds(TQ3ViewerObject theViewer, Rect *bounds);
OSErr			Q3ViewerSetDimension(TQ3ViewerObject theViewer, TQ3Uns32 width, TQ3Uns32 height);
OSErr			Q3ViewerGetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height);
OSErr			Q3ViewerGetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height);
OSErr			Q3ViewerSetPort(TQ3ViewerObject theViewer, CGrafPtr port);
CGrafPtr		Q3ViewerGetPort(TQ3ViewerObject theViewer);
Boolean			Q3ViewerAdjustCursor(TQ3ViewerObject theViewer, Point *pt);
OSErr			Q3ViewerCursorChanged(TQ3ViewerObject theViewer);
TQ3Uns32		Q3ViewerGetState(TQ3ViewerObject theViewer);
OSErr			Q3ViewerClear(TQ3ViewerObject theViewer);
OSErr			Q3ViewerCut(TQ3ViewerObject theViewer);
OSErr			Q3ViewerCopy(TQ3ViewerObject theViewer);
OSErr			Q3ViewerPaste(TQ3ViewerObject theViewer);
Boolean			Q3ViewerMouseDown(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
Boolean			Q3ViewerContinueTracking(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
Boolean			Q3ViewerMouseUp(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
Boolean			Q3ViewerHandleKeyEvent(TQ3ViewerObject theViewer, EventRecord *evt);
OSErr			Q3ViewerSetDrawingCallbackMethod(TQ3ViewerObject theViewer, TQ3ViewerDrawingCallbackMethod callbackMethod, const void *data);
OSErr			Q3ViewerSetWindowResizeCallback(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod windowResizeCallbackMethod, const void *data);
OSErr			Q3ViewerSetPaneResizeNotifyCallback(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod paneResizeNotifyCallbackMethod, const void *data);
OSErr			Q3ViewerUndo(TQ3ViewerObject theViewer);
Boolean			Q3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, TQ3Uns32 *cnt);
OSErr			Q3ViewerGetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cnt);
OSErr			Q3ViewerSetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraNo);
OSErr			Q3ViewerSetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType);
OSErr			Q3ViewerSetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType);
OSErr			Q3ViewerGetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType);
OSErr			Q3ViewerChangeBrightness(TQ3ViewerObject theViewer, float brightness);
OSErr			Q3ViewerSetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean remove);
OSErr			Q3ViewerGetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *remove);
OSErr			Q3ViewerSetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phong);
OSErr			Q3ViewerGetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phong);

#else

// common prototypes for all other platforms
TQ3Status		Q3ViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision);
TQ3Status		Q3ViewerGetReleaseVersion(TQ3Uns32 *releaseRevision);
TQ3Status		Q3ViewerDispose(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerUseData(TQ3ViewerObject theViewer, void *data, TQ3Int32 size);
TQ3Status		Q3ViewerWriteData(TQ3ViewerObject theViewer, void *data, TQ3Uns32 dataSize, TQ3Uns32 *actualDataSize);
TQ3Status		Q3ViewerDraw(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerDrawContent(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerDrawControlStrip(TQ3ViewerObject theViewer);
TQ3Uns32		Q3ViewerGetCurrentButton(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerSetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 button);
TQ3Status		Q3ViewerUseGroup(TQ3ViewerObject theViewer, TQ3GroupObject group);
TQ3GroupObject	Q3ViewerGetGroup(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerSetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color);
TQ3Status		Q3ViewerGetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color);
TQ3ViewObject	Q3ViewerGetView(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerRestoreView(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerSetFlags(TQ3ViewerObject theViewer, TQ3Uns32 flags);
TQ3Uns32		Q3ViewerGetFlags(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerSetDimension(TQ3ViewerObject theViewer, TQ3Uns32 width, TQ3Uns32 height);
TQ3Status		Q3ViewerGetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height);
TQ3Status		Q3ViewerGetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height);
TQ3Boolean		Q3ViewerAdjustCursor(TQ3ViewerObject theViewer, TQ3Int32 x , TQ3Int32 y);
TQ3Status		Q3ViewerCursorChanged(TQ3ViewerObject theViewer);
TQ3Uns32		Q3ViewerGetState(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerClear(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerCut(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerCopy(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerPaste(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerSetDrawingCallbackMethod(TQ3ViewerObject theViewer, TQ3ViewerDrawingCallbackMethod callbackMethod, const void *data);
TQ3Status		Q3ViewerSetWindowResizeCallback(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod windowResizeCallbackMethod, const void *data);
TQ3Status		Q3ViewerSetPaneResizeNotifyCallback(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod paneResizeNotifyCallbackMethod, const void *data);
TQ3Status		Q3ViewerUndo(TQ3ViewerObject theViewer);
TQ3Boolean		Q3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, TQ3Uns32 stringSize, TQ3Uns32 *actualSize);
TQ3Status		Q3ViewerGetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cnt);
TQ3Status		Q3ViewerSetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraNo);
TQ3Status		Q3ViewerSetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType);
TQ3Status		Q3ViewerSetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType);
TQ3Status		Q3ViewerGetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType);
TQ3Status		Q3ViewerChangeBrightness(TQ3ViewerObject theViewer, float brightness);
TQ3Status		Q3ViewerSetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean remove);
TQ3Status		Q3ViewerGetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *remove);
TQ3Status		Q3ViewerSetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phong);
TQ3Status		Q3ViewerGetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phong);

// specific prototypes for all other platforms
	#if defined(OS_WIN32) && OS_WIN32

TQ3ViewerObject	Q3ViewerNew(HWND theWindow, const RECT *rect, TQ3Uns32 flags);
TQ3Status		Q3ViewerUseFile(TQ3ViewerObject theViewer, HANDLE fileHandle);
TQ3Status		Q3ViewerWriteFile(TQ3ViewerObject theViewer, HANDLE fileHandle);

HBITMAP			Q3ViewerGetBitmap(TQ3ViewerObject theViewer);
TQ3Status		Q3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, RECT *rect);
TQ3Status		Q3ViewerSetBounds(TQ3ViewerObject theViewer, RECT *bounds);
TQ3Status		Q3ViewerGetBounds(TQ3ViewerObject theViewer, RECT *bounds);

BOOL			Q3ViewerMouseDown(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
BOOL			Q3ViewerContinueTracking(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
BOOL			Q3ViewerMouseUp(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
	#else

TQ3ViewerObject	Q3ViewerNew(void* theWindow, const TQ3Area *rect, TQ3Uns32 flags);

TQ3Status		Q3ViewerUseFile(TQ3ViewerObject theViewer, void *fileRef);
TQ3Status		Q3ViewerWriteFile(TQ3ViewerObject theViewer, void *fileRef);
void*			Q3ViewerGetPicture(TQ3ViewerObject theViewer);

TQ3Status		Q3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, TQ3Area *rect);
TQ3Status		Q3ViewerSetBounds(TQ3ViewerObject theViewer, TQ3Area *bounds);
TQ3Status		Q3ViewerGetBounds(TQ3ViewerObject theViewer, TQ3Area *bounds);

TQ3Boolean		Q3ViewerMouseDown(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
TQ3Boolean		Q3ViewerContinueTracking(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
TQ3Boolean		Q3ViewerMouseUp(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);

	#endif
#endif

// new APIs
TQ3Status		Q3ViewerGetGroupBounds(TQ3ViewerObject theViewer, TQ3BoundingBox* bounds);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


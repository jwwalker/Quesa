/*  NAME:
        E3ViewerOldAPIs.c

    DESCRIPTION:
        Part of the Quesa viewer library.  This file implements the old,
        mostly platform-specific APIs that were defined in QD3D.  These
        implementations just do a bit of constant-mapping and then call
        through to the newer, unified Quesa APIs.

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
#include "E3Prefix.h"
#include "E3Memory.h"
#include "E3Viewer.h"





//=============================================================================
//      Mac OS Viewer APIs
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

#pragma mark Mac OS Viewer APIs
#pragma mark -

//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3viewer_q3status_oserr : Convert a TQ3Status into an OSErr.
//-----------------------------------------------------------------------------
//		Note :	The old error codes were not defined in the QD3D docs as far as
//				I can tell.  For now, I'll just do a simple map.  (-- JJS)
//-----------------------------------------------------------------------------
static inline OSErr e3viewer_q3status_oserr(TQ3Status status)
{
	if (kQ3Success == status) return noErr;
	// If we have a failure status, we really should get the error code from
	// Quesa and try to map it onto an appropriate OSErr code.  But that's
	// hard, so for now we'll content ourselves with:
	return kGeneralFailureErr;
}


//-----------------------------------------------------------------------------
//      e3viewer_oldButtonToNew : Convert an old Mac QD3DViewer button
//			identifier into the ones used with the new unified API.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_oldButtonToNew(TQ3Uns32 button)
{
	switch (button)
		{
		case kQ3ViewerButtonCamera:		return kQ3ViewerFlagButtonCamera;
		case kQ3ViewerButtonTruck:		return kQ3ViewerFlagButtonTruck;
		case kQ3ViewerButtonOrbit:		return kQ3ViewerFlagButtonOrbit;
		case kQ3ViewerButtonZoom:		return kQ3ViewerFlagButtonZoom;
		case kQ3ViewerButtonDolly:		return kQ3ViewerFlagButtonDolly;
		case kQ3ViewerButtonReset:		return kQ3ViewerFlagButtonReset;
		case kQ3ViewerButtonOptions:	return kQ3ViewerFlagButtonOptions;
		}
	return 0;
}

//-----------------------------------------------------------------------------
//      e3viewer_newButtonToOld : Convert a button identifier used with the
//			new unified API into one used with the old Mac QD3DViewer.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_newButtonToOld(TQ3Uns32 button)
{
	switch (button)
		{
		case kQ3ViewerFlagButtonCamera:		return kQ3ViewerButtonCamera;
		case kQ3ViewerFlagButtonTruck:		return kQ3ViewerButtonTruck;
		case kQ3ViewerFlagButtonOrbit:		return kQ3ViewerButtonOrbit;
		case kQ3ViewerFlagButtonZoom:		return kQ3ViewerButtonZoom;
		case kQ3ViewerFlagButtonDolly:		return kQ3ViewerButtonDolly;
		case kQ3ViewerFlagButtonReset:		return kQ3ViewerButtonReset;
		case kQ3ViewerFlagButtonOptions:	return kQ3ViewerButtonOptions;
		}
	return 0;
}

//-----------------------------------------------------------------------------
//      e3viewer_rect_area : Convert a MacOS Rect into a TQ3Area.
//-----------------------------------------------------------------------------
static void e3viewer_rect_area(Rect *inRect, TQ3Area *outArea)
{
	outArea->min.x = inRect->left;
	outArea->min.y = inRect->top;
	outArea->max.x = inRect->right;
	outArea->max.y = inRect->bottom;
}

//-----------------------------------------------------------------------------
//      e3viewer_area_rect : Convert a TQ3Area into a MacOS Rect.
//-----------------------------------------------------------------------------
static void e3viewer_area_rect(TQ3Area *inArea, Rect *outRect)
{
	outRect->left = inArea->min.x;
	outRect->top = inArea->min.y;
	outRect->right = inArea->max.x;
	outRect->bottom = inArea->max.y;
}

//-----------------------------------------------------------------------------
//      e3viewer_oldFlagsToNew : Convert old viewer flags into the new
//			flags used with the unified viewer API.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_oldFlagsToNew(TQ3Uns32 oldFlags)
{
	TQ3Uns32 newFlags = 0L;

	if (kQ3ViewerDefault & oldFlags)
		newFlags = kQ3ViewerFlagDefault;

	if (oldFlags & kQ3ViewerShowBadge)			newFlags |= kQ3ViewerFlagShowBadge;
	if (oldFlags & kQ3ViewerActive)				newFlags |= kQ3ViewerFlagActive;
	if (oldFlags & kQ3ViewerControllerVisible)	newFlags |= kQ3ViewerFlagControllerVisible;
	if (oldFlags & kQ3ViewerDrawFrame)			newFlags |= kQ3ViewerFlagDrawFrame;
	if (oldFlags & kQ3ViewerDraggingOff)		newFlags |= kQ3ViewerFlagDraggingOff;
	if (oldFlags & kQ3ViewerButtonCamera)		newFlags |= kQ3ViewerFlagButtonCamera;
	if (oldFlags & kQ3ViewerButtonTruck)		newFlags |= kQ3ViewerFlagButtonTruck;
	if (oldFlags & kQ3ViewerButtonOrbit)		newFlags |= kQ3ViewerFlagButtonOrbit;
	if (oldFlags & kQ3ViewerButtonZoom)			newFlags |= kQ3ViewerFlagButtonZoom;
	if (oldFlags & kQ3ViewerButtonDolly)		newFlags |= kQ3ViewerFlagButtonDolly;
	if (oldFlags & kQ3ViewerButtonReset)		newFlags |= kQ3ViewerFlagButtonReset;
	if (oldFlags & kQ3ViewerOutputTextMode)		newFlags |= kQ3ViewerFlagOutputTextMode;
	if (oldFlags & kQ3ViewerDragMode)			newFlags |= kQ3ViewerFlagDragMode;
	if (oldFlags & kQ3ViewerDrawGrowBox)		newFlags |= kQ3ViewerFlagDrawGrowBox;
	if (oldFlags & kQ3ViewerDrawDragBorder)		newFlags |= kQ3ViewerFlagDrawDragBorder;
	if (oldFlags & kQ3ViewerDraggingInOff)		newFlags |= kQ3ViewerFlagDraggingInOff;
	if (oldFlags & kQ3ViewerDraggingOutOff)		newFlags |= kQ3ViewerFlagDraggingOutOff;
	if (oldFlags & kQ3ViewerButtonOptions)		newFlags |= kQ3ViewerFlagButtonOptions;
	if (oldFlags & kQ3ViewerPaneGrowBox)		newFlags |= kQ3ViewerFlagPaneGrowBox;
	
	return newFlags;
}

//-----------------------------------------------------------------------------
//      e3viewer_newFlagsToOld : Convert new viewer flags into the old
//			flags used with the QD3D Mac/Win viewer API.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_newFlagsToOld(TQ3Uns32 newFlags)
{
	TQ3Uns32 oldFlags = 0L;

	if (kQ3ViewerFlagDefault & newFlags)
		oldFlags = kQ3ViewerDefault;

	if (newFlags & kQ3ViewerFlagShowBadge)			oldFlags |= kQ3ViewerShowBadge;
	if (newFlags & kQ3ViewerFlagActive)				oldFlags |= kQ3ViewerActive;
	if (newFlags & kQ3ViewerFlagVisible)			oldFlags |= kQ3ViewerControllerVisible;
	if (newFlags & kQ3ViewerFlagDrawFrame)			oldFlags |= kQ3ViewerDrawFrame;
	if (newFlags & kQ3ViewerFlagDraggingOff)		oldFlags |= kQ3ViewerDraggingOff;
	if (newFlags & kQ3ViewerFlagButtonCamera)		oldFlags |= kQ3ViewerButtonCamera;
	if (newFlags & kQ3ViewerFlagButtonTruck)		oldFlags |= kQ3ViewerButtonTruck;
	if (newFlags & kQ3ViewerFlagButtonOrbit)		oldFlags |= kQ3ViewerButtonOrbit;
	if (newFlags & kQ3ViewerFlagButtonZoom)			oldFlags |= kQ3ViewerButtonZoom;
	if (newFlags & kQ3ViewerFlagButtonDolly)		oldFlags |= kQ3ViewerButtonDolly;
	if (newFlags & kQ3ViewerFlagButtonReset)		oldFlags |= kQ3ViewerButtonReset;
	if (newFlags & kQ3ViewerFlagOutputTextMode)		oldFlags |= kQ3ViewerOutputTextMode;
	if (newFlags & kQ3ViewerFlagDragMode)			oldFlags |= kQ3ViewerDragMode;
	if (newFlags & kQ3ViewerFlagDrawGrowBox)		oldFlags |= kQ3ViewerDrawGrowBox;
	if (newFlags & kQ3ViewerFlagDrawDragBorder)		oldFlags |= kQ3ViewerDrawDragBorder;
	if (newFlags & kQ3ViewerFlagDraggingInOff)		oldFlags |= kQ3ViewerDraggingInOff;
	if (newFlags & kQ3ViewerFlagDraggingOutOff)		oldFlags |= kQ3ViewerDraggingOutOff;
	if (newFlags & kQ3ViewerFlagButtonOptions)		oldFlags |= kQ3ViewerButtonOptions;
	if (newFlags & kQ3ViewerFlagPaneGrowBox)		oldFlags |= kQ3ViewerPaneGrowBox;
	
	return oldFlags;
}

//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
OSErr		
E3ViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{
	return e3viewer_q3status_oserr( Q3Viewer_GetVersion(majorRevision, minorRevision) );
}


OSErr		
E3ViewerGetReleaseVersion(TQ3Uns32 *releaseRevision)
{
	return e3viewer_q3status_oserr( Q3Viewer_GetReleaseVersion(releaseRevision) );
}


TQ3ViewerObject
E3ViewerNew(CGrafPtr port, Rect *rect, TQ3Uns32 oldFlags)
{
	TQ3Area area;
	TQ3Uns32 newFlags = e3viewer_oldFlagsToNew(oldFlags);

	e3viewer_rect_area(rect, &area);
	
	return Q3Viewer_New(port, &area, newFlags);
}


OSErr		
E3ViewerDispose(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_Dispose(theViewer) );
}


OSErr		
E3ViewerUseFile(TQ3ViewerObject theViewer, TQ3Int32 refNum)
{
	return e3viewer_q3status_oserr( Q3Viewer_UseFile(theViewer, refNum) );
}


OSErr		
E3ViewerUseData(TQ3ViewerObject theViewer, void *data, TQ3Int32 size)
{
	return e3viewer_q3status_oserr( Q3Viewer_UseData(theViewer, data, size) );	
}


OSErr		
E3ViewerWriteFile(TQ3ViewerObject theViewer, TQ3Int32 refNum)
{
	return e3viewer_q3status_oserr( Q3Viewer_WriteFile(theViewer, refNum) );
}


TQ3Uns32	
E3ViewerWriteData(TQ3ViewerObject theViewer, Handle data)
{
	void *tempData;
	TQ3Uns32 dataSize;
	OSErr memErr;
	TQ3Status status = Q3Viewer_WriteData(theViewer, &tempData, &dataSize);
	if (kQ3Success == status)
		{
		SetHandleSize(data, dataSize);
		memErr = MemError();
		if (noErr == memErr)
			BlockMoveData( tempData, *data, dataSize );
		}
	
	E3Memory_Free( &tempData );
	if (memErr) return memErr;
	return e3viewer_q3status_oserr(status);
}


OSErr		
E3ViewerDraw(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_Draw(theViewer) );
}


OSErr		
E3ViewerDrawContent(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_DrawContent(theViewer) );
}


OSErr		
E3ViewerDrawControlStrip(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_DrawControlStrip(theViewer) );
}


Boolean		
E3ViewerEvent(TQ3ViewerObject theViewer, EventRecord *evt)
{
	TQ3Boolean handled = kQ3False;
	switch (evt->what)
		{
		case mouseDown:
			handled = Q3Viewer_EventMouseDown(theViewer, evt->where.h, evt->where.v);
			break;
		case mouseUp:
			handled = Q3Viewer_EventMouseUp(theViewer, evt->where.h, evt->where.v);
			break;
	}
	if (!handled && (evt->message & 0xFF000000)==0xFA000000)  // mouse moved
		handled = Q3Viewer_EventMouseTrack(theViewer, evt->where.h, evt->where.v);
		
	if (kQ3True == handled) return true;
	else return false;
}

PicHandle	
E3ViewerGetPict(TQ3ViewerObject theViewer)
{

	return(NULL);  // unimplemented
}


OSErr		
E3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, Rect *rect)
{
	TQ3Area area;
	TQ3Status status = Q3Viewer_GetButtonRect(theViewer, 
				e3viewer_oldButtonToNew(button), &area);
	if (kQ3Success == status)
		e3viewer_area_rect(&area, rect);
	return e3viewer_q3status_oserr(status);
}


TQ3Uns32	
E3ViewerGetCurrentButton(TQ3ViewerObject theViewer)
{
	return e3viewer_newButtonToOld( Q3Viewer_GetCurrentButton(theViewer) );
}


OSErr		
E3ViewerSetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 button)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetCurrentButton(theViewer,
				e3viewer_oldButtonToNew(button)) );
}


OSErr		
E3ViewerUseGroup(TQ3ViewerObject theViewer, TQ3GroupObject group)
{
	return e3viewer_q3status_oserr( Q3Viewer_UseGroup(theViewer, group) );
}


TQ3GroupObject
E3ViewerGetGroup(TQ3ViewerObject theViewer)
{
	return Q3Viewer_GetGroup(theViewer);
}


OSErr		
E3ViewerSetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetBackgroundColor(theViewer, color) );
}


OSErr		
E3ViewerGetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
{
	return e3viewer_q3status_oserr( Q3Viewer_GetBackgroundColor(theViewer, color) );
}


TQ3ViewObject
E3ViewerGetView(TQ3ViewerObject theViewer)
{
	return Q3Viewer_GetView(theViewer);
}


OSErr		
E3ViewerRestoreView(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_RestoreView(theViewer) );
}


OSErr		
E3ViewerSetFlags(TQ3ViewerObject theViewer, TQ3Uns32 flags)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetFlags(theViewer,
				e3viewer_oldFlagsToNew(flags)) );
}


TQ3Uns32	
E3ViewerGetFlags(TQ3ViewerObject theViewer)
{
	return e3viewer_newFlagsToOld( Q3Viewer_GetFlags(theViewer) );
}


OSErr		
E3ViewerSetBounds(TQ3ViewerObject theViewer, Rect *bounds)
{
	TQ3Area area;
	e3viewer_rect_area(bounds, &area);
	return e3viewer_q3status_oserr( Q3Viewer_SetBounds(theViewer, &area) );
}


OSErr		
E3ViewerGetBounds(TQ3ViewerObject theViewer, Rect *bounds)
{
	TQ3Area area;
	TQ3Status status = Q3Viewer_GetBounds(theViewer, &area);
	if (kQ3Success == status)
		e3viewer_area_rect(&area, bounds);
	return e3viewer_q3status_oserr(status);
}


OSErr		
E3ViewerSetDimension(TQ3ViewerObject theViewer, TQ3Uns32 width, TQ3Uns32 height)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetDimension(theViewer, width, height) );
}


OSErr		
E3ViewerGetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height)
{
	return e3viewer_q3status_oserr( Q3Viewer_GetDimension(theViewer, width, height) );
}


OSErr		
E3ViewerGetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height)
{
	return e3viewer_q3status_oserr( Q3Viewer_GetMinimumDimension(theViewer, width, height) );
}


OSErr		
E3ViewerSetPort(TQ3ViewerObject theViewer, CGrafPtr port)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetWindow(theViewer, port) );
}


CGrafPtr	
E3ViewerGetPort(TQ3ViewerObject theViewer)
{
	return Q3Viewer_GetWindow(theViewer);
}


Boolean		
E3ViewerAdjustCursor(TQ3ViewerObject theViewer, Point *pt)
{
	return kQ3True == Q3Viewer_AdjustCursor(theViewer, pt->h, pt->v);
}


OSErr		
E3ViewerCursorChanged(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_CursorChanged(theViewer) );
}


TQ3Uns32	
E3ViewerGetState(TQ3ViewerObject theViewer)
{
	return Q3Viewer_GetState(theViewer);
}


OSErr		
E3ViewerClear(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_EditClear(theViewer) );
}


OSErr		
E3ViewerCut(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_EditCut(theViewer) );
}


OSErr		
E3ViewerCopy(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_EditCopy(theViewer) );
}


OSErr		
E3ViewerPaste(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_EditPaste(theViewer) );
}


Boolean		
E3ViewerMouseDown(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{
	return kQ3True == Q3Viewer_EventMouseDown(theViewer, x, y);
}


Boolean		
E3ViewerContinueTracking(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{
	return kQ3True == Q3Viewer_EventMouseTrack(theViewer, x, y);
}


Boolean		
E3ViewerMouseUp(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{
	return kQ3True == Q3Viewer_EventMouseUp(theViewer, x, y);
}


Boolean		
E3ViewerHandleKeyEvent(TQ3ViewerObject theViewer, EventRecord *evt)
{
	return kQ3True == Q3Viewer_EventKeyboard(theViewer, evt);
}


OSErr		
E3ViewerSetDrawingCallbackMethod(TQ3ViewerObject theViewer, TQ3ViewerDrawingCallbackMethod callbackMethod, const void *data)
{
	return(-4);  // unimplemented	
}


OSErr		
E3ViewerSetWindowResizeCallback(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod windowResizeCallbackMethod, const void *data)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerSetPaneResizeNotifyCallback(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod paneResizeNotifyCallbackMethod, const void *data)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerUndo(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_EditUndo(theViewer) );
}


Boolean		
E3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, TQ3Uns32 *cnt)
{
	return kQ3True == Q3Viewer_GetUndoString(theViewer, str, cnt);
}


OSErr		
E3ViewerGetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cnt)
{
	return e3viewer_q3status_oserr( Q3Viewer_GetCameraCount(theViewer, cnt) );
}


OSErr		
E3ViewerSetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraNo)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetCameraByNumber(theViewer, cameraNo) );
}


OSErr		
E3ViewerSetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetCameraByView(theViewer, viewType) );
}


OSErr		
E3ViewerSetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetRendererType(theViewer, rendererType) );
}


OSErr		
E3ViewerGetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType)
{
	return e3viewer_q3status_oserr( Q3Viewer_GetRendererType(theViewer, rendererType) );
}


OSErr		
E3ViewerChangeBrightness(TQ3ViewerObject theViewer, float brightness)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetBrightness(theViewer, brightness) );
}


OSErr		
E3ViewerSetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean remove)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetRemoveBackfaces(theViewer, remove) );
}


OSErr		
E3ViewerGetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *remove)
{
	return e3viewer_q3status_oserr( Q3Viewer_GetRemoveBackfaces(theViewer, remove) );
}


OSErr		
E3ViewerSetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phong)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetPhongShading(theViewer, phong) );
}


OSErr		
E3ViewerGetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phong)
{
	return e3viewer_q3status_oserr( Q3Viewer_GetPhongShading(theViewer, phong) );
}

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Function prototypes
//      Old cross-platform QD3D Viewer APIs
//-----------------------------------------------------------------------------
#pragma mark -
#pragma mark old common APIs
#pragma mark -

#if QUESA_OS_MACINTOSH || QUESA_OS_WIN32

TQ3Status	
E3ViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerGetReleaseVersion(TQ3Uns32 *releaseRevision)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerDispose(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerUseData(TQ3ViewerObject theViewer, void *data, TQ3Int32 size)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerWriteData(TQ3ViewerObject theViewer, void *data, TQ3Uns32 dataSize, TQ3Uns32 *actualDataSize)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerDraw(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerDrawContent(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerDrawControlStrip(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Uns32	
E3ViewerGetCurrentButton(TQ3ViewerObject theViewer)
{

	return(0); // unimplemented
}


TQ3Status	
E3ViewerSetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 button)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerUseGroup(TQ3ViewerObject theViewer, TQ3GroupObject group)
{

	return(-4);  // unimplemented
}


TQ3GroupObject
E3ViewerGetGroup(TQ3ViewerObject theViewer)
{

	return(NULL);  // unimplemented
}


TQ3Status	
E3ViewerSetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerGetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
{

	return(-4);  // unimplemented
}


TQ3ViewObject
E3ViewerGetView(TQ3ViewerObject theViewer)
{

	return(NULL);  // unimplemented
}


TQ3Status	
E3ViewerRestoreView(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerSetFlags(TQ3ViewerObject theViewer, TQ3Uns32 flags)
{

	return(-4);  // unimplemented
}


TQ3Uns32	
E3ViewerGetFlags(TQ3ViewerObject theViewer)
{

	return(0); // unimplemented
}


TQ3Status	
E3ViewerSetDimension(TQ3ViewerObject theViewer, TQ3Uns32 width, TQ3Uns32 height)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerGetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerGetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height)
{

	return(-4);  // unimplemented
}


TQ3Boolean	
E3ViewerAdjustCursor(TQ3ViewerObject theViewer, TQ3Int32 x , TQ3Int32 y)
{

	return(kQ3False);  // unimplemented
}


TQ3Status	
E3ViewerCursorChanged(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Uns32	
E3ViewerGetState(TQ3ViewerObject theViewer)
{

	return(0); // unimplemented
}


TQ3Status	
E3ViewerClear(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerCut(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerCopy(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerPaste(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerSetDrawingCallbackMethod(TQ3ViewerObject theViewer, TQ3ViewerDrawingCallbackMethod callbackMethod, const void *data)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerSetWindowResizeCallback(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod windowResizeCallbackMethod, const void *data)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerSetPaneResizeNotifyCallback(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod paneResizeNotifyCallbackMethod, const void *data)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerUndo(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Boolean	
E3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, TQ3Uns32 stringSize, TQ3Uns32 *actualSize)
{

	return(kQ3False);  // unimplemented
}


TQ3Status	
E3ViewerGetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cnt)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerSetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraNo)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerSetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerSetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerGetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerChangeBrightness(TQ3ViewerObject theViewer, float brightness)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerSetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean remove)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerGetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *remove)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerSetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phong)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerGetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phong)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerGetGroupBounds(TQ3ViewerObject theViewer, TQ3BoundingBox* bounds)
{

	return(-4);  // unimplemented
}

#endif // QUESA_OS_MACINTOSH || QUESA_OS_WIN32





//=============================================================================
//      Windows QD3D Viewer APIs
//-----------------------------------------------------------------------------
#pragma mark -
#pragma mark Windows QD3DViewer APIs
#pragma mark -

#if QUESA_OS_WIN32

//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
TQ3ViewerObject
E3ViewerNew(HWND theWindow, const RECT *rect, TQ3Uns32 flags)
{

	return(NULL);  // unimplemented
}


TQ3Status	
E3ViewerUseFile(TQ3ViewerObject theViewer, HANDLE fileHandle)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerWriteFile(TQ3ViewerObject theViewer, HANDLE fileHandle)
{

	return(-4);  // unimplemented
}



HBITMAP		
E3ViewerGetBitmap(TQ3ViewerObject theViewer)
{

	return(NULL);	// unimplemented
}


HWND		
E3ViewerGetWindow(TQ3ViewerObject theViewer)
{

	return(NULL);	// unimplemented
}


TQ3Status	
E3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, RECT *rect)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerSetBounds(TQ3ViewerObject theViewer, RECT *bounds)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerGetBounds(TQ3ViewerObject theViewer, RECT *bounds)
{

	return(-4);  // unimplemented
}



BOOL		
E3ViewerMouseDown(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{

	return(0);	// unimplemented
}


BOOL		
E3ViewerContinueTracking(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{

	return(0);	// unimplemented
}


BOOL		
E3ViewerMouseUp(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{

	return(0);	// unimplemented
}

TQ3ViewerObject
E3ViewerNew(void* theWindow, const TQ3Area *rect, TQ3Uns32 flags)
{

	return(NULL);  // unimplemented
}



TQ3Status	
E3ViewerUseFile(TQ3ViewerObject theViewer, void *fileRef)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerWriteFile(TQ3ViewerObject theViewer, void *fileRef)
{

	return(-4);  // unimplemented
}


void*		
E3ViewerGetPicture(TQ3ViewerObject theViewer)
{

	return(defvoid*		);
}



TQ3Status	
E3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, TQ3Area *rect)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerSetBounds(TQ3ViewerObject theViewer, TQ3Area *bounds)
{

	return(-4);  // unimplemented
}


TQ3Status	
E3ViewerGetBounds(TQ3ViewerObject theViewer, TQ3Area *bounds)
{

	return(-4);  // unimplemented
}



TQ3Boolean	
E3ViewerMouseDown(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{

	return(kQ3False);  // unimplemented
}


TQ3Boolean	
E3ViewerContinueTracking(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{

	return(kQ3False);  // unimplemented
}


TQ3Boolean	
E3ViewerMouseUp(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{

	return(kQ3False);  // unimplemented
}

#endif // QUESA_OS_WIN32


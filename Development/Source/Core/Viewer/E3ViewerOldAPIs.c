/*  NAME:
        E3ViewerOldAPIs.c

    DESCRIPTION:
        Part of the Quesa viewer library.  This file implements the old,
        mostly platform-specific APIs that were defined in QD3D.  These
        implementations just do a bit of constant-mapping and then call
        through to the newer, unified Quesa APIs.

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
#include "E3Memory.h"
#include "E3Viewer.h"





//=============================================================================
//      Mac OS Viewer APIs
//-----------------------------------------------------------------------------
#if QUESA_SUPPORT_HITOOLBOX

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
	outRect->left = (short)inArea->min.x;
	outRect->top = (short)inArea->min.y;
	outRect->right = (short)inArea->max.x;
	outRect->bottom = (short)inArea->max.y;
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
	if (newFlags & kQ3ViewerFlagControllerVisible)	oldFlags |= kQ3ViewerControllerVisible;
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
E3ViewerGetVersion(unsigned long *majorRevision, unsigned long *minorRevision)
{
	TQ3Uns32 major = 0;
	TQ3Uns32 minor = 0;
	
	TQ3Status status = Q3Viewer_GetVersion(&major, &minor);
	
	*majorRevision = major;
	*minorRevision = minor;
	
	return e3viewer_q3status_oserr(status);
}


OSErr		
E3ViewerGetReleaseVersion(unsigned long *releaseRevision)
{
	TQ3Uns32 vers = 0;
	
	TQ3Status status = Q3Viewer_GetReleaseVersion(&vers);
	
	*releaseRevision = vers;
	
	return e3viewer_q3status_oserr(status);
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
			memmove( *data, tempData, dataSize );
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
	Point   localWhere = evt->where;
	GlobalToLocal(&localWhere);

	switch (evt->what)
		{
		case mouseDown:
			// we can recode this with TrackMouseLocation,
			if(Q3Viewer_EventMouseDown( theViewer, localWhere.h, localWhere.v )){
				while (StillDown()) {
					GetMouse( &localWhere );
					Q3ViewerContinueTracking( theViewer, localWhere.h, localWhere.v );
				}
				Q3Viewer_EventMouseUp( theViewer, localWhere.h, localWhere.v );
				handled = kQ3True;
			}
			break;
		case mouseUp:
			handled = Q3Viewer_EventMouseUp(theViewer, localWhere.h, localWhere.v);
			break;
	}
	if (!handled && (evt->message & 0xFF000000)==0xFA000000)  // mouse moved
		handled = Q3Viewer_EventMouseTrack(theViewer, localWhere.h, localWhere.v);
		
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
E3ViewerSetDimension(TQ3ViewerObject theViewer, unsigned long width, unsigned long height)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetDimension(theViewer, width, height) );
}


OSErr		
E3ViewerGetDimension(TQ3ViewerObject theViewer, unsigned long *width, unsigned long *height)
{
	TQ3Uns32 width_ = 0;
	TQ3Uns32 height_ = 0;
	
	TQ3Status status = Q3Viewer_GetDimension(theViewer, &width_, &height_);
	
	*width = width_;
	*height = height_;
	
	return e3viewer_q3status_oserr(status);
}


OSErr		
E3ViewerGetMinimumDimension(TQ3ViewerObject theViewer, unsigned long *width, unsigned long *height)
{
	TQ3Uns32 width_ = 0;
	TQ3Uns32 height_ = 0;
	
	TQ3Status status = Q3Viewer_GetMinimumDimension(theViewer, &width_, &height_);
	
	*width = width_;
	*height = height_;
	
	return e3viewer_q3status_oserr(status);
}


OSErr		
E3ViewerSetPort(TQ3ViewerObject theViewer, CGrafPtr port)
{
	return e3viewer_q3status_oserr( Q3Viewer_SetWindow(theViewer, port) );
}


CGrafPtr	
E3ViewerGetPort(TQ3ViewerObject theViewer)
{
	return (CGrafPtr) Q3Viewer_GetWindow(theViewer);
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
	return(unimpErr);  // unimplemented	
}


OSErr		
E3ViewerSetWindowResizeCallback(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod windowResizeCallbackMethod, const void *data)
{

	return(unimpErr);  // unimplemented
}


OSErr		
E3ViewerSetPaneResizeNotifyCallback(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod paneResizeNotifyCallbackMethod, const void *data)
{

	return(unimpErr);  // unimplemented
}


OSErr		
E3ViewerUndo(TQ3ViewerObject theViewer)
{
	return e3viewer_q3status_oserr( Q3Viewer_EditUndo(theViewer) );
}


Boolean		
E3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, unsigned long *cnt)
{
	TQ3Uns32 count = 0;
	
	TQ3Boolean result = Q3Viewer_GetUndoString(theViewer, str, &count);
	
	*cnt = count;
	
	return result == kQ3True;
}


OSErr		
E3ViewerGetCameraCount(TQ3ViewerObject theViewer, unsigned long *cnt)
{
	TQ3Uns32 count = 0;
	
	TQ3Status status = Q3Viewer_GetCameraCount(theViewer, &count);
	
	*cnt = count;
	
	return e3viewer_q3status_oserr(status);
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



//=============================================================================
//      Win32 versions of old cross-platform QD3D Viewer APIs
//-----------------------------------------------------------------------------
#pragma mark -
#pragma mark old common APIs
#pragma mark -

#else    // i.e., if !QUESA_OS_MACINTOSH

TQ3Status	
E3ViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{

	return((TQ3Status)-4);  // unimplemented  
}


TQ3Status	
E3ViewerGetReleaseVersion(TQ3Uns32 *releaseRevision)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerDispose(TQ3ViewerObject theViewer)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerUseData(TQ3ViewerObject theViewer, void *data, TQ3Int32 size)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerWriteData(TQ3ViewerObject theViewer, void *data, TQ3Uns32 dataSize, TQ3Uns32 *actualDataSize)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerDraw(TQ3ViewerObject theViewer)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerDrawContent(TQ3ViewerObject theViewer)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerDrawControlStrip(TQ3ViewerObject theViewer)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Uns32	
E3ViewerGetCurrentButton(TQ3ViewerObject theViewer)
{

	return(0); // unimplemented
}


TQ3Status	
E3ViewerSetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 button)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerUseGroup(TQ3ViewerObject theViewer, TQ3GroupObject group)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3GroupObject
E3ViewerGetGroup(TQ3ViewerObject theViewer)
{

	return(NULL);  // unimplemented
}


TQ3Status	
E3ViewerSetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerGetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3ViewObject
E3ViewerGetView(TQ3ViewerObject theViewer)
{

	return(NULL);  // unimplemented
}


TQ3Status	
E3ViewerRestoreView(TQ3ViewerObject theViewer)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerSetFlags(TQ3ViewerObject theViewer, TQ3Uns32 flags)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Uns32	
E3ViewerGetFlags(TQ3ViewerObject theViewer)
{

	return(0); // unimplemented
}


TQ3Status	
E3ViewerSetDimension(TQ3ViewerObject theViewer, TQ3Uns32 width, TQ3Uns32 height)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerGetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerGetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Boolean	
E3ViewerAdjustCursor(TQ3ViewerObject theViewer, TQ3Int32 x , TQ3Int32 y)
{

	return(kQ3False);  // unimplemented
}


TQ3Status	
E3ViewerCursorChanged(TQ3ViewerObject theViewer)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Uns32	
E3ViewerGetState(TQ3ViewerObject theViewer)
{

	return(0); // unimplemented
}


TQ3Status	
E3ViewerClear(TQ3ViewerObject theViewer)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerCut(TQ3ViewerObject theViewer)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerCopy(TQ3ViewerObject theViewer)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerPaste(TQ3ViewerObject theViewer)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status		
E3ViewerSetDrawingCallbackMethod(TQ3ViewerObject theViewer, TQ3ViewerDrawingCallbackMethod callbackMethod, const void *data)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerSetWindowResizeCallback(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod windowResizeCallbackMethod, const void *data)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerSetPaneResizeNotifyCallback(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod paneResizeNotifyCallbackMethod, const void *data)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerUndo(TQ3ViewerObject theViewer)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Boolean	
E3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, TQ3Uns32 stringSize, TQ3Uns32 *actualSize)
{

	return(kQ3False);  // unimplemented
}


TQ3Status	
E3ViewerGetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cnt)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerSetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraNo)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerSetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerSetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerGetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerChangeBrightness(TQ3ViewerObject theViewer, float brightness)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerSetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean remove)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerGetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *remove)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerSetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phong)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerGetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phong)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerGetGroupBounds(TQ3ViewerObject theViewer, TQ3BoundingBox* bounds)
{

	return((TQ3Status)-4);  // unimplemented
}



//=============================================================================
//      Windows and (other non-Mac) QD3D Viewer APIs
//-----------------------------------------------------------------------------
#pragma mark -
#pragma mark old non-Mac APIs
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

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerWriteFile(TQ3ViewerObject theViewer, HANDLE fileHandle)
{

	return((TQ3Status)-4);  // unimplemented
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

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerSetBounds(TQ3ViewerObject theViewer, RECT *bounds)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerGetBounds(TQ3ViewerObject theViewer, RECT *bounds)
{

	return((TQ3Status)-4);  // unimplemented
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

#else		// non-Mac, non-Win32

// NOTE: there's really no reason for these non-Mac, non-Win32 old APIs to exist
//		 anymore; new platforms should use the new unified Viewer APIs.

TQ3ViewerObject
E3ViewerNew(void* theWindow, const TQ3Area *rect, TQ3Uns32 flags)
{

	return(NULL);  // unimplemented
}



TQ3Status	
E3ViewerUseFile(TQ3ViewerObject theViewer, void *fileRef)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerWriteFile(TQ3ViewerObject theViewer, void *fileRef)
{

	return((TQ3Status)-4);  // unimplemented
}


void*		
E3ViewerGetPicture(TQ3ViewerObject theViewer)
{

	return(NULL);
}



TQ3Status	
E3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, TQ3Area *rect)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerSetBounds(TQ3ViewerObject theViewer, TQ3Area *bounds)
{

	return((TQ3Status)-4);  // unimplemented
}


TQ3Status	
E3ViewerGetBounds(TQ3ViewerObject theViewer, TQ3Area *bounds)
{

	return((TQ3Status)-4);  // unimplemented
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

#endif // !QUESA_SUPPORT_HITOOLBOX



/*  NAME:
        E3Viewer.h

    DESCRIPTION:
        Header file for E3Viewer.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#include <QuesaViewer.h>




//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//      New, unified Quesa Viewer API
//-----------------------------------------------------------------------------
#pragma mark new unified APIs

TQ3Status			E3Viewer_RegisterClass(void);
TQ3Status			E3Viewer_UnregisterClass(void);

TQ3Status			E3Viewer_GetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision);
TQ3Status			E3Viewer_GetReleaseVersion(TQ3Uns32 *releaseRevision);
TQ3ViewerObject		E3Viewer_New(const void *theWindow, const TQ3Area *theRect, TQ3Uns32 theFlags);
TQ3Status			E3Viewer_Dispose(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_UseFile(TQ3ViewerObject theViewer, TQ3Uns32 fileRef);
TQ3Status			E3Viewer_UseData(TQ3ViewerObject theViewer, const void *theData, TQ3Uns32 dataSize);
TQ3Status			E3Viewer_UseGroup(TQ3ViewerObject theViewer, TQ3GroupObject theGroup);
TQ3GroupObject		E3Viewer_GetGroup(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_WriteFile(TQ3ViewerObject theViewer, TQ3Uns32 fileRef);
TQ3Status			E3Viewer_WriteData(TQ3ViewerObject theViewer, void **theData, TQ3Uns32 *dataSize);
TQ3Status			E3Viewer_Draw(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_DrawContent(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_DrawControlStrip(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_GetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 theButton, TQ3Area *theRect);
TQ3Uns32			E3Viewer_GetCurrentButton(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_SetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 theButton);
TQ3Status			E3Viewer_GetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *theColor);
TQ3Status			E3Viewer_SetBackgroundColor(TQ3ViewerObject theViewer, const TQ3ColorARGB *theColor);
TQ3ViewObject		E3Viewer_GetView(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_RestoreView(TQ3ViewerObject theViewer);
TQ3Uns32			E3Viewer_GetFlags(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_SetFlags(TQ3ViewerObject theViewer, TQ3Uns32 theFlags);
TQ3Status			E3Viewer_GetBounds(TQ3ViewerObject theViewer, TQ3Area *theRect);
TQ3Status			E3Viewer_SetBounds(TQ3ViewerObject theViewer, const TQ3Area *theRect);
TQ3Status			E3Viewer_GetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *theWidth, TQ3Uns32 *theHeight);
TQ3Status			E3Viewer_SetDimension(TQ3ViewerObject theViewer, TQ3Uns32 theWidth, TQ3Uns32 theHeight);
TQ3Status			E3Viewer_GetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *theWidth, TQ3Uns32 *theHeight);
void *				E3Viewer_GetWindow(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_SetWindow(TQ3ViewerObject theViewer, const void *theWindow);
TQ3ViewerObject		E3Viewer_GetViewer(const void *theWindow);
void *				E3Viewer_GetControlStripWindow(TQ3ViewerObject theViewer);
TQ3Boolean			E3Viewer_AdjustCursor(TQ3ViewerObject theViewer, TQ3Uns32 hPos, TQ3Uns32 vPos);
TQ3Status			E3Viewer_CursorChanged(TQ3ViewerObject theViewer);
TQ3Uns32			E3Viewer_GetState(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_EditCut(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_EditCopy(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_EditPaste(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_EditClear(TQ3ViewerObject theViewer);
TQ3Status			E3Viewer_EditUndo(TQ3ViewerObject theViewer);
TQ3Boolean			E3Viewer_GetUndoString(TQ3ViewerObject theViewer, char *theBuffer, TQ3Uns32 *bufferSize);
TQ3Status			E3Viewer_GetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cameraCount);
TQ3Status			E3Viewer_SetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraIndex);
TQ3Status			E3Viewer_SetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType);
TQ3Boolean			E3Viewer_EventMouseDown(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos);
TQ3Boolean			E3Viewer_EventMouseTrack(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos);
TQ3Boolean			E3Viewer_EventMouseUp(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos);
TQ3Boolean			E3Viewer_EventKeyboard(TQ3ViewerObject theViewer, const void *theEvent);
TQ3Status			E3Viewer_GetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType);
TQ3Status			E3Viewer_SetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType);
TQ3Status			E3Viewer_GetBrightness(TQ3ViewerObject theViewer, float *theBrightness);
TQ3Status			E3Viewer_SetBrightness(TQ3ViewerObject theViewer, float theBrightness);
TQ3Status			E3Viewer_GetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *removeBackfaces);
TQ3Status			E3Viewer_SetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean removeBackfaces);
TQ3Status			E3Viewer_GetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phongShading);
TQ3Status			E3Viewer_SetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phongShading);
void *				E3Viewer_GetImage(TQ3ViewerObject theViewer);

TQ3ViewerDrawCallbackMethod				E3Viewer_GetCallbackDraw(TQ3ViewerObject theViewer);
TQ3Status								E3Viewer_SetCallbackDraw(TQ3ViewerObject theViewer, TQ3ViewerDrawCallbackMethod theCallback, const void *userData);
TQ3ViewerWindowResizeCallbackMethod		E3Viewer_GetCallbackResize(TQ3ViewerObject theViewer);
TQ3Status								E3Viewer_SetCallbackResize(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod theCallback, const void *userData);
TQ3ViewerPaneResizeNotifyCallbackMethod	E3Viewer_GetCallbackResizeNotify(TQ3ViewerObject theViewer);
TQ3Status								E3Viewer_SetCallbackResizeNotify(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod theCallback, const void *userData);





//=============================================================================
//      Function prototypes
//      Old MacOS QD3D Viewer API
//-----------------------------------------------------------------------------
#pragma mark old Mac APIs

#if QUESA_OS_MACINTOSH

OSErr			E3ViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision);
OSErr			E3ViewerGetReleaseVersion(TQ3Uns32 *releaseRevision);
TQ3ViewerObject	E3ViewerNew(CGrafPtr port, Rect *rect, TQ3Uns32 flags);
OSErr			E3ViewerDispose(TQ3ViewerObject theViewer);
OSErr			E3ViewerUseFile(TQ3ViewerObject theViewer, TQ3Int32 refNum);
OSErr			E3ViewerUseData(TQ3ViewerObject theViewer, void *data, TQ3Int32 size);
OSErr			E3ViewerWriteFile(TQ3ViewerObject theViewer, TQ3Int32 refNum);
TQ3Uns32		E3ViewerWriteData(TQ3ViewerObject theViewer, Handle data);
OSErr			E3ViewerDraw(TQ3ViewerObject theViewer);
OSErr			E3ViewerDrawContent(TQ3ViewerObject theViewer);
OSErr			E3ViewerDrawControlStrip(TQ3ViewerObject theViewer);
Boolean			E3ViewerEvent(TQ3ViewerObject theViewer, EventRecord *evt);
PicHandle		E3ViewerGetPict(TQ3ViewerObject theViewer);
OSErr			E3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, Rect *rect);
TQ3Uns32		E3ViewerGetCurrentButton(TQ3ViewerObject theViewer);
OSErr			E3ViewerSetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 button);
OSErr			E3ViewerUseGroup(TQ3ViewerObject theViewer, TQ3GroupObject group);
TQ3GroupObject	E3ViewerGetGroup(TQ3ViewerObject theViewer);
OSErr			E3ViewerSetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color);
OSErr			E3ViewerGetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color);
TQ3ViewObject	E3ViewerGetView(TQ3ViewerObject theViewer);
OSErr			E3ViewerRestoreView(TQ3ViewerObject theViewer);
OSErr			E3ViewerSetFlags(TQ3ViewerObject theViewer, TQ3Uns32 flags);
TQ3Uns32		E3ViewerGetFlags(TQ3ViewerObject theViewer);
OSErr			E3ViewerSetBounds(TQ3ViewerObject theViewer, Rect *bounds);
OSErr			E3ViewerGetBounds(TQ3ViewerObject theViewer, Rect *bounds);
OSErr			E3ViewerSetDimension(TQ3ViewerObject theViewer, TQ3Uns32 width, TQ3Uns32 height);
OSErr			E3ViewerGetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height);
OSErr			E3ViewerGetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height);
OSErr			E3ViewerSetPort(TQ3ViewerObject theViewer, CGrafPtr port);
CGrafPtr		E3ViewerGetPort(TQ3ViewerObject theViewer);
Boolean			E3ViewerAdjustCursor(TQ3ViewerObject theViewer, Point *pt);
OSErr			E3ViewerCursorChanged(TQ3ViewerObject theViewer);
TQ3Uns32		E3ViewerGetState(TQ3ViewerObject theViewer);
OSErr			E3ViewerClear(TQ3ViewerObject theViewer);
OSErr			E3ViewerCut(TQ3ViewerObject theViewer);
OSErr			E3ViewerCopy(TQ3ViewerObject theViewer);
OSErr			E3ViewerPaste(TQ3ViewerObject theViewer);
Boolean			E3ViewerMouseDown(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
Boolean			E3ViewerContinueTracking(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
Boolean			E3ViewerMouseUp(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
Boolean			E3ViewerHandleKeyEvent(TQ3ViewerObject theViewer, EventRecord *evt);
OSErr			E3ViewerSetDrawingCallbackMethod(TQ3ViewerObject theViewer, TQ3ViewerDrawingCallbackMethod callbackMethod, const void *data);
OSErr			E3ViewerSetWindowResizeCallback(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod windowResizeCallbackMethod, const void *data);
OSErr			E3ViewerSetPaneResizeNotifyCallback(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod paneResizeNotifyCallbackMethod, const void *data);
OSErr			E3ViewerUndo(TQ3ViewerObject theViewer);
Boolean			E3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, TQ3Uns32 *cnt);
OSErr			E3ViewerGetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cnt);
OSErr			E3ViewerSetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraNo);
OSErr			E3ViewerSetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType);
OSErr			E3ViewerSetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType);
OSErr			E3ViewerGetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType);
OSErr			E3ViewerChangeBrightness(TQ3ViewerObject theViewer, float brightness);
OSErr			E3ViewerSetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean remove);
OSErr			E3ViewerGetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *remove);
OSErr			E3ViewerSetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phong);
OSErr			E3ViewerGetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phong);

#endif





//=============================================================================
//      Function prototypes
//      Windows versions of old cross-platform QD3D Viewer APIs
//-----------------------------------------------------------------------------
#pragma mark old common APIs

#if QUESA_OS_WIN32

TQ3Status		E3ViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision);
TQ3Status		E3ViewerGetReleaseVersion(TQ3Uns32 *releaseRevision);
TQ3Status		E3ViewerDispose(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerUseData(TQ3ViewerObject theViewer, void *data, TQ3Int32 size);
TQ3Status		E3ViewerWriteData(TQ3ViewerObject theViewer, void *data, TQ3Uns32 dataSize, TQ3Uns32 *actualDataSize);
TQ3Status		E3ViewerDraw(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerDrawContent(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerDrawControlStrip(TQ3ViewerObject theViewer);
TQ3Uns32		E3ViewerGetCurrentButton(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerSetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 button);
TQ3Status		E3ViewerUseGroup(TQ3ViewerObject theViewer, TQ3GroupObject group);
TQ3GroupObject	E3ViewerGetGroup(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerSetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color);
TQ3Status		E3ViewerGetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color);
TQ3ViewObject	E3ViewerGetView(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerRestoreView(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerSetFlags(TQ3ViewerObject theViewer, TQ3Uns32 flags);
TQ3Uns32		E3ViewerGetFlags(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerSetDimension(TQ3ViewerObject theViewer, TQ3Uns32 width, TQ3Uns32 height);
TQ3Status		E3ViewerGetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height);
TQ3Status		E3ViewerGetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height);
TQ3Boolean		E3ViewerAdjustCursor(TQ3ViewerObject theViewer, TQ3Int32 x , TQ3Int32 y);
TQ3Status		E3ViewerCursorChanged(TQ3ViewerObject theViewer);
TQ3Uns32		E3ViewerGetState(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerClear(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerCut(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerCopy(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerPaste(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerSetDrawingCallbackMethod(TQ3ViewerObject theViewer, TQ3ViewerDrawingCallbackMethod callbackMethod, const void *data);
TQ3Status		E3ViewerSetWindowResizeCallback(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod windowResizeCallbackMethod, const void *data);
TQ3Status		E3ViewerSetPaneResizeNotifyCallback(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod paneResizeNotifyCallbackMethod, const void *data);
TQ3Status		E3ViewerUndo(TQ3ViewerObject theViewer);
TQ3Boolean		E3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, TQ3Uns32 stringSize, TQ3Uns32 *actualSize);
TQ3Status		E3ViewerGetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cnt);
TQ3Status		E3ViewerSetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraNo);
TQ3Status		E3ViewerSetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType);
TQ3Status		E3ViewerSetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType);
TQ3Status		E3ViewerGetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType);
TQ3Status		E3ViewerChangeBrightness(TQ3ViewerObject theViewer, float brightness);
TQ3Status		E3ViewerSetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean remove);
TQ3Status		E3ViewerGetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *remove);
TQ3Status		E3ViewerSetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phong);
TQ3Status		E3ViewerGetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phong);
TQ3Status		E3ViewerGetGroupBounds(TQ3ViewerObject theViewer, TQ3BoundingBox* bounds);

//=============================================================================
//      Function prototypes
//      Old non-Mac QD3D Viewer APIs
//-----------------------------------------------------------------------------
#pragma mark old non-Mac APIs

	#if QUESA_OS_WIN32

TQ3ViewerObject	E3ViewerNew(HWND theWindow, const RECT *rect, TQ3Uns32 flags);
TQ3Status		E3ViewerUseFile(TQ3ViewerObject theViewer, HANDLE fileHandle);
TQ3Status		E3ViewerWriteFile(TQ3ViewerObject theViewer, HANDLE fileHandle);

HBITMAP			E3ViewerGetBitmap(TQ3ViewerObject theViewer);
HWND			E3ViewerGetWindow(TQ3ViewerObject theViewer);
TQ3Status		E3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, RECT *rect);
TQ3Status		E3ViewerSetBounds(TQ3ViewerObject theViewer, RECT *bounds);
TQ3Status		E3ViewerGetBounds(TQ3ViewerObject theViewer, RECT *bounds);

BOOL			E3ViewerMouseDown(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
BOOL			E3ViewerContinueTracking(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
BOOL			E3ViewerMouseUp(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
	#else

TQ3ViewerObject	E3ViewerNew(void* theWindow, const TQ3Area *rect, TQ3Uns32 flags);

TQ3Status		E3ViewerUseFile(TQ3ViewerObject theViewer, void *fileRef);
TQ3Status		E3ViewerWriteFile(TQ3ViewerObject theViewer, void *fileRef);
void*			E3ViewerGetPicture(TQ3ViewerObject theViewer);

TQ3Status		E3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, TQ3Area *rect);
TQ3Status		E3ViewerSetBounds(TQ3ViewerObject theViewer, TQ3Area *bounds);
TQ3Status		E3ViewerGetBounds(TQ3ViewerObject theViewer, TQ3Area *bounds);

TQ3Boolean		E3ViewerMouseDown(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
TQ3Boolean		E3ViewerContinueTracking(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);
TQ3Boolean		E3ViewerMouseUp(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y);

	#endif
#endif





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        E3Viewer.h

    DESCRIPTION:
        Header file for E3Viewer.c.

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
#ifndef E3VIEWER_HDR
#define E3VIEWER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
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
TQ3ViewerPaneResizeNotifyCallbackMethod	E3Viewer_GetCallbackResize(TQ3ViewerObject theViewer);
TQ3Status								E3Viewer_SetCallbackResize(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod theCallback, const void *userData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


/*  NAME:
        E3WinViewer.h

    DESCRIPTION:
        Header file for E3WinViewer.c.

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
#ifndef E3WINVIEWER_HDR
#define E3WINVIEWER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here


#include "QuesaViewer.h"


//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3WinViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision);
TQ3Status			E3WinViewerGetReleaseVersion(TQ3Uns32 *releaseRevision);
TQ3ViewerObject		E3WinViewerNew(HWND window, const RECT *rect, TQ3Uns32 flags);
TQ3Status			E3WinViewerDispose(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerUseFile(TQ3ViewerObject viewer, HANDLE fileHandle);
TQ3Status			E3WinViewerUseData(TQ3ViewerObject viewer, void *data, TQ3Uns32 size);
TQ3Status			E3WinViewerWriteFile(TQ3ViewerObject viewer, HANDLE fileHandle);
TQ3Status			E3WinViewerWriteData(TQ3ViewerObject viewer, void *data, TQ3Uns32 dataSize, TQ3Uns32 *actualDataSize);
TQ3Status			E3WinViewerDraw(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerDrawContent(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerDrawControlStrip(TQ3ViewerObject viewer);
BOOL				E3WinViewerMouseDown(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y);
BOOL				E3WinViewerContinueTracking(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y);
BOOL				E3WinViewerMouseUp(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y);
HBITMAP				E3WinViewerGetBitmap(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerGetButtonRect(TQ3ViewerObject viewer, TQ3Uns32 button, RECT *rectangle);
TQ3Uns32			E3WinViewerGetCurrentButton(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerSetCurrentButton(TQ3ViewerObject viewer, TQ3Uns32 button);
TQ3Status			E3WinViewerUseGroup(TQ3ViewerObject viewer, TQ3GroupObject group);
TQ3GroupObject		E3WinViewerGetGroup(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerSetBackgroundColor(TQ3ViewerObject viewer, TQ3ColorARGB *color);
TQ3Status			E3WinViewerGetBackgroundColor(TQ3ViewerObject viewer, TQ3ColorARGB *color);
TQ3ViewObject		E3WinViewerGetView(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerRestoreView(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerSetFlags(TQ3ViewerObject viewer, TQ3Uns32 flags);
TQ3Uns32			E3WinViewerGetFlags(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerSetBounds(TQ3ViewerObject viewer, RECT *bounds);
TQ3Status			E3WinViewerGetBounds(TQ3ViewerObject viewer, RECT *bounds);
TQ3Status			E3WinViewerSetDimension(TQ3ViewerObject viewer, TQ3Uns32 width, TQ3Uns32 height);
TQ3Status			E3WinViewerGetDimension(TQ3ViewerObject viewer, TQ3Uns32 *width, TQ3Uns32 *height);
TQ3Status			E3WinViewerGetMinimumDimension(TQ3ViewerObject viewer, TQ3Uns32 *width, TQ3Uns32 *height);
TQ3Status			E3WinViewerSetWindow(TQ3ViewerObject viewer, HWND window);
HWND				E3WinViewerGetWindow(TQ3ViewerObject viewer);
TQ3ViewerObject		E3WinViewerGetViewer(HWND theWindow);
HWND				E3WinViewerGetControlStrip(TQ3ViewerObject viewer);
TQ3Boolean			E3WinViewerAdjustCursor(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y);
TQ3Status			E3WinViewerCursorChanged(TQ3ViewerObject viewer);
TQ3Uns32			E3WinViewerGetState(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerClear(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerCut(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerCopy(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerPaste(TQ3ViewerObject viewer);
TQ3Status			E3WinViewerUndo(TQ3ViewerObject viewer);
TQ3Boolean			E3WinViewerGetUndoString(TQ3ViewerObject viewer, char *theString, TQ3Uns32 stringSize, TQ3Uns32 *actualSize);
TQ3Status			E3WinViewerGetCameraCount(TQ3ViewerObject viewer, TQ3Uns32 *count);
TQ3Status			E3WinViewerSetCameraNumber(TQ3ViewerObject viewer, TQ3Uns32 cameraNo);
TQ3Status			E3WinViewerSetCameraView(TQ3ViewerObject viewer, TQ3ViewerCameraView viewType);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


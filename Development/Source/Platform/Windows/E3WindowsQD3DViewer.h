/*  NAME:
        E3WindowsQD3DViewer.h

    DESCRIPTION:
        Header file for E3WindowsQD3DViewer.c.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
#ifndef E3WINDOWSQD3DVIEWER_HDR
#define E3WINDOWSQD3DVIEWER_HDR
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

TQ3ViewerObject		E3WinViewerNew(HWND window,  const RECT *rect, unsigned long flags);
TQ3Status			E3WinViewerRegisterWindowClasses(HINSTANCE hInstance);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


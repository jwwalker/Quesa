/*  NAME:
        E3ViewerOldAPIs.c

    DESCRIPTION:
        Part of the Quesa viewer library.  This file implements the old,
        mostly platform-specific APIs that were defined in QD3D.  These
        implementations just do a bit of constant-mapping and then call
        through to the newer, unified Quesa APIs.

    COPYRIGHT:
        Quesa Copyright � 1999-2002, Quesa Developers.
        
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
#include "E3Viewer.h"

#pragma mark new unified APIs
#pragma mark -

#if QUESA_OS_MACINTOSH

OSErr		
E3ViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerGetReleaseVersion(TQ3Uns32 *releaseRevision)
{

	return(-4);  // unimplemented
}


TQ3ViewerObject
E3ViewerNew(CGrafPtr port, Rect *rect, TQ3Uns32 flags)
{

	return(NULL);  // unimplemented
}


OSErr		
E3ViewerDispose(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerUseFile(TQ3ViewerObject theViewer, TQ3Int32 refNum)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerUseData(TQ3ViewerObject theViewer, void *data, TQ3Int32 size)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerWriteFile(TQ3ViewerObject theViewer, TQ3Int32 refNum)
{

	return(-4);  // unimplemented
}


TQ3Uns32	
E3ViewerWriteData(TQ3ViewerObject theViewer, Handle data)
{

	return(0); // unimplemented
}


OSErr		
E3ViewerDraw(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerDrawContent(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerDrawControlStrip(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


Boolean		
E3ViewerEvent(TQ3ViewerObject theViewer, EventRecord *evt)
{

	return(false);  // unimplemented
}


PicHandle	
E3ViewerGetPict(TQ3ViewerObject theViewer)
{

	return(NULL);  // unimplemented
}


OSErr		
E3ViewerGetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 button, Rect *rect)
{

	return(-4);  // unimplemented
}


TQ3Uns32	
E3ViewerGetCurrentButton(TQ3ViewerObject theViewer)
{

	return(0); // unimplemented
}


OSErr		
E3ViewerSetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 button)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerUseGroup(TQ3ViewerObject theViewer, TQ3GroupObject group)
{

	return(-4);  // unimplemented
}


TQ3GroupObject
E3ViewerGetGroup(TQ3ViewerObject theViewer)
{

	return(NULL);  // unimplemented
}


OSErr		
E3ViewerSetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerGetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
{

	return(-4);  // unimplemented
}


TQ3ViewObject
E3ViewerGetView(TQ3ViewerObject theViewer)
{

	return(NULL);  // unimplemented
}


OSErr		
E3ViewerRestoreView(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerSetFlags(TQ3ViewerObject theViewer, TQ3Uns32 flags)
{

	return(-4);  // unimplemented
}


TQ3Uns32	
E3ViewerGetFlags(TQ3ViewerObject theViewer)
{

	return(0); // unimplemented
}


OSErr		
E3ViewerSetBounds(TQ3ViewerObject theViewer, Rect *bounds)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerGetBounds(TQ3ViewerObject theViewer, Rect *bounds)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerSetDimension(TQ3ViewerObject theViewer, TQ3Uns32 width, TQ3Uns32 height)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerGetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerGetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *width, TQ3Uns32 *height)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerSetPort(TQ3ViewerObject theViewer, CGrafPtr port)
{

	return(-4);  // unimplemented
}


CGrafPtr	
E3ViewerGetPort(TQ3ViewerObject theViewer)
{

	return(NULL);  // unimplemented
}


Boolean		
E3ViewerAdjustCursor(TQ3ViewerObject theViewer, Point *pt)
{

	return(false);  // unimplemented
}


OSErr		
E3ViewerCursorChanged(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


TQ3Uns32	
E3ViewerGetState(TQ3ViewerObject theViewer)
{

	return(0); // unimplemented
}


OSErr		
E3ViewerClear(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerCut(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerCopy(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerPaste(TQ3ViewerObject theViewer)
{

	return(-4);  // unimplemented
}


Boolean		
E3ViewerMouseDown(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{

	return(false);  // unimplemented
}


Boolean		
E3ViewerContinueTracking(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{

	return(false);  // unimplemented
}


Boolean		
E3ViewerMouseUp(TQ3ViewerObject theViewer, TQ3Int32 x, TQ3Int32 y)
{

	return(false);  // unimplemented
}


Boolean		
E3ViewerHandleKeyEvent(TQ3ViewerObject theViewer, EventRecord *evt)
{

	return(false);  // unimplemented
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

	return(-4);  // unimplemented
}


Boolean		
E3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, TQ3Uns32 *cnt)
{

	return(false);  // unimplemented
}


OSErr		
E3ViewerGetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cnt)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerSetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraNo)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerSetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerSetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerGetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerChangeBrightness(TQ3ViewerObject theViewer, float brightness)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerSetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean remove)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerGetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *remove)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerSetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phong)
{

	return(-4);  // unimplemented
}


OSErr		
E3ViewerGetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phong)
{

	return(-4);  // unimplemented
}



#else // QUESA_OS_MACINTOSH

//=============================================================================
//      Function prototypes
//      Old cross-platform QD3D Viewer APIs
//-----------------------------------------------------------------------------
#pragma mark -
#pragma mark old common APIs
#pragma mark -

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



//=============================================================================
//      Function prototypes
//      Old non-Mac QD3D Viewer APIs
//-----------------------------------------------------------------------------
#pragma mark -
#pragma mark old non-Mac APIs
#pragma mark -

	#if defined(QUESA_OS_WIN32) && QUESA_OS_WIN32

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


	#else

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



	#endif
#endif


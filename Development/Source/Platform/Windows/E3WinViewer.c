/*  NAME:
        E3WinViewer.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
#include "E3Prefix.h"
#include "E3WinViewer.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3WinViewerGetVersion : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3WinViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetReleaseVersion : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetReleaseVersion(TQ3Uns32 *releaseRevision)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerNew : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewerObject
E3WinViewerNew(HWND window, const RECT *rect, TQ3Uns32 flags)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3WinViewerDispose : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerDispose(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerUseFile : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerUseFile(TQ3ViewerObject viewer, HANDLE fileHandle)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerUseData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerUseData(TQ3ViewerObject viewer, void *data, TQ3Uns32 size)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerWriteFile : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerWriteFile(TQ3ViewerObject viewer, HANDLE fileHandle)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerWriteData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerWriteData(TQ3ViewerObject viewer, void *data, TQ3Uns32 dataSize, TQ3Uns32 *actualDataSize)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerDraw : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerDraw(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerDrawContent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerDrawContent(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerDrawControlStrip : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerDrawControlStrip(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerMouseDown : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
BOOL
E3WinViewerMouseDown(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y)
{


	// To be implemented...
	return(FALSE);
}





//=============================================================================
//      E3WinViewerContinueTracking : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
BOOL
E3WinViewerContinueTracking(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y)
{


	// To be implemented...
	return(FALSE);
}





//=============================================================================
//      E3WinViewerMouseUp : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
BOOL
E3WinViewerMouseUp(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y)
{


	// To be implemented...
	return(FALSE);
}





//=============================================================================
//      E3WinViewerGetBitmap : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
HBITMAP
E3WinViewerGetBitmap(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3WinViewerGetButtonRect : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetButtonRect(TQ3ViewerObject viewer, TQ3Uns32 button, RECT *rectangle)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetCurrentButton : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Uns32
E3WinViewerGetCurrentButton(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(0);
}





//=============================================================================
//      E3WinViewerSetCurrentButton : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetCurrentButton(TQ3ViewerObject viewer, TQ3Uns32 button)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerUseGroup : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerUseGroup(TQ3ViewerObject viewer, TQ3GroupObject group)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetGroup : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3GroupObject
E3WinViewerGetGroup(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3WinViewerSetBackgroundColor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetBackgroundColor(TQ3ViewerObject viewer, TQ3ColorARGB *color)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetBackgroundColor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetBackgroundColor(TQ3ViewerObject viewer, TQ3ColorARGB *color)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetView : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewObject
E3WinViewerGetView(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3WinViewerRestoreView : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerRestoreView(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerSetFlags : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetFlags(TQ3ViewerObject viewer, TQ3Uns32 flags)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetFlags : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Uns32
E3WinViewerGetFlags(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(0);
}





//=============================================================================
//      E3WinViewerSetBounds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetBounds(TQ3ViewerObject viewer, RECT *bounds)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetBounds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetBounds(TQ3ViewerObject viewer, RECT *bounds)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerSetDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetDimension(TQ3ViewerObject viewer, TQ3Uns32 width, TQ3Uns32 height)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetDimension(TQ3ViewerObject viewer, TQ3Uns32 *width, TQ3Uns32 *height)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetMinimumDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetMinimumDimension(TQ3ViewerObject viewer, TQ3Uns32 *width, TQ3Uns32 *height)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerSetWindow : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetWindow(TQ3ViewerObject viewer, HWND window)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetWindow : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
HWND
E3WinViewerGetWindow(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3WinViewerGetViewer : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewerObject
E3WinViewerGetViewer(HWND theWindow)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3WinViewerGetControlStrip : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
HWND
E3WinViewerGetControlStrip(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3WinViewerAdjustCursor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3WinViewerAdjustCursor(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3WinViewerCursorChanged : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerCursorChanged(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetState : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Uns32
E3WinViewerGetState(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(0);
}





//=============================================================================
//      E3WinViewerClear : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerClear(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerCut : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerCut(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerCopy : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerCopy(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerPaste : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerPaste(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerUndo : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerUndo(TQ3ViewerObject viewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetUndoString : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3WinViewerGetUndoString(TQ3ViewerObject viewer, char *theString, TQ3Uns32 stringSize, TQ3Uns32 *actualSize)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3WinViewerGetCameraCount : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetCameraCount(TQ3ViewerObject viewer, TQ3Uns32 *count)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerSetCameraNumber : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetCameraNumber(TQ3ViewerObject viewer, TQ3Uns32 cameraNo)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3WinViewerSetCameraView : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetCameraView(TQ3ViewerObject viewer, TQ3ViewerCameraView viewType)
{


	// To be implemented...
	return(kQ3Failure);
}






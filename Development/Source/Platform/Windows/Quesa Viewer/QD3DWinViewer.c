/*  NAME:
        QD3DWinViewer.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

		In many cases the Q3Winxxx - which implements the general
		behaviour - is called directly. If there is some need to
		call a Windows API function it is preferable to call
		the E3Winxxx routine, which is exclusively Windows,
		than to decorate E3Viewer.c with #ifdefs.

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
#include "QuesaViewer.h"
#include "E3Viewer.h"
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
//      Q3WinViewerGetVersion : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
Q3WinViewerGetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRevision), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRevision), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on majorRevision
		return(kQ3Failure);

	if (0) // Further checks on minorRevision
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetVersion(majorRevision, minorRevision));
}





//=============================================================================
//      Q3WinViewerGetReleaseVersion : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetReleaseVersion(TQ3Uns32 *releaseRevision)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(releaseRevision), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on releaseRevision
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetReleaseVersion(releaseRevision));
}





//=============================================================================
//      Q3WinViewerNew : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewerObject
Q3WinViewerNew(HWND window, const RECT *rect, TQ3Uns32 flags)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rect), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on window
		return(NULL);

	if (0) // Further checks on rect
		return(NULL);

	if (0) // Further checks on flags
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerNew(window, rect, flags));
}





//=============================================================================
//      Q3WinViewerDispose : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerDispose(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerDispose(viewer));
}





//=============================================================================
//      Q3WinViewerUseFile : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerUseFile(TQ3ViewerObject viewer, HANDLE fileHandle)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on fileHandle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerUseFile(viewer, fileHandle));
}





//=============================================================================
//      Q3WinViewerUseData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerUseData(TQ3ViewerObject viewer, void *data, TQ3Uns32 size)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on size
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerUseData(viewer, data, size));
}





//=============================================================================
//      Q3WinViewerWriteFile : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerWriteFile(TQ3ViewerObject viewer, HANDLE fileHandle)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on fileHandle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerWriteFile(viewer, fileHandle));
}





//=============================================================================
//      Q3WinViewerWriteData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerWriteData(TQ3ViewerObject viewer, void *data, TQ3Uns32 dataSize, TQ3Uns32 *actualDataSize)
{


	// Release build checks
	// The following requirements are not quite true:
	// The original implementation allows zeros as a shortcut for
	// finding the final data size.
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(actualDataSize), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on dataSize
		return(kQ3Failure);

	if (0) // Further checks on actualDataSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerWriteData(viewer, data, dataSize, actualDataSize));
}





//=============================================================================
//      Q3WinViewerDraw : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerDraw(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerDraw(viewer));
}





//=============================================================================
//      Q3WinViewerDrawContent : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerDrawContent(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerDrawContent(viewer));
}





//=============================================================================
//      Q3WinViewerDrawControlStrip : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerDrawControlStrip(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerDrawControlStrip(viewer));
}





//=============================================================================
//      Q3WinViewerMouseDown : Quesa API entry point.
//-----------------------------------------------------------------------------
BOOL
Q3WinViewerMouseDown(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(FALSE);

	if (0) // Further checks on x
		return(FALSE);

	if (0) // Further checks on y
		return(FALSE);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerMouseDown(viewer, x, y));
}





//=============================================================================
//      Q3WinViewerContinueTracking : Quesa API entry point.
//-----------------------------------------------------------------------------
BOOL
Q3WinViewerContinueTracking(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(FALSE);

	if (0) // Further checks on x
		return(FALSE);

	if (0) // Further checks on y
		return(FALSE);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerContinueTracking(viewer, x, y));
}





//=============================================================================
//      Q3WinViewerMouseUp : Quesa API entry point.
//-----------------------------------------------------------------------------
BOOL
Q3WinViewerMouseUp(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(FALSE);

	if (0) // Further checks on x
		return(FALSE);

	if (0) // Further checks on y
		return(FALSE);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerMouseUp(viewer, x, y));
}





//=============================================================================
//      Q3WinViewerGetBitmap : Quesa API entry point.
//-----------------------------------------------------------------------------
HBITMAP
Q3WinViewerGetBitmap(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetBitmap(viewer));
}





//=============================================================================
//      Q3WinViewerGetButtonRect : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetButtonRect(TQ3ViewerObject viewer, TQ3Uns32 button, RECT *rectangle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rectangle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on button
		return(kQ3Failure);

	if (0) // Further checks on rectangle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetButtonRect(viewer, button, rectangle));
}





//=============================================================================
//      Q3WinViewerGetCurrentButton : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3WinViewerGetCurrentButton(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(0);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetCurrentButton(viewer));
}





//=============================================================================
//      Q3WinViewerSetCurrentButton : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetCurrentButton(TQ3ViewerObject viewer, TQ3Uns32 button)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on button
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetCurrentButton(viewer, button));
}





//=============================================================================
//      Q3WinViewerUseGroup : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerUseGroup(TQ3ViewerObject viewer, TQ3GroupObject group)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(group->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(group, kQ3ShapeTypeGroup), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on group
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(Q3ViewerUseGroup(viewer, group));
}





//=============================================================================
//      Q3WinViewerGetGroup : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupObject
Q3WinViewerGetGroup(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(Q3ViewerGetGroup(viewer));
}





//=============================================================================
//      Q3WinViewerSetBackgroundColor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetBackgroundColor(TQ3ViewerObject viewer, TQ3ColorARGB *color)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on color
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetBackgroundColor(viewer, color));
}





//=============================================================================
//      Q3WinViewerGetBackgroundColor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetBackgroundColor(TQ3ViewerObject viewer, TQ3ColorARGB *color)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on color
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetBackgroundColor(viewer, color));
}





//=============================================================================
//      Q3WinViewerGetView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewObject
Q3WinViewerGetView(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(Q3ViewerGetView(viewer));
}





//=============================================================================
//      Q3WinViewerRestoreView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerRestoreView(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(Q3ViewerRestoreView(viewer));
}





//=============================================================================
//      Q3WinViewerSetFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetFlags(TQ3ViewerObject viewer, TQ3Uns32 flags)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on flags
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetFlags(viewer, flags));
}





//=============================================================================
//      Q3WinViewerGetFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3WinViewerGetFlags(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(0);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetFlags(viewer));
}





//=============================================================================
//      Q3WinViewerSetBounds : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetBounds(TQ3ViewerObject viewer, RECT *bounds)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bounds), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on bounds
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetBounds(viewer, bounds));
}





//=============================================================================
//      Q3WinViewerGetBounds : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetBounds(TQ3ViewerObject viewer, RECT *bounds)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bounds), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on bounds
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetBounds(viewer, bounds));
}





//=============================================================================
//      Q3WinViewerSetDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetDimension(TQ3ViewerObject viewer, TQ3Uns32 width, TQ3Uns32 height)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on width
		return(kQ3Failure);

	if (0) // Further checks on height
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetDimension(viewer, width, height));
}





//=============================================================================
//      Q3WinViewerGetDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetDimension(TQ3ViewerObject viewer, TQ3Uns32 *width, TQ3Uns32 *height)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(width), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(height), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on width
		return(kQ3Failure);

	if (0) // Further checks on height
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetDimension(viewer, width, height));
}





//=============================================================================
//      Q3WinViewerGetMinimumDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetMinimumDimension(TQ3ViewerObject viewer, TQ3Uns32 *width, TQ3Uns32 *height)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(width), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(height), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on width
		return(kQ3Failure);

	if (0) // Further checks on height
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return (E3WinViewerGetMinimumDimension(viewer, width, height));
}





//=============================================================================
//      Q3WinViewerSetWindow : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetWindow(TQ3ViewerObject viewer, HWND window)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on window
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetWindow(viewer, window));
}





//=============================================================================
//      Q3WinViewerGetWindow : Quesa API entry point.
//-----------------------------------------------------------------------------
HWND
Q3WinViewerGetWindow(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetWindow(viewer));
}





//=============================================================================
//      Q3WinViewerGetViewer : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewerObject
Q3WinViewerGetViewer(HWND theWindow)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theWindow
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetViewer(theWindow));
}





//=============================================================================
//      Q3WinViewerGetControlStrip : Quesa API entry point.
//-----------------------------------------------------------------------------
HWND
Q3WinViewerGetControlStrip(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetControlStrip(viewer));
}





//=============================================================================
//      Q3WinViewerAdjustCursor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3WinViewerAdjustCursor(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3False);

	if (0) // Further checks on x
		return(kQ3False);

	if (0) // Further checks on y
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(Q3ViewerAdjustCursor(viewer, x, y));
}





//=============================================================================
//      Q3WinViewerCursorChanged : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerCursorChanged(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(Q3ViewerCursorChanged(viewer));
}





//=============================================================================
//      Q3WinViewerGetState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3WinViewerGetState(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(0);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetState(viewer));
}





//=============================================================================
//      Q3WinViewerClear : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerClear(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerClear(viewer));
}





//=============================================================================
//      Q3WinViewerCut : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerCut(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(Q3ViewerCut(viewer));
}





//=============================================================================
//      Q3WinViewerCopy : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerCopy(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerCopy(viewer));
}





//=============================================================================
//      Q3WinViewerPaste : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerPaste(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerPaste(viewer));
}





//=============================================================================
//      Q3WinViewerUndo : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerUndo(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(Q3ViewerUndo(viewer));
}





//=============================================================================
//      Q3WinViewerGetUndoString : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3WinViewerGetUndoString(TQ3ViewerObject viewer, char *theString, TQ3Uns32 stringSize, TQ3Uns32 *actualSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theString), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(actualSize), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3False);

	if (0) // Further checks on theString
		return(kQ3False);

	if (0) // Further checks on stringSize
		return(kQ3False);

	if (0) // Further checks on actualSize
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(Q3ViewerGetUndoString(viewer, theString, stringSize, actualSize));
}





//=============================================================================
//      Q3WinViewerGetCameraCount : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetCameraCount(TQ3ViewerObject viewer, TQ3Uns32 *count)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(count), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on count
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetCameraCount(viewer, count));
}





//=============================================================================
//      Q3WinViewerSetCameraNumber : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetCameraNumber(TQ3ViewerObject viewer, TQ3Uns32 cameraNo)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on cameraNo
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetCameraNumber(viewer, cameraNo));
}





//=============================================================================
//      Q3WinViewerSetCameraView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetCameraView(TQ3ViewerObject viewer, TQ3ViewerCameraView viewType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on viewType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetCameraView(viewer, viewType));
}






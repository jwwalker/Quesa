/*  NAME:
        E3WindowsQD3DViewer.c

    DESCRIPTION:
        Implementation of the QD3D Viewer api for Windows.
        
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

/* To do:
		- Cursor appearance
		- badge
		- Undo
		- Cameras
		- move the rest of the unimplemented routines from QD3DWinViewer
		- implementation of the "virtual system callbacks", i.e. the E3SysViewer... routines
*/

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "../../Core/Viewer/E3Viewer.h"
#include "E3WinViewer.h"
#include <commctrl.h>




//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here


HINSTANCE gDllInstance;// the global reference to the DLL

#define ID_TOOLBAR 117
#define IDB_TOOLBAR 117



//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here


static unsigned long e3_button_item_to_quesa( TQ3ViewerObject viewer, unsigned long item )
{
	TBBUTTON info;
	LRESULT win_res= SendMessage
	( E3WinViewerGetControlStrip(viewer)
	, TB_GETBUTTON 
    , (WPARAM) item
    , (LPARAM) (LPTBBUTTON) &info
	);
	if (win_res == 0) return 0;

	return info.idCommand;
}


static unsigned long e3_button_quesa_to_item( TQ3ViewerObject viewer, unsigned long quesa )
{
	LRESULT win_res= 0;
	TBBUTTONINFO info;
		info.cbSize= sizeof(info);
		info.dwMask= 0;

	win_res= SendMessage
	( E3WinViewerGetControlStrip(viewer)
	, TB_GETBUTTONINFO
    , (WPARAM)(INT) quesa
    , (LPARAM)(LPTBBUTTONINFO) &info
	);

	return win_res;
}


static TQ3Boolean e3_is_quesa_button(unsigned long theButton, unsigned long theQuesaButton, TQ3Uns32 theFlags, TQ3Uns32* n )
{
	if (theButton == theQuesaButton)
		return(kQ3True);
	if (theFlags & theQuesaButton)
		(*n)++;
	return (kQ3False);
}


static unsigned long e3_button_quesa_to_app( TQ3ViewerObject viewer, unsigned long theButton )
{
	// Some strange thing.
	//
	// In my testing application (from Apple) one does input kQ3... buttons.
	// The Q3Viewer method however does use "application" buttons as
	// input, i.e. the numbering which the user does see on the screen.
	// Is there really such a difference between the Mac and the Win button 
	// numbering scheme? I will check it when nobody else does.
	//
	// This code tries to make the conversion. Essentially it is
	// e3_viewer_button_to_external from E3Viewer.c
	TQ3Uns32 flags= Q3WinViewerGetFlags(viewer);
	TQ3Uns32			n= 0;

#define _check(b) if (e3_is_quesa_button(theButton, b, flags, &n )) return n;
	_check(kQ3ViewerButtonCamera);
	_check(kQ3ViewerButtonTruck);
	_check(kQ3ViewerButtonOrbit);
	_check(kQ3ViewerButtonZoom);
	_check(kQ3ViewerButtonDolly);
	_check(kQ3ViewerButtonReset);
	_check(kQ3ViewerButtonOptions);
#undef _check

	return(theButton);
}


static TQ3Boolean e3_is_app_button(unsigned long theButton, unsigned long theQuesaButton, TQ3Uns32 theFlags, TQ3Uns32* n )
{
	if (theFlags & theQuesaButton)
	{
		if (theButton == *n) return(kQ3True);
		(*n)++;
	}
	return (kQ3False);
}


static unsigned long e3_button_app_to_quesa( TQ3ViewerObject viewer, unsigned long app )
{
	TQ3Uns32 flags= Q3WinViewerGetFlags(viewer);
	TQ3Uns32			n= 0;
#define _check(b) if (e3_is_app_button(app, b, flags, &n )) return b;
	_check(kQ3ViewerButtonCamera);
	_check(kQ3ViewerButtonTruck);
	_check(kQ3ViewerButtonOrbit);
	_check(kQ3ViewerButtonZoom);
	_check(kQ3ViewerButtonDolly);
	_check(kQ3ViewerButtonReset);
	_check(kQ3ViewerButtonOptions);
#undef _check

	return 0;
}



static void e3_check_button( HWND controlStrip, unsigned int idButton )
{
	SendMessage
		( controlStrip
		, TB_CHECKBUTTON 
		, (WPARAM) idButton
		, (LPARAM) MAKELONG(1, 0)
		); 
}


static void e3_show_button( HWND controlStrip, TQ3Uns32 flags, TQ3Uns32 idButton )
{
	SendMessage
		( controlStrip
		, TB_HIDEBUTTON 
		, (WPARAM) idButton
		, (LPARAM) MAKELONG((idButton & flags) == 0, 0)
		); 
}







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
	return Q3ViewerGetVersion(majorRevision,minorRevision);
}





//=============================================================================
//      E3WinViewerGetReleaseVersion : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetReleaseVersion(TQ3Uns32 *releaseRevision)
{
	return Q3ViewerGetReleaseVersion(releaseRevision);
}





//=============================================================================
//      E3WinViewerNew : Create a viewer child window.
//-----------------------------------------------------------------------------
//		Note : To fit neatly into the Windows scheme, we take the
//		input window as the parent of the viewer window.
//
//		To do:
//		-- investigation of the correct handling of the rect parameter
//-----------------------------------------------------------------------------

TQ3ViewerObject
E3WinViewerNew(HWND window, const RECT *rect, TQ3Uns32 flags)
{
	TQ3ViewerObject theViewer;

	// Create the dawing pane
	HWND thePort= CreateWindowEx
		( WS_EX_LEFT
			| WS_EX_CONTROLPARENT
			| WS_EX_RIGHTSCROLLBAR
			| WS_EX_ACCEPTFILES 
			// extended window style, from Spy
		,  kQ3ViewerClassName // pointer to registered class name
		, "" // pointer to window name
		, WS_CHILD
			| WS_VISIBLE
			| WS_CLIPSIBLINGS
			| WS_CLIPCHILDREN
			| WS_OVERLAPPED       
			// window style, from Spy
		, rect->left               // horizontal position of window
		, rect->top               // vertical position of window
		, rect->right-rect->left          // window width
		, rect->bottom-rect->top         // window height
		, window     // handle to parent or owner window
		, 0         // handle to menu, or child-window identifier
		, gDllInstance // handle to application instance
		, 0        // pointer to window-creation data
	);
 

	// The buttons of the viewer control strip
#define _button(n,q) 		{n, q, TBSTATE_ENABLED | TBSTATE_HIDDEN, TBSTYLE_BUTTON, 0L, 0}
#define _button_group(n,q) 		{n, q, TBSTATE_ENABLED | TBSTATE_HIDDEN, TBSTYLE_BUTTON|TBSTYLE_CHECKGROUP, 0L, 0}	
#define _separator()  {7, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0L, 0}
	TBBUTTON tbButtons [] = 
	{
		_separator(),
		_button(0,kQ3ViewerButtonCamera),
		_separator(),
		_button_group(1, kQ3ViewerButtonTruck),
		_button_group(2, kQ3ViewerButtonOrbit),
		_button_group(3, kQ3ViewerButtonZoom),
		_button_group(4, kQ3ViewerButtonDolly),
		_separator(),
		_button(5, kQ3ViewerButtonReset),
		_button(6, kQ3ViewerButtonOptions)
	};
#undef _separator
#undef _button
#undef _button_group

	// Create the control strip.
	// It is - like the Apple viewer - a child window of the drawing pane.
	HWND hWndToolbar = CreateToolbarEx 
		( thePort // parent
		, WS_CHILD 
			| WS_CLIPSIBLINGS 
			| WS_VISIBLE 
			| TBSTYLE_TOOLTIPS 
			| CCS_BOTTOM  // window style
		, ID_TOOLBAR // toolbar ID
		, 1 // number of bitmaps
		, gDllInstance  // mod instance
		, IDB_TOOLBAR // resource ID for bitmap
		, (LPCTBBUTTON)&tbButtons // address of buttons
		, 10 //24, // number of buttons
		, 31, 24 //16, 16, // width & height of buttons
		, 31, 30 //16, 16, // width & height of bitmaps
		, sizeof (TBBUTTON) // structure size
		);

	// Recalculate the size of the tool bar
	SendMessage(hWndToolbar,TB_AUTOSIZE,0,0);

	// Set the notification receiver
	SendMessage(hWndToolbar,TB_SETPARENT,(WPARAM) thePort,0);

	// Create the Quesa viewer data
	theViewer= Q3ViewerNew(thePort,rect,flags);
	if (theViewer)
	{
		// This comes handy in Windows:
		// We can assign several properties to our window.
		SetProp(thePort,"Q3ViewerObject",theViewer);
		SetProp(thePort,"Q3ControlStrip",hWndToolbar);
	}
	return theViewer;
}





//=============================================================================
//      E3WinViewerDispose : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerDispose(TQ3ViewerObject viewer)
{
	return Q3ViewerDispose(viewer);
}





//=============================================================================
//      E3WinViewerUseFile : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerUseFile(TQ3ViewerObject viewer, HANDLE fileHandle)
{
	return(Q3ViewerUseFile(viewer, fileHandle));
}





//=============================================================================
//      E3WinViewerUseData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerUseData(TQ3ViewerObject viewer, void *data, TQ3Uns32 size)
{
	return(Q3ViewerUseData(viewer, data, size));
}





//=============================================================================
//      E3WinViewerWriteFile : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerWriteFile(TQ3ViewerObject viewer, HANDLE fileHandle)
{
	return(Q3ViewerWriteFile(viewer, fileHandle));
}





//=============================================================================
//      E3WinViewerWriteData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerWriteData(TQ3ViewerObject viewer, void *data, TQ3Uns32 dataSize, TQ3Uns32 *actualDataSize)
{
	return(Q3ViewerWriteData(viewer, data, dataSize, actualDataSize));
}





//=============================================================================
//      E3WinViewerDraw : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerDraw(TQ3ViewerObject viewer)
{
	TQ3Status status;
	status = Q3WinViewerDrawControlStrip (viewer);
	if (status == kQ3Success)
		status = Q3WinViewerDrawContent (viewer);
	return status;
}





//=============================================================================
//      E3WinViewerDrawContent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerDrawContent(TQ3ViewerObject viewer)
{
	return(Q3ViewerDrawContent(viewer));
}





//=============================================================================
//      E3WinViewerDrawControlStrip : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerDrawControlStrip(TQ3ViewerObject viewer)
{
	TQ3Status status;
	HWND controlStrip= E3WinViewerGetControlStrip(viewer);
	TQ3Uns32 flags= Q3WinViewerGetFlags(viewer);
	TQ3Boolean visible= (flags&kQ3ViewerControllerVisible) != 0;
	if (visible)
	{
		ShowWindow(controlStrip,SW_SHOW);
		{
			unsigned long currentButton= Q3WinViewerGetCurrentButton(viewer);
			
#define _show( idButton ) e3_show_button( controlStrip, flags, idButton )
			_show(kQ3ViewerButtonCamera);
			_show(kQ3ViewerButtonTruck);
			_show(kQ3ViewerButtonOrbit);
			_show(kQ3ViewerButtonZoom);
			_show(kQ3ViewerButtonDolly);
			_show(kQ3ViewerButtonReset);
			_show(kQ3ViewerButtonOptions);
#undef _show
			
			e3_check_button(controlStrip,currentButton);
		}
	}
	else
	{
		ShowWindow(controlStrip,SW_HIDE);
	}

	status= Q3ViewerDrawControlStrip(viewer);
	SendMessage(controlStrip,TB_AUTOSIZE,0,0);
	return status;
}





//=============================================================================
//      E3WinViewerMouseDown : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
BOOL
E3WinViewerMouseDown(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y)
{
	return(Q3ViewerMouseDown(viewer, x, y));
}





//=============================================================================
//      E3WinViewerContinueTracking : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
BOOL
E3WinViewerContinueTracking(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y)
{
	return(Q3ViewerContinueTracking(viewer, x, y));
}





//=============================================================================
//      E3WinViewerMouseUp : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
BOOL
E3WinViewerMouseUp(TQ3ViewerObject viewer, TQ3Int32 x, TQ3Int32 y)
{
	return(Q3ViewerMouseUp(viewer, x, y));
}




 //=============================================================================
//      E3WinViewerGetBitmap : Get a Bitmap of the current Quesa picture.
//-----------------------------------------------------------------------------
//		Note : No need to call Q3ViewerGetBitmap.
//-----------------------------------------------------------------------------
HBITMAP
E3WinViewerGetBitmap(TQ3ViewerObject viewer)
{
	HBITMAP result;
	HWND wnd;
	HDC dc;
	HDC result_dc;
	TQ3Status status;
	unsigned long cx;
	unsigned long cy;
	BOOL ok;
		
	status= Q3WinViewerGetDimension (viewer,&cx,&cy);
	if (status == kQ3Failure) return 0;

	wnd= Q3WinViewerGetWindow(viewer);
	if (wnd == 0) return 0;

	dc= GetDC(wnd);
	if (dc == 0) return 0;

	result=  CreateCompatibleBitmap(dc, cx, cy);
	result_dc= CreateCompatibleDC(dc); 

	if (!SelectObject(result_dc, result)) 
	{
		ReleaseDC(wnd,dc);
		DeleteDC(result_dc);
		DeleteObject(result);
		return 0;
	}

    ok= BitBlt (result_dc, 0, 0, cx, cy, dc, 0, 0, SRCCOPY);
 
	ReleaseDC(wnd,dc);
	DeleteDC(result_dc);

	return result;
}





//=============================================================================
//      E3WinViewerGetButtonRect : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetButtonRect(TQ3ViewerObject viewer, TQ3Uns32 button, RECT *rectangle)
{
	LRESULT win_res= SendMessage
	( E3WinViewerGetControlStrip(viewer)
	, TB_GETITEMRECT
    , (WPARAM) e3_button_quesa_to_item(viewer,button)
    , (LPARAM) (LPRECT) rectangle
	);

	return(win_res == 1 ? kQ3Success: kQ3Failure);
}





//=============================================================================
//      E3WinViewerGetCurrentButton : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Uns32
E3WinViewerGetCurrentButton(TQ3ViewerObject viewer)
{
	return e3_button_app_to_quesa(viewer,Q3ViewerGetCurrentButton(viewer));
}





//=============================================================================
//      E3WinViewerSetCurrentButton : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetCurrentButton(TQ3ViewerObject viewer, TQ3Uns32 button)
{
	TQ3Status status= Q3ViewerSetCurrentButton(viewer, e3_button_quesa_to_app(viewer,button));
	if (status == kQ3Success)
	{
		Q3WinViewerDrawControlStrip(viewer);
	}
	return status;
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
	return(Q3ViewerSetBackgroundColor(viewer, color));
}





//=============================================================================
//      E3WinViewerGetBackgroundColor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetBackgroundColor(TQ3ViewerObject viewer, TQ3ColorARGB *color)
{
	return(Q3ViewerGetBackgroundColor(viewer, color));
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
	TQ3Status status= (Q3ViewerSetFlags(viewer, flags));

	Q3_ASSERT(Q3WinViewerGetCurrentButton(viewer)&Q3WinViewerGetFlags(viewer));
	Q3WinViewerDrawControlStrip(viewer);
	return status;
}





//=============================================================================
//      E3WinViewerGetFlags : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Uns32
E3WinViewerGetFlags(TQ3ViewerObject viewer)
{
	return(Q3ViewerGetFlags(viewer));
}





//=============================================================================
//      E3WinViewerSetBounds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : Set the child size and call the common Quesa
//		resizing procedure.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetBounds(TQ3ViewerObject viewer, RECT *bounds)
{
	HWND wnd;
	BOOL ok;
	wnd= Q3WinViewerGetWindow(viewer);
	if (wnd == 0) return kQ3Failure;
	
	ok= SetWindowPos(
		wnd,             // handle to window
		0,  // placement-order handle
		bounds->left,                 // horizontal position
		bounds->top,                 // vertical position
		bounds->right-bounds->left,                // width
		bounds->bottom-bounds->top,                // height
		0            // window-positioning flags
	);
	if (!ok) return kQ3Failure;
 

	return(Q3ViewerSetBounds(viewer,bounds));
}





//=============================================================================
//      E3WinViewerGetBounds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetBounds(TQ3ViewerObject viewer, RECT *bounds)
{
	return(Q3ViewerGetBounds(viewer, bounds));
}





//=============================================================================
//      E3WinViewerSetDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetDimension(TQ3ViewerObject viewer, TQ3Uns32 width, TQ3Uns32 height)
{
	HWND wnd;
	BOOL ok;
	wnd= Q3WinViewerGetWindow(viewer);
	if (wnd == 0) return kQ3Failure;
	
	ok= SetWindowPos(
		wnd,             // handle to window
		0,  // placement-order handle
		0,                 // horizontal position
		0,                 // vertical position
		width,                // width
		height,                // height
		SWP_NOMOVE            // window-positioning flags
	);
	if (!ok) return kQ3Failure;
 

	return(Q3ViewerSetDimension(viewer,width,height));
}





//=============================================================================
//      E3WinViewerGetDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetDimension(TQ3ViewerObject viewer, TQ3Uns32 *width, TQ3Uns32 *height)
{
	return(Q3ViewerGetDimension(viewer, width, height));
}





//=============================================================================
//      E3WinViewerGetMinimumDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerGetMinimumDimension(TQ3ViewerObject viewer, TQ3Uns32 *width, TQ3Uns32 *height)
{
	return (Q3ViewerGetMinimumDimension(viewer, width, height));
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
	return(Q3ViewerGetWindow(viewer));
}





//=============================================================================
//      E3WinViewerGetViewer : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewerObject
E3WinViewerGetViewer(HWND theWindow)
{
	return (TQ3ViewerObject)GetProp(theWindow,"Q3ViewerObject");
}





//=============================================================================
//      E3WinViewerGetControlStrip : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
HWND
E3WinViewerGetControlStrip(TQ3ViewerObject viewer)
{
	return (HWND)GetProp(Q3WinViewerGetWindow(viewer),"Q3ControlStrip");
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
	return(Q3ViewerGetState(viewer));
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
	TQ3Status status = Q3WinViewerCopy (viewer);
	if (status == kQ3Success)
		return Q3WinViewerClear (viewer);
	return status;
}





//=============================================================================
//      E3WinViewerCopy : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------

static TQ3Status set_3dmf_clip( TQ3ViewerObject viewer )
{
	unsigned long actualDataSize;
	HGLOBAL handle;
	LPVOID data;
	TQ3Status success;
	UINT fmt3dmf;

// preflight
	actualDataSize= 0;
	success= Q3WinViewerWriteData (viewer, 0, 0, &actualDataSize);
	if (success == kQ3Failure) return success;
	if (actualDataSize<=0) return kQ3Success;

	handle= GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, actualDataSize);
	if (handle == 0) return kQ3Failure;

	data= GlobalLock(handle);
	success= Q3WinViewerWriteData (viewer, data, actualDataSize, &actualDataSize);

	if (success == kQ3Success)
	{
		fmt3dmf= RegisterClipboardFormat(kQ3ViewerClipboardFormat);
		SetClipboardData(fmt3dmf, data);
	}

	GlobalUnlock(data);
	return success;
}


static TQ3Status set_bitmap_clip( TQ3ViewerObject viewer )
{
	HBITMAP bitmap;

	bitmap=Q3WinViewerGetBitmap(viewer);
	if (bitmap == 0) return kQ3Failure;

	SetClipboardData(CF_BITMAP, bitmap);

	return kQ3Success;
}


TQ3Status
E3WinViewerCopy(TQ3ViewerObject viewer)
{
	HWND wnd;
	TQ3Status success;

	success= kQ3Success;

	wnd= Q3WinViewerGetWindow(viewer);
	if (wnd == 0) return kQ3Failure;

    if (!OpenClipboard(wnd))  return kQ3Failure;

//Send the bitmap to the clipboard if we can get one.     
	success= set_bitmap_clip( viewer );

// 3DMF, does not yet work correctly in Q3WinViewerWriteData
//	success= set_3dmf_clip( viewer );

//Free clipboard ownership.
	CloseClipboard(); 

	return(success);
}


static TQ3Status e3_win_viewer_render_3dmf_clip( TQ3ViewerObject viewer )
{
	TQ3Status success;
	unsigned long actualDataSize;
// preflight
	actualDataSize= 0;
	success= Q3WinViewerWriteData (viewer, 0, 0, &actualDataSize);
	if (success == kQ3Failure) return success;

	// To be implemented...
	return kQ3Failure;

	// SetClipboardData(fmt3dmf, 0);
}



//=============================================================================
//      E3WinViewerPaste : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//		Known Problems:
//			-- Quesa Assertion: 'classType != kQ3ObjectTypeInvalid' failed on line 1096 of E3ClassTree.c
//				when pasting the test triangle
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerPaste(TQ3ViewerObject viewer)
{
	TQ3Status result;
	HWND wnd;
	UINT fmt3dmf;
	HANDLE data;
	LPVOID clipData;

	result= kQ3Success;

	wnd= Q3WinViewerGetWindow(viewer);
	if (wnd == 0) return kQ3Failure;

    if (!OpenClipboard(wnd))  return kQ3Failure;

	// We only accept 3DMF data:
	//
	// Q3WinViewerSetData(viewer,data);
	fmt3dmf= RegisterClipboardFormat(kQ3ViewerClipboardFormat);
	data= GetClipboardData(fmt3dmf);
	if (data)
	{	
		clipData = GlobalLock(data);
		if (clipData == 0) 
		{
			result= kQ3Failure;
		}
		else
		{
			result= Q3WinViewerUseData(viewer, clipData, GlobalSize(data));
			GlobalUnlock(data);
		}
	}
  
 
//Free clipboard ownership.
	CloseClipboard(); 

	return(result);
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
	return(Q3ViewerGetCameraCount(viewer, count));
}





//=============================================================================
//      E3WinViewerSetCameraNumber : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3WinViewerSetCameraNumber(TQ3ViewerObject viewer, TQ3Uns32 cameraNo)
{
	// return(Q3ViewerSetCameraNumber(viewer, cameraNo));


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
//	return(Q3ViewerSetCameraView(viewer, viewType));


	// To be implemented...
	return(kQ3Failure);
}



LRESULT CALLBACK ViewerWndProc(
                HWND hWnd,        
                UINT message,      
                WPARAM wParam,     
                LPARAM lParam)   
{
		PAINTSTRUCT ps;
		HDC hdc;
		TQ3ViewerObject theViewer;
		HWND controlStrip;
		int fwKeys;
	    int idCtrl; 
		LPNMHDR pnmh; 
		LPNMMOUSE lpnmmouse;

        switch (message) 
		{
			case WM_PAINT:
				theViewer= E3WinViewerGetViewer(hWnd);
				if (theViewer)
				{
					hdc = BeginPaint (hWnd, &ps);
					Q3WinViewerDrawContent (theViewer);
					EndPaint (hWnd, &ps);
				}
				break;

			case WM_LBUTTONDOWN:
				theViewer= E3WinViewerGetViewer(hWnd);
				Q3WinViewerMouseDown(theViewer,LOWORD(lParam),HIWORD(lParam));
				break;

			case WM_LBUTTONUP:
				theViewer= E3WinViewerGetViewer(hWnd);
				Q3WinViewerMouseUp(theViewer,LOWORD(lParam),HIWORD(lParam));
				break;

			case WM_MOUSEMOVE:
				theViewer= E3WinViewerGetViewer(hWnd);
				fwKeys = wParam;        // key flags 
				if (fwKeys&MK_LBUTTON)
				{
					Q3WinViewerContinueTracking(theViewer,LOWORD(lParam),HIWORD(lParam));
				}
				break;

			case WM_NOTIFY:
				// Handling of button clicks
				idCtrl = (int) wParam; 
				pnmh = (LPNMHDR) lParam; 
				if (pnmh->code == NM_CLICK)
				{
					controlStrip= E3WinViewerGetControlStrip(E3WinViewerGetViewer(hWnd));
					if (pnmh->hwndFrom == controlStrip)
					{
					    lpnmmouse = (LPNMMOUSE) lParam;
						Q3WinViewerSetCurrentButton(E3WinViewerGetViewer(hWnd),lpnmmouse->dwItemSpec);
					}
				}
				break;

			case WM_SIZE:
				controlStrip= E3WinViewerGetControlStrip(E3WinViewerGetViewer(hWnd));
				SendMessage(controlStrip,TB_AUTOSIZE,0,0);
				break;

			// case WM_KEY
			// 

// should we answer to WM_DESTROY ???
// e.g. by calling ViewerDispose

            default:          // Passes it on if unproccessed
                    return (DefWindowProc(hWnd, message, wParam, lParam));
        }
        return (0);
}



//=============================================================================
//      e3_viewer_register_window_classes: Registration of the viewer window classes
//-----------------------------------------------------------------------------
//		Note : 
//			The Spy tool shows the following hierarchy for the original viewer
//			Viewer Test { QD3DViewerWindow { ToolbarWindow32 } }
//-----------------------------------------------------------------------------
TQ3Status E3WinViewerRegisterWindowClasses(HINSTANCE hInstance)
{
    WNDCLASSEX  wc;
	ATOM	  success;
    
// Define viewer window class.
    wc.cbSize= sizeof(WNDCLASSEX); 
	wc.style         = CS_OWNDC;// from Spy
    wc.lpfnWndProc   = (WNDPROC)ViewerWndProc;       
    wc.cbClsExtra    = 0;                      
    wc.cbWndExtra    = 0;                     
    wc.hInstance     = hInstance;             
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);// from Spy
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);// from Spy
    wc.lpszMenuName  = 0; 
	wc.lpszClassName = kQ3ViewerClassName;
	wc.hIconSm= 0;
	
    success = RegisterClassEx(&wc);

	return success > 0 ? kQ3ErrorNone : kQ3Failure;
}


#if defined(_USRDLL)

// This procedure should be in some more central point of Quesa
// but for now it does help here.
BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,  // handle to DLL module
  DWORD ul_reason_for_call,     // reason for calling function
  LPVOID lpvReserved   // reserved
)
{
	gDllInstance= hinstDLL;
    switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (E3WinViewerRegisterWindowClasses(hinstDLL) != kQ3ErrorNone)
		{
			return FALSE;
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif
 




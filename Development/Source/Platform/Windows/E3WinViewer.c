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
#include "E3Viewer.h"
#include "E3WinViewer.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here


HINSTANCE gDllInstance;// the global reference to the DLL




//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here


static const char* e3_viewer_window_class_name()
{
#if __REALQD3D__
	return "QD3DViewerWindow";// from Spy
#else
	return "QuesaViewerWindow";
#endif
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
//      E3WinViewerNew : Create a viewer child window.
//-----------------------------------------------------------------------------
//		Note : To fit neatly into the Windows scheme, we take the
//		input window as the parent of the viewer window.
//
//		To do:
//		-- investigation of the correct handling of the rect parameter
//		-- control buttons
//		-- badge
//-----------------------------------------------------------------------------

TQ3ViewerObject
E3WinViewerNew(HWND window, const RECT *rect, TQ3Uns32 flags)
{
	TQ3ViewerObject theViewer;

	HWND thePort= CreateWindowEx
		( WS_EX_LEFT
			| WS_EX_CONTROLPARENT
			| WS_EX_RIGHTSCROLLBAR
			| WS_EX_ACCEPTFILES 
			// extended window style, from Spy
		,  e3_viewer_window_class_name() // pointer to registered class name
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
 
	theViewer= Q3ViewerNew(thePort,rect,flags);
	if (theViewer)
	{
		SetProp(thePort,"Q3ViewerObject",theViewer);
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
		0,                 // horizontal position
		0,                 // vertical position
		bounds->right-bounds->left,                // width
		bounds->bottom-bounds->top,                // height
		SWP_NOMOVE            // window-positioning flags
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
		fmt3dmf= RegisterClipboardFormat("Quickdraw 3D Metafile");
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
	fmt3dmf= RegisterClipboardFormat("Quickdraw 3D Metafile");
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



LRESULT CALLBACK ViewerWndProc(
                HWND hWnd,        
                UINT message,      
                WPARAM wParam,     
                LPARAM lParam)   
{
		PAINTSTRUCT ps;
		HDC hdc;
		TQ3ViewerObject theViewer;

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

// should we answer to WM_DESTROY ???

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
	wc.lpszClassName = e3_viewer_window_class_name();
	wc.hIconSm= 0;
	
    success = RegisterClassEx(&wc);

// Todo: ToolbarWindow32

	return success > 0 ? kQ3ErrorNone : kQ3Failure;
}


#if defined(_USRDLL)

// This procedure should be in some more central point of Quesa
// but for now it does help here.
BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,  // handle to DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpvReserved   // reserved
)
{
	gDllInstance= hinstDLL;
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		if (E3WinViewerRegisterWindowClasses(hinstDLL) != kQ3ErrorNone)
		{
			return FALSE;
		}

 
#if 0
		UINT RegisterClipboardFormat(
  LPCTSTR lpszFormat   // address of name string
);
#endif
	}
	return TRUE;
}

#endif
 




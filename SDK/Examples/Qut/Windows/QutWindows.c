/*  NAME:
        QutWindows.c

    DESCRIPTION:
        Quesa Utility Toolkit - Windows.

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
#include "Qut.h"
#include "QutInternal.h"

#include "QuesaStorage.h"

#include "resource.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kQutWindowClass									"Qut"
#define kQutTimer										2

#define kMenuRenderer									1
#define kMenuStyle										2
#define kMenuSpecial									3

#define kMenuRendererOffset								600
#define kMenuSpecialOffset								700





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
HINSTANCE				gInstance    = NULL;
HDC						gDC          = NULL;
UINT                    gTimer       = 0;
BOOL					gMouseDown   = FALSE;
BOOL					gSpecialMenu = FALSE;
TQ3Point2D				gLastMouse   = {0.0f, 0.0f};





//=============================================================================
//		Internal functions.
//-----------------------------------------------------------------------------
//		qut_build_renderer_menu : Build the renderer menu.
//-----------------------------------------------------------------------------
static void
qut_build_renderer_menu(void)
{	TCHAR				theName[1024];
	TQ3SubClassData		rendererData;
	TQ3Status			qd3dStatus;
	HMENU				theMenu;
	TQ3Uns32			n, m;



	// Get the renderer menu
	theMenu = GetMenu(gWindow);
	theMenu = GetSubMenu(theMenu, kMenuRenderer);
	if (theMenu == NULL)
		return;



	// Collect the renderers which are available
	qd3dStatus = Q3ObjectHierarchy_GetSubClassData(kQ3SharedTypeRenderer, &rendererData);
	if (qd3dStatus != kQ3Success)
		return;



	// If we can find any renderers, add them to the menu
	if (rendererData.numClasses != 0)
		{
		// First slot is a dummy
		gRenderers[0] = kQ3ObjectTypeInvalid;
		m = 1;
		
		
		// Fill the remaining slots
		for (n = 0; n < rendererData.numClasses; n++)
			{
			// Skip the generic renderer, since it can't actually render
			if (rendererData.classTypes[n] != kQ3RendererTypeGeneric)
				{
				// Grab the nick name, falling back to the class name if that fails
				qd3dStatus = Q3RendererClass_GetNickNameString(rendererData.classTypes[n], theName);
				if (qd3dStatus == kQ3Failure || theName[0] == 0x00)
					qd3dStatus = Q3ObjectHierarchy_GetStringFromType(rendererData.classTypes[n], theName);


				// Add the menu item and save the type
				if (qd3dStatus == kQ3Success)
					{
					AppendMenu(theMenu, MF_STRING, kMenuRendererOffset + m, theName);
					gRenderers[m++] = rendererData.classTypes[n];
					}
				}
			}
		}



	// Clean up
	Q3ObjectHierarchy_EmptySubClassData(&rendererData);
}





//=============================================================================
//      qut_get_window_size : Get the size of a window.
//-----------------------------------------------------------------------------
static void
qut_get_window_size(HWND theWnd, TQ3Area *theArea)
{	RECT		theRect;



	// Get the size of the window
	GetClientRect(theWnd, &theRect);

	theArea->min.x = (float) theRect.left;
	theArea->min.y = (float) theRect.top;
	theArea->max.x = (float) theRect.right;
	theArea->max.y = (float) theRect.bottom;
}





//=============================================================================
//      qut_about_proc : About box message handler.
//-----------------------------------------------------------------------------
static LRESULT CALLBACK
qut_about_proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{


	// Handle the message
	switch (message) {
		case WM_INITDIALOG:
			return(TRUE);

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
				{
				EndDialog(hDlg, LOWORD(wParam));
				return(TRUE);
				}
			break;
		}

    return(FALSE);
}





//=============================================================================
//      qut_wnd_proc : Window message handler.
//-----------------------------------------------------------------------------
static LRESULT CALLBACK
qut_wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	TQ3Point2D				mouseDiff, theMouse;
	TQ3DrawContextObject	theDrawContext;
	int						wmId, wmEvent;
	TQ3Area					thePane;
	PAINTSTRUCT				ps;



	// Handle the message
	switch (message) {
		case WM_COMMAND:
			// Handle menu selections
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 

			switch (wmId) {
				case IDM_ABOUT:
				   DialogBox(gInstance, (LPCTSTR) IDD_ABOUTBOX, hWnd, (DLGPROC) qut_about_proc);
				   break;

				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;

				case IDM_STYLE_FILL_FILLED:
					Qut_InvokeStyleCommand(kStyleCmdFillFilled);
					break;
				case IDM_STYLE_FILL_EDGES:
					Qut_InvokeStyleCommand(kStyleCmdFillEdges);
					break;
				case IDM_STYLE_FILL_POINTS:
					Qut_InvokeStyleCommand(kStyleCmdFillPoints);
					break;
				case IDM_STYLE_BACKFACING_BOTH:
					Qut_InvokeStyleCommand(kStyleCmdBackfacingBoth);
					break;
				case IDM_STYLE_BACKFACING_REMOVE:
					Qut_InvokeStyleCommand(kStyleCmdBackfacingRemove);
					break;
				case IDM_STYLE_BACKFACING_FLIP:
					Qut_InvokeStyleCommand(kStyleCmdBackfacingFlip);
					break;
				case IDM_STYLE_INTERPOLATION_NONE:
					Qut_InvokeStyleCommand(kStyleCmdInterpolationNone);
					break;
				case IDM_STYLE_INTERPOLATION_VERTEX:
					Qut_InvokeStyleCommand(kStyleCmdInterpolationVertex);
					break;
				case IDM_STYLE_INTERPOLATION_PIXEL:
					Qut_InvokeStyleCommand(kStyleCmdInterpolationPixel);
					break;
				case IDM_STYLE_ORIENTATION_CLOCKWISE:
					Qut_InvokeStyleCommand(kStyleCmdOrientationClockwise);
					break;
				case IDM_STYLE_ORIENTATION_COUNTER_CLOCKWISE:
					Qut_InvokeStyleCommand(kStyleCmdOrientationCounterClockwise);
					break;
				case IDM_STYLE_ANTIALIAS_NONE:
					Qut_InvokeStyleCommand(kStyleCmdAntiAliasNone);
					break;
				case IDM_STYLE_ANTIALIAS_EDGES:
					Qut_InvokeStyleCommand(kStyleCmdAntiAliasEdges);
					break;
				case IDM_STYLE_ANTIALIAS_FILLED:
					Qut_InvokeStyleCommand(kStyleCmdAntiAliasFilled);
					break;
				case IDM_STYLE_FOG_ON:
					Qut_InvokeStyleCommand(kStyleCmdFogOn);
					break;
				case IDM_STYLE_FOG_OFF:
					Qut_InvokeStyleCommand(kStyleCmdFogOff);
					break;
				case IDM_STYLE_SUBDIVISION_CONSTANT1:
					Qut_InvokeStyleCommand(kStyleCmdSubdivisionConstant1);
					break;
				case IDM_STYLE_SUBDIVISION_CONSTANT2:
					Qut_InvokeStyleCommand(kStyleCmdSubdivisionConstant2);
					break;
				case IDM_STYLE_SUBDIVISION_CONSTANT3:
					Qut_InvokeStyleCommand(kStyleCmdSubdivisionConstant3);
					break;
				case IDM_STYLE_SUBDIVISION_CONSTANT4:
					Qut_InvokeStyleCommand(kStyleCmdSubdivisionConstant4);
					break;
				case IDM_STYLE_SUBDIVISION_WORLDSPACE1:
					Qut_InvokeStyleCommand(kStyleCmdSubdivisionWorldSpace1);
					break;
				case IDM_STYLE_SUBDIVISION_WORLDSPACE2:
					Qut_InvokeStyleCommand(kStyleCmdSubdivisionWorldSpace2);
					break;
				case IDM_STYLE_SUBDIVISION_WORLDSPACE3:
					Qut_InvokeStyleCommand(kStyleCmdSubdivisionWorldSpace3);
					break;
				case IDM_STYLE_SUBDIVISION_SCREENSPACE1:
					Qut_InvokeStyleCommand(kStyleCmdSubdivisionScreenSpace1);
					break;
				case IDM_STYLE_SUBDIVISION_SCREENSPACE2:
					Qut_InvokeStyleCommand(kStyleCmdSubdivisionScreenSpace2);
					break;
				case IDM_STYLE_SUBDIVISION_SCREENSPACE3:
					Qut_InvokeStyleCommand(kStyleCmdSubdivisionScreenSpace3);
					break;

				default:
					if (wmId >= kMenuSpecialOffset)
						{
						// If the app has a callback, call it
						if (gAppMenuSelect != NULL)
							gAppMenuSelect(gView, wmId - kMenuSpecialOffset - 1);
						}

					else if (wmId >= kMenuRendererOffset)
						Q3View_SetRendererByType(gView, gRenderers[wmId - kMenuRendererOffset]);

					else
						return(DefWindowProc(hWnd, message, wParam, lParam));
				}
			break;


		case WM_PAINT:
			BeginPaint(hWnd, &ps);
			Qut_RenderFrame();
			EndPaint(hWnd, &ps);
			break;


		case WM_DESTROY:
			PostQuitMessage(0);
			break;


		case WM_TIMER:
			Qut_RenderFrame();
			break;


		case WM_SIZE:
			qut_get_window_size((HWND) gWindow, &thePane);
			
			Q3View_GetDrawContext(gView, &theDrawContext);
			if (theDrawContext != NULL)
				{
				Q3DrawContext_SetPane(theDrawContext, &thePane);
				Q3Object_Dispose(theDrawContext);
				}
			break;


		case WM_LBUTTONDOWN:
			// Grab the mouse
			gLastMouse.x = (float) LOWORD(lParam);
			gLastMouse.y = (float) HIWORD(lParam);
			gMouseDown   = TRUE;

			// If we have a mouse down callback, call it
			if (gFuncAppMouseDown != NULL)
				gFuncAppMouseDown(gView, gLastMouse);
			break;


		case WM_LBUTTONUP:
			gMouseDown = FALSE;
			break;


		case WM_MOUSEMOVE:
			if (gFuncAppMouseTrack != NULL && gMouseDown)
				{
				theMouse.x = (float) LOWORD(lParam);
				theMouse.y = (float) HIWORD(lParam);
			
				mouseDiff.x = theMouse.x - gLastMouse.x;
				mouseDiff.y = theMouse.y - gLastMouse.y;

				gFuncAppMouseTrack(gView, mouseDiff);
				Qut_RenderFrame();

				gLastMouse = theMouse;
				}
			break;


		default:
			return(DefWindowProc(hWnd, message, wParam, lParam));
	   }

	return(0);
}





//=============================================================================
//      qut_register_class : Register the window class.
//-----------------------------------------------------------------------------
static ATOM
qut_register_class(void)
{	WNDCLASSEX wcex;



	// Register the window
	wcex.cbSize        = sizeof(WNDCLASSEX); 
	wcex.style         = CS_OWNDC;
	wcex.lpfnWndProc   = (WNDPROC) qut_wnd_proc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = gInstance;
	wcex.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName  = (LPCSTR) IDC_QUT;
	wcex.lpszClassName = kQutWindowClass;
	wcex.hIconSm       = NULL;

	return(RegisterClassEx(&wcex));
}





//=============================================================================
//      qut_initialize : Initialise the application.
//-----------------------------------------------------------------------------
static BOOL
qut_initialize(int nCmdShow)
{	TQ3Status		qd3dStatus;



	// Initialise ourselves
	qd3dStatus = Q3Initialize();
	if (qd3dStatus != kQ3Success)
		return(FALSE);

	App_Initialise();

	if (gWindow == NULL)
		return(FALSE);



	// Build the renderer menu
	qut_build_renderer_menu();



	// Show the window and start the timer
	ShowWindow((HWND) gWindow, nCmdShow);

	gTimer = SetTimer(gWindow, WM_TIMER, kQutTimer, NULL);



	// Adjust the menu bar
	RemoveMenu(GetSubMenu(GetMenu(gWindow), kMenuRenderer), 0, MF_BYPOSITION);
	RemoveMenu(GetSubMenu(GetMenu(gWindow), kMenuSpecial),  0, MF_BYPOSITION);

	if (!gSpecialMenu)
		RemoveMenu(GetMenu(gWindow), kMenuSpecial, MF_BYPOSITION);

	DrawMenuBar(gWindow);

	return(TRUE);
}





//=============================================================================
//      qut_terminate : Terminate ourselves.
//-----------------------------------------------------------------------------
static void
qut_terminate(void)
{	TQ3Status		qd3dStatus;



	// Clean up
	if (gView != NULL)
		Q3Object_Dispose(gView);

	if (gDC != NULL)
		ReleaseDC((HWND) gWindow, gDC);

	KillTimer(gWindow, gTimer);

    DestroyWindow((HWND) gWindow);



	// Terminate Quesa
	qd3dStatus = Q3Exit();
}





//=============================================================================
//		Public functions.
//-----------------------------------------------------------------------------
//      Qut_CreateWindow : Create the window.
//-----------------------------------------------------------------------------
#pragma mark -
void
Qut_CreateWindow(const char		*windowTitle,
					TQ3Uns32	theWidth,
					TQ3Uns32	theHeight,
					TQ3Boolean	canResize)
{


	// Create the window
	gWindow = (void *) CreateWindow(kQutWindowClass, windowTitle,
									WS_OVERLAPPEDWINDOW |
									WS_CLIPCHILDREN     |
									WS_CLIPSIBLINGS,
									30, 30, theWidth, theHeight,
									NULL, NULL, gInstance, NULL);



	// Save the window details
	gWindowCanResize = canResize;
}





//=============================================================================
//		Qut_CreateDrawContext : Create the draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
Qut_CreateDrawContext(void)
{	TQ3Win32DCDrawContextData	winDrawContextData;
	TQ3Boolean					resetDrawContext;
	TQ3DrawContextObject		theDrawContext;
	TQ3Status					qd3dStatus;



	// Get the DC
	gDC = GetDC((HWND) gWindow);



	// See if we've got an existing draw context we can reuse. If we
	// do, we grab as much of its state data as we can - this means we
	// wil preserve any changes made by the app's view-configure method.
	resetDrawContext = kQ3True;
	if (gView != NULL)
		{
		qd3dStatus = Q3View_GetDrawContext(gView, &theDrawContext);
		if (qd3dStatus == kQ3Success)
			{
			resetDrawContext = kQ3False;
			Q3DrawContext_GetData(theDrawContext, &winDrawContextData.drawContextData);
			Q3Object_Dispose(theDrawContext);
			}
		}



	// Reset the draw context data if required
	if (resetDrawContext)
		{
		// Fill in the draw context data
		winDrawContextData.drawContextData.clearImageMethod  = kQ3ClearMethodWithColor;
		winDrawContextData.drawContextData.clearImageColor.a = 0.0f;
		winDrawContextData.drawContextData.clearImageColor.r = 1.0f;
		winDrawContextData.drawContextData.clearImageColor.g = 1.0f;
		winDrawContextData.drawContextData.clearImageColor.b = 1.0f;
		winDrawContextData.drawContextData.paneState         = kQ3True;
		winDrawContextData.drawContextData.maskState		 = kQ3False;	
		winDrawContextData.drawContextData.doubleBufferState = kQ3True;
		}



	// Reset the fields which are always updated
	qut_get_window_size((HWND) gWindow, &winDrawContextData.drawContextData.pane);
	winDrawContextData.hdc = gDC;



	// Create the draw context object
	theDrawContext = Q3Win32DCDrawContext_New(&winDrawContextData);
	return(theDrawContext);
}





//=============================================================================
//		Qut_SelectMetafileToOpen : Select a metafile for opening.
//-----------------------------------------------------------------------------
TQ3StorageObject
Qut_SelectMetafileToOpen(void)
{	char				typeFilter[MAX_PATH] = "All Files (*.*)\0*.*\0"
											   "\0\0";
    char            	thePath[MAX_PATH]    = "";
	BOOL				selectedFile;
	TQ3StorageObject	theStorage;
    OPENFILENAME    	openFile;



	// Prompt the user for a file
	memset(&openFile, 0x00, sizeof(openFile));
	openFile.lStructSize       = sizeof(openFile);
    openFile.hwndOwner         = NULL;
    openFile.hInstance         = gInstance;
    openFile.lpstrFilter       = typeFilter;
    openFile.nFilterIndex      = 1;
    openFile.lpstrFile         = thePath;
    openFile.nMaxFile          = sizeof(thePath) - 1;
    openFile.lpstrInitialDir   = "..\\Support Files\\Models\\3DMF\\";
    openFile.lpstrTitle        = "Select a Model";
    openFile.Flags             = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
                                 OFN_LONGNAMES     | OFN_HIDEREADONLY;

	selectedFile = GetOpenFileName(&openFile);
	if (!selectedFile)
		return(NULL);



	// Create a storage object for the file
	theStorage = Q3PathStorage_New(thePath);
	return(theStorage);
}





//=============================================================================
//		Qut_SelectMetafileToSaveTo : Select a metafile to save to.
//-----------------------------------------------------------------------------
TQ3StorageObject
Qut_SelectMetafileToSaveTo(void)
{	char				typeFilter[MAX_PATH] = "All Files (*.*)\0*.*\0"
											   "\0\0";
    char            	thePath[MAX_PATH]    = "";
	BOOL				selectedFile;
	TQ3StorageObject	theStorage;
    OPENFILENAME    	openFile;



	// Prompt the user for a file
	memset(&openFile, 0x00, sizeof(openFile));
	openFile.lStructSize       = sizeof(openFile);
    openFile.hwndOwner         = NULL;
    openFile.hInstance         = gInstance;
    openFile.lpstrFilter       = typeFilter;
    openFile.nFilterIndex      = 1;
    openFile.lpstrFile         = thePath;
    openFile.nMaxFile          = sizeof(thePath) - 1;
    openFile.lpstrTitle        = "Save a Model";
    openFile.Flags             = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT
                                 OFN_LONGNAMES     | OFN_HIDEREADONLY;

	selectedFile = GetSaveFileName(&openFile);
	if (!selectedFile)
		return(NULL);



	// Create a storage object for the file
	theStorage = Q3PathStorage_New(thePath);
	return(theStorage);
}





//=============================================================================
//		Qut_SelectPictureFile : Select a picture file.
//-----------------------------------------------------------------------------
TQ3Status
Qut_SelectPictureFile(void *theFile, TQ3Uns32 fileLen)
{

	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      Qut_CreateMenu : Create the Special menu.
//-----------------------------------------------------------------------------
void
Qut_CreateMenu(qutFuncAppMenuSelect appMenuSelect)
{


	// Set the flag and the callback
    gSpecialMenu   = TRUE;
	gAppMenuSelect = appMenuSelect;
}





//=============================================================================
//      Qut_CreateMenuItem : Create a menu item.
//-----------------------------------------------------------------------------
void
Qut_CreateMenuItem(TQ3Uns32 itemNum, char *itemText)
{	TQ3Uns32	numItems, finalItemNum;
	HMENU		theMenu;



	// Get the special menu
	theMenu = GetMenu(gWindow);
	theMenu = GetSubMenu(theMenu, kMenuSpecial);
	if (theMenu == NULL)
		return;



	// Work out where the item is going to be
	numItems = GetMenuItemCount(theMenu);
	if (itemNum == 0)
		finalItemNum = 1;
	else if (itemNum > numItems)
		finalItemNum = numItems + 1;
	else
		finalItemNum = itemNum;



	// Insert the item
	if (strcmp(itemText, kMenuItemDivider) == 0)
		AppendMenu(theMenu, MF_SEPARATOR, kMenuSpecialOffset + finalItemNum, NULL);
	else
		AppendMenu(theMenu, MF_STRING,    kMenuSpecialOffset + finalItemNum, itemText);
}





//=============================================================================
//      WinMain : App entry point.
//-----------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	HACCEL	hAccelTable;
	MSG		theMsg;



	// Initialise ourselves
	gInstance = hInstance;
	qut_register_class();

	if (!qut_initialize(nCmdShow)) 
		return(FALSE);

	hAccelTable = LoadAccelerators(gInstance, (LPCTSTR)IDC_QUT);



	// Run the app
	while (GetMessage(&theMsg, NULL, 0, 0))
		{
		if (!TranslateAccelerator(theMsg.hwnd, hAccelTable, &theMsg)) 
			{
			TranslateMessage(&theMsg);
			DispatchMessage(&theMsg);
			}
		}



	// Clean up
	App_Terminate();
	qut_terminate();

	return(theMsg.wParam);
}


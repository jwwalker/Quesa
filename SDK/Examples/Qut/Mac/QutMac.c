/*  NAME:
        QutMac.c

    DESCRIPTION:
        Quesa Utility Toolkit - Mac.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kMenuBarQut										128
#define kDialogAbout									128

#define kSleepTicksDefault								1
#define kSleepTicksFPS									0
#define kWindowMinSize									50
#define kWindowMaxSize									5000

#define kMenuApple										128
#define kMenuAppleAbout									1
#define kMenuFile										129
#define kMenuFileClose									1
#define kMenuFileQuit									3
#define kMenuEdit										130
#define kMenuRenderer									131
#define kMenuSpecial									132





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
MenuHandle					gMenuSpecial = NULL;
TQ3Boolean					gShouldQuit  = kQ3False;
TQ3Boolean					gShowFPS     = kQ3False;
TQ3Uns32					gSleepTime   = kSleepTicksDefault;





//=============================================================================
//		Internal functions.
//-----------------------------------------------------------------------------
//		qut_build_renderer_menu : Build the renderer menu.
//-----------------------------------------------------------------------------
static void
qut_build_renderer_menu(void)
{	TQ3SubClassData		rendererData;
	TQ3Status			qd3dStatus;
	MenuHandle			theMenu;
	Str255				theStr;
	TQ3Uns32			n, m;



	// Get the renderer menu
	theMenu = GetMenuHandle(kMenuRenderer);
	if (theMenu == NULL)
		return;



	// Collect the renderers which are available
	qd3dStatus = Q3ObjectHierarchy_GetSubClassData(kQ3SharedTypeRenderer, &rendererData);
	if (qd3dStatus != kQ3Success)
		return;



	// If we can find any renderers, add them to the menu
	if (rendererData.numClasses != 0)
		{
		// First two slots are already used
		gRenderers[0] = kQ3ObjectTypeInvalid;
		gRenderers[1] = kQ3ObjectTypeInvalid;
		m = 2;
		
		
		// Fill the remaining slots
		for (n = 0; n < rendererData.numClasses; n++)
			{
			// Skip the generic renderer, since it can't actually render
			if (rendererData.classTypes[n] != kQ3RendererTypeGeneric)
				{
				// Grab the nick name, falling back to the class name if that fails
				qd3dStatus = Q3RendererClass_GetNickNameString(rendererData.classTypes[n], (char *) &theStr[1]);
				if (qd3dStatus == kQ3Failure || theStr[1] == 0x00)
					qd3dStatus = Q3ObjectHierarchy_GetStringFromType(rendererData.classTypes[n], (char *) &theStr[1]);


				// Add the menu item and save the type
				if (qd3dStatus == kQ3Success)
					{
					theStr[0] = strlen((char *) &theStr[1]);
					if (theStr[0] != 0x00)
						{
						AppendMenu(theMenu, theStr);				
						SetMenuItemText(theMenu, n + 3, theStr);

						gRenderers[m++] = rendererData.classTypes[n];
						}
					}
				}
			}
		}
	else
		DisableItem(theMenu, 0);



	// Clean up
	Q3ObjectHierarchy_EmptySubClassData(&rendererData);
}





//=============================================================================
//		qut_handle_menu : Handle menu selections.
//-----------------------------------------------------------------------------
static void
qut_handle_menu(TQ3Uns32 menuInfo)
{	TQ3Uns16			menuID, menuItem;
	ModalFilterUPP 		dialogFilter;
	DialogItemIndex		dialogItem;
	DialogPtr			theDialog; 
	Str255				theStr;



	// Handle the menu selection
	menuID   = HiWord(menuInfo);
	menuItem = LoWord(menuInfo);
	
	switch (menuID) {
		// Handle Apple menu
		case kMenuApple:
			// Handle about box
			if (menuItem == kMenuAppleAbout)
				{
				// Set up the dialog
				GetStdFilterProc(&dialogFilter);
				theDialog = GetNewDialog(kDialogAbout, NULL, (WindowPtr) -1);
				SetDialogDefaultItem(theDialog, kStdOkItemIndex);
				
				
				// Run the dialog
				do
					{
					ModalDialog(dialogFilter, &dialogItem);
					}
				while (dialogItem != kStdOkItemIndex);
				
				
				// Clean up
				DisposeDialog(theDialog);
				break;
				}
			
			
			// Handle anything else
			else
				{
				GetMenuItemText(GetMenuHandle(kMenuApple), menuItem, theStr);
				OpenDeskAcc(theStr);
				}
			break;



		// Handle file menu
		case kMenuFile:
			// Close/Quit quit the app
			if (menuItem == kMenuFileClose || menuItem == kMenuFileQuit)
				gShouldQuit = kQ3True;
			break;



		// Handle renderer menu
		case kMenuRenderer:
			if (menuItem == 1)
				{
				gShowFPS   = (TQ3Boolean) !gShowFPS;
				gSleepTime = gShowFPS ? kSleepTicksFPS : kSleepTicksDefault;
				CheckItem(GetMenuHandle(kMenuRenderer), menuItem, gShowFPS);
				}
			else
				Q3View_SetRendererByType(gView, gRenderers[menuItem - 1]);
			break;



		// Handle special menu
		case kMenuSpecial:
			// If the app has a callback, call it
			if (gAppMenuSelect != NULL)
				gAppMenuSelect(gView, menuItem);
			break;
		}



	// Unhilight the menu
	HiliteMenu(0);
}





//=============================================================================
//		qut_handle_key : Handle keystrokes.
//-----------------------------------------------------------------------------
static void
qut_handle_key(const EventRecord *theEvent)
{	TQ3Uns8		charCode;



	// If the command key is down, dispatch menu keys
	if (theEvent->modifiers & cmdKey)
		{
		charCode = (theEvent->message & charCodeMask);
		qut_handle_menu(MenuKey(charCode));
		}
}



//=============================================================================
//		qut_handle_nav_event : Event callback from Nav Services.
//-----------------------------------------------------------------------------
static void
qut_handle_nav_event(NavEventCallbackMessage    callBackSelector,
						NavCBRecPtr             callBackParms,
						NavCallBackUserData     callBackUD)
{
#pragma unused(callBackSelector)
#pragma unused(callBackParms)
#pragma unused(callBackUD)



	// Just ignore the event
	return;
}





//=============================================================================
//      qut_initialize : Initialise ourselves.
//-----------------------------------------------------------------------------
static void
qut_initialize(void)
{	TQ3Status		qd3dStatus;
	Handle			theMenuBar;
	TQ3Uns32		n;
	


	// Initialise our globals
	gWindow        = NULL;
	gView          = NULL;
	gShouldQuit    = kQ3False;
	gFuncAppRender = NULL;



	// Initialise the Toolbox
	MaxApplZone();
	for (n = 0; n < 5; n++)
		MoreMasters();

	InitGraf(&qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(NULL);
	InitCursor();



	// Initialise Quesa
	qd3dStatus = Q3Initialize();
	if (qd3dStatus != kQ3Success)
		exit(-1);



	// Set up the menu bar
	theMenuBar = GetNewMBar(kMenuBarQut);
	if (theMenuBar == NULL)
		exit(-1);
	
	SetMenuBar(theMenuBar);
	DisposeHandle(theMenuBar);
	
	AppendResMenu(GetMenuHandle(kMenuApple), 'DRVR');
	qut_build_renderer_menu();
	DrawMenuBar();
}





//=============================================================================
//      qut_terminate : Terminate ourselves.
//-----------------------------------------------------------------------------
static void
qut_terminate(void)
{	TQ3Status		qd3dStatus;



	// Clean up
	if (gWindow != NULL)
		DisposeWindow(gWindow);

	if (gView != NULL)
		Q3Object_Dispose(gView);



	// Terminate Quesa
	qd3dStatus = Q3Exit();
}





//=============================================================================
//      qut_mainloop : Run until we're done.
//-----------------------------------------------------------------------------
static void
qut_mainloop(void)
{	Rect					growRect = { kWindowMinSize, kWindowMinSize, kWindowMaxSize, kWindowMaxSize };
	Point					lastMouse, theMouse;
	TQ3DrawContextObject	theDrawContext;
	UInt32					windowSize;
	WindowPtr				theWindow;
	TQ3Point2D				mouseDiff;
	EventRecord				theEvent;
	SInt16					partCode;
	Str255					theStr;



	// Make sure we're set up correctly
	if (gView == NULL || gWindow == NULL)
		return;



	// While we're not done, keep running
	while (!gShouldQuit)
		{
		// Process the next event
		WaitNextEvent(everyEvent, &theEvent, gSleepTime, NULL);
		switch (theEvent.what) {
			case updateEvt:
				// Clear the update event
				BeginUpdate(gWindow);
				EndUpdate(gWindow);
				break;


			case keyDown:
			case autoKey:
				// Handle keystrokes
				qut_handle_key(&theEvent);
				break;


			case mouseDown:
				// Handle clicks for our window
				partCode = FindWindow(theEvent.where, &theWindow);
				if (partCode == inMenuBar)
					qut_handle_menu(MenuSelect(theEvent.where));

				else if (theWindow == gWindow)
					{
					switch (partCode) {
						case inDrag:
							DragWindow(gWindow, theEvent.where, &qd.screenBits.bounds);
							break;

						case inGoAway:
							gShouldQuit = (TQ3Boolean) TrackGoAway(gWindow, theEvent.where);
							break;
						
						case inGrow:
							if (gWindowCanResize)
								{
								windowSize = GrowWindow(gWindow, theEvent.where, &growRect);
								if (windowSize != 0)
									{
									SizeWindow(gWindow, LoWord(windowSize), HiWord(windowSize), true);
									theDrawContext = Qut_CreateDrawContext();
									if (theDrawContext != NULL)
										{
										Q3View_SetDrawContext(gView, theDrawContext);
										Q3Object_Dispose(theDrawContext);
										}
									}
								}
							break;
					
						case inContent:
							if (gFuncAppMouseTrack != NULL)
								{
								GetMouse(&lastMouse);
								while (StillDown())
									{
									GetMouse(&theMouse);
									mouseDiff.x = (float) (theMouse.h - lastMouse.h);
									mouseDiff.y = (float) (theMouse.v - lastMouse.v);

									gFuncAppMouseTrack(gView, mouseDiff);
									Qut_RenderFrame();

									lastMouse = theMouse;
									}
								}
							break;
						}
					}
				break;


			case nullEvent:
				// Render another frame, and update the FPS counter
				Qut_RenderFrame();
				
				if (gShowFPS)
					{
					sprintf((char *) &theStr[1], "FPS: %.2f", gFPS);
					theStr[0] = strlen((char *) &theStr[1]);

					SetPort((GrafPtr) gWindow);
					ForeColor(whiteColor);
					TextFont(kFontIDGeneva);
					TextSize(9);
				
					MoveTo(5, 12);
					DrawString(theStr);
					}
				break;
			}
		}
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
{	Rect		theRect;
	Str255		theStr;



	// Create the window
	SetRect(&theRect, 0, 0, theWidth, theHeight);
	OffsetRect(&theRect, 20, 60);

	strcpy((char *) &theStr[1], windowTitle);
	theStr[0] = strlen((char *) windowTitle);

	gWindow = NewCWindow(NULL,
						 &theRect,
						 theStr,
						 true,
						 documentProc,
						 (WindowPtr) -1,
						 true,
						 0);

	gWindowCanResize = canResize;
	
	SetPort((GrafPtr) gWindow);
}





//=============================================================================
//		Qut_SelectMetafile : Select a metafile.
//-----------------------------------------------------------------------------
TQ3StorageObject
Qut_SelectMetafile(void)
{	Str255				thePrompt = "\pSelect a model:";
	SFTypeList			fileTypes = { '3DMF', 'TEXT', '3DS ', 'BINA' };
	const TQ3Int16		numTypes  = 4;
	/*
		Jose'
		note we have to found a global mechanism to choose the supported file formats
	*/
	NavEventUPP         navEventFilterUPP;
 	NavDialogOptions    dialogOptions;
    AEKeyword           theAEKeyword;
    NavTypeListHandle   typeListHnd;
	TQ3StorageObject	theStorage;
	FSSpec				theFSSpec;
    AEDesc              theAEDesc;
	NavReplyRecord      navReply;
	StandardFileReply	sfReply;
	OSErr				theErr;



	// If we have Navigation services, use it
	if ((TQ3Uns32) NavLibraryVersion != (TQ3Uns32) kUnresolvedCFragSymbolAddress && NavServicesCanRun())
		{
		// Initialise
		NavLoad();
		NavGetDefaultDialogOptions(&dialogOptions);
		
		dialogOptions.dialogOptionFlags -= kNavAllowMultipleFiles;
		dialogOptions.dialogOptionFlags += kNavNoTypePopup;
		memcpy(&dialogOptions.message, thePrompt, thePrompt[0]+1);
		navEventFilterUPP = NewNavEventProc(qut_handle_nav_event);

		typeListHnd = (NavTypeListHandle) NewHandle(sizeof(NavTypeList) + numTypes * sizeof(OSType));
        if (typeListHnd != NULL)
            {
            HLock((Handle) typeListHnd);

            (*typeListHnd)->componentSignature = '????';
            (*typeListHnd)->osTypeCount        = numTypes;
            memcpy((*typeListHnd)->osType, &fileTypes, numTypes * sizeof(OSType));
            }
 


		// Prompt for the file
		theErr = NavGetFile(NULL, &navReply, &dialogOptions, navEventFilterUPP,
							NULL, NULL, typeListHnd, NULL);



		// Clean up
		NavUnload();
		DisposeRoutineDescriptor(navEventFilterUPP);

		if (!navReply.validRecord)
			return(NULL);



		// Extract the file
		AEGetNthDesc(&navReply.selection, 1, typeFSS, &theAEKeyword, &theAEDesc);
		memcpy(&theFSSpec, *(theAEDesc.dataHandle), sizeof(FSSpec));
		}



	// Otherwise, use Standard File
	else
		{
		StandardGetFile(NULL, numTypes, fileTypes, &sfReply);
		if (!sfReply.sfGood)
			return(NULL);
		
		theFSSpec = sfReply.sfFile;
		}



	// Create a storage object for the file
	theStorage = Q3FSSpecStorage_New(&theFSSpec);
	return(theStorage);
}





//=============================================================================
//		Qut_SelectPictureFile : Select a picture file.
//-----------------------------------------------------------------------------
TQ3Status
Qut_SelectPictureFile(void *theFile, TQ3Uns32 fileLen)
{	Str255				thePrompt = "\pSelect a picture file:";
	NavEventUPP         navEventFilterUPP;
 	NavDialogOptions    dialogOptions;
    AEKeyword           theAEKeyword;
    AEDesc              theAEDesc;
	NavReplyRecord      navReply;
	StandardFileReply	sfReply;
	OSErr				theErr;



	// Make sure we're returning an FSSpec
	if (fileLen != sizeof(FSSpec))
		return(kQ3Failure);



	// If we have Navigation services, use it
	if ((TQ3Uns32) NavLibraryVersion != (TQ3Uns32) kUnresolvedCFragSymbolAddress && NavServicesCanRun())
		{
		// Initialise
		NavLoad();
		NavGetDefaultDialogOptions(&dialogOptions);
		
		dialogOptions.dialogOptionFlags -= kNavAllowMultipleFiles;
		dialogOptions.dialogOptionFlags += kNavNoTypePopup;
		memcpy(&dialogOptions.message, thePrompt, thePrompt[0]+1);
		navEventFilterUPP = NewNavEventProc(qut_handle_nav_event);



		// Prompt for the file
		theErr = NavGetFile(NULL, &navReply, &dialogOptions, navEventFilterUPP,
							NULL, NULL, NULL, NULL);



		// Clean up
		NavUnload();
		DisposeRoutineDescriptor(navEventFilterUPP);

		if (!navReply.validRecord)
			return(kQ3Failure);



		// Extract the file
		AEGetNthDesc(&navReply.selection, 1, typeFSS, &theAEKeyword, &theAEDesc);
		memcpy(theFile, *(theAEDesc.dataHandle), sizeof(FSSpec));
		}



	// Otherwise, use Standard File
	else
		{
		StandardGetFile(NULL, -1, NULL, &sfReply);
		if (!sfReply.sfGood)
			return(kQ3Failure);

		memcpy(theFile, &sfReply.sfFile, sizeof(FSSpec));
		}

	return(kQ3Success);
}





//=============================================================================
//		Qut_CreateDrawContext : Create the draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
Qut_CreateDrawContext(void)
{	TQ3MacDrawContextData	macDrawContextData;
	TQ3Boolean				resetDrawContext;
	TQ3DrawContextObject	theDrawContext;
	TQ3Status				qd3dStatus;
	CGrafPtr				thePort;



	// Get our window
	thePort = (CGrafPtr) gWindow;



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
			Q3DrawContext_GetData(theDrawContext, &macDrawContextData.drawContextData);
			Q3Object_Dispose(theDrawContext);
			}
		}



	// Reset the draw context data if required
	if (resetDrawContext)
		{
		// Fill in the draw context data
		macDrawContextData.drawContextData.clearImageMethod  = kQ3ClearMethodWithColor;
		macDrawContextData.drawContextData.clearImageColor.a = 0.0f;
		macDrawContextData.drawContextData.clearImageColor.r = 1.0f;
		macDrawContextData.drawContextData.clearImageColor.g = 1.0f;
		macDrawContextData.drawContextData.clearImageColor.b = 1.0f;
		macDrawContextData.drawContextData.paneState         = kQ3True;
		macDrawContextData.drawContextData.maskState		 = kQ3False;	
		macDrawContextData.drawContextData.doubleBufferState = kQ3True;
		}



	// Reset the fields which are always updated
	macDrawContextData.drawContextData.pane.min.x = (float) thePort->portRect.left;
	macDrawContextData.drawContextData.pane.min.y = (float) thePort->portRect.top;
	macDrawContextData.drawContextData.pane.max.x = (float) thePort->portRect.right;
	macDrawContextData.drawContextData.pane.max.y = (float) thePort->portRect.bottom;
	macDrawContextData.window	                  = thePort;
	macDrawContextData.library	                  = kQ3Mac2DLibraryNone;
	macDrawContextData.viewPort                   = NULL;
	macDrawContextData.grafPort                   = NULL;



	// Create the draw context object
	theDrawContext = Q3MacDrawContext_New(&macDrawContextData);
	return(theDrawContext);
}





//=============================================================================
//      Qut_CreateMenu : Create the Special menu.
//-----------------------------------------------------------------------------
void
Qut_CreateMenu(qutFuncAppMenuSelect appMenuSelect)
{


	// Create the Special menu
	gMenuSpecial = NewMenu(kMenuSpecial, "\pSpecial");
	if (gMenuSpecial == NULL)
		return;



	// Insert the menu and redraw
	InsertMenu(gMenuSpecial, 0);
	DrawMenuBar();



	// Set the callback
	gAppMenuSelect = appMenuSelect;
}





//=============================================================================
//      Qut_CreateMenuItem : Create a menu item.
//-----------------------------------------------------------------------------
void
Qut_CreateMenuItem(TQ3Uns32 itemNum, char *itemText)
{	TQ3Uns32	numItems, finalItemNum;
	Str255		theStr;



	// Make sure we have a menu
	if (gMenuSpecial == NULL)
		return;



	// Convert the text to a Pascal string
	theStr[0] = strlen(itemText);
	memcpy(&theStr[1], itemText, theStr[0]);



	// Work out where the item is going to be
	numItems = CountMenuItems(gMenuSpecial);
	if (itemNum == 0)
		finalItemNum = 1;
	else if (itemNum > numItems)
		finalItemNum = numItems + 1;
	else
		finalItemNum = itemNum;



	// Insert the item and redraw the menu bar
	if (strcmp(itemText, kMenuItemDivider) == 0)
		InsertMenuItem(gMenuSpecial, "\p-", itemNum);
	else
		{
		InsertMenuItem(gMenuSpecial,  "\pmoof",     itemNum);
		SetMenuItemText(gMenuSpecial, finalItemNum, theStr);
		}

	DrawMenuBar();
}





//=============================================================================
//      main : App entry point.
//-----------------------------------------------------------------------------
int main(void)
{


	// Initialise ourselves
	qut_initialize();
	App_Initialise();



	// Run the app
	qut_mainloop();



	// Clean up
	App_Terminate();
	qut_terminate();

	return(0);
}

/*  NAME:
        QutMac.c

    DESCRIPTION:
        Quesa Utility Toolkit - Mac.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#include "QutMac.h"





//=============================================================================
//      Carbon pre-amble
//-----------------------------------------------------------------------------
//		Note :	At the moment we only use Carbon Events on X, but we should be
//				able to turn this on for 8/9 when running with CarbonLib 1.1.
//-----------------------------------------------------------------------------
#ifndef QUT_MAC_CARBON_EVENTS
	#if defined(TARGET_RT_MAC_MACHO) && (TARGET_RT_MAC_MACHO)
		#define QUT_MAC_CARBON_EVENTS				1
	#else
		#define QUT_MAC_CARBON_EVENTS				0
	#endif
#endif




//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kMenuBarQut										128
#define kDialogAbout									128

#define kQutMacUpdateSeconds							0.05f
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

#if QUT_MAC_CARBON_EVENTS
	#define kMenuRenderer								132
	#define kMenuStyle									133
	#define kMenuSpecial								134
#else
	#define kMenuRenderer								131
	#define kMenuStyle									132
	#define kMenuSpecial								133
#endif

enum {
	kQutCommandShowFPS									= FOUR_CHAR_CODE('Sfps'),
	kQutCommandUseRenderer								= FOUR_CHAR_CODE('UsRn'),
	kQutCommandAboutBox									= FOUR_CHAR_CODE('Abot'),
	kQutCommandClose									= FOUR_CHAR_CODE('clos'),
	kQutCommandStyleFillFilled							= FOUR_CHAR_CODE('sc01'),
	kQutCommandStyleFillEdges							= FOUR_CHAR_CODE('sc02'),
	kQutCommandStyleFillPoints							= FOUR_CHAR_CODE('sc03'),
	kQutCommandStyleBackfacingBoth						= FOUR_CHAR_CODE('sc04'),
	kQutCommandStyleBackfacingRemove					= FOUR_CHAR_CODE('sc05'),
	kQutCommandStyleBackfacingFlip						= FOUR_CHAR_CODE('sc06'),
	kQutCommandStyleInterpolationNone					= FOUR_CHAR_CODE('sc07'),
	kQutCommandStyleInterpolationVertex					= FOUR_CHAR_CODE('sc08'),
	kQutCommandStyleInterpolationPixel					= FOUR_CHAR_CODE('sc09'),
	kQutCommandStyleOrientationClockwise				= FOUR_CHAR_CODE('sc10'),
	kQutCommandStyleOrientationCounterClockwise			= FOUR_CHAR_CODE('sc11'),
	kQutCommandStyleAntiAliasNone						= FOUR_CHAR_CODE('sc12'),
	kQutCommandStyleAntiAliasEdges						= FOUR_CHAR_CODE('sc13'),
	kQutCommandStyleAntiAliasFilled						= FOUR_CHAR_CODE('sc14'),
	kQutCommandStyleFogOn								= FOUR_CHAR_CODE('sc15'),
	kQutCommandStyleFogOff								= FOUR_CHAR_CODE('sc16'),
	kQutCommandStyleSubdivisionConstant1				= FOUR_CHAR_CODE('sc17'),
	kQutCommandStyleSubdivisionConstant2				= FOUR_CHAR_CODE('sc18'),
	kQutCommandStyleSubdivisionConstant3				= FOUR_CHAR_CODE('sc19'),
	kQutCommandStyleSubdivisionConstant4				= FOUR_CHAR_CODE('sc20'),
	kQutCommandStyleSubdivisionWorldSpace1				= FOUR_CHAR_CODE('sc21'),
	kQutCommandStyleSubdivisionWorldSpace2				= FOUR_CHAR_CODE('sc22'),
	kQutCommandStyleSubdivisionWorldSpace3				= FOUR_CHAR_CODE('sc23'),
	kQutCommandStyleSubdivisionScreenSpace1				= FOUR_CHAR_CODE('sc24'),
	kQutCommandStyleSubdivisionScreenSpace2				= FOUR_CHAR_CODE('sc25'),
	kQutCommandStyleSubdivisionScreenSpace3				= FOUR_CHAR_CODE('sc26')
};





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
MenuHandle					gMenuSpecial = NULL;
TQ3Boolean					gShouldQuit  = kQ3False;
TQ3Boolean					gShowFPS     = kQ3False;
TQ3Uns32					gSleepTime   = kSleepTicksDefault;

#if QUT_MAC_CARBON_EVENTS
EventLoopTimerRef			gUpdateTimer = NULL;
#endif





//=============================================================================
//		Internal functions.
//-----------------------------------------------------------------------------
//		qut_carbon_get_nib : Get our nib reference.
//-----------------------------------------------------------------------------
#if QUT_MAC_CARBON_EVENTS
static IBNibRef
qut_carbon_get_nib(void)
{
  static IBNibRef nibRef = NULL;
  if(!nibRef)
  {
    OSStatus err = CreateNibReference(CFSTR("main"), &nibRef);
    if(err)
    {
        fprintf(stderr, "Can't load Main Nib: %d\n", (int)err);
        ExitToShell();
    }
  }
  return nibRef;
}





//=============================================================================
//		qut_carbon_window_event : Handle window events.
//-----------------------------------------------------------------------------
static OSStatus
qut_carbon_window_event(EventHandlerCallRef inHandlerCallRef, 
                                  EventRef inEvent, void *inUserData)
{
  OSStatus err = noErr;
    switch(GetEventKind(inEvent))
    {
      case kEventWindowClose:
        gShouldQuit = kQ3True;
        QuitApplicationEventLoop();
        break;
      case kEventWindowClickResizeRgn: {
        TQ3DrawContextObject theDrawContext;

        if (gWindowCanResize)
        {
            err = CallNextEventHandler(inHandlerCallRef, inEvent);
            if(err)
              break;
            
            theDrawContext = Qut_CreateDrawContext();
            if (theDrawContext != NULL)
              {
                Q3View_SetDrawContext(gView, theDrawContext);
                Q3Object_Dispose(theDrawContext);
              }
          }
        }
      case kEventWindowClickContentRgn: {
		TQ3Point2D				q3MousePoint, q3MouseDiff;
		Point					mousePoint, lastMouse;
		MouseTrackingResult		result;


		// Grab the event data
		err = GetEventParameter(inEvent,kEventParamMouseLocation,
        	                typeQDPoint, NULL, sizeof(Point),NULL, &mousePoint);


		// If we have a mouse down handler, call it
		if (gFuncAppMouseDown != NULL)
			{
			q3MousePoint.x = (float) mousePoint.h;
			q3MousePoint.y = (float) mousePoint.v;
			
			gFuncAppMouseDown(gView, q3MousePoint);
			}


		// If we have a mouse track handler, call it
		if (gFuncAppMouseTrack != NULL)
			{
              lastMouse = mousePoint;
              err = TrackMouseLocation(GetWindowPort(gWindow),&mousePoint,&result);
              while (!err && (result != kMouseTrackingMouseReleased))
              {
                q3MouseDiff.x = (float) (mousePoint.h - lastMouse.h);
                q3MouseDiff.y = (float) (mousePoint.v - lastMouse.v);
  
                gFuncAppMouseTrack(gView, q3MouseDiff);
                Qut_RenderFrame();
                lastMouse = mousePoint;
                err = TrackMouseLocation(GetWindowPort(gWindow),&mousePoint,&result);
			}
		}
      
      }
        break;
      default:
            err = CallNextEventHandler(inHandlerCallRef, inEvent);
        break;
    }
    return err;
}





//=============================================================================
//		qut_carbon_command_event : Handle commands.
//-----------------------------------------------------------------------------
static OSStatus
qut_carbon_command_event(EventHandlerCallRef inHandlerCallRef, 
                                  EventRef inEvent, void *inUserData)
{
  OSStatus err = noErr;
  HICommand command;
  err = GetEventParameter(inEvent,kEventParamDirectObject,
                        typeHICommand, NULL, sizeof(HICommand),NULL, &command);
//  fprintf(stdout,"Command Number %u\n",(unsigned int)command.commandID);fflush(stdout);
  if(command.commandID == 0)
  {
    switch(GetMenuID(command.menu.menuRef))
    {
      case kMenuRenderer:
			if (command.menu.menuItemIndex == 1)
				{
				gShowFPS   = (TQ3Boolean) !gShowFPS;
				gSleepTime = gShowFPS ? kSleepTicksFPS : kSleepTicksDefault;
				CheckMenuItem(GetMenuHandle(kMenuRenderer), command.menu.menuItemIndex, gShowFPS);
				}
			else
				Q3View_SetRendererByType(gView, gRenderers[command.menu.menuItemIndex - 1]);
			break;
      case kMenuStyle:
			Qut_InvokeStyleCommand(command.menu.menuItemIndex);
        break;
      case kMenuSpecial:
			if (gAppMenuSelect != NULL)
				gAppMenuSelect(gView, command.menu.menuItemIndex);
        break;
      default:
          err = CallNextEventHandler(inHandlerCallRef, inEvent);
        break;
    }
    	// Unhilight the menu
      HiliteMenu(0);
  } else {
	switch(command.commandID) {
		case kQutCommandShowFPS:
			gShowFPS   = (TQ3Boolean) !gShowFPS;
			gSleepTime = gShowFPS ? kSleepTicksFPS : kSleepTicksDefault;
			CheckMenuItem(GetMenuHandle(kMenuRenderer), command.menu.menuItemIndex, gShowFPS);
			break;

        case kQutCommandUseRenderer:
			Q3View_SetRendererByType(gView, gRenderers[command.menu.menuItemIndex - 1]);
			break;

        case kQutCommandClose:
			gShouldQuit = kQ3True;
			QuitApplicationEventLoop();
			break;

		case kQutCommandStyleFillFilled:
			Qut_InvokeStyleCommand(kStyleCmdFillFilled);
			break;
		case kQutCommandStyleFillEdges:
			Qut_InvokeStyleCommand(kStyleCmdFillEdges);
			break;
		case kQutCommandStyleFillPoints:
			Qut_InvokeStyleCommand(kStyleCmdFillPoints);
			break;
		case kQutCommandStyleBackfacingBoth:
			Qut_InvokeStyleCommand(kStyleCmdBackfacingBoth);
			break;
		case kQutCommandStyleBackfacingRemove:
			Qut_InvokeStyleCommand(kStyleCmdBackfacingRemove);
			break;
		case kQutCommandStyleBackfacingFlip:
			Qut_InvokeStyleCommand(kStyleCmdBackfacingFlip);
			break;
		case kQutCommandStyleInterpolationNone:
			Qut_InvokeStyleCommand(kStyleCmdInterpolationNone);
			break;
		case kQutCommandStyleInterpolationVertex:
			Qut_InvokeStyleCommand(kStyleCmdInterpolationVertex);
			break;
		case kQutCommandStyleInterpolationPixel:
			Qut_InvokeStyleCommand(kStyleCmdInterpolationPixel);
			break;
		case kQutCommandStyleOrientationClockwise:
			Qut_InvokeStyleCommand(kStyleCmdOrientationClockwise);
			break;
		case kQutCommandStyleOrientationCounterClockwise:
			Qut_InvokeStyleCommand(kStyleCmdOrientationCounterClockwise);
			break;
		case kQutCommandStyleAntiAliasNone:
			Qut_InvokeStyleCommand(kStyleCmdAntiAliasNone);
			break;
		case kQutCommandStyleAntiAliasEdges:
			Qut_InvokeStyleCommand(kStyleCmdAntiAliasEdges);
			break;
		case kQutCommandStyleAntiAliasFilled:
			Qut_InvokeStyleCommand(kStyleCmdAntiAliasFilled);
			break;
		case kQutCommandStyleFogOn:
			Qut_InvokeStyleCommand(kStyleCmdFogOn);
			break;
		case kQutCommandStyleFogOff:
			Qut_InvokeStyleCommand(kStyleCmdFogOff);
			break;
		case kQutCommandStyleSubdivisionConstant1:
			Qut_InvokeStyleCommand(kStyleCmdSubdivisionConstant1);
			break;
		case kQutCommandStyleSubdivisionConstant2:
			Qut_InvokeStyleCommand(kStyleCmdSubdivisionConstant2);
			break;
		case kQutCommandStyleSubdivisionConstant3:
			Qut_InvokeStyleCommand(kStyleCmdSubdivisionConstant3);
			break;
		case kQutCommandStyleSubdivisionConstant4:
			Qut_InvokeStyleCommand(kStyleCmdSubdivisionConstant4);
			break;
		case kQutCommandStyleSubdivisionWorldSpace1:
			Qut_InvokeStyleCommand(kStyleCmdSubdivisionWorldSpace1);
			break;
		case kQutCommandStyleSubdivisionWorldSpace2:
			Qut_InvokeStyleCommand(kStyleCmdSubdivisionWorldSpace2);
			break;
		case kQutCommandStyleSubdivisionWorldSpace3:
			Qut_InvokeStyleCommand(kStyleCmdSubdivisionWorldSpace3);
			break;
		case kQutCommandStyleSubdivisionScreenSpace1:
			Qut_InvokeStyleCommand(kStyleCmdSubdivisionScreenSpace1);
			break;
		case kQutCommandStyleSubdivisionScreenSpace2:
			Qut_InvokeStyleCommand(kStyleCmdSubdivisionScreenSpace2);
			break;
		case kQutCommandStyleSubdivisionScreenSpace3:
			Qut_InvokeStyleCommand(kStyleCmdSubdivisionScreenSpace3);
			break;

		case kQutCommandAboutBox: {
            ModalFilterUPP 		dialogFilter;
            DialogItemIndex		dialogItem;
            DialogPtr			theDialog; 
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
			}
			break;

		default:
			err = CallNextEventHandler(inHandlerCallRef, inEvent);
			break;
		}
      HiliteMenu(0);
  }
  return err;
}





//=============================================================================
//		qut_carbon_timer_fired : Timer callback.
//-----------------------------------------------------------------------------
static void
qut_carbon_timer_fired(EventLoopTimerRef updateTimer,void* inUserData)
{
    Str255 theStr;
    GrafPtr oldPort;

	Qut_RenderFrame();
    if (gShowFPS)
    {
      sprintf((char *) &theStr[1], "FPS: %.2f", gFPS);
      theStr[0] = strlen((char *) &theStr[1]);
      GetPort(&oldPort);
      SetPort((GrafPtr) GetWindowPort(gWindow));
      ForeColor(whiteColor);
      TextFont(kFontIDGeneva);
      TextSize(9);
  
      MoveTo(5, 12);
      DrawString(theStr);
      SetPort(oldPort);
    }
}





//=============================================================================
//      qut_carbon_install_handlers : Install Carbon Event handlers.
//-----------------------------------------------------------------------------
static OSStatus
qut_carbon_install_handlers(void)
{
	const EventTypeSpec windowEventTypes[] = { 
                                        { kEventClassWindow, 
                                          kEventWindowClose },
                                        { kEventClassWindow, 
                                          kEventWindowClickResizeRgn },
                                        { kEventClassWindow,
                                          kEventWindowClickContentRgn }
                                      };
	EventTypeSpec appEventTypes = { kEventClassCommand, kEventProcessCommand };
	OSStatus err;
	err = InstallWindowEventHandler((WindowRef)gWindow,
                      NewEventHandlerUPP(qut_carbon_window_event),
                      3,
                      windowEventTypes,
                      NULL,NULL);
	if(err) return err;

    err = InstallApplicationEventHandler(NewEventHandlerUPP(qut_carbon_command_event),
                                1,
                                &appEventTypes,
                                NULL,NULL);                    
	if(err) return err;
 
    err = InstallEventLoopTimer(GetMainEventLoop(),
                          0.0f,
                          kQutMacUpdateSeconds,
                          NewEventLoopTimerUPP(qut_carbon_timer_fired), 
                          NULL, &gUpdateTimer);
    return err;                      
}
#endif // QUT_MAC_CARBON_EVENTS





//=============================================================================
//		qut_classic_handle_menu : Handle menu selections.
//-----------------------------------------------------------------------------
#if !QUT_MAC_CARBON_EVENTS
static void
qut_classic_handle_menu(TQ3Uns32 menuInfo)
{	TQ3Uns16			menuID, menuItem;
	ModalFilterUPP 		dialogFilter;
	DialogItemIndex		dialogItem;
	DialogPtr			theDialog; 
#if !TARGET_API_MAC_CARBON
	Str255				theStr;
#endif



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
#if !TARGET_API_MAC_CARBON
				GetMenuItemText(GetMenuHandle(kMenuApple), menuItem, theStr);
				OpenDeskAcc(theStr);
#endif
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
				CheckMenuItem(GetMenuHandle(kMenuRenderer), menuItem, gShowFPS);
				}
			else
				Q3View_SetRendererByType(gView, gRenderers[menuItem - 1]);
			break;



		// Handle style menu
		case kMenuStyle:
			Qut_InvokeStyleCommand(menuItem);
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
//		qut_classic_handle_key : Handle keystrokes.
//-----------------------------------------------------------------------------
static void
qut_classic_handle_key(const EventRecord *theEvent)
{	TQ3Uns8		charCode;



	// If the command key is down, dispatch menu keys
	if (theEvent->modifiers & cmdKey)
		{
		charCode = (theEvent->message & charCodeMask);
		qut_classic_handle_menu(MenuKey(charCode));
		}
}
#endif // QUT_MAC_CARBON_EVENTS





//=============================================================================
//		qut_handle_nav_event : Event callback from Nav Services.
//-----------------------------------------------------------------------------
static pascal void
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
#if QUT_MAC_CARBON_EVENTS                        
                        SetMenuItemCommandID(theMenu, n + 3, kQutCommandUseRenderer);
#endif                       
						gRenderers[m++] = rendererData.classTypes[n];
						}
					}
				}
			}
		}
	else
		DisableMenuItem(theMenu, 0);



	// Clean up
	Q3ObjectHierarchy_EmptySubClassData(&rendererData);
}





//=============================================================================
//      qut_initialize : Initialise ourselves.
//-----------------------------------------------------------------------------
static void
qut_initialize(void)
{	TQ3Status		qd3dStatus;

#if QUT_MAC_CARBON_EVENTS
    OSStatus		theErr;
#else
	Handle			theMenuBar;
#endif

#if !TARGET_API_MAC_CARBON
	TQ3Uns32		n;
#endif



	// Initialise our globals
	gWindow        = NULL;
	gView          = NULL;
	gShouldQuit    = kQ3False;
	gFuncAppRender = NULL;



	// Initialise the Toolbox if we're not on Carbon
#if !TARGET_API_MAC_CARBON
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
#endif



	// Initialise Quesa
	qd3dStatus = Q3Initialize();
	if (qd3dStatus != kQ3Success)
		exit(-1);



	// Set up the menu bar
#if QUT_MAC_CARBON_EVENTS
	// Use a nib for Mac OS X
	theErr = SetMenuBarFromNib(qut_carbon_get_nib(), CFSTR("MainMenu"));

	if (theErr != noErr)
		{
		fprintf(stderr, "Can't load MenuBar %d\n", (int) theErr);
		ExitToShell();
		}
#else
	// Or use the resource fork for classic Mac OS
	theMenuBar = GetNewMBar(kMenuBarQut);
	if (theMenuBar == NULL)
		exit(-1);
	
	SetMenuBar(theMenuBar);
	DisposeHandle(theMenuBar);
	
	AppendResMenu(GetMenuHandle(kMenuApple), 'DRVR');
#endif



	// Build the renderer menu and redraw the menu bar
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
	if (gView != NULL)
		Q3Object_Dispose(gView);

	if (gWindow != NULL)
		DisposeWindow(gWindow);



	// Terminate Quesa
	qd3dStatus = Q3Exit();
}





//=============================================================================
//      qut_mainloop : Run until we're done.
//-----------------------------------------------------------------------------
static void
qut_mainloop(void)
{
#if !QUT_MAC_CARBON_EVENTS
	// Classic WNE event loop
	Rect					dragRect = { -32767, -32767, 32767, 32767 };
	Rect					growRect = { kWindowMinSize, kWindowMinSize, kWindowMaxSize, kWindowMaxSize };
	TQ3Point2D				q3MousePoint, q3MouseDiff;
	Point					lastMouse, theMouse;
	TQ3DrawContextObject	theDrawContext;
	TQ3Uns32				windowSize;
	WindowPtr				theWindow;
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
				// Clear the window on an update event
				BeginUpdate(gWindow);
				
				BackColor(whiteColor);
				EraseRect(&dragRect);
				
				// And invoke the redraw function, if there is one
				if (gFuncAppRedraw != NULL)
					gFuncAppRedraw(gView);
				
				EndUpdate(gWindow);
				break;


			case keyDown:
			case autoKey:
				// Handle keystrokes
				qut_classic_handle_key(&theEvent);
				break;


			case mouseDown:
				// Handle clicks for our window
				partCode = FindWindow(theEvent.where, &theWindow);
				if (partCode == inMenuBar)
					qut_classic_handle_menu(MenuSelect(theEvent.where));

				else if (theWindow == gWindow)
					{
					switch (partCode) {
						case inDrag:
							DragWindow(gWindow, theEvent.where, &dragRect);
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
							// Grab the mouse
							GetMouse(&lastMouse);


							// If we have a mouse down handler, call it
							if (gFuncAppMouseDown != NULL)
								{
								q3MousePoint.x = (float) lastMouse.h;
								q3MousePoint.y = (float) lastMouse.v;
			
								gFuncAppMouseDown(gView, q3MousePoint);
								}
							
							
							// If we have a mouse track handler, call it
							if (gFuncAppMouseTrack != NULL)
								{
								while (StillDown())
									{
									GetMouse(&theMouse);
									q3MouseDiff.x = (float) (theMouse.h - lastMouse.h);
									q3MouseDiff.y = (float) (theMouse.v - lastMouse.v);

									gFuncAppMouseTrack(gView, q3MouseDiff);
									Qut_RenderFrame();

									lastMouse = theMouse;
									}
								}
							
							// If we have a mouse up handler, call it
							if (gFuncAppMouseUp != NULL)
								{
								q3MousePoint.x = (float) lastMouse.h;
								q3MousePoint.y = (float) lastMouse.v;
			
								gFuncAppMouseUp(gView, q3MousePoint);
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

					SetPort((GrafPtr) GetWindowPort(gWindow));
					ForeColor(whiteColor);
					TextFont(kFontIDGeneva);
					TextSize(9);
				
					MoveTo(5, 12);
					DrawString(theStr);
					}
				
				// If we have an idle handler, call it
				if (gFuncAppIdle != NULL)
					gFuncAppIdle(gView);
				
				break;
			}
		}



#else
	// Carbon event loop
	OSStatus	theErr;


	// Make sure we're set up correctly
	if (gView == NULL || gWindow == NULL)
		return;


	// Install the event handlers
	theErr = qut_carbon_install_handlers();
	if (theErr != noErr)
		{
		fprintf(stderr,"Error %d installing carbon event handlers\n",(int) theErr);
	    return;
		}


	// Run the event loop
	RunApplicationEventLoop();
#endif                
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
#if !QUT_MAC_CARBON_EVENTS
	// Classic Window Manager
	Rect		theRect;
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
	
	SetPort((GrafPtr) GetWindowPort(gWindow));
	SetRect(&theRect, 0, 0, theWidth, theHeight);

	// And invoke the redraw function, if there is one
	if (gFuncAppRedraw != NULL)
		gFuncAppRedraw(gView);

	ValidWindowRect(gWindow, &theRect);


#else
	// Carbon window manager (do we really need to use a nib for this?)
	OSStatus		theErr;
	
	
	// Create the window
	theErr = CreateWindowFromNib(qut_carbon_get_nib(), CFSTR("MainWindow"), (WindowRef *) &gWindow);
	if (theErr != noErr)
		{
		fprintf(stderr, "Can't load Window %d\n", (int) theErr);
		ExitToShell();
		}

	gWindowCanResize = canResize;
    SizeWindow(gWindow, (short) theWidth, (short) theHeight, false);                               
	ShowWindow(gWindow);
#endif        
}





//=============================================================================
//		Qut_SelectMetafileToOpen : Select a metafile for opening.
//-----------------------------------------------------------------------------
TQ3StorageObject
Qut_SelectMetafileToOpen(void)
{	Str255				thePrompt = "\pSelect a model:";
	OSType				fileTypes[] = { '3DMF', 'TEXT', '3DS ', 'BINA' };
	UInt32				numTypes  = sizeof(fileTypes) / sizeof(OSType);
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
#if !TARGET_API_MAC_CARBON
	StandardFileReply	sfReply;
#endif
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
		navEventFilterUPP = NewNavEventUPP(qut_handle_nav_event);

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
		DisposeNavEventUPP(navEventFilterUPP);

		if (!navReply.validRecord)
			return(NULL);



		// Extract the file
		AEGetNthDesc(&navReply.selection, 1, typeFSS, &theAEKeyword, &theAEDesc);
		AEGetDescData(&theAEDesc, &theFSSpec, sizeof(FSSpec));
		}



	// Otherwise, use Standard File (but not on Carbon!)
	else
		{
#if TARGET_API_MAC_CARBON
		return(NULL);
#else
		StandardGetFile(NULL, numTypes, fileTypes, &sfReply);
		if (!sfReply.sfGood)
			return(NULL);
		
		theFSSpec = sfReply.sfFile;
#endif
		}



	// Create a storage object for the file
	theStorage = Q3FSSpecStorage_New(&theFSSpec);
	return(theStorage);
}





//=============================================================================
//		Qut_SelectMetafileToSaveTo : Select a metafile to save to.
//-----------------------------------------------------------------------------
TQ3StorageObject
Qut_SelectMetafileToSaveTo(void)
{	Str255				thePrompt = "\pSave a model:";
	OSType			fileType =  '3DMF';
	OSType			creator =  'ttxt';// what will be the creator for the viewer?
	const TQ3Int16		numTypes  = 1;
	/*
		Jose'
		note we have to found a global mechanism to choose the supported file formats
	*/
	NavEventUPP         navEventFilterUPP;
 	NavDialogOptions    dialogOptions;
    AEKeyword           theAEKeyword;
	TQ3StorageObject	theStorage;
	FSSpec				theFSSpec;
    AEDesc              theAEDesc;
	NavReplyRecord      navReply;
#if !TARGET_API_MAC_CARBON
	StandardFileReply	sfReply;
#endif
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
		navEventFilterUPP = NewNavEventUPP(qut_handle_nav_event);

 


		// Prompt for the file
		theErr = NavPutFile(NULL, &navReply, &dialogOptions, navEventFilterUPP,
							fileType, creator, NULL);


		// is there really need to call NavCompleteSave?

		// Clean up
		NavUnload();
		DisposeNavEventUPP(navEventFilterUPP);

		if (!navReply.validRecord)
			return(NULL);



		// Extract the file
		AEGetNthDesc(&navReply.selection, 1, typeFSS, &theAEKeyword, &theAEDesc);
		AEGetDescData(&theAEDesc, &theFSSpec, sizeof(FSSpec));
		}



	// Otherwise, use Standard File (but not on Carbon!)
	else
		{
#if TARGET_API_MAC_CARBON
		return(NULL);
#else
		StandardPutFile(thePrompt, NULL, &sfReply);

		if (!sfReply.sfGood)
			return(NULL);
		
		theFSSpec = sfReply.sfFile;
#endif
		}

		// Blind delete
		FSpDelete(&theFSSpec);

		theErr = FSpCreate(&theFSSpec,creator, fileType, smSystemScript);


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
#if !TARGET_API_MAC_CARBON
	StandardFileReply	sfReply;
#endif
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
		navEventFilterUPP = NewNavEventUPP(qut_handle_nav_event);



		// Prompt for the file
		theErr = NavGetFile(NULL, &navReply, &dialogOptions, navEventFilterUPP,
							NULL, NULL, NULL, NULL);



		// Clean up
		NavUnload();
		DisposeNavEventUPP(navEventFilterUPP);

		if (!navReply.validRecord)
			return(kQ3Failure);



		// Extract the file
		AEGetNthDesc(&navReply.selection, 1, typeFSS, &theAEKeyword, &theAEDesc);
		AEGetDescData(&theAEDesc, theFile, sizeof(FSSpec));
		}



	// Otherwise, use Standard File (but not on Carbon!)
	else
		{
#if TARGET_API_MAC_CARBON
		return(kQ3Failure);
#else
		StandardGetFile(NULL, -1, NULL, &sfReply);
		if (!sfReply.sfGood)
			return(kQ3Failure);

		memcpy(theFile, &sfReply.sfFile, sizeof(FSSpec));
#endif
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
	Rect					theRect;



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
    GetPortBounds(GetWindowPort(gWindow), &theRect);
	macDrawContextData.drawContextData.pane.min.x = (float) theRect.left;
	macDrawContextData.drawContextData.pane.min.y = (float) theRect.top;
	macDrawContextData.drawContextData.pane.max.x = (float) theRect.right;
	macDrawContextData.drawContextData.pane.max.y = (float) theRect.bottom;
	macDrawContextData.window	                  = gWindow;
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

#if QUT_MAC_CARBON_EVENTS

	// Create the Special menu
	gMenuSpecial = GetMenuHandle(kMenuSpecial);
    DeleteMenuItem(gMenuSpecial,1);

#else

	// Create the Special menu
	gMenuSpecial = NewMenu(kMenuSpecial, "\pSpecial");
	if (gMenuSpecial == NULL)
		return;


	// Insert the menu and redraw
	InsertMenu(gMenuSpecial, 0);
	DrawMenuBar();
#endif



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

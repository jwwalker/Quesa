// Revisions:
//
//	 7/05/98	JJS		added LoadHierMenus() to support hierarchical menus
//	 7/17/98	JJS		fixed bug in HandleOSEvent()
//	 7/18/98	JJS		fixed bug in HandleWindowActivate()
//	12/05/98	JJS		added mInBackground initializer to constructor
//	 7/30/99	JJS		misc. minor changes for improved functionality

#include "MoofApp.h"
#include "MoofResources.h"
#include "MoofWindow.h"
#include "Translator.h"

MoofApp *gApplication = 0;

MoofApp::MoofApp()
: mDone(false), mWindow(0), mStarted(false), mInBackground(false)
{
	// constructor -- set up toolbox, etc.
	if (gApplication) {
		// uh-oh! somebody's already created an application object!!!
		SysBeep(50);
		ExitToShell();
	}
	gApplication = this;
}

MoofApp::~MoofApp()
{
	gApplication = 0;
	if (mWindow) delete mWindow;
}

void MoofApp::Startup()
{
	if (mStarted) return;
	StartToolbox();
	
	#if !TARGET_API_MAC_CARBON
		SysEnvirons(1, &mSysEnv);
	#else
		// can't use SysEnvirons anymore -- use Gestalt instead...
		mSysEnv.hasColorQD = 1;		// (but color QD is *always* available in Carbon!)
	#endif
	if (!AppCanRun()) {
		SysBeep(50);
		ExitToShell();
	}
	
	StartMenus();
	StartWindows();	

	// clear out the event queue in case any stray clicks or keypresses are left there	
	FlushEvents( everyEvent, 0 );

	// note that we've started up
	mStarted = true;
}

Boolean MoofApp::AppCanRun()
{
	return (mSysEnv.hasColorQD);
}
	
void MoofApp::StartToolbox()
{
	/* Initialize all the needed managers. */
	#if !TARGET_API_MAC_CARBON
		InitGraf(&qd.thePort);
		InitFonts();
		InitWindows();
		InitMenus();
		TEInit();
		InitDialogs(nil);
	#endif
	InitCursor();
}

void MoofApp::StartMenus()
{
	// installs the menu bar. By default, we just install 'MBAR' ID = 128, which means you
	// don't need to override this to get other menus- just create the resources you want.

	Handle menuBar = GetNewMBar( kStdMenubarID );
	
	if ( menuBar )
	{
		SetMenuBar( menuBar );
		DisposeHandle( menuBar );
		
		// search for all the attached hierarchical menus and load those too.
		menuBar = GetResource( 'MBAR', kStdMenubarID );
		if (menuBar) {
			HLock( menuBar );
			
			short	mCount = **(short**) menuBar;	// first word in mBar is count of menus		
			for (short i = 0; i < mCount; i++ )
			{
				short menuID = ((short*) *menuBar)[ i + 1 ];
				LoadHierMenus( menuID );
				TranslateMenu( menuID );
			}
			
			HUnlock( menuBar );
			ReleaseResource( menuBar );
		}
	}

	// get the apple menu and add the DA names		
	#if !TARGET_API_MAC_CARBON		// DON'T do this under Carbon!
		MenuHandle theMenu = GetMenuHandle( kAppleMenuID );		
		if ( theMenu ) AppendResMenu( theMenu, 'DRVR' );
	#endif
	
	DrawMenuBar();	
}

void MoofApp::LoadHierMenus( const short parentMenuID )
{
	// This routine searches the given menu for hierarchical menu tags,
	// and installs them when found.  Many thanks to Graham Cox for his
	// most excellent "MacZoop" framework, from which this code was
	// pretty much directly ripped off.
	
	MenuHandle mH = GetMenuHandle( parentMenuID );	
	if ( mH ) {
		short iCount = CountMenuItems( mH );
		
		// loop through each item looking for the hier cmd mark
		for( short item = 1; item <= iCount; item++ ) {
			short iCmd;
			GetItemCmd( mH, item, &iCmd );
			
			// if this indicates a hierarchical submenu, fetch its ID
			if ( iCmd == hMenuCmd ) {
				short hResID;
				GetItemMark( mH, item, &hResID );
			
				// attempt to load the submenu				
				MenuHandle subMenu = GetMenu( hResID );
				
				if (subMenu) {
					InsertMenu( subMenu, hierMenu ); 
					TranslateMenu( hResID );
					// now scan that to see if it has any submenus of its own
					LoadHierMenus( hResID );
				}
			}
		}
	}
}

void MoofApp::StartWindows()
{
	//
	//	Make a new window for drawing in, and it must be a color window.  
	//
	#if TARGET_API_MAC_CARBON
		BitMap screenBits;
		GetQDGlobalsScreenBits(&screenBits);
		Rect windRect = screenBits.bounds;
	#else
		Rect windRect = qd.screenBits.bounds;
	#endif
	InsetRect(&windRect, 50, 50);
	windRect.right /= 2;
	windRect.bottom /= 2;
	mWindow = new MoofWindow( windRect, "\pUntitled" );
	
	mWindow->Focus();
}

void MoofApp::Tick()
{
	// make sure we're properly started
	if (!mStarted) Startup();
	
	// do one time through the event loop, or else just idle
	if (!GetAndHandleEvent()) Idle();

	// LATER: give time here to periodic tasks or works-in-progress
}

void MoofApp::Idle()
{
	// idle the window
	if (mWindow) mWindow->Idle();
}

void MoofApp::DoAboutBox()
{
	// For the simplest about box, just display an alert from the resource fork!
	// (The downside is that it insists on beeping; we could turn this off
	//	with ErrorSound(0L), but this would disable all alert sounds throughout
	//	the application.)
	
	Alert( kAlertBoxID, 0L );
}

Boolean MoofApp::GetAndHandleEvent(const long timeout)
{
	// Get one event and handle it.
	// If there were no events within timeout ticks, return false.

	EventRecord anEvent;
	if (!WaitNextEvent( everyEvent, &anEvent, timeout, NULL )) {
		IsDialogEvent(&anEvent);		// ensures modeless dialogs get balloon help, etc.
		return false;
	}
	else {
		IsDialogEvent(&anEvent);		// ensures modeless dialogs get balloon help, etc.
		DispatchEvent( anEvent );
	}
	return true;
}

void MoofApp::DispatchEvent( EventRecord &theEvent )
{
	// dispatch the event according to its type and location
	
	switch ( theEvent.what )
	{
		case mouseDown:
			HandleMouseEvent( theEvent );
			break;
		case activateEvt:
			HandleWindowActivate(	(WindowPtr) theEvent.message,
									(theEvent.modifiers & activeFlag) );
			break;
		case autoKey:
		case keyDown:
			char theKey = theEvent.message & charCodeMask;
			char theCode = (theEvent.message & keyCodeMask) >> 8;
			HandleKey( theKey, theCode, theEvent.modifiers );
			break;

		case updateEvt:
			HandleWindowUpdate((WindowPtr) theEvent.message );
			break;

		case diskEvt:
			#if !TARGET_API_MAC_CARBON
				Point dPt = {100,100};					
				if( HiWord( theEvent.message ) != 0)
					DIBadMount( dPt, theEvent.message );
			#endif
			break;
		case kHighLevelEvent:
			HandleHLEvent( theEvent );
			break;
		case osEvt:
			HandleOSEvent( theEvent );
			break;
	}
}

void MoofApp::HandleWindowActivate( const WindowPtr theWindow, const Boolean makeActive )
{
	// NOTE: I used to save and restore the port here, but that's rather silly --
	// If the window's inactivating, then it probably is the current port upon entrance,
	// but should no longer be current when we exit; if the window's activating, then
	// it wasn't the current port, but should be when we exit.  Either way, we don't
	// want to restore the previous port.
	MoofWindow *moofwin = RefConToMoofWin( GetWRefCon( theWindow ) );
	if (moofwin) {
		if (moofwin->mKillMe) {
			delete moofwin;
			if (mWindow == moofwin) {
				WindowPtr win = FrontWindow();
				if (win) mWindow = RefConToMoofWin( GetWRefCon( win ) );
				else mWindow = 0;
			}
		} else {
			moofwin->Focus();
			moofwin->Activate( makeActive );
		}
	}
}

void MoofApp::HandleWindowUpdate( const WindowPtr theWindow )
{
	GrafPtr			savePort;

	MoofWindow *moofwin = RefConToMoofWin( GetWRefCon( theWindow ) );
	
	GetPort( &savePort );
	if (moofwin) moofwin->Focus();
		
	BeginUpdate( theWindow );
	if (moofwin) moofwin->Draw();
	EndUpdate( theWindow );
		
	SetPort(savePort);	
}


void MoofApp::HandleMouseEvent( EventRecord &theEvent )
{
	WindowPtr		targetWindow;
	short			partCode;
	long			menucode;
	Point			mousePt;
	
	// find out what part of the screen the mouse was clicked on
	
	partCode = FindWindow( theEvent.where, &targetWindow );
	
	switch (partCode)
	{	
		case inDesk:
			break;
		
		case inMenuBar:
			// the user clicked the menubar...
			menucode = MenuSelect( theEvent.where );	// pull down the menu and track it
			
			// handle the user's choice	
			if ( HiWord( menucode ))
				HandleMenuSelection( HiWord(menucode), LoWord(menucode) );
			HiliteMenu(0);

			break;
		
		case inSysWindow:
			#if !TARGET_API_MAC_CARBON
				SystemClick( &theEvent, targetWindow );	// system will handle it (DA window)
			#endif
			break;
	
		case inContent:
			if ( targetWindow )						// the user clicked in a window
			{
				if ( targetWindow == (WindowPtr)FrontWindow() ) {
					// if the window is already active, pass the click to the window object
					// in its local coordinate system.
					
					MoofWindow *moofwin = RefConToMoofWin( GetWRefCon(targetWindow) );
					if (moofwin) {
						moofwin->Focus();
						Point mousePt = theEvent.where;
						GlobalToLocal( &mousePt );
						moofwin->Click( mousePt, theEvent.modifiers );	
					}
				}
				else
				{
					// If the window wasn't active, make it active.
					
					// (If your app supports modal dialogs, then you'll want to check
					//	to make sure the front window isn't one of these.)
					
					SelectWindow( targetWindow );

				}
			}
			break;

		case inDrag:
			// Drag the window.
			// Again, disallow this if the front window is modal.
			#if TARGET_API_MAC_CARBON
				DragWindow( targetWindow, theEvent.where, NULL );
			#else
				Rect r = (*GetGrayRgn())->rgnBBox;
				InsetRect( &r, 4, 4 );
				DragWindow( targetWindow, theEvent.where, &r );
			#endif
			break;
		
		case inGrow:
		{
			// the user wants to resize the window
			// first, get min and max bounds
			Rect r;	// minimum and maximum window size
			MoofWindow *moofwin = RefConToMoofWin( GetWRefCon(targetWindow) );
			if (moofwin) {
				SetRect( &r, moofwin->mMinWidth, moofwin->mMinHeight,
							 moofwin->mMaxWidth, moofwin->mMaxHeight );
			} else {
				SetRect( &r, 100, 50, 640, 800 );	// arbitrary bounds for non-Moof windows!
			}
			// now, drag the stretchy window outline
			long newSize = GrowWindow( targetWindow, theEvent.where, &r );
			if (newSize) {
				short newHeight = HiWord(newSize);
				short newWidth = LoWord(newSize);
				// finally, do the actual resize
				if (moofwin) {
					moofwin->Resize( newWidth, newHeight );
				} else {
					SizeWindow( targetWindow, newWidth, newHeight, true );
					Rect invalr = {0,0,newHeight,newWidth};
					#if TARGET_API_MAC_CARBON
						InvalWindowRect( targetWindow, &invalr );
					#else
						InvalRect( &invalr );
					#endif
				}
			}
			break;
		}
/*		
		case inGoAway:
			if( TrackGoAway( targetWindow, theEvent.where ))
			{
				// if the user clicks in the go-away box, the window will be closed. If the
				// option key is down, we close all of the windows.
				
				if ( theEvent.modifiers & optionKey )
					gApplication->CloseAll();
				else
				{
					if ( zappWindow )
						zappWindow->Close( gApplication->GetPhase());
				}
			}
			break;
		
		case inZoomIn:
		case inZoomOut:
			if( TrackBox( targetWindow, theEvent.where, partCode ))
			{
				// if the user clicked in the zoom box, the window will be zoomed
				
				if( zappWindow )
					zappWindow->Zoom( partCode );
			}
			break;
*/
	}

}

void MoofApp::HandleMenuSelection( const short menuNum, const short itemNum )
{
	// User has selected a menu command.
	// Here, we'll handle only a few default cases.
	if (menuNum == kAppleMenuID) {
		if (itemNum == 1) {
			// show the about box
			DoAboutBox();
		} else {
			// launch Apple Menu item... not needed under carbon
			#ifndef TARGET_API_MAC_CARBON
				Str255 appleItemName;
				GetMenuItemText( GetMenuHandle(menuNum), itemNum, appleItemName );
				GrafPtr savePort;
				GetPort( &savePort );
				OpenDeskAcc( appleItemName );
				SetPort( savePort );
			#endif
		}
		return;
	}
	
	if (menuNum == kFileMenuID) {
		// for the file menu, we only handle "quit" here.
		// We'll assume that's the last item in the File menu.
		if (itemNum == CountMenuItems( GetMenuHandle(menuNum) )) {
			// QUIT!
			mDone = true;
		}
	}
}

void MoofApp::HandleKey( const char key, const char keycode, const short modifiers )
{
	// A key has been pressed -- handle typical cases.
	if (modifiers & cmdKey) {
		// command key was pressed -- check for menu shortcut.
		long menucode = MenuKey( key );		
		// handle the user's choice	
		if ( HiWord( menucode )) {
			HandleMenuSelection( HiWord(menucode), LoWord(menucode) );
			HiliteMenu(0);
			return;
		}
	}
	
	// obscure the cursor so it doesn't obscure what you're typing
	// (yet one more thing that makes MacOS better than WinXX!)
	ObscureCursor();
	
	// pass it to the window
	if (mWindow) mWindow->HandleKey( key, keycode, modifiers );
}


/*------------------------------***  HANDLEOSEVENT  ***---------------------------------*/
/*	

handle the suspend and resume events sent by MultiFinder/System 7

----------------------------------------------------------------------------------------*/

void	MoofApp::HandleOSEvent( const EventRecord& theEvent )
{
	if ( theEvent.message & suspendResumeMessage )
	{
		mInBackground = FALSE;
	}
	else
	{
		mInBackground = TRUE;
	}
	if (mWindow) mWindow->Activate( !mInBackground );
}


/*------------------------------***  HANDLEHLEVENT  ***---------------------------------*/
/*	

handle the high level events sent by AOCE, Applescript, et al.

----------------------------------------------------------------------------------------*/

void	MoofApp::HandleHLEvent(const EventRecord& theEvent)
{
	// handles high level events such as applescript events
	
	OSErr err = AEProcessAppleEvent( &theEvent );
	
	if (err != errAEEventNotHandled &&
		err != errAECantSupplyType ) {
		//FailOSErr(err);
	}
}

MoofWindow* MoofApp::RefConToMoofWin(const long refcon) const
{
	// return a pointer to the Moof window which corresponds to the given refcon
	// if no such window, then return NULL
	
	// default implementation assumes refcon is a pointer to the window,
	// but we'll at least check to see if it's within our memory space
// NOTE: under Carbon, pointers may be negative...  ick.
#if !TARGET_API_MAC_CARBON
	if (refcon > 1024 /* and refcon < (long)TopMem() */) // LATER: do a better check here!!!
#endif
	{
//		return dynamic_cast<MoofWindow*> (refcon);
		return (MoofWindow*)(refcon);
	}
	return 0;
}

long MoofApp::MoofWinToRefCon(const MoofWindow &win) const
{
	// return a refcon (long int) which can later be used to find the MoofWindow
	
	return (long)(&win);
}


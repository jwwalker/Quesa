#include "AdvApp.h"
#include "MenuWindow.h"
#include "ResourceIDs.h"
#include <Balloons.h>

Boolean gHaveAppearanceMgr = false;
AEDesc gSelfAddrDesc;

// function to throw an exception if there is an OSErr
static inline void FailOSErr(OSErr x)
{
	if (x!=noErr) throw x;
}

AdvApp::AdvApp()
{
	// start the appearance manager
	StartAppearance();

	// start up apple-event handling
	SetupMomSupport();
	
	// set up a reasonable default for file types we can open
	if (!mOpenTypes) {
		mQtyOpenTypes = 1;
		mOpenTypes = new OSType[1];
		mOpenTypes[0] = kDocFileType;
	}

	// get our own AppleEvent address
	ProcessSerialNumber self;
	self.lowLongOfPSN = kCurrentProcess;
	self.highLongOfPSN = 0;
	
	OSErr err = AECreateDesc(typeProcessSerialNumber, &self, 
                                 sizeof(ProcessSerialNumber), &gSelfAddrDesc);
}

void AdvApp::StartAppearance()
{
	// Register with the Appearance Manager, if present.

	OSStatus err;
	long response;
	Boolean haveAppearance;
	// Attempt to call Gestalt; if we succeed, test for presence of Appearance Mgr
	if (!(err = Gestalt (gestaltAppearanceAttr, &response))) {
		gHaveAppearanceMgr = response & (1 << gestaltAppearanceExists) ? true : false;
	}
	else if (err == gestaltUndefSelectorErr)
	{
		// If the Appearance Mgr selector is undefined, the Appearance Mgr is not present
		gHaveAppearanceMgr = false;
	}
	if (!gHaveAppearanceMgr) return;		// no Appearance Manager, so just return

	err = RegisterAppearanceClient();
	ASSERT(noErr == err or themeProcessRegisteredErr == err);
}

void AdvApp::StartWindows()
{
	// Note: since we respond to AppleEvents, we don't need to automatically
	//	create a new window; instead, we'll do it in response to the kAEOpenApplication
	//	event.  That way, we avoid creating an empty window when launched for printing,
	//	etc.
//	mWindow = CreateWindow();
}

MenuWindow* AdvApp::CreateWindow()
{
	//
	//	Make a new window of the "MenuWindow" type.
	//
	#if TARGET_CARBON
		Rect windRect = {0,0,480,640};
	#else
		Rect windRect = qd.screenBits.bounds;
	#endif
	InsetRect(&windRect, 50, 50);
	windRect.right /= 2;
	windRect.bottom /= 2;
	MenuWindow *win = new MenuWindow( windRect, "\pUntitled" );
	win->Focus();
	return win;
}


void AdvApp::HandleMouseEvent( EventRecord &theEvent )
{
	// handle some mouse events not handled by the basic MoofApp.
	
	WindowPtr		targetWindow;
	short			partCode;
	long			menucode;
	Point			mousePt;
	
	// find out what part of the screen the mouse was clicked on
	
	partCode = FindWindow( theEvent.where, &targetWindow );
	
	switch (partCode)
	{	
		case inGoAway:
			if( TrackGoAway( targetWindow, theEvent.where ))
			{
				// if the user clicks in the go-away box, the window will be closed. If the
				// option key is down, we close all of the windows (LATER!)
/*				
				if ( theEvent.modifiers & optionKey )
					HandleCloseAll();
				else
				{
*/
					MenuWindow *win = (MenuWindow*)RefConToMoofWin( GetWRefCon(targetWindow) );
					HandleClose(win);
//				}
			}
			return;
	}
	
	inherited::HandleMouseEvent( theEvent );
}

void AdvApp::HandleMenuSelection( const short menuNum, const short itemNum )
{
	//	Handle common file commands
	if (menuNum == kFileMenuID) {
		switch (itemNum) {
			case kNewItem: HandleNew(); return;
			case kOpenItem: HandleOpen(); return;
			case kCloseItem: HandleClose(); return;
			case kQuitItem: HandleQuit(); return;
		}
	}
	
	// try to pass it to the frontmost window (if any)
	// NOTE: this will crash if the window is not derived from MenuWindow!!!
	WindowPtr wptr = FrontWindow();
	if (wptr) {
		MenuWindow *win = (MenuWindow*)RefConToMoofWin( GetWRefCon(wptr) );
		if (win->HandleMenuSelection( menuNum, itemNum )) return;
	}

	// if not handled, give the default implementation a try
	inherited::HandleMenuSelection( menuNum, itemNum );
}

void AdvApp::HandleNew()
{
	mWindow = CreateWindow();
}

void AdvApp::HandleOpen()
{
	// here we just use Opener to post a file dialog...
	DoOpenFile();
}

OSErr AdvApp::OpenDoc(const FSSpec& spec)
{
	// create a document window from the given filespec
	MenuWindow *win = CreateWindow();
	if (win) {
		win->InitFromFile( spec );
		return noErr;
	}
	else return memFullErr;
}

short AdvApp::OpenDocBatch(AEDescList &list)
{
	// unlike the standard Opener method, here we'll take the list
	// of files to open and send it to self via an AppleEvent,
	// making the opening Recordable!
	
	AppleEvent myAppleEvent, defReply;

	// 1. create the 'open documents' event, addressed to self
	OSErr err = AECreateAppleEvent(kCoreEventClass, kAEOpenDocuments,
			&gSelfAddrDesc,
			kAutoGenerateReturnID, 
			kAnyTransactionID, &myAppleEvent);
	if (err) return 0;
	
	// 2. set the direct object (files to open)
	AEPutParamDesc(&myAppleEvent, keyDirectObject, &list);

	// 3. finally, send this event
 	err = AESend( &myAppleEvent, &defReply, 
                    kAENoReply+kAEAlwaysInteract,
                    kAENormalPriority, kAEDefaultTimeout,
                    NULL, NULL);

	AEDisposeDesc( &myAppleEvent );
	long count = 1;
	AECountItems( &list, &count );
	return count;
}

void AdvApp::OpenerWindowUpdate(WindowPtr window)
{
	// If it's a Moof window, tell it to refresh!
	MoofWindow *moofwin = RefConToMoofWin( GetWRefCon(window) );
	if (moofwin) {
		CGrafPtr oldPort;
		GDHandle oldDevice;
		GetGWorld(&oldPort, &oldDevice);
		moofwin->Focus();
		BeginUpdate(window);
		moofwin->Draw();
		EndUpdate(window);
		SetGWorld(oldPort, oldDevice);
	}
	else {
		Opener::OpenerWindowUpdate(window);
	}
}

void AdvApp::HandleClose(MenuWindow *winToClose)
{
	// find frontmost window
	MenuWindow *win = winToClose;
	if (not win) {
		WindowPtr wptr = FrontWindow();
		if (wptr) win = (MenuWindow*)RefConToMoofWin( GetWRefCon(wptr) );
	}
	if (win) {
		// pass the request to the window...
		// if it returns true, delete the window, else don't
		if (win->CanClose()) {
			win->CloseSelf();
//			win->Activate(false);
			if (win->mKillMe) delete win;
			if (win == mWindow) mWindow = 0L;
		}
		return;
	}
}

void AdvApp::HandleKey( const char key, const char keycode, const short modifiers )
{
	// Check for help key, and toggle balloon help.
	// Otherwise, do the standard thing.
	#if !TARGET_CARBON
		if (keycode == 0x72 and !(modifiers & cmdKey)) {
			HMSetBalloons( !HMGetBalloons() );
		}
		else
	#endif
	inherited::HandleKey(key, keycode, modifiers);
}

void AdvApp::HandleQuit()
{
	// try closing each window
	WindowPtr wptr = FrontWindow();
	while (wptr) {
		MoofWindow *win = RefConToMoofWin( GetWRefCon(wptr) );
		if (win->CanClose(true)) {
			win->Activate(false);
			delete win;
		}
		else return;		// quit cancelled!
		if (win == mWindow) mWindow = 0;
		wptr = FrontWindow();
	}
	// if we made it this far, then let the inherited method finish it
	inherited::HandleMenuSelection( kFileMenuID, kQuitItem );
	return;
}

void AdvApp::Idle()
{
	// make the frontmost window mWindow
	WindowPtr wptr = FrontWindow();
	#if TARGET_API_MAC_CARBON
		// (skip help-tag windows, which for some odd reason show up in our window list)
		WindowClass c;
		while (wptr and noErr == GetWindowClass(wptr, &c) and kHelpWindowClass == c) {
			wptr = GetNextWindow(wptr);
		}
	#endif
	if (wptr) {
		MenuWindow *win = (MenuWindow*)RefConToMoofWin( GetWRefCon(wptr) );
		if (win and win->GetWindowPtr() == wptr) {
			mWindow = win;
		}
	}
	
	// then, do usual stuff
	inherited::Idle();
}

/*----------------------------***  HANDLEAPPLEEVENT  ***--------------------------------*/
/*	
Handle the four required apple events. If you override this to handle your own apple
events, be sure to call the inherited method for everything else.
----------------------------------------------------------------------------------------*/

void	AdvApp::HandleAppleEvent(	AEEventClass aeClass, AEEventID aeID, const AEDesc* token,
										const AppleEvent* aeEvt, AppleEvent* reply )
{
	if ( aeClass == kCoreEventClass )
	{
		FSSpec		aFile;
		AEDescList	docList;
		long		i, n;
		AEKeyword	keyWrd;
		DescType	retType;
		Size		actualSize;
		FInfo		fi;
		
		switch ( aeID )
		{
			case kAEOpenApplication:
				HandleNew();
				break;
			
			case kAEOpenDocuments:
				FailOSErr( AEGetParamDesc( aeEvt, keyDirectObject, typeAEList, &docList ));	
				FailOSErr( AECountItems( &docList, &n ));
				
				// get each document and open it
				try
				{
					for (i = 1; i <= n; i++)
					{
						FailOSErr( AEGetNthPtr( &docList,
												i, 
												typeFSS, 
												&keyWrd,
												&retType, 
												&aFile, 
												sizeof( FSSpec ), 
												&actualSize ));
						
//						FailOSErr( FSpGetFInfo( &aFile, &fi ));
						
						// open the file into a window, i.e.:
						// create a window, set its fsspec, and give it the proper name
						MenuWindow *win = CreateWindow();
						if (win) {
							win->InitFromFile( aFile );
						}
					}
				}
				catch( OSErr err )
				{
					AEDisposeDesc( &docList );
					
					throw err;
				}
				FailOSErr( AEDisposeDesc( &docList ));
				break;
			
			case kAEPrintDocuments:
				FailOSErr( AEGetParamDesc( aeEvt, keyDirectObject, typeAEList, &docList ));	
				FailOSErr( AECountItems( &docList, &n ));
				
				// ask the application to show the page setup dialog
				
				// LATER... DoPageSetup();
				
				// get each document passed by the finder
				
				try
				{
					for (i = 1; i <= n; i++)
					{
						FailOSErr( AEGetNthPtr( &docList,
												i, 
												typeFSS, 
												&keyWrd,
												&retType, 
												&aFile, 
												sizeof( FSSpec ), 
												&actualSize ));
						
						FailOSErr(FSpGetFInfo( &aFile, &fi ));
						
						// open the file into a window and select it
						
						// LATER... OpenFile( aFile, fi.fdType );
						
						// update the window (not strictly needed, but looks better)
						
						// LATER... zEH->HandleWindowUpdate( mostRecent->GetMacWindow());
						
						// ask the application to print the document
						
						// LATER... DoPrint();
						
						// that done, we can close the window and move on to the next
						
						//mostRecent->Close( GetPhase());
					}
				}
				catch( OSErr err )
				{
					AEDisposeDesc( &docList );
					
					throw err;
				}
				
				FailOSErr( AEDisposeDesc( &docList ));
				break;
			
			case kAEQuitApplication:
				HandleQuit();
				break;
			
			default:
				MomObject::HandleAppleEvent( aeClass, aeID, token, aeEvt, reply );
				break;	
		}
	}
	else
		MomObject::HandleAppleEvent( aeClass, aeID, token, aeEvt, reply );
}

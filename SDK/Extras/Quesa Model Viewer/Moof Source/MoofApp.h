//	MoofApp.h													7/05/98 JJS
//
//	This file defines the "MoofApp" class -- the base class for any Moof application.
//	There must be exacty one MoofApp created for your application.  All you have to
//	do is this:
//
//		1. create a MoofApp (we'll call it yourApp for this example).
//		2. call yourApp.Tick() until you find yourApp.mDone is true.
//
//	That's it!  The rest of the functions are provided for you to override; you'll
//	probably want to at least override StartWindows() to create your custom window
//	type.

#ifndef __MoofApp_H
#define __MoofApp_H

#ifndef __QUICKDRAW__		// (easiest way to check whether MacHeaders have been included)
#include <MacHeaders.h>
#endif

class MoofWindow;

class MoofApp
{
  public:
	MoofApp();		// constructor
	virtual ~MoofApp();		// destructor

	virtual void Startup();			// initializes toolbox, creates windows, etc.
	virtual void StartToolbox();	// initializes toolbox
	virtual Boolean AppCanRun();	// checks to make sure the app can run
	virtual void StartMenus();		// initializes the menus
	virtual void LoadHierMenus( const short parentMenuID );	// helps initialize menus
	virtual void StartWindows();	// creates the initial window(s)

	// do one time through the event loop (mainly, idle)
	virtual void Tick();
	
	// twiddle thumbs while nothing better to do -- also, idle window
	virtual void Idle();
	
	// show the about box -- by default, just shows ALRT resource number kAlertBoxID
	virtual void DoAboutBox();
	
	// get one event and handle it -- return whether there was an event
	virtual Boolean GetAndHandleEvent(const long timeout=10);
	
	// various event handling
	virtual void DispatchEvent( EventRecord &theEvent);
	virtual void HandleMouseEvent( EventRecord &theEvent);
	virtual void HandleHLEvent( const EventRecord& theEvent);
	virtual void HandleOSEvent( const EventRecord& theEvent );
	virtual void HandleKey( const char key, const char keycode, const short modifiers );
	virtual void HandleWindowActivate( const WindowPtr theWindow, const Boolean makeActive );
	virtual void HandleWindowUpdate( const WindowPtr theWindow );

	// handle a menu selection -- you'll probably want to override this,
	//	but still call the inherited function to handle the Apple Menu and Quit
	virtual void HandleMenuSelection( const short menuNum, const short itemNum );

	// utility functions for finding app windows from Mac windows, and vice versa
	virtual MoofWindow *RefConToMoofWin(const long refcon) const;
	virtual long MoofWinToRefCon(const MoofWindow &win) const;

	// public member variables
	Boolean		mDone;			// when set to true, app quits
	Boolean		mInBackground;	// true while running in background
	MoofWindow	*mWindow;		// pointer to main window
	SysEnvRec	mSysEnv;		// system environment data

  protected:
	// protected member variables
	Boolean		mStarted;		// true when we've started toolbox, etc.
};

// global pointer to the application
extern MoofApp *gApplication;

#endif

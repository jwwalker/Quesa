//	AdvApp.h											JJS 3/11/98
//
//	This is a MoofApp derivative which is designed to work with MenuWindow
//	windows (i.e., those which can handle menu selections).
//	It assumes all the windows it finds are of this type, and
//	passes menu selections, etc. to the frontmost window.

#ifndef __ADVAPP_H
#define __ADVAPP_H

#include "MoofApp.h"
#include "MoofOM.h"
#include "Opener.h"

class MenuWindow;

class AdvApp : public MoofApp, public MomObject, public Opener
{
	typedef MoofApp inherited;			// required in CWPro2; prohibited in earlier versions
  public:
	AdvApp();
	virtual DescType GetClass() { return 'capp'; }
	virtual void StartAppearance();		// start the Appearance Manager
	virtual void StartWindows();		// creates the initial window(s)
	virtual MenuWindow* CreateWindow();	// create one window (a lˆ New or Open)
	
	virtual void HandleAppleEvent(	AEEventClass aeClass, AEEventID aeID, const AEDesc* token,
										const AppleEvent* aeEvt, AppleEvent* reply );
	virtual void HandleMouseEvent( EventRecord &theEvent );
	virtual void HandleKey( const char key, const char keycode, const short modifiers );

	// handle a menu selection -- or pass it to the frontmost window
	virtual void HandleMenuSelection( const short menuNum, const short itemNum );

	// standard command handlers
	virtual void HandleOpen();
	virtual void HandleNew();
	virtual void HandleClose(MenuWindow *winToClose=0);
	virtual void HandleQuit();
	
	// idle the frontmost window
	virtual void Idle();
	
	// methods overriding standard Opener behavior for opening documents:
	virtual OSErr OpenDoc(const FSSpec& filespec);	
	virtual short OpenDocBatch(AEDescList &list);
	virtual void OpenerWindowUpdate(WindowRef window);
};

// global flag indicating whether the Appearance Manager is available
extern Boolean gHaveAppearanceMgr;

// public AppleEvent identifier of the application
extern AEDesc gSelfAddrDesc;

// handy debugging macro
#ifndef ASSERT
#define ASSERT(x) if (!(x)) DebugStr("\pAssert Failed!");
#endif

#endif

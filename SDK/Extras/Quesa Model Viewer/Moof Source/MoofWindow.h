//	MoofWindow.h												 7/12/98 JJS
//
//	This file defines the "MoofWindow" class -- the base class for a window that
//	uses basic Moof services.  Note that you can use non-Moof windows in your
//	application too, but they won't receive automatically receive events like
//	MoofWindows do.
//
//	You'll probably want to derive your own window class from this, and override
//	the Draw method to draw the window contents, and the Click and/or HandleKey
//	methods to make it respond to the user.

// Revision history:
//
//	 7/12/98	JJS		changed 'modifiers' param of Click() from long to short
//	 3/10/99	JJS		added inclusion of <MacHeaders.h>

#ifndef __MoofWindow_H
#define __MoofWindow_H

#ifndef __QUICKDRAW__		// (easiest way to check whether MacHeaders have been included)
#include <MacHeaders.h>
#endif

#include "MoofApp.h"

class MoofWindow
{
  public:
	// constructor -- pass in the window bounds, its title, and (optionally)
	//	the type of window (see Mac programming docs under "NewWindow")
	MoofWindow(const Rect &r, const Str255 title, 
			   const short defProc=documentProc, const Boolean hasCloseBox=true);
	
	// initialize from a file (does nothing here -- override)
	virtual void InitFromFile(const FSSpec&) {}
	
	// destructor
	virtual ~MoofWindow();
	
	// get the WindowPtr (for passing to toolbox functions)
	virtual WindowPtr GetWindowPtr() { return mWindowPtr; }
	
	// get the port rect
	virtual Rect& GetPortRect();

	// set the port to the window (constrains drawing, etc.)
	virtual void Focus();
	
	// determine whether it's OK to close this window
	virtual Boolean CanClose(Boolean quitting=false) { return true; }
	
	// draw the window contents (OVERRIDE ME)
	virtual void Draw();
	
	// handle an activate/deactive event -- by adjusting appearance of size box, etc.
	virtual void Activate( const Boolean becomingActive=true )
		{ if (becomingActive and gApplication) gApplication->mWindow = this; }
	
	// override standard 'Alert' behavior, to Activate/Deactivate window as it should
	virtual short Alert(short alertID, ModalFilterUPP procPtr=NULL)
		{ Activate(false); short out = ::Alert(alertID, procPtr); Activate(true); return out; }
	
	// unhide and become the frontmost window
	virtual void Select()
		{ SelectWindow(mWindowPtr); }
	
	// force an update event next time through the event loop
	virtual void Refresh( const Rect *subrect=0 );
	
	// twiddle proverbial thumbs
	virtual void Idle() {}
	
	// handle clicks and keypresses
	virtual void Click( const Point p, const short modifiers );
	virtual void HandleKey( const char key, const char keycode, const short modifiers ) {}
	
	// change size (usually, because of dragging the grow box)
	virtual void Resize( const short newWidth, const short newHeight );
	
	// method called by a window when it wants to safely destroy itself;
	// also called by AdvApp when closing a window
	virtual void CloseSelf()
		{ if (!mKillMe) { mKillMe = true; Activate(false); if (mWindowPtr) HideWindow(mWindowPtr); } }
	
	// PUBLIC DATA:
	short	mMaxWidth;		// maximum window width
	short	mMinWidth;		// minimum window width
	short	mMaxHeight;		// maximum window height
	short	mMinHeight;		// minimum window height

	Boolean	mKillMe;		// if true, app should delete this window
		
  protected:
	// do-nothing constructor -- only for use by derived classes -- use with caution!
	MoofWindow() : mWindowPtr(0), mKillMe(false) {}

	// protected data:
	WindowPtr	mWindowPtr;

  private:
	#if TARGET_API_MAC_CARBON
		// temp storage of the port rect, filled in by GetPortRect:
		Rect mPortRect;
	#endif
};

#endif

//	MenuWindow.h											7/16/98 JJS
//
//	This class extends the basic MoofWindow functionality by providing methods
//	for handling menu commands (in particular, edit menu).  It is the base
//	type of all windows used with AdvApp.

#ifndef __MENUWINDOW_H
#define __MENUWINDOW_H

#include "MoofWindow.h"


class MenuWindow: public MoofWindow
{
	typedef MoofWindow inherited;
	
  public:
	MenuWindow(const Rect &r, const Str255 title,
			  const short defProc=documentProc, const Boolean hasCloseBox=true)
	: MoofWindow( r, title, defProc, hasCloseBox ) {}

	// on activation/deactivation, update standard menus
	virtual void Activate( const Boolean becomingActive=true );

	// handle a menu command -- return true if handled, false otherwise
	virtual Boolean HandleMenuSelection( const short menuNum, const short itemNum );

	// map F1-F4 to standard editing operations
	virtual void HandleKey( const char key, const char keycode, const short modifiers );

	// perform the basic editing operations
	virtual void DoUndo()		{}
	virtual void DoCopy()		{}
	virtual void DoCut()		{ DoCopy(); DoClear(); }
	virtual void DoPaste()		{}
	virtual void DoClear()		{}

  protected:
	// do-nothing constructor -- only for use by derived classes -- use with caution!
	MenuWindow() {}
};

#endif

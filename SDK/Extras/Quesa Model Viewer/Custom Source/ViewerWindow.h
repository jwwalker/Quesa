//	ViewerWindow.h

//	This is the header file for your custom application class.
//	Here, declare any methods which override or add to the 
//	standard MoofWindow methods.

#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include "AdvWindow.h"

//#if TARGET_API_MAC_CARBON
	#include <QuesaViewer.h>
//#else
//	#include <QD3DViewer.h>
//#endif

class ViewerWindow: public AdvWindow
{
  typedef AdvWindow inherited;
  public:
	// constructor -- pass in the window bounds, its title, and (optionally)
	//	the type of window (see Mac programming docs under "NewWindow")
	ViewerWindow(const Rect &r, const Str255 title, const short defProc=documentProc);
	virtual ~ViewerWindow();

	// class functions...
	
	// handle the event and return true, or return false if not handled
	static bool DispatchEvent(EventRecord &theEvent);

	// member functions...

	virtual void HandleError(OSErr err);	// inform the user of an error

	// load data into the previewer, from file or memory buffer
	void LoadData( FSSpec &spec );
	void LoadData( void* data, long bytes );

	// read data from a file
	virtual OSErr ReadFromFile(const short fileRefNum);
	
	// draw the window contents
	virtual void Draw();

	// on activation/deactivation, update menus
	virtual void Activate( const Boolean becomingActive=true );

	// handle a menu command -- return true if handled, false otherwise
	virtual Boolean HandleMenuSelection( const short menuNum, const short itemNum );

	// twiddle proverbial thumbs
	virtual void Idle();
	
	// handle clicks and keypresses
	virtual void Click( const Point p, const short modifiers );
	virtual void HandleKey( const char key, const char keycode, const short modifiers );
	
	// change size (usually, because of dragging the grow box)
	virtual void Resize( const short newWidth, const short newHeight );

	// editing functions
	virtual void DoCopy() { if (mViewer) Q3ViewerCopy(mViewer); }
	virtual void DoCut() { if (mViewer) Q3ViewerCut(mViewer); }
	virtual void DoPaste() { if (mViewer) Q3ViewerPaste(mViewer); }
	virtual void DoClear() { if (mViewer) Q3ViewerClear(mViewer); }

  private:
	TQ3ViewerObject mViewer;
	bool	mTracking;
};

#endif

//		Pane.h
//		http://www-acs.ucsd.edu/~jstrout/macdev/panes/
//		last modified: 11/14/97

//	11/14/97:	added virtual destructor -- required if any derived classes
//				have a destructor

//	 3/12/99:	added HandleKey and Idle methods

#ifndef __PANE_H
#define __PANE_H

#ifndef __QUICKDRAW__
#include <MacHeaders.h>
#endif

class Pane
{
  public:

	Pane() : mHidden(false)
		{ mFrame.top = 20; mFrame.left = 20; mFrame.right = 100; mFrame.bottom = 100; }
	virtual ~Pane() {}

	// draw the pane (assumes grafport coordinates are set up)
	virtual void Draw()=0;

	// try to handle a click -- return 1 if handled
	virtual Boolean Click(Point where, short modifiers)
		{ return false; }
	
	// try to handle a keypress -- return 1 if handled
	virtual Boolean HandleKey(char key, char keycode, short modifiers)
		{ return false; }
	
	// do some thumb-twiddling while nothing else is going on;
	// IF maxSleep is not null, you may set it to max. time till next idle
	virtual void Idle(short *maxSleep=NULL) {}

	// adjust to a change in frame
	virtual void Adjust() {}

	// return 1 if the given point is within this pane
	virtual Boolean	Contains(Point where)
		{ return (mHidden ? 0 : PtInRect(where, &mFrame)); }

	Rect			mFrame;			// bounding frame
	Boolean			mHidden;		// set to true if pane is "hidden"
};

#endif

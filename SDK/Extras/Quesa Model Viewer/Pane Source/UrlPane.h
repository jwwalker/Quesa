//		UrlPane.h
//		http://www-acs.ucsd.edu/~jstrout/macdev/panes/
//		last modified: 6/11/97

//	To use this pane, you must have the Internet Config API directory
//	in your system path, and you must include the library (e.g.,
//	ICGluePPC.lib) and TextPane.cpp in your project.

#ifndef __URLPANE_H
#define __URLPANE_H

#include "TextPane.h"

class UrlPane : public TextPane
{
  public:
	UrlPane() : TextPane(), mCreator('????') { mURL[0] = 0; }
	UrlPane( const Str255 pText ) : TextPane( pText ), mCreator('????') {}
	UrlPane( const Str255 pText, const Str255 pURL ) : TextPane( pText ), mCreator('????')
		{ BlockMove( pURL, mURL, pURL[0]+1 ); }

	// try to handle a click by launching the URL -- return 1 if successful
	virtual Boolean Click(Point where, short modifiers);
	
	// member variables
	Str255		mURL;			// URL to launch (if different from mText)
	OSType		mCreator;		// creator code for Internet Config's edification
};

#endif

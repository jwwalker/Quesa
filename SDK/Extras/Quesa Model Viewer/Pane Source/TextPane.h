//		TextPane.h
//		http://www-acs.ucsd.edu/~jstrout/macdev/panes/
//		last modified: 2/01/97

#ifndef __TEXTPANE_H
#define __TEXTPANE_H

#include "Pane.h"

class TextPane : public Pane
{
  public:
	enum tVAlign { top, center, bottom };
	enum tHAlign { left, middle, right };

	TextPane() : Pane(), mFont(systemFont), mSize(12), mStyle(0),
				 mHAlign(left), mVAlign(top), mShadow(0), mRestoreFont(1)
		{ mColor.red = mColor.blue = mColor.green = 0; 
		  mShadowColor.red = mShadowColor.blue = mShadowColor.green = 0x8000;
		  mText[0] = 0; }
	TextPane( const Str255 pText ) : Pane(), mFont(systemFont), mSize(12), mStyle(0),
				 mHAlign(left), mVAlign(top), mShadow(0), mRestoreFont(1)
		{ mColor.red = mColor.blue = mColor.green = 0; 
		  mShadowColor.red = mShadowColor.blue = mShadowColor.green = 0x8000;
		  BlockMove( pText, mText, pText[0]+1 ); }

	// draw the pane (assumes grafport coordinates are set up)
	virtual void Draw();

	// member variables -- examples are shown at right
	
	Str255		mText;			// string to be drawn		"\pFoo Bar"
	short		mFont;			// font		 				monaco
	short		mSize;			// size in points			24
	Style		mStyle;			// style					bold | italic
	tHAlign		mHAlign;		// horizontal alignment		TextPane::left
	tVAlign 	mVAlign;		// vertical alignment		TextPane::center
	RGBColor	mColor;			// text color (defaults to black)
	Boolean		mShadow;		// draw a shadow?
	RGBColor	mShadowColor;	// shadow color (defaults to grey)
	Boolean		mRestoreFont;	// restore previous font settings?
};

#endif

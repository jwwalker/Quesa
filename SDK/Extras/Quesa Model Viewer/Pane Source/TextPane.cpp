//		TextPane.cpp
//		http://www-acs.ucsd.edu/~jstrout/macdev/panes/
//		last modified: 2/12/01
//
//	1/19/97: changed position when VAlign=top to eliminate wasted space
//	2/12/01: Carbonized

#include "TextPane.h"

void TextPane::Draw()
{
	if (mHidden || !mText[0]) return;

	FontInfo	fi;
	short	lineDots;

	// store previous font info if set to do so
	short prevFont, prevSize;
	Style prevStyle;
	if (mRestoreFont) {
		GrafPtr thePort;
		GetPort( &thePort );
		#if TARGET_API_MAC_CARBON
			prevFont = GetPortTextFont(thePort);
			prevSize = GetPortTextSize(thePort);
			prevStyle = GetPortTextFace(thePort);
		#else
			prevFont = thePort->txFont;
			prevSize = thePort->txSize;
			prevStyle = thePort->txFace;
		#endif
	}

	// set font characteristics
	TextFont( mFont );
	TextSize( mSize );
	TextFace( mStyle );
	GetFontInfo( &fi );
	lineDots = fi.ascent + fi.descent + fi.leading;

	// break into lines, display each one
	short done = 0;
	Str255 line;
	short linenum = 0;
	short linechar, textchar = 1;
	while (!done) {
		// find next line
		linechar = 1;
		while (textchar <= mText[0] && mText[textchar] != '\n'
				&& mText[textchar] != '\r') {
			line[linechar] = mText[textchar];
			linechar++;
			textchar++;
		}
		line[0] = linechar-1;
		textchar++;
		if (textchar > mText[0]) done = 1;
	
		short width = StringWidth( line );
		switch (mHAlign) {
			case left:
				MoveTo( mFrame.left + 1, mFrame.top );
				break;
			case middle:
				MoveTo( (mFrame.left + mFrame.right - width) / 2, mFrame.top );
				break;
			case right:
				MoveTo( mFrame.right - width - 1, mFrame.top );
				break;
		}
		switch (mVAlign) {
			case top:
				Move( 0, fi.ascent );
				break;
			case center:
				Move( 0, (mFrame.bottom - mFrame.top + fi.ascent - fi.descent) / 2);
				break;
			case bottom:
				Move( 0, mFrame.bottom - mFrame.top - fi.descent - 1 );
				break;
		}
		
		// move down for subsequent lines
		// (assumes mVAlign == top!)
		Move( 0, lineDots * linenum );

		// draw shadow
		RGBColor oldrgb;
		GetForeColor( &oldrgb );
		if (mShadow) {
			Move( 1, 1 );
			RGBForeColor( &mShadowColor );
			DrawString( line );
			Move( -1 - width, -1 );
		}
		RGBForeColor( &mColor );
		DrawString( line );
		RGBForeColor( &oldrgb );
		
		linenum++;
	} // next line, until done

	// restore previous font characteristics

	if (mRestoreFont) {
		TextFont( prevFont );
		TextSize( prevSize );
		TextFace( prevStyle );
	}
}


#include "MoofWindow.h"
#include "MoofApp.h"

MoofWindow::MoofWindow(const Rect &r, const Str255 title, 
					   const short defProc, const Boolean hasCloseBox)
: mMinWidth(100), mMaxWidth(640), mMinHeight(50), mMaxHeight(800), mKillMe(false)
{
	// create the window
	mWindowPtr = NewCWindow(nil, &r, title, true, defProc, 
						(WindowPtr) -1, hasCloseBox, 0);
	if (!mWindowPtr) {
		// LATER: handle this!
	} else {
		// set the window refcon to point to us...
		long refcon = gApplication->MoofWinToRefCon(*this);
		SetWRefCon( mWindowPtr, (long)refcon );
		// make sure the default min/max size includes the current size!
		const Rect& r = GetPortRect();
		short w = r.right - r.left;
		short h = r.bottom - r.top;
		if (mMinWidth > w) mMinWidth = w;
		if (mMinHeight > h) mMinHeight = h;
		if (mMaxWidth < w) mMaxWidth = w;
		if (mMaxHeight < h) mMaxHeight = h;
	}
}

MoofWindow::~MoofWindow()
{
	if (mWindowPtr) {
		DisposeWindow(mWindowPtr);
		mWindowPtr = 0;
	}
	if (gApplication and gApplication->mWindow == this) {
		gApplication->mWindow = NULL;
	}
}

void MoofWindow::Draw()
{
	MoveTo(50,50);
	DrawString("\pMoof!");
	DrawControls( mWindowPtr );
}

void MoofWindow::Click( const Point where, const short modifiers )
{
	Str255 str;
	NumToString( modifiers, str );
	Focus();
	MoveTo(where.h,where.v);
	DrawString( str );
}

void MoofWindow::Focus()
{
	#if TARGET_API_MAC_CARBON
		SetPortWindowPort(mWindowPtr);
	#else
		SetPort(mWindowPtr);
	#endif
	SetOrigin( 0, 0 );
	ClipRect( &GetPortRect() );
}

Rect& MoofWindow::GetPortRect()
{
	#if TARGET_API_MAC_CARBON
		GetWindowPortBounds( mWindowPtr, &mPortRect );
		return mPortRect;
	#else
		return mWindowPtr->portRect;
	#endif
}

void MoofWindow::Refresh( const Rect *subrect )
{
	Focus();
	#if TARGET_API_MAC_CARBON
		if (subrect) {
			InvalWindowRect( mWindowPtr, subrect );
		} else {
			InvalWindowRect( mWindowPtr, &GetPortRect() );
		}
	#else
		if (subrect) {
			InvalRect( subrect );
		} else {
			InvalRect( &mWindowPtr->portRect );
		}
	#endif
}

void MoofWindow::Resize( const short newWidth, const short newHeight )
{
	// resize the actual window
	SizeWindow( mWindowPtr, newWidth, newHeight, true );
	// refresh the whole thing, just to be safe
	Refresh();
}

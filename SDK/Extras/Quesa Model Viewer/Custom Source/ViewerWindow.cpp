//	ViewerWindow.cpp

//	This is the code for your custom window class.
//	This file will contain most of the "guts" of your application.

#include "InfoWindow.h"
#include "ResourceIDs.h"
#include "ViewerWindow.h"

#if TARGET_API_MAC_CARBON
	#include "QuesaView.h"
	#include "QuesaRenderer.h"
#else
	#include "QD3DView.h"
	#include "QD3DAcceleration.h"
	#include "QD3DRenderer.h"
#endif

#ifndef ASSERT
	#define ASSERT(x) if (!(x)) DebugStr("\pAssertion failed!")
#endif

#if TARGET_API_MAC_CARBON
	#define EnableItem(x,y) EnableMenuItem(x,y)
	#define DisableItem(x,y) DisableMenuItem(x,y)
#endif

static bool ViewerAvailable()
{
	// make sure our viewer is at least version 1.1
	if ((unsigned long)Q3ViewerGetVersion == kUnresolvedCFragSymbolAddress) return false;

	// (we could now use Q3ViewerGetVersion(), but if this trap exists,
	//  we know it's at least 1.1)
	return true;
}


ViewerWindow::ViewerWindow(const Rect &r, const Str255 title, const short defProc)
: inherited( r, title, defProc ), mViewer(NULL), mTracking(false)
{
	mMaxWidth = mMaxHeight = 1600;
	if (!ViewerAvailable()) {
		ParamText( "\pQuickDraw 3D", "\p1.5.4", NULL, NULL );
// PATCH!!!		Alert(kNeedUpgradeID);
		return;
	}

	// post a wait cursor
	CursHandle curs = GetCursor( watchCursor );
	if (curs) SetCursor( *curs );
	ShowCursor();
	
	// create the viewer
	CGrafPtr windowPort;
	#if TARGET_API_MAC_CARBON
		windowPort = GetWindowPort(mWindowPtr);
	#else
		windowPort = (CGrafPtr)mWindowPtr;
	#endif
	mViewer = Q3ViewerNew(windowPort, &GetPortRect(), 
		kQ3ViewerDefault | kQ3ViewerDrawGrowBox | kQ3ViewerButtonOptions);
	
	if (mViewer) {
		// set window minimum width and height
		unsigned long w,h;
		Q3ViewerGetMinimumDimension(mViewer, &w, &h);
		mMinWidth = w + 32;
		mMinHeight = h + 64;
		
		// set background color (appears to not work in QD3D 1.6)
		TQ3ColorARGB color = {0.0f, 0.6f, 0.6f, 0.7f};	// should A be 1.0 or 0.0?
		Q3ViewerSetBackgroundColor( mViewer, &color );
	}

	// if called for, set it to software-only rendering
	// (tried this to help Voodoo folk, but it didn't help)
	if (0) {
		TQ3Status err;
		// get the view object
		TQ3ViewObject view = Q3ViewerGetView(mViewer);

		// get or make the renderer
		TQ3RendererObject renderer = Q3Renderer_NewFromType(kQ3RendererTypeInteractive);
		ASSERT(renderer);
		#if !TARGET_API_MAC_CARBON
			err = Q3InteractiveRenderer_SetPreferences(renderer, kQAVendor_Apple, kQAEngine_AppleSW );
			ASSERT(err == kQ3Success);
		#endif
//		err = Q3View_GetRenderer(view, &renderer);
		
		// set its rasterizer
//		err = Q3InteractiveRenderer_SetPreferences(renderer, kQAVendor_Apple, kQAEngine_AppleSW );
		err = Q3View_SetRenderer(view, renderer);
		ASSERT(err == kQ3Success);		
	}
}

ViewerWindow::~ViewerWindow()
{
	if (mViewer) Q3ViewerDispose(mViewer);
}

void ViewerWindow::Activate( const Boolean becomingActive )
{
	// update menus
	MenuHandle mh = GetMenuHandle(kFileMenuID);
	if (becomingActive) {
		EnableItem( mh, kGetInfoItem );
	} else {
		DisableItem( mh, kGetInfoItem );
	}
	
	inherited::Activate( becomingActive );
}

void ViewerWindow::Draw()
{
 	if (mViewer) {
		Q3ViewerDraw( mViewer );
	}
	else {
		MoveTo( 20, 20 );
		DrawString("\pUnable to create QD3D Viewer.");
		MoveTo( 20, 40 );
		DrawString("\pProbably insufficient memory.");
	}
}

void ViewerWindow::HandleError(OSErr err)
{
	Str255 errstr;
	NumToString(err, errstr);
	ParamText(errstr, NULL, NULL, NULL);
	StopAlert(kUnknownErrID, NULL);
}

Boolean ViewerWindow::HandleMenuSelection( const short menuNum, const short itemNum )
{
	if (kFileMenuID == menuNum and kGetInfoItem == itemNum) {
		TQ3GroupObject group = Q3ViewerGetGroup( mViewer );
		InfoWindow *iw = new InfoWindow(group);
		iw->Select();
		return true;
	}
	
	return inherited::HandleMenuSelection(menuNum, itemNum);
}

void ViewerWindow::Idle()
{
	if (!mViewer) return;
	if (FrontWindow() != mWindowPtr) return;
	
	Point p;
	GetMouse(&p);
	if (mTracking) {
		if (StillDown()) {
			Q3ViewerContinueTracking( mViewer, p.h, p.v );
		} else {
			mTracking = false;
			Q3ViewerMouseUp( mViewer, p.h, p.v );
		}
	}
	else {
		Q3ViewerAdjustCursor( mViewer, &p );
	}
}
	
void ViewerWindow::Click( const Point pin, const short modifiers )
{
	if (!mViewer) return;
	Point p = pin;
	Q3ViewerMouseDown( mViewer, p.h, p.v );
	while (StillDown()) {
		GetMouse( &p );
		Q3ViewerContinueTracking( mViewer, p.h, p.v );
	}
	Q3ViewerMouseUp( mViewer, p.h, p.v );
//	mTracking = true;
}

void ViewerWindow::HandleKey( const char key, const char keycode, const short modifiers )
{
	if (keycode == 0x35	or key == '.') CloseSelf();
}

void ViewerWindow::LoadData( FSSpec &spec )
{
	if (!mViewer) return;
	
	// load data
	short refNum;
	OSErr err = FSpOpenDF(&spec,fsRdPerm,&refNum);
	if (!err) Q3ViewerUseFile(mViewer, refNum);
	else HandleError(err);
	FSClose(refNum);
	
	// update the view
	Q3ViewerDrawContent(mViewer);
}

void ViewerWindow::LoadData( void* data, long bytes )
{
	if (!mViewer) return;
	
	// load data
	OSErr err = Q3ViewerUseData( mViewer, data, bytes);
	if (err) HandleError(err);
	
	// update the view
	Q3ViewerDrawContent(mViewer);
}

void ViewerWindow::Resize( const short newWidth, const short newHeight )
{
	if (mViewer) {
		Rect r = {0,0, newHeight, newWidth};
		Q3ViewerSetBounds( mViewer, &r );
	}

	inherited::Resize(newWidth, newHeight);
}

OSErr ViewerWindow::ReadFromFile(const short fileRefNum)
{
	long fileSize;
	OSErr err = GetEOF(fileRefNum, &fileSize);
	Ptr data = NULL;
	if (not err) {
		data = NewPtr(fileSize);
		err = FSRead(fileRefNum, &fileSize, data);
	}
	if (not err) {
		LoadData(data, fileSize);
	}
	if (data) DisposePtr(data);
	return err;
}

//	MyApp.cpp

//	This is the code for your custom application class.

#include "MyApp.h"
#include "ViewerWindow.h"
#include "AboutBox.h"
#include "ResourceIDs.h"

short MyApp::cAppResFile = 0;

MenuWindow* MyApp::CreateWindow()
{
	//
	//	Make a new window of the "ViewerWindow" type.
	//
	#if TARGET_CARBON
		Rect windRect = {0,0,480,640};
	#else
		Rect windRect = qd.screenBits.bounds;
	#endif
	InsetRect(&windRect, 50, 50);
	windRect.right /= 2;
	windRect.bottom /= 2;
	MenuWindow *win = new ViewerWindow( windRect, "\pUntitled" );
	win->Focus();
	return win;
}

void MyApp::DoAboutBox()
{
	AboutBox *aboutBox = new AboutBox(kAboutBoxID);
}

void MyApp::Startup()
{
	// first, do standard startup stuff
	inherited::Startup();
	
	// then, store the application resource fork refnum
	cAppResFile = CurResFile();
	
	// finally, start up QD3D
	Q3Initialize();
}

#include "AboutBox.h"
#include "MyApp.h"
#include "URLPane.h"
#include "ResourceIDs.h"

// resource constants
enum {
	kUrlItem = 3,
	kVersionItem,
	
	kUrlStrID = 128,
	kHandCursorID = 500,
};

const long kSphereDelay = 6;		// time between logo turns, in ticks

static UrlPane *MakeURLPane(Rect &r)
{
	StringHandle url = GetString(kUrlStrID);
	UrlPane *out;
	if (url) out = new UrlPane( *url, *url );
	else out = new UrlPane();
	out->mCreator = kAppCreatorCode;
	out->mFont = kFontIDMonaco;
	out->mSize = 9;
	out->mStyle = underline;
	out->mHAlign = TextPane::middle;
	out->mColor.blue = 0xA000;
	out->mFrame = r;
	return out;
}	

static void DrawVersion()
{
	static Str255 versionString = "\p";
	if (not versionString[0]) {
		// Get version string from 'vers' resource
		short oldResFile = CurResFile();
		UseResFile(MyApp::cAppResFile);
		Handle h = GetResource('vers', 1);
		unsigned char *p = (unsigned char *)*h;
		BlockMoveData(p+6, &versionString[0], p[6]+1);
		ReleaseResource(h);
		UseResFile(oldResFile);
	}
	DrawString(versionString);
	#if TARGET_API_MAC_CARBON
		DrawString("\p Carbon");
	#endif
}


#pragma mark -

AboutBox::AboutBox(const short dialogID)
: MoofDialog(dialogID), mUrlPane(NULL), mHandCursor(NULL)
{
}

AboutBox::~AboutBox()
{
	if (mUrlPane) delete mUrlPane;
	if (mHandCursor) ReleaseResource((Handle)mHandCursor);
}


void AboutBox::ClickItem( const short theItem )
{
	if (theItem == kUrlItem) {
		Point p = {0,0};
		mUrlPane->Click(p, 0);
	}
	else if (theItem == ok) {
		CloseSelf();
	}
	else inherited::ClickItem(theItem);	
}

void AboutBox::DrawUserItem( const short item )
{
	short	itemType;
	Handle	itemHand;
	Rect	itemBox;
	GetDialogItem( mDialogPtr, item, &itemType, &itemHand, &itemBox );

	if (item == kUrlItem) {
	
		if (!mUrlPane) {
			mUrlPane = MakeURLPane(itemBox);
		}
		mUrlPane->Draw();
	}
	
	else if (kVersionItem == item) {
		MoveTo(itemBox.left, itemBox.bottom - 3);
		TextFont(kFontIDGeneva);
		TextSize(9);
		DrawVersion();	
	}

	else {
		inherited::DrawUserItem(item);
		return;
	}

}

void AboutBox::Idle()
{
	if (mKillMe) return;
	
	// if cursor is over the URL pane, go to click cursor
	Point p;
	GetMouse( &p );
	if (PtInRect( p, &mUrlPane->mFrame )) {
		if (mHandCursor) SetCursor(*mHandCursor);
		else {
			mHandCursor = GetCursor(kHandCursorID);
		}
	}
	else {
	  #if TARGET_API_MAC_CARBON
		Cursor arrow;
		::SetCursor(GetQDGlobalsArrow(&arrow));
	  #else
		::SetCursor(&qd.arrow);
	  #endif
	}
	
}


//	MoofDialog.cpp
//
//	a class derived from MoofWindow.h, to handle dialog processing
//	

#include "MoofDialog.h"
#include "MoofApp.h"
#include "Translator.h"
#include <Balloons.h>


#if !__POWERPC__
// Not available to 68k apps:
static TEHandle GetDialogTextEditHandle(DialogRef dlog)
{
	return ((DialogPeek)dlog)->textH;
}
#else
// Apparently missing from my copy of Carbon Dialogs.h:
EXTERN_API( TEHandle ) GetDialogTextEditHandle( DialogRef dialog);
#endif

// static method and global UPP to handle user items in dialogs in a general
// object-oriented fashion.

static pascal void	UserItemVectorProc(DialogPtr theDialog, short item);
UserItemUPP	gUIVectorUPP = NULL;

// similarly, for filtering events
static pascal Boolean DlogEventFilter(DialogPtr theDialog, EventRecord *theEvent, short *itemHit);
ModalFilterUPP gFilterUPP = NewModalFilterUPP(DlogEventFilter);


/*-------------------------------***  CONSTRUCTOR  ***----------------------------------*/


MoofDialog::MoofDialog(const short dialogID) 
: mNotMyBalloons(true), hasDefaultItem(false), isActive(false), 
  mQtyStatesCached(0), mStatesCached(NULL), mCicnUserItems(NULL)
{
	if (not gUIVectorUPP) gUIVectorUPP = NewUserItemUPP(UserItemVectorProc);

	mMinWidth = 100;
	mMaxWidth = 640;
	mMinHeight = 50;
	mMaxHeight = 800;
	mKillMe = false;
	
	isModal = FALSE;	// set up by MakeMacWindow
	rGroupList = ( RGroupHdl ) NewHandle( 0 );	// resized as needed

	Initialize(dialogID);

	// we have to duplicate some of the MoofWindow code here;
	// that's unfortunate, but necessary, because calling the
	// MoofWindow constructor would make the wrong sort of window...

	// set the window refcon to point to us...
	long refcon = gApplication->MoofWinToRefCon(*this);
	SetWRefCon( mWindowPtr, (long)refcon );
	// make sure the default min/max size includes the current size!
	Rect r = GetPortRect();
	short w = r.right - r.left;
	short h = r.bottom - r.top;
	if (mMinWidth > w) mMinWidth = w;
	if (mMinHeight > h) mMinHeight = h;
	if (mMaxWidth < w) mMaxWidth = w;
	if (mMaxHeight < h) mMaxHeight = h;

}


/*---------------------------------***  DESTRUCTOR  ***----------------------------------*/


MoofDialog::~MoofDialog()
{
	#if !TARGET_API_MAC_CARBON
		// turn off balloons if they were turned on for a modal dialog
		if (HMGetBalloons() and !mNotMyBalloons and IsModal()) {
			HMSetBalloons( false );
		}
	#endif

	if (mCicnUserItems) {
		short qty = CountDITL( mDialogPtr )+1;
		for (short i=0; i<qty; i++) {
			if (mCicnUserItems[i]) DisposeCIcon(mCicnUserItems[i]);
		}
		delete[] mCicnUserItems;
	}
	
	if (mQtyStatesCached) delete[] mStatesCached;
	
	if ( mWindowPtr )
		DisposeDialog( mDialogPtr );

	mWindowPtr = NULL;
	mDialogPtr = NULL;
	
	if ( rGroupList )
		DisposeHandle(( Handle ) rGroupList );
	

}

/*-------------------------------***  INITIALIZE  ***----------------------------------*/
/*	

construct the dialog and set it up

----------------------------------------------------------------------------------------*/

void	MoofDialog::Initialize(const short dialogID)
{
	MakeMacWindow( dialogID );	// make the dialog box, and set modal flag
	SetUpUserItems();			// install user item procs so we get callbacks
	SetUpRadioGroups();			// automatically deal with radio button groups

	#if !TARGET_API_MAC_CARBON
		// note whether help balloons are already on
		mNotMyBalloons = HMGetBalloons();
	#endif
	
	#if NO_TRANSLATION
		// no translation wanted or needed
	#else
		// Translate the dialog text.
		short qty = CountDITL(mDialogPtr);
		for (short i=1; i<=qty; i++) {
			short	iType;
			Handle	iHand;
			Rect	iBox;	
			GetDialogItem( mDialogPtr, i, &iType, &iHand, &iBox );
			ZString transText;
			switch (iType & 127) {
				case kStaticTextDialogItem:
				case kEditTextDialogItem:
					GetDialogItemText( iHand, transText );
					if (Translate(transText)) SetDialogItemText( iHand, transText );
					break;
				case kButtonDialogItem:
				case kCheckBoxDialogItem:
				case kRadioButtonDialogItem:
				case kResourceControlDialogItem:
					ControlHandle ch = (ControlHandle)iHand;
					GetControlTitle( ch, transText );
					if (Translate(transText)) SetControlTitle( ch, transText );
					// While we're at it, set the default button.
					if (i == 1) {
						SetDialogDefaultItem(mDialogPtr, i);
					}
					break;
			}
		}
		ZString winTitle;
		GetWTitle(mWindowPtr, winTitle);
		if (Translate(winTitle)) SetWTitle(mWindowPtr, winTitle);
	#endif
	
	SetUp();					// call user's set up method
}


/*-----------------------------***  SETCICNUSERITEM  ***--------------------------------*/
/*	

Set a certain user item to draw a cicn.  Handy since the DITL definition gives
us no way to do this naturally.

----------------------------------------------------------------------------------------*/

void	MoofDialog::SetCicnUserItem(short item, short cicnID)
{
	short qty = CountDITL(mDialogPtr)+1;
	if (!mCicnUserItems) {
		mCicnUserItems = new CIconHandle[qty];
		if (!mCicnUserItems) return;
		for (short i=0; i<qty; i++) mCicnUserItems[i] = NULL;
	}
	
	if (item >= 0 and item < qty) {
		mCicnUserItems[item] = GetCIcon(cicnID);
	}
}


/*------------------------------***  MAKEMACWINDOW  ***---------------------------------*/
/*	

make a dialog. This tries to construct a dialog using a 'DLOG' resource with the ID
passed. The refcon is set to this object, so do not use it. Sets the modal flag too.

----------------------------------------------------------------------------------------*/

void	MoofDialog::MakeMacWindow( const short dialogID )
{
	mDialogPtr = GetNewDialog( dialogID, 0, (WindowPtr)-1 );
	#if TARGET_API_MAC_CARBON
		mWindowPtr = GetDialogWindow(mDialogPtr);
	#else
		mWindowPtr = (WindowRef)mDialogPtr;
	#endif
	if (!mWindowPtr) return;

	// set our refcon to refer to the object, in the standard Moof way.	
	long refcon = gApplication->MoofWinToRefCon(*this);
	SetWRefCon( mWindowPtr, (long)refcon );
	
	// see whether this is modal or not by examining the style of the window. This
	// flag affects the way commands are processed.
	
	short wVar = GetWVariant( mWindowPtr );
	
	isModal = ( wVar == dBoxProc 	||
				wVar == plainDBox 	||
				wVar == altDBoxProc ||
				wVar == movableDBoxProc );	
}


/*-----------------------------***  SETUPUSERITEMS  ***---------------------------------*/
/*	

make every user item point to our vector proc for handling these items. Then you can
simply override DrawUserItem to draw anything you want in the dialog!

----------------------------------------------------------------------------------------*/

void	MoofDialog::SetUpUserItems()
{
	short	item, itemType;
	Handle	itemHand;
	Rect	itemBox;
	
	item = CountDITL(mDialogPtr);
	
	while(item)
	{
		GetDialogItem( mDialogPtr, item--, &itemType, &itemHand, &itemBox );
		
		if ((itemType & 0x7F) == userItem ) {
			SetDialogItem( mDialogPtr, item + 1, itemType, (Handle) gUIVectorUPP, &itemBox ); 
		}
		else {
		}
	}
}



/*-----------------------------------***  DRAW  ***-------------------------------------*/
/*	
draws the dialog items
----------------------------------------------------------------------------------------*/

void	MoofDialog::Draw()
{
	#if TARGET_API_MAC_CARBON
		RgnHandle visRgn = NewRgn();
		GetPortVisibleRegion(GetWindowPort(mWindowPtr), visRgn);
		UpdateDialog( mDialogPtr, visRgn );
		DisposeRgn(visRgn);
	#else
		UpdateDialog( mWindowPtr, mWindowPtr->visRgn );
	#endif
	
/*	if ( isModal or (hasDefaultItem and isActive) ) {
		OutlineDefaultItem();
	}
*/
}


/*-----------------------------------***  CLICK  ***------------------------------------*/
/*	
handle clicks in the dialog, This determines the item and calls ClickItem.
----------------------------------------------------------------------------------------*/

void	MoofDialog::Click( const Point mouse, const short modifiers )
{
	// which item was hit?	
	short	item = FindDialogItem( mDialogPtr, mouse ) + 1;
	
	// if the item is enabled, call TrackControl, TEClick, etc as needed.
	
	if ( item > 0 )
	{
		short	iType, partCode;
		Handle	iHand;
		Rect	iBox;
		
		GetDialogItem( mDialogPtr, item, &iType, &iHand, &iBox );
		
		// if the item is a control, track it. Call ClickItem if control clicked and
		// item was enabled.
		
		if ( iType & ctrlItem )
		{
			// use FindControl for automatic bypassing of disabled items
			if (FindControl(mouse, mWindowPtr, &(ControlHandle)iHand) == 0
				or iHand == 0) return;	// disabled!
			// otherwise, use TrackControl to track mouse till mouse-up
			partCode = TrackControl((ControlHandle) iHand, mouse, ( ControlActionUPP ) -1L );
		
			if (( partCode != 0 ) && (( iType & 0x80 ) == 0 ))
				ClickItem( item );
		}
		else
		{
			if ( iType & editText )
			{
				// if an edit field, pass click to text edit. If not in the current
				// field, switch fields to the one clicked.
				
				//#if TARGET_API_MAC_CARBON
					SInt16 focusFld = GetDialogKeyboardFocusItem(mDialogPtr);
				//#else
				//	SInt16 focusFld = mDialogPtr->editField;
				//#endif
				
				if ( item != focusFld + 1) {
					SelectDialogItemText( mDialogPtr, item, 0, 0 );
				}
							
				TEClick( mouse, (modifiers & shiftKey) == shiftKey , GetDialogTextEditHandle(mDialogPtr) );
			}
			
			// all enabled items are passed to ClickItem
			
			if (( iType & 0x80 ) == 0 )
				ClickItem( item );
		}
	}
}


Boolean MoofDialog::DoModalDialog()
{
	Draw();
	OutlineDefaultItem();
	SetDialogDefaultItem( mDialogPtr, ok );
	SetDialogCancelItem( mDialogPtr, cancel );
	short itemHit;
	do {
		ModalDialog( gFilterUPP, &itemHit );
		ClickItem( itemHit );
	} while (itemHit != cancel and itemHit != ok);
	return (itemHit == ok);
}

/*---------------------------------***  ACTIVATE  ***-----------------------------------*/
/*	
[de]activates TextEdit if present
----------------------------------------------------------------------------------------*/

void	MoofDialog::Activate(const Boolean becomingActive)
{
	// enable/disable the TE fields and other controls
	if (becomingActive) {
		if ( HasEditFields()) TEActivate( GetDialogTextEditHandle(mDialogPtr) );
		isActive = true;
		// if we've cached item states, restore them now
		for (short i=0; i<mQtyStatesCached; i++) {
			if (mStatesCached[i]) Enable(i+1);
		}
	} else {
		if ( HasEditFields()) TEDeactivate( GetDialogTextEditHandle(mDialogPtr) );
		// make a cache of which items were enabled, and disable them
		short qty = CountDITL(mDialogPtr);
		if (qty > mQtyStatesCached) {
			delete[] mStatesCached;
			mQtyStatesCached = qty;
			mStatesCached = new Boolean[qty];
		}
		for (short i=0; i<mQtyStatesCached; i++) {
			mStatesCached[i] = IsEnabled(i+1);
			if (mStatesCached[i]) Disable(i+1);
		}
		isActive = false;
	}
	
	inherited::Activate(becomingActive);
	
	// redraw items -- so that active/inactive state is reflected
	Focus();
	Refresh();
}


/*-----------------------------------***  IDLE  ***-------------------------------------*/
/*	
blink TextEdit cursor if present
...also adjust the cursor
...and do balloon help
----------------------------------------------------------------------------------------*/

void	MoofDialog::Idle()
{
	if ( HasEditFields())
		TEIdle( GetDialogTextEditHandle(mDialogPtr) );
	
	Point p;
	GetMouse(&p);
	AdjustCursor( p, 0 );

	#if CARBON
		Boolean foo = HMGetBalloons();
		Boolean bar = HMIsBalloon();
		if (foo and not bar) ShowBalloonHelp();
		//	if (HMGetBalloons() and not HMIsBalloon()) ShowBalloonHelp();
	#endif
}


/*-----------------------------------***  TYPE  ***-------------------------------------*/
/*	
pass TextEdit the typed character if present. If tab key, cycle through fields
----------------------------------------------------------------------------------------*/

void	MoofDialog::Type( const char theKey )
{
	short		iType;
	Handle		iHand;
	Rect		iBox;
	
	Focus();
	
	if ( HasEditFields())
	{
		if ( theKey == 0x09 )		// tab
		{
			short	nextField, fMax;
			
			// find the next edit field item that can be set
			
			nextField = GetDialogKeyboardFocusItem(mDialogPtr) + 1;
			fMax = CountDITL( mDialogPtr );
			
			do
			{
				if ( nextField > fMax )
					nextField = 1;
				
				// if we got back to where we started, so nothing. This will occur
				// if the dialog has only 1 edit field
				
				if ( nextField == GetDialogKeyboardFocusItem(mDialogPtr) )
					return;
				
				GetDialogItem( mDialogPtr, nextField, &iType, &iHand, &iBox );
				
				// if this is an edit field, then select that one
				
				if ( iType & editText )
				{
					SelectDialogItemText( mDialogPtr, nextField, 0, 32767 );
					break;	
				}
				
				nextField++;
			}
			while( 1 );
		}
		else
		{
			if ( GetDialogTextEditHandle(mDialogPtr) )
				TEKey( theKey, GetDialogTextEditHandle(mDialogPtr) );
		}
		
		ClipRect( &GetPortRect() );
		
		// if item is enabled, call ClickItem too
		
		GetDialogItem( mDialogPtr, GetDialogKeyboardFocusItem(mDialogPtr), &iType, &iHand, &iBox );
		
		if (( iType & 0x80 ) == 0 )
			ClickItem( GetDialogKeyboardFocusItem(mDialogPtr) );

	}

}



/*------------------------------***  DRAWUSERITEM  ***----------------------------------*/
/*	

this is called to update each user item in the dialog. You can override this to draw
custom dialog items. The default method draws a 50% grey outline around the item.

----------------------------------------------------------------------------------------*/

void	MoofDialog::DrawUserItem( const short item )
{
	short	itemType;
	Handle	itemHand;
	Rect	itemBox;
	
	GetDialogItem( mDialogPtr, item, &itemType, &itemHand, &itemBox );
	
	if (mCicnUserItems and mCicnUserItems[item]) {
		// if we have a cicn user item set, draw that...
		PlotCIcon( &itemBox, mCicnUserItems[item] );
	}
	else {
		// otherwise, just draw a gray outline
		#if TARGET_API_MAC_CARBON
			Pattern gray;
			GetQDGlobalsGray( &gray );
			PenPat( &gray );
		#else
			PenPat( &qd.gray );
		#endif
		PenSize( 1,1 );
		
		FrameRect( &itemBox );
		PenNormal();
	}
}


Boolean MoofDialog::ModalFilter(EventRecord *theEvent, short *itemHit)
{
	// this is called upon receiving events from a modal dialog...
	// by default, we check for return and command-period,
	// then just do normal filtering

	if (theEvent->what == keyDown) {
		short		itemType;
		Handle		itemHand;
		Rect		itemBox;
		char theKey = theEvent->message & charCodeMask;
			
		if (theKey == 0x0D ||		// return
			theKey == 0x03)			// enter
		{
			// make sure that item 1 is an enabled button
			
			GetDialogItem( mDialogPtr, ok, &itemType, &itemHand, &itemBox );
			
//			if ( itemType == ( ctrlItem + btnCtrl ))
			if ( kButtonDialogItem == itemType )
			{
				FakeClick( ok );
				*itemHit = ok;
				return true;
			}
		}
		
		#if !TARGET_API_MAC_CARBON
			if ((theEvent->message & keyCodeMask) == 0x7200)	// help key
			{
				HMSetBalloons( !HMGetBalloons() );
				return true;			
			}
		#endif
		
		Boolean cmdPeriod = ((theEvent->what == keyDown) &&
					 ((theEvent->modifiers & cmdKey) == cmdKey ) &&
					 (theKey == '.'));
		if (( theKey == 0x1B ) || cmdPeriod )	// escape or command-period
		{
			// make sure item 2 is an enabled button
			
			GetDialogItem( mDialogPtr, cancel, &itemType, &itemHand, &itemBox );
			
			if ( itemType == ( ctrlItem + btnCtrl ))
			{
				FakeClick( cancel );
				*itemHit = cancel;
				return TRUE;
			}
		}
	}
	
	return Filter(theEvent);
}

/*----------------------------------***  DOCUT  ***-------------------------------------*/
/*	
handle edit command cut.
----------------------------------------------------------------------------------------*/

void	MoofDialog::DoCut()
{
	DialogCut( mDialogPtr );
	#if TARGET_API_MAC_CARBON
		ClearCurrentScrap();
	#else
		ZeroScrap();
	#endif
	TEToScrap();
}


/*----------------------------------***  DOCOPY  ***------------------------------------*/
/*	
handle edit command copy.
----------------------------------------------------------------------------------------*/

void	MoofDialog::DoCopy()
{
	DialogCopy( mDialogPtr );
	#if TARGET_API_MAC_CARBON
		ClearCurrentScrap();
	#else
		ZeroScrap();
	#endif
	TEToScrap();
}


/*---------------------------------***  DOPASTE  ***------------------------------------*/
/*	
handle edit command paste.
----------------------------------------------------------------------------------------*/

void	MoofDialog::DoPaste()
{
	TEFromScrap();
	DialogPaste( mDialogPtr );
}


/*---------------------------------***  DOCLEAR  ***------------------------------------*/
/*	
handle edit command clear.
----------------------------------------------------------------------------------------*/

void	MoofDialog::DoClear()
{
	DialogDelete( mDialogPtr );
}

/*-------------------------------***  UPDATEMENUS  ***----------------------------------*/
/*	
enable the edit menu items if we have edit fields
----------------------------------------------------------------------------------------*/
/*
void	MoofDialog::UpdateMenus()
{
	// enable cut, copy, clear IF we have edit fields. If modal,
	// do NOT pass this on up the chain.
	
	MenuHandle	mH;
	
	if ( HasEditFields())
	{
		mH = GetMenuHandle( kEditMenu );
		
		if ( mH )
		{
			EnableItem( mH, kCutCmd );
			EnableItem( mH, kCopyCmd );
			EnableItem( mH, kClearCmd );
			
			if ( CanPasteType())
				EnableItem( mH, kPasteCmd );
		}
	}
	
	if (! isModal)
	{
		inherited::UpdateMenus();
		
		// disable the save/save as menu item, since it is inappropriate for
		// dialogs (ancestor window may have enabled it)
		
		mH = GetMenuHandle( kFileMenu );
		
		if ( mH )
		{	
			DisableItem( mH, kItemSave );
			DisableItem( mH, kItemSaveAs );
		}	
	}
}

*/
/*-------------------------------***  CANPASTETYPE  ***---------------------------------*/
/*	
we can paste if we have edit fields and there is text on the clipboard
----------------------------------------------------------------------------------------*/

Boolean	MoofDialog::CanPasteType()
{
	return true;//	return ( HasEditFields() && gClipboard->QueryType( 'TEXT' ));
}


/*----------------------------------***  SETUP  ***-------------------------------------*/
/*	

called when the dialog is built so you can initialise your dialog items. For user items,
override DrawUserItem rather than installing your own procs here. By default, this hilites
the text of the first edit field it finds.
----------------------------------------------------------------------------------------*/

void	MoofDialog::SetUp()
{
	// set the first edit field to be hilited
	
	short	i, iType, items = CountDITL( mDialogPtr );
	Handle	iHand;
	Rect	iBox;
	
	for ( i = 1; i <= items; i++ )
	{
		GetDialogItem( mDialogPtr, i, &iType, &iHand, &iBox );
	
		if (( iType & 0x7F ) == editText )
		{
			SelectDialogItemText( mDialogPtr, i, 0, 32767 );
			break;
		}
	}
	// override this method to initialise your controls, etc to the desired
	// state for your dialog, but call the inherited method as well.
}


/*-----------------------------***  ADJUSTCURSOR  ***-----------------------------------*/
/*	

overrides MoofWindow to set the cursor to an i-beam over edit fields.
----------------------------------------------------------------------------------------*/

void	MoofDialog::AdjustCursor( const Point mouse, const short modifiers )
{
	short		item, itemType;
	Handle		itemHand;
	Rect		itemBox;
	CursHandle	iBeam;
	
	item = FindItem( mouse );
	
	if ( item > 0 )
	{
		GetDialogItem( mDialogPtr, item, &itemType, &itemHand, &itemBox );
		
		if (( itemType & 0x7F ) == editText )
		{
			iBeam = GetCursor( iBeamCursor );
			SetCursor( *iBeam );
			
			return;
		}
	}
	
	#if TARGET_API_MAC_CARBON
		Cursor arrow;
		::SetCursor(GetQDGlobalsArrow(&arrow));
	#else
		::SetCursor(&qd.arrow);
	#endif
}


/*--------------------------------***  CLICKITEM  ***-----------------------------------*/
/*	

an enabled item was clicked (or an enabled field was typed in) This handles check boxes
by toggling their state, and OK and Cancel buttons in modal dialogs. It also now handles
groups of radio buttons. Override to handle clicks in other dialog items, but call the
inherited method to deal with these standard items.
----------------------------------------------------------------------------------------*/

void	MoofDialog::ClickItem( const short theItem )
{
	short	itemType;
	Handle	itemHand;
	Rect	itemBox;
	
	// get info about the item clicked
	
	GetDialogItem( mDialogPtr, theItem, &itemType, &itemHand, &itemBox );
	
	// if a checkbox, toggle its state
	
	if (( itemType & 0x7F ) == ( ctrlItem + chkCtrl ))
		SetControlValue(( ControlHandle ) itemHand, GetControlValue(( ControlHandle ) itemHand ) ^ 1 );
		
	// if the item is a radio button, handle it if it is part of a group
	
	if (( itemType & 0x7F ) == ( ctrlItem + radCtrl ))
		HandleRButtonGroupClick( theItem );

	// if it is the OK or cancel button, dismiss the dialog if a modal dialog.
	
	if ((( theItem == ok ) || ( theItem == cancel ))	&&
		(( itemType & 0x7F ) == ( ctrlItem + btnCtrl )) &&
		isModal )
	{
		if ( theItem == cancel or CanClose() ) {
			CloseSelf();
		}
	}
}


/*-------------------------------***  CLOSEDIALOG  ***----------------------------------*/
/*	

the dialog is about to close. You can abort the close by returning FALSE. Override to
read the values of controls, other items, etc.
----------------------------------------------------------------------------------------*/

Boolean	MoofDialog::CanClose(Boolean quitting)
{
	// the dialog is about to close. Return FALSE if you can't close (Verify fields?)
	
	return TRUE;
}



/*---------------------------------***  FILTER  ***-------------------------------------*/
/*	

now's your chance! This allows you to examine and modify the event that is going to come to
this dialog before the dialog manager gets it. By default, this maps return and enter keys
to the dialog buttons. If the event is fully handled, return TRUE, else FALSE. If TRUE is
returned, no further processing will occur.
----------------------------------------------------------------------------------------*/

Boolean	MoofDialog::Filter( EventRecord* theEvent )
{
	char		theKey;
	Boolean		fullyHandled = FALSE;
	short		itemType;
	Handle		itemHand;
	Rect		itemBox;
	Boolean		cmdPeriod;
	
	// see if command-period is down
	
	cmdPeriod = ((theEvent->what == keyDown) &&
				 isModal &&
				 ((theEvent->modifiers & cmdKey) == cmdKey ) &&
				 ((theEvent->message & charCodeMask) == '.'));
	
	// map return, enter and escape keys to ok and cancel
	
	if ((theEvent->what == keyDown) && isModal)
	{
		theKey = theEvent->message & charCodeMask;
		
		if (theKey == 0x0D ||		// return
			theKey == 0x03)			// enter
		{
			// make sure that item 1 is an enabled button
			
			GetDialogItem( mDialogPtr, ok, &itemType, &itemHand, &itemBox );
			
			if ( itemType == ( ctrlItem + btnCtrl ))
			{
				FakeClick( ok );
				fullyHandled = TRUE;
			}
		}
		
		if (( theKey == 0x1B ) || cmdPeriod )	// escape or command-period
		{
			// make sure item 2 is an enabled button
			
			GetDialogItem( mDialogPtr, cancel, &itemType, &itemHand, &itemBox );
			
			if ( itemType == ( ctrlItem + btnCtrl ))
			{
				FakeClick( cancel );
				fullyHandled = TRUE;
			}
		}
	}

	return fullyHandled;
}


/*---------------------------------***  FAKECLICK  ***----------------------------------*/
/*	
fake a click on an item (presumably a button).
----------------------------------------------------------------------------------------*/

void	MoofDialog::FakeClick( const short item )
{
	unsigned long ignored;
	
	HiliteItem( item, 1 );
	Delay( 8, &ignored );
	HiliteItem( item, 0 );
	
	ClickItem( item );
}


void	MoofDialog::Enable(const short item, const Boolean enableIt)
{
	// enable/disable the item
	// NOTE: if you want it drawn in gray, you'll have to do that yourself!

	// if window is not active, then just update the cache
	if (!isActive) {
		if (item-1 < mQtyStatesCached) mStatesCached[item-1] = enableIt;
		return;
	}

	short iType;
	Handle iHandle;
	Rect iRect;
	
	GetDialogItem( mDialogPtr, item, &iType, &iHandle, &iRect );
	// make sure it's a control!
	if (iType & 4) {
		HiliteControl( (ControlHandle)iHandle, enableIt ? 0 : 255 );
	}
}

Boolean	MoofDialog::IsEnabled(const short item) const
{
	short iType;
	Handle iHandle;
	Rect iRect;
	
	GetDialogItem( mDialogPtr, item, &iType, &iHandle, &iRect );
	return 0 == (iType & kItemDisableBit);
}

/*--------------------------------***  SETVALUE  ***------------------------------------*/
/*	

sets the value of the dialog item to <value>. This determines the type of the item and
does the obvious thing.  Various data types are accepted by overloading the parameters,
though for controls, the value must be between -32767 and +32768 or an exception is thrown.
----------------------------------------------------------------------------------------*/

void	MoofDialog::SetValue(const short item, const long value)
{
	short	itemType;
	Handle	itemHand;
	Rect	itemBox;
	
	GetDialogItem( mDialogPtr, item, &itemType, &itemHand, &itemBox );
	
	// for controls: check that parameter is within range, then set the control value
	
	if (( itemType & ctrlItem ) == ctrlItem ) 
	{
		if ( value < -32767 || value > 32768 )
			;//FailOSErr( paramErr );
		else
			SetControlValue(( ControlHandle ) itemHand, value );
	}
	else
	{	
		// for text items: convert long to string, and set item text
		
		if ( itemType & ( editText | statText ))
		{
			Str255	iText;
			
			NumToString( value, iText );
			SetDialogItemText( itemHand, iText );
		}
	}
}


/*--------------------------------***  SETVALUE  ***------------------------------------*/

void	MoofDialog::SetValue( const short item, const Str255 value )
{
	short	itemType;
	Handle	itemHand;
	Rect	itemBox;
	
	GetDialogItem( mDialogPtr, item, &itemType, &itemHand, &itemBox );
	
	if ( itemType & ( editText | statText ))
		SetDialogItemText( itemHand, value );
}


/*--------------------------------***  SETVALUE  ***------------------------------------*/
/*
void	MoofDialog::SetValue( const short item, const double value )
{
	short	itemType;
	Handle	itemHand;
	Rect	itemBox;
	
	GetDialogItem( mDialogPtr, item, &itemType, &itemHand, &itemBox );
	
	if (( itemType & ctrlItem ) == ctrlItem )
	{
		SetValue( item, (long) value );
	}
	else
	{
		if ( itemType & ( editText | statText ))
		{
			Str255	iText;

			RealToString( value, iText );
			SetDialogItemText( itemHand, iText );
		}
	}
}
*/
/*--------------------------------***  GETVALUE  ***------------------------------------*/
/*	

gets the value of the dialog item. This determines the type of the item and
does the obvious thing. For unknown items, returns 0.
----------------------------------------------------------------------------------------*/

long	MoofDialog::GetValue( const short item ) const
{
	short	itemType;
	Handle	itemHand;
	Rect	itemBox;
	
	GetDialogItem( mDialogPtr, item, &itemType, &itemHand, &itemBox );
	
	if (( itemType & ctrlItem ) == ctrlItem )
		return GetControlValue(( ControlHandle ) itemHand );
		
	if (( itemType & editText ) == editText )
	{
		long	value;
		Str255	iText;
		
		GetDialogItemText( itemHand, iText );
		StringToNum( iText, &value );
		
		return value;
	}
	
	return 0;
}


/*-----------------------------***  GETVALUEASTEXT  ***---------------------------------*/
/*	
get text of item, or if control, convert value to string first (complementary function to
GetValue().
----------------------------------------------------------------------------------------*/

void	MoofDialog::GetValueAsText( const short item, Str255 aStr ) const
{
	short	iType;
	Handle	iHand;
	Rect	iBox;
	
	GetDialogItem( mDialogPtr, item, &iType, &iHand, &iBox );
	
	if ( iType & ( statText | editText ))
		GetDialogItemText( iHand, aStr );
	else
	{
		if ( iType & ctrlItem )
		{
			long	v = GetValue( item );
		
			NumToString( v, aStr );
		}
	}
}


/*-----------------------------***  GETITEMBOUNDS  ***----------------------------------*/
/*	
get rect of item
----------------------------------------------------------------------------------------*/

void	MoofDialog::GetMenuItemTextBounds( const short item, Rect* bounds )
{
	short	iType;
	Handle	iHand;
	
	GetDialogItem( mDialogPtr, item, &iType, &iHand, bounds );
}


/*--------------------------------***  FINDITEM  ***------------------------------------*/
/*	
find the item under the mouse point (local coordinates), 0 if no item there
----------------------------------------------------------------------------------------*/

short	MoofDialog::FindItem( const Point localMouse )
{
	return ( FindDialogItem( mDialogPtr, localMouse ) + 1);
}


/*------------------------------***  HILITEITEM  ***------------------------------------*/
/*	
allows controls to be hilited or dimmed
----------------------------------------------------------------------------------------*/

void	MoofDialog::HiliteItem(const short item, const short state)
{
	short	itemType;
	Handle	itemHand;
	Rect	itemBox;
	
	GetDialogItem( mDialogPtr, item, &itemType, &itemHand, &itemBox );
	
	if (( itemType & ctrlItem ) == ctrlItem )
		HiliteControl(( ControlHandle ) itemHand, state );
}


/*--------------------------***  OUTLINEDEFAULTITEM  ***--------------------------------*/
/*	
draws the bold border around the item number 1 in the dialog
----------------------------------------------------------------------------------------*/

void	MoofDialog::OutlineDefaultItem()
{
	short	itemType;
	Handle	itemHand;
	Rect	itemBox;
	
	GetDialogItem( mDialogPtr, ok, &itemType, &itemHand, &itemBox );
	
	// check that this item is a button
	
	if ( itemType & ( ctrlItem + btnCtrl ))
	{
		Focus();
		PenSize( 3, 3 );
		InsetRect( &itemBox, -4, -4 );
		PenMode( patOr );
		ForeColor( blackColor );
		
		// if button disabled, draw the outline in grey
		#if TARGET_API_MAC_CARBON
			int hilite = GetControlHilite( (ControlHandle)itemHand );
		#else
			int hilite = (*(ControlHandle)itemHand)->contrlHilite;
		#endif
		if (hilite != 0 )
		{
			RGBColor	aGray = { 0x7F7F, 0x7F7F, 0x7F7F };				
			RGBForeColor( &aGray );	
		}	
		FrameRoundRect( &itemBox, 16, 16 );
		PenNormal();
		ForeColor( blackColor );
	}	
}


/*-----------------------------***  HASEDITFIELDS  ***----------------------------------*/
/*	

returns TRUE if this dialog has any edit fields. Used to determine if Edit commands are
enabled or not.

----------------------------------------------------------------------------------------*/

Boolean	MoofDialog::HasEditFields()
{
	// returns TRUE if this dialog has at least one edit field. Use to enable
	// the edit menu.

	short	itemType, item;
	Handle	itemHand;
	Rect	itemBox;
	Boolean	hasEF = FALSE;

	item = CountDITL( mDialogPtr );
	
	while(item)
	{
		GetDialogItem( mDialogPtr, item--, &itemType, &itemHand, &itemBox );
	
		if (( itemType & 0x7F ) == editText )
		{
			hasEF = TRUE;
			break;
		}
	}

	return hasEF;
}


/*-----------------------------***  SETUPRADIOGROUPS  ***-------------------------------*/
/*	
This builds the array of radio button groups so that the click handler can correctly
manage them. This is done very easily- in your DITL resource, add the group ID to the
button title, separated by a double slash- e.g. My Button//1. This function strips the
extra characters from the name, so you never see them. Groups start at ID = 1, and
obviously buttons with the same group ID operate as a set. If you do not extend the title
of a group button in this way, it will operate in the normal way- i.e. not very well.

----------------------------------------------------------------------------------------*/

void	MoofDialog::SetUpRadioGroups()
{
	short	iType, tIndex, groupID;
	Boolean	iDefault;
	Handle	iHand;
	Rect	iBox;
	long	tSize;
	Str255	cTitle;
	
	short	iCount = CountDITL( mDialogPtr );

	do
	{
		GetDialogItem( mDialogPtr, iCount, &iType, &iHand, &iBox );
		
		// is this item a radio button?
		
		if ((iType & ( radCtrl + ctrlItem )) == ( radCtrl + ctrlItem ))
		{
			// yes, so make an entry for it in the group table. Note that all radio
			// buttons get an entry- those that do not take advantage of this scheme
			// have a groupID of zero, which instructs HandleRButtonGroupClick() to
			// ignore the button.
			
			tSize = GetHandleSize((Handle) rGroupList );
			tIndex = tSize / sizeof( RGroupEntry );
			
			SetHandleSize((Handle) rGroupList, tSize + sizeof( RGroupEntry ));
		
			// get the title of the button
			
			GetControlTitle((ControlHandle) iHand, cTitle );
			
			// parse the title looking for a double slash followed by a number. This
			// is returned in groupID, and the title is stripped of the extra chars.
			
			ParseRButtonTitle( cTitle, &groupID, &iDefault );
			
			// set up the entry in the table
			
			(*rGroupList)[tIndex].item = iCount;
			(*rGroupList)[tIndex].groupID = groupID;
			
			// set the button's name to eliminate the grouping info
			
			SetControlTitle((ControlHandle) iHand, cTitle );
			
			// if this is the default button, set it ON
			
			if ( iDefault )
				SetControlValue((ControlHandle) iHand, 1 );
		}
	}
	while( --iCount );
}


/*---------------------------***  PARSERBUTTONTITLE  ***--------------------------------*/
/*	
Finds any group ID "buried" in the button name. This modifies the input string so that
the extra characters are removed.
----------------------------------------------------------------------------------------*/

void	MoofDialog::ParseRButtonTitle( Str255 buttonTitle, short* groupID, Boolean* isDefault )
{
	register char	cc = 1;
	long			gID = 0;
	Str15			subStr;
	
	*groupID = 0;		// in case we find nothing at all
	*isDefault = FALSE;
	
	// scan the string looking for two consecutive forward slashes:
	
	do
	{
		if ((buttonTitle[cc] == '/') && (buttonTitle[cc + 1] == '/'))
		{
			// found two consecutive slashes- now copy the rest of the string into
			// subStr.	
			
			subStr[0] = buttonTitle[0] - cc - 1;	// length of remainder of string
			if (subStr[0])
			{	
				BlockMoveData(&buttonTitle[cc + 2], &subStr[1], subStr[0]);
				
				// look to see if this is the default button in the group. This is indicated
				// by the ID number being followed by a '*' character.
				
				if ( subStr[subStr[0]] == '*' )
				{
					*isDefault = TRUE;
					
					// remove char from string
					
					subStr[0]--;
				}
				else
					*isDefault = FALSE;
			
				// convert substring to a number. Note that results are unpredictable if
				// the string does not have a pure number following the two slashes.
				// e.g. "My Button//hello" will certainly not work!
				
				StringToNum( subStr, &gID );
				
				// shorten original string by the amount needed to remove extra
				
				buttonTitle[0] = cc - 1;
				*groupID = LoWord( gID );
				
				break;
			}	
		}
	}
	while( ++cc <= buttonTitle[0] );
}


/*------------------------***  HANDLERBUTTONGROUPCLICK  ***-----------------------------*/
/*	
turns off the buttons in the same group as this item, then turns this one on.
----------------------------------------------------------------------------------------*/

void	MoofDialog::HandleRButtonGroupClick(const short item)
{
	// <item> is already known to be a radio button, so find its "groupies" and turn them
	// all off. Then turn this one on.

	short	groupID, tIndex, gIndex, offItem;
	
	// search the group table for this item.
	// How many entries in the table?
	
	gIndex = tIndex = GetHandleSize((Handle) rGroupList) / sizeof(RGroupEntry);
	
	// for each item in the table, see if it is this item...
	
	while( tIndex )
	{
		if ((*rGroupList)[--tIndex].item == item )
		{
			// found this item- what is its group ID?
			
			groupID = (*rGroupList)[tIndex].groupID;
			
			if ( groupID != 0 )
			{
				// found the group ID and it is not zero (meaning do not handle)
				// now we need to find all the other items in this group and turn them off
				
				while( gIndex )
				{
					if ((*rGroupList)[--gIndex].groupID == groupID )
					{
						offItem = (*rGroupList)[gIndex].item;
						SetValue( offItem, 0 );
					}
				}
				
				// now turn on the desired item
				
				SetValue( item, 1 );
			}
			// this item is now fully handled, so don't bother searching any further.
			
			break;
		}
	}	
}

void MoofDialog::HandleKey( const char key, const char keycode, const short modifiers )
{
	// map return/enter keys to click of 'ok'
	if (keycode == 0x24 or keycode == 0x4C) {
		FakeClick(ok);
	}
	// and command-period to 'cancel'
	else if (key == '.' and (modifiers & cmdKey)) {
		FakeClick(cancel);
	}
	#if !TARGET_API_MAC_CARBON
		// and help key
		else if (keycode == 0x72) {
			HMSetBalloons( !HMGetBalloons() );
		}
	#endif
	// try it as a typing key
	else if (!(modifiers & cmdKey)) Type( key );
	else inherited::HandleKey( key, keycode, modifiers );
}


void MoofDialog::ShowBalloonHelp()
{
	// Determine which item the mouse is over,
	// and call help for that item, with the
	// help rect (the item's rect in global coords, inset 2 pixels)
	Point mouse;
	GetMouse(&mouse);
	short item = FindDialogItem( mDialogPtr, mouse ) + 1;
	if (item) {
		short	iType;
		Handle	iHand;
		Rect	iBox;	
		GetDialogItem( mDialogPtr, item, &iType, &iHand, &iBox );
		InsetRect( &iBox, -2, -2 );
		Point	tip = { iBox.bottom-4, iBox.right-4 };
		LocalToGlobal( &tip );
		Point topLeft = {iBox.top, iBox.left};
		LocalToGlobal( &topLeft );
		Point botRight = {iBox.bottom, iBox.right};
		LocalToGlobal( &botRight );
		Rect globalBox = { topLeft.v, topLeft.h, botRight.v, botRight.h };

		ShowBalloonHelp(item, tip, globalBox);
	}
}

void MoofDialog::GetItemBounds( const short item, Rect *bounds )
{
	short	itemType;
	Handle	itemHand;
	GetDialogItem( mDialogPtr, item, &itemType, &itemHand, bounds );
}

void MoofDialog::RefreshItem( const short item, const Boolean eraseFirst )
{
	Rect r;
	GetItemBounds(item, &r);
	if (eraseFirst) EraseRect(&r);
	Refresh(&r);
}


#pragma mark -
/*--------------------------***  UserItemVectorProc  ***--------------------------------*/
/*	

This is used to vector user-item updates to the DrawUserItem method. Do not change this-
override DrawUserItem instead.

----------------------------------------------------------------------------------------*/

static pascal void	UserItemVectorProc( DialogPtr theDialog, short item )
{
	#if TARGET_API_MAC_CARBON
		MoofDialog *moofwin = (MoofDialog*)gApplication->RefConToMoofWin( 
				GetWRefCon( GetDialogWindow(theDialog) ) );
	#else
		MoofDialog *moofwin = (MoofDialog*)gApplication->RefConToMoofWin( 
				GetWRefCon( (WindowRef)theDialog ) );
	#endif
	
	if ( moofwin )
		moofwin->DrawUserItem( item );
	else {
DebugStr("\p couldn't resolve refcon into a moofwin!");	
	}
}

static pascal Boolean DlogEventFilter(DialogPtr theDialog, EventRecord *theEvent, short *itemHit)
{
	#if TARGET_API_MAC_CARBON
		MoofDialog *moofwin = (MoofDialog*)gApplication->RefConToMoofWin( 
				GetWRefCon( GetDialogWindow(theDialog) ) );
	#else
		MoofDialog *moofwin = (MoofDialog*)gApplication->RefConToMoofWin( 
				GetWRefCon( (WindowRef)theDialog ) );
	#endif
	
	if (moofwin) return moofwin->ModalFilter( theEvent, itemHit );
	else {
DebugStr("\p couldn't resolve refcon into a moofwin!");	
		return false;
	}
}



/*
	MoofDialog.h

This is a MoofWindow that does basic dialog handling.  Many thanks to Graham Cox for his
MacZoop framework; this code is inspired by his MoofDialog class.

This adaptation is still very much a hack, and needs significant cleaning up.

This class now handles radio button groups in a very easy to set up way. In your dialog
template (DITL), append a double-slash plus a number to the name of a button to set the group
the button is a part of- e.g. "My Button//1". The appended info is used to set up a table of
radio groups that is private to MoofDialog. Naturally buttons with the same number interact as
a group. The extra information in the button title is removed
and is not visible when the dialog is shown. Clicks in such buttons are handled internally by
the default ClickItem() method. If you want to handle radio groups yourself in the old way (i.e.
without any help from MoofDialog, simply leave off the group info in the button title. Thus this
is entirely backward-compatible, too! Now was that so hard that Apple couldn't have put this
into the Dialog Manager?

*/

#ifndef MOOFDIALOG_H
#define	MOOFDIALOG_H

#include	"MenuWindow.h"

typedef struct
{
	short	item;
	short	groupID;
}
RGroupEntry, *RGroupPtr, **RGroupHdl;


class	MoofDialog : public MenuWindow
{
	typedef MenuWindow inherited;
protected:
	DialogRef		mDialogPtr;		// copy of mWindowPtr, but with DialogRef type to make Carbon happy
	Boolean			isModal;		// TRUE if dialog is modal
	Boolean			hasDefaultItem;	// TRUE if it's not modal, but has a default button anyway
	RGroupHdl		rGroupList;		// array of rGroupEntrys for managing radio groups
	Boolean			isActive;		// true when this is the active window
public:
	
	MoofDialog(const short dialogID);
	virtual ~MoofDialog();
	
	operator DialogRef(void) { return mDialogPtr; }

	void			Initialize(const short dialogID);	// (called from ctor, can't be virtual)
	virtual void	SetCicnUserItem(short item, short cicnID);
	virtual void	Draw();
	virtual void	AdjustCursor(const Point mouse, const short modifiers);
	virtual void	Click( const Point mouse, const short modifiers );
	virtual void	Activate( const Boolean becomingActive=true );
	virtual void	Idle();
	virtual void	Type( const char theKey );

	virtual void HandleKey( const char key, const char keycode, const short modifiers );

	// do a modal dialog loop
	virtual Boolean	DoModalDialog();
	
	// standard edit commands
	
	virtual Boolean	CanPasteType();
	virtual void	DoCut();
	virtual void	DoCopy();
	virtual void	DoPaste();
	virtual void	DoClear();
	
	// dialog specific stuff
	
	virtual void	SetUp();
	virtual void	ClickItem( const short theItem );		// override me for custom items
	virtual Boolean	CanClose(Boolean quitting=false);
	virtual Boolean	Filter( EventRecord* theEvent );
	virtual void	DrawUserItem( const short item );		// override me!
	virtual Boolean ModalFilter(EventRecord *theEvent, short *itemHit);	// and me!
	
	// convenience functions
	
	virtual void	SetValue( const short item, const long value );
	virtual void	SetValue( const short item, const int value ) 	{ SetValue(item, (long) value); }
	virtual void	SetValue( const short item, const short value )	{ SetValue(item, (long) value); }
	virtual void	SetValue( const short item, const Str255 value );
//	virtual void	SetValue( const short item, const double value );
//	virtual void	SetValue( const short item, const float value )	{ SetValue(item, (double) value); }
	
	virtual void	Enable( const short item, const Boolean enableIt=true );
	virtual void	Disable( const short item ) { Enable(item, false); }
	virtual Boolean IsEnabled( const short item ) const;
	
	virtual long	GetValue( const short item ) const;
	virtual void	GetValueAsText( const short item, Str255 aStr ) const;
	virtual void	HiliteItem( const short item, const short state );
	virtual void	OutlineDefaultItem();
	virtual void	GetMenuItemTextBounds( const short item, Rect* bounds );
	virtual short	FindItem( const Point localMouse );
	
	virtual void	GetItemBounds( const short item, Rect *bounds );
	virtual void	RefreshItem( const short item, const Boolean eraseFirst=false );
	
	inline	Boolean	IsModal() { return isModal; };
	
protected:

	virtual void	MakeMacWindow( const short dialogID );
	virtual Boolean	HasEditFields();
	virtual void	SetUpUserItems();
	virtual void	SetUpRadioGroups();
	virtual void	ParseRButtonTitle( Str255 buttonTitle, short* groupID, Boolean* isDefault );
	virtual void	HandleRButtonGroupClick( const short item );
	virtual void	FakeClick( const short item );
	virtual void	ShowBalloonHelp();
	virtual void	ShowBalloonHelp( const short item, const Point& tip, const Rect& rect ) {}		// override this!

private:
	Boolean	mNotMyBalloons;
	Boolean mInitialized;
	short mQtyStatesCached;			// how many [en|dis]abled states we're storing
	Boolean *mStatesCached;			// state of dialog items, while window is inactive
	CIconHandle *mCicnUserItems;	// used for easily drawing cicns as user items
};
#endif
#include "MenuWindow.h"
#include "MoofApp.h"
#include "ResourceIDs.h"

#if TARGET_API_MAC_CARBON
	#define EnableItem(x,y) EnableMenuItem(x,y)
	#define DisableItem(x,y) DisableMenuItem(x,y)
#endif

void MenuWindow::Activate( const Boolean becomingActive )
{
	// update menus
	MenuHandle mh = GetMenuHandle(kFileMenuID);
	if (becomingActive) {
		EnableItem( mh, kCloseItem );
		EnableItem( mh, kSaveItem );
		EnableItem( mh, kSaveAsItem );
	} else {
		DisableItem( mh, kCloseItem );
		DisableItem( mh, kSaveItem );
		DisableItem( mh, kSaveAsItem );
	}
	
	inherited::Activate( becomingActive );
}

Boolean MenuWindow::HandleMenuSelection( const short menuNum, const short itemNum )
{
	// we implement standard editing commands here...
	// override to to respond to further menu commands
	
	if (menuNum == kEditMenuID) {
		switch (itemNum) {
			case kUndoItem:		DoUndo();		return true;
			case kCutItem:		DoCut();		return true;
			case kCopyItem:		DoCopy();		return true;
			case kPasteItem:	DoPaste();		return true;
			case kClearItem:	DoClear();		return true;
		}
	}
	return false;
}

void MenuWindow::HandleKey( const char key, const char keycode, const short modifiers )
{
	long sel;
	switch (keycode) {
		case 0x7A:			// F1: undo
			sel = MenuKey('z');
			break;
		case 0x78:			// F2: cut
			sel = MenuKey('x');
			break;
		case 0x63:			// F3: copy
			sel = MenuKey('c');
			break;		
		case 0x76:			// F4: paste
			sel = MenuKey('v');
			break;
		default:
			inherited::HandleKey(key, keycode, modifiers);
			return;
	}

	if ( HiWord( sel )) {
		gApplication->HandleMenuSelection( HiWord(sel), LoWord(sel) );
		HiliteMenu(0);
	}
}

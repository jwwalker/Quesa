//	ResourceIDs.h
//
//	You must have a file by this name in your path which defines constants
//	for the various resources used by Advanced Moof.  You can use it for
//	your own resource constants, too.
//
//	This supplements MoofResources.h, which you don't have to worry about
//	except to make sure that the five items defined there match the actual
//	resource IDs in your resource file.

#ifndef __RESOURCEIDS_H
#define __RESOURCEIDS_H

#include "MoofResources.h"

// application creator code (be sure to change this to your own code!)
#define kAppCreatorCode 'QsVr'

// file type used for newly saved documents (see AdvWindow::DoSave)
#define kDocFileType '3DMF'

// File Menu
enum {
	kOpenItem=1,
	kCloseItem,
	kGetInfoItem=4,
	kQuitItem=6,
	// Not used:
	kNewItem=999,
	kSaveItem,
	kSaveAsItem	
};

// Edit Menu
enum {
	kUndoItem=1,
	kCutItem=3,
	kCopyItem,
	kPasteItem,
	kClearItem
};

// ALRTs
enum {
	kAboutBoxID = 128,
	kSaveConfirmID = 129,
	kUnknownErrID = 130
};

#endif

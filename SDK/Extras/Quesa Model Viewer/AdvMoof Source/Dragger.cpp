#include "Dragger.h"

static OSErr MyTrackingHandler(DragTrackingMessage theMessage, WindowPtr theWindow,
	void *handlerRefCon, DragReference theDrag)
{
	if (!handlerRefCon) return noErr;
	Dragger *dragger = (Dragger*)handlerRefCon;
	ASSERT(dragger);
	if (dragger) dragger->HandleDragTrack( theMessage, theDrag );
	return noErr;
}

static OSErr MyReceiveHandler(WindowPtr theWindow, void *handlerRefCon, DragReference theDrag)
{
	if (!handlerRefCon) return noErr;
	Dragger *dragger = (Dragger*)handlerRefCon;
	ASSERT(dragger);
	if (dragger) dragger->HandleDrop( theDrag );
	return noErr;
}

#pragma mark -

Dragger::Dragger( WindowPtr wind )
: mWindow(wind), mQtyDropAcceptTypes(0), mDropAcceptTypes(NULL), mDropHilited(false),
  mTrackHandler(NULL), mRecvHandler(NULL)
{
	// NOTE: we really ought to check the Gestalt to make
	// sure that the Drag Manager is installed!
	
	mTrackHandler = NewDragTrackingHandlerProc(MyTrackingHandler);
	mRecvHandler = NewDragReceiveHandlerProc(MyReceiveHandler);

	OSErr err;
	err = InstallTrackingHandler(mTrackHandler, wind, this);
	ASSERT(!err);
	err = InstallReceiveHandler(mRecvHandler, wind, this);
	ASSERT(!err);
}

Dragger::~Dragger()
{
	// NOTE: we really ought to check the Gestalt to make
	// sure that the Drag Manager is installed!
	OSErr err;
	err = RemoveTrackingHandler(mTrackHandler, mWindow);
	ASSERT(!err);
	err = RemoveReceiveHandler(mRecvHandler, mWindow);
	ASSERT(!err);
}


bool Dragger::CanAcceptDrag(DragReference theDrag)
{
	unsigned short items, index;
	FlavorFlags theFlags;
	ItemReference theItem;
	OSErr result;

	// loop over all dragged items...
	CountDragItems(theDrag, &items);
	for (index = 1; index <= items; index++) {
		GetDragItemReferenceNumber(theDrag, index, &theItem);

		// check for a type we accept
		bool ok = false;
		for (short i=0; i<mQtyDropAcceptTypes and not ok; i++) {
			result = GetFlavorFlags(theDrag, theItem, mDropAcceptTypes[i], &theFlags);
			if (result == noErr) ok = true;
		}
		
		// bail out if we can't accept something
		if (not ok) return false;
	}
	
	// looks good!
	return true;
}

OSErr Dragger::ExtractDropFSSpec(DragReference theDrag, ItemReference theItem, FSSpec& outspec)
{
	// Extract a FSSpec from the given drag item.
	// See http://developer.apple.com/technotes/tn/tn1085.html#Section5
	// for comments, but I think we can just do this:

	HFSFlavor hfsFlavor;
	Size bytes = sizeof(hfsFlavor);
	OSErr err = GetFlavorData 
         (theDrag, theItem, flavorTypeHFS, &hfsFlavor, &bytes, 0);
    if (!err) {
		FSSpec& inspec = hfsFlavor.fileSpec;
		err = FSMakeFSSpec( inspec.vRefNum, inspec.parID, inspec.name, &outspec );
	}
	return err;
}

void Dragger::HandleDragTrack(DragTrackingMessage theMessage, DragReference theDrag)
{
	Point mouse, localMouse;
	DragAttributes attributes;
	RgnHandle hiliteRgn;
	GetDragAttributes(theDrag, &attributes);

	switch(theMessage) {

		case kDragTrackingEnterHandler:
			break;

		case kDragTrackingEnterWindow:
			mCanAcceptDrag = CanAcceptDrag(theDrag);
			mDropHilited = false;
			break;

		case kDragTrackingInWindow:
			if (!mCanAcceptDrag) break;

			GetDragMouse(theDrag, &mouse, 0L);
			localMouse = mouse;
			GlobalToLocal(&localMouse);
			if (attributes & kDragHasLeftSenderWindow) {
				if (InDropRgn(localMouse)) {
					if (!mDropHilited) {
						RgnHandle hiliteRgn = GetDropRgn(localMouse);
						if (hiliteRgn) {
							ShowDragHilite(theDrag, hiliteRgn, true);
							DisposeRgn(hiliteRgn);
							mDropHilited = true;
						}
					}
				} else {
					if (mDropHilited) {
						HideDragHilite(theDrag, hiliteRgn, true);
						mDropHilited = false;
					}
				}
			}

			DragTrackItemUnderMouse(localMouse);
			break;

		case kDragTrackingLeaveWindow:
			if (mCanAcceptDrag && mDropHilited) {
				RgnHandle hiliteRgn = GetDropRgn(localMouse);
				HideDragHilite(theDrag, hiliteRgn, true);
			}
			mCanAcceptDrag = false;
			break;

		case kDragTrackingLeaveHandler:
			break;
	}

}

void Dragger::HandleDrop(DragReference theDrag)
{
	if (!mCanAcceptDrag) return;
	
	Point mouse;
	unsigned short items, index;
	ItemReference theItem;
	Size dataSize, stylSize;

	GetDragMouse(theDrag, &mouse, 0L);
	CountDragItems(theDrag, &items);
	for (index = 1; index <= items; index++) {
		GetDragItemReferenceNumber(theDrag, index, &theItem);
		HandleDropItem( theDrag, theItem );
	}
}

void Dragger::HandleDropItem(DragReference theDrag, ItemReference theItem)
{
	// Implementation-dependent...
	// but here's a sample:
/*
		FlavorFlags theFlags;
		result = GetFlavorFlags(theDrag, theItem, 'DATA', &theFlags);
		if ((result == noErr) && (theFlags & flavorSenderOnly)) {
			GetFlavorDataSize(theDrag, theItem, 'DATA', &dataSize);
			theData = NewPtr(dataSize);
			GetFlavorData(theDrag, theItem, 'DATA', theData, dataSize, 0L);
			MyInsertDataAtPoint(theData, dataSize, mouse, theWindow);
			DisposePtr(theData);
			continue;
		}
		result = GetFlavorFlags(theDrag, theItem, 'TEXT', &theFlags);
		if (result == noErr) {
		theStyl = 0L;
		if (GetFlavorDataSize(theDrag, theItem, 'styl', &stylSize) == noErr) {
			theStyl = NewPtr(stylSize);
			GetFlavorData(theDrag, theItem, 'styl',
			theStyl, stylSize, 0L);
		}
		GetFlavorDataSize(theDrag, theItem, 'TEXT', &dataSize);
		theData = NewPtr(dataSize);
		GetFlavorData(theDrag, theItem, 'TEXT', theData, dataSize, 0L);
		MyInsertStylTextAtPoint(theData, dataSize,
		theStyl, stylSize, mouse, theWindow);
		DisposePtr(theData);
		if (theStyl) {
			DisposePtr(theStyl);
		}
*/
}

OSErr Dragger::HideDragHilite (DragReference theDrag, RgnHandle hiliteFrame, Boolean inside)
{
	// since we've probably clobbered the region hilighted,
	// we must now invalidate it so it will be redrawn
	InvalRgn( hiliteFrame );
	// Note: for performance, we could shrink the region by 2 pixels,
	// and call ValidRgn to indicate that the interior need not
	// be redrawn.  But that will probably be ignored anyway,
	// so let's not bother.
	
	return ::HideDragHilite(theDrag);
}

OSErr Dragger::ShowDragHilite (DragReference theDrag, RgnHandle hiliteFrame, Boolean inside)
{
	// we'll let the Drag Manager do the hilighting,
	// but to work around its brokenness, we'll first erase
	// the area to be drawn
	RGBColor forec, backc;
	GetBackColor( &backc );
	GetForeColor( &forec );

	RGBForeColor( &backc );
	PenSize(2,2);
	if (inside) {
		// to frame inside, just do it
		FrameRgn( hiliteFrame );
	} else {
		// to frame outside, make a new region inset by -2 pixels
		// UNTESTED!
		RgnHandle newrgn;
		CopyRgn( hiliteFrame, newrgn );
		InsetRgn( newrgn, -2, -2 );
		FrameRgn( newrgn );
		DisposeRgn( newrgn );
	}

	PenSize(1,1);		// (which we'll assume was the previous size!)
	RGBForeColor( &forec );

	// then, do usual hilighting
	return ::ShowDragHilite(theDrag, hiliteFrame, inside);
}


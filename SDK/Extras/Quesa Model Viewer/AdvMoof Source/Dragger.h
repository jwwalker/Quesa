//	Dragger.h
//
//	This is a mix-in class that adds support to a MoofWindow for 
//	drag-and-drop -- currently, just receiving drags.

#ifndef DRAGGER_H
#define DRAGGER_H

class Dragger
{
  public:
	// constructor: requires a window ptr to attach the handlers to
	Dragger( WindowPtr wind );
	
	// destructor: uninstalls the handles
	virtual ~Dragger();

	// Drag handlers:
	virtual void HandleDragTrack(DragTrackingMessage theMessage, DragReference theDrag);
	virtual void HandleDrop(DragReference theDrag);
	virtual void HandleDropItem(DragReference theDrag, ItemReference theItem);

	// Operators called by the above handlers (you'll probably want to override):
	
	// return true if the drag contains anything we can accept:
	virtual bool CanAcceptDrag(DragReference theDrag);

	// return true if the mouse is over any drop region:
	virtual bool InDropRgn(Point localPt) { return false; }
	
	// return the region describing the drop area, for hiliting purposes:
	virtual RgnHandle GetDropRgn(Point localPt) { return NULL; }
	
	// idle with a drag -- consider hiliting other objects the drag is over, etc.
	void DragTrackItemUnderMouse(Point localPt) {}

	// show or remove the drag hilight
	// (we override Apple's broken implementation with our own)
	OSErr ShowDragHilite (DragReference theDrag, RgnHandle hiliteFrame, Boolean inside);
	OSErr HideDragHilite (DragReference theDrag, RgnHandle hiliteFrame, Boolean inside);

	// helpers:
	virtual OSErr ExtractDropFSSpec(DragReference, ItemReference, FSSpec &outspec); 

  protected:
	short mQtyDropAcceptTypes;		// how many types (flavors) of drops we can accept
	OSType *mDropAcceptTypes;		// what drop types we can accept

  private:
	bool mCanAcceptDrag;			// temp set to true if current drag can be accepted
	bool mDropHilited;				// temp set to true while content is highlighted
	WindowPtr mWindow;				// window to which the handlers are attached
	DragTrackingHandlerUPP mTrackHandler;	// pointer to drag tracking handler
	DragReceiveHandlerUPP mRecvHandler;		// pointer to drag receive handler
};

#endif

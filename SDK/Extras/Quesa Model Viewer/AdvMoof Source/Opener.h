//	Opener.h
//
//	This class provides methods and code for objects which need to 
//	open files.  It will use Navigation Services when available,
//	and fall back on Standard File otherwise.  To make this class
//	actually do anything interesting, you must subclass it and
//	override a few methods (noted below).

#ifndef __OPENER_H
#define __OPENER_H

#ifndef USENAV
#define USENAV 1
#endif

#ifndef __QUICKDRAW__
#include <MacHeaders.h>
#endif

class Opener
{
  public:
	Opener() : mQtyOpenTypes(0), mOpenTypes(NULL) {}
	virtual ~Opener() { delete[] mOpenTypes; mOpenTypes=NULL; }
	
	// main method -- call this in response to an "Open" command,
	//	or whenever you want to post the Get File dialog box.
	//	Returns the number of documents successfully opened.
	virtual short DoOpenFile();
	
	// document opener -- called with an FSSpec for each document
	//	chosen by the user during DoOpenFile.
	virtual OSErr OpenDoc(const FSSpec& filespec) { return noErr; }

	// batch document opener -- by default, just calls OpenDoc
	//	for each file, but you can override this if you want.
	virtual OSErr OpenDocBatch(AEDescList& list);

	// error handler -- override this to post a message,
	// throw an exception, etc.
	virtual void HandleOpenError( OSErr err ) {}
	
	// load info about the file types we can open from an 'open' resource
	virtual OSErr GetOpenRsrc(short openID=128);
	
	// callback function to refresh a window when the nav dialog is moved
	virtual void OpenerWindowUpdate(WindowRef window);
	
	// file types we can open... build this manually,
	// or use GetOpenRsrc to fill it in from an 'open' resource
	short mQtyOpenTypes;
	OSType *mOpenTypes;		// create your own array; deleted automatically
};

#endif
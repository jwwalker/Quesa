//	Saver.h
//
//	This class provides method and code for objects which need to
//	save files.  It will use Navigation Services when available,
//	and fall back on Standard File otherwise.  To make this class
//	actually do anything interesting, you must subclass it and
//	override a few methods (noted below).


#ifndef __SAVER_H
#define __SAVER_H

// switch: remove & recompile to avoid Nav entirely
#ifndef USENAV
#define USENAV 1
#endif

#if USENAV
#include <Navigation.h>
#endif

class Saver
{
  public:
	Saver() : mSaveFileType('TEXT'), mSaveFileCreator('????'),
			  mQtyResTypesNotCopied(0), mResTypesNotCopied(NULL)
		{ BlockMoveData(kDefName, mDefaultName, kDefName[0]+1); }
	
	// main method -- call this in response to a "Save As" command,
	// or whenever you want to post the Put File dialog box.
	// Optionally pass the address of the default FSSpec; if given,
	// the fsspec will be modified to match the user's request.
	// Calls SaveDoc to actually write the data; returns a standard
	// OSErr error code.
	virtual OSErr DoSaveAs(FSSpec *inOutSpec=NULL, ScriptCode *outScript=NULL, Str255 prompt=NULL);
	
	// document saver -- called to actually save to the given
	// filespec.  Uses a safe-save strategy, with the data actually
	// written by WriteDocData.
	virtual OSErr SaveDoc(const FSSpec& filespec, ScriptCode script, Boolean replacing=0);
	
	// This method actually writes the data.  This is often all you
	// need to override.
	virtual OSErr WriteDocData(const short fileRefNum) { return 0; }

	// Utility function for copying resources from one fsspec to another.
	virtual OSErr CopyResources(const FSSpec& srcspec, const FSSpec& destspec);
	
	// callback function to refresh a window when the nav dialog is moved
	virtual void SaverWindowUpdate(WindowRef window);

	// File type and creator code for saved files...
	// Be sure to set these (usually right after creation).
	OSType mSaveFileType;
	OSType mSaveFileCreator;

	Str255 mDefaultName;	// default file name when no fsspec is given

	// When doing the "safe save" strategy, we need to copy most resources.
	// Here, you can define what resources you don't want copied.
	short mQtyResTypesNotCopied;
	ResType* mResTypesNotCopied;
	
  private:
	static Str255 kDefName;
};


#endif

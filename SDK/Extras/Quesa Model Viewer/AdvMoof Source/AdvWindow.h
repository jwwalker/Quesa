//	AdvWindow.h											7/16/98 JJS
//
//	This class extends the basic MenuWindow functionality by providing methods
//	for handling commands, files, etc.

#ifndef __ADVWINDOW_H
#define __ADVWINDOW_H

#include "MenuWindow.h"
#include "Saver.h"

class AdvWindow: public MenuWindow, public Saver
{
	typedef MenuWindow inherited;
	
  public:
	AdvWindow(const Rect &r, const Str255 title,
			  const short defProc=documentProc, const Boolean hasCloseBox=true);

	// initialize from a file (does nothing here -- override)
	virtual void InitFromFile(const FSSpec&);

	// handle a menu command -- return true if handled, false otherwise
	virtual Boolean HandleMenuSelection( const short menuNum, const short itemNum );

	// perform the basic editing operations
	virtual void DoUndo()		{}
	virtual void DoCopy()		{}
	virtual void DoCut()		{ DoCopy(); DoClear(); }
	virtual void DoPaste()		{}
	virtual void DoClear()		{}
	
	// document handling
	virtual Boolean CanClose(Boolean quitting=false);		// close requested; if OK to delete, return true
	virtual void DoSave();			// save info into mFSSpec
	virtual OSErr DoSaveAs(FSSpec *inOutSpec=NULL, ScriptCode *outScript=NULL, Str255 prompt=NULL);		// get mFSSpec, then save
	virtual void SetFSSpec(const FSSpec& spec);	// set file spec (and update window title)
	virtual void SaverWindowUpdate(WindowRef window);  // update window when Save As dialog is moved
	// NOTE: you must override Saver::WriteDocData to actually write your data!

	virtual OSErr ReadFromFile(const short fileRefNum) {return noErr;}
	virtual void ForgetFSSpec() { mFSSpec.name[0] = 0; }
	virtual void HandleError(OSErr err)=0;	// inform the user of an error
	
	// extra drawing utilities
	virtual void DrawGrowBox( const Boolean withScrollBars=true );
	
  protected:
	FSSpec	mFSSpec;		// current file specification
	ScriptCode	mScript;	// international script tag
	Boolean mDirty;
};

#endif

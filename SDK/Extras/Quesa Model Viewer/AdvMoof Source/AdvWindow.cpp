#include "AdvWindow.h"
#include "ResourceIDs.h"

AdvWindow::AdvWindow(const Rect &r, const Str255 title,
		  const short defProc, const Boolean hasCloseBox)
: inherited( r, title, defProc, hasCloseBox ), mDirty(false) 
{
	mFSSpec.name[0] = 0;
	mSaveFileType = kDocFileType;
	mSaveFileCreator = kAppCreatorCode;
}

void AdvWindow::InitFromFile( const FSSpec &spec )
{
	short fsRefNum;
	OSErr err = FSpOpenDF(&spec, fsCurPerm, &fsRefNum);
	if (!err) {
		SetFSSpec( spec );
		SetWTitle( mWindowPtr, spec.name );
		err = ReadFromFile( fsRefNum );
		FSClose( fsRefNum );
	}
	else {
		HandleError( err );
		CloseSelf();
	}
}

Boolean AdvWindow::HandleMenuSelection( const short menuNum, const short itemNum )
{
	if (menuNum == kFileMenuID) {
		switch (itemNum) {
//			case kCloseItem:	DoClose();		return true;
			case kSaveItem:		DoSave();		return true;
			case kSaveAsItem:	DoSaveAs();		return true;
		}
		
	}
	return inherited::HandleMenuSelection( menuNum, itemNum );
}

Boolean AdvWindow::CanClose(Boolean quitting)
{
	// if not saved, ask whether to save first
	if (mDirty) {
		Str255 name;
		GetWTitle( mWindowPtr, name );
		#if TARGET_API_MAC_CARBON
			/*
				// Under Carbon, we're only supposed to ask once if quitting.
				// So remember any previous answer.
				static Boolean askedAboutQuitting = false;
				static Boolean saveBeforeQuit = false;
				if (quitting and askedAboutQuitting) {
					if (saveBeforeQuit) DoSave();
					return true;
				}
			*/
			NavDialogRef dlog = NULL;
			NavDialogCreationOptions options;
			NavGetDefaultDialogCreationOptions(&options);
			options.saveFileName = CFStringCreateWithPascalStringNoCopy(
					kCFAllocatorDefault, name, kCFStringEncodingMacRoman, NULL);
			OSStatus err = NavCreateAskSaveChangesDialog(&options,
				quitting ? kNavSaveChangesQuittingApplication : kNavSaveChangesClosingDocument,
				NULL,
				NULL,
				&dlog);
			if (noErr == err) {
				err = NavDialogRun(dlog);	// doesn't actually run under CarbonLib!!!
			}
			if (noErr == err) {
				NavUserAction answer = NavDialogGetUserAction(dlog);
				if (kNavUserActionSaveChanges == answer) DoSave();
				NavDialogDispose(dlog);
				if (kNavUserActionCancel == answer) return false;
				/*	if (quitting) {
						askedAboutQuitting = true;
						saveBeforeQuit = (kNavUserActionSaveChanges == answer);
					}
				*/
				return true;
			}
		#endif
		if (quitting) {
			ParamText( name, "\pclosing", NULL, NULL );
		}
		else {
			ParamText( name, "\pquitting", NULL, NULL );
		}
		short button = Alert(kSaveConfirmID, 0L);
		if (button == 1) DoSave();
		else if (button == 3) return false;		// cancelled
	}
	// OK, now we can close the window...
	return true;
}

void AdvWindow::DoSave()
{
	// if we don't already have a filespec name, do "save as" instead
	if (!mFSSpec.name[0]) {
		DoSaveAs();
	}
	// We'll let Saver do the hard work for us...
	else {
		OSErr err = Saver::SaveDoc(mFSSpec, mScript);
		if (err == noErr) {
			// note that we're now saved...
			mDirty = false;
		} else HandleError(err);
	}
}

OSErr AdvWindow::DoSaveAs(FSSpec *inOutSpec, ScriptCode *outScript, Str255 prompt)
{
	// We'll use Saver to display a Nav-savvy dialog box,
	// and trigger subsequent saving.
	Str255 oldFName;
	BlockMoveData( &mFSSpec.name, oldFName, 256 );
	GetWTitle( mWindowPtr, mFSSpec.name );
	OSErr err = Saver::DoSaveAs(&mFSSpec, &mScript, prompt);
	if (err == noErr) {
		mDirty = false;
		SetWTitle( mWindowPtr, mFSSpec.name );
	} else if (err == userCanceledErr) {
		// user cancelled -- restore previous FSSpec name, if any
		BlockMoveData( oldFName, mFSSpec.name, 256 );
		return err;
	}
	else HandleError(err);
	if (inOutSpec) BlockMoveData( &mFSSpec, inOutSpec, sizeof(FSSpec) );
	if (outScript) *outScript = mScript;
	return err; 
}

void AdvWindow::SetFSSpec(const FSSpec& spec)
{
	// copy the spec
	BlockMoveData( &spec, &mFSSpec, sizeof(FSSpec) );

	// set the window title to match
	SetWTitle( mWindowPtr, mFSSpec.name );
}

void AdvWindow::DrawGrowBox(const Boolean withScrollBars)
{
	// draw grow icon
	if (!withScrollBars) {
		// get previous clipping region
		RgnHandle saveClipRgn = NewRgn();
		GetClip( saveClipRgn );	

		// clip to the area just around the grow box
		Rect r = GetPortRect();
		r.top = r.bottom - 15;
		r.left = r.right - 15;
		ClipRect(&r);

		// draw the grow icon
		DrawGrowIcon(mWindowPtr);

		// restore saved clipping region
		SetClip( saveClipRgn );			/* restore previous value */
		DisposeRgn( saveClipRgn );		/* not needed any more */
	}
	else DrawGrowIcon(mWindowPtr);
}


void AdvWindow::SaverWindowUpdate(WindowPtr window)
{
	// If it's a Moof window, tell it to refresh!
	MoofWindow *moofwin = gApplication->RefConToMoofWin( GetWRefCon(window) );
	if (moofwin) {
		CGrafPtr oldPort;
		GDHandle oldDevice;
		GetGWorld(&oldPort, &oldDevice);
		moofwin->Focus();
		BeginUpdate(window);
		moofwin->Draw();
		EndUpdate(window);
		SetGWorld(oldPort, oldDevice);
	}
	else {
		Saver::SaverWindowUpdate(window);
	}
}


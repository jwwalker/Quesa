#include "Saver.h"


#ifndef __NUMBERFORMATTING__
#include <NumberFormatting.h>	// (needed for NumToString)
#endif

#ifndef __FOLDERS__
#include <Folders.h>			// (needed for FindFolder)
#endif

#if !TARGET_API_MAC_CARBON
	#ifndef __STANDARDFILE__
	#include <StandardFile.h>
	#endif
#endif

#include <Resources.h>

Str255 Saver::kDefName = "\pUntitled";

#define MIN(a,b) ((a)<(b) ? (a) : (b))

static pascal void navEventProc(NavEventCallbackMessage callBackSelector, NavCBRecPtr callBackParms, NavCallBackUserData callBackUD)
{
	switch (callBackSelector)
	{
		case kNavCBEvent:
			switch (callBackParms->eventData.eventDataParms.event->what)
			{
				case updateEvt:
					Saver *saver = (Saver*)callBackUD;
					saver->SaverWindowUpdate((WindowPtr)callBackParms->eventData.eventDataParms.event->message);
					break;
			}
			break;
	}
}

OSErr Saver::CopyResources(const FSSpec& srcspec, const FSSpec& destspec)
{
	// copy resources (ignoring errors)
	short srcRefNum = FSpOpenResFile(&srcspec, fsRdPerm);
	if (srcRefNum == -1) return ResError();
	short destRefNum = FSpOpenResFile(&destspec, fsRdWrPerm);
	if (destRefNum == -1) return ResError();

	UseResFile(srcRefNum);
	short qtyTypes = Count1Types();
	for (short i=0; i<qtyTypes; i++) {
		ResType type;
		UseResFile(srcRefNum);
		Get1IndType(&type, i);
		// skip this if it's in our list of types to not copy
		bool skip = false;
		for (short s=0; s<mQtyResTypesNotCopied; s++) {
			if (mResTypesNotCopied[s] == type) skip = true;
		}
		if (!skip) {
			short qtyRes = Count1Resources(type);
			for (short id=0; id<qtyRes; id++) {
				Str255 name;
				short who;
				ResType cares;
				UseResFile(srcRefNum);
				Handle res = Get1IndResource(type, id);
				GetResInfo(res, &who, &cares, name);
				short attrs = GetResAttrs(res);
				DetachResource(res);
				UseResFile(destRefNum);
				AddResource(res, type, id, name);
				SetResAttrs(res, attrs);
			}
		}
	}
	return noErr;
}


OSErr Saver::DoSaveAs(FSSpec *inOutSpec, ScriptCode *outScript, Str255 prompt)
{
	// Post the "Save As?" dialog, and then save the file
	// (unless the user cancels or something goes wrong.)
	OSErr err = noErr;

#if USENAV
	#if TARGET_API_MAC_CARBON
		static NavEventUPP navEventProcRD = NewNavEventUPP(navEventProc);
	#else
		static RoutineDescriptor navEventProcRD = BUILD_ROUTINE_DESCRIPTOR(uppNavEventProcInfo, navEventProc);
	#endif

	if (NavServicesAvailable()) {

		NavReplyRecord theReply;
		NavDialogOptions dialogOptions;
		NavEventUPP eventProc = NULL;
		#if TARGET_API_MAC_CARBON
			eventProc = navEventProcRD;
		#elif TARGET_RT_MAC_CFM
			eventProc = &navEventProcRD;
		#else
			eventProc = navEventProc;
		#endif

		//Initialize dialog options structure and set up default values
		err = NavGetDefaultDialogOptions( &dialogOptions );
		dialogOptions.dialogOptionFlags |= kNavNoTypePopup;
		if (prompt) BlockMoveData( prompt, dialogOptions.message, prompt[0]+1 );
		if (inOutSpec and inOutSpec->name[0]) {
			BlockMoveData( inOutSpec->name, dialogOptions.savedFileName, inOutSpec->name[0]+1 );
		} else {
			BlockMoveData( mDefaultName, dialogOptions.savedFileName, mDefaultName[0]+1 );
		}

		// post the dialog
		err = NavPutFile( NULL, &theReply, &dialogOptions, eventProc,
					mSaveFileType, mSaveFileCreator, this );

		if (theReply.validRecord && err == noErr)
		{
			FSSpec finalFSSpec;
			AEDesc resultDesc;
			// retrieve the returned selection:
			if ((err = AEGetNthDesc( &(theReply.selection), 1, typeFSS, NULL, &resultDesc )) == noErr)
			{
				#if TARGET_API_MAC_CARBON
					err = AEGetDescData( &resultDesc, &finalFSSpec, sizeof(FSSpec) );
				#else
					BlockMoveData( *resultDesc.dataHandle, &finalFSSpec, sizeof(FSSpec) );
				#endif
				if (!err) err = SaveDoc( finalFSSpec, theReply.keyScript, theReply.replacing );
				if (!err) err = NavCompleteSave( &theReply, kNavTranslateInPlace );
				if (inOutSpec) BlockMoveData( &finalFSSpec, inOutSpec, sizeof(FSSpec) );
				if (outScript) *outScript = theReply.keyScript;
			}
			//Always dispose of your reply structure and descriptors
			err = NavDisposeReply( &theReply );
		}
	
	}
	else
#endif // USENAV
	{
	#if TARGET_API_MAC_CARBON
		err = ioErr;
	#else
		// No Navigation Services available -- use good ol' StandardFile
		StandardFileReply theReply;
		Str31	sfprompt = "\pSave As:";
		if (prompt) BlockMoveData( prompt, sfprompt, MIN(31,prompt[0]+1) );
		Str255 name;
		if (inOutSpec) {
			BlockMoveData( inOutSpec->name, name, inOutSpec->name[0]+1 );
		} else {
			BlockMoveData( mDefaultName, name, mDefaultName[0]+1 );
		}
		if (name[0] > 31) name[0] = 31;		// file names can only be 31 chars!
		StandardPutFile( sfprompt, name, &theReply );
		if (theReply.sfGood) {
			err = SaveDoc( theReply.sfFile, theReply.sfScript, theReply.sfReplacing );
			if (inOutSpec) BlockMoveData( &theReply.sfFile, inOutSpec, sizeof(FSSpec) );
			if (outScript) *outScript = theReply.sfScript;
		}
	#endif
	}
	
	return err;
}

OSErr Saver::SaveDoc(const FSSpec& filespec, ScriptCode script, Boolean replacing)
{
	OSErr error;
	short fileRefNum;

	// check to see if file already exists --
	// if it does, we want to do a safe-save thing
	// (Note: we could use 'replacing', but why trust that when we can
	// check for ourselves?)
	FInfo fndrInfo;
	error = FSpGetFInfo(&filespec,&fndrInfo);
	
	if ( !error /* file already exists */ ) {
		Str255 tempFileName;
		short tempVRefNum;
		long tempDirID;
		FSSpec tempFSSpec;

		// make up a temporary filename
		unsigned long seconds;
		GetDateTime(&seconds);
		NumToString(seconds, tempFileName);

		// find the temporary folder;
		// create it if necessary
		error = FindFolder(filespec.vRefNum,
				kTemporaryFolderType, kCreateFolder,
				&tempVRefNum, &tempDirID);
		// make an FSSpec for the temporary filename
		error = FSMakeFSSpec(tempVRefNum, tempDirID,
					tempFileName, &tempFSSpec);
		if (error and error!=fnfErr) return error;

		// create a temporary file
		error = FSpCreate(&tempFSSpec, 'trsh', 'trsh', script );
		if (error) return error;

		// copy resources (ignoring errors)
		CopyResources(filespec, tempFSSpec);

		// open the newly created file
		error = FSpOpenDF(&tempFSSpec, fsRdWrPerm, &fileRefNum);
		if (error) return error;

		// write the data
		error = WriteDocData( fileRefNum );

		// close the temporary file
		FSClose(fileRefNum);

		// exchange the contents of the two files
		if (!error) error = FSpExchangeFiles(&tempFSSpec, &filespec);

		// delete the temporary file
		FSpDelete(&tempFSSpec);
	}
	else
	{
		// file doesn't already exist -- create it
		error = FSpCreate( &filespec, mSaveFileCreator, mSaveFileType, script );

		// open the newly created file
		error = FSpOpenDF(&filespec, fsRdWrPerm, &fileRefNum);
		if (error) return error;

		// write the data
		error = WriteDocData( fileRefNum );

		// close the file
		FSClose(fileRefNum);

	}
	return error;
}

void Saver::SaverWindowUpdate(WindowPtr window)
{
	SetPortWindowPort(window);
	BeginUpdate(window);
	EndUpdate(window);
}


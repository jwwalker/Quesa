#include "Opener.h"
#include <Errors.h>
#include <Navigation.h>

#if !TARGET_API_MAC_CARBON
	#ifndef __STANDARDFILE__
	#include <StandardFile.h>
	#endif
#endif

#ifndef __RESOURCES__
#include <Resources.h>
#endif

//OSErr AEGetDescData(const AEDesc *desc, DescType *typeCode, void
//	*dataBuffer, ByteCount maximumSize, ByteCount *actualSize);

static pascal void navEventProc(NavEventCallbackMessage callBackSelector, NavCBRecPtr callBackParms, NavCallBackUserData callBackUD)
{
	switch (callBackSelector)
	{
		case kNavCBEvent:
			switch (callBackParms->eventData.eventDataParms.event->what)
			{
				case updateEvt:
					Opener *opener = (Opener*)callBackUD;
					opener->OpenerWindowUpdate((WindowPtr)callBackParms->eventData.eventDataParms.event->message);
					break;
			}
			break;
	}
}


short Opener::DoOpenFile()
{
	FSSpec		returnSpec;
	OSErr	 theErr = noErr;
	short	fileCount = 0;
	
	#if USENAV
	#if TARGET_API_MAC_CARBON
		static NavEventUPP navEventProcRD = NewNavEventUPP(navEventProc);
	#else
		static RoutineDescriptor navEventProcRD = BUILD_ROUTINE_DESCRIPTOR(uppNavEventProcInfo, navEventProc);
	#endif

	if (NavServicesAvailable())
	{
		OSErr			 theErr;
		NavReplyRecord	 theReply;
		NavDialogOptions  dialogOptions;
		NavTypeListHandle openTypeList;
		NavEventUPP		 eventUPP = NULL;
		#if TARGET_API_MAC_CARBON
			eventUPP = navEventProcRD;
		#elif TARGET_RT_MAC_CFM
			eventUPP = &navEventProcRD;
		#else
			eventUPP = navEventProc;
		#endif
		
		// set options
		theErr = NavGetDefaultDialogOptions(&dialogOptions);
		dialogOptions.dialogOptionFlags |= kNavNoTypePopup;
		
		// build list of types we accept
		short n = (mQtyOpenTypes > 0 ? mQtyOpenTypes : 1);
		openTypeList = (NavTypeListHandle)NewHandle(
				sizeof(NavTypeList) + sizeof(OSType)*n );
		if (!openTypeList) {
			HandleOpenError(memFullErr);
			return 0;
		}
		NavTypeListPtr tlp = *openTypeList;
		tlp->osTypeCount = n;
		if (mQtyOpenTypes <= 0) tlp->osType[0] = 'TEXT';
		else {
			for (short i=0; i<mQtyOpenTypes; i++) {
				tlp->osType[i] = mOpenTypes[i];
			}
		}

		// call NavGetFile to do most of the work
		theErr = NavGetFile(NULL, &theReply, &dialogOptions, eventUPP, NULL,
				NULL, openTypeList, this);

		// release allocated memory etc.
		if (openTypeList) DisposeHandle((Handle)openTypeList);
		
		if (theErr) {
			if (theErr == userCanceledErr) return 0;
			HandleOpenError(theErr);
		}		
		else if (theReply.validRecord)
		{
			fileCount = OpenDocBatch( theReply.selection );
			NavDisposeReply(&theReply);
		}
	}
	else
	#endif // USENAV
	{  
		// If no Nav Servs, use Standard File instead.
		#if !TARGET_API_MAC_CARBON	
		StandardFileReply reply;
		// note: is it really necessary to restrict the types to 4?
		// Everybody does so, but Inside Mac mentions no such restriction.
		short numTypes = (mQtyOpenTypes < 4 ? mQtyOpenTypes : 4);
		
		// NOTE: if we use SFGetFile instead, we have more control and compatibility!
		StandardGetFile(nil, numTypes, mOpenTypes , &reply);
		
		if (reply.sfGood) {
			returnSpec = reply.sfFile;
			if (OpenDoc(returnSpec) == noErr) fileCount++;
		}
		#endif
	}
	return fileCount;
}

OSErr Opener::OpenDocBatch(AEDescList& list)
{
	short fileCount=0;
	FSSpec	filespec;
	AEDesc	resultDesc = {typeNull,NULL};
	long	count;
	OSErr theErr;
	
	if ((theErr = AECountItems( &(list), &count )) == noErr) {
		for (long index=1; index<=count; index++ ) {
			if ((theErr = AEGetNthDesc( &(list),
					index, typeFSS, NULL, &resultDesc )) == noErr)
			{
				#if TARGET_API_MAC_CARBON
					AEGetDescData(&resultDesc, &filespec, sizeof(FSSpec));
				#else
					BlockMoveData( *resultDesc.dataHandle, &filespec, sizeof(FSSpec) );
				#endif
				if (OpenDoc(filespec) == noErr) fileCount++;
				AEDisposeDesc( &resultDesc );
			}
		}
	}
	
	AEDisposeDesc(&resultDesc);
	return fileCount;
}


OSErr Opener::GetOpenRsrc(short openID)
{
	NavTypeListHandle openTypeList = (NavTypeListHandle)GetResource('open', openID);
	if (!openTypeList) return ResError();
	HLock((Handle)openTypeList);
	NavTypeListPtr otl = *openTypeList;

	mQtyOpenTypes = otl->osTypeCount;
	delete mOpenTypes;
	mOpenTypes = new OSType[mQtyOpenTypes];
	for (short i=0; i<mQtyOpenTypes; i++) {
		mOpenTypes[i] = otl->osType[i];
	}
	
	HUnlock((Handle)openTypeList);
	ReleaseResource((Handle)openTypeList);
	return noErr;
}

void Opener::OpenerWindowUpdate(WindowPtr window)
{
	SetPortWindowPort(window);
	BeginUpdate(window);
	EndUpdate(window);
}

/*
OSErr AEGetDescData(const AEDesc *desc, DescType *typeCode, void
*dataBuffer, ByteCount maximumSize, ByteCount *actualSize)
{  
	Handle		theHand = (Handle)desc->dataHandle;
	ByteCount	dataSize = GetHandleSize(theHand);
		
	*typeCode = desc->descriptorType;

	if (dataSize > maximumSize)
	{
		*actualSize = maximumSize;
	}
	else
	{
		*actualSize = dataSize;
	}

	BlockMoveData(*theHand, dataBuffer, *actualSize);

	return noErr;
}
*/

/*
Boolean NavEventProc(const NavEventCallbackMessage callBackSelector,
NavCBRecPtr callBackParms, NavCallBackUserData callBackUD)
{
#pragma unused (callBackUD)
	Boolean handled = false;
	
	if (callBackSelector == kNavCBEvent)
	{
		switch (callBackParms->eventData.eventDataParms.event->what)
		{
			case updateEvt:
				DoUpdate(callBackParms->eventData.eventDataParms.event);
				handled = true;
			break;
		}
	}
	
	return handled;
}
*/
/*************************************************************************************************
*
*
*		MoofOM.cpp			- Moof Object Model support code
*
*		12/18/97		©1997, Joseph Strout
*
*					Share and Enjoy
*
*************************************************************************************************/

#include "MoofOM.h"
#include "AdvApp.h"
#include "AEDescUtils.h"

// prototype of handler function for apple-events- sends them up the command chain.
// NOTE that the type of the last parameter varies randomly depending on
// which version of Universal Headers you happen to be using.   >:(
typedef long AEHandlerLastParam;

static pascal OSErr		MoofAEHandler ( const AppleEvent* aeEvt, AppleEvent* reply, AEHandlerLastParam refCon );

// global UPP to same
AEEventHandlerUPP		gAEUPP  = NewAEEventHandlerUPP( MoofAEHandler );


// default handler (normally, the first MomObject created)
MomObject *gAEHandler = NULL;

// reference to the app object -- this *MUST* derive from MomObject!!!
extern MoofApp *gApplication;

#define FailOSErr(x) { OSStatus err = x; if (err!=noErr) throw (err); }

/*-----------------------------***  GENERALACCESSOR  ***--------------------------------*/
/*	

This function is a general-purpose accessor function -- it is called by AEResolve
to turn an object reference into a token that points to the actual object.

----------------------------------------------------------------------------------------*/
static pascal OSErr GeneralAccessor( 		
				DescType		desiredClass,		// class of what we're looking for
				const AEDesc*	containerToken,		// token of the parent
				DescType		containerClass,		// class of the parent
				DescType		keyForm,			// form of the search key
				const AEDesc*	keyData,			// value of the search key
				AEDesc*			resultToken,		// stuff this with the result
				long 			refCon)				// refcon (not used)
{
	// if the container is an object, then let that object find it
	if (containerToken->descriptorType == 'Mom ') {
		MomTokenHandle hdl = (MomTokenHandle)containerToken->dataHandle;
		return (*hdl)->FindObject( desiredClass, keyForm, keyData, resultToken );
	}
	
	// if it's not, then let gApplication have a stab at it
	return ((AdvApp*)gApplication)->FindObject( desiredClass, keyForm, keyData, resultToken );
}

static pascal OSErr	MoofAEHandler( const AppleEvent* aeEvt, AppleEvent* reply, AEHandlerLastParam refCon )
{
	// this is the global vector proc that is called for every apple evnt. This determines the
	// event class and ID, then sends that info, plus the event, up the command chain, where,
	// presumably, it will be handled.

	AEEventClass	pClass;
	AEEventID		pID;
	DescType		returnedType;
	long			returnedSize;
	OSErr			theErr = noErr;
	AEDesc			tokenDesc = {typeNull, nil};	// direct parameter, in token form		
	
	try
	{
		// determine the AE class and ID (this is laborious, so it's done here once and passed on)
	
		FailOSErr( AEGetAttributePtr( 	aeEvt, keyEventClassAttr,
								 		typeType, &returnedType,
								 		&pClass, sizeof( AEEventClass ), &returnedSize ));

		FailOSErr( AEGetAttributePtr( 	aeEvt, keyEventIDAttr,
								 		typeType, &returnedType,
								 		&pID, sizeof( AEEventID ), &returnedSize ));

		// determine the proper object to handle the event

		// resolve the direct parameter, if it exists
//		DescType tokenType		= typeNull;
//		DescType dispatchClass  = typeNull;
		theErr = ResolveKeyDesc(aeEvt, keyDirectObject, typeWildCard, &tokenDesc);

		if (theErr == errAEDescNotFound || theErr == errAENotAnObjSpec) {
			// if no direct parameter was given, 
			// or one was given but it's not an object specifier,
			// then pass the event to the current handler
			if ( gAEHandler ) {
				gAEHandler->HandleAppleEvent( pClass, pID, &tokenDesc, aeEvt, reply );
				return noErr;	// (if there were an error, it would've been thrown)
			}
			else
				return errAEEventNotHandled;
		}
		
		else {	// if a direct parameter was given...

			if (theErr == errAENoSuchObject || theErr == errAEIllegalIndex)
			{
				// But: The requested object was not found.
				
				// If we were executing an "Exists..." appleevent, we can reply it here
				// because we have already determined that it DOES NOT exist.
				// First, we get the event ID. We use "eventError" instead of "theErr"
				// so that the result of AEGetAttributePtr() does not disturb the
				// errAENoSuchObject result previously returned.
				
				AEEventID	eventID;
				OSType		typeCode;
				Size			actualSize = 0L;		
				OSErr eventError = AEGetAttributePtr(aeEvt, 
												  		 keyEventIDAttr, 
												  		 typeType, 
												  		 &typeCode, 
												  		 (Ptr)&eventID, 	// Get the eventID from the AppleEvent
												  		 sizeof(eventID), 
												  		 &actualSize);
												  
				// If event was an "Exists..." message, store the result (false) in the reply
				// because AEResolve() returned errAENoSuchObject.
				
				if (eventError == noErr && eventID == kAEDoObjectsExist)
				{
					Boolean foundIt = false;
					AEPutParamPtr(reply, keyAEResult, typeBoolean, (Ptr)&foundIt, sizeof(Boolean));
					
					// Now, we set the error to noErr so that the scripting component doesn't complain
					// that the object does not exist. We only do this if we were executing an "Exists..."
					// event. Otherwise, the errAENoSuchObject will be returned.
					theErr = noErr;
				}
				throw noErr;		// can we do this?
			}
			
			if (theErr != noErr) throw theErr;

			// Pass the token returned by AEResolve(), and the original AppleEvent event and reply,
			// on to the appropriate object dispatcher

			// Check the type of the "token", as it may actually be a list or some such.

			if (tokenDesc.descriptorType == typeAEList)
			{
//				DebugStr("\plists are not yet handled!");
				throw errAEEventNotHandled;
			}
			else
			{
				// we have a single token -- dispatch the event
				MomPtr dispatchTo = NULL;
				if (tokenDesc.descriptorType == 'Mom ') {
					MomTokenHandle mh = (MomTokenHandle)tokenDesc.dataHandle;
					dispatchTo = (*mh)->momPtr;
				}
				if (dispatchTo)
					dispatchTo->HandleAppleEvent( pClass, pID, &tokenDesc, aeEvt, reply );
				else {
					if ( gAEHandler )
						gAEHandler->HandleAppleEvent( pClass, pID, &tokenDesc, aeEvt, reply );
					else
						theErr = errAEEventNotHandled;
				}
			}
		}		// end of: if we have a direct parameter

	}
	catch( OSErr err )
	{
		theErr = err;
		
		// exceptions are not propagated, but returned to applescript as
		// an error code
	}	

	AEDisposeDesc(&tokenDesc);
	
	return theErr;
}

/*----------------------------***  SETUPMOMSUPPORT  ***--------------------------------*/
/*	

Initializes the object model, installs accessor functions, etc.

----------------------------------------------------------------------------------------*/

void SetupMomSupport()
{
	// install a general apple-event handler
	AEInstallEventHandler( '****', '****', gAEUPP, (long) gApplication, FALSE );
	
	// initialize AEObjects
	OSErr error = AEObjectInit() ;

	// install object accessor function
	error = AEInstallObjectAccessor('****',
									'****', 
									NewOSLAccessorUPP(GeneralAccessor), 
									0L, 
									false);
	
	error = AEInstallObjectAccessor('cwin',
									'****', 
									NewOSLAccessorUPP(GeneralAccessor), 
									0L, 
									false);
	// (ADVANCED STUFF)
	// install object-counting function

	// install object-comparison function

}

#pragma mark -

MomObject::MomObject() : mContainer(NULL)
{
	// if we don't already have a default handler, make this it.
	if (!gAEHandler) gAEHandler = this;
}

MomObject::~MomObject()
{
	if (gAEHandler==this) gAEHandler=NULL;
}

OSErr MomObject::BuildObjSpecifier(AEDesc *descToStuff)
{
	// Build an AppleEvent reference to this object.
	// Useful when making an app recordable; such a reference can
	// be used (via AEPutParamDesc) as an AE parameter value.
	ASSERT(descToStuff);
	
	// get container
	AEDesc containerDesc = {typeNull, NULL};
	MomPtr container;
	if (!mContainer) {
		// we have no container -- which means, by default, that the application
		// is our container (unless this *is* the application!)
		container = (AdvApp*)gApplication;
		ASSERT(container != this);		// should never happen
	}
	else {
		container = mContainer;
		container->BuildObjSpecifier(&containerDesc);
	}
	
	// find this object within the container
	AEDesc keyData = {typeNull, NULL};
	DescType keyForm;
	OSErr err = container->BuildFindKey(this, &keyData, &keyForm);
	if (err) return err;
	
	// build an object specifier relative to the container
	err = CreateObjSpecifier( GetClass(), &containerDesc, keyForm, &keyData, false, descToStuff );
	
	// release temporaries
	AEDisposeDesc( &containerDesc );
	AEDisposeDesc( &keyData );
	return err;
}

OSErr MomObject::BuildPropSpecifier(DescType propID, AEDesc *descToStuff)
{
	// Build an AppleEvent reference to a property of this object.
	// Useful when making an app recordable; such a reference can
	// be used (via AEPutParamDesc) as an AE parameter value.
	ASSERT(descToStuff);
	
	// start by building a reference to this object itself
	AEDesc objDesc;
	OSErr err = BuildObjSpecifier( &objDesc );
	if (err) return err;
	
	// then, construct a ref to a property of that object.
	AEDesc propFormDesc;
	err = AECreateDesc(typeType, &propID, sizeof(propID), &propFormDesc);
	if (!err) {
		err = CreateObjSpecifier(cProperty, &objDesc, formPropertyID,
				&propFormDesc, false, descToStuff );
	}
	
	// release temporaries
	AEDisposeDesc( &propFormDesc );
	return err;
}


OSErr MomObject::SendSetDataEvent(DescType propID, const AEDesc &propValue)
{
	// send self (this app) a 'set data' AppleEvent for 
	// the specified property and value

	AppleEvent myAppleEvent, defReply;

	// 1. create the 'set data' event
	OSErr err = AECreateAppleEvent(kAECoreSuite, 'setd',
			&gSelfAddrDesc,
			kAutoGenerateReturnID, 
			kAnyTransactionID, &myAppleEvent);
	if (err) return err;
	
	// 2. set the direct object (object to set)
	//	2a. create the desc for "destination" of this object
	AEDesc propDesc;
	err = BuildPropSpecifier(propID, &propDesc);
	if (!err) {
		//	2b. attach this as the direct object of the event
		AEPutParamDesc(&myAppleEvent, keyDirectObject, &propDesc);
		AEDisposeDesc( &propDesc );

		// 3. set the value (the "to" parameter)
		AEPutParamDesc(&myAppleEvent, 'data', &propValue);
		
		// 4. finally, send this event to self
	 	err = AESend( &myAppleEvent, &defReply, 
	                    kAENoReply+kAEAlwaysInteract,
	                    kAENormalPriority, kAEDefaultTimeout,
	                    NULL, NULL);
	}
	AEDisposeDesc( &myAppleEvent );
	return err;
}


/*------------------------------***  GETPROPVALUE  ***---------------------------------*/
/*	

Given a four-character property code, stuff the property value into the given
AppleEvent descriptor.  Derived classes will probably need to override this.

----------------------------------------------------------------------------------------*/

OSErr MomObject::GetPropValue( DescType prop, AEDesc *value )
{
	// derived classes must override this
	return errAECantSupplyType;
}

/*------------------------------***  SETPROPVALUE  ***---------------------------------*/
/*	

Given a four-character property code and a value as an AE descriptor,
set the property value.  Derived classes will probably need to override this.

----------------------------------------------------------------------------------------*/

OSErr MomObject::SetPropValue( DescType, AEDesc *value )
{
	// derived classes must override this
	return errAEEventNotHandled;
}

/*----------------------------***  HANDLEAPPLEEVENT  ***--------------------------------*/
/*	

Handle the apple event, given the four-character class and ID codes, a token representing
the direct parameter, and the original AppleEvent itself.  Stuff the result, if any,
into the reply record.  Derived classes may need to override, but should call the
inherited method if they can't handle something.

----------------------------------------------------------------------------------------*/

void MomObject::HandleAppleEvent( AEEventClass pClass, AEEventID pID, 
					const AEDesc *tokenOrList, const AppleEvent* aeEvt, AppleEvent* reply )
{
	OSErr err = errAEEventNotHandled;
	switch (pID) {
		case kAEGetData:		err = HandleGetData( tokenOrList, aeEvt, reply );
								break;
		case kAESetData:		err = HandleSetData( tokenOrList, aeEvt /*, reply */);
	}
	
	if (err and err != errAEEventNotHandled) throw err;
}

/*------------------------------***  HANDLESETDATA  ***--------------------------------*/
/*	

This is a low-level function called in response to the kAESetData event.  Derived
classes can probably forget about this, and override SetPropValue instead.

----------------------------------------------------------------------------------------*/

OSErr MomObject::HandleSetData( const AEDesc *propToSet , const AEDesc *aeEvt /*, AEDesc *reply */)
{
	OSErr error 			= noErr;
	
	// make sure that we actually have some properties to set
	if (propToSet == NULL || propToSet->descriptorType != 'Mom ')
	{
		return errAEWrongDataType;
	}

	// find the property code
	MomTokenHandle zh = (MomTokenHandle)(propToSet->dataHandle);
	DescType prop = (*zh)->propertyCode;

	// find the value to set it to
	AEDesc	keyData  	= {typeNull, nil};
	error = ExtractKeyData(aeEvt, keyAEData, typeWildCard, &keyData);

	// now call SetPropValue to do the actual work
	error = SetPropValue( prop, &keyData );

	AEDisposeDesc(&keyData);
	
	return error;		
}

/*------------------------------***  HANDLEGETDATA  ***--------------------------------*/
/*	

This is a low-level function called in response to the kAEGetData event.  Derived
classes can probably forget about this, and override SetPropValue instead.

----------------------------------------------------------------------------------------*/

OSErr MomObject::HandleGetData( const AEDesc *tokenOrTokenList, const AEDesc *aeEvt, AEDesc *reply )
{
	// for now, let's assume it's just a token...
	// and that knows what property it wants!

	// also, we won't worry yet about "desiredTypes" ... this might be a list
	// of types the user wants us to coerce the result to.  LATER!
	
	OSErr error = noErr;
	AEDesc			data    		 	= {typeNull, nil};
//	AEDesc			desiredTypes   = {typeNull, nil};

//	AEGetParamDesc(appleEvent, keyAERequestedType, typeAEList, &desiredTypes);
	
//	error = GetDataFromWindow(tokenOrTokenList, &desiredTypes, &data);

	if (tokenOrTokenList->descriptorType == 'Mom ') {
		// extract the data from the token record
		MomTokenHandle hdl = (MomTokenHandle)tokenOrTokenList->dataHandle;
		error = (*hdl)->ExtractData( &data );
	}

	// if we got data, stuff it into the reply as the direct object
	if (error == noErr && reply->descriptorType != typeNull)
		error = AEPutKeyDesc(reply, keyDirectObject, &data);
	
	// dispose of temporary desc's we may have lying around
	AEDisposeDesc(&data);
//	AEDisposeDesc(&desiredTypes);

	return error;
}

/*---------------------------***  HANDLEGETPROPERTIES  ***------------------------------*/
/*	

This function should be overridden to return a list of the object's properties.
Not yet implemented.

----------------------------------------------------------------------------------------*/

OSErr MomObject::HandleGetProperties( )
{ return errAEEventNotHandled; }

/*--------------------------------***  FINDOBJECT  ***----------------------------------*/
/*	

Find an object given the four-character desiredClass, a method of finding, and the
relevant key data (e.g., a number).  The object should be contained (an element of)
this one in most cases, or contained by this one's container in the case of 
formRelativePosition (not yet implemented).

Derived classes can probably ignore this method, and instead  override
FindByPosition (and later, FindNext or FindPrevious).

----------------------------------------------------------------------------------------*/

OSErr MomObject::FindObject(const DescType desiredClass, 
		const DescType keyForm, const AEDesc* keyData, AEDesc* result )
{
	OSErr err;
	short i;
	DescType t;
	MomTokenRecord	*tokptr = 0;

	switch (keyForm) {
		case 'prop':
			tokptr = new MomTokenRecord();
			tokptr->momPtr = this;
			tokptr->dispatchClass = tokptr->objectClass = GetClass();
			err = DescToDescType( keyData, &tokptr->propertyCode );
			break; 

		case formAbsolutePosition:														// window by number
			if ( keyData->descriptorType == typeLongInteger ) {
				err = DescToShort( keyData, &i );
				if ( i > 0 )
					err = FindByPosition( desiredClass, i, tokptr );
				else
					err = FindByReversePosition( desiredClass, -i, tokptr );
			} else {	// should be typeAbsoluteOrdinal
				err = DescToDescType( keyData, &t );
				switch (t) {
					case kAEFirst:	err = FindFirst( desiredClass, tokptr );
									break;
					case kAEMiddle:	err = FindMiddle( desiredClass, tokptr );
									break;
					case kAELast:	err = FindLast( desiredClass, tokptr );
									break;
					case kAEAny:	err = FindAny( desiredClass, tokptr );
									break;
					default:
						err = errAEEventNotHandled;
				}
			}
			break;
		
		case formName:
			// key data should be type 'TEXT'
			Str255 name;
			err = DescToPString( keyData, name );
			if (!err) err = FindByName( desiredClass, name, tokptr );
			break;
	}

	if (tokptr) {
//		err = AECreateDesc('Mom ', tokptr, sizeof(MomTokenRecord), result);
		err = tokptr->CreateDesc(result);
	} else {
		err = errAEEventNotHandled;
	}
	
	return err;
}

/*------------------------------***  FINDBYPOSITION  ***--------------------------------*/
/*	

Find a contained object, given its four-character class, and its position in the list.
Build a MomTokenRecord out of it, and stuff a pointer to it into tokptr.

Derived classes will probably need to override this method.  If you can't handle
the desired class, call the inherited method.  If the index is out of range, return
errAEIllegalIndex.

----------------------------------------------------------------------------------------*/

OSErr MomObject::FindByPosition(const DescType desiredClass, const short pos, MomTokenPtr &tokptr )
{
	// not implemented here -- subclasses must override
	return errAENoSuchObject;
}

/*------------------------------***  FINDBYNAME  ***--------------------------------*/
/*	

Find a contained object, given its four-character class, and a name.
Build a MomTokenRecord out of it, and stuff a pointer to it into tokptr.

Derived classes will probably need to override this method.  If you can't handle
the desired class, call the inherited method.  If object is not found, return
errAENoSuchObject.

----------------------------------------------------------------------------------------*/
OSErr MomObject::FindByName(const DescType desiredClass,
			 const Str255 name, MomTokenPtr &tokptr)
{
	// not implemented here -- subclasses must override
	return errAENoSuchObject;
}

/*---------------------------***  other find methods  ***-------------------------------*/
/*	

Here are some other ways the user might ask to find something...
Not yet implemented.  Coming Real Soon Now.

----------------------------------------------------------------------------------------*/

OSErr MomObject::FindByID(const DescType desiredClass, MomTokenPtr &)
{ return errAENoSuchObject; }
OSErr MomObject::FindProperty(const DescType desiredClass, MomTokenPtr &)
{ return errAENoSuchObject; }

/*--------------------------***  FINDBYREVERSEPOSITION  ***-----------------------------*/
/*	

Find a contained object, given its four-character class, and its position from the
END of the list. Build a MomTokenRecord out of it, and stuff a pointer to it into tokptr.

Derived classes can safely ignore this and just override FindByPosition, unless
you want to do something special to make it more efficient.

----------------------------------------------------------------------------------------*/

OSErr MomObject::FindByReversePosition(const DescType desiredClass, const short negpos, MomTokenPtr &tokptr )
{
	short count = CountContents( desiredClass );
	if (count && negpos <= count) {
		return FindByPosition( desiredClass, count - negpos + 1, tokptr );
	} else {
		return errAEEventNotHandled;
	}
}

/*-------------------------------***  FINDMIDDLE  ***----------------------------------*/
/*	

Find the middle of a set of contained objects, given its four-character class
Build a MomTokenRecord out of it, and stuff a pointer to it into tokptr.

Derived classes can safely ignore this and just override FindByPosition, unless
you want to do something special to make it more efficient.

----------------------------------------------------------------------------------------*/

OSErr MomObject::FindMiddle(const DescType desiredClass, MomTokenPtr &tokptr )
{
	short count = CountContents( desiredClass );
	if (count) {
		return FindByPosition( desiredClass, (count+1) / 2, tokptr );
	} else {
		return errAEEventNotHandled;
	}
}

/*-------------------------------***  FINDPREVIOUS  ***--------------------------------*/
/*	

Find the object contained by this one's container, just previous to this one.
Not yet implemented!

----------------------------------------------------------------------------------------*/

OSErr MomObject::FindPrevious(const DescType desiredClass, MomTokenPtr &)
{ return errAEEventNotHandled; }

/*-------------------------------***  FINDNEXT  ***--------------------------------*/
/*	

Find the object contained by this one's container, just after this one.
Not yet implemented!

----------------------------------------------------------------------------------------*/

OSErr MomObject::FindNext(const DescType desiredClass, MomTokenPtr &)
{ return errAEEventNotHandled; }


/*-------------------------------***  FINDALL  ***--------------------------------*/
/*	

Return a list of all objects of the given class contained by this one.
Not yet implemented!

----------------------------------------------------------------------------------------*/

OSErr MomObject::FindAll(const DescType desiredClass, MomTokenPtr &)
{ return errAEEventNotHandled; }

/*-------------------------------***  FINDRANGE  ***--------------------------------*/
/*	

Return a list of a range of objects of the given class contained by this one.
Not yet implemented!

----------------------------------------------------------------------------------------*/

OSErr MomObject::FindRange(const DescType desiredClass, MomTokenPtr &)
{ return errAEEventNotHandled; }

/*------------------------------***  COUNTCONTENTS  ***---------------------------------*/
/*	

Return how many objects of the given four-character class are contained by this one.
Derived classes will probably need to override this method, but should call the
inherited method for classes they can't handle.

----------------------------------------------------------------------------------------*/

short MomObject::CountContents( const DescType )
{ return 0; }

/*----------------------------------***  COMPARE  ***-----------------------------------*/
/*	

This method will be used to compare two objects by some criteria, and returns whether
they match.
Not yet implemented.

----------------------------------------------------------------------------------------*/

Boolean MomObject::Compare( )
{ return FALSE; }

#pragma mark -

/*------------------------------***  MOMTOKENRECORD  ***--------------------------------*/
/*	

Construct an empty MomTokenRecord.

----------------------------------------------------------------------------------------*/

MomTokenRecord::MomTokenRecord()
: momPtr(0)
{
	dispatchClass = objectClass = propertyCode = typeNull;
}	

/*------------------------------***  MOMTOKENRECORD  ***--------------------------------*/
/*	

Construct a MomTokenRecord that points to the given MomObject.

----------------------------------------------------------------------------------------*/

MomTokenRecord::MomTokenRecord( MomPtr mom )
: momPtr(mom)
{
	if (mom) {
		dispatchClass = objectClass = mom->GetClass();
	} else {
		dispatchClass = objectClass = typeNull;
	}
	propertyCode = typeNull;
}	

/*--------------------------------***  EXTRACTDATA  ***---------------------------------*/
/*	

Get the actual data referred to by this token.  If the token refers to a property, get
the value of that property.

----------------------------------------------------------------------------------------*/

OSErr MomTokenRecord::ExtractData( AEDesc *result )
{
	// get the data referred to by this token
	// ...what we do here depends on the situation
	
	// if we refer to a property, get its value
	if (propertyCode != typeNull && momPtr) {
		return momPtr->GetPropValue( propertyCode, result );
	}
	
	return errAECantSupplyType;
}


/*************************************************************************************************
*
*
*		MoofOM.cpp		- Moof Object Model support code
*
*		12/19/97		©1997, Joseph Strout
*
*					Share and Enjoy
*
*************************************************************************************************/
/****************************************************************************

	Here's how it works:
		1.	All apple events are caught by the global HandleAppleEvent
			function, which constructs a MomTokenRecord from the parameter
			list.
			
			1a. It may call MomObject::FindObject() multiple times to
				resolve the parameter into a token.

		2.	That MomTokenRecord's DispatchEvent() examines the type of
			event, and calls HandleAppleEvent on its object or on the
			gApplication, as appropriate.
		
		3.	Each MomObject's HandleAppleEvent method does whatever operations
			are needed, and stuffs the result into the reply parameter.
	
	In practice, the most common types of handling are handled by MomObject,
	and you need only to override a few methods referring to properties
	or elements of your custom object.
	
 ****************************************************************************/

#ifndef __MOOFOM_H
#define __MOOFOM_H

#ifndef __QUICKDRAW__		// (easiest way to check whether MacHeaders have been included)
#include <MacHeaders.h>
#endif

#include <AEDataModel.h>
#include <Errors.h>

//	Global Setup Function
void SetupMomSupport();

//	MomObject		(Moof Object Model object)
//
//	This is the base class of any Moof object which can be
//	referred to by a MomTokenRecord.  Such objects must know how
//	to handle AppleEvent requests, etc.

class MomTokenRecord;
typedef MomTokenRecord *MomTokenPtr, **MomTokenHandle;

class MomObject {
  public:
	MomObject();
	virtual ~MomObject();
	
	//============= STUFF DERIVED CLASSES MAY WANT TO OVERRIDE ===========

	// Return this object's class, as a four-character code.
	virtual DescType GetClass() { return 'Mom '; }
	
	// Get the value of a property, and stuff it into value with AECreateDesc.
	virtual OSErr GetPropValue( DescType prop, AEDesc *value );

	// Set the value of a property, by unpacking a record
	virtual OSErr SetPropValue( DescType prop, AEDesc *value );
	
	// Find an object contained in this one (in various ways).
	virtual OSErr FindByID(const DescType desiredClass, MomTokenPtr &);
	virtual OSErr FindProperty(const DescType desiredClass, MomTokenPtr &);
	virtual OSErr FindByName(const DescType desiredClass, const Str255 name, MomTokenPtr &);
	virtual OSErr FindByPosition(const DescType desiredClass, const short position, MomTokenPtr & );

	// Find the previous or next object, relative to this one.
	virtual OSErr FindPrevious(const DescType desiredClass, MomTokenPtr &);
	virtual OSErr FindNext(const DescType desiredClass, MomTokenPtr &);

	// Find a set of objects (not implemented yet).
	virtual OSErr FindAll(const DescType desiredClass, MomTokenPtr &);
	virtual OSErr FindRange(const DescType desiredClass, MomTokenPtr &);

	// Get a list of properties (not implemented yet).
	virtual OSErr HandleGetProperties( );

	// Return how many objects of the desiredClass this one contains.
	virtual short CountContents( const DescType desiredClass );

	// Compare two objects for a match -- not implemented yet.
	virtual Boolean Compare( );

	// Find a contained element, and return the key form and data needed
	// to find it again.  This is the inverse of FindObject.
	virtual OSErr BuildFindKey(MomObject *element, AEDesc *outData, DescType *outKeyForm) { return errAENoSuchObject; }

	//============= STUFF YOU MAY NOT NEED TO OVERRIDE ===========

	// Handle an AppleEvent, considering the token; stuff result into reply.
	virtual void HandleAppleEvent( AEEventClass pClass, AEEventID pID,
				const AEDesc *tokenOrList, const AppleEvent* aeEvt, AppleEvent* reply );

	// Specific handlers which are automatically called by HandleAppleEvent
	// (above).  Not implemented yet.
	virtual OSErr HandleSetData( const AEDesc *propDescList, const AEDesc *aeEvt );
	virtual OSErr HandleGetData( const AEDesc *tokenOrTokenList, const AEDesc *desiredTypes, AEDesc *data );

	// Find an object or list of objects contained by this one
	//	(or before or after this one, in the case of formRelativePosition)
	//	...When found, create and return a token which refers to it/them
	virtual OSErr FindObject(const DescType desiredClass, const DescType keyForm, const AEDesc* keyData, AEDesc* result );

	// if you provide CountContents, you don't have to override these:
	virtual OSErr FindByReversePosition(const DescType desiredClass, const short negpos, MomTokenPtr & );
	virtual OSErr FindFirst(const DescType desiredClass, MomTokenPtr &t)
		{ return FindByPosition(desiredClass, 1,t); }
	virtual OSErr FindLast(const DescType desiredClass, MomTokenPtr &t)
		{ return FindByReversePosition(desiredClass, 1,t); }
	virtual OSErr FindMiddle(const DescType desiredClass, MomTokenPtr &);
	virtual OSErr FindAny(const DescType desiredClass, MomTokenPtr &t)
		{ return FindByPosition(desiredClass, 1,t); }

	// build an AppleEvent object reference to this object
	virtual OSErr BuildObjSpecifier(AEDesc *descToStuff);
	
	// build an AppleEvent object reference to a property
	virtual OSErr BuildPropSpecifier(DescType propID, AEDesc *descToStuff);

	// send self (this app) a 'set data' AppleEvent for 
	// the specified property and value
	virtual OSErr SendSetDataEvent(DescType propID, const AEDesc &propValue);

	// public member variable:
	// the container of this object (if NULL, assumed to be the application)
	MomObject *mContainer;
};
typedef MomObject *MomPtr, **MomHandle;

//	MomTokenRecord
//
//	This class defines a token, which is a reference to a Mom
//	(Moof object model) object or a property thereof.  This gets
//	passed around by the various Apple Event Manager and Object
//	Support Library routines, but they don't do anything with it;
//	its structure is ours to do with as we please.
//
//	Note that MomTokenRecords always *refer* to real data, but
//	do not own it; these things may be copied or deleted at will.

class MomTokenRecord {
  public:
	// structure of a token:
	DescType    dispatchClass;   // class that will handle an event
	DescType    objectClass;     // actual class of this object
	DescType    propertyCode;    // requested property code, 
                                 // or typeNull if not a property token
	MomPtr		momPtr;          // pointer to a MomObject

	// things you can do with a token:
	
	// constructors -- initialize to nothing, or to a MomPtr,
	//					or to a particular field of an AppleEvent record
	MomTokenRecord();
	MomTokenRecord( MomPtr mom );

	// FindObject -- just passes this on to its momPtr, if any
	OSErr FindObject(const DescType desiredClass, const DescType keyForm, const AEDesc* keyData, AEDesc* result )
	{ if (momPtr) return momPtr->FindObject(desiredClass, keyForm, keyData, result );
	  else return errAEEventNotHandled; /* or some other code? */ }
	
	// ExtractData -- converts this token to raw data, if possible
	// (and returns it as a descriptor of data or a MomTokenRecord)
	OSErr ExtractData( AEDesc *result );
	
	// CreateDesc -- stuff result (an existing AEDesc) such that
	//		it now refers to this token
	OSErr CreateDesc( AEDesc *result )
		{ return AECreateDesc('Mom ', this, sizeof(MomTokenRecord), result); }
};

#endif

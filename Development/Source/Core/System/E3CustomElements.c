/*  NAME:
        E3CustomElements.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3CustomElements.h"
#include "E3HashTable.h"




//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct TCEUrlDataPrivate {
	char *			url;
	TQ3StringObject	description;
	TCEUrlOptions 	options;
} TCEUrlDataPrivate;


typedef struct TCEPropertyPrivate {
	E3HashTablePtr	table;
} TCEPropertyPrivate;





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kQ3ClassNameCustomElementDepthBits		"Quesa:DepthBitsElement"
#define kQ3ClassNameCustomElementBeforeRender	"Quesa:BeforeRenderCallback"
#define kQ3ClassNameCustomElementAfterRender	"Quesa:AfterRenderCallback"
#define kQ3ClassNameCustomElementBeforePick		"Quesa:BeforePickCallback"
#define kQ3ClassNameCustomElementAfterPick		"Quesa:AfterPickCallback"

#define	kPropertyHashTableSize					10





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3propertyelement_delete_one : Free one item in the hash table.
//-----------------------------------------------------------------------------
static TQ3Status
e3propertyelement_delete_one( E3HashTablePtr theTable, TQ3ObjectType theKey,
	void *theItem, void *userData)
{
#pragma unused(theTable, theKey, userData)
	Q3Memory_Free( &theItem );
	return kQ3Success;
}





//=============================================================================
//      e3propertyelement_delete : Delete method.
//-----------------------------------------------------------------------------
static TQ3Status
e3propertyelement_delete( TCEPropertyPrivate *propData )
{
	TQ3Status	status = E3HashTable_Iterate( propData->table,
		e3propertyelement_delete_one, NULL );
	
	if (status == kQ3Success)
	{
		E3HashTable_Destroy( &propData->table );
	}
	return status;
}





//=============================================================================
//      e3propertyelement_copy_one : Copy one item in the hash table.
//-----------------------------------------------------------------------------
static TQ3Status
e3propertyelement_copy_one( E3HashTablePtr theTable, TQ3ObjectType theKey,
	void *theItem, void *userData)
{
	E3HashTablePtr	destTable = (E3HashTablePtr)userData;
	TQ3Status	status;
	TQ3Uns32	itemDataSize;
	void*	itemCopy;
	
	Q3Memory_Copy( theItem, &itemDataSize, sizeof(itemDataSize) );
	itemCopy = Q3Memory_Allocate( itemDataSize + sizeof(itemDataSize) );
	status = (itemCopy == NULL)? kQ3Failure : kQ3Success;
	if (status == kQ3Success)
	{
		Q3Memory_Copy( theItem, itemCopy, itemDataSize + sizeof(itemDataSize) );
		
		status = E3HashTable_Add( destTable, theKey, itemCopy );
		
		if (status == kQ3Failure)
		{
			Q3Memory_Free( &itemCopy );
		}
	}
	return status;
}





//=============================================================================
//      e3propertyelement_copyduplicate : Duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3propertyelement_copyduplicate( TCEPropertyPrivate *source, TCEPropertyPrivate *dest )
{
	TQ3Status status;
	
	dest->table = E3HashTable_Create( kPropertyHashTableSize );
	status = (dest->table == NULL)? kQ3Failure : kQ3Success;
	
	if ( (status == kQ3Success) && (source->table != NULL) )
	{
		status = E3HashTable_Iterate( source->table, e3propertyelement_copy_one,
			dest->table );
	}
	return status;
}





//=============================================================================
//      e3propertyelement_metahandler : Property element metahandler.
//-----------------------------------------------------------------------------
//	Since the user will not be accessing the element data directly, but only
//	using APIs such as E3Object_GetProperty, we can let the CopyAdd, CopyGet methods
//	just do the default bitwise copy.  The implementations of E3Object_SetProperty,
//	E3Object_RemoveProperty depend on that.
static TQ3XFunctionPointer
e3propertyelement_metahandler(TQ3XMethodType methodType)
{
	TQ3XFunctionPointer		theMethod = NULL;
	
	switch (methodType)
	{
		case kQ3XMethodTypeElementDelete:
			theMethod = (TQ3XFunctionPointer)e3propertyelement_delete;
			break;
			
		case kQ3XMethodTypeElementCopyDuplicate:
			theMethod = (TQ3XFunctionPointer)e3propertyelement_copyduplicate;
			break;
	}
	return theMethod;
}





#pragma mark -
//-----------------------------------------------------------------------------
//      e3nameelement_traverse : Traverse method (writing).
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_traverse (TQ3Object object, TQ3StringObject *string, TQ3ViewObject view)
{
#pragma unused(object)

	if (string == NULL || *string == NULL)
		return kQ3Success;

	if (Q3XView_SubmitWriteData(view, 0, NULL, NULL) == kQ3Failure)
		return kQ3Failure;

	return Q3Object_Submit(*string, view);
}





//=============================================================================
//      e3nameelement_readdata : Read data method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_readdata (TQ3Object object, TQ3FileObject file)
{
	TQ3Status		status;
	TQ3StringObject	string;

	string = Q3File_ReadObject(file);
	if (string == NULL) 
		return kQ3Failure;
		
	status = Q3Shape_AddElement(object, kQ3ObjectTypeCustomElementName, &string);
	Q3Object_Dispose(string);
	
	return status;
}





//=============================================================================
//      e3nameelement_copyadd : Copy add/get method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_copyadd (TQ3StringObject *source, TQ3StringObject *dest)
{
	*dest = Q3Shared_GetReference(*source);

	return (*dest != NULL) ? kQ3Success : kQ3Failure;
}





//=============================================================================
//      e3nameelement_copyduplicate : Copy duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_copyduplicate( TQ3StringObject *source, TQ3StringObject *dest)
{
	*dest = Q3Object_Duplicate(*source);

	return (*dest != NULL) ? kQ3Success : kQ3Failure;
}





//=============================================================================
//      e3nameelement_copyreplace : Copy replace method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_copyreplace (TQ3StringObject *source, TQ3StringObject *dest)
{
	TQ3StringObject	temp;
	
	temp = Q3Shared_GetReference(*source);
	if (temp == NULL) 
		return kQ3Failure;

	if (*dest)
		Q3Object_Dispose(*dest);
	
	*dest = temp;

	return kQ3Success;
}





//=============================================================================
//      e3nameelement_delete : Delete method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_delete (TQ3StringObject *string)
{
	if (*string != NULL) {
		Q3Object_Dispose(*string);
		*string = NULL;
	}

	return kQ3Success;
}





//=============================================================================
//      e3nameelement_metahandler : Name element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3nameelement_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) e3nameelement_traverse;
			break;

		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) e3nameelement_readdata;
			break;

		case kQ3XMethodTypeElementCopyAdd:
		case kQ3XMethodTypeElementCopyGet:
			theMethod = (TQ3XFunctionPointer) e3nameelement_copyadd;
			break;

		case kQ3XMethodTypeElementCopyDuplicate:
			theMethod = (TQ3XFunctionPointer) e3nameelement_copyduplicate;
			break;

		case kQ3XMethodTypeElementCopyReplace:
			theMethod = (TQ3XFunctionPointer) e3nameelement_copyreplace;
			break;

		case kQ3XMethodTypeElementDelete:
			theMethod = (TQ3XFunctionPointer) e3nameelement_delete;
			break;
		}

	return(theMethod);
}





//=============================================================================
//      e3urlelement_traverse : Traverse method (writing).
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3urlelement_traverse (TQ3Object object, TCEUrlDataPrivate *urlData, TQ3ViewObject view)
{
	TQ3Size	size;
#pragma unused(object)


	if (urlData == NULL || urlData->url == NULL)
		return kQ3Success;

	size = Q3Size_Pad(strlen(urlData->url) + 1);
	size += sizeof(TQ3Uns32);

	if (Q3XView_SubmitWriteData (view, size, urlData, NULL) == kQ3Failure)
		return kQ3Failure;

	if (urlData->description) {
		if (Q3Object_Submit(urlData->description, view) == kQ3Failure)
			return kQ3Failure;
	}

	return kQ3Success;
}





//=============================================================================
//      e3urlelement_write : Write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_write (TCEUrlDataPrivate *urlData, TQ3FileObject file)
{
	if (Q3String_Write (urlData->url, file) == kQ3Failure)
		return kQ3Failure;
		
	return Q3Uns32_Write ((TQ3Uns32)urlData->options, file);
}





//=============================================================================
//      e3urlelement_readdata : Read data method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_readdata (TQ3Object object, TQ3FileObject file)
{
	TCEUrlDataPrivate 	urlData;
	TQ3Uns32			length;
	char 				buffer[kQ3StringMaximumLength];
	
	if (Q3String_Read (buffer, &length, file) == kQ3Failure)
		return kQ3Failure;

	urlData.url = (char*)Q3Memory_Allocate(length + 1);
	strcpy (urlData.url, buffer);
	
	if (Q3Uns32_Read ((TQ3Uns32*)&urlData.options, file) == kQ3Failure)
		return kQ3Failure;

	if (Q3File_IsEndOfContainer(file, NULL) == kQ3False) {
		urlData.description = Q3File_ReadObject(file);
	}

	return Q3Shape_AddElement(object, kQ3ObjectTypeCustomElementUrl, &urlData);
}





//=============================================================================
//      e3urlelement_copyadd : Copy add/get/duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_copyadd (TCEUrlDataPrivate *source, TCEUrlDataPrivate *dest)
{
	dest->url = (char*)Q3Memory_Allocate(strlen(source->url) + 1);
	if (dest->url == NULL)
		return kQ3Failure;

	strcpy (dest->url, source->url);

	if (source->description) {  // optional
		
		// In the case of duplication, this needs to be Q3Object_Duplicate
		// rather than Q3Shared_Duplicate.  In the case of Add and Get,
		// it doesn't matter much, since the strings being copied are
		// short-lived.
		dest->description = Q3Object_Duplicate( source->description );
		if (dest->description == NULL) {
			return kQ3Failure;
		}
		
	} else {

		dest->description = NULL;
	}
		
	dest->options = source->options;
	
	return kQ3Success;
}





//=============================================================================
//      e3urlelement_copyreplace : Copy replace method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_copyreplace (TCEUrlDataPrivate *source, TCEUrlDataPrivate *dest)
{
	TQ3StringObject	string = NULL;

	if (Q3Memory_Reallocate(&dest->url ,strlen(source->url) + 1) == kQ3Failure)
		return kQ3Failure;
		
	strcpy (dest->url, source->url);

	if (source->description) {  // optional

		string = Q3Shared_GetReference (source->description);
		if (string == NULL) {
			return kQ3Failure;
		}
		
		if (dest->description) {
			Q3Object_Dispose (dest->description);
		}

		dest->description = string;
		
	} else {

		dest->description = NULL;
	}
		
	dest->options = source->options;
	
	return kQ3Success;
}





//=============================================================================
//      e3urlelement_delete : Delete method.
//-----------------------------------------------------------------------------
static TQ3Status
e3urlelement_delete (TCEUrlDataPrivate *urlData)
{
	if (urlData->url != NULL) {
	
		Q3Memory_Free (&urlData->url);
		urlData->url = NULL;
	}
	
	if (urlData->description != NULL) {
	
		Q3Object_Dispose (urlData->description);
		urlData->description = NULL;
	}

	return kQ3Success;
}





//=============================================================================
//      e3urlelement_metahandler : URL element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3urlelement_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) e3urlelement_traverse;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) e3urlelement_write;
			break;

		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) e3urlelement_readdata;
			break;

		case kQ3XMethodTypeElementCopyAdd:
		case kQ3XMethodTypeElementCopyGet:
		case kQ3XMethodTypeElementCopyDuplicate:
			theMethod = (TQ3XFunctionPointer) e3urlelement_copyadd;
			break;

		case kQ3XMethodTypeElementCopyReplace:
			theMethod = (TQ3XFunctionPointer) e3urlelement_copyreplace;
			break;

		case kQ3XMethodTypeElementDelete:
			theMethod = (TQ3XFunctionPointer) e3urlelement_delete;
			break;
		}

	return(theMethod);
}





//=============================================================================
//      e3wireelement_traverse : Traverse method (writing).
//-----------------------------------------------------------------------------
#if QUESA_SUPPORT_QUICKTIME
#pragma mark -
static TQ3Status
e3wireelement_traverse (TQ3Object object, QTAtomContainer *atom, TQ3ViewObject view)
{
#pragma unused(object)



	// To be implemented...
	return kQ3Failure;
}





//=============================================================================
//      e3wireelement_write : Write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3wireelement_write (QTAtomContainer *urlData, TQ3FileObject file)
{

	// To be implemented...
	return kQ3Failure;
}





//=============================================================================
//      e3wireelement_readdata : Read data method.
//-----------------------------------------------------------------------------
static TQ3Status
e3wireelement_readdata (TQ3Object object, TQ3FileObject file)
{

	// To be implemented...
	return kQ3Failure;
}





//=============================================================================
//      e3wireelement_copyadd : Copy add/get/duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3wireelement_copyadd(QTAtomContainer *source, QTAtomContainer *dest)
{


	// Copy the element
	*dest = *source;

	return(kQ3Success);
}





//=============================================================================
//      e3wireelement_copyreplace : Copy replace method.
//-----------------------------------------------------------------------------
static TQ3Status
e3wireelement_copyreplace(QTAtomContainer *source, QTAtomContainer *dest)
{


	// Copy the element
	*dest = *source;

	return(kQ3Success);
}





//=============================================================================
//      e3wireelement_delete : Delete method.
//-----------------------------------------------------------------------------
static TQ3Status
e3wireelement_delete(QTAtomContainer *atom)
{


	// We don't currently copy the atom container, so nothing to do
	return(kQ3Success);
}





//=============================================================================
//      e3wireelement_metahandler : Wire element metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3wireelement_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) e3wireelement_traverse;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) e3wireelement_write;
			break;

		case kQ3XMethodTypeObjectReadData:
			theMethod = (TQ3XFunctionPointer) e3wireelement_readdata;
			break;

		case kQ3XMethodTypeElementCopyAdd:
		case kQ3XMethodTypeElementCopyGet:
		case kQ3XMethodTypeElementCopyDuplicate:
			theMethod = (TQ3XFunctionPointer) e3wireelement_copyadd;
			break;

		case kQ3XMethodTypeElementCopyReplace:
			theMethod = (TQ3XFunctionPointer) e3wireelement_copyreplace;
			break;

		case kQ3XMethodTypeElementDelete:
			theMethod = (TQ3XFunctionPointer) e3wireelement_delete;
			break;
		}

	return(theMethod);
}
#endif





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Customelements_RegisterClass : Register the classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3CustomElements_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the classes
	qd3dStatus = E3ClassTree_RegisterClass(
				kQ3ObjectTypeElement,
				kQ3ObjectTypeCustomElementProperties,
				kQ3ClassNameCustomElementProperties,
				e3propertyelement_metahandler,
				sizeof(TCEPropertyPrivate));

	qd3dStatus = E3ClassTree_RegisterClass(
				kQ3ObjectTypeElement,
				kQ3ObjectTypeCustomElementName,
				kQ3ClassNameCustomElementName,
				e3nameelement_metahandler,
				sizeof(TQ3StringObject));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(
					kQ3ObjectTypeElement,
					kQ3ObjectTypeCustomElementUrl,
					kQ3ClassNameCustomElementUrl,
					e3urlelement_metahandler,
					sizeof(TCEUrlDataPrivate));

#if QUESA_SUPPORT_QUICKTIME
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(
					kQ3ObjectTypeElement,
					kQ3ObjectTypeCustomElementWire,
					kQ3ClassNameCustomElementWire,
					e3wireelement_metahandler,
					sizeof(QTAtomContainer));
#endif

	// The depth bits element does not need a metahandler, because it is plain old data
	// and is never read or written (being attached to renderers, which aren't read or
	// written).
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(
					kQ3ObjectTypeElement,
					kQ3ElementTypeDepthBits,
					kQ3ClassNameCustomElementDepthBits,
					NULL,
					sizeof(TQ3Uns32));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(
					kQ3ObjectTypeElement,
					kQ3CallbackElementTypeBeforeRender,
					kQ3ClassNameCustomElementBeforeRender,
					NULL,
					sizeof(TQ3ObjectEventCallback) );

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(
					kQ3ObjectTypeElement,
					kQ3CallbackElementTypeAfterRender,
					kQ3ClassNameCustomElementAfterRender,
					NULL,
					sizeof(TQ3ObjectEventCallback) );

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(
					kQ3ObjectTypeElement,
					kQ3CallbackElementTypeBeforePick,
					kQ3ClassNameCustomElementBeforePick,
					NULL,
					sizeof(TQ3ObjectEventCallback) );

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(
					kQ3ObjectTypeElement,
					kQ3CallbackElementTypeAfterPick,
					kQ3ClassNameCustomElementAfterPick,
					NULL,
					sizeof(TQ3ObjectEventCallback) );

	return(qd3dStatus);
}





//=============================================================================
//      E3Customelements_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3CustomElements_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
#if QUESA_SUPPORT_QUICKTIME
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeCustomElementWire, kQ3True);
#endif

	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeCustomElementProperties, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeCustomElementName, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeCustomElementUrl,  kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ElementTypeDepthBits,  kQ3True);

	return(qd3dStatus);
}





#pragma mark -
//=============================================================================
//      E3Object_GetProperty : Get the data of a property of an object.
//-----------------------------------------------------------------------------
//	You may pass NULL for actualSize if you do not need that information.
//	You may pass NULL for buffer if you need only the size.
TQ3Status
E3Object_GetProperty( TQ3Object object, TQ3ObjectType propType, TQ3Uns32 bufferSize,
	TQ3Uns32* actualSize, void* buffer )
{
	TQ3Status	status = kQ3Success;
	TCEPropertyPrivate	theData;
	
	status = Q3Object_GetElement( object, kQ3ObjectTypeCustomElementProperties,
		&theData );

	if (status == kQ3Success)
	{
		void*	theItem = E3HashTable_Find( theData.table, propType );
		
		if (theItem == NULL)
		{
			status = kQ3Failure;
		}
		else
		{
			TQ3Uns32	trueSize;
			Q3Memory_Copy( theItem, &trueSize, sizeof(TQ3Uns32) );
			if (actualSize != NULL)
			{
				*actualSize = trueSize;
			}
			
			if (buffer != NULL)
			{
				if (bufferSize >= trueSize)
				{
					char*	dataSource = ((char*)theItem) + sizeof(TQ3Uns32);
					Q3Memory_Copy( dataSource, buffer, trueSize );
				}
				else
				{
					status = kQ3Failure;
				}
			}
		}
	}
	return status;
}





//=============================================================================
//      E3Object_RemoveProperty : Remove a property from an object.
//-----------------------------------------------------------------------------
TQ3Status
E3Object_RemoveProperty( TQ3Object object, TQ3ObjectType propType )
{
	TQ3Status	status = kQ3Success;
	TCEPropertyPrivate	theData;
	
	status = Q3Object_GetElement( object, kQ3ObjectTypeCustomElementProperties,
		&theData );

	if (status == kQ3Success)
	{
		void*	theItem = E3HashTable_Find( theData.table, propType );
		
		if (theItem == NULL)
		{
			status = kQ3Failure;
		}
		else
		{
			Q3Memory_Free( &theItem );
			E3HashTable_Remove( theData.table, propType );
		}
	}
	return status;
}





//=============================================================================
//      E3Object_SetProperty : Set a property of an object.
//-----------------------------------------------------------------------------
TQ3Status
E3Object_SetProperty( TQ3Object object, TQ3ObjectType propType,
	TQ3Uns32 dataSize, const void* data )
{
	TQ3Status	status = kQ3Success;
	TCEPropertyPrivate	theData;
	
	status = Q3Object_GetElement( object, kQ3ObjectTypeCustomElementProperties,
		&theData );
	
	if (status == kQ3Failure)
	{
		theData.table = E3HashTable_Create( kPropertyHashTableSize );
		
		if (theData.table == NULL)
		{
			status = kQ3Failure;
		}
		else
		{
			status = Q3Object_AddElement( object, kQ3ObjectTypeCustomElementProperties,
				&theData );
		}
	}
	
	if (status == kQ3Success)
	{
		char*	itemContainer = Q3Memory_Allocate( dataSize + sizeof(TQ3Uns32) );
		
		if (itemContainer == NULL)
		{
			status = kQ3Failure;
		}
		else
		{
			void*	oldItem;
			
			Q3Memory_Copy( &dataSize, itemContainer, sizeof(TQ3Uns32) );
			Q3Memory_Copy( data, itemContainer + sizeof(TQ3Uns32), dataSize );
			
			oldItem = E3HashTable_Find( theData.table, propType );
			if (NULL != oldItem)
			{
				Q3Memory_Free( &oldItem );
				E3HashTable_Remove( theData.table, propType );
			}
			
			status = E3HashTable_Add( theData.table, propType, itemContainer );
			
			if (status == kQ3Failure)
			{
				Q3Memory_Free( &itemContainer );
			}
		}
	}
	return status;
}





//=============================================================================
//      E3NameElement_SetData : : Attach a name to an object.
//-----------------------------------------------------------------------------
//		Note :	Passing NULL for the name parameter removes the Name Element
//				from the object, if one is present.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3NameElement_SetData(TQ3Object object, const char *name)
{
	TQ3Status status;
	TQ3StringObject	string = NULL;

	if (name == NULL)
		return Q3Object_ClearElement(object, kQ3ObjectTypeCustomElementName);


	string = Q3CString_New(name);
	if (string == NULL)
		return kQ3Failure;
				
	status = Q3Object_AddElement(object, kQ3ObjectTypeCustomElementName, &string);
	Q3Object_Dispose(string);
	
	return status;
}





//=============================================================================
//      E3NameElement_GetData : Get the name  for an object.
//-----------------------------------------------------------------------------
//		Note :	The memory for the name must be disposed of by the caller.
//-----------------------------------------------------------------------------
TQ3Status
E3NameElement_GetData(TQ3Object object, char **name)
{
	TQ3Status status;
	TQ3StringObject	string = NULL;



	// Initialise a return value
	*name = NULL;

	
	if (Q3Object_ContainsElement(object, kQ3ObjectTypeCustomElementName)) {
	
		status = Q3Object_GetElement(object, kQ3ObjectTypeCustomElementName, (TQ3StringObject)&string);
		if (status == kQ3Failure)
			return status;
			
	} else {

		return kQ3Failure;
	}

	status = Q3CString_GetString(string, name);
	Q3Object_Dispose(string);
					
	return status;
}





//=============================================================================
//      E3NameElement_EmptyData : Dispose of the memory allocated.
//-----------------------------------------------------------------------------
TQ3Status
E3NameElement_EmptyData(char **name)
{


	// Frees name and sets it to NULL
	return Q3CString_EmptyData(name);
}





//=============================================================================
//      E3UrlElement_SetData : Set the URL for an object.
//-----------------------------------------------------------------------------
//		Note :	Passing NULL for the name parameter removes the URL Element
//				from the object, if one is present.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3UrlElement_SetData(TQ3Object object, TCEUrlData *urlData)
{
	TQ3Status 			status;
	TCEUrlDataPrivate  	urlDataPrivate;
	TQ3StringObject		string = NULL;

	// That's what QD3D does: you can't hold a description without url
	if (urlData == NULL || urlData->url == NULL ||urlData->url[0] == '\0')
		return Q3Object_ClearElement(object, kQ3ObjectTypeCustomElementUrl);


	// Validate our parameters
	Q3_ASSERT(urlData->options == kCEUrlOptionNone || urlData->options == kCEUrlOptionUseMap);
	

	urlDataPrivate.url = urlData->url;
	urlDataPrivate.options = urlData->options;
	urlDataPrivate.description = NULL;

	if (urlData->description != NULL) {

		string = Q3CString_New (urlData->description);

		if (string == NULL)
			return kQ3Failure;
		else
			urlDataPrivate.description = string;
	}

	status = Q3Object_AddElement(object, kQ3ObjectTypeCustomElementUrl, &urlDataPrivate);
		
	if (string != NULL)
		Q3Object_Dispose(string);

	return status;
}





//=============================================================================
//      E3UrlElement_GetData : Get the URL for an object.
//-----------------------------------------------------------------------------
//		Note :	The memory for the name must be disposed of by the caller.
//-----------------------------------------------------------------------------
TQ3Status
E3UrlElement_GetData(TQ3Object object, TCEUrlData **urlData)
{
	TQ3Status 			status;
	TCEUrlDataPrivate  	urlDataPrivate;


	// Initialise a return value
	*urlData = NULL;


	if (Q3Object_ContainsElement(object, kQ3ObjectTypeCustomElementUrl)) {
	
		status = Q3Object_GetElement(object, kQ3ObjectTypeCustomElementUrl, &urlDataPrivate);
		if (status == kQ3Failure)
			return status;
			
	} else {
	
		return kQ3Failure;
	}


	*urlData = (TCEUrlData*)Q3Memory_Allocate(sizeof(TCEUrlData));
	(**urlData).url = (char*)Q3Memory_Allocate(strlen(urlDataPrivate.url) + 1);
		
	strcpy ((**urlData).url, urlDataPrivate.url);
	
	(**urlData).options = urlDataPrivate.options;
	(**urlData).description = NULL;

	if (urlDataPrivate.description) {
	
		status = Q3CString_GetString(urlDataPrivate.description, &(**urlData).description);
		Q3Object_Dispose(urlDataPrivate.description);
	}

	return status;
}





//=============================================================================
//      E3UrlElement_EmptyData : Dispose of the memory allocated.
//-----------------------------------------------------------------------------
TQ3Status
E3UrlElement_EmptyData(TCEUrlData **urlData)
{


	Q3Memory_Free(&(**urlData).url);

	if ((**urlData).description)
		Q3CString_EmptyData(&(**urlData).description);

	Q3Memory_Free(urlData);
	
	return kQ3Success; 
}





//=============================================================================
//      E3WireElement_SetData : Set the atom container element.
//-----------------------------------------------------------------------------
//		Note : We don't currently copy atom containers.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_SUPPORT_QUICKTIME
TQ3Status
E3WireElement_SetData(TQ3Object object, QTAtomContainer wireData)
{	TQ3Status		qd3dStatus;



	// Add the element
	qd3dStatus = Q3Object_AddElement(object, kQ3ObjectTypeCustomElementWire, &wireData);

	return(qd3dStatus);
}





//=============================================================================
//      E3WireElement_GetData : Get the atom container element.
//-----------------------------------------------------------------------------
//		Note : We don't currently copy atom containers.
//-----------------------------------------------------------------------------
TQ3Status
E3WireElement_GetData(TQ3Object object, QTAtomContainer *wireData)
{	TQ3Status			qd3dStatus;



	// Initialise a return value
	*wireData = NULL;



	// Get the element
	qd3dStatus = kQ3Success;
	if (Q3Object_ContainsElement(object, kQ3ObjectTypeCustomElementWire))
		qd3dStatus = Q3Object_GetElement(object, kQ3ObjectTypeCustomElementWire, wireData);

	return(qd3dStatus);
}





//=============================================================================
//      E3WireElement_EmptyData : Release an atom container element.
//-----------------------------------------------------------------------------
//		Note :	We don't currently copy atom containers, so for now we just
//				reset the pointer.
//-----------------------------------------------------------------------------
TQ3Status
E3WireElement_EmptyData(QTAtomContainer *wireData)
{


	// Reset the pointer
	*wireData = NULL;

	return(kQ3Success);
}
#endif // QUESA_SUPPORT_QUICKTIME

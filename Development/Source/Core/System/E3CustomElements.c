/*  NAME:
        E3CustomElements.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3CustomElements.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct {
	char *			url;
	TQ3StringObject	description;
	TCEUrlOptions 	options;
} TCEUrlDataPrivate;





//=============================================================================
//      Internal functions
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
//      e3nameelement_copyadd : Copy add/get/duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3nameelement_copyadd (TQ3StringObject *source, TQ3StringObject *dest)
{
	*dest = Q3Shared_GetReference(*source);

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
		case kQ3XMethodTypeElementCopyDuplicate:
			theMethod = (TQ3XFunctionPointer) e3nameelement_copyadd;
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

	urlData.url = (char*)E3Memory_Allocate(length + 1);
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
	dest->url = (char*)E3Memory_Allocate(strlen(source->url) + 1);
	if (dest->url == NULL)
		return kQ3Failure;

	strcpy (dest->url, source->url);

	if (source->description) {  // optional

		dest->description = Q3Shared_GetReference (source->description);
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

	if (E3Memory_Reallocate(&dest->url ,strlen(source->url) + 1) == kQ3Failure)
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
	
		E3Memory_Free (&urlData->url);
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

	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeCustomElementName, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeCustomElementUrl,  kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3NameElement_SetData : : Attach a name to an object.
//-----------------------------------------------------------------------------
//		Note :	Passing NULL for the name parameter removes the Name Element
//				from the object, if one is present.
//
//				The object's type must be kQ3SharedTypeShape or
//				kQ3SharedTypeSet.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3NameElement_SetData(TQ3Object object, const char *name)
{
	TQ3Status status;
	TQ3StringObject	string = NULL;

	if ((Q3Object_IsType(object, kQ3SharedTypeShape) == kQ3False) &&
		(Q3Object_IsType(object, kQ3SharedTypeSet) == kQ3False))
		return kQ3Failure;

	if (name == NULL)
		return Q3Shape_ClearElement(object, kQ3ObjectTypeCustomElementName);


	string = Q3CString_New(name);
	if (string == NULL)
		return kQ3Failure;
				
	status = Q3Shape_AddElement(object, kQ3ObjectTypeCustomElementName, &string);
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

	
	if ((Q3Object_IsType(object, kQ3SharedTypeShape) == kQ3False) &&
		(Q3Object_IsType(object, kQ3SharedTypeSet) == kQ3False))
		return kQ3Failure;

	if (Q3Shape_ContainsElement(object, kQ3ObjectTypeCustomElementName)) {
	
		status = Q3Shape_GetElement(object, kQ3ObjectTypeCustomElementName, (TQ3StringObject)&string);
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
//
//				The object's type must be kQ3SharedTypeShape or
//				kQ3SharedTypeSet.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3UrlElement_SetData(TQ3Object object, TCEUrlData *urlData)
{
	TQ3Status 			status;
	TCEUrlDataPrivate  	urlDataPrivate;
	TQ3StringObject		string = NULL;

	if ((Q3Object_IsType(object, kQ3SharedTypeShape) == kQ3False) &&
		(Q3Object_IsType(object, kQ3SharedTypeSet) == kQ3False))
		return kQ3Failure;

	// That's what QD3D does: you can't hold a description without url
	if (urlData == NULL || urlData->url == NULL ||urlData->url[0] == '\0')
		return Q3Shape_ClearElement(object, kQ3ObjectTypeCustomElementUrl);


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

	status = Q3Shape_AddElement(object, kQ3ObjectTypeCustomElementUrl, &urlDataPrivate);
		
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


	if ((Q3Object_IsType(object, kQ3SharedTypeShape) == kQ3False) &&
		(Q3Object_IsType(object, kQ3SharedTypeSet) == kQ3False))
		return kQ3Failure;

	if (Q3Shape_ContainsElement(object, kQ3ObjectTypeCustomElementUrl)) {
	
		status = Q3Shape_GetElement(object, kQ3ObjectTypeCustomElementUrl, &urlDataPrivate);
		if (status == kQ3Failure)
			return status;
			
	} else {
	
		return kQ3Failure;
	}


	*urlData = (TCEUrlData*)E3Memory_Allocate(sizeof(TCEUrlData));
	(**urlData).url = (char*)E3Memory_Allocate(strlen(urlDataPrivate.url) + 1);
		
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


	E3Memory_Free(&(**urlData).url);

	if ((**urlData).description)
		Q3CString_EmptyData(&(**urlData).description);

	E3Memory_Free(urlData);
	
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



	// Make sure we have a shape or a set
	if ((Q3Object_IsType(object, kQ3SharedTypeShape) == kQ3False) &&
		(Q3Object_IsType(object, kQ3SharedTypeSet)   == kQ3False))
		return(kQ3Failure);



	// Add the element
	qd3dStatus = Q3Shape_AddElement(object, kQ3ObjectTypeCustomElementWire, &wireData);

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



	// Make sure we have a shape or a set
	if ((Q3Object_IsType(object, kQ3SharedTypeShape) == kQ3False) &&
		(Q3Object_IsType(object, kQ3SharedTypeSet)   == kQ3False))
		return(kQ3Failure);



	// Get the element
	qd3dStatus = kQ3Success;
	if (Q3Shape_ContainsElement(object, kQ3ObjectTypeCustomElementWire))
		qd3dStatus = Q3Shape_GetElement(object, kQ3ObjectTypeCustomElementWire, wireData);

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

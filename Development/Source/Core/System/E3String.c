/*  NAME:
        E3String.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#include "E3String.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef char *TQ3StringPtr;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3string_c_new : C string new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3string_c_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3StringPtr			*instanceData = (TQ3StringPtr *)     privateData;
	const TQ3StringPtr		theString     = (const TQ3StringPtr) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	*instanceData = (TQ3StringPtr) Q3Memory_Allocate(strlen(theString) + 1);
	if (*instanceData == NULL)
		return(kQ3Failure);
	
	strcpy(*instanceData, theString);	

	return(kQ3Success);
}





//=============================================================================
//      e3string_c_delete : C string delete method.
//-----------------------------------------------------------------------------
static void
e3string_c_delete(TQ3Object theObject, void *privateData)
{	TQ3StringPtr	*instanceData = (TQ3StringPtr *) privateData;
#pragma unused(privateData)



	// Dispose of our instance data
	Q3Memory_Free(instanceData);
}





//=============================================================================
//      e3string_c_duplicate : C string duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3string_c_duplicate(TQ3Object fromObject, const void *fromPrivateData,
					 TQ3Object toObject,   void       *toPrivateData)
{	const TQ3StringPtr		*fromInstanceData = (const TQ3StringPtr *) fromPrivateData;
	TQ3StringPtr			*toInstanceData   = (TQ3StringPtr *)       toPrivateData;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	*toInstanceData = NULL;
	if (*fromInstanceData != NULL)
		{
		*toInstanceData = (TQ3StringPtr) Q3Memory_Allocate(strlen(*fromInstanceData) + 1);
		if (*toInstanceData == NULL)
			return(kQ3Failure);
	
		strcpy(*toInstanceData, *fromInstanceData);	
		}

	return(kQ3Success);
}





//=============================================================================
//      e3string_c_metahandler : C string metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3string_c_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3string_c_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3string_c_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3string_c_duplicate;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3String_RegisterClass : Register the string classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3String_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
											kQ3SharedTypeString,
											kQ3ClassNameString,
											NULL,
											0);
	
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeString,
												kQ3StringTypeCString,
												kQ3ClassNameStringTypeC,
												e3string_c_metahandler,
												sizeof(TQ3StringPtr));

	return(qd3dStatus);
}





//=============================================================================
//      E3String_UnregisterClass : Unregister the string classes.
//-----------------------------------------------------------------------------
TQ3Status
E3String_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StringTypeCString, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3SharedTypeString,  kQ3True);
	return(qd3dStatus);
}





//=============================================================================
//      E3String_GetType : Get the type of a string object.
//-----------------------------------------------------------------------------
//		Note : Only kQ3StringTypeCString objects are supported at present.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3String_GetType(TQ3StringObject stringObj)
{


	// Get the type
	return(E3ClassTree_GetObjectType(stringObj, kQ3SharedTypeString));
}





//=============================================================================
//      E3CString_New : Create a new C string object.
//-----------------------------------------------------------------------------
TQ3StringObject
E3CString_New(const char *str)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3StringTypeCString, kQ3False, str);

	return(theObject);
}





//=============================================================================
//      E3CString_GetLength : Get the length of a C string object.
//-----------------------------------------------------------------------------
TQ3Status
E3CString_GetLength(TQ3StringObject stringObj, TQ3Uns32 *length)
{	TQ3StringPtr		*instanceData = (TQ3StringPtr *) stringObj->instanceData;



	// Return the length of the string
	if (*instanceData == NULL)
		*length = 0;
	else
		*length = strlen(*instanceData);
	
	return(kQ3Success);
}





//=============================================================================
//      E3CString_SetString : Set the data for a string.
//-----------------------------------------------------------------------------
TQ3Status
E3CString_SetString(TQ3StringObject stringObj, const char *str)
{	TQ3StringPtr		*instanceData = (TQ3StringPtr *) stringObj->instanceData;
	TQ3Status			qd3dStatus;



	// Resize the string data
	qd3dStatus = Q3Memory_Reallocate(instanceData, strlen(str) + 1);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Copy the new string data in
	strcpy(*instanceData, str);

	Q3Shared_Edited(stringObj);

	return(kQ3Success);
}





//=============================================================================
//      E3CString_GetString : Get the data for a string.
//-----------------------------------------------------------------------------
//		Note :	We return a copy of the string data in str - posting a warning
//				if this pointer is not already NULL, since this may indicate
//				a memory leak.
//-----------------------------------------------------------------------------
TQ3Status
E3CString_GetString(TQ3StringObject stringObj, char **str)
{	TQ3StringPtr		*instanceData = (TQ3StringPtr *) stringObj->instanceData;



	// If the pointer isn't NULL, warn that they might be leaking memory
	if (*str != NULL)
		E3ErrorManager_PostWarning(kQ3WarningPossibleMemoryLeak);



	// Allocate the data for the string
	*str = (char *) Q3Memory_Allocate(strlen(*instanceData) + 1);
	if (*str == NULL)
		return(kQ3Failure);
	
	
	
	// Copy the string
	strcpy(*str, *instanceData);	

	return(kQ3Success);
}





//=============================================================================
//      E3CString_EmptyData : Dispose of data allocated by Q3CString_GetData.
//-----------------------------------------------------------------------------
//		Note :	After disposing of the data, we must also reset str to NULL.
//				This is done for us by Q3Memory_Free, but we assert it in case
//				that behaviour is ever changed.
//-----------------------------------------------------------------------------
TQ3Status
E3CString_EmptyData(char **str)
{


	// Dispose of the memory
	Q3Memory_Free(str);



	// Make sure we really did get rid of it
	Q3_ASSERT(*str == NULL);
	
	return(kQ3Success);
}






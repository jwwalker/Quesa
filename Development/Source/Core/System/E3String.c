/*  NAME:
        E3String.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

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
#include "E3String.h"
#include "E3Main.h"




//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef char *TQ3StringPtr;





class E3String : public E3Shared // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
	{
Q3_CLASS_ENUMS ( kQ3SharedTypeString, E3String, E3Shared )
public :

	// There is no extra data for this class
	} ;
	


class E3CString : public E3String  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3StringTypeCString, E3CString, E3String )
public :

	TQ3StringPtr						instanceData ;
	} ;
	


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
	qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (	kQ3ClassNameString,
										NULL,
										E3String ) ;
	
	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameStringTypeC,
											e3string_c_metahandler,
											E3CString ) ;

	return(qd3dStatus);
}





//=============================================================================
//      E3String_UnregisterClass : Unregister the string classes.
//-----------------------------------------------------------------------------
TQ3Status
E3String_UnregisterClass(void)
{



	// Unregister the classes
	E3ClassTree::UnregisterClass(kQ3StringTypeCString, kQ3True);
	E3ClassTree::UnregisterClass(kQ3SharedTypeString,  kQ3True);
	return(kQ3Success);
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
	return stringObj->GetObjectType ( kQ3SharedTypeString ) ;
}





//=============================================================================
//      E3CString_New : Create a new C string object.
//-----------------------------------------------------------------------------
TQ3StringObject
E3CString_New(const char *str)
	{
	// Create the object
	return E3ClassTree::CreateInstance ( kQ3StringTypeCString, kQ3False, str ) ;
	}





//=============================================================================
//      E3CString_GetLength : Get the length of a C string object.
//-----------------------------------------------------------------------------
TQ3Status
E3CString_GetLength(TQ3StringObject stringObj, TQ3Uns32 *length)
	{
	// Return the length of the string
	if ( ( (E3CString*) stringObj )->instanceData == NULL )
		*length = 0 ;
	else
		*length = strlen ( ( (E3CString*) stringObj )->instanceData ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3CString_SetString : Set the data for a string.
//-----------------------------------------------------------------------------
TQ3Status
E3CString_SetString(TQ3StringObject stringObj, const char *str)
	{
	TQ3StringPtr* instanceData = & ( (E3CString*) stringObj )->instanceData ;

	// Resize the string data
	TQ3Status qd3dStatus = Q3Memory_Reallocate( instanceData, strlen ( str ) + 1 ) ;
	if ( qd3dStatus == kQ3Failure )
		return qd3dStatus ;



	// Copy the new string data in
	strcpy ( *instanceData, str ) ;

	Q3Shared_Edited ( stringObj ) ;

	return kQ3Success ;
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
	{
	TQ3StringPtr* instanceData = & ( (E3CString*) stringObj )->instanceData ;

	// If the pointer isn't NULL, warn that they might be leaking memory
	if ( *str != NULL )
		E3ErrorManager_PostWarning ( kQ3WarningPossibleMemoryLeak ) ;



	// Allocate the data for the string
	*str = (char *) Q3Memory_Allocate ( strlen ( *instanceData ) + 1 ) ;
	if ( *str == NULL )
		return kQ3Failure ;
	
	
	
	// Copy the string
	strcpy ( *str, *instanceData ) ;	

	return kQ3Success ;
	}





//=============================================================================
//      E3CString_PeekString : Get the actual data of a string, not a copy.
//-----------------------------------------------------------------------------
const char*
E3CString_PeekString( TQ3StringObject stringObj )
{
	TQ3StringPtr		*instanceData = & ( (E3CString*) stringObj )->instanceData ;
	
	return *instanceData;
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






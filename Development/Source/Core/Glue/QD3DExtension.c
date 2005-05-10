/*  NAME:
        QD3DExtension.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
#include "E3Extension.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3XObjectHierarchy_RegisterClass : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3XObjectClass
Q3XObjectHierarchy_RegisterClass(TQ3ObjectType parentType, TQ3ObjectType *objectType,
								const char *objectName, TQ3XMetaHandler metaHandler,
								TQ3XMetaHandler virtualMetaHandler, TQ3Uns32 methodsSize, TQ3Uns32 instanceSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(objectType), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(objectName), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on parentType
		return(NULL);

	if (0) // Further checks on objectType
		return(NULL);

	if (0) // Further checks on objectName
		return(NULL);

	if (0) // Further checks on metaHandler
		return(NULL);

	if (0) // Further checks on virtualMetaHandler
		return(NULL);

	if (0) // Further checks on methodsSize
		return(NULL);

	if (0) // Further checks on instanceSize
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XObjectHierarchy_RegisterClass(parentType, objectType, objectName,
		metaHandler, virtualMetaHandler, methodsSize, instanceSize));
}





//=============================================================================
//      Q3XObjectHierarchy_UnregisterClass : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XObjectHierarchy_UnregisterClass(TQ3XObjectClass objectClass)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(objectClass), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClass
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XObjectHierarchy_UnregisterClass(objectClass));
}





//=============================================================================
//      Q3XObjectClass_GetMethod : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3XFunctionPointer
Q3XObjectClass_GetMethod(TQ3XObjectClass objectClass, TQ3XMethodType methodType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClass
		return(NULL);

	if (0) // Further checks on methodType
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XObjectClass_GetMethod(objectClass, methodType));
}





//=============================================================================
//      Q3XObjectHierarchy_NewObject : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Object
Q3XObjectHierarchy_NewObject(TQ3XObjectClass objectClass, void *parameters)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClass
		return(NULL);

	if (0) // Further checks on parameters
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XObjectHierarchy_NewObject(objectClass, parameters));
}





//=============================================================================
//      Q3XObjectClass_GetLeafType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3XObjectClass_GetLeafType(TQ3XObjectClass objectClass)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClass
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XObjectClass_GetLeafType(objectClass));
}





//=============================================================================
//      Q3XObjectHierarchy_GetClassVersion : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XObjectHierarchy_GetClassVersion(TQ3ObjectType objectClassType, TQ3XObjectClassVersion *version)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(version), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClassType
		return(kQ3Failure);

	if (0) // Further checks on version
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XObjectHierarchy_GetClassVersion(objectClassType, version));
}





//=============================================================================
//      Q3XObjectClass_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XObjectClass_GetType(TQ3XObjectClass objectClass, TQ3ObjectType *theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theType), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClass
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XObjectClass_GetType(objectClass, theType));
}





//=============================================================================
//      Q3XObjectHierarchy_FindClassByType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3XObjectClass
Q3XObjectHierarchy_FindClassByType(TQ3ObjectType theType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theType
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XObjectHierarchy_FindClassByType(theType));
}





//=============================================================================
//      Q3XObjectClass_GetPrivate : Quesa API entry point.
//-----------------------------------------------------------------------------
void *
Q3XObjectClass_GetPrivate(TQ3XObjectClass objectClass, TQ3Object targetObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(targetObject->IsObjectValid (), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClass
		return(NULL);

	if (0) // Further checks on targetObject
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XObjectClass_GetPrivate(objectClass, targetObject));
}





//=============================================================================
//      Q3XObject_GetClass : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3XObjectClass
Q3XObject_GetClass(TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XObject_GetClass(object));
}





//=============================================================================
//      Q3XSharedLibrary_Register : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XSharedLibrary_Register(TQ3XSharedLibraryInfo *sharedLibraryInfo)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(sharedLibraryInfo), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedLibraryInfo
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XSharedLibrary_Register(sharedLibraryInfo));
}





//=============================================================================
//      Q3XSharedLibrary_Unregister : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XSharedLibrary_Unregister(TQ3Uns32 sharedLibrary)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedLibrary
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XSharedLibrary_Unregister(sharedLibrary));
}





//=============================================================================
//      Q3XError_Post : Quesa API entry point.
//-----------------------------------------------------------------------------
void
Q3XError_Post(TQ3Error error)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on error
		return;
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	E3XError_Post(error);
}





//=============================================================================
//      Q3XWarning_Post : Quesa API entry point.
//-----------------------------------------------------------------------------
void
Q3XWarning_Post(TQ3Warning warning)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on warning
		return;
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	E3XWarning_Post(warning);
}





//=============================================================================
//      Q3XNotice_Post : Quesa API entry point.
//-----------------------------------------------------------------------------
void
Q3XNotice_Post(TQ3Notice notice)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on notice
		return;
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	E3XNotice_Post(notice);
}





//=============================================================================
//      Q3XMacintoshError_Post : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
void
Q3XMacintoshError_Post(OSErr macOSErr)
{


	// Call our implementation
	E3XMacintoshError_Post(macOSErr);
}
#endif





/*  NAME:
        E3Extension.c

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
#include "E3Extension.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3XObjectHierarchy_RegisterClass : Register a new object class.
//-----------------------------------------------------------------------------
//		Note :	The Quesa class implementation does not support virtual
//				methods, as methods are inherited from the parent metahandler.
//
//				If you are using the virtual metahandler parameter, you will
//				probably need to update the class tree to support this: please
//				contact dair@webthing.net about this.
//
//				In addition, we do not use the methodsSize parameter: if you
//				understand how this parameter is used, please let me know - the
//				comments in QD3DExtension.h seem to indicate that it's used to
//				provide class specific private data, however the routine to
//				access it (Q3ObjectClass_GetClassPrivate), does't actually
//				exist (and so we don't support it).
//-----------------------------------------------------------------------------
TQ3XObjectClass
E3XObjectHierarchy_RegisterClass(TQ3ObjectType parentType, TQ3ObjectType *objectType, char *objectName, TQ3XMetaHandler metaHandler, TQ3XMetaHandler virtualMetaHandler, TQ3Uns32 methodsSize, TQ3Uns32 instanceSize)
{	TQ3Status			qd3dStatus;
	E3ClassInfoPtr		theClass;
#pragma unused(virtualMetaHandler)
#pragma unused(methodsSize)



	// Make sure this class hasn't been registered
	if (E3ClassTree_GetClassByName(objectName) != NULL)
		return(NULL);



	// If parentType is kQ3ObjectTypeInvalid, use the root object as the parent
	if (parentType == kQ3ObjectTypeInvalid)
		parentType = kQ3ObjectTypeRoot;



	// Allocate a unique type for this class
	*objectType = E3ClassTree_GetNextClassType();



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(parentType,
											*objectType,
											objectName,
											metaHandler,
											instanceSize);
	if (qd3dStatus != kQ3Success)
		return(NULL);



	// Find the class
	theClass = E3ClassTree_GetClassByType(*objectType);

	return((TQ3XObjectClass) theClass);
}





//=============================================================================
//      E3XObjectHierarchy_UnregisterClass : Unregister a class.
//-----------------------------------------------------------------------------
//		Note :	We're not required to unregister the class, and so we allow
//				the call to fail if there are still instances of the object.
//-----------------------------------------------------------------------------
TQ3Status
E3XObjectHierarchy_UnregisterClass(TQ3XObjectClass objectClass)
{	E3ClassInfoPtr		theClass = (E3ClassInfoPtr) objectClass;
	TQ3Status			qd3dStatus;
	TQ3ObjectType		classType;



	// Get the type of the class, and try and unregister it
	classType  = E3ClassTree_GetType(theClass);
	qd3dStatus = E3ClassTree_UnregisterClass(classType, kQ3False);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3XObjectHierarchy_NewObject : Instantiate a new object.
//-----------------------------------------------------------------------------
//		Note :	By passing true for the sharedParams parameter, we make sure
//				that parent objects of the class are instantiaed with the same
//				parameter data as the leaf class.
//
//				This is the same behaviour as QD3D, although it's different
//				from the normal Quesa scheme.
//-----------------------------------------------------------------------------
TQ3Object
E3XObjectHierarchy_NewObject(TQ3XObjectClass objectClass, void *parameters)
{	E3ClassInfoPtr		theClass = (E3ClassInfoPtr) objectClass;
	TQ3ObjectType		classType;
	TQ3Object			theObject;



	// Get the type of the class, and instantiate it
	classType = E3ClassTree_GetType(theClass);
	theObject = E3ClassTree_CreateInstance(classType, kQ3True, parameters);
	
	return(theObject);
}





//=============================================================================
//      E3XObjectHierarchy_GetClassVersion : Get the version of a class.
//-----------------------------------------------------------------------------
TQ3Status
E3XObjectHierarchy_GetClassVersion(TQ3ObjectType objectClassType, TQ3XObjectClassVersion *version)
{	E3ClassInfoPtr		theClass;



	// Initialise a return value
	*version = 0;



	// Find the class
	theClass = E3ClassTree_GetClassByType(objectClassType);
	if (theClass == NULL)
		return(kQ3Failure);



	// Get the version for the class
	*version = (TQ3XObjectClassVersion)
					E3ClassTree_GetMethod(theClass, kQ3XMethodTypeObjectClassVersion);

	return(kQ3Success);
}





//=============================================================================
//      E3XObjectHierarchy_FindClassByType : Find a class.
//-----------------------------------------------------------------------------
TQ3XObjectClass
E3XObjectHierarchy_FindClassByType(TQ3ObjectType theType)
{	E3ClassInfoPtr		theClass;



	// Find the class
	theClass = E3ClassTree_GetClassByType(theType);

	return((TQ3XObjectClass) theClass);
}





//=============================================================================
//      E3XObjectClass_GetMethod : Find a method for a class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3XFunctionPointer
E3XObjectClass_GetMethod(TQ3XObjectClass objectClass, TQ3XMethodType methodType)
{	E3ClassInfoPtr			theClass = (E3ClassInfoPtr) objectClass;
	TQ3XFunctionPointer		theMethod;



	// Get the method for the class
	theMethod = E3ClassTree_GetMethod(theClass, methodType);

	return(theMethod);
}





//=============================================================================
//      E3XObjectClass_GetLeafType : Get the leaf type for a class.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3XObjectClass_GetLeafType(TQ3XObjectClass objectClass)
{	E3ClassInfoPtr		theClass = (E3ClassInfoPtr) objectClass;
	TQ3ObjectType		classType;



	// Get the type of the class
	classType  = E3ClassTree_GetType(theClass);
	
	return(classType);
}





//=============================================================================
//      E3XObjectClass_GetType : Get the type for a class.
//-----------------------------------------------------------------------------
//		Note :	It's not clear how the type of a class differs from the leaf
//				type of the class (since, by definition, a class is a leaf
//				unto itself - if the class has children, which type should
//				be returned?), so we just return the type of the class.
//-----------------------------------------------------------------------------
TQ3Status
E3XObjectClass_GetType(TQ3XObjectClass objectClass, TQ3ObjectType *theType)
{	E3ClassInfoPtr		theClass = (E3ClassInfoPtr) objectClass;



	// Get the type of the class
	*theType  = E3ClassTree_GetType(theClass);
	
	return(kQ3Success);
}





//=============================================================================
//      E3XObjectClass_GetPrivate : Get the instance data for an object.
//-----------------------------------------------------------------------------
void *
E3XObjectClass_GetPrivate(TQ3XObjectClass objectClass, TQ3Object targetObject)
{


	// Return the instance data
	return(targetObject->instanceData);
}





//=============================================================================
//      E3XObject_GetClass : Get the class for an object.
//-----------------------------------------------------------------------------
TQ3XObjectClass
E3XObject_GetClass(TQ3Object object)
{


	// Return the class
	return((TQ3XObjectClass) object->theClass);
}





//=============================================================================
//      E3XSharedLibrary_Register : Register a shared library.
//-----------------------------------------------------------------------------
//		Note :	Called during E3System_LoadPlugins, as plug-ins register their
//				shared library details with Quesa.
//
//				We simply save the library details in the global state, since
//				E3System_LoadPlugins may need to impose some order on the
//				actual plug-in registration.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3XSharedLibrary_Register(TQ3XSharedLibraryInfo *sharedLibraryInfo)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();
	TQ3Status		qd3dStatus;



	// Try and allocate some space to save the library info
	qd3dStatus = Q3Memory_Reallocate(&theGlobals->sharedLibraryInfo,
									 sizeof(TQ3XSharedLibraryInfo) *
									 	(theGlobals->sharedLibraryCount+1));
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Save the library info
	theGlobals->sharedLibraryInfo[theGlobals->sharedLibraryCount] = *sharedLibraryInfo;
	theGlobals->sharedLibraryCount++;

	return(kQ3Success);
}





//=============================================================================
//      E3XSharedLibrary_Unregister : Unregister a shared library.
//-----------------------------------------------------------------------------
//		Note : Does nothing at the moment.
//-----------------------------------------------------------------------------
TQ3Status
E3XSharedLibrary_Unregister(TQ3Uns32 sharedLibrary)
{


	// Do nothing for now
	return(kQ3Success);
}





//=============================================================================
//      E3XError_Post : Post an error.
//-----------------------------------------------------------------------------
void
E3XError_Post(TQ3Error theError)
{	TQ3Boolean		isFatal;



	// Post the error
	isFatal = (TQ3Boolean) (theError == kQ3ErrorInternalError || theError == kQ3ErrorNoRecovery);
	E3ErrorManager_PostError(theError, isFatal);
}





//=============================================================================
//      E3XWarning_Post : Post a warning.
//-----------------------------------------------------------------------------
void
E3XWarning_Post(TQ3Warning theWarning)
{


	// Post the warning
	E3ErrorManager_PostWarning(theWarning);
}





//=============================================================================
//      E3XNotice_Post : Post a notice.
//-----------------------------------------------------------------------------
void
E3XNotice_Post(TQ3Notice theNotice)
{


	// Post the notice
	E3ErrorManager_PostNotice(theNotice);
}

/*  NAME:
        E3ClassTree.c

    DESCRIPTION:
        Quesa class tree.
        
        Quesa maintains an class tree, which identifies the various classes
        which are registered with the system.
        
        A class contains all the non-instance specific data for a Quesa
        object. A Quesa object contains a pointer to the relevent node in the
        class tree, along with the instance-specific data for that object.
        
        The class tree is populated with the default Quesa objects when Quesa
        initialises itself, and applications can register additional classes
        such as renderers/etc.
        
        The class tree is stored as a hash table for efficiency, but nodes
        within the tree maintain pointers to their children/parent in order
        to record their relationship to the rest of the tree.

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
#include "E3ClassTree.h"
#include "E3HashTable.h"

#include <time.h>
#include <stdio.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kClassHashTableSize							64
#define kMethodHashTableSize						20





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// A single node within the class tree
typedef struct E3ClassInfo {
	// Class
	TQ3ObjectType		classType;
	char				*className;
	TQ3XMetaHandler		classMetaHandler;
	E3HashTablePtr		methodTable;

	// Instances
	TQ3Uns32			numInstances;
	TQ3Uns32			instanceSize;	


	// Parent/children
	TQ3Uns32			numChildren;
	E3ClassInfoPtr		theParent;
	E3ClassInfoPtr		*theChildren;
} E3ClassInfo;





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Called through macro to avoid superfluous function call in release builds
#if Q3_DEBUG
	#define Q3_CLASS_VERIFY(_o)			e3class_verify(_o)
#else
	#define Q3_CLASS_VERIFY(_o)			
#endif





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3class_verify : Verify the instance data hasn't been corrupted.
//-----------------------------------------------------------------------------
//		Note :	Used for debug builds, to verify object instance data doesn't
//				write beyond the space allocated for it.
//-----------------------------------------------------------------------------
#if Q3_DEBUG

static void
e3class_verify(TQ3Object theObject)
{	TQ3ObjectType		*instanceTrailer;



	// Verify the object
	Q3_ASSERT(theObject->quesaTag == kQ3ObjectTypeQuesa);
	Q3_ASSERT_VALID_PTR(theObject->theClass);

	if (theObject->theClass->instanceSize == 0)
		Q3_ASSERT(theObject->instanceData == NULL);
	else
		{
		Q3_ASSERT_VALID_PTR(theObject->instanceData);
		instanceTrailer = (TQ3ObjectType *) (((TQ3Uns8 *) theObject->instanceData) +
											 theObject->theClass->instanceSize);
		
		Q3_ASSERT(*instanceTrailer == kQ3ObjectTypeQuesa);
		}

	if (theObject->theClass->classType != kQ3ObjectTypeRoot)
		Q3_ASSERT_VALID_PTR(theObject->parentObject);
}

#endif // Q3_DEBUG





//=============================================================================
//      e3class_attach : Attach a child node to its parent.
//-----------------------------------------------------------------------------
//		Note :	We assume the child currently has no parent.
//-----------------------------------------------------------------------------
static TQ3Status
e3class_attach(E3ClassInfoPtr theChild, E3ClassInfoPtr theParent)
{	TQ3Status		qd3dStatus;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theChild),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theParent),     kQ3Failure);
	Q3_REQUIRE_OR_RESULT(theChild->theParent == NULL, kQ3Failure);



	// Grow the list of child pointers
	qd3dStatus = Q3Memory_Reallocate(&theParent->theChildren,
									 sizeof(E3ClassInfoPtr) * (theParent->numChildren+1));
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Connect the child to the parent and the parent to the child
	theParent->theChildren[theParent->numChildren] = theChild;
	theParent->numChildren++;
	
	theChild->theParent = theParent;
	
	return(kQ3Success);
}





//=============================================================================
//      e3class_detach : Detach a node from its parent.
//-----------------------------------------------------------------------------
static void
e3class_detach(E3ClassInfoPtr theChild)
{	TQ3Status		qd3dStatus;
	E3ClassInfoPtr	theParent;
	TQ3Uns32		n;
	


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theChild));
	Q3_REQUIRE(Q3_VALID_PTR(theChild->theParent));



	// Get the parent
	theParent = theChild->theParent;



	// Find the slot occupied by theChild in its parent's child table,
	// and shift everything above that slot down by one (overwriting
	// thepointer to theChild, and leaving a bogus entry at the end).
	for (n = 0; n < theParent->numChildren; n++)
		{
		// If this child matches, shift everything else down
		if (theParent->theChildren[n] == theChild)
			{
			// If there's anything above us, copy it down
			if (n != (theParent->numChildren-1))
				Q3Memory_Copy(&theParent->theChildren[n+1],
					   		  &theParent->theChildren[n],
					   		  sizeof(E3ClassInfoPtr) * (theParent->numChildren-n-1));


			// We're done
			break;
			}
		}



	// Shrink the parent's list of children
	theParent->numChildren--;
	qd3dStatus = Q3Memory_Reallocate(&theParent->theChildren,
									 sizeof(E3ClassInfoPtr) * theParent->numChildren);
	Q3_ASSERT(qd3dStatus == kQ3Success);



	// Remove the parent from the child
	theChild->theParent = NULL;	
}





//=============================================================================
//      e3class_find_by_name : Recursive search for a class given a name.
//-----------------------------------------------------------------------------
static E3ClassInfoPtr
e3class_find_by_name(E3ClassInfoPtr theClass, const char *className)
{	E3ClassInfoPtr		theChild, theResult;
	TQ3Uns32			n;
	


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClass),                     NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(className),                    NULL);
	Q3_REQUIRE_OR_RESULT(strlen(className) < kQ3StringMaximumLength, NULL);



	// Check this node
	if (E3CString_IsEqual(theClass->className, className))
		return(theClass);



	// Check the children of the class
	for (n = 0; n < theClass->numChildren; n++)
		{
		// Get the child
		theChild = theClass->theChildren[n];
		Q3_ASSERT_VALID_PTR(theChild);


		// Check it
		theResult = e3class_find_by_name(theChild, className);
		if (theResult != NULL)
			return(theResult);
		}
	
	return(NULL);
}





//=============================================================================
//      e3class_find_method : Find a method for a class.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3class_find_method(E3ClassInfoPtr theClass, TQ3XMethodType methodType, TQ3Boolean canInherit)
{	TQ3XFunctionPointer		theMethod;
	TQ3Boolean				areDone;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClass), NULL);



	// Walk up the class tree until we find the method
	areDone   = kQ3False;
	theMethod = NULL;
	do
		{
		// Check the current class
		if (theClass->classMetaHandler != NULL)
			theMethod = theClass->classMetaHandler(methodType);



		// If this class doesn't implement it, and we can inherit, try the parent
		if (theMethod == NULL && theClass->theParent != NULL && canInherit)
			theClass = theClass->theParent;


		// Otherwise, we're done
		else
			areDone = kQ3True;
		}
	while (!areDone);



	// Return whatever we found
	return(theMethod);
}





//=============================================================================
//      e3class_dump_class : Dump some stats on a class.
//-----------------------------------------------------------------------------
//		Note :	Debug only - dumps some stats on a class to a file, and calls
//				itself recursively to dump the children of the class.
//-----------------------------------------------------------------------------
static void
e3class_dump_class(FILE *theFile, TQ3Uns32 indent, E3ClassInfoPtr theClass)
{	E3GlobalsPtr		theGlobals = E3Globals_Get();
	char				thePad[100];
	E3ClassInfoPtr		theChild;
	TQ3Uns32			n;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theFile);
	Q3_ASSERT_VALID_PTR(theClass);
	Q3_ASSERT(indent < (sizeof(thePad)-1));



	// Prepare the pad string
	for (n = 0; n < indent; n++)
		thePad[n] = ' ';
	
	thePad[n] = 0x00;



	// Dump the class
	fprintf(theFile, "\n%s%s%s\n",
					thePad,
					theClass->className,
					theClass->numInstances == 0 ? "" : " *** MEMORY LEAK ***");

	if ( (theClass->classType < 0) && (theClass->classType >= theGlobals->nextType) )
		fprintf(theFile, "%s-> classType    = 0x%lx\n", thePad, theClass->classType);
	else
		fprintf(theFile, "%s-> classType    = %c%c%c%c\n", thePad,
						((char *) &theClass->classType)[0],
						((char *) &theClass->classType)[1],
						((char *) &theClass->classType)[2],
						((char *) &theClass->classType)[3]);
	
	fprintf(theFile, "%s-> numInstances = %lu\n", thePad, theClass->numInstances);

	fprintf(theFile, "%s-> instanceSize = %lu\n", thePad, theClass->instanceSize);

	fprintf(theFile, "%s-> numChildren  = %lu\n", thePad, theClass->numChildren);
	
	if (E3HashTable_GetNumItems(theClass->methodTable) == 0)
		fprintf(theFile, "%s-> method cache is empty\n", thePad);
	else
		{
		fprintf(theFile, "%s-> method cache, collision max = %lu\n", thePad,
							E3HashTable_GetCollisionMax(theClass->methodTable));

		fprintf(theFile, "%s-> method cache, collision avg = %.2f\n", thePad,
							E3HashTable_GetCollisionAverage(theClass->methodTable));

		fprintf(theFile, "%s-> method cache, num items     = %lu\n", thePad,
							E3HashTable_GetNumItems(theClass->methodTable));

		fprintf(theFile, "%s-> method cache, table size    = %lu\n", thePad,
							E3HashTable_GetTableSize(theClass->methodTable));
		}



	// Dump the children of the class
	for (n = 0; n < theClass->numChildren; n++)
		{
		// Get the child
		theChild = theClass->theChildren[n];
		Q3_ASSERT_VALID_PTR(theChild);


		// Dump it
		e3class_dump_class(theFile, indent + 2, theChild);
		}

}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3ClassTree_Destroy : Destroy the class tree.
//-----------------------------------------------------------------------------
#pragma mark -
void
E3ClassTree_Destroy(void)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();



	// If we have a class tree, destroy it
	if (theGlobals->classTree != NULL)
		{
		E3HashTable_Destroy(&theGlobals->classTree);
		theGlobals->classTreeRoot = NULL;
		}
}





//=============================================================================
//      E3ClassTree_GetNextClassType : Get the next available class type.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3ClassTree_GetNextClassType(void)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();



	// Decrement the class type, and return the next available type
	--theGlobals->nextType;
	
	return(theGlobals->nextType);
}





//=============================================================================
//      E3ClassTree_RegisterClass : Register a new class.
//-----------------------------------------------------------------------------
//		Note :	Attempts to register a new class within the class tree.
//
//				If parentClass is kQ3ObjectTypeInvalid, the new node is created
//				at the root of the class tree. This can only be performed once,
//				and it is an error to attempt to create two nodes at the root.
//-----------------------------------------------------------------------------
TQ3Status
E3ClassTree_RegisterClass(TQ3ObjectType			parentClassType,
							TQ3ObjectType		classType,
							const char			*className,
							TQ3XMetaHandler		classMetaHandler,
							TQ3Uns32			instanceSize)
{	E3GlobalsPtr		theGlobals = E3Globals_Get();
	TQ3Status			qd3dStatus = kQ3Success;
	E3ClassInfoPtr		parentClass, theClass;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(className),                    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(strlen(className) < kQ3StringMaximumLength, kQ3Failure);

	if (parentClassType == kQ3ObjectTypeInvalid)
		Q3_REQUIRE_OR_RESULT(theGlobals->classTree == NULL, kQ3Failure);



	// Make sure the class isn't registered yet
	if (E3ClassTree_GetClassByType(classType) != NULL)
		return(kQ3Failure);



	// Find the parent class
	parentClass = NULL;
	if (parentClassType != kQ3ObjectTypeInvalid)
		{
		parentClass = E3ClassTree_GetClassByType(parentClassType);
		if (parentClass == NULL)
			return(kQ3Failure);
		}



	// Allocate the new class
	theClass = (E3ClassInfoPtr) Q3Memory_AllocateClear(sizeof(E3ClassInfo));
	if (theClass == NULL)
		return(kQ3Failure);

	theClass->className   = (char *) Q3Memory_Allocate(strlen(className) + 1);
	theClass->methodTable = E3HashTable_Create(kMethodHashTableSize);

	if (theClass->className == NULL || theClass->methodTable == NULL)
		{
		if (theClass->className != NULL)
			Q3Memory_Free(&theClass->className);
		
		if (theClass->methodTable != NULL)
			E3HashTable_Destroy(&theClass->methodTable);

		Q3Memory_Free(&theClass);
		return(kQ3Failure);
		}



	// Initialise the class
	theClass->classType        = classType;
	theClass->classMetaHandler = classMetaHandler;
	theClass->instanceSize     = instanceSize;
	strcpy(theClass->className, className);



	// If we don't have a hash table yet, create it
	if (theGlobals->classTree == NULL)
		{
		theGlobals->classTreeRoot = theClass;
		theGlobals->classTree     = E3HashTable_Create(kClassHashTableSize);
		if (theGlobals->classTree == NULL)
			qd3dStatus = kQ3Failure;
		}



	// Store the class in the hash table
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3HashTable_Add(theGlobals->classTree, classType, theClass);

	if (qd3dStatus == kQ3Success && parentClass != NULL)
		qd3dStatus = e3class_attach(theClass, parentClass);



	// Handle failure
	if (qd3dStatus != kQ3Success)
		{
		// Clean up the class tree
		if (theGlobals->classTree != NULL)
			{
			if (E3HashTable_Find(theGlobals->classTree, classType) != NULL)
				E3HashTable_Remove(theGlobals->classTree, classType);
			}
			
		if (theGlobals->classTreeRoot == theClass)
				theGlobals->classTreeRoot = NULL;


		// Clean up the class
		Q3Memory_Free(&theClass->className);
		E3HashTable_Destroy(&theClass->methodTable);
		Q3Memory_Free(&theClass);
		}

	return(qd3dStatus);
}





//=============================================================================
//      E3ClassTree_UnregisterClass : Unregister a class.
//-----------------------------------------------------------------------------
//		Note :	Any child classes derived from this class will also be removed
//				from the class tree.
//
//				If isRequired is true, we always unregister the class.
//				Otherwise we check to see if this class has any instances left.
//				If it does, those instances will have pointers back to the
//				class and so it ca't be unregistered.
//-----------------------------------------------------------------------------
TQ3Status
E3ClassTree_UnregisterClass(TQ3ObjectType classType, TQ3Boolean isRequired)
{	E3GlobalsPtr		theGlobals = E3Globals_Get();
	E3ClassInfoPtr		theClass, theChild;
	TQ3Status			qd3dStatus;



	// Find the class to unregister
	theClass = E3ClassTree_GetClassByType(classType);
	if (theClass == NULL)
		return(kQ3Failure);



	// If we're not required to unregister the class, check the instance count
	if (!isRequired)
		{
		// Make sure the class doesn't have any instances left. Instances have
		// a pointer back to the class tree, so we can't remove the class from
		// the tree while those pointers still exist.
		if (theClass->numInstances != 0)
			{
			E3ErrorManager_PostError(kQ3ErrorObjectClassInUse, kQ3False);
			return(kQ3Failure);
			}
		}



	// Unregister any children of the class
	while (theClass->numChildren != 0)
		{
		// Get the first child
		theChild = theClass->theChildren[0];
		Q3_ASSERT_VALID_PTR(theChild);


		// Try and unregister it
		qd3dStatus = E3ClassTree_UnregisterClass(theChild->classType, isRequired);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);
		}



	// Remove the class from the tree
	if (theClass->theParent != NULL)
		e3class_detach(theClass);
	
	if (theGlobals->classTreeRoot == theClass)
		theGlobals->classTreeRoot = NULL;

	E3HashTable_Remove(theGlobals->classTree, classType);



	// Dispose of the class itself
	Q3_ASSERT(theClass->numChildren == 0);
	Q3_ASSERT(theClass->theChildren == NULL);

	Q3Memory_Free(&theClass->className);
	E3HashTable_Destroy(&theClass->methodTable);
	Q3Memory_Free(&theClass);
	
	return(kQ3Success);
}





//=============================================================================
//      E3ClassTree_CreateInstance : Create an instance of a class.
//-----------------------------------------------------------------------------
//		Note :	Creates instances of the specified class and its parent
//				classes, passing paramData to the new method.
//
//				If sharedParams is true, the paramData parameter is passed to
//				the specified class and its parent classes.
//
//				Otherwise, NULL is passed as the paramData for any classes
//				above the parent.
//
//				The sharedParams behaviour is necessary to support the QD3D
//				call Q3XObjectHierarchy_NewObject.
//-----------------------------------------------------------------------------
TQ3Object
E3ClassTree_CreateInstance(TQ3ObjectType	classType,
							TQ3Boolean		sharedParams,
							const void		*paramData)
{	TQ3ObjectType				parentType, *instanceTrailer;
	TQ3XElementCopyAddMethod	copyAddMethod;
	TQ3Status					qd3dStatus;
	TQ3XObjectNewMethod			newMethod;
	TQ3Object					theObject;
	E3ClassInfoPtr				theClass;
	


	// Find the class to instantiate
	//
	// Instantiating objects is often the first thing to fail if the library
	// has not been initialised yet, so we also check for that case here.
	theClass = E3ClassTree_GetClassByType(classType);
	if (theClass == NULL)
		{
		E3ErrorManager_PostWarning(kQ3WarningTypeHasNotBeenRegistered);

		if (!Q3IsInitialized())
			E3ErrorManager_PostError(kQ3ErrorNotInitialized, kQ3False);

		return(NULL);
		}



	// Allocate and initialise the object
	theObject = (TQ3Object) Q3Memory_AllocateClear(sizeof(OpaqueTQ3Object));
	if (theObject == NULL)
		return(NULL);

	theObject->quesaTag = kQ3ObjectTypeQuesa;
	theObject->theClass = theClass;



	// If this class has a parent, instantiate the parent object
	if (theClass->theParent != NULL)
		{
		parentType = E3ClassTree_GetType(theClass->theParent);

		if (sharedParams)
			theObject->parentObject = E3ClassTree_CreateInstance(parentType, sharedParams, paramData);
		else
			theObject->parentObject = E3ClassTree_CreateInstance(parentType, sharedParams, NULL);
			
		if (theObject->parentObject == NULL)
			{
			Q3Memory_Free(&theObject);
			return(NULL);
			}
		
		#if Q3_DEBUG
		theObject->parentObject->childObject = theObject;
		#endif
		}



	// If this class has any private data, allocate and initialise it
	if (theClass->instanceSize != 0)
		{
		// Allocate the private data
		theObject->instanceData = Q3Memory_AllocateClear(theClass->instanceSize + sizeof(TQ3ObjectType));
		if (theObject->instanceData == NULL)
			{
			if (theObject->parentObject != NULL)
				E3ClassTree_DestroyInstance(theObject->parentObject);

			Q3Memory_Free(&theObject);
			return(NULL);
			}



		// Initialise the trailer
		instanceTrailer = (TQ3ObjectType *) (((TQ3Uns8 *) theObject->instanceData) +
											 theObject->theClass->instanceSize);
		*instanceTrailer = kQ3ObjectTypeQuesa;



		// If the object has a new method, call it to initialise the object
		newMethod = (TQ3XObjectNewMethod) e3class_find_method(theClass,
															  kQ3XMethodTypeObjectNew,
															  kQ3False);
		if (newMethod != NULL)
			{
			qd3dStatus = newMethod(theObject, theObject->instanceData, (void *) paramData);
			if (qd3dStatus != kQ3Success)
				{
				if (theObject->parentObject != NULL)
					E3ClassTree_DestroyInstance(theObject->parentObject);

				Q3Memory_Free(&theObject->instanceData);
				Q3Memory_Free(&theObject);
				return(NULL);
				}
			}


		else 
			{
			// If the object is an element, it might have a copy add method
			// which we call to initialise the object.
			copyAddMethod = (TQ3XElementCopyAddMethod) e3class_find_method(theClass,
																  kQ3XMethodTypeElementCopyAdd,
																  kQ3False);
			if (copyAddMethod != NULL)
				{
				qd3dStatus = copyAddMethod(paramData, theObject->instanceData);
				if (qd3dStatus != kQ3Success)
					{
					if (theObject->parentObject != NULL)
						E3ClassTree_DestroyInstance(theObject->parentObject);

					Q3Memory_Free(&theObject->instanceData);
					Q3Memory_Free(&theObject);
					return(NULL);
					}
				}

			// Otherwise if there was no new method, but there was parameter data, do a
			// bitwise copy. Classes which require more advanced initialisation must supply
			// a new method, and classes which don't have any parameter data will be left
			// with instance data that's initialised to 0s.
			else if (paramData != NULL)
				Q3Memory_Copy(paramData, theObject->instanceData, theClass->instanceSize);
			}
		}



	// Increment the instance count of the class (watch for overflow)
	theClass->numInstances++;
	Q3_ASSERT(theClass->numInstances > 0);



	// Verify the object and return it
	Q3_CLASS_VERIFY(theObject);

	return(theObject);
}





//=============================================================================
//      E3ClassTree_DestroyInstance : Destroy an instance of a class.
//-----------------------------------------------------------------------------
void
E3ClassTree_DestroyInstance(TQ3Object theObject)
{	TQ3XElementDeleteMethod 	elementDeleteMethod;
	TQ3XObjectDeleteMethod		deleteMethod;



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theObject));
	Q3_CLASS_VERIFY(theObject);



	// Call the object's delete method
	elementDeleteMethod = (TQ3XElementDeleteMethod) e3class_find_method(theObject->theClass,
																		kQ3XMethodTypeElementDelete,
																		kQ3False);
	if (elementDeleteMethod != NULL) 
		elementDeleteMethod(theObject->instanceData);		
	else
		{
		deleteMethod = (TQ3XObjectDeleteMethod) e3class_find_method(theObject->theClass,
																	kQ3XMethodTypeObjectDelete,
																	kQ3False);
		if (deleteMethod != NULL)
			deleteMethod(theObject, theObject->instanceData);
		}



	// Dispose of the parent object, if any
	if (theObject->parentObject != NULL)
		E3ClassTree_DestroyInstance(theObject->parentObject);



	// Decrement the instance count of the class
	Q3_ASSERT(theObject->theClass->numInstances > 0);
	theObject->theClass->numInstances--;



	// Dispose of the object		
	Q3Memory_Free(&theObject->instanceData);
	Q3Memory_Free(&theObject);
}





//=============================================================================
//      E3ClassTree_DuplicateInstance : Duplicate an instance of a class.
//-----------------------------------------------------------------------------
TQ3Object
E3ClassTree_DuplicateInstance(TQ3Object theObject)
{	TQ3XElementCopyDuplicateMethod	elementDuplicateMethod;
	TQ3ObjectType					*instanceTrailer;
	TQ3XObjectDuplicateMethod		duplicateMethod;
	TQ3Status						qd3dStatus;
	TQ3Object						newObject;
	E3ClassInfoPtr					theClass;



	// Verify our parameters
	Q3_CLASS_VERIFY(theObject);



	// Get the class we're instantiating
	theClass = theObject->theClass;
	Q3_ASSERT_VALID_PTR(theClass);



	// Allocate and initialise the object
	newObject = (TQ3Object) Q3Memory_AllocateClear(sizeof(OpaqueTQ3Object));
	if (newObject == NULL)
		return(NULL);

	newObject->quesaTag = kQ3ObjectTypeQuesa;
	newObject->theClass = theClass;



	// If the object has a parent, duplicate the parent object
	if (theObject->parentObject != NULL)
		{
		newObject->parentObject = E3ClassTree_DuplicateInstance(theObject->parentObject);
		if (newObject->parentObject == NULL)
			{
			Q3Memory_Free(&newObject);
			return(NULL);
			}
		
		#if Q3_DEBUG
		newObject->parentObject->childObject = newObject;
		#endif
		}



	// If the object has any private data, allocate and duplicate it
	if (theObject->instanceData != NULL)
		{
		// Allocate the private data
		newObject->instanceData = Q3Memory_AllocateClear(theClass->instanceSize + sizeof(TQ3ObjectType));
		if (newObject->instanceData == NULL)
			{
			if (newObject->parentObject != NULL)
				Q3Object_Dispose(newObject->parentObject);

			Q3Memory_Free(&newObject);
			return(NULL);
			}



		// Initialise the trailer
		instanceTrailer = (TQ3ObjectType *) (((TQ3Uns8 *) newObject->instanceData) +
											 newObject->theClass->instanceSize);
		*instanceTrailer = kQ3ObjectTypeQuesa;



		// Call the object's duplicate method to initialise it. If the object
		// does not have duplicate method, we do a bitwise copy.
		duplicateMethod = (TQ3XObjectDuplicateMethod) e3class_find_method(theClass,
																		  kQ3XMethodTypeObjectDuplicate,
																		  kQ3False);
		if (duplicateMethod == NULL)
			{
			elementDuplicateMethod = (TQ3XElementCopyDuplicateMethod) e3class_find_method(theObject->theClass,
																		kQ3XMethodTypeElementCopyDuplicate,
																		kQ3False);
			if (elementDuplicateMethod != NULL)
				{
				qd3dStatus = elementDuplicateMethod(theObject->instanceData, newObject->instanceData);
				if (qd3dStatus != kQ3Success)
					{
					if (newObject->parentObject != NULL)
						Q3Object_Dispose(newObject->parentObject);

					Q3Memory_Free(&newObject->instanceData);
					Q3Memory_Free(&newObject);
					return(NULL);
					}
				}
			else
				Q3Memory_Copy(theObject->instanceData, newObject->instanceData, theClass->instanceSize);
			}	
		else
			{
			qd3dStatus = duplicateMethod(theObject, theObject->instanceData,
										 newObject, newObject->instanceData);
			if (qd3dStatus != kQ3Success)
				{
				if (newObject->parentObject != NULL)
					Q3Object_Dispose(newObject->parentObject);

				Q3Memory_Free(&newObject->instanceData);
				Q3Memory_Free(&newObject);
				return(NULL);
				}
			}
		}



	// Increment the instance count of the object's class
	theClass->numInstances++;
	Q3_ASSERT(theClass->numInstances > 0);



	// Verify the object and return it
	Q3_CLASS_VERIFY(newObject);

	return(newObject);
}





//=============================================================================
//      E3ClassTree_FindParentInstance : Find the parent object of an object.
//-----------------------------------------------------------------------------
//		Note :	Given an object, we walk upwards through its parent objects
//				until we find one with the appropriate class type.
//-----------------------------------------------------------------------------
TQ3Object
E3ClassTree_FindParentInstance(TQ3Object theObject, TQ3ObjectType classType)
{	TQ3Object		parentObject;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theObject), NULL);
	Q3_CLASS_VERIFY(theObject);



	// Check to see if this is object is what we're looking for
	if (theObject->theClass->classType == classType)
		return(theObject);



	// Walk up the parents of the object, looking for the correct one
	parentObject = theObject->parentObject;
	while (parentObject != NULL && parentObject->theClass->classType != classType)
		parentObject = parentObject->parentObject;
	
	return(parentObject);
}





//=============================================================================
//      E3ClassTree_FindInstanceData : Find the instance data of an object.
//-----------------------------------------------------------------------------
//		Note :	Given an object, we walk upwards through its parent objects
//				until we find one with the appropriate class type. We then
//				return the appropriate instance data.
//
//				Useful for objects which are inherited from, as this allows
//				them to pass in a possible sub-classed object and obtain their
//				instance data rather than the child class.
//-----------------------------------------------------------------------------
void *
E3ClassTree_FindInstanceData(TQ3Object theObject, TQ3ObjectType classType)
{	TQ3Object		parentObject;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theObject), NULL);
	Q3_CLASS_VERIFY(theObject);



	// Check to see if this is object is what we're looking for
	if (theObject->theClass->classType == classType)
		return(theObject->instanceData);



	// Walk up the parents of the object, looking for the correct one
	parentObject = theObject->parentObject;
	while (parentObject != NULL && parentObject->theClass->classType != classType)
		parentObject = parentObject->parentObject;

	if (parentObject != NULL)
		return(parentObject->instanceData);
	
	return(NULL);
}





//=============================================================================
//      E3ClassTree_GetObjectType : Get a method for a class.
//-----------------------------------------------------------------------------
//		Note :	Gets the type of the instance first child of the instance of
//				base type.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3ClassTree_GetObjectType(TQ3Object theObject, TQ3ObjectType baseType)
{	E3ClassInfoPtr		theClass;
	TQ3ObjectType		theType;



	// Verify our parameters
	Q3_CLASS_VERIFY(theObject);



	// Find the class of the object
	theClass = theObject->theClass;



	// Walk up to the level immediately below the base class
	while ( (theClass != NULL) && (theClass->theParent != NULL) &&
			(theClass->theParent->classType != baseType) )
		{
		Q3_ASSERT_VALID_PTR(theClass);
		theClass = theClass->theParent;
		}

	if ( (theClass == NULL) || (theClass->theParent == NULL) )
		theType = kQ3ObjectTypeInvalid;
	else
		theType = theClass->classType;



	// Return the appropriate type
	return theType;

}





//=============================================================================
//      E3ClassTree_GetClassByType : Find a node in the tree by type.
//-----------------------------------------------------------------------------
E3ClassInfoPtr
E3ClassTree_GetClassByType(TQ3ObjectType classType)
{	E3GlobalsPtr		theGlobals = E3Globals_Get();
	E3ClassInfoPtr		theClass;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(classType != kQ3ObjectTypeInvalid, NULL);



	// We can't find anything if we don't have a tree
	if (theGlobals->classTree == NULL)
		return(NULL);



	// Find the class
	theClass = (E3ClassInfoPtr) E3HashTable_Find(theGlobals->classTree, classType);
	
	return(theClass);
}





//=============================================================================
//      E3ClassTree_GetClassByName : Find a node in the tree by name.
//-----------------------------------------------------------------------------
//		Note :	NB - the class tree is implemented as a hash table, keyed off
//				the class type. This means that searches by name are very much
//				sub-optimal, and are implemented as a linear search.
//-----------------------------------------------------------------------------
E3ClassInfoPtr
E3ClassTree_GetClassByName(const char *className)
{	E3GlobalsPtr		theGlobals = E3Globals_Get();
	E3ClassInfoPtr		theClass;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(className),                    NULL);
	Q3_REQUIRE_OR_RESULT(strlen(className) < kQ3StringMaximumLength, NULL);



	// Make sure we have a root
	if (theGlobals->classTreeRoot == NULL)
		return(NULL);



	// Find the class, starting at the root
	theClass = e3class_find_by_name(theGlobals->classTreeRoot, className);

	return(theClass);
}





//=============================================================================
//      E3ClassTree_GetParent : Get the parent of a class, if any.
//-----------------------------------------------------------------------------
//		Note : The root node of the class tree has no parent.
//-----------------------------------------------------------------------------
E3ClassInfoPtr
E3ClassTree_GetParent(E3ClassInfoPtr theClass)
{


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClass), NULL);



	// Return the parent of the class
	return(theClass->theParent);
}





//=============================================================================
//      E3ClassTree_GetNumChildren : Get the number of children of a class.
//-----------------------------------------------------------------------------
TQ3Uns32
E3ClassTree_GetNumChildren(E3ClassInfoPtr theClass)
{


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClass), 0);



	// Return the number of children of the class
	return(theClass->numChildren);
}





//=============================================================================
//      E3ClassTree_GetChild : Get a child of a class.
//-----------------------------------------------------------------------------
//		Note : We assume the index of theChild is valid.
//-----------------------------------------------------------------------------
E3ClassInfoPtr
E3ClassTree_GetChild(E3ClassInfoPtr theClass, TQ3Uns32 childIndex)
{	E3ClassInfoPtr		theChild;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClass),                                NULL);
	Q3_REQUIRE_OR_RESULT(childIndex >= 0 && childIndex < theClass->numChildren, NULL);



	// Return the child of the class
	theChild = theClass->theChildren[childIndex];
	Q3_ASSERT_VALID_PTR(theChild);
	
	return(theChild);
}





//=============================================================================
//      E3ClassTree_GetType : Get the type of a class.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3ClassTree_GetType(E3ClassInfoPtr theClass)
{


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClass), kQ3ObjectTypeInvalid);



	// Return the type of the class
	return(theClass->classType);
}





//=============================================================================
//      E3ClassTree_IsType : Is a class an instance of a type?
//-----------------------------------------------------------------------------
TQ3Boolean
E3ClassTree_IsType(E3ClassInfoPtr theClass, TQ3ObjectType theType)
{


	// Walk up through the class and its parents, checking the type
	while (theClass != NULL)
		{
		// Check this object
		if (theClass->classType == theType)
			return(kQ3True);



		// Move up a level
		theClass = theClass->theParent;
		}



	// If we're still here, we didn't find the type
	return(kQ3False);
}





//=============================================================================
//      E3ClassTree_GetName : Get the name of a class.
//-----------------------------------------------------------------------------
//		Note : The pointer returned is read-only, and owned by the class tree.
//-----------------------------------------------------------------------------
const char *
E3ClassTree_GetName(E3ClassInfoPtr theClass)
{


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClass), NULL);



	// Return the name of the class
	return(theClass->className);
}





//=============================================================================
//      E3ClassTree_GetMetaHandler : Get the metahandler of a class.
//-----------------------------------------------------------------------------
TQ3XMetaHandler
E3ClassTree_GetMetaHandler(E3ClassInfoPtr theClass)
{


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClass), NULL);



	// Return the metahandler of the class
	return(theClass->classMetaHandler);
}





//=============================================================================
//      E3ClassTree_GetInstanceSize : Get the size of a class's instance data.
//-----------------------------------------------------------------------------
TQ3Uns32
E3ClassTree_GetInstanceSize(E3ClassInfoPtr theClass)
{


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClass), 0);



	// Return the size of the instance data for the class
	return(theClass->instanceSize);
}





//=============================================================================
//      E3ClassTree_GetNumInstances : Get number of instances of a class.
//-----------------------------------------------------------------------------
TQ3Uns32
E3ClassTree_GetNumInstances(E3ClassInfoPtr theClass)
{


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClass), 0);



	// Return the number of instances of the class
	return(theClass->numInstances);
}





//=============================================================================
//      E3ClassTree_GetMethod : Get a method for a class.
//-----------------------------------------------------------------------------
//		Note :	When looking for methods, we first check the method table for
//				the class. If this fails, we call the class metahandler.
//
//				When calling the metahandler, we inherit methods that the class
//				does't implement from the parent of the class.
//
//				If we find the method, we store it in the method table to cache
//				it for the next time.
//-----------------------------------------------------------------------------
TQ3XFunctionPointer
E3ClassTree_GetMethod(E3ClassInfoPtr theClass, TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClass), NULL);



	// Check the hash table
	theMethod = (TQ3XFunctionPointer) E3HashTable_Find(theClass->methodTable, methodType);
	if (theMethod != NULL)
		return(theMethod);



	// Find the method, allowing inheritence, and save it for later
	theMethod = e3class_find_method(theClass, methodType, kQ3True);
	if (theMethod != NULL)
		E3HashTable_Add(theClass->methodTable, methodType, theMethod);

	return(theMethod);
}





//=============================================================================
//      E3ClassTree_AddMethod : Add a method for a class.
//-----------------------------------------------------------------------------
//		Note :	In order to support renderers, we need to be able to add
//				methods to the class tree's cache - this is because these
//				objects go through a secondary metahandler, which needs to be
//				queried by the renderer class itself.
//-----------------------------------------------------------------------------
void
E3ClassTree_AddMethod(E3ClassInfoPtr theClass, TQ3XMethodType methodType, TQ3XFunctionPointer theMethod)
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theClass));
	Q3_REQUIRE(Q3_VALID_PTR(theMethod));



	// Add the method to the hash table for the class
	E3HashTable_Add(theClass->methodTable, methodType, theMethod);
}





//=============================================================================
//      E3ClassTree_AddMethodByType : Add a method for a class given its type.
//-----------------------------------------------------------------------------
void
E3ClassTree_AddMethodByType(TQ3ObjectType classType, TQ3XMethodType methodType, TQ3XFunctionPointer theMethod)
{	E3ClassInfoPtr theClass;



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theMethod));



	// Find the class, then add the method
	theClass = E3ClassTree_GetClassByType(classType);
	Q3_ASSERT( theClass != NULL );
	if (theClass != NULL)
		E3ClassTree_AddMethod(theClass, methodType, theMethod);
}





//=============================================================================
//      E3ClassTree_Dump : Dump some stats on the class tree.
//-----------------------------------------------------------------------------
//		Note :	Dumps some stats on the class tree to a file, for testing hash
//				table sizes/memory leaks/etc.
//-----------------------------------------------------------------------------
void
E3ClassTree_Dump(void)
{	E3GlobalsPtr		theGlobals = E3Globals_Get();
	FILE				*theFile;
	time_t				theTime;



	// Open our file
	theFile = fopen("Quesa class tree.dump", "wt");
	if (theFile == NULL)
		return;



	// Write out a header
	theTime = time(NULL);
	fprintf(theFile, "Dumping Quesa class tree - %s", ctime(&theTime));



	// Print some stats on the class tree
	fprintf(theFile, "class tree, collision max = %lu\n",
						E3HashTable_GetCollisionMax(theGlobals->classTree));

	fprintf(theFile, "class tree, collision avg = %.2f\n",
						E3HashTable_GetCollisionAverage(theGlobals->classTree));

	fprintf(theFile, "class tree, num items     = %lu\n",
						E3HashTable_GetNumItems(theGlobals->classTree));

	fprintf(theFile, "class tree, table size    = %lu\n",
						E3HashTable_GetTableSize(theGlobals->classTree));



	// Dump the class tree, starting at the root
	e3class_dump_class(theFile, 1, theGlobals->classTreeRoot);



	// Clean up
	fclose(theFile);
}

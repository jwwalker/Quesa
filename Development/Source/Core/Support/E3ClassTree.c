/*  NAME:
        E3ClassTree.c

    DESCRIPTION:
        Quesa class tree.
        
        Quesa maintains a class tree, which identifies the various classes
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
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
#define kMethodHashTableSize						32





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------







//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Called through macro to avoid superfluous function call in release builds
#if Q3_DEBUG
	#define Q3_CLASS_VERIFY(_o)			_o->Verify()
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

void
OpaqueTQ3Object::Verify ()
	{
	// Verify the object
	Q3_ASSERT ( quesaTag == kQ3ObjectTypeQuesa ) ;
	Q3_ASSERT_VALID_PTR ( theClass ) ;

	TQ3ObjectType* instanceTrailer = (TQ3ObjectType*) ( ( (TQ3Uns8 *) this ) + theClass->instanceSize ) ;
		
	Q3_ASSERT ( *instanceTrailer == kQ3ObjectTypeQuesa ) ;
	}

#endif // Q3_DEBUG





//=============================================================================
//      e3class_attach : Attach a child node to its parent.
//-----------------------------------------------------------------------------
//		Note :	We assume the child currently has no parent.
//-----------------------------------------------------------------------------
TQ3Status
E3ClassInfo::Attach ( E3ClassInfoPtr theChild, E3ClassInfoPtr theParent )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theChild),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theParent),     kQ3Failure);
	Q3_REQUIRE_OR_RESULT(theChild->theParent == NULL, kQ3Failure);



	// Grow the list of child pointers
	TQ3Status qd3dStatus = Q3Memory_Reallocate(&theParent->theChildren,
									 sizeof(E3ClassInfoPtr) * (theParent->numChildren+1));
	if ( qd3dStatus == kQ3Failure )
		return kQ3Failure ;



	// Connect the child to the parent and the parent to the child
	theParent->theChildren [ theParent->numChildren ] = theChild ;
	theParent->numChildren++ ;
	
	theChild->theParent = theParent ;
	
	return kQ3Success ;
	}





//=============================================================================
//      e3class_detach : Detach a node from its parent.
//-----------------------------------------------------------------------------
void
E3ClassInfo::Detach ( void )
	{
	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(this));
	Q3_REQUIRE(Q3_VALID_PTR(theParent));



	// and shift everything above that slot down by one (overwriting
	// thepointer to theChild, and leaving a bogus entry at the end).
	for ( TQ3Uns32 n = 0 ; n < theParent->numChildren ; ++n )
		{
		// If this child matches, shift everything else down
		if ( theParent->theChildren [ n ] == this )
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
	--theParent->numChildren ;
	TQ3Status qd3dStatus = Q3Memory_Reallocate(&theParent->theChildren,
									 sizeof(E3ClassInfoPtr) * theParent->numChildren);
	Q3_ASSERT(qd3dStatus == kQ3Success);



	// Remove the parent from the child
	theParent = NULL ;	
	}





//=============================================================================
//      e3class_find_by_name : Recursive search for a class given a name.
//-----------------------------------------------------------------------------
E3ClassInfoPtr
E3ClassInfo::Find ( const char *theClassName )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this),							NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theClassName),                    NULL);
	Q3_REQUIRE_OR_RESULT(strlen(theClassName) < kQ3StringMaximumLength, NULL);



	// Check this node
	if ( E3CString_IsEqual ( className, theClassName ) )
		return this ;



	// Check the children of the class
	for ( TQ3Uns32 n = 0 ; n < numChildren ; ++n )
		{
		// Get the child
		E3ClassInfoPtr theChild = theChildren [ n ] ;
		Q3_ASSERT_VALID_PTR(theChild);


		// Check it
		E3ClassInfoPtr theResult = theChild->Find ( theClassName ) ;
		if ( theResult != NULL )
			return theResult ;
		}
	
	return NULL ;
	}





//=============================================================================
//      e3class_find_method : Find a method for a class.
//-----------------------------------------------------------------------------
TQ3XFunctionPointer
E3ClassInfo::Find_Method ( TQ3XMethodType methodType, TQ3Boolean canInherit )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), NULL);



	// Walk up the class tree until we find the method
	
	if ( classMetaHandler != NULL )
		if ( TQ3XFunctionPointer leafMethod = classMetaHandler ( methodType ) )
			return leafMethod ;
	
	// If this class doesn't implement it, and we can inherit, try each of the parents in turn
	if ( canInherit )	
		for ( E3ClassInfoPtr theClass = theParent ; theClass != NULL ; theClass = theClass->theParent )
			if ( theClass->classMetaHandler != NULL ) // Check the current class
				if ( TQ3XFunctionPointer theMethod = theClass->classMetaHandler ( methodType ) )
					return theMethod ;



	// Did not find a suitable method, return NULL
	return NULL ;
	}





//=============================================================================
//      e3class_dump_class : Dump some stats on a class.
//-----------------------------------------------------------------------------
//		Note :	Debug only - dumps some stats on a class to a file, and calls
//				itself recursively to dump the children of the class.
//-----------------------------------------------------------------------------
void
E3ClassInfo::Dump_Class ( FILE *theFile, TQ3Uns32 indent )
	{
	char				thePad[100];
	TQ3Uns32			n;
	
	E3GlobalsPtr theGlobals = E3Globals_Get () ;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theFile);
	Q3_ASSERT_VALID_PTR(this);
	Q3_ASSERT(indent < (sizeof(thePad)-1));



	// Prepare the pad string
	for (n = 0; n < indent; ++n )
		thePad[n] = ' ';
	
	thePad[n] = 0x00;



	// Dump the class
	fprintf(theFile, "\n%s%s%s\n",
					thePad,
					className,
					numInstances == 0 ? "" : " *** MEMORY LEAK ***");

	if ( ( classType < 0) && ( classType >= theGlobals->classNextType) )
		fprintf(theFile, "%s-> classType    = 0x%lx\n", thePad, classType);
	else
		fprintf(theFile, "%s-> classType    = %c%c%c%c\n", thePad,
						((char *) &classType)[0],
						((char *) &classType)[1],
						((char *) &classType)[2],
						((char *) &classType)[3]);
	
	fprintf(theFile, "%s-> numInstances = %lu\n", thePad, numInstances);

	fprintf(theFile, "%s-> instanceSize = %lu\n", thePad, instanceSize);

	fprintf(theFile, "%s-> numChildren  = %lu\n", thePad, numChildren);
	
	if (E3HashTable_GetNumItems( methodTable) == 0)
		fprintf(theFile, "%s-> method cache is empty\n", thePad);
	else
		{
		fprintf(theFile, "%s-> method cache, collision max = %lu\n", thePad,
							E3HashTable_GetCollisionMax( methodTable));

		fprintf(theFile, "%s-> method cache, collision avg = %.2f\n", thePad,
							E3HashTable_GetCollisionAverage( methodTable));

		fprintf(theFile, "%s-> method cache, num items     = %lu\n", thePad,
							E3HashTable_GetNumItems( methodTable));

		fprintf(theFile, "%s-> method cache, table size    = %lu\n", thePad,
							E3HashTable_GetTableSize( methodTable));
		}



	// Dump the children of the class
	for (n = 0; n < numChildren; ++n )
		{
		// Get the child
		E3ClassInfoPtr theChild = theChildren[n];
		Q3_ASSERT_VALID_PTR(theChild);


		// Dump it
		theChild->Dump_Class ( theFile, indent + 2 ) ;
		}
	}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3ClassTree_Destroy : Destroy the class tree.
//-----------------------------------------------------------------------------
#pragma mark -

void
E3ClassTree::Destroy ( void )
	{
	E3GlobalsPtr theGlobals = E3Globals_Get () ;

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
E3ClassTree::GetNextClassType ( void )
	{
	E3GlobalsPtr theGlobals = E3Globals_Get () ;

	// Decrement the class type, and return the next available type
	theGlobals->classNextType--;
	
	return theGlobals->classNextType ;
	}







//=============================================================================
//      E3ClassTree_RegisterClass : Register a new class.
//-----------------------------------------------------------------------------
//		Note :	Attempts to register a new class within the class tree.
//
//				If parentClass is kQ3ObjectTypeInvalid, the new node is created
//				at the root of the class tree. This can only be performed once,
//				and it is an error to attempt to create two nodes at the root.
//
// Temporarily, instanceSize is replaced by instanceSizeAndFlag. For old style
// instance records which do not inherit from their base class, this is just the
// sizeof the record, for new style ones which do inherit, it is ~sizeof the
// record. Originally this was planned to be minus, but bitwise complement does
// affect zero wheras minus does not. When everything has been converted, the code
// for handling old style ones will be removed, but keeping the check on the
// parameter so the tildas in the calling code do not need to be removed immediately
// but when convenient when other work is done on each file.
//
//-----------------------------------------------------------------------------
TQ3Status
E3ClassTree::RegisterExternalClass (
								TQ3ObjectType		parentClassType,
								TQ3ObjectType		classType,
								const char			*className,
								TQ3XMetaHandler		classMetaHandler,
								TQ3Int32			leafInstanceSize )
	{
	E3ClassInfoPtr parentClass = E3ClassTree::GetClass ( parentClassType ) ;
	if ( parentClass == NULL )
		return kQ3Failure ;
		
	return E3ClassTree::RegisterClass ( parentClassType,
										classType,
										className,
										classMetaHandler,
										leafInstanceSize + parentClass->instanceSize ) ;
	}
	
	
TQ3Status
E3ClassTree::RegisterClass (	TQ3ObjectType		parentClassType,
								TQ3ObjectType		classType,
								const char			*className,
								TQ3XMetaHandler		classMetaHandler,
								TQ3Int32			instanceSizeAndRedundantFlag )
	{
	TQ3Uns32 instanceSize = instanceSizeAndRedundantFlag >= 0 ? instanceSizeAndRedundantFlag : ~instanceSizeAndRedundantFlag ;
	E3GlobalsPtr theGlobals = E3Globals_Get () ;
	TQ3Status qd3dStatus = kQ3Success ;

	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(className),                    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(strlen(className) < kQ3StringMaximumLength, kQ3Failure);

	if (parentClassType == kQ3ObjectTypeInvalid)
		Q3_REQUIRE_OR_RESULT(theGlobals->classTree == NULL, kQ3Failure);



	// Make sure the class isn't registered yet
	if ( E3ClassTree::GetClass ( classType ) != NULL )
		return kQ3Failure ;



	// Find the parent class
	E3ClassInfoPtr parentClass = NULL;
	if ( parentClassType != kQ3ObjectTypeInvalid )
		{
		parentClass = E3ClassTree::GetClass ( parentClassType ) ;
		if ( parentClass == NULL )
			return kQ3Failure ;
		Q3_ASSERT ( instanceSize >= parentClass->instanceSize ) ;
		}



	// Allocate the new class
	E3ClassInfoPtr theClass = (E3ClassInfoPtr) Q3Memory_AllocateClear ( sizeof ( E3ClassInfo ) ) ;
	if ( theClass == NULL )
		return kQ3Failure ;

	theClass->className   = (char *) Q3Memory_Allocate ( strlen ( className ) + 1 ) ;
	theClass->methodTable = E3HashTable_Create ( kMethodHashTableSize)  ;

	if ( theClass->className == NULL || theClass->methodTable == NULL )
		{
		if ( theClass->className != NULL )
			Q3Memory_Free ( & theClass->className ) ;
		
		if ( theClass->methodTable != NULL )
			E3HashTable_Destroy ( &theClass->methodTable ) ;

		Q3Memory_Free ( &theClass ) ;
		return kQ3Failure ;
		}



	// Initialise the class
	theClass->classType        = classType ;
	theClass->classMetaHandler = classMetaHandler ;
	theClass->instanceSize     = instanceSize ;

	strcpy ( theClass->className, className ) ;



	// If we don't have a hash table yet, create it
	if ( theGlobals->classTree == NULL )
		{
		theGlobals->classTreeRoot = theClass ;
		theGlobals->classTree     = E3HashTable_Create ( kClassHashTableSize ) ;
		if ( theGlobals->classTree == NULL )
			qd3dStatus = kQ3Failure ;
		}



	// Store the class in the hash table
	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3HashTable_Add ( theGlobals->classTree, classType, theClass ) ;

	if ( qd3dStatus != kQ3Failure && parentClass != NULL )
		qd3dStatus = E3ClassInfo::Attach ( theClass, parentClass ) ;



	// Handle failure
	if ( qd3dStatus == kQ3Failure )
		{
		// Clean up the class tree
		if ( theGlobals->classTree != NULL )
			{
			if ( E3HashTable_Find ( theGlobals->classTree, classType) != NULL )
				E3HashTable_Remove ( theGlobals->classTree, classType ) ;
			}
			
		if ( theGlobals->classTreeRoot == theClass )
				theGlobals->classTreeRoot = NULL ;


		// Clean up the class
		Q3Memory_Free ( & theClass->className ) ;
		E3HashTable_Destroy ( & theClass->methodTable ) ;
		Q3Memory_Free ( & theClass ) ;
		}

	return qd3dStatus ;
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
E3ClassTree::UnregisterClass ( TQ3ObjectType classType, TQ3Boolean isRequired )
	{
	E3GlobalsPtr theGlobals = E3Globals_Get () ;
	
	

	// Find the class to unregister
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( classType ) ;
	if ( theClass == NULL )
		return kQ3Failure ;



	// If we're not required to unregister the class, check the instance count
	if (!isRequired)
		{
		// Make sure the class doesn't have any instances left. Instances have
		// a pointer back to the class tree, so we can't remove the class from
		// the tree while those pointers still exist.
		if (theClass->numInstances != 0)
			{
			E3ErrorManager_PostError(kQ3ErrorObjectClassInUse, kQ3False);
			return kQ3Failure ;
			}
		}



	// Unregister any children of the class
	while (theClass->numChildren != 0)
		{
		// Get the first child
		E3ClassInfoPtr theChild = theClass->theChildren[0];
		Q3_ASSERT_VALID_PTR(theChild);


		// Try and unregister it
		TQ3Status qd3dStatus = E3ClassTree::UnregisterClass ( theChild->classType, isRequired ) ;
		if ( qd3dStatus == kQ3Failure )
			return qd3dStatus ;
		}



	// Remove the class from the tree
	if ( theClass->theParent != NULL )
		theClass->E3ClassInfo::Detach () ;
	
	if ( theGlobals->classTreeRoot == theClass )
		theGlobals->classTreeRoot = NULL ;

	E3HashTable_Remove(theGlobals->classTree, classType);



	// Dispose of the class itself
	Q3_ASSERT(theClass->numChildren == 0);
	Q3_ASSERT(theClass->theChildren == NULL);

	Q3Memory_Free(&theClass->className);
	E3HashTable_Destroy(&theClass->methodTable);
	Q3Memory_Free(&theClass);
	
	return kQ3Success ;
	}





//=============================================================================
//      E3ClassTree_InitialiseInstanceDataOfClass : Initialise the instance data of a class.
//-----------------------------------------------------------------------------
//		Note :	Initialises instances of the specified class and its parent
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
TQ3Status
OpaqueTQ3Object::InitialiseInstanceData (	E3ClassInfoPtr	theClass,
											TQ3Boolean		sharedParams,
											const void		*paramData )
	{	
	TQ3Status qd3dStatus = kQ3Success ;
	TQ3Uns32 parentInstanceSize = 0 ;	
	E3ClassInfoPtr parentClass = theClass->theParent ;
	
	// If this class has a parent, initialise the parent object
	if ( parentClass != NULL )
		{
		parentInstanceSize = parentClass->instanceSize ;
		if (sharedParams)
			qd3dStatus = InitialiseInstanceData ( parentClass, sharedParams, paramData ) ;
		else
			qd3dStatus = InitialiseInstanceData ( parentClass, sharedParams, NULL ) ;
		}

	// If this class has any private data, initialise it
	if ( theClass->instanceSize != parentInstanceSize )
		{
		// If the object has a new method, call it to initialise the object
		TQ3XObjectNewMethod newMethod = (TQ3XObjectNewMethod) theClass->Find_Method (
															  kQ3XMethodTypeObjectNew,
															  kQ3False ) ;
		if ( newMethod != NULL )
			{
			qd3dStatus = newMethod ( (TQ3Object) this, (void*) ( (TQ3Uns8*) this + parentInstanceSize ), (void *) paramData ) ;
			if ( qd3dStatus == kQ3Failure )
				return kQ3Failure ;
			}
		else 
			{
			// If the object is an element, it might have a copy add method
			// which we call to initialise the object.
			TQ3XElementCopyAddMethod copyAddMethod = (TQ3XElementCopyAddMethod) theClass->Find_Method (
																  kQ3XMethodTypeElementCopyAdd,
																  kQ3False ) ;
			if ( copyAddMethod != NULL )
				{
				qd3dStatus = copyAddMethod ( paramData, ( (TQ3Uns8*) this + parentInstanceSize ) ) ;
				if ( qd3dStatus == kQ3Failure )
					return kQ3Failure ;
				}

			// Otherwise if there was no new method, but there was parameter data, do a
			// bitwise copy. Classes which require more advanced initialisation must supply
			// a new method, and classes which don't have any parameter data will be left
			// with instance data that's initialised to 0s.
			else
			if ( paramData != NULL )
				Q3Memory_Copy ( paramData, ( (TQ3Uns8*) this ) + parentInstanceSize, theClass->instanceSize - parentInstanceSize ) ;
			}
		}

	return qd3dStatus ;
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
E3ClassTree::CreateInstance (	TQ3ObjectType	classType,
								TQ3Boolean		sharedParams,
								const void		*paramData )
	{
	// Find the class to instantiate
	//
	// Instantiating objects is often the first thing to fail if the library
	// has not been initialised yet, so we also check for that case here.
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( classType ) ;
	if ( theClass == NULL )
		{
		E3ErrorManager_PostWarning ( kQ3WarningTypeHasNotBeenRegistered ) ;

		if ( ! Q3IsInitialized () )
			E3ErrorManager_PostError ( kQ3ErrorNotInitialized, kQ3False ) ;

		return NULL ;
		}


	return theClass->CreateInstance ( sharedParams , paramData ) ;
	}
	
	
	
TQ3Object
E3ClassInfo::CreateInstance (	TQ3Boolean		sharedParams,
								const void		*paramData )
	{
	// Allocate and initialise the object
	TQ3Object theObject = (TQ3Object) Q3Memory_AllocateClear ( instanceSize + sizeof ( TQ3ObjectType ) ) ;
	if ( theObject == NULL )
		return NULL ;

	theObject->quesaTag = kQ3ObjectTypeQuesa ;
	theObject->theClass = this ;
	
	// Initialise the trailer
	TQ3ObjectType* instanceTrailer = (TQ3ObjectType *) (((TQ3Uns8 *) theObject) + instanceSize ) ;
	*instanceTrailer = kQ3ObjectTypeQuesa ;
		
	TQ3Status qd3dStatus = theObject->InitialiseInstanceData ( this, sharedParams, paramData ) ;

	if ( qd3dStatus == kQ3Failure )
		{
		Q3Memory_Free ( &theObject ) ;
		return NULL ;
		}
		
	// Increment the instance count of the class (watch for overflow)
	++numInstances ;
	Q3_ASSERT ( numInstances > 0 ) ;



	// Verify the object and return it
	Q3_CLASS_VERIFY ( theObject ) ;

	return theObject ;
	}





//=============================================================================
//      e3ClassTree_DeleteInstanceDataOfClass : Call delete methods on the instance data of a class and its parent classes.
//-----------------------------------------------------------------------------
void
OpaqueTQ3Object::DeleteInstanceData ( E3ClassInfoPtr theClass )
	{
	TQ3Uns32 parentInstanceSize = 0 ;
	
	if ( theClass->theParent != NULL )
		parentInstanceSize = theClass->theParent->instanceSize ;

	// Call the object's delete method
	TQ3XElementDeleteMethod elementDeleteMethod = (TQ3XElementDeleteMethod) theClass->Find_Method (
																			kQ3XMethodTypeElementDelete,
																			kQ3False ) ;
	if ( elementDeleteMethod != NULL )
		{
		OpaqueTQ3Object* thisPtr = this ; // So can be set to null in delete method
		elementDeleteMethod ( ( (TQ3Uns8*) this + parentInstanceSize ) ) ;
		}	
	else
		{
		// Get the method
		TQ3XObjectDeleteMethod deleteMethod = (TQ3XObjectDeleteMethod) theClass->Find_Method (
																		kQ3XMethodTypeObjectDelete,
																		kQ3False ) ;

		if ( deleteMethod != NULL )
			deleteMethod ( (TQ3Object) this , (void*)  ( (TQ3Uns8*) this + parentInstanceSize ) ) ;
		}



	// Dispose of the parent object, if any
	if ( theClass->theParent != NULL )
		DeleteInstanceData ( theClass->theParent ) ;

	}





//=============================================================================
//      E3ClassTree_DestroyInstance : Destroy an instance of a class.
//-----------------------------------------------------------------------------
void
OpaqueTQ3Object::DestroyInstance ( void )
	{
	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(this));
	Q3_CLASS_VERIFY(this);



	// Call the object's delete method and all its parent classes' delete methods
	DeleteInstanceData ( theClass ) ;



	// Decrement the instance count of the class
	Q3_ASSERT(theClass->numInstances > 0);
	theClass->numInstances-- ;



	// Dispose of the object
	TQ3Object theObject = (TQ3Object) this ;		
	Q3Memory_Free ( & theObject ) ;
	}





//=============================================================================
//      E3ClassTree_DuplicateInstanceDataOfClass : Duplicate the instance data of a class and its parent classes.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::DuplicateInstanceData (	TQ3Object		newObject,
										E3ClassInfoPtr	theClass )
	{
	TQ3Status qd3dStatus ;
	TQ3Uns32 parentInstanceSize = 0 ;
	
	// If the object has a parent, duplicate the parent object
	if ( theClass->theParent != NULL )
		{
		parentInstanceSize = theClass->theParent->instanceSize ;
		qd3dStatus = DuplicateInstanceData ( newObject , theClass->theParent ) ;
		if ( qd3dStatus == kQ3Failure )
			return kQ3Failure ;
		}



	// If the object has any private data, allocate and duplicate it
	if ( theClass->instanceSize != parentInstanceSize )
		{
		// Call the object's duplicate method to initialise it. If the object
		// does not have duplicate method, we do a bitwise copy.

		TQ3XObjectDuplicateMethod duplicateMethod = (TQ3XObjectDuplicateMethod) theClass->Find_Method (
																			  kQ3XMethodTypeObjectDuplicate,
																			  kQ3False ) ;
		if (duplicateMethod == NULL)
			{
			TQ3XElementCopyDuplicateMethod elementDuplicateMethod = (TQ3XElementCopyDuplicateMethod) theClass->Find_Method (
																			kQ3XMethodTypeElementCopyDuplicate,
																			kQ3False ) ;
			if (elementDuplicateMethod != NULL)
				{
				qd3dStatus = elementDuplicateMethod ( (void*) ( (TQ3Uns8*) this + parentInstanceSize ) , (void*) ( (TQ3Uns8*) newObject + parentInstanceSize ) ) ;
				if ( qd3dStatus == kQ3Failure )
					{
					if ( theClass->theParent != NULL )
						newObject->DeleteInstanceData ( theClass->theParent ) ;
					return kQ3Failure ;
					}
				}
			else
				Q3Memory_Copy ( (void*) ( (TQ3Uns8*) this + parentInstanceSize ) ,
								(void*) ( (TQ3Uns8*) newObject + parentInstanceSize ) , theClass->instanceSize - parentInstanceSize ) ;
			}	
		else
			{
			qd3dStatus = duplicateMethod ( (TQ3Object) this, (void*) ( (TQ3Uns8*) this + parentInstanceSize ),
											newObject, (void*) ( (TQ3Uns8*) newObject + parentInstanceSize ) ) ;
			if ( qd3dStatus == kQ3Failure )
				{
				if ( theClass->theParent != NULL )
					newObject->DeleteInstanceData (	theClass->theParent ) ;
				return kQ3Failure ;
				}
			}
		}
	return kQ3Success ;
	}




//=============================================================================
//      E3ClassTree_DuplicateInstance : Duplicate an instance of a class.
//-----------------------------------------------------------------------------
TQ3Object
OpaqueTQ3Object::DuplicateInstance ( void )
	{
	// Verify our parameters
	Q3_CLASS_VERIFY(this);
	Q3_ASSERT_VALID_PTR(theClass);



	// Allocate and initialise the object
	TQ3Object newObject = (TQ3Object) Q3Memory_AllocateClear ( theClass->instanceSize + sizeof ( TQ3ObjectType ) ) ;
	if ( newObject == NULL )
		return NULL ;

	newObject->quesaTag = kQ3ObjectTypeQuesa ;
	newObject->theClass = theClass;

	// Initialise the trailer
	TQ3ObjectType* instanceTrailer = (TQ3ObjectType *) (((TQ3Uns8 *) newObject) + theClass->instanceSize ) ;
	*instanceTrailer = kQ3ObjectTypeQuesa ;


	TQ3Status qd3dStatus = DuplicateInstanceData ( newObject , theClass ) ;
	if ( qd3dStatus == kQ3Failure )
		{
		Q3Memory_Free ( &newObject ) ;
		return NULL ;
		}
	

	// Increment the instance count of the object's class
	theClass->numInstances++ ;
	Q3_ASSERT ( theClass->numInstances > 0 ) ;



	// Verify the object and return it
	Q3_CLASS_VERIFY ( newObject ) ;

	return newObject ;
	}



//=============================================================================
//      E3ClassTree_FindInstanceData : Find the instance data of an object.
//-----------------------------------------------------------------------------
//		Note :	Returns the instance data of an object or one of its parents.
//
//				If passed kQ3ObjectTypeLeaf, returns the instance data of the
//				leaf object.
//-----------------------------------------------------------------------------

void *
OpaqueTQ3Object::FindLeafInstanceData ( void ) // Same as the old FindInstanceData ( kQ3ObjectTypeLeaf ) but simpler
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), NULL);
	Q3_CLASS_VERIFY(this);

	if ( theClass->theParent != NULL )
		return (void*) ( (TQ3Uns8*) this + theClass->theParent->instanceSize ) ;
	
	return (void*) this ;
	}





//=============================================================================
//      E3ClassTree_GetObjectType : Get a method for a class.
//-----------------------------------------------------------------------------
//		Note :	Gets the type of the first sub-class of baseType.
//-----------------------------------------------------------------------------
TQ3ObjectType
OpaqueTQ3Object::GetObjectType ( TQ3ObjectType baseType )
	{
	// Verify our parameters
	Q3_CLASS_VERIFY(this);

	E3ClassInfoPtr aClass = theClass ;
	

	// Walk up to the level immediately below the base class
	while ( ( aClass != NULL )
	&& ( aClass->theParent != NULL )
	&& ( aClass->theParent->classType != baseType ) )
		{
		Q3_ASSERT_VALID_PTR(aClass);
		aClass = aClass->theParent ;
		}

	if ( ( aClass == NULL ) || ( aClass->theParent == NULL ) )
		return kQ3ObjectTypeInvalid ;
	
	return aClass->classType ;
	}





//=============================================================================
//      E3ClassTree_GetLeafObject : Get the leaf object for an object.
//-----------------------------------------------------------------------------
//		Note : Not available in release builds.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
TQ3Object
OpaqueTQ3Object::GetLeafObject ( void )
	{
	return (TQ3Object) this ;
	}
#endif





//=============================================================================
//      E3ClassTree_IsObjectValid : Is an object valid?
//-----------------------------------------------------------------------------
TQ3Boolean
OpaqueTQ3Object::IsObjectValid ( void )
	{
	// Check to see if the object is valid
	return (TQ3Boolean) ( quesaTag == kQ3ObjectTypeQuesa ) ;
	}





//=============================================================================
//      E3ClassTree_GetClassByType : Find a node in the tree by type.
//-----------------------------------------------------------------------------
E3ClassInfoPtr
E3ClassTree::GetClass ( TQ3ObjectType classType )
	{
	E3GlobalsPtr theGlobals = E3Globals_Get () ;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(classType != kQ3ObjectTypeInvalid, NULL);



	// We can't find anything if we don't have a tree
	if ( theGlobals->classTree == NULL )
		return NULL ;



	// Find the class
	return (E3ClassInfoPtr) E3HashTable_Find ( theGlobals->classTree, classType ) ;
	}





//=============================================================================
//      E3ClassTree_GetClassByName : Find a node in the tree by name.
//-----------------------------------------------------------------------------
//		Note :	NB - the class tree is implemented as a hash table, keyed off
//				the class type. This means that searches by name are very much
//				sub-optimal, and are implemented as a linear search.
//-----------------------------------------------------------------------------
E3ClassInfoPtr
E3ClassTree::GetClass ( const char *className )
	{
	E3GlobalsPtr theGlobals = E3Globals_Get () ;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(className),                    NULL);
	Q3_REQUIRE_OR_RESULT(strlen(className) < kQ3StringMaximumLength, NULL);



	// Make sure we have a root
	if ( theGlobals->classTreeRoot == NULL )
		return NULL ;



	// Find the class, starting at the root
	E3ClassInfoPtr theClass = theGlobals->classTreeRoot->Find ( className ) ;

	return theClass ;
	}





//=============================================================================
//      E3ClassTree_GetClassByObject : Find a node in the tree from an object.
//-----------------------------------------------------------------------------
E3ClassInfoPtr
E3ClassTree::GetClass ( TQ3Object theObject )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theObject), NULL);



	// Get the class
	return theObject->theClass ;
	}





E3ClassInfoPtr
OpaqueTQ3Object::GetClass ( void )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), NULL);



	// Get the class
	return theClass ;
	}





//=============================================================================
//      E3ClassTree_GetParent : Get the parent of a class, if any.
//-----------------------------------------------------------------------------
//		Note : The root node of the class tree has no parent.
//-----------------------------------------------------------------------------
E3ClassInfoPtr
E3ClassInfo::GetParent ( void )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), NULL);



	// Return the parent of the class
	return theParent ;
	}





//=============================================================================
//      E3ClassTree_GetNumChildren : Get the number of children of a class.
//-----------------------------------------------------------------------------
TQ3Uns32
E3ClassInfo::GetNumChildren ( void )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), 0);



	// Return the number of children of the class
	return numChildren ;
	}





//=============================================================================
//      E3ClassTree_GetChild : Get a child of a class.
//-----------------------------------------------------------------------------
//		Note : We assume the index of theChild is valid.
//-----------------------------------------------------------------------------
E3ClassInfoPtr
E3ClassInfo::GetChild ( TQ3Uns32 childIndex )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), NULL);
	Q3_REQUIRE_OR_RESULT(childIndex >= 0 && childIndex < numChildren, NULL);



	// Return the child of the class
	E3ClassInfoPtr theChild = theChildren [ childIndex ] ;
	Q3_ASSERT_VALID_PTR(theChild);
	
	return theChild ;
	}





//=============================================================================
//      E3ClassTree_GetType : Get the type of a class.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3ClassInfo::GetType ( void )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), kQ3ObjectTypeInvalid);



	// Return the type of the class
	return classType ;
	}





//=============================================================================
//      E3ClassTree_IsType : Is a class an instance of a type?
//-----------------------------------------------------------------------------
TQ3Boolean
E3ClassInfo::IsType ( TQ3ObjectType theType)
	{
	E3ClassInfoPtr theClass = this ;

	// Walk up through the class and its parents, checking the type
	while ( theClass != NULL)
		{
		// Check this object
		if (theClass->classType == theType)
			return kQ3True ;



		// Move up a level
		theClass = theClass->theParent;
		}



	// If we're still here, we didn't find the type
	return kQ3False ;
	}





//=============================================================================
//      E3ClassTree_GetName : Get the name of a class.
//-----------------------------------------------------------------------------
//		Note : The pointer returned is read-only, and owned by the class tree.
//-----------------------------------------------------------------------------
const char *
E3ClassInfo::GetName ( void )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), NULL);



	// Return the name of the class
	return className ;
	}





//=============================================================================
//      E3ClassTree_GetMetaHandler : Get the metahandler of a class.
//-----------------------------------------------------------------------------
TQ3XMetaHandler
E3ClassInfo::GetMetaHandler ( void )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), NULL);



	// Return the metahandler of the class
	return classMetaHandler ;
	}





//=============================================================================
//      E3ClassTree_GetInstanceSize : Get the size of a class's instance data.
//-----------------------------------------------------------------------------
TQ3Uns32
E3ClassInfo::GetInstanceSize ( void )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), 0);



	// Return the size of the instance data for the class
	if ( theParent )
		return instanceSize - theParent->instanceSize ;
		
	return instanceSize ;
	}





//=============================================================================
//      E3ClassTree_GetNumInstances : Get number of instances of a class.
//-----------------------------------------------------------------------------
TQ3Uns32
E3ClassInfo::GetNumInstances ( void )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), 0);



	// Return the number of instances of the class
	return numInstances ;
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
E3ClassInfo::GetMethod ( TQ3XMethodType methodType )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), NULL);



	// Find the method
	//
	// We first check the hash table for the class. If this fails, we invoke the
	// metahandler for the class to obtain the method and store it away in the
	// hash table for future use.
	//
	// When invoking the metahandler, we inherit methods that this class doesn't
	// implement from the parent - ensuring that the hash table is eventually
	// populated with all of the (invoked) methods of the class.
	TQ3XFunctionPointer theMethod = (TQ3XFunctionPointer) E3HashTable_Find ( methodTable, methodType ) ;
	if ( theMethod == NULL )
		{
		theMethod = Find_Method ( methodType, kQ3True ) ;
		if (theMethod != NULL)
			E3HashTable_Add ( methodTable, methodType, theMethod ) ;
		}

	return theMethod ;
	}





//=============================================================================
//      E3ClassTree_GetMethodByObject : Get a method for an object's class.
//-----------------------------------------------------------------------------
TQ3XFunctionPointer
OpaqueTQ3Object::GetMethod ( TQ3XMethodType methodType )
	{
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(this), NULL);



	// Get the method
	return theClass->GetMethod ( methodType ) ;
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
E3ClassInfo::AddMethod ( TQ3XMethodType methodType, TQ3XFunctionPointer theMethod )
	{
	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(this));
	Q3_REQUIRE(Q3_VALID_PTR(theMethod));



	// Add the method to the hash table for the class
	E3HashTable_Add ( methodTable, methodType, theMethod ) ;
	}





//=============================================================================
//      E3ClassTree_AddMethodByType : Add a method for a class given its type.
//-----------------------------------------------------------------------------
void
E3ClassTree::AddMethod ( TQ3ObjectType classType, TQ3XMethodType methodType, TQ3XFunctionPointer theMethod )
	{
	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theMethod));



	// Find the class, then add the method
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( classType ) ;
	Q3_ASSERT( theClass != NULL );
	if ( theClass != NULL )
		theClass->AddMethod ( methodType, theMethod ) ;
	}





//=============================================================================
//      E3ClassTree_Dump : Dump some stats on the class tree.
//-----------------------------------------------------------------------------
//		Note :	Dumps some stats on the class tree to a file, for testing hash
//				table sizes/memory leaks/etc.
//-----------------------------------------------------------------------------
void
E3ClassTree::Dump ( void )
	{
	E3GlobalsPtr theGlobals = E3Globals_Get () ;



	// Open our file
	FILE* theFile = fopen("Quesa class tree.dump", "wt");
	if (theFile == NULL)
		return;



	// Write out a header
	time_t theTime = time(NULL);
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

	fprintf(theFile, "class tree, stat counters = %d/%d/%d/%d/%d\n",
						theGlobals->classStats1,
						theGlobals->classStats2,
						theGlobals->classStats3,
						theGlobals->classStats4,
						theGlobals->classStats5);

	fprintf(theFile, "class tree, table size    = %lu\n",
						E3HashTable_GetTableSize(theGlobals->classTree));



	// Dump the class tree, starting at the root
	theGlobals->classTreeRoot->Dump_Class ( theFile, 1 ) ;



	// Clean up
	fclose(theFile);
	}

/*  NAME:
        E3Main.c

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
#include "E3Version.h"
#include "E3View.h"
#include "E3Camera.h"
#include "E3Geometry.h"
#include "E3DrawContext.h"
#include "E3Renderer.h"
#include "E3Group.h"
#include "E3Set.h"
#include "E3Light.h"
#include "E3Style.h"
#include "E3String.h"
#include "E3Transform.h"
#include "E3Main.h"
#include "E3Memory.h"
#include "E3Storage.h"
#include "E3Pick.h"
#include "E3IO.h"
#include "E3Shader.h"
#include "E3Texture.h"
#include "E3CustomElements.h"
#include "E3IOFileFormat.h"
#include "E3StackCrawl.h"

#if QUESA_OS_MACINTOSH
// Viewer supported only on Carbon/Classic now
#include "E3Viewer.h"
#endif





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Shared object data
typedef struct TQ3SharedData {
	TQ3Uns32		refCount;
	TQ3Uns32		editIndex;
} TQ3SharedData;






//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3shared_new : Shared new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3shared_new(TQ3Object theObject, void *privateData, void *paramData)
{	TQ3SharedData		*instanceData = (TQ3SharedData *) privateData;
#pragma unused(theObject)
#pragma unused(paramData)



	// Initialise our instance data
	instanceData->refCount  = 1;
	instanceData->editIndex = 1;
	
	return(kQ3Success);
}





//=============================================================================
//      e3shared_dispose : Shared dispose method.
//-----------------------------------------------------------------------------
static void
e3shared_dispose(TQ3Object theObject)
{	TQ3SharedData		*instanceData;



	// Find the instance data
	instanceData = (TQ3SharedData *) E3ClassTree_FindInstanceData(theObject, kQ3ObjectTypeShared);
	if (instanceData == NULL)
		return;



	// Decrement the reference count
	Q3_ASSERT(instanceData->refCount >= 1);
	instanceData->refCount--;



	// If the reference count falls to 0, dispose of the object
	if (instanceData->refCount == 0)
		E3ClassTree_DestroyInstance(theObject);
}





//=============================================================================
//      e3shared_duplicate : Shared duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3shared_duplicate(TQ3Object fromObject,     const void *fromPrivateData,
						 TQ3Object toObject, void *toPrivateData)
{	TQ3SharedData		*instanceData = (TQ3SharedData *) toPrivateData;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	instanceData->refCount  = 1;
	instanceData->editIndex = 1;

	return(kQ3Success);
}





//=============================================================================
//      e3shared_write : Default write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3shared_write(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status		qd3dStatus;



	// Submit the object
	qd3dStatus = E3FileFormat_Method_SubmitObject (theView, theObject, objectType, objectData);

	return(qd3dStatus);
}





//=============================================================================
//      e3shared_metahandler : Shared metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3shared_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3shared_new;
			break;

		case kQ3XMethodTypeObjectDispose:
			theMethod = (TQ3XFunctionPointer) e3shared_dispose;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3shared_duplicate;
			break;

		case kQ3XMethodTypeObjectSubmitWrite:
			theMethod = (TQ3XFunctionPointer) e3shared_write;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3root_new : Root object new method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3root_new( TQ3Object theObject, void *privateData, void *paramData )
{
#pragma unused( paramData )
	TQ3ObjectData	*instanceData = (TQ3ObjectData *) privateData;


#if Q3_DEBUG
	E3GlobalsPtr	theGlobals = E3Globals_Get();
	static TQ3Boolean	sIsMakingListHead = kQ3False;
	
	if (sIsMakingListHead == kQ3True)
	{
		instanceData->next = theObject;
		instanceData->prev = theObject;
		instanceData->stackCrawl = NULL;
	}
	else
	{
		// initialize instance data
		if (theGlobals->isLeakChecking == kQ3True)
		{
			// make sure the list has a header
			if (theGlobals->listHead == NULL)
			{
				sIsMakingListHead = kQ3True;	// prevent infinite recursion
				theGlobals->listHead = E3ClassTree_CreateInstance( kQ3ObjectTypeRoot,
					kQ3False, NULL );
				sIsMakingListHead = kQ3False;
				Q3_REQUIRE_OR_RESULT( theGlobals->listHead != NULL, kQ3Failure );
			}
			Q3_ASSERT( E3ClassTree_GetType(theGlobals->listHead->theClass) ==
				kQ3ObjectTypeRoot );
			
			// insert the new node between the list header and last normal node
			instanceData->next = theGlobals->listHead;
			instanceData->prev = PREVLINK( theGlobals->listHead );
			NEXTLINK( PREVLINK( theGlobals->listHead ) ) = theObject;
			PREVLINK( theGlobals->listHead ) = theObject;
			
			// Record a stack crawl if possible
			instanceData->stackCrawl = E3StackCrawl_New();
		}
		else
		{
			instanceData->next = NULL;
			instanceData->prev = NULL;
			instanceData->stackCrawl = NULL;
		}
	}
#endif
	
	instanceData->theSet = NULL;
	
	return kQ3Success;
}





//=============================================================================
//      e3root_duplicate : Root object duplicate method.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
static TQ3Status
e3root_duplicate(TQ3Object fromObject,     const void *fromPrivateData,
						 TQ3Object toObject, void *toPrivateData)
{
#pragma unused( fromObject, fromPrivateData )
	TQ3Status	q3status;
	
	q3status = e3root_new( toObject, toPrivateData, NULL );
	
	if (q3status == kQ3Success)
	{
		TQ3ObjectData*	fromInstanceData = (TQ3ObjectData*) fromPrivateData;
		TQ3ObjectData*	toInstanceData = (TQ3ObjectData*) toPrivateData;
		
		
		if (fromInstanceData->theSet != NULL)
		{
			toInstanceData->theSet = Q3Object_Duplicate( fromInstanceData->theSet );
			if (toInstanceData->theSet == NULL)
			{
				q3status = kQ3Failure;
			}
		}
	}
	
	return q3status;
}
#endif





//=============================================================================
//      e3root_delete : Root delete method.
//-----------------------------------------------------------------------------
static void
e3root_delete( TQ3Object theObject, void *privateData )
{
	TQ3ObjectData	*instanceData = (TQ3ObjectData *) privateData;

	Q3_ASSERT( privateData == theObject->instanceData );
	
	Q3Object_CleanDispose( &instanceData->theSet );

#if Q3_DEBUG
	if ( instanceData->prev != NULL )
	{
		NEXTLINK( instanceData->prev ) = instanceData->next;
		PREVLINK( instanceData->next ) = instanceData->prev;
	}

	instanceData->prev = NULL;
	instanceData->next = NULL;
	
	E3StackCrawl_Dispose( instanceData->stackCrawl );
#endif
}





//=============================================================================
//      e3root_dispose : Root object dispose method.
//-----------------------------------------------------------------------------
static void
e3root_dispose(TQ3Object theObject)
{


	// Dispose of the object
	E3ClassTree_DestroyInstance(theObject);
}





//=============================================================================
//      e3root_metahandler : Root object metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3root_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectClassVersion:
			theMethod = (TQ3XFunctionPointer) kQ3PackedVersion;
			break;

		case kQ3XMethodTypeObjectDispose:
			theMethod = (TQ3XFunctionPointer) e3root_dispose;
			break;
		
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3root_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3root_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3root_duplicate;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3main_registercoreclasses : Register the core object classes.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3main_registercoreclasses(void)
{	TQ3Status		qd3dStatus;



	// Register the classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeInvalid,
											kQ3ObjectTypeRoot,
											kQ3ClassNameRoot,
											e3root_metahandler,
											sizeof(TQ3ObjectData)
											);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
												kQ3ObjectTypeShared,
												kQ3ClassNameShared,
												e3shared_metahandler,
												sizeof(TQ3SharedData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
												kQ3SharedTypeShape,
												kQ3ClassNameShape,
												NULL,
												0);

	return(qd3dStatus);
}





//=============================================================================
//      e3main_unregistercoreclasses : Unregister the core object classes.
//-----------------------------------------------------------------------------
static TQ3Status
e3main_unregistercoreclasses(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes. Unregistering the root class will
	// unregister everything else anchored underneath it.
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeRoot, kQ3True);
	return(qd3dStatus);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Initialize : Initialise Quesa.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Initialize(void)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();
	TQ3Status		qd3dStatus = kQ3Success;


	// If we've not initialised Quesa yet, do so now
	if (!theGlobals->systemInitialised)
		{
		// Initialise the platform
		qd3dStatus = E3System_Initialise();



		// Initialise Quesa
		if (qd3dStatus == kQ3Success)
			qd3dStatus = e3main_registercoreclasses();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Memory_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3String_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Transform_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Group_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Set_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Light_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Style_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3View_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3DrawContext_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Camera_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Geometry_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Shader_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Texture_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Storage_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3File_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Pick_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3CustomElements_RegisterClass();

		#if QUESA_OS_MACINTOSH
		// Viewer  supported only on Carbon/Classic
		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Viewer_RegisterClass();
		#endif
		


		// Load our plug-ins
		if (qd3dStatus == kQ3Success)
			E3System_LoadPlugins();



		// Set our flag
		if (qd3dStatus == kQ3Success)
			theGlobals->systemInitialised = kQ3True;
		}
	
	
	// Or post a notice
	else
		E3ErrorManager_PostNotice(kQ3NoticeSystemAlreadyInitialized);



	// If all went well, increment the reference count
	if (qd3dStatus == kQ3Success)
		theGlobals->systemRefCount++;

	return(qd3dStatus);
}





//=============================================================================
//      E3Exit : Terminate Quesa.
//-----------------------------------------------------------------------------
TQ3Status
E3Exit(void)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();
	TQ3Status		qd3dStatus;



	// Make sure we've been initialised
	if (!theGlobals->systemInitialised)
		return(kQ3Failure);



	// Decrement the instance count		
	Q3_ASSERT(theGlobals->systemRefCount > 0);
	theGlobals->systemRefCount--;



	// If this was the last instance, terminate Quesa
	if (theGlobals->systemRefCount == 0)
		{
		// Dump some stats
#if QUESA_DUMP_STATS_ON_EXIT
		E3ClassTree_Dump();
#endif

#if QUESA_ALLOW_QD3D_EXTENSIONS && Q3_DEBUG
		if ( Q3Memory_IsRecording() && (Q3Memory_CountRecords() > 0) )
		{
			E3ErrorManager_PostError( kQ3ErrorMemoryLeak, kQ3False );
			Q3Memory_DumpRecording( "Quesa-leaks.txt", "Q3Exit" );
			Q3Memory_ForgetRecording();
		}
#endif



		// Reset leak-checking globals to initial state
#if Q3_DEBUG
		Q3Object_CleanDispose(&theGlobals->listHead );
		theGlobals->isLeakChecking = kQ3False;
#endif



		// Unload our plug-ins
		E3System_UnloadPlugins();



		// Terminate Quesa
		#if QUESA_OS_MACINTOSH
		// Viewer  supported only on Carbon/Classic
		qd3dStatus = E3Viewer_UnregisterClass();
		#endif
		qd3dStatus = E3CustomElements_UnregisterClass();
		qd3dStatus = E3Pick_UnregisterClass();
		qd3dStatus = E3File_UnregisterClass();
		qd3dStatus = E3Storage_UnregisterClass();
		qd3dStatus = E3Renderer_UnregisterClass();
		qd3dStatus = E3Texture_UnregisterClass();
		qd3dStatus = E3Shader_UnregisterClass();
		qd3dStatus = E3Geometry_UnregisterClass();
		qd3dStatus = E3Camera_UnregisterClass();
		qd3dStatus = E3DrawContext_UnregisterClass();
		qd3dStatus = E3View_UnregisterClass();
		qd3dStatus = E3Style_UnregisterClass();
		qd3dStatus = E3Light_UnregisterClass();
		qd3dStatus = E3Set_UnregisterClass();
		qd3dStatus = E3Group_UnregisterClass();
		qd3dStatus = E3Transform_UnregisterClass();
		qd3dStatus = E3String_UnregisterClass();
		qd3dStatus = E3Memory_UnregisterClass();
		qd3dStatus = e3main_unregistercoreclasses();
		E3ClassTree_Destroy();



		// Terminate the platform
		E3System_Terminate();



		// Set our flag
		theGlobals->systemInitialised = kQ3False;
		}

	return(kQ3Success);
}





//=============================================================================
//      E3IsInitialized : Test to see if Quesa has been initialised.
//-----------------------------------------------------------------------------
TQ3Boolean
E3IsInitialized(void)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();



	// Return as we've initialised Quesa
	return(theGlobals->systemInitialised);
}





//=============================================================================
//      E3GetVersion : Return the build version.
//-----------------------------------------------------------------------------
//		Note :	kQ3MajorVersion and kQ3MinorVersion are each in BCD format.
//				For example, 12 is represented as 0x12.
//
//				May be called outside of a Q3Initialize/Q3Exit block.
//-----------------------------------------------------------------------------
TQ3Status
E3GetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{


	// Return the build version
	*majorRevision = 10 * (kQ3MajorVersion >> 4) + (kQ3MajorVersion & 0x0f);
	if (kQ3MinorVersion & 0x0f)
		*minorRevision = 10 * (kQ3MinorVersion >> 4) + (kQ3MinorVersion & 0x0f);
	else // single-digit minor version
		*minorRevision = (kQ3MinorVersion >> 4);

	return(kQ3Success);
}





//=============================================================================
//      E3GetReleaseVersion : Return the build version in 'vers' format.
//-----------------------------------------------------------------------------
//		Note :	We return the version number in the format of the first four
//				bytes of a 'vers' resource. For example, "1.23a56" is
//				represented as 0x01234056.
//
//				For more information, see the description of the 'vers'
//				resource in "Inside Macintosh: Macintosh Toolbox Essential",
//				p. 7-69.
//
//				May be called outside of a Q3Initialize/Q3Exit block.
//-----------------------------------------------------------------------------
TQ3Status
E3GetReleaseVersion(TQ3Uns32 *releaseRevision)
{


	// Return the release version
	*releaseRevision = kQ3PackedVersion;

	return(kQ3Success);
}





//=============================================================================
//      E3ObjectHierarchy_GetTypeFromString : Find the type for a class.
//-----------------------------------------------------------------------------
//		Note : Given a class name, returns the type of the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3ObjectHierarchy_GetTypeFromString(const TQ3ObjectClassNameString objectClassString, TQ3ObjectType *objectClassType)
{	E3ClassInfoPtr		theClass;



	// Initialise a return value
	*objectClassType = kQ3ObjectTypeInvalid;



	// Find the class
	theClass = E3ClassTree_GetClassByName(objectClassString);
	if (theClass == NULL)
		return(kQ3Failure);



	// Return the type
	*objectClassType = E3ClassTree_GetType(theClass);
	return(kQ3Success);
}





//=============================================================================
//      E3ObjectHierarchy_GetStringFromType : Find the name for a class.
//-----------------------------------------------------------------------------
//		Note : Given a class type, returns the name of the class.
//-----------------------------------------------------------------------------
TQ3Status
E3ObjectHierarchy_GetStringFromType(TQ3ObjectType objectClassType, TQ3ObjectClassNameString objectClassString)
{	E3ClassInfoPtr		theClass;



	// Initialise a return value
	strcpy(objectClassString, "");



	// Find the class
	theClass = E3ClassTree_GetClassByType(objectClassType);
	if (theClass == NULL)
		return(kQ3Failure);



	// Return the name
	strcpy(objectClassString, E3ClassTree_GetName(theClass));
	return(kQ3Success);
}





//=============================================================================
//      E3ObjectHierarchy_IsTypeRegistered : Test if a class is registered.
//-----------------------------------------------------------------------------
TQ3Boolean
E3ObjectHierarchy_IsTypeRegistered(TQ3ObjectType objectClassType)
{	E3ClassInfoPtr		theClass;



	// Find the class
	theClass = E3ClassTree_GetClassByType(objectClassType);

	return(theClass != NULL ? kQ3True : kQ3False);
}





//=============================================================================
//      E3ObjectHierarchy_IsNameRegistered : Test if a class is registered.
//-----------------------------------------------------------------------------
TQ3Boolean
E3ObjectHierarchy_IsNameRegistered(const char *objectClassName)
{	E3ClassInfoPtr		theClass;



	// Find the class
	theClass = E3ClassTree_GetClassByName(objectClassName);

	return(theClass != NULL ? kQ3True : kQ3False);
}





//=============================================================================
//      E3ObjectHierarchy_GetSubClassData : Get the sub-classes of a class.
//-----------------------------------------------------------------------------
//		Note :	Fills in subClassData with the number and class types of all of
//				the subclasses immediately below the specified class.
//-----------------------------------------------------------------------------
TQ3Status
E3ObjectHierarchy_GetSubClassData(TQ3ObjectType objectClassType, TQ3SubClassData *subClassData)
{	E3ClassInfoPtr		theClass, theChild;
	TQ3Uns32			n, numChildren;



	// Initialise a return value
	subClassData->numClasses = 0;
	subClassData->classTypes = NULL;



	// Find the class
	theClass = E3ClassTree_GetClassByType(objectClassType);
	if (theClass == NULL)
		return(kQ3Failure);



	// Allocate the array for the child classes
	Q3Memory_Clear(subClassData, sizeof(TQ3SubClassData));

	numChildren = E3ClassTree_GetNumChildren(theClass);
	if (numChildren != 0)
		{
		subClassData->classTypes = (TQ3ObjectType *) Q3Memory_Allocate(sizeof(TQ3ObjectType) * numChildren);
		if (subClassData->classTypes == NULL)
			return(kQ3Failure);

		subClassData->numClasses = numChildren;
		}



	// Collect the child classes	
	for (n = 0; n < numChildren; n++)
		{
		// Get the child
		theChild = E3ClassTree_GetChild(theClass, n);
		Q3_ASSERT_VALID_PTR(theChild);
		
		
		// Grab the type
		subClassData->classTypes[n] = E3ClassTree_GetType(theChild);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3ObjectHierarchy_EmptySubClassData : Dispose of the sub-class data.
//-----------------------------------------------------------------------------
TQ3Status
E3ObjectHierarchy_EmptySubClassData(TQ3SubClassData *subClassData)
{


	// Dispose of the data
	Q3Memory_Free(&subClassData->classTypes);



	// Reset the structure
	subClassData->numClasses = 0;
	subClassData->classTypes = NULL;

	return(kQ3Success);
}





//=============================================================================
//      E3Object_Dispose : Dispose of an object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Object_Dispose(TQ3Object theObject)
{	TQ3XObjectDisposeMethod		disposeMethod;



	// Skip NULL objects
	if (theObject == NULL)
		return(kQ3Failure);



	// Get the method
	disposeMethod = (TQ3XObjectDisposeMethod) E3ClassTree_GetMethod(
												theObject->theClass,
												kQ3XMethodTypeObjectDispose);
	if (disposeMethod == NULL)
		return(kQ3Failure);



	// Dispose of the object
	disposeMethod(theObject);

	return(kQ3Success);
}





//=============================================================================
//      E3Object_CleanDispose : Dispose of an object.
//-----------------------------------------------------------------------------
TQ3Status
E3Object_CleanDispose(TQ3Object *theObject)
{	TQ3Status		qd3dStatus;



	// If we have an object, dispose of it and clear the pointer
	if (theObject != NULL && *theObject != NULL)
		{
		qd3dStatus = Q3Object_Dispose(*theObject);
		*theObject = NULL;
		}
	else
		qd3dStatus = kQ3Success;

	return(qd3dStatus);
}





//=============================================================================
//      E3Object_Duplicate : Duplicate an object.
//-----------------------------------------------------------------------------
//		Note :	Draw context and view objects can not be duplicated, since
//				these refer to resources created by the application that belong
//				to the window system.
//
//				If the new object is a shared object, its reference count is
//				set to 1.
//-----------------------------------------------------------------------------
TQ3Object
E3Object_Duplicate(TQ3Object theObject)
{	TQ3Object		newObject;



	// Reject draw context and view objects
	//
	// Note - the current Quesa implementation does not support
	// duplicating storage objects (although this is part of the
	// the QD3D behaviour).
	//
	// When this is functionality is added, the kQ3SharedTypeStorage
	// test should be removed.
	//
	// If this causes your app problems, please contact Jose Cruanyes
	// or Dair Grant to discuss a fix.
	if (Q3Object_IsType(theObject, kQ3ObjectTypeView)        ||
		Q3Object_IsType(theObject, kQ3SharedTypeDrawContext) ||
		Q3Object_IsType(theObject, kQ3SharedTypeStorage))
		{
		E3ErrorManager_PostError(kQ3ErrorInvalidObjectType, kQ3False);
		return(NULL);
		}



	// Duplicate the object
	newObject = E3ClassTree_DuplicateInstance(theObject);
	return(newObject);
}





//=============================================================================
//      E3Object_Submit : Submit an object to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3Object_Submit(TQ3Object theObject, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object
	qd3dStatus = E3View_SubmitRetained(theView, theObject);
	return(qd3dStatus);
}





//=============================================================================
//      E3Object_IsDrawable : Determine if an object is drawable.
//-----------------------------------------------------------------------------
//		Note :	There is no is-drawable method as such, instead we interpret
//				the 'address' of the method as a TQ3Boolean.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Object_IsDrawable(TQ3Object theObject)
{	TQ3Boolean		isDrawable;



	// Get the 'method'
	isDrawable = (TQ3Boolean) E3ClassTree_GetMethod(
									theObject->theClass,
									kQ3XMethodTypeObjectIsDrawable);

	return(isDrawable);
}





//=============================================================================
//      E3Object_IsWritable : Determine if an object is writeable.
//-----------------------------------------------------------------------------
//		Note : We return as the object has a kQ3XMethodTypeObjectWrite method.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Object_IsWritable(TQ3Object theObject, TQ3FileObject theFile)
{	TQ3XObjectWriteMethod		writeMethod;
#pragma unused(theFile)



	// Get the method
	writeMethod = (TQ3XObjectWriteMethod) E3ClassTree_GetMethod(
												theObject->theClass,
												kQ3XMethodTypeObjectWrite);



	// Return as the method exists or not
	return((TQ3Boolean) (writeMethod != NULL));
}





//=============================================================================
//      E3Object_GetType : Get the fundamental type of an object.
//-----------------------------------------------------------------------------
//		Note :	To determine the fundamental type, we walk up the object's
//				parents until we come to the level below the root object.
//
//				NB - we can not filter the result, since some undocumented QD3D
//				objects are registered off the root object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Object_GetType(TQ3Object theObject)
{	TQ3ObjectType	theType;



	// Get the type of the object below the root object
	theType = E3ClassTree_GetObjectType(theObject, kQ3ObjectTypeRoot);

	return(theType);
}





//=============================================================================
//      E3Object_GetLeafType : Get the most specific type of the object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Object_GetLeafType(TQ3Object theObject)
{	TQ3ObjectType		theType;



	// Get the most specific type of the object
	theType = E3ClassTree_GetType(theObject->theClass);

	return(theType);
}





//=============================================================================
//      E3Object_IsType : Is an object an instance of a particular type?
//-----------------------------------------------------------------------------
TQ3Boolean
E3Object_IsType(TQ3Object theObject, TQ3ObjectType theType)
{	TQ3Boolean		isType;



	// Return as the object is an instance of the type
	isType = E3ClassTree_IsType(theObject->theClass, theType);
	
	return(isType);
}





//=============================================================================
//      E3Object_AddElement : Add an element to an object
//-----------------------------------------------------------------------------
TQ3Status
E3Object_AddElement(TQ3Object theObject, TQ3ElementType theType, const void *theData)
{
	TQ3ObjectData*	instanceData;
	TQ3Status		qd3dStatus;



	// Translate public type to internal type for set elements
	if (theType == kQ3ElementTypeSet)
	{
		theType = kQ3ObjectTypeSetElement;
	}



	// If we've actually been passed a set, use it directly
	if (Q3Object_IsType(theObject, kQ3SharedTypeSet))
	{
		qd3dStatus = Q3Set_Add( (TQ3SetObject) theObject, theType, theData );
	}
	else	// otherwise use the set within the instance data
	{
		instanceData = (TQ3ObjectData *) E3ClassTree_FindInstanceData(theObject, kQ3ObjectTypeRoot);
		if (instanceData == NULL)
			return(kQ3Failure);
		
		if (instanceData->theSet == NULL)
		{
			instanceData->theSet = Q3Set_New();
			
			if (instanceData->theSet == NULL)
				return(kQ3Failure);
		}
		
		qd3dStatus = Q3Set_Add( instanceData->theSet, theType, theData );
		
		
		if ( (qd3dStatus == kQ3Success) && Q3Object_IsType( theObject, kQ3ObjectTypeShared ) )
		{
			E3Shared_Edited( theObject );
		}
	}
	
	return qd3dStatus;
}





//=============================================================================
//      E3Object_GetElement : Get element data from an object.
//-----------------------------------------------------------------------------
TQ3Status
E3Object_GetElement(TQ3Object theObject, TQ3ElementType theType, void *theData)
{
	TQ3ObjectData*	instanceData;
	TQ3Status		qd3dStatus;
	
	
	
	// Translate public type to internal type for set elements
	if (theType == kQ3ElementTypeSet)
	{
		theType = kQ3ObjectTypeSetElement;
	}



	// If we've actually been passed a set, use it directly
	if (Q3Object_IsType(theObject, kQ3SharedTypeSet))
	{
		qd3dStatus = Q3Set_Get( (TQ3SetObject) theObject, theType, theData );
	}
	else	// otherwise use the set within the instance data
	{
		instanceData = (TQ3ObjectData *) E3ClassTree_FindInstanceData(theObject, kQ3ObjectTypeRoot);
		if (instanceData == NULL)
			return(kQ3Failure);
		
		
		if (instanceData->theSet == NULL)
		{
			qd3dStatus = kQ3Failure;
		}
		else
		{
			qd3dStatus = Q3Set_Get(instanceData->theSet, theType, (void*)theData);
		}
	}
	
	return qd3dStatus;
}





//=============================================================================
//      E3Object_ContainsElement : Test whether an object contains an element.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Object_ContainsElement(TQ3Object theObject, TQ3ElementType theType)
{
	TQ3ObjectData*	instanceData;
	TQ3Boolean		isElementPresent = kQ3False;
	
	
	
	// Translate public type to internal type for set elements
	if (theType == kQ3ElementTypeSet)
	{
		theType = kQ3ObjectTypeSetElement;
	}



	// If we've actually been passed a set, use it directly
	if (Q3Object_IsType(theObject, kQ3SharedTypeSet))
	{
		isElementPresent = Q3Set_Contains( (TQ3SetObject) theObject, theType );
	}
	else	// otherwise use the set within the instance data
	{
		instanceData = (TQ3ObjectData *) E3ClassTree_FindInstanceData(theObject, kQ3ObjectTypeRoot);
		
		
		if ( (instanceData != NULL) && (instanceData->theSet != NULL) )
		{
			isElementPresent = Q3Set_Contains( instanceData->theSet, theType );
		}
	}
	
	
	return isElementPresent;
}





//=============================================================================
//      E3Object_GetNextElementType : Get the next element type in an object.
//-----------------------------------------------------------------------------
TQ3Status
E3Object_GetNextElementType(TQ3Object theObject, TQ3ElementType *theType)
{
	TQ3ObjectData*	instanceData;
	TQ3Status		qd3dStatus = kQ3Failure;
	
	
	
	// Translate public type to internal type for set elements
	if (*theType == kQ3ElementTypeSet)
	{
		*theType = kQ3ObjectTypeSetElement;
	}



	// If we've actually been passed a set, use it directly
	if (Q3Object_IsType(theObject, kQ3SharedTypeSet))
	{
		qd3dStatus = Q3Set_GetNextElementType( (TQ3SetObject) theObject, theType );
	}
	else	// otherwise use the set within the instance data
	{
		instanceData = (TQ3ObjectData *) E3ClassTree_FindInstanceData(theObject, kQ3ObjectTypeRoot);
		
		
		if ( instanceData != NULL )
		{
			if (instanceData->theSet == NULL)
			{
				*theType = kQ3ElementTypeNone;
				qd3dStatus = kQ3Success;
			}
			else
			{
				qd3dStatus = Q3Set_GetNextElementType(instanceData->theSet, theType);
			}
		}
	}



	if (*theType == kQ3ObjectTypeSetElement)
	{
		*theType = kQ3ElementTypeSet;
	}
	
	
	return qd3dStatus;
}





//=============================================================================
//      E3Object_EmptyElements : Remove all elements from an object.
//-----------------------------------------------------------------------------
TQ3Status
E3Object_EmptyElements(TQ3Object theObject)
{
	TQ3ObjectData*	instanceData;
	TQ3Status		qd3dStatus = kQ3Failure;
	
	
	
	// If we've actually been passed a set, use it directly
	if (Q3Object_IsType(theObject, kQ3SharedTypeSet))
	{
		qd3dStatus = Q3Set_Empty( (TQ3SetObject) theObject );
	}
	else	// otherwise use the set within the instance data
	{
		instanceData = (TQ3ObjectData *) E3ClassTree_FindInstanceData(theObject, kQ3ObjectTypeRoot);
		
		
		if (instanceData == NULL)
		{
			qd3dStatus = kQ3Failure;	// should never happen
		}
		else if ( instanceData->theSet == NULL )
		{
			qd3dStatus = kQ3Success;
		}
		else
		{
			qd3dStatus = Q3Set_Empty(instanceData->theSet);
		
		
			if ( (qd3dStatus == kQ3Success) && Q3Object_IsType( theObject, kQ3ObjectTypeShared ) )
			{
				E3Shared_Edited( theObject );
			}
		}
	}
	
	
	return qd3dStatus;
}





//=============================================================================
//      E3Object_ClearElement : Remove a specific type of element from an object.
//-----------------------------------------------------------------------------
TQ3Status
E3Object_ClearElement(TQ3Object theObject, TQ3ElementType theType)
{
	TQ3ObjectData*	instanceData;
	TQ3Status		qd3dStatus = kQ3Failure;
	
	
	
	// Translate public type to internal type for set elements
	if (theType == kQ3ElementTypeSet)
	{
		theType = kQ3ObjectTypeSetElement;
	}



	// If we've actually been passed a set, use it directly
	if (Q3Object_IsType(theObject, kQ3SharedTypeSet))
	{
		qd3dStatus = Q3Set_Clear( (TQ3SetObject) theObject, theType );
	}
	else	// otherwise use the set within the instance data
	{
		instanceData = (TQ3ObjectData *) E3ClassTree_FindInstanceData(theObject, kQ3ObjectTypeRoot);
		
		
		if (instanceData == NULL)
		{
			qd3dStatus = kQ3Failure;	// should never happen
		}
		else if ( instanceData->theSet == NULL )
		{
			qd3dStatus = kQ3Success;
		}
		else
		{
			qd3dStatus = Q3Set_Clear( instanceData->theSet, theType );
		
		
			if ( (qd3dStatus == kQ3Success) && Q3Object_IsType( theObject, kQ3ObjectTypeShared ) )
			{
				E3Shared_Edited( theObject );
			}
		}
	}
	
	
	return qd3dStatus;
}





//=============================================================================
//      E3Shared_GetType : Get the type of a shared object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
E3Shared_GetType(TQ3SharedObject sharedObject)
{


	// Return the type
	return(E3ClassTree_GetObjectType(sharedObject, kQ3ObjectTypeShared));
}





//=============================================================================
//      E3Shared_GetReference : Get a reference to a shared object.
//-----------------------------------------------------------------------------
//		Note :	We increase the reference count of the shared object by 1.
//
//				Note that other Quesa code assumes acquiring a new reference
//				to an object can not fail, provided the object is in fact a
//				shared object.
//
//				If acquiring a new reference could ever fail, routines like
//				E3Shared_Replace will need to be updated.
//-----------------------------------------------------------------------------
TQ3SharedObject
E3Shared_GetReference(TQ3SharedObject sharedObject)
{	TQ3SharedData		*instanceData;



	// Find the instance data
	instanceData = (TQ3SharedData *) E3ClassTree_FindInstanceData(sharedObject, kQ3ObjectTypeShared);
	if (instanceData == NULL)
		return(NULL);



	// Increment the reference count and return the object. Note that we
	// return the object passed in: this is OK since we're not declared
	// to return a different object.
	instanceData->refCount++;
	Q3_ASSERT(instanceData->refCount >= 2);

	return(sharedObject);
}





//=============================================================================
//      E3Shared_IsReferenced : Is a reference shared?
//-----------------------------------------------------------------------------
//		Note :	We return kQ3True if there is more than one reference to the
//				object, and kQ3False if there is one reference to the object.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Shared_IsReferenced(TQ3SharedObject sharedObject)
{	TQ3SharedData		*instanceData;



	// Find the instance data
	instanceData = (TQ3SharedData *) E3ClassTree_FindInstanceData(sharedObject, kQ3ObjectTypeShared);
	if (instanceData == NULL)
		return(kQ3False);



	// Return as the reference count is greater than 1
	return((TQ3Boolean) (instanceData->refCount > 1));
}





//=============================================================================
//      E3Shared_GetReferenceCount : Return the reference count.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Shared_GetReferenceCount( TQ3SharedObject sharedObject )
{
	TQ3SharedData		*instanceData;


	// Find the instance data
	instanceData = (TQ3SharedData *) E3ClassTree_FindInstanceData(sharedObject, kQ3ObjectTypeShared);
	if (instanceData == NULL)
		return(0);



	// Return the reference count
	return instanceData->refCount;
}





//=============================================================================
//      E3Shared_GetEditIndex : Return the edit index of a shared object.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Shared_GetEditIndex(TQ3SharedObject sharedObject)
{	TQ3SharedData		*instanceData;



	// Find the instance data
	instanceData = (TQ3SharedData *) E3ClassTree_FindInstanceData(sharedObject, kQ3ObjectTypeShared);
	if (instanceData == NULL)
		return(0);



	// Return the edit index
	return(instanceData->editIndex);
}





//=============================================================================
//      E3Shared_Edited : Increase the edit index of an object.
//-----------------------------------------------------------------------------
TQ3Status
E3Shared_Edited(TQ3SharedObject sharedObject)
{	TQ3SharedData		*instanceData;



	// Find the instance data
	instanceData = (TQ3SharedData *) E3ClassTree_FindInstanceData(sharedObject, kQ3ObjectTypeShared);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Increment the edit index
	instanceData->editIndex++;
	return(kQ3Success);
}





//=============================================================================
//      E3Shape_GetType : Get the type of a shape object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
E3Shape_GetType(TQ3ShapeObject theShape)
{


	// Return the type
	return(E3ClassTree_GetObjectType(theShape, kQ3SharedTypeShape));
}





//=============================================================================
//      E3Shape_GetSet : Get the set for a shape.
//-----------------------------------------------------------------------------
TQ3Status
E3Shape_GetSet(TQ3ShapeObject theShape, TQ3SetObject *theSet)
{
	*theSet = NULL;
	return E3Object_GetElement( theShape, kQ3ElementTypeSet, theSet );
}





//=============================================================================
//      E3Shape_SetSet : Set the set for a shape.
//-----------------------------------------------------------------------------
TQ3Status
E3Shape_SetSet(TQ3ShapeObject theShape, TQ3SetObject theSet)
{
	return E3Object_AddElement( theShape, kQ3ElementTypeSet, &theSet );
}





//=============================================================================
//      E3Shape_SubmitElements : Submit custom elements of a shape.
//-----------------------------------------------------------------------------
//		This will be called by E3FFW_3DMF_TraverseObject to submit
//		any custom elements that might be attached to the shape.
//-----------------------------------------------------------------------------
TQ3Status
E3Shape_SubmitElements( TQ3ShapeObject inShape, TQ3ViewObject inView )
{
	TQ3ObjectData		*instanceData;
	TQ3Status			qd3dStatus;



	// Find the instance data
	instanceData = (TQ3ObjectData *) E3ClassTree_FindInstanceData(inShape, kQ3ObjectTypeRoot);
	if (instanceData == NULL)
		return(kQ3Failure);



	// Use the shape's set, if any.
	if (instanceData->theSet == NULL)
		qd3dStatus = kQ3Success;
	else
		qd3dStatus = E3Set_SubmitElements( instanceData->theSet, inView );
	
	
	
	return qd3dStatus;
}





//=============================================================================
//      E3Bitmap_Empty : Dispose of the memory allocated for a bitmap.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Bitmap_Empty(TQ3Bitmap *theBitmap)
{


	// Dispose of the bitmap's image
	Q3Memory_Free(&theBitmap->image);

	return(kQ3Success);
}





//=============================================================================
//      E3Bitmap_GetImageSize : Get the amount of memory required for a bitmap.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Bitmap_GetImageSize(TQ3Uns32 theWidth, TQ3Uns32 theHeight)
{	TQ3Uns32		imageSize;



	// Bitmaps are 1 bit deep, so every 8 pixels requires 1 byte
	imageSize  = ((theWidth - 1) / 8) + 1;
	imageSize *= theHeight;

	return(imageSize);
}



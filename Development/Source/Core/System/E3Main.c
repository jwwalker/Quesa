/*  NAME:
        E3Main.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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
#include <new>

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




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      E3ShapeInfo::E3ShapeInfo : Constructor for class info of root class.
//-----------------------------------------------------------------------------

E3ShapeInfo::E3ShapeInfo	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
		: E3SharedInfo ( newClassMetaHandler, newParent )
	{
	// Fill in the method data of the class
	
	// There are (currently) no new methods in shape class
	} ;


//=============================================================================
//      e3shape_new_class_info : Method to construct a class info record.
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3shape_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent
			 	)
	{
	return new ( std::nothrow ) E3ShapeInfo ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3shape_metahandler : Shape metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3shape_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeNewObjectClass:
			theMethod = (TQ3XFunctionPointer) e3shape_new_class_info;
			break;

		}
	
	return(theMethod);
}





//=============================================================================
//      E3SharedInfo::E3SharedInfo : Constructor for class info of root class.
//-----------------------------------------------------------------------------
#pragma mark -
E3SharedInfo::E3SharedInfo	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
		: E3Root ( newClassMetaHandler, newParent )
	{
	// Fill in the method data of the class
	
//	disposeMethod = (TQ3XObjectDisposeMethod) Find_Method ( kQ3XMethodTypeObjectDispose , kQ3True ) ;
//	newMethod = (TQ3XObjectNewMethod) Find_Method ( kQ3XMethodTypeObjectNew , kQ3False ) ; // N.B. False, not inherited
//	deleteMethod = (TQ3XObjectDeleteMethod) Find_Method ( kQ3XMethodTypeObjectDelete , kQ3False ) ; // N.B. False, not inherited
//	duplicateMethod = (TQ3XObjectDuplicateMethod) Find_Method ( kQ3XMethodTypeObjectDuplicate , kQ3False ) ; // N.B. False, not inherited
	} ;


//=============================================================================
//      e3shared_new_class_info : Method to construct a class info record.
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3shared_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent
			 	)
	{
	return new ( std::nothrow ) E3SharedInfo ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3shared_new : Shared new method.
//-----------------------------------------------------------------------------
TQ3Status
e3shared_new ( E3Shared* theObject, void *privateData, void *paramData )
	{
#pragma unused(privateData)
#pragma unused(paramData)



	// Initialise our instance data
	theObject->refCount  = 1 ;
	theObject->editIndex = 1 ;
	
	return kQ3Success ;
	}





//=============================================================================
//      e3shared_dispose : Shared dispose method.
//-----------------------------------------------------------------------------
void
e3shared_dispose ( E3Shared* theObject )
	{
	// Find the instance data
	if ( theObject == NULL )
		return ;



	// Decrement the reference count
	Q3_ASSERT(theObject->refCount >= 1);
	theObject->refCount--;



	// If the reference count falls to 0, dispose of the object
	if ( theObject->refCount == 0 )
		theObject->DestroyInstance () ;
	}





//=============================================================================
//      e3shared_duplicate : Shared duplicate method.
//-----------------------------------------------------------------------------
TQ3Status
e3shared_duplicate(TQ3Object fromObject,     const void *fromPrivateData,
						 TQ3Object toObject, void *toPrivateData)
{
	E3Shared		*instanceData = (E3Shared *) toObject ;
	E3Shared		*fromInstanceData = (E3Shared *) fromObject ;
#pragma unused(fromPrivateData, toPrivateData)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	instanceData->refCount  = 1;
	instanceData->editIndex = fromInstanceData->editIndex;

	return(kQ3Success);
}





//=============================================================================
//      e3shared_write : Default write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3shared_write ( TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData )
	{
	// Submit the object
	return E3FileFormat_Method_SubmitObject ( theView, theObject, objectType, objectData ) ;
	}





//=============================================================================
//      e3shared_metahandler : Shared metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3shared_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeNewObjectClass:
			theMethod = (TQ3XFunctionPointer) e3shared_new_class_info;
			break;

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
//      E3Root::E3Root : Constructor for class info of root class.
//-----------------------------------------------------------------------------
#pragma mark -

E3Root::E3Root	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
	: E3ClassInfo ( newClassMetaHandler , newParent ) ,
	// Fill in the method data of the class
	disposeMethod		( (TQ3XObjectDisposeMethod)		Find_Method ( kQ3XMethodTypeObjectDispose , kQ3True ) ) ,
	newMethod			( (TQ3XObjectNewMethod)			Find_Method ( kQ3XMethodTypeObjectNew , kQ3False ) ) , // N.B. False, not inherited
	deleteMethod		( (TQ3XObjectDeleteMethod)		Find_Method ( kQ3XMethodTypeObjectDelete , kQ3False ) ) , // N.B. False, not inherited
	duplicateMethod		( (TQ3XObjectDuplicateMethod)	Find_Method ( kQ3XMethodTypeObjectDuplicate , kQ3False ) ) , // N.B. False, not inherited
	submitRenderMethod	( (TQ3XObjectSubmitMethod)		Find_Method ( kQ3XMethodTypeObjectSubmitRender , kQ3True ) ) ,
	submitPickMethod	( (TQ3XObjectSubmitMethod)		Find_Method ( kQ3XMethodTypeObjectSubmitPick , kQ3True ) ) ,
	submitBoundsMethod	( (TQ3XObjectSubmitMethod)		Find_Method ( kQ3XMethodTypeObjectSubmitBounds , kQ3True ) ) ,
	submitWriteMethod	( (TQ3XObjectSubmitMethod)		Find_Method ( kQ3XMethodTypeObjectSubmitWrite , kQ3True ) )
	{
	if ( disposeMethod == NULL )
		SetAbstract () ;
	} ;


//=============================================================================
//      e3root_new_class_info : Method to construct a class info record for the root class.
// This is necessary because applications can register their own object classes based on ours
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3root_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
	{
	return new ( std::nothrow ) E3Root ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3root_new : Root object new method.
//-----------------------------------------------------------------------------

TQ3Status
e3root_new( TQ3Object theObject, void *privateData, void *paramData )
{
#pragma unused( paramData )
#pragma unused( privateData )


#if Q3_DEBUG
	E3GlobalsPtr	theGlobals = E3Globals_Get();
	static TQ3Boolean	sIsMakingListHead = kQ3False;
	
	if (sIsMakingListHead == kQ3True)
	{
		theObject->next = theObject;
		theObject->prev = theObject;
		theObject->stackCrawl = NULL;
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
				theGlobals->listHead = E3ClassTree::CreateInstance ( kQ3ObjectTypeRoot,
					kQ3False, NULL ) ;
				sIsMakingListHead = kQ3False;
				Q3_REQUIRE_OR_RESULT( theGlobals->listHead != NULL, kQ3Failure );
			}
			Q3_ASSERT( theGlobals->listHead->GetClass ()->GetType () == kQ3ObjectTypeRoot ) ;
			
			// insert the new node between the list header and last normal node
			theObject->next = theGlobals->listHead;
			theObject->prev = PREVLINK( theGlobals->listHead );
			NEXTLINK( PREVLINK( theGlobals->listHead ) ) = theObject;
			PREVLINK( theGlobals->listHead ) = theObject;
			
			// Record a stack crawl if possible
			theObject->stackCrawl = E3StackCrawl_New();
		}
		else
		{
			theObject->next = NULL;
			theObject->prev = NULL;
			theObject->stackCrawl = NULL;
		}
	}
#endif
	
	theObject->theSet = NULL;
	
	return kQ3Success;
}





//=============================================================================
//      e3root_duplicate : Root object duplicate method.
//-----------------------------------------------------------------------------
TQ3Status
e3root_duplicate(TQ3Object fromObject,     const void *fromPrivateData,
						 TQ3Object toObject, void *toPrivateData)
{
#pragma unused( fromObject, fromPrivateData )
	TQ3Status	q3status;
	
	q3status = e3root_new( toObject, toPrivateData, NULL );
	
	if (q3status == kQ3Success)
	{
		OpaqueTQ3Object*	fromInstanceData = (OpaqueTQ3Object*) fromPrivateData;
		OpaqueTQ3Object*	toInstanceData = (OpaqueTQ3Object*) toPrivateData;
		
		
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





//=============================================================================
//      e3root_delete : Root delete method.
//-----------------------------------------------------------------------------
void
e3root_delete( TQ3Object theObject, void *privateData )
{
	OpaqueTQ3Object	*instanceData = (OpaqueTQ3Object *) privateData;

	Q3_ASSERT(privateData == theObject);
//	Q3_ASSERT(privateData == theObject->FindLeafInstanceData () ) ;
	
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
	theObject->DestroyInstance () ;
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

		case kQ3XMethodTypeNewObjectClass:
			theMethod = (TQ3XFunctionPointer) e3root_new_class_info;
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
	{
	// Register the classes
	TQ3Status qd3dStatus = E3ClassTree::RegisterClass (
											kQ3ObjectTypeQuesa,
											OpaqueTQ3Object::eClassType,
											kQ3ClassNameRoot,
											e3root_metahandler,
											sizeof ( OpaqueTQ3Object )
											);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (		kQ3ClassNameShared,
												e3shared_metahandler,
												E3Shared ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (		kQ3ClassNameShape,
												e3shape_metahandler,
												E3Shape ) ;

	return qd3dStatus ;
	}




//=============================================================================
//      e3main_unregistercoreclasses : Unregister the core object classes.
//-----------------------------------------------------------------------------
static TQ3Status
e3main_unregistercoreclasses(void)
	{
	// Unregister the classes. Unregistering the root class will
	// unregister everything else anchored underneath it.
	return E3ClassTree::UnregisterClass ( kQ3ObjectTypeRoot, kQ3True ) ;
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
			qd3dStatus = E3Camera::RegisterClass();

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
		E3ClassTree::Dump () ;
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
		qd3dStatus = E3Camera::UnregisterClass();
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
		E3ClassTree::Destroy () ;



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
	{
	// Initialise a return value
	*objectClassType = kQ3ObjectTypeInvalid ;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectClassString ) ;
	if ( theClass == NULL )
		return kQ3Failure ;



	// Return the type
	*objectClassType = theClass->GetType () ;
	return kQ3Success ;
	}





//=============================================================================
//      E3ObjectHierarchy_GetStringFromType : Find the name for a class.
//-----------------------------------------------------------------------------
//		Note : Given a class type, returns the name of the class.
//-----------------------------------------------------------------------------
TQ3Status
E3ObjectHierarchy_GetStringFromType(TQ3ObjectType objectClassType, TQ3ObjectClassNameString objectClassString)
	{
	// Initialise a return value
	strcpy ( objectClassString, "" ) ;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectClassType ) ;
	if ( theClass == NULL )
		return kQ3Failure ;



	// Return the name
	strcpy ( objectClassString, theClass->GetName () ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3ObjectHierarchy_IsTypeRegistered : Test if a class is registered.
//-----------------------------------------------------------------------------
TQ3Boolean
E3ObjectHierarchy_IsTypeRegistered(TQ3ObjectType objectClassType)
	{
	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectClassType ) ;

	return theClass != NULL ? kQ3True : kQ3False ;
	}





//=============================================================================
//      E3ObjectHierarchy_IsNameRegistered : Test if a class is registered.
//-----------------------------------------------------------------------------
TQ3Boolean
E3ObjectHierarchy_IsNameRegistered(const char *objectClassName)
	{
	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectClassName ) ;

	return theClass != NULL ? kQ3True : kQ3False ;
	}





//=============================================================================
//      E3ObjectHierarchy_GetSubClassData : Get the sub-classes of a class.
//-----------------------------------------------------------------------------
//		Note :	Fills in subClassData with the number and class types of all of
//				the subclasses immediately below the specified class.
//-----------------------------------------------------------------------------
TQ3Status
E3ObjectHierarchy_GetSubClassData(TQ3ObjectType objectClassType, TQ3SubClassData *subClassData)
	{
	// Initialise a return value
	subClassData->numClasses = 0;
	subClassData->classTypes = NULL;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectClassType ) ;
	if ( theClass == NULL )
		return kQ3Failure ;



	// Allocate the array for the child classes
	Q3Memory_Clear(subClassData, sizeof(TQ3SubClassData));

	TQ3Uns32 numChildren = theClass->GetNumChildren () ;
	if (numChildren != 0)
		{
		subClassData->classTypes = (TQ3ObjectType *) Q3Memory_Allocate(sizeof(TQ3ObjectType) * numChildren);
		if (subClassData->classTypes == NULL)
			return(kQ3Failure);

		subClassData->numClasses = numChildren;
		}



	// Collect the child classes	
	for ( TQ3Uns32 n = 0 ; n < numChildren ; ++n )
		{
		// Get the child
		E3ClassInfoPtr theChild = theClass->GetChild ( n ) ;
		Q3_ASSERT_VALID_PTR(theChild);
		
		
		// Grab the type
		subClassData->classTypes [ n ] = theChild->GetType () ;
		}

	return kQ3Success ;
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
OpaqueTQ3Object::Dispose ( void )
	{

	// Dispose of the object
	 ( (E3Root*) GetClass () )->disposeMethod ( this ) ;

	return kQ3Success ;
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
	{
	// Reject draw context and view objects
	//
	// Note - the current Quesa implementation does not support
	// duplicating file storage objects, as opposed to memory storage objects.
	//
	// When this is functionality is added, the kQ3SharedTypeStorage
	// test should be removed.
	//
	// If this causes your app problems, please contact Jose Cruanyes
	// or James Walker to discuss a fix.
	if ( E3View_IsOfMyClass ( theObject ) // Can't access E3View here as it isn't in a header file (yet) 
	|| Q3_OBJECT_IS_CLASS ( theObject, E3DrawContext )
	|| ( Q3_OBJECT_IS_CLASS ( theObject, E3Storage ) && ! Q3_OBJECT_IS_CLASS ( theObject, E3MemoryStorage ) ) )
		{
		E3ErrorManager_PostError(kQ3ErrorInvalidObjectType, kQ3False);
		return(NULL);
		}



	// Duplicate the object
	return theObject->DuplicateInstance () ;
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
	{
	// Get the 'method'
	TQ3Boolean isDrawable = (TQ3Boolean) ( NULL != theObject->GetClass ()->GetMethod (
									kQ3XMethodTypeObjectIsDrawable ) ) ;

	return isDrawable ;
	}





//=============================================================================
//      E3Object_IsWritable : Determine if an object is writeable.
//-----------------------------------------------------------------------------
//		Note : We return as the object has a kQ3XMethodTypeObjectWrite method.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Object_IsWritable(TQ3Object theObject, TQ3FileObject theFile)
	{
#pragma unused(theFile)



	// Get the method
	TQ3XObjectWriteMethod writeMethod = (TQ3XObjectWriteMethod) theObject->GetClass ()->GetMethod ( kQ3XMethodTypeObjectWrite ) ;



	// Return as the method exists or not
	return (TQ3Boolean) ( writeMethod != NULL ) ;
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
	{
	// Get the type of the object below the root object
	return theObject->GetObjectType ( kQ3ObjectTypeRoot ) ;
	}





//=============================================================================
//      E3Object_IsType : Is an object an instance of a particular type?
//-----------------------------------------------------------------------------
TQ3Boolean
E3Object_IsType(TQ3Object theObject, TQ3ObjectType theType)
	{
	// Return as the object is an instance of the type
	return theObject->GetClass ()->IsType ( theType ) ;
	}





//=============================================================================
//      E3Object_AddElement : Add an element to an object
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::AddElement ( TQ3ElementType theType, const void *theData )
	{
	// Translate public type to internal type for set elements
	if ( theType == kQ3ElementTypeSet )
		theType = kQ3ObjectTypeSetElement;



	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		return Q3Set_Add ( (TQ3SetObject) this, theType, theData ) ;
		
	// otherwise use the set within the instance data
	if ( theSet == NULL )
		{
		theSet = Q3Set_New () ;
		
		if ( theSet == NULL )
			return kQ3Failure ;
		}
	
	TQ3Status qd3dStatus = Q3Set_Add ( theSet, theType, theData ) ;
	
	
	if ( ( qd3dStatus != kQ3Failure ) && Q3_OBJECT_IS_CLASS ( this, E3Shared ) )
		( (E3Shared*) this )->Edited () ;
	
	return qd3dStatus ;
	}





//=============================================================================
//      E3Object_GetElement : Get element data from an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::GetElement ( TQ3ElementType theType, void *theData )
	{
	// Translate public type to internal type for set elements
	if (theType == kQ3ElementTypeSet)
		theType = kQ3ObjectTypeSetElement;



	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		return Q3Set_Get ( (TQ3SetObject) this, theType, theData ) ;
	
	// otherwise use the set within the instance data
	
	if ( theSet == NULL )
		return kQ3Failure ;
	
	return Q3Set_Get ( theSet, theType, (void*)theData ) ;
	}





//=============================================================================
//      E3Object_ContainsElement : Test whether an object contains an element.
//-----------------------------------------------------------------------------
TQ3Boolean
OpaqueTQ3Object::ContainsElement ( TQ3ElementType theType )
	{
	// Translate public type to internal type for set elements
	if ( theType == kQ3ElementTypeSet )
		theType = kQ3ObjectTypeSetElement ;



	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		return Q3Set_Contains ( (TQ3SetObject) this, theType ) ;
	
	// otherwise use the set within the instance data		
		
	if ( theSet != NULL )
		return Q3Set_Contains ( theSet, theType ) ;
		
		
	
	return kQ3False ;
	}





//=============================================================================
//      E3Object_GetNextElementType : Get the next element type in an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::GetNextElementType ( TQ3ElementType *theType )
	{
	TQ3Status qd3dStatus = kQ3Failure ;
	
	
	
	// Translate public type to internal type for set elements
	if ( *theType == kQ3ElementTypeSet )
		*theType = kQ3ObjectTypeSetElement ;



	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		qd3dStatus = Q3Set_GetNextElementType ( (TQ3SetObject) this, theType ) ;
	else	// otherwise use the set within the instance data
		{
		if ( theSet == NULL )
			{
			*theType = kQ3ElementTypeNone ;
			return kQ3Success ;
			}
		
		qd3dStatus = Q3Set_GetNextElementType ( theSet, theType ) ;
		}



	if ( *theType == kQ3ObjectTypeSetElement )
		*theType = kQ3ElementTypeSet ;
	
	
	return qd3dStatus ;
	}





//=============================================================================
//      E3Object_EmptyElements : Remove all elements from an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::EmptyElements ( void )
	{
	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		return Q3Set_Empty ( (TQ3SetObject) this ) ;
	
	// otherwise use the set within the instance data
	if ( theSet == NULL )
		return kQ3Success ;
	
	TQ3Status qd3dStatus = Q3Set_Empty ( theSet ) ;
	
	
	if ( ( qd3dStatus != kQ3Failure ) && Q3_OBJECT_IS_CLASS ( this, E3Shared ) )
		( (E3Shared*) this )->Edited () ;
	
	
	return qd3dStatus ;
	}





//=============================================================================
//      E3Object_ClearElement : Remove a specific type of element from an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::ClearElement ( TQ3ElementType theType )
	{
	// Translate public type to internal type for set elements
	if (theType == kQ3ElementTypeSet)
		theType = kQ3ObjectTypeSetElement;



	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		return Q3Set_Clear ( (TQ3SetObject) this, theType ) ;
	
	// otherwise use the set within the instance data
	if ( theSet == NULL )
		return kQ3Success ;
		
	TQ3Status qd3dStatus = Q3Set_Clear ( theSet, theType ) ;
		
		
	if ( ( qd3dStatus != kQ3Failure ) && Q3_OBJECT_IS_CLASS ( this, E3Shared ) )
		( (E3Shared*) this )->Edited () ;
	
	
	return qd3dStatus ;
	}





//=============================================================================
//      E3Object_GetSet : Get the set of an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::GetSet ( TQ3SetObject *set )
	{
	E3Shared_Acquire ( set, theSet ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Object_SetSet : Set the set of an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::SetSet ( TQ3SetObject set )
	{
	E3Shared_Replace ( &theSet, set ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Shared_IsOfMyClass : Check if object pointer is valid and of type shared
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3ObjectTypeShared )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Boolean
E3Shared_IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3Shared ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3Shared_GetType : Get the type of a shared object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Shared_GetType(TQ3SharedObject sharedObject)
	{
	// Return the type
	return sharedObject->GetObjectType ( kQ3ObjectTypeShared ) ;
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
E3Shared*
E3Shared::GetReference ( void )
	{
	// Increment the reference count and return the object. Note that we
	// return the object passed in: this is OK since we're not declared
	// to return a different object.
	refCount++;
	Q3_ASSERT(refCount >= 2);

	return this ;
	}





//=============================================================================
//      E3Shared_IsReferenced : Is a reference shared?
//-----------------------------------------------------------------------------
//		Note :	We return kQ3True if there is more than one reference to the
//				object, and kQ3False if there is one reference to the object.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Shared::IsReferenced ( void )
	{
	// Return as the reference count is greater than 1
	return ( (TQ3Boolean) ( refCount > 1 ) ) ;
	}





//=============================================================================
//      E3Shared_GetReferenceCount : Return the reference count.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Shared::GetReferenceCount ( void )
	{
	// Return the reference count
	return refCount ;
	}





//=============================================================================
//      E3Shared_GetEditIndex : Return the edit index of a shared object.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Shared::GetEditIndex ( void )
	{
	// Return the edit index
	return editIndex ;
	}





//=============================================================================
//      E3Shared::SetEditIndex : Set the edit index of a shared object.
//-----------------------------------------------------------------------------
void
E3Shared::SetEditIndex( TQ3Uns32 inIndex )
{
	editIndex = inIndex;
}





//=============================================================================
//      E3Shared_Edited : Increase the edit index of an object.
//-----------------------------------------------------------------------------
TQ3Status
E3Shared::Edited ( void )
	{
	// Increment the edit index
	++editIndex ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Shape_IsOfMyClass : Check if object pointer is valid and of type shape
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3SharedTypeShape )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Boolean
E3Shape_IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3Shape ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3Shape_GetType : Get the type of a shape object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Shape_GetType(TQ3ShapeObject theShape)
	{
	// Return the type
	return theShape->GetObjectType ( kQ3SharedTypeShape ) ;
	}





//=============================================================================
//      E3Shape_GetSet : Get the set for a shape.
//-----------------------------------------------------------------------------
TQ3Status
E3Shape_GetSet(TQ3ShapeObject theShape, TQ3SetObject *theSet)
	{
	*theSet = NULL ;
	return theShape->GetElement ( kQ3ElementTypeSet, theSet ) ;
	}





//=============================================================================
//      E3Shape_SetSet : Set the set for a shape.
//-----------------------------------------------------------------------------
TQ3Status
E3Shape_SetSet(TQ3ShapeObject theShape, TQ3SetObject theSet)
	{
	if ( theSet == NULL )
		return theShape->ClearElement ( kQ3ElementTypeSet ) ;
	
	return theShape->AddElement ( kQ3ElementTypeSet, &theSet ) ;
	}





//=============================================================================
//      E3Shape_SubmitElements : Submit custom elements of a shape.
//-----------------------------------------------------------------------------
//		This will be called by E3FFW_3DMF_TraverseObject to submit
//		any custom elements that might be attached to the shape.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::SubmitElements ( TQ3ViewObject inView )
	{
	// Use the shape's set, if any.
	if ( theSet == NULL )
		return kQ3Success ;
	
	
	return ( (E3Set*) theSet )->SubmitElements ( inView ) ;
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





//=============================================================================
//      E3Bitmap_GetBit : Get a bit from a bitmap.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Bitmap_GetBit(const TQ3Bitmap *theBitmap, TQ3Uns32 x, TQ3Uns32 y)
{	TQ3Uns8			*bytePtr, theByte, bitShift;
	TQ3Boolean		theState;



	// Locate the byte we need
	bytePtr  = theBitmap->image + (y * theBitmap->rowBytes);
	bytePtr += (x / 8);
	theByte  = *bytePtr;



	// Locate the bit we need
	bitShift = (TQ3Uns8) (x % 8);
	if (theBitmap->bitOrder == kQ3EndianBig)
		theState = ((theByte >> (7 - bitShift)) & 0x01) != 0 ? kQ3True : kQ3False;
	else
		theState = ((theByte >>  (   bitShift)) & 0x01) != 0 ? kQ3True : kQ3False;

	return(theState);
}





//=============================================================================
//      E3Bitmap_SetBit : Set a bit within a bitmap.
//-----------------------------------------------------------------------------
TQ3Status
E3Bitmap_SetBit(TQ3Bitmap *theBitmap, TQ3Uns32 x, TQ3Uns32 y, TQ3Boolean theState)
{	TQ3Uns8		*bytePtr, theByte, bitShift, byteMask;



	// Locate the byte we need
	bytePtr  = theBitmap->image + (y * theBitmap->rowBytes);
	bytePtr += (x / 8);



	// Prepare the mask
	bitShift = (TQ3Uns8) (x % 8);

	if (theBitmap->bitOrder == kQ3EndianBig)
		byteMask = (1 << (7 - bitShift));
	else
		byteMask = (1 << (    bitShift));



	// Update the byte
	theByte = *bytePtr;
	
	if (theState)
		theByte |=  byteMask;
	else
		theByte &= ~byteMask;
	
	*bytePtr = theByte;

	return(kQ3Success);
}







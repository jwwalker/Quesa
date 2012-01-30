/*  NAME:
        QD3DMain.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
#include "E3Main.h"
#include "E3CustomElements.h"
#include "E3Set.h"
#include "E3View.h"





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
//      Q3Initialize : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
Q3Initialize(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Initialize());
}





//=============================================================================
//      Q3Exit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Exit(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Exit());
}





//=============================================================================
//      Q3IsInitialized : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3IsInitialized(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3IsInitialized());
}





//=============================================================================
//      Q3GetVersion : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRevision), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRevision), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on majorRevision
		return(kQ3Failure);

	if (0) // Further checks on minorRevision
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GetVersion(majorRevision, minorRevision));
}





//=============================================================================
//      Q3GetReleaseVersion : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GetReleaseVersion(TQ3Uns32 *releaseRevision)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(releaseRevision), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on releaseRevision
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GetReleaseVersion(releaseRevision));
}





//=============================================================================
//      Q3LogMessage : Quesa API entry point.
//-----------------------------------------------------------------------------
void
Q3LogMessage( const char* inMessage )
{
	E3LogMessage( inMessage );
}




//=============================================================================
//      Q3ObjectHierarchy_GetTypeFromString : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
Q3ObjectHierarchy_GetTypeFromString(const TQ3ObjectClassNameString objectClassString, TQ3ObjectType *objectClassType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(objectClassType), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClassString
		return(kQ3Failure);

	if (0) // Further checks on objectClassType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ObjectHierarchy_GetTypeFromString(objectClassString, objectClassType));
}





//=============================================================================
//      Q3ObjectHierarchy_GetStringFromType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ObjectHierarchy_GetStringFromType(TQ3ObjectType objectClassType, TQ3ObjectClassNameString objectClassString)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClassType
		return(kQ3Failure);

	if (0) // Further checks on objectClassString
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ObjectHierarchy_GetStringFromType(objectClassType, objectClassString));
}





//=============================================================================
//      Q3ObjectHierarchy_IsTypeRegistered : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3ObjectHierarchy_IsTypeRegistered(TQ3ObjectType objectClassType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClassType
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ObjectHierarchy_IsTypeRegistered(objectClassType));
}





//=============================================================================
//      Q3ObjectHierarchy_IsNameRegistered : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3ObjectHierarchy_IsNameRegistered(const char *objectClassName)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(objectClassName), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClassName
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ObjectHierarchy_IsNameRegistered(objectClassName));
}





//=============================================================================
//      Q3ObjectHierarchy_GetSubClassData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ObjectHierarchy_GetSubClassData(TQ3ObjectType objectClassType, TQ3SubClassData *subClassData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(subClassData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on objectClassType
		return(kQ3Failure);

	if (0) // Further checks on subClassData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ObjectHierarchy_GetSubClassData(objectClassType, subClassData));
}





//=============================================================================
//      Q3ObjectHierarchy_EmptySubClassData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ObjectHierarchy_EmptySubClassData(TQ3SubClassData *subClassData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(subClassData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on subClassData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ObjectHierarchy_EmptySubClassData(subClassData));
}





//=============================================================================
//      Q3Object_Dispose : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
Q3Object_Dispose(TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object != NULL, kQ3Failure );
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid () , kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->Dispose () ;
}





//=============================================================================
//      Q3Object_CleanDispose : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_CleanDispose(TQ3Object *object)
{


	// Release build checks
	if (object != NULL && *object != NULL)
		Q3_REQUIRE_OR_RESULT( (*object)->IsObjectValid () , kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Object_CleanDispose(object));
}





//=============================================================================
//      Q3Object_Duplicate : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Object
Q3Object_Duplicate(TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object != NULL, NULL ) ;
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid () , NULL ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Object_Duplicate(object));
}





//=============================================================================
//      Q3Object_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_Submit(TQ3Object object, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Object_Submit(object, view));
}





//=============================================================================
//      Q3Object_IsDrawable : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Object_IsDrawable(TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid () , kQ3False ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Object_IsDrawable(object));
}





//=============================================================================
//      Q3Object_IsWritable : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Object_IsWritable(TQ3Object object, TQ3FileObject theFile)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid () , kQ3False ) ;
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theFile, kQ3SharedTypeFile), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3False);

	if (0) // Further checks on theFile
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Object_IsWritable(object, theFile));
}





//=============================================================================
//      Q3Object_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3Object_GetType(TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3ObjectTypeInvalid ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Object_GetType(object));
}





//=============================================================================
//      Q3Object_GetLeafType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3Object_GetLeafType(TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3ObjectTypeInvalid ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->GetLeafType () ;
}





//=============================================================================
//      Q3Object_IsType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Object_IsType(TQ3Object object, TQ3ObjectType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object != NULL, kQ3False);
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid () , kQ3False ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3False);

	if (0) // Further checks on theType
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Object_IsType(object, theType));
}




//=============================================================================
//      Q3Object_AddElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_AddElement(TQ3Object object, TQ3ElementType theType, const void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->AddElement ( theType, data ) ;
}





//=============================================================================
//      Q3Object_GetElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_GetElement(TQ3Object object, TQ3ElementType theType, void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->GetElement ( theType, data ) ;
}





//=============================================================================
//      Q3Object_ContainsElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Object_ContainsElement(TQ3Object object, TQ3ElementType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid () , kQ3False ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3False);

	if (0) // Further checks on theType
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->ContainsElement ( theType ) ;
}





//=============================================================================
//      Q3Object_GetNextElementType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_GetNextElementType(TQ3Object object, TQ3ElementType *theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theType), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->GetNextElementType ( theType ) ;
}





//=============================================================================
//      Q3Object_EmptyElements : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_EmptyElements(TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->EmptyElements () ;
}





//=============================================================================
//      Q3Object_ClearElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_ClearElement(TQ3Object object, TQ3ElementType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->ClearElement ( theType ) ;
}





//=============================================================================
//      Q3Object_GetProperty : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_GetProperty(
	TQ3Object				object,
	TQ3ObjectType			propType,
	TQ3Uns32				bufferSize,
	TQ3Uns32*				actualSize,
	void*					buffer )
{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;
	
	
	
	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->GetProperty( propType, bufferSize, actualSize, buffer );
}





//=============================================================================
//      Q3Object_RemoveProperty : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_RemoveProperty(
	TQ3Object				object,
	TQ3ObjectType			propType )
{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;
	

	
	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->RemoveProperty( propType );
}





//=============================================================================
//      Q3Object_SetProperty : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_SetProperty(
	TQ3Object				object,
	TQ3ObjectType			propType,
	TQ3Uns32				dataSize,
	const void*				data )
{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	

	
	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->SetProperty( propType, dataSize, data );
}





/*!
	@function	Q3Object_IterateProperties
	@abstract	Iterate over the properties of an object.
	@discussion	The user callback will be called for each property type that
				exists on the object.  If the object has no properties, the
				function will simply return kQ3Success without ever calling the
				callback.
	@param		object			A Quesa object.
	@param		userIterator	A callback function.
	@param		userData		A pointer to be passed back to the callback
								(use NULL if none is needed).
	@result		Success or failure of the operation.
*/
TQ3Status
Q3Object_IterateProperties(
	TQ3Object				object,
	TQ3PropertyIterator		userIterator,
	void*					userData )
{
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( userIterator != NULL, kQ3Failure ) ;

	
	// Call the bottleneck
	E3System_Bottleneck();


	
	return object->IterateProperties( userIterator, userData );
}




//=============================================================================
//      Q3Object_GetSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_GetSet ( TQ3Object object, TQ3SetObject* set )
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(set), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->GetSet ( set ) ;
}





//=============================================================================
//      Q3Object_SetSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Object_SetSet ( TQ3Object object, TQ3SetObject set )
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( object->IsObjectValid (), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( ( set == NULL ) || ( set->IsObjectValid () && Q3_OBJECT_IS_CLASS ( set, E3Set ) ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return object->SetSet ( set ) ;
}





//=============================================================================
//      Q3Shared_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
Q3Shared_GetType(TQ3SharedObject sharedObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Shared_IsOfMyClass ( sharedObject ), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedObject
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Shared_GetType(sharedObject));
}





//=============================================================================
//      Q3Shared_GetReference : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3SharedObject
Q3Shared_GetReference(TQ3SharedObject sharedObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Shared_IsOfMyClass ( sharedObject ), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedObject
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shared*) sharedObject )->GetReference () ;
}





//=============================================================================
//      Q3Shared_IsReferenced : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Shared_IsReferenced(TQ3SharedObject sharedObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Shared_IsOfMyClass ( sharedObject ), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedObject
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shared*) sharedObject )->IsReferenced () ;
}





//=============================================================================
//      Q3Shared_GetReferenceCount : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Uns32
Q3Shared_GetReferenceCount( TQ3SharedObject               sharedObject )
{

	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Shared_IsOfMyClass ( sharedObject ), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedObject
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shared*) sharedObject )->GetReferenceCount () ;
}
#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      Q3Shared_GetEditIndex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3Shared_GetEditIndex(TQ3SharedObject sharedObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Shared_IsOfMyClass ( sharedObject ), 0);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedObject
		return(0);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shared*) sharedObject )->GetEditIndex () ;
}




/*!
	@function
		Q3Shared_SetEditIndex
	
	@abstract	Set the edit index of a shared object.
	
	@discussion	This function should be used even more rarely than
				Q3Shared_Edited.  It was added to solve a specific problem:
				Caching information in a custom element attached to an object
				without changing the edit index of the object.
				
				 <em>This function is not available in QD3D.</em>
	
	@param		inObject		A shared object to update.
	@param		inEditIndex		New edit index.
*/
void
Q3Shared_SetEditIndex(
	TQ3SharedObject inObject,
	TQ3Uns32 inEditIndex
)
{
	// Release build checks
	Q3_REQUIRE( E3Shared_IsOfMyClass ( inObject ) );
	



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	( (E3Shared*) inObject )->SetEditIndex( inEditIndex );
}




//=============================================================================
//      Q3Shared_Edited : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shared_Edited(TQ3SharedObject sharedObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Shared_IsOfMyClass ( sharedObject ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedObject
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shared*) sharedObject )->Edited () ;
}



//=============================================================================
//      Q3Shared_SetEditIndexLocked : Quesa API entry point.
//-----------------------------------------------------------------------------
void
Q3Shared_SetEditIndexLocked( TQ3SharedObject inObject, TQ3Boolean inIsLocked )
{
	// Release build checks
	Q3_REQUIRE( E3Shared_IsOfMyClass ( inObject ) );
	
	
	
	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	( (E3Shared*) inObject )->SetEditIndexLocked( inIsLocked );
}



//=============================================================================
//      Q3Shared_IsEditIndexLocked : Quesa API entry point.
TQ3Boolean
Q3Shared_IsEditIndexLocked( TQ3SharedObject inObject )
{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Shared_IsOfMyClass ( inObject ), kQ3False );
	
	
	
	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Shared*) inObject )->IsEditIndexLocked();
}



//=============================================================================
//      Q3Shape_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
Q3Shape_GetType(TQ3ShapeObject shape)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Shape_IsOfMyClass ( shape ), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Shape_GetType(shape));
}





//=============================================================================
//      Q3Shape_GetSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shape_GetSet(TQ3ShapeObject shape, TQ3SetObject *theSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Shape_IsOfMyClass ( shape ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Shape_GetSet(shape, theSet));
}





//=============================================================================
//      Q3Shape_SetSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shape_SetSet(TQ3ShapeObject shape, TQ3SetObject theSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Shape_IsOfMyClass ( shape ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(((theSet == NULL) || ( theSet->IsObjectValid () && Q3_OBJECT_IS_CLASS ( theSet, E3Set ) ) ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Shape_SetSet(shape, theSet));
}





//=============================================================================
//      Q3Shape_AddElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shape_AddElement(TQ3ShapeObject shape, TQ3ElementType theType, const void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( shape != NULL, kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( shape->IsObjectValid () , kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( Q3_OBJECT_IS_CLASS ( shape, E3Shape ) || Q3_OBJECT_IS_CLASS ( shape, E3Set ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return shape->AddElement ( theType, data ) ;
}





//=============================================================================
//      Q3Shape_GetElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shape_GetElement(TQ3ShapeObject shape, TQ3ElementType theType, void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( shape != NULL, kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( shape->IsObjectValid () , kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( Q3_OBJECT_IS_CLASS ( shape, E3Shape ) || Q3_OBJECT_IS_CLASS ( shape, E3Set ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return shape->GetElement ( theType, data ) ;
}





//=============================================================================
//      Q3Shape_ContainsElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Shape_ContainsElement(TQ3ShapeObject shape, TQ3ElementType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( shape != NULL, kQ3False ) ;
	Q3_REQUIRE_OR_RESULT( shape->IsObjectValid () , kQ3False ) ;
	Q3_REQUIRE_OR_RESULT( Q3_OBJECT_IS_CLASS ( shape, E3Shape ) || Q3_OBJECT_IS_CLASS ( shape, E3Set ), kQ3False ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3False);

	if (0) // Further checks on theType
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return shape->ContainsElement ( theType ) ;
}





//=============================================================================
//      Q3Shape_GetNextElementType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shape_GetNextElementType(TQ3ShapeObject shape, TQ3ElementType *theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( shape != NULL, kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( shape->IsObjectValid () , kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( Q3_OBJECT_IS_CLASS ( shape, E3Shape ) || Q3_OBJECT_IS_CLASS ( shape, E3Set ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theType), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return shape->GetNextElementType ( theType ) ;
}





//=============================================================================
//      Q3Shape_EmptyElements : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shape_EmptyElements(TQ3ShapeObject shape)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( shape != NULL, kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( shape->IsObjectValid () , kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( Q3_OBJECT_IS_CLASS ( shape, E3Shape ) || Q3_OBJECT_IS_CLASS ( shape, E3Set ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return shape->EmptyElements () ;
}





//=============================================================================
//      Q3Shape_ClearElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shape_ClearElement(TQ3ShapeObject shape, TQ3ElementType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( shape != NULL, kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( shape->IsObjectValid () , kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( Q3_OBJECT_IS_CLASS ( shape, E3Shape ) || Q3_OBJECT_IS_CLASS ( shape, E3Set ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);

	if (0) // Further checks on theType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return shape->ClearElement ( theType ) ;
}





//=============================================================================
//      Q3Bitmap_Empty : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
Q3Bitmap_Empty(TQ3Bitmap *bitmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bitmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on bitmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Bitmap_Empty(bitmap));
}





//=============================================================================
//      Q3Bitmap_GetImageSize : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3Bitmap_GetImageSize(TQ3Uns32 width, TQ3Uns32 height)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on width
		return(0);

	if (0) // Further checks on height
		return(0);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Bitmap_GetImageSize(width, height));
}





//=============================================================================
//      Q3Bitmap_GetBit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Bitmap_GetBit(const TQ3Bitmap *theBitmap, TQ3Uns32 x, TQ3Uns32 y)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theBitmap), kQ3False);
	Q3_REQUIRE_OR_RESULT(x < theBitmap->width,    kQ3False);
	Q3_REQUIRE_OR_RESULT(y < theBitmap->height,   kQ3False);
	


	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theBitmap
		return(kQ3False);

	if (0) // Further checks on x
		return(kQ3False);

	if (0) // Further checks on y
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Bitmap_GetBit(theBitmap, x, y));
}





//=============================================================================
//      Q3Bitmap_SetBit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Bitmap_SetBit(TQ3Bitmap *theBitmap, TQ3Uns32 x, TQ3Uns32 y, TQ3Boolean theState)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theBitmap), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(x < theBitmap->width,    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(y < theBitmap->height,   kQ3Failure);
	


	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theBitmap
		return(kQ3Failure);

	if (0) // Further checks on x
		return(kQ3Failure);

	if (0) // Further checks on y
		return(kQ3Failure);

	if (0) // Further checks on theState
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Bitmap_SetBit(theBitmap, x, y, theState));
}






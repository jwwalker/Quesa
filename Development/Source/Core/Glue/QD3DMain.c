/*  NAME:
        QD3DMain.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
#include "E3Main.h"





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
//      Q3ObjectHierarchy_GetTypeFromString : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
Q3ObjectHierarchy_GetTypeFromString(TQ3ObjectClassNameString objectClassString, TQ3ObjectType *objectClassType)
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
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Object_Dispose(object));
}





//=============================================================================
//      Q3Object_Duplicate : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Object
Q3Object_Duplicate(TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, NULL);



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
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3False);



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
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3False);
	Q3_REQUIRE_OR_RESULT(theFile->quesaTag == kQ3ObjectTypeQuesa, kQ3False);
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
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);



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
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Object_GetLeafType(object));
}





//=============================================================================
//      Q3Object_IsType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Object_IsType(TQ3Object object, TQ3ObjectType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3False);



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
//      Q3Shared_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
Q3Shared_GetType(TQ3SharedObject sharedObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(sharedObject->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(sharedObject, kQ3ObjectTypeShared), kQ3ObjectTypeInvalid);



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
	Q3_REQUIRE_OR_RESULT(sharedObject->quesaTag == kQ3ObjectTypeQuesa, NULL);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(sharedObject, kQ3ObjectTypeShared), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedObject
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Shared_GetReference(sharedObject));
}





//=============================================================================
//      Q3Shared_IsReferenced : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Shared_IsReferenced(TQ3SharedObject sharedObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(sharedObject->quesaTag == kQ3ObjectTypeQuesa, kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(sharedObject, kQ3ObjectTypeShared), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedObject
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Shared_IsReferenced(sharedObject));
}





//=============================================================================
//      Q3Shared_GetEditIndex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3Shared_GetEditIndex(TQ3SharedObject sharedObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(sharedObject->quesaTag == kQ3ObjectTypeQuesa,       0);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(sharedObject, kQ3ObjectTypeShared), 0);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedObject
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Shared_GetEditIndex(sharedObject));
}





//=============================================================================
//      Q3Shared_Edited : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shared_Edited(TQ3SharedObject sharedObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(sharedObject->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(sharedObject, kQ3ObjectTypeShared), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on sharedObject
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Shared_Edited(sharedObject));
}





//=============================================================================
//      Q3Shape_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
Q3Shape_GetType(TQ3ShapeObject shape)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(shape->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shape, kQ3SharedTypeShape), kQ3ObjectTypeInvalid);



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
	Q3_REQUIRE_OR_RESULT(shape->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shape, kQ3SharedTypeShape), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(shape->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shape, kQ3SharedTypeShape), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(theSet->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theSet, kQ3SharedTypeSet), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(shape->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shape, kQ3SharedTypeShape) ||
		Q3Object_IsType(shape, kQ3SharedTypeSet), kQ3Failure);
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
	return(E3Shape_AddElement(shape, theType, data));
}





//=============================================================================
//      Q3Shape_GetElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shape_GetElement(TQ3ShapeObject shape, TQ3ElementType theType, void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(shape->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shape, kQ3SharedTypeShape) ||
		Q3Object_IsType(shape, kQ3SharedTypeSet), kQ3Failure);
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
	return(E3Shape_GetElement(shape, theType, data));
}





//=============================================================================
//      Q3Shape_ContainsElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Shape_ContainsElement(TQ3ShapeObject shape, TQ3ElementType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(shape->quesaTag == kQ3ObjectTypeQuesa, kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shape, kQ3SharedTypeShape) ||
		Q3Object_IsType(shape, kQ3SharedTypeSet), kQ3Failure);



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
	return(E3Shape_ContainsElement(shape, theType));
}





//=============================================================================
//      Q3Shape_GetNextElementType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shape_GetNextElementType(TQ3ShapeObject shape, TQ3ElementType *theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(shape->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shape, kQ3SharedTypeShape) ||
		Q3Object_IsType(shape, kQ3SharedTypeSet), kQ3Failure);
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
	return(E3Shape_GetNextElementType(shape, theType));
}





//=============================================================================
//      Q3Shape_EmptyElements : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shape_EmptyElements(TQ3ShapeObject shape)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(shape->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shape, kQ3SharedTypeShape) ||
		Q3Object_IsType(shape, kQ3SharedTypeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shape
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Shape_EmptyElements(shape));
}





//=============================================================================
//      Q3Shape_ClearElement : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Shape_ClearElement(TQ3ShapeObject shape, TQ3ElementType theType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(shape->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shape, kQ3SharedTypeShape) ||
		Q3Object_IsType(shape, kQ3SharedTypeSet), kQ3Failure);



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
	return(E3Shape_ClearElement(shape, theType));
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
		return(NULL);

	if (0) // Further checks on height
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Bitmap_GetImageSize(width, height));
}






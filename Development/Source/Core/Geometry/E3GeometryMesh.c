/*  NAME:
		E3GeometryMesh.c

	DESCRIPTION:
		Implementation of Quesa Mesh geometry class.

	COPYRIGHT:
		Quesa Copyright © 1999-2002, Quesa Developers.

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
#include "E3View.h"
#include "E3Geometry.h"
#include "E3GeometryMesh.h"
#include "E3ArrayOrList.h"
#include "E3Pool.h"





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
//		Unlike C++, C does not distinguish between various kinds of type casts.
//		Use E3_CONST_CAST to cast away const-ness.
//-----------------------------------------------------------------------------
#ifndef E3_CONST_CAST
#define E3_CONST_CAST(cast, value) ((cast) (value))
#endif





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Mesh and part data types
typedef struct TE3MeshPartData TE3MeshPartData;
typedef struct TE3MeshVertexData TE3MeshVertexData;
typedef struct TE3MeshContourData TE3MeshContourData;
typedef struct TE3MeshFaceData TE3MeshFaceData;
typedef struct TE3MeshData TE3MeshData;



// Internal references to part data types - distinct types
typedef struct TE3MeshPartIntRefOpaque* TE3MeshPartIntRef;
typedef struct TE3MeshVertexIntRefOpaque* TE3MeshVertexIntRef;
typedef struct TE3MeshFaceIntRefOpaque* TE3MeshFaceIntRef;



// External references to part data types - distinct types
typedef TE3MeshPartData** TE3MeshPartExtRef;
typedef TQ3MeshVertex TE3MeshVertexExtRef;
typedef TQ3MeshEdge TE3MeshEdgeExtRef;
typedef TQ3MeshContour TE3MeshContourExtRef;
typedef TQ3MeshFace TE3MeshFaceExtRef;
typedef TQ3MeshComponent TE3MeshComponentExtRef;



// TE3MeshPartData
struct TE3MeshPartData {
	TE3MeshPartData**				partDataHdl;
};



// TE3MeshPartDataPtr
typedef TE3MeshPartData* TE3MeshPartDataPtr;

E3POOL_DECLARE(TE3MeshPartDataPtr, e3meshPartDataPtr, static);
E3POOL_DEFINE(TE3MeshPartDataPtr, e3meshPartDataPtr, static, 16);



// TE3MeshVertexData
struct TE3MeshVertexData {
	TE3MeshPartData					partData;				// base class
	TQ3Point3D 						point;
	TQ3AttributeSet 				attributeSet;
};

E3ARRAY_DECLARE(TE3MeshVertexData, e3meshVertexData, static);
E3ARRAY_DEFINE(TE3MeshVertexData, e3meshVertexData, static);

E3LIST_DECLARE(TE3MeshVertexData, e3meshVertexData, static);
E3LIST_DEFINE(TE3MeshVertexData, e3meshVertexData, static);

E3ARRAY_OR_LIST_DECLARE(TE3MeshVertexData, e3meshVertexData, static);
E3ARRAY_OR_LIST_DEFINE(TE3MeshVertexData, e3meshVertexData, static);



// TE3MeshVertexIntRef
E3ARRAY_DECLARE(TE3MeshVertexIntRef, e3meshVertexIntRef, static);
E3ARRAY_DEFINE(TE3MeshVertexIntRef, e3meshVertexIntRef, static);



// TE3MeshContourData
struct TE3MeshContourData {
	TE3MeshPartData					partData;				// base class
	TE3MeshFaceIntRef				containerFaceIntRef;
	TE3MeshVertexIntRefArray		vertexIntRefArray;
};

E3ARRAY_DECLARE(TE3MeshContourData, e3meshContourData, static);
E3ARRAY_DEFINE(TE3MeshContourData, e3meshContourData, static);

E3LIST_DECLARE(TE3MeshContourData, e3meshContourData, static);
E3LIST_DEFINE(TE3MeshContourData, e3meshContourData, static);

E3ARRAY_OR_LIST_DECLARE(TE3MeshContourData, e3meshContourData, static);
E3ARRAY_OR_LIST_DEFINE(TE3MeshContourData, e3meshContourData, static);



// TE3MeshFaceData
struct TE3MeshFaceData {
	TE3MeshPartData					partData;				// base class
	TE3MeshContourDataArrayOrList	contourDataArrayOrList;
	TQ3AttributeSet 				attributeSet;
};

E3ARRAY_DECLARE(TE3MeshFaceData, e3meshFaceData, static);
E3ARRAY_DEFINE(TE3MeshFaceData, e3meshFaceData, static);

E3LIST_DECLARE(TE3MeshFaceData, e3meshFaceData, static);
E3LIST_DEFINE(TE3MeshFaceData, e3meshFaceData, static);

E3ARRAY_OR_LIST_DECLARE(TE3MeshFaceData, e3meshFaceData, static);
E3ARRAY_OR_LIST_DEFINE(TE3MeshFaceData, e3meshFaceData, static);



// TE3MeshData
struct TE3MeshData {
	// Note:	In order for tags to work in the partDataPtrPool,
	//			the first member of a mesh structure must *not* be,
	//			in effect, a handle (TE3MeshData**) referring back to
	//			this mesh!
	TE3MeshPartDataPtrPool			partDataPtrPool;
	TE3MeshVertexDataArrayOrList	vertexDataArrayOrList;
	TE3MeshFaceDataArrayOrList		faceDataArrayOrList;
	TQ3AttributeSet 				attributeSet;
};





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3meshPartData_AllocateMasterPointerInMesh :	Allocate master pointer
//														to part data.
//-----------------------------------------------------------------------------
//		Errors :	If unable to allocate master pointer (out of memory),
//					return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshPartData_AllocateMasterPointerInMesh(TE3MeshPartData* partDataPtr, TE3MeshData* meshDataPtr)
{
	TE3MeshPartData** partDataHdl;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	
	if (partDataPtr->partDataHdl == NULL)
	{
		// Allocate master pointer from mesh's pool
			// Note: The type of items in the pool is, in effect, a union:
			//
			//		union {
			//			TE3MeshPartData*	partDataPtr;
			//			TE3MeshData*		meshDataPtr;
			//		};
			//
			// Regular items in the pool are of type TE3MeshPartData* and tags in
			// the pool are of type TE3MeshData*.
			//
			// Neverthless, for simplicity the type is declared to be simply
			// TE3MeshPartData* and we use a type cast to coerce one pointer
			// type to the other.
		if ((partDataHdl = e3meshPartDataPtrPool_AllocateTagged(&meshDataPtr->partDataPtrPool, (const TE3MeshPartDataPtr*) &meshDataPtr)) == NULL)
			goto failure;

		// Link master pointer and part data
		*partDataHdl = partDataPtr;
		partDataPtr->partDataHdl = partDataHdl;
	}

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshPartData_FreeMasterPointerInMesh :	Free master pointer, if any,
//													to part data.
//-----------------------------------------------------------------------------
//		Warning :	Use this function with care! Freeing a master pointer to a
//					part that is still referenced, either internally by another
//					part of the mesh or externally by the application, will
//					result in a fatal error. 
//-----------------------------------------------------------------------------
static
void
e3meshPartData_FreeMasterPointerInMesh(TE3MeshPartData* partDataPtr, TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	e3meshPartDataPtrPool_Free(&meshDataPtr->partDataPtrPool, &partDataPtr->partDataHdl);
}





//=============================================================================
//      e3meshPartData_IntRefInMesh : Return internal reference to part.
//-----------------------------------------------------------------------------
//		Note : If unable to get internal reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
static
TE3MeshPartIntRef
e3meshPartData_IntRefInMesh(TE3MeshPartData* partDataPtr, TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// Allocate master pointer to part data
	if (e3meshPartData_AllocateMasterPointerInMesh(partDataPtr, meshDataPtr) == kQ3Failure)
		goto failure;

	// Return handle to part data
	return((TE3MeshPartIntRef) partDataPtr->partDataHdl);
	
failure:

	return(NULL);
}





//=============================================================================
//      e3meshPartData_ExtRefInMesh : Return external reference to part.
//-----------------------------------------------------------------------------
//		Note : If unable to get external reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
static
TE3MeshPartExtRef
e3meshPartData_ExtRefInMesh(TE3MeshPartData* partDataPtr, TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// Allocate master pointer to part data
	if (e3meshPartData_AllocateMasterPointerInMesh(partDataPtr, meshDataPtr) == kQ3Failure)
		goto failure;

	return(partDataPtr->partDataHdl);
	
failure:

	return(NULL);
}





//=============================================================================
//      e3meshPartData_Create : TE3MeshPartData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshPartData_Create(TE3MeshPartData* partDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// Initialize pointer to master pointer BEFORE calling e3meshPartData_AllocateMasterPointerInMesh
	partDataPtr->partDataHdl = NULL;
	
	// If requested, allocate master pointer to part data
	if (isReferenced)
	{
		if (e3meshPartData_AllocateMasterPointerInMesh(partDataPtr, meshDataPtr) == kQ3Failure)
			goto failure;
	}

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshPartData_Destroy : TE3MeshPartData destructor.
//-----------------------------------------------------------------------------
static
void
e3meshPartData_Destroy(TE3MeshPartData* partDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partDataPtr);

	// If part data has master pointer, clear
	if (partDataPtr->partDataHdl)
		*partDataPtr->partDataHdl = NULL;
}





//=============================================================================
//      e3meshPartData_Relocate : Relocate part data.
//-----------------------------------------------------------------------------
static
void
e3meshPartData_Relocate(TE3MeshPartData* partDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partDataPtr);

	// If part data has master pointer, update
	if (partDataPtr->partDataHdl)
		*partDataPtr->partDataHdl = partDataPtr;
}





//=============================================================================
//      e3meshPartDataPtr_IsMeshDataPtr :	Return if this part data pointer is
//											actually a mesh data pointer.
//-----------------------------------------------------------------------------
//		Warning :	In contrast to other functions, the parameter for this
//					function is a pointer to a pointer, not a pointer!
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Boolean
e3meshPartDataPtr_IsMeshDataPtr(const TE3MeshPartDataPtr* partDataHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partDataHdl);

	// A null pointer is not a pointer to mesh data	
	if ((*partDataHdl) == NULL)
		return(kQ3False);

	// If this pointer points to a pointer that points back to this pointer,
	// it is a pointer to part data -- not mesh data	
	if ((*partDataHdl)->partDataHdl == partDataHdl)
		return(kQ3False);
	
	return(kQ3True);
}





//=============================================================================
//      e3meshPartIntRef_DataInMesh : Return data for this part.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshPartData*
e3meshPartIntRef_DataInMesh(TE3MeshPartIntRef partIntRef, const TE3MeshData* meshDataPtr)
{
#pragma unused(meshDataPtr)

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partIntRef);

	return(* (TE3MeshPartData**) partIntRef);
}





//=============================================================================
//      e3meshPartIntRef_IsEqualTo :	Return if two part internal references
//										are equal.
//-----------------------------------------------------------------------------
//		Warning :	In contrast to other functions, the parameters for this
//					function are pointers to internal references, not internal
//					references!
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3meshPartIntRef_IsEqualTo(const TE3MeshPartIntRef* partIntRefPtr1, const TE3MeshPartIntRef* partIntRefPtr2)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partIntRefPtr1);
	Q3_ASSERT_VALID_PTR(partIntRefPtr2);

	return(*partIntRefPtr1 == *partIntRefPtr2 ? kQ3True : kQ3False);
}





//=============================================================================
//      e3meshPartExtRef_Data : Return data for this part.
//-----------------------------------------------------------------------------
//		Note : If part deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshPartData*
e3meshPartExtRef_Data(TE3MeshPartExtRef partExtRef)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partExtRef);

	return(*partExtRef);
}





//=============================================================================
//      e3meshPartExtRef_MeshData : Return data for mesh having this part.
//-----------------------------------------------------------------------------
static
TE3MeshData*
e3meshPartExtRef_MeshData(TE3MeshPartExtRef partExtRef)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partExtRef);

	return(* (TE3MeshData**) e3meshPartDataPtrPoolItem_Tag(partExtRef, e3meshPartDataPtr_IsMeshDataPtr));
}





//=============================================================================
//      e3meshVertexData_IntRefInMesh : Return internal reference to vertex.
//-----------------------------------------------------------------------------
//		Note : If unable to get internal reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshVertexIntRef
e3meshVertexData_IntRefInMesh(TE3MeshVertexData* vertexDataPtr, TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshVertexIntRef,
		e3meshPartData_IntRefInMesh(
			E3_UP_CAST(TE3MeshPartData*, vertexDataPtr),
			meshDataPtr)));
}





//=============================================================================
//      e3meshVertexData_ExtRefInMesh :	Return external reference to vertex.
//-----------------------------------------------------------------------------
//		Note : If unable to get external reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
static
TE3MeshVertexExtRef
e3meshVertexData_ExtRefInMesh(TE3MeshVertexData* vertexDataPtr, TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshVertexExtRef,
		e3meshPartData_ExtRefInMesh(
			E3_UP_CAST(TE3MeshPartData*, vertexDataPtr),
			meshDataPtr)));
}





//=============================================================================
//      e3meshVertexData_Create : TE3MeshVertexData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshVertexData_Create(TE3MeshVertexData* vertexDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced, const TQ3Vertex3D* vertexPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(vertexPtr);

	// Create part data
	if (e3meshPartData_Create(E3_UP_CAST(TE3MeshPartData*, vertexDataPtr), meshDataPtr, isReferenced) == kQ3Failure)
		goto failure;

	// Initialize point
	vertexDataPtr->point = vertexPtr->point;

	// Acquire attribute set
	E3Shared_Acquire(&vertexDataPtr->attributeSet, vertexPtr->attributeSet);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshVertexData_Destroy : TE3MeshVertexData destructor.
//-----------------------------------------------------------------------------
static
void
e3meshVertexData_Destroy(TE3MeshVertexData* vertexDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexDataPtr);

	// Release attribute set
	E3Object_DisposeAndForget(vertexDataPtr->attributeSet);

	// Destroy part data
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, vertexDataPtr));
}





//=============================================================================
//      e3meshVertexData_Relocate : Relocate vertex data.
//-----------------------------------------------------------------------------
static
void
e3meshVertexData_Relocate(TE3MeshVertexData* vertexDataPtr)
{
	e3meshPartData_Relocate(E3_UP_CAST(TE3MeshPartData*, vertexDataPtr));
}





//=============================================================================
//      e3meshVertexIntRef_DataInMesh : Return data for this vertex.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshVertexData*
e3meshVertexIntRef_DataInMesh(TE3MeshVertexIntRef vertexIntRef, const TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshVertexData*,
		e3meshPartIntRef_DataInMesh(
			E3_UP_CAST(TE3MeshPartIntRef, vertexIntRef),
			meshDataPtr)));
}





//=============================================================================
//      e3meshVertexExtRef_Data : Return data for this vertex.
//-----------------------------------------------------------------------------
//		Note : If vertex deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshVertexData*
e3meshVertexExtRef_Data(TE3MeshVertexExtRef vertexExtRef)
{
	return(E3_DOWN_CAST(TE3MeshVertexData*,
		e3meshPartExtRef_Data(
			E3_UP_CAST(TE3MeshPartExtRef, vertexExtRef))));
}





//=============================================================================
//      e3meshVertexExtRef_MeshData : Return data for mesh having this vertex.
//-----------------------------------------------------------------------------
static
TE3MeshData*
e3meshVertexExtRef_MeshData(TE3MeshVertexExtRef vertexExtRef)
{
	return(e3meshPartExtRef_MeshData(E3_UP_CAST(TE3MeshPartExtRef, vertexExtRef)));
}





//=============================================================================
//      e3meshContourData_ExtRefInMesh : Return external reference to contour.
//-----------------------------------------------------------------------------
//		Note : If unable to get external reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshContourExtRef
e3meshContourData_ExtRefInMesh(TE3MeshContourData* contourDataPtr, TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshContourExtRef,
		e3meshPartData_ExtRefInMesh(
			E3_UP_CAST(TE3MeshPartData*, contourDataPtr),
			meshDataPtr)));
}





//=============================================================================
//      e3meshContourData_Create : TE3MeshContourData constructor.
//-----------------------------------------------------------------------------
//		Errors :	If any vertex deleted or unable to create (out of memory),
//					return kQ3Failure.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs more than once in succession,
//				repeated occurrences are eliminated. If after this elimination
//				less than 2 vertices remain, the creation fails. Thus we ensure
//				that every edge connects two distinct vertices.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshContourData_Create(TE3MeshContourData* contourDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced, TE3MeshFaceIntRef containerFaceIntRef, TQ3Uns32 numVertices, const TE3MeshVertexExtRef* vertexExtRefs)
{
	TQ3Uns32 effectiveNumVertices;
	TQ3Uns32 i;
	TE3MeshVertexIntRef* vertexIntRefPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(containerFaceIntRef);
	Q3_ASSERT(numVertices > 0);
	Q3_ASSERT_VALID_PTR(vertexExtRefs);

	// Check for deleted vertices; count effective number of vertices (excluding repeats)
	effectiveNumVertices = 0;
	for (i = 0; i < numVertices; ++i)
	{
		if (e3meshVertexExtRef_Data(vertexExtRefs[i]) == NULL)
			goto failure_1;
		if (vertexExtRefs[i] != vertexExtRefs[i > 0 ? i-1 : numVertices-1])
			++effectiveNumVertices;
	}
	if (effectiveNumVertices < 2)
		goto failure_2;

	// Create part data
	if (e3meshPartData_Create(E3_UP_CAST(TE3MeshPartData*, contourDataPtr), meshDataPtr, isReferenced) == kQ3Failure)
		goto failure_3;

	// Initialize container face internal reference
	contourDataPtr->containerFaceIntRef = containerFaceIntRef;

	// Create uninitialized vertex internal reference array
	if (e3meshVertexIntRefArray_Create(&contourDataPtr->vertexIntRefArray, effectiveNumVertices, NULL) == kQ3Failure)
		goto failure_4;

	// Initialize vertex internal reference array
	vertexIntRefPtr = e3meshVertexIntRefArray_FirstItem(&contourDataPtr->vertexIntRefArray);
	for (i = 0; i < numVertices; ++i)
	{
		if (vertexExtRefs[i] != vertexExtRefs[i > 0 ? i-1 : numVertices-1])
		{
			TE3MeshVertexData* vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRefs[i]);
			
			*vertexIntRefPtr = e3meshVertexData_IntRefInMesh(vertexDataPtr, meshDataPtr);
			++vertexIntRefPtr;
		}
	}

	return(kQ3Success);

	// Dead code to reverse e3meshVertexIntRefArray_Create
failure_4:

	e3meshPartData_FreeMasterPointerInMesh(E3_UP_CAST(TE3MeshPartData*, contourDataPtr), meshDataPtr);
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, contourDataPtr));
failure_3:

failure_2:

failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshContourData_Destroy : TE3MeshContourData destructor.
//-----------------------------------------------------------------------------
static
void
e3meshContourData_Destroy(TE3MeshContourData* contourDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourDataPtr);

	// Destroy vertex internal reference array
	e3meshVertexIntRefArray_Destroy(&contourDataPtr->vertexIntRefArray, NULL);

	// Destroy part data
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, contourDataPtr));
}





//=============================================================================
//      e3meshContourData_Relocate : Relocate vertex data.
//-----------------------------------------------------------------------------
static
void
e3meshContourData_Relocate(TE3MeshContourData* contourDataPtr)
{
	e3meshPartData_Relocate(E3_UP_CAST(TE3MeshPartData*, contourDataPtr));
}





//=============================================================================
//      e3meshContourData_SetContainerFaceIntRef :	Set internal reference to
//													container face.
//-----------------------------------------------------------------------------
static
void
e3meshContourData_SetContainerFaceIntRef(TE3MeshContourData* contourDataPtr, TE3MeshFaceIntRef containerFaceIntRef)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourDataPtr);
	Q3_ASSERT_VALID_PTR(containerFaceIntRef);

	contourDataPtr->containerFaceIntRef = containerFaceIntRef;
}





//=============================================================================
//      e3meshContourData_ContainerFaceIntRef :	Return internal reference to
//												container face.
//-----------------------------------------------------------------------------
static
TE3MeshFaceIntRef
e3meshContourData_ContainerFaceIntRef(TE3MeshContourData* contourDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourDataPtr);

	return(contourDataPtr->containerFaceIntRef);
}





//=============================================================================
//      e3meshContourData_NumVertices : Return number of vertices in contour.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is counted separately.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3meshContourData_NumVertices(const TE3MeshContourData* contourDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourDataPtr);

	return(e3meshVertexIntRefArray_Length(&contourDataPtr->vertexIntRefArray));
}





//=============================================================================
//      e3meshContourData_HasVertexIntRef : Return if contour has vertex.
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3meshContourData_HasVertexIntRef(const TE3MeshContourData* contourDataPtr, TE3MeshVertexIntRef vertexIntRef)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourDataPtr);
	Q3_ASSERT_VALID_PTR(vertexIntRef);

	return(e3meshVertexIntRefArray_OrForEach(
		&contourDataPtr->vertexIntRefArray,
		E3_DOWN_CAST(TQ3Boolean (*)(const TE3MeshVertexIntRef*, void*), e3meshPartIntRef_IsEqualTo),
		&vertexIntRef));
}





//=============================================================================
//      e3meshContourExtRef_Data : Return data for this contour.
//-----------------------------------------------------------------------------
//		Note : If contour deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshContourData*
e3meshContourExtRef_Data(TE3MeshContourExtRef contourExtRef)
{
	return(E3_DOWN_CAST(TE3MeshContourData*,
		e3meshPartExtRef_Data(
			E3_UP_CAST(TE3MeshPartExtRef, contourExtRef))));
}





//=============================================================================
//      e3meshContourExtRef_MeshData : Return data for mesh having this contour.
//-----------------------------------------------------------------------------
static
TE3MeshData*
e3meshContourExtRef_MeshData(TE3MeshContourExtRef contourExtRef)
{
	return(e3meshPartExtRef_MeshData(E3_UP_CAST(TE3MeshPartExtRef, contourExtRef)));
}





//=============================================================================
//      e3meshFaceData_IntRefInMesh : Return internal reference to face.
//-----------------------------------------------------------------------------
//		Note : If unable to get internal reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshFaceIntRef
e3meshFaceData_IntRefInMesh(TE3MeshFaceData* faceDataPtr, TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshFaceIntRef,
		e3meshPartData_IntRefInMesh(
			E3_UP_CAST(TE3MeshPartData*, faceDataPtr),
			meshDataPtr)));
}





//=============================================================================
//      e3meshFaceData_ExtRefInMesh : Return external reference to face.
//-----------------------------------------------------------------------------
//		Note : If unable to get external reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
static
TE3MeshFaceExtRef
e3meshFaceData_ExtRefInMesh(TE3MeshFaceData* faceDataPtr, TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshFaceExtRef,
		e3meshPartData_ExtRefInMesh(
			E3_UP_CAST(TE3MeshPartData*, faceDataPtr),
			meshDataPtr)));
}





//=============================================================================
//      e3meshFaceData_Create : TE3MeshFaceData constructor.
//-----------------------------------------------------------------------------
//		Errors :	If any vertex deleted or unable to create (out of memory),
//					return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshFaceData_Create(TE3MeshFaceData* faceDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced, TQ3Uns32 numContours, TQ3Uns32* numVerticesPtr, const TE3MeshVertexExtRef** vertexExtRefsPtr, TQ3AttributeSet attributeSet)
{
	TE3MeshFaceIntRef faceIntRef;
	TQ3Uns32 i;
	TE3MeshContourData* contourDataPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(faceDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT(numContours > 0);

	// Create part data, with reference
	if (e3meshPartData_Create(E3_UP_CAST(TE3MeshPartData*, faceDataPtr), meshDataPtr, kQ3True) == kQ3Failure)
		goto failure_1;

	// Get internal reference to face
	if ((faceIntRef = e3meshFaceData_IntRefInMesh(faceDataPtr, meshDataPtr)) == NULL)
		goto failure_2;

	// Create uninitialized contour data array
	if (e3meshContourDataArray_Create(&faceDataPtr->contourDataArrayOrList.array, numContours, NULL) == kQ3Failure)
		goto failure_3;

	// Create each contour data
	for (i = 0, contourDataPtr = e3meshContourDataArray_FirstItem(&faceDataPtr->contourDataArrayOrList.array);
		i < numContours;
		++i, contourDataPtr = e3meshContourDataArray_NextItem(&faceDataPtr->contourDataArrayOrList.array, contourDataPtr))
	{
		// Create contour data, without reference
		if (e3meshContourData_Create(contourDataPtr, meshDataPtr, kQ3False, faceIntRef, numVerticesPtr[i], vertexExtRefsPtr[i]) == kQ3Failure)
			goto failure_4;
	}

	// Acquire attribute set
	E3Shared_Acquire(&faceDataPtr->attributeSet, attributeSet);

	return(kQ3Success);

	// Dead code to reverse e3meshContourData_Create
failure_4:

	while (i-- > 0)
	{
		contourDataPtr = e3meshContourDataArray_PreviousItem(&faceDataPtr->contourDataArrayOrList.array, contourDataPtr);
		e3meshContourData_Destroy(contourDataPtr);
	}
	e3meshContourDataArray_Destroy(&faceDataPtr->contourDataArrayOrList.array, NULL);
failure_3:
	
failure_2:

	e3meshPartData_FreeMasterPointerInMesh(E3_UP_CAST(TE3MeshPartData*, faceDataPtr), meshDataPtr);
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, faceDataPtr));
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshFaceData_Destroy : TE3MeshFaceData destructor.
//-----------------------------------------------------------------------------
static
void
e3meshFaceData_Destroy(TE3MeshFaceData* faceDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(faceDataPtr);

	// Release attribute set
	E3Object_DisposeAndForget(faceDataPtr->attributeSet);

	// Destroy contour data array or list
	e3meshContourDataArrayOrList_Destroy(&faceDataPtr->contourDataArrayOrList, e3meshContourData_Destroy);

	// Destroy part data
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, faceDataPtr));
}





//=============================================================================
//      e3meshFaceData_Relocate : Relocate vertex data.
//-----------------------------------------------------------------------------
static
void
e3meshFaceData_Relocate(TE3MeshFaceData* faceDataPtr)
{
	e3meshPartData_Relocate(E3_UP_CAST(TE3MeshPartData*, faceDataPtr));
}





//=============================================================================
//      e3meshFaceData_NumContours : Return number of contours in face.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3meshFaceData_NumContours(const TE3MeshFaceData* faceDataPtr)
{	TQ3Uns32		numContours;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(faceDataPtr);



	// Get the number of contours
	numContours = e3meshContourDataArrayOrList_Length(&faceDataPtr->contourDataArrayOrList);
	return(numContours);
}





//=============================================================================
//      e3meshFaceData_NumVertices : Return number of vertices in face.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is counted separately.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3meshFaceData_NumVertices(const TE3MeshFaceData* faceDataPtr)
{
	TQ3Uns32 numVertices;
	const TE3MeshContourData* contourDataPtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(faceDataPtr);

	// Number of vertices in face is sum of number of vertices in each contour
	numVertices = 0;
	for (contourDataPtr = e3meshContourDataArrayOrList_FirstItemConst(&faceDataPtr->contourDataArrayOrList);
		contourDataPtr != NULL;
		contourDataPtr = e3meshContourDataArrayOrList_NextItemConst(&faceDataPtr->contourDataArrayOrList, contourDataPtr))
	{
		numVertices += e3meshContourData_NumVertices(contourDataPtr);
	}

	return(numVertices);
}





//=============================================================================
//      e3meshFaceData_HasVertexIntRef : Return if face has vertex.
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3meshFaceData_HasVertexIntRef(const TE3MeshFaceData* faceDataPtr, TE3MeshVertexIntRef vertexIntRef)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(faceDataPtr);
	Q3_ASSERT_VALID_PTR(vertexIntRef);

	return(e3meshContourDataArrayOrList_OrForEach(
		&faceDataPtr->contourDataArrayOrList,
		E3_DOWN_CAST(TQ3Boolean (*)(const TE3MeshContourData*, void*), e3meshContourData_HasVertexIntRef),
		vertexIntRef));
}





//=============================================================================
//      e3meshFaceIntRef_DataInMesh : Return data for this face.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshFaceData*
e3meshFaceIntRef_DataInMesh(TE3MeshFaceIntRef faceIntRef, const TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshFaceData*,
		e3meshPartIntRef_DataInMesh(
			E3_UP_CAST(TE3MeshPartIntRef, faceIntRef),
			meshDataPtr)));
}





//=============================================================================
//      e3meshFaceExtRef_Data : Return data for this face.
//-----------------------------------------------------------------------------
//		Note : If face deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshFaceData*
e3meshFaceExtRef_Data(TE3MeshFaceExtRef faceExtRef)
{
	return(E3_DOWN_CAST(TE3MeshFaceData*,
		e3meshPartExtRef_Data(
			E3_UP_CAST(TE3MeshPartExtRef, faceExtRef))));
}





//=============================================================================
//      e3meshFaceExtRef_MeshData : Return data for mesh having this face.
//-----------------------------------------------------------------------------
static
TE3MeshData*
e3meshFaceExtRef_MeshData(TE3MeshFaceExtRef faceExtRef)
{
	return(e3meshPartExtRef_MeshData(E3_UP_CAST(TE3MeshPartExtRef, faceExtRef)));
}





//=============================================================================
//      e3meshData_Create : TE3MeshData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshData_Create(TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// Create part references pool
	if (e3meshPartDataPtrPool_Create(&meshDataPtr->partDataPtrPool) == kQ3Failure)
		goto failure_1;

	// Create empty vertex data array
	if (e3meshVertexDataArray_Create(&meshDataPtr->vertexDataArrayOrList.array, 0, NULL) == kQ3Failure)
		goto failure_2;

	// Create empty face data array
	if (e3meshFaceDataArray_Create(&meshDataPtr->faceDataArrayOrList.array, 0, NULL) == kQ3Failure)
		goto failure_3;

	// Initialize attribute set
	meshDataPtr->attributeSet = NULL;

	return(kQ3Success);
	
	// Dead code to reverse e3meshFaceDataArray_Create
failure_3:

	e3meshVertexDataArray_Destroy(&meshDataPtr->vertexDataArrayOrList.array, e3meshVertexData_Destroy);
failure_2:

	e3meshPartDataPtrPool_Destroy(&meshDataPtr->partDataPtrPool);
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshData_Destroy : TE3MeshData destructor.
//-----------------------------------------------------------------------------
static
void
e3meshData_Destroy(TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	//	Release attribute set
	E3Object_DisposeAndForget(meshDataPtr->attributeSet);

	// Destroy face data array or list
	e3meshFaceDataArrayOrList_Destroy(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Destroy);

	// Destroy vertex data array or list
	e3meshVertexDataArrayOrList_Destroy(&meshDataPtr->vertexDataArrayOrList, e3meshVertexData_Destroy);

	// Destroy part references pool
	e3meshPartDataPtrPool_Destroy(&meshDataPtr->partDataPtrPool);
}





//=============================================================================
//      e3meshData_NumFaces : Return number of faces in mesh.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3meshData_NumFaces(const TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	return(e3meshFaceDataArrayOrList_Length(&meshDataPtr->faceDataArrayOrList));
}





//=============================================================================
//      e3meshData_NumContours : Return number of contours in mesh.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3meshData_NumContours(const TE3MeshData* meshDataPtr)
{
	TQ3Uns32 numContours;
	const TE3MeshFaceData* faceDataPtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// Number of contours in mesh is sum of number of contours in each face
	numContours = 0;
	for (faceDataPtr = e3meshFaceDataArrayOrList_FirstItemConst(&meshDataPtr->faceDataArrayOrList);
		faceDataPtr != NULL;
		faceDataPtr = e3meshFaceDataArrayOrList_NextItemConst(&meshDataPtr->faceDataArrayOrList, faceDataPtr))
	{
		numContours += e3meshFaceData_NumContours(faceDataPtr);
	}

	return(numContours);
}





//=============================================================================
//      e3meshData_NumVertices : Return number of vertices in mesh.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3meshData_NumVertices(const TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	return(e3meshVertexDataArrayOrList_Length(&meshDataPtr->vertexDataArrayOrList));
}





//=============================================================================
//      e3meshIterator_Initialize : TQ3MeshIterator partial constructor.
//-----------------------------------------------------------------------------
#pragma mark -
static
void
e3meshIterator_Initialize(TQ3MeshIterator* iteratorPtr, TE3MeshData* meshDataPtr, const char* iteratorKind)
{
	// Save mesh
	iteratorPtr->var4.field1 = meshDataPtr;

	// Save iterator kind
	strncpy(iteratorPtr->var4.field2, iteratorKind, 4);
	
	// Initialize other fields
	iteratorPtr->var1 =
	iteratorPtr->var2 =
	iteratorPtr->var3 = NULL;
}





//=============================================================================
//      e3geom_mesh_new : TE3MeshData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3geom_mesh_new(TQ3Object theObject, void *privateData, const void *paramData)
{
#pragma unused(theObject)
#pragma unused(paramData)

	return(e3meshData_Create(E3_DOWN_CAST(TE3MeshData*, privateData)));
}





//=============================================================================
//      e3geom_mesh_delete : TE3MeshData destructor.
//-----------------------------------------------------------------------------
static
void
e3geom_mesh_delete(TQ3Object theObject, void *privateData)
{
#pragma unused(theObject)

	e3meshData_Destroy(E3_DOWN_CAST(TE3MeshData*, privateData));
}





//=============================================================================
//      e3geom_mesh_duplicate : Mesh duplicate method.
//-----------------------------------------------------------------------------
static
TQ3Status
e3geom_mesh_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TE3MeshData	*fromInstanceData = (const TE3MeshData *) fromPrivateData;
	TE3MeshData			*toInstanceData   = (TE3MeshData *)       toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = kQ3Success;



	// Handle failure
	if (qd3dStatus != kQ3Success)
		;

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_mesh_cache_new : Mesh cache new method.
//-----------------------------------------------------------------------------
static
TQ3Object
e3geom_mesh_cache_new(TQ3ViewObject view, TQ3GeometryObject mesh, const TE3MeshData* meshDataPtr)
{
#pragma unused(view)
	TQ3GeometryObject polyhedron;
	TQ3PolyhedronData polyhedronData;	
	const TE3MeshFaceData* faceDataPtr;
	const TE3MeshVertexData* firstMeshVertexDataPtr;
	const TE3MeshVertexIntRef* vertexIntRefPtr;
	TQ3Uns32 i, k;
	


	// Assume unable to create polyhedron
	polyhedron = NULL;
	
	// Allocate memory for polyhedron vertices
	polyhedronData.numVertices = e3meshData_NumVertices(meshDataPtr);
	if (polyhedronData.numVertices > 0)
	{
		if ((polyhedronData.vertices = (TQ3Vertex3D*) Q3Memory_Allocate(polyhedronData.numVertices * sizeof(TQ3Vertex3D))) == NULL)
			goto cleanup_1;
	}
	else
		polyhedronData.vertices = NULL;
	
	// Allocate memory for polyhedron edges
	polyhedronData.numEdges = 0;
	polyhedronData.edges = NULL;
	
	// Allocate memory for polyhedron triangles
	polyhedronData.numTriangles = 0;
	for (faceDataPtr = e3meshFaceDataArrayOrList_FirstItemConst(&meshDataPtr->faceDataArrayOrList);
		faceDataPtr != NULL;
		faceDataPtr = e3meshFaceDataArrayOrList_NextItemConst(&meshDataPtr->faceDataArrayOrList, faceDataPtr))
	{
		if (e3meshFaceData_NumContours(faceDataPtr) == 1)
			polyhedronData.numTriangles += e3meshFaceData_NumVertices(faceDataPtr) - 2;
	}
	if (polyhedronData.numTriangles > 0)
	{
		if ((polyhedronData.triangles = (TQ3PolyhedronTriangleData*) Q3Memory_Allocate(polyhedronData.numTriangles * sizeof(TQ3PolyhedronTriangleData))) == NULL)
			goto cleanup_3;
	}
	else
		polyhedronData.triangles = NULL;

	// Use array of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3meshVertexDataArrayOrList_UseArray(E3_CONST_CAST(TE3MeshVertexDataArrayOrList*, &meshDataPtr->vertexDataArrayOrList), e3meshVertexData_Relocate) == kQ3Failure)
		goto cleanup_4;
	
	// Get first vertex data
	firstMeshVertexDataPtr = e3meshVertexDataArray_FirstItemConst(&meshDataPtr->vertexDataArrayOrList.array);

	// Initialize the vertices
	for (i = 0; i < polyhedronData.numVertices; ++i)
	{
		polyhedronData.vertices[i].point = firstMeshVertexDataPtr[i].point;
		E3Shared_Acquire(&polyhedronData.vertices[i].attributeSet, firstMeshVertexDataPtr[i].attributeSet);
	}
	
	// Initialize the triangles
	k = 0;
	for (faceDataPtr = e3meshFaceDataArrayOrList_FirstItemConst(&meshDataPtr->faceDataArrayOrList);
		faceDataPtr != NULL;
		faceDataPtr = e3meshFaceDataArrayOrList_NextItemConst(&meshDataPtr->faceDataArrayOrList, faceDataPtr))
	{
		if (e3meshFaceData_NumContours(faceDataPtr) == 1)
		{
			const TE3MeshContourData* contourDataPtr = e3meshContourDataArrayOrList_FirstItemConst(&faceDataPtr->contourDataArrayOrList);
			TQ3Uns32 numberOfContourVertices = e3meshContourData_NumVertices(contourDataPtr);
			
			for (vertexIntRefPtr = e3meshVertexIntRefArray_FirstItemConst(&contourDataPtr->vertexIntRefArray), i = 0;
				vertexIntRefPtr != NULL;
				vertexIntRefPtr = e3meshVertexIntRefArray_NextItemConst(&contourDataPtr->vertexIntRefArray, vertexIntRefPtr), ++i)
			{
				TE3MeshVertexData* vertexDataPtr = e3meshVertexIntRef_DataInMesh(*vertexIntRefPtr, meshDataPtr);
				TQ3Uns32 vertexIndex = e3meshVertexDataArray_ItemIndex(&meshDataPtr->vertexDataArrayOrList.array, vertexDataPtr);
				TQ3Uns32 vertexIndex0, vertexIndex1, vertexIndex2;
				
				switch (i)
				{
				case 0:
					vertexIndex0 = vertexIndex;
					break;
				case 1:
					vertexIndex2 = vertexIndex;
					break;
				default:
					vertexIndex1 = vertexIndex2;
					vertexIndex2 = vertexIndex;
					polyhedronData.triangles[k].vertexIndices[0] = vertexIndex0;
					polyhedronData.triangles[k].vertexIndices[1] = vertexIndex1;
					polyhedronData.triangles[k].vertexIndices[2] = vertexIndex2;
					polyhedronData.triangles[k].edgeFlag = kQ3PolyhedronEdge12;
					if (i == 2)
						polyhedronData.triangles[k].edgeFlag |= kQ3PolyhedronEdge01;
					if (i == numberOfContourVertices-1)
						polyhedronData.triangles[k].edgeFlag |= kQ3PolyhedronEdge20;
					polyhedronData.triangles[k].triangleAttributeSet = faceDataPtr->attributeSet;
					++k;
					break;
				}
					
			}
		}
	}
	polyhedronData.polyhedronAttributeSet = meshDataPtr->attributeSet;

	// Create the TriMesh and clean up
	polyhedron = Q3Polyhedron_New(&polyhedronData);

cleanup_4:

	Q3Memory_Free(&polyhedronData.triangles);
cleanup_3:

	Q3Memory_Free(&polyhedronData.edges);
cleanup_2:

	Q3Memory_Free(&polyhedronData.vertices);
cleanup_1:

	return(polyhedron);
}





//=============================================================================
//      e3geom_mesh_pick : Mesh picking method.
//-----------------------------------------------------------------------------
static
TQ3Status
e3geom_mesh_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_mesh_bounds : Mesh bounds method.
//-----------------------------------------------------------------------------
static
TQ3Status
e3geom_mesh_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)
#pragma unused(theObject)
	const TE3MeshData			*instanceData = (const TE3MeshData *) objectData;



	// Update the bounds

	return(kQ3Success);
}





//=============================================================================
//      e3geom_mesh_get_attribute : Mesh get attribute set pointer.
//-----------------------------------------------------------------------------
static
TQ3AttributeSet *
e3geom_mesh_get_attribute(TQ3GeometryObject theObject)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(theObject, kQ3GeometryTypeMesh);


	// Return the address of the geometry attribute set
	return(&meshDataPtr->attributeSet);
}





//=============================================================================
//      e3geom_mesh_metahandler : Mesh metahandler.
//-----------------------------------------------------------------------------
static
TQ3XFunctionPointer
e3geom_mesh_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_bounds;
			break;

		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_get_attribute;
			break;
		}

	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryMesh_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryMesh_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeMesh,
											kQ3ClassNameGeometryMesh,
											e3geom_mesh_metahandler,
											sizeof(TE3MeshData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryMesh_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryMesh_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeMesh, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Mesh_New : Create new mesh object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Mesh_New(void)
{
	TQ3Object theObject;


	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeMesh, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3Mesh_DelayUpdates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_DelayUpdates(TQ3GeometryObject mesh)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_ResumeUpdates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_ResumeUpdates(TQ3GeometryObject mesh)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FaceNew : Add new face created from vertices and attribute set.
//-----------------------------------------------------------------------------
//		Errors :	If unable to relocate faces, if unable to insert face item,
//					or if unable to create face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_FaceNew(TQ3GeometryObject mesh, TQ3Uns32 numVertices, const TE3MeshVertexExtRef* vertexExtRefs,
	TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* faceDataPtr;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure_1;

	// Push back new uninitialized face data item into mesh
	if ((faceDataPtr = e3meshFaceDataList_PushBackItem(&meshDataPtr->faceDataArrayOrList.list, NULL)) == NULL)
		goto failure_2;

	// Create face data, with reference
	if (e3meshFaceData_Create(faceDataPtr, meshDataPtr, kQ3True, 1, &numVertices, &vertexExtRefs, attributeSet) == kQ3Failure)
		goto failure_3;

	Q3Shared_Edited(mesh);

	return(e3meshFaceData_ExtRefInMesh(faceDataPtr, meshDataPtr));
	
	// Dead code to reverse e3meshFaceData_Create
failure_3:

	e3meshFaceDataList_EraseItem(&meshDataPtr->faceDataArrayOrList.list, NULL, faceDataPtr);
failure_2:

failure_1:

	return(NULL);
}





//=============================================================================
//      E3Mesh_FaceDelete : Delete face from mesh.
//-----------------------------------------------------------------------------
//		Note : If unable to relocate faces, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_FaceDelete(TQ3GeometryObject mesh, TE3MeshFaceExtRef faceExtRef)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* faceDataPtr;

	// Check face; if face already deleted, return kQ3Success
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto success;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck face (in case relocated)
	faceDataPtr = e3meshFaceExtRef_Data(faceExtRef);

	// Erase face data item
	e3meshFaceDataList_EraseItem(&meshDataPtr->faceDataArrayOrList.list, e3meshFaceData_Destroy,
		faceDataPtr);

	Q3Shared_Edited(mesh);

success:

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FaceToContour :	Append face's contours to container face's, and
//								delete face.
//-----------------------------------------------------------------------------
//		Errors :	If container face deleted, face deleted, unable to relocate
//					faces, or unable to relocate contours, return NULL.
//-----------------------------------------------------------------------------
TE3MeshContourExtRef
E3Mesh_FaceToContour(TQ3GeometryObject mesh, TE3MeshFaceExtRef containerFaceExtRef, TE3MeshFaceExtRef faceExtRef)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* containerFaceDataPtr;
	TE3MeshFaceIntRef containerFaceIntRef;
	TE3MeshFaceData* faceDataPtr;
	TE3MeshContourData* contourDataPtr;
	TE3MeshContourExtRef contourExtRef;

	// Check container face
	if ((containerFaceDataPtr = e3meshFaceExtRef_Data(containerFaceExtRef)) == NULL)
		goto failure;

	// Check face
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck container face (in case relocated)
	containerFaceDataPtr = e3meshFaceExtRef_Data(containerFaceExtRef);

	// Recheck face (in case relocated)
	faceDataPtr = e3meshFaceExtRef_Data(faceExtRef);
	
	// Get internal reference to container face
	if ((containerFaceIntRef = e3meshFaceData_IntRefInMesh(containerFaceDataPtr, meshDataPtr)) == NULL)
		goto failure;

	// Use list of contours in container face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&containerFaceDataPtr->contourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure;

	// Use list of contours in face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&faceDataPtr->contourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure;

	// Get first contour in face
	if ((contourDataPtr = e3meshContourDataList_FirstItem(&faceDataPtr->contourDataArrayOrList.list)) == NULL)
		goto failure;
	if ((contourExtRef = e3meshContourData_ExtRefInMesh(contourDataPtr, meshDataPtr)) == NULL)
		goto failure;

	// For each contour in face, reset container face
	for (contourDataPtr = e3meshContourDataList_FirstItem(&faceDataPtr->contourDataArrayOrList.list);
		contourDataPtr != NULL;
		contourDataPtr = e3meshContourDataList_NextItem(&faceDataPtr->contourDataArrayOrList.list, contourDataPtr))
	{
		// Reset container face
		e3meshContourData_SetContainerFaceIntRef(contourDataPtr, containerFaceIntRef);
	}

	// Splice contours from face into container face
	e3meshContourDataList_SpliceBackList(&containerFaceDataPtr->contourDataArrayOrList.list, &faceDataPtr->contourDataArrayOrList.list);

	// Erase face data item from mesh
	e3meshFaceDataList_EraseItem(&meshDataPtr->faceDataArrayOrList.list, e3meshFaceData_Destroy,
		faceDataPtr);

	Q3Shared_Edited(mesh);

	// Return contour
	return(contourExtRef);
	
failure:

	return(NULL);
}





//=============================================================================
//      E3Mesh_ContourToFace :	Add new face with contour.
//-----------------------------------------------------------------------------
//		Errors :	If contour deleted, unable to relocate faces, unable to
//					insert face item, or unable to relocate contours, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If the specified contour is the only contour in its container
//				face, then this function merely returns that container face.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_ContourToFace(TQ3GeometryObject mesh, TE3MeshContourExtRef contourExtRef)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshContourData* contourDataPtr;
	TE3MeshFaceIntRef containerFaceIntRef;
	TE3MeshFaceData* containerFaceDataPtr;
	TE3MeshFaceData* faceDataPtr;

	// Check contour
	if ((contourDataPtr = e3meshContourExtRef_Data(contourExtRef)) == NULL)
		goto failure_1;
		
	// Get and check container face
	containerFaceIntRef = e3meshContourData_ContainerFaceIntRef(contourDataPtr);
	if ((containerFaceDataPtr = e3meshFaceIntRef_DataInMesh(containerFaceIntRef, meshDataPtr)) == NULL)
		goto failure_2;
	if (e3meshFaceData_NumContours(containerFaceDataPtr) == 1)
		goto success;
	
	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure_3;
		
	// Recheck container face (in case relocated)
	containerFaceIntRef = e3meshContourData_ContainerFaceIntRef(contourDataPtr);
	if ((containerFaceDataPtr = e3meshFaceIntRef_DataInMesh(containerFaceIntRef, meshDataPtr)) == NULL)
		goto failure_4;

	// Push back new uninitialized face data item into mesh
	if ((faceDataPtr = e3meshFaceDataList_PushBackItem(&meshDataPtr->faceDataArrayOrList.list, NULL)) == NULL)
		goto failure_5;

	// Create face data with no contours, with reference
	if (e3meshFaceData_Create(faceDataPtr, meshDataPtr, kQ3True, 0, NULL, NULL, NULL) == kQ3Failure)
		goto failure_6;

	// Use list of contours in container face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&containerFaceDataPtr->contourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure_7;

	// Use list of contours in face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&faceDataPtr->contourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure_8;

	// Splice contour from container face into new face
	e3meshContourDataList_SpliceBackList(&containerFaceDataPtr->contourDataArrayOrList.list, &faceDataPtr->contourDataArrayOrList.list);

	Q3Shared_Edited(mesh);

success:

	// Return face
	return(e3meshFaceData_ExtRefInMesh(faceDataPtr, meshDataPtr));
	
	// Dead code to reverse e3meshContourDataArrayOrList_UseList
failure_8:
	
failure_7:
	
	e3meshFaceData_Destroy(faceDataPtr);
failure_6:

	e3meshFaceDataList_EraseItem(&meshDataPtr->faceDataArrayOrList.list, NULL, faceDataPtr);
failure_5:

failure_4:

failure_3:

failure_2:

failure_1:

	return(NULL);
}





//=============================================================================
//      E3Mesh_VertexNew : Add new vertex created from coordinates and attribute set.
//-----------------------------------------------------------------------------
//		Errors :	If unable to relocate vertices, if unable to insert vertex
//					item, or if unable to create vertex, return NULL.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_VertexNew(TQ3GeometryObject mesh, const TQ3Vertex3D *vertexPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexDataPtr;

	// Use list of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3meshVertexDataArrayOrList_UseList(&meshDataPtr->vertexDataArrayOrList, e3meshVertexData_Relocate) == kQ3Failure)
		goto failure_1;

	// Push back new uninitialized vertex data item into mesh
	if ((vertexDataPtr = e3meshVertexDataList_PushBackItem(&meshDataPtr->vertexDataArrayOrList.list, NULL)) == NULL)
		goto failure_2;

	// Create vertex data, with reference
	if (e3meshVertexData_Create(vertexDataPtr, meshDataPtr, kQ3True, vertexPtr) == kQ3Failure)
		goto failure_3;

	Q3Shared_Edited(mesh);

	return(e3meshVertexData_ExtRefInMesh(vertexDataPtr, meshDataPtr));
	
	// Dead code to reverse e3meshVertexData_Create
failure_3:

	e3meshVertexDataList_EraseItem(&meshDataPtr->vertexDataArrayOrList.list, NULL, vertexDataPtr);
failure_2:

failure_1:

	return(NULL);
}





//=============================================================================
//      E3Mesh_VertexDelete : Delete vertex from mesh.
//-----------------------------------------------------------------------------
//		Note : If unable to relocate vertices, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_VertexDelete(TQ3GeometryObject mesh, TE3MeshVertexExtRef vertexExtRef)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexDataPtr;
	TE3MeshVertexIntRef vertexIntRef;
	TE3MeshFaceData* faceDataPtr;

	// Check vertex; if vertex already deleted, return kQ3Success
	if ((vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRef)) == NULL)
		goto success;

	// Use list of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3meshVertexDataArrayOrList_UseList(&meshDataPtr->vertexDataArrayOrList, e3meshVertexData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck vertex (in case relocated)
	vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRef);
	
	// Get internal reference to vertex
	if ((vertexIntRef = e3meshVertexData_IntRefInMesh(vertexDataPtr, meshDataPtr)) == NULL)
		goto failure;

	// Get first face in mesh
	faceDataPtr = e3meshFaceDataArrayOrList_FirstItem(&meshDataPtr->faceDataArrayOrList);

	// Delete each face having vertex
	while (faceDataPtr != NULL)
	{
		TE3MeshFaceData* markedMeshFaceDataPtr = NULL;

		// Check if face has vertex
		if (e3meshFaceData_HasVertexIntRef(faceDataPtr, vertexIntRef))
		{
			TE3MeshFaceExtRef faceExtRef = NULL;

			// Save face
			if ((faceExtRef = e3meshFaceData_ExtRefInMesh(faceDataPtr, meshDataPtr)) == NULL)
				goto failure;

			// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
			if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
				goto failure;

			// Restore face (in case relocated)
			if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
				goto failure;

			// Mark face for erasure
			markedMeshFaceDataPtr = faceDataPtr;
		}

		// Get next face in mesh
		faceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->faceDataArrayOrList, faceDataPtr);

		// If face marked for erasure, erase face data item
		if (markedMeshFaceDataPtr)
			e3meshFaceDataList_EraseItem(&meshDataPtr->faceDataArrayOrList.list, e3meshFaceData_Destroy,
				markedMeshFaceDataPtr);
	}

	// Erase vertex data item from mesh
	e3meshVertexDataList_EraseItem(&meshDataPtr->vertexDataArrayOrList.list, e3meshVertexData_Destroy,
		vertexDataPtr);

	Q3Shared_Edited(mesh);

success:

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetOrientable : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetOrientable(TQ3GeometryObject mesh, TQ3Boolean *orientable)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetNumComponents : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetNumComponents(TQ3GeometryObject mesh, TQ3Uns32 *numComponents)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstMeshComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshComponentExtRef
E3Mesh_FirstMeshComponent(TQ3GeometryObject mesh, TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshComponentExtRef
E3Mesh_NextMeshComponent(TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetNumFaces : Get number of faces in mesh.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumFaces(TQ3GeometryObject mesh, TQ3Uns32 *numFaces)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);

	// Get number of faces in mesh
	*numFaces = e3meshData_NumFaces(meshDataPtr);

	return(kQ3Success);
}





//=============================================================================
//      E3Mesh_FirstMeshFace : Get first face in mesh.
//-----------------------------------------------------------------------------
//		Errors :	If no first face or unable to create external reference to
//					face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_FirstMeshFace(TQ3GeometryObject mesh, TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* faceDataPtr;
	TE3MeshFaceExtRef faceExtRef;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshDataPtr, "mefa");

	// Get and save first face in mesh
	if ((faceDataPtr = e3meshFaceDataArrayOrList_FirstItem(&meshDataPtr->faceDataArrayOrList)) == NULL)
		goto failure;
	if ((faceExtRef = e3meshFaceData_ExtRefInMesh(faceDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = faceExtRef;

	// Return first face in mesh
	return(faceExtRef);
	
failure:

	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshFace : Get next face in mesh.
//-----------------------------------------------------------------------------
//		Errors :	If iterator ended, current face deleted, no next face or
//					unable to create external reference to face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_NextMeshFace(TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr = iteratorPtr->var4.field1;
	TE3MeshFaceExtRef faceExtRef;
	TE3MeshFaceData* faceDataPtr;

	// Restore and check current face in mesh
	if ((faceExtRef = (TE3MeshFaceExtRef) iteratorPtr->var1) == NULL)
		goto failure;
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;

	// Get and save next face in mesh
	if ((faceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->faceDataArrayOrList, faceDataPtr)) == NULL)
		goto failure;
	if ((faceExtRef = e3meshFaceData_ExtRefInMesh(faceDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = faceExtRef;

	// Return next face in mesh
	return(faceExtRef);
	
failure:

	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetNumEdges : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumEdges(TQ3GeometryObject mesh, TQ3Uns32 *numEdges)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstMeshEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_FirstMeshEdge(TQ3GeometryObject mesh, TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_NextMeshEdge(TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetNumVertices : Get number of vertices in mesh.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumVertices(TQ3GeometryObject mesh, TQ3Uns32 *numVertices)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);

	// Get number of vertices in mesh
	*numVertices = e3meshData_NumVertices(meshDataPtr);

	return(kQ3Success);
}





//=============================================================================
//      E3Mesh_FirstMeshVertex : Get first vertex in mesh.
//-----------------------------------------------------------------------------
//		Errors :	If no first vertex or unable to create external reference
//					to vertex, return NULL.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_FirstMeshVertex(TQ3GeometryObject mesh, TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexDataPtr;
	TE3MeshVertexExtRef vertexExtRef;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshDataPtr, "meve");

	// Get and save first vertex in mesh
	if ((vertexDataPtr = e3meshVertexDataArrayOrList_FirstItem(&meshDataPtr->vertexDataArrayOrList)) == NULL)
		goto failure;
	if ((vertexExtRef = e3meshVertexData_ExtRefInMesh(vertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = vertexExtRef;

	// Return first vertex in mesh
	return(vertexExtRef);
	
failure:

	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshVertex : Get next vertex in mesh.
//-----------------------------------------------------------------------------
//		Errors :	If iterator ended, current vertex deleted, no next vertex or
//					unable to create external reference to vertex, return NULL.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_NextMeshVertex(TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr = iteratorPtr->var4.field1;
	TE3MeshVertexExtRef vertexExtRef;
	TE3MeshVertexData* vertexDataPtr;

	// Restore and check current vertex in mesh
	if ((vertexExtRef = (TE3MeshVertexExtRef) iteratorPtr->var1) == NULL)
		goto failure;
	if ((vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRef)) == NULL)
		goto failure;

	// Get and save next vertex in mesh
	if ((vertexDataPtr = e3meshVertexDataArrayOrList_NextItem(&meshDataPtr->vertexDataArrayOrList, vertexDataPtr)) == NULL)
		goto failure;
	if ((vertexExtRef = e3meshVertexData_ExtRefInMesh(vertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = vertexExtRef;

	// Return next vertex in mesh
	return(vertexExtRef);
	
failure:

	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetNumCorners : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumCorners(TQ3GeometryObject mesh, TQ3Uns32 *numCorners)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetComponentOrientable : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetComponentOrientable(TQ3GeometryObject mesh, TE3MeshComponentExtRef componentExtRef, TQ3Boolean *orientable)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetComponentBoundingBox : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetComponentBoundingBox(TQ3GeometryObject mesh, TE3MeshComponentExtRef componentExtRef, TQ3BoundingBox *boundingBox)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetComponentNumEdges : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetComponentNumEdges(TQ3GeometryObject mesh, TE3MeshComponentExtRef componentExtRef, TQ3Uns32 *numEdges)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstComponentEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_FirstComponentEdge(TE3MeshComponentExtRef componentExtRef, TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextComponentEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_NextComponentEdge(TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetComponentNumVertices : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetComponentNumVertices(TQ3GeometryObject mesh, TE3MeshComponentExtRef componentExtRef, TQ3Uns32 *numVertices)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstComponentVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_FirstComponentVertex(TE3MeshComponentExtRef componentExtRef, TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextComponentVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_NextComponentVertex(TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetFaceIndex : Get index (0..N-1) of face in mesh.
//-----------------------------------------------------------------------------
//		Note : If unable to relocate faces or if face deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetFaceIndex(TQ3GeometryObject mesh, TE3MeshFaceExtRef faceExtRef, TQ3Uns32 *index)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* faceDataPtr;

	// Check face
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;

	// Use array of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseArray(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck face (in case relocated)
	faceDataPtr = e3meshFaceExtRef_Data(faceExtRef);

	// Get index of face
	*index = e3meshFaceDataArray_ItemIndex(&meshDataPtr->faceDataArrayOrList.array, faceDataPtr);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetFacePlaneEquation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetFacePlaneEquation(TQ3GeometryObject mesh, TE3MeshFaceExtRef faceExtRef, TQ3PlaneEquation *planeEquation)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetFaceAttributeSet : Get attribute set for face.
//-----------------------------------------------------------------------------
//		Note : If face deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetFaceAttributeSet(TQ3GeometryObject mesh, TE3MeshFaceExtRef faceExtRef, TQ3AttributeSet *attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* faceDataPtr;

	// Check face
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;

	// Get attribute set
	E3Shared_Acquire(attributeSet, faceDataPtr->attributeSet);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetFaceAttributeSet : Set attribute set for face.
//-----------------------------------------------------------------------------
//		Note : If face deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetFaceAttributeSet(TQ3GeometryObject mesh, TE3MeshFaceExtRef faceExtRef, TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* faceDataPtr;

	// Check face
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;

	// Set attribute set
	E3Shared_Replace(&faceDataPtr->attributeSet, attributeSet);

	Q3Shared_Edited(mesh);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetFaceComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetFaceComponent(TQ3GeometryObject mesh, TE3MeshFaceExtRef faceExtRef, TE3MeshComponentExtRef* componentExtRefPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstFaceFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_FirstFaceFace(TE3MeshFaceExtRef faceExtRef, TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_NextFaceFace(TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetFaceNumContours : Get number of contours in face.
//-----------------------------------------------------------------------------
//		Note : If face deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetFaceNumContours(TQ3GeometryObject mesh, TE3MeshFaceExtRef faceExtRef, TQ3Uns32 *numContours)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* faceDataPtr;

	// Check face
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;

	// Get number of contours in face
	*numContours = e3meshFaceData_NumContours(faceDataPtr);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstFaceContour : Get first contour in face.
//-----------------------------------------------------------------------------
//		Errors :	If face deleted, no first contour or unable to create
//					external reference to contour, return NULL.
//-----------------------------------------------------------------------------
TE3MeshContourExtRef
E3Mesh_FirstFaceContour(TE3MeshFaceExtRef faceExtRef, TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshFaceData* faceDataPtr;
	TE3MeshContourData* contourDataPtr;
	TE3MeshContourExtRef contourExtRef;

	// Get mesh for face
	if ((meshDataPtr = e3meshFaceExtRef_MeshData(faceExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshDataPtr, "fact");

	// Check and save face
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;
	iteratorPtr->var2 = faceExtRef;

	// Get and save first contour in face
	if ((contourDataPtr = e3meshContourDataArrayOrList_FirstItem(&faceDataPtr->contourDataArrayOrList)) == NULL)
		goto failure;
	if ((contourExtRef = e3meshContourData_ExtRefInMesh(contourDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = contourExtRef;

	// Return first contour in face
	return(contourExtRef);
	
failure:

	iteratorPtr->var2 = NULL;
	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceContour : Get next contour in face.
//-----------------------------------------------------------------------------
//		Errors :	If iterator ended, face deleted, current contour deleted,
//					no next contour or unable to create external reference to
//					contour, return NULL.
//-----------------------------------------------------------------------------
TE3MeshContourExtRef
E3Mesh_NextFaceContour(TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr = iteratorPtr->var4.field1;
	TE3MeshFaceExtRef faceExtRef;
	TE3MeshFaceData* faceDataPtr;
	TE3MeshContourExtRef contourExtRef;
	TE3MeshContourData* contourDataPtr;

	// Restore and check face
	if ((faceExtRef = (TE3MeshFaceExtRef) iteratorPtr->var2) == NULL)
		goto failure;
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;

	// Restore and check current contour in face
	if ((contourExtRef = (TE3MeshContourExtRef) iteratorPtr->var1) == NULL)
		goto failure;
	if ((contourDataPtr = e3meshContourExtRef_Data(contourExtRef)) == NULL)
		goto failure;

	// Get and save next contour in face
	if ((contourDataPtr = e3meshContourDataArrayOrList_NextItem(&faceDataPtr->contourDataArrayOrList, contourDataPtr)) == NULL)
		goto failure;
	if ((contourExtRef = e3meshContourData_ExtRefInMesh(contourDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = contourExtRef;

	// Return next contour in face
	return(contourExtRef);
	
failure:

	iteratorPtr->var2 = NULL;
	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstFaceEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_FirstFaceEdge(TE3MeshFaceExtRef faceExtRef, TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_NextFaceEdge(TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetFaceNumVertices : Get number of vertices in face.
//-----------------------------------------------------------------------------
//		Note : If face deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is counted separately.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetFaceNumVertices(TQ3GeometryObject mesh, TE3MeshFaceExtRef faceExtRef, TQ3Uns32 *numVertices)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* faceDataPtr;

	// Check face
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;

	*numVertices = e3meshFaceData_NumVertices(faceDataPtr);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstFaceVertex : Get first vertex in face.
//-----------------------------------------------------------------------------
//		Errors :	If face deleted, no first vertex or unable to create
//					external reference to vertex, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_FirstFaceVertex(TE3MeshFaceExtRef faceExtRef, TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshFaceData* faceDataPtr;
	TE3MeshContourData* contourDataPtr;
	TE3MeshContourExtRef contourExtRef;
	TE3MeshVertexIntRef* vertexIntRefPtr;
	TE3MeshVertexData* vertexDataPtr;
	TE3MeshVertexExtRef vertexExtRef;

	// Get mesh for face
	if ((meshDataPtr = e3meshFaceExtRef_MeshData(faceExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshDataPtr, "fave");

	// Check and save face
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;
	iteratorPtr->var3 = faceExtRef;

	// Get first contour in face
	if ((contourDataPtr = e3meshContourDataArrayOrList_FirstItem(&faceDataPtr->contourDataArrayOrList)) == NULL)
		goto failure;

	// Get first vertex in face
	for (;;)
	{
		// If first vertex in current contour exists, break
		if ((vertexIntRefPtr = e3meshVertexIntRefArray_FirstItem(&contourDataPtr->vertexIntRefArray)) != NULL)
			break;

		// Otherwise, get next contour in face
		if ((contourDataPtr = e3meshContourDataArrayOrList_NextItem(&faceDataPtr->contourDataArrayOrList, contourDataPtr)) == NULL)
			goto failure;
	}

	// Save current contour in face
	if ((contourExtRef = e3meshContourData_ExtRefInMesh(contourDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var2 = contourExtRef;

	// Save first vertex in face
	vertexDataPtr = e3meshVertexIntRef_DataInMesh(*vertexIntRefPtr, meshDataPtr);
	if ((vertexExtRef = e3meshVertexData_ExtRefInMesh(vertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = vertexIntRefPtr;

	// Return first vertex in face
	return(vertexExtRef);
	
failure:

	iteratorPtr->var3 = NULL;
	iteratorPtr->var2 = NULL;
	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceVertex : Get next vertex in face.
//-----------------------------------------------------------------------------
//		Errors :	If iterator ended, face deleted, current vertex deleted,
//					no next vertex or unable to create external reference to
//					vertex, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_NextFaceVertex(TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr = iteratorPtr->var4.field1;
	TE3MeshFaceExtRef faceExtRef;
	TE3MeshFaceData* faceDataPtr;
	TE3MeshContourExtRef contourExtRef;
	TE3MeshContourData* contourDataPtr;
	TE3MeshVertexIntRef* vertexIntRefPtr;
	TE3MeshVertexData* vertexDataPtr;
	TE3MeshVertexExtRef vertexExtRef;

	// Restore and check face
	if ((faceExtRef = (TE3MeshFaceExtRef) iteratorPtr->var3) == NULL)
		goto failure;
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;

	// Restore and check current contour
	if ((contourExtRef = (TE3MeshContourExtRef) iteratorPtr->var2) == NULL)
		goto failure;
	if ((contourDataPtr = e3meshContourExtRef_Data(contourExtRef)) == NULL)
		goto failure;

	// ??? check that contour still belongs to face ???

	// Restore and check current vertex
	if ((vertexIntRefPtr = (TE3MeshVertexIntRef*) iteratorPtr->var1) == NULL)
		goto failure;

	// If next vertex in current contour exists, break
	if ((vertexIntRefPtr = e3meshVertexIntRefArray_NextItem(&contourDataPtr->vertexIntRefArray, vertexIntRefPtr)) != NULL)
		;
	// Otherwise, get next vertex in face
	else
	{
		for (;;)
		{
			// Get next contour in face
			if ((contourDataPtr = e3meshContourDataArrayOrList_NextItem(&faceDataPtr->contourDataArrayOrList, contourDataPtr)) == NULL)
				goto failure;
				
			// If first vertex in current contour exists, break
			if ((vertexIntRefPtr = e3meshVertexIntRefArray_FirstItem(&contourDataPtr->vertexIntRefArray)) != NULL)
				break;
		}
	}

	// Save current contour in face
	if ((contourExtRef = e3meshContourData_ExtRefInMesh(contourDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var2 = contourExtRef;

	// Save next vertex in face
	vertexDataPtr = e3meshVertexIntRef_DataInMesh(*vertexIntRefPtr, meshDataPtr);
	if ((vertexExtRef = e3meshVertexData_ExtRefInMesh(vertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = vertexIntRefPtr;

	// Return next vertex in face
	return(vertexExtRef);
	
failure:

	iteratorPtr->var3 = NULL;
	iteratorPtr->var2 = NULL;
	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetContourFace : Get face containing contour.
//-----------------------------------------------------------------------------
//		Errors :	If contour deleted or If unable to get external reference
//					(out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetContourFace(TQ3GeometryObject mesh, TE3MeshContourExtRef contourExtRef, TE3MeshFaceExtRef* containerFaceExtRefPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshContourData* contourDataPtr;
	TE3MeshFaceIntRef containerFaceIntRef;
	TE3MeshFaceData* containerFaceDataPtr;
	
	// Check contour
	if ((contourDataPtr = e3meshContourExtRef_Data(contourExtRef)) == NULL)
		goto failure;
	
	// Get external reference to container face
	containerFaceIntRef = e3meshContourData_ContainerFaceIntRef(contourDataPtr);
	containerFaceDataPtr = e3meshFaceIntRef_DataInMesh(containerFaceIntRef, meshDataPtr);
	if ((*containerFaceExtRefPtr = e3meshFaceData_ExtRefInMesh(containerFaceDataPtr, meshDataPtr)) == NULL)
		goto failure;
	
	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstContourFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_FirstContourFace(TE3MeshContourExtRef contourExtRef, TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextContourFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_NextContourFace(TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstContourEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_FirstContourEdge(TE3MeshContourExtRef contourExtRef, TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextContourEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_NextContourEdge(TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetContourNumVertices : Get number of vertices in contour.
//-----------------------------------------------------------------------------
//		Note : If contour deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is counted separately.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetContourNumVertices(TQ3GeometryObject mesh, TE3MeshContourExtRef contourExtRef, TQ3Uns32 *numVertices)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshContourData* contourDataPtr;

	// Check contour
	if ((contourDataPtr = e3meshContourExtRef_Data(contourExtRef)) == NULL)
		goto failure;

	// Get number of vertices in contour
	*numVertices = e3meshContourData_NumVertices(contourDataPtr);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstContourVertex : Get first vertex in contour.
//-----------------------------------------------------------------------------
//		Errors :	If contour deleted, no first vertex or unable to create
//					external reference to vertex, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence is
//				iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_FirstContourVertex(TE3MeshContourExtRef contourExtRef, TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshContourData* contourDataPtr;
	TE3MeshVertexIntRef* vertexIntRefPtr;
	TE3MeshVertexData* vertexDataPtr;
	TE3MeshVertexExtRef vertexExtRef;

	// Get mesh for contour
	if ((meshDataPtr = e3meshContourExtRef_MeshData(contourExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshDataPtr, "ctve");

	// Check and save contour
	if ((contourDataPtr = e3meshContourExtRef_Data(contourExtRef)) == NULL)
		goto failure;
	iteratorPtr->var2 = contourExtRef;

	// Get and save first vertex in contour
	if ((vertexIntRefPtr = e3meshVertexIntRefArray_FirstItem(&contourDataPtr->vertexIntRefArray)) == NULL)
		goto failure;
	vertexDataPtr = e3meshVertexIntRef_DataInMesh(*vertexIntRefPtr, meshDataPtr);
	if ((vertexExtRef = e3meshVertexData_ExtRefInMesh(vertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = vertexIntRefPtr;

	// Return first vertex in contour
	return(vertexExtRef);
	
failure:

	iteratorPtr->var2 = NULL;
	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextContourVertex : Get next vertex in contour.
//-----------------------------------------------------------------------------
//		Errors :	If iterator ended, contour deleted, current vertex deleted,
//					no next vertex or unable to create external reference to
//					vertex, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence is
//				iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_NextContourVertex(TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr = iteratorPtr->var4.field1;
	TE3MeshContourExtRef contourExtRef;
	TE3MeshContourData* contourDataPtr;
	TE3MeshVertexIntRef* vertexIntRefPtr;
	TE3MeshVertexData* vertexDataPtr;
	TE3MeshVertexExtRef vertexExtRef;

	// Restore and check contour
	if ((contourExtRef = (TE3MeshContourExtRef) iteratorPtr->var2) == NULL)
		goto failure;
	if ((contourDataPtr = e3meshContourExtRef_Data(contourExtRef)) == NULL)
		goto failure;

	// Restore and check current vertex in contour
	if ((vertexIntRefPtr = (TE3MeshVertexIntRef*) iteratorPtr->var1) == NULL)
		goto failure;

	// Get and save next vertex in contour
	if ((vertexIntRefPtr = e3meshVertexIntRefArray_NextItem(&contourDataPtr->vertexIntRefArray, vertexIntRefPtr)) == NULL)
		goto failure;
	vertexDataPtr = e3meshVertexIntRef_DataInMesh(*vertexIntRefPtr, meshDataPtr);
	if ((vertexExtRef = e3meshVertexData_ExtRefInMesh(vertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = vertexIntRefPtr;

	// Return next vertex in contour
	return(vertexExtRef);
	
failure:

	iteratorPtr->var2 = NULL;
	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetEdgeOnBoundary : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetEdgeOnBoundary(TQ3GeometryObject mesh, TE3MeshEdgeExtRef edgeExtRef, TQ3Boolean *onBoundary)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetEdgeAttributeSet(TQ3GeometryObject mesh, TE3MeshEdgeExtRef edgeExtRef, TQ3AttributeSet *attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetEdgeAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetEdgeAttributeSet(TQ3GeometryObject mesh, TE3MeshEdgeExtRef edgeExtRef, TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	Q3Shared_Edited(mesh);

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetEdgeComponent(TQ3GeometryObject mesh, TE3MeshEdgeExtRef edgeExtRef, TE3MeshComponentExtRef* componentExtRefPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeFaces : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetEdgeFaces(TQ3GeometryObject mesh, TE3MeshEdgeExtRef edgeExtRef, TE3MeshFaceExtRef* faceExtRefPtr1, TE3MeshFaceExtRef* faceExtRefPtr2)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeVertices : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetEdgeVertices(TQ3GeometryObject mesh, TE3MeshEdgeExtRef edgeExtRef,
	TE3MeshVertexExtRef* vertexExtRefPtr1, TE3MeshVertexExtRef* vertexExtRefPtr2)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexIndex : Get index (0..N-1) of vertex in mesh.
//-----------------------------------------------------------------------------
//		Note : If unable to relocate vertices or if vertex deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetVertexIndex(TQ3GeometryObject mesh, TE3MeshVertexExtRef vertexExtRef, TQ3Uns32 *index)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexDataPtr;

	// Check vertex
	if ((vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRef)) == NULL)
		goto failure;

	// Use array of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3meshVertexDataArrayOrList_UseArray(&meshDataPtr->vertexDataArrayOrList, e3meshVertexData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck vertex (in case relocated)
	vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRef);

	// Get index of vertex
	*index = e3meshVertexDataArray_ItemIndex(&meshDataPtr->vertexDataArrayOrList.array, vertexDataPtr);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexOnBoundary : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetVertexOnBoundary(TQ3GeometryObject mesh, TE3MeshVertexExtRef vertexExtRef, TQ3Boolean *onBoundary)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexCoordinates : Get vertex coordinates.
//-----------------------------------------------------------------------------
//		Note : If vertex deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetVertexCoordinates(TQ3GeometryObject mesh, TE3MeshVertexExtRef vertexExtRef, TQ3Point3D *coordinates)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexDataPtr;

	// Check vertex
	if ((vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRef)) == NULL)
		goto failure;

	// Get vertex coordinates
	*coordinates = vertexDataPtr->point;

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetVertexCoordinates : Set vertex coordinates.
//-----------------------------------------------------------------------------
//		Note : If vertex deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetVertexCoordinates(TQ3GeometryObject mesh, TE3MeshVertexExtRef vertexExtRef, const TQ3Point3D *coordinates)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexDataPtr;

	// Check vertex
	if ((vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRef)) == NULL)
		goto failure;

	// Set vertex coordinates
	vertexDataPtr->point = *coordinates;

	Q3Shared_Edited(mesh);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexAttributeSet : Get vertex attribute set.
//-----------------------------------------------------------------------------
//		Note : If vertex deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetVertexAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexExtRef vertexExtRef, TQ3AttributeSet *attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexDataPtr;

	// Check vertex
	if ((vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRef)) == NULL)
		goto failure;

	// Get vertex attribute set
	E3Shared_Acquire(attributeSet, vertexDataPtr->attributeSet);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetVertexAttributeSet : Set vertex attribute set.
//-----------------------------------------------------------------------------
//		Note : If vertex deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetVertexAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexExtRef vertexExtRef, TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexDataPtr;

	// Check vertex
	if ((vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRef)) == NULL)
		goto failure;

	// Set vertex attribute set
	E3Shared_Replace(&vertexDataPtr->attributeSet, attributeSet);

	Q3Shared_Edited(mesh);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetVertexComponent(TQ3GeometryObject mesh, TE3MeshVertexExtRef vertexExtRef, TE3MeshComponentExtRef* componentExtRefPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstVertexFace : Get first face having vertex.
//-----------------------------------------------------------------------------
//		Errors :	If vertex deleted, no first face or unable to create
//					external reference to face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_FirstVertexFace(TE3MeshVertexExtRef vertexExtRef, TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshVertexData* vertexDataPtr;
	TE3MeshVertexIntRef vertexIntRef;
	TE3MeshFaceData* faceDataPtr;
	TE3MeshFaceExtRef faceExtRef;

	// Get mesh for vertex
	if ((meshDataPtr = e3meshVertexExtRef_MeshData(vertexExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshDataPtr, "vefa");

	// Check and save vertex
	if ((vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRef)) == NULL)
		goto failure;
	if ((vertexIntRef = e3meshVertexData_IntRefInMesh(vertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var2 = vertexExtRef;

	// Get first face in mesh
	if ((faceDataPtr = e3meshFaceDataArrayOrList_FirstItem(&meshDataPtr->faceDataArrayOrList)) == NULL)
		goto failure;

	// Get first face having vertex
	for (;;)
	{
		// If face has vertex, break
		if (e3meshFaceData_HasVertexIntRef(faceDataPtr, vertexIntRef))
			break;

		// Otherwise, get next face in mesh
		if ((faceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->faceDataArrayOrList, faceDataPtr)) == NULL)
			goto failure;
	}

	// Save first face having vertex
	if ((faceExtRef = e3meshFaceData_ExtRefInMesh(faceDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = faceExtRef;

	// Return first face having vertex
	return(faceExtRef);
	
failure:

	iteratorPtr->var2 = NULL;
	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextVertexFace : Get next face having vertex.
//-----------------------------------------------------------------------------
//		Errors :	If iterator ended, vertex deleted, current face deleted,
//					no next face or unable to create external reference to face,
//					return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_NextVertexFace(TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshDataPtr = iteratorPtr->var4.field1;
	TE3MeshVertexExtRef vertexExtRef;
	TE3MeshVertexData* vertexDataPtr;
	TE3MeshVertexIntRef vertexIntRef;
	TE3MeshFaceExtRef faceExtRef;
	TE3MeshFaceData* faceDataPtr;

	// Restore and check vertex
	if ((vertexExtRef = (TE3MeshVertexExtRef) iteratorPtr->var2) == NULL)
		goto failure;
	if ((vertexDataPtr = e3meshVertexExtRef_Data(vertexExtRef)) == NULL)
		goto failure;

	// Restore and check current face having vertex
	if ((faceExtRef = (TE3MeshFaceExtRef) iteratorPtr->var1) == NULL)
		goto failure;
	if ((faceDataPtr = e3meshFaceExtRef_Data(faceExtRef)) == NULL)
		goto failure;
	if ((vertexIntRef = e3meshVertexData_IntRefInMesh(vertexDataPtr, meshDataPtr)) == NULL)
		goto failure;

	// Get next face having vertex
	for (;;)
	{
		// Get next face in mesh
		if ((faceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->faceDataArrayOrList, faceDataPtr)) == NULL)
			goto failure;

		// If face has vertex, break
		if (e3meshFaceData_HasVertexIntRef(faceDataPtr, vertexIntRef))
			break;
	}

	// Save next face having vertex
	if ((faceExtRef = e3meshFaceData_ExtRefInMesh(faceDataPtr, meshDataPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = faceExtRef;

	// Return next face having vertex
	return(faceExtRef);
	
failure:

	iteratorPtr->var2 = NULL;
	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstVertexEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_FirstVertexEdge(TE3MeshVertexExtRef vertexExtRef, TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextVertexEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_NextVertexEdge(TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstVertexVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_FirstVertexVertex(TE3MeshVertexExtRef vertexExtRef, TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextVertexVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_NextVertexVertex(TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetCornerAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetCornerAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexExtRef vertexExtRef, TE3MeshFaceExtRef faceExtRef, TQ3AttributeSet *attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetCornerAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetCornerAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexExtRef vertexExtRef, TE3MeshFaceExtRef faceExtRef, TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	Q3Shared_Edited(mesh);

	return(kQ3Failure);
}

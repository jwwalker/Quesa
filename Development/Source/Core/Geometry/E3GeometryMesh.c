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
// Mesh and mesh part data types
typedef struct TE3MeshPartData TE3MeshPartData;
typedef struct TE3MeshVertexData TE3MeshVertexData;
typedef struct TE3MeshContourData TE3MeshContourData;
typedef struct TE3MeshFaceData TE3MeshFaceData;
typedef struct TE3MeshData TE3MeshData;



// Internal references to part data types - distinct types
typedef struct TE3MeshPartIntRefOpaque* TE3MeshPartIntRef;
typedef struct TE3MeshVertexIntRefOpaque* TE3MeshVertexIntRef;
typedef struct TE3MeshContourIntRefOpaque* TE3MeshContourIntRef;
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
	TE3MeshPartData**				meshPartDataHdl;
};



// TE3MeshPartDataPtr
typedef TE3MeshPartData* TE3MeshPartDataPtr;

E3POOL_DECLARE(TE3MeshPartDataPtr, e3meshPartDataPtr, static);
E3POOL_DEFINE(TE3MeshPartDataPtr, e3meshPartDataPtr, static, 16);



// TE3MeshVertexData
struct TE3MeshVertexData {
	TE3MeshPartData					meshPartData;			// base class
	TQ3Point3D 						point;
	TQ3AttributeSet 				vertexAttributeSet;
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
	TE3MeshPartData					meshPartData;			// base class
	TE3MeshFaceIntRef				containerMeshFaceIntRef;
	TE3MeshVertexIntRefArray		meshVertexIntRefArray;
};

E3ARRAY_DECLARE(TE3MeshContourData, e3meshContourData, static);
E3ARRAY_DEFINE(TE3MeshContourData, e3meshContourData, static);

E3LIST_DECLARE(TE3MeshContourData, e3meshContourData, static);
E3LIST_DEFINE(TE3MeshContourData, e3meshContourData, static);

E3ARRAY_OR_LIST_DECLARE(TE3MeshContourData, e3meshContourData, static);
E3ARRAY_OR_LIST_DEFINE(TE3MeshContourData, e3meshContourData, static);



// TE3MeshFaceData
struct TE3MeshFaceData {
	TE3MeshPartData					meshPartData;			// base class
	TE3MeshContourDataArrayOrList	meshContourDataArrayOrList;
	TQ3AttributeSet 				faceAttributeSet;
};

E3ARRAY_DECLARE(TE3MeshFaceData, e3meshFaceData, static);
E3ARRAY_DEFINE(TE3MeshFaceData, e3meshFaceData, static);

E3LIST_DECLARE(TE3MeshFaceData, e3meshFaceData, static);
E3LIST_DEFINE(TE3MeshFaceData, e3meshFaceData, static);

E3ARRAY_OR_LIST_DECLARE(TE3MeshFaceData, e3meshFaceData, static);
E3ARRAY_OR_LIST_DEFINE(TE3MeshFaceData, e3meshFaceData, static);



// TE3MeshData
struct TE3MeshData {
	// Note:	In order for tags to work in the meshPartDataPtrPool,
	//			the first member of a mesh structure must *not* be,
	//			in effect, a handle (TE3MeshData**) referring back to
	//			this mesh!
	TE3MeshPartDataPtrPool			meshPartDataPtrPool;
	TE3MeshVertexDataArrayOrList	meshVertexDataArrayOrList;
	TE3MeshFaceDataArrayOrList		meshFaceDataArrayOrList;
	TQ3AttributeSet 				meshAttributeSet;
};





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3meshPartData_AllocateMasterPointerInMesh :	Allocate master pointer
//														to mesh part data.
//-----------------------------------------------------------------------------
//		Errors :	If unable to allocate master pointer (out of memory),
//					return kQ3Failure.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshPartData_AllocateMasterPointerInMesh(TE3MeshPartData* meshPartDataPtr, TE3MeshData* meshDataPtr)
{
	TE3MeshPartData** meshPartDataHdl;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	
	if (meshPartDataPtr->meshPartDataHdl == NULL)
	{
		// Allocate master pointer from mesh's pool
			// Note: The type of items in the pool is, in effect, a union:
			//
			//		union {
			//			TE3MeshPartData*	meshPartDataPtr;
			//			TE3MeshData*		meshDataPtr;
			//		};
			//
			// Regular items in the pool are of type TE3MeshPartData* and tags in
			// the pool are of type TE3MeshData*.
			//
			// Neverthless, for simplicity the type is declared to be simply
			// TE3MeshPartData* and we use a type cast to coerce one pointer
			// type to the other.
		if ((meshPartDataHdl = e3meshPartDataPtrPool_AllocateTagged(&meshDataPtr->meshPartDataPtrPool, (const TE3MeshPartDataPtr*) &meshDataPtr)) == NULL)
			goto failure;

		// Link master pointer and mesh part data
		*meshPartDataHdl = meshPartDataPtr;
		meshPartDataPtr->meshPartDataHdl = meshPartDataHdl;
	}

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshPartData_FreeMasterPointerInMesh :	Free master pointer, if any,
//													to mesh part data.
//-----------------------------------------------------------------------------
//		Warning :	Use this function with care! Freeing a master pointer to a
//					part that is still referenced, either internally by another
//					part of the mesh or externally by the application, will
//					result in a fatal error. 
//-----------------------------------------------------------------------------
static void
e3meshPartData_FreeMasterPointerInMesh(TE3MeshPartData* meshPartDataPtr, TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	e3meshPartDataPtrPool_Free(&meshDataPtr->meshPartDataPtrPool, &meshPartDataPtr->meshPartDataHdl);
}





//=============================================================================
//      e3meshPartData_IntRefInMesh : Return internal reference to mesh part.
//-----------------------------------------------------------------------------
//		Note : If unable to get internal reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
static TE3MeshPartIntRef
e3meshPartData_IntRefInMesh(TE3MeshPartData* meshPartDataPtr, TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// Allocate master pointer to mesh part data
	if (e3meshPartData_AllocateMasterPointerInMesh(meshPartDataPtr, meshDataPtr) == kQ3Failure)
		goto failure;

	// Return handle to mesh part data
	return((TE3MeshPartIntRef) meshPartDataPtr->meshPartDataHdl);
	
failure:

	return(NULL);
}





//=============================================================================
//      e3meshPartData_ExtRefInMesh : Return external reference to mesh part.
//-----------------------------------------------------------------------------
//		Note : If unable to get external reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
static TE3MeshPartExtRef
e3meshPartData_ExtRefInMesh(TE3MeshPartData* meshPartDataPtr, TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// Allocate master pointer to mesh part data
	if (e3meshPartData_AllocateMasterPointerInMesh(meshPartDataPtr, meshDataPtr) == kQ3Failure)
		goto failure;

	return(meshPartDataPtr->meshPartDataHdl);
	
failure:

	return(NULL);
}





//=============================================================================
//      e3meshPartData_Create : TE3MeshPartData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshPartData_Create(TE3MeshPartData* meshPartDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// Initialize pointer to master pointer BEFORE calling e3meshPartData_AllocateMasterPointerInMesh
	meshPartDataPtr->meshPartDataHdl = NULL;
	
	// If requested, allocate master pointer to mesh part data
	if (isReferenced)
	{
		if (e3meshPartData_AllocateMasterPointerInMesh(meshPartDataPtr, meshDataPtr) == kQ3Failure)
			goto failure;
	}

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshPartData_Destroy : TE3MeshPartData destructor.
//-----------------------------------------------------------------------------
static void
e3meshPartData_Destroy(TE3MeshPartData* meshPartDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);

	// If mesh part data has master pointer, clear
	if (meshPartDataPtr->meshPartDataHdl)
		*meshPartDataPtr->meshPartDataHdl = NULL;
}





//=============================================================================
//      e3meshPartData_Relocate : Relocate mesh part data.
//-----------------------------------------------------------------------------
static void
e3meshPartData_Relocate(TE3MeshPartData* meshPartDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);

	// If mesh part data has master pointer, update
	if (meshPartDataPtr->meshPartDataHdl)
		*meshPartDataPtr->meshPartDataHdl = meshPartDataPtr;
}





//=============================================================================
//      e3meshPartDataPtr_IsMeshDataPtr :	Return if this mesh part data
//											pointer is actually a mesh data
//											pointer.
//-----------------------------------------------------------------------------
//		Warning :	In contrast to other functions, the parameter for this
//					function is a pointer to a pointer, not a pointer!
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Boolean
e3meshPartDataPtr_IsMeshDataPtr(const TE3MeshPartDataPtr* meshPartDataHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataHdl);

	// A null pointer is not a pointer to mesh data	
	if ((*meshPartDataHdl) == NULL)
		return(kQ3False);

	// If this pointer points to a pointer that points back to this pointer,
	// it is a pointer to mesh part data -- not mesh data	
	if ((*meshPartDataHdl)->meshPartDataHdl == meshPartDataHdl)
		return(kQ3False);
	
	return(kQ3True);
}





//=============================================================================
//      e3meshPartIntRef_DataInMesh : Return data for this mesh part.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshPartData*
e3meshPartIntRef_DataInMesh(TE3MeshPartIntRef meshPartIntRef, const TE3MeshData* meshDataPtr)
{
#pragma unused(meshDataPtr)

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartIntRef);

	return(* (TE3MeshPartData**) meshPartIntRef);
}





//=============================================================================
//      e3meshPartIntRef_IsEqualTo :	Return if two part internal references
//										are equal.
//-----------------------------------------------------------------------------
//		Warning :	In contrast to other functions, the parameters for this
//					function are pointers to internal references, not internal
//					references!
//-----------------------------------------------------------------------------
static TQ3Boolean
e3meshPartIntRef_IsEqualTo(const TE3MeshPartIntRef* meshPartIntRefPtr1, const TE3MeshPartIntRef* meshPartIntRefPtr2)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartIntRefPtr1);
	Q3_ASSERT_VALID_PTR(meshPartIntRefPtr2);

	return(*meshPartIntRefPtr1 == *meshPartIntRefPtr2 ? kQ3True : kQ3False);
}





//=============================================================================
//      e3meshPartExtRef_Data : Return data for this mesh part.
//-----------------------------------------------------------------------------
//		Note : If part deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshPartData*
e3meshPartExtRef_Data(TE3MeshPartExtRef meshPartExtRef)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartExtRef);

	return(*meshPartExtRef);
}





//=============================================================================
//      e3meshPartExtRef_MeshData : Return data for mesh having this part.
//-----------------------------------------------------------------------------
static TE3MeshData*
e3meshPartExtRef_MeshData(TE3MeshPartExtRef meshPartExtRef)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartExtRef);

	return(* (TE3MeshData**) e3meshPartDataPtrPoolItem_Tag(meshPartExtRef, e3meshPartDataPtr_IsMeshDataPtr));
}





//=============================================================================
//      e3meshVertexData_IntRefInMesh : Return internal reference to mesh vertex.
//-----------------------------------------------------------------------------
//		Note : If unable to get internal reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshVertexIntRef
e3meshVertexData_IntRefInMesh(TE3MeshVertexData* meshVertexDataPtr, TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshVertexIntRef,
		e3meshPartData_IntRefInMesh(
			E3_UP_CAST(TE3MeshPartData*, meshVertexDataPtr),
			meshDataPtr)));
}





//=============================================================================
//      e3meshVertexData_ExtRefInMesh :	Return external referece to mesh vertex.
//-----------------------------------------------------------------------------
//		Note : If unable to get external reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
static TE3MeshVertexExtRef
e3meshVertexData_ExtRefInMesh(TE3MeshVertexData* meshVertexDataPtr, TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshVertexExtRef,
		e3meshPartData_ExtRefInMesh(
			E3_UP_CAST(TE3MeshPartData*, meshVertexDataPtr),
			meshDataPtr)));
}





//=============================================================================
//      e3meshVertexData_Create : TE3MeshVertexData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshVertexData_Create(TE3MeshVertexData* meshVertexDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced, const TQ3Vertex3D* vertexPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshVertexDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(vertexPtr);

	// Create part data
	if (e3meshPartData_Create(E3_UP_CAST(TE3MeshPartData*, meshVertexDataPtr), meshDataPtr, isReferenced) == kQ3Failure)
		goto failure;

	// Initialize point
	meshVertexDataPtr->point = vertexPtr->point;

	// Acquire attribute set
	E3Shared_Acquire(&meshVertexDataPtr->vertexAttributeSet, vertexPtr->attributeSet);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshVertexData_Destroy : TE3MeshVertexData destructor.
//-----------------------------------------------------------------------------
static void
e3meshVertexData_Destroy(TE3MeshVertexData* meshVertexDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshVertexDataPtr);

	// Release attribute set
	E3Object_DisposeAndForget(meshVertexDataPtr->vertexAttributeSet);

	// Destroy part data
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, meshVertexDataPtr));
}





//=============================================================================
//      e3meshVertexData_Relocate : Relocate mesh vertex data.
//-----------------------------------------------------------------------------
static void
e3meshVertexData_Relocate(TE3MeshVertexData* meshVertexDataPtr)
{
	e3meshPartData_Relocate(E3_UP_CAST(TE3MeshPartData*, meshVertexDataPtr));
}





//=============================================================================
//      e3meshVertexIntRef_DataInMesh : Return data for this mesh vertex.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshVertexData*
e3meshVertexIntRef_DataInMesh(TE3MeshVertexIntRef meshVertexIntRef, const TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshVertexData*,
		e3meshPartIntRef_DataInMesh(
			E3_UP_CAST(TE3MeshPartIntRef, meshVertexIntRef),
			meshDataPtr)));
}





//=============================================================================
//      e3meshVertexExtRef_Data : Return data for this mesh vertex.
//-----------------------------------------------------------------------------
//		Note : If vertex deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshVertexData*
e3meshVertexExtRef_Data(TE3MeshVertexExtRef meshVertexExtRef)
{
	return(E3_DOWN_CAST(TE3MeshVertexData*,
		e3meshPartExtRef_Data(
			E3_UP_CAST(TE3MeshPartExtRef, meshVertexExtRef))));
}





//=============================================================================
//      e3meshVertexExtRef_MeshData : Return data for mesh having this mesh vertex.
//-----------------------------------------------------------------------------
static TE3MeshData*
e3meshVertexExtRef_MeshData(TE3MeshVertexExtRef meshVertexExtRef)
{
	return(e3meshPartExtRef_MeshData(E3_UP_CAST(TE3MeshPartExtRef, meshVertexExtRef)));
}





//=============================================================================
//      e3meshContourData_ExtRefInMesh : Return external reference to mesh contour.
//-----------------------------------------------------------------------------
//		Note : If unable to get external reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshContourExtRef
e3meshContourData_ExtRefInMesh(TE3MeshContourData* meshContourDataPtr, TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshContourExtRef,
		e3meshPartData_ExtRefInMesh(
			E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr),
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
static TQ3Status
e3meshContourData_Create(TE3MeshContourData* meshContourDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced, TE3MeshFaceIntRef containerMeshFaceIntRef, TQ3Uns32 numVertices, const TE3MeshVertexExtRef* meshVertexExtRefs)
{
	TQ3Uns32 effectiveNumVertices;
	TQ3Uns32 i;
	TE3MeshVertexIntRef* meshVertexIntRefPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(containerMeshFaceIntRef);
	Q3_ASSERT(numVertices > 0);
	Q3_ASSERT_VALID_PTR(meshVertexExtRefs);

	// Check for deleted vertices; count effective number of vertices (excluding repeats)
	effectiveNumVertices = 0;
	for (i = 0; i < numVertices; ++i)
	{
		if (e3meshVertexExtRef_Data(meshVertexExtRefs[i]) == NULL)
			goto failure_1;
		if (meshVertexExtRefs[i] != meshVertexExtRefs[i > 0 ? i-1 : numVertices-1])
			++effectiveNumVertices;
	}
	if (effectiveNumVertices < 2)
		goto failure_2;

	// Create part data
	if (e3meshPartData_Create(E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr), meshDataPtr, isReferenced) == kQ3Failure)
		goto failure_3;

	// Initialize container face internal reference
	meshContourDataPtr->containerMeshFaceIntRef = containerMeshFaceIntRef;

	// Create uninitialized vertex internal reference array
	if (e3meshVertexIntRefArray_Create(&meshContourDataPtr->meshVertexIntRefArray, effectiveNumVertices, NULL) == kQ3Failure)
		goto failure_4;

	// Initialize vertex internal reference array
	meshVertexIntRefPtr = e3meshVertexIntRefArray_FirstItem(&meshContourDataPtr->meshVertexIntRefArray);
	for (i = 0; i < numVertices; ++i)
	{
		if (meshVertexExtRefs[i] != meshVertexExtRefs[i > 0 ? i-1 : numVertices-1])
		{
			TE3MeshVertexData* meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRefs[i]);
			
			*meshVertexIntRefPtr = e3meshVertexData_IntRefInMesh(meshVertexDataPtr, meshDataPtr);
			++meshVertexIntRefPtr;
		}
	}

	return(kQ3Success);

	// Dead code to reverse e3meshVertexIntRefArray_Create
failure_4:

	e3meshPartData_FreeMasterPointerInMesh(E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr), meshDataPtr);
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr));
failure_3:

failure_2:

failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshContourData_Destroy : TE3MeshContourData destructor.
//-----------------------------------------------------------------------------
static void
e3meshContourData_Destroy(TE3MeshContourData* meshContourDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);

	// Destroy vertex internal reference array
	e3meshVertexIntRefArray_Destroy(&meshContourDataPtr->meshVertexIntRefArray, NULL);

	// Destroy part data
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr));
}





//=============================================================================
//      e3meshContourData_Relocate : Relocate mesh vertex data.
//-----------------------------------------------------------------------------
static void
e3meshContourData_Relocate(TE3MeshContourData* meshContourDataPtr)
{
	e3meshPartData_Relocate(E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr));
}





//=============================================================================
//      e3meshContourData_SetContainerFaceIntRef :	Set internal reference to
//													container mesh face.
//-----------------------------------------------------------------------------
static void
e3meshContourData_SetContainerFaceIntRef(TE3MeshContourData* meshContourDataPtr, TE3MeshFaceIntRef containerMeshFaceIntRef)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);
	Q3_ASSERT_VALID_PTR(containerMeshFaceIntRef);

	meshContourDataPtr->containerMeshFaceIntRef = containerMeshFaceIntRef;
}





//=============================================================================
//      e3meshContourData_ContainerFaceIntRef :	Return internal reference to
//												container mesh face.
//-----------------------------------------------------------------------------
static TE3MeshFaceIntRef
e3meshContourData_ContainerFaceIntRef(TE3MeshContourData* meshContourDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);

	return(meshContourDataPtr->containerMeshFaceIntRef);
}





//=============================================================================
//      e3meshContourData_NumVertices : Return number of vertices in mesh contour.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is counted separately.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3meshContourData_NumVertices(const TE3MeshContourData* meshContourDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);

	return(e3meshVertexIntRefArray_Length(&meshContourDataPtr->meshVertexIntRefArray));
}





//=============================================================================
//      e3meshContourData_HasVertexIntRef : Return if mesh contour has vertex.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3meshContourData_HasVertexIntRef(const TE3MeshContourData* meshContourDataPtr, TE3MeshVertexIntRef meshVertexIntRef)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);
	Q3_ASSERT_VALID_PTR(meshVertexIntRef);

	return(e3meshVertexIntRefArray_OrForEach(
		&meshContourDataPtr->meshVertexIntRefArray,
		E3_DOWN_CAST(TQ3Boolean (*)(const TE3MeshVertexIntRef*, void*), e3meshPartIntRef_IsEqualTo),
		&meshVertexIntRef));
}





//=============================================================================
//      e3meshContourExtRef_Data : Return data for this mesh contour.
//-----------------------------------------------------------------------------
//		Note : If contour deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshContourData*
e3meshContourExtRef_Data(TE3MeshContourExtRef meshContourExtRef)
{
	return(E3_DOWN_CAST(TE3MeshContourData*,
		e3meshPartExtRef_Data(
			E3_UP_CAST(TE3MeshPartExtRef, meshContourExtRef))));
}





//=============================================================================
//      e3meshContourExtRef_MeshData : Return data for mesh having this mesh contour.
//-----------------------------------------------------------------------------
static TE3MeshData*
e3meshContourExtRef_MeshData(TE3MeshContourExtRef meshContourExtRef)
{
	return(e3meshPartExtRef_MeshData(E3_UP_CAST(TE3MeshPartExtRef, meshContourExtRef)));
}





//=============================================================================
//      e3meshFaceData_IntRefInMesh : Return internal reference to mesh face.
//-----------------------------------------------------------------------------
//		Note : If unable to get internal reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshFaceIntRef
e3meshFaceData_IntRefInMesh(TE3MeshFaceData* meshFaceDataPtr, TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshFaceIntRef,
		e3meshPartData_IntRefInMesh(
			E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr),
			meshDataPtr)));
}





//=============================================================================
//      e3meshFaceData_ExtRefInMesh : Return external reference to mesh face.
//-----------------------------------------------------------------------------
//		Note : If unable to get external reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
static TE3MeshFaceExtRef
e3meshFaceData_ExtRefInMesh(TE3MeshFaceData* meshFaceDataPtr, TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshFaceExtRef,
		e3meshPartData_ExtRefInMesh(
			E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr),
			meshDataPtr)));
}





//=============================================================================
//      e3meshFaceData_Create : TE3MeshFaceData constructor.
//-----------------------------------------------------------------------------
//		Errors :	If any vertex deleted or unable to create (out of memory),
//					return kQ3Failure.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshFaceData_Create(TE3MeshFaceData* meshFaceDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced, TQ3Uns32 numContours, TQ3Uns32* numVerticesPtr, const TE3MeshVertexExtRef** meshVertexExtRefsPtr, TQ3AttributeSet faceAttributeSet)
{
	TE3MeshFaceIntRef meshFaceIntRef;
	TQ3Uns32 i;
	TE3MeshContourData* meshContourDataPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT(numContours > 0);

	// Create part data, with reference
	if (e3meshPartData_Create(E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr), meshDataPtr, kQ3True) == kQ3Failure)
		goto failure_1;

	// Get internal reference to face
	if ((meshFaceIntRef = e3meshFaceData_IntRefInMesh(meshFaceDataPtr, meshDataPtr)) == NULL)
		goto failure_2;

	// Create uninitialized contour data array
	if (e3meshContourDataArray_Create(&meshFaceDataPtr->meshContourDataArrayOrList.array, numContours, NULL) == kQ3Failure)
		goto failure_3;

	// Create each contour data
	for (i = 0, meshContourDataPtr = e3meshContourDataArray_FirstItem(&meshFaceDataPtr->meshContourDataArrayOrList.array);
		i < numContours;
		++i, meshContourDataPtr = e3meshContourDataArray_NextItem(&meshFaceDataPtr->meshContourDataArrayOrList.array, meshContourDataPtr))
	{
		// Create contour data, without reference
		if (e3meshContourData_Create(meshContourDataPtr, meshDataPtr, kQ3False, meshFaceIntRef, numVerticesPtr[i], meshVertexExtRefsPtr[i]) == kQ3Failure)
			goto failure_4;
	}

	// Acquire attribute set
	E3Shared_Acquire(&meshFaceDataPtr->faceAttributeSet, faceAttributeSet);

	return(kQ3Success);

	// Dead code to reverse e3meshContourData_Create
failure_4:

	while (i-- > 0)
	{
		meshContourDataPtr = e3meshContourDataArray_PreviousItem(&meshFaceDataPtr->meshContourDataArrayOrList.array, meshContourDataPtr);
		e3meshContourData_Destroy(meshContourDataPtr);
	}
	e3meshContourDataArray_Destroy(&meshFaceDataPtr->meshContourDataArrayOrList.array, NULL);
failure_3:
	
failure_2:

	e3meshPartData_FreeMasterPointerInMesh(E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr), meshDataPtr);
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr));
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshFaceData_Destroy : TE3MeshFaceData destructor.
//-----------------------------------------------------------------------------
static void
e3meshFaceData_Destroy(TE3MeshFaceData* meshFaceDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);

	// Release attribute set
	E3Object_DisposeAndForget(meshFaceDataPtr->faceAttributeSet);

	// Destroy contour data array or list
	e3meshContourDataArrayOrList_Destroy(&meshFaceDataPtr->meshContourDataArrayOrList, e3meshContourData_Destroy);

	// Destroy part data
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr));
}





//=============================================================================
//      e3meshFaceData_Relocate : Relocate mesh vertex data.
//-----------------------------------------------------------------------------
static void
e3meshFaceData_Relocate(TE3MeshFaceData* meshFaceDataPtr)
{
	e3meshPartData_Relocate(E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr));
}





//=============================================================================
//      e3meshFaceData_NumContours : Return number of contours in face.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3meshFaceData_NumContours(const TE3MeshFaceData* meshFaceDataPtr)
{	TQ3Uns32		numContours;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);



	// Get the number of contours
	numContours = e3meshContourDataArrayOrList_Length(&meshFaceDataPtr->meshContourDataArrayOrList);
	return(numContours);
}





//=============================================================================
//      e3meshFaceData_NumVertices : Return number of vertices in face.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is counted separately.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3meshFaceData_NumVertices(const TE3MeshFaceData* meshFaceDataPtr)
{
	TQ3Uns32 numVertices;
	const TE3MeshContourData* meshContourDataPtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);

	// Number of vertices in face is sum of number of vertices in each contour
	numVertices = 0;
	for (meshContourDataPtr = e3meshContourDataArrayOrList_FirstItemConst(&meshFaceDataPtr->meshContourDataArrayOrList);
		meshContourDataPtr != NULL;
		meshContourDataPtr = e3meshContourDataArrayOrList_NextItemConst(&meshFaceDataPtr->meshContourDataArrayOrList, meshContourDataPtr))
	{
		numVertices += e3meshContourData_NumVertices(meshContourDataPtr);
	}

	return(numVertices);
}





//=============================================================================
//      e3meshFaceData_HasVertexIntRef : Return if mesh face has vertex.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3meshFaceData_HasVertexIntRef(const TE3MeshFaceData* meshFaceDataPtr, TE3MeshVertexIntRef meshVertexIntRef)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);
	Q3_ASSERT_VALID_PTR(meshVertexIntRef);

	return(e3meshContourDataArrayOrList_OrForEach(
		&meshFaceDataPtr->meshContourDataArrayOrList,
		E3_DOWN_CAST(TQ3Boolean (*)(const TE3MeshContourData*, void*), e3meshContourData_HasVertexIntRef),
		meshVertexIntRef));
}





//=============================================================================
//      e3meshFaceIntRef_DataInMesh : Return data for this mesh face.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshFaceData*
e3meshFaceIntRef_DataInMesh(TE3MeshFaceIntRef meshFaceIntRef, const TE3MeshData* meshDataPtr)
{
	return(E3_DOWN_CAST(TE3MeshFaceData*,
		e3meshPartIntRef_DataInMesh(
			E3_UP_CAST(TE3MeshPartIntRef, meshFaceIntRef),
			meshDataPtr)));
}





//=============================================================================
//      e3meshFaceExtRef_Data : Return data for this face.
//-----------------------------------------------------------------------------
//		Note : If face deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshFaceData*
e3meshFaceExtRef_Data(TE3MeshFaceExtRef meshFaceExtRef)
{
	return(E3_DOWN_CAST(TE3MeshFaceData*,
		e3meshPartExtRef_Data(
			E3_UP_CAST(TE3MeshPartExtRef, meshFaceExtRef))));
}





//=============================================================================
//      e3meshFaceExtRef_MeshData : Return data for mesh having this face.
//-----------------------------------------------------------------------------
static TE3MeshData*
e3meshFaceExtRef_MeshData(TE3MeshFaceExtRef meshFaceExtRef)
{
	return(e3meshPartExtRef_MeshData(E3_UP_CAST(TE3MeshPartExtRef, meshFaceExtRef)));
}





//=============================================================================
//      e3meshData_Create : TE3MeshData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3meshData_Create(TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// Create mesh part references pool
	if (e3meshPartDataPtrPool_Create(&meshDataPtr->meshPartDataPtrPool) == kQ3Failure)
		goto failure_1;

	// Create empty vertex data array
	if (e3meshVertexDataArray_Create(&meshDataPtr->meshVertexDataArrayOrList.array, 0, NULL) == kQ3Failure)
		goto failure_2;

	// Create empty face data array
	if (e3meshFaceDataArray_Create(&meshDataPtr->meshFaceDataArrayOrList.array, 0, NULL) == kQ3Failure)
		goto failure_3;

	// Initialize attribute set
	meshDataPtr->meshAttributeSet = NULL;

	return(kQ3Success);
	
	// Dead code to reverse e3meshFaceDataArray_Create
failure_3:

	e3meshVertexDataArray_Destroy(&meshDataPtr->meshVertexDataArrayOrList.array, e3meshVertexData_Destroy);
failure_2:

	e3meshPartDataPtrPool_Destroy(&meshDataPtr->meshPartDataPtrPool);
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshData_Destroy : TE3MeshData destructor.
//-----------------------------------------------------------------------------
static void
e3meshData_Destroy(TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	//	Release attribute set
	E3Object_DisposeAndForget(meshDataPtr->meshAttributeSet);

	// Destroy face data array or list
	e3meshFaceDataArrayOrList_Destroy(&meshDataPtr->meshFaceDataArrayOrList, e3meshFaceData_Destroy);

	// Destroy vertex data array or list
	e3meshVertexDataArrayOrList_Destroy(&meshDataPtr->meshVertexDataArrayOrList, e3meshVertexData_Destroy);

	// Destroy mesh part references pool
	e3meshPartDataPtrPool_Destroy(&meshDataPtr->meshPartDataPtrPool);
}





//=============================================================================
//      e3meshData_NumFaces : Return number of faces in mesh.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3meshData_NumFaces(const TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	return(e3meshFaceDataArrayOrList_Length(&meshDataPtr->meshFaceDataArrayOrList));
}





//=============================================================================
//      e3meshData_NumContours : Return number of contours in mesh.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3meshData_NumContours(const TE3MeshData* meshDataPtr)
{
	TQ3Uns32 numContours;
	const TE3MeshFaceData* meshFaceDataPtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// Number of contours in mesh is sum of number of contours in each face
	numContours = 0;
	for (meshFaceDataPtr = e3meshFaceDataArrayOrList_FirstItemConst(&meshDataPtr->meshFaceDataArrayOrList);
		meshFaceDataPtr != NULL;
		meshFaceDataPtr = e3meshFaceDataArrayOrList_NextItemConst(&meshDataPtr->meshFaceDataArrayOrList, meshFaceDataPtr))
	{
		numContours += e3meshFaceData_NumContours(meshFaceDataPtr);
	}

	return(numContours);
}





//=============================================================================
//      e3meshData_NumVertices : Return number of vertices in mesh.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3meshData_NumVertices(const TE3MeshData* meshDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	return(e3meshVertexDataArrayOrList_Length(&meshDataPtr->meshVertexDataArrayOrList));
}





//=============================================================================
//      e3meshIterator_Initialize : TQ3MeshIterator partial constructor.
//-----------------------------------------------------------------------------
#pragma mark -
static void
e3meshIterator_Initialize(TQ3MeshIterator* meshIteratorPtr, TE3MeshData* meshDataPtr, const char* iteratorKind)
{
	// Save mesh
	meshIteratorPtr->var4.field1 = meshDataPtr;

	// Save iterator kind
	strncpy(meshIteratorPtr->var4.field2, iteratorKind, 4);
	
	// Initialize other fields
	meshIteratorPtr->var1 =
	meshIteratorPtr->var2 =
	meshIteratorPtr->var3 = NULL;
}





//=============================================================================
//      e3geom_mesh_new : TE3MeshData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3geom_mesh_new(TQ3Object theObject, void *privateData, const void *paramData)
{
#pragma unused(theObject)
#pragma unused(paramData)

	return(e3meshData_Create(E3_DOWN_CAST(TE3MeshData*, privateData)));
}





//=============================================================================
//      e3geom_mesh_delete : TE3MeshData destructor.
//-----------------------------------------------------------------------------
static void
e3geom_mesh_delete(TQ3Object theObject, void *privateData)
{
#pragma unused(theObject)

	e3meshData_Destroy(E3_DOWN_CAST(TE3MeshData*, privateData));
}





//=============================================================================
//      e3geom_mesh_duplicate : Mesh duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
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
static TQ3Object
e3geom_mesh_cache_new(TQ3ViewObject view, TQ3GeometryObject mesh, const TE3MeshData* meshDataPtr)
{
#pragma unused(view)
	TQ3GeometryObject polyhedron;
	TQ3PolyhedronData polyhedronData;	
	const TE3MeshFaceData* meshFaceDataPtr;
	const TE3MeshVertexData* firstMeshVertexDataPtr;
	const TE3MeshVertexIntRef* meshVertexIntRefPtr;
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
	for (meshFaceDataPtr = e3meshFaceDataArrayOrList_FirstItemConst(&meshDataPtr->meshFaceDataArrayOrList);
		meshFaceDataPtr != NULL;
		meshFaceDataPtr = e3meshFaceDataArrayOrList_NextItemConst(&meshDataPtr->meshFaceDataArrayOrList, meshFaceDataPtr))
	{
		if (e3meshFaceData_NumContours(meshFaceDataPtr) == 1)
			polyhedronData.numTriangles += e3meshFaceData_NumVertices(meshFaceDataPtr) - 2;
	}
	if (polyhedronData.numTriangles > 0)
	{
		if ((polyhedronData.triangles = (TQ3PolyhedronTriangleData*) Q3Memory_Allocate(polyhedronData.numTriangles * sizeof(TQ3PolyhedronTriangleData))) == NULL)
			goto cleanup_3;
	}
	else
		polyhedronData.triangles = NULL;

	// Use array of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3meshVertexDataArrayOrList_UseArray(E3_CONST_CAST(TE3MeshVertexDataArrayOrList*, &meshDataPtr->meshVertexDataArrayOrList), e3meshVertexData_Relocate) == kQ3Failure)
		goto cleanup_4;
	
	// Get first mesh vertex data
	firstMeshVertexDataPtr = e3meshVertexDataArray_FirstItemConst(&meshDataPtr->meshVertexDataArrayOrList.array);

	// Initialize the vertices
	for (i = 0; i < polyhedronData.numVertices; ++i)
	{
		polyhedronData.vertices[i].point = firstMeshVertexDataPtr[i].point;
		E3Shared_Acquire(&polyhedronData.vertices[i].attributeSet, firstMeshVertexDataPtr[i].vertexAttributeSet);
	}
	
	// Initialize the triangles
	k = 0;
	for (meshFaceDataPtr = e3meshFaceDataArrayOrList_FirstItemConst(&meshDataPtr->meshFaceDataArrayOrList);
		meshFaceDataPtr != NULL;
		meshFaceDataPtr = e3meshFaceDataArrayOrList_NextItemConst(&meshDataPtr->meshFaceDataArrayOrList, meshFaceDataPtr))
	{
		if (e3meshFaceData_NumContours(meshFaceDataPtr) == 1)
		{
			const TE3MeshContourData* meshContourDataPtr = e3meshContourDataArrayOrList_FirstItemConst(&meshFaceDataPtr->meshContourDataArrayOrList);
			TQ3Uns32 numberOfContourVertices = e3meshContourData_NumVertices(meshContourDataPtr);
			
			for (meshVertexIntRefPtr = e3meshVertexIntRefArray_FirstItemConst(&meshContourDataPtr->meshVertexIntRefArray), i = 0;
				meshVertexIntRefPtr != NULL;
				meshVertexIntRefPtr = e3meshVertexIntRefArray_NextItemConst(&meshContourDataPtr->meshVertexIntRefArray, meshVertexIntRefPtr), ++i)
			{
				TE3MeshVertexData* meshVertexDataPtr = e3meshVertexIntRef_DataInMesh(*meshVertexIntRefPtr, meshDataPtr);
				TQ3Uns32 vertexIndex = e3meshVertexDataArray_ItemIndex(&meshDataPtr->meshVertexDataArrayOrList.array, meshVertexDataPtr);
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
					polyhedronData.triangles[k].triangleAttributeSet = meshFaceDataPtr->faceAttributeSet;
					++k;
					break;
				}
					
			}
		}
	}
	polyhedronData.polyhedronAttributeSet = meshDataPtr->meshAttributeSet;

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
static TQ3Status
e3geom_mesh_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_mesh_bounds : Mesh bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
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
static TQ3AttributeSet *
e3geom_mesh_get_attribute(TQ3GeometryObject theObject)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(theObject, kQ3GeometryTypeMesh);


	// Return the address of the geometry attribute set
	return(&meshDataPtr->meshAttributeSet);
}





//=============================================================================
//      e3geom_mesh_metahandler : Mesh metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
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
E3Mesh_FaceNew(TQ3GeometryObject mesh, TQ3Uns32 numVertices, const TE3MeshVertexExtRef* meshVertexExtRefs,
	TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->meshFaceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure_1;

	// Push back new uninitialized face data item into mesh
	if ((meshFaceDataPtr = e3meshFaceDataList_PushBackItem(&meshDataPtr->meshFaceDataArrayOrList.list, NULL)) == NULL)
		goto failure_2;

	// Create face data, with reference
	if (e3meshFaceData_Create(meshFaceDataPtr, meshDataPtr, kQ3True, 1, &numVertices, &meshVertexExtRefs, attributeSet) == kQ3Failure)
		goto failure_3;

	Q3Shared_Edited(mesh);

	return(e3meshFaceData_ExtRefInMesh(meshFaceDataPtr, meshDataPtr));
	
	// Dead code to reverse e3meshFaceData_Create
failure_3:

	e3meshFaceDataList_EraseItem(&meshDataPtr->meshFaceDataArrayOrList.list, NULL, meshFaceDataPtr);
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
E3Mesh_FaceDelete(TQ3GeometryObject mesh, TE3MeshFaceExtRef meshFaceExtRef)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Check face; if face already deleted, return kQ3Success
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto success;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->meshFaceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck face (in case relocated)
	meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef);

	// Erase face data item
	e3meshFaceDataList_EraseItem(&meshDataPtr->meshFaceDataArrayOrList.list, e3meshFaceData_Destroy,
		meshFaceDataPtr);

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
E3Mesh_FaceToContour(TQ3GeometryObject mesh, TE3MeshFaceExtRef containerMeshFaceExtRef, TE3MeshFaceExtRef meshFaceExtRef)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* containerMeshFaceDataPtr;
	TE3MeshFaceIntRef containerMeshFaceIntRef;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshContourExtRef meshContourExtRef;

	// Check container face
	if ((containerMeshFaceDataPtr = e3meshFaceExtRef_Data(containerMeshFaceExtRef)) == NULL)
		goto failure;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->meshFaceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck container face (in case relocated)
	containerMeshFaceDataPtr = e3meshFaceExtRef_Data(containerMeshFaceExtRef);

	// Recheck face (in case relocated)
	meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef);
	
	// Get internal reference to container mesh face
	if ((containerMeshFaceIntRef = e3meshFaceData_IntRefInMesh(containerMeshFaceDataPtr, meshDataPtr)) == NULL)
		goto failure;

	// Use list of contours in container face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&containerMeshFaceDataPtr->meshContourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure;

	// Use list of contours in face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&meshFaceDataPtr->meshContourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure;

	// Get first contour in face
	if ((meshContourDataPtr = e3meshContourDataList_FirstItem(&meshFaceDataPtr->meshContourDataArrayOrList.list)) == NULL)
		goto failure;
	if ((meshContourExtRef = e3meshContourData_ExtRefInMesh(meshContourDataPtr, meshDataPtr)) == NULL)
		goto failure;

	// For each contour in face, reset container face
	for (meshContourDataPtr = e3meshContourDataList_FirstItem(&meshFaceDataPtr->meshContourDataArrayOrList.list);
		meshContourDataPtr != NULL;
		meshContourDataPtr = e3meshContourDataList_NextItem(&meshFaceDataPtr->meshContourDataArrayOrList.list, meshContourDataPtr))
	{
		// Reset container face
		e3meshContourData_SetContainerFaceIntRef(meshContourDataPtr, containerMeshFaceIntRef);
	}

	// Splice contours from face into container face
	e3meshContourDataList_SpliceBackList(&containerMeshFaceDataPtr->meshContourDataArrayOrList.list, &meshFaceDataPtr->meshContourDataArrayOrList.list);

	// Erase face data item from mesh
	e3meshFaceDataList_EraseItem(&meshDataPtr->meshFaceDataArrayOrList.list, e3meshFaceData_Destroy,
		meshFaceDataPtr);

	Q3Shared_Edited(mesh);

	// Return contour
	return(meshContourExtRef);
	
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
E3Mesh_ContourToFace(TQ3GeometryObject mesh, TE3MeshContourExtRef meshContourExtRef)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshFaceIntRef containerMeshFaceIntRef;
	TE3MeshFaceData* containerMeshFaceDataPtr;
	TE3MeshFaceData* meshFaceDataPtr;

	// Check contour
	if ((meshContourDataPtr = e3meshContourExtRef_Data(meshContourExtRef)) == NULL)
		goto failure_1;
		
	// Get and check container face
	containerMeshFaceIntRef = e3meshContourData_ContainerFaceIntRef(meshContourDataPtr);
	if ((containerMeshFaceDataPtr = e3meshFaceIntRef_DataInMesh(containerMeshFaceIntRef, meshDataPtr)) == NULL)
		goto failure_2;
	if (e3meshFaceData_NumContours(containerMeshFaceDataPtr) == 1)
		goto success;
	
	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->meshFaceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure_3;
		
	// Recheck container face (in case relocated)
	containerMeshFaceIntRef = e3meshContourData_ContainerFaceIntRef(meshContourDataPtr);
	if ((containerMeshFaceDataPtr = e3meshFaceIntRef_DataInMesh(containerMeshFaceIntRef, meshDataPtr)) == NULL)
		goto failure_4;

	// Push back new uninitialized face data item into mesh
	if ((meshFaceDataPtr = e3meshFaceDataList_PushBackItem(&meshDataPtr->meshFaceDataArrayOrList.list, NULL)) == NULL)
		goto failure_5;

	// Create face data with no contours, with reference
	if (e3meshFaceData_Create(meshFaceDataPtr, meshDataPtr, kQ3True, 0, NULL, NULL, NULL) == kQ3Failure)
		goto failure_6;

	// Use list of contours in container face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&containerMeshFaceDataPtr->meshContourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure_7;

	// Use list of contours in face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&meshFaceDataPtr->meshContourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure_8;

	// Splice contour from container face into new face
	e3meshContourDataList_SpliceBackList(&containerMeshFaceDataPtr->meshContourDataArrayOrList.list, &meshFaceDataPtr->meshContourDataArrayOrList.list);

	Q3Shared_Edited(mesh);

success:

	// Return face
	return(e3meshFaceData_ExtRefInMesh(meshFaceDataPtr, meshDataPtr));
	
	// Dead code to reverse e3meshContourDataArrayOrList_UseList
failure_8:
	
failure_7:
	
	e3meshFaceData_Destroy(meshFaceDataPtr);
failure_6:

	e3meshFaceDataList_EraseItem(&meshDataPtr->meshFaceDataArrayOrList.list, NULL, meshFaceDataPtr);
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
	TE3MeshVertexData* meshVertexDataPtr;

	// Use list of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3meshVertexDataArrayOrList_UseList(&meshDataPtr->meshVertexDataArrayOrList, e3meshVertexData_Relocate) == kQ3Failure)
		goto failure_1;

	// Push back new uninitialized vertex data item into mesh
	if ((meshVertexDataPtr = e3meshVertexDataList_PushBackItem(&meshDataPtr->meshVertexDataArrayOrList.list, NULL)) == NULL)
		goto failure_2;

	// Create vertex data, with reference
	if (e3meshVertexData_Create(meshVertexDataPtr, meshDataPtr, kQ3True, vertexPtr) == kQ3Failure)
		goto failure_3;

	Q3Shared_Edited(mesh);

	return(e3meshVertexData_ExtRefInMesh(meshVertexDataPtr, meshDataPtr));
	
	// Dead code to reverse e3meshVertexData_Create
failure_3:

	e3meshVertexDataList_EraseItem(&meshDataPtr->meshVertexDataArrayOrList.list, NULL, meshVertexDataPtr);
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
E3Mesh_VertexDelete(TQ3GeometryObject mesh, TE3MeshVertexExtRef meshVertexExtRef)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshVertexIntRef meshVertexIntRef;
	TE3MeshFaceData* meshFaceDataPtr;

	// Check vertex; if vertex already deleted, return kQ3Success
	if ((meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRef)) == NULL)
		goto success;

	// Use list of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3meshVertexDataArrayOrList_UseList(&meshDataPtr->meshVertexDataArrayOrList, e3meshVertexData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck vertex (in case relocated)
	meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRef);
	
	// Get internal reference to mesh vertex
	if ((meshVertexIntRef = e3meshVertexData_IntRefInMesh(meshVertexDataPtr, meshDataPtr)) == NULL)
		goto failure;

	// Get first face in mesh
	meshFaceDataPtr = e3meshFaceDataArrayOrList_FirstItem(&meshDataPtr->meshFaceDataArrayOrList);

	// Delete each face having vertex
	while (meshFaceDataPtr != NULL)
	{
		TE3MeshFaceData* markedMeshFaceDataPtr = NULL;

		// Check if face has vertex
		if (e3meshFaceData_HasVertexIntRef(meshFaceDataPtr, meshVertexIntRef))
		{
			TE3MeshFaceExtRef meshFaceExtRef = NULL;

			// Save face
			if ((meshFaceExtRef = e3meshFaceData_ExtRefInMesh(meshFaceDataPtr, meshDataPtr)) == NULL)
				goto failure;

			// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
			if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->meshFaceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
				goto failure;

			// Restore face (in case relocated)
			if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
				goto failure;

			// Mark face for erasure
			markedMeshFaceDataPtr = meshFaceDataPtr;
		}

		// Get next face in mesh
		meshFaceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->meshFaceDataArrayOrList, meshFaceDataPtr);

		// If face marked for erasure, erase face data item
		if (markedMeshFaceDataPtr)
			e3meshFaceDataList_EraseItem(&meshDataPtr->meshFaceDataArrayOrList.list, e3meshFaceData_Destroy,
				markedMeshFaceDataPtr);
	}

	// Erase vertex data item from mesh
	e3meshVertexDataList_EraseItem(&meshDataPtr->meshVertexDataArrayOrList.list, e3meshVertexData_Destroy,
		meshVertexDataPtr);

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
E3Mesh_FirstMeshComponent(TQ3GeometryObject mesh, TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_NextMeshComponent(TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_FirstMeshFace(TQ3GeometryObject mesh, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshFaceExtRef meshFaceExtRef;
	
	// Initialize iterator
	e3meshIterator_Initialize(meshIteratorPtr, meshDataPtr, "mefa");

	// Get and save first face in mesh
	if ((meshFaceDataPtr = e3meshFaceDataArrayOrList_FirstItem(&meshDataPtr->meshFaceDataArrayOrList)) == NULL)
		goto failure;
	if ((meshFaceExtRef = e3meshFaceData_ExtRefInMesh(meshFaceDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshFaceExtRef;

	// Return first face in mesh
	return(meshFaceExtRef);
	
failure:

	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshFace : Get next face in mesh.
//-----------------------------------------------------------------------------
//		Errors :	If iterator ended, current face deleted, no next face or
//					unable to create external reference to face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_NextMeshFace(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshFaceExtRef meshFaceExtRef;
	TE3MeshFaceData* meshFaceDataPtr;

	// Restore and check current face in mesh
	if ((meshFaceExtRef = (TE3MeshFaceExtRef) meshIteratorPtr->var1) == NULL)
		goto failure;
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;

	// Get and save next face in mesh
	if ((meshFaceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->meshFaceDataArrayOrList, meshFaceDataPtr)) == NULL)
		goto failure;
	if ((meshFaceExtRef = e3meshFaceData_ExtRefInMesh(meshFaceDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshFaceExtRef;

	// Return next face in mesh
	return(meshFaceExtRef);
	
failure:

	meshIteratorPtr->var1 = NULL;
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
E3Mesh_FirstMeshEdge(TQ3GeometryObject mesh, TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_NextMeshEdge(TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_FirstMeshVertex(TQ3GeometryObject mesh, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshVertexExtRef meshVertexExtRef;
	
	// Initialize iterator
	e3meshIterator_Initialize(meshIteratorPtr, meshDataPtr, "meve");

	// Get and save first vertex in mesh
	if ((meshVertexDataPtr = e3meshVertexDataArrayOrList_FirstItem(&meshDataPtr->meshVertexDataArrayOrList)) == NULL)
		goto failure;
	if ((meshVertexExtRef = e3meshVertexData_ExtRefInMesh(meshVertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshVertexExtRef;

	// Return first vertex in mesh
	return(meshVertexExtRef);
	
failure:

	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshVertex : Get next vertex in mesh.
//-----------------------------------------------------------------------------
//		Errors :	If iterator ended, current vertex deleted, no next vertex or
//					unable to create external reference to vertex, return NULL.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_NextMeshVertex(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshVertexExtRef meshVertexExtRef;
	TE3MeshVertexData* meshVertexDataPtr;

	// Restore and check current vertex in mesh
	if ((meshVertexExtRef = (TE3MeshVertexExtRef) meshIteratorPtr->var1) == NULL)
		goto failure;
	if ((meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRef)) == NULL)
		goto failure;

	// Get and save next vertex in mesh
	if ((meshVertexDataPtr = e3meshVertexDataArrayOrList_NextItem(&meshDataPtr->meshVertexDataArrayOrList, meshVertexDataPtr)) == NULL)
		goto failure;
	if ((meshVertexExtRef = e3meshVertexData_ExtRefInMesh(meshVertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshVertexExtRef;

	// Return next vertex in mesh
	return(meshVertexExtRef);
	
failure:

	meshIteratorPtr->var1 = NULL;
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
E3Mesh_GetComponentOrientable(TQ3GeometryObject mesh, TE3MeshComponentExtRef meshComponentExtRef, TQ3Boolean *orientable)
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
E3Mesh_GetComponentBoundingBox(TQ3GeometryObject mesh, TE3MeshComponentExtRef meshComponentExtRef, TQ3BoundingBox *boundingBox)
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
E3Mesh_GetComponentNumEdges(TQ3GeometryObject mesh, TE3MeshComponentExtRef meshComponentExtRef, TQ3Uns32 *numEdges)
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
E3Mesh_FirstComponentEdge(TE3MeshComponentExtRef meshComponentExtRef, TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_NextComponentEdge(TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_GetComponentNumVertices(TQ3GeometryObject mesh, TE3MeshComponentExtRef meshComponentExtRef, TQ3Uns32 *numVertices)
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
E3Mesh_FirstComponentVertex(TE3MeshComponentExtRef meshComponentExtRef, TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_NextComponentVertex(TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_GetFaceIndex(TQ3GeometryObject mesh, TE3MeshFaceExtRef meshFaceExtRef, TQ3Uns32 *index)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;

	// Use array of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseArray(&meshDataPtr->meshFaceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck face (in case relocated)
	meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef);

	// Get index of face
	*index = e3meshFaceDataArray_ItemIndex(&meshDataPtr->meshFaceDataArrayOrList.array, meshFaceDataPtr);

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
E3Mesh_GetFacePlaneEquation(TQ3GeometryObject mesh, TE3MeshFaceExtRef meshFaceExtRef, TQ3PlaneEquation *planeEquation)
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
E3Mesh_GetFaceAttributeSet(TQ3GeometryObject mesh, TE3MeshFaceExtRef meshFaceExtRef, TQ3AttributeSet *attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;

	// Get attribute set
	E3Shared_Acquire(attributeSet, meshFaceDataPtr->faceAttributeSet);

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
E3Mesh_SetFaceAttributeSet(TQ3GeometryObject mesh, TE3MeshFaceExtRef meshFaceExtRef, TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;

	// Set attribute set
	E3Shared_Replace(&meshFaceDataPtr->faceAttributeSet, attributeSet);

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
E3Mesh_GetFaceComponent(TQ3GeometryObject mesh, TE3MeshFaceExtRef meshFaceExtRef, TE3MeshComponentExtRef* componentExtRefPtr)
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
E3Mesh_FirstFaceFace(TE3MeshFaceExtRef meshFaceExtRef, TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_NextFaceFace(TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_GetFaceNumContours(TQ3GeometryObject mesh, TE3MeshFaceExtRef meshFaceExtRef, TQ3Uns32 *numContours)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;

	// Get number of contours in face
	*numContours = e3meshFaceData_NumContours(meshFaceDataPtr);

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
E3Mesh_FirstFaceContour(TE3MeshFaceExtRef meshFaceExtRef, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshContourExtRef meshContourExtRef;

	// Get mesh for face
	if ((meshDataPtr = e3meshFaceExtRef_MeshData(meshFaceExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(meshIteratorPtr, meshDataPtr, "fact");

	// Check and save face
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;
	meshIteratorPtr->var2 = meshFaceExtRef;

	// Get and save first contour in face
	if ((meshContourDataPtr = e3meshContourDataArrayOrList_FirstItem(&meshFaceDataPtr->meshContourDataArrayOrList)) == NULL)
		goto failure;
	if ((meshContourExtRef = e3meshContourData_ExtRefInMesh(meshContourDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshContourExtRef;

	// Return first contour in face
	return(meshContourExtRef);
	
failure:

	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
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
E3Mesh_NextFaceContour(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshFaceExtRef meshFaceExtRef;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshContourExtRef meshContourExtRef;
	TE3MeshContourData* meshContourDataPtr;

	// Restore and check face
	if ((meshFaceExtRef = (TE3MeshFaceExtRef) meshIteratorPtr->var2) == NULL)
		goto failure;
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;

	// Restore and check current contour in face
	if ((meshContourExtRef = (TE3MeshContourExtRef) meshIteratorPtr->var1) == NULL)
		goto failure;
	if ((meshContourDataPtr = e3meshContourExtRef_Data(meshContourExtRef)) == NULL)
		goto failure;

	// Get and save next contour in face
	if ((meshContourDataPtr = e3meshContourDataArrayOrList_NextItem(&meshFaceDataPtr->meshContourDataArrayOrList, meshContourDataPtr)) == NULL)
		goto failure;
	if ((meshContourExtRef = e3meshContourData_ExtRefInMesh(meshContourDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshContourExtRef;

	// Return next contour in face
	return(meshContourExtRef);
	
failure:

	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstFaceEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_FirstFaceEdge(TE3MeshFaceExtRef meshFaceExtRef, TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_NextFaceEdge(TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_GetFaceNumVertices(TQ3GeometryObject mesh, TE3MeshFaceExtRef meshFaceExtRef, TQ3Uns32 *numVertices)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;

	*numVertices = e3meshFaceData_NumVertices(meshFaceDataPtr);

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
E3Mesh_FirstFaceVertex(TE3MeshFaceExtRef meshFaceExtRef, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshContourExtRef meshContourExtRef;
	TE3MeshVertexIntRef* meshVertexIntRefPtr;
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshVertexExtRef meshVertexExtRef;

	// Get mesh for face
	if ((meshDataPtr = e3meshFaceExtRef_MeshData(meshFaceExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(meshIteratorPtr, meshDataPtr, "fave");

	// Check and save face
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;
	meshIteratorPtr->var3 = meshFaceExtRef;

	// Get first contour in face
	if ((meshContourDataPtr = e3meshContourDataArrayOrList_FirstItem(&meshFaceDataPtr->meshContourDataArrayOrList)) == NULL)
		goto failure;

	// Get first vertex in face
	for (;;)
	{
		// If first vertex in current contour exists, break
		if ((meshVertexIntRefPtr = e3meshVertexIntRefArray_FirstItem(&meshContourDataPtr->meshVertexIntRefArray)) != NULL)
			break;

		// Otherwise, get next contour in face
		if ((meshContourDataPtr = e3meshContourDataArrayOrList_NextItem(&meshFaceDataPtr->meshContourDataArrayOrList, meshContourDataPtr)) == NULL)
			goto failure;
	}

	// Save current contour in face
	if ((meshContourExtRef = e3meshContourData_ExtRefInMesh(meshContourDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var2 = meshContourExtRef;

	// Save first vertex in face
	meshVertexDataPtr = e3meshVertexIntRef_DataInMesh(*meshVertexIntRefPtr, meshDataPtr);
	if ((meshVertexExtRef = e3meshVertexData_ExtRefInMesh(meshVertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshVertexIntRefPtr;

	// Return first vertex in face
	return(meshVertexExtRef);
	
failure:

	meshIteratorPtr->var3 = NULL;
	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
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
E3Mesh_NextFaceVertex(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshFaceExtRef meshFaceExtRef;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshContourExtRef meshContourExtRef;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshVertexIntRef* meshVertexIntRefPtr;
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshVertexExtRef meshVertexExtRef;

	// Restore and check face
	if ((meshFaceExtRef = (TE3MeshFaceExtRef) meshIteratorPtr->var3) == NULL)
		goto failure;
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;

	// Restore and check current contour
	if ((meshContourExtRef = (TE3MeshContourExtRef) meshIteratorPtr->var2) == NULL)
		goto failure;
	if ((meshContourDataPtr = e3meshContourExtRef_Data(meshContourExtRef)) == NULL)
		goto failure;

	// ??? check that contour still belongs to face ???

	// Restore and check current vertex
	if ((meshVertexIntRefPtr = (TE3MeshVertexIntRef*) meshIteratorPtr->var1) == NULL)
		goto failure;

	// If next vertex in current contour exists, break
	if ((meshVertexIntRefPtr = e3meshVertexIntRefArray_NextItem(&meshContourDataPtr->meshVertexIntRefArray, meshVertexIntRefPtr)) != NULL)
		;
	// Otherwise, get next vertex in face
	else
	{
		for (;;)
		{
			// Get next contour in face
			if ((meshContourDataPtr = e3meshContourDataArrayOrList_NextItem(&meshFaceDataPtr->meshContourDataArrayOrList, meshContourDataPtr)) == NULL)
				goto failure;
				
			// If first vertex in current contour exists, break
			if ((meshVertexIntRefPtr = e3meshVertexIntRefArray_FirstItem(&meshContourDataPtr->meshVertexIntRefArray)) != NULL)
				break;
		}
	}

	// Save current contour in face
	if ((meshContourExtRef = e3meshContourData_ExtRefInMesh(meshContourDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var2 = meshContourExtRef;

	// Save next vertex in face
	meshVertexDataPtr = e3meshVertexIntRef_DataInMesh(*meshVertexIntRefPtr, meshDataPtr);
	if ((meshVertexExtRef = e3meshVertexData_ExtRefInMesh(meshVertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshVertexIntRefPtr;

	// Return next vertex in face
	return(meshVertexExtRef);
	
failure:

	meshIteratorPtr->var3 = NULL;
	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
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
E3Mesh_GetContourFace(TQ3GeometryObject mesh, TE3MeshContourExtRef meshContourExtRef, TE3MeshFaceExtRef* containerMeshFaceExtRefPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshFaceIntRef containerMeshFaceIntRef;
	TE3MeshFaceData* containerMeshFaceDataPtr;
	
	// Check contour
	if ((meshContourDataPtr = e3meshContourExtRef_Data(meshContourExtRef)) == NULL)
		goto failure;
	
	// Get external reference to container face
	containerMeshFaceIntRef = e3meshContourData_ContainerFaceIntRef(meshContourDataPtr);
	containerMeshFaceDataPtr = e3meshFaceIntRef_DataInMesh(containerMeshFaceIntRef, meshDataPtr);
	if ((*containerMeshFaceExtRefPtr = e3meshFaceData_ExtRefInMesh(containerMeshFaceDataPtr, meshDataPtr)) == NULL)
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
E3Mesh_FirstContourFace(TE3MeshContourExtRef meshContourExtRef, TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_NextContourFace(TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_FirstContourEdge(TE3MeshContourExtRef meshContourExtRef, TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_NextContourEdge(TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_GetContourNumVertices(TQ3GeometryObject mesh, TE3MeshContourExtRef meshContourExtRef, TQ3Uns32 *numVertices)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshContourData* meshContourDataPtr;

	// Check contour
	if ((meshContourDataPtr = e3meshContourExtRef_Data(meshContourExtRef)) == NULL)
		goto failure;

	// Get number of vertices in contour
	*numVertices = e3meshContourData_NumVertices(meshContourDataPtr);

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
E3Mesh_FirstContourVertex(TE3MeshContourExtRef meshContourExtRef, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshVertexIntRef* meshVertexIntRefPtr;
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshVertexExtRef meshVertexExtRef;

	// Get mesh for contour
	if ((meshDataPtr = e3meshContourExtRef_MeshData(meshContourExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(meshIteratorPtr, meshDataPtr, "ctve");

	// Check and save contour
	if ((meshContourDataPtr = e3meshContourExtRef_Data(meshContourExtRef)) == NULL)
		goto failure;
	meshIteratorPtr->var2 = meshContourExtRef;

	// Get and save first vertex in contour
	if ((meshVertexIntRefPtr = e3meshVertexIntRefArray_FirstItem(&meshContourDataPtr->meshVertexIntRefArray)) == NULL)
		goto failure;
	meshVertexDataPtr = e3meshVertexIntRef_DataInMesh(*meshVertexIntRefPtr, meshDataPtr);
	if ((meshVertexExtRef = e3meshVertexData_ExtRefInMesh(meshVertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshVertexIntRefPtr;

	// Return first vertex in contour
	return(meshVertexExtRef);
	
failure:

	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
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
E3Mesh_NextContourVertex(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshContourExtRef meshContourExtRef;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshVertexIntRef* meshVertexIntRefPtr;
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshVertexExtRef meshVertexExtRef;

	// Restore and check contour
	if ((meshContourExtRef = (TE3MeshContourExtRef) meshIteratorPtr->var2) == NULL)
		goto failure;
	if ((meshContourDataPtr = e3meshContourExtRef_Data(meshContourExtRef)) == NULL)
		goto failure;

	// Restore and check current vertex in contour
	if ((meshVertexIntRefPtr = (TE3MeshVertexIntRef*) meshIteratorPtr->var1) == NULL)
		goto failure;

	// Get and save next vertex in contour
	if ((meshVertexIntRefPtr = e3meshVertexIntRefArray_NextItem(&meshContourDataPtr->meshVertexIntRefArray, meshVertexIntRefPtr)) == NULL)
		goto failure;
	meshVertexDataPtr = e3meshVertexIntRef_DataInMesh(*meshVertexIntRefPtr, meshDataPtr);
	if ((meshVertexExtRef = e3meshVertexData_ExtRefInMesh(meshVertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshVertexIntRefPtr;

	// Return next vertex in contour
	return(meshVertexExtRef);
	
failure:

	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetEdgeOnBoundary : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetEdgeOnBoundary(TQ3GeometryObject mesh, TE3MeshEdgeExtRef meshEdge, TQ3Boolean *onBoundary)
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
E3Mesh_GetEdgeAttributeSet(TQ3GeometryObject mesh, TE3MeshEdgeExtRef meshEdge, TQ3AttributeSet *attributeSet)
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
E3Mesh_SetEdgeAttributeSet(TQ3GeometryObject mesh, TE3MeshEdgeExtRef meshEdge, TQ3AttributeSet attributeSet)
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
E3Mesh_GetEdgeComponent(TQ3GeometryObject mesh, TE3MeshEdgeExtRef meshEdge, TE3MeshComponentExtRef* componentExtRefPtr)
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
E3Mesh_GetEdgeFaces(TQ3GeometryObject mesh, TE3MeshEdgeExtRef meshEdge, TE3MeshFaceExtRef* faceExtRefPtr1, TE3MeshFaceExtRef* faceExtRefPtr2)
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
E3Mesh_GetEdgeVertices(TQ3GeometryObject mesh, TE3MeshEdgeExtRef meshEdge,
	TE3MeshVertexExtRef* meshVertexExtRefPtr1, TE3MeshVertexExtRef* meshVertexExtRefPtr2)
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
E3Mesh_GetVertexIndex(TQ3GeometryObject mesh, TE3MeshVertexExtRef meshVertexExtRef, TQ3Uns32 *index)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;

	// Check vertex
	if ((meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRef)) == NULL)
		goto failure;

	// Use array of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3meshVertexDataArrayOrList_UseArray(&meshDataPtr->meshVertexDataArrayOrList, e3meshVertexData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck vertex (in case relocated)
	meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRef);

	// Get index of vertex
	*index = e3meshVertexDataArray_ItemIndex(&meshDataPtr->meshVertexDataArrayOrList.array, meshVertexDataPtr);

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
E3Mesh_GetVertexOnBoundary(TQ3GeometryObject mesh, TE3MeshVertexExtRef meshVertexExtRef, TQ3Boolean *onBoundary)
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
E3Mesh_GetVertexCoordinates(TQ3GeometryObject mesh, TE3MeshVertexExtRef meshVertexExtRef, TQ3Point3D *coordinates)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;

	// Check vertex
	if ((meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRef)) == NULL)
		goto failure;

	// Get vertex coordinates
	*coordinates = meshVertexDataPtr->point;

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
E3Mesh_SetVertexCoordinates(TQ3GeometryObject mesh, TE3MeshVertexExtRef meshVertexExtRef, const TQ3Point3D *coordinates)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;

	// Check vertex
	if ((meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRef)) == NULL)
		goto failure;

	// Set vertex coordinates
	meshVertexDataPtr->point = *coordinates;

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
E3Mesh_GetVertexAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexExtRef meshVertexExtRef, TQ3AttributeSet *attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;

	// Check vertex
	if ((meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRef)) == NULL)
		goto failure;

	// Get vertex attribute set
	E3Shared_Acquire(attributeSet, meshVertexDataPtr->vertexAttributeSet);

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
E3Mesh_SetVertexAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexExtRef meshVertexExtRef, TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;

	// Check vertex
	if ((meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRef)) == NULL)
		goto failure;

	// Set vertex attribute set
	E3Shared_Replace(&meshVertexDataPtr->vertexAttributeSet, attributeSet);

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
E3Mesh_GetVertexComponent(TQ3GeometryObject mesh, TE3MeshVertexExtRef meshVertexExtRef, TE3MeshComponentExtRef* componentExtRefPtr)
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
E3Mesh_FirstVertexFace(TE3MeshVertexExtRef meshVertexExtRef, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshVertexIntRef meshVertexIntRef;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshFaceExtRef meshFaceExtRef;

	// Get mesh for vertex
	if ((meshDataPtr = e3meshVertexExtRef_MeshData(meshVertexExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(meshIteratorPtr, meshDataPtr, "vefa");

	// Check and save vertex
	if ((meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRef)) == NULL)
		goto failure;
	if ((meshVertexIntRef = e3meshVertexData_IntRefInMesh(meshVertexDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var2 = meshVertexExtRef;

	// Get first face in mesh
	if ((meshFaceDataPtr = e3meshFaceDataArrayOrList_FirstItem(&meshDataPtr->meshFaceDataArrayOrList)) == NULL)
		goto failure;

	// Get first face having vertex
	for (;;)
	{
		// If face has vertex, break
		if (e3meshFaceData_HasVertexIntRef(meshFaceDataPtr, meshVertexIntRef))
			break;

		// Otherwise, get next face in mesh
		if ((meshFaceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->meshFaceDataArrayOrList, meshFaceDataPtr)) == NULL)
			goto failure;
	}

	// Save first face having vertex
	if ((meshFaceExtRef = e3meshFaceData_ExtRefInMesh(meshFaceDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshFaceExtRef;

	// Return first face having vertex
	return(meshFaceExtRef);
	
failure:

	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
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
E3Mesh_NextVertexFace(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshVertexExtRef meshVertexExtRef;
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshVertexIntRef meshVertexIntRef;
	TE3MeshFaceExtRef meshFaceExtRef;
	TE3MeshFaceData* meshFaceDataPtr;

	// Restore and check vertex
	if ((meshVertexExtRef = (TE3MeshVertexExtRef) meshIteratorPtr->var2) == NULL)
		goto failure;
	if ((meshVertexDataPtr = e3meshVertexExtRef_Data(meshVertexExtRef)) == NULL)
		goto failure;

	// Restore and check current face having vertex
	if ((meshFaceExtRef = (TE3MeshFaceExtRef) meshIteratorPtr->var1) == NULL)
		goto failure;
	if ((meshFaceDataPtr = e3meshFaceExtRef_Data(meshFaceExtRef)) == NULL)
		goto failure;
	if ((meshVertexIntRef = e3meshVertexData_IntRefInMesh(meshVertexDataPtr, meshDataPtr)) == NULL)
		goto failure;

	// Get next face having vertex
	for (;;)
	{
		// Get next face in mesh
		if ((meshFaceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->meshFaceDataArrayOrList, meshFaceDataPtr)) == NULL)
			goto failure;

		// If face has vertex, break
		if (e3meshFaceData_HasVertexIntRef(meshFaceDataPtr, meshVertexIntRef))
			break;
	}

	// Save next face having vertex
	if ((meshFaceExtRef = e3meshFaceData_ExtRefInMesh(meshFaceDataPtr, meshDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshFaceExtRef;

	// Return next face having vertex
	return(meshFaceExtRef);
	
failure:

	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstVertexEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_FirstVertexEdge(TE3MeshVertexExtRef meshVertexExtRef, TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_NextVertexEdge(TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_FirstVertexVertex(TE3MeshVertexExtRef meshVertexExtRef, TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_NextVertexVertex(TQ3MeshIterator* meshIteratorPtr)
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
E3Mesh_GetCornerAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexExtRef meshVertexExtRef, TE3MeshFaceExtRef meshFaceExtRef, TQ3AttributeSet *attributeSet)
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
E3Mesh_SetCornerAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexExtRef meshVertexExtRef, TE3MeshFaceExtRef meshFaceExtRef, TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	Q3Shared_Edited(mesh);

	return(kQ3Failure);
}

/*  NAME:
		E3GeometryMesh.c

	DESCRIPTION:
		Implementation of Quesa Mesh geometry class.

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
#include "E3View.h"
#include "E3Geometry.h"
#include "E3GeometryMesh.h"
#include "E3ArrayOrList.h"
#include "E3Pool.h"
#include "GLPrefix.h"





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
// Part and mesh data
typedef struct TE3MeshPartData TE3MeshPartData;
typedef struct TE3MeshCornerData TE3MeshCornerData;
typedef struct TE3MeshVertexData TE3MeshVertexData;
typedef struct TE3MeshContourData TE3MeshContourData;
typedef struct TE3MeshFaceData TE3MeshFaceData;
typedef struct TE3MeshData TE3MeshData;



// External references to parts - distinct types
typedef TQ3MeshVertex TE3MeshVertexExtRef;
typedef TQ3MeshContour TE3MeshContourExtRef;
typedef TQ3MeshFace TE3MeshFaceExtRef;
typedef TQ3MeshEdge TE3MeshEdgeExtRef;
typedef TQ3MeshComponent TE3MeshComponentExtRef;



// TE3MeshPartPtr
typedef TE3MeshPartData* TE3MeshPartPtr;

E3_POOL_DECLARE(TE3MeshPartPtr, e3meshPartPtr, static);
E3_POOL_DEFINE(TE3MeshPartPtr, e3meshPartPtr, static, 16);



// TE3MeshVertexPtr
typedef TE3MeshVertexData* TE3MeshVertexPtr;

E3_PTR_ARRAY_DECLARE(TE3MeshVertexPtr, e3meshVertexPtr, static);
E3_PTR_ARRAY_DEFINE(TE3MeshVertexPtr, e3meshVertexPtr, static, kE3MeshVertexPtr);



// TE3MeshPartData
struct TE3MeshPartData {
	union
	{
		TE3MeshPartData*					newPartPtr;
		TE3MeshPartData**					partHdl;
	}								partPtrOrHdl;
};


// TE3MeshContourData
struct TE3MeshContourData {
	TE3MeshPartData					part;					// base class
	TE3MeshFaceData*				containerFacePtr;
	TE3MeshVertexPtrArray			vertexPtrArray;
};

E3_ARRAY_OR_LIST_DECLARE(TE3MeshContourData, e3meshContour, static);
E3_ARRAY_OR_LIST_DEFINE(TE3MeshContourData, e3meshContour, static, kE3MeshContour);



// TE3MeshFaceData
struct TE3MeshFaceData {
	TE3MeshPartData					part;					// base class
	TE3MeshContourDataArrayOrList	contourArrayOrList;
	TQ3AttributeSet					attributeSet;
};

E3_ARRAY_OR_LIST_DECLARE(TE3MeshFaceData, e3meshFace, static);
E3_ARRAY_OR_LIST_DEFINE(TE3MeshFaceData, e3meshFace, static, kE3MeshFace);

// TE3MeshFacePtr
typedef TE3MeshFaceData* TE3MeshFacePtr;

E3_PTR_ARRAY_OR_LIST_DECLARE(TE3MeshFacePtr, e3meshFacePtr, static);
E3_PTR_ARRAY_OR_LIST_DEFINE(TE3MeshFacePtr, e3meshFacePtr, static, kE3MeshFacePtr);



// TE3MeshCornerData
struct TE3MeshCornerData {
	// *** A corner is NOT a part! ***
	TE3MeshFacePtrArrayOrList		facePtrArrayOrList;
	TQ3AttributeSet					attributeSet;
};

E3_ARRAY_OR_LIST_DECLARE(TE3MeshCornerData, e3meshCorner, static);
E3_ARRAY_OR_LIST_DEFINE(TE3MeshCornerData, e3meshCorner, static, kE3MeshCorner);



// TE3MeshVertexData
struct TE3MeshVertexData {
	TE3MeshPartData					part;					// base class
	TQ3Point3D						point;
	TE3MeshCornerDataArrayOrList	cornerArrayOrList;
	TQ3AttributeSet					attributeSet;
};

E3_ARRAY_OR_LIST_DECLARE(TE3MeshVertexData, e3meshVertex, static);
E3_ARRAY_OR_LIST_DEFINE(TE3MeshVertexData, e3meshVertex, static, kE3MeshVertex);






// GLU callback
#ifndef CALLBACK
	#define CALLBACK
#endif

typedef void (CALLBACK *GLcallback)();



// TQ3PolyhedronTriangleData
E3_ARRAY_OR_LIST_DECLARE(TQ3PolyhedronTriangleData, e3polyhedronTriangle, static);
E3_ARRAY_OR_LIST_DEFINE(TQ3PolyhedronTriangleData, e3polyhedronTriangle, static, kE3PolyhedronTriangle);



// TE3MeshTessellator
typedef struct TE3MeshTessellator {
	const TE3MeshVertexDataArray* vertexArrayPtr;
	TQ3AttributeSet faceAttributeSet;
	GLUtriangulatorObj* gluTessellatorPtr;
	TQ3PolyhedronTriangleDataArrayOrList polyhedronTriangleArrayOrList;
	TQ3Status status;
	TQ3PolyhedronTriangleData polyhedronTriangle;
	TQ3Boolean isEdge;
	TQ3Uns32 vertexIdx;
} TE3MeshTessellator;



// TE3MeshData
struct TE3MeshData {
	//	Note:	In order for e3meshPartPtr_IsMeshPtr to be an effective function
	//			for recognizing tags in partPtrPool, the first word of a mesh
	//			structure must *not* be, in effect, a handle (TE3MeshData**)
	//			referring back to this mesh! As the first word of a
	//			TE3MeshPartPtrPool is a pointer (to a block or a free item) or
	//			is null, making 'partPtrPool' the first member of a TE3MeshData
	//			ensures that tags will work.
	TE3MeshPartPtrPool				partPtrPool;

	TQ3Uns32						numCorners;
	TE3MeshVertexDataArrayOrList	vertexArrayOrList;
	TE3MeshFaceDataArrayOrList		faceArrayOrList;
	TQ3AttributeSet					attributeSet;
};





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3meshPartPtr_Relink : Relink pointer to part.
//-----------------------------------------------------------------------------
//		Warning :	In contrast to other functions, the parameter for this
//					function is a pointer to a pointer, not a pointer!
//-----------------------------------------------------------------------------
//		Note :		See e3meshPart_Relocate.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshPartPtr_Relink(
	TE3MeshPartPtr* partHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partHdl);
	Q3_ASSERT_VALID_PTR(*partHdl);
	
	(*partHdl) = (*partHdl)->partPtrOrHdl.newPartPtr;

	return(kQ3Success);
}





//=============================================================================
//      e3meshPartPtr_IsMeshPtr :	Return if this part pointer is actually a
//									mesh pointer.
//-----------------------------------------------------------------------------
//		Warning :	In contrast to other functions, the parameter for this
//					function is a pointer to a pointer, not a pointer!
//-----------------------------------------------------------------------------
//		Note :	The e3meshPartPtr_IsMeshPtr function handles only allocated
//				master pointers. Unallocated pointers can be ignored because
//				pointers in the pool are allocated sequentially and the mesh
//				code never unallocates (frees) a master pointer -- except in
//				specific, well-defined cases where a pointer is allocated,
//				found to be unneeded, and immediately unallocated: "pop" then
//				"push".
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3meshPartPtr_IsMeshPtr(
	const TE3MeshPartPtr* partHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partHdl);

	// A null pointer is not a pointer to mesh data	
	if ((*partHdl) == NULL)
		return(kQ3False);

	// If this pointer points to a pointer that points back to this pointer,
	// it is a pointer to part data -- not mesh data	
	if ((*partHdl)->partPtrOrHdl.partHdl == partHdl)
		return(kQ3False);
	
	return(kQ3True);
}





//=============================================================================
//      e3meshCornerPtr_Relink : Relink pointer to corner.
//-----------------------------------------------------------------------------
//		Warning :	In contrast to other functions, the parameter for this
//					function is a pointer to a pointer, not a pointer!
//-----------------------------------------------------------------------------
//		Note :		See e3meshCorner_Relocate.
//-----------------------------------------------------------------------------
#pragma mark -
static
void
e3meshCornerPtr_Relink(
	TE3MeshCornerData** cornerHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerHdl);
	
	// Get pointer to new corner from old corner's attribute set
	(*cornerHdl) = (TE3MeshCornerData*) (*cornerHdl)->attributeSet;
}





//=============================================================================
//      e3meshVertexPtr_Relink : Relink pointer to vertex.
//-----------------------------------------------------------------------------
//		Warning :	In contrast to other functions, the parameter for this
//					function is a pointer to a pointer, not a pointer!
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshVertexPtr_Relink(
	TE3MeshVertexPtr* vertexHdl,
	void* unusedArg)
{
    unusedArg; /* Suppress compiler warning */

	return(e3meshPartPtr_Relink(E3_UP_CAST(TE3MeshPartPtr*, vertexHdl)));
}





//=============================================================================
//      e3meshVertexPtr_Tessellate : Tessellate vertex.
//-----------------------------------------------------------------------------
//		Warning :	In contrast to other functions, the parameter for this
//					function is a pointer to a pointer, not a pointer!
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshVertexPtr_Tessellate(
	TE3MeshVertexPtr* vertexHdl,
	GLUtriangulatorObj* gluTessellatorPtr)
{
	GLdouble vertCoords[3];
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexHdl);
	Q3_ASSERT_VALID_PTR(*vertexHdl);
	Q3_ASSERT_VALID_PTR(gluTessellatorPtr);

	vertCoords[0] = (GLdouble) (*vertexHdl)->point.x;
	vertCoords[1] = (GLdouble) (*vertexHdl)->point.y;
	vertCoords[2] = (GLdouble) (*vertexHdl)->point.z;
	gluTessVertex(gluTessellatorPtr, vertCoords, *vertexHdl);

	return(kQ3Success);
}





//=============================================================================
//      e3meshFacePtr_Relink : Relink pointer to face.
//-----------------------------------------------------------------------------
//		Warning :	In contrast to other functions, the parameter for this
//					function is a pointer to a pointer, not a pointer!
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshFacePtr_Relink(
	TE3MeshFacePtr* faceHdl,
	void* unusedArg)
{
    unusedArg; /* Suppress compiler warning */

	return(e3meshPartPtr_Relink(E3_UP_CAST(TE3MeshPartPtr*, faceHdl)));
}





//=============================================================================
//      e3meshPart_AcquireHandleInMesh : Acquire handle to part.
//-----------------------------------------------------------------------------
//		Note : If unable to acquire handle (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshPart_AcquireHandleInMesh(
	TE3MeshPartData* partPtr,
	TE3MeshData* meshPtr)
{
	TE3MeshPartData** partHdl;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partPtr);
	Q3_ASSERT_VALID_PTR(meshPtr);
	
	if (partPtr->partPtrOrHdl.partHdl == NULL)
	{
		// Allocate master pointer to part from mesh's pool
			// Note: The type of items in the pool is, in effect, a union:
			//
			//		union {
			//			TE3MeshPartData*	partPtr;
			//			TE3MeshData*		meshPtr;
			//		};
			//
			// Regular items in the pool are of type TE3MeshPartData* and tags in
			// the pool are of type TE3MeshData*.
			//
			// Neverthless, for simplicity the type is declared to be simply
			// TE3MeshPartData* and we use a type cast to coerce one pointer
			// type to the other.
		if ((partHdl = e3meshPartPtrPool_AllocateTagged(
			&meshPtr->partPtrPool, (const TE3MeshPartPtr*) &meshPtr)) == NULL)
				goto failure;

		// Initialize master pointer to part
		*partHdl = partPtr;
		
		// Initialize part's handle to self
		partPtr->partPtrOrHdl.partHdl = partHdl;
	}

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshPart_ReleaseHandleInMesh : Release handle, if any, to part.
//-----------------------------------------------------------------------------
//		Warning :	Use this function with care! Releasing the handle to a part
//					that is still referenced will result in a fatal error. 
//-----------------------------------------------------------------------------
static
void
e3meshPart_ReleaseHandleInMesh(
	TE3MeshPartData* partPtr,
	TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partPtr);
	Q3_ASSERT_VALID_PTR(meshPtr);

	// Free master pointer to part in mesh's pool; set part's handle to self to NULL
	e3meshPartPtrPool_Free(&meshPtr->partPtrPool, &partPtr->partPtrOrHdl.partHdl);
}





//=============================================================================
//      e3meshPart_HandleInMesh : Return handle to part.
//-----------------------------------------------------------------------------
//		Note : If unable to get handle (out of memory), return NULL.
//-----------------------------------------------------------------------------
static
TE3MeshPartData**
e3meshPart_HandleInMesh(
	TE3MeshPartData* partPtr,
	TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partPtr);
	Q3_ASSERT_VALID_PTR(meshPtr);

	// Acquire handle to part
	if (e3meshPart_AcquireHandleInMesh(partPtr, meshPtr) == kQ3Failure)
		goto failure;

	return(partPtr->partPtrOrHdl.partHdl);
	
failure:

	return(NULL);
}





//=============================================================================
//      e3meshPart_CreateUnreferenced : TE3MeshPartData constructor.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshPart_CreateUnreferenced(
	TE3MeshPartData* partPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partPtr);

	// Initialize handle
	partPtr->partPtrOrHdl.partHdl = NULL;

	return(kQ3Success);
}





//=============================================================================
//      e3meshPart_Create : TE3MeshPartData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshPart_Create(
	TE3MeshPartData* partPtr,
	TE3MeshData* meshPtr,
	TQ3Boolean isReferenced)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partPtr);
	Q3_ASSERT_VALID_PTR(meshPtr);

	// Initialize handle BEFORE calling e3meshPart_AcquireHandleInMesh
	partPtr->partPtrOrHdl.partHdl = NULL;
	
	// If requested, acquire handle to part
	if (isReferenced)
	{
		if (e3meshPart_AcquireHandleInMesh(partPtr, meshPtr) == kQ3Failure)
			goto failure;
	}

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshPart_Destroy : TE3MeshPartData destructor.
//-----------------------------------------------------------------------------
static
void
e3meshPart_Destroy(
	TE3MeshPartData* partPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partPtr);

	// If part has handle, clear master pointer to part
	if (partPtr->partPtrOrHdl.partHdl != NULL)
		*partPtr->partPtrOrHdl.partHdl = NULL;
}





//=============================================================================
//      e3meshPart_Relocate : Relocate part.
//-----------------------------------------------------------------------------
//		Note : See also e3meshPartPtr_Relink.
//-----------------------------------------------------------------------------
static
void
e3meshPart_Relocate(
	TE3MeshPartData* newPartPtr,
	TE3MeshPartData* oldPartPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(newPartPtr);
	Q3_ASSERT_VALID_PTR(oldPartPtr);

	// If part has handle, update master pointer to part
	if (newPartPtr->partPtrOrHdl.partHdl != NULL)
		*newPartPtr->partPtrOrHdl.partHdl = newPartPtr;
	
	// Save pointer to new part in old part
	oldPartPtr->partPtrOrHdl.newPartPtr = newPartPtr;
}





//=============================================================================
//      e3meshPartHdl_Part : Return part for part handle.
//-----------------------------------------------------------------------------
//		Note : If part deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshPartData*
e3meshPartHdl_Part(
	TE3MeshPartData** partHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partHdl);

	return(*partHdl);
}





//=============================================================================
//      e3meshPartHdl_Mesh : Return mesh for part handle.
//-----------------------------------------------------------------------------
//		Note :	This is a subtle function. It takes a handle (pointer to
//				pointer) to a mesh part (vertex, contour, face, edge or
//				component) and returns the containing mesh, even though parts
//				don't store pointers to their containing mesh. This is possible
//				because part handles point to master pointers in a pool of
//				master pointers, and the pool has been "tagged" with a pointer
//				to the mesh. The subtle part is that the pool can contain
//				allocated non-null or null master pointers, and unallocated
//				pointers. See e3meshPartPtr_IsMeshPtr.
//-----------------------------------------------------------------------------
static
TE3MeshData*
e3meshPartHdl_Mesh(
	TE3MeshPartData** partHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(partHdl);

	return(* (TE3MeshData**) e3meshPartPtrPoolItem_Tag(partHdl, &e3meshPartPtr_IsMeshPtr));
}





//=============================================================================
//      e3meshCorner_CreateEmptyListOfFacePtrs : TE3MeshCornerData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshCorner_CreateEmptyListOfFacePtrs(
	TE3MeshCornerData* cornerPtr,
	TQ3AttributeSet attributeSet)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);

	// Must have a non-null attribute set
	Q3_ASSERT_VALID_PTR(attributeSet);

	// Create empty list of face pointers
	if (e3meshFacePtrList_Create(&cornerPtr->facePtrArrayOrList.list, 0, NULL) == kQ3Failure)
		goto failure;

	// Initialize attribute set
	E3Shared_Acquire(&cornerPtr->attributeSet, attributeSet);

	return(kQ3Success);
	
	// Dead code to reverse e3meshFacePtrList_Create
failure:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshCorner_CreateFromExtData :	TE3MeshCornerData constructor from
//											external data.
//-----------------------------------------------------------------------------
//		Note :	If face indices out-of-range or duplicated, if corner attribute
//				set null, or if unable to create (out of memory), return
//				kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshCorner_CreateFromExtData(
	TE3MeshCornerData* cornerPtr,
	const TQ3MeshCornerData* cornerExtDataPtr,
	TE3MeshFaceDataArray* meshFaceArrayPtr)
{
	TQ3Uns32 meshNumFaces;
	TE3MeshFaceData* meshFaces;

	TQ3Uns32 numFaces;
	const TQ3Uns32* faceIndices;
	TE3MeshFacePtr* facePtrs;
	TQ3Uns32 i;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);
	Q3_ASSERT_VALID_PTR(cornerExtDataPtr);
	Q3_ASSERT_VALID_PTR(meshFaceArrayPtr);

	// Get mesh faces
	meshNumFaces = e3meshFaceArray_Length(meshFaceArrayPtr);
	meshFaces = e3meshFaceArray_FirstItem(meshFaceArrayPtr);

	// Get and validate face indices
	numFaces = cornerExtDataPtr->numFaces;
	faceIndices = cornerExtDataPtr->faceIndices;
	if (numFaces == 0)
		goto failure_1;
	if (faceIndices == NULL)
		goto failure_2;

	// Check for out-of-range or duplicate face indices
	for (i = 0; i < numFaces; ++i)
	{
		TQ3Uns32 faceIndex = faceIndices[i];
		TQ3Uns32 i2;

		if (faceIndex >= meshNumFaces)
			goto failure_3;

		for (i2 = 0; i2 < i; ++i2)
		{
			TQ3Uns32 faceIndex2 = faceIndices[i2];

			if (faceIndex == faceIndex2)
				goto failure_4;
		}
	}

	// Check for null attribute set
	if (cornerExtDataPtr->cornerAttributeSet == NULL)
		goto failure_5;

	// Create array of uninitialized face pointers
	if (e3meshFacePtrArray_Create(&cornerPtr->facePtrArrayOrList.array, numFaces, NULL) == kQ3Failure)
		goto failure_6;

	// Initialize face pointers
	facePtrs = e3meshFacePtrArray_FirstItem(&cornerPtr->facePtrArrayOrList.array);
	for (i = 0; i < numFaces; ++i)
		facePtrs[i] = &meshFaces[faceIndices[i]];

	// Initialize attribute set
	E3Shared_Acquire(&cornerPtr->attributeSet, cornerExtDataPtr->cornerAttributeSet);

	return(kQ3Success);
	
	// Dead code to reverse e3meshFacePtrArray_Create
failure_6:

failure_5:

failure_4:

failure_3:

failure_2:

failure_1:

	return(kQ3Failure);
}



//=============================================================================
//      e3meshCorner_Destroy : TE3MeshCornerData destructor.
//-----------------------------------------------------------------------------
static
void
e3meshCorner_Destroy(
	TE3MeshCornerData* cornerPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);

	// Release attribute set
	Q3Object_CleanDispose(&cornerPtr->attributeSet);

	// Destroy face pointer array or list
	e3meshFacePtrArrayOrList_Destroy(&cornerPtr->facePtrArrayOrList, NULL);
}





//=============================================================================
//      e3meshCorner_Relocate : Relocate corner.
//-----------------------------------------------------------------------------
//		Note : See also e3meshCornerPtr_Relink.
//-----------------------------------------------------------------------------
static
void
e3meshCorner_Relocate(
	TE3MeshCornerData* newCornerPtr,
	TE3MeshCornerData* oldCornerPtr)
{
	// Save pointer to new corner in old corner's attribute set
	*((TE3MeshCornerData**) &oldCornerPtr->attributeSet) = newCornerPtr;
}





//=============================================================================
//      e3meshCorner_RelinkFaces : Relink pointers to faces.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshCorner_RelinkFaces(
	TE3MeshCornerData* cornerPtr,
	void* unusedArg)
{
    unusedArg; /* Suppress compiler warning */

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);

	return(e3meshFacePtrArrayOrList_DoForEach(
		&cornerPtr->facePtrArrayOrList,
		&e3meshFacePtr_Relink,
		NULL));
}





//=============================================================================
//      e3meshCorner_UseFacePtrArray : Use face pointer array.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshCorner_UseFacePtrArray(
	TE3MeshCornerData* cornerPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);

	return(e3meshFacePtrArrayOrList_UseArray(
		&cornerPtr->facePtrArrayOrList,
		NULL,
		NULL,
		NULL));
}





//=============================================================================
//      e3meshCorner_UseFacePtrList : Use face pointer list.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshCorner_UseFacePtrList(
	TE3MeshCornerData* cornerPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);

	return(e3meshFacePtrArrayOrList_UseList(
		&cornerPtr->facePtrArrayOrList,
		NULL,
		NULL,
		NULL));
}





//=============================================================================
//      e3meshCorner_NumFaces : Return number of faces for corner.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3meshCorner_NumFaces(
	const TE3MeshCornerData* cornerPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);

	return(e3meshFacePtrArrayOrList_Length(&cornerPtr->facePtrArrayOrList));
}





//=============================================================================
//      e3meshCorner_HasFace : Return if corner has face.
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3meshCorner_HasFace(
	const TE3MeshCornerData* cornerPtr,
	TE3MeshFaceData* facePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);
	Q3_ASSERT_VALID_PTR(facePtr);

	return(e3meshFacePtrArrayOrList_HasPtr(
		&cornerPtr->facePtrArrayOrList,
		facePtr));
}





//=============================================================================
//      e3meshCorner_AttachFace : Attach face to corner.
//-----------------------------------------------------------------------------
//		Note :	If unable to use list of face pointers of if unable to insert
//				face pointer, return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshCorner_AttachFace(
	TE3MeshCornerData* cornerPtr,
	TE3MeshFaceData* facePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);
	Q3_ASSERT_VALID_PTR(facePtr);

	// Use list of face pointers (*** MAY RELOCATE FACE POINTERS ***)
	if (e3meshCorner_UseFacePtrList(cornerPtr) == kQ3Failure)
		goto failure_1;

	// Push back face pointer
	if (e3meshFacePtrList_PushBackPtr(&cornerPtr->facePtrArrayOrList.list, facePtr) == NULL)
		goto failure_2;

	return(kQ3Success);
	
	// Dead code to reverse e3meshFacePtrList_PushBackPtr
failure_2:

failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshCorner_DetachFace : Detach face from corner.
//-----------------------------------------------------------------------------
//		Note : If unable to use list of face pointers, return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshCorner_DetachFace(
	TE3MeshCornerData* cornerPtr,
	TE3MeshFaceData* facePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);
	Q3_ASSERT_VALID_PTR(facePtr);

	// Use list of face pointers (*** MAY RELOCATE FACE POINTERS ***)
	if (e3meshCorner_UseFacePtrList(cornerPtr) == kQ3Failure)
		goto failure_1;

	// Erase face pointer
	if (e3meshFacePtrList_ErasePtr(&cornerPtr->facePtrArrayOrList.list, facePtr) == kQ3Failure)
		goto failure_2;

	return(kQ3Success);
	
	// Dead code to reverse e3meshFacePtrList_ErasePtr
failure_2:

failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshCorner_SpliceFace : Splice face into new corner from old corner.
//-----------------------------------------------------------------------------
//		Note : If unable to use list of face pointers, return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshCorner_SpliceFace(
	TE3MeshCornerData* newCornerPtr,
	TE3MeshCornerData* oldCornerPtr,
	TE3MeshFaceData* facePtr)
{
	TE3MeshFaceData** faceHdl;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(newCornerPtr);
	Q3_ASSERT_VALID_PTR(oldCornerPtr);
	Q3_ASSERT_VALID_PTR(facePtr);

	// Use list of face pointers (*** MAY RELOCATE FACE POINTERS ***)
	if (e3meshCorner_UseFacePtrList(oldCornerPtr) == kQ3Failure)
		goto failure_1;

	// Find face pointer for old corner
	if ((faceHdl = e3meshFacePtrList_FindPtr(&oldCornerPtr->facePtrArrayOrList.list, facePtr)) == NULL)
		goto failure_2;

	// Use list of face pointers (*** MAY RELOCATE FACE POINTERS ***)
	if (e3meshCorner_UseFacePtrList(newCornerPtr) == kQ3Failure)
		goto failure_3;

	// Splice face pointer
	e3meshFacePtrList_SpliceBackListItem(
		&newCornerPtr->facePtrArrayOrList.list,
		&oldCornerPtr->facePtrArrayOrList.list,
		faceHdl);

	return(kQ3Success);
	
failure_3:

failure_2:

failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshCorner_HasAttributeSet : Return if corner has attribute set.
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3meshCorner_HasAttributeSet(
	const TE3MeshCornerData* cornerPtr,
	TQ3AttributeSet attributeSet)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);
	Q3_ASSERT_VALID_PTR(attributeSet);

	return(cornerPtr->attributeSet == attributeSet ? kQ3True : kQ3False);
}





//=============================================================================
//      e3meshCorner_GetExtData : Get external data.
//-----------------------------------------------------------------------------
//		Note : If unable to get (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshCorner_GetExtData(
	const TE3MeshCornerData* cornerPtr,
	TQ3MeshCornerData* cornerExtDataPtr,
	const TE3MeshFaceDataArray* meshFaceArrayPtr)
{
	const TE3MeshFaceData* meshFaces;

	TQ3Uns32 numFaces;
	TQ3Uns32* faceIndices;
	const TE3MeshFacePtr* facePtrs;
	TQ3Uns32 i;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerPtr);
	Q3_ASSERT_VALID_PTR(cornerExtDataPtr);
	Q3_ASSERT_VALID_PTR(meshFaceArrayPtr);

	// Get mesh faces
	meshFaces = e3meshFaceArray_FirstItemConst(meshFaceArrayPtr);

	// Use array of face pointers (*** MAY RELOCATE FACE POINTERS ***)
	if (e3meshCorner_UseFacePtrArray(E3_CONST_CAST(TE3MeshCornerData*, cornerPtr)) == kQ3Failure)
		goto failure_1;

	// Allocate array of uninitialized face indices
	numFaces = e3meshCorner_NumFaces(cornerPtr);
	if ((faceIndices = Q3Memory_Allocate(numFaces * sizeof(TQ3Uns32))) == NULL)
		goto failure_2;
	cornerExtDataPtr->numFaces = numFaces;
	cornerExtDataPtr->faceIndices = faceIndices;

	// Get face indices
	facePtrs = e3meshFacePtrArray_FirstItemConst(&cornerPtr->facePtrArrayOrList.array);
	for (i = 0; i < numFaces; ++i)
		faceIndices[i] = facePtrs[i] - meshFaces;

	// Get attribute set
	E3Shared_Acquire(&cornerExtDataPtr->cornerAttributeSet, cornerPtr->attributeSet);

	return(kQ3Success);

	// Dead code to reverse Q3Memory_Allocate
failure_2:

failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshCornerExtData_Empty : Empty external data.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshCornerExtData_Empty(
	TQ3MeshCornerData* cornerExtDataPtr)
{
	TQ3Status result = kQ3Success;

	TQ3Uns32 numFaces;
	TQ3Uns32* faceIndices;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(cornerExtDataPtr);

	// Release attribute set
	Q3Object_CleanDispose(&cornerExtDataPtr->cornerAttributeSet);

	// Validate face indices
	numFaces = cornerExtDataPtr->numFaces;
	faceIndices = cornerExtDataPtr->faceIndices;
	if (numFaces == 0)
		result = kQ3Failure;
	if (faceIndices == NULL)
		result = kQ3Failure;

	// Deallocate array of face indices
	Q3Memory_Free(&cornerExtDataPtr->faceIndices);
	cornerExtDataPtr->numFaces = 0;

	return(result);
}





//=============================================================================
//      e3meshVertex_CreateEmptyArrayOfCorners : TE3MeshVertexData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshVertex_CreateEmptyArrayOfCorners(
	TE3MeshVertexData* vertexPtr,
	TE3MeshData* meshPtr,
	TQ3Boolean isReferenced,
	const TQ3Vertex3D* vertexExtDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);
	Q3_ASSERT_VALID_PTR(meshPtr);
	Q3_ASSERT_VALID_PTR(vertexExtDataPtr);

	// Create part
	if (e3meshPart_Create(&vertexPtr->part, meshPtr, isReferenced) == kQ3Failure)
		goto failure_1;

	// Initialize point
	vertexPtr->point = vertexExtDataPtr->point;

	// Create empty array of corners
	if (e3meshCornerArray_Create(&vertexPtr->cornerArrayOrList.array, 0, NULL) == kQ3Failure)
		goto failure_2;

	// Initialize attribute set
	E3Shared_Acquire(&vertexPtr->attributeSet, vertexExtDataPtr->attributeSet);

	return(kQ3Success);
	
	// Dead code to reverse e3meshCornerArray_Create
failure_2:

	e3meshPart_ReleaseHandleInMesh(&vertexPtr->part, meshPtr);
	e3meshPart_Destroy(&vertexPtr->part);
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshVertex_CreateFromExtData :	TE3MeshVertexData constructor from
//											external data.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshVertex_CreateFromExtData(
	TE3MeshVertexData* vertexPtr,
	const TQ3MeshVertexData* vertexExtDataPtr,
	TE3MeshFaceDataArray* meshFaceArrayPtr)
{
	TQ3Uns32 numCorners;
	const TQ3MeshCornerData* cornerExtDatas;
	TE3MeshCornerData* corners;
	TQ3Uns32 iSave;
	TQ3Uns32 i1, j1, i2, j2;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);
	Q3_ASSERT_VALID_PTR(vertexExtDataPtr);
	Q3_ASSERT_VALID_PTR(meshFaceArrayPtr);

	// Create part, without reference
	if (e3meshPart_CreateUnreferenced(&vertexPtr->part) == kQ3Failure)
		goto failure_1;

	// Initialize point
	vertexPtr->point = vertexExtDataPtr->point;

	// Get and validate corners
	numCorners = vertexExtDataPtr->numCorners;
	cornerExtDatas = vertexExtDataPtr->corners;
	if (numCorners > 0 && cornerExtDatas == NULL)
		goto failure_2;

	// Create array of uninitialized corners
	if (e3meshCornerArray_Create(&vertexPtr->cornerArrayOrList.array, numCorners, NULL) == kQ3Failure)
		goto failure_3;

	// Initialize corners
	corners = e3meshCornerArray_FirstItem(&vertexPtr->cornerArrayOrList.array);
	for (iSave = 0; iSave < numCorners; ++iSave)
		if (e3meshCorner_CreateFromExtData(&corners[iSave], &cornerExtDatas[iSave], meshFaceArrayPtr) == kQ3Failure)
			goto failure_4;

	// Check for duplicate face indices (in different corners for this vertex)
	for (i2 = 0; i2 < numCorners; ++i2)
	{
		TQ3Uns32 cornerNumFaces2 = cornerExtDatas[i2].numFaces;
		TQ3Uns32* cornerFacesIndices2 = cornerExtDatas[i2].faceIndices;

		for (j2 = 0; j2 < cornerNumFaces2; ++j2)
		{
			TQ3Uns32 faceIndex2 = cornerFacesIndices2[j2];

			for (i1 = 0; i1 < i2; ++i1)
			{
				TQ3Uns32 cornerNumFaces1 = cornerExtDatas[i1].numFaces;
				TQ3Uns32* cornerFacesIndices1 = cornerExtDatas[i1].faceIndices;

				for (j1 = 0; j1 < cornerNumFaces1; ++j1)
				{
					TQ3Uns32 faceIndex1 = cornerFacesIndices1[j1];

					if (faceIndex1 == faceIndex2)
						goto failure_5;
				}
			}
		}
	}

	// Initialize attribute set
	E3Shared_Acquire(&vertexPtr->attributeSet, vertexExtDataPtr->attributeSet);

	return(kQ3Success);
	
failure_5:

failure_4:
	while (iSave > 0)
		e3meshCorner_Destroy(&corners[--iSave]);

	e3meshCornerArray_Destroy(&vertexPtr->cornerArrayOrList.array, NULL);
failure_3:

failure_2:

	e3meshPart_Destroy(&vertexPtr->part);
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshVertex_Destroy : TE3MeshVertexData destructor.
//-----------------------------------------------------------------------------
static
void
e3meshVertex_Destroy(
	TE3MeshVertexData* vertexPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);

	// Release attribute set
	Q3Object_CleanDispose(&vertexPtr->attributeSet);

	// Destroy corner array or list
	e3meshCornerArrayOrList_Destroy(&vertexPtr->cornerArrayOrList, e3meshCorner_Destroy);

	// Destroy part
	e3meshPart_Destroy(&vertexPtr->part);
}





//=============================================================================
//      e3meshVertex_Relocate : Relocate vertex.
//-----------------------------------------------------------------------------
static
void
e3meshVertex_Relocate(
	TE3MeshVertexData* newVertexPtr,
	TE3MeshVertexData* oldVertexPtr)
{
	// Relocate part
	e3meshPart_Relocate(
		&newVertexPtr->part,
		&oldVertexPtr->part);
}





//=============================================================================
//      e3meshVertex_RelinkCornerFaces : Relink pointers to corner faces.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshVertex_RelinkCornerFaces(
	TE3MeshVertexData* vertexPtr,
	void* unusedArg)
{
    unusedArg; /* Suppress compiler warning */

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);

	return(e3meshCornerArrayOrList_DoForEach(
		&vertexPtr->cornerArrayOrList,
		&e3meshCorner_RelinkFaces,
		NULL));
}





//=============================================================================
//      e3meshVertex_UseCornerArray :	Use corner array, optionally relinking
//										corner pointer.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshVertex_UseCornerArray(
	TE3MeshVertexData* vertexPtr,
	TE3MeshCornerData** cornerHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);

	if (cornerHdl == NULL)
		return(e3meshCornerArrayOrList_UseArray(
			&vertexPtr->cornerArrayOrList,
			NULL,
			NULL,
			NULL));
	else
		return(e3meshCornerArrayOrList_UseArray(
			&vertexPtr->cornerArrayOrList,
			NULL,
			E3_UP_CAST(void (*)(void*), &e3meshCornerPtr_Relink),
			cornerHdl));
}





//=============================================================================
//      e3meshVertex_UseCornerList :	Use corner list, optionally relinking
//										corner pointer.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshVertex_UseCornerList(
	TE3MeshVertexData* vertexPtr,
	TE3MeshCornerData** cornerHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);

	if (cornerHdl == NULL)
		return(e3meshCornerArrayOrList_UseList(
			&vertexPtr->cornerArrayOrList,
			NULL,
			NULL,
			NULL));
	else
		return(e3meshCornerArrayOrList_UseList(
			&vertexPtr->cornerArrayOrList,
			NULL,
			E3_UP_CAST(void (*)(void*), &e3meshCornerPtr_Relink),
			cornerHdl));
}





//=============================================================================
//      e3meshVertex_NumCorners : Return number of corners in vertex.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3meshVertex_NumCorners(
	const TE3MeshVertexData* vertexPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);

	return(e3meshCornerArrayOrList_Length(&vertexPtr->cornerArrayOrList));
}





//=============================================================================
//      e3meshVertex_NewCorner : Return new empty corner for vertex.
//-----------------------------------------------------------------------------
//		Note :	If unable to use list of corners, if unable to insert corner
//				item, or if unable to create corner, return NULL.
//-----------------------------------------------------------------------------
static
TE3MeshCornerData*
e3meshVertex_NewCorner(
	TE3MeshVertexData* vertexPtr,
	TE3MeshData* meshPtr,
	TQ3AttributeSet attributeSet)
{
	TE3MeshCornerData* cornerPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);
	Q3_ASSERT_VALID_PTR(meshPtr);

	// Must have a non-null attribute set
	Q3_ASSERT_VALID_PTR(attributeSet);

	// Use list of corners (*** MAY RELOCATE CORNERS ***)
	if (e3meshVertex_UseCornerList(vertexPtr, NULL) == kQ3Failure)
		goto failure_1;

	// Push back new uninitialized corner
	if ((cornerPtr = e3meshCornerList_PushBackItem(&vertexPtr->cornerArrayOrList.list, NULL)) == NULL)
		goto failure_2;

	// Create corner with empty list of face pointers
	if (e3meshCorner_CreateEmptyListOfFacePtrs(cornerPtr, attributeSet) == kQ3Failure)
		goto failure_3;

	// Increment number of (defined) corners in mesh
	++meshPtr->numCorners;

	return(cornerPtr);
	
	// Dead code to reverse e3meshCorner_CreateEmptyListOfFacePtrs
failure_3:
	
	e3meshCornerList_EraseItem(&vertexPtr->cornerArrayOrList.list, NULL, cornerPtr);
failure_2:

failure_1:

	return(NULL);
}





//=============================================================================
//      e3meshVertex_DeleteCorner : Delete corner from vertex.
//-----------------------------------------------------------------------------
//		Note : If unable to use list of corners, return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshVertex_DeleteCorner(
	TE3MeshVertexData* vertexPtr,
	TE3MeshData* meshPtr,
	TE3MeshCornerData* cornerPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);
	Q3_ASSERT_VALID_PTR(meshPtr);
	Q3_ASSERT_VALID_PTR(cornerPtr);

	// Use list of corners (*** MAY RELOCATE CORNERS ***)
	if (e3meshVertex_UseCornerList(vertexPtr, &cornerPtr) == kQ3Failure)
		goto failure;

	// Erase corner
	e3meshCornerList_EraseItem(&vertexPtr->cornerArrayOrList.list, NULL, cornerPtr);

	// Decrement number of (defined) corners in mesh
	--meshPtr->numCorners;

	return(kQ3Success);

failure:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshVertex_FaceCorner : Return corner for vertex-face combination.
//-----------------------------------------------------------------------------
//		Note : If corner undefined, return NULL.
//-----------------------------------------------------------------------------
static
TE3MeshCornerData*
e3meshVertex_FaceCorner(
	const TE3MeshVertexData* vertexPtr,
	const TE3MeshFaceData* facePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);
	Q3_ASSERT_VALID_PTR(facePtr);

	return(e3meshCornerArrayOrList_Find(
		&vertexPtr->cornerArrayOrList,
		E3_DOWN_CAST(TQ3Boolean (*)(const TE3MeshCornerData*, void*), &e3meshCorner_HasFace),
		(void*)facePtr));
}





//=============================================================================
//      e3meshVertex_AttributeSetCorner :	Return corner for vertex-attribute
//											set combination.
//-----------------------------------------------------------------------------
//		Note : If corner undefined, return NULL.
//-----------------------------------------------------------------------------
static
TE3MeshCornerData*
e3meshVertex_AttributeSetCorner(
	TE3MeshVertexData* vertexPtr,
	TE3MeshData* meshPtr,
	TQ3AttributeSet attributeSet)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);
	Q3_ASSERT_VALID_PTR(meshPtr);
	Q3_ASSERT_VALID_PTR(attributeSet);

	return(e3meshCornerArrayOrList_Find(
		&vertexPtr->cornerArrayOrList,
		E3_DOWN_CAST(TQ3Boolean (*)(const TE3MeshCornerData*, void*), &e3meshCorner_HasAttributeSet),
		attributeSet));
}





//=============================================================================
//      e3meshVertex_ExtRefInMesh :	Return external reference to vertex.
//-----------------------------------------------------------------------------
//		Note : If unable to get external reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
static
TE3MeshVertexExtRef
e3meshVertex_ExtRefInMesh(
	TE3MeshVertexData* vertexPtr,
	TE3MeshData* meshPtr)
{
	return(E3_DOWN_CAST(TE3MeshVertexExtRef,
		e3meshPart_HandleInMesh(
			&vertexPtr->part,
			meshPtr)));
}





//=============================================================================
//      e3meshVertex_GetExtData : Get external data.
//-----------------------------------------------------------------------------
//		Note : If unable to get (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshVertex_GetExtData(
	const TE3MeshVertexData* vertexPtr,
	TQ3MeshVertexData* vertexExtDataPtr,
	const TE3MeshFaceDataArray* meshFaceArrayPtr)
{
	TQ3Uns32 numCorners;
	TQ3MeshCornerData* cornerExtDatas;
	const TE3MeshCornerData* corners;
	TQ3Uns32 iSave;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);
	Q3_ASSERT_VALID_PTR(vertexExtDataPtr);
	Q3_ASSERT_VALID_PTR(meshFaceArrayPtr);

	// Get point
	vertexExtDataPtr->point = vertexPtr->point;

	// Use array of corners (*** MAY RELOCATE CORNERS ***)
	if (e3meshVertex_UseCornerArray(E3_CONST_CAST(TE3MeshVertexData*, vertexPtr), NULL) == kQ3Failure)
		goto failure_1;

	// Allocate array of uninitialized corners
	numCorners = e3meshVertex_NumCorners(vertexPtr);
	if (numCorners == 0)
		cornerExtDatas = NULL;
	else if ((cornerExtDatas = Q3Memory_Allocate(numCorners * sizeof(TQ3MeshCornerData))) == NULL)
		goto failure_2;
	vertexExtDataPtr->numCorners = numCorners;
	vertexExtDataPtr->corners = cornerExtDatas;

	// Get corners
	corners = e3meshCornerArray_FirstItemConst(&vertexPtr->cornerArrayOrList.array);
	for (iSave = 0; iSave < numCorners; ++iSave)
		if (e3meshCorner_GetExtData(&corners[iSave], &cornerExtDatas[iSave], meshFaceArrayPtr) == kQ3Failure)
			goto failure_3;

	// Get attribute set
	E3Shared_Acquire(&vertexExtDataPtr->attributeSet, vertexPtr->attributeSet);

	return(kQ3Success);

failure_3:
	while (iSave > 0)
		e3meshCornerExtData_Empty(&cornerExtDatas[--iSave]);

	Q3Memory_Free(&cornerExtDatas);
failure_2:

failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshVertexExtRef_Vertex : Return vertex for external reference.
//-----------------------------------------------------------------------------
//		Note : If vertex deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshVertexData*
e3meshVertexExtRef_Vertex(
	TE3MeshVertexExtRef vertexExtRef)
{
	return(E3_DOWN_CAST(TE3MeshVertexData*,
		e3meshPartHdl_Part(
			E3_UP_CAST(TE3MeshPartData**, vertexExtRef))));
}





//=============================================================================
//      e3meshVertexExtRef_Mesh : Return mesh for external reference.
//-----------------------------------------------------------------------------
static
TE3MeshData*
e3meshVertexExtRef_Mesh(
	TE3MeshVertexExtRef vertexExtRef)
{
	return(e3meshPartHdl_Mesh(E3_UP_CAST(TE3MeshPartData**, vertexExtRef)));
}





//=============================================================================
//      e3meshVertexExtData_Empty : Empty external data.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshVertexExtData_Empty(
	TQ3MeshVertexData* vertexExtDataPtr)
{
	TQ3Status result = kQ3Success;

	TQ3Uns32 numCorners;
	TQ3MeshCornerData* cornerExtDatas;
	TQ3Uns32 i;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexExtDataPtr);

	// Release attribute set
	Q3Object_CleanDispose(&vertexExtDataPtr->attributeSet);

	// Validate and empty corners
	numCorners = vertexExtDataPtr->numCorners;
	cornerExtDatas = vertexExtDataPtr->corners;
	if (numCorners > 0 && cornerExtDatas == NULL)
		result = kQ3Failure;
	else
	{
		// Empty corners
		for (i = 0; i < numCorners; ++i)
			if (e3meshCornerExtData_Empty(&cornerExtDatas[i]) == kQ3Failure)
				result = kQ3Failure;
	}

	// Deallocate array of corners
	Q3Memory_Free(&vertexExtDataPtr->corners);
	vertexExtDataPtr->numCorners = 0;
	
	return(result);
}





//=============================================================================
//      e3meshContour_CreateFromVertexExtRefs : TE3MeshContourData constructor
//												from an array of vertex
//												external references.
//-----------------------------------------------------------------------------
//		Note :	If any vertex is deleted or if unable to create (out of memory),
//				return kQ3Failure. If a particular vertex occurs more than once
//				in succession, repeated occurrences are eliminated. If after
//				this elimination less than 2 vertices remain, return kQ3Failure.
//				Thus we ensure that every edge connects two distinct vertices.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshContour_CreateFromVertexExtRefs(
	TE3MeshContourData* contourPtr,
	TE3MeshData* meshPtr,
	TQ3Boolean isReferenced,
	TE3MeshFaceData* containerFacePtr,
	TQ3Uns32 numVertices,
	const TE3MeshVertexExtRef* vertexExtRefs)
{
	TQ3Uns32 effectiveNumVertices;
	TE3MeshVertexPtr* vertexPtrs;
	TQ3Uns32 i, i2;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourPtr);
	Q3_ASSERT_VALID_PTR(meshPtr);
	Q3_ASSERT_VALID_PTR(containerFacePtr);
	Q3_ASSERT_VALID_PTR(vertexExtRefs);

	// Check for deleted vertices; count effective number of vertices (excluding repeats)
	effectiveNumVertices = 0;
	for (i = 0; i < numVertices; ++i)
	{
		if (e3meshVertexExtRef_Vertex(vertexExtRefs[i]) == NULL)
			goto failure_1;
		if (vertexExtRefs[i] != vertexExtRefs[i > 0 ? i-1 : numVertices-1])
			++effectiveNumVertices;
	}
	if (effectiveNumVertices < 2)
		goto failure_2;

	// Create part
	if (e3meshPart_Create(&contourPtr->part, meshPtr, isReferenced) == kQ3Failure)
		goto failure_3;

	// Initialize pointer to container face
	contourPtr->containerFacePtr = containerFacePtr;

	// Create array of uninitialized vertex pointers
	if (e3meshVertexPtrArray_Create(&contourPtr->vertexPtrArray, effectiveNumVertices, NULL) == kQ3Failure)
		goto failure_4;

	// Initialize vertex pointers
	vertexPtrs = e3meshVertexPtrArray_FirstItem(&contourPtr->vertexPtrArray);
	for (i = 0, i2 = 0; i < numVertices; ++i)
		if (vertexExtRefs[i] != vertexExtRefs[i > 0 ? i-1 : numVertices-1])
			vertexPtrs[i2++] = e3meshVertexExtRef_Vertex(vertexExtRefs[i]);

	return(kQ3Success);

	// Dead code to reverse e3meshVertexPtrArray_Create
failure_4:

	e3meshPart_ReleaseHandleInMesh(&contourPtr->part, meshPtr);
	e3meshPart_Destroy(&contourPtr->part);
failure_3:

failure_2:

failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshContour_CreateFromExtData :	TE3MeshContourData constructor from
//											external data.
//-----------------------------------------------------------------------------
//		Warning : Upon entry, the mesh vertices must be organized in an array.
//-----------------------------------------------------------------------------
//		Note :	If any vertex index is out of range or if unable to create
//				(out of memory), return kQ3Failure. If a particular vertex
//				occurs more than once in succession, repeated occurrences are
//				eliminated. If after this elimination less than 2 vertices
//				remain, return kQ3Failure. Thus we ensure that every edge
//				connects two distinct vertices.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshContour_CreateFromExtData(
	TE3MeshContourData* contourPtr,
	TE3MeshFaceData* containerFacePtr,
	const TQ3MeshContourData* contourExtDataPtr,
	TE3MeshVertexDataArray* meshVertexArrayPtr)
{
	TQ3Uns32 meshNumVertices;
	TE3MeshVertexData* meshVertices;

	TQ3Uns32 numVertices, effectiveNumVertices;
	const TQ3Uns32* vertexIndices;
	TE3MeshVertexPtr* vertexPtrs;
	TQ3Uns32 i, i2;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourPtr);
	Q3_ASSERT_VALID_PTR(containerFacePtr);
	Q3_ASSERT_VALID_PTR(contourExtDataPtr);
	Q3_ASSERT_VALID_PTR(meshVertexArrayPtr);

	// Get mesh vertices
	meshNumVertices = e3meshVertexArray_Length(meshVertexArrayPtr);
	meshVertices = e3meshVertexArray_FirstItem(meshVertexArrayPtr);

	// Get and validate vertex indices
	numVertices = contourExtDataPtr->numVertices;
	vertexIndices = contourExtDataPtr->vertexIndices;
	if (numVertices > 0 && vertexIndices == NULL)
		goto failure_2;

	// Check for out-of-range vertex indices; count effective number of vertices (excluding repeats)
	effectiveNumVertices = 0;
	for (i = 0; i < numVertices; ++i)
	{
		if (vertexIndices[i] >= meshNumVertices)
			goto failure_1;
		if (vertexIndices[i] != vertexIndices[i > 0 ? i-1 : numVertices-1])
			++effectiveNumVertices;
	}
	if (effectiveNumVertices < 2)
		goto failure_2;

	// Create part, without reference
	if (e3meshPart_CreateUnreferenced(&contourPtr->part) == kQ3Failure)
		goto failure_3;

	// Initialize pointer to container face
	contourPtr->containerFacePtr = containerFacePtr;

	// Create array of uninitialized vertex pointers
	if (e3meshVertexPtrArray_Create(&contourPtr->vertexPtrArray, effectiveNumVertices, NULL) == kQ3Failure)
		goto failure_4;

	// Initialize vertex pointers
	vertexPtrs = e3meshVertexPtrArray_FirstItem(&contourPtr->vertexPtrArray);
	for (i = 0, i2 = 0; i < numVertices; ++i)
		if (vertexIndices[i] != vertexIndices[i > 0 ? i-1 : numVertices-1])
			vertexPtrs[i2++] = &meshVertices[vertexIndices[i]];

	return(kQ3Success);

	// Dead code to reverse e3meshVertexPtrArray_Create
failure_4:

	e3meshPart_Destroy(&contourPtr->part);
failure_3:

failure_2:

failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshContour_Destroy : TE3MeshContourData destructor.
//-----------------------------------------------------------------------------
static
void
e3meshContour_Destroy(
	TE3MeshContourData* contourPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourPtr);

	// Destroy vertex pointer array
	e3meshVertexPtrArray_Destroy(&contourPtr->vertexPtrArray, NULL);

	// Destroy part
	e3meshPart_Destroy(&contourPtr->part);
}





//=============================================================================
//      e3meshContour_Relocate : Relocate contour.
//-----------------------------------------------------------------------------
static
void
e3meshContour_Relocate(
	TE3MeshContourData* newContourPtr,
	TE3MeshContourData* oldContourPtr)
{
	// Relocate part
	e3meshPart_Relocate(
		&newContourPtr->part,
		&oldContourPtr->part);
}





//=============================================================================
//      e3meshContour_Tessellate : Tessellate contour.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshContour_Tessellate(
	TE3MeshContourData* contourPtr,
	GLUtriangulatorObj* gluTessellatorPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourPtr);
	Q3_ASSERT_VALID_PTR(gluTessellatorPtr);

	gluTessBeginContour(gluTessellatorPtr);
	e3meshVertexPtrArray_DoForEach(
		&contourPtr->vertexPtrArray,
		E3_DOWN_CAST(TQ3Status (*)(TE3MeshVertexPtr*, void*), &e3meshVertexPtr_Tessellate),
		gluTessellatorPtr);
	gluTessEndContour(gluTessellatorPtr);

	return(kQ3Success);
}





//=============================================================================
//      e3meshContour_RelinkContainerFace : Relink pointer to container face.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshContour_RelinkContainerFace(
	TE3MeshContourData* contourPtr,
	void* unusedArg)
{
    unusedArg; /* Suppress compiler warning */

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourPtr);

	return(e3meshFacePtr_Relink(&contourPtr->containerFacePtr, NULL));
}





//=============================================================================
//      e3meshContour_SetContainerFace : Set container face.
//-----------------------------------------------------------------------------
static
void
e3meshContour_SetContainerFace(
	TE3MeshContourData* contourPtr,
	TE3MeshFaceData* containerFacePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourPtr);
	Q3_ASSERT_VALID_PTR(containerFacePtr);

	contourPtr->containerFacePtr = containerFacePtr;
}





//=============================================================================
//      e3meshContour_ContainerFace : Return container face.
//-----------------------------------------------------------------------------
static
TE3MeshFaceData*
e3meshContour_ContainerFace(
	TE3MeshContourData* contourPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourPtr);

	return(contourPtr->containerFacePtr);
}





//=============================================================================
//      e3meshContour_RelinkVertices : Relink pointers to vertices.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshContour_RelinkVertices(
	TE3MeshContourData* contourPtr,
	void* unusedArg)
{
    unusedArg; /* Suppress compiler warning */

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourPtr);

	return(e3meshVertexPtrArray_DoForEach(
		&contourPtr->vertexPtrArray,
		&e3meshVertexPtr_Relink,
		NULL));
}





//=============================================================================
//      e3meshContour_NumVertices : Return number of vertices in contour.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is counted separately.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3meshContour_NumVertices(
	const TE3MeshContourData* contourPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourPtr);

	return(e3meshVertexPtrArray_Length(&contourPtr->vertexPtrArray));
}





//=============================================================================
//      e3meshContour_HasVertex : Return if contour has vertex.
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3meshContour_HasVertex(
	const TE3MeshContourData* contourPtr,
	TE3MeshVertexData* vertexPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourPtr);
	Q3_ASSERT_VALID_PTR(vertexPtr);

	return(e3meshVertexPtrArray_HasPtr(
		&contourPtr->vertexPtrArray,
		vertexPtr));
}





//=============================================================================
//      e3meshContour_ExtRefInMesh : Return external reference to contour.
//-----------------------------------------------------------------------------
//		Note : If unable to get external reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
static
TE3MeshContourExtRef
e3meshContour_ExtRefInMesh(
	TE3MeshContourData* contourPtr,
	TE3MeshData* meshPtr)
{
	return(E3_DOWN_CAST(TE3MeshContourExtRef,
		e3meshPart_HandleInMesh(
			&contourPtr->part,
			meshPtr)));
}





//=============================================================================
//      e3meshContour_GetExtData : Get external data.
//-----------------------------------------------------------------------------
//		Note : If unable to get (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshContour_GetExtData(
	const TE3MeshContourData* contourPtr,
	TQ3MeshContourData* contourExtDataPtr,
	const TE3MeshVertexDataArray* meshVertexArrayPtr)
{
	const TE3MeshVertexData* meshVertices;

	TQ3Uns32 numVertices;
	TQ3Uns32* vertexIndices;
	const TE3MeshVertexPtr* vertexPtrs;
	TQ3Uns32 i;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourPtr);
	Q3_ASSERT_VALID_PTR(contourExtDataPtr);
	Q3_ASSERT_VALID_PTR(meshVertexArrayPtr);

	// Get mesh vertices
	meshVertices = e3meshVertexArray_FirstItemConst(meshVertexArrayPtr);

	// Allocate array of uninitialized vertex indices
	numVertices = e3meshContour_NumVertices(contourPtr);
	if (numVertices == 0)
		vertexIndices = NULL;
	else if ((vertexIndices = Q3Memory_Allocate(numVertices * sizeof(TQ3Uns32))) == NULL)
		goto failure_1;
	contourExtDataPtr->numVertices = numVertices;
	contourExtDataPtr->vertexIndices = vertexIndices;

	// Get vertex indices
	vertexPtrs = e3meshVertexPtrArray_FirstItemConst(&contourPtr->vertexPtrArray);
	for (i = 0; i < numVertices; ++i)
		vertexIndices[i] = vertexPtrs[i] - meshVertices;

	return(kQ3Success);

	// Dead code to reverse Q3Memory_Allocate

failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshContourExtRef_Contour : Return contour for external reference.
//-----------------------------------------------------------------------------
//		Note : If contour deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshContourData*
e3meshContourExtRef_Contour(
	TE3MeshContourExtRef contourExtRef)
{
	return(E3_DOWN_CAST(TE3MeshContourData*,
		e3meshPartHdl_Part(
			E3_UP_CAST(TE3MeshPartData**, contourExtRef))));
}





//=============================================================================
//      e3meshContourExtRef_Mesh : Return mesh for external reference.
//-----------------------------------------------------------------------------
static
TE3MeshData*
e3meshContourExtRef_Mesh(
	TE3MeshContourExtRef contourExtRef)
{
	return(e3meshPartHdl_Mesh(E3_UP_CAST(TE3MeshPartData**, contourExtRef)));
}





//=============================================================================
//      e3meshContourExtData_Empty : Empty external data.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshContourExtData_Empty(
	TQ3MeshContourData* contourExtDataPtr)
{
	TQ3Status result = kQ3Success;

	TQ3Uns32 numVertices;
	TQ3Uns32* vertexIndices;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(contourExtDataPtr);

	// Validate face indices
	numVertices = contourExtDataPtr->numVertices;
	vertexIndices = contourExtDataPtr->vertexIndices;
	if (numVertices == 0)
		result = kQ3Failure;
	if (vertexIndices == NULL)
		result = kQ3Failure;

	// Deallocate array of vertex indices
	Q3Memory_Free(&contourExtDataPtr->vertexIndices);
	contourExtDataPtr->numVertices = 0;

	return(result);
}





//=============================================================================
//      e3meshFace_CreateEmptyListOfContours : TE3MeshFaceData constructor.
//-----------------------------------------------------------------------------
//		Note :	If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshFace_CreateEmptyListOfContours(
	TE3MeshFaceData* facePtr,
	TE3MeshData* meshPtr,
	TQ3Boolean isReferenced,
	TQ3AttributeSet attributeSet)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);
	Q3_ASSERT_VALID_PTR(meshPtr);

	// Create part
	if (e3meshPart_Create(&facePtr->part, meshPtr, isReferenced) == kQ3Failure)
		goto failure_1;

	// Create empty list of contours
	if (e3meshContourList_Create(&facePtr->contourArrayOrList.list, 0, NULL) == kQ3Failure)
		goto failure_2;

	// Initialize attribute set
	E3Shared_Acquire(&facePtr->attributeSet, attributeSet);

	return(kQ3Success);

	// Dead code to reverse e3meshContourList_Create
failure_2:

	e3meshPart_ReleaseHandleInMesh(&facePtr->part, meshPtr);
	e3meshPart_Destroy(&facePtr->part);
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshFace_CreateFromVertexExtRefs :	TE3MeshFaceData constructor from
//												an array of vertex external
//												references.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshFace_CreateFromVertexExtRefs(
	TE3MeshFaceData* facePtr,
	TE3MeshData* meshPtr,
	TQ3Boolean isReferenced,
	TQ3Uns32 numVertices,
	const TE3MeshVertexExtRef* vertexExtRefs,
	TQ3AttributeSet attributeSet)
{
	TE3MeshContourData* contourPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);
	Q3_ASSERT_VALID_PTR(meshPtr);
	Q3_ASSERT_VALID_PTR(vertexExtRefs);

	// Create part
	if (e3meshPart_Create(&facePtr->part, meshPtr, isReferenced) == kQ3Failure)
		goto failure_1;

	// Create array of one uninitialized contour
	if (e3meshContourArray_Create(&facePtr->contourArrayOrList.array, 1, NULL) == kQ3Failure)
		goto failure_2;

	// Initialize contour
	contourPtr = e3meshContourArray_FirstItem(&facePtr->contourArrayOrList.array);
	if (e3meshContour_CreateFromVertexExtRefs(contourPtr, meshPtr, kQ3False, facePtr, numVertices, vertexExtRefs) == kQ3Failure)
		goto failure_3;

	// Initialize attribute set
	E3Shared_Acquire(&facePtr->attributeSet, attributeSet);

	return(kQ3Success);

	// Dead code to reverse e3meshContour_CreateFromVertexExtRefs
failure_3:

	e3meshContourArray_Destroy(&facePtr->contourArrayOrList.array, NULL);
failure_2:

	e3meshPart_ReleaseHandleInMesh(&facePtr->part, meshPtr);
	e3meshPart_Destroy(&facePtr->part);
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshFace_CreateFromExtData :	TE3MeshFaceData constructor from
//										external data.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshFace_CreateFromExtData(
	TE3MeshFaceData* facePtr,
	const TQ3MeshFaceData* faceExtDataPtr,
	TE3MeshVertexDataArray* meshVertexArrayPtr)
{
	TQ3Uns32 numContours;
	TQ3MeshContourData* contourExtDatas;
	TE3MeshContourData* contours;
	TQ3Uns32 iSave;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);
	Q3_ASSERT_VALID_PTR(faceExtDataPtr);
	Q3_ASSERT_VALID_PTR(meshVertexArrayPtr);

	// Create part, without reference
	if (e3meshPart_CreateUnreferenced(&facePtr->part) == kQ3Failure)
		goto failure_1;

	// Get and validate contours
	numContours = faceExtDataPtr->numContours;
	contourExtDatas = faceExtDataPtr->contours;
	if (numContours == 0)
		goto failure_2;
	if (contourExtDatas == NULL)
		goto failure_3;

	// Create array of uninitialized contours
	if (e3meshContourArray_Create(&facePtr->contourArrayOrList.array, numContours, NULL) == kQ3Failure)
		goto failure_4;

	// Initialize contours
	contours = e3meshContourArray_FirstItem(&facePtr->contourArrayOrList.array);
	for (iSave = 0; iSave < numContours; ++iSave)
		if (e3meshContour_CreateFromExtData(&contours[iSave], facePtr, &contourExtDatas[iSave], meshVertexArrayPtr) == kQ3Failure)

	// Initialize attribute set
	E3Shared_Acquire(&facePtr->attributeSet, faceExtDataPtr->faceAttributeSet);

	return(kQ3Success);

//failure_5:
	while (iSave > 0)
		e3meshContour_Destroy(&contours[--iSave]);

	e3meshContourArray_Destroy(&facePtr->contourArrayOrList.array, NULL);
failure_4:

failure_3:

failure_2:

	e3meshPart_Destroy(&facePtr->part);
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshFace_Destroy : TE3MeshFaceData destructor.
//-----------------------------------------------------------------------------
static
void
e3meshFace_Destroy(
	TE3MeshFaceData* facePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);

	// Release attribute set
	Q3Object_CleanDispose(&facePtr->attributeSet);

	// Destroy contour array or list
	e3meshContourArrayOrList_Destroy(&facePtr->contourArrayOrList, &e3meshContour_Destroy);

	// Destroy part
	e3meshPart_Destroy(&facePtr->part);
}





//=============================================================================
//      e3meshFace_Relocate : Relocate face.
//-----------------------------------------------------------------------------
static
void
e3meshFace_Relocate(
	TE3MeshFaceData* newFacePtr,
	TE3MeshFaceData* oldFacePtr)
{
	// Relocate part
	e3meshPart_Relocate(
		&newFacePtr->part,
		&oldFacePtr->part);
}





//=============================================================================
//      e3meshFace_Tessellate : Tessellate face.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshFace_Tessellate(
	TE3MeshFaceData* facePtr,
	TE3MeshTessellator* tessellatorPtr)
{
	GLUtriangulatorObj* gluTessellatorPtr;	

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);
	Q3_ASSERT_VALID_PTR(tessellatorPtr);

	// Remember face attribute set (but don't increment reference count!)
	tessellatorPtr->faceAttributeSet = facePtr->attributeSet;

	gluTessellatorPtr = tessellatorPtr->gluTessellatorPtr;	

	gluTessBeginPolygon(gluTessellatorPtr, tessellatorPtr);
	e3meshContourArrayOrList_DoForEach(
		&facePtr->contourArrayOrList,
		E3_DOWN_CAST(TQ3Status (*)(TE3MeshContourData*, void*), &e3meshContour_Tessellate),
		gluTessellatorPtr);
	gluTessEndPolygon(gluTessellatorPtr);
	
	return(kQ3Success);
}





//=============================================================================
//      e3meshFace_RelinkContourFaces : Relink pointers to contour faces.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshFace_RelinkContourFaces(
	TE3MeshFaceData* facePtr,
	void* unusedArg)
{
    unusedArg; /* Suppress compiler warning */

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);

	return(e3meshContourArrayOrList_DoForEach(
		&facePtr->contourArrayOrList,
		&e3meshContour_RelinkContainerFace,
		NULL));
}





//=============================================================================
//      e3meshFace_UseContourArray : Use contour array.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshFace_UseContourArray(
	TE3MeshFaceData* facePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);

	return(e3meshContourArrayOrList_UseArray(
		&facePtr->contourArrayOrList,
		&e3meshContour_Relocate,
		NULL,
		NULL));
}





//=============================================================================
//      e3meshFace_UseContourList : Use contour list.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshFace_UseContourList(
	TE3MeshFaceData* facePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);

	return(e3meshContourArrayOrList_UseList(
		&facePtr->contourArrayOrList,
		&e3meshContour_Relocate,
		NULL,
		NULL));
}





//=============================================================================
//      e3meshFace_NumContours : Return number of contours in face.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3meshFace_NumContours(
	const TE3MeshFaceData* facePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);

	return(e3meshContourArrayOrList_Length(&facePtr->contourArrayOrList));
}





//=============================================================================
//      e3meshFace_RelinkContourVertices : Relink pointers to contour vertices.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshFace_RelinkContourVertices(
	TE3MeshFaceData* facePtr,
	void* unusedArg)
{
    unusedArg; /* Suppress compiler warning */

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);

	return(e3meshContourArrayOrList_DoForEach(
		&facePtr->contourArrayOrList,
		&e3meshContour_RelinkVertices,
		NULL));
}





//=============================================================================
//      e3meshFace_NumVertices : Return number of vertices in face.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is counted separately.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3meshFace_NumVertices(
	const TE3MeshFaceData* facePtr)
{
	TQ3Uns32 numVertices;
	const TE3MeshContourData* contourPtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);

	// Number of vertices in face is sum of number of vertices in each contour
	numVertices = 0;
	for (contourPtr = e3meshContourArrayOrList_FirstItemConst(&facePtr->contourArrayOrList);
		contourPtr != NULL;
		contourPtr = e3meshContourArrayOrList_NextItemConst(&facePtr->contourArrayOrList, contourPtr))
	{
		numVertices += e3meshContour_NumVertices(contourPtr);
	}

	return(numVertices);
}





//=============================================================================
//      e3meshFace_HasVertex : Return if face has vertex.
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3meshFace_HasVertex(
	const TE3MeshFaceData* facePtr,
	TE3MeshVertexData* vertexPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);
	Q3_ASSERT_VALID_PTR(vertexPtr);

	return(e3meshContourArrayOrList_OrForEach(
		&facePtr->contourArrayOrList,
		E3_DOWN_CAST(TQ3Boolean (*)(const TE3MeshContourData*, void*), &e3meshContour_HasVertex),
		vertexPtr));
}





//=============================================================================
//      e3meshFace_ExtRefInMesh : Return external reference to face.
//-----------------------------------------------------------------------------
//		Note : If unable to get external reference (out of memory), return NULL.
//-----------------------------------------------------------------------------
static
TE3MeshFaceExtRef
e3meshFace_ExtRefInMesh(
	TE3MeshFaceData* facePtr, 
	TE3MeshData* meshPtr)
{
	return(E3_DOWN_CAST(TE3MeshFaceExtRef,
		e3meshPart_HandleInMesh(
			&facePtr->part,
			meshPtr)));
}





//=============================================================================
//      e3meshFace_GetExtData : Get external data.
//-----------------------------------------------------------------------------
//		Note : If unable to get (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3meshFace_GetExtData(
	const TE3MeshFaceData* facePtr,
	TQ3MeshFaceData* faceExtDataPtr,
	const TE3MeshVertexDataArray* meshVertexArrayPtr)
{
	TQ3Uns32 					iSave;
	TE3MeshContourData*	contours;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(facePtr);
	Q3_ASSERT_VALID_PTR(faceExtDataPtr);
	Q3_ASSERT_VALID_PTR(meshVertexArrayPtr);
	
	faceExtDataPtr->numContours = e3meshFace_NumContours(facePtr);

	if (faceExtDataPtr->numContours == 0)
		faceExtDataPtr->contours = NULL;
	else
		if ((faceExtDataPtr->contours = Q3Memory_Allocate(faceExtDataPtr->numContours * sizeof(TQ3MeshContourData))) == NULL)
			goto failure_2;
	
	// Use list of contours in face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshFace_UseContourArray((TE3MeshFaceData*)facePtr) == kQ3Failure)
		goto failure;

	contours = e3meshContourArray_FirstItem((TE3MeshContourDataArray*)&facePtr->contourArrayOrList.array);

	for (iSave = 0; iSave < faceExtDataPtr->numContours; ++iSave)
		if (e3meshContour_GetExtData(&contours[iSave], &faceExtDataPtr->contours[iSave], meshVertexArrayPtr) == kQ3Failure)
			goto failure;

	// Get attribute set
	E3Shared_Acquire(&faceExtDataPtr->faceAttributeSet, facePtr->attributeSet);

	return(kQ3Success);

failure:
	while (iSave > 0)
		e3meshContourExtData_Empty(&faceExtDataPtr->contours[--iSave]);

	Q3Memory_Free(&faceExtDataPtr->contours);

failure_2:
	return(kQ3Failure);
}





//=============================================================================
//      e3meshFaceExtRef_Face : Return face for external reference.
//-----------------------------------------------------------------------------
//		Note : If face deleted, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3MeshFaceData*
e3meshFaceExtRef_Face(
	TE3MeshFaceExtRef faceExtRef)
{
	return(E3_DOWN_CAST(TE3MeshFaceData*,
		e3meshPartHdl_Part(
			E3_UP_CAST(TE3MeshPartData**, faceExtRef))));
}





//=============================================================================
//      e3meshFaceExtRef_Mesh : Return mesh for external reference.
//-----------------------------------------------------------------------------
static
TE3MeshData*
e3meshFaceExtRef_Mesh(
	TE3MeshFaceExtRef faceExtRef)
{
	return(e3meshPartHdl_Mesh(E3_UP_CAST(TE3MeshPartData**, faceExtRef)));
}





//=============================================================================
//      e3meshFaceExtData_Empty : Empty external data.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshFaceExtData_Empty(
	TQ3MeshFaceData* faceExtDataPtr)
{
	TQ3Status result = kQ3Success;

	TQ3Uns32 numContours;
	TQ3MeshContourData* contourExtDatas;
	TQ3Uns32 i;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(faceExtDataPtr);

	// Release attribute set
	Q3Object_CleanDispose(&faceExtDataPtr->faceAttributeSet);

	// Validate and empty contours
	numContours = faceExtDataPtr->numContours;
	contourExtDatas = faceExtDataPtr->contours;
	if (numContours > 0 && contourExtDatas == NULL)
		result = kQ3Failure;
	else
	{
		// Empty contours
		for (i = 0; i < numContours; ++i)
			if (e3meshContourExtData_Empty(&contourExtDatas[i]) == kQ3Failure)
				result = kQ3Failure;
	}

	// Deallocate array of contours
	Q3Memory_Free(&faceExtDataPtr->contours);
	faceExtDataPtr->numContours = 0;
	
	return(result);
}





//=============================================================================
//      e3meshTessellatorCallback_Begin : Begin a new triangle.
//-----------------------------------------------------------------------------
#pragma mark -
static
void CALLBACK
e3meshTessellatorCallback_Begin(
	GLenum which,
	TE3MeshTessellator* tessellatorPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tessellatorPtr);

	// Validate our state - since we asked for edge data, we should only see triangles
	Q3_ASSERT(which == GL_TRIANGLES);
	
	tessellatorPtr->polyhedronTriangle.edgeFlag = kQ3PolyhedronEdgeNone;
	// Copy face attribute set to triangle (but don't increment reference count!)
	tessellatorPtr->polyhedronTriangle.triangleAttributeSet = tessellatorPtr->faceAttributeSet;
	tessellatorPtr->vertexIdx = 0;
	tessellatorPtr->isEdge = kQ3True;
}





//=============================================================================
//      e3meshTessellatorCallback_End : End the current triangle.
//-----------------------------------------------------------------------------
static
void CALLBACK
e3meshTessellatorCallback_End(
	TE3MeshTessellator* tessellatorPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tessellatorPtr);

	// Push back polyhedron triangle
	if (e3polyhedronTriangleList_PushBackItem(&tessellatorPtr->polyhedronTriangleArrayOrList.list, &tessellatorPtr->polyhedronTriangle) == NULL)
		tessellatorPtr->status = kQ3Failure;
}





//=============================================================================
//      e3meshTessellatorCallback_Edge : Update the edge state.
//-----------------------------------------------------------------------------
static
void CALLBACK
e3meshTessellatorCallback_Edge(
	GLboolean isEdge,
	TE3MeshTessellator* tessellatorPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tessellatorPtr);
	
	tessellatorPtr->isEdge = isEdge == GL_TRUE ? kQ3True : kQ3False;
}





//=============================================================================
//      e3meshTessellatorCallback_Vertex : Process another vertex.
//-----------------------------------------------------------------------------
static
void CALLBACK
e3meshTessellatorCallback_Vertex(
	TE3MeshVertexData* vertexPtr,
	TE3MeshTessellator* tessellatorPtr)
{
	static const TQ3PolyhedronEdge kEdgeFlags[3] = {
		kQ3PolyhedronEdge01,
		kQ3PolyhedronEdge12,
		kQ3PolyhedronEdge20
	};
		
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(vertexPtr);
	Q3_ASSERT_VALID_PTR(tessellatorPtr);

	// If we're not getting individual begin/end callbacks for each triangle,
	// our index will eventually overflow - once this happens, we need to
	// close this triangle and start the next one ourselves.
	if (tessellatorPtr->vertexIdx == 3)
	{
		e3meshTessellatorCallback_End(tessellatorPtr);
		e3meshTessellatorCallback_Begin(GL_TRIANGLES, tessellatorPtr);
	}

	tessellatorPtr->polyhedronTriangle.vertexIndices[tessellatorPtr->vertexIdx] =
		e3meshVertexArray_ItemIndex(tessellatorPtr->vertexArrayPtr, vertexPtr);

	if (tessellatorPtr->isEdge)
		tessellatorPtr->polyhedronTriangle.edgeFlag |= kEdgeFlags[tessellatorPtr->vertexIdx];

	++tessellatorPtr->vertexIdx;
}





//=============================================================================
//      e3meshTessellatorCallback_Combine : Combine vertices to form a new vertex.
//-----------------------------------------------------------------------------
static
void CALLBACK
e3meshTessellatorCallback_Combine(
	const GLdouble pointIn[3],
	const void* dataIn[4],
	const GLfloat theWeights[4],
	void** dataOut,
	TE3MeshTessellator* tessellatorPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tessellatorPtr);
}





//=============================================================================
//      e3meshTessellatorCallback_Error : Store an error code in the tessellator state.
//-----------------------------------------------------------------------------
static
void CALLBACK
e3meshTessellatorCallback_Error(
	GLenum errorCode,
	TE3MeshTessellator* tessellatorPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tessellatorPtr);
}





//=============================================================================
//      e3meshTessellator_Create : TE3MeshTessellator constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshTessellator_Create(
	TE3MeshTessellator* tessellatorPtr,
	const TE3MeshVertexDataArray* vertexArrayPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tessellatorPtr);
	Q3_ASSERT_VALID_PTR(vertexArrayPtr);

	// Save vertex array
	tessellatorPtr->vertexArrayPtr = vertexArrayPtr;

	// Initialize face attribute set
	tessellatorPtr->faceAttributeSet = NULL;

	// Create new OpenGL tessellator
	if ((tessellatorPtr->gluTessellatorPtr = gluNewTess()) == NULL)
	{
		E3ErrorManager_PostError(kQ3ErrorOutOfMemory, kQ3False);
		goto failure_1;
	}

	// Create empty list of polyhedron triangles
	if (e3polyhedronTriangleList_Create(&tessellatorPtr->polyhedronTriangleArrayOrList.list, 0, NULL) == kQ3Failure)
		goto failure_2;

	tessellatorPtr->status = kQ3Success;
	
	return(kQ3Success);
	
	// Dead code to reverse e3polyhedronTriangleList_Create
failure_2:
	
	gluDeleteTess(tessellatorPtr->gluTessellatorPtr);
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshTessellator_Destroy : TE3MeshTessellator destructor.
//-----------------------------------------------------------------------------
static
void
e3meshTessellator_Destroy(
	TE3MeshTessellator* tessellatorPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tessellatorPtr);

	// Destroy polyhedron triangle list
	e3polyhedronTriangleArrayOrList_Destroy(&tessellatorPtr->polyhedronTriangleArrayOrList, NULL);

	// Delete OpenGL tessellator
	gluDeleteTess(tessellatorPtr->gluTessellatorPtr);

	// Forget face attribute set (don't decrement reference count!)
	//	???
}





//=============================================================================
//      e3mesh_Create : TE3MeshData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3mesh_Create(
	TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	// Create pool of part references
	if (e3meshPartPtrPool_Create(&meshPtr->partPtrPool) == kQ3Failure)
		goto failure_1;

	// Initialize number of corners
	meshPtr->numCorners = 0;

	// Create empty array of vertices
	if (e3meshVertexArray_Create(&meshPtr->vertexArrayOrList.array, 0, NULL) == kQ3Failure)
		goto failure_2;

	// Create empty array of faces
	if (e3meshFaceArray_Create(&meshPtr->faceArrayOrList.array, 0, NULL) == kQ3Failure)
		goto failure_3;

	// Initialize attribute set
	meshPtr->attributeSet = NULL;

	return(kQ3Success);
	
	// Dead code to reverse e3meshFaceArray_Create
failure_3:

	e3meshVertexArray_Destroy(&meshPtr->vertexArrayOrList.array, &e3meshVertex_Destroy);
failure_2:

	e3meshPartPtrPool_Destroy(&meshPtr->partPtrPool);
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3mesh_CreateFromExtData : TE3MeshData constructor from external data.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3mesh_CreateFromExtData(
	TE3MeshData* meshPtr,
	const TQ3MeshData* meshExtDataPtr)
{
	TQ3Uns32 numVertices;
	const TQ3MeshVertexData* vertexExtDatas;
	TQ3Uns32 numFaces;
	const TQ3MeshFaceData* faceExtDatas;
	TE3MeshVertexData* vertices;
	TQ3Uns32 iSave;
	TE3MeshFaceData* faces;
	TQ3Uns32 jSave;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);
	Q3_ASSERT_VALID_PTR(meshExtDataPtr);

	// Create pool of part references
	if (e3meshPartPtrPool_Create(&meshPtr->partPtrPool) == kQ3Failure)
		goto failure_1;

	// Initialize number of corners
	meshPtr->numCorners = 0;

	// Get and validate vertices
	numVertices = meshExtDataPtr->numVertices;
	vertexExtDatas = meshExtDataPtr->vertices;
	if (numVertices > 0 && vertexExtDatas == NULL)
		goto failure_2;

	// Create array of uninitialized vertices
	if (e3meshVertexArray_Create(&meshPtr->vertexArrayOrList.array, meshExtDataPtr->numVertices, NULL) == kQ3Failure)
		goto failure_3;

	// Get and validate faces
	numFaces = meshExtDataPtr->numFaces;
	faceExtDatas = meshExtDataPtr->faces;
	if (numFaces > 0 && faceExtDatas == NULL)
		goto failure_4;

	// Create array of uninitialized faces
	if (e3meshFaceArray_Create(&meshPtr->faceArrayOrList.array, meshExtDataPtr->numFaces, NULL) == kQ3Failure)
		goto failure_5;

	// Initialize vertices
	vertices = e3meshVertexArray_FirstItem(&meshPtr->vertexArrayOrList.array);
	for (iSave = 0; iSave < numVertices; ++iSave)
	{
		if (e3meshVertex_CreateFromExtData(&vertices[iSave], &vertexExtDatas[iSave], &meshPtr->faceArrayOrList.array) == kQ3Failure)
			goto failure_6;
		
		meshPtr->numCorners += vertexExtDatas[iSave].numCorners;
	}

	// Initialize faces
	faces = e3meshFaceArray_FirstItem(&meshPtr->faceArrayOrList.array);
	for (jSave = 0; jSave < numFaces; ++jSave)
		if (e3meshFace_CreateFromExtData(&faces[jSave], &faceExtDatas[jSave], &meshPtr->vertexArrayOrList.array) == kQ3Failure)
			goto failure_7;

	// Set attribute set
	E3Shared_Acquire(&meshPtr->attributeSet, meshExtDataPtr->meshAttributeSet);

	return(kQ3Success);

failure_7:
	while (jSave > 0)
		e3meshFace_Destroy(&faces[--jSave]);

failure_6:
	while (iSave > 0)
		e3meshVertex_Destroy(&vertices[--iSave]);

	e3meshFaceArray_Destroy(&meshPtr->faceArrayOrList.array, NULL);
failure_5:

failure_4:

	e3meshVertexArray_Destroy(&meshPtr->vertexArrayOrList.array, NULL);
failure_3:

failure_2:

	e3meshPartPtrPool_Destroy(&meshPtr->partPtrPool);
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3mesh_Destroy : TE3MeshData destructor.
//-----------------------------------------------------------------------------
static
void
e3mesh_Destroy(
	TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	//	Release attribute set
	Q3Object_CleanDispose(&meshPtr->attributeSet);

	// Destroy face array or list
	e3meshFaceArrayOrList_Destroy(&meshPtr->faceArrayOrList, &e3meshFace_Destroy);

	// Destroy vertex array or list
	e3meshVertexArrayOrList_Destroy(&meshPtr->vertexArrayOrList, &e3meshVertex_Destroy);

	// Destroy part references pool
	e3meshPartPtrPool_Destroy(&meshPtr->partPtrPool);
}





//=============================================================================
//      e3mesh_NumCorners : Return number of (defined) corners in mesh.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3mesh_NumCorners(
	const TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	return(meshPtr->numCorners);
}





//=============================================================================
//      e3mesh_RelinkVertices : Relink pointers to vertices.
//-----------------------------------------------------------------------------
static
void
e3mesh_RelinkVertices(
	TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	e3meshFaceArrayOrList_DoForEach(
		&meshPtr->faceArrayOrList,
		&e3meshFace_RelinkContourVertices,
		NULL);
}





//=============================================================================
//      e3mesh_UseVertexArray : Use vertex array.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3mesh_UseVertexArray(
	TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	return(e3meshVertexArrayOrList_UseArray(
		&meshPtr->vertexArrayOrList,
		&e3meshVertex_Relocate,
		E3_UP_CAST(void (*)(void*), &e3mesh_RelinkVertices),
		meshPtr));
}





//=============================================================================
//      e3mesh_UseVertexList : Use vertex list.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3mesh_UseVertexList(
	TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	return(e3meshVertexArrayOrList_UseList(
		&meshPtr->vertexArrayOrList,
		&e3meshVertex_Relocate,
		E3_UP_CAST(void (*)(void*), &e3mesh_RelinkVertices),
		meshPtr));
}





//=============================================================================
//      e3mesh_NumVertices : Return number of vertices in mesh.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3mesh_NumVertices(
	const TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	return(e3meshVertexArrayOrList_Length(&meshPtr->vertexArrayOrList));
}





//=============================================================================
//      e3mesh_NumContours : Return number of contours in mesh.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3mesh_NumContours(
	const TE3MeshData* meshPtr)
{
	TQ3Uns32 numContours;
	const TE3MeshFaceData* facePtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	// Number of contours in mesh is sum of number of contours in each face
	numContours = 0;
	for (facePtr = e3meshFaceArrayOrList_FirstItemConst(&meshPtr->faceArrayOrList);
		facePtr != NULL;
		facePtr = e3meshFaceArrayOrList_NextItemConst(&meshPtr->faceArrayOrList, facePtr))
	{
		numContours += e3meshFace_NumContours(facePtr);
	}

	return(numContours);
}





//=============================================================================
//      e3mesh_RelinkFaces : Relink pointers to faces.
//-----------------------------------------------------------------------------
static
void
e3mesh_RelinkFaces(
	TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	e3meshVertexArrayOrList_DoForEach(
		&meshPtr->vertexArrayOrList,
		&e3meshVertex_RelinkCornerFaces,
		NULL);

	e3meshFaceArrayOrList_DoForEach(
		&meshPtr->faceArrayOrList,
		&e3meshFace_RelinkContourFaces,
		NULL);
}





//=============================================================================
//      e3mesh_UseFaceArray : Use face array.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3mesh_UseFaceArray(
	TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	return(e3meshFaceArrayOrList_UseArray(
		&meshPtr->faceArrayOrList,
		&e3meshFace_Relocate,
		E3_UP_CAST(void (*)(void*), &e3mesh_RelinkFaces),
		meshPtr));
}





//=============================================================================
//      e3mesh_UseFaceList : Use face list.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3mesh_UseFaceList(
	TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	return(e3meshFaceArrayOrList_UseList(
		&meshPtr->faceArrayOrList,
		&e3meshFace_Relocate,
		E3_UP_CAST(void (*)(void*), &e3mesh_RelinkFaces),
		meshPtr));
}





//=============================================================================
//      e3mesh_NumFaces : Return number of faces in mesh.
//-----------------------------------------------------------------------------
static
TQ3Uns32
e3mesh_NumFaces(
	const TE3MeshData* meshPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);

	return(e3meshFaceArrayOrList_Length(&meshPtr->faceArrayOrList));
}





//=============================================================================
//      e3mesh_SetExtData : Set mesh from external data.
//-----------------------------------------------------------------------------
static
TQ3Status
e3mesh_SetExtData(
	TE3MeshData* meshPtr,
	const TQ3MeshData* meshExtDataPtr)
{
	TE3MeshData tempMesh;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);
	Q3_ASSERT_VALID_PTR(meshExtDataPtr);

	// Create new temporary mesh from data
	if (e3mesh_CreateFromExtData(&tempMesh, meshExtDataPtr) == kQ3Failure)
		goto failure;

	// Destroy old mesh
	e3mesh_Destroy(meshPtr);

	// Swap new temporary mesh in for old mesh
	*meshPtr = tempMesh;

	return(kQ3Success);

failure:

	return(kQ3Failure);
}





//=============================================================================
//      e3mesh_GetExtData : Get external data from mesh.
//-----------------------------------------------------------------------------
static
TQ3Status
e3mesh_GetExtData(
	const TE3MeshData* meshPtr,
	TQ3MeshData* meshExtDataPtr)
{
	const TE3MeshVertexData* vertices;
	TQ3Uns32 iSave;

	const TE3MeshFaceData* faces;
	TQ3Uns32 jSave;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPtr);
	Q3_ASSERT_VALID_PTR(meshExtDataPtr);

	// Use array of vertices (*** MAY RELOCATE VERTICES ***)
	if (e3mesh_UseVertexArray(E3_CONST_CAST(TE3MeshData*, meshPtr)) == kQ3Failure)
		goto failure_1;

	// Allocate array of uninitialized vertices
	meshExtDataPtr->numVertices = e3mesh_NumVertices(meshPtr);
	if (meshExtDataPtr->numVertices == 0)
		meshExtDataPtr->vertices = NULL;
	else if ((meshExtDataPtr->vertices = Q3Memory_Allocate(meshExtDataPtr->numVertices * sizeof(TQ3MeshVertexData))) == NULL)
		goto failure_2;

	// Get vertices
	for (iSave = 0, vertices = e3meshVertexArray_FirstItemConst(&meshPtr->vertexArrayOrList.array);
		iSave < meshExtDataPtr->numVertices;
		++iSave, ++vertices)
	{
		if (e3meshVertex_GetExtData(vertices, &meshExtDataPtr->vertices[iSave], &meshPtr->faceArrayOrList.array) == kQ3Failure)
			goto failure_3;
	}

	// Use array of faces (*** MAY RELOCATE FACES ***)
	if (e3mesh_UseFaceArray(E3_CONST_CAST(TE3MeshData*, meshPtr)) == kQ3Failure)
		goto failure_4;

	// Allocate array of uninitialized faces
	meshExtDataPtr->numFaces = e3mesh_NumFaces(meshPtr);
	if (meshExtDataPtr->numFaces == 0)
		meshExtDataPtr->faces = NULL;
	else if ((meshExtDataPtr->faces = Q3Memory_Allocate(meshExtDataPtr->numFaces * sizeof(TQ3MeshFaceData))) == NULL)
		goto failure_5;

	// Get faces
	for (jSave = 0, faces = e3meshFaceArray_FirstItemConst(&meshPtr->faceArrayOrList.array);
		jSave < meshExtDataPtr->numFaces;
		++jSave, ++faces)
	{
		if (e3meshFace_GetExtData(faces, &meshExtDataPtr->faces[jSave], &meshPtr->vertexArrayOrList.array) == kQ3Failure)
			goto failure_6;
	}

	// Get attribute set
	E3Shared_Acquire(&meshExtDataPtr->meshAttributeSet, meshPtr->attributeSet);

	return(kQ3Success);

failure_6:
	while (jSave > 0)
		e3meshFaceExtData_Empty(&meshExtDataPtr->faces[--jSave]);

	Q3Memory_Free(&meshExtDataPtr->faces);
failure_5:

failure_4:

failure_3:
	while (iSave > 0)
		e3meshVertexExtData_Empty(&meshExtDataPtr->vertices[--iSave]);

	Q3Memory_Free(&meshExtDataPtr->vertices);
failure_2:

failure_1:

	return(kQ3Failure);
}





//=============================================================================
//      e3meshExtData_Empty : Empty external data.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Status
e3meshExtData_Empty(
	TQ3MeshData* meshExtDataPtr)
{
	TQ3Status result = kQ3Success;

	TQ3Uns32 numFaces;
	TQ3MeshFaceData* faceExtDatas;
	TQ3Uns32 j;

	TQ3Uns32 numVertices;
	TQ3MeshVertexData* vertexExtDatas;
	TQ3Uns32 i;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshExtDataPtr);

	// Release attribute set
	Q3Object_CleanDispose(&meshExtDataPtr->meshAttributeSet);

	// Validate and empty faces
	numFaces = meshExtDataPtr->numFaces;
	faceExtDatas = meshExtDataPtr->faces;
	if (numFaces > 0 && faceExtDatas == NULL)
		result = kQ3Failure;
	else
	{
		// Empty faces
		for (j = 0; j < numFaces; ++j)
			if (e3meshFaceExtData_Empty(&faceExtDatas[j]) == kQ3Failure)
				result = kQ3Failure;
	}

	// Deallocate array of faces
	Q3Memory_Free(&meshExtDataPtr->faces);
	meshExtDataPtr->numFaces = 0;

	// Validate and empty vertices
	numVertices = meshExtDataPtr->numVertices;
	vertexExtDatas = meshExtDataPtr->vertices;
	if (numVertices > 0 && vertexExtDatas == NULL)
		result = kQ3Failure;
	else
	{
		// Empty vertices
		for (i = 0; i < numVertices; ++i)
			if (e3meshVertexExtData_Empty(&vertexExtDatas[i]) == kQ3Failure)
				result = kQ3Failure;
	}

	// Deallocate array of vertices
	Q3Memory_Free(&meshExtDataPtr->vertices);
	meshExtDataPtr->numVertices = 0;

	return(result);
}





//=============================================================================
//      e3meshIterator_Initialize : TQ3MeshIterator partial constructor.
//-----------------------------------------------------------------------------
#pragma mark -
static
void
e3meshIterator_Initialize(
	TQ3MeshIterator* iteratorPtr,
	TE3MeshData* meshPtr,
	const char* iteratorKind)
{
	// Save mesh
	iteratorPtr->var4.field1 = meshPtr;

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
e3geom_mesh_new(
	TQ3Object theObject,
	void *privateData,
	const void *paramData)
{
#pragma unused(theObject)
#pragma unused(paramData)

	return(e3mesh_Create(E3_DOWN_CAST(TE3MeshData*, privateData)));
}





//=============================================================================
//      e3geom_mesh_delete : TE3MeshData destructor.
//-----------------------------------------------------------------------------
static
void
e3geom_mesh_delete(
	TQ3Object theObject,
	void *privateData)
{
#pragma unused(theObject)

	e3mesh_Destroy(E3_DOWN_CAST(TE3MeshData*, privateData));
}





//=============================================================================
//      e3geom_mesh_duplicate : Mesh duplicate method.
//-----------------------------------------------------------------------------
static
TQ3Status
e3geom_mesh_duplicate(
	TQ3Object fromObject,
	const void *fromPrivateData,
	TQ3Object toObject,
	void *toPrivateData)
{	const TE3MeshData*		fromInstanceData = (const TE3MeshData*) fromPrivateData;
	TE3MeshData*			toInstanceData   = (TE3MeshData*)       toPrivateData;
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
//      e3geom_mesh_cache_new_as_polys : Mesh cache new method.
//-----------------------------------------------------------------------------
static
TQ3Object
e3geom_mesh_cache_new_as_polys(
	const TE3MeshData* meshPtr)
{
#define _MESH_AS_POLYS_OBJECTS_TO_DELETE_GROW 16

	const TE3MeshFaceData* 			facePtr;
	const TE3MeshContourData* 		contourPtr;
	const TE3MeshVertexPtr* 		vertexHdl;
	const TE3MeshCornerData* 		cornerPtr;
	
	TQ3GroupObject 					thePolysGroup = NULL;
	TQ3GeneralPolygonData			polyData;
	TQ3Object						*objectsToDelete;
	TQ3Uns32						numObjectsToDelete;
	TQ3Uns32						allocatedObjectsToDelete;
	TQ3Vertex3D						*currentVertex;
	TQ3Object 						thePoly;
	TQ3Uns32						i,j;
	
	
    polyData.contours = NULL;
    polyData.shapeHint = kQ3GeneralPolygonShapeHintComplex;
    polyData.generalPolygonAttributeSet = NULL;
    


	thePolysGroup = Q3OrderedDisplayGroup_New ();
	if(thePolysGroup == NULL)
		return NULL;
	
	objectsToDelete = Q3Memory_Allocate(_MESH_AS_POLYS_OBJECTS_TO_DELETE_GROW * sizeof(TQ3Object));
	if(objectsToDelete == NULL)
		return thePolysGroup;
		
	allocatedObjectsToDelete = _MESH_AS_POLYS_OBJECTS_TO_DELETE_GROW;
	
	// add the mesh attribute Set to the group
	if(meshPtr->attributeSet != NULL)
		Q3Group_AddObject(thePolysGroup,meshPtr->attributeSet);
		
	numObjectsToDelete = 0;
		
	
	for (facePtr = e3meshFaceArrayOrList_FirstItemConst(&meshPtr->faceArrayOrList);
		facePtr != NULL;
		facePtr = e3meshFaceArrayOrList_NextItemConst(&meshPtr->faceArrayOrList, facePtr))
		{
		numObjectsToDelete = 0;
		
		polyData.numContours = e3meshFace_NumContours(facePtr);
		polyData.contours = Q3Memory_AllocateClear(polyData.numContours * sizeof(TQ3GeneralPolygonContourData));
		if(polyData.contours == NULL)
			goto cleanup;
		
	    polyData.generalPolygonAttributeSet = facePtr->attributeSet;
		
		for (contourPtr = e3meshContourArrayOrList_FirstItemConst(&facePtr->contourArrayOrList), i = 0;
			contourPtr != NULL;
			contourPtr = e3meshContourArrayOrList_NextItemConst(&facePtr->contourArrayOrList, contourPtr), ++i)
			{
			polyData.contours[i].numVertices = e3meshContour_NumVertices(contourPtr);
			polyData.contours[i].vertices = Q3Memory_Allocate(polyData.contours[i].numVertices * sizeof(TQ3Vertex3D));
			if(polyData.contours == NULL)
				goto cleanup;


			for (vertexHdl = e3meshVertexPtrArray_FirstItemConst(&contourPtr->vertexPtrArray), j = 0;
				vertexHdl != NULL;
				vertexHdl = e3meshVertexPtrArray_NextItemConst(&contourPtr->vertexPtrArray, vertexHdl), ++j)
				{

				currentVertex = &polyData.contours[i].vertices[j];
				currentVertex->point = (*vertexHdl)->point;
				currentVertex->attributeSet = (*vertexHdl)->attributeSet;

				cornerPtr = e3meshVertex_FaceCorner(*vertexHdl, facePtr);
				if(cornerPtr != NULL)
					{
					if(cornerPtr->attributeSet != NULL)
						{
						if(currentVertex->attributeSet != NULL)
							{
							currentVertex->attributeSet = Q3AttributeSet_New();
							if(currentVertex->attributeSet == NULL)
								{
								currentVertex->attributeSet = (*vertexHdl)->attributeSet;
								}
							else
								{
								if((1 + numObjectsToDelete) > allocatedObjectsToDelete)
									{
									if(Q3Memory_Reallocate(&objectsToDelete, (allocatedObjectsToDelete + _MESH_AS_POLYS_OBJECTS_TO_DELETE_GROW)*sizeof(TQ3Object)) != kQ3Success)
										goto cleanup;
										
									allocatedObjectsToDelete += _MESH_AS_POLYS_OBJECTS_TO_DELETE_GROW;
									}
									
								objectsToDelete[numObjectsToDelete] = currentVertex->attributeSet;
								numObjectsToDelete++;
								
								Q3AttributeSet_Inherit((*vertexHdl)->attributeSet, cornerPtr->attributeSet, currentVertex->attributeSet);
								}
							}
						else
							{
							currentVertex->attributeSet = cornerPtr->attributeSet;
							}
						}
					}
					
				}
						
			}
			
		thePoly = Q3GeneralPolygon_New(&polyData);
		if(thePoly != NULL){
			Q3Group_AddObject(thePolysGroup,thePoly);
			Q3Object_Dispose(thePoly);
			}
			
		while(numObjectsToDelete > 0)
			{
			numObjectsToDelete --;
			Q3Object_Dispose(objectsToDelete[numObjectsToDelete]);
			};
			
		if(polyData.contours != NULL)
			{
			for(i = 0; i < polyData.numContours; i++)
				{
				if(polyData.contours[i].vertices != NULL)
					Q3Memory_Free(&polyData.contours[i].vertices);
				}
			Q3Memory_Free(&polyData.contours);
			}
		}
		
cleanup:	
	while(numObjectsToDelete > 0)
		{
		numObjectsToDelete --;
		Q3Object_Dispose(objectsToDelete[numObjectsToDelete]);
		};
		
	if(objectsToDelete != NULL)	
		Q3Memory_Free(&objectsToDelete);
		
	if(polyData.contours != NULL)
		{
		for(i = 0; i < polyData.numContours; i++)
			{
			if(polyData.contours[i].vertices != NULL)
				Q3Memory_Free(&polyData.contours[i].vertices);
			}
		Q3Memory_Free(&polyData.contours);
		}
	
	return thePolysGroup;

#undef _MESH_AS_POLYS_OBJECTS_TO_DELETE_GROW

}





//=============================================================================
//      e3geom_mesh_cache_new_as_polyhedron : Mesh cache new method.
//-----------------------------------------------------------------------------
static
TQ3Object
e3geom_mesh_cache_new_as_polyhedron(
	const TE3MeshData* meshPtr)
{
/*
	TQ3GeometryObject polyhedron;
	TQ3PolyhedronData polyhedronData;	
	const TE3MeshFaceData* facePtr;
	const TE3MeshVertexData* firstMeshVertexPtr;
	const TE3MeshVertexPtr* vertexHdl;
	TQ3Uns32 i, k;
	


	// Assume unable to create polyhedron
	polyhedron = NULL;
	
	// Allocate memory for polyhedron vertices
	polyhedronData.numVertices = e3mesh_NumVertices(meshPtr);
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
	for (facePtr = e3meshFaceArrayOrList_FirstItemConst(&meshPtr->faceArrayOrList);
		facePtr != NULL;
		facePtr = e3meshFaceArrayOrList_NextItemConst(&meshPtr->faceArrayOrList, facePtr))
	{
		if (e3meshFace_NumContours(facePtr) == 1)
			polyhedronData.numTriangles += e3meshFace_NumVertices(facePtr) - 2;
	}
	if (polyhedronData.numTriangles > 0)
	{
		if ((polyhedronData.triangles = (TQ3PolyhedronTriangleData*) Q3Memory_Allocate(polyhedronData.numTriangles * sizeof(TQ3PolyhedronTriangleData))) == NULL)
			goto cleanup_3;
	}
	else
		polyhedronData.triangles = NULL;

	// Use array of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3mesh_UseVertexArray(E3_CONST_CAST(TE3MeshData*, meshPtr)) == kQ3Failure)
		goto cleanup_4;
	
	// Get first vertex
	firstMeshVertexPtr = e3meshVertexArray_FirstItemConst(&meshPtr->vertexArrayOrList.array);

	// Initialize the vertices
	for (i = 0; i < polyhedronData.numVertices; ++i)
	{
		polyhedronData.vertices[i].point = firstMeshVertexPtr[i].point;
		E3Shared_Acquire(&polyhedronData.vertices[i].attributeSet, firstMeshVertexPtr[i].attributeSet);
	}
	
	// Initialize the triangles
	k = 0;
	for (facePtr = e3meshFaceArrayOrList_FirstItemConst(&meshPtr->faceArrayOrList);
		facePtr != NULL;
		facePtr = e3meshFaceArrayOrList_NextItemConst(&meshPtr->faceArrayOrList, facePtr))
	{
		if (e3meshFace_NumContours(facePtr) == 1)
		{
			const TE3MeshContourData* contourPtr = e3meshContourArrayOrList_FirstItemConst(&facePtr->contourArrayOrList);
			TQ3Uns32 numContourVertices = e3meshContour_NumVertices(contourPtr);
			
			for (vertexHdl = e3meshVertexPtrArray_FirstItemConst(&contourPtr->vertexPtrArray), i = 0;
				vertexHdl != NULL;
				vertexHdl = e3meshVertexPtrArray_NextItemConst(&contourPtr->vertexPtrArray, vertexHdl), ++i)
			{
				TQ3Uns32 vertexIndex = e3meshVertexArray_ItemIndex(&meshPtr->vertexArrayOrList.array, *vertexHdl);
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
					if (i == numContourVertices-1)
						polyhedronData.triangles[k].edgeFlag |= kQ3PolyhedronEdge20;
					polyhedronData.triangles[k].triangleAttributeSet = facePtr->attributeSet;
					++k;
					break;
				}
					
			}
		}
	}
	polyhedronData.polyhedronAttributeSet = meshPtr->attributeSet;

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
*/
	TE3MeshTessellator tessellator;
	GLUtriangulatorObj* gluTessellatorPtr;
	TQ3GeometryObject polyhedron;
	TQ3PolyhedronData polyhedronData;	
	const TE3MeshVertexData* firstMeshVertexPtr;
	TQ3Uns32 vertexIdx;
	TQ3Uns32 triangleIndex;
	const TE3MeshFaceData* facePtr;

	// Assume unable to create polyhedron
	polyhedron = NULL;

	// Use array of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3mesh_UseVertexArray(E3_CONST_CAST(TE3MeshData*, meshPtr)) == kQ3Failure)
		goto cleanup_1;

	// Create tessellator
	if (e3meshTessellator_Create(&tessellator, &meshPtr->vertexArrayOrList.array) == kQ3Failure)
		goto cleanup_2;

	gluTessellatorPtr = tessellator.gluTessellatorPtr;

	// Set it up
	gluTessProperty(gluTessellatorPtr, GLU_TESS_WINDING_RULE,   GLU_TESS_WINDING_ODD);
	gluTessCallback(gluTessellatorPtr, GLU_TESS_BEGIN_DATA,     (GLcallback) e3meshTessellatorCallback_Begin);
	gluTessCallback(gluTessellatorPtr, GLU_TESS_END_DATA,       (GLcallback) e3meshTessellatorCallback_End);
	gluTessCallback(gluTessellatorPtr, GLU_TESS_EDGE_FLAG_DATA, (GLcallback) e3meshTessellatorCallback_Edge);
	gluTessCallback(gluTessellatorPtr, GLU_TESS_VERTEX_DATA,    (GLcallback) e3meshTessellatorCallback_Vertex);
//	gluTessCallback(gluTessellatorPtr, GLU_TESS_COMBINE_DATA,   (GLcallback) e3meshTessellatorCallback_Combine);
//	gluTessCallback(gluTessellatorPtr, GLU_TESS_ERROR_DATA,     (GLcallback) e3meshTessellatorCallback_Error);

	e3meshFaceArrayOrList_DoForEach(
		E3_CONST_CAST(TE3MeshFaceDataArrayOrList*, &meshPtr->faceArrayOrList),
		E3_DOWN_CAST(TQ3Status (*)(TE3MeshFaceData*, void*), &e3meshFace_Tessellate),
		&tessellator);
	
	// Allocate memory for polyhedron vertices
	polyhedronData.numVertices = e3mesh_NumVertices(meshPtr);
	if (polyhedronData.numVertices > 0)
	{
		if ((polyhedronData.vertices = (TQ3Vertex3D*) Q3Memory_Allocate(polyhedronData.numVertices * sizeof(TQ3Vertex3D))) == NULL)
			goto cleanup_3;
	}
	else
		polyhedronData.vertices = NULL;
	
	// Get first vertex
	firstMeshVertexPtr = e3meshVertexArray_FirstItemConst(&meshPtr->vertexArrayOrList.array);

	// Initialize the vertices
	for (vertexIdx = 0; vertexIdx < polyhedronData.numVertices; ++vertexIdx)
	{
		polyhedronData.vertices[vertexIdx].point = firstMeshVertexPtr[vertexIdx].point;
		polyhedronData.vertices[vertexIdx].attributeSet = firstMeshVertexPtr[vertexIdx].attributeSet;
	}
	
	// Allocate memory for polyhedron edges
	polyhedronData.numEdges = 0;
	polyhedronData.edges = NULL;

	// Use array of triangles in tessellator
	if (e3polyhedronTriangleArrayOrList_UseArray(&tessellator.polyhedronTriangleArrayOrList, NULL, NULL, NULL) == kQ3Failure)
		goto cleanup_4;
	
	// Set polyhedron triangles
	polyhedronData.numTriangles = e3polyhedronTriangleArray_Length(&tessellator.polyhedronTriangleArrayOrList.array);
	polyhedronData.triangles = e3polyhedronTriangleArray_FirstItem(&tessellator.polyhedronTriangleArrayOrList.array);

	// Set polyhedron attribute set
	polyhedronData.polyhedronAttributeSet = meshPtr->attributeSet;
	
	if(polyhedronData.numTriangles == 0){
		//something has failed with tesselation probably a non planar face, lets try to create a fan
		facePtr = e3meshFaceArrayOrList_FirstItemConst(&meshPtr->faceArrayOrList);
		if(facePtr)
			{
			polyhedronData.numTriangles = polyhedronData.numVertices - 2;
			polyhedronData.triangles = (TQ3PolyhedronTriangleData *) Q3Memory_AllocateClear(polyhedronData.numTriangles * sizeof(TQ3PolyhedronTriangleData));
			for (triangleIndex = 0; triangleIndex < polyhedronData.numTriangles; ++triangleIndex)
				{
				polyhedronData.triangles[triangleIndex].vertexIndices[0] = 0;
				polyhedronData.triangles[triangleIndex].vertexIndices[1] = triangleIndex + 1;
				polyhedronData.triangles[triangleIndex].vertexIndices[2] = triangleIndex + 2;
				polyhedronData.triangles[triangleIndex].triangleAttributeSet = facePtr->attributeSet;
				}
			polyhedron = Q3Polyhedron_New(&polyhedronData);
			Q3Memory_Free(&polyhedronData.triangles);
			}
		
	}
	else
	// Create the polyhedron and clean up
		polyhedron = Q3Polyhedron_New(&polyhedronData);

cleanup_4:

	Q3Memory_Free(&polyhedronData.vertices);
cleanup_3:

	e3meshTessellator_Destroy(&tessellator);
cleanup_2:

cleanup_1:
	
	return(polyhedron);
}





//=============================================================================
//      e3geom_mesh_cache_new : Mesh cache new method.
//-----------------------------------------------------------------------------
static
TQ3Object
e3geom_mesh_cache_new(
	TQ3ViewObject view,
	TQ3GeometryObject meshObject,
	const TE3MeshData* meshPtr)
{
#pragma unused(meshObject)
#pragma unused(view)
	const TE3MeshFaceData* 			facePtr;
	TQ3AttributeSet 				attr;
	

	if(meshPtr->numCorners || e3mesh_NumFaces(meshPtr) == 1)
		return e3geom_mesh_cache_new_as_polys(meshPtr);
	else
		{
		facePtr = e3meshFaceArrayOrList_FirstItemConst(&meshPtr->faceArrayOrList);
		attr = facePtr->attributeSet;
		facePtr = e3meshFaceArrayOrList_NextItemConst(&meshPtr->faceArrayOrList, facePtr);
		while (facePtr != NULL)
			{ // bail out at first different attributeSet
			if(facePtr->attributeSet != attr)
				return e3geom_mesh_cache_new_as_polys(meshPtr);
			facePtr = e3meshFaceArrayOrList_NextItemConst(&meshPtr->faceArrayOrList, facePtr);
			}
		return e3geom_mesh_cache_new_as_polyhedron(meshPtr);
		}
}





//=============================================================================
//      e3geom_mesh_pick : Mesh picking method.
//-----------------------------------------------------------------------------
static
TQ3Status
e3geom_mesh_pick(
	TQ3ViewObject theView,
	TQ3ObjectType objectType,
	TQ3Object theObject,
	const void *objectData)
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
e3geom_mesh_bounds(
	TQ3ViewObject theView,
	TQ3ObjectType objectType,
	TQ3Object theObject,
	const void *objectData)
{
#pragma unused(objectType)
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(theObject, kQ3GeometryTypeMesh);

	// Use array of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3mesh_UseVertexArray(E3_CONST_CAST(TE3MeshData*, meshPtr)) == kQ3Failure)
		goto failure;

	// Update the bounds
	E3View_UpdateBounds(theView, e3mesh_NumVertices(meshPtr), sizeof(TE3MeshVertexData), &e3meshVertexArray_FirstItemConst(&meshPtr->vertexArrayOrList.array)->point);

	return(kQ3Success);

failure:
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_mesh_get_attribute : Mesh get attribute set pointer.
//-----------------------------------------------------------------------------
static
TQ3AttributeSet* 
e3geom_mesh_get_attribute(
	TQ3GeometryObject theObject)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(theObject, kQ3GeometryTypeMesh);


	// Return the address of the geometry attribute set
	return(&meshPtr->attributeSet);
}





//=============================================================================
//      e3geom_mesh_metahandler : Mesh metahandler.
//-----------------------------------------------------------------------------
static
TQ3XFunctionPointer
e3geom_mesh_metahandler(
	TQ3XMethodType methodType)
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
//      E3Mesh_SetData : Set the data for a mesh object.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetData(TQ3GeometryObject meshObject, const TQ3MeshData *meshData)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);

	if (e3mesh_SetExtData(meshPtr, meshData) == kQ3Failure)
		goto failure;

	Q3Shared_Edited(meshObject);

	return(kQ3Success);

failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetData : Get the data for a mesh object.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetData(TQ3GeometryObject meshObject, TQ3MeshData *meshData)
{
	const TE3MeshData* meshPtr = (const TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);

	if (e3mesh_GetExtData(meshPtr, meshData) == kQ3Failure)
		goto failure;

	return(kQ3Success);

failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_EmptyData : Empty the data for a mesh object.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_EmptyData(TQ3MeshData *meshData)
{
	if (e3meshExtData_Empty(meshData) == kQ3Failure)
		goto failure;

	return(kQ3Success);

failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_DelayUpdates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_DelayUpdates(
	TQ3GeometryObject meshObject)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);



	// To be implemented...
	return(kQ3Success);
}





//=============================================================================
//      E3Mesh_ResumeUpdates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_ResumeUpdates(
	TQ3GeometryObject meshObject)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);



	// To be implemented...
	return(kQ3Success);
}





//=============================================================================
//      E3Mesh_FaceNew : Add new face created from vertices and attribute set.
//-----------------------------------------------------------------------------
//		Note :	If unable to use list of faces, if unable to insert face item,
//				or if unable to create face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_FaceNew(
	TQ3GeometryObject meshObject,
	TQ3Uns32 numVertices,
	const TE3MeshVertexExtRef* vertexExtRefs,
	TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshFaceData* facePtr;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3mesh_UseFaceList(meshPtr) == kQ3Failure)
		goto failure_1;

	// Push back new uninitialized face
	if ((facePtr = e3meshFaceList_PushBackItem(&meshPtr->faceArrayOrList.list, NULL)) == NULL)
		goto failure_2;

	// Create face, with reference
	if (e3meshFace_CreateFromVertexExtRefs(facePtr, meshPtr, kQ3True, numVertices, vertexExtRefs, attributeSet) == kQ3Failure)
		goto failure_3;

	Q3Shared_Edited(meshObject);

	return(e3meshFace_ExtRefInMesh(facePtr, meshPtr));
	
	// Dead code to reverse e3meshFace_Create
failure_3:

	e3meshFaceList_EraseItem(&meshPtr->faceArrayOrList.list, NULL, facePtr);
failure_2:

failure_1:

	return(NULL);
}





//=============================================================================
//      E3Mesh_FaceDelete : Delete face from mesh.
//-----------------------------------------------------------------------------
//		Note : If unable to use list of faces, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_FaceDelete(
	TQ3GeometryObject meshObject,
	TE3MeshFaceExtRef faceExtRef)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshFaceData* facePtr;

	// Check face; if face already deleted, return kQ3Success
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto success;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3mesh_UseFaceList(meshPtr) == kQ3Failure)
		goto failure;

	// Recheck face (in case relocated)
	facePtr = e3meshFaceExtRef_Face(faceExtRef);

	// Erase face
	e3meshFaceList_EraseItem(&meshPtr->faceArrayOrList.list, &e3meshFace_Destroy,
		facePtr);

	Q3Shared_Edited(meshObject);

success:

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FaceToContour :	Append face's contours to container face's, and
//								delete face.
//-----------------------------------------------------------------------------
//		Note :	If container face deleted, face deleted, unable to relocate
//				faces, or unable to relocate contours, return NULL.
//-----------------------------------------------------------------------------
TE3MeshContourExtRef
E3Mesh_FaceToContour(
	TQ3GeometryObject meshObject,
	TE3MeshFaceExtRef containerFaceExtRef,
	TE3MeshFaceExtRef faceExtRef)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshFaceData* containerFacePtr;
	TE3MeshFaceData* facePtr;
	TE3MeshContourData* contourPtr;
	TE3MeshContourExtRef contourExtRef;

	// Check container face
	if ((containerFacePtr = e3meshFaceExtRef_Face(containerFaceExtRef)) == NULL)
		goto failure;

	// Check face
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3mesh_UseFaceList(meshPtr) == kQ3Failure)
		goto failure;

	// Recheck container face (in case relocated)
	containerFacePtr = e3meshFaceExtRef_Face(containerFaceExtRef);

	// Recheck face (in case relocated)
	facePtr = e3meshFaceExtRef_Face(faceExtRef);

	// Use list of contours in container face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshFace_UseContourList(containerFacePtr) == kQ3Failure)
		goto failure;

	// Use list of contours in face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshFace_UseContourList(facePtr) == kQ3Failure)
		goto failure;

	// Get first contour in face
	if ((contourPtr = e3meshContourList_FirstItem(&facePtr->contourArrayOrList.list)) == NULL)
		goto failure;
	if ((contourExtRef = e3meshContour_ExtRefInMesh(contourPtr, meshPtr)) == NULL)
		goto failure;

	// For each contour in face, reset container face
	for (contourPtr = e3meshContourList_FirstItem(&facePtr->contourArrayOrList.list);
		contourPtr != NULL;
		contourPtr = e3meshContourList_NextItem(&facePtr->contourArrayOrList.list, contourPtr))
	{
		// Reset container face
		e3meshContour_SetContainerFace(contourPtr, containerFacePtr);
	}

	// Splice contours from face into container face
	e3meshContourList_SpliceBackList(&containerFacePtr->contourArrayOrList.list, &facePtr->contourArrayOrList.list);

	// Erase face from mesh
	e3meshFaceList_EraseItem(&meshPtr->faceArrayOrList.list, &e3meshFace_Destroy,
		facePtr);

	Q3Shared_Edited(meshObject);

	// Return contour
	return(contourExtRef);
	
failure:

	return(NULL);
}





//=============================================================================
//      E3Mesh_ContourToFace :	Add new face with contour.
//-----------------------------------------------------------------------------
//		Note :	If contour deleted, unable to relocate faces, unable to
//				insert face item, or unable to relocate contours, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If the specified contour is the only contour in its container
//				face, then this function merely returns that container face.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_ContourToFace(
	TQ3GeometryObject meshObject,
	TE3MeshContourExtRef contourExtRef)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshContourData* contourPtr;
	TE3MeshFaceData* containerFacePtr;
	TE3MeshFaceData* facePtr;

	// Check contour
	if ((contourPtr = e3meshContourExtRef_Contour(contourExtRef)) == NULL)
		goto failure_1;
		
	// Get and check container face
	containerFacePtr = e3meshContour_ContainerFace(contourPtr);
	if (e3meshFace_NumContours(containerFacePtr) == 1)
		goto success;
	
	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3mesh_UseFaceList(meshPtr) == kQ3Failure)
		goto failure_2;
		
	// Recheck container face (in case relocated)
	containerFacePtr = e3meshContour_ContainerFace(contourPtr);

	// Push back new uninitialized face
	if ((facePtr = e3meshFaceList_PushBackItem(&meshPtr->faceArrayOrList.list, NULL)) == NULL)
		goto failure_3;

	// Create face with empty list of contours, with reference
	if (e3meshFace_CreateEmptyListOfContours(facePtr, meshPtr, kQ3True, NULL) == kQ3Failure)
		goto failure_4;

	// Use list of contours in container face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshFace_UseContourList(containerFacePtr) == kQ3Failure)
		goto failure_5;

	// Use list of contours in face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshFace_UseContourList(facePtr) == kQ3Failure)
		goto failure_6;

	// Splice contour from container face into new face
	e3meshContourList_SpliceBackList(&containerFacePtr->contourArrayOrList.list, &facePtr->contourArrayOrList.list);

	Q3Shared_Edited(meshObject);

success:

	// Return face
	return(e3meshFace_ExtRefInMesh(facePtr, meshPtr));
	
	// Dead code to reverse e3meshContourArrayOrList_UseList
failure_6:
	
failure_5:
	
	e3meshFace_Destroy(facePtr);
failure_4:

	e3meshFaceList_EraseItem(&meshPtr->faceArrayOrList.list, NULL, facePtr);
failure_3:

failure_2:

failure_1:

	return(NULL);
}





//=============================================================================
//      E3Mesh_VertexNew : Add new vertex created from coordinates and attribute set.
//-----------------------------------------------------------------------------
//		Note :	If unable to use list of vertices, if unable to insert vertex
//				item, or if unable to create vertex, return NULL.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_VertexNew(
	TQ3GeometryObject meshObject,
	const TQ3Vertex3D* vertexExtDataPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexPtr;

	// Use list of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3mesh_UseVertexList(meshPtr) == kQ3Failure)
		goto failure_1;

	// Push back new uninitialized vertex
	if ((vertexPtr = e3meshVertexList_PushBackItem(&meshPtr->vertexArrayOrList.list, NULL)) == NULL)
		goto failure_2;

	// Create vertex, with reference
	if (e3meshVertex_CreateEmptyArrayOfCorners(vertexPtr, meshPtr, kQ3True, vertexExtDataPtr) == kQ3Failure)
		goto failure_3;

	Q3Shared_Edited(meshObject);

	return(e3meshVertex_ExtRefInMesh(vertexPtr, meshPtr));
	
	// Dead code to reverse e3meshVertex_CreateEmptyArrayOfCorners
failure_3:

	e3meshVertexList_EraseItem(&meshPtr->vertexArrayOrList.list, NULL, vertexPtr);
failure_2:

failure_1:

	return(NULL);
}





//=============================================================================
//      E3Mesh_VertexDelete : Delete vertex from mesh.
//-----------------------------------------------------------------------------
//		Note : If unable to use list of vertices, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_VertexDelete(
	TQ3GeometryObject meshObject,
	TE3MeshVertexExtRef vertexExtRef)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexPtr;
	TE3MeshFaceData* facePtr;

	// Check vertex; if vertex already deleted, return kQ3Success
	if ((vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef)) == NULL)
		goto success;

	// Use list of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3mesh_UseVertexList(meshPtr) == kQ3Failure)
		goto failure;

	// Recheck vertex (in case relocated)
	vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef);

	// Get first face in mesh
	facePtr = e3meshFaceArrayOrList_FirstItem(&meshPtr->faceArrayOrList);

	// Delete each face having vertex
	while (facePtr != NULL)
	{
		TE3MeshFaceData* markedMeshFacePtr = NULL;

		// Check if face has vertex
		if (e3meshFace_HasVertex(facePtr, vertexPtr))
		{
			TE3MeshFaceExtRef faceExtRef = NULL;

			// Save face
			if ((faceExtRef = e3meshFace_ExtRefInMesh(facePtr, meshPtr)) == NULL)
				goto failure;

			// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
			if (e3mesh_UseFaceList(meshPtr) == kQ3Failure)
				goto failure;

			// Restore face (in case relocated)
			if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
				goto failure;

			// Mark face for erasure
			markedMeshFacePtr = facePtr;
		}

		// Get next face in mesh
		facePtr = e3meshFaceArrayOrList_NextItem(&meshPtr->faceArrayOrList, facePtr);

		// If face marked for erasure, erase face
		if (markedMeshFacePtr)
			e3meshFaceList_EraseItem(&meshPtr->faceArrayOrList.list, &e3meshFace_Destroy,
				markedMeshFacePtr);
	}

	// Erase vertex from mesh
	e3meshVertexList_EraseItem(&meshPtr->vertexArrayOrList.list, &e3meshVertex_Destroy,
		vertexPtr);

	Q3Shared_Edited(meshObject);

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
E3Mesh_GetOrientable(
	TQ3GeometryObject meshObject,
	TQ3Boolean* orientablePtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


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
E3Mesh_GetNumComponents(
	TQ3GeometryObject meshObject,
	TQ3Uns32* numComponentsPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstMeshComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshComponentExtRef
E3Mesh_FirstMeshComponent(
	TQ3GeometryObject meshObject,
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_NextMeshComponent(
	TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetNumFaces : Get number of faces in mesh.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumFaces(
	TQ3GeometryObject meshObject,
	TQ3Uns32* numFacesPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);

	// Get number of faces in mesh
	*numFacesPtr = e3mesh_NumFaces(meshPtr);

	return(kQ3Success);
}





//=============================================================================
//      E3Mesh_FirstMeshFace : Get first face in mesh.
//-----------------------------------------------------------------------------
//		Note :	If no first face or unable to create external reference to
//				face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_FirstMeshFace(
	TQ3GeometryObject meshObject,
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshFaceData* facePtr;
	TE3MeshFaceExtRef faceExtRef;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshPtr, "mefa");

	// Get and save first face in mesh
	if ((facePtr = e3meshFaceArrayOrList_FirstItem(&meshPtr->faceArrayOrList)) == NULL)
		goto failure;
	if ((faceExtRef = e3meshFace_ExtRefInMesh(facePtr, meshPtr)) == NULL)
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
//		Note :	If iterator ended, current face deleted, no next face or
//				unable to create external reference to face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_NextMeshFace(
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr = iteratorPtr->var4.field1;
	TE3MeshFaceExtRef faceExtRef;
	TE3MeshFaceData* facePtr;

	// Restore and check current face in mesh
	if ((faceExtRef = (TE3MeshFaceExtRef) iteratorPtr->var1) == NULL)
		goto failure;
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	// Get and save next face in mesh
	if ((facePtr = e3meshFaceArrayOrList_NextItem(&meshPtr->faceArrayOrList, facePtr)) == NULL)
		goto failure;
	if ((faceExtRef = e3meshFace_ExtRefInMesh(facePtr, meshPtr)) == NULL)
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
E3Mesh_GetNumEdges(
	TQ3GeometryObject meshObject,
	TQ3Uns32* numEdgesPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstMeshEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_FirstMeshEdge(
	TQ3GeometryObject meshObject,
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_NextMeshEdge(
TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetNumVertices : Get number of vertices in mesh.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumVertices(
	TQ3GeometryObject meshObject,
	TQ3Uns32* numVerticesPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);

	// Get number of vertices in mesh
	*numVerticesPtr = e3mesh_NumVertices(meshPtr);

	return(kQ3Success);
}





//=============================================================================
//      E3Mesh_FirstMeshVertex : Get first vertex in mesh.
//-----------------------------------------------------------------------------
//		Note :	If no first vertex or unable to create external reference
//				to vertex, return NULL.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_FirstMeshVertex(
	TQ3GeometryObject meshObject,
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexPtr;
	TE3MeshVertexExtRef vertexExtRef;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshPtr, "meve");

	// Get and save first vertex in mesh
	if ((vertexPtr = e3meshVertexArrayOrList_FirstItem(&meshPtr->vertexArrayOrList)) == NULL)
		goto failure;
	if ((vertexExtRef = e3meshVertex_ExtRefInMesh(vertexPtr, meshPtr)) == NULL)
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
//		Note :	If iterator ended, current vertex deleted, no next vertex or
//				unable to create external reference to vertex, return NULL.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_NextMeshVertex(
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr = iteratorPtr->var4.field1;
	TE3MeshVertexExtRef vertexExtRef;
	TE3MeshVertexData* vertexPtr;

	// Restore and check current vertex in mesh
	if ((vertexExtRef = (TE3MeshVertexExtRef) iteratorPtr->var1) == NULL)
		goto failure;
	if ((vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef)) == NULL)
		goto failure;

	// Get and save next vertex in mesh
	if ((vertexPtr = e3meshVertexArrayOrList_NextItem(&meshPtr->vertexArrayOrList, vertexPtr)) == NULL)
		goto failure;
	if ((vertexExtRef = e3meshVertex_ExtRefInMesh(vertexPtr, meshPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = vertexExtRef;

	// Return next vertex in mesh
	return(vertexExtRef);
	
failure:

	iteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetNumCorners : Get number of (distinct, defined) corners in mesh.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumCorners(
	TQ3GeometryObject meshObject,
	TQ3Uns32* numCornersPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);

	// Get number of corners in mesh
	*numCornersPtr = e3mesh_NumCorners(meshPtr);

	return(kQ3Success);
}





//=============================================================================
//      E3Mesh_GetComponentOrientable : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetComponentOrientable(
	TQ3GeometryObject meshObject,
	TE3MeshComponentExtRef componentExtRef,
	TQ3Boolean* orientablePtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetComponentBoundingBox : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetComponentBoundingBox(
	TQ3GeometryObject meshObject,
	TE3MeshComponentExtRef componentExtRef,
	TQ3BoundingBox* boundingBoxPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


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
E3Mesh_GetComponentNumEdges(
	TQ3GeometryObject meshObject,
	TE3MeshComponentExtRef componentExtRef,
	TQ3Uns32* numEdgesPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstComponentEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeExtRef
E3Mesh_FirstComponentEdge(
	TE3MeshComponentExtRef componentExtRef,
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_NextComponentEdge(
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_GetComponentNumVertices(
	TQ3GeometryObject meshObject,
	TE3MeshComponentExtRef componentExtRef,
	TQ3Uns32* numVerticesPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstComponentVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_FirstComponentVertex(
	TE3MeshComponentExtRef componentExtRef,
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_NextComponentVertex(
	TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetFaceIndex : Get index (0..N-1) of face in mesh.
//-----------------------------------------------------------------------------
//		Note :	If unable to use array of faces or if face deleted, return
//				kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetFaceIndex(
	TQ3GeometryObject meshObject,
	TE3MeshFaceExtRef faceExtRef,
	TQ3Uns32* indexPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshFaceData* facePtr;

	// Check face
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	// Use array of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3mesh_UseFaceArray(meshPtr) == kQ3Failure)
		goto failure;

	// Recheck face (in case relocated)
	facePtr = e3meshFaceExtRef_Face(faceExtRef);

	// Get index of face
	*indexPtr = e3meshFaceArray_ItemIndex(&meshPtr->faceArrayOrList.array, facePtr);

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
E3Mesh_GetFacePlaneEquation(
	TQ3GeometryObject meshObject,
	TE3MeshFaceExtRef faceExtRef,
	TQ3PlaneEquation* planeEquationPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetFaceAttributeSet : Get attribute set for face.
//-----------------------------------------------------------------------------
//		Note : If face deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetFaceAttributeSet(
	TQ3GeometryObject meshObject,
	TE3MeshFaceExtRef faceExtRef,
	TQ3AttributeSet* attributeSetPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshFaceData* facePtr;

	// Check face
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	// Get attribute set
	E3Shared_Acquire(attributeSetPtr, facePtr->attributeSet);

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
E3Mesh_SetFaceAttributeSet(
	TQ3GeometryObject meshObject,
	TE3MeshFaceExtRef faceExtRef,
	TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshFaceData* facePtr;

	// Check face
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	// Set attribute set
	E3Shared_Replace(&facePtr->attributeSet, attributeSet);

	Q3Shared_Edited(meshObject);

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
E3Mesh_GetFaceComponent(
	TQ3GeometryObject meshObject,
	TE3MeshFaceExtRef faceExtRef,
	TE3MeshComponentExtRef* componentExtRefPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstFaceFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_FirstFaceFace(
	TE3MeshFaceExtRef faceExtRef,
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_NextFaceFace(
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_GetFaceNumContours(
	TQ3GeometryObject meshObject,
	TE3MeshFaceExtRef faceExtRef,
	TQ3Uns32* numContoursPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshFaceData* facePtr;

	// Check face
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	// Get number of contours in face
	*numContoursPtr = e3meshFace_NumContours(facePtr);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstFaceContour : Get first contour in face.
//-----------------------------------------------------------------------------
//		Note :	If face deleted, no first contour or unable to create
//				external reference to contour, return NULL.
//-----------------------------------------------------------------------------
TE3MeshContourExtRef
E3Mesh_FirstFaceContour(
	TE3MeshFaceExtRef faceExtRef,
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr;
	TE3MeshFaceData* facePtr;
	TE3MeshContourData* contourPtr;
	TE3MeshContourExtRef contourExtRef;

	// Get mesh for face
	if ((meshPtr = e3meshFaceExtRef_Mesh(faceExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshPtr, "fact");

	// Check and save face
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;
	iteratorPtr->var2 = faceExtRef;

	// Get and save first contour in face
	if ((contourPtr = e3meshContourArrayOrList_FirstItem(&facePtr->contourArrayOrList)) == NULL)
		goto failure;
	if ((contourExtRef = e3meshContour_ExtRefInMesh(contourPtr, meshPtr)) == NULL)
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
//		Note :	If iterator ended, face deleted, current contour deleted,
//				no next contour or unable to create external reference to
//				contour, return NULL.
//-----------------------------------------------------------------------------
TE3MeshContourExtRef
E3Mesh_NextFaceContour(
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr = iteratorPtr->var4.field1;
	TE3MeshFaceExtRef faceExtRef;
	TE3MeshFaceData* facePtr;
	TE3MeshContourExtRef contourExtRef;
	TE3MeshContourData* contourPtr;

	// Restore and check face
	if ((faceExtRef = (TE3MeshFaceExtRef) iteratorPtr->var2) == NULL)
		goto failure;
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	// Restore and check current contour in face
	if ((contourExtRef = (TE3MeshContourExtRef) iteratorPtr->var1) == NULL)
		goto failure;
	if ((contourPtr = e3meshContourExtRef_Contour(contourExtRef)) == NULL)
		goto failure;

	// Get and save next contour in face
	if ((contourPtr = e3meshContourArrayOrList_NextItem(&facePtr->contourArrayOrList, contourPtr)) == NULL)
		goto failure;
	if ((contourExtRef = e3meshContour_ExtRefInMesh(contourPtr, meshPtr)) == NULL)
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
E3Mesh_FirstFaceEdge(
	TE3MeshFaceExtRef faceExtRef,
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_GetFaceNumVertices(
	TQ3GeometryObject meshObject,
	TE3MeshFaceExtRef faceExtRef,
	TQ3Uns32* numVerticesPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshFaceData* facePtr;

	// Check face
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	*numVerticesPtr = e3meshFace_NumVertices(facePtr);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstFaceVertex : Get first vertex in face.
//-----------------------------------------------------------------------------
//		Note :	If face deleted, no first vertex or unable to create
//				external reference to vertex, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_FirstFaceVertex(
	TE3MeshFaceExtRef faceExtRef,
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr;
	TE3MeshFaceData* facePtr;
	TE3MeshContourData* contourPtr;
	TE3MeshContourExtRef contourExtRef;
	TE3MeshVertexData** vertexHdl;
	TE3MeshVertexExtRef vertexExtRef;

	// Get mesh for face
	if ((meshPtr = e3meshFaceExtRef_Mesh(faceExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshPtr, "fave");

	// Check and save face
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;
	iteratorPtr->var3 = faceExtRef;

	// Get first contour in face
	if ((contourPtr = e3meshContourArrayOrList_FirstItem(&facePtr->contourArrayOrList)) == NULL)
		goto failure;

	// Get first vertex in face
	for (;;)
	{
		// If first vertex in current contour exists, break
		if ((vertexHdl = e3meshVertexPtrArray_FirstItem(&contourPtr->vertexPtrArray)) != NULL)
			break;

		// Otherwise, get next contour in face
		if ((contourPtr = e3meshContourArrayOrList_NextItem(&facePtr->contourArrayOrList, contourPtr)) == NULL)
			goto failure;
	}

	// Save current contour in face
	if ((contourExtRef = e3meshContour_ExtRefInMesh(contourPtr, meshPtr)) == NULL)
		goto failure;
	iteratorPtr->var2 = contourExtRef;

	// Save first vertex in face
	if ((vertexExtRef = e3meshVertex_ExtRefInMesh(*vertexHdl, meshPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = vertexHdl;

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
//		Note :	If iterator ended, face deleted, current vertex deleted,
//				no next vertex or unable to create external reference to
//				vertex, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_NextFaceVertex(
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr = iteratorPtr->var4.field1;
	TE3MeshFaceExtRef faceExtRef;
	TE3MeshFaceData* facePtr;
	TE3MeshContourExtRef contourExtRef;
	TE3MeshContourData* contourPtr;
	TE3MeshVertexData** vertexHdl;
	TE3MeshVertexExtRef vertexExtRef;

	// Restore and check face
	if ((faceExtRef = (TE3MeshFaceExtRef) iteratorPtr->var3) == NULL)
		goto failure;
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	// Restore and check current contour
	if ((contourExtRef = (TE3MeshContourExtRef) iteratorPtr->var2) == NULL)
		goto failure;
	if ((contourPtr = e3meshContourExtRef_Contour(contourExtRef)) == NULL)
		goto failure;

	// ??? check that contour still belongs to face ???

	// Restore and check current vertex
	if ((vertexHdl = (TE3MeshVertexData**) iteratorPtr->var1) == NULL)
		goto failure;

	// If next vertex in current contour exists, break
	if ((vertexHdl = e3meshVertexPtrArray_NextItem(&contourPtr->vertexPtrArray, vertexHdl)) != NULL)
		;
	// Otherwise, get next vertex in face
	else
	{
		for (;;)
		{
			// Get next contour in face
			if ((contourPtr = e3meshContourArrayOrList_NextItem(&facePtr->contourArrayOrList, contourPtr)) == NULL)
				goto failure;
				
			// If first vertex in current contour exists, break
			if ((vertexHdl = e3meshVertexPtrArray_FirstItem(&contourPtr->vertexPtrArray)) != NULL)
				break;
		}
	}

	// Save current contour in face
	if ((contourExtRef = e3meshContour_ExtRefInMesh(contourPtr, meshPtr)) == NULL)
		goto failure;
	iteratorPtr->var2 = contourExtRef;

	// Save next vertex in face
	if ((vertexExtRef = e3meshVertex_ExtRefInMesh(*vertexHdl, meshPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = vertexHdl;

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
//		Note :	If contour deleted or If unable to get external reference
//				(out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetContourFace(
	TQ3GeometryObject meshObject,
	TE3MeshContourExtRef contourExtRef,
	TE3MeshFaceExtRef* containerFaceExtRefPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshContourData* contourPtr;
	TE3MeshFaceData* containerFacePtr;
	
	// Check contour
	if ((contourPtr = e3meshContourExtRef_Contour(contourExtRef)) == NULL)
		goto failure;
	
	// Get external reference to container face
	containerFacePtr = e3meshContour_ContainerFace(contourPtr);
	if ((*containerFaceExtRefPtr = e3meshFace_ExtRefInMesh(containerFacePtr, meshPtr)) == NULL)
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
E3Mesh_FirstContourFace(
	TE3MeshContourExtRef contourExtRef,
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_NextContourFace(
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_FirstContourEdge(
	TE3MeshContourExtRef contourExtRef,
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_NextContourEdge(
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_GetContourNumVertices(
	TQ3GeometryObject meshObject,
	TE3MeshContourExtRef contourExtRef,
	TQ3Uns32* numVerticesPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshContourData* contourPtr;

	// Check contour
	if ((contourPtr = e3meshContourExtRef_Contour(contourExtRef)) == NULL)
		goto failure;

	// Get number of vertices in contour
	*numVerticesPtr = e3meshContour_NumVertices(contourPtr);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstContourVertex : Get first vertex in contour.
//-----------------------------------------------------------------------------
//		Note :	If contour deleted, no first vertex or unable to create
//				external reference to vertex, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence is
//				iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_FirstContourVertex(
	TE3MeshContourExtRef contourExtRef,
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr;
	TE3MeshContourData* contourPtr;
	TE3MeshVertexData** vertexHdl;
	TE3MeshVertexExtRef vertexExtRef;

	// Get mesh for contour
	if ((meshPtr = e3meshContourExtRef_Mesh(contourExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshPtr, "ctve");

	// Check and save contour
	if ((contourPtr = e3meshContourExtRef_Contour(contourExtRef)) == NULL)
		goto failure;
	iteratorPtr->var2 = contourExtRef;

	// Get and save first vertex in contour
	if ((vertexHdl = e3meshVertexPtrArray_FirstItem(&contourPtr->vertexPtrArray)) == NULL)
		goto failure;
	if ((vertexExtRef = e3meshVertex_ExtRefInMesh(*vertexHdl, meshPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = vertexHdl;

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
//		Note :	If iterator ended, contour deleted, current vertex deleted,
//				no next vertex or unable to create external reference to
//				vertex, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence is
//				iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexExtRef
E3Mesh_NextContourVertex(
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr = iteratorPtr->var4.field1;
	TE3MeshContourExtRef contourExtRef;
	TE3MeshContourData* contourPtr;
	TE3MeshVertexData** vertexHdl;
	TE3MeshVertexExtRef vertexExtRef;

	// Restore and check contour
	if ((contourExtRef = (TE3MeshContourExtRef) iteratorPtr->var2) == NULL)
		goto failure;
	if ((contourPtr = e3meshContourExtRef_Contour(contourExtRef)) == NULL)
		goto failure;

	// Restore and check current vertex in contour
	if ((vertexHdl = (TE3MeshVertexData**) iteratorPtr->var1) == NULL)
		goto failure;

	// Get and save next vertex in contour
	if ((vertexHdl = e3meshVertexPtrArray_NextItem(&contourPtr->vertexPtrArray, vertexHdl)) == NULL)
		goto failure;
	if ((vertexExtRef = e3meshVertex_ExtRefInMesh(*vertexHdl, meshPtr)) == NULL)
		goto failure;
	iteratorPtr->var1 = vertexHdl;

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
E3Mesh_GetEdgeOnBoundary(
	TQ3GeometryObject meshObject,
	TE3MeshEdgeExtRef edgeExtRef,
	TQ3Boolean* onBoundaryPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetEdgeAttributeSet(
	TQ3GeometryObject meshObject,
	TE3MeshEdgeExtRef edgeExtRef,
	TQ3AttributeSet* attributeSetPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetEdgeAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetEdgeAttributeSet(
	TQ3GeometryObject meshObject,
	TE3MeshEdgeExtRef edgeExtRef,
	TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	Q3Shared_Edited(meshObject);

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetEdgeComponent(
	TQ3GeometryObject meshObject,
	TE3MeshEdgeExtRef edgeExtRef,
	TE3MeshComponentExtRef* componentExtRefPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeFaces : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetEdgeFaces(
	TQ3GeometryObject meshObject,
	TE3MeshEdgeExtRef edgeExtRef,
	TE3MeshFaceExtRef* faceExtRefPtr1,
	TE3MeshFaceExtRef* faceExtRefPtr2)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeVertices : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetEdgeVertices(
	TQ3GeometryObject meshObject,
	TE3MeshEdgeExtRef edgeExtRef,
	TE3MeshVertexExtRef* vertexExtRefPtr1,
	TE3MeshVertexExtRef* vertexExtRefPtr2)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexIndex : Get index (0..N-1) of vertex in mesh.
//-----------------------------------------------------------------------------
//		Note :	If unable to use array of vertices or if vertex deleted, return
//				kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetVertexIndex(
	TQ3GeometryObject meshObject,
	TE3MeshVertexExtRef vertexExtRef,
	TQ3Uns32* indexPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexPtr;

	// Check vertex
	if ((vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef)) == NULL)
		goto failure;

	// Use array of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3mesh_UseVertexArray(meshPtr) == kQ3Failure)
		goto failure;

	// Recheck vertex (in case relocated)
	vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef);

	// Get index of vertex
	*indexPtr = e3meshVertexArray_ItemIndex(&meshPtr->vertexArrayOrList.array, vertexPtr);

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
E3Mesh_GetVertexOnBoundary(
	TQ3GeometryObject meshObject,
	TE3MeshVertexExtRef vertexExtRef,
	TQ3Boolean* onBoundaryPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexCoordinates : Get vertex coordinates.
//-----------------------------------------------------------------------------
//		Note : If vertex deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetVertexCoordinates(
	TQ3GeometryObject meshObject,
	TE3MeshVertexExtRef vertexExtRef,
	TQ3Point3D* coordinatesPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexPtr;

	// Check vertex
	if ((vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef)) == NULL)
		goto failure;

	// Get vertex coordinates
	*coordinatesPtr = vertexPtr->point;

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
E3Mesh_SetVertexCoordinates(
	TQ3GeometryObject meshObject,
	TE3MeshVertexExtRef vertexExtRef,
	const TQ3Point3D* coordinates)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexPtr;

	// Check vertex
	if ((vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef)) == NULL)
		goto failure;

	// Set vertex coordinates
	vertexPtr->point = *coordinates;

	Q3Shared_Edited(meshObject);

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
E3Mesh_GetVertexAttributeSet(
	TQ3GeometryObject meshObject,
	TE3MeshVertexExtRef vertexExtRef,
	TQ3AttributeSet* attributeSetPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexPtr;

	// Check vertex
	if ((vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef)) == NULL)
		goto failure;

	// Get vertex attribute set
	E3Shared_Acquire(attributeSetPtr, vertexPtr->attributeSet);

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
E3Mesh_SetVertexAttributeSet(
	TQ3GeometryObject meshObject,
	TE3MeshVertexExtRef vertexExtRef,
	TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexPtr;

	// Check vertex
	if ((vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef)) == NULL)
		goto failure;

	// Set vertex attribute set
	E3Shared_Replace(&vertexPtr->attributeSet, attributeSet);

	Q3Shared_Edited(meshObject);

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
E3Mesh_GetVertexComponent(
	TQ3GeometryObject meshObject,
	TE3MeshVertexExtRef vertexExtRef,
	TE3MeshComponentExtRef* componentExtRefPtr)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstVertexFace : Get first face having vertex.
//-----------------------------------------------------------------------------
//		Note :	If vertex deleted, no first face or unable to create
//				external reference to face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_FirstVertexFace(
	TE3MeshVertexExtRef vertexExtRef,
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr;
	TE3MeshVertexData* vertexPtr;
	TE3MeshFaceData* facePtr;
	TE3MeshFaceExtRef faceExtRef;

	// Get mesh for vertex
	if ((meshPtr = e3meshVertexExtRef_Mesh(vertexExtRef)) == NULL)
		goto failure;
	
	// Initialize iterator
	e3meshIterator_Initialize(iteratorPtr, meshPtr, "vefa");

	// Check and save vertex
	if ((vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef)) == NULL)
		goto failure;
	iteratorPtr->var2 = vertexExtRef;

	// Get first face in mesh
	if ((facePtr = e3meshFaceArrayOrList_FirstItem(&meshPtr->faceArrayOrList)) == NULL)
		goto failure;

	// Get first face having vertex
	for (;;)
	{
		// If face has vertex, break
		if (e3meshFace_HasVertex(facePtr, vertexPtr))
			break;

		// Otherwise, get next face in mesh
		if ((facePtr = e3meshFaceArrayOrList_NextItem(&meshPtr->faceArrayOrList, facePtr)) == NULL)
			goto failure;
	}

	// Save first face having vertex
	if ((faceExtRef = e3meshFace_ExtRefInMesh(facePtr, meshPtr)) == NULL)
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
//		Note :	If iterator ended, vertex deleted, current face deleted,
//				no next face or unable to create external reference to face,
//				return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceExtRef
E3Mesh_NextVertexFace(
	TQ3MeshIterator* iteratorPtr)
{
	TE3MeshData* meshPtr = iteratorPtr->var4.field1;
	TE3MeshVertexExtRef vertexExtRef;
	TE3MeshVertexData* vertexPtr;
	TE3MeshFaceExtRef faceExtRef;
	TE3MeshFaceData* facePtr;

	// Restore and check vertex
	if ((vertexExtRef = (TE3MeshVertexExtRef) iteratorPtr->var2) == NULL)
		goto failure;
	if ((vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef)) == NULL)
		goto failure;

	// Restore and check current face having vertex
	if ((faceExtRef = (TE3MeshFaceExtRef) iteratorPtr->var1) == NULL)
		goto failure;
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	// Get next face having vertex
	for (;;)
	{
		// Get next face in mesh
		if ((facePtr = e3meshFaceArrayOrList_NextItem(&meshPtr->faceArrayOrList, facePtr)) == NULL)
			goto failure;

		// If face has vertex, break
		if (e3meshFace_HasVertex(facePtr, vertexPtr))
			break;
	}

	// Save next face having vertex
	if ((faceExtRef = e3meshFace_ExtRefInMesh(facePtr, meshPtr)) == NULL)
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
E3Mesh_FirstVertexEdge(
	TE3MeshVertexExtRef vertexExtRef,
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_NextVertexEdge(
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_FirstVertexVertex(
	TE3MeshVertexExtRef vertexExtRef,
	TQ3MeshIterator* iteratorPtr)
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
E3Mesh_NextVertexVertex(
	TQ3MeshIterator* iteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetCornerAttributeSet : Get attribute set for corner.
//-----------------------------------------------------------------------------
//		Note : If vertex or face deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetCornerAttributeSet(
	TQ3GeometryObject meshObject,
	TE3MeshVertexExtRef vertexExtRef,
	TE3MeshFaceExtRef faceExtRef,
	TQ3AttributeSet* attributeSetPtr)
{
	TE3MeshVertexData* vertexPtr;
	TE3MeshFaceData* facePtr;
	TE3MeshCornerData* cornerPtr;

	// Check vertex
	if ((vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef)) == NULL)
		goto failure;

	// Check face
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	// Get corner
	cornerPtr = e3meshVertex_FaceCorner(vertexPtr, facePtr);
	
	// Get attribute set
	if (cornerPtr == NULL)
		*attributeSetPtr = NULL;
	else
		E3Shared_Acquire(attributeSetPtr, cornerPtr->attributeSet);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetCornerAttributeSet : Set attribute set for corner.
//-----------------------------------------------------------------------------
//		Note : If vertex or face deleted, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetCornerAttributeSet(
	TQ3GeometryObject meshObject,
	TE3MeshVertexExtRef vertexExtRef,
	TE3MeshFaceExtRef faceExtRef,
	TQ3AttributeSet newAttributeSet)
{
	TE3MeshData* meshPtr = (TE3MeshData*) E3ClassTree_FindInstanceData(meshObject, kQ3GeometryTypeMesh);
	TE3MeshVertexData* vertexPtr;
	TE3MeshFaceData* facePtr;
	TE3MeshCornerData* oldCornerPtr;
	TQ3AttributeSet oldAttributeSet;
	TQ3Uns32 oldFaceCount;

	// Check vertex
	if ((vertexPtr = e3meshVertexExtRef_Vertex(vertexExtRef)) == NULL)
		goto failure;

	// Check face
	if ((facePtr = e3meshFaceExtRef_Face(faceExtRef)) == NULL)
		goto failure;

	// Get old corner, attribute set and face count
	if ((oldCornerPtr = e3meshVertex_FaceCorner(vertexPtr, facePtr)) == NULL)
	{
		oldAttributeSet = NULL;
		oldFaceCount = 0;
	}
	else
	{
		oldAttributeSet = oldCornerPtr->attributeSet;
		oldFaceCount = e3meshFacePtrArrayOrList_Length(&oldCornerPtr->facePtrArrayOrList);
	}
	
	// If changing attribute set
	if (oldAttributeSet != newAttributeSet)
	{
		TE3MeshCornerData* newCornerPtr;
		
		if (newAttributeSet == NULL)
		{
			// ? -> NULL
			
			switch (oldFaceCount)
			{
			case 0:		// NULL -> NULL
				Q3_ASSERT(0);
				break;

			case 1:		// x -> NULL
				// Delete corner (*** MAY RELOCATE CORNERS ***)
				if (e3meshVertex_DeleteCorner(vertexPtr, meshPtr, oldCornerPtr) == kQ3Failure)
					goto failure;
				break;

			default:	// x+ -> NULL
				if (e3meshCorner_DetachFace(oldCornerPtr, facePtr) == kQ3Failure)
					goto failure;
				break;
			}
		}
		else if ((newCornerPtr = e3meshVertex_AttributeSetCorner(vertexPtr, meshPtr, newAttributeSet)) == NULL)
		{
			// ? -> y
			
			switch (oldFaceCount)
			{
			case 0:		// NULL -> y
				// New corner (*** MAY RELOCATE CORNERS ***)
				if ((newCornerPtr = e3meshVertex_NewCorner(vertexPtr, meshPtr, newAttributeSet)) == NULL)
					goto failure;
				if (e3meshCorner_AttachFace(newCornerPtr, facePtr) == kQ3Failure)
				{
					// e3meshVertex_DeleteCorner can't fail, as e3meshVertex_NewCorner uses list of corners
					e3meshVertex_DeleteCorner(vertexPtr, meshPtr, newCornerPtr);
					goto failure;
				}
				break;

			case 1:		// x -> y
				E3Shared_Replace(&oldCornerPtr->attributeSet, newAttributeSet);
				break;

			default:	// x+ -> y
				// New corner (*** MAY RELOCATE CORNERS ***)
				if ((newCornerPtr = e3meshVertex_NewCorner(vertexPtr, meshPtr, newAttributeSet)) == NULL)
					goto failure;
				if (e3meshCorner_SpliceFace(newCornerPtr, oldCornerPtr, facePtr) == kQ3Failure)
				{
					// e3meshVertex_DeleteCorner can't fail, as e3meshVertex_NewCorner uses list of corners
					e3meshVertex_DeleteCorner(vertexPtr, meshPtr, newCornerPtr);
					goto failure;
				}
				break;
			}
		}
		else
		{
			// ? -> y+
			
			switch (oldFaceCount)
			{
			case 0:		// NULL -> y+
				if (e3meshCorner_AttachFace(newCornerPtr, facePtr) == kQ3Failure)
					goto failure;
				break;

			case 1:		// x -> y+
				if (e3meshCorner_SpliceFace(newCornerPtr, oldCornerPtr, facePtr) == kQ3Failure)
					goto failure;
				if (e3meshVertex_DeleteCorner(vertexPtr, meshPtr, oldCornerPtr) == kQ3Failure)
				{
					// e3meshCorner_SpliceFace can't fail, as previous call already uses lists of face pointers
					e3meshCorner_SpliceFace(oldCornerPtr, newCornerPtr, facePtr);
					goto failure;
				}
				break;

			default:	// x+ -> y+
				if (e3meshCorner_SpliceFace(newCornerPtr, oldCornerPtr, facePtr) == kQ3Failure)
					goto failure;
				break;
			}
		}

		Q3Shared_Edited(meshObject);
	}

	return(kQ3Success);

failure:

	return(kQ3Failure);
}

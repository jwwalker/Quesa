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
//      Internal types
//-----------------------------------------------------------------------------
typedef TQ3MeshVertex				TE3MeshVertexHdl;
typedef TQ3MeshEdge					TE3MeshEdgeHdl;
typedef TQ3MeshContour				TE3MeshContourHdl;
typedef TQ3MeshFace					TE3MeshFaceHdl;
typedef TQ3MeshComponent			TE3MeshComponentHdl;



// TE3MeshPartData
typedef struct TE3MeshPartData* TE3MeshPartDataPtr;
typedef TE3MeshPartDataPtr* TE3MeshPartHdl;

typedef struct TE3MeshPartData {
	TE3MeshPartHdl					partHdl;
} TE3MeshPartData;

E3POOL_DECLARE(TE3MeshPartDataPtr, e3meshPartDataPtr);
E3POOL_DEFINE(TE3MeshPartDataPtr, e3meshPartDataPtr, static, 8);



// TE3MeshVertexData
typedef struct TE3MeshVertexData {
	TE3MeshPartData					partData;			// base class
	struct TE3MeshData*				meshDataPtr;		// ptr to (containing) mesh
	TQ3Point3D 						point;
	TQ3AttributeSet 				attributeSet;
} TE3MeshVertexData;

E3ARRAY_DECLARE(TE3MeshVertexData, e3meshVertexData);
E3ARRAY_DEFINE(TE3MeshVertexData, e3meshVertexData, static);

E3LIST_DECLARE(TE3MeshVertexData, e3meshVertexData);
E3LIST_DEFINE(TE3MeshVertexData, e3meshVertexData, static);

E3ARRAY_OR_LIST_DECLARE(TE3MeshVertexData, e3meshVertexData);
E3ARRAY_OR_LIST_DEFINE(TE3MeshVertexData, e3meshVertexData, static);



// TE3MeshVertexHdl
E3ARRAY_DECLARE(TE3MeshVertexHdl, e3meshVertexHdl);
E3ARRAY_DEFINE(TE3MeshVertexHdl, e3meshVertexHdl, static);



// TE3MeshContourData
typedef struct TE3MeshContourData {
	TE3MeshPartData					partData;			// base class
	TE3MeshFaceHdl					containerFaceHdl;
	TE3MeshVertexHdlArray			vertexHdlArray;
} TE3MeshContourData;

E3ARRAY_DECLARE(TE3MeshContourData, e3meshContourData);
E3ARRAY_DEFINE(TE3MeshContourData, e3meshContourData, static);

E3LIST_DECLARE(TE3MeshContourData, e3meshContourData);
E3LIST_DEFINE(TE3MeshContourData, e3meshContourData, static);

E3ARRAY_OR_LIST_DECLARE(TE3MeshContourData, e3meshContourData);
E3ARRAY_OR_LIST_DEFINE(TE3MeshContourData, e3meshContourData, static);



// TE3MeshFaceData
typedef struct TE3MeshFaceData {
	TE3MeshPartData					partData;			// base class
	TE3MeshContourDataArrayOrList	contourDataArrayOrList;
	TQ3AttributeSet 				attributeSet;
} TE3MeshFaceData;

E3ARRAY_DECLARE(TE3MeshFaceData, e3meshFaceData);
E3ARRAY_DEFINE(TE3MeshFaceData, e3meshFaceData, static);

E3LIST_DECLARE(TE3MeshFaceData, e3meshFaceData);
E3LIST_DEFINE(TE3MeshFaceData, e3meshFaceData, static);

E3ARRAY_OR_LIST_DECLARE(TE3MeshFaceData, e3meshFaceData);
E3ARRAY_OR_LIST_DEFINE(TE3MeshFaceData, e3meshFaceData, static);



// TE3MeshData
typedef struct TE3MeshData {
	TE3MeshPartDataPtrPool			partDataPtrPool;
	TE3MeshVertexDataArrayOrList	vertexDataArrayOrList;
	TE3MeshFaceDataArrayOrList		faceDataArrayOrList;
	TQ3AttributeSet 				attributeSet;
} TE3MeshData;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3meshData_ReferencePartData : Allocate reference to mesh part data.
//-----------------------------------------------------------------------------
//		Note :	If unable to generate reference (out of memory), return
//					kQ3Failure.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshData_ReferencePartData(TE3MeshData* meshDataPtr, TE3MeshPartData* meshPartDataPtr)
{
	TE3MeshPartHdl meshPartHdl;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);

	if ((meshPartHdl = e3meshPartDataPtrPool_Allocate(&meshDataPtr->partDataPtrPool)) == NULL)
		goto failure;

	*meshPartHdl = meshPartDataPtr;
	meshPartDataPtr->partHdl = meshPartHdl;

	return(kQ3Success);

failure:
	return(kQ3Failure);
}





//=============================================================================
//      e3meshData_DereferencePartData : Free reference to mesh part data.
//-----------------------------------------------------------------------------
static void
e3meshData_DereferencePartData(TE3MeshData* meshDataPtr, TE3MeshPartData* meshPartDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);

	if (meshPartDataPtr->partHdl != NULL)
	{
		e3meshPartDataPtrPool_Free(&meshDataPtr->partDataPtrPool, meshPartDataPtr->partHdl);
		meshPartDataPtr->partHdl = NULL;
	}
}





//=============================================================================
//      e3meshData_PartDataToHandle :	Convert TE3MeshPartData (internal) to
//										TE3MeshPartHdl (external).
//-----------------------------------------------------------------------------
//		Note : If unable to get handle (out of memory), return NULL.
//-----------------------------------------------------------------------------
static TE3MeshPartHdl
e3meshData_PartDataToHandle(TE3MeshData* meshDataPtr, TE3MeshPartData* meshPartDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);

	if (meshPartDataPtr->partHdl == NULL)
	{
		if (e3meshData_ReferencePartData(meshDataPtr, meshPartDataPtr) == kQ3Failure)
			goto failure;
	}

	return(meshPartDataPtr->partHdl);

failure:
	return(NULL);
}





//=============================================================================
//      e3meshData_VertexDataToHandle :	Convert TE3MeshVertexData (internal) to
//										TE3MeshVertexHdl (external).
//-----------------------------------------------------------------------------
//		Note : If unable to get handle (out of memory), return NULL.
//-----------------------------------------------------------------------------
static TE3MeshVertexHdl
e3meshData_VertexDataToHandle(TE3MeshData* meshDataPtr, TE3MeshVertexData* meshVertexDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(meshVertexDataPtr);

	return(E3_DOWN_CAST(TE3MeshVertexHdl,
		e3meshData_PartDataToHandle(meshDataPtr,
			E3_UP_CAST(TE3MeshPartData*, meshVertexDataPtr))));
}





//=============================================================================
//      e3meshData_ContourDataToHandle :	Convert TE3MeshContourData (internal)
//											to TE3MeshContourHdl (external).
//-----------------------------------------------------------------------------
//		Note : If unable to get handle (out of memory), return NULL.
//-----------------------------------------------------------------------------
static TE3MeshContourHdl
e3meshData_ContourDataToHandle(TE3MeshData* meshDataPtr, TE3MeshContourData* meshContourDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);

	return(E3_DOWN_CAST(TE3MeshContourHdl,
		e3meshData_PartDataToHandle(meshDataPtr,
			E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr))));
}





//=============================================================================
//      e3meshData_FaceDataToHandle :	Convert TE3MeshFaceData (internal)
//										to TQ3MeshFace (external).
//-----------------------------------------------------------------------------
//		Note : If unable to get handle (out of memory), return NULL.
//-----------------------------------------------------------------------------
static TE3MeshFaceHdl
e3meshData_FaceDataToHandle(TE3MeshData* meshDataPtr, TE3MeshFaceData* meshFaceDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);

	return(E3_DOWN_CAST(TE3MeshFaceHdl,
		e3meshData_PartDataToHandle(meshDataPtr,
			E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr))));
}





//=============================================================================
//      e3meshPartData_Create : TE3MeshPartData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3meshPartData_Create(TE3MeshPartData* meshPartDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// If requested, reference mesh part data
	if (isReferenced)
	{
		if (e3meshData_ReferencePartData(meshDataPtr, meshPartDataPtr) == kQ3Failure)
			goto failure;
	}
	else
		meshPartDataPtr->partHdl = NULL;

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

	// If mesh part data is referenced, unreference
	if (meshPartDataPtr->partHdl)
		*meshPartDataPtr->partHdl = NULL;
}





//=============================================================================
//      e3meshPartData_Relocate : Relocate mesh part data.
//-----------------------------------------------------------------------------
static void
e3meshPartData_Relocate(TE3MeshPartData* meshPartDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);

	// If mesh part data is referenced, re-reference
	if (meshPartDataPtr->partHdl)
		*meshPartDataPtr->partHdl = meshPartDataPtr;
}





/*
???
//=============================================================================
//      e3meshPartData_IsReferencedByHandle :	Return if mesh part data is
//												referenced by handle.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshPartData_IsReferencedByHandle(TE3MeshPartData* meshPartDataPtr, TE3MeshPartHdl meshPartHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);
	Q3_ASSERT_VALID_PTR(meshPartHdl);

	return(meshPartDataPtr->partHdl == meshPartHdl ? kQ3Success : kQ3Failure);
}
*/





/*
???
//=============================================================================
//      e3meshPartData_IsNotReferencedByHandle :	Return if mesh part data is
//													NOT referenced by handle.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshPartData_IsNotReferencedByHandle(TE3MeshPartData* meshPartDataPtr, TE3MeshPartHdl meshPartHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartDataPtr);
	Q3_ASSERT_VALID_PTR(meshPartHdl);

	return(meshPartDataPtr->partHdl != meshPartHdl ? kQ3Success : kQ3Failure);
}
*/





//=============================================================================
//      e3meshPartHdl_Data : Return data for this part.
//-----------------------------------------------------------------------------
//		Note : If part no longer exists, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshPartData*
e3meshPartHdl_Data(TE3MeshPartHdl meshPartHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartHdl);

	return(*meshPartHdl);
}





//=============================================================================
//      e3meshPartHdl_IsEqualTo : Return if two part handles are equal.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3meshPartHdl_IsEqualTo(TE3MeshPartHdl* meshPartHdlPtr1, TE3MeshPartHdl* meshPartHdlPtr2)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshPartHdlPtr1);
	Q3_ASSERT_VALID_PTR(meshPartHdlPtr2);

	return(*meshPartHdlPtr1 == *meshPartHdlPtr2 ? kQ3True : kQ3False);
}





//=============================================================================
//      e3meshVertexData_Create : TE3MeshVertexData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
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

	// Initialize mesh data pointer
	meshVertexDataPtr->meshDataPtr = meshDataPtr;

	// Initialize point
	meshVertexDataPtr->point = vertexPtr->point;

	// Acquire attribute set
	E3Shared_Acquire(&meshVertexDataPtr->attributeSet, vertexPtr->attributeSet);

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
	E3Object_DisposeAndForget(meshVertexDataPtr->attributeSet);

	// Destroy part data
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, meshVertexDataPtr));
}





//=============================================================================
//      e3meshVertexData_Relocate : Relocated mesh vertex data.
//-----------------------------------------------------------------------------
static void
e3meshVertexData_Relocate(TE3MeshVertexData* meshVertexDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshVertexDataPtr);

	// Relocate part data
	e3meshPartData_Relocate(E3_UP_CAST(TE3MeshPartData*, meshVertexDataPtr));
}





//=============================================================================
//      e3meshVertexData_MeshData : Return data for mesh having this vertex.
//-----------------------------------------------------------------------------
static TE3MeshData*
e3meshVertexData_MeshData(TE3MeshVertexData* meshVertexDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshVertexDataPtr);

	return(meshVertexDataPtr->meshDataPtr);
}





//=============================================================================
//      e3meshVertexHdl_Data : Return data for this vertex.
//-----------------------------------------------------------------------------
//		Note : If vertex no longer exists, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshVertexData*
e3meshVertexHdl_Data(TE3MeshVertexHdl meshVertexHdl)
{
	return(E3_DOWN_CAST(TE3MeshVertexData*,
		e3meshPartHdl_Data(
			E3_UP_CAST(TE3MeshPartHdl, meshVertexHdl))));
}





//=============================================================================
//      e3meshVertexHdl_MeshData : Return data for mesh having this vertex.
//-----------------------------------------------------------------------------
//		Note : If vertex no longer exists, return NULL.
//-----------------------------------------------------------------------------
static TE3MeshData*
e3meshVertexHdl_MeshData(TE3MeshVertexHdl meshVertexHdl)
{
	TE3MeshVertexData* meshVertexDataPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshVertexHdl);

	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
		goto failure;

	return(e3meshVertexData_MeshData(meshVertexDataPtr));

failure:
	return(NULL);
}





//=============================================================================
//      e3meshContourData_Create : TE3MeshContourData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs more than once in succession,
//				repeated occurrences are eliminated. If after this elimination
//				less than 2 vertices remain, the creation fails.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3meshContourData_Create(TE3MeshContourData* meshContourDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced, TE3MeshFaceHdl containerMeshFaceHdl, TQ3Uns32 numVertices, const TE3MeshVertexHdl* meshVertexHdls)
{
	TQ3Uns32 effectiveNumVertices;
	TQ3Uns32 i;
	TE3MeshVertexHdl* meshVertexHdlPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT(numVertices > 0);
	Q3_ASSERT_VALID_PTR(meshVertexHdls);

	// Count effective number of vertices (excluding repeats)
	effectiveNumVertices = 0;
	for (i = 0; i < numVertices; ++i)
		if (meshVertexHdls[i] != meshVertexHdls[i > 0 ? i-1 : numVertices-1])
			++effectiveNumVertices;
	if (effectiveNumVertices < 2)
		goto failure_1;

	// Create part data
	if (e3meshPartData_Create(E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr), meshDataPtr, isReferenced) == kQ3Failure)
		goto failure_1;

	// Initialize container face handle
	meshContourDataPtr->containerFaceHdl = containerMeshFaceHdl;

	// Create uninitialized vertex handle array
	if (e3meshVertexHdlArray_Create(&meshContourDataPtr->vertexHdlArray, effectiveNumVertices, NULL) == kQ3Failure)
		goto failure_2;

	// Initialize vertex handle array
	meshVertexHdlPtr = e3meshVertexHdlArray_FirstItem(&meshContourDataPtr->vertexHdlArray);
	for (i = 0; i < numVertices; ++i)
	{
		if (meshVertexHdls[i] != meshVertexHdls[i > 0 ? i-1 : numVertices-1])
		{
			*meshVertexHdlPtr = meshVertexHdls[i];
			++meshVertexHdlPtr;
		}
	}

	return(kQ3Success);

failure_2:
	e3meshData_DereferencePartData(meshDataPtr, E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr));
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr));

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

	// Destroy vertex handle array
	e3meshVertexHdlArray_Destroy(&meshContourDataPtr->vertexHdlArray, NULL);

	// Destroy part data
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr));
}





//=============================================================================
//      e3meshContourData_Relocate : Relocated mesh vertex data.
//-----------------------------------------------------------------------------
static void
e3meshContourData_Relocate(TE3MeshContourData* meshContourDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);

	// Relocate part data
	e3meshPartData_Relocate(E3_UP_CAST(TE3MeshPartData*, meshContourDataPtr));
}





//=============================================================================
//      e3meshContourData_MeshData : Return data for mesh having this contour.
//-----------------------------------------------------------------------------
static TE3MeshData*
e3meshContourData_MeshData(TE3MeshContourData* meshContourDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);

	return(e3meshVertexHdl_MeshData(*e3meshVertexHdlArray_FirstItem(&meshContourDataPtr->vertexHdlArray)));
}





//=============================================================================
//      e3meshContourData_SetContainerFaceHdl : Set handle to container face.
//-----------------------------------------------------------------------------
static void
e3meshContourData_SetContainerFaceHdl(TE3MeshContourData* meshContourDataPtr, TE3MeshFaceHdl containerMeshFaceHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);
	Q3_ASSERT_VALID_PTR(containerMeshFaceHdl);

	meshContourDataPtr->containerFaceHdl = containerMeshFaceHdl;
}





//=============================================================================
//      e3meshContourData_ContainerFaceHdl : Return handle to container face.
//-----------------------------------------------------------------------------
static TE3MeshFaceHdl
e3meshContourData_ContainerFaceHdl(TE3MeshContourData* meshContourDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);

	return(meshContourDataPtr->containerFaceHdl);
}





//=============================================================================
//      e3meshContourData_HasVertexHdl : Return if contour has vertex.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3meshContourData_HasVertexHdl(TE3MeshContourData* meshContourDataPtr, TE3MeshVertexHdl meshVertexHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourDataPtr);
	Q3_ASSERT_VALID_PTR(meshVertexHdl);

	return(e3meshVertexHdlArray_OrForEach(&meshContourDataPtr->vertexHdlArray,
		E3_DOWN_CAST(TQ3Boolean (*)(TE3MeshVertexHdl*, void*), e3meshPartHdl_IsEqualTo), &meshVertexHdl));
}





//=============================================================================
//      e3meshContourHdl_Data : Return data for this contour.
//-----------------------------------------------------------------------------
//		Note : If contour no longer exists, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshContourData*
e3meshContourHdl_Data(TE3MeshContourHdl meshContourHdl)
{
	return(E3_DOWN_CAST(TE3MeshContourData*,
		e3meshPartHdl_Data(
			E3_UP_CAST(TE3MeshPartHdl, meshContourHdl))));
}





//=============================================================================
//      e3meshContourHdl_MeshData : Return data for mesh having this contour.
//-----------------------------------------------------------------------------
//		Note : If contour no longer exists, return NULL.
//-----------------------------------------------------------------------------
static TE3MeshData*
e3meshContourHdl_MeshData(TE3MeshContourHdl meshContourHdl)
{
	TE3MeshContourData* meshContourDataPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshContourHdl);

	if ((meshContourDataPtr = e3meshContourHdl_Data(meshContourHdl)) == NULL)
		goto failure;

	return(e3meshContourData_MeshData(meshContourDataPtr));

failure:
	return(NULL);
}





//=============================================================================
//      e3meshFaceData_Create : TE3MeshFaceData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3meshFaceData_Create(TE3MeshFaceData* meshFaceDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced, TQ3Uns32 numContours, TQ3Uns32* numVerticesPtr, const TE3MeshVertexHdl** meshVertexHdlsPtr, TQ3AttributeSet attributeSet)
{
	TE3MeshFaceHdl meshFaceHdl;
	TQ3Uns32 i;
	TE3MeshContourData* meshContourDataPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT(numContours > 0);

	// Create part data, with reference
	if (e3meshPartData_Create(E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr), meshDataPtr, kQ3True) == kQ3Failure)
		goto failure_1;

	// Get handle to face
	if ((meshFaceHdl = e3meshData_FaceDataToHandle(meshDataPtr, meshFaceDataPtr)) == NULL)
		goto failure_2;

	// Create uninitialized contour data array
	if (e3meshContourDataArray_Create(&meshFaceDataPtr->contourDataArrayOrList.array, numContours, NULL) == kQ3Failure)
		goto failure_2;

	// Create each contour data
	for (i = 0, meshContourDataPtr = e3meshContourDataArray_FirstItem(&meshFaceDataPtr->contourDataArrayOrList.array);
		i < numContours;
		++i, meshContourDataPtr = e3meshContourDataArray_NextItem(&meshFaceDataPtr->contourDataArrayOrList.array, meshContourDataPtr))
	{
		Q3_ASSERT(numVerticesPtr[i] > 0);
		Q3_ASSERT_VALID_PTR(meshVertexHdlsPtr[i]);

		// Create contour data
		if (e3meshContourData_Create(meshContourDataPtr, meshDataPtr, kQ3False, meshFaceHdl, numVerticesPtr[i], meshVertexHdlsPtr[i]) == kQ3Failure)
			goto failure_3;
	}

	// Acquire attribute set
	E3Shared_Acquire(&meshFaceDataPtr->attributeSet, attributeSet);

	return(kQ3Success);

failure_3:
	while (i-- > 0)
	{
		meshContourDataPtr = e3meshContourDataArray_PreviousItem(&meshFaceDataPtr->contourDataArrayOrList.array, meshContourDataPtr);
		e3meshContourData_Destroy(meshContourDataPtr);
	}
	e3meshContourDataArray_Destroy(&meshFaceDataPtr->contourDataArrayOrList.array, NULL);

failure_2:
	e3meshData_DereferencePartData(meshDataPtr, E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr));
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
	E3Object_DisposeAndForget(meshFaceDataPtr->attributeSet);

	// Destroy contour data array or list
	e3meshContourDataArrayOrList_Destroy(&meshFaceDataPtr->contourDataArrayOrList, e3meshContourData_Destroy);

	// Destroy part data
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr));
}





//=============================================================================
//      e3meshFaceData_Relocate : Relocated mesh vertex data.
//-----------------------------------------------------------------------------
static void
e3meshFaceData_Relocate(TE3MeshFaceData* meshFaceDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);

	// Relocate part data
	e3meshPartData_Relocate(E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr));
}





//=============================================================================
//      e3meshFaceData_MeshData : Return data for mesh having this face.
//-----------------------------------------------------------------------------
static TE3MeshData*
e3meshFaceData_MeshData(TE3MeshFaceData* meshFaceDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);

	return(e3meshContourData_MeshData(e3meshContourDataArrayOrList_FirstItem(&meshFaceDataPtr->contourDataArrayOrList)));
}





//=============================================================================
//      e3meshFaceData_HasVertexHdl : Return if face has vertex.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3meshFaceData_HasVertexHdl(TE3MeshFaceData* meshFaceDataPtr, TE3MeshVertexHdl meshVertexHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);
	Q3_ASSERT_VALID_PTR(meshVertexHdl);

	return(e3meshContourDataArrayOrList_OrForEach(&meshFaceDataPtr->contourDataArrayOrList,
		E3_DOWN_CAST(TQ3Boolean (*)(TE3MeshContourData*, void*), e3meshContourData_HasVertexHdl), meshVertexHdl));
}





//=============================================================================
//      e3meshFaceHdl_Data : Return data for this face.
//-----------------------------------------------------------------------------
//		Note : If face no longer exists, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshFaceData*
e3meshFaceHdl_Data(TE3MeshFaceHdl meshFaceHdl)
{
	return(E3_DOWN_CAST(TE3MeshFaceData*,
		e3meshPartHdl_Data(
			E3_UP_CAST(TE3MeshPartHdl, meshFaceHdl))));
}





//=============================================================================
//      e3meshFaceHdl_MeshData : Return data for mesh having this face.
//-----------------------------------------------------------------------------
//		Note : If face no longer exists, return NULL.
//-----------------------------------------------------------------------------
static TE3MeshData*
e3meshFaceHdl_MeshData(TE3MeshFaceHdl meshFaceHdl)
{
	TE3MeshFaceData* meshFaceDataPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceHdl);

	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	return(e3meshFaceData_MeshData(meshFaceDataPtr));

failure:
	return(NULL);
}





/*
???
//=============================================================================
//      e3meshData_FaceHandleToData :	Convert TE3MeshFaceHdl (external) to
//										TE3MeshFaceData (internal).
//-----------------------------------------------------------------------------
//		Note : If face no longer exists, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static TE3MeshFaceData*
e3meshData_FaceHandleToData(TE3MeshData* meshDataPtr, TE3MeshFaceHdl meshFaceHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(meshFace);

	return(E3_DOWN_CAST(TE3MeshFaceData*,
		e3meshData_PartPtrToData(meshDataPtr,
			E3_UP_CAST(TE3MeshPartHdl, meshFace))));
}





//=============================================================================
//      e3meshData_FaceDataToHandle :	Convert TE3MeshFaceData (internal) to
//										TE3MeshFaceHdl (external).
//-----------------------------------------------------------------------------
//		Note : If unable to get handle, return NULL.
//-----------------------------------------------------------------------------
static TE3MeshFaceHdl
e3meshData_FaceDataToHandle(TE3MeshData* meshDataPtr, TE3MeshFaceData* meshFaceDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshDataPtr);
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);

	return(E3_DOWN_CAST(TE3MeshFaceHdl,
		e3meshData_PartDataToHandle(meshDataPtr,
			E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr))));
}





//=============================================================================
//      e3meshFaceData_Create : TE3MeshFaceData constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create, return kQ3Failure.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshFaceData_Create(TE3MeshFaceData* meshFaceDataPtr, TE3MeshData* meshDataPtr,
	TQ3Boolean isReferenced, TQ3Uns32 numVertices, const TE3MeshVertexHdl* meshVertexHdls, TQ3AttributeSet attributeSet)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);
	Q3_ASSERT_VALID_PTR(meshDataPtr);

	// Create part data
	if (e3meshPartData_Create(E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr), meshDataPtr, isReferenced) == kQ3Failure)
		goto failure_1;

	// Initialize mesh face data kind
	meshFaceDataPtr->kind = kE3MeshFaceDataKindSimpleConvex;

	// Create mesh contour data or list
	switch (meshFaceDataPtr->kind)
	{
	case kE3MeshFaceDataKindSimpleConvex:
	case kE3MeshFaceDataKindSimpleConcave:
		if (e3meshContourData_Create(&meshFaceDataPtr->contourDataOrList.data, meshDataPtr,
			kQ3False, numVertices, meshVertexHdls) == kQ3Failure)
				goto failure_2;
		break;

	case kE3MeshFaceDataKindComplex:
		if (e3meshContourDataList_CreateUninitialized(&meshFaceDataPtr->contourDataOrList.list, 1) == kQ3Failure)
			goto failure_2;
		// ??? Must create contour data ???
		break;
	}

	// Acquire mesh face attribute set
	E3Shared_Acquire(&meshFaceDataPtr->attributeSet, attributeSet);

	return(kQ3Success);

failure_2:
	e3meshData_DereferencePartData(meshDataPtr, E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr));
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
	E3Object_DisposeAndForget(meshFaceDataPtr->attributeSet);

	// Destroy mesh contour data or list
	switch (meshFaceDataPtr->kind)
	{
	case kE3MeshFaceDataKindSimpleConvex:
	case kE3MeshFaceDataKindSimpleConcave:
		e3meshContourData_Destroy(&meshFaceDataPtr->contourDataOrList.data);
		break;
	case kE3MeshFaceDataKindComplex:
		e3meshContourDataList_Destroy(&meshFaceDataPtr->contourDataOrList.list,
			e3meshContourData_Destroy);
		break;
	}

	// Destroy part data
	e3meshPartData_Destroy(E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr));
}





//=============================================================================
//      e3meshFaceData_Relocate : Relocated mesh vertex data.
//-----------------------------------------------------------------------------
static void
e3meshFaceData_Relocate(TE3MeshFaceData* meshFaceDataPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(meshFaceDataPtr);

	// Relocate part data
	e3meshPartData_Relocate(E3_UP_CAST(TE3MeshPartData*, meshFaceDataPtr));
}
*/





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
	TE3MeshData* meshDataPtr = (TE3MeshData *) privateData;
#pragma unused(paramData)

	// Create mesh part references pool
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

failure_3:
	e3meshVertexDataArray_Destroy(&meshDataPtr->vertexDataArrayOrList.array, e3meshVertexData_Destroy);

failure_2:
	e3meshPartDataPtrPool_Destroy(&meshDataPtr->partDataPtrPool);

failure_1:
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_mesh_delete : TE3MeshData destructor.
//-----------------------------------------------------------------------------
static void
e3geom_mesh_delete(TQ3Object theObject, void *privateData)
{
#pragma unused(theObject)
	TE3MeshData* meshDataPtr = (TE3MeshData *) privateData;

	//	Release attribute set
	E3Object_DisposeAndForget(meshDataPtr->attributeSet);

	// Destroy face data array or list
	e3meshFaceDataArrayOrList_Destroy(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Destroy);

	// Destroy vertex data array or list
	e3meshVertexDataArrayOrList_Destroy(&meshDataPtr->vertexDataArrayOrList, e3meshVertexData_Destroy);

	// Destroy mesh part references pool
	e3meshPartDataPtrPool_Destroy(&meshDataPtr->partDataPtrPool);
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
e3geom_mesh_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TE3MeshData *geomData)
{	TQ3GroupObject					theGroup;
#pragma unused(theView)



	// Create a group to hold the cached geometry
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Add the cached form to the group



	// Finish off the group state (in-line, since we don't make any view state changes)
	Q3DisplayGroup_SetState(theGroup, kQ3DisplayGroupStateMaskIsInline  |
									  kQ3DisplayGroupStateMaskIsDrawn   |
									  kQ3DisplayGroupStateMaskIsWritten |
									  kQ3DisplayGroupStateMaskIsPicked);

	return(theGroup);
}





//=============================================================================
//      e3geom_mesh_bounds : Mesh bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_mesh_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
	//??? Use accessor function ???
	const TE3MeshData			*instanceData = (const TE3MeshData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



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
	return(&meshDataPtr->attributeSet);
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
//		Note :	If unable to relocate faces, if unable to insert face item,
//					or if unable to create face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceHdl
E3Mesh_FaceNew(TQ3GeometryObject mesh, TQ3Uns32 numVertices, const TE3MeshVertexHdl* meshVertexHdls,
	TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure_1;

	// Push back new uninitialized face data item into mesh
	if ((meshFaceDataPtr = e3meshFaceDataList_PushBackItem(&meshDataPtr->faceDataArrayOrList.list, NULL)) == NULL)
		goto failure_1;

	// Create face data
	if (e3meshFaceData_Create(meshFaceDataPtr, meshDataPtr, kQ3True, 1, &numVertices, &meshVertexHdls, attributeSet) == kQ3Failure)
		goto failure_2;

	Q3Shared_Edited(mesh);

	return(e3meshData_FaceDataToHandle(meshDataPtr, meshFaceDataPtr));

failure_2:
	e3meshFaceDataList_EraseItem(&meshDataPtr->faceDataArrayOrList.list, NULL, meshFaceDataPtr);

failure_1:
	return(NULL);
}





//=============================================================================
//      E3Mesh_FaceDelete : Delete face from mesh.
//-----------------------------------------------------------------------------
//		Note :	If face already deleted or unable to relocate faces,
//					return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_FaceDelete(TQ3GeometryObject mesh, TE3MeshFaceHdl meshFaceHdl)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck face (in case relocated)
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Erase face data item
	e3meshFaceDataList_EraseItem(&meshDataPtr->faceDataArrayOrList.list, e3meshFaceData_Destroy,
		meshFaceDataPtr);

	Q3Shared_Edited(mesh);

	return(kQ3Success);

failure:
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FaceToContour :	Append face's contours to container face's, and
//								delete face.
//-----------------------------------------------------------------------------
//		Note :	If face deleted, unable to relocate faces, or unable to
//					relocate contours, return NULL.
//-----------------------------------------------------------------------------
TE3MeshContourHdl
E3Mesh_FaceToContour(TQ3GeometryObject mesh, TE3MeshFaceHdl containerMeshFaceHdl, TE3MeshFaceHdl meshFaceHdl)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* containerMeshFaceDataPtr;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshContourHdl meshContourHdl;

	// Check container face
	if ((containerMeshFaceDataPtr = e3meshFaceHdl_Data(containerMeshFaceHdl)) == NULL)
		goto failure;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck container face (in case relocated)
	if ((containerMeshFaceDataPtr = e3meshFaceHdl_Data(containerMeshFaceHdl)) == NULL)
		goto failure;

	// Recheck face (in case relocated)
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Use list of contours in container face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&containerMeshFaceDataPtr->contourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure;

	// Use list of contours in face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&meshFaceDataPtr->contourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure;

	// Get first contour in face
	if ((meshContourDataPtr = e3meshContourDataList_FirstItem(&meshFaceDataPtr->contourDataArrayOrList.list)) == NULL)
		goto failure;
	if ((meshContourHdl = e3meshData_ContourDataToHandle(meshDataPtr, meshContourDataPtr)) == NULL)
		goto failure;

	// For each contour in face, reset container face
	for (meshContourDataPtr = e3meshContourDataList_FirstItem(&meshFaceDataPtr->contourDataArrayOrList.list);
		meshContourDataPtr != NULL;
		meshContourDataPtr = e3meshContourDataList_NextItem(&meshFaceDataPtr->contourDataArrayOrList.list, meshContourDataPtr))
	{
		// Reset container face
		e3meshContourData_SetContainerFaceHdl(meshContourDataPtr, containerMeshFaceHdl);
	}

	// Splice contours from face into container face
	e3meshContourDataList_SpliceBackList(&containerMeshFaceDataPtr->contourDataArrayOrList.list, &meshFaceDataPtr->contourDataArrayOrList.list);

	// Erase face data item from mesh
	e3meshFaceDataList_EraseItem(&meshDataPtr->faceDataArrayOrList.list, e3meshFaceData_Destroy,
		meshFaceDataPtr);

	Q3Shared_Edited(mesh);

	// Return contour
	return(meshContourHdl);

failure:
	return(NULL);
}





//=============================================================================
//      E3Mesh_ContourToFace : Add new face with contour, and delete container
//								face if it has no remaining contours.
//-----------------------------------------------------------------------------
//		Note :	If contour deleted, unable to relocate faces, unable to
//					insert face item, or unable to relocate contours, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceHdl
E3Mesh_ContourToFace(TQ3GeometryObject mesh, TE3MeshContourHdl meshContourHdl)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshFaceHdl containerMeshFaceHdl;
	TE3MeshFaceData* containerMeshFaceDataPtr;
	TE3MeshFaceData* meshFaceDataPtr;

	// Check contour
	if ((meshContourDataPtr = e3meshContourHdl_Data(meshContourHdl)) == NULL)
		goto failure_1;
	
	// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure_1;
		
	// Get and check container face
	containerMeshFaceHdl = e3meshContourData_ContainerFaceHdl(meshContourDataPtr);
	if ((containerMeshFaceDataPtr = e3meshFaceHdl_Data(containerMeshFaceHdl)) == NULL)
		goto failure_1;

	// Push back new uninitialized face data item into mesh
	if ((meshFaceDataPtr = e3meshFaceDataList_PushBackItem(&meshDataPtr->faceDataArrayOrList.list, NULL)) == NULL)
		goto failure_1;

	// Create face data with no contours
	if (e3meshFaceData_Create(meshFaceDataPtr, meshDataPtr, kQ3True, 0, NULL, NULL, NULL) == kQ3Failure)
		goto failure_2;

	// Use list of contours in container face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&containerMeshFaceDataPtr->contourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure_2;

	// Use list of contours in face (*** MAY RELOCATE CONTOURS ***)
	if (e3meshContourDataArrayOrList_UseList(&meshFaceDataPtr->contourDataArrayOrList, e3meshContourData_Relocate) == kQ3Failure)
		goto failure_2;

	// Splice contour from container face into new face
	e3meshContourDataList_SpliceBackList(&containerMeshFaceDataPtr->contourDataArrayOrList.list, &meshFaceDataPtr->contourDataArrayOrList.list);
	
	// If container face has no contours, erase container face data item
	if (e3meshContourDataList_Length(&containerMeshFaceDataPtr->contourDataArrayOrList.list) == 0)
		e3meshFaceDataList_EraseItem(&meshDataPtr->faceDataArrayOrList.list, e3meshFaceData_Destroy,
			meshFaceDataPtr);

	Q3Shared_Edited(mesh);

	// Return face
	return(e3meshData_FaceDataToHandle(meshDataPtr, meshFaceDataPtr));

failure_2:
	e3meshFaceDataList_EraseItem(&meshDataPtr->faceDataArrayOrList.list, NULL, meshFaceDataPtr);

failure_1:
	return(NULL);
}





//=============================================================================
//      E3Mesh_VertexNew : Add new vertex created from coordinates and attribute set.
//-----------------------------------------------------------------------------
//		Note :	If unable to relocate vertices, if unable to insert vertex
//					item, or if unable to create vertex, return NULL.
//-----------------------------------------------------------------------------
TE3MeshVertexHdl
E3Mesh_VertexNew(TQ3GeometryObject mesh, const TQ3Vertex3D *vertexPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;

	// Use list of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3meshVertexDataArrayOrList_UseList(&meshDataPtr->vertexDataArrayOrList, e3meshVertexData_Relocate) == kQ3Failure)
		goto failure_1;

	// Push back new uninitialized vertex data item into mesh
	if ((meshVertexDataPtr = e3meshVertexDataList_PushBackItem(&meshDataPtr->vertexDataArrayOrList.list, NULL)) == NULL)
		goto failure_1;

	// Create vertex data
	if (e3meshVertexData_Create(meshVertexDataPtr, meshDataPtr, kQ3True, vertexPtr) == kQ3Failure)
		goto failure_2;

	Q3Shared_Edited(mesh);

	return(e3meshData_VertexDataToHandle(meshDataPtr, meshVertexDataPtr));

failure_2:
	e3meshVertexDataList_EraseItem(&meshDataPtr->vertexDataArrayOrList.list, NULL, meshVertexDataPtr);

failure_1:
	return(NULL);
}





//=============================================================================
//      E3Mesh_VertexDelete : Delete vertex from mesh.
//-----------------------------------------------------------------------------
//		Note :	If vertex already deleted or unable to relocate vertices,
//					return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_VertexDelete(TQ3GeometryObject mesh, TE3MeshVertexHdl meshVertexHdl)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshFaceData* meshFaceDataPtr;

	// Check vertex
	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
		goto failure;

	// Use list of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3meshVertexDataArrayOrList_UseList(&meshDataPtr->vertexDataArrayOrList, e3meshVertexData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck vertex (in case relocated)
	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
		goto failure;

	// Get first face in mesh
	meshFaceDataPtr = e3meshFaceDataArrayOrList_FirstItem(&meshDataPtr->faceDataArrayOrList);

	// Delete each face having vertex
	while (meshFaceDataPtr != NULL)
	{
		TE3MeshFaceData* markedMeshFaceDataPtr = NULL;

		// Check if face has vertex
		if (e3meshFaceData_HasVertexHdl(meshFaceDataPtr, meshVertexHdl))
		{
			TE3MeshFaceHdl meshFaceHdl = NULL;

			// Save face
			if ((meshFaceHdl = e3meshData_FaceDataToHandle(meshDataPtr, meshFaceDataPtr)) == NULL)
				goto failure;

			// Use list of faces in mesh (*** MAY RELOCATE FACES ***)
			if (e3meshFaceDataArrayOrList_UseList(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
				goto failure;

			// Restore face (in case relocated)
			if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
				goto failure;

			// Mark face for erasure
			markedMeshFaceDataPtr = meshFaceDataPtr;
		}

		// Get next face in mesh
		meshFaceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->faceDataArrayOrList, meshFaceDataPtr);

		// If face marked for erasure, erase face data item
		if (markedMeshFaceDataPtr)
			e3meshFaceDataList_EraseItem(&meshDataPtr->faceDataArrayOrList.list, e3meshFaceData_Destroy,
				markedMeshFaceDataPtr);
	}

	// Erase vertex data item from mesh
	e3meshVertexDataList_EraseItem(&meshDataPtr->vertexDataArrayOrList.list, e3meshVertexData_Destroy,
		meshVertexDataPtr);

	Q3Shared_Edited(mesh);

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
TE3MeshComponentHdl
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
TE3MeshComponentHdl
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
	*numFaces = e3meshFaceDataArrayOrList_Length(&meshDataPtr->faceDataArrayOrList);

	return(kQ3Success);
}





//=============================================================================
//      E3Mesh_FirstMeshFace : Get first face in mesh.
//-----------------------------------------------------------------------------
//		Note : If no first face or unable to create handle to face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceHdl
E3Mesh_FirstMeshFace(TQ3GeometryObject mesh, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshFaceHdl meshFaceHdl;

	// Save mesh
	meshIteratorPtr->var4.field1 = meshDataPtr;

	// Save iterator kind
	strncpy(meshIteratorPtr->var4.field2, "mefa", 4);

	// Get and save first face in mesh
	if ((meshFaceDataPtr = e3meshFaceDataArrayOrList_FirstItem(&meshDataPtr->faceDataArrayOrList)) == NULL)
		goto failure;
	if ((meshFaceHdl = e3meshData_FaceDataToHandle(meshDataPtr, meshFaceDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshFaceHdl;

	// Return first face in mesh
	return(meshFaceHdl);

failure:
	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshFace : Get next face in mesh.
//-----------------------------------------------------------------------------
//		Note :	If iterator ended, current face deleted, no next face or
//					unable to create handle to face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceHdl
E3Mesh_NextMeshFace(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshFaceHdl meshFaceHdl;
	TE3MeshFaceData* meshFaceDataPtr;

	// Restore and check current face in mesh
	if ((meshFaceHdl = (TE3MeshFaceHdl) meshIteratorPtr->var1) == NULL)
		goto failure;
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Get and save next face in mesh
	if ((meshFaceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->faceDataArrayOrList, meshFaceDataPtr)) == NULL)
		goto failure;
	if ((meshFaceHdl = e3meshData_FaceDataToHandle(meshDataPtr, meshFaceDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshFaceHdl;

	// Return next face in mesh
	return(meshFaceHdl);

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
TE3MeshEdgeHdl
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
TE3MeshEdgeHdl
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
	*numVertices = e3meshVertexDataArrayOrList_Length(&meshDataPtr->vertexDataArrayOrList);

	return(kQ3Success);
}





//=============================================================================
//      E3Mesh_FirstMeshVertex : Get first vertex in mesh.
//-----------------------------------------------------------------------------
//		Note : If no first vertex or unable to create handle to vertex, return NULL.
//-----------------------------------------------------------------------------
TE3MeshVertexHdl
E3Mesh_FirstMeshVertex(TQ3GeometryObject mesh, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshVertexHdl meshVertexHdl;

	// Save mesh
	meshIteratorPtr->var4.field1 = meshDataPtr;

	// Save iterator kind
	strncpy(meshIteratorPtr->var4.field2, "meve", 4);

	// Get and save first vertex in mesh
	if ((meshVertexDataPtr = e3meshVertexDataArrayOrList_FirstItem(&meshDataPtr->vertexDataArrayOrList)) == NULL)
		goto failure;
	if ((meshVertexHdl = e3meshData_VertexDataToHandle(meshDataPtr, meshVertexDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshVertexHdl;

	// Return first vertex in mesh
	return(meshVertexHdl);

failure:
	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshVertex : Get next vertex in mesh.
//-----------------------------------------------------------------------------
//		Note :	If iterator ended, current vertex deleted, no next vertex
//					or unable to create handle to vertex, return NULL.
//-----------------------------------------------------------------------------
TE3MeshVertexHdl
E3Mesh_NextMeshVertex(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshVertexHdl meshVertexHdl;
	TE3MeshVertexData* meshVertexDataPtr;

	// Restore and check current vertex in mesh
	if ((meshVertexHdl = (TE3MeshVertexHdl) meshIteratorPtr->var1) == NULL)
		goto failure;
	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
		goto failure;

	// Get and save next vertex in mesh
	if ((meshVertexDataPtr = e3meshVertexDataArrayOrList_NextItem(&meshDataPtr->vertexDataArrayOrList, meshVertexDataPtr)) == NULL)
		goto failure;
	if ((meshVertexHdl = e3meshData_VertexDataToHandle(meshDataPtr, meshVertexDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshVertexHdl;

	// Return next vertex in mesh
	return(meshVertexHdl);

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
E3Mesh_GetComponentOrientable(TQ3GeometryObject mesh, TE3MeshComponentHdl meshComponentHdl, TQ3Boolean *orientable)
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
E3Mesh_GetComponentBoundingBox(TQ3GeometryObject mesh, TE3MeshComponentHdl meshComponentHdl, TQ3BoundingBox *boundingBox)
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
E3Mesh_GetComponentNumEdges(TQ3GeometryObject mesh, TE3MeshComponentHdl meshComponentHdl, TQ3Uns32 *numEdges)
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
TE3MeshEdgeHdl
E3Mesh_FirstComponentEdge(TE3MeshComponentHdl meshComponentHdl, TQ3MeshIterator* meshIteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextComponentEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeHdl
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
E3Mesh_GetComponentNumVertices(TQ3GeometryObject mesh, TE3MeshComponentHdl meshComponentHdl, TQ3Uns32 *numVertices)
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
TE3MeshVertexHdl
E3Mesh_FirstComponentVertex(TE3MeshComponentHdl meshComponentHdl, TQ3MeshIterator* meshIteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextComponentVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshVertexHdl
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
E3Mesh_GetFaceIndex(TQ3GeometryObject mesh, TE3MeshFaceHdl meshFaceHdl, TQ3Uns32 *index)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Use array of faces in mesh (*** MAY RELOCATE FACES ***)
	if (e3meshFaceDataArrayOrList_UseArray(&meshDataPtr->faceDataArrayOrList, e3meshFaceData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck face (in case relocated)
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Get index of face
	*index = e3meshFaceDataArray_ItemIndex(&meshDataPtr->faceDataArrayOrList.array, meshFaceDataPtr);

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
E3Mesh_GetFacePlaneEquation(TQ3GeometryObject mesh, TE3MeshFaceHdl meshFaceHdl, TQ3PlaneEquation *planeEquation)
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
E3Mesh_GetFaceAttributeSet(TQ3GeometryObject mesh, TE3MeshFaceHdl meshFaceHdl, TQ3AttributeSet *attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Get attribute set
	E3Shared_Acquire(attributeSet, meshFaceDataPtr->attributeSet);

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
E3Mesh_SetFaceAttributeSet(TQ3GeometryObject mesh, TE3MeshFaceHdl meshFaceHdl, TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Set attribute set
	E3Shared_Replace(&meshFaceDataPtr->attributeSet, attributeSet);

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
E3Mesh_GetFaceComponent(TQ3GeometryObject mesh, TE3MeshFaceHdl meshFaceHdl, TE3MeshComponentHdl* componentHdlPtr)
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
TE3MeshFaceHdl
E3Mesh_FirstFaceFace(TE3MeshFaceHdl meshFaceHdl, TQ3MeshIterator* meshIteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshFaceHdl
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
E3Mesh_GetFaceNumContours(TQ3GeometryObject mesh, TE3MeshFaceHdl meshFaceHdl, TQ3Uns32 *numContours)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Get number of contours in face
	*numContours = e3meshContourDataArrayOrList_Length(&meshFaceDataPtr->contourDataArrayOrList);

	return(kQ3Success);

failure:
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstFaceContour : Get first contour in face.
//-----------------------------------------------------------------------------
//		Note :	If face deleted, no first contour or unable to create
//					handle to contour, return NULL.
//-----------------------------------------------------------------------------
TE3MeshContourHdl
E3Mesh_FirstFaceContour(TE3MeshFaceHdl meshFaceHdl, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshContourHdl meshContourHdl;

	// Get mesh for face
	if ((meshDataPtr = e3meshFaceHdl_MeshData(meshFaceHdl)) == NULL)
		goto failure;

	// Save mesh
	meshIteratorPtr->var4.field1 = meshDataPtr;

	// Save iterator kind
	strncpy(meshIteratorPtr->var4.field2, "fact", 4);

	// Check and save face
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;
	meshIteratorPtr->var2 = meshFaceHdl;

	// Get and save first contour in face
	if ((meshContourDataPtr = e3meshContourDataArrayOrList_FirstItem(&meshFaceDataPtr->contourDataArrayOrList)) == NULL)
		goto failure;
	if ((meshContourHdl = e3meshData_ContourDataToHandle(meshDataPtr, meshContourDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshContourHdl;

	// Return first contour in face
	return(meshContourHdl);

failure:
	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceContour : Get next contour in face.
//-----------------------------------------------------------------------------
//		Note :	If iterator ended, face deleted, current contour deleted,
//					no next contour or unable to create handle to contour,
//					return NULL.
//-----------------------------------------------------------------------------
TE3MeshContourHdl
E3Mesh_NextFaceContour(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshFaceHdl meshFaceHdl;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshContourHdl meshContourHdl;
	TE3MeshContourData* meshContourDataPtr;

	// Restore and check face
	if ((meshFaceHdl = (TE3MeshFaceHdl) meshIteratorPtr->var2) == NULL)
		goto failure;
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Restore and check current contour in face
	if ((meshContourHdl = (TE3MeshContourHdl) meshIteratorPtr->var1) == NULL)
		goto failure;
	if ((meshContourDataPtr = e3meshContourHdl_Data(meshContourHdl)) == NULL)
		goto failure;

	// Get and save next contour in face
	if ((meshContourDataPtr = e3meshContourDataArrayOrList_NextItem(&meshFaceDataPtr->contourDataArrayOrList, meshContourDataPtr)) == NULL)
		goto failure;
	if ((meshContourHdl = e3meshData_ContourDataToHandle(meshDataPtr, meshContourDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshContourHdl;

	// Return next contour in face
	return(meshContourHdl);

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
TE3MeshEdgeHdl
E3Mesh_FirstFaceEdge(TE3MeshFaceHdl meshFaceHdl, TQ3MeshIterator* meshIteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeHdl
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
E3Mesh_GetFaceNumVertices(TQ3GeometryObject mesh, TE3MeshFaceHdl meshFaceHdl, TQ3Uns32 *numVertices)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshContourData* meshContourDataPtr;

	// Check face
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Number of vertices in face is sum of number of vertices in each contour
	*numVertices = 0;
	for (meshContourDataPtr = e3meshContourDataArrayOrList_FirstItem(&meshFaceDataPtr->contourDataArrayOrList);
		meshContourDataPtr != NULL;
		meshContourDataPtr = e3meshContourDataArrayOrList_NextItem(&meshFaceDataPtr->contourDataArrayOrList, meshContourDataPtr))
	{
		*numVertices += e3meshVertexHdlArray_Length(&meshContourDataPtr->vertexHdlArray);
	}

	return(kQ3Success);

failure:
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstFaceVertex : Get first vertex in face.
//-----------------------------------------------------------------------------
//		Note :	If face deleted, no first vertex or unable to create
//					handle to vertex, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexHdl
E3Mesh_FirstFaceVertex(TE3MeshFaceHdl meshFaceHdl, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshContourHdl meshContourHdl;
	TE3MeshVertexHdl* meshVertexHdlPtr;

	// Get mesh for face
	if ((meshDataPtr = e3meshFaceHdl_MeshData(meshFaceHdl)) == NULL)
		goto failure;

	// Save mesh
	meshIteratorPtr->var4.field1 = meshDataPtr;

	// Save iterator kind
	strncpy(meshIteratorPtr->var4.field2, "fave", 4);

	// Check and save face
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;
	meshIteratorPtr->var3 = meshFaceHdl;

	// Get first contour in face
	if ((meshContourDataPtr = e3meshContourDataArrayOrList_FirstItem(&meshFaceDataPtr->contourDataArrayOrList)) == NULL)
		goto failure;

	// Get first vertex in face
	for (;;)
	{
		// If first vertex in current contour exists, break
		if ((meshVertexHdlPtr = e3meshVertexHdlArray_FirstItem(&meshContourDataPtr->vertexHdlArray)) != NULL)
			break;

		// Otherwise, get next contour in face
		if ((meshContourDataPtr = e3meshContourDataArrayOrList_NextItem(&meshFaceDataPtr->contourDataArrayOrList, meshContourDataPtr)) == NULL)
			goto failure;
	}

	// Save current contour in face
	if ((meshContourHdl = e3meshData_ContourDataToHandle(meshDataPtr, meshContourDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var2 = meshContourHdl;

	// Save first vertex in face
	meshIteratorPtr->var1 = meshVertexHdlPtr;

	// Return first vertex in face
	return(*meshVertexHdlPtr);

failure:
	meshIteratorPtr->var3 = NULL;
	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceVertex : Get next vertex in face.
//-----------------------------------------------------------------------------
//		Note :	If iterator ended, face deleted, current vertex deleted,
//					no next vertex or unable to create handle to vertex,
//					return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence
//				is iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexHdl
E3Mesh_NextFaceVertex(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshFaceHdl meshFaceHdl;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshContourHdl meshContourHdl;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshVertexHdl* meshVertexHdlPtr;

	// Restore and check face
	if ((meshFaceHdl = (TE3MeshFaceHdl) meshIteratorPtr->var3) == NULL)
		goto failure;
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Restore and check current contour
	if ((meshContourHdl = (TE3MeshContourHdl) meshIteratorPtr->var2) == NULL)
		goto failure;
	if ((meshContourDataPtr = e3meshContourHdl_Data(meshContourHdl)) == NULL)
		goto failure;

	// ??? check that contour still belongs to face ???

	// Restore and check current vertex
	if ((meshVertexHdlPtr = (TE3MeshVertexHdl*) meshIteratorPtr->var1) == NULL)
		goto failure;

	// If next vertex in current contour exists, break
	if ((meshVertexHdlPtr = e3meshVertexHdlArray_NextItem(&meshContourDataPtr->vertexHdlArray, meshVertexHdlPtr)) != NULL)
		;
	// Otherwise, get next vertex in face
	else
	{
		for (;;)
		{
			// Get next contour in face
			if ((meshContourDataPtr = e3meshContourDataArrayOrList_NextItem(&meshFaceDataPtr->contourDataArrayOrList, meshContourDataPtr)) == NULL)
				goto failure;
				
			// If first vertex in current contour exists, break
			if ((meshVertexHdlPtr = e3meshVertexHdlArray_FirstItem(&meshContourDataPtr->vertexHdlArray)) != NULL)
				break;
		}
	}

	// Save current contour in face
	if ((meshContourHdl = e3meshData_ContourDataToHandle(meshDataPtr, meshContourDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var2 = meshContourHdl;

	// Save next vertex in face
	meshIteratorPtr->var1 = meshVertexHdlPtr;

	// Return next vertex in face
	return(*meshVertexHdlPtr);

failure:
	meshIteratorPtr->var3 = NULL;
	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetContourFace : Get face containing contour.
//-----------------------------------------------------------------------------
//		Note :	If contour deleted or If unable to get handle (out of
//					memory), return kQ3Failure.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Mesh_GetContourFace(TQ3GeometryObject mesh, TE3MeshContourHdl meshContourHdl, TE3MeshFaceHdl* faceHdlPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshContourData* meshContourDataPtr;

	// Check contour
	if ((meshContourDataPtr = e3meshContourHdl_Data(meshContourHdl)) == NULL)
		goto failure;
	
	// Get handle to container face
	*faceHdlPtr = e3meshContourData_ContainerFaceHdl(meshContourDataPtr);
	
	return(kQ3Success);

failure:
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstContourFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshFaceHdl
E3Mesh_FirstContourFace(TE3MeshContourHdl meshContourHdl, TQ3MeshIterator* meshIteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextContourFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshFaceHdl
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
TE3MeshEdgeHdl
E3Mesh_FirstContourEdge(TE3MeshContourHdl meshContourHdl, TQ3MeshIterator* meshIteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextContourEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeHdl
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
E3Mesh_GetContourNumVertices(TQ3GeometryObject mesh, TE3MeshContourHdl meshContourHdl, TQ3Uns32 *numVertices)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshContourData* meshContourDataPtr;

	// Check contour
	if ((meshContourDataPtr = e3meshContourHdl_Data(meshContourHdl)) == NULL)
		goto failure;

	// Get number of vertices in contour
	*numVertices = e3meshVertexHdlArray_Length(&meshContourDataPtr->vertexHdlArray);

	return(kQ3Success);

failure:
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstContourVertex : Get first vertex in contour.
//-----------------------------------------------------------------------------
//		Note :	If contour deleted, no first vertex or unable to create
//					handle to vertex, return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence is
//				iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexHdl
E3Mesh_FirstContourVertex(TE3MeshContourHdl meshContourHdl, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshVertexHdl* meshVertexHdlPtr;

	// Get mesh for contour
	if ((meshDataPtr = e3meshContourHdl_MeshData(meshContourHdl)) == NULL)
		goto failure;

	// Save mesh
	meshIteratorPtr->var4.field1 = meshDataPtr;

	// Save iterator kind
	strncpy(meshIteratorPtr->var4.field2, "ctve", 4);

	// Check and save contour
	if ((meshContourDataPtr = e3meshContourHdl_Data(meshContourHdl)) == NULL)
		goto failure;
	meshIteratorPtr->var2 = meshContourHdl;

	// Get and save first vertex in contour
	if ((meshVertexHdlPtr = e3meshVertexHdlArray_FirstItem(&meshContourDataPtr->vertexHdlArray)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshVertexHdlPtr;

	// Return first vertex in contour
	return(*meshVertexHdlPtr);

failure:
	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextContourVertex : Get next vertex in contour.
//-----------------------------------------------------------------------------
//		Note :	If iterator ended, contour deleted, current vertex deleted,
//					no next vertex or unable to create handle to vertex,
//					return NULL.
//-----------------------------------------------------------------------------
//		Note :	If a particular vertex occurs multiple times, each occurrence is
//				iterated separately.
//-----------------------------------------------------------------------------
TE3MeshVertexHdl
E3Mesh_NextContourVertex(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshContourHdl meshContourHdl;
	TE3MeshContourData* meshContourDataPtr;
	TE3MeshVertexHdl* meshVertexHdlPtr;

	// Restore and check contour
	if ((meshContourHdl = (TE3MeshContourHdl) meshIteratorPtr->var2) == NULL)
		goto failure;
	if ((meshContourDataPtr = e3meshContourHdl_Data(meshContourHdl)) == NULL)
		goto failure;

	// Restore and check current vertex in contour
	if ((meshVertexHdlPtr = (TE3MeshVertexHdl*) meshIteratorPtr->var1) == NULL)
		goto failure;

	// Get and save next vertex in contour
	if ((meshVertexHdlPtr = e3meshVertexHdlArray_NextItem(&meshContourDataPtr->vertexHdlArray, meshVertexHdlPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshVertexHdlPtr;

	// Return next vertex in contour
	return(*meshVertexHdlPtr);

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
E3Mesh_GetEdgeOnBoundary(TQ3GeometryObject mesh, TE3MeshEdgeHdl meshEdge, TQ3Boolean *onBoundary)
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
E3Mesh_GetEdgeAttributeSet(TQ3GeometryObject mesh, TE3MeshEdgeHdl meshEdge, TQ3AttributeSet *attributeSet)
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
E3Mesh_SetEdgeAttributeSet(TQ3GeometryObject mesh, TE3MeshEdgeHdl meshEdge, TQ3AttributeSet attributeSet)
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
E3Mesh_GetEdgeComponent(TQ3GeometryObject mesh, TE3MeshEdgeHdl meshEdge, TE3MeshComponentHdl* componentHdlPtr)
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
E3Mesh_GetEdgeFaces(TQ3GeometryObject mesh, TE3MeshEdgeHdl meshEdge, TE3MeshFaceHdl* faceHdlPtr1, TE3MeshFaceHdl* faceHdlPtr2)
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
E3Mesh_GetEdgeVertices(TQ3GeometryObject mesh, TE3MeshEdgeHdl meshEdge,
	TE3MeshVertexHdl* meshVertexHdlPtr1, TE3MeshVertexHdl* meshVertexHdlPtr2)
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
E3Mesh_GetVertexIndex(TQ3GeometryObject mesh, TE3MeshVertexHdl meshVertexHdl, TQ3Uns32 *index)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;

	// Check vertex
	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
		goto failure;

	// Use array of vertices in mesh (*** MAY RELOCATE VERTICES ***)
	if (e3meshVertexDataArrayOrList_UseArray(&meshDataPtr->vertexDataArrayOrList, e3meshVertexData_Relocate) == kQ3Failure)
		goto failure;

	// Recheck vertex (in case relocated)
	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
		goto failure;

	// Get index of vertex
	*index = e3meshVertexDataArray_ItemIndex(&meshDataPtr->vertexDataArrayOrList.array, meshVertexDataPtr);

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
E3Mesh_GetVertexOnBoundary(TQ3GeometryObject mesh, TE3MeshVertexHdl meshVertexHdl, TQ3Boolean *onBoundary)
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
E3Mesh_GetVertexCoordinates(TQ3GeometryObject mesh, TE3MeshVertexHdl meshVertexHdl, TQ3Point3D *coordinates)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;

	// Check vertex
	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
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
E3Mesh_SetVertexCoordinates(TQ3GeometryObject mesh, TE3MeshVertexHdl meshVertexHdl, const TQ3Point3D *coordinates)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;

	// Check vertex
	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
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
E3Mesh_GetVertexAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexHdl meshVertexHdl, TQ3AttributeSet *attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;

	// Check vertex
	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
		goto failure;

	// Get vertex attribute set
	E3Shared_Acquire(attributeSet, meshVertexDataPtr->attributeSet);

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
E3Mesh_SetVertexAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexHdl meshVertexHdl, TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);
	TE3MeshVertexData* meshVertexDataPtr;

	// Check vertex
	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
		goto failure;

	// Set vertex attribute set
	E3Shared_Replace(&meshVertexDataPtr->attributeSet, attributeSet);

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
E3Mesh_GetVertexComponent(TQ3GeometryObject mesh, TE3MeshVertexHdl meshVertexHdl, TE3MeshComponentHdl* componentHdlPtr)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstVertexFace : Get first face having vertex.
//-----------------------------------------------------------------------------
//		Note :	If vertex deleted, no first face or unable to create
//					handle to face, return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceHdl
E3Mesh_FirstVertexFace(TE3MeshVertexHdl meshVertexHdl, TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr;
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshFaceData* meshFaceDataPtr;
	TE3MeshFaceHdl meshFaceHdl;

	// Get mesh for vertex
	if ((meshDataPtr = e3meshVertexHdl_MeshData(meshVertexHdl)) == NULL)
		goto failure;

	// Save mesh
	meshIteratorPtr->var4.field1 = meshDataPtr;

	// Save iterator kind
	strncpy(meshIteratorPtr->var4.field2, "vefa", 4);

	// Check and save vertex
	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
		goto failure;
	meshIteratorPtr->var2 = meshVertexHdl;

	// Get first face in mesh
	if ((meshFaceDataPtr = e3meshFaceDataArrayOrList_FirstItem(&meshDataPtr->faceDataArrayOrList)) == NULL)
		goto failure;

	// Get first face having vertex
	for (;;)
	{
		// If face has vertex, break
		if (e3meshFaceData_HasVertexHdl(meshFaceDataPtr, meshVertexHdl))
			break;

		// Otherwise, get next face in mesh
		if ((meshFaceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->faceDataArrayOrList, meshFaceDataPtr)) == NULL)
			goto failure;
	}

	// Save first face having vertex
	if ((meshFaceHdl = e3meshData_FaceDataToHandle(meshDataPtr, meshFaceDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshFaceHdl;

	// Return first face having vertex
	return(meshFaceHdl);

failure:
	meshIteratorPtr->var2 = NULL;
	meshIteratorPtr->var1 = NULL;
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextVertexFace : Get next face having vertex.
//-----------------------------------------------------------------------------
//		Note :	If iterator ended, vertex deleted, current face deleted,
//					no next face or unable to create handle to face,
//					return NULL.
//-----------------------------------------------------------------------------
TE3MeshFaceHdl
E3Mesh_NextVertexFace(TQ3MeshIterator* meshIteratorPtr)
{
	TE3MeshData* meshDataPtr = meshIteratorPtr->var4.field1;
	TE3MeshVertexHdl meshVertexHdl;
	TE3MeshVertexData* meshVertexDataPtr;
	TE3MeshFaceHdl meshFaceHdl;
	TE3MeshFaceData* meshFaceDataPtr;

	// Restore and check vertex
	if ((meshVertexHdl = (TE3MeshVertexHdl) meshIteratorPtr->var2) == NULL)
		goto failure;
	if ((meshVertexDataPtr = e3meshVertexHdl_Data(meshVertexHdl)) == NULL)
		goto failure;

	// Restore and check current face having vertex
	if ((meshFaceHdl = (TE3MeshFaceHdl) meshIteratorPtr->var1) == NULL)
		goto failure;
	if ((meshFaceDataPtr = e3meshFaceHdl_Data(meshFaceHdl)) == NULL)
		goto failure;

	// Get next face having vertex
	for (;;)
	{
		// Get next face in mesh
		if ((meshFaceDataPtr = e3meshFaceDataArrayOrList_NextItem(&meshDataPtr->faceDataArrayOrList, meshFaceDataPtr)) == NULL)
			goto failure;

		// If face has vertex, break
		if (e3meshFaceData_HasVertexHdl(meshFaceDataPtr, meshVertexHdl))
			break;
	}

	// Save next face having vertex
	if ((meshFaceHdl = e3meshData_FaceDataToHandle(meshDataPtr, meshFaceDataPtr)) == NULL)
		goto failure;
	meshIteratorPtr->var1 = meshFaceHdl;

	// Return next face having vertex
	return(meshFaceHdl);

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
TE3MeshEdgeHdl
E3Mesh_FirstVertexEdge(TE3MeshVertexHdl meshVertexHdl, TQ3MeshIterator* meshIteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextVertexEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshEdgeHdl
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
TE3MeshVertexHdl
E3Mesh_FirstVertexVertex(TE3MeshVertexHdl meshVertexHdl, TQ3MeshIterator* meshIteratorPtr)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextVertexVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TE3MeshVertexHdl
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
E3Mesh_GetCornerAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexHdl meshVertexHdl, TE3MeshFaceHdl meshFaceHdl, TQ3AttributeSet *attributeSet)
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
E3Mesh_SetCornerAttributeSet(TQ3GeometryObject mesh, TE3MeshVertexHdl meshVertexHdl, TE3MeshFaceHdl meshFaceHdl, TQ3AttributeSet attributeSet)
{
	TE3MeshData* meshDataPtr = (TE3MeshData *) E3ClassTree_FindInstanceData(mesh, kQ3GeometryTypeMesh);


	// To be implemented...
	Q3Shared_Edited(mesh);

	return(kQ3Failure);
}


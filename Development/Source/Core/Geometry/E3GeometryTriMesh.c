/*  NAME:
        E3GeometryTriMesh.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3View.h"
#include "E3Set.h"
#include "E3Math.h"
#include "E3Geometry.h"
#include "E3GeometryTriMesh.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_trimesh_clone : Clone a block of memory.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_clone(void *srcPtr, void **dstPtr, TQ3Uns32 theSize)
{


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(srcPtr  != NULL, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(dstPtr  != NULL, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(theSize != 0,    kQ3Failure);



	// Alocate the memory	
	*dstPtr = E3Memory_Allocate(theSize);
	if (*dstPtr == NULL)
		return(kQ3Failure);



	// Copy it over
	memcpy(*dstPtr, srcPtr, theSize);
	
	return(kQ3Success);
}





//=============================================================================
//		e3geom_trimesh_disposeattributes: Free a TQ3TriMeshAttributeData array.
//-----------------------------------------------------------------------------
static void
e3geom_trimesh_disposeattributes(TQ3Uns32						numAttributeTypes,
									TQ3TriMeshAttributeData		**attributeTypes)
{
	TQ3Uns32 i;
	
	if (*attributeTypes != NULL)
		{
		for (i=0; i<numAttributeTypes; i++)
			{
			E3Memory_Free( &((*attributeTypes)[i].data) );
			E3Memory_Free( &((*attributeTypes)[i].attributeUseArray) );
			}
		}

	E3Memory_Free( attributeTypes );
}





//=============================================================================
//      e3geom_trimesh_copyattributes : Copy a TQ3TriMeshAttributeData array.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_copyattributes(TQ3Uns32						numAttributeTypes,
								TQ3Uns32					numElements,
								TQ3TriMeshAttributeData		*srcAttributeTypes,
								TQ3TriMeshAttributeData		**destAttributeTypes)
{
	TQ3Uns32				i, bytes, attrSize;	
	TQ3Status				qd3dStatus;
	TQ3AttributeType		attrType;
	E3ClassInfoPtr			theClass;

	if (numAttributeTypes < 1) {
		*destAttributeTypes = NULL;
		return(kQ3Success);
	}
	
	qd3dStatus = e3geom_trimesh_clone(srcAttributeTypes,
									  (void **) destAttributeTypes,
									  numAttributeTypes * sizeof(TQ3TriMeshAttributeData));
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);

	for (i=0; i<numAttributeTypes && qd3dStatus == kQ3Success; i++)
		{
		// We must make a copy of the data elements; but first, we must figure
		// out how big they are (depends on the attribute type)
		attrType = srcAttributeTypes[i].attributeType;
		attrType = E3Attribute_AttributeToClassType(attrType);
		theClass = E3ClassTree_GetClassByType(attrType);
		if (theClass != NULL)
			{
			// Copy the attribute data
			attrSize = E3ClassTree_GetInstanceSize(theClass);
			bytes    = numElements * attrSize;
			if (bytes != 0 )
				qd3dStatus = e3geom_trimesh_clone(srcAttributeTypes[i].data,
												  &(*destAttributeTypes)[i].data,
												  bytes);
			else
				(*destAttributeTypes)[i].data = NULL;


			// Copy the custom attribute useage flags
			bytes = numElements * sizeof(char);
			if (bytes != 0 && srcAttributeTypes[i].attributeUseArray != NULL)
				qd3dStatus = e3geom_trimesh_clone(srcAttributeTypes[i].attributeUseArray,
												  (void **) &(*destAttributeTypes)[i].attributeUseArray,
												  bytes);
			else
				(*destAttributeTypes)[i].attributeUseArray = NULL;
			}
		}
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_disposedata : Dispose of a TQ3TriMeshData.
//-----------------------------------------------------------------------------
static void
e3geom_trimesh_disposedata(TQ3TriMeshData *theTriMesh)
{


	// Dispose of the TriMesh
	E3Object_DisposeAndForget( theTriMesh->triMeshAttributeSet );

	E3Memory_Free( &theTriMesh->triangles );
	e3geom_trimesh_disposeattributes( theTriMesh->numTriangleAttributeTypes,
									  &theTriMesh->triangleAttributeTypes );

	E3Memory_Free( &theTriMesh->edges );
	e3geom_trimesh_disposeattributes( theTriMesh->numEdgeAttributeTypes,
									  &theTriMesh->edgeAttributeTypes );

	E3Memory_Free( &theTriMesh->points );
	e3geom_trimesh_disposeattributes( theTriMesh->numVertexAttributeTypes,
									  &theTriMesh->vertexAttributeTypes );
}





//=============================================================================
//      e3geom_trimesh_copydata : Copy TQ3TriMeshData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_copydata(const TQ3TriMeshData *src, TQ3TriMeshData *dst, TQ3Boolean isDuplicate)
{	TQ3Status		qd3dStatus = kQ3Success;
	TQ3Uns32		n;



	// 0. Initialise ourselves
	E3Memory_Clear(dst, sizeof(TQ3TriMeshData));



	// 1. triMeshAttributeSet
	if (isDuplicate)
		{
		if (src->triMeshAttributeSet != NULL)
			{
			dst->triMeshAttributeSet = Q3Object_Duplicate(src->triMeshAttributeSet);
			if (dst->triMeshAttributeSet == NULL)
				qd3dStatus = kQ3Failure;
			}
		}
	else
		E3Shared_Replace(&dst->triMeshAttributeSet, src->triMeshAttributeSet);



	// 2. triangles
	if (qd3dStatus == kQ3Success)
		{
		n = src->numTriangles;
		if (n)
			{
			qd3dStatus = e3geom_trimesh_clone( src->triangles, (void **) &dst->triangles,
										       n * sizeof(TQ3TriMeshTriangleData) );
			if (qd3dStatus == kQ3Success)
				dst->numTriangles = n;
			}
		}



	// 3. triangleAttributeTypes
	if (qd3dStatus == kQ3Success)
		{
		qd3dStatus = e3geom_trimesh_copyattributes( src->numTriangleAttributeTypes, n,
							 					    src->triangleAttributeTypes,
												   &dst->triangleAttributeTypes );
		if (qd3dStatus == kQ3Success)
			dst->numTriangleAttributeTypes = src->numTriangleAttributeTypes;
		}



	// 4. edges
	if (qd3dStatus == kQ3Success)
		{
		n = src->numEdges;
		if (n)
			{
			qd3dStatus = e3geom_trimesh_clone( src->edges, (void **) &dst->edges,
										       n * sizeof(TQ3TriMeshEdgeData) );
			if (qd3dStatus == kQ3Success)
				dst->numEdges = n;
			}
		}



	// 5. edgeAttributesTypes
	if (qd3dStatus == kQ3Success)
		{
		qd3dStatus = e3geom_trimesh_copyattributes( src->numEdgeAttributeTypes, n,
												    src->edgeAttributeTypes,
												   &dst->edgeAttributeTypes );	
		if (qd3dStatus == kQ3Success)
			dst->numEdgeAttributeTypes = src->numEdgeAttributeTypes;
		}



	// 6. points (vertices)
	if (qd3dStatus == kQ3Success)
		{
		n = src->numPoints;
		if (n)
			{
			qd3dStatus = e3geom_trimesh_clone( src->points, (void **) &dst->points, n * sizeof(TQ3Point3D) );
			if (qd3dStatus == kQ3Success)
				dst->numPoints = n;
			}
		}



	// 7. vertexAttributeTypes
	if (qd3dStatus == kQ3Success)
		{
		qd3dStatus = e3geom_trimesh_copyattributes( src->numVertexAttributeTypes, n,
												    src->vertexAttributeTypes,
												   &dst->vertexAttributeTypes );
		if (qd3dStatus == kQ3Success)
			dst->numVertexAttributeTypes = src->numVertexAttributeTypes;
		}



	// 8. bounding box
	if (qd3dStatus == kQ3Success)
		E3BoundingBox_Copy( &src->bBox, &dst->bBox );



	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_trimesh_disposedata(dst);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_new : TriMesh new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3TriMeshData			*instanceData  = (TQ3TriMeshData *)       privateData;
	const TQ3TriMeshData	*trimeshData  = (const TQ3TriMeshData *) paramData;
	TQ3Status				qd3dStatus;
#pragma unused(theObject)



	qd3dStatus = e3geom_trimesh_copydata(trimeshData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_delete : TriMesh delete method.
//-----------------------------------------------------------------------------
static void
e3geom_trimesh_delete(TQ3Object theObject, void *privateData)
{	TQ3TriMeshData		*instanceData = (TQ3TriMeshData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	e3geom_trimesh_disposedata(instanceData);
}





//=============================================================================
//      e3geom_trimesh_duplicate : TriMesh duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,  void       *toPrivateData)
{	const TQ3TriMeshData		*fromData = (const TQ3TriMeshData *) fromPrivateData;
	TQ3TriMeshData				*toData   = (TQ3TriMeshData *)       toPrivateData;
	TQ3Status					qd3dStatus;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_trimesh_copydata( fromData, toData, kQ3True );

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_trimesh_cache_new : TriMesh cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_trimesh_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3TriMeshData *geomData)
{	TQ3Uns32				n, v, a, vertexIdx;
	TQ3TriangleData			triangleData;
	TQ3GeometryObject		theTriangle;
	TQ3GroupObject			theGroup;
	TQ3AttributeType 		attrType;
	TQ3Uns32				attrSize;
	E3ClassInfoPtr			theClass;
#pragma unused(theView)



	// Create a group to hold the cached geometry
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Add the cached form to the group
	triangleData.vertices[0].attributeSet = NULL;
	triangleData.vertices[1].attributeSet = NULL;
	triangleData.vertices[2].attributeSet = NULL;

	for (n = 0; n < geomData->numTriangles; n++)
		{
		// Set up the triangle
		for (v=0; v<3; v++)
			{
			// copy the vertex location
			vertexIdx = geomData->triangles[n].pointIndices[v];
			triangleData.vertices[v].point = geomData->points[vertexIdx];

			// copy the vertex attribute set, if any
			if (geomData->numVertexAttributeTypes != 0)
				{
				Q3_ASSERT(Q3_VALID_PTR(geomData->vertexAttributeTypes));
				triangleData.vertices[v].attributeSet = Q3AttributeSet_New();
				if (triangleData.vertices[v].attributeSet != NULL)
					{
					for (a=0; a<geomData->numVertexAttributeTypes; a++)
						{
						attrType = geomData->vertexAttributeTypes[a].attributeType;
						attrType = E3Attribute_AttributeToClassType(attrType);
						theClass = E3ClassTree_GetClassByType(attrType);
						if (theClass != NULL)
							{
							attrSize = E3ClassTree_GetInstanceSize(theClass);
							Q3AttributeSet_Add(triangleData.vertices[v].attributeSet,
												attrType,
												(char*)geomData->vertexAttributeTypes[a].data + vertexIdx*attrSize);
							}
						}
					}
				}
			}
			
		// finally, copy the trimesh attribute set, if any
		triangleData.triangleAttributeSet = geomData->triMeshAttributeSet;



		// Create the triangle
		theTriangle = Q3Triangle_New(&triangleData);
		if (theTriangle != NULL)
			{
			Q3Group_AddObject(theGroup, theTriangle);
			Q3Object_Dispose(theTriangle);
			}

			
		// dispose of vertex attributes, if any
		if (geomData->numVertexAttributeTypes != 0)
			{
			for (v=0; v<3; v++)
				Q3Object_Dispose(triangleData.vertices[v].attributeSet);
			}
		}



	// Finish off the group state (in-line, since we don't make any view state changes)
	Q3DisplayGroup_SetState(theGroup, kQ3DisplayGroupStateMaskIsInline |
									  kQ3DisplayGroupStateMaskIsDrawn  |
									  kQ3DisplayGroupStateMaskIsWritten |
									  kQ3DisplayGroupStateMaskIsPicked);

	return(theGroup);
}





//=============================================================================
//      e3geom_trimesh_pick : TriMesh picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_trimesh_bounds : TriMesh bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_trimesh_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3TriMeshData		*instanceData = (TQ3TriMeshData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)

	//calculate bBox if not set
	if(instanceData->bBox.isEmpty == kQ3True)
		Q3BoundingBox_SetFromPoints3D (&instanceData->bBox, instanceData->points, instanceData->numPoints, sizeof(TQ3Point3D));


	// Update the bounds
	E3View_UpdateBounds(theView, 2, sizeof(TQ3Point3D), &instanceData->bBox.min);
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_trimesh_get_attribute : TriMesh get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_trimesh_get_attribute(TQ3GeometryObject theObject)
{	TQ3TriMeshData		*instanceData = (TQ3TriMeshData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->triMeshAttributeSet);
}





//=============================================================================
//      e3geom_trimesh_metahandler : TriMesh metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_trimesh_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_trimesh_get_attribute;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryTriMesh_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryTriMesh_RegisterClass(void)
{	TQ3Status		qd3dStatus;


	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeTriMesh,
											kQ3ClassNameGeometryTriMesh,
											e3geom_trimesh_metahandler,
											sizeof(TQ3TriMeshData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryTriMesh_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryTriMesh_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeTriMesh, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3TriMesh_New : Create a TriMesh object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3TriMesh_New(const TQ3TriMeshData *triMeshData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeTriMesh, kQ3False, triMeshData);
	return(theObject);
}





//=============================================================================
//      E3TriMesh_Submit : Submit a TriMesh.
//-----------------------------------------------------------------------------
TQ3Status
E3TriMesh_Submit(const TQ3TriMeshData *triMeshData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeTriMesh, triMeshData);
	return(qd3dStatus);
}





//=============================================================================
//      E3TriMesh_SetData : Set the data for a TriMesh object.
//-----------------------------------------------------------------------------
TQ3Status
E3TriMesh_SetData(TQ3GeometryObject triMesh, const TQ3TriMeshData *triMeshData)
{	TQ3TriMeshData		*instanceData = (TQ3TriMeshData *) triMesh->instanceData;
	TQ3Status			qd3dStatus;



	// first, free the old data
	e3geom_trimesh_disposedata(instanceData);



	// then copy in the new data
	qd3dStatus = e3geom_trimesh_copydata(triMeshData, instanceData, kQ3False);

	Q3Shared_Edited(triMesh);

	return(qd3dStatus);
}





//=============================================================================
//      E3TriMesh_GetData : Return the data for a TriMesh object.
//-----------------------------------------------------------------------------
TQ3Status
E3TriMesh_GetData(TQ3GeometryObject triMesh, TQ3TriMeshData *triMeshData)
{	TQ3TriMeshData		*instanceData = (TQ3TriMeshData *) triMesh->instanceData;
	TQ3Status			qd3dStatus;
	


	// Copy the data out of the TriMesh
	triMeshData->triMeshAttributeSet = NULL;
	qd3dStatus = e3geom_trimesh_copydata(instanceData, triMeshData, kQ3False);

	return(qd3dStatus);
}



//=============================================================================
//      E3TriMesh_EmptyData : Empty the data for a TriMesh object.
//-----------------------------------------------------------------------------
TQ3Status
E3TriMesh_EmptyData(TQ3TriMeshData *triMeshData)
{


	// Dispose of the data
	e3geom_trimesh_disposedata(triMeshData);

	return(kQ3Success);
}


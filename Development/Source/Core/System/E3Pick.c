/*  NAME:
        E3Pick.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
#include "E3Pick.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct {
	// Common data
	TQ3Uns32							numHits;
	TQ3PickedData*						pickedData;
	
	// Pick specific. Note that we assume that a TQ3PickData
	// structure is the first item within each pick structure,
	// since this lets us use a union and have the common data
	// overlap.
	union	{
		TQ3PickData 					common;
		TQ3WindowPointPickData			windowPointData;
		TQ3WindowRectPickData			windowRectData;
		TQ3WorldRayPickData				worldRayData;
	} data;
} TQ3PickUnionData;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3pick_windowpoint_new : Window point pick new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3pick_windowpoint_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PickUnionData				*instanceData = (TQ3PickUnionData *) privateData;
	const TQ3WindowPointPickData	*pickData   = (const TQ3WindowPointPickData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->numHits				= 0;
	instanceData->pickedData			= NULL;
	instanceData->data.windowPointData 	= *pickData;

	return(kQ3Success);
}





//=============================================================================
//      e3pick_windowpoint_metahandler : Window point pick metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3pick_windowpoint_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3pick_windowpoint_new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3pick_windowrect_new : Window rect pick new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3pick_windowrect_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PickUnionData				*instanceData = (TQ3PickUnionData *) privateData;
	const TQ3WindowRectPickData		*pickData   = (const TQ3WindowRectPickData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->numHits			  = 0;
	instanceData->pickedData		  = NULL;
	instanceData->data.windowRectData = *pickData;

	return(kQ3Success);
}





//=============================================================================
//      e3pick_windowrect_metahandler : Window rect pick metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3pick_windowrect_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3pick_windowrect_new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3pick_worldray_new : World ray pick new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3pick_worldray_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PickUnionData				*instanceData = (TQ3PickUnionData *) privateData;
	const TQ3WorldRayPickData		*pickData   = (const TQ3WorldRayPickData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->numHits			= 0;
	instanceData->pickedData		= NULL;
	instanceData->data.worldRayData	= *pickData;

	return(kQ3Success);
}





//=============================================================================
//      e3pick_worldray_metahandler : World ray pick metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3pick_worldray_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3pick_worldray_new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3shapepart_new : Shape part new method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3shapepart_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3ShapeObject				*instanceData = (TQ3ShapeObject *)       privateData;
	const TQ3ShapeObject		*shape        = (const TQ3ShapeObject *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	if (*shape)
		*instanceData = Q3Shared_GetReference(*shape);
	else
		*instanceData = NULL;
	return(kQ3Success);
}





//=============================================================================
//      e3shapepart_delete : Shape part delete method.
//-----------------------------------------------------------------------------
static void
e3shapepart_delete(TQ3Object theObject, void *privateData)
{	TQ3ShapeObject				*instanceData = (TQ3ShapeObject *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	if (*instanceData)
		Q3Object_Dispose(*instanceData);
}





//=============================================================================
//      e3shapepart_metahandler : Shape part metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3shapepart_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3shapepart_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3shapepart_delete;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3meshpart__new : Mesh part new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshpart__new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3MeshComponent				*instanceData = (TQ3MeshComponent *)       privateData;
	const TQ3MeshComponent			*data         = (const TQ3MeshComponent *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	*instanceData = *data;
	return(kQ3Success);
}





//=============================================================================
//      e3meshpart_metahandler : Mesh part metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3meshpart_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3meshpart__new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3meshpart_face__new : Face mesh part new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshpart_face__new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3MeshFace				*instanceData = (TQ3MeshFace *)       privateData;
	const TQ3MeshFace		*data         = (const TQ3MeshFace *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	*instanceData = *data;
	return(kQ3Success);
}





//=============================================================================
//      e3meshpart_face_metahandler : Face mesh part metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3meshpart_face_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3meshpart_face__new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3meshpart_edge_new : Edge mesh part new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshpart_edge_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3MeshEdge				*instanceData = (TQ3MeshEdge *)       privateData;
	const TQ3MeshEdge		*data         = (const TQ3MeshEdge *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	*instanceData = *data;
	return(kQ3Success);
}





//=============================================================================
//      e3meshpart_edge_metahandler : Edge mesh part metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3meshpart_edge_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3meshpart_edge_new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3meshpart_vertex_new : Vertex mesh part new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3meshpart_vertex_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3MeshVertex				*instanceData = (TQ3MeshVertex *)       privateData;
	const TQ3MeshVertex			*data         = (const TQ3MeshVertex *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	*instanceData = *data;
	return(kQ3Success);
}





//=============================================================================
//      e3meshpart_vertex_metahandler : Vertex mesh part metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3meshpart_vertex_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3meshpart_vertex_new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Pick_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Pick_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the pick classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3ObjectTypePick,
											kQ3ClassNamePick,
											NULL,
											0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypePick,
												kQ3PickTypeWindowPoint,
												kQ3ClassNamePickWindowPoint,
												e3pick_windowpoint_metahandler,
												sizeof(TQ3PickUnionData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypePick,
												kQ3PickTypeWindowRect,
												kQ3ClassNamePickWindowRect,
												e3pick_windowrect_metahandler,
												sizeof(TQ3PickUnionData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypePick,
												kQ3PickTypeWorldRay,
												kQ3ClassNamePickWorldRay,
												e3pick_worldray_metahandler,
												sizeof(TQ3PickUnionData));
	
	//----------------------------------------------------------------------------------
	
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeShape,
												kQ3SharedTypeShapePart,
												kQ3ClassNameShapePart,
												e3shapepart_metahandler,
												sizeof(TQ3ShapeObject));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeShapePart,
												kQ3ShapePartTypeMeshPart,
												kQ3ClassNameMeshPart,
												e3meshpart_metahandler,
												sizeof(TQ3MeshComponent));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapePartTypeMeshPart,
												kQ3MeshPartTypeMeshFacePart,
												kQ3ClassNameMeshPartFace,
												e3meshpart_face_metahandler,
												sizeof(TQ3MeshFace));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapePartTypeMeshPart,
												kQ3MeshPartTypeMeshEdgePart,
												kQ3ClassNameMeshPartEdge,
												e3meshpart_edge_metahandler,
												sizeof(TQ3MeshEdge));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapePartTypeMeshPart,
												kQ3MeshPartTypeMeshVertexPart,
												kQ3ClassNameMeshPartVertex,
												e3meshpart_vertex_metahandler,
												sizeof(TQ3MeshVertex));

	return(qd3dStatus);
}





//=============================================================================
//      E3Pick_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class in reverse order
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3MeshPartTypeMeshVertexPart,	kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3MeshPartTypeMeshEdgePart,	kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3MeshPartTypeMeshFacePart,	kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShapePartTypeMeshPart,		kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3SharedTypeShapePart,		kQ3True);

	qd3dStatus = E3ClassTree_UnregisterClass(kQ3PickTypeWorldRay,			kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3PickTypeWindowRect,			kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3PickTypeWindowPoint,		kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypePick,				kQ3True);

	return(qd3dStatus);
}





//-----------------------------------------------------------------------------
//      E3Pick_GetType : Gets the pick object's type.
//-----------------------------------------------------------------------------
#pragma mark -
#pragma mark ========== Picks ==========
#pragma mark -
TQ3ObjectType
E3Pick_GetType(TQ3PickObject pick)
{

	// Return the type, always a leaf type
	return(E3ClassTree_GetObjectType(pick, kQ3ObjectTypePick));
}





//=============================================================================
//      E3Pick_GetData : Gets the pick object's data.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetData(TQ3PickObject pick, TQ3PickData *data)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Get the field
	*data = instanceData->data.common;
	return(kQ3Success);
}





//=============================================================================
//      E3Pick_SetData : Sets the pick object's data.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_SetData(TQ3PickObject pick, const TQ3PickData *data)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Set the field
	instanceData->data.common = *data;
	return(kQ3Success);
}





//=============================================================================
//      E3Pick_GetVertexTolerance : Gets the pick object's vertex tolerance.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetVertexTolerance(TQ3PickObject pick, float *vertexTolerance)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;


	// Get the field
	if (Q3Object_IsType(pick, kQ3PickTypeWindowPoint))
		{
		*vertexTolerance = instanceData->data.windowPointData.vertexTolerance;
		return(kQ3Success);
		}
	else
		{
		*vertexTolerance = 0;
		return(kQ3Failure);
		}
}





//=============================================================================
//      E3Pick_GetEdgeTolerance : Gets the pick object's edge tolerance.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetEdgeTolerance(TQ3PickObject pick, float *edgeTolerance)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Get the field
	if (Q3Object_IsType(pick, kQ3PickTypeWindowPoint))
		{
		*edgeTolerance = instanceData->data.windowPointData.edgeTolerance;
		return(kQ3Success);
		}
	else
		{
		*edgeTolerance = 0;
		return(kQ3Failure);
		}
}





//=============================================================================
//      E3Pick_SetVertexTolerance : Sets the pick object's vertex tolerance.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_SetVertexTolerance(TQ3PickObject pick, float vertexTolerance)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Set the field
	if (Q3Object_IsType(pick, kQ3PickTypeWindowPoint))
		{
		instanceData->data.windowPointData.vertexTolerance = vertexTolerance;
		return(kQ3Success);
		}
	return(kQ3Failure);
}





//=============================================================================
//      E3Pick_SetEdgeTolerance : Sets the pick object's edge tolerance.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_SetEdgeTolerance(TQ3PickObject pick, float edgeTolerance)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Set the field
	if (Q3Object_IsType(pick, kQ3PickTypeWindowPoint))
		{
		instanceData->data.windowPointData.edgeTolerance = edgeTolerance;
		return(kQ3Success);
		}
	return(kQ3Failure);
}





//=============================================================================
//      E3Pick_GetNumHits : Gets the pick object's number of hits.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetNumHits(TQ3PickObject pick, TQ3Uns32 *numHits)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Get the field
	*numHits = instanceData->numHits;
	return(kQ3Success);
}





//=============================================================================
//      E3Pick_EmptyHitList : Adds a new picked data to the hit list.
//		(Semi-private, no access to the 3rd party programmer)
//-----------------------------------------------------------------------------
//		Note :	This will not take any references to the TQ3Objects
//				so you must not dispose of them.
//				This routine takes ownership of the data.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Pick_AddHitData(TQ3PickObject pick, const TQ3PickedData* data)
{	TQ3PickedData* newData = (TQ3PickedData*) Q3Memory_Allocate(sizeof(TQ3PickedData));

	if (newData)
		{
		TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) pick->instanceData;
		TQ3PickedData		*current = instanceData->pickedData;
		
		*newData = *data;
		++instanceData->numHits;
		
		if (current)
			{
			// add it in the correct position
			if (newData->validMask & kQ3PickDetailMaskDistance)
				{
				register float distance = newData->distance; // put into register for speed
				TQ3PickSort sort = instanceData->data.common.sort;
				
				if (sort == kQ3PickSortNearToFar)
					{
					register TQ3PickedData* next = current->next;
					if (next)
						{
						while (next)
							{
							if (next->validMask & kQ3PickDetailMaskDistance)
								{
								if (next->distance > distance)
									{
									// insert after current
									current->next = newData;
									newData->next = next;
									break;
									}
								}
							current = next;
							next = next->next;
							}
						}
					else // this is the end of the chain
						{
						current->next = newData;
						newData->next = NULL;
						}
					}
				else
				if (sort == kQ3PickSortFarToNear)
					{
					register TQ3PickedData* prev = NULL;
					while (current)
						{
						if (current->validMask & kQ3PickDetailMaskDistance)
							{
							if (current->distance < distance)
								{
								// insert before current
								newData->next = current;
								if (prev)
									prev->next = newData;
								else // first time round the loop, insert at beginning of list
									instanceData->pickedData = newData;
								break;
								}
							}
						prev = current;
						current = current->next;
						}
					}
				else // no sorting, add newData to beginning
					{
					instanceData->pickedData = newData;
					newData->next = current;
					}
				}
			else // newData has no valid distance value
				{
				while (current->next)
					current = current->next;
				current->next = newData;
				newData->next = NULL;
				}
			}
		else // this is the first item
			{
			instanceData->pickedData = newData;
			newData->next = NULL;
			}
		return(kQ3Success);
		}
	return(kQ3Failure);
}





//=============================================================================
//      E3Pick_EmptyHitList : Empties the hit list.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_EmptyHitList(TQ3PickObject pick)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) pick->instanceData;
	TQ3PickedData		*current = instanceData->pickedData;
	
	instanceData->pickedData = NULL;
	instanceData->numHits = 0;
	
	while (current)
		{
		TQ3PickedData* next = current->next;
		if (current->validMask & kQ3PickDetailMaskPath)
			Q3HitPath_EmptyData (&current->path);
		if (current->validMask & kQ3PickDetailMaskObject)
			Q3Object_Dispose (current->object);
		if (current->validMask & kQ3PickDetailMaskShapePart)
			Q3Object_Dispose (current->shapePart);
		Q3Memory_Free (&current);
		current = next;
		}
	
	return(kQ3Success);
}





//=============================================================================
//      E3Pick_GetPickDetailValidMask : Gets the pick mask for the nth pick.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetPickDetailValidMask(TQ3PickObject pick, TQ3Uns32 index, TQ3PickDetail *pickDetailValidMask)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) pick->instanceData;
	
	if (index < instanceData->numHits)
		{
		TQ3PickedData *current = instanceData->pickedData;
		
		while (current && index)
			{
			--index;
			current = current->next;
			}
		
		if (current)
			{
			*pickDetailValidMask = current->validMask;
			return(kQ3Success);
			}
		}
	*pickDetailValidMask = kQ3PickDetailNone;
	return(kQ3Failure);
}





//=============================================================================
//      E3Pick_GetPickDetailData : Gets the data for the nth object's detail.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetPickDetailData(TQ3PickObject pick, TQ3Uns32 index, TQ3PickDetail pickDetailValue, void *detailData)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) pick->instanceData;
	
	if (index < instanceData->numHits)
		{
		TQ3PickedData *current = instanceData->pickedData;

		while (current && index)
			{
			--index;
			current = current->next;
			}
		
		if (current)
			{
			if (current->validMask & pickDetailValue)
				{
				switch (pickDetailValue)
					{
					case kQ3PickDetailMaskPickID:
						*((TQ3Int32*)(detailData)) = current->pickID;
						break;
					case kQ3PickDetailMaskPath:
						*((TQ3HitPath*)(detailData)) = current->path;
						if (current->path.rootGroup)
							((TQ3HitPath*)(detailData))->rootGroup = Q3Shared_GetReference (current->path.rootGroup);
						break;
					case kQ3PickDetailMaskObject:
						*((TQ3SharedObject*)(detailData)) = Q3Shared_GetReference (current->object);
						break;
					case kQ3PickDetailMaskLocalToWorldMatrix:
						*((TQ3Matrix4x4*)(detailData)) = current->localToWorldMatrix;
						break;
					case kQ3PickDetailMaskXYZ:
						*((TQ3Point3D*)(detailData)) = current->xyz;
						break;
					case kQ3PickDetailMaskDistance:
						*((float*)(detailData)) = current->distance;
						break;
					case kQ3PickDetailMaskNormal:
						*((TQ3Vector3D*)(detailData)) = current->normal;
						break;
					case kQ3PickDetailMaskShapePart:
						*((TQ3ShapePartObject*)(detailData)) = Q3Shared_GetReference (current->shapePart);
						break;
					case kQ3PickDetailMaskPickPart:
						*((TQ3PickParts*)(detailData)) = current->pickPart;
						break;
					case kQ3PickDetailMaskUV:
						*((TQ3Param2D*)(detailData)) = current->uv;
						break;
					default:
						return(kQ3Failure);
					}
				return(kQ3Success);
				}
			}
		}
	return(kQ3Failure);
}





//=============================================================================
//      E3WindowPointPick_New : Creates a new window point pick.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3PickObject
E3WindowPointPick_New(const TQ3WindowPointPickData *data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3PickTypeWindowPoint, kQ3False, data);
	return(theObject);
}





//=============================================================================
//      E3WindowPointPick_GetPoint : Gets the window point pick's point.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowPointPick_GetPoint(TQ3PickObject pick, TQ3Point2D *point)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Get the field
	*point = instanceData->data.windowPointData.point;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowPointPick_SetPoint : Sets the window point pick's point.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowPointPick_SetPoint(TQ3PickObject pick, const TQ3Point2D *point)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Set the field
	instanceData->data.windowPointData.point = *point;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowPointPick_GetData : Gets the window point pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowPointPick_GetData(TQ3PickObject pick, TQ3WindowPointPickData *data)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Get the field
	*data = instanceData->data.windowPointData;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowPointPick_SetData : Sets the window point pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowPointPick_SetData(TQ3PickObject pick, const TQ3WindowPointPickData *data)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Set the field
	instanceData->data.windowPointData = *data;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowRectPick_New : Creates a new window rect pick.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3PickObject
E3WindowRectPick_New(const TQ3WindowRectPickData *data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3PickTypeWindowRect, kQ3False, data);
	return(theObject);
}





//=============================================================================
//      E3WindowRectPick_GetRect : Gets the window rect pick's rect.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowRectPick_GetRect(TQ3PickObject pick, TQ3Area *rect)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Get the field
	*rect = instanceData->data.windowRectData.rect;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowRectPick_SetRect : Sets the window rect pick's rect.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowRectPick_SetRect(TQ3PickObject pick, const TQ3Area *rect)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Set the field
	instanceData->data.windowRectData.rect = *rect;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowRectPick_GetData : Gets the window rect pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowRectPick_GetData(TQ3PickObject pick, TQ3WindowRectPickData *data)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Get the field
	*data = instanceData->data.windowRectData;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowRectPick_SetData : Sets the window rect pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowRectPick_SetData(TQ3PickObject pick, const TQ3WindowRectPickData *data)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Set the field
	instanceData->data.windowRectData = *data;
	return(kQ3Success);
}





//=============================================================================
//      E3WorldRayPick_New : Creates a new world ray pick.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3PickObject
E3WorldRayPick_New(const TQ3WorldRayPickData *data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3PickTypeWorldRay, kQ3False, data);
	return(theObject);
}





//=============================================================================
//      E3WorldRayPick_GetRay : Gets the world ray pick's ray.
//-----------------------------------------------------------------------------
TQ3Status
E3WorldRayPick_GetRay(TQ3PickObject pick, TQ3Ray3D *ray)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Get the field
	*ray = instanceData->data.worldRayData.ray;
	return(kQ3Success);
}





//=============================================================================
//      E3WorldRayPick_SetRay : Sets the world ray pick's ray.
//-----------------------------------------------------------------------------
TQ3Status
E3WorldRayPick_SetRay(TQ3PickObject pick, const TQ3Ray3D *ray)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Set the field
	instanceData->data.worldRayData.ray = *ray;
	return(kQ3Success);
}





//=============================================================================
//      E3WorldRayPick_GetData : Gets the world ray pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WorldRayPick_GetData(TQ3PickObject pick, TQ3WorldRayPickData *data)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Get the field
	*data = instanceData->data.worldRayData;
	return(kQ3Success);
}





//=============================================================================
//      E3WorldRayPick_SetData : Sets the world ray pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WorldRayPick_SetData(TQ3PickObject pick, const TQ3WorldRayPickData *data)
{	TQ3PickUnionData *instanceData = (TQ3PickUnionData *) pick->instanceData;



	// Set the field
	instanceData->data.worldRayData = *data;
	return(kQ3Success);
}





//=============================================================================
//      E3ShapePart_New : Creates a new shape part.
//		(Semi-private, no access to the 3rd party programmer)
//-----------------------------------------------------------------------------
#pragma mark -
#pragma mark ========== Parts ==========
#pragma mark -
TQ3ShapePartObject
E3ShapePart_New(const TQ3ShapeObject object)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3SharedTypeShapePart, kQ3False, object);
	return(theObject);
}





//=============================================================================
//      E3ShapePart_GetType : Gets the type of a shape part.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3ShapePart_GetType(TQ3ShapePartObject shapePartObject)
{


	// Return the type
	return(E3ClassTree_GetObjectType(shapePartObject, kQ3SharedTypeShapePart));
}





//=============================================================================
//      E3ShapePart_GetShape : Gets the shape of a shape part.
//-----------------------------------------------------------------------------
TQ3Status
E3ShapePart_GetShape(TQ3ShapePartObject shapePartObject, TQ3ShapeObject *shapeObject)
{	TQ3ShapeObject *instanceData = (TQ3ShapeObject *) shapePartObject->instanceData;

	if (*instanceData)
		*shapeObject = Q3Shared_GetReference (*instanceData);
	else
		*shapeObject = NULL;
	return(kQ3Success);
}





//=============================================================================
//      E3MeshPart_New : Creates a new mesh part.
//		(Semi-private, no access to the 3rd party programmer)
//-----------------------------------------------------------------------------
#pragma mark -
TQ3MeshPartObject
E3MeshPart_New(const TQ3MeshComponent data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3ShapePartTypeMeshPart, kQ3False, data);
	return(theObject);
}





//=============================================================================
//      E3MeshPart_GetType : Gets the type of a mesh part.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3MeshPart_GetType(TQ3MeshPartObject meshPartObject)
{


	// Return the type
	return(E3ClassTree_GetObjectType(meshPartObject, kQ3ShapePartTypeMeshPart));
}





//=============================================================================
//      E3MeshPart_GetComponent : Gets the component of a mesh part.
//-----------------------------------------------------------------------------
TQ3Status
E3MeshPart_GetComponent(TQ3MeshPartObject meshPartObject, TQ3MeshComponent *component)
{	TQ3MeshComponent *instanceData = (TQ3MeshComponent *) meshPartObject->instanceData;


	*component = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3MeshFacePart_New : Creates a new world mesh face part.
//		(Semi-private, no access to the 3rd party programmer)
//-----------------------------------------------------------------------------
#pragma mark -
TQ3MeshFacePartObject
E3MeshFacePart_New(const TQ3MeshFace data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3MeshPartTypeMeshFacePart, kQ3False, data);
	return(theObject);
}





//=============================================================================
//      E3MeshFacePart_GetFace : Gets the face of a face mesh part.
//-----------------------------------------------------------------------------
TQ3Status
E3MeshFacePart_GetFace(TQ3MeshFacePartObject meshFacePartObject, TQ3MeshFace *face)
{	TQ3MeshFace *instanceData = (TQ3MeshFace *) meshFacePartObject->instanceData;


	*face = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3MeshEdgePart_New : Creates a new mesh edge part.
//		(Semi-private, no access to the 3rd party programmer)
//-----------------------------------------------------------------------------
#pragma mark -
TQ3MeshEdgePartObject
E3MeshEdgePart_New(const TQ3MeshEdge data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3MeshPartTypeMeshEdgePart, kQ3False, data);
	return(theObject);
}





//=============================================================================
//      E3MeshEdgePart_GetEdge : Gets the edge of an edge mesh part.
//-----------------------------------------------------------------------------
TQ3Status
E3MeshEdgePart_GetEdge(TQ3MeshEdgePartObject meshEdgePartObject, TQ3MeshEdge *edge)
{	TQ3MeshEdge *instanceData = (TQ3MeshEdge *) meshEdgePartObject->instanceData;


	*edge = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3MeshVertexPart_New : Creates a new mesh vertex part.
//		(Semi-private, no access to the 3rd party programmer)
//-----------------------------------------------------------------------------
#pragma mark -
TQ3MeshVertexPartObject
E3MeshVertexPart_New(const TQ3MeshVertex data)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3MeshPartTypeMeshVertexPart, kQ3False, data);
	return(theObject);
}





//=============================================================================
//      E3MeshVertexPart_GetVertex : Gets the vertex of a vertex mesh part.
//-----------------------------------------------------------------------------
TQ3Status
E3MeshVertexPart_GetVertex(TQ3MeshVertexPartObject meshVertexPartObject, TQ3MeshVertex *vertex)
{	TQ3MeshVertex *instanceData = (TQ3MeshVertex *) meshVertexPartObject->instanceData;


	*vertex = *instanceData;
	return(kQ3Success);
}





//=============================================================================
//      E3HitPath_EmptyData : Empties a TQ3HitPath.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3HitPath_EmptyData(TQ3HitPath *hitPath)
{
	if (hitPath->rootGroup)
		{
		Q3Object_Dispose (hitPath->rootGroup);
		hitPath->rootGroup = NULL;
		}
	hitPath->depth = 0;
	hitPath->positions = NULL;
		
	return(kQ3Success);
}

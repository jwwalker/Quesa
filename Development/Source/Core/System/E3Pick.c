/*  NAME:
        E3Pick.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Quesa Copyright � 1999-2003, Quesa Developers.
        
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
#include "E3Group.h"
#include "E3Pick.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Pick hit result
typedef struct TQ3PickHit {
	// Next hit in the list
	struct TQ3PickHit			*nextHit;

	// Mask indicating valid fields for this hit
	TQ3PickDetail				validMask;

	// Data for this hit
	TQ3Uns32					pickedID;
	TQ3HitPath					pickedPath;
	TQ3PickParts				pickedPart;
	TQ3SharedObject				pickedObject;
	TQ3ShapePartObject			pickedShape;
	TQ3Point3D					hitXYZ;
	TQ3Param2D					hitUV;
	TQ3Vector3D					hitNormal;
	float						hitDistance;
	TQ3Matrix4x4				localToWorld;
} TQ3PickHit;


// Pick object instance data
typedef struct TQ3PickUnionData {
	// Common data
	TQ3Uns32							numHits;
	TQ3PickHit							*pickHits;


	// Pick specific. Note that we assume that a TQ3PickData structure
	// is the first item within each pick structure, since this lets us
	// use a union and have the common data overlap.
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
//      e3pick_hit_duplicate_path : Duplicate a TQ3HitPath.
//-----------------------------------------------------------------------------
static TQ3Status
e3pick_hit_duplicate_path(TQ3HitPath *pickedPath, TQ3HitPath *newPath)
{	TQ3Uns32		theSize;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pickedPath), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(newPath),    kQ3Failure);



	// Duplicate the position array. Note that we assume group positions
	// are simple objects and are not reference counted - this will cause
	// a memory leak if this is ever changed.
	theSize            = pickedPath->depth * sizeof(TQ3GroupPosition);
	newPath->positions = (TQ3GroupPosition *) Q3Memory_Allocate(theSize);
	if (newPath->positions == NULL)
		return(kQ3Failure);

	Q3Memory_Copy(pickedPath->positions, newPath->positions, theSize);



	// Finish off the new hit path structure
	newPath->rootGroup = Q3Shared_GetReference(pickedPath->rootGroup);
	newPath->depth     = pickedPath->depth;
	
	return(kQ3Success);
}





//=============================================================================
//      e3pick_hit_initialise : Initialise a TQ3PickHit.
//-----------------------------------------------------------------------------
static void
e3pick_hit_initialise(TQ3PickHit				*theHit,
						TQ3PickObject			thePick,
						TQ3ViewObject			theView,
						const TQ3Point3D		*hitXYZ,
						const TQ3Vector3D		*hitNormal,
						const TQ3Param2D		*hitUV,
						TQ3ShapePartObject		hitShape)
{	TQ3CameraPlacement		cameraPlacement;
	TQ3HitPath				*currentPath;
	TQ3Status				qd3dStatus;
	TQ3CameraObject			theCamera;
	TQ3Vector3D				eyeVector;
	TQ3PickData				pickData;
	TQ3ObjectType			theType;
	TQ3Ray3D				pickRay;



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theHit));
	Q3_REQUIRE(Q3_VALID_PTR(thePick));
	Q3_REQUIRE(Q3_VALID_PTR(theView));



	// Get the data for the pick object
	qd3dStatus = Q3Pick_GetData(thePick, &pickData);
	if (qd3dStatus != kQ3Success)
		return;



	// Save the pick ID
	if (E3Bit_IsSet(pickData.mask, kQ3PickDetailMaskPickID))
		{
		qd3dStatus = Q3View_GetPickIDStyleState(theView, &theHit->pickedID);
		if (qd3dStatus == kQ3Success)
			theHit->validMask |= kQ3PickDetailMaskPickID;
		}



	// Save the path to the hit object
	if (E3Bit_IsSet(pickData.mask, kQ3PickDetailMaskPath))
		{
		currentPath = E3View_PickStack_GetPickedPath(theView);
		qd3dStatus  = e3pick_hit_duplicate_path(currentPath, &theHit->pickedPath);
		if (qd3dStatus == kQ3Success)
			theHit->validMask |= kQ3PickDetailMaskPath;
		}



	// Save the hit object
	if (E3Bit_IsSet(pickData.mask, kQ3PickDetailMaskObject))
		{
		theHit->pickedObject = E3View_PickStack_GetPickedObject(theView);
		if (theHit->pickedObject != NULL)
			theHit->validMask |= kQ3PickDetailMaskObject;
		}



	// Save the local->world matrix
	if (E3Bit_IsSet(pickData.mask, kQ3PickDetailMaskLocalToWorldMatrix))
		{
		qd3dStatus = E3View_GetLocalToWorldMatrixState(theView, &theHit->localToWorld);
		if (qd3dStatus == kQ3Success)
			theHit->validMask |= kQ3PickDetailMaskLocalToWorldMatrix;
		}



	// Save the world coordinates of the hit point
	if (E3Bit_IsSet(pickData.mask, kQ3PickDetailMaskXYZ) && hitXYZ != NULL)
		{
		theHit->hitXYZ     = *hitXYZ;
		theHit->validMask |= kQ3PickDetailMaskXYZ;
		}



	// Save the distance to the viewer
	if (E3Bit_IsSet(pickData.mask, kQ3PickDetailMaskDistance) && hitXYZ != NULL)
		{
		if (Q3Pick_GetType( thePick ) == kQ3PickTypeWorldRay)
			{
			Q3WorldRayPick_GetRay( thePick, &pickRay );
			Q3Point3D_Subtract(hitXYZ, &pickRay.origin, &eyeVector);
			}
		else
			{
			qd3dStatus = Q3View_GetCamera(theView, &theCamera);
			if (qd3dStatus == kQ3Success)
				{
				Q3Camera_GetPlacement(theCamera, &cameraPlacement);
				Q3Point3D_Subtract(hitXYZ, &cameraPlacement.cameraLocation, &eyeVector);
				Q3Object_Dispose(theCamera);

				}
			}
		theHit->hitDistance = Q3Vector3D_Length(&eyeVector);
		theHit->validMask  |= kQ3PickDetailMaskDistance;
		}



	// Save the normal at the hit point
	if (E3Bit_IsSet(pickData.mask, kQ3PickDetailMaskNormal) && hitNormal != NULL)
		{
		Q3Vector3D_Normalize(hitNormal, &theHit->hitNormal);
		theHit->validMask |= kQ3PickDetailMaskNormal;
		}



	// Save the shape part
	if (E3Bit_IsSet(pickData.mask, kQ3PickDetailMaskShapePart))
		{
		theHit->pickedShape = Q3Shared_GetReference(hitShape);
		theHit->validMask  |= kQ3PickDetailMaskShapePart;
		}



	// Save the pick part
	//
	// Pick parts are only supported for meshes at present - from the QD3D docs,
	// that appears to be the only geometry which supports picking of sub-object
	// components.
	//
	// To determine what type of sub-object has been picked, we assume that the
	// appropriate shape part has been supplied by the mesh and return its type.
	// If no shape part has been supplied, or it's not a face/edge/vertex, we
	// return that the entire object has been picked.
	//
	// This assumes the mesh object's picking implementation has examined the
	// current pick part style and picked the appropriate sub-object types.
	if (E3Bit_IsSet(pickData.mask, kQ3PickDetailMaskPickPart))
		{
		// Set up the default
		theHit->pickedPart = kQ3PickPartsObject;
		theHit->validMask |= kQ3PickDetailMaskPickPart;
		
		
		// If we have a shape part, indicate what type
		if (hitShape != NULL)
			{
			theType = Q3Object_GetLeafType(hitShape);
			switch (theType) {
				case kQ3MeshPartTypeMeshFacePart:
					theHit->pickedPart = kQ3PickPartsMaskFace;
					break;

				case kQ3MeshPartTypeMeshEdgePart:
					theHit->pickedPart = kQ3PickPartsMaskEdge;
					break;

				case kQ3MeshPartTypeMeshVertexPart:
					theHit->pickedPart = kQ3PickPartsMaskVertex;
					break;

				default:
					break;
				}
			}
		}



	// Save the UV at the hit point
	if (E3Bit_IsSet(pickData.mask, kQ3PickDetailMaskUV) && hitUV != NULL)
		{
		theHit->hitUV      = *hitUV;
		theHit->validMask |= kQ3PickDetailMaskUV;
		}
}





//=============================================================================
//      e3pick_hit_find : Find the nth pick hit in a list.
//-----------------------------------------------------------------------------
static TQ3PickHit *
e3pick_hit_find(TQ3PickUnionData *pickInstanceData, TQ3Uns32 n)
{	TQ3PickHit		*currentHit = pickInstanceData->pickHits;



	// Check we're not out of range
	if (n > pickInstanceData->numHits)
		return(NULL);
	
	if (pickInstanceData->data.common.numHitsToReturn != kQ3ReturnAllHits)
		{
		if (n > pickInstanceData->data.common.numHitsToReturn)
			return(NULL);
		}



	// Walk through the list to find the right item
	while (currentHit != NULL && n != 0)
		{
		n--;
		currentHit = currentHit->nextHit;
		}

	return(currentHit);
}





//=============================================================================
//      e3pick_set_sort_mask : Set the sort mask as per QD3D.
//-----------------------------------------------------------------------------
//		Note :	QD3D will sort hits if any sort method is supplied, even if
//				the distance bit is not set for the pick. We handle this by
//				turning on that bit whenever a new sort method is set.
//-----------------------------------------------------------------------------
static void
e3pick_set_sort_mask(TQ3PickData *pickData)
{


	// Turn on the distance flag if any kind of sorting is requested
	if (pickData->sort != kQ3PickSortNone)
		pickData->mask |= kQ3PickDetailMaskDistance;
}





//=============================================================================
//      e3pick_windowpoint_new : Window point pick new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3pick_windowpoint_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PickUnionData				*instanceData = (TQ3PickUnionData *) privateData;
	const TQ3WindowPointPickData	*pickData     = (const TQ3WindowPointPickData *) paramData;



	// Initialise our instance data
	instanceData->data.windowPointData = *pickData;

	e3pick_set_sort_mask(&instanceData->data.windowPointData.data);
	
	return(kQ3Success);
}





//=============================================================================
//      e3pick_windowpoint_delete : Window point pick delete method.
//-----------------------------------------------------------------------------
static void
e3pick_windowpoint_delete(TQ3Object theObject, void *privateData)
{
#pragma unused(privateData)



	// Empty the pick list
	E3Pick_EmptyHitList(theObject);
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

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3pick_windowpoint_delete;
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
	const TQ3WindowRectPickData		*pickData     = (const TQ3WindowRectPickData *) paramData;



	// Initialise our instance data
	instanceData->data.windowRectData = *pickData;

	return(kQ3Success);
}





//=============================================================================
//      e3pick_windowrect_delete : Window rect pick delete method.
//-----------------------------------------------------------------------------
static void
e3pick_windowrect_delete(TQ3Object theObject, void *privateData)
{
#pragma unused(privateData)



	// Empty the pick list
	E3Pick_EmptyHitList(theObject);
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

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3pick_windowrect_delete;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3pick_worldray_new : World ray pick new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3pick_worldray_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PickUnionData			*instanceData = (TQ3PickUnionData *) privateData;
	const TQ3WorldRayPickData	*pickData     = (const TQ3WorldRayPickData *) paramData;



	// Initialise our instance data
	instanceData->data.worldRayData = *pickData;

	e3pick_set_sort_mask(&instanceData->data.windowPointData.data);
	
	return(kQ3Success);
}





//=============================================================================
//      e3pick_worldray_delete : World ray pick delete method.
//-----------------------------------------------------------------------------
static void
e3pick_worldray_delete(TQ3Object theObject, void *privateData)
{
#pragma unused(privateData)



	// Empty the pick list
	E3Pick_EmptyHitList(theObject);
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

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3pick_worldray_delete;
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
{	TQ3ShapeObject			*instanceData = (TQ3ShapeObject *)       privateData;
	const TQ3ShapeObject	*shape        = (const TQ3ShapeObject *) paramData;
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
{	TQ3ShapeObject			*instanceData = (TQ3ShapeObject *) privateData;
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
{	TQ3MeshComponent			*instanceData = (TQ3MeshComponent *)       privateData;
	const TQ3MeshComponent		*data         = (const TQ3MeshComponent *) paramData;
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
{	TQ3MeshVertex			*instanceData = (TQ3MeshVertex *)       privateData;
	const TQ3MeshVertex		*data         = (const TQ3MeshVertex *) paramData;
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
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
												kQ3SharedTypeShapePart,
												kQ3ClassNameShapePart,
												e3shapepart_metahandler,
												sizeof(TQ3ShapeObject));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeShapePart,
												kQ3ShapePartTypeMeshPart,
												kQ3ClassNameMeshShapePart,
												e3meshpart_metahandler,
												sizeof(TQ3MeshComponent));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapePartTypeMeshPart,
												kQ3MeshPartTypeMeshFacePart,
												kQ3ClassNameMeshFacePart,
												e3meshpart_face_metahandler,
												sizeof(TQ3MeshFace));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapePartTypeMeshPart,
												kQ3MeshPartTypeMeshEdgePart,
												kQ3ClassNameMeshEdgePart,
												e3meshpart_edge_metahandler,
												sizeof(TQ3MeshEdge));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapePartTypeMeshPart,
												kQ3MeshPartTypeMeshVertexPart,
												kQ3ClassNameMeshVertexPart,
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





//=============================================================================
//      E3Pick_GetType : Gets the pick object's type.
//-----------------------------------------------------------------------------
#pragma mark -
#pragma mark ========== Picks ==========
#pragma mark -
TQ3ObjectType
E3Pick_GetType(TQ3PickObject thePick)
{

	// Return the type, always a leaf type
	return(E3ClassTree_GetObjectType(thePick, kQ3ObjectTypePick));
}





//=============================================================================
//      E3Pick_GetData : Gets the pick object's data.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetData(TQ3PickObject thePick, TQ3PickData *data)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Get the field
	*data = instanceData->data.common;
	return(kQ3Success);
}





//=============================================================================
//      E3Pick_SetData : Sets the pick object's data.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_SetData(TQ3PickObject thePick, const TQ3PickData *data)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Set the field
	instanceData->data.common = *data;

	e3pick_set_sort_mask(&instanceData->data.common);

	return(kQ3Success);
}





//=============================================================================
//      E3Pick_GetVertexTolerance : Gets the pick object's vertex tolerance.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetVertexTolerance(TQ3PickObject thePick, float *vertexTolerance)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;
	TQ3Status			qd3dStatus    = kQ3Success;



	// Get the field
	if (Q3Object_IsType(thePick, kQ3PickTypeWindowPoint))
		*vertexTolerance = instanceData->data.windowPointData.vertexTolerance;

	else if (Q3Object_IsType(thePick, kQ3PickTypeWorldRay))
		*vertexTolerance = instanceData->data.worldRayData.vertexTolerance;

	else
		{
		*vertexTolerance = 0.0f;
		qd3dStatus       = kQ3Failure;
		}

	return(qd3dStatus);
}





//=============================================================================
//      E3Pick_GetEdgeTolerance : Gets the pick object's edge tolerance.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetEdgeTolerance(TQ3PickObject thePick, float *edgeTolerance)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;
	TQ3Status			qd3dStatus    = kQ3Success;



	// Get the field
	if (Q3Object_IsType(thePick, kQ3PickTypeWindowPoint))
		*edgeTolerance = instanceData->data.windowPointData.edgeTolerance;

	else if (Q3Object_IsType(thePick, kQ3PickTypeWorldRay))
		*edgeTolerance = instanceData->data.worldRayData.edgeTolerance;

	else
		{
		*edgeTolerance = 0.0f;
		qd3dStatus     = kQ3Failure;
		}

	return(qd3dStatus);
}





//=============================================================================
//      E3Pick_SetVertexTolerance : Sets the pick object's vertex tolerance.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_SetVertexTolerance(TQ3PickObject thePick, float vertexTolerance)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;
	TQ3Status			qd3dStatus    = kQ3Success;



	// Set the field
	if (Q3Object_IsType(thePick, kQ3PickTypeWindowPoint))
		instanceData->data.windowPointData.vertexTolerance = vertexTolerance;

	else if (Q3Object_IsType(thePick, kQ3PickTypeWorldRay))
		instanceData->data.worldRayData.vertexTolerance = vertexTolerance;

	else
		qd3dStatus = kQ3Failure;
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Pick_SetEdgeTolerance : Sets the pick object's edge tolerance.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_SetEdgeTolerance(TQ3PickObject thePick, float edgeTolerance)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;
	TQ3Status			qd3dStatus    = kQ3Success;



	// Set the field
	if (Q3Object_IsType(thePick, kQ3PickTypeWindowPoint))
		instanceData->data.windowPointData.edgeTolerance = edgeTolerance;

	else if (Q3Object_IsType(thePick, kQ3PickTypeWorldRay))
		instanceData->data.worldRayData.edgeTolerance = edgeTolerance;

	else
		qd3dStatus = kQ3Failure;
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Pick_GetNumHits : Gets the pick object's number of hits.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetNumHits(TQ3PickObject thePick, TQ3Uns32 *numHits)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Get the field, clamping it if a limit was supplied
	*numHits = instanceData->numHits;
	
	if (instanceData->data.common.numHitsToReturn != kQ3ReturnAllHits)
		{
		if (*numHits > instanceData->data.common.numHitsToReturn)
			*numHits = instanceData->data.common.numHitsToReturn;
		}
	
	return(kQ3Success);
}





//=============================================================================
//      E3Pick_EmptyHitList : Empties the hit list.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_EmptyHitList(TQ3PickObject thePick)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;
	TQ3PickHit			*currentHit;
	TQ3PickHit			*nextHit;



	// Dispose of the hit list
	currentHit = instanceData->pickHits;
	while (currentHit != NULL)
		{
		// Grab the next item in the list
		nextHit = currentHit->nextHit;
		
		
		// Dispose of the item
		if (E3Bit_IsSet(currentHit->validMask, kQ3PickDetailMaskPath))
			Q3HitPath_EmptyData(&currentHit->pickedPath);

		if (E3Bit_IsSet(currentHit->validMask, kQ3PickDetailMaskObject))
			Q3Object_Dispose(currentHit->pickedObject);

		if (E3Bit_IsSet(currentHit->validMask, kQ3PickDetailMaskShapePart))
			Q3Object_Dispose(currentHit->pickedShape);

		Q3Memory_Free(&currentHit);


		// Move on to the next item in the list
		currentHit = nextHit;
		}



	// Reset our state
	instanceData->numHits  = 0;
	instanceData->pickHits = NULL;

	return(kQ3Success);
}





//=============================================================================
//      E3Pick_GetPickDetailValidMask : Gets the pick mask for the nth pick.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetPickDetailValidMask(TQ3PickObject thePick, TQ3Uns32 index, TQ3PickDetail *pickDetailValidMask)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;
	TQ3PickHit			*theHit;



	// Find the item
	theHit = e3pick_hit_find(instanceData, index);
	if (theHit == NULL)
		{
		*pickDetailValidMask = kQ3PickDetailNone;
		return(kQ3Failure);
		}



	// Return the mask
	*pickDetailValidMask = theHit->validMask;

	return(kQ3Success);
}





//=============================================================================
//      E3Pick_GetPickDetailData : Gets the data for the nth object's detail.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_GetPickDetailData(TQ3PickObject thePick, TQ3Uns32 index, TQ3PickDetail pickDetailValue, void *detailData)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;
	TQ3Status			qd3dStatus;
	TQ3PickHit			*theHit;



	// Find the item
	theHit = e3pick_hit_find(instanceData, index);
	if (theHit == NULL)
		return(kQ3Failure);



	// Check to see if the data is present
	if (E3Bit_IsNotSet(theHit->validMask, pickDetailValue))
		return(kQ3Failure);



	// Return the data
	qd3dStatus = kQ3Success;
	switch (pickDetailValue) {
		case kQ3PickDetailMaskPickID:
			*((TQ3Int32*)(detailData)) = theHit->pickedID;
			break;
		case kQ3PickDetailMaskPath:
			qd3dStatus = e3pick_hit_duplicate_path(&theHit->pickedPath, (TQ3HitPath *) detailData);
			break;
		case kQ3PickDetailMaskObject:
			*((TQ3SharedObject*)(detailData)) = Q3Shared_GetReference(theHit->pickedObject);
			break;
		case kQ3PickDetailMaskLocalToWorldMatrix:
			*((TQ3Matrix4x4*)(detailData)) = theHit->localToWorld;
			break;
		case kQ3PickDetailMaskXYZ:
			*((TQ3Point3D*)(detailData)) = theHit->hitXYZ;
			break;
		case kQ3PickDetailMaskDistance:
			*((float*)(detailData)) = theHit->hitDistance;
			break;
		case kQ3PickDetailMaskNormal:
			*((TQ3Vector3D*)(detailData)) = theHit->hitNormal;
			break;
		case kQ3PickDetailMaskShapePart:
			*((TQ3ShapePartObject*)(detailData)) = Q3Shared_GetReference(theHit->pickedShape);
			break;
		case kQ3PickDetailMaskPickPart:
			*((TQ3PickParts*)(detailData)) = theHit->pickedPart;
			break;
		case kQ3PickDetailMaskUV:
			*((TQ3Param2D*)(detailData)) = theHit->hitUV;
			break;
		default:
			qd3dStatus = kQ3Failure;
		}

	return(qd3dStatus);
}





//=============================================================================
//      E3Pick_RecordHit : Record a hit against a pick object.
//-----------------------------------------------------------------------------
//		Note :	Invoked when a successful pick between a geometry and a pick
//				object is detected. Responsible for saving the details of the
//				pick in the pick object's results list.
//-----------------------------------------------------------------------------
TQ3Status
E3Pick_RecordHit(TQ3PickObject				thePick,
					TQ3ViewObject			theView,
					const TQ3Point3D		*hitXYZ,
					const TQ3Vector3D		*hitNormal,
					const TQ3Param2D		*hitUV,
					TQ3ShapePartObject		hitShape)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;
	TQ3PickHit			*theHit, *currentHit, *previousHit;
	TQ3Boolean			savedHit, foundHit;
	TQ3PickSort			sortType;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(thePick),   kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theView),   kQ3Failure);



	// Allocate another hit record (groan :-)
	theHit = (TQ3PickHit*)Q3Memory_AllocateClear(sizeof(TQ3PickHit));
	if (theHit == NULL)
		return(kQ3Failure);



	// Fill out the data for the hit
	e3pick_hit_initialise(theHit, thePick, theView, hitXYZ, hitNormal, hitUV, hitShape);



	// Decide what kind of sorting is in effect. We try and sort if asked to, unless
	// we couldn't establish a distance for the hit. In that case (e.g., it's window
	// rect pick and the user has incorrectly asked for the results to be sorted)
	// we simply fall back to unsorted.
	//
	// To simplify the code below, we also revert to unsorted if the list is empty.
	sortType = instanceData->data.common.sort;

	if (E3Bit_IsNotSet(theHit->validMask, kQ3PickDetailMaskDistance))
		sortType = kQ3PickSortNone;

	if (instanceData->pickHits == NULL)
		sortType = kQ3PickSortNone;



	// Save the hit into the correct position within the list
	switch (sortType) {
		case kQ3PickSortNone:
			// Add it to the head of the list
			theHit->nextHit        = instanceData->pickHits;
			instanceData->pickHits = theHit;
			break;


		case kQ3PickSortNearToFar:
		case kQ3PickSortFarToNear:
			// We assume the list is non-empty. We walk through the list until we find
			// the hit that is closer/further than the current hit, and save ourselves
			// immediately before it to preserve the sorting order.
			//
			// Note that pick hits which don't have a distance will always come to the
			// front of the list, although this shouldn't cause problems.
			Q3_ASSERT(instanceData->pickHits != NULL);
			currentHit  = instanceData->pickHits;
			previousHit = NULL;
			savedHit    = kQ3False;


			// Search the list
			while (currentHit != NULL && !savedHit)
				{
				// Check this node
				foundHit = E3Bit_IsSet(currentHit->validMask, kQ3PickDetailMaskDistance);
				if (foundHit)
					{
					// Stop when we find a node further away
					if (sortType == kQ3PickSortNearToFar)
						foundHit = (TQ3Boolean)(currentHit->hitDistance > theHit->hitDistance);

					// Or stop when we find a node that's closer
					else
						foundHit = (TQ3Boolean)(currentHit->hitDistance < theHit->hitDistance);
					}


				// If we found the right place, save ourselves before it
				if (foundHit)
					{
					// Point the previous node at this node
					if (previousHit != NULL)
						previousHit->nextHit = theHit;
					else
						instanceData->pickHits = theHit;

					// Point this node at the next node
					theHit->nextHit = currentHit;

					// We're done
					savedHit = kQ3True;
					}


				// Move on to the next node
				previousHit = currentHit;
				currentHit  = currentHit->nextHit;
				}


			// If we didn't save it yet, append it to the end
			if (!savedHit)
				previousHit->nextHit = theHit;
			break;


		default:
			Q3_ASSERT(!"Unknown sort type");
			Q3Memory_Free(&theHit);
			return(kQ3Failure);
		}



	// Increment the hit count
	instanceData->numHits++;

	return(kQ3Success);
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
E3WindowPointPick_GetPoint(TQ3PickObject thePick, TQ3Point2D *point)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Get the field
	*point = instanceData->data.windowPointData.point;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowPointPick_SetPoint : Sets the window point pick's point.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowPointPick_SetPoint(TQ3PickObject thePick, const TQ3Point2D *point)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Set the field
	instanceData->data.windowPointData.point = *point;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowPointPick_GetData : Gets the window point pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowPointPick_GetData(TQ3PickObject thePick, TQ3WindowPointPickData *data)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Get the field
	*data = instanceData->data.windowPointData;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowPointPick_SetData : Sets the window point pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowPointPick_SetData(TQ3PickObject thePick, const TQ3WindowPointPickData *data)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Set the field
	instanceData->data.windowPointData = *data;

	e3pick_set_sort_mask(&instanceData->data.windowPointData.data);

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
E3WindowRectPick_GetRect(TQ3PickObject thePick, TQ3Area *rect)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Get the field
	*rect = instanceData->data.windowRectData.rect;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowRectPick_SetRect : Sets the window rect pick's rect.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowRectPick_SetRect(TQ3PickObject thePick, const TQ3Area *rect)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Set the field
	instanceData->data.windowRectData.rect = *rect;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowRectPick_GetData : Gets the window rect pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowRectPick_GetData(TQ3PickObject thePick, TQ3WindowRectPickData *data)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Get the field
	*data = instanceData->data.windowRectData;
	return(kQ3Success);
}





//=============================================================================
//      E3WindowRectPick_SetData : Sets the window rect pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WindowRectPick_SetData(TQ3PickObject thePick, const TQ3WindowRectPickData *data)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



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
E3WorldRayPick_GetRay(TQ3PickObject thePick, TQ3Ray3D *ray)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Get the field
	*ray = instanceData->data.worldRayData.ray;
	return(kQ3Success);
}





//=============================================================================
//      E3WorldRayPick_SetRay : Sets the world ray pick's ray.
//-----------------------------------------------------------------------------
TQ3Status
E3WorldRayPick_SetRay(TQ3PickObject thePick, const TQ3Ray3D *ray)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Set the field
	instanceData->data.worldRayData.ray = *ray;
	return(kQ3Success);
}





//=============================================================================
//      E3WorldRayPick_GetData : Gets the world ray pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WorldRayPick_GetData(TQ3PickObject thePick, TQ3WorldRayPickData *data)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Get the field
	*data = instanceData->data.worldRayData;
	return(kQ3Success);
}





//=============================================================================
//      E3WorldRayPick_SetData : Sets the world ray pick's data.
//-----------------------------------------------------------------------------
TQ3Status
E3WorldRayPick_SetData(TQ3PickObject thePick, const TQ3WorldRayPickData *data)
{	TQ3PickUnionData	*instanceData = (TQ3PickUnionData *) thePick->instanceData;



	// Set the field
	instanceData->data.worldRayData = *data;

	e3pick_set_sort_mask(&instanceData->data.worldRayData.data);

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


	// Dispose of the hit path data
	Q3Object_CleanDispose(&hitPath->rootGroup);
	Q3Memory_Free(&hitPath->positions);



	// And reset it
	hitPath->depth = 0;

	return(kQ3Success);
}

/*  NAME:
        E3GeometryPoint.c

    DESCRIPTION:
        Implementation of Quesa Point geometry class.

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
#include "E3Pick.h"
#include "E3Geometry.h"
#include "E3GeometryPoint.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_point_new : Point new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_point_new( TQ3Object theObject, void * privateData, const void * paramData )
{
	TQ3PointData *			instanceData = (TQ3PointData *)			privateData ;
	const TQ3PointData *	pointData    = (const TQ3PointData *)	paramData ;
	TQ3Status					qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3PointData));
	
	qd3dStatus = Q3Point_SetData(theObject, pointData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_point_delete : Point delete method.
//-----------------------------------------------------------------------------
static void
e3geom_point_delete(TQ3Object theObject, void *privateData)
{	TQ3PointData		*instanceData = (TQ3PointData *) privateData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3Point_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_point_duplicate : Point duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_point_duplicate(TQ3Object fromObject, const void *fromPrivateData,
					   TQ3Object toObject,   void       *toPrivateData)
{	TQ3PointData			*toInstanceData = (TQ3PointData *) toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Point_GetData(fromObject, toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_point_pick_window_point : Point window-point picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_point_pick_window_point(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3PointData			*instanceData = (const TQ3PointData *) objectData;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Point2D					windowPoint;
	TQ3WindowPointPickData		pickData;
	TQ3Point3D					hitXYZ;



	// Get the pick data
	Q3WindowPointPick_GetData(thePick, &pickData);



	// Transform our point to the screen
	Q3View_TransformLocalToWindow(theView, &instanceData->point, &windowPoint);



	// See if we fall within the pick
	if ((windowPoint.x >= (pickData.point.x - pickData.vertexTolerance)) &&
		(windowPoint.x <= (pickData.point.x + pickData.vertexTolerance)) &&
		(windowPoint.y >= (pickData.point.y - pickData.vertexTolerance)) &&
		(windowPoint.y <= (pickData.point.y + pickData.vertexTolerance)))
		{
		Q3View_TransformLocalToWorld(theView, &instanceData->point, &hitXYZ);
		qd3dStatus = E3Pick_RecordHit(thePick, theView, &hitXYZ, NULL, NULL, NULL);
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_point_pick_window_rect : Point window-rect picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_point_pick_window_rect(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3PointData			*instanceData = (const TQ3PointData *) objectData;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Point2D					windowPoint;
	TQ3WindowRectPickData		pickData;
	TQ3Point3D					hitXYZ;



	// Get the pick data
	Q3WindowRectPick_GetData(thePick, &pickData);



	// Transform our point to the screen
	Q3View_TransformLocalToWindow(theView, &instanceData->point, &windowPoint);



	// See if we fall within the pick
	if ((windowPoint.x >= pickData.rect.min.x) && (windowPoint.x <= pickData.rect.max.x) &&
		(windowPoint.y >= pickData.rect.min.y) && (windowPoint.y <= pickData.rect.max.y))
		{
		Q3View_TransformLocalToWorld(theView, &instanceData->point, &hitXYZ);
		qd3dStatus = E3Pick_RecordHit(thePick, theView, &hitXYZ, NULL, NULL, NULL);
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_point_pick_world_ray : Point world-ray picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_point_pick_world_ray(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3PointData			*instanceData = (const TQ3PointData *) objectData;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Sphere					theSphere;
	TQ3WorldRayPickData			pickData;
	TQ3Point3D					hitHYZ;



	// Get the pick data
	Q3WorldRayPick_GetData(thePick, &pickData);



	// Create a bounding sphere around the point
	Q3View_TransformLocalToWorld(theView, &instanceData->point, &theSphere.origin);
	theSphere.radius = pickData.vertexTolerance;



	// See if we fall within the pick
	if (Q3Ray3D_IntersectSphere(&pickData.ray, &theSphere, &hitHYZ))
		qd3dStatus = E3Pick_RecordHit(thePick, theView, &hitHYZ, NULL, NULL, NULL);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_point_pick : Point picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_point_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3PickObject		thePick;



	// Handle the pick
	thePick = E3View_AccessPick(theView);
	switch (Q3Pick_GetType(thePick)) {
		case kQ3PickTypeWindowPoint:
			qd3dStatus = e3geom_point_pick_window_point(theView, thePick, theObject, objectData);
			break;

		case kQ3PickTypeWindowRect:
			qd3dStatus = e3geom_point_pick_window_rect(theView, thePick, theObject, objectData);
			break;

		case kQ3PickTypeWorldRay:
			qd3dStatus = e3geom_point_pick_world_ray(theView, thePick, theObject, objectData);
			break;

		default:
			qd3dStatus = kQ3Failure;
			break;
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_point_bounds : Point bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_point_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3PointData			*instanceData = (const TQ3PointData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	E3View_UpdateBounds(theView, 1, 0, &instanceData->point);

	return(kQ3Success);
}





//=============================================================================
//      e3geom_point_get_attribute : Point get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_point_get_attribute(TQ3GeometryObject theObject)
{	TQ3PointData		*instanceData = (TQ3PointData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->pointAttributeSet);
}





//=============================================================================
//      e3geom_point_metahandler : Point metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_point_metahandler(TQ3XMethodType methodType)
{	
	TQ3XFunctionPointer		theMethod = NULL;

	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_point_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_point_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_point_duplicate;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_point_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_point_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_point_get_attribute;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryPoint_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryPoint_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
												kQ3GeometryTypePoint,
												kQ3ClassNameGeometryPoint,
												e3geom_point_metahandler,
												sizeof(TQ3PointData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryPoint_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryPoint_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypePoint, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Point_New : Create a point object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Point_New(const TQ3PointData *pointData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypePoint, kQ3False, pointData);
	return(theObject);
}





//=============================================================================
//      E3Point_Submit : Submit a point.
//-----------------------------------------------------------------------------
TQ3Status
E3Point_Submit(const TQ3PointData *pointData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypePoint, pointData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Point_GetData : Get the data describing the point
//-----------------------------------------------------------------------------
TQ3Status
E3Point_GetData(TQ3GeometryObject thePoint, TQ3PointData *pointData)
{
	TQ3PointData *		instanceData = (TQ3PointData *) thePoint->instanceData ;

	//return the data
	pointData->point = instanceData->point ;
	
	E3Shared_Acquire(&pointData->pointAttributeSet, instanceData->pointAttributeSet);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Point_SetData :	Set the data that describes a point
//-----------------------------------------------------------------------------
TQ3Status
E3Point_SetData(TQ3GeometryObject thePoint, const TQ3PointData *pointData)
{
	TQ3PointData *			instanceData = (TQ3PointData *) thePoint->instanceData ;
	
	//set the point
	instanceData->point	= pointData->point ;
	
	E3Shared_Replace(&instanceData->pointAttributeSet, pointData->pointAttributeSet);

	Q3Shared_Edited(thePoint);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Point_EmptyData :	release the memory occupied by the data structure 
//							returned by a previous call to Q3Point_GetData.
//-----------------------------------------------------------------------------
TQ3Status
E3Point_EmptyData(TQ3PointData *pointData)
{

	Q3Object_CleanDispose(&pointData->pointAttributeSet ) ;

	return(kQ3Success);
}





//=============================================================================
//      E3Point_SetPosition :	Set the position of the point
//-----------------------------------------------------------------------------
TQ3Status
E3Point_SetPosition(TQ3GeometryObject thePoint, const TQ3Point3D *position)
{
	TQ3PointData *			instanceData = (TQ3PointData *) thePoint->instanceData ;
	
	//set the position
	instanceData->point = *position ;

	Q3Shared_Edited(thePoint);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Point_GetPosition :	Get the position of the point
//-----------------------------------------------------------------------------
TQ3Status
E3Point_GetPosition(TQ3GeometryObject thePoint, TQ3Point3D *position)
{
	TQ3PointData *			instanceData = (TQ3PointData *) thePoint->instanceData ;

	*position = instanceData->point ;
	
	return(kQ3Success) ;
}



/*  NAME:
        E3GeometryPoint.c

    DESCRIPTION:
        Implementation of Quesa Point geometry class.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
#include "E3View.h"
#include "E3Pick.h"
#include "E3Geometry.h"
#include "E3GeometryPoint.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Point : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypePoint, E3Point, E3Geometry )
public :

	TQ3PointData			instanceData ;

	} ;
	


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
	TQ3AttributeSet		dupSet;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Point_GetData(fromObject, toInstanceData);
	
	if ( (qd3dStatus == kQ3Success) &&
		(toInstanceData->pointAttributeSet != NULL) )
	{
		dupSet = Q3Object_Duplicate( toInstanceData->pointAttributeSet );
		Q3Object_Dispose( toInstanceData->pointAttributeSet );
		toInstanceData->pointAttributeSet = dupSet;
		if (dupSet == NULL)
		{
			qd3dStatus = kQ3Failure;
		}
	}

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
	E3View_UpdateBounds(theView, 1, sizeof ( TQ3Point3D ), &instanceData->point);

	return(kQ3Success);
}





//=============================================================================
//      e3geom_point_get_attribute : Point get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_point_get_attribute ( E3Point* point )
	{
	// Return the address of the geometry attribute set
	return & point->instanceData.pointAttributeSet ;
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
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryPoint,
								e3geom_point_metahandler,
								E3Point ) ;
	}





//=============================================================================
//      E3GeometryPoint_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryPoint_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypePoint, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Point_New : Create a point object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Point_New(const TQ3PointData *pointData)
{	TQ3Object		theObject;

	
	if (pointData == NULL)
	{
		TQ3PointData defaultPt = {
			{ 0.0f, 0.0f, 0.0f }, NULL
		};
		theObject = E3ClassTree::CreateInstance (  kQ3GeometryTypePoint, kQ3False, &defaultPt );
	}
	else
	{
		theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypePoint, kQ3False, pointData);
	}

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
	E3Point* point = (E3Point*) thePoint ;

	//return the data
	pointData->point = point->instanceData.point ;
	
	E3Shared_Acquire ( & pointData->pointAttributeSet, point->instanceData.pointAttributeSet ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Point_SetData :	Set the data that describes a point
//-----------------------------------------------------------------------------
TQ3Status
E3Point_SetData(TQ3GeometryObject thePoint, const TQ3PointData *pointData)
	{
	E3Point* point = (E3Point*) thePoint ;
	
	//set the point
	point->instanceData.point = pointData->point ;
	
	E3Shared_Replace ( & point->instanceData.pointAttributeSet, pointData->pointAttributeSet ) ;

	Q3Shared_Edited ( point ) ;
	
	return kQ3Success ;
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
	E3Point* point = (E3Point*) thePoint ;
	
	//set the position
	point->instanceData.point = *position ;

	Q3Shared_Edited ( point ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Point_GetPosition :	Get the position of the point
//-----------------------------------------------------------------------------
TQ3Status
E3Point_GetPosition(TQ3GeometryObject thePoint, TQ3Point3D *position)
	{
	E3Point* point = (E3Point*) thePoint ;

	*position = point->instanceData.point ;
	
	return kQ3Success ;
	}



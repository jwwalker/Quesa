/*  NAME:
        E3GeometryLine.c

    DESCRIPTION:
        Implementation of Quesa Line geometry class.

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
#include "E3Pick.h"
#include "E3Geometry.h"
#include "E3GeometryLine.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_line_ray_intersect : Intersect a line with a ray.
//-----------------------------------------------------------------------------
//		Note :	Returns the smallest segment between a line and a ray. This
//				segment starts from a point on theLine, and ends on a point on
//				theRay. Algorithm obtained from:
//				http://astronomy.swin.edu.au/pbourke/geometry/lineline3d/
//-----------------------------------------------------------------------------
static TQ3Boolean
e3geom_line_ray_intersect(const TQ3Point3D 		linePoints[2],
							const TQ3Ray3D		*theRay,
							TQ3Point3D			smallestSegment[2])
{	float			d1, d2, d1343, d4321, d1321, d4343, d2121;
	TQ3Vector3D		raySegment, rayLine1, rayLine2;
	float			numer, denom, mua, mub;
	TQ3Point3D		p1, p2, p3, p4, pa, pb;
	TQ3Point3D		p13, p43, p21;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(linePoints),      kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRay),          kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(smallestSegment), kQ3False);



	// Grab a copy of the line
	p1 = linePoints[0];
	p2 = linePoints[1];



	// Grab a copy of the ray. We convert the ray to a line segment that
	// will overshoot the line intersection (if one exists) by extending
	// the ray from its origin out past the distance to the furthest
	// vertex.
	p3 = theRay->origin;
	
	Q3Point3D_Subtract(&p1, &p3, &rayLine1);
	Q3Point3D_Subtract(&p2, &p3, &rayLine2);

	d1 = Q3Vector3D_Length(&rayLine1);
	d2 = Q3Vector3D_Length(&rayLine2);
	
	Q3Vector3D_Scale(&theRay->direction, E3Num_Max(d1, d2) * 3.0f, &raySegment);
	Q3Point3D_Vector3D_Add(&p3, &raySegment, &p4);



	// Calculate the line segment PaPb that is the shortest route between
	// two lines P1P2 and P3P4. Calculate also the values of mua and mub where
	//
	//		Pa = P1 + mua (P2 - P1)
	//		Pb = P3 + mub (P4 - P3)
	//
	// Returns kQ3False if no solution exists
	p13.x = p1.x - p3.x;
	p13.y = p1.y - p3.y;
	p13.z = p1.z - p3.z;
	p43.x = p4.x - p3.x;
	p43.y = p4.y - p3.y;
	p43.z = p4.z - p3.z;
	if (E3Float_Abs(p43.x) < kQ3RealZero && E3Float_Abs(p43.y) < kQ3RealZero && E3Float_Abs(p43.z) < kQ3RealZero)
		return(kQ3False);

	p21.x = p2.x - p1.x;
	p21.y = p2.y - p1.y;
	p21.z = p2.z - p1.z;
	if (E3Float_Abs(p21.x) < kQ3RealZero && E3Float_Abs(p21.y) < kQ3RealZero && E3Float_Abs(p21.z) < kQ3RealZero)
		return(kQ3False);

	d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
	d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
	d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
	d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
	d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

	denom = d2121 * d4343 - d4321 * d4321;
	if (E3Float_Abs(denom) < kQ3RealZero)
		return(kQ3False);
	numer = d1343 * d4321 - d1321 * d4343;

	mua = numer / denom;
	mub = (d1343 + d4321 * (mua)) / d4343;

	pa.x = p1.x + mua * p21.x;
	pa.y = p1.y + mua * p21.y;
	pa.z = p1.z + mua * p21.z;
	pb.x = p3.x + mub * p43.x;
	pb.y = p3.y + mub * p43.y;
	pb.z = p3.z + mub * p43.z;



	// Set up the returning segment. pa is the point on theLine, so
	// the segment we want runs from pa to pb.
	smallestSegment[0] = pa;
	smallestSegment[1] = pb;

	return(kQ3True);
}





//=============================================================================
//      e3geom_line_new : Line new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_line_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3LineData			*instanceData	= (TQ3LineData *)		privateData;
	const TQ3LineData 	*lineData 		= (const TQ3LineData *)	paramData;
	TQ3Status			qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3LineData));
	
	qd3dStatus = Q3Line_SetData(theObject, lineData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_line_delete : Line delete method.
//-----------------------------------------------------------------------------
static void
e3geom_line_delete(TQ3Object theObject, void *privateData)
{	TQ3LineData			*instanceData = (TQ3LineData *) privateData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3Line_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_line_duplicate : Line duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_line_duplicate(TQ3Object fromObject, const void *fromPrivateData,
					  TQ3Object toObject,   void *toPrivateData)
{	TQ3LineData			*toInstanceData = (TQ3LineData *) toPrivateData;
	TQ3Status			qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Line_GetData(fromObject, toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_line_pick_window_point : Line window-point picking method.
//-----------------------------------------------------------------------------
//		Note :	Point-line intersection from comp.graphics.algorithms FAQ.
//				<http://www.faqs.org/faqs/graphics/algorithms-faq/>
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_line_pick_window_point(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3LineData			*instanceData = (const TQ3LineData *) objectData;
	TQ3Vector2D					windowStartToPick, windowStartToEnd, windowHitToPick;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Point2D					hitXY,  windowPoints[2];
	TQ3Point3D					hitXYZ, worldPoints[2];
	TQ3Vector3D					worldStartToEnd;
	TQ3WindowPointPickData		pickData;
	float						d;



	// Get the pick data
	Q3WindowPointPick_GetData(thePick, &pickData);



	// Transform our points
	Q3View_TransformLocalToWindow(theView, &instanceData->vertices[0].point, &windowPoints[0]);
	Q3View_TransformLocalToWindow(theView, &instanceData->vertices[1].point, &windowPoints[1]);



	// Calculate the distance d along the line to its closest point to the pick point.
	// If it's outside the range 0-1 then the closest point is before or after the
	// bounds of the line and so we know we can't possibly have a hit.
	Q3Point2D_Subtract(&pickData.point,  &windowPoints[0], &windowStartToPick);
	Q3Point2D_Subtract(&windowPoints[1], &windowPoints[0], &windowStartToEnd);

	d = Q3Vector2D_Dot(&windowStartToPick, &windowStartToEnd) / Q3Vector2D_LengthSquared(&windowStartToEnd);
	if (d < 0.0f || d > 1.0f)
		return(kQ3Success);



	// Scale the line vector by d to obtain its closest point to the pick point
	Q3Vector2D_Scale(&windowStartToEnd, d,   &windowStartToEnd);
	Q3Point2D_Vector2D_Add(&windowPoints[0], &windowStartToEnd, &hitXY);



	// Get the distance between that point and the pick point
	Q3Point2D_Subtract(&pickData.point, &hitXY, &windowHitToPick);
	d = Q3Vector2D_Length(&windowHitToPick);



	// See if we fall within the pick
	if (d <= pickData.edgeTolerance)
		{
		// Calculate the intersection point on the 3D line
		Q3View_TransformLocalToWorld(theView,  &instanceData->vertices[0].point, &worldPoints[0]);
		Q3View_TransformLocalToWorld(theView,  &instanceData->vertices[1].point, &worldPoints[1]);

		Q3Point3D_Subtract(&worldPoints[1], &worldPoints[0], &worldStartToEnd);
		Q3Vector3D_Scale(&worldStartToEnd, d,   &worldStartToEnd);
		Q3Point3D_Vector3D_Add(&worldPoints[0], &worldStartToEnd, &hitXYZ);


		// Record the hit
		qd3dStatus = E3Pick_RecordHit(thePick, theView, &hitXYZ, NULL, NULL, NULL);
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_line_pick_window_rect : Line window-rect picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_line_pick_window_rect(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3LineData			*instanceData = (const TQ3LineData *) objectData;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Point2D					windowPoints[2];
	TQ3WindowRectPickData		pickData;



	// Get the pick data
	Q3WindowRectPick_GetData(thePick, &pickData);



	// Transform our points
	Q3View_TransformLocalToWindow(theView, &instanceData->vertices[0].point, &windowPoints[0]);
	Q3View_TransformLocalToWindow(theView, &instanceData->vertices[1].point, &windowPoints[1]);



	// See if we fall within the pick
	if (E3Rect_ContainsLine(&pickData.rect, &windowPoints[0], &windowPoints[1]))
		qd3dStatus = E3Pick_RecordHit(thePick, theView, NULL, NULL, NULL, NULL);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_line_pick_world_ray : Line world-ray picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_line_pick_world_ray(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3LineData			*instanceData = (const TQ3LineData *) objectData;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Point3D					worldSegment[2];
	TQ3Point3D					worldPoints[2];
	TQ3Vector3D					theVector;
	TQ3WorldRayPickData			pickData;
	float						d;



	// Get the pick data
	Q3WorldRayPick_GetData(thePick, &pickData);



	// Transform our points
	Q3View_TransformLocalToWorld(theView, &instanceData->vertices[0].point, &worldPoints[0]);
	Q3View_TransformLocalToWorld(theView, &instanceData->vertices[1].point, &worldPoints[1]);



	// Test for an intersection
	if (!e3geom_line_ray_intersect(worldPoints, &pickData.ray, worldSegment))
		return(qd3dStatus);



	// Calculate the distance between the closest point and the ray
	Q3Point3D_Subtract(&worldSegment[1], &worldSegment[0], &theVector);
	d = Q3Vector3D_Length(&theVector);



	// See if we fall within the pick
	if (d <= pickData.edgeTolerance)
		qd3dStatus = E3Pick_RecordHit(thePick, theView, &worldSegment[0], NULL, NULL, NULL);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_line_pick : Line picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_line_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3PickObject		thePick;



	// Handle the pick
	thePick = E3View_AccessPick(theView);
	switch (Q3Pick_GetType(thePick)) {
		case kQ3PickTypeWindowPoint:
			qd3dStatus = e3geom_line_pick_window_point(theView, thePick, theObject, objectData);
			break;

		case kQ3PickTypeWindowRect:
			qd3dStatus = e3geom_line_pick_window_rect(theView, thePick, theObject, objectData);
			break;

		case kQ3PickTypeWorldRay:
			qd3dStatus = e3geom_line_pick_world_ray(theView, thePick, theObject, objectData);
			break;

		default:
			qd3dStatus = kQ3Failure;
			break;
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_line_bounds : Line bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_line_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3LineData		*instanceData = (const TQ3LineData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	E3View_UpdateBounds(theView, 2, sizeof(TQ3Vertex3D), &instanceData->vertices[0].point);

	return(kQ3Success);
}





//=============================================================================
//      e3geom_line_get_attribute : Line get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_line_get_attribute(TQ3GeometryObject theObject)
{	TQ3LineData			*instanceData = (TQ3LineData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->lineAttributeSet);
}





//=============================================================================
//      e3geom_line_metahandler : Line metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_line_metahandler(TQ3XMethodType methodType)
{	
	TQ3XFunctionPointer		theMethod = NULL;

	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_line_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_line_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_line_duplicate;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_line_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_line_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_line_get_attribute;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryLine_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryLine_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
												kQ3GeometryTypeLine,
												kQ3ClassNameGeometryLine,
												e3geom_line_metahandler,
												sizeof(TQ3LineData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryLine_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryLine_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeLine, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Line_New : Create a line object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Line_New(const TQ3LineData *lineData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeLine, kQ3False, lineData);
	return(theObject);
}





//=============================================================================
//      E3Line_Submit : Submit a line.
//-----------------------------------------------------------------------------
TQ3Status
E3Line_Submit(const TQ3LineData *lineData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeLine, lineData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Line_GetData :	Get the data that describes theLine
//-----------------------------------------------------------------------------
//		Note :	Allocates memory for lineData internally, you must call
//				Q3Line_EmptyData to dispose of this memory
//-----------------------------------------------------------------------------
TQ3Status
E3Line_GetData(TQ3GeometryObject theLine, TQ3LineData *lineData)
{
	TQ3LineData	*		instanceData = (TQ3LineData *) theLine->instanceData;
	TQ3Uns32			n;


	//return the data
	for(n = 0; n < 2; n++)
	{
		lineData->vertices[n].point = instanceData->vertices[n].point ;
		E3Shared_Acquire(&lineData->vertices[n].attributeSet, instanceData->vertices[n].attributeSet);
	}
	
	E3Shared_Acquire(&lineData->lineAttributeSet, instanceData->lineAttributeSet);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Line_SetData :	Set the data describing a line and its attributes
//-----------------------------------------------------------------------------
TQ3Status
E3Line_SetData(TQ3GeometryObject theLine, const TQ3LineData *lineData)
{
	TQ3LineData	*		instanceData = (TQ3LineData *) theLine->instanceData;
	TQ3Uns32			n;


	//return the data
	for(n = 0; n < 2; n++)
	{
		instanceData->vertices[n].point = lineData->vertices[n].point ;
		E3Shared_Acquire(&instanceData->vertices[n].attributeSet, lineData->vertices[n].attributeSet);
	}
	

	E3Shared_Replace(&instanceData->lineAttributeSet, lineData->lineAttributeSet);
	Q3Shared_Edited(theLine);

	return(kQ3Success);
}





//=============================================================================
//      E3Line_GetVertexPosition :	Get the vertex position at index
//-----------------------------------------------------------------------------
TQ3Status
E3Line_GetVertexPosition(TQ3GeometryObject theLine, TQ3Uns32 index, TQ3Point3D *position)
{
	TQ3LineData	*		instanceData = (TQ3LineData *) theLine->instanceData;

	//get the position	
	*position = instanceData->vertices[index].point ;

	return(kQ3Success);
}





//=============================================================================
//      E3Line_SetVertexPosition :	Set the vertex at index
//-----------------------------------------------------------------------------
TQ3Status
E3Line_SetVertexPosition(TQ3GeometryObject theLine, TQ3Uns32 index, const TQ3Point3D *position)
{
	TQ3LineData	*		instanceData = (TQ3LineData *) theLine->instanceData;
	
	
	//set the position
	instanceData->vertices[index].point = *position ;
	Q3Shared_Edited(theLine);

	return(kQ3Success);
}





//=============================================================================
//      E3Line_GetVertexAttributeSet :	Get attributes for vertex at index
//-----------------------------------------------------------------------------
TQ3Status
E3Line_GetVertexAttributeSet(TQ3GeometryObject theLine, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
{
	TQ3LineData	*		instanceData = (TQ3LineData *) theLine->instanceData;


	// Return the attribute set
	E3Shared_Acquire(attributeSet, instanceData->vertices[index].attributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Line_SetVertexAttributeSet : Set the attributes for vertex at index.
//-----------------------------------------------------------------------------
TQ3Status
E3Line_SetVertexAttributeSet(TQ3GeometryObject theLine, TQ3Uns32 index, TQ3AttributeSet attributeSet)
{
	TQ3LineData	*		instanceData = (TQ3LineData *) theLine->instanceData;


	// Set the attribute set
	E3Shared_Replace(&instanceData->vertices[index].attributeSet, attributeSet);
	Q3Shared_Edited(theLine);

	return(kQ3Success);
}





//=============================================================================
//      E3Line_EmptyData : Empty the data for a line object
//-----------------------------------------------------------------------------
//		Note :	The memory for lineData was allocated by Q3Line_GetData
//-----------------------------------------------------------------------------
TQ3Status
E3Line_EmptyData(TQ3LineData *lineData)
{
	TQ3Uns32		n;



	// Release the data
	for (n = 0; n < 2; n++)
		E3Object_DisposeAndForget(lineData->vertices[n].attributeSet);

	E3Object_DisposeAndForget(lineData->lineAttributeSet);

	return(kQ3Success);
}



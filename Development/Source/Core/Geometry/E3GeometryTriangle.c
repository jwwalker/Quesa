/*  NAME:
        E3GeometryTriangle.c

    DESCRIPTION:
        Implementation of Quesa Triangle geometry class.

    COPYRIGHT:
        Quesa Copyright � 1999-2002, Quesa Developers.
        
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
#include "E3GeometryTriangle.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_triangle_new : Triangle new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_triangle_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3TriangleData				*instanceData = (TQ3TriangleData *)       privateData;
	const TQ3TriangleData		*triangleData = (const TQ3TriangleData *) paramData;
	TQ3Status					qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3TriangleData));
	
	qd3dStatus = Q3Triangle_SetData(theObject, triangleData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_triangle_delete : Triangle delete method.
//-----------------------------------------------------------------------------
static void
e3geom_triangle_delete(TQ3Object theObject, void *privateData)
{	TQ3TriangleData			*instanceData = (TQ3TriangleData *) privateData;
	TQ3Status				qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3Triangle_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_triangle_duplicate : Triangle duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_triangle_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	TQ3TriangleData		*toInstanceData = (TQ3TriangleData *) toPrivateData;
	TQ3Status			qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Triangle_GetData(fromObject, toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_triangle_pick_with_ray : Triangle ray picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_triangle_pick_with_ray(TQ3ViewObject			theView,
								TQ3PickObject		thePick,
								const TQ3Ray3D		*theRay,
								TQ3Object			theObject,
								const void			*objectData)
{	const TQ3TriangleData		*instanceData = (const TQ3TriangleData *) objectData;
	TQ3Boolean					haveUV, cullBackface;
	TQ3Param2D					hitUV, *resultUV;
	TQ3BackfacingStyle			backfacingStyle;
	TQ3Point3D					worldPoints[3];
	TQ3TriangleData				worldTriangle;
	TQ3Status					qd3dStatus;
	TQ3Vector3D					hitNormal;
	TQ3Point3D					hitXYZ;
	TQ3Param3D					theHit;
	TQ3Uns32					n;



	// Transform our points
	for (n = 0; n < 3; n++)
		Q3View_TransformLocalToWorld(theView, &instanceData->vertices[n].point, &worldPoints[n]);



	// Determine if we should cull back-facing triangles or not
	qd3dStatus   = E3View_GetBackfacingStyleState(theView, &backfacingStyle);
	cullBackface = (TQ3Boolean)(qd3dStatus == kQ3Success && backfacingStyle == kQ3BackfacingStyleRemove);



	// See if we fall within the pick
	//
	// Note we do not use any vertex/edge tolerances supplied for the pick, since
	// QD3D's blue book appears to suggest neither are used for triangles.
	if (Q3Ray3D_IntersectTriangle(theRay, &worldPoints[0], &worldPoints[1], &worldPoints[2], cullBackface, &theHit))
		{
		// Set up a temporary triangle that holds the world points
		worldTriangle = *instanceData;
		for (n = 0; n < 3; n++)
			worldTriangle.vertices[n].point = worldPoints[n];


		// Obtain the XYZ, normal, and UV for the hit point. We always return an
		// XYZ and normal for the hit, however we need to cope with missing UVs.
		E3Triangle_InterpolateHit(&worldTriangle, &theHit, &hitXYZ, &hitNormal, &hitUV, &haveUV);
		resultUV = (haveUV ? &hitUV : NULL);


		// Record the hit
		qd3dStatus = E3Pick_RecordHit(thePick, theView, &hitXYZ, &hitNormal, resultUV, NULL);
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_triangle_pick_window_point : Triangle window-point picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_triangle_pick_window_point(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	TQ3Status		qd3dStatus;
	TQ3Ray3D		theRay;



	// Get the ray through the pick point
	E3View_GetRayThroughPickPoint(theView, &theRay);



	// Pick using the ray
	qd3dStatus = e3geom_triangle_pick_with_ray(theView, thePick, &theRay, theObject, objectData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_triangle_pick_window_rect : Triangle window-rect picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_triangle_pick_window_rect(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3TriangleData		*instanceData = (const TQ3TriangleData *) objectData;
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Point2D					windowPoints[3];
	TQ3WindowRectPickData		pickData;
	TQ3Uns32					n;
	


	// Get the pick data
	Q3WindowRectPick_GetData(thePick, &pickData);



	// Transform our points
	for (n = 0; n < 3; n++)
		Q3View_TransformLocalToWindow(theView, &instanceData->vertices[n].point, &windowPoints[n]);



	// See if we fall within the pick
	if (E3Rect_ContainsLine(&pickData.rect, &windowPoints[0], &windowPoints[1]) ||
		E3Rect_ContainsLine(&pickData.rect, &windowPoints[0], &windowPoints[2]) ||
		E3Rect_ContainsLine(&pickData.rect, &windowPoints[1], &windowPoints[2]))
		qd3dStatus = E3Pick_RecordHit(thePick, theView, NULL, NULL, NULL, NULL);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_triangle_pick_world_ray : Triangle world-ray picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_triangle_pick_world_ray(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	TQ3Status				qd3dStatus;
	TQ3WorldRayPickData		pickData;



	// Get the pick data
	Q3WorldRayPick_GetData(thePick, &pickData);



	// Pick using the ray
	qd3dStatus = e3geom_triangle_pick_with_ray(theView, thePick, &pickData.ray, theObject, objectData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_triangle_pick : Triangle picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_triangle_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3PickObject		thePick;



	// Handle the pick
	thePick = E3View_AccessPick(theView);
	switch (Q3Pick_GetType(thePick)) {
		case kQ3PickTypeWindowPoint:
			qd3dStatus = e3geom_triangle_pick_window_point(theView, thePick, theObject, objectData);
			break;

		case kQ3PickTypeWindowRect:
			qd3dStatus = e3geom_triangle_pick_window_rect(theView, thePick, theObject, objectData);
			break;

		case kQ3PickTypeWorldRay:
			qd3dStatus = e3geom_triangle_pick_world_ray(theView, thePick, theObject, objectData);
			break;

		default:
			qd3dStatus = kQ3Failure;
			break;
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_triangle_bounds : Triangle bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_triangle_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3TriangleData			*instanceData = (const TQ3TriangleData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	E3View_UpdateBounds(theView, 3, sizeof(TQ3Vertex3D), &instanceData->vertices[0].point);
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_triangle_get_attribute : Triangle get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_triangle_get_attribute(TQ3GeometryObject theObject)
{	TQ3TriangleData		*instanceData = (TQ3TriangleData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->triangleAttributeSet);
}





//=============================================================================
//      e3geom_triangle_metahandler : Triangle metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_triangle_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_triangle_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_triangle_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_triangle_duplicate;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_triangle_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_triangle_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_triangle_get_attribute;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryTriangle_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryTriangle_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeTriangle,
											kQ3ClassNameGeometryTriangle,
											e3geom_triangle_metahandler,
											sizeof(TQ3TriangleData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryTriangle_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryTriangle_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeTriangle, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Triangle_New : Create a triangle object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Triangle_New(const TQ3TriangleData *triangleData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeTriangle, kQ3False, triangleData);
	return(theObject);
}





//=============================================================================
//      E3Triangle_Submit : Submit a triangle.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_Submit(const TQ3TriangleData *triangleData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeTriangle, triangleData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Triangle_SetData : Set the data for a triangle object.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_SetData(TQ3GeometryObject theTriangle, const TQ3TriangleData *triangleData)
{	TQ3TriangleData		*instanceData = (TQ3TriangleData *) theTriangle->instanceData;
	TQ3Uns32			n;



	// Set the data
	for (n = 0; n < 3; n++)
		{
		instanceData->vertices[n].point = triangleData->vertices[n].point;
		E3Shared_Replace(&instanceData->vertices[n].attributeSet,
						  triangleData->vertices[n].attributeSet);
		}

	E3Shared_Replace(&instanceData->triangleAttributeSet, triangleData->triangleAttributeSet);

	Q3Shared_Edited(theTriangle);

	return(kQ3Success);
}





//=============================================================================
//      E3Triangle_GetData : Return the data for a triangle object.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_GetData(TQ3GeometryObject theTriangle, TQ3TriangleData *triangleData)
{	TQ3TriangleData		*instanceData = (TQ3TriangleData *) theTriangle->instanceData;
	TQ3Uns32			n;



	// Return the data
	for (n = 0; n < 3; n++)
		{
		triangleData->vertices[n].point = instanceData->vertices[n].point;

		E3Shared_Acquire(&triangleData->vertices[n].attributeSet,
						  instanceData->vertices[n].attributeSet);
		}

	E3Shared_Acquire(&triangleData->triangleAttributeSet, instanceData->triangleAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Triangle_EmptyData : Empty the data for a triangle object.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_EmptyData(TQ3TriangleData *triangleData)
{	TQ3Uns32		n;



	// Release the data
	for (n = 0; n < 3; n++)
		E3Object_DisposeAndForget(triangleData->vertices[n].attributeSet);

	E3Object_DisposeAndForget(triangleData->triangleAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Triangle_GetVertexPosition : Return the position for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_GetVertexPosition(TQ3GeometryObject theTriangle, TQ3Uns32 index, TQ3Point3D *point)
{	TQ3TriangleData		*instanceData = (TQ3TriangleData *) theTriangle->instanceData;



	// Return the position
	*point = instanceData->vertices[index].point;
	return(kQ3Success);
}





//=============================================================================
//      E3Triangle_SetVertexPosition : Set the position for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_SetVertexPosition(TQ3GeometryObject theTriangle, TQ3Uns32 index, const TQ3Point3D *point)
{	TQ3TriangleData		*instanceData = (TQ3TriangleData *) theTriangle->instanceData;



	// Set the position
	instanceData->vertices[index].point = *point;

	Q3Shared_Edited(theTriangle);

	return(kQ3Success);
}





//=============================================================================
//      E3Triangle_GetVertexAttributeSet : Return the attributes for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_GetVertexAttributeSet(TQ3GeometryObject theTriangle, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
{	TQ3TriangleData		*instanceData = (TQ3TriangleData *) theTriangle->instanceData;



	// Return the attribute set
	E3Shared_Acquire(attributeSet, instanceData->vertices[index].attributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Triangle_SetVertexAttributeSet : Set the attributes for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_SetVertexAttributeSet(TQ3GeometryObject theTriangle, TQ3Uns32 index, TQ3AttributeSet attributeSet)
{	TQ3TriangleData		*instanceData = (TQ3TriangleData *) theTriangle->instanceData;



	// Set the attribute set
	E3Shared_Replace(&instanceData->vertices[index].attributeSet, attributeSet);
	Q3Shared_Edited(theTriangle);

	return(kQ3Success);
}




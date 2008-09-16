/*  NAME:
        E3GeometryTriangle.c

    DESCRIPTION:
        Implementation of Quesa Triangle geometry class.

    COPYRIGHT:
        Copyright (c) 1999-2008, Quesa Developers. All rights reserved.

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
#include "E3GeometryTriangle.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Triangle : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypeTriangle, E3Triangle, E3Geometry )
public :

	TQ3TriangleData			instanceData ;

	} ;
	


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
	TQ3AttributeSet		dupSet;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Triangle_GetData(fromObject, toInstanceData);
	
	if ( (qd3dStatus == kQ3Success) &&
		(toInstanceData->triangleAttributeSet != NULL) )
	{
		dupSet = Q3Object_Duplicate( toInstanceData->triangleAttributeSet );
		Q3Object_Dispose( toInstanceData->triangleAttributeSet );
		toInstanceData->triangleAttributeSet = dupSet;
		if (dupSet == NULL)
		{
			qd3dStatus = kQ3Failure;
		}
	}

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
		E3Triangle_InterpolateHit(theView,&worldTriangle, &theHit, &hitXYZ, &hitNormal, &hitUV, &haveUV);
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
	TQ3Point3D					windowCoordPts[3];
	TQ3WindowRectPickData		pickData;
	TQ3Uns32					n;
	


	// Get the pick data
	Q3WindowRectPick_GetData(thePick, &pickData);



	// Transform our points
	TQ3Matrix4x4	frustumToWindow, localToWindow;
	const TQ3Matrix4x4&	localToFrustum( E3View_State_GetMatrixLocalToFrustum( theView ) );
	E3View_GetFrustumToWindowMatrixState( theView, &frustumToWindow );
	Q3Matrix4x4_Multiply( &localToFrustum, &frustumToWindow, &localToWindow );
	for (n = 0; n < 3; n++)
	{
		Q3Point3D_Transform( &instanceData->vertices[n].point, &localToWindow,
			&windowCoordPts[n] );
		windowPoints[n].x = windowCoordPts[n].x;
		windowPoints[n].y = windowCoordPts[n].y;
	}



	// See if we fall within the pick
	const TQ3Matrix4x4 *localToWorld = E3View_State_GetMatrixLocalToWorld( theView );
	TQ3Point3D	worldHit;
	
	if (E3Rect_ContainsLine(&pickData.rect, &windowPoints[0], &windowPoints[1]) ||
		E3Rect_ContainsLine(&pickData.rect, &windowPoints[0], &windowPoints[2]))
	{
		Q3Point3D_Transform( &instanceData->vertices[0].point, localToWorld,
			&worldHit );
		qd3dStatus = E3Pick_RecordHit(thePick, theView, &worldHit, NULL, NULL, NULL);
	}
	else if (E3Rect_ContainsLine(&pickData.rect, &windowPoints[1], &windowPoints[2]))
	{
		Q3Point3D_Transform( &instanceData->vertices[1].point, localToWorld,
			&worldHit );
		qd3dStatus = E3Pick_RecordHit(thePick, theView, &worldHit, NULL, NULL, NULL);
	}

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
e3geom_triangle_get_attribute ( E3Triangle* triangle )
	{
	// Return the address of the geometry attribute set
	return & triangle->instanceData.triangleAttributeSet ;
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
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryTriangle,
								e3geom_triangle_metahandler,
								E3Triangle ) ;
	}





//=============================================================================
//      E3GeometryTriangle::UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryTriangle_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypeTriangle, kQ3True);
	
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
	theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeTriangle, kQ3False, triangleData);
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
	{
	E3Triangle* triangle = (E3Triangle*) theTriangle ;

	// Set the data
	for ( TQ3Uns32 n = 0 ; n < 3 ; ++n )
		{
		triangle->instanceData.vertices [ n ].point = triangleData->vertices [ n ].point ;
		E3Shared_Replace ( & triangle->instanceData.vertices [ n ].attributeSet,
							 triangleData->vertices [ n ].attributeSet ) ;
		}

	E3Shared_Replace ( & triangle->instanceData.triangleAttributeSet, triangleData->triangleAttributeSet ) ;

	Q3Shared_Edited ( triangle ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Triangle_GetData : Return the data for a triangle object.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_GetData(TQ3GeometryObject theTriangle, TQ3TriangleData *triangleData)
	{
	E3Triangle* triangle = (E3Triangle*) theTriangle ;

	// Return the data
	for ( TQ3Uns32 n = 0 ; n < 3 ; ++n )
		{
		triangleData->vertices [ n ].point = triangle->instanceData.vertices [ n ].point ;

		E3Shared_Acquire ( & triangleData->vertices [ n ].attributeSet,
							 triangle->instanceData.vertices [ n ].attributeSet ) ;
		}

	E3Shared_Acquire ( & triangleData->triangleAttributeSet, triangle->instanceData.triangleAttributeSet ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Triangle_EmptyData : Empty the data for a triangle object.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_EmptyData(TQ3TriangleData *triangleData)
{	TQ3Uns32		n;



	// Release the data
	for (n = 0; n < 3; n++)
		Q3Object_CleanDispose(&triangleData->vertices[n].attributeSet);

	Q3Object_CleanDispose(&triangleData->triangleAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Triangle_GetVertexPosition : Return the position for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_GetVertexPosition(TQ3GeometryObject theTriangle, TQ3Uns32 index, TQ3Point3D *point)
	{
	E3Triangle* triangle = (E3Triangle*) theTriangle ;


	// Return the position
	*point = triangle->instanceData.vertices [ index ].point ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Triangle_SetVertexPosition : Set the position for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_SetVertexPosition(TQ3GeometryObject theTriangle, TQ3Uns32 index, const TQ3Point3D *point)
	{
	E3Triangle* triangle = (E3Triangle*) theTriangle ;

	// Set the position
	triangle->instanceData.vertices [ index ].point = *point ;

	Q3Shared_Edited ( triangle ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Triangle_GetVertexAttributeSet : Return the attributes for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_GetVertexAttributeSet(TQ3GeometryObject theTriangle, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
	{
	E3Triangle* triangle = (E3Triangle*) theTriangle ;

	// Return the attribute set
	E3Shared_Acquire ( attributeSet, triangle->instanceData.vertices [ index ].attributeSet ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Triangle_SetVertexAttributeSet : Set the attributes for a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3Triangle_SetVertexAttributeSet(TQ3GeometryObject theTriangle, TQ3Uns32 index, TQ3AttributeSet attributeSet)
	{
	E3Triangle* triangle = (E3Triangle*) theTriangle ;

	// Set the attribute set
	E3Shared_Replace ( & triangle->instanceData.vertices [ index ].attributeSet, attributeSet ) ;

	Q3Shared_Edited ( triangle ) ;

	return kQ3Success ;
	}




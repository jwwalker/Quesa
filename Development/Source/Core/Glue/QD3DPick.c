/*  NAME:
        QD3DPick.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
#include "E3Pick.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3Pick_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
Q3Pick_GetType(TQ3PickObject pick)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pick_GetType(pick));
}





//=============================================================================
//      Q3Pick_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pick_GetData(TQ3PickObject pick, TQ3PickData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pick_GetData(pick, data));
}





//=============================================================================
//      Q3Pick_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pick_SetData(TQ3PickObject pick, const TQ3PickData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pick_SetData(pick, data));
}





//=============================================================================
//      Q3Pick_GetVertexTolerance : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pick_GetVertexTolerance(TQ3PickObject pick, float *vertexTolerance)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vertexTolerance), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on vertexTolerance
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pick_GetVertexTolerance(pick, vertexTolerance));
}





//=============================================================================
//      Q3Pick_GetEdgeTolerance : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pick_GetEdgeTolerance(TQ3PickObject pick, float *edgeTolerance)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(edgeTolerance), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on edgeTolerance
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pick_GetEdgeTolerance(pick, edgeTolerance));
}





//=============================================================================
//      Q3Pick_SetVertexTolerance : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pick_SetVertexTolerance(TQ3PickObject pick, float vertexTolerance)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on vertexTolerance
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pick_SetVertexTolerance(pick, vertexTolerance));
}





//=============================================================================
//      Q3Pick_SetEdgeTolerance : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pick_SetEdgeTolerance(TQ3PickObject pick, float edgeTolerance)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on edgeTolerance
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pick_SetEdgeTolerance(pick, edgeTolerance));
}





//=============================================================================
//      Q3Pick_GetNumHits : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pick_GetNumHits(TQ3PickObject pick, TQ3Uns32 *numHits)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numHits), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on numHits
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pick_GetNumHits(pick, numHits));
}





//=============================================================================
//      Q3Pick_EmptyHitList : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pick_EmptyHitList(TQ3PickObject pick)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pick_EmptyHitList(pick));
}





//=============================================================================
//      Q3Pick_GetPickDetailValidMask : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pick_GetPickDetailValidMask(TQ3PickObject pick, TQ3Uns32 index, TQ3PickDetail *pickDetailValidMask)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pickDetailValidMask), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on pickDetailValidMask
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pick_GetPickDetailValidMask(pick, index, pickDetailValidMask));
}





//=============================================================================
//      Q3Pick_GetPickDetailData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pick_GetPickDetailData(TQ3PickObject pick, TQ3Uns32 index, TQ3PickDetail pickDetailValue, void *detailData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(detailData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on pickDetailValue
		return(kQ3Failure);

	if (0) // Further checks on detailData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Pick_GetPickDetailData(pick, index, pickDetailValue, detailData));
}





//=============================================================================
//      Q3HitPath_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3HitPath_EmptyData(TQ3HitPath *hitPath)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(hitPath), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on hitPath
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3HitPath_EmptyData(hitPath));
}



#pragma mark -

//=============================================================================
//      Q3WindowPointPick_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3PickObject
Q3WindowPointPick_New(const TQ3WindowPointPickData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WindowPointPick_New(data));
}





//=============================================================================
//      Q3WindowPointPick_GetPoint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WindowPointPick_GetPoint(TQ3PickObject pick, TQ3Point2D *point)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on point
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WindowPointPick_GetPoint(pick, point));
}





//=============================================================================
//      Q3WindowPointPick_SetPoint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WindowPointPick_SetPoint(TQ3PickObject pick, const TQ3Point2D *point)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on point
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WindowPointPick_SetPoint(pick, point));
}





//=============================================================================
//      Q3WindowPointPick_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WindowPointPick_GetData(TQ3PickObject pick, TQ3WindowPointPickData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WindowPointPick_GetData(pick, data));
}





//=============================================================================
//      Q3WindowPointPick_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WindowPointPick_SetData(TQ3PickObject pick, const TQ3WindowPointPickData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WindowPointPick_SetData(pick, data));
}



#pragma mark -

//=============================================================================
//      Q3WindowRectPick_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3PickObject
Q3WindowRectPick_New(const TQ3WindowRectPickData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WindowRectPick_New(data));
}





//=============================================================================
//      Q3WindowRectPick_GetRect : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WindowRectPick_GetRect(TQ3PickObject pick, TQ3Area *rect)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rect), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on rect
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WindowRectPick_GetRect(pick, rect));
}





//=============================================================================
//      Q3WindowRectPick_SetRect : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WindowRectPick_SetRect(TQ3PickObject pick, const TQ3Area *rect)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rect), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on rect
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WindowRectPick_SetRect(pick, rect));
}





//=============================================================================
//      Q3WindowRectPick_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WindowRectPick_GetData(TQ3PickObject pick, TQ3WindowRectPickData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WindowRectPick_GetData(pick, data));
}





//=============================================================================
//      Q3WindowRectPick_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WindowRectPick_SetData(TQ3PickObject pick, const TQ3WindowRectPickData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WindowRectPick_SetData(pick, data));
}


#pragma mark -


//=============================================================================
//      Q3WorldRayPick_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3PickObject
Q3WorldRayPick_New(const TQ3WorldRayPickData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on data
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WorldRayPick_New(data));
}





//=============================================================================
//      Q3WorldRayPick_GetRay : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WorldRayPick_GetRay(TQ3PickObject pick, TQ3Ray3D *ray)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ray), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on ray
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WorldRayPick_GetRay(pick, ray));
}





//=============================================================================
//      Q3WorldRayPick_SetRay : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WorldRayPick_SetRay(TQ3PickObject pick, const TQ3Ray3D *ray)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ray), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on ray
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WorldRayPick_SetRay(pick, ray));
}





//=============================================================================
//      Q3WorldRayPick_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WorldRayPick_GetData(TQ3PickObject pick, TQ3WorldRayPickData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WorldRayPick_GetData(pick, data));
}





//=============================================================================
//      Q3WorldRayPick_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WorldRayPick_SetData(TQ3PickObject pick, const TQ3WorldRayPickData *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(pick->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(pick, kQ3ObjectTypePick), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pick
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WorldRayPick_SetData(pick, data));
}



#pragma mark -

//=============================================================================
//      Q3ShapePart_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3ShapePart_GetType(TQ3ShapePartObject shapePartObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(shapePartObject->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shapePartObject, kQ3SharedTypeShapePart), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shapePartObject
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ShapePart_GetType(shapePartObject));
}





//=============================================================================
//      Q3MeshPart_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3MeshPart_GetType(TQ3MeshPartObject meshPartObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(meshPartObject->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(meshPartObject, kQ3ShapePartTypeMeshPart), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on meshPartObject
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MeshPart_GetType(meshPartObject));
}





//=============================================================================
//      Q3ShapePart_GetShape : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ShapePart_GetShape(TQ3ShapePartObject shapePartObject, TQ3ShapeObject *shapeObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(shapePartObject->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(shapePartObject, kQ3SharedTypeShapePart), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(shapeObject), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on shapePartObject
		return(kQ3Failure);

	if (0) // Further checks on shapeObject
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ShapePart_GetShape(shapePartObject, shapeObject));
}





//=============================================================================
//      Q3MeshPart_GetComponent : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MeshPart_GetComponent(TQ3MeshPartObject meshPartObject, TQ3MeshComponent *component)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(meshPartObject->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(meshPartObject, kQ3ShapePartTypeMeshPart), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(component), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on meshPartObject
		return(kQ3Failure);

	if (0) // Further checks on component
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MeshPart_GetComponent(meshPartObject, component));
}





//=============================================================================
//      Q3MeshFacePart_GetFace : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MeshFacePart_GetFace(TQ3MeshFacePartObject meshFacePartObject, TQ3MeshFace *face)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(meshFacePartObject->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(meshFacePartObject, kQ3MeshPartTypeMeshFacePart), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(face), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on meshFacePartObject
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MeshFacePart_GetFace(meshFacePartObject, face));
}





//=============================================================================
//      Q3MeshEdgePart_GetEdge : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MeshEdgePart_GetEdge(TQ3MeshEdgePartObject meshEdgePartObject, TQ3MeshEdge *edge)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(meshEdgePartObject->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(meshEdgePartObject, kQ3MeshPartTypeMeshEdgePart), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(edge), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on meshEdgePartObject
		return(kQ3Failure);

	if (0) // Further checks on edge
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MeshEdgePart_GetEdge(meshEdgePartObject, edge));
}





//=============================================================================
//      Q3MeshVertexPart_GetVertex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MeshVertexPart_GetVertex(TQ3MeshVertexPartObject meshVertexPartObject, TQ3MeshVertex *vertex)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(meshVertexPartObject->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(meshVertexPartObject, kQ3MeshPartTypeMeshVertexPart), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vertex), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on meshVertexPartObject
		return(kQ3Failure);

	if (0) // Further checks on vertex
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MeshVertexPart_GetVertex(meshVertexPartObject, vertex));
}






/*  NAME:
        E3GeometryGeneralPolygon.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3Set.h"
#include "E3Tessellate.h"
#include "E3Geometry.h"
#include "E3GeometryGeneralPolygon.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_generalpolygon_new : GeneralPolygon new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3GeneralPolygonData			*instanceData		= (TQ3GeneralPolygonData *)       privateData;
	const TQ3GeneralPolygonData		*generalPolygonData = (const TQ3GeneralPolygonData *) paramData;
	TQ3Status						qd3dStatus;
#pragma unused(theObject)



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3GeneralPolygonData));
	
	qd3dStatus = Q3GeneralPolygon_SetData(theObject, generalPolygonData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_generalpolygon_delete : GeneralPolygon delete method.
//-----------------------------------------------------------------------------
static void
e3geom_generalpolygon_delete(TQ3Object theObject, void *privateData)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) privateData;
	TQ3Status					qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3GeneralPolygon_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_generalpolygon_copydata : Copy TQ3GeneralPolygonData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_copydata( const TQ3GeneralPolygonData* src,
	TQ3GeneralPolygonData* dst, TQ3Boolean isDuplicate )
{
	TQ3Status					qd3dStatus = kQ3Success;
	TQ3Uns32					contourIndex, vertexIndex;


	// Allocate some space for the new contours
	dst->contours = (TQ3GeneralPolygonContourData *)
		Q3Memory_Allocate(src->numContours * sizeof(TQ3GeneralPolygonContourData));
	if (dst->contours == NULL)
		return (kQ3Failure);

	
	// Copy non-contour fields
	dst->numContours = src->numContours;
	dst->shapeHint = src->shapeHint;
	if (src->generalPolygonAttributeSet == NULL)
	{
		dst->generalPolygonAttributeSet = NULL;
	}
	else if (isDuplicate)
	{
		dst->generalPolygonAttributeSet = Q3Object_Duplicate( src->generalPolygonAttributeSet );
		if (dst->generalPolygonAttributeSet == NULL)
			qd3dStatus = kQ3Failure;
	}
	else
	{
		E3Shared_Acquire( &dst->generalPolygonAttributeSet, src->generalPolygonAttributeSet );
	}
	
	
	// Copy contour data
	for (contourIndex = 0; contourIndex < dst->numContours; ++contourIndex)
	{
		dst->contours[ contourIndex ].numVertices = src->contours[ contourIndex ].numVertices;
		dst->contours[ contourIndex ].vertices = (TQ3Vertex3D *) Q3Memory_Allocate(
			src->contours[ contourIndex ].numVertices * sizeof(TQ3Vertex3D));
		if (dst->contours[ contourIndex ].vertices == NULL)
		{
			qd3dStatus = kQ3Failure;
		}
		else
		{
			for (vertexIndex = 0;
				vertexIndex < dst->contours[ contourIndex ].numVertices;
				++vertexIndex)
			{
				dst->contours[ contourIndex ].vertices[vertexIndex].point =
					src->contours[ contourIndex ].vertices[vertexIndex].point;
				
				if (src->contours[ contourIndex ].vertices[vertexIndex].attributeSet == NULL)
				{
					dst->contours[ contourIndex ].vertices[vertexIndex].attributeSet = NULL;
				}
				else if (isDuplicate)
				{
					dst->contours[ contourIndex ].vertices[vertexIndex].attributeSet =
						Q3Object_Duplicate(
							src->contours[ contourIndex ].vertices[vertexIndex].attributeSet );
					
					if (dst->contours[ contourIndex ].vertices[vertexIndex].attributeSet == NULL)
						qd3dStatus = kQ3Failure;
				}
				else
				{
					E3Shared_Acquire( &dst->contours[ contourIndex ].vertices[vertexIndex].attributeSet,
						src->contours[ contourIndex ].vertices[vertexIndex].attributeSet );
				}
			}
		}
	}
	
	
	
	// Clean up after failure
	if (qd3dStatus == kQ3Failure)
	{
		E3GeneralPolygon_EmptyData( dst );
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_generalpolygon_duplicate : GeneralPolygon duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  		TQ3Object toObject,   void       *toPrivateData)
{	TQ3GeneralPolygonData		*toInstanceData = (TQ3GeneralPolygonData *) toPrivateData;
	TQ3GeneralPolygonData		*fromInstanceData = (TQ3GeneralPolygonData *) fromPrivateData;
	TQ3Status					qd3dStatus;
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = e3geom_generalpolygon_copydata( fromInstanceData, toInstanceData, kQ3True );


	return(qd3dStatus);
}





//=============================================================================
//      e3geom_generalpolygon_cache_new : GeneralPolygon cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_generalpolygon_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3GeneralPolygonData *geomData)
{	TQ3Contour				*theContours;
	TQ3GeometryObject		theTriMesh;
#pragma unused(theView)
#pragma unused(theGeom)



	// Obtain the contour data. For now we can simply cast between the two structures,
	// since they are identical - we assert this to make sure, since TQ3Contour is
	// internal at present and so may change.
	Q3_REQUIRE_OR_RESULT(sizeof(TQ3Contour) == sizeof(TQ3GeneralPolygonContourData), NULL);
	theContours = (TQ3Contour *) geomData->contours;



	// Tessellate the polygon
	theTriMesh = E3Tessellate_Contours(geomData->numContours, theContours, geomData->generalPolygonAttributeSet);	

	return(theTriMesh);
}





//=============================================================================
//      e3geom_generalpolygon_bounds : GeneralPolygon bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_generalpolygon_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3GeneralPolygonData			*instanceData = (const TQ3GeneralPolygonData *) objectData;
	TQ3Uns32							n;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds from our contours - although general polys are normally
	// defined as an outer polygon with interior holes, the contours are allowed
	// to be disjoint and so we need to accumulate each contour in turn.
	for (n = 0; n < instanceData->numContours; n++)
		E3View_UpdateBounds(theView,
							instanceData->contours[n].numVertices,
							sizeof(TQ3Vertex3D),
							&instanceData->contours[n].vertices[0].point);

	return(kQ3Success);
}





//=============================================================================
//      e3geom_generalpolygon_get_attribute : GeneralPolygon get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_generalpolygon_get_attribute(TQ3GeometryObject theObject)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->generalPolygonAttributeSet);
}





//=============================================================================
//      e3geom_generalpolygon_metahandler : GeneralPolygon metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_generalpolygon_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_generalpolygon_get_attribute;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryGeneralPolygon_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryGeneralPolygon_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeGeneralPolygon,
											kQ3ClassNameGeometryGeneralPolygon,
											e3geom_generalpolygon_metahandler,
											sizeof(TQ3GeneralPolygonData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryGeneralPolygon_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryGeneralPolygon_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeGeneralPolygon, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3GeneralPolygon_New : Create a general polygon object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3GeneralPolygon_New(const TQ3GeneralPolygonData *generalPolygonData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeGeneralPolygon, kQ3False, generalPolygonData);
	return(theObject);
}





//=============================================================================
//      E3GeneralPolygon_Submit : Submit a general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_Submit(const TQ3GeneralPolygonData *generalPolygonData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeGeneralPolygon, generalPolygonData);
	return(qd3dStatus);
}





//=============================================================================
//      E3GeneralPolygon_SetData : Set the data fora general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetData(TQ3GeometryObject generalPolygon, const TQ3GeneralPolygonData *generalPolygonData)
{	TQ3GeneralPolygonData			*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;
	TQ3Status						qd3dStatus;

	
	E3GeneralPolygon_EmptyData( instanceData );
	
	qd3dStatus = e3geom_generalpolygon_copydata( generalPolygonData, instanceData, kQ3False );


	Q3Shared_Edited(generalPolygon);

	return (qd3dStatus);
}





//=============================================================================
//      E3GeneralPolygon_GetData : Get the data for a general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetData(TQ3GeometryObject generalPolygon, TQ3GeneralPolygonData *generalPolygonData)
{	TQ3GeneralPolygonData			*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;
	TQ3Status						qd3dStatus;


	qd3dStatus = e3geom_generalpolygon_copydata( instanceData, generalPolygonData, kQ3False );

	return(qd3dStatus);
}





//=============================================================================
//      E3GeneralPolygon_EmptyData : Release the data for a general polygon.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_EmptyData(TQ3GeneralPolygonData *generalPolygonData)
{	TQ3Uns32		n, m;



	// Release the data
	for (n = 0; n < generalPolygonData->numContours; n++)
		{
		for (m = 0; m < generalPolygonData->contours[n].numVertices; m++)
			Q3Object_CleanDispose(&generalPolygonData->contours[n].vertices[m].attributeSet);

		Q3Memory_Free(&generalPolygonData->contours[n].vertices);
		}

	Q3Memory_Free(&generalPolygonData->contours);
	Q3Object_CleanDispose(&generalPolygonData->generalPolygonAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_GetVertexPosition : Get the position of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetVertexPosition(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3Point3D *position)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Get the vertex position
	*position = instanceData->contours[contourIndex].vertices[pointIndex].point;

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_SetVertexPosition : Set the position of a vertex.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetVertexPosition(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, const TQ3Point3D *position)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Set the vertex position
	instanceData->contours[contourIndex].vertices[pointIndex].point = *position;

	Q3Shared_Edited(generalPolygon);

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_GetVertexAttributeSet : Get vertex attributes.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetVertexAttributeSet(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3AttributeSet *attributeSet)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Get the vertex attribute set
	E3Shared_Acquire(attributeSet, instanceData->contours[contourIndex].vertices[pointIndex].attributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_SetVertexAttributeSet : Set vertex attributes.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetVertexAttributeSet(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3AttributeSet attributeSet)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Set the vertex attribute set
	E3Shared_Replace(&instanceData->contours[contourIndex].vertices[pointIndex].attributeSet, attributeSet);

	Q3Shared_Edited(generalPolygon);

	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_GetShapeHint : Get the polygon shape hint.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_GetShapeHint(TQ3GeometryObject generalPolygon, TQ3GeneralPolygonShapeHint *shapeHint)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Get the shape hint
	*shapeHint = instanceData->shapeHint;
	
	return(kQ3Success);
}





//=============================================================================
//      E3GeneralPolygon_SetShapeHint : Set the polygon shape hint.
//-----------------------------------------------------------------------------
TQ3Status
E3GeneralPolygon_SetShapeHint(TQ3GeometryObject generalPolygon, TQ3GeneralPolygonShapeHint shapeHint)
{	TQ3GeneralPolygonData		*instanceData = (TQ3GeneralPolygonData *) generalPolygon->instanceData;



	// Set the shape hint
	instanceData->shapeHint = shapeHint;
	
	Q3Shared_Edited(generalPolygon);

	return(kQ3Success);
}



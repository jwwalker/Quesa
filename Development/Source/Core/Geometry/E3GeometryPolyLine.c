/*  NAME:
        E3GeometryPolyLine.c

    DESCRIPTION:
        Implementation of Quesa PolyLine geometry class.

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
#include "E3Geometry.h"
#include "E3GeometryPolyLine.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_polyline_new : PolyLine new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyline_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PolyLineData				*instanceData = (TQ3PolyLineData *)       privateData;
	const TQ3PolyLineData		*polyLineData = (const TQ3PolyLineData *) paramData;
	TQ3Status					qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3PolyLineData));
	
	qd3dStatus = Q3PolyLine_SetData(theObject, polyLineData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_polyline_delete : PolyLine delete method.
//-----------------------------------------------------------------------------
static void
e3geom_polyline_delete(TQ3Object theObject, void *privateData)
{	TQ3PolyLineData		*instanceData = (TQ3PolyLineData *) privateData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3PolyLine_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_polyline_duplicate : PolyLine duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyline_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	TQ3PolyLineData		*toInstanceData = (TQ3PolyLineData *) toPrivateData;
	TQ3Status			qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3PolyLine_GetData(fromObject, toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_polyline_cache_new : PolyLine cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_polyline_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3PolyLineData *geomData)
{	TQ3GroupObject		theGroup;
	TQ3LineData			lineData;
	TQ3GeometryObject	theLine;
	TQ3Uns32			n;
#pragma unused(theView)



	// Create a group to hold the cached geometry
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Add the cached form to the group
	for (n = 0; n < (geomData->numVertices-1); n++)
		{
		// Set up the line data
		lineData.vertices[0] = geomData->vertices[n];
		lineData.vertices[1] = geomData->vertices[n + 1];
			
		if (geomData->segmentAttributeSet != NULL && geomData->segmentAttributeSet[n] != NULL)
			lineData.lineAttributeSet = geomData->segmentAttributeSet[n];
		else
			lineData.lineAttributeSet = geomData->polyLineAttributeSet;



		// Create the line
		theLine = Q3Line_New(&lineData);
		if (theLine != NULL)
			{
			Q3Group_AddObject(theGroup, theLine);
			Q3Object_Dispose(theLine);
			}
		}



	// Finish off the group state (in-line, since we don't make any view state changes)
	Q3DisplayGroup_SetState(theGroup, kQ3DisplayGroupStateMaskIsInline  |
									  kQ3DisplayGroupStateMaskIsDrawn   |
									  kQ3DisplayGroupStateMaskIsWritten |
									  kQ3DisplayGroupStateMaskIsPicked);

	return(theGroup);
}





//=============================================================================
//      e3geom_polyline_pick : PolyLine picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyline_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_polyline_bounds : PolyLine bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyline_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3PolyLineData		*instanceData = (const TQ3PolyLineData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	E3View_UpdateBounds(theView, instanceData->numVertices, sizeof(TQ3Vertex3D), &instanceData->vertices[0].point);
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_polyline_get_attribute : Polyline get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_polyline_get_attribute(TQ3GeometryObject theObject)
{	TQ3PolyLineData		*instanceData = (TQ3PolyLineData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->polyLineAttributeSet);
}





//=============================================================================
//      e3geom_polyline_metahandler : PolyLine metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_polyline_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_polyline_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_polyline_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_polyline_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_polyline_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_polyline_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_polyline_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_polyline_get_attribute;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryPolyLine_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryPolyLine_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypePolyLine,
											kQ3ClassNameGeometryPolyLine,
											e3geom_polyline_metahandler,
											sizeof(TQ3PolyLineData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryPolyLine_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryPolyLine_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypePolyLine, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3PolyLine_New : Create a polyLine object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3PolyLine_New(const TQ3PolyLineData *polyLineData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypePolyLine, kQ3False, polyLineData);
	return(theObject);
}





//=============================================================================
//      E3PolyLine_Submit : Submit a polyline.
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_Submit(const TQ3PolyLineData *polyLineData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypePolyLine, polyLineData);
	return(qd3dStatus);
}





//=============================================================================
//      E3PolyLine_SetData : Set the polyline data.
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_SetData(TQ3GeometryObject polyLine, const TQ3PolyLineData *polyLineData)
{
	TQ3PolyLineData *		instanceData = (TQ3PolyLineData *) polyLine->instanceData;
	TQ3Uns32				n, newSize;
	
	//deallocate the old vertex attribute memory
	for(n = 0; n < instanceData->numVertices; n++)
		Q3Object_Dispose(instanceData->vertices[n].attributeSet) ;


	//deallocate the old segment attributes if they exist
	if( instanceData->segmentAttributeSet != NULL )
		for(n = 0 ; instanceData->numVertices - 1; n++)
			Q3Object_Dispose( instanceData->segmentAttributeSet[n] ) ;



	//reallocate memory for vertices
	newSize = polyLineData->numVertices * sizeof(TQ3Vertex3D);
	Q3Memory_Reallocate(&instanceData->vertices, newSize);

	if (instanceData->vertices == NULL)
		return(kQ3Failure);

	Q3Memory_Clear(instanceData->vertices, newSize);



	//set vertex data
	instanceData->numVertices = polyLineData->numVertices ;

	for(n = 0; n < polyLineData->numVertices; n++)
	{
		instanceData->vertices[n].point = polyLineData->vertices[n].point ;
		
		//set vertex attributes
		E3Shared_Replace(	&instanceData->vertices[n].attributeSet,
							polyLineData->vertices[n].attributeSet ) ;
	}
	
	//set the segment attribute sets if they exits
	if( polyLineData->segmentAttributeSet != NULL )
	{
		//reallocate memory for segment attributes
		newSize = (polyLineData->numVertices - 1) * sizeof(TQ3AttributeSet);
		Q3Memory_Reallocate(&instanceData->segmentAttributeSet, newSize);

		if (instanceData->segmentAttributeSet != NULL)
			{
			Q3Memory_Clear(instanceData->segmentAttributeSet, newSize);
			
			for(n = 0; n < polyLineData->numVertices - 1; n++)
				E3Shared_Replace(	&instanceData->segmentAttributeSet[n],
									polyLineData->segmentAttributeSet[n] ) ;
			}
	}
	else
	{
		//otherwise free the memory for the old segment attribute set
		Q3Memory_Free(&instanceData->segmentAttributeSet) ;
	}
	
	//copy attributes
	E3Shared_Replace(&instanceData->polyLineAttributeSet, polyLineData->polyLineAttributeSet);

	Q3Shared_Edited(polyLine);
	
	return(kQ3Success);
}





//=============================================================================
//      E3PolyLine_GetData : Get the data describing a polyline
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_GetData(TQ3GeometryObject polyLine, TQ3PolyLineData *polyLineData)
{
	const TQ3PolyLineData *		instanceData = (const TQ3PolyLineData *) polyLine->instanceData;
	TQ3Uns32					n ;
	
	//get numVertices
	polyLineData->numVertices = instanceData->numVertices ;
	
	//allocate memory for vertices
	polyLineData->vertices =
		(TQ3Vertex3D *) Q3Memory_Allocate( instanceData->numVertices * sizeof( TQ3Vertex3D ) ) ;
		
	//handle vertex allocation failure
	if( polyLineData->vertices == NULL )
		return(kQ3Failure) ;
	
	//allocate memory for segmentAttributeSet if it exists
	if( instanceData->segmentAttributeSet != NULL )
	{
		polyLineData->segmentAttributeSet = 
			(TQ3AttributeSet *) Q3Memory_Allocate( (instanceData->numVertices - 1) 
													* sizeof(TQ3AttributeSet) ) ;
			
		//handle segment allocation failure
		if( polyLineData->segmentAttributeSet == NULL )
		{
			Q3Memory_Free( &polyLineData->vertices ) ;
			return(kQ3Failure) ;
		}
	}
	else
	{
		polyLineData->segmentAttributeSet = NULL ;
	}
	
	//copy vertex data
	for(n = 0; n < instanceData->numVertices; n++)
	{
		polyLineData->vertices[n].point = instanceData->vertices[n].point ;
		
		E3Shared_Acquire(	&polyLineData->vertices[n].attributeSet,
							instanceData->vertices[n].attributeSet ) ;		
		
	}
	
	//copy the segment attributes, if they exist
	if( instanceData->segmentAttributeSet != NULL )
	{
		for(n = 0; n < instanceData->numVertices - 1; n++)
		{
			E3Shared_Acquire(	&polyLineData->segmentAttributeSet[n],
								instanceData->segmentAttributeSet[n] ) ;
		}
	}
	
	//get attributes
	E3Shared_Acquire(&polyLineData->polyLineAttributeSet, instanceData->polyLineAttributeSet);
	
	return(kQ3Success);
}





//=============================================================================
//      E3PolyLine_EmptyData :	Empty the data for a line object
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_EmptyData(TQ3PolyLineData *polyLineData)
{
	TQ3Uns32				n ;
	
	//delete memory for the vertex attribute sets
	for(n = 0; n < polyLineData->numVertices; n++)
	{
		E3Object_DisposeAndForget( polyLineData->vertices[n].attributeSet ) ;
	}
	
	//delete memory for the vertex array
	Q3Memory_Free( &polyLineData->vertices ) ;
	
	//if there is a segment attribute set, delete the individual items
	if( polyLineData->segmentAttributeSet != NULL )
	{
		for(n = 0; n < polyLineData->numVertices - 1; n++)
		{
			E3Object_DisposeAndForget( polyLineData->segmentAttributeSet[n] ) ;
		}
	
		//then delete it
		Q3Memory_Free(&polyLineData->segmentAttributeSet);
	}

	E3Object_DisposeAndForget(polyLineData->polyLineAttributeSet);			

	return(kQ3Success);
}





//=============================================================================
//      E3PolyLine_GetVertexPosition :	Get the position of polyline vertex
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_GetVertexPosition(	TQ3GeometryObject polyLine, TQ3Uns32 index, 
								TQ3Point3D *position)
{
	const TQ3PolyLineData *		instanceData = (const TQ3PolyLineData *) polyLine->instanceData;
	
	*position = instanceData->vertices[index].point ;
	
	return(kQ3Success);
}





//=============================================================================
//      E3PolyLine_SetVertexPosition :	Set the position of polyline vertex
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_SetVertexPosition(	TQ3GeometryObject polyLine, TQ3Uns32 index, 
								const TQ3Point3D *position)
{
	TQ3PolyLineData *		instanceData = (TQ3PolyLineData *) polyLine->instanceData;
	
	instanceData->vertices[index].point = *position ;

	Q3Shared_Edited(polyLine);
	
	return(kQ3Success);
}





//=============================================================================
//      E3PolyLine_GetVertexAttributeSet :	Get polyline vertex attribute set
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_GetVertexAttributeSet(	TQ3GeometryObject polyLine, TQ3Uns32 index, 
									TQ3AttributeSet *attributeSet)
{
	const TQ3PolyLineData *		instanceData = (const TQ3PolyLineData *) polyLine->instanceData;
		
	//return the attribute set
	E3Shared_Acquire(attributeSet, instanceData->vertices[index].attributeSet) ;
	
	return(kQ3Success);
}





//=============================================================================
//      E3PolyLine_SetVertexAttributeSet :	Set polyline vertex attriubte set
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_SetVertexAttributeSet(	TQ3GeometryObject polyLine, TQ3Uns32 index, 
									TQ3AttributeSet attributeSet)
{
	TQ3PolyLineData *		instanceData = (TQ3PolyLineData *) polyLine->instanceData;
	
	E3Shared_Replace(&instanceData->vertices[index].attributeSet, attributeSet) ;

	Q3Shared_Edited(polyLine);
	
	return(kQ3Success);
}





//=============================================================================
//      E3PolyLine_GetSegmentAttributeSet :	Get polyline segement attribute set
//-----------------------------------------------------------------------------
//		Note :	The QuickView refs describe index as being as if it was for
//				a vertex index, rather than a segment - is this correct?
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_GetSegmentAttributeSet(TQ3GeometryObject polyLine, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
{	TQ3PolyLineData		*instanceData = (TQ3PolyLineData *) polyLine->instanceData;



	// Get the segment attribute set
	E3Shared_Acquire(attributeSet, instanceData->segmentAttributeSet[index]);
	
	return(kQ3Success);
}





//=============================================================================
//      E3PolyLine_SetSegmentAttributeSet : Set polyline segment attribute set.
//-----------------------------------------------------------------------------
//		Note :	The QuickView refs describe index as being as if it was for
//				a vertex index, rather than a segment - is this correct?
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_SetSegmentAttributeSet(TQ3GeometryObject polyLine, TQ3Uns32 index, TQ3AttributeSet attributeSet)
{	TQ3PolyLineData		*instanceData = (TQ3PolyLineData *) polyLine->instanceData;



	// Set the segment attribute set
	E3Shared_Replace(&instanceData->segmentAttributeSet[index], attributeSet);

	Q3Shared_Edited(polyLine);
	
	return(kQ3Success);
}



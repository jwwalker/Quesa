/*  NAME:
        E3GeometryPolyLine.c

    DESCRIPTION:
        Implementation of Quesa PolyLine geometry class.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
//      e3geom_polyline_copydata : Copy TQ3PolyLineData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyline_copydata( const TQ3PolyLineData* src, TQ3PolyLineData* dst,
	TQ3Boolean isDuplicate )
{
	TQ3Status	q3Status = kQ3Success;
	TQ3Uns32	n;
	
	
	dst->numVertices = src->numVertices;
	
	// allocate memory for vertices
	dst->vertices =
		(TQ3Vertex3D *) Q3Memory_Allocate( src->numVertices * sizeof( TQ3Vertex3D ) ) ;
		
	// handle vertex allocation failure
	if ( dst->vertices == NULL )
		return (kQ3Failure);
	
	// allocate memory for segment attributes, if present
	if (src->segmentAttributeSet != NULL)
	{
		dst->segmentAttributeSet = 
			(TQ3AttributeSet *) Q3Memory_Allocate( (dst->numVertices - 1) 
													* sizeof(TQ3AttributeSet) ) ;
			
		//handle segment allocation failure
		if( dst->segmentAttributeSet == NULL )
		{
			Q3Memory_Free( &dst->vertices ) ;
			return(kQ3Failure) ;
		}
	}
	else
	{
		dst->segmentAttributeSet = NULL;
	}
	
	
	// Copy vertex data
	for (n = 0; n < dst->numVertices; ++n)
	{
		dst->vertices[n].point = src->vertices[n].point;
		
		if (src->vertices[n].attributeSet == NULL)
		{
			dst->vertices[n].attributeSet = NULL;
		}
		else if (isDuplicate)
		{
			dst->vertices[n].attributeSet = Q3Object_Duplicate( src->vertices[n].attributeSet );
			if (dst->vertices[n].attributeSet == NULL)
				q3Status = kQ3Failure;
		}
		else
		{
			E3Shared_Acquire( &dst->vertices[n].attributeSet, src->vertices[n].attributeSet );
		}
	}
	
	
	// Copy segment attributes, if present
	if (src->segmentAttributeSet != NULL)
	{
		for (n = 0; n < dst->numVertices - 1; ++n)
		{
			if (src->segmentAttributeSet[n] == NULL)
			{
				dst->segmentAttributeSet[n] = NULL;
			}
			else if (isDuplicate)
			{
				dst->segmentAttributeSet[n] = Q3Object_Duplicate( src->segmentAttributeSet[n] );
				if (dst->segmentAttributeSet[n] == NULL)
					q3Status = kQ3Failure;
			}
			else
			{
				E3Shared_Acquire( &dst->segmentAttributeSet[n], src->segmentAttributeSet[n] );
			}
		}
	}
	
	
	// Copy overall attributes
	if (src->polyLineAttributeSet == NULL)
	{
		dst->polyLineAttributeSet = NULL;
	}
	else if (isDuplicate)
	{
		dst->polyLineAttributeSet = Q3Object_Duplicate( src->polyLineAttributeSet );
		if (dst->polyLineAttributeSet == NULL)
			q3Status = kQ3Failure;
	}
	else
	{
		E3Shared_Acquire( &dst->polyLineAttributeSet, src->polyLineAttributeSet );
	}
	
	
	
	// Clean up after failure
	if (q3Status == kQ3Failure)
	{
		E3PolyLine_EmptyData( dst );
	}
	
	
	return q3Status;
}





//=============================================================================
//      e3geom_polyline_duplicate : PolyLine duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_polyline_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	TQ3PolyLineData		*toInstanceData = (TQ3PolyLineData *) toPrivateData;
	TQ3PolyLineData		*fromInstanceData = (TQ3PolyLineData *) fromPrivateData;
	TQ3Status			qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = e3geom_polyline_copydata( fromInstanceData, toInstanceData, kQ3True );

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
{	TQ3PolyLineData		*instanceData = (TQ3PolyLineData *) E3ClassTree_FindInstanceData(theObject, kQ3GeometryTypePolyLine);



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
	TQ3PolyLineData *		instanceData = (TQ3PolyLineData *) E3ClassTree_FindInstanceData(polyLine, kQ3GeometryTypePolyLine);
	TQ3Status				q3status;
	
	
	
	E3PolyLine_EmptyData( instanceData );

	q3status = e3geom_polyline_copydata( polyLineData, instanceData, kQ3False );


	Q3Shared_Edited(polyLine);
	
	return (q3status);
}





//=============================================================================
//      E3PolyLine_GetData : Get the data describing a polyline
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_GetData(TQ3GeometryObject polyLine, TQ3PolyLineData *polyLineData)
{
	const TQ3PolyLineData *		instanceData = (const TQ3PolyLineData *) E3ClassTree_FindInstanceData(polyLine, kQ3GeometryTypePolyLine);
	TQ3Status	q3status;
	
	
	
	q3status = e3geom_polyline_copydata( instanceData, polyLineData, kQ3False );
	
	return q3status;
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
		Q3Object_CleanDispose(&polyLineData->vertices[n].attributeSet ) ;
	}
	
	//delete memory for the vertex array
	Q3Memory_Free( &polyLineData->vertices ) ;
	
	//if there is a segment attribute set, delete the individual items
	if( polyLineData->segmentAttributeSet != NULL )
	{
		for(n = 0; n < polyLineData->numVertices - 1; n++)
		{
			Q3Object_CleanDispose(&polyLineData->segmentAttributeSet[n] ) ;
		}
	
		//then delete it
		Q3Memory_Free(&polyLineData->segmentAttributeSet);
	}

	Q3Object_CleanDispose(&polyLineData->polyLineAttributeSet);			

	return(kQ3Success);
}





//=============================================================================
//      E3PolyLine_GetVertexPosition :	Get the position of polyline vertex
//-----------------------------------------------------------------------------
TQ3Status
E3PolyLine_GetVertexPosition(	TQ3GeometryObject polyLine, TQ3Uns32 index, 
								TQ3Point3D *position)
{
	const TQ3PolyLineData *		instanceData = (const TQ3PolyLineData *) E3ClassTree_FindInstanceData(polyLine, kQ3GeometryTypePolyLine);
	
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
	TQ3PolyLineData *		instanceData = (TQ3PolyLineData *) E3ClassTree_FindInstanceData(polyLine, kQ3GeometryTypePolyLine);
	
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
	const TQ3PolyLineData *		instanceData = (const TQ3PolyLineData *) E3ClassTree_FindInstanceData(polyLine, kQ3GeometryTypePolyLine);
		
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
	TQ3PolyLineData *		instanceData = (TQ3PolyLineData *) E3ClassTree_FindInstanceData(polyLine, kQ3GeometryTypePolyLine);
	
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
{	TQ3PolyLineData		*instanceData = (TQ3PolyLineData *) E3ClassTree_FindInstanceData(polyLine, kQ3GeometryTypePolyLine);



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
{	TQ3PolyLineData		*instanceData = (TQ3PolyLineData *) E3ClassTree_FindInstanceData(polyLine, kQ3GeometryTypePolyLine);



	// Set the segment attribute set
	E3Shared_Replace(&instanceData->segmentAttributeSet[index], attributeSet);

	Q3Shared_Edited(polyLine);
	
	return(kQ3Success);
}



/*  NAME:
        E3GeometryLine.c

    DESCRIPTION:
        Implementation of Quesa Line geometry class.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#include "E3GeometryLine.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_line_new : Line new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_line_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3LineData			*instanceData	= (TQ3LineData *)		privateData;
	const TQ3LineData 	*lineData 		= (const TQ3LineData *)	paramData;
	TQ3Status			qd3dStatus;



	// Initialise our instance data
	E3Memory_Clear(instanceData, sizeof(TQ3LineData));
	
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
//      e3geom_line_pick : Line picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_line_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
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
E3Line_GetVertexPosition(TQ3GeometryObject theLine, unsigned long index, TQ3Point3D *position)
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
E3Line_SetVertexPosition(TQ3GeometryObject theLine, unsigned long index, const TQ3Point3D *position)
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
E3Line_GetVertexAttributeSet(TQ3GeometryObject theLine, unsigned long index, TQ3AttributeSet *attributeSet)
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
E3Line_SetVertexAttributeSet(TQ3GeometryObject theLine, unsigned long index, TQ3AttributeSet attributeSet)
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



/*  NAME:
        E3GeometryPixmapMarker.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3GeometryPixmapMarker.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_pixmapmarker_new : PixmapMarker new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_pixmapmarker_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PixmapMarkerData			*instanceData  = (TQ3PixmapMarkerData *)       privateData;
	const TQ3PixmapMarkerData	*pixmapMarkerData  = (const TQ3PixmapMarkerData *) paramData;
	TQ3Status					qd3dStatus;



	// Initialise our instance data
	E3Memory_Clear(instanceData, sizeof(TQ3PixmapMarkerData));
	
	qd3dStatus = Q3PixmapMarker_SetData(theObject, pixmapMarkerData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_pixmapmarker_delete : PixmapMarker delete method.
//-----------------------------------------------------------------------------
static void
e3geom_pixmapmarker_delete(TQ3Object theObject, void *privateData)
{	TQ3PixmapMarkerData		*instanceData = (TQ3PixmapMarkerData *) privateData;
	TQ3Status				qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3PixmapMarker_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_pixmapmarker_duplicate : PixmapMarker duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_pixmapmarker_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						      TQ3Object toObject,   void *toPrivateData)
{	TQ3PixmapMarkerData		*toInstanceData = (TQ3PixmapMarkerData *) toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3PixmapMarker_GetData(fromObject, toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_pixmapmarker_pick : PixmapMarker picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_pixmapmarker_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_pixmapmarker_bounds : PixmapMarker bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_pixmapmarker_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3PixmapMarkerData			*instanceData = (const TQ3PixmapMarkerData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	E3View_UpdateBounds(theView, 1, 0, &instanceData->position);
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_pixmapmarker_get_attribute : Pixmap marker attribute pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_pixmapmarker_get_attribute(TQ3GeometryObject theObject)
{	TQ3PixmapMarkerData		*instanceData = (TQ3PixmapMarkerData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->pixmapMarkerAttributeSet);
}





//=============================================================================
//      e3geom_pixmapmarker_metahandler : PixmapMarker metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_pixmapmarker_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_pixmapmarker_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_pixmapmarker_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_pixmapmarker_duplicate;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_pixmapmarker_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_pixmapmarker_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_pixmapmarker_get_attribute;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryPixmapMarker_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryPixmapMarker_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypePixmapMarker,
											kQ3ClassNameGeometryPixmapMarker,
											e3geom_pixmapmarker_metahandler,
											sizeof(TQ3PixmapMarkerData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryPixmapMarker_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryPixmapMarker_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypePixmapMarker, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3PixmapMarker_New : Create a Pixmap marker object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3PixmapMarker_New(const TQ3PixmapMarkerData *pixmapMarkerData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypePixmapMarker, kQ3False, pixmapMarkerData);
	return(theObject);
}





//=============================================================================
//      E3PixmapMarker_Submit : Submit a Pixmap marker.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_Submit(const TQ3PixmapMarkerData *pixmapMarkerData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypePixmapMarker, pixmapMarkerData);
	return(qd3dStatus);
}





//=============================================================================
//      E3PixmapMarker_SetData : Set the data for a pixmap marker.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_SetData(TQ3GeometryObject theGeom, const TQ3PixmapMarkerData *pixmapMarkerData)
{	TQ3PixmapMarkerData		*instanceData = (TQ3PixmapMarkerData *) theGeom->instanceData;



	// Set the data
	instanceData->position         = pixmapMarkerData->position;
	instanceData->xOffset          = pixmapMarkerData->xOffset;
	instanceData->yOffset          = pixmapMarkerData->yOffset;
	instanceData->pixmap.width     = pixmapMarkerData->pixmap.width;
	instanceData->pixmap.height    = pixmapMarkerData->pixmap.height;
	instanceData->pixmap.rowBytes  = pixmapMarkerData->pixmap.rowBytes;
	instanceData->pixmap.pixelSize = pixmapMarkerData->pixmap.pixelSize;
	instanceData->pixmap.pixelType = pixmapMarkerData->pixmap.pixelType;
	instanceData->pixmap.bitOrder  = pixmapMarkerData->pixmap.bitOrder;
	instanceData->pixmap.byteOrder = pixmapMarkerData->pixmap.byteOrder;
	
	E3Shared_Replace(&instanceData->pixmap.image, pixmapMarkerData->pixmap.image);
	E3Shared_Replace(&instanceData->pixmapMarkerAttributeSet, pixmapMarkerData->pixmapMarkerAttributeSet);

	Q3Shared_Edited(theGeom);

	return(kQ3Success);
}





//=============================================================================
//      E3PixmapMarker_GetData : Get the data for a pixmap marker.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_GetData(TQ3GeometryObject theGeom, TQ3PixmapMarkerData *pixmapMarkerData)
{	TQ3PixmapMarkerData		*instanceData = (TQ3PixmapMarkerData *) theGeom->instanceData;



	// Get the data
	pixmapMarkerData->position         = instanceData->position;
	pixmapMarkerData->xOffset          = instanceData->xOffset;
	pixmapMarkerData->yOffset          = instanceData->yOffset;
	pixmapMarkerData->pixmap.width     = instanceData->pixmap.width;
	pixmapMarkerData->pixmap.height    = instanceData->pixmap.height;
	pixmapMarkerData->pixmap.rowBytes  = instanceData->pixmap.rowBytes;
	pixmapMarkerData->pixmap.pixelSize = instanceData->pixmap.pixelSize;
	pixmapMarkerData->pixmap.pixelType = instanceData->pixmap.pixelType;
	pixmapMarkerData->pixmap.bitOrder  = instanceData->pixmap.bitOrder;
	pixmapMarkerData->pixmap.byteOrder = instanceData->pixmap.byteOrder;
	
	E3Shared_Acquire(&pixmapMarkerData->pixmap.image,             instanceData->pixmap.image);
	E3Shared_Acquire(&pixmapMarkerData->pixmapMarkerAttributeSet, instanceData->pixmapMarkerAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3PixmapMarker_EmptyData : Empty the data for a pixmap marker.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_EmptyData(TQ3PixmapMarkerData *pixmapMarkerData)
{


	// Release the data
	E3Object_DisposeAndForget(pixmapMarkerData->pixmap.image);
	E3Object_DisposeAndForget(pixmapMarkerData->pixmapMarkerAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3PixmapMarker_GetPosition : Get the position of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_GetPosition(TQ3GeometryObject pixmapMarker, TQ3Point3D *position)
{
	const TQ3PixmapMarkerData *	instanceData = (const TQ3PixmapMarkerData *) pixmapMarker->instanceData;
	
	
	//get the position
	*position = instanceData->position ;
	
	return(kQ3Success);
}





//=============================================================================
//      E3PixmapMarker_SetPosition : Set the position of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_SetPosition(TQ3GeometryObject pixmapMarker, const TQ3Point3D *position)
{
	TQ3PixmapMarkerData *		instanceData = (TQ3PixmapMarkerData *) pixmapMarker->instanceData;
	
	//set the position
	instanceData->position = *position ;	

	Q3Shared_Edited(pixmapMarker);

	return(kQ3Success);
}





//=============================================================================
//      E3PixmapMarker_GetXOffset : Get horizontal position of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_GetXOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 *xOffset)
{
	const TQ3PixmapMarkerData *	instanceData = (const TQ3PixmapMarkerData *) pixmapMarker->instanceData;
	
	
	//get the horizontal offset
	*xOffset = instanceData->xOffset ;
	
	return(kQ3Success);
}





//=============================================================================
//      E3PixmapMarker_SetXOffset : Set horizontal position of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_SetXOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 xOffset)
{
	TQ3PixmapMarkerData *		instanceData = (TQ3PixmapMarkerData *) pixmapMarker->instanceData;
	
	
	//set the horizontal offset
	instanceData->xOffset = xOffset ;

	Q3Shared_Edited(pixmapMarker);
	
	return(kQ3Success);
}





//=============================================================================
//      E3PixmapMarker_GetYOffset :	Get the vertical position of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_GetYOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 *yOffset)
{
	const TQ3PixmapMarkerData *	instanceData = (const TQ3PixmapMarkerData *) pixmapMarker->instanceData;
	
	//get the vertical offset
	*yOffset = instanceData->yOffset ;
	
	return(kQ3Success);
}





//=============================================================================
//      E3PixmapMarker_SetYOffset :	Set the vertical position of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_SetYOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 yOffset)
{
	TQ3PixmapMarkerData *		instanceData = (TQ3PixmapMarkerData *) pixmapMarker->instanceData;
	
	//set the vertical offset
	instanceData->yOffset = yOffset ;

	Q3Shared_Edited(pixmapMarker);
	
	return(kQ3Success);
}





//=============================================================================
//      E3PixmapMarker_GetPixmap :	Get the Pixmap of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_GetPixmap(TQ3GeometryObject pixmapMarker, TQ3StoragePixmap *pixmap)
{	TQ3PixmapMarkerData		*instanceData = (TQ3PixmapMarkerData *) pixmapMarker->instanceData;



	// Get the pixmap
	*pixmap = instanceData->pixmap;
	
	E3Shared_Acquire(&pixmap->image, instanceData->pixmap.image);

	return(kQ3Success);
}





//=============================================================================
//      E3PixmapMarker_SetPixmap :	Set the Pixmap of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_SetPixmap(TQ3GeometryObject pixmapMarker, const TQ3StoragePixmap *pixmap)
{	TQ3PixmapMarkerData		*instanceData = (TQ3PixmapMarkerData *) pixmapMarker->instanceData;



	// Set the pixmap
	E3Object_DisposeAndForget(instanceData->pixmap.image);

	instanceData->pixmap = *pixmap;

	E3Shared_Replace(&instanceData->pixmap.image, pixmap->image);

	Q3Shared_Edited(pixmapMarker);

	return(kQ3Success);
}

/*  NAME:
        E3GeometryMarker.c

    DESCRIPTION:
        Implementation of Quesa Marker geometry class.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#include "E3Utils.h"
#include "E3Geometry.h"
#include "E3GeometryMarker.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_marker_pixel_is_set : Is a pixel within a marker set?
//-----------------------------------------------------------------------------
static TQ3Boolean
e3geom_marker_pixel_is_set(const TQ3MarkerData *instanceData, TQ3Int32 x, TQ3Int32 y)
{	TQ3Uns32		bitOffset, byteOffset;
	TQ3Uns8			*thePtr, theByte;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(instanceData), kQ3False);



	// Check to see if the pixel is in range
	if (x < 0 || x >= (TQ3Int32) instanceData->bitmap.width ||
		y < 0 || y >= (TQ3Int32) instanceData->bitmap.height)
		return(kQ3False);



	// Find the right row within the image
	thePtr = instanceData->bitmap.image + (y * instanceData->bitmap.rowBytes);



	// Find the right byte within the row
	byteOffset = (x / 8);
	thePtr    += byteOffset;
	theByte    = *thePtr;



	// Find the right bit within the byte
	bitOffset = 7 - (x - byteOffset);



	// Test the pixel
	return(E3Bit_IsSet(theByte, 1 << bitOffset));
}





//=============================================================================
//      e3geom_marker_new : Marker new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_marker_new(TQ3Object theObject, void *privateData, const void *paramData)
{	
	TQ3MarkerData			*instanceData = (TQ3MarkerData *)		privateData;
	const TQ3MarkerData		*markerData   = (const TQ3MarkerData *)	paramData;
	TQ3Status				qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3MarkerData));
	
	qd3dStatus = Q3Marker_SetData(theObject, markerData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_marker_delete : Marker delete method.
//-----------------------------------------------------------------------------
static void
e3geom_marker_delete(TQ3Object theObject, void *privateData)
{	
	TQ3MarkerData		*instanceData = (TQ3MarkerData *) privateData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3Marker_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_marker_duplicate : Marker duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_marker_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						TQ3Object toObject,   void *toPrivateData)
{	
	TQ3MarkerData		*toInstanceData	= (TQ3MarkerData *) toPrivateData;
	TQ3Status			qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Marker_GetData(fromObject, toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_marker_pick_window_point : Marker window-point picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_marker_pick_window_point(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3MarkerData			*instanceData = (const TQ3MarkerData *) objectData;
	TQ3Status					qd3dStatus    = kQ3Success;
	TQ3Point2D					windowPoint, markerPixel;
	TQ3WindowPointPickData		pickData;



	// Get the pick data
	Q3WindowPointPick_GetData(thePick, &pickData);



	// Transform our point to the screen, and adjust it by our offset
	// to get the location of the top-left pixel within the image.
	Q3View_TransformLocalToWindow(theView, &instanceData->location, &windowPoint);
	windowPoint.x += instanceData->xOffset;
	windowPoint.y += instanceData->yOffset;



	// Calculate where in our image the pick point will fall
	markerPixel.x = pickData.point.x - windowPoint.x;
	markerPixel.y = pickData.point.y - windowPoint.y;



	// See if we fall within the pick
	if (e3geom_marker_pixel_is_set(instanceData, (TQ3Int32) markerPixel.x, (TQ3Int32) markerPixel.y))
		qd3dStatus = E3Pick_RecordHit(thePick, theView, NULL, NULL, NULL, NULL);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_marker_pick_window_rect : Marker window-rect picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_marker_pick_window_rect(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3MarkerData			*instanceData = (const TQ3MarkerData *) objectData;
	TQ3Status					qd3dStatus    = kQ3Success;
	TQ3Area						markerRect;
	TQ3WindowRectPickData		pickData;
	TQ3Uns32					x, y;



	// Get the pick data
	Q3WindowRectPick_GetData(thePick, &pickData);



	// Transform our point to the screen, adjust it by our offset to get the
	// location of the top-left pixel within the image, and add our width/height
	// to get the location of the botom-right pixel.
	Q3View_TransformLocalToWindow(theView, &instanceData->location, &markerRect.min);

	markerRect.min.x += instanceData->xOffset;
	markerRect.min.y += instanceData->yOffset;

	markerRect.max.x = markerRect.min.x + instanceData->bitmap.width;
	markerRect.max.y = markerRect.min.y + instanceData->bitmap.height;



	// See if we fall within the pick
	if (E3Rect_IntersectRect(&markerRect, &pickData.rect))
		{
		// Look for an active pixel within the pick rect
		for (y = 0; y < instanceData->bitmap.height; y++)
			{
			for (x = 0; x < instanceData->bitmap.width; x++)
				{
				if (e3geom_marker_pixel_is_set(instanceData, x, y))
					{
					qd3dStatus = E3Pick_RecordHit(thePick, theView, NULL, NULL, NULL, NULL);
					return(qd3dStatus);
					}
				}
			}
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_marker_pick : Marker picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_marker_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3PickObject		thePick;



	// Handle the pick
	thePick = E3View_AccessPick(theView);
	switch (Q3Pick_GetType(thePick)) {
		case kQ3PickTypeWindowPoint:
			qd3dStatus = e3geom_marker_pick_window_point(theView, thePick, theObject, objectData);
			break;

		case kQ3PickTypeWindowRect:
			qd3dStatus = e3geom_marker_pick_window_rect(theView, thePick, theObject, objectData);
			break;

		case kQ3PickTypeWorldRay:
			// Can't be picked, but don't stop picking
			qd3dStatus = kQ3Success;
			break;

		default:
			qd3dStatus = kQ3Failure;
			break;
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_marker_bounds : Marker bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_marker_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3MarkerData		*instanceData = (const TQ3MarkerData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	E3View_UpdateBounds(theView, 1, 0, &instanceData->location);
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_marker_get_attribute : Marker get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_marker_get_attribute(TQ3GeometryObject theObject)
{	TQ3MarkerData		*instanceData = (TQ3MarkerData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->markerAttributeSet);
}





//=============================================================================
//      e3geom_marker_metahandler : Marker metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_marker_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_marker_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_marker_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_marker_duplicate;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_marker_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_marker_bounds;
			break;

		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_marker_get_attribute;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryMarker_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryMarker_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeMarker,
											kQ3ClassNameGeometryMarker,
											e3geom_marker_metahandler,
											sizeof(TQ3MarkerData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryMarker_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryMarker_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeMarker, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Marker_New : Create a marker object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Marker_New(const TQ3MarkerData *markerData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeMarker, kQ3False, markerData);
	return(theObject);
}





//=============================================================================
//      E3Marker_Submit : Submit a marker.
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_Submit(const TQ3MarkerData *markerData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeMarker, markerData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Marker_SetData : Set the data associated with a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_SetData(TQ3GeometryObject theMarker, const TQ3MarkerData *markerData)
{
	TQ3MarkerData *		instanceData = (TQ3MarkerData *) theMarker->instanceData;

	//set the data
	instanceData->location	= markerData->location ;
	instanceData->xOffset	= markerData->xOffset ;
	instanceData->yOffset	= markerData->yOffset ;
	
	// copy the bitmap
	E3Bitmap_Replace( &markerData->bitmap, &instanceData->bitmap, kQ3True ) ;
	
	//copy the attribute set
	E3Shared_Replace(&instanceData->markerAttributeSet, markerData->markerAttributeSet);

	Q3Shared_Edited(theMarker);

	return(kQ3Success);
}





//=============================================================================
//      E3Marker_GetData : Get the data associated with a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_GetData(TQ3GeometryObject theMarker, TQ3MarkerData *markerData)
{
	const TQ3MarkerData *	instanceData = (const TQ3MarkerData *) theMarker->instanceData;
	
	markerData->location	= instanceData->location ;
	markerData->xOffset		= instanceData->xOffset ;
	markerData->yOffset		= instanceData->yOffset ;
	
	//copy the attributes
	markerData->markerAttributeSet = NULL ;
	
	E3Geometry_GetAttributeSet( theMarker, &markerData->markerAttributeSet ) ;
	
	//copy the bitmap
	E3Bitmap_Replace( &instanceData->bitmap, &markerData->bitmap, kQ3False ) ;
	
	return(kQ3Success);
}





//=============================================================================
//      E3Marker_EmptyData : Free the memory allocated by a call to 
//							 Q3Marker_GetData
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_EmptyData(TQ3MarkerData *markerData)
{

	//empty the bitmap
	Q3Bitmap_Empty( &markerData->bitmap ) ;
	
	//empty the attributes
	Q3Object_CleanDispose(&markerData->markerAttributeSet ) ;

	return(kQ3Success);
}





//=============================================================================
//      E3Marker_GetPosition : Get the position of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_GetPosition(TQ3GeometryObject theMarker, TQ3Point3D *location)
{
	const TQ3MarkerData *	instanceData = (const TQ3MarkerData *) theMarker->instanceData;
	
	
	//get the location
	*location = instanceData->location ;

	return(kQ3Success);
}





//=============================================================================
//      E3Marker_SetPosition : Set the position of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_SetPosition(TQ3GeometryObject theMarker, const TQ3Point3D *location)
{
	TQ3MarkerData *		instanceData = (TQ3MarkerData *) theMarker->instanceData;
	
	
	//set the location
	instanceData->location = *location ;

	Q3Shared_Edited(theMarker);

	return(kQ3Success);
}





//=============================================================================
//      E3Marker_GetXOffset : Get horizontal offset of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_GetXOffset(TQ3GeometryObject theMarker, TQ3Int32 *xOffset)
{
	const TQ3MarkerData *	instanceData = (const TQ3MarkerData *) theMarker->instanceData;


	//get the horizontal offset
	*xOffset = instanceData->xOffset ;
	
	return(kQ3Success);
}





//=============================================================================
//      E3Marker_SetXOffset : Set horizontal offset of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_SetXOffset(TQ3GeometryObject theMarker, TQ3Int32 xOffset)
{
	TQ3MarkerData *		instanceData = (TQ3MarkerData *) theMarker->instanceData;


	//set the horizontal offset
	instanceData->xOffset = xOffset ;

	Q3Shared_Edited(theMarker);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Marker_GetYOffset : Get vertical offset of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_GetYOffset(TQ3GeometryObject theMarker, TQ3Int32 *yOffset)
{
	const TQ3MarkerData *	instanceData = (const TQ3MarkerData *) theMarker->instanceData;
	
	
	//get the vertical offset
	*yOffset = instanceData->yOffset ;

	return(kQ3Success);
}





//=============================================================================
//      E3Marker_SetYOffset : Set vertical offset of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_SetYOffset(TQ3GeometryObject theMarker, TQ3Int32 yOffset)
{
	TQ3MarkerData *		instanceData = (TQ3MarkerData *) theMarker->instanceData;


	//set the vertical offset of a marker
	instanceData->yOffset = yOffset ;

	Q3Shared_Edited(theMarker);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Marker_GetBitmap : Get the bitmap of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_GetBitmap(TQ3GeometryObject theMarker, TQ3Bitmap *bitmap)
{
	const TQ3MarkerData *	instanceData = (const TQ3MarkerData *) theMarker->instanceData;
	
	//get the bitmap
	return(E3Bitmap_Replace( &instanceData->bitmap, bitmap, kQ3False ));
}





//=============================================================================
//      E3Marker_SetBitmap : Set the bitmap of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_SetBitmap(TQ3GeometryObject theMarker, const TQ3Bitmap *bitmap)
{
	TQ3MarkerData *		instanceData = (TQ3MarkerData *) theMarker->instanceData;
	TQ3Status			qd3dStatus;



	//set the bitmap
	qd3dStatus = E3Bitmap_Replace( bitmap, &instanceData->bitmap, kQ3True );
	Q3Shared_Edited(theMarker);
	
	return(qd3dStatus);
}



/*  NAME:
        E3GeometryPixmapMarker.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3Geometry.h"
#include "E3GeometryPixmapMarker.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_pixmapmarker_get_data : Get the data for a marker's image.
//-----------------------------------------------------------------------------
//		Note : If possible, we try and avoid copying the image data.
//-----------------------------------------------------------------------------
static TQ3Uns8 *
e3geom_pixmapmarker_get_data(const TQ3PixmapMarkerData *instanceData, TQ3Boolean *wasCopied)
{	TQ3Uns32			validSize, bufferSize;
	TQ3StorageObject	theStorage;
	TQ3Status			qd3dStatus;
	TQ3Uns8				*basePtr;
	TQ3ObjectType		theType;
	


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(instanceData), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(wasCopied), NULL);



	// Fetch the storage field into a local variable for readability
	theStorage = instanceData->pixmap.image;



	// Get a pointer to the data for the image, ideally without copying
	*wasCopied = kQ3False;
	theType    = Q3Storage_GetType(theStorage);
	switch (theType) {
		case kQ3StorageTypeMemory:
			qd3dStatus = Q3MemoryStorage_GetBuffer(theStorage, &basePtr, &validSize, &bufferSize);
			break;

#if QUESA_OS_MACINTOSH
		case kQ3MemoryStorageTypeHandle:
			{	Handle	theHnd;
			
			qd3dStatus = Q3HandleStorage_Get(theStorage, &theHnd, &validSize);
			if (qd3dStatus == kQ3Success && theHnd != NULL)
				{
			    MoveHHi(theHnd);
    			HLock(theHnd);
    			basePtr = (TQ3Uns8 *) *theHnd;
    			}
    		}
    		break;
#endif

		default:
			qd3dStatus = Q3Storage_GetSize(theStorage, &bufferSize);
			if (qd3dStatus == kQ3Success)
				basePtr = (TQ3Uns8 *) Q3Memory_Allocate(bufferSize);
			
			if (basePtr != NULL)
				{
				qd3dStatus = Q3Storage_GetData(theStorage, 0, bufferSize, basePtr, &validSize);
				*wasCopied = (TQ3Boolean) (qd3dStatus == kQ3Success);
				
				if (qd3dStatus != kQ3Success)
					Q3Memory_Free(&basePtr);
				}
			break;
		}



	// Return the image
	if (qd3dStatus != kQ3Success)
		return(NULL);
	
	return(basePtr);
}





//=============================================================================
//      e3geom_pixmapmarker_release_data : Release the data for an image.
//-----------------------------------------------------------------------------
//		Note :	If we copied the data for the image, we dispose of it here.
//
//				We also need to check for Mac Handle storage objects, and
//				unlock the handle - we left it locked when the image data was
//				acquired, since it may have saved us from having to copy the
//				data.
//-----------------------------------------------------------------------------
static void
e3geom_pixmapmarker_release_data(const TQ3PixmapMarkerData	*instanceData,
									TQ3Uns8					*basePtr,
									TQ3Boolean				wasCopied)
{
	TQ3StorageObject	theStorage;
#if QUESA_OS_MACINTOSH
	TQ3Status			qd3dStatus;
	TQ3Uns32			validSize;
	TQ3ObjectType		theType;
	Handle				theHnd;
#endif



	// Fetch the storage field into a local variable for readability
	theStorage = instanceData->pixmap.image;



	// If this is a Mac handle object, unlock the handle
#if QUESA_OS_MACINTOSH
	theType = Q3Storage_GetType(theStorage);
	if (theType == kQ3MemoryStorageTypeHandle)
		{
		qd3dStatus = Q3HandleStorage_Get(theStorage, &theHnd, &validSize);
		if (qd3dStatus == kQ3Success && theHnd != NULL)
			HUnlock(theHnd);
		}
#endif



	// If the data was copied, dispose of it
	if (wasCopied)
		Q3Memory_Free(&basePtr);
}





//=============================================================================
//      e3geom_pixmapmarker_pixel_is_set : Is a pixel within a marker set?
//-----------------------------------------------------------------------------
//		Note :	Potentially very inefficient, since we may need to copy the
///				entire marker data from storage into a local buffer on each
//				call.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3geom_pixmapmarker_pixel_is_set(const TQ3PixmapMarkerData *instanceData, TQ3Int32 x, TQ3Int32 y)
{	TQ3Uns8			*thePtr, *basePtr;
	TQ3Boolean		wasCopied, isSet;
	TQ3Uns32		thePixel;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(instanceData), kQ3False);



	// Check to see if the pixel is in range
	if (x < 0 || x >= (TQ3Int32) instanceData->pixmap.width ||
		y < 0 || y >= (TQ3Int32) instanceData->pixmap.height)
		return(kQ3False);



	// Check to see if the pixel type has alpha - if not, they're all set
	if (instanceData->pixmap.pixelType != kQ3PixelTypeARGB32 &&
		instanceData->pixmap.pixelType != kQ3PixelTypeARGB16)
		return(kQ3True);		



	// Get the data for the marker
	basePtr = e3geom_pixmapmarker_get_data(instanceData, &wasCopied);
	if (basePtr == NULL)
		return(kQ3False);



	// Find the right row within the image
	thePtr = basePtr + (y * instanceData->pixmap.rowBytes);



	// Test the pixel within the row
	if (instanceData->pixmap.pixelType == kQ3PixelTypeARGB32)
		{
		thePixel = (TQ3Uns32) *((TQ3Uns32 *) (thePtr + (x * 4)));
		thePixel = (thePixel & 0xFF000000) >> 24;
		}
	
	else
		{
		Q3_ASSERT(instanceData->pixmap.pixelType == kQ3PixelTypeARGB16);
		thePixel = (TQ3Uns32) *((TQ3Uns16 *) (thePtr + (x * 2)));
		thePixel = (thePixel & 0x00008000) >> 15;
		}

	isSet = (TQ3Boolean) (thePixel != 0x00);



	// Clean up
	e3geom_pixmapmarker_release_data(instanceData, basePtr, wasCopied);
	
	return(isSet);
}





//=============================================================================
//      e3geom_pixmapmarker_new : PixmapMarker new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_pixmapmarker_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3PixmapMarkerData			*instanceData  = (TQ3PixmapMarkerData *)       privateData;
	const TQ3PixmapMarkerData	*pixmapMarkerData  = (const TQ3PixmapMarkerData *) paramData;
	TQ3Status					qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3PixmapMarkerData));
	
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
//      e3geom_pixmapmarker_pick_window_point : Pixmap window-point picking.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_pixmapmarker_pick_window_point(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3PixmapMarkerData		*instanceData = (const TQ3PixmapMarkerData *) objectData;
	TQ3Status						qd3dStatus    = kQ3Success;
	TQ3Point2D						windowPoint, markerPixel;
	TQ3WindowPointPickData			pickData;



	// Get the pick data
	Q3WindowPointPick_GetData(thePick, &pickData);



	// Transform our point to the screen, and adjust it by our offset
	// to get the location of the top-left pixel within the image.
	Q3View_TransformLocalToWindow(theView, &instanceData->position, &windowPoint);
	windowPoint.x += instanceData->xOffset;
	windowPoint.y += instanceData->yOffset;



	// Calculate where in our image the pick point will fall
	markerPixel.x = pickData.point.x - windowPoint.x;
	markerPixel.y = pickData.point.y - windowPoint.y;



	// See if we fall within the pick
	if (e3geom_pixmapmarker_pixel_is_set(instanceData, (TQ3Int32) markerPixel.x, (TQ3Int32) markerPixel.y))
		qd3dStatus = E3Pick_RecordHit(thePick, theView, NULL, NULL, NULL, NULL);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_pixmapmarker_pick_window_rect : Pixmap window-rect picking.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_pixmapmarker_pick_window_rect(TQ3ViewObject theView, TQ3PickObject thePick, TQ3Object theObject, const void *objectData)
{	const TQ3PixmapMarkerData		*instanceData = (const TQ3PixmapMarkerData *) objectData;
	TQ3Status						qd3dStatus    = kQ3Success;
	TQ3Area							markerRect;
	TQ3WindowRectPickData			pickData;
	TQ3Uns32						x, y;



	// Get the pick data
	Q3WindowRectPick_GetData(thePick, &pickData);



	// Transform our point to the screen, adjust it by our offset to get the
	// location of the top-left pixel within the image, and add our width/height
	// to get the location of the botom-right pixel.
	Q3View_TransformLocalToWindow(theView, &instanceData->position, &markerRect.min);

	markerRect.min.x += instanceData->xOffset;
	markerRect.min.y += instanceData->yOffset;

	markerRect.max.x = markerRect.min.x + instanceData->pixmap.width;
	markerRect.max.y = markerRect.min.y + instanceData->pixmap.height;



	// See if we fall within the pick
	if (E3Rect_IntersectRect(&markerRect, &pickData.rect))
		{
		// Look for an active pixel within the pick rect
		for (y = 0; y < instanceData->pixmap.height; y++)
			{
			for (x = 0; x < instanceData->pixmap.width; x++)
				{
				if (e3geom_pixmapmarker_pixel_is_set(instanceData, x, y))
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
//      e3geom_pixmapmarker_pick : Pixmap marker picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_pixmapmarker_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status			qd3dStatus;
	TQ3PickObject		thePick;



	// Handle the pick
	thePick = E3View_AccessPick(theView);
	switch (Q3Pick_GetType(thePick)) {
		case kQ3PickTypeWindowPoint:
			qd3dStatus = e3geom_pixmapmarker_pick_window_point(theView, thePick, theObject, objectData);
			break;

		case kQ3PickTypeWindowRect:
			qd3dStatus = e3geom_pixmapmarker_pick_window_rect(theView, thePick, theObject, objectData);
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
	Q3Object_CleanDispose(&pixmapMarkerData->pixmap.image);
	Q3Object_CleanDispose(&pixmapMarkerData->pixmapMarkerAttributeSet);

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
	Q3Object_CleanDispose(&instanceData->pixmap.image);

	instanceData->pixmap = *pixmap;

	E3Shared_Replace(&instanceData->pixmap.image, pixmap->image);

	Q3Shared_Edited(pixmapMarker);

	return(kQ3Success);
}

/*  NAME:
        E3GeometryMarker.c

    DESCRIPTION:
        Implementation of Quesa Marker geometry class.

    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

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
#include "E3Utils.h"
#include "E3Geometry.h"
#include "E3GeometryMarker.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Marker : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypeMarker, E3Marker, E3Geometry )
public :

	TQ3MarkerData			instanceData ;

	} ;
	


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
	byteOffset = (TQ3Uns32) (x / 8);
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
#pragma unused(theObject)



	// Dispose of our instance data
	Q3Marker_EmptyData(instanceData);
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
	TQ3AttributeSet		dupSet;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Marker_GetData(fromObject, toInstanceData);
	
	if ( (qd3dStatus == kQ3Success) &&
		(toInstanceData->markerAttributeSet != NULL) )
	{
		dupSet = Q3Object_Duplicate( toInstanceData->markerAttributeSet );
		Q3Object_Dispose( toInstanceData->markerAttributeSet );
		toInstanceData->markerAttributeSet = dupSet;
		if (dupSet == NULL)
		{
			qd3dStatus = kQ3Failure;
		}
	}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_marker_cache_new : Marker cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_marker_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3MarkerData *geomData)
{	TQ3Uns32					x, y,rowBytes, theSize;
	TQ3Uns16					*pixelPtr, thePixel;
	TQ3PixmapMarkerData			pixmapMarkerData;
	TQ3GeometryObject			pixmapMarker;
	TQ3StorageObject			theStorage;
	TQ3Uns8						*theBuffer;
	TQ3ColorRGB					theColour;



	// Get the marker colour
	//
	// Not clear if bitmap markers can be tinted by a colour inherited from
	// the currently active attribute set, or if they are either white or
	// the colour in the marker attribute set.
	//
	// If the former (i.e., this code is incorrect), remove this section and
	// the marker attribute set should be inherited with whatever is current
	// when the underlying TriMesh is submitted (however this would then.
	//
	// At present colours do not tint either bitmap or pixmap markers: if this
	// it to be changed for bitmaps, re-test the pixmap case to see if QD3D's
	// behaviour was the same for both geometries (or only for bitmaps).
	Q3ColorRGB_Set(&theColour, 1.0f, 1.0f, 1.0f);
	
	if (geomData->markerAttributeSet != NULL)
		Q3AttributeSet_Get(geomData->markerAttributeSet, kQ3AttributeTypeDiffuseColor, &theColour);
	
	thePixel = (                         0x0001 << 15) |
			   (((TQ3Uns16) (theColour.r * 31)) << 10) |
			   (((TQ3Uns16) (theColour.g * 31)) <<  5) |
			   (((TQ3Uns16) (theColour.b * 31)) <<  0);



	// Convert the bitmap to an image
	rowBytes  = geomData->bitmap.width * sizeof(TQ3Uns16);
	theSize   = rowBytes * geomData->bitmap.height;

	theBuffer = (TQ3Uns8*) Q3Memory_AllocateClear(theSize);
	if (theBuffer == NULL)
		return(NULL);

	for (y = 0; y < geomData->bitmap.height; y++)
		{
		for (x = 0; x < geomData->bitmap.width; x++)
			{
			if (Q3Bitmap_GetBit(&geomData->bitmap, x, y))
				{
				pixelPtr  = (TQ3Uns16 *) &theBuffer[(y * rowBytes) + (x * sizeof(TQ3Uns16))];
				*pixelPtr = thePixel;
				}
			}
		}



	// Create the storage object for the image
	theStorage = Q3MemoryStorage_New((unsigned char*)theBuffer, theSize);
	if (theStorage == NULL)
		{
		Q3Memory_Free(&theBuffer);
		return(NULL);
		}



	// Create the Pixmap Marker
	pixmapMarkerData.position                 = geomData->location;
	pixmapMarkerData.xOffset                  = geomData->xOffset;
	pixmapMarkerData.yOffset                  = geomData->yOffset;
	pixmapMarkerData.pixmap.image             = theStorage;
	pixmapMarkerData.pixmap.width             = geomData->bitmap.width;
	pixmapMarkerData.pixmap.height            = geomData->bitmap.height;
    pixmapMarkerData.pixmap.rowBytes          = rowBytes;
    pixmapMarkerData.pixmap.pixelSize         = 16;
    pixmapMarkerData.pixmap.pixelType         = kQ3PixelTypeARGB16;
    pixmapMarkerData.pixmap.bitOrder          = kQ3EndianBig;
    pixmapMarkerData.pixmap.byteOrder         = kQ3EndianBig;
	pixmapMarkerData.pixmapMarkerAttributeSet = geomData->markerAttributeSet;

	pixmapMarker = Q3PixmapMarker_New(&pixmapMarkerData);



	// Clean up
	Q3Memory_Free(&theBuffer);
	Q3Object_Dispose(theStorage);
	
	return(pixmapMarker);
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
				if (e3geom_marker_pixel_is_set(instanceData, (TQ3Int32) x, (TQ3Int32) y))
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
e3geom_marker_get_attribute ( E3Marker* marker )
	{
	// Return the address of the geometry attribute set
	return & marker->instanceData.markerAttributeSet ;
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

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_marker_cache_new;
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
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryMarker,
								e3geom_marker_metahandler,
								E3Marker ) ;
	}





//=============================================================================
//      E3GeometryMarker_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryMarker_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypeMarker, kQ3True);
	
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
	theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeMarker, kQ3False, markerData);
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
	E3Marker* marker = (E3Marker*) theMarker ;

	//set the data
	marker->instanceData.location	= markerData->location ;
	marker->instanceData.xOffset	= markerData->xOffset ;
	marker->instanceData.yOffset	= markerData->yOffset ;
	
	// copy the bitmap
	E3Bitmap_Replace ( & markerData->bitmap, & marker->instanceData.bitmap, kQ3True ) ;
	
	//copy the attribute set
	E3Shared_Replace ( & marker->instanceData.markerAttributeSet, markerData->markerAttributeSet ) ;

	Q3Shared_Edited ( marker ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Marker_GetData : Get the data associated with a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_GetData(TQ3GeometryObject theMarker, TQ3MarkerData *markerData)
	{
	const E3Marker* marker = (const E3Marker*) theMarker ;
	
	markerData->location	= marker->instanceData.location ;
	markerData->xOffset		= marker->instanceData.xOffset ;
	markerData->yOffset		= marker->instanceData.yOffset ;
	
	//copy the attributes
	markerData->markerAttributeSet = NULL ;
	
	E3Geometry_GetAttributeSet ( theMarker, &markerData->markerAttributeSet ) ;
	
	//copy the bitmap
	E3Bitmap_Replace ( &marker->instanceData.bitmap, &markerData->bitmap, kQ3False ) ;
	
	return kQ3Success ;
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
	const E3Marker* marker = (const E3Marker*) theMarker ;
		
	//get the location
	*location = marker->instanceData.location ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Marker_SetPosition : Set the position of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_SetPosition(TQ3GeometryObject theMarker, const TQ3Point3D *location)
	{
	E3Marker* marker = (E3Marker*) theMarker ;
	
	
	//set the location
	marker->instanceData.location = *location ;

	Q3Shared_Edited ( marker ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Marker_GetXOffset : Get horizontal offset of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_GetXOffset(TQ3GeometryObject theMarker, TQ3Int32 *xOffset)
	{
	const E3Marker* marker = (const E3Marker*) theMarker ;


	//get the horizontal offset
	*xOffset = marker->instanceData.xOffset ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Marker_SetXOffset : Set horizontal offset of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_SetXOffset(TQ3GeometryObject theMarker, TQ3Int32 xOffset)
	{
	E3Marker* marker = (E3Marker*) theMarker ;


	//set the horizontal offset
	marker->instanceData.xOffset = xOffset ;

	Q3Shared_Edited ( marker ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Marker_GetYOffset : Get vertical offset of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_GetYOffset(TQ3GeometryObject theMarker, TQ3Int32 *yOffset)
	{
	const E3Marker* marker = (const E3Marker*) theMarker ;
	
	
	//get the vertical offset
	*yOffset = marker->instanceData.yOffset ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Marker_SetYOffset : Set vertical offset of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_SetYOffset(TQ3GeometryObject theMarker, TQ3Int32 yOffset)
	{
	E3Marker* marker = (E3Marker*) theMarker ;


	//set the vertical offset of a marker
	marker->instanceData.yOffset = yOffset ;

	Q3Shared_Edited ( marker ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Marker_GetBitmap : Get the bitmap of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_GetBitmap(TQ3GeometryObject theMarker, TQ3Bitmap *bitmap)
	{
	const E3Marker* marker = (const E3Marker*) theMarker ;
	
	//get the bitmap
	return E3Bitmap_Replace ( & marker->instanceData.bitmap, bitmap, kQ3False ) ;
	}





//=============================================================================
//      E3Marker_SetBitmap : Set the bitmap of a marker
//-----------------------------------------------------------------------------
TQ3Status
E3Marker_SetBitmap(TQ3GeometryObject theMarker, const TQ3Bitmap *bitmap)
	{
	E3Marker* marker = (E3Marker*) theMarker ;

	//set the bitmap
	TQ3Status qd3dStatus = E3Bitmap_Replace ( bitmap, & marker->instanceData.bitmap, kQ3True ) ;
	
	Q3Shared_Edited ( marker ) ;
	
	return qd3dStatus ;
	}



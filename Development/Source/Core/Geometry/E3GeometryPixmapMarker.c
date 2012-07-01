/*  NAME:
        E3GeometryPixmapMarker.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3Geometry.h"
#include "E3GeometryPixmapMarker.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3PixmapMarker : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypePixmapMarker, E3PixmapMarker, E3Geometry )
public :

	TQ3PixmapMarkerData		instanceData ;

	} ;
	


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
	TQ3Uns8				*basePtr = NULL;
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
			qd3dStatus = Q3MemoryStorage_GetBuffer(theStorage, (unsigned char**)&basePtr, &validSize, &bufferSize);
			break;

#if QUESA_OS_MACINTOSH
		case kQ3MemoryStorageTypeHandle:
			{	Handle	theHnd;
			
			qd3dStatus = Q3HandleStorage_Get(theStorage, &theHnd, &validSize);
			if (qd3dStatus == kQ3Success && theHnd != NULL)
				{
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
				qd3dStatus = Q3Storage_GetData(theStorage, 0, bufferSize, (unsigned char*)basePtr, &validSize);
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
		thePixel = (TQ3Uns32) *((TQ3Uns32 *)(void*) (thePtr + (x * 4)));
		thePixel = (thePixel & 0xFF000000) >> 24;
		}
	
	else
		{
		Q3_ASSERT(instanceData->pixmap.pixelType == kQ3PixelTypeARGB16);
		thePixel = (TQ3Uns32) *((TQ3Uns16 *)(void*) (thePtr + (x * 2)));
		thePixel = (thePixel & 0x00008000) >> 15;
		}

	isSet = (TQ3Boolean) (thePixel != 0x00);



	// Clean up
	e3geom_pixmapmarker_release_data(instanceData, basePtr, wasCopied);
	
	return(isSet);
}





//=============================================================================
//      e3geom_pixmapmarker_update_position : Update the position.
//-----------------------------------------------------------------------------
static void
e3geom_pixmapmarker_update_position(TQ3ViewObject theView, const TQ3PixmapMarkerData *geomData, TQ3Object cachedGeom)
{	TQ3Matrix4x4		theMatrix, worldToFrustum, frustumToWindow;
	TQ3TransformObject		theTransform;
	TQ3Status				qd3dStatus;
	TQ3GroupPosition		groupPos;
	TQ3Point3D				thePoint;
	TQ3Vector3D				theOffset;
	


	// Find the translation transform
	qd3dStatus = Q3Group_GetFirstPositionOfType(cachedGeom, kQ3TransformTypeTranslate, &groupPos);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3Group_GetPositionObject(cachedGeom, groupPos, &theTransform);
	
	if (qd3dStatus != kQ3Success)
		return;



	// Determine where to place the marker
	Q3View_GetWorldToFrustumMatrixState( theView, &worldToFrustum);
	Q3View_GetFrustumToWindowMatrixState(theView, &frustumToWindow);

	Q3Matrix4x4_Multiply(E3View_State_GetMatrixLocalToWorld(theView), &worldToFrustum, &theMatrix);
	Q3Matrix4x4_Multiply(&theMatrix, &frustumToWindow, &theMatrix);

	Q3Point3D_Transform(&geomData->position, &theMatrix, &thePoint);



	// Update the transform
	//
	// The canonical frustum ranges from 0..-1 in z (near..far). Since a
	// a rasterise camera transform produces z values from 0..1 (near..far)
	// we need to invert z to place the marker at the correct depth.
	theOffset.x =  thePoint.x + geomData->xOffset;
	theOffset.y =  thePoint.y + geomData->yOffset;
	theOffset.z = -thePoint.z;

	Q3TranslateTransform_Set(theTransform, &theOffset);
	
	
	
	// Get rid of the extra reference from Q3Group_GetPositionObject
	Q3Object_Dispose( theTransform );
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
#pragma unused(theObject)



	// Dispose of our instance data
	Q3PixmapMarker_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_pixmapmarker_duplicate : PixmapMarker duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_pixmapmarker_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						      TQ3Object toObject,   void *toPrivateData)
{	TQ3PixmapMarkerData		*toInstanceData = (TQ3PixmapMarkerData *) toPrivateData;
	TQ3Status				qd3dStatus;
	TQ3AttributeSet		dupSet;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3PixmapMarker_GetData(fromObject, toInstanceData);
	
	if ( (qd3dStatus == kQ3Success) &&
		(toInstanceData->pixmapMarkerAttributeSet != NULL) )
	{
		dupSet = Q3Object_Duplicate( toInstanceData->pixmapMarkerAttributeSet );
		Q3Object_Dispose( toInstanceData->pixmapMarkerAttributeSet );
		toInstanceData->pixmapMarkerAttributeSet = dupSet;
		if (dupSet == NULL)
		{
			qd3dStatus = kQ3Failure;
		}
	}

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
				if (e3geom_pixmapmarker_pixel_is_set(instanceData, (TQ3Int32) x, (TQ3Int32) y))
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
//      e3geom_pixmapmarker_cache_new : Pixmap marker cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_pixmapmarker_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3PixmapMarkerData *geomData)
{	const TQ3Vector3D			theOrigin = { 0.0f, 0.0f, 0.0f };
	TQ3TransformObject			transformRasterise, transformTranslate;
	TQ3StyleObject				styleFill, styleOrientation;
	TQ3TriMeshAttributeData		vertexAttributes[2];
	TQ3Vector3D					vertexNormals[4];
	TQ3TriMeshTriangleData		theTriangles[2];
	TQ3Param2D					vertexUVs[4];
	TQ3Point3D					thePoints[4];
	TQ3TriMeshData				triMeshData;
	TQ3TextureObject			theTexture;
	TQ3GeometryObject			theTriMesh;
	TQ3ShaderObject				theShader;
	TQ3GroupObject				theGroup;
	TQ3Uns32					n;
	


	// Create the group	
	theGroup = Q3OrderedDisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Create the rendering state
	//
	// Markers are unaffected by illumination, fill, or orientation.
	theShader        = Q3NULLIllumination_New();
	styleFill        = Q3FillStyle_New(kQ3FillStyleFilled);
	styleOrientation = Q3OrientationStyle_New(kQ3OrientationStyleCounterClockwise);
	
	Q3Group_AddObjectAndDispose(theGroup, &theShader);
	Q3Group_AddObjectAndDispose(theGroup, &styleFill);
	Q3Group_AddObjectAndDispose(theGroup, &styleOrientation);



	// Create the transforms
	//
	// We insert a rasterise transform to ensure we draw in pixel coordinates, and
	// a translate transform to allow us to position the marker within the window.
	transformRasterise = Q3RasterizeCameraTransform_New();
	transformTranslate = Q3TranslateTransform_New(&theOrigin);

	Q3Group_AddObjectAndDispose(theGroup, &transformRasterise);
	Q3Group_AddObjectAndDispose(theGroup, &transformTranslate);



	// Initialise the points, edges, and triangles
	//
	// The marker is drawn from within a rasterise transform, so x and y are in pixels.
	Q3Point3D_Set(&thePoints[0],                           0.0f,                            0.0f, 0.0f);
	Q3Point3D_Set(&thePoints[1], (float) geomData->pixmap.width,                            0.0f, 0.0f);
	Q3Point3D_Set(&thePoints[2], (float) geomData->pixmap.width, (float) geomData->pixmap.height, 0.0f);
	Q3Point3D_Set(&thePoints[3],                           0.0f, (float) geomData->pixmap.height, 0.0f);

	theTriangles[0].pointIndices[0] = 0;
	theTriangles[0].pointIndices[1] = 2;
	theTriangles[0].pointIndices[2] = 1;

	theTriangles[1].pointIndices[0] = 0;
	theTriangles[1].pointIndices[1] = 3;
	theTriangles[1].pointIndices[2] = 2;



	// Initialise the vertex attributes
	Q3Param2D_Set(&vertexUVs[0], 0.0f, 1.0f);
	Q3Param2D_Set(&vertexUVs[1], 1.0f, 1.0f);
	Q3Param2D_Set(&vertexUVs[2], 1.0f, 0.0f);
	Q3Param2D_Set(&vertexUVs[3], 0.0f, 0.0f);
	
	for (n = 0; n < 4; n++)
		Q3Vector3D_Set(&vertexNormals[n], 0.0f, 0.0f, -1.0f);
		
	vertexAttributes[0].attributeType     = kQ3AttributeTypeSurfaceUV;
	vertexAttributes[0].data              = vertexUVs;
	vertexAttributes[0].attributeUseArray = NULL;
	
	vertexAttributes[1].attributeType     = kQ3AttributeTypeNormal;
	vertexAttributes[1].data              = vertexNormals;
	vertexAttributes[1].attributeUseArray = NULL;



	// Initialise the TriMesh data
	triMeshData.numPoints                 = 4;
	triMeshData.points                    = thePoints;
	triMeshData.numTriangles              = 2;
	triMeshData.triangles                 = theTriangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = 0;
	triMeshData.edges                     = NULL;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 2;
	triMeshData.vertexAttributeTypes      = vertexAttributes;
	triMeshData.triMeshAttributeSet       = geomData->pixmapMarkerAttributeSet;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox, triMeshData.points, triMeshData.numPoints, sizeof(TQ3Point3D));



	// Create the TriMesh
	theTriMesh = Q3TriMesh_New(&triMeshData);
	if (theTriMesh != NULL)
		Q3Group_AddObjectAndDispose(theGroup, &theTriMesh);



	// Create the texture
	theTexture = Q3PixmapTexture_New(&geomData->pixmap);
	if (theTexture != NULL)
		{
		theShader = Q3TextureShader_New(theTexture);
		if (theShader != NULL)
			{
			Q3Shader_SetUBoundary(theShader, kQ3ShaderUVBoundaryClamp);
			Q3Shader_SetVBoundary(theShader, kQ3ShaderUVBoundaryClamp);
			Q3Group_AddObjectAndDispose(theGroup, &theShader);
			}

		Q3Object_Dispose(theTexture);
		}



	// Update the position
	e3geom_pixmapmarker_update_position(theView, geomData, theGroup);

	return(theGroup);
}





//=============================================================================
//      e3geom_pixmapmarker_cache_isvalid : Pixmap marker cache test method.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3geom_pixmapmarker_cache_isvalid(TQ3ViewObject		theView,
									TQ3ObjectType	objectType, TQ3GeometryObject theGeom,
									const void		*geomData,  TQ3Object         *cachedGeom)
{
#pragma unused(theView)
#pragma unused(objectType)
#pragma unused(theGeom)
#pragma unused(geomData)
#pragma unused(cachedGeom)



	// Our cached form always requires an update
	return(kQ3False);
}





//=============================================================================
//      e3geom_pixmapmarker_cache_update : Pixmap marker cache update method.
//-----------------------------------------------------------------------------
static void
e3geom_pixmapmarker_cache_update(TQ3ViewObject		theView,
									TQ3ObjectType	objectType, TQ3GeometryObject theGeom,
									const void		*geomData,  TQ3Object         *cachedGeom)
{
#pragma unused(objectType)



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(cachedGeom));



	// Create a new object if required
	if (*cachedGeom == NULL)
		*cachedGeom = e3geom_pixmapmarker_cache_new(theView, theGeom,
			(const TQ3PixmapMarkerData*)geomData);



	// Or update the position of an existing object
	else
		e3geom_pixmapmarker_update_position(theView, (const TQ3PixmapMarkerData*)geomData,
			*cachedGeom);
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
	
	return kQ3Success ;
	}





//=============================================================================
//      e3geom_pixmapmarker_get_attribute : Pixmap marker attribute pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_pixmapmarker_get_attribute ( E3PixmapMarker* pixmapMarker )
	{
	// Return the address of the geometry attribute set
	return & pixmapMarker->instanceData.pixmapMarkerAttributeSet ;
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

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_pixmapmarker_cache_new;
			break;

		case kQ3XMethodTypeGeomCacheIsValid:
			theMethod = (TQ3XFunctionPointer) e3geom_pixmapmarker_cache_isvalid;
			break;

		case kQ3XMethodTypeGeomCacheUpdate:
			theMethod = (TQ3XFunctionPointer) e3geom_pixmapmarker_cache_update;
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
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryPixmapMarker,
								e3geom_pixmapmarker_metahandler,
								E3PixmapMarker ) ;
	}





//=============================================================================
//      E3GeometryPixmapMarker_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryPixmapMarker_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypePixmapMarker, kQ3True);
	
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
	theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypePixmapMarker, kQ3False, pixmapMarkerData);
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
E3PixmapMarker_SetData(TQ3GeometryObject thePixmapMarker, const TQ3PixmapMarkerData *pixmapMarkerData)
	{
	E3PixmapMarker* pixmapMarker = (E3PixmapMarker*) thePixmapMarker ;

	// Set the data
	pixmapMarker->instanceData.position         = pixmapMarkerData->position ;
	pixmapMarker->instanceData.xOffset          = pixmapMarkerData->xOffset ;
	pixmapMarker->instanceData.yOffset          = pixmapMarkerData->yOffset ;
	pixmapMarker->instanceData.pixmap.width     = pixmapMarkerData->pixmap.width ;
	pixmapMarker->instanceData.pixmap.height    = pixmapMarkerData->pixmap.height ;
	pixmapMarker->instanceData.pixmap.rowBytes  = pixmapMarkerData->pixmap.rowBytes ;
	pixmapMarker->instanceData.pixmap.pixelSize = pixmapMarkerData->pixmap.pixelSize ;
	pixmapMarker->instanceData.pixmap.pixelType = pixmapMarkerData->pixmap.pixelType ;
	pixmapMarker->instanceData.pixmap.bitOrder  = pixmapMarkerData->pixmap.bitOrder ;
	pixmapMarker->instanceData.pixmap.byteOrder = pixmapMarkerData->pixmap.byteOrder ;
	
	E3Shared_Replace ( & pixmapMarker->instanceData.pixmap.image, pixmapMarkerData->pixmap.image ) ;
	E3Shared_Replace  (& pixmapMarker->instanceData.pixmapMarkerAttributeSet, pixmapMarkerData->pixmapMarkerAttributeSet ) ;

	Q3Shared_Edited ( pixmapMarker ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PixmapMarker_GetData : Get the data for a pixmap marker.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_GetData(TQ3GeometryObject thePixmapMarker, TQ3PixmapMarkerData *pixmapMarkerData)
	{
	E3PixmapMarker* pixmapMarker = (E3PixmapMarker*) thePixmapMarker ;

	// Get the data
	pixmapMarkerData->position         = pixmapMarker->instanceData.position ;
	pixmapMarkerData->xOffset          = pixmapMarker->instanceData.xOffset ;
	pixmapMarkerData->yOffset          = pixmapMarker->instanceData.yOffset ;
	pixmapMarkerData->pixmap.width     = pixmapMarker->instanceData.pixmap.width ;
	pixmapMarkerData->pixmap.height    = pixmapMarker->instanceData.pixmap.height ;
	pixmapMarkerData->pixmap.rowBytes  = pixmapMarker->instanceData.pixmap.rowBytes ;
	pixmapMarkerData->pixmap.pixelSize = pixmapMarker->instanceData.pixmap.pixelSize ;
	pixmapMarkerData->pixmap.pixelType = pixmapMarker->instanceData.pixmap.pixelType ;
	pixmapMarkerData->pixmap.bitOrder  = pixmapMarker->instanceData.pixmap.bitOrder ;
	pixmapMarkerData->pixmap.byteOrder = pixmapMarker->instanceData.pixmap.byteOrder ;
	
	E3Shared_Acquire ( & pixmapMarkerData->pixmap.image,             pixmapMarker->instanceData.pixmap.image ) ;
	E3Shared_Acquire ( & pixmapMarkerData->pixmapMarkerAttributeSet, pixmapMarker->instanceData.pixmapMarkerAttributeSet ) ;

	return kQ3Success ;
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
E3PixmapMarker_GetPosition(TQ3GeometryObject thePixmapMarker, TQ3Point3D *position)
	{
	const E3PixmapMarker* pixmapMarker = (const E3PixmapMarker*) thePixmapMarker ;
	
	//get the position
	*position = pixmapMarker->instanceData.position ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3PixmapMarker_SetPosition : Set the position of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_SetPosition(TQ3GeometryObject thePixmapMarker, const TQ3Point3D *position)
	{
	E3PixmapMarker* pixmapMarker = (E3PixmapMarker*) thePixmapMarker ;
	
	//set the position
	pixmapMarker->instanceData.position = *position ;	

	Q3Shared_Edited ( pixmapMarker ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PixmapMarker_GetXOffset : Get horizontal position of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_GetXOffset(TQ3GeometryObject thePixmapMarker, TQ3Int32 *xOffset)
	{
	const E3PixmapMarker* pixmapMarker = (const E3PixmapMarker*) thePixmapMarker ;
	
	//get the horizontal offset
	*xOffset = pixmapMarker->instanceData.xOffset ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3PixmapMarker_SetXOffset : Set horizontal position of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_SetXOffset(TQ3GeometryObject thePixmapMarker, TQ3Int32 xOffset)
	{
	E3PixmapMarker* pixmapMarker = (E3PixmapMarker*) thePixmapMarker ;
	
	//set the horizontal offset
	pixmapMarker->instanceData.xOffset = xOffset ;

	Q3Shared_Edited(pixmapMarker);
	
	return kQ3Success ;
	}





//=============================================================================
//      E3PixmapMarker_GetYOffset :	Get the vertical position of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_GetYOffset(TQ3GeometryObject thePixmapMarker, TQ3Int32 *yOffset)
	{
	const E3PixmapMarker* pixmapMarker = (const E3PixmapMarker*) thePixmapMarker ;
	
	//get the vertical offset
	*yOffset = pixmapMarker->instanceData.yOffset ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3PixmapMarker_SetYOffset :	Set the vertical position of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_SetYOffset(TQ3GeometryObject thePixmapMarker, TQ3Int32 yOffset)
	{
	E3PixmapMarker* pixmapMarker = (E3PixmapMarker*) thePixmapMarker ;
	
	//set the vertical offset
	pixmapMarker->instanceData.yOffset = yOffset ;

	Q3Shared_Edited ( pixmapMarker ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3PixmapMarker_GetPixmap :	Get the Pixmap of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_GetPixmap(TQ3GeometryObject thePixmapMarker, TQ3StoragePixmap *pixmap)
	{
	E3PixmapMarker* pixmapMarker = (E3PixmapMarker*) thePixmapMarker ;

	// Get the pixmap
	*pixmap = pixmapMarker->instanceData.pixmap ;
	
	E3Shared_Acquire ( & pixmap->image, pixmapMarker->instanceData.pixmap.image ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3PixmapMarker_SetPixmap :	Set the Pixmap of a PixmapMarker
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapMarker_SetPixmap(TQ3GeometryObject thePixmapMarker, const TQ3StoragePixmap *pixmap)
	{
	E3PixmapMarker* pixmapMarker = (E3PixmapMarker*) thePixmapMarker ;

	// Set the pixmap
	Q3Object_CleanDispose ( & pixmapMarker->instanceData.pixmap.image ) ;

	pixmapMarker->instanceData.pixmap = *pixmap ;

	E3Shared_Replace ( & pixmapMarker->instanceData.pixmap.image, pixmap->image ) ;

	Q3Shared_Edited ( pixmapMarker ) ;

	return kQ3Success ;
	}

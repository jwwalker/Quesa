/*  NAME:
        E3DrawContext.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
#include "E3DrawContext.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3drawcontext_pixmap_new : Pixmap draw context new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_pixmap_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DrawContextUnionData			*instanceData = (TQ3DrawContextUnionData *) privateData;
	const TQ3PixmapDrawContextData	*pixmapData   = (const TQ3PixmapDrawContextData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->data.pixmapData = *pixmapData;

	return(kQ3Success);
}





//=============================================================================
//      e3drawcontext_pixmap_delete : Pixmap draw context delete method.
//-----------------------------------------------------------------------------
static void
e3drawcontext_pixmap_delete(TQ3Object theObject, void *privateData)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) privateData;
	TQ3Status					qd3dStatus;
#pragma unused(privateData)



	// Dispose of the common instance data
	qd3dStatus = E3DrawContext_CreateRegions(theObject, 0);

	if (instanceData->data.common.maskState)
		qd3dStatus = Q3Bitmap_Empty(&instanceData->data.common.mask);
}





//=============================================================================
//      e3drawcontext_pixmap_update : Pixmap draw context update method.
//-----------------------------------------------------------------------------
//		Note :	Because Pixmap draw contexts don't have any relationship with
//				a window system, their draw region state is substantially
//				simpler than other draw context types.
//
//				We support Pixmap draw contexts by creating a single draw
//				region, and rebuilding the draw region from scratch if any
//				state changes occur.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_pixmap_update(TQ3DrawContextObject theDrawContext)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->instanceData;
	TQ3Pixmap					*thePixmap;
	TQ3Status					qd3dStatus;
	TQ3XDevicePixelType			pixelType;



	// If we have a draw region, and nothing has changed, we're done
	if (instanceData->numDrawRegions != 0 && instanceData->theState == kQ3XDrawContextValidationClearFlags)
		return(kQ3Success);



	// Build a single draw region
	qd3dStatus = E3DrawContext_CreateRegions(theDrawContext, 1);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Configure the draw region. Note that we don't bother setting up
	// the colorDescriptor field of the draw region descriptor since
	// these fields aren't currently used by the Interactive Renderer.
	//
	// Since we want to move away from draw regions, this is OK for now.
	//
	// Clipping masks aren't currently supported.
	thePixmap = &instanceData->data.pixmapData.pixmap;
	pixelType = E3DrawContext_GetDevicePixelTypeFromQD3DType(thePixmap->pixelType);
	
	instanceData->drawRegions[0].deviceOffsetX           = 0.0f;
	instanceData->drawRegions[0].deviceOffsetY           = 0.0f;
	instanceData->drawRegions[0].windowOffsetX           = 0.0f;
	instanceData->drawRegions[0].windowOffsetY           = 0.0f;
	instanceData->drawRegions[0].deviceScaleX            = (float) thePixmap->width;
	instanceData->drawRegions[0].deviceScaleY            = (float) thePixmap->height;
	instanceData->drawRegions[0].windowScaleX            = (float) thePixmap->width;
	instanceData->drawRegions[0].windowScaleY            = (float) thePixmap->height;
	instanceData->drawRegions[0].theDescriptor.width		 = thePixmap->width ;
	instanceData->drawRegions[0].theDescriptor.height		 = thePixmap->height;
	instanceData->drawRegions[0].theDescriptor.rowBytes	 = thePixmap->rowBytes;
	instanceData->drawRegions[0].theDescriptor.pixelSize = thePixmap->pixelSize;
	instanceData->drawRegions[0].theDescriptor.pixelType = pixelType;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.redShift	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.redMask	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.greenShift	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.greenMask	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.blueShift	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.blueMask	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.alphaShift	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.alphaMask	 = ???;
	instanceData->drawRegions[0].theDescriptor.bitOrder	 = thePixmap->bitOrder;
	instanceData->drawRegions[0].theDescriptor.byteOrder = thePixmap->byteOrder;
	instanceData->drawRegions[0].theDescriptor.clipMask = NULL;
	instanceData->drawRegions[0].imageBuffer             = thePixmap->image;
	instanceData->drawRegions[0].isActive                = kQ3True;
	instanceData->drawRegions[0].clipMaskState           = kQ3XClipMaskFullyExposed;

	// clear the DrawContext
	if(instanceData->data.common.clearImageMethod == kQ3ClearMethodWithColor){
		NULL;
		}
		


	// Update the state flag
	instanceData->theState = kQ3XDrawContextValidationAll;

	return(kQ3Success);		
}





//=============================================================================
//      e3drawcontext_pixmap_metahandler : Pixmap draw context metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3drawcontext_pixmap_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_pixmap_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_pixmap_delete;
			break;

		case kQ3XMethodTypeDrawContextUpdate:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_pixmap_update;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3DrawContext_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3DrawContext_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the draw context classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
											kQ3SharedTypeDrawContext,
											kQ3ClassNameDrawContext,
											NULL,
											0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeDrawContext,
												kQ3DrawContextTypePixmap,
												kQ3ClassNameDrawContextPixmap,
												e3drawcontext_pixmap_metahandler,
												sizeof(TQ3DrawContextUnionData));

#if QUESA_OS_MACINTOSH
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3MacDrawContext_RegisterClass();

#elif QUESA_OS_UNIX
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3XDrawContext_RegisterClass();

#elif QUESA_OS_WIN32
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3Win32DCDrawContext_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3DDSurfaceDrawContext_RegisterClass();

#elif QUESA_OS_BE
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3BeDrawContext_RegisterClass();
#endif

	return(qd3dStatus);
}





//=============================================================================
//      E3DrawContext_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the draw context classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3DrawContextTypePixmap, kQ3True);

#if QUESA_OS_MACINTOSH
	qd3dStatus = E3MacDrawContext_UnregisterClass();

#elif QUESA_OS_UNIX
	qd3dStatus = E3XDrawContext_UnregisterClass();

#elif QUESA_OS_WIN32
	qd3dStatus = E3Win32DCDrawContext_UnregisterClass();
	qd3dStatus = E3DDSurfaceDrawContext_UnregisterClass();

#elif QUESA_OS_BE
	qd3dStatus = E3BeDrawContext_UnregisterClass();
#endif

	qd3dStatus = E3ClassTree_UnregisterClass(kQ3SharedTypeDrawContext, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3DrawContext_Update : Return the type of a draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_Update(TQ3DrawContextObject drawContext)
{	TQ3XDrawContextUpdateMethod		updateMethod;
	TQ3Status						qd3dStatus;



	// Find the method
	updateMethod = (TQ3XDrawContextUpdateMethod)
							E3ClassTree_GetMethod(drawContext->theClass,
												  kQ3XMethodTypeDrawContextUpdate);
	if (updateMethod == NULL)
		return(kQ3Success);



	// Call the method
	qd3dStatus = updateMethod(drawContext);
	return(qd3dStatus);
}





//=============================================================================
//      E3DrawContext_ResetState : Reset the validation state.
//-----------------------------------------------------------------------------
//		Note :	Used by the view to reset the draw context state when an event
//				(like changing the renderer) occurs.
//-----------------------------------------------------------------------------
void
E3DrawContext_ResetState(TQ3DrawContextObject drawContext)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Reset the state
	instanceData->theState = kQ3XDrawContextValidationAll;
}





//=============================================================================
//      E3DrawContext_CreateRegions : Update the draw region array.
//-----------------------------------------------------------------------------
//		Note :	Rebuilds the draw region array to provide numRegions draw
//				regions for the draw context.
//
//				If a draw region array already exists, we dispose of it and any
//				renderer private data it contains first.
//
//				Initialises the ownership pointers for the draw regions, but
//				otherwise leaves every field set to 0s.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_CreateRegions(TQ3DrawContextObject drawContext, TQ3Uns32 numRegions)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;
	TQ3Uns32					n;



	// If we have any draw regions, get rid of them
	if (instanceData->numDrawRegions != 0)
		{
		// Dispose of any renderer private data
		Q3_ASSERT_VALID_PTR(instanceData->drawRegions);
		for (n = 0; n < instanceData->numDrawRegions; n++)
			{
			Q3_ASSERT(instanceData->drawRegions[n].theOwner == instanceData);
			if (instanceData->drawRegions[n].rendererPrivate       != NULL &&
				instanceData->drawRegions[n].rendererPrivateDelete != NULL)
				{
				instanceData->drawRegions[n].rendererPrivateDelete(instanceData->drawRegions[n].rendererPrivate);
				}			
			}



		// Dispose of the draw region array
		E3Memory_Free(&instanceData->drawRegions);
		instanceData->numDrawRegions = 0;
		}



	// If we need any new draw regions, allocate them
	if (numRegions != 0)
		{
		// Allocate the draw region array
		instanceData->drawRegions = (OpaqueTQ3XDrawRegion *) E3Memory_AllocateClear(
																sizeof(OpaqueTQ3XDrawRegion) * numRegions);
		if (instanceData->drawRegions == NULL)
			return(kQ3Failure);



		// Initialise it
		instanceData->numDrawRegions = numRegions;
		for (n = 0; n < instanceData->numDrawRegions; n++)
			{
			instanceData->drawRegions[n].ownerIndex = n;
			instanceData->drawRegions[n].theOwner   = instanceData;
			Q3Matrix4x4_SetIdentity(&instanceData->drawRegions[n].deviceTransform);
			}
		}

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetDevicePixelTypeFromBPP : Get a pixel type.
//-----------------------------------------------------------------------------
TQ3XDevicePixelType
E3DrawContext_GetDevicePixelTypeFromBPP(TQ3Uns32 pixelSize)
{	TQ3XDevicePixelType		pixelType;



	// Select the appropriate type
	switch (pixelSize) {
		case 32:
			pixelType = kQ3XDevicePixelTypeRGB32;
			break;
		case 24:
			pixelType = kQ3XDevicePixelTypeRGB24;
			break;
		case 16:
			pixelType = kQ3XDevicePixelTypeRGB16;
			break;
		case 8:
			pixelType = kQ3XDevicePixelTypeIndexed8;
			break;
		case 4:
			pixelType = kQ3XDevicePixelTypeIndexed4;
			break;
		case 2:
			pixelType = kQ3XDevicePixelTypeIndexed2;
			break;
		case 1:
			pixelType = kQ3XDevicePixelTypeIndexed1;
			break;
		default:
			pixelType = kQ3XDevicePixelTypeInvalid;
			break;
		}

	return(pixelType);
}





//=============================================================================
//      E3DrawContext_GetDevicePixelTypeFromQD3DType : Get a pixel type.
//-----------------------------------------------------------------------------
TQ3XDevicePixelType
E3DrawContext_GetDevicePixelTypeFromQD3DType(TQ3PixelType qd3dType)
{	TQ3XDevicePixelType		pixelType;



	// Select the appropriate type
	switch (qd3dType) {
		case kQ3PixelTypeRGB32:
			pixelType = kQ3XDevicePixelTypeRGB32;
			break;
		case kQ3PixelTypeARGB32:
			pixelType = kQ3XDevicePixelTypeARGB32;
			break;
		case kQ3PixelTypeRGB16:
			pixelType = kQ3XDevicePixelTypeRGB16;
			break;
		case kQ3PixelTypeARGB16:
			pixelType = kQ3XDevicePixelTypeARGB16;
			break;
		case kQ3PixelTypeRGB16_565:
			pixelType = kQ3XDevicePixelTypeRGB16_565;
			break;
		case kQ3PixelTypeRGB24:
			pixelType = kQ3XDevicePixelTypeRGB24;
			break;
		default:
			pixelType = kQ3XDevicePixelTypeInvalid;
			break;
		}

	return(pixelType);
}





//=============================================================================
//      E3DrawContext_GetType : Return the type of a draw context.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3DrawContext_GetType(TQ3DrawContextObject drawContext)
{


	// Return the type
	return(E3ClassTree_GetObjectType(drawContext, kQ3SharedTypeDrawContext));
}





//=============================================================================
//      E3DrawContext_SetData : Set the common data for a draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetData(TQ3DrawContextObject drawContext, const TQ3DrawContextData *contextData)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the data and update our flags
	instanceData->data.common = *contextData;
	instanceData->theState   |= kQ3XDrawContextValidationAll;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetData : Return the common data for a draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetData(TQ3DrawContextObject drawContext, TQ3DrawContextData *contextData)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the data
	*contextData = instanceData->data.common;
	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_SetClearImageColor : Set the clear colour.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetClearImageColor(TQ3DrawContextObject drawContext, const TQ3ColorARGB *color)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the clear colour and update our flags
	instanceData->data.common.clearImageColor = *color;
	instanceData->theState                   |= kQ3XDrawContextValidationBackgroundShader;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetClearImageColor : Return the clear colour.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetClearImageColor(TQ3DrawContextObject drawContext, TQ3ColorARGB *color)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the clear colour
	*color = instanceData->data.common.clearImageColor;
	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_SetPane : Set the pane.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetPane(TQ3DrawContextObject drawContext, const TQ3Area *pane)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the pane and update our flags
	instanceData->data.common.pane = *pane;
	instanceData->theState        |= kQ3XDrawContextValidationPane;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetPane : Return the pane.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetPane(TQ3DrawContextObject drawContext, TQ3Area *pane)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the pane
	*pane = instanceData->data.common.pane;
	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_SetPaneState : Set the pane state.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetPaneState(TQ3DrawContextObject drawContext, TQ3Boolean state)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the pane state and update our flags
	instanceData->data.common.paneState = state;
	instanceData->theState             |= kQ3XDrawContextValidationPane;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetPaneState : Return the pane state.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetPaneState(TQ3DrawContextObject drawContext, TQ3Boolean *state)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the pane state
	*state = instanceData->data.common.paneState;
	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_SetClearImageMethod : Set the clear image method.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetClearImageMethod(TQ3DrawContextObject drawContext, TQ3DrawContextClearImageMethod method)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the clear image method and update our flags
	instanceData->data.common.clearImageMethod = method;
	instanceData->theState                    |= kQ3XDrawContextValidationClearFunction;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetClearImageMethod : Return the clear image method.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetClearImageMethod(TQ3DrawContextObject drawContext, TQ3DrawContextClearImageMethod *method)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the clear image method
	*method = instanceData->data.common.clearImageMethod;
	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_SetMask : Set the mask.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetMask(TQ3DrawContextObject drawContext, const TQ3Bitmap *mask)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;
	TQ3Status					qd3dStatus;



	// Copy the mask image and update our flags
	qd3dStatus = E3Bitmap_Replace(mask, &instanceData->data.common.mask, kQ3True);
	instanceData->theState |= kQ3XDrawContextValidationMask;
	Q3Shared_Edited(drawContext);

	return(qd3dStatus);
}





//=============================================================================
//      E3DrawContext_GetMask : Return a copy of the mask.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetMask(TQ3DrawContextObject drawContext, TQ3Bitmap *mask)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;
	TQ3Status					qd3dStatus;



	// Copy the mask image and update our flags
	qd3dStatus = E3Bitmap_Replace(&instanceData->data.common.mask, mask, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      E3DrawContext_SetMaskState : Set the mask state.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetMaskState(TQ3DrawContextObject drawContext, TQ3Boolean state)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the mask state and update our flags
	instanceData->data.common.maskState = state;
	instanceData->theState             |= kQ3XDrawContextValidationMask;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetMaskState : Return the mask state.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetMaskState(TQ3DrawContextObject drawContext, TQ3Boolean *state)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the mask state
	*state = instanceData->data.common.maskState;
	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_SetDoubleBufferState : Set the double buffer state.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetDoubleBufferState(TQ3DrawContextObject drawContext, TQ3Boolean state)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the double buffer state and update our flags
	instanceData->data.common.doubleBufferState = state;
	instanceData->theState                     |= kQ3XDrawContextValidationDoubleBuffer;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetDoubleBufferState : Return the double buffer state.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetDoubleBufferState(TQ3DrawContextObject drawContext, TQ3Boolean *state)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the double buffer state
	*state = instanceData->data.common.doubleBufferState;
	return(kQ3Success);
}





//=============================================================================
//      E3PixmapDrawContext_New : Create a pixmap draw context.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3DrawContextObject
E3PixmapDrawContext_New(const TQ3PixmapDrawContextData *drawContextData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3DrawContextTypePixmap, kQ3False, drawContextData);
	return(theObject);
}





//=============================================================================
//      E3PixmapDrawContext_SetPixmap : Set the pixmap.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapDrawContext_SetPixmap(TQ3DrawContextObject drawContext, const TQ3Pixmap *pixmap)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;




	// Set the pixmap and update our flag
	instanceData->data.pixmapData.pixmap = *pixmap;
	instanceData->theState              |= kQ3XDrawContextValidationAll;
	Q3Shared_Edited(drawContext);

	return(kQ3Success);
}





//=============================================================================
//      E3PixmapDrawContext_GetPixmap : Return the pixmap.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapDrawContext_GetPixmap(TQ3DrawContextObject drawContext, TQ3Pixmap *pixmap)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Return the pixmap
	*pixmap = instanceData->data.pixmapData.pixmap;
	return(kQ3Success);
}








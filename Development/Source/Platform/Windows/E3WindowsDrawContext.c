/*  NAME:
        E3WindowsDrawContext.c

    DESCRIPTION:
        Windows specific Draw Context calls.

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
#include "E3DrawContext.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3drawcontext_win32dc_new : Win32DC draw context new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_win32dc_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DrawContextUnionData			*instanceData = (TQ3DrawContextUnionData *) privateData;
	const TQ3Win32DCDrawContextData	*win32Data    = (const TQ3Win32DCDrawContextData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->data.win32Data.theData = *win32Data;

	return(kQ3Success);
}



//=============================================================================
//      e3drawcontext_win32dc_delete : Win32DC draw context delete method.
//-----------------------------------------------------------------------------
static void
e3drawcontext_win32dc_delete(TQ3Object theObject, void *privateData)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) privateData;
	TQ3Status					qd3dStatus;
#pragma unused(privateData)



	// Dispose of the common instance data
	qd3dStatus = E3DrawContext_CreateRegions(theObject, 0);

	if (instanceData->data.common.maskState)
		qd3dStatus = Q3Bitmap_Empty(&instanceData->data.common.mask);
}





//=============================================================================
//      e3drawcontext_win32dc_update : Win32DC draw context update method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_win32dc_update(TQ3DrawContextObject theDrawContext)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->instanceData;
	TQ3Status					qd3dStatus;
	BITMAP						bitMap;
	HGDIOBJ						hBitMap;
	TQ3Uns32					cx,cy;
	TQ3XDevicePixelType			pixelType;



	// If we have a draw region, and nothing has changed, we're done
	if (instanceData->numDrawRegions != 0 && instanceData->theState == kQ3XDrawContextValidationClearFlags)
		return(kQ3Success);



	// Build a single draw region
	qd3dStatus = E3DrawContext_CreateRegions(theDrawContext, 1);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	//
	// Clipping masks aren't currently supported.
	
	// Create a little bitmap to query color information
	hBitMap = CreateCompatibleBitmap(instanceData->data.win32Data.theData.hdc, 1, 1);
	if(hBitMap == NULL){
		Q3Error_PlatformPost(GetLastError());
		return(kQ3Failure);
		}
		
	cx = GetObject(hBitMap,sizeof(BITMAP),&bitMap);
	DeleteObject(hBitMap);
	
	if(cx != sizeof(BITMAP)){
		Q3Error_PlatformPost(GetLastError());
		return(kQ3Failure);
		}
		
	cx = (TQ3Uns32)(instanceData->data.common.pane.max.x - instanceData->data.common.pane.min.x);
	cy = (TQ3Uns32)(instanceData->data.common.pane.max.y - instanceData->data.common.pane.min.y);
	
	pixelType = E3DrawContext_GetDevicePixelTypeFromBPP(bitMap.bmBitsPixel);
	
	instanceData->drawRegions[0].deviceOffsetX           = 0.0f;
	instanceData->drawRegions[0].deviceOffsetY           = 0.0f;
	instanceData->drawRegions[0].windowOffsetX           = 0.0f;
	instanceData->drawRegions[0].windowOffsetY           = 0.0f;
	instanceData->drawRegions[0].deviceScaleX            = (float) cx;
	instanceData->drawRegions[0].deviceScaleY            = (float) cy;
	instanceData->drawRegions[0].windowScaleX            = instanceData->drawRegions[0].deviceScaleX;
	instanceData->drawRegions[0].windowScaleY            = instanceData->drawRegions[0].deviceScaleY;
	instanceData->drawRegions[0].theDescriptor.width		 = cx;
	instanceData->drawRegions[0].theDescriptor.height		 = cy;
	instanceData->drawRegions[0].theDescriptor.rowBytes	 = 2 * ((cx * bitMap.bmBitsPixel + 15) / 16);
	instanceData->drawRegions[0].theDescriptor.pixelSize = bitMap.bmBitsPixel;
	instanceData->drawRegions[0].theDescriptor.pixelType = pixelType;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.redShift	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.redMask	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.greenShift	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.greenMask	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.blueShift	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.blueMask	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.alphaShift	 = ???;
	//instanceData->drawRegions[0].theDescriptor.colorDescriptor.alphaMask	 = ???;
	instanceData->drawRegions[0].theDescriptor.bitOrder	 = kQ3EndianLittle;
	instanceData->drawRegions[0].theDescriptor.byteOrder = kQ3EndianLittle;
	instanceData->drawRegions[0].theDescriptor.clipMask = NULL;
	instanceData->drawRegions[0].imageBuffer             = NULL;
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
//      e3drawcontext_win32dc_metahandler : Win32DC draw context metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3drawcontext_win32dc_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_win32dc_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_win32dc_delete;
			break;

		case kQ3XMethodTypeDrawContextUpdate:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_win32dc_update;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3drawcontext_ddsurface_new : DD surface draw context new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_ddsurface_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DrawContextUnionData				*instanceData = (TQ3DrawContextUnionData *) privateData;
	const TQ3DDSurfaceDrawContextData	*winDDData    = (const TQ3DDSurfaceDrawContextData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->data.winDDData.theData = *winDDData;

	return(kQ3Success);
}





//=============================================================================
//      e3drawcontext_ddsurface_metahandler : DD surface metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3drawcontext_ddsurface_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_ddsurface_new;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Win32DCDrawContext_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Win32DCDrawContext_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeDrawContext,
											kQ3DrawContextTypeWin32DC,
											kQ3ClassNameDrawContextWin32DC,
											e3drawcontext_win32dc_metahandler,
											sizeof(TQ3DrawContextUnionData));

	return(qd3dStatus);
}





//=============================================================================
//      E3Win32DCDrawContext_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Win32DCDrawContext_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3DrawContextTypeWin32DC, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Win32DCDrawContext_New : Create a new Win32 draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3Win32DCDrawContext_New(const TQ3Win32DCDrawContextData *drawContextData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3DrawContextTypeWin32DC, kQ3False, drawContextData);
	return(theObject);
}





//=============================================================================
//      E3Win32DCDrawContext_SetDC : Set the DC for a draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3Win32DCDrawContext_SetDC(TQ3DrawContextObject drawContext, HDC newHDC)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the field and reset our flag
	if (instanceData->data.win32Data.theData.hdc != newHDC)
		{
		instanceData->data.win32Data.theData.hdc = newHDC;
		instanceData->theState                  |= kQ3XDrawContextValidationAll;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3Win32DCDrawContext_GetDC : Get the DC for a draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3Win32DCDrawContext_GetDC(TQ3DrawContextObject drawContext, HDC *curHDC)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Get the field
	*curHDC = instanceData->data.win32Data.theData.hdc;
	
	return(kQ3Success);
}





//=============================================================================
//      E3DDSurfaceDrawContext_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
TQ3Status
E3DDSurfaceDrawContext_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeDrawContext,
											kQ3DrawContextTypeDDSurface,
											kQ3ClassNameDrawContextDDSurface,
											e3drawcontext_ddsurface_metahandler,
											sizeof(TQ3DrawContextUnionData));

	return(qd3dStatus);
}





//=============================================================================
//      E3DDSurfaceDrawContext_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3DDSurfaceDrawContext_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3DrawContextTypeDDSurface, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3DDSurfaceDrawContext_New : Create a new DD Surface draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3DDSurfaceDrawContext_New(const TQ3DDSurfaceDrawContextData *drawContextData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3DrawContextTypeDDSurface, kQ3False, drawContextData);
	return(theObject);
}





//=============================================================================
//      E3DDSurfaceDrawContext_SetDirectDrawSurface : Set the surface.
//-----------------------------------------------------------------------------
TQ3Status
E3DDSurfaceDrawContext_SetDirectDrawSurface(TQ3DrawContextObject drawContext, const TQ3DDSurfaceDescriptor *ddSurfaceDescriptor)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the field and reset our flag. We don't compare the current
	// state, and assume that setting a new dd surface may cause a rebuild.
	instanceData->data.winDDData.theData.ddSurfaceDescriptor = *ddSurfaceDescriptor;
	instanceData->theState                                  |= kQ3XDrawContextValidationAll;

	Q3Shared_Edited(drawContext);
	return(kQ3Success);
}





//=============================================================================
//      E3DDSurfaceDrawContext_GetDirectDrawSurface : Get the surface.
//-----------------------------------------------------------------------------
TQ3Status
E3DDSurfaceDrawContext_GetDirectDrawSurface(TQ3DrawContextObject drawContext, TQ3DDSurfaceDescriptor *ddSurfaceDescriptor)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Get the field
	*ddSurfaceDescriptor = instanceData->data.winDDData.theData.ddSurfaceDescriptor;
	
	return(kQ3Success);
}






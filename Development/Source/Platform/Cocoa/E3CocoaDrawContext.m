/*  NAME:
        E3CocoaDrawContext.m

    DESCRIPTION:
        Cocoa specific draw context implementation.

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
#include "E3DrawContext.h"
#include "E3CocoaDrawContextNotify.h"
#import <Cocoa/Cocoa.h>





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3drawcontext_cocoa_new : Cocoa draw context new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_cocoa_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DrawContextUnionData			*instanceData = (TQ3DrawContextUnionData *) privateData;
	const TQ3CocoaDrawContextData	*cocoaData    = (const TQ3CocoaDrawContextData *) paramData;



	// Initialise our instance data
	instanceData->data.cocoaData.theData = *cocoaData;



	// Register our notification callback
    e3cocoa_nsview_register(cocoaData->nsView, theObject);

	return(kQ3Success);
}





//=============================================================================
//      e3drawcontext_cocoa_delete : Cocoa draw context delete method.
//-----------------------------------------------------------------------------
static void
e3drawcontext_cocoa_delete(TQ3Object theObject, void *privateData)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) privateData;
	TQ3Status					qd3dStatus;
#pragma unused(privateData)



	// Unregister our notification callback
    e3cocoa_nsview_unregister(instanceData->data.cocoaData.theData.nsView);



	// Dispose of the common instance data
	qd3dStatus = E3DrawContext_CreateRegions(theObject, 0);

	if (instanceData->data.common.maskState)
		qd3dStatus = Q3Bitmap_Empty(&instanceData->data.common.mask);
}





//=============================================================================
//      e3drawcontext_cocoa_update : Cocoa draw context update method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_cocoa_update(TQ3DrawContextObject theDrawContext)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->instanceData;
	TQ3Status					qd3dStatus;
	TQ3Uns32					cx, cy;




	// If we have a draw region, and nothing has changed, we're done
	if (instanceData->numDrawRegions != 0 && instanceData->theState == kQ3XDrawContextValidationClearFlags)
		return(kQ3Success);



	// Build a single draw region
	qd3dStatus = E3DrawContext_CreateRegions(theDrawContext, 1);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Fill it in. Note that draw regions are only needed to support Apple's
	// Interactive Renderer, which does not run on any Cocoa systems: and so
	// we can just fill in the basic details for the structure.
	cx = (TQ3Uns32) E3Num_Max(instanceData->data.common.pane.max.x - instanceData->data.common.pane.min.x, 0.0f);
	cy = (TQ3Uns32) E3Num_Max(instanceData->data.common.pane.max.y - instanceData->data.common.pane.min.y, 0.0f);
	
	instanceData->drawRegions[0].deviceOffsetX           = 0.0f;
	instanceData->drawRegions[0].deviceOffsetY           = 0.0f;
	instanceData->drawRegions[0].windowOffsetX           = 0.0f;
	instanceData->drawRegions[0].windowOffsetY           = 0.0f;
	instanceData->drawRegions[0].deviceScaleX            = (float) cx;
	instanceData->drawRegions[0].deviceScaleY            = (float) cy;
	instanceData->drawRegions[0].windowScaleX            = instanceData->drawRegions[0].deviceScaleX;
	instanceData->drawRegions[0].windowScaleY            = instanceData->drawRegions[0].deviceScaleY;
	instanceData->drawRegions[0].theDescriptor.width	 = cx;
	instanceData->drawRegions[0].theDescriptor.height	 = cy;
	instanceData->drawRegions[0].theDescriptor.rowBytes  = 0;
	instanceData->drawRegions[0].theDescriptor.pixelSize = 32;
	instanceData->drawRegions[0].theDescriptor.pixelType = kQ3PixelTypeRGB32;
	instanceData->drawRegions[0].theDescriptor.colorDescriptor.redShift	  = 0;
	instanceData->drawRegions[0].theDescriptor.colorDescriptor.redMask	  = 0;
	instanceData->drawRegions[0].theDescriptor.colorDescriptor.greenShift = 0;
	instanceData->drawRegions[0].theDescriptor.colorDescriptor.greenMask  = 0;
	instanceData->drawRegions[0].theDescriptor.colorDescriptor.blueShift  = 0;
	instanceData->drawRegions[0].theDescriptor.colorDescriptor.blueMask	  = 0;
	instanceData->drawRegions[0].theDescriptor.colorDescriptor.alphaShift = 0;
	instanceData->drawRegions[0].theDescriptor.colorDescriptor.alphaMask  = 0;
	instanceData->drawRegions[0].theDescriptor.bitOrder	 = kQ3EndianBig;
	instanceData->drawRegions[0].theDescriptor.byteOrder = kQ3EndianBig;
	instanceData->drawRegions[0].theDescriptor.clipMask  = NULL;
	instanceData->drawRegions[0].imageBuffer             = NULL;
	instanceData->drawRegions[0].isActive                = kQ3True;
	instanceData->drawRegions[0].clipMaskState           = kQ3XClipMaskFullyExposed;

		


	// Update the state flag
	instanceData->theState = kQ3XDrawContextValidationAll;
	
	return(qd3dStatus);		
}





//=============================================================================
//      e3drawcontext_cocoa_get_dimensions : Cocoa draw context dimensions.
//-----------------------------------------------------------------------------
static void
e3drawcontext_cocoa_get_dimensions(TQ3DrawContextObject theDrawContext, TQ3Area *thePane)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->instanceData;
	NSRect						viewFrame;



	// Return our dimensions
	viewFrame = [(id)instanceData->data.cocoaData.theData.nsView bounds];
	thePane->min.x = (float) viewFrame.origin.x;
	thePane->min.y = (float) viewFrame.origin.y;
	thePane->max.x = (float) (viewFrame.origin.x + viewFrame.size.width);
	thePane->max.y = (float) (viewFrame.origin.y + viewFrame.size.height);
}





//=============================================================================
//      e3drawcontext_cocoa_metahandler : Cocoa draw context metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3drawcontext_cocoa_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_cocoa_new;
			break;
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_cocoa_delete;
			break;

		case kQ3XMethodTypeDrawContextUpdate:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_cocoa_update;
			break;

		case kQ3XMethodTypeDrawContextGetDimensions:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_cocoa_get_dimensions;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3CocoaDrawContext_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
TQ3Status
E3CocoaDrawContext_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeDrawContext,
											kQ3DrawContextTypeCocoa,
											kQ3ClassNameDrawContextCocoa,
											e3drawcontext_cocoa_metahandler,
											sizeof(TQ3DrawContextUnionData));

	return(qd3dStatus);
}





//=============================================================================
//      E3CocoaDrawContext_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3CocoaDrawContext_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3DrawContextTypeCocoa, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3MacDrawContext_New : Create a new Cocoa draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3CocoaDrawContext_New(const TQ3CocoaDrawContextData *drawContextData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3DrawContextTypeCocoa, 
                                           kQ3False, drawContextData);

	return(theObject);
}





//=============================================================================
//      E3CocoaDrawContext_NewWithWindow : Create a new Cocoa Draw Context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3CocoaDrawContext_NewWithWindow(TQ3ObjectType drawContextType, void *drawContextTarget)
{	TQ3CocoaDrawContextData		cocoaDrawContextData;
	TQ3DrawContextData			drawContextData;
	TQ3DrawContextObject		drawContext;



	// Check we have a suitable target for the draw context
	if (drawContextType != kQ3DrawContextTypeCocoa)
		return(NULL);



	// Prepare the draw context
	Q3ColorARGB_Set(&drawContextData.clearImageColor, kQ3DrawContextDefaultBackgroundColour);

	drawContextData.clearImageMethod  = kQ3ClearMethodWithColor;
	drawContextData.paneState         = kQ3False;
	drawContextData.maskState		  = kQ3False;
	drawContextData.doubleBufferState = kQ3True;
	drawContextData.pane.min.x        = 0.0f;
	drawContextData.pane.min.y        = 0.0f;
	drawContextData.pane.max.x        = 0.0f;
	drawContextData.pane.max.y        = 0.0f;

	cocoaDrawContextData.drawContextData = drawContextData;
	cocoaDrawContextData.nsView          = (NSView *) drawContextTarget;



	// Create the draw context
	drawContext = Q3CocoaDrawContext_New(&cocoaDrawContextData);

	return(drawContext);
}





//=============================================================================
//      E3CocoaDrawContext_SetNSView : Set the view for an Cocoa draw context.
//-----------------------------------------------------------------------------
//		Note :	Quesa is not involved in any OS-specific reference counting,
//				and so our caller must ensure that the old NSView is released
//				as appropriate.
//-----------------------------------------------------------------------------
TQ3Status
E3CocoaDrawContext_SetNSView(TQ3DrawContextObject drawContext, void *nsView)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Set the field and reset our flag
	if (instanceData->data.cocoaData.theData.nsView != nsView)
		{
		instanceData->data.cocoaData.theData.nsView = nsView;
		instanceData->theState                     |= kQ3XDrawContextValidationAll;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3CocoaDrawContext_GetDisplay : Get the display for an Cocoa draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3CocoaDrawContext_GetNSView(TQ3DrawContextObject drawContext, void **nsView)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->instanceData;



	// Get the field
	*nsView = instanceData->data.cocoaData.theData.nsView;

	return(kQ3Success);
}


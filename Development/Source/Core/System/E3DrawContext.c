/*  NAME:
        E3DrawContext.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
#include "E3DrawContext.h"
#include "E3Main.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------



class E3PixmapDrawContext : public E3DrawContext  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3DrawContextTypePixmap, E3PixmapDrawContext, E3DrawContext )

public :

	TQ3DrawContextUnionData				instanceData ;
	} ;
	




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      E3CameraInfo::E3CameraInfo : Constructor for class info of the class.
//-----------------------------------------------------------------------------

E3DrawContextInfo::E3DrawContextInfo	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
		: E3SharedInfo ( newClassMetaHandler, newParent ) ,
		updateMethod		( (TQ3XDrawContextUpdateMethod)			Find_Method ( kQ3XMethodTypeDrawContextUpdate ) ) ,		 
		getDimensionsMethod	( (TQ3XDrawContextGetDimensionsMethod)	Find_Method ( kQ3XMethodTypeDrawContextGetDimensions ) )		 
	{
	if ( updateMethod == NULL 
	|| getDimensionsMethod == NULL )
		SetAbstract () ;
	} ;


//=============================================================================
//      e3drawcontext_new_class_info : Method to construct a class info record.
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3drawcontext_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent
			 	)
	{
	return new ( std::nothrow ) E3DrawContextInfo ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3drawcontext_pixmap_new : Pixmap draw context new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_pixmap_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DrawContextUnionData			*instanceData = (TQ3DrawContextUnionData *) privateData;
	const TQ3PixmapDrawContextData	*pixmapData   = (const TQ3PixmapDrawContextData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->data.pixmapData = *pixmapData;

	E3DrawContext_InitaliseData(&instanceData->data.pixmapData.drawContextData);

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
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->FindLeafInstanceData () ;
	TQ3Pixmap					*thePixmap;
	TQ3Status					qd3dStatus;
	TQ3XDevicePixelType			pixelType;
	TQ3Uns32					irrelevantStateFlags =
										kQ3XDrawContextValidationClearFunction |
										kQ3XDrawContextValidationDepthState;



	// If we have a draw region, and nothing relevant has changed, we're done
	if (instanceData->numDrawRegions != 0 &&
		(instanceData->theState & ~irrelevantStateFlags) == kQ3XDrawContextValidationClearFlags)
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
	if(instanceData->data.common.clearImageMethod == kQ3ClearMethodWithColor)
		NULL;
		


	// Update the state flag
	instanceData->theState = kQ3XDrawContextValidationAll;

	return(kQ3Success);		
}





//=============================================================================
//      e3drawcontext_pixmap_get_dimensions : Pixmap draw context dimensions.
//-----------------------------------------------------------------------------
static void
e3drawcontext_pixmap_get_dimensions(TQ3DrawContextObject theDrawContext, TQ3Area *thePane)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) theDrawContext->FindLeafInstanceData () ;



	// Return our dimensions
	thePane->min.x = 0.0f;
	thePane->min.y = 0.0f;
	thePane->max.x = (float) instanceData->data.pixmapData.pixmap.width;
	thePane->max.y = (float) instanceData->data.pixmapData.pixmap.height;
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

		case kQ3XMethodTypeDrawContextGetDimensions:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_pixmap_get_dimensions;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3drawcontext_update : Draw context default update method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_update ( TQ3DrawContextObject theDrawContext )
	{
	return kQ3Success ;
	}
	


//=============================================================================
//      e3drawcontext_get_dimensions : Draw context default dimension method.
//-----------------------------------------------------------------------------
static void
e3drawcontext_get_dimensions(TQ3DrawContextObject theDrawContext, TQ3Area *thePane)
	{
	// Return default dimensions
	thePane->min.x = 0.0f ;
	thePane->min.y = 0.0f ;
	thePane->max.x = 0.0f ;
	thePane->max.y = 0.0f ;
	}





//=============================================================================
//      e3drawcontext_metahandler : Draw context metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3drawcontext_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeNewObjectClass :
			return (TQ3XFunctionPointer) e3drawcontext_new_class_info ;

		case kQ3XMethodTypeDrawContextUpdate:
			return (TQ3XFunctionPointer) e3drawcontext_update;

		case kQ3XMethodTypeDrawContextGetDimensions:
			return (TQ3XFunctionPointer) e3drawcontext_get_dimensions;
		}
	
	return NULL ;
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
	qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameDrawContext,
										e3drawcontext_metahandler,
										E3DrawContext ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameDrawContextPixmap,
											e3drawcontext_pixmap_metahandler,
											E3PixmapDrawContext ) ;

#if QUESA_OS_MACINTOSH
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3MacDrawContext_RegisterClass();

#elif QUESA_OS_UNIX
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3XDrawContext_RegisterClass();

#elif QUESA_OS_WIN32
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3Win32DCDrawContext_RegisterClass();

#if !defined(QD3D_NO_DIRECTDRAW)
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3DDSurfaceDrawContext_RegisterClass();
#endif // QD3D_NO_DIRECTDRAW

#elif QUESA_OS_BE
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3BeDrawContext_RegisterClass();

#elif QUESA_OS_COCOA
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3CocoaDrawContext_RegisterClass();
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
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3DrawContextTypePixmap, kQ3True);

#if QUESA_OS_MACINTOSH
	qd3dStatus = E3MacDrawContext_UnregisterClass();

#elif QUESA_OS_UNIX
	qd3dStatus = E3XDrawContext_UnregisterClass();

#elif QUESA_OS_WIN32
	qd3dStatus = E3Win32DCDrawContext_UnregisterClass();
#if !defined(QD3D_NO_DIRECTDRAW)
	qd3dStatus = E3DDSurfaceDrawContext_UnregisterClass();
#endif // QD3D_NO_DIRECTDRAW

#elif QUESA_OS_BE
	qd3dStatus = E3BeDrawContext_UnregisterClass();

#elif QUESA_OS_COCOA
    qd3dStatus = E3CocoaDrawContext_UnregisterClass();
#endif

	qd3dStatus = E3ClassTree::UnregisterClass(kQ3SharedTypeDrawContext, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3DrawContext_InitaliseData : Initialise the data for a draw context.
//-----------------------------------------------------------------------------
void
E3DrawContext_InitaliseData(TQ3DrawContextData *drawContextData)
{


	// Initialise the data
	//
	// The draw context structure contains two optional fields. The values
	// marked by these fields are normalised on creation, to ensure that
	// we always return consistent values to apps if they later query our
	// state.
	//
	// In addition, this ensures that the mask bitmap field is left in a
	// state where it can be safely replaced without attempting to dispose
	// of an invalid bitmap.
	if (drawContextData->paneState == kQ3False)
		Q3Memory_Clear(&drawContextData->pane, sizeof(drawContextData->pane));

	if (drawContextData->maskState == kQ3False)
		Q3Memory_Clear(&drawContextData->mask, sizeof(drawContextData->mask));
}





//=============================================================================
//      E3DrawContext_New : Create a new draw context object.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3DrawContext_New(TQ3ObjectType drawContextType, void *drawContextTarget)
{	TQ3DrawContextObject		drawContext;



	// Create the draw context object
#if QUESA_OS_MACINTOSH
	drawContext = E3MacDrawContext_NewWithWindow(drawContextType, drawContextTarget);

#elif QUESA_OS_UNIX
	drawContext = E3XDrawContext_NewWithWindow(drawContextType, drawContextTarget);

#elif QUESA_OS_WIN32
	drawContext = E3Win32DCDrawContext_NewWithWindow(drawContextType, drawContextTarget);

#elif QUESA_OS_BE
	drawContext = E3BeDrawContext_NewWithWindow(drawContextType, drawContextTarget);

#elif QUESA_OS_COCOA
    drawContext = E3CocoaDrawContext_NewWithWindow(drawContextType, drawContextTarget);

#else
	drawContext = NULL;
#endif

	return(drawContext);
}





//=============================================================================
//      E3DrawContext_Update : Return the type of a draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext::Update ( void )
	{
	// Call the method
	return GetClass ()->updateMethod ( this ) ;
	}





//=============================================================================
//      E3DrawContext_ResetState : Reset the validation state.
//-----------------------------------------------------------------------------
//		Note :	Used by the view to reset the draw context state when an event
//				(like changing the renderer) occurs.
//-----------------------------------------------------------------------------
void
E3DrawContext_ResetState(TQ3DrawContextObject drawContext)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



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
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;
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
		Q3Memory_Free(&instanceData->drawRegions);
		instanceData->numDrawRegions = 0;
		}



	// If we need any new draw regions, allocate them
	if (numRegions != 0)
		{
		// Allocate the draw region array
		instanceData->drawRegions = (OpaqueTQ3XDrawRegion *) Q3Memory_AllocateClear(
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
	return drawContext->GetObjectType ( kQ3SharedTypeDrawContext ) ;
	}





//=============================================================================
//      E3DrawContext_SetData : Set the common data for a draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetData(TQ3DrawContextObject drawContext, const TQ3DrawContextData *contextData)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



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
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the data
	*contextData = instanceData->data.common;



	// If we don't have a pane set, grab the full size before returning
	if (!contextData->paneState)
		Q3DrawContext_GetPane(drawContext, &contextData->pane);

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_SetClearImageColor : Set the clear colour.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetClearImageColor(TQ3DrawContextObject drawContext, const TQ3ColorARGB *color)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Set the clear colour and update our flags
	if (memcmp(&instanceData->data.common.clearImageColor, color, sizeof(TQ3ColorARGB)) != 0)
		{
		instanceData->data.common.clearImageColor = *color;
		instanceData->theState                   |= kQ3XDrawContextValidationBackgroundShader;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetClearImageColor : Return the clear colour.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetClearImageColor(TQ3DrawContextObject drawContext, TQ3ColorARGB *color)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the clear colour
	*color = instanceData->data.common.clearImageColor;
	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_SetPane : Set the pane.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetPane(TQ3DrawContextObject drawContext, const TQ3Area *pane)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Set the pane and update our flags
	if (memcmp(&instanceData->data.common.pane, pane, sizeof(TQ3Area)) != 0)
		{
		instanceData->data.common.pane = *pane;
		instanceData->theState        |= kQ3XDrawContextValidationPane;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetPane : Return the pane.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext::GetPane ( TQ3Area* pane )
	{
	// The way the data is held is a mess, long term the union should be removed and ALL the common data moved into E3DrawContext
	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) FindLeafInstanceData () ;



	
	if ( instanceData->data.common.paneState )
		*pane = instanceData->data.common.pane ; // We have a pane, return it
	else
		GetClass ()->getDimensionsMethod ( this, pane ) ; // Get the full bounds of the draw context
		

	return kQ3Success ;
	}





//=============================================================================
//      E3DrawContext_SetPaneState : Set the pane state.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetPaneState(TQ3DrawContextObject drawContext, TQ3Boolean state)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Set the pane state and update our flags
	if (instanceData->data.common.paneState != state)
		{
		instanceData->data.common.paneState = state;
		instanceData->theState             |= kQ3XDrawContextValidationPane;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetPaneState : Return the pane state.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetPaneState(TQ3DrawContextObject drawContext, TQ3Boolean *state)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the pane state
	*state = instanceData->data.common.paneState;
	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_SetClearImageMethod : Set the clear image method.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetClearImageMethod(TQ3DrawContextObject drawContext, TQ3DrawContextClearImageMethod method)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Set the clear image method and update our flags
	if (instanceData->data.common.clearImageMethod != method)
		{
		instanceData->data.common.clearImageMethod = method;
		instanceData->theState                    |= kQ3XDrawContextValidationClearFunction;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetClearImageMethod : Return the clear image method.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetClearImageMethod(TQ3DrawContextObject drawContext, TQ3DrawContextClearImageMethod *method)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the clear image method
	*method = instanceData->data.common.clearImageMethod;
	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_SetMask : Set the mask.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetMask(TQ3DrawContextObject drawContext, const TQ3Bitmap *mask)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;
	TQ3Status					qd3dStatus;



	// Copy the mask image and update our flags. We don't compare the current
	// state, and assume that setting a mask may cause a rebuild.
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
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;
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
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Set the mask state and update our flags
	if (instanceData->data.common.maskState != state)
		{
		instanceData->data.common.maskState = state;
		instanceData->theState             |= kQ3XDrawContextValidationMask;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetMaskState : Return the mask state.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetMaskState(TQ3DrawContextObject drawContext, TQ3Boolean *state)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the mask state
	*state = instanceData->data.common.maskState;
	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_SetDoubleBufferState : Set the double buffer state.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_SetDoubleBufferState(TQ3DrawContextObject drawContext, TQ3Boolean state)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Set the double buffer state and update our flags
	if (instanceData->data.common.doubleBufferState != state)
		{
		instanceData->data.common.doubleBufferState = state;
		instanceData->theState                     |= kQ3XDrawContextValidationDoubleBuffer;
		Q3Shared_Edited(drawContext);
		}

	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_GetDoubleBufferState : Return the double buffer state.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_GetDoubleBufferState(TQ3DrawContextObject drawContext, TQ3Boolean *state)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the double buffer state
	*state = instanceData->data.common.doubleBufferState;
	return(kQ3Success);
}





//=============================================================================
//      E3DrawContext_IsOfMyClass : Check if object pointer is valid and of type DrawContext
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3SharedTypeDrawContext )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3DrawContext_IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3DrawContext ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3PixmapDrawContext_New : Create a pixmap draw context.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3DrawContextObject
E3PixmapDrawContext_New(const TQ3PixmapDrawContextData *drawContextData)
	{
	// Create the object
	return E3ClassTree::CreateInstance(kQ3DrawContextTypePixmap, kQ3False, drawContextData);
	}





//=============================================================================
//      E3PixmapDrawContext_SetPixmap : Set the pixmap.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapDrawContext_SetPixmap(TQ3DrawContextObject drawContext, const TQ3Pixmap *pixmap)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;




	// Set the pixmap and update our flag. We don't compare the current
	// state, and assume that setting a new pixmap may cause a rebuild.
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
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Return the pixmap
	*pixmap = instanceData->data.pixmapData.pixmap;
	return(kQ3Success);
}








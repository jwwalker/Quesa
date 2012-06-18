/*  NAME:
        E3DrawContext.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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

	E3DrawContext_InitialiseData(&instanceData->data.pixmapData.drawContextData);

	return(kQ3Success);
}





//=============================================================================
//      e3drawcontext_pixmap_delete : Pixmap draw context delete method.
//-----------------------------------------------------------------------------
static void
e3drawcontext_pixmap_delete(TQ3Object theObject, void *privateData)
{	TQ3DrawContextUnionData		*instanceData = (TQ3DrawContextUnionData *) privateData;
#pragma unused(privateData)



	// Dispose of the common instance data
	if (instanceData->data.common.maskState)
		Q3Bitmap_Empty(&instanceData->data.common.mask);
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
//      e3drawcontext_pixmap_newwithpixmap : Create default pixmap draw context.
//-----------------------------------------------------------------------------
static TQ3DrawContextObject
e3drawcontext_pixmap_newwithpixmap( const TQ3Pixmap* inPixmap )
{
	TQ3DrawContextObject theDC = NULL;
	TQ3PixmapDrawContextData dcData;
	dcData.pixmap = *inPixmap;
	dcData.drawContextData.clearImageMethod = kQ3ClearMethodNone;
	dcData.drawContextData.paneState = kQ3False;
	dcData.drawContextData.maskState = kQ3False;
	dcData.drawContextData.doubleBufferState = kQ3False;
	
	theDC = Q3PixmapDrawContext_New( &dcData );
	
	return theDC;
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
	qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (	kQ3ClassNameDrawContext,
										e3drawcontext_metahandler,
										E3DrawContext ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameDrawContextPixmap,
											e3drawcontext_pixmap_metahandler,
											E3PixmapDrawContext ) ;

#if QUESA_OS_MACINTOSH
	#if QUESA_SUPPORT_HITOOLBOX
	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3MacDrawContext_RegisterClass();
	#endif

	#if QUESA_OS_COCOA
		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3CocoaDrawContext_RegisterClass();
	#endif
	
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


#endif

	return(qd3dStatus);
}





//=============================================================================
//      E3DrawContext_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3DrawContext_UnregisterClass(void)
{
	bool success = true;



	// Unregister the draw context classes
	success = (kQ3Success == E3ClassTree::UnregisterClass(kQ3DrawContextTypePixmap, kQ3True))
		&& success;

#if QUESA_OS_MACINTOSH
	#if QUESA_SUPPORT_HITOOLBOX
		success = (kQ3Success == E3MacDrawContext_UnregisterClass())
			&& success;
	#endif

	#if QUESA_OS_COCOA
		success = (kQ3Success == E3CocoaDrawContext_UnregisterClass())
			&& success;
	#endif

#elif QUESA_OS_UNIX
	qsuccess = (kQ3Success == E3XDrawContext_UnregisterClass())
			&& success;

#elif QUESA_OS_WIN32
	success = (kQ3Success == E3Win32DCDrawContext_UnregisterClass())
			&& success;
#if !defined(QD3D_NO_DIRECTDRAW)
	success = (kQ3Success == E3DDSurfaceDrawContext_UnregisterClass())
			&& success;
#endif // QD3D_NO_DIRECTDRAW

#endif

	success = (kQ3Success == E3ClassTree::UnregisterClass(kQ3SharedTypeDrawContext, kQ3True))
			&& success;

	return (success? kQ3Success : kQ3Failure);
}





//=============================================================================
//      E3DrawContext_InitialiseData : Initialise the data for a draw context.
//-----------------------------------------------------------------------------
void
E3DrawContext_InitialiseData(TQ3DrawContextData *drawContextData)
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
{
	TQ3DrawContextObject		drawContext = NULL;



	// Create the draw context object
	switch (drawContextType)
	{
		case kQ3DrawContextTypePixmap:
			drawContext = e3drawcontext_pixmap_newwithpixmap( (const TQ3Pixmap*)
				drawContextTarget );
			break;
	
	#if QUESA_OS_MACINTOSH
	#if QUESA_SUPPORT_HITOOLBOX
		case kQ3DrawContextTypeMacintosh:
			drawContext = E3MacDrawContext_NewWithWindow(drawContextType, drawContextTarget);
			break;
	#endif
		
	#if QUESA_OS_COCOA
		case kQ3DrawContextTypeCocoa:
			drawContext = E3CocoaDrawContext_NewWithWindow(drawContextType, drawContextTarget);
			break;
	#endif
	#endif
	
	
	#if QUESA_OS_WIN32
		case kQ3DrawContextTypeWin32DC:
			drawContext = E3Win32DCDrawContext_NewWithWindow(drawContextType, drawContextTarget);
			break;
	#endif
	
	
	#if QUESA_OS_UNIX
		case kQ3DrawContextTypeX11:
			drawContext = E3XDrawContext_NewWithWindow(drawContextType, drawContextTarget);
			break;
	#endif
	}

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








/*  NAME:
        E3WindowsDrawContext.c

    DESCRIPTION:
        Windows specific Draw Context calls.

    COPYRIGHT:
        Copyright (c) 1999-2015, Quesa Developers. All rights reserved.

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





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------



class E3Win32DCDrawContext : public E3DrawContext  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
	Q3_CLASS_ENUMS( kQ3DrawContextTypeWin32DC, E3Win32DCDrawContext, E3DrawContext );
public :
	TQ3DrawContextUnionData				instanceData ;
	} ;
	


class E3DDSurfaceDrawContext : public E3DrawContext  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
	Q3_CLASS_ENUMS( kQ3DrawContextTypeDDSurface, E3DDSurfaceDrawContext, E3DrawContext );
public :

	TQ3DrawContextUnionData				instanceData ;
	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3drawcontext_win32dc_get_dimensions_from_DC : Win32DC DC dimensions.
//-----------------------------------------------------------------------------
static void
e3drawcontext_win32dc_get_dimensions_from_DC( HDC theDC, TQ3Area *thePane)
{
	// Return the dimensions associated with our HDC.
	HWND win = WindowFromDC( theDC );
	if (win) {
		// GetClientRect just gives you the width and height.  We'll assume
		// that the coordinates needed are local (HWND) coordinates, such
		// that the top-left corner is 0,0.
		RECT r;
		GetClientRect( win, &r );
		thePane->min.x = (float) 0;
		thePane->min.y = (float) 0;
		thePane->max.x = (float) r.right;
		thePane->max.y = (float) r.bottom;
		}
	else {
		// If for some reason we can't get the HWND, then we'll have to fall back on returning
		// the dimensions of the screen.  This may not be right, but it's probably better
		// than nothing.
		thePane->min.x = 0.0f;
		thePane->min.y = 0.0f;
		thePane->max.x = (float) GetDeviceCaps(theDC, HORZRES);
		thePane->max.y = (float) GetDeviceCaps(theDC, VERTRES);
		}
}





//-----------------------------------------------------------------------------
//      e3drawcontext_win32dc_get_dimensions : Win32DC draw context dimensions.
//-----------------------------------------------------------------------------
static void
e3drawcontext_win32dc_get_dimensions ( E3Win32DCDrawContext* theDrawContext, TQ3Area *thePane )
	{
	e3drawcontext_win32dc_get_dimensions_from_DC( theDrawContext->instanceData.data.win32Data.theData.hdc, thePane );
	}





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
	e3drawcontext_win32dc_get_dimensions_from_DC( instanceData->data.win32Data.theData.hdc,
		&instanceData->data.win32Data.windowRect );

	E3DrawContext_InitialiseData(&instanceData->data.win32Data.theData.drawContextData);

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
	if (instanceData->data.common.maskState)
		qd3dStatus = Q3Bitmap_Empty(&instanceData->data.common.mask);
}





//=============================================================================
//      e3drawcontext_win32dc_update : Win32DC draw context update method.
//-----------------------------------------------------------------------------
static TQ3Status
e3drawcontext_win32dc_update ( E3Win32DCDrawContext* theDrawContext )
{
	// Check whether the window has been resized.
	TQ3Area		newArea;
	e3drawcontext_win32dc_get_dimensions_from_DC( theDrawContext->instanceData.data.win32Data.theData.hdc,
		&newArea );
	if ( (newArea.max.x != theDrawContext->instanceData.data.win32Data.windowRect.max.x) ||
		(newArea.max.y != theDrawContext->instanceData.data.win32Data.windowRect.max.y) )
		{
		theDrawContext->instanceData.data.win32Data.windowRect = newArea;
		theDrawContext->instanceData.theState = kQ3XDrawContextValidationWindowSize;
		}
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

		case kQ3XMethodTypeDrawContextGetDimensions:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_win32dc_get_dimensions;
			break;
		}
	
	return(theMethod);
}




#if !defined(QD3D_NO_DIRECTDRAW)

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

	E3DrawContext_InitialiseData(&instanceData->data.winDDData.theData.drawContextData);

	return(kQ3Success);
}





//=============================================================================
//      e3drawcontext_ddsurface_get_dimensions : DD surface dimensions.
//-----------------------------------------------------------------------------
static void
e3drawcontext_ddsurface_get_dimensions(TQ3DrawContextObject theDrawContext, TQ3Area *thePane)
	{
	// Return our dimensions (not currently implemented for Direct Draw surfaces)
	thePane->min.x = 0.0f;
	thePane->min.y = 0.0f;
	thePane->max.x = 0.0f;
	thePane->max.y = 0.0f;
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

		case kQ3XMethodTypeDrawContextGetDimensions:
			theMethod = (TQ3XFunctionPointer) e3drawcontext_ddsurface_get_dimensions;
			break;
		}
	
	return(theMethod);
}

#endif // QD3D_NO_DIRECTDRAW




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
	qd3dStatus = Q3_REGISTER_CLASS( kQ3ClassNameDrawContextWin32DC,
		e3drawcontext_win32dc_metahandler, E3Win32DCDrawContext );

	return(qd3dStatus);
}





//=============================================================================
//      E3Win32DCDrawContext_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Win32DCDrawContext_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3DrawContextTypeWin32DC, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Win32DCDrawContext_New : Create a new Win32 draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3Win32DCDrawContext_New(const TQ3Win32DCDrawContextData *drawContextData)
	{
	// Create the object
	return E3ClassTree::CreateInstance(kQ3DrawContextTypeWin32DC, kQ3False, drawContextData);
	}





//=============================================================================
//      E3Win32DCDrawContext_NewWithWindow : Create a new Win32 Draw Context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3Win32DCDrawContext_NewWithWindow(TQ3ObjectType drawContextType, void *drawContextTarget)
{	TQ3Win32DCDrawContextData	winDrawContextData;
	TQ3DrawContextData			drawContextData;
	TQ3DrawContextObject		drawContext;



	// Check we have a suitable target for the draw context
	if (drawContextType != kQ3DrawContextTypeWin32DC)
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

	winDrawContextData.drawContextData = drawContextData;
	winDrawContextData.hdc             = (HDC) drawContextTarget;
	


	// Create the draw context
	drawContext = Q3Win32DCDrawContext_New(&winDrawContextData);

	return(drawContext);
}





//=============================================================================
//      E3Win32DCDrawContext_SetDC : Set the DC for a draw context.
//-----------------------------------------------------------------------------
TQ3Status
E3Win32DCDrawContext_SetDC(TQ3DrawContextObject drawContext, HDC newHDC)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



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
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Get the field
	*curHDC = instanceData->data.win32Data.theData.hdc;
	
	return(kQ3Success);
}




#if !defined(QD3D_NO_DIRECTDRAW)

//=============================================================================
//      E3DDSurfaceDrawContext_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
TQ3Status
E3DDSurfaceDrawContext_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = Q3_REGISTER_CLASS( kQ3ClassNameDrawContextDDSurface,
		e3drawcontext_ddsurface_metahandler, E3DDSurfaceDrawContext );

	return(qd3dStatus);
}





//=============================================================================
//      E3DDSurfaceDrawContext_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3DDSurfaceDrawContext_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3DrawContextTypeDDSurface, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3DDSurfaceDrawContext_New : Create a new DD Surface draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
E3DDSurfaceDrawContext_New(const TQ3DDSurfaceDrawContextData *drawContextData)
	{
	// Create the object
	return E3ClassTree::CreateInstance(kQ3DrawContextTypeDDSurface, kQ3False, drawContextData);
	}





//=============================================================================
//      E3DDSurfaceDrawContext_SetDirectDrawSurface : Set the surface.
//-----------------------------------------------------------------------------
TQ3Status
E3DDSurfaceDrawContext_SetDirectDrawSurface(TQ3DrawContextObject drawContext, const TQ3DDSurfaceDescriptor *ddSurfaceDescriptor)
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



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
{	TQ3DrawContextUnionData *instanceData = (TQ3DrawContextUnionData *) drawContext->FindLeafInstanceData () ;



	// Get the field
	*ddSurfaceDescriptor = instanceData->data.winDDData.theData.ddSurfaceDescriptor;
	
	return(kQ3Success);
}

#endif // QD3D_NO_DIRECTDRAW





/*  NAME:
        E3WindowsDrawContext.c

    DESCRIPTION:
        Windows specific Draw Context calls.

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
	instanceData->data.win32Data.theData.hdc = newHDC;
	instanceData->theState                  |= kQ3XDrawContextValidationAll;

	Q3Shared_Edited(drawContext);
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



	// Set the field and reset our flag
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






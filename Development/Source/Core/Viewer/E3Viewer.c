/*  NAME:
        E3Viewer.c

    DESCRIPTION:
        Quesa viewer library implementation.

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
#include "E3Version.h"
#include "E3Viewer.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct {
	TQ3ViewObject			mView;
	TQ3GroupObject			mGroup;
	void					*mWindow;
	TQ3Area					mArea;
	TQ3Uns32				mFlags;
} TQ3ViewerData;

typedef struct {
	void					*mWindow;
	TQ3Area					*mArea;
	TQ3Uns32				mFlags;
} te3ViewerParams;



//=============================================================================
//      e3viewer_new : Viewer class new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3viewer_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3ViewerData			*instanceData  = (TQ3ViewerData *) privateData;
	te3ViewerParams			*params = (te3ViewerParams*) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->mView  = Q3View_New();
	instanceData->mGroup = Q3OrderedDisplayGroup_New();
	instanceData->mFlags = params->mFlags;
	instanceData->mWindow = params->mWindow;
	instanceData->mArea = *params->mArea;
	
	return(kQ3Success);
}





//=============================================================================
//      e3viewer_delete : Viewer class delete method.
//-----------------------------------------------------------------------------
static void
e3viewer_delete(TQ3Object theObject, void *privateData)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	E3Object_DisposeAndForget(instanceData->mView);
	E3Object_DisposeAndForget(instanceData->mGroup);
}





//=============================================================================
//      e3viewer_metahandler : Viewer class metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3viewer_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3viewer_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3viewer_delete;
			break;
		}
	
	return(theMethod);
}



//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Viewer_RegisterClass : Register the viewer class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Viewer_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the viewer class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3ObjectTypeViewer,
											kQ3ClassNameViewer,
											e3viewer_metahandler,
											sizeof(TQ3ViewerData));

	return(qd3dStatus);
}





//=============================================================================
//      E3Viewer_UnregisterClass : Unregister the viewer class.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the viewer class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3CameraTypeViewAngleAspect, kQ3True);

	return(qd3dStatus);
}

#pragma mark -



//=============================================================================
//      E3Viewer_GetVersion : Return the viewer version.
//-----------------------------------------------------------------------------
//		Note : The version numbers are not encoded in BCD as per Q3GetVersion.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{


	// Return the version
	*majorRevision = kQ3MajorVersion;
	*minorRevision = kQ3MinorVersion;

	return(kQ3Success);
}





//=============================================================================
//      E3Viewer_GetReleaseVersion : Return the viewer release version.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetReleaseVersion(TQ3Uns32 *releaseRevision)
{


	// Return the release version
	*releaseRevision = kQ3PackedVersion;

	return(kQ3Success);
}





//=============================================================================
//      E3Viewer_New : Creates a viewer.
//-----------------------------------------------------------------------------
//		Note : The window should be a CGrafPtr on MacOS, and a HWND under
//			   Win32.
//-----------------------------------------------------------------------------
TQ3ViewerObject
E3Viewer_New(const void *theWindow, const TQ3Area *theRect, TQ3Uns32 theFlags)
{	TQ3ViewerObject		theViewer;

	te3ViewerParams		paramData;
	
	// Set up initial values (to be copied into the actual Quesa object).
	paramData.mFlags = theFlags;
	paramData.mWindow = (void*)theWindow;
	paramData.mArea = (TQ3Area*)theRect;

	// Create the object
	theViewer = E3ClassTree_CreateInstance(kQ3ObjectTypeViewer, kQ3False, &paramData);
	
	return(theViewer);
}





//=============================================================================
//      E3Viewer_Dispose : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_Dispose(TQ3ViewerObject theViewer)
{
	// Dair: do we need this?
	// There's no such method in E3Group.c, for example...


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_UseFile : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UseFile(TQ3ViewerObject theViewer, TQ3Uns32 fileRef)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_UseData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UseData(TQ3ViewerObject theViewer, const void *theData, TQ3Uns32 dataSize)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_UseGroup : Set our group.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UseGroup(TQ3ViewerObject theViewer, TQ3GroupObject theGroup)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;



	// Set our group, replacing any existing group
	E3Shared_Replace(&instanceData->mGroup, theGroup);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Viewer_GetGroup : Get our group.
//-----------------------------------------------------------------------------
TQ3GroupObject
E3Viewer_GetGroup(TQ3ViewerObject theViewer)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3GroupObject		theGroup;



	// Return another reference to our group
	theGroup = Q3Shared_GetReference(instanceData->mGroup);
	
	return(theGroup);
}





//=============================================================================
//      E3Viewer_WriteFile : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_WriteFile(TQ3ViewerObject theViewer, TQ3Uns32 fileRef)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_WriteData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_WriteData(TQ3ViewerObject theViewer, void **theData, TQ3Uns32 *dataSize)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_Draw : Draw the viewer in its entirety.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_Draw(TQ3ViewerObject theViewer)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3Status status = kQ3Success;
	
	// For now, let's just do a Mac-specific hack to demonstrate
	// that we can draw something.
	CGrafPtr port = (CGrafPtr)instanceData->mWindow;
	SetPort(port);
	MoveTo(instanceData->mArea.min.x, instanceData->mArea.min.y);
	LineTo(instanceData->mArea.max.x, instanceData->mArea.max.y);
	MoveTo(instanceData->mArea.max.x, instanceData->mArea.min.y);
	LineTo(instanceData->mArea.min.x, instanceData->mArea.max.y);

	// Then, draw the content and the control strip.
	if (kQ3Success == status)
		E3Viewer_DrawContent(theViewer);
	if (kQ3Success == status)
		E3Viewer_DrawControlStrip(theViewer);
		
	return(status);
}





//=============================================================================
//      E3Viewer_DrawContent : Draw the content part of the view.
//-----------------------------------------------------------------------------
//		Note : We just inset the area a bit to account for the buttons and
//			   other frame elements, then submit the view.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_DrawContent(TQ3ViewerObject theViewer)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;

	// Not yet implemented, but let's pretend it's working.

	return(kQ3Success);
}





//=============================================================================
//      E3Viewer_DrawControlStrip : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : Simply draw each button in the control strip.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_DrawControlStrip(TQ3ViewerObject theViewer)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3Uns32			i, button;
	TQ3Status			status;
	TQ3Area				buttonRect;
	
	for (i=0; i<9; i++)
		{
		button = (kQ3ViewerFlagButtonCamera << i);
		status = E3Viewer_GetButtonRect(theViewer, button, &buttonRect);
		if (kQ3Success != status) break;
		
		// Mac-specific hack:
		{
			Rect r;
			E3Area_ToRect(&buttonRect, &r);
			SetPort((CGrafPtr)instanceData->mWindow);
			FrameRect(&r);
		}
		
		button <<= 1;
	}

	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetButtonRect : Get the bounds of the given button.
//-----------------------------------------------------------------------------
//		Note : Button is specified using new (Quesa) constants, e.g.,
//			   kQ3ViewerFlagButtonCamera.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 theButton, TQ3Area *theRect)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;

	// Hack: just define a small box per button,
	// assuming all buttons should appear.
	theRect->min.x = 0.0f;
	theRect->max.x = 32.0f;
	theRect->max.y = instanceData->mArea.max.y;
	theRect->min.y = theRect->max.y - 24.0f;
	
	if (kQ3ViewerFlagButtonCamera == theButton) return kQ3Success;

	theRect->min.x = theRect->max.x + 2.0f;
	theRect->max.x = theRect->min.x + 32.0f;
	if (kQ3ViewerFlagButtonTruck == theButton) return kQ3Success;

	theRect->min.x = theRect->max.x + 2.0f;
	theRect->max.x = theRect->min.x + 32.0f;
	if (kQ3ViewerFlagButtonOrbit == theButton) return kQ3Success;

	theRect->min.x = theRect->max.x + 2.0f;
	theRect->max.x = theRect->min.x + 32.0f;
	if (kQ3ViewerFlagButtonZoom == theButton) return kQ3Success;

	theRect->min.x = theRect->max.x + 2.0f;
	theRect->max.x = theRect->min.x + 32.0f;
	if (kQ3ViewerFlagButtonDolly == theButton) return kQ3Success;

	theRect->min.x = theRect->max.x + 2.0f;
	theRect->max.x = theRect->min.x + 32.0f;
	if (kQ3ViewerFlagButtonReset == theButton) return kQ3Success;

	theRect->min.x = theRect->max.x + 2.0f;
	theRect->max.x = theRect->min.x + 32.0f;
	if (kQ3ViewerFlagButtonOptions == theButton) return kQ3Success;


	// Invalid or unavailable button; return failure.
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetCurrentButton : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Viewer_GetCurrentButton(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_SetCurrentButton : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 theButton)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetBackgroundColor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *theColor)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetBackgroundColor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetBackgroundColor(TQ3ViewerObject theViewer, const TQ3ColorARGB *theColor)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetView : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewObject
E3Viewer_GetView(TQ3ViewerObject theViewer)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3ViewObject		theView;
	


	// Return another reference to our view
	theView = Q3Shared_GetReference(instanceData->mView);
	
	return(theView);
}





//=============================================================================
//      E3Viewer_RestoreView : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_RestoreView(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetFlags : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Viewer_GetFlags(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_SetFlags : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetFlags(TQ3ViewerObject theViewer, TQ3Uns32 theFlags)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetBounds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetBounds(TQ3ViewerObject theViewer, TQ3Area *theRect)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetBounds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetBounds(TQ3ViewerObject theViewer, const TQ3Area *theRect)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *theWidth, TQ3Uns32 *theHeight)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetDimension(TQ3ViewerObject theViewer, TQ3Uns32 theWidth, TQ3Uns32 theHeight)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetMinimumDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *theWidth, TQ3Uns32 *theHeight)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetWindow : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
void *
E3Viewer_GetWindow(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_SetWindow : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetWindow(TQ3ViewerObject theViewer, const void *theWindow)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetViewer : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewerObject
E3Viewer_GetViewer(const void *theWindow)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_GetControlStripWindow : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
void *
E3Viewer_GetControlStripWindow(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_AdjustCursor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_AdjustCursor(TQ3ViewerObject theViewer, TQ3Uns32 hPos, TQ3Uns32 vPos)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3Viewer_CursorChanged : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_CursorChanged(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetState : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Viewer_GetState(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_EditCut : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_EditCut(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_EditCopy : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_EditCopy(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_EditPaste : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_EditPaste(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_EditClear : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_EditClear(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_EditUndo : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_EditUndo(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetUndoString : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_GetUndoString(TQ3ViewerObject theViewer, char *theBuffer, TQ3Uns32 *bufferSize)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3Viewer_GetCameraCount : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cameraCount)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetCameraByNumber : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraIndex)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetCameraByView : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_EventMouseDown : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_EventMouseDown(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3Viewer_EventMouseTrack : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_EventMouseTrack(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3Viewer_EventMouseUp : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_EventMouseUp(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3Viewer_EventKeyboard : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_EventKeyboard(TQ3ViewerObject theViewer, const void *theEvent)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3Viewer_GetRendererType : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetRendererType : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetBrightness : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetBrightness(TQ3ViewerObject theViewer, float *theBrightness)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetBrightness : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetBrightness(TQ3ViewerObject theViewer, float theBrightness)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetRemoveBackfaces : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *removeBackfaces)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetRemoveBackfaces : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean removeBackfaces)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetPhongShading : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phongShading)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetPhongShading : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phongShading)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetImage : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
void *
E3Viewer_GetImage(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_GetCallbackDraw : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewerDrawCallbackMethod
E3Viewer_GetCallbackDraw(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_SetCallbackDraw : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCallbackDraw(TQ3ViewerObject theViewer, TQ3ViewerDrawCallbackMethod theCallback, const void *userData)
{


	// To be implemented...
	return(kQ3Failure);
}







//=============================================================================
//      E3Viewer_GetCallbackResize : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewerPaneResizeNotifyCallbackMethod
E3Viewer_GetCallbackResize(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_SetCallbackResize : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCallbackResize(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod theCallback, const void *userData)
{


	// To be implemented...
	return(kQ3Failure);
}





